/*
 * Test of 6809 instructions.
 *
 * The strategy is to allocate one kilobyte for a test area, which will
 * allow the code to load data at locations under and over the code itself.
 * There is a small control routine that is set and then calls the test
 * at location 500. It expects the test to return with a RTS instruction.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cputest.h"
#include "framework.h"

#ifdef EMULATOR
#include "../6809-fallback.h"

// Modify this header to suit your target...
extern SINT32 A, B, C;
extern UINT16 PC, iPC, X, Y, S, U, Z, DP;
extern UINT8 EFI, /* E, F, I,*/ N, H, V  /*, CC*/;
extern int DEBUG;

void mon(char *s) {
  /*if (DEBUG)*/
  if (PC <= 0xff) fprintf(stdout,
          "%04x: A=%02x B=%02x X=%04x Y=%04x S=%04x U=%04x DP=%02x E%cF%cH%cI%cC%cZ%cV%cN%c   %s\n",
          PC,
          A, B, X, Y, S, U,
          (UINT8)(DP>>8), (EFI&1)+'0', ((EFI>>1)&1)+'0', (((H & 0x10) != 0)?1:0)+'0', ((EFI>>2)&1)+'0', (((C & 0x100) != 0)?1:0)+'0', ((Z != 0)?1:0)+'0', (((SINT8)V < 0)?1:0)+'0', (((SINT8)N < 0)?1:0)+'0', s);
  fflush(stdout);
}
#endif

void systemf1(const char *command, const char *param) {
  char tmp[strlen(command)+strlen(param)+2];
  sprintf(tmp, command, param);
  //fprintf(stdout, "Executing %s\n", tmp);
  system(tmp);
}


/* Allocate enough memory to run the instructions in that allows
 * for jumps back and forth. Stack pointer is set at top.
 * initregs must follow immediately.
 */

// CODESTRT: ORG 0
// memory: ORG 0 - 99
// ctlmemory: ORG 100 - 1xx

// unsigned char memory[MEMSIZE+CTLMEMSIZE+REGSIZE+4]; // 4 for save_s and dpLoc

extern unsigned char memory[0x10000];



/* Small code segment to save registers and call test.
 */
unsigned char *ctlmemory /* [100] */ = &memory[MEMSIZE]; /* WARNING! Host byte-sex sensitive */
struct registers *initregs = (struct registers *)&memory[MEMSIZE+CTLMEMSIZE];
char *currtest;


#define CODESTRT 0


/* Store the initial value of the stack pointer. */
unsigned short *save_s = (unsigned short *)&memory[MEMSIZE+CTLMEMSIZE+REGSIZE];
unsigned short *dpLoc  = (unsigned short *)&memory[MEMSIZE+CTLMEMSIZE+REGSIZE+2];

/**
 * Copy the instructions to test.
 * Instructions must begin at CODESTRT
 * The instructions must end with an RTS
 */
void copydata(int start, char *insv, int insc)
{
    register int i;

    for (i = 0; i < insc; i++) {
        memory[start++] = insv[i];
    }
}

void writebyte(int loc, char value)
{
    memory[loc] = value;
}

unsigned setMem(int addr, char value)
{
    memory[addr] = value;
    return (unsigned) addr;
}

void writeword(int loc, unsigned value)
{
  memory[loc++] = Micro(value) >> 8;
  memory[loc]   = Micro(value) & 0xFF;
}

/**
 * Calculate a valid value for the DP register.
 */
static unsigned calcDP(void)
{
    //register unsigned t = 0 /* (unsigned) memory*/;
    //t = (t & 0xff00) + ((t & 0xff) != 0 ? 0x100 : 0x0);
    //return t;
    return 0x0200; // let's always put DP in page 2
}

/*
 * Start location is the argument on the stack.
 */
static int codelen = 0;
void jsrtest(int startloc)
{
  //fprintf(stdout, "Jsr %04x\n", startloc&0xffff);
#ifdef SBT

  // SBT
  int memp;
  FILE *bin, *dat;
  dat = fopen("code/prog.dat", "w");
  fprintf(dat, "code 0000\n");
  fprintf(dat, "label 0000 %s\n", currtest);
  fprintf(dat, "data %04x-03FF\n", codelen);
  fprintf(dat, "code 0400\n");
  fprintf(dat, "label 0400 ENTRYPT\n");
  fprintf(dat, "data 0418-0464\n");
  fclose(dat);
  bin = fopen("code/prog.bin", "wb");
  for (memp = 0; memp < MEMSIZE+CTLMEMSIZE; memp++) {
    fputc(memory[memp], bin);
  }
  fclose(bin);
  systemf1("../6809dasm --sbt --ss code/prog.bin code/prog.dat > code/%s.c", currtest);

#else
#ifdef EMULATOR

  // Emulator
  int CC, E, F, I;
  A = (initregs->rg_a)&0xff;
  B = (initregs->rg_b)&0xff;
  X = (initregs->rg_x)&0xffff;
  Y = (initregs->rg_y)&0xffff;
  U = (initregs->rg_u)&0xffff;
    S = 0x3FE;
    DP = (initregs->rg_dp << 8)&0xff;
    CC = initregs->rg_cc;
    C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1;
    EFI = (E?E_FLAG:0) | (F?F_FLAG:0) | (I?I_FLAG:0);
    N = (N==1 ? 0x80:0);
    V = (V==1 ? 0x80:0);
    Z = (Z==1 ? 0 : 1);
    C = (C==1 ? 0x100 : 0);
    H = (H==1 ? 0x10 : 0);
    PC = startloc;
    (void)cpu_execute(0x7FFFFFFF);
#else

  // Native 6809 - deprecated
#asm
*   ldu 4,s
*   stu save_s,y
    jsr [4,s]
#endasm

#endif
#endif /* SBT */

}

void runcode(char *ctest, char *insv, int insc)
{
    codelen = insc; // length of test code if using SBT.
    copydata(CODESTRT, insv, insc);
#ifdef SBT
// shouln't happen, but has been happening, not sure of circumstances
if (*ctest == '"') fprintf(stderr, "runcode: %s\n", ctest);
#endif
    runtest(ctest);
}

void runtest(char *ctest)
{
    currtest = ctest;
#ifdef SBT
    // SBT
// shouln't happen, but has been happening, not sure of circumstances
if (*ctest == '"') fprintf(stderr, "runtest: %s\n", ctest);
#else
#ifdef EMULATOR
    // Emulator
#else
    // Native 6809
#endif
#endif /* SBT */
    jsrtest((char *)ctlmemory-(char *)memory);
}

int setupCtl(void)
{
  int start;

    start = 0;
    
    /* printf("Memory addr: %X\n", memory); */
    *dpLoc = Micro(calcDP());

    /* Push existing register state on normal stack */
    ctlmemory[start++] = 0x34; ctlmemory[start++] = 0x7F; //  0x347F   (PshS all regs?)

    /* Remember SP value */
    ctlmemory[start++] = 0x10; ctlmemory[start++] = 0xFF; //  0x10FF /* STS */

    ctlmemory[start++] = ((unsigned short)((char *)save_s-(char *)&memory[0])) >> 8;
    ctlmemory[start++] = ((unsigned short)((char *)save_s-(char *)&memory[0])) & 0xFF; //  &save_s;

    /* Change SP to initregs */
    ctlmemory[start++] = 0x10; ctlmemory[start++] = 0xCE; //  0x10CE /* LDS */

    ctlmemory[start++] = ((unsigned short)(unsigned int)((char *)initregs-(char *)&memory[0])) >> 8;
    ctlmemory[start++] = ((unsigned short)(unsigned int)((char *)initregs-(char *)&memory[0])) & 0xff; //  &initregs;

    /* Pull all registers */
    ctlmemory[start++] = 0x35; ctlmemory[start++] = 0x7F; //  0x357F /* PulS all regs */

    /* Call subroutine */
    ctlmemory[start++] = 0xBD; /* JSR */

    ctlmemory[start++] = ((unsigned short)(unsigned int)(CODESTRT)) >> 8;
    ctlmemory[start++] = ((unsigned short)(unsigned int)(CODESTRT)) & 0xFF; //  &memory[CODESTRT] (0000)

    /* Push registers back to initregs */
    ctlmemory[start++] = 0x34; ctlmemory[start++] = 0x7F; //  0x347F /* PshS all regs */

    /* Reset SP to normal stack by loading content of save_s */
                                                                                 // NOT ADDRESS OF save_s ??
    ctlmemory[start++] = 0x10; ctlmemory[start++] = 0xFE; //  0x10FE /* LDS */

    // THIS MAY BE WRONG:
    ctlmemory[start++] = ((unsigned short)((char *)save_s-(char *)&memory[0])) >> 8;
    ctlmemory[start++] = ((unsigned short)((char *)save_s-(char *)&memory[0])) & 0xFF; //  &save_s;

    /* Pull all registers */
    ctlmemory[start++] = 0x35; ctlmemory[start++] = 0x7F; //  0x357F /* PulS all regs */
    ctlmemory[start++] = 0x39; /* RTS */

    
    return start;
}

void printMem(int start, int size)
{
    register int i;

    for (i = start; i < start+size; i++) {
      fprintf(stdout, "%02X", memory[i]);
        if (i % 12 == 11) {
          fprintf(stdout, "\n");
        } else {
          fprintf(stdout, ":");
        }
    }
}

// to use: printCtl(CTLMEMSIZE);
void printCtl(int ctlsize)
{
    register int i;

    // should use 'dumpfile' format from my 'hexdump' program...
    
    /* This is all assuming a 16-bit host! Totally fails when host is an emulator on 32- or 64-bit machine */
    //fprintf(stdout, "Save_s addr: %04x\n", (char *)save_s-(char *)&memory[0]);  /* not interesting */
    //  fprintf(stdout, "Save_s content: %04x\n", (save_s[0]&255)<<8 | (save_s[1]&255));
    // fprintf(stdout, "Memory addr: %04x\n", &memory[0]-&memory[0]); /* always 0 */
    fprintf(stdout, "Initregs addr: %04x  size: %d\n",(unsigned int) ( (char *)initregs-(char *)&memory[0]), (int)REGSIZE);
    printMem((char *)initregs-(char *)&memory[0], REGSIZE);
    // fprintf(stdout, "Preloaded Stack addr: %04x\n", 0x0464); printMem(0x0464, 10); /* same as initregs */
    fprintf(stdout, "\nCtlMem addr: %04x  size: %d\n",(unsigned int) ((char *)ctlmemory-(char *)&memory[0]), (int)CTLMEMSIZE);

    // updated for portability
    for (i = 0; i < ctlsize; i+=2) {
      fprintf(stdout, "%04X", ((ctlmemory[i]&255)<<8) | (ctlmemory[i+1]&255) );
        if (i % 12 == 11) {
          fprintf(stdout, "\n");
        } else {
          fprintf(stdout, ":");
        }
    }
    fprintf(stdout, "\n");
}

/**
 * Read a value from the direct page.
 */
int readDPloc(int offset)
{
    int loc = Host(*dpLoc) /* - (unsigned) memory */ + offset;
    //fprintf(stdout, "readDPloc(%d): memory[%04x] = 0x%02x\n", offset, loc, memory[loc] & 0xFF);
    return memory[loc] & 0xFF;
}

/**
 * Write a value into the direct page.
 */
void writeDPloc(int offset, unsigned char value)
{
    int loc = Host(*dpLoc) /* - (unsigned) memory */ + offset;
    memory[loc] = value;
    //fprintf(stdout, "writeDPloc(%d,%u): memory[%04x] = 0x%02x\n", offset, value, loc, value);
    fprintf(stdout, "# memory[%04x] = 0x%02x\n", loc, value);
}
