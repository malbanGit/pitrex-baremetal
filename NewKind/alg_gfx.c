/**
 *
 * Elite - The New Kind.
 *
 * Allegro version of Graphics routines.
 *
 * The code in this file has not been derived from the original Elite code.
 * Written by C.J.Pinder 1999-2001.
 * email: <christian@newkind.co.uk>
 *
 * Routines for drawing anti-aliased lines and circles by T.Harte.
 *
 **/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#include <vectrex/vectrexInterface.h>
#include "allegro_dummy.h"

#include "config.h"
#include "gfx.h"
#include "alg_data.h"
#include "elite.h"

volatile int frame_count;
//DATAFILE *datafile;

#define MAX_POLYS	100

static int start_poly;
static int total_polys;

struct poly_data
{
	int z;
	int no_points;
	int face_colour;
	int point_list[16];
	int next;
};

static struct poly_data poly_chain[MAX_POLYS];






void gfx_graphics_shutdown (void)
{
}


/*
 * Blit the back buffer to the screen.
 */

void gfx_update_screen (void)
{
    v_readButtons();
    v_readJoystick1Analog();
    v_doSound();
    v_WaitRecal();
    v_setBrightness(64);        /* set intensity of vector beam... */

}


void gfx_acquire_screen (void)
{
}


void gfx_release_screen (void)
{
}

void gfx_fast_plot_pixel (int x, int y, int col)
{
//	_putpixel(gfx_screen, x, y, col);
//	gfx_screen->line[y][x] = col;
 gfx_plot_pixel (x, y, col);
}

void putpixel(void* bmp, int x, int y, int color)
{
  gfx_draw_line ( x, y, x, y);
}

void gfx_plot_pixel (int x, int y, int col)
{

  putpixel (0, x, y , col);
//  printf("Plot alert!\n\r");
}

void gfx_plot_pixel_b (int x, int y, int b)
{
  x+=GFX_X_OFFSET;
  y+=GFX_Y_OFFSET;
  y = -y;
  x = x << 7;
  y = y << 7;
  v_directDraw32(x*0.47,y*0.47,x*0.47,y*0.47, b);  
}



#define AA_BITS 3
#define AA_AND  7
#define AA_BASE 235

#define trunc(x) ((x) & ~65535)
#define frac(x) ((x) & 65535)
#define invfrac(x) (65535-frac(x))
#define plot(x,y,c) putpixel(gfx_screen, (x), (y), (c)+AA_BASE)

void gfx_draw_circle (int cx, int cy, int radius, int circle_colour)
{
//    circle (gfx_screen, cx + GFX_X_OFFSET, cy + GFX_Y_OFFSET, radius, circle_colour);
  

 int segments = radius/10;
 if (segments <6) segments = 6;
 gfx_draw_circle_fixed (cx, cy, radius, circle_colour, segments);

}

void gfx_draw_circle_fixed_ (int cx, int cy, int radius, int circle_colour, int segments)
{
 float angle = 0;
 float increase = 2*M_PI / segments;
 
 int x0 = radius * cos(angle);
 int y0 = radius * sin(angle);
 
 
 
 for (int i=0;i<segments-1;i++)
 {
   angle += increase;
   int x1 = radius * cos(angle);
   int y1 = radius * sin(angle);
   gfx_draw_line (cx+x0, cy+y0, cx+x1, cy+y1);
   x0 = x1;
   y0 = y1;
 }

 gfx_draw_line (cx+x0, cy+y0, cx+radius * cos(angle), cy+radius * sin(angle));
}
void gfx_draw_circle_fixed (int cx, int cy, int radius, int circle_colour, int segments)
{
 float angle = 0;
 float increase = 2*M_PI / segments;
 
 int x0 = (  ((int)  (cx + radius * cos(angle)+GFX_X_OFFSET)) << 7 ) *0.47;
 int y0 = (  ((int)(-(cy + radius * sin(angle)+GFX_Y_OFFSET))) << 7 ) *0.47; 
 int x1 = x0;
 int y1 = y0;
 
 
 
 for (int i=0;i<segments-1;i++)
 {
   angle += increase;
   int x2 =   (  ((int)  (cx + radius * cos(angle)+GFX_X_OFFSET)) << 7 ) *0.47;
   int y2 =   (  ((int)(-(cy + radius * sin(angle)+GFX_Y_OFFSET))) << 7 ) *0.47;
   
   v_directDraw32 (x1, y1, x2, y2, circle_colour);
   x1 = x2;
   y1 = y2;
 }
 v_directDraw32 (x1, y1, x0, y0, circle_colour);
}










void gfx_draw_filled_circle (int cx, int cy, int radius, int circle_colour)
{
  gfx_draw_circle(cx, cy, radius, circle_colour);
}

void gfx_draw_filled_circle_fixed (int cx, int cy, int radius, int circle_colour, int segments)
{
  gfx_draw_circle_fixed(cx, cy, radius, circle_colour, segments);
}


#undef trunc
#undef frac
#undef invfrac
#undef plot

#undef AA_BITS
#undef AA_AND
#undef AA_BASE






void gfx_draw_line (int x1, int y1, int x2, int y2)
{
//printf("Pre %i,%i,%i,%i\n\r", x1,y1,x2,y2);  
  x1+=GFX_X_OFFSET;
  x2+=GFX_X_OFFSET;
  y1+=GFX_Y_OFFSET;
  y2+=GFX_Y_OFFSET;

  y1 = -y1;
  y2 = -y2;

//  if (x1>=256) x1 = 255;
//  if (y1>=256) y1 = 255;
//  if (x2>=256) x2 = 255;
//  if (y2>=256) y2 = 255;
  
  x1 = x1 << 7;
  x2 = x2 << 7;
  y1 = y1 << 7;
  y2 = y2 << 7;
  
//printf("Post %i,%i,%i,%i\n\r", x1,y1,x2,y2);  
  
  v_directDraw32(x1*0.47,y1*0.47,x2*0.47,y2*0.47, 0x40);
}






void polygon(int vertices, int *points, int color)
{
  // basically DrawVLc with absolut coordinates

  while (vertices >0)  
  {
    int x0 = *points++;
    int y0 = *points++;
    int x1 = *points++;
    int y1 = *points++;
    gfx_draw_line(x0,y0,x1,y1);
    vertices --;
    vertices --;
  }
  
}

void gfx_draw_colour_line (int x1, int y1, int x2, int y2, int line_colour)
{
  gfx_draw_line(x1,y1,x2,y2);
}



void gfx_draw_triangle (int x1, int y1, int x2, int y2, int x3, int y3, int col)
{
//	triangle (gfx_screen, x1 + GFX_X_OFFSET, y1 + GFX_Y_OFFSET, x2 + GFX_X_OFFSET, y2 + GFX_Y_OFFSET,  x3 + GFX_X_OFFSET, y3 + GFX_Y_OFFSET, col);

    gfx_draw_line(x1,y1,x2,y2);
    gfx_draw_line(x2,y2,x3,y3);
    gfx_draw_line(x3,y3,x1,y1);
}


static char *_toUpper(char *t)
{
  static char _ubuf[256];
  char *p = _ubuf;
  do
  {
    *p++ = toupper(*t++);
  } while (*t != 0);
  *p = 0;
  return _ubuf;
}

void printString( int x, int y , char *strbuf)
{
  y+=16; // this is better in relation to 16bit positioning
  
  y = -y;
  y = y + GFX_Y_OFFSET;
  y = y / 2; // value from -128 - +128

  x = x + GFX_X_OFFSET;
  x = x / 2; // value from -128 - +128
  
//  if (strlen(strbuf)>20)
    v_printStringRasterFont( x, y, _toUpper(strbuf), 30, -4, 0, alternate_raster5);
//  else
//    v_printStringBiRasterFont( x, y, _toUpper(strbuf), 30, -4, 0, alternate_raster5,1);
  
/*
  y = -y;
  if (x>=256) x = 255;
  if (y>=256) y = 255;
*/
//  v_printStringRasterFont( x, y, strbuf, 30, -4, 0, alternate_raster5);
//  if (strlen(strbuf)>20)
//    v_printStringRasterFont( x, y, _toUpper(strbuf), 30, -4, 0, alternate_raster5);
//  else
//    v_printStringBiRasterFont( x, y, _toUpper(strbuf), 30, -4, 0, alternate_raster5,1);
  
  
//v_printString(x, y, _toUpper(strbuf), 3, 0x2f);
  
  // 27400
    // 28 - 31 000 raster non bi
    // 29 - 32 000 vector
    // 25 - 28 000 bi
}

void printString_bi( int x, int y , char *strbuf)
{
  y+=16; // this is better in relation to 16bit positioning
  
  y = -y;
  y = y + GFX_Y_OFFSET;
  y = y / 2; // value from -128 - +128

  x = x + GFX_X_OFFSET;
  x = x / 2; // value from -128 - +128
  
  v_printStringBiRasterFont( x, y, _toUpper(strbuf), 30, -4, 0, alternate_raster5,1);
}

void printString_b( int x, int y , char *strbuf)
{
  y+=16; // this is better in relation to 16bit positioning
  y = -y;
  y = y + GFX_Y_OFFSET;
  y = y / 2; // value from -128 - +128

  x = x + GFX_X_OFFSET;
  x = x / 2; // value from -128 - +128
  v_printStringRasterFont( x, y, _toUpper(strbuf), 30, -4, 0, 0);
}
void gfx_display_centre_text_b (int y, char *str, int psize, int col)
{
 int  x=256;
 int len =  strlen(str);
 x-=5*len;

 v_setBrightness(col);        /* set intensity of vector beam... */
 printString_b(x,y, str);
}


void gfx_display_text (int x, int y, char *txt)
{
    printString( x,y, txt);
}
void gfx_display_text_b (int x, int y, char *txt)
{
    printString_b( x,y, txt);
}


void gfx_display_colour_text (int x, int y, char *txt, int col)
{
    printString( x,y, txt);
}


void gfx_display_centre_text (int y, char *str, int psize, int col)
{
 int  x=256;
 int len =  strlen(str);
 x-=5*len;

 printString( x,y, str);
}


void gfx_clear_display (void)
{
}

void gfx_clear_text_area (void)
{
}


void gfx_clear_area (int tx, int ty, int bx, int by)
{
}

void gfx_draw_rectangle (int x1, int y1, int x2, int y2, int col)
{
  gfx_draw_line (x1, y1, x1, y2);
  gfx_draw_line (x1, y2, x2, y2);
  gfx_draw_line (x2, y2, x2, y1);
  gfx_draw_line (x2, y1, x1, y1);
}

// planet description e.g.
//    description = describe_planet (hyperspace_planet);
//    gfx_display_pretty_text (16, 298, 400, 384, description);

void gfx_display_pretty_text_where (int tx, int ty, int bx, int by, char *txt, int toList)
{
	char strbuf[100];
	char *str;
	char *bptr;
	int len;
	int pos;
	int maxlen;
	
	maxlen = (bx - tx) / 8;

	str = txt;	
	len = strlen(txt);
	
	while (len > 0)
	{
		pos = maxlen;
		if (pos > len)
			pos = len;

		while ((str[pos] != ' ') && (str[pos] != ',') &&
			   (str[pos] != '.') && (str[pos] != '\0'))
		{
			pos--;
		}

		len = len - pos - 1;
	
		for (bptr = strbuf; pos >= 0; pos--)
			*bptr++ = *str++;

		*bptr = '\0';
        if (toList)
          itemListAdd(strbuf, 0x5f);
        else
          printString(tx , ty , strbuf);
//		textout (gfx_screen, datafile[ELITE_1].dat, strbuf, tx + GFX_X_OFFSET, ty + GFX_Y_OFFSET, GFX_COL_WHITE);
		ty += 32;
	}
}
void gfx_display_pretty_text_toList (int tx, int ty, int bx, int by, char *txt)
{
 gfx_display_pretty_text_where (tx, ty, bx, by, txt, 1);
}
void gfx_display_pretty_text (int tx, int ty, int bx, int by, char *txt)
{
 gfx_display_pretty_text_where (tx, ty, bx, by, txt, 0);
}


void gfx_draw_scanner (void)
{
//	blit (scanner_image, gfx_screen, 0, 0, GFX_X_OFFSET, 385+GFX_Y_OFFSET, scanner_image->w, scanner_image->h);
}

void gfx_set_clip_region (int tx, int ty, int bx, int by)
{
//	set_clip (gfx_screen, tx + GFX_X_OFFSET, ty + GFX_Y_OFFSET, bx + GFX_X_OFFSET, by + GFX_Y_OFFSET);
}


void gfx_start_render (void)
{
	start_poly = 0;
	total_polys = 0;
}


void gfx_render_polygon (int num_points, int *point_list, int face_colour, int zavg)
{
	int i;
	int x;
	int nx;
	
	if (total_polys == MAX_POLYS)
		return;

	x = total_polys;
	total_polys++;
	
	poly_chain[x].no_points = num_points;
	poly_chain[x].face_colour = face_colour;
	poly_chain[x].z = zavg;
	poly_chain[x].next = -1;

	for (i = 0; i < 16; i++)
		poly_chain[x].point_list[i] = point_list[i];				

	if (x == 0)
		return;

	if (zavg > poly_chain[start_poly].z)
	{
		poly_chain[x].next = start_poly;
		start_poly = x;
		return;
	} 	

	for (i = start_poly; poly_chain[i].next != -1; i = poly_chain[i].next)
	{
		nx = poly_chain[i].next;
		
		if (zavg > poly_chain[nx].z)
		{
			poly_chain[i].next = x;
			poly_chain[x].next = nx;
			return;
		}
		
	}	
	
	poly_chain[i].next = x;
}


void gfx_render_line (int x1, int y1, int x2, int y2, int dist, int col)
{
	int point_list[4];
	
	point_list[0] = x1;
	point_list[1] = y1;
	point_list[2] = x2;
	point_list[3] = y2;
	
	gfx_render_polygon (2, point_list, col, dist);
}

void gfx_finish_render (void)
{
	int num_points;
	int *pl;
	int i;
	int col;
	
	if (total_polys == 0)
		return;
		
	for (i = start_poly; i != -1; i = poly_chain[i].next)
	{
		num_points = poly_chain[i].no_points;
		pl = poly_chain[i].point_list;
		col = poly_chain[i].face_colour;

        polygon (num_points, pl, col);
	};
}



void gfx_draw_sprite (int sprite_no, int x, int y)
{
  /*
	BITMAP *sprite_bmp;
	
	switch (sprite_no)
	{
		case IMG_GREEN_DOT:
			sprite_bmp = datafile[GRNDOT].dat;
			break;
		
		case IMG_RED_DOT:
			sprite_bmp = datafile[REDDOT].dat;
			break;
			
		case IMG_BIG_S:
			sprite_bmp = datafile[SAFE].dat;
			break;
		
		case IMG_ELITE_TXT:
			sprite_bmp = datafile[ELITETXT].dat;
			break;
			
		case IMG_BIG_E:
			sprite_bmp = datafile[ECM].dat;
			break;
			
		case IMG_BLAKE:
			sprite_bmp = datafile[BLAKE].dat;
			break;
		
		case IMG_MISSILE_GREEN:
			sprite_bmp = datafile[MISSILE_G].dat;
			break;

		case IMG_MISSILE_YELLOW:
			sprite_bmp = datafile[MISSILE_Y].dat;
			break;

		case IMG_MISSILE_RED:
			sprite_bmp = datafile[MISSILE_R].dat;
			break;

		default:
			return;
	}

	if (x == -1)
		x = ((256 * GFX_SCALE) - sprite_bmp->w) / 2;
	
	draw_sprite (gfx_screen, sprite_bmp, x + GFX_X_OFFSET, y + GFX_Y_OFFSET);
    */
}


int gfx_request_file (char *title, char *path, char *ext)
{
/*
  int okay;

	show_mouse (screen);
	okay = file_select (title, path, ext);
	show_mouse (NULL);

	return okay;
*/    
 return 0;
}

