/* 
   window.h
	
   Mike Hufnagel and Bill Kendrick
   Last modified: 11/18/95 (clean up)
	
   Window creation routines.
*/

#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define BORDER_WIDTH 2

Window OpenWindow(Display *display, Window parent, int x, int y, int width,
		  int height, unsigned long bordercolor, 
		  unsigned long backcolor, unsigned long event_mask,
		  Visual *visual);
/*
   Creates a window.
	
   returns: Window id.
*/

#endif /* WINDOW_H */
