
#include <stdio.h>
#include <string.h>
#include "earom.h"
#include "globals.h"
#include "log.h"

static int earom_offset=0;
static int earom_data=0;
static int earom[0x3f];

extern int gamenum;

UINT8 EaromRead (UINT32 address, struct MemoryReadByte *psMemRead)
{
	//log_it("Earom Read? address %x data %d", address,earom_data);
	return (earom_data);
}

void EaromWrite(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite)
{
	earom_offset = (address & 0x00ff) ;
	if (gamenum==REDBARON) {earom_offset-=0x20;}
	if (gamenum==GRAVITAR || gamenum==BWIDOW){earom_offset-=0x40;}
	//log_it("Earom write? address %x data %d", earom_offset,data);
	//if (earom_offset > 0x3f){earom_offset-=0x40;} //gravitar & bwidow
	earom_data = data;
	//log_it("Earom write? address %x data %d", earom_offset,data);
}

/* 0,8 and 14 get written to this location, too.
 * Don't know what they do exactly
 */
void EaromCtrl(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite)
{
	
	/*
		0x01 = clock
		0x02 = set data latch? - writes only (not always)
		0x04 = write mode? - writes only
		0x08 = set addr latch?
	*/
   
	//if (earom_offset > 0x3f){earom_offset-=0x40;} //grav&bwidow

	if (data & 0x01)
		earom_data = earom[earom_offset];
	if ((data & 0x0c) == 0x0c)
	{
		earom[earom_offset]=earom_data;
		
	}
	//log_it("Earom Control Write: address %x data %d", address,data);
}


void LoadEarom()
{

  FILE *fp;
  char c;
  int i=0;
  char fullpath[180];
	
	strcpy (fullpath, "hi\\");
    strcat (fullpath,driver[gamenum].name);
	strcat (fullpath, ".aae");

   //allegro_message(" OK, fullpath is %s",fullpath);
   fp = fopen(fullpath,"rb");

 if (!fp){return;}
   else {
      do {
         c = getc(fp);    /* get one character from the file */
         earom[i]=c;         /* display it on the monitor       */
        i++;
		//log_it("Earom Read for %x",c);
	  } while (i< 62);    /* repeat until EOF (end of file)  */
   }
   fclose(fp);
 log_it("Loaded Earom Data for %s",driver[gamenum].name); 	
}

void SaveEarom(void)
{
    FILE *fp;
    int i;
    char fullpath[180];
	
	strcpy (fullpath, "hi\\");
    strcat (fullpath,driver[gamenum].name);
	strcat (fullpath, ".aae");
    //allegro_message(" OK, fullpath is %s",fullpath);  
    fp = fopen(fullpath, "wb");
      if (fp == NULL) {return;}
    for (i= 0; i < 62; i++)
		
	{fprintf(fp, "%c", earom[i]);}//allegro_message("Writing %x",earom[i&0xff]);
    
 fclose(fp);   
 log_it("Saved Earom Data for %s",driver[gamenum].name); 	
	
}

