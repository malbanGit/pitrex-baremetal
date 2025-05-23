#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include <vectrex/vectrexInterface.h>
#include <baremetal/pitrexio-gpio.h>
#include <vectrex/osWrapper.h>
#include <vectrex/mathSupport.h>

#if RASPPI != 1 
#define IMG_FILE_PREFIX "piZero2/"
#else
#define IMG_FILE_PREFIX "piZero1/"
#endif

// must be same as in vectrex/main.c
#define MAX_VECTREX_FILENAME_LENGTH 127

#define MAX_V_FILES 1000
#define MAX_V_DIRS 100
#define MAX_LOAD (1024*1024*100) // 100 MB




GlobalMemSettings **settingsPointer;
GlobalMemSettings loaderSettings
=
{
  0, // flags
  0, // loader main
  0, // orientation
  0, // lastSelection
  "                                                                                                                               ", // parameter 1
  "                                                                                                                               ", // parameter 2
  "                                                                                                                               ", // parameter 3
  "                                                                                                                               ", // parameter 4
  LOADER_STARTING // resetType;
};

int *loaderAvailable = (int *)LOADER_AVAILABLE;
//int *loaderMain = (int *)LOADER_MAIN;


char subdirectoryPath[MAX_VECTREX_FILENAME_LENGTH];
char _dirsInDir[MAX_V_DIRS][MAX_VECTREX_FILENAME_LENGTH];
char _filesInDir[MAX_V_FILES+MAX_V_DIRS][MAX_VECTREX_FILENAME_LENGTH];
char *dirsInDir[MAX_V_DIRS];
char *filesInDir[MAX_V_FILES+MAX_V_DIRS];
char fileTypes[MAX_V_FILES+MAX_V_DIRS];
int currentSelectedItem = 0;
int currentSelectedItem2 = 0;



typedef struct MenuItemNew {
  int id;
  char *DISPLAYNAME;
  char *dir;
  char *img;
  char *param1; // max 127 chars
  char *param2; // max 127 chars
  const signed char *icon;
  const signed char *smallIcon; // used for parent display
  void *parent;
  void *child;
  void *left;
  void *right;
  char *scrolltext;
  char *text[];
} MenuItemNew;
MenuItemNew *currentMenuItem;

void ensureMainPath();


void cleanup_before_linux (void)
{
	/*
	 * this function is called just before we call linux
	 * it prepares the processor for linux
	 *
	 * we turn off caches etc ...
	 */
	mmu_disable(); 
	cache_flush(); /* flush I/D-cache */
}

void startWithCurrentSettings()
{
    loaderSettings.flags &= 255-FLAG_LOADER_RUNNING;
    isb();
    dsb();
    dmb();
    cleanup_before_linux();
  __asm__ __volatile__(
      "mov r5, #0x4000000   \n\t"
      "sub r5, r5, #100 \n\t"
      "ldr r0, [r5]      \n\t"

      "add r5, r5, #4 \n\t"
      "ldr r1, [r5]      \n\t"

      "add r5, r5, #4 \n\t"
      "ldr r2, [r5]      \n\t"

      "ldr pc, = 0x8000  \n\t"
    );
}

// for dev only
void reloadLoader()
{
    v_removeIRQHandling();
#if RASPPI != 1 
    v_removeMultiCore();
#endif    
#if RASPPI != 1 
    char *FILE_NAME = "pitrex7.img";
#else
    char *FILE_NAME = "pitrex.img";
#endif    
    void *progSpace = (void *) 0x8000; 

    ensureMainPath();
    
    FRESULT rc_rd = FR_DISK_ERR;
    FIL file_object_rd;
    rc_rd = f_open(&file_object_rd, FILE_NAME, (unsigned char) FA_READ);
    
    v_noSound();
    
    printf("Loading: %s \r\n", FILE_NAME);
    unsigned int fsize = MAX_LOAD;


    rc_rd = f_read(&file_object_rd, progSpace, fsize, &fsize);
    if ( rc_rd!= FR_OK)
    {
      printf("loaderMain.c: reloadLoader(): File not loaded (%s) (error got = %i)\r\n", FILE_NAME, rc_rd);
      f_close(&file_object_rd);
    }
    else
    {
        f_close(&file_object_rd);

        printf("Booting with %s... \r\n", FILE_NAME);
	*loaderAvailable=0;
	//*loaderMain = (int)0;
	startWithCurrentSettings();
    }
}


char dirBuffer[MAX_VECTREX_FILENAME_LENGTH*2];

void loadAndStart(MenuItemNew *item, int button)
{
    ensureMainPath();

    v_removeIRQHandling();
#if RASPPI != 1 
    v_removeMultiCore();
#endif    

    void *progSpace = (void *) 0x8000; 
    char *FILE_NAME = item->img;
    loaderSettings.lastSelection = item;
    
    // parameter 3 always contains button
    // that the selected item waas started with!
    loaderSettings.parameter3[0] = button;
    loaderSettings.parameter3[1] = 0;
    
    FRESULT rc_rd = FR_DISK_ERR;
    FIL file_object_rd;
    rc_rd = f_open(&file_object_rd, FILE_NAME, (unsigned char) FA_READ);
    
    v_noSound();
    
    if (rc_rd != FR_OK)
    {
      printf("loaderMain.c: loadAndStart(): Could not open file %s (%i) \r\n", FILE_NAME, rc_rd);
    }
    else
    {
      /*			
	FIL* fp, 	/* Pointer to the file object 
	void* buff,	/* Pointer to data buffer 
	UINT btr,	/* Number of unsigned chars to read 
	UINT* br	/* Pointer to number of unsigned chars read 
      */
      printf("Loading: %s \r\n", FILE_NAME);
      unsigned int fsize = MAX_LOAD;

      rc_rd = f_read(&file_object_rd, progSpace, fsize, &fsize);
      if ( rc_rd!= FR_OK)
      {
        printf("loaderMain.c: loadAndStart(): File not loaded (%s) (size got = %i)\r\n", FILE_NAME, rc_rd);
        f_close(&file_object_rd);
      }
      else
      {
          f_close(&file_object_rd);
          int c=0;
          if ((item->param1 != 0) || (item->id == 9999))
          {
            char *parameter = item->param1;
            if (item->id == 9999)
            {
              dirBuffer[0] = 0;
              strcat(dirBuffer,subdirectoryPath);
              strcat(dirBuffer,"/");
              strcat(dirBuffer,filesInDir[currentSelectedItem]);
              parameter = dirBuffer;
            }
            while (*parameter != (char) 0)
            {
              loaderSettings.parameter1[c++] =  (unsigned char) *parameter;
              parameter++;
	      
              if (c==126) break;
            } 
          }
          loaderSettings.parameter1[c]= (unsigned char) 0;

          c=0;
          if (item->param2 != 0)
          {
            char *parameter = item->param2;
            while (*parameter != (char) 0)
            {
              loaderSettings.parameter2[c++] =  (unsigned char) *parameter;
              parameter++;
              if (c==126) break;
            } 
          }

          if (item->id == 6000)
          {
              loaderSettings.parameter1[0] = currentSelectedItem2;
          }
          loaderSettings.parameter2[c]= (unsigned char) 0;

          printf("Starting loaded file... \r\n");
          printf("Debug Info: Core 0 state: %i\n", *((unsigned int *)CORE0_STATE));
          printf("Debug Info: Core 1 state: %i\n", *((unsigned int *)CORE1_STATE));
          printf("Debug Info: Core 2 state: %i\n", *((unsigned int *)CORE2_STATE));
          printf("Debug Info: Core 3 state: %i\n", *((unsigned int *)CORE3_STATE));
          
          loaderSettings.flags |= FLAG_SELECTION_MADE;
          
          startWithCurrentSettings();
      }
    }
}


#include "ymStuff.i"


char sampleBuffer[200*1000]; // max 200k ym buffer
unsigned int sampleSize = 0;
int loadRAW(TCHAR *FILE_NAME)
{
    FRESULT rc_rd = FR_DISK_ERR;
    FIL file_object_rd;
    rc_rd = f_open(&file_object_rd, FILE_NAME, (unsigned char) FA_READ);

    if (rc_rd != FR_OK)
    {
      printf("Could not open file %s (%i) \r\n", FILE_NAME, rc_rd);
    }
    else
    {
    /*          
    FIL* fp,    /* Pointer to the file object 
    void* buff, /* Pointer to data buffer 
    UINT btr,   /* Number of unsigned chars to read 
    UINT* br    /* Pointer to number of unsigned chars read 
    */
      printf("Loading: %s \r\n", FILE_NAME);
      unsigned int fsize = 200*1000-1;
      rc_rd = f_read(&file_object_rd, sampleBuffer, fsize, &fsize);
      if ( rc_rd!= FR_OK)
      {
          printf("File not loaded (size got = %i)\r\n", fsize);
          f_close(&file_object_rd);
          return 0;
      }
      else
      {
      f_close(&file_object_rd);
      // file is loaded
      printf("File loaded successfully (%i)!\r\n",fsize);
      sampleSize = fsize;
      }
    }
    return 1;
}

int loadAndPlayRAW()
{
  loadRAW("pitrexSample.raw");
  v_playDirectSampleAll(sampleBuffer, sampleSize, 15000);     
  return 1;
}

extern int __bss_start__;
extern int __bss_end__;
double pitrexSize=0;
int selectionMade = 0;
void initMenu();
void displayMenu();
void drawPitrex();
void initTestRoms();

int skipWave = 0;
#if RASPPI != 1 
int useSMP = 0; // use piZero SMP if available (loader)
#endif
// for now INI setting just stupidly overwrite other saved settings!
static int loaderIniHandler(void* user, const char* section, const char* name, const char* value)
{
  #define MATCH_NAME(n) strcmp(name, n) == 0
  if (MATCH_NAME("SKIP_WAV")) 
  {
    skipWave = atoi(value);
  }
#if RASPPI != 1 
  if (MATCH_NAME("USE_SMP")) 
  {
    useSMP = atoi(value);
  }
#endif
  return 1;
}


extern int clipActive;
extern int clipminX;
extern int clipminY;
extern int clipmaxX;
extern int clipmaxY;
extern int clipMode;

int fall = 0;
int blow = 0;


void c_irq_handler (void) {
}

static void KeyPressedHandler (const char *pString)
{
  printf("<%s>", pString);
}
void doBitmap();


int _clipminX=-5000;
int _clipmaxX= 5000;
int _clipminY= -5000;
int _clipmaxY= 5000;

#if RASPPI != 1 
void __attribute__ ((noreturn)) __attribute__ ((naked)) parkCore1() ;
void startCore1(unsigned int coreStart);
extern volatile unsigned int isSMPMode;
extern volatile unsigned int pendingDisableMultiCore;
void v_removeMultiCore();
void handleVectrexOutputSMP();
void goHavoc();    
#endif

/** Main function - we'll never return from here */
int main(int argc, char *argv[])
{
#if RASPPI != 1
    printf("core0entered: %X (%p), State: %i\r\n", *((unsigned int *)CORE0_ENTERED), CORE0_ENTERED, *((unsigned int *)CORE0_STATE));
    printf("core1entered: %X (%p), State: %i\r\n", *((unsigned int *)CORE1_ENTERED), CORE1_ENTERED, *((unsigned int *)CORE1_STATE));
    printf("core2entered: %X (%p), State: %i\r\n", *((unsigned int *)CORE2_ENTERED), CORE2_ENTERED, *((unsigned int *)CORE2_STATE));
    printf("core3entered: %X (%p), State: %i\r\n", *((unsigned int *)CORE3_ENTERED), CORE3_ENTERED, *((unsigned int *)CORE3_STATE));
#endif     
  *loaderAvailable=1;
  resetImportantStuff();
  int rebootPending = 0;
  
  if (loaderSettings.resetType == LOADER_STARTING)
  {
      subdirectoryPath[0] = 0;
      loaderSettings.flags = 0;
  }
  else
  {
    printf("Restarting loader...\n");
    if (loaderSettings.resetType == RESET_TYPE_CONTINUE_LOADER)
    {
      // just continue
    }
    else if (loaderSettings.resetType == RESET_TYPE_RESTART_SELECTION)
    {
      if (loaderSettings.flags &= FLAG_SELECTION_MADE)
	startWithCurrentSettings();
    }
    else if (loaderSettings.resetType == RESET_TYPE_RESTART_BOOT_NAKED)
    {
      rebootPending = 1;
    }
   
  }
  loaderSettings.flags |= FLAG_LOADER_RUNNING;
  
  {
    settingsPointer = (GlobalMemSettings **)SETTING_STORE;
    *settingsPointer = &loaderSettings;

    printf("Loader starting...\r\n");
    printf("BSS start: %X, end: %X\r\n", &__bss_start__, &__bss_end__);
    printf("SettingPointer: %08x, settings: %08x\r\n", settingsPointer, &loaderSettings);

    unsigned int arm_ram = lib_bcm2835_vc_get_memory(BCM2835_VC_TAG_GET_ARM_MEMORY) / 1024 / 1024;	///< MB
    unsigned int gpu_ram = lib_bcm2835_vc_get_memory(BCM2835_VC_TAG_GET_VC_MEMORY) / 1024 / 1024;	///< MB
    printf("Pi ARM memory: %iMB\n", arm_ram);
    printf("Pi GPU memory: %iMB\n", gpu_ram);
    
    tweakVectors();

    printf("Start mounting fs...\r\n");
    FRESULT result = f_mount(&fat_fs, (const TCHAR *) "0:", (unsigned char) 1);
    if (result != FR_OK) 
    {
      vectrexinit(1); // pitrex
      v_init(); // vectrex interface
      printf("loaderMain.c: loaderMain(): NO filesystem...!\r\n");
      printf("loaderMain.c: loaderMain(): f_mount failed! %d\r\n", (int) result);
      v_error("MOUNT FAILED");
    }
    else
    { 
	printf("FAT filesystem found!\r\n");
    }

    vectrexinit(1); // pitrex
    printf("gpio init done!\r\n");
    v_init(); // vectrex interface
    printf("Loader: init done!\r\n");
  }

  
  int ymloaded;
  int s = 0;
  int bDir = 1;
  int b = 30;
  char *ss[] = {"PITREX"};
  
  
  printf("\r\nSettingPointer: %08x, settings: %08x\r\n", loaderSettings.loader, (void (*)(void)) main);

  if (loaderSettings.flags & FLAG_IN_MENU)
  {
    printf("Loader is reinitializing...\r\n");
    initMenu();
    currentMenuItem = (MenuItemNew *)loaderSettings.lastSelection;
    ymloaded = loadYM();
  }
  else
  {
    printf("Loader is initializing...\r\n");
    ini_parse("loader.ini", loaderIniHandler, 0);

    loaderSettings.loader = (void (*)(void)) main;
    loaderSettings.lastSelection = 0;
    
    if ((!skipWave) && (rebootPending==0))
    {
      loadAndPlayRAW();
      v_init(); // vectrex interface
    }
//goHavoc();    
    usePipeline = 1; 
//    v_initKeyboard(); // can lead to "waits" after return to the loader, this must be investigated in USPI lib and the timer reset before init again
//    printf("Keyboard available=%i\n",usbKeyboardAvailable);


// doBitmap();
    
    v_setupIRQHandling();  
    v_enableSoundOut(1);
    v_enableButtons(1);
    v_enableJoystickDigital(0,0,0,0);
    v_enableJoystickAnalog(0,0,0,0);

    v_setHardClipping(1, 1, 1, _clipminX,_clipminY, _clipmaxX,_clipmaxY);
    
    fall = 0;
    blow=100;
    
    while(1)
    {
      
      fall-=4;
      
      _clipminY += fall;
      _clipmaxY += fall;
      _clipminX += blow;
      _clipmaxX += blow;

      if ((_clipmaxX >= 18000) && (blow>0)) blow =-blow;
      if ((_clipminX <= -18000) && (blow<0)) blow =-blow;
      if ((_clipminY <= -20000) && (fall<0)) {fall =-350;_clipminY = -20000; _clipmaxY = _clipminY+10000;fall =-fall;}
      
      v_setHardClipping(1, 1, 1, _clipminX,_clipminY, _clipmaxX,_clipmaxY);
      v_WaitRecal();
      if (pitrexSize<1) pitrexSize+=0.01;else pitrexSize=1; 

      //if ((currentButtonState&0x0f) != (0x01)) // exactly button 1

      drawPitrex();

      v_doSound(); // not needed with IRQ Mode
      v_readButtons(); // not needed with IRQ Mode
      if ((currentButtonState&0x0f) == (0x08)) // exactly button 4
        break;
//      if ((currentButtonState&0x0f) == (0x01)) // exactly button 1
//      {
//	 startCore1((unsigned int) (&core1Wait));
//      }
      
//      if (usbKeyboardAvailable)
//      {
//	  printf(".");
//	char lk = USPiKeyboardLastKeyDown();
//	if (lk)
//	{
//	  printf("%c", lk);
//	}
//      }
//      printf("core1entered: %X (%p), State: %i\r\n", *((unsigned int *)CORE1_ENTERED), CORE1_ENTERED, *((unsigned int *)CORE1_STATE));
//      asm volatile ("sev");
     
      
      if (rebootPending) break;
    }
    v_removeIRQHandling();  
    v_setCustomClipping(0, -14000, -14000, 14000, 14000);
    clipActive = 0;
    v_setHardClipping(0, 1, 1, _clipminX,_clipminY, _clipmaxX,_clipmaxY);
    initMenu();
    ymloaded = loadYM();
    if (ymloaded) v_playYM();      
  }

  if (rebootPending)
    reloadLoader(); 
  
  v_enableJoystickAnalog(1,1,0,0);
  v_enableJoystickDigital(0,0,0,0);
  
//  v_setupIRQHandling();  
  v_enableSoundOut(1);
  v_enableButtons(1);
  v_setRefresh(50);
  v_disableReturnToLoader();
//  loadRAW("notReverse.raw");
//  v_playIRQSample(sampleBuffer, sampleSize, 10000, PLAY_LOOP);     


#if RASPPI != 1 
  if ((isSMPMode == 1) && (useSMP==0))
  {
    pendingDisableMultiCore = 1;
  }
  else
  if ((isSMPMode == 0) && (useSMP==1))
  {
    v_setupSMPHandling();
  }
#endif

  
  loaderSettings.flags |= FLAG_IN_MENU;
  while(1)
  {
    v_WaitRecal();
    v_doSound(); // not needed with IRQ Mode
    v_readButtons(); // not needed with IRQ Mode
    v_readJoystick1Analog();// not needed with IRQ Mode
//    v_readJoystick1Digital();// not needed with IRQ Mode
    
    b=b+bDir;
    if (b==70) bDir = -1;
    if (b==20) bDir = 1;
    v_printString(-100, 100, ss[0], 5, b);
    displayMenu();
    if (ymloaded) v_playYM();      
  }
}

#include "icons.i"


int angleX = 0;
int angleY = 0;
int angleZ = 0;
void draw3dList(Vector3d l[], float mul, int angleOffset, int xOffset, int yOffset)
{
  int count = 0;
  angleX = (angleX + 1)%360;
  angleY = (angleY + 1)%360;
  angleZ = (angleZ + 1)%360;
  
  
  buildRotationX((angleX+angleOffset)%360);
  buildRotationY((angleY+angleOffset)%360);
  buildRotationZ((angleZ+angleOffset)%360);
  
  
  while ((l[count].y0!=-1) && (l[count].x0!=-1))
  {
    Vertex3d original;
    Vertex3d rotatedStart;
    original.x = l[count].x0;
    original.y = l[count].y0;
    original.z = l[count].z0;
    
    multiplyMatrix(rotX, original, &rotatedStart);
    copyVertex3d(rotatedStart, &original);
    multiplyMatrix(rotY, original, &rotatedStart);
    copyVertex3d(rotatedStart, &original);
    multiplyMatrix(rotZ, original, &rotatedStart);
 
    Vertex3d rotatedEnd;
    original.x = l[count].x1;
    original.y = l[count].y1;
    original.z = l[count].z1;
    if (((angleX+angleOffset)%360) != 0)
    {
      multiplyMatrix(rotX, original, &rotatedEnd);
      copyVertex3d(rotatedEnd, &original);
      multiplyMatrix(rotY, original, &rotatedEnd);
      copyVertex3d(rotatedEnd, &original);
      multiplyMatrix(rotZ, original, &rotatedEnd);
    }
    else
    {
      copyVertex3d(original, &rotatedEnd);
    }
    
    int x0 = (int)(((rotatedStart.x)<<7)*pitrexSize);
    int y0 = (int)(((rotatedStart.y)<<7)*pitrexSize);
    int x1 = (int)(((rotatedEnd.x)<<7)*pitrexSize);
    int y1 = (int)(((rotatedEnd.y)<<7)*pitrexSize);
    
    v_directDraw32(x0*mul+xOffset, y0*mul+yOffset,x1*mul+xOffset,y1*mul+yOffset, 0x5f);
    count++;
  }
}

void drawPitrex()
{
  v_setCustomClipping(1, -14000, -14000, 14000, 14000);
  draw3dList(PiTrexHershey3d,1.5, 0,0,0);
  commonHints = PL_BASE_FORCE_NOT_CLIPPED | PL_BASE_FORCE_STABLE;
  v_setCustomClipping(1, _clipminX, _clipminY, _clipmaxX, _clipmaxY);
  draw3dList(PiTrexHershey3d, 0.6, 60,5000+_clipminX,5000+_clipminY);
  commonHints = 0;
}
 
 
void displayLargeList(const signed char list[])
{
  int count =(unsigned char) (*list++);

  while (count >0)  
  {
    int y0 = *list++;
    int x0 = *list++;
    int y1 = *list++;
    int x1 = *list++;
    v_directDraw32(x0<<7, y0<<7,x1<<7,y1<<7, 0x5f);
    count --;
  }
}
void displayLargeListUp(const signed char list[])
{
  int count = *list++;

  while (count >0)  
  {
    int y0 = *list++;
    int x0 = *list++;
    int y1 = *list++;
    int x1 = *list++;
    v_directDraw32(x0<<6, (y0<<6)+15000,x1<<6,(y1<<6)+15000, 0x3f);
    count --;
  }
}

MenuItemNew vectrexMenu;
int scrollReset = 1;
int loadFileNames();

void initMenu()
{
  selectionMade = 0;
  currentMenuItem = &vectrexMenu;
  initTestRoms();

  if (subdirectoryPath[0]!=0)
  {
    char dirBuffer[MAX_VECTREX_FILENAME_LENGTH*2];
    dirBuffer[0] = 0;
    strcat(dirBuffer, "vectrex"); // starting path
    strcat(dirBuffer,"/");
    strcat(dirBuffer,subdirectoryPath);
    loadFileNames(dirBuffer);
  }
  else
  {
    loadFileNames("vectrex");
  }
  scrollReset = 1;
}
//////////////////////////////////////////
// Uppest "Menu"
//////////////////////////////////////////

MenuItemNew vectrexMenu;
MenuItemNew settingsMenu =
{
  0,    // ID
  "PLEASE ENTER SETTINGS",    // no name
  0,    // no start directory
  0,    // no start image
  0,    // no parameter
  0,    // no parameter
  0, // icon
  0, // smallicon
  0,    // has no parent
  0,    // ! firstChild must be set
  0,    // has no left
  &vectrexMenu,    // ! right must be set
  0,    // scrolltext
  0,    // no text
};
MenuItemNew vectorbladeMenu;
MenuItemNew arcadeMenu;
MenuItemNew vectrexMenu =
{
  1,    // ID
  0,    // no name
  0,    // no start directory
  0,    // no start image
  0,    // no parameter
  0,    // no parameter
  vectrexIcon, // icon
  vectrexSmallIcon, // smallicon
  0,    // has no parent
  &vectorbladeMenu,    // ! firstChild must be set
  &settingsMenu,    // has no left
  &arcadeMenu,    // ! right must be set
  0,    // scrolltext
  0,    // no text
};

MenuItemNew computerMenu;
MenuItemNew atariMenu;
MenuItemNew arcadeMenu =
{
  2,    // ID
  0,    // no name
  0,    // no start directory
  0,    // no start image
  0,    // no parameter
  0,    // no parameter
  arcadeIcon, // icon
  arcadeSmallIcon, // smallicon
  0,    // has no parent
  &atariMenu,    // ! firstChild must be set
  &vectrexMenu,    // ! left must be set
  &computerMenu,    // ! right must be set
  0,    // scrolltext
  0,    // no text
};
MenuItemNew zBlastMenu;

MenuItemNew videoMenu;
MenuItemNew computerMenu =
{
  3,    // ID
  0,    // no name
  0,    // no start directory
  0,    // no start image
  0,    // no parameter
  0,    // no parameter
  computerIcon, // icon
  computerSmallIcon, // small icon
  0,    // has no parent
  &zBlastMenu,    // ! firstChild must be set
  &arcadeMenu,    // ! left must be set
  &videoMenu,    // ! right must be set
  0,    // scrolltext
  0,    // no text
};
 
MenuItemNew badAppleMenu;
MenuItemNew audioMenu;
MenuItemNew videoMenu =
{
  4,    // ID
  0,    // no name
  0,    // no start directory
  0,    // no start image
  0,    // no parameter
  0,    // no parameter
  videoIcon, // icon
  videoSmallIcon, // small icon
  0,    // has no parent
  &badAppleMenu,    // ! firstChild must be set
  &computerMenu,    // ! left must be set
  &audioMenu,    // ! right must be set
  0,    // scrolltext
  0,    // no text
};
MenuItemNew bohemianMenu;
MenuItemNew audioMenu =
{
  5,    // ID
  0,    // no name
  0,    // no start directory
  0,    // no start image
  0,    // no parameter
  0,    // no parameter
  audioIcon, // icon
  audioSmallIcon, // smallicon
  0,    // has no parent
  &bohemianMenu,    // ! firstChild must be set
  &videoMenu,    // ! left must be set
  0,    // ! right must be set
  0,    // scrolltext
  0,    // no text
};
MenuItemNew hyperoidsMenu;
MenuItemNew zBlastMenu =
{
  300,    // ID
  "ZBLAST",    // no name
  0,    // no start directory
  IMG_FILE_PREFIX "zblast.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  zblastList, // icon
  0, // smallicon
  &computerMenu,    // has no parent
  0,    // ! firstChild must be set
  0,    // ! left must be set
  &hyperoidsMenu,    // ! right must be set
  0,    // scrolltext
  "","","","","A GAME BY MARK RUSSELS", "PITREX PORT KEVIN KOSTER","MUSIC BY ROALD STRAUSS"  , 0  // no text
};
MenuItemNew basicMenu;
MenuItemNew hyperoidsMenu =
{
  301,    // ID
  "HYPEROIDS",    // no name
  0,    // no start directory
  IMG_FILE_PREFIX "vhyperoid.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  hyperList, // icon
  0, // smallicon
  &computerMenu,    // has no parent
  0,    // ! firstChild must be set
  &zBlastMenu,    // ! left must be set
  &basicMenu,    // ! right must be set
  0,    // scrolltext
  "","","","","A GAME BY EDWARD HUTCHINS","LINUX PORT MARK RUSSELS", "PITREX PORT KEVIN KOSTER", 0   // no text
};

MenuItemNew newKindMenu;
MenuItemNew basicMenu =
{
  302,    // ID
  "GS BASIC",    // no name
  0,    // no start directory
  IMG_FILE_PREFIX "gsbasic.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  0, // icon
  0, // smallicon
  &computerMenu,    // has no parent
  0,    // ! firstChild must be set
  &hyperoidsMenu,    // ! left must be set
  &newKindMenu,    // ! right must be set
  0,    // scrolltext
  "","","","","GS BASIC", "BY BOB ALEXANDER",""  , 0  // no text
};

MenuItemNew wwviMenu;
MenuItemNew newKindMenu =
{
  303,    // ID
  "ELITE (BETA)",    // no name
  0,    // no start directory
  IMG_FILE_PREFIX "newkind.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  eliteList, // icon
  0, // smallicon
  &computerMenu,    // has no parent
  0,    // ! firstChild must be set
  &basicMenu,    // ! left must be set
  &wwviMenu,    // ! right must be set
  0,    // scrolltext
  "","","","","A GAME BY CHRISTIAN PINDER","OIGINAL BY", "DAVID BRABEN", "IAN BELL", 0   // no text
};

MenuItemNew bz2Menu;
MenuItemNew wwviMenu =
{
  304,    // ID
  "WW VI (BETA)",    // no name
  0,    // no start directory
  IMG_FILE_PREFIX "ww_vi.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  WWVIIcon, // icon
  0, // smallicon
  &computerMenu,    // has no parent
  0,    // ! firstChild must be set
  &newKindMenu,    // ! left must be set
  &bz2Menu,    // ! right must be set
  0,    // scrolltext
  "","","","","A GAME BY STEPHEN M. CAMERON", 0   // no text
};
MenuItemNew voomMenu;
MenuItemNew bz2Menu =
{
  305,    // ID
  "BZ2 (BETA)",    // no name
  0,    // no start directory
  IMG_FILE_PREFIX "bz2.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  Bz2Icon, // icon
  0, // smallicon
  &computerMenu,    // has no parent
  0,    // ! firstChild must be set
  &wwviMenu,    // ! left must be set
  &voomMenu,    // ! right must be set
  0,    // scrolltext
  "","","","","A GAME BY PETER HIRSCHBERG","","(NO SOUND)", 0   // no text
};
MenuItemNew testMenu;
MenuItemNew rebelMenu;
MenuItemNew voomMenu =
{
  306,    // ID
  "VOOM (DEMO)",    // no name
  0,    // no start directory
  IMG_FILE_PREFIX "voom.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  DoomIcon, // icon
  0, // smallicon
  &computerMenu,    // has no parent
  0,    // ! firstChild must be set
  &bz2Menu,    // ! left must be set
  &rebelMenu,    // ! right must be set
  0,    // scrolltext
  "","","","","SPRITESMOD!","","", 0   // no text
};

MenuItemNew rebelMenu =
{
  308,    // ID
  "REBEL DEFENDER",    // no name
  0,    // no start directory
  IMG_FILE_PREFIX "rebel.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  RebelIcon, // icon
  0, // smallicon
  &computerMenu,    // has no parent
  0,    // ! firstChild must be set
  &voomMenu,    // ! left must be set
  &testMenu,    // ! right must be set
  0,    // scrolltext
  "","","","","VINCENT PORTET 2015","(NO SOUND)","", 0   // no text
};
MenuItemNew testMenu =
{
  309,    // ID
  "VTERM",    // no name
  0,    // no start directory
  IMG_FILE_PREFIX "vterm.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  0, // icon
  0, // smallicon
  &computerMenu,    // has no parent
  0,    // ! firstChild must be set
  &rebelMenu,    // ! left must be set
  0,    // ! right must be set
  0,    // scrolltext
  "","","","","TEST!","","", 0   // no text
};


//////////////////////////////////////////
// Vectrex "Menu"
//////////////////////////////////////////

MenuItemNew karlQuappeeMenu;
MenuItemNew speedMenu;
MenuItemNew vectorbladeMenu =
{
  100,    // ID
  "VECTORBLADE",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectorblade.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  vb2List, // icon
  0, // smallicon
  &vectrexMenu,    // has parent
  0,    // ! firstChild must be set
  0,    // ! left must be set
  &speedMenu,    // ! right must be set
#define SCROLL_CHAR_START "                          "

  SCROLL_CHAR_START "VECTORBLADE IS MY NEWEST GAME FOR THE VECTREX. VECTORBLADE IS A CLASSIC BOTTOM UP SHOOTER HEAVILY INSPIRED BY WARBLADE FROM EDGAR M. VIGDAL. WARBLADE OR ITS PREDECESSOR DELUXE GALAGA WAS ORIGINALLY DEVELOPED FOR THE AMIGA COMPUTER, BUT WAS LATER ADAPATED TO WINDOWS, OS X AND EVEN IOS.",
  {"","","","","","","A GAME BY MALBAN", 0},    // no text
};
MenuItemNew webWarsMenu;
MenuItemNew exactMenu;

MenuItemNew webWarsSpeedMenu;
MenuItemNew speedMenu =
{
  102,    // ID
  "",
  0,    // directory 
//  "",    // no start image
  IMG_FILE_PREFIX "vectrexspeedy.img",    // no start image
  "",    // no parameter
  0,    // no parameter
  vecxSpeedList, // icon
  vecxSpeedList, // smallicon
  &vectrexMenu,    // has parent
  &webWarsSpeedMenu,    // ! firstChild must be set
  &vectorbladeMenu,    // ! left must be set
  &exactMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","  SPEEDY VECTREX", 0},    // no text
};
//////////////////////////////////////////
// Vectrex Speed "Menu"
//////////////////////////////////////////

MenuItemNew berzerkSpeedMenu;
MenuItemNew webWarsSpeedMenu =
{
  1020,    // ID
  " WEB WARS",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexspeedy.img",    // no start image
  "original/WEBWARS.BIN",    // no parameter
  0,    // no parameter
  webWarsList, // icon
  0, // smallicon
  &speedMenu,    // has parent
  0,    // ! firstChild must be set
  0,    // ! left must be set
  &berzerkSpeedMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","GCE WILLIAM HAWKINS", "DUNCAN MUIRHEAD", 0},    // no text
};
//MenuItemNew speedMenu;
MenuItemNew mine2SpeedMenu;
MenuItemNew darkTowerSpeedMenu;
MenuItemNew starCastleSpeedMenu;
MenuItemNew narzodSpeedMenu;
MenuItemNew scrambleSpeedMenu;
MenuItemNew solarSpeedMenu;

MenuItemNew berzerkSpeedMenu =
{
  1021,    // ID
  " BERZERK",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexspeedy.img",    // no start image
  "original/BERZERK.BIN",    // no parameter
  0,    // no parameter
  berzerkList, // icon
  0, // smallicon
  &speedMenu,    // has parent
  0,    // ! firstChild must be set
  &webWarsSpeedMenu,    // ! left must be set
  &mine2SpeedMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","GCE CHRIS KING", 0},    // no text
};
MenuItemNew mine2SpeedMenu =
{
  1022,    // ID
  "MINESTORM 2",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexspeedy.img",    // no start image
  "original/MSTORM2.BIN",    // no parameter
  0,    // no parameter
  mineStormList, // icon
  0, // smallicon
  &speedMenu,    // has parent
  0,    // ! firstChild must be set
  &berzerkSpeedMenu,    // ! left must be set
  &darkTowerSpeedMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","GCE JOHN HALL", 0},    // no text
};
MenuItemNew darkTowerSpeedMenu =
{
  1023,    // ID
  "DARK TOWER",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexspeedy.img",    // no start image
  "original/DKTOWER.BIN",    // no parameter
  0,    // no parameter
  darkTowerList, // icon
  0, // smallicon
  &speedMenu,    // has parent
  0,    // ! firstChild must be set
  &mine2SpeedMenu,    // ! left must be set
  &starCastleSpeedMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","GCE JOHN HALL", 0},    // no text
};
MenuItemNew starCastleSpeedMenu =
{
  1024,    // ID
  "STAR CASTLE",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexspeedy.img",    // no start image
  "original/CASTLE.BIN",    // no parameter
  0,    // no parameter
  starCastleList, // icon
  0, // smallicon
  &speedMenu,    // has parent
  0,    // ! firstChild must be set
  &darkTowerSpeedMenu,    // ! left must be set
  &narzodSpeedMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","GCE WILLIAM HAWKINS", 0},    // no text
};
MenuItemNew narzodSpeedMenu =
{
  1025,    // ID
  "FORTRESS OF NARZOD",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexspeedy.img",    // no start image
  "original/NARZOD.BIN",    // no parameter
  0,    // no parameter
  narzodList, // icon
  0, // smallicon
  &speedMenu,    // has parent
  0,    // ! firstChild must be set
  &starCastleSpeedMenu,    // ! left must be set
  &scrambleSpeedMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","GCE JOHN HALL", 0},    // no text
};
MenuItemNew scrambleSpeedMenu =
{
  1026,    // ID
  "SCRAMBLE",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexspeedy.img",    // no start image
  "original/SCRAMBLE.BIN",    // no parameter
  0,    // no parameter
  scrambleList, // icon
  0, // smallicon
  &speedMenu,    // has parent
  0,    // ! firstChild must be set
  &narzodSpeedMenu,    // ! left must be set
  &solarSpeedMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","GCE PAUL ALLEN NEWELL", 0},    // no text
};
MenuItemNew solarSpeedMenu =
{
  1027,    // ID
  "SOLAR QUEST",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexspeedy.img",    // no start image
  "original/SOLAR.BIN",    // no parameter
  0,    // no parameter
  solarList, // icon
  0, // smallicon
  &speedMenu,    // has parent
  0,    // ! firstChild must be set
  &scrambleSpeedMenu,    // ! left must be set
  0,    // ! right must be set
  0,    // scrolltext
  {"","","","","GCE SCOTT BODEN", 0},    // no text
};

//////////////////////////////////////////
// Vectrex Exact "Menu"
//////////////////////////////////////////
MenuItemNew exactMenuRelease;
MenuItemNew exactkarlQuappeMenu;
MenuItemNew demoMenu;
MenuItemNew exactMenu =
{
  107,    // ID
  "",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexexact.img",    // no start image
  "",    // no parameter
  0,    // no parameter
  vecxExactList, // icon
  vecxExactList, // smallicon
  &vectrexMenu,    // has parent
  &exactkarlQuappeMenu,    // ! firstChild must be set
  &speedMenu,    // ! left must be set
  &demoMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","  EXACT VECTREX", 0},    // no text
};

MenuItemNew exactkarlQuappeMenu =
{
  1070,    // ID
  "KARL QUAPPE",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexexact.img",    // no start image
  "KARL_QUAPPE.BIN",    // no parameter
  0,    // no parameter
  froggerList, // icon
  0, // smallicon
  &exactMenu,    // has parent
  0,    // ! firstChild must be set
  0,    // ! left must be set
  &exactMenuRelease,    // ! right must be set
  0,    // scrolltext
  {"","","","","A GAME BY MALBAN","NO PERSISTENCY YET !", 0},    // no text
};
MenuItemNew exactmrBoston;
MenuItemNew exactMenuRelease =
{
  1071,    // ID
  "RELEASE",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexexact.img",    // no start image
  "RELEASE.BIN",    // no parameter
  0,    // no parameter
  releaseList, // icon
  0, // smallicon
  &exactMenu,    // has parent
  0,    // ! firstChild must be set
  &exactkarlQuappeMenu,    // ! left must be set
  &exactmrBoston,    // ! right must be set
//  0,    // scrolltext
  SCROLL_CHAR_START "RELEASE ! THE GAME IS A PORT OF A GAME ORIGINALLY DEVELOPED FOR IOS AND ANDROID BY GIMOGAMES. THE PORT TO VECTREX WAS DONE BY PERMISSION OF THE ORIGINAL AUTHORS.  THANK YOU FOR ALLOWING ME TO PORT THE GAME!",   

  {"","","","","GAME BY MALBAN!","ORIGINAL BY GIMOGAMES", 0},    // no text
};
MenuItemNew exactPickOne;
MenuItemNew exactmrBoston =
{
  1072,    // ID
  "MR BOSTON",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexexact.img",    // no start image
  "original/MR_BOSTON.BIN",    // no parameter
  0,    // no parameter
  bostenList, // icon
  0, // smallicon
  &exactMenu,    // has parent
  0,    // ! firstChild must be set
  &exactMenuRelease,    // ! left must be set
  &exactPickOne,    // ! right must be set
//  0,    // scrolltext
 "",
  
  {"","","","","GCE RICHARD MOSZKOWSKI", 0},    // no text
};
MenuItemNew exactPickOne =
{
  9999,    // ID
  "PICK A GAME",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexexact.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  0, // icon
  0, // smallicon
  0,    // has parent
  0,    // ! firstChild must be set
  &exactmrBoston,    // ! left must be set
  0,    // ! right must be set
//  0,    // scrolltext
  "",
  
  {"", 0},    // no text
};

//////////////////////////////////////////
// Vectrex Demo "Menu"
//////////////////////////////////////////
MenuItemNew demo1Menu;
MenuItemNew demo2Menu;
MenuItemNew demo3Menu;
MenuItemNew demo4Menu;
MenuItemNew demoMenu =
{
  108,    // ID
  "  DEMOS",    // no name
  0,    // directory
  "",    // no start image
  "",    // no parameter
  0,    // no parameter
  demosList, // icon
  demosList, // smallicon
  &vectrexMenu,    // has parent
  &demo1Menu,    // ! firstChild must be set
  &exactMenu,    // ! left must be set
  0,    // ! right must be set
  0,    // scrolltext
  {"","","","","VECTREX SCENE DEMOS!", 0},    // no text
};
MenuItemNew demo1Menu =
{
  1080,    // ID
  "RAIDING PARTY",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexexact.img",    // no start image
  "demos/RaidingParty.bin",    // no parameter
  0,    // no parameter
  0, // icon
  0, // smallicon
  &demoMenu,    // has parent
  0,    // ! firstChild must be set
  0,    // ! left must be set
  &demo2Menu,    // ! right must be set
  0,    // scrolltext
  {"BY FELL AND BEX", 0},    // no text
};
MenuItemNew demo2Menu =
{
  1081,    // ID
  "BRESENHAM",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexexact.img",    // no start image
  "demos/bresenham.bin",    // no parameter
  0,    // no parameter
  0, // icon
  0, // smallicon
  &demoMenu,    // has parent
  0,    // ! firstChild must be set
  &demo1Menu,    // ! left must be set
  &demo3Menu,    // ! right must be set
  0,    // scrolltext
  {"NUANCE AND METALVOTZE", 0},    // no text
};
MenuItemNew demo3Menu =
{
  1082,    // ID
  "EPIC REALTIME",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexexact.img",    // no start image
  "demos/epic.bin",    // no parameter
  0,    // no parameter
  0, // icon
  0, // smallicon
  &demoMenu,    // has parent
  0,    // ! firstChild must be set
  &demo2Menu,    // ! left must be set
  &demo4Menu,    // ! right must be set
  0,    // scrolltext
  {"NUANCE AND METALVOTZE", 0},    // no text
};
MenuItemNew demo5Menu;
MenuItemNew demo4Menu =
{
  1083,    // ID
  "WHATPG",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexexact.img",    // no start image
  "demos/whatpg.bin",    // no parameter
  0,    // no parameter
  0, // icon
  0, // smallicon
  &demoMenu,    // has parent
  0,    // ! firstChild must be set
  &demo3Menu,    // ! left must be set
  &demo5Menu,    // ! right must be set
  0,    // scrolltext
  {"WHERE HAVE ALL", "THE PIXELS GONE", "CMCC", 0},    // no text
};
MenuItemNew demo5Menu =
{
  1084,    // ID
  "LINEART",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexlineart.img",    // no start image
  "demos/lineart8banks.bin",    // no parameter
  0,    // no parameter
  linArtIcon, // icon
  0, // smallicon
  &demoMenu,    // has parent
  0,    // ! firstChild must be set
  &demo3Menu,    // ! left must be set
  0,    // ! right must be set
  0,    // scrolltext
  {"","","","LINEART VOZ 2011", "VECFLASH 4 BANK DEMO", 0},    // no text
};


//////////////////////////////////////////
// Vectrex Arcade "Menu"
//////////////////////////////////////////
MenuItemNew swsbtMenu;
MenuItemNew allCinematronicsMenu;

MenuItemNew atariMenu =
{
  200,    // ID
  "ATARI",    // no name
  0,    // directory
  0,    // no start image
  0,    // no parameter
  0,    // no parameter
  AtarilIcon, // icon
  AtariSmallIcon, // icon
  &arcadeMenu,    // has no parent
  &swsbtMenu,    // ! firstChild must be set
  0,    // ! left must be set
  &allCinematronicsMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","", 0},    // no text
};


MenuItemNew segaMenu;
MenuItemNew tailgunnerMenu;
MenuItemNew allCinematronicsMenu =
{
  201,    // ID
  "CINEMATRONICS",    // no name
  0,    // directory
  0,    // no start image
  0,    // no parameter
  0,    // no parameter
  CinematronicslIcon, // icon
  CinematronicsSmallIcon, // icon
  &arcadeMenu,    // has no parent
  &tailgunnerMenu,    // ! firstChild must be set
  &atariMenu,    // ! left must be set
  &segaMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","GRAHAMS PASSION!", 0},    // no text
};
MenuItemNew zektorMenu;
MenuItemNew otherArcadeMenu;
MenuItemNew segaMenu =
{
  202,    // ID
  "SEGA",    // no name
  0,    // directory
  0,    // no start image
  0,    // no parameter
  0,    // no parameter
  SegaIcon, // icon
  SegaVerySmallIcon, // icon
  &arcadeMenu,    // has no parent
  &zektorMenu,    // ! firstChild must be set
  &allCinematronicsMenu,    // ! left must be set
  &otherArcadeMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","", 0},    // no text
};
MenuItemNew omegaMenu;
MenuItemNew otherArcadeMenu =
{
  203,    // ID
  "OTHER ARCADE",    // no name
  0,    // directory
  0,    // no start image
  0,    // no parameter
  0,    // no parameter
  0, // icon
  0, // icon
  &arcadeMenu,    // has no parent
  &omegaMenu,    // ! firstChild must be set
  &segaMenu,    // ! left must be set
  0,    // ! right must be set
  0,    // scrolltext
  {"","","","","", 0},    // no text
};

//////////////////////////////////////////
// Other Atari "Menu"
//////////////////////////////////////////
MenuItemNew omegaMenu =
{
  5000,    // ID
  "OMEGA RACE",    // no name
  0,    // directory
  IMG_FILE_PREFIX "aae.img",
  "\x0a",    // no parameter
  0,    // no parameter
  OmegaIcon, // icon
  0, // icon
  &otherArcadeMenu,    // has no parent
  0,    // ! firstChild must be set
  0,    // ! left must be set
  0,    // ! right must be set
  0,    // scrolltext
  {"","","","","WORK IN PROGRESS!!","NO SOUND!",  0},    // no text
};

//////////////////////////////////////////
// Atari "Menu"
//////////////////////////////////////////
MenuItemNew asteroidsMenu;
MenuItemNew swsbtMenu =
{
  2000,    // ID
  "Star Wars (SBT)",    // no name
  0,    // directory
  IMG_FILE_PREFIX "swsbt.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  StarWarsIcon, // icon
  0, // icon
  &atariMenu,    // has no parent
  0,    // ! firstChild must be set
  0,    // ! left must be set
  &asteroidsMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","", 0},    // no text
};
MenuItemNew battleZoneMenu;
MenuItemNew asteroidsMenu =
{
  2001,    // ID
  "ASTEROIDS",    // no name
  0,    // directory
  IMG_FILE_PREFIX "asteroids_sbt.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  asteroidList, // icon
  0, // icon
  &atariMenu,    // has no parent
  0,    // ! firstChild must be set
  &swsbtMenu,    // ! left must be set
  &battleZoneMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","","THOMAS PASSION!", 0},    // no text
};
MenuItemNew blackWidowMenu;
MenuItemNew battleZoneMenu =
{
  2002,    // ID
  "BATTLE ZONE",    // no name
  0,    // directory
  IMG_FILE_PREFIX "battlezone.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  bzList, // icon
  0, // icon
  &atariMenu,    // has no parent
  0,    // ! firstChild must be setvecxExactList
  &asteroidsMenu,    // ! left must be set
  &blackWidowMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","","","",  0},    // no text
};
MenuItemNew redBaronMenu;
MenuItemNew blackWidowMenu =
{
  2003,    // ID
  "BLACK WIDOW",    // no name
  0,    // directory
  IMG_FILE_PREFIX "blackwidow.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  teklaList, // icon
  0, // icon
  &atariMenu,    // has no parent
  0,    // ! firstChild must be set
  &battleZoneMenu,    // ! left must be set
  &redBaronMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","WORK IN PROGRESS!!","FLICKER, NO SOUND!",  0},    // no text
};
MenuItemNew lunarMenu;
MenuItemNew redBaronMenu =
{
  2004,    // ID
  "RED BARON",    // no name
  0,    // directory
  IMG_FILE_PREFIX "redbaron.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  baronlist, // icon
  0, // icon
  &atariMenu,    // has no parent
  0,    // ! firstChild must be set
  &blackWidowMenu,    // ! left must be set
  &lunarMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","WORK IN PROGRESS!!","FLICKER, NO SOUND!",  0},    // no text
};
MenuItemNew gravitarMenu;
MenuItemNew lunarMenu =
{
  2005,    // ID
  "LUNAR LANDER",    // no name
  0,    // directory
  IMG_FILE_PREFIX "lunar.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  landerList, // icon
  0, // icon
  &atariMenu,    // has no parent
  0,    // ! firstChild must be set
  &redBaronMenu,    // ! left must be set
  &gravitarMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","WORKING GOOD!","",  0},    // no text
};
MenuItemNew spaceDuelMenu;
MenuItemNew gravitarMenu =
{
  2006,    // ID
  "GRAVITAR",    // no name
  0,    // directory
  IMG_FILE_PREFIX "gravitar.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  gravitarList, // icon
  0, // icon
  &atariMenu,    // has no parent
  0,    // ! firstChild must be set
  &lunarMenu,    // ! left must be set
  &spaceDuelMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","WORK IN PROGRESS!","NO SOUND!",  0},    // no text
};
MenuItemNew tempestMenu;
MenuItemNew spaceDuelMenu =
{
  2007,    // ID
  "SPACE DUEL",    // no name
  0,    // directory
  IMG_FILE_PREFIX "spaceduel.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  duelList, // icon
  0, // icon
  &atariMenu,    // has no parent
  0,    // ! firstChild must be set
  &gravitarMenu,    // ! left must be set
  &tempestMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","WORK IN PROGRESS!!","FLICKER, NO SOUND!",  0},    // no text
};

MenuItemNew asteroids2Menu;
MenuItemNew majorHavocMenu;
MenuItemNew tempestMenu =
{
  2008,    // ID
  "TEMPEST",    // no name
  0,    // directory
  IMG_FILE_PREFIX "tempest.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  tempestList, // icon
  0, // icon
  &atariMenu,    // has no parent
  0,    // ! firstChild must be set
  &spaceDuelMenu,    // ! left must be set
  &majorHavocMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","WORK IN PROGRESS!!","FLICKER, NO SOUND!",  0},    // no text
};

MenuItemNew asteroidsDeluxeMenu;
MenuItemNew majorHavocMenu =
{
  2009,    // ID
  "MAJOR HAVOC",    // no name
  0,    // directory
  IMG_FILE_PREFIX "aae.img",
  "\x2d",    // no parameter
  0,    // no parameter
  MajorHavocIcon, // icon
  0, // icon
  &atariMenu,    // has no parent
  0,    // ! firstChild must be set
  &tempestMenu,    // ! left must be set
  &asteroidsDeluxeMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","WORK IN PROGRESS!!","FLICKER, NO SOUND!",  0},    // no text
};

MenuItemNew quantumMenu;
MenuItemNew asteroidsDeluxeMenu =
{
  2010,    // ID
  "ASTEROIDS DELUXE",    // no name
  0,    // directory
  IMG_FILE_PREFIX "aae.img",
  "\x09",    // no parameter
  0,    // no parameter
  Asteroids2Icon, // icon
  0, // icon
  &atariMenu,    // has no parent
  0,    // ! firstChild must be set
  &majorHavocMenu,    // ! left must be set
  &quantumMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","WORK IN PROGRESS!!","FLICKER, NO SOUND!","","",  0},    // no text
};

MenuItemNew quantumMenu =
{
  2011,    // ID
  "QUANTUM",    // no name
  0,    // directory
  IMG_FILE_PREFIX "aae.img",
  "\x36",    // no parameter
  0,    // no parameter
  QuantumIcon, // icon
  0, // icon
  &atariMenu,    // has no parent
  0,    // ! firstChild must be set
  &asteroidsDeluxeMenu,    // ! left must be set
  0,    // ! right must be set
  0,    // scrolltext
  {"","","","","WORK IN PROGRESS!!","FLICKER, NO SOUND!","","",  0},    // no text
};

//////////////////////////////////////////
// Vectrex Cine "Menu"
//////////////////////////////////////////
MenuItemNew speedfreakMenu;
MenuItemNew tailgunnerMenu =
{
  3000,    // ID
  "TAILGUNNER",    // no name
  0,    // directory
  IMG_FILE_PREFIX "tailgunner.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  tailgunnerList, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  0,    // ! left must be set
  &speedfreakMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","","GRAHAMS PASSION!", 0},    // no text
};
MenuItemNew armorAttackMenu;
MenuItemNew speedfreakMenu =
{
  3001,    // ID
  "SPEEDFREAK",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
//  "speedfreak",    // no parameter
  IMG_FILE_PREFIX "aae.img",
  "\x42",    // no parameter
  0,    // no parameter
  speedfrkList, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &tailgunnerMenu,    // ! left must be set
  &armorAttackMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
};
MenuItemNew boxingbugsMenu;
MenuItemNew armorAttackMenu =
{
  3002,    // ID
  "ARMOR ATTACK",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
  IMG_FILE_PREFIX "aae.img",
  "\x3a",    // no parameter
  0,    // no parameter
  armoraList, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &speedfreakMenu,    // ! left must be set
  &boxingbugsMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
};
MenuItemNew demonMenu;
MenuItemNew boxingbugsMenu =
{
  3003,    // ID
  "BOXING BUGS",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
//  "boxingbugs",    // no parameter
  IMG_FILE_PREFIX "aae.img",
  "\x44",    // no parameter
  0,    // no parameter
  bbList, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &armorAttackMenu,    // ! left must be set
  &demonMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
};
MenuItemNew ripoffMenu;
MenuItemNew demonMenu =
{
  3004,    // ID
  "DEMON",    // no name
  0,    // directory
  IMG_FILE_PREFIX "cine.img",    // no start image
  "demon",    // no parameter
  0,    // no parameter
  demonList, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &boxingbugsMenu,    // ! left must be set
  &ripoffMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","CINEMATRONICS EMU","","SORT OF...",  0},    // no text
};
MenuItemNew spaceWarsMenu;
MenuItemNew ripoffMenu =
{
  3005,    // ID
  "RIPOFF",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
//  "ripoff",    // no parameter
  IMG_FILE_PREFIX "aae.img",
  "\x3e",    // no parameter
  0,    // no parameter
  ripoffList, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &boxingbugsMenu,    // ! left must be set
  &spaceWarsMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
};
MenuItemNew starcastleMenu;
MenuItemNew spaceWarsMenu =
{
  3006,    // ID
  "SPACE WARS",    // no name
  0,    // directory
  IMG_FILE_PREFIX "cine.img",    // no start image
  "spacewars",    // no parameter
  0,    // no parameter
  0, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &ripoffMenu,    // ! left must be set
  &starcastleMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","CINEMATRONICS EMU","","INPUT NOT WORKING",  0},    // no text
};
MenuItemNew sundanceMenu;
MenuItemNew starcastleMenu =
{
  3007,    // ID
  "STAR CASTLE",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
//  "starcastle",    // no parameter
  IMG_FILE_PREFIX "aae.img",
  "\x39",    // no parameter
  0,    // no parameter
  starCastleIcon, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &spaceWarsMenu,    // ! left must be set
  &sundanceMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
};
MenuItemNew waroftheworldsMenu;
MenuItemNew sundanceMenu =
{
  3008,    // ID
  "SUNDANCE",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
//  "sundance",    // no parameter
  IMG_FILE_PREFIX "aae.img",
  "\x3c",    // no parameter
  0,    // no parameter
  sundanceList, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &starcastleMenu,    // ! left must be set
  &waroftheworldsMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
};
MenuItemNew barrierMenu;
MenuItemNew waroftheworldsMenu =
{
  3009,    // ID
  "WAR OF THE WORLDS",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
//  "waroftheworlds",    // no parameter
  IMG_FILE_PREFIX "aae.img",
  "\x45",    // no parameter
  0,    // no parameter
  WOTWIcon, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &sundanceMenu,    // ! left must be set
  &barrierMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
};
MenuItemNew qb3Menu;
MenuItemNew cosmicchasmMenu;
MenuItemNew solarquestMenu;
MenuItemNew barrierMenu =
{
  3010,    // ID
  "BARRIER",    // no name
  0,    // directory
  IMG_FILE_PREFIX "cine.img",    // no start image
  "barrier",    // no parameter
  0,    // no parameter
  barrierList, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &waroftheworldsMenu,    // ! left must be set
  &solarquestMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","CINEMATRONICS EMU","","STRANGE GAME",  0},    // no text
};
// I don't know how to load cosmic chasm...
MenuItemNew cosmicchasmMenu =
{
  3011,    // ID
  "COSMIC CHASM",    // no name
  0,    // directory
  IMG_FILE_PREFIX "cine.img",    // no start image
  "cosmicchasm",    // no parameter
  0,    // no parameter
  0, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &barrierMenu,    // ! left must be set
  &qb3Menu,    // ! right must be set
  0,    // scrolltext
  {"","","","","CINEMATRONICS EMU","","NOT WORKING NEEDS 68000","",  0},    // no text
};
// Not qorking and crashing....
MenuItemNew qb3Menu =
{
  3012,    // ID
  "QB3",    // no name
  0,    // directory
  IMG_FILE_PREFIX "cine.img",    // no start image
  "qb3",    // no parameter
  0,    // no parameter
  0, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &barrierMenu,    // ! left must be set
  &solarquestMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","CINEMATRONICS EMU","","CRASHING",  0},    // no text
};
MenuItemNew starhawkMenu;
MenuItemNew solarquestMenu =
{
  3013,    // ID
  "SOLAR QUEST",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
//  "solarquest",    // no parameter
  IMG_FILE_PREFIX "aae.img",
  "\x38",    // no parameter
  0,    // no parameter
  solarQuestIcon, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &barrierMenu,    // ! left must be set
  &starhawkMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
};
MenuItemNew warriorMenu;
MenuItemNew starhawkMenu =
{
  3014,    // ID
  "STAR HAWK",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
//  "starhawk",    // no parameter
  IMG_FILE_PREFIX "aae.img",
  "\x40",    // no parameter
  0,    // no parameter
  shawkList, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &solarquestMenu,    // ! left must be set
  &warriorMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
};
MenuItemNew tailgunner2Menu;
MenuItemNew warriorMenu =
{
  3015,    // ID
  "WARRIOR",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
//  "warrior",    // no parameter
  IMG_FILE_PREFIX "aae.img",
  "\x3d",    // no parameter
  0,    // no parameter
  warriorList, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &starhawkMenu,    // ! left must be set
  0,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
};
/*
MenuItemNew tailgunner2Menu =
{
  2000,    // ID
  "TAILGUNNER",    // no name
  0,    // directory
  IMG_FILE_PREFIX "tailgunner.img",    // no start image
  0,    // no parameter
  0,    // no parameter
  tailgunnerList, // icon
  0, // icon
  &allCinematronicsMenu,    // has no parent
  0,    // ! firstChild must be set
  &warriorMenu,    // ! left must be set
  0,    // ! right must be set
  0,    // scrolltext
  {"","","","SBT","","GRAHAMS PASSION!", 0},    // no text
};
*/


//////////////////////////////////////////
// Vectrex Sega "Menu"
//////////////////////////////////////////

MenuItemNew tacscanMenu;
MenuItemNew zektorMenu =
{
  4000,    // ID
  "Zektor",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
//  "starhawk",    // no parameter
  IMG_FILE_PREFIX "aae.img",
  "\x22",    // no parameter
  0,    // no parameter
  ZektorIcon, // icon
  0, // icon
  &segaMenu,    // has no parent
  0,    // ! firstChild must be set
  0,    // ! left must be set
  &tacscanMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
};

MenuItemNew starTrekMenu;
MenuItemNew tacscanMenu =
{
  4001,    // ID
  "TACSCAN",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
//  "starhawk",    // no parameter
  IMG_FILE_PREFIX "aae.img",
  "\x23",    // no parameter
  0,    // no parameter
  TacScanIcon, // icon
  0, // icon
  &segaMenu,    // has no parent
  0,    // ! firstChild must be set
  &zektorMenu,    // ! left must be set
  &starTrekMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
};

MenuItemNew spaceFuryMenu;
MenuItemNew starTrekMenu =
{
  4002,    // ID
  "STARTREK",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
//  "starhawk",    // no parameter
  IMG_FILE_PREFIX "aae.img",
  "\x24",    // no parameter
  0,    // no parameter
  StarTrekIcon, // icon
  0, // icon
  &segaMenu,    // has no parent
  0,    // ! firstChild must be set
  &tacscanMenu,    // ! left must be set
  &spaceFuryMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
}; 

MenuItemNew eleminatorMenu;
MenuItemNew spaceFuryMenu =
{
  4003,    // ID
  "SPACFURY",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
//  "starhawk",    // no parameter
  IMG_FILE_PREFIX "aae.img",
  "\x25",    // no parameter
  0,    // no parameter
  SpaceFuryIcon, // icon
  0, // icon
  &segaMenu,    // has no parent
  0,    // ! firstChild must be set
  &starTrekMenu,    // ! left must be set
  &eleminatorMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
};
MenuItemNew eleminatorMenu =
{
  4004,    // ID
  "ELEMINATOR",    // no name
  0,    // directory
//  IMG_FILE_PREFIX "cine.img",    // no start image
//  "starhawk",    // no parameter
  IMG_FILE_PREFIX "aae.img",
  "\x28",    // no parameter
  0,    // no parameter
  EleminatorIcon, // icon
  0, // icon
  &segaMenu,    // has no parent
  0,    // ! firstChild must be set
  &spaceFuryMenu,    // ! left must be set
  0,    // ! right must be set
  0,    // scrolltext
  {"","","","","AAE EMU","","No SOUND",  0},    // no text
};


MenuItemNew aaeMenu;


MenuItemNew aaeMenu =
{
  6000,    // ID
  "AAE",    // no name
  0,    // directory
  IMG_FILE_PREFIX "aae.img",    // 
  0,    // no parameter
  0,    // no parameter
  0, // icon
  0, // icon
  0,    // has no parent
  0,    // ! firstChild must be set
  0,    // ! left must be set
  0,    // ! right must be set
  0,    // scrolltext
  {"","","","","", 0},    // no text
};

MenuItemNew blablaMenu;
MenuItemNew badAppleMenu =
{
  300,    // ID
  "BAD APPLE",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexvideo.img",    // no start image
  "vectrex/movies",    // no parameter
  "BadApple.bin",    // no parameter
  badAppleList, // icon
  0, // icon
  &videoMenu,    // has no parent 
  0,    // ! firstChild must be set
  0,    // ! left must be set
  &blablaMenu,    // ! right must be set
  0,    // scrolltext
  {"", "", "","THE CLIP EVERY!", "MACHINE MUST SHOW!", 0},    // no text
}; 
MenuItemNew laLineaMenu;
MenuItemNew blablaMenu =
{
  301,    // ID
  "BLA BLA",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexvideo.img",    // no start image
  "vectrex/movies",    // no parameter
  "BlaBla.bin",    // no parameter
  blablaList, // icon
  0, // icon
  &videoMenu,    // has no parent
  0,    // ! firstChild must be set
  &badAppleMenu,    // ! left must be set
  &laLineaMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","VERY RYTHMIC,", "LA LINEA STYLE", 0},    // no text
};
MenuItemNew badAppleDeMenu;
MenuItemNew laLineaMenu =
{
  302,    // ID
  "LA LINEA",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexvideo.img",    // no start image 
  "vectrex/movies",    // no parameter
  "LaLinea.bin",    // no parameter
  lineaList, // icon
  0, // icon
  &videoMenu,    // has no parent
  0,    // ! firstChild must be set
  &blablaMenu,    // ! left must be set
  &badAppleDeMenu,    // ! right must be set
  0,    // scrolltext
  {"","","","", "ITALIEN LA LINEA", 0},    // no text
};
MenuItemNew badAppleDeMenu =
{
  303,    // ID
  "BAD APPLE DE",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexvideo.img",    // no start image
  "vectrex/movies",    // no parameter
  "BadAppleDE.bin",    // no parameter
  badAppleList, // icon
  0, // icon
  &videoMenu,    // has no parent
  0,    // ! firstChild must be set
  &laLineaMenu,    // ! left must be set
  0,    // ! right must be set
  0,    // scrolltext
  {"","","","A GERAMN VERSION", "OF THE WELL KNOWN CLIP", 0},    // no text
};

MenuItemNew purpleMenu;
MenuItemNew bohemianMenu =
{
  400,    // ID
  "QUEEN",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexaudio.img",    // no start image
  "vectrex/waves",    // no parameter
  "Bohemian.bin",    // no parameter
  0, // icon
  0, // icon
  &audioMenu,    // has no parent
  0,    // ! firstChild must be set
  0,    // ! left must be set
  &purpleMenu,    // ! right must be set
  0,    // scrolltext
  {"DIGITIZED MUSIC", "SAMPLED WITH 12KHZ", 0},    // no text
};
MenuItemNew purpleMenu =
{
  401,    // ID

  "PRINCE",    // no name
  0,    // directory
  IMG_FILE_PREFIX "vectrexaudio.img",    // no start image
  "vectrex/waves",    // no parameter
  "PurpleRain.bin",    // no parameter
  0, // icon
  0, // icon
  &audioMenu,    // has no parent
  0,    // ! firstChild must be set
  &bohemianMenu,    // ! left must be set
  0,    // ! right must be set
  0,    // scrolltext
  {"DIGITIZED MUSIC", "SAMPLED WITH 12KHZ", 0},    // no text
};

// todo - all other roms!
int testOneRomId(int id)
{
  char *testPath;
  switch (id)
  {
    case 2000: // release
    {
      testPath = "roms/aae/starwars.zip";
      break;
    }
    case 2001: // asteroids_sbt.img
    {
      testPath = "roms/asteroid2.zip";
      break;
    }
    case 2002: // battleZone
    {
      testPath = "roms/bzone.zip";
      break;
    }
    case 2003: // blackWidow
    {
      testPath = "roms/bwidow.zip";
      break;
    }
    case 2004: // red baron
    {
      testPath = "roms/redbaron.zip";
      break;
    }
    case 2005: // "LUNAR LANDER"
    {
      testPath = "roms/llander.zip";
      break;
    }
    case 2006: // GRAVITAR
    {
      testPath = "roms/gravitar.zip";
      break;
    }
    case 2007: // SPACE DUEL
    {
      testPath = "roms/spacduel.zip";
      break;
    }
    case 2008: // TEMPEST
    {
      testPath = "roms/tempest.zip";
      break;
    }
    case 2009: //
    {
      testPath = "roms/aae/mhavoc.zip";
      break;
    }
    case 2010: // Asteroid deluce
    {
      testPath = "roms/aae/astdelux.zip";
      break;
    }
    case 2011: // 
    {
      testPath = "roms/aae/quantum.zip";
      break;
    }
    case 3000: // tailgunner
    {
      testPath = "roms/tailg.zip";
      break;
    }
    case 3001: // tailgunner
    {
      testPath = "roms/aae/speedfrk.zip";
      break;
    }
    case 3002: // 
    {
      testPath = "roms/aae/armora.zip";
      break;
    }
    case 3003: // 
    {
      testPath = "roms/aae/boxingb.zip";
      break;
    }
    case 3004: // 
    {
      testPath = "roms/demon.zip";
      break;
    }
    case 3005: // 
    {
      testPath = "roms/aae/ripoff.zip";
      break;
    }
    case 3006: // 
    {
      testPath = "roms/spacewar.zip";
      break;
    }
    case 3007: // 
    {
      testPath = "roms/aae/starcas.zip";
      break;
    }
    case 3008: // 
    {
      testPath = "roms/aae/sundance.zip";
      break;
    }
    case 3009: // 
    {
      testPath = "roms/aae/wotw.zip";
      break;
    }
    case 3010: // 
    {
      testPath = "roms/barrier.zip";
      break;
    }
    case 3013: // 
    {
      testPath = "roms/aae/solarq.zip";
      break;
    }
    case 3014: // 
    {
      testPath = "roms/aae/starhawk.zip";
      break;
    }
    case 3015: // 
    {
      testPath = "roms/aae/warrior.zip";
      break;
    }
    case 4000: // 
    {
      testPath = "roms/aae/zektor.zip";
      break;
    }
    case 4001: // 
    {
      testPath = "roms/aae/tacscan.zip";
      break;
    }
    case 4002: // 
    {
      testPath = "roms/aae/startrek.zip";
      break;
    }
    case 4003: // 
    {
      testPath = "roms/aae/spacfury.zip";
      break;
    }
    case 4004: // 
    {
      testPath = "roms/aae/elim2.zip";
      break;
    }
    case 5000: // 
    {
      testPath = "roms/aae/omegrace.zip";
      break;
    }
    
    default:
      return 1; // ok
      break;
  }
  FRESULT rc_rd = FR_DISK_ERR;
  FIL file_object_rd;
  rc_rd = f_open(&file_object_rd, testPath, (unsigned char) FA_READ);
  
  if (rc_rd != FR_OK)
  {
//    printf("File not found!\r\n");
    return 0;
  }
  f_close(&file_object_rd);
  return 1;
}
    int yPos = 0;

int allRomOks[10000]; // lokup table... yes - slightly oversized :-)
void initTestRoms()
{
  for (int i=0; i<10000;i++)
  {
    allRomOks[i] = testOneRomId(i);
  }
}


int testRom(MenuItemNew *m)
{
  return allRomOks[m->id];
}

char *scrollpointer=0;
int scroll_y;
int scroll_x;
int scroll_x_offset;
int scrollDelay;

void displayMenuItem(MenuItemNew *m)
{
  if (m->icon != 0)
  {
    displayLargeList(m->icon);
  }
  if (m->parent != 0)
  {
    MenuItemNew *p = m->parent;
    if (p->smallIcon != 0)
    {
      displayLargeListUp(p->smallIcon);
    }
    else if (p->DISPLAYNAME != 0)
    {
      v_printString(-20, 100, p->DISPLAYNAME, 3, 0x57);
    }
    else 
    {
      const signed char *list = upIcon;
      int count = *list++;

      while (count >0)  
      {
        int y0 = *list++;
        int x0 = *list++;
        int y1 = *list++;
        int x1 = *list++;
        v_directDraw32( (x0<<6), (y0<<6) +(110<<7),(x1<<6) ,(y1<<6)+(110<<7), 0x5f);
        count --;
      }
    }
  }
  if (m->left != 0)
  {
    const signed char *list = leftIcon;
    int count = *list++;

    while (count >0)  
    {
      int y0 = *list++;
      int x0 = *list++;
      int y1 = *list++;
      int x1 = *list++;
      v_directDraw32((x0<<6) -((110)<<7), y0<<6,(x1<<6) -((110)<<7),y1<<6, 0x5f);
      count --;
    }
  }

  if (m->right != 0)
  {
    const signed char *list = rightIcon;
    int count = *list++;

    while (count >0)   
    {
      int y0 = *list++;
      int x0 = *list++;
      int y1 = *list++;
      int x1 = *list++;
      v_directDraw32((x0<<6) +((110)<<7), y0<<6,(x1<<6) +((110)<<7),y1<<6, 0x5f);
      count --;
    }
  }
  if (m->child != 0)
  {
    const signed char *list = downIcon;
    int count = *list++;

    while (count >0)  
    {
      int y0 = *list++;
      int x0 = *list++;
      int y1 = *list++;
      int x1 = *list++;
      v_directDraw32( (x0<<6), (y0<<6) -(110<<7),(x1<<6) ,(y1<<6)-(110<<7), 0x5f);
      count --;
    }
  }

  if (m->DISPLAYNAME != 0) 
  {
    int l = strlen(m->DISPLAYNAME)*5;
    v_printString(-l, 50, m->DISPLAYNAME, 10, 0x57);

    if (!testRom(m))
    {
      v_printString(-60, -125, "ROM NOT FOUND!", 10, 0x57);
    }
  }
  if (m->text != 0)
  {
    int yPos = 0;
    int count = 0;
    while (m->text[count] != 0)
    {
      v_printString(-40, yPos, m->text[count++], 5, 0x48);
      yPos -= 18;
    }
  }
  if (scrollReset)
  {
    scrollpointer = m->scrolltext;
    scroll_y= -125;
    scroll_x= -80;
    scroll_x_offset= 0;
    scrollReset = 0;
    scrollDelay = 40;
    
  }
  if (m->scrolltext != 0)
  {
     v_printStringMax(-38-(2-(scrollDelay/3) ), scroll_y, scrollpointer, 4, 0x2f, 20);

    scrollDelay--;
    if (scrollDelay <= 0)
    {
      scrollDelay = 6;
      scrollpointer++;
    }

     if (*scrollpointer == 0)scrollReset=1;
  }
}


// return 1 on true
int isNameOk(dirent *finfo)
{
  char *n = finfo->d_name;
	  
  int len = strlen(n);
  if (*n=='.') return 0; // exclude all files starting with a .
  
  if (strcasecmp(".bin", n+len-4) == 0) return 1;
  if (finfo->d_type == DT_DIR) return 1;
  return 0;
  
}

// loads all files in the given Dir (sub directory from here)
// into: nameToFill -> names
//     : fileTypes -> types
// filenames are only accepted if they end in ".bin" (not case sensitive)
// directories are also accepted with any name

// Comparison function for qsort
int compareStrings(const void *a, const void *b) 
{
    const char **strA = (const char **)a;
    const char **strB = (const char **)b;
    return strcmp(*strA, *strB);
}

void ensureMainPath()
{
  chdir("0:/");
}


// return 0 on ok
int loadFileNames(char *dir)
{
//    outputCurrentDirectory();
//    char *vectrexDir = "vectrex";

    if (chdir(dir)<0)
    {
        printf("loaderMain.c: loadFileNames(): NO '%s' directory found...!\r\n", dir);
        return -1;
    }
  
    char buf[256];
    if (getcwd (buf,256)==0)
    {
      printf("loaderMain.c: loadFileNames(): f_getcwd failed (%i), '%s' \r\n", errno, dir);
    
      ensureMainPath();
      return -1;
    }
//printf("Dir: %s\n", buf);    
    

    DIR *dp;
    dp = opendir (buf);
    if (dp == 0)
    {
      printf("loaderMain.c: loadFileNames(): opendir failed (%i), '%s'\r\n", errno, dir);
      ensureMainPath();
      return -2;
    }
    dirent *finfo;
    int d = 0;
    int f = 0;
    
    // init pointers for sorting
    for (int i=0;i<MAX_V_FILES; i++)
      filesInDir[i] = &_filesInDir[i][0];
    for (int i=0;i<MAX_V_DIRS; i++)

      dirsInDir[i] = &_dirsInDir[i][0];


//int count = 0;
    
    do
    {
      finfo =  readdir(dp);
//printf("FileInfoCount: %i - ", count++);
      if (finfo != 0)
      {
        if (isNameOk(finfo))
        {
	  if (finfo->d_type == DT_DIR)
	  {
//printf("Dir: %s - \n", finfo->d_name);



	    // skip known names
	    if (strcmp(finfo->d_name, "bios") == 0) continue;
	    if (strcmp(finfo->d_name, "movies") == 0) continue;
	    if (strcmp(finfo->d_name, "waves") == 0) continue;

	    // allow valid vectrex file directories
	    // if (strcmp(finfo->d_name, "original") == 0) continue;
	    // if (strcmp(finfo->d_name, "demos") == 0) continue;
	    
	    char *nameToFill=(char *) &(_filesInDir[d][0]);
	    int c=0;
	    for (; c<MAX_VECTREX_FILENAME_LENGTH-1; c++)
	    {
	      nameToFill[c]=finfo->d_name[c];
	      if (nameToFill[c]==0) break;
	    }
	    nameToFill[c]=0;
	    d++;
	  }
	  else
	  {
//printf("File: %s - \n", finfo->d_name);
	    char *nameToFill=(char *) &(_dirsInDir[f][0]);
	    int c=0;
	    for (; c<MAX_VECTREX_FILENAME_LENGTH-1; c++)
	    {
	      nameToFill[c]=finfo->d_name[c];
	      if (nameToFill[c]==0) break;
	    }
	    nameToFill[c]=0;
//printf("File Read: %s\n", &(_dirsInDir[f][0]));
	    f++;
	  }
        }
        else
	{
//printf("Name not Ok: '%s'\n", finfo->d_name);
	  
	}
      }
    } while (finfo != 0);
    closedir (dp);
    
    
//outputCurrentDirectory();
    
    
    
    ensureMainPath();
    
    // sort directories
    qsort(filesInDir, d, sizeof(const char *), compareStrings);
    
    // sort files
    qsort(dirsInDir, f, sizeof(const char *), compareStrings);
    
    // put them all into one array
    for (int i=0;i<f;i++)
    {
      filesInDir[d+i] = dirsInDir[i];
    }

    // correct file types
    for (int i=0;i<d;i++)
    {
      fileTypes[i] = DT_DIR;
    }
    for (int i=0;i<f;i++)
    {
      fileTypes[d+i] = DT_REG;
    }
    
    for (int i=d+f; i<MAX_V_DIRS+MAX_V_FILES;i++)
    {
	filesInDir[i][0] = 0;
    }
    return 0;
}

  static char *aaeList[]=
  {
//Lunar Lander Hardware
/* 00 */ "LLANDER1",  			
/* 01 */ "LLANDER",   
//Asteroids Hardware
/* 02 */ "METEORTS",  
/* 03 */ "ASTEROCK",  
/* 04 */ "ASTEROIB",  
/* 05 */ "ASTEROI1",  
/* 06 */ "ASTEROID",  
/* 07 */ "ASTDELU1",  
/* 08 */ "ASTDELU2",  
/* 09 */ "ASTDELUX",  
//Midway Omega Race Hardware
/* 0a */ "OMEGRACE",  
/* 0b */ "DELTRACE",  
//BattleZone Hardware
/* 0c */ "BZONE",     
/* 0d */ "BZONE2",    
/* 0e */ "BZONEC",    
/* 0f */ "BZONEP",    
/* 10 */ "REDBARON",  
/* 11 */ "BRADLEY (NOT)",   
//Spacduel Hardware
/* 12 */ "SPACDUEL",  
/* 13 */ "BWIDOW",    
/* 14 */ "GRAVITAR",  
/* 15*/ "GRAVITR2", 
/* 16 */ "GRAVP",     
/* 17 */ "LUNARBAT",  
/* 18 */ "LUNARBA1",  
//Tempest Hardware
/* 19 */ "TEMPESTM",  
/* 1a */ "TEMPEST",   
/* 1b */ "TEMPEST3",  
/* 1c */ "TEMPEST2", 
/* 1d */ "TEMPEST1",  
/* 1e */ "TEMPTUBE",  
/* 1f */ "ALIENST",   
/* 20 */ "VBREAK",    
/* 21 */ "VORTEX",    
//Sega G80 Vector Hardware
/* 22 */ "ZEKTOR",    
/* 23 */ "TACSCAN",   
/* 24 */ "STARTREK",  
/* 25 */ "SPACFURY", 
/* 26 */ "SPACFURA",  
/* 27 */ "SPACFURB",  
/* 28 */ "ELIM2",     
/* 29 */ "ELIM2A",    
/* 2a */ "ELIM2C",    
/* 2b */ "ELIM4",     
/* 2c */ "ELIM4P",    
//Major Havoc Hardware
/* 2d */ "MHAVOC",    
/* 2e */ "MHAVOC2",   
/* 2f */ "MHAVOCRV",  
/* 30 */ "MHAVOCP",   
/* 31 */ "ALPHAONE",  
/* 32 */ "ALPHAONA",  
//Star Wars Hardware
/* 33 */ "STARWARS",  
/* 34 */ "STARWAR1",  
//Quantum Hardware
/* 35 */ "QUANTUM1 (NOT)",  
/* 36 */ "QUANTUM",  
/* 37 */ "QUANTUMP (NOT)",  
//Cinematronics Hardware
/* 38 */ "SOLARQ",    
/* 39 */ "STARCAS",   
/* 3a */ "ARMORA",    
/* 3b */ "BARRIER (NOT)",   
/* 3c */ "SUNDANCE",  
/* 3d */ "WARRIOR",   
/* 3e */ "RIPOFF",    
/* 3f */ "TAILG",     
/* 40 */ "STARHAWK",  
/* 41 */ "SPACEWAR (NOT)",  
/* 42 */ "SPEEDFRK",  
/* 43 */ "DEMON (NOT)",     
/* 44 */ "BOXINGB",   
/* 45 */ "WOTW", 

    0,
    0,
    0,
    0,
    0,
  };

void oneSubDirIn()
{
  if (fileTypes[currentSelectedItem] != DT_DIR) return;

  strcat(subdirectoryPath,"/");
  strcat(subdirectoryPath,filesInDir[currentSelectedItem]);

  char dirBuffer[MAX_VECTREX_FILENAME_LENGTH*2];
  dirBuffer[0] = 0;
  strcat(dirBuffer, "vectrex"); // starting path
  strcat(dirBuffer,"/");
  strcat(dirBuffer,subdirectoryPath);
  loadFileNames(dirBuffer);
  currentSelectedItem = 0;
}

void oneSubDirOut()
{
  int i;
  char lastSelection[MAX_VECTREX_FILENAME_LENGTH];

//printf("OrgSub: %s\n", subdirectoryPath);  
  for (i=strlen(subdirectoryPath);i>=0;i--)
  {
    if (subdirectoryPath[i]=='/')
    {
      subdirectoryPath[i]=0;
      break;
    }
  }
//printf("Reduced: '%s'\n", subdirectoryPath);  
  
  int c=0;
  do
  {
    i++;
    lastSelection[c++] = subdirectoryPath[i];
    
  } while (subdirectoryPath[i] != 0);
//printf("lastSelection: %s\n", lastSelection);  
  
  
  char dirBuffer[MAX_VECTREX_FILENAME_LENGTH*2];
  dirBuffer[0] = 0;
  strcat(dirBuffer, "vectrex"); // starting path
  strcat(dirBuffer,"/");
  strcat(dirBuffer,subdirectoryPath);
  
  if (dirBuffer[strlen(dirBuffer)-1]=='/') dirBuffer[strlen(dirBuffer)-1]=0;

//printf("DirLoading: %s\n", dirBuffer);  

  loadFileNames(dirBuffer);

  // find selected item
  c = 0;
  currentSelectedItem = 0;
  while (c<MAX_V_FILES+MAX_V_DIRS)
  {
    if (strcmp(filesInDir[c], lastSelection) ==0 )
    {
//printf("SelectionFound: %s\n", filesInDir[c]);  
	currentSelectedItem = c;
	break;
    }
    c++;
    
  }
//printf("Done\n");  
  
}
    
  
  
#define TIMING_MAX 25
int timingAnalog=TIMING_MAX;
  
void displayMenu()
{
  displayMenuItem(currentMenuItem);
  int currentType = DT_DIR-1;
  
  // vectrex exact menu select
  if (currentMenuItem->id == 9999)
  {
    int yPos = 0;
    int itemDisplayStart = currentSelectedItem-2;

    if ((itemDisplayStart>=0) && (filesInDir[itemDisplayStart+0][0] != 0))
    {
      if (fileTypes[itemDisplayStart+0] == DT_DIR)
      {
	char __buff[256];
	sprintf(__buff,"$ %s", (char *) &(filesInDir[itemDisplayStart][0]));
	v_printString(-51, yPos -0*18, __buff, 5, 0x18);
      }
      else
	v_printString(-40, yPos -0*18, (char *) &(filesInDir[itemDisplayStart][0]), 5, 0x18);
    } 

    if ((itemDisplayStart+1>=0) && (filesInDir[itemDisplayStart+1][0] != 0))
    {
      if (fileTypes[itemDisplayStart+1] == DT_DIR)
      {
	char __buff[256];
	sprintf(__buff,"$ %s", (char *) &(filesInDir[itemDisplayStart+1][0]));
	v_printString(-51, yPos -1*18, __buff, 5, 0x28);
      }
      else
	v_printString(-40, yPos-1*18, (char *) &(filesInDir[itemDisplayStart+1][0]), 5, 0x28);
    }  

    if (filesInDir[itemDisplayStart+2][0] != 0)
    {
      if (fileTypes[itemDisplayStart+2] == DT_DIR)
      {
	char __buff[256];
	sprintf(__buff,"$ %s", (char *) &(filesInDir[itemDisplayStart+2][0]));
	v_printString(-51, yPos -2*18, __buff, 5, 0x4f);
      }
      else
	v_printString(-40, yPos-2*18, (char *) &(filesInDir[itemDisplayStart+2][0]), 5, 0x4f);
    }  
    
    if (filesInDir[itemDisplayStart+3][0] != 0)
    {
      if (fileTypes[itemDisplayStart+3] == DT_DIR)
      {
	char __buff[256];
	sprintf(__buff,"$ %s", (char *) &(filesInDir[itemDisplayStart+3][0]));
	v_printString(-51, yPos -3*18, __buff, 5, 0x28);
      }
      else
	v_printString(-40, yPos-3*18, (char *) &(filesInDir[itemDisplayStart+3][0]), 5, 0x28);
    }  
    
    if (filesInDir[itemDisplayStart+4][0] != 0)
    {
      if (fileTypes[itemDisplayStart+4] == DT_DIR)
      {
	char __buff[256];
	sprintf(__buff,"$ %s", (char *) &(filesInDir[itemDisplayStart+4][0]));
	v_printString(-51, yPos -4*18, __buff, 5, 0x18);
      }
      else
	v_printString(-40, yPos-4*18, (char *) &(filesInDir[itemDisplayStart+4][0]), 5, 0x18);
    }  
  }

  // aae menu select
  // not used atm
/*  
  if (currentMenuItem->id == 6000)
  {
    int yPos = 0;
    int itemDisplayStart = currentSelectedItem2-2;

    if ((itemDisplayStart>=0) && (aaeList[itemDisplayStart+0] != 0))
      v_printString(-40, yPos -0*18, (char *) (aaeList[itemDisplayStart]), 5, 0x18);

    if ((itemDisplayStart+1>=0) && (aaeList[itemDisplayStart+1] != 0))
      v_printString(-40, yPos-1*18, (char *) (aaeList[itemDisplayStart+1]), 5, 0x28);

    if (aaeList[itemDisplayStart+2] != 0)
      v_printString(-40, yPos-2*18, (char *) (aaeList[itemDisplayStart+2]), 5, 0x4f);
    
    if (aaeList[itemDisplayStart+3] != 0)
      v_printString(-40, yPos-3*18, (char *) (aaeList[itemDisplayStart+3]), 5, 0x28);
    
    if (aaeList[itemDisplayStart+4] != 0)
      v_printString(-40, yPos-4*18, (char *) (aaeList[itemDisplayStart+4]), 5, 0x18);
  }
*/  
  
  // navigate
  // navigate with joystick
  // select/start with a button

  int analogYChange = (abs(currentJoy1Y)/10); // 0-128
  timingAnalog -=analogYChange;
  
  
  
  if ((currentJoy1Y<-20) && (timingAnalog<0))
  {
    if (currentMenuItem->id == 9999)
    {
      if (filesInDir[currentSelectedItem+1][0] != 0)
      {
        currentSelectedItem++;
      }
    }    
//    if (currentMenuItem->id == 6000)
//    {
//      if (aaeList[currentSelectedItem2+1] != 0)
//        currentSelectedItem2++;
//    }    
    timingAnalog=TIMING_MAX;
  }
  if ((currentJoy1Y>20) && (timingAnalog<0))
  {
    if (currentMenuItem->id == 9999)
    {
      if (currentSelectedItem > 0)
      {
        currentSelectedItem--;
      }
    }    
//    if (currentMenuItem->id == 6000)
//    {
//      if (currentSelectedItem2 > 0)
//        currentSelectedItem2--;
//    }    
    timingAnalog=TIMING_MAX;
  }
  
  // joypos to "digital"
  if (currentJoy1X > 50) currentJoy1X = 1;
  else if (currentJoy1X < -50) currentJoy1X = -1;
  else currentJoy1X = 0;
  if (currentJoy1Y > 50) currentJoy1Y = 1;
  else if (currentJoy1Y < -50) currentJoy1Y = -1;
  else currentJoy1Y = 0;
  
  // only "main" menu follows  
  if ((currentJoy1X==1) && (selectionMade==0))
  {
      if (currentMenuItem->id == 9999)
      {
	oneSubDirIn();
      }
      else
      {
        if (currentMenuItem->right != 0)
	{
	  currentMenuItem = currentMenuItem->right;
	}
      }
      scrollReset = 1;
      selectionMade = 1;
  }
  
  if ((currentJoy1X==-1) && (selectionMade==0))
  {
      if ((currentMenuItem->id == 9999) && (strlen(subdirectoryPath)!=0))
      {
	oneSubDirOut();
      }
      else
      {
	if (currentMenuItem->left != 0)
	{
	  currentMenuItem = currentMenuItem->left;
	  scrollReset = 1;
	}
      }
      scrollReset = 1;
      selectionMade = 1;
  }

  
  if ((currentJoy1Y==-1) && (selectionMade==0))
  {
    if (currentMenuItem->child != 0)
    {
      currentMenuItem = currentMenuItem->child;
      scrollReset = 1;
    }
    selectionMade = 1;
  }
  if ((currentJoy1Y==1) && (selectionMade==0))
  {
    if (currentMenuItem->parent != 0)
    {
      currentMenuItem = currentMenuItem->parent;
      scrollReset = 1;
    }
    selectionMade = 1; 
  }

  if ((ABS(currentJoy1Y)==0) && (ABS(currentJoy1X)==0)) selectionMade =0;

  if ((currentButtonState&0x0f) == (0x08)) // exactly button 4
  {
    if (currentMenuItem->img != 0)
    {
      if (!((currentMenuItem->id == 9999) && (fileTypes[currentSelectedItem] == DT_DIR)) )
	loadAndStart(currentMenuItem, 4);
    }
    if (currentMenuItem->id == 0)
    {
      while ((v_directReadButtons()&0x0f) == (0x08));
      
      v_SettingsGUI(1);
      v_saveIni("vectrexInterface.ini");
    }
  }  
  if ((currentButtonState&0x0f) == (0x01)) // exactly button 1
  {
    if (currentMenuItem->img != 0)
    {
      loadAndStart(currentMenuItem, 1);
    }
  }  
/*  ENABLE ONLY FOR DEBUGGING THE LOADER!
 * otherwise e.g. a spinner in port 2 will reset the loader!
  if ((currentButtonState&0xff) == (0x10)) // exactly button 1 pad 2
  {
    reloadLoader();
  }
*/  
}
#if 0
void v_printBitmapUni(unsigned char *bitmapBlob, int width, int height, int sizeX, int x, int y);
// 9x75
unsigned char __uniDirectional[] =
{
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01111111, 0b11100000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00111111, 0b11111111, 0b11111111, 0b11100000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b11111111, 0b11111100, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000011, 0b11111111, 0b11111111, 0b11111111, 0b11111110, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11000000, 0b00000000,
  0b00000000, 0b00000000, 0b00111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11100000, 0b00000000,
  0b00000000, 0b00000000, 0b01111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000, 0b00000000,
  0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000, 0b00000000,
  0b00000000, 0b00000001, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111000, 0b00000000,
  0b00000000, 0b00000001, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111100, 0b00000000,
  0b00000000, 0b00000011, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111110, 0b00000000,
  0b00000000, 0b00000011, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000000,
  0b00000000, 0b00000011, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000000,
  0b00000000, 0b00000111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000000,
  0b00000000, 0b00000111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b10000000,
  0b00000000, 0b00000111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b10000000,
  0b00000000, 0b00000111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b10000000,
  0b00000000, 0b00001111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b10000000,
  0b00000000, 0b00000111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b10000000,
  0b00000000, 0b00000111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b10000000,
  0b00000000, 0b00000111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000000,
  0b00000000, 0b00000111, 0b11111111, 0b11110111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000000,
  0b00000000, 0b00001111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111000, 0b11111111, 0b00000000,
  0b00000000, 0b00001111, 0b11111111, 0b11100000, 0b00011111, 0b11111111, 0b00000011, 0b11111111, 0b00000000,
  0b00000001, 0b00001111, 0b11111000, 0b00000011, 0b11011111, 0b11111110, 0b00000000, 0b01111110, 0b00000000,
  0b00000001, 0b10011111, 0b11111000, 0b10000111, 0b11111111, 0b11111111, 0b00000000, 0b00000011, 0b00000000,
  0b00000000, 0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11000011, 0b10000011, 0b11000000,
  0b00000000, 0b00011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11000000,
  0b00000001, 0b10001111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11000000,
  0b00000001, 0b10001111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11000000,
  0b00000001, 0b10001111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b10000000,
  0b00000001, 0b10000111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b10000000,
  0b00000001, 0b00000011, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b10000000,
  0b00000001, 0b00000011, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000000,
  0b00000000, 0b00000001, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000000,
  0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111110, 0b00000000,
  0b00000000, 0b00000001, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111110, 0b00000000,
  0b00000000, 0b00000001, 0b11111111, 0b11111111, 0b11100001, 0b11111110, 0b11111111, 0b11111110, 0b00000000,
  0b00000000, 0b00000001, 0b11111111, 0b11111111, 0b11000000, 0b01110000, 0b01111111, 0b11111110, 0b00000000,
  0b00000000, 0b00000011, 0b11111111, 0b11111111, 0b11111000, 0b00000001, 0b11111111, 0b11111110, 0b00000000,
  0b00000000, 0b00000001, 0b11111111, 0b11111111, 0b11111111, 0b10000001, 0b11111111, 0b11111100, 0b00000000,
  0b00000000, 0b00000001, 0b11111111, 0b11111111, 0b11000001, 0b11100000, 0b01111111, 0b11111100, 0b00000000,
  0b00000000, 0b00000000, 0b11111111, 0b11111110, 0b00000000, 0b00000000, 0b00011111, 0b11111100, 0b00000000,
  0b00000000, 0b00000000, 0b11111111, 0b11111000, 0b00000000, 0b00000000, 0b00000111, 0b11111000, 0b00000000,
  0b00000000, 0b00000000, 0b11111111, 0b11110000, 0b00000000, 0b11100000, 0b00000011, 0b11110000, 0b00000000,
  0b00000000, 0b00000000, 0b11111111, 0b11100000, 0b00011111, 0b11111100, 0b00000001, 0b11110000, 0b00000000,
  0b00000000, 0b00000000, 0b01111111, 0b11000000, 0b00000000, 0b00000000, 0b00000001, 0b11110000, 0b00000000,
  0b00000000, 0b00000000, 0b00111111, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b11100000, 0b00000000,
  0b00000000, 0b00000000, 0b00011111, 0b10000010, 0b00111111, 0b11110000, 0b01100000, 0b01000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b10000011, 0b11111111, 0b11111110, 0b11110000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11100111, 0b11110000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00001000, 0b01111111, 0b11111111, 0b11110000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00001000, 0b00111111, 0b11111111, 0b11110000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000010, 0b00000111, 0b11111100, 0b11100000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000010, 0b00000000, 0b00000000, 0b11000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b00000000, 0b00000001, 0b10000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11110111, 0b11111111, 0b10000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11111111, 0b11111110, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000001, 0b11111111, 0b11111100, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00110000, 0b00000001, 0b11111111, 0b11101100, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00011100, 0b00000000, 0b01111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00011110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000011, 0b11100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000001, 0b11110000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b11111100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b01111110, 0b00000000, 0b00011110, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00011111, 0b11111110, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b01111111, 0b11001100, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111100, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01111111, 0b11111000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00111110, 0b00000000, 0b01000000, 0b00000000, 0b00000000
};

void doBitmap()
{
  v_removeIRQHandling();
  v_enableSoundOut(1);
  v_enableButtons(1);
  v_enableJoystickDigital(0,0,0,0);
  v_enableJoystickAnalog(0,0,0,0);
  
  clipminX=-5000;
  clipmaxX= 5000;
  clipminY= -5000;
  clipmaxY= 5000;
  
  fall = 0;
  blow=100;
  while(1)
  {
    v_WaitRecal();
    drawPitrex();
    v_doSound(); // not needed with IRQ Mode

    v_printBitmapUni(__uniDirectional, 9, 75, 80, -50, 0);
  }

//  v_enableJoystickAnalog(1,1,0,0);
  v_enableJoystickDigital(1,1,0,0);
  v_setupIRQHandling();  
  v_enableSoundOut(1);
  v_enableButtons(1);
  v_setRefresh(50);
  v_disableReturnToLoader();  
  
}


// for SMP test
#if RASPPI != 1 

const signed char HavocTitle[]=
{	(signed char) 110, // count of vectors
	(signed char) 0x5B, (signed char) 0x77, (signed char) 0x53, (signed char) 0x74,	// y0, x0, y1, x1
	(signed char) 0x53, (signed char) 0x74, (signed char) 0x53, (signed char) 0x1A,	// y0, x0, y1, x1
	(signed char) 0x53, (signed char) 0x1A, (signed char) 0x61, (signed char) 0x20,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0x20, (signed char) 0x61, (signed char) 0x18,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0x18, (signed char) 0x53, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0x53, (signed char) 0x12, (signed char) 0x53, (signed char) 0xAA,	// y0, x0, y1, x1
	(signed char) 0x53, (signed char) 0xAA, (signed char) 0x69, (signed char) 0xB2,	// y0, x0, y1, x1
	(signed char) 0x69, (signed char) 0xB2, (signed char) 0x53, (signed char) 0xA5,	// y0, x0, y1, x1
	(signed char) 0x53, (signed char) 0xA5, (signed char) 0x53, (signed char) 0x9A,	// y0, x0, y1, x1
	(signed char) 0x53, (signed char) 0x9A, (signed char) 0x69, (signed char) 0x9F,	// y0, x0, y1, x1
	(signed char) 0x69, (signed char) 0x9F, (signed char) 0x53, (signed char) 0x94,	// y0, x0, y1, x1
	(signed char) 0x53, (signed char) 0x94, (signed char) 0x53, (signed char) 0x89,	// y0, x0, y1, x1
	(signed char) 0x53, (signed char) 0x89, (signed char) 0x74, (signed char) 0x9D,	// y0, x0, y1, x1
	(signed char) 0x74, (signed char) 0x9D, (signed char) 0x74, (signed char) 0xA7,	// y0, x0, y1, x1
	(signed char) 0x74, (signed char) 0xA7, (signed char) 0x5B, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0xA2, (signed char) 0x74, (signed char) 0xB2,	// y0, x0, y1, x1
	(signed char) 0x74, (signed char) 0xB2, (signed char) 0x74, (signed char) 0xBD,	// y0, x0, y1, x1
	(signed char) 0x74, (signed char) 0xBD, (signed char) 0x5B, (signed char) 0xB2,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0xB2, (signed char) 0x5B, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0x0D, (signed char) 0x74, (signed char) 0x18,	// y0, x0, y1, x1
	(signed char) 0x74, (signed char) 0x18, (signed char) 0x74, (signed char) 0x20,	// y0, x0, y1, x1
	(signed char) 0x74, (signed char) 0x20, (signed char) 0x66, (signed char) 0x1A,	// y0, x0, y1, x1
	(signed char) 0x66, (signed char) 0x1A, (signed char) 0x66, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0x66, (signed char) 0x22, (signed char) 0x74, (signed char) 0x27,	// y0, x0, y1, x1
	(signed char) 0x74, (signed char) 0x27, (signed char) 0x74, (signed char) 0x30,	// y0, x0, y1, x1
	(signed char) 0x74, (signed char) 0x30, (signed char) 0x5B, (signed char) 0x25,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0x25, (signed char) 0x5B, (signed char) 0x77,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0xC0, (signed char) 0x61, (signed char) 0xC3,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0xC3, (signed char) 0x69, (signed char) 0xC8,	// y0, x0, y1, x1
	(signed char) 0x69, (signed char) 0xC8, (signed char) 0x5B, (signed char) 0xBB,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0xBB, (signed char) 0x5B, (signed char) 0xB5,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0xB5, (signed char) 0x6E, (signed char) 0xC5,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0xC5, (signed char) 0x6E, (signed char) 0xD0,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0xD0, (signed char) 0x5B, (signed char) 0xC8,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0xC8, (signed char) 0x5B, (signed char) 0xC0,	// y0, x0, y1, x1
	(signed char) 0x5E, (signed char) 0xCE, (signed char) 0x66, (signed char) 0xD0,	// y0, x0, y1, x1
	(signed char) 0x66, (signed char) 0xD0, (signed char) 0x66, (signed char) 0xD5,	// y0, x0, y1, x1
	(signed char) 0x66, (signed char) 0xD5, (signed char) 0x61, (signed char) 0xD3,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0xD3, (signed char) 0x61, (signed char) 0xD9,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0xD9, (signed char) 0x6E, (signed char) 0xDE,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0xDE, (signed char) 0x6E, (signed char) 0xE3,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0xE3, (signed char) 0x5B, (signed char) 0xDB,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0xDB, (signed char) 0x5B, (signed char) 0xD0,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0xD0, (signed char) 0x5E, (signed char) 0xCE,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0xE6, (signed char) 0x6E, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0xF6, (signed char) 0x5B, (signed char) 0xEE,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0xEE, (signed char) 0x5B, (signed char) 0xDE,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0xDE, (signed char) 0x6E, (signed char) 0xE6,	// y0, x0, y1, x1
	(signed char) 0x69, (signed char) 0xE8, (signed char) 0x69, (signed char) 0xEE,	// y0, x0, y1, x1
	(signed char) 0x69, (signed char) 0xEE, (signed char) 0x61, (signed char) 0xE8,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0xE8, (signed char) 0x61, (signed char) 0xE6,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0xE6, (signed char) 0x69, (signed char) 0xE8,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0xF9, (signed char) 0x5B, (signed char) 0xF1,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0xF1, (signed char) 0x6E, (signed char) 0xF9,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0xF9, (signed char) 0x6E, (signed char) 0x04,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0x04, (signed char) 0x6C, (signed char) 0x07,	// y0, x0, y1, x1
	(signed char) 0x6C, (signed char) 0x07, (signed char) 0x66, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0x66, (signed char) 0x00, (signed char) 0x5B, (signed char) 0x04,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0x04, (signed char) 0x5B, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0x00, (signed char) 0x66, (signed char) 0xFE,	// y0, x0, y1, x1
	(signed char) 0x66, (signed char) 0xFE, (signed char) 0x69, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0x69, (signed char) 0x00, (signed char) 0x69, (signed char) 0xFE,	// y0, x0, y1, x1
	(signed char) 0x69, (signed char) 0xFE, (signed char) 0x5B, (signed char) 0xF9,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0x36, (signed char) 0x61, (signed char) 0x36,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0x36, (signed char) 0x69, (signed char) 0x3B,	// y0, x0, y1, x1
	(signed char) 0x69, (signed char) 0x3B, (signed char) 0x5B, (signed char) 0x30,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0x30, (signed char) 0x5B, (signed char) 0x2B,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0x2B, (signed char) 0x6E, (signed char) 0x38,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0x38, (signed char) 0x6E, (signed char) 0x43,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0x43, (signed char) 0x5B, (signed char) 0x3D,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0x3D, (signed char) 0x5B, (signed char) 0x32,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0x45, (signed char) 0x6E, (signed char) 0x4B,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0x4B, (signed char) 0x61, (signed char) 0x48,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0x48, (signed char) 0x71, (signed char) 0x53,	// y0, x0, y1, x1
	(signed char) 0x71, (signed char) 0x53, (signed char) 0x71, (signed char) 0x59,	// y0, x0, y1, x1
	(signed char) 0x71, (signed char) 0x59, (signed char) 0x5B, (signed char) 0x4B,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0x4B, (signed char) 0x5B, (signed char) 0x3D,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0x3D, (signed char) 0x6E, (signed char) 0x45,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0x59, (signed char) 0x6E, (signed char) 0x69,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0x69, (signed char) 0x5B, (signed char) 0x61,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0x61, (signed char) 0x5B, (signed char) 0x50,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0x50, (signed char) 0x6E, (signed char) 0x59,	// y0, x0, y1, x1
	(signed char) 0x69, (signed char) 0x5E, (signed char) 0x69, (signed char) 0x61,	// y0, x0, y1, x1
	(signed char) 0x69, (signed char) 0x61, (signed char) 0x61, (signed char) 0x5E,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0x5E, (signed char) 0x61, (signed char) 0x59,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0x59, (signed char) 0x69, (signed char) 0x5E,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0x63, (signed char) 0x6E, (signed char) 0x6C,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0x6C, (signed char) 0x6E, (signed char) 0x7C,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0x7C, (signed char) 0x66, (signed char) 0x79,	// y0, x0, y1, x1
	(signed char) 0x66, (signed char) 0x79, (signed char) 0x66, (signed char) 0x71,	// y0, x0, y1, x1
	(signed char) 0x66, (signed char) 0x71, (signed char) 0x69, (signed char) 0x74,	// y0, x0, y1, x1
	(signed char) 0x69, (signed char) 0x74, (signed char) 0x69, (signed char) 0x6E,	// y0, x0, y1, x1
	(signed char) 0x69, (signed char) 0x6E, (signed char) 0x61, (signed char) 0x6C,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0x6C, (signed char) 0x61, (signed char) 0x6E,	// y0, x0, y1, x1
	(signed char) 0x61, (signed char) 0x6E, (signed char) 0x63, (signed char) 0x71,	// y0, x0, y1, x1
	(signed char) 0x63, (signed char) 0x71, (signed char) 0x63, (signed char) 0x77,	// y0, x0, y1, x1
	(signed char) 0x63, (signed char) 0x77, (signed char) 0x5B, (signed char) 0x74,	// y0, x0, y1, x1
	(signed char) 0x5B, (signed char) 0x74, (signed char) 0x5B, (signed char) 0x63,	// y0, x0, y1, x1
	(signed char) 0x53, (signed char) 0x8F, (signed char) 0x6E, (signed char) 0x9F,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0x9F, (signed char) 0x6E, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0xA2, (signed char) 0x56, (signed char) 0x9F,	// y0, x0, y1, x1
	(signed char) 0x56, (signed char) 0x9F, (signed char) 0x56, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0x56, (signed char) 0xA2, (signed char) 0x6E, (signed char) 0xB2,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0xB2, (signed char) 0x6E, (signed char) 0xB8,	// y0, x0, y1, x1
	(signed char) 0x6E, (signed char) 0xB8, (signed char) 0x56, (signed char) 0xB0,	// y0, x0, y1, x1
	(signed char) 0x56, (signed char) 0xB0, (signed char) 0x56, (signed char) 0x0F,	// y0, x0, y1, x1
	(signed char) 0x56, (signed char) 0x0F, (signed char) 0x74, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0x63, (signed char) 0x15, (signed char) 0x63, (signed char) 0x25,	// y0, x0, y1, x1
	(signed char) 0x74, (signed char) 0x2D, (signed char) 0x56, (signed char) 0x20,	// y0, x0, y1, x1
	(signed char) 0x56, (signed char) 0x20, (signed char) 0x56, (signed char) 0x77,	// y0, x0, y1, x1
};




const signed char HavocText[]={
//720, // count of vectors
	(signed char) 0x36, (signed char) 0xCA, (signed char) 0x3B, (signed char) 0xCC,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0xCC, (signed char) 0x3E, (signed char) 0xCA,	// y0, x0, y1, x1
	(signed char) 0x3E, (signed char) 0xCA, (signed char) 0x3E, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0x3E, (signed char) 0xC4, (signed char) 0x3B, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0xC4, (signed char) 0x36, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0xC4, (signed char) 0x36, (signed char) 0xCA,	// y0, x0, y1, x1
	(signed char) 0x39, (signed char) 0xCA, (signed char) 0x39, (signed char) 0xC7,	// y0, x0, y1, x1
	(signed char) 0x39, (signed char) 0xC7, (signed char) 0x3B, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0xC4, (signed char) 0x3B, (signed char) 0xC7,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0xC7, (signed char) 0x3B, (signed char) 0xCA,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0xCC, (signed char) 0x3B, (signed char) 0xCC,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0xCC, (signed char) 0x39, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0x39, (signed char) 0xCF, (signed char) 0x3B, (signed char) 0xD2,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0xD2, (signed char) 0x36, (signed char) 0xD2,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0xDA, (signed char) 0x36, (signed char) 0xD7,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0xD7, (signed char) 0x39, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0x39, (signed char) 0xD4, (signed char) 0x3B, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0xD4, (signed char) 0x3B, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0xDA, (signed char) 0x3B, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0xDA, (signed char) 0x39, (signed char) 0xDC,	// y0, x0, y1, x1
	(signed char) 0x39, (signed char) 0xDC, (signed char) 0x3B, (signed char) 0xDF,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0xDF, (signed char) 0x36, (signed char) 0xDF,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0xE2, (signed char) 0x3B, (signed char) 0xE2,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0xE2, (signed char) 0x36, (signed char) 0xE7,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0xE7, (signed char) 0x3B, (signed char) 0xEC,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0xE7, (signed char) 0x36, (signed char) 0xEC,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0xEF, (signed char) 0x3B, (signed char) 0xF2,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0xEF, (signed char) 0x36, (signed char) 0xF2,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0xF4, (signed char) 0x3B, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0xF4, (signed char) 0x36, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0xF8, (signed char) 0x36, (signed char) 0xFA,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0xF8, (signed char) 0x3B, (signed char) 0xF8,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0xF8, (signed char) 0x3B, (signed char) 0xFA,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0xFD, (signed char) 0x36, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0x00, (signed char) 0x3B, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0xFD, (signed char) 0x3B, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0x05, (signed char) 0x36, (signed char) 0x08,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0x05, (signed char) 0x3B, (signed char) 0x05,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0x05, (signed char) 0x3B, (signed char) 0x08,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0x10, (signed char) 0x39, (signed char) 0x10,	// y0, x0, y1, x1
	(signed char) 0x39, (signed char) 0x10, (signed char) 0x3B, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0x12, (signed char) 0x3B, (signed char) 0x15,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0x15, (signed char) 0x36, (signed char) 0x15,	// y0, x0, y1, x1
	(signed char) 0x39, (signed char) 0x10, (signed char) 0x39, (signed char) 0x15,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0x1A, (signed char) 0x3B, (signed char) 0x1A,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0x18, (signed char) 0x3B, (signed char) 0x1A,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0x1D, (signed char) 0x39, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0x39, (signed char) 0x1D, (signed char) 0x3B, (signed char) 0x20,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0x20, (signed char) 0x3B, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0x22, (signed char) 0x36, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0x39, (signed char) 0x1D, (signed char) 0x39, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0x28, (signed char) 0x39, (signed char) 0x28,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0x25, (signed char) 0x3B, (signed char) 0x25,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0x25, (signed char) 0x3B, (signed char) 0x28,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0x28, (signed char) 0x3B, (signed char) 0x28,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0x28, (signed char) 0x39, (signed char) 0x28,	// y0, x0, y1, x1
	(signed char) 0x39, (signed char) 0x28, (signed char) 0x39, (signed char) 0x25,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0x2A, (signed char) 0x36, (signed char) 0x30,	// y0, x0, y1, x1
	(signed char) 0x36, (signed char) 0x2D, (signed char) 0x3B, (signed char) 0x2D,	// y0, x0, y1, x1
	(signed char) 0x3B, (signed char) 0x2A, (signed char) 0x3B, (signed char) 0x30,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0x8A, (signed char) 0x13, (signed char) 0x8D,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0x8D, (signed char) 0x11, (signed char) 0x8D,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0x8D, (signed char) 0x11, (signed char) 0x8A,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0x8A, (signed char) 0x0E, (signed char) 0x8A,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x8A, (signed char) 0x0E, (signed char) 0x8D,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x90, (signed char) 0x0E, (signed char) 0x90,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x90, (signed char) 0x0E, (signed char) 0x90,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x98, (signed char) 0x13, (signed char) 0x98,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0x98, (signed char) 0x13, (signed char) 0x9A,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0x9A, (signed char) 0x11, (signed char) 0x9D,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0x9D, (signed char) 0x0E, (signed char) 0x9A,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x9A, (signed char) 0x0E, (signed char) 0x98,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x9D, (signed char) 0x0E, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x9D, (signed char) 0x13, (signed char) 0x9D,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0x9D, (signed char) 0x13, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xA0, (signed char) 0x11, (signed char) 0x9D,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xA2, (signed char) 0x0E, (signed char) 0xA4,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xA4, (signed char) 0x0E, (signed char) 0xA7,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xA7, (signed char) 0x11, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xA2, (signed char) 0x13, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xA2, (signed char) 0x13, (signed char) 0xA4,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xAA, (signed char) 0x13, (signed char) 0xAA,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xA7, (signed char) 0x13, (signed char) 0xAA,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xAF, (signed char) 0x11, (signed char) 0xAF,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xAC, (signed char) 0x13, (signed char) 0xAC,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xAC, (signed char) 0x13, (signed char) 0xAF,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xAF, (signed char) 0x11, (signed char) 0xAF,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xAF, (signed char) 0x11, (signed char) 0xAF,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xAF, (signed char) 0x11, (signed char) 0xAC,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xB4, (signed char) 0x11, (signed char) 0xB4,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xB4, (signed char) 0x13, (signed char) 0xB4,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xB4, (signed char) 0x13, (signed char) 0xB2,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xB2, (signed char) 0x11, (signed char) 0xB2,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xB2, (signed char) 0x0E, (signed char) 0xB2,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xB2, (signed char) 0x0E, (signed char) 0xB4,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xB7, (signed char) 0x11, (signed char) 0xBA,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xBA, (signed char) 0x0E, (signed char) 0xB7,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xC2, (signed char) 0x13, (signed char) 0xC2,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xC2, (signed char) 0x11, (signed char) 0xC2,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xC2, (signed char) 0x13, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xC4, (signed char) 0x0E, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xCA, (signed char) 0x11, (signed char) 0xCA,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xCA, (signed char) 0x13, (signed char) 0xCA,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xCA, (signed char) 0x13, (signed char) 0xC7,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xC7, (signed char) 0x11, (signed char) 0xC7,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xC7, (signed char) 0x0E, (signed char) 0xC7,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xC7, (signed char) 0x0E, (signed char) 0xCA,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xCC, (signed char) 0x13, (signed char) 0xCC,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xCA, (signed char) 0x13, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xCF, (signed char) 0x13, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xCF, (signed char) 0x11, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xD4, (signed char) 0x0E, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xD4, (signed char) 0x0E, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xD4, (signed char) 0x13, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xD4, (signed char) 0x13, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xD7, (signed char) 0x11, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xDF, (signed char) 0x11, (signed char) 0xDC,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xDA, (signed char) 0x13, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xDA, (signed char) 0x13, (signed char) 0xDC,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xDC, (signed char) 0x11, (signed char) 0xDF,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xDF, (signed char) 0x11, (signed char) 0xDC,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xDC, (signed char) 0x11, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xDF, (signed char) 0x0E, (signed char) 0xE2,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xE2, (signed char) 0x0E, (signed char) 0xE4,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xE4, (signed char) 0x11, (signed char) 0xDF,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xDF, (signed char) 0x13, (signed char) 0xE2,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xE2, (signed char) 0x13, (signed char) 0xE2,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xE4, (signed char) 0x13, (signed char) 0xE4,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xE4, (signed char) 0x11, (signed char) 0xEA,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xEA, (signed char) 0x0E, (signed char) 0xEA,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xEA, (signed char) 0x0E, (signed char) 0xEC,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xEC, (signed char) 0x13, (signed char) 0xEC,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xEA, (signed char) 0x13, (signed char) 0xEC,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xEF, (signed char) 0x13, (signed char) 0xEF,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xEF, (signed char) 0x13, (signed char) 0xF2,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xF2, (signed char) 0x11, (signed char) 0xF2,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xF2, (signed char) 0x11, (signed char) 0xF2,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xF2, (signed char) 0x11, (signed char) 0xEF,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xF6, (signed char) 0x13, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xF6, (signed char) 0x13, (signed char) 0xF8,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xF8, (signed char) 0x11, (signed char) 0xF8,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xF8, (signed char) 0x0E, (signed char) 0xF8,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xF8, (signed char) 0x0E, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xFA, (signed char) 0x0E, (signed char) 0xFD,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0xFA, (signed char) 0x13, (signed char) 0xFA,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0xFA, (signed char) 0x13, (signed char) 0xFD,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0xFD, (signed char) 0x11, (signed char) 0xFA,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x00, (signed char) 0x13, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0x00, (signed char) 0x13, (signed char) 0x02,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0x02, (signed char) 0x11, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x05, (signed char) 0x0E, (signed char) 0x08,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x05, (signed char) 0x13, (signed char) 0x05,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0x05, (signed char) 0x13, (signed char) 0x08,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0x08, (signed char) 0x11, (signed char) 0x05,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x0A, (signed char) 0x13, (signed char) 0x0A,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0x0A, (signed char) 0x0E, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x0D, (signed char) 0x13, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x0D, (signed char) 0x0E, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x12, (signed char) 0x0E, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x12, (signed char) 0x11, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0x0D, (signed char) 0x13, (signed char) 0x10,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0x10, (signed char) 0x13, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x12, (signed char) 0x0E, (signed char) 0x18,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x12, (signed char) 0x13, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0x12, (signed char) 0x13, (signed char) 0x18,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0x15, (signed char) 0x11, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x18, (signed char) 0x0E, (signed char) 0x1A,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x1A, (signed char) 0x0E, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0x0E, (signed char) 0x1D, (signed char) 0x11, (signed char) 0x18,	// y0, x0, y1, x1
	(signed char) 0x11, (signed char) 0x18, (signed char) 0x13, (signed char) 0x1A,	// y0, x0, y1, x1
	(signed char) 0x13, (signed char) 0x1A, (signed char) 0x13, (signed char) 0x1A,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x8A, (signed char) 0x01, (signed char) 0x8D,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x8A, (signed char) 0x03, (signed char) 0x8D,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x8D, (signed char) 0xFE, (signed char) 0x8D,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x8D, (signed char) 0xFE, (signed char) 0x8A,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x90, (signed char) 0xFE, (signed char) 0x90,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x90, (signed char) 0xFE, (signed char) 0x90,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x98, (signed char) 0x03, (signed char) 0x98,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x98, (signed char) 0xFE, (signed char) 0x9D,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x9D, (signed char) 0x01, (signed char) 0x9D,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x9D, (signed char) 0x03, (signed char) 0xA0,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xA0, (signed char) 0x03, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xA2, (signed char) 0xFE, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x9D, (signed char) 0x01, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xA2, (signed char) 0x03, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xA2, (signed char) 0xFE, (signed char) 0xA7,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xA7, (signed char) 0x03, (signed char) 0xA7,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xA7, (signed char) 0x03, (signed char) 0xA7,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xA7, (signed char) 0x03, (signed char) 0xAA,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xAA, (signed char) 0x01, (signed char) 0xAA,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0xAA, (signed char) 0xFE, (signed char) 0xAA,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xAA, (signed char) 0xFE, (signed char) 0xA7,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xB4, (signed char) 0x01, (signed char) 0xB4,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0xB4, (signed char) 0x03, (signed char) 0xB4,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xB4, (signed char) 0x03, (signed char) 0xB2,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xB2, (signed char) 0x01, (signed char) 0xB2,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0xB2, (signed char) 0xFE, (signed char) 0xB2,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xB2, (signed char) 0xFE, (signed char) 0xB4,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xB7, (signed char) 0x03, (signed char) 0xB7,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xB7, (signed char) 0xFE, (signed char) 0xBA,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xBA, (signed char) 0x03, (signed char) 0xBA,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xC2, (signed char) 0xFE, (signed char) 0xC2,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xC2, (signed char) 0x03, (signed char) 0xC2,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xC2, (signed char) 0xFE, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xC4, (signed char) 0x03, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xC4, (signed char) 0x03, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0xC4, (signed char) 0x01, (signed char) 0xCA,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xCA, (signed char) 0xFE, (signed char) 0xCA,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xCC, (signed char) 0xFE, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xCC, (signed char) 0x03, (signed char) 0xCC,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xCC, (signed char) 0x03, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xD2, (signed char) 0x03, (signed char) 0xD2,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xCF, (signed char) 0x03, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xD4, (signed char) 0xFE, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xD4, (signed char) 0x03, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xD4, (signed char) 0x03, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0xD7, (signed char) 0x01, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xDF, (signed char) 0x03, (signed char) 0xDF,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xDF, (signed char) 0x03, (signed char) 0xE2,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xE2, (signed char) 0x01, (signed char) 0xE2,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0xE2, (signed char) 0x01, (signed char) 0xE2,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0xE2, (signed char) 0x01, (signed char) 0xDF,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xE4, (signed char) 0x03, (signed char) 0xE4,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xE4, (signed char) 0xFE, (signed char) 0xE7,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xEA, (signed char) 0x01, (signed char) 0xEA,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0xEA, (signed char) 0x03, (signed char) 0xEC,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xEC, (signed char) 0x03, (signed char) 0xEC,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xEC, (signed char) 0xFE, (signed char) 0xEC,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0xEA, (signed char) 0x01, (signed char) 0xEC,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xEF, (signed char) 0x03, (signed char) 0xEF,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xEF, (signed char) 0x03, (signed char) 0xF2,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xF4, (signed char) 0x03, (signed char) 0xF4,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xF4, (signed char) 0x03, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0xF6, (signed char) 0x01, (signed char) 0xF4,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xF8, (signed char) 0x01, (signed char) 0xF8,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0xF8, (signed char) 0x03, (signed char) 0xF8,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xF8, (signed char) 0x03, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xF6, (signed char) 0x01, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0xF6, (signed char) 0xFE, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xF6, (signed char) 0xFE, (signed char) 0xF8,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xFD, (signed char) 0x01, (signed char) 0xFD,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0xFA, (signed char) 0x03, (signed char) 0xFA,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xFA, (signed char) 0x03, (signed char) 0xFD,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0xFD, (signed char) 0x01, (signed char) 0xFD,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0xFD, (signed char) 0x01, (signed char) 0xFD,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0xFD, (signed char) 0x01, (signed char) 0xFA,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x00, (signed char) 0x03, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x00, (signed char) 0x01, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x00, (signed char) 0x03, (signed char) 0x02,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x02, (signed char) 0xFE, (signed char) 0x02,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x0D, (signed char) 0x01, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x0D, (signed char) 0x03, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x0D, (signed char) 0x03, (signed char) 0x0A,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x0A, (signed char) 0x01, (signed char) 0x08,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x08, (signed char) 0xFE, (signed char) 0x0A,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x0A, (signed char) 0xFE, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x0D, (signed char) 0x03, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x0D, (signed char) 0x03, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x10, (signed char) 0x01, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x18, (signed char) 0x03, (signed char) 0x18,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x18, (signed char) 0x03, (signed char) 0x1A,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x1A, (signed char) 0x01, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x1D, (signed char) 0xFE, (signed char) 0x1A,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x1A, (signed char) 0xFE, (signed char) 0x18,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x1D, (signed char) 0xFE, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x1D, (signed char) 0x03, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x1D, (signed char) 0x03, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x20, (signed char) 0x01, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x22, (signed char) 0x03, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x22, (signed char) 0x03, (signed char) 0x25,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x25, (signed char) 0x01, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x28, (signed char) 0xFE, (signed char) 0x2A,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x28, (signed char) 0x03, (signed char) 0x28,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x28, (signed char) 0x03, (signed char) 0x2A,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x2A, (signed char) 0x01, (signed char) 0x28,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x2D, (signed char) 0x01, (signed char) 0x2D,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x2D, (signed char) 0x03, (signed char) 0x30,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x30, (signed char) 0x03, (signed char) 0x30,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x30, (signed char) 0xFE, (signed char) 0x30,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x2D, (signed char) 0x01, (signed char) 0x30,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x32, (signed char) 0x03, (signed char) 0x32,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x32, (signed char) 0x03, (signed char) 0x35,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x38, (signed char) 0xFE, (signed char) 0x3A,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x38, (signed char) 0x03, (signed char) 0x38,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x38, (signed char) 0x03, (signed char) 0x3A,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x3A, (signed char) 0x01, (signed char) 0x38,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x3D, (signed char) 0x03, (signed char) 0x3D,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x3D, (signed char) 0x03, (signed char) 0x40,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x40, (signed char) 0x01, (signed char) 0x40,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x40, (signed char) 0xFE, (signed char) 0x40,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x40, (signed char) 0xFE, (signed char) 0x3D,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x45, (signed char) 0x03, (signed char) 0x45,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x45, (signed char) 0x01, (signed char) 0x48,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x48, (signed char) 0x03, (signed char) 0x4A,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x4A, (signed char) 0xFE, (signed char) 0x4A,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x4C, (signed char) 0x01, (signed char) 0x4F,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x4F, (signed char) 0x03, (signed char) 0x4C,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x4C, (signed char) 0x03, (signed char) 0x4C,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x4C, (signed char) 0x01, (signed char) 0x4A,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x4A, (signed char) 0xFE, (signed char) 0x4C,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x4C, (signed char) 0xFE, (signed char) 0x4C,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x52, (signed char) 0x03, (signed char) 0x52,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x4F, (signed char) 0x03, (signed char) 0x54,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x54, (signed char) 0x03, (signed char) 0x54,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x54, (signed char) 0x01, (signed char) 0x5A,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x5A, (signed char) 0xFE, (signed char) 0x5A,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x5A, (signed char) 0xFE, (signed char) 0x5C,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x5A, (signed char) 0x03, (signed char) 0x5A,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x5A, (signed char) 0x03, (signed char) 0x5C,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x5C, (signed char) 0x01, (signed char) 0x5A,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x62, (signed char) 0x01, (signed char) 0x62,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x5F, (signed char) 0x03, (signed char) 0x5F,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x5F, (signed char) 0x03, (signed char) 0x62,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x62, (signed char) 0x01, (signed char) 0x62,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x62, (signed char) 0x01, (signed char) 0x62,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x62, (signed char) 0x01, (signed char) 0x5F,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x64, (signed char) 0xFE, (signed char) 0x67,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x67, (signed char) 0xFE, (signed char) 0x67,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x67, (signed char) 0x01, (signed char) 0x64,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x64, (signed char) 0x03, (signed char) 0x64,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x64, (signed char) 0x03, (signed char) 0x67,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x6A, (signed char) 0x03, (signed char) 0x6A,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x6A, (signed char) 0x01, (signed char) 0x6C,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x6C, (signed char) 0xFE, (signed char) 0x6C,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x6F, (signed char) 0xFE, (signed char) 0x72,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x6F, (signed char) 0x03, (signed char) 0x6F,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x6F, (signed char) 0x03, (signed char) 0x72,	// y0, x0, y1, x1
	(signed char) 0xFE, (signed char) 0x74, (signed char) 0x03, (signed char) 0x74,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x74, (signed char) 0x03, (signed char) 0x77,	// y0, x0, y1, x1
	(signed char) 0x03, (signed char) 0x77, (signed char) 0x01, (signed char) 0x77,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x77, (signed char) 0x01, (signed char) 0x77,	// y0, x0, y1, x1
	(signed char) 0x01, (signed char) 0x77, (signed char) 0x01, (signed char) 0x74,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x8A, (signed char) 0xF1, (signed char) 0x8A,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0x8A, (signed char) 0xF1, (signed char) 0x8D,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x8D, (signed char) 0xEF, (signed char) 0x8D,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x90, (signed char) 0xEF, (signed char) 0x90,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x90, (signed char) 0xEF, (signed char) 0x90,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x9A, (signed char) 0xEF, (signed char) 0x9A,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x9A, (signed char) 0xF3, (signed char) 0x9A,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x9A, (signed char) 0xF3, (signed char) 0x9A,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x9D, (signed char) 0xF3, (signed char) 0x9D,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x9D, (signed char) 0xEF, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xA2, (signed char) 0xF3, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xA7, (signed char) 0xF3, (signed char) 0xA7,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xA7, (signed char) 0xF1, (signed char) 0xAA,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0xAA, (signed char) 0xF3, (signed char) 0xAA,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xAA, (signed char) 0xEF, (signed char) 0xAA,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xAC, (signed char) 0xF1, (signed char) 0xAC,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0xAC, (signed char) 0xF3, (signed char) 0xAF,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xAF, (signed char) 0xF3, (signed char) 0xAF,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xAF, (signed char) 0xEF, (signed char) 0xAF,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0xAC, (signed char) 0xF1, (signed char) 0xAF,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xB2, (signed char) 0xF3, (signed char) 0xB4,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xB4, (signed char) 0xEF, (signed char) 0xB2,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xB2, (signed char) 0xEF, (signed char) 0xB4,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xB7, (signed char) 0xEF, (signed char) 0xBA,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xB7, (signed char) 0xF3, (signed char) 0xB7,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xB7, (signed char) 0xF3, (signed char) 0xBA,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0xBA, (signed char) 0xF1, (signed char) 0xB7,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xBC, (signed char) 0xEF, (signed char) 0xBC,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xBC, (signed char) 0xEF, (signed char) 0xBC,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xBC, (signed char) 0xEF, (signed char) 0xBC,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xC4, (signed char) 0xF3, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xC4, (signed char) 0xF3, (signed char) 0xCA,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0xCA, (signed char) 0xF1, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xCC, (signed char) 0xEF, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xCC, (signed char) 0xF3, (signed char) 0xCC,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xCC, (signed char) 0xF3, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xCF, (signed char) 0xF3, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xCF, (signed char) 0xEF, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xD4, (signed char) 0xF3, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xD4, (signed char) 0xF3, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xD4, (signed char) 0xF3, (signed char) 0xD7,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xD7, (signed char) 0xF1, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0xDA, (signed char) 0xEF, (signed char) 0xD7,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xD7, (signed char) 0xEF, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xE2, (signed char) 0xF3, (signed char) 0xE2,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xDF, (signed char) 0xF3, (signed char) 0xE2,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xE4, (signed char) 0xF3, (signed char) 0xE4,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0xE4, (signed char) 0xF1, (signed char) 0xE7,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xE7, (signed char) 0xEF, (signed char) 0xE7,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xEA, (signed char) 0xEF, (signed char) 0xEC,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xEA, (signed char) 0xF3, (signed char) 0xEA,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xEA, (signed char) 0xF3, (signed char) 0xEC,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0xEC, (signed char) 0xF1, (signed char) 0xEA,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xF6, (signed char) 0xF1, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xF4, (signed char) 0xF3, (signed char) 0xF4,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xF4, (signed char) 0xF3, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xF6, (signed char) 0xF1, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0xF6, (signed char) 0xF1, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0xF6, (signed char) 0xF1, (signed char) 0xF4,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xF6, (signed char) 0xEF, (signed char) 0xF8,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xF6, (signed char) 0xF3, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xF6, (signed char) 0xF3, (signed char) 0xF8,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0xF8, (signed char) 0xF1, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0xFA, (signed char) 0xF1, (signed char) 0xFA,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0xFA, (signed char) 0xF3, (signed char) 0xFA,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xFA, (signed char) 0xF3, (signed char) 0xFD,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0xFD, (signed char) 0xEF, (signed char) 0xFD,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0xFA, (signed char) 0xF1, (signed char) 0xFD,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x02, (signed char) 0xEF, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x00, (signed char) 0xF1, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0x00, (signed char) 0xF3, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x00, (signed char) 0xF3, (signed char) 0x02,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x05, (signed char) 0xF3, (signed char) 0x05,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x05, (signed char) 0xF3, (signed char) 0x08,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x0D, (signed char) 0xF1, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0x0D, (signed char) 0xF3, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x0D, (signed char) 0xF3, (signed char) 0x0A,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x0A, (signed char) 0xF1, (signed char) 0x08,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0x08, (signed char) 0xEF, (signed char) 0x0A,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x0A, (signed char) 0xEF, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x10, (signed char) 0xEF, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x10, (signed char) 0xF3, (signed char) 0x10,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x10, (signed char) 0xF3, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x12, (signed char) 0xF3, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x12, (signed char) 0xF3, (signed char) 0x15,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x15, (signed char) 0xF1, (signed char) 0x18,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0x18, (signed char) 0xEF, (signed char) 0x15,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x15, (signed char) 0xEF, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x1D, (signed char) 0xF1, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0x1D, (signed char) 0xF3, (signed char) 0x20,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x20, (signed char) 0xF3, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x22, (signed char) 0xEF, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0x1D, (signed char) 0xF1, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x22, (signed char) 0xF3, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x22, (signed char) 0xEF, (signed char) 0x25,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x25, (signed char) 0xF3, (signed char) 0x25,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x28, (signed char) 0xF3, (signed char) 0x28,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x28, (signed char) 0xF3, (signed char) 0x2A,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x2A, (signed char) 0xF1, (signed char) 0x2A,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0x2A, (signed char) 0xEF, (signed char) 0x2A,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x2A, (signed char) 0xEF, (signed char) 0x28,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x32, (signed char) 0xF3, (signed char) 0x32,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x32, (signed char) 0xF3, (signed char) 0x35,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x3A, (signed char) 0xF1, (signed char) 0x3A,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0x3A, (signed char) 0xF3, (signed char) 0x3A,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x3A, (signed char) 0xF3, (signed char) 0x38,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x38, (signed char) 0xF1, (signed char) 0x38,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0x38, (signed char) 0xEF, (signed char) 0x38,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x38, (signed char) 0xEF, (signed char) 0x3A,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x3D, (signed char) 0xF3, (signed char) 0x3D,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x40, (signed char) 0xEF, (signed char) 0x40,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x40, (signed char) 0xEF, (signed char) 0x3D,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x3D, (signed char) 0xEF, (signed char) 0x3D,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x45, (signed char) 0xEF, (signed char) 0x42,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x42, (signed char) 0xF1, (signed char) 0x40,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0x40, (signed char) 0xF3, (signed char) 0x42,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x42, (signed char) 0xF3, (signed char) 0x45,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x45, (signed char) 0xF3, (signed char) 0x45,	// y0, x0, y1, x1
	(signed char) 0xF1, (signed char) 0x45, (signed char) 0xF1, (signed char) 0x4A,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x4A, (signed char) 0xEF, (signed char) 0x4A,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x4F, (signed char) 0xEF, (signed char) 0x52,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x52, (signed char) 0xF3, (signed char) 0x52,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x4F, (signed char) 0xF3, (signed char) 0x52,	// y0, x0, y1, x1
	(signed char) 0xEF, (signed char) 0x57, (signed char) 0xF3, (signed char) 0x57,	// y0, x0, y1, x1
	(signed char) 0xF3, (signed char) 0x54, (signed char) 0xF3, (signed char) 0x5A,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x8A, (signed char) 0xE3, (signed char) 0x8D,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x8D, (signed char) 0xE5, (signed char) 0x8D,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x8D, (signed char) 0xE5, (signed char) 0x8A,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x8A, (signed char) 0xE5, (signed char) 0x8A,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x8A, (signed char) 0xE5, (signed char) 0x8D,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x90, (signed char) 0xE3, (signed char) 0x90,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x90, (signed char) 0xE3, (signed char) 0x90,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x98, (signed char) 0xE3, (signed char) 0x9D,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x98, (signed char) 0xE5, (signed char) 0x98,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x98, (signed char) 0xE5, (signed char) 0x9D,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x9A, (signed char) 0xE5, (signed char) 0x98,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x9D, (signed char) 0xE5, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x9D, (signed char) 0xE3, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xA2, (signed char) 0xE3, (signed char) 0xA4,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xA4, (signed char) 0xE5, (signed char) 0xA4,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xA2, (signed char) 0xE5, (signed char) 0xA4,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xAA, (signed char) 0xE5, (signed char) 0xAA,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xA7, (signed char) 0xE5, (signed char) 0xAA,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xB2, (signed char) 0xE5, (signed char) 0xB2,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xB2, (signed char) 0xE5, (signed char) 0xB4,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xB4, (signed char) 0xE5, (signed char) 0xB4,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xB4, (signed char) 0xE3, (signed char) 0xB4,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xB7, (signed char) 0xE5, (signed char) 0xB7,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xB7, (signed char) 0xE5, (signed char) 0xB7,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xB7, (signed char) 0xE5, (signed char) 0xBA,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xBA, (signed char) 0xE3, (signed char) 0xBA,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xB7, (signed char) 0xE5, (signed char) 0xBA,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xBC, (signed char) 0xE5, (signed char) 0xBF,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xBF, (signed char) 0xE3, (signed char) 0xBC,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xBC, (signed char) 0xE3, (signed char) 0xBF,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xC2, (signed char) 0xE3, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xC2, (signed char) 0xE5, (signed char) 0xC2,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xC2, (signed char) 0xE5, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xC4, (signed char) 0xE5, (signed char) 0xC2,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xCA, (signed char) 0xE5, (signed char) 0xCA,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xCA, (signed char) 0xE5, (signed char) 0xCC,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xCC, (signed char) 0xE5, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xCF, (signed char) 0xE3, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xCF, (signed char) 0xE3, (signed char) 0xCA,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xCA, (signed char) 0xE5, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xCF, (signed char) 0xE3, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xCF, (signed char) 0xE5, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xCF, (signed char) 0xE5, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xD2, (signed char) 0xE5, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xD4, (signed char) 0xE5, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xD4, (signed char) 0xE5, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xD7, (signed char) 0xE5, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xDC, (signed char) 0xE5, (signed char) 0xDF,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xDF, (signed char) 0xE5, (signed char) 0xDC,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xDC, (signed char) 0xE5, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xDA, (signed char) 0xE5, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xDA, (signed char) 0xE3, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xDA, (signed char) 0xE3, (signed char) 0xDC,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xE2, (signed char) 0xE5, (signed char) 0xE2,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xDF, (signed char) 0xE5, (signed char) 0xDF,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xDF, (signed char) 0xE5, (signed char) 0xE2,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xE2, (signed char) 0xE5, (signed char) 0xE4,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xE4, (signed char) 0xE5, (signed char) 0xE2,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xE2, (signed char) 0xE5, (signed char) 0xDF,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xE4, (signed char) 0xE3, (signed char) 0xE7,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xE4, (signed char) 0xE5, (signed char) 0xE4,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xE4, (signed char) 0xE5, (signed char) 0xE7,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xE7, (signed char) 0xE5, (signed char) 0xE4,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xF2, (signed char) 0xE5, (signed char) 0xF2,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xEF, (signed char) 0xE5, (signed char) 0xEF,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xEF, (signed char) 0xE5, (signed char) 0xF2,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xF2, (signed char) 0xE5, (signed char) 0xF2,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xF2, (signed char) 0xE5, (signed char) 0xF2,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xF2, (signed char) 0xE5, (signed char) 0xEF,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xF4, (signed char) 0xE3, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xF4, (signed char) 0xE5, (signed char) 0xF4,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xF4, (signed char) 0xE5, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xF6, (signed char) 0xE5, (signed char) 0xF4,	// y0, x0, y1, x140
	(signed char) 0xE3, (signed char) 0xF6, (signed char) 0xE5, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xF6, (signed char) 0xE5, (signed char) 0xF8,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xF8, (signed char) 0xE5, (signed char) 0xF8,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xF8, (signed char) 0xE3, (signed char) 0xF8,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xF6, (signed char) 0xE5, (signed char) 0xF8,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xFD, (signed char) 0xE3, (signed char) 0xFD,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0xFD, (signed char) 0xE5, (signed char) 0xFA,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xFA, (signed char) 0xE5, (signed char) 0xFA,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0xFA, (signed char) 0xE5, (signed char) 0xFD,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x00, (signed char) 0xE5, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x00, (signed char) 0xE5, (signed char) 0x02,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x08, (signed char) 0xE5, (signed char) 0x08,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x08, (signed char) 0xE5, (signed char) 0x08,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x08, (signed char) 0xE5, (signed char) 0x05,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x05, (signed char) 0xE5, (signed char) 0x05,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x05, (signed char) 0xE3, (signed char) 0x05,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x05, (signed char) 0xE3, (signed char) 0x08,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x0A, (signed char) 0xE3, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x0A, (signed char) 0xE5, (signed char) 0x0A,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x0A, (signed char) 0xE5, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x0D, (signed char) 0xE5, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x0D, (signed char) 0xE5, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x12, (signed char) 0xE5, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x12, (signed char) 0xE3, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x12, (signed char) 0xE3, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x18, (signed char) 0xE5, (signed char) 0x18,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x18, (signed char) 0xE5, (signed char) 0x1A,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x1A, (signed char) 0xE5, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x1D, (signed char) 0xE3, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x1D, (signed char) 0xE3, (signed char) 0x18,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x18, (signed char) 0xE5, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x1D, (signed char) 0xE5, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x1D, (signed char) 0xE3, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x25, (signed char) 0xE5, (signed char) 0x28,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x28, (signed char) 0xE5, (signed char) 0x25,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x25, (signed char) 0xE5, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x22, (signed char) 0xE5, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x22, (signed char) 0xE3, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x22, (signed char) 0xE3, (signed char) 0x25,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x28, (signed char) 0xE3, (signed char) 0x2A,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x2A, (signed char) 0xE5, (signed char) 0x2A,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x2A, (signed char) 0xE3, (signed char) 0x2A,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x2A, (signed char) 0xE5, (signed char) 0x2A,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x2D, (signed char) 0xE3, (signed char) 0x30,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x30, (signed char) 0xE3, (signed char) 0x30,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x30, (signed char) 0xE5, (signed char) 0x2D,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x2D, (signed char) 0xE5, (signed char) 0x2D,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x2D, (signed char) 0xE5, (signed char) 0x30,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x38, (signed char) 0xE5, (signed char) 0x38,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x3A, (signed char) 0xE3, (signed char) 0x3A,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x3A, (signed char) 0xE3, (signed char) 0x38,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x38, (signed char) 0xE3, (signed char) 0x38,	// y0, x0, y1, x1
	(signed char) 0xE3, (signed char) 0x3D, (signed char) 0xE5, (signed char) 0x3D,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x3D, (signed char) 0xE5, (signed char) 0x40,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x40, (signed char) 0xE5, (signed char) 0x40,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x40, (signed char) 0xE5, (signed char) 0x40,	// y0, x0, y1, x1
	(signed char) 0xE5, (signed char) 0x40, (signed char) 0xE5, (signed char) 0x3D,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x8A, (signed char) 0xC5, (signed char) 0x8A,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x8A, (signed char) 0xC3, (signed char) 0x8D,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x8D, (signed char) 0xC5, (signed char) 0x8D,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x92, (signed char) 0xC5, (signed char) 0x92,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x92, (signed char) 0xC5, (signed char) 0x92,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x92, (signed char) 0xC5, (signed char) 0x90,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x90, (signed char) 0xC5, (signed char) 0x8D,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x8D, (signed char) 0xC3, (signed char) 0x90,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x90, (signed char) 0xC3, (signed char) 0x92,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x95, (signed char) 0xC5, (signed char) 0x95,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x92, (signed char) 0xC5, (signed char) 0x98,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x98, (signed char) 0xC3, (signed char) 0x9D,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x98, (signed char) 0xC5, (signed char) 0x98,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x98, (signed char) 0xC5, (signed char) 0x9D,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x9A, (signed char) 0xC5, (signed char) 0x98,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x9D, (signed char) 0xC5, (signed char) 0xA2,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xA2, (signed char) 0xC5, (signed char) 0xA7,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xA7, (signed char) 0xC5, (signed char) 0xA7,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xA7, (signed char) 0xC5, (signed char) 0xAA,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xAA, (signed char) 0xC3, (signed char) 0xAA,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xAF, (signed char) 0xC5, (signed char) 0xAF,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xAF, (signed char) 0xC5, (signed char) 0xAF,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xAF, (signed char) 0xC5, (signed char) 0xAC,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xAC, (signed char) 0xC5, (signed char) 0xAC,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xAC, (signed char) 0xC3, (signed char) 0xAC,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xAC, (signed char) 0xC3, (signed char) 0xAF,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xB2, (signed char) 0xC5, (signed char) 0xB2,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xB2, (signed char) 0xC3, (signed char) 0xB4,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xB7, (signed char) 0xC5, (signed char) 0xB7,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xB7, (signed char) 0xC5, (signed char) 0xBA,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xBA, (signed char) 0xC5, (signed char) 0xBA,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xBA, (signed char) 0xC3, (signed char) 0xBA,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xBA, (signed char) 0xC3, (signed char) 0xB7,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xC2, (signed char) 0xC3, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xC4, (signed char) 0xC3, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xC4, (signed char) 0xC5, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xC4, (signed char) 0xC5, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xCA, (signed char) 0xC3, (signed char) 0xCA,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xCA, (signed char) 0xC3, (signed char) 0xC7,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xC7, (signed char) 0xC3, (signed char) 0xC4,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xCA, (signed char) 0xC5, (signed char) 0xCA,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xCA, (signed char) 0xC5, (signed char) 0xCC,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xCC, (signed char) 0xC5, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xCF, (signed char) 0xC3, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xCF, (signed char) 0xC5, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xCF, (signed char) 0xC5, (signed char) 0xD2,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xD2, (signed char) 0xC5, (signed char) 0xD4,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xD4, (signed char) 0xC5, (signed char) 0xD2,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xD2, (signed char) 0xC5, (signed char) 0xCF,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xDA, (signed char) 0xC5, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xDA, (signed char) 0xC5, (signed char) 0xDC,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xDC, (signed char) 0xC5, (signed char) 0xDF,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xDF, (signed char) 0xC3, (signed char) 0xDF,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xDF, (signed char) 0xC3, (signed char) 0xDA,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xDA, (signed char) 0xC5, (signed char) 0xDF,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xDF, (signed char) 0xC5, (signed char) 0xDF,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xE2, (signed char) 0xC3, (signed char) 0xE2,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xE2, (signed char) 0xC3, (signed char) 0xE2,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xE2, (signed char) 0xC3, (signed char) 0xDF,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xE7, (signed char) 0xC5, (signed char) 0xE7,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xE4, (signed char) 0xC5, (signed char) 0xE7,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xEC, (signed char) 0xC5, (signed char) 0xEC,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xEA, (signed char) 0xC5, (signed char) 0xEC,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xF2, (signed char) 0xC5, (signed char) 0xF2,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xF2, (signed char) 0xC5, (signed char) 0xF2,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xF2, (signed char) 0xC5, (signed char) 0xEF,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xEF, (signed char) 0xC5, (signed char) 0xEF,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xEF, (signed char) 0xC3, (signed char) 0xEF,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xEF, (signed char) 0xC3, (signed char) 0xF2,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xF4, (signed char) 0xC5, (signed char) 0xF4,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xF4, (signed char) 0xC3, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xF6, (signed char) 0xC5, (signed char) 0xF6,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0xFA, (signed char) 0xC5, (signed char) 0xFA,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xFA, (signed char) 0xC5, (signed char) 0xFD,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0xFD, (signed char) 0xC5, (signed char) 0xFA,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x02, (signed char) 0xC5, (signed char) 0x02,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x02, (signed char) 0xC5, (signed char) 0x02,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x02, (signed char) 0xC5, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x00, (signed char) 0xC5, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x00, (signed char) 0xC3, (signed char) 0x00,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x00, (signed char) 0xC3, (signed char) 0x02,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x08, (signed char) 0xC5, (signed char) 0x05,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x05, (signed char) 0xC5, (signed char) 0x05,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x05, (signed char) 0xC5, (signed char) 0x08,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x08, (signed char) 0xC5, (signed char) 0x08,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x08, (signed char) 0xC5, (signed char) 0x05,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x05, (signed char) 0xC5, (signed char) 0x05,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x0D, (signed char) 0xC5, (signed char) 0x0D,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x0D, (signed char) 0xC5, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x12, (signed char) 0xC3, (signed char) 0x12,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x15, (signed char) 0xC3, (signed char) 0x15,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x15, (signed char) 0xC5, (signed char) 0x15,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x15, (signed char) 0xC5, (signed char) 0x15,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x1A, (signed char) 0xC5, (signed char) 0x18,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x18, (signed char) 0xC5, (signed char) 0x1A,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x1A, (signed char) 0xC5, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x1A, (signed char) 0xC5, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x1D, (signed char) 0xC3, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x1D, (signed char) 0xC3, (signed char) 0x1A,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x1D, (signed char) 0xC5, (signed char) 0x1D,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x1D, (signed char) 0xC5, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x22, (signed char) 0xC3, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x22, (signed char) 0xC3, (signed char) 0x25,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x22, (signed char) 0xC5, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x22, (signed char) 0xC5, (signed char) 0x25,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x25, (signed char) 0xC5, (signed char) 0x22,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x2A, (signed char) 0xC5, (signed char) 0x2A,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x28, (signed char) 0xC5, (signed char) 0x28,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x28, (signed char) 0xC5, (signed char) 0x2A,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x2A, (signed char) 0xC5, (signed char) 0x2A,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x2A, (signed char) 0xC5, (signed char) 0x2A,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x2A, (signed char) 0xC5, (signed char) 0x28,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x32, (signed char) 0xC3, (signed char) 0x35,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x35, (signed char) 0xC3, (signed char) 0x35,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x35, (signed char) 0xC5, (signed char) 0x35,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x38, (signed char) 0xC5, (signed char) 0x38,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x3A, (signed char) 0xC3, (signed char) 0x3A,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x3A, (signed char) 0xC3, (signed char) 0x38,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x38, (signed char) 0xC3, (signed char) 0x38,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x3D, (signed char) 0xC5, (signed char) 0x3D,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x3D, (signed char) 0xC5, (signed char) 0x3D,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x3D, (signed char) 0xC5, (signed char) 0x40,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x40, (signed char) 0xC3, (signed char) 0x40,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x42, (signed char) 0xC5, (signed char) 0x42,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x42, (signed char) 0xC5, (signed char) 0x45,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x45, (signed char) 0xC5, (signed char) 0x45,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x45, (signed char) 0xC5, (signed char) 0x45,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x45, (signed char) 0xC5, (signed char) 0x42,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x45, (signed char) 0xC3, (signed char) 0x4A,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x4A, (signed char) 0xC3, (signed char) 0x4A,	// y0, x0, y1, x1
	(signed char) 0xC3, (signed char) 0x4A, (signed char) 0xC5, (signed char) 0x45,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x45, (signed char) 0xC5, (signed char) 0x48,	// y0, x0, y1, x1
	(signed char) 0xC5, (signed char) 0x48, (signed char) 0xC5, (signed char) 0x4A,	// y0, x0, y1, x1
};






void __displayLargeList(const signed char list[])
{
  int count = *list++;

  while (count >0)  
  {
    int y0 = *list++;
    int x0 = *list++;
    int y1 = *list++;
    int x1 = *list++;
    v_directDraw32(x0<<7, y0<<7,x1<<7,y1<<7, 0x5f);
    count --;
  }
}
void __displaySmallList(const signed char list[])
{
  int count = 720;

  while (count >0)  
  {
    int y0 = *list++;
    int x0 = *list++;
    int y1 = *list++;
    int x1 = *list++;
    v_directDraw32(x0<<6, (y0<<6)+000,x1<<6,(y1<<6)+000, 0x3f);
    count --;
  }
}

void handlePipeline();

// example of L2 cache disturbances by pressing button
static unsigned long int next = 1;
void goHavoc()
{

  v_setRefresh(50);
  v_setClientHz(2000);
  v_setupSMPHandling();
  v_enableJoystickAnalog(1,1,1,1);
  v_enableSoundOut(1);
  v_enableButtons(1);
  useDoubleTimer = 0;
    
  int t=10;
  v_setCustomClipping(1, -16000, -14000, 14400, 15000);
  __displayLargeList(HavocTitle);
  __displaySmallList(HavocText);

  printf("PipelineSize = %i\n", sizeof(VectorPipeline)*1900);
  
  
  
//  if the complete emulation can run in say 300K of "RAM"
//  than perhaps this can run in parallel to core 1
  
  while (1)
  {
    v_playAllSFX();
    v_doSound();    // not needed in IRQ Mode
    v_readButtons(); // not neededin IRQ mode
    v_readJoystick1Analog(); // not neededin IRQ mode

    v_WaitRecal();
40
    while ((((volatile unsigned char)currentButtonState&0x0f)) == (0x08)) 
    {
      extern VectorPipeline tmp_VPL[MAX_PIPELINE];
      extern VectorPipeline *_VPL[];
      extern int vectorPipeLineWriting;
      register unsigned int * p= (unsigned int *)_VPL[vectorPipeLineWriting];
      volatile register int a;
      while (1) 
      {
	//    for (register int i=0; i<MAX_PIPELINE*sizeof(VectorPipeline)/4; i++ )
            for (register int i=0; i<1900*sizeof(VectorPipeline)/4; i++ )
	    {
	      a += *(p+i);
	    }
	    if (a==1) break;
	    printf("Looping\n");
      }     
      
      v_readButtons(); // not neededin IRQ mode
      printf("%i\n",a);
    }
  }    

  while (1)  
    v_WaitRecal();
}
#endif

#endif // ifdef 0
