#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef FALSE
#define FALSE (0!=0)
#endif
#ifndef TRUE
#define TRUE (0==0)
#endif

int DEBUG = FALSE;

static int exit_rc = EXIT_SUCCESS;

typedef uint32_t UINT32;
typedef int32_t SINT32;
typedef uint16_t UINT16;
typedef int16_t SINT16;
typedef uint8_t UINT8;
typedef int8_t SINT8;

UINT8 memory[0x10000], *memory_DP = &memory[0xD000];

// Modify this header to suit your target...
SINT32 res, A, B, D, C;
UINT16 PC, X, Y, S, U, Z, DP, arg, ea, val;
UINT8 E, F, I, N, H, V, CC, msn, lsn;

// CC=EFHINZVC
#define E_FLAG 0x80
#define F_FLAG 0x40
#define H_FLAG 0x20
#define I_FLAG 0x10
#define N_FLAG 0x08
#define Z_FLAG 0x04
#define V_FLAG 0x02
#define C_FLAG 0x01

int clockticks = 0;

static inline void checkDP(void) {
  // hook for specific roms eg starwars to assert that DP never points to I/O space!
}

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
  memory[addr] = value;
}

static inline unsigned int rd_mem(int addr) {
  // This is where we pull out access to device space.
  // We may want to also mirror the values in the memory[] array for convenience
  return memory[addr&0xffff]&0xff;
}

void mon(char *s) {
  fflush(stdout);
  if (DEBUG) fprintf(stderr, "A=%02x B=%02x X=%04x Y=%04x S=%04x U=%04x CC=%02x DP=%02x E%cF%cH%cI%cC%cZ%cV%cN%c   %s", A, B, X, Y, S, U, CC, (UINT8)(DP>>8), E+'0', F+'0', (((H & 0x10) != 0)?1:0)+'0', I+'0', (((C & 0x100) != 0)?1:0)+'0', ((Z != 0)?1:0)+'0', (((SINT8)V < 0)?1:0)+'0', (((SINT8)N < 0)?1:0)+'0', s);
}

int main (int argc, char **argv) {
  FILE *bin = fopen ("cputest.bin", "rb");

  if (argc == 2 && strcmp(argv[1], "-d") == 0) {
    DEBUG = TRUE; argc += 1;
  } else if (argc == 2 && strcmp(argv[1], "-d0") == 0) {
    DEBUG = FALSE; argc += 1;
  } else if (argc == 2 && strcmp(argv[1], "-d1") == 0) {
    DEBUG = TRUE; argc += 1;
  } else if (argc == 3 && strcmp(argv[1], "-d") == 0) {
    int deb, rc = sscanf(argv[2], "%d", &deb);
    if (rc == 1 && (deb == 0 || deb == 1)) DEBUG = deb;
    argc += 2;
  }
  
  X = 0x8100;
  for (;;) {
    A = fgetc (bin);
    if (A == EOF)
      break;
    memory[X++] = A;
  }
  fclose(bin);
  
  PC = 0x8100; S = 0x4000; A = B = X = Y = U = 0;
  CC = E = F = H = I = C = Z = V = N = 0;
  DP = 0; memory_DP = memory;
  
  for (;;) {
    switch (PC) {
#include "cputest.c"
    case 0xCD1E: /*PSTRING*/              // X points to string!
          LCD1E:
      if (X == 0x93A3) exit_rc = EXIT_SUCCESS;
      if (X == 0x9395) exit_rc = EXIT_FAILURE;
      printf("**************** ");
      {
        static char errmess[128];
        int s = 0;
        for (;;) {
          int i = memory[X++];
          if (i == 4)
            break;
          if (isprint (i)) {
            putchar (i);
            errmess[s++] = i; errmess[s] = '\0';
          } else
            printf ("<%02x>", i);
        }
        // putchar (10);
        printf(" **************\n");
        fflush(stdout);
      }
      PC = memory[S++] << 8;
      PC |= memory[S++];
      JUMP;

    case 0xCD03: /*WARMS*/
          LCD03:
      exit(exit_rc);
      
    default:
      fprintf (stderr, "Unknown jump to %04x\n", PC);
      exit (1);
    }
  }
}
