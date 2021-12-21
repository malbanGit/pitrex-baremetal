/*
  icbm3d.c
  
  ICBM3D 0.4
  
  A "Missile Command" clone, in Three Dimensions
  
  by Bill Kendrick
  
  New Breed Software
  nbs@sonic.net
  http://www.sonic.net/~nbs/unix/x/icbm3d/
  
  With help from:
  - Brian Mordecai  (random comments and suggestions)
  - Matt Hermes     (random comments and suggestions)
  - Roger Mamer     ("make the screen flash!", and beloved sysadmin)
  - Steve Becerra   (targetter toggle option suggestion)
  - Martin Green    (growing cities idea)
  - Robert Hamilton (code speed enhancements)       <hamil@barbarian.tamu.edu>
  - Luis Fernandes  (XCreatePixmap correction)             <elf@ee.ryerson.ca>
  - Tim Rightnour   (Better makefile)                       <root@garbled.net>

  Based on Missile Command
  by Dave Thuerer of Atari, 1981

  March 2, 1998 - July 29, 1998
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <sys/types.h>
#include <sys/time.h>
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "window.h"
#include "connect.h"
#include "hints.h"
#include "visual.h"
#include "gc.h"
#include "color.h"
#include "randnum.h"
#include "text.h"
#include "keydefs.h"
#include "language.h"


/* Use backbuffer? */

#define DOUBLE_BUFFER True


/* Drawing speed-ups: */

#define FASTDRAW_BY_DEFAULT 0
#define HALFFRAME_BY_DEFAULT 0
#define SIMPLE_CITIES_BY_DEFAULT 1


/* Speed controls: */

#define FRAMERATE 70000   /* fps = 100,000 / FRAMERATE */


/* Object definitions: */

#define OBJ_SMOKE 0
#define OBJ_MISSILE 1
#define OBJ_MISSILE_TARGETTER 2
#define OBJ_CITY 3
#define OBJ_BASE 4
#define OBJ_POINTER 5
#define OBJ_POINTER_SHADOW 6
#define OBJ_POINTER_EXCITED 7
#define OBJ_EXPLOSION 8
#define OBJ_GROUND 9
#define OBJ_SKY 10
#define OBJ_GROUND_HORZ 11
#define OBJ_FLAME 12
#define OBJ_BULLET 13
#define OBJ_BULLET_DEST 14
#define OBJ_DEBRIS 15
#define OBJ_LETTER 16
#define OBJ_DEFENSEBASE 17
#define OBJ_PLANE 18
#define NUM_OBJECTS 19

char * object_colors[NUM_OBJECTS] = {
  "grey",
  "yellow",
  "dark grey",
  "blue",
  "brown",
  "white",
  "grey",
  "red",
  "red",
  "dark green",
  "navy blue",
  "black",
  "orange",
  "purple",
  "purple",
  "red",
  "cyan",
  "green",
  "cyan"};


/* Keys: */

#define HKEY_UP      0
#define HKEY_DOWN    1
#define HKEY_LEFT    2
#define HKEY_RIGHT   3
#define HKEY_AWAY    4
#define HKEY_TOWARDS 5

int key_holds[6];


/* Bonus mode definitions: */

#define BONUS_OFF 0
#define BONUS_BULLETS 1
#define BONUS_CITIES 2
#define BONUS_DONE 3


/* City/Defensebase chunk definitions: */

#define CHUNK_IS_CITY 0
#define CHUNK_IS_DEFENSEBASE 1


/* Global X-Window Variables: */

char server[512];
Display * display;
Colormap colormap;
Window window, root;
Pixmap backbuffer;
GC whitegc, blackgc;
GC colorgcs[NUM_OBJECTS], yougc;
XFontStruct * font;
int fh, screen, black, white, has_color;


/* Window size: */

#define HEIGHT 400
#define WIDTH 600


/* 3D controls: */

#define DISTANCE 400
#define ASPECT 200


/* Game area size: */

#define Y_HEIGHT 200
#define X_WIDTH 400
#define Z_DEPTH 400


/* Game area values: */

#define START_HEIGHT -200
#define SPLIT_HEIGHT 0


/* Max. objects: */

#define MAX_MISSILES 20
#define MAX_EXPLOSIONS 100
#define MAX_BULLETS 20
#define MAX_SMOKES 5000
#define MAX_DEBRISERS 16
#define MAX_CITYCHUNKS 128
#define MAX_DEBRIS 256
#define MAX_LETTERS 20
#define MAX_CITIES 16
#define MAX_DEFENSEBASES 4


/* Scoring: */

#define NEW_CITY_SCORE 10000


/* Typedefs: */

typedef struct player_type {
  float x, y, z;
} player_type;

typedef struct city_type {
  int alive, bonused;
  float x, z;
  float percent;
  int height[4], shape[4];
} city_type;

typedef struct defensebase_type {
  int alive, bonusing;
  float x, z;
  int bullets;
} defensebase_type;

typedef struct missile_type {
  int alive;
  float x, y, z;
  float xm, ym, zm;
  float destx, destz;
  int splitter;
  int spiraller;
} missile_type;

typedef struct bullet_type {
  int alive;
  float x, y, z;
  float xm, ym, zm;
  float destx, desty, destz;
} bullet_type; 

typedef struct explosion_type {
  int alive;
  float x, y, z;
  int size, sizem;
  int users;
} explosion_type;

typedef struct smoke_type {
  int alive;
  float x, y, z;
  int owner;
  int attachtime;
} smoke_type;

typedef struct debris_type {
  int alive;
  float x, y, z;
  float xm, ym, zm;
  int time;
} debris_type;

typedef struct debriser_type {
  int alive;
  float x, y, z;
  int time;
} debriser_type;

typedef struct citychunk_type {
  int alive;
  float x1, y1, z1, x2, y2, z2;
  float xm1, ym1, zm1, xm2, ym2, zm2;
  int is_defensechunk;
} citychunk_type;

typedef struct letter_type {
  int alive;
  int killatdest;
  int wave;
  float x, y, z;
  float destx, desty, destz;
  float xm, ym, zm;
  char letter;
} letter_type;

typedef struct plane_type {
  int alive, hasdropped;
  float x, y, z;
  float xm, ym, zm;
  float wantx;
} plane_type;


/* Global game variables: */

int toggle, ttoggle, gameover, flashtime, flashcolors, num_missiles,
  chance_of_spiral, chance_of_split, missile_view, missile_view_missile,
  tries, show_targets, drawlinetoggle, fastdraw, how_many_coming, halfframe,
  num_bullets, num_explosions, bonus_mode, bonus_time, last_num_missiles,
  got_negative_time_padding, simple_cities, bullet_view,
  bullet_view_bullet, wind, windtime, title_mode, title_timer, chance_of_plane;
float windxm, windzm;
player_type player;
missile_type missiles[MAX_MISSILES];
bullet_type bullets[MAX_BULLETS];
explosion_type explosions[MAX_EXPLOSIONS];
smoke_type smokes[MAX_SMOKES];
debris_type debris[MAX_DEBRIS];
debriser_type debrisers[MAX_DEBRISERS];
citychunk_type citychunks[MAX_CITYCHUNKS];
letter_type letters[MAX_LETTERS];
city_type cities[MAX_CITIES];
defensebase_type defensebases[MAX_DEFENSEBASES];
plane_type plane;
float anglex, angley, cos_anglex, sin_anglex, cos_angley, sin_angley;
int zoom;
int x_change, y_change, z_change;
int level, showingletters, paused, score, last_score;


/* Local function prototypes: */

void eventloop(void);
void setup(int argc, char * argv[]);
void Xsetup();
unsigned long MyAllocNamedColor(Display *display, Colormap colormap,
                                char* colorname, unsigned long default_color,
                                int has_color);
void initlevel();
void drawline3d(float x1, float y1, float z1, float x2, float y2, float z2,
		GC color);
void drawpoint3d(float x, float y, float z2, GC color);
void addexplosion(float x, float y, float z, int whose);
void addsmoke(float x, float y, float z, int owner);
void addmissile(int which, int cansplit, float x, float y, float z,
		int angle, int whichangle);
void addbullet(float x, float y, float z);
void removemissile(int which, int cause_explosion);
void recalculatetrig();
void domove(void);
void killcity(int which);
void killdefensebase(int which);
void adddebris(float x, float y, float z);
void adddebriser(float x, float y, float z);
void addcitychunks(float x, float y, float z, int what);
void turnonletters(char * str, int wavey, float wantz);
void turnoffletters(void);
void initgame(void);
void endoflevel(void);
int calc3d(float * sx, float * sy, float x, float y, float z);
float sign(float v);


/* ---- MAIN FUNCTION ---- */

int main(int argc, char * argv[])
{
  /* Program setup: */
  
  setup(argc, argv);
  randinit();
  
  
  /* Connect to the X Servers and draw the program's windows: */
  
  Xsetup();
  
  
  /* Run the main loop: */
  
  eventloop();
  
  
  /* Nice message! */
  
  printf("\n\n\n\n\n\n");
  printf("Thank you for playing ICBM3D!\n\n");
  
  printf("If you have comments, suggestions, questions or donations,\n");
  printf("send them to:\n\n");
  
  printf("New Breed Software\n");
  printf("c/o Bill Kendrick\n");
  printf("619 Pole Line Road #249\n");
  printf("Davis, CA 95616 USA\n\n");
  
  printf("email: nbs@sonic.net\n");
  printf("web:   http://www.newbreedsoftware.com/\n\n");
  
  if (got_negative_time_padding)
    {
      printf("NOTE: You may wish to run the game in HALF-FRAME\n");
      printf("      and/or FASTDRAW mode on this machine, or\n");
      printf("      run it on a faster server...\n\n");
    }
}


/* --- MAIN EVENT LOOP --- */

void eventloop(void)
{
  int old_button, old_x, old_y, i, j, k, eventloopcounter, num_cities,
    found, angle, any, wave_count, which_color;
  long time_padding;
  char string[128], temp[128];
  XEvent event;
  char key[1024];
  KeySym keysym;
  XComposeStatus composestatus;
  struct timeval now, then;
  int done, do_move, c;
  float x1, y1, z1, x2, y2, z2, x3, y3, z3;
  
  
  /* Init event loop variables: */
  
  toggle = 0;
  ttoggle = 0;
  drawlinetoggle = 0;
  fastdraw = FASTDRAW_BY_DEFAULT;
  halfframe = HALFFRAME_BY_DEFAULT;
  simple_cities = SIMPLE_CITIES_BY_DEFAULT;
  got_negative_time_padding = 0;
  
  done = 0;
  
  
  /* Init Player variables: */
  
  player.x = X_WIDTH / 2;
  player.y = Y_HEIGHT / 2;
  player.z = Z_DEPTH / 2;
  old_button = -1;
  
  missile_view = 0;
  missile_view_missile = 0;
  bullet_view = 0;
  bullet_view_bullet = 0;
  show_targets = 1;
  
  anglex = 0;
  angley = 0;
  recalculatetrig();
  
  
  /* Init level 1: */
  
  initgame();
  
  initlevel();
  gameover = 1;
  turnonletters("ICBM-3D", 2, -100);
  title_mode = 1;
  title_timer = 0;
  
  
  /* -- MAIN LOOP -- */
  

  do
    {
      /* Toggle our toggle switches: */
      
      toggle = 1 - toggle;
      
      if (toggle == 0)
	ttoggle = 1 - ttoggle;
      
      drawlinetoggle = toggle;
      
      gettimeofday(&then, NULL);
      
      
      x_change = 0;
      y_change = 0;
      z_change = 0;
      
      
      /* Handle controls: */
      
      for (eventloopcounter = 0; eventloopcounter < 500; eventloopcounter++)
	{
	  if (XPending(display))
	    {
	      strcpy(key, "");
	      
	      XNextEvent(display, &event);
	      
	      if (event.type == KeyPress || event.type == KeyRelease)
		{
		  /* Get the key's name: */
		  
		  XLookupString(&event.xkey, key, 1, &keysym,
				&composestatus);
		  
		  if (XKeysymToString(keysym) != NULL)
		    strcpy(key, XKeysymToString(keysym));


		  /* Movement controls: */
		  
		  if (strcasecmp(key, KEY_UP) == 0)
		    {
		      /* A - Up: */
		      
		      key_holds[HKEY_UP] = event.type;
		    }
		  else if (strcasecmp(key, KEY_DOWN) == 0)
		    {
		      /* Z: - Down: */
		      
		      key_holds[HKEY_DOWN] = event.type;
		    }
		  else if (strcasecmp(key, "KP Left") == 0 ||
			   strcasecmp(key, KEY_LEFT) == 0)
		    {
		      /* Left - Left: */
		      
		      key_holds[HKEY_LEFT] = event.type;
		    }
		  else if (strcasecmp(key, "KP Right") == 0 ||
			   strcasecmp(key, KEY_RIGHT) == 0)
		    {
		      /* Right - Right: */
		      
		      key_holds[HKEY_RIGHT] = event.type;
		    }
		  else if (strcasecmp(key, "KP Up") == 0 ||
			   strcasecmp(key, KEY_AWAY) == 0)
		    {
		      /* Up - Away: */
		      
		      key_holds[HKEY_AWAY] = event.type;
		    }
		  else if (strcasecmp(key, "KP Down") == 0 ||
			   strcasecmp(key, KEY_TOWARDS) == 0)
		    {
		      /* Down - Towards: */
		      
		      key_holds[HKEY_TOWARDS] = event.type;
		    }


		  if (event.type == KeyPress)
		    {
		      if (strcasecmp(key, KEY_QUIT) == 0)
			{
			  /* Q: Quit: */
			  
			  done = True;
			}
		      else if (strcasecmp(key, KEY_ABORT) == 0)
			{
			  /* X: Abort game: */
			  
			  if (gameover == 0)
			    {
			      gameover = 1;
			      
			      for (i = 0; i < MAX_CITIES; i++)
				{
				  if (cities[i].alive)
				    killcity(i);
				}
			      
			      flashtime = 30;
			      flashcolors = 1;
			      
			      turnonletters(LANG_GAMEOVER, 0, 0);
			    }
			}
		      else if (strcasecmp(key, KEY_LEVEL_NEXT) == 0)
			{
			  /* L: Change level (+1): */
			  
			  if (gameover == 1)
			    {
			      level = level + 1;
			      if (level > 99)
				level = 1;
			    }
			}
		      else if (strcasecmp(key, KEY_LEVEL_PREV) == 0)
			{
			  /* K: Change level (-1): */
			  
			  if (gameover == 1)
			    {
			      level = level - 1;
			      if (level < 1)
				level = 99;
			    }
			}
		      else if (strcasecmp(key, KEY_TOG_TARGET) == 0)
			{
			  /* T: Toggle missile targetting lines: */
			  
			  show_targets = 1 - show_targets;
			}
		      else if (strcasecmp(key, KEY_TOG_FASTDRAW) == 0)
			{
			  /* F: Toggle Fast-Draw mode: */
			  
			  fastdraw = 1 - fastdraw;
			}
		      else if (strcasecmp(key, KEY_TOG_HALFFRAME) == 0)
			{
			  /* H: Toggle Half-Frame mode: */
			  
			  halfframe = 1 - halfframe;
			}
		      else if (strcasecmp(key, KEY_TOG_CITY_STYLE) == 0)
			{
			  /* C: Toggle Simple-Cities mode: */
			  
			  simple_cities = 1 - simple_cities;
			}
		      else if (strcasecmp(key, KEY_VIEW_MISSILE) == 0)
			{
			  /* M: Switch to missile view: */
			  
			  bullet_view = 0;
			  missile_view = 1 - missile_view;
			  
			  if (missile_view == 1)
			    {
			      tries = 0;
			      
			      while (missiles[missile_view_missile].alive == 0
				     && tries < MAX_MISSILES)
				{
				  missile_view_missile =
				    missile_view_missile + 1;
				  
				  if (missile_view_missile >= MAX_MISSILES)
				    missile_view_missile = 0;
				  
				  tries++;
				}
			      
			      if (tries >= MAX_MISSILES)
				{
				  missile_view = 0;
				  recalculatetrig();
				}
			    }
			  else
			    recalculatetrig();
			}
		      else if (strcasecmp(key, KEY_VIEW_BULLET) == 0 &&
			       1 == 0)
			{
			  /* B: Switch to bullet view: */
			  
			  missile_view = 0;
			  bullet_view = 1 - bullet_view;
			  
			  if (bullet_view == 1)
			    {
			      tries = 0;
			      
			      while (bullets[bullet_view_bullet].alive == 0 &&
				     tries < MAX_BULLETS)
				{
				  bullet_view_bullet = bullet_view_bullet + 1;
				  
				  if (bullet_view_bullet >= MAX_BULLETS)
				    bullet_view_bullet = 0;
				  
				  tries++;
				}
			      
			      if (tries >= MAX_BULLETS)
				{
				  bullet_view = 0;
				  recalculatetrig();
				}
			    }
			  else
			    recalculatetrig();
			}
		      else if (strcasecmp(key, KEY_VIEW_MISSILE_SELECT) == 0)
			{
			  /* Tab: Select a missile to view from: */
			  
			  tries = 0;
			  
			  do
			    {
			      missile_view_missile = missile_view_missile + 1;
			      
			      if (missile_view_missile >= MAX_MISSILES)
				missile_view_missile = 0;
			      
			      tries++;
			    }
			  while (missiles[missile_view_missile].alive == 0 &&
				 tries < MAX_MISSILES);
			  
			  if (tries >= MAX_MISSILES)
			    {
			      missile_view = 0;
			      recalculatetrig();
			    }
			}
		      else if (strcasecmp(key, KEY_PAUSE) == 0)
			{
			  /* P: Pause / Unpause: */
			  
			  if (paused == 0)
			    {
			      if (gameover == 0)
				{
				  paused = 1;
				  turnonletters(LANG_PAUSED, 1, 0);
				}
			    }
			  else
			    {
			      paused = 0;
			      turnoffletters();
			    }
			}
		      else if (strcasecmp(key, KEY_FIRE) == 0)
			{
			  title_mode = 0;
			  
			  if (showingletters == 0 && last_num_missiles != 0)
			    {
			      /* Space: Fire: */
			      
			      addbullet(player.x, player.y, player.z);
			    }
			  else
			    {
			      /* Space: Begin Level / Unpause: */
			      
			      turnoffletters();
			      paused = 0;
			      
			      if (gameover == 1)
				{
				  gameover = 0;
				  initgame();
				  initlevel();
				}
			    }
			}
		      else if (strcasecmp(key, KEY_IN) == 0 ||
			       strcasecmp(key, KEY_OUT) == 0)
			{
			  /* I/O - Zoom In/Out: */
			  
			  i = 20;
			  
			  if (strcasecmp(key, KEY_OUT) == 0)
			    i = -20;
			  
			  zoom = zoom + i;
			  
			  if (zoom < -100)
			    zoom = -100;
			  else if (zoom > 100)
			    zoom = 100;
			}
		    }
		}
	      else if (event.type == ButtonPress)
		{
		  old_button = event.xbutton.button;
		  old_x = event.xbutton.x;
		  old_y = event.xbutton.y;
		}
	      else if (event.type == ButtonRelease)
		{
		  old_button = -1;
		}
	      else if (event.type == MotionNotify)
		{
		  /* Move pointer: */
		  
		  if (old_button == Button1)
		    {
		      /* Move left/right, up/down: */
		      
		      x_change = (event.xbutton.x - old_x) / 10;
		      y_change = (event.xbutton.y - old_y) / 10;
		      
		    }
		  else if (old_button == Button2)
		    {
		      /* Move left/right, in/out: */
		      
		      x_change = (event.xbutton.x - old_x) / 10;
		      z_change = -(event.xbutton.y - old_y) / 5;
		    }
		  else if (old_button == Button3)
		    {
		      /* Rotate angle: */
		      
		      anglex = anglex + event.xbutton.x - old_x;
		      
		      if (anglex < -30)
			anglex = -30;
		      
		      if (anglex > 30)
			anglex = 30;
		      
		      angley = angley   + event.xbutton.y - old_y;
		      
		      if (angley < -30)
			angley = -30;
		      
		      if (angley > 40)
			angley = 40;
		      
		      recalculatetrig();
		      
		      old_x = event.xbutton.x;
		      old_y = event.xbutton.y;
		    }
		  
		  domove();
		}
	    }
	}
      
      
      if (key_holds[HKEY_UP] == KeyPress)
	y_change = -10;
      else if (key_holds[HKEY_DOWN] == KeyPress)
	y_change = 10;

      if (key_holds[HKEY_LEFT] == KeyPress)
	x_change = -10;
      else if (key_holds[HKEY_RIGHT] == KeyPress)
	x_change = 10;
      
      if (key_holds[HKEY_AWAY] == KeyPress)
	z_change = 10;
      else if (key_holds[HKEY_TOWARDS] == KeyPress)
	z_change = -10;
      
      domove();
      
      
      if (paused == 0)
	{
	  /* Handle bullets: */
	  
	  num_bullets = 0;
	  
	  for (i = 0; i < MAX_BULLETS; i++)
	    {
	      if (bullets[i].alive == 1)
		{
		  num_bullets++;
		  
		  bullets[i].x = bullets[i].x + bullets[i].xm;
		  bullets[i].y = bullets[i].y + bullets[i].ym;
		  bullets[i].z = bullets[i].z + bullets[i].zm;
		  
		  
		  /* "Go off" when we hit the right spot: */
		  
		  if ((bullets[i].x >= bullets[i].destx - 5 &&
		       bullets[i].x <= bullets[i].destx + 5 &&
		       bullets[i].z >= bullets[i].destz - 5 &&
		       bullets[i].z <= bullets[i].destz + 5 &&
		       bullets[i].y <= bullets[i].desty) ||
		      bullets[i].y <= bullets[i].desty)
		    {
		      bullets[i].alive = 0;
		      
		      addexplosion(bullets[i].destx,
				   bullets[i].desty,
				   bullets[i].destz, 1);
		    }
		}
	    }
	  
	  
	  /* Handle debrisers: */
	  
	  for (i = 0; i < MAX_DEBRISERS; i++)
	    {
	      if (debrisers[i].alive == 1)
		{
		  if (randnum(500) < debrisers[i].time)
		    adddebris(debrisers[i].x + randnum(20) - 10,
			      debrisers[i].y,
			      debrisers[i].z + randnum(20) - 10);
		  
		  debrisers[i].time--;
		  
		  if (debrisers[i].time <= 0)
		    debrisers[i].alive = 0;
		}
	    }
	  
	  
	  /* Handle debris: */
	  
	  for (i = 0; i < MAX_DEBRIS; i++)
	    {
	      if (debris[i].alive == 1)
		{
		  debris[i].x = debris[i].x + debris[i].xm;
		  debris[i].y = debris[i].y + debris[i].ym;
		  debris[i].z = debris[i].z + debris[i].zm;
		  
		  if (debris[i].y > Y_HEIGHT)
		    {
		      debris[i].y = Y_HEIGHT;
		      debris[i].ym = - debris[i].ym / 4;
		    }
		  
		  debris[i].ym = debris[i].ym + 0.5;
		  
		  if (wind == 1)
		    {
		      debris[i].x = debris[i].x + windxm;
		      debris[i].z = debris[i].z + windzm;
		    }
		  
		  debris[i].time--;
		  
		  if (debris[i].time <= 0)
		    debris[i].alive = 0;
		}
	    }

	  
	  /* Handle cities: */
	  
	  for (i = 0; i < MAX_CITIES; i++)
	    {
	      if (cities[i].percent < 1.0)
		cities[i].percent = cities[i].percent + 0.05;
	    }
	  
	  
	  /* Handle city chunks: */
	  
	  for (i = 0; i < MAX_CITYCHUNKS; i++)
	    {
	      if (citychunks[i].alive == 1)
		{
		  citychunks[i].x1 = citychunks[i].x1 + citychunks[i].xm1;
		  citychunks[i].y1 = citychunks[i].y1 + citychunks[i].ym1;
		  citychunks[i].z1 = citychunks[i].z1 + citychunks[i].zm1;
		  
		  citychunks[i].x2 = citychunks[i].x2 + citychunks[i].xm2;
		  citychunks[i].y2 = citychunks[i].y2 + citychunks[i].ym2;
		  citychunks[i].z2 = citychunks[i].z2 + citychunks[i].zm2;
		  
		  if (citychunks[i].y1 > Y_HEIGHT ||
		      citychunks[i].y2 > Y_HEIGHT)
		    citychunks[i].alive = 0;
		  
		  citychunks[i].ym1 = citychunks[i].ym1 + 0.3;
		  citychunks[i].ym2 = citychunks[i].ym2 + 0.3;
		}
	    }
	  
	  
	  num_explosions = 0;
	  
	  /* Handle explosions: */

	  for (i = 0; i < MAX_EXPLOSIONS; i++)
	    {
	      if (explosions[i].alive == 1)
		{
		  num_explosions++;
		  
		  if (toggle == 0)
		    {
		      explosions[i].size = explosions[i].size +
			explosions[i].sizem;
		      
		      if (explosions[i].size >= 20)
			explosions[i].sizem = -1;
		      
		      if (explosions[i].size <= 0)
			explosions[i].alive = 0;
		    }
		}
	    }
	  
	  
	  /* Handle missiles: */
	  
	  for (i = 0; i < MAX_MISSILES; i++)
	    {
	      if (missiles[i].alive == 1)
		{
		  num_missiles++;
		  
		  if (toggle == 0)
		    {
		      /* Move missile: */
		      
		      missiles[i].x = missiles[i].x + missiles[i].xm;
		      missiles[i].y = missiles[i].y + missiles[i].ym;
		      missiles[i].z = missiles[i].z + missiles[i].zm;
		      
		      
		      /* Add some smoke: */
		      
		      if (ttoggle == 0 && toggle == 0)
			addsmoke(missiles[i].x, missiles[i].y, missiles[i].z,
				 i);
		      
		      
		      if (missiles[i].y > Y_HEIGHT)
			{
			  /* If it hit the ground, get rid of it: */
			  
			  removemissile(i, 1);
			}
		      else
			{
			  /* Check for collissions with explosions: */
			  
			  for (j = 0; j < MAX_EXPLOSIONS; j++)
			    {
			      x1 = explosions[j].x - explosions[j].size - 10;
			      y1 = explosions[j].y - explosions[j].size - 10;
			      z1 = explosions[j].z - explosions[j].size - 10;
			      
			      x2 = explosions[j].x + explosions[j].size + 10;
			      y2 = explosions[j].y + explosions[j].size + 10;
			      z2 = explosions[j].z + explosions[j].size + 10;
			      
			      if (missiles[i].x >= x1 && missiles[i].x <= x2 &&
				  missiles[i].y >= y1 && missiles[i].y <= y2 &&
				  missiles[i].z >= z1 && missiles[i].z <= z2 &&
				  missiles[i].alive == 1 &&
				  explosions[j].alive == 1)
				{
				  removemissile(i, 1);
				  
				  if (explosions[j].users == 1)
				    score = score + Y_HEIGHT - missiles[i].y;
				}
			    }
			}
		      
		      
		      /* Control spiral: */
		      
		      if (missiles[i].spiraller == 1)
			{
			  if (missiles[i].x > missiles[i].destx)
			    missiles[i].xm = missiles[i].xm - 0.125;
			  else
			    missiles[i].xm = missiles[i].xm + 0.125;

			  if (missiles[i].z > missiles[i].destz)
			    missiles[i].zm = missiles[i].zm - 0.125;
			  else
			    missiles[i].zm = missiles[i].zm + 0.125;
			}

		      
		      /* Split missile: */
		      
		      if (missiles[i].splitter == 1 &&
			  missiles[i].y >= SPLIT_HEIGHT)
			{
			  /* Turn off this missile (it gets replaced): */
			  
			  removemissile(i, 0);
			  
			  
			  /* Add a burst of 3 more missiles: */
			  
			  angle = randnum(360);
			  
			  for (j = 0; j < 3; j++)
			    {
			      /* Find a slot for a missile: */
			      
			      found = -1;
			      
			      for (k = 0; k < MAX_MISSILES && found == -1; k++)
				{
				  if (missiles[k].alive == 0)
				    found = k;
				}
			      
			      if (found != -1)
				{
				  addmissile(found, 0,
					     missiles[i].x,
					     missiles[i].y,
					     missiles[i].z, angle, j);
				}
			    }
			}
		    }
		}
	      else if (showingletters == 0)
		{
		  /* Possibly create a new missile: */
		  
		  if ((randnum(5000) <= (level / 5) || num_missiles == 0) &&
		      how_many_coming > 0)
		    {
		      addmissile(i, 1, -1, -1, -1, 0, 0);
		      num_missiles++;
		      how_many_coming--;
		    }
		}
	    }
	  
	  
	  /* Handle plane: */
	  
	  if (plane.alive == 1)
	    {
	      /* Move plane: */
	      
	      plane.x = plane.x + plane.xm;
	      plane.y = plane.y + plane.ym;
	      plane.z = plane.z + plane.zm;
	      
	      
	      /* If it goes "off-screen", get rid of it 'naturally': */
	      
	      if (plane.x < -X_WIDTH * 0.5 ||
		  plane.x > X_WIDTH + X_WIDTH * 0.5)
		plane.alive = 0;
	      
	      
	      /* If it's getting too low, make it slow down: */
	      
	      if (plane.y > Y_HEIGHT / 2)
		plane.ym = plane.ym - 0.5;
	      
	      
	      /* Get the **** out of here!? */
	      
	      if (plane.hasdropped == 1)
		plane.ym = plane.ym - 0.25;
	      
	      
	      /* See if we hit an explosion: */
	      
	      for (i = 0; i < MAX_EXPLOSIONS; i++)
		{
		  if (explosions[i].x >= plane.x - 20 &&
		      explosions[i].x <= plane.x + 20 &&
		      explosions[i].y >= plane.y - 20 &&
		      explosions[i].y <= plane.y + 20 &&
		      explosions[i].z >= plane.z - 20 &&
		      explosions[i].z <= plane.z + 20)
		    {
		      addexplosion(plane.x, plane.y, plane.z, 0);
		      plane.alive = 0;
		      score = score + 200 + (300 * plane.hasdropped);
		    }
		}
	      
	      
	      /* Drop a missile: */
	      
	      if (plane.hasdropped == 0 && plane.x >= 0 && plane.x <= X_WIDTH)
		{
		  if ((plane.x >= plane.wantx && plane.xm > 0) ||
		      (plane.x <= plane.wantx && plane.xm < 0))
		    {
		      found = -1;
		      
		      for (k = 0; k < MAX_MISSILES && found == -1; k++)
			{
			  if (missiles[k].alive == 0)
			    found = k;
			}
		      
		      if (found != -1)
			{
			  addmissile(found, 1,
				     plane.x, plane.y, plane.z, -1, -1);
			  plane.hasdropped = 1;
			}
		    }
		}
	    }
	  else
	    {
	      /* Randomly add a plane: */
	      
	      if (gameover == 0 && showingletters == 0 &&
		  bonus_mode == BONUS_OFF)
		{
		  if (randnum(chance_of_plane) == 0)
		    {
		      /* Start from left or right, moving inwards; */
		      
		      plane.alive = 1;
		      plane.hasdropped = 0;
		      
		      plane.x = -X_WIDTH;
		      plane.xm = randnum(5) + 3;
		      
		      if (randnum(10) < 5)
			{
			  plane.x = -plane.x;
			  plane.xm = -plane.xm;
			}
		      
		      plane.x = plane.x + (X_WIDTH / 2);
		      
		      
		      /* Pick a random spot at which to drop bombs: */
		      
		      plane.wantx = randnum(X_WIDTH);
		      
		      
		      /* Pick a random direction in the other two planes
			 (no pun intended): */
		      
		      plane.y = randnum(Y_HEIGHT / 2);
		      plane.ym = (randnum(10) - 5) / 2;
		      
		      plane.z = randnum(Z_DEPTH);
		      plane.zm = (randnum(10) - 5) / 10;
		    }
		}
	    }
	  
	  
	  /* All missiles gone?  No more coming?  Handle bonus!
	     Then, go to next level or see if game is over yet: */
	  
	  if (how_many_coming == 0 && num_missiles == 0 &&
	      num_explosions == 0 && num_bullets == 0 && gameover == 0)
	    {
	      if (bonus_mode == BONUS_OFF)
		{
		  bonus_mode++;
		}
	      else if (bonus_mode == BONUS_BULLETS)
		{
		  any = 0;
		  
		  for (i = 0; i < MAX_DEFENSEBASES && any == 0; i++)
		    {
		      if (defensebases[i].bullets > 0)
			{
			  any = 1;
			  
			  if (defensebases[i].bullets > 5)
			    {
			      defensebases[i].bullets =
				defensebases[i].bullets - 5;
			      score = score + 5;
			      defensebases[i].bonusing = 1;
			    }
			  else
			    {
			      score = score + defensebases[i].bullets;
			      defensebases[i].bullets = 0;
			      defensebases[i].bonusing = 0;
			    }
			}
		      else
			defensebases[i].bonusing = 0;
		    }
		  
		  if (any == 0)
		    bonus_mode++;
		}
	      else if (bonus_mode == BONUS_CITIES)
		{
		  any = 0;
		  
		  for (i = 0; i < MAX_CITIES && any == 0; i++)
		    {
		      if (cities[i].bonused == 0 && cities[i].alive != 0)
			{
			  any = 1;
			  
			  cities[i].bonused = 1;
			  
			  score = score + 100;
			}
		    }
		  
		  if (any == 0)
		    bonus_mode++;
		}
	      else if (bonus_mode == BONUS_DONE)
		{
		  /* See if we add a new city: */
		  
		  if (score / NEW_CITY_SCORE > last_score / NEW_CITY_SCORE &&
		      num_cities < MAX_CITIES)
		    {
		      /* Find a dead spot to bring back to life: */
		      
		      do
			{
			  i = randnum(MAX_CITIES);
			}
		      while (cities[i].alive == 1);
		      
		      
		      /* Reincarnate it: */
		      
		      cities[i].alive = 1;
		      cities[i].percent = 0;
		      cities[i].bonused = 0;
		      
		      num_cities++; 
		    }
		  
		  
		  if (num_cities == 0)
		    {
		      /* No more cities!?  Later! */
		      
		      if (gameover == 0)
			{
			  gameover = 1;
			  
			  flashtime = 30;
			  flashcolors = 1;
			  
			  turnonletters(LANG_GAMEOVER, 0, 0);
			}
		    }
		  else
		    {
		      /* Still some cities?  Next level! */
		      
		      endoflevel();
		    }
		}
	    }
	  
	  
	  last_num_missiles = num_missiles;
	  
	  num_missiles = 0;
	}
      
      
      /* Turn off missile view if the missile is gone! */
      
      if (missile_view == 1)
	{
	  if (missiles[missile_view_missile].alive == 0)
	    missile_view = 0;
	}
      
      
      /* Turn off bullet view if the bullet is gone! */
      
      /* if (bullet_view == 1)
	{
	  if (bullets[bullet_view_bullet].alive == 0)
	    bullet_view = 0;
	} */
      
      
      /* Clear window: */
      
      if ((fastdraw == 0 || ttoggle == 0) &&
	  (halfframe == 0 || drawlinetoggle == 0))
	{
	  if (flashtime <= 0)
	    {
	      XFillRectangle(display, backbuffer, blackgc, 0, 0,
			     WIDTH, HEIGHT);
	    }
	  else
	    {
	      if (ttoggle)
		{
		  if (flashcolors == 0)
		    XFillRectangle(display, backbuffer, whitegc, 0, 0,
				   WIDTH, HEIGHT);
		  else
		    XFillRectangle(display, backbuffer,
				   colorgcs[randnum(NUM_OBJECTS)],
				   0, 0, WIDTH, HEIGHT);
		}
	      else
		{
		  XFillRectangle(display, backbuffer, blackgc, 0, 0,
				 WIDTH, HEIGHT);
		}
	    }
	  
	  if (flashtime > 0)
	    flashtime--;
      
	  /* Draw horizon: */
	  
	  if (missile_view == 0 && flashtime <= 0)
	    {
	      if (calc3d(&x1, &y1, 0, 0, 50000))
		{
		  XFillRectangle(display, backbuffer, colorgcs[OBJ_SKY],
				 0, 0, WIDTH, y1);

		  XFillRectangle(display, backbuffer,
				 colorgcs[OBJ_GROUND_HORZ],
				 0, y1, WIDTH, HEIGHT - y1);
		}
	    }
	}
      
	  
      /* Draw ground: */
      
      drawline3d(0, Y_HEIGHT, 0,
		 X_WIDTH, Y_HEIGHT, 0, colorgcs[OBJ_GROUND]);
      
      drawline3d(X_WIDTH, Y_HEIGHT, 0,
		 X_WIDTH, Y_HEIGHT, Z_DEPTH, colorgcs[OBJ_GROUND]);
      
      drawline3d(X_WIDTH, Y_HEIGHT, Z_DEPTH,
		 0, Y_HEIGHT, Z_DEPTH, colorgcs[OBJ_GROUND]);
      
      drawline3d(0, Y_HEIGHT, Z_DEPTH,
		 0, Y_HEIGHT, 0, colorgcs[OBJ_GROUND]);
      
      
      /* Draw cities: */
      
      num_cities = 0;
      
      for (i = 0; i < MAX_CITIES; i++)
	{
	  if (cities[i].alive == 1)
	    {
	      num_cities++;
	      
	      x1 = cities[i].x - ((X_WIDTH / 12) * cities[i].percent);
	      y1 = Y_HEIGHT;
	      z1 = cities[i].z - ((Z_DEPTH / 12) * cities[i].percent);
	      
	      x2 = cities[i].x + ((X_WIDTH / 12) * cities[i].percent);
	      y2 = Y_HEIGHT;
	      z2 = cities[i].z + ((Z_DEPTH / 12) * cities[i].percent);
	      
	      x3 = cities[i].x;
	      y3 = Y_HEIGHT;
	      z3 = cities[i].z;
	      
	      drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_CITY]);
	      drawline3d(x2, y1, z1, x2, y1, z2, colorgcs[OBJ_CITY]);
	      drawline3d(x2, y1, z2, x1, y1, z2, colorgcs[OBJ_CITY]);
	      drawline3d(x1, y1, z2, x1, y1, z1, colorgcs[OBJ_CITY]);
	      
	      if (simple_cities == 0)
		{
		  /* Nice looking, complicated cities: */
		  
		  if (cities[i].bonused == 0)
		    y3 = Y_HEIGHT - cities[i].height[0] * cities[i].percent;
		  else
		    y3 = Y_HEIGHT;
		  
		  drawline3d(x1, y1, z3, x2, y1, z3, colorgcs[OBJ_CITY]);
		  drawline3d(x3, y1, z1, x3, y1, z2, colorgcs[OBJ_CITY]);
		  
		  if (cities[i].shape[0] == 0)
		    {
		      drawline3d(x1, y3, z1, x3, y3, z1, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z1, x3, y3, z3, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z3, x1, y3, z3, colorgcs[OBJ_CITY]);
		      drawline3d(x1, y3, z3, x1, y3, z1, colorgcs[OBJ_CITY]);
		      
		      drawline3d(x1, y3, z1, x1, y1, z1, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z1, x3, y1, z1, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z3, x3, y1, z3, colorgcs[OBJ_CITY]);
		      drawline3d(x1, y3, z3, x1, y1, z3, colorgcs[OBJ_CITY]);
		    }
		  else
		    {
		      drawline3d(x1, y1, z1,
				 (x1 + x3) / 2, y3, (z1 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		      drawline3d(x3, y1, z1,
				 (x1 + x3) / 2, y3, (z1 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		      drawline3d(x3, y1, z3,
				 (x1 + x3) / 2, y3, (z1 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		      drawline3d(x1, y1, z3,
				 (x1 + x3) / 2, y3, (z1 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		    }

		  if (cities[i].bonused == 0)
		    y3 = Y_HEIGHT - cities[i].height[1] * cities[i].percent;
		  else
		    y3 = Y_HEIGHT;
	      
		  if (cities[i].shape[1] == 0)
		    {
		      drawline3d(x2, y3, z1, x3, y3, z1, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z1, x3, y3, z3, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z3, x2, y3, z3, colorgcs[OBJ_CITY]);
		      drawline3d(x2, y3, z3, x2, y3, z1, colorgcs[OBJ_CITY]);
		      
		      drawline3d(x2, y3, z1, x2, y1, z1, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z1, x3, y1, z1, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z3, x3, y1, z3, colorgcs[OBJ_CITY]);
		      drawline3d(x2, y3, z3, x2, y1, z3, colorgcs[OBJ_CITY]);
		    }
		  else
		    {
		      drawline3d(x2, y1, z1,
				 (x2 + x3) / 2, y3, (z1 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		      drawline3d(x3, y1, z1,
				 (x2 + x3) / 2, y3, (z1 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		      drawline3d(x3, y1, z3,
				 (x2 + x3) / 2, y3, (z1 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		      drawline3d(x2, y1, z3,
				 (x2 + x3) / 2, y3, (z1 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		    }
		  
		  if (cities[i].bonused == 0)
		    y3 = Y_HEIGHT - cities[i].height[2] * cities[i].percent;
		  else
		    y3 = Y_HEIGHT;
		  
		  if (cities[i].shape[2] == 0)
		    {
		      drawline3d(x1, y3, z2, x3, y3, z2, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z2, x3, y3, z3, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z3, x1, y3, z3, colorgcs[OBJ_CITY]);
		      drawline3d(x1, y3, z3, x1, y3, z2, colorgcs[OBJ_CITY]);
		      
		      drawline3d(x1, y3, z2, x1, y1, z2, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z2, x3, y1, z2, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z3, x3, y1, z3, colorgcs[OBJ_CITY]);
		      drawline3d(x1, y3, z3, x1, y1, z3, colorgcs[OBJ_CITY]);
		    }
		  else
		    {
		      drawline3d(x1, y1, z2,
				 (x1 + x3) / 2, y3, (z2 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		      drawline3d(x3, y1, z2,
				 (x1 + x3) / 2, y3, (z2 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		      drawline3d(x3, y1, z3,
				 (x1 + x3) / 2, y3, (z2 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		      drawline3d(x1, y1, z3,
				 (x1 + x3) / 2, y3, (z2 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		    }
		  
		  if (cities[i].bonused == 0)
		    y3 = Y_HEIGHT - cities[i].height[3] * cities[i].percent;
		  else
		    y3 = Y_HEIGHT;
		  
		  if (cities[i].shape[3] == 0)
		    {
		      drawline3d(x2, y3, z2, x3, y3, z2, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z2, x3, y3, z3, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z3, x2, y3, z3, colorgcs[OBJ_CITY]);
		      drawline3d(x2, y3, z3, x2, y3, z2, colorgcs[OBJ_CITY]);
		      
		      drawline3d(x2, y3, z2, x2, y1, z2, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z2, x3, y1, z2, colorgcs[OBJ_CITY]);
		      drawline3d(x3, y3, z3, x3, y1, z3, colorgcs[OBJ_CITY]);
		      drawline3d(x2, y3, z3, x2, y1, z3, colorgcs[OBJ_CITY]);
		    }
		  else
		    {
		      drawline3d(x2, y1, z2,
				 (x2 + x3) / 2, y3, (z2 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		      drawline3d(x3, y1, z2,
				 (x2 + x3) / 2, y3, (z2 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		      drawline3d(x3, y1, z3,
				 (x2 + x3) / 2, y3, (z2 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		      drawline3d(x2, y1, z3,
				 (x2 + x3) / 2, y3, (z2 + z3) / 2,
				 colorgcs[OBJ_CITY]);
		    }
		}
	      else
		{
		  /* Simple-looking cities: */
		  	
		  if (cities[i].bonused == 0)
		    y3 = Y_HEIGHT - 40 * cities[i].percent;
		  else
		    y3 = Y_HEIGHT;
		  
		  drawline3d(x1, y1, z1, x3, y3, z3, colorgcs[OBJ_CITY]);
		  drawline3d(x2, y1, z1, x3, y3, z3, colorgcs[OBJ_CITY]);
		  drawline3d(x2, y1, z2, x3, y3, z3, colorgcs[OBJ_CITY]);
		  drawline3d(x1, y1, z2, x3, y3, z3, colorgcs[OBJ_CITY]);
		}
	    }
	}
      
      
      /* Draw defense bases: */
      
      for (i = 0; i < MAX_DEFENSEBASES; i++)
	{
	  if (defensebases[i].alive == 1)
	    {
	      j = 16;
	      
	      if (defensebases[i].bonusing == 1)
		j = 8;
	      
	      x1 = defensebases[i].x - (X_WIDTH / j);
	      y1 = Y_HEIGHT;
	      z1 = defensebases[i].z - (Z_DEPTH / j);
	      
	      x2 = defensebases[i].x + (X_WIDTH / j);
	      y2 = Y_HEIGHT;
	      z2 = defensebases[i].z + (Z_DEPTH / j);
	      
	      x3 = defensebases[i].x;
	      y3 = Y_HEIGHT - 30;
	      z3 = defensebases[i].z;
	      
	      if (defensebases[i].bonusing == 1)
		y3 = Y_HEIGHT - 60;
	      
	      drawline3d(x1, y1, z1, x2, y1, z1,
			 colorgcs[OBJ_DEFENSEBASE]);
	      drawline3d(x2, y1, z1, x2, y1, z2,
			 colorgcs[OBJ_DEFENSEBASE]);
	      drawline3d(x2, y1, z2, x1, y1, z2,
			 colorgcs[OBJ_DEFENSEBASE]);
	      drawline3d(x1, y1, z2, x1, y1, z1,
			 colorgcs[OBJ_DEFENSEBASE]);
	      
	      
	      if (defensebases[i].bullets > 0)
		{
		  /* Shape for active defense bases: */
		  
		  drawline3d(x1, y3, z1, x3, y1, z1,
			     colorgcs[OBJ_DEFENSEBASE]);
		  drawline3d(x2, y3, z1, x3, y1, z1,
			     colorgcs[OBJ_DEFENSEBASE]);
		  
		  drawline3d(x1, y3, z2, x3, y1, z2,
			     colorgcs[OBJ_DEFENSEBASE]);
		  drawline3d(x2, y3, z2, x3, y1, z2,
			     colorgcs[OBJ_DEFENSEBASE]);
		  
		  drawline3d(x1, y3, z1, x1, y1, z3,
			     colorgcs[OBJ_DEFENSEBASE]);
		  drawline3d(x1, y3, z2, x1, y1, z3,
			     colorgcs[OBJ_DEFENSEBASE]);
		  
		  drawline3d(x2, y3, z1, x2, y1, z3,
			     colorgcs[OBJ_DEFENSEBASE]);
		  drawline3d(x2, y3, z2, x2, y1, z3,
			     colorgcs[OBJ_DEFENSEBASE]);
		  
		  drawline3d(x1, y3, z1, x3, y1, z3,
			     colorgcs[OBJ_DEFENSEBASE]);
		  drawline3d(x2, y3, z1, x3, y1, z3,
			     colorgcs[OBJ_DEFENSEBASE]);
		  drawline3d(x2, y3, z2, x3, y1, z3,
			     colorgcs[OBJ_DEFENSEBASE]);
		  drawline3d(x1, y3, z2, x3, y1, z3,
			     colorgcs[OBJ_DEFENSEBASE]);
		  
		  drawline3d(x1, y3, z1, x1, y1, z1,
			     colorgcs[OBJ_DEFENSEBASE]);
		  drawline3d(x2, y3, z1, x2, y1, z1,
			     colorgcs[OBJ_DEFENSEBASE]);
		  drawline3d(x2, y3, z2, x2, y1, z2,
			     colorgcs[OBJ_DEFENSEBASE]);
		  drawline3d(x1, y3, z2, x1, y1, z2,
			     colorgcs[OBJ_DEFENSEBASE]);
		}
	      else
		{
		  /* Shape for empty defense bases: */
		  
		  drawline3d(x1, y1, z1, x3, y3, z3,
			     colorgcs[OBJ_DEFENSEBASE]);
		  drawline3d(x2, y1, z1, x3, y3, z3,
			     colorgcs[OBJ_DEFENSEBASE]);
		  drawline3d(x2, y1, z2, x3, y3, z3,
			     colorgcs[OBJ_DEFENSEBASE]);
		  drawline3d(x1, y1, z2, x3, y3, z3,
			     colorgcs[OBJ_DEFENSEBASE]);
		}
	    }
	}

      
      /* Draw plane: */
      
      if (plane.alive == 1)
	{
	  x1 = plane.x + sign(plane.xm) * 20;
	  y1 = plane.y - 10 - plane.ym * 2;
	  z1 = plane.z - 10 - plane.zm * 2;
	  
	  x3 = plane.x;
	  y3 = plane.y;
	  z3 = plane.z;
	  
	  x2 = plane.x - sign(plane.xm) * 20;
	  y2 = plane.y + 10 + plane.ym * 2;
	  z2 = plane.z + 10 + plane.zm * 2;
	  
	  drawline3d(x1, y3 + plane.ym, z3, x2, y3 - plane.ym, z3,
		     colorgcs[OBJ_PLANE]);
	  
	  drawline3d(x1, y3 + plane.ym, z3, x2, y3 - plane.ym, z2,
		     colorgcs[OBJ_PLANE]);
	  drawline3d(x1, y3 + plane.ym, z3, x2, y3 - plane.ym, z1,
		     colorgcs[OBJ_PLANE]);
	  drawline3d(x2, y3 - plane.ym, z2, x2, y3 - plane.ym, z1,
		     colorgcs[OBJ_PLANE]);
	  
	  drawline3d(x2, y3 - plane.ym, z3, x2, y1 + plane.ym, z3,
		     colorgcs[OBJ_PLANE]);
	  drawline3d(x1, y3 + plane.ym, z3, x2, y1 + plane.ym, z3,
		     colorgcs[OBJ_PLANE]);
	  
	  if (show_targets == 1 ||
	      (player.x >= plane.x - 50 &&
	       player.x <= plane.x + 50 &&
	       player.z >= plane.z - 50 &&
	       player.z <= plane.z + 50))
	    {
	      drawline3d(plane.x, Y_HEIGHT, plane.z,
			 plane.x, plane.y, plane.z,
			 colorgcs[OBJ_MISSILE_TARGETTER]);
	    }
	}
      
      
      /* Draw pointer: */
      
      if (gameover == 0)
	{
	  /* White (normal) or red (near a plane or missile): */
	  
	  which_color = OBJ_POINTER;
	  
	  if (plane.alive == 1 &&
	      player.x >= plane.x - 20 &&
	      player.x <= plane.x + 20 &&
	      player.y >= plane.y - 20 &&
	      player.y <= plane.y + 20 &&
	      player.z >= plane.z - 20 &&
	      player.z <= plane.z + 20)
	    {
	      which_color = OBJ_POINTER_EXCITED;
	    }
	  
	  for (i = 0; i < MAX_MISSILES && which_color != OBJ_POINTER_EXCITED;
	       i++)
	    {
	      if (missiles[i].alive == 1 &&
		  player.x >= missiles[i].x - 20 &&
		  player.x <= missiles[i].x + 20 &&
		  player.y >= missiles[i].y - 20 &&
		  player.y <= missiles[i].y + 20 &&
		  player.z >= missiles[i].z - 20 &&
		  player.z <= missiles[i].z + 20)
		{
		  which_color = OBJ_POINTER_EXCITED;
		}
	    }
	  
	  
	  drawline3d(player.x - 10, player.y, player.z,
		     player.x + 10, player.y, player.z, colorgcs[which_color]);
	  
	  drawline3d(player.x, player.y - 10, player.z,
		     player.x, player.y + 10, player.z, colorgcs[which_color]);
	  
	  drawline3d(player.x, player.y, player.z - 10,
		     player.x, player.y, player.z + 10, colorgcs[which_color]);
	  
	  
	  /* Draw pointer "shadow": */
	  
	  drawline3d(player.x, Y_HEIGHT, player.z - 10,
		     player.x, Y_HEIGHT, player.z + 10,
		     colorgcs[OBJ_POINTER_SHADOW]);
	  
	  drawline3d(player.x - 10, Y_HEIGHT, player.z,
		     player.x + 10, Y_HEIGHT, player.z,
		     colorgcs[OBJ_POINTER_SHADOW]);
	}
      
      
      /* Draw missiles: */
      
      for (i = 0; i < MAX_MISSILES; i++)
	{ 
	  if (missiles[i].alive == 1)
	    {
	      drawline3d(missiles[i].x, missiles[i].y, missiles[i].z,
			 missiles[i].x - missiles[i].xm,
			 missiles[i].y - missiles[i].ym,
			 missiles[i].z - missiles[i].zm,
			 colorgcs[OBJ_MISSILE]);
	      
	      if (show_targets == 1 ||
		  (player.x >= missiles[i].x - 50 &&
		   player.x <= missiles[i].x + 50 &&
		   player.z >= missiles[i].z - 50 &&
		   player.z <= missiles[i].z + 50))
		{
		  drawline3d(missiles[i].x, Y_HEIGHT, missiles[i].z,
			     missiles[i].x, missiles[i].y, missiles[i].z,
			     colorgcs[OBJ_MISSILE_TARGETTER]);
		}
	    }
	}
      
      
      /* Draw bullets: */
      
      for (i = 0; i < MAX_BULLETS; i++)
	{
	  if (bullets[i].alive == 1)
	    {
	      /* Draw destination target: */
	      
	      drawline3d(bullets[i].destx - 5,
			 bullets[i].desty - 5,
			 bullets[i].destz,
			 bullets[i].destx + 5,
			 bullets[i].desty + 5,
			 bullets[i].destz, colorgcs[OBJ_BULLET_DEST]);

	      drawline3d(bullets[i].destx + 5,
			 bullets[i].desty - 5,
			 bullets[i].destz,
			 bullets[i].destx - 5,
			 bullets[i].desty + 5,
			 bullets[i].destz, colorgcs[OBJ_BULLET_DEST]);
	      
	      
	      /* Draw "plasma"-y bullet: */
	      
	      drawline3d(bullets[i].x - randnum(5),
			 bullets[i].y - randnum(5),
			 bullets[i].z - randnum(5),
			 bullets[i].x + randnum(5),
			 bullets[i].y - randnum(5),
			 bullets[i].z - randnum(5),
			 colorgcs[OBJ_BULLET]);
	      
	      drawline3d(bullets[i].x - randnum(5),
			 bullets[i].y - randnum(5),
			 bullets[i].z - randnum(5),
			 bullets[i].x - randnum(5),
			 bullets[i].y + randnum(5),
			 bullets[i].z - randnum(5),
			 colorgcs[OBJ_BULLET]);
	      
	      drawline3d(bullets[i].x - randnum(5),
			 bullets[i].y - randnum(5),
			 bullets[i].z - randnum(5),
			 bullets[i].x - randnum(5),
			 bullets[i].y - randnum(5),
			 bullets[i].z + randnum(5),
			 colorgcs[OBJ_BULLET]);
	      
	      drawline3d(bullets[i].x - randnum(5),
			 bullets[i].y + randnum(5),
			 bullets[i].z - randnum(5),
			 bullets[i].x - randnum(5),
			 bullets[i].y - randnum(5),
			 bullets[i].z + randnum(5),
			 colorgcs[OBJ_BULLET]);
	    }
	}
      
      
      /* Draw smoke: */
      
      for (i = 0; i < MAX_SMOKES; i++)
	{
	  if (smokes[i].alive == 1)
	    {
	      if (smokes[i].attachtime >= 0)
		{
		  drawline3d(smokes[i].x, smokes[i].y, smokes[i].z,
			     missiles[smokes[i].owner].x,
			     missiles[smokes[i].owner].y,
			     missiles[smokes[i].owner].z,
			     colorgcs[OBJ_FLAME]);
		  
		  smokes[i].attachtime--;
		}
	      else
		{
		  drawpoint3d(smokes[i].x, smokes[i].y, smokes[i].z,
			     colorgcs[OBJ_SMOKE]);
		}
	      
	      if (ttoggle == 0 && toggle == 0)
		{
		  smokes[i].x = smokes[i].x + (randnum(3) - 1) / 2.0;
		  smokes[i].y = smokes[i].y + (randnum(3) - 1) / 2.0;
		  smokes[i].z = smokes[i].z + (randnum(3) - 1) / 2.0;
		  
		  if (wind == 1 && smokes[i].attachtime <= 0)
		    {
		      smokes[i].x = smokes[i].x + windxm;
		      smokes[i].z = smokes[i].z + windzm;
		    }
		}
	    }
	}
      
      
      /* Handle wind: */
      
      if (wind == 1)
	{
	  windtime--;
	  
	  if (windtime <= 0)
	    {
	      windxm = (randnum(80) - 40) / 10.0;
	      windzm = (randnum(80) - 40) / 10.0;
	      windtime = randnum(100) + 100;
	    }
	}
      
      
      /* Draw debris: */
      
      for (i = 0; i < MAX_DEBRIS; i++)
	{
	  if (debris[i].alive == 1)
	    {
	      drawpoint3d(debris[i].x, debris[i].y, debris[i].z,
			  colorgcs[OBJ_DEBRIS]);
	    }
	}
      
      
      /* Draw city chunks: */
      
      for (i = 0; i < MAX_CITYCHUNKS; i++)
	{
	  if (citychunks[i].alive == 1)
	    {
	      if (citychunks[i].is_defensechunk == CHUNK_IS_CITY)
		drawline3d(citychunks[i].x1, citychunks[i].y1,
			   citychunks[i].z1,
			   citychunks[i].x2, citychunks[i].y2,
			   citychunks[i].z2,
			   colorgcs[OBJ_CITY]);
	      else
		drawline3d(citychunks[i].x1, citychunks[i].y1,
			   citychunks[i].z1,
			   citychunks[i].x2, citychunks[i].y2,
			   citychunks[i].z2,
			   colorgcs[OBJ_DEFENSEBASE]);
	    }
	}
      
      
      /* Draw explosions: */
      
      for (i = 0; i < MAX_EXPLOSIONS; i++)
	{
	  if (explosions[i].alive == 1)
	    {
	      j = explosions[i].size;
	      
	      x1 = explosions[i].x - j;
	      x2 = explosions[i].x + j;
	      x3 = explosions[i].x;
	      
	      y1 = explosions[i].y - j;
	      y2 = explosions[i].y + j;
	      y3 = explosions[i].y;
	      
	      z1 = explosions[i].z - j;
	      z2 = explosions[i].z + j;
	      z3 = explosions[i].z;
	      
	      
	      drawline3d(x1, y3, z1, x2, y3, z1, colorgcs[OBJ_EXPLOSION]);
	      drawline3d(x2, y3, z1, x2, y3, z2, colorgcs[OBJ_EXPLOSION]);
	      drawline3d(x2, y3, z2, x1, y3, z2, colorgcs[OBJ_EXPLOSION]);
	      drawline3d(x1, y3, z2, x1, y3, z1, colorgcs[OBJ_EXPLOSION]);
	      
	      drawline3d(x1, y3, z1, x3, y1, z3, colorgcs[OBJ_EXPLOSION]);
	      drawline3d(x2, y3, z1, x3, y1, z3, colorgcs[OBJ_EXPLOSION]);
	      drawline3d(x2, y3, z2, x3, y1, z3, colorgcs[OBJ_EXPLOSION]);
	      drawline3d(x1, y3, z2, x3, y1, z3, colorgcs[OBJ_EXPLOSION]);
	      
	      drawline3d(x1, y3, z1, x3, y2, z3, colorgcs[OBJ_EXPLOSION]);
	      drawline3d(x2, y3, z1, x3, y2, z3, colorgcs[OBJ_EXPLOSION]);
	      drawline3d(x2, y3, z2, x3, y2, z3, colorgcs[OBJ_EXPLOSION]);
	      drawline3d(x1, y3, z2, x3, y2, z3, colorgcs[OBJ_EXPLOSION]);
	    }	      
	}

      
      /* Handle title effect: */

      if (title_mode == 1)
	{
	  title_timer++;
	  
	  if (title_timer >= 100)
	    {
	      title_mode = 2;
	      
	      for (i = 0; i < MAX_LETTERS; i++)
		{
		  letters[i].destx = X_WIDTH / 2;
		  letters[i].desty = Y_HEIGHT / 2;
		  letters[i].destz = Z_DEPTH / 2;
		}
	    }
	}
      
      
      /* Show letters: */
      
      wave_count = (wave_count + 10) % 360;
      
      for (i = 0; i < MAX_LETTERS; i++)
	{
	  if (letters[i].alive == 1)
	    {
	      x3 = letters[i].x;
	      y3 = letters[i].y;
	      z3 = letters[i].z;
	      
	      
	      /* Do wave effects: */
	      
	      if (letters[i].wave == 1)
		{
		  y3 = y3 + sin(M_PI * (i * 50 + wave_count) / 180) * 5;
		}
	      else if (letters[i].wave == 2)
		{
		  if (letters[i].y < Y_HEIGHT - 20 - 1 ||
		      letters[i].y > Y_HEIGHT - 20 + 1 ||
		      letters[i].ym > 0.125)
		    {
		      letters[i].ym = letters[i].ym + 0.25;
		      
		      if (letters[i].y > Y_HEIGHT - 20)
			{
			  letters[i].y = Y_HEIGHT - 20;
			  letters[i].ym = -letters[i].ym / 4;
			}
		    }
		  else
		    letters[i].ym = 0;
		}
	      
	      
	      /* Do title effects: */
	      
	      if (title_mode == 2)
		{
		  if (letters[i].z > Z_DEPTH / 2)
		    letters[i].zm = letters[i].zm - 0.5;
		  else
		    letters[i].zm = letters[i].zm + 0.5;
		  
		  if (letters[i].y > Y_HEIGHT / 2)
		    letters[i].ym = letters[i].ym - 0.5;
		  else
		    letters[i].ym = letters[i].ym + 0.5;
		  
		  if (toggle == 1)
		    adddebris(letters[i].x, letters[i].y, letters[i].z);
		}
	      
	      
	      x1 = letters[i].x - 5;
	      y1 = y3 - 5;
	      z1 = letters[i].z;
	      
	      x2 = letters[i].x + 5;
	      y2 = y3 + 5;
	      z2 = letters[i].z;
	      
	      
	      c = toupper(letters[i].letter);
	      
	      
	      /* Draw the letter shape: */
	      
	      if (c == 'L')
		{
		  drawline3d(x1, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == '-')
		{
		  drawline3d(x1, y3, z1, x2, y3, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'C')
		{
		  drawline3d(x1, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'B')
		{
		  drawline3d(x1, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x1, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y2, z1, x1, y3, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'I')
		{
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x3, y1, z1, x3, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'E' || c == 'F')
		{
		  drawline3d(x1, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y3, z1, x2, y3, z1, colorgcs[OBJ_LETTER]);
		  if (c == 'E')
		    drawline3d(x1, y2, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'V')
		{
		  drawline3d(x1, y1, z1, x3, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x3, y2, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'G')
		{
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y2, z1, x2, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y3, z1, x3, y3, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'A')
		{
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y3, z1, x2, y3, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'M')
		{
		  drawline3d(x1, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y1, z1, x3, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x3, y3, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'R')
		{
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y3, z1, x1, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y3, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'P')
		{
		  drawline3d(x1, y1, z1, x2, y1, z2, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y3, z2, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y3, z1, x1, y3, z2, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y1, z1, x1, y2, z2, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'U')
		{
		  drawline3d(x1, y1, z1, x1, y2, z2, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x2, y2, z2, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y2, z2, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'D')
		{
		  drawline3d(x1, y1, z1, x1, y2, z2, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y1, z1, x2, y3, z2, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x2, y3, z2, colorgcs[OBJ_LETTER]);
		}
	      else if (c == '0' || c == 'O' || c == 'Q')
		{
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y2, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x1, y1, z1, colorgcs[OBJ_LETTER]);
		  
		  if (c == 'Q')
		    drawline3d(x3, y3, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == '1')
		{
		  drawline3d(x2, y1, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == '2')
		{
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y3, z1, x1, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y3, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == '3')
		{
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y3, z1, x3, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y3, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == '4')
		{
		  drawline3d(x1, y1, z1, x1, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y3, z1, x2, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == '5' || c == 'S')
		{
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y1, z1, x1, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y3, z1, x1, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y3, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == '6')
		{
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y3, z1, x1, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y3, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == '7')
		{
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == '8')
		{
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y2, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x1, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y3, z1, x2, y3, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == '9')
		{
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y2, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y3, z1, x1, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y3, z1, x2, y3, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'H')
		{
		  drawline3d(x1, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y3, z1, x2, y3, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'J')
		{
		  drawline3d(x2, y1, z1, x2, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y3, z1, x3, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x3, y2, z1, x1, y3, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'K')
		{
		  drawline3d(x1, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y3, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y3, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'N')
		{
		  drawline3d(x1, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y1, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'T')
		{	
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x3, y1, z1, x3, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'W')
		{
		  drawline3d(x1, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x3, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y2, z1, x3, y3, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'X')
		{
		  drawline3d(x1, y1, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'Y')
		{
		  drawline3d(x1, y1, z1, x3, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x3, y3, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x3, y3, z1, x3, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      else if (c == 'Z')
		{
		  drawline3d(x1, y1, z1, x2, y1, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x2, y1, z1, x1, y2, z1, colorgcs[OBJ_LETTER]);
		  drawline3d(x1, y2, z1, x2, y2, z1, colorgcs[OBJ_LETTER]);
		}
	      
	      
	      /* Make letter stop moving when it hits its destination: */
	      
	      if (letters[i].x >= letters[i].destx - 1 &&
		  letters[i].x <= letters[i].destx + 1)
		letters[i].xm = 0;
	      
	      if (letters[i].y >= letters[i].desty - 1 &&
		  letters[i].y <= letters[i].desty + 1 && letters[i].wave != 2)
		letters[i].ym = 0;
	      
	      if (letters[i].z >= letters[i].destz - 1 &&
		  letters[i].z <= letters[i].destz + 1)
		letters[i].zm = 0;
	      
	      
	      /* Kill when it hits its destination (if it's supposed to): */
	      
	      if (letters[i].xm == 0 && letters[i].ym == 0 &&
		  letters[i].zm == 0 && letters[i].killatdest == 1)
		letters[i].alive = 0;
	      
	      
	      /* Move letter: */
	      
	      letters[i].x = letters[i].x + letters[i].xm;
	      letters[i].y = letters[i].y + letters[i].ym;
	      letters[i].z = letters[i].z + letters[i].zm;
	    }
	}
      
      
      /* Draw score, level, etc.: */
      
      sprintf(temp, "%s: %d", LANG_LEVEL, level);
      
      drawtext(display, backbuffer, colorgcs[OBJ_LETTER],
	       0, fh, temp);
      
      if (gameover == 1)
	{
	  drawcenteredtext(display, backbuffer, colorgcs[randnum(NUM_OBJECTS)],
			   0, WIDTH, (fh + 1) * 3,
			   temp, font);
	  
	  sprintf(temp, "[%s] %s", KEY_LEVEL_NEXT, LANG_SELECT_NEXT_LEVEL);
	  drawcenteredtext(display, backbuffer, colorgcs[OBJ_LETTER],
			   0, WIDTH, (fh + 1) * 5, temp, font);
	  
	  sprintf(temp, "[%s] %s", KEY_LEVEL_PREV, LANG_SELECT_PREV_LEVEL);
	  drawcenteredtext(display, backbuffer, colorgcs[OBJ_LETTER],
			   0, WIDTH, (fh + 1) * 6, temp, font);
	  
	  sprintf(temp, "[%s] %s, [%s] %s", KEY_FIRE, LANG_BEGIN,
		  KEY_QUIT, LANG_QUIT);
	  drawcenteredtext(display, backbuffer, colorgcs[OBJ_LETTER],
			   0, WIDTH, (fh + 1) * 8, temp, font);
	}
      
      sprintf(temp, "%s: %.5d", LANG_SCORE, score);
      
      drawcenteredtext(display, backbuffer, colorgcs[OBJ_LETTER],
	       0, WIDTH, fh, temp, font);
      
      strcpy(temp, LANG_VIEW);
      
      if (fastdraw == 1)
	strcat(temp, LANG_VIEW_FAST);
      
      if (halfframe == 1)
	strcat(temp, LANG_VIEW_HALF);
      
      if (missile_view == 1)
	strcat(temp, LANG_VIEW_MISSILE);
      
      if (bullet_view == 1)
	strcat(temp, LANG_VIEW_BULLET);
      
      if (strcmp(temp, LANG_VIEW) == 0)
	strcat(temp, LANG_VIEW_NORMAL);
      
      drawtext(display, backbuffer, colorgcs[OBJ_LETTER],
	       0, HEIGHT, temp);
      
      
      /* Draw AICBM Bullet info.: */
      
      for (i = 0; i < MAX_DEFENSEBASES; i++)
	{
	  if (calc3d(&x1, &y1,
		     defensebases[i].x, Y_HEIGHT, defensebases[i].z))
	    {
	      sprintf(temp, "%d", defensebases[i].bullets);
	      
	      drawtext(display, backbuffer, colorgcs[OBJ_LETTER],
		       x1, y1, temp);
	    }
	}
      
      
      /* Flush: */
      
      if (DOUBLE_BUFFER == True)
	{
	  XCopyArea(display, backbuffer, window,
		    whitegc, 0, 0, WIDTH, HEIGHT, 0, 0);
	}
      
      XSync(display, 0);
      
      
      /* Keep framerate exact: */
      
      gettimeofday(&now, NULL);
      
      time_padding = FRAMERATE - ((now.tv_sec - then.tv_sec) * 1000000 +
				  (now.tv_usec - then.tv_usec));
      
      if (time_padding > 0)
	usleep(time_padding);
      else
	got_negative_time_padding = 1;
    }
  while (done == False);
}


/* Program set-up (check usage, load data, etc.): */

void setup(int argc, char * argv[])
{
  FILE * fi;
  char temp[512], color[512], file[1024];
  int i, len, z, zc, cntl, want_length, sound_pos_tmp, which_sound;
  char c;
  struct timeval now, then;
  
  
  /* What level to start out at? (Default is 1, use -L# to set) */
  
  level = 1;
  
  if (argc >= 2)
    {
      if (argv[argc - 1][0] == '-' &&
	  toupper(argv[argc - 1][1]) == 'L' &&
	  isdigit(argv[argc - 1][2]))
	{
	  level = atoi(argv[argc - 1] + 2);
	  
	  if (level < 0)
	    level = 0;
	  else if (level > 99)
	    level = 99;
	  
	  argc--;
	}
    }
  
  if (argc == 2)
    {
      if (strcmp(argv[1], "-version") == 0 ||
	  strcmp(argv[1], "-v") == 0)
	{
	  /* Check for "-version": */
	  
	  printf("\nicbm3d version 0.4\n\n");
	  
	  printf("by Bill Kendrick\n");
	  printf("nbs@sonic.net\n");
	  printf("http://www.sonic.net/~nbs/unix/x/icbm3d/\n\n");
	  exit(0);
	}
      else if (strcmp(argv[1], "-help") == 0 ||
	       strcmp(argv[1], "-h") == 0)
	{
	  /* Check for "-help": */
	  
	  fprintf(stderr, "\n");
	  fprintf(stderr, "Usage: %s [display] | -help | -version | -lyrics\n",
		  argv[0]);
	  fprintf(stderr, "OBJECT:\n");
	  fprintf(stderr, "  Destroy ICBMs before they destroy the cities.\n");
	  fprintf(stderr, "MOVEMENT:\n");
	  fprintf(stderr, "     Left-Click - Move up/down/right/left\n");
	  fprintf(stderr, "   Middle-Click - Move in/out/right/left\n");
	  fprintf(stderr, "    Right-Click - Change perspective\n");
	  fprintf(stderr, "%7s/%7s - Move up/down\n", KEY_UP, KEY_DOWN);
	  fprintf(stderr, "%7s/%7s - Move left/right\n", KEY_LEFT, KEY_RIGHT);
	  fprintf(stderr, "%7s/%7s - Move in/out\n", KEY_AWAY, KEY_TOWARDS);
	  fprintf(stderr, "%15s - Fire Anti-ICBM\n", KEY_FIRE);
	  fprintf(stderr, "GAME CONTROLS:\n");
	  fprintf(stderr, "%15s - Pause/Unpause\n", KEY_PAUSE);
	  fprintf(stderr, "%7s/%7s - Select next/prev. level\n",
		  KEY_LEVEL_NEXT, KEY_LEVEL_PREV);
	  fprintf(stderr, "%15s - Abort game (game over)\n", KEY_ABORT);
	  fprintf(stderr, "%15s - Quit\n", KEY_QUIT);
	  fprintf(stderr, "GRAPHICS CONTROLS:\n");
	  fprintf(stderr, "%7s/%7s - Zoom view in and out\n", KEY_IN, KEY_OUT);
	  fprintf(stderr, "%15s - Toggle missile view on/off\n",
		  KEY_VIEW_MISSILE);
	  fprintf(stderr, "%15s - Choose missile (missile view)\n",
		  KEY_VIEW_MISSILE_SELECT);
	  fprintf(stderr, "%15s - Toggle targetting lines\n",
		  KEY_TOG_TARGET);
	  fprintf(stderr, "%15s - Toggle fastdraw mode\n",
		  KEY_TOG_FASTDRAW);
	  fprintf(stderr, "%15s - Toggle half-frame mode\n",
		  KEY_TOG_HALFFRAME);
	  fprintf(stderr, "%15s - Toggle simple city graphics\n",
		  KEY_TOG_CITY_STYLE);
	  
	  exit(0);
	}
      else if (strcmp(argv[1], "-lyrics") == 0 ||
	       strcmp(argv[1], "-l") == 0)
	{
	  printf("When you attend a funeral,\n");
	  printf("It is sad to think that sooner o'\n");
	  printf("Later those you love will do the same for you.\n");
	  printf("And you may have thought it tragic,\n");
	  printf("Not to mention other adjec-\n");
	  printf("Tives, to think of all the weeping they will do.\n");
	  printf("(But don't you worry.)\n");
	  printf("\n");
	  printf("No more ashes, no more sackcloth,\n");
	  printf("And an arm band made of black cloth\n");
	  printf("Will some day nevermore adorn a sleeve.\n");
	  printf("For if the bomb that drops on you\n");
	  printf("Gets your friends and neighbors too,\n");
	  printf("There'll be nobody left behind to grieve.\n");
	  printf("\n");
	  printf("And we will all go together when we go.\n");
	  printf("What a comforting fact that is to know.\n");
	  printf("Universal bereavement,\n");
	  printf("An inspiring achievement,\n");
	  printf("Yes, we will all go together when we go.\n");
	  printf("\n");
	  printf("We will all go together when we go.\n");
	  printf("All suffused with an incandescent glow.\n");
	  printf("No one will have the endurance\n");
	  printf("To collect on his insurance,\n");
	  printf("Lloyd's of London will be loaded when they go.\n");
	  printf("\n");
	  printf("Oh we will all fry together when we fry.\n");
	  printf("We'll be french fried potatoes by and by.\n");
	  printf("There will be no more misery\n");
	  printf("When the world is our rotisserie,\n");
	  printf("Yes, we will all fry together when we fry.\n");
	  printf("\n");
	  printf("Down by the old maelstrom,\n");
	  printf("There'll be a storm before the calm.\n");
	  printf("\n");
	  printf("And we will all bake together when we bake.\n");
	  printf("There'll be nobody present at the wake.\n");
	  printf("With complete participation\n");
	  printf("In that grand incineration,\n");
	  printf("Nearly three billion hunks of well-done steak.\n");
	  printf("\n");
	  printf("Oh we will all char together when we char.\n");
	  printf("And let there be no moaning of the bar.\n");
	  printf("Just sing out a Te Deum\n");
	  printf("When you see that I.C.B.M.,\n");
	  printf("And the party will be come-as-you-are.\n");
	  printf("\n");
	  printf("Oh, we will all burn together when we burn.\n");
	  printf("There'll be no need to stand and wait your turn.\n");
	  printf("When it's time for the fallout\n");
	  printf("And Saint Peter calls us all out,\n");
	  printf("We'll just drop our agendas and adjourn.\n");
	  printf("\n");
	  printf("You will all go directly to your respective Valhallas.\n");
	  printf("Go directly, do not pass Go, do not collect two hundred dollars.\n");
	  printf("\n");
	  printf("And we will all go together when we go.\n");
	  printf("Every Hottentot and every Eskimo.\n");
	  printf("When the air becomes uranious,\n");
	  printf("We will all go simultaneous.\n");
	  printf("Yes, we all will go together\n");
	  printf("When we all go together,\n");
	  printf("Yes we all will go together when we go.\n");
	  printf("\n");
	  printf("TOM LEHRER\n");
	  
	  exit(0);
	}
    }
  
  if (argc == 1)
    {
      if (getenv("DISPLAY") != NULL)
	{
	  strcpy(server, getenv("DISPLAY"));
	}
      else
	{
	  fprintf(stderr, LANG_ERROR_NODISPLAY);
	  exit(1);
	}
    }
  else if (argc == 2)
    {
      strcpy(server, argv[1]);
    }
  else
    {
      fprintf(stderr, "Usage: %s [display] | -help | -version | -lyrics\n",
	      argv[0]);
      exit(1);
    }
  
  randinit();
}


/* Setup the application: */

void Xsetup()
{
  int i, tempint, ret, max_depth, num_avail_depths, which_max_depth;
  int * avail_depths;
  char title[1024], file[128];
  int status, temp_depth;
  Visual * temp_visual = CopyFromParent;
  FILE * fi;
  XGCValues gcvalues;
  XColor mycolor;
  
  
  /* Connect to display: */
  
  fprintf(stderr, "%s: %s\n", LANG_CONNECTING, server);
  
  display = ConnectToServer(server, &screen, &root);
  if (display == NULL)
    {
      perror(server);
      exit(1);
    }
  
  
  /* Load font: */
  
  font = LoadFont(display, "variable", "fixed");
  fh = FontHeight(font);
  
  
  /* Get our primitve colors: */
  
  black = BlackPixel(display, screen);
  white = WhitePixel(display, screen);
  
  
  /* Open window: */
  
  window = OpenWindow(display, root, 10, 10, WIDTH, HEIGHT,
			   CopyFromParent, black,
			   (KeyPressMask | KeyReleaseMask | ButtonMotionMask |
			    ButtonPressMask | ButtonReleaseMask),
			   (Visual *)CopyFromParent);
  
  if (DOUBLE_BUFFER == True)
    {
      backbuffer = XCreatePixmap(display, root, WIDTH, HEIGHT,
				 DefaultDepthOfScreen(DefaultScreenOfDisplay(display)));
    }
  else
    backbuffer = window;
    
  sprintf(title, "ICBM3D by Bill Kendrick");
  SetStandardHints(display, window, "ICBM3D", title, 10, 10,
		   WIDTH, HEIGHT);
  
  
  /* Set up visual: */
  
  has_color = 0;
  
  if (SetUpVisual(display, screen, &temp_visual, &temp_depth))
    {
      if (!SetUpColormap(display, screen, window, temp_visual,
                         &colormap))
        {
          fprintf(stderr, "Could not create a colormap!\n");
        }
      else
        has_color = 1;
    }
  else
    {
      fprintf(stderr, "Could not find a PseudoColor visual!\n");
    }
  
  
  /* Create Primitive GC's: */
  
  whitegc = CreateGC(display, window, white, black);

  blackgc = CreateGC(display, window, black, black);
  
  
  /* Create object GC's: */
  
  for (i = 0; i < NUM_OBJECTS; i++)
    colorgcs[i] = CreateGC(display, window,
				MyAllocNamedColor(display, colormap,
						  object_colors[i], white,
						  has_color),
			   black);
  
  
  /* Special GC "fixes": */
  
  XSetLineAttributes(display, colorgcs[OBJ_MISSILE_TARGETTER], 1, 
		     LineOnOffDash, CapNotLast, JoinMiter);
  
  
  /* Put us up!: */
  
  XMapWindow(display, window);
  XMapRaised(display, window);
  XFlush(display);
}


/* Allocate a color (or white): */

unsigned long MyAllocNamedColor(Display *display, Colormap colormap,
                                char* colorname, unsigned long default_color,
                                int has_color)
{
  if (has_color == 0)
    return(default_color);
  else
    return(AllocNamedColor(display, colormap, colorname, default_color));
}


/* Init the level stuff: */

void initlevel(void)
{
  char temp[10];
  int i;
  float x1, z1;
  
  
  /* Erase everything: */
  
  for (i = 0; i < MAX_MISSILES; i++)
    missiles[i].alive = 0;
  
  for (i = 0; i < MAX_BULLETS; i++)
    bullets[i].alive = 0;
  
  for (i = 0; i < MAX_EXPLOSIONS; i++)
    explosions[i].alive = 0;
  
  for (i = 0; i < MAX_SMOKES; i++)
    smokes[i].alive = 0;

  for (i = 0; i < MAX_DEBRIS; i++)
    debris[i].alive = 0;
  
  for (i = 0; i < MAX_DEBRISERS; i++)
    debrisers[i].alive = 0;
  
  for (i = 0; i < MAX_CITYCHUNKS; i++)
    citychunks[i].alive = 0;
  
  plane.alive = 0;
  
  
  /* Set up letters to introduce the level: */
  
  sprintf(temp, "%s %d", LANG_LEVEL, level);
  
  turnonletters(temp, 0, 0);
  
  
  /* Turn off stuff: */
  
  paused = 0;
  gameover = 0;
  flashtime = 0;
  num_missiles = 0;
  last_num_missiles = 0;
  how_many_coming = level * 1;
  bonus_mode = BONUS_OFF;
  
  
  /* Determine this level's statistics: */
  
  chance_of_split = (100 - level);
  
  if (level % 2 == 0)
    chance_of_spiral = 10;
  else
    chance_of_spiral = 10000;
  
  if (level > 10)
    chance_of_plane = 100;
  else
    chance_of_plane = 10000;
  
  if (level > 50)
    {
      wind = 1;
      windxm = (randnum(80) - 40) / 10.0;
      windzm = (randnum(80) - 40) / 10.0;
      windtime = randnum(100) + 100;
    }
  else
    wind = 0;
  
  
  /* Record our score: */
  
  last_score = score;
  
  
  /* Init defense bases: */
  
  x1 = 0;
  z1 = 0;
  
  for (i = 0; i < MAX_DEFENSEBASES; i++)
    {
      defensebases[i].alive = 1;
      defensebases[i].bonusing = 0;
      
      defensebases[i].x = x1;
      defensebases[i].z = z1;
      
      defensebases[i].bullets = 99;
      
      if (x1 == 0)
	{
	  x1 = X_WIDTH;
	}
      else
	{
	  x1 = 0;
	  z1 = Z_DEPTH;
	}
    }
  
  
  /* Turn of "bonused"'s on cities: */
  
  for (i = 0; i < MAX_CITIES; i++)
    cities[i].bonused = 0;
}


/* Draw a line: */

void drawline3d(float x1, float y1, float z1, float x2, float y2, float z2,
		GC color)
{
  float sx1, sy1, sx2, sy2;
  
  
  if (fastdraw == 1)
    drawlinetoggle = 1 - drawlinetoggle;
  
  if ((fastdraw == 0 && halfframe == 0) || drawlinetoggle == 0)
    {
      if (calc3d(&sx1, &sy1, x1, y1, z1) &&
	  calc3d(&sx2, &sy2, x2, y2, z2))
	{
	  /* Draw the line into the window: */
	  
	  XDrawLine(display, backbuffer, color, sx1, sy1, sx2, sy2);
	}
    }
}


/* Draw point: */

void drawpoint3d(float x, float y, float z, GC color)
{
  float sx, sy;
  
  
  if (fastdraw == 1)
    drawlinetoggle = 1 - drawlinetoggle;
  
  if ((fastdraw == 0 && halfframe == 0) || drawlinetoggle == 0)
    {
      if (calc3d(&sx, &sy, x, y, z))
	{
	  /* Draw the point into the window: */
	  
	  XDrawPoint(display, backbuffer, color, sx, sy);
	}
    }
}


/* Add an explosion: */

void addexplosion(float x, float y, float z, int whose)
{
  int i, found;
  
  
  found = -1;
  
  for (i = 0; i < MAX_EXPLOSIONS; i++)
    {
      if (explosions[i].alive == 0)
	found = i;
    }
  
  if (found != -1)
    {
      explosions[found].alive = 1;
      explosions[found].x = x;
      explosions[found].y = y;
      explosions[found].z = z;
      explosions[found].size = 1;
      explosions[found].sizem = 1;
      explosions[found].users = whose;
    }
  
  for (i = 0; i < MAX_CITIES; i++)
    {
      if (cities[i].alive == 1)
	{
	  if (x + 10 >= cities[i].x - X_WIDTH / 12 &&
	      x - 10 <= cities[i].x + X_WIDTH / 12 &&
	      y + 10 >= Y_HEIGHT - 10 &&
	      z + 10 >= cities[i].z - Z_DEPTH / 12 &&
	      z - 10 <= cities[i].z + Z_DEPTH / 12)
	    {
	      killcity(i);
	    }
	}
    }


  for (i = 0; i < MAX_DEFENSEBASES; i++)
    {
      if (defensebases[i].alive == 1)
	{
	  if (x + 10 >= defensebases[i].x - X_WIDTH / 12 &&
	      x - 10 <= defensebases[i].x + X_WIDTH / 12 &&
	      y + 10 >= Y_HEIGHT - 10 &&
	      z + 10 >= defensebases[i].z - Z_DEPTH / 12 &&
	      z - 10 <= defensebases[i].z + Z_DEPTH / 12)
	    {
	      killdefensebase(i);
	    }
	}
    }
}


/* Add a smoke: */

void addsmoke(float x, float y, float z, int owner)
{
  int i, found;
  
  
  found = -1;
  
  for (i = 0; i < MAX_SMOKES && found == -1; i++)
    {
      if (smokes[i].alive == 0)
	found = i;
    }
  
  if (found != -1)
    {
      smokes[found].alive = 1;
      smokes[found].x = x;
      smokes[found].y = y;
      smokes[found].z = z;
      smokes[found].owner = owner;
      smokes[found].attachtime = randnum(5) + 20;
    }
}


/* Add a missile: */

void addmissile(int which, int cansplit, float x, float y, float z,
		int angle, int whichangle)
{
  int take_time;
  float destx, destz;
  
  
  missiles[which].alive = 1;
  
  if (x == -1)
    {
      missiles[which].x = randnum(X_WIDTH);
      missiles[which].y = START_HEIGHT;
      missiles[which].z = randnum(Z_DEPTH);
    }
  else
    {
      missiles[which].x = x;
      missiles[which].y = y;
      missiles[which].z = z;
    }
  
  
  /* Pick a spot to aim for: */
  
  missiles[which].destx = randnum(X_WIDTH);
  missiles[which].destz = randnum(Z_DEPTH);
  
  
  /* Pick a random amount of time to take to get there: */
  
  take_time = randnum(150) + 300 - level * 2;
  
  
  /* Aim for it: */
  
  missiles[which].xm = (missiles[which].destx - missiles[which].x) / take_time;
  missiles[which].ym = (Y_HEIGHT - missiles[which].y) / take_time;
  missiles[which].zm = (missiles[which].destz - missiles[which].z) / take_time;
  
  
  /* Does this missile split? */
  
  if (randnum(chance_of_split) < 1 && cansplit == 1)
    missiles[which].splitter = 1;
  else
    missiles[which].splitter = 0;


  /* Does this missile spliral? */
  
  if (randnum(chance_of_spiral) < 1)
    missiles[which].spiraller = 1;
  else
    missiles[which].spiraller = 0;
}


/* Add a bullet: */

void addbullet(float x, float y, float z)
{
  int i, found, whichbase;
  float distance, this_dist;
  
  
  whichbase = -1;
  distance = 100000.0;
  
  for (i = 0; i < MAX_DEFENSEBASES; i++)
    {
      if (defensebases[i].alive == 1 && defensebases[i].bullets > 0)
	{
	  this_dist = sqrt(((defensebases[i].x - x) *
			    (defensebases[i].x - x)) +
			   ((defensebases[i].z - z) *
			    (defensebases[i].z - z)));
	  
	  if (this_dist < distance)
	    {
	      distance = this_dist;
	      whichbase = i;
	    }
	}
    }
  
  
  if (whichbase != -1)
    {
      found = -1;
      
      for (i = 0; i < MAX_BULLETS && found == -1; i++)
	{
	  if (bullets[i].alive == 0)
	    found = i;
	}
      
      if (found != -1)
	{
	  bullets[found].alive = 1;
	  
	  
	  /* Set it's starting position and destination: */
	  
	  bullets[found].x = defensebases[whichbase].x;
	  bullets[found].y = Y_HEIGHT;
	  bullets[found].z = defensebases[whichbase].z;
	  
	  bullets[found].destx = x;
	  bullets[found].desty = y;
	  bullets[found].destz = z;
	  
	  
	  /* Determine it's speed and direction: */
	  
	  distance = cbrt(((bullets[found].x - bullets[found].destx) *
			   (bullets[found].x - bullets[found].destx)) +
			  ((bullets[found].y - bullets[found].desty) *
			   (bullets[found].y - bullets[found].desty)) +
			  ((bullets[found].z - bullets[found].destz) *
			   (bullets[found].z - bullets[found].destz))) / 5;
	  
	  bullets[found].xm = (bullets[found].destx - bullets[found].x) /
	    distance;
	  bullets[found].ym = (bullets[found].desty - bullets[found].y) /
	    distance;
	  bullets[found].zm = (bullets[found].destz - bullets[found].z) /
	    distance;
	  
	  
	  /* Eat one of the base's bullets: */
	  
	  defensebases[whichbase].bullets--;
	}
    }
}


/* Kill a missile (and it's smoke): */

void removemissile(int which, int cause_explosion)
{
  int i;
  
  
  missiles[which].alive = 0;
  
  for (i = 0; i < MAX_SMOKES; i++)
    {
      if (smokes[i].owner == which && smokes[i].alive == 1)
	{
	  smokes[i].alive = 0;
	}
    }
  
  if (cause_explosion == 1)
    addexplosion(missiles[which].x, missiles[which].y, missiles[which].z, 0);
}


/* Recalculate rotation trig values: */

void recalculatetrig()
{
  cos_anglex = cos(M_PI * anglex / 180.0);
  sin_anglex = sin(M_PI * anglex / 180.0);

  cos_angley = cos(M_PI * angley / 180.0);
  sin_angley = sin(M_PI * angley / 180.0);
}


/* Move pointer; keep it in bounds: */

void domove(void)
{
  if (paused == 0)
    {
      player.x = player.x + x_change;
      if (player.x < 0)
	player.x = 0;
      else if (player.x >= X_WIDTH)
	player.x = X_WIDTH - 1;
      
      player.y = player.y + y_change;
      if (player.y < -100)
	player.y = -100;
      else if (player.y > Y_HEIGHT)
	player.y = Y_HEIGHT - 1;
      
      player.z = player.z + z_change;
      if (player.z < 0)
	player.z = 0;
      else if (player.z > Z_DEPTH)
	player.z = Z_DEPTH - 1;
    }
}


/* Destroy a city: */

void killcity(int which)
{
  int i;
  
  
  cities[which].alive = 0;
  
  for (i = 0; i < 8; i++)
    addcitychunks(cities[which].x, Y_HEIGHT, cities[which].z, CHUNK_IS_CITY);
  
  adddebriser(cities[which].x, Y_HEIGHT, cities[which].z);
  
  flashtime = 6;
  flashcolors = 0;
}


/* Destroy a city: */

void killdefensebase(int which)
{
  int i;
  
  
  defensebases[which].alive = 0;
  defensebases[which].bullets = 0;
  
  for (i = 0; i < 10; i++)
    addcitychunks(defensebases[which].x, Y_HEIGHT, defensebases[which].z,
		  CHUNK_IS_DEFENSEBASE);
  
  adddebriser(defensebases[which].x, Y_HEIGHT, defensebases[which].z);
  
  flashtime = 6;
  flashcolors = 1;
}


/* Add some debris: */

void adddebris(float x, float y, float z)
{
  int i, found;
  
  found = -1;
  
  for (i = 0; i < MAX_DEBRIS; i++)
    {
      if (debris[i].alive == 0)
	found = i;
    }
  
  if (found != -1)
    {
      debris[found].alive = 1;
      
      debris[found].x = x;
      debris[found].y = y;
      debris[found].z = z;
      
      debris[found].xm = randnum(5) - 2;
      debris[found].ym = -randnum(5) - 5;
      debris[found].zm = randnum(5) - 2;
      
      debris[found].time = 20 + randnum(10);
    }
}


/* Add a debrisers: */

void adddebriser(float x, float y, float z)
{
  int i, found;
  
  found = -1;
  
  for (i = 0; i < MAX_DEBRISERS; i++)
    {
      if (debrisers[i].alive == 0)
	found = i;
    }
  
  if (found != -1)
    {
      debrisers[found].alive = 1;
      
      debrisers[found].x = x;
      debrisers[found].y = y;
      debrisers[found].z = z;
      
      debrisers[found].time = 500;
    }
}


/* Add a chunk of city flying around: */

void addcitychunks(float x, float y, float z, int what)
{
  int i, found;
  
  found = -1;
  
  for (i = 0; i < MAX_CITYCHUNKS; i++)
    {
      if (citychunks[i].alive == 0)
	found = i;
    }
  
  if (found != -1)
    {
      citychunks[found].alive = 1;
      citychunks[found].is_defensechunk = what;
      
      citychunks[found].x1 = x + randnum(10) - 5;
      citychunks[found].y1 = y;
      citychunks[found].z1 = z + randnum(10) - 5;
      
      citychunks[found].xm1 = randnum(6) - 3;
      citychunks[found].ym1 = -randnum(3) - 10;
      citychunks[found].zm1 = randnum(6) - 3;
      
      citychunks[found].x2 = x + randnum(10) - 5;
      citychunks[found].y2 = y - 20;
      citychunks[found].z2 = z + randnum(10) - 5;
      
      citychunks[found].xm2 = randnum(6) - 3;
      citychunks[found].ym2 = -randnum(3) - 10;
      citychunks[found].zm2 = randnum(6) - 3;
    }
}



/* Turns on letter graphics: */

void turnonletters(char * str, int wavey, float wantz)
{
  int i, r;
  
  showingletters = 1;
  
  for (i = 0; i < MAX_LETTERS; i++)
    letters[i].alive = 0;
  
  for (i = 0; i < strlen(str); i++)
    {
      letters[i].alive = 1;
      letters[i].letter = str[i];
      
      letters[i].x = randnum(X_WIDTH);
      letters[i].y = randnum(Y_HEIGHT);
      letters[i].z = Z_DEPTH * 2 + randnum(50);
      
      letters[i].destx = (X_WIDTH / 2) + (i - 3) * 20;
      letters[i].desty = Y_HEIGHT / 2;
      letters[i].destz = wantz;
      
      r = randnum(5);
      
      letters[i].xm = (letters[i].destx - letters[i].x) / (15 + i * 2 + r);
      letters[i].ym = (letters[i].desty - letters[i].y) / (15 + i * 2 + r);
      letters[i].zm = (letters[i].destz - letters[i].z) / (15 + i * 2 + r);
      
      letters[i].killatdest = 0;
      letters[i].wave = wavey;
    }
}


/* Make letters fly away: */

void turnoffletters(void)
{
  int i;
  
  for (i = 0; i < MAX_LETTERS; i++)
    {
      letters[i].destz = -200;
      letters[i].zm = (letters[i].destz - letters[i].z) / 20;
      letters[i].killatdest = 1;
    }
  
  showingletters = 0;
}


/* Init game values: */

void initgame(void)
{
  int i, j;
  float x1, z1;
  
  
  /* Init cities : */
  
  x1 = X_WIDTH / 8;
  z1 = Z_DEPTH / 8;
  
  for (i = 0; i < MAX_CITIES; i++)
    {
      cities[i].alive = 1;
      cities[i].percent = 0;
      cities[i].bonused = 0;
      
      cities[i].x = x1;
      cities[i].z = z1;
      
      for (j = 0; j < 4; j++)
	{
	  cities[i].height[j] = randnum(40) + 10;
	  cities[i].shape[j] = randnum(2);
	}
      
      x1 = x1 + (X_WIDTH / 4);
      if (x1 >= X_WIDTH)
	{
	  x1 = X_WIDTH / 8;
	  z1 = z1 + (Z_DEPTH / 4);
	}
    }

  
  /* Init. score: */
  
  score = 0;
}


/* End of level sequence: */

void endoflevel(void)
{
  level++;
  initlevel();
}


/* Convert 3D to screen (2D): */

int calc3d(float * sx, float * sy, float x, float y, float z)
{
  float xx, yy, zz;
  float cos_x, cos_y, sin_x, sin_y, hyp;
  int ok;
  
  
  ok = 0;
  
  /* Calc. x,y,z based on your viewpoint: */
  
  if (missile_view == 1)
    {
      /* Missile view: */
      
      x = x - missiles[missile_view_missile].x -
	missiles[missile_view_missile].xm;
      y = y - missiles[missile_view_missile].y - DISTANCE + 80;
      z = z - missiles[missile_view_missile].z -
	missiles[missile_view_missile].zm;
      
      xx = x;
      yy = -z;
      zz = y;
    }
  else if (bullet_view == 1)
    {
      /* Bullet view: */
      
      x = x - bullets[bullet_view_bullet].x -
	bullets[bullet_view_bullet].xm * 2;
      y = y - bullets[bullet_view_bullet].y -
	bullets[bullet_view_bullet].ym * 2;
      z = z - bullets[bullet_view_bullet].z -
	bullets[bullet_view_bullet].zm * 2;
      
      hyp = sqrt((bullets[bullet_view_bullet].ym *
		  bullets[bullet_view_bullet].ym) +
		 (bullets[bullet_view_bullet].xm *
		  bullets[bullet_view_bullet].xm));
      
      cos_x = bullets[bullet_view_bullet].xm / hyp;
      sin_x = bullets[bullet_view_bullet].zm / hyp;
      
      cos_y = bullets[bullet_view_bullet].xm / hyp;
      sin_y = bullets[bullet_view_bullet].ym / hyp;
      
      xx = x * cos_x - z * sin_x;
      zz = x * sin_x + z * cos_x;
      
      yy = y * cos_y - zz * sin_y;
      zz = y * sin_y + zz * cos_y;
      
      zz = zz - DISTANCE;
    }
  else
    {
      /* Normal (freeview) view: */
      
      x = x - X_WIDTH / 2;
      y = y - Y_HEIGHT / 2;
      z = z - Z_DEPTH / 2;
      
      xx = x * cos_anglex - z * sin_anglex;
      zz = x * sin_anglex + z * cos_anglex;
      
      yy = y * cos_angley - zz * sin_angley;
      zz = y * sin_angley + zz * cos_angley;
    }
  
  
  zz = zz - zoom;
  
  if (zz > -DISTANCE)
    {
      /* Convert (x,y,z) into (x,y) with a 3D look: */
      
      *sx = xx / ((zz + DISTANCE) / ASPECT);
      *sy = yy / ((zz + DISTANCE) / ASPECT);
      
      
      /* Transpose (0,0) origin to the center of the window: */
      
      *sx = *sx + WIDTH / 2;
      *sy = *sy + HEIGHT / 2;
      
      ok = 1;
    }
  
  return(ok);
}


float sign(float v)
{
  if (v < 0.0)
    return(-1.0);
  else if (v > 0.0)
    return(1.0);
  else
    return(0.0);
}
