#include <stdio.h>
#include "cputest.h"

extern int dry_run;

unsigned short Host(unsigned short Micro) {
  return ((Micro>>8)&255) | ((Micro&255)<<8);
}

unsigned short Micro(unsigned short Host) {
  return ((Host>>8)&255) | ((Host&255)<<8);
}

void setRegs(char a, char b, unsigned x, unsigned y, unsigned u)
{
    initregs->rg_a = a;
    initregs->rg_b = b;
    initregs->rg_x = Micro(x);
    initregs->rg_y = Micro(y);
    initregs->rg_u = Micro(u);
} 

void assertWord(unsigned short exp, unsigned short act, char *reg)
{
    if (exp != act) {
      fprintf(stderr, "%s:%s expected %04x - actual %04x\n", currtest, reg, exp, act);
      if (!dry_run) exit(1);
    }
}

void assertByte(unsigned char exp, unsigned char act, char *reg)
{
    if (exp != act) {
      fprintf(stderr, "%s:%s expected %02x - actual %02x\n", currtest, reg, exp, act);
      if (!dry_run) exit(1);
    }
}

void setDP(void)
{
    initregs->rg_dp = (Host(*dpLoc) >> 8);
}

char getDP(void)
{
  return (Host(*dpLoc)) >> 8;
}


void setCCflag(int exp, int bit)
{
    register int mask = 1 << bit;
    if (exp == 0) {
        initregs->rg_cc &= ~mask;
    } else {
        initregs->rg_cc |= mask;
    }
}

void assertCC(int exp, int bit)
{
    static char names[] = "CVZNIHFE";
    if ((initregs->rg_cc & (1 << bit)) != (exp << bit)) {
      fprintf(stderr, "%s:CC-%c expected %d\n", currtest, names[bit], exp);
      if (!dry_run) exit(1);
    }
}

void assertRegs(char a, char b, unsigned x, unsigned y, unsigned u)
{
    assertByte(a,  initregs->rg_a, "A");
    assertByte(b,  initregs->rg_b, "B");
    assertWord(x,  Host(initregs->rg_x), "X");
    assertWord(y,  Host(initregs->rg_y), "Y");
    assertWord(u,  Host(initregs->rg_u), "U");
} 

void printRegs(void)
{
    fprintf(stdout, "CC=%02x A=%02x B=%02x DP=%02x\n",
            initregs->rg_cc&255, initregs->rg_a&255, initregs->rg_b&255, initregs->rg_dp&255);
    fprintf(stdout, "X=%04x Y=%04x U=%04x\n", Host(initregs->rg_x), Host(initregs->rg_y), Host(initregs->rg_u));
}
