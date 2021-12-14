#ifndef FALSE
#define FALSE (0!=0)
#endif
#ifndef TRUE
#define TRUE (0==0)
#endif

int DEBUG = TRUE;

// WINDOWS
//#include <basetsd.h>
//#define SINT8 INT8
//#define SINT16 INT16
//#define SINT32 INT32
//#define SINT64 INT64

// LINUX
typedef uint32_t UINT32;
typedef int32_t SINT32;
typedef uint16_t UINT16;
typedef int16_t SINT16;
typedef uint8_t UINT8;
typedef int8_t SINT8;


static UINT8 memory[0x10000]; // , *memory_DP = &memory[0xD000];


// The width of these variables is important and part of the design.  Do not widen a
// byte or short to make things more efficient - you will get errors when the high
// bytes get written to unexpectedly.  Do no narrow the 32 or 16 bit variables to
// match the 6809 register sizes - the extra space is used for the carry bit and
// detecting overflow.

static SINT32 res, A, B, /*D,*/ C;  // D is never used. Everything is done with A and B
static UINT16 PC, X, Y, S, U, Z, DP, arg, ea, val;
// During execution of the program, the flag bits use our internal conventions -
// they are NOT always 0 or 1 and even when they are, the sense may be inverted
// from what the 6809 uses.  Flags have to be converted to canonical form when
// loaded/saved/*or printed* in diagnostics. (eg "mon()") Or when CC is compared
// in a dual CPU environment.
// EFI not fully implemented.
static UINT8 E, F, I, N, H, V, CC, msn, lsn;
int clockticks = 0;

unsigned int iCount = 0;

static void checkDP(void) {
  // hook for specific roms eg starwars to assert that DP never points to I/O space!
}

#define JUMP continue
//#define INSTRUCTION_START 
#define INSTRUCTION_START if (clockticks>=maxClockTicks) {goto sbtDone; }

#define E_FLAG 0x80
#define F_FLAG 0x40
#define H_FLAG 0x20
#define I_FLAG 0x10
#define N_FLAG 0x08
#define Z_FLAG 0x04
#define V_FLAG 0x02
#define C_FLAG 0x01


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
  
void regToSTB1()
{
//  /* 6809 registers */
//  static byte cc,dpreg,areg,breg;
//  static word xreg,yreg,ureg,sreg,pcreg;
  A = areg;
  B = breg;
  X = xreg;
  Y = yreg;
  U = ureg;
  S = sreg;
  PC = pcreg;
  DP = dpreg<<8;
  CC = cc;
  C = CC; V = C>>1; Z = V>>1; N = Z>>1; I = N>>1; H = I>>1; F = H>>1; E = (F>>1)&1; F &= 1; H &= 1; I &= 1; N &= 1; Z &= 1; V &= 1; C &= 1;
  restore_flags();

//  log_it("IN: $%04x", PC);
}
void regFromSTB1()
{
  areg = A;
  breg = B;
  xreg = X;
  yreg = Y;
  ureg = U;
  sreg = S;
  pcreg = PC;
  dpreg = (DP>>8)&0xff;
  simplify_flags();
  CC = (((((((((((((E<<1) | F)<<1) | H)<<1) |I)<<1) | N)<<1) | Z)<<1) | V)<<1) | C; 
  cc = CC;
//  log_it("OUT: $%04x", PC);
}
void startSBT1()
{
  clockticks = 0;
  regToSTB1();
}
void endSBT1()
{
  regFromSTB1();
  m6809_1_ICount -= clockticks;
  iCount += clockticks;
}

// These must be procedures because the parameters may include ++
static void wr_mem(int addr, unsigned char value) 
{
 M_WRMEM (addr&0xffff,value); 
}

static unsigned int rd_mem(int addr) 
{
  return M_RDMEM(addr&0xffff);
}

int bank = 0;
extern unsigned char currentButtonState; // bit values as in vectrex

void mon(char *s) 
{
//  if (DEBUG) log_it("P=%02x A=%02x B=%02x X=%04x Y=%04x S=%04x U=%04x CC=%02x DP=%02x E%cF%cH%cI%cC%cZ%cV%cN%c   %s",
//                    bank, A, B, X, Y, S, U, CC, (UINT8)(DP>>8), E+'0', F+'0', (((H & 0x10) != 0)?1:0)+'0', I+'0',
//                    (((C & 0x100) != 0)?1:0)+'0', ((Z != 0)?1:0)+'0', (((SINT8)V < 0)?1:0)+'0', (((SINT8)N < 0)?1:0)+'0', s);
  if (currentButtonState == 1)
    if (DEBUG) log_it("iCount=%i, P=%02x A=%02x B=%02x X=%04x Y=%04x S=%04x U=%04x CC=%02x DP=%02x E%cF%cH%cI%cC%cZ%cV%cN%c   %s",iCount,
                    bank, A, B, X, Y, S, U, CC, (UINT8)(DP>>8), E+'0', F+'0', (((H & 0x10) != 0)?1:0)+'0', I+'0',
                    (((C & 0x100) != 0)?1:0)+'0', ((Z != 0)?1:0)+'0', (((SINT8)V < 0)?1:0)+'0', (((SINT8)N < 0)?1:0)+'0', s);
}

int getBank();
extern int noCopyright;
extern int noCoinInfo;
extern int countOfStars; // 0 == all 0x32 (max)
extern int countOfStarsIngame; // 0 == all 0x32 (max)
extern int gameRunning; // only if countOfStarsIngame!=0

extern int noShieldGrafik;
extern int noCockpit;
extern int enablePSGSound;
extern int displayReducedHighscore; // max 10 if 0, than all are display (10)
extern int starWarsZoom; // used internally, is one, when we zoom in on death star

extern int reduceDeathStarExplosion;
#define EXPLOSION_VAL (10)

extern int currentLineCount;
extern int displayedLineCount;
//extern int lineStartWith;
//extern int lineDisplayed;
extern int inAttract4;
extern int inAttract3;
extern int reduceLinesDisplayTo; // 0 = all
extern int savetyNet;
extern int savetyNetCount;


void doSBT(int maxClockTicks)
{
  startSBT1();
bankswitch_check:

  bank = getBank();
  if (bank == 0)
  {
    while (1)
    {
      if (clockticks>=maxClockTicks) goto sbtDone;
      switch (PC) 
      {
        #include "starwars-bank0.c"     // 6000-7FFF
        #include "starwars-unbanked.c"
        default:
        {
          printf("Unknown jump to %04x in bank0\n", PC); 
          goto sbtDone;
        }
      }
    }
  }
  else
  {
    while (1)
    {
      if (clockticks>=maxClockTicks) goto sbtDone;
      switch (PC) 
      {
	#include "starwars-bank1.c"     // 6000-7FFF
        #include "starwars-unbanked1.c"
        default:
        {
          printf("Unknown jump to %04x in bank0\n", PC); 
          goto sbtDone;
        }
      }
    }
  }
  
sbtDone:  
  endSBT1();
}



