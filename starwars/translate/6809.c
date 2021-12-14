// TODO
// a) for SW assume DP = $48 - than all DP accesses can be done "extended" with the real address
//    -- add in DASM a "dat" instruction to allow DP setting for a region (this is what Vide does)
//
// b) Read and Write memory separation for cases:
// RAM
// ROM
// IO
// MEMORY
// to optimize access functions
//    -- add in DASM a "dat" instruction to allow setting for a region what kind of memory it is
//
// c) clockticks only ONCE per instruction
//
// d) clockticks at the end of instruction (before end or JMP or GOTO)
//
// e) hook for extern implementation for memory addresses
//    so SBT can be "modded" from the outside

// If checking the logic of opcode covers, the original emulator on which this
// was based is currently in 6809-fallback.c

// turn this on occasionally and make sure regression tests still working OK
// Caught by cputest:
//#define TESTING_ERROR_DETECTION_1 1
// Caught by sorenroug test (I haven't actually found a test yet that succeeds in cputest and fails in sorenroug test)
// - wait a minute... I know, use the broken TFR B,X code! (TO DO)
//#define TESTING_ERROR_DETECTION_2 1

/*

    6809.c    - cycle accurate simple 6809 cpu emulator
    Copyright (C) 2001  Arto Salmi
                        Joze Fabcic
    modification into a static binary translator
    Copyright (c) 2020  Graham Toal (Last edited $Date: 2021/06/11 22:08:14 $)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    $Log: 6809.c,v $
    Revision 1.21  2021/06/11 22:08:14  gtoal
    Adding RCS headers

*/

const char *SBT_Version = "6809.c: $Revision: 1.21 $";

#include "6809dasm.h"

extern int ALT_LABEL;
extern int ADD_INSTRUCTION_MODE;  // 0 = none, 1 = allways, 2 = only on breaks, 3 = always, PC and labels are generated
extern unsigned int F_[0x10000];

 

/*

These are the types that the run-time for the generated code uses:

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

- there are several redundant casts in the generated code which
I should remove... by default most things will be unsigned; only
a few casts to SINTx will be needed.
 */

/*
    This module generates the C translations of 6809 opcodes, as detected
    by 6809dasm.c. ( http://gtoal.com/vectrex/6809sbt/6809dasm.c.html )
    We *only* handle a single opcode here, passed to us from the disassembler.
    That keeps this code quite clean and totally separate from the mess that
    6809dasm has become! (that module probably deserves a rewrite)

    The code uses the def/use chain technique described in my Static Binary $(MAKE) -C translate -f Makefile all
    $(MAKE) -C translate/starwars -f Makefile all

    Translation HOWTO ( http://gtoal.com/sbt/ ) to reduce a lot of unnecessary
    flag setting.  However a lot more optimisation could still be done - but may
    not be worth the effort as this now looks acceptable.  (Actually it wouldn't
    be unreasonable to omit those optimisations altogether and allow the compiler
    to handle them behind the scenes, but doing them here looks better as long
    as they are done correctly.)

    The initial implementation used the HOWTO's technique of passing the def/use
    register info to 'dumpf' explicitly.  It should however be possible to infer
    that info just from the code string, with a little care being taken to catch
    dependencies on the left-hand side of assignments such as memory[X] = Y - in
    this case both X and Y are 'use' registers.  (And memory is an 'Always' def,
    if you were wondering...)  We've made small steps in that direction by using
    the R_lookup() procedure quite heavily.

    The sequence of handling statements here is marginally problematic - although
    the def/use chain eliminates redundant loads, it still leaves the problem of
    constant expression folding unhandled.  To do that properly, I think I need
    to parse the generated code strings as proper C expressions!  And then be
    very careful with what is remembered and what is forgotten when a flush() is
    executed.  I'm definitely tempted to attempt that optimisation.  Remembered
    registers may also include symbolic substitution as well as constant folding,
    so that ea = X; Y = ea becomes Y = X...  as long as 'ea' is not used again.
    Whether that needs a second layer of def/use handling or just some care in
    writing the opcode covers, remains to be seen...

    Although we don't handle Phi nodes yet (and probably never will), a lot of
    optimisation can be done locally:
    1) follow through paths from entry to a procedure until return at the same
       level, note which regs and flags are used as inputs (or not used at all)
       and avoid flushing flags which get overwritten in the procedure
    2) half-carry flag only ever used in DAA instruction, so if no DAA used,
       never flush the half-carry.  If it is used, work back to where it was
       set and as long as it is linear, keep those but drop any others.
       Possibly can be done by replacing 'maybe used half carry' bit with
       'definitely used half carry' bit in def/use flags?
    3) technique of working backwards from where flags are used may be
       useful in more places that just H/DAA...
    4) remembering of values when assigned - even symbolic, not necessarily
       for constants - might allow great simplification of comparisons and
       branches, e.g. "if (memory[S--] > 0x45) ... instead of what it does
       generate.  Might only work for ==0 != 0 at first, care needs to be
       taken with signedness for more complex tests.


These are mostly from Minestorm derivatives, reusing
some of the Minestorm internal code.

Warning: undetected bios call to e7b5 from 04a6
Warning: undetected bios call to e7d2 from 0826
Warning: undetected bios call to e8e3 from 10c6
Warning: undetected bios call to e98a from 0585
Warning: undetected bios call to ea3e from 04a1
Warning: undetected bios call to ea6d from 05e5
Warning: undetected bios call to ea9d from 0bcd
Warning: undetected bios call to eaa8 from 0bdc
Warning: undetected bios call to eacf from 12f4
Warning: undetected bios call to eaf0 from 0144

I may have solved the transitive closure problem...
When chaining backwards looking for remaining registers in def/use chain,
whenever you come to a label, that label has to be tagged with all the
places that the code could have come from. (as in orion data flow)
So instead of just going linearly backwards, you also need to go backwards
though all possible 'come from' locations as well.
However if you ever hit a location that you've already visited
during this traverse, stop. (at least for that branch) to avoid
recursion caused by loops.

Conclusion - have entire program in memory, not just
the current buffer up to a flush point.

    GT 2020/03/29
 */

/*
There are certainly a few places in this code where I've taken
some shortcuts, and implemented features with quick hacks of which
I'm not proud - but the majority of those are in places where I
knew what was needed to implement the features properly - they
just needed a much more complex infrastructure to be in place
which wasn't feasible during the early stages of development
while so much was still not working.  For those hacks, I had
plans to reimplement them properly later once a basic level
of functionality was working.

HOWEVER, one problem did surface which was caused by a lack
of foresight at the design stage, and it appears that
the temporary solutions I have tried are just making it
worse.  That is the area of pre-decremented and post-incremented
registers:

                ADDA    ,-X
                ADDA    ,--X
                ADDA    ,-Y
                ADDA    ,--Y
                ADDA    ,-U
                ADDA    ,--U
                ADDA    ,-S
                ADDA    ,--S
                ADDA    [,--X]
                ADDA    [,--Y]
                ADDA    [,--U]
                ADDA    [,--S]
                ADDA    ,X+
                ADDA    ,X++
                ADDA    ,Y+
                ADDA    ,Y++
                ADDA    ,U++
                ADDA    ,S++
                ADDA    [,X++]
                ADDA    [,Y++]
                ADDA    [,U++]
                ADDA    [,S++]

The code to handle these needs to be completely rewritten.
for any operation, the pre-decrement register operations
should be handled first; then the operation performed as
if no pre-decrement is present, then on completion, any
post-increments should be performed.

This will unfortunately generate less concise or readable
code, but that is the price that has to be paid for
correctness.  Tidying up the generated C may be done
as a post-processing pass if required, but if so, that
will necessitate a new infrastructure for handling
multiple statements over several lines, i.e. a proper
C expression parser and an AST.  So be it.

I think I've now completed this, but will leave this comment
here until I've had a little longer to look at the code and
confirm it is OK...

*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "6809.h"

extern int rom_start, rom_end; // communicate with 6809dasm.c
int HTOI(char *s) {
  int I = 0;
  while (*s != '\0') {
    int c = *s++;
    if (('0' <= c) && (c <= '9')) {
      I = (I << 4) | (c - '0');
    } else if (('a' <= c) && (c <= 'f')) {
      I = (I << 4) | (c - 'a' + 0xa);
    } else if (('A' <= c) && (c <= 'F')) {
      I = (I << 4) | (c - 'A' + 0xA);
    } else {
      fprintf(stderr, "Bad hex: %s\n", s);
    }
  }
  return I;
}


// register sizes in translated code not the same! The generated code now relies
// primarily on casting (eg (UINT8)Reg) as a substitute for masking (Reg&0xff)
// (although not 100% consistently so revisit that later)

unsigned X, Y, S, U, PC;
unsigned A, B, DP;
unsigned CC, H, N, Z, V, C;
unsigned E, F, I, EFI;
unsigned iPC, CodePTR, NextPC;

// MALBAN
void hookInstructionStart(int _iPC); 
void hookInstructionEnd(int _iPC);
//#define END_HOOK hookInstructionEnd(iPC) // MALBAN
#define END_HOOK \
      do{ \
      if (Single_Step)  \
      { \
        if (Accessible)  \
        { \
          dumpf (R_Always, 0, "PC = 0x%04X;", NextPC);  \
          hookInstructionEnd(iPC); \
          dumpf (R_Always, 0, "JUMP; // sstep");  \
        } \
        else \
        { \
          hookInstructionEnd(iPC); \
        } \
      } \
      else \
      { \
        hookInstructionEnd(iPC); \
      } \
      }while (0)

      /*
        if (Accessible)  \
        { \
          dumpf (R_Always, 0, "PC = 0x%04X;", NextPC);  \
        } \
*/
/*
Notes:
   Whenever you assign to A or B, set D = (A<<8) | B (no longer true)
   Whenever CC is read from, consolidate flags into CC
   Whenever CC is written to, split CC into flags
   Flags are lazily evaluated -
     canonicalisation is only needed when converting in or out of CC;
     tests of flag bits are done similar to "(C != 0)" or "(N < 0)"
   EFI flags are so seldom used they're kept separately in a single variable,
   for now. Though that area is under active review.

   If assignments to variables can be remembered symbolically, then
      A = mem[...]; N = A; if (N < 0) ...
   might end up being translated as
      A = mem[...]; if (A < 0) ...
   or if we're very lucky, 
      if (mem[...] < 0) ...
   which would more closely match the original intent.

   The scheme above, of keeping individual CC flags separate,  is believed
   to be more suitable to a translator which handled redundant loads/
   dead stores than one which uses lookup tables to update all the CC bits
   within a combined CC at once. (cf 6502 and z80 handling in Orion SBT)

   Eventually, constant values in registers will be remembered and folded,
     so optimising writes to PC *should* fall out in the wash and not need
     a special case handler - but initially that's what it will get.
   For now almost all PC manipulations are from values of PC known at
   translate time.  If/when we add a fallback interpreter for handling
   unknown code, some extra effort may need to be put in to the handling
   of the PC...

   Same *ought* to apply to 'clockticks' in an ideal world.  At the moment
   clockticks are pretty much ignored because the code to add clock ticks
   depending on conditional execution is quite broken.

   There are some internal variables which are handled like registers
     for optimisation purposes.  If we run out of bits (32 max for now)
     in the def/usr code, we can overlay any variables that are only used
     within a single opcode cover, i.e. written to, read from, then never
     accessed again.  For example msn and lsn in DAA, and tmp1 and tmp2 in EXG.
     However for simplicity it may just be easier to move to 64 bit 'long long's.

   ALL short opcodes that use DP need DP in the RHS of the def/use params.

 */

char *UC(char *lc) {
  int i=0; // try to copy in place, if that doesn't work create a buffer to copy into.
  while (lc[i] != '\0') {
    if ((isalpha(lc[i])) && (islower(lc[i]))) lc[i] = toupper(lc[i]);
    i++;
  }
  return lc;
}

UINT8 *memory = NULL;
UINT8 *dpmem = NULL;

#ifndef FALSE
#define FALSE (0!=0)
#define TRUE  (0==0)
#endif

#define MAX 40960
long defs[MAX], uses[MAX];
char *codes[MAX];
int nextfree = 0;

int ishex(int c) {
  return ('0' <= c && c <= '9')
      || ('A' <= c && c <= 'F')
      || ('a' <= c && c <= 'f');
}

int R_lookup(char *expr) {
  // TO DO!  '//' and '/*' ... '*/' comments in code strings
  static char varname[128];
  char *s, *v;
  int Regs = 0;
  // single letter: A B D X Y S U H N Z V C
  // two letter: PC DP CC EA 0x?????
  // three letter: val res msn lsn arg
  // multi: clockticks
  // Unless we end up parsing complete expressions, some of those above will never be seen in a call to R_lookup()
  // Need to handle both kinds of comments if they appear in 'expr'
  s = expr;
  for (;;) {
    if (*s == '\0') break;
    if ((s[0] == 'c') && (strncmp(s, "clockticks", strlen("clockticks")) == 0)) {
      Regs |= R_CLOCKTICKS; s += strlen("clockticks"); continue;
    }
    if (s[0] == '0' && s[1] == 'x' && ishex(s[2])) {
      s += 2; while (ishex(s[0])) s+=1; continue;
    } else if (!isalnum(s[1])) {
      if (s[0] == 'A') { Regs |= R_A; s += 1; continue; }
      if (s[0] == 'B') { Regs |= R_B; s += 1; continue; }
      if (s[0] == 'D') { Regs |= R_D; s += 1; continue; }
      if (s[0] == 'X') { Regs |= R_X; s += 1; continue; }
      if (s[0] == 'Y') { Regs |= R_Y; s += 1; continue; }
      if (s[0] == 'S') { Regs |= R_S; s += 1; continue; }
      if (s[0] == 'U') { Regs |= R_U; s += 1; continue; }
      if (s[0] == 'H') { Regs |= R_H; s += 1; continue; }
      if (s[0] == 'N') { Regs |= R_N; s += 1; continue; }
      if (s[0] == 'Z') { Regs |= R_Z; s += 1; continue; }
      if (s[0] == 'V') { Regs |= R_V; s += 1; continue; }
      if (s[0] == 'C') { Regs |= R_C; s += 1; continue; }
      if (s[0] == 'E') { Regs |= R_E; s += 1; continue; }
      if (s[0] == 'F') { Regs |= R_F; s += 1; continue; }
      if (s[0] == 'I') { Regs |= R_I; s += 1; continue; }
    } else if (!isalnum(s[2])) {
      if (s[0] == 'P' && s[1] == 'C') { Regs |= R_PC; s += 2; continue; }
      if (s[0] == 'D' && s[1] == 'P') { Regs |= R_DP; s += 2; continue; }
      if (s[0] == 'C' && s[1] == 'C') { Regs |= R_CC; s += 2; continue; }
      if (s[0] == 'e' && s[1] == 'a') { Regs |= R_EA; s += 2; continue; }
    }
    if (isalpha(s[0])) {
      v = varname;
      while (isalnum(*v++ = s[0])) s += 1;
      *--v = '\0';
      if (strcmp(varname, "val") == 0) {
        Regs |= R_VAL;
      } else if (strcmp(varname, "res") == 0) {
        Regs |= R_RES;
      } else if (strcmp(varname, "msn") == 0) {
        Regs |= R_MSN;
      } else if (strcmp(varname, "lsn") == 0) {
        Regs |= R_LSN;
      } else if (strcmp(varname, "arg") == 0) {
        Regs |= R_ARG;
      } else if (strcmp(varname, "clockticks") == 0) {
        Regs |= R_CLOCKTICKS;
      } else if (strcmp(varname, "memory") == 0) { // Is memory_DP handled properly yet? Should activate DP reg...
        Regs |= R_Always; // should be in the dest not the src regs...
      } else if (strcmp(varname, "UINT8") == 0) { // not relevant
      } else if (strcmp(varname, "SINT8") == 0) {
      } else if (strcmp(varname, "UINT16") == 0) {
      } else if (strcmp(varname, "SINT16") == 0) {
      } else {
        fprintf(stderr, "R_lookup: '%s' not handled in '%s'.\n", varname, expr);
      }
    } else if (*s == '\0') {
      break;
    } else s += 1; // skip anything else.
  }
  return Regs;
}
  
void dumpf(int def, int use, char *s, ...);

static int clockticks = 0, cond_clockticks = 0;
void CK (int ticks)
{
//  if (Do_Timing) dumpf(R_CLOCKTICKS, R_CLOCKTICKS, "clockticks = clockticks + %d;", ticks);
  if (Do_Timing) dumpf(R_Always|R_CLOCKTICKS, R_CLOCKTICKS, "clockticks = clockticks + %d;", ticks);
}

/* conditional_clockticks would do a dumpf to set clockticks equal
to the sum of clockticks plus the parameter but doesn't update
clockticks itself.  For use in conditional branches.
Ideally delta-clockticks chain starts afresh on each basic block
then gets added to absolute clockticks at end of BB (RTS, JUMPs etc)
"To do..." */

void conditional_clockticks (int ticks)
{
   cond_clockticks += ticks;
}

void flush(long regs)
{
   int i;
   long common_regs;

   for (i = nextfree - 1; i >= 0; --i) {
     if ((common_regs = (regs & defs[i])) != 0) {
        /* if bits in 'defs' then this needs to be dumped. */
        // fprintf(stdout, "// %02d lookfor: %02x Item %d - def %02x\n", nextfree, regs, i, defs[i]);
         defs[i] |= R_Always;   /* flag this line for printing */
         regs &= ~common_regs;  /* don't need to look for these ones any more */
      }
      if (defs[i] & R_Always)
         regs |= uses[i];       /* Any unconditionals + parents */
   }

   fflush(stdout);
   for (i = 0; i < nextfree; i++) {
#ifdef DEBUG
      fprintf (stdout, "/* %03d [def:%04x] [use:%04x] */", i, defs[i], uses[i]);
#endif
      if (Gen_C) {
        if (strncmp(codes[i], "case ", 5) == 0) {
          fprintf (stdout, "  %s%s\n", (defs[i] & R_Always ? "" : " // "), codes[i]);
        } else if ((defs[i] & R_Always) == 0) {


	  
//if (
//       (strstr(codes[i], "N = ")) 
//  )
//            fprintf (stdout, "/**/%s\n", codes[i]); // we can't optimise in a dual cpu situation...
//else
//            fprintf (stdout, " // %s\n", codes[i]);
	    
//fprintf (stdout, "/**/%s\n", codes[i]); // we can't optimise in a dual cpu situation...

	  if (Single_Step) {
            fprintf (stdout, "/**/%s\n", codes[i]); // we can't optimise in a dual cpu situation...
          } else {
            fprintf (stdout, " // %s\n", codes[i]);
          }
          
          
        } else if (strncmp(codes[i], "     ", 5) == 0) {
          fprintf (stdout, "  %s\n", codes[i]);
        } else {
          fprintf (stdout, "    %s\n", codes[i]);
        }
      } else {
        fprintf (stdout, "%s%s\n", (defs[i] & R_Always ? "" : " // "), codes[i]);
      }
      /* should free strings here... */
   }
   fflush(stdout);
   nextfree = 0;
}

void unconditional_flush(char *inst)
{
   flush(R_FLUSH);
   fprintf (stdout, "          clockticks += %d; /*poll_nmi(opadd);*/\n", clockticks);
   clockticks = 0; cond_clockticks = 0;
   fprintf (stdout, "          %s\n", inst);
   nextfree = 0;
}

void conditional_flush(char *inst)
{
   flush(R_FLUSH);
   if (cond_clockticks) fprintf (stdout, "              clockticks += %d;\n", clockticks + cond_clockticks);
   // TO DO: fprintf (stdout, "              poll_nmi(opadd); /* if (...) return to dispatch loop - PC is address of NMI proc - real PC is on stack */\n");
   cond_clockticks = 0;
   fprintf (stdout, "          %s\n", inst);
   nextfree = 0;
}

static char *reg = NULL;
static int post_increment = 0;
void set_post(char *r, int incr) {
  reg = r; post_increment = incr;
}
void post(void) { // output the delayed post-increment for indexed() arguments.
                  // pre-decrements have already been handled more sneakily.
  if (reg && post_increment) dumpf(R_Always|R_lookup(reg), R_lookup(reg), "%s = %s + %d;", reg, reg, post_increment);
  reg = NULL; post_increment = 0;
}

signed short SEX8(UINT8 tmp);

unsigned int imm_byte(void)
{
  return memory[NextPC-1];
}

char *imm_byte_s(void)
{
  static char tmp[16];
  sprintf (tmp, "(SINT8)0x%02x", (int8_t)memory[NextPC-1]&0xff);
  return strdup (tmp);
}

int imm_word(void)
{
  return (memory[NextPC-2] << 8) | memory[NextPC-1];
}

char *imm_word_s(void)
{
  static char tmp[7];
  sprintf (tmp, "0x%04x", ((memory[NextPC-2] << 8) | memory[NextPC-1])&0xffff);
  return strdup (tmp);
}

void
push_stack8(char *stack, char *data)
{
// MALBAN
//  dumpf(R_Always|R_lookup(stack), R_lookup(stack)|R_lookup(data), "memory[--%s] = %s;", stack, data);
  dumpf(R_Always|R_lookup(stack), R_lookup(stack), "--%s;", stack);
  dumpf(R_Always, R_lookup(stack)|R_lookup(data), "memory[%s] = %s;", stack, data);
  CK (1);
}

void
pop_stack8(char *stack, char *dest) // stack adjustment done by caller
{
  CK (1);
// MALBAN
// dumpf(R_Always|R_lookup(dest)|R_lookup(stack), R_lookup(stack), "%s = memory[%s++];", dest, stack);
  dumpf(R_Always|R_lookup(dest), R_lookup(stack), "%s = memory[%s];", dest, stack);
  dumpf(R_Always|R_lookup(stack), R_lookup(stack), "%s++;",stack);
}

char *
RDMEM(char *addr)
{
  static char tmp[256];
  sprintf (tmp, "memory[%s]", addr); // bypassing the use/def chain?????
  return strdup (tmp);
}

char *
RDMEM16(char *addr)
{
  //static char tmp[256];
  CK (1);
  if (strncmp(addr, "DP|", 3) == 0) {
    // this is ugly and we unpick it later which is a dirty hack.
    addr = addr+strlen("DP|");
    if (strncmp(addr, "(UINT16)", strlen("(UINT16)")) == 0) {
      addr[5] = '8';addr[6] = ')';addr[7] = ' ';
    }
    if (strncmp(addr, "0x", 2) == 0) {
      dumpf(R_EA|R_Always, R_DP, "ea = memory[DP|0x%02x]<<8;", HTOI(addr+2));
      dumpf(R_EA|R_Always, R_EA|R_DP, "ea = ea | memory[DP|0x%02x];", (SEX8(HTOI(addr+2))+1)&0xff);
    } else {
      dumpf(R_EA|R_Always, R_DP|R_lookup(addr), "ea = memory[DP|%s]<<8;", addr);
      dumpf(R_EA|R_Always, R_EA|R_DP|R_lookup(addr), "ea = ea | memory[DP|(UINT8)((SINT8)%s + 1))];", addr);
    }
  } else {
    if (strncmp(addr, "0x", 2) == 0) {
      dumpf(R_EA|R_Always, 0, "ea = memory[0x%04x]<<8;", HTOI(addr+2));
      dumpf(R_EA|R_Always, R_EA, "ea = ea | memory[0x%04x];", HTOI(addr+2)+1);
    } else {
      dumpf(R_EA|R_Always, R_lookup(addr), "ea = memory[%s]<<8;", addr);
      dumpf(R_EA|R_Always, R_EA|R_lookup(addr), "ea = ea | memory[(UINT16)((%s)+1)];", addr);
    }
    // sprintf (tmp, "((memory[%s]<<8)|memory[(UINT16)((%s)+1)])", addr, addr);
  }
  return "ea";
}

void
push_stack16(char *stack, char *data)
{                                                                 // memory is a byte anyway. so casting not needed
// MALBAN
//  dumpf(R_Always|R_lookup(stack), R_lookup(stack)|R_lookup(data), "memory[--%s] = %s;", stack, data);
//  dumpf(R_Always|R_lookup(stack), R_lookup(stack)|R_lookup(data), "memory[--%s] = %s >> 8;", stack, data);
  dumpf(R_Always|R_lookup(stack), R_lookup(stack), "--%s;", stack);
  dumpf(R_Always, R_lookup(stack)|R_lookup(data), "memory[%s] = %s;", stack, data);
  dumpf(R_Always|R_lookup(stack), R_lookup(stack), "--%s;", stack);
  dumpf(R_Always, R_lookup(stack)|R_lookup(data), "memory[%s] = %s >> 8;", stack, data);
  CK (1);
}

void
pop_stack16(char *stack, char *dest) // stack adjustment done by caller
{
  CK (1);
// MALBAN
//  dumpf(R_Always|R_lookup(dest)|R_lookup(stack),  R_lookup(stack),                "%s = memory[%s++]<<8;", dest, stack);
//  dumpf(R_Always|R_lookup(dest)|R_lookup(stack),  R_lookup(stack)|R_lookup(dest), "%s = %s | memory[%s++];", dest, dest, stack);
  dumpf(R_Always|R_lookup(dest),  R_lookup(stack),                "%s = memory[%s]<<8;", dest, stack);
  dumpf(R_Always|R_lookup(stack),  R_lookup(stack),                "%s++;", stack);
  dumpf(R_Always|R_lookup(dest),  R_lookup(stack)|R_lookup(dest), "%s = %s | memory[%s];", dest, dest, stack);
  dumpf(R_Always|R_lookup(stack),  R_lookup(stack), "%s++;", stack);
}

char *
direct(void)
{
  static char tmp[16];
  sprintf (tmp, "DP|0x%02x", (memory[NextPC-1])&0xff);
  return strdup (tmp);
}

char *get_d (void);

char *f(char *s, ...)
{
   static FILE *nullfile = NULL;
   static char *buff = NULL;
   static int bufflen = 0;
   va_list ap;
   
   if (nullfile == NULL) {
      nullfile = fopen ("/dev/null", "w");
      if (nullfile == NULL) {
         fprintf (stderr, "Major error - cannot open /dev/null\n");
         exit (1);
      }
   }
   if (buff == NULL) {
      buff = malloc (bufflen = 256);
      if (buff == NULL) exit (2);              // if malloc failed so will printf!
   }
   {
      int string_length;

      va_start (ap, s);
      string_length = vfprintf (nullfile, s, ap);
      va_end (ap);

      if (string_length + 1 >= bufflen) {
         free (buff);
         buff = malloc (bufflen = 2 * string_length);
         if (buff == NULL) exit (3);
      }
      va_start (ap, s);
      //vsprintf (buff, s, ap);
      vsnprintf (buff, 0x7FFFFFFF, s, ap);
      va_end (ap);
   }

   return strdup (buff);
}

char *
indexed (void)          /* note take 1 extra cycle */
{
  char *regname[4] = { "X", "Y", "U", "S" };
  unsigned post = memory[CodePTR]; // imm_byte ();
  char *R = regname[(post >> 5) & 0x3];

  if (post & 0x80)
    {
      switch (post & 0x1f)
        {

        case 0x00:          set_post(R, 1); CK (6); return R;
        case 0x01:          set_post(R, 2); CK (7); return R;

        case 0x02:          CK (6); dumpf(R_Always|R_lookup(R), R_lookup(R),     "%s = %s - 1;", R, R); return R;
        case 0x03:          CK (7); dumpf(R_Always|R_lookup(R), R_lookup(R),     "%s = %s - 2;", R, R); return R;

        case 0x04:          CK (4); return R;
        case 0x05:          CK (5); return f("(UINT16)(%s + (SINT8)B)", R); // need to check hardware manual as to whether A or B are signed
        case 0x06:          CK (5); return f("(UINT16)(%s + (SINT8)A)", R);
        case 0x08:          CK (5); return f("(UINT16)(%s + %s)", R, imm_byte_s()); // TO DO: UNSIGNED OR SIGNED???          
        case 0x09:          CK (8); return f("(UINT16)(%s + %s)", R, imm_word_s());
        case 0x0b:          CK (8); return f("(UINT16)(%s + %s)", R, get_d());
        case 0x0c:          CK (5); return f("0x%04x", (NextPC+SEX8(imm_byte()))&0xffff);
        case 0x0d:          CK (9); return f("0x%04x", (NextPC+imm_word())&0xffff);

        case 0x11:          set_post(R, 2); CK (7); CK (2); return RDMEM16(R);
        case 0x13:          CK (7); dumpf(R_Always|R_lookup(R), R_lookup(R),     "%s = %s - 2;", R, R); CK (2); return RDMEM16(R);

        case 0x14:          CK (4); CK (2); return RDMEM16(R);

        case 0x15:          CK (5); CK (2);
                            dumpf(R_EA|R_Always, R_lookup(R)|R_B, "ea = memory[(UINT16)(%s + (SINT8)B)]<<8;", R);
                            dumpf(R_EA|R_Always, R_EA|R_lookup(R)|R_B, "ea = ea | memory[(UINT16)(%s + (SINT8)B + 1)];", R); // wrap when B = 255, 0x7f, 0x80???
                            return "ea";
                            
        case 0x16:          CK (5); CK (2);
                            dumpf(R_EA|R_Always, R_lookup(R)|R_A, "ea = memory[(UINT16)(%s + (SINT8)A)]<<8;", R);
                            dumpf(R_EA|R_Always, R_EA|R_lookup(R)|R_A, "ea = ea | memory[(UINT16)(%s + (SINT8)A + 1)];", R); // wrap when A = 255, 0x7f, 0x80???
                            return "ea";
                            
        case 0x18:          CK (5); CK (2); // I HOPE THIS DOES NOT WRAP WITHIN 256 BYTE BLOCK!
                            dumpf(R_EA|R_Always, R_lookup(R), "ea = memory[(UINT16)(%s + (SINT8)%s)]<<8;", R, imm_byte_s());
                            dumpf(R_EA|R_Always, R_EA|R_lookup(R), "ea = ea | memory[(UINT16)(%s + 0x%04x)];", R, (SEX8(imm_byte())+1)&0xffff);
                            assert(strcmp(R, "PC") != 0);
                            return "ea";
                            
        case 0x19:          CK (8); CK (2);
                            dumpf(R_EA|R_Always, R_lookup(R), "ea = memory[(UINT16)(%s + %s)]<<8;", R, imm_word_s());
                            dumpf(R_EA|R_Always, R_EA|R_lookup(R), "ea = ea | memory[(UINT16)(%s + 0x%04x)];", R, (imm_word()+1)&0xffff);
                            assert(strcmp(R, "PC") != 0);
                            return "ea";
                            
        case 0x1b:          CK (8); CK (2);
                            dumpf(R_EA|R_Always, R_lookup(R)|R_A|R_B, "ea = memory[(UINT16)(%s + %s)]<<8;", R, get_d());
                            dumpf(R_EA|R_Always, R_EA|R_lookup(R)|R_A|R_B, "ea = ea | memory[(UINT16)(%s + %s + 1)];", R, get_d());
                            assert(strcmp(R, "PC") != 0);
                            return "ea";

        case 0x1c:          CK (5); CK (2);
                            dumpf(R_EA|R_Always, 0, "ea = memory[0x%04x]<<8;", (NextPC+SEX8(imm_byte()))&0xffff);
                            dumpf(R_EA|R_Always, R_EA, "ea = ea | memory[0x%04x];", (NextPC+SEX8(imm_byte())+1)&0xffff);
                            return "ea";
                            
        case 0x1d:          CK (9); CK (2);
                            dumpf(R_EA|R_Always, 0, "ea = memory[0x%04x]<<8;", (NextPC+imm_word())&0xffff);
                            dumpf(R_EA|R_Always, R_EA, "ea = ea | memory[0x%04x];", (NextPC+imm_word()+1)&0xffff);
                            return "ea";

        case 0x1f:          CK (6); CK (2); return RDMEM16(imm_word_s());                                  

        default:            printf ("%X: invalid index post $%02X\n", iPC, post); exit(1);
        }
    } else {
      if (post & 0x10) post |= 0xfff0; else post &= 0x000f;
      CK (5);
      return f("(UINT16)(%s + 0x%04x)", R, post&0xffff);
    }
  assert(__LINE__);
  return ("ea"); /* BUG??? */
}

char *
extended (void)
{
  static char addr[16];
  // Since this is translating known code at a known fixed address, these can be folded at translate time...
  // (if there was any code in ram, this would not catch it...)
  sprintf(addr, "0x%04x", ((memory[NextPC-2]<<8) | memory[NextPC-1])&0xffff);
  return(strdup(addr));  // or put 0xnnnn in the return string...
}

//void Make_D_from_A_and_B(void) {
//  dumpf(R_D, R_AB, "D = (A << 8) | B;");
//}

char *
get_d(void)
{
  return "(((UINT8)A<<8)|(UINT8)B)";
}

//void Get_A_and_B_from_D(void) { // is this still used?  I think I now use A and B everywhere.
//  dumpf(R_A, R_D, "A = (UINT8)(D >> 8);");
//  dumpf(R_B, R_D, "B = (UINT8) D & 0xff;");
//}

void
set_d(char *val)
{
  assert(__LINE__);
  dumpf(R_Always|R_A, R_lookup(val), "A = (UINT8)(%s >> 8);", val);
  dumpf(R_Always|R_B, R_lookup(val), "B = (UINT8)%s;", val);
}

/* handle condition code register */

void
CC_Reg_from_Individual_Flags(void)
{
  //unsigned res = EFI & (E_FLAG | F_FLAG | I_FLAG);
  dumpf(R_Always|R_HNZVC|R_F|R_I, R_HNZVC|R_F|R_I, "simplify_flags();"); // canonicalise flags to all 0 or 1
  dumpf(R_Always|R_CC, R_HNZVC|R_F|R_I, "CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; // Placeholder for now");
  dumpf(R_Always|R_HNZVC|R_F|R_I, R_HNZVC|R_F|R_I, "restore_flags();"); // this is excessive but works and well tested
}

void
Individual_Flags_from_CC_Reg (void)
{
  dumpf(R_Always|R_HNZVC|R_F|R_I, R_CC, "C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1;");
  dumpf(R_Always|R_HNZVC|R_F|R_I, R_HNZVC|R_F|R_I, "restore_flags();");
}

char *
get_reg (unsigned nro)
{
  switch (nro)
    {
    case 0:      return "((A << 8) | B)";      break;
    case 1:      return "X";                   break;
    case 2:      return "Y";                   break;
    case 3:      return "U";                   break;
    case 4:      return "S";                   break;
    case 5:      return "PC";                  break;
    case 8:      return "A";                   break;
    case 9:      return "B";                   break;
    case 10:     CC_Reg_from_Individual_Flags ();
                 return "CC";                  break;
    case 11:     return "(UINT8)(DP >> 8)";    break;
    default:     printf ("%X: invalid register code $%02X\n", iPC, nro); exit(1);
    }
}

void
set_reg (unsigned nro, char *val)
{
  switch (nro)
    {
    case 0:      dumpf(R_Always|R_A,  R_lookup(val), "A = (UINT8)((%s) >> 8);", val);
                 if (
                      ((strncmp(val, "(SINT8)", strlen("(SINT8)"))) == 0)
                      ||
                      ((strncmp(val, "(UINT8)", strlen("(UINT8)"))) == 0)
                     ) {
                   dumpf(R_Always|R_B,  R_lookup(val), "B = (UINT8)%s;", val+strlen("(SINT8)"));
                 } else {
                   dumpf(R_Always|R_B,  R_lookup(val), "B = (UINT8)%s;", val);
                 }
                 break;
    case 1:      dumpf(R_Always|R_X,  R_lookup(val), "X = %s;", val);              break;
    case 2:      dumpf(R_Always|R_Y,  R_lookup(val), "Y = %s;", val);              break;
    case 3:      dumpf(R_Always|R_U,  R_lookup(val), "U = %s;", val);              break;
    case 4:      dumpf(R_Always|R_S,  R_lookup(val), "S = %s;", val);              break;
    case 5:      dumpf(R_Always|R_PC, R_lookup(val), "PC = %s; // check for missing JUMP", val);             break;
    case 8:      dumpf(R_Always|R_A,  R_lookup(val), "A = %s;", val);              break;
    case 9:      dumpf(R_Always|R_B,  R_lookup(val), "B = %s;", val);              break;
    case 10:     dumpf(R_Always|R_CC, 0,             "CC = %s;", val);
                 Individual_Flags_from_CC_Reg();                          break;
    case 11:     dumpf(R_Always|R_DP, R_lookup(val), "DP = (%s) << 8; checkDP();", val);
                 dumpf(R_Always, R_DP,      "// memory_DP = &memory[DP];");      break;
    default:     printf ("%X: invalid register code $%02X\n", iPC, nro); exit(1);
    }
}

/* 8-Bit Accumulator and Memory Instructions */


void peephole(char *code) {
  // everything goes through here.  Maybe we can fix up some problems...

  // NOTE!!! The peephole procedure is TEMPORARY in this form and basically a
  // dirty hack.  This is one of those 'pay no attention to the man behind the curtain'
  // procedures, for now.

  char *assignment, *lhs, *rhs, *commentp, *comment, *s = strdup(code);
  if (s[strlen(s)-1] == '\n') s[strlen(s)-1] = '\0'; 
  commentp = strstr(s, "//"); // TO DO: change /* to //  If fixed, could rely on " //"
  if (commentp) {
    comment = strdup(commentp);
    if (comment[strlen(comment)-1] == '\n') comment[strlen(comment)-1] = '\0'; 
    *commentp = '\0';
  } else comment = "";

  assignment = strstr(s, " = "); // and not in a string
  if (assignment) {
    *assignment = '\0';
    lhs = s;
    rhs = assignment + 3;
    // we need a minimal lexer here
    while (*lhs == ' ') lhs += 1;
    while (*rhs == ' ') rhs += 1;
    while (lhs[strlen(lhs)-1] == ' ') lhs[strlen(lhs)-1] = '\0';
    while (rhs[strlen(rhs)-1] == ' ') rhs[strlen(rhs)-1] = '\0';
    if (rhs[strlen(rhs)-1] == ';') rhs[strlen(rhs)-1] = '\0';
    if (strstr(lhs, "memory[") && strstr(rhs, "memory[")) {
      assert(__LINE__); // should not have memory on both sides of assignment (even for 'inc mem', although inc may need special handling)
    } else if (strstr(lhs, "memory[")) {
      if (strncmp(lhs, "memory[", strlen("memory[")) == 0) {
        lhs += strlen("memory[");
        if (lhs[strlen(lhs)-1] == ']') {
          lhs[strlen(lhs)-1] = '\0';
        } else {
          assert(__LINE__);
        }
        s = f("wr_mem(%s, %s); %s", lhs, rhs, comment);
      } else {
        assert(__LINE__);
      }
    } else if ((s = strstr(rhs, "memory[")) != NULL) {
      while ((s = strstr(rhs, "memory[")) != NULL) { // handle nested memory[memory[]] expressions
        int depth = 0;
        memmove(s, "rd_mem(", strlen("memory["));
                // "memory["
        while (*s != '\0') {
        if (*s == '[') depth += 1;
          if (*s == ']') {
            if (depth == 0) {
              *s = ')';
              break;
            }
            depth -= 1;
          }
          s += 1;
        }
      }
      //if (s[strlen(s)-1] == '\n') s[strlen(s)-1] = '\0'; 
      s = f("%s = %s; %s", lhs, rhs, comment);
    } else {
      //if (s[strlen(s)-1] == '\n') s[strlen(s)-1] = '\0'; 
      s = f("%s = %s; %s", lhs, rhs, comment);
    }
    //if (*s == '\0') return;
    if ((lhs=strstr(s, "(UINT8)(SINT8)")) != NULL) {
      // remove the unneccessaty (SINT8)
      memmove(lhs+strlen("(UINT8)"), lhs+strlen("(UINT8)(SINT8)"), strlen(lhs+strlen("(UINT8)(SINT8)"))+1);
    }
    codes[nextfree] = s;
  } else {
    // procedure call, label, or something
    //if (*code == '\0') return;
    codes[nextfree] = strdup(code);
  }

  nextfree += 1;
}


/* WARNING: There are two different kinds of 64 bit architectures - both support 64 bit
            integers, but one of them also defaults to 64 bit pointers whereas the other
            doesn't.  On one of them (I'm not sure which but probably the one with 64 bit
            pointers) the varargs code below fails - most likely because the final parameter
            before the "..." is a pointer, so all subsequent parameters will also be
            pointer-sized but in fact are not.  I have not yet fixed this.
*/

void dumpf(int def, int use, char *s, ...)                               /* Works like printf */
{
   static FILE *nullfile = NULL;
   static char *buff = NULL;
   static int bufflen = 0;
   va_list ap;
   
   if (nullfile == NULL) {
      nullfile = fopen ("/dev/null", "w");
      if (nullfile == NULL) {
         fprintf (stderr, "Major error - cannot open /dev/null\n");
         exit (1);
      }
   }
   if (buff == NULL) {
      buff = malloc (bufflen = 256);
      if (buff == NULL)
         exit (2);              // if malloc failed so will printf!
   }

   {
   int string_length;

      va_start (ap, s);
      string_length = vfprintf (nullfile, s, ap);
      va_end (ap);

      if (string_length + 1 >= bufflen) {
         free (buff);
         buff = malloc (bufflen = 2 * string_length);
         if (buff == NULL)
            exit (3);
      }
      va_start (ap, s); 
     //vsprintf (buff, s, ap);
      vsnprintf (buff, 0x7FFFFFFF, s, ap);
      va_end (ap);
   }
   defs[nextfree] = def;
   uses[nextfree] = use;

   peephole(buff); // should put indent in front of statement? Nah - just use gnu indent...
}

void
adc(char *arg, char *val)  // arg is a register name, val is a data value or memory location
{
  dumpf(R_ARG,             R_lookup(arg),                         "arg = %s;", arg);
  dumpf(R_VAL,             R_lookup(val),                         "val = %s;", val);
  dumpf(R_RES,             R_ARG|R_VAL|R_C,                       "res = arg + val + (C != 0 ? 1:0);");
  dumpf(R_C,               R_RES,                                 "C = (res >> 1) & 0x80;");
  dumpf(R_Z,               R_RES,                                 "Z = (UINT8)res;");
  dumpf(R_N,               R_RES,                                 "N = (UINT8)res;");
// TODO Malban
// That half carry is not correct
  dumpf(R_H,               R_ARG|R_VAL|R_RES|R_C,                 "H = arg ^ val ^ (UINT8)res ^ C;");
  dumpf(R_V,               R_ARG|R_VAL|R_RES|R_C,                 "V = arg ^ val ^ (UINT8)res ^ C;");
  dumpf(R_Always|R_lookup(arg), R_RES,                            "%s = (UINT8)res;", arg);
}

void
add(char *arg, char *opd)
{
  //    DEF                USE                          CODE
  if (strncmp(opd, "(SINT8)", strlen("(SINT8)")) == 0) opd = opd+strlen("(SINT8)"); //GT: bugfix?
  dumpf(R_VAL,             R_lookup(arg),               "val = (UINT8)%s;", arg);
  dumpf(R_ARG,             R_lookup(opd),               "arg = %s;", opd);
  dumpf(R_RES,             R_VAL|R_ARG,                 "res = val + (UINT8)arg;");
#ifndef TESTING_ERROR_DETECTION_1
  // if defined, we don't generate this line... which should cause a wrong result
  dumpf(R_C,               R_RES,                       "C = (res >> 1) & 0x80;");
#endif
  dumpf(R_Z,               R_RES,                       "Z = (UINT8)res;");
  dumpf(R_N,               R_RES,                       "N = (UINT8)res;");
// TODO Malban
// That half carry is not correct
// there should be a couple of >> 4 in there or so
  dumpf(R_H,               R_VAL|R_ARG|R_RES|R_C,       "H = (UINT8)(val ^ arg ^ res ^ C);");
  dumpf(R_V,               R_VAL|R_ARG|R_RES|R_C,       "V = (UINT8)(val ^ arg ^ res ^ C);");
  dumpf(R_Always|R_lookup(arg), R_RES,                  "%s = (UINT8)res;", arg);
}

void
and(char *arg, char *opd)
{
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(arg),               "val = (UINT8)%s;", arg);
  dumpf(R_RES,             R_VAL|R_lookup(opd),         "res = val & %s;", opd);
  dumpf(R_Always|R_lookup(arg),    R_RES,               "%s = res;", arg);
  dumpf(R_Z,               R_RES,                       "Z = res;");
  dumpf(R_N,               R_RES,                       "N = res;");
  dumpf(R_V,               0,                           "V = 0;");
}

void
asl(char *arg)          /* same as lsl */
{
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(arg),               "val = (UINT8)%s;", arg);
  dumpf(R_RES,             R_VAL,                       "res = val << 1;");
  dumpf(R_C,               R_RES,                       "C = res & 0x100;");
  dumpf(R_RES,             R_RES,                       "res = (UINT8)res;");
  dumpf(R_Z,               R_RES,                       "Z = res;");
  dumpf(R_N,               R_RES,                       "N = res;");
  dumpf(R_V,               R_VAL | R_RES,               "V = val ^ res;");
  dumpf(R_Always|R_lookup(arg), R_RES,                  "%s = res;", arg);

  CK (2);
}

void
asr(char *arg)
{
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(arg),               "val = (UINT8)%s;", arg);
  dumpf(R_C,               R_VAL,                       "C = val & 1;");
  dumpf(R_VAL,             R_VAL,                       "val = (UINT8)((SINT8)val >> 1);");
  dumpf(R_Always|R_lookup(arg),     R_VAL,              "%s = val;", arg);
  dumpf(R_Z,               R_VAL,                       "Z = val;");
  dumpf(R_N,               R_VAL,                       "N = val;");

  CK (2);
}

void
bit(char *arg, char *val)
{
  //    DEF                USE                          CODE
  dumpf(R_RES,             R_lookup(arg)|R_lookup(val), "res = %s & %s;", arg, val);
  dumpf(R_Z,               R_RES,                       "Z = res;");
  dumpf(R_N,               R_RES,                       "N = res;");
  dumpf(R_V,               0,                           "V = 0;");
}

void
clr(char *arg)
{
  //    DEF                USE                          CODE
  if (strstr(arg, "memory") != NULL) { // can be a reg *OR* memory!
    // better to use this?  if (strncmp(arg, "memory", strlen("memory")) == 0) {  // TO DO after checking
    dumpf(R_Always,        R_lookup(arg),               "%s = 0;", arg);
                           // any regs given are in index of memory addr and therefore USEs not DEFs 
  } else {
    dumpf(R_Always|R_lookup(arg),   0,                  "%s = 0; // SEARCH_ME", arg);
  }
  dumpf(R_V,               0,                           "V = 0;");
  dumpf(R_Z,               0,                           "Z = 0;");
  dumpf(R_N,               0,                           "N = 0;");
  dumpf(R_C,               0,                           "C = 0;");

  CK (2);
}

void
cmp(char *arg, char *opd)
{
  //    DEF                USE                          CODE

  if (strncmp(opd, "(SINT8)", strlen("(SINT8)")) == 0) opd = opd+strlen("(SINT8)"); //GT: bugfix?
  dumpf(R_VAL,             R_lookup(arg),               "val = (UINT8)%s;", arg);
  dumpf(R_ARG,             R_lookup(opd),               "arg = %s;", opd);
  dumpf(R_RES,             R_VAL|R_ARG,                 "res = val - (UINT8)arg;");
  dumpf(R_C,               R_RES,                       "C = res & 0x100;");
  dumpf(R_Z,               R_RES,                       "Z = (UINT8)res;");
  dumpf(R_N,               R_RES,                       "N = (UINT8)res;");
  dumpf(R_V,               R_VAL|R_ARG|R_RES,           "V = (UINT8)((val ^ arg) & (val ^ res));");
}

void
com(char *arg)
{
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(arg),               "val = (UINT8)%s ^ 0xff;", arg);
  dumpf(R_Always|R_lookup(arg), R_VAL,                  "%s = val;", arg);
  dumpf(R_Z,               R_VAL,                       "Z = val;");
  dumpf(R_N,               R_VAL,                       "N = val;");
  dumpf(R_V,               0,                           "V = 0;");
  dumpf(R_C,               0,                           "C = 1;");

  CK (2);
}

void
daa (void)
{
    //    DEF                USE                          CODE
  dumpf(R_RES,             R_A,                         "res = A;");
  dumpf(R_MSN,             R_RES,                       "msn = res & 0xf0;");
  dumpf(R_LSN,             R_RES,                       "lsn = res & 0x0f;");

  // Yes, these must be '+', not '|'...

  dumpf(R_RES,             R_RES | R_LSN | R_H,         "res = res + (((lsn > 0x09) || (H & 0x10)) ? 0x06 : 0);");
  // msn and lsn may already be UINT8 ?
  dumpf(R_RES,             R_RES | R_MSN | R_LSN | R_C, "res = res + ((((UINT8)msn > 0x80U) && ((UINT8)lsn > 0x09U)) || (((UINT8)msn > 0x90U) || (C != 0)) ? 0x60 : 0);");

  dumpf(R_C,               R_C | R_RES,                 "C = C | (res & 0x100);");
  dumpf(R_RES,             R_RES,                       "res = (UINT8)res;");
  dumpf(R_Always|R_A,      R_RES,                       "A = res;");
  dumpf(R_N,               R_RES,                       "N = res;");
  dumpf(R_Z,               R_RES,                       "Z = res;");
  dumpf(R_V,               0,                           "V = 0;");

  CK (2);
}


void
dec(char *arg)
{
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(arg),               "val = (UINT8)%s;", arg);
  dumpf(R_RES,             R_VAL,                       "res = (UINT8)(val - 1);");
  dumpf(R_N,               R_RES,                       "N = res;");
  dumpf(R_Z,               R_RES,                       "Z = res;");
  dumpf(R_V,               R_RES | R_VAL,               "V = val & ~res;");
  dumpf(R_Always|R_lookup(arg), R_RES,                  "%s = res;", arg);
  CK (2);
}


void
eor(char *arg, char *val)
{
  //    DEF                USE                          CODE
  dumpf(R_RES,             R_lookup(arg)|R_lookup(val), "res = %s ^ %s;", arg, val);
  dumpf(R_Always|R_lookup(arg),     R_RES,              "%s = res;", arg);
  dumpf(R_Z,               R_RES,                       "Z = res;");
  dumpf(R_N,               R_RES,                       "N = res;");
  dumpf(R_V,               0,                           "V = 0;");
}

/*
## TFR/EXG with unaligned register sizes

tfr x,b         ; 6809,6309: b low byte of x, a unchanged
tfr x,a         ; 6809, a low byte of x, b unchanged 

tfr a,x         ; low byte of x has value of a, high byte is $FF
tfr b,x         ; low byte of x has value of b, high byte is $FF

exg a,x         ; low byte of x has value of a, high byte is $FF
exg b,x         ; low byte of x has value of b, high byte is $FF

According to Motorola 6809 and Hitachi 6309 Programmer's Reference

Except for the case where the first operand is 8 bit and register CC or DP:
In this case the 16-bit register has the value of the 8-bit register in
high and low byte!
*/

void
exg(void)
{
  //    DEF                USE                          CODE
  unsigned post = imm_byte ();

  // TO DO: undocumented case described above...
  if (((post ^ (post << 4)) & 0x80) == 0) {
    char *tmp = get_reg (post >> 4);
    dumpf(R_VAL|R_Always, R_lookup(tmp), "val = %s;", tmp);
    set_reg (post >> 4, get_reg (post & 15));
    set_reg (post & 15, "val");
  }
  CK (8);
}


void
inc(char *arg)
{
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(arg),               "val = (UINT8)%s;", arg);
  dumpf(R_RES,             R_VAL,                       "res = (UINT8)(val + 1);");
  dumpf(R_Z,               R_RES,                       "Z = res;");
  dumpf(R_N,               R_RES,                       "N = res;");
  dumpf(R_V,               R_RES|R_VAL,                 "V = res & ~val;");
  dumpf(R_Always|R_lookup(arg), R_RES,                  "%s = res;", arg);
  CK (2);
}

void
ld(char *arg, char *opd)
{
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(opd),               "val = (UINT8)%s;", opd);
  dumpf(R_Always|R_lookup(arg), R_VAL,                  "%s = val;", arg);
  dumpf(R_N,               R_VAL,                       "N = val;");
  dumpf(R_Z,               R_VAL,                       "Z = val;");
  dumpf(R_V,               0,                           "V = 0;");
}

void
lsr(char *arg)
{
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(arg),               "val = (UINT8)%s;", arg);
  dumpf(R_C,               R_VAL,                       "C = val & 1;");
  dumpf(R_VAL,             R_VAL,                       "val = val >> 1;");
  dumpf(R_Always|R_lookup(arg),     R_VAL,              "%s = val;", arg);
  dumpf(R_N,               0,                           "N = 0;");
  dumpf(R_Z,               R_VAL,                       "Z = val;");
  CK (2);
}

void
mul(void)
{
  //    DEF                USE                          CODE
  dumpf(R_RES,             R_A|R_B,                     "res = (UINT16)(A * B);");
  dumpf(R_Z,               R_RES,                       "Z = res;");
  dumpf(R_C,               R_RES,                       "C = res & 0x80;");
  dumpf(R_Always|R_A,      R_RES,                       "A = res >> 8;");
  dumpf(R_Always|R_B,      R_RES,                       "B = (UINT8)res;");
  CK (11);
}

void
neg(char *arg)
{
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(arg),               "val = (UINT8)%s;", arg);
  dumpf(R_RES,             R_VAL,                       "res = -val;");
  dumpf(R_V,               R_VAL|R_RES,                 "V = val & (UINT8)res;");
  dumpf(R_C,               R_RES,                       "C = res & 0x100;");
  dumpf(R_VAL,             R_RES,                       "val = (UINT8)res;");
  dumpf(R_Always|R_lookup(arg),     R_VAL,              "%s = val;", arg);
  dumpf(R_Z,               R_VAL,                       "Z = val;");
  dumpf(R_N,               R_VAL,                       "N = val;");
  CK (2);
}


void
or(char *arg, char *val)
{
  //    DEF                USE                          CODE
  dumpf(R_RES,             R_lookup(arg)|R_lookup(val), "res = %s | (UINT8)%s;", arg, val); // val may have been cast to SINT8 already...
  dumpf(R_Always|R_lookup(arg),     R_RES,              "%s = res;", arg);
  dumpf(R_Z,               R_RES,                       "Z = res;");
  dumpf(R_N,               R_RES,                       "N = res;");
  dumpf(R_V,               0,                           "V = 0;");
}


void
rol(char *arg)
{
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(arg),               "val = (UINT8)%s;", arg);
  dumpf(R_RES,             R_VAL|R_C,                   "res = (val << 1) + (C != 0 ?1:0);");
  dumpf(R_C,               R_RES,                       "C = res & 0x100;");
  dumpf(R_Z,               R_RES,                       "Z = (UINT8)res;");
  dumpf(R_N,               R_RES,                       "N = (UINT8)res;"); // N, and V are both 8 bit variables so no cast required
  dumpf(R_V,               R_VAL|R_RES,                 "V = (UINT8)res ^ val;");
  dumpf(R_Always|R_lookup(arg), R_RES,                  "%s = (UINT8)res;", arg);
  CK (2);
}


void
ror(char *arg)
{
  //    DEF                USE                          CODE
  dumpf(R_RES,             R_lookup(arg)|R_C,           "res = ((UINT8)%s) | ((C != 0) ? 0x100 : 0);", arg);
  dumpf(R_C,               R_RES,                       "C = res & 1;");
  dumpf(R_RES,             R_RES,                       "res = (UINT8)(res >> 1);");
  dumpf(R_Z,               R_RES,                       "Z = res;");
  dumpf(R_N,               R_RES,                       "N = res;");
  dumpf(R_Always|R_lookup(arg),     R_RES,              "%s = res;", arg);
  CK (2);
}


void
sub(char *arg, char *val)
{
  //    DEF                USE                          CODE
  if (strncmp(val, "(SINT8)", strlen("(SINT8)")) == 0) val = val+strlen("(SINT8)"); //GT: bugfix?
  dumpf(R_VAL,             R_lookup(arg),               "val = (UINT8)%s;", arg);
  dumpf(R_ARG,             R_lookup(val),               "arg = %s;", val);
  dumpf(R_RES,             R_VAL|R_ARG,                 "res = val - (UINT8)arg;");
  dumpf(R_C,               R_RES,                       "C = res & 0x100;");
  dumpf(R_Z,               R_RES,                       "Z = (UINT8)res;");
  dumpf(R_N,               R_RES,                       "N = (UINT8)res;"); // cast is redundant but suppresses warning
  dumpf(R_V,               R_VAL|R_ARG|R_RES,           "V = (UINT8)((val ^ arg) & (val ^ res));");
  dumpf(R_Always|R_lookup(arg), R_RES,                  "%s = (UINT8)(res);", arg);
}


void
sbc(char *arg, char *val)
{
  //    DEF                USE                          CODE

  if (strncmp(val, "(SINT8)", strlen("(SINT8)")) == 0) val = val+strlen("(SINT8)");
  dumpf(R_VAL,             R_lookup(arg),               "val = (UINT8)%s;", arg);
  dumpf(R_ARG,             R_lookup(val),               "arg = %s;", val);
  dumpf(R_RES,             R_VAL|R_ARG|R_C,             "res = val - (UINT8)arg - (C != 0 ? 1:0);");
  dumpf(R_C,               R_RES,                       "C = res & 0x100;");
  dumpf(R_Z,               R_RES,                       "Z = (UINT8)res;");
  dumpf(R_N,               R_RES,                       "N = (UINT8)res;");
  dumpf(R_V,               R_VAL|R_ARG|R_RES,           "V = (UINT8)((val ^ arg) & (val ^ res));"); // FIXED!
  dumpf(R_Always|R_lookup(arg), R_RES,                  "%s = (UINT8)res;", arg);
}


void
st(char *arg, char *ea)
{
  // just FYI, none of the validation tests test storing at all well :-(  val = %s+1 went undetected!
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(arg),               "val = %s;", arg);
  dumpf(R_Z,               R_VAL,                       "Z = val;");
  dumpf(R_N,               R_VAL,                       "N = val;");
  dumpf(R_V,               0,                           "V = 0;");
  if (strncmp(ea, "0x", 2) == 0) {
    dumpf(R_Always,          R_lookup(ea)|R_VAL,                  "memory[%s] = val;", ea);
  } else {
    dumpf(R_EA,              R_lookup(ea),                "ea = %s;", ea);
    dumpf(R_Always,          R_EA|R_VAL,                  "memory[ea] = val;");
  }
}


void
tst(char *arg)
{
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(arg),               "val = (UINT8)%s;", arg);
  dumpf(R_Z,               R_VAL,                       "Z = val;");
  dumpf(R_N,               R_VAL,                       "N = val;");
  dumpf(R_V,               0,                           "V = 0;");
  CK (2);
}


/*
## TFR/EXG with unaligned register sizes

tfr x,b         ; 6809,6309: b low byte of x, a unchanged
tfr x,a         ; 6809, a low byte of x, b unchanged 

tfr a,x         ; low byte of x has value of a, high byte is $FF
tfr b,x         ; low byte of x has value of b, high byte is $FF

exg a,x         ; low byte of x has value of a, high byte is $FF
exg b,x         ; low byte of x has value of b, high byte is $FF

According to Motorola 6809 and Hitachi 6309 Programmer's Reference

Except for the case where the first operand is 8 bit and register CC or DP:
In this case the 16-bit register has the value of the 8-bit register in
high and low byte!
*/

void
tfr (void)
{
  unsigned post = imm_byte ();
  char *val;

  /*if(((post ^ (post << 4)) & 0x80) == 0)*/  // fixing TFR A,X ...
  val = get_reg((post >> 4)&15);

  //dumpf(R_Always, 0,      "/* from R%d to R%d */", (post >> 4)&15, post & 15);

  switch (post & 15)
    {
                 //    DEF   USE            CODE
    case 0:      dumpf(R_VAL,         R_lookup(val), "val = %s;", val);
                 dumpf(R_Always|R_A,  R_VAL,         "A = (UINT8)(val >> 8);");
                 dumpf(R_Always|R_B,  R_VAL,         "B = (UINT8)val;");      break;
    case 1:
      if ((((post >> 4)&15) == 8) || (((post >> 4)&15) == 9)) { // TFR A,X or TFR B,X
        dumpf(R_Always|R_X,  R_lookup(val), "X = %s | 0xFF00; // undocumented", val);
      } else {
        dumpf(R_Always|R_X,  R_lookup(val), "X = %s;", val);
      }
      break;
      
    case 2:      dumpf(R_Always|R_Y,  R_lookup(val), "Y = %s;", val);              break;
    case 3:      dumpf(R_Always|R_U,  R_lookup(val), "U = %s;", val);              break;
    case 4:      dumpf(R_Always|R_S,  R_lookup(val), "S = %s;", val);              break;
    case 5:      dumpf(R_Always|R_PC, R_lookup(val), "PC = %s; // check for missing JUMP", val);             break;
    case 8:      dumpf(R_Always|R_A,  R_lookup(val), "A = (UINT8)%s;", val);              break;
    case 9:      dumpf(R_Always|R_B,  R_lookup(val), "B = (UINT8)%s;", val);              break;
    case 10:     dumpf(R_Always|R_CC, R_lookup(val), "CC = %s;", val);
                 Individual_Flags_from_CC_Reg();                          break;
    case 11:     dumpf(R_Always|R_DP, R_lookup(val), "DP = (%s) << 8; checkDP();", val);
                 dumpf(R_Always, R_DP,      "// memory_DP = &memory[DP];");      break;
    default:     printf ("%X: invalid register code $%02X\n", iPC, post); exit(1);
  }

  CK (6);
}


/* 16-Bit Accumulator Instructions */
void
abx (void)
{
  //    DEF                USE                          CODE
  dumpf(R_Always|R_X,               R_X|R_B,                     "X = X + B;");
  CK (3);
}


void
addd(char *val)
{
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(val),               "val = %s;", val);
  dumpf(R_ARG,             R_A|R_B,                     "arg = (A << 8) | B;"); // I think one of the only two places we use ARG at the moment
  dumpf(R_RES,             R_ARG|R_VAL,                 "res = arg + val;");
  dumpf(R_C,               R_RES,                       "C = res & 0x10000;");
  dumpf(R_RES,             R_RES,                       "res = (UINT16)res;");
  dumpf(R_Z,               R_RES,                       "Z = res;");
  dumpf(R_V,               R_ARG|R_RES|R_VAL,           "V = ((arg ^ res) & (val ^ res)) >> 8;");
  dumpf(R_N,               R_RES,                       "N = res >> 8;");
  dumpf(R_Always|R_A,      R_RES,                       "A = (UINT8)(res >> 8);");
  dumpf(R_Always|R_B,      R_RES,                       "B = (UINT8)res;");
}


void
cmp16 (char *arg, char *val) // if val is not already simple it will be bracketed
{
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(arg),               "val = %s;", arg);
  dumpf(R_ARG,             R_lookup(val),               "arg = %s;", val);
//  dumpf(R_RES,             R_VAL|R_lookup(val),         "res = val - arg;");
  dumpf(R_RES,             R_VAL|R_ARG,         "res = val - arg;");
  dumpf(R_C,               R_RES,                       "C = res & 0x10000;");
  dumpf(R_RES,             R_RES,                       "res = (UINT16)res;");
  dumpf(R_Z,               R_RES,                       "Z = res;");
  dumpf(R_N,               R_RES,                       "N = res >> 8;");
  dumpf(R_V,               R_RES|R_VAL|R_ARG,   "V = ((val ^ arg) & (val ^ res)) >> 8;"); // TO DO cast result to UINT8?
}


// Plain LDD should no longer be called.
void
ldd(char *arg)
{
  char *s;
  //    DEF                USE                          CODE
  assert(__LINE__);
  if (arg[0] == '(' && (s=strstr(arg, "<<8)|")) && (strstr(s+1, "<<8)|") == NULL) && arg[strlen(arg)-1] == ')') {
    // Split up D into separate parts for assignment to A and B
    *s++ = '\0';
    if ((R_lookup(arg)&R_A) != 0) {
      dumpf(R_Always|R_VAL,             R_lookup(arg+2),             "val = %s;", arg+2);
      s = strchr(s, '|')+1; s[strlen(s)-1] = '\0';
      dumpf(R_Always|R_B,               R_lookup(s),                 "B = %s;", s);
      dumpf(R_Always|R_A,               R_VAL,                       "A = val;");
    } else {
      dumpf(R_Always|R_A,               R_lookup(arg+2),             "A = %s;", arg+2);
      s = strchr(s, '|')+1; s[strlen(s)-1] = '\0';
      dumpf(R_Always|R_B,               R_lookup(s),                 "B = %s;", s);
    }
  } else {
    dumpf(R_Always|R_A,               R_lookup(arg),               "A = (UINT8)((%s) >> 8);", arg); // is this ever passed a rd_mem? If so, cache in arg
    dumpf(R_Always|R_B,               R_lookup(arg),               "B = (UINT8)%s;", arg);
  }
}

// ldd_imm_word() == ldd(imm_word_s())
void ldd_imm_word(void) {
  dumpf(R_Always|R_A,      0,               "A = 0x%02x;", memory[NextPC-2]);
  dumpf(R_Always|R_B,      0,               "B = 0x%02x;", memory[NextPC-1]);
  dumpf(R_Z,               R_A|R_B,         "Z = A|B;");
  dumpf(R_N,               R_A,             "N = A;");
  dumpf(R_V,               0,               "V = 0;");
}

// ldd_direct() == ldd(RDMEM16(direct()))
void ldd_direct(void) {
#ifdef NEVER
  dumpf(R_Always|R_A,      R_DP,            "A = memory_DP[0x%02x]; // Care needed with I/O space and word fetches", memory[NextPC-1]);
  dumpf(R_Always|R_B,      R_DP,            "B = memory_DP[0x%02x];", memory[NextPC-1]+1);
#else
  dumpf(R_Always|R_A,      R_DP,            "A = memory[DP|0x%02x]; // Care needed with I/O space and word fetches", memory[NextPC-1]);
  dumpf(R_Always|R_B,      R_DP,            "B = memory[DP|0x%02x];", memory[NextPC-1]+1);
#endif
  dumpf(R_Z,               R_A|R_B,         "Z = A|B;");
  dumpf(R_N,               R_A,             "N = A;");
  dumpf(R_V,               0,               "V = 0;");
}

// ldd_indexed_post() == ldd(RDMEM16(indexed())); post()
void ldd_indexed_post(void) {
  char *s = indexed();
  if (strlen(s)>12) {// HACK!
    dumpf(R_EA,              R_lookup(s),                   "ea = %s;", s);
    post();
    //ldd(RDMEM16(indexed()));
    dumpf(R_Always|R_A,      R_EA,                        "A = memory[ea]; // Care needed with I/O space and word fetches");
    dumpf(R_Always|R_B,      R_EA,                        "B = memory[(UINT16)(ea+1)];");
  } else {
    dumpf(R_Always|R_A,      R_lookup(s),                 "A = memory[%s]; // Care needed with I/O space and word fetches. Use 'ea' here?", s);
    dumpf(R_Always|R_B,      R_lookup(s),                 "B = memory[(UINT16)(%s+1)];", s);
    post();
  }
  dumpf(R_Z,               R_A|R_B,                     "Z = A|B;");
  dumpf(R_N,               R_A,                         "N = A;");
  dumpf(R_V,               0,                           "V = 0;");
}

// ldd_extended() == ldd(RDMEM16(extended()))
void ldd_extended(void) {
  dumpf(R_Always|R_A,      0,               "A = memory[0x%04x]; // Care needed with I/O space and word fetches", ((memory[NextPC-2]<<8) | memory[NextPC-1])&0xffff);
  dumpf(R_Always|R_B,      0,               "B = memory[0x%04x];", (((memory[NextPC-2]<<8) | memory[NextPC-1]) + 1)&0xffff);
  dumpf(R_Z,               R_A|R_B,         "Z = A|B;");
  dumpf(R_N,               R_A,             "N = A;");
  dumpf(R_V,               0,               "V = 0;");
}


void
ld16(char *reg, char *arg)
{
  //    DEF                USE                          CODE
  dumpf(R_Always|R_lookup(reg), R_lookup(arg),          "%s = %s;", reg, arg);
  dumpf(R_Z,               R_lookup(reg),               "Z = %s;", reg);
  dumpf(R_N,               R_lookup(reg),               "N = (%s) >> 8;", reg);
  dumpf(R_V,               0,                           "V = 0;");
}


void
sex (void)
{
  //    DEF                USE                          CODE
  dumpf(R_Z,               R_B,                         "Z = B;");
  //dumpf(R_N,               R_B,                         "N = B & 0x80;");
  dumpf(R_N,               R_B,                         "N = B;");
  //dumpf(R_Always|R_A,      R_B,                         "A = ((B & 0x80) != 0) ? 0xff : 0;");
  dumpf(R_Always|R_A,      R_B,                         "A = ((SINT8)B < 0 ? 0xff : 0);");
  CK (2);
}


void
std(char *addr)
{
  //    DEF                USE                          CODE
  dumpf(R_Z,               R_A|R_B,                     "Z = A | B;");
  dumpf(R_N,               R_A,                         "N = A;");
  dumpf(R_V,               0,                           "V = 0;");
  if (strncmp(addr, "0x", 2) == 0) {
    dumpf(R_Always,          R_A|R_lookup(addr),          "memory[0x%04x] = A; // Care needed with I/O space and word fetches", HTOI(addr+2));
    dumpf(R_Always,          R_B|R_lookup(addr),          "memory[0x%04x] = B;", HTOI(addr+2)+1);
  } else {
    dumpf(R_EA,              R_lookup(addr),              "ea = %s;", addr);
    dumpf(R_Always,          R_A|R_EA,                    "memory[ea] = A; // Care needed with I/O space and word fetches");
    dumpf(R_Always,          R_B|R_EA,                    "memory[ea + 1] = B;");
  }
}


void
st16(char *arg, char *addr)
{
  //    DEF                USE                          CODE
  if (strncmp(addr, "(UINT16)", 8) == 0) addr = addr+strlen("(UINT16)");
  dumpf(R_VAL,             R_lookup(arg),               "val = %s;", arg);
  dumpf(R_Z,               R_VAL,                       "Z = val;");
  dumpf(R_N,               R_VAL,                       "N = (UINT8)(val >> 8);");
  dumpf(R_V,               0,                           "V = 0;");
  if (strncmp(addr, "0x", 2) == 0) {
    dumpf(R_Always,          R_VAL,                     "memory[0x%04x] = (UINT8)(val >> 8); // Care needed with I/O space and word fetches", HTOI(addr+2));
//    dumpf(R_Always,          R_VAL,      		"memory[0x%04x] = (UINT8)val;", (HTOI(addr+2)+1)&0xffff);
    dumpf(R_Always,          R_VAL|R_lookup(addr),      "memory[0x%04x] = (UINT8)val;", (HTOI(addr+2)+1)&0xffff);
  } else {
    dumpf(R_EA,              R_lookup(addr),            "ea = %s;", addr);
    dumpf(R_Always,          R_VAL|R_EA,                "memory[ea] = (UINT8)(val >> 8); // Care needed with I/O space and word fetches");
    dumpf(R_Always,          R_VAL|R_EA,                "memory[(UINT16)(ea + 1)] = (UINT8)val;");
  }
}


void
subd(char *val)
{
  //    DEF                USE                          CODE
  dumpf(R_VAL,             R_lookup(val),               "val = %s;", val);
  dumpf(R_ARG,             R_A|R_B,                     "arg = (A << 8) | B;"); // only other place we use ARG so far
  dumpf(R_RES,             R_ARG|R_VAL,                 "res = arg - val;");
  dumpf(R_C,               R_RES,                       "C = res & 0x10000;");
  dumpf(R_RES,             R_RES,                       "res = (UINT16)res;");
  dumpf(R_Z,               R_RES,                       "Z = res;");
  dumpf(R_V,               R_ARG|R_RES|R_VAL,           "V = ((arg ^ val) & (arg ^ res)) >> 8;");
  dumpf(R_Always|R_A,      R_RES,                       "A = (UINT8)(res >> 8);");
  dumpf(R_N,               R_RES,                       "N = res >> 8;");
  dumpf(R_Always|R_B,      R_RES,                       "B = (UINT8)res;");
}


/* stack instructions */
void
pshs (void)
{
  //    DEF                USE                          CODE
  unsigned post = imm_byte ();

  CK (5);

  if (post & 0x80) { // Recoded to be more sanitary...
    char tmp[16];
    //dumpf(R_Always|R_PC, 0, "PC = 0x%04x;", NextPC&0xffff); // check this is correct...?
    CK (2);
    //push_stack16 ("S", "PC");
    sprintf(tmp, "0x%02x", NextPC&0xff);
    push_stack8 ("S", tmp);
    sprintf(tmp, "0x%02x", (NextPC>>8)&0xff);
    push_stack8 ("S", tmp);
  }
  if (post & 0x40) { CK (2); push_stack16 ("S", "U"); }
  if (post & 0x20) { CK (2); push_stack16 ("S", "Y"); }
  if (post & 0x10) { CK (2); push_stack16 ("S", "X"); }
  if (post & 0x08) { CK (1); push_stack8 ("S", "(DP >> 8)"); }
  if (post & 0x04) { CK (1); push_stack8 ("S", "B"); }
  if (post & 0x02) { CK (1); push_stack8 ("S", "A"); }
  if (post & 0x01) { CK (1); CC_Reg_from_Individual_Flags(); push_stack8 ("S", "CC"); }
}


void
pshu (void)
{
  //    DEF                USE                          CODE
  unsigned post = imm_byte ();

  CK (5);

  if (post & 0x80) {
    char tmp[16];
    //dumpf(R_Always|R_PC, 0, "PC = 0x%04x;", NextPC&0xffff); // check this is correct...?
    CK (2);
    //push_stack16 ("U", "PC");
    sprintf(tmp, "0x%02x", NextPC&0xff);
    push_stack8 ("U", tmp);
    sprintf(tmp, "0x%02x", (NextPC>>8)&0xff);
    push_stack8 ("U", tmp);
  }
  if (post & 0x40) { CK (2); push_stack16 ("U", "S"); }
  if (post & 0x20) { CK (2); push_stack16 ("U", "Y"); }
  if (post & 0x10) { CK (2); push_stack16 ("U", "X"); }
  if (post & 0x08) { CK (1); push_stack8 ("U", "(DP >> 8)"); } // R_lookup
  if (post & 0x04) { CK (1); push_stack8 ("U", "B"); }
  if (post & 0x02) { CK (1); push_stack8 ("U", "A"); }
  if (post & 0x01) { CK (1); CC_Reg_from_Individual_Flags(); push_stack8 ("U", "CC"); }
}


void
puls (void)
{
  //    DEF                USE                          CODE
  unsigned post = imm_byte ();

  CK (5);

  if (post & 0x01) { CK (1); pop_stack8 ("S", "CC"); Individual_Flags_from_CC_Reg(); }
  if (post & 0x02) { CK (1); pop_stack8 ("S", "A"); }
  if (post & 0x04) { CK (1); pop_stack8 ("S", "B"); }
  if (post & 0x08) {
    CK (1); pop_stack8 ("S", "DP");  dumpf(R_DP, R_DP, "DP = DP << 8; checkDP();");
    dumpf(R_Always, R_DP, "// memory_DP = &memory[DP];");
  }
  if (post & 0x10) { CK (2); pop_stack16 ("S", "X"); }
  if (post & 0x20) { CK (2); pop_stack16 ("S", "Y"); }
  if (post & 0x40) { CK (2); pop_stack16 ("S", "U"); }
  if (post & 0x80) {
    CK (2); pop_stack16 ("S", "PC");
    Accessible = FALSE;END_HOOK;dumpf(R_Always,          R_FLUSH & ~R_HNZVC, "JUMP;"); 
  }
  else
  {
    END_HOOK;
  }
}


void
pulu (void)
{
  //    DEF                USE                          CODE
  unsigned post = imm_byte ();

  CK (5);

  if (post & 0x01) { CK (1); pop_stack8 ("U", "CC"); Individual_Flags_from_CC_Reg(); }
  if (post & 0x02) { CK (1); pop_stack8 ("U", "A"); }
  if (post & 0x04) { CK (1); pop_stack8 ("U", "B"); }
  if (post & 0x08) { CK (1); pop_stack8 ("U", "DP"); dumpf(R_DP, R_DP, "DP = DP << 8; checkDP();"); dumpf(R_Always, R_DP, "// memory_DP = &memory[DP];"); }
  if (post & 0x10) { CK (2); pop_stack16 ("U", "X"); }
  if (post & 0x20) { CK (2); pop_stack16 ("U", "Y"); }
  if (post & 0x40) { CK (2); pop_stack16 ("U", "S"); }
  if (post & 0x80) {
    CK (2); pop_stack16 ("U", "PC");
    Accessible = FALSE;END_HOOK;dumpf(R_Always,          R_FLUSH & ~R_HNZVC, "JUMP;"); 
  }
  else
  {
    END_HOOK;
  }
}


/* Miscellaneous Instructions */
void
nop (void)
{
  //    DEF                USE                          CODE
  CK (2);
}


void
jsr (char *addr)
{
  //    DEF                USE                          CODE

  static char tmp[16];
  // take care of return address:
  // (It is not possible with the current code to be executing a JSR from RAM.  We would
  //  need to add an interpreter in order to support that. So the assumption that NextPC
  //  is a compile-time constant is currently valid.)
  //sprintf(tmp, "0x%04x", NextPC&0xffff);

  //dumpf(R_Always|R_PC,              0,                           "PC = 0x%04x;", NextPC&0xffff);

  //push_stack16 ("S", tmp); // unclean!  unclean!
  //push_stack16 ("S", "PC");  // cleaner... 
  sprintf(tmp, "0x%02x", NextPC&0xff);
  push_stack8 ("S", tmp);
  sprintf(tmp, "0x%02x", (NextPC>>8)&0xff);
  push_stack8 ("S", tmp); // cleanest...NextPCParam


  if (!VECTREX) {
    if (Gen_Switch || (strncmp(addr, "0x", 2) != 0)) {
      dumpf(R_Always|R_PC,              R_lookup(addr),      "PC = %s;", addr);
      post(); Accessible = FALSE;END_HOOK;dumpf(R_Always,  R_FLUSH,                    "JUMP;"); 
    } else {
      if ((HTOI(addr+2) >= rom_start) && (HTOI(addr+2) < rom_end)) {
        // REDUNDANT? dumpf(R_Always|R_PC,              R_lookup(addr),      "PC = %s;", addr);
        // BUG! Only works for hex addresses, does not look up label as in disassembler...:
        if (Single_Step) {
          dumpf(R_Always|R_PC,              R_lookup(addr),      "PC = %s;", addr);
          post(); Accessible = FALSE;END_HOOK;dumpf(R_Always,  R_FLUSH,                    "JUMP;"); 
        } else {
          post();Accessible = FALSE;END_HOOK;
if (ALT_LABEL)
      dumpf(R_Always,          R_FLUSH & ~R_HNZVC, "goto L%s_;", UC(addr+2)); 
  else
      dumpf(R_Always,          R_FLUSH & ~R_HNZVC, "goto L%s;", UC(addr+2)); 
        }
      } else {
        dumpf(R_Always|R_PC,              R_lookup(addr),      "PC = %s;", addr);
        post(); Accessible = FALSE;END_HOOK;dumpf(R_Always,  R_FLUSH,                    "JUMP;"); 
      }
    }
  } else {    // Not a vectrex program so don't use Vectrex optimisations...
    int rc;
    unsigned int call;

    dumpf(R_Always|R_PC,              R_lookup(addr),              "PC = %s;", addr);
    rc = sscanf(addr, "%x", &call); // could have used the "0x" test?
    if (Single_Step || (rc != 1)) {       // not a constant jsr target.  Eg "JSR [,X++]"  PC has been assigned above.
                         // Unless this is a known address in the translated code, it will require
                         // support from a fallback interpreter.
      call = 0;
      post(); Accessible = FALSE;END_HOOK;dumpf(R_Always,          R_FLUSH & ~R_HNZVC, "JUMP;"); 
    } else if ((call&0xffff) >= 0x8000U) {
      if (*Name[call] != '\0') {        // We know this jsr target in the Vectrex BIOS
        // TO DO: If DP is known at this point, could we also check it against the expected value and warn if there is a mismatch?
        post(); Accessible = FALSE;END_HOOK;dumpf(R_Always|Out[call], R_PC|In[call], "JUMP;"); 
        
        //dumpf(Trash[call], 0, "// This should not be inserted! Someone is using a trashed register?");
        // if anyone tries to access a register that was trashed by this call,
        // the string above will be inserted.

        //if ((Out[call] & R_DP) != 0) {
          // If we ever do a high level emulation and avoid interpreting the vectrex BIOS, then we can use
          // this data to assign to DP where-ever the BIOS would have:
          //dumpf(R_Always, 0, "DP = 0x%02x << 8; memory_DP = &memory[DP];", DP_Out[call]);
          // Note that this has to be inserted after the return, and the return doesn't
          // happen exactly here - it happens after the label which is planted at the start
          // of the following instruction.  So that will have to be dealt with eventually...
        //}

      } else {
        // A *very few* roms use deeply internal calls in the BIOS that have not been documented
        // as externally usable interfaces.  The warning below is more of a call to document the
        // interface than anything critical to this translation.
        fprintf(stderr, "Warning: undetected bios call to %04x from %04x\n", call, iPC);
        post(); Accessible = FALSE;END_HOOK;dumpf(R_Always,          R_FLUSH & ~R_HNZVC, "JUMP;"); 
      }
    } else {
      // internal call - we could probably work out the regs that are required, trashed, or ignores using a transitive closure...
      // for now we'll err on the side of safety by requiring all registers except the flags... (fairly sure there's
      // nothing in the BIOS that takes flag bits as input parameters; and I think there's maybe only one that sets
      // a flag as a return value).NextPCParam
      post(); Accessible = FALSE;END_HOOK;dumpf(R_Always,          R_FLUSH & ~R_HNZVC, "JUMP;");  // remove the exceptions!
    }
  }
}


void
jmp (char *addr)
{
  //    DEF                USE                          CODE
  if (Gen_Switch || (strncmp(addr, "0x", 2) != 0)) {
    dumpf(R_Always|R_PC,     R_lookup(addr),            "PC = %s;", addr);
    post(); Accessible = FALSE;END_HOOK;dumpf(R_Always,  R_FLUSH,                   "JUMP;"); 
  } else {
    // REDUNDANT?  dumpf(R_Always|R_PC,     R_lookup(addr),              "PC = %s;", addr);
    // BUG! Only works for hex addresses, does not look up label as in disassembler...:
    if (HTOI(addr+2) >= rom_start && HTOI(addr+2) < rom_end) {
      if (Single_Step) {
        dumpf(R_Always|R_PC,     R_lookup(addr),            "PC = %s;", addr);
        post(); Accessible = FALSE;END_HOOK;dumpf(R_Always,  R_FLUSH,                   "JUMP;"); 
      } else {
        post(); Accessible = FALSE;END_HOOK;
	
if (ALT_LABEL)
  dumpf(R_Always,  R_FLUSH,                   "goto L%s_;", UC(addr+2)); 
else
  dumpf(R_Always,  R_FLUSH,                   "goto L%s;", UC(addr+2)); 
      }
    } else {
      // Jumping out of this block - label cannot exist, so use raw address...
      dumpf(R_Always|R_PC,     R_lookup(addr),            "PC = %s;", addr);
      post(); Accessible = FALSE;END_HOOK;dumpf(R_Always,  R_FLUSH,                   "JUMP;"); 
    }
  }
}


void
rti (void)
{
  // MALBAN
  dumpf(R_RES, R_E, "{\n    res=E;");
  //    DEF                USE                          CODE
  CK (6);
  pop_stack8 ("S", "CC"); Individual_Flags_from_CC_Reg ();

  dumpf(R_Always, R_RES, "if (res) {");
    CK (9);
    pop_stack8 ("S", "A");
    pop_stack8 ("S", "B");
    pop_stack8 ("S", "DP"); dumpf(R_DP, R_DP, "DP = DP << 8; checkDP();"); dumpf(R_Always, R_DP, "// memory_DP = &memory[DP];");
    pop_stack16 ("S", "X");
    pop_stack16 ("S", "Y");
    pop_stack16 ("S", "U");
  dumpf(R_Always, 0, "}");
  pop_stack16 ("S", "PC"); // needs special handling
  Accessible = FALSE;END_HOOK;dumpf(R_Always,           R_FLUSH,                    "JUMP;"); 
  dumpf(R_Always, 0, "}");
}


void
rts (void)
{
  //    DEF                USE                          CODE
  CK (5);
  pop_stack16 ("S", "PC");
  Accessible = FALSE;END_HOOK;dumpf(R_Always,           R_FLUSH,                    "JUMP;"); 
}


void
swi (void)
{
  //    DEF                USE                          CODE
  CK (19);

  //EFI |= E_FLAG;                                              // TO DO!!!

  static char tmp[16];

  //dumpf(R_Always|R_PC, 0, "PC = 0x%04x", NextPC&0xffff);

  //sprintf(tmp, "0x%04x", NextPC&0xffff);
  //push_stack16 ("S", tmp); // unclean!  unclean!
  //push_stack16 ("S", "PC");// cleaner
  sprintf(tmp, "0x%02x", NextPC&0xff);
  push_stack8 ("S", tmp);
  sprintf(tmp, "0x%02x", (NextPC>>8)&0xff);
  push_stack8 ("S", tmp); // cleanest...

  push_stack16 ("S", "U");
  push_stack16 ("S", "Y");
  push_stack16 ("S", "X");
  push_stack8 ("S", "(DP >> 8)"); // be careful
  push_stack8 ("S", "B");
  push_stack8 ("S", "A");
  CC_Reg_from_Individual_Flags(); push_stack8 ("S", "CC");

  //EFI |= (I_FLAG | F_FLAG);                                    // TO DO!!!

  dumpf(R_Always|R_PC, R_FLUSH, "PC = (memory[0xfffa]<<8)|memory[0xfffb];");
  Accessible = FALSE;END_HOOK;dumpf(R_Always, R_FLUSH & ~R_HNZVC, "JUMP;"); 
}


void
swi2 (void)
{
  //    DEF                USE                          CODE
  CK (20);

  // EFI |= E_FLAG;                                                // TO DO!!!

  static char tmp[16];

  //dumpf(R_Always|R_PC, 0, "PC = 0x%04x", NextPC&0xffff);

  //sprintf(tmp, "0x%04x", NextPC&0xffff);
  //push_stack16 ("S", tmp); // unclean!  unclean!
  //push_stack16 ("S", "PC");// cleaner
  sprintf(tmp, "0x%02x", NextPC&0xff);
  push_stack8 ("S", tmp);
  sprintf(tmp, "0x%02x", (NextPC>>8)&0xff);
  push_stack8 ("S", tmp); // cleanest...

  push_stack16 ("S", "U");
  push_stack16 ("S", "Y");
  push_stack16 ("S", "X");
  push_stack8 ("S", "(DP >> 8)"); // be careful
  push_stack8 ("S", "B");
  push_stack8 ("S", "A");
  CC_Reg_from_Individual_Flags(); push_stack8 ("S", "CC");
  dumpf(R_Always|R_PC, R_FLUSH, "PC = (memory[0xfff4]<<8)|memory[0xfff5];");
  Accessible = FALSE;END_HOOK;dumpf(R_Always, R_FLUSH & ~R_HNZVC, "JUMP;"); 
}


void
swi3 (void)
{
  //    DEF                USE                          CODE
  CK (20);

  // EFI |= E_FLAG;                                                // TO DO!!!

  static char tmp[16];

  //dumpf(R_Always|R_PC, 0, "PC = 0x%04x", NextPC&0xffff);

  //sprintf(tmp, "0x%04x", NextPC&0xffff);
  //push_stack16 ("S", tmp); // unclean!  unclean!
  //push_stack16 ("S", "PC");// cleaner
  sprintf(tmp, "0x%02x", NextPC&0xff);
  push_stack8 ("S", tmp);
  sprintf(tmp, "0x%02x", (NextPC>>8)&0xff); // cleanest...
  push_stack8 ("S", tmp); // cleanest...

  push_stack16 ("S", "U");
  push_stack16 ("S", "Y");
  push_stack16 ("S", "X");
  push_stack8 ("S", "(DP >> 8)");
  push_stack8 ("S", "B");
  push_stack8 ("S", "A");
  CC_Reg_from_Individual_Flags();  push_stack8 ("S", "CC");
  dumpf(R_Always|R_PC, R_FLUSH, "PC = (memory[0xfff2]<<8)|memory[0xfff3];");
  Accessible = FALSE;END_HOOK;dumpf(R_Always, R_FLUSH & ~R_HNZVC, "JUMP;"); 
}


void
cwai (void)
{
  //    DEF                USE                          CODE
  dumpf(R_Always,          R_FLUSH,                     "// CWAI - not suported yet!");
  //exit(1);
}


void
sync (void)
{
  //    DEF                USE                          CODE
  CK (4);
  dumpf(R_Always,          R_FLUSH,                     "// SYNC - not suported yet!");
  //exit(1);
}


void
orcc (void)
{
  //    DEF                USE                          CODE
  int tmp = imm_byte ();

  // TO DO: sort out this EFI hack...
  if ((tmp&E_FLAG) != 0) dumpf(R_Always|R_E, 0, "E = 1;");
  if ((tmp&F_FLAG) != 0) dumpf(R_Always|R_F, 0, "F = 1;");
  if ((tmp&I_FLAG) != 0) dumpf(R_Always|R_I, 0, "I = 1;");

  // use our conventions for TRUE and FALSE
  if ((tmp&H_FLAG) != 0) dumpf(R_Always|R_H, 0, "H = 0x10;");
  if ((tmp&N_FLAG) != 0) dumpf(R_Always|R_N, 0, "N = 0x80;");
  if ((tmp&Z_FLAG) != 0) dumpf(R_Always|R_Z, 0, "Z = 0;");
  if ((tmp&V_FLAG) != 0) dumpf(R_Always|R_V, 0, "V = 0x80;");
  if ((tmp&C_FLAG) != 0) dumpf(R_Always|R_C, 0, "C = 1;");

  CK (3);
}


void
andcc (void)
{
  //    DEF                USE                          CODE
  int tmp = imm_byte ();

  // TO DO: work on EFI hack
  if ((tmp&E_FLAG) == 0) dumpf(R_Always|R_E, 0, "E = 0;");
  if ((tmp&F_FLAG) == 0) dumpf(R_Always|R_F, 0, "F = 0;");
  if ((tmp&I_FLAG) == 0) dumpf(R_Always|R_I, 0, "I = 0;");

  // use our conventions for TRUE and FALSE
  if ((tmp&H_FLAG) == 0) dumpf(R_Always|R_H, 0, "H = 0;");
  if ((tmp&N_FLAG) == 0) dumpf(R_Always|R_N, 0, "N = 0;");
  if ((tmp&Z_FLAG) == 0) dumpf(R_Always|R_Z, 0, "Z = 1;");
  if ((tmp&V_FLAG) == 0) dumpf(R_Always|R_V, 0, "V = 0;");
  if ((tmp&C_FLAG) == 0) dumpf(R_Always|R_C, 0, "C = 0;");

  CK (3);
}


/* Branch Instructions */

#define cond_HI() "(Z && (!C))"
#define cond_LS() "((!Z) || C)"
#define cond_HS() "(!C)"
#define cond_LO() "(C)"
#define cond_NE() "(Z)"
#define cond_EQ() "(!Z)"
#define cond_VC() "((SINT8)V >= 0)"
#define cond_VS() "((SINT8)V <  0)"
#define cond_PL() "((SINT8)N >= 0)"
#define cond_MI() "((SINT8)N <  0)"
#define cond_GE() "((SINT8)(N^V) >= 0)"
#define cond_LT() "((SINT8)(N^V) <  0)"
#define cond_GT() "(((SINT8)(N^V) >= 0) && Z)"
// #define cond_GT() "(" cond_GE() " && " cond_NE() ")"
#define cond_LE() "(((SINT8)(N^V) <  0) || (!Z))"
// #define cond_LE() "(" cond_LT() " || " cond_EQ() ")"

signed short SEX8(UINT8 tmp) {
  if (tmp & 0x80) return 0xff00 | tmp;
  return tmp & 0x00ff;
}


void
bra (int TWEAKED_FLUSH)
{
  int IGNORE = R_HNZVC & ~TWEAKED_FLUSH;
  CK (3); // MALBAN
  //    DEF                USE                          CODE
  if (Single_Step || Gen_Switch) {
    dumpf(R_Always|R_PC,     0,                         "PC = 0x%04x;", (NextPC+SEX8(imm_byte()))&0xffff );
    post(); Accessible = FALSE;END_HOOK;dumpf(R_Always,  R_FLUSH & ~IGNORE,         "JUMP;"); 
  } else {
    // REDUNDANT? dumpf(R_Always|R_PC,     0,           "PC = 0x%04x;", (NextPC+SEX8(imm_byte()))&0xffff );
    // BUG! Only works for hex addresses, does not look up label as in disassembler...:
    post(); Accessible = FALSE;END_HOOK;
    
if (ALT_LABEL)
    dumpf(R_Always,  R_FLUSH & ~IGNORE,         "goto L%04X_;", (NextPC+SEX8(imm_byte()))&0xffff); 
else    
    dumpf(R_Always,  R_FLUSH & ~IGNORE,         "goto L%04X;", (NextPC+SEX8(imm_byte()))&0xffff); 
  }
}


void brn(void)
{
  
}


void
branch (char *cond)
{
  //    DEF                USE                          CODE
   dumpf(R_Always,          R_FLUSH,                     "// temp hack to force a flush");
  
  dumpf(R_Always,          R_lookup(cond),              "if %s {", cond);
  bra(R_lookup(cond));
  dumpf(R_Always,          0,                           "}"); 
  Accessible = TRUE;END_HOOK;
}


void
long_bra (void)
{
  //    DEF                USE                          CODE
  if (Single_Step || Gen_Switch) {
    dumpf(R_Always|R_PC,     0,                         "PC = 0x%04x;", (NextPC+imm_word())&0xffff);
    post(); Accessible = FALSE;END_HOOK;dumpf(R_Always,  R_FLUSH,                   "JUMP;"); 
  } else {
    // REDUNDANT? dumpf(R_Always|R_PC,     0,           "PC = 0x%04x;", (NextPC+imm_word())&0xffff);
    // BUG! Only works for hex addresses, does not look up label as in disassembler...:
    post(); Accessible = FALSE;END_HOOK;
    
if (ALT_LABEL)
    dumpf(R_Always,  R_FLUSH,                   "goto L%04X_;", (NextPC+imm_word())&0xffff); 
  else
    dumpf(R_Always,  R_FLUSH,                   "goto L%04X;", (NextPC+imm_word())&0xffff); 
  }
}


void long_brn(void)
{
  
}


void
long_branch (char *cond)
{
  //    DEF                USE                          CODE
  CK (5);
  dumpf(R_Always,          R_lookup(cond),              "if %s {", cond);
  CK (1);
  long_bra ();
  dumpf(R_Always,          R_lookup(cond),              "}"); Accessible = TRUE;
  END_HOOK;
}


void
long_bsr (void)
{
  //    DEF                USE                          CODE

  static char tmp[16];
  //sprintf(tmp, "0x%04x", NextPC&0xffff);
  //push_stack16 ("S", tmp); // unclean!  unclean!
  //push_stack16 ("S", "PC");// cleaner
  sprintf(tmp, "0x%02x", NextPC&0xff);
  push_stack8 ("S", tmp);
  sprintf(tmp, "0x%02x", (NextPC>>8)&0xff);
  push_stack8 ("S", tmp); // cleanest...

  CK (9);
  if (Single_Step || Gen_Switch) {
    dumpf(R_Always|R_PC,     0,                         "PC = 0x%04x;", (NextPC+imm_word())&0xffff);
    post(); Accessible = FALSE;END_HOOK;dumpf(R_Always,  R_FLUSH,                   "JUMP;"); 
  } else {
    // REDUNDANT? dumpf(R_Always|R_PC,     0,           "PC = 0x%04x;", (NextPC+imm_word())&0xffff);
    // BUG! Only works for hex addresses, does not look up label as in disassembler...:
    post(); Accessible = FALSE;END_HOOK;
    
if (ALT_LABEL)
    dumpf(R_Always,  R_FLUSH,                   "goto L%04X_;", (NextPC+imm_word())&0xffff); 
else
dumpf(R_Always,  R_FLUSH,                   "goto L%04X;", (NextPC+imm_word())&0xffff); 
  }
}


void
bsr (void)
{
  //    DEF                USE                          CODE

  static char tmp[16];
  //sprintf(tmp, "0x%04x", NextPC&0xffff);
  //push_stack16 ("S", tmp); // unclean!  unclean!
  //push_stack16 ("S", "PC");// cleaner
  sprintf(tmp, "0x%02x", NextPC&0xff);
  push_stack8 ("S", tmp);
  sprintf(tmp, "0x%02x", (NextPC>>8)&0xff);
  push_stack8 ("S", tmp); // cleanest...

  CK (7);
  if (Single_Step || Gen_Switch) {
    dumpf(R_Always|R_PC,     0,                         "PC = 0x%04x;", (NextPC+SEX8(imm_byte()))&0xffff);
    post(); Accessible = FALSE;END_HOOK;dumpf(R_Always,  R_FLUSH,                   "JUMP;"); 
  } else {
    // REDUNDANT? dumpf(R_Always|R_PC,     0,           "PC = 0x%04x;", (NextPC+SEX8(imm_byte()))&0xffff);
    // BUG! Only works for hex addresses, does not look up label as in disassembler...:
    post(); Accessible = FALSE;END_HOOK;
    
if (ALT_LABEL)
    dumpf(R_Always,  R_FLUSH,                   "goto L%04X_;", (NextPC+SEX8(imm_byte()))&0xffff); 
else
dumpf(R_Always,  R_FLUSH,                   "goto L%04X;", (NextPC+SEX8(imm_byte()))&0xffff); 
  }
}

void set(char *reg, char *val)
{
  //    DEF                USE                          CODE
  // since it is only X Y, S and U we can skip the cast...
  if (strncmp(val, "(UINT16)", strlen("(UINT16)")) == 0) val = val+strlen("(UINT16)");
  dumpf(R_Always|R_lookup(reg),     R_lookup(val),      "%s = %s;", reg, val);
  if (strcmp(reg, "X") == 0 || strcmp(reg, "Y") == 0) {
    dumpf(R_Z,             R_lookup(reg),               "Z = %s;", reg);
  }
}

/* translate 6809 code */
void translate6809 (int _iPC, int NextPCParam, int cycles)
{
  iPC = _iPC; // MALBAN global has same name than local... error out in some cases (functions) is false!
  
  // https://atjs.mbnet.fi/mc6809/Information/6809.htm for checking cycle counts
  /*
     The caller is responsible for identifying code areas and stepping over them. CodePtr is updated here
   in order to point at the second instruction byte if present, then to the start of the operands if present;
   it does *not* step over the operands, which are more reliably found by stepping *back* from NextPC ...
   */
  unsigned opcode;
#define S "S"
#define U "U"
#define X "X"
#define Y "Y"
#define A "A"
#define B "B"

  /* NextPC points to the byte following this instruction. Relative branches etc
     are relative to that address, not to the address of this instruction.
     NextPC is not modified by the opcode fetching process. */
  NextPC = NextPCParam;
  /* CodePTR initially points to the first byte of the current instruction, and
     is moved forward as the instruction and its operands are decoded.  It should
     be equal to NextPC by the time this instruction is fully translated (though
     might not be if the address part is skipped, eg in the "brn" instruction?)
  */
  CodePTR = iPC;

  opcode = memory[CodePTR++];
  // MALBAN
  // this can be used e.g. for clock checking
  // easier to start at the "next" instruction, than to do it at the end of every instrucion
  hookInstructionStart(iPC); // MALBAN
  if (ADD_INSTRUCTION_MODE == 1)
    dumpf (R_Always, 0, "INSTRUCTION_START"); 
  else if (ADD_INSTRUCTION_MODE == 2)
  {
    if (F_[iPC] & F_LABEL)
    {
      dumpf (R_Always, 0, "PC = 0x%04x", iPC); 
      dumpf (R_Always, 0, "INSTRUCTION_START"); 
    }
  }
  else if (ADD_INSTRUCTION_MODE == 3)
  {
    dumpf (R_Always, 0, "PC = 0x%04x", iPC); 
    if (!(F_[iPC] & F_LABEL))
    {
      dumpf (R_Always, 0, "case 0x%04X:\n", iPC); 
    }
    dumpf (R_Always, 0, "INSTRUCTION_START"); 
    Accessible = TRUE;
  }

  
  

  // if we want the PC to always be valid (eg for diags) then force it here:
  //dumpf(R_PC, 0, "PC = 0x%04x; /* optional? */", NextPC);
  switch (opcode) {
    case 0x00: CK(4); neg(RDMEM(direct()));END_HOOK; break; /* NEG direct */
    case 0x03: CK(4); com(RDMEM(direct()));END_HOOK; break; /* COM direct */
    case 0x04: CK(4); lsr(RDMEM(direct()));END_HOOK; break; /* LSR direct */
    case 0x06: CK(4); ror(RDMEM(direct()));END_HOOK; break; /* ROR direct */
    case 0x07: CK(4); asr(RDMEM(direct()));END_HOOK; break; /* ASR direct */
    case 0x08: CK(4); asl(RDMEM(direct()));END_HOOK; break; /* ASL direct */
    case 0x09: CK(4); rol(RDMEM(direct()));END_HOOK; break; /* ROL direct */
    case 0x0a: CK(4); dec(RDMEM(direct()));END_HOOK; break; /* DEC direct */
    case 0x0c: CK(4); inc(RDMEM(direct()));END_HOOK; break; /* INC direct */
    case 0x0d: CK(4); tst(RDMEM(direct()));END_HOOK; break; /* TST direct */
    case 0x0e: CK(3); jmp(direct());        break; /* JMP direct */
    case 0x0f: CK(4); clr(RDMEM(direct()));END_HOOK;break; /* CLR direct */

    case 0x10: {
      opcode = memory[CodePTR++];

      switch (opcode) {
      case 0x21:                        CK(5);END_HOOK;      /* PC += 2; */                               break; // lbrn
        case 0x22:                                    long_branch( cond_HI() );                  break;
        case 0x23:                                    long_branch( cond_LS() );                  break;
        case 0x24:                                    long_branch( cond_HS() );                  break;
        case 0x25:                                    long_branch( cond_LO() );                  break;
        case 0x26:                                    long_branch( cond_NE() );                  break;
        case 0x27:                                    long_branch( cond_EQ() );                  break;
        case 0x28:                                    long_branch( cond_VC() );                  break;
        case 0x29:                                    long_branch( cond_VS() );                  break;
        case 0x2a:                                    long_branch( cond_PL() );                  break;
        case 0x2b:                                    long_branch( cond_MI() );                  break;
        case 0x2c:                                    long_branch( cond_GE() );                  break;
        case 0x2d:                                    long_branch( cond_LT() );                  break;
        case 0x2e:                                    long_branch( cond_GT() );                  break;
        case 0x2f:                                    long_branch( cond_LE() );                  break;
        case 0x3f:                                    swi2();                                    break;
        case 0x83:                        CK(5);      cmp16(get_d(),imm_word_s());END_HOOK;               break;
        case 0x8c:                        CK(5);      cmp16(Y,imm_word_s());END_HOOK;                     break;
        case 0x8e:                        CK(4);      ld16(Y, imm_word_s()); END_HOOK;                    break;
        case 0x93:                        CK(5);      cmp16(get_d(),RDMEM16(direct())); CK(1);END_HOOK;   break;
        case 0x9c:                        CK(5);      cmp16(Y, RDMEM16(direct()));      CK(1);END_HOOK;   break;
        case 0x9e:                        CK(5);      ld16(Y, RDMEM16(direct()));END_HOOK;                break;
        case 0x9f:                        CK(5);      st16(Y, direct()); END_HOOK;                        break;
        case 0xa3: CK(1);                             cmp16(get_d(),RDMEM16(indexed())); post(); CK(1);END_HOOK;  break;
        case 0xac: CK(1);                             cmp16(Y,RDMEM16(indexed())); post();       CK(1);END_HOOK;  break;
        case 0xae: CK(1);                             ld16(Y, RDMEM16(indexed())); post(); END_HOOK;      break;
        case 0xaf: CK(1);                             st16(Y, indexed()); post(); END_HOOK;               break;
        case 0xb3:                        CK(6);      cmp16(get_d(),RDMEM16(extended())); CK(1);END_HOOK; break;
        case 0xbc:                        CK(6);      cmp16(Y,RDMEM16(extended()));       CK(1);END_HOOK; break;
        case 0xbe:                        CK(6);      ld16(Y, RDMEM16(extended()));END_HOOK;              break;
        case 0xbf:                        CK(6);      st16(Y, extended());END_HOOK;                       break;
        case 0xce:                        CK(4);      ld16(S, imm_word_s()); END_HOOK;                    break;
        case 0xde:                        CK(5);      ld16(S, RDMEM16(direct())); END_HOOK;               break;
        case 0xdf:                        CK(5);      st16(S, direct());  END_HOOK;                       break; 
        case 0xee: CK(1);                             ld16(S, RDMEM16(indexed())); post();END_HOOK;       break;
        case 0xef: CK(1);                             st16(S, indexed()); post(); END_HOOK;               break; 
        case 0xfe: CK(6);                             ld16(S, RDMEM16(extended())); END_HOOK;             break;
        case 0xff:                        CK(6);      st16(S, extended()); END_HOOK;                      break;
        default:   printf("%X: invalid opcode $10%02X\n", iPC, opcode);
                   // exit(1);
      }           
    }
    break;

    case 0x11: {
      opcode = memory[CodePTR++];

      switch (opcode) {
        case 0x3f:        swi3();                        break;
        case 0x83: CK(5); cmp16(U,imm_word_s());END_HOOK;        break;
        case 0x8c: CK(5); cmp16(S,imm_word_s()); END_HOOK;        break;
        case 0x93: CK(6); cmp16(U,RDMEM16(direct())); END_HOOK;   break;
        case 0x9c: CK(6); cmp16(S,RDMEM16(direct())); END_HOOK;   break;
        case 0xa3: CK(2); cmp16(U,RDMEM16(indexed())); post(); END_HOOK;  break;
        case 0xac: CK(2); cmp16(S,RDMEM16(indexed())); post(); END_HOOK;  break;
        case 0xb3: CK(7); cmp16(U,RDMEM16(extended())); END_HOOK; break;
        case 0xbc: CK(7); cmp16(S,RDMEM16(extended())); END_HOOK; break;
        default:   printf("%X: invalid opcode $11%02X\n",iPC,opcode);
                   // exit(1);
      }
    }
    break;

    case 0x12: nop();END_HOOK;                    break;
    case 0x13: sync();                   break;
    case 0x16: long_bra(); CK(5);        break;
    case 0x17: long_bsr();               break;
    case 0x19: daa(); END_HOOK;                   break;
    case 0x1a: orcc(); END_HOOK;                 break;
    case 0x1c: andcc(); END_HOOK;                 break;
    case 0x1d: sex(); END_HOOK;                   break;
    case 0x1e: exg(); END_HOOK;                   break;
    case 0x1f: tfr(); END_HOOK;                   break;

    case 0x20: bra(R_FLUSH);                break;
    case 0x21: brn();                CK(3);END_HOOK;       break; // brn
    case 0x22: branch( cond_HI() );               break;
    case 0x23: branch( cond_LS() );               break;
    case 0x24: branch( cond_HS() );               break;
    case 0x25: branch( cond_LO() );               break;
    case 0x26: branch( cond_NE() );               break;
    case 0x27: branch( cond_EQ() );               break;
    case 0x28: branch( cond_VC() );               break;
    case 0x29: branch( cond_VS() );               break;
    case 0x2a: branch( cond_PL() );               break;
    case 0x2b: branch( cond_MI() );               break;
    case 0x2c: branch( cond_GE() );               break;
    case 0x2d: branch( cond_LT() );               break;
    case 0x2e: branch( cond_GT() );               break;
    case 0x2f: branch( cond_LE() );               break;

    case 0x30: set(X, indexed()); post();END_HOOK; break; /* LEAX indexed */  // set_X and set_Y should also set Z
    case 0x31: set(Y, indexed()); post();END_HOOK; break; /* LEAY indexed */
    case 0x32: set(S, indexed()); post();END_HOOK; break; /* LEAS indexed */
    case 0x33: set(U, indexed()); post();END_HOOK; break; /* LEAU indexed */
    case 0x34: pshs();END_HOOK;            break; /* PSHS implied */
    case 0x35: puls();            break; /* PULS implied */
    case 0x36: pshu();END_HOOK;            break; /* PSHU implied */
    case 0x37: pulu();            break; /* PULU implied */
    case 0x39: rts();             break; /* RTS implied  */
    case 0x3a: abx(); END_HOOK;            break; /* ABX implied  */
    case 0x3b: rti();             break; /* RTI implied  */
    case 0x3c: cwai();END_HOOK;            break; /* CWAI implied */
    case 0x3d: mul(); END_HOOK;            break; /* MUL implied  */
    case 0x3f: swi();             break; /* SWI implied  */

    case 0x40: neg(A);END_HOOK;break; /* NEGA implied */
    case 0x43: com(A);END_HOOK; break; /* COMA implied */
    case 0x44: lsr(A);END_HOOK; break; /* LSRA implied */
    case 0x46: ror(A);END_HOOK; break; /* RORA implied */
    case 0x47: asr(A);END_HOOK; break; /* ASRA implied */
    case 0x48: asl(A); END_HOOK;break; /* ASLA implied */
    case 0x49: rol(A); END_HOOK;break; /* ROLA implied */
    case 0x4a: dec(A); END_HOOK;break; /* DECA implied */
    case 0x4c: inc(A);END_HOOK; break; /* INCA implied */
    case 0x4d: tst(A); END_HOOK;break; /* TSTA implied */
    case 0x4f: clr(A); END_HOOK;break; /* CLRA implied */

    case 0x50: neg(B); END_HOOK;break; /* NEGB implied */
    case 0x53: com(B); END_HOOK;break; /* COMB implied */
    case 0x54: lsr(B); END_HOOK;break; /* LSRB implied */
    case 0x56: ror(B);END_HOOK; break; /* RORB implied */
    case 0x57: asr(B); END_HOOK;break; /* ASRB implied */
    case 0x58: asl(B);END_HOOK; break; /* ASLB implied */
    case 0x59: rol(B); END_HOOK;break; /* ROLB implied */
    case 0x5a: dec(B); END_HOOK;break; /* DECB implied */
    case 0x5c: inc(B);END_HOOK; break; /* INCB implied */
    case 0x5d: tst(B);END_HOOK; break; /* TSTB implied */
    case 0x5f: clr(B); END_HOOK;break; /* CLRB implied */

    case 0x60:        neg(RDMEM(indexed())); post(); END_HOOK;break; /* NEG indexed */
    case 0x63:        com(RDMEM(indexed())); post(); END_HOOK;break; /* COM indexed */
    case 0x64:        lsr(RDMEM(indexed())); post(); END_HOOK;break; /* LSR indexed */
    case 0x66:        ror(RDMEM(indexed())); post();END_HOOK; break; /* ROR indexed */
    case 0x67:        asr(RDMEM(indexed())); post(); END_HOOK;break; /* ASR indexed */
    case 0x68:        asl(RDMEM(indexed())); post();END_HOOK; break; /* ASL indexed */
    case 0x69:        rol(RDMEM(indexed())); post();END_HOOK;break; /* ROL indexed */
    case 0x6a:        dec(RDMEM(indexed())); post(); END_HOOK;break; /* DEC indexed */
    case 0x6c:        inc(RDMEM(indexed())); post(); END_HOOK;break; /* INC indexed */
    case 0x6d:        tst(RDMEM(indexed())); post(); END_HOOK;break; /* TST indexed */
    case 0x6e: CK(1); jmp(indexed());                break; /* JMP indexed */   // post() is handled in jmp() code so it comes before JUMP
    case 0x6f:        clr(RDMEM(indexed())); post(); END_HOOK;break; /* CLR indexed */

    case 0x70: CK(5); neg(RDMEM(extended())); END_HOOK;break; /* NEG extended */
    case 0x73: CK(5); com(RDMEM(extended())); END_HOOK;break; /* COM extended */
    case 0x74: CK(5); lsr(RDMEM(extended())); END_HOOK;break; /* LSR extended */
    case 0x76: CK(5); ror(RDMEM(extended())); END_HOOK;break; /* ROR extended */
    case 0x77: CK(5); asr(RDMEM(extended())); END_HOOK;break; /* ASR extended */
    case 0x78: CK(5); asl(RDMEM(extended())); END_HOOK;break; /* ASL extended */
    case 0x79: CK(5); rol(RDMEM(extended())); END_HOOK;break; /* ROL extended */
    case 0x7a: CK(5); dec(RDMEM(extended())); END_HOOK;break; /* DEC extended */
    case 0x7c: CK(5); inc(RDMEM(extended())); END_HOOK;break; /* INC extended */
    case 0x7d: CK(5); tst(RDMEM(extended())); END_HOOK;break; /* TST extended */
    case 0x7e: CK(4); jmp(extended());        break; /* JMP extended */
    case 0x7f: CK(5); clr(RDMEM(extended())); END_HOOK;break; /* CLR extended */

    case 0x80: CK(2); sub(A, imm_byte_s());  END_HOOK;break; // TO DO: all these recipients of imm_byte_s need to be checked that they handle signedness correctly
    case 0x81: CK(2); cmp(A, imm_byte_s());  END_HOOK;break;
    case 0x82: CK(2); sbc(A, imm_byte_s());  END_HOOK;break;
    case 0x83: CK(4); subd(imm_word_s());    END_HOOK;break;
    case 0x84: CK(2); and(A, imm_byte_s());  END_HOOK;break;
    case 0x85: CK(2); bit(A, imm_byte_s());  END_HOOK;break;
    case 0x86: CK(2); ld(A, imm_byte_s());   END_HOOK;break;
    case 0x88: CK(2); eor(A, imm_byte_s());  END_HOOK;break;
    case 0x89: CK(2); adc(A, imm_byte_s());  END_HOOK;break;
    case 0x8a: CK(2); or(A, imm_byte_s());   END_HOOK;break;
    case 0x8b: CK(2); add(A, imm_byte_s());  END_HOOK;break;
    case 0x8c: CK(4); cmp16(X, imm_word_s());END_HOOK;break;
    case 0x8d:        bsr();                 break;
    case 0x8e: CK(3); ld16(X, imm_word_s()); END_HOOK;break;

    case 0x90: CK(4); sub(A, RDMEM(direct()));     END_HOOK;break;
    case 0x91: CK(4); cmp(A, RDMEM(direct()));    END_HOOK; break;
    case 0x92: CK(4); sbc(A, RDMEM(direct()));    END_HOOK; break;
    case 0x93: CK(5); subd(RDMEM16(direct()));   END_HOOK;  break;
    case 0x94: CK(4); and(A, RDMEM(direct()));   END_HOOK;  break;
    case 0x95: CK(4); bit(A, RDMEM(direct()));    END_HOOK; break;
    case 0x96: CK(4); ld(A, RDMEM(direct()));     END_HOOK; break;
    case 0x97: CK(4); st(A, direct());            END_HOOK; break;
    case 0x98: CK(4); eor(A, RDMEM(direct()));   END_HOOK;  break;
    case 0x99: CK(4); adc(A, RDMEM(direct()));    END_HOOK; break;
    case 0x9a: CK(4); or(A, RDMEM(direct()));    END_HOOK;  break;
    case 0x9b: CK(4); add(A, RDMEM(direct()));    END_HOOK; break;
    case 0x9c: CK(5); cmp16(X, RDMEM16(direct()));END_HOOK; break;
    case 0x9d: CK(7); jsr(direct());               break;
    case 0x9e: CK(4); ld16(X, RDMEM16(direct())); END_HOOK; break;
    case 0x9f: CK(4); st16(X, direct());          END_HOOK; break;

    case 0xa0:        sub(A, RDMEM(indexed())); post(); END_HOOK;           break;
    case 0xa1:        cmp(A, RDMEM(indexed())); post(); END_HOOK;           break;
    case 0xa2:        sbc(A, RDMEM(indexed())); post(); END_HOOK;           break;
    case 0xa3: CK(1); subd(RDMEM16(indexed())); post();  END_HOOK;          break;
    case 0xa4:        and(A, RDMEM(indexed())); post(); END_HOOK;           break;
    case 0xa5:        bit(A, RDMEM(indexed())); post();  END_HOOK;          break;
    case 0xa6:        ld(A, RDMEM(indexed())); post();  END_HOOK;           break;
    case 0xa7:        st(A, indexed()); post();         END_HOOK;           break;
    case 0xa8:        eor(A, RDMEM(indexed())); post(); END_HOOK;           break;
    case 0xa9:        adc(A, RDMEM(indexed())); post(); END_HOOK;           break;
    case 0xaa:        or(A, RDMEM(indexed())); post();  END_HOOK;           break;
    case 0xab:        add(A, RDMEM(indexed())); post();END_HOOK;            break;
    case 0xac: CK(1); cmp16(X, RDMEM16(indexed())); post(); END_HOOK;       break;
    case 0xad: CK(3); jsr(indexed());                              break;   // AHA!: post() is handled in jsr() code so it comes before JUMP
    case 0xae:        ld16(X, RDMEM16(indexed())); post(); END_HOOK;        break;
    case 0xaf:        st16(X, indexed()); post();          END_HOOK;        break;

    case 0xb0: CK(5); sub(A, RDMEM(extended()));       END_HOOK;     break;
    case 0xb1: CK(5); cmp(A, RDMEM(extended()));       END_HOOK;     break;
    case 0xb2: CK(5); sbc(A, RDMEM(extended()));       END_HOOK;     break;
    case 0xb3: CK(6); subd(RDMEM16(extended()));       END_HOOK;     break;
    case 0xb4: CK(5); and(A, RDMEM(extended()));       END_HOOK;     break;
    case 0xb5: CK(5); bit(A, RDMEM(extended()));       END_HOOK;     break;
    case 0xb6: CK(5); ld(A, RDMEM(extended()));        END_HOOK;     break;
    case 0xb7: CK(5); st(A, extended());               END_HOOK;     break;
    case 0xb8: CK(5); eor(A, RDMEM(extended()));       END_HOOK;     break;
    case 0xb9: CK(5); adc(A, RDMEM(extended()));       END_HOOK;     break;
    case 0xba: CK(5); or(A, RDMEM(extended()));        END_HOOK;     break;
    case 0xbb: CK(5); add(A, RDMEM(extended()));       END_HOOK;     break;
    case 0xbc: CK(6); cmp16(X, RDMEM16(extended()));   END_HOOK;     break;
    case 0xbd: CK(8); jsr(extended());                      break;
    case 0xbe: CK(5); ld16(X, RDMEM16(extended()));    END_HOOK;     break;
    case 0xbf: CK(5); st16(X, extended());             END_HOOK;     break;

    case 0xc0: CK(2); sub(B, imm_byte_s());  END_HOOK;  break;
    case 0xc1: CK(2); cmp(B, imm_byte_s());  END_HOOK;  break;
    case 0xc2: CK(2); sbc(B, imm_byte_s());  END_HOOK;  break;
    case 0xc3: CK(4); addd(imm_word_s());    END_HOOK;  break;
    case 0xc4: CK(2); and(B, imm_byte_s());  END_HOOK;  break;
    case 0xc5: CK(2); bit(B, imm_byte_s());  END_HOOK;  break;
    case 0xc6: CK(2); ld(B, imm_byte_s());   END_HOOK;  break;
    case 0xc8: CK(2); eor(B, imm_byte_s());  END_HOOK;  break;
    case 0xc9: CK(2); adc(B, imm_byte_s());  END_HOOK;  break;
    case 0xca: CK(2); or(B, imm_byte_s());   END_HOOK;  break;
    case 0xcb: CK(2); add(B, imm_byte_s());  END_HOOK;  break;
    case 0xcc: CK(3); ldd_imm_word();        END_HOOK;  break;  // ldd_imm_word() == ldd(imm_word_s())
    case 0xce: CK(3); ld16(U, imm_word_s()); END_HOOK;  break;

    case 0xd0: CK(4); sub(B, RDMEM(direct()));  END_HOOK;   break;
    case 0xd1: CK(4); cmp(B, RDMEM(direct()));  END_HOOK;   break;
    case 0xd2: CK(4); sbc(B, RDMEM(direct()));  END_HOOK;   break;
    case 0xd3: CK(5); addd(RDMEM16(direct()));  END_HOOK;   break;
    case 0xd4: CK(4); and(B, RDMEM(direct()));  END_HOOK;   break;
    case 0xd5: CK(4); bit(B, RDMEM(direct()));  END_HOOK;   break;
    case 0xd6: CK(4); ld(B, RDMEM(direct()));   END_HOOK;   break;
    case 0xd7: CK(4); st(B, direct());          END_HOOK;   break;
    case 0xd8: CK(4); eor(B, RDMEM(direct()));  END_HOOK;   break;
    case 0xd9: CK(4); adc(B, RDMEM(direct()));  END_HOOK;   break;
    case 0xda: CK(4); or(B, RDMEM(direct()));   END_HOOK;   break;
    case 0xdb: CK(4); add(B, RDMEM(direct()));  END_HOOK;   break;
    case 0xdc: CK(4); ldd_direct();             END_HOOK;   break;  // ldd_direct() == ldd(RDMEM16(direct()))
    case 0xdd: CK(4); std(direct());            END_HOOK;   break;
    case 0xde: CK(4); ld16(U, RDMEM16(direct()));END_HOOK;  break;
    case 0xdf: CK(4); st16(U, direct());        END_HOOK;   break;

    case 0xe0:        sub(B, RDMEM(indexed())); post();  END_HOOK;   break;
    case 0xe1:        cmp(B, RDMEM(indexed())); post();  END_HOOK;   break;
    case 0xe2:        sbc(B, RDMEM(indexed())); post();  END_HOOK;   break;
    case 0xe3: CK(1); addd(RDMEM16(indexed())); post();  END_HOOK;   break;
    case 0xe4:        and(B, RDMEM(indexed())); post();  END_HOOK;   break;
    case 0xe5:        bit(B, RDMEM(indexed())); post();  END_HOOK;   break;
    case 0xe6:        ld(B, RDMEM(indexed())); post();   END_HOOK;   break;
    case 0xe7:        st(B, indexed()); post();          END_HOOK;   break;
    case 0xe8:        eor(B, RDMEM(indexed())); post();  END_HOOK;   break;
    case 0xe9:        adc(B, RDMEM(indexed())); post();  END_HOOK;   break;
    case 0xea:        or(B, RDMEM(indexed())); post();   END_HOOK;   break;
    case 0xeb:        add(B, RDMEM(indexed())); post(); END_HOOK;    break;
    case 0xec:        ldd_indexed_post();               END_HOOK;    break; // ldd_indexed_post() == ldd(RDMEM16(indexed())); post();ldd(RDMEM16(indexed())); post()
    case 0xed:        std(indexed()); post();           END_HOOK;   break;
    case 0xee:        ld16(U, RDMEM16(indexed())); post();END_HOOK;  break;  // TO DO: NEED TO CHECK THINGS LIKE "LDU 3,U++"
    case 0xef:        st16(U, indexed()); post();       END_HOOK;    break;

    case 0xf0: CK(5); sub(B, RDMEM(extended()));  END_HOOK;  break;
    case 0xf1: CK(5); cmp(B, RDMEM(extended()));  END_HOOK;   break;
    case 0xf2: CK(5); sbc(B, RDMEM(extended()));  END_HOOK;   break;
    case 0xf3: CK(6); addd(RDMEM16(extended()));  END_HOOK;   break;
    case 0xf4: CK(5); and(B, RDMEM(extended()));  END_HOOK;   break;
    case 0xf5: CK(5); bit(B, RDMEM(extended()));  END_HOOK;   break;
    case 0xf6: CK(5); ld(B, RDMEM(extended()));   END_HOOK;   break;
    case 0xf7: CK(5); st(B, extended());          END_HOOK;   break;
    case 0xf8: CK(5); eor(B, RDMEM(extended()));  END_HOOK;   break;
    case 0xf9: CK(5); adc(B, RDMEM(extended()));  END_HOOK;   break;
    case 0xfa: CK(5); or(B, RDMEM(extended()));   END_HOOK;   break;
    case 0xfb: CK(5); add(B, RDMEM(extended()));  END_HOOK;   break;
    case 0xfc: CK(5); ldd_extended();             END_HOOK;   break; // ldd_extended() == ldd(RDMEM16(extended()))
    case 0xfd: CK(5); std(extended());            END_HOOK;   break;
    case 0xfe: CK(5); ld16(U, RDMEM16(extended())); END_HOOK; break;
    case 0xff: CK(5); st16(U, extended());         END_HOOK;  break;
    default:   printf("%04X: invalid opcode $%02X\n",iPC,opcode); exit(1);
  }
}
