#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif


#include "z80/z80.h"

#include "SegaG80.h"
#include "globals.h"



extern char *gamename[];
extern int gamenum;



 int zektor_loadhi()
{
    int num=0;
    char fullpath[180];
	FILE *fd; 
    membuffer = malloc(60);
    memset(membuffer, 0, 60); 
	
	strcpy (fullpath,"hi");
	strcat (fullpath, "\\");
	strcat (fullpath,gamename[gamenum]);
	strcat (fullpath, ".aae");
	
	//zektor
	if ((memcmp(&z80.z80Base[0xC924],"\x90\x02",2) == 0) &&
			(memcmp(&z80.z80Base[0xC95C],"\x10\x00",2) == 0))	{
		
  	
	fd = fopen(fullpath, "rb");
	if(!fd) {write_to_log("HiScore file not found");return 1;}
    
	fread(membuffer, 1, 60, fd);
    memcpy(z80.z80Base + 0xC924,membuffer, 60);
	fread(membuffer, 1, 15, fd);
    memcpy(z80.z80Base + 0xCFD2,membuffer, 15);
	
	
	fclose(fd);
	free (membuffer);
	return 1;
	}
	else return 0;	// we can't load the hi scores yet 
}

 void zektor_savehi()
{
    int num=0;
    char fullpath[180];
	membuffer = malloc(75);
    
	memset(membuffer, 0, 75);
	strcpy (fullpath,"hi");
	strcat (fullpath, "\\");
	strcat (fullpath,gamename[gamenum]);
	strcat (fullpath, ".aae");
			
	memcpy(membuffer,z80.z80Base + 0xC924,60);
     memcpy(membuffer+60,z80.z80Base + 0xCFD2,15);
	save_file(fullpath, membuffer, 75);
	free (membuffer);
}


 int spacfury_loadhi()
{
    int num=0;
    char fullpath[180];
	FILE *fd; 
    membuffer = malloc(60);
    memset(membuffer, 0, 60); 
	
	strcpy (fullpath,"hi");
	strcat (fullpath, "\\");
	strcat (fullpath,gamename[gamenum]);
	strcat (fullpath, ".aae");
	
	//zektor
	if ((memcmp(&z80.z80Base[0xC924],"\x90\x02",2) == 0) &&
			(memcmp(&z80.z80Base[0xC95C],"\x10\x00",2) == 0))	{
	 
	
	fd = fopen(fullpath, "rb");
	if(!fd) {write_to_log("HiScore file not found");return 1;}
    
	fread(membuffer, 1, 60, fd);
    memcpy(z80.z80Base + 0xC924,membuffer, 60);
	fread(membuffer, 1, 30, fd);
    memcpy(z80.z80Base + 0xCFD2,membuffer, 30);
	
	
		
	fclose(fd);
	free (membuffer);
	return 1;
	}
	else return 0;	// we can't load the hi scores yet 
}

 void spacfury_savehi()
{
    int num=0;
    char fullpath[180];
	membuffer = malloc(90);
    
	memset(membuffer, 0, 90);
	strcpy (fullpath,"hi");
	strcat (fullpath, "\\");
	strcat (fullpath,gamename[gamenum]);
	strcat (fullpath, ".aae");
			
	memcpy(membuffer,z80.z80Base + 0xC924,60);
        memcpy(membuffer+60,z80.z80Base + 0xCFD2,30);
	save_file(fullpath, membuffer, 90);
	free (membuffer);
}


 int tacscan_loadhi()
{
    int num=0;
    char fullpath[180];
	FILE *fd; 
    membuffer = malloc(30);
    memset(membuffer, 0, 30); 
	
	strcpy (fullpath,"hi");
	strcat (fullpath, "\\");
	strcat (fullpath,gamename[gamenum]);
	strcat (fullpath, ".aae");
	
	//tacscan
	if ((memcmp(&z80.z80Base[0xCB44],"MCL",3) == 0) &&
		(memcmp(&z80.z80Base[0xCB95],"\x02\x03\x00",3) == 0))	{
   
	
	fd = fopen(fullpath, "rb");
	if(!fd) {write_to_log("HiScore file not found");return 1;}
    
    

	fread(membuffer, 1, 15, fd);
    memcpy(z80.z80Base + 0xCB44,membuffer,15);
	fread(membuffer, 1, 15, fd);
    memcpy(z80.z80Base + 0xCB95,membuffer,15);
	fclose(fd);
	free (membuffer);
	return 1;
	}
	else return 0;	// we can't load the hi scores yet 
}

 void tacscan_savehi()
{
    int num=0;
    char fullpath[180];
	membuffer = malloc(30);
    
	memset(membuffer, 0, 30);
	strcpy (fullpath,"hi");
	strcat (fullpath, "\\");
	strcat (fullpath,gamename[gamenum]);
	strcat (fullpath, ".aae");
			
	memcpy(membuffer,z80.z80Base + 0xCB44,15);
    memcpy(membuffer+15,z80.z80Base + 0xCB95,15);
	save_file(fullpath, membuffer, 30);
	free (membuffer);
}


 int elim2_loadhi()
{
    int num=0;
    char fullpath[180];
	FILE *fd; 
    membuffer = malloc(60);
    memset(membuffer, 0, 60); 
	
	strcpy (fullpath,"hi");
	strcat (fullpath, "\\");
	strcat (fullpath,gamename[gamenum]);
	strcat (fullpath, ".aae");
	
	//zektor
	if ((memcmp(&z80.z80Base[0xC99F],"\x0C\x0B\x07",3) == 0) &&
		(memcmp(&z80.z80Base[0xC9BA],"\x0A\x08\x03",3) == 0))	{
            				
	fd = fopen(fullpath, "rb");
	if(!fd) {write_to_log("HiScore file not found");return 1;}
    
	fread(membuffer, 1, 60, fd);
    memcpy(z80.z80Base + 0xC924,membuffer, 60);
	fread(membuffer, 1, 30, fd); 
    memcpy(z80.z80Base + 0xC99F,membuffer, 30);
	fclose(fd);
	free (membuffer);
	return 1;
	}
	else return 0;	// we can't load the hi scores yet 
}

 void elim2_savehi()
{
    int num=0;
    char fullpath[180];
	membuffer = malloc(90);
    
	memset(membuffer, 0, 90);
	strcpy (fullpath,"hi");
	strcat (fullpath, "\\");
	strcat (fullpath,gamename[gamenum]);
	strcat (fullpath, ".aae");
			
	memcpy(membuffer,z80.z80Base + 0xC924,60);
    memcpy(membuffer+60,z80.z80Base + 0xC99F,30);
	save_file(fullpath, membuffer, 90);
	free (membuffer);
}


 int startrek_loadhi()
{
    int num=0;
    char fullpath[180];
	FILE *fd; 
    membuffer = malloc(0x3f);
    memset(membuffer, 0, 0x3f); 
	
	strcpy (fullpath,"hi");
	strcat (fullpath, "\\");
	strcat (fullpath,gamename[gamenum]);
	strcat (fullpath, ".aae");
	
	//STARTREK
	if ((memcmp(&z80.z80Base[0xC98B],"SLP",3) == 0) &&
		(memcmp(&z80.z80Base[0xC910],"\x25\x06\x09",3) == 0)) {
	   
	
	fd = fopen(fullpath, "rb");
	if(!fd) {write_to_log("HiScore file not found");return 1;}
    
	fread(membuffer, 1, 0x1B, fd);
    memcpy(z80.z80Base + 0xC98B,membuffer, 0x1B);
	fread(membuffer, 1, 0x24, fd);
    memcpy(z80.z80Base + 0xC910,membuffer, 0x24);          
			                                      
	fclose(fd);
	free (membuffer);
	return 1;
	}
	else return 0;	// we can't load the hi scores yet 
}

 void startrek_savehi()
{
    int num=0;
    char fullpath[180];
	membuffer = malloc(0x3f);
    
	memset(membuffer, 0, 0x3f);
	strcpy (fullpath,"hi");
	strcat (fullpath, "\\");
	strcat (fullpath,gamename[gamenum]);
	strcat (fullpath, ".aae");
			
	memcpy(membuffer,z80.z80Base + 0xC98B,0x1B);
        memcpy(membuffer+0x1b,z80.z80Base + 0xC910,0x24);
	save_file(fullpath, membuffer, 0x3f);
	free (membuffer);
}


int check_sega_hi(int gamenum)
{
   int ret;

 switch (gamenum)
  {
  case 38: ret=spacfury_loadhi();break;
 // case 41: ret=tacscan_loadhi();break;
  case 42: ret=zektor_loadhi();break;
  case 43: ret=startrek_loadhi();break;
  case 44: ret=elim2_loadhi();break;
  case 45: ret=spacfury_loadhi();break;
  }
  
 return ret;
}
void save_sega_hi()
{
   
 switch (gamenum) 
  {
  case 38: spacfury_savehi();break;
  //case 41: tacscan_savehi();break;
  case 42: zektor_savehi();break;
  case 43: startrek_savehi();break;
  case 44: elim2_savehi();break;
  case 45: spacfury_savehi();break;
  }
  
}













/***************************************************************************

  Hi Score Routines

**************************************************************************

 int spacfury_hiload(void)
{
	unsigned char *RAM = Machine->memory_region[Machine->drv->cpu[0].memory_region];


	
	if ((memcmp(&RAM[0xC924],"\x90\x02",2) == 0) &&
			(memcmp(&RAM[0xC95C],"\x10\x00",2) == 0))
	{
		void *f;

		if ((f = osd_fopen(Machine->gamedrv->name,0,OSD_FILETYPE_HIGHSCORE,0)) != 0)
		{
			osd_fread(f,&RAM[0xC924],2*30); 
			osd_fread(f,&RAM[0xCFD2],3*10);   

			osd_fclose(f);
		}
		return 1;
	}
	else return 0; 
}


 void spacfury_hisave(void)
{
	void *f;
	unsigned char *RAM = Machine->memory_region[Machine->drv->cpu[0].memory_region];


	if ((f = osd_fopen(Machine->gamedrv->name,0,OSD_FILETYPE_HIGHSCORE,1)) != 0)
	{
		osd_fwrite(f,&RAM[0xC924],2*30);
		osd_fwrite(f,&RAM[0xCFD2],3*10); 
		osd_fclose(f);
	}
}


 int zektor_hiload(void)
{
	unsigned char *RAM = Machine->memory_region[Machine->drv->cpu[0].memory_region];


	
	if ((memcmp(&RAM[0xC924],"\x90\x02",2) == 0) &&
			(memcmp(&RAM[0xC95C],"\x10\x00",2) == 0))
	{
		void *f;

		if ((f = osd_fopen(Machine->gamedrv->name,0,OSD_FILETYPE_HIGHSCORE,0)) != 0)
		{
			osd_fread(f,&RAM[0xC924],2*30); 
			osd_fread(f,&RAM[0xCFD2],3*5);   
			osd_fclose(f);
		}
		return 1;
	}
	else return 0; 
}


 void zektor_hisave(void)
{
	void *f;
	unsigned char *RAM = Machine->memory_region[Machine->drv->cpu[0].memory_region];


	if ((f = osd_fopen(Machine->gamedrv->name,0,OSD_FILETYPE_HIGHSCORE,1)) != 0)
	{
		osd_fwrite(f,&RAM[0xC924],2*30); 
		osd_fwrite(f,&RAM[0xCFD2],3*5); 
		osd_fclose(f);
	}
}

 int tacscan_hiload(void)
{
	unsigned char *RAM = Machine->memory_region[Machine->drv->cpu[0].memory_region];


	
	if ((memcmp(&RAM[0xCB44],"MCL",3) == 0) &&
		(memcmp(&RAM[0xCB95],"\x02\x03\x00",3) == 0))
	{
		void *f;

		if ((f = osd_fopen(Machine->gamedrv->name,0,OSD_FILETYPE_HIGHSCORE,0)) != 0)
		{
			osd_fread(f,&RAM[0xCB44],3*5); 
			osd_fread(f,&RAM[0xCB95],3*5); 
			osd_fclose(f);
		}

		return 1;
	}
	else return 0;  
}


 void tacscan_hisave(void)
{
	void *f;
	unsigned char *RAM = Machine->memory_region[Machine->drv->cpu[0].memory_region];


	if ((f = osd_fopen(Machine->gamedrv->name,0,OSD_FILETYPE_HIGHSCORE,1)) != 0)
	{
		osd_fwrite(f,&RAM[0xCB44],3*5);  
		osd_fwrite(f,&RAM[0xCB95],3*5);  
		osd_fclose(f);
	}
}


 int elim2_hiload(void)
{
	unsigned char *RAM = Machine->memory_region[Machine->drv->cpu[0].memory_region];


	
	if ((memcmp(&RAM[0xC99F],"\x0C\x0B\x07",3) == 0) &&
			(memcmp(&RAM[0xC9BA],"\x0A\x08\x03",3) == 0))
	{
		void *f;

		if ((f = osd_fopen(Machine->gamedrv->name,0,OSD_FILETYPE_HIGHSCORE,0)) != 0)
		{
			osd_fread(f,&RAM[0xC924],2*30); 
			osd_fread(f,&RAM[0xC99F],3*10); 

			osd_fclose(f);
		}

		return 1;
	}
	else return 0; 
}


 void elim2_hisave(void)
{
	void *f;
	unsigned char *RAM = Machine->memory_region[Machine->drv->cpu[0].memory_region];


	if ((f = osd_fopen(Machine->gamedrv->name,0,OSD_FILETYPE_HIGHSCORE,1)) != 0)
	{
		osd_fwrite(f,&RAM[0xC924],2*30);
		osd_fwrite(f,&RAM[0xC99F],3*10); 
		osd_fclose(f);
	}
}

 int elim4_hiload(void)
{
	unsigned char *RAM = Machine->memory_region[Machine->drv->cpu[0].memory_region];


	
	if ((memcmp(&RAM[0xCC4D],"\x0C\x0B\x07",3) == 0) &&
			(memcmp(&RAM[0xCC68],"\x0A\x08\x03",3) == 0))
	{
		void *f;

		if ((f = osd_fopen(Machine->gamedrv->name,0,OSD_FILETYPE_HIGHSCORE,0)) != 0)
		{
			osd_fread(f,&RAM[0xC928],2*30);  
			osd_fread(f,&RAM[0xCC4D],3*10);  

			osd_fclose(f);
		}

		return 1;
	}
	else return 0;  
}


 void elim4_hisave(void)
{
	void *f;
	unsigned char *RAM = Machine->memory_region[Machine->drv->cpu[0].memory_region];


	if ((f = osd_fopen(Machine->gamedrv->name,0,OSD_FILETYPE_HIGHSCORE,1)) != 0)
	{
		osd_fwrite(f,&RAM[0xC928],2*30); 
		osd_fwrite(f,&RAM[0xCC4D],3*10); 
		osd_fclose(f);
	}
}

 int startrek_hiload(void)
{
	unsigned char *RAM = Machine->memory_region[Machine->drv->cpu[0].memory_region];


	
	if ((memcmp(&RAM[0xC98B],"SLP",3) == 0) &&
			(memcmp(&RAM[0xC910],"\x25\x06\x09",3) == 0))
	{
		void *f;

		if ((f = osd_fopen(Machine->gamedrv->name,0,OSD_FILETYPE_HIGHSCORE,0)) != 0)
		{
			osd_fread(f,&RAM[0xC98B],0x1B);
			osd_fread(f,&RAM[0xC910],0x24);
			// osd_fread(f,&RAM[0xC98B],0xF0); 

			osd_fclose(f);
		}

		return 1;
	}
	else return 0;  
}


 void startrek_hisave(void)
{
	void *f;
	unsigned char *RAM = Machine->memory_region[Machine->drv->cpu[0].memory_region];


	if ((f = osd_fopen(Machine->gamedrv->name,0,OSD_FILETYPE_HIGHSCORE,1)) != 0)
	{
		osd_fwrite(f,&RAM[0xC98B],0x1B);
		osd_fwrite(f,&RAM[0xC910],0x24);
		osd_fclose(f);
	}
}

*/
