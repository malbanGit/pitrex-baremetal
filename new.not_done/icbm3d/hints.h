/* 
   hints.h
	
   Mike Hufnagel and Bill Kendrick
   Last modified: 11/18/95 (removed things that were local prototypes)

   Class hint functions.
*/

#ifndef HINTS_H
#define HINTS_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define APPL_CLASS "Examples"

void SetSizeHints(Display *display, Window window, int x, int y, int width,
		  int height);
/*
   Sends the size hints to the WM
*/

void SetStandardHints(Display *display, Window window, char* app_name,
		      char* wind_name, int x, int y, int width, int height);
/*
   Makes suggestions to the WM about the look of new windows.
*/ 

void SetWindowName(Display *display, Window window, char* name);
/*
   Sets a window's name.
*/

#endif /* HINTS_H */
