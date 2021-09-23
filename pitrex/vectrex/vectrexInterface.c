// todo ADD to Pipeline
// 8 bit for now
//void v_printBitmapUni(unsigned char *bitmapBlob, int width, int height, int sizeX, int x, int y)

#define SUPPORT_SAMPLES
#define MULTI_SAMPLES
//#define ENABLE_SAMPLE_MIXER // sounds shitty - but works!

// if defined ALL debug features, incls terminal mode are off!
//#define NO_DEBUG
// intensive debug output, if "mydebug" is set from terminal
#define MY_DEBUG

int inIRQ_MODE = 0;
 

// browse mode info
#ifdef NO_DEBUG
#define LINE_DEBUG_OUT
#else
#define LINE_DEBUG_OUT(...) \
        if (((browseMode) && (lineNo==currentBrowsline) && (currentDisplayedBrowseLine != currentBrowsline)) || (myDebug))  \
        { \
          printf(__VA_ARGS__); \
        }
#endif

#ifdef NO_DEBUG
#undef PITREX_DEBUG // browsemode, terminal stuff
#undef DEBUG_TO_FILE
#undef MY_DEBUG
#endif


#ifdef MULTI_SAMPLES

//#define SAMPLE_MAX 100
#define SAMPLE_MAX 5

typedef struct {
    char *data;
    unsigned int pos;
    unsigned int size;
    int mode;
    int used;
} OneSample;

int v_multiSampleEnabled = 0;
OneSample allSamples[SAMPLE_MAX];
#endif

/*


*/



/*

With IRQ mode, only "double buffered" sound works!
DO not execute ANY routine, that writes to VIA!!!

This may lead to an unrecoverable CRASH!

1. Cases

Display Hz - screen refreh of vectrex
Client Hz - refresh of running emulaor

A) Best case, Pi Zero is fast enough to run both at 100 percent,
  than it doesn't matter which mode is used.
  If both Hz are the same - I would suggest: running IRQ mode = 1, with Vectrex Hz = 50, client Hz = 0 (synced with vectrex)
  If both Hz are different - I would suggest: running IRQ mode = 1, with Vectrex Hz = 50, client Hz = 60 

B) Pi is not fast enough to run without IRQ mode 100%, but with IRQ mode it is (IRQ mode is abou 10% faster overall)
  If both Hz are the same - I would suggest: running IRQ mode = 1, with Vectrex Hz = 50, client Hz = 0 (synced with vectrex)
  If both Hz are different - I would suggest: running IRQ mode = 1, with Vectrex Hz = 50, client Hz = 60 

C) Pi is not fast enough to run 100% even in IRQ mode
  Regardless of Hz - I would suggest: running IRQ mode = 1, with Vectrex Hz = 50, client Hz = 0 (synced with vectrex)
  If not "synced" with vectrex (sync goes vice / versa), the system will try with all his might
  to display in 50Hz, even to the point where it slows down emulation.
  If the display can not reach 50Hz, all that the emulation update receives ... are the IRQ bits and pieces while
  drawing, if that is not enough to calculate a correct game speed, than the emulator just slows down!
  (Here SBT DO help!)
  The "normal" emulators need about 10-15% time, the SBT less than half!
  
  If you want the game to have the "correct" speed, regardless of "flicker", than I would suggest, set the 
  client Hz, to the game speed needed, and the vectrex output to the highest, where the emulation
  speed can still be achieved.
  
  
  
C) example Gravitar:
Main screen (with all the objects/stations)

Refresh no IRQ Mode: 39Hz (also game speed)
  
Refresh IRQ Mode NO sync: Screen 47Hz (game speed 42Hz)

Refresh IRQ Mode sync: Screen 45Hz (game speed 45Hz)
  
Refresh IRQ Mode: Screen 40Hz (game speed 60Hz)


not allowed 
inner functions that write to VIA
- PSG write functions
- "macros" SET/GET ZERO UNZERO T1WAIT...
- deflok
- integrator calibrations
- immediate functions

// TODO
add NoBuzz calibration!
0.0) precognition T1 timer???
0.3) add cranky settings to config
document / video calibration

delay to zero in respect to the actual position we are on!
2) save config for individual emu
4) Video of config
5) faster emu of "lists"
6) different calibrations for scale ranges, one calibration for a complete cluster
- combine MAX CONSECUTIVE DRAWS - with clusterings, finish a cluster and THAN Zero


*/

//Drift control not working any more (no buzz),
/*
CLUSTER
A cluster is 1 or more vectors (preferable more), which are drawn
in one go without zeroing.
Additional attributes may be:
- all one color
- all draw (no move)

In battle zone the score - the last "digit" should be moved in front of the "score" cluster


Pokey




Pipeline implementation:

a) Base Pipeline
Every time a vector is added to the interface one VectorPipelineBase is configured.

The Base pipeline is an array of MAX_PIPELINE elements held in the array:
  VectorPipelineBase pb[MAX_PIPELINE];

The global variable cpb (currentBasePipeline) always holds the next available item.
When it is used, the next item must be gotten with a sequence of:
(since we allways reuse the same array elements, we must ensure they are clean from start)

    cpb = &pb[++pipelineCounter];
    cpb->force = 0;
    cpb->pattern = 0;
    cpb->debug[0] = 0;

Once per round (or less, if double buffer is active) the contents of the
VectorPipelineBase is processed and ( handlePipeline() ) - and a second stage VectorPipeline
is built.
The goal is that "VectorPipeline" has all information needed to (repeatedly) be drawn as fast as possible.

handlePipeline()
Does following at the moment:
a) processes clipping if needed
b) sorts "stable" vectors to the beginning of the list
c) sorts dots to the end of the list
d) builds optimal vector scales
e) handles (some) hints
f) can handle (starting to) debug information and output these

The VectorPipeline pipeline itself is displayed with:
  void displayPipeline().

 */



#ifdef PITREX_DEBUG
int myDebug;
#endif

/*  Version 0.x
 */

/***********************************************************************/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h> // these are fresstanding includes!
#include <stdint.h> // also "available":  <float.h>, <iso646.h>, <limits.h>, <stdarg.h>
#include <ctype.h>

#include <pitrex/pitrexio-gpio.h>
#include <pitrex/bcm2835.h>
#include "vectrexInterface.h"


// ff used for state saving
// state are saved in two 1024 byte blobs
// one for vectrexInterface, one for the "user"
// the blobs can be cast after loading/saving to anything one wants...
//#include <baremetal/rpi-base.h>
#include <baremetal/rpi-aux.h>
#include <baremetal/rpi-gpio.h>

#include "interrupt.i"

unsigned int iCounter =0;
uint32_t releaseIRQTime = 0;

// USB 
typedef void TInterruptHandler (void *pParam);
TInterruptHandler *usbIRQHandler = 0;
void *usbIRQParam = 0;
int useUSB=0;
int usbKeyboardAvailable = 0;
// USB END
int stdinEcho = 1;

int clearPipelineIssued = 0;
  




/***********************************************************************/


int32_t currentCursorX; // 16 bit positioning value X (stored in long), with sizing!
int32_t currentCursorY; // 16 bit positioning value Y (stored in long)
uint16_t currentScale; // currently active scale factor (T1 timer in VIA)
uint16_t lastScale; // last active scale factor (T1 timer in VIA)

uint8_t currentButtonState; // bit values as in vectrex
uint8_t internalButtonState; // bit values as in vectrex

int8_t currentJoy1X; // -127 left, 0 center, +127 right
int8_t currentJoy1Y; // -127 up, 0 center, +127 down
int8_t currentJoy2X; // not used yet
int8_t currentJoy2Y; // not used yet
int internalJoy1X; // -127 left, 0 center, +127 right
int internalJoy1Y; // -127 up, 0 center, +127 down
int internalJoy2X; // not used yet
int internalJoy2Y; // not used yet
int v_dotDwell;
unsigned int roundCycles;
int8_t commonIntensity;
unsigned char lastPortA=0;

#ifdef USE_PERSISTENT_VECTORS
int clearNextPipeline = 0;
void v_clearPipeline()
{
  clearNextPipeline = 1;
}
#endif


// these are 16 bit to allow illegal values!
// illegal values = unkown state
int16_t currentPortA; // == portA (also X SH) = current X Strength
int16_t currentYSH; // Y SH = current Y Strength
int16_t currentZSH; // Z SH = Brightness (if bit 7 is set -> brightness is off!)

int lastPLSize=0;
int selectedCalibrationMenu;
int selectionCalibrationMade;
uint8_t ioDone; // vectrex IO should only be done once per "round"

uint32_t timerMark;

uint32_t clientRate = 0;

void v_initSound();
void v_initDebug();
void handleUARTInterface();

// I don't think the "medians" really do anything
// but for now I leave them enabled.
#define MEDIAN_MAX 5
int medianX1[MEDIAN_MAX];
int medianY1[MEDIAN_MAX];
int medianX2[MEDIAN_MAX];
int medianY2[MEDIAN_MAX];

int lastViaWrite;
int setWaitMin;

// pipeline bases, that are written to 
// from the "user program
VectorPipelineBase BPL0[MAX_PIPELINE];
VectorPipelineBase BPL1[MAX_PIPELINE];
VectorPipelineBase *_BPL[]={BPL0,BPL1};
VectorPipelineBase *pb; // current base pipeline 
VectorPipelineBase *cpb; // current base pipeline element that is "written" to
int basePipeLineWriting = 0;
int pipelineFilled;
int pipelineCounter; // ONE which is currently written to!

VectorPipeline VPL0[MAX_PIPELINE];
VectorPipeline VPL1[MAX_PIPELINE];
VectorPipeline VPL2[MAX_PIPELINE];
VectorPipeline *_VPL[]={VPL0,VPL1,VPL2};
VectorPipeline *pv; // current vector pipeline 
int vectorPipeLineIsRead = 1;
int vectorPipeLineNextRead = 2;
int vectorPipeLineWriting = 0;

int biRasterReturnDelayLeft = 9;
int biRasterReturnDelayRight = 9;
int base_biRasterDown = 13;
float biRasterDownDriftModifier = 0;// 


/* values that we remember for possible optimization purposes */



uint8_t DELAY_AFTER_XSH_VALUE;
uint8_t DELAY_AFTER_YSH_VALUE;
uint8_t DELAY_PORT_B_BEFORE_PORT_A_VALUE;
uint8_t CRANKY_DELAY_Y_TO_NULL_VALUE;
uint8_t MAX_USED_STRENGTH;
uint8_t MAX_CONSECUTIVE_DRAWS;
uint8_t DELAY_ZERO_VALUE; // 70 // probably less, this can be adjusted, by max x position, the nearer to the center the less waits
uint8_t DELAY_AFTER_T1_END_VALUE;
uint8_t SCALE_STRENGTH_DIF;
uint8_t MINSCALE; //#define MINSCALE 5 //15 to low might interfere with switching shift states to soon -> shift stalling
uint8_t calibrationValue; // tut calibration
uint8_t optimizationON;
uint8_t useDoubleTimer;
uint8_t usePipeline;
uint8_t consecutiveDraws; // how many lines/moves were drawn/made directly after another! (without zeroing)
uint8_t beamOffBetweenConsecutiveDraws;
uint8_t bufferType;
uint8_t keepDotsTogether;
uint8_t wasIRQMode = 255;
uint8_t inGameSettingsAllowed = 0;

unsigned int Vec_Rfrsh; // 30000 cylces (vectrex) = $7530, little endian = $3075
unsigned int cycleEquivalent;
unsigned int resetToZeroDifMax;
uint8_t orientation;
int16_t offsetX;
int16_t offsetY;
float sizeX;
float sizeY;
CrankyFlags crankyFlag; // cranky should be checked during calibration! In "VecFever" terms cranky off = burst modus

#ifdef USE_PERSISTENT_VECTORS  
static int id_counter; // 
#endif  


int inCalibration;



int commonHints;

unsigned int piCounterMark;

int clipActive;
int clipminX;
int clipminY;
int clipmaxX;
int clipmaxY;
int clipMode;


int currentMarkedMenu = 0;
int menuOffset = 0;  


int logOutput=0;
volatile int isIRQMode = 0;
int clientHz = 50;

#define SYNC_DISPLAY_FINISHED 1
#define SYNC_CLIENT_FINISHED 2

int isSynchronizedWithIRQ = 0;
volatile int synqSignalIRQ = 0;
int allowReturnToLoader = 1;
void v_disableReturnToLoader()
{
  allowReturnToLoader = 0;
}


char *knownName;

int browseMode;
int browseSkip;
int currentBrowsline;
int currentDisplayedBrowseLine;
int valueChangeDelay;
int saveresetToZeroDifMax;
/***********************************************************************/
int (*executeDebugger)(int);
int noExecuteDebugger(int x)
{
  printf("Debugger not set!\r\n");
}

int customClipxMin;
int customClipyMin;
int customClipxMax;
int customClipyMax;
int customClippingEnabled;



/* should be called once on startup / reset
 */
GlobalMemSettings *settings;

#include "vectorFont.i" // includes font definition and string printing routines
#include "rasterFont.i" // includes font definition and string printing routines

#include "ini.c"

// for now INI setting just stupidly overwrite other saved settings!
int iniHandler(void* user, const char* section, const char* name, const char* value)
{
    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    #define MATCH_NAME(n) strcmp(name, n) == 0
  
    if (MATCH_NAME("DELAY_AFTER_XSH_VALUE")) DELAY_AFTER_XSH_VALUE = atoi(value); else 
    if (MATCH_NAME("DELAY_AFTER_YSH_VALUE")) DELAY_AFTER_YSH_VALUE = atoi(value); else 
    if (MATCH_NAME("DELAY_PORT_B_BEFORE_PORT_A_VALUE")) DELAY_PORT_B_BEFORE_PORT_A_VALUE = atoi(value); else 
    if (MATCH_NAME("CRANKY_DELAY_Y_TO_NULL_VALUE")) CRANKY_DELAY_Y_TO_NULL_VALUE = atoi(value); else 
    if (MATCH_NAME("DELAY_ZERO_VALUE")) DELAY_ZERO_VALUE = atoi(value); else 
    if (MATCH_NAME("DELAY_AFTER_T1_END_VALUE")) DELAY_AFTER_T1_END_VALUE = atoi(value); else 
    if (MATCH_NAME("SCALE_STRENGTH_DIF")) SCALE_STRENGTH_DIF = atoi(value); else 
    if (MATCH_NAME("MAX_USED_STRENGTH")) {MAX_USED_STRENGTH = atoi(value);} else 
    if (MATCH_NAME("RESET_TO_ZERO_DIF_MAX")) resetToZeroDifMax = atoi(value); else 
    if (MATCH_NAME("CRANKY_FLAG")) crankyFlag = atoi(value); else 
    if (MATCH_NAME("CYCLE_EQUIVALENT")) cycleEquivalent = atoi(value); else 
    if (MATCH_NAME("ORIENTATION")) orientation = atoi(value); else 
    if (MATCH_NAME("CALIBRATION_VALUE")) calibrationValue = atoi(value); else 
    if (MATCH_NAME("USE_PIPELINE")) usePipeline = atoi(value); else 
    if (MATCH_NAME("BUFFER_TYPE")) bufferType = atoi(value); else 
    if (MATCH_NAME("BEAM_OFF_BETWEEN_CONSECUTIVE_DRAWS")) beamOffBetweenConsecutiveDraws = atoi(value); else 
    if (MATCH_NAME("REFRESH_HZ")) v_setRefresh(atoi(value)); else  // refreh in Hz
    if (MATCH_NAME("SIZE_X")) sizeX = atof(value); else  // 1.0 is "normal", this is a float!
    if (MATCH_NAME("SIZE_Y")) sizeY = atof(value); else  // 1.0 is "normal", this is a float!
    if (MATCH_NAME("OFFSET_X")) offsetX = atoi(value); else  //positive -> right
    if (MATCH_NAME("OFFSET_Y")) offsetY = atoi(value); else  // positive up
    if (MATCH_NAME("LOG_OUTPUT")) logOutput = atoi(value); else  // if != 0, than write all "printf" also to filesystem!
    if (MATCH_NAME("MINSCALE")) MINSCALE = atoi(value); else  // if != 0, than write all "printf" also to filesystem!
    if (MATCH_NAME("DOUBLE_TIMER")) useDoubleTimer = atoi(value); else  // default 0
    if (MATCH_NAME("KEEP_DOTS_ORDERED")) keepDotsTogether = atoi(value); else  // default 0
    if (MATCH_NAME("OPTIMIZATION")) optimizationON = atoi(value); else  

    if (MATCH_NAME("RASTER_RETURN_DELAY_LEFT")) biRasterReturnDelayLeft = atoi(value); else  
    if (MATCH_NAME("RASTER_RETURN_DELAY_RIGHT")) biRasterReturnDelayRight = atoi(value); else  
    if (MATCH_NAME("RASTER_DOWN_BASE")) base_biRasterDown = atoi(value); else  
    if (MATCH_NAME("RASTER_DOWN_MODIFIER")) biRasterDownDriftModifier = atof(value); else  
      
    if (MATCH_NAME("IN_GAME_SETTINGS")) inGameSettingsAllowed = atoi(value); else  
    {
        // since we use this with "cascades"
        // do not notify if a setting was not found!
        // printf("Ini-Handler, unkown entry: %s = %s", name, value);
        return 0;  /* unknown section/name, error */
    }
    return 1;
}




#ifdef DEBUG_TO_FILE
FILE *__debugf=0;
#endif

void resetImportantStuff()
{
  fsInit = 0;
#ifdef DEBUG_TO_FILE
  __debugf=0;
#endif
}
int (*checkExternal)(VectorPipelineBase **, VectorPipeline **, int *, int *, int *);
int irqSound;
int extendedIRQSound;
int irqButtons;
int irqJoyDigital;
int irqJoyAnalog;

void v_init()
{ 
  inCalibration = 0;
  lastViaWrite = -1;
  setWaitMin = 0;
  logOutput=0;
  GlobalMemSettings **settingsPointer;
  settingsPointer = (GlobalMemSettings **)0x0000008c;
  settings = *settingsPointer; 
 
  unsigned char *buffer;
  buffer = malloc(200000);

  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  initFileSystem();

  // once to ENABLE in general!
  PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET|COUNTER_ZERO);

  printf("\r\nSettingPointer: %08x, settings: %0x08\r\n", settingsPointer, settings);
  printf("v_init()\r\n");
/*
  uint32_t timerValue_start;
  uint32_t timerValue_end;
  // measure time
  {
    
      for (int t = 0; t<10; t++)
      {
          // find a good start
          Vec_Rfrsh = 0x3075;//0x3075;//12405; // 30000 = $7530 little endian = $3075 = 12405
          SETW (VIA_t2, Vec_Rfrsh); //Set T2 timer to 30000 cycles and start timer and reset IFlag
          while ((GET(VIA_int_flags) & 0x20) == 0) ;

          // 50 times the vectrex T2 
          // is 1 second
          PMNC(CYCLE_COUNTER_ENABLE|COUNTER_ZERO);
          timerValue_start = rpiSystemTimer->counter_lo;
          int i;
          for (i=0;i<50;i++)
          {
            SETW (VIA_t2, Vec_Rfrsh); //Set T2 timer to 30000 cycles and start timer and reset IFlag
            while ((GET(VIA_int_flags) & 0x20) == 0) ;
          }
          
          unsigned int nanoCounter;
          CCNT0(nanoCounter);
          timerValue_end = rpiSystemTimer->counter_lo;
          printf("Vectrex time in Seconds: %f \n\r", ((float)(i+1)/50));    

          float t1 = timerValue_end-timerValue_start;
          float t2 = (1*1000*1000);

          printf("Timer in Seconds: %f \n\r", (t1)/(t2));    
          t1 = nanoCounter;
          t2 = (1*1000*1000*1000);
          printf("Cycles in Seconds: %f \n\r", (t1)/(t2));    
          printf("----\n\r");    
      }
  
  }
*/  
  
/*  
  unsigned int cc;
  unsigned int cc0;
  CCNT(cc);
  CCNT0(cc0);
  printf("CC: %i, CC0: %i\r\n", cc, cc0);  
  PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET);
  CCNT(cc);
  CCNT0(cc0);
  printf("REI_SET CC: %i, CC0: %i\r\n", cc, cc0);  
  PMNC(CYCLE_COUNTER_ENABLE|COUNTER_ZERO);
  CCNT(cc);
  CCNT0(cc0);
  printf("REI_SET0 CC: %i, CC0: %i\r\n", cc, cc0);  
*/  
  usbKeyboardAvailable = 0;
  useUSB = 0;
  

  useDoubleTimer=0;
  keepDotsTogether = 0;
  
  executeDebugger = noExecuteDebugger;
  checkExternal = 0;
  v_initSound();
  #ifdef PITREX_DEBUG
  v_initDebug();
  myDebug = 0;
  #endif

  DELAY_AFTER_XSH_VALUE = 2;
  DELAY_AFTER_YSH_VALUE = 8;
  DELAY_PORT_B_BEFORE_PORT_A_VALUE = 2;
  CRANKY_DELAY_Y_TO_NULL_VALUE=6;
  

  MAX_CONSECUTIVE_DRAWS = 65;
  DELAY_ZERO_VALUE = 40; // 70 // probably less, this can be adjusted, by max x position, the nearer to the center the less waits
  DELAY_AFTER_T1_END_VALUE = 20; // last vectors should add 5!!!
  SCALE_STRENGTH_DIF = 2;
  MINSCALE = 5;
  bufferType = 0;
  MAX_USED_STRENGTH = 100;// 110;
  resetToZeroDifMax = 4000;
  customClippingEnabled = 0;
  commonHints = 0;
  pipelineFilled = 0;
  v_dotDwell = 10;
  browseMode=0;
  currentBrowsline=0;
  currentDisplayedBrowseLine=-1;
  commonIntensity = 0x50;
  irqSound = 1;
  extendedIRQSound = 0;
  irqButtons = 1;
  irqJoyDigital = 0;
  irqJoyAnalog = 1+2+4+8;
  clientHz = 50;

  for (int i=0;i<MEDIAN_MAX;i++)
  {
    medianX1[i] = 0;
    medianY1[i] = 0;
  }

  crankyFlag=0x14;//CRANKY_NULLING_CALIBRATE | CRANKY_BETWEEN_VIA_B+6; // for is the normal cranky delay after switching port b to MUX_y (or Z)

  cycleEquivalent = 666;
  beamOffBetweenConsecutiveDraws = 1;

  clipActive = 0;
  clipminX=-10000;
  clipminY= 00000;
  clipmaxX= 10000;
  clipmaxY= 15000;
  clipMode = 1; // 0 normal clipping, 1 = inverse clipping

  
  ioDone = 0;
  orientation = 0;
  timerMark = 0;
  calibrationValue = 0;
  optimizationON = 1;

  usePipeline = 1;
  pipelineCounter = 0;
  basePipeLineWriting = 0;
  pb = _BPL[basePipeLineWriting];        // current base pipeline
  cpb = &pb[pipelineCounter];   // current base pipeline "element"

  vectorPipeLineIsRead = 1;
  vectorPipeLineNextRead = 2;
  vectorPipeLineWriting = 0;

  pv = _VPL[vectorPipeLineIsRead];        // current base pipeline
  pv[0].type = PL_END;
  pv = _VPL[vectorPipeLineNextRead];        // current base pipeline
  pv[0].type = PL_END;

  
  currentCursorX = 0; // 16 bit positioning value X
  currentCursorY = 0; // 16 bit positioning value Y
  currentButtonState = 0; // nothing pressed
  currentJoy1X = 0; // centered
  currentJoy1Y = 0; // centered
  currentJoy2X = 0; // centered
  currentJoy2Y = 0; // centered

  currentScale = 0; // currently active scale factor (T1 timer in VIA)
  lastScale = 1; // last active scale factor (T1 timer in VIA)
  consecutiveDraws = 0; // how many lines/moves were made after another!
  sizeX = 1.0;
  sizeY = 1.0;
  offsetX = 0;
  offsetY = 0;

  selectedCalibrationMenu = 0;
  selectionCalibrationMade = 0;


  // these are 16 bit to allow illegal values!
  currentPortA = 0x100; // == portA (also X SH)
  currentYSH = 0x100; // Y SH
  currentZSH = 0x100; // Z SH = brightness

  SET (VIA_DDR_b, 0x9F); // All Outputs VIA Port B except COMPARE input and PB6 at Cartridge Port
  SET (VIA_DDR_a, 0xFF); // All Outputs VIA Port A
  #ifdef BEAM_LIGHT_BY_SHIFT
  SET (VIA_aux_cntl, 0x98); //Shift Reg. Enabled, T1 PB7 Enabled
  #endif
  #ifdef BEAM_LIGHT_BY_CNTL
  SET (VIA_aux_cntl, 0x80); // Shift reg mode = 000 free disable, T1 PB7 enabled
  #endif

  Vec_Rfrsh = 0x3075;//0x3075;//12405; // 30000 = $7530 little endian = $3075 = 12405
  SETW (VIA_t2, Vec_Rfrsh); //Set T2 timer to 30000 cycles and start timer and reset IFlag

  v_readButtons(); // read buttons to check if we should enter calibration on init

  knownName = "";

  int pv = 0;
  if (pv = ini_parse("vectrexInterface.ini", iniHandler, 0) < 0) 
  {
        printf("vectrexInterface.ini not loaded!\n\r");
  }
  printf("vectrexInterface.ini-Parser returned: %i\n\r", pv);

  
}
void resetPipeline()
{
  usePipeline = 1;
  pipelineCounter = 0;
  basePipeLineWriting = 0;
  pb = _BPL[basePipeLineWriting];        // current base pipeline
  cpb = &pb[pipelineCounter];   // current base pipeline "element"

  vectorPipeLineIsRead = 1;
  vectorPipeLineNextRead = 2;
  vectorPipeLineWriting = 0;

  pv = _VPL[vectorPipeLineIsRead];        // current base pipeline
  pv[0].type = PL_END;
  pv = _VPL[vectorPipeLineNextRead];        // current base pipeline
  pv[0].type = PL_END;
}

/***********************************************************************/

/* Set current brightness of the Vectorbeam (provided BLANK is inactive).
 I f bit 7 is set, vectrex does not show an*y* brightness!
 It is checked if brightness is already set to the
 given value, if so - the function returns immediately.

 Ends without a delay, last set: VIA port B!
 */
void v_setBrightness(uint8_t brightness)
{
  commonIntensity = brightness;
  if (isIRQMode) 
  {
    return; // or add to pipeline?
  }
    
  
  if (brightness == currentZSH) return; // beware -> this might cause brightness drift to zero on completely same intensities all the time!
  if (brightness != currentPortA)
  {
    I_SET(VIA_port_a, brightness);
    currentPortA = (int8_t) brightness;
  }
  I_SET(VIA_port_b, 0x84); // MUX to intensity
  currentZSH = brightness;
  DELAY_ZSH();
  I_SET(VIA_port_b, 0x81); // I_SET port b to no Mux
}

/***********************************************************************/

/* Moves from the current "cursor" position to given offsets.
 O ffsets are given in 16bit and are downsc*a*led by
 "optimal" scale factors to vectrex strength/scale values.
 Wait is done using the Vectrex Timer T1.
 The wait is not "finished" - so other "pi" things can be done in between.
 After these "other pi things" are done "v_directDeltaMoveEnd()" must be
 called to insure T1 timer has expired.
 */
void v_directDeltaMove32start(int32_t _xLen, int32_t _yLen)
{
  int32_t xLen = _xLen*sizeX;
  int32_t yLen = _yLen*sizeY;
  SET_OPTIMAL_SCALE(xLen, yLen);
  SET_YSH16(yLen);
  SET_XSH16(xLen);
  START_T1_TIMER();
  currentCursorX += xLen;
  currentCursorY += yLen;
}
// called from v_directDraw32, where sizing already occured!
void v_directDeltaMove32start_nosizing(int32_t xLen, int32_t yLen)
{
  SET_OPTIMAL_SCALE(xLen, yLen);
  SET_YSH16(yLen);
  SET_XSH16(xLen);
  START_T1_TIMER();
  currentCursorX += xLen;
  currentCursorY += yLen;
}
// called from v_directDraw32, where sizing already occured!
void v_directDeltaDraw32start_nosizing(int32_t xLen, int32_t yLen)
{
  SET_OPTIMAL_SCALE(xLen, yLen);
  SET_YSH16(yLen);
  SET_XSH16(xLen);
  START_T1_TIMER();
  SWITCH_BEAM_ON();
  WAIT_T1_END();
  SWITCH_BEAM_OFF();
  currentCursorX += xLen;
  currentCursorY += yLen;
}
/*
 M oves to the given position "absolut".     * *
 at the beginning the vectrex beam is reset to zero (center of screen).

 The difference between a sequence of
 v_zeroWait();
 v_directDeltaMove32start(_xLen, _yLen);

 and a call to v_directMove32(...) is, that
 the absolut position call respects (can respect!) the
 chosen offsetX/offsetY.
 Delta move do not (can not) respect the offsets!
 */
void v_directMove32(int32_t xEnd, int32_t yEnd)
{
  v_zeroWait();
  int32_t x = xEnd*sizeX+offsetX;
  int32_t y = yEnd*sizeY+offsetY;

  UNZERO(); // ensure vector beam can be moves
  SET_OPTIMAL_SCALE(x, y);
  SET_YSH16(y);
  SET_XSH16(x);
  START_T1_TIMER();
  consecutiveDraws=0;
  currentCursorX = x;
  currentCursorY = y;
  WAIT_T1_END();
}

/***********************************************************************/

/*
 W aits till T1 interrupt flag of vectrex VI**A is set.
 Instead the Macro: WAIT_T1_END can be used!
 See also:
 v_directDeltaMove32start(int32_t xLen, int32_t yLen)
 */
void v_directDeltaMoveEnd()
{
  WAIT_T1_END();
}

/***********************************************************************/

/*      Draws to vectrex screen.
 All given coordinates are absolut sc*reen c*oordinates.
 Assume coordinates are in the range: -32768 - +32767,
 assuming 0,0 is in the middle of the screen.
 "32" name tag denotes signed long range.

 This function acts "smart", it investigates given data (in regards to
 last used data) and determines:
 - whether a move/zeroing is necessary
 - translates data to "delta" data if appropriate
 - calculates the vectrex scale and strength
 - respects "OPTIMIZED" flag!

 Leaves non zeroed!
*/

int v_debug = 0;

void v_directDraw32HintedDebug(int32_t xStart, int32_t yStart, int32_t xEnd, int32_t yEnd, uint8_t brightness, int forced, char* debugInfo)
{
  cpb->force = forced;

  int i=0;
  if (debugInfo != 0)
  {
    for (;i<239; i++)
    {
      if (!(cpb->debug[i] = *debugInfo++)) break;
    }
  }
  cpb->debug[i] = (char)0;
  v_directDraw32(xStart, yStart, xEnd, yEnd, brightness);
}

void v_directDraw32Hinted(int32_t xStart, int32_t yStart, int32_t xEnd, int32_t yEnd, uint8_t brightness, int forced)
{
  cpb->force = forced;
  cpb->debug[0] = (char)0;
  v_directDraw32(xStart, yStart, xEnd, yEnd, brightness);
}

void cohen_sutherlandCustom(int32_t *x1, int32_t *y1,  int32_t *x2, int32_t *y2, int xmin, int ymin, int xmax, int ymax);

// only pipelined!
void v_directDraw32Patterned(int32_t xStart, int32_t yStart, int32_t xEnd, int32_t yEnd, uint8_t brightness, uint8_t pattern)
{
    cpb->pattern = pattern;
    v_directDraw32(xStart, yStart, xEnd, yEnd, brightness);
}

void v_addZeroRef()
{
  cpb->force |= PL_BASE_FORCE_ZERO;
}

void v_directDraw32(int32_t xStart, int32_t yStart, int32_t xEnd, int32_t yEnd, uint8_t brightness)
{
  if (brightness==0) return;
// ATTENTION -2 as brightness uses "commonIntensity" (last set value with v_setBrightness();
//  if (brightness == -2) brightness = commonIntensity;

  if (usePipeline)
  {
    cpb->xStart = xStart;
    cpb->yStart = yStart;
    cpb->xEnd = xEnd;
    cpb->yEnd = yEnd;
  }
//      printf("x0,y0,x1,y2: %i,%i,%i,%i\r\n",xStart,  yStart,  xEnd,  yEnd);

  
  
  if (customClippingEnabled)
  {
    cohen_sutherlandCustom(&xStart, &yStart,  &xEnd, &yEnd, customClipxMin, customClipyMin, customClipxMax, customClipyMax);
    if (xStart == 1000000) return; // vector completely out of bounds
  }
  xStart = xStart*sizeX+offsetX;
  yStart = yStart*sizeY+offsetY;
  xEnd = xEnd*sizeX+offsetX;
  yEnd = yEnd*sizeY+offsetY;
  
  if (orientation == 0) ;// normal
  else  if (orientation == 1)
  {
    int32_t xStart_t = xStart;
    int32_t xEnd_t = xEnd;
    xStart = yStart;
    yStart = -xStart_t;
    xEnd = yEnd;
    yEnd = -xEnd_t;
  }
  else  if (orientation == 2) 
  {
    xStart = -xStart;
    yStart = -yStart;
    xEnd = -xEnd;
    yEnd = -yEnd;
  }
  else  if (orientation == 3)
  {
    int32_t xStart_t = xStart;
    int32_t xEnd_t = xEnd;
    xStart = -yStart;
    yStart = xStart_t;
    xEnd = -yEnd;
    yEnd = xEnd_t;
  } 
  if (usePipeline)
  {
//    printf("DRAW: %i- ", usePipeline);//setWaitMin);
//    printf("");//setWaitMin);
    cpb->y0 = yStart & (~POSTION_MARGIN_AND);
    cpb->x0 = xStart & (~POSTION_MARGIN_AND);
    cpb->y1 = yEnd & (~POSTION_MARGIN_AND);
    cpb->x1 = xEnd & (~POSTION_MARGIN_AND);
#ifdef MY_DEBUG
    if (myDebug) 
    {
      printf("x0,y0,x1,y2: %i,%i,%i,%i\r\n",cpb->x0,  cpb->y0,  cpb->x1,  cpb->y1);
    }
#endif    



    cpb->intensity = brightness;
    cpb->force |= commonHints;

#ifdef USE_PERSISTENT_VECTORS  
    if ((cpb->force & PL_BASE_PERSISTENT_ENTITY) == PL_BASE_PERSISTENT_ENTITY)
      cpb->id = (id_counter++);
#endif  

    cpb->sms = MAX_USED_STRENGTH;
    if (optimizationON==0) cpb->force |= PL_BASE_FORCE_ZERO;
    if (cpb->force & PL_BASE_FORCE_USE_DOT_DWELL) cpb->timingForced = v_dotDwell;
    if (cpb->force & PL_BASE_FORCE_USE_FIX_SIZE) cpb->timingForced = currentScale;

    cpb = &pb[++pipelineCounter];
    cpb->pattern = 0;
    cpb->force = 0;
    cpb->debug[0] = 0;
    
    if (pipelineCounter>MAX_PIPELINE-10)
    {
      printf("DANGER!\n\r");
      printf("DANGER!\n\r");
      printf("DANGER!\n\r");
    }
    
    return;
  }

  v_setBrightness(brightness);
  UNZERO(); // ensure vector beam can be moves

  // this will be the delta values for drawing
  int32_t xDrawDif;
  int32_t yDrawDif;

  // how far away is the cursor from the position we want to start drawing?
  int32_t xMoveDif = xStart-currentCursorX;
  int32_t yMoveDif = yStart-currentCursorY;

  // if not optimized, we always reposition!
  // test if the position of the last end - and the current start differs by more than our set margin
  if (((ABS(xMoveDif) > POSITION_MARGIN) || (ABS(yMoveDif) > POSITION_MARGIN) ) || (optimizationON==0))
  {
    // determine whether to zero and reposition (rather than do a move from here)
    // three facters
    // - did we already have to many consecutive moves? If so drift will be large - and it is better to zero once in a while
    // - will the next scale be considerably different from the current one? - if so - better start anew
    // - is the new position very far away from the old, if so, it might even be faster to zero and reposition
    //   (possibly redundant to reason two)

    int resetPos = consecutiveDraws > MAX_CONSECUTIVE_DRAWS;
    resetPos += ABS((currentScale-GET_OPTIMAL_SCALE(xMoveDif, yMoveDif)) > 20);
    resetPos += ((ABS(xMoveDif)>7000) || (ABS(yMoveDif)>7000) );

    // if not optimized, we always reposition!
    resetPos += optimizationON==0;
    if (resetPos)
    {
      // reset to zero - and position absolut
      ZERO_AND_WAIT();
      UNZERO();
      
      // reposition beam
      v_directDeltaMove32start_nosizing(xStart, yStart);
      
      consecutiveDraws = 0;
      xDrawDif = xEnd-xStart;
      yDrawDif = yEnd-yStart;
      SET_OPTIMAL_SCALE(xDrawDif, yDrawDif);
      WAIT_T1_END_LAST();//v_directDeltaMoveEnd();

      //if (calibrationValue!=0)    v_resetIntegratorOffsets();

    }
    else
    {
      // position relative
      // reposition beam
      v_directDeltaMove32start_nosizing(xMoveDif, yMoveDif);
      consecutiveDraws++;
      xDrawDif = xEnd-xStart;
      yDrawDif = yEnd-yStart;
      SET_OPTIMAL_SCALE(xDrawDif, yDrawDif);
      WAIT_T1_END_LAST();//v_directDeltaMoveEnd();
    }
  }
  else
  {
    // no repositioning -> draw directly again!
    xDrawDif = xEnd-xStart;
    yDrawDif = yEnd-yStart;
    SET_OPTIMAL_SCALE(xDrawDif, yDrawDif);
  }
  SET_YSH16(yDrawDif);
  SET_XSH16(xDrawDif);
  START_T1_TIMER();
  SWITCH_BEAM_ON();
  consecutiveDraws++;
  currentCursorX = xEnd;
  currentCursorY = yEnd;

  WAIT_T1_END();
  SWITCH_BEAM_OFF();
}

/***********************************************************************/

/*      Draws "directly" (not buffered) to vectrex screen.
 D raws from the current position to t*he giv*en delta values.
 Assume delta values are in the range: -32768 - +32767,
 assuming 0,0 is in the middle of the screen.
 "32" name tag denotes signed long range.

 This function acts "smart", it investigates given data (in regards to
 last used data) and determines:
 - calculates the vectrex scale and strength

 Leaves non zeroed!
 */
void v_directDeltaDraw32(int32_t _xLen, int32_t _yLen, uint8_t brightness)
{
  int32_t xLen = _xLen*sizeX;
  int32_t yLen = _yLen*sizeY;


  v_setBrightness(brightness);
  SET_OPTIMAL_SCALE(xLen, yLen);

  SET_YSH16(yLen);
  SET_XSH16(xLen);
  SWITCH_BEAM_ON();
  START_T1_TIMER();
  currentCursorX += xLen;
  currentCursorY += yLen;

  // if we have enough space and a correct pi timer
  // than we could wait with a PI timer in parallel and
  // resume "earlier", and switch the beam off
  // at EXACTLY the moment the timer finishes
  // lookup wait table 0x00 - 0xff (scale) for 100% correct timer values?
  WAIT_T1_END();
  SWITCH_BEAM_OFF();
}

/***********************************************************************/

/* This routine resets the integrator offsets to zero. This might be neccessary once in a while.
 *
 * The "opposite" (apart from natural degradation of the offsets) is Kristof Tuts like calibration.
 * Which actively sets a integrator offset to a very small value to compensate vectrex "drift".
 */

inline static void v_resetIntegratorOffsets()
{
  I_SET (VIA_port_b, 0x81);
  DELAY_PORT_B_BEFORE_PORT_A();
  if (calibrationValue==0)
  {
    I_SET (VIA_port_a, 0x00);
    ADD_DELAY_CYCLES(4);
    // reset integrators
    I_SET (VIA_port_b, 0x82);    // mux=1, enable mux - integrator offset = 0
    ADD_DELAY_CYCLES(6);
    I_SET (VIA_port_b, 0x81);    // disable mux
  }
  else
  {
//    I_SET (VIA_port_b, 0x81);
//    DELAY_PORT_B_BEFORE_PORT_A();

    I_SET (VIA_port_a, 0);
    ADD_DELAY_CYCLES(6);
    I_SET (VIA_port_b, 0x82);
    ADD_DELAY_CYCLES(6);
    I_SET (VIA_port_b, 0x81);    // disable mux
    ADD_DELAY_CYCLES(6);
    
    
    I_SET (VIA_port_a, calibrationValue);
    ADD_DELAY_CYCLES(6);
    I_SET (VIA_port_b, 0x82);
    DELAY_PORT_B_BEFORE_PORT_A();
    I_SET (VIA_port_a, 0xff);
    ADD_DELAY_CYCLES(2);
//    I_SET (VIA_port_b, 0x81);
    
    waitUntilAndUse(setWaitMin*ONE_WAIT_UNIT - 200);
    vectrexwrite(VIA_port_b, 0x81);
    PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET);
    lastViaWrite = (0);
    setWaitMin = 0;
    
  }
  ADD_DELAY_CYCLES(4);
  currentPortA=0x100;// non regular value!
}

/* ***********************************************************************/

/* This routine moves the vector beam to the extrem edges of the screen and zeros afterwards
 * this prevents "vector collaps".
 * If the "application" uses vectors which are far from the center anyway... this is not needed.
 */
void v_deflok()
{
  ZERO_AND_WAIT();
  UNZERO();
  v_setScale(255);
  SET_YSH_IMMEDIATE_8(127);
  SET_XSH_IMMEDIATE_8(127);
  START_WAIT_T1();
  ZERO_AND_WAIT();
  UNZERO();
  SET_YSH_IMMEDIATE_8(-127);
  SET_XSH_IMMEDIATE_8(-127);
  START_WAIT_T1();
  ZERO_AND_WAIT();
}

/***********************************************************************/

/*      Set vectrex refresh rate for T2 "WaitRecal".
 P arameter in in Hz.                 *      *
 -> so 50 Hz -> results in  Vec_Rfrsh = 0x3075
 */
void v_setRefresh(int hz)
{
  int32_t cycles = 1500000 / hz; // Hz to vectrex cycles
  Vec_Rfrsh = ((cycles&0xff)*256) + ((cycles>>8)&0xff); // swap hi / lo bytes for little endian VIA
}

int v_getRefresh()
{
  return 1500000 / (((Vec_Rfrsh&0xff)*256) + ((Vec_Rfrsh>>8)&0xff));
}

/***********************************************************************/

/*      A vectrex BIOS like function that can be called "once per round".
 T his function waits till the Vectrex* Timer* T2 is finished (timing can be set
 using "v_setRefresh(int hz)".

 After the wait finishes, following things are further executed:
 - the Timer T2 will be reset to the set frequency.
 - v_deflok()
 - v_resetIntegratorOffsets()

 Leaves with scale set to 0xff.

 Resets ioDone (so that joystick reads are only done once per round).

 Also checks and reacts on PiTrex "button" sets (return to boot screen, calibrate etc)
 */
void handlePipeline();
void displayPipeline();
void v_WaitRecal_buffered(int buildBuffer);

// this is ONLY
// a WaitRecal - no
// additional testing, menu entereing measuremnts etc
void v_WaitRecal_direct()
{
  ioDone = 0;
  SWITCH_BEAM_OFF();
  CCNT0(roundCycles);
   // wait for Via T2 to expire -> 50Hz
  while ((GET(VIA_int_flags) & 0x20) == 0);
  
  // reset T2 VIA timer to 50Hz
  SETW (VIA_t2, Vec_Rfrsh);

    // at the start of each "round" 
  // reset pi counter 1
  PMNC(CYCLE_COUNTER_ENABLE|COUNTER_ZERO);

  v_deflok();
  v_resetIntegratorOffsets0();
  currentZSH = 0x100;
  consecutiveDraws = 0; 
}

void handleClient_WR();
void v_WaitRecal()
{
//  printf("iRQ = %i\r\n",isIRQMode);
  
  if (isIRQMode)
  {
    handleClient_WR();
    return;
  }
  
  
  #ifdef PITREX_DEBUG
  while (browseMode)
  {
    displayPipeline();

//printf("B");

    handleUARTInterface();
    // wait for Via T2 to expire
    while ((GET(VIA_int_flags) & 0x20) == 0)
    {;}

    // reset T2 VIA timer to 50Hz
    SETW (VIA_t2, Vec_Rfrsh);
    currentZSH = 0x100;
    v_deflok();
    v_resetIntegratorOffsets0();
    if (browseSkip) // one round onward
    {
      browseSkip = 0; 
      break;
    }
  }
  #endif

  if ((bufferType == 2) && (pipelineFilled))
  {
    if (pipelineCounter == 0)
    {
      v_WaitRecal_buffered(0);
      return;
    }
    v_WaitRecal_buffered(1);
    return;
  }
  v_WaitRecal_buffered(1);
  if (bufferType == 1)
  {
    v_WaitRecal_buffered(0);
  }
}
static int roundCounter =0;

// must not be called directly, when in IRQ Mode
void returnToPiTrex()
{
  if (allowReturnToLoader)
  {
    v_removeIRQHandling();
    printf("Restarting kernel...%08x %08x\r\n",(int)settings->loader, (int)*settings->loader);
    settings->loader();
  }
}

uint32_t soundStart;
uint32_t soundEnd;
uint32_t avgStart;
uint32_t avgEnd;
uint32_t inputStart;
uint32_t inputEnd;

uint32_t cpu1Start;
uint32_t cpu1End;
uint32_t cpu2Start;
uint32_t cpu2End;


void v_WaitRecal_buffered(int buildBuffer)
{
  // savety
  // within WaitRecal - neverchange the pipeline
  #ifdef PITREX_DEBUG
  uint16_t timeLeftCoding = ((Vec_Rfrsh&0xff)*256+(Vec_Rfrsh>>8)) - GET(VIA_t2_cnt_hi)*256;
  #endif
  
  uint32_t timeNonVectrexStart;
  uint32_t timeVectrexStart;
  uint32_t timePipelineStart;

  uint32_t timeNonVectrex;
  uint32_t timePipeline;
  uint32_t timeDisplay;
  
  uint32_t _now;
  CCNT0(_now);
  
  timePipelineStart = _now;
  timeNonVectrex = _now-timeNonVectrexStart;
  
  if (usePipeline)
  {
    v_deflok();
    if (bufferType != 3)
    {
      CCNT0(timePipelineStart);
      if (buildBuffer)
      {
        handlePipeline();
      }
      CCNT0(_now);
      timePipeline = _now - timePipelineStart;
      timeVectrexStart = _now;

      roundCounter++;
      v_resetIntegratorOffsets0();
      displayPipeline();

      CCNT0(_now);
      timeDisplay = _now - timeVectrexStart;
    }
  }
  
  ioDone = 0;
  SWITCH_BEAM_OFF();


  #ifdef PITREX_DEBUG

  // in vectrex cycles:
  timeNonVectrex = timeNonVectrex/666; 
  timePipeline = timePipeline/666;
  timeDisplay = timeDisplay/666;
  
  uint32_t timeAll = timeNonVectrex+timePipeline+timeDisplay; // minus deflok ...
  


//  uint16_t timeLeftPrinting = ((Vec_Rfrsh&0xff)*256+(Vec_Rfrsh>>8)) - GET(VIA_t2_cnt_hi)*256;
//  int usagePercentAll  =(timeLeftPrinting*100/((Vec_Rfrsh&0xff)*256+(Vec_Rfrsh>>8)));

//  int usageCoding  =(timeLeftCoding*100/((Vec_Rfrsh&0xff)*256+(Vec_Rfrsh>>8)));
//  int usageDrawing  =usagePercentAll-usageCoding;
  if (currentButtonState ==0xc) // button 3 + 4 pressed - print speed info
  {
//    printf("Refresh usage: %i (Code: %i Draw: %i, all: %i) \r\n", usagePercentAll, usageCoding, usageDrawing , roundCycles/666);

    soundStart = (soundEnd-soundStart)/666;
    avgStart = (avgEnd-avgStart)/666;
    inputStart = (inputEnd-inputStart)/666;

    cpu1Start = (cpu1End-cpu1Start)/666;
    cpu2Start = (cpu2End-cpu2Start)/666;
    printf("Refresh usage (new): %i (Code: %i, pipeline: %i:%i, Display: %i) \r\n", timeAll,timeNonVectrex, lastPLSize,timePipeline, timeDisplay );
    printf("sound: %i, avg: %i, input: %i, cpu1: %i, cpu2:%i  \r\n", soundStart, avgStart, inputStart, cpu1Start, cpu2Start );

    
  }
  handleUARTInterface();
  if (isIRQMode) 
  {
    // switching to IRQ mode via command line
    // reset T2 VIA timer to 50Hz
    SETW (VIA_t2, Vec_Rfrsh);
    resetPipeline();
    return;
  }
  #endif



  v_resetDetection();
  if (currentButtonState ==0xf) // button 1+ 2 + 3+4 -> go menu
  {
    returnToPiTrex();
  }
  if ((currentButtonState&0xff)== 0x20) // button 2 -> joypad 2
  {
    if (inGameSettingsAllowed)
    {
      inCalibration = 1;
      
      if (wasIRQMode == 255)
      {
        wasIRQMode = isIRQMode;
        if (wasIRQMode==1)
        {
          v_removeIRQHandling();  
        }
      }
    }
  }

  
  if (inCalibration)
  {
    v_SettingsGUI(0);
  }
  CCNT0(roundCycles);
  
   // wait for Via T2 to expire -> 50Hz
  while ((GET(VIA_int_flags) & 0x20) == 0)
  {
    v_resetDetection();
    ; // might do a usleep(1); here
  }
  
  // reset T2 VIA timer to 50Hz
  SETW (VIA_t2, Vec_Rfrsh);

    // at the start of each "round" 
  // reset pi counter 1
  PMNC(CYCLE_COUNTER_ENABLE|COUNTER_ZERO);

  if (!usePipeline)
  {
    v_deflok();
    v_resetIntegratorOffsets0();
  }
  CCNT0(timeNonVectrexStart);
  
  
  currentZSH = 0x100;
  consecutiveDraws = 0; 
}

/***********************************************************************/
// detect VIA reset
// TODO
//-> this might work, if we don't use T1, but our own PI timer
// which atm we do...
void v_resetDetection()
{
  if ((GET(VIA_DDR_a) == 0) && (GET(VIA_DDR_b) == 0))
  {
    printf("Reset detected!\r\n");
    // wait till reset is finished
    while ((GET(VIA_DDR_a) == 0) && (GET(VIA_DDR_b) == 0))
    {
      SET(VIA_DDR_a, 0xff);
      ADD_DELAY_CYCLES(4);
    }
    settings->loader();
  }
}


/***********************************************************************/

/*      Reads the vectrex button states (port 1 and port 2).
 N eeds only be called once per round *(or ev*en only every second round)
 result also in variable: currentButtonState
 bit 0 = button 1 port 1
 bit 1 = button 2 port 1
 bit 2 = button 3 port 1
 bit 3 = button 4 port 1
 bit 4 = button 1 port 2
 bit 5 = button 2 port 2
 bit 6 = button 3 port 2
 bit 7 = button 4 port 2
 if bit == 1, than button is pressed
   if bit == 0, than button is not pressed
     */
uint8_t v_directReadButtons()
{
  int ibs=0;

//  ADD_DELAY_CYCLES(4);
//  SET(VIA_DDR_a, 0xff); // set VIA DDR A to output
//  ADD_DELAY_CYCLES(4);
//  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
//  ADD_DELAY_CYCLES(6);

  I_SET(VIA_port_a, 0x0e); // prepare access of psg port A (0x0e) by writing the register value to VIA port A
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x99); // set VIA port B to settings: sound BDIR on, BC1 on, mux off (latch Address to PSG)
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  ADD_DELAY_CYCLES(4);

  SET(VIA_DDR_a, 0x00); // set VIA DDR A to input
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x89); // set VIA port B to settings: sound BDIR on, BC1 on, mux off (Read from PSG)
  ADD_DELAY_CYCLES(6);
  ibs = ~I_GET(VIA_port_a); // Read buttons
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  ADD_DELAY_CYCLES(4);
  SET(VIA_DDR_a, 0xff); // set VIA DDR A to output
  ADD_DELAY_CYCLES(4);
  currentPortA = 0x100;

  return ibs;
}

// only once changed per round
uint8_t v_readButtons()
{
  if (isIRQMode) return currentButtonState;
  if (ioDone & V_BUTTONS_READ) return currentButtonState;

  ioDone |= V_BUTTONS_READ;
  // read of buttons goes thru the PSG sound chip, PSG port A
  //
  I_SET(VIA_port_a, 0x0e); // prepare access of psg port A (0x0e) by writing the register value to VIA port A
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x99); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  ADD_DELAY_CYCLES(4);

  SET(VIA_DDR_a, 0x00); // set VIA DDR A to input
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x89); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
  ADD_DELAY_CYCLES(6);
  currentButtonState = ~I_GET(VIA_port_a); // Read buttons
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  ADD_DELAY_CYCLES(4);
  SET(VIA_DDR_a, 0xff); // set VIA DDR A to output
  ADD_DELAY_CYCLES(4);
  currentPortA = 0x100;

  internalButtonState = currentButtonState;
  if (inCalibration)
  {
    currentButtonState = 0;
  }
  return currentButtonState;
}

/***********************************************************************/

/*      Reads the vectrex joystick states (port 1) DIGITAL.
 ( Faster than analog call!)          *      *
 Needs only be called once per round (or even only every second round)
 result in variable:
 currentJoy1Y = -1 down
 currentJoy1Y = 0 centered
 currentJoy1Y = 1 up

 currentJoy1X = -1 left
 currentJoy1X = 0 centered
 currentJoy1X = 1 right
 */
void v_readJoystick1Digital()
{
  if (isIRQMode) return;
  if (ioDone & V_JOY_DIGITAL_READ) return;
  ioDone |= V_JOY_DIGITAL_READ;

  // Y ANALOG
  I_SET(VIA_port_a, 0x00); // clear VIA port A
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x82); // set VIA port B mux enabled, mux sel = 01 (vertical pot port 0)

  // wait for joystick comparators to "settle"
  ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

  currentJoy1Y = -1; // default down
  I_SET(VIA_port_b, 0x83); // set VIA port B mux
  DELAY_PORT_B_BEFORE_PORT_A();
  I_SET(VIA_port_a, 0x40); // load a with test value (positive y), test value to DAC
  ADD_DELAY_CYCLES(4);
  if ((I_GET(VIA_port_b) & 0x20) == 0x20)
  {
    currentJoy1Y = 1; //up
  }
  else
  {
    I_SET(VIA_port_a, -0x40); // load a with test value (negative y), test value to DAC
    ADD_DELAY_CYCLES(4);
    if ((I_GET(VIA_port_b) & 0x20) == 0x20)
    {
      currentJoy1Y = 0; // no direction
    }
  }

  // X ANALOG
  I_SET(VIA_port_a, 0x00); // clear VIA port A
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x80); // set VIA port B mux enabled, mux sel = 00 (horizontal pot port 0)
  // wait for joystick comparators to "settle"
  ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

  currentJoy1X = -1; // default left
  I_SET(VIA_port_b, 0x83); // set VIA port B mux
  DELAY_PORT_B_BEFORE_PORT_A();
  I_SET(VIA_port_a, 0x40); // load a with test value (positive y), test value to DAC
  ADD_DELAY_CYCLES(2);
  if ((I_GET(VIA_port_b) & 0x20) == 0x20)
  {
    currentJoy1X = 1; //right
  }
  else
  {
    ADD_DELAY_CYCLES(4);
    I_SET(VIA_port_a, -0x40); // load a with test value (negative y), test value to DAC
    ADD_DELAY_CYCLES(4);
    if ((I_GET(VIA_port_b) & 0x20) == 0x20)
    {
      currentJoy1X = 0; // no direction
    }
    ADD_DELAY_CYCLES(4);
  }
  // if joyport 2  needed....


  // Y ANALOG
  I_SET(VIA_port_a, 0x00); // clear VIA port A
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x82+0x04); // set VIA port B mux enabled, mux sel = 03 (vertical pot port 1)

  // wait for joystick comparators to "settle"
  ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

  currentJoy2Y = -1; // default down
  I_SET(VIA_port_b, 0x83); // set VIA port B mux
  DELAY_PORT_B_BEFORE_PORT_A();
  I_SET(VIA_port_a, 0x40); // load a with test value (positive y), test value to DAC
  ADD_DELAY_CYCLES(4);
  if ((I_GET(VIA_port_b) & 0x20) == 0x20)
  {
    currentJoy2Y = 1; //up
  }
  else
  {
    I_SET(VIA_port_a, -0x40); // load a with test value (negative y), test value to DAC
    ADD_DELAY_CYCLES(4);
    if ((I_GET(VIA_port_b) & 0x20) == 0x20)
    {
      currentJoy2Y = 0; // no direction
    }
  }

  // X ANALOG
  I_SET(VIA_port_a, 0x00); // clear VIA port A
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x84); // set VIA port B mux enabled, mux sel = 2 (horizontal pot port 1)
  // wait for joystick comparators to "settle"
  ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

  currentJoy2X = -1; // default left
  I_SET(VIA_port_b, 0x83); // set VIA port B mux
  DELAY_PORT_B_BEFORE_PORT_A();
  I_SET(VIA_port_a, 0x40); // load a with test value (positive y), test value to DAC
  ADD_DELAY_CYCLES(2);
  if ((I_GET(VIA_port_b) & 0x20) == 0x20)
  {
    currentJoy2X = 1; //right
  }
  else
  {
    ADD_DELAY_CYCLES(4);
    I_SET(VIA_port_a, -0x40); // load a with test value (negative y), test value to DAC
    ADD_DELAY_CYCLES(4);
    if ((I_GET(VIA_port_b) & 0x20) == 0x20)
    {
      currentJoy2X = 0; // no direction
    }
    ADD_DELAY_CYCLES(4);
  }

  // set port A reference value to unkown
  currentYSH = currentPortA=0x100; // reset saved current values to unkown state
  v_resetIntegratorOffsets0();
}

/***********************************************************************/

/*      Reads the vectrex joystick states (port 1) ANALOG.
 ( Slower than digital call!)         *      *
 Needs only be called once per round (or even only every second round)
 result in variable:
 currentJoy1Y = -1 - -127 down
 currentJoy1Y = 0 centered
 currentJoy1Y = +1 - +127 up

 currentJoy1X = -1 - -127  left
 currentJoy1X = 0 centered
 currentJoy1X = +1 - +127 right
 */



void v_readJoystick1Analog()
{
  if (isIRQMode) return;
  if (ioDone & V_JOY_ANALOG_READ) return;
  ioDone |= V_JOY_ANALOG_READ;

  I_SET(VIA_port_a, 0x00); // clear VIA port A
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x82); // set VIA port B mux enabled, mux sel = 01 (vertical pot port 0)
  // wait for joystick comparators to "settle"
  ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

  currentJoy1Y = 0; // default centered
  I_SET(VIA_port_b, 0x83); // set VIA port B mux disabled
  DELAY_PORT_B_BEFORE_PORT_A();
  uint8_t compareBit = 0x80;

  int8_t thisInternalJoy1Y = 0;
  do
  {
    I_SET(VIA_port_a, thisInternalJoy1Y); // load a with test value (positive y), test value to DAC
    ADD_DELAY_CYCLES(4);
    if ((I_GET(VIA_port_b) & 0x20) == 0)
    {
      thisInternalJoy1Y = thisInternalJoy1Y ^ compareBit;
    }
    ADD_DELAY_CYCLES(4);
    compareBit = compareBit>>1;
    thisInternalJoy1Y = thisInternalJoy1Y | compareBit;
  } while (compareBit!=0);

  I_SET(VIA_port_a, 0x00); // clear VIA port A
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x80); // set VIA port B mux enabled, mux sel = 01 (horizontal pot port 0)
  // wait for joystick comparators to "settle"
  ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

  currentJoy1X = 0; // default centered
  I_SET(VIA_port_b, 0x81); // set VIA port B mux disabled
  DELAY_PORT_B_BEFORE_PORT_A();
  compareBit = 0x80;

  
  int8_t thisInternalJoy1X = 0;
  do
  {
    I_SET(VIA_port_a, thisInternalJoy1X); // load a with test value (positive y), test value to DAC
    ADD_DELAY_CYCLES(4);
    if ((I_GET(VIA_port_b) & 0x20) == 0)
    {
      thisInternalJoy1X = thisInternalJoy1X ^ compareBit;
    }
    ADD_DELAY_CYCLES(4);
    compareBit = compareBit>>1;
    thisInternalJoy1X = thisInternalJoy1X | compareBit;
  } while (compareBit!=0);

  int difx = thisInternalJoy1X - internalJoy1X;
  int dify = thisInternalJoy1Y - internalJoy1Y;

  internalJoy1X = internalJoy1X+(difx>>1);
  internalJoy1Y = internalJoy1Y+(dify>>1);

  if (!inCalibration)
  {
    currentJoy1X = internalJoy1X;
    currentJoy1Y = internalJoy1Y;
  }

  // todo
  // if joyport 2  needed....

  I_SET(VIA_port_a, 0x00); // clear VIA port A
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x82+0x04); // set VIA port B mux enabled, mux sel = 03 (vertical pot port 1)
  // wait for joystick comparators to "settle"
  ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

  currentJoy2Y = 0; // default centered
  I_SET(VIA_port_b, 0x83); // set VIA port B mux disabled
  DELAY_PORT_B_BEFORE_PORT_A();
  compareBit = 0x80;

  do
  {
    I_SET(VIA_port_a, currentJoy2Y); // load a with test value (positive y), test value to DAC
    ADD_DELAY_CYCLES(4);
    if ((I_GET(VIA_port_b) & 0x20) == 0)
    {
      currentJoy2Y = currentJoy2Y ^ compareBit;
    }
    ADD_DELAY_CYCLES(4);
    compareBit = compareBit>>1;
    currentJoy2Y = currentJoy2Y | compareBit;
  } while (compareBit!=0);

  I_SET(VIA_port_a, 0x00); // clear VIA port A
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x84); // set VIA port B mux enabled, mux sel = 2 (horizontal pot port 1)
  // wait for joystick comparators to "settle"
  ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

  currentJoy2X = 0; // default centered
  I_SET(VIA_port_b, 0x81); // set VIA port B mux disabled
  DELAY_PORT_B_BEFORE_PORT_A();
  compareBit = 0x80;

  do
  {
    I_SET(VIA_port_a, currentJoy2X); // load a with test value (positive y), test value to DAC
    ADD_DELAY_CYCLES(4);
    if ((I_GET(VIA_port_b) & 0x20) == 0)
    {
      currentJoy2X = currentJoy2X ^ compareBit;
    }
    ADD_DELAY_CYCLES(4);
    compareBit = compareBit>>1;
    currentJoy2X = currentJoy2X | compareBit;
  } while (compareBit!=0);


  // set port A reference value to unkown
  currentYSH = currentPortA=0x100; // reset saved current values to unkown state
  v_resetIntegratorOffsets0();
}

/***********************************************************************/

/*      Sets the Vectrex Zero state - and waits for zeroing to have full effect.
 Z eroing is NOT disabled upon end of *functi*on!
 */
void v_zeroWait()
{
  ZERO_AND_WAIT();
}

/***********************************************************************/

/*      Sets the current scale value absolut to given value.
 D oes NOT change VIA!                    *  *
 */
void v_setScale(uint16_t s)
{
  lastScale = currentScale;
  currentScale = s;
}

void v_setScaleForced(uint16_t s)
{
  lastScale = s+1;
  currentScale = s;
}

/***********************************************************************/

/*      Moves from current location to
 d elta given by parameters.          *      *
 - does not set any internal variables
 - does not change scale
 - waits till position is reached

 Caller must ensure zeroing and similar states are taken care of beforehand!
 */
void v_moveToImmediate8(int8_t xLen, int8_t yLen)
{
  SET_YSH_IMMEDIATE_8(yLen);
  SET_XSH_IMMEDIATE_8(xLen);
  START_T1_TIMER();
  WAIT_T1_END();
}

/***********************************************************************/

/*      Draws from current location to
 d elta given by parameters.          *      *
 - does not set any internal variables
 - does not change scale
 - waits till position is reached

 Caller must ensure zeroing and similar states are taken care of!
 */
void v_drawToImmediate8(int8_t xLen, int8_t yLen)
{
  SET_YSH_IMMEDIATE_8(yLen);
  SET_XSH_IMMEDIATE_8(xLen);
  SWITCH_BEAM_ON();
  START_T1_TIMER();
  WAIT_T1_END();
  SWITCH_BEAM_OFF();
}


/***********************************************************************/

//#include <baremetal/rpi-systimer.h>
//static rpi_sys_timer_t* rpiSystemTimer = (rpi_sys_timer_t*)RPI_SYSTIMER_BASE;

// only using low counters!
uint32_t v_millis()
{
  uint32_t micros= rpiSystemTimer->counter_lo;
  return micros/1000;
}

// only using low counters!
uint32_t v_micros()
{
  return rpiSystemTimer->counter_lo;
}

// blocking error message!
void v_error(char *message)
{
  printf("Vectrex/PiTrex bailing out - error:\r\n\t%s\r\n" , message);
  while (1)
  {
    v_WaitRecal();
    v_setBrightness(64);        /* set intensity of vector beam... */
    v_printStringRaster(-30, 80, "ERROR", 40, -4, 0);
    v_printStringRaster(-100, 0, message, 40, -4, 0);
    v_readButtons();
  }
}

void v_errori(char *message, int i)
{
  printf("Vectrex/PiTrex bailing out - error:\r\n\t%s (%i)\r\n" , message, i);
  char buf[16];
  itoa(i,buf,10);
  while (1)
  {
    v_WaitRecal();
    v_setBrightness(64);        /* set intensity of vector beam... */
    v_printStringRaster(-30, 80, "ERROR", 40, -4, 0);
    v_printStringRaster(-100, 0, message, 40, -4, 0);
    v_printStringRaster(-100, -20, buf, 40, -4, 0);
    v_readButtons();
  }
}

void strnucpy(char *to,char *from, int count)
{
    for (int i=0;i<count;i++)
    {
      *to = toupper(*from++);
      if (!*to++) break;
    }
}

// non blocking error message!
void v_message(char *message)
{
  while (1)
  {
    v_WaitRecal();
    v_setBrightness(64);        /* set intensity of vector beam... */
    v_printStringRaster(-30, 80, "ATTENTION!", 40, -4, 0);

    int len = strlen(message);
    int pointer = 0;
    int y=20;
    char buf[41];
    while (1)
    {
      if (len > pointer)
      {
          strnucpy(buf, message+pointer, 40);
          pointer+=40;
          buf[40]=0;
          v_printStringRaster(-110, y, buf, 40, -4, 0);
          y-=20;
      }
      else
        break;
    }

    v_printStringRaster(-30, -80, "(BUTTON 1+4)", 40, -4, 0);
    v_readButtons();
    if ((currentButtonState&0x0f) == (0x09)) break;
  }
  
  while (currentButtonState&0xff)
  {
    v_WaitRecal();
    v_readButtons();
  }
}


////////////////////////////////////////////////////////////////////////////
// PSG and samples -> all stuff sound
////////////////////////////////////////////////////////////////////////////
/*
 A ccessing PSG is "expensive" (to set* one r*egister 6/8 (write/read) VIA access are necessary!)!

 To "directly" access PSG registers use functions:
 void            v_writePSG(uint8_t reg, uint8_t data)
 uint8_t         v_readPSG(uint8_t reg)

 However since it is so "expansive" there is a buffer thru which all PSG acccess should be done:
 Therefor we use a buffer system, if all reads writes are done using the buffer functions
 void v_writePSG_buffered(uint8_t reg, uint8_t data)
 uint8_t v_readPSG_buffered(uint8_t reg)

 we create two significant improvements:
 a) all reads are done via the buffer (saves 8 VIA accesses)
 b) only data is written to the PSG, that is not the same as in the buffer already (and thus in the PSG)

 Even more however:
 Also a double buffer is implemented!

 This can be used if you
 e.g.
 - play a YM file
 - and want to "overlay" distinct channels with a sound effect

 The functions are:
 void v_writePSG_double_buffered(uint8_t reg, uint8_t data)
 uint8_t v_readPSG_double_buffered(uint8_t reg)
 void v_PSG_writeDoubleBuffer()

 The read/write functions are more or less the same as the "buffered" ones, but the data is not
 written to VIA when necessary but written to another (double) buffer.
 So you can "overwrite" PSG data in the order you intend, and not actually put the data to the PSG directly.

 When all PSG stuff is done in your "round" - than a call to "v_PSG_writeDoubleBuffer()" must be made, which
 transports the complete contents of double buffer to the PSG (with respect to already buffered data - meaning
 if the PSG already contains a value, that also is not overwritten!)

   -------------------

   In conclusion:
   - use the double buffer functions!
   - once per round write the double buffer!
   */

static unsigned int psgDoubleBuffer[16];
static unsigned int psgShadow[16];
static uint16_t ymLength;
static uint16_t ymPos;
static uint8_t *ymBuffer;
static int ymloop;
unsigned int sfx_status_1;
unsigned int sfx_status_2;
unsigned int sfx_status_3;
uint8_t *sfx_pointer_1;
uint8_t *sfx_pointer_2;
uint8_t *sfx_pointer_3;

uint8_t *sfx_pointer_1_org;
uint8_t *sfx_pointer_2_org;
uint8_t *sfx_pointer_3_org;
int sfx_priority1;
int sfx_priority2;
int sfx_priority3;

typedef struct {
    unsigned int *status;
    int *priority;
} SFX;

SFX ayfx[3] =
{
    {&sfx_status_1, &sfx_priority1},
    {&sfx_status_2, &sfx_priority2},
    {&sfx_status_3, &sfx_priority3}
};

// used to cut of thrust in asteroids
int v_addSFXForced(unsigned char *buffer, int channel, int loop)
{
    v_playSFXCont(buffer, channel, loop);
    *ayfx[channel].priority=0;
    return channel;
}

// allows the same effect to be played multiple times on// different channels
// return -1 on failure
// channel on success
int v_addSFX(unsigned char *buffer, int priority, int loop, int whenEqualPriorityPlayNew)
{
    int use = -1;
    int possibleUse1 = -1;
    int possibleUse2 = -1;
    for (int i=0;i<3;i++)
    {
        if (*ayfx[i].status == NOT_PLAYING)
        {
            use = i;
            break;
        }
        if (*ayfx[i].priority < priority)
        {
            possibleUse1 = i;
        }
        if (whenEqualPriorityPlayNew)
        {
            if (*ayfx[i].priority == priority)
            {
                possibleUse2 = i;
            }
        }
    }
    int reallyUse = -1;
    if (use != -1) reallyUse = use;
    else if (possibleUse1 != -1) reallyUse = possibleUse1;
    else if (possibleUse2 != -1) reallyUse = possibleUse2;
    if (reallyUse == -1) return -1;

    v_playSFXCont(buffer, reallyUse, loop);
    return reallyUse;
}

void v_noSound();

void v_writePSG_double_buffered(uint8_t reg, uint8_t data);
void v_writePSG_buffered(uint8_t reg, uint8_t data);
void v_writePSG(uint8_t reg, uint8_t data);
uint8_t v_readPSG_double_buffered(uint8_t reg);
uint8_t v_readPSG_buffered(uint8_t reg);
uint8_t v_readPSG(uint8_t reg);
void v_PSG_writeDoubleBuffer();

void v_initYM(uint8_t *b, uint16_t length, int l);
int v_playYM();
int play_sfx1();
int play_sfx2();
int play_sfx3();

void v_playDirectSampleAll(char *ymBufferLoad, int fsize, int rate);


// does play a complete sample on vectrex, does not return in between... nothing is displayed!
// sample rate is e.g. 15000
// one sample each 0,000066 seconds
// = 0,0666 milli seconds
// = each 66 micro seconds
void v_playDirectSampleAll(char *ymBufferLoad, int fsize, int rate)
{
  if (isIRQMode) return;
  int counter = 0;
  int microsToWait = (1000*1000) / rate;

  I_SET(VIA_port_a, 0);
  ADD_DELAY_CYCLES(4);

  I_SET(VIA_port_b, 0x86);
  ADD_DELAY_CYCLES(4);
  while (counter < fsize)
  {
    char b = *ymBufferLoad++;
    I_SET(VIA_port_a, b);
    int endTime = v_micros()+microsToWait;
    counter++;
    while (v_micros()<endTime) ;
  }
  I_SET(VIA_port_b, 0x81);
  ADD_DELAY_CYCLES(4);
}

// set PSG sound output to "none"
void v_noSound()
{
  v_writePSG_double_buffered(8,0); // volume 0
  v_writePSG_double_buffered(9,0);
  v_writePSG_double_buffered(10,0);
  v_writePSG_double_buffered(7,0x3f); // all channel off
  
  if (isIRQMode) return;
  v_writePSG_buffered(8,0); // volume 0
  v_writePSG_buffered(9,0);
  v_writePSG_buffered(10,0);
  v_writePSG_buffered(7,0x3f); // all channel off
}

void v_noSound_channel(int c)
{
  if (c == 0)
  {
    v_writePSG_buffered(8,0); // volume 0
    v_writePSG_buffered(7, psgShadow[7] | (1+8) ); // all channel off

    v_writePSG_double_buffered(8,0); // volume 0
    v_writePSG_double_buffered(7, psgDoubleBuffer[7] | (1+8) ); // all channel off
  }
  if (c == 1)
  {
    v_writePSG_buffered(9,0); // volume 0
    v_writePSG_buffered(7, psgShadow[7] | (2+16) ); // all channel off

    v_writePSG_double_buffered(9,0); // volume 0
    v_writePSG_double_buffered(7, psgDoubleBuffer[7] | (2+16) ); // all channel off
  }
  if (c == 2)
  {
    v_writePSG_buffered(10,0); // volume 0
    v_writePSG_buffered(7, psgShadow[7] | (4+32) ); // all channel off

    v_writePSG_double_buffered(10,0); // volume 0
    v_writePSG_double_buffered(7, psgDoubleBuffer[7] | (4+32) ); // all channel off
  }
}


void v_doSound()
{
  if (isIRQMode) return;
  v_PSG_writeDoubleBuffer();
}

void v_initSound()
{
  for (int i=0;i<16;i++)
  {
    psgShadow[i] = -1;
    psgDoubleBuffer[i] = 0;
  }
  sfx_status_1 = NOT_PLAYING;
  sfx_status_2 = NOT_PLAYING;
  sfx_status_3 = NOT_PLAYING;
  sfx_pointer_1 = (uint8_t *) 0;
  sfx_pointer_2 = (uint8_t *) 0;
  sfx_pointer_3 = (uint8_t *) 0;
  sfx_pointer_1_org = (uint8_t *) 0;
  sfx_pointer_2_org = (uint8_t *) 0;
  sfx_pointer_3_org = (uint8_t *) 0;
  sfx_priority1 =0;
  sfx_priority2 =0;
  sfx_priority3 =0;
  ymLength = 0;
  ymBuffer = 0;
  ymloop = 0;
  v_noSound();
}

/***********************************************************************/
uint8_t v_readPSG_double_buffered(uint8_t reg)
{
  return psgDoubleBuffer[reg];
}

void v_writePSG_double_buffered(uint8_t reg, uint8_t data)
{
  psgDoubleBuffer[reg] = data;
  if (reg == 13)
    psgShadow[reg] = data+1; // something different,I don't know why this is needed!
    
}

void v_PSG_writeDoubleBuffer()
{
  if (isIRQMode) return;
  for (int i=0;i<15;i++)
  {
    uint8_t data = psgDoubleBuffer[i];
    if (psgShadow[i] == data) continue;
    psgShadow[i] = data;

    I_SET(VIA_port_a, i); // prepare access of psg port A (0x0e) by writing the register value to VIA port A
    I_SET(VIA_port_b, 0x99); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
    I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)

    I_SET(VIA_port_a, data); // write data to port a of via -> and than to psg
    I_SET(VIA_port_b, 0x91); // set VIA port B to settings: sound BDIR on, BC1 on, mux off, write to PSG
    I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  }
  currentPortA = 0x100; // undefined
}

void v_writePSG_buffered(uint8_t reg, uint8_t data)
{
  if (isIRQMode) return;
  if (psgShadow[reg] == data) return;
  psgShadow[reg] = data;

  I_SET(VIA_port_a, reg); // prepare access of psg port A (0x0e) by writing the register value to VIA port A
  I_SET(VIA_port_b, 0x99); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)

  I_SET(VIA_port_a, data); // write data to port a of via -> and than to psg
  I_SET(VIA_port_b, 0x91); // set VIA port B to settings: sound BDIR on, BC1 on, mux off, write to PSG
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  currentPortA = 0x100; // undefined
}
uint8_t v_readPSG_buffered(uint8_t reg)
{
  return psgShadow[reg];
}

void v_writePSG(uint8_t reg, uint8_t data)
{
  if (isIRQMode) return;
  I_SET(VIA_port_a, reg); // prepare access of psg port A (0x0e) by writing the register value to VIA port A
  I_SET(VIA_port_b, 0x99); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)

  I_SET(VIA_port_a, data); // write data to port a of via -> and than to psg
  I_SET(VIA_port_b, 0x91); // set VIA port B to settings: sound BDIR on, BC1 on, mux off, write to PSG
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  currentPortA = 0x100; // undefined
}

/***********************************************************************/

uint8_t v_readPSG(uint8_t reg)
{
  if (isIRQMode) return 0;
  I_SET(VIA_port_a, reg); // prepare access of psg port A (0x0e) by writing the register value to VIA port A
  I_SET(VIA_port_b, 0x99); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)

  SET(VIA_DDR_a, 0x00); // set VIA DDR A to input
  I_SET(VIA_port_b, 0x89); // set VIA port B to settings: sound BDIR on, BC1 on, mux off, read from psg
  uint8_t directData = GET(VIA_port_a); // Read buttons
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  SET(VIA_DDR_a, 0xff); // set VIA DDR A to output
  currentPortA = 0x100; // undefined
  return directData;
}

void v_initYM(uint8_t *b, uint16_t length, int l)
{
  ymBuffer = b;
  ymloop = l;
  ymLength = length;
  ymPos = 0;
printf("YM file init to vectrex interface - 2\n\r");
}

void v_setymPos(int i)
{
  ymPos = i;
}

int v_playYM()
{
  if (ymBuffer==0) return 0;
  if (ymPos>=ymLength)
  {
    if (ymloop==1)
      ymPos = 0;
    else
    {
      ymBuffer = 0;
      return 0;
    }
  }
  uint8_t *currentPointer = ymBuffer+ymPos*14;
  ymPos++;
  for (int i=0; i<14; i++)
  {
    v_writePSG_double_buffered(i, *currentPointer);
    currentPointer++;
  }
  return 1;
}
int v_stopYM()
{
  ymLength = 0;
  ymBuffer = 0;
  ymloop = 0;
  v_noSound();
}
int v_isYMPlaying()
{
  return ymBuffer!=0;
}

// plays a SFX
// if the sfx is already playing... does nothing
void v_playSFXCont(unsigned char *buffer, int channel, int loop)
{
  if (channel == 0)
  {
    if (sfx_pointer_1_org == buffer) return;
    sfx_pointer_1_org = buffer;
    sfx_pointer_1 = buffer;
    if (loop) sfx_status_1 = PLAY_LOOP; else  sfx_status_1 = PLAY_END;
  }
  if (channel == 1)
  {
    if (sfx_pointer_2_org == buffer) return;
    sfx_pointer_2_org = buffer;
    sfx_pointer_2 = buffer;
    if (loop) sfx_status_2 = PLAY_LOOP; else  sfx_status_2 = PLAY_END;
  }
  if (channel == 2)
  {
    if (sfx_pointer_3_org == buffer) return;
    sfx_pointer_3_org = buffer;
    sfx_pointer_3 = buffer;
    if (loop) sfx_status_3 = PLAY_LOOP; else  sfx_status_3 = PLAY_END;
  }

}
// plays a SFX
// if the sfx is already playing... restart it
void v_playSFXStart(unsigned char *buffer, int channel, int loop)
{
 // printf("Play start %d, %p\n", channel +1, buffer);
  if (channel == 0)
  {
    sfx_pointer_1_org = buffer;
    sfx_pointer_1 = buffer;
    if (loop) sfx_status_1 = PLAY_LOOP; else  sfx_status_1 = PLAY_END;
 // printf("sfx_status_1 %d\n",sfx_status_1);
  }
  if (channel == 1)
  {
    sfx_pointer_2_org = buffer;
    sfx_pointer_2 = buffer;
    if (loop) sfx_status_2 = PLAY_LOOP; else  sfx_status_2 = PLAY_END;
  }
  if (channel == 2)
  {
    sfx_pointer_3_org = buffer;
    sfx_pointer_3 = buffer;
    if (loop) sfx_status_3 = PLAY_LOOP; else  sfx_status_3 = PLAY_END;
  }
}

void v_playSFXStop(unsigned char *buffer, int channel)
{
  if (channel == 0)
  {
    if (sfx_pointer_1_org != buffer) return;
    sfx_pointer_1_org = 0;
    sfx_status_1 = NOT_PLAYING;
    v_writePSG_double_buffered(0+8, 0);

    uint8_t enable = v_readPSG_double_buffered(7);
    // disable tone
    enable = enable | (1<<0); // channel 0
    // disable noise
    enable = enable | (1<<(3+0)); // channel 0
    v_writePSG_double_buffered(7, enable);
  }
  if (channel == 1)
  {
    if (sfx_pointer_2_org != buffer) return;
    sfx_pointer_2_org = 0;
    sfx_status_2 = NOT_PLAYING;
    v_writePSG_double_buffered(1+8, 0);
    uint8_t enable = v_readPSG_double_buffered(7);
    // disable tone
    enable = enable | (1<<1); // channel 0
    // disable noise
    enable = enable | (1<<(3+1)); // channel 0
    v_writePSG_double_buffered(7, enable);
  }
  if (channel == 2)
  {
    if (sfx_pointer_3_org != buffer) return;
    sfx_pointer_3_org = 0;
    sfx_status_3 = NOT_PLAYING;
    v_writePSG_double_buffered(2+8, 0);
    uint8_t enable = v_readPSG_double_buffered(7);
    // disable tone
    enable = enable | (1<<2); // channel 0
    // disable noise
    enable = enable | (1<<(3+2)); // channel 0
    v_writePSG_double_buffered(7, enable);
  }
}

#undef PSG_CHANNEL
#undef SFX_STATUS
#undef SFX_POINTER
#undef SFX_POINTER_ORG
#define PSG_CHANNEL 0
#define SFX_STATUS sfx_status_1
#define SFX_POINTER sfx_pointer_1
#define SFX_POINTER_ORG sfx_pointer_1_org
// return 0 on finish or nothing
int play_sfx1()
{
//  printf("Playing 1");
//  printf("2 sfx_status_1 %d\n",sfx_status_1);
  if (!SFX_STATUS) return 0;

  uint8_t b = *SFX_POINTER++;

  if (b == 0xd0)
  {
    if ((*SFX_POINTER) == 0x20)
    {
      if (SFX_STATUS == PLAY_LOOP)
      {
        SFX_POINTER = SFX_POINTER_ORG;
        b = *SFX_POINTER++;
      }
      else
      {
        SFX_POINTER_ORG = 0;
        SFX_STATUS = 0;
        v_writePSG_double_buffered(PSG_CHANNEL+8, 0);
        uint8_t enable = v_readPSG_double_buffered(7);
        enable = enable | (1<<PSG_CHANNEL); // channel 0
        enable = enable | (1<<(3+PSG_CHANNEL)); // channel 0
        v_writePSG_double_buffered(7, enable);
        return 0;
      }
    }
  }
  if ((b & (1 << 5)) == (1 << 5))
  {
    // tone frequency
    v_writePSG_double_buffered(PSG_CHANNEL*2+0, *SFX_POINTER++);
    v_writePSG_double_buffered(PSG_CHANNEL*2+1, *SFX_POINTER++);
  }
  if ((b & (1 << 6)) == (1 << 6))
  {
    // noise frequency
    v_writePSG_double_buffered(6, *SFX_POINTER++);
  }
  uint8_t volume = b&0xf;
  v_writePSG_double_buffered(PSG_CHANNEL+8, volume);
  uint8_t enable = v_readPSG_double_buffered(7);
  if ((b & (1 << 4)) == (1 << 4))
  {
    // disable tone
    enable = enable | (1<<PSG_CHANNEL); // channel 0
  }
  else
  {
    // enable tone
    enable = enable & (0xff - (1<<PSG_CHANNEL)); // channel 0
  }

  if ((b & (1 << 7)) == (1 << 7))
  {
    // disable noise
    enable = enable | (1<<(3+PSG_CHANNEL)); // channel 0
  }
  else
  {
    // enable tone
    enable = enable & (0xff - (1<<(3+PSG_CHANNEL))); // channel 0
  }
  v_writePSG_double_buffered(7, enable);
  return 1;
}


#undef PSG_CHANNEL
#undef SFX_STATUS
#undef SFX_POINTER
#undef SFX_POINTER_ORG
#define PSG_CHANNEL 1
#define SFX_STATUS sfx_status_2
#define SFX_POINTER sfx_pointer_2
#define SFX_POINTER_ORG sfx_pointer_2_org
// return 0 on finish or nothing
int play_sfx2()
{
  if (!SFX_STATUS) return 0;
//  printf("Playing 2\n");

  uint8_t b = *SFX_POINTER++;

  if (b == 0xd0)
  {
    if ((*SFX_POINTER) == 0x20)
    {
      if (SFX_STATUS == PLAY_LOOP)
      {
        SFX_POINTER = SFX_POINTER_ORG;
        b = *SFX_POINTER++;
      }
      else
      {
        SFX_POINTER_ORG = 0;
        SFX_STATUS = 0;
        v_writePSG_double_buffered(PSG_CHANNEL+8, 0);
        uint8_t enable = v_readPSG_double_buffered(7);
        enable = enable | (1<<PSG_CHANNEL); // channel 0
        enable = enable | (1<<(3+PSG_CHANNEL)); // channel 0
        v_writePSG_double_buffered(7, enable);
        return 0;
      }
    }
  }
  if ((b & (1 << 5)) == (1 << 5))
  {
    // tone frequency
    v_writePSG_double_buffered(PSG_CHANNEL*2+0, *SFX_POINTER++);
    v_writePSG_double_buffered(PSG_CHANNEL*2+1, *SFX_POINTER++);
  }
  if ((b & (1 << 6)) == (1 << 6))
  {
    // noise frequency
    v_writePSG_double_buffered(6, *SFX_POINTER++);
  }
  uint8_t volume = b&0xf;
  v_writePSG_double_buffered(PSG_CHANNEL+8, volume);
  uint8_t enable = v_readPSG_double_buffered(7);
  if ((b & (1 << 4)) == (1 << 4))
  {
    // disable tone
    enable = enable | (1<<PSG_CHANNEL); // channel 0
  }
  else
  {
    // enable tone
    enable = enable & (0xff - (1<<PSG_CHANNEL)); // channel 0
  }

  if ((b & (1 << 7)) == (1 << 7))
  {
    // disable noise
    enable = enable | (1<<(3+PSG_CHANNEL)); // channel 0
  }
  else
  {
    // enable tone
    enable = enable & (0xff - (1<<(3+PSG_CHANNEL))); // channel 0
  }
  v_writePSG_double_buffered(7, enable);
  return 1;
}



#undef PSG_CHANNEL
#undef SFX_STATUS
#undef SFX_POINTER
#undef SFX_POINTER_ORG
#define PSG_CHANNEL 2
#define SFX_STATUS sfx_status_3
#define SFX_POINTER sfx_pointer_3
#define SFX_POINTER_ORG sfx_pointer_3_org
// return 0 on finish or nothing
int play_sfx3()
{
  if (!SFX_STATUS) return 0;
//  printf("Playing 3\n");

  uint8_t b = *SFX_POINTER++;

  if (b == 0xd0)
  {
    if ((*SFX_POINTER) == 0x20)
    {
      if (SFX_STATUS == PLAY_LOOP)
      {
        SFX_POINTER = SFX_POINTER_ORG;
        b = *SFX_POINTER++;
      }
      else
      {
        SFX_POINTER_ORG = 0;
        SFX_STATUS = 0;
        v_writePSG_double_buffered(PSG_CHANNEL+8, 0);
        uint8_t enable = v_readPSG_double_buffered(7);
        enable = enable | (1<<PSG_CHANNEL); // channel 0
        enable = enable | (1<<(3+PSG_CHANNEL)); // channel 0
        v_writePSG_double_buffered(7, enable);
        return 0;
      }
    }
  }
  if ((b & (1 << 5)) == (1 << 5))
  {
    // tone frequency
    v_writePSG_double_buffered(PSG_CHANNEL*2+0, *SFX_POINTER++);
    v_writePSG_double_buffered(PSG_CHANNEL*2+1, *SFX_POINTER++);
  }
  if ((b & (1 << 6)) == (1 << 6))
  {
    // noise frequency
    v_writePSG_double_buffered(6, *SFX_POINTER++);
  }
  uint8_t volume = b&0xf;
  v_writePSG_double_buffered(PSG_CHANNEL+8, volume);
  uint8_t enable = v_readPSG_double_buffered(7);
  if ((b & (1 << 4)) == (1 << 4))
  {
    // disable tone
    enable = enable | (1<<PSG_CHANNEL); // channel 0
  }
  else
  {
    // enable tone
    enable = enable & (0xff - (1<<PSG_CHANNEL)); // channel 0
  }

  if ((b & (1 << 7)) == (1 << 7))
  {
    // disable noise
    enable = enable | (1<<(3+PSG_CHANNEL)); // channel 0
  }
  else
  {
    // enable tone
    enable = enable & (0xff - (1<<(3+PSG_CHANNEL))); // channel 0
  }
  v_writePSG_double_buffered(7, enable);
  return 1;
}
#undef PSG_CHANNEL 
#undef SFX_STATUS
#undef SFX_POINTER
#undef SFX_POINTER_ORG

void v_playAllSFX()
{
//  printf("PALY ALL\n\r");
  play_sfx1();
  play_sfx2();
  play_sfx3();
}

void handleVectorRequest(int type, int x, int y)
{
}
Command *userCommandList;
#ifdef PITREX_DEBUG
#include "commands.i"
#endif



// if zeroing is enabled -we can not moveor draw
// ensure it is off!


// if zeroing is enabled -we can not moveor draw
// ensure it is off!

// moving from the current position, without bothering about the beam illumination state
// zeroing off is assumed

/*
a) only large positioning moves
b) only where scale > sms
c) sss must be with small vectors/moves
*/


#define PL_CONTINUE_TO(_x, _y) \
do{ \
  if (cpb->force & PL_BASE_FORCE_USE_FIX_SIZE) \
  { \
    timingNow = cpb->timingForced; \
    if ((ABS((_y)/(timingNow+SCALE_STRENGTH_DIF))>127) || (ABS((_x)/(timingNow+SCALE_STRENGTH_DIF))>127)) \
    { /* we must use another scaling!!! -> OR build two vectors! */ \
      SET_OPTIMAL_SCALE_SMS(_x, _y,cpb->sms); \
      timingNow = currentScale; \
    } \
  } \
  else \
  { \
    SET_OPTIMAL_SCALE_SMS(_x, _y,cpb->sms); \
    timingNow = currentScale; \
  } \
  rampingNow = 1; \
  pl[fpc].yStart = (cpb->yStart); /* before offset and user scaling */\
  pl[fpc].xStart = (cpb->xStart); \
  pl[fpc].yEnd = (cpb->yEnd); /* before offset and user scaling */\
  pl[fpc].xEnd = (cpb->xEnd); \
  pl[fpc].y = ((_y)/(timingNow+SCALE_STRENGTH_DIF)); \
  pl[fpc].x = ((_x)/(timingNow+SCALE_STRENGTH_DIF)); \
  pl[fpc].y0 = (cpb->y0); \
  pl[fpc].x0 = (cpb->x0); \
  pl[fpc].yRest = _y-(pl[fpc].y*(timingNow+SCALE_STRENGTH_DIF)); \
  pl[fpc].xRest = _x-(pl[fpc].x*(timingNow+SCALE_STRENGTH_DIF)); \
  if (currentY == pl[fpc].y) \
  { \
  /* not need to set Y at all */  \
  } \
  else \
  { \
    pl[fpc].flags = pl[fpc].flags | PL_Y_MUST_BE_SET; \
    if (currentMUX != MUX_Y) \
      pl[fpc].flags = pl[fpc].flags | PL_MUX_Y_MUST_BE_SET; \
    currentMUX = MUX_Y; \
    /* we must set Y */ \
    if ((pl[fpc].y==0) && (currentY!=0)) \
    {  \
        if ((crankyFlag&CRANKY_NULLING_CALIBRATE) && (!calibDone)) \
        { \
            if (calibrationValue == 0) \
            { \
              pl[fpc].flags = pl[fpc].flags | PL_CALIBRATE_0 | PL_MUX_Y_MUST_BE_SET; /* because integrator reset, when y = 0 */  \
              currentA =0x0; \
              calibDone = 1;\
            } \
            else \
            { /* costs much time - perhaps we can leave that out? */ \
              if (pl[fpc].type != PL_MOVE) \
              {\
                pl[fpc].flags = pl[fpc].flags | PL_CALIBRATE | PL_MUX_Y_MUST_BE_SET;  /* because integrator reset, when y = 0 */  \
                currentA =0x100;\
                calibDone = 1;\
              } \
            } \
        } \
        else if (crankyFlag&CRANKY_NULLING_WAIT) \
        { \
            pl[fpc].flags = pl[fpc].flags | PL_Y_DELAY_TO_NULL; /* because integrator reset, when y = 0 */  \
        } \
    } \
    if (currentA == pl[fpc].y) \
    { \
      ;/* no need to load A */ \
    } \
    else \
    { \
      /* we must set VIA_a */ \
      currentA = pl[fpc].y; \
      pl[fpc].flags = pl[fpc].flags | PL_Y_A_MUST_BE_SET; \
    } \
    currentY = currentA; /* store A to Y */ \
  } \
  if (currentA != pl[fpc].x) \
  { \
    pl[fpc].flags = pl[fpc].flags | PL_X_A_MUST_BE_SET; \
  } \
  currentA = pl[fpc].x; \
  if (currentMUX != MUX_X) \
    pl[fpc].flags = pl[fpc].flags | PL_MUX_X_MUST_BE_SET; \
  currentMUX = MUX_X; \
  if ((t1_timingSet&0xff) == (timingNow&0xff)) \
    pl[fpc].flags = pl[fpc].flags | PL_T1_LO_EQUALS; \
  t1_timingSet=timingNow; \
} while (0)


#define INIT_NEXT_PIPELINE_ITEM \
  pl[fpc].this_timing = timingNow; \
  pl[fpc].last_timing = timingLast; \
  fpc++; \
  /*pl[fpc].base=0;*/ \
  pl[fpc].debug[0]=0; \
  pl[fpc].flags = 0; \
  if (lastMustFinish) \
    pl[fpc].flags = pl[fpc].flags | PL_LAST_MUST_FINISH; \
  if (rampingNow) \
    pl[fpc].flags = pl[fpc].flags | PL_LAST_IS_RAMPING;  \
  timingLast = timingNow; \
  timingNow = 0; \
  rampingLast = rampingNow;  \
  rampingNow = 0;



#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)

#define ADD_CLIPPED_VECTOR(_x0,_y0,_x1,_y1, baseVector) \
do { \
  cpb->x0 = _x0; \
  cpb->y0 = _y0; \
  cpb->x1 = _x1; \
  cpb->y1 = _y1; \
  cpb->intensity = baseVector->intensity; \
  cpb->force = baseVector->force | PL_BASE_FORCE_NOT_CLIPPED; \
  cpb->force &= ~PL_BASE_FORCE_EMPTY; \
  cpb->sms = MAX_USED_STRENGTH; \
  cpb = &pb[++pipelineCounter]; \
  cpb->debug[0] = 0; \
  cpb->pattern = 0; \
  cpb->force = 0; \
  } while (0)

#define ADD_PIPELINE(_x0,_y0,_x1,_y1, intens) \
do { \
  cpb->x0 = _x0; \
  cpb->y0 = _y0; \
  cpb->x1 = _x1; \
  cpb->y1 = _y1; \
  cpb->intensity = intens; \
  cpb->force = cpb->force | PL_BASE_FORCE_NOT_CLIPPED | PL_BASE_FORCE_STABLE; \
  cpb->sms = MAX_USED_STRENGTH; \
  cpb = &pb[++pipelineCounter]; \
  cpb->debug[0] = 0; \
  cpb->pattern = 0; \
  cpb->force = 0; \
  } while (0)


enum {TOP = 0x1, BOTTOM = 0x2, RIGHT = 0x4, LEFT = 0x8};
enum {FALSE, TRUE};
typedef unsigned int outcode;
outcode compute_outcode(int x, int y, int xmin, int ymin, int xmax, int ymax)
{
  outcode oc = 0;
  if (y > ymax)
    oc |= TOP;
  else if (y < ymin)
    oc |= BOTTOM;
  if (x > xmax)
    oc |= RIGHT;
  else if (x < xmin)
    oc |= LEFT;
  return oc;
}

// returns x1 = 1000000 on complete outside!
void cohen_sutherlandCustom(int32_t *x1, int32_t *y1,  int32_t *x2, int32_t *y2, int xmin, int ymin, int xmax, int ymax)
{
  int accept;
  int done;
  outcode outcode1, outcode2;
  accept = FALSE;
  done = FALSE;
  outcode1 = compute_outcode(*x1, *y1, xmin, ymin, xmax, ymax);
  outcode2 = compute_outcode(*x2, *y2, xmin, ymin, xmax, ymax);
  do
  {
    if (outcode1 == 0 && outcode2 == 0)
    {
      accept = TRUE;
      done = TRUE;
    }
    else if (outcode1 & outcode2)
    {
      done = TRUE;
    }
    else
    {
      int x, y;
      int outcode_ex = outcode1 ? outcode1 : outcode2;
      if (outcode_ex & TOP)
      {
        x = *x1 + (*x2 - *x1) * (ymax - *y1) / (*y2 - *y1);
        y = ymax;
      }
      else if (outcode_ex & BOTTOM)
      {
        x = *x1 + (*x2 - *x1) * (ymin - *y1) / (*y2 - *y1);
        y = ymin;
      }
      else if (outcode_ex & RIGHT)
      {
        y = *y1 + (*y2 - *y1) * (xmax - *x1) / (*x2 - *x1);
        x = xmax;
      }
      else
      {
        y = *y1 + (*y2 - *y1) * (xmin - *x1) / (*x2 - *x1);
        x = xmin;
      }
      if (outcode_ex == outcode1)
      {
        *x1 = x;
        *y1 = y;
        outcode1 = compute_outcode(*x1, *y1, xmin, ymin, xmax, ymax);
      }
      else
      {
        *x2 = x;
        *y2 = y;
        outcode2 = compute_outcode(*x2, *y2, xmin, ymin, xmax, ymax);
      }
    }
  } while (done == FALSE);
  if (accept == TRUE)
  {
    return;
  }
  *x1 = 1000000;
  return;
}

// reuses current baseline
void cohen_sutherland(VectorPipelineBase *baseVector, int xmin, int ymin, int xmax, int ymax)
{
  int x1 = baseVector->x0;
  int y1 = baseVector->y0;
  int x2 = baseVector->x1;
  int y2 = baseVector->y1;

  int accept;
  int done;
  outcode outcode1, outcode2;
  accept = FALSE;
  done = FALSE;
  outcode1 = compute_outcode(x1, y1, xmin, ymin, xmax, ymax);
  outcode2 = compute_outcode(x2, y2, xmin, ymin, xmax, ymax);
  do
  {
    if (outcode1 == 0 && outcode2 == 0)
    {
      accept = TRUE;
      done = TRUE;
    }
    else if (outcode1 & outcode2)
    {
      done = TRUE;
    }
    else
    {
      int x, y;
      int outcode_ex = outcode1 ? outcode1 : outcode2;
      if (outcode_ex & TOP)
      {
        x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
        y = ymax;
      }
      else if (outcode_ex & BOTTOM)
      {
        x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
        y = ymin;
      }
      else if (outcode_ex & RIGHT)
      {
        y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
        x = xmax;
      }
      else
      {
        y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
        x = xmin;
      }
      if (outcode_ex == outcode1)
      {
        x1 = x;
        y1 = y;
        outcode1 = compute_outcode(x1, y1, xmin, ymin, xmax, ymax);
      }
      else
      {
        x2 = x;
        y2 = y;
        outcode2 = compute_outcode(x2, y2, xmin, ymin, xmax, ymax);
      }
    }
  } while (done == FALSE);
  if (accept == TRUE)
  {
    baseVector->x0 =x1;
    baseVector->y0 =y1;
    baseVector->x1 =x2;
    baseVector->y1 =y2;
    return;
  }
  baseVector->force |= PL_BASE_FORCE_EMPTY;
  return;
}

// https://stackoverflow.com/questions/47884592/how-to-reverse-cohen-sutherland-algorithm
// adds new baselines
// invalidates old baseline
void reverse_cohen_sutherland(VectorPipelineBase *baseVector, int xmin, int ymin, int xmax, int ymax)
{
  int x1 = baseVector->x0;
  int y1 = baseVector->y0;
  int x2 = baseVector->x1;
  int y2 = baseVector->y1;
  baseVector->force |= PL_BASE_FORCE_EMPTY;

  int accept;
  int done;
  outcode outcode1, outcode2;
  accept = FALSE;
  done = FALSE;
  outcode1 = compute_outcode(x1, y1, xmin, ymin, xmax, ymax);
  outcode2 = compute_outcode(x2, y2, xmin, ymin, xmax, ymax);
  do
  {
    if (outcode1 == 0 && outcode2 == 0)
    {
      done = TRUE;
    }
    else if (outcode1 & outcode2)
    {
      accept = TRUE;
      done = TRUE;
    }
    else
    {
      int x, y;
      int outcode_ex = outcode1 ? outcode1 : outcode2;

      if (outcode_ex & TOP)
      {
        x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
        y = ymax;
      }
      else if (outcode_ex & BOTTOM)
      {
        x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
        y = ymin;
      }
      else if (outcode_ex & RIGHT)
      {
        y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
        x = xmax;
      }
      else
      {
        y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
        x = xmin;
      }

      if (outcode_ex == outcode1)
      {
        ADD_CLIPPED_VECTOR(x1,y1,x,y,baseVector);
        x1 = x;
        y1 = y;
        outcode1 = compute_outcode(x1, y1, xmin, ymin, xmax, ymax);
      }
      else
      {
        ADD_CLIPPED_VECTOR(x,y,x2,y2,baseVector);
        x2 = x;
        y2 = y;
        outcode2 = compute_outcode(x2, y2, xmin, ymin, xmax, ymax);
      }
    }
  } while (done == FALSE);
  if (accept == TRUE)
  {
    ADD_CLIPPED_VECTOR(x1,y1,x2,y2,baseVector);
    return;
  }
  return;
}
/*
VectorPipelineBase *getUnusedVectorPipelineBase()
{
  VectorPipelineBase *newObject = cpb;
  cpb = &pb[++pipelineCounter]; 
  cpb->debug[0] = 0; 
  cpb->pattern = 0; 
  cpb->force = 0; 
  return newObject;
}
*/

void v_setHardClipping(int enabled, int mode, int respectOrientation, int x0, int y0, int x1, int y1)
{
  clipActive = enabled;
  clipMode = mode; // 0 normal clipping, 1 = inverse clipping

  if (respectOrientation)
  {
    x0= x0*sizeX;
    y0= y0*sizeY;
    x1= x1*sizeX;
    y1= y1*sizeY;
    
    if (orientation == 0) ;// normal
    else  if (orientation == 1)
    {
      int32_t x0_t = x0;
      int32_t x1_t = x1;
      x0 = y0;
      y0 = -x0_t;
      x1 = y1;
      y1 = -x1_t;
    }
    else  if (orientation == 2) 
    {
      x0 = -x0;
      y0 = -y0;
      x1 = -x1;
      y1 = -y1;
    }
    else  if (orientation == 3)
    {
      int32_t x0_t = x0;
      int32_t x1_t = x1;
      x0 = -y0;
      y0 = x0_t;
      x1 = -y1;
      y1 = x1_t;
    }     
  }
  if (x0<x1)
  {
    clipminX= x0;
    clipmaxX= x1;
  }
  else
  {
    clipminX= x1;
    clipmaxX= x0;
  }
  if (y0<y1)
  {
    clipminY= y0;
    clipmaxY= y1;
  }
  else
  {
    clipminY= y1;
    clipmaxY= y0;
  }
}
// custom clippings
// are DIRECTLY
// in the coordinates given
// from caller!
// before repositioning, scaling, orientation ....
void v_setCustomClipping(int enabled, int x0, int y0, int x1, int y1)
{
  customClippingEnabled = enabled;

  customClipxMin = x0;
  customClipyMin = y0;
  customClipxMax = x1;
  customClipyMax = y1;

  
  if (x0<x1)
  {
    customClipxMin= x0;
    customClipxMax= x1;
  }
  else
  {
    customClipxMin= x1;
    customClipxMax= x0;
  }
  if (y0<y1)
  {
    customClipyMin= y0;
    customClipyMax= y1;
  }
  else
  {
    customClipyMin= y1;
    customClipyMax= y0;
  }
  
}
/*
void setInverseClipping(int enabled, int x0, int y0, int x1, int y1)
{
  customClippingEnabled = enabled;
  customClipxMin = x0;
  customClipyMin = y0;
  customClipxMax = x1;
  customClipyMax = y1;
}
*/
// this can be called from normal space!

// return 1 if a new vectorpipeline was created
// returns 0 is not
void handlePipeline()
{
  // build a list of all items
  // and put "dots" last
  // and stables first (hint)
  if ((pipelineCounter > 0) || (clearPipelineIssued))
  {
     clearPipelineIssued = 0;
     pipelineFilled = 1;
  }
  else
  {
    return;
  }
    
  // the array is converted to a double linked list
  // that way we can later easily manipulate the "array"
  VectorPipelineBase *head=(VectorPipelineBase *)0;
  VectorPipelineBase *element=(VectorPipelineBase *)0;

  // move dots to be done "last" - possibly without ZERO REF
  // in between
  // dots are often fast moving small objects
  // a) they desturb the integrators
  // b) exact position is not so important!
  //
  // last head is for "dots" -> for a short time an own linked list
  // which is later put at the end of the "normal" vectors
  VectorPipelineBase *lastHead=(VectorPipelineBase *)0;
  VectorPipelineBase *lastElement=(VectorPipelineBase *)0;

  VectorPipelineBase *firstHead=(VectorPipelineBase *)0;
  VectorPipelineBase *firstElement=(VectorPipelineBase *)0;

  // clipping is "costly"
  // and is done on the array
  // this way "reverse" clipping can more easily
  // add new vectors to the end, since
  // we still are in "addition" mode of the pipeline
  if (clipActive)
  {
    if (isIRQMode)
    {
      // add a "window"
      ADD_PIPELINE(clipminX, clipminY, clipminX, clipmaxY, 90);
      ADD_PIPELINE(clipminX, clipmaxY, clipmaxX, clipmaxY, 90);
      ADD_PIPELINE(clipmaxX, clipmaxY, clipmaxX, clipminY, 90);
      ADD_PIPELINE(clipmaxX, clipminY, clipminX, clipminY, 90);
    }
    else
    {
      //v_directDraw32(clipminX, clipminY, clipminX, clipmaxY, 90);

      v_setBrightness(90);
      ZERO_AND_WAIT();
      UNZERO();
      v_directDeltaMove32start_nosizing(clipminX, clipminY);
      v_directDeltaMoveEnd();
      v_directDeltaDraw32start_nosizing(clipminX-clipminX, clipmaxY-clipminY);

      //v_directDraw32(clipminX, clipmaxY, clipmaxX, clipmaxY, 90);
      v_directDeltaDraw32start_nosizing(clipmaxX-clipminX, clipmaxY-clipmaxY);
      //v_directDraw32(clipmaxX, clipmaxY, clipmaxX, clipminY, 90);
      v_directDeltaDraw32start_nosizing(clipmaxX-clipmaxX, clipminY-clipmaxY);
      //v_directDraw32(clipmaxX, clipminY, clipminX, clipminY, 90);
      v_directDeltaDraw32start_nosizing(clipminX-clipmaxX, clipminY-clipminY);
      ZERO_AND_WAIT();
    }
    
    VectorPipelineBase *c_cpb;
    for (int i=0; i<pipelineCounter; i++)
    {
      c_cpb = &pb[i];
      // we can "hint" not to clip
      // e.g. already inverse clipped and added reverse results!
      if (!(c_cpb->force & PL_BASE_FORCE_NOT_CLIPPED))
      {
        if (clipMode==0)
        {
          cohen_sutherland(c_cpb, clipminX, clipminY, clipmaxX, clipmaxY);
        }
        else if (clipMode==1)
        {
          reverse_cohen_sutherland(c_cpb, clipminX, clipminY, clipmaxX, clipmaxY);
        }
      }
    }
  }

  // now we build the linked list
  //
  // adding "stables" to the front
  // in case "stables" are sorted (which is likely)
  // backwards ensured, that stables are correctly ordered!
  // when we move them to the front!
  for (int i=0; i<pipelineCounter; i++)
  {
    cpb = &pb[i];

    // stables are added in front of the list head!
    if (cpb->force & PL_BASE_FORCE_STABLE)
    {
      if (firstHead == (VectorPipelineBase *)0)
      {
        firstHead = cpb;
        firstElement = cpb;
        firstElement->next = (VectorPipelineBase *)0;
        firstElement->previous = (VectorPipelineBase *)0;
      }
      else
      {
        cpb->next = (VectorPipelineBase *)0;
        cpb->previous = firstElement;
        firstElement->next = cpb;
        firstElement = cpb;
      }
      continue;
    }
    // MOVE dots to the back of the list!
    if (!keepDotsTogether)
    {
      if ((cpb->x0 == cpb->x1) && (cpb->y0 == cpb->y1))
      {
        // in some cases it might be ok
        // to not zero "stars"
        // for now - we let the algorythm decide
        // and do not force to NOT ZERO
        //      if (!(cpb->force & PL_BASE_FORCE_ZERO))
        //        cpb->force |= PL_BASE_FORCE_NO_ZERO; // not sure if that is always a good thing!
        if (lastHead == (VectorPipelineBase *)0)
        {
          lastHead = cpb;
          lastElement = cpb;
          lastElement->next = (VectorPipelineBase *)0;
          lastElement->previous = (VectorPipelineBase *)0;
        }
        else
        {
          cpb->previous = lastElement;
          lastElement->next = cpb;
          lastElement = cpb;
          lastElement->next = (VectorPipelineBase *)0;
        }
        continue;
      }
    }

    // everything not dot and not stable
    // is just added to the normal list
    if (head == (VectorPipelineBase *)0)
    {
      head = cpb;
      element = cpb;
      element->next = (VectorPipelineBase *)0;
      element->previous = (VectorPipelineBase *)0;
    }
    else
    {
      cpb->previous = element;
      element->next = cpb;
      element = cpb;
      element->next = (VectorPipelineBase *)0;
    }
  }
  if (lastHead != 0)
  {
    // add dots last...
    if (head == (VectorPipelineBase *)0)
    {
      head = lastHead;
    }
    if (element != (VectorPipelineBase *)0)
    {
      element->next = lastHead;
      lastHead->previous = element;
    }
  }
  if (firstHead != 0)
  {
    if (head != 0)
    {
      firstElement->next = head;
      head->previous = firstElement;
    }
    head = firstHead;
  }
  cpb = head;

  // todo
  // might sort for brightness
  // supposedly all same brightnesses will build a (or several) "clusters"

  // now we calculate our "optimal"
  // VectorPipeline
  // for that we keep track of a hell of a lot
  // single items
  // like position, and contents of registers...
  // starts with position 0,0 assumed
  int cx = 0;
  int cy = 0;

  int beamState = 0; // off
  int currentY = 0x100;  // value in reg A (illegal)
  int currentA = 0x100;  // value in reg A (illegal)
  int zeroActive = 1;
  int currentBrightness = 0x100; // invalid
  int timingNow = 0;
  int timingLast = 0;
  int t1_timingSet = 0xffff;
  int rampingNow = 0;
  int rampingLast;
  int currentMUX = 0x100;
  int lastMustFinish = 0;

  // before pipeline is called
  // WR does a zero calib!
  int calibDone = 0; // -1 = no, 0 = 0 offset, 1 calibration Tuts done
  #define  MUX_BRIGHTNESS 4
  #define  MUX_Y 0
  #define  MUX_X 1
  #define  MUX_UNKOWN 255


  // since we run in "normal" space
  // we MUST NOT change the vectorpipeline
  // that is used to currently output
  // to the vectrex (that is the vectorPipeLineReading one)
  
  VectorPipeline *pl = _VPL[vectorPipeLineWriting];        // vectorpipeline we write to NOW
  
  // (since we allways reuse the same array elements, we must ensure they are clean from start)
  int fpc = 0; // final pipeline counter
  pl[fpc].flags = 0;

  // not used anymore...
  consecutiveDraws = 0;
  // when clipping thats not true anymore - but who cares
  // clippin is special anyway
  
  
#ifdef MY_DEBUG
  if (myDebug)  printf("Base pipeline has %i items!\r\n", pipelineCounter);
#endif  
  while (cpb != (VectorPipelineBase *)0)
  {
    int cv = 0;
    if (checkExternal != 0)
    {
      cv = checkExternal(&cpb, &pl, &fpc, &cx, &cy);
      if (cv != 0)
      {
        if (cv == CV_SPECIAL_SKIP ) continue;
        if (cv == CV_SPECIAL_DEFLOK )
        {
#ifdef MY_DEBUG
          if (myDebug)  printf("Special DEFLOK!\r\n");
#endif    
          pl[fpc].type = PL_DEFLOK;
          calibDone = 0;
          timingNow = 0;
          rampingNow = 0;
          lastMustFinish = 0;
          currentMUX = MUX_X;
          currentA = 0x100;
          currentY=0x100;
          if (beamState)
          {
            pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
            beamState = 0;
          }
          zeroActive = 1;
          cx = 0;
          cy = 0;
          consecutiveDraws = 0;
          INIT_NEXT_PIPELINE_ITEM;
        }
        else 
        {}
      }
    }
    
    
    
    
    // invalidated element... do nothing
    if (cpb->force & PL_BASE_FORCE_EMPTY)
    {
#ifdef MY_DEBUG
      if (myDebug)  printf("Base EMPTY!\r\n");
#endif      
      // cleanup and next!
      cpb->force = 0;
      cpb = cpb->next;
      continue;
    }

    if (currentBrightness != cpb->intensity)
    {
#ifdef MY_DEBUG
      if (myDebug)  printf("Base BRIGHTNESS!\r\n");
#endif      
      pl[fpc].type = PL_SET_BRIGHTNESS;
      pl[fpc].intensity = cpb->intensity;
      currentBrightness = cpb->intensity;

      if (currentA != currentBrightness)
      {
        pl[fpc].flags = pl[fpc].flags | PL_I_A_MUST_BE_SET;
        currentA = currentBrightness;
      }

      // if we don't switch the beam off
      // while setting the brightness -
      // bright dots might be possible!
      if (beamState)
      {
        pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
        beamState = 0;
      }

      currentMUX = MUX_BRIGHTNESS;
      if (crankyFlag & CRANKY_BETWEEN_VIA_B)
      {
        currentA = 0;
        currentMUX = MUX_X;
      }
      
      
      
      timingNow = 0;
      rampingNow = 0;
      lastMustFinish = 0;
      INIT_NEXT_PIPELINE_ITEM;
    }
    
    
    if ((cpb->force & PL_BASE_FORCE_IS_RASTER_DISPLAY)|| (cpb->force & PL_BASE_FORCE_IS_BI_RASTER_DISPLAY))
    {
#ifdef MY_DEBUG
      if (myDebug)  printf("Base RASTER!\r\n");
#endif      
      pl[fpc].type = PL_RASTER_TEXT;
      if ((cpb->force & PL_BASE_FORCE_IS_BI_RASTER_DISPLAY) && (isIRQMode))
      {
        
        pl[fpc].type = PL_RASTER_TEXT_BI_DIRECTIONAL;
        pl[fpc].xRest = (int) (base_biRasterDown + strlen(&cpb->debug[0])*biRasterDownDriftModifier); // adaptive down size
        if (pl[fpc].xRest<0) pl[fpc].xRest = 0;
      }
      pl[fpc].x = cpb->x0; // 8 bit pos
      pl[fpc].y = cpb->y0;
      pl[fpc].y0 = cpb->y1; // height of font
      pl[fpc].x0 = cpb->x1; // width of font
      pl[fpc].pattern = cpb->pattern; // halfOffset
      pl[fpc].rlines = cpb->rlines; // raster font to use
      pl[fpc].yRest = ((uint8_t)cpb->sms); //movementScale;

      int ii=0;
      for (ii=0;ii<239; ii++)
      {
        if (cpb->debug[ii] == 0) break;
        pl[fpc].debug[ii] = cpb->debug[ii];
      }
      pl[fpc].debug[ii] = 0;


      // leaves with a "zero continuing"

      // is not really needed
      // since raster switches off the beam in itself
      // but here (since we are lazy and raster is an element I rather neglect) we
      // must switch it off twice, since
      // otherwise if previously drawn vector will not get its
      // T1off pause
      // lest I program a special if clause in the "cleanup phase of drawing
      // and I don't want todo that for stupid raster
      if (beamState)
      {
        pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
        beamState = 0;
      }
      
      
      zeroActive = 1;
      cx = 0;
      cy = 0;
      rampingNow = 0;
      timingNow = DELAY_ZERO_VALUE;
      lastMustFinish = 1;
      
      calibDone = 0;
      currentMUX = MUX_X;
      currentA = 0x100;
      currentY = 0;
      
      
      INIT_NEXT_PIPELINE_ITEM;
      // reset base -> ready to be reused next "round"
      cpb->force = 0;
      cpb->pattern = 0;
      cpb->debug[0] = 0;
      cpb = cpb->next;
      
      continue;
    }
    
    if (cpb->force & PL_BASE_FORCE_DEFLOK)
    {
#ifdef MY_DEBUG
      if (myDebug)  printf("Base DEFLOK!\r\n");
#endif      
      pl[fpc].type = PL_DEFLOK;
      timingNow = 0;
      rampingNow = 0;
      lastMustFinish = 0;
      currentMUX = MUX_X;
      currentA = 0x100;
      calibDone = 0;
      currentY = 0;
      if (beamState)
      {
        pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
        beamState = 0;
      }
      zeroActive = 1;
      cx = 0;
      cy = 0;
      consecutiveDraws = 0;
      INIT_NEXT_PIPELINE_ITEM;
      
      // reset base -> ready to be reused next "round"
      cpb->force = 0;
      cpb->pattern = 0;
      cpb->debug[0] = 0;
      cpb = cpb->next;
      
      continue;
    }

    // forcing zero prevents algorythm!
    if (((cpb->force & PL_BASE_FORCE_ZERO )== PL_BASE_FORCE_ZERO) && (!zeroActive))
    {
#ifdef MY_DEBUG
      if (myDebug)  printf("Base ZERO!\r\n");
#endif      
      pl[fpc].type = PL_ZERO;
      
      // todo in relation to current position?
      timingNow = DELAY_ZERO_VALUE;
      
      calibDone = 0;
      currentMUX = MUX_X;
      currentA = 0x100;
      currentY = 0;

      
      // todo
      // zero timer in relation to current position
      // the less far from zero -> the less time needed!
      rampingNow = 0;
      lastMustFinish = 1;
      if (beamState)
      {
        pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
        beamState = 0;
      }
      zeroActive = 1;
      cx = 0;
      cy = 0;
      consecutiveDraws = 0;
      INIT_NEXT_PIPELINE_ITEM;
    }
    else if (!(cpb->force & PL_BASE_FORCE_NO_ZERO )) // do not force the algorythm if we are not prepared to eventually ZERO
    {
      // VERY EASY algorythm
      // like the "old" one

      // how far away is the cursor from the position we want to start drawing?
      int32_t xMoveDif = cpb->x0-cx;
      int32_t yMoveDif = cpb->y0-cy;

      // test if the position of the last end - and the current start differs by more than our set margin
      if ((ABS(xMoveDif) > POSITION_MARGIN) || (ABS(yMoveDif) > POSITION_MARGIN) )
      {
        // not on the same position, so we either do a MOVE
        // or do a Zero

        // the only left over criteria at the moment is - how far away from the last cursor position
        // if too far away, we throw in a zero ref
        // old:
        //    consecutiveDraws > MAX_CONSECUTIVE_DRAWS;
        //    resetPos += ABS((currentScale-GET_OPTIMAL_SCALE(xMoveDif, yMoveDif)) > 20);
        int resetPos = ((ABS(xMoveDif)>resetToZeroDifMax) || (ABS(yMoveDif)>resetToZeroDifMax) );

        if ((resetPos) && (!zeroActive))
        {
#ifdef MY_DEBUG
          if (myDebug)  printf("Base CALC ZERO!\r\n");
#endif    
          // do a zeroing!
          // copy paste from above
          pl[fpc].type = PL_ZERO;
          timingNow = DELAY_ZERO_VALUE;

          // zero allways recalibs
          calibDone = 0;
          currentMUX = MUX_X;
          currentA = 0x100;
          currentY = 0;

          // todo
          // zero timer in relation to current position
          // the less far from zero -> the less time needed!
          rampingNow = 0;
          lastMustFinish = 1;
          if (beamState)
          {
            pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
            beamState = 0;
          }
          zeroActive = 1;
          cx = 0;
          cy = 0;
          consecutiveDraws = 0;
          INIT_NEXT_PIPELINE_ITEM;
        }
      }
    }

    // the result from above is either
    // a) we are not correctly positioned -> do a MOVE
    // b) we ARE correctly positioned -> do a DRAW
    // both are handled below
    /*****************************************************/
    if ((cpb->force & PL_BASE_FORCE_RESET_ZERO_REF ) && (!calibDone))
    {
#ifdef MY_DEBUG
        if (myDebug)  printf("Base PL_CALIBRATE_0!\r\n");
#endif  
        pl[fpc].flags = pl[fpc].flags | PL_CALIBRATE_0;
        currentA = 0x100;  // value in reg A (illegal)
        currentMUX = MUX_X;
        currentY = 0x100;
        calibDone = 0;
    }
    if ((cpb->force & PL_BASE_FORCE_CALIBRATE_INTEGRATORS ) && (!calibDone))
    {
#ifdef MY_DEBUG
        if (myDebug)  printf("Base PL_CALIBRATE!\r\n");
#endif  
        pl[fpc].flags = pl[fpc].flags | PL_CALIBRATE;
        currentA = 0x100;  // value in reg A (illegal)
        currentMUX = MUX_X;
        currentY = 0x100;
        calibDone = 1;
    }

    
    
    
    
    /************ HANDLE start position of Vector ************/
    // fill secondary "optimized" pipeline with values
    if ((cx != cpb->x0) || (cy != cpb->y0))
    {
#ifdef MY_DEBUG
      if (myDebug)  printf("Base MOVE!\r\n");
#endif      
      pl[fpc].type = PL_MOVE;

      // we nust move to start the new vector
      if (zeroActive)
      {
        zeroActive = 0;
        pl[fpc].flags = pl[fpc].flags | PL_DEACTIVATE_ZERO;
      }
      if (beamState)
      {
        beamState = 0;
        pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
      }

      if ((cx==0)&& (cy==0))
      {
        if ((calibrationValue != 0) && (calibDone == 1))
        {
          pl[fpc].flags  = pl[fpc].flags | PL_CALIBRATE_0;
          currentMUX = MUX_X;
          currentA = 0;
          calibDone = 0;
          currentY = 0;
        }
        PL_CONTINUE_TO((cpb->x0-cx), (cpb->y0-cy));
        cx = cpb->x0;
        cy = cpb->y0;
        lastMustFinish = 1;
        INIT_NEXT_PIPELINE_ITEM;

        if (useDoubleTimer)
        {
            if ((ABS(cpb->x0)>MAX_USED_STRENGTH) || (ABS(cpb->y0)>MAX_USED_STRENGTH))
            {
              pl[fpc].type = PL_MOVE;
              PL_CONTINUE_TO(pl[fpc-1].xRest, pl[fpc-1].yRest);
              lastMustFinish = 1;
              INIT_NEXT_PIPELINE_ITEM;
            }
        }
      }
      else
      {
        PL_CONTINUE_TO((cpb->x0-cx), (cpb->y0-cy));
        cx = cpb->x0;
        cy = cpb->y0;
        lastMustFinish = 1;
        INIT_NEXT_PIPELINE_ITEM;
      }
      consecutiveDraws++;
    }
    /*********************************************************/

    /************ HANDLE draw position of Vector ************/
    if ((cx != cpb->x1) || (cy != cpb->y1))
    {
      if ((calibrationValue != 0) && (calibDone != 1))
      {
        // during calibration, the beam MUST be off!
        if (beamState)
          pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
        beamState = 0;

        pl[fpc].flags  = pl[fpc].flags | PL_CALIBRATE;
        currentMUX = MUX_X;

        currentA = 0x100;

// only if cranky? - > no, always!
        currentY = 0x100;
        calibDone = 1;
      }

      pl[fpc].type = PL_DRAW;

      // we must move to start the new vector
      if (zeroActive)
      {
        zeroActive = 0;
        pl[fpc].flags = pl[fpc].flags | PL_DEACTIVATE_ZERO;
      }

      // before timer
      if (beamOffBetweenConsecutiveDraws)
      {
        if (beamState)
          pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
        beamState = 0;
      }
      // after timer
      if (!beamState)
      {
        beamState = 1;
        pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_ON;
      }
      
      //check last one, if also draw, than try to use the same T1
      
      PL_CONTINUE_TO(cpb->x1-cpb->x0, cpb->y1-cpb->y0);
      cx = cpb->x1;
      cy = cpb->y1;

      // remember the "base" for possible debug informations
      // todo:
      // while debugging - prevent baselist switching!
      //pl[fpc].base=cpb;
      
#ifdef MY_DEBUG
      if ((browseMode) || (myDebug))
      {
        for (int ii=0;ii<239; ii++)
        {
          pl[fpc].debug[ii] = cpb->debug[ii];
          if (cpb->debug[ii] ==0) break;
        }
      }
#endif     
      
      lastMustFinish = 1;

      if ((cpb->pattern != 0) && (cpb->pattern != 0xff))
      {
#ifdef MY_DEBUG
        if (myDebug)  printf("Base PATTERN!\r\n");
#endif  
        pl[fpc].type = PL_DRAW_PATTERN;
        pl[fpc].pattern = cpb->pattern;
        lastMustFinish = 0;
        rampingNow = 0;
      }
#ifdef MY_DEBUG
      else
      {
        if (myDebug)  printf("Base DRAW!\r\n");
      }
#endif
      INIT_NEXT_PIPELINE_ITEM;
      consecutiveDraws++;
    }
    else
    {
      // start and end coordinates are the same
      // do a "dot"
#ifdef MY_DEBUG
      if (myDebug)  printf("Base DOT!\r\n");
#endif
      // remember the "base" for possible debug informations
      // todo:
      // while debugging - prevent baselist switching!
#ifdef PITREX_DEBUG
      if ((browseMode) || (myDebug))
      {
        for (int ii=0;ii<239; ii++)
        {
          pl[fpc].debug[ii] = cpb->debug[ii];
          if (cpb->debug[ii] ==0) break;
        }
      }
#endif
      pl[fpc].type = PL_DRAW_DOT;
      if (!beamState)
      {
        beamState = 1;
        pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_ON;
      }
      
      
      // dot dwell?
      // todo dwell for this_timing?
      lastMustFinish = 1;
      cx = cpb->x1;
      cy = cpb->y1;
      rampingNow = 0;
      if (cpb->force & PL_BASE_FORCE_USE_DOT_DWELL)
      {
        timingNow = cpb->timingForced;
        pl[fpc].flags = pl[fpc].flags /*| PL_DEFLOK_AFTER */;

        pl[fpc].x = cx;
        pl[fpc].y = cy;
        zeroActive = 1;
        cx = 0;
        cy = 0;
        lastMustFinish = 1;
        INIT_NEXT_PIPELINE_ITEM;

/// REVERSE SHOT EFFECT!        
        {
          currentMUX = MUX_X;
          currentA = 0x100;
          calibDone = 1;
          zeroActive = 1;
          consecutiveDraws = 0;

          
          
          pl[fpc].type = PL_MOVE;
          pl[fpc].flags = pl[fpc].flags | PL_DEACTIVATE_ZERO;
          
          
          PL_CONTINUE_TO(-cpb->x1, -cpb->y1);
          cx = -cpb->x1;
          cy = -cpb->y1;
          lastMustFinish = 1;
          INIT_NEXT_PIPELINE_ITEM;

          pl[fpc].x = cx;
          pl[fpc].y = cy;
          pl[fpc].type = PL_DRAW_DOT;
          timingNow = cpb->timingForced;
          pl[fpc].flags = pl[fpc].flags /*| PL_DEFLOK_AFTER */;
          pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
          pl[fpc].flags = pl[fpc].flags | PL_DEFLOK_AFTER; // get rid of after DOT "spreading"
          
          lastMustFinish = 1;
          zeroActive = 1;
          cx = 0;
          cy = 0;

          currentMUX = MUX_X;
          currentY = 0x100;
          currentA = 0x100;
          calibDone = 1;
          zeroActive = 1;
          consecutiveDraws = 0;
        }
        
        
        
        
      }
      else
      {
        timingNow = v_dotDwell;
      }
      INIT_NEXT_PIPELINE_ITEM;

    }

    if (checkExternal != 0)
    {
      if (cv > 0x80)
      {
        if (cv == 0x80 + CV_SPECIAL_DEFLOK)
        {
#ifdef MY_DEBUG
          if (myDebug)  printf("Special DEFLOK AFTER!\r\n");
#endif    
          pl[fpc].type = PL_DEFLOK;
          timingNow = 0;
          rampingNow = 0;
          lastMustFinish = 0;
          currentMUX = MUX_X;
          currentA = 0x100;
          currentY=0x100;
          calibDone = 0;
          if (beamState)
          {
            pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF;
            beamState = 0;
          }
          zeroActive = 1;
          cx = 0;
          cy = 0;
          consecutiveDraws = 0;
          INIT_NEXT_PIPELINE_ITEM;
        }
      }
    }    
    
    /*********************************************************/

    // when calibrate?
    // when calibareted calibration
    // user commands to SYNC etc

    // reset base -> ready to be reused next "round"
    cpb->force = 0;
    cpb->pattern = 0;
    cpb->debug[0] = 0;
    cpb = cpb->next;
  }
  // clean up after last item of the list
  if (beamState)
    pl[fpc].flags = pl[fpc].flags | PL_SWITCH_BEAM_OFF_AFTER | PL_SWITCH_ZERO_AFTER | PL_SWITCH_BEAM_OFF;

  pl[fpc].type = PL_END;
  pl[fpc].last_timing = timingLast;
  lastPLSize = pipelineCounter;
  // setup next round
  pipelineCounter = 0;
  basePipeLineWriting = basePipeLineWriting?0:1;
  pb = _BPL[basePipeLineWriting]; // current base pipeline 
  cpb = &pb[0];
  
  // tell hyperspace
  // to next use the newly written pipeline
  // to output data with
  
//  !!!!
  if (isIRQMode) _disable_interrupts();




  vectorPipeLineNextRead = vectorPipeLineWriting;
  vectorPipeLineWriting = (vectorPipeLineWriting +1)%3;
  if (vectorPipeLineWriting == vectorPipeLineIsRead)
  {
    vectorPipeLineWriting = (vectorPipeLineWriting +1)%3;
  }

#ifdef USE_PERSISTENT_VECTORS  

  if (clearNextPipeline == 0)
  {
    VectorPipelineBase *old_pipeline =_BPL[(basePipeLineWriting?0:1)];
    for (int i=0; i<lastPLSize; i++)
    {
      VectorPipelineBase *o_cpb = &old_pipeline[i];
      if ((o_cpb->force & PL_BASE_PERSISTENT_ENTITY) == PL_BASE_PERSISTENT_ENTITY)
      {
        cpb->yStart = o_cpb->yStart;
        cpb->xStart = o_cpb->xStart;
        cpb->yEnd = o_cpb->yEnd;
        cpb->xEnd = o_cpb->xEnd;
        cpb->y0 = o_cpb->y0;
        cpb->x0 = o_cpb->x0;
        cpb->y1 = o_cpb->y1;
        cpb->y1 = o_cpb->x1;
        cpb->intensity = o_cpb->intensity;
        cpb->pattern = o_cpb->pattern;
        cpb->force = o_cpb->force;
        cpb->sms = o_cpb->sms;
        cpb->timingForced = o_cpb->timingForced;
        cpb->id = o_cpb->id;
        cpb->rlines = o_cpb->rlines;

        int ii=0;
        for (ii=0;ii<239; ii++)
        {
          cpb->debug[ii] = o_cpb->debug[ii];
          if (cpb->debug[ii] == 0) break;
        }

        
        
        // initnext
        cpb = &pb[++pipelineCounter];
        cpb->pattern = 0;
        cpb->force = 0;
        cpb->debug[0] = 0;
      }
    }
  }
  clearNextPipeline = 0;
#endif  
  
  
  
  
  
  if (isIRQMode) _enable_interrupts();
}




void displayPipeline()
{
//SCALE_STRENGTH_DIF=1;
//MINSCALE= 1;
  int c = 0;
  unsigned int t1OffMark = 0;
#define waitT1OffMark(waiter) \
do {\
  unsigned int value;\
  /* Read CCNT Register */ \
  do\
  {\
    CCNT0(value);\
    USE_TIME( (t1OffMark+((waiter)*ONE_WAIT_UNIT) ) -value);\
  } while (value < (t1OffMark+((waiter)*ONE_WAIT_UNIT) ) );\
} while (0)

  vectorPipeLineIsRead = vectorPipeLineNextRead;
  VectorPipeline *dpl = _VPL[vectorPipeLineIsRead];
  int delayedBeamOff=0;
#ifdef MY_DEBUG
  if (myDebug) printf("Display pipeline started...!\r\n");
  uint32_t startValue;
  if (myDebug)  
  {
    //PMNC(CYCLE_COUNTER_ENABLE|COUNTER_ZERO);
    CCNT0(startValue);
  }
  if (browseMode)
  {
    v_setBrightness(50);
  }
#endif



  int lineNo = 0;

  while (dpl[c].type != PL_END)
  {
    LINE_DEBUG_OUT("---- %i ----\r\n", lineNo);
#ifdef PITREX_DEBUG
    if (browseMode)
    {
        if (lineNo ==currentBrowsline)
        {
          v_setBrightness(127);
  I_SET (VIA_port_b, 0x81);
  DELAY_PORT_B_BEFORE_PORT_A();
  I_SET (VIA_port_a, 0x00);
  DELAY_YSH();
  DELAY_YSH();
  DELAY_YSH();
        }
        if (lineNo ==currentBrowsline+1)
        {
          v_setBrightness(50);
        }
    }
#endif
    switch (dpl[c].type)
    {
      case PL_DEFLOK:
      {
        LINE_DEBUG_OUT("PL DEFLOK\r\n");
        // loop ensures that there are no pending draws/moves
        if (dpl[c].flags & PL_SWITCH_BEAM_OFF) SWITCH_BEAM_OFF();
        v_deflok();
        LINE_DEBUG_OUT("PL DEFLOK CALIB \r\n");
        v_resetIntegratorOffsets0();
        break;
      }
      case PL_ZERO:
      {
        // waitfor last draw??? -> was done at the end of last draw
        ZERO_AND_CONTINUE();
        LINE_DEBUG_OUT("PL ZERO: %i, %i\r\n", dpl[c+1].last_timing, dpl[c].this_timing);
        {
          int timeDone =24;
          // TODO enable calib again when zeroing!
          v_resetIntegratorOffsets0();
          
          
          dpl[c+1].last_timing = (dpl[c].this_timing - timeDone);
          if (dpl[c+1].last_timing<0) dpl[c+1].last_timing=0;
        }
        break;
      }
      
      case PL_RASTER_TEXT:
      {
        LINE_DEBUG_OUT("PL RASTER DISPLAY AT x,y: %i,%i\r\n", dpl[c].x, dpl[c].y);
//        printf("PL RASTER DISPLAY AT x,y: %i,%i\r\n", dpl[c].x, dpl[c].y);
//      printf("PL RASTER No %i,%08X,%08X,%08X,%08X!\r\n", c, dpl[c], &dpl[c], dpl[c].rlines, &dpl[c].rlines);

        // reverse "translation"
        char* _string = dpl[c].debug;
        int8_t movementScale =  dpl[c].yRest; //movementScale;
        int x = dpl[c].x;
        int y = dpl[c].y;
        int halfOffset = dpl[c].pattern; // halfOffset
        int8_t xSize = dpl[c].x0; // width of font
        int8_t ySize = dpl[c].y0; // height of font

        int8_t yoffset = 0;
        
        int i=0;
        unsigned char *currentRasterline = dpl[c].rlines[i++];
        while (currentRasterline!=0)
        {
            ZERO_AND_WAIT();
            unsigned char* string = (unsigned char*)_string;
            UNZERO();

            // move to position
            v_setScale(movementScale);
            v_moveToImmediate8(x, y);

            // move to line in String (only y-movement)
            if (yoffset!=0)
            {
              if (halfOffset)
              {
                v_setScale(10);
                SET_YSH_IMMEDIATE_8(yoffset<<1);
                SET_XSH_IMMEDIATE_8(0);
              }
              else
              {
                v_setScale(19);
                SET_YSH_IMMEDIATE_8(yoffset);
                SET_XSH_IMMEDIATE_8(0);
              }
              START_T1_TIMER();
              WAIT_T1_END();
            }
            else
            {
              I_SET(VIA_port_a, 0x00);
              DELAY_XSH();
            }
    #define RASTER_WAIT 17
            // disable ramp and set y moevemtn to 0
            I_SET(VIA_port_b, 0x80); // disable ramp, mux = y integrator, enable mux
            DELAY_YSH();
            I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
            DELAY_PORT_B_BEFORE_PORT_A();

            // set width of text 
            I_SET(VIA_port_a, xSize);
            DELAY_XSH();
            
            // prepare for raster output
            I_SET(VIA_aux_cntl, 0x18);

            I_SET(VIA_port_b, 0x01); // enable ramp, mux = y integrator, disable mux
            
            // the vectorbeam is moving... now fill the shiftregister ASAP with the bitmap, and wait for 18 cycles to pass

            // print this rasterline!
            do 
            {
                ADD_DELAY_CYCLES(RASTER_WAIT); // wait for enable ramp, and printing the last letter

                // draw one char bitmap
                unsigned char charBitmap = currentRasterline[*string-0x20];
                string++;
                I_SET(VIA_shift_reg, charBitmap);
            } while (*string != 0);

            ADD_DELAY_CYCLES(6); // disabling ramp takes a certain amount of time! - reduce that from the original shift wait

            I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
            ADD_DELAY_CYCLES(4+4    +1);
            currentRasterline = dpl[c].rlines[i++];

            // enable non raster output
    #ifdef BEAM_LIGHT_BY_SHIFT
        I_SET (VIA_aux_cntl, 0x98); //Shift Reg. Enabled, T1 PB7 Enabled
    #endif
    #ifdef BEAM_LIGHT_BY_CNTL
        I_SET (VIA_aux_cntl, 0x80); // Shift reg mode = 000 free disable, T1 PB7 enabled 
    #endif
            ADD_DELAY_CYCLES(2);
            yoffset=yoffset+(ySize);
        }

        I_SET(VIA_shift_reg, 0);
        ZERO_AND_CONTINUE();
        break;
      }
      
      
      case PL_SET_BRIGHTNESS:
      {
        LINE_DEBUG_OUT("PL Brightness  A = %x\r\n", dpl[c].intensity);
#ifdef PITREX_DEBUG
        if (browseMode) break;
#endif
        LINE_DEBUG_OUT("     IMUX MUST BE SET\r\n");
        I_SET(VIA_port_b, 0x84); // MUX to intensity

        if (dpl[c].flags & PL_I_A_MUST_BE_SET)
        {
          LINE_DEBUG_OUT("     IA MUST BE SET\r\n");
          DELAY_PORT_B_BEFORE_PORT_A_BRIGHTNESS(); 
          I_SET(VIA_port_a, dpl[c].intensity); 


      
/*    
            if (dpl[c].flags & PL_Y_DELAY_TO_NULL)
            {
              LINE_DEBUG_OUT("     PL_Y_DELAY_TO_NULL\r\n");
                if (crankyFlag & CRANKY_NULLING_WAIT)
                {
              LINE_DEBUG_OUT("     CRANKY_NULLING_WAIT\r\n");
                    // some crankies need additional waits here!
                    ADD_DELAY_CYCLES(CRANKY_DELAY_Y_TO_NULL_VALUE);
                }
            }
*/    
      
      
        }
        DELAY_ZSH();

        if (crankyFlag & CRANKY_BETWEEN_VIA_B)
        {
          // TODO
          
          I_SET(VIA_port_b, 0x81); // MUX to intensity
          DELAY_PORT_B_BEFORE_PORT_A(); 
          I_SET(VIA_port_a, 0); 

          I_SET(VIA_port_b, 0x81); // MUX to intensity
          DELAY_PORT_B_BEFORE_PORT_A(); 
          I_SET(VIA_port_a, 0); 
        }

        break;
      }
      case PL_DRAW_DOT:
      {
        LINE_DEBUG_OUT ("PL DOT  %i, %i :%i\r\n    %s, time %i\r\n", dpl[c].x*dpl[c].this_timing, dpl[c].y*dpl[c].this_timing, dpl[c].this_timing, dpl[c].debug, dpl[c].this_timing);
        if (dpl[c].flags & PL_SWITCH_BEAM_ON)
        {
          SWITCH_BEAM_ON();
        }
        break;
      }
      case PL_MOVE:
      {
        LINE_DEBUG_OUT("PL MOVE: xStart: %i, x:%i x*s:%i, yStart: %i, y:%i y*s:%i scale:%i\r\n", dpl[c].xStart, dpl[c].x,dpl[c].x*dpl[c].this_timing, dpl[c].yStart, dpl[c].y, dpl[c].y*dpl[c].this_timing, dpl[c].this_timing);
        LINE_DEBUG_OUT("PL MOVE: X0: %i, Y0: %i\r\n",dpl[c].x0,dpl[c].y0);
        LINE_DEBUG_OUT("PL MOVE: Restx(%i), Resty(%i)\r\n",dpl[c].xRest,dpl[c].yRest);
      }
      case PL_DRAW_PATTERN:
      {
        if (dpl[c].type ==PL_DRAW_PATTERN)
          LINE_DEBUG_OUT ("PL DRAW PATTERN \r\n    %s\r\n", dpl[c].debug);
      }
      case PL_DRAW:
      {
// todo if consecutive draws
// handle them in one loop
// and draw them in one go!      
// would that be faster???        
        if (dpl[c].type ==PL_DRAW)
        {
          LINE_DEBUG_OUT("PL DRAW: xStart: %i, x:%i x*s:%i, yStart: %i, y:%i y*s:%i -> xEnd: %i, yEnd: %i scale:%i\r\n",dpl[c].xStart, dpl[c].x,dpl[c].x*dpl[c].this_timing, dpl[c].yStart ,dpl[c].y, dpl[c].y*dpl[c].this_timing,dpl[c].xEnd,dpl[c].yEnd, dpl[c].this_timing);
          LINE_DEBUG_OUT("PL DRAW: X0: %i, Y0: %i\r\n",dpl[c].x0,dpl[c].y0);
          LINE_DEBUG_OUT("PL DRAW: %s", dpl[c].debug);
        }

        if (dpl[c].flags & PL_CALIBRATE_0)
        {
          // TODO
          LINE_DEBUG_OUT ("    PL D CALIB 0\r\n");
          v_resetIntegratorOffsets0();
        }
        else if (dpl[c].flags & PL_CALIBRATE)
        {
          LINE_DEBUG_OUT ("    PL D CALIB \r\n");
          v_resetIntegratorOffsets();
          if (calibrationValue !=0)
          {
            // TODO Costs time :-()
            // isn't that too much???
            I_SET(VIA_port_a, 0x00);
            DELAY_YSH();
            I_SET(VIA_port_b, 0x80);
            DELAY_YSH();
            I_SET(VIA_port_b, 0x81);
            DELAY_PORT_B_BEFORE_PORT_A();
          }
        }
        
        if (dpl[c].flags & PL_Y_MUST_BE_SET)
        {
          LINE_DEBUG_OUT("     Y MUST BE I_SET (%i)\r\n", dpl[c].y);
          if (dpl[c].flags & PL_Y_A_MUST_BE_SET)
          {
            LINE_DEBUG_OUT("     YA MUST BE I_SET\r\n");
            if (dpl[c].flags & PL_MUX_Y_MUST_BE_SET)
            {
              LINE_DEBUG_OUT("     YMUX MUST BE I_SET\r\n");
              I_SET(VIA_port_b, 0x80);
              DELAY_PORT_B_BEFORE_PORT_A();
            }
            //what was set before? -> waiting for T1 timer
            I_SET(VIA_port_a, dpl[c].y);

            if (dpl[c].flags & PL_Y_DELAY_TO_NULL)
            {
              LINE_DEBUG_OUT("     PL_Y_DELAY_TO_NULL\r\n");
                if (crankyFlag & CRANKY_NULLING_WAIT)
                {
              LINE_DEBUG_OUT("     CRANKY_NULLING_WAIT\r\n");
                    // some crankies need additional waits here!
                    ADD_DELAY_CYCLES(CRANKY_DELAY_Y_TO_NULL_VALUE);
                }
            }
          }
          else
          {
            if (dpl[c].flags & PL_MUX_Y_MUST_BE_SET)
            {
              //what was set before? -> waiting for T1 timer
              I_SET(VIA_port_b, 0x80); // MUX to y integrator
            }
          }
          DELAY_YSH();
        }
        else
          LINE_DEBUG_OUT("     Y NEED NOT BE I_SET\r\n");

        
        // for the beam to be switched off from last drawing we have
        // to wait alltogether for "DELAY_AFTER_T1_END_VALUE" cycles
        if (delayedBeamOff)
        {
// ??          if (dpl[c].type == PL_DRAW) toDelayOff-=5; // consecutive draws wait less ;-)
          waitT1OffMark(DELAY_AFTER_T1_END_VALUE);
          SWITCH_BEAM_OFF();
        }
        else
        {
          if (dpl[c].flags & PL_DEACTIVATE_ZERO)
          {
              // attention!
              // UNZERO is also a BEAM_OFF!
              UNZERO();
          }
        }

        // not checking - since we also do ramp with B
        // if (dpl[c].flags & PL_MUX_X_MUST_BE_SET;
        I_SET(VIA_port_b, 0x81);
        if (dpl[c].flags & PL_X_A_MUST_BE_SET)
        {
          // to test only if cranky?dpl[c].x
          DELAY_PORT_B_BEFORE_PORT_A();
          I_SET(VIA_port_a, dpl[c].x);
  DELAY_XSH(); 
        }
        // start T1 timer
        if (dpl[c].flags & PL_T1_LO_EQUALS)
        {
          SET(VIA_t1_cnt_hi, (dpl[c].this_timing)>>8);
        }
        else
        {
          SETW_inverse(VIA_t1, dpl[c].this_timing);  /* scale due to "enlargement" is 16 bit! */
        }

        if (dpl[c].type ==PL_DRAW)
        {
            LINE_DEBUG_OUT ("     x:%i, y:%i scale:%i \r\n", dpl[c].x*dpl[c].this_timing, dpl[c].y*dpl[c].this_timing, dpl[c].this_timing);
            if (dpl[c].flags & PL_SWITCH_BEAM_ON)
            {
              LINE_DEBUG_OUT ("     beam switched on \r\n");
              SWITCH_BEAM_ON();
            }
        }
        else if (dpl[c].type == PL_DRAW_PATTERN)// this must be  (dpl[c].type == PL_DRAW_PATTERN))
        {
            LINE_DEBUG_OUT ("     %i, %i :%i - $%02x\r\n", dpl[c].x*dpl[c].this_timing, dpl[c].y*dpl[c].this_timing, dpl[c].this_timing, dpl[c].pattern);
            int patternAnds[] = {128,64,32,16,8,4,2,1};
            int pCount = 0;
            while ((GET(VIA_int_flags) & 0x40) == 0)
            {
#ifdef BEAM_LIGHT_BY_CNTL
                if (dpl[c].pattern & patternAnds[pCount])
                    SWITCH_BEAM_ON();
                else
                    SWITCH_BEAM_OFF();
                pCount=pCount+1;
                if (pCount==8) pCount=0;
#endif
#ifdef BEAM_LIGHT_BY_SHIFT
                if (pCount==0)
                {
                    SET_SHIFT_REG(dpl[c].pattern);
                    pCount=18;
                }
                else
                    pCount-=2;
#endif
            }
            int delayT1 = DELAY_AFTER_T1_END_VALUE;
            while (delayT1>0)
            {
#ifdef BEAM_LIGHT_BY_CNTL
                if (dpl[c].pattern & patternAnds[pCount])
                    SWITCH_BEAM_ON();
                else
                    SWITCH_BEAM_OFF();
                pCount=pCount+1;
                if (pCount==8) pCount=0;
#endif
#ifdef BEAM_LIGHT_BY_SHIFT
                if (pCount==0)
                {
                    SET_SHIFT_REG(dpl[c].pattern);
                    pCount=18;
                }
                else
                    pCount-=2;
#endif
                delayT1-=2;
            }
            SWITCH_BEAM_OFF();
#ifdef PITREX_DEBUG
            if ((browseMode) && (dpl[c].type == PL_DRAW_PATTERN))
            {
              if (lineNo==currentBrowsline)
                currentDisplayedBrowseLine = currentBrowsline;
              lineNo++;
            }
#endif            
        }
        break;
      }
      default:
        break;
    }
    c++;
#ifdef MY_DEBUG
    if (myDebug)   printf("handled %i display elements...!\r\n", c);
#endif
    delayedBeamOff = 0;
    CCNT0(t1OffMark); // reset T1 "off" marker
    
    
    if (dpl[c].flags & PL_LAST_MUST_FINISH )
    {

      if (dpl[c].flags & PL_LAST_IS_RAMPING)
      {

//working?


        while ((GET(VIA_int_flags) & 0x40) == 0);
        // set T1 off marker
        CCNT0(t1OffMark);
        if ((dpl[c-1].type == PL_MOVE) && (dpl[c].type == PL_DRAW_DOT)) // same as !((dpl[c].flags & PL_SWITCH_BEAM_OFF)
        {
          // only needed for dots?
          // only needed, when (dpl[c].flags & PL_SWITCH_BEAM_ON)?
          waitT1OffMark(DELAY_AFTER_T1_END_VALUE);
        }
      }
      else
      {
        // e.g. zeroing
        // dots
        // RASTER
        LINE_DEBUG_OUT ("Dot/Zero/Raster delay: %i\n\r", dpl[c].last_timing);
        ADD_DELAY_CYCLES(dpl[c].last_timing);
      }
#ifdef PITREX_DEBUG
      if ((browseMode) && ((dpl[c-1].type ==PL_DRAW)||(dpl[c-1].type ==PL_DRAW_DOT)||(dpl[c-1].type ==PL_DRAW_PATTERN)   ))
      {
        if (lineNo==currentBrowsline)
          currentDisplayedBrowseLine = currentBrowsline;
        lineNo++;
      }
#endif      
    }

    if ((dpl[c].flags & PL_SWITCH_BEAM_OFF)/* && (dpl[c].type != PL_ZERO)*/)
    {
      // last was with light on, now switch it off
      // or at least prepare switch off
//&& (!(dpl[c].flags & PL_CALIBRATE))      
      if (((dpl[c].type == PL_MOVE) || (dpl[c].type == PL_DRAW)|| (dpl[c].type == PL_DRAW_PATTERN) ) && (dpl[c-1].type != PL_DRAW_DOT)) 
      {
        // we were drawing now - and next we will either move or draw again
        delayedBeamOff = 1;
        LINE_DEBUG_OUT ("Delayed beam off: \n\r");
      }
      else
      {
        // we are last was a dot or a zero
        // pls don't ask why +5
        // it just is !)(/"(!=!??!!!
        // e.g. this is a move, last time was a draw -> switch off
        waitT1OffMark(DELAY_AFTER_T1_END_VALUE+5);
        LINE_DEBUG_OUT ("Beam off: \n\r");
        
        
        SWITCH_BEAM_OFF();
        if ((dpl[c-1].flags & PL_DEFLOK_AFTER)/* && (dpl[c].type != PL_ZERO)*/)
        {
          LINE_DEBUG_OUT ("Deflok after: \n\r");
//          v_deflok();
          v_resetIntegratorOffsets0();
          ZERO_AND_WAIT();
        }        
      }
    }
  }

  // savety only
  SWITCH_BEAM_OFF();
  ZERO_AND_CONTINUE();
  
 
#ifdef MY_DEBUG
  uint32_t value;
  CCNT0(value);

  value = (value-startValue) / 666; // in vectrex cycles
  if (myDebug)   printf("Pipeline display: %i\r\n", value);
#endif    
}


//  0x50, 0x09
unsigned char uniDirectional[] =
{
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b11100000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00111111, 0b11110000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000001, 0b10000000, 0b01111111, 0b11111000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000011, 0b11111001, 0b11111111, 0b11111100, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000111, 0b11111111, 0b11111111, 0b11111110, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00001111, 0b11111111, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00001111, 0b11111111, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b10000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00111111, 0b11111111, 0b11111111, 0b11111111, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00111111, 0b11111111, 0b11111111, 0b11111111, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00111111, 0b11111111, 0b11111111, 0b11111111, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b01111111, 0b11111111, 0b11111111, 0b11111111, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b01111111, 0b11111111, 0b11111111, 0b11111111, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b01111111, 0b11111111, 0b11111111, 0b11111111, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00111111, 0b11111111, 0b11111111, 0b11111111, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00011111, 0b11111111, 0b11111111, 0b11110111, 0b11111000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00111111, 0b11111111, 0b11111111, 0b00000000, 0b10111000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00001111, 0b11111111, 0b11111000, 0b00000000, 0b00111000, 0b11000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b01111111, 0b11111000, 0b00011111, 0b01111000, 0b11000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00111111, 0b11111000, 0b00000111, 0b11111000, 0b11000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00111000, 0b00001111, 0b11110000, 0b00000001, 0b11111000, 0b11100000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00100000, 0b00000111, 0b11100010, 0b00010001, 0b11111101, 0b11100000, 0b00000000, //forward
  0b00000000, 0b00010000, 0b11000000, 0b01000011, 0b11000011, 0b00011111, 0b11111101, 0b11000000, 0b00000000, //forward
  0b00000000, 0b00010000, 0b11000100, 0b01001011, 0b11001111, 0b11111111, 0b11111101, 0b11000000, 0b00000000, //forward
  0b00000000, 0b00010000, 0b11100100, 0b11111001, 0b11011111, 0b11111111, 0b11111101, 0b11000000, 0b00000000, //forward
  0b00000000, 0b00001001, 0b11111111, 0b11111001, 0b11011111, 0b11111111, 0b11111001, 0b11000000, 0b00000000, //forward
  0b00000000, 0b00001001, 0b11111111, 0b11111101, 0b11111111, 0b11111111, 0b11111001, 0b11000000, 0b00000000, //forward
  0b00000000, 0b00001001, 0b11111111, 0b11111001, 0b11111111, 0b11111111, 0b11111001, 0b11000000, 0b00000000, //forward
  0b00000000, 0b00001001, 0b11111111, 0b11111001, 0b11001111, 0b11111111, 0b11110001, 0b11000000, 0b00000000, //forward
  0b00000000, 0b00001001, 0b11111111, 0b11111001, 0b11001111, 0b11111111, 0b11110001, 0b11000000, 0b00000000, //forward
  0b00000000, 0b00000001, 0b11111111, 0b11111001, 0b11101111, 0b11111111, 0b11110001, 0b10000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b11111111, 0b11111011, 0b11100011, 0b11111111, 0b11110011, 0b10000000, 0b00000000, //forward
  0b00000000, 0b00000100, 0b11111111, 0b11110011, 0b11110011, 0b11111111, 0b11110011, 0b10000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b11111111, 0b11100011, 0b11111001, 0b11111111, 0b11110011, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000010, 0b01111111, 0b11100111, 0b11111111, 0b11111111, 0b11110000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000010, 0b01111111, 0b11101011, 0b11100011, 0b11111111, 0b11110000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b01111111, 0b11110000, 0b11000111, 0b11111111, 0b11110000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00111111, 0b11111000, 0b00011111, 0b11111111, 0b11110000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00111111, 0b11111111, 0b11111001, 0b10111111, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00011111, 0b10000111, 0b11111101, 0b11011111, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00011111, 0b10011111, 0b11111111, 0b11111111, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00011111, 0b11111111, 0b11111110, 0b00111111, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00011111, 0b11110111, 0b11100000, 0b00111111, 0b11000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00011111, 0b11000000, 0b00000001, 0b11111111, 0b10010000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00001111, 0b11100000, 0b00111111, 0b11111111, 0b10010000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000111, 0b11111111, 0b11111111, 0b11111111, 0b00110000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000011, 0b11111111, 0b11101111, 0b11111110, 0b00110000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000001, 0b11111100, 0b00000011, 0b11111110, 0b01110000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b11111110, 0b00011111, 0b11111100, 0b01100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b01111111, 0b11111111, 0b11111000, 0b11110000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b01111111, 0b11111111, 0b11110001, 0b11110000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00111111, 0b11111111, 0b11100011, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b11111111, 0b11000111, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b11111111, 0b10001111, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11111101, 0b00011111, 0b11100000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000000, 0b00000001, 0b11111000, 0b00111111, 0b11000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000001, 0b00000000, 0b00000000, 0b01111111, 0b11000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000001, 0b10000000, 0b00000001, 0b11111111, 0b10000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000001, 0b10000000, 0b00111111, 0b11111111, 0b10000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000001, 0b11000011, 0b11111111, 0b11111110, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000001, 0b11100001, 0b11111111, 0b11110000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000001, 0b11110001, 0b11111111, 0b11000000, 0b00000000, 0b00000000, 0b00000000, //forward
  0b00000000, 0b00000000, 0b00000001, 0b11111100, 0b11111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, //forward
};

void v_directMove32n(int32_t xEnd, int32_t yEnd)
{
  v_zeroWait();
  int32_t x = xEnd*sizeX+offsetX;
  int32_t y = yEnd*sizeY+offsetY;

  UNZERO(); // ensure vector beam can be moves
  SET_OPTIMAL_SCALE(x, y);
  SET_YSH16(y);
  SET_XSH16(x);
  START_T1_TIMER();
  consecutiveDraws=0;
  currentCursorX = x;
  currentCursorY = y;
  WAIT_T1_END();
}
void v_directFastDown(int32_t y)
{

  SET_YSH8(y);
  SET_XSH8(0);
  I_SET(VIA_port_b, 0x01); // enable ramp, mux = y integrator, disable mux
  ADD_DELAY_CYCLES(5);
  I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
  SET_YSH8(0);
  I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
}
// todo ADD to Pipeline
// 8 bit for now
void v_printBitmapUni(unsigned char *bitmapBlob, int width, int height, int sizeX, int x, int y)
{
//  v_readButtons(); 
//  bitmapBlob=uniDirectional;
//  width = 0x09;
//  height = 0x50;
//  x = -(9*4);
//  y = 0x28;
//    v_setBrightness(64);
  
    int patternAnds[] = {128,64,32,16,8,4,2,1};
    // uni directional
    
    for (int yy=0;yy<height;yy++)
    {
    currentYSH=currentPortA=0x100;

      v_directMove32n(x*128,(y-yy*2)*128);
////////////////
// Prepare line print


        int afterYDelay = 6; // cranky dependend
        if (crankyFlag & CRANKY_BETWEEN_VIA_B)
        {
            afterYDelay += crankyFlag&0x0f;
        }

        I_SET(VIA_port_b, 0x80);
        DELAY_PORT_B_BEFORE_PORT_A();
        I_SET(VIA_port_a, 0);

//        UNZERO();
        afterYDelay -= 2;

        if (crankyFlag & CRANKY_NULLING_WAIT)
        {
            // some crankies need additional waits here!
            afterYDelay += CRANKY_DELAY_Y_TO_NULL_VALUE;
        }
        if (afterYDelay>0)
          ADD_DELAY_CYCLES(afterYDelay);

        I_SET(VIA_port_b, 0x81);
        DELAY_PORT_B_BEFORE_PORT_A();
        I_SET(VIA_port_a, sizeX);
////////////////
        // prepare for raster output

        SET(VIA_aux_cntl, 0x00);
//ADD_DELAY_CYCLES(1);
        // now print one line pattern

        I_SET(VIA_port_b, 0x01); // enable ramp, mux = y integrator, disable mux
        for (int xx=0;xx<width;xx++)
        {
            for (int bit=0;bit<8;bit++)
            {
                // output one raster dot - or not
                if (*bitmapBlob & patternAnds[bit])
                {
                  SET(VIA_cntl, 0xee);
//                    SWITCH_BEAM_ON();
                }
                else
                {
                  SET(VIA_cntl, 0xce);
//                    SWITCH_BEAM_OFF();
                }
            }
            bitmapBlob++;
        }
        // switch off, if last bit was on!
        if (*bitmapBlob & 1)
            SET(VIA_cntl, 0xce);
        
        
        I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
        // assume lightning is done CNTL
        SET (VIA_aux_cntl, 0x80); // Shift reg mode = 000 free disable, T1 PB7 enabled
    }
}

/*
// 8 bit for now
void v_printBitmapUniBi(unsigned char *bitmapBlob, int width, int height, int sizeX, int x, int y)
{
//  v_readButtons(); 
//  bitmapBlob=uniDirectional;
//  width = 0x09;
//  height = 0x50;
//  x = -(9*4);
//  y = 0x28;
//    v_setBrightness(64);
  
    v_directMove32n(x*128,y*128);
    int patternAnds[] = {128,64,32,16,8,4,2,1};
    // uni directional
    
    for (int yy=0;yy<height;yy++)
    {
      currentYSH=currentPortA=0x100;
        int afterYDelay = 6; // cranky dependend
        if (crankyFlag & CRANKY_BETWEEN_VIA_B)
        {
            afterYDelay += crankyFlag&0x0f;
        }

        I_SET(VIA_port_b, 0x80);
        DELAY_PORT_B_BEFORE_PORT_A();
        I_SET(VIA_port_a, 0);
        afterYDelay -= 2;

        if (crankyFlag & CRANKY_NULLING_WAIT)
        {
            // some crankies need additional waits here!
            afterYDelay += CRANKY_DELAY_Y_TO_NULL_VALUE;
        }
        if (afterYDelay>0)
          ADD_DELAY_CYCLES(afterYDelay);

        I_SET(VIA_port_b, 0x81);
        DELAY_PORT_B_BEFORE_PORT_A();
        I_SET(VIA_port_a, sizeX);
////////////////
        // prepare for raster output

        SET(VIA_aux_cntl, 0x00);
//ADD_DELAY_CYCLES(1);
        // now print one line pattern

        I_SET(VIA_port_b, 0x01); // enable ramp, mux = y integrator, disable mux
if (yy!=0) ADD_DELAY_CYCLES(21);
        for (int xx=0;xx<width;xx++)
        {
            for (int bit=0;bit<8;bit++)
            {
                // output one raster dot - or not
                if (*bitmapBlob & patternAnds[bit])
                {
                  vectrexwrite_short(VIA_cntl, 0xee);
//                    SWITCH_BEAM_ON();
                }
                else
                {
                  vectrexwrite_short(VIA_cntl, 0xce);
//                    SWITCH_BEAM_OFF();
                }
            }
            bitmapBlob++;
        }
        // switch off, if last bit was on!
        if (*bitmapBlob & 1) vectrexwrite_short(VIA_cntl, 0xce);
        I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
        
        
        
        // and go back!
        yy++;
        if (yy<height)
        {
          v_directFastDown(-15);
          DELAY_PORT_B_BEFORE_PORT_A();

          int afterYDelay = 6; // cranky dependend
          if (crankyFlag & CRANKY_BETWEEN_VIA_B)
          {
              afterYDelay += crankyFlag&0x0f;
          }

          I_SET(VIA_port_b, 0x80);
          DELAY_PORT_B_BEFORE_PORT_A();
          I_SET(VIA_port_a, 0);
          afterYDelay -= 2;

          if (crankyFlag & CRANKY_NULLING_WAIT)
          {
              // some crankies need additional waits here!
              afterYDelay += CRANKY_DELAY_Y_TO_NULL_VALUE;
          }
          if (afterYDelay>0)
            ADD_DELAY_CYCLES(afterYDelay);

          I_SET(VIA_port_b, 0x81);
          DELAY_PORT_B_BEFORE_PORT_A();
            I_SET(VIA_port_a, -sizeX);
          
          
          I_SET(VIA_port_b, 0x01); // enable ramp, mux = y integrator, disable mux
ADD_DELAY_CYCLES(23);
          
          for (int xx=width-1;xx>=0;xx--)
          {
              for (int bit=7;bit>=0;bit--)
              {
                  // output one raster dot - or not
                  if (*(bitmapBlob+xx) & patternAnds[bit])
                  {
                    vectrexwrite_short(VIA_cntl, 0xee);
                  }
                  else
                  {
                    vectrexwrite_short(VIA_cntl, 0xce);
                  }
              }
          }
          if (*bitmapBlob & 128) vectrexwrite_short(VIA_cntl, 0xce);
          I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
          
          bitmapBlob+=width;
          v_directFastDown(-15);
        }
    }
    SET (VIA_aux_cntl, 0x80); // Shift reg mode = 000 free disable, T1 PB7 enabled
}
*/









// shifted version ONLY!
void v_immediateDraw32Patterned(int8_t xEnd, int8_t yEnd, uint8_t pattern)
{
  int patternAnds[] = {128,64,32,16,8,4,2,1};
  int pCount = 0;
  
  // regardless of BEAM Control - here we use the shift register
  SET (VIA_aux_cntl, 0x98); //Shift Reg. Enabled, T1 PB7 Enabled

  SET_YSH_IMMEDIATE_8(yEnd);

  
  
  waitUntil(666);
  SET_YSH_IMMEDIATE_8(yEnd);
  waitUntil(666);

  SET_XSH_IMMEDIATE_8(xEnd);

  START_T1_TIMER();
  while ((GET(VIA_int_flags) & 0x40) == 0)
  {
      SET_SHIFT_REG(pattern);
      PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET);
      waitUntil(15*640+500);
  }
  SET_SHIFT_REG(0x00);
  #ifdef BEAM_LIGHT_BY_CNTL
  SET (VIA_aux_cntl, 0x80); // Shift reg mode = 000 free disable, T1 PB7 enabled
  #endif
}


void orgv_immediateDraw32Patterned(int8_t xEnd, int8_t yEnd, uint8_t pattern)
{
  int patternAnds[] = {128,64,32,16,8,4,2,1};
  int pCount = 0;
  #ifdef BEAM_LIGHT_BY_SHIFT
  SET (VIA_aux_cntl, 0x98); //Shift Reg. Enabled, T1 PB7 Enabled
  #endif
  #ifdef BEAM_LIGHT_BY_CNTL
  SET (VIA_aux_cntl, 0x80); // Shift reg mode = 000 free disable, T1 PB7 enabled
  #endif


  SET_YSH_IMMEDIATE_8(yEnd);
  SET_XSH_IMMEDIATE_8(xEnd);

  START_T1_TIMER();
  while ((GET(VIA_int_flags) & 0x40) == 0)
  {
#ifdef BEAM_LIGHT_BY_CNTL
      if (pattern & patternAnds[pCount])
          SWITCH_BEAM_ON();
      else
          SWITCH_BEAM_OFF();
      pCount=pCount+1;
      if (pCount==8) pCount=0;
#endif
#ifdef BEAM_LIGHT_BY_SHIFT
      if (pCount==0)
      {
          SET_SHIFT_REG(pattern);
          pCount=18;
      }
      else
          pCount-=2;
#endif
      
  }

  int delayT1 = DELAY_AFTER_T1_END_VALUE;
  while (delayT1>0)
  {
#ifdef BEAM_LIGHT_BY_CNTL
      if (pattern & patternAnds[pCount])
          SWITCH_BEAM_ON();
      else
          SWITCH_BEAM_OFF();
      pCount=pCount+1;
      if (pCount==8) pCount=0;
#endif
#ifdef BEAM_LIGHT_BY_SHIFT
      if (pCount==0)
      {
          SET_SHIFT_REG(pattern);
          pCount=18;
      }
      else
          pCount-=2;
#endif
      delayT1-=2;
  }
  SWITCH_BEAM_OFF();
  ZERO_AND_CONTINUE();
}

void saveScreenshot(void)
{
  int c = 0;

  VectorPipeline *dpl = _VPL[vectorPipeLineNextRead];
  while (dpl[c].type != PL_END)
  {
    if ((dpl[c].type ==PL_DRAW) || (dpl[c].type == PL_DRAW_PATTERN)|| (dpl[c].type == PL_DRAW_DOT))
    {
      int xStart = dpl[c].xStart*sizeX+offsetX;
      int yStart = dpl[c].yStart*sizeY+offsetY;
      int xEnd = dpl[c].xEnd*sizeX+offsetX;
      int yEnd = dpl[c].yEnd*sizeY+offsetY;
      
      
      printf(" db $%02X, $%02X, $%02X, $%02X\n\r", (yStart/256)&0xff, (xStart/256)&0xff, (yEnd/256)&0xff, (xEnd/256)&0xff);
    }
    c++;
  }
}



typedef enum {
  CAL_TYPE_NONE = 0,
  CAL_TYPE_UCHAR = 1,
  CAL_TYPE_BOOL = 2,
  CAL_TYPE_UINT = 3,
  CAL_TYPE_SINT = 4,
  CAL_TYPE_FLOAT = 5
} CalType;

typedef struct {
  uint8_t *ucvalue;
  uint8_t *scvalue;
  unsigned int *uivalue;
  int16_t *sivalue;
  float *fvalue;
  int min;
  int max;
  int defaultValue;
  char *name;
  CalType type;
  char **help;
} CalibrationItems;

// pos in 8 bit (7f)
// width in 8 bit
// scale is changed within component
// text is 0 terminated
// reacts on current josytick input
int v_immediateWidgetSlider(CalibrationItems *item, int x, int y, int active)
{
  char buf[80];
  int width = 80;

  int min = item->min;
  int max = item->max;
  int resetValue = item->defaultValue;
  char *text = item->name;
  
  float pos;

  if ((item->type == CAL_TYPE_UCHAR) || (item->type == CAL_TYPE_BOOL))
    pos = ((float)width)/((float)(max-min)) * ((float)((*item->ucvalue)-min));
  else if(item->type == CAL_TYPE_UINT)
    pos = ((float)width)/((float)(max-min)) * ((float)((*item->uivalue)-min) );
  else if(item->type == CAL_TYPE_SINT)
    pos = ((float)width)/((float) (max-min) ) * ((float)((*item->sivalue) - min));
  else if(item->type == CAL_TYPE_FLOAT)
    pos = ((float)width)/((float) (max-min) ) * ((float)((*item->fvalue) - min));




  
  
  if (active)
  {
    v_setBrightness(0x5f);
    sprintf(buf, "%i", min);
    v_printStringRaster(x-10, y-10, buf, 50, -4, 0);
    sprintf(buf, "%i", max);
    v_printStringRaster(x+width-10, y-10, buf, 50, -4, 0);

    if ((item->type == CAL_TYPE_UCHAR) || (item->type == CAL_TYPE_BOOL))
      sprintf(buf, "%i", *item->ucvalue);
    else if(item->type == CAL_TYPE_UINT)
      sprintf(buf, "%i", *item->uivalue);
    else if(item->type == CAL_TYPE_SINT)
      sprintf(buf, "%i", *item->sivalue);
    else if(item->type == CAL_TYPE_FLOAT)
      sprintf(buf, "%1.2f", *item->fvalue);
    
    v_printStringRaster((int)(x+pos)-5, y+15, buf, 35, -2, 0);

    // attention - float!
    if ((internalButtonState&0x0f) == (0x06)) // button 2+3 
    {
      if ((item->type == CAL_TYPE_UCHAR) || (item->type == CAL_TYPE_BOOL)) *item->ucvalue=resetValue;
      else if(item->type == CAL_TYPE_UINT)  *item->uivalue=resetValue;
      else if(item->type == CAL_TYPE_SINT)  *item->sivalue=resetValue;
      else if(item->type == CAL_TYPE_FLOAT)  *item->fvalue=resetValue;
    }
    
    if ((internalButtonState &0xf) == 0)
    {
      if (valueChangeDelay>0) 
        valueChangeDelay--;
      if ((internalJoy1X==0) && (internalJoy1Y==0)) valueChangeDelay =0;
      if (valueChangeDelay<=0)
      {
        if (internalJoy1X>80)
        {
          if ((item->type == CAL_TYPE_UCHAR) || (item->type == CAL_TYPE_BOOL))
          {
            if (*item->ucvalue<max) *item->ucvalue=*item->ucvalue + 1;
          }
          else if (item->type == CAL_TYPE_UINT) 
          {
            if (*item->uivalue<max) *item->uivalue=*item->uivalue + 1;
          }
          else if(item->type == CAL_TYPE_SINT)  
          {
            if (*item->sivalue<max) *item->sivalue=*item->sivalue + 1;
          }
          else if(item->type == CAL_TYPE_FLOAT)  
          {
            if (*item->fvalue<max) *item->fvalue=*item->fvalue + 0.01;
          }

          valueChangeDelay = 64-(internalJoy1X>>1);
        }
        if (internalJoy1X<-80)
        {
          if ((item->type == CAL_TYPE_UCHAR) || (item->type == CAL_TYPE_BOOL))
          {
            if (*item->ucvalue>min) *item->ucvalue=*item->ucvalue - 1;
          }
          else if (item->type == CAL_TYPE_UINT) 
          {
            if (*item->uivalue>min) *item->uivalue=*item->uivalue - 1;
          }
          else if(item->type == CAL_TYPE_SINT)  
          {
            if (*item->sivalue>min) *item->sivalue=*item->sivalue - 1;
          }
          else if(item->type == CAL_TYPE_FLOAT)  
          {
            if (*item->fvalue>min) *item->fvalue=*item->fvalue - 0.01;
          }
          valueChangeDelay = 64+(internalJoy1X>>1);
        }
      }
    }
    else if ((internalButtonState &0xf) == 2)
    {
      if (valueChangeDelay>0) 
        valueChangeDelay--;
      if ((internalJoy1X==0) && (internalJoy1Y==0)) valueChangeDelay =0;
      if (valueChangeDelay<=0)
      {
        if (internalJoy1X>80)
        {
          if ((item->type == CAL_TYPE_UCHAR) || (item->type == CAL_TYPE_BOOL))
          {
            if (*item->ucvalue<max- 10) *item->ucvalue=*item->ucvalue + 10;
          }
          else if (item->type == CAL_TYPE_UINT) 
          {
            if (*item->uivalue<max- 10) *item->uivalue=*item->uivalue + 10;
          }
          else if(item->type == CAL_TYPE_SINT)  
          {
            if (*item->sivalue<max- 10) *item->sivalue=*item->sivalue + 10;
          }
          else if(item->type == CAL_TYPE_FLOAT)  
          {
            if (*item->fvalue<max- 0.1) *item->fvalue=*item->fvalue + 0.1;
          }

          valueChangeDelay = 64-(internalJoy1X>>1);
        }
        if (internalJoy1X<-80)
        {
          if ((item->type == CAL_TYPE_UCHAR) || (item->type == CAL_TYPE_BOOL))
          {
            if (*item->ucvalue>min+ 10) *item->ucvalue=*item->ucvalue - 10;
          }
          else if (item->type == CAL_TYPE_UINT) 
          {
            if (*item->uivalue>min+ 10) *item->uivalue=*item->uivalue - 10;
          }
          else if(item->type == CAL_TYPE_SINT)  
          {
            if (*item->sivalue>min+ 10) *item->sivalue=*item->sivalue - 10;
          }
          else if(item->type == CAL_TYPE_FLOAT)  
          {
            if (*item->fvalue>min+0.1) *item->fvalue=*item->fvalue - 0.1;
          }
          valueChangeDelay = 64+(internalJoy1X>>1);
        }
      }
    }
  }
  else
  {
    v_setBrightness(0x20);
  }
  
  v_printStringRaster(x-20, y+25, text, 40, -2, 0);
  ZERO_AND_WAIT();
  UNZERO();
  v_setScaleForced(0x7f);
  v_moveToImmediate8(x, y);
  v_drawToImmediate8(width, 0);

  ZERO_AND_WAIT();
  UNZERO();
  
  v_moveToImmediate8((int)(x+pos), y+5);
  v_drawToImmediate8(0, -10);
  ZERO_AND_WAIT();
}







const signed char vectrexExactExample[]=
{   (signed char) 0xFF, +0x00, +0x0C,  // pattern, y, x
    (signed char) 0x00, +0x00, +0x10,  // pattern, y, x
    (signed char) 0xFF, +0x00, +0x10,  // pattern, y, x
    (signed char) 0x00, +0x18, +0x18,  // pattern, y, x
    (signed char) 0xFF, -0x40, +0x00,  // pattern, y, x
    (signed char) 0xFF, +0x10, +0x24,  // pattern, y, x
    (signed char) 0xFF, +0x44, -0x02,  // pattern, y, x
    (signed char) 0xFF, +0x44, -0x02,  // pattern, y, x
    (signed char) 0xFF, +0x42, +0x00,  // pattern, y, x
    (signed char) 0xFF, +0x42, +0x00,  // pattern, y, x
    (signed char) 0xFF, +0x0C, -0x08,  // pattern, y, x
    (signed char) 0xFF, +0x08, -0x24,  // pattern, y, x
    (signed char) 0xFF, -0x08, -0x4C,  // pattern, y, x
    (signed char) 0xFF, -0x08, -0x4C,  // pattern, y, x
    (signed char) 0xFF, -0x10, -0x08,  // pattern, y, x
    (signed char) 0xFF, -0x18, +0x00,  // pattern, y, x
    (signed char) 0x00, -0x10, +0x0C,  // pattern, y, x
    (signed char) 0xFF, -0x18, +0x00,  // pattern, y, x
    (signed char) 0x00, +0x00, -0x0C,  // pattern, y, x
    (signed char) 0xFF, +0x18, -0x0C,  // pattern, y, x
    (signed char) 0x00, -0x10, -0x14,  // pattern, y, x
    (signed char) 0xFF, -0x14, +0x14,  // pattern, y, x
    (signed char) 0x00, -0x0C, -0x08,  // pattern, y, x
    (signed char) 0xFF, +0x10, -0x18,  // pattern, y, x
    (signed char) 0x00, +0x48, +0x30,  // pattern, y, x
    (signed char) 0xFF, +0x08, +0x4E,  // pattern, y, x
    (signed char) 0xFF, +0x08, +0x4E,  // pattern, y, x
    (signed char) 0x00, -0x04, +0x04,  // pattern, y, x
    (signed char) 0xFF, -0x36, +0x02,  // pattern, y, x
    (signed char) 0xFF, -0x36, +0x02,  // pattern, y, x
    (signed char) 0xFF, -0x36, +0x02,  // pattern, y, x
    (signed char) 0xFF, -0x36, +0x02,  // pattern, y, x
    (signed char) 0xFF, +0x08, +0x20,  // pattern, y, x
    (signed char) 0x00, -0x18, +0x00,  // pattern, y, x
    (signed char) 0xFF, -0x10, +0x00,  // pattern, y, x
    (signed char) 0xFF, -0x0C, -0x18,  // pattern, y, x
    (signed char) 0xFF, +0x14, +0x00,  // pattern, y, x
    (signed char) 0xFF, +0x08, +0x18,  // pattern, y, x
    (signed char) 0x00, -0x34, -0x24,  // pattern, y, x
    (signed char) 0xFF, +0x00, -0x08,  // pattern, y, x
    (signed char) 0xFF, +0x44, +0x00,  // pattern, y, x
    (signed char) 0xFF, +0x02, -0x4E,  // pattern, y, x
    (signed char) 0xFF, +0x02, -0x4E,  // pattern, y, x
    (signed char) 0x00, +0x08, +0x00,  // pattern, y, x
    (signed char) 0xFF, -0x40, +0x00,  // pattern, y, x
    (signed char) 0x00, +0x00, +0x08,  // pattern, y, x
    (signed char) 0xFF, +0x04, +0x1C,  // pattern, y, x
    (signed char) 0x00, -0x04, -0x0C,  // pattern, y, x
    (signed char) 0xFF, -0x06, +0x42,  // pattern, y, x
    (signed char) 0xFF, -0x06, +0x42,  // pattern, y, x
    (signed char) 0x00, +0x08, +0x00,  // pattern, y, x
    (signed char) 0xFF, +0x04, -0x3A,  // pattern, y, x
    (signed char) 0xFF, +0x04, -0x3A,  // pattern, y, x
    (signed char) 0xFF, +0x30, -0x04,  // pattern, y, x
    (signed char) 0x00, +0x24, +0x18,  // pattern, y, x
    (signed char) 0xFF, +0x00, +0x4C,  // pattern, y, x
    (signed char) 0xFF, +0x48, +0x00,  // pattern, y, x
    (signed char) 0xFF, +0x48, +0x00,  // pattern, y, x
    (signed char) 0xFF, -0x04, -0x2C,  // pattern, y, x
    (signed char) 0xFF, -0x04, -0x2C,  // pattern, y, x
    (signed char) 0x00, +0x04, -0x14,  // pattern, y, x
    (signed char) 0xFF, +0x04, -0x1C,  // pattern, y, x
    (signed char) 0xFF, -0x08, -0x24,  // pattern, y, x
    (signed char) 0xFF, -0x1C, -0x1C,  // pattern, y, x
    (signed char) 0xFF, -0x30, -0x10,  // pattern, y, x
    (signed char) 0xFF, -0x24, +0x08,  // pattern, y, x
    (signed char) 0xFF, -0x18, +0x18,  // pattern, y, x
    (signed char) 0xFF, -0x18, +0x28,  // pattern, y, x
    (signed char) 0xFF, +0x08, +0x30,  // pattern, y, x
    (signed char) 0xFF, +0x1C, +0x20,  // pattern, y, x
    (signed char) 0xFF, +0x24, +0x14,  // pattern, y, x
    (signed char) 0xFF, +0x30, -0x08,  // pattern, y, x
    (signed char) 0xFF, +0x1C, -0x18,  // pattern, y, x
    (signed char) 0xFF, +0x10, -0x20,  // pattern, y, x
    (signed char) 0x00, -0x0C, +0x04,  // pattern, y, x
    (signed char) 0xFF, -0x18, -0x10,  // pattern, y, x
    (signed char) 0x00, -0x20, -0x08,  // pattern, y, x
    (signed char) 0xFF, -0x24, +0x08,  // pattern, y, x
    (signed char) 0xFF, -0x0C, +0x08,  // pattern, y, x
    (signed char) 0xFF, -0x10, -0x10,  // pattern, y, x
    (signed char) 0xFF, +0x10, -0x10,  // pattern, y, x
    (signed char) 0xFF, +0x0C, +0x08,  // pattern, y, x
    (signed char) 0xFF, +0x24, +0x08,  // pattern, y, x
    (signed char) 0x00, +0x28, +0x28,  // pattern, y, x
    (signed char) 0xFF, -0x14, -0x14,  // pattern, y, x
    (signed char) 0x00, -0x0C, +0x08,  // pattern, y, x
    (signed char) 0xFF, +0x10, +0x1C,  // pattern, y, x
    (signed char) 0x00, +0x34, +0x2E,  // pattern, y, x
    (signed char) 0x00, +0x34, +0x2E,  // pattern, y, x
    (signed char) 0xFF, -0x10, +0x04,  // pattern, y, x
    (signed char) 0xFF, -0x04, +0x20,  // pattern, y, x
    (signed char) 0x01 // endmarker (high bit in pattern not set)
};

const signed char _audioIcon[]=
{   (signed char) 111, // count of vectors
    (signed char) 0x3C, (signed char) 0x3B, (signed char) 0xD8, (signed char) 0x2C, // y0, x0, y1, x1
    (signed char) 0xD8, (signed char) 0x2C, (signed char) 0xC9, (signed char) 0x20, // y0, x0, y1, x1
    (signed char) 0xC9, (signed char) 0x20, (signed char) 0xD1, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xD1, (signed char) 0x00, (signed char) 0x32, (signed char) 0xEF, // y0, x0, y1, x1
    (signed char) 0x32, (signed char) 0xEF, (signed char) 0x3A, (signed char) 0x02, // y0, x0, y1, x1
    (signed char) 0x3A, (signed char) 0x02, (signed char) 0x3C, (signed char) 0x3B, // y0, x0, y1, x1
    (signed char) 0x31, (signed char) 0x28, (signed char) 0x2F, (signed char) 0xF3, // y0, x0, y1, x1
    (signed char) 0x2F, (signed char) 0xF3, (signed char) 0xD4, (signed char) 0x02, // y0, x0, y1, x1
    (signed char) 0xD4, (signed char) 0x02, (signed char) 0xCE, (signed char) 0x1E, // y0, x0, y1, x1
    (signed char) 0xCE, (signed char) 0x1E, (signed char) 0x31, (signed char) 0x28, // y0, x0, y1, x1
    (signed char) 0xF1, (signed char) 0x14, (signed char) 0xF1, (signed char) 0x0B, // y0, x0, y1, x1
    (signed char) 0xF1, (signed char) 0x0B, (signed char) 0xED, (signed char) 0x07, // y0, x0, y1, x1
    (signed char) 0xED, (signed char) 0x07, (signed char) 0xE4, (signed char) 0x05, // y0, x0, y1, x1
    (signed char) 0xE4, (signed char) 0x05, (signed char) 0xDE, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0xDE, (signed char) 0x09, (signed char) 0xDC, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0xDC, (signed char) 0x12, (signed char) 0xE3, (signed char) 0x19, // y0, x0, y1, x1
    (signed char) 0xE3, (signed char) 0x19, (signed char) 0xEA, (signed char) 0x19, // y0, x0, y1, x1
    (signed char) 0xEA, (signed char) 0x19, (signed char) 0xF1, (signed char) 0x14, // y0, x0, y1, x1
    (signed char) 0xEA, (signed char) 0x10, (signed char) 0xE7, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0xE7, (signed char) 0x12, (signed char) 0xE4, (signed char) 0x11, // y0, x0, y1, x1
    (signed char) 0xE4, (signed char) 0x11, (signed char) 0xE3, (signed char) 0x0E, // y0, x0, y1, x1
    (signed char) 0xE3, (signed char) 0x0E, (signed char) 0xE6, (signed char) 0x0C, // y0, x0, y1, x1
    (signed char) 0xE6, (signed char) 0x0C, (signed char) 0xE9, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0xE9, (signed char) 0x0D, (signed char) 0xEA, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0xED, (signed char) 0x0D, (signed char) 0xE8, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0xE8, (signed char) 0x09, (signed char) 0xE2, (signed char) 0x0A, // y0, x0, y1, x1
    (signed char) 0xE2, (signed char) 0x0A, (signed char) 0xE0, (signed char) 0x11, // y0, x0, y1, x1
    (signed char) 0xE0, (signed char) 0x11, (signed char) 0xE4, (signed char) 0x15, // y0, x0, y1, x1
    (signed char) 0xE4, (signed char) 0x15, (signed char) 0xEC, (signed char) 0x13, // y0, x0, y1, x1
    (signed char) 0xEC, (signed char) 0x13, (signed char) 0xEE, (signed char) 0x0E, // y0, x0, y1, x1
    (signed char) 0xF6, (signed char) 0x0D, (signed char) 0xF3, (signed char) 0x07, // y0, x0, y1, x1
    (signed char) 0xF3, (signed char) 0x07, (signed char) 0xEC, (signed char) 0x02, // y0, x0, y1, x1
    (signed char) 0xEC, (signed char) 0x02, (signed char) 0xE1, (signed char) 0x02, // y0, x0, y1, x1
    (signed char) 0xE1, (signed char) 0x02, (signed char) 0xDC, (signed char) 0x06, // y0, x0, y1, x1
    (signed char) 0xDC, (signed char) 0x06, (signed char) 0xD8, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0xD8, (signed char) 0x0D, (signed char) 0xDC, (signed char) 0x19, // y0, x0, y1, x1
    (signed char) 0xDC, (signed char) 0x19, (signed char) 0xE4, (signed char) 0x1D, // y0, x0, y1, x1
    (signed char) 0xE4, (signed char) 0x1D, (signed char) 0xED, (signed char) 0x1C, // y0, x0, y1, x1
    (signed char) 0xED, (signed char) 0x1C, (signed char) 0xF5, (signed char) 0x14, // y0, x0, y1, x1
    (signed char) 0xF5, (signed char) 0x14, (signed char) 0xF6, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x2D, (signed char) 0x11, (signed char) 0x2A, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0x2A, (signed char) 0x18, (signed char) 0x1E, (signed char) 0x22, // y0, x0, y1, x1
    (signed char) 0x1E, (signed char) 0x22, (signed char) 0x11, (signed char) 0x23, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0x23, (signed char) 0x04, (signed char) 0x1E, // y0, x0, y1, x1
    (signed char) 0x04, (signed char) 0x1E, (signed char) 0xFF, (signed char) 0x15, // y0, x0, y1, x1
    (signed char) 0xFF, (signed char) 0x15, (signed char) 0xFF, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0xFF, (signed char) 0x09, (signed char) 0x04, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x04, (signed char) 0x00, (signed char) 0x11, (signed char) 0xF9, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0xF9, (signed char) 0x19, (signed char) 0xF9, // y0, x0, y1, x1
    (signed char) 0x19, (signed char) 0xF9, (signed char) 0x23, (signed char) 0xFD, // y0, x0, y1, x1
    (signed char) 0x23, (signed char) 0xFD, (signed char) 0x2A, (signed char) 0x04, // y0, x0, y1, x1
    (signed char) 0x2A, (signed char) 0x04, (signed char) 0x2D, (signed char) 0x11, // y0, x0, y1, x1
    (signed char) 0x28, (signed char) 0x12, (signed char) 0x27, (signed char) 0x08, // y0, x0, y1, x1
    (signed char) 0x27, (signed char) 0x08, (signed char) 0x20, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x20, (signed char) 0x00, (signed char) 0x16, (signed char) 0xFE, // y0, x0, y1, x1
    (signed char) 0x16, (signed char) 0xFE, (signed char) 0x0E, (signed char) 0xFF, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xFF, (signed char) 0x04, (signed char) 0x06, // y0, x0, y1, x1
    (signed char) 0x04, (signed char) 0x06, (signed char) 0x02, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0x12, (signed char) 0x05, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0x05, (signed char) 0x18, (signed char) 0x0E, (signed char) 0x1E, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0x1E, (signed char) 0x1D, (signed char) 0x1D, // y0, x0, y1, x1
    (signed char) 0x1D, (signed char) 0x1D, (signed char) 0x25, (signed char) 0x17, // y0, x0, y1, x1
    (signed char) 0x25, (signed char) 0x17, (signed char) 0x28, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0x1F, (signed char) 0x0D, (signed char) 0x1C, (signed char) 0x14, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0x14, (signed char) 0x16, (signed char) 0x17, // y0, x0, y1, x1
    (signed char) 0x16, (signed char) 0x17, (signed char) 0x0F, (signed char) 0x15, // y0, x0, y1, x1
    (signed char) 0x0F, (signed char) 0x15, (signed char) 0x0B, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0x0B, (signed char) 0x12, (signed char) 0x0B, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x0B, (signed char) 0x0D, (signed char) 0x0E, (signed char) 0x07, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0x07, (signed char) 0x17, (signed char) 0x05, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0x05, (signed char) 0x1D, (signed char) 0x08, // y0, x0, y1, x1
    (signed char) 0x1D, (signed char) 0x08, (signed char) 0x1F, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x1A, (signed char) 0x0D, (signed char) 0x17, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0x09, (signed char) 0x13, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0x13, (signed char) 0x09, (signed char) 0x10, (signed char) 0x0C, // y0, x0, y1, x1
    (signed char) 0x10, (signed char) 0x0C, (signed char) 0x10, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0x10, (signed char) 0x10, (signed char) 0x13, (signed char) 0x13, // y0, x0, y1, x1
    (signed char) 0x13, (signed char) 0x13, (signed char) 0x18, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0x18, (signed char) 0x12, (signed char) 0x1A, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x1E, (signed char) 0xE6, (signed char) 0x1D, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0x1D, (signed char) 0xEA, (signed char) 0x17, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0xE6, (signed char) 0x0E, (signed char) 0xE5, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xE5, (signed char) 0x02, (signed char) 0xEC, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xEC, (signed char) 0x01, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0xEA, (signed char) 0x04, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0x04, (signed char) 0xE4, (signed char) 0x0D, (signed char) 0xE1, // y0, x0, y1, x1
    (signed char) 0x0D, (signed char) 0xE1, (signed char) 0x18, (signed char) 0xE2, // y0, x0, y1, x1
    (signed char) 0x18, (signed char) 0xE2, (signed char) 0x1E, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0x22, (signed char) 0xD9, (signed char) 0x1C, (signed char) 0xD5, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0xD5, (signed char) 0x0F, (signed char) 0xD3, // y0, x0, y1, x1
    (signed char) 0x0F, (signed char) 0xD3, (signed char) 0x00, (signed char) 0xD6, // y0, x0, y1, x1
    (signed char) 0x00, (signed char) 0xD6, (signed char) 0xFB, (signed char) 0xDA, // y0, x0, y1, x1
    (signed char) 0xFB, (signed char) 0xDA, (signed char) 0xFA, (signed char) 0xDE, // y0, x0, y1, x1
    (signed char) 0xFA, (signed char) 0xDE, (signed char) 0xFD, (signed char) 0xE0, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0xE0, (signed char) 0x05, (signed char) 0xD9, // y0, x0, y1, x1
    (signed char) 0x05, (signed char) 0xD9, (signed char) 0x15, (signed char) 0xD8, // y0, x0, y1, x1
    (signed char) 0x15, (signed char) 0xD8, (signed char) 0x20, (signed char) 0xDD, // y0, x0, y1, x1
    (signed char) 0x20, (signed char) 0xDD, (signed char) 0x22, (signed char) 0xD9, // y0, x0, y1, x1
    (signed char) 0x29, (signed char) 0xCA, (signed char) 0x22, (signed char) 0xC5, // y0, x0, y1, x1
    (signed char) 0x22, (signed char) 0xC5, (signed char) 0x10, (signed char) 0xC4, // y0, x0, y1, x1
    (signed char) 0x10, (signed char) 0xC4, (signed char) 0x00, (signed char) 0xC6, // y0, x0, y1, x1
    (signed char) 0x00, (signed char) 0xC6, (signed char) 0xF2, (signed char) 0xCC, // y0, x0, y1, x1
    (signed char) 0xF2, (signed char) 0xCC, (signed char) 0xF2, (signed char) 0xCF, // y0, x0, y1, x1
    (signed char) 0xF2, (signed char) 0xCF, (signed char) 0xF4, (signed char) 0xD1, // y0, x0, y1, x1
    (signed char) 0xF4, (signed char) 0xD1, (signed char) 0xF7, (signed char) 0xD1, // y0, x0, y1, x1
    (signed char) 0xF7, (signed char) 0xD1, (signed char) 0xF9, (signed char) 0xCE, // y0, x0, y1, x1
    (signed char) 0xF9, (signed char) 0xCE, (signed char) 0xFE, (signed char) 0xCB, // y0, x0, y1, x1
    (signed char) 0xFE, (signed char) 0xCB, (signed char) 0x1F, (signed char) 0xCA, // y0, x0, y1, x1
    (signed char) 0x1F, (signed char) 0xCA, (signed char) 0x26, (signed char) 0xCE, // y0, x0, y1, x1
    (signed char) 0x26, (signed char) 0xCE, (signed char) 0x29, (signed char) 0xCD, // y0, x0, y1, x1
    (signed char) 0x29, (signed char) 0xCD, (signed char) 0x29, (signed char) 0xCA, // y0, x0, y1, x1
};

const signed char _nullList[]=
{   (signed char) 0, // count of vectors
    (signed char) 0x0, (signed char) 0x0, (signed char) 0x0, (signed char) 0x0, // y0, x0, y1, x1
  
};
const signed char _tailgunnerList[]=
{   (signed char) 39, // count of vectors
    (signed char) 0x18, (signed char) 0x30, (signed char) 0x18, (signed char) 0xCF, // y0, x0, y1, x1
    (signed char) 0x12, (signed char) 0xD4, (signed char) 0xEB, (signed char) 0xD4, // y0, x0, y1, x1
    (signed char) 0xEB, (signed char) 0xD4, (signed char) 0xEB, (signed char) 0x2A, // y0, x0, y1, x1
    (signed char) 0xEB, (signed char) 0x2A, (signed char) 0x12, (signed char) 0x2A, // y0, x0, y1, x1
    (signed char) 0x12, (signed char) 0x2A, (signed char) 0x12, (signed char) 0xD4, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xF0, (signed char) 0x0E, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0xEA, (signed char) 0x0E, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xF8, (signed char) 0x01, (signed char) 0xF3, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0xFC, (signed char) 0x0E, (signed char) 0xF8, // y0, x0, y1, x1
    (signed char) 0x08, (signed char) 0xFB, (signed char) 0x08, (signed char) 0xF6, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0x06, (signed char) 0x01, (signed char) 0x06, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0x0F, (signed char) 0x0E, (signed char) 0x0F, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0x18, (signed char) 0x01, (signed char) 0x0F, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0x1C, (signed char) 0xFD, (signed char) 0x1C, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0x25, (signed char) 0xFD, (signed char) 0x1C, // y0, x0, y1, x1
    (signed char) 0xF7, (signed char) 0x25, (signed char) 0xFD, (signed char) 0x25, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0x25, (signed char) 0xF7, (signed char) 0x24, // y0, x0, y1, x1
    (signed char) 0xF7, (signed char) 0x1C, (signed char) 0xF7, (signed char) 0x25, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0x0F, (signed char) 0xF0, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0x0F, (signed char) 0xF0, (signed char) 0x0F, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0x18, (signed char) 0xFD, (signed char) 0x0F, // y0, x0, y1, x1
    (signed char) 0xF7, (signed char) 0x0F, (signed char) 0xF7, (signed char) 0x13, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0x0A, (signed char) 0xFD, (signed char) 0x0A, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0x01, (signed char) 0xF0, (signed char) 0x0A, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0x01, (signed char) 0xFD, (signed char) 0x01, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0xFC, (signed char) 0xFD, (signed char) 0xFC, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0xF3, (signed char) 0xF0, (signed char) 0xFC, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0xF3, (signed char) 0xFD, (signed char) 0xF3, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0xEF, (signed char) 0xFD, (signed char) 0xEF, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0xE6, (signed char) 0xF0, (signed char) 0xEF, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0xE6, (signed char) 0xF0, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0xF5, (signed char) 0xE0, (signed char) 0xF5, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0xE1, (signed char) 0xF5, (signed char) 0xE1, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0xD8, (signed char) 0xF0, (signed char) 0xE1, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0xD8, (signed char) 0xF0, (signed char) 0xD8, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0xE1, (signed char) 0xFD, (signed char) 0xD8, // y0, x0, y1, x1
    (signed char) 0x18, (signed char) 0xCF, (signed char) 0xE5, (signed char) 0xCF, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0xCF, (signed char) 0xE5, (signed char) 0x30, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x30, (signed char) 0x18, (signed char) 0x30, // y0, x0, y1, x1
};


const signed char _vecxExactList[]=
{   (signed char) 56, // count of vectors
    (signed char) 0x21, (signed char) 0x16, (signed char) 0x22, (signed char) 0x0E, // y0, x0, y1, x1
    (signed char) 0x22, (signed char) 0x0E, (signed char) 0x26, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x10, (signed char) 0xF2, (signed char) 0x0B, (signed char) 0xED, // y0, x0, y1, x1
    (signed char) 0x08, (signed char) 0xEF, (signed char) 0x0C, (signed char) 0xF6, // y0, x0, y1, x1
    (signed char) 0x06, (signed char) 0xE8, (signed char) 0xFD, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0xEA, (signed char) 0xFA, (signed char) 0xEC, // y0, x0, y1, x1
    (signed char) 0xFA, (signed char) 0xEC, (signed char) 0xF6, (signed char) 0xE8, // y0, x0, y1, x1
    (signed char) 0xF6, (signed char) 0xE8, (signed char) 0xFA, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0xFA, (signed char) 0xE4, (signed char) 0xFD, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0xE6, (signed char) 0x06, (signed char) 0xE8, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xEA, (signed char) 0x14, (signed char) 0xEE, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0xED, (signed char) 0x13, (signed char) 0xF5, // y0, x0, y1, x1
    (signed char) 0x13, (signed char) 0xF5, (signed char) 0x0C, (signed char) 0xFB, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0xFB, (signed char) 0x00, (signed char) 0xFD, // y0, x0, y1, x1
    (signed char) 0x00, (signed char) 0xFD, (signed char) 0xF7, (signed char) 0xF8, // y0, x0, y1, x1
    (signed char) 0xF7, (signed char) 0xF8, (signed char) 0xF0, (signed char) 0xF0, // y0, x0, y1, x1
    (signed char) 0xF0, (signed char) 0xF0, (signed char) 0xEE, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0xEE, (signed char) 0xE4, (signed char) 0xF4, (signed char) 0xDA, // y0, x0, y1, x1
    (signed char) 0xF4, (signed char) 0xDA, (signed char) 0xFA, (signed char) 0xD4, // y0, x0, y1, x1
    (signed char) 0xFA, (signed char) 0xD4, (signed char) 0x03, (signed char) 0xD2, // y0, x0, y1, x1
    (signed char) 0x03, (signed char) 0xD2, (signed char) 0x0F, (signed char) 0xD6, // y0, x0, y1, x1
    (signed char) 0x0F, (signed char) 0xD6, (signed char) 0x16, (signed char) 0xDD, // y0, x0, y1, x1
    (signed char) 0x16, (signed char) 0xDD, (signed char) 0x18, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0x18, (signed char) 0xE6, (signed char) 0x17, (signed char) 0xED, // y0, x0, y1, x1
    (signed char) 0x16, (signed char) 0xF2, (signed char) 0x18, (signed char) 0x08, // y0, x0, y1, x1
    (signed char) 0x18, (signed char) 0x08, (signed char) 0xF4, (signed char) 0x08, // y0, x0, y1, x1
    (signed char) 0xF4, (signed char) 0x08, (signed char) 0xF4, (signed char) 0xF5, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0xDA, (signed char) 0x08, (signed char) 0xE0, // y0, x0, y1, x1
    (signed char) 0x0B, (signed char) 0xE2, (signed char) 0x10, (signed char) 0xDD, // y0, x0, y1, x1
    (signed char) 0x14, (signed char) 0xE2, (signed char) 0x0E, (signed char) 0xE5, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xE8, (signed char) 0x14, (signed char) 0xE8, // y0, x0, y1, x1
    (signed char) 0x18, (signed char) 0xE5, (signed char) 0x1E, (signed char) 0xE5, // y0, x0, y1, x1
    (signed char) 0x1E, (signed char) 0xE5, (signed char) 0x22, (signed char) 0xE7, // y0, x0, y1, x1
    (signed char) 0x22, (signed char) 0xE7, (signed char) 0x26, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x26, (signed char) 0x0D, (signed char) 0x24, (signed char) 0x16, // y0, x0, y1, x1
    (signed char) 0x24, (signed char) 0x16, (signed char) 0x21, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0x21, (signed char) 0x18, (signed char) 0xDE, (signed char) 0x19, // y0, x0, y1, x1
    (signed char) 0xDE, (signed char) 0x19, (signed char) 0xDA, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0xDA, (signed char) 0x10, (signed char) 0xEA, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0xE4, (signed char) 0x0A, (signed char) 0xE4, (signed char) 0x06, // y0, x0, y1, x1
    (signed char) 0xE4, (signed char) 0x02, (signed char) 0xE4, (signed char) 0xFF, // y0, x0, y1, x1
    (signed char) 0xEB, (signed char) 0xEF, (signed char) 0xDF, (signed char) 0xF0, // y0, x0, y1, x1
    (signed char) 0xDF, (signed char) 0xF0, (signed char) 0xDD, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0xDB, (signed char) 0x0D, (signed char) 0xDE, (signed char) 0xEC, // y0, x0, y1, x1
    (signed char) 0xDF, (signed char) 0xEF, (signed char) 0xDE, (signed char) 0xE8, // y0, x0, y1, x1
    (signed char) 0xDE, (signed char) 0xE6, (signed char) 0xEE, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0xEC, (signed char) 0xE6, (signed char) 0xEB, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0xEB, (signed char) 0x0D, (signed char) 0xDA, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0xDA, (signed char) 0x0D, (signed char) 0xDA, (signed char) 0x0F, // y0, x0, y1, x1
    (signed char) 0xE7, (signed char) 0x18, (signed char) 0xE3, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0xE3, (signed char) 0x18, (signed char) 0xE0, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0xE0, (signed char) 0x12, (signed char) 0xE5, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x12, (signed char) 0xE7, (signed char) 0x18, // y0, x0, y1, x1
    (signed char) 0xED, (signed char) 0x18, (signed char) 0xEB, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0xEB, (signed char) 0x10, (signed char) 0x21, (signed char) 0x0E, // y0, x0, y1, x1
    (signed char) 0x22, (signed char) 0x0D, (signed char) 0x1E, (signed char) 0xE6, // y0, x0, y1, x1
};

const signed char _gravitarList[]=
{   (signed char) 112, // count of vectors
    (signed char) 0x01, (signed char) 0xE4, (signed char) 0x03, (signed char) 0xE9, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xEA, (signed char) 0x02, (signed char) 0xEE, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xF2, (signed char) 0x03, (signed char) 0xF7, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xFB, (signed char) 0x02, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0x00, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0xDE, (signed char) 0xFE, (signed char) 0xD9, // y0, x0, y1, x1
    (signed char) 0x0F, (signed char) 0xDA, (signed char) 0x11, (signed char) 0xDE, // y0, x0, y1, x1
    (signed char) 0x09, (signed char) 0xD6, (signed char) 0x0F, (signed char) 0xDA, // y0, x0, y1, x1
    (signed char) 0x0A, (signed char) 0xD9, (signed char) 0x09, (signed char) 0xD6, // y0, x0, y1, x1
    (signed char) 0x06, (signed char) 0xD8, (signed char) 0x0A, (signed char) 0xD9, // y0, x0, y1, x1
    (signed char) 0x05, (signed char) 0xD2, (signed char) 0x06, (signed char) 0xD8, // y0, x0, y1, x1
    (signed char) 0x12, (signed char) 0xD8, (signed char) 0x05, (signed char) 0xD2, // y0, x0, y1, x1
    (signed char) 0x14, (signed char) 0xDF, (signed char) 0x12, (signed char) 0xD8, // y0, x0, y1, x1
    (signed char) 0x19, (signed char) 0xE1, (signed char) 0x14, (signed char) 0xDF, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0xD6, (signed char) 0x19, (signed char) 0xE1, // y0, x0, y1, x1
    (signed char) 0xFA, (signed char) 0xCA, (signed char) 0x17, (signed char) 0xD6, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0xFA, (signed char) 0xCA, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0xE4, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0xE6, (signed char) 0x01, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0xE4, (signed char) 0x0C, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0xE5, (signed char) 0x0C, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0xE7, (signed char) 0x11, (signed char) 0xE5, // y0, x0, y1, x1
    (signed char) 0x14, (signed char) 0xE4, (signed char) 0x11, (signed char) 0xE7, // y0, x0, y1, x1
    (signed char) 0x00, (signed char) 0xDF, (signed char) 0x14, (signed char) 0xE4, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x00, (signed char) 0xDF, // y0, x0, y1, x1
    (signed char) 0x00, (signed char) 0xDF, (signed char) 0xFF, (signed char) 0xDB, // y0, x0, y1, x1
    (signed char) 0xFA, (signed char) 0xCA, (signed char) 0xFE, (signed char) 0xD9, // y0, x0, y1, x1
    (signed char) 0xFE, (signed char) 0xD9, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x02, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0x09, (signed char) 0x03, (signed char) 0x04, // y0, x0, y1, x1
    (signed char) 0x03, (signed char) 0x00, (signed char) 0x0C, (signed char) 0x03, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0xFD, (signed char) 0x1C, (signed char) 0xF9, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0xFC, (signed char) 0x1C, (signed char) 0xFD, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0xFE, (signed char) 0x11, (signed char) 0xFC, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0x00, (signed char) 0x0C, (signed char) 0xFE, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0xFF, (signed char) 0x11, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0x01, (signed char) 0x1C, (signed char) 0xFF, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0x03, (signed char) 0x1C, (signed char) 0x01, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0xF9, (signed char) 0x03, (signed char) 0xF7, // y0, x0, y1, x1
    (signed char) 0x03, (signed char) 0xF7, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x02, (signed char) 0xEE, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xEE, (signed char) 0x0E, (signed char) 0xF0, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xF0, (signed char) 0x0E, (signed char) 0xF2, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0xF2, (signed char) 0x12, (signed char) 0xF3, // y0, x0, y1, x1
    (signed char) 0x12, (signed char) 0xF3, (signed char) 0x12, (signed char) 0xF2, // y0, x0, y1, x1
    (signed char) 0x12, (signed char) 0xF2, (signed char) 0x15, (signed char) 0xF4, // y0, x0, y1, x1
    (signed char) 0x15, (signed char) 0xF4, (signed char) 0x03, (signed char) 0xF2, // y0, x0, y1, x1
    (signed char) 0x03, (signed char) 0xF2, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0xFE, (signed char) 0xDB, // y0, x0, y1, x1
    (signed char) 0xFE, (signed char) 0xDB, (signed char) 0x19, (signed char) 0xE1, // y0, x0, y1, x1
    (signed char) 0x19, (signed char) 0xE1, (signed char) 0x1B, (signed char) 0xE6, // y0, x0, y1, x1
    (signed char) 0x1B, (signed char) 0xE6, (signed char) 0x14, (signed char) 0xEC, // y0, x0, y1, x1
    (signed char) 0x14, (signed char) 0xEC, (signed char) 0x0F, (signed char) 0xEB, // y0, x0, y1, x1
    (signed char) 0x0F, (signed char) 0xEB, (signed char) 0x0D, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0x0D, (signed char) 0xEA, (signed char) 0x0C, (signed char) 0xEB, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0xEB, (signed char) 0x03, (signed char) 0xE9, // y0, x0, y1, x1
    (signed char) 0x03, (signed char) 0xE9, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x01, (signed char) 0x1C, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0x1C, (signed char) 0x02, (signed char) 0x17, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0x1B, (signed char) 0x0C, (signed char) 0x19, // y0, x0, y1, x1
    (signed char) 0x03, (signed char) 0x12, (signed char) 0x17, (signed char) 0x10, // y0, x0, y1, x1
    (signed char) 0x1B, (signed char) 0x12, (signed char) 0x1C, (signed char) 0x08, // y0, x0, y1, x1
    (signed char) 0x15, (signed char) 0x13, (signed char) 0x1B, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0x10, (signed char) 0x15, (signed char) 0x13, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0x08, (signed char) 0x03, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0x00, (signed char) 0x20, (signed char) 0xFF, (signed char) 0x25, // y0, x0, y1, x1
    (signed char) 0xFF, (signed char) 0x25, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x02, (signed char) 0x12, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0x12, (signed char) 0x02, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0x0D, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0x0B, (signed char) 0x02, (signed char) 0x0D, // y0, x0, y1, x1
    (signed char) 0x17, (signed char) 0x09, (signed char) 0x17, (signed char) 0x0B, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0x08, (signed char) 0x17, (signed char) 0x09, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0x03, (signed char) 0x1C, (signed char) 0x08, // y0, x0, y1, x1
    (signed char) 0x03, (signed char) 0x04, (signed char) 0x1C, (signed char) 0x03, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x03, (signed char) 0x04, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xFC, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0xF8, (signed char) 0x02, (signed char) 0xFC, // y0, x0, y1, x1
    (signed char) 0x1C, (signed char) 0xF9, (signed char) 0x0C, (signed char) 0xF8, // y0, x0, y1, x1
    (signed char) 0x1B, (signed char) 0xF3, (signed char) 0x1C, (signed char) 0xF9, // y0, x0, y1, x1
    (signed char) 0x14, (signed char) 0xED, (signed char) 0x1B, (signed char) 0xF3, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xEA, (signed char) 0x14, (signed char) 0xED, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x02, (signed char) 0xEA, // y0, x0, y1, x1
    (signed char) 0xFF, (signed char) 0x20, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x14, (signed char) 0x1C, (signed char) 0xFF, (signed char) 0x20, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0x19, (signed char) 0x14, (signed char) 0x1C, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0x1C, (signed char) 0x11, (signed char) 0x19, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0x1C, (signed char) 0x11, (signed char) 0x1C, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0x19, (signed char) 0x0C, (signed char) 0x1C, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0xFE, (signed char) 0x26, // y0, x0, y1, x1
    (signed char) 0xFE, (signed char) 0x26, (signed char) 0x19, (signed char) 0x1E, // y0, x0, y1, x1
    (signed char) 0x19, (signed char) 0x1E, (signed char) 0x18, (signed char) 0x25, // y0, x0, y1, x1
    (signed char) 0x18, (signed char) 0x25, (signed char) 0x11, (signed char) 0x2C, // y0, x0, y1, x1
    (signed char) 0x11, (signed char) 0x2C, (signed char) 0x0C, (signed char) 0x2E, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0x2E, (signed char) 0x0C, (signed char) 0x2D, // y0, x0, y1, x1
    (signed char) 0x0C, (signed char) 0x2D, (signed char) 0xFA, (signed char) 0x35, // y0, x0, y1, x1
    (signed char) 0xFA, (signed char) 0x35, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0xFD, (signed char) 0x2B, // y0, x0, y1, x1
    (signed char) 0xFD, (signed char) 0x2B, (signed char) 0x12, (signed char) 0x24, // y0, x0, y1, x1
    (signed char) 0x12, (signed char) 0x24, (signed char) 0x0F, (signed char) 0x27, // y0, x0, y1, x1
    (signed char) 0x0F, (signed char) 0x27, (signed char) 0x0E, (signed char) 0x26, // y0, x0, y1, x1
    (signed char) 0x0E, (signed char) 0x26, (signed char) 0x0B, (signed char) 0x27, // y0, x0, y1, x1
    (signed char) 0x0B, (signed char) 0x27, (signed char) 0x09, (signed char) 0x2B, // y0, x0, y1, x1
    (signed char) 0x09, (signed char) 0x2B, (signed char) 0xFC, (signed char) 0x30, // y0, x0, y1, x1
    (signed char) 0xFC, (signed char) 0x30, (signed char) 0xE5, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xE5, (signed char) 0x00, (signed char) 0x02, (signed char) 0x17, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0x17, (signed char) 0x12, (signed char) 0x14, // y0, x0, y1, x1
    (signed char) 0x12, (signed char) 0x14, (signed char) 0x1A, (signed char) 0x1A, // y0, x0, y1, x1
    (signed char) 0x1A, (signed char) 0x1A, (signed char) 0x1A, (signed char) 0x1D, // y0, x0, y1, x1
    (signed char) 0x1A, (signed char) 0x1D, (signed char) 0xFE, (signed char) 0x25, // y0, x0, y1, x1
    (signed char) 0xFE, (signed char) 0x26, (signed char) 0xFD, (signed char) 0x2B, // y0, x0, y1, x1
    (signed char) 0xFB, (signed char) 0x30, (signed char) 0xFA, (signed char) 0x35, // y0, x0, y1, x1
};

const signed char *exampleLists[]=
{
  _tailgunnerList,
  _vecxExactList,
  _gravitarList,
  _audioIcon,
  _nullList,
  0
};

void displayHelp(char *textLines[], int direct)
{
  if (textLines == 0) return;

  int startLine = 0;
  int f1Request = 0;
  int starting = 1;
  
  
  while (1)
  {
    int line = 1;
    int y = 100;
//    if (direct)
//      v_WaitRecal_direct();
//    else
      v_WaitRecal();
    
    v_readButtons();
    v_readJoystick1Analog();
    v_setBrightness(0x40);
    v_printStringRasterFont(-(strlen(textLines[0])*5), y, textLines[0], 50, -5, 0, alternate_rasterlines);
    y-=20;
    y-=20;
    
    while (textLines[line+startLine] != 0)
    {
      v_printStringRasterFont(-90, y, textLines[line+startLine], 40, -3, 0, alternate_raster5);
      y-=20;
      line++;
      if (line >6) break;
    }
    
    v_printStringRasterFont(-20, -80, "BUTTON 1", 40, -3, 0, alternate_raster5);
    if ((internalButtonState&0x0f) == (0x01)) // exactly button 1
    {
      if (!starting)
        f1Request =1;
    }
    else
    {
      if (f1Request)
      {
        if ((line >6) && (textLines[line+startLine] != 0))
        {
          startLine+=6;
        }
        else
        {
          break;
        }
      }
      f1Request =0;
      starting = 0;
    }
  }
}
char *calibrationHelp[]=
{
  "CALIBRATION", // title
  "IF VECTORS ARE DISPLAYED SKEWED,",
  "TRY USE CALIBRATION TO IMPROVE",
  "THE LOOK.",
  "BUTTON 3 TO DISPLAY DIFFERENT",
  "VECTORLISTS.",
  "(NON ZERO SLIGHTLY SLOWER)",
  0
};
char *delayXHelp[]=
{
  "DELAY AFTER XSH", // title
  "CRANKY VECTREX MIGHT NEED",
  "SOME TWEAKING HERE.",
  "   ",
  "- BUT PROBABLY NOT -",
  "(THE LOWER THE FASTER)",
  0
};
char *delayYHelp[]=
{
  "DELAY AFTER YSH", // title
  "CRANKY VECTREX NEED TWEAKING",
  "THIS. IF SINGLE VECTORS",
  "IN A CLUSTER 'STICK OUT' THAN",
  "THIS MIGHT HELP.",
  "(THE LOWER THE FASTER)",
  0
};
char *delayBbeforeAHelp[]=
{
  "DELAY B BEFORE A", // title
  "CRANKY VECTREX NEED TWEAKING",
  "THIS. IF SINGLE VECTORS",
  "IN A CLUSTER 'STICK OUT' THAN",
  "THIS MIGHT HELP.",
  "(THE LOWER THE FASTER)",
  0
};
char *delayYto0Help[]=
{
  "DELAY Y TO NULL", // title
  "CRANKY VECTREX NEED TWEAKING",
  "THIS. IF SINGLE VECTORS",
  "IN A CLUSTER 'STICK OUT' THAN",
  "THIS MIGHT HELP.",
  "(THE LOWER THE FASTER)",
  0
};
char *maxStrengthHelp[]=
{
  "MAX STRENGTH", // title
  "CRANKY VECTREX NEED TWEAKING!",
  "CHANGING HIGH VALUES TO ",
  "NEGATIVE HIGH VALUES ",
  "CAN 'CONFUSE' INTEGRATORS.",
  "    ",
  "    ",
  "THIS CAN BE PREVENTED BY NOT",
  "USING HIGH VALUES AT ALL...",
  "(THE HIGHER THE FASTER)",
  0
};

char *delayZEROHelp[]=
{
  "DELAY ZERO", // title
  "A FIXED NUMBER OF",
  "VECTREX CYCLES FOR THE",
  "ZEROING TO SETTLE.",
  "    ",
  "- SUBJECT TO CHANGE -",
  "(THE LOWER THE FASTER)",
  0
};

char *delayAFTERT1Help[]=
{
  "DELAY AFTER T1", // title
  "POSITIONING IS DONE USING",
  "VIA TIMER 1. T1 CONTROLS",
  "THE RAMP SIGNAL. HOWEVER",
  "REACTING IMMEDIATELY TO",
  "AN EXPIRED T1 IS TO",
  "'EARLY'!",
  "THESE ARE THE CYCLES TO",
  "ADDITIONALLY WAIT BEFORE",
  "CONTINUING.",
  "(THE LOWER THE FASTER)",
  0
};

char *scaleStrengthHelp[]=
{
  "SCALE STRENGTH DIF", // title
  "A CONSTANT USED AS A",
  "CORRECTION VALUE TO",
  "CALCULATE POSITIONING WITH",
  "STRENGTH AND SCALE.",
  "(NO SPEED INFLUENCE)",
  0
};
char *MINSCALEHelp[]=
{
  "MINSCALE", // title
  "THE MINIMUM SCALE THAT",
  "MIGHT BE USED INTERNALLY.",
  "(THE LOWER THE FASTER)",
  0
};

char *orientationHelp[]=
{
  "ORIENTATION", // title
  "TRY IT AND SEE FOR YOURSELF!",
  "(NO SPEED INFLUENCE)",
  0
};
char *pipelinTypeHelp[]=
{
  "PIPELINE TYPE", // title
  "DON'T BOTHER.",
  "THIS VALUE MUST BE SET",
  "PER INDIVIDUAL PROGRAM.",
  "(NO SPEED INFLUENCE)",
  0
};

char *doubleTimerHelp[]=
{
  "DOUBLE TIMER", // title
  "POSITIONING IS MORE EXACT.",
  "FOR NON MOVING OBJECTS, THIS",
  "DOES NOT MATTER.",
  "FAST MOVING OBJECT MIGHT NEED",
  "THIS ENABLED.",
  "(THE LOWER THE FASTER)",
  0
};

char *dotsOrderedHelp[]=
{
  "KEEP DOTS ORDERED", // title
  "THE PIPELINE PER DEFAULT",
  "HANDLES ALL DOTS 'LAST'.",
  "SOME PROGRAMS NEED THIS FOR",
  "GOOD DISPLAY, SOME THE OTHER",
  "WAY AROUND.",
  "(NO SPEED INFLUENCE)",
  0
};

char *beamOffInDrawHelp[]=
{
  "BEAM OFF IN DRAWS", // title
  "WHILE DRAWING A CLUSTER OF",
  "VECTORS, THE LIGHT CAN BE LEFT",
  "ON, OR BE SWITCHED OFF.",
  "   ",
  "   ",
  "   ",
  "LEAVING 'ON' RESULTS IN",
  "SLIGHTLY MORE DOTTED LINES.",
  "(THE LOWER THE FASTER)",
  0
};

char *cycleEquiHelp[]=
{
  "CYCLE EQUIVALENZ", // title
  "HOW MANY PI CYCLES REPRESENT",
  "ONE VECTREX CYCLE.",
  "WEIRD THING CAN HAPPEN IF",
  "CHANGED.",
  "(THE LOWER THE FASTER)",
  0
};

char *zeroBorderHelp[]=
{
  "ZERO BORDER", // title
  "AT WHAT DISTANCE BETWEEN",
  "DRAWS THE PIPELINE WILL",
  "AUTOMATICALLY INSERT A ",
  "ZERO REF.",
  "(THE HIGHER CAN BE FASTER)",
  0
};

char *irqModeHelp[]=
{
  "IRQ MODE", // title
  "SWITCH THE OUTPUT ROUTINES TO",
  "BE IRQ DRIVEN, THIS ENABLES",
  "A MULTITASKING OF SORTS. ",
  "ONLY ACTIVATED WHEN LEAVING",
  "THE SETTINGS MENU.",
  0
};
char *vectrexHzHelp[]=
{
  "VECTREX HZ", // title
  "REFRESH FREQUENCY OF",
  "THE VECTREX OUTPUT.",
  "SHOULD BE 50HZ FOR A ",
  "CLEAN DISPLAY.",
  0
};
char *appHzHelp[]=
{
  "APP HZ", // title
  "REFRESH FREQUENCY OF",
  "THE PROGRAM RUNNING.",
  "ONLY ACTIVE WITH IRQ MODE.",
  "VALUE 0 SYNCS WITH VECTREX",
  "DISPLAY FREQUENCY",
  0
};

char *xOffsetHelp[]=
{
  "X OFFSET", // title
  "POSITIOING ON SCREEN.",
  "HORIZONTALLY",
  "(NO SPEED INFLUENCE)",
  0
};

char *yOffsetHelp[]=
{
  "Y OFFSET", // title
  "POSITIOING ON SCREEN.",
  "VERTICALLY",
  "(NO SPEED INFLUENCE)",
  0
};

char *xSizeHelp[]=
{
  "X SIZE", // title
  "HORIZONTAL SIZE FACTOR.",
  "(NO SPEED INFLUENCE)",
  0
};

char *ySizeHelp[]=
{
  "Y SIZE", // title
  "VERTICAL SIZE FACTOR.",
  "(NO SPEED INFLUENCE)",
  0
};

char *frequencyHelp[]=
{
  "TEST FRQ", // title
  "PLAYTHING - SET THE",
  "REFRESH VALUE OF VECTREX.",
  "   ",
  "ATTENTION:",
  "THIS WILL BE SAVED!",
  0
};
char *pipelinUseHelp[]=
{
  "USE PIPELINE", // title
  "ALWAYS!!!",
  "USE THE PIPELINE!",
  0
};

unsigned int vecHz;
unsigned int appHz;

CalibrationItems calItems[] =
{
  {&calibrationValue,0,0,0,0                    ,0,255,0,"CALIBRATION", CAL_TYPE_UCHAR, calibrationHelp},
  {&DELAY_AFTER_XSH_VALUE,0,0,0,0               ,0,10,2, "DELAY AFTER XSH", CAL_TYPE_UCHAR, delayXHelp },
  {&DELAY_AFTER_YSH_VALUE,0,0,0,0               ,0,20,4, "DELAY AFTER YSH", CAL_TYPE_UCHAR, delayYHelp },
  {&DELAY_PORT_B_BEFORE_PORT_A_VALUE,0,0,0,0    ,0,20,2, "DELAY B BEFORE A", CAL_TYPE_UCHAR, delayBbeforeAHelp }, // yes the same applies here!
  {&CRANKY_DELAY_Y_TO_NULL_VALUE,0,0,0,0        ,0,20,6, "DELAY Y TO NULL", CAL_TYPE_UCHAR, delayYto0Help }, // and here!
  {&MAX_USED_STRENGTH,0,0,0,0                   ,1,127,90, "MAX STRENGTH", CAL_TYPE_UCHAR, maxStrengthHelp},
//  {&MAX_CONSECUTIVE_DRAWS,0,0,0,0               ,1,255,60, "MAX_CONSECUTIVE_DRAWS", CAL_TYPE_UCHAR},
  {&DELAY_ZERO_VALUE,0,0,0,0                    ,0,100,30, "DELAY ZERO", CAL_TYPE_UCHAR, delayZEROHelp },
  {&DELAY_AFTER_T1_END_VALUE,0,0,0,0            ,0,50,16, "DELAY AFTER T1", CAL_TYPE_UCHAR, delayAFTERT1Help },
  {&SCALE_STRENGTH_DIF,0,0,0,0                  ,0,10,2, "SCALE STRENGTH DIF", CAL_TYPE_UCHAR, scaleStrengthHelp },
  {&MINSCALE,0,0,0,0                            ,1,20,1, "MINSCALE", CAL_TYPE_UCHAR, MINSCALEHelp },
  {&bufferType,0,0,0,0                          ,0,2,2, "PIPELINE TYPE", CAL_TYPE_UCHAR, pipelinTypeHelp },
  {&orientation,0,0,0,0                         ,0,3,0, "ORIENTATION", CAL_TYPE_UCHAR, orientationHelp },
//  {&optimizationON,0,0,0,0                            ,0,1,1, "OPTIMIZATION", CAL_TYPE_BOOL},
  {&usePipeline,0,0,0,0                         ,0,1,1, "USE PIPELINE", CAL_TYPE_BOOL, pipelinUseHelp },
  {&useDoubleTimer,0,0,0,0                      ,0,1,0, "DOUBLE TIMER", CAL_TYPE_BOOL, doubleTimerHelp },
  {&keepDotsTogether,0,0,0,0                    ,0,1,0, "KEEP DOTS ORDERED", CAL_TYPE_BOOL, dotsOrderedHelp },
  {&beamOffBetweenConsecutiveDraws,0,0,0,0      ,0,1,0, "BEAM OFF IN DRAWS", CAL_TYPE_BOOL, beamOffInDrawHelp },


  {&wasIRQMode,0,0,0,0                          ,0,1,1, "IRQ MODE", CAL_TYPE_BOOL, irqModeHelp },
  {0,0,&vecHz,0,0                               ,20,100,50, "VECTREX HZ", CAL_TYPE_UINT, vectrexHzHelp },
  {0,0,&appHz,0,0                               ,0,100,0, "APP HZ", CAL_TYPE_UINT, appHzHelp },


  {0,0,&cycleEquivalent,0,0                     ,0,1000,666, "CYCLE EQUIVALENZ", CAL_TYPE_UINT, cycleEquiHelp },
  {0,0,&resetToZeroDifMax,0,0                   ,0,10000,2500, "ZERO BORDER", CAL_TYPE_UINT, zeroBorderHelp },
  {0,0,0,&offsetX,0                             ,-20000,+20000,0, "X OFFSET", CAL_TYPE_SINT, xOffsetHelp },
  {0,0,0,&offsetY,0                             ,-20000,+20000,0, "Y OFFSET", CAL_TYPE_SINT, yOffsetHelp },

  {0,0,0,0,&sizeX                               ,0,2,1, "X SIZE", CAL_TYPE_FLOAT, xSizeHelp },
  {0,0,0,0,&sizeY                               ,0,2,1, "Y SIZE", CAL_TYPE_FLOAT, ySizeHelp },
  
  {0,0,&biRasterReturnDelayLeft,0,0                 ,0,20,7, "RASTER LEFT DELAY", CAL_TYPE_UINT, cycleEquiHelp },
  {0,0,&biRasterReturnDelayRight,0,0                 ,0,20,7, "RASTER RIGHT DELAY", CAL_TYPE_UINT, cycleEquiHelp },
  {0,0,&base_biRasterDown,0,0                   ,0,20,6, "RASTER DOWN BASE", CAL_TYPE_UINT, cycleEquiHelp },
  {0,0,0,0,&biRasterDownDriftModifier           ,-2,2,0, "RASTER DOWN MODIFIER", CAL_TYPE_FLOAT, ySizeHelp },

  {0,0,0,0,0,0,0,0,0,CAL_TYPE_NONE}
};

/*
 * 

 
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
 unsigned int Vec_Rfrsh; // 30000 cylces (vectrex) = $7530, little endian = $3075
CrankyFlags crankyFlag; // cranky should be checked during calibration! In "VecFever" terms cranky off = burst modus
*/
void _displayLargeList(signed char lx, signed char ly, const signed char list[])
{
  int count = *list++;

  while (count >0)  
  {
    int y0 = ly + *list++;
    int x0 = lx + *list++;
    int y1 = ly + *list++;
    int x1 = lx + *list++;
    v_directDraw32((x0<<7), (y0<<7),(x1<<7),(y1<<7), 0x5f);
    count --;
  }
}

/*    
press "2" to change example list
press and hold X axis does ten times number change

press and HOLD 3 zo move sample list
press 4 tp exit
joystick up down to cjose settings type
joystick left right to change setting

button 2+3 at the same time resets to default

button 1 for help

on exit ASK for save as "ini"
*/

void v_SettingsGUI(int ownWaitRecal)
{
  static signed char lx = 0,ly = 0; 
  static int sel=0;
  static int lastButton = 0;
  static int doingSomething = 0;
  int oldCommenHints = commonHints;
  commonHints = oldCommenHints;
  char buf[256];
  int listSel = 0;

  vecHz = 1500000 / (((Vec_Rfrsh&0xff)*256) + ((Vec_Rfrsh>>8)&0xff));
  appHz = clientHz;
  
  if (ownWaitRecal)
  {
    if (wasIRQMode == 255)
    {
      wasIRQMode = isIRQMode;
      lastButton = internalButtonState;
      if (wasIRQMode==1)
        v_removeIRQHandling();  
    }
  }
  else
  {
    clipMode = 1; // 0 normal clipping, 1 = inverse clipping
    clipActive = 1;

    clipminX=-8000;
    clipmaxX= 9000;
    clipminY= 6500;
    clipmaxY= 16000;
    commonHints = PL_BASE_FORCE_NOT_CLIPPED | PL_BASE_FORCE_STABLE;
  }
  v_noSound();

  do 
  {
    if (ownWaitRecal)
    {
      v_WaitRecal();
    }
    v_setBrightness(0x40);
    v_readButtons();
    v_readJoystick1Analog();
    if (ownWaitRecal) v_printString(-100, 100, "PITREX", 5, 0x3f);
    
    int x = -40;
    int y = 90;
    
    switch (calItems[sel].type)
    {
      case CAL_TYPE_BOOL:
      case CAL_TYPE_UCHAR:
      case CAL_TYPE_UINT:
      case CAL_TYPE_SINT:
      case CAL_TYPE_FLOAT:
      {
        v_immediateWidgetSlider(&calItems[sel], x, y, (sel) == sel);
        break;
      }
      
      default:
          break;
    }
    sprintf(buf,"%i", (roundCycles / 666));
    v_printStringRaster(-10, 70, buf, 30, -2, 0);
    
    if (calItems[sel].uivalue == &vecHz)
    {
      int32_t cycles = 1500000 / vecHz; // Hz to vectrex cycles
      Vec_Rfrsh = ((cycles&0xff)*256) + ((cycles>>8)&0xff); // swap hi / lo bytes for little endian VIA
    }
    
    if (calItems[sel].uivalue == &appHz)
    {
      if (appHz == 0) isSynchronizedWithIRQ = 1; else isSynchronizedWithIRQ = 0;
      clientHz = appHz;
    }
    
    if (ownWaitRecal)
    {
      _displayLargeList(lx,ly,exampleLists[listSel]);
      v_printString(-80, -80, "SOME STRING LIKE IN ZBLAST OR SO!", 5, 0x30);
    }

    if ((internalButtonState&0x0f) == (0x04)) // exactly button 3
    {
      if (internalJoy1Y<-80)
      {
          ly--;
      }
      if (internalJoy1Y>80)
      {
          ly++;
      }
      if (internalJoy1X<-80)
      {
          lx--;
      }
      if (internalJoy1X>80)
      {
          lx++;
      }
    }
    else
    {
      if (lastButton != internalButtonState)
      {
        if ((internalButtonState&0x0f) == (0x08)) // exactly button 4
        {
          while ((v_directReadButtons()&0x0f) == (0x08));

          inCalibration = 0;
          clipActive = 0;
          if ((wasIRQMode==1) && (!ownWaitRecal))
          {
            wasIRQMode = 255;
            v_setupIRQHandling();  
          }
          break;
        }

        if ( (internalButtonState&0x0f) == (0x02)) // exactly button 2
        {
          listSel++;
          if (exampleLists[listSel] == 0) listSel = 0;
        }
        
        if ((internalButtonState&0x0f) == (0x01)) // exactly button 1
        {
          while ((v_directReadButtons()&0x0f) == (0x01));
          displayHelp(calItems[sel].help,1);
        }
      }
      lastButton = internalButtonState;
    
      
      if ((ABS(internalJoy1X)<20) && (ABS(internalJoy1Y)<20)) 
        doingSomething =0;

      if (doingSomething == 0)
      {
        if (internalJoy1Y<-80)
        {
          if (calItems[sel+1].type != CAL_TYPE_NONE)
          {
            sel++;
          }
          doingSomething=1;
        }
        if (internalJoy1Y>80)
        {
          if (sel > 0)
            sel--;
          doingSomething=1;
        }
      }
    }
    if (ownWaitRecal)
    {
      v_directDraw32(-15000, 15000,-15000,14000, 0x5f);
      v_directDraw32(-15000,14000,-14000,14000, 0x5f);
      v_directDraw32(-14000,14000,-14000,15000, 0x5f);

      v_directDraw32(-15000,-15000,-15000,-14000, 0x5f);
      v_directDraw32(-15000,-14000,-14000,-14000, 0x5f);
      v_directDraw32(-14000,-14000,-14000,-15000, 0x5f);

      v_directDraw32(-12000, 14000,-12000,15000, 0x5f);
      v_directDraw32(-12000, 15000,-13000,15000, 0x5f);
      v_directDraw32(-13000, 15000,-13000,14000, 0x5f);
    }
  }while(ownWaitRecal);

  if (ownWaitRecal)
  {
    if (wasIRQMode==1)
      v_setupIRQHandling();  
    wasIRQMode = 255;
  }

  commonHints = oldCommenHints;
}


void v_saveIni(char *filename)
{
  FILE *iniOut=0;
  iniOut = fopen(filename, "w"); 
  if (iniOut == 0) 
  {
    v_message("FILE ERROR, INI NOT SAVED!");
    return;
  }

  fprintf(iniOut,"; Global Vectrex Interface ini file for PiTrex baremetal\n"); 
 
  fprintf(iniOut,"; This file must not be larger then 10kb!\n"); 
  fprintf(iniOut,"[VECTERX_INTERFACE]\n"); 
  fprintf(iniOut,"\n"); 
  fprintf(iniOut,"; DELAY_ZERO_VALUE: the value that is 'waited' before a next move/draw\n"); 
  fprintf(iniOut,"; after a zeroRef was done\n"); 
  fprintf(iniOut,"; if screen looks ok, than the smaller the value - the faster\n"); 
  fprintf(iniOut,"DELAY_ZERO_VALUE = %i;  \n", DELAY_ZERO_VALUE); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; CRANKY_FLAG: \n"); 
  fprintf(iniOut,";typedef enum {\n"); 
  fprintf(iniOut,";  CRANKY_CRANKY_VIA_B_DELAY_B0 = 0,   \n"); 
  fprintf(iniOut,";  CRANKY_CRANKY_VIA_B_DELAY_B1 = 0, \n"); 
  fprintf(iniOut,";  CRANKY_CRANKY_VIA_B_DELAY_B2 = 0,  \n"); 
  fprintf(iniOut,";  CRANKY_CRANKY_VIA_B_DELAY_B3 = 0, \n"); 
  fprintf(iniOut,";  CRANKY_BETWEEN_VIA_B = 16,       < uses above delay\n"); 
  fprintf(iniOut,";  CRANKY_NULLING_WAIT = 32,        < uses fixed '6' cycles\n"); 
  fprintf(iniOut,";  CRANKY_NULLING_CALIBRATE = 64,   < // does a calibrate after each 'null' of Y\n"); 
  fprintf(iniOut,";  CRANKY_T1_DELAYED = 128          < // not implemented\n"); 
  fprintf(iniOut,";} CrankyFlags;\n"); 
  fprintf(iniOut,"\n"); 
  fprintf(iniOut,"; 0x16 = 22 cranky delay of 6 cycles - this is for MY worst vectrex\n"); 
  fprintf(iniOut,"; 0x14 = 20 cranky delay of 4 cycles - \n"); 
  fprintf(iniOut,"; the smaller the faster\n"); 
  fprintf(iniOut,"CRANKY_FLAG = %i \n", crankyFlag); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; DELAY_AFTER_T1_END_VALUE: \n"); 
  fprintf(iniOut,"; changes from vectrex to vectrex, the less, the faster\n"); 
  fprintf(iniOut,"; values go between 17 - 22\n"); 
  fprintf(iniOut,"; STO\n"); 
  fprintf(iniOut,"DELAY_AFTER_T1_END_VALUE = %i \n", DELAY_AFTER_T1_END_VALUE); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; SCALE_STRENGTH_DIF: \n"); 
  fprintf(iniOut,"; 'correction' cycles with a conversion from scale to strength\n"); 
  fprintf(iniOut,"; SSS\n"); 
  fprintf(iniOut,"SCALE_STRENGTH_DIF = %i;\n", SCALE_STRENGTH_DIF); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; BUFFER_TYPE\n"); 
  fprintf(iniOut,"; buffer type\n"); 
  fprintf(iniOut,"; 0 = no buffer, 1 = doublebuffer, 2 = autobuffer, 3 = no display\n"); 
  fprintf(iniOut,"BUFFER_TYPE = %i;\n", bufferType); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; MAX_USED_STRENGTH: \n"); 
  fprintf(iniOut,"; maximal strength that can be used without getting 'distortions'\n"); 
  fprintf(iniOut,"; on cranky vectrex this can be as low as 90\n"); 
  fprintf(iniOut,"; 'normal' vectrex can go as high as 127\n"); 
  fprintf(iniOut,"; potentially, the higher the faster\n"); 
  fprintf(iniOut,"MAX_USED_STRENGTH = %i\n", MAX_USED_STRENGTH); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; RESET_TO_ZERO_DIF_MAX: \n"); 
  fprintf(iniOut,"; when is a 'move' too far and a return to zero should be done instead\n"); 
  fprintf(iniOut,"; (plus an additional move)\n"); 
  fprintf(iniOut,"RESET_TO_ZERO_DIF_MAX = %i;\n", resetToZeroDifMax); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; CYCLE_EQUIVALENT: \n"); 
  fprintf(iniOut,"; how many Pi cycles represent one vectrex cycle. 666 nanoseconds should be pretty close\n"); 
  fprintf(iniOut,"CYCLE_EQUIVALENT = %i;\n", cycleEquivalent); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; BEAM_OFF_BETWEEN_CONSECUTIVE_DRAWS:\n"); 
  fprintf(iniOut,"; a thing of taste, vectors look more 'dotted' if this is 0\n"); 
  fprintf(iniOut,"; potentially a bit faster if 0 though\n"); 
  fprintf(iniOut,"BEAM_OFF_BETWEEN_CONSECUTIVE_DRAWS = %i;\n", beamOffBetweenConsecutiveDraws); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; ORIENTATION: \n"); 
  fprintf(iniOut,"; orientation rotated clock wise 0=normal, 1 = left, 2=up, 3 = right\n"); 
  fprintf(iniOut,"ORIENTATION = %i\n", orientation); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; CALIBRATION_VALUE: \n"); 
  fprintf(iniOut,"; value for a Tuts calibration, if 0 no calibration!\n"); 
  fprintf(iniOut,"CALIBRATION_VALUE = %i;\n", calibrationValue); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; USE_PIPELINE: \n"); 
  fprintf(iniOut,"; use pipeline 1= yes, 0 = 0\n"); 
  fprintf(iniOut,"; should always be yes\n"); 
  fprintf(iniOut,"USE_PIPELINE = %i;\n",usePipeline ); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; KEEP_DOTS_ORDERED: \n"); 
  fprintf(iniOut,"KEEP_DOTS_ORDERED = %i;\n",keepDotsTogether ); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; OFFSET_X: \n"); 
  fprintf(iniOut,"; screen position, - more left, + more right\n"); 
  fprintf(iniOut,"OFFSET_X = %i\n", offsetX); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; OFFSET_Y: \n"); 
  fprintf(iniOut,"; screen position, - more left, + more right\n"); 
  fprintf(iniOut,"OFFSET_Y = %i\n", offsetY); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; SIZE_X:\n"); 
  fprintf(iniOut,"; size correction of display, floating post value 1.0 = 'normal' output\n"); 
  fprintf(iniOut,"; higher values enlarge, smaller shrink\n"); 
  fprintf(iniOut,"SIZE_X = %f\n", sizeX); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; SIZE_Y:\n"); 
  fprintf(iniOut,"; size correction of display, floating post value 1.0 = 'normal' output\n"); 
  fprintf(iniOut,"; higher values enlarge, smaller shrink\n"); 
  fprintf(iniOut,"SIZE_Y = %f\n", sizeY); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; LOG_OUTPUT: \n"); 
  fprintf(iniOut,"; if different than 0 - than log all 'printf' outputs also to SD\n"); 
  fprintf(iniOut,"LOG_OUTPUT = %i\n", logOutput); 
  fprintf(iniOut,"\n"); 
  
  int hz = 1500000 / (((Vec_Rfrsh&0xff)*256) + ((Vec_Rfrsh>>8)&0xff));
  fprintf(iniOut,"; REFRESH_HZ: \n"); 
  fprintf(iniOut,"REFRESH_HZ = %i\n", hz); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; MINSCALE: \n"); 
  fprintf(iniOut,"MINSCALE = %i\n", MINSCALE); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; DOUBLE_TIMER: \n"); 
  fprintf(iniOut,"DOUBLE_TIMER = %i\n", useDoubleTimer); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; DELAY_AFTER_XSH_VALUE :\n"); 
  fprintf(iniOut,"DELAY_AFTER_XSH_VALUE = %i\n", DELAY_AFTER_XSH_VALUE); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; DELAY_AFTER_YSH_VALUE: \n"); 
  fprintf(iniOut,"DELAY_AFTER_YSH_VALUE = %i \n", DELAY_AFTER_YSH_VALUE); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; DELAY_PORT_B_BEFORE_PORT_A_VALUE: \n"); 
  fprintf(iniOut,"DELAY_PORT_B_BEFORE_PORT_A_VALUE = %i \n", DELAY_PORT_B_BEFORE_PORT_A_VALUE); 
  fprintf(iniOut,"\n"); 
  
  fprintf(iniOut,"; CRANKY_DELAY_Y_TO_NULL_VALUE: \n"); 
  fprintf(iniOut,"CRANKY_DELAY_Y_TO_NULL_VALUE = %i\n", CRANKY_DELAY_Y_TO_NULL_VALUE); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"; IN_GAME_SETTINGS: IS BUTTON 2 JOYPAD 2 configured to call the settings menu\n"); 
  fprintf(iniOut,"IN_GAME_SETTINGS = %i\n", inGameSettingsAllowed); 
  fprintf(iniOut,"\n"); 
  fprintf(iniOut,"\n"); 

  fprintf(iniOut,"RASTER_RETURN_DELAY_LEFT = %i \n", biRasterReturnDelayLeft); 
  fprintf(iniOut,"\n"); 
  fprintf(iniOut,"RASTER_RETURN_DELAY_RIGHT = %i \n", biRasterReturnDelayRight); 
  fprintf(iniOut,"\n"); 
  fprintf(iniOut,"RASTER_DOWN_BASE = %i \n", base_biRasterDown); 
  fprintf(iniOut,"\n"); 
//  fprintf(iniOut,"; SIZE_X:\n"); 
//  fprintf(iniOut,"; size correction of display, floating post value 1.0 = 'normal' output\n"); 
//  fprintf(iniOut,"; higher values enlarge, smaller shrink\n"); 
  fprintf(iniOut,"RASTER_DOWN_MODIFIER = %f\n", biRasterDownDriftModifier); 
  fprintf(iniOut,"\n"); 
  
  
   // as of now this is non optional!
   // {&wasIRQMode,0,0,0,0                          ,0,1,1, "IRQ MODE", CAL_TYPE_BOOL, irqModeHelp },
   // {0,0,&vecHz,0,0                               ,20,100,50, "VECTREX HZ", CAL_TYPE_UINT, vectrexHzHelp },
   // {0,0,&appHz,0,0                               ,0,100,0, "APP HZ", CAL_TYPE_UINT, appHzHelp },

  fclose(iniOut);
}


void *ptr;

#define MERGE_(a,b)  a##b    
#define LABEL_(a) MERGE_(_, a)    
#define MAIN_TAKEOVER(freeTime) \
  ptr = && LABEL_(__LINE__); \
 /* printf("hy:jumpSet): %08x, %i\r\n", ptr, freeTime);*/  \
  return freeTime;\
LABEL_(__LINE__):\
 /* printf("hy:jumpTaken): %08x\r\n", ptr); */ \
  ptr = 0; \

#define waitCounter1MarkLocal(waiter) \
do {\
  static unsigned int value;\
  /* Read CCNT Register */ \
  CCNT0(value);\
  if (value < (piCounterMark+((waiter)*ONE_WAIT_UNIT) ) )\
  {\
    USE_TIME( (piCounterMark+((waiter)*ONE_WAIT_UNIT) ) -value);\
  }\
  do\
  {\
    CCNT0(value);\
  } while (value < (piCounterMark+((waiter)*ONE_WAIT_UNIT) ) );\
} while (0)
  
char *v_sampleBuffer = 0;
int v_samplePosition = 0;
int v_sampleRate = 0;
int v_sampleSize = 0;
int v_sampleInterval = 0;
int v_sampleEnabled = 0;
int v_playMode = PLAY_ONCE;

uint32_t v_nextSampleAt=0;
void v_playIRQSample(char *buf, int size, int rate, int mode)
{
  if (size<=0) return;
  v_sampleEnabled = 1;
  v_sampleBuffer = buf;
  v_samplePosition = 0;
  v_sampleRate = rate;
  v_sampleSize = size;
  v_playMode = mode;
  v_sampleInterval = 1*1000*1000 / rate;
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}

#ifdef ENABLE_SAMPLE_MIXER
char *v_sample2Buffer = 0;
int v_sample2Position = 0;
int v_sample2Size = 0;
int v_sample2Enabled = 0;
int v_playMode2 = PLAY_ONCE;

void v_playIRQSample2(char *buf, int size, int rate, int mode)
{
  if (size<=0) return;
  v_sample2Enabled = 1;
  v_sample2Buffer = buf;
  v_sample2Position = 0;
  v_sample2Size = size;
  v_playMode2 = mode;
}
char *v_getSample2Playing()
{
  if (v_sample2Enabled == 0)return 0;
  return v_sample2Buffer;
}
static inline void v_playOneSample()
{
  int s=0;
  if ((!v_sampleEnabled) && (!v_sample2Enabled)) return;
  if (v_nextSampleAt > rpiSystemTimer->counter_lo) return;

  v_samplePosition++;
  if (v_samplePosition>=v_sampleSize) 
  {
    v_samplePosition = 0;
    if (v_playMode == PLAY_ONCE)
    {
      v_sampleEnabled =0;
      v_sampleBuffer =0;
    }
  }
  
  v_sample2Position++;
  if (v_sample2Position>=v_sample2Size) 
  {
    v_sample2Position = 0;
    if (v_playMode2 == PLAY_ONCE)
    {
      v_sample2Enabled =0;
      v_sample2Buffer =0;
    }
  }
  if ((v_sampleBuffer== 0) && (v_sample2Buffer== 0)) return;

  if (v_sample2Buffer!=0)
    s +=  (signed int) ( (signed char)( *(v_sample2Buffer+v_sample2Position)));
  if (v_sampleBuffer!=0)
    s +=  (signed int) ( (signed char)( *(v_sampleBuffer+v_samplePosition)));

  if (s>127) s = 127;
  if (s<-128) s = -128;

  unsigned char a = lastPortA;
  I_SET(VIA_port_a, (signed char) s);
  I_SET(VIA_port_b, 0x86);
  I_SET(VIA_port_b, 0x81);
  ADD_DELAY_CYCLES(2);
  I_SET(VIA_port_a, a);
  ADD_DELAY_CYCLES(2);
  
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}
static inline void v_playOneSample_ignoreA()
{
  int s=0;
  if ((!v_sampleEnabled) && (!v_sample2Enabled)) return;
  if (v_nextSampleAt > rpiSystemTimer->counter_lo) return;

  v_samplePosition++;
  if (v_samplePosition>=v_sampleSize) 
  {
    v_samplePosition = 0;
    if (v_playMode == PLAY_ONCE)
    {
      v_sampleEnabled =0;
      v_sampleBuffer =0;
    }
  }
  
  v_sample2Position++;
  if (v_sample2Position>=v_sample2Size) 
  {
    v_sample2Position = 0;
    if (v_playMode2 == PLAY_ONCE)
    {
      v_sample2Enabled =0;
      v_sample2Buffer =0;
    }
  }
  if ((v_sampleBuffer== 0) && (v_sample2Buffer== 0)) return;

  if (v_sample2Buffer!=0)
    s +=  (signed int) ( (signed char)( *(v_sample2Buffer+v_sample2Position)));
  if (v_sampleBuffer!=0)
    s +=  (signed int) ( (signed char)( *(v_sampleBuffer+v_samplePosition)));

  if (s>127) s = 127;
  if (s<-128) s = -128;
  
  I_SET(VIA_port_a, (signed char) s);
  I_SET(VIA_port_b, 0x86);
  I_SET(VIA_port_b, 0x81);
  ADD_DELAY_CYCLES(2);
  
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}
#else


#ifdef MULTI_SAMPLES

static inline void v_playOneSample_m()
{
  int s=0;
  if (!v_multiSampleEnabled)return;
  if (v_nextSampleAt > rpiSystemTimer->counter_lo) return;

  for (int i=0;i<SAMPLE_MAX; i++)
  {
      if (allSamples[i].used == 1)
      {
        allSamples[i].pos++;
        if (allSamples[i].pos>=allSamples[i].size) 
        {
          allSamples[i].pos = 0;
          if (allSamples[i].mode == PLAY_ONCE)
          {
            allSamples[i].used =0;
          }
        }
        else
        {
          s += (signed int) ( (signed char)( *(allSamples[i].data+allSamples[i].pos)));
        }
      }
  }
  if (s>127) s = 127;
  if (s<-128) s = -128;

  
//  s = 0;
  
  unsigned char a = lastPortA;
  I_SET(VIA_port_a, (signed char) s);
  I_SET(VIA_port_b, 0x86);
  I_SET(VIA_port_b, 0x81);
  ADD_DELAY_CYCLES(2);
  I_SET(VIA_port_a, a);
  ADD_DELAY_CYCLES(2);
  
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}
static inline void v_playOneSample_ignoreA_m()
{



v_playOneSample_m(); return;




  int s=0;
  if (!v_multiSampleEnabled)return;
  if (v_nextSampleAt > rpiSystemTimer->counter_lo) return;

  for (int i=0;i<SAMPLE_MAX; i++)
  {
      if (allSamples[i].used == 1)
      {
        allSamples[i].pos++;
        if (allSamples[i].pos>=allSamples[i].size) 
        {
          allSamples[i].pos = 0;
          if (allSamples[i].mode == PLAY_ONCE)
          {
            allSamples[i].used =0;
          }
        }
        else
        {
          s +=  (signed int) ( (signed char)( *(allSamples[i].data+allSamples[i].pos)));
        }
      }
  }
  if (s>127) s = 127;
  if (s<-128) s = -128;
  
  I_SET(VIA_port_a, (signed char) s);
  I_SET(VIA_port_b, 0x86);
  I_SET(VIA_port_b, 0x81);
  ADD_DELAY_CYCLES(2);
  
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}
#endif

static inline void v_playOneSample()
{
#ifdef MULTI_SAMPLES
  if (v_multiSampleEnabled)
  {
    v_playOneSample_m();
    return;
  }
#endif  
  if (!v_sampleEnabled) return;
  if (v_nextSampleAt > rpiSystemTimer->counter_lo) return;

  v_samplePosition++;
  if (v_samplePosition>=v_sampleSize) 
  {
    v_samplePosition = 0;
    if (v_playMode == PLAY_ONCE)
    {
      v_sampleEnabled =0;
      v_sampleBuffer =0;
      return;
    }
  }

  unsigned char a = lastPortA;
  I_SET(VIA_port_a, *(v_sampleBuffer+v_samplePosition));
  I_SET(VIA_port_b, 0x86);
  I_SET(VIA_port_b, 0x81);
  ADD_DELAY_CYCLES(2);
  I_SET(VIA_port_a, a);
  ADD_DELAY_CYCLES(2);
  
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}
static inline void v_playOneSample_ignoreA()
{
#ifdef MULTI_SAMPLES
  if (v_multiSampleEnabled)
  {
    v_playOneSample_ignoreA_m();
    return;
  }
#endif  
  if (!v_sampleEnabled) return;
  if (v_nextSampleAt > rpiSystemTimer->counter_lo) return;

  v_samplePosition++;
  if (v_samplePosition>=v_sampleSize) 
  {
    v_samplePosition = 0;
    if (v_playMode == PLAY_ONCE)
    {
      v_sampleEnabled =0;
      v_sampleBuffer =0;
      return;
    }
  }
  I_SET(VIA_port_a, *(v_sampleBuffer+v_samplePosition));
  I_SET(VIA_port_b, 0x86);
  I_SET(VIA_port_b, 0x81);
  ADD_DELAY_CYCLES(2);
  
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}
#endif

// returns startAddress of current sample / or 0
char *v_getSamplePlaying()
{
  if (v_sampleEnabled == 0) return 0;
  return v_sampleBuffer;
}
void v_stopSamplePlaying()
{
  v_sampleEnabled=0;
  v_sampleBuffer =0;
#ifdef MULTI_SAMPLES
  v_multiSampleEnabled=0;
#else
#ifdef ENABLE_SAMPLE_MIXER
  v_sample2Enabled=0;
  v_sample2Buffer =0;
#endif  
#endif  
}

#ifdef MULTI_SAMPLES
void initMultiSamples(int rate)
{
  for (int i=0;i<SAMPLE_MAX; i++)
    allSamples[i].used = 0;
  v_multiSampleEnabled = 1;
  v_sampleEnabled = 0;

  v_sampleInterval = 1*1000*1000 / rate;
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}
void deinitMultiSamples()
{
  v_multiSampleEnabled = 0;
}
void v_addIRQMultiSample(char *buf, int size, int mode)
{ 
  if (size<=0) return;

  for (int i=0;i<SAMPLE_MAX; i++)
  {
      if (allSamples[i].used == 0)
      {
        allSamples[i].used = 1;
        allSamples[i].pos = 0;
        allSamples[i].data = buf;
        allSamples[i].size = size;
        allSamples[i].mode = mode;
        break;
      }
  }
}
#endif



#ifdef SUPPORT_SAMPLES
#define PLAY_SAMPLE() do {v_playOneSample();} while(0)
#define PLAY_SAMPLE_IGNORE_A() do {v_playOneSample_ignoreA();} while(0)
#else
#define PLAY_SAMPLE() 
#define PLAY_SAMPLE_IGNORE_A()
#endif


/*
 The ugly thing about IRQ handling is, that due to the "non functional" nature we have to
 stay within ONE function - otherwise we would not be able
 to jump "back" to our destination after the IRQ is freed.
 So all "subroutine" calls, which should in any way "WAIT" (return the IRQ) must be
 resolved into this one function.
 This looks way ugly, but cannot be helped, unless we have different levels
 of IRQ dispatching - which would also be ugly.
 
 Also it would probably look nicer, if the old "function" could be "macrofied", this also does
 not work due to the current implemenation of the unique jump label, which takes the
 line number as an identifier. 
 Even a "multiline" macro has only one distinct __LINE__ number, so within one macro
 only one jump identifier can be created!
 
 WATCH OUT!
 All variables that must be accessed over a "IRQ return" MUST NOT be local variables (stack based) but
 must be static or global. Otherwise after a return from "normal space" the contents of the
 variable will be screwed!
*/ 

    
int pendingDisableInterrupts = 0;
int pendingReturnToPiTrex = 0;
int handleVectrexOutput()
{
    if (ptr != 0)
    {
      goto *ptr;
    }
    // except for release to normal space
    // there is no reason to leave the hyper space
    while (1)
    {
      // TDOD IRQ enable those!
      PLAY_SAMPLE_IGNORE_A();
      if (irqSound) 
      {
        
        if (extendedIRQSound)
        {
          v_playYM();
          v_playAllSFX();
        }
        
        // v_doSound();
        static int i;
//printf("---\n\r");          

        for (i=0;i<15;i++)
        {
          static uint8_t data;
          data = psgDoubleBuffer[i];
          if (psgShadow[i] == data) continue;
          psgShadow[i] = data;

          I_SET(VIA_port_a, i); // prepare access of psg port A (0x0e) by writing the register value to VIA port A
          I_SET(VIA_port_b, 0x99); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
          I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)

          I_SET(VIA_port_a, data); // write data to port a of via -> and than to psg
          I_SET(VIA_port_b, 0x91); // set VIA port B to settings: sound BDIR on, BC1 on, mux off, write to PSG
          I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
        }
//        currentPortA = 0x100; // undefined
      }
      PLAY_SAMPLE_IGNORE_A();
      if (irqButtons) 
      {
        //v_readButtons();
        ioDone |= V_BUTTONS_READ;
        // read of buttons goes thru the PSG sound chip, PSG port A
        //
        I_SET(VIA_port_a, 0x0e); // prepare access of psg port A (0x0e) by writing the register value to VIA port A
        ADD_DELAY_CYCLES(4);
        I_SET(VIA_port_b, 0x99); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
        ADD_DELAY_CYCLES(4);
        I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
        ADD_DELAY_CYCLES(4);

        SET(VIA_DDR_a, 0x00); // set VIA DDR A to input
        ADD_DELAY_CYCLES(4);
        I_SET(VIA_port_b, 0x89); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
        ADD_DELAY_CYCLES(6);
        currentButtonState = ~I_GET(VIA_port_a); // Read buttons
        I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
        ADD_DELAY_CYCLES(4);
        SET(VIA_DDR_a, 0xff); // set VIA DDR A to output
        ADD_DELAY_CYCLES(4);
     //  currentPortA = 0x100;

        internalButtonState = currentButtonState;
        if (inCalibration)
        {
          currentButtonState = 0;
        }
      }
      PLAY_SAMPLE_IGNORE_A();
      if (irqJoyAnalog) 
      {
        // v_readJoystick1Analog();
        ioDone |= V_JOY_ANALOG_READ;
        static int difx;
        static int dify;
        static uint8_t compareBit;
        static int8_t thisInternalJoy1Y;
        static int8_t thisInternalJoy1X;

        if (irqJoyAnalog && (1<<1) )
        {
          I_SET(VIA_port_a, 0x00); // clear VIA port A
          ADD_DELAY_CYCLES(4);
          I_SET(VIA_port_b, 0x82); // set VIA port B mux enabled, mux sel = 01 (vertical pot port 0)
          // wait for joystick comparators to "settle"
          ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

          currentJoy1Y = 0; // default centered
          I_SET(VIA_port_b, 0x83); // set VIA port B mux disabled
          DELAY_PORT_B_BEFORE_PORT_A();
          compareBit = 0x80;
          thisInternalJoy1Y = 0;

          do
          {
            I_SET(VIA_port_a, thisInternalJoy1Y); // load a with test value (positive y), test value to DAC
            ADD_DELAY_CYCLES(4);
            if ((I_GET(VIA_port_b) & 0x20) == 0)
            {
              thisInternalJoy1Y = thisInternalJoy1Y ^ compareBit;
            }
            ADD_DELAY_CYCLES(4);
            compareBit = compareBit>>1;
            thisInternalJoy1Y = thisInternalJoy1Y | compareBit;
          } while (compareBit!=0);
        }
        
        
        if (irqJoyAnalog && (1) )
        {
          I_SET(VIA_port_a, 0x00); // clear VIA port A
          ADD_DELAY_CYCLES(4);
          I_SET(VIA_port_b, 0x80); // set VIA port B mux enabled, mux sel = 01 (horizontal pot port 0)
          // wait for joystick comparators to "settle"
          ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

          currentJoy1X = 0; // default centered
          I_SET(VIA_port_b, 0x81); // set VIA port B mux disabled
          DELAY_PORT_B_BEFORE_PORT_A();
          compareBit = 0x80;

          thisInternalJoy1X = 0;
          do
          {
            I_SET(VIA_port_a, thisInternalJoy1X); // load a with test value (positive y), test value to DAC
            ADD_DELAY_CYCLES(4);
            if ((I_GET(VIA_port_b) & 0x20) == 0)
            {
              thisInternalJoy1X = thisInternalJoy1X ^ compareBit;
            }
            ADD_DELAY_CYCLES(4);
            compareBit = compareBit>>1;
            thisInternalJoy1X = thisInternalJoy1X | compareBit;
          } while (compareBit!=0);
        }

        // if joysticks disabled
        // the values might contain nonsense!
        difx = thisInternalJoy1X - internalJoy1X;
        dify = thisInternalJoy1Y - internalJoy1Y;

        internalJoy1X = internalJoy1X+(difx>>1);
        internalJoy1Y = internalJoy1Y+(dify>>1);

        if (!inCalibration)
        {
          currentJoy1X = internalJoy1X;
          currentJoy1Y = internalJoy1Y;
        }

        // if joyport 2  needed....
        if (irqJoyAnalog && +(1<<3) )
        {
          I_SET(VIA_port_a, 0x00); // clear VIA port A
          ADD_DELAY_CYCLES(4);
          I_SET(VIA_port_b, 0x82+0x04); // set VIA port B mux enabled, mux sel = 03 (vertical pot port 1)
          // wait for joystick comparators to "settle"
          ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

          currentJoy2Y = 0; // default centered
          I_SET(VIA_port_b, 0x83); // set VIA port B mux disabled
          DELAY_PORT_B_BEFORE_PORT_A();
          compareBit = 0x80;

          do
          {
            I_SET(VIA_port_a, currentJoy2Y); // load a with test value (positive y), test value to DAC
            ADD_DELAY_CYCLES(4);
            if ((I_GET(VIA_port_b) & 0x20) == 0)
            {
              currentJoy2Y = currentJoy2Y ^ compareBit;
            }
            ADD_DELAY_CYCLES(4);
            compareBit = compareBit>>1;
            currentJoy2Y = currentJoy2Y | compareBit;
          } while (compareBit!=0);
        }

        if (irqJoyAnalog && +(1<<2) )
        {
          I_SET(VIA_port_a, 0x00); // clear VIA port A
          ADD_DELAY_CYCLES(4);
          I_SET(VIA_port_b, 0x84); // set VIA port B mux enabled, mux sel = 2 (horizontal pot port 1)
          // wait for joystick comparators to "settle"
          ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

          currentJoy2X = 0; // default centered
          I_SET(VIA_port_b, 0x81); // set VIA port B mux disabled
          DELAY_PORT_B_BEFORE_PORT_A();
          compareBit = 0x80;

          do
          {
            I_SET(VIA_port_a, currentJoy2X); // load a with test value (positive y), test value to DAC
            ADD_DELAY_CYCLES(4);
            if ((I_GET(VIA_port_b) & 0x20) == 0)
            {
              currentJoy2X = currentJoy2X ^ compareBit;
            }
            ADD_DELAY_CYCLES(4);
            compareBit = compareBit>>1;
            currentJoy2X = currentJoy2X | compareBit;
          } while (compareBit!=0);
        }
        // set port A reference value to unkown
  //      currentYSH = currentPortA=0x100; // reset saved current values to unkown state
        
        ////////////////////////////////////////////////////////////
        // v_resetIntegratorOffsets0();
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
    //    currentPortA=0x100;// non regular value!
        ////////////////////////////////////////////////////////////
      }
      else if (irqJoyDigital) 
      {
        //v_readJoystick1Digital();
        ioDone |= V_JOY_DIGITAL_READ;

        if (irqJoyDigital && (1<<1) )
        {
          // Y ANALOG
          I_SET(VIA_port_a, 0x00); // clear VIA port A
          ADD_DELAY_CYCLES(4);
          I_SET(VIA_port_b, 0x82); // set VIA port B mux enabled, mux sel = 01 (vertical pot port 0)

          // wait for joystick comparators to "settle"
          ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

          currentJoy1Y = -1; // default down
          I_SET(VIA_port_b, 0x83); // set VIA port B mux
          DELAY_PORT_B_BEFORE_PORT_A();
          I_SET(VIA_port_a, 0x40); // load a with test value (positive y), test value to DAC
          ADD_DELAY_CYCLES(4);
          if ((I_GET(VIA_port_b) & 0x20) == 0x20)
          {
            currentJoy1Y = 1; //up
          }
          else
          {
            I_SET(VIA_port_a, -0x40); // load a with test value (negative y), test value to DAC
            ADD_DELAY_CYCLES(4);
            if ((I_GET(VIA_port_b) & 0x20) == 0x20)
            {
              currentJoy1Y = 0; // no direction
            }
          }
        }

        if (irqJoyDigital && (1) )
        {
          // X ANALOG
          I_SET(VIA_port_a, 0x00); // clear VIA port A
          ADD_DELAY_CYCLES(4);
          I_SET(VIA_port_b, 0x80); // set VIA port B mux enabled, mux sel = 00 (horizontal pot port 0)
          // wait for joystick comparators to "settle"int no_back = 0;

          ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

          currentJoy1X = -1; // default left
          I_SET(VIA_port_b, 0x83); // set VIA port B mux
          DELAY_PORT_B_BEFORE_PORT_A();
          I_SET(VIA_port_a, 0x40); // load a with test value (positive y), test value to DAC
          ADD_DELAY_CYCLES(2);
          if ((I_GET(VIA_port_b) & 0x20) == 0x20)
          {
            currentJoy1X = 1; //right
          }
          else
          {
            ADD_DELAY_CYCLES(4);
            I_SET(VIA_port_a, -0x40); // load a with test value (negative y), test value to DAC
            ADD_DELAY_CYCLES(4);
            if ((I_GET(VIA_port_b) & 0x20) == 0x20)
            {
              currentJoy1X = 0; // no direction
            }
            ADD_DELAY_CYCLES(4);
          }
        }

        // if joyport 2  needed....
        if (irqJoyDigital && (1<<3) )
        {
          // Y ANALOG
          I_SET(VIA_port_a, 0x00); // clear VIA port A
          ADD_DELAY_CYCLES(4);
          I_SET(VIA_port_b, 0x82+0x04); // set VIA port B mux enabled, mux sel = 03 (vertical pot port 1)

          // wait for joystick comparators to "settle"
          ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

          currentJoy2Y = -1; // default down
          I_SET(VIA_port_b, 0x83); // set VIA port B mux
          DELAY_PORT_B_BEFORE_PORT_A();
          I_SET(VIA_port_a, 0x40); // load a with test value (positive y), test value to DAC
          ADD_DELAY_CYCLES(4);
          if ((I_GET(VIA_port_b) & 0x20) == 0x20)
          {
            currentJoy2Y = 1; //up
          }
          else
          {
            I_SET(VIA_port_a, -0x40); // load a with test value (negative y), test value to DAC
            ADD_DELAY_CYCLES(4);
            if ((I_GET(VIA_port_b) & 0x20) == 0x20)
            {
              currentJoy2Y = 0; // no direction
            }
          }
        }

        if (irqJoyDigital && (1<<2) )
        {
          // X ANALOG
          I_SET(VIA_port_a, 0x00); // clear VIA port A
          ADD_DELAY_CYCLES(4);
          I_SET(VIA_port_b, 0x84); // set VIA port B mux enabled, mux sel = 2 (horizontal pot port 1)
          // wait for joystick comparators to "settle"
          ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

          currentJoy2X = -1; // default left
          I_SET(VIA_port_b, 0x83); // set VIA port B mux
          DELAY_PORT_B_BEFORE_PORT_A();
          I_SET(VIA_port_a, 0x40); // load a with test value (positive y), test value to DAC
          ADD_DELAY_CYCLES(2);
          if ((I_GET(VIA_port_b) & 0x20) == 0x20)
          {
            currentJoy2X = 1; //right
          }
          else
          {
            ADD_DELAY_CYCLES(4);
            I_SET(VIA_port_a, -0x40); // load a with test value (negative y), test value to DAC
            ADD_DELAY_CYCLES(4);
            if ((I_GET(VIA_port_b) & 0x20) == 0x20)
            {
              currentJoy2X = 0; // no direction
            }
            ADD_DELAY_CYCLES(4);
          }
        }

        // set port A reference value to unkown
        currentYSH = currentPortA=0x100; // reset saved current values to unkown state
        ////////////////////////////////////////////////////////////
        // v_resetIntegratorOffsets0();
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
   //     currentPortA=0x100;// non regular value!
        ////////////////////////////////////////////////////////////
      }

      if ((currentButtonState&0xff)== 0x20) // button 2 -> joypad 2
      {
        if (inGameSettingsAllowed)
        {
          if (wasIRQMode == 255)
          {
            inCalibration = 1;
            wasIRQMode = isIRQMode;
            pendingDisableInterrupts = 1;
          }
        }
      }
      if (currentButtonState ==0xf) // button 1+ 2 + 3+4 -> go menu
      {
        pendingReturnToPiTrex = 1;
//    allowReturnToLoader = 1;
//    returnToPiTrex();
        
      }
      PLAY_SAMPLE_IGNORE_A();
  #ifdef PITREX_DEBUG
      handleUARTInterface();
      PLAY_SAMPLE_IGNORE_A();
  #endif      
      
      //******************************************************************

      // Round is done
      //printf("hy:set up synchronization\r\n");  
      synqSignalIRQ = synqSignalIRQ | SYNC_DISPLAY_FINISHED; // only needed when in sync mode

      //******************************************************************
      if (v_sampleEnabled)
      {
        while ((GET(VIA_int_flags) & 0x20) == 0)
        {
          // the default entry is a WaitRecal
          int lo = GET(VIA_t1_cnt_lo); // reading lo, resets the T2 interrupt flag!
          int hi = GET(VIA_t2_cnt_hi);
          int t2 = hi*256+lo;
          
          if (t2 < (((Vec_Rfrsh&0xff)*256) + ((Vec_Rfrsh>>8)&0xff)))
          {
            if (t2 > v_sampleInterval) t2 = v_sampleInterval;
            // must wait for timer to expire
            // if not here, the timer expired bedore we got here -> no wait!
            MAIN_TAKEOVER(t2);
            PLAY_SAMPLE_IGNORE_A();
          }
        }
        if (isSynchronizedWithIRQ)
        {
          while (!(synqSignalIRQ & SYNC_CLIENT_FINISHED) ) 
          {
              // give client time to finish its round
            MAIN_TAKEOVER(v_sampleInterval); // hundred vectrex cycles
            PLAY_SAMPLE_IGNORE_A();
          }
          synqSignalIRQ = synqSignalIRQ & (~SYNC_CLIENT_FINISHED); // reset signal
        }
        ioDone = 0;
        SWITCH_BEAM_OFF();
        CCNT0(roundCycles);

        // wait for Via T2 to expire -> 50Hz
        while ((GET(VIA_int_flags) & 0x20) == 0) 
          PLAY_SAMPLE_IGNORE_A();
      }
      else
      {
        if ((GET(VIA_int_flags) & 0x20) == 0)
        {
          // the default entry is a WaitRecal
          int lo = GET(VIA_t1_cnt_lo); // reading lo, resets the T2 interrupt flag!
          int hi = GET(VIA_t2_cnt_hi);
          int t2 = hi*256+lo;
          if (t2 < (((Vec_Rfrsh&0xff)*256) + ((Vec_Rfrsh>>8)&0xff)))
          {
            // must wait for timer to expire
            // if not here, the timer expired bedore we got here -> no wait!
            MAIN_TAKEOVER(t2);
          }
        }
        if (isSynchronizedWithIRQ)
        {
          while (!(synqSignalIRQ & SYNC_CLIENT_FINISHED) ) 
          {
              // give client time to finish its round
            MAIN_TAKEOVER(100); // hundred vectrex cycles
          }
          synqSignalIRQ = synqSignalIRQ & (~SYNC_CLIENT_FINISHED); // reset signal
        }
        ioDone = 0;
        SWITCH_BEAM_OFF();
        CCNT0(roundCycles);

        // wait for Via T2 to expire -> 50Hz
        while ((GET(VIA_int_flags) & 0x20) == 0) ;
      }
    
      //******************************************************************
      
      // reset T2 VIA timer to 50Hz
      SETW (VIA_t2, Vec_Rfrsh);

        // at the start of each "round" 
      // reset pi counter 1
      PMNC(CYCLE_COUNTER_ENABLE|COUNTER_ZERO);

////////////////////////////////////////////////////////////      
//v_deflok();


//  ZERO_AND_WAIT();
    ZERO_AND_CONTINUE(); 
    PLAY_SAMPLE_IGNORE_A();
    waitCounter1MarkLocal(DELAY_ZERO_VALUE);
    
    UNZERO();
//    v_setScale(255);
    
//  SET_YSH_IMMEDIATE_8(127);
//  SET_XSH_IMMEDIATE_8(127);
    I_SET(VIA_port_b, 0x80); 
    DELAY_PORT_B_BEFORE_PORT_A(); 
    I_SET(VIA_port_a, ((uint8_t)(127))); 
    DELAY_YSH();

    I_SET(VIA_port_b, 0x81); 
    DELAY_XSH();
  
//  START_WAIT_T1();
//    START_T1_TIMER();
    SETW_inverse(VIA_t1, 255);  setCounter1Mark(); 

    PLAY_SAMPLE_IGNORE_A();
    waitCounter1MarkLocal(255+DELAY_AFTER_T1_END_VALUE);

//  ZERO_AND_WAIT();
    ZERO_AND_CONTINUE(); 
    waitCounter1MarkLocal(DELAY_ZERO_VALUE);
    PLAY_SAMPLE_IGNORE_A();

    UNZERO();
//  SET_YSH_IMMEDIATE_8(-127);
//  SET_XSH_IMMEDIATE_8(-127);
    I_SET(VIA_port_b, 0x80); 
    DELAY_PORT_B_BEFORE_PORT_A(); 
    I_SET(VIA_port_a, ((uint8_t)(-127))); 
    DELAY_YSH();
    I_SET(VIA_port_b, 0x81); 
    DELAY_XSH();
  
//  START_WAIT_T1();
//    START_T1_TIMER();
    SET(VIA_t1_cnt_hi, 0); setCounter1Mark(); 
    PLAY_SAMPLE_IGNORE_A();
    waitCounter1MarkLocal(255+DELAY_AFTER_T1_END_VALUE);
    PLAY_SAMPLE_IGNORE_A();

//  ZERO_AND_WAIT();
    ZERO_AND_CONTINUE(); 
    waitCounter1MarkLocal(DELAY_ZERO_VALUE);
    PLAY_SAMPLE_IGNORE_A();
////////////////////////////////////////////////////////////      



////////////////////////////////////////////////////////////
// v_resetIntegratorOffsets0();
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
//  currentPortA=0x100;// non regular value!
////////////////////////////////////////////////////////////
PLAY_SAMPLE_IGNORE_A();
    
//      currentZSH = 0x100;
      consecutiveDraws = 0; 
      #include "pipeline.i"
    }
}

uint32_t waitClientHzWait=0;
void handleClient_WR()
{
  // if an IRQ move is pending - do it at once
  // so we get rid of it...
  if (pendingDisableInterrupts)
  {
    pendingDisableInterrupts = 0;
    v_removeIRQHandling();
    resetPipeline();
    return;
  }
  if (pendingReturnToPiTrex)
  {
    pendingReturnToPiTrex = 0;
    v_removeIRQHandling();
    returnToPiTrex();
    return;
  }
  if (useUSB == 1)
    if (usbIRQHandler != 0)
      usbIRQHandler (usbIRQParam);
  
  // handle the pipeline
  handlePipeline();
  synqSignalIRQ = synqSignalIRQ | SYNC_CLIENT_FINISHED;
  
  
  if (isSynchronizedWithIRQ)
  {
    while (!(synqSignalIRQ & SYNC_DISPLAY_FINISHED) ) ; //printf("S");
    synqSignalIRQ = synqSignalIRQ & (~SYNC_DISPLAY_FINISHED); // reset signal

    clientRate = (1*1000*1000) / (((1*1000*1000)/clientHz) + (rpiSystemTimer->counter_lo - waitClientHzWait));
    waitClientHzWait = (rpiSystemTimer->counter_lo) + ((1*1000*1000)/clientHz);
    return;
  }
  
  while (rpiSystemTimer->counter_lo < waitClientHzWait) ;

  clientRate = (1*1000*1000) / (((1*1000*1000)/clientHz) + (rpiSystemTimer->counter_lo - waitClientHzWait));
  uint32_t counter = rpiSystemTimer->counter_lo;
  waitClientHzWait = counter + ((1*1000*1000)/clientHz);
}
void v_enableExtendedSoundOut(int yesNo)
{
  extendedIRQSound = yesNo;
}
void v_enableSoundOut(int yesNo)
{
  irqSound = yesNo;
}
void v_enableButtons(int yesNo)
{
  irqButtons = yesNo;
}
void v_enableJoystickDigital(int yesNoX1,int yesNoY1,int yesNoX2,int yesNoY2)
{
  irqJoyAnalog = 0;
  irqJoyDigital = yesNoX1+(yesNoY1<<1)+(yesNoX2<<2)+(yesNoY2<<3);
}
void v_enableJoystickAnalog(int yesNoX1,int yesNoY1,int yesNoX2,int yesNoY2)  // resets digital
{
  irqJoyDigital = 0;
  irqJoyAnalog = yesNoX1+(yesNoY1<<1)+(yesNoX2<<2)+(yesNoY2<<3);
}
void v_enable50HzSync(int yesNo)
{
  isSynchronizedWithIRQ = yesNo;
}

void v_setClientHz(int c)
{
  if (c==0) isSynchronizedWithIRQ = 1; else isSynchronizedWithIRQ = 0;
  clientHz = c;
}


// SYSTEM TIMER CODE
/**
    @brief The IRQ Interrupt handler
    This handler is run every time an interrupt source is triggered. It's
    up to the handler to determine the source of the interrupt and most
    importantly clear the interrupt flag so that the interrupt won't
    immediately put us back into the start of the handler again.
*/




// Overhead os betwen 230 - 800 nano seconds (guessed)
// this included latency
// to be on the save side we 
// should only use this if we have to wait at least 3 or for vectrex cycles!

void __attribute__((interrupt("IRQ"))) v_USBinterrupt_vector(void)
{
  if (rpiIRQController->IRQ_pending_1 & (1<<9))
  {
    rpiIRQController->IRQ_pending_1 = rpiIRQController->IRQ_pending_1;
    // DO USB interrupt

    if (usbIRQHandler != 0)
      usbIRQHandler (usbIRQParam);
    return;
  }
}  
void __attribute__((interrupt("IRQ"))) v_interrupt_vector(void)
{
  // releasetime in Vectrex cycles!
/*  for debugging
  if (currentButtonState == 0x02)
  {
    struct pt_regs *p;
    u_long *lnk_ptr;

    // On IRQ exception the LR points to PC+4
    asm __volatile__ (
        "sub lr, lr, #4\n"
        "mov %0, lr" : "=r" (lnk_ptr)
    );
    asm __volatile__ ("mov %0, sp\n"  : "=r" (p) );  
    asm __volatile__ ("mov lr, pc\n"  );  
    printf( "IRQ EXCEPTION at %p 0x%08lX\r\n", lnk_ptr, *(lnk_ptr));
    
    // following does not work well
    // some bugs in there
    // but since I don't need it right now...
    // I did not debug it!
    ptrace_stackdump_regs( p );    
    for (;;);
  }
*/  
  inIRQ_MODE = 1;
  while (1)
  {
    //wait for release time - in vectrex cycles
    releaseIRQTime = handleVectrexOutput();
    
    if (releaseIRQTime > 3)
    {
      // free the IRQ
      uint32_t counter = rpiSystemTimer->counter_lo;

      // intervall must be at least 3 <= (releaseIRQTime-1)
      if (releaseIRQTime==4) releaseIRQTime = 5; // prevent interval = 1
      
      uint32_t interval = ((releaseIRQTime-1) * 666) / 1000; // * 1000 = cycles
      rpiSystemTimer->control_status = 2; // acknoledge interrupt see: https://www.raspberrypi.org/forums/viewtopic.php?t=23969
                                          // BCM2837-ARM-Peripherals.-.Revised.-.V2-1-1.pdf page 172/173
      rpiSystemTimer->compare1 = counter+interval;
  inIRQ_MODE = 0;
      return;
    }
    else
    {
    }
    // do a busy wait
    // or just nothing, since
    // the place we are returnning to (returning as in goto handleVectrexOutput() )
    // wait cycle exact
  }
  inIRQ_MODE = 0;
}
void v_removeIRQHandling()
{
  if (isIRQMode==0) return;
  isIRQMode = 0;
  _disable_interrupts(); // function defined in out baremetal.S
  resetPipeline();
  printf("no:IRQ is removed\r\n");  
}


void DisconnectUSBInterrupt()
{
  _disable_interrupts(); // function defined in out baremetal.S
  // reset everything interrupt
  rpiIRQController->FIQ_control = 0; // clear FIQ
  rpiIRQController->Disable_IRQs_1 = (uint32_t) -1; // clear all 1 interrupts
  rpiIRQController->Disable_IRQs_2 = (uint32_t) -1; // clear all 2 interrupts
  rpiIRQController->Disable_Basic_IRQs = (uint32_t) -1; // clear all Basic interrupts
  
  rpiIRQController->IRQ_basic_pending = rpiIRQController->IRQ_basic_pending; // acknoldedge all interrupts
  rpiIRQController->IRQ_pending_1 = rpiIRQController->IRQ_pending_1; // acknoldedge all interrupts
  rpiIRQController->IRQ_pending_2 = rpiIRQController->IRQ_pending_2; // acknoldedge all interrupts
}

void ConnectInterrupt (unsigned nIRQ, TInterruptHandler *pHandler, void *pParam)
{
  useUSB = 1;
  pi_vectors *pv ;
  asm("ldr %[result],= 0\n\t" : [result]"=r" (pv) :: "cc");  // circumvent gcc UDF
  pv->_irq_pointer = (unsigned int)v_USBinterrupt_vector;

  usbIRQHandler = pHandler;
  usbIRQParam = pParam;
  
/*  
typedef struct {
    volatile uint32_t IRQ_basic_pending;
    volatile uint32_t IRQ_pending_1;
    volatile uint32_t IRQ_pending_2;
    volatile uint32_t FIQ_control;
    volatile uint32_t Enable_IRQs_1;
    volatile uint32_t Enable_IRQs_2;
    volatile uint32_t Enable_Basic_IRQs;
    volatile uint32_t Disable_IRQs_1;
    volatile uint32_t Disable_IRQs_2;
    volatile uint32_t Disable_Basic_IRQs;
    } rpi_irq_controller_t;
*/
  
  
  
  // reset everything interrupt
  rpiIRQController->FIQ_control = 0; // clear FIQ
  rpiIRQController->Disable_IRQs_1 = (uint32_t) -1; // clear all 1 interrupts
  rpiIRQController->Disable_IRQs_2 = (uint32_t) -1; // clear all 2 interrupts
  rpiIRQController->Disable_Basic_IRQs = (uint32_t) -1; // clear all Basic interrupts
  
  rpiIRQController->IRQ_basic_pending = rpiIRQController->IRQ_basic_pending; // acknoldedge all interrupts
  rpiIRQController->IRQ_pending_1 = rpiIRQController->IRQ_pending_1; // acknoldedge all interrupts
  rpiIRQController->IRQ_pending_2 = rpiIRQController->IRQ_pending_2; // acknoldedge all interrupts
  
  
  
  
  
  // enable USB interrupt
  rpiIRQController->Enable_IRQs_1 = 1 << nIRQ; // USB = 9

  
  // enable interrupts in geenral
  /* Enable interrupts! */
  _enable_interrupts(); // function defined in out baremetal.S
}
 
// in order to connect the usb keyboard
// for a short time we have to enable the
// usb keyboard interrupt (this is done inside the USPiInitialize() )
// for that to work, we have to ensure our own interrupt is disabled beforhand...
// ... later
// we do not want that interrupt in our system - so we remove it immedeatily after the init
//
// if we had an "own" interrupt - we have to reinitialize it...

int v_initKeyboard()
{
  if (usbKeyboardAvailable) return 1;
  int oldIRQMode = isIRQMode;
  if (isIRQMode == 1)
    v_removeIRQHandling();
    
    
  usbKeyboardAvailable = 0;
  useUSB = 0;
  // Initialize USB system we will want keyboard and mouse 
  if (!USPiInitialize ())
  {
    printf("USBV failed to initialize!\n\r");
    DisconnectUSBInterrupt();
    if (oldIRQMode) v_setupIRQHandling();
    return 0;
  }
  if (!USPiKeyboardAvailable ())
  {
    printf("Keyboard not found\n\r");
    DisconnectUSBInterrupt();
    if (oldIRQMode) v_setupIRQHandling();
    return 0;
  }
//    USPiKeyboardRegisterKeyPressedHandler (KeyPressedHandler);
  DisconnectUSBInterrupt();
  usbKeyboardAvailable = 1;
  useUSB = 1;
  printf("USB Keyboard found...\n\r");

  if (oldIRQMode) v_setupIRQHandling();
  return 1;
}


/* https://github.com/dwelch67/raspberrypi/blob/master/blinker07/blinker07.c
https://github.com/s-matyukevich/raspberry-pi-os/tree/master/src/lesson03/src

https://www.studica.com/blog/raspberry-pi-timer-embedded-environments

System Timer is in Micro seconds!
it is a 1Mhz timer
*/

void v_setupIRQHandling()
{
  if (isIRQMode==1) return;
  _disable_interrupts(); // function defined in out baremetal.S
  
  
  
  // reset everything interrupt
  rpiIRQController->FIQ_control = 0; // clear FIQ
  rpiIRQController->Disable_IRQs_1 = (uint32_t) -1; // clear all 1 interrupts
  rpiIRQController->Disable_IRQs_2 = (uint32_t) -1; // clear all 2 interrupts
  rpiIRQController->Disable_Basic_IRQs = (uint32_t) -1; // clear all Basic interrupts
  
  rpiIRQController->IRQ_basic_pending = rpiIRQController->IRQ_basic_pending; // acknoldedge all interrupts
  rpiIRQController->IRQ_pending_1 = rpiIRQController->IRQ_pending_1; // acknoldedge all interrupts
  rpiIRQController->IRQ_pending_2 = rpiIRQController->IRQ_pending_2; // acknoldedge all interrupts
  
  
  
  
  
  pi_vectors *pv ;
  asm("ldr %[result],= 0\n\t" : [result]"=r" (pv) :: "cc");  // circumvent gcc UDF
  pv->_irq_pointer = (unsigned int)v_interrupt_vector;

  synqSignalIRQ = 0;
  ptr = 0;
  setWaitMin = 0;
  waitClientHzWait = 0;

  
  printf("IRQ is set up\r\n");  
  resetPipeline();
  uint32_t counter = rpiSystemTimer->counter_lo;
  uint32_t interval = 1000;
  rpiSystemTimer->compare1 = counter+interval;
  rpiSystemTimer->control_status = 2;
  rpiIRQController->Enable_IRQs_1 = 2; // Timer 1
  isIRQMode = 1;

  /* Enable interrupts! */
  _enable_interrupts(); // function defined in out baremetal.S
}

void v_doInputMapping(InputMapping inputMapping[])
{
  int i=0;
  while (inputMapping[i].destination!=0)
  {
    // always reset to zero!
    *inputMapping[i].destination = 0;
    // buttons
    if (inputMapping[i].mapping & BUTTON_AND)
    {
      if ((currentButtonState & (inputMapping[i].mapping&0xff)) == (inputMapping[i].mapping&0xff))
      {
        *inputMapping[i].destination = 1;
      }
    }
    else
    {
      // any button matchings
      if (currentButtonState & (inputMapping[i].mapping&0xff))
        *inputMapping[i].destination = 1;
    }
    if (inputMapping[i].mapping&JOYPAD_1_DIGITAL)
    {
      if (inputMapping[i].mapping&JOYPAD_UP)
      {
        if (currentJoy1Y>0) *inputMapping[i].destination = 1;
      }
      if (inputMapping[i].mapping&JOYPAD_DOWN)
      {
        if (currentJoy1Y<0) *inputMapping[i].destination = 1;
      }
      if (inputMapping[i].mapping&JOYPAD_RIGHT)
      {
        if (currentJoy1X>0) *inputMapping[i].destination = 1;
      }
      if (inputMapping[i].mapping&JOYPAD_LEFT)
      {
        if (currentJoy1X<0) *inputMapping[i].destination = 1;
      }
    }
    if (inputMapping[i].mapping&JOYPAD_2_DIGITAL)
    {
      if (inputMapping[i].mapping&JOYPAD_UP)
      {
        if (currentJoy2Y>0) *inputMapping[i].destination = 1;
      }
      if (inputMapping[i].mapping&JOYPAD_DOWN)
      {
        if (currentJoy2Y<0) *inputMapping[i].destination = 1;
      }
      if (inputMapping[i].mapping&JOYPAD_RIGHT)
      {
        if (currentJoy2X>0) *inputMapping[i].destination = 1;
      }
      if (inputMapping[i].mapping&JOYPAD_LEFT)
      {
        if (currentJoy2X<0) *inputMapping[i].destination = 1;
      }
    }
#define MIN_VALUE 5
    
    // all analog values are ABS(x) > MIN_VALUE, so thy do not "overwrite" with small values already gotten values
    if (inputMapping[i].mapping&JOYPAD_1_ANALOG)
    {
      if (inputMapping[i].mapping&JOYPAD_UP)
      {
        //if (currentJoy1Y>30) *inputMapping[i].destination = currentJoy1Y;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
          if (currentJoy1Y>MIN_VALUE)
            *inputMapping[i].destination = currentJoy1Y;
      }
      if (inputMapping[i].mapping&JOYPAD_DOWN)
      {
        //if (currentJoy1Y<-30) *inputMapping[i].destination = currentJoy1Y;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
        {
          if (currentJoy1Y<-MIN_VALUE)
            *inputMapping[i].destination = currentJoy1Y;
        }
      }
      if (inputMapping[i].mapping&JOYPAD_RIGHT)
      {
        //if (currentJoy1X>30) *inputMapping[i].destination = currentJoy1X;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
          if (currentJoy1X>MIN_VALUE)
          *inputMapping[i].destination = currentJoy1X;
      }
      if (inputMapping[i].mapping&JOYPAD_LEFT)
      {
        //if (currentJoy1X<-30) *inputMapping[i].destination = currentJoy1X;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
          if (currentJoy1X<-MIN_VALUE)
          *inputMapping[i].destination = currentJoy1X;
      }
    }
    if (inputMapping[i].mapping&JOYPAD_2_ANALOG)
    {
      if (inputMapping[i].mapping&JOYPAD_UP)
      {
        //if (currentJoy2Y>30) *inputMapping[i].destination = currentJoy2Y;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
          if (currentJoy2Y>MIN_VALUE)
          *inputMapping[i].destination = currentJoy2Y;
      }
      if (inputMapping[i].mapping&JOYPAD_DOWN)
      {
        //if (currentJoy2Y<-30) *inputMapping[i].destination = currentJoy2Y;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
        {
          if (currentJoy2Y<-MIN_VALUE)
            *inputMapping[i].destination = currentJoy2Y;
        }
      }
      if (inputMapping[i].mapping&JOYPAD_RIGHT)
      {
        //if (currentJoy2X>30) *inputMapping[i].destination = currentJoy2X;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
          if (currentJoy2X>MIN_VALUE)
            *inputMapping[i].destination = currentJoy2X;
      }
      if (inputMapping[i].mapping&JOYPAD_LEFT)
      {
        //if (currentJoy2X<-30) *inputMapping[i].destination = currentJoy2X;
        if (*inputMapping[i].destination == 0) // donot overwrite allready gotten values!
          if (currentJoy2X<-MIN_VALUE)
            *inputMapping[i].destination = currentJoy2X;
      }
    }
    i++;
  }  
}


#include "calculator.c"


// i KNOW that all with are shorter/equal than rep
// otherwise this would be BAD
void replace(char *in, char *rep, char *with)
{
  int lenRep = strlen(rep);
  int lenWith = strlen(with);
  while (1)
  {
    char *where = strstr(in, rep);
    if (where==0) return;
    
    strncpy(where, with, lenWith);
    char *pos = where+lenWith;
    strcpy(pos, where+lenRep);  
  }
  
}

int evaluateInputIniLine(const char *valueString)
{
  char buf[1024];
  strcpy(buf, valueString);
  replace(buf, "JOYPAD_1_DIGITAL", "4096");
  replace(buf, "JOYPAD_2_DIGITAL", "8192");
  replace(buf, "JOYPAD_1_ANALOG", "16384");
  replace(buf, "JOYPAD_2_ANALOG", "32768");
  
  replace(buf, "JOYPAD_UP", "256");
  replace(buf, "JOYPAD_DOWN", "512");
  replace(buf, "JOYPAD_LEFT", "1024");
  replace(buf, "JOYPAD_RIGHT", "2048");

  replace(buf, "BUTTON_AND", "65536");

  replace(buf, "BUTTON_1_1", "1");
  replace(buf, "BUTTON_1_2", "2");
  replace(buf, "BUTTON_1_3", "4");
  replace(buf, "BUTTON_1_4", "8");
  replace(buf, "BUTTON_2_1", "16");
  replace(buf, "BUTTON_2_2", "32");
  replace(buf, "BUTTON_2_3", "64");
  replace(buf, "BUTTON_2_4", "128");

  replace(buf, "|", "+"); // this is not CORRECT, but should suffice for what I use this for!
  
  // now we should have a line, that we can "calculate"
  simple_queue_t output;
  simple_stack_t ops;

  queue_init(&output);
  stack_init(&ops);

  /* Convert to RPN */
  shunting_yard(buf, &ops, &output);

  /* Compute result */
  int result = compute_rpn(&output);
//  printf("%s -> %s -> result: %i\n", valueString, buf, result);
  return result;
}
int evaluateLine(const char *valueString, NameReplacePair switchMap[])
{
  int i=0;
  char buf[1024];
  strcpy(buf, valueString);

  while (switchMap[i].replace != 0)
  {
    replace(buf, switchMap[i].replace, switchMap[i].with);
    i++;
  }
  // now we should have a line, that we can "calculate"
  simple_queue_t output;
  simple_stack_t ops;

  queue_init(&output);
  stack_init(&ops);

  /* Convert to RPN */
  shunting_yard(buf, &ops, &output);

  /* Compute result */
  int result = compute_rpn(&output);
  return result;
}


char *dipHelp[]=
{
  "ITEM SETTINGS", // title
  "UP/DOWN SCROLLS THROUGH",
  "ITEMS.",
  "LEFT/RIGHT CHANGES SETTINGS.",
  "BUTTON 4 TO EXIT.",
  0
};


int getSelectedItem(DipSwitchSetting *dip)
{
  int selectedValue = *dip->destination;
  int i=0;
  while (dip->values[i].name !=0)
  {
    if (dip->values[i].value == selectedValue) return i;  
    i++;
  }
  return 0;
}


void v_getDipSettings(DipSwitchSetting *dips[], char * title)
{
//  *dips[0]->destination = 1; // german

  if (dips[0] ==0) return;
  
  int saveClientHz =   clientHz;
  clientHz = 50;
  
  int item = 0;
  int itemValue;
  int doingSomething = 0;
  v_noSound();
  int starting = 1;
  itemValue = getSelectedItem(dips[item]);

  int tlen = strlen(title);
  while (1) 
  {
    v_WaitRecal();
    v_setBrightness(0x40);
    v_readButtons();
    v_readJoystick1Analog();
    
    v_printString(0-tlen*5, 100, title, 10, 0x50);
    
    if (!(currentButtonState&0x0f)) starting = 0;
    
    if ((currentButtonState&0x0f) == (0x01)) // exactly button 1
    {
      if (!starting) displayHelp(dipHelp,0);
    }
    if ((currentButtonState&0x0f) == (0x08)) // exactly button 4
    {
      if (!starting) break;
    }
    
    if ((ABS(internalJoy1X)<20) && (ABS(internalJoy1Y)<20)) 
      doingSomething =0;

    if (doingSomething == 0)
    {
      if (internalJoy1Y<-80)
      {
        if (dips[item+1] != 0)
        {
          item++;
          itemValue = getSelectedItem(dips[item]);
        }
        doingSomething=1;
      }
      if (internalJoy1Y>80)
      {
        if (item > 0)
        {
          item--;
          itemValue = getSelectedItem(dips[item]);
        }
        doingSomething=1;
      }
    }
   
    if (doingSomething == 0)
    {
      if (internalJoy1X>80)
      {
        if (dips[item]->values[itemValue+1].name != 0)
        {
          itemValue++;
          *dips[item]->destination = dips[item]->values[itemValue].value;
        }
        doingSomething=1;
      }
      if (internalJoy1X<-80)
      {
        if (itemValue > 0)
        {
          itemValue--;
          *dips[item]->destination = dips[item]->values[itemValue].value;
        }
        doingSomething=1;
      }
    }
    
    int y = 60;
    int i = 0;
    while(dips[i] != 0)
    {
      int intensity = 0x20;
      if (i==item) intensity = 0x50;
      v_printString(-60, y, dips[i]->name, 5, intensity);
      
      int currentValue = getSelectedItem(dips[i]);
      v_printString(40, y, dips[i]->values[currentValue].name, 5, intensity);
      y-=20;
      i++;
    }
    
    
  }
  // save?
  clientHz = saveClientHz;
  
}


// -1 on error
// length of sample else
int v_loadRAW(const char *filename, unsigned char *largeEnoughBuffer)
{
  FILE *fileRead;
  fileRead = fopen(filename, "rb");
  if (fileRead == 0)
  {
    printf("File (%s) could not be opened: %s!\r\n",filename, ff_getErrorText(errno));
    return -1;
  }
  int len = __filelength(fileRead);
  int loadLen = fread(largeEnoughBuffer, len, 1, fileRead);
  if (1 != loadLen)
  {
    printf("Read(1) of %s fails (len loaded: %i) (Error: %i)\r\n", filename, len, errno);
    fclose(fileRead);
    return -1;
  }
  fclose(fileRead);
  printf("File loaded successfully (%s, %i)!\r\n",filename,len);
  return len;
}





void v_displayAbsolutList_8(const signed char list[], int brightness)
{
  int count = (unsigned char) (*list++);

  while (count >0)  
  {
    int y0 = *list++;
    int x0 = *list++;
    int y1 = *list++;
    int x1 = *list++;
    v_directDraw32(x0<<7, y0<<7,x1<<7,y1<<7, brightness);
    count --;
  }
}


void v_displayAbsolutList_8o(int x, int y, const signed char list[], int brightness)
{
  int count = (unsigned char) (*list++);

  while (count >0)  
  {
    int y0 = y+*list++;
    int x0 = x+*list++;
    int y1 = y+*list++;
    int x1 = x+*list++;
    v_directDraw32(x0<<7, y0<<7,x1<<7,y1<<7, brightness);
    count --;
  }
}






const signed char arrowDown[]=
{   (signed char) 6, // count of vectors
    (signed char) 0xFF, (signed char) 0xFD, (signed char) 0xFF, (signed char) 0x03, // y0, x0, y1, x1
    (signed char) 0xFF, (signed char) 0x03, (signed char) 0xFB, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xFB, (signed char) 0x00, (signed char) 0xFF, (signed char) 0xFD, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0xFD, (signed char) 0x02, (signed char) 0x03, // y0, x0, y1, x1
    (signed char) 0x02, (signed char) 0x03, (signed char) 0xFF, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0xFF, (signed char) 0x00, (signed char) 0x02, (signed char) 0xFD, // y0, x0, y1, x1
};

const signed char arrowUp[]=
{   (signed char) 6, // count of vectors
    (signed char) 0x01, (signed char) 0xFD, (signed char) 0x01, (signed char) 0x03, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0x03, (signed char) 0x05, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x05, (signed char) 0x00, (signed char) 0x01, (signed char) 0xFD, // y0, x0, y1, x1
    (signed char) 0xFE, (signed char) 0xFD, (signed char) 0xFE, (signed char) 0x03, // y0, x0, y1, x1
    (signed char) 0xFE, (signed char) 0x03, (signed char) 0x01, (signed char) 0x00, // y0, x0, y1, x1
    (signed char) 0x01, (signed char) 0x00, (signed char) 0xFE, (signed char) 0xFD, // y0, x0, y1, x1
};


static char *_toUpper(char *t)
{
  static char _ubuf[256];
  char *p = _ubuf;
  do
  {
    *p++ = toupper(*t++);
  } while (*t != 0);
  *p = 0;
  return _ubuf;
}


DisplayItem it00;
DisplayItem it01;
DisplayItem it02;
DisplayItem it03;
DisplayItem it04;
DisplayItem it05;
DisplayItem it06;
DisplayItem it07;
DisplayItem it08;
DisplayItem it09;
DisplayItem it10;
DisplayItem it11;
DisplayItem it12;
DisplayItem it13;
DisplayItem it14;
DisplayItem it15;
DisplayItem it16;
DisplayItem it17;
DisplayItem it18;
DisplayItem it19;
DisplayItem it20;
DisplayItem it21;
DisplayItem it22;
DisplayItem it23;
DisplayItem it24;
DisplayItem it25;
DisplayItem it26;
DisplayItem it27;
DisplayItem it28;
DisplayItem it29;


DisplayList defaultDisplayList=
{
  0, // x
  0, // y
  2, // lineSpacing
  
  1, // displayArrows
  1, // rollover

  0, // startYOffset
  0, // itemOffset, f
  5, // itemDisplayCount

  0, // itemCount
  0, // currentMainItem

  0, // autoRaster
  0, // displayRectangle
  1, // fade
  30,// fontWidth
  -4,// fontHeight
  alternate_rasterlines,
  8, // rasterlineCount

  {
    &it00, &it01, &it02, &it03,&it04, &it05,&it06, &it07, &it08, &it09,
    &it10, &it11, &it12, &it13,&it14, &it15,&it16, &it17, &it18, &it19,
    &it20, &it21, &it22, &it23,&it24, &it25,&it26, &it27, &it28, &it29
  }
};

void itemListSetPos(int x, int y)
{
  defaultDisplayList.x=x;
  defaultDisplayList.y=y;
}

void itemListClear()
{
  defaultDisplayList.itemCount=0;
  defaultDisplayList.itemOffset=0;
  defaultDisplayList.autoRaster=0;
  defaultDisplayList.displayRectangle=0;
  defaultDisplayList.displayArrows=1;
  defaultDisplayList.fade=1;
  defaultDisplayList.lineSpacing=2;

  defaultDisplayList.startYOffset=0;
  defaultDisplayList.currentMainItem=-1;
  defaultDisplayList.rollover = 1;
}

void itemListAdd(char* text, int brightness)
{
  if (defaultDisplayList.itemCount+1 == ITEM_MAX_ITEMS) return;
  DisplayItem *item = defaultDisplayList.items[defaultDisplayList.itemCount];

  strncpy(item->text,  _toUpper(text), ITEM_MAX_STRINGSIZE-1);
  item->baseBrightness = brightness;

  defaultDisplayList.itemCount++;
}

// always positive
void itemListScrollDown(int step)
{
  defaultDisplayList.startYOffset -= step;
  if (defaultDisplayList.startYOffset< -defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing)
  {
    defaultDisplayList.startYOffset += (defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing);

    defaultDisplayList.itemOffset--;

    if (defaultDisplayList.rollover)
    {
      if (defaultDisplayList.itemOffset<0)
        defaultDisplayList.itemOffset += defaultDisplayList.itemCount;
    }
    else
    {
      if (defaultDisplayList.itemOffset<0)
      {
        defaultDisplayList.itemOffset++;
        defaultDisplayList.startYOffset -= (defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing);
        defaultDisplayList.startYOffset += step;
      }
    }
  }
}

// always positive
void itemListScrollUp(int step)
{
  defaultDisplayList.startYOffset += step;
  if (defaultDisplayList.startYOffset> defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing)
  {
    defaultDisplayList.startYOffset -= (defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing);

    defaultDisplayList.itemOffset++;
    if (defaultDisplayList.rollover)
    {
      if (defaultDisplayList.itemOffset>=defaultDisplayList.itemCount)
        defaultDisplayList.itemOffset -= defaultDisplayList.itemCount;
    }
    else
    {
      if (defaultDisplayList.itemOffset>=defaultDisplayList.itemCount)
      {
        defaultDisplayList.itemOffset--;
        defaultDisplayList.startYOffset += (defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing);
        defaultDisplayList.startYOffset -= step;
      }
    }
    
  }
}

static float getPercentFactor(int mid, int val)
{
  if (!defaultDisplayList.fade) return 1;
  if (val >mid) val = mid-val;

  float ret = 1.0 - ((float)val) / ((float)mid);
  if (ret >1) ret = 0;
  return ret;
}

void itemListDisplay()
{
  int moved = 0;
  static int lastDirection = 0;
  
  if (irqJoyDigital) 
  {
    if (currentJoy1Y>0)
    {
      itemListScrollDown(3);
      lastDirection = 1;
      moved= 1;
    }
    if (currentJoy1Y<0)
    {
      itemListScrollUp(3);
      lastDirection = -1;
      moved= -1;
    }
  }
  else if (irqJoyAnalog) 
  {
    // todo analog
    if (currentJoy1Y>0)
    {
      itemListScrollDown(currentJoy1Y/30);
      lastDirection = 1;
      moved= 1;
    }
    if (currentJoy1Y<0)
    {
      itemListScrollUp(-currentJoy1Y/30);
      lastDirection = -1;
      moved= -1;
    }
  }

  if ((defaultDisplayList.autoRaster) && (!moved) && (lastDirection))
  {
    if (defaultDisplayList.startYOffset > (defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing)/2)
    {
      defaultDisplayList.itemOffset++;
      if (defaultDisplayList.itemOffset>=defaultDisplayList.itemCount)
        defaultDisplayList.itemOffset -= defaultDisplayList.itemCount;
      defaultDisplayList.startYOffset = 0;
      lastDirection = 0;
    }
    else if (defaultDisplayList.startYOffset < -(defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing)/2)
    {
      defaultDisplayList.itemOffset--;
      if (defaultDisplayList.itemOffset<0)
        defaultDisplayList.itemOffset += defaultDisplayList.itemCount;
      defaultDisplayList.startYOffset = 0;
      lastDirection = 0;
    }
    else 
    {
      defaultDisplayList.startYOffset = 0;
      lastDirection = 0;
    }
  }
  
  if (defaultDisplayList.itemDisplayCount > defaultDisplayList.itemCount) defaultDisplayList.itemDisplayCount = defaultDisplayList.itemCount;
  
  
  int x = defaultDisplayList.x;
  int y = defaultDisplayList.y;
  int yOffset = 0;
  
  int even = defaultDisplayList.itemDisplayCount & 1;
  
  
  float brightnessFactor; // percent, top = 0%, bottom = 0%, center = 100%
  
  int oneDisplayLength = defaultDisplayList.rasterlineCount*defaultDisplayList.lineSpacing; // assuming 1 above, 1 below, 1 in between and text
  int size = defaultDisplayList.itemDisplayCount*oneDisplayLength;
  int center = size/2;

  y -=center; // start is in the top left corner
  int centerStart = /*center+*/(oneDisplayLength)/2; // + -> start "above"
  
  if (even) centerStart = 0; 
  
  
  // a positive offset srcolls "up"
  // a negative offset srcolls "down"
  centerStart+=defaultDisplayList.startYOffset; // vectrex inverse Y default is start 5 pixel below start of "home" range


  // display center first
  int displayItem;
  if (defaultDisplayList.rollover)
  {
    displayItem = (defaultDisplayList.itemOffset + defaultDisplayList.itemDisplayCount/2)%defaultDisplayList.itemCount;
    if (displayItem>=defaultDisplayList.itemCount) displayItem-=defaultDisplayList.itemCount;
  }
  else
  {
    displayItem = (defaultDisplayList.itemOffset + defaultDisplayList.itemDisplayCount/2);
  }
    
  defaultDisplayList.currentMainItem = displayItem;
  
  brightnessFactor = getPercentFactor(center, abs(centerStart-((oneDisplayLength)/2) ) );
  int b = (int)(defaultDisplayList.items[displayItem]->baseBrightness * brightnessFactor);
  v_setBrightness(b);    // adjust brightness with position offset

  if (displayItem<defaultDisplayList.itemCount) 
    v_printStringRasterFont( x, y+centerStart,defaultDisplayList.items[displayItem]->text, defaultDisplayList.fontWidth, defaultDisplayList.fontHeight, 0, defaultDisplayList.rlines);

  if (defaultDisplayList.displayRectangle>0)
  {
    int x1 = x-10;
    int x2 = x1+defaultDisplayList.displayRectangle;

    int y1;
    if (!even)
      y1 = y+(oneDisplayLength)-2;
    else
      y1 = y+(oneDisplayLength)/2-2;
    int y2 = y1-(oneDisplayLength);
    
    x1 = x1 << 7;
    x2 = x2 << 7;
    y1 = y1 << 7;
    y2 = y2 << 7;
  
    v_directDraw32 (x2, y1, x1, y1, 0x40);
    v_directDraw32 (x1, y1, x1, y2, 0x40);
    v_directDraw32 (x1, y2, x2, y2, 0x40);
    v_directDraw32 (x2, y2, x2, y1, 0x40);
  }
  
  
  int upper=0;
  // display items "above"
  for (int i=0; i<defaultDisplayList.itemDisplayCount/2; i++)
  {
    int updisplayItem = displayItem-(1+i);
    
    if (defaultDisplayList.rollover)
    {
      if (updisplayItem<0) updisplayItem+=defaultDisplayList.itemCount;

      brightnessFactor = getPercentFactor(center, abs(centerStart+(1+i)*(oneDisplayLength)-((oneDisplayLength)/2)  ));
      b = (int)(defaultDisplayList.items[updisplayItem]->baseBrightness * brightnessFactor);
      v_setBrightness(b);    // adjust brightness with position offset
      v_printStringRasterFont( x, y+centerStart+(1+i)*(oneDisplayLength),defaultDisplayList.items[updisplayItem]->text, defaultDisplayList.fontWidth, defaultDisplayList.fontHeight, 0, defaultDisplayList.rlines);
    }
    else
    {
      if ((updisplayItem>=0) && (updisplayItem<defaultDisplayList.itemCount))
      {
        brightnessFactor = getPercentFactor(center, abs(centerStart+(1+i)*(oneDisplayLength)-((oneDisplayLength)/2)  ));
        b = (int)(defaultDisplayList.items[updisplayItem]->baseBrightness * brightnessFactor);
        v_setBrightness(b);    // adjust brightness with position offset
        v_printStringRasterFont( x, y+centerStart+(1+i)*(oneDisplayLength),defaultDisplayList.items[updisplayItem]->text, defaultDisplayList.fontWidth, defaultDisplayList.fontHeight, 0, defaultDisplayList.rlines);
      }
    }
    upper = y+(2+i)*(oneDisplayLength);
  }
  
  int downer=0;
  // display items "below"
  for (int i=0; i<(defaultDisplayList.itemDisplayCount-1)/2; i++)
  {
    int downdisplayItem = displayItem+(1+i);
    if (defaultDisplayList.rollover)
    {
      if (downdisplayItem>=defaultDisplayList.itemCount) downdisplayItem-=defaultDisplayList.itemCount;

      brightnessFactor = getPercentFactor(center, abs(centerStart-(1+i)*(oneDisplayLength)-((oneDisplayLength)/2)  ));
      b = (int)(defaultDisplayList.items[downdisplayItem]->baseBrightness * brightnessFactor);

      v_setBrightness(b);    // adjust brightness with position offset
      v_printStringRasterFont( x, y+centerStart-(1+i)*(oneDisplayLength),defaultDisplayList.items[downdisplayItem]->text, defaultDisplayList.fontWidth, defaultDisplayList.fontHeight, 0, defaultDisplayList.rlines);
    }
    else
    {
      if ((downdisplayItem>=0) && (downdisplayItem<defaultDisplayList.itemCount))
      {
        brightnessFactor = getPercentFactor(center, abs(centerStart-(1+i)*(oneDisplayLength)-((oneDisplayLength)/2)  ));
        b = (int)(defaultDisplayList.items[downdisplayItem]->baseBrightness * brightnessFactor);

        v_setBrightness(b);    // adjust brightness with position offset
        v_printStringRasterFont( x, y+centerStart-(1+i)*(oneDisplayLength),defaultDisplayList.items[downdisplayItem]->text, defaultDisplayList.fontWidth, defaultDisplayList.fontHeight, 0, defaultDisplayList.rlines);
      }
    }
    downer= y-(1+2)*(oneDisplayLength);
  }

  if (defaultDisplayList.displayArrows)
  {
    if (!defaultDisplayList.fade)
    {
      v_displayAbsolutList_8o(0, upper+20, arrowUp, (moved==1) ? 0x40:0x20);
      v_displayAbsolutList_8o(0, downer-20,arrowDown, (moved==-1) ? 0x40:0x20);
    }
    else
    {
      v_displayAbsolutList_8o(0, upper+5, arrowUp, (moved==1) ? 0x40:0x20);
      v_displayAbsolutList_8o(0, downer-5,arrowDown, (moved==-1) ? 0x40:0x20);
    }
    v_setBrightness(64);
  }

}

#include "ymStuff.i"



