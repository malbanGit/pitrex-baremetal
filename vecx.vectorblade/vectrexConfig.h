#ifndef __VECX_CONFIG_H
#define __VECX_CONFIG_H

#define CUTOM_INIT
#define DIRECT_EMULATION
#define WAIT_EMULATION
#define EXACT_WAIT_EMULATION
//#define DO_NOT_WATCH_VIA_FOR_BUTTONS
//#define EMULATE_KNOWN_PRINTSTR


#include <vectrex/osWrapper.h>
#include <vectrex/vectrexInterface.h>

#include "../vectrexBase/e6522.h"
#include "../vectrexBase/e6809.h"
#include "../vectrexBase/edac.h"

extern unsigned char *alternate_raster5[];
extern unsigned char *alternate_rasterlines[];

extern uint8_t cart[32768*2*4]; // Vectorblade
extern uint8_t ram[1024];
extern uint8_t rom[8192];
extern unsigned char flashHS[4096];
extern unsigned char flashGS[4096];
void loadVectrexBin(char *selectedName, uint8_t *loadMem);

extern int32_t PrintStr(int32_t cycles);
extern int32_t PrintStr_d(int32_t cycles, int width, int height, unsigned char *rlines[]);

extern int currentBank;
extern int mustWork;
#define PC_TESTS \
do \
{ \
  cycles = vectorbladePCTests(cycles); \
} \
while(0);

static inline void saveFlashHS();
static inline void saveFlashGS();
static inline void loadFlashHS()
{
  int err=0;
  // loads flash 
  // or if not available creates an empty file
  
  // prepareName
  char *loadName = "VB_HS.SAV";
  char *dir = "vectrex";

  err = chdir (dir);
  if (err)
  {
    printf("vecx.vectorblade.vectrexConfig.h:loadFlashHS(): NO vectrex directory found...(%i)!\r\n", errno);
    return;
  }
  FILE *f;
  f = fopen(loadName, "r");
 
  if (f == 0)
  {
    err = chdir("..");
    printf("Creating: %s \r\n", loadName);
    // create file
    for (int i=0;i<4096;i++) flashHS[i] = cart[0xb000+i];
    
    saveFlashHS();
    return;
  }
  else
  {
    printf("Loading: %s \r\n", loadName);

    unsigned int fsize = __filelength(f);
    int sizeRead = fread(flashHS, 1,4096 , f);
    if ( sizeRead== 0)
    {
        fclose(f);
        printf("vecx.vectorblade.vectrexConfig.h:loadFlashHS(): File not loaded\r\n");
        err = chdir("..");
        return;
    }
    else
    {
        fclose(f);
        // file is loaded
        printf("... loaded file...\r\n");
    }
  }  
  err = chdir("..");
  
  // inject flash
  for (int i=0;i<4096;i++) cart[0xb000+i] = flashHS[i];
  
  
}

static inline void saveFlashHS()
{
  int err=0;
  char *loadName = "VB_HS.SAV";
  char *dir = "vectrex";

  printf("Saving: %s \r\n", loadName);

  err = chdir (dir);
  if (err)
  {
    printf("vecx.vectorblade.vectrexConfig.h:saveFlashHS(): NO vectrex directory found...(%i)!\r\n", errno);
    return;
  }

  
  FILE *fileWrite;
  // always as a "new file"
  fileWrite = fopen(loadName, "wb");
  if (fileWrite == 0)
  {
    printf("vecx.vectorblade.vectrexConfig.h:saveFlashHS(): Could not open file %s (%i) \r\n", loadName, errno);
    err = chdir("..");
    return;
  }
  unsigned int lenSaved=0;
  lenSaved = fwrite(flashHS, 4096, 1, fileWrite);
  if ( lenSaved != 1)
  {
    printf("vecx.vectorblade.vectrexConfig.h:saveFlashHS(): File not saved (1) (size written = %i) (error: %i)\r\n", lenSaved, errno);
  }
  fclose(fileWrite);
  err = chdir("..");
}

static inline void loadFlashGS()
{
  int err=0;
  // loads flash 
  // or if not available creates an empty file
  
  // prepareName
  char *loadName = "VB_GS.SAV";
  char *dir = "vectrex";

  err = chdir (dir);
  if (err)
  {
    printf("vecx.vectorblade.vectrexConfig.h:loadFlashGS(): NO vectrex directory found...(%i)!\r\n", errno);
    return;
  }
  FILE *f;
  f = fopen(loadName, "r");
 
  if (f == 0)
  {
    err = chdir("..");
    printf("Creating: %s \r\n", loadName);
    // create file
    for (int i=0;i<4096;i++) flashGS[i] = cart[0xa000+i];
    
    saveFlashGS();
    return;
  }
  else
  {
    printf("Loading: %s \r\n", loadName);

    unsigned int fsize = __filelength(f);
    int sizeRead = fread(flashGS, 1,4096 , f);
    if ( sizeRead== 0)
    {
        fclose(f);
        printf("vecx.vectorblade.vectrexConfig.h:loadFlashGS(): File not loaded (%s)\r\n", loadName);
        err = chdir("..");
        return;
    }
    else
    {
        fclose(f);
        // file is loaded
        printf("... loaded file...\r\n");
    }
  }  
  err = chdir("..");
  
  // inject flash
  for (int i=0;i<4096;i++) cart[0xa000+i] = flashGS[i];
}

static inline void saveFlashGS()
{
  int err=0;
  char *loadName = "VB_GS.SAV";
  char *dir = "vectrex";

  printf("Saving: %s \r\n", loadName);

  err = chdir (dir);
  if (err)
  {
    printf("vecx.vectorblade.vectrexConfig.h:saveFlashGS(): NO vectrex directory found...(%i)!\r\n", errno);
    return;
  }

  
  FILE *fileWrite;
  // always as a "new file"
  fileWrite = fopen(loadName, "wb");
  if (fileWrite == 0)
  {
    printf("vecx.vectorblade.vectrexConfig.h:saveFlashGS(): Could not open file %s (%i) \r\n", loadName, errno);
    err = chdir("..");
    return;
  }
  unsigned int lenSaved=0;
  lenSaved = fwrite(flashGS, 4096, 1, fileWrite);
  if ( lenSaved != 1)
  {
    printf("vecx.vectorblade.vectrexConfig.h:saveFlashGS(): File not saved (1) (size written = %i) (error: %i)\r\n", lenSaved, errno);
  }
  fclose(fileWrite);
  err = chdir("..");
}


// these tests look horrendous
// but keep in mind, that 
// you can probably do over 200 tests on the Pi in about 1 (ONE!) Vectrex cycle
// so speed or efficiency is not really a top priority!
static inline int32_t vectorbladePCTests(int32_t cycles)
{
#define CB ;
  if (currentBank == 0)
  {
    if (CPU.reg_pc == 0x9cb5) // reset
    {    
      // delete files
      // fill 
      for (int i=0; i< 4096; i++)
      {
        flashHS[i] = cart[0xb000+i]= 0xff;
      }
      ram[0xc8af&0x3ff] = 0xb0;
      ram[(0xc8af+1)&0x3ff] = 0x00;
    }    
    else if (CPU.reg_pc == 0x9b5e) // checkFlash 9b5e
    {    
      unsigned char flashAvailable = ram[0xC89c&0x3ff];
      flashAvailable = flashAvailable |1;
      ram[0xC89c&0x3ff] = flashAvailable;
      CPU.reg_pc = 0xf57d; // RTS
      mustWork = 0;
    }    
    else if (CPU.reg_pc == 0x9b12) // HS/Option write 
    {    
      // in U address of RAM
      // in X address of flash to write to

      // X = $b000 ++

      // Options
      do
      {
        unsigned char b = ram[(CPU.reg_u++)&0x3ff];
        flashHS[(CPU.reg_x++)&0xfff] = b;
      } while (CPU.reg_u<0xc8ae);
       
      CPU.reg_u= 0xca7d;
      // Highscore
      do
      {
        unsigned char b = ram[(CPU.reg_u++)&0x3ff];
        flashHS[(CPU.reg_x++)&0xfff] = b;
      } while (CPU.reg_u<0xcabd);
      saveFlashHS();
      loadFlashHS();


      CPU.reg_pc = 0xf57d; // RTS
      mustWork = 0;
    }    
    else if (CPU.reg_pc == 0x9d10) // game write 
    {    
      int i=0;
      flashGS[i++] = 12;
      
      for (int r=0xc800; r< 0xcc00; r++)
      {
        flashGS[i++] = ram[(r)&0x3ff];
      }
      saveFlashGS();
      loadFlashGS();

      CPU.reg_pc = 0x9d16; // in save game
      mustWork = 0;
    }    
    
#ifdef EMULATE_KNOWN_PRINTSTR    
    else if (CPU.reg_pc == 0x0750) // 8 bit font
    {
        cycles = PrintStr_d(cycles, 0x58, ram[0xC82A&0x3ff]<<2, alternate_rasterlines);
        CPU.reg_pc = 0xf57d; // RTS
        mustWork = 0;
    }
    else if (CPU.reg_pc == 0x1c6f) // 5 bit font
    {
        cycles = PrintStr_d(cycles, ram[0xC82B&0x3ff], -3, alternate_raster5);
        CPU.reg_pc = 0xf57d; // RTS
        mustWork = 0;
    }
#endif    
  }
  else if (currentBank == 1)
  {
#ifdef EMULATE_KNOWN_PRINTSTR    
    if (CPU.reg_pc == 0x68c2) // 5 bit font
     {
      cycles = PrintStr_d(cycles, ram[0xC82B&0x3ff], -3, alternate_raster5);
      CPU.reg_pc = 0xf57d; // RTS
      mustWork = 0;
    }
#endif    
  }
  
  else if (currentBank == 2)
  {
#ifdef EMULATE_KNOWN_PRINTSTR    
    if (CPU.reg_pc == 0x31fe) // // 5 bit font
    {
        cycles = PrintStr_d(cycles, ram[0xC82B&0x3ff], -3, alternate_raster5);
        CPU.reg_pc = 0xf57d; // RTS
        mustWork = 0;
    }
    else if (CPU.reg_pc == 0x15df) // // 5 bit font
    {
        cycles = PrintStr_d(cycles, ram[0xC82B&0x3ff], -3, alternate_raster5);
        CPU.reg_pc = 0xf57d; // RTS
        mustWork = 0;
    }
#endif    
  }
  else if (currentBank == 3)
  {
#ifdef EMULATE_KNOWN_PRINTSTR    
    if (CPU.reg_pc == 0x2cb3) // message full text, // 5 bit font
    {
        CPU.reg_a = 0x70;// 7010
        CPU.reg_b = 0x10;// 7010
        cycles = PrintStr_d(cycles, ram[0xC82B&0x3ff], -4, alternate_raster5);
        CPU.reg_pc = 0x2a95; // calibration7
        mustWork = 0;
    }
#endif    
  }
  
 if (CPU.reg_pc<0x02e0)
 {
   
  switch (CPU.reg_pc) //
  {
    case 0x003f: {currentBank =0; CB;break;}

    case 0x005c: {currentBank =1; CB break;}
    case 0x0068: {currentBank =2; CB break;}

    case 0x0083: {currentBank =0; CB break;}
    case 0x008d: {currentBank =3; CB break;}
    
    case 0x00b4: {currentBank =0; CB break;}
    case 0x00b8: {currentBank =3; CB break;}

    case 0x00d4: {currentBank =0; CB break;}
    case 0x00d8: {currentBank =2; CB break;}

    case 0x00ef: {currentBank =0; CB break;}
    case 0x00f3: {currentBank =2; CB break;}

    case 0x0102: {currentBank =currentBank & 2; CB break;}
    case 0x010c: {currentBank =currentBank | 1; CB break;}
    case 0x010e:  {currentBank =currentBank & 1; CB break;}

    case 0x0128: {currentBank =2; CB break;}

    case 0x0143: {currentBank =3; CB break;}

    case 0x0149: {currentBank +=2; CB break;}
    case 0x0154: {currentBank -=2; CB break;}
    
    case 0x016b: {currentBank =1; CB break;}
    case 0x0173: {currentBank =3; CB break;}
    
    case 0x0180: {currentBank =1; CB break;}
    case 0x0184: {currentBank =0; CB break;}

    case 0x019b: {currentBank =2; CB break;}
    case 0x019f: {currentBank =1; CB break;}

    case 0x01c3: {currentBank =2; CB break;}
    case 0x01c7: {currentBank =1; CB break;}
    
    case 0x01ee: {currentBank =3; CB break;}
    case 0x01f5: {currentBank =0; CB break;}
    
    case 0x0211: {currentBank =2; CB break;}

    case 0x0223: {currentBank +=2; CB break;}

    case 0x0229: {currentBank -=1; CB break;}
    case 0x0231: {currentBank +=1; CB break;}

    case 0x0238: {currentBank -=1; CB break;}

    case 0x0242: {currentBank +=1; CB break;}
    case 0x0246: {currentBank -=1; CB break;}

    case 0x024f: {currentBank +=1; CB break;}
    
    case 0x0255: {currentBank +=1; CB break;}
    case 0x025e: {currentBank -=1; CB break;}
    
    case 0x026b: {currentBank =3; CB break;}
    
    case 0x0272: {currentBank =3; CB break;}

    case 0x0279: {currentBank =3; CB break;}

    case 0x0284: {currentBank =2; CB break;}

    case 0x028d: {currentBank =2; CB break;}

    case 0x029a: {if (currentBank==3){ currentBank =2; CB} break;}

    case 0x02d4: {if (currentBank==2){currentBank =3; CB} break;}
  } 
 }
 return cycles;
}

// if 1 is returned, the normal vecx init is also run!
static inline int customInit()
{
  loadVectrexBin("VECTORBLADE.NIB", cart);
  loadFlashHS();
  loadFlashGS();
  cart[2*32768+0x9037] = 4; //9037 = 4    only 4 star "waves" in highscore
  return 0;
}

#endif

