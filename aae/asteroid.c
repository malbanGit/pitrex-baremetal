/* Asteroid Emu */
#include <stdlib.h>
#include <string.h>

#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

#include "asteroid.h"
#include "globals.h"
#include "samples.h"
#include "vector.h"
#include "input.h"
#include "cpu_control.h"
#include "osd_cpu.h"
#include "earom.h"
#include "pokey.h"
#include "sndhrdw/pokyintf.h"
#include "cpu_control.h"
#include "acommon.h"
#include "log.h"
#include "loaders.h"

//extern uint8 rng[MAXPOKEYS];
int SCRFLIP;
int dvggo=0;
UINT8 buffer[0x100];
//int astdelux_bank;
int check=0;
int psound=0;
int set=0;
int vec_done=0;
int aticks2=0;



 void malbancheck_joystick(void)
{ int x=0;
  int y=0;
  int c;
/*
  poll_joystick();

    for (c=0; c<joy[0].num_buttons; c++) 
	  {
         if (joy[0].button[0].b) glPrint(290, 580, 255,255,255,255,1,0,0,"button pressed");
	   }
*/
    
   /*
	 if (joy[x].stick[y].axis[0].d1) {
	 glPrint(200, 700, 255,255,255,255,1,0,0," Joystick %d Stick %d axis 0 d1",x,y);
	 }
	 if (joy[x].stick[y].axis[0].d2) {
	 glPrint(200, 670, 255,255,255,255,1,0,0," Joystick %d Stick %d axis 0 d2",x,y);
	 }
	 if (joy[x].stick[y].axis[1].d1) {
	 glPrint(200, 640, 255,255,255,255,1,0,0," Joystick %d Stick %d axis 1 d1",x,y);
	 }
	 if (joy[x].stick[y].axis[1].d2) {
	 glPrint(200, 610, 255,255,255,255,1,0,0," Joystick %d Stick %d axis 1 d2",x,y);
	 }*/
	 // }
/*	 
  if (joy[0].stick[0].flags & JOYFLAG_ANALOGUE)	
  {
	  glPrint(230, 610 + (x*20), 255,255,255,255,1,0,0,"analog stick %d axis 0 %d",y,joy[0].stick[0].axis[2].pos);
	  glPrint(200, 630 + (x*20), 255,255,255,255,1,0,0,"analog stick %d axis 0 %d",y,joy[0].stick[0].axis[3].pos);
  }
*/		 
	 //log_it("analog stick %d axis 1 %d",y,joy[x].stick[y].axis[1].pos);} 
	 
	 
	
	
	  
	  

}

/*
void asteroid_int()
{
  static int count=0;
  count ++;
  if (count==1) { if (!testsw ){m6502zpnmi();} count=0;} 
}
*/

static struct POKEYinterface pokey_interface =
{
	1,			/* 4 chips */
	1512000,
	255,	/* volume */
	6,
	NO_CLIP,
	/* The 8 pot handlers */
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0},
	/* The allpot handler */
	{ DSRead_1 }, //Dip here
};

int check_hi(void)
{ 
    int num=0;
    if (gamenum==ASTEROID) {num=0x1d;} else num = 0x1c;

	if (memcmp(&GI[CPU0][num],"\x00\x00",2) == 0 && memcmp(&GI[CPU0][0x0050],"\x00\x00",2) == 0 &&
			memcmp(&GI[CPU0][0x0000],"\x10",1) == 0)	
    {
        load_hi_aae(num, 0x35, 0);
		return 1;
	}
	else return 0;	// we can't load the hi scores yet 
}
void  save_hi()
{
	int num;
	if (gamenum==ASTEROID) 
    {
      num=0x1d;
    }
    else 
      num = 0x1c;
    save_hi_aae(num, 0x35, 0);
}
void set_ast_colors(void)
{
	int i=0;
	
	vec_colors[0].r=0;
	vec_colors[0].g=0;
	vec_colors[0].b=0;

	for (i=1;i<17;i++)
	 {
	  vec_colors[i].r=i*16;
      vec_colors[i].g=i*16;
      vec_colors[i].b=i*16;
	 }
	
}



////////////   SOUND ROUTINE FOR FIRST SET OF SOUNDS  /////////////////////////
WRITE_HANDLER(Sounds2)
{
    static int mfire = 0;
	static int vsfire = 0;
	static int saucer = 0;
	static int lastsaucer = 0;
	static int lastthrust = 0;
	static int saucertoggle=0;
   
	//int sound;
	int mfire2;
	int vsfire2;

GI[CPU0][address] = data;
/*
	switch (address)
	{Explosions
	case 0x3c00:  
		    if (data&0x80 && saucertoggle==0) 
				{
					if (GI[CPU0][0x3c02]&0x80)
				    sample_start(4,9,1);
					else
					sample_start(4,8,1);
				    saucertoggle=1; break;
				}
			if (!(data&0x80) && saucertoggle==1)
			{	saucertoggle=0;
			    sample_stop(4);
			}       
		      break;	
	case 0x3c01:  
	              vsfire2 = data & 0x80;
				if (vsfire2!=vsfire)
					{
						sample_stop(3);
						if (vsfire2) sample_start(3,1,0);
					}
					vsfire=vsfire2;
					break;
			        	
	case 0x3c03:  if ((data&0x80) && !(lastthrust&0x80)) sample_start(5,2,1);
			      if (!(data&0x80) && (lastthrust&0x80)){sample_end(5);}
			      lastthrust=data;
			      break;
			           
	case 0x3c04:    mfire2 = data & 0x80;
					if (mfire2 != mfire)
					{
						 sample_stop(2);
						if (mfire2) sample_start(2,0,0);
					}
					mfire = mfire2;
					break;
	case 0x3c05:
		         if (data & 0x80)
				{   if (sample_playing(6)==0) {
				    sample_start(6,10,0);
				}   break;
					}
	default: break;
	}
*/
}

////////////////// END SOUND ROUTINE FOR SECOND SET OF SOUNDS ////////////////////////

WRITE_HANDLER(Explosions)
{   
    static int explosion = -1;
	int explosion2;
	int sound = -1;
/*
	if (data & 0x3c)
	{
		explosion2 = data >> 6;
		if (explosion2 != explosion)
		{
			
			switch (explosion2)
			{
				case 0:
				case 1:
				    sample_start(7,3,0);
					break;
				case 2:
					 sample_start(7,4,0);
					break;
				case 3:
					sample_start(7,5,0);
					break;
			   
				default: sample_start(7,4,0);
			}
		}
		explosion = explosion2;
	}
	else explosion = -1;
    */
}

WRITE_HANDLER(Heartbeat)
{
 if (data & 0x10)
	{
//		if (data & 0x0f){sample_start(8,7,0);}
//		else {sample_start(8,6,0);}
	}      
}
///////////////////////////////////////////////////////////////////////////////
WRITE_HANDLER(DeluxeLedWrite)
{   //Thanks to whoever decoded this for MAME!!!
	static int led0=0;
	static int led1=0;
	     
	if (address&0xff) {led1=(data&0x80)?0:1;}
	else {led0=(data&0x80)?0:1;}
    
    set_aae_leds(led0,led1,0);
}
WRITE_HANDLER(DeluxeSwapRam)
{
	static int astdelux_bank = 0;
	int astdelux_newbank;
	unsigned char *RAM = GI[CPU0];


	astdelux_newbank = (data >> 7) & 1;
	SCRFLIP=GI[CPU0][0x1e];
	if (astdelux_bank != astdelux_newbank) 
    {
		/* Perform bankswitching on page 2 and page 3 */
		int temp;
		int i;

		astdelux_bank = astdelux_newbank;
		for (i = 0; i < 0x100; i++) 
        {
			temp = RAM[0x200 + i];
			RAM[0x200 + i] = RAM[0x300 + i];
			RAM[0x300 + i] = temp;
		}
	}
	/*int astdelux_newbank=0;
   
    astdelux_newbank = (data >> 7) & 1; 
	SCRFLIP=GI[CPU0][0x1e]; 
	if (astdelux_bank != astdelux_newbank )
	
	{
		astdelux_bank = astdelux_newbank;
		memcpy(buffer, GI[CPU0] + 0x200, 0x100);
		memcpy(GI[CPU0] + 0x200, GI[CPU0] + 0x300, 0x100);
		memcpy(GI[CPU0] + 0x300, buffer, 0x100);
	}
  */
}
////////////  CALL ASTEROIDS SWAPRAM  ////////////////////////////////////////////
WRITE_HANDLER(AsteroidsSwapRam)
{
	static int asteroid_bank = 0;
	int asteroid_newbank;
	asteroid_newbank = (data >> 2) & 1;
	SCRFLIP=GI[CPU0][0x18];
	if (asteroid_bank != asteroid_newbank) 
    {
		/* Perform bankswitching on page 2 and page 3 */
		asteroid_bank = asteroid_newbank;
		memcpy(buffer, GI[CPU0] + 0x200, 0x100);
		memcpy(GI[CPU0] + 0x200, GI[CPU0] + 0x300, 0x100);
    	memcpy(GI[CPU0] + 0x300, buffer, 0x100);
	}
	set_aae_leds ((~data & 0x02), (~data & 0x01),0 );
}
/////////// END ASTEROIDS SWAPRAM ///////////////////////////////////////////////////

READ_HANDLER(RandRead)
{  
	//static uint8 pokey_random[4];     
 
	//if (rng[chip]) { pokey_random[chip] = (pokey_random[chip]>>4) | (rand()&0xf0); }
  if ((address  & 0x0f) ==  0x08)
  {
    return 0x0;
  }
  if ((address  & 0x0f) ==  0x0a)
  { 
    return rand() & 0xff;
  }//pokey_random[chip];
  else 
    return 0;
}

WRITE_HANDLER(Thrust)
{
/*  
	static int lastthrust = 0;
	if (!(data&0x80) && (lastthrust&0x80)) 
	{ sample_adjust(4, PLAYMODE_PLAY); }
	//{sample_end(0);}
	if ((data&0x80) && !(lastthrust&0x80)){sample_start(4,0,1);}
	lastthrust=data;
*/
  
}

WRITE_HANDLER(DxExplosions)
{
//  	if (data == 0x3d)  {sample_start(1,1,0);}
//	if (data == 0xfd)  {sample_start(2,2,0);}
//	if (data == 0xbd)  {sample_start(3,3,0);}
}
static void dvg_vector_timer (int scale)
{total_length += scale;}
/////////////////////////////VECTOR GENERATOR//////////////////////////////////
void dvg_generate_vector_list(void)
{
	   
	    int  pc = 0x4000;
		int sp = 0;
		int stack [4];
		int scale = 0;
		int done = 0;
        UINT16 firstwd,secondwd = 0;
	    UINT16 opcode;
        int  x, y;
        int temp;
		int z;
		int a;
        float deltax, deltay; //float
        float currentx, currenty = 0; //float
		int div =0;
        int bright=0;
		total_length=0;
	    currentx= 0;
	    currenty=0;
		while (!done)
	{
	   firstwd = memrdwd(pc);opcode = firstwd & 0xf000;pc++;pc++;
	   
      
	   switch (opcode)
		{
    	  case 0xf000: 
		       			
			    // compute raw X and Y values //
			    z = (firstwd & 0xf0) >> 4;
				y = firstwd & 0x0300;
				x = (firstwd & 0x03) << 8;
				//Scale Y best we can			
			    x=x*4;
				y=y*4;
				//Check Sign Values and adjust as necessary
				if (firstwd & 0x0400) {y = -y;}
				if (firstwd & 0x04)   {x = -x;}
			    //Invert Drawing if in Cocktal mode and Player 2 selected
				if (!testsw){
				if (SCRFLIP  && config.cocktail )
				 	{x=-x;y=-y;}
					 }	
				
			    temp = 2 + ((firstwd >> 2) & 0x02) + ((firstwd >>11) & 0x01);
	  		    temp = ((scale + temp) & 0x0f);
			    if (temp > 9) temp = -1;
				dvg_vector_timer(temp);
				
			    deltax = x >> (9-temp);
			    deltay = y >> (9-temp);
			    deltax=deltax*.25;
			    deltay=deltay*.25;
/*			   								
				if ((currentx==(currentx)+deltax) && (currenty==(currenty)-deltay))
				  { 
                    if (z ==7){cache_txt(currentx,currenty,config.explode_point_size,255);}else {cache_point(currentx, currenty ,z,config.gain,0,1.0);}
          
                  }
				
                else
*/                  
			    	{
   int x0 = ((int)currentx)<<5;
   int y0 = -(((int)currenty)<<5);
   int x1 = ((int)(currentx+deltax))<<5;
   int y1 = -( (int)(currenty-deltay)<<5);
   int zz =(z<<3)%128;
  v_directDraw32(x0-17000, y0+17000, x1-17000,y1+17000, zz);
// add_line(currentx,currenty,currentx+deltax,currenty-deltay, z,config.gain,0);
// do itright

/* 
   int x0 = ((int)currentx)<<5;
   int y0 = -(((int)currenty)<<5);
   int x1 = ((int)(currentx+deltax))<<5;
   int y1 = -( (int)(currenty-deltay)<<5);
   int zz =(z<<3)%128;
  v_directDraw32(x0-17000, y0+17000, x1-17000,y1+17000, zz);
*/ 
//					cache_line(currentx,currenty,currentx+deltax,currenty-deltay, z,config.gain,0);
//					cache_point(currentx,currenty,z,config.gain,0,0);
//					cache_point(currentx+deltax,currenty-deltay,z,config.gain,0,0);
					}

			 		        
			    currentx += deltax;
				currenty -= deltay;
				break;
				
			case 0:done=1;break;
			case 0x1000:
			case 0x2000:
			case 0x3000:
			case 0x4000:
			case 0x5000:
			case 0x6000:
			case 0x7000:
			case 0x8000:
			case 0x9000:
	  		
			    // Get Second Word
				secondwd =  memrdwd(pc);pc++;pc++;
			    // compute raw X and Y values and intensity //
				z = secondwd >> 12;
				y = firstwd & 0x03ff;
				x = secondwd & 0x03ff;
			    //Scale Y best we can			
				x=x*4;y=y*4;
				
				//Check Sign Values and adjust as necessary
				if (firstwd & 0x0400)
					{y = -y;}
				if (secondwd & 0x400)
					{x=-x;}
				//Invert Drawing if in Cocktal mode and Player 2 selected
				if (!testsw){	
				if (SCRFLIP  && config.cocktail )
				 	{x=-x;y=-y;}}
				// Do overall scaling
				temp = scale + (opcode >> 12);temp = temp & 0x0f;
				if (temp > 9){temp = -1;}
				dvg_vector_timer(temp);
						   
			    deltax = x >> (9-temp);
			    deltay = y >> (9-temp);	
						  
				deltax=deltax*.25;
				deltay=deltay*.25;
/*             
			 	 if ((currentx==(currentx)+deltax) && (currenty==(currenty)-deltay))
				   { if (z>14) cache_txt(currentx,currenty,config.fire_point_size,255); else cache_point(currentx, currenty ,z,0,0,1.0);}
				   
						cache_line(currentx, currenty, currentx+deltax, currenty-deltay, z,config.gain,0);
						cache_point(currentx,currenty,z,config.gain,0,0);
						cache_point(currentx+deltax,currenty-deltay,z,config.gain,0,0);
*/				 
   int x0 = ((int)currentx)<<5;
   int y0 = -(((int)currenty)<<5);
   int x1 = ((int)(currentx+deltax))<<5;
   int y1 = -( (int)(currenty-deltay)<<5);
   int zz =(z<<3)%128;
  v_directDraw32(x0-17000, y0+17000, x1-17000,y1+17000, zz);
// add_line(currentx,currenty,currentx+deltax,currenty-deltay, z,config.gain,0);
				currentx += deltax;currenty -= deltay;	
				break;
			
			case 0xa000:
			     
				secondwd =  memrdwd(pc);
			    pc++;
		        pc++;
				x = twos_comp_val (secondwd, 12);
				y = twos_comp_val (firstwd, 12);
               	//Invert the screen drawing if cocktail and Player 2 selected
				if (!testsw){
				if (SCRFLIP && config.cocktail)
				  {x=1024-x; y=1024-y;}}
				//Do overall draw scaling
				scale = (secondwd >> 12) & 0x0f;
			  	currenty = 1130-y; //y-100; 1130-y
				currentx = x;
				break;

			case 0xb000: done = 1;break;
           
			case 0xc000: a = 0x4000 + ((firstwd & 0x0fff) << 1) ;
				         stack [sp] = pc;
				         if (sp == 4){done = 1;sp = 0;}else {sp=sp+1;pc = a;}
				         break;
			    
			case 0xd000:
				 sp=sp-1;
			     pc = stack [sp] ;
				 break;
				
			case 0xe000:
				 a = 0x4000 + ((firstwd & 0x0fff)  << 1);
				 pc = a;
				 break;
         }
	}
}

//////////////// VECTOR GENERATOR TRIGGER ///////////////////////////////////////
WRITE_HANDLER (Watchdog_reset_w)
{
 WATCHDOG=data;
}

WRITE_HANDLER (BWVectorGeneratorInternal)
{   

	if (vec_done) return;

	dvggo=1;
    //cache_clear();
    total_length=0;dvg_generate_vector_list();set=1;vec_done=1;//log_it("DVG GO CALLED THIS FRAME");
	if (total_length==0)  
    {
      dvggo=0;
      vec_done=0;
      set=0;
      aticks2=get_video_ticks(0xff);
    }//log_it("DVG GO SCRATCHED ---------- THIS FRAME");}
	
	
}
///////////// END VECTOR GENERATOR TRIGGER  //////////////////////////////////////
/////////////////////END VECTOR GENERATOR///////////////////////////////////////

/////////////////////READ KEYS FROM PIA 1 //////////////////////////////////////
READ_HANDLER(AstPIA1ROCKRead)
{
	int res;
	//int bitmask;
	int val=0;
	float me;
	res=0;
	

  switch (address) 
	{
	case 0x2001: if (get_video_ticks(0) & 0x100)  val=0x80; break;//if (get_hertz_counter()) val=0x80;break;
	case 0x2002: me = 6.7 * total_length;if ( get_video_ticks(0) > me && vec_done==1 ) {vec_done=0;}break;//me = (((4500 * total_length)/ 1000000) * 1512);
//	case 0x2003: if (key[config.kp1but3] || my_j[config.j1but3])return 0;else return 0x80; break; /*Shield */
//   case 0x2004: if (key[config.kp1but1] || my_j[config.j1but1])return 0;else return 0x80; break;/* Fire */
    case 0x2005: return 0x80;break;
    case 0x2006: return 0;break;
    case 0x2007: return 0;break;//if (testsw)return 0;else return 0x80;break;/* Self Test */
    }
   return 0;
}

READ_HANDLER(AstPIA1Read)
{
    float me;
	int res;
	int bitmask;

	res=getport(0);

	bitmask =  (1 << (address & 0x0f));

	if (get_video_ticks(0) & 0x100)
		res |= 0x02;
	if (address==0x2002) { 
		                   me = (((4500 * total_length)/ 1000000) * 1512);
	                       if ( get_video_ticks(0) > me && vec_done==1 ) {vec_done=0;res |= 0x04;}
	                     }
	//if (!avgdvg_done()) {
		
		//res |= 0x04;
	//}

	if (res & bitmask)
		res = 0x80;
	else
		res = ~0x80;
	//if (address == 0x2007 &&  res = 0x80) (testsw^=1;));

	return res;
  	//int val=0;
	//float me;

  //switch(address)
	//{      
  //    case 0x2001: if (get_video_ticks(0) & 0x100)  val=0x80; break;//if (get_hertz_counter()) val=0x80;break;
	//  case 0x2002: me = (((4500 * total_length)/ 1000000) * 1512);if ( get_video_ticks(0) > me && vec_done==1 ) {vec_done=0;}break;
	//  case 0x2003: if (key[config.kp1but3]) val = 0x80;break;
	//  case 0x2004: if (key[config.kp1but1]) val = 0x80;break;
	//  case 0x2005: break;
	//  case 0x2006: break;
	//  case 0x2007: if (testsw) val = 0x80;

  //}
 //return val;

 }

READ_HANDLER(AstPIA2Read) 
{
	short int val;
	int bitmask;

	val=getport(1);
	bitmask = (1 << (address & 0x0f));
    
	if (val & bitmask)
		val = 0x80;
	else
	 	val = ~0x80;
    
//printf ("Port1: %02x\n\r", (val&0xff));    
    
	return (val);
	
}
//////////////////////////////////////////////////////////////////////////
////////////////////////////LOAD HISCORE//////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////  MAIN LOOP /////////////////////////////////////

void run_asteroids()
{
	static int k=0;
	
	//if (KeyCheck(config.kreset))  {cpu_needs_reset(0);}
	if (getport(0) & 0x80) {cpu_disable_interrupts(0,0);} else {cpu_disable_interrupts(0,1);}
	if (psound && !paused) {pokey_sh_update();}	
	
    
	//log_it("Watchdog this frame %x",WATCHDOG);
	if (WATCHDOG==0) k++;
	if (k > 60) {cpu_needs_reset(0);k=0;}

    WATCHDOG=0;
}
/////////////////END MAIN LOOP/////////////////////////////////////////////




MEM_WRITE(AsteroidDeluxeWrite)
    //MEM_ADDR(0x2400, 0x2407, NoWrite)
	//MEM_ADDR(0x2000, 0x2007, NoWrite)
	MEM_ADDR(0x2c00, 0x2c0f, pokey_1_w)
	MEM_ADDR(0x3000, 0x3000, BWVectorGeneratorInternal)
	MEM_ADDR(0x3c03, 0x3c03, Thrust)
	MEM_ADDR(0x3c04, 0x3c04, DeluxeSwapRam)
	MEM_ADDR(0x3600, 0x3600, DxExplosions)
	MEM_ADDR(0x3400, 0x3400, Watchdog_reset_w)
	MEM_ADDR(0x3200, 0x323f, EaromWrite)
	MEM_ADDR(0x3a00, 0x3a00, EaromCtrl)
	MEM_ADDR(0x3c00, 0x3c01, DeluxeLedWrite)
	MEM_ADDR(0x4800, 0x7fff, NoWrite)
MEM_END

MEM_READ(AsteroidDeluxeRead)
	MEM_ADDR(0x2c40, 0x2c7f, EaromRead)
	MEM_ADDR(0x2c00, 0x2c0f, RandRead)
	MEM_ADDR(0x2400, 0x2407, AstPIA2Read)
	MEM_ADDR(0x2000, 0x2007, AstPIA1Read)
MEM_END

MEM_READ(AsteroidRead)
	MEM_ADDR(0x2400, 0x2407, AstPIA2Read)
	MEM_ADDR(0x2000, 0x2007, AstPIA1Read)
MEM_END

MEM_READ(AsterRockRead)
	MEM_ADDR( 0x2400, 0x2407, AstPIA2Read)
	MEM_ADDR(0x2000, 0x2007, AstPIA1ROCKRead)
MEM_END

MEM_WRITE(AsteroidWrite)
	MEM_ADDR(0x3000,0x3000,BWVectorGeneratorInternal)
	MEM_ADDR(0x3200, 0x3200, AsteroidsSwapRam)
	MEM_ADDR(0x3400, 0x3400, Watchdog_reset_w)
	MEM_ADDR(0x3600, 0x3600, Explosions)
	MEM_ADDR(0x3a00, 0x3a00, Heartbeat)
	MEM_ADDR(0x3c00, 0x3c05, Sounds2)
	MEM_ADDR(0x6800, 0x7fff, NoWrite) //Program Rom
	MEM_ADDR(0x5000, 0x57ff, NoWrite) //Vector Rom
MEM_END
/////////////////// MAIN() for program ///////////////////////////////////////////////////
void end_asteroids()
{
 save_hi();
}
void end_astdelux()
{
//    cache_clear();
	SaveEarom();
	pokey_sh_stop();
  	
}
int init_asteroid(void)
{
    
	if (gamenum==ASTEROCK)
	{init6502Z(AsterRockRead, AsteroidWrite,0);}
	else
	{init6502Z(AsteroidRead, AsteroidWrite,0);}
	 
	GI[CPU0][0x2002] =  0x0;
	GI[CPU0][0x2007] =  0x0;
    GI[CPU0][0x2801] =  0xff; // Just to clear 0 value
	 
//    cache_clear();
	set_ast_colors();
	
	if (config.bezel) {config.cocktail=0;}//Just to check for stupidity
	check=0;
	psound=0;
	dvggo=0;
	total_length=0;
	
//	while (check_hi()==0)
	//{
	//m6502zpexec(6250);
	//m6502zpnmi();
	//}
    
    log_it("End init");
	return 0;   
}

int init_astdelux(void)
{
	int k;
    if (config.bezel) {config.cocktail=0;}//Just to check for stupidity
	init6502Z(AsteroidDeluxeRead,AsteroidDeluxeWrite,0);
		
//	cache_clear();
	set_ast_colors();
	LoadEarom();
	
	check=1; //Disable Asteroids HiScore Loading
	psound=1; //Enable Pokey Sound Processing
	dvggo=0;
	total_length=0;
	k=pokey_sh_start(&pokey_interface);

	
	
	return 0;   
}