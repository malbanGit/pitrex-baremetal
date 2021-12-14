#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "cputest.h"
#include "initregs.h"
#include "framework.h"

#ifndef FALSE
#define FALSE (0!=0)
#endif
#ifndef TRUE
#define TRUE (0==0)
#endif

int DEBUG = TRUE;
int dry_run = FALSE;

static inline void checkDP(void) {
  // hook for specific roms eg starwars to assert that DP never points to I/O space!
}

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


#ifdef EMULATOR
unsigned int saved_code_addr = 0;
#define callinst(proc, code) do { runinst(#proc, code); } while (0)
#define calltest(proc)       do { runtest(#proc); } while (0)
#endif

#ifdef SBT
#define callinst(proc, code) do { runinst(proc, code); if (!dry_run) proc(); } while (0)
#define calltest(proc)       do { runtest(#proc); if (!dry_run) proc(); } while (0)

void ANDA1(void);
void ANDA2(void);
void ANDA3(void);
void NEG0(void);
void NEG1(void);
void NEG2(void);
void NEG80(void);
void BITimm(void);
void COMA(void);
void DAA1(void);
void DAA2(void);
void DAA3(void);
void DAA4(void);
void DAA5(void);
void DAA6(void);
void DAA7(void);
void DAA8(void);
void EXGdx(void);
void TFRyx(void);
void TFRax(void);
void TFRxb(void);
void MUL(void);
void MUL2(void);
void SEX1(void);
void SEX2(void);
void TSTmemory(void);
void BGT1(void);
void BGT2(void);
void BGT3(void);
void BGT4(void);
void BGT5(void);
void BHI1(void);
void BLE1(void);
void BLE2(void);
void BLE3(void);
void BLE4(void);
void BLE5(void);
void ASRMem(void);
void ASRB1(void);
void ASRB2(void);
void LSL1(void);
void LSL2(void);
void LSL3(void);
void LSR1(void);
void LSR2(void);
void LSR3(void);
void ROLB1(void);
void ROLB2(void);
void ROLB3(void);
void RORB1(void);
void RORB2(void);
void RORB3(void);
void ABX1(void);
void ABX2(void);
void ADCANoC1(void);
void ADCANoC2(void);
void ADCAWiC(void);
void ADCAWiHC(void);
void ADDB1(void);
void ADDB2(void);
void ADDB3(void);
void ADDB4(void);
void ADDANoC(void);
void ADDAWiC(void);
void ADDDNoC(void);
void ADDD1(void);
void ADDD2(void);
void ADDD3(void);
void INCA1(void);
void INCA2(void);
void INCA3(void);
void CMP1(void);
void CMP2(void);
void CMP3(void);
void CMP16(void);
void DECA0x32(void);
void DECA0x80(void);
void DECA0x00(void);
void SBCB(void);
void SBCA1(void);
void SBCA2(void);
void SBCA3(void);
void SBCA4(void);
void SUBA1(void);
void SUBA2(void);
void SUBB1(void);
void SUBB2(void);
void SUBBY(void);
void LEAXpcr(void);
void LEAXincby2(void);
void LEAXno(void);
void LEAXinc1(void);
void LEAXinc2(void);
void Doffset1(void);
void Doffset2(void);
#endif

/**
 * Tests AND A with a value ($F) loaded from the direct page.
 */
static void ANDA(void)
{
    static char testins[] = {0x94, 0xEF, RTS};
    char dp;

    fprintf(stdout, "=================================================== ANDA:\n");
    fprintf(stdout, "--------------------------------------------------- ANDA1\n");
    setA(0x8B);
    setCC(0x02);
    setDP();
    dp = getDP();
    writeDPloc(0xEF, 0x0F);
    callinst(ANDA1, testins);
    assertA(0x0B);
    assertCC(0, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);
    assertDP(dp);
    fprintf(stdout, "--------------------------------------------------- ANDA2\n");

    /* Test where result is 0 */
    setA(0x10);
    calltest(ANDA2);
    assertA(0x00);
    assertCC(1, CC_Z);
    assertCC(0, CC_N);
    fprintf(stdout, "--------------------------------------------------- ANDA3\n");

    /* Test where the result becomes negative. */
    writeDPloc(0xEF, 0xF0);
    setA(0xE0);
    calltest(ANDA3);
    assertA(0xE0);
    assertCC(0, CC_Z);
    assertCC(1, CC_N);
}

/**
 * Tests negation of the A register.
 */
static void NEG(void) {
    static char testins[] = {0x40, RTS};

    fprintf(stdout, "=================================================== NEG:\n");
    fprintf(stdout, "--------------------------------------------------- NEG0\n");
    copydata(CODESTRT, testins, sizeof testins);
    /* Negate 0 */
    setRegs(0,0,0,0,0);
    setCC(0);
    calltest(NEG0);
    assertRegs(0,0,0,0,0);
    assertCC(0, CC_C);
    assertCC(0, CC_V);
    fprintf(stdout, "--------------------------------------------------- NEG1  A=1  CC=0  ->  A=FF C=1 V=0\n");

    /* Negate 1 */
    setRegs(1,0,0,0,0);
    setCC(0);
    calltest(NEG1);
    assertRegs(0xFF,0,0,0,0);
    assertCC(1, CC_C);
    assertCC(0, CC_V);
    fprintf(stdout, "--------------------------------------------------- NEG2\n");

    /* Negate 2 */
    setRegs(2,0,0,0,0);
    setCC(0);
    calltest(NEG2);
    assertRegs(0xFE,0,0,0,0);
    assertCC(1, CC_C);
    assertCC(0, CC_V);
    fprintf(stdout, "--------------------------------------------------- NEG80\n");

    /* Negate 0x80 */
    setRegs(0x80,0,0,0,0);
    setCC(0);
    calltest(NEG80);
    assertRegs(0x80,0,0,0,0);
    assertCC(1, CC_C);
    assertCC(1, CC_V);
    assertCC(0, CC_Z);
    assertCC(1, CC_N);
}

static void BITimm_(void) {
    static char testins[] = {0x85, 0xAA, RTS};

    fprintf(stdout, "=================================================== BITimm:  A=8B CC=0F  ->  A=8B C0 Z0 V0 N1\n");
    setA(0x8B);
    setCC(0x0F);
    copydata(CODESTRT, testins, sizeof testins);
    calltest(BITimm);
    assertA(0x8B);
    assertCC(1, CC_N);
    assertCC(0, CC_Z);
    assertCC(0, CC_V);
    assertCC(1, CC_C);
}

/**
 * Complement register A.
 */
static void COMA_(void) {
    static char testins[] = {0x43, RTS};

    fprintf(stdout, "=================================================== COMA:\n");
    setCC(0);
    setA(0x74);
    callinst(COMA, testins);
    assertA(0x8B);
    assertCC(1, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(1, CC_N);
}

/**
 * Decimal Addition Adjust.
 * The Half-Carry flag is not affected by this instruction.
 * The Negative flag is set equal to the new value of bit 7 in Accumulator A.
 * The Zero flag is set if the new value of Accumulator A is zero; cleared otherwise.
 * The affect this instruction has on the Overflow flag is undefined.
 * The Carry flag is set if the BCD addition produced a carry; cleared otherwise.
 */
static void DAA(void) {
    static char testins[] = {0x19, RTS};

    fprintf(stdout, "=================================================== DAA:\n");
    fprintf(stdout, "--------------------------------------------------- DAA1  CC=0 A=7F  ->  A=85 H0 C0 Z0 N1\n");
    setCC(0);
    setA(0x7F);
    copydata(CODESTRT, testins, sizeof testins);
    calltest(DAA1);
    assertA(0x85);
    assertCC(0, CC_C);
    assertCC(0, CC_Z);
    assertCC(1, CC_N);
    assertCC(0, CC_H);
    fprintf(stdout, "--------------------------------------------------- DAA2  CC=0 A=0F  ->  A=15 H0 C0 Z0 N0\n");

    setCC(0);
    setA(0x0F);
    calltest(DAA2);
    assertA(0x15);
    assertCC(0, CC_C);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);
    assertCC(0, CC_H);
    fprintf(stdout, "--------------------------------------------------- DAA3  CC=0 A=99  ->  A=99 H0 C0 Z0 N1\n");

    setCC(0);
    setA(0x99);
    calltest(DAA3);
    assertA(0x99);
    assertCC(0, CC_C);
    assertCC(0, CC_Z);
    assertCC(1, CC_N);
    assertCC(0, CC_H);
    fprintf(stdout, "--------------------------------------------------- DAA4  CC=0,H=1 A=40  ->  A=46 H1 C0 Z0 N0\n");

    /* Perform DAA on LSN if half-carry is set */
    setCC(0);
    setCCflag(1, CC_H);
    setA(0x40);
    calltest(DAA4);
    assertA(0x46);
    assertCC(1, CC_H);
    assertCC(0, CC_N);
    assertCC(0, CC_Z);
    assertCC(0, CC_C);
    fprintf(stdout, "--------------------------------------------------- DAA5\n");

    /* Perform DAA on MSN if carry is set */
    setCC(0);
    setCCflag(1, CC_C);
    setA(0x40);
    calltest(DAA5);
    assertA(0xA0);
    assertCC(0, CC_H);
    assertCC(1, CC_N);
    assertCC(0, CC_Z);
    assertCC(1, CC_C);
    fprintf(stdout, "--------------------------------------------------- DAA6\n");

    /* Perform DAA of 9A */
    setCC(0);
    setCCflag(1, CC_N); /* BUG! 0, or 1, was missing */
    setA(0x9A);
    calltest(DAA6);
    assertA(0x00);
    assertCC(0, CC_H);
    assertCC(0, CC_N);
    assertCC(1, CC_Z);
    assertCC(1, CC_C);
    fprintf(stdout, "--------------------------------------------------- DAA7\n");

    /* Perform DAA of A2 */
    setCC(0);
    setCCflag(1, CC_N); /* BUG! 0, or 1, was missing */
    setA(0xA2);
    calltest(DAA7);
    assertA(0x02);
    assertCC(0, CC_N);
    assertCC(0, CC_Z);
    assertCC(1, CC_C);
}

/**
 * Set A to 0x91, add 0x91, then do DAA.
 * Result should be C=1.
 */
static void DAA8_(void) {
    static char testins[] = {0x8b, 0x91, 0x19, RTS};

    fprintf(stdout, "=================================================== DAA8:  CC=0 A=91  ->  A=82 C1 Z0 N1\n");
    setCC(0);
    setA(0x91);
    copydata(CODESTRT, testins, sizeof testins);
    //printCtl(CTLMEMSIZE); DEBUG = TRUE;
    //calltest(DAA8);
    callinst(DAA8, testins);
    //DEBUG = FALSE; printCtl(CTLMEMSIZE);
    assertA(0x82);
    assertCC(1, CC_N);
    assertCC(0, CC_Z);
    assertCC(1, CC_C);
    //system("head -28 code/DAA8.c|grep -v '// FCB'");
}

/**
 * Exchange D and X.
 */
static void EXGdx_(void) {
    static char testins[] = {0x1E, 0x01, RTS};

    fprintf(stdout, "=================================================== EXGdx:\n");
    setCC(0);
    setA(0x11);
    setB(0x7F);
    setX(0xFF16);
    callinst(EXGdx, testins);
    assertA(0xFF);
    assertB(0x16);
    assertX(0x117F);
}

/**
 * Transfers.
 */
static void TFR(void) {
    /* Transfer Y to X */
    static char instructions[] = {0x1F, 0x21, RTS};

    fprintf(stdout, "=================================================== TFR:\n");
    fprintf(stdout, "--------------------------------------------------- TFRyx\n");
    instructions[1] = 0x21;
    setCC(0);
    setY(0x1234);
    setX(0xFF16);
    callinst(TFRyx, instructions);
    assertY(0x1234);
    assertX(0x1234);
    fprintf(stdout, "--------------------------------------------------- TFRax\n");

    /* Transfer A to X */
    instructions[1] = 0x81;
    setA(0x56);
    setB(0x78);
    callinst(TFRax, instructions);
    assertX(0xFF56);
    fprintf(stdout, "--------------------------------------------------- TFRxb\n");

    /* Transfer X to B */
    instructions[1] = 0x19;
    setX(0x6541);
    setB(0x78);
    callinst(TFRxb, instructions);
    assertX(0x6541);
    assertB(0x41);
}

/**
 * Multiply 0x0C with 0x64. Result is 0x04B0.
 * The Zero flag is set if the 16-bit result is zero; cleared otherwise.
 * The Carry flag is set equal to the new value of bit 7 in Accumulator B.
 */
static void MUL_(void) {
    static char testins[] = {0x3D, RTS};

    fprintf(stdout, "=================================================== MUL_:\n");
    fprintf(stdout, "--------------------------------------------------- MUL\n");
    setCCflag(0, CC_C);
    setCCflag(1, CC_Z);
    setA(0x0C);
    setB(0x64);
    callinst(MUL, testins);
    assertA(0x04);
    assertB(0xB0);
    assertCC(0, CC_Z);
    assertCC(1, CC_C);
    fprintf(stdout, "--------------------------------------------------- MUL2\n");

    setA(0x00);
    setB(0x64);
    calltest(MUL2);
    assertA(0x00);
    assertB(0x00);
    assertCC(1, CC_Z);
    assertCC(0, CC_C);
}

static void SEX(void) {
    static char testins[] = {0x1D, RTS};

    fprintf(stdout, "=================================================== SEX:\n");
    fprintf(stdout, "--------------------------------------------------- SEX1\n");
    setA(0x02);
    setB(0xE6);
    callinst(SEX1, testins);
    assertA(0xFF);
    assertB(0xE6);
    fprintf(stdout, "--------------------------------------------------- SEX2\n");

    setA(0x02);
    setB(0x76);
    calltest(SEX2);
    assertA(0x00);
    assertB(0x76);
}

/**
 * Test the instruction TST.
 * TST: The Z and N bits are affected according to the value
 * of the specified operand. The V bit is cleared.
 */
static void TSTmemory_(void) {
    static char testins[] = {0x4D, RTS};

    fprintf(stdout, "=================================================== TSTmemory:\n");
    setCC(0);
    setA(0xFF);
    copydata(CODESTRT, testins, sizeof testins);
    calltest(TSTmemory);
    assertA(0xFF);
    assertCC(1, CC_N);
    assertCC(0, CC_Z);
    assertCC(0, CC_V);
}

/**
 * Branch on greater than. Signed conditional.
 * BGT = 0x2E
 */
static void BGT(void)
{
    static char instructions[] = {0x2E, 0x03, LDA, 0x01, RTS, LDA, 0x02, RTS};

    fprintf(stdout, "=================================================== BGT:\n");
    fprintf(stdout, "--------------------------------------------------- BGT1  A=0 CC=0 Z=1 N=0 V=0 -> A=1\n");
    setA(0);
    setCC(0x00);
    setCCflag(1, CC_Z);
    setCCflag(0, CC_N);
    setCCflag(0, CC_V);
    callinst(BGT1, instructions);
    assertA(1);

    fprintf(stdout, "--------------------------------------------------- BGT2  A=0 CC=0 Z=0 N=0 V=0 -> A=2\n");
    setCCflag(0, CC_Z);
    setCCflag(0, CC_N);
    setCCflag(0, CC_V);
    calltest(BGT2);
    assertA(2);

    fprintf(stdout, "--------------------------------------------------- BGT3  A=0 CC=0 Z=0 N=1 V=0 -> A=1\n");
    setCCflag(0, CC_Z);
    setCCflag(1, CC_N);
    setCCflag(0, CC_V);
    calltest(BGT3);
    assertA(1);

    fprintf(stdout, "--------------------------------------------------- BGT4  A=0 CC=0 Z=0 N=1 V=1 -> A=2\n");
    setCCflag(0, CC_Z);
    setCCflag(1, CC_N);
    setCCflag(1, CC_V);
    calltest(BGT4);
    assertA(2);

    fprintf(stdout, "--------------------------------------------------- BGT5  A=0 CC=0 Z=1 N=1 V=0 -> A=1\n");
    setCCflag(1, CC_Z);
    setCCflag(1, CC_N);
    setCCflag(0, CC_V);
    calltest(BGT5);
    assertA(1);
}

/**
 * Branch on higher. Unsigned conditional.
 * BHI = 0x22
 */
static void BHI(void)
{
    static char instructions[] = {0x22, 0x03, LDA, 0x01, RTS, LDA, 0x02, RTS};

    fprintf(stdout, "=================================================== BHI:    A=0 CC=0 Z=1 C=0 -> A=1\n");
    setA(0);
    setCC(0x00);
    setCCflag(1, CC_Z);
    setCCflag(0, CC_C);
    callinst(BHI1, instructions);
    assertA(1);
}

static void BLE(void)
{
    static char instructions[] = {0x2F, 0x03, LDA, 0x01, RTS, LDA, 0x02, RTS};

    fprintf(stdout, "=================================================== BLE:\n");
    fprintf(stdout, "--------------------------------------------------- BLE1  A=0 CC=0 Z=1 N=0 V=0 -> A=2\n");
    setA(0);
    setCC(0x00);
    setCCflag(1, CC_Z);
    setCCflag(0, CC_N);
    setCCflag(0, CC_V);
    callinst(BLE1, instructions);
    assertA(2);

    fprintf(stdout, "--------------------------------------------------- BLE2  A=0 CC=0 Z=0 N=0 V=0 -> A=1\n");
    setCCflag(0, CC_Z);
    setCCflag(0, CC_N);
    setCCflag(0, CC_V);
    calltest(BLE2);
    assertA(1);

    fprintf(stdout, "--------------------------------------------------- BLE3  A=0 CC=0 Z=0 N=1 V=0 -> A=2\n");
    setCCflag(0, CC_Z);
    setCCflag(1, CC_N);
    setCCflag(0, CC_V);
    calltest(BLE3);
    assertA(2);

    fprintf(stdout, "--------------------------------------------------- BLE4  A=0 CC=0 Z=0 N=1 V=1 -> A=1\n");
    setCCflag(0, CC_Z);
    setCCflag(1, CC_N);
    setCCflag(1, CC_V);
    calltest(BLE4);
    assertA(1);

    fprintf(stdout, "--------------------------------------------------- BLE5  A=0 CC=0 Z=1 N=1 V=0 -> A=2\n");
    setCCflag(1, CC_Z);
    setCCflag(1, CC_N);
    setCCflag(0, CC_V);
    calltest(BLE5);
    assertA(2);
}

static void ASRMem_() {
    static char instructions[] = { 0x07, 0x02, RTS };
    int result;

    fprintf(stdout, "=================================================== ASRMem: A=0 B=0 X=0 Y=0 U=0 CC=0 DP=??? >$02=F1 -> C=1 V=0 Z=0 N=1 >$02=F8\n");
    setRegs(0,0,0,0,0);
    setCC(0x00);
    setDP(); // initregs->rg_dp = ((*dpLoc) >> 8);
    writeDPloc(0x02, 0xF1);
    callinst(ASRMem, instructions);
    assertCC(1, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(1, CC_N);
    result = readDPloc(0x02);
    assertByte(0xf8, result, "DP[0x02]");
}

/**
 * Shift a byte at 0x0402, because DP = 0x04.
 */
static void ASRB() {
    static char instructions[] = { 0x57, RTS };

    fprintf(stdout, "=================================================== ASRB1:\n");
    setB(0xF2);
    setCC(0x00);
    callinst(ASRB1, instructions);
    assertB(0xF9);
    assertCC(0, CC_C);
    assertCC(0, CC_Z);
    assertCC(1, CC_N);

    fprintf(stdout, "--------------------------------------------------- ASRB2\n");
    setB(0x01);
    calltest(ASRB2);
    assertB(0x00);
    assertCC(1, CC_C);
    assertCC(1, CC_Z);
    assertCC(0, CC_N);
}

/**
 * Test the LSL - Logical Shift Left instruction.
 *  H The affect on the Half-Carry flag is undefined for these instructions.
 *  N The Negative flag is set equal to the new value of bit 7; previously bit 6.
 *  Z The Zero flag is set if the new 8-bit value is zero; cleared otherwise.
 *  V The Overflow flag is set to the Exclusive-OR of the original values of bits 6 and 7.
 *  C The Carry flag receives the value shifted out of bit 7.
 *
 * Logical Shift Left of 0xff in register A
 */
static void LSLA() {
    static char instructions[] = { 0x48, RTS };

    fprintf(stdout, "=================================================== LSL1:\n");
    setCC(0);
    setA(0xFF);
    callinst(LSL1, instructions);
    assertA(0xFE);
    assertCC(1, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(1, CC_N);

    fprintf(stdout, "--------------------------------------------------- LSL2\n");
    /* Logical Shift Left of 1. */
    setCC(0);
    setCCflag(1, CC_V);
    setA(0x01);
    callinst(LSL2, instructions);
    assertA(0x02);
    assertCC(0, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);

    fprintf(stdout, "--------------------------------------------------- LSL3\n");
    /* Logical Shift Left of 0xB8. */
    setCC(0);
    setCCflag(0, CC_V);
    setA(0xB8);
    callinst(LSL3, instructions);
    assertA(0x70);
    assertCC(1, CC_C);
    assertCC(1, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);
}

/**
 * Test the LSR - Logical Shift Right instruction.
 * Logical Shift Right of 0x3E to 0x1F
 */
static void LSRA() {
    static char instructions[] = { 0x44, RTS };

    fprintf(stdout, "=================================================== LSR1:\n");
    setCC(0x0F);
    setA(0x3E);
    callinst(LSR1, instructions);
    assertA(0x1F);
    assertCC(0, CC_C);
    assertCC(1, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);

    fprintf(stdout, "--------------------------------------------------- LSR2\n");
    /* Logical Shift Right of 1 */
    setCCflag(0, CC_C);
    setCCflag(1, CC_V);
    setCCflag(1, CC_N);
    setA(0x01);
    callinst(LSR2, instructions);
    assertA(0x00);
    assertCC(1, CC_C);
    assertCC(1, CC_Z);
    assertCC(0, CC_N);

    fprintf(stdout, "--------------------------------------------------- LSR3\n");
    /* Logical Shift Right of 0xB8.  */
    setCCflag(0, CC_C);
    setCCflag(0, CC_V);
    setA(0xB8);
    callinst(LSR3, instructions);
    assertA(0x5C);
    assertCC(0, CC_C);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);

}

/**
 * Rotate 8-Bit Accumulator or Memory Byte Left through Carry.
 * N The Negative flag is set equal to the new value of bit 7.
 * Z The Zero flag is set if the new 8-bit value is zero; cleared otherwise.
 * V The Overflow flag is set equal to the exclusive-OR of the original values of bits 6 and 7.
 * C The Carry flag receives the value shifted out of bit 7.
 * Rotate 0x89 to 0x13.
 */
static void ROLB() {
    static char instructions[] = { 0x59, RTS };

    fprintf(stdout, "=================================================== ROLB1:\n");
    setB(0x89);
    setCC(0);
    setCCflag(1, CC_N);
    setCCflag(1, CC_C);
    callinst(ROLB1, instructions);
    assertB(0x13);
    assertCC(1, CC_V);
    assertCC(1, CC_C);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);

    fprintf(stdout, "--------------------------------------------------- ROLB2\n");
    /* Logical Shift Left of 1 with carry set */
    setCCflag(1, CC_C);
    setCCflag(1, CC_V);
    setB(0x01);
    callinst(ROLB2, instructions);
    assertB(0x03);
    assertCC(0, CC_V);
    assertCC(0, CC_C);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);

    fprintf(stdout, "--------------------------------------------------- ROLB3\n");
    /* Rotate Left of 0xD8 */
    setCCflag(0, CC_C);
    setCCflag(0, CC_V);
    setB(0xD8);
    callinst(ROLB3, instructions);
    assertB(0xB0);
    assertCC(1, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(1, CC_N);
}

/**
 * Rotate 8-Bit Accumulator or Memory Byte Right through Carry
 * N The Negative flag is set equal to the new value of bit 7 (original value of Carry).
 * Z The Zero flag is set if the new 8-bit value is zero; cleared otherwise.
 * V The Overflow flag is not affected by these instructions.
 * C The Carry flag receives the value shifted out of bit 0.
 */
static void RORB() {
    /* Rotate 0x89 with CC set to 0xC4 */
    static char instructions[] = { 0x56, RTS };

    fprintf(stdout, "=================================================== RORB1:\n");
    setB(0x89);
    setCC(0);
    setCCflag(1, CC_C);
    callinst(RORB1, instructions);
    assertB(0xC4);
    assertCC(1, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(1, CC_N);

    fprintf(stdout, "--------------------------------------------------- RORB2\n");
    /* Rotate 0x89 with CC clear to 0x44 */
    setB(0x89);
    setCC(0);
    setCCflag(0, CC_C);
    callinst(RORB2, instructions);
    assertB(0x44);
    assertCC(1, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);

    fprintf(stdout, "--------------------------------------------------- RORB3\n");
    /* Rotate 0x08 with CC clear to 0x04 */
    setB(0x08);
    setCC(0);
    setCCflag(0, CC_C);
    callinst(RORB3, instructions);
    assertB(0x04);
    assertCC(0, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);
}

/**
 * Add Accumulator B into index register X.
 * The ABX instruction was included in the 6809 instruction set for
 * compatibility with the 6801 microprocessor.
 * void setRegs(a, b, x, y, u)
 */
static void ABX(void)
{
    static char instructions[] = {0x3a, RTS};

    fprintf(stdout, "=================================================== ABX1:\n");
    setRegs(0,0xCE,0x8006,0,0);
    setCC(0x00);
    callinst(ABX1, instructions);
    assertRegs(0, 0xCE, 0x80D4, 0,0);
    assertCC(0, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);
    assertCC(0, CC_H);

    fprintf(stdout, "--------------------------------------------------- ABX2\n");
    setRegs(0,0xD6,0x7FFE,0,0);
    setCC(0x07);
    callinst(ABX2, instructions);
    assertRegs(0, 0xD6, 0x80D4, 0,0);
    assertCC(1, CC_C);
    assertCC(1, CC_V);
    assertCC(1, CC_Z);
    assertCC(0, CC_N);
    assertCC(0, CC_H);
}

/**
 * Add 2 to register A.
 */
static void ADCANoC(void) {
    static char instructions[] = {0x89, 0x02, RTS};

    fprintf(stdout, "=================================================== ADCANoC1:\n");
    copydata(CODESTRT, instructions, sizeof instructions);
    setA(5);
    setCC(0);
    callinst(ADCANoC1, instructions);
    assertA(7);
    assertCC(0, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);
    assertCC(0, CC_H);

    fprintf(stdout, "--------------------------------------------------- ADCANoC2\n");
    /* Test half-carry $E + $2 = $10 */
    setA(0x0E);
    setCC(0);
    calltest(ADCANoC2);
    assertA(0x10);
    assertCC(0, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);
    assertCC(1, CC_H);
}

/**
 * Add $22 and carry to register A ($14).
 */
static void ADCAWiC_(void) {
    static char instructions[] = {0x89, 0x22, RTS};

    fprintf(stdout, "=================================================== ADCAWiC:\n");
    setA(0x14);
    setCC(0);
    setCCflag(1, CC_C);
    setCCflag(1, CC_H);
    callinst(ADCAWiC, instructions);
    assertA(0x37);
    assertCC(0, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);
    assertCC(0, CC_H);
}

/*
 * Test that half-carry is set when adding with a carry.
 */
static void ADCAWiHC_(void) {
    static char instructions[] = {
        0x89, /* ADCA */
        0x2B,  /* value */
        RTS
    };

    fprintf(stdout, "=================================================== ADCAWiHC:\n");
    setCCflag(1, CC_C);
    setA(0x14);
    callinst(ADCAWiHC, instructions);
    assertA(0x40);
    assertCC(0, CC_C);
    assertCC(1, CC_H);
}

/**
 * positive + positive with overflow.
 * B=0x40 + 0x41 becomes 0x81 or -127
 */
static void ADDB1_(void) {
    static char instructions[] = {0xCB, 0x41, RTS};

    fprintf(stdout, "=================================================== ADDB1:\n");
    setCC(0);
    setB(0x40);
    copydata(CODESTRT, instructions, sizeof instructions);
    calltest(ADDB1);
    assertB(0x81);
    assertCC(1, CC_N);
    assertCC(0, CC_Z);
    assertCC(1, CC_V);
    assertCC(0, CC_C);
    assertCC(0, CC_H);
}

/**
 * negative + negative
 * B=0xFF + 0xFF becomes 0xFE or -2
 */
static void ADDB2_(void) {
    static char instructions[] = {0xCB, 0xFF, RTS};

    fprintf(stdout, "=================================================== ADDB2:\n");
    setCC(0);
    setB(0xFF);
    copydata(CODESTRT, instructions, sizeof instructions);
    //printCtl(CTLMEMSIZE); DEBUG = TRUE;
    calltest(ADDB2);
    //DEBUG = FALSE; printCtl(CTLMEMSIZE);
    assertB(0xFE);
    assertCC(1, CC_N);
    assertCC(0, CC_Z);
    assertCC(0, CC_V);
    assertCC(1, CC_C);
    //systemf1("head -28 code/%s.c|grep -v '// FCB'", "ADDB2");
}

/**
 * negative + negative with overflow
 * B=0xC0 + 0xBF becomes 0x7F or 127
 */
static void ADDB3_(void) {
    static char instructions[] = {0xCB, 0xBF, RTS};

    fprintf(stdout, "=================================================== ADDB3:\n");
    setCC(0);
    setB(0xC0);
    copydata(CODESTRT, instructions, sizeof instructions);
    //printCtl(CTLMEMSIZE); DEBUG = TRUE;
    calltest(ADDB3);
    //DEBUG = FALSE; printCtl(CTLMEMSIZE);
    assertB(0x7F);
    assertCC(0, CC_N);
    assertCC(0, CC_Z);
    assertCC(1, CC_V);
    assertCC(1, CC_C);
    //systemf1("head -28 code/%s.c|grep -v '// FCB'", "ADDB3");
}

/**
 * positive + negative with negative result
 * B=0x02 + 0xFC becomes 0xFE or -2
 */
static void ADDB4_(void) {
    static char instructions[] = {0xCB, 0xFC, RTS};

    fprintf(stdout, "=================================================== ADDB4:\n");
    setCC(0);
    setB(0x02);
    copydata(CODESTRT, instructions, sizeof instructions);
    //printCtl(CTLMEMSIZE); DEBUG = TRUE;
    calltest(ADDB4);
    //DEBUG = FALSE; printCtl(CTLMEMSIZE);
    assertB(0xFE);
    assertCC(1, CC_N);
    assertCC(0, CC_Z);
    assertCC(0, CC_V);
    assertCC(0, CC_C);
    //systemf1("head -28 code/%s.c|grep -v '// FCB'", "ADDB4");
}

/**
 * Add 0x02 to A=0x04.
 */
static void ADDANoC_(void) {
    static char instructions[] = {
        0x8B, /* ADDA */
        0x02, /* value */
        RTS
    };

    fprintf(stdout, "=================================================== ADDANoC:\n");
    setCC(0);
    setA(0x04);
    setB(0x05);
    callinst(ADDANoC, instructions);
    assertA(0x06);
    assertB(0x05);
    assertCC(0, CC_H);
    assertCC(0, CC_N);
    assertCC(0, CC_Z);
    assertCC(0, CC_V);
    assertCC(0, CC_C);
}

/**
 * The overflow (V) bit indicates signed two’s complement overflow, which occurs when the
 * sign bit differs from the carry bit after an arithmetic operation.
 * A=0x03 + 0xFF becomes 0x02
 */
static void ADDAWiC_(void) {
    static char instructions[] = {
        0x8B, /* ADDA */
        0xFF, /* value */
        RTS
    };

    fprintf(stdout, "=================================================== ADDAWiC:\n");
    setCC(0);
    setA(0x03);
    copydata(CODESTRT, instructions, sizeof instructions);
    //printCtl(CTLMEMSIZE); DEBUG = TRUE;
    callinst(ADDAWiC, instructions);
    //DEBUG = FALSE; printCtl(CTLMEMSIZE);
    assertA(0x02);
    assertCC(0, CC_N);
    assertCC(0, CC_V);
    assertCC(1, CC_C);
    //systemf1("head -28 code/%s.c|grep -v '// FCB'", "ADDAWic");
}

/**
 * Add 0x02B0 to D=0x0405 becomes 0x6B5.
 * positive + positive = positive
 */
static void ADDDNoC_(void) {
    static char instructions[] = {
        0xC3, /* ADDD */
        0x02, /* value */
        0xB0,  /* value */
        RTS
    };

    fprintf(stdout, "=================================================== ADDDNoC:\n");
    setCC(0);
    setA(0x04);
    setB(0x05);
    copydata(CODESTRT, instructions, sizeof instructions);
    callinst(ADDDNoC, instructions);
    assertA(0x06);
    assertB(0xB5);
    assertCC(0, CC_N);
    assertCC(0, CC_Z);
    assertCC(0, CC_V);
    assertCC(0, CC_C);
}

/**
 * Add 0xE2B0 to D=0x8405 becomes 0x66B5.
 * negative + negative = positive + overflow
 */
static void ADDD1_(void) {
    static char instructions[] = {
        0xC3, /* ADDD */
        0xE2, /* value */
        0xB0,  /* value */
        RTS
    };

    fprintf(stdout, "=================================================== ADDD1:\n");
    setCC(0);
    setA(0x84);
    setB(0x05);
    copydata(CODESTRT, instructions, sizeof instructions);
    callinst(ADDD1, instructions);
    assertA(0x66);
    assertB(0xB5);
    assertCC(0, CC_N);
    assertCC(0, CC_Z);
    assertCC(1, CC_V);
    assertCC(1, CC_C);
}

/**
 * negative + negative = negative
 * Add 0xE000 to D=0xD000 becomes 0xB000
 */
static void ADDD2_(void) {
    static char instructions[] = {
        0xC3, /* ADDD */
        0xE0, /* value */
        0x00,  /* value */
        RTS
    };

    fprintf(stdout, "=================================================== ADDD2:\n");
    setCC(0);
    setA(0xD0);
    setB(0x00);
    copydata(CODESTRT, instructions, sizeof instructions);
    callinst(ADDD2, instructions);
    assertA(0xB0);
    assertB(0x00);
    assertCC(1, CC_N);
    assertCC(0, CC_Z);
    assertCC(0, CC_V);
    assertCC(1, CC_C);
}

/**
 * positive + positive = negative + overflow
 * Add 0x7000 to D=0x7000 becomes 0xE000
 */
static void ADDD3_(void) {

    static char instructions[] = {
        0xC3, /* ADDD */
        0x70, /* value */
        0x00,  /* value */
        RTS
    };

    fprintf(stdout, "=================================================== ADDD3:\n");
    setCC(0);
    setA(0x70);
    setB(0x00);
    copydata(CODESTRT, instructions, sizeof instructions);
    callinst(ADDD3, instructions);
    assertA(0xE0);
    assertB(0x00);
    assertCC(1, CC_N);
    assertCC(0, CC_Z);
    assertCC(1, CC_V);
    assertCC(0, CC_C);
}

/**
 * Increment register A.
 */
static void INCA(void) {
    static char instructions[] = {
        0x4C, /* INCA */
        RTS
    };

    fprintf(stdout, "=================================================== INCA1:\n");
    setCC(0);
    setA(0x32);
    copydata(CODESTRT, instructions, sizeof instructions);
    callinst(INCA1, instructions);
    assertA(0x33);
    assertCC(0, CC_N);
    assertCC(0, CC_Z);
    assertCC(0, CC_V);
    assertCC(0, CC_C);

    fprintf(stdout, "--------------------------------------------------- INCA2\n");
    /* Test 0x7F - special case */
    setCC(0);
    setA(0x7F);
    copydata(CODESTRT, instructions, sizeof instructions);
    calltest(INCA2);
    assertA(0x80);
    assertCC(1, CC_N);
    assertCC(0, CC_Z);
    assertCC(1, CC_V);
    assertCC(0, CC_C);

    fprintf(stdout, "--------------------------------------------------- INCA3\n");
    /* Test 0xFF - special case */
    setCC(0);
    setA(0xFF);
    copydata(CODESTRT, instructions, sizeof instructions);
    calltest(INCA3);
    assertA(0x00);
    assertCC(0, CC_N);
    assertCC(1, CC_Z);
    assertCC(0, CC_V);
    assertCC(0, CC_C);
}

/**
 * Test indirect mode: CMPA ,Y+
 * We're subtracting 0xff from 0xff and incrementing Y
 */

static void CMP1_(void) {

    static char testins[] = {0xA1, 0xA0, RTS};
    unsigned memloc;

    fprintf(stdout, "=================================================== CMP1:\n");
    /* Set up a byte to test at address 0x205 */
    memloc = setMem(0x205, 0xff);
    setCC(0);
    /* Set register Y to point to that location */
    setRegs(0xff, 0, 0, memloc, 0);
    /* Two bytes of instruction */
    copydata(CODESTRT, testins, sizeof testins);
    callinst(CMP1, testins);
    assertRegs(0xff, 0, 0, memloc+1, 0);
    /* assertCC(1, CC_H); */
    assertCC(1, CC_Z);
}
    
static void CMP2_(void)
{
    /* B = 0xA0, CMPB with 0xA0 */
    static char testins[] = {0xC1, 0xA0, RTS};

    fprintf(stdout, "=================================================== CMP2:\n");
    setCCflag(1, CC_N);
    setCCflag(0, CC_Z);

    setB(0xA0); 
    copydata(CODESTRT, testins, sizeof testins);
    //printCtl(CTLMEMSIZE); DEBUG = TRUE;
    callinst(CMP2, testins);
    //DEBUG = FALSE; printCtl(CTLMEMSIZE);
    assertCC(0, CC_N);
    assertCC(1, CC_Z);
    assertCC(0, CC_V);
    assertCC(0, CC_C);
    //systemf1("head -28 code/%s.c|grep -v '// FCB'", "CMP2");
}
    
static void CMP3_(void)
{
    static char testins[] = {0xC1, 0xA0, RTS};

    fprintf(stdout, "=================================================== CMP3:\n");
    /* B = 0x70, CMPB with 0xA0 */
    /* positive - negative = negative + overflow */
    setCC(0);
    setB(0x70); 
    copydata(CODESTRT, testins, sizeof testins);
    //printCtl(CTLMEMSIZE); DEBUG = TRUE;
    callinst(CMP3, testins);
    //DEBUG = FALSE; printCtl(CTLMEMSIZE);
    assertCC(1, CC_C);
    assertCC(1, CC_V);
    assertCC(0, CC_Z);
    assertCC(1, CC_N);
    //systemf1("head -28 code/%s.c|grep -v '// FCB'", "CMP3");
}

/**
 * Compare 0x5410 with 0x5410.
 */
static void CMP16_(void)
{
    static char testins[] = {0xBC, 0, 0, RTS}; // make a testins properly...
    unsigned memloc;

    fprintf(stdout, "=================================================== CMP16:\n");
    setCC(0x23);
    setX(0x5410);
    memloc = setMem(0x33, 0x54);
    (void)setMem(0x34, 0x10);

    testins[1] = memloc >> 8;
    testins[2] = memloc & 255;
    copydata(CODESTRT, testins, sizeof testins);
    callinst(CMP16, testins);
    
    assertX(0x5410);
    assertCC(0, CC_C);
    assertCC(0, CC_V);
    assertCC(1, CC_Z);
    assertCC(0, CC_N);
}

/**
 * Decrement register A.
 */
static void DECA(void) {
    static char testins[] = {0x4A, RTS };

    fprintf(stdout, "=================================================== DECA0x32:\n");
    setCC(0);
    setA(0x32);
    copydata(CODESTRT, testins, sizeof testins);
    callinst(DECA0x32, testins);
    assertA(0x31);
    assertCC(0, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);

    fprintf(stdout, "--------------------------------------------------- DECA0x80\n");
    /* Test 0x80 - special case */
    setA(0x80);
    calltest(DECA0x80);
    assertA(0x7F);
    assertCC(0, CC_C);
    assertCC(1, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);

    fprintf(stdout, "--------------------------------------------------- DECA0x00\n");
    /* Test 0x00 - special case */
    setA(0x00);
    calltest(DECA0x00);
    assertA(0xFF);
    assertCC(0, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(1, CC_N);
}

/**
 * Test the subtraction with carry instruction.
 * B=0x35 - addr(0x503)=0x3 - C=1 becomes 0x31
 * SBCB dp+03
 */
static void SBCB_(void) {
    static char testins[] = {0xD2, 0x03, RTS};

    fprintf(stdout, "=================================================== SBCB:\n");
    setDP();
    writeDPloc(0x03, 0x03);
    setB(0x35);
    setCC(0);
    setCCflag(1, CC_C);
    copydata(CODESTRT, testins, sizeof testins);
    callinst(SBCB, testins);
    assertB(0x31);
    assertCC(0, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);
    /* assertCC(1, CC_H); */
}

/**
 * Test the SBCA instruction.
 * A=0xFF - 0xFE - C=1 becomes 0x00
 */
static void SBCA1_(void) {
    static char testins[] = {0x82, 0xFE, RTS};

    fprintf(stdout, "=================================================== SBCA1:\n");
    setCCflag(1, CC_C);
    setCCflag(1, CC_N);
    setCCflag(0, CC_Z);
    setA(0xFF);
    copydata(CODESTRT, testins, sizeof testins);
    //printCtl(CTLMEMSIZE); DEBUG = TRUE;
    callinst(SBCA1, testins);
    //DEBUG = FALSE; printCtl(CTLMEMSIZE);
    assertA(0);
    assertCC(0, CC_C);
    assertCC(0, CC_V);
    assertCC(1, CC_Z);
    assertCC(0, CC_N);
    //systemf1("head -28 code/%s.c|grep -v '// FCB'", "SBCA1");
}

/**
 * Test the SBCA instruction.
 * A=0x00 - 0xFF - C=0 becomes 0x01
 */
static void SBCA2_(void) {
    static char testins[] = {0x82, 0xFF, RTS};

    fprintf(stdout, "=================================================== SBCA2:\n");
    setCCflag(0, CC_C);
    setCCflag(1, CC_N);
    setCCflag(0, CC_Z);
    setCCflag(1, CC_V);
    setA(0x00);
    copydata(CODESTRT, testins, sizeof testins);
    //printCtl(CTLMEMSIZE); DEBUG = TRUE;
    callinst(SBCA2, testins);
    //DEBUG = FALSE; printCtl(CTLMEMSIZE);
    assertA(0x01);
    assertCC(1, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);
    //systemf1("head -28 code/%s.c", "SBCA2");
}

/**
 * Test the SBCA instruction.
 * A=0x00 - 0x01 - C=0 becomes 0xFF
 */
static void SBCA3_(void) {
    static char testins[] = {0x82, 0x01, RTS};

    fprintf(stdout, "=================================================== SBCA3:\n");
    setCCflag(0, CC_C);
    setCCflag(1, CC_N);
    setCCflag(0, CC_Z);
    setCCflag(1, CC_V);
    setA(0x00);
    copydata(CODESTRT, testins, sizeof testins);
    callinst(SBCA3, testins);
    assertA(0xFF);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(1, CC_N);
}

/**
 * Test the SBCA instruction.
 * A=0x00 - 0xFF - C=1 becomes 0x00
 */
static void SBCA4_(void) {
    static char testins[] = {0x82, 0xFF, RTS};

    fprintf(stdout, "=================================================== SBCA4:\n");
    setCCflag(1, CC_N);
    setCCflag(0, CC_Z);
    setCCflag(1, CC_V);
    setCCflag(1, CC_C);
    setA(0x00);
    copydata(CODESTRT, testins, sizeof testins);
    //printCtl(CTLMEMSIZE); DEBUG = TRUE;
    callinst(SBCA4, testins);
    //DEBUG = FALSE; printCtl(CTLMEMSIZE);
    assertA(0x00);
    assertCC(0, CC_N);
    assertCC(1, CC_Z);
    assertCC(0, CC_V);
    assertCC(1, CC_C);
    //systemf1("head -28 code/%s.c", "SBCA4");
}

/**
 * Test the SUBA instruction.
 * The overflow (V) bit indicates signed two’s complement overflow, which
 * occurs when the sign bit differs from the carry bit after an arithmetic
 * operation.
 * A=0x00 - 0xFF becomes 0x01
 * positive - negative = positive
 */
static void SUBA1_(void) {
    static char testins[] = {0x80, 0xFF, RTS};

    fprintf(stdout, "=================================================== SUBA1:\n");
    setCCflag(1, CC_C);
    setCCflag(1, CC_N);
    setCCflag(0, CC_Z);
    setCCflag(1, CC_V);
    setA(0x00);
    copydata(CODESTRT, testins, sizeof testins);
    //printCtl(CTLMEMSIZE); DEBUG = TRUE;
    callinst(SUBA1, testins);
    //DEBUG = FALSE; printCtl(CTLMEMSIZE);
    assertA(0x01);
    assertCC(1, CC_C);
    assertCC(0, CC_V);
    assertCC(0, CC_Z);
    assertCC(0, CC_N);
    //systemf1("head -28 code/%s.c|grep -v '// FCB'", "SUBA1");
}

/**
 * A=0x00 - 0x01 becomes 0xFF
 * positive - positive = negative
 */
static void SUBA2_(void) {
    static char testins[] = {0x80, 0x01, RTS};

    fprintf(stdout, "=================================================== SUBA2:\n");
    setCCflag(1, CC_C);
    setCCflag(1, CC_N);
    setCCflag(0, CC_Z);
    setCCflag(1, CC_V);
    setA(0x00);
    copydata(CODESTRT, testins, sizeof testins);
    callinst(SUBA2, testins);
    assertA(0xFF);
    assertCC(1, CC_N);
    assertCC(0, CC_Z);
    assertCC(0, CC_V);
    assertCC(1, CC_C);
}

/**
 * Test the subtraction instruction.
 * IMMEDIATE mode:   B=0x02 - 0xB3  becomes 0x4F
 * positive - negative = positive
 */
static void SUBB1_(void) {
    static char testins[] = {0xC0, 0xB3, RTS};

    fprintf(stdout, "=================================================== SUBB1  -> N=0 Z=0 V=0 C=1:\n");
    setB(0x02);
    setCC(0);
    copydata(CODESTRT, testins, sizeof testins);
    //printCtl(CTLMEMSIZE); DEBUG = TRUE;
    callinst(SUBB1, testins);
    //DEBUG = FALSE; printCtl(CTLMEMSIZE);
    assertB(0x4F);
    assertCC(0, CC_N);
    assertCC(0, CC_Z);
    assertCC(0, CC_V);
    assertCC(1, CC_C);
    //systemf1("head -28 code/%s.c|grep -v '// FCB'", "SUBB1");
}

/**
 * Test the subtraction instruction.
 * IMMEDIATE mode:   B=0x02 - 0x81  becomes 0x81
 * positive - negative = negative + overflow
 */
static void SUBB2_(void) {
    static char testins[] = {0xC0, 0x81, RTS};

    fprintf(stdout, "=================================================== SUBB2: -> N=1 Z=0 V=1 C=1\n");
    setB(0x02);
    setCC(0);
    copydata(CODESTRT, testins, sizeof testins);
    //printCtl(CTLMEMSIZE); DEBUG = TRUE;
    callinst(SUBB2, testins);
    //DEBUG = FALSE; printCtl(CTLMEMSIZE);
    assertB(0x81);
    assertCC(1, CC_N);
    assertCC(0, CC_Z);
    assertCC(1, CC_V);
    assertCC(1, CC_C);
    //systemf1("head -28 code/%s.c|grep -v '// FCB'", "SUBB2");
}

/**
 * Example from Programming the 6809.
 * 0x03 - 0x21 = 0xE2
 * positive - positive = negative
 */
static void SUBBY_(void) {
    unsigned memloc;
    static char testins[] = {0xE0, 0xA4, RTS};

    fprintf(stdout, "=================================================== SUBBY: -> N=1 Z=0 V=0 C=1\n");
    setB(0x03);
    setCC(0);
    setCCflag(1, CC_Z);
    memloc = setMem(0x21, 0x21);
    setY(memloc);
    callinst(SUBBY, testins);
    assertB(0xE2);
    assertCC(1, CC_N);
    assertCC(0, CC_Z);
    assertCC(0, CC_V);
    assertCC(1, CC_C);
}

// indirect.c

/**
 */
static void LEAXpcr_(void)
{
    static char instructions[] = {0x30, 0x8C, 0xE4, RTS};

    fprintf(stdout, "=================================================== SUBBY: -> N=1 Z=0 V=0 C=1\n");
    setCC(0x00);
    setX(0x1234);
    callinst(LEAXpcr, instructions);
    assertX(CODESTRT - 25);
    assertCC(0, CC_Z);
}

/*
 * Increment X by 2.
 */
static void LEAXincby2_(void)
{
    static char instructions[] = {0x30, 0x02, RTS};

    fprintf(stdout, "=================================================== LEAXincby2:\n");
    setCC(0x00);
    setX(0x1234);
    callinst(LEAXincby2, instructions);
    assertX(0x1236);
    assertCC(0, CC_Z);
}

/**
 * LEAX from Y with no offset.
 */
static void LEAXno_(void)
{
    static char instructions[] = {0x30, 0xA4, RTS};

    fprintf(stdout, "=================================================== LEAXno:\n");
    setCC(0x00);
    setX(0x1234);
    setY(0x4321);
    callinst(LEAXno, instructions);
    assertX(0x4321);
    assertCC(0, CC_Z);
}

static void LEAXinc_(void)
{
    static char instructions[] = {0x30, 0xA0, RTS};

    fprintf(stdout, "=================================================== LEAXinc1:\n");
    setCC(0x00);
    setX(0x1234);
    setY(0x4321);
    callinst(LEAXinc1, instructions);
    assertX(0x4321);
    assertY(0x4322);
    assertCC(0, CC_Z);

    fprintf(stdout, "--------------------------------------------------- LEAXinc2\n");
    setCC(0x00);
    setY(0x0);
    callinst(LEAXinc2, instructions);
    assertX(0x0);
    assertY(0x1);
    assertCC(1, CC_Z);
}

/**
 * LEAX D,Y
 */
static void Doffset_(void)
{
    static char instructions[] = {0x30, 0xAB, RTS};

    fprintf(stdout, "=================================================== Doffset1:\n");
    setCC(0x00);
    setX(0xABCD);
    setY(0x804F);
    setA(0x80);
    setB(0x01);
    callinst(Doffset1, instructions);
    assertX(0x50);
    assertCC(0, CC_Z);

    fprintf(stdout, "--------------------------------------------------- Doffset2 CC=0x28 X=0EFA Y=0EF8 A=FF B=82 -> X=8E7A Z=0\n");
    setCC(0x28);
    setX(0x0EFA);
    setY(0x0EF8);
    setA(0xFF);
    setB(0x82);
    callinst(Doffset2, instructions);
    assertX(0x0E7A);
    assertCC(0, CC_Z);
}

int main(int argc, char **argv)
{
    if ((argc == 2) && (strcmp(argv[1], "--dry-run") == 0)) {
      dry_run = TRUE; // allow us to generate the C thunks without stopping on errors...
      fprintf(stderr, "Generating translations.  Ignore error messages.\n");
    }
    setupCtl();
    ANDA();
    NEG();
    BITimm_();
    COMA_();
    DAA();
    DAA8_();
    EXGdx_();
    TFR();
    MUL_();
    SEX();
    TSTmemory_();
    BGT();
    BHI();
    BLE();
    ASRMem_();
    ASRB();
    LSLA();
    LSRA();
    ROLB();
    RORB();
    ABX();
    ADCANoC();
    ADCAWiC_();
    ADCAWiHC_();
    ADDB1_();
    ADDB2_();
    ADDB3_();
    ADDB4_();
    ADDANoC_();
    ADDDNoC_(); // added.
    ADDAWiC_();
    ADDD1_();
    ADDD2_();
    ADDD3_();
    INCA();
    CMP1_();
    CMP2_();
    CMP3_();
    CMP16_();
    DECA();
    SBCB_();
    SBCA1_();
    SBCA2_();
    SBCA3_();
    SBCA4_();
    SUBA1_();
    SUBA2_();
    SUBB1_();
    SUBB2_();
    SUBBY_();
    LEAXpcr_();
    LEAXincby2_();
    LEAXinc_();
    LEAXno_();
    Doffset_();
    fprintf(stderr, "************ opcode tests successful *************\n");
    exit(0);
    return 0;
}

#ifdef SBT

typedef uint32_t UINT32;
typedef int32_t SINT32;
typedef uint16_t UINT16;
typedef int16_t SINT16;
typedef uint8_t UINT8;
typedef int8_t SINT8;

UINT8 /* memory[0x10000], */  *memory_DP = &memory[0x0000];

// Modify this header to suit your target...
SINT32 res, A, B, /* D, */ C;
UINT16 PC, X, Y, S, U, Z, DP, arg, ea, val;
UINT8 E, F, I, N, H, V, CC, msn, lsn;

#define JUMP continue

// BUG? changing H from 0x10 to 0x20 ... NOW BACK AGAIN

#define simplify_flags() do { N = (((SINT8)N < 0)?1:0);     \
                              V = (((SINT8)V < 0)?1:0);     \
                              Z = ((Z == 0)?1:0);           \
                              C = ((C != 0)?1:0);           \
                              H = (((H & 0x10) != 0)?1:0);  \
                         } while (0)
#define restore_flags() do { N = (N==1 ? 0x80:0);    \
                             V = (V==1 ? 0x80:0);    \
                             Z = (Z==1 ? 0 : 1);     \
                             C = (C==1 ? 0x100 : 0); \
                             H = (H==1 ? 0x10 : 0);  \
                           } while (0)

#ifdef NEVER
#define restore_flags() do { N = (N ? 0x80:0);    \
                             V = (V ? 0x80:0);    \
                             Z = (Z ? 0 : 1);     \
                             C = (C ? 0x100 : 0); \
                             H = (H ? 0x20 : 0);  \
                           } while (0)
#endif  

void mon(char *str) { // SBT version
  int off=strlen("ENTRYPT  PSHS  U,Y,X,DP,B,A,CC         ; ");
  if (!DEBUG) return;
  simplify_flags();
  CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C;
  fprintf(stdout, "PC=%c%c%c%c A=%02x B=%02x X=%04x Y=%04x S=%04x U=%04x CC=%02x DP=%02x E%cF%cH%cI%cC%cZ%cV%cN%c   %s",
          str[off+0], str[off+1], str[off+2], str[off+3], A, B, X, Y, S, U, CC, (UINT8)(DP>>8),
          E+'0', F+'0', H+'0', I+'0', C+'0', Z+'0', V+'0', N+'0', str);
  restore_flags();
  //printMem(0x046e, 2);fprintf(stdout, "\n");
}


//  S = (save_s[0]&255)<<8 | (save_s[1]&255); 

#define INITREGS()  do {   PC = 0x400;          \
  memory[0x03FE] = 0xFF; memory[0x03FF] = 0xFF; S = 0x3FE; \
  A  = initregs->rg_a&255; \
  B  = initregs->rg_b&255; \
  X  = Host(initregs->rg_x&65535); \
  Y  = Host(initregs->rg_y&65535); \
  U  = Host(initregs->rg_u&65535); \
  CC = initregs->rg_cc&255; \
  C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1; \
  restore_flags(); \
  DP = (initregs->rg_dp&255) << 8; \
  memory_DP = &memory[DP]; \
  } while(0)


#define RESTOREREGS() do { ; } while(0)
/*
#define RESTOREREGS() do {                      \
  simplify_flags(); \
  CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; \
  initregs->rg_cc = CC; \
  initregs->rg_a = A; \
  initregs->rg_b = B; \
  initregs->rg_dp = (DP>>8)&255; \
  initregs->rg_x = Micro(X); \
  initregs->rg_y = Micro(Y); \
  initregs->rg_u = Micro(U); \
  } while(0)
*/


#define DEFPROC(proc) void proc(void) { \
  INITREGS(); \
  for (;;) { \
    switch (PC) {
#define ENDPROC() \
  case 0xFFFF: /* fake return address means return to framework.c caller */ \
      RESTOREREGS(); \
      return; \
       \
    default: \
      fprintf (stdout, "Unknown jump to %04x\n", PC); \
      if (dry_run) return; else exit (1);		      \
    } \
  } \
}

DEFPROC(BGT1)
#include "code/BGT1.c"
ENDPROC()
    
DEFPROC(BGT2)
#include "code/BGT2.c"
ENDPROC()
    
DEFPROC(BGT3)
#include "code/BGT3.c"
ENDPROC()
    
DEFPROC(BGT4)
#include "code/BGT4.c"
ENDPROC()
    
DEFPROC(BGT5)
#include "code/BGT5.c"
ENDPROC()
    
DEFPROC(BHI1)
#include "code/BHI1.c"
ENDPROC()
    
DEFPROC(BLE1)
#include "code/BLE1.c"
ENDPROC()
    
DEFPROC(BLE2)
#include "code/BLE2.c"
ENDPROC()
    
DEFPROC(BLE3)
#include "code/BLE3.c"
ENDPROC()
    
DEFPROC(BLE4)
#include "code/BLE4.c"
ENDPROC()
    
DEFPROC(BLE5)
#include "code/BLE5.c"
ENDPROC()
    
DEFPROC(ANDA1)
#include "code/ANDA1.c"
ENDPROC()
    
DEFPROC(ANDA2)
#include "code/ANDA2.c"
ENDPROC()

DEFPROC(ANDA3)
#include "code/ANDA3.c"
ENDPROC()

DEFPROC(NEG0)
#include "code/NEG0.c"
ENDPROC()


DEFPROC(NEG1)
#include "code/NEG1.c"
ENDPROC()


DEFPROC(NEG2)
#include "code/NEG2.c"
ENDPROC()

DEFPROC(NEG80)
#include "code/NEG80.c"
ENDPROC()

DEFPROC(BITimm)
#include "code/BITimm.c"
ENDPROC()


DEFPROC(COMA)
#include "code/COMA.c"
ENDPROC()

DEFPROC(DAA1)
#include "code/DAA1.c"
ENDPROC()

DEFPROC(DAA2)
#include "code/DAA2.c"
ENDPROC()

DEFPROC(DAA3)
#include "code/DAA3.c"
ENDPROC()

DEFPROC(DAA4)
#include "code/DAA4.c"
ENDPROC()

DEFPROC(DAA5)
#include "code/DAA5.c"
ENDPROC()

DEFPROC(DAA6)
#include "code/DAA6.c"
ENDPROC()

DEFPROC(DAA7)
#include "code/DAA7.c"
ENDPROC()

DEFPROC(DAA8)
#include "code/DAA8.c"
ENDPROC()

DEFPROC(EXGdx)
#include "code/EXGdx.c"
ENDPROC()

DEFPROC(TFRyx)
#include "code/TFRyx.c"
ENDPROC()

DEFPROC(TFRax)
#include "code/TFRax.c"
ENDPROC()

DEFPROC(TFRxb)
#include "code/TFRxb.c"
ENDPROC()

DEFPROC(MUL)
#include "code/MUL.c"
ENDPROC()

DEFPROC(MUL2)
#include "code/MUL2.c"
ENDPROC()

DEFPROC(SEX1)
#include "code/SEX1.c"
ENDPROC()

DEFPROC(SEX2)
#include "code/SEX2.c"
ENDPROC()

DEFPROC(TSTmemory)
#include "code/TSTmemory.c"
ENDPROC()

DEFPROC(ASRMem)
#include "code/ASRMem.c"
ENDPROC()

DEFPROC(ASRB1)
#include "code/ASRB1.c"
ENDPROC()

DEFPROC(ASRB2)
#include "code/ASRB2.c"
ENDPROC()

DEFPROC(LSL1)
#include "code/LSL1.c"
ENDPROC()

DEFPROC(LSL2)
#include "code/LSL2.c"
ENDPROC()

DEFPROC(LSL3)
#include "code/LSL3.c"
ENDPROC()

DEFPROC(LSR1)
#include "code/LSR1.c"
ENDPROC()

DEFPROC(LSR2)
#include "code/LSR2.c"
ENDPROC()

DEFPROC(LSR3)
#include "code/LSR3.c"
ENDPROC()

DEFPROC(ROLB1)
#include "code/ROLB1.c"
ENDPROC()

DEFPROC(ROLB2)
#include "code/ROLB2.c"
ENDPROC()

DEFPROC(ROLB3)
#include "code/ROLB3.c"
ENDPROC()

DEFPROC(RORB1)
#include "code/RORB1.c"
ENDPROC()

DEFPROC(RORB2)
#include "code/RORB2.c"
ENDPROC()

DEFPROC(RORB3)
#include "code/RORB3.c"
ENDPROC()

DEFPROC(ABX1)
#include "code/ABX1.c"
ENDPROC()

DEFPROC(ABX2)
#include "code/ABX2.c"
ENDPROC()

DEFPROC(ADCANoC1)
#include "code/ADCANoC1.c"
ENDPROC()

DEFPROC(ADCANoC2)
#include "code/ADCANoC2.c"
ENDPROC()

DEFPROC(ADCAWiC)
#include "code/ADCAWiC.c"
ENDPROC()

DEFPROC(ADCAWiHC)
#include "code/ADCAWiHC.c"
ENDPROC()

DEFPROC(ADDB1)
#include "code/ADDB1.c"
ENDPROC()

DEFPROC(ADDB2)
#include "code/ADDB2.c"
ENDPROC()

DEFPROC(ADDB3)
#include "code/ADDB3.c"
ENDPROC()

DEFPROC(ADDB4)
#include "code/ADDB4.c"
ENDPROC()

DEFPROC(ADDANoC)
#include "code/ADDANoC.c"
ENDPROC()

DEFPROC(ADDAWiC)
#include "code/ADDAWiC.c"
ENDPROC()

DEFPROC(ADDDNoC)
#include "code/ADDDNoC.c"
ENDPROC()

DEFPROC(ADDD1)
#include "code/ADDD1.c"
ENDPROC()

DEFPROC(ADDD2)
#include "code/ADDD2.c"
ENDPROC()

DEFPROC(ADDD3)
#include "code/ADDD3.c"
ENDPROC()

DEFPROC(INCA1)
#include "code/INCA1.c"
ENDPROC()

DEFPROC(INCA2)
#include "code/INCA2.c"
ENDPROC()

DEFPROC(INCA3)
#include "code/INCA3.c"
ENDPROC()

DEFPROC(CMP1)
#include "code/CMP1.c"
ENDPROC()

DEFPROC(CMP2)
#include "code/CMP2.c"
ENDPROC()

DEFPROC(CMP3)
#include "code/CMP3.c"
ENDPROC()

DEFPROC(CMP16)
#include "code/CMP16.c"
ENDPROC()

DEFPROC(DECA0x32)
#include "code/DECA0x32.c"
ENDPROC()

DEFPROC(DECA0x80)
#include "code/DECA0x80.c"
ENDPROC()

DEFPROC(DECA0x00)
#include "code/DECA0x00.c"
ENDPROC()

DEFPROC(SBCB)
#include "code/SBCB.c"
ENDPROC()

DEFPROC(SBCA1)
#include "code/SBCA1.c"
ENDPROC()

DEFPROC(SBCA2)
#include "code/SBCA2.c"
ENDPROC()

DEFPROC(SBCA3)
#include "code/SBCA3.c"
ENDPROC()

DEFPROC(SBCA4)
#include "code/SBCA4.c"
ENDPROC()

DEFPROC(SUBA1)
#include "code/SUBA1.c"
ENDPROC()

DEFPROC(SUBA2)
#include "code/SUBA2.c"
ENDPROC()

DEFPROC(SUBB1)
#include "code/SUBB1.c"
ENDPROC()

DEFPROC(SUBB2)
#include "code/SUBB2.c"
ENDPROC()

DEFPROC(SUBBY)
#include "code/SUBBY.c"
ENDPROC()

DEFPROC(LEAXpcr)
#include "code/LEAXpcr.c"
ENDPROC()

DEFPROC(LEAXincby2)
#include "code/LEAXincby2.c"
ENDPROC()

DEFPROC(LEAXno)
#include "code/LEAXno.c"
ENDPROC()

DEFPROC(LEAXinc1)
#include "code/LEAXinc1.c"
ENDPROC()

DEFPROC(LEAXinc2)
#include "code/LEAXinc2.c"
ENDPROC()

DEFPROC(Doffset1)
#include "code/Doffset1.c"
ENDPROC()

DEFPROC(Doffset2)
#include "code/Doffset2.c"
ENDPROC()

#endif /* SBT */
