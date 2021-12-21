/*
   visual.c

   Bill Kendrick & Mike Hufnagel
   Last modified: 11/18/95 (clean up)
*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "visual.h"

int SetUpVisual(Display *display, int screen, Visual **visual, int *depth)
{
  int number_visuals;
  XVisualInfo *visual_array;
  XVisualInfo visual_info_template;
  int status;
  
  status = False;
  
  /* Will the default visual work? */
  if (DefaultVisual(display,screen)->class == PseudoColor ||
      DefaultVisual(display,screen)->class == TrueColor)
    {
      *visual = DefaultVisual(display,screen);
      *depth = DefaultDepth(display,screen);
      status = True;
    }
  else
    {
      /* we look for a PseudoColor visual on the current screen */
      visual_info_template.class = PseudoColor;
      visual_info_template.screen = screen;
      visual_array = XGetVisualInfo(display,VisualClassMask | VisualScreenMask,
                                  &visual_info_template,&number_visuals);
      
      /* Did we find one? */
      if ((number_visuals>0) && (visual_array != NULL))
        {
          /* Choose the first PseudoColor visual (LAZY!) */
          *visual = visual_array[0].visual;
          *depth = visual_array[0].depth;
          XFree(visual_array);   /* done with this chunk of RAM */
          status = True;
        }
      else
        {
          *visual = CopyFromParent;
          status = False;  /* sorry - live with what's left */
        }
    }
  
  return(status);
}
