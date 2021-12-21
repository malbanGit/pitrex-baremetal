/*
   text.h

   Mike Hufnagel & Bill Kendrick
   Last modified: 11/18/95 (clean up)

   Font functions, text messages, etc.  */

#ifndef TEXT_H
#define TEXT_H

#include <X11/Xlib.h>

int FontHeight(XFontStruct *font_struct);
/*
   Returns height of text in a given font.
*/

XFontStruct *LoadFont(Display *display, char* font_name,
		      char* fallback_font_name);
/*
   Loads an X font into an XFontStruct and returns a pointer to the 
   struct.  If "font_name" can't be loaded, it tries to load
   "fallback_font_name".  If THAT fails too, it will return NULL.
*/

void drawtext(Display *display, Window window, GC gc, int x, int y, char *s);

/*
   Draws string 's' into 'window' on a 'display' at an 'x,y' location using
   the graphic context 'gc'
*/

void drawcenteredtext(Display *display, Window window, GC gc, int x1, int x2,
		      int y, char *s, XFontStruct *font);
/*
  Draws text centered
  */


#endif /* TEXT_H */
