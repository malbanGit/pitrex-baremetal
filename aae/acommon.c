#include <math.h>
#include <string.h>
#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

#define abs(a) ((a)<0?-(a):(a))

#include "osd_cpu.h"
#include "loaders.h"
#include "mathbox.h"
#include "input.h"
#include "pokey.h"
#include "rand.h"
#include "samples.h"
#include "globals.h"
#include "log.h"

extern int errorsound;

extern int menulevel;
extern int gamenum;


int vector_timer (int deltax, int deltay)
{
	deltax = abs (deltax);
	deltay = abs (deltay);
	
	if (deltax > deltay)
		return deltax >> VEC_SHIFT;
	else
		return deltay >> VEC_SHIFT;
}

void set_aae_leds(int a,int b, int c ) 
{
}



int DSRead_1(int some)
{
return  z80dip1;
}
int DSRead_2(int some)
{
return  z80dip2;
}


UINT8 randRead (unsigned int address, struct MemoryReadByte *psMemRead)
{return randintmm(0, 255);}

void Pokey0Write(unsigned int address, UINT8 data, struct MemoryWriteByte *psMemWrite)
{chip=0;Update_pokey_sound( address,data,chip,gain);if (data) GI[CPU0][address] = data;} //Dont overwrite dips

void Pokey1Write(unsigned int address, UINT8 data, struct MemoryWriteByte *psMemWrite)
{chip=1;Update_pokey_sound( address,data,chip,gain);if (data) GI[CPU0][address] = data;} //Dont overwrite dips

void NoWrite(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite)
{;}

void playstreamedsample(int channel,signed char *data,int len,int vol)
{
  printf("playstreamedsample()\n\r");
  /*
    int i;
//	int samples;
	unsigned char *p = get_audio_stream_buffer(stream);
    
	if(!p) return;
	if(channel) return;
  //Streams[channel].bUpdated=TRUE;
  //Streams[channel].nVol=vol;
	
      for (i=0; i<(len); i++) {
	          
				p[i] = data[i];    
	           
			((short *)p)[i] ^= 0x8000;

			}

	
		   
	

	// free_audio_stream_buffer(stream);	
	
	//if (p){memcpy( p , data,len);
	free_audio_stream_buffer(stream);
*/	
}

void do_AYsound(void)
{
  printf("playstreamedsample()\n\r");
  /*
   int i;
   
    signed char *p = get_audio_stream_buffer(stream);
        
         if (p) 
		 {  //AYUpdate();
		   
	        for (i=0; i<(1500); i++) {
	          
				p[i] = aybuffer[i];    
	           
			 ((short *)p)[i] ^= 0x8000;
//p[i] ^= 0x8000;
			}
		  free_audio_stream_buffer(stream);
		 }
*/   
}

void center_window(void)
{
}

void Set_ForeGround(void)
{
}

int load_state6502()
{
   	FILE *fd;
	char fullpath[40];

	strcpy (fullpath,"sta\\");
	strcat (fullpath,"aae");
	strcat (fullpath,driver[gamenum].name);
	strcat (fullpath,".sav");
	fd = fopen(fullpath, "rb");
    
	if(!fd) return (0);
   /*
    m6502zpGetContext(psCpu1);
	psCpu1->irqPending = fgetc( fd); 
	psCpu1->m6502af=fgetc(fd); 
	psCpu1->m6502pc=fgetc(fd); 
	psCpu1->m6502s=fgetc(fd); 
	psCpu1->m6502x=fgetc(fd);
	psCpu1->m6502y=fgetc(fd); 
	fread(GI, 0x10000, 1, fd);
	*/
    fclose(fd);
    
	return (1);
}


int save_state6502()
{
    FILE *fd = NULL;
	char fullpath[40];

	strcpy (fullpath,"sta\\");
	strcat (fullpath,"aae");
	strcat (fullpath,driver[gamenum].name);
    strcat (fullpath,".sav");
	if(!(fd = fopen(fullpath, "wb"))) return (0);
    /*
	m6502zpGetContext(psCpu1);
	
	fputc(psCpu1->irqPending, fd); 
	fputc(psCpu1->m6502af, fd); 
	fputc(psCpu1->m6502pc, fd); 
	fputc(psCpu1->m6502s, fd); 
	fputc(psCpu1->m6502x, fd);
	fputc(psCpu1->m6502y, fd); 
	fwrite(GI, 0x10000, 1, fd);
	*/
    fclose(fd);
    return (1);
}


int load_file(char *filename, unsigned char *buf, int size)
{
    FILE *fd = fopen(filename, "rb");
    
    printf("loading save file");
    if(!fd) printf("Hi Score file for game:%s not found",driver[gamenum].name);return (0);
    if (fread(membuffer, size, 1, fd)) ;
    fclose(fd);
    return (1);
}


int save_file(char *filename, unsigned char *buf, int size)
{
    FILE *fd = NULL;
    printf("Writing save file");
    if(!(fd = fopen(filename, "wb"))) {printf("Saving Ram failed!"); return (0);}
    fwrite(buf, size, 1, fd);
    fclose(fd);
    return (1);
}

int save_file_char(char *filename, unsigned char *buf, int size)
{
    FILE *fd = NULL;
	if(!(fd = fopen(filename, "w"))) {printf("Saving Ram failed!"); return (0);}
    fwrite(buf, size, 1, fd);
    fclose(fd);
    return (1);
}
void sanity_check_config(void)
{
    //SANITY CHECKS GO HERE
	if (config.prescale <  1 || config.prescale > 2) 
	{ printf("!!!!!Raster prescale set to unsupported value, supported values are 1 and 2!!!!!");
	  config.prescale=2;have_error=3;
	}
    
	
	
	if (config.anisfilter < 2 || config.anisfilter > 16 ||  (config.anisfilter%2 != 0) )
	{
		if (config.anisfilter !=0) { //FINAl CHECK 
	    printf("!!!!!Ansitropic Filterings set to unsupported value, supported values are 2,4,8,16 !!!!!");
		have_error=3;
		config.anisfilter =0; //RESET TIL FIXED
		}
	}

   if (config.priority < 0 || config.priority > 4) 
	{ printf("!!!!!Priority set to unsupported value, supported values are 0,1,2,3,4 - defaulted to 1!!!!!");
      config.priority=1;
	}
}


void msg_loop(void)
{

  
}

void video_loop(void)
{
}

void return_to_menu(void)
{
free_samples(); //Free and allocated Samples
//free_game_textures(); //Free textures
printf("Done Freeing All"); //Log it.
gamenum=0; //Set gamenum to zero (menu)
done=0; //Set done false
//init_keys(0)
//reset_to_default_keys(); //Reset to menu keys, custom or not.
gamefps=60;
}


void setup_ambient(int style)
{

}






