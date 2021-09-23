/*Sega Emu */
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

// << Multiply >> Divide  (I always forget)
#include "SegaG80.h"
//#include "mz80.h"
#include "segacrypt.c"
#include "globals.h"
#include "samples.h"
#include "vector.h"
//#include "glcode.h"
#include "dips.h"
//#include "keysets.h"
#include "input.h"
#include "cpu_control.h" 
//#include "dxinput.h"
#include "osd_cpu.h"

//#define PI 3.1415926535

extern char *gamename[];
extern int gamenum;

#define MAKE_RGB(r,g,b) 	((((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))

INLINE UINT8 pal2bit(UINT8 bits)
{
	bits &= 3;
	return (bits << 6) | (bits << 4) | (bits << 2) | bits;
}


#define VECTOR_COLOR222(c) \
	MAKE_RGB(pal2bit((c) >> 4), pal2bit((c) >> 2), pal2bit((c) >> 0))
/*
static struct AY8910interface interface =
{
   1,
   1500000,
   { 8},
   {2},
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 }
};
*/

int sega_rotate=0;
int vectorram_size =0;
static long int *sinTable, *cosTable;
int nodecrypt=0;
static int intensity;
//MAJOR VARIABLES
unsigned char mult1;
unsigned short result;
unsigned char ioSwitch;
long int mz80clockticks=0;
int coin_in=0;
int coin_count=0;

//float r,g,b;
BYTE *vectorram;
int sysReset=0;
int spinvert = 0;
int toggle_mouse=1;
int lastcolor=0;

static UINT8 spinner_select;
static UINT8 spinner_sign;
static UINT8 spinner_count;

#define VECTOR_CLOCK		15468480			/* master clock */
#define U34_CLOCK			(VECTOR_CLOCK/3)	/* clock for interrupt chain */
#define VCL_CLOCK			(U34_CLOCK/2)		/* clock for vector generator */
#define U51_CLOCK			(VCL_CLOCK/16)		/* clock for phase generator */
#define IRQ_CLOCK			(U34_CLOCK/0x1f788)	/* 40Hz interrupt */


static int min_x, min_y;


/*

    Vector system is clocked by a 15-phase clock.

    The counter is a LS161 4-bit binary counter at U51, and its output
    goes to a LS154 1-of-16 decoder at U50.

    Each phase various things happen. The phases are:

     0 -> (sheet 7/7) clocks CD7 in

     1 -> (sheet 5/7) loads CD0-7 into counters at U15/U16
          (sheet 6/7) clear LS175 flip flops at U35, U36, U37, U38

     2 -> (sheet 5/7) loads CD0-3 into counter at U17

     3 -> (sheet 5/7) loads CD0-7 into counters at U18/U19

     4 -> (sheet 5/7) loads CD0-3 into counter at U20

     5 ->

     6 ->

     7 -> (sheet 6/7) at end, latches CD0-7 into LS374 tri-state flip flop at U55 (SYM angle)

     8 -> (sheet 6/7) at end, latches CD0-1 into LS74 flip flops at U26 (upper SYM angle)

     9 -> (sheet 4/7) at end, latches CD0-7 into 25LS14 multiplier X input at U8 (scale)

    10 -> (sheet 4/7) at end, latches CD0-CD7 into LS374 tri-state flip flop at U2 (attributes)
          (sheet 7/7) at end, latches CD7 into U52 (low), which sets the preload value
            for the LS161 at U51 to be either 0 (if CD7==1) or 10 (if CD7==0)

    11 -> (sheet 4/7) at end, starts multiply circuit

    12 -> (sheet 6/7) at end, latches CD0-7 into LS374 tri-state flip flop at U56 (VEC angle)

    13 -> (sheet 6/7) at end, latches output from 2708 PROM into tri-state flip flop at U48
          (sheet 6/7) at end, latches bit $200 of angle into D/UX output

    14 -> (sheet 6/7) at end, latches output from 2708 PROM into tri-state flip flop at U49
          (sheet 6/7) at end, latches bit $200 of angle into D/UY output
          (sheet 7/7) signals /PE on the LS161 at U51, loading the new value for the state clock
          (sheet 7/7) sets up the DRAW signal to clock on the next VCL edge

    15 ->



    PROM inputs:
        A0 = GND
        A1-A8 = sum of VEC angle and SYM angle (low 8 bits)
        A9 = sum of bit 8 of VEC angle and SYM angle, plus 1 for phase 13


*/


INLINE int adjust_xy(int rawx, int rawy, int *outx, int *outy)
{
	int clipped = FALSE;

	/* first apply the XOR at 0x200 */
	*outx = (rawx & 0x7ff) ^ 0x200;
	*outy = (rawy & 0x7ff) ^ 0x200;

	/* apply clipping logic to X */
	if ((*outx & 0x600) == 0x200)
		*outx = 0x000, clipped = TRUE;
	else if ((*outx & 0x600) == 0x400)
		*outx = 0x3ff, clipped = TRUE;
	else
		*outx &= 0x3ff;

	/* apply clipping logic to Y */
	if ((*outy & 0x600) == 0x200)
		*outy = 0x000, clipped = TRUE;
	else if ((*outy & 0x600) == 0x400)
		*outy = 0x3ff, clipped = TRUE;
	else
		*outy &= 0x3ff;

	/* convert into .16 values */
	*outx = (*outx - (min_x - 512)) << 16;
	*outy = (*outy - (min_y - 512)) << 16;
	return clipped;
}


void sega_generate_vector_list(void)
{
	UINT8 *sintable = GI[1];
	double total_time = 1.0 / (double)IRQ_CLOCK;
	UINT16 symaddr = 0;

	int red=0;
	int green=0;
	int blue=0;

	int sx,sy,ex,ey;
	sx=0;
	sy=0;
	ex=0;
	ey=0;

//    cache_clear();
	/* Loop until we run out of time. */
	while (total_time > 0)
	{
		UINT16 curx, cury, xaccum, yaccum;
		UINT16 vecaddr, symangle;
		UINT8 scale, draw;

		/* The "draw" flag is clocked at the end of phase 0. */
		draw = vectorram[symaddr++ & 0xfff];

		/* The low byte of the X coordinate is latched into the        */
		/* up/down counters at U15/U16 during phase 1. */
		curx = vectorram[symaddr++ & 0xfff];

		/* The low 3 bits of the high byte of the X coordinate are     */
		/* latched into the up/down counter at U17 during phase 2.     */
		/* Bit 2 of the input is latched as both bit 2 and 3. */
		curx |= (vectorram[symaddr++ & 0xfff] & 7) << 8;
		curx |= (curx << 1) & 0x800;

		/* The low byte of the Y coordinate is latched into the        */
		/* up/down counters at U18/U19 during phase 3. */
		cury = vectorram[symaddr++ & 0xfff];

		/* The low 3 bits of the high byte of the X coordinate are     */
		/* latched into the up/down counter at U17 during phase 4.     */
		/* Bit 2 of the input is latched as both bit 2 and 3. */
		cury |= (vectorram[symaddr++ & 0xfff] & 7) << 8;
		cury |= (cury << 1) & 0x800;

		/* The low byte of the vector address is latched into the      */
		/* counters at U10/U11 during phase 5. */
		vecaddr = vectorram[symaddr++ & 0xfff];

		/* The low 4 bits of the high byte of the vector address is    */
		/* latched into the counter at U12 during phase 6. */
		vecaddr |= (vectorram[symaddr++ & 0xfff] & 0xf) << 8;

		/* The low byte of the symbol angle is latched into the tri-   */
		/* state flip flop at U55 at the end of phase 7. */
		symangle = vectorram[symaddr++ & 0xfff];

		/* The low 2 bits of the high byte of the symbol angle are     */
		/* latched into flip flops at U26 at the end of phase 8. */
		symangle |= (vectorram[symaddr++ & 0xfff] & 3) << 8;

		/* The scale is latched in phase 9 as the X input to the       */
		/* 25LS14 multiplier at U8. */
		scale = vectorram[symaddr++ & 0xfff];

		/* Account for the 10 phases so far. */
		total_time -= 10.0 / (double)U51_CLOCK;

		/* Skip the rest if we're not drawing this symbol. */
		if (draw & 1)
		{
			int adjx, adjy, clipped;

			/* Add a starting point to the vector list. */
			clipped = adjust_xy(curx, cury, &adjx, &adjy);
			if (!clipped) sx=adjx;sy=adjy;
				//vector_add_point(adjx, adjy, 0, 0);
            
			/* Loop until we run out of time. */
			while (total_time > 0)
			{
				UINT16 vecangle, length, deltax, deltay;
				UINT8 attrib, intensity;
				UINT32 color;

				/* The 'attribute' byte is latched at the end of phase 10 into */
				/* the tri-state flip flop at U2. The low bit controls whether */
				/* or not the beam is enabled. Bits 1-6 control the RGB color  */
				/* (2 bits per component). In addition, bit 7 of this value is */
				/* latched into U52, which controls the pre-load value for the */
				/* phase generator. If bit 7 is high, then the phase generator */
				/* will reset back to 0 and draw a new symbol; if bit 7 is low */
				/* the phase generator will reset back to 10 and draw another  */
				/* vector. */
				attrib = vectorram[vecaddr++ & 0xfff];

				/* The length of the vector is loaded into the shift registers */
				/* at U6/U7 during phase 11. During phase 12, the 25LS14       */
				/* multiplier at U8 is used to multiply the length by the      */
				/* scale that was loaded during phase 9. The length is clocked */
				/* bit by bit out of U6/U7 and the result is clocked into the  */
				/* other side. After the multiply, the 9 MSBs are loaded into  */
				/* the counter chain at U15/16/17 and are used to count how    */
				/* long to draw the vector. */
				length = (vectorram[vecaddr++ & 0xfff] * scale) >> 7;

				/* The vector angle low byte is latched at the end of phase 12 */
				/* into the tri-state flip flop at U56. */
				vecangle = vectorram[vecaddr++ & 0xfff];

				/* The vector angle high byte is preset on the CD bus during   */
				/* phases 13 and 14, and is used as inputs to the adder at     */
				/* U46. */
				vecangle |= (vectorram[vecaddr++ & 0xfff] & 3) << 8;

				/* The X increment value is looked up first (phase 13). The    */
				/* sum of the latched symbol angle and the vector angle is     */
				/* used as input to the PROM at U39. A0 is tied to ground.     */
				/* A1-A9 map to bits 0-8 of the summed angles. The output from */
				/* the PROM is latched into U48. */
				deltax = sintable[((vecangle + symangle) & 0x1ff) << 1];

				/* The Y increment value is looked up second (phase 14). The   */
				/* angle sum is used once again as the input to the PROM, but  */
				/* this time an additional 0x100 is effectively added to it    */
				/* before it is used; this separates sin from cos. The output  */
				/* from the PROM is latched into U49. */
				deltay = sintable[((vecangle + symangle + 0x100) & 0x1ff) << 1];
               	/* Account for the 4 phases for data fetching. */
				total_time -= 4.0 / (double)U51_CLOCK;

				/* Compute color/intensity values from the attributes */
				color = VECTOR_COLOR222((attrib >> 1) & 0x3f);
				//log_it("color %x",color);
				blue=color & 0xff;
				green = (color>>8)& 0xff;
                red=(color>>16)& 0xff;
				
				if ((attrib & 1) && color)
					intensity = 0xff;
				else
					intensity = 0;

				/* Loop over the length of the vector. */
				clipped = adjust_xy(curx, cury, &adjx, &adjy);
				sx=adjx;
				sy=adjy;
				xaccum = 0;
				yaccum = 0;
				while (length-- != 0 && total_time > 0)
				{
					int newclip;

					/* The adders at U44/U45 are used as X accumulators. The value */
					/* from U48 is repeatedly added to itself here. The carry out  */
					/* of bit 8 clocks the up/down counters at U15/U16/U17. Bit 7  */
					/* of the input value from U48 is used as a carry in to round  */
					/* small values downward and larger values upward. */
					xaccum += deltax + (deltax >> 7);

					/* Bit 9 of the summed angles controls the direction the up/   */
					/* down counters at U15/U16/U17. */
					if (((vecangle + symangle) & 0x200) == 0)
						curx += xaccum >> 8;
					else
						curx -= xaccum >> 8;
					xaccum &= 0xff;

					/* The adders at U46/U47 are used as Y accumulators. The value */
					/* from U49 is repeatedly added to itself here. The carry out  */
					/* of bit 8 clocks the up/down counters at U18/U19/U20. Bit 7  */
					/* of the input value from U49 is used as a carry in to round  */
					/* small values downward and larger values upward. */
					yaccum += deltay + (deltay >> 7);

					/* Bit 9 of the summed angles controls the direction the up/   */
					/* down counters at U18/U19/U20. */
					if (((vecangle + symangle + 0x100) & 0x200) == 0)
						cury += yaccum >> 8;
					else
						cury -= yaccum >> 8;
					yaccum &= 0xff;

					/* Apply the clipping from the DAC circuit. If the values clip */
					/* the beam is turned off, but the computations continue right */
					/* on going. */
					newclip = adjust_xy(curx, cury, &adjx, &adjy);
					if (newclip != clipped)
					{
						/* if we're just becoming unclipped, add an empty point */
						if (!newclip) {;}//sx=adjx;sy=adjy;}
							//vector_add_point(adjx, adjy, 0, 0);

						/* otherwise, add a colored point */
						else {
							if (intensity){	
                              
								if (gamenum==TACSCAN) {
   int x0 = (sx>>11)-16000;
   int y0 = -(sy>>11)+16000;
   int x1 = (adjx>>11)-16000;
   int y1 = -(adjy>>11)+16000;
   int zz = (red+green+blue)/3/2;
//									                   add_color_line( (sy>>16),(sx>>16),(adjy>>16), (adjx>>16),red,green,blue);
   v_directDraw32(y0, x0, y1,x1,zz);
//													   add_color_point((sy>>16),(sx>>16),red,green,blue);
//								                       add_color_point((adjy>>16), (adjx>>16),red,green,blue);
								                      }
							                          
								else  {
//									   add_color_line((sx>>16), (sy>>16),(adjx>>16), (adjy>>16), red,green,blue);
   int x0 = (sx>>11)-16000;
   int y0 = (sy>>11)-12000;
   int x1 = (adjx>>11)-16000;
   int y1 = (adjy>>11)-12000;
   int zz = (red+green+blue)/3/2;

   v_directDraw32(x0, y0, x1,y1,zz);
                                       
                                       
//									   add_color_point((sx>>16),(sy>>16),red,green,blue);
//								       add_color_point((adjx>>16), (adjy>>16),red,green,blue);
								      }
							
								
								}
														
							//log_it("LINE from sx %d sy %d to endx %d endy %d",sx>>16,sy>>16,adjx>>16,adjy>>16);
						     } 
							
					}
					clipped = newclip;

					/* account for vector drawing time */
					total_time -= 1.0 / (double)VCL_CLOCK;
				}

				/* We're done; if we are not clipped, add a final point. */
				if (!clipped)
                {
								if (intensity)
                                {
                                  
								if (gamenum==TACSCAN) {
   int x0 = (sx>>11)-16000;
   int y0 = -(sy>>11)+16000;
   int x1 = (adjx>>11)-16000;
   int y1 = -(adjy>>11)+16000;
   int zz = (red+green+blue)/3/2;
                                  
   v_directDraw32(y0, x0, y1,x1,zz);
//									                   add_color_line( (sy>>16),(sx>>16),(adjy>>16), (adjx>>16),red,green,blue);
//													   add_color_point((sy>>16),(sx>>16),red,green,blue);
//								                       add_color_point((adjy>>16), (adjx>>16),red,green,blue);
								                      }
							                          
								else  {
   int x0 = (sx>>11)-16000;
   int y0 = (sy>>11)-12000;
   int x1 = (adjx>>11)-16000;
   int y1 = (adjy>>11)-12000;
   int zz = (red+green+blue)/3/2;
                                  
                                      
   
   v_directDraw32(x0, y0, x1,y1,zz);
                                  
//									   add_color_line((sx>>16), (sy>>16),(adjx>>16), (adjy>>16), red,green,blue);
//									   add_color_point((sx>>16),(sy>>16),red,green,blue);
//								       add_color_point((adjx>>16), (adjy>>16),red,green,blue);
								      }
							
								
								}
								     
							}
					//vector_add_point(adjx, adjy, color, intensity);

				/* if the high bit of the attribute is set, we break out of   */
				/* this loop and fetch another symbol */
				if (attrib & 0x80)
					break;
			}
		}

		/* if the high bit of the draw flag is set, we break out of this loop */
		/* and stop the rendering altogether for this frame. */
		if (draw & 0x80)
			break;
	}
}



/***************************************************************************

  Stop the video hardware emulation.

***************************************************************************/

static int coin_routine(int bits)
{
   
   if (coin_in > 0 && coin_in < 4){ bitclr(bits, 0x20);  coin_in++;}
     
  return bits;

}



static void BWVectorGenerator(void)
{      
     sega_generate_vector_list();
}



static void swap(int *i, int *j)
{
    int t;
    t = *i;
    *i = *j;
    *j = t;
}

READ_HANDLER(VectorRam_r)
{ return vectorram[address-0xe000];}

READ_HANDLER(SoundRam)
{ return 0x00; }


WRITE_HANDLER(Vector_Write)
{
  int op =0;
  int page=0;
  int val=0; //unsigned
  int offset=0;
  unsigned int bad=0;//
  int test=0;
   
  	
        	   
        val=mz80GetRegisterValue(0, CPUREG_PC);
		//log_it("OP Handler, Z80PC is %x",mz80GetRegisterValue(temp, CPUREG_PC));	
		test=val;
		val=val-3;
		
		op=cMZ80[0].z80Base[val]& 0xff;
		//log_it("current %x -3 val %x",val,test);
		if (op==0x32){
			
			bad = cMZ80[0].z80Base[(val+1)] & 0xFF;
			page = (cMZ80[0].z80Base[(val+2)] & 0xFF) << 8;
			(*sega_decrypt)(val,&bad);
						
			offset = (page & 0xFF00) | (bad & 0x00FF);
			//log_it("VAL %x, OP %x BAD %x PAGE %x ADDRESS %x OFFSET %x",val,op,bad,page,address,offset);
			
			if (nodecrypt) {offset=address;}
			else {address=offset;}
			//if ((offset>=0x0000) && (offset<=0xbfff)){;}
			//if (offset >= 0xc800 && offset <= 0xdfff ) {cMZ80[0]->z80Base[offset]=data;} //memory+soundram
			//if (offset >= 0xe000 && offset <= 0xefff ) {vectorram[offset-0xe000]=data;cMZ80[0]->z80Base[offset]=data;} //vectorram
		 
		}
		
		if ((address>=0x0000) && (address<=0xbfff)){;}
		else if (address >= 0xc800 && address <= 0xdfff ){cMZ80[0].z80Base[address]=data;}
		else if (address >= 0xe000 && address <= 0xefff ){vectorram[address-0xe000]=data;cMZ80[0].z80Base[address]=data; }
		//if (op!=0x32){
		// if ((address>=0x0000) && (address<=0xbfff)){;}
		 //if (address >= 0xc800 && address <= 0xdfff ){z80.z80Base[address]=data;}
		 //if (address >= 0xe000 && address <= 0xefff ){vectorram[address-0xe000]=data;z80.z80Base[address]=data; }
		//}
	    
}  


static int sega_fix_dips(int port, int val)
{
	int dip1, dip2;

	dip1 = z80dip1;
	dip2 = z80dip2;
	switch(port)
	{
		case 0xf8:
			return ((val & 0xF0) | ((dip2 & 0x08)>>3) |
				 ((dip2 & 0x80)>>6) | ((dip1 & 0x08)>>1) | ((dip1 & 0x80)>>4));
		case 0xf9:
			return ((val & 0xF0) | ((dip2 & 0x04)>>2) |
				 ((dip2 & 0x40)>>5) | ((dip1 & 0x04)>>0) | ((dip1 & 0x40)>>3));
		case 0xfa:
			return ((val & 0xF0) | ((dip2 & 0x02)>>1) |
				 ((dip2 & 0x20)>>4) | ((dip1 & 0x02)<<1) | ((dip1 & 0x20)>>2));
		case 0xfb:
			return ((val & 0xF0) | ((dip2 & 0x01)>>0) |
				 ((dip2 & 0x10)>>3) | ((dip1 & 0x01)<<2) | ((dip1 & 0x10)>>1));
	}
	return 0;
}

PORT_READ_HANDLER(sega_sh_r)
{
  if (sample_playing(0))
		return 0x81;
	else
		return 0x80;
}
PORT_READ_HANDLER(sega_mult_r)
{	
	int c;

	c = result & 0xff;
	result >>= 8;
	return (c);
}

PORT_READ_HANDLER(sega_ports_r)
{
  int bits=0xe0;
      
  switch (port) 
  {  

    case 0xf8: return sega_fix_dips(port,getport(0));break;
      
    case 0xf9: return sega_fix_dips(port,getport(1));break;//return keyin;break;

    case 0xfa: return sega_fix_dips(port,getport(2));break;
              
    case 0xfb: return sega_fix_dips(port,getport(3));break;
  
    case 0xfc: return getport(4);break;//bits;
  }
  return 0;
}
PORT_READ_HANDLER(sega_ports_r2)
{
  INT8 delta=0;



  switch (port) 
  {  
    case 0xf8: return sega_fix_dips(port,getport(0));break;
      
    case 0xf9: return sega_fix_dips(port,getport(1));break;

    case 0xfa: return sega_fix_dips(port,getport(2));break;
              
    case 0xfb: return sega_fix_dips(port,getport(3));break;
      
    case 0xfc: 
            
          if (ioSwitch & 1) // ioSwitch = 0x01 or 0xff 
          return getport(4);//+0xf0;
          
          // I'm sure this can be further simplified ;-)
          delta = getport(6);
          
              if (delta != 0)
              {
                  spinner_sign = (delta >> 7) & 1;
                  spinner_count += abs(delta);
              }
      return ~((spinner_count << 1) | spinner_sign);
          break;

  }
  return 0;
}
PORT_READ_HANDLER(sega_ports_r3)
{
  static int count=0;
  INT8 delta=0;
  static int sign;
  static INT8 counter;
  int keyin=0;

  int bits=0xe0;
  switch (port) 
  {  

    case 0xf8:  bits=coin_routine(bits);//if (key[config.kcoin1]){ bitclr(bits, 0x80); }
              return sega_fix_dips(port,bits);//0xeb;break;//e7
        
    case 0xf9: bits=0xf0;
        
              if (INPUT_START1){bitclr(bits, 0x20);}
              if (INPUT_START2){bitclr(bits, 0x10);}
    
    //	        if (key[config.kstart1]){bitclr(bits, 0x20);}
    //			if (key[config.kstart2]){bitclr(bits, 0x10);}
                return sega_fix_dips(port,bits);//return keyin;break;

    case 0xfa:  bits=0xf0; 
              if (INPUT_P1_BUTTON2) {bitclr(bits, 0x10);}
              if (INPUT_P1_DIGITAL_RIGHT){bitclr(bits, 0x20);}
              if (INPUT_P1_DIGITAL_LEFT) {bitclr(bits, 0x40);}
    //           if (key[config.kp1but2]) {bitclr(bits, 0x10);}
    //           if (key[config.kp1right]){bitclr(bits, 0x20);}
    //           if (key[config.kp1left]) {bitclr(bits, 0x40);}
                return sega_fix_dips(port,bits);
                
    case 0xfb:  bits=0xf0;
              if (INPUT_P2_DIGITAL_LEFT) {bitclr(bits, 0x10);}
              if (INPUT_P1_BUTTON1) {bitclr(bits, 0x20);}
    //           if (key[config.kp2left]) {bitclr(bits, 0x10);}
    //           if (key[config.kp1but1]) {bitclr(bits, 0x20);}
                return sega_fix_dips(port,bits);
      
    case 0xfc: bits=0x00; 
              if (INPUT_P2_DIGITAL_RIGHT) {bitset(bits, 0x04);}
              if (INPUT_P2_BUTTON1)  {bitset(bits, 0x01);}
              if (INPUT_P2_BUTTON2)  {bitset(bits, 0x02);}
    //          if (key[config.kp2right]) {bitset(bits, 0x04);}
    //          if (key[config.kp2but1])  {bitset(bits, 0x01);}
    //          if (key[config.kp2but2])  {bitset(bits, 0x02);}
                return bits;
                
                
  }	
  return 0;
}


PORT_WRITE_HANDLER(sega_switch_w)
{ioSwitch = data;}
PORT_WRITE_HANDLER(sega_mult1_w)
{mult1 = data;}
PORT_WRITE_HANDLER(sega_mult2_w)
{result = mult1 * data;}
PORT_WRITE_HANDLER(sega_coin_counter_w)
{;}//log_it("coin counter write");}

///////////////////////  MAIN LOOP /////////////////////////////////////
void run_segag80(void)
{
//	log_it("FRAME-------------------------------");
            if (INPUT_TEST)     {set_pending_interrupt(INT_TYPE_NMI, 0);}
            if ((INPUT_COIN1) || (INPUT_COIN2) )    {coin_in=1; }  
            if (INPUT_RESET)    {cpu_needs_reset(0);}
//            if (KeyCheck(config.ktest))     {set_pending_interrupt(INT_TYPE_NMI, 0);}
//            if (KeyCheck(config.kcoin1) || KeyCheck(config.kcoin2) )    {coin_in=1; }  
//            if (KeyCheck(config.kreset))    {cpu_needs_reset(0);}
			
				
			BWVectorGenerator();
			if (gamenum==TACSCAN)
			{
				tacscan_sh_update();
			}
			else 
				sega_sh_update(); 
			
			
	
}


struct MemoryWriteByte SegaWrite[] =
{
	//{ 0x0000,  0xc7ff, ProgRom },
	{ 0x0000,  0xffff, Vector_Write },
	//{ 0xe000,  0xefff, Vector_Write },
	{(UINT32) -1,	(UINT32) -1,		NULL}
};

struct MemoryReadByte SegaRead[] =
{
	{ 0xe000, 0xefff, VectorRam_r },
	{ 0xd000, 0xdfff, SoundRam },
	{ 0xf000, 0xffff, SoundRam },
	{(UINT32) -1,	(UINT32) -1,		NULL}
};

struct z80PortRead SpacfuryPortRead[] =
{
	{ 0x3f, 0x3f, sega_sh_r },
	{ 0xbe, 0xbe, sega_mult_r },
	{ 0xf8, 0xfc,  sega_ports_r },
	{-1,	  -1,	  NULL}
};

struct z80PortRead G80SpinPortRead[] =
{
	{ 0x3f, 0x3f, sega_sh_r },
	{ 0xbe, 0xbe, sega_mult_r },
	{ 0xf8, 0xfc, sega_ports_r2 },
	{-1,	  -1,	  NULL}
};
struct z80PortRead ElimPortRead[] =
{
	{ 0x3f, 0x3f, sega_sh_r },
	{ 0xbe, 0xbe, sega_mult_r },
	{ 0xf8, 0xfc, sega_ports_r3 },
	{-1,	  -1,	  NULL}
};
PORT_WRITE(SpacfuryPortWrite)
 PORT_ADDR(0x38, 0x38, sega_sh_speech_w)
 PORT_ADDR(0x3e, 0x3e, spacfury1_sh_w)
 PORT_ADDR(0x3f, 0x3f, spacfury2_sh_w)
 PORT_ADDR(0xbd, 0xbd, sega_mult1_w)
 PORT_ADDR(0xbe, 0xbe, sega_mult2_w)
 PORT_ADDR(0xf8, 0xf8, sega_coin_counter_w) /* 0x80 = enable, 0x00 = disable */
 PORT_ADDR(0xf9, 0xf9, sega_coin_counter_w) /* 0x80 = enable, 0x00 = disable */
PORT_END

struct z80PortWrite ElimPortWrite[] =
{
	{ 0x38, 0x38, sega_sh_speech_w },
	{ 0x3e, 0x3e, elim1_sh_w },
	{ 0x3f, 0x3f, elim2_sh_w },
	{ 0xbd, 0xbd, sega_mult1_w },
	{ 0xbe, 0xbe, sega_mult2_w },
	{ 0xf9, 0xf9, sega_coin_counter_w }, /* 0x80 = enable, 0x00 = disable */
	{-1,	-1,	NULL}
};

struct z80PortWrite ZektorPortWrite[] =
{
	{ 0x38, 0x38, sega_sh_speech_w },
	{ 0x3c, 0x3c,Zektor_AY8910_w, NULL },
	//{ 0x3d, 0x3d, Zektor_AY8910_w, NULL },
	{ 0x3e, 0x3e, Zektor1_sh_w },
	{ 0x3f, 0x3f, Zektor2_sh_w },
	{ 0xbd, 0xbd, sega_mult1_w },
	{ 0xbe, 0xbe, sega_mult2_w },
	{ 0xf8, 0xf8, sega_switch_w },
	{ 0xf9, 0xf9, sega_coin_counter_w }, /* 0x80 = enable, 0x00 = disable */
	{-1,	-1,	NULL}
};

struct z80PortWrite StarTrekPortWrite[] =
{
	{ 0x38, 0x38, sega_sh_speech_w },
	{ 0x3f, 0x3f, StarTrek_sh_w },
	{ 0xbd, 0xbd, sega_mult1_w },
	{ 0xbe, 0xbe, sega_mult2_w },
	{ 0xf8, 0xf8, sega_switch_w },
	{ 0xf9, 0xf9, sega_coin_counter_w }, /* 0x80 = enable, 0x00 = disable */
	{-1,	-1,	NULL}
};

struct z80PortWrite TacScanPortWrite[] =
{
	{ 0x38, 0x38, sega_sh_speech_w },
	{ 0x3f, 0x3f, TacScan_sh_w },
	{ 0xbd, 0xbd, sega_mult1_w },
	{ 0xbe, 0xbe, sega_mult2_w },
	{ 0xf8, 0xf8, sega_switch_w },
	{ 0xf9, 0xf9, sega_coin_counter_w }, /* 0x80 = enable, 0x00 = disable */
	{-1,	-1,	NULL}
};


int init_segag80()
{ 
  int goodload=0;
  int goodloads=0;
  int loop=0;
  
	
     nodecrypt=0;
     sega_rotate=0;

      min_x =512;
	  min_y =512;

	   vectorram=(unsigned char *)malloc(0x1000);
	
	switch (gamenum)
	{
	case ZEKTOR:initz80N(SegaRead,SegaWrite,G80SpinPortRead, ZektorPortWrite,0);NUM_SPEECH_SAMPLES=NUM_ZEKTOR_SPEECH;sega_security(82);break;
	case TACSCAN:sega_rotate=1;initz80N(SegaRead,SegaWrite,G80SpinPortRead, TacScanPortWrite,0);sega_security(76);break;
	case STARTREK:initz80N(SegaRead,SegaWrite,G80SpinPortRead, StarTrekPortWrite,0);NUM_SPEECH_SAMPLES=NUM_STARTREK_SPEECH;sega_security(64);break;
	case SPACFURY:initz80N(SegaRead,SegaWrite,SpacfuryPortRead, SpacfuryPortWrite,0);NUM_SPEECH_SAMPLES=NUM_SPACFURY_SPEECH;sega_security(0);Decrypt(0);nodecrypt=1;break;
	case SPACFURA:initz80N(SegaRead,SegaWrite,SpacfuryPortRead, SpacfuryPortWrite,0);NUM_SPEECH_SAMPLES=NUM_SPACFURY_SPEECH;sega_security(0);Decrypt(5);nodecrypt=1;break;
	case SPACFURB:initz80N(SegaRead,SegaWrite,SpacfuryPortRead, SpacfuryPortWrite,0);NUM_SPEECH_SAMPLES=NUM_SPACFURY_SPEECH;sega_security(0);Decrypt(6);nodecrypt=1;break;
	case ELIM2:initz80N(SegaRead,SegaWrite,ElimPortRead, ElimPortWrite,0);sega_security(70);break;
	case ELIM2A:initz80N(SegaRead,SegaWrite,ElimPortRead, ElimPortWrite,0);sega_security(70);break;
	case ELIM2C:initz80N(SegaRead,SegaWrite,ElimPortRead, ElimPortWrite,0);sega_security(70);break;
	case ELIM4:initz80N(SegaRead,SegaWrite,ElimPortRead, ElimPortWrite,0);sega_security(76);break;
	case ELIM4P:initz80N(SegaRead,SegaWrite,ElimPortRead, ElimPortWrite,0);sega_security(76);break;
	}
	z80dip2=0xcc; //This is coin info, I don't care about it!! Set to 1 coin 1 play
	
   
    sega_sh_start ();
	mz80reset();
   	return 0;  
	
}

void end_segag80()
{
    //sega_vh_stop ();
    free (GI[1]);
	free (vectorram);
	save_dips();
    //save_sega_hi();
	sega_rotate=0;	
}