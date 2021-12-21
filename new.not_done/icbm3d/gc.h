/* 
   gc.h
	
   Mike Hufnagel and Bill Kendrick
   Last modified: 11/18/95 (clean up)
	
   Graphic Context Routines.
*/

#ifndef GC_H
#define GC_H

#include <X11/Xlib.h>

GC CreateGC(Display *display, Drawable drawable, unsigned long forecolor,
            unsigned long backcolor);
/* 
   creates a graphics context with the given foreground and background
   pixel values.
   
   Returns a GC
*/

GC CreateClearGC(Display *display, Drawable drawable);
/* 
   creates a graphics context with a clear function.
   
   Returns a GC
*/

GC CreateXorGC(Display *display, Drawable drawable, unsigned long forecolor,
	       unsigned long backcolor);
/*
   Creates an XOR graphics context.

   Returns a GC
*/

GC CreateAndGC(Display *display, Drawable drawable, unsigned long forecolor,
	       unsigned long backcolor);
/*
   Creates an AND graphics context.

   Returns a GC
*/

GC CreateAndInvertedGC(Display *display, Drawable drawable,
		       unsigned long forecolor, unsigned long backcolor);
/*
   Creates an AND_Inverted graphics context.

   Returns a GC
*/

GC CreateOrGC(Display *display, Drawable drawable, unsigned long forecolor,
	      unsigned long backcolor);
/*
   Creates an Or graphics context.

   Returns a GC
*/

#endif /* GC_H */

