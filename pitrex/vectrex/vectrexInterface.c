// todo ADD to Pipeline
// 8 bit for now
//void v_printBitmapUni(unsigned char *bitmapBlob, int width, int height, int sizeX, int x, int y)

//differen scales

//unsigned char DELAY_AFTER_BEAM_ON = 0;
//unsigned char DELAY_AFTER_BEAM_STOP_MOVING = 0;
//unsigned char BEAMOFF_CYCLE_BEFORE_TIMEOUT = 0;


/*

TO TEST

Clustered scale!
Use the best "smallest?" scale, if a vector is bigger -> than uses a double draw (new flag direct continue / double draw (resue of X,Y just restart of timer) )
If vector is smaller reduce size
-> all vectors in a cluster use the same scale!
Each cluster starts with a moveto (uncalibrated) and a calibration and than all following draws/moves.
A cluster must consist of at least X (10?, 20?) Vectors




a) only 3 different scales
$08 minimal

$20 midrange

$80 max (for whole screen this must be two!)


Asteroids shüsse mit zero trail

Manche Spiele zu dunkel

Vektoren schief - wie immer

unzero wait?

test switch beam on before T1 -> with a "shift" add %11110000 ....






convert current pipeline (s) to ONE pipeline



Build a pure VIA pipeline


*/




//T1 setting for LAST vector only!

//Find settings (at no matter what cost) to create fonts with 90 degrees which FIT!

//Omega Race DOTS


void __attribute__ ((noreturn)) __attribute__ ((naked)) parkCore1();
int handleVectrexOutputSMP();
void startCore1(unsigned int coreStart);
void v_removeMultiCore();
void handleClient_SMP_WR();

volatile int pendingDisableMultiCore = 0;
volatile int pendingEnableMultiCore = 0;
volatile unsigned int isSMPMode = 0;
volatile int printSemaphore=0;

volatile int pendingDisableInterrupts=0;
volatile int pendingEnableInterrupts=0;
int pendingReturnToPiTrex = 0;
int invertYAxis = 0;


unsigned char calibrationValues[512];

unsigned char calibrationValue_5 = 0;
unsigned char calibrationValue_10 = 0;
unsigned char calibrationValue_15 = 0;
unsigned char calibrationValue_20 = 0;
unsigned char calibrationValue_50 = 0;
unsigned char calibrationValue_100 = 0;


volatile unsigned int vCritical=0;
int irqBreakCheckValue;
int v_forcedBreakInMicroSeconds=0;



float softwareXCalibration = 0.0;
float softwareYCalibration = 0.0;



int inIRQ_MODE = 0;



// if defined ALL debug features, incls terminal mode are off!
//#define NO_DEBUG
// intensive debug output, if "mydebug" is set from terminal
#define MY_DEBUG

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


#ifdef PITREX_DEBUG
int myDebug;
#endif

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




/*  Version 0.x
 */

/***********************************************************************/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>


#include "vectrexInterface.h"

// ff used for state saving
// state are saved in two 1024 byte blobs
// one for vectrexInterface, one for the "user"
// the blobs can be cast after loading/saving to anything one wants...

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
  


  VectorPipeline *wpl = 0;
  VectorPipeline *rpl = 0;


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


/* values that we remember for possible optimization purposes */


uint8_t YSH_A_DELAY; 
uint8_t YSH_B_DELAY; 
uint8_t XSH_A_DELAY; 
uint8_t XSH_B_DELAY; 

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
unsigned int MAXSCALE; //#define MAXSCALE 

uint8_t calibrationValue; // tut calibration


uint8_t swapPorts = 0;
uint8_t optimizationON;
uint8_t useDoubleTimer;
uint8_t usePipeline;
uint8_t consecutiveDraws; // how many lines/moves were drawn/made directly after another! (without zeroing)
uint8_t beamOffBetweenConsecutiveDraws;
uint8_t bufferType;
uint8_t keepDotsTogether;
uint8_t wasIRQMode = 255;
uint8_t inGameSettingsAllowed = 0;
uint8_t doAngleOptimization=0;
uint8_t doOverlappingOptimization=0;
uint8_t angleValue=10; 
uint8_t iniOverwrite=0;
int inSettings=0;


uint8_t intensityMax=127;
uint8_t intensityMin=1;

float intensityMul=1.0;

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
int uartOutput=1;
volatile int isIRQMode = 0;
int clientHz = 50;
int serialCommandsAllowed = 1;

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


int pipelineCleanup = 0;
void v_enableSerialCommands(int yesNo)
{
  serialCommandsAllowed = yesNo;
}
void v_enablePipelineCleanup(int e)
{
  pipelineCleanup = e;
}
void setCalibrationValues();

#include "vectorFont.i" // includes font definition and string printing routines
#include "rasterFont.i" // includes font definition and string printing routines
#include "ini.c"
void setCycleEquivalent(unsigned int i)
{
  printf("Set Cycle EQ = %i\n", i);
  cycleEquivalent = i;
#if RASPPI != 1
    cycleEquivalent += RASPPI3_CYCLE_OFFSET;
#endif  
}
unsigned int getCycleEquivalent()
{
#if RASPPI != 1
    return cycleEquivalent - RASPPI3_CYCLE_OFFSET;
#else    
  return cycleEquivalent;
#endif  
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

char lastAccessedIni[255];

void v_init()
{ 
  printf("v_init()...\n");
  inCalibration = 0;
  lastViaWrite = -1;
  setWaitMin = 0;
  logOutput=0;
  lastAccessedIni[0]=0;
  int *loaderAvailable = (int *)LOADER_AVAILABLE;
  if (*loaderAvailable == 1)
  {
      GlobalMemSettings **settingsPointer;
      settingsPointer = (GlobalMemSettings **)SETTING_STORE;
      settings = *settingsPointer; 
    
      settings->resetType = RESET_TYPE_CONTINUE_LOADER;
      printf("\r\nSettingPointer: %08x, settings: %08x\r\n", settingsPointer, settings);
  }
  else
  {
      printf("No Loader data available... yet?\n");
  }
  
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);

  serialCommandsAllowed = 1;
  // once to ENABLE in general!
  PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET|COUNTER_ZERO);

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

  YSH_A_DELAY = 2;
  YSH_B_DELAY = 2;
  XSH_A_DELAY = 2;
  XSH_B_DELAY = 2; 
  
  
  
  DELAY_AFTER_XSH_VALUE = 2;
  DELAY_AFTER_YSH_VALUE = 8;
  DELAY_PORT_B_BEFORE_PORT_A_VALUE = 2;
  CRANKY_DELAY_Y_TO_NULL_VALUE=6;
  

  MAX_CONSECUTIVE_DRAWS = 65;
  DELAY_ZERO_VALUE = 40; // 70 // probably less, this can be adjusted, by max x position, the nearer to the center the less waits
  DELAY_AFTER_T1_END_VALUE = 20; // last vectors should add 5!!!
  SCALE_STRENGTH_DIF = 2;
  MINSCALE = 5;
  MAXSCALE = 0x500;
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

  
  intensityMax=127;
  intensityMin=1;
  intensityMul=1.0;

  
  for (int i=0;i<MEDIAN_MAX;i++)
  {
    medianX1[i] = 0;
    medianY1[i] = 0;
  }

  crankyFlag=0x14;//CRANKY_NULLING_CALIBRATE | CRANKY_BETWEEN_VIA_B+6; // for is the normal cranky delay after switching port b to MUX_y (or Z)

  setCycleEquivalent(666);
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
  swapPorts = 0;

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

  initFileSystem();
  int pv = 0;
  if (pv = ini_parse("vectrexInterface.ini", iniHandler, 0) < 0) 
  {
        printf("vectrexInterface.ini not loaded!\n\r");
  }
  printf("vectrexInterface.ini-Parser returned: %i\n\r", pv);

  setCalibrationValues();
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
  if (isSMPMode) 
  {
    return; // or add to pipeline?
  }
    
  brightness = brightness*intensityMul;
  if (brightness!=0)
  {
    if (brightness>intensityMax)brightness=intensityMax;
    if (brightness<intensityMin)brightness=intensityMin;
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
  
  // prevent overflow
  
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
  
  
  
  
/*

  int xo = xStart; 
  xStart -= abs(xStart) * softwareXCalibration;
  if ((xo>20000) && (xStart<-20000)) return;
  if ((xo<-20000) && (xStart>20000)) return;

  xo = xEnd; 
  xEnd -= abs(xEnd) * softwareXCalibration;
  if ((xo>20000) && (xEnd<-20000)) return;
  if ((xo<-20000) && (xEnd>20000)) return;
  
  xStart = roundf(xStart * 100000) / 100000;
  xEnd = roundf(xEnd * 100000) / 100000;



/*

  int yo = yStart; 
  yStart -= abs(yStart) * softwareYCalibration;
  if ((yo>20000) && (yStart<-20000)) return;
  if ((yo<-20000) && (yStart>20000)) return;

  yo = yEnd; 
  yEnd -= abs(yEnd) * softwareYCalibration;
  if ((yo>20000) && (yEnd<-20000)) return;
  if ((yo<-20000) && (yEnd>20000)) return;
  
  yStart = roundf(yStart * 100000) / 100000;
  yEnd = roundf(yEnd * 100000) / 100000;
*/






  
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
    if (brightness >= 0)
    {
      brightness = brightness*intensityMul;
      if (brightness!=0)
      {
        if (brightness>intensityMax)brightness=intensityMax;
        if (brightness<intensityMin)brightness=intensityMin;
      }
    }
    
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

    
/*    
    if (pipelineCounter>0)
    {
      VectorPipelineBase *lastpb = &pb[pipelineCounter-1];
      if ((brightness>0) && (lastpb->intensity == brightness))
      {
        if ( (lastpb->y1==cpb->y0) && (lastpb->x1==cpb->x0) )
        {

          if ( (abs(cpb->y1-cpb->y0)<200) && (abs(cpb->x1-cpb->x0)<200) )
          {
            // skip very small vector!
      lastpb->y1 = (cpb->y1) & (~POSTION_MARGIN_AND);
      lastpb->x1 = (cpb->x1) & (~POSTION_MARGIN_AND);

      cpb->pattern = 0;
      cpb->force = 0;
      cpb->debug[0] = 0;
      
//      printf("Small Vector reduction\n");
  return;    
          }

          
        }
      }
      
    }
*/    
    
    
    if (pipelineCounter>MAX_PIPELINE-2)
    {
      printf("PIPELINE OVERFLOW!\n\r");
      return;
    }
    
    
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
/*
static inline int CALIBRATED_Y(unsigned int y) 
{
  unsigned int cyclesNow; 
  CCNT0(cyclesNow);
  double i = (cyclesNow-lastZeroCycles);
  i=(i/10000000.0) * calibrationValue;
  return y+((int)i);
}
static inline int CALIBRATED_X(unsigned int x) 
{
  unsigned int cyclesNow; 
  CCNT0(cyclesNow);
  double i = (cyclesNow-lastZeroCycles);
  i=(i/10000000.0) * calibrationValue;
  return x+((int)i);
}
*/

inline static void v_resetIntegratorOffsets()
{
  // zero to A -> YSH and XSH
  I_SET (VIA_port_b, 0x81); // disable MUX
  DELAY_PORT_B_BEFORE_PORT_A();

  I_SET (VIA_port_a, 0x00);
  ADD_DELAY_CYCLES(4);
  
  I_SET (VIA_port_b, 0x80); // enable MUX Y Int
  ADD_DELAY_CYCLES(6);
  

  // reset integrators

  // zero to A -> I 
  I_SET (VIA_port_b, 0x82);    // mux=1, enable mux - integrator offset = 0
  ADD_DELAY_CYCLES(6);
  I_SET (VIA_port_b, 0x81);    // disable mux

//  possible drift compensation with update of x y pos over time since last zero?
//  CCNT0(lastZeroCycles);
  
  
  
  if (calibrationValue!=0)
  {
    ADD_DELAY_CYCLES(6);
    I_SET (VIA_port_a, calibrationValue);
    ADD_DELAY_CYCLES(6);
    I_SET (VIA_port_b, 0x82);
    DELAY_PORT_B_BEFORE_PORT_A();
    I_SET (VIA_port_a, 0xff);
    ADD_DELAY_CYCLES(2);
    
    waitUntilAndUse(setWaitMin*ONE_WAIT_UNIT - 200);
    vectrexwrite(VIA_port_b, 0x81);
    PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET);
    lastViaWrite = (0);
    setWaitMin = 0;
    DELAY_PORT_B_BEFORE_PORT_A();
    I_SET(VIA_port_a, 0x00);
    
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
#if RASPPI != 1 
  if (isSMPMode)
  {
    handleClient_SMP_WR();
    return;
  }
  else
  {
    if (pendingEnableInterrupts)
    {
      pendingEnableInterrupts = 0;
      v_setupIRQHandling();
      if (isIRQMode)
      {
        handleClient_WR();
        return;
      }
    }
  }
#endif  
  
  #ifdef PITREX_DEBUG
  while (browseMode)
  {
    displayPipeline();

//printf("B");
    if (serialCommandsAllowed) handleUARTInterface();
    if (isSMPMode == 1) return;
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
printf("Return to PiTrex called!\r\n");


  int *loaderAvailable = (int *)LOADER_AVAILABLE;
  if (allowReturnToLoader)
  {
    
    if (*loaderAvailable != 1)
    {
      printf("v1) Loader not available -> not resetting...\r\n");
      return;
    }
    
    printf("V: settings: %08x\r\n", settings);
    
     printf("Loader.flags: %02x\n ",settings->flags);
    
      if (settings->resetType == RESET_TYPE_RESTART_SELECTION)
      {
	if ((settings->flags) &FLAG_LOADER_RUNNING) 
	{
	  printf("toLoader-c: loader already running!\r\n");
	  return;
	}
        printf("V: Restarting selection...\r\n");
        #if RASPPI != 1 
        v_removeMultiCore();
        #endif    
        v_removeIRQHandling();
    	settings->loader();
      }
    
      
      if (settings->resetType == RESET_TYPE_CONTINUE_LOADER)
      {
	  if (settings->flags &FLAG_LOADER_RUNNING) 
	  {
	    printf("toLoader-c: loader already running!\r\n");
	    return;
	  }
	  printf("V: Continue to loader...\r\n");
	  #if RASPPI != 1 
	  v_removeMultiCore();
	  #endif    
	  v_removeIRQHandling();
	  settings->loader();
      }
      /*
      else if (settings->resetType == RESET_TYPE_RESTART_LOADER)
      {
	  printf("V: Restarting loader...\r\n");
	  #if RASPPI != 1 
	  v_removeMultiCore();
	  #endif    
	  v_removeIRQHandling();
	  
	  uint32_t progSpace = *((int *)LOADER_START); // loader soft reset / starts at main()
	  void (*progStart)(void) = (void (*)(void))progSpace;
	  progStart();
      }
      */
      else if (settings->resetType == RESET_TYPE_RESTART_BOOT_NAKED)
      {
	printf("Rebooting...\r\n");
        #if RASPPI != 1 
        v_removeMultiCore();
        #endif    
        v_removeIRQHandling();
        settings->loader();
      }
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
  if (serialCommandsAllowed) handleUARTInterface();
  if (isSMPMode) 
  {
    // switching to IRQ mode via command line
    // reset T2 VIA timer to 50Hz
    resetPipeline();
    return;
  }
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
  if ((currentButtonState&0xf) == 0xf) // button 1+ 2 + 3+4 -> go menu
  {
    printf ("v_WaitRecal_buffered -> calling returnToPiTrex()\n");
    returnToPiTrex();
  }
  if ((currentButtonState&0x20)== 0x20) // button 2 -> joypad 2
  {
    if (inGameSettingsAllowed)
    {
printf("Setting in Calibration\n");      
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
printf("Exiting Setting\n");
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
    int *loaderAvailable = (int *)LOADER_AVAILABLE;
    if (*loaderAvailable == 1)
      settings->loader();
  }
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


// shifted version ONLY!
void v_immediateDraw32Patterned(int8_t xEnd, int8_t yEnd, uint8_t pattern)
{
  int patternAnds[] = {128,64,32,16,8,4,2,1};
  int pCount = 0;
  
  // regardless of BEAM Control - here we use the shift register
  SET (VIA_aux_cntl, 0x98); //Shift Reg. Enabled, T1 PB7 Enabled

  SET_YSH_IMMEDIATE_8(yEnd);

  
  
  waitUntil(cycleEquivalent);
  SET_YSH_IMMEDIATE_8(yEnd);
  waitUntil(cycleEquivalent);

  SET_XSH_IMMEDIATE_8(xEnd);

  START_T1_TIMER();
  while ((GET(VIA_int_flags) & 0x40) == 0)
  {
      SET_SHIFT_REG(pattern);
      PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET);
      
//#if RASPPI != 1
//      waitUntil(15*(640+RASPPI3_CYCLE_OFFSET)+500);
//#else
      waitUntil(15*640+500);
//#endif      
      
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
    
    if (internalButtonState!=0)
    {
      void resetCommand(void);
      resetCommand();
    }
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
    if (internalButtonState!=0)
    {
      void resetCommand(void);
      resetCommand();
    }
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


void handleVectorRequest(int type, int x, int y)
{
}
Command *userCommandList;
#ifdef PITREX_DEBUG
#include "commands.c"
#endif

#include "samples.c"
#include "handleDefs.i"
#include "psgSound.c"
#include "inputDevice.c"

#include "vectrexInterface_bitmap.c"
#include "vectrexInterface_settings.c"

// !!!!
// Basically all 3 different
// drawing approaches use the same pipeline, and the same CODE - since it is one include "pipeline.i" in each of the following "includes"
// the IRQ and SMP
// implementation additionally use macros
// like:
//      HANDLE_SOUND
//      HANDLE_BUTTONS
//      HANDLE_JOYSTICK
//      DEFLOCK
//      IRESET
//
// to... well handle that stuff "automatically" while looping there I/O loop with the Vectrex VIA
// that code is also exactly the same source code, defined in yet another include, namely "handleDefs.i"
//
// Thus all 3 different pipeline output methods use exactly the same code base!


// NORMAL PIPELINE
// BUILDING the pipeline from lines to VectrexPipeline
// also includes the displayPipeline() for a normal (not IRQ not SMP) list
// further included is thus "pipeline.i"
#include "vectrexInterface_pipeline.c" 

// IRQ PIPELINE
// this include the setup and handling 
// of the interrupt system
// (the complete vectrex communication is handled in an interrupt, which has
// obviously priority over any other running program
// the interrupt handler, can give "back" the control to the normal program, 
// which can then execute "normal" stuff, like emulating a cpu
// once the vector drawing needs to continue, the controll is forced back to the
// interrupt handler, and thus it can continue drawing in priority)
// this also includes function "handleVectrexOutput()"
// which basically executes the pipeline, and thus includes again the "pipeline.i"
#include "vectrexInterface_irq.c"

// SMP PIPELINE (multi core)
// this include the setup and handling 
// of the multi core system (Pi Zero 2)
// all communication with the Vectrex (VIA) is handled in core 1
// all program execution is handled in core 0
// this also includes function "handleVectrexOutputSMP()"
// which basically executes the pipeline, and thus includes again the "pipeline.i"
#include "vectrexInterface_smp.c"

// this is not done yet 
// and should build a machine close pipeline, which only consists
// of via read/write and waits
#include "vectrexInterface_VIAPipeline.c"



#include "vectrexInterface_gadgets.c"


