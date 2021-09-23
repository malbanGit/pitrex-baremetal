#pragma once

// if defined, all printf outputs 
// are also written in a log printf.log
// (as long as the file system is activated)
#ifndef NO_DEBUG
#define DEBUG_TO_FILE
#endif

#include <stddef.h>
#include <stdint.h>
#include <pitrex/pitrexio-gpio.h>
#include "baremetalUtil.h"
#include <vectrex/osWrapper.h>
#include <uspi.h>


#ifdef DEBUG_TO_FILE
extern FILE *__debugf;
#define printf(...) do \
{ \
  int __ss = errno;\
  char __buff[256];\
  sprintf(__buff,__VA_ARGS__);\
  fprintf(stdout,__buff); \
  if (logOutput)\
  { \
    if (!__debugf) {__debugf = fopen("../../../printf.log", "a"); \
      {if (!__debugf) __debugf = fopen("../../printf.log", "a"); \
         {if (!__debugf) __debugf = fopen("../printf.log", "a"); \
            if (!__debugf) __debugf = fopen("printf.log", "a"); }}}\
    if (__debugf) \
    { \
      fprintf(__debugf,__buff); \
      fflush(__debugf); \
    } \
  }\
  errno=__ss;} while(0)
#endif


// used in VECX
#define DELAY_AFTER_T1_END_VALUE_DIRECT 8
#define DELAY_Y_SET_DIRECT 2


void v_init(); // also mounts sd card
void v_init_(int i); // also mounts sd card

void returnToPiTrex();


void v_setBrightness(uint8_t brightness);
void v_directDeltaMove32start(int32_t xLen, int32_t yLen);
void v_directDeltaMoveEnd();
void v_directMove32(int32_t xEnd, int32_t yEnd);
void v_directDraw32(int32_t xStart, int32_t yStart, int32_t xEnd, int32_t yEnd, uint8_t brightness);
void v_directDraw32Patterned(int32_t xStart, int32_t yStart, int32_t xEnd, int32_t yEnd, uint8_t brightness, uint8_t pattern); // only pipelined!
void v_directDraw32Hinted(int32_t xStart, int32_t yStart, int32_t xEnd, int32_t yEnd, uint8_t brightness, int forced);
void v_directDraw32HintedDebug(int32_t xStart, int32_t yStart, int32_t xEnd, int32_t yEnd, uint8_t brightness, int forced, char* debugInfo);
void v_directDeltaDraw32(int32_t xLen, int32_t yLen, uint8_t brightness);
void v_displayAbsolutList_8(const signed char list[], int brightness);
void v_displayAbsolutList_8o(int x, int y, const signed char list[], int brightness);
void v_addZeroRef(); // current pipeline item
void v_setCustomClipping(int enabled, int x0, int y0, int x1, int y1);
void v_setHardClipping(int enabled, int mode, int respectOrientation, int x0, int y0, int x1, int y1);

void v_setClientHz(int hz);
void v_setRefresh(int hz);
int v_getRefresh();

void v_removeIRQHandling();
void v_setupIRQHandling();

void v_enableExtendedSoundOut(int yesNo);
void v_enableSoundOut(int yesNo);
void v_enableButtons(int yesNo);
void v_enableJoystickDigital(int yesNoX1,int yesNoY1,int yesNoX2,int yesNoY2); // resets analog
void v_enableJoystickAnalog(int yesNoX1,int yesNoY1,int yesNoX2,int yesNoY2);  // resets digital
void v_enable50HzSync(int yesNo);
void v_disableReturnToLoader();


#ifdef USE_PERSISTENT_VECTORS
void v_clearPipeline();
#endif

extern int extendedIRQSound; // also plays sfx and ym in IRQ if available
extern int irqSound;
extern int irqButtons;
extern int irqJoyDigital;
extern int irqJoyAnalog;

void v_zeroWait();
void v_deflok();
 
void v_WaitRecal();
void v_WaitRecal_direct();
void v_resetDetection();
void v_calibrate();
uint8_t v_directReadButtons();
uint8_t v_readButtons();
void v_readJoystick1Digital();
void v_readJoystick1Analog();
void v_printString(int8_t x, int8_t y, char* string, uint8_t textSize, uint8_t brightness);
void v_printStringMax(int8_t x, int8_t y, char* string, uint8_t textSize, uint8_t brightness, int maxchars);
int v_printStringRaster(int8_t x, int8_t y, char* _string, int8_t xSize, int8_t ySize, unsigned char delimiter);
int v_printStringRaster16(int x, int y, char* _string, int8_t xSize, int8_t ySize, unsigned char delimiter);
int v_printStringRasterFont(int8_t x, int8_t y, char* _string, int8_t xSize, int8_t ySize, unsigned char delimiter, unsigned char *rlines[]);
int v_printStringBiRasterFont(int8_t x, int8_t y, char* _string, int8_t xSize, int8_t ySize, unsigned char delimiter, unsigned char *rlines[], int bidrectional);

void v_SettingsGUI(int ownWaitRecal);

extern unsigned char *alternate_rasterlines[9];
extern unsigned char *alternate_raster5[6];


void v_setScale(uint16_t s);
void v_setScaleForced(uint16_t s);

void v_moveToImmediate8(int8_t xLen, int8_t yLen);
void v_drawToImmediate8(int8_t xLen, int8_t yLen);

int iniHandler(void* user, const char* section, const char* name, const char* value);
void v_saveIni();


uint32_t v_millis(); // only using low counters!
uint32_t v_micros(); // only using low counters!



extern int (*executeDebugger)(int);


////////////////////////////////////////////////////////////////////////////
// sound stuff
////////////////////////////////////////////////////////////////////////////
typedef enum {
    NOT_PLAYING = 0,        ///<
    PLAY_END,       ///<
    PLAY_LOOP,       ///<
    PLAY_ONCE       ///<
} SfxMode;

void v_noSound();
void v_noSound_channel(int c);

void v_playDirectSampleAll(char *ymBufferLoad, int fsize, int rate);
void v_doSound();
void v_playSFXCont(unsigned char *buffer, int channel, int loop);
void v_playSFXStop(unsigned char *buffer, int channel);
void v_playSFXStart(unsigned char *buffer, int channel, int loop);

void v_playAllSFX();
void v_initYM(unsigned char *ymBuffer, uint16_t length, int loop);
int v_playYM();
int v_stopYM();
void v_setymPos(int i);
int v_isYMPlaying();

int v_loadYM(unsigned char *FILE_NAME, int len ,char *dest, char *loadbuf);


void v_writePSG_double_buffered(uint8_t reg, uint8_t data);
void v_writePSG_buffered(uint8_t reg, uint8_t data);
void v_writePSG(uint8_t reg, uint8_t data);
uint8_t v_readPSG_double_buffered(uint8_t reg);
uint8_t v_readPSG_buffered(uint8_t reg);
uint8_t v_readPSG(uint8_t reg);
int play_sfx1();
int play_sfx2();
int play_sfx3();
void v_PSG_writeDoubleBuffer();

void v_playIRQSample(char *buf, int size, int rate, int mode);
char *v_getSamplePlaying();
void v_stopSamplePlaying();

int v_loadRAW(const char *filename, unsigned char *largeEnoughBuffer);


////////////////////////////////////////////////////////////////////////////
// debug stuff
////////////////////////////////////////////////////////////////////////////

typedef struct {
    int id;
    char *command;
    char *commandShort;
    char *commandHelp;
    void (*commandHandler)(void);
} Command;
extern Command *userCommandList;
extern int browseSkip;
extern int browseMode;
extern int currentBrowsline;
extern int currentDisplayedBrowseLine;
extern int logOutput;

void v_error(char *message); // halts the program and displays an the message on the vectrex!
void v_errori(char *message, int i);
void v_message(char *message);

char *getParameter(int p);

extern int32_t currentCursorX; // 32 bit positioning value X, "normal" values are 16 bit,
                        // but enlarging the screen may leed to higher values
extern int32_t currentCursorY; // 32 bit positioning value Y
extern uint16_t currentScale; // currently active scale factor (T1 timer in VIA)
extern uint16_t lastScale; // last active scale factor (T1 timer in VIA)
extern uint8_t currentButtonState;
extern int8_t currentJoy1X;
extern int8_t currentJoy1Y;
extern int8_t currentJoy2X;
extern int8_t currentJoy2Y;
extern int16_t currentPortA; // == portA (also X SH)
extern int16_t currentYSH; // Y SH
extern int16_t currentZSH; // Brightness
extern int v_dotDwell;

// todo
typedef enum {
  CRANKY_CRANKY_VIA_B_DELAY_B0 = 0,               ///<
  CRANKY_CRANKY_VIA_B_DELAY_B1 = 0,               ///<
  CRANKY_CRANKY_VIA_B_DELAY_B2 = 0,               ///<
  CRANKY_CRANKY_VIA_B_DELAY_B3 = 0,               ///<
  CRANKY_BETWEEN_VIA_B = 16,               ///< uses above delay
  CRANKY_NULLING_WAIT = 32,               ///< uses fixed "6" cycles
  CRANKY_NULLING_CALIBRATE = 64,               ///< // does a calibrate after each "null" of Y
  CRANKY_T1_DELAYED = 128               ///< // not implemented
} CrankyFlags;

extern CrankyFlags crankyFlag;
extern uint8_t DELAY_ZERO_VALUE; // 70 // probably less, this can be adjusted, by max x position, the nearer to the center the less waits
extern uint8_t DELAY_AFTER_T1_END_VALUE;
extern uint8_t DELAY_AFTER_XSH_VALUE;
extern uint8_t DELAY_AFTER_YSH_VALUE;
extern uint8_t DELAY_PORT_B_BEFORE_PORT_A_VALUE;
extern uint8_t CRANKY_DELAY_Y_TO_NULL_VALUE;
extern uint8_t SCALE_STRENGTH_DIF;
extern uint8_t MAX_USED_STRENGTH;
extern uint8_t MAX_CONSECUTIVE_DRAWS;
extern uint8_t MINSCALE; //#define MINSCALE 5 //15 to low might interfere with switching shift states to soon -> shift stalling
extern uint8_t beamOffBetweenConsecutiveDraws;
extern uint8_t useDoubleTimer;
extern uint8_t usePipeline;
extern uint8_t keepDotsTogether;
extern uint8_t bufferType; // 0 = none, 1 = double buffer, 2 = auto buffer (if pipeline is empty -> use previous (battlezone!)
extern uint8_t calibrationValue; // tut calibration
extern uint8_t optimizationON;
extern uint8_t consecutiveDraws; // how many lines/moves were drawn/made directly after another! (without zeroing)
extern uint8_t orientation;
extern unsigned int cycleEquivalent;
extern unsigned int resetToZeroDifMax;
extern unsigned int Vec_Rfrsh;//0x3075;//12405; // 30000 = $7530 little endian = 3075 = 12405 50U;  // PLACEHOLDER FOR NOW -- FIX!
extern int16_t offsetX;
extern int16_t offsetY;
extern float sizeX;
extern float sizeY;

extern int Vec_Loop_Count;
extern unsigned int piCounterMark;
extern int lastViaWrite;
extern int setWaitMin; // in vectrex cycles
extern volatile int isIRQMode;
extern int clientHz;


extern unsigned char lastPortA;

#define VIA_B     ((unsigned short) 0x00) // VIA port B data I/O register
#define VIA_A     ((unsigned short) 0x01) // VIA port A data I/O register (handshaking)
#define VIA_SHIFT ((unsigned short) 0x0A) // VIA shift register

#define VIA_port_b    ((unsigned short) 0xD000) // VIA port B data I/O register
#define VIA_port_a    ((unsigned short) 0xD001) // VIA port A data I/O register (handshaking)
#define VIA_DDR_b     ((unsigned short) 0xD002) // VIA port B data direction register (0=input 1=output)
#define VIA_DDR_a     ((unsigned short) 0xD003) // VIA port A data direction register (0=input 1=output)
#define VIA_t1        ((unsigned short) 0xD004) // VIA timer 1 count
#define VIA_t1_cnt_lo ((unsigned short) 0xD004) // VIA timer 1 count register lo (scale
#define VIA_t1_cnt_hi ((unsigned short) 0xD005) // VIA timer 1 count register hi
#define VIA_t2        ((unsigned short) 0xD008) // VIA timer 2 count register BOTH BYTES
#define VIA_t2_cnt_lo ((unsigned short) 0xD008) // VIA timer 2 count register lo
#define VIA_t2_cnt_hi ((unsigned short) 0xD009) // VIA timer 2 count register hi
#define VIA_shift_reg ((unsigned short) 0xD00A) // VIA shift register
#define VIA_aux_cntl  ((unsigned short) 0xD00B) // VIA auxiliary control
#define VIA_cntl      ((unsigned short) 0xD00C) // VIA control register
#define VIA_int_flags ((unsigned short) 0xD00D) // VIA interrupt flags register
#define VIA_int_enable ((unsigned short) 0xD00E)    // VIA interrupt enable register

#define MAX(A,B) ((A)>(B)?(A):(B))
#define ABS(A) ((A)>0?(A):(-(A)))


#define ONE_WAIT_UNIT cycleEquivalent

// in ARM cycles
#define USE_TIME(a) do{if ((signed int)(a)>0) {;} } while(0)

// waiter in arm cycles
// that means in nano seconds
// waiter always in relation to last cleared cycle counter
//    this is usually the last VIA write
#define waitUntilAndUse(waiter) \
do {\
  unsigned int value;\
  /* Read CCNT Register */ \
  CCNT(value);\
  if ((waiter) >value)\
  {\
    USE_TIME((waiter)-value);\
  }\
  unsigned int value1;\
  do\
  {\
    CCNT(value1);\
  } while (value1 < (waiter) );\
} while (0)

#define waitUntil(waiter) \
do {\
  unsigned int value;\
  /* Read CCNT Register */ \
  do\
  {\
    CCNT(value);\
  } while (value < (waiter) );\
} while (0)

// all setter WAIT for "setWaitMin*ONE_WAIT_UNIT"
// all things starting with I_ also initialize the setWaitMin to 0
#define GET(address) vectrexread(address)


// usually "getters" do not need a delay
// I think in case of joystick read (orPSG in general?)
// there might be one needed
// since the last B- MUX is DIRECTLY in front
// therefore we also have a "wait getter" (which also resets)
static inline unsigned char I_GET(int address)
{
  waitUntilAndUse(setWaitMin*ONE_WAIT_UNIT);
  setWaitMin = 0;
  return vectrexread(address);
}

// some sets should "overstep" the waiter
// they should wait, but not reset the waiter
// e.g. if we change some register while "waiting" for another
#define SET(address, val) \
do\
{\
  waitUntilAndUse(setWaitMin*ONE_WAIT_UNIT);\
  vectrexwrite(address, val);\
  lastViaWrite = (address&0xf);\
} while(0)

// some sets should "overstep" the waiter
// they should wait, but not reset the waiter
// e.g. if we change some register while "waiting" for another
#define SET_noUse(address, val) \
do\
{\
  waitUntil(setWaitMin*ONE_WAIT_UNIT);\
  vectrexwrite(address, val);\
  lastViaWrite = (address&0xf);\
} while(0)


#define I_SET(address, val) \
do\
{\
  waitUntilAndUse(setWaitMin*ONE_WAIT_UNIT);\
  vectrexwrite(address, val);\
  PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET);\
  setWaitMin = 0;\
} while(0)


#ifdef SUPPORT_SAMPLES

// saving register A
// in case we play a sample and must restore A
#define I_SET_A(val) \
do\
{\
  waitUntilAndUse(setWaitMin*ONE_WAIT_UNIT);\
  vectrexwrite(0xd001, val);\
  PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET);\
  lastPortA=val;\
  setWaitMin = 0;\
} while(0)

#else
#define I_SET_A(a) I_SET(VIA_port_a,a)
#endif



#define I_SET_noUse(address, val) \
do\
{\
  waitUntil(setWaitMin*ONE_WAIT_UNIT);\
  vectrexwrite(address, val);\
  PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET);\
  lastViaWrite = (address&0xf);\
  setWaitMin = 0;\
} while(0)

#define I_SET_WORD_REVERSE(adr, v1, v2) do {I_SET( (adr)+1, v2); DELAY_CRANKY_BETWEEN_VIA_B(); I_SET(adr, v1); } while(0)
#define I_SET_WORD_ORDERED(adr, v1, v2) do {I_SET( (adr), v1); DELAY_PORT_B_BEFORE_PORT_A(); I_SET(adr+1, v2); } while(0)

#define SETW_inverse(address, val) do {SET_noUse((address), (val)&255);SET_noUse((address)+1, ((val)>>8)&255); } while(0)
#define SETW(address, val) do {SET_noUse(address, ((val)>>8)&255); SET_noUse((address)+1, (val)&255);} while(0)

#define I_SETW_inverse(address, val) do {I_SET_noUse((address), (val)&255);I_SET_noUse((address)+1, ((val)>>8)&255); } while(0)
#define I_SETW(address, val) do {I_SET_noUse(address, ((val)>>8)&255); I_SET_noUse((address)+1, (val)&255);} while(0)


// one "round" is measured with COUNTER 1
// one vectrex WRITE (a,b, shift) is measured with CYCLE COUNTER
// T1 "marks" are also measured with T1 counters -> but with mark and difference!
// T1 also marks the start of a ZERO    

#define setCounter1Mark() CCNT0(piCounterMark)

#define waitCounter1Mark(waiter) \
do {\
  static unsigned int value;\
  /* Read CCNT Register */ \
  do\
  {\
    CCNT0(value);\
    USE_TIME( (piCounterMark+((waiter)*ONE_WAIT_UNIT) ) -value);\
  } while (value < (piCounterMark+((waiter)*ONE_WAIT_UNIT) ) );\
} while (0)

#define DELAY_NOW(waiter)\
do {\
  unsigned int privateMark;\
  CCNT0(privateMark);\
  privateMark += ((waiter)*ONE_WAIT_UNIT);\
  unsigned int value;\
  /* Read CCNT Register */ \
  do\
  {\
    CCNT0(value);\
  } while (value < privateMark);\
} while (0)



#define V_BUTTONS_READ 1
#define V_JOY_DIGITAL_READ 2
#define V_JOY_ANALOG_READ 4


// strength needed for NON pipeline!
//#define SCALE_STRENGTH_DIF 4//2//4 //
//#define MINSCALE 5 //15 to low might interfere with switching shift states to soon -> shift stalling
#define POSITION_MARGIN 100 // margin from where two positions are taken as equal 16bit
#define POSTION_MARGIN_AND 0x0//0x7f//0x3f // 64


// delay value in vectrex cycles
//#define DELAY_NANO(n) delayNano((n))

/*
#ifdef MHZ1000
  #define DELAY_PI_CYCLE_EQUIVALENT 940//940 // nano seconds
#else
  #define DELAY_PI_CYCLE_EQUIVALENT 660 // nano seconds
#endif
*/


/* possibly add:
  DELAY_AFTER_T1_START_VALUE 2 // (my personal stupid vectrex)
  DELAY_AFTER_T1_SET_VALUE 2 // (wait after scale is set - if started to fast??? dunno, is that a thing?)
*/

#define SET_SHIFT_REG(v) I_SET(VIA_shift_reg, (v))

// usually the beam (~BLANK) is switched using the SHIFT register
// but using other VIA settings the BEAM can also be switched on/off using the
// CNTL register
//#define BEAM_LIGHT_BY_SHIFT 1 //
#define BEAM_LIGHT_BY_CNTL 1

#ifdef BEAM_LIGHT_BY_SHIFT
#define SWITCH_BEAM_ON() SET_SHIFT_REG(0xff)
#define SWITCH_BEAM_OFF() SET_SHIFT_REG(0x00)
#endif
#ifdef BEAM_LIGHT_BY_CNTL
#define SWITCH_BEAM_ON() SET(VIA_cntl, 0xee)
#define SWITCH_BEAM_OFF() SET(VIA_cntl, 0xce)
#endif





#define ADD_DELAY_CYCLES(c) do{setWaitMin += (c);} while (0)


#define DELAY_PORT_B_BEFORE_PORT_A() do{ADD_DELAY_CYCLES(DELAY_PORT_B_BEFORE_PORT_A_VALUE);} while (0)
#define DELAY_PORT_B_BEFORE_PORT_A_BRIGHTNESS() do{ADD_DELAY_CYCLES(DELAY_PORT_B_BEFORE_PORT_A_VALUE+1);} while (0)
#define DELAY_CRANKY_BETWEEN_VIA_B() do{if (crankyFlag & CRANKY_BETWEEN_VIA_B){ADD_DELAY_CYCLES(crankyFlag&0x07);}} while (0)// 
#define DELAY_CRANKY_Y_TO_NULL() do{if (crankyFlag & CRANKY_NULLING_WAIT){ADD_DELAY_CYCLES (CRANKY_DELAY_Y_TO_NULL_VALUE);}} while (0)// only VERY strange vectrex
#define DELAY_XSH() do{ADD_DELAY_CYCLES(DELAY_AFTER_XSH_VALUE);} while (0)
#define DELAY_ZSH() DELAY_YSH()
#define DELAY_YSH()  \
    do{ADD_DELAY_CYCLES(DELAY_AFTER_YSH_VALUE); \
    DELAY_CRANKY_BETWEEN_VIA_B();} while (0)

#define START_T1_TIMER()  \
do { \
    if (lastScale == currentScale) \
    { \
        SET(VIA_t1_cnt_hi, currentScale>>8);  \
    } \
    else \
    { \
        SETW_inverse(VIA_t1, currentScale);  /* scale due to "enlargement" is 16 bit! */ \
    }  \
    setCounter1Mark(); \
} while (0)
#define WAIT_T1_END_LAST() do{waitCounter1Mark(lastScale+DELAY_AFTER_T1_END_VALUE);   } while (0)
#define WAIT_T1_END()      do{waitCounter1Mark(currentScale+DELAY_AFTER_T1_END_VALUE);} while (0)
#define START_WAIT_T1() do{START_T1_TIMER();WAIT_T1_END();} while (0)


// chose a good scale for the max size given
// depending on "currentScale" (since it also needs time to switch scales)
// and on size, size should be so that
// it should maxVSizeWord/scale == very near to 127 (8 bit max)
//
// attention! for cranky vectrex large (SH values in Y) values (>100?) need additional waits
// when written to Port A of VIA
#define GET_OPTIMAL_SCALE(a,b) GET_OPTIMAL_SCALE_SMS(a,b,MAX_USED_STRENGTH)
#define SET_OPTIMAL_SCALE(a,b) \
    do {lastScale = currentScale;currentScale = GET_OPTIMAL_SCALE(a,b); \
    if (currentScale <= MINSCALE) currentScale = MINSCALE;\
    } while (0)
    
#define GET_OPTIMAL_SCALE_SMS(a,b,sms) ((MAX(ABS(a), ABS(b))+(sms-1))/sms)

// try using the same scale for small differences
#define SET_OPTIMAL_SCALE_SMS(a,b,sms) \
    do {lastScale = currentScale;currentScale = GET_OPTIMAL_SCALE_SMS(a,b, sms); \
    if (currentScale <= MINSCALE) currentScale = MINSCALE;\
    } while (0)

#define UNZERO() SET(VIA_cntl, 0xCE) // disable zeroing, otherwise no positioning can be done
#define ZERO_AND_CONTINUE() do{SET(VIA_cntl, 0xCC);setCounter1Mark();currentCursorX = 0; currentCursorY = 0;} while (0)
#define ZERO_AND_WAIT() do{ \
    ZERO_AND_CONTINUE(); \
    waitCounter1Mark(DELAY_ZERO_VALUE);} while (0)

#define SET_YSH_IMMEDIATE_8(v) do{ \
    I_SET(VIA_port_b, 0x80); \
    DELAY_PORT_B_BEFORE_PORT_A(); \
    currentYSH=currentPortA=(uint8_t)(v); \
    I_SET(VIA_port_a, currentPortA); \
    DELAY_YSH();} while (0)

#define SET_XSH_IMMEDIATE_8(v) do{ \
    I_SET(VIA_port_b, 0x81); \
    DELAY_PORT_B_BEFORE_PORT_A(); \
    currentPortA=(uint8_t)(v); \
    I_SET(VIA_port_a, currentPortA); \
    DELAY_XSH();} while (0)

// leaves with mux set to y integrator!
// sets 16 bit values divided by current scale!
// actually: due to possible display scaling, input values can now be larger 16bit
#define SET_YSH16(v) \
  do { \
    int16_t t = (uint8_t)((v)/(currentScale+SCALE_STRENGTH_DIF));  \
    if (t != currentYSH) \
    { \
        if (t != currentPortA) \
        { \
            I_SET(VIA_port_b, 0x80); \
            DELAY_PORT_B_BEFORE_PORT_A(); \
            currentPortA = currentYSH = t; \
            I_SET(VIA_port_a, currentPortA); \
        } \
        else \
        { \
            I_SET(VIA_port_b, 0x80); \
            currentYSH = currentPortA; \
        } \
    } \
    DELAY_YSH(); \
} while (0)

// sets 16 bit values divided by current scale!
// actually: due to possible display scaling, input values can now be larger 16bit
// if no value set, this "at least" switches the MUX off
#define SET_XSH16(v) \
  do { \
    int16_t t = (uint8_t)((v)/(currentScale+SCALE_STRENGTH_DIF));  \
    if (t != currentPortA) \
    { \
        I_SET(VIA_port_b, 0x81); \
        DELAY_PORT_B_BEFORE_PORT_A(); \
        currentPortA = t; \
        I_SET(VIA_port_a, currentPortA); \
    }  \
    else \
  { \
        I_SET(VIA_port_b, 0x81); \
  } \
    DELAY_XSH(); \
} while (0)


#define SET_YSH8(t) \
do { \
  if (t != currentYSH) \
  { \
  if (t != currentPortA) \
  { \
  I_SET(VIA_port_b, 0x80); \
  DELAY_PORT_B_BEFORE_PORT_A(); \
  currentPortA = currentYSH = t; \
  I_SET(VIA_port_a, currentPortA); \
  } \
  else \
  { \
  I_SET(VIA_port_b, 0x80); \
  currentYSH = currentPortA; \
  } \
  } \
  DELAY_YSH(); \
  } while (0)

#define SET_XSH8(t) \
do { \
  if (t != currentPortA) \
  { \
  I_SET(VIA_port_b, 0x81); \
  DELAY_PORT_B_BEFORE_PORT_A(); \
  currentPortA = t; \
  I_SET(VIA_port_a, currentPortA); \
  }  \
  else \
  { \
  I_SET(VIA_port_b, 0x81); \
  } \
  DELAY_XSH(); \
  } while (0)

  
#define WAIT_CYCLE_NANO(n) do{ \
    uint32_t l = ((n)>>2)-((n)>>5)+((n)>>7)-((n)>>9); /* divided by 4 */  \
    if (!l) {l=1;} \
    else { l+=(n)>>7; \
    }asm volatile( "0:" "SUBS %[count], #2;" "SUBS %[count], #-1;" "BNE 0b;" :[count]"+r"(l) ); \
    } while(0)
/*
// 4 cycles per loop?
// 1 cycle = 1 nano second
#define CYCLE_PER_LOOP 4

#define GET_SYSTEM_TIMER_LO *(bcm2835_st + BCM2835_ST_CLO/4)
#define SET_TIMER_MARK do{timerMark = (volatile uint32_t)*(bcm2835_st + BCM2835_ST_CLO/4);} while(0)

// 32 bit timer, perhaps reset each timing?
// adjust overhead accordingly
// 1 tick = 0,001 us
// ticks * 1000 = 1000 us
#define WAIT_MICRO_TIME(utime) do{ \
       __sync_synchronize(); \
       uint32_t tstart = GET_SYSTEM_TIMER_LO; \
       uint32_t tend = tstart +(utime); \
       __sync_synchronize(); \
       while (GET_SYSTEM_TIMER_LO < tend);} while (0)

#define WAIT_MICRO_MARK_DELTA(utime)  do{ \\
       uint32_t tend = timerMark +(utime); \
       __sync_synchronize(); \
       while (GET_SYSTEM_TIMER_LO < tend);} while (0)



// max 4000 micro
#define delayMicro(u) delayNano(((u)*1000))

// max 4 milli
#define delayMilli(u) delayNano(((u)*10000000))
*/
#define OFFSET_CYCLE_OVERHEAD 17// # of cycles "wasted" by function calling etc
#define OFFSET_TIMER_OVERHEAD 24// # of cycles "wasted" by function calling etc



typedef struct {
  int yStart;
  int xStart;
  int yEnd;
  int xEnd;
  int y0;
  int x0;
  int y1; // int8_t fontWidth;
  int x1; // int8_t fontHeight;
  int intensity;
  int pattern; // halfOffset
  int sms;
  int timingForced;
  int force;
#ifdef USE_PERSISTENT_VECTORS  
  int id; // 
#endif  
  
  unsigned char **rlines;
  void *next;
  void *previous;
  char debug[240]; // reused for raster strings!
} VectorPipelineBase;

typedef enum {
  PL_BASE_NO_FORCE = 0,
  PL_BASE_FORCE_ZERO = 1,
  PL_BASE_FORCE_NO_ZERO = 2,
  PL_BASE_FORCE_DEFLOK = 4,
  PL_BASE_FORCE_RESET_ZERO_REF = 8, 
  PL_BASE_FORCE_CALIBRATE_INTEGRATORS = 16, 
  PL_BASE_FORCE_STABLE = 32,                // < does never "move" - should be at the start of  drawingsr
  PL_BASE_FORCE_EMPTY = 64,
  PL_BASE_FORCE_NOT_CLIPPED = 128,
  PL_BASE_FORCE_USE_FIX_SIZE = 256, // optimal scale is not calculated, the currently set timing is used!
  PL_BASE_FORCE_USE_DOT_DWELL = 512, // 
  PL_BASE_FORCE_IS_RASTER_DISPLAY = 1024, // 
  PL_BASE_FORCE_IS_BI_RASTER_DISPLAY = 1024*2 // 

#ifdef USE_PERSISTENT_VECTORS  
  , PL_BASE_PERSISTENT_ENTITY = 2048 // 
#endif  
} BaseActions;

typedef enum {
  PL_ZERO = 0,   
  PL_DEFLOK,
  PL_MOVE,             
  PL_DRAW_PATTERN,
  PL_DRAW,             
  PL_DRAW_DOT,
  PL_SET_BRIGHTNESS, 
  PL_RASTER_TEXT, 
  PL_RASTER_TEXT_BI_DIRECTIONAL,
  PL_END             
} VectorActions;

typedef enum {
  PL_DEACTIVATE_ZERO   = 1,               ///<
  PL_SWITCH_ZERO_AFTER = 2,
  PL_Y_MUST_BE_SET     = 4,   // when moving, Y integrator must be set
  PL_Y_A_MUST_BE_SET   = 8,  // when moving, Y integrator must be set and reg A must be loaded with y value
  PL_X_A_MUST_BE_SET   = 16,
  PL_I_A_MUST_BE_SET   = 32,  // when moving, Y integrator must be set and reg A must be loaded with y value
  PL_LAST_MUST_FINISH  = 64,
  PL_SWITCH_BEAM_ON    = 128,
  PL_SWITCH_BEAM_OFF   = 256,
  PL_SWITCH_BEAM_OFF_AFTER = 512,
  PL_MUX_Y_MUST_BE_SET = 1024,
  PL_MUX_X_MUST_BE_SET = 2048,
  PL_IS_RAMPING        = 4096,
  PL_LAST_IS_RAMPING   = 8192,
  PL_Y_DELAY_TO_NULL   = 8192*2,
  PL_T1_LO_EQUALS      = (8192*2)*2,
  PL_CALIBRATE_0       = (8192*2)*2*2,
  PL_CALIBRATE         = (8192*2)*2*2*2,
  PL_DEFLOK_AFTER      = (8192*2)*2*2*2*2
} ActionFlags;

typedef struct {
  VectorActions type;
  int flags;
  int y;
  int x;
  int y0;
  int x0;
  int yRest;
  int xRest;
  int yStart;
  int xStart;
  int yEnd;
  int xEnd;
  int pattern; // if 0, than treated as ff -> no pattern
  int intensity;
  unsigned char **rlines;
  int this_timing; // scale or cycle delay
  int last_timing; // scale or cycle delay
  char debug[240]; // reused for raster strings!
//  VectorPipelineBase *base;
} VectorPipeline;
extern VectorPipeline *pl;

#define MAX_PIPELINE 3000

extern VectorPipelineBase *cpb;
//extern VectorPipelineBase pb[MAX_PIPELINE];
extern int pipelineCounter; // used in aae to test, whether pipeline is areadly filled!

extern int commonHints;

#define GLOBAL_FLAG_IS_INIT 1  // bit 0

// 0x0000008c is a pointer to a structur of->
typedef struct {
  unsigned char flags;
  void (*loader)(void);
  unsigned char orientation;
  void *lastSelection; // in loader menu
  char parameter1[127];
  char parameter2[127];
  char parameter3[127];
  char parameter4[127];
} GlobalMemSettings;

extern GlobalMemSettings *settings;


inline static void v_resetIntegratorOffsets0()
{
//  printf("CALIBRATION 0\r\n");
  I_SET (VIA_port_b, 0x81);
  DELAY_PORT_B_BEFORE_PORT_A();
  I_SET (VIA_port_a, 0x00);
  ADD_DELAY_CYCLES(4);
  I_SET (VIA_port_b, 0x80);
  ADD_DELAY_CYCLES(6);
  // reset integrators
  I_SET (VIA_port_b, 0x82);    // mux=1, enable mux - integrator offset = 0
  ADD_DELAY_CYCLES(6);
  I_SET (VIA_port_b, 0x81);    // disable mux
  ADD_DELAY_CYCLES(4);
  currentPortA=0x100;// non regular value!
}
void resetImportantStuff();



extern int (*checkExternal)(VectorPipelineBase **, VectorPipeline **, int *, int *, int *);
#define CV_SPECIAL_NONE 0
#define CV_SPECIAL_SKIP 1
#define CV_SPECIAL_DEFLOK 2
#define CV_SPECIAL_AFTER 0x80
void saveScreenshot(void);

#include <vectrex/ini.h>


typedef struct 
{
  char *name;
  int value;
} NameValuePair;

typedef struct 
{
  char *replace;
  char *with;
} NameReplacePair;

typedef struct 
{
  char *name;
  unsigned char *destination;
  NameValuePair values[];
} DipSwitchSetting;


int evaluateInputIniLine(const char *);
int evaluateLine(const char *valueString, NameReplacePair switchMap[]);



typedef struct 
{
  char *name;
  signed char *destination;
  int mapping;
} InputMapping;

#define JOYPAD_1_DIGITAL 0x1000 // +-1 or 0
#define JOYPAD_2_DIGITAL 0x2000 // +-1 or 0
#define JOYPAD_1_ANALOG 0x4000 // -128 - 127
#define JOYPAD_2_ANALOG 0x8000 // -128 - 127

#define JOYPAD_UP 0x0100 
#define JOYPAD_DOWN 0x0200 
#define JOYPAD_LEFT 0x0400 
#define JOYPAD_RIGHT 0x0800 

#define BUTTON_AND 0x10000 // 

#define BUTTON_1_1 0x01 // 0 or 1
#define BUTTON_1_2 0x02 // 0 or 1
#define BUTTON_1_3 0x04 // 0 or 1
#define BUTTON_1_4 0x08 // 0 or 1
#define BUTTON_2_1 0x10 // 0 or 1
#define BUTTON_2_2 0x20 // 0 or 1
#define BUTTON_2_3 0x40 // 0 or 1
#define BUTTON_2_4 0x80 // 0 or 1

void v_doInputMapping(InputMapping inputMapping[]);





void v_getDipSettings(DipSwitchSetting *dips[], char * title);

extern int useUSB;
extern int usbKeyboardAvailable;
// USB END
int v_initKeyboard();


#define ITEM_MAX_ITEMS 30
#define ITEM_MAX_STRINGSIZE 80

typedef struct
{
  char text[ITEM_MAX_STRINGSIZE];
  int baseBrightness;
  
} DisplayItem;

typedef struct
{
  int x;
  int y;
  int lineSpacing; // factor how much "room" one line should use (vertically)
  
  int displayArrows;
  int rollover;
  int startYOffset; // adjusted by scrolls, this is a pixel offset!
  int itemOffset; // first item to be displayed (middle item)
  int itemDisplayCount; // number of items that are displayed at one time
  
  int itemCount; // current count of items in all of the itemlist
  int currentMainItem; // current center item

  int autoRaster;
  int displayRectangle;
  int fade;
  int fontWidth;
  int fontHeight;
  void *rlines;
  int rasterlineCount; // space between two rows of text
  
  DisplayItem *items[];
} DisplayList;

extern DisplayList defaultDisplayList;


void itemListDisplay();
void itemListScrollUp(int step);
void itemListScrollDown(int step);
void itemListAdd(char* text, int brightness);
void itemListClear();
void itemListSetPos(int x, int y);
