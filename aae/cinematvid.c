/***************************************************************************

    Cinematronics vector hardware

***************************************************************************/

#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif
#include "globals.h"

#include "vector.h"
#include "ccpu.h"
#include "cinemat.h"

#define SWAPV(x,y) {int t;t=x;x=y;y=t;}
/* an rgb_t is a single combined R,G,B (and optionally alpha) value */
typedef UINT32 rgb_t;

/* an rgb15_t is a single combined 15-bit R,G,B value */
typedef UINT16 rgb15_t;

/***************************************************************************
    MACROS
***************************************************************************/

/* macros to assemble rgb_t values */
#define MAKE_RGB(r,g,b) 	((((rgb_t)(r) & 0xff) << 16) | (((rgb_t)(g) & 0xff) << 8) | ((rgb_t)(b) & 0xff))
#define MAKE_ARGB(a,r,g,b)	(MAKE_RGB(r,g,b) | (((rgb_t)(a) & 0xff) << 24))

/* macros to extract components from rgb_t values */
#define RGB_ALPHA(rgb)		(((rgb) >> 24) & 0xff)
#define RGB_RED(rgb)		(((rgb) >> 16) & 0xff)
#define RGB_GREEN(rgb)		(((rgb) >> 8) & 0xff)
#define RGB_BLUE(rgb)		((rgb) & 0xff)

/* common colors */
#define RGB_BLACK			(MAKE_RGB(0,0,0))
#define RGB_WHITE			(MAKE_RGB(255,255,255))


//#define MAKE_RGB(r,g,b) 	((((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))


/*************************************
 *
 *  Local variables
 *
 *************************************/
static rgb_t vector_color=0;
static int color_mode;
static INT16 lastx, lasty;
static UINT8 last_control;



/*************************************
 *
 *  Vector rendering
 *
 *************************************/
extern int gamenum;
void cinemat_vector_callback(INT16 sx, INT16 sy, INT16 ex, INT16 ey, UINT8 shift)
{
	/*

	// adjust for slop
	sx = sx - visarea->min_x;
	ex = ex - visarea->min_x;
	sy = sy - visarea->min_y;
	ey = ey - visarea->min_y;
   */

	 int r,g,b;
	 int intensity = 0;
  //if (fromx < MinX) MinX = fromx;
  //if (tox < MinX) MinX = tox;
  //if (fromy < MinY) MinY = fromy;
  //if (toy < MinY) MinY = toy;
  //if (fromx > MaxX) MaxX = fromx;
  //if (tox > MaxX) MaxX = tox;
  //if (fromy > MaxY) MaxY = fromy;
  //if (toy > MaxY) MaxY = toy;

	//temp_word = toy;
    //toy = tox;
    //tox = temp_word;

    //temp_word = fromy;
    //fromy = fromx;
    //fromx = temp_word;
 // log_it("SX %x SY %x EX %x EY %x",sx,sy,ex,ey);


	 if (bSwapXY) 
	 {
		 SWAPV(sx,sy);
	     SWAPV(ex,ey);
	 }

  r=RGB_RED(vector_color);
  g=RGB_GREEN(vector_color);
  b=RGB_BLUE(vector_color);
  
              if (sx == ex && sy == ey) {
				                         intensity = 0x1ff * shift / 8; 
//			                             cache_txt(sx,sy,4, 0xff ); //Keep constant intensity for now
//                    add_color_point(sx, sy, r,g,b);
			                            }  //fake intensity
//                    add_color_line(sx, sy, ex, ey, r,g,b);
//                    add_color_point(sx, sy, r,g,b);
//                    add_color_point(ex, ey, r,g,b);

   int x0;
   int y0;
   int x1;
   int y1;
   int zz = (r+g+b)/3/2;
   switch (gamenum)
   {
     case ARMORA:
     case WARRIOR:
     case STARCAS:
     case TAILG:
     case SPEEDFRK:
     case BOXINGB:
     case STARHAWK:
     case SPACEWAR:
     case WOTW:
    {
      x0 = (sx<<5)-16000;
      y0 = (sy<<5)-16000;
      x1 = (ex<<5)-16000;
      y1 = (ey<<5)-16000;
      zz = (r+g+b)/3/2;
      break;
      }
     default: // SOLARQ BARRIER // sundance
    {
      x0 = -(sx<<5)+16000;
      y0 = (sy<<5)-16000;
      x1 = -(ex<<5)+16000;
      y1 = (ey<<5)-16000;
      zz = (r+g+b)/3/2;
      break;
      }
   }
                    

   v_directDraw32(x0, y0, x1,y1,zz);
                    
                    
  lastx = ex;
  lasty = ey;
}



/*************************************
 *
 *  Vector color handling
 *
 *************************************/

void vec_control_write (int data)
{
	int r, g, b, i;

	//log_it("Video control write");

	switch (color_mode)
	{
		case COLOR_BILEVEL:
			/* color is either bright or dim, selected by the value sent to the port */
			vector_color = (data & 1) ? MAKE_RGB(0x80,0x80,0x80) : MAKE_RGB(0xff,0xff,0xff);
			break;

		case COLOR_16LEVEL:
			/* on the rising edge of the data value, latch bits 0-3 of the */
			/* X register as the intensity */
			if (data != last_control && data)
			{
				int xval = cpunum_get_reg(0, CCPU_X) & 0x0f;
				i = (xval + 1) * 255 / 16;
				vector_color = MAKE_RGB(i,i,i);
			}
			break;

		case COLOR_64LEVEL:
			/* on the rising edge of the data value, latch bits 2-7 of the */
			/* X register as the intensity */
			if (data != last_control && data)
			{
				int xval = cpunum_get_reg(0, CCPU_X);
				xval = (~xval >> 2) & 0x3f;
				i = (xval + 1) * 255 / 64;
				vector_color = MAKE_RGB(i,i,i);
			}
			break;

		case COLOR_RGB:
			/* on the rising edge of the data value, latch the X register */
			/* as 4-4-4 BGR values */
			if (data != last_control && data)
			{
				int xval = cpunum_get_reg(0, CCPU_X);
				r = (~xval >> 0) & 0x0f;
				r = r * 255 / 15;
				g = (~xval >> 4) & 0x0f;
				g = g * 255 / 15;
				b = (~xval >> 8) & 0x0f;
				b = b * 255 / 15;
				vector_color = MAKE_RGB(r,g,b);
			}
			break;

		case COLOR_QB3:
			{
				static int lastx, lasty;

				/* on the falling edge of the data value, remember the original X,Y values */
				/* they will be restored on the rising edge; this is to simulate the fact */
				/* that the Rockola color hardware did not overwrite the beam X,Y position */
				/* on an IV instruction if data == 0 here */
				if (data != last_control && !data)
				{
					lastx = cpunum_get_reg(0, CCPU_X);
					lasty = cpunum_get_reg(0, CCPU_Y);
				}

				/* on the rising edge of the data value, latch the Y register */
				/* as 2-3-3 BGR values */
				if (data != last_control && data)
				{
					int yval = cpunum_get_reg(0, CCPU_Y);
					r = (~yval >> 0) & 0x07;
					r = r * 255 / 7;
					g = (~yval >> 3) & 0x07;
					g = g * 255 / 7;
					b = (~yval >> 6) & 0x03;
					b = b * 255 / 3;
					vector_color = MAKE_RGB(r,g,b);

					/* restore the original X,Y values */
					cpunum_set_reg(0, CCPU_X, lastx);
					cpunum_set_reg(0, CCPU_Y, lasty);
				}
			}
			break;
	}

	/* remember the last value */
	last_control = data;
}



/*************************************
 *
 *  Video startup
 *
 *************************************/

void video_type_set(int type)
{
  color_mode = type; 
}

/*
VIDEO_START( cinemat_bilevel )
{
	color_mode = COLOR_BILEVEL;
	VIDEO_START_CALL(vector);
}


VIDEO_START( cinemat_16level )
{
	color_mode = COLOR_16LEVEL;
	VIDEO_START_CALL(vector);
}


VIDEO_START( cinemat_64level )
{
	color_mode = COLOR_64LEVEL;
	VIDEO_START_CALL(vector);
}


VIDEO_START( cinemat_color )
{
	color_mode = COLOR_RGB;
	VIDEO_START_CALL(vector);
}


VIDEO_START( cinemat_qb3color )
{
	color_mode = COLOR_QB3;
	VIDEO_START_CALL(vector);
}
*/


/*************************************
 *
 *  End-of-frame
 *
 *************************************/

int cinevid_up(  )
{
	//VIDEO_UPDATE_CALL(vector);
	//vector_clear_list();

	//cpuintrf_push_context(0);
	ccpu_wdt_timer_trigger();
	//cpuintrf_pop_context();

	return 0;
}



/*************************************
 *
 *  Space War update
 *
 ************************************

VIDEO_UPDATE( spacewar )
{
	int sw_option = readinputportbytag("INPUTS");

	VIDEO_UPDATE_CALL(cinemat);

	// set the state of the artwork 
	output_set_value("pressed3", (~sw_option >> 0) & 1);
	output_set_value("pressed8", (~sw_option >> 1) & 1);
	output_set_value("pressed4", (~sw_option >> 2) & 1);
	output_set_value("pressed9", (~sw_option >> 3) & 1);
	output_set_value("pressed1", (~sw_option >> 4) & 1);
	output_set_value("pressed6", (~sw_option >> 5) & 1);
	output_set_value("pressed2", (~sw_option >> 6) & 1);
	output_set_value("pressed7", (~sw_option >> 7) & 1);
	output_set_value("pressed5", (~sw_option >> 10) & 1);
	output_set_value("pressed0", (~sw_option >> 11) & 1);
	return 0;
}
*/
