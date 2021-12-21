/*
   color.c

   Bill Kendrick & Mike Hufnagel
   Last modified: 11/18/95 (combined color.c with colormap.c)
*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "color.h"

unsigned long AllocNamedColor(Display *display, Colormap colormap,
                              char* colorname, unsigned long default_color)
{
  XColor hardwarecolor,exactcolor;
  unsigned long color;
  int status;
  
  status = XAllocNamedColor(display,colormap,colorname,&hardwarecolor,
			  &exactcolor);
  if (status != 0)
    color = hardwarecolor.pixel;
  else
    color = default_color;
  
  return(color);
}

int SetUpColormap(Display *display, int screen, Window window, Visual *visual,
		  Colormap *colormap)
{
  int status = False;
  
  if (visual == DefaultVisual(display,screen))
    {
      *colormap = DefaultColormap(display,screen);
      status = True;
    }
  else
    {
      *colormap = XCreateColormap(display,window,visual,AllocNone);
      if (*colormap != None)
        {
          XSetWindowColormap(display,window,*colormap);
          status = True;
        }
      else
        *colormap = DefaultColormap(display,screen);
    }
  
  return(status);
}

