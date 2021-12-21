/*
   window.c
	
   Mike Hufnagel & Bill Kendrick
   Last modified: 11/18/95
*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "window.h"

Window OpenWindow(Display *display, Window parent, int x, int y, int width,
		  int height, unsigned long bordercolor, 
		  unsigned long backcolor, unsigned long event_mask,
		  Visual *visual)
{
  Window window;
  XSetWindowAttributes attributes;
  unsigned long attr_mask;
  
  /* set up window attributes first */
  
  attributes.event_mask = event_mask;
  attributes.border_pixel = bordercolor;
  attributes.background_pixel = backcolor;
  attr_mask = CWEventMask | CWBackPixel | CWBorderPixel;
  
  /* uncomment this to override-redirect: */
  /*
    attributes.override_redirect = True;
    attr_mask = attr_mask | CWOverrideRedirect;
    */
  
  /* create window! */
  window = XCreateWindow(display,parent,x,y,width,height,BORDER_WIDTH,
			 CopyFromParent,InputOutput,visual,attr_mask,
			 &attributes);
  
  return(window);
}




