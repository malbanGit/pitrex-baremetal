/* Lunar Lander Emu */

#ifndef NO_PI 
#include <vectrex/vectrexInterface.h>
#endif

#include "llander.h"
#include "globals.h"
#include "samples.h"
#include "vector.h"
#include "input.h"
#include "cpu_control.h"
#include "acommon.h"
#include "log.h"

//extern char *gamename[];

static int ldvggo=0;
static int thrust=0;
static int pic=0;
static int lamp0=0;
static int lastlamp0;
static int a,b,c;


static void ldvg_generate_vector_list(void)
{
	
	    int pc = 0x4000;
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
        float deltax, deltay;
        float currentx, currenty = 0;
	    int me=0;
        
		
	 currentx=0;
	 currenty=0;
		while (!done)
	{
	    
	   
		firstwd = memrdwd(pc);
		opcode = firstwd & 0xf000;
	  	pc++;
		pc++; 
	    
	
		switch (opcode)
		{
    		case 0xf000: 
		       			
			    // compute raw X and Y values //
			
				z = (firstwd & 0xf0) >> 4;
				y = firstwd & 0x0300;
				x = (firstwd & 0x03) << 8;
				//Scale Y best we can			
			    x=x*65536;
				y=y*65536;
				//Check Sign Values and adjust as necessary
				if (firstwd & 0x0400) {y = -y;}
				if (firstwd & 0x04)   {x = -x;}
			    //Invert Drawing if in Cocktal mode and Player 2 selected
				temp = 2 + ((firstwd >> 2) & 0x02) + ((firstwd >>11) & 0x01);
	  			temp = ((scale + temp) & 0x0f);
				if (temp > 9) temp = -1;
				
				goto DRAWCODE;
			   	break;
				
			case 0:
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
				 secondwd =  memrdwd(pc);
			     pc++;
		         pc++;
			
				 // compute raw X and Y values and intensity //
				z = secondwd >> 12;
				y = firstwd & 0x03ff;
				x = secondwd & 0x03ff;
			    //Scale Y best we can			
				x=x*65536;
				y=y*65536;
				
				//Check Sign Values and adjust as necessary
				if (firstwd & 0x0400)
					{y = -y;}
				if (secondwd & 0x400)
					{x=-x;}
				//Invert Drawing if in Cocktal mode and Player 2 selected
							  
				// Do overall scaling
			
				temp = scale + (opcode >> 12);
				temp = temp & 0x0f;
	  		
				if (temp > 9)
				   {temp = -1;}
DRAWCODE:  		   	
			   
			   deltax = x >> (9-temp);
			   deltay = y >> (9-temp);	
			   
			  
			   deltax=deltax/65536;
			   deltay=deltay/65536;	
			  	
				
				
				if (z>-1)
				{  		
					
					if ((currentx==(currentx)+deltax) && (currenty==(currenty)-deltay))
				    {
//                      cache_point(currentx, currenty ,z,config.gain,0,1.0);
                    }
//                    add_line(currentx,currenty,currentx+deltax,currenty-deltay, z,config.gain,0);
//                	cache_line(currentx,currenty,currentx+deltax,currenty-deltay, z,config.gain,0);
//					cache_point(currentx,currenty,z,config.gain,0,0);
//					cache_point(currentx+deltax,currenty-deltay,z,config.gain,0,0);
                    
   int x0 = ((int)currentx)<<5;
   int y0 = -(((int)currenty)<<5);
   int x1 = ((int)(currentx+deltax))<<5;
   int y1 = -( (int)(currenty-deltay)<<5);
   int zz =(z<<3)%128;
  v_directDraw32(x0-17000, y0+17000, x1-17000,y1+17000, zz);
                    
                    
				}
		    	 
				currentx += deltax;
				currenty -= deltay;
                deltax,deltay=0;
			    break;

			case 0xa000:
			     
				secondwd =  memrdwd(pc);
			    pc++;
		        pc++;
				x = twos_comp_val (secondwd, 12);
				y = twos_comp_val (firstwd, 12);
               	 //Scale Y drawing as best we can
				//Invert the screen drawing if cocktail and Player 2 selected
				
				scale = (secondwd >> 12) & 0x0f;
			    currenty = 1015-y;
				currentx = x ;
				break;

			case 0xb000:
                done = 1; 
				break;

			case 0xc000:
				
				 a = 0x4000 + ((firstwd & 0x0fff) << 1) ;
			     stack [sp] = pc;
				 				
				 if (sp == 4)
	    			{
					done = 1;
					sp = 0;
					}
				else
			    sp=sp+1;
				pc = a;
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


READ_HANDLER(llander_zeropage_r)
{return GI[CPU0][address];}


READ_HANDLER(Llander_dips)
{    
	 switch(address) //fc fd
	 {      
     case 0x2800:return z80dip1;break;
	 case 0x2801:return (z80dip1&0x04)>>2 | z80dip2;
	 case 0x2802:return 0xfc;break;
 	 case 0x2803:return 0xfd;break;
       }
return 0;

	return 0xfc;
}

WRITE_HANDLER(llander_zeropage_w)
{
  GI[CPU0][address]=data;
  GI[CPU0][0x0100+address]=data;
}


WRITE_HANDLER(LanderGeneratorInternal)
{ldvggo=1; }

static void set_ll_colors(void)
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
READ_HANDLER(Readinput_port_0)
{
	 int val;//=0xff;
	 //static int lastret=1;
      val=getport(0);
	 //lastret^=1;

    // if (lastret)         {bitclr(val, 0x40);}else{bitset(val, 0x40);}
	// if (testsw)          {bitclr(val, 0x02);}
	 if (ldvggo)          {bitclr(val, 0x01);}else {bitset(val, 0x01);}
    // if (config.ktestadv) {bitclr(val, 0x80);}
 
    return val;
}
READ_HANDLER(Readinput_port_1)
{

	
	int res;
	int bitmask;

	res=getport(1);

	bitmask =  (1 << (address & 0x0f));

	//if (get_video_ticks(0) & 0x100)
	//	res |= 0x02;
	//if (address==0x2002) { 
		                 //  me = (((4500 * total_length)/ 1000000) * 1512);
	                      // if ( get_video_ticks(0) > me && vec_done==1 ) {vec_done=0;res |= 0x04;}
	                    // }
	//if (!avgdvg_done()) {
		
		//res |= 0x04;
	//}

	if (res & bitmask)
		res = 0x80;
	else
		res = ~0x80;
	
	return   res;
	/*
switch(address)
	{      
     case 0x2400: // Start 
         if (key[config.kstart1])
            return 0x80;break;
				
	 case 0x2401: /* Coin in 
		 if (key[config.kcoin1])return 0x7f;else return 0x80; break;
	 
	 case 0x2402: return 0x7f;break; //Nothing
     
	 case 0x2403: /* Coin in 
         if (key[config.kcoin2])return 0x7f;else return 0x80; break;
  
	 case 0x2404: /* Game Select 
         if (key[config.kstart2])
            return 0x80;
         break;
      case 0x2405: /* Abort 
         if (key[config.kp1but3])
            return 0x80;
         break;
      case 0x2406: /* Rotate right 
         if (key[config.kp1right])
            return 0x80;
         break;
      case 0x2407: /* Rotate left
         if (key[config.kp1left])
            return 0x80;
         break;
      }
    return 0x7f;
	*/
}

READ_HANDLER(Readinput_port_3)
{


/*
if (key[config.kp1but1])
{
	if (thrust < 0xff)
		{thrust +=  0x1;return thrust;}
}
if (key[config.kp1but2])
{   if (thrust > 0x2)
        {thrust -= 0x1;return thrust;}
}
*/
//return thrust;
return getport(5);
}

WRITE_HANDLER(llander_snd_reset_w)
{;}

WRITE_HANDLER (watchdog_reset_w)
{
 WATCHDOG=data;
}

WRITE_HANDLER(llander_sounds_w)
{
	int tvol;
	//volume    = data & 0x07;
	//tone_3khz = data & 0x10;
    //tone_6khz = data & 0x20; //Only used in Selftest
	//llander_explosion = data & 0x08;
	
	if (data == 0) { sample_set_volume(1,2);}
	if (data == 1) { sample_set_volume(1,10);}
	if (data & 0x07)
{
    
	tvol=GI[CPU0][0x01] << 4;
	if (tvol < 10){tvol=10;}
	
	sample_set_volume(1,tvol);
}
	
	if (data & 0x10) 
{
	if  (sample_playing(3)==0)
	 {sample_start(3,1,0);}
	
}

if (data & 0x20) 
{
	if  (sample_playing(4)==0)
	 {sample_start(4,3,0);}
	
}
if (data & 0x08)
{  if (sample_playing(2)==0)
		{ sample_start(2,2,0);}
}

}

///////////////////  /READ KEYS FROM PIA 1 //////////////////////////////////////
/* Lunar lander LED port seems to be mapped thus:

   NNxxxxxx - Apparently unused
   xxNxxxxx - Unknown gives 4 high pulses of variable duration when coin put in ?
   xxxNxxxx - Start    Lamp ON/OFF == 0/1
   xxxxNxxx - Training Lamp ON/OFF == 1/0
   xxxxxNxx - Cadet    Lamp ON/OFF
   xxxxxxNx - Prime    Lamp ON/OFF
   xxxxxxxN - Command  Lamp ON/OFF

   Selection lamps seem to all be driver 50/50 on/off during attract mode ?

*/

///////////////////////////////////////////////////////////////////////////////
WRITE_HANDLER(led_write)
{
	
data=data & 0xff;
lamp0=((data >> (4 - 0)) & 1);
b+=((data >> (4 - 1)) & 1);
c+=((data >> (4 - 2)) & 1);

	   
data=data & 0x0f;
if (data ==0x0f) {pic=1;}
if (data ==0x08) {pic=2;}
if (data ==0x04) {pic=3;}
if (data ==0x02) {pic=4;}
if (data ==0x01) {pic=5;}
}

MEM_WRITE(LlanderWrite)
 MEM_ADDR(0x0000, 0x00ff, llander_zeropage_w)
 MEM_ADDR(0x3000, 0x3000, LanderGeneratorInternal)
 MEM_ADDR(0x3200, 0x3200, led_write)
 MEM_ADDR(0x3400, 0x3400, watchdog_reset_w)
 
 MEM_ADDR(0x3c00, 0x3c00, llander_sounds_w)
 MEM_ADDR(0x3e00, 0x3e00, llander_snd_reset_w)
 MEM_ADDR(0x4800, 0x5fff, NoWrite)
 MEM_ADDR(0x6000, 0x7fff, NoWrite)
MEM_END

MEM_READ(LlanderRead)
 MEM_ADDR(0x0000, 0x00ff, llander_zeropage_r)
 MEM_ADDR(0x2c00, 0x2c00, Readinput_port_3)
 MEM_ADDR(0x2400, 0x2407, Readinput_port_1)
 MEM_ADDR(0x2800, 0x2803, Llander_dips)
 MEM_ADDR(0x2000, 0x2000, Readinput_port_0)
MEM_END



void run_llander()
{
	static int k=0;
	
	/*
	UINT32 dwResult = 0;
	int blink=0;
	int count=0;
	int x;
	UINT32 m6502NmiTicks = 0;
	UINT32 dwElapsedTicks = 0;
	
        count++;b=0;c=0;
	if (count==10){count=0;blink^=1;}
					
	for (x=0; x<6; x++)
	 {
				dwResult = m6502exec(6250); //6250
		        if (0x80000000 != dwResult)
				{
					m6502GetContext(psCpu1);
					allegro_message("Invalid instruction at %.2x\n", psCpu1->m6502pc);
					exit(1);
				}
				if (!testsw ){m6502nmi();}

		 }
		
		
         log_it ("\n FRAME!!!!!!!!!!!!!??????????????????????????????????????????????????????????????? \n");
		 if (KeyCheck(config.ktest))      {m6502reset();testsw^=1;}
         if (KeyCheck(config.kreset))     {m6502reset();}
		
		 		              
		 if (ldvggo) {
			           ldvg_generate_vector_list();
					   ldvggo=0;
		 }
						
						/* 
												
						 if (config.artwork){
							   
								blit_any_tex(&art_tex[0],6,1,0,0,1024,164); //bright //6
								if (b==6 && c==6) {
									         
											 if (blink ){
								              blit_any_tex(&art_tex[3],1,1,0,0,1023,164);
											  blit_any_tex(&art_tex[4],1,1,0,0,1023,164);
											  blit_any_tex(&art_tex[5],1,1,0,0,1023,164);
											  blit_any_tex(&art_tex[6],1,1,0,0,1023,164);
											 }
								             } 
								else {
								
							
								switch(pic){
									 case 1:blit_any_tex(&art_tex[2],1,1,0,0,1024,164);break;
									 case 2:blit_any_tex(&art_tex[3],1,1,0,0,1024,164);break;
									 case 3:blit_any_tex(&art_tex[4],1,1,0,0,1024,164);break;
									 case 4:blit_any_tex(&art_tex[5],1,1,0,0,1024,164);break;
									 case 5:blit_any_tex(&art_tex[6],1,1,0,0,1024,164);break;
									
								          }
								}								
								 if (lamp0){blit_any_tex(&art_tex[2],1,1,0,0,1024,164);}
								blit_any_tex(&art_tex[1],15,.70,0,0,1024,164);//dim
				
						 }
						
		*/

		
	
	    //if (KeyCheck(config.kreset))  {cpu_needs_reset(0);}
   //  if (getport(0) & 0x80) {cpu_disable_interrupts(0,0);} else {cpu_disable_interrupts(0,1);}
	
	//log_it("Watchdog this frame %x",WATCHDOG);
	//if (WATCHDOG==0) k++;
	//if (k > 60) {cpu_needs_reset(0);k=0;}

    WATCHDOG=0;

   // if (KeyCheck(config.ktest))      {testsw^=1; cpu_needs_reset(0);}
	//if (KeyCheck(config.kreset))  {cpu_needs_reset(0);}
	//if (testsw) cpu_disable_interrupts(0,0); else cpu_disable_interrupts(0,1);
	

	if (ldvggo) {ldvg_generate_vector_list(); ldvggo=0; }
}

/////////////////// MAIN() for program ///////////////////////////////////////////////////
int init_llander(void)
{
    
    
	init6502(LlanderRead,LlanderWrite,0);
	
	
    /*
	if (config.artwork){
	make_single_bitmap( &art_tex[0],"lunaroff.png","llander.zip");
    make_single_bitmap( &art_tex[1],"lunaron.png","llander.zip"); 
    make_single_bitmap( &art_tex[2],"lunarins.png","llander.zip"); 
	make_single_bitmap( &art_tex[3],"lunartrn.png","llander.zip");
	make_single_bitmap( &art_tex[4],"lunarcad.png","llander.zip");
	make_single_bitmap( &art_tex[5],"lunarprm.png","llander.zip");
	make_single_bitmap( &art_tex[6],"lunarcom.png","llander.zip");	
	}
	*/
	GI[CPU0][0x2403] =  0x80;
	sample_start(1,0,1);
	set_ll_colors();
//	cache_clear();
	setup_ambient(VECTOR);
    
	return 0;   
}


void end_llander()
{

}
//////////////////  END OF MAIN PROGRAM /////////////////////////////////////////////
