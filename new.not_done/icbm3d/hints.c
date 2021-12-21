/*
   hints.c
	
   Mike Hufnagel & Bill Kendrick
   Last modified: 11/18/95
*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "hints.h"

/* Local prototypes */

void SetClassHints(Display *display, Window window, char *res_name, 
		   char *res_class);
void FillSizeHints(int x, int y, int width, int height, XSizeHints *sizehints);
void SetWMHints(Display *display, Window window, int initial_state);

/* Local Definations */

void SetClassHints(Display *display, Window window, char *res_name,
		   char *res_class)
/*
   Sets a windows class hints.
*/
{
  XClassHint class_hints;

  class_hints.res_class = res_class;
  class_hints.res_name = res_name;

  XSetClassHint(display, window, &class_hints);
}

void FillSizeHints(int x, int y, int width, int height, XSizeHints *sizehints)
/*
   Fills in an XSizeHints struct
*/
{
  /* set size hints */
  sizehints->x = x;             /* obsolete */
  sizehints->y = y;             /* obsolete */
  sizehints->height = height;   /* obsolete */
  sizehints->width = width;     /* obsolete */
  sizehints->min_height = height;
  sizehints->min_width = width;
  sizehints->flags = USPosition | USSize | PMinSize;

  /* if R4, we can set base stuff, too... */
#ifdef X11R4
  sizehints->base_height = height;
  sizehints->base_width = width;
  sizehints->flags = sizehints->flags | PBaseSize;
#endif
}

void SetWMHints(Display *display, Window window, int initial_state)
/*
   Sets the window manager hints.  "initial_state" can be:
   Withdrawn (0), Normal (1) or Iconic (3).
*/
{
  XWMHints wm_hints;

  wm_hints.flags = InputHint | StateHint;
  wm_hints.initial_state = initial_state;
  wm_hints.input = True;

  XSetWMHints(display,window,&wm_hints);
}

/* Global Definations */

void SetSizeHints(Display *display, Window window, int x, int y, int width,
		  int height)
{
  XSizeHints sizehints;

  /* get hint values */
  FillSizeHints(x,y,width,height,&sizehints);

  /* test for R4 - adds XSetWMNormalHints */
#ifdef X11R4
  XSetWMNormalHints(display,window,&sizehints);
#else
  XSetNormalHints(display,window,&sizehints);
#endif
}

void SetStandardHints(Display *display, Window window, char* app_name,
		      char* wind_name, int x, int y, int width, int height)
{
  SetSizeHints(display,window,x,y,width,height);
  SetWindowName(display,window,wind_name);
  SetClassHints(display,window,app_name,APPL_CLASS);
  SetWMHints(display,window,NormalState);
}

void SetWindowName(Display *display, Window window, char* name)
{
  XStoreName(display, window, name);
}











