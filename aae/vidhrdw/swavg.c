/*
 * vector.c: Atari DVG and AVG simulators
 *
 * Copyright 1991, 1992, 1996 Eric Smith
 *
 * Modified for the MAME project 1997 by
 * Brad Oliver, Bernd Wiebelt & Aaron Giles
 */

#include <string.h>
#include <stdlib.h>
#include <vectrex/vectrexInterface.h>


#include "../driver.h"
#include "swavg.h"
#include "../log.h"
#include "../vector.h"
#undef VG_DEBUG

int dvg = 0;

int x_res;	/* X-resolution of the display device */
int y_res;	/* Y-resolution of the display device */

/* This struct holds the actual X/Y coordinates the vector game uses.
 * Gets initialized in vg_init().
 */
static struct { int width; int height;
	 int x_cent; int y_cent;
	 int x_min; int x_max;
	 int y_min; int y_max; } vg_video;

int flip_word;	/* determines the endian-ness of the words read from vectorram */

/* ASG 080497 */
#define DVG_SHIFT 10		/* NOTE: must be even */
#define AVG_SHIFT 16		/* NOTE: must be even */
#define RES_SHIFT 16		/* NOTE: must be even */
#define VEC_SHIFT 16

#define DVG_X(x,xs)	( ( ((x) >> DVG_SHIFT/2) * ((xs) >> RES_SHIFT/2) ) >> (RES_SHIFT/2 + DVG_SHIFT/2) )
#define DVG_Y(y,ys)	( ( ((y) >> DVG_SHIFT/2) * ((ys) >> RES_SHIFT/2) ) >> (RES_SHIFT/2 + DVG_SHIFT/2) )

#define AVG_X(x,xs)	( ( ((x) >> AVG_SHIFT/2) * ((xs) >> RES_SHIFT/2) ) >> (RES_SHIFT/2 + AVG_SHIFT/2) )
#define AVG_Y(y,ys)	( ( ((y) >> AVG_SHIFT/2) * ((ys) >> RES_SHIFT/2) ) >> (RES_SHIFT/2 + AVG_SHIFT/2) )

int vg_step = 0; /* single step the vector generator */
int last_vgo_cyc=0;
int vgo_count=0;

int vg_busy = 0;
int vg_done_cyc = 0; /* cycle after which VG will be done */

unsigned char *vectorram; /* LBO 062797 */

#define MAXSTACK 8 	/* Tempest needs more than 4     BW 210797 */

#define VCTR 0
#define HALT 1
#define SVEC 2
#define STAT 3
#define CNTR 4
#define JSRL 5
#define RTSL 6
#define JMPL 7
#define SCAL 8

#define DVCTR 0x01
#define DLABS 0x0a
#define DHALT 0x0b
#define DJSRL 0x0c
#define DRTSL 0x0d
#define DJMPL 0x0e
#define DSVEC 0x0f

#define twos_comp_val(num,bits) ((num&(1<<(bits-1)))?(num|~((1<<bits)-1)):(num&((1<<bits)-1)))

char *avg_mnem[] = { "vctr", "halt", "svec", "stat", "cntr", "jsrl", "rtsl",
		 "jmpl", "scal" };

char *dvg_mnem[] = { "????", "vct1", "vct2", "vct3",
		     "vct4", "vct5", "vct6", "vct7",
		     "vct8", "vct9", "labs", "halt",
		     "jsrl", "rtsl", "jmpl", "svec" };

#define map_addr(n) (((n)<<1))
#define swmemrdwd(offset,PC,cyc) ((vectorram[offset]) | (vectorram[offset+1]<<8))
#define swmemrdwd_flip(offset,PC,cyc) ((vectorram[offset+1]) | (vectorram[offset]<<8))
//#define memrdwd_flip(address) ((GI[pc+1]) | (GI[pc]<<8)) 
#define vmax(x,y) (((x)>(y))?(x):(y))


#define bitget(p,m) ((p) & (m))
#define bitset(p,m) ((p) |= (m))
#define bitclr(p,m) ((p) &= ~(m))
#define bitflp(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))





int effective_z(int z, int statz)
{
	/* Star Wars blends Z and an 8-bit STATZ */
	/* STATZ of 128 should give highest intensity */
	
		z = (z * statz) / 8;//(translucency ? 12 : 8);
		if (z > 0xff)
			z = 0xff;
	
	return z;
}


static void swvector_timer (int deltax, int deltay)
{
	deltax = abs (deltax);
	deltay = abs (deltay);
	vg_done_cyc += vmax (deltax, deltay) >> (AVG_SHIFT+1);
}
extern uint32_t avgStart;
extern uint32_t avgEnd;

static void avg_draw_vector_list (void)
{
  CCNT0(avgStart);
  
//printf("swavg called\n");
	int pc;
	int sp;
	int stack [MAXSTACK];
    int scale=1;
	int statz;
//	int color;
	int currentx=0, currenty=0;
	int done = 0;
	int firstwd, secondwd;
	int opcode;
	int x, y, z, b, l, /*d, */ a;

	int deltax, deltay;
	int red=0;
	int green=0;
	int blue=0;
	
	pc = 0;
	sp = 0;
	statz = 0;
//	color = 0;
	
  

	if (flip_word) {
		firstwd = swmemrdwd_flip (map_addr (pc), 0, 0);
		secondwd = swmemrdwd_flip (map_addr (pc+1), 0, 0);
	} else {
		firstwd = swmemrdwd (map_addr (pc), 0, 0);
		secondwd = swmemrdwd (map_addr (pc+1), 0, 0);
	}
	if ((firstwd == 0) && (secondwd == 0)) {
		if (errorlog) fprintf (errorlog,"VGO with zeroed vector memory\n");
		return;
	}

	//scale = 0;		
	//currentx = vg_video.x_cent << AVG_SHIFT;		
	//currenty = vg_video.y_cent << AVG_SHIFT;		

	while (!done) {

     		firstwd = swmemrdwd_flip (map_addr (pc), 0, 0);
		    opcode = firstwd >> 13; pc++;
		    if (opcode == VCTR) { secondwd = swmemrdwd_flip (map_addr (pc), 0, 0); pc++;}
 		    else if (opcode == STAT && (firstwd & 0x1000))	opcode = SCAL;


	switch (opcode) {
		case VCTR:
			x = twos_comp_val (secondwd,13);
			y = twos_comp_val (firstwd,13);
			z = (secondwd >> 12) & 0x0e;
			z = effective_z(z, statz);
			
			goto DRAWME;
			break;

		case SVEC:
			x = twos_comp_val (firstwd, 5) * 2;
			y = twos_comp_val (firstwd >> 8, 5) * 2;
			z = (firstwd >> 4) & 0x0e;
			z = effective_z(z, statz);
			
DRAWME:
//printf("draw in swavg.c\n");
			deltax = x * scale;	deltay = y * scale;
			deltay = -deltay;
            if (z){
                z=z+1;
				if (z>255) z=255;
				if (red)  {red=z;}
				if (green){ green=z;}
				if (blue) { blue=z;}
			    
				 if ((currentx==(currentx)+deltax) && (currenty==(currenty)-deltay))
				 {
                   add_color_point((currentx>>VEC_SHIFT),currenty>>VEC_SHIFT, blue,green,red);
				}
				 else
				 {
				    add_color_line2((float) (currentx>>VEC_SHIFT), (float) (currenty>>VEC_SHIFT), (float) ((currentx+deltax)>>VEC_SHIFT),  (float) ((currenty-deltay)>>VEC_SHIFT),(int) blue, (int)green, (int)red);
//				    add_color_point((float) (currentx>>VEC_SHIFT), (float) (currenty>>VEC_SHIFT), blue, green,red);
//				    add_color_point((float) ((currentx+deltax)>>VEC_SHIFT),  (float) ((currenty-deltay)>>VEC_SHIFT), blue, green, red);
				}	
			 }

			 
/*              
#define MULSHIFT 5
#define X_OFFSET -14000
#define Y_OFFSET -12000
            
   v_directDraw32((currentx>>(VEC_SHIFT - MULSHIFT)) +X_OFFSET, (currenty>>(VEC_SHIFT - MULSHIFT))+Y_OFFSET, ((currentx+deltax)>>(VEC_SHIFT - MULSHIFT))+X_OFFSET,((currenty-deltay)>>(VEC_SHIFT - MULSHIFT))+Y_OFFSET,z/3);
*/              
              
              
              
              
			 
			 
			currentx += deltax;	currenty -= deltay;
			swvector_timer (deltax,deltay);
			break;

		case STAT:
			               
				//color = (firstwd >> 8) & 7;
				blue= (firstwd >> 8) & 0x04;
				red= (firstwd >> 8) & 0x01;
				green=(firstwd >> 8) & 0x02;
			   	statz = (firstwd &0xff);
        
			break;

		case SCAL:
			b = ((firstwd >> 8) & 7) + 8;
			l = ~firstwd & 0xff;
			scale = (l << 16) >> b;
			scale*=3;

			break;

		case CNTR:
			/*d = firstwd & 0xff*/;
			currentx = 375 << AVG_SHIFT;		
			currenty = 420 << AVG_SHIFT;		
	
			break;

		case RTSL:

			if (sp == 0) {
				if (errorlog)
					fprintf (errorlog,"\n*** Vector generator stack underflow! ***\n");
				done = 1;
				sp = MAXSTACK - 1;
			} else
				sp--;
			pc = stack [sp];
			break;

		case HALT:

			done = 1;
			break;

		case JMPL:
			a = firstwd & 0x1fff;
		    // if a = 0x0000, treat as HALT 
			if (a == 0x0000) done = 1;
			else pc = a;
			break;

		case JSRL:
			a = firstwd & 0x1fff;
			// if a = 0x0000, treat as HALT 
			if (a == 0x0000) done = 1;
			else {
				stack [sp] = pc;
				if (sp == (MAXSTACK - 1)) {
					if (errorlog)
						fprintf (errorlog,"\n*** Vector generator stack overflow! ***\n");
					done = 1;
					sp = 0;
				}
				else sp++;pc = a;
			}
			break;

		default:
			if (errorlog)
				fprintf (errorlog,"internal error\n");
		}
	}

	//close_page ();
	  CCNT0(avgEnd);

}

int vg_done (int cyc)
{
//  printf("swavg.c done called\n");
	if (cyc-last_vgo_cyc>vg_done_cyc)
		vg_busy = 0;
	return (!vg_busy);
}

void vg_go (int cyc)
{   
//  printf("swavg.c go called\n");


	if (errorlog) {
		vgo_count++;
		fprintf (errorlog,"VGO #%d cpu-cycle %d vector cycles %d\n",
			vgo_count, cpu_gettotalcycles(), vg_done_cyc);
	}
	last_vgo_cyc=cyc;
	vg_busy=1;
	vg_done_cyc = 8;
	
	avg_draw_vector_list ();
    /*
#ifndef NO_PI
    v_playAllSFX();
    v_doSound();    // not needed in IRQ Mode
    v_readButtons(); // not neededin IRQ mode
    v_readJoystick1Analog(); // not neededin IRQ mode
    v_WaitRecal();
#endif
*/    
}

void vg_reset (int offset, int data)
{
//  printf("swavg.c reset called\n");
	vg_busy = 0;
	if (errorlog)
		fprintf (errorlog,"vector generator reset @%04x\n",cpu_getpc());
}

int vg_init (int len, int usingDvg, int flip)
{
//  printf("swavg.c init called\n");
	if (usingDvg)
		dvg = 1;
	else
		dvg = 0;
	flip_word = flip;

	vg_step = 0;
	last_vgo_cyc = 0;
	vgo_count = 0;
	vg_busy = 0;
	vg_done_cyc = 0;

	vg_video.width =Machine->drv->visible_area.max_x-Machine->drv->visible_area.min_x;
	vg_video.height=Machine->drv->visible_area.max_y-Machine->drv->visible_area.min_y;
	vg_video.x_cent=(Machine->drv->visible_area.max_x+Machine->drv->visible_area.min_x)/2;
	vg_video.y_cent=(Machine->drv->visible_area.max_y+Machine->drv->visible_area.min_y)/2;
	vg_video.x_min=Machine->drv->visible_area.min_x;
	vg_video.y_min=Machine->drv->visible_area.min_y;
	vg_video.x_max=Machine->drv->visible_area.max_x;
	vg_video.y_max=Machine->drv->visible_area.max_y;

	return 0;
}

void vg_stop (void)
{
}

