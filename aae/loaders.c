#include <stdlib.h>
#include <string.h>
#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

#include "stdio.h"
#include "loaders.h"
#include "gameroms.h"
#include "globals.h"
#include "samples.h"
//#include "unzip.h"
#include "log.h"
#include "acommon.h"


#define ZIPNOTFOUND 5
#define ROMNOTFOUND 4
#define ROMWRONGSIZE 3
#define ROMOKAY 1
#define ROMERROR 0
char *samplepath = "samples\\";


#ifndef NO_PI
#else
#include "puff.c"
#include "puff.h"
#include "junzip.h"
#include "junzip.c"
#endif



void cpu_mem(int cpunum, int size)
{
	log_it("Allocating Game Memory, Cpu# %d Amount 0x%x",cpunum,size);
   if (GI[cpunum]){free(GI[cpunum]);GI[cpunum] = NULL;}  
   GI[cpunum] = malloc(size); 
   if (GI[cpunum] == NULL) {log_it("Can't allocate system ram for Cpu Emulation! - This is bad.");exit(1);}
   memset(GI[cpunum], 0, size);
   //log_it("Memory Allocation Completed");
}


void swap(unsigned char *src, unsigned char *dst, int length, int odd)
{
    int count;
    for(count = 0; count < length; count += 1)
        dst[(count << 1) | (odd)] = src[(count)];
}

void bswap(char *mem, int length)
{
    int i, j;
    for(i = 0; i < (length / 2); i += 1)
    {
        j = mem[i*2+0];
        mem[i*2+0] = mem[i*2+1];
        mem[i*2+1] = j;
    }
}

int load_roms(const char *archname , const struct roms p[])
{
//  int test=0;
  int skip=0;
  unsigned char zipdata[100000]; // max rom size
//  int ret = 0;
  int i,j=0;
//  int size=0;
  int cpunum=0;
  int flipnum=0;

  for(i = 0; p[i].romSize > 0; i += 1)
  {
    char *filename;
    //   Check for ROM_REGION: IF SO, decode and skip, also reset the even/odd counter:
    if (p[i].loadAddr ==0x999)
    {
      cpu_mem(p[i].loadtype,p[i].romSize);
      cpunum=p[i].loadtype;
      flipnum=0;
    }
    else
    {
      // Find the requested file, ignore case 
	  //Is it a ROM_RELOAD? Then use previous filename. This SUCKS BTW!!!!
      if (p[i].filename== (char *)-1)
      { 
	filename = p[i-1].filename;
      }
      else 
      {
	filename = p[i].filename;
	
      }

//	zipdata = malloc(p[i].romSize);
	log_it("Loading Rom: %s",filename);
	
	//This is for ROM CONTINUE Support
	if (p[i].filename == (char *)-2)
	{
	  skip=p[i-1].romSize;
	  // Malban continue???
	} 
	else 
	  skip=0;
	  
	char zip[100];
    char *p2 = zip;
    strcpy(p2, "roms/aae/");
    p2+=9;
    strcpy(p2, archname);
    p2+= strlen(archname);
    strcpy(p2, ".zip");
    
/*
    if (gamenum == LLANDER1) zip = "roms/aae/starwars.zip";
    
    if (gamenum == STARWARS) zip = "roms/aae/starwars.zip";
	if (gamenum == MHAVOC) zip = "roms/aae/mhavoc.zip";
    if (gamenum == OMEGRACE) zip = "roms/aae/omegrace.zip";
    if (gamenum == TAILG) zip = "roms/aae/tailg.zip";
    if (gamenum == SOLARQ) zip = "roms/aae/solarq.zip";
    if (gamenum == ASTEROID) zip = "roms/aae/astdelux.zip";
    if (gamenum == QUANTUM) zip = "roms/aae/quantum.zip";
    if (gamenum == STARCAS) zip = "roms/aae/starcas.zip";
    if (gamenum == ARMORA) zip = "roms/aae/armora.zip";
    if (gamenum == BARRIER) zip = "roms/aae/barrier.zip";
    if (gamenum == SUNDANCE) zip = "roms/aae/sundance.zip";
    if (gamenum == WARRIOR) zip = "roms/aae/warrior.zip";
    if (gamenum == RIPOFF) zip = "roms/aae/ripoff.zip";
    if (gamenum == STARHAWK) zip = "roms/aae/starhawk.zip";
    if (gamenum == SPACEWAR) zip = "roms/aae/spacewar.zip";
    if (gamenum == SPEEDFRK) zip = "roms/aae/speedfrk.zip";
    if (gamenum == DEMON) zip = "roms/aae/demon.zip";
    if (gamenum == BOXINGB) zip = "roms/aae/boxingb.zip";
    if (gamenum == WOTW) zip = "roms/aae/wotw.zip";
	
    if (gamenum == 1) zip = "roms/aae/mhavoc.zip";
*/    
    
    log_it("Loading from ZIP: %s",zip);
    v_printString(-40, 40, "LOADING ROMS", 5, 0x28);
    v_WaitRecal();
    
    
	if (loadFromZip (zip, filename, zipdata) != 0)
	{
	  log_it("file not found in zip,%s/%s",zip, filename);
      v_printString(-100, 40, "SOME ROMS NOT FOUND", 5, 0x28);
      v_WaitRecal();
      
	}
	else
	{
	  log_it("Loaded: %s/%s",zip, filename);
	}

	switch(p[i].loadtype)
	{
	  case 0: 
	  {
	    for (j=skip; j < p[i].romSize;j++)
	    {
	      GI[cpunum][j + p[i].loadAddr]=zipdata[j];
	    } 
	    break;
	  }
	  case 1: 
	  {
	    for(j = 0; j < p[i].romSize; j += 1) //Even odd based on flipword
	    {
	      GI[cpunum][(j*2 + flipnum)+ p[i].loadAddr] =zipdata[j];
	    }
	    break;
	  }
	  default: log_it("Something bad happened in the ROM Loading Routine");
	    break;
	}	
      }
      if (p[i].loadtype) flipnum^=1;

  } // Close the archive 
  log_it("Finished loading roms");
  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
/*  
 * archname = driver[gamenum].name
struct roms 
{
	char *filename;
	unsigned int loadAddr;
	int romSize;
	int loadtype;
};



  int test=0;
    int skip=0;
    char temppath[255]; 
    char *rompath = "roms\\";
    unsigned char *zipdata;
    int ret = 0;
    int i,j=0;
    unzFile *fd = NULL;
    unz_file_info info;
    int size=0;
    int cpunum=0;
	int flipnum=0;

	strcpy(temppath,config.exrompath);
	put_backslash(temppath);
	strcat(temppath,archname);
	strcat(temppath,".zip");
    
	test=file_exist(temppath);
	if (test==0)
	{
    strcpy(temppath,rompath);
    strcat(temppath,archname);
    strcat(temppath,".zip\0");
	}
    log_it("Opening, %s",temppath);      
    // Attempt to open the archive 
   	fd = unzOpen(temppath);
    if(!fd) {
		     log_it("Romzip not found, %s",temppath); 
			 if (in_gui==0) {allegro_message("Rom Zip File not found in path.");}
	         return (0);
			}
  //Start Loading ROMS HERE////////////////////////////////////////////////////
	//allegro_message("starting with romsize = %d romsize 1 = %d",p[0].romSize,p[1].romSize);
	for(i = 0; p[i].romSize > 0; i += 1)
	  {
     //   Check for ROM_REGION: IF SO, decode and skip, also reset the even/odd counter:
		  if (p[i].loadAddr ==0x999){cpu_mem(p[i].loadtype,p[i].romSize);cpunum=p[i].loadtype;flipnum=0;}
		  else{
			   
	            // Find the requested file, ignore case 
		        //Is it a ROM_RELOAD? Then use previous filename. This SUCKS BTW!!!!
			   if (p[i].filename== (char *)-1){ ret = unzLocateFile(fd, p[i-1].filename, 2);}
			   else {ret = unzLocateFile(fd, p[i].filename, 2);}
		    if(ret != UNZ_OK) { 
			                   log_it("file not found in zip,%s",p[i].filename);
		                       if (in_gui==0) {allegro_message("A required Rom was not found in zip.\nUse aae gamename -verifyroms to audit your romset.");}
							   return (0);
		                      }

       // Get information on the current file 
		ret = unzGetCurrentFileInfo(fd, &info, NULL, 0, NULL, 0, NULL, 0);
		if(ret != UNZ_OK) return (0);

        // Size mismatch? 
		if(p[i].romSize != info.uncompressed_size) { log_it("file in zip does not match size,%s",p[i].filename);}//return (0);}

        // Open the file for reading 
	    ret = unzOpenCurrentFile(fd);
		if(ret != UNZ_OK) return (0);
		zipdata = malloc(p[i].romSize);
        // Read (decompress) the file 
		ret = unzReadCurrentFile(fd, zipdata, info.uncompressed_size);
		if(ret != info.uncompressed_size) return (0);
		//This is just to display the ROM LOADING Message
		if (p[i].filename== (char *)-1){ log_it("Loading Rom: %s",p[i-1].filename);}  else {log_it("Loading Rom: %s",p[i].filename);}
		//This is for ROM CONTINUE Support
		if (p[i].filename== (char *)-2){skip=p[i-1].romSize;} else skip=0;

        // Close the current file 
		ret = unzCloseCurrentFile(fd);
		if(ret != UNZ_OK) return (0);
   		 
		switch(p[i].loadtype){
		case 0: {for (j=skip; j < p[i].romSize;j++){GI[cpunum][j + p[i].loadAddr]=zipdata[j];} break;}
		
		
				
		case 1: {for(j = 0; j < p[i].romSize; j += 1) //Even odd based on flipword
			    {GI[cpunum][(j*2 + flipnum)+ p[i].loadAddr] =zipdata[j];}
		      	break;}
		default: log_it("Something bad happened in the ROM Loading Routine");break;
		}	
		//Finished loading Romsa 
	    free(zipdata);
		  }
		  if (p[i].loadtype) flipnum^=1;

  } // Close the archive 
    ret = unzClose(fd);
    if(ret != UNZ_OK) return (0);
	log_it("Finished loading roms");
*/    
    return (1);
}


int load_samples(const char *p[], int val)
{
   printf("TODO SAMPLES Load!!!\n\r");
   /*
    FILE *filePtr;
    char temppath[255]; 
    //char *rompath = "samples\\";
    unsigned char *wavsize;
	unsigned char *zipdata;
	int ret = 0;
    int i=1;
    unzFile *fd = NULL;
    unz_file_info info;
    int size=0;
   

	if (strcmp(p[0],"nosamples.zip")==0){return 1;}

	strcpy(temppath,"samples\\");//if it's not there, try sample dir
	strcat(temppath,p[0]);
	strcat(temppath,"\0");
	
    log_it("Opening, %s",temppath);      
    wavsize = malloc(4); //SET
	memset(wavsize,0,4); //SET
	// Attempt to open the archive 
    
	fd = unzOpen(temppath);
    if(!fd) {log_it("Sample zip not found, %s",temppath);}
   	
	do
	{
    // Find the requested file, ignore case 
    ret = unzLocateFile(fd,(const char *) p[i], 2);
	   if(ret != UNZ_OK) { log_it("Couldn't find the sound %s.\n Will use silence for that sample.",p[i]);
							if (val==0){
								 game_sounds[i-1]=create_sample(8, 1, 22000,220);
								 memset((short *)game_sounds[(i-1)]->data,0,220);
										}
							else  { game_sounds[(num_samples+i)]=create_sample(8, 1, 22000,220);
									memset((short *)game_sounds[(num_samples+i)]->data,0,220);}
	                     }
	   else{
    // Get information on the current file 
    ret = unzGetCurrentFileInfo(fd, &info, NULL, 0, NULL, 0, NULL, 0);
	if(ret != UNZ_OK) {log_it("Sample issue");}

    // Size mismatch? 
	//if(p[i].romSize != info.uncompressed_size) { log_it("file in zip does not match size,%s",p[i]);}//return (0);}

    // Open the file for reading 
    ret = unzOpenCurrentFile(fd);
	if(ret != UNZ_OK) {log_it("file could not be read, corrupt?,%s",p[i]);}
    zipdata = malloc(info.uncompressed_size);
    // Read (decompress) the file 
    ret = unzReadCurrentFile(fd, zipdata, info.uncompressed_size);
	if(ret != info.uncompressed_size) {log_it("Bad File Size");}
  
    // Close the current file 
    ret = unzCloseCurrentFile(fd);
	if(ret != UNZ_OK) {log_it("Couldn't close file");}
    
		//log_it("Opening Sample - %s",p[i]);
		filePtr = fopen("temp.wav", "wb");
		if (!filePtr) {log_it("failed to create file %s",p[i]);return 0;}
        fwrite(zipdata,info.uncompressed_size, 1, filePtr);
		fclose(filePtr);
		
		
		if (val==0){game_sounds[(i-1)]=load_sample("temp.wav");}
		else {game_sounds[(num_samples+(i-1))]=load_sample("temp.wav");}
	    remove ("temp.wav");
		 
	    free(zipdata);
	   }
		i++;
     }while (strcmp (p[i],"NULL"));
		
     // Close the archive 
     ret = unzClose(fd);
	 if(ret != UNZ_OK) {log_it("Samples loading failure, bad with no sound...");return (0);}
	 if (val==0) {num_samples=(i-1);} else { num_samples=((num_samples+i)-1);}
	     
	 return (i-1);
*/	
 return 1;
	}

int load_hi_aae(int start,int size , int image)
{
//    int num=0;
    char fullpath[180];
	FILE *fd; 
    membuffer = malloc(size);
    memset(membuffer, 0, size); 
    
		
    strcpy (fullpath,"hi");
	strcat (fullpath, "\\");
	strcat (fullpath,driver[gamenum].name);
	strcat (fullpath, ".aae");
	
	fd = fopen(fullpath, "rb");
	if(!fd) {log_it("Hiscore / nvram file for this game not found");return 1;}
    log_it("Loading Hiscore table / nvram");
	if (fread(membuffer, 1, size, fd));
    
	memcpy((GI[image] + start),membuffer, size);
	fclose(fd);
	free (membuffer);
	return 0;
	
	

}

int save_hi_aae(int start, int size, int image)
{
//    int num=0;
    char fullpath[180];
	membuffer = malloc(size+3);
    
	memset(membuffer, 0, size);
	strcpy (fullpath,"hi");
	strcat (fullpath, "\\");
	strcat (fullpath,driver[gamenum].name);
	strcat (fullpath, ".aae");
	log_it("Saving Hiscore table / nvram");
	memcpy(membuffer,(GI[image] + start),size);
    save_file(fullpath, membuffer, size);
	free (membuffer);
	return 0;
}




int verify_rom(const char *archname , const struct roms p[], int romnum)
{
  
   printf("TODO ROM VERIFY!!!\n\r");
  
/*  
    int test=0;
	char temppath[255]; 
	char *rompath = "roms\\";
   	int ret = 0;
    
	unzFile *fd = NULL;
    unz_file_info info;
   
	strcpy(temppath,config.exrompath);
	put_backslash(temppath);
	strcat(temppath,archname);
	strcat(temppath,".zip");
    
	test=file_exist(temppath);
	if (test==0)
	{
    strcpy(temppath,rompath);
    strcat(temppath,archname);
    strcat(temppath,".zip\0");
	}
    log_it("Opening, %s",temppath);      
    
	
	fd = unzOpen(temppath);
    if(!fd) { return (ZIPNOTFOUND);}
  
			   
	    ret = unzLocateFile(fd,p[romnum].filename, 2);
			 
		if(ret != UNZ_OK) { return (ROMNOTFOUND);}

       // Get information on the current file 
		ret = unzGetCurrentFileInfo(fd, &info, NULL, 0, NULL, 0, NULL, 0);
		if(ret != UNZ_OK) return (ROMERROR);

        // Size mismatch? 
		if(p[romnum].romSize != info.uncompressed_size) { unzClose(fd); return (ROMWRONGSIZE);}

        // Open the file for reading 
	    ret = unzOpenCurrentFile(fd);
		if(ret != UNZ_OK) return (ROMERROR);
		
        // Close the current file 
		ret = unzCloseCurrentFile(fd);
		if(ret != UNZ_OK) return (ROMERROR);
   
	
        // Close the archive 
        ret = unzClose(fd);
        if(ret != UNZ_OK) return (ROMERROR);
*/	
        return (1);
	

	}


int verify_sample(const char *p[], int num)
{
        return (1);
	

	}


int file_exist(char *filename)
{
    FILE *fd = fopen(filename, "rb");
    if(!fd) return (0);
    fclose(fd);
    return (1);
}
