// If running on an ARM, try forcing commonly used 6809 registers into ARM registers?
// #define ARM_OPT 1
/*
    The basic harness for starwars is looking reasonable now.

    Bank-switching is implemented (inefficiently), but no other IO or
    peripherals.  Further development will need some actual emnulator
    knowlege.

    At some point the translator will need to trap writes to places
    other than RAM.  Not to mention the vector and sound stuff.
 */

/*

https://github.com/cavnex/mc6809/blob/master/documentation/patch.md

   The Williams Stargate subroutine at $119 contains a minor error. At
   $11D if the value of the U register is nonzero, the BNE instruction
   jumps incorrectly into the middle of the next instruction.

   [60]Williams Stargate disassembly and hex dump

   The intent is to skip over the JSR $115. Instead the code jumps to
   $120, executing $01 $15. $01 is documented as an invalid opcode,
   however the Motorola 6809 actually executes it as NEG <$15. In
   contrast, the Hitachi 6309 traps on the illegal instruction.

   The code at $11D can be patched to execute BNE $0122 instead of BNE
   $0120 as follows.

   Assuming the first Stargate ROM is a file named "01" with SHA-1 hash
   f003a5a9319c4eb8991fa2aae3f10c72d6b8e81a use the following command
   line:

          echo "11e: 03" | xxd -r - 01

   Alternately change $2601 to $2603 using a hex editor.

   End result is a file with SHA-1 hash
   6a467d3619dae915d192c3b5171064a5c273c870 which is now Hitachi 6309
   compatible.

 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef FALSE
#define FALSE (0!=0)
#endif
#ifndef TRUE
#define TRUE (0==0)
#endif

int DEBUG = TRUE;

typedef uint32_t UINT32;
typedef int32_t SINT32;
typedef uint16_t UINT16;
typedef int16_t SINT16;
typedef uint8_t UINT8;
typedef int8_t SINT8;

UINT8 memory[0x10000]; // , *memory_DP = &memory[0xD000];

// Modify this header to suit your target...
#ifdef ARM_OPT

// Not profiled at all, this is just a finger in the air guess...
// Should assign one register at a time and see which one gives
// the best result, assign it, then repeat.

register SINT32 res asm ("r10");
register SINT32 A asm ("r4");
SINT32 B; /*D,*/
register SINT32 C asm ("r5");

UINT16 PC, X, Y, S, U;
register UINT16 Z asm ("r6");
UINT16 DP, arg, ea;
register UINT16 val asm ("r7");
UINT8 E, F, I;
register UINT8 N asm ("r8");
UINT8 H;
register UINT8 V asm ("r9");
UINT8 CC, msn, lsn;
#else
// The width of these variables is important and part of the design.  Do not widen a
// byte or short to make things more efficient - you will get errors when the high
// bytes get written to unexpectedly.  Do no narrow the 32 or 16 bit variables to
// match the 6809 register sizes - the extra space is used for the carry bit and
// detecting overflow.

SINT32 res, A, B, /*D,*/ C;  // D is never used. Everything is done with A and B
UINT16 PC, X, Y, S, U, Z, DP, arg, ea, val;
// During execution of the program, the flag bits use our internal conventions -
// they are NOT always 0 or 1 and even when they are, the sense may be inverted
// from what the 6809 uses.  Flags have to be converted to canonical form when
// loaded/saved/*or printed* in diagnostics. (eg "mon()") Or when CC is compared
// in a dual CPU environment.
// EFI not fully implemented.
UINT8 E, F, I, N, H, V, CC, msn, lsn;
#endif

int clockticks = 0;

static inline void checkDP(void) {
  // hook for specific roms eg starwars to assert that DP never points to I/O space!
  // This was important earlier when we were using an optimisation to access 256-byte
  // blocks of memory via the memory_DP[] pointer, but after a code simplication where
  // that mechanism was removed, it's now not really important to separately check
  // memory accesses in DP.
}


// quick hack to get starwars to compile:
UINT8 LASTBANK;
#define BANK memory[0x4684] // This is star wars bank-switch variable...

#define E_FLAG 32

// JUMP's behaviour depends on how the main loop is written.
//#define JUMP continue
// You could also use 'break' or 'continue', and you could also invoke a procedure
// to write back all the registers to a struct.  Returning to the outer loop may
// also be where interrupts are simulated but it doesn't have to be.

#define JUMP continue
#define INSTRUCTION_START

// We use an alternative convention for flags in order to reduce the operations applied to them most frequently
// reduce flags to the individual bits in CC
#define simplify_flags() do { N = (((SINT8)N < 0)?1:0);     \
  V = (((SINT8)V < 0)?1:0);     \
  Z = ((Z == 0)?1:0);           \
  C = ((C != 0)?1:0);           \
  H = (((H & 0x10) != 0)?1:0);  \
  } while (0)

// make individual bits from CC compatible with our optimised version of the flags
#define restore_flags() do { N = (N ? 0x80:0);    \
  V = (V ? 0x80:0);    \
  Z = (Z ? 0 : 1);     \
  C = (C ? 0x100 : 0); \
  H = (H ? 0x10 : 0);  \
  } while (0)


// These must be procedures because the parameters may include ++
static inline void wr_mem(int addr, unsigned char value) {
  // This is where we pull out access to device space.
  // We may want to also mirror the values in the memory[] array for convenience
  memory[addr&0xffff] = value;
}

static inline unsigned int rd_mem(int addr) {
  // This is where we pull out access to device space.
  // We may want to also mirror the values in the memory[] array for convenience
  return memory[addr&0xffff]&0xff;
}

void mon(char *s) {
  if (DEBUG) printf("P=%02x A=%02x B=%02x X=%04x Y=%04x S=%04x U=%04x CC=%02x DP=%02x E%cF%cH%cI%cC%cZ%cV%cN%c   %s",
                    BANK, A, B, X, Y, S, U, CC, (UINT8)(DP>>8), E+'0', F+'0', (((H & 0x10) != 0)?1:0)+'0', I+'0',
                    (((C & 0x100) != 0)?1:0)+'0', ((Z != 0)?1:0)+'0', (((SINT8)V < 0)?1:0)+'0', (((SINT8)N < 0)?1:0)+'0', s);
}

// Back when memory_DP[] was a thing, I was contemplating a restructuring where instead of
// a 0x10000 array for memory, we would have an array of 256 pointers into 256 byte blocks,
// and bank switching would consist of updating 0x20 pointers rather than 0x2000 bytes.
// I've abandoned this idea for the moment because it would impose a high overhead for
// general code, as opposed to this which is expensive but seldom invoked, so overall,
// probably cheaper:
static char bank0[0x2000]; // for the moment, quick hack is to copy in entire bank on a switch!
static char bank1[0x2000];
                      
int main (int argc, char **argv) {
  int addr;
  FILE *bin = fopen ("starwars-unbanked.bin", "rb");
  addr = 0x8000;
  for (;;) {
    A = fgetc (bin);
    if (A == EOF)
      break;
    memory[addr++] = A;
  }
  fclose(bin);
  if (addr != 0x10000) {
    fprintf(stderr, "did not read all of starwars-unbanked.bin - stopped at %X\n", addr);
  }

  bin = fopen ("starwars-bank0.bin", "rb");
  addr = 0x0000; // memory:6000
  for (;;) {
    A = fgetc (bin);
    if (A == EOF)
      break;
    bank0[addr++] = A;
  }
  fclose(bin);
  if (addr != 0x2000) {
    fprintf(stderr, "did not read all of starwars-bank0.bin - stopped at %X\n", addr+0x6000);
  }
  

  bin = fopen ("starwars-bank1.bin", "rb");
  addr = 0x0000; // memory:6000
  for (;;) {
    A = fgetc (bin);
    if (A == EOF)
      break;
    bank1[addr++] = A;
  }
  fclose(bin);
  if (addr != 0x2000) {
    fprintf(stderr, "did not read all of starwars-bank1.bin - stopped at %X\n", addr+0x6000);
  }
  
  
  PC = memory[0xFFFE]<<8 | memory[0xFFFF]; S = 0; A = B = X = Y = U = 0;
  CC = E = F = H = I = C = Z = V = N = 0;
  DP = 0; // memory_DP = memory;
  LASTBANK = 0xFF;
  for (;;) {
    switch (BANK == 0 ? 0 : 1) {
    case 0:
      if (BANK != LASTBANK) {
        if (DEBUG) printf("Entering Bank0  PC=%X  memory[6044] = %02x:%02x\n", PC, memory[0x6044], memory[0x6045]);
        memcpy(&memory[0x6000], &bank0[0], 0x2000); // let's just hope memcpy is heavily optimised!
        LASTBANK = BANK;
      }
      auto void Bank0(void) {
        for (;;) {
          switch (PC) {
            #include "starwars-bank0.c"     // 6000-7FFF
            #include "starwars-unbanked.c"  // 8000-FFFF
          default:
bankswitch_check:
            if (BANK != 0) return;
            fprintf (stderr, "Unknown jump to %04x in bank0\n", PC); exit (1);
          }
        }
      }
      Bank0();
      break;
    case 1:
      if (BANK != LASTBANK) {
        if (DEBUG) printf("Entering Bank1  PC=%X  memory[6044] = %02x:%02x\n", PC, memory[0x6044], memory[0x6045]);
        memcpy(&memory[0x6000], &bank1[0], 0x2000);
        LASTBANK = BANK;
      }
      auto void Bank1(void) {
        for (;;) {
          switch (PC) {
            #include "starwars-bank1.c"     // 6000-7FFF
            #include "starwars-unbanked.c"  // 8000-FFFF  Including the common code twice is much faster than messing about at runtime
bankswitch_check:
          default:
            if (BANK == 0) return;
            fprintf (stderr, "Unknown jump to %04x in bank1\n", PC); exit (1);
          }
        }
      }
      Bank1();
      break;
    }
  }
}
