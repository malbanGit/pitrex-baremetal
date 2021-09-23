#include <string.h>
#include <stdlib.h>

#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif


#define SIXTEEN 1


#include "quantum.h"
#include "globals.h"
#include "log.h"
#include "samples.h"
#include "vector.h"
//#include "glcode.h"

#include "input.h"
#include "cpu_control.h"
#include "pokey.h"
#include "earom.h"
#include "sndhrdw/pokyintf.h"
#include "math.h"
#include "aae_avg.h"
//#include "dxinput.h"
#include "acommon.h"


#ifndef INLINE
#define INLINE  __inline
#endif

//#define BYTESWAP(x) ((((uint16_t)(x))>>8) | (((uint16_t)(x))<<8))


static unsigned char program_rom[0x14000];
static unsigned char main_ram[0x4fff];
unsigned char nv_ram[0x200];  



static struct POKEYinterface pokey_interface =
{
	2,			/* 2 chips */
	600000,
	200,	/* volume */
	6, //POKEY_DEFAULT_GAIN/2
	NO_CLIP,
	/* The 8 pot handlers */
	/* The 8 pot handlers */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	/* The allpot handler */
	{ DSRead_1, DSRead_2 },
};


 void QSaveScore(void)
{
	/*
	char temppath[255]; 
	FILE *fp;
    int i;
   
	strcpy(temppath,"hi\\");
	strcat(temppath,driver[gamenum].name);
	strcat(temppath,".aae");
    //log_it("TEMPPATH SAVING is %s",temppath);
    fp = fopen(temppath, "w");
    if (fp == NULL) {log_it("Error - I couldn't write Quantum High Score");}
    for (i= 0; i < 0x200; i++) {fprintf(fp, "%c", nv_ram[i]);}
	fclose(fp);   
	*/
}
 int QLoadScore(void)
{
    FILE *fp;
    char c;
    int i=0;
	char temppath[255]; 
    strcpy(temppath,"hi\\");
	strcat(temppath,driver[gamenum].name);
	strcat(temppath,".aae");
   // log_it("TEMPPATH LOADING is %s",temppath);
    fp = fopen(temppath,"r");
  
    if (fp == NULL) {log_it("Error - I couldn't READ Quantum High Score");}
    if (fp != NULL) 
    {
      do {
         c = getc(fp);    /* get one character from the file */
        nv_ram[i]=c;         /* display it on the monitor       */
        i++;
	 } while (i< 0x200);    /* repeat until EOF (end of file)  */
    fclose(fp);
    return 1;
	}
 else return 0;
}


static void byteswap(unsigned char *mem, int length)
{
    int i, j;
    for(i = 0; i < (length / 2); i += 1)
    {
        j = mem[i*2+0];
        mem[i*2+0] = mem[i*2+1];
        mem[i*2+1] = j;
    }
}
void my_reset_handler(void) {log_it("_____RESET CALLED _____");}



void vec_ram_w8(unsigned address, unsigned data)
{
	//log_it("Vector Ram Write Address: %x data %x  Datapart1 %x Data part2 %x",address,data,(data & 0xff00)>>8,(data & 0x00ff) );

	vec_ram[address-0x800000]=(data & 0xff);
}
void vec_ram_w16(unsigned address, unsigned data)
{
	//log_it("Vector Ram Write Address: %x data %x  Datapart1 %x Data part2 %x",address,data,(data & 0xff00)>>8,(data & 0x00ff) );
    vec_ram[address-0x800000]=(data & 0xff00)>>8;
	vec_ram[(address+1)-0x800000]=data & 0x00ff;
}
unsigned vec_ram_r8(unsigned address)
{
	int c;
	c=vec_ram[address-0x800000] & 0xff;
	//log_it("Vector Ram READ: Data Returned %x",(vec_ram[address-0x800000]<<8 | vec_ram[(address+1)-0x800000]) );
    return c;
}
unsigned vec_ram_r16(unsigned address)
{
	int c;
	c=vec_ram[address-0x800000]<<8 | vec_ram[(address+1)-0x800000];
	//log_it("Vector Ram READ: Data Returned %x",(vec_ram[address-0x800000]<<8 | vec_ram[(address+1)-0x800000]) );
    return c;
}

unsigned quantum_trackball_r8(unsigned address)
{ 
	 return ( getport(2) << 4) |  getport(5);
}
unsigned quantum_trackball_r16(unsigned address)
{ 
	 return ( getport(2) << 4) |  getport(5);
}
unsigned quantum_switches_r8(unsigned address)
{
 int c;
 c= getport(0);
// if (key[KEY_F3])   s68000reset();
 return c;
}
unsigned quantum_switches_r16(unsigned address)
{
 int c;
 c= getport(0);
// if (key[KEY_F3])   s68000reset();
 return c;
}

void quantum_led_write8(unsigned address, unsigned data)
{  	
    if (data & 0xff)
	{
		/* bits 0 and 1 are coin counters */
		
		/* bit 3 = select second trackball for cocktail mode? */

		/* bits 4 and 5 are LED controls */
	    set_aae_leds( data & 0x10,data & 0x20,0);
		/* bits 6 and 7 flip screen */
		//vector_set_flip_x (data & 0x40);
		//vector_set_flip_y (data & 0x80);
	}
}
void quantum_led_write16(unsigned address, unsigned data)
{  	
    if (data & 0xff)
	{
		/* bits 0 and 1 are coin counters */
		
		/* bit 3 = select second trackball for cocktail mode? */

		/* bits 4 and 5 are LED controls */
	    set_aae_leds( data & 0x10,data & 0x20,0);
		/* bits 6 and 7 flip screen */
		//vector_set_flip_x (data & 0x40);
		//vector_set_flip_y (data & 0x80);
	}
}
void MWA_NOP(unsigned address, unsigned data)
{ 
  //log_it("NOOP Write Address: %x Data: %x",address,data);
  
}

void UN_READ(unsigned address)
{
//	log_it("--------------------Unhandled Read, %x",address);
}

void UN_WRITE(unsigned address, unsigned data)
{
//	log_it("--------------------Unhandled Write, %x data: %x",address,data);
}

unsigned MRA_NOP(unsigned address)
{return 0x00;}//log_it("WATCHDOG read");

void watchdog_reset_w8(unsigned address, unsigned data)
{;}//log_it("watchdog reset WRITE");
void watchdog_reset_w16(unsigned address, unsigned data)
{;}//log_it("watchdog reset WRITE");
unsigned quantum_snd_read8(unsigned address)
{
	int a=0; //coin 1
    int b=0; //coin 2
	//log_it("---------------------------sound read-------------------------- ADDRESS %x",(address>>1)&0xf);
    
    switch(z80dip1)
    {
     case 0: a=0xff;break;
     case 1: break;
     case 2: a=0xff;b=0xff;break;
	 case 3: b=0xff;break;
   }

	address=(address>>1)&0xf;
	switch (address)
	{
  	 case 0: return 0x00;break; //1
	 case 1: return 0x00;break; //2
	 case 2: return 0x00;break; //3
	 case 3: return 0x00;break; //4
	 case 4: return 0x00;break; //5
	 case 5: return 0x00;break; //6
	 case 6: return a;break; //7 COINAGE 1 
	 case 7: return b;break; //8 COINAGE 2
	 case 8: return 0x00;break; //??
	 default: log_it("unhandled pokey read &x",address);
	}

return 0;
//if ( (address&0x0f) == 0x0a){return rand();}
}
unsigned quantum_snd_read16(unsigned address)
{
	int a=0; //coin 1
    int b=0; //coin 2
	//log_it("---------------------------sound read-------------------------- ADDRESS %x",(address>>1)&0xf);
    
    switch(z80dip1)
    {
     case 0: a=0xff;break;
     case 1: break;
     case 2: a=0xff;b=0xff;break;
	 case 3: b=0xff;break;
   }

	address=(address>>1)&0xf;
	switch (address)
	{
  	 case 0: return 0x00;break; //1
	 case 1: return 0x00;break; //2
	 case 2: return 0x00;break; //3
	 case 3: return 0x00;break; //4
	 case 4: return 0x00;break; //5
	 case 5: return 0x00;break; //6
	 case 6: return a;break; //7 COINAGE 1 
	 case 7: return b;break; //8 COINAGE 2
	 case 8: return 0x00;break; //??
	 default: log_it("unhandled pokey read &x",address);
	}

return 0;
//if ( (address&0x0f) == 0x0a){return rand();}
}
void quantum_snd_write8(unsigned address, unsigned data) 
{
	unsigned int data1;
	unsigned int data2;

	data1=(data & 0xff00)>>8;
	data2=data & 0x00ff;

   //log_it("SOUND ADDRESS CONVERSION %x data %x", (address>>1)&0xf,data1);
   if (address & 0x1) {
	                    pokey1_w ((address>>1)&0xf,data1 & 0xff);
					  }
   else {
	     pokey2_w ((address>>1)&0xf,data2 & 0xff);
		}
}
void quantum_snd_write16(unsigned address, unsigned data) 
{
	unsigned int data1;
	unsigned int data2;

	data1=(data & 0xff00)>>8;
	data2=data & 0x00ff;

   //log_it("SOUND ADDRESS CONVERSION %x data %x", (address>>1)&0xf,data1);
   if (address & 0x1) {
	                    pokey1_w ((address>>1)&0xf,data1 & 0xff);
					  }
   else {
	     pokey2_w ((address>>1)&0xf,data2 & 0xff);
		}
}
void quantum_colorram_w8(unsigned address, unsigned data)
{ 
	int r,g,b;
	int bit0,bit1,bit2,bit3;
	
    address=(address&0xff)>>1;
    data=data&0x00ff;
	     
	bit3 = (~data >> 3) & 1;
	bit2 = (~data >> 2) & 1;
	bit1 = (~data >> 1) & 1;
	bit0 = (~data >> 0) & 1;

	g = bit1 * 0xaa + bit0 * 0x54; //54
	b = bit2 * 0xdf;
	r = bit3 * 0xe9; //ce

	if (r>255)r=255;
	// log_it("vec color set R %d G %d B %d",r,g,b);
	vec_colors[address].r=r;
    vec_colors[address].g=g;
    vec_colors[address].b=b;
		
}
void quantum_colorram_w16(unsigned address, unsigned data)
{ 
	int r,g,b;
	int bit0,bit1,bit2,bit3;
	
    address=(address&0xff)>>1;
    data=data&0x00ff;
	     
	bit3 = (~data >> 3) & 1;
	bit2 = (~data >> 2) & 1;
	bit1 = (~data >> 1) & 1;
	bit0 = (~data >> 0) & 1;

	g = bit1 * 0xaa + bit0 * 0x54; //54
	b = bit2 * 0xdf;
	r = bit3 * 0xe9; //ce

	if (r>255)r=255;
	// log_it("vec color set R %d G %d B %d",r,g,b);
	vec_colors[address].r=r;
    vec_colors[address].g=g;
    vec_colors[address].b=b;
		
}
void avgdvg_resetQ(unsigned address, unsigned data)
{
	log_it("AVG Reset");
}
void avgdvg_goQ(unsigned address, unsigned data)
{
	//avg_go();
 }
/*
unsigned read_handler(unsigned address)
{;}
void write_handler(unsigned address, unsigned data)
{;}
*/

/*
	QUANTUM MEMORY MAP (per schem):

	000000-003FFF	ROM0
	004000-004FFF	ROM1
	008000-00BFFF	ROM2
	00C000-00FFFF	ROM3
	010000-013FFF	ROM4

	018000-01BFFF	RAM0
	01C000-01CFFF	RAM1

	940000			TRACKBALL
	948000			SWITCHES
	950000			COLORRAM
	958000			CONTROL (LED and coin control)
	960000-970000	RECALL (nvram read)
	968000			VGRST (vector reset)
	970000			VGGO (vector go)
	978000			WDCLR (watchdog)
	900000			NVRAM (nvram write)
	840000			I/OS (sound and dip switches)
	800000-801FFF	VMEM (vector display list)
	940000			I/O (shematic label really - covered above)
	900000			DTACK1
*/
/*
 struct STARSCREAM_PROGRAMREGION quantum_fetch[] = {
      {0x000000, 0x013FFF, (unsigned )program_rom - 0x000000},
      {0x018000, 0x01CFFF, (unsigned) main_ram -0x018000},
   	  {0x900000, 0x9001ff, (unsigned )nv_ram - 0x900000},
	  {-1, -1, (unsigned ) NULL}
   };
*/

struct m68000_dataregion {
	unsigned lowaddr;
	unsigned highaddr;
	void    *memorycall;
	void    *userdata;
};


struct m68000_dataregion quantum_readbyte[] =
{
	{ 0x000000, 0x013fff,  NULL, program_rom },
	{ 0x018000, 0x01cfff,  NULL,main_ram },
	{ 0x800000, 0x801fff,  vec_ram_r8,NULL},
	{ 0x840000, 0x84003f,  quantum_snd_read8,NULL },
	{ 0x900000, 0x9001ff,  NULL, nv_ram},
	{ 0x940000, 0x940001,  quantum_trackball_r8, NULL}, 
	{ 0x948000, 0x948001,  quantum_switches_r8, NULL },
	//{ 0x950000, 0x95001f,  NULL, color_ram},
	{ 0x960000, 0x9601ff,  UN_READ,NULL },
	{ 0x978000, 0x978001,  MRA_NOP,NULL },	
	//{ 0x000000, 0xfffff, UN_READ, NULL },
	 {-1, -1, NULL, NULL}
};

  
struct m68000_dataregion quantum_readword[] =
{
	{ 0x000000, 0x013fff,  NULL, program_rom },
	{ 0x018000, 0x01cfff,  NULL,main_ram },
	{ 0x800000, 0x801fff,  vec_ram_r16,NULL},
	{ 0x840000, 0x84003f,  quantum_snd_read16,NULL },
	{ 0x900000, 0x9001ff,  NULL, nv_ram},
	{ 0x940000, 0x940001,  quantum_trackball_r16, NULL}, 
	{ 0x948000, 0x948001,  quantum_switches_r16, NULL },
	//{ 0x950000, 0x95001f,  NULL, color_ram},
	{ 0x960000, 0x9601ff,  UN_READ,NULL },
	{ 0x978000, 0x978001,  MRA_NOP,NULL },	
	//{ 0x000000, 0xfffff, UN_READ, NULL },
	 {-1, -1, NULL, NULL}
};

struct m68000_dataregion quantum_writebyte[] =
{
	{ 0x000000, 0x013fff,  UN_WRITE,NULL },
	{ 0x018000, 0x01cfff,  NULL,main_ram },
	{ 0x800000, 0x801fff,  vec_ram_w8,NULL},
	{ 0x840000, 0x84003f,  quantum_snd_write8,NULL },
	{ 0x900000, 0x9001ff,  NULL,nv_ram },
	{ 0x950000, 0x95001f,  quantum_colorram_w8,NULL},
	{ 0x958000, 0x958001,  quantum_led_write8,NULL },
	{ 0x960000, 0x960001,  MWA_NOP,NULL },	// enable NVRAM? 
	{ 0x968000, 0x968001,  avgdvg_resetQ,NULL },
	//{ 0x970000, 0x970001,  avgdvg_goQ,NULL },
	//{ 0x970000, 0x970001, NULL,qavggo },
	//{ 0x978000, 0x978001,  watchdog_reset_w,NULL },
	// the following is wrong, but it's the only way I found to fix the service mode 
	//{ 0x978000, 0x978001,  avgdvg_goQ,NULL },
	//{ 0x000000, 0xfffff, UN_WRITE, NULL }, 
	{-1, -1, NULL, NULL}
};

struct m68000_dataregion quantum_writeword[] =
{
	{ 0x000000, 0x013fff, UN_WRITE,NULL },
	{ 0x018000, 0x01cfff,  NULL,main_ram },
	{ 0x800000, 0x801fff, vec_ram_w16,NULL},
	{ 0x840000, 0x84003f,  quantum_snd_write16,NULL },
	{ 0x900000, 0x9001ff, NULL,nv_ram },
	{ 0x950000, 0x95001f, quantum_colorram_w16, NULL},
	{ 0x958000, 0x958001, quantum_led_write16,NULL },
	{ 0x960000, 0x960001, MWA_NOP,NULL },	// enable NVRAM? 
	{ 0x968000, 0x968001, avgdvg_resetQ,NULL },
	//{ 0x970000, 0x970001,  avgdvg_goQ,NULL },
	//{ 0x978000, 0x978001, watchdog_reset_w,NULL },
	// the following is wrong, but it's the only way I found to fix the service mode 
	{ 0x978000, 0x978001,  avgdvg_goQ,NULL },
	{ 0x000000, 0xfffff, UN_WRITE, NULL },
	 {-1, -1, NULL, NULL}
};


void m68k_write_memory_8(unsigned int addr, unsigned int value)
{
  struct m68000_dataregion *psWrite = quantum_writebyte; /* Beginning of our handler */
  while (psWrite->lowaddr != ((unsigned) -1))
  {
      if ((addr >= psWrite->lowaddr) && (addr <= psWrite->highaddr))
      {
		  if (psWrite->memorycall != NULL)
		  {
	          ( (void (*)(unsigned, unsigned)) psWrite->memorycall) (addr, value);
	          return;
		  }
		  else
		  {
			  if (psWrite->userdata != NULL)
	          ((unsigned char *)psWrite->userdata)[addr-psWrite->lowaddr] = value;
	          return;
		  }
		  UN_WRITE(addr, value); // error
          return ;
      }
      ++psWrite;
  }
  UN_WRITE(addr, value); // error
  return ;
}

unsigned int m68k_read_memory_8(unsigned int addr)
{
  struct m68000_dataregion *psRead = quantum_readbyte; /* Beginning of our handler */
  // byteswap
#ifdef SIXTEEN
//  log_it("8: %i", addr);
  if (addr < 0x14000) 
  {
	  if (addr & 1) addr-=1; else addr+=1;
	  return (0xff & (((unsigned char*)psRead->userdata)[addr]));
  }
#endif
  while (psRead->lowaddr != ((unsigned) -1))
  {
      if ((addr >= psRead->lowaddr) && (addr <= psRead->highaddr))
      {
		  if (psRead->memorycall != NULL)
		  {
	          return ((unsigned (*)(unsigned))psRead->memorycall) (addr);
		  }
		  else
		  {
			  if (psRead->userdata != NULL)
				return ((unsigned char*)psRead->userdata)[addr-psRead->lowaddr];
		  }
		  UN_READ(addr); // error
          return -1;
      }
      ++psRead;
  }
  UN_READ(addr); // error
  return -1;
}

unsigned int m68k_read_memory_16(unsigned int addr)
{
  struct m68000_dataregion *psRead = quantum_readword; /* Beginning of our handler */
#ifdef SIXTEEN
  if (addr < 0x14000) 
  {
	  unsigned short *a = ((unsigned short int*)(((unsigned char*)psRead->userdata)+addr));
//	  log_it("16: %i = %i", addr, *a);
	  return *a;
  }
#endif
  while (psRead->lowaddr != ((unsigned) -1))
  {
      if ((addr >= psRead->lowaddr) && (addr <= psRead->highaddr))
      {
		  if (psRead->memorycall != NULL)
		  {
	          return ((unsigned (*)(unsigned))psRead->memorycall) (addr);
		  }
		  else
		  {
			  if (psRead->userdata != NULL)
			  {
				  int v;
 				  v = (0xff00 & (((unsigned char*)psRead->userdata)[addr-psRead->lowaddr]<<8));
				  v +=           ((unsigned char*)psRead->userdata)[addr-psRead->lowaddr+1]  & 0xff;
//log_it("16: %i = %i", addr, v);
				  return v;
			  }
		  }
		  UN_READ(addr); // error
          return -1;
      }
      ++psRead;
  }
  UN_READ(addr); // error
  return -1;
}

unsigned int m68k_read_memory_32(unsigned int addr)
{
	int v = 0;
//  log_it("32: %i", addr);
	v = m68k_read_memory_16(addr)<<16;
	v += m68k_read_memory_16(addr+2);
	return v;
}


void m68k_write_memory_16(unsigned int addr, unsigned int value)
{
  struct m68000_dataregion *psWrite = quantum_writeword; /* Beginning of our handler */
  while (psWrite->lowaddr != ((unsigned) -1))
  {
      if ((addr >= psWrite->lowaddr) && (addr <= psWrite->highaddr))
      {
		  if (psWrite->memorycall != NULL)
		  {
	          ( (void (*)(unsigned, unsigned)) psWrite->memorycall) (addr, value);
	          return;
		  }
		  else
		  {
			  if (psWrite->userdata != NULL)
			  {
				  ((unsigned char*)psWrite->userdata)[addr-psWrite->lowaddr] = (value & 0xff00)>>8;
				  ((unsigned char*)psWrite->userdata)[addr-psWrite->lowaddr+1] = (value & 0xff);
			  }
	          return;
		  }
		  UN_WRITE(addr, value); // error
          return ;
      }
      ++psWrite;
  }
  UN_WRITE(addr, value); // error
  return;
}

void m68k_write_memory_32(unsigned int addr, unsigned int value)
{
  m68k_write_memory_16(addr, (value >>16)&0xffff);
  m68k_write_memory_16(addr+2, (value)&0xffff);
  return;
}





/*
void init68k(struct STARSCREAM_PROGRAMREGION *fetch, struct STARSCREAM_DATAREGION *readbyte, struct STARSCREAM_DATAREGION *readword, struct STARSCREAM_DATAREGION *writebyte, struct STARSCREAM_DATAREGION *writeword)
{
      s68000init();
       s68000context.fetch=fetch;
	  s68000context.s_fetch=fetch;
      s68000context.u_fetch=fetch;
      s68000context.s_readbyte=readbyte;
      s68000context.u_readbyte=readbyte;
      s68000context.s_readword=readword;
      s68000context.u_readword=readword;
      s68000context.s_writebyte=writebyte;
      s68000context.u_writebyte=writebyte;
      s68000context.s_writeword=writeword;
      s68000context.u_writeword=writeword;
	  s68000SetContext(&s68000context);
      s68000reset();
	  s68000exec(100);
       s68000reset();
     log_it("Initial PC is %06X\n",s68000context.pc);
}

*/

/*--------------------------------------------------------------------------*/
/* Memory handlers                                                          */
/*--------------------------------------------------------------------------*/
/*
unsigned int m68k_read_memory_8(unsigned int address)
{
  ; 
}


void m68k_write_memory_8(unsigned int address, unsigned int value)
{
;
}

unsigned int m68k_read_memory_16(unsigned int address)
{
     log_it("read word from %08X (%08X)\n", address, m68k_readpc());
    return (0xFFFF);
}

void m68k_write_memory_16(unsigned int address, unsigned int value)
{
   log_it("write word %04X to %08X (%08X)\n", value, address, m68k_readpc());
}
*/


void run_quantum()
{
//	log_it("---------------FRAME START --------------");   
	avg_clear();
	avg_go();
	
	pokey_sh_update();
	avg_clear();
}


int init_quantum()
{
 
	memset(main_ram, 0x00, 0x4fff);
	memset(vec_ram, 0x00, 0x1fff);
    memset(program_rom,0x00,0x13fff);
	memset(nv_ram,0xff,0x200);
	memcpy(program_rom,GI[CPU0],0x14000);//0x14000
	free(GI[CPU0]);
#ifdef SIXTEEN
	byteswap(program_rom, 0x14000);
#endif

//	byteswap
//	8bit : 16bit
//	1 : 100

// byteswap seems a good concept!

//    init68k(quantum_fetch,quantum_readbyte,quantum_readword,quantum_writebyte,quantum_writeword);
    init68k();
	avg_init();
	pokey_sh_start (&pokey_interface);
	QLoadScore();
	return 0;
}


void end_quantum()
{
	QSaveScore();
	pokey_sh_stop();	
}

