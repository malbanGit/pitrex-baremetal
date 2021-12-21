/*         REBEL DEFENDER V1.20 - V.Portet 2015         /
/          http://vincent95.users.sourceforge.net/     */

/* Releases:
/  - V0.50 - Fully functional but without sound				    /
/  - V1.00 - Sound added						    /
/  - V1.10 - Game speed increased and compliance improvement (warnings)     /
/  - V1.20 - Audio mishandling correction (causing OSS emulation problems  */

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include <fcntl.h>
#include <linux/soundcard.h>

/* Colors */

#define MAX_COLORS 256
#define BLACK 0
#define RED 63
#define GREEN 127
#define BLUE 191
#define YELLOW 192
#define WHITE 255
#define TRUE 1
#define FALSE 0

/* Timers */

#define TIME_T  time_t
#define TIMEVAL struct timeval
#define CLOCK_T clock_t

/* Vectorized 3D */

#define	perspective	200		/* perspective factor */
const	signed	int zviewmax = 5000;	/* Highest z coord where the object is considered as viewable */

/* Window size */

#define WIN_WIDTH  640
#define WIN_HEIGHT 480
const	int	dispheight = WIN_HEIGHT - 80;
const	int	dispwidth = WIN_WIDTH;
const	int	midy = (WIN_HEIGHT - 80) / 2;
const	int	midx = WIN_WIDTH / 2;
#define	radarheight	70
#define	radarwidth	70
#define radarmidw	35
#define	radarmidh	35

/*================================================================*/

/* Key image variables */

unsigned char keyup = 0;
unsigned char keydown = 0;
unsigned char keyleft = 0;
unsigned char keyright = 0;
unsigned char keyspace = 0;
unsigned char keyescape = 0;
unsigned char keydet;


/*================================================================*/

/* Vectorized 3D variables and constants */

/* Existing Object types */
#define	tiefighter_nl		312
#define	xwingfighter_nl		252
#define	badbullet_nl		24
#define	mybullet_nl		48
#define	explosion1_nl		96
#define	explosion2_nl		48
#define	type_tiefighter		0
#define	type_xwingfighter	1
#define	type_badbullet		2
#define	type_mybullet		3
#define	type_explosion1		4
#define	type_explosion2		5
#define type_none		6

/* In-game sounds */
#define	xplode			0x01
#define	fire			0x02
#define	tfsound			0x03
#define	nosnd			0x04
#define fragments		4096

signed int rpol;                      /* polar coords in x-z plane when defining a 3D object refering to its center */
signed int polang;
signed int ydim;                      /* Y axis dimension */

/* Up to 40 3D objects existing in the game. All this Coords are absolute */
signed int	rpolobj[40];	/* Object-to-player radius in the x-z plane */
signed int	polangobj[40];  /* Object-to-player angle in the x-z plane */
signed int	yobj[40];	/* Object position on the y axis. */
signed int	angobj[40];     /* Orientation of the object (pivoting around the Y axis) */
signed int	dyobj[40];	/* Current dy/dt (depending on object type) */
unsigned char	defobj[40];     /* Type of object affected to the object ID */
unsigned char	maxindex;	/* Maximum index used for currently existing object set */
unsigned char	objindex;	/* Index adressing the currently processed object */
unsigned char	skindex;	/* sub-index used to scan within the loop using objindex */
unsigned char	lkindex;	/* Index used to seek for a free position in case of maxindex = 39 */
signed   int	angle;          /* Player orientation */
signed int	dangle;		/* angle / dt */
signed int	relx;		/* Player relative cartesian coords */
signed int	relz;
unsigned char	nlines[6];      /* Number of 3D lines building the object selected by objtype */
signed int typdata[312][6];     /* Object definition data array */
const signed int  tiefighter[tiefighter_nl] = {-20,20,-40,-20,20,40,20,20,-40,20,20,40,-20,-20,-40,-20,-20,40,20,-20,-40,20,-20,40,
			-20,20,-40,20,20,-40,-20,-20,-40,20,-20,-40,-20,20,-40,-20,-20,-40,20,-20,-40,20,20,-40,
			-20,20,40,20,20,40,-20,-20,40,20,-20,40,-20,20,40,-20,-20,40,20,-20,40,20,20,40,
			-20,20,-40,0,0,-50,20,20,-40,0,0,-50,20,-20,-40,0,0,-50,-20,-20,-40,0,0,-50,
			-20,20,40,0,0,50,20,20,40,0,0,50,20,-20,40,0,0,50,-20,-20,40,0,0,50,
			20,10,-10,20,10,10,20,-10,-10,20,-10,10,20,10,-10,20,-10,-10,20,10,10,20,-10,10,
			-20,10,-10,-20,10,10,-20,-10,-10,-20,-10,10,-20,10,-10,-20,-10,-10,-20,10,10,-20,-10,10,
			20,10,-10,50,50,-50,20,-10,-10,50,-50,-50,20,10,10,50,50,50,20,-10,10,50,-50,50,
			-20,10,-10,-50,50,-50,-20,-10,-10,-50,-50,-50,-20,10,10,-50,50,50,-20,-10,10,-50,-50,50,
			50,-50,-50,50,50,-50,50,-50,50,50,50,50,50,50,-50,50,50,50,50,-50,-50,50,-50,50,
			-50,-50,-50,-50,50,-50,-50,-50,50,-50,50,50,-50,50,-50,-50,50,50,-50,-50,-50,-50,-50,50,
			50,-50,-50,70,0,0,50,-50,50,70,0,0,50,50,-50,70,0,0,50,50,50,70,0,0,
			-50,-50,-50,-70,0,0,-50,-50,50,-70,0,0,-50,50,-50,-70,0,0,-50,50,50,-70,0,0};

const signed int  xwingfighter[xwingfighter_nl] = {-10,-10,-70,10,-10,-70,-10,-10,-70,-10,-10,50,10,-10,-70,10,-10,50,-10,-10,50,10,-10,50,
			-10,-10,-70,-10,20,50,10,-10,-70,10,20,50,-10,-10,50,-10,20,50,10,-10,50,10,20,50,
			-10,20,50,10,20,50,0,0,-40,0,10,0,
			10,0,0,40,0,0,10,0,30,40,0,30,10,0,0,10,0,30,40,0,0,40,0,30,
			40,5,-10,50,5,-10,40,-5,-10,50,-5,-10,40,5,-10,40,-5,-10,50,5,-10,50,-5,-10,
			40,5,40,50,5,40,40,-5,40,50,-5,40,40,5,40,40,-5,40,50,5,40,50,-5,40,
			40,5,-10,40,5,40,50,5,-10,50,5,40,40,-5,-10,40,-5,40,50,-5,-10,50,-5,40,
			-10,0,0,-40,0,0,-10,0,30,-40,0,30,-10,0,0,-10,0,30,-40,0,0,-40,0,30,
			-40,5,-10,-50,5,-10,-40,-5,-10,-50,-5,-10,-40,5,-10,-40,-5,-10,-50,5,-10,-50,-5,-10,
			-40,5,40,-50,5,40,-40,-5,40,-50,-5,40,-40,5,40,-40,-5,40,-50,5,40,-50,-5,40,
			-40,5,-10,-40,5,40,-50,5,-10,-50,5,40,-40,-5,-10,-40,-5,40,-50,-5,-10,-50,-5,40};

const signed int  badbullet[badbullet_nl] = {-5,15,0,5,15,0,-5,-15,0,5,-15,0,-15,-5,0,-15,5,0,15,-5,0,15,5,0};

const signed int  mybullet[mybullet_nl] = {-5,5,0,5,5,0,5,5,0,5,-5,0,5,-5,0,-5,-5,0,-5,-5,0,-5,5,0,
			-5,5,0,0,15,40,5,5,0,0,15,40,5,-5,0,0,15,40,-5,-5,0,0,15,40};

const signed int  explosion1[explosion1_nl] = {0,0,0,2,0,0,10,4,2,10,6,2,-20,10,5,-18,10,5,5,20,30,5,20,32,
			-30,-40,15,-30,-38,15,20,18,-25,20,18,-23,-40,-30,10,-40,-30,12,40,-40,20,42,-40,20,
			0,35,0,2,37,0,20,4,2,20,6,2,-20,10,45,-18,10,45,5,35,30,5,35,32,-30,-35,25,-30,-38,25,
			20,-18,-25,20,-18,-23,64,-30,10,64,-30,12,40,-10,20,42,-10,20};

const signed int  explosion2[explosion2_nl] = {0,5,0,2,5,0,15,4,2,15,6,2,-20,10,15,-18,10,15,5,25,30,5,25,32,
			-35,-40,15,-35,-38,15,20,28,-25,20,28,-23,-40,-30,5,-40,-30,7,40,-45,20,42,-45,20};





/*================================================================*/

/********* X11 routine variables ***********/

/* window structs */
Display     *display;
int         screen_num;
Window      win;
Screen      *screen_ptr;
XFontStruct *font_info;

/* gc's & color data */
GC             black_gc, color_gc, text_gc;
XGCValues      gc_values;
unsigned long  gc_valuemask = 0;
XColor         color_info;
unsigned int   color_table [MAX_COLORS];
unsigned short color_data [MAX_COLORS][3];

/* window buffers */
Pixmap       double_buffer;

/* misc window info */
char          *display_name = NULL;
char          *window_name = "Rebel Defender 1.1";
XTextProperty wname;
char          *font_name = "10x20";
unsigned int  window_width, window_height;

/* Mouse polling */
int	mousex;
int	mousey;
int	selmousex;
int	selmousey;
char	mouseb;

/***** X11 routine variables - END ******/

/* Timing related variables */

typedef struct TIMERSTRUCT TIMER;
struct TIMERSTRUCT
	{
	time_t  init_time_stamp;
	struct timeval t0;
	struct timeval t1;
	};

TIMER	gtimer, *gt;
long	reftim;

/* Other variables */

/* 1 - General purpose */

signed int     u;
signed int	i1;
signed int	i2;
signed int	i3;
unsigned char	tmpch;
char	tmpchar[2]={0,0};

/* 2 - Application related */

/* Sound */
unsigned char	sndon;
unsigned char pendingsound;
unsigned char reqsound;
unsigned long sampointer;
unsigned char   *sampledata[3];
unsigned char   *sampbuffer;
unsigned long	samplen[3];
int 		handle;
FILE    *fp;
unsigned char nfiles;

/* Game flow and display */
int	lifebarlevel;
int	lifebarx;
int	lifebary;
int	scorelevel;
int	hiscore = 2000;
int	scorex;
int	scorey;
unsigned char	gamelevel;
unsigned char	shootcount;
unsigned char	ntie;
int		interbullet;
signed char	tick;
unsigned char	recoverdelay;
char		gameover;
/* Fast trigo */
const float	trigotable[37] = {0,0.09,0.17,0.26,0.34,0.42,0.5,0.57,0.64,0.71,0.77,0.82,0.87,0.91,0.94,0.96,0.98,0.99,1,0.99,0.98,0.96,0.94,0.91,0.87,0.82,0.77,0.71,0.64,0.57,0.5,0.42,0.34,0.26,0.17,0.09,0};
const float	tgtable[19] = {0,0.09,0.18,0.27,0.36,0.47,0.58,0.70,0.84,1,1.19,1.43,1.73,2.14,2.75,3.73,5.67,11.43,60};
/* Titles */
const char	descripta[147] = {'Y','o','u','r',' ','r','e','b','e','l',' ','f','o','r','t','r','e','s','s',13,
				'i','s',' ','s','u','r','r','o','u','n','d','e','d',' ','b','y',' ','i','m','p','e','r','i','a','l',13,
				't','i','e',' ','f','i','g','h','t','e','r','s','.',' ','Y','o','u','r',' ','o','n','l','y',13,
				'c','h','a','n','c','e',' ','f','o','r',' ','s','u','r','v','i','v','a','l',' ','i','s',' ','t','o',13,
				'd','e','f','e','n','d',' ','t','h','e',' ','p','l','a','c','e',' ','b','y',' ','u','s','i','n','g',13,
				'y','o','u','r',' ','r','o','t','a','t','i','v','e',' ','g','u','n',' ','t','o','w','e','r','.',0};
const char	descriptb[102] = {'A','v','o','i','d',' ','s','h','o','o','t','i','n','g',' ','o','n',' ','t','h','e',' ','r','e','b','e','l',13,
				'f','i','g','h','t','e','r','s',' ','w','h','i','c','h',' ','a','r','e',13,
				'g','i','v','i','n','g',' ','y','o','u',' ','a','s','s','i','s','t','a','n','c','e',13,
				'o','t','h','e','r','w','i','s','e',' ','y','o','u',' ','w','i','l','l',' ','l','o','o','s','e',13,
				'p','o','i','n','t','s','.',0};


/* Functions prototypes */

unsigned char Scanall  ( void );
int  Graphics_init     ( unsigned int, unsigned int );
void Graphics_shutdown ( void );
void Draw_line         ( int, int, int, int, unsigned int );
void Set_3d_color      ( unsigned int );
void Line_3d           ( int, int, int, int, int, int );
void Draw_point        ( int, int, unsigned int );
void Draw_rectangle    ( int, int, int, int, unsigned int );
void Draw_text         ( char *, int, int, unsigned int );
void Dispnum           ( int, int, int, int, unsigned int );
int  Update_display    ( void );
void Load_objects      ( void );
void Place_obj         ( unsigned char, int, int, int, int, unsigned int );
char Look_free         ( void );
void Polconv           (int, int, int );
double  Timer_sec   ( TIMER * );
long    Timer_msec  ( TIMER * );
float  fastsin         (int);
float  fastcos         (int);
int    fastatn         (float);
void snd_init          (void);
extern float sqrtf (float);

int main ( int argc, char **argv )
	{
	if (!Graphics_init (WIN_WIDTH , WIN_HEIGHT))
		{
		printf("X11 Server could not be accessed !\n");
		exit(0);
		}
	gt = &gtimer;	/* This is the timer pointer */

	/* Load 3D object data */
	Load_objects();

	/* Starting Intro loop part 1 */
	keyspace = 0;
	keyescape = 0;
	/* Prepare a tiefighter as rotating figure for intro screen... */
	defobj[0] = type_tiefighter;
	angobj[0] = 0;
	Draw_text ( " REBEL  DEFENDER", dispwidth/3, 30, WHITE );
	Draw_text ( " Press Space to start, press Esc to quit...", dispwidth/4, dispheight+20, WHITE );
	i1=0;
	i2=(dispwidth/3)+10;
	i3=i2;
	u=150;
	tmpchar[0]=1;
	while ((keyspace == 0)&&(keyescape == 0))
		{
		reftim = Timer_msec(gt) + 60;
		while (Timer_msec(gt) < reftim) {}
		Update_display();
		if (tmpchar[0]!=0)
			{
			tmpchar[0] = descripta[i1];
			if (tmpchar[0]==13)
				{
				i3=i2;
				u += 15;
				}
			else
				{
				if (tmpchar[0]!=0)
					{
					Draw_text (tmpchar,i3,u,YELLOW);
					i3 += 15;
					}
				}
			i1 += 1;
			}
		Draw_rectangle (0,0,dispwidth/3,dispheight,BLACK); /* Pre-erase the screen part to create an updated picture */
		keydet = Scanall();  /* Read keyboard and mouse (update mousex, mousey, and s.o...) */
		Place_obj ( defobj[0], -150, dispheight/2, 300, angobj[0], GREEN );
		angobj[0] += 10;
		if (angobj[0] > 360)
			{
			angobj[0] = 10;
			}
		}
	if (keyescape != 0)
		{
		Graphics_shutdown ();  /* Close X11 window and session */
		exit(0);
		}

	/* Starting Intro loop part 2 */
	keyspace = 0;
	keyescape = 0;
	/* Prepare an xwing fighter as rotating figure for intro screen... */
	defobj[0] = type_xwingfighter;
	angobj[0] = 0;
	Draw_rectangle (dispwidth/3,80,dispwidth,300,BLACK); /* Erase Previous text */
	i1=0;
	i2=(dispwidth/3)+10;
	i3=i2;
	u=150;
	tmpchar[0]=1;
	while ((keyspace == 0)&&(keyescape == 0))
		{
		reftim = Timer_msec(gt) + 60;
		while (Timer_msec(gt) < reftim) {}
		Update_display();
		if (tmpchar[0]!=0)
			{
			tmpchar[0] = descriptb[i1];
			if (tmpchar[0]==13)
				{
				i3=i2;
				u += 15;
				}
			else
				{
				if (tmpchar[0]!=0)
					{
					Draw_text (tmpchar,i3,u,YELLOW);
					i3 += 15;
					}
				}
			i1 += 1;
			}
		Draw_rectangle (0,0,dispwidth/3,dispheight,BLACK); /* Pre-erase the screen part to create an updated picture */
		keydet = Scanall();  /* Read keyboard and mouse (update mousex, mousey, and s.o...) */
		Place_obj ( defobj[0], -150, dispheight/2, 300, angobj[0], GREEN );
		angobj[0] += 10;
		if (angobj[0] > 360)
			{
			angobj[0] = 10;
			}
		}
	if (keyescape != 0)
		{
		Graphics_shutdown ();  /* Close X11 window and session */
		exit(0);
		}
restart:
	/* Display control info */
	Draw_rectangle (0,80,dispwidth,dispheight,BLACK); /* Erase Previous text */
	Place_obj ( type_tiefighter, -150, dispheight/3, 300, 30, GREEN );
	Draw_text ( " CONTROLS", dispwidth/3, 90, RED );
	Draw_text ( "Left / Right = Move rotative tower", dispwidth/3, 140, BLUE );
	Draw_text ( "Mouse = Gun positioning", dispwidth/3, 160, BLUE );
	Draw_text ( "Mouse L-Button or Space = Fire / Start", dispwidth/3, 180, BLUE );
	Draw_text ( "Up and Down = Sound enable :", dispwidth/3, 200, BLUE );
	Draw_text ( "Esc = Quit", dispwidth/3, 220, BLUE );
	Draw_text ("ON", dispwidth/3 + 290, 200, YELLOW );
	Update_display();
	keyspace = 0;
	keyescape = 0;
	keyup = 0;
	keydown = 0;
	sndon = 1;
	while ((keyspace == 0)&&(keyescape == 0))
		{
		keydet = Scanall();  /* Read keyboard and mouse (update mousex, mousey, and s.o...) */
		if ((keyup==1)||(keydown==1))
			{
			sndon = 1 - sndon;
			Draw_rectangle (dispwidth/3 + 290, 185, dispwidth/3 + 320, 205, BLACK);
			if (sndon == 1)
				{
				Draw_text ("ON", dispwidth/3 + 290, 200, YELLOW );
				}
			else
				{
				Draw_text ("OFF", dispwidth/3 + 290, 200, YELLOW );
				}
			reftim = Timer_msec(gt) + 200;
			while (Timer_msec(gt) < reftim) {}
			}
		Update_display();
		reftim = Timer_msec(gt) + 20;
		while (Timer_msec(gt) < reftim) {}
		}
	if (keyescape != 0)
		{
		Graphics_shutdown ();  /* Close X11 window and session */
		exit(0);
		}

	/* Define starting conditions and draw frame */
	Draw_rectangle (0,0,WIN_WIDTH,WIN_HEIGHT, BLACK);
	Draw_line (0,dispheight,WIN_WIDTH-1,dispheight, YELLOW);
	Draw_line (0,WIN_HEIGHT-1,WIN_WIDTH-1,WIN_HEIGHT-1, YELLOW);
	Draw_line (0,dispheight,0,WIN_HEIGHT-1, YELLOW);
	Draw_line (WIN_WIDTH-1,dispheight,WIN_WIDTH-1,WIN_HEIGHT-1, YELLOW);

	lifebarlevel = 0;
	lifebarx = 2*dispwidth/3 + 50;
	lifebary = (WIN_HEIGHT + dispheight)/2;
	Draw_text ("DAMMAGE",lifebarx - 80, lifebary + 15, WHITE);
	Draw_rectangle (lifebarx, lifebary, lifebarx + 100, lifebary + 20, GREEN);
	scorelevel = 0;
	scorex = dispwidth/4;
	scorey = lifebary + 15;
	Draw_text ("SCORE : 000000", scorex - 80, scorey, WHITE);
	Draw_text ("LEVEL 1", midx - 40, scorey, RED);
	angle = 0;      /* Initial player view */
	gamelevel = 1;
	shootcount = 0;
	gameover = 0;
	/* Prepare immediately some enemy spaceships */
	maxindex = 2;   /* Start with 3 tiefighters */
	yobj[0] = 1000;
	yobj[1] = 0;
	yobj[2] = -1000;
	polangobj[0] = -45;
	polangobj[1] = 0;
	polangobj[2] = 45;
	for (objindex = 0; objindex != maxindex + 1; objindex++)
		{
		defobj[objindex] = type_tiefighter;
		rpolobj[objindex] = zviewmax - (100*(objindex+1));
		angobj[objindex] = polangobj[objindex];
		dyobj[objindex] = 0;
		}
	ntie = 3;
	recoverdelay = 0;
	tick = 0;
	/* Soundcard init and sound files loading */
	if (sndon==1)
		{
		snd_init();
		sampointer = 0;
		}
	reqsound = 0;
	pendingsound = nosnd;
	/* Reset firing speed limitation */
	interbullet = Timer_msec(gt);
	reftim = Timer_msec(gt) + 30;

	/* Game main loop */
	while ((keyescape == 0)&&(gameover==0))
		{
		keydet = Scanall();  /* Read keyboard and mouse (update mousex, mousey, and s.o...) */
		/* Periodical screen clear */
		Draw_rectangle (0,0,dispwidth,dispheight,BLACK); /* Pre-erase the screen to create an updated picture */
		if (sndon==1)
			/* Sound handling */
			{
			if (reqsound != 0)
				{
				if (reqsound <= pendingsound)
					{
					sampointer = 0;
					pendingsound = reqsound;
					}
				reqsound = 0;
				}
			if (pendingsound == nosnd)
				{
				/* Continue supplying datastream to soundcard, but zeroes */
				write(handle,&sampbuffer[0],fragments);
				}
			else
				{
				if ((sampointer + fragments) <= samplen[pendingsound-1])
					{
					/* Send currently pending sound fragment */
					write(handle,&sampledata[pendingsound-1][sampointer],fragments);
					sampointer += fragments;
					}
				else
					{
					/* Sound ends */
					pendingsound = nosnd;
					sampointer = 0;
					/* Continue supplying datastream to soundcard, but zeroes */
					write(handle,&sampbuffer[0],fragments);
					}
				}
			}
		else
			{
			/* Delay instead of sound handling */
			while (Timer_msec(gt) < reftim) {}
			}
		reftim = Timer_msec(gt) + 12;
		/* Frame counter */
		tick += 1;
		/* Graphics and game flow */
		/* Rotating the tower */
		dangle = 0;
		if (keyright==1)
			{
			dangle = 4;
			angle += dangle;
			if (angle>180)
				{
				angle -= 360;
				dangle = -356;
				}
			}
		if (keyleft==1)
			{
			dangle = -4;
			angle += dangle;
			if (angle<-179)
				{
				angle += 360;
				dangle = 356;
				}
			}
		/* Object table cleaning */
		while ((defobj[maxindex]==type_none)&&(maxindex!=0))
			{
			maxindex -= 1;
			}
		/* Fire */
		if ((keyspace==1)||(mouseb==1))
			{
			/* Create a new bullet if enough time elapsed after previous one */
			if ((reftim > interbullet)&&(maxindex < 38))
				{
				reqsound = fire;
				interbullet = reftim + 300;
				objindex = Look_free();
				defobj[objindex] = type_mybullet;
				rpolobj[objindex] = 60;
				angobj[objindex] = fastatn(((float) (mousex-midx))/(2*rpolobj[objindex]));
				polangobj[objindex] = angobj[objindex]+angle;
				if (polangobj[objindex]>180)
					{
					polangobj[objindex] -= 360;
					}
				else
					{
					if (polangobj[objindex]<-179)
						{
						polangobj[objindex] += 360;
						}
					}
				yobj[objindex] = dispheight-40;
				dyobj[objindex] = (mousey-100-midy)/13;
				/* Player smart assistance for firing angle adjustment */
				for (skindex=0;skindex<=maxindex;skindex++)
					{
					if (defobj[skindex]==type_tiefighter)
						{
						if (abs(rpolobj[skindex])>1500)
							{
							i1 = polangobj[skindex];
							i2 = 7;
							/* Rear firing */
							if (rpolobj[skindex]<0)
								{
								if (i1<=0)
									{
									i1 += 180;
									}
								else
									{
									i1 -= 180;
									}
								i2 = 10;
								}
							if ((abs(i1 - polangobj[objindex])<i2)&&(abs(angle - i1)<80))
								{
								polangobj[objindex] = i1;
								skindex = maxindex; /* shorten current loop */
								}
							}
						}
					}
				}
			}
		/* Rest of the game process */
		/* Level evolution */
		if (shootcount > (gamelevel * 7))
			{
			shootcount = 0;
			gamelevel += 1;
			if (gamelevel != 10)
				{
				tmpchar[0] = 0x30 + gamelevel;
				tmpchar[1] = 0;
				Draw_rectangle (midx + 20, scorey - 15, midx + 50, scorey + 5, BLACK);
				Draw_text (tmpchar, midx + 20, scorey, RED);
				if (gamelevel & 0x01)
					{
					/* Add a new xwingfighter every 2 level step */
					objindex = Look_free();
					defobj[objindex] = type_xwingfighter;
					rpolobj[objindex] = zviewmax - ((signed int) (gamelevel)) * 200;
					polangobj[objindex] = angle - 90 - (((signed int) objindex)*2);
					angobj[objindex] = polangobj[objindex] - 90;
					yobj[objindex] = ((rpolobj[objindex])>>3);
					dyobj[objindex] = -5;
					}
				}
			else
				{
				gameover = 1;
				}
			}
		/* Radar display */
		Draw_line ( 5, dispheight-radarheight, radarwidth, dispheight-radarheight, GREEN );
		Draw_line ( 5, dispheight-5, radarwidth, dispheight-5, GREEN );
		Draw_line ( 5, dispheight-radarheight, 5, dispheight-5, GREEN );
		Draw_line ( radarwidth, dispheight-radarheight, radarwidth, dispheight-5, GREEN );
		Draw_line ( 5, dispheight-radarheight, radarwidth, dispheight-5, GREEN );
		Draw_line ( 5, dispheight-5, radarwidth, dispheight-radarheight, GREEN );
		/* Enemy regeneration */
		if (recoverdelay < 100)
			{
			recoverdelay += 1;
			}
		else
			{
			if (ntie < (gamelevel+4))
				{
				/* Create a new tie-fighter */
				objindex = Look_free();
				defobj[objindex] = type_tiefighter;
				rpolobj[objindex] = zviewmax;
				yobj[objindex] = tick*4 - 512;
				polangobj[objindex] = (signed int) (objindex*3) + tick;
				angobj[objindex] = polangobj[objindex] - angle;
				dyobj[objindex] = 0;
				ntie += 1;
				recoverdelay = 0;
				}
			}
		/* Object A.I */
		for (objindex=0; objindex<=maxindex; objindex++)
			{
			/* Process currently pending objects */
			angobj[objindex] -= dangle;
			switch (defobj[objindex])
				{
				case type_none :
				/* This is not a currently pending object */
				break;

				case type_tiefighter :
				rpolobj[objindex] -= 10;
				u = polangobj[objindex]-angle; /* Preset viewing angle */
				if (abs(rpolobj[objindex])<1500)
					{
					yobj[objindex] -= 15;
					if (dyobj[objindex]==0) /* Particular use of dyobj for this object */
						{
						dyobj[objindex]=1;
						/* Launch a bullet */
						skindex = Look_free();
						defobj[skindex] = type_badbullet;
						angobj[skindex] = angobj[objindex];
						polangobj[skindex] = polangobj[objindex];
						rpolobj[skindex] = rpolobj[objindex] - 10;
						yobj[skindex] = yobj[objindex];
						if (yobj[skindex]>midy)
							{
							dyobj[skindex] = -8;
							}
						else
							{
							dyobj[skindex] = 8;
							}
						}
					if (rpolobj[objindex]<0)
						{
						u += 180;
						}
					else
						{
						if (pendingsound == nosnd)
							{
							reqsound = tfsound;
							}
						}
					}
				else
					{
					if (rpolobj[objindex]<0)
						{
						yobj[objindex] *= 0.90;
						u += 180;
						}
					else
						{
						if (yobj[objindex]>(midy+10))
							{
							yobj[objindex] -= 2;
							}
						if (yobj[objindex]<(midy-10))
							{
							yobj[objindex] += 2;
							}
						}
					if (rpolobj[objindex] < (-1.2*zviewmax))
						{
						rpolobj[objindex] = zviewmax;
						yobj[objindex] = (0x07FF & Timer_msec(gt)) - 1000;
						dyobj[objindex]=0;
						}
					}
				/* Viewing angle correction (improve 3D position accuracy) */
				if (u<-179)
					{
					u += 360;
					}
				else
					{
					while (u >= 180)
						{
						u -= 360;
						}
					}
				relx = midx + (abs(rpolobj[objindex]))*fastsin(u);
				relz = (abs(rpolobj[objindex]))*fastcos(u);
				Place_obj (defobj[objindex], relx, yobj[objindex], relz, angobj[objindex], GREEN );
				/* Spot on the radar display */
				if ((abs(relx) < zviewmax)&&(abs(relz) < zviewmax))
					{
					if ((rpolobj[objindex] > 2500)||(rpolobj[objindex] < 0))
						{
						u=BLUE;
						}
					else
						{
						u=RED;
						}
					if (abs(rpolobj[objindex]) > 200)
						{
						Draw_point (5 + radarmidw + (relx * radarmidw)/(1.2*zviewmax), dispheight - 5 - radarmidh - (relz * radarmidh)/(1.2*zviewmax), u );
						}
					else
						{
						Draw_point ( 5 + radarmidw, dispheight - 5 - radarmidh, YELLOW );
						}
					}
				break;

				case type_xwingfighter :
				polangobj[objindex] += 1;
				angobj[objindex] += 1;
				if (rpolobj[objindex] < 2000)
					{
					polangobj[objindex] += 1;
					angobj[objindex] = polangobj[objindex] - 90 - angle; /* Overriding angobj handling for this object */
					}
				if (polangobj[objindex] > 180)
					{
					polangobj[objindex] -= 360;
					}
				if (angobj[objindex] > 180)
					{
					angobj[objindex] -= 360;
					}
				if (yobj[objindex] < -100)
					{
					dyobj[objindex] = 4;
					}
				if (yobj[objindex] > 100)
					{
					dyobj[objindex] = -4;
					}
				yobj[objindex] += dyobj[objindex];
				if (rpolobj[objindex] > 600)
					{
					rpolobj[objindex] -= 1;
					}
				relx = midx + (rpolobj[objindex])*fastsin(polangobj[objindex]-angle);
				relz = (rpolobj[objindex])*fastcos(polangobj[objindex]-angle);
				Place_obj (defobj[objindex], relx, yobj[objindex], relz, angobj[objindex], YELLOW );
				break;

				case type_badbullet :
				rpolobj[objindex] -= 10;
				if (abs(yobj[objindex]-midy)>12)
					{
					yobj[objindex] += dyobj[objindex];
					}
				relx = midx + (rpolobj[objindex])*fastsin(polangobj[objindex]-angle);
				relz = (rpolobj[objindex])*fastcos(polangobj[objindex]-angle);
				Place_obj (defobj[objindex], relx, yobj[objindex], relz, angobj[objindex], YELLOW );
				if (rpolobj[objindex] < 10)
					{
					/* Player hit. Make the screen flash */
					Draw_rectangle (0,0,dispwidth,dispheight,WHITE);
					/* Increase the dammage level */
					lifebarlevel += 10;
					Draw_rectangle (lifebarx, lifebary, lifebarx + lifebarlevel, lifebary + 20, RED);
					/* Bullet end-of-life */
					defobj[objindex] = type_none;
					if ((objindex==maxindex)&&(maxindex!=0))
						{
						maxindex -= 1;
						}
					/* Game over if dammage = 100% */
					if (lifebarlevel>=100)
						{
						gameover=1;
						}
					}
				break;

				case type_mybullet :
				rpolobj[objindex] += 35;
				if (rpolobj[objindex] > zviewmax)
					{
					if ((objindex==maxindex)&&(maxindex!=0))
						{
						maxindex -= 1;
						}
					defobj[objindex]=type_none;
					}
				else
					{
					yobj[objindex] += dyobj[objindex];
					relx = midx + (rpolobj[objindex])*fastsin(polangobj[objindex]-angle);
					relz = (rpolobj[objindex])*fastcos(polangobj[objindex]-angle);
					Place_obj (defobj[objindex], relx, yobj[objindex], relz, angobj[objindex], YELLOW );
					/* Look for succesful shots */
					for (skindex=0;skindex<=maxindex;skindex++)
						{
						if (defobj[skindex]==type_badbullet)
							{
							/* "bad" bullets */
							if ((abs(yobj[skindex]-yobj[objindex])<160)&&(abs(rpolobj[skindex]-rpolobj[objindex])<150))
								{
								i1 = abs(polangobj[skindex]-polangobj[objindex]);
								while (i1>180)
									{
									i1 -= 360;
									}
								i2=rpolobj[skindex];
								if (i2<50)
									{
									i2=50;
									}
								if (i1<5000/i2)
									{
									/* neutralized "bad" bullet */
									defobj[skindex]=type_none;
									if ((skindex==maxindex)&&(maxindex!=0))
										{
										maxindex -= 1;
										}
									defobj[objindex]=type_none;
									if ((objindex==maxindex)&&(maxindex!=0))
										{
										maxindex -= 1;
										}
									}
								}
							}
						else
							{
							if (defobj[skindex]==type_tiefighter)
								{
								/* Enemy tie-fighters */
								if (abs(yobj[skindex]-yobj[objindex])<160)
									{
									i2 = rpolobj[skindex];
									i3 = polangobj[skindex];
									if(i2<0)
										{
										i3 += 180;
										i2 = -i2;
										}
									while (i3 > 180)
										{
										i3 -= 360;
										}
									if (abs(i2-rpolobj[objindex])<150)
										{
										i1 = abs(i3 - polangobj[objindex]);
										if (rpolobj[skindex]<0)
											{
											i2 /= 2;
											}
										if (i2<50)
											{
											i2=50;
											}
										if (i1<(5000/i2))
											{
											/* tie-fighter shot down */
											reqsound = xplode;
											defobj[skindex]=type_explosion1;
											dyobj[skindex]=0;
											rpolobj[skindex] = i2;
											polangobj[skindex] = i3;
											defobj[objindex]=type_none;
											if ((objindex==maxindex)&&(maxindex!=0))
												{
												maxindex -= 1;
												}
											shootcount += 1;
											ntie -= 1;
											recoverdelay = 0;
											scorelevel += 5*((int) gamelevel);
											Dispnum (6, scorelevel, scorex, scorey, WHITE);
											}
										}
									}
								}
							else
								{
								if (defobj[skindex]==type_xwingfighter)
									{
									/* Rebel fighters */
									if ((abs(yobj[skindex]-yobj[objindex])<80)&&(abs(rpolobj[skindex]-rpolobj[objindex])<80))
										{
										if (abs(polangobj[skindex]-polangobj[objindex])<(5000/rpolobj[skindex])) /* rpolobj Never lower than 600 */
											{
											/* friendly shot ! */
											scorelevel -= 400;
											if (scorelevel<0)
												{
												scorelevel = 0;
												}
											Dispnum (6, scorelevel, scorex, scorey, WHITE);
											defobj[skindex]=type_none;
											if ((skindex==maxindex)&&(maxindex!=0))
												{
												maxindex -= 1;
												}
											}
										}

									}
								}
							}
						}
					}
				break;

				case type_explosion1 :
				angobj[objindex] += 1; /* Explosion is "rotating" continuously */
				if (angobj[objindex]>180)
					{
					angobj[objindex] -= 360;
					}
				rpolobj[objindex] -= 15; /* Continue initial tie-fighter progression, slower */
				dyobj[objindex] += 1;    /* Special use of this variable for presence time */
				if (dyobj[objindex] > 150)
					{
					defobj[objindex] = type_explosion2;
					}
				relx = midx + (rpolobj[objindex])*fastsin(polangobj[objindex]-angle);
				relz = (rpolobj[objindex])*fastcos(polangobj[objindex]-angle);
				Place_obj (defobj[objindex], relx, yobj[objindex], relz, angobj[objindex], RED );
				break;

				case type_explosion2 :
				/* Explosion terminal phase (smaller) */
				angobj[objindex] += 1; /* Explosion is "rotating" continuously */
				if (angobj[objindex]>180)
					{
					angobj[objindex] -= 360;
					}
				rpolobj[objindex] -= 15; /* Continue initial progression */
				dyobj[objindex] += 1;    /* Special use of this variable for presence time */
				relx = midx + (rpolobj[objindex])*fastsin(polangobj[objindex]-angle);
				relz = (rpolobj[objindex])*fastcos(polangobj[objindex]-angle);
				Place_obj (defobj[objindex], relx, yobj[objindex], relz, angobj[objindex], RED );
				if (dyobj[objindex] > 250)
					{
					defobj[objindex] = type_none;
					if ((objindex==maxindex)&&(maxindex!=0))
						{
						maxindex -= 1;
						}
					}
				break;
				}
			}
		Update_display();
		}
	if (sndon==1)
		{
		/* Stop soundcard use if sound was ON */
		close(handle);
		}
	if (keyescape==1)
		{
		/* Quit was requested */
		Graphics_shutdown ();  /* Close X11 window and session */
		exit(0);
		}
	/* Game end - loose or win... */
	if (lifebarlevel==100)
		{
		i1 = 0;
		angle = -180;
		reftim = Timer_msec(gt) + 40;
		while (i1<50)
			{
			keydet = Scanall();
			while (Timer_msec(gt) < reftim) {}
			reftim = Timer_msec(gt) + 10;
			angle += 10;
			if (angle>180)
				{
				angle = -180;
				}
			i1 += 1;
			Draw_rectangle (0,0,dispwidth,dispheight,BLACK); /* Pre-erase the screen to create an updated picture */
			Place_obj (type_explosion1, midx-100, midy, 70-i1, angle, RED );
			Place_obj (type_explosion1, midx, midy, 50-i1, angle, RED );
			Place_obj (type_explosion1, midx+100, midy, 70-i1, angle, RED );
			Update_display();
			}
		Draw_text ( "YOUR SCORE :", midx - 100, midy + 40, BLUE );
		Dispnum ( 6, scorelevel, midx + 30, midy + 40, BLUE );
		Draw_text ( "HI - SCORE :", midx - 100, midy + 80, BLUE );
		Dispnum ( 6, hiscore, midx + 30, midy + 80, BLUE );
		if (scorelevel > hiscore)
			{
			hiscore = scorelevel;
			Draw_text ( "YOU BEAT THE HI-SCORE !", midx - 110, midy + 120, YELLOW );
			}
		tmpch=0;
		while (keydet==0)
			{
			while (Timer_msec(gt) < reftim) {}
			reftim = Timer_msec(gt) + 60;
			keydet = Scanall();
			tmpch = 1-tmpch;
			if (tmpch)
				{
				Draw_text ( "GAME OVER", midx - 50, midy, YELLOW );
				}
			else
				{
				Draw_text ( "GAME OVER", midx - 50, midy, RED );
				}
			Update_display();
			}
		}
	else
		{
		Draw_text ( "YOUR SCORE :", midx - 100, midy + 40, BLUE );
		Dispnum ( 6, scorelevel, midx + 30, midy + 40, BLUE );
		tmpch=0;
		while (keydet==0)
			{
			while (Timer_msec(gt) < reftim) {}
			reftim = Timer_msec(gt) + 60;
			keydet = Scanall();
			tmpch = 1-tmpch;
			if (tmpch)
				{
				Draw_text ( "COMPLETED !", midx - 50, midy, YELLOW );
				}
			else
				{
				Draw_text ( "COMPLETED !", midx - 50, midy, RED );
				}
			Update_display();
			}
		}
	Draw_rectangle (0,0,WIN_WIDTH,WIN_HEIGHT,BLACK);
	goto restart;
	}

/*================================================================*/
/* Game loop related subroutines */
/*================================================================*/

/* Look for a free object definition index */

char Look_free()
	{
	unsigned char posfound = 40;
	if (maxindex==0)
		{
		if (defobj[0]==type_none)
			{
			return 0;
			}
		else
			{
			maxindex += 1;
			return 1;
			}
		}
	for (lkindex=0;(lkindex!=maxindex)&&(posfound==40);lkindex++)  /* (Don't scan maxindex because it cannot be free) */
		{
		if (defobj[lkindex] == type_none)
			{
			posfound = lkindex;
			}
		}
	if (posfound==40)
		{
		/* No position free => take the first one after current maxindex */
		maxindex += 1;
		if (maxindex==40)
			{
			printf("3D object handling overload !\n");
			exit(0);
			}
		posfound = maxindex;
		}
	return posfound;
	}

/*================================================================*/

/* Vectorized 3D routines */

/*================================================================*/

void Load_objects()
	{
	for (u=0; u!=tiefighter_nl; u+=3)
		{
		Polconv(tiefighter[u],tiefighter[u+1],tiefighter[u+2]);
		typdata[u][type_tiefighter]=rpol;
		typdata[u+1][type_tiefighter]=ydim;
		typdata[u+2][type_tiefighter]=polang;
		}
	nlines[type_tiefighter] = tiefighter_nl / 6;
	for (u=0; u!=xwingfighter_nl; u+=3)
		{
		Polconv(xwingfighter[u],xwingfighter[u+1],xwingfighter[u+2]);
		typdata[u][type_xwingfighter]=rpol;
		typdata[u+1][type_xwingfighter]=ydim;
		typdata[u+2][type_xwingfighter]=polang;
		}
	nlines[type_xwingfighter] = xwingfighter_nl / 6;
	for (u=0; u!=badbullet_nl; u+=3)
		{
		Polconv(badbullet[u],badbullet[u+1],badbullet[u+2]);
		typdata[u][type_badbullet]=rpol;
		typdata[u+1][type_badbullet]=ydim;
		typdata[u+2][type_badbullet]=polang;
		}
	nlines[type_badbullet] = badbullet_nl / 6;
	for (u=0; u!=mybullet_nl; u+=3)
		{
		Polconv(mybullet[u],mybullet[u+1],mybullet[u+2]);
		typdata[u][type_mybullet]=rpol;
		typdata[u+1][type_mybullet]=ydim;
		typdata[u+2][type_mybullet]=polang;
		}
	nlines[type_mybullet] = mybullet_nl / 6;
	for (u=0; u!=explosion1_nl; u+=3)
		{
		Polconv(explosion1[u],explosion1[u+1],explosion1[u+2]);
		typdata[u][type_explosion1]=rpol;
		typdata[u+1][type_explosion1]=ydim;
		typdata[u+2][type_explosion1]=polang;
		}
	nlines[type_explosion1] = explosion1_nl / 6;
	for (u=0; u!=explosion2_nl; u+=3)
		{
		Polconv(explosion2[u],explosion2[u+1],explosion2[u+2]);
		typdata[u][type_explosion2]=rpol;
		typdata[u+1][type_explosion2]=ydim;
		typdata[u+2][type_explosion2]=polang;
		}
	nlines[type_explosion2] = explosion2_nl / 6;
	}

/*================================================================*/

void Place_obj ( unsigned char otype, int xbj, int ybj, int zbj, int angbj, unsigned int color )
	{
	unsigned char ct;
	unsigned int cptr;
	int xpt0;
	int ypt0;
	int zpt0;
	int xpt1;
	int ypt1;
	int zpt1;
	if (zbj > 0)
		{
		Set_3d_color (color);
		cptr = 0;
		for (ct=0; ct != nlines[otype]; ct++)
			{
			/* Rotation applied to object into X-Z plane around Y axis */
			xpt0 = xbj + (int) (typdata[cptr][otype] * fastsin (typdata[cptr+2][otype] + angbj));
			zpt0 = zbj + (int) (typdata[cptr][otype] * fastcos (typdata[cptr+2][otype] + angbj));
			xpt1 = xbj + (int) (typdata[cptr+3][otype] * fastsin (typdata[cptr+5][otype] + angbj));
			zpt1 = zbj + (int) (typdata[cptr+3][otype] * fastcos (typdata[cptr+5][otype] + angbj));
			/* Y coords are not affected (but axis is inverted) */
			ypt0 = ybj - typdata[cptr+1][otype];
			ypt1 = ybj - typdata[cptr+4][otype];
			Line_3d (xpt0,ypt0,zpt0,xpt1,ypt1,zpt1);
			cptr += 6;
			}
		}
	}

/*================================================================*/

/* Convert 3D cartesian x-z data to x-z plane polar coords, refering to 3D object center */

void Polconv(int xr, int yr, int zr)
	{
	float xtemp;                   /* rotated cartesian coords of the positionned 3D object */
	float ztemp;
	xtemp = (float) 2 * xr;        /* (magnify the object by the way) */
	ydim = 2 * yr;
	ztemp = (float) 2 * zr;
	rpol = (int) sqrtf((xtemp*xtemp) + (ztemp*ztemp));
	if (ztemp != 0)
		{
		polang = fastatn(xtemp/ztemp);
		if (ztemp < 0)
			{
			polang += 180;
			}
		}
	else
		{
		if (xtemp >= 0)
			{
			polang = 90;
			}
		else
			{
			polang = -90;
			}
		}
	}


/*================================================================*/

void Draw_line ( int x0, int y0, int x1, int y1, unsigned int color )
{
   XSetForeground ( display, color_gc, color_table[color] );
   XDrawLine ( display, double_buffer, color_gc, x0, y0, x1, y1 );
}

/*================================================================*/

void Set_3d_color ( unsigned int color )
	{
	XSetForeground ( display, color_gc, color_table[color] );
	}

/*================================================================*/

void Line_3d ( int x3, int y3, int z3, int x4, int y4, int z4 )
	{
	int x0;
	int y0;
	int x1;
	int y1;
	if ((z3 < zviewmax )&&(z4 < zviewmax)&&(z3 > 0)&&(z4 > 0))
		{
		x0 = midx + (((x3 - midx) * perspective) / (z3 + perspective));
		y0 = midy + (((y3 - midy) * perspective) / (z3 + perspective));
		x1 = midx + (((x4 - midx) * perspective) / (z4 + perspective));
		y1 = midy + (((y4 - midy) * perspective) / (z4 + perspective));
		if ((x0 > 0)&&(x1 > 0)&&(x0 < dispwidth)&&(x1 < dispwidth)&&(y0 > 0)&&(y1 > 0)&&(y0 < dispheight)&&(y1 < dispheight))
			{
			XDrawLine ( display, double_buffer, color_gc, x0, y0, x1, y1 );
			}
		}
	}

/*================================================================*/

void Draw_rectangle ( int x0, int y0, int x1, int y1, unsigned int color )
{
   XSetForeground ( display, color_gc, color_table[color] );

   XFillRectangle ( display, double_buffer, color_gc, x0, y0, x1-x0, y1-y0 );
}


/*================================================================*/

void Draw_point ( int x0, int y0, unsigned int color )
{
   XSetForeground ( display, color_gc, color_table[color] );

   XFillRectangle ( display, double_buffer, color_gc, x0-3, y0+3, 3, 3 );
}

/*================================================================*/

void Draw_text ( char *message, int x0, int y0, unsigned int color )
{
   int texlen=0;
   while(message[texlen]!=0)
	{
	texlen++;
	}
   XSetForeground ( display, text_gc, color_table[color] );

   XDrawString ( display, double_buffer, text_gc, x0, y0, message, texlen );
}

/*================================================================*/

void Dispnum ( int ndigits, int numval, int x0, int y0, unsigned int color )
{
   char dispstr[11];
   int npt;
   int divent;
   int divisor;
   if (ndigits > 10)
   	{
	ndigits = 10;
	}
   XFillRectangle ( display, double_buffer, black_gc, x0, y0-20, 15*ndigits, 20 ); /* Pre-erase area */
   divisor = 1;
   if (ndigits > 1)
   	{
   	for (npt = 0; npt != (ndigits - 1); npt += 1)
		{
		divisor *= 10;
		}
	npt = 0;
	while (divisor != 1)
		{
		divent = (int) (numval / divisor);
		numval -= (divent * divisor);
		dispstr[npt] = ((char) divent) + 0x30;
		divisor /= 10;
		npt++;
		}
	}
   else
   	{
	npt=0;
	}
   dispstr[npt] = (char) numval + 0x30;
   dispstr[npt+1] = 0;
   Draw_text (dispstr, x0, y0, color );
}



/*================================================================*/

int Graphics_init ( unsigned int win_width, unsigned int win_height )
   {
   int i, j;

   window_width  = win_width;
   window_height = win_height;

   display = XOpenDisplay ( display_name );
   if ( !display )
      return FALSE;

   screen_num = DefaultScreen ( display );
   screen_ptr = DefaultScreenOfDisplay ( display );

   win = XCreateSimpleWindow ( display, RootWindow ( display, screen_num ), 0, 0, window_width, window_height, 1, BlackPixel ( display, screen_num ), WhitePixel ( display, screen_num ) );

/*   XSelectInput ( display, win, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask ); */

   XSelectInput ( display, win, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask );

   XStringListToTextProperty ( &window_name, 1, &wname );
   XSetWMProperties ( display, win, &wname, NULL, NULL, 0, NULL, NULL, NULL );

   /* create GCs for clearing, text, & drawing */
   black_gc = XCreateGC ( display, win, gc_valuemask, &gc_values );
   color_gc = XCreateGC ( display, win, gc_valuemask, &gc_values );
   text_gc  = XCreateGC ( display, win, gc_valuemask, &gc_values );


   /* load default font */
   font_info = XLoadQueryFont ( display, font_name );
   if ( !font_info )
	{
	perror ( "WARNING: could not load default font.\n" );
	}
   else
	{
	XSetFont ( display, text_gc, font_info->fid );
	}

   /* load default color scheme */

   /* red */
   for ( i=0, j=0; i<64; i++, j++ )
	{
	color_data[i][0] = 1024 * j;
	color_data[i][1] = color_data[i][2] = 0;
	}

   /* green */
   for ( i=64, j=0; i<128; i++, j++ )
      {
      color_data[i][1] = 1024 * j;
      color_data[i][0] = color_data[i][2] = 0;
      }

   /* blue */
   for ( i=128, j=0; i<192; i++, j++ )
      {
      color_data[i][2] = 1024 * j;
      color_data[i][0] = color_data[i][1] = 0;
      }

   /* white */
   for ( i=192, j=0; i<256; i++, j++ )
      {
      color_data[i][0] = color_data[i][1] = color_data[i][2] = j * 1024;
      }

   /* yellow */
   color_data[192][0] = 63 * 1024;
   color_data[192][1] = 63 * 1024;
   color_data[192][2] = 32 * 1024;


   for ( i=0; i<MAX_COLORS; i++ )
      {
      color_info.red   = color_data[i][0];
      color_info.green = color_data[i][1];
      color_info.blue  = color_data[i][2];

      XAllocColor ( display,
            DefaultColormap ( display, screen_num ),
            &color_info );

      color_table[i] = color_info.pixel;
      }

   XSetForeground ( display, black_gc, color_table[BLACK] );
   XSetForeground ( display, color_gc, color_table[GREEN] );
   XSetForeground ( display, text_gc, color_table[GREEN] );

   double_buffer = XCreatePixmap ( display, win, window_width, window_height, DefaultDepth ( display, screen_num ) );
   XFillRectangle ( display, double_buffer, black_gc, 0, 0, window_width, window_height );
   XAutoRepeatOff ( display );
   XMapWindow ( display, win );

   return TRUE;
   }

/*================================================================*/

int Update_display ( void )
{
   XCopyArea ( display, double_buffer, win, black_gc, 0, 0, window_width, window_height, 0, 0 );
   return TRUE;
}


/*================================================================*/

void Graphics_shutdown ( void )
{
   if ( font_info )
      XUnloadFont ( display, font_info->fid );

   XFreeGC ( display, black_gc );
   XFreeGC ( display, color_gc );
   XFreeGC ( display, text_gc );

   XAutoRepeatOn ( display );

   XCloseDisplay ( display );
}


/*================================================================*/

/* Scan keyboard and mouse */

unsigned char Scanall ( void )
{
   char pressdet = 0;
   char followmouse = 1;
   char kbact;
   XEvent event;
   KeySym keysym;
   while ((followmouse == 1)||(kbact == 1))
	{
	followmouse = 0;
	kbact = 0;
	XNextEvent ( display, &event );
	switch ( event.type )
      	  {
          case ButtonPress:
	    mouseb = event.xbutton.button;
	    selmousex = event.xbutton.x;
	    selmousey = event.xbutton.y;
	    followmouse = 1;
            break;

	  case ButtonRelease:
	    mouseb = 0;
	    followmouse = 1;
	    break;

	  case KeyPress:
            keysym = XLookupKeysym ( &(event.xkey), 0 );
	    pressdet = 1;
	    kbact = 1;
            switch ( keysym )
            {
               case XK_space:
                  keyspace = 1;
                  break;

               case XK_Up:
               case XK_KP_Up:
                  keyup = 1;
                  break;

               case XK_Down:
               case XK_KP_Down:
                  keydown = 1;
                  break;

               case XK_Left:
               case XK_KP_Left:
                  keyleft = 1;
                  break;

               case XK_Right:
               case XK_KP_Right:
                  keyright = 1;
                  break;

               case XK_Escape:
                  keyescape = 1;
                  break;

               default:
                  break;
            }
            break;

          case KeyRelease:
            keysym = XLookupKeysym ( &(event.xkey), 0 );
	    kbact = 1;
	    switch ( keysym )
            {
               case XK_space:
                  keyspace = 0;
                  break;

               case XK_Up:
               case XK_KP_Up:
                  keyup = 0;
                  break;

               case XK_Down:
               case XK_KP_Down:
                  keydown = 0;
                  break;

               case XK_Left:
               case XK_KP_Left:
                  keyleft = 0;
                  break;

               case XK_Right:
               case XK_KP_Right:
                  keyright = 0;
                  break;

	       case XK_Escape:
                  keyescape = 0;
                  break;

               default:
                  break;
            }
	    break;

	  case MotionNotify:
	    mousex = event.xmotion.x;
	    mousey = event.xmotion.y;
	    followmouse = 1;
	    break;

          default:
	    pressdet = (keyspace | keyup | keydown | keyleft | keyright | keyescape);
            break;
	  }
	}

   return pressdet;
}

/*================================================================*/

/*------------------------------------------------------------------
 *
 * System msec & sec Timer functions
 *
------------------------------------------------------------------*/

/*================================================================*/

long Timer_msec ( TIMER *t )
{
   long msec;

   if ( gettimeofday ( &(t->t1), NULL ) < 0 ) return -1;

   msec = ((t->t1.tv_sec-t->t0.tv_sec)*1000)+
      ((t->t1.tv_usec-t->t0.tv_usec)/1000);

   return msec;
}


/*================================================================*/

double Timer_sec ( TIMER *t )
{
   return difftime ( time(NULL), t->init_time_stamp );
}


/*================================================================*/

/*------------------------------------------------------------------
 *
 * Math & computation functions
 *
------------------------------------------------------------------*/

/*================================================================*/

/* Fast sinus function (degrees) */

float fastsin (int argangle)
	{
	float sinres;
	while (argangle > 180)
		{
		argangle -= 360;
		}
	while (argangle <= -180)
		{
		argangle += 360;
		}
	if (argangle < 0)
		{
		sinres = -trigotable[(int) (-argangle / 5)];
		}
	else
		{
		sinres = trigotable[(int) (argangle / 5)];
		}
	return sinres;
	}

/* Fast cosinus function (degrees) */

float fastcos (int argangle)
	{
	return fastsin (argangle + 90);
	}

/* Fast tengent inverse */

int fastatn (float tgvalue)
	{
	unsigned char aptr;
	aptr = 18;
	if (tgvalue >= 0)
		{
		while (tgtable[aptr] > tgvalue)
			{
			aptr -= 1;
			}
		return 5 * aptr;
		}
	else
		{
		tgvalue = -tgvalue;
		while (tgtable[aptr] > tgvalue)
			{
			aptr -= 1;
			}
		return -5 * aptr;
		}
	}

/* Soundcard init + sound files loading */

void snd_init (void)
    {
    int tmp;
    char filname[35] = {'.','/','s','n','d','1','.','w','a','v',0};

    /* Clear and set size of the zero-string buffer */
    sampbuffer = calloc(1,fragments);

    /* Allocate memory space and load sound files */
    for (nfiles = 1; nfiles != 4; nfiles++)
	{
	filname[29] = 0x30 + nfiles;
	fp=fopen(filname,"rb");
	if(fp==NULL)
		{
		Graphics_shutdown ();  /* Close X11 window and session */
		printf("%s not found\n",filname);
        	exit(0);
		}
	fseek(fp,0,SEEK_END);
	samplen[nfiles-1]=ftell(fp)-44;
	fseek(fp,44,SEEK_SET);
	sampledata[nfiles-1]=calloc(1,samplen[nfiles-1]);
	if(sampledata[nfiles-1]==NULL)
		{
		Graphics_shutdown ();  /* Close X11 window and session */
		printf("Not enough available memory\n");
        	exit(0);
		}
	fread(sampledata[nfiles-1],1,samplen[nfiles-1],fp);
	fclose(fp);
	}

    /* Open the sound device for writing */
    handle=open("/dev/dsp",O_WRONLY);
    if(handle==-1)
	{
	Graphics_shutdown ();  /* Close X11 window and session */
	printf("Sound card could not be accessed\n");
        exit(0);
	}

    /* Init by Marq/Fit 2000 + Advices from the author */

    /* Set sound parameters. If you set them in a different order I can't
       promise it will work. ioctl() calls return -1 if the operation
       failed completely. If the parameter you passed to an ioctl()
       is modified, the device can't do something you asked. Didn't
       bother to check them... */

    /* Set fragment length (don't do if you don't care about latency).
       Small values give a small latency but will sound bad if the app
       won't get enough CPU time.
       (2<<8) - We want two fragments
       12      - The size of fragment is 2^12 = 4096 bytes
    */
    tmp=(2<<8)+12;
    ioctl(handle,SNDCTL_DSP_SETFRAGMENT,&tmp);

    /* Set output format. Some usual values are:
       AFMT_S16_LE - 16-bit signed
       AFMT_U8     - 8-bit unsigned */
    tmp=AFMT_S16_LE;
    ioctl(handle,SNDCTL_DSP_SETFMT,&tmp);

    /* Set number of channels. 0=mono, 1=stereo */
    tmp=1;
    ioctl(handle,SNDCTL_DSP_STEREO,&tmp);

    /* Set playing frequency. 44100, 22050 and 11025 are safe bets. */
    tmp=44100;
    ioctl(handle,SNDCTL_DSP_SPEED,&tmp);

    /* OK. Now the device should be initialised. It is ready to handle the audio writing. */
    }

