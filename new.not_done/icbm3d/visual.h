/*
   visual.h

   Bill Kendrick & Mike Hufnagel
   Last modified: 11/18/95 (clean up)

   Visual routine(s).
*/

#ifndef VISUAL_H
#define VISUAL_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

int SetUpVisual(Display *display, int screen, Visual **visual, int *depth);
/*
   Grabs a PseudoColor visual.
*/

#endif /* VISUAL_H */
