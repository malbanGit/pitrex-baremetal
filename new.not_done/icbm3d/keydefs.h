/*
  keydefs.h
  
  for ICBM3D
  
  Keyboard definitions.
  Use X-Event strings.
  
  Make sure not to use the same key twice!
  
  
  Bill Kendrick
  New Breed Software
  nbs@sonic.net
  http://www.sonic.net/~nbs/unix/x/icbm3d/
*/


/* Game controls: */                       /* Distribution defaults: */

#define KEY_PAUSE               "P"        /* P */
#define KEY_QUIT                "Q"        /* Q */
#define KEY_ABORT               "X"        /* X */


/* Level controls: */

#define KEY_LEVEL_NEXT          "L"        /* L */
#define KEY_LEVEL_PREV          "K"        /* K */


/* Screen options: */

#define KEY_TOG_TARGET          "T"        /* T */
#define KEY_TOG_FASTDRAW        "F"        /* F */
#define KEY_TOG_HALFFRAME       "H"        /* H */
#define KEY_TOG_CITY_STYLE      "C"        /* C */


/* Viewing controls: */

#define KEY_VIEW_MISSILE        "M"        /* M */
#define KEY_VIEW_MISSILE_SELECT "Tab"      /* Tab */
#define KEY_VIEW_BULLET         "B"        /* B */
#define KEY_IN                  "I"        /* I */
#define KEY_OUT                 "O"        /* O */


/* Movement/firing: */

#define KEY_LEFT                "Left"     /* Left */
#define KEY_RIGHT               "Right"    /* Right */
#define KEY_AWAY                "Up"       /* Up */
#define KEY_TOWARDS             "Down"     /* Down */
#define KEY_UP                  "A"        /* A */
#define KEY_DOWN                "Z"        /* Z */
#define KEY_FIRE                "Space"    /* Space */


/*
  Note:  KP_Up, KP_Down, KP_Left and KP_Right are hard-coded to
  do the same as what KEY_UP, KEY_DOWN, KEY_LEFT and KEY_RIGHT do.
  */
