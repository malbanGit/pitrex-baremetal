#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h> // for usleep
#include <errno.h>

#include "vecx.h"

#include <vectrex/vectrexInterface.h>
#include <vectrex/osWrapper.h>
#include <vectrex/ini.h>
#include "e6809.h"
#include "e8910.h"
#include "e6522.h"
#include "edac.h"
#include "ser.h"


// must be same as in loaderMain.c
#define MAX_VECTREX_FILENAME_LENGTH 127


enum
{
    EMU_TIMER = 20, /* the emulators heart beats at 20 milliseconds */
// 
    DEFAULT_WIDTH = 495,
    DEFAULT_HEIGHT = 615
};

static int32_t scl_factor;



int isVecMania = 0;
int isCavernRescue = 0;
int strangeButtonCart = 0;

void loadSelected(int selected);
void initEmulator();

int t1EmulationNeeded = 0;
int doStates=1;

int forceButtons = 0;
char romName[MAX_VECTREX_FILENAME_LENGTH];
char cartName[MAX_VECTREX_FILENAME_LENGTH+1];

unsigned char flashHS[4096];
unsigned char flashGS[4096];

#ifdef FILE_PLAYER
void loadFile();
#endif

uint8_t biosBuffer[4096*2]; // 

uint8_t romBuffer[32768*9]; // lineart
long int bufferState = 0; // 0 = empty, != 0 -> size of buffer


extern volatile int singleStep;
volatile int nextStep = 0;

 
int UARTGetAsciiInt();
int UARTGetBinaryData(int size, unsigned char *romBuffer);


void vLoadBios4KCommand(void)
{
  // expect fileSize in ASCII + return
  // after that exactly that amount of bytes
  int size = UARTGetAsciiInt();
  if ((size == -1) || (size >4096))
  {
      printf("Pi:Illegal Size - load bios4k aborted (%i)!\n", size);
      return;
  }
  
  bufferState = 0; // reset buffer state 
  printf("Pi:size=%i\n", size);
  if (UARTGetBinaryData(size, biosBuffer) != 0) // 0 = ok
  {
      printf("Pi:File Load error - load bios4k aborted!\n");
      return;
  }  
  printf("Pi:Binary Data loaded to bios.\n");
  for (int i=0;i<4096;i++) rom[4096+i] =biosBuffer[i] ;
}

void vLoadBios8KCommand(void)
{
  // expect fileSize in ASCII + return
  // after that exactly that amount of bytes
  int size = UARTGetAsciiInt();
  if ((size == -1) || (size >4096*2))
  {
      printf("Pi:Illegal Size - load bios8k aborted (%i)!\n", size);
      return;
  }
  
  bufferState = 0; // reset buffer state 
  printf("Pi:size=%i\n", size);
  if (UARTGetBinaryData(size, biosBuffer) != 0) // 0 = ok
  {
      printf("Pi:File Load error - load bios8k aborted!\n");
      return;
  }  
  printf("Pi:Binary Data loaded to bios + minestorm.\n");
  for (int i=0;i<4096*2;i++) rom[i] =biosBuffer[i] ;
}


void vLoadRomCommand(void)
{
  // expect fileSize in ASCII + return
  // after that exactly that amount of bytes
  int size = UARTGetAsciiInt();
  if ((size == -1) || (size >32768*9))
  {
      printf("Pi:Illegal Size - load rom aborted (%i)!\n", size);
      return;
  }
  
  bufferState = 0; // reset buffer state 
  printf("Pi:size=%i\n", size);
  if (UARTGetBinaryData(size, romBuffer) != 0) // 0 = ok
  {
      printf("Pi:File Load error - load rom aborted!\n");
      return;
  }  
  printf("Pi:Binary Data loaded to buffer\n");
  bufferState = size;
  printf("Pi:bufferState (%i, size=%i)\n", bufferState, size);
}

void vPlayRomCommand(void)
{
  if (bufferState == 0)
  {
      printf("Pi:Buffer empty! (%i)\n", bufferState);
      return;
  }
  vecx_reset();
  _used_bank_size = BANKSIZE;
  is48kRom =0;
  is256kRom = 0;
  doStates = 0;
  
  void initVectrexBin(unsigned int fsize, uint8_t *loadMem);
  initVectrexBin(bufferState, romBuffer);
}

void vDumpRomCommand(void)
{
  if (bufferState == 0)
  {
      printf("Pi:Buffer empty! (%i)\n", bufferState);
      return;
  }

  char *getParameter(int p);
  char * name = getParameter(0);
  
  if (strlen(name) == 0)
  {
    printf("Pi:No Name given\n");
    return;
  }
  if (strcmp("Pi:invalid parameter", name) ==0)
  {
    printf("Pi:No Name given\n");
    return;
  }
  
  FILE *f;
  if (!(f = fopen(name, "wb")))
  {
      printf("Pi:dump buffer - error opening file: %s!\n", name);
      return;
  }
  fwrite(romBuffer, bufferState, 1, f);
    
  fclose(f);
  printf("Pi:Buffer written to: %s (%bytes)!\n", name, bufferState);
}

void vStepCommand(void)
{
  nextStep = 1;
}
void vNSSCommand(void)
{
  singleStep = 0;
}
void vSSCommand(void)
{
  singleStep = 1;
}

unsigned int vDebugTo = 0;
void vRunCommand(void)
{
  unsigned int adr = bm_atoi(getParameter(0),10);
  vDebugTo = adr;
  singleStep = 1;
  nextStep = 1;
}

Command vectrexCommandList[] =
{
	{1,"vloadBuffer", "vlb", "vloadBuffer | vlb -> load game into Vectrex game buffer\r\n" ,  vLoadRomCommand },
	{1,"vdumpBuffer", "vdb", "vdumpBuffer | vdb name -> save buffer to pitrex\r\n" ,  vDumpRomCommand },
	{1,"vplayBuffer", "vpb", "vplayBuffer | vpb -> play game from Vectrex game buffer\r\n" ,  vPlayRomCommand },
	{1,"vStep",       "+",   "vStep       | +   -> Do a single step of emulation\r\n" ,  vStepCommand },
	{1,"vExitStep", "vx",    "vExitStep   | vx  -> singleStep = 0\r\n" ,  vNSSCommand },
	{1,"vEnterStep", "ve",   "vEnterStep  | ve  -> singleStep = 1\r\n" ,  vSSCommand },
	{1,"vStepTo", "vst",     "vStepTo     | vst $xxxx  -> run till pc =xxxx\r\n" ,  vRunCommand },
	
	{1,"vBios4K", "v4k",     "vBios4K     | v4k name  -> load 4k BIOS to $f000 (direct)\r\n" ,  vLoadBios4KCommand },
	{1,"vBios8K", "v8k",     "vBios8K     | v8k name  -> load 8k BIOS to $e000 (direct)\r\n" ,  vLoadBios8KCommand },
	
	{0,"", "", "" ,  (void (*)(void)) 0 }
};

void initVectrexBin(unsigned int fsize, uint8_t *loadMem)
{
  int c=0;
  
  printf("Rom Size: %i\n",fsize);
  if ((fsize > 49152) && (fsize <= 65536))
  {
    is64kBankSwitch = 1; // assuming
    printf("64k rom loaded, assuming PB6 bankswitching\r\n");
  }
  if (fsize == 262144) // vectorblade alike 4 banks
  {
     is48kRom = 1;
     is256kRom = 1;
     is64kBankSwitch = 1;
      _used_bank_size = 2;
     printf("48k - Vecctorblade style!\n");
  }

  else if ((fsize>32768) && (fsize<65536))
  {
      is48kRom = 1;
      printf("48k (64) Banks! (%i) \n",is48kRom);
      _used_bank_size = 2;
  }
  else
    printf("32k Banks!\n");

  for (int i=0; i< 32768*9; i++)
  {
    cart[i] = loadMem[i];
  }

  void initRomMemory(int fsize, uint8_t *loadMem);
  initRomMemory(fsize, loadMem);
}


static int readevents(void)
{
    vecx_input(VECTREX_PAD1_X, currentJoy1X<-80?0x00:currentJoy1X>80?0xff:0x80);
    vecx_input(VECTREX_PAD1_Y, currentJoy1Y<-80?0x00:currentJoy1Y>80?0xff:0x80);
    return 0;
}

static void render(void)
{
  v_readButtons(); // read buttons to check if we should enter calibration on init
  v_readJoystick1Analog(); // read buttons to check if we should enter calibration on init
  v_WaitRecal();
  readevents();
  for (size_t v = 0; v < vector_draw_cnt; v++)
  {
      uint8_t c = vectors[v].color ;//* 256 / VECTREX_COLORS;
      int32_t x0 = (vectors[v].x0 )-(DAC_MAX_X/2);
      int32_t y0 = -((vectors[v].y0 )-(DAC_MAX_Y/2));
      int32_t x1 = (vectors[v].x1 )-(DAC_MAX_Y/2) ;
      int32_t y1 = -((vectors[v].y1 )-(DAC_MAX_X/2) );
      v_directDraw32(x0,y0,x1,y1, c );
  }
}

static void resize(void)
{
    int sclx, scly;
    int screenx, screeny;
    int width, height;
	return;
}


volatile uint64_t bcm2835_st_read(void); // from pitrexio-gpio.c

#define GET_SYSTEM_TIMER bcm2835_st_read()
 
int orgbetterRasterPositioning; // saving vectrexInterface value for the time "after" vectrex emulation (PiTrex)
extern uint32_t cycleCount;
uint32_t cycleCount_mark;
int vectrexResetCount = 0;

void handleUARTInterface();

static void emuloop(void)
{
    vecx_reset();
    cycleCount_mark = 0;
    uint32_t mark = (uint32_t)(GET_SYSTEM_TIMER&0xffffffff);

    char stateName[MAX_VECTREX_FILENAME_LENGTH];
    int c=0;
    stateName[c++]='s';
    stateName[c++]='t';
    stateName[c++]='a';
    stateName[c++]='t';
    stateName[c++]='e';
    stateName[c++]='s';
    stateName[c++]='/';
    if (doStates)
	{
      for (int ii=0;c<MAX_VECTREX_FILENAME_LENGTH;ii++)
      {
		stateName[c++]=cartName[ii];
		if (cartName[ii]==0) break;
      }
	}
    // if game was started with button 1 (instead of 4)
    // the state is loaded!
    if (getLoadParameter(2)[0] == 1)
    {
      if (doStates==1)
      {
		vecx_load(stateName);
      }
      // todo init VIA
    }

    for (;;)
    {
      if (isTerminalAccessAllowed) 
      {
		handleUARTInterface();
      }
#ifdef DIRECT_EMULATION
      vecx_direct(10000);
	  
	  if (singleStep)
	  {
	    if (vDebugTo != 0)
	    {
			handleUARTInterface();
	    }
	    else
	    {
	      nextStep = 0;
	      while ((nextStep==0) && (singleStep==1))
	      {
			handleUARTInterface();
	      }
	    }
	  }
	  
	  
    #ifndef DO_NOT_WATCH_VIA_FOR_BUTTONS
            if (vectrexButtonState != 0x1ff)
            {
			 int pressed = 0;
			 pressed = ((vectrexButtonState & 0xff) == 0xf0);
	//	     if (strangeButtonCart) pressed = ((vectrexButtonState & 0xff) == 0xf0);
	//	     else pressed = ((vectrexButtonState & 0x0f) == 0x00);
	     
              if (pressed) 
              {

				vectrexResetCount++;
                if (vectrexResetCount == 10) // 1/20 second 
                {
                  printf("Vectrex 4 button press detected %02x!\r\n", vectrexButtonState);
                  if (doStates==1)
                  {
                    printf("Starting to save to: %s\r\n", stateName);
                    vecx_save(stateName);
                    printf("Save done: %s\r\n", stateName);
                  }
                  betterRasterPositioning = orgbetterRasterPositioning;

                  returnToPiTrex();
                }
              }
              else
              {
                  if (doStates==1)
                  {
					if ((isVecMania) || (isCavernRescue))
					{
					  if ((vectrexButtonState & 0xff) == 0xff-0x80) // JUST ONLY button 4 joy 2
					  {
						vecx_load(stateName);
					  }
					  if ((vectrexButtonState & 0x10) == 0xff-0x10) // JUST ONLY button 1 joy 2
					  {
						vecx_save(stateName);
					  }
					}
					else
					{
					  if ((vectrexButtonState & 0x80) == 0x00) // button 4 joy 2
					  {
						vecx_load(stateName);
					  }
					  if ((vectrexButtonState & 0x10) == 0x00) // button 1 joy 2
					  {
						vecx_save(stateName);
					  }
					}
                  }
                  vectrexResetCount = 0;
              }
              vectrexButtonState = 0x1ff;
            }
    #endif
#else
        vecx_emulate(10000);

        if (ram[0xC80F & 0x3ff] == 0x0f) // 4 buttons
        {
            vecx_save(stateName);
            returnToPiTrex();
        }
#endif      

#ifdef OUTPUT_SPEED
        uint32_t mark2 = (uint32_t)(GET_SYSTEM_TIMER&0xffffffff);

        if ( mark2 >= mark +1000000) // 1 second
        {
          mark = mark2;
          uint32_t percent = ((cycleCount-cycleCount_mark)*100) / (1500000);
          printf("Emulation percent: %i\n\r", (int)percent);
          cycleCount_mark = cycleCount;
        }
#endif        
    }
}

static void load_overlay()
{

}

#define SETTINGS_SIZE 1024
unsigned char settingsBlob[SETTINGS_SIZE];


// for now INI setting just stupidly overwrite other saved settings!
static int vectrexIniHandler(void* user, const char* section, const char* name, const char* value)
{
  bootUpName[0]=(char)0; // init "no boot title"
  // cascading ini files
  // first check if there are "general" entries
  if (iniHandler(user, section, name, value) == 1) return 1;

  
  #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
  #define MATCH_NAME(n) strcmp(name, n) == 0
  if (MATCH("VECTREX_EXACT", "TERMINAL_ACCESS")) {isTerminalAccessAllowed = atoi(value);} else 
  if (MATCH("VECTREX_EXACT", "INTERNAL_STRING_DISPLAY")) {isForcedinternalStringDisplay = atoi(value);} else 
  if (MATCH("VECTREX_EXACT", "IS_48_KROM")) {is48kRom = atoi(value); printf("48 - 0\n");} else 
  if (MATCH("VECTREX_EXACT", "CART_BOOT_FILE")) strcpy(bootUpName, value); else 

  if (MATCH("VECTREX_EXACT", "ROM_FILE")) strcpy(romName, value); else  
  if (MATCH("VECTREX_EXACT", "HANDLE_STATES")) doStates = atoi(value); else 
  {
      printf("Ini-Handler, unkown entry: %s = %s", name, value);
      return 0;  /* unknown section/name, error */
  }
  return 1;
}

// called once from main
static int init(void)
{
  orgbetterRasterPositioning = 0;
  romName[0] = 0;
  _used_bank_size = BANKSIZE;

  vectrexinit(1); // pitrexio-gpio.c
  printf("VectrexExact - loaded!");
  v_init();

#ifdef PITREX_DEBUG
  userCommandList = vectrexCommandList;
#endif

  
  v_removeIRQHandling();
  usePipeline = 0;
  vectrexResetCount = 0;
  forceButtons = 0;
  
  int pv = 0;
  if (pv = ini_parse("vectrex.ini", vectrexIniHandler, 0) < 0) 
  {
        printf("vectrex.ini not loaded!\n\r");
  }
  
  if (is48kRom == 1)
    _used_bank_size = 2;
  orgbetterRasterPositioning = betterRasterPositioning;
  
  
// somehow, the fileplayer does not like this
// I have no explanation... it looks like the timer is called more than once 	    
// but I can't find it...
// also this does only "reset" when the complete PiTrex is resetted - not logical at all!	    
#ifdef FILE_PLAYER
  isForcedinternalStringDisplay=0;
  printf("Forced String display not allowed with movie player.");
#endif
  
  
  if (isForcedinternalStringDisplay == 1)
  {
       printf("Alternate String display enabled.\n\r");
       betterRasterPositioning = 1;
  }
  else 
       printf("Alternate String display disabled: \n\r", isForcedinternalStringDisplay);
  
  if (romName[0] != 0)
  {
    // load bios
    char *dir = "vectrex";

    int err=0;
    err = chdir (dir);
    if (err)
    {
      printf("vectrexBase.main.c:init(): NO vectrex directory found...(%i)!\r\n", errno);
      return 1; // return Ok, since we still have the "hard BIOS"
    }

    FILE *fileRead;
    fileRead = fopen(romName, "rb");
    if (fileRead == 0)
    {
      printf("vectrexBase.main.c:init(): Could not open file %s (%i) \r\n", romName, errno);
      err = chdir("..");
      return 1; // return Ok, since we still have the "hard BIOS"
    }

    unsigned int lenLoaded=0;
    lenLoaded = fread(rom, 8192, 1, fileRead);
    if (1 != lenLoaded)
    {
      printf("vectrexBase.main.c:init(): Read(1) of %s fails (len loaded: %i) (Error: %i)\r\n", romName, lenLoaded, errno);
    }
    else
    {
      printf("Rom loaded: %s\r\n", romName);
    }
    fclose(fileRead);
    err = chdir("..");
  }
  return 1;
}

void main()
{
    init();
printf("logOutput: %i\r\n", logOutput);    

#ifdef CUTOM_INIT
    if (customInit())
    {
      printf("Custom Init\r\n");
      initEmulator();
    }
#else
    printf("Non Custom Init\r\n");

    initEmulator();
#endif    
#ifdef FILE_PLAYER    
    loadFile();
#endif
    resize();
    e8910_init();
    vecx_render = render;
    emuloop();
}

char name1[MAX_VECTREX_FILENAME_LENGTH];
char name2[MAX_VECTREX_FILENAME_LENGTH];
char name3[MAX_VECTREX_FILENAME_LENGTH];
char name4[MAX_VECTREX_FILENAME_LENGTH];

char *names[] =
{
  name1,
  name2,
  name3,
  name4,
};
int NAME_COUNT = 4;
void translateName(char *from, char *to);
int fillDirNames(int startWith);


int maxInDir = 0;
void initEmulator()
{
    char *vectrexDir = "vectrex";
    if (chdir(vectrexDir)<0)
    {
        printf("vectrexBase.main.c:initEmulator(): NO vectrex directory found...!\r\n");
        return;
    }
    // see if there is a directory "vectrex"
    int startWith = 0;
    int nameResult = fillDirNames(startWith);
    if (nameResult<0) return;
    int selectedMenu = 0;
    int selectionMade=0;
    
    while(1)
    {
      v_WaitRecal();
      v_readButtons();
      v_readJoystick1Analog();
      v_setBrightness(64);        /* set intensity of vector beam... */
      if ((currentButtonState&0x04) == (0x04))
      {
        chdir("..");
        getLoadParameter(2)[0] = 4;
        loadSelected(selectedMenu);
        return;
      }
      
      // load state if available
      if ((currentButtonState&0x01) == (0x01))
      {
        chdir("..");
        getLoadParameter(2)[0] = 1;
        loadSelected(selectedMenu);
        return;
      }
      
      if ((currentJoy1Y>50) && (selectionMade==0))
      {
          selectedMenu--;
          if (selectedMenu<0) selectedMenu=0;
          selectionMade = 1;
      }
      if ((currentJoy1Y<-50) && (selectionMade==0))
      {
          selectedMenu++;
          if (selectedMenu>=NAME_COUNT) selectedMenu=NAME_COUNT-1;
          selectionMade = 1;
      }
      if ((currentJoy1X>50) && (selectionMade==0))
      {
        if (nameResult == NAME_COUNT)
          startWith+=4;
          nameResult = fillDirNames(startWith);
        selectionMade = 1;
      }
      if ((currentJoy1X<-50) && (selectionMade==0))
      {
        if (startWith>=4)
          startWith-=4;
          nameResult = fillDirNames(startWith);
        selectionMade = 1;
      }
      if ((ABS(currentJoy1Y)<20) && (ABS(currentJoy1X)<20)) selectionMade =0;

      int x = -40;
      int y = 80;
      char *selected = ">";
      for (int i=0;i<NAME_COUNT;i++)
      {
          char translatesName[40];
          translateName(names[i],translatesName);
          v_printStringRaster(x, y, translatesName, 40, -5, 0);
          if (i == selectedMenu)
          {
            v_printString(-60, y-7, selected, 5, 70);
          }
          y-=20;
      }
    }
}

void loadVectrexBin(char *selectedName, uint8_t *loadMem)
{
  // prepareName
  char *loadName;
  char loadName_r[128+MAX_VECTREX_FILENAME_LENGTH];
  int c=0;
  
  
  if (bootUpName[0] != (char) 0)
  {
      // load a forced rom from filesystem - anywhere it points to!
    loadName = bootUpName;
    for (int ii=0;c<MAX_VECTREX_FILENAME_LENGTH;ii++)
    {
      cartName[ii]=bootUpName[ii];
      if (bootUpName[ii]==0) break;
    }
  }
  else
  {
    loadName = loadName_r;
    loadName[c++]='v';
    loadName[c++]='e';
    loadName[c++]='c';
    loadName[c++]='t';
    loadName[c++]='r';
    loadName[c++]='e';
    loadName[c++]='x';
    loadName[c++]='/';
    
    for (int ii=0;c<MAX_VECTREX_FILENAME_LENGTH;ii++)
    {
      loadName[c++]=selectedName[ii];
      cartName[ii]=selectedName[ii];
      if (selectedName[ii]==0) break;
    }
  }
  

  printf("Loading: %s \r\n", loadName);
  
  FILE *f;
  f = fopen(loadName, "r");
  if (f == 0)
  {
            printf("vectrexBase.main.c:loadVectrexBin(): Could not open file %s (%i) \r\n", loadName, errno);
	    return;
  }
  unsigned int fsize = __filelength(f);
  
  if ((fsize > 64000) && (fsize < 70000))
  {
    is64kBankSwitch = 1; // assuming
    printf("64k rom loaded, assuming PB6 bankswitching\r\n");
  }
  printf("Rom Size: %i\n",fsize);

#ifdef BANKS_48
  is48kRom = 1;
  printf("48 - 1\n");
#endif
  if (fsize == 262144) // vectorblade alike 4 banks
  {
     is48kRom = 1;
     is256kRom = 1;
      _used_bank_size = 2;
      is64kBankSwitch = 1;
     printf("48k - Vecctorblade style!\n");
  }
  else if ((fsize>32768) && (fsize<65536))
  {
    printf("48 - 2\n");
    is48kRom = 1;
  }
  
  if ((is48kRom==1) || (  is256kRom == 1))
  {
      printf("48k (64) Banks! (%i) \n",is48kRom);
      _used_bank_size = 2;
  }
  else
    printf("32k Banks!\n");
  
  int sizeRead = fread(loadMem, 1,fsize , f);
  fclose(f);
  if ( sizeRead== 0)
  {
      printf("vectrexBase.main.c:loadVectrexBin(): File not loaded (%s)\r\n", loadName);
      return;
  }
  void initRomMemory(int fsize, uint8_t *loadMem);
  initRomMemory(fsize, loadMem);
  
}

void initRomMemory(int fsize, uint8_t *loadMem)
{
    // file is loaded
    printf("Starting loaded file...\r\n");
    if ((fsize <32768*2) && (is48kRom==0) && (  is256kRom == 0))
    {
        printf("Matching all 8 (32k) banks to loaded bank\r\n");
        // fill all banks with the loaded ROM
        for (int r=0;r<32768;r++)
        {
          loadMem[r+32768*2*1] = loadMem[r+32768*2*2] = loadMem[r+32768*2*3] = loadMem[r+32768*2*0];
          loadMem[r+32768*1*1] = loadMem[r+32768*1*1] = loadMem[r+32768*1*2] = loadMem[r+32768*1*3] = loadMem[r+32768*2*0];
        }
    }
    else
    if (fsize <32768*4) 
    {
        printf("Matching all 4 (64k) banks to loaded (64k?) bank\r\n");
        // fill all banks with the loaded ROM
        for (int r=0;r<32768*2;r++)
        {
          loadMem[r+32768*2] = loadMem[r];
          loadMem[r+32768*4] = loadMem[r];
          loadMem[r+32768*6] = loadMem[r];
        }
    }
    
    printf("%04x    ", 0);
    for (int i=0;i<128;i++)
    {
      if ((i%16==0) && (i!=0))
      {
	for (int ii=i-16;ii<i;ii++)
	{
	  if ((loadMem[ii]>31) && (loadMem[ii]<128))
	    printf("%c",loadMem[ii]);
	  else
	    printf(".");
	}
	printf("\n%04x    ", i);
      }
      printf("%02x ",loadMem[i]);
    }
    printf("\n");
    
    // test for T1 (at the moment SPIKE
    t1EmulationNeeded = 1;
    unsigned char test1[]={0x64,0x20,0x47,0x43,0x45,0x20,0x31,0x39,0x38,0x33,0x80,0x0f,0x43,0xf8,0x48,0x20,0xe0,0x53,0x50,0x49,0x45,0x80,0,0xbd};
    int i=0;
    while (test1[i] != 0xbd)
    {
      if (loadMem[i] != test1[i])  
      {
        t1EmulationNeeded = 0;
        break;
      }
      i++;
    }
    int isVecMania1 = 1;                     
   
    unsigned char test2a[]={0x67, 0x20, 0x47, 0x43, 0x45, 0x20, 0x4e, 0x4f, 0x54, 0x21, 0x80, 0x0f, 0x96, 0xf8, 0x40, 0x2f,
			   0x90,0x49,0x54,0x20,0x57,0x41,0x53,0x20,0x57,0x4f,0x52,0x54,0x48,0x20,0x54,0x48,0x45};
    i=0x0;
    while (test2a[i] != 0x48)
    {
      // printf("%02x - %02x \n",isVecMania,  loadMem[i]);
      if (loadMem[i] != test2a[i])
      {
        isVecMania1 = 0;
        break;
      }
      i++;
    }

    int isVecMania2 = 1;  
    unsigned char test2b[]={0x67, 0x20, 0x47, 0x43, 0x45, 0x20, 0x4e, 0x4f, 0x54, 0x21, 0x80, 0x0fe, 0xe6, 0xf8, 0x40, 0x2f,
			   0x90,0x49,0x54,0x20,0x57,0x41,0x53,0x20,0x57,0x4f,0x52,0x54,0x48,0x20,0x54,0x48,0x45};
    i=0x0;
    while (test2b[i] != 0x48)
    {
      // printf("%02x - %02x \n",isVecMania,  loadMem[i]);
      if (loadMem[i] != test2b[i])
      {
        isVecMania2 = 0;
        break;
      }
      i++;
    }
    isVecMania = isVecMania2+isVecMania1;                     

    
    isCavernRescue = 1;
    unsigned char test3[]={0x67, 0x20, 0x47, 0x43, 0x45, 0x20, 0x32, 0x30, 0x32, 0x32, 0x80, 0xfe, 0xc5, 0xf5, 0x53, 0x20,
			   0xab,0x43,0x41,0x56,0x45,0x52,0x4e,0x20,0x52,0x45,0x53,0x43,0x55,0x45,0x80,0xfa,0x45};
    i=0x0;
    while (test3[i] != 0xfa)
    {
      // printf("%02x - %02x \n",isCavernRescue,  loadMem[i]);
      if (loadMem[i] != test3[i])
      {
        isCavernRescue = 0;
        break;
      }
      i++;
    }

    int isMine3d = 1;  
    unsigned char test3b[]={0x67, 0x20, 0x47, 0x43, 0x45, 0x20, 0x31, 0x39, 0x38, 0x33, 0x80, 0xed,0x77, 0xf8, 0x50, 0x00, 0xb0,
			   0x33,0x44,0x20,0x4d,0x49,0x4e,0x45,0x20,0x53,0x54,0x4f,0x52,0x4d,0x80,0x54,0x48,0x45};
    i=0x0;
    while (test3b[i] != 0x52)
    {
      // printf("%02x - %02x \n",isVecMania,  loadMem[i]);
      if (loadMem[i] != test3b[i])
      {
        isMine3d = 0;
        break;
      }
      i++;
    }
    strangeButtonCart = isVecMania+isCavernRescue+isMine3d;
    

    
    printf("StrangeButtonCart: %i\n",strangeButtonCart );
//    printf("Vecmania: %i\n",isVecMania );
//    printf("isCavernRescue: %i\n",isCavernRescue );
}

void loadSelected(int selected)
{
  char *selectedName = names[selected];
  uint8_t *loadMem = cart;
  loadVectrexBin(selectedName, loadMem);
}


// to upper case :-)
void translateName(char *from, char *to)
{
  int i=0;
  char c=1;
  while (c!=0)
  {
    c = *from++;
    if ((c>'a') && (c<'z'))
    {
      c = c-('a'-'A');
    }
    *to++=c;
  }

}

// return 1 on error
// otherwise the count of "names" filled
int fillDirNames(int startWith)
{
    char buf[256];
    if (getcwd (buf,256)==0)
    {
      printf("vectrexBase.main.c:fillDirNames(): f_getcwd failed (%i) \r\n", errno);
      chdir("..");
      return -1;
    }
//    printf("Current directory: %s \r\n", buf);
    DIR *dp;
    dp = opendir (buf);
    if (dp == 0)
    {
      printf("vectrexBase.main.c:fillDirNames(): opendir failed (%i) \r\n", errno);
      chdir("..");
      return -2;
    }
//    printf("Contents: \r\n");
//    printf("--------- \r\n");
    dirent *finfo;
    int currentCounter=0;
    maxInDir = 0;
    int selected = 0;
    do
    {
      finfo =  readdir(dp);
      if (finfo != 0)
      {
      if (currentCounter>=startWith)
      {
                if (currentCounter<startWith+NAME_COUNT)
                {
                  int i = currentCounter-startWith;
                  char *nameToFill=names[i];
                  int c=0;
                  for (; c<MAX_VECTREX_FILENAME_LENGTH-1; c++)
                  {
                    nameToFill[c]=finfo->d_name[c];
                    if (nameToFill[c]==0) break;
                  }
                  nameToFill[c]=0;
                  selected++;
                }
      }
//      printf("\t %s\r\n",finfo->d_name);
      maxInDir++;
      currentCounter++;
      }
    } while (finfo != 0);
    
//    printf("\r\n");
    closedir (dp);
    return selected;
}

#ifdef FILE_PLAYER    
void loadFile()
{
  char *selectedName = getLoadParameter(0); // file is always the first parameter
  char *s;
  // prepareName
  char loadName[255]; 

  int p=0;
  int c=0;
  while ((s = getLoadParameter(p++)) != 0)
  {
    if (*s < ' ') break; // third parameter is the "button"...
    printf("Paramter %i: %s \r\n",p-1, s);
    while (*s != 0)
    {
      loadName[c++]=*(s++);
    }
    loadName[c++]='/';
  }
  loadName[c-1]=0;

  printf("Loading: %s \r\n", loadName);
  
  FILE *f;
  f = fopen(loadName, "r");
  if (f == 0)
  {
        printf("vectrexBase.main.c:loadFile(): Could not open file %s (%i) \r\n", loadName, errno);
  }
  else
  {
    unsigned int fsize = __filelength(f);
    if (fsize > 1024*1024*20) fsize = 1024*1024*20 -2;
    movieLength = fsize;
    int sizeRead = fread(movieRom, 1,fsize , f);
    if ( sizeRead== 0)
    {
        fclose(f);
        printf("vectrexBase.main.c:loadFile(): File not loaded (%s)\r\n", loadName);
    }
    else
    {
        fclose(f);
        // file is loaded
        printf("File loaded... %i\r\n", sizeRead);
    }
  }
}
#endif


#include "ser.c"


