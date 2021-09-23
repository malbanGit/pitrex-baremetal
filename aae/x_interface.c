/*
 * x_interface.c: X window system display for Atari Vector game simulator
 *
 * Copyright 1991, 1992, 1993, 1996, 2003 Eric Smith and Hedley Rainnie
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: x_interface.c 2 2003-08-20 01:51:05Z eric $
 */

#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

/*
#include "display.h"
#include "memory.h"   / * for switches[], start1, and start2 * /
#include "sim6502.h"  / * for stepflag and traceflag * /
#include "game.h"
*/


int blank_display = 1; /* flag to avoid erasing & drawing if no vectors */


Display *mydisplay;
Window mywindow;
Drawable mypixmap;
GC mygc;

/* pixel values */
unsigned long black, white;
unsigned long color_value [8][16];
unsigned long last_pixel;

int smallwindow;
int use_pixmap;
int line_width;

int window_width, window_height;

#define MAX_KEY 256
int *keymap [MAX_KEY];
int escape_code;


void setup_keyboard (void)
{
/*
  int i;

  for (i = 0; i < MAX_KEY; i++)
    keymap [i] = NULL;
  keymap [XKeysymToKeycode (mydisplay, XK_1)] = & start1;
  keymap [XKeysymToKeycode (mydisplay, XK_2)] = & start2;

  keymap [XKeysymToKeycode (mydisplay, XK_0)] = & self_test;

  if (game == BATTLEZONE)
    {
      keymap [XKeysymToKeycode (mydisplay, XK_d)] = & switches [0].leftfwd;
      keymap [XKeysymToKeycode (mydisplay, XK_c)] = & switches [0].leftrev;
      keymap [XKeysymToKeycode (mydisplay, XK_k)] = & switches [0].rightfwd;
      keymap [XKeysymToKeycode (mydisplay, XK_m)] = & switches [0].rightrev;
      keymap [XKeysymToKeycode (mydisplay, XK_space)] = & switches [0].fire;
    }
  else
    {
#if 1
  keymap [XKeysymToKeycode (mydisplay, XK_d)] = & switches [0].left;
  keymap [XKeysymToKeycode (mydisplay, XK_f)] = & switches [0].right;
  keymap [XKeysymToKeycode (mydisplay, XK_k)] = & switches [0].fire;
  keymap [XKeysymToKeycode (mydisplay, XK_j)] = & switches [0].thrust;
  keymap [XKeysymToKeycode (mydisplay, XK_h)] = & switches [0].hyper;
  keymap [XKeysymToKeycode (mydisplay, XK_space)] = & switches [0].shield;
  keymap [XKeysymToKeycode (mydisplay, XK_q)] = & switches [0].abort;
#else
  keymap [XKeysymToKeycode (mydisplay, XK_l)] = & switches [0].left;
  keymap [XKeysymToKeycode (mydisplay, XK_r)] = & switches [0].right;
  keymap [XKeysymToKeycode (mydisplay, XK_f)] = & switches [0].fire;
  keymap [XKeysymToKeycode (mydisplay, XK_t)] = & switches [0].thrust;
  keymap [XKeysymToKeycode (mydisplay, XK_h)] = & switches [0].hyper;
  keymap [XKeysymToKeycode (mydisplay, XK_s)] = & switches [0].shield;
  keymap [XKeysymToKeycode (mydisplay, XK_a)] = & switches [0].abort;
#endif
    }

  escape_code = XKeysymToKeycode (mydisplay, XK_Escape);
  */
}


void setup_colors (void)
{
  int color;
  int z;
  Colormap cmap;
  XColor xcolor;
  unsigned long plane_masks [1];
  unsigned long pixels [105];
  int try, i;

  cmap = DefaultColormap (mydisplay, DefaultScreen (mydisplay));

  for (try = 15; try >= 1; try--)
  { int err;
      if (0 != (err=XAllocColorCells (mydisplay, cmap,
				 0, /* contig */
				 plane_masks,
				 0, /* nplanes */
				 pixels,
				      try * 7)))
	break;
      printf("try %d err %d\n", try, err);
    }

  printf ("Allocated %d color cells\n", try * 7);

  if (try != 0)
    {
      i = try * 7 - 1;
      for (z = 15; z >= 0; z--)
	for (color = 0; color < 8; color++)
	  if ((color == 0) || (z == 0))
	    color_value [color] [z] = black;
	  else
	    {
	      if (i >= 0)
		{
		  xcolor.red   = (color & 4) ? (0x8000 + (0x0888 * z)) : 0;
		  xcolor.green = (color & 2) ? (0x8000 + (0x0888 * z)) : 0;
		  xcolor.blue  = (color & 1) ? (0x8000 + (0x0888 * z)) : 0;
		  xcolor.pixel = pixels [i];
		  xcolor.flags = DoRed | DoGreen | DoBlue;
		  i--;
		  XStoreColor (mydisplay, cmap, & xcolor);
		  color_value [color] [z] = xcolor.pixel;
		}
	      else
		color_value [color] [z] = color_value [color] [z+1];
	    }
    }
  else
    {
      for (z = 15; z >= 0; z--)
	for (color = 0; color < 8; color++)
	  if ((color == 0) || (z == 0))
	    color_value [color] [z] = black;
	  else
	    color_value [color] [z] = white;
    }
}


/* create a pixmap to double-buffer the window */
Drawable create_pixmap (Display *display, Drawable drawable)
{
  Pixmap apixmap;
  Window root;
  unsigned int width, height, depth, border_width;
  unsigned int act_width, act_height, act_depth;
  int x, y;

  /* get the width, height, and depth of the window */
  if (0 == XGetGeometry (display, drawable, & root, & x, & y,
			 & width, & height, & border_width,
			 & depth))
    {
      fprintf (stderr, "XGeometry() failed for window\n");
      goto fail;
    }

  printf ("depth: %d\n", depth);

  /* create a pixmap */
  apixmap = XCreatePixmap (display, drawable, width, height, depth);

  /* find out if we are successful */
  if (0 == XGetGeometry (display, apixmap, & root, & x, & y,
			 & act_width, & act_height, & border_width,
			 & act_depth))
    {
      fprintf (stderr, "XGeometry() failed for pixmap\n");
      goto fail;
    }

  if ((act_width == width) && (act_height == height) && (act_depth == depth))
    return (apixmap);

  fprintf (stderr, "pixmap error, requested width %d height %d depth %d\n",
	   width, height, depth);
  fprintf (stderr, "                    got width %d height %d depth %d\n",
	   act_width, act_height, act_depth);

  /* if unsuccessful, draw directly into the window */
 fail:
  return (drawable);
}

int portrait = 1;
void init_graphics (int argc, char *argv[], int p_smallwindow, 
		    int p_use_pixmap, int p_line_width, char *window_name)
{
  XSizeHints myhint;
  int myscreen;

  smallwindow = p_smallwindow;
  use_pixmap = p_use_pixmap;
  line_width = p_line_width;

  mydisplay = XOpenDisplay ("");
  if (!mydisplay)
    {
      fprintf (stderr, "Can't init X\n");
      exit (1);
    }
  myscreen = DefaultScreen (mydisplay);
  white = WhitePixel (mydisplay, myscreen);
  black = BlackPixel (mydisplay, myscreen);

  if (portrait)
    {
      window_height = 1024 >> smallwindow;
      window_width  = 768 >> smallwindow;
    }
  else
    {
      window_width = 1024 >> smallwindow;
      window_height = 768 >> smallwindow;
    }

  myhint.x = 50; myhint.y = 50;
  myhint.width = window_width;
  myhint.height = window_height;
  myhint.flags = PPosition | PSize;

  mywindow = XCreateSimpleWindow
    (mydisplay,
     DefaultRootWindow (mydisplay),
     myhint.x, myhint.y, myhint.width, myhint.height,
     5, 
     white, black);

  XSetStandardProperties (mydisplay, mywindow, window_name, window_name,
			  None, argv, argc, &myhint);

  mygc = XCreateGC (mydisplay, mywindow, 0, 0);
  XSetBackground (mydisplay, mygc, black);
  XSetForeground (mydisplay, mygc, white);

  XSetLineAttributes (mydisplay, mygc, line_width, LineSolid, CapButt, JoinMiter);
/*
  if (uses_analog_joystick (game))
    XSelectInput (mydisplay, mywindow,
		  KeyPressMask | KeyReleaseMask | PointerMotionMask);
  else
*/ 
    XSelectInput (mydisplay, mywindow,
		  KeyPressMask | KeyReleaseMask);

  XMapRaised (mydisplay, mywindow);

  if (use_pixmap)
    mypixmap = create_pixmap (mydisplay, mywindow);
  else
    mypixmap = mywindow;

  if (mypixmap != mywindow)
    {
      XSetForeground (mydisplay, mygc, black);
      XFillRectangle (mydisplay, mypixmap, mygc, 0, 0, window_width, window_height);
      XSetForeground (mydisplay, mygc, white);
    }

  setup_colors ();

  blank_display = 1;
  last_pixel = black;

  setup_keyboard ();
}


void term_graphics ()
{
}

int stepflag = 0;
int traceflag = 0;

void handle_input ()
{
  XEvent event;

  while (XEventsQueued (mydisplay, QueuedAfterReading) != 0)
    {
      XNextEvent (mydisplay, & event);
      switch (event.type)
	{
	case KeyPress:
	case KeyRelease:
	  if (event.xkey.keycode == escape_code)
	    {
	      stepflag = 1;
	      traceflag = 0;
	    }
	  else if (keymap [event.xkey.keycode])
	    {
	      *(keymap[event.xkey.keycode]) = (event.type == KeyPress);
	    }
	  break;
	case MotionNotify:
//	  joystick.x = (event.xmotion.x * 256.0) / window_width;
//	  joystick.y = (event.xmotion.y * 256.0) / window_height;
	  break;
	case NoExpose:
	  break;
	default:
	  printf ("unknown event %d\n", event.type);
	  break;
	}
    }
}

//int drawn = 0;


void draw_line (int x1, int y1, int x2, int y2, int color, int z)
{
  
//printf("xdraw_line %i,%i,%i,%i,%i,%i\n", x1, y1, x2, y2,color,z);
//drawn++;
  if (z==0) return;
  if (color==0) return;

  
  
//y1 = 930 -y1;
//y2 = 930 -y2;

  unsigned long pixel;
 z=15;
 color = 7;
 
  if (z != 0)
    {
      if ((x1 == x2) && (y1 == y2))
	x2++;
      pixel = color_value [color & 0x07] [z];
      if (pixel != last_pixel)
	{
	  XSetForeground (mydisplay, mygc, pixel);
	  pixel = last_pixel;
	}
      XDrawLine (mydisplay, mypixmap, mygc,
		 x1 >> smallwindow, y1 >> smallwindow,
		 x2 >> smallwindow, y2 >> smallwindow);
      blank_display = 0;
    }
}


void xopen_page (int step)
{
// drawn = 0;
  XSynchronize (mydisplay, step);

  if (! blank_display)
    {
      if (mypixmap == mywindow)
	XClearWindow (mydisplay, mywindow);
      else
	{
	  XSetForeground (mydisplay, mygc, black);
	  XFillRectangle (mydisplay, mypixmap, mygc, 0, 0, window_width, window_height);
	  XSetForeground (mydisplay, mygc, white);
	}
    }

  blank_display = 1;
}


void xclose_page (void)
{
// printf("DRAW called: %i\n",drawn);
  if (! blank_display)
    {
      if (mywindow != mypixmap)
	{
	  XCopyArea (mydisplay, mypixmap, mywindow, mygc,
		     0, 0, /* source upper left X, Y */
		     window_width, window_height, /* width, height */
		     0, 0); /* dest upper left X, Y */
	}

      XFlush (mydisplay);
    }
}


int repeat_frame (void)
{
  return (0);
}


void bell (void)
{
  XBell (mydisplay, 50);
}
