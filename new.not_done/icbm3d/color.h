/*
   color.h
   
   Bill Kendrick & Mike Hufnagel
   Last modified: 11/18/95 (combined color.h with colormap.h)

   Color related routines routines.
*/

#ifndef COLOR_H
#define COLOR_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

unsigned long AllocNamedColor(Display *display, Colormap colormap,
                              char* colorname, unsigned long default_color);
/*
   Allocates a read-only (sharable) color cell in the given colormap,
   using the RGB database to convert the color name to an RGB value.
   See /usr/lib/X11/rgb.txt for the database listing.
*/


int SetUpColormap(Display *display, int screen, Window window, Visual *visual,
		  Colormap *colormap);
/*
   Create an X colormap using the given screen and visual.  Uses the
   default colormap, if possible, to avoid palette switches when
   other windows are brought "forward".
*/

#endif /* COLOR_H */
