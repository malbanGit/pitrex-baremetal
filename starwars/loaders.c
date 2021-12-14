#include <stdlib.h>
#include <string.h>
#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

#include "stdio.h"
#include "loaders.h"
#include "globals.h"
#include "log.h"


#define ZIPNOTFOUND 5
#define ROMNOTFOUND 4
#define ROMWRONGSIZE 3
#define ROMOKAY 1
#define ROMERROR 0

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
  int skip=0;
  unsigned char zipdata[100000]; // max rom size
  int i,j=0;
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
    return (1);
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

int file_exist(char *filename)
{
    FILE *fd = fopen(filename, "rb");
    if(!fd) return (0);
    fclose(fd);
    return (1);
}
