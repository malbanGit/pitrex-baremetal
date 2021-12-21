/* 
   connect.h
	
   Mike Hufnagel and Bill Kendrick
   Last modified 11/17/95 (changed to return NULL on error)
	
   Assist in connecting to the X server.
*/

#ifndef CONNECT_H
#define CONNECT_H

#include <X11/Xlib.h>

Display *ConnectToServer(char* display_name, int *screen, Window *rootwindow);
/* 
   Connects to an X server, gets default screen # and root window id for
   that display (server) and screen 
	
   returns: Pointer to the display or NULL if there was a problem.
*/

#endif /* CONNECT_H */
