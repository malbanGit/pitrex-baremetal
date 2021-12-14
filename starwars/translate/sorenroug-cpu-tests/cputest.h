#ifndef __CPUTEST_H__
#define __CPUTEST_H__ 1
#ifdef __STDC__
#include <stdlib.h>
#else
#define void int
#endif

#define MEMSIZE 1024

/*
  Pull Order
  |
  v

     CC         Increasing
     A          memory
     B          |
     DP         v
     X Hi 
     X Lo 
     Y Hi 
     Y Lo 
     U/S Hi 
     U/S Lo 
     PC Hi 
     PC Lo 

  ^
  |
  Push Order
 */

struct registers {
  char rg_cc;
  char rg_a;
  char rg_b;
  char rg_dp;
  unsigned short rg_x;
  unsigned short rg_y;
  unsigned short rg_u;
};

#define REGSIZE (sizeof (struct registers))

unsigned short Host(unsigned short Micro);
unsigned short Micro(unsigned short Host);

//extern char memory[MEMSIZE];
extern struct registers *initregs;
#define CTLMEMSIZE 100 /* 2 * 50 */
struct registers *initregs;

// All we're using here is: unsigned char memory[MEMSIZE+CTLMEMSIZE+REGSIZE+4];
// but we'll overspecify it for the benefit of the emulator - it would only
// be a problem if we ressurect native execution...
unsigned char memory[0x10000];

/* initregs = (struct registers *)&memory[MEMSIZE+CTLMEMSIZE]; */

extern char *currtest;


#define CODESTRT 0

#define runinst(funcname, inst) runcode(#funcname, inst, sizeof inst)

/* Small code segment to save registers and call test.
 */
extern unsigned char *ctlmemory /* [100] */; /* WARNING! Host byte-sex sensitive */

extern unsigned short *save_s;
extern unsigned short *dpLoc;

#define CC_C 0
#define CC_V 1
#define CC_Z 2
#define CC_N 3
#define CC_I 4
#define CC_H 5
#define CC_F 6
#define CC_E 7

#define LDA 0x86
#define LDB 0xC6
#define RTS 0x39

#define setA(value)    initregs->rg_a = value
#define assertA(exp)   assertByte(exp, initregs->rg_a & 0xFF, "A")
#define setB(value)    initregs->rg_b = value
#define assertB(exp)   assertByte(exp, initregs->rg_b & 0xFF, "B")
#define setX(value)    initregs->rg_x = Micro(value)
#define assertX(exp)   assertWord(exp, Host(initregs->rg_x), "X")
#define setY(value)    initregs->rg_y = Micro(value)
#define assertY(exp)   assertWord(exp, Host(initregs->rg_y), "Y")

/**
 * Set the condition codes but don't turn off interrupts.
 */
#define setCC(value)   initregs->rg_cc = (value & 0xAF)
#define assertDP(exp)   assertByte(exp, initregs->rg_dp & 0xFF, "DP")

#endif
