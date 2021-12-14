/*****************************************************************************
* CINEDB.CPP                                                                 *
*                                                                            *
* Cinematronics Emulator / Debugger.                                         *
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include <vectrex/vectrexInterface.h>








#define   VERSION   "Version 1.3"

#include "cines.h"

int filelength(FILE *fd)		// also used in inifile.c so not static
{
   int fsize;
   fseek (fd, 0, SEEK_END);
// my book says return 0 on success!
   fsize = ftell(fd); 
   (void) fseek (fd, 0, SEEK_SET);
   return fsize;
}

/*****************************************************************************
 * Read ROM images.                                                          *
 *                                                                           *
 * Returns:                                                                  *
 *      0 - Ok                                                               *
 *      1 - Error, (error message already displayed)                         *
 *****************************************************************************/
uint readRoms (char *romName[8], uchar OptMemSize)
{
   FILE *inFile;
   char *aFileName;
   int ii;
   int base[2];
   int addr;
   unsigned int sizeRead;
   uint romSize=0, romCount=0, evnOdd;
   ulong tempSize;

   switch (OptMemSize) {
   case 0:			// 4k games
      romSize = 2048;      romCount = 2;
      break;

   case 1:			// 8k games
      romSize = 2048;      romCount = 4;
      break;

   case 2:			// 16k games
      romSize = 4096;      romCount = 4;
      break;

   case 3:			// 32k game
      romSize = 4096;      romCount = 8;
      break;

   case 4:			// 32k game
      romSize = 8192;      romCount = 4;
      break;
   }

   evnOdd = 0;			// zero even odd index
   base[0] = 0x0000; base[1] = 0x0001;
   
   
   for (ii = 0; ii < romCount; ii++) 
   {
      addr = base[evnOdd];
      aFileName = romName[ii];

      inFile = fopen (aFileName, "r");
      if (inFile == 0) 
      {
         printf ("readRoms: error opening file: \"%s\"\n", aFileName);
         return (1);
      }
      tempSize = (int) filelength (inFile);	// get length of file BEFORE ANY READING IS DONE

      if (tempSize != romSize) 
      {
        fclose (inFile);
        printf ("readRoms: file \"%s\" wrong size. (Must be %u bytes.)\n", aFileName, romSize);
        return (1);
      }
      printf( "Loading %s at 0x%04x\n", aFileName, addr);

      sizeRead = 0;
      for (;;) 
      {
        int c = fgetc(inFile); 
        if (c == EOF) break;
        
        rom[addr] = c;
        addr += 2; base[evnOdd] += 2;
        sizeRead += 1;
      }
      
      if (sizeRead != romSize)
      {
        fclose (inFile);
        printf ("readRoms: error reading file: \"%s\"(%i,%i)\r\n", aFileName, sizeRead, romSize);
        return (1);
      }
      fclose (inFile);

      evnOdd ^= 0x0001;
   }
   return (0);
}


/**************************** main **********************************/
int Game;

#define No 0
#define Yes 1
void loadTailgunner()
{
    unsigned char romLoad[0x2000];
    int r = 0;
    r += loadFromZip ("roms/tailg.zip", "tgunner.t70", &romLoad[0x0000]);
    r += loadFromZip ("roms/tailg.zip", "tgunner.p70", &romLoad[0x0800]);
    r += loadFromZip ("roms/tailg.zip", "tgunner.t71", &romLoad[0x1000]);
    r += loadFromZip ("roms/tailg.zip", "tgunner.p71", &romLoad[0x1800]);
    uint32_t crc;
    crc = 0;printf("CRC32 tgunner.t70: %08X\r\n", crc32(&romLoad[0x0000], 0x800, &crc ));
    crc = 0;printf("CRC32 tgunner.p70: %08X\r\n",  crc32(&romLoad[0x0800], 0x800, &crc ));
    crc = 0;printf("CRC32 tgunner.t71: %08X\r\n",  crc32(&romLoad[0x1000], 0x800, &crc ));
    crc = 0;printf("CRC32 tgunner.p71: %08X\r\n", crc32(&romLoad[0x1800], 0x800, &crc ));
    
    crc = 0;if (crc32(&romLoad[0x0000], 0x800, &crc ) != 0x21EC9A04) r -=1;
    crc = 0;if (crc32(&romLoad[0x0800], 0x800, &crc ) != 0x8D7410B3) r -=1;
    crc = 0;if (crc32(&romLoad[0x1000], 0x800, &crc ) != 0x2C954AB6) r -=1;
    crc = 0;if (crc32(&romLoad[0x1800], 0x800, &crc ) != 0x8E2C8494) r -=1;
    
    
    for (int i=0; i<0x800; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x0800+i];
      
      rom[0x1000+i*2] = romLoad[0x1000+0x0000+i];
      rom[0x1000+1+i*2] = romLoad[0x1000+0x0800+i];
    }
    
    if (r != 0)
    {
        v_message("TROUBLE WITH RIP OFF ROMS!");
    }
}

void loadRipoff()
{
    unsigned char romLoad[0x2000];
    int r = 0;
    r += loadFromZip ("roms/ripoff.zip", "ripoff.t7", &romLoad[0x0000]);
    r += loadFromZip ("roms/ripoff.zip", "ripoff.p7", &romLoad[0x0800]);
    r += loadFromZip ("roms/ripoff.zip", "ripoff.u7", &romLoad[0x1000]);
    r += loadFromZip ("roms/ripoff.zip", "ripoff.r7", &romLoad[0x1800]);
    uint32_t crc;
    crc = 0;printf("CRC32 ripoff.t7: %08X\r\n", crc32(&romLoad[0x0000], 0x800, &crc ));
    crc = 0;printf("CRC32 ripoff.p7: %08X\r\n",  crc32(&romLoad[0x0800], 0x800, &crc ));
    crc = 0;printf("CRC32 ripoff.u7: %08X\r\n",  crc32(&romLoad[0x1000], 0x800, &crc ));
    crc = 0;printf("CRC32 ripoff.r7: %08X\r\n", crc32(&romLoad[0x1800], 0x800, &crc ));
    
    crc = 0;if (crc32(&romLoad[0x0000], 0x800, &crc ) != 0x40C2C5B8) r -=1;
    crc = 0;if (crc32(&romLoad[0x0800], 0x800, &crc ) != 0xA9208AFB) r -=1;
    crc = 0;if (crc32(&romLoad[0x1000], 0x800, &crc ) != 0x29C13701) r -=1;
    crc = 0;if (crc32(&romLoad[0x1800], 0x800, &crc ) != 0x150BD4C8) r -=1;
    
    
    for (int i=0; i<0x800; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x0800+i];
      
      rom[0x1000+i*2] = romLoad[0x1000+0x0000+i];
      rom[0x1000+1+i*2] = romLoad[0x1000+0x0800+i];
    }
    
    if (r != 0)
    {
        v_message("TROUBLE WITH RIP OFF ROMS!");
    }
}

void loadArmorAttack()
{
    unsigned char romLoad[0x4000];
    int r = 0;
    r += loadFromZip ("roms/armora.zip", "ar414le.t6", &romLoad[0x0000]);
    r += loadFromZip ("roms/armora.zip", "ar414lo.p6", &romLoad[0x1000]);
    r += loadFromZip ("roms/armora.zip", "ar414ue.u6", &romLoad[0x2000]);
    r += loadFromZip ("roms/armora.zip", "ar414uo.r6", &romLoad[0x3000]);
    uint32_t crc;
    crc = 0;printf("CRC32 ar414le.t6: %08X\r\n", crc32(&romLoad[0x0000], 0x1000, &crc ));
    crc = 0;printf("CRC32 ar414lo.p6: %08X\r\n",  crc32(&romLoad[0x1000], 0x1000, &crc ));
    crc = 0;printf("CRC32 ar414ue.u6: %08X\r\n",  crc32(&romLoad[0x2000], 0x1000, &crc ));
    crc = 0;printf("CRC32 ar414uo.r6: %08X\r\n", crc32(&romLoad[0x3000], 0x1000, &crc ));
    
    crc = 0;if (crc32(&romLoad[0x0000], 0x1000, &crc ) != 0xD7E71F84) r -=1;
    crc = 0;if (crc32(&romLoad[0x1000], 0x1000, &crc ) != 0xDF1C2370) r -=1;
    crc = 0;if (crc32(&romLoad[0x2000], 0x1000, &crc ) != 0xB0276118) r -=1;
    crc = 0;if (crc32(&romLoad[0x3000], 0x1000, &crc ) != 0x229D779F) r -=1;
    
    
    for (int i=0; i<0x1000; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x1000+i];
      
      rom[0x2000+i*2] = romLoad[0x2000+0x0000+i];
      rom[0x2000+1+i*2] = romLoad[0x2000+0x1000+i];
    }
    if (r != 0)
    {
        v_message("TROUBLE WITH ARMOR ATTACK ROMS!");
    }
}

void loadDemon()
{
    unsigned char romLoad[0x4000];
    int r = 0;
    r += loadFromZip ("roms/demon.zip", "demon.7t", &romLoad[0x0000]);
    r += loadFromZip ("roms/demon.zip", "demon.7p", &romLoad[0x1000]);
    r += loadFromZip ("roms/demon.zip", "demon.7u", &romLoad[0x2000]);
    r += loadFromZip ("roms/demon.zip", "demon.7r", &romLoad[0x3000]);
    uint32_t crc;
    crc = 0;printf("CRC32 demon.7t: %08X\r\n", crc32(&romLoad[0x0000], 0x1000, &crc ));
    crc = 0;printf("CRC32 demon.7p: %08X\r\n",  crc32(&romLoad[0x1000], 0x1000, &crc ));
    crc = 0;printf("CRC32 demon.7u: %08X\r\n",  crc32(&romLoad[0x2000], 0x1000, &crc ));
    crc = 0;printf("CRC32 demon.7r: %08X\r\n", crc32(&romLoad[0x3000], 0x1000, &crc ));
    
    crc = 0;if (crc32(&romLoad[0x0000], 0x1000, &crc ) != 0x866596C1) r -=1;
    crc = 0;if (crc32(&romLoad[0x1000], 0x1000, &crc ) != 0x1109E2F1) r -=1;
    crc = 0;if (crc32(&romLoad[0x2000], 0x1000, &crc ) != 0xD447A3C3) r -=1;
    crc = 0;if (crc32(&romLoad[0x3000], 0x1000, &crc ) != 0x64B515F0) r -=1;
    
    
    for (int i=0; i<0x1000; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x1000+i];
      
      rom[0x2000+i*2] = romLoad[0x2000+0x0000+i];
      rom[0x2000+1+i*2] = romLoad[0x2000+0x1000+i];
    }
    if (r != 0)
    {
        v_message("TROUBLE WITH DEMON ROMS!");
    }
}

void loadSolarQuest()
{
    unsigned char romLoad[0x4000];
    int r = 0;
    uint32_t crc;
    
    r += loadFromZip ("roms/solarq.zip", "solar.6t", &romLoad[0x0000]);
    r += loadFromZip ("roms/solarq.zip", "solar.6p", &romLoad[0x1000]);
    r += loadFromZip ("roms/solarq.zip", "solar.6u", &romLoad[0x2000]);
    r += loadFromZip ("roms/solarq.zip", "solar.6r", &romLoad[0x3000]);
    if (r == 0)
    {
      crc = 0;printf("CRC32 solar.6t: %08X\r\n", crc32(&romLoad[0x0000], 0x1000, &crc ));
      crc = 0;printf("CRC32 solar.6p: %08X\r\n",  crc32(&romLoad[0x1000], 0x1000, &crc ));
      crc = 0;printf("CRC32 solar.6u: %08X\r\n",  crc32(&romLoad[0x2000], 0x1000, &crc ));
      crc = 0;printf("CRC32 solar.6r: %08X\r\n", crc32(&romLoad[0x3000], 0x1000, &crc ));
    }
    else
    {
      r = 0;
      r += loadFromZip ("roms/solarq.zip", "sq-2_le.6t", &romLoad[0x0000]);
      r += loadFromZip ("roms/solarq.zip", "sq-2_lo.6p", &romLoad[0x1000]);
      r += loadFromZip ("roms/solarq.zip", "sq-2_ue.6u", &romLoad[0x2000]);
      r += loadFromZip ("roms/solarq.zip", "sq-2_uo.6r", &romLoad[0x3000]);
      crc = 0;printf("CRC32 sq-2_le.6t: %08X\r\n", crc32(&romLoad[0x0000], 0x1000, &crc ));
      crc = 0;printf("CRC32 sq-2_lo.6p: %08X\r\n",  crc32(&romLoad[0x1000], 0x1000, &crc ));
      crc = 0;printf("CRC32 sq-2_ue.6u: %08X\r\n",  crc32(&romLoad[0x2000], 0x1000, &crc ));
      crc = 0;printf("CRC32 sq-2_uo.6r: %08X\r\n", crc32(&romLoad[0x3000], 0x1000, &crc ));
    }
      
      


    
    crc = 0;if (crc32(&romLoad[0x0000], 0x1000, &crc ) != 0x1F3C5333) r -=1;
    crc = 0;if (crc32(&romLoad[0x1000], 0x1000, &crc ) != 0xD6C16BCC) r -=1;
    crc = 0;if (crc32(&romLoad[0x2000], 0x1000, &crc ) != 0xA5970E5C) r -=1;
    crc = 0;if (crc32(&romLoad[0x3000], 0x1000, &crc ) != 0xB763FFF2) r -=1;
    
    
    for (int i=0; i<0x1000; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x1000+i];
      
      rom[0x2000+i*2] = romLoad[0x2000+0x0000+i];
      rom[0x2000+1+i*2] = romLoad[0x2000+0x1000+i];
    }
    if (r != 0)
    {
        v_message("TROUBLE WITH SOLAR QUEST ROMS!");
    }
}

void loadWotw()
{
    unsigned char romLoad[0x4000];
    int r = 0;
    r += loadFromZip ("roms/wotw.zip", "wow_le.t7", &romLoad[0x0000]);
    r += loadFromZip ("roms/wotw.zip", "wow_lo.p7", &romLoad[0x1000]);
    r += loadFromZip ("roms/wotw.zip", "wow_ue.u7", &romLoad[0x2000]);
    r += loadFromZip ("roms/wotw.zip", "wow_uo.r7", &romLoad[0x3000]);
    uint32_t crc;
    crc = 0;printf("CRC32 wow_le.t7: %08X\r\n", crc32(&romLoad[0x0000], 0x1000, &crc ));
    crc = 0;printf("CRC32 wow_lo.p7: %08X\r\n",  crc32(&romLoad[0x1000], 0x1000, &crc ));
    crc = 0;printf("CRC32 wow_ue.u7: %08X\r\n",  crc32(&romLoad[0x2000], 0x1000, &crc ));
    crc = 0;printf("CRC32 wow_uo.r7: %08X\r\n", crc32(&romLoad[0x3000], 0x1000, &crc ));
    
    crc = 0;if (crc32(&romLoad[0x0000], 0x1000, &crc ) != 0xB16440F9) r -=1;
    crc = 0;if (crc32(&romLoad[0x1000], 0x1000, &crc ) != 0xBFDF4A5A) r -=1;
    crc = 0;if (crc32(&romLoad[0x2000], 0x1000, &crc ) != 0x9B5CEA48) r -=1;
    crc = 0;if (crc32(&romLoad[0x3000], 0x1000, &crc ) != 0xC9D3C866) r -=1;
    
    
    for (int i=0; i<0x1000; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x1000+i];
      
      rom[0x2000+i*2] = romLoad[0x2000+0x0000+i];
      rom[0x2000+1+i*2] = romLoad[0x2000+0x1000+i];
    }
    if (r != 0)
    {
        v_message("TROUBLE WITH WAR OF THE WORLDS ROMS!");
    }
}


void loadStarCastle()
{
    unsigned char romLoad[0x4000];
    int r = 0;
    r += loadFromZip ("roms/starcas.zip", "starcas3.t7", &romLoad[0x0000]);
    r += loadFromZip ("roms/starcas.zip", "starcas3.p7", &romLoad[0x0800]);
    r += loadFromZip ("roms/starcas.zip", "starcas3.u7", &romLoad[0x1000]);
    r += loadFromZip ("roms/starcas.zip", "starcas3.r7", &romLoad[0x1800]);
    uint32_t crc;
    crc = 0;printf("CRC32 starcas3.t7: %08X\r\n", crc32(&romLoad[0x0000], 0x0800, &crc ));
    crc = 0;printf("CRC32 starcas3.p7: %08X\r\n",  crc32(&romLoad[0x0800], 0x0800, &crc ));
    crc = 0;printf("CRC32 starcas3.u7: %08X\r\n",  crc32(&romLoad[0x1000], 0x0800, &crc ));
    crc = 0;printf("CRC32 starcas3.r7: %08X\r\n", crc32(&romLoad[0x1800], 0x0800, &crc ));
    
    crc = 0;if (crc32(&romLoad[0x0000], 0x0800, &crc ) != 0xB5838B5D) r -=1;
    crc = 0;if (crc32(&romLoad[0x0800], 0x0800, &crc ) != 0xF6BC2F4D) r -=1;
    crc = 0;if (crc32(&romLoad[0x1000], 0x0800, &crc ) != 0x188CD97C) r -=1;
    crc = 0;if (crc32(&romLoad[0x1800], 0x0800, &crc ) != 0xC367B69D) r -=1;
    
    
    for (int i=0; i<0x0800; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x0800+i];
      
      rom[0x1000+i*2] = romLoad[0x1000+0x0000+i];
      rom[0x1000+1+i*2] = romLoad[0x1000+0x0800+i];
    }
    if (r != 0)
    {
        v_message("TROUBLE WITH STAR CASTLE ROMS!");
    }
}
void loadSpeedFreak()
{
    unsigned char romLoad[0x4000];
    int r = 0;
    r += loadFromZip ("roms/speedfrk.zip", "speedfrk.t7", &romLoad[0x0000]);
    r += loadFromZip ("roms/speedfrk.zip", "speedfrk.p7", &romLoad[0x0800]);
    r += loadFromZip ("roms/speedfrk.zip", "speedfrk.u7", &romLoad[0x1000]);
    r += loadFromZip ("roms/speedfrk.zip", "speedfrk.r7", &romLoad[0x1800]);
    uint32_t crc;
    crc = 0;printf("CRC32 speedfrk.t7: %08X\r\n", crc32(&romLoad[0x0000], 0x0800, &crc ));
    crc = 0;printf("CRC32 speedfrk.p7: %08X\r\n",  crc32(&romLoad[0x0800], 0x0800, &crc ));
    crc = 0;printf("CRC32 speedfrk.u7: %08X\r\n",  crc32(&romLoad[0x1000], 0x0800, &crc ));
    crc = 0;printf("CRC32 speedfrk.r7: %08X\r\n", crc32(&romLoad[0x1800], 0x0800, &crc ));
    
    crc = 0;if (crc32(&romLoad[0x0000], 0x0800, &crc ) != 0x3552C03F) r -=1;
    crc = 0;if (crc32(&romLoad[0x0800], 0x0800, &crc ) != 0x4B90CDEC) r -=1;
    crc = 0;if (crc32(&romLoad[0x1000], 0x0800, &crc ) != 0x616C7CF9) r -=1;
    crc = 0;if (crc32(&romLoad[0x1800], 0x0800, &crc ) != 0xFBE90D63) r -=1;
    
    
    for (int i=0; i<0x0800; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x0800+i];
      
      rom[0x1000+i*2] = romLoad[0x1000+0x0000+i];
      rom[0x1000+1+i*2] = romLoad[0x1000+0x0800+i];
    }
    if (r != 0)
    {
        v_message("TROUBLE WITH SPEED FREAK ROMS!");
    }
}
void loadWarrior()
{
    unsigned char romLoad[0x4000];
    int r = 0;
    r += loadFromZip ("roms/warrior.zip", "warrior.t7", &romLoad[0x0000]);
    r += loadFromZip ("roms/warrior.zip", "warrior.p7", &romLoad[0x0800]);
    r += loadFromZip ("roms/warrior.zip", "warrior.u7", &romLoad[0x1000]);
    r += loadFromZip ("roms/warrior.zip", "warrior.r7", &romLoad[0x1800]);
    uint32_t crc;
    crc = 0;printf("CRC32 warrior.t7: %08X\r\n", crc32(&romLoad[0x0000], 0x0800, &crc ));
    crc = 0;printf("CRC32 warrior.p7: %08X\r\n",  crc32(&romLoad[0x0800], 0x0800, &crc ));
    crc = 0;printf("CRC32 warrior.u7: %08X\r\n",  crc32(&romLoad[0x1000], 0x0800, &crc ));
    crc = 0;printf("CRC32 warrior.r7: %08X\r\n", crc32(&romLoad[0x1800], 0x0800, &crc ));
    
    crc = 0;if (crc32(&romLoad[0x0000], 0x0800, &crc ) != 0xAC3646F9) r -=1;
    crc = 0;if (crc32(&romLoad[0x0800], 0x0800, &crc ) != 0x517D3021) r -=1;
    crc = 0;if (crc32(&romLoad[0x1000], 0x0800, &crc ) != 0x2E39340F) r -=1;
    crc = 0;if (crc32(&romLoad[0x1800], 0x0800, &crc ) != 0x8E91B502) r -=1;
    
    
    for (int i=0; i<0x0800; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x0800+i];
      
      rom[0x1000+i*2] = romLoad[0x1000+0x0000+i];
      rom[0x1000+1+i*2] = romLoad[0x1000+0x0800+i];
    }
    if (r != 0)
    {
        v_message("TROUBLE WITH WARRIOR ROMS!");
    }
}
 
void loadSundance()
{
    unsigned char romLoad[0x4000];
    int r = 0;
    r += loadFromZip ("roms/sundance.zip", "sundance.t7", &romLoad[0x0000]);
    r += loadFromZip ("roms/sundance.zip", "sundance.p7", &romLoad[0x0800]);
    r += loadFromZip ("roms/sundance.zip", "sundance.u7", &romLoad[0x1000]);
    r += loadFromZip ("roms/sundance.zip", "sundance.r7", &romLoad[0x1800]);
    uint32_t crc;
    crc = 0;printf("CRC32 sundance.t7: %08X\r\n", crc32(&romLoad[0x0000], 0x0800, &crc ));
    crc = 0;printf("CRC32 sundance.p7: %08X\r\n",  crc32(&romLoad[0x0800], 0x0800, &crc ));
    crc = 0;printf("CRC32 sundance.u7: %08X\r\n",  crc32(&romLoad[0x1000], 0x0800, &crc ));
    crc = 0;printf("CRC32 sundance.r7: %08X\r\n", crc32(&romLoad[0x1800], 0x0800, &crc ));
    
    crc = 0;if (crc32(&romLoad[0x0000], 0x0800, &crc ) != 0xD5B9CB19) r -=1;
    crc = 0;if (crc32(&romLoad[0x0800], 0x0800, &crc ) != 0x445C4F20) r -=1;
    crc = 0;if (crc32(&romLoad[0x1000], 0x0800, &crc ) != 0x67887D48) r -=1;
    crc = 0;if (crc32(&romLoad[0x1800], 0x0800, &crc ) != 0x10B77EBD) r -=1;
    
    
    for (int i=0; i<0x0800; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x0800+i];
      
      rom[0x1000+i*2] = romLoad[0x1000+0x0000+i];
      rom[0x1000+1+i*2] = romLoad[0x1000+0x0800+i];
    }
    if (r != 0)
    {
        v_message("TROUBLE WITH SUNDANCE ROMS!");
    }
}

     
void loadSpaceWar()
{
    unsigned char romLoad[0x2000];
    int r = 0;
    r += loadFromZip ("roms/spacewar.zip", "spacewar.1l", &romLoad[0x0000]);
    r += loadFromZip ("roms/spacewar.zip", "spacewar.2r", &romLoad[0x0800]);
    uint32_t crc;
    crc = 0;printf("CRC32 spacewar.1l: %08X\r\n", crc32(&romLoad[0x0000], 0x800, &crc ));
    crc = 0;printf("CRC32 spacewar.2r: %08X\r\n",  crc32(&romLoad[0x0800], 0x800, &crc ));
    
    crc = 0;if (crc32(&romLoad[0x0000], 0x800, &crc ) != 0xEDF0FD53) r -=1;
    crc = 0;if (crc32(&romLoad[0x0800], 0x800, &crc ) != 0x4F21328B) r -=1;
    
    
    for (int i=0; i<0x800; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x0800+i];
    }
    
    if (r != 0)
    {
        v_message("TROUBLE WITH SPACE WARS ROMS!");
    }
}

void loadStarHawk()
{
    unsigned char romLoad[0x2000];
    int r = 0;
    r += loadFromZip ("roms/starhawk.zip", "u7", &romLoad[0x0000]);
    r += loadFromZip ("roms/starhawk.zip", "r7", &romLoad[0x0800]);
    uint32_t crc;
    crc = 0;printf("CRC32 u7: %08X\r\n", crc32(&romLoad[0x0000], 0x800, &crc ));
    crc = 0;printf("CRC32 r7: %08X\r\n",  crc32(&romLoad[0x0800], 0x800, &crc ));
    
    crc = 0;if (crc32(&romLoad[0x0000], 0x800, &crc ) != 0x376E6C5C) r -=1;
    crc = 0;if (crc32(&romLoad[0x0800], 0x800, &crc ) != 0xBB71144F) r -=1;
    
    
    for (int i=0; i<0x800; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x0800+i];
    }
    
    if (r != 0)
    {
        v_message("TROUBLE WITH STAR HAWK ROMS!");
    }
}
void loadBarrier()
{
    unsigned char romLoad[0x2000];
    int r = 0;
    r += loadFromZip ("roms/barrier.zip", "barrier.t7", &romLoad[0x0000]);
    r += loadFromZip ("roms/barrier.zip", "barrier.p7", &romLoad[0x0800]);
    uint32_t crc;
    crc = 0;printf("CRC32 barrier.t7: %08X\r\n", crc32(&romLoad[0x0000], 0x800, &crc ));
    crc = 0;printf("CRC32 barrier.p7: %08X\r\n",  crc32(&romLoad[0x0800], 0x800, &crc ));
    
    crc = 0;if (crc32(&romLoad[0x0000], 0x800, &crc ) != 0x7C3D68C8) r -=1;
    crc = 0;if (crc32(&romLoad[0x0800], 0x800, &crc ) != 0xAEC142B5) r -=1;
    
    
    for (int i=0; i<0x800; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x0800+i];
    }
    
    if (r != 0)
    {
        v_message("TROUBLE WITH BARRIER ROMS!");
    }
}


void loadBoxingBugs()
{
    unsigned char romLoad[0x8000];
    int r = 0;
    r += loadFromZip ("roms/boxingb.zip", "u1a", &romLoad[0x0000]);
    r += loadFromZip ("roms/boxingb.zip", "u1b", &romLoad[0x1000]);
    r += loadFromZip ("roms/boxingb.zip", "u2a", &romLoad[0x2000]);
    r += loadFromZip ("roms/boxingb.zip", "u2b", &romLoad[0x3000]);
    r += loadFromZip ("roms/boxingb.zip", "u3a", &romLoad[0x4000]);
    r += loadFromZip ("roms/boxingb.zip", "u3b", &romLoad[0x5000]);
    r += loadFromZip ("roms/boxingb.zip", "u4a", &romLoad[0x6000]);
    r += loadFromZip ("roms/boxingb.zip", "u4b", &romLoad[0x7000]);
    uint32_t crc;
    crc = 0;printf("CRC32 u1a: %08X\r\n", crc32(&romLoad[0x0000], 0x1000, &crc ));
    crc = 0;printf("CRC32 u1b: %08X\r\n",  crc32(&romLoad[0x1000], 0x1000, &crc ));
    crc = 0;printf("CRC32 u2a: %08X\r\n",  crc32(&romLoad[0x2000], 0x1000, &crc ));
    crc = 0;printf("CRC32 u2b: %08X\r\n", crc32(&romLoad[0x3000], 0x1000, &crc ));
    crc = 0;printf("CRC32 u3a: %08X\r\n", crc32(&romLoad[0x4000], 0x1000, &crc ));
    crc = 0;printf("CRC32 u3b: %08X\r\n",  crc32(&romLoad[0x5000], 0x1000, &crc ));
    crc = 0;printf("CRC32 u4a: %08X\r\n",  crc32(&romLoad[0x6000], 0x1000, &crc ));
    crc = 0;printf("CRC32 u4b: %08X\r\n", crc32(&romLoad[0x7000], 0x1000, &crc ));
    
    crc = 0;if (crc32(&romLoad[0x0000], 0x1000, &crc ) != 0xD3115B0F) r -=1;
    crc = 0;if (crc32(&romLoad[0x1000], 0x1000, &crc ) != 0x3A44268D) r -=1;
    crc = 0;if (crc32(&romLoad[0x2000], 0x1000, &crc ) != 0xC97A9CBB) r -=1;
    crc = 0;if (crc32(&romLoad[0x3000], 0x1000, &crc ) != 0x98D34FF5) r -=1;
    crc = 0;if (crc32(&romLoad[0x4000], 0x1000, &crc ) != 0x5BB3269B) r -=1;
    crc = 0;if (crc32(&romLoad[0x5000], 0x1000, &crc ) != 0x85BF83AD) r -=1;
    crc = 0;if (crc32(&romLoad[0x6000], 0x1000, &crc ) != 0x25B51799) r -=1;
    crc = 0;if (crc32(&romLoad[0x7000], 0x1000, &crc ) != 0x7F41DE6A) r -=1;
    
    
    for (int i=0; i<0x1000; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x1000+i];
      
      rom[0x2000+i*2] = romLoad[0x2000+0x0000+i];
      rom[0x2000+1+i*2] = romLoad[0x2000+0x1000+i];
      
      rom[0x4000+i*2] = romLoad[0x4000+0x0000+i];
      rom[0x4000+1+i*2] = romLoad[0x4000+0x1000+i];
      
      rom[0x6000+i*2] = romLoad[0x6000+0x0000+i];
      rom[0x6000+1+i*2] = romLoad[0x6000+0x1000+i];
    }
    
    if (r != 0)
    {
        v_message("TROUBLE WITH BOXING BUGS ROMS!");
    }
}
void loadCosmicChasm()
{
    // this seems to be a 64k ROM
    // - or I don't know how to load it...
  
  
   // MAME roms contain u1 - u16 (each 4k)
  /*
   * 
   * Is there a bankswitch
   * Or do they use the full 64kk address space?
     RomImages[0]="roms/cosmicchasm/chasm.u4";
     RomImages[1]="roms/cosmicchasm/chasm.u12";
     RomImages[2]="roms/cosmicchasm/chasm.u8";
     RomImages[3]="roms/cosmicchasm/chasm.u16";
     RomImages[4]="roms/cosmicchasm/chasm.u3";
     RomImages[5]="roms/cosmicchasm/chasm.u11";
     RomImages[6]="roms/cosmicchasm/chasm.u7";
     RomImages[7]="roms/cosmicchasm/chasm.u15";
     
     what about 1 / 9
     what about 2 / 10
     what about 5 / 13
     what about 6 / 14
     ???
     
     INFO: MAME
     MCFG_CPU_ADD("maincpu", M68000, CCHASM_68K_CLOCK)    / * 8 MHz (from schematics) * /
     
ROM_START( cchasm )
    ROM_REGION( 0x010000, "maincpu", 0 )
    ROM_LOAD16_BYTE( "chasm.u4",  0x000000, 0x001000, CRC(19244f25) SHA1(79deaae82da8d1b16d05bbac43ba900c4b1d9f26) )
    ROM_LOAD16_BYTE( "chasm.u12", 0x000001, 0x001000, CRC(5d702c7d) SHA1(cbdceed45a1112594fbcbeb6976edc932b32d518) )
    ROM_LOAD16_BYTE( "chasm.u8",  0x002000, 0x001000, CRC(56a7ce8a) SHA1(14c790dcddb78d3b81b5a65fe3529e42c9708273) )
    ROM_LOAD16_BYTE( "chasm.u16", 0x002001, 0x001000, CRC(2e192db0) SHA1(1a8ff983295ab52b5099c089b3142cdc56d28aee) )
    ROM_LOAD16_BYTE( "chasm.u3",  0x004000, 0x001000, CRC(9c71c600) SHA1(900526eaff7483fc478ebfb3f14796ff8fd1d01f) )
    ROM_LOAD16_BYTE( "chasm.u11", 0x004001, 0x001000, CRC(a4eb59a5) SHA1(a7bb3ca8f1f000f224def6342ca9d1eabcb210e6) )
    ROM_LOAD16_BYTE( "chasm.u7",  0x006000, 0x001000, CRC(8308dd6e) SHA1(82ad7c27e9a41af5280ecd975d3530ff2ed27ad4) )
    ROM_LOAD16_BYTE( "chasm.u15", 0x006001, 0x001000, CRC(9d3abf97) SHA1(476d684182d92d66263df82e1b5c4ff24b6814e8) )
    ROM_LOAD16_BYTE( "u2",        0x008000, 0x001000, CRC(4e076ae7) SHA1(a72f5425b256785b810ee5f23917b44f778cfcd3) )
    ROM_LOAD16_BYTE( "u10",       0x008001, 0x001000, CRC(cc9e19ca) SHA1(6c46ec265c2cc0683470ed1df978b96b577c5ca1) )
    ROM_LOAD16_BYTE( "chasm.u6",  0x00a000, 0x001000, CRC(a96525d2) SHA1(1c41bc3bf051cf1830182cbde6fba4e56db7e431) )
    ROM_LOAD16_BYTE( "chasm.u14", 0x00a001, 0x001000, CRC(8e426628) SHA1(2d70a7717b18cc892332b9d5d2de3ceba6c1481d) )
    ROM_LOAD16_BYTE( "u1",        0x00c000, 0x001000, CRC(88b71027) SHA1(49fa676d7838c643d642fbc70579ce29e76ba724) )
    ROM_LOAD16_BYTE( "chasm.u9",  0x00c001, 0x001000, CRC(d90c9773) SHA1(4033f0579f0782db2157f6cbece53b0d74e61d4f) )
    ROM_LOAD16_BYTE( "chasm.u5",  0x00e000, 0x001000, CRC(e4a58b7d) SHA1(0e5f948cd110804e6119fafb4e3fa5904dd1390f) )
    ROM_LOAD16_BYTE( "chasm.u13", 0x00e001, 0x001000, CRC(877e849c) SHA1(bdeb97fcb7488e7f0866dd651204c362d2ec9f4f) )

    ROM_REGION( 0x10000, "audiocpu", 0 )
    ROM_LOAD( "2732.bin", 0x0000, 0x1000, CRC(715adc4a) SHA1(426be4f3334ef7f2e8eb4d533e64276c30812aa3) )

    ROM_REGION( 0x0200, "plds", 0 )
    ROM_LOAD( "pal12l6.u76", 0x0000, 0x0034, CRC(a30e02b7) SHA1(572f6d3f03e559f12e3bd5e087d7680ac69e9182) )
    ROM_LOAD( "pal12l6.u77", 0x0100, 0x0034, CRC(458b9cdb) SHA1(a3bff56d805f6dc494d294f079c3580430acf317) )
ROM_END     
     
     */
}

void loadQB3()
{
    unsigned char romLoad[0x8000];
    int r = 0;
    r += loadFromZip ("roms/qb3.zip", "qb3_le_t7.bin", &romLoad[0x0000]);
    r += loadFromZip ("roms/qb3.zip", "qb3_lo_p7.bin", &romLoad[0x2000]);
    r += loadFromZip ("roms/qb3.zip", "qb3_ue_u7.bin", &romLoad[0x4000]);
    r += loadFromZip ("roms/qb3.zip", "qb3_uo_r7.bin", &romLoad[0x6000]);
    uint32_t crc;
    crc = 0;printf("CRC32 qb3_le_t7.bin: %08X\r\n", crc32(&romLoad[0x0000], 0x2000, &crc ));
    crc = 0;printf("CRC32 qb3_lo_p7.bin: %08X\r\n",  crc32(&romLoad[0x2000], 0x2000, &crc ));
    crc = 0;printf("CRC32 qb3_ue_u7.bin: %08X\r\n",  crc32(&romLoad[0x4000], 0x2000, &crc ));
    crc = 0;printf("CRC32 qb3_uo_r7.bin: %08X\r\n", crc32(&romLoad[0x6000], 0x2000, &crc ));
    
    crc = 0;if (crc32(&romLoad[0x0000], 0x2000, &crc ) != 0xADAAEE4C) r -=1;
    crc = 0;if (crc32(&romLoad[0x2000], 0x2000, &crc ) != 0x72F6199F) r -=1;
    crc = 0;if (crc32(&romLoad[0x4000], 0x2000, &crc ) != 0x050A996D) r -=1;
    crc = 0;if (crc32(&romLoad[0x6000], 0x2000, &crc ) != 0x33FA77A2) r -=1;
    
    
    for (int i=0; i<0x2000; i++)
    {
      rom[i*2] = romLoad[0x0000+i];
      rom[1+i*2] = romLoad[0x2000+i];
      
      rom[0x4000+i*2] = romLoad[0x4000+0x0000+i];
      rom[0x4000+1+i*2] = romLoad[0x4000+0x2000+i];
    }
    if (r != 0)
    {
        v_message("TROUBLE WITH QB3 ROMS!");
    }
}

int main (int argc, char **argv)
{
   int Rotate;
   int Flip_X;
   int Flip_Y;
   int Switches;
   int Inputs;
   int MemSize=0;
   int JMI=0;
//   char *RomImages[8];
   static int parity = 0;

   Game = 0; useParity = 0;

   vectrexinit (1);
   v_init ();
   usePipeline = 1;

   char *_argv[3];
   _argv[0] = "cine.img";
   _argv[1] = getLoadParameter (0);
   _argv[2] = NULL;
   argc = 2;
   argv = _argv;
   printf ("Game selected: %s\r\n", _argv[1]);

   static char hdr[] =
    "CINEMU - Cinematronics Emulator. " VERSION "\n"
    "(c) Copyright 1997, Zonn Moore.  All rights reserved.\n";

   if ((argc == 2) && (strcmp(argv[1], "-V") == 0)) {
     fputs (hdr, stderr);
     exit(0);
   }

   if ((argc == 3) && (strcmp(argv[1], "-d") == 0)) {
     argc -= 1; argv += 1; ccpudebug = 1;
   }

   if ((argc != 2) || (strcmp(argv[1], "-h") == 0)) {
      fprintf (stderr, "syntax: cinemu [-h | -V | [-d] <game>]  where the supported games are:\n");
      fprintf (stderr, "  armorattack  boxingbugs   demon  ripoff      spacewars   starcastle  sundance    waroftheworlds\n");
      fprintf (stderr, "  barrier      cosmicchasm  qb3    solarquest  speedfreak  starhawk    tailgunner  warrior\n");
      fprintf (stderr, "\n");
      exit (1);
   }
   /*
   * Games known to work 100% or nearly so, at some point or another of
   * the emulators development (but not necessarily right now :):
   * RipOff, Solar Quest, Spacewar, Speed Freak, Star Castle, Star Hawk,
   * Tail Gunner, War of the Worlds, Warrior
   *
   * For reference, all of the cinematronics games are:
   * Armor Attack, Barrier, Boxing Bugs, Demon, Ripoff, Solar Quest,
   * Spacewar, Speed Freak, Star Castle, Star Hawk, Sundance, Tail Gunner
   * War of the worlds, Warrior
   */

   // It's looking like all of the cinematronics games need the 'parity' tweak to handle 2 consecutive WAI instructions.
   // If that turns out to be correct, we can probably just remove all the special handling...

   Rotate = No; Flip_X = No; Flip_Y = No; Switches = 0; Inputs = 0xFFFF; vgSetTwinkle(255); useParity=1;

   if (strcasecmp (argv[1], "tailgunner") == 0) 
   {
     Game = GAME_TAILGUNNER;
     Rotate=Yes; Flip_X=Yes; Flip_Y=Yes;
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=1;
     JMI=No;
     Switches=0b0000000;
     Inputs=0xBFFF; // 8 way digital joystick mode
     vgSetCineSize(0,0,1024,768); // MinX, MinY, MaxX, MaxY
     //  Monitor=BiLevel
     vgSetTwinkle(7);
     loadTailgunner();
   } else if (strcasecmp (argv[1], "ripoff") == 0) {
     Game = GAME_RIPOFF;
     Rotate=Yes; Flip_X=Yes; Flip_Y=Yes;
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=1;
     JMI=Yes;
     /*
DIP_START(ripoff_dips)
 DIP_SET4(DIPSW1,0xfc,"Units",0x00,"8",0x01,"16",0x02,"4",0x03,"12",3)
 DIP_SET4(DIPSW1,0xf3,"Coins",0x08,"2Coins 1Credit",0x00,"4Coins 3Credits",0x0c,"1Coin 1Credit",0x04,"2Coins 3Credits",2)
 DIP_SET2(DIPSW1,0xef,"Attract Sound",0x10,"Off",0x00,"On",0)
 DIP_SET2(DIPSW1,0xdf,"Scores",0x20,"Individual",0x00,"Combined",0)
 DIP_SET2(DIPSW1,0xbf,"Diagnostic",0x00,"Off",0x40,"On",0)
DIP_END
      */
     Switches=0b1011101;
     //         ^ trying 1 to remove diagnostic frame
     vgSetCineSize(-16,-16,1041,785); // MinX, MinY, MaxX, MaxY
     // Monitor=BiLevel
     loadRipoff();
   } else if (strcasecmp (argv[1], "spacewars") == 0) {
     Game = GAME_SPACEWARS;
     Rotate=Yes; Flip_X=Yes; Flip_Y=Yes;
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=0;
     JMI=No;
     Switches=0b0000011;
     vgSetCineSize(0,0,1024,768); // MinX, MinY, MaxX, MaxY
     // Monitor=BiLevel
     vgSetTwinkle(9);
     loadSpaceWar();
   } else if (strcasecmp (argv[1], "boxingbugs") == 0) {
     Game = GAME_BOXINGBUGS;
     Rotate=Yes; Flip_X=Yes; Flip_Y=Yes;
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=3;
     JMI=Yes;
     // Mouse=BoxingBugs
     // MouseSpeedX=1024	; use negative number to reverse mouse control
     Switches=0b1101100;
     //         ^ that '1' makes it play the game as opposed to a diag mode
     vgSetCineSize(-1,0,1026,784); // MinX, MinY, MaxX, MaxY
     // Monitor=Color
     loadBoxingBugs();
   } else if (strcasecmp (argv[1], "armorattack") == 0) {
     Game = GAME_ARMORATTACK;
     Rotate=Yes; Flip_X=Yes; Flip_Y=Yes;
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=2;
     JMI=Yes;
     Switches=0b1000000; // diagnostics, no sound, 1 credit per quarter, 5 jeeps per game ???
     vgSetCineSize(2,2,1023,767); // MinX, MinY, MaxX, MaxY
     // Monitor=BiLevel
     loadArmorAttack();
   } else if (strcasecmp (argv[1], "starcastle") == 0) {
     Game = GAME_STARCASTLE;
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=1;
     JMI=Yes;
     Switches=0b1000011;
     vgSetCineSize(-9,3,1033,795); // MinX, MinY, MaxX, MaxY
     // Monitor=BiLevel
     vgSetTwinkle(8);
     loadStarCastle();
   } else if (strcasecmp (argv[1], "starhawk") == 0) {
     Game = GAME_STARHAWK;
     Rotate=Yes; Flip_X=Yes; Flip_Y=Yes;
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=0;
     JMI=Yes;
     vgSetCineSize(20+20,-16,1130-20,750);
     // Monitor=BiLevel
     vgSetTwinkle(9);
     loadStarHawk();
   } else if (strcasecmp (argv[1], "speedfreak") == 0) {
     Game = GAME_SPEEDFREAK;
     Rotate=Yes; Flip_X=Yes; Flip_Y=Yes;
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=1;
     JMI=No;
     // Mouse=SpeedFreak
     // MouseSpeedX=1536
     Inputs=0xFFE0; // zero steering wheel, start in 1st gear
     vgSetCineSize(-9,3,1033,795); // MinX, MinY, MaxX, MaxY
     
     vgSetCineSize(-300,-250,1300,700); // MinX, MinY, MaxX, MaxY
     vgSetCineSize(000,000,1000,750); // MinX, MinY, MaxX, MaxY
//x -300 - + 1300
//y - 250 700
     

      // special vectrex  interface settings for Bzone
      keepDotsTogether = 1;

     
     // Monitor=BiLevel
     vgSetTwinkle(8);
     //Brightness=100,100,100		; RGB percentages for brightest level
     //Contrast=67,67,67		; RGB percentages for dimest level
     loadSpeedFreak();
   } else if (strcasecmp (argv[1], "demon") == 0) {
     Game = GAME_DEMON;
     Rotate=Yes; Flip_X=Yes; Flip_Y=Yes;
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=2;
     JMI=Yes;
     Switches=0b1000000;
     vgSetCineSize(7,7,1021,805); // MinX, MinY, MaxX, MaxY
     // Monitor=BiLevel
     vgSetTwinkle(8);
     loadDemon();
   } else if (strcasecmp (argv[1], "solarquest") == 0) {
     Game = GAME_SOLARQUEST;
     //if (settings->orientation == 0) {
       Rotate=Yes; Flip_Y=Yes; // hardware correction in X
       //}
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=2;
     JMI=Yes;
     //Switches=0b1111000;
     Switches=0;
     vgSetCineSize(0,0,1020,768); // MinX, MinY, MaxX, MaxY
     // Monitor=64Level
     loadSolarQuest();
   } else if (strcasecmp (argv[1], "cosmicchasm") == 0) {
     Game = GAME_COSMICCHASM;
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=3; // this may be a 64k rom
     JMI=Yes;
     // Mouse=BoxingBugs
     // MouseSpeedX=1024	; use negative number to reverse mouse control
     Switches=0b0101100;
     vgSetCineSize(-1,0,1026,784); // MinX, MinY, MaxX, MaxY
     // Monitor=Color
     loadCosmicChasm();
   } else if (strcasecmp (argv[1], "waroftheworlds") == 0) {
     Game = GAME_WAROFTHEWORLDS;
     Rotate=Yes; Flip_X=Yes; Flip_Y=Yes;
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=2;
     JMI=Yes;
     //ORIG: Switches=0b0100000; // free play
     Switches=0b01000000; // debugging diag mode startup
     vgSetCineSize(-27,0,1060,768); // MinX, MinY, MaxX, MaxY
     loadWotw();
     // Monitor=Color
   } else if (strcasecmp (argv[1], "warrior") == 0) {
     Game = GAME_WARRIOR;
     Rotate=Yes; Flip_X=Yes; Flip_Y=Yes;
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=1;
     JMI=Yes;
     //Switches=0b1111111;
     Switches=0b00000000;
     vgSetCineSize(0,0,1024,768); // MinX, MinY, MaxX, MaxY
     // Monitor=BiLevel
     loadWarrior();
   } else if (strcasecmp (argv[1], "barrier") == 0) {
     Game = GAME_BARRIER;
     //Rotate=Yes;
     //if (settings->orientation == 0) {
       Flip_X=Yes; Flip_Y=Yes;
       //}
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=0;
     JMI=Yes;
     Switches=0b0000001;
     vgSetCineSize(67,74,981,693); // MinX, MinY, MaxX, MaxY
     vgSetCineSize(0,74,730,1024); // MinX, MinY, MaxX, MaxY
     // Monitor=BiLevel
     loadBarrier();
   } else if (strcasecmp (argv[1], "sundance") == 0) {
     Game = GAME_SUNDANCE;
     //Rotate=Yes;
     //if (settings->orientation != 0) {
       Flip_X=Yes; Flip_Y=Yes;
       //}
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=1;
     JMI=Yes;
     Switches=0b0000101;
     vgSetCineSize(0,0,1024,768); // MinX, MinY, MaxX, MaxY
     vgSetCineSize(0,0,800,1024); // MinX, MinY, MaxX, MaxY
     // Monitor=16Level
     vgSetTwinkle(0);
     loadSundance();
   } else if (strcasecmp (argv[1], "qb3") == 0) {
     Game = GAME_QB3;
     // 0=4k, 1=8k, 2=16k, 3=32k
     MemSize=4; // only game with 4 x 8k roms...
     JMI=No;
     // Inputs unknown for now # <input players="1" buttons="4" coins="1">
     Switches=0b1001001000010; // free play on.  Is this extended switch plausible???
     vgSetCineSize(-9,3,1033,795); // MinX, MinY, MaxX, MaxY
     // Monitor=BiLevel ; probably not.  should research this game.
     vgSetTwinkle(8);
     //Brightness=100,100,100		; RGB percentages for brightest level
     //Contrast=67,67,67		; RGB percentages for dimmest level
     loadQB3();
   } else {
      fprintf (stderr, "The supported games are:\n");
      fprintf (stderr, "  armorattack  boxingbugs   demon  ripoff      spacewars   starcastle  sundance    waroftheworlds\n");
      fprintf (stderr, "  barrier      cosmicchasm  qb3    solarquest  speedfreak  starhawk    tailgunner  warrior\n");
      fprintf (stderr, "\n");
      exit (1);
   }
   
   cineSetGame (argv[1], Game);
   cineSetJMI (JMI);		 // set JMI option
   cineSetMSize (MemSize);	 // set memory size
   cineSetRate (38);             // Ini files support "Standard" vs "Full" but not used.
   cineSetSw (Switches, Inputs); // set switches & inputs
   // standalone mode already has rotation applied...
   //if (settings->orientation == 0) {
     // By now screen is correctly rotated for horizontal mode
     // If menu orientation is 'rotated' (assume 90 anti-clockwise as per standard stand)
     // then unrotate back to vertical mode.
     Rotate = !Rotate;
     Flip_X = !Flip_X;
     Flip_Y = !Flip_Y;
   //}
   vgSetRotateFlip (Rotate, Flip_X, Flip_Y);
   vgInit ();
    v_enableSoundOut(1);
    v_enableButtons(1);
    v_enableJoystickAnalog(1,1,0,0);    
    v_setRefresh(50);
    v_setClientHz(38); // t 
    v_setupIRQHandling();

   // #################################################################################
   // ##                                                                             ##
   // ##                               EMULATE!                                      ##
   // ##                                                                             ##
   // #################################################################################
   cineInit(); 	 // setup segment pointer, reset breakpoints
   cineReset();	 // called after all initialisations complete

   startFrame(); // needs an initial call to get start values of buttons, cursor etc
                 // - it is *start* Frame after all, not *end* Frame...

   for (;;) {
     cineExec (); // run an unspecified number of CCPU instructions
     if (bNewFrame) { // If a complete frame's worth ..?
       if (useParity) {
         parity ^= 1;
         if (parity) startFrame(); // this will call game-specific I/O handler
       } else {
         startFrame();
       }
     }
   }   
   exit(0); // probably never reached.
   return 0; // definitely never reached.
}
