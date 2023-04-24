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

enum
{
    EMU_TIMER = 20, /* the emulators heart beats at 20 milliseconds */
// 
    DEFAULT_WIDTH = 495,
    DEFAULT_HEIGHT = 615
};

static int32_t scl_factor;

int isVecMania = 0;

void loadSelected(int selected);
void initEmulator();

int t1EmulationNeeded = 0;
int doStates=1;

int forceButtons = 0;
char romName[32];
char cartName[64];

unsigned char flashHS[4096];
unsigned char flashGS[4096];

#ifdef FILE_PLAYER
void loadFile();
#endif

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
 

extern uint32_t cycleCount;
uint32_t cycleCount_mark;

int vectrexResetCount = 0;

static void emuloop(void)
{
    vecx_reset();
    cycleCount_mark = 0;
    uint32_t mark = (uint32_t)(GET_SYSTEM_TIMER&0xffffffff);

char stateName[128];
int c=0;
  stateName[c++]='s';
  stateName[c++]='t';
  stateName[c++]='a';
  stateName[c++]='t';
  stateName[c++]='e';
  stateName[c++]='s';
  stateName[c++]='/';
  for (int ii=0;c<50;ii++)
  {
    stateName[c++]=cartName[ii];
    if (cartName[ii]==0) break;
  }

  // if game was started with button 1 (instead of 4)
  // the state is loaded!
  if (getLoadParameter(2)[0] == 1)
  {
    if (doStates==1)
      vecx_load(stateName);
    // todo init VIA
  }

    for (;;)
    {
      //handleUARTInterface();
#ifdef DIRECT_EMULATION
          vecx_direct(10000);
    #ifndef DO_NOT_WATCH_VIA_FOR_BUTTONS
            if (vectrexButtonState != 0x1ff)
            {
//              if ((vectrexButtonState & 0xff) == 0xf0) // only 4 button of joy 1 (Vecmania does something strange and presses all 8! buttons!
	      
	     int pressed = 0;
	     if (isVecMania) pressed = ((vectrexButtonState & 0xff) == 0xf0);
	     else pressed = ((vectrexButtonState & 0x0f) == 0x00);
	     
	     
	     
              if (pressed) 
              {
//printf("Vectrex 1 button press detected $%02x!\r\n", vectrexButtonState);

		vectrexResetCount++;
                if (vectrexResetCount == 10) // 1/20 second 
                {
                  printf("Vectrex 4 button press detected!\r\n");
                  if (doStates==1)
                  {
                    printf("Starting to save to: %s\r\n", stateName);
                    vecx_save(stateName);
                    printf("Save done: %s\r\n", stateName);
                  }
                  returnToPiTrex();
                }
              }
              else
              {
                  if (doStates==1)
                  {
		    if (isVecMania)
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
  // cascading ini files
  // first check if there are "general" entries
  if (iniHandler(user, section, name, value) == 1) return 1;

  
  #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
  #define MATCH_NAME(n) strcmp(name, n) == 0

  if (MATCH("VECTREX_EXACT", "ROM_FILE")) strcpy(romName, value); else 
  if (MATCH("VECTREX_EXACT", "HANDLE_STATES")) doStates = atoi(value); else 
  {
      printf("Ini-Handler, unkown entry: %s = %s", name, value);
      return 0;  /* unknown section/name, error */
  }
  return 1;
}


static int init(void)
{
  romName[0] = 0;
  vectrexinit(1);
  v_init();
  v_removeIRQHandling();
  usePipeline = 0;
//  v_loadSettings("vecxDirect", settingsBlob, SETTINGS_SIZE);
  vectrexResetCount = 0;
  forceButtons = 0;
  
  int pv = 0;
  if (pv = ini_parse("vectrex.ini", vectrexIniHandler, 0) < 0) 
  {
        printf("vectrex.ini not loaded!\n\r");
  }
  
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

char name1[40];
char name2[40];
char name3[40];
char name4[40];

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
  char loadName[50];
  int c=0;
  loadName[c++]='v';
  loadName[c++]='e';
  loadName[c++]='c';
  loadName[c++]='t';
  loadName[c++]='r';
  loadName[c++]='e';
  loadName[c++]='x';
  loadName[c++]='/';
  for (int ii=0;c<50;ii++)
  {
    loadName[c++]=selectedName[ii];
    cartName[ii]=selectedName[ii];
    if (selectedName[ii]==0) break;
  }

  printf("Loading: %s \r\n", loadName);
  
  FILE *f;
  f = fopen(loadName, "r");
  if (f == 0)
  {
            printf("vectrexBase.main.c:loadVectrexBin(): Could not open file %s (%i) \r\n", loadName, errno);
  }
  else
  {
    unsigned int fsize = __filelength(f);
    
    if ((fsize > 64000) && (fsize < 70000))
    {
      is64kBankSwitch = 1; // assuming
      printf("64k rom loaded, assuming PB6 bankswitching\r\n");
/*
      for (int r=0;r<32768*2;r++)
      {
	loadMem[r+32768*2] = loadMem[r];
	loadMem[r+32768*4] = loadMem[r];
	loadMem[r+32768*6] = loadMem[r];
      }
*/      
    }
#ifdef BANKS_48
printf("48k (64) Banks!\n");
#else
printf("32k Banks!\n");
#endif
    
    int sizeRead = fread(loadMem, 1,fsize , f);
    fclose(f);
    if ( sizeRead== 0)
    {
        printf("vectrexBase.main.c:loadVectrexBin(): File not loaded (%s)\r\n", loadName);
        return;
    }
    // file is loaded
    printf("Starting loaded file...\r\n");
    if (fsize <32768*2)
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
    isVecMania = 1;                     
    
    unsigned char test2[]={0x67, 0x20, 0x47, 0x43, 0x45, 0x20, 0x4e, 0x4f, 0x54, 0x21, 0x80, 0x0f, 0x96, 0xf8, 0x40, 0x2f,
			   0x90,0x49,0x54,0x20,0x57,0x41,0x53,0x20,0x57,0x4f,0x52,0x54,0x48,0x20,0x54,0x48,0x45};
    i=0x0;
    while (test2[i] != 0x48)
    {
    printf("%02x - %02x \n",isVecMania,  loadMem[i]);
      if (loadMem[i] != test2[i])
      {
        isVecMania = 0;
        break;
      }
      i++;
    }
    printf("Vecmania: %i\n",isVecMania );
  }
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
                  for (; c<39; c++)
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


