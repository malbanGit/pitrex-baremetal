/*
   connect.c
	
   Mike Hufnagel & Bill Kendrick
   Last modified 11/17/95 (changed to return NULL on error)
*/

#include <X11/Xlib.h>
#include "connect.h"

Display *ConnectToServer(char* display_name, int *screen, Window *rootwindow)
{
  Display *display;

  /* connect & check for problems */
  display = XOpenDisplay(display_name);
  if (display != NULL)
    {
      /* get screen # */
      *screen = DefaultScreen(display);

      /* get screens root window id */
      *rootwindow = RootWindow(display,*screen);
    }

  return(display);
}
