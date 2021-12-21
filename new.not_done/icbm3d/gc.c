/*
   gc.c
	
   Mike Hufnagel & Bill Kendrick
   Last modified: 11/18/95
*/

#include <X11/Xlib.h>
#include "gc.h"

GC CreateGC(Display *display, Drawable drawable, unsigned long forecolor,
            unsigned long backcolor)
{
  XGCValues xgcvalues;
  GC gc;
  
  xgcvalues.foreground = forecolor;
  xgcvalues.background = backcolor;
  gc = XCreateGC(display,drawable,(GCForeground | GCBackground),
		 &xgcvalues);
  
  return(gc);
}

GC CreateClearGC(Display *display, Drawable drawable)
{
  XGCValues xgcvalues;
  GC gc;
  
  xgcvalues.function = GXclear;
  xgcvalues.fill_style = FillSolid;
  gc = XCreateGC(display, drawable, (GCFunction), &xgcvalues);
  
  return(gc);
}

GC CreateXorGC(Display *display, Drawable drawable, unsigned long forecolor,
	       unsigned long backcolor)
{
  XGCValues xgcvalues;
  GC gc;

  xgcvalues.foreground = forecolor ^ backcolor;
  xgcvalues.background = 0;
  xgcvalues.function = GXxor;

  gc = XCreateGC(display,drawable,(GCForeground | GCBackground | GCFunction),
		 &xgcvalues);

  return (gc);
}

GC CreateAndGC(Display *display, Drawable drawable, unsigned long forecolor,
	       unsigned long backcolor)
{
  XGCValues xgcvalues;
  GC gc;

  xgcvalues.foreground = forecolor;
  xgcvalues.background = backcolor;
  xgcvalues.function = GXand;

  gc = XCreateGC(display,drawable,(GCForeground | GCBackground | GCFunction),
		 &xgcvalues);

  return (gc);
}


GC CreateAndInvertedGC(Display *display, Drawable drawable,
		       unsigned long forecolor, unsigned long backcolor)
{
  XGCValues xgcvalues;
  GC gc;

  xgcvalues.foreground = forecolor;
  xgcvalues.background = backcolor;
  xgcvalues.function = GXandInverted;

  gc = XCreateGC(display,drawable,(GCForeground | GCBackground | GCFunction),
		 &xgcvalues);

  return (gc);
}

GC CreateOrGC(Display *display, Drawable drawable, unsigned long forecolor,
	       unsigned long backcolor)
{
  XGCValues xgcvalues;
  GC gc;

  xgcvalues.foreground = forecolor;
  xgcvalues.background = backcolor;
  xgcvalues.function = GXor;
  
  gc = XCreateGC(display,drawable,(GCForeground | GCBackground | GCFunction),
		 &xgcvalues);

  return (gc);
}
