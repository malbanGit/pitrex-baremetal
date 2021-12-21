/*
   text.c

   Mike Hufnagel & Bill Kendrick
   Last modified: 11/18/95 (clean up)
*/

#include <X11/Xlib.h>
#include "text.h"

int FontHeight(XFontStruct *font_struct)
{
  return(font_struct->ascent + font_struct->descent);
}

XFontStruct *LoadFont(Display *display, char* font_name,
		      char* fallback_font_name)
{
  XFontStruct *font_struct;

  font_struct = XLoadQueryFont(display, font_name);
  if (font_struct == (XFontStruct*)NULL)
    {
      /* try to load the fallbacck font */
      font_struct = XLoadQueryFont(display,fallback_font_name);
    }

  return(font_struct);
}


void drawtext(Display *display, Window window, GC gc, int x, int y, char *s)
{
  XDrawImageString(display, window, gc, x, y, s, strlen(s));
}

void drawcenteredtext(Display *display, Window window, GC gc, int x1, int x2,
		      int y, char *s, XFontStruct *font)
{
  int x;
  
  x = ((x2 - x1) / 2) + x1;
  x = x - (XTextWidth(font, s, strlen(s))) / 2;
  
  drawtext(display, window, gc, x, y, s);
}
