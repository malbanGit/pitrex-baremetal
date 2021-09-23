/* Tempest Emu */
#include <string.h>
#include <stdlib.h>

#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif
#include "log.h"
#include "tempest.h" 
#include "globals.h"
#include "vector.h"
//#include "glcode.h"
#include "input.h"
#include "cpu_control.h"
#include "sndhrdw/pokyintf.h"
#include "earom.h"
//#include "dxinput.h"
#include "aae_avg.h"
#include "mathbox.h"
#include "acommon.h"
//#include "config.h"

static int difficulty=2;

static int flipscreen=0;
static int INMENU=0;
static int tempprot=1;
static char *tbuffer;

static struct POKEYinterface pokey_interface =
{
	2,			/* 4 chips */
	1512000,
	255,	/* volume */
	6, //POKEY_DEFAULT_GAIN/2
	USE_CLIP,
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
	{ 0, 0, },
};

void tempm_reset()
{
memcpy(GI[CPU0],tbuffer,0x10000);  
m6502zpreset();
INMENU=1;
}

static void SwitchGame(void)
{  
	int a;
	int b;
	int oldgamenum=TEMPESTM;
	
	if (!INMENU){return;}

	INMENU=0;
	oldgamenum=gamenum;
	a =(GI[CPU0][0x51])+1;
	switch(a)
	{
	case 1: b=0x10000;gamenum=ALIENST;break;
	case 2: b=0x20000;gamenum=VBREAK;break;
	case 3: b=0x30000;gamenum=VORTEX;break;
	case 4: b=0x40000;gamenum=TEMPTUBE;break;
	case 5: b=0x50000;gamenum=TEMPEST1;break;
	case 6: b=0x60000;gamenum=TEMPEST2;break;
	case 7: b=0x70000;gamenum=TEMPEST;break;
	default: log_it("Hey unhandled game number!");
	}
	//log_it("A value is - %d",a);
//	setup_video_config();
	gamenum=oldgamenum; //Reset back
	memset(GI[CPU0],0x10000,0);
	memcpy(GI[CPU0],GI[CPU0]+ b,0x10000);  
	//cache_clear();
    m6502zpreset();
}	

READ_HANDLER (Pokey1Read)
{

static int rng=0;

	if (address==0x60ca)
	{	if ((GI[CPU0][0x60cf] & 0x02) != 0x00)
				{
					rng = (rand () >> 12) & 0xff;
					GI[CPU0][address]=rng;
					return rng;
				}
	else return rng;
	}
	if (address==0x60c8) {return getport(3);}//return z80dip1; 
 
   //if (config.cocktail)      {return getport(3) | 0x10;} //Cocktail flip bit, inverted config.
   //if (address < 0x60c8) return getport(3);
   return 0;//getport(3);
}
READ_HANDLER (Pokey2Read)
{
    static int rng=0;
    int val=getport(1);
	//if (address==0x60d8) return z80dip2;
	if (address==0x60da)
	{
	 if ((GI[CPU0][0x60df] & 0x02) != 0x00)
		{
			rng = (rand () >> 12) & 0xff;
		    GI[CPU0][address]=rng;
			return rng;
		}
	 else return rng;
	}
	
	if (gamenum==TEMPESTM && (val & 0x10))  {SwitchGame(); }//FIRE
	if (gamenum==TEMPESTM){	if (getport(5)) {tempm_reset();}}
    return val | difficulty;
}

WRITE_HANDLER(TempGo)
{   
	avg_go();
	//if (clearme==1) return;
	//if (clearme==0){tempest_vector_list();clearme=1;}
}

WRITE_HANDLER (watchdog_reset_w)
{
 WATCHDOG=data;
}


READ_HANDLER(TempestIN0read)
{
    int val=getport(0);//0x7f;//0x7f
  
	val = val | ((get_eterna_ticks(0) >> 1) & 0x80); //3Khz clock
   	if (avg_check())                      {bitclr(val, 0x40);}
	//if (key[config.ktestadv])             {bitclr(val, 0x20);} 
  	//if (KeyCheck(config.ktest))           {testsw^=1;}
	

	return val;
}

WRITE_HANDLER(tempest_led_w)
{set_aae_leds( ~data & 0x02, ~data & 0x01,0);}

WRITE_HANDLER(colorram_w)
{
  int bit3 = (~data >> 3) & 1;
  int bit2 = (~data >> 2) & 1;
  int bit1 = (~data >> 1) & 1;
  int bit0 = (~data >> 0) & 1;
  int r = bit1 * 0xee + bit0 * 0x11;
  int g = bit3 * 0xee;
  int b = bit2 * 0xee;

  vec_colors[address-0x800].r=r;
  vec_colors[address-0x800].g=g;
  vec_colors[address-0x800].b=b;
  //RIPPED THE ABOVE CODE FROM MAME. I COULD NEVER HAVE FIGURED IT OUT THIS NICE
}
WRITE_HANDLER(avg_reset_w)
{avg_clear();}//AVGRESET

WRITE_HANDLER(coin_write)
{if ((data & 0x08)){flipscreen=1;}else {flipscreen=0;}}

WRITE_HANDLER(WSpecial1)
{
	 GI[CPU0][address]=0;
}

WRITE_HANDLER(WSpecial2)
{
	GI[CPU0][address]=0;
}

WRITE_HANDLER(WSpecial3)
{
	GI[CPU0][address]=0;
}
///////////////////////  MAIN LOOP /////////////////////////////////////
void set_tempest_video()
{
	if (gamenum!=VBREAK)
      avg_clear();
}



READ_HANDLER (TempestDSW1)
{if (gamenum==25){return 0;}else {return z80dip1;}}

READ_HANDLER (TempestDSW2)
{if (gamenum==25){return 0;}else {return z80dip2;}}
//////////////////////////////////////////////////////////////////////////

MEM_READ(TempestRead)
 MEM_ADDR(0x0c00, 0x0c00, TempestIN0read)
 MEM_ADDR(0x0d00, 0x0d00, TempestDSW1)
 MEM_ADDR(0x0e00, 0x0e00, TempestDSW2)
 MEM_ADDR(0x60c0, 0x60cf, Pokey1Read)
 MEM_ADDR(0x60d0, 0x60df, Pokey2Read)
 MEM_ADDR(0x6040, 0x6040, MathboxStatusRead)
 MEM_ADDR(0x6050, 0x6050, EaromRead)
 MEM_ADDR(0x6060, 0x6060, MathboxLowbitRead)
 MEM_ADDR(0x6070, 0x6070, MathboxHighbitRead)
MEM_END

MEM_WRITE(TempestWrite)
 MEM_ADDR(0x011b, 0x011b, WSpecial1)
 MEM_ADDR(0x011f, 0x011f, WSpecial2)
 MEM_ADDR(0x0455, 0x0455, WSpecial3)
 MEM_ADDR(0x0800, 0x080f, colorram_w)
 MEM_ADDR(0x60c0, 0x60cf, pokey_1_w)
 MEM_ADDR(0x60d0, 0x60df, pokey_2_w)
 MEM_ADDR(0x6080, 0x609f, MathboxGo)
 MEM_ADDR(0x4000, 0x4000, coin_write)
 MEM_ADDR(0x4800, 0x4800, TempGo)
 MEM_ADDR(0x3000, 0x3fff, NoWrite)
 MEM_ADDR(0x6000, 0x603f, EaromWrite)
 MEM_ADDR(0x6040, 0x6040, EaromCtrl)
 MEM_ADDR(0x5000, 0x5000, watchdog_reset_w )
 MEM_ADDR(0x5800, 0x5800, avg_reset_w)
 MEM_ADDR(0x60e0, 0x60e0, tempest_led_w)
 MEM_ADDR(0x9000, 0xffff, NoWrite)
 MEM_ADDR(0x3000, 0x57ff, NoWrite)
MEM_END



void run_tempest()
{
	
	 //GI[CPU0][0x48]=0x7;
    // GI[CPU0][0x33a]=0;
	pokey_sh_update();
}
/////////////////// MAIN() for program ///////////////////////////////////////////////////
int init_tempest(void)
{  
   	init6502Z(TempestRead,TempestWrite,0);
  
   
	//cache_clear();
	
	
	if (gamenum==TEMPESTM) {  tbuffer=malloc(0x10000);
	                          memcpy(tbuffer,GI[CPU0],0x10000);
							  INMENU=1;
	                        } //Save the menu code so we can overwrite it.
	
	if (gamenum==TEMPTUBE ||
	    gamenum==TEMPEST1 ||
	    gamenum==TEMPEST2 ||
		gamenum==TEMPEST3 ||
		gamenum==VBREAK   ||
	    gamenum==TEMPEST )
	{
		LoadEarom();
		//LEVEL SELECTION HACK   (Does NOT Work on Protos) 
	    if (config.hack){GI[CPU0][0x90cd]=0xea;GI[CPU0][0x90ce]=0xea;}
	}
	
	
    difficulty = GI[CPU0][0x60d0]; //SET Difficulty, hack really but it works.
	
	pokey_sh_start (&pokey_interface);
	avg_init();   
	return 0;   
}

void end_tempest()
{
    if (gamenum==TEMPTUBE ||
	    gamenum==TEMPEST1 ||
	    gamenum==TEMPEST2 ||
		gamenum==TEMPEST3 ||
		gamenum==VBREAK   ||
	    gamenum==TEMPEST )
	{
		SaveEarom();
	}
	
	pokey_sh_stop();
	
}