//Consolidated Atari AVG Code, my interpretation.

#include <stdlib.h>
#include <string.h>

#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

#include "globals.h"
#include "aae_avg.h"
#include "cpu_control.h"
#include "vector.h"
//#include "glcode.h"
#include "log.h"
#include "acommon.h"

static int NEW_FRAME=0;
static int XFLIP=0;
static int YFLIP=0;
static int TYPE_BZ=0;
static int TYPE_TEMP=0;
static int TYPE_GV=0;
static int TYPE_MH=0;
static int TYPE_QU=0;
static int TYPE_SD=0;
static int TYPE_SW=0;
static int pc=0;
static int AVG_BUSY=0;
//static int TOTAL_LENGTH=0;
static int SCALEADJ=2;
static int vectorbank=0x18000;
static float sweep=0;
static int PCTOP=0x2000;

static void calc_sweep()
{
  sweep = 2.35 * total_length; // 2.25 = 1500 us * 1.512 (freq)
  //sweep = (((1775 * total_length)/ 1000000) * 1512); //1775
  //sweep = (double) (1512 * total_length) * (1512000 / 1000000000.0f);
}

void set_new_frame()
{
NEW_FRAME = 1; //TRUE!!
}

static void set_sd_colors(void)
{   
	int i,c=0;
	int *cmap;
	
	int colormapsd[]={  0,0,0, //should be zero0,255,255,
		                0,0,255,
						0,255,0,
						0,255,255, //???
						255,0,0,
						255,0,255,
						255,255,0,
						255,255,255,
						255,0,0,
						255,255,255,
						0,0,255,
						0,255,0,
						255,0,0,
						255,0,0,
						255,0,255,
						255,255,255};
						

	cmap=colormapsd;
		
	for (i=0;i<16;i++)
	{
	   vec_colors[i].r=cmap[c];
       vec_colors[i].g=cmap[c+1];
       vec_colors[i].b=cmap[c+2];
	   c+=3;
	}
}


static void set_bw_colors()
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
/*
static void MH_DRAW(int currentx, int currenty, int deltax, int deltay, int z, int color)
{
    float sy=0;
	float ey=0;
	float sx=0;
	float ex=0;
	static int sparkle=0;
	static int spkl_shift=0;
	int clip=0;
	int ywindow=0;
	int red,green,blue;
	int one,two;
	int draw=0;

    ey= ((currenty-deltay)>>VEC_SHIFT);   
	sy= currenty>>VEC_SHIFT;
	sx= (currentx>>VEC_SHIFT);
	sx= (currentx+deltax)>>VEC_SHIFT;
					                   
                    if (sparkle)
					{
					    one=0;
	                    two=0;
					   

						if (ex > sx) one = 1;
						if (ey > sy) two = 1;
                        
						 add_color_point(ex, ey,red,green,blue);
						 
						 color =0xf +   (((spkl_shift & 1) << 3) | (spkl_shift & 4)
										| ((spkl_shift & 0x10) >> 3) | ((spkl_shift & 0x40) >> 6));
						 
						 if (vec_colors[color].r) red=z; else red=0;
					     if (vec_colors[color].g) green=z; else green=0;
					     if (vec_colors[color].b) blue=z; else blue=0;
                         
						 add_color_point(sx, sy,red,green,blue);
                       
						while ( ex != sx || ey != sy )
						{
						  color =0xf +   (((spkl_shift & 1) << 3) | (spkl_shift & 4)
										| ((spkl_shift & 0x10) >> 3) | ((spkl_shift & 0x40) >> 6));
                         if (vec_colors[color].r) red=z; else red=0;
					     if (vec_colors[color].g) green=z; else green=0;
					     if (vec_colors[color].b) blue=z; else blue=0;

						 if (sx!=ex){
							           if (one) {sx +=1;} else {sx -=1;}
						             }

                         if (sy!=ey){
							            if (two) {sy+=1;} else {sy -=1;}
						             }
						 
                         //if (escape > 40) log_it("SX  %f  EX %f SY %f EY %f",sx,ex,ey,sy);
                         add_color_point(sx, sy,red,green,blue);
						 //add_color_point(sx, sy,255,255,255);

                         spkl_shift = (((spkl_shift & 0x40) >> 6) ^ ((spkl_shift & 0x20) >> 5)
								        ^ 1 ) | (spkl_shift << 1);

				        if ((spkl_shift & 0x7f) == 0x7f) spkl_shift = 0;
						}

					}
					if (ywindow==1)
					{
						//Y-Window clipping
						if (sy < clip && ey < clip ){ draw = 0; } else { draw = 1;}
	                 	if (ey < clip && ey < sy ){	ex=((clip-ey)*((ex-sx)/(ey-sy)))+ex;ey=clip;}
						if (sy< clip && sy < ey ) { sx=((clip-sy)*((sx-ex)/(sy-ey)))+sx;sy=clip;}
					}
                  
					if (draw && sparkle==0) 
					{  
					 add_color_line(sx, sy,ex,ey,red,green,blue);
					 add_color_point(sx, sy,red,green,blue);
					 add_color_point(ex, ey,red,green,blue);
					}

}
*/
static void BZ_DRAW(int currentx, int currenty, int deltax, int deltay, int z, int color)
{
	float sy=0;
	float ey=0;
	float sx=0;
	float ex=0;
//	int gc =16; 
	int BZ_CLIP = 726;

	ey= ((currenty-deltay)>>VEC_SHIFT);   
	sy= currenty>>VEC_SHIFT;
	    sx= (currentx>>VEC_SHIFT);
	    ex= (currentx+deltax)>>VEC_SHIFT;
	    
	    if ( !testsw && gamenum!=REDBARON )
	    {
	    //Line color 0 clipping
	    if (sy> BZ_CLIP && sy > ey && color==0){sx=((BZ_CLIP-sy)*((ex-sx)/(ey-sy)))+sx;sy=BZ_CLIP;}
	    if (ey> BZ_CLIP && ey > sy && color==0){ex=((BZ_CLIP-ey)*((sx-ex)/(sy-ey)))+ex;ey=BZ_CLIP;}
	    }
//	log_it("X %d Y %d end X %d Y %d" ,sx,sy,ex,ey);			    
//	    gc=z+1;
//	    gc=z/16;
       add_line(sx, sy, ex, ey, z, z,z);
}

void AVG_RUN (void)
{
	int pc=0x0000;
	int sp;
	int stack [8];
//    int flipword = 0;
	int scale=0;
	int statz   = 0;
	int xflip   = 0;
  	int color   = 0;
	static int  sparkle =0;
	static int spkl_shift=0;
	int currentx=0, currenty=0;
	int done    = 0;
	int firstwd, secondwd=0;
	int opcode;
	int x, y, z=0, b, l,/* d,*/ a;
	int deltax, deltay=0;
	int red,green,blue;
	int COMPSHFT=13;
	static int lastbank=0;
	int ywindow=1;
//	int clip = 0;
	int oldscale=0;
	//PCTOP=0;
    pc=PCTOP;
	
	if (TYPE_QU) COMPSHFT=12;
	sp = 0;
	statz = 0;
	color = 0;
	scale = 0;          
	total_length=0;
    pc=PCTOP;
	
	while (!done)
	{
	    if (TYPE_QU) firstwd = memrdwdf (pc);
		else firstwd = memrdwd (pc);
		
		opcode = firstwd >> 13;
		pc++;pc++;	
	   //log_it("FirstWord is  %x",firstwd);
	   if (opcode == VCTR) //Get the second word if it's a draw command
		{
			if (TYPE_QU) secondwd = memrdwdf (pc);
			else secondwd = memrdwd (pc);
			pc++;pc++;
	    }
	
	   if ((opcode == STAT) && ((firstwd & 0x1000) != 0))opcode = SCAL;
	   switch (opcode)
		{           
			case VCTR: x=twos_comp_val(secondwd, COMPSHFT);
				       y=twos_comp_val(firstwd, COMPSHFT);
					   z = (secondwd >> 12) & ~0x01;
				
				   goto DRAWCODE;
				   break;

			case SVEC: x=twos_comp_val(firstwd, 5) << 1;
				       y=twos_comp_val(firstwd >> 8,5) << 1;
				       z = ((firstwd >> 4) & 0x0e);

DRAWCODE:			if (z == 2){z = statz;}
					if (z) {z = (z << 4) | 0x1f;}
				    
					total_length+=vector_timer(x * oldscale, y * oldscale); 
					
			        deltax = x * scale;
					deltay = y * scale;
					
			        if (xflip) deltax = -deltax;
					if (YFLIP) deltay = -deltay;
					if (XFLIP) deltax = -deltax;
					if (TYPE_TEMP) if (sparkle) {color = rand() & 0x07;}
			        
			 if (z > -1)
			 {     
                if (TYPE_BZ) 
                  BZ_DRAW(currentx,currenty, deltax, deltay, z, color);
				else 
                {

				 if (sparkle && TYPE_MH) { color =0xf +   (((spkl_shift & 1) << 3) | (spkl_shift & 4)
										| ((spkl_shift & 0x10) >> 3) | ((spkl_shift & 0x40) >> 6));}
			    //log_it("red %f green %f blue %f z %d",vec_colors[color].r,vec_colors[color].g,vec_colors[color].b,z);
				// glColor4ub(vec_colors[color].r,vec_colors[color].g,vec_colors[color].b,z);
				 if (vec_colors[color].r) red=z; else red=0;
				 if (vec_colors[color].g) green=z; else green=0;
				 if (vec_colors[color].b) blue=z; else blue=0;

				 if (TYPE_GV && testsw) { if ( color==0) {green=z; blue=z;}} //Gravitar Test Mode Fix
				
                 if (NEW_FRAME){

	        					if ((currentx==(currentx)+deltax) && (currenty==(currenty)-deltay))
									 {add_color_point((currentx>>VEC_SHIFT),currenty>>VEC_SHIFT, red,green,blue);}
					
								else{	 
									 add_color_line((currentx>>VEC_SHIFT), (currenty>>VEC_SHIFT), (currentx+deltax)>>VEC_SHIFT, (currenty-deltay)>>VEC_SHIFT, red,green,blue);
//									 add_color_point((currentx>>VEC_SHIFT),currenty>>VEC_SHIFT,  red,green,blue);
//									 add_color_point((currentx+deltax)>>VEC_SHIFT,(currenty-deltay)>>VEC_SHIFT,  red,green,blue);
									}
                              }
				}
			 }
				currentx += deltax;currenty -= deltay;
				
				break;

			case STAT: statz = (firstwd >> 4) & 0x000f;
				       color = (firstwd) & 0x000f;
					   if (TYPE_MH || TYPE_TEMP) {sparkle = !(firstwd & 0x0800);}
					   if (TYPE_MH)
				       {
					   xflip = firstwd & 0x0400;
					   vectorbank = 0x18000 + ((firstwd >> 8) & 3) * 0x2000;
						   if (lastbank!=vectorbank)
						   { lastbank=vectorbank;
							 log_it("Vector Bank Switch %x",0x18000 + ((firstwd >> 8) & 3) * 0x2000);
							 memcpy(GI[CPU0]+0x6000,GI[CPU0]+vectorbank,0x2000);
						   }
					   }
					   break;
			
			
			case SCAL:
				b = ((firstwd >> 8) & 0x07)+8;
				l = (~firstwd) & 0xff;
				scale = (l << VEC_SHIFT) >> b;		
			    oldscale=scale; //Double the scale for 1024x768 resolution
				scale*=SCALEADJ;
				if (TYPE_MH)
				{
				if (firstwd & 0x0800)
					{ if (ywindow == 0) {ywindow = 1;/*clip = currenty >> VEC_SHIFT;*/} else {ywindow = 0;}	}
				}
				break;

			case CNTR: /*d = firstwd & 0xff;*/
				       currentx = 512 << VEC_SHIFT;
					   currenty = 512 << VEC_SHIFT;
					   break; 
			case RTSL:

				if (sp == 0)
				{	log_it("AVG Stack Underflow, <quantum error.>");
					done = 1;sp = MAXSTACK - 1;}
				else {sp--; pc = stack [sp];	}
				break;

			case HALT: done = 1;break;
			case JMPL: a=PCTOP+((firstwd & 0x1fff)<<1);if (a == PCTOP){done=1;}else{pc=a;}break;
			case JSRL: a=PCTOP+((firstwd & 0x1fff)<<1) ;
				       if (a == PCTOP){done = 1;}
					   else {stack [sp] = pc;
					
					   if (sp == (MAXSTACK - 1))
					   {log_it ("AVG Stack Overflow <quantum error>");done = 1;sp = 0;}
					   else{sp++;pc=a;}
			            }break;
				

			default: log_it("Some sort of Error in AVG engine, default reached.");
			}	
		}

		
}

int avg_go()
{
	if (AVG_BUSY) {return 1;}
	else { 
 	AVG_RUN();
	if (total_length > 1 ) {AVG_BUSY=1;get_video_ticks(0xff);calc_sweep();} else {AVG_BUSY=0;}
	}

return 0;
}


int avg_clear()
{
	AVG_BUSY=0;
    return 0;
}


int avg_check()
{
	//log_it("Total LENGTH HERE %f and TOTAL TICKS %d",me,ticks );
	if ((get_video_ticks(0) > sweep) && AVG_BUSY ) {avg_clear();}
 
	return AVG_BUSY;
}

void avg_init()
{
	TYPE_BZ=0;
	TYPE_TEMP=0;
	TYPE_GV=0;
	TYPE_SD=0;
	TYPE_MH=0;
	TYPE_QU=0;
	TYPE_SD=0;
	TYPE_SW=0;
	AVG_BUSY=0;
	XFLIP=0;
	YFLIP=0;

	if (gamenum == BZONE  || 
		gamenum == BZONE2 ||
		gamenum == BZONEP ||
		gamenum == BZONEC ||
		gamenum == BRADLEY ||
		gamenum == REDBARON) {
			                   TYPE_BZ=1;
							   pc=0x2000;
							   set_bw_colors();
							   PCTOP=0x2000;
							   YFLIP=1;
							   AVG_BUSY=1;
	                          }
      
	else if (gamenum == SPACDUEL ||
		gamenum == BWIDOW ||
		gamenum == LUNARBAT ||
		gamenum == LUNARBA1) {YFLIP=1;TYPE_SD=1;PCTOP=0x2000;set_sd_colors();}
	else if (gamenum == GRAVITAR ||
		 gamenum == GRAVITR2 ||
		 gamenum == GRAVP)  {YFLIP=1;TYPE_GV=1;PCTOP=0x2000;set_sd_colors();}
	else if (gamenum == TEMPEST ||
		gamenum == TEMPESTM ||
		gamenum == TEMPEST1 ||
		gamenum == TEMPEST2 ||
		gamenum == TEMPEST3 ||
		gamenum == TEMPTUBE ||
		gamenum == VBREAK ||
		gamenum == VORTEX ||
		gamenum == ALIENST) {YFLIP=1;TYPE_TEMP=1;PCTOP=0x2000;SCALEADJ=1;}
	else if (gamenum == MHAVOC ||
		gamenum == MHAVOC2 ||
		gamenum == MHAVOCRV ||
		gamenum == MHAVOCP ||
		gamenum == ALPHAONA ||
		gamenum == ALPHAONE) {TYPE_MH=1;PCTOP=0x4000;}

	else if (gamenum == QUANTUM ||
		gamenum == QUANTUM1 ||
		gamenum == QUANTUMP) {YFLIP=1;TYPE_QU=1;PCTOP=0;SCALEADJ=1;}
	else if (gamenum == STARWARS ||
		gamenum == QUANTUM1 ||
		gamenum == STARWAR1) {TYPE_SW=1;PCTOP=0;}
}
