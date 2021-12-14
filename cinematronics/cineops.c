#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//#define DEBUGGING_STARHAWK 1
#include <vectrex/osWrapper.h>
#include <vectrex/vectrexInterface.h>

#include "cines.h"
//#include "options.h"

// these should be in the vectrex library
#define VEC_BUTTON_1_1 0x01
#define VEC_BUTTON_1_2 0x02
#define VEC_BUTTON_1_3 0x04
#define VEC_BUTTON_1_4 0x08
#define VEC_BUTTON_2_1 0x10
#define VEC_BUTTON_2_2 0x20
#define VEC_BUTTON_2_3 0x40
#define VEC_BUTTON_2_4 0x80

// Put any switch or IO bits that are common to all machines here.
#define SW_ABORT  0x100		/* for ioSwitches */

int useParity = 0;

static int world_xl = -1800, world_yb = -2400, world_xr = 1800, world_yt = 2400; // dummy values should never be used

unsigned long isqrt(unsigned long x)
{
  register unsigned long op, res, one;

  op = x;
  res = 0;

  /* "one" starts at the highest power of four <= than the argument. */
  one = 1 << 30;  /* second-to-top bit set */
  while (one > op) one >>= 2;

  while (one != 0) {
    if (op >= res + one) {
      op -= res + one;
      res += one << 1;  // <-- faster than 2 * one
    }
    res >>= 1;
    one >>= 2;
  }
  return res;
}

int /*fp14*/ sine[] =
  {
    0,    402,    803,   1205,   1605,   2005,   2404,   2801,
    3196,   3589,   3980,   4369,   4756,   5139,   5519,   5896,
    6269,   6639,   7005,   7366,   7723,   8075,   8423,   8765,
    9102,   9434,   9759,  10079,  10393,  10701,  11002,  11297,
    11585,  11866,  12139,  12406,  12665,  12916,  13159,  13395,
    13622,  13842,  14053,  14255,  14449,  14634,  14810,  14978,
    15136,  15286,  15426,  15557,  15678,  15790,  15892,  15985,
    16069,  16142,  16206,  16260,  16305,  16339,  16364,  16379,
    16384,  16379,  16364,  16339,  16305,  16260,  16206,  16142,
    16069,  15985,  15892,  15790,  15678,  15557,  15426,  15286,
    15136,  14978,  14810,  14634,  14449,  14255,  14053,  13842,
    13622,  13395,  13159,  12916,  12665,  12406,  12139,  11866,
    11585,  11297,  11002,  10701,  10393,  10079,   9759,   9434,
    9102,   8765,   8423,   8075,   7723,   7366,   7005,   6639,
    6269,   5896,   5519,   5139,   4756,   4369,   3980,   3589,
    3196,   2801,   2404,   2005,   1605,   1205,    803,    402,
    0,   -402,   -803,  -1205,  -1605,  -2005,  -2404,  -2801,
    -3196,  -3589,  -3980,  -4369,  -4756,  -5139,  -5519,  -5896,
    -6269,  -6639,  -7005,  -7366,  -7723,  -8075,  -8423,  -8765,
    -9102,  -9434,  -9759, -10079, -10393, -10701, -11002, -11297,
    -11585, -11866, -12139, -12406, -12665, -12916, -13159, -13395,
    -13622, -13842, -14053, -14255, -14449, -14634, -14810, -14978,
    -15136, -15286, -15426, -15557, -15678, -15790, -15892, -15985,
    -16069, -16142, -16206, -16260, -16305, -16339, -16364, -16379,
    -16384, -16379, -16364, -16339, -16305, -16260, -16206, -16142,
    -16069, -15985, -15892, -15790, -15678, -15557, -15426, -15286,
    -15136, -14978, -14810, -14634, -14449, -14255, -14053, -13842,
    -13622, -13395, -13159, -12916, -12665, -12406, -12139, -11866,
    -11585, -11297, -11002, -10701, -10393, -10079,  -9759,  -9434,
    -9102,  -8765,  -8423,  -8075,  -7723,  -7366,  -7005,  -6639,
    -6269,  -5896,  -5519,  -5139,  -4756,  -4369,  -3980,  -3589,
    -3196,  -2801,  -2404,  -2005,  -1605,  -1205,   -803,   -402,
  };

FILE *debugf = NULL;

#define DEBUG_OUT(...)

#ifndef FALSE
#define FALSE (0!=0)
#define TRUE (0==0)
#endif

// Start of CCPU emulator

int ccpudebug = 0;		/* default is off (0) - can be turned on with -d flag */

// war of the worlds had a missing instruction.  Now added and it runs.

/*
boxingbugs had a missing instruction:

2EAE: C8    : ldi  $8     : A=047 B=045 I=047Z J=E9B P=1 A0=46 00 N1 O46 A
2EAF: EA    : lda  [i]    : A=18D B=045 I=047Z J=E9B P=1 A0=47 00 N18D O47 AA
2EB0: D7    : sta  $7     : A=18D B=045 I=017Z J=E9B P=1 A0=47 00 N18D O47 A
2EB1: 02    : lda  #$200  : A=200 B=045 I=017Z J=E9B P=1 A0=8D 01 N200 O18D AA
2EB2: 76    : sub  $6     : A=13C B=045 I=016Z J=E9B P=1 A0=00 11 NC4 O200 AA
2EB3: 57    : nopb        : A=13C B=045 I=016Z J=E9B P=1 A0=00 11 NC4 O200   B
2EB4: 03    : lda  #$300  : A=13C B=300 I=016Z J=E9B P=1 A0=3C 01 N300 O45 AA
2EB5: 57    : nopb        : A=13C B=300 I=016Z J=E9B P=1 A0=3C 01 N300 O45   B
2eb6 DANGER: Unimplemented instruction - opLSRe B 1
 */

/*

Had to add missing opcode to get cosmicchasm to run - now it runs but is
stuck in a loop:

0840: 30 39 : sub  #$39   : A=040 B=039 I=08CZ J=840 P=8 A0=40 10 N39 O72 AA
0842: 00    : clr         : A=000 B=039 I=08CZ J=840 P=8 A0=40 00 N0 O40 AA
0843: FF    : lsld        : A=000 B=072 I=08CZ J=840 P=8 A0=00 0F NFFF O0 AA
0844: EA    : lda  [i]    : A=000 B=072 I=08CZ J=840 P=8 A0=00 00 N0 O0 AA
0845: 90    : out  $0     : A=000 B=072 I=08CZ J=840 P=8 A0=00 00 N0 O0 A
0846: E3    : mul  [i]    : A=000 B=039 I=08CZ J=840 P=8 A0=00 00 N0 O0 AA
0847: 40 48 : ldj  #$840  : A=000 B=039 I=08CZ J=840 P=8 A0=00 00 N0 O0 A
0849: C0    : ldi  $0     : A=000 B=039 I=000Z J=840 P=8 A0=00 00 N0 O0 A
084A: D0    : sta  $0     : A=000 B=039 I=080Z J=840 P=8 A0=00 00 N0 O0 A
084B: BC    : cmp  $C     : A=000 B=039 I=08CZ J=840 P=8 A0=00 10 N0 O0 AA
084C: 00    : clr         : A=000 B=039 I=08CZ J=840 P=8 A0=00 00 N0 O0 AA
084D: FF    : lsld        : A=000 B=072 I=08CZ J=840 P=8 A0=00 0F NFFF O0 AA
084E: EB    : lsr         : A=000 B=072 I=08CZ J=840 P=8 A0=00 0B NBEB O0 AA
084F: 88    : ldp  #$8    : A=000 B=072 I=08CZ J=840 P=8 A0=00 0B NBEB O0 A
0850: 20 40 : add  #$40   : A=040 B=072 I=08CZ J=840 P=8 A0=00 00 N40 O0 AA
0852: 52    : jdrb <jump> : A=040 B=072 I=08CZ J=840 P=8 A0=00 00 N40 O0   B
0852: 52    : jdrb <jump> : A=040 B=072 I=08CZ J=840 P=8 A0=00 00 N40 O0   BB
0853: 50    : jppb <jump> : A=040 B=072 I=08CZ J=840 P=8 A0=00 00 N40 O0   B

*/
#define CARRYBIT (1<<12)
#define A0BIT 1

/* Contorted sign-extend macro only evaluates its parameter once, and
   executes in two shifts.  This is far more efficient that any other
   sign extend procedure I can think of, and relatively safe */

#define SEX(twelvebit) ((((int)twelvebit) << (int)((sizeof(int)*8)-12)) \
                          >> (int)((sizeof(int)*8)-12))

/* Mouse values currently sampled once per frame and set in WAI code.
   This is not ideal. */

/* RCstate setting macros to exit opcode handler */
#define jumpCineRet_A     do {RCstate = RCstate_A; goto cineExecBottom;} while (0)
#define jumpCineRet_B     do {RCstate = RCstate_B; goto cineExecBottom;} while (0)
#define jumpCineRet_AA    do {RCstate = RCstate_AA; goto cineExecBottom;} while (0)
#define jumpCineRet_BB    do {RCstate = RCstate_BB; goto cineExecBottom;} while (0)
#define jumpCineRet_WEIRD do (DEBUG_OUT( "Didn't bother yet.\n" ); exit ( 0 );} while (0)

/* Handy new operators ... */

/* for Zonn translation :) */
#define SAR16(var,arg)    ( ( (signed short int) var ) >> arg )

/* for setting/checking the A0 flag */
#define SETA0(var)    ( RCacc_a0 = var )
#define GETA0()       ( RCacc_a0 )

/* for setting/checking the Carry flag */
#define SETFC(val)    ( RCflag_C = val )
#define GETFC()       ( ( RCflag_C >> 8 ) & 0xFF )

/* Define new types for the c-cpu emulator */
typedef short unsigned int RCCINEWORD;	/* 12bits on the C-CPU */
typedef unsigned char RCCINEBYTE;	/* 8 (or less) bits on the C-CPU */
typedef short signed int RCCINESWORD;	/* 12bits on the C-CPU */
typedef signed char RCCINESBYTE;	/* 8 (or less) bits on the C-CPU */

typedef enum
{
   RCstate_A = 0, RCstate_AA, RCstate_B, RCstate_BB
} RCCINESTATE;			/* current RCstate */

volatile static UINT32 dwElapsedTicks = 0;
static UINT8 bBailOut = FALSE;

/* C-CPU context information begins -- */
RCCINEWORD RCregister_PC = 0;	/* C-CPU registers; program counter */
RCCINEWORD RCregister_A = 0;	/* ; A-Register (accumulator) */
RCCINEWORD RCregister_B = 0;	/* ; B-Register (accumulator) */
RCCINEBYTE RCregister_I = 0;	/* ; I-Register (last access RAM location) */
RCCINEWORD RCregister_J = 0;	/* ; J-Register (target address for JMP
				   opcodes) */
RCCINEBYTE RCregister_P = 0;	/* ; Page-Register (4 bits) */
RCCINEWORD RCFromX = 0;		/* ; X-Register (start of a vector) */
RCCINEWORD RCFromY = 0;		/* ; Y-Register (start of a vector) */
RCCINEWORD RCregister_T = 0;	/* ; T-Register (vector draw length timer) */
RCCINEWORD RCflag_C = 0;	/* C-CPU flags; carry. Is word sized, instead
				   of RCCINEBYTE, so we can do direct
				   assignment and then change to BYTE during
				   inspection. */

RCCINEWORD RCcmp_old = 0;	/* last accumulator value */
RCCINEWORD RCcmp_new = 0;	/* new accumulator value */
RCCINEBYTE RCacc_a0 = 0;	/* bit0 of A-reg at last accumulator access */

RCCINESTATE RCstate = RCstate_A;	/* C-CPU RCstate machine current
					   RCstate */
RCCINEWORD RCram[256];		/* C-CPU RCram (for all pages) */

UINT8 rom[0x8000];	// = tailgunner was 0x2000 - others may be larger

static char ccpu_game_name[32];	/* canonical name we now use as cinemu
				   parameter */
static int ccpu_game_id = 0;    /* as set by cineSetGame */
static UINT8 ccpu_jmi_dip = 1;	/* as set by cineSetJMI */
static UINT8 ccpu_msize = 0;	/* as set by cineSetMSize */
static UINT8 ccpu_monitor = 0;	/* as set by cineSetMonitor */

/* cinem graphics junk */
RCCINEBYTE RCvgShiftLength = 0;	/* number of shifts loaded into length reg */

/* -- Context information ends. */

UINT8 bFlipX;
UINT8 bFlipY;
UINT8 bSwapXY;
UINT8 bOverlay;

UINT16 ioSwitches = 0xFF;	/* high values for non-triggered switches */
UINT16 initialSwitches = 0xFF;	/* high values for non-triggered switches */
UINT16 ioInputs = 0;
UINT16 initialInputs = 0;
UINT8 ioOutputs = 0;

UINT16 RCvgColour = 0;

INT16 JoyX;
INT16 JoyY;
UINT8 bNewFrame;

INT32 sdwGameXSize;
INT32 sdwGameYSize;
INT32 sdwXOffset = 0;
INT32 sdwYOffset = 0;

// This will move to the vectrex graphics library.
static int64_t ScaleXMul = 1LL, ScaleXDiv = 1LL, ScaleXOffsetPre = 0LL, ScaleXOffsetPost = 0LL,
               ScaleYMul = 1LL, ScaleYDiv = 1LL, ScaleYOffsetPre = 0LL, ScaleYOffsetPost = 0LL;
static int v_rotate = 0, v_flip_x = 0, v_flip_y = 0;
static int v_xl = 0, v_yb = 0, v_xr = 0, v_yt = 0;

int tx (int x) {				// convert x from window to viewport
   if (v_flip_x) x = v_xr - (x - v_xl);
   return (int) (((((int64_t) x) + ScaleXOffsetPre) * ScaleXMul) / ScaleXDiv + ScaleXOffsetPost);
}

int ty (int y) {				// and y
   if (v_flip_y) y = v_yt - (y - v_yb);
   return (int) (((((int64_t) y) + ScaleYOffsetPre) * ScaleYMul) / ScaleYDiv + ScaleYOffsetPost);
}

int frameCounter = 0;
static int cineTwinkle = 255;	// off by default
void line (int xl, int yb, int xr, int yt, int col)
{
//  printf("x,y %i,%i -> x,y %i,%i\n\r",xl, yb,  xr, yt);
   if (v_rotate) {
      int tmp;
      tmp = xl; xl = yb; yb = tmp;
      tmp = xr; xr = yt; yt = tmp;
      xl = v_xr - (xl - v_xl);
      xr = v_xr - (xr - v_xl);
   }

   if ((ccpu_game_id == GAME_TAILGUNNER) && (xl==xr) && (yb==yt)) {
     // stars...  tweak the intensity according to the distance from the center
     int SCREEN_W = world_xr-world_xl, SCREEN_H = world_yt-world_yb;
     int dist = isqrt((SCREEN_W/2 - xl)*(SCREEN_W/2 - xl) + (SCREEN_H/2 - yb)*(SCREEN_H/2 - yb));
     int maxdist = isqrt((SCREEN_W/2)*(SCREEN_W/2) + (SCREEN_H/2)*(SCREEN_H/2));
     if (maxdist == 0) maxdist = 1;
     if (col == 7) {
       col = (col - (((maxdist-dist) * 7) / maxdist) + 1)*2+1;
     }
     v_directDraw32Patterned (tx (xl), ty (yb), tx (xr), ty (yt), (col + 1) * 8 - 1, random());
   } else if ((ccpu_game_id == GAME_TAILGUNNER) && (RCregister_PC == 0x03B2)) {
     // 0x1BBD - PUSH START
     // 0x03B2 - 3 lines in the shields have Twinkle values
     // Tailgunner's shields are funny and need fixing.
     v_directDraw32Patterned (tx (xl), ty (yb), tx (xr), ty (yt), 127, random ()); //one way to make sparky shields...
     //DEBUG_OUT("line(%d,%d, %d,%d, %02x) at PC=0x%04x;\n", xl,yb, xr,yt, col, RCregister_PC);
   } else {
     /* Twinkle may be 0 (sundance), or 7-9 */
     if ((col <= cineTwinkle) || (xl!=xr) || (yb!=yt))
     {
    
       
       
       DEBUG_OUT("v_directDraw32 (%d,%d, %d,%d, %d);\n", tx (xl), ty (yb), tx (xr), ty (yt), (col + 1) * 8 - 1);
       v_directDraw32 (tx (xl), ty (yb), tx (xr), ty (yt), (col + 1) * 8 - 1);
     } else { // twinkle parameter denotes flash.
       v_directDraw32 (tx (xl), ty (yb), tx (xr), ty (yt), (127 * (sine[(frameCounter<<3)&255]+16384+32768)) / 65536); // brightness 63..127
     }
   }
}


int _tx (int x) {                // convert x from window to viewport
   if (v_flip_x) x = v_xr - (x - v_xl);
   return (int) (((((int64_t) x) + ScaleXOffsetPre) * ScaleXMul) / ScaleXDiv + ScaleXOffsetPost);
}

int _ty (int y) {                // and y
   if (v_flip_y) y = v_yt - (y - v_yb);
   return (int) (((((int64_t) y) + ScaleYOffsetPre) * ScaleYMul) / ScaleYDiv + ScaleYOffsetPost);
}


void window (int xl, int yb, int xr, int yt)
{
   // We will use normalised viewport coordinates of x = -18000:18000 and y = -24000:24000 for consistency
   int64_t width, height;
//   int xc, yc;
   int  oxl = xl, oyb = yb, oxr = xr, oyt = yt;

   v_xl = xl; v_yb = yb; v_xr = xr; v_yt = yt;

   width = ABS((int64_t) xr - (int64_t) xl);
   height = ABS((int64_t) yt - (int64_t) yb);
   // However, if OS tells us that vectrex is on its side, we'll handle these differently.
   // For now, though, Malban's orientation code is doing the rotation behind the scenes
   // so we don't want to do it twice.  Need to think about whether that solution is OK
   // or has to be changed. Although it doesn't matter too much in terms of what is displayed,
   // it makes a different with respect to loading and saving configs and using the same
   // default for multiple games.
/*
   if (width * 4 > height * 3) {
      // window is wider than aspect ratio, so we will have black bars at the top and bottom
      height = (width * 4) / 3;
      yc = (yb + yt) / 2;
      yb = yc - height / 2;
      yt = yc + height / 2;
   } else if (width * 4 < height * 3) {
      // window is taller than aspect ratio, so we will have black bars at
      // the sides
      width = (height * 3) / 4;
      xc = (xl + xr) / 2;
      xl = xc - width / 2;
      xr = xc + width / 2;
   }
*/
   ScaleXMul = 36000LL;
   ScaleXDiv = width;
   ScaleXOffsetPre = -width / 2LL;
   ScaleXOffsetPost = 0LL;
//   ScaleXOffsetPost = (tx (xr) - tx (oxr)) / 2LL; // possible  black bars 

   ScaleYMul = 36000LL;
   ScaleYDiv = height;
   ScaleYOffsetPre = -height / 2LL;
   ScaleYOffsetPost = 0LL;
//   ScaleYOffsetPost = (ty (yt) - ty (oyt)) / 2LL; // possible  black bars 
   v_setCustomClipping(TRUE, tx(oxl), ty(oyb), tx(oxr), ty(oyt));

   // setCustomClipping(TRUE, tx(oxl), ty(oyb), tx(oxr), ty(oyt));
       // transform world (window) coordinates to viewport (normalised device
       // coordinates) before clipping.  That way clipping code does not need to know about world
       // coordinates.  Not implemented as I don't have the updated library yet
}



void _window (int xl, int yb, int xr, int yt)
{
   // We will use normalised viewport coordinates of x = -18000:18000 and y = -24000:24000 for consistency
   int64_t width, height;
   int xc, yc;
   int /* oxl = xl, oyb = yb, */  oxr = xr, oyt = yt;

   v_xl = xl; v_yb = yb; v_xr = xr; v_yt = yt;

   width = (int64_t) xr - (int64_t) xl;
   height = (int64_t) yt - (int64_t) yb;
   // However, if OS tells us that vectrex is on its side, we'll handle these differently.
   // For now, though, Malban's orientation code is doing the rotation behind the scenes
   // so we don't want to do it twice.  Need to think about whether that solution is OK
   // or has to be changed. Although it doesn't matter too much in terms of what is displayed,
   // it makes a different with respect to loading and saving configs and using the same
   // default for multiple games.
   if (width * 4 > height * 3) {
      // window is wider than aspect ratio, so we will have black bars at the top and bottom
      height = (width * 4) / 3;
      yc = (yb + yt) / 2;
      yb = yc - height / 2;
      yt = yc + height / 2;
   } else if (width * 4 < height * 3) {
      // window is taller than aspect ratio, so we will have black bars at
      // the sides
      width = (height * 3) / 4;
      xc = (xl + xr) / 2;
      xl = xc - width / 2;
      xr = xc + width / 2;
   }

   ScaleXMul = 36000LL;
   ScaleXDiv = width;
   ScaleXOffsetPre = -width / 2LL;
   ScaleXOffsetPost = 0LL;
   ScaleXOffsetPost = (tx (xr) - tx (oxr)) / 2LL;

   ScaleYMul = 48000LL;
   ScaleYDiv = height;
   ScaleYOffsetPre = -height / 2LL;
   ScaleYOffsetPost = 0LL;
   ScaleYOffsetPost = (ty (yt) - ty (oyt)) / 2LL;

   // setCustomClipping(TRUE, tx(oxl), ty(oyb), tx(oxr), ty(oyt));
       // transform world (window) coordinates to viewport (normalised device
       // coordinates) before clipping.  That way clipping code does not need to know about world
       // coordinates.  Not implemented as I don't have the updated library yet
}
// end of window library

// start of startFrame section
#include "startframe.i"
// end of startFrame section

void startFrame(void)
{
   switch (Game)
   {
     case GAME_RIPOFF:
       startFrame_ripoff();
       return;
     case GAME_SPACEWARS:
       startFrame_spacewars();
       return;
     case GAME_BOXINGBUGS:
       startFrame_boxingbugs();
       return;
     case GAME_ARMORATTACK:
       startFrame_armorattack();
       return;
     case GAME_STARCASTLE:
       startFrame_starcastle();
       return;
     case GAME_STARHAWK:
       startFrame_starhawk();
       return;
     case GAME_SOLARQUEST:
       startFrame_solarquest();
       return;
     case GAME_COSMICCHASM:
       startFrame_cosmicchasm();
       return;
     case GAME_WAROFTHEWORLDS:
       startFrame_waroftheworlds();
       return;
     case GAME_WARRIOR:
       startFrame_warrior();
       return;
     case GAME_BARRIER:
       startFrame_barrier();
       return;
     case GAME_SUNDANCE:
       startFrame_sundance();
       return;
     case GAME_QB3:
       startFrame_qb3();
       return;
     case GAME_TAILGUNNER:
       startFrame_tailgunner(); // Tailgunner calls the instruction that we use to determine vsync twice in a row without intervening drawing code.
       return;
     case GAME_DEMON:
       startFrame_demon(); // Demon calls the instruction that we use to determine vsync twice in a row without intervening drawing code.
       return;
     case GAME_SPEEDFREAK:
       startFrame_speedfreak(); // Speedfreak calls the instruction that we use to determine vsync twice in a row without intervening drawing code.
       return;
     default:
       fprintf(stderr, "Game %d not defined!\n", Game);
    }
}

void CinemaVectorData (int RCFromX, int RCFromY, int RCToX, int RCToY, int vgColour)
{
   RCFromX = SEX (RCFromX); RCFromY = SEX (RCFromY); RCToX = SEX (RCToX); RCToY = SEX (RCToY);
   line (RCFromX, RCFromY, RCToX, RCToY, vgColour);
}

void xUNFINISHED (char *s, int RCregister_PC)
{
   DEBUG_OUT("%04x DANGER: Unimplemented instruction - %s\n", RCregister_PC, s);
   exit (1);
}
#define UNFINISHED(s) xUNFINISHED(s, RCregister_PC)

/* Reset C-CPU registers, flags, etc to default starting values */
void cineReset (void)
{

   /* zero registers */
   RCregister_PC = 0;
   RCregister_A = 0;
   RCregister_B = 0;
   RCregister_I = 0;
   RCregister_J = 0;
   RCregister_P = 0;
   RCFromX = 0;
   RCFromY = 0;
   RCregister_T = 0;

   /* zero flags */
   RCflag_C = 0;

   /* reset RCstate */
   RCstate = 0;

   /* reset RCram */
   memset (RCram, 0, sizeof (RCram));

   /* reset internal RCstate */
   RCcmp_old = 0;
   RCcmp_new = 0;
   SETA0 (0);

}

void cineSetGame (char *name, int id)
{
   strcpy (ccpu_game_name, name);
   ccpu_game_id = id;
}

void cineSetJMI (UINT8 j)
{
   ccpu_jmi_dip = j;
   DEBUG_OUT("cineSetJMI(%u);\n", j);
}

void cineSetMSize (UINT8 m)
{
   ccpu_msize = m;
   DEBUG_OUT("cineSetMSize(%u);\n", m);
}

void cineSetMonitor (UINT8 m)
{
   ccpu_monitor = m;
   DEBUG_OUT("cineSetMonitor(%u);\n", m);
}

UINT32 cineGetElapsedTicks (UINT32 dwClearIt)
{
   UINT32 dwTemp;
   dwTemp = dwElapsedTicks;
   if (dwClearIt) dwElapsedTicks = 0;
   return (dwTemp);
}

void cineReleaseTimeslice (void)
{
   bBailOut = TRUE;
}

/* extra functions */

#include "cinedbg.i"

/* cineExec() is what performs all the "processors" work; it will
 * continue to execute until something horrible happens, a watchpoint
 * is hit, cycle count exceeded, or other happy things. */
UINT32 cineExec(void)
{
   RCCINEBYTE temp_byte = 0; /* cause it'll be handy */
   //RCCINEBYTE temp_byte_2 = 0;       /* cause it'll be handy */
   RCCINEWORD temp_word = 0; /* cause it'll be handy */
   RCCINEWORD temp_word_2 = 0;       /* cause it'll be handy */
   RCCINEWORD original_PC;

   /* before opcode handlers ... */
   ioSwitches &= (~SW_ABORT);	/* ( ! SW_ABORT ) ? */
   bNewFrame = 0;

   /* label; after each opcode is finished running, execution returns to this
      point, where the RCstate is examined and the proper jump table used. */
 cineExecTop:
   original_PC = RCregister_PC;
   bBailOut = FALSE;

   /* examine the current RCstate, and jump down to the correct opcode jump table. */
   switch (RCstate) {
   case RCstate_A:
      goto opCodeTblA;
   case RCstate_AA:
      goto opCodeTblAA;
   case RCstate_B:
      goto opCodeTblB;
   case RCstate_BB:
      goto opCodeTblBB;
   } /* switch on current RCstate */

   /* include the opcode jump tables; goto the correct piece of code for the
      current opcode. That piece of code will set the RCstate for the next
      run, as well. The opcode jumptables were generated with a perl script
      and are kept separate for make and ugly sake. */

#include "cineops.i"

   /* opcode exit point (so we can catch and do debugging, etc. */
 cineExecBottom:

   /* handle debugger */
   if (ccpudebug) {
     static char disassembly[256];     /* debugger buffer */

     disassemble (disassembly, sizeof (disassembly), original_PC);
     fprintf(stdout, "%s\n", disassembly);
   }

   /* the opcode code has set a RCstate and done mischief with flags and the
      programcounter; now jump back to the top and run through another
      opcode. */

//cineExit:
   goto cineExecTop; // consecutive instructions in a tight loop...

 cineLoopExit:
   /* return control to main system at end of a frame (WAI instruction) */
   dwElapsedTicks = 100000;     /* some magic number ... not really sure about this or the return value...*/

   return (0x80000000);
}

void vgSetRotateFlip (int rotate, int flipX, int flipY)
{
  /*
      rotate and flip are what is necessary so that all games are oriented the same way up.
      This corrects differences in the arcade hardware.
      HOWEVER they do not take into account the rotation of the screen, which has to be applied after these.
   */
   DEBUG_OUT("vgSetRotateFlip( int rotate=%d, int flipX=%d, int flipY=%d);\n", rotate, flipX, flipY);
   v_rotate = rotate;
   v_flip_x = flipX;
   v_flip_y = flipY;
}

// for now INI setting just stupidly overwrite other saved settings!
static int cineIniHandler(void* user, const char* section, const char* name, const char* value)
{
  // cascading ini files
  // first check if there are "general" entries
  if (iniHandler(user, section, name, value) == 1) return 1;

  
  #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
  #define MATCH_NAME(n) strcmp(name, n) == 0
/*
 * No special values as of yet
  if (MATCH("VECTREX_EXACT", "ROM_FILE")) strcpy(romName, value); else 
  {
      printf("Ini-Handler, unkown entry: %s = %s", name, value);
      return 0;  / * unknown section/name, error * /
  }
*/  
  return 1;
}


void cineInit (void)
{
//#define SETTINGS_SIZE 1024
//   static unsigned char settingsBlob[SETTINGS_SIZE];

   DEBUG_OUT("cineInit(void);\n");
//   v_loadSettings (ccpu_game_name, settingsBlob, SETTINGS_SIZE);
//   DEBUG_OUT("v_loadSettings(\"%s\");\n", ccpu_game_name /*, settingsBlob*/);
   
   
    int pv = 0;
    if ((pv = ini_parse("cine.ini", cineIniHandler, 0)) < 0) 
    {
          printf("cine.ini not loaded!\n\r");
    }
    else
      printf("cine.ini loaded!\n\r");
    int i;
    char iniBuf[80];
    for (i=0; ccpu_game_name[i]!=0;i++)
      iniBuf[i] = ccpu_game_name[i];
    iniBuf[i++] = '.';
    iniBuf[i++] = 'i';
    iniBuf[i++] = 'n';
    iniBuf[i++] = 'i';
    iniBuf[i++] = 0;
    pv = 0;
    if ((pv = ini_parse(iniBuf, cineIniHandler, 0)) < 0) 
    {
          printf("%s not loaded!\n\r", iniBuf);
    }
    else
          printf("%s loaded!\n\r", iniBuf);   
}

void vgInit (void)
{
}

void vgSetTwinkle (int aTwinkle)
{
   DEBUG_OUT("vgSetTwinkle( int aTwinkle=%d);\n", aTwinkle);
   cineTwinkle = aTwinkle;
}

void vgSetMonitor (int aMonitor, int RB, int GB, int BB, int RC, int GC,
		   int BC)
{
   DEBUG_OUT("vgSetMonitor( int aMonitor, int RB, int GB, int BB, int RC, int GC, int BC);\n");
}

void cineSetRate (unsigned int aRefresh)
{
   DEBUG_OUT("cineSetRate( unsigned int aRefresh=%d );\n", aRefresh);
   if (aRefresh < 50) {
     v_setRefresh (aRefresh*2);
     bufferType = 1;
   } else {
     v_setRefresh (aRefresh);
     bufferType = 0;
   }
   
}

void cineSetSw (unsigned int aSwitches, unsigned int aInputs)
{
   DEBUG_OUT("cineSetSw( unsigned int aSwitches=%0x, unsigned int aInputs=%0x); // TO DO!\n", aSwitches, aInputs);
   initialSwitches = aSwitches;
   initialInputs = aInputs;
}

void cineSetMouse (unsigned int aMouseType,
		   unsigned int aMouseSpeedX,
		   unsigned int aMouseSpeedY,
		   unsigned int aKeyMask, unsigned char *aMouseSeg)
{
   DEBUG_OUT("cineSetMouse( aMouseType=%d, ...);\n", aMouseType);
}

void vgSetMode (int mode)
{
   DEBUG_OUT("vgSetMode( int mode=%d);\n", mode);
}

void vgSetCineSize (int Xmin, int Ymin, int Xmax, int Ymax)
{
  DEBUG_OUT("vgSetCineSize( int Xmin=%d, int Ymin=%d, int Xmax=%d, int Ymax=%d, orientation=%d);\n", Xmin, Ymin, Xmax, Ymax, settings->orientation);
  if (settings->orientation == 0) {
    world_xl = Xmin; world_yb = Ymin; world_xr = Xmax; world_yt = Ymax;
    window (Xmin, Ymin, Xmax, Ymax);
  } else { // assume == 1 although 2, 3 is also allowed by Malban's menu code.
    world_xl = Xmin; world_yb = Ymin; world_xr = Xmax; world_yt = Ymax;
    window (Xmin, Ymin, Xmax, Ymax);
    //window (Ymin, Xmin, Ymax, Xmax);
  }
}

void vsetmode (unsigned int mode)
{
   DEBUG_OUT("vsetmode( unsigned int mode=%d );\n", mode);
}
