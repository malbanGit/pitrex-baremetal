#define Y_LOW_CLIP -9000
#define Y_HIGH_CLIP 13500

#define OPENLASE 1
#define OPENLASEGRAYSCALE 1

/*
    (C) Copyright 2007,2008, Stephen M. Cameron.

    This file is part of wordwarvi.

    wordwarvi is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    wordwarvi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with wordwarvi; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 */
#include <stdio.h>
#include <stdlib.h>
//#include <gtk/gtk.h>

#define gboolean int
#define gint int
#define guint16 unsigned short int
#define guint32 unsigned int

#define GdkDrawable void 
#define GdkGC void 
#define GdkEvent void
typedef struct 
{
  guint32 pixel;
  guint16 red;
  guint16 green;
  guint16 blue;
} GdkColor;



#include <string.h>
//#include <sys/time.h>
//#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdint.h>
#include <math.h>

#include <vectrex/vectrexInterface.h>



#define GNU_SOURCE
#include <getopt.h>

#include "compat.h"
#include "rumble.h"
#include "wwviaudio.h"
#include "my_point.h"
#include "wwvi_font.h"
#include "version.h"
//#include "stamp.h"
static int builder = 1;
static uint64_t builtat = 1;

#ifndef DATADIR
#define DATADIR ""
#endif

#ifdef OPENLASE
/* openlase related defines */
#define LASERTOP 127
#define LASERBOTTOM -127
#define LASERLEFT -127
#define LASERRIGHT 127
#define LASERWIDTH (LASERRIGHT - LASERLEFT)
#define LASERHEIGHT (-(LASERBOTTOM - LASERTOP)) // vectrex MINUS
#define LASERFRAMERATE (1000)
#define LASERMAXLINES 2000
#endif

#ifndef TRUE
#define TRUE (1==1)
#define FALSE (1==0)
#endif

#ifndef M_PI
#define M_PI  (3.14159265)
#endif
#define TWOPI (M_PI * 2.0)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define DEBUG_HITZONE 0

/* #define DEBUG_TARGET_LIST */

#define MAX_CONCURRENT_SOUNDS (26)
#define NCLIPS 59

/* define sound clip constants */
#define PLAYER_LASER_SOUND 0
#define BOMB_IMPACT_SOUND 1
#define ROCKET_LAUNCH_SOUND 2
#define FLAK_FIRE_SOUND 3
#define LARGE_EXPLOSION_SOUND 4
#define ROCKET_EXPLOSION_SOUND 5
#define LASER_EXPLOSION_SOUND 6
#define GROUND_SMACK_SOUND 7
#define INSERT_COIN_SOUND 8
#define MUSIC_SOUND 9
#define SAM_LAUNCH_SOUND 10 
#define THUNDER_SOUND 11
#define INTERMISSION_MUSIC_SOUND 12
#define MISSILE_LOCK_SIREN_SOUND 13
#define CARDOOR_SOUND 14
#define OWMYSPINE_SOUND 15
#define WOOHOO_SOUND 16
#define HELPDOWNHERE_SOUND 17
#define CRONSHOT 18
#define HELPUPHERE_SOUND 19
#define ABDUCTED_SOUND 20 
#define CLANG_SOUND 21
#define SCREAM_SOUND 22
#define BODYSLAM_SOUND 23
#define USETHESOURCE_SOUND 24
#define OOF_SOUND 25
#define METALBANG1 26
#define METALBANG2 27
#define METALBANG3 28
#define METALBANG4 29
#define METALBANG5 30 
#define METALBANG6 31
#define METALBANG7 32
#define METALBANG8 33
#define STONEBANG1 34
#define STONEBANG2 35
#define STONEBANG3 36
#define STONEBANG4 37
#define STONEBANG5 38
#define STONEBANG6 39
#define STONEBANG7 40
#define STONEBANG8 41
#define CORROSIVE_SOUND 42
#define VOLCANO_ERUPTION 43
#define IT_BURNS 44
#define ZZZT_SOUND 45
#define GRAVITYBOMB_SOUND 46
#define DESTINY_FACEDOWN 47
#define HIGH_SCORE_MUSIC 48
#define JETWASH_SOUND 49
#define TIMPANI_BOING 50
#define NICE_BANK_SHOT 51
#define HOHOHO 52
#define HOHOHO_MERRY_XMAS 53
#define YAY_SANTA 54
#define RADAR_FAIL 55
#define RADAR_READY 56
#define GUNWHEEL 57
#define TESLA_FIRE 58
/* ...End of audio stuff */

#define NHUMANOIDS 4 /* number of vi .swp files, initially */
#define MAXHUMANS 10 /* maxiumum number possible of vi .swp files */
#define RADAR_HEIGHT 60  /* height, in pixels, of radar screen */
#define RADAR_LEFT_MARGIN 10 /* space to leave, in pixels left/right of radar screen to window edge. */
#define RADAR_RIGHT_MARGIN 160 /* space to leave, in pixels left/right of radar screen to window edge. */
#define RADAR_YMARGIN 10 /* space to leave, in pixels from bottom edge of radar to bottom of window */
#define MAX_RADAR_NOISE 100 /* maximum number of noise pixels placed by radar jammers, increasing */
                 /* this may adversely affect performance, as this number of x's are */
                 /* drawn EVERY FRAME when in close proximity to jammer. */
#ifdef OPENLASE
#define FRAME_RATE_HZ 30    /* target frame rate at which gtk callback fires by default */
#else
#define FRAME_RATE_HZ 30    /* target frame rate at which gtk callback fires by default */
#endif
int frame_rate_hz = FRAME_RATE_HZ; /* Actual frame rate, user adjustable. */

#define TERRAIN_LENGTH 1000 /* length, in number of line segments, of terrain */
#define SCREEN_WIDTH 800        /* window width, in pixels */
#define SCREEN_HEIGHT 600       /* window height, in pixels */
#define WORLDWIDTH (SCREEN_WIDTH * 40)  /* width of world, 40 screens wide. */
#define KERNEL_Y_BOUNDARY (-1000) /* basically, an arbitrary altitude limit on movement of player. */

#define VOLCANO_XFRACTION (15)   /* volcano x pos is 1/VOLCANO_XFRACTION of the way across the world. */

#define LARGE_SCALE_ROUGHNESS (0.04)   /* limits roughness, on large scale, of fractal terrain algorithm */
#define SMALL_SCALE_ROUGHNESS (0.09)   /* limits roughtness, on small scale, of fractal terrain algorithm */
#define MAXOBJS 8500               /* arbitrary, maximum number of objects in game, ~4000 are commonly seen. */


                    /* object allocation algorithm uses a bit per object to indicate */
                    /* free/allocated... how many 32 bit blocks do we need?  NBITBLOCKS. */
#define NBITBLOCKS ((MAXOBJS >> 5) + 1)  /* 5, 2^5 = 32, 32 bits per int. */


#define LASER_BOLT_DAMAGE 5         /* damage done by flak guns to player */
#define PLAYER_LASER_DAMAGE 20      /* damage done by player's laser (to blimps, clipper ships */

#define NFLAK 10            /* Number of flak guns (laser turrets) */
#define NKGUNS 30           /* Number of Kernel defense guns (laser turrets) */
#define KGUN_INIT_HEALTH 1      /* number of hits it takes to kill kgun. */
#define MAX_KGUN_HEALTH  1      /* An interesting idea, but >1 makes the game too hard. */
#define NROCKETS 20             /* Number of rockets sprinkled into the terrain */ 
#define NBIGROCKETS 15
#define NJETS 15            /* Number of jets sprinkled into the terrain */ 
#define LAUNCH_DIST 1200            /* How close player can get in x dimension before rocket launches */
#define BIG_ROCKET_LAUNCH_DIST 200          /* How close player can get in x dimension before rocket launches */
#define MAX_ROCKET_SPEED -32        /* max vertical speed of rocket */
#define SAM_LAUNCH_DIST 400     /* How close player can get in x deminsion before SAM might launch */
#define GDB_LAUNCH_DIST 700     /* How close player can get in x deminsion before GDB might launch */
#define SAM_LAUNCH_CHANCE 15        /* chances out of 100 if in ranch, missile will be launched */
#define PLAYER_SPEED 8          /* max X player speed, in pixels per frame */
#define MAX_VX 15           /* Hmm... another max X player speed?  a bug, I think. */
#define MAX_VY 25           /* Max player y speed, pixels per frame. */
#define LASERLEAD (11)          /* How many pixels left/right to lead the player in aiming flak guns */ 
#define LASERLEADX (11)         /* How many pixels left/right to lead the player in aiming flak guns */ 
#define LASERLEADY (0)          /* How many pixels up/down to lead the player in aiming flak guns */    
#define LASER_SPEED 50          /* Speed of player's laser beams, pixels/frame */
#define LASER_PROXIMITY 300
#define LASER_Y_PROXIMITY 8
#define BOMB_PROXIMITY 10000 /* square root of 30000, how close bomb has to be to be considered a "hit", squared. */
#define BOMB_X_PROXIMITY 100 /* X proximity to hit, for bombs which impact the ground. */

#define CRON_SHOT_CHANCE 40  /* chance out of 100 that cron jobs take a shot if in range, and timer % 10 == 0 */
#define CRON_SHOT_DIST_SQR (300*300)  /* 300 pixels dist for cron to consider player "in range" */
#define CRON_DX_THRESHOLD 14  /* dist must be <= this many pixels for cron to consider himself to have */
#define CRON_DY_THRESHOLD 14  /* reached his destination. */
#define CRON_MAX_VX 8         /* max x and y velocities of cron jobs */
#define CRON_MAX_VY 5 

#define GDB_DX_THRESHOLD 25  /* dist must be <= this many pixels for gdb to consider himself to have */
#define GDB_DY_THRESHOLD 25  /* reached it's destination.  It's large, so they won't just sit on the */
                  /* player's face. */
#define GDB_MAX_VX 13         /* max x and y velocities of GDBs. */
#define GDB_MAX_VY 13 

#define JETPILOT_BIG_DY_THRESHOLD 40
#define JETPILOT_SMALL_DY_THRESHOLD 20 
#define JETPILOT_MAX_VX 5
#define JETPILOT_MAX_VY 13 

#define NBUILDINGS 15       /* initial number of buildings on the terrain */
#define MAXBUILDING_WIDTH 9 /* max building width, in terrain line segments */
#define NFUELTANKS 20       /* Initial number of fuel tanks sprinkled around the terrain */
/* How many hit points a fuel tank contains */
#define FUELTANK_CAPACITY (game_state.max_player_health / 3)
/* #define REFUEL_RATE 1 */     /* lower numbers = faster, used as modulo of timer */
/* lower numbers = faster, used as modulo of timer */
#define REFUEL_RATE (game_state.max_player_health >= 100 ? 1 : 100/game_state.max_player_health)    
/* lower numbers == faster, used as modulo of timer */
#define REFILL_RATE (FUELTANK_CAPACITY <= 30 ? 30/FUELTANK_CAPACITY * frame_rate_hz * 3 : frame_rate_hz * 3) 

#define NJAMMERS 1      /* Initial number of radar jammers sprinkled through the terrain */
#define NCRON 15        /* Initial number of cron jobs sprinkeled through the terrain */
#define NSHIPS 1        /* Initial number of clipper ships sprinkeled through the terrain */
#define NGDBS 3         /* Initial number of gdbs sprinkeled through the terrain */
#define NOCTOPI 0       /* Initial number of octopi sprinkled through the terrain */
#define NTENTACLES 2        /* Initial number of tentacles sprinkled through the terrain */
#define NSAMS 3         /* Initial number of SAM stations sprinkled through the terrain */
#define BOMB_SPEED 10       /* differential x velocity of bomb as it leaves player's ship */

#define MAX_ALT 100     /* for "attract mode", max altitude above ground player flies. */
#define MIN_ALT 50      /* for "attract mode", min altitude above ground player flies. */

#define EASY_MAXHEALTH 150      /* Max, and initial health value of player */
#define MEDIUM_MAXHEALTH 100        /* Max, and initial health value of player */
#define HARD_MAXHEALTH 32       /* Max, and initial health value of player */
#define INSANE_MAXHEALTH 10     /* Max, and initial health value of player */
#define BATSHIT_INSANE_MAXHEALTH 3  /* Max, and initial health value of player */

        /* If player's health drops below this, the radar goes on the fritz */
#define RADAR_FRITZ_HEALTH (game_state.max_player_health / 4)   
#define NAIRSHIPS 1     /* Initial number of blimps sprinkled through the terrain */
#define NBALLOONS 2         /* Initial number of balloons sprinkled through the terrain */
#define NWORMS 2        /* Initial number of worms sprinkled through the terrain */
#define MAX_WORM_VX 8
#define MAX_WORM_VY MAX_WORM_VX 

#define MAX_BALLOON_HEIGHT 300  /* these two control blimps, balloons, and clipper ships */
#define MIN_BALLOON_HEIGHT 50   /* limiting their altitude to a range above the ground (pixels) */

#define MAX_MISSILE_VELOCITY 19 /* Max x and y missile/harpoon velocities */
#define MISSILE_DAMAGE 20   /* amount of damage missile/harpoon inflict on player */
#define MISSILE_PROXIMITY 10    /* x and y proximity for missiles to be considered a hit (pixels) */
#define MISSILE_FIRE_PERIOD (frame_rate_hz * 1) /* time which must elapse between firings of missiles */
#define WORM_DAMAGE 5

#define BULLET_SPEED 25     /* max x/y velocities of bullets (cron jobs shoot) */
#define BULLET_DAMAGE 20    /* amount of damage bullets inflict on player */
#define BULLET_PROXIMITY 10 /* x and y proximity for bullet to be considered a hit (pixels) */
#define BULLET_LEAD_TIME 15 /* pixels per unit of velocity to lead player while aiming bullets */

#define HUMANOID_DIST 15       /* proximity to pick up a .swp file */

#define MAX_TENTACLE_SEGS 40       /* max number of segments of a tentacle */
#define MAX_SEG_ANGLE 60       /* max angle a tentacle may bend with the one next to it */
                /* macro to compute initial tentacle angles */
#define TENTACLE_RANGE(t) (randomn(t.upper_angle - t.lower_angle) + t.lower_angle)

/* Scoring stuff */

#define DELIVER_PRESENTS_SCORE 1000  /* score for delivering presents (xmas mode) */

#define GROUND_OOPS 64000   /* There are some functions which, given an x value, */
                /* return the corresponding y value for the terrain */
                /* at that x value.  For some values of x, there is */
                /* no y.  The value returned in that case is GROUND_OOPS */
                /* The functions are: find_ground_level() and */
                /* approximate_horizon() */

/* some globals... maybe should be in game_state */
int game_pause = 0;     /* is game paused? */
int game_pause_help = 0;    /* is game in help-pause mode? */
int in_the_process_of_quitting = 0;
int current_quit_selection = 0;
int final_quit_selection = 0;
int attract_mode = 0;       /* is game in attract mode */
int credits = 0;        /* how many quarters have been put in, but not played? */
int no_colors_any_more = 0;
/* int toggle = 0;      
*/
int total_radar_noise;      /* count of how much radar noise has been drawn in a given frame */
                /* This is to limit things, so if there are 10 radar jammers in */
                /* close range, we don't draw 10x the amount of noise necessary */
                /* to obscure things. If the radar is totally obscured, obscuring */
                /* it 10 more times is wasting cpu. */

int timer = 0;          /* monotonically increases by 1 with every frame */
int next_timer = 0;     /* when timer reaches this value, timer_expired() is called */
int timer_event = 0;        /* timer_expired() switches on this value... */ 

int sound_device = -1;
/* timer_event values. */
#define BLINK_EVENT 1
#define READY_EVENT 2
#define SET_EVENT 3
#define GO_EVENT 4
#define BLANK_EVENT 5
#define GAME_OVER_EVENT 6
#define BLANK_GAME_OVER_1_EVENT 7
#define INSERT_COIN_EVENT 8
#define BLANK_GAME_OVER_2_EVENT 9
#define GAME_ENDED_EVENT 10
#define GAME_ENDED_EVENT_2 11
#define CREDITS1_EVENT 12
#define CREDITS2_EVENT 13
#define INTRO1_EVENT 14
#define INTRO2_EVENT 15
#define START_INTERMISSION_EVENT 16
#define END_INTERMISSION_EVENT 17
#define KEYS1_EVENT 18
#define KEYS2_EVENT 19
#define START_HIGHSCORES_EVENT 20
#define END_HIGHSCORES_EVENT 21
#define NEW_HIGH_SCORE_PRE_EVENT 22
#define NEW_HIGH_SCORE_PRE2_EVENT 23
#define NEW_HIGH_SCORE_EVENT 24

int nomusic = 0;
int sound_working = 0;
int round_explosions = 0;
int spherical_explosions = 1;
int explosion_factor = 1;
int brightsparks = 0;       /* controls preference for how to draw sparks */
int xmas_mode = 0;
int thicklines = 0;     /* controls preference for how to draw lines. */
int nframes = 0;        /* count of total frames drawn, used for calculating actual frame rate */
struct timeval start_time, end_time;        /* start and end time of game, for calc'ing frame rate */

unsigned int free_obj_bitmap[NBITBLOCKS] = {0}; /* bitmaps for object allocater free/allocated status */
int jsfd = -1;

#define NCOLORS 10      /* number of "cardinal" colors */
#define NSPARKCOLORS 25     /* 25 shades from yellow to red for the sparks */
#define NRAINBOWSTEPS (16)
#define NRAINBOWCOLORS (NRAINBOWSTEPS*3)

GdkColor huex[NCOLORS + NSPARKCOLORS + NRAINBOWCOLORS]; /* all the colors we have to work with are in here */
GdkColor *sparkcolor;       /* a pointer into the huex[] array where the spark colors begin */
GdkColor *rainbow_color;    /* a pointer into the huex[] array where the rainbow colors begin */

/* cardinal color indexes into huex array */
#define WHITE 0
#define BLUE 1
#define BLACK 2
#define GREEN 3
#define YELLOW 4
#define RED 5
#define ORANGE 6
#define CYAN 7
#define MAGENTA 8
#define DARKGREEN 9

int planet_color[] = {
    RED, GREEN, YELLOW, ORANGE, MAGENTA, CYAN, WHITE
};

#include "levels.h"

int score_table[256]; /* table of scores for each object type. */
int kill_tally[256];  /* tally of various things killed */

void init_score_table()
{
    memset(score_table, 0, sizeof(score_table));
    score_table[OBJ_TYPE_AIRSHIP]       = -5000;
    score_table[OBJ_TYPE_JET]       = 2000;
    score_table[OBJ_TYPE_BALLOON]       = 3000;
    score_table[OBJ_TYPE_CRON]      = 400;
    score_table[OBJ_TYPE_FUEL]      = 0;
    score_table[OBJ_TYPE_SHIP]      = 10000;
    score_table[OBJ_TYPE_GUN]       = 400;
    score_table[OBJ_TYPE_HUMAN]     = 1000;
    score_table[OBJ_TYPE_MISSILE]       = 50;
    score_table[OBJ_TYPE_HARPOON]       = 50;
    score_table[OBJ_TYPE_ROCKET]        = 100;
    score_table[OBJ_TYPE_BIG_ROCKET]    = 400;
    score_table[OBJ_TYPE_SAM_STATION]   = 400;
    score_table[OBJ_TYPE_BRIDGE]        = 10;
    score_table[OBJ_TYPE_GDB]       = 400;
    score_table[OBJ_TYPE_OCTOPUS]       = 880;
    score_table[OBJ_TYPE_TENTACLE]      = 100;
    score_table[OBJ_TYPE_JAMMER]        = 100;
    score_table[OBJ_TYPE_WORM]      = 30;
    score_table[OBJ_TYPE_KGUN]      = 400;
    score_table[OBJ_TYPE_JETPILOT]      = 300;
}

void init_kill_tally()
{
    memset(kill_tally, 0, sizeof(kill_tally));
}

int current_level = 0;      /* current level of the game, starts at zero */
struct level_parameters_t {
    int random_seed;    /* so the games always have the same terrain, setup, etc. */
    int level_number;

    /* numbers of various objects on a given level */
    int nhumanoids;
    int nbridges;
    int nbuildings;

    int nrockets;
    int nbigrockets;
    int njets;
    int nflak;
    int nfueltanks;
    int njammers;
    int ncron;
    int nships;
    int ngdbs;
    int noctopi;
    int ntentacles;
    int nballoons;
    int nsams;
    /* int nbombs; */
    /* int ngbombs; */
    int nairships;
    int nworms;
    int nkguns;

    int kgun_health;

    /* how often flak guns (laser turrets) fire. */
    int laser_fire_chance;

    /* how the terrain looks.  Starts out smooth, later levels more mountainous */
    double large_scale_roughness;
    double small_scale_roughness;
    int ground_color;
    int jetpilot_firechance;
} level = {
    /* initial values */
    31415927, /* This value is not used */
    0,
    NHUMANOIDS,
    NBRIDGES,
    NBUILDINGS,

    NROCKETS,
    NBIGROCKETS,
    NJETS,
    NFLAK,
    NFUELTANKS,
    NJAMMERS,
    NCRON,
    NSHIPS,
    NGDBS,
    NOCTOPI,
    NTENTACLES,
    NBALLOONS,
    NSAMS,
    /* NBOMBS, */
    /* NGBOMBS, */
    NAIRSHIPS,
    NWORMS,
    NKGUNS,
    KGUN_INIT_HEALTH,
    AVERAGE_LASER, 
    LARGE_SCALE_ROUGHNESS,
    SMALL_SCALE_ROUGHNESS,
    0,
    AGGRESSIVE_LASER,
};

/* vxy_2_dxy is a 2d array which translates a vx and vy velocity vextor
 * into an x,y vector in the same direction but a fixed magnitude.  The array
 * caches precomputed values.  This is used for drawing the missiles.  If the
 * missiles are travelling at vx, vy, then this array allows us to compute x,y
 * offsets to draw the missile pointing in the right direction with just a 
 * table lookup.  See init_vxy_2_dxy() function, below. */
#define MAX_VELOCITY_TO_COMPUTE 31 
#define V_MAGNITUDE (20.0)
struct my_point_t vxy_2_dxy[MAX_VELOCITY_TO_COMPUTE+1][MAX_VELOCITY_TO_COMPUTE+1];

/* these hold cached values of sine and cosine functions for 0-360 degress */
double sine[361];
double cosine[361];

/* Below, arrays of points telling how to draw the various objects in */
/* the game. */

struct my_point_t kgun_points[] = {

    { -10, -25 },
    { -10, -5 },
    { 10, -5 },
    { 10, -25 },
    { -10, -5 },
    { -7,  5 },
    { 7, 5 },
    { 10, -5 },
    { -10, -25 },
};

struct my_point_t inverted_kgun_points[] = {
    { -10, 25 },
    { -10, 5 },
    { 10, 5 },
    { 10, 25 },
    { -10, 5 },
    { -7,  -5 },
    { 7, -5 },
    { 10, 5 },
    { -10, 25 },
};

struct my_point_t tesla_points[] = {
    { -10, -25 },
    { -10, -5 },
    { 10, -5 },
    { 10, -25 },
    { -10, -5 },
    { -7,  5 },
    { 7, 5 },
    { 10, -5 },
    { -10, -25 },
    { LINE_BREAK, LINE_BREAK },
    { -7, 5 },
    { -2, 40 },
    { -5, 42 },
    { -2, 44 },
    {  2, 44 },
    {  5, 42 },
    {  2, 40 },
    {  7, 5 },
};

struct my_point_t octopus_points[] = {
    { 7, 0 },
    { 10, -5 },
    { 15, -25 },
    { 10, -30 },
    { -10, -30 },
    { -15, -25 },
    { -10, -5 },
    { -7, 0 },
    { LINE_BREAK, LINE_BREAK },
    { -8, -10 },
    { -4, -7 },
    { LINE_BREAK, LINE_BREAK },
    { 8, -10 },
    { 4, -7 }
};

struct my_point_t bullet_points[] = {
    { -4, 0 },
    { 0, -4 },
    { 4, 0 },
    { 0, 4 },
    { -4, 0 },
    { 4, 0 },
    { LINE_BREAK, LINE_BREAK },
    { 0, -4 },
    { 0, 4},
};

struct my_point_t gdb_points_left[] = {
    { 10, 0 },
    { 5, 0 },
    { -15, 5 },
    { -20, 0 },
    { -20, -5 },
    { 0, -15, },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, GREEN },

    { -20, -5 },
    { -18, -12 },
    { -10, -16 },
    { -3, -15 },
    { 0, -15, },

    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, CYAN },
    { 0, -15, },
    { 0, 15 },
    { -5, 20 },
    { -10, 20 },
    { LINE_BREAK, LINE_BREAK },
    { 15, -22 },
    { 0, -15 },
    { 0, 0 },
    { 15, 0  },
    { 15, -22  },
    { 15, -45  },
    { 18, -45  },
    { 18, -22  },
    { 18, 0  },
    { 33, 0  },
    { 33, -15  },
    { 18, -22  },
};

struct my_point_t gdb_points_right[] = {
    { 10, 0 },
    { 5, 0 },
    { -15, 5 },
    { -20, 0 },
    { -20, -5 },
    { 0, -15, },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, GREEN },

    { -20, -5 },
    { -18, -12 },
    { -10, -16 },
    { -3, -15 },
    { 0, -15, },

    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, CYAN },
    { 0, -15, },
    { 0, 15 },
    { -5, 20 },
    { -10, 20 },
    { LINE_BREAK, LINE_BREAK },
    { 15, -22 },
    { 0, -15 },
    { 0, 0 },
    { 15, 0  },
    { 15, -22  },
    { 15, -45  },
    { 18, -45  },
    { 18, -22  },
    { 18, 0  },
    { 33, 0  },
    { 33, -15  },
    { 18, -22  },
};

struct my_point_t humanoid_points[] = {
    { -1, 0 }, /* waist */
    {  1, 0 },
    {  4, -6 }, /* shoulders */
    {  -4, -6 },
    { -1, 0 }, 
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, CYAN },
    { -1, 0 }, /* left leg */
    { -3, 4 },
    { -3, 7 },
    { LINE_BREAK, LINE_BREAK },
    { 1, 0 }, /* right leg */
    { 3, 4 },
    { 3, 7 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, MAGENTA },
    { 4, -6, }, /* right arm */
    { 6, -3 },
    { 6, 1 }, 
    { LINE_BREAK, LINE_BREAK },
    { -4, -6, }, /* left arm */
    { -6, -3 },
    { -6, 1 }, 
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, ORANGE },
    { -1, -7 }, /* head */
    { -1, -9 },
    { 1, -9, },
    { 1, -7 },
};

struct my_point_t jetpilot_points_left[] = {
    { COLOR_CHANGE, GREEN },
    { 0, -8 },
    { 0,  0 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, CYAN },
    { 0,  0 },
    { -2, 5 },
    { 0, 9 },
    { LINE_BREAK, LINE_BREAK },
    { 2,  0 },
    { 0, 5 },
    { 2, 9 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, GREEN },
    /* { 0, 9 }, */
    /* { -2, 11 },  */
    { 2, 9 },
    { -1, 11 }, 
    { 0, 9 },
    { 2, 9 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, WHITE },
    { -2, -1 }, /* gun */
    { -5, -1 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, YELLOW },
    { 0, -8,  },
    { 2, -8,  },
    { 2, -5,  },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, ORANGE },
    { 2, -5,  },
    { 0, -5,  },
    { 0, -8,  },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, WHITE },
    { 2, -5,  }, /* jetpack */
    { 4, -5,  },
    { 4, -0,  },
    { 4, -0,  },
    { 2, -0,  },
    { 2, -5,  },
    { LINE_BREAK, LINE_BREAK },
    { 3,  0 }, /* nozzle */
    { 2,  2 },
    { 4,  2},
    { 3,  0},
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, GREEN },
    { 1,  -5 }, /* arm */
    { 3, -1 },
    { -2, 0 }, 
};

struct my_point_t jetpilot_points_right[ARRAY_SIZE(jetpilot_points_left)];

struct my_point_t SAM_station_points[] = {
    { -5, 0 },   /* Bottom base */
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, RED },
    { -5, 0 },   /* Bottom base */
    { -5, -10 },
    { 5, 0 },
    { 5, -10 },
    { -5, 0 },
    { 5, 0 },   
    { LINE_BREAK, LINE_BREAK },
    { -5, -10 },   /* middle base */
    { -5, -20 },
    { 5, -10 },
    { 5, -20 },
    { -5, -10 },
    { 5, -10 },     
    { LINE_BREAK, LINE_BREAK },
    { -5, -20 },   /* Top base */
    { -5, -30 },
    { 5, -20 },
    { 5, -30 },
    { -5, -20 },
    { 5, -20 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, WHITE },
    { -5, -30 },
    { 5, -30 },     
    { LINE_BREAK, LINE_BREAK },
    { -3, -30 }, /* Base of radar */
    { 0, -35, },
    { 3, -30 },
    { LINE_BREAK, LINE_BREAK },
    { 0, -35, }, /* Radar dish */
    { 10, -45 },
    { 13, -59 },
    { LINE_BREAK, LINE_BREAK },
    { 0, -35, }, /* Radar dish */
    { -10, -25, },
    { -25, -22, },
    { 13, -59 },
    { LINE_BREAK, LINE_BREAK },
    { 0, -35, }, /* Radar dish */
    { -15, -50, }, /* Radar dish */
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, WHITE },
    { 20, 0 }, /* Little building */
    { 20, -50 },
    { 30, -50 },
    { 30, 0 },

};

struct my_point_t SAM_station_debris_points[] = {
    { -5, 0 },   /* Bottom base */
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, RED },
    { -5, 0 },   /* Bottom base */
    { -5, -10 },
    { 5, 0 },
    { 5, -10 },
    { -5, 0 },
    { 5, 0 },   
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, WHITE },
    { 20, 0 }, /* Little building */
    { 20, -10 },
    { 30, -13 },
    { 30, 0 },
};

struct my_point_t airship_points[] = {
    { -70, -50 }, /* tip of nose */
    { -60, -60 },
    { -50, -65 },
    { -40, -67 },
    { -30, -70 }, /* top left */
    {  30, -70 }, /* top right */
    {  40, -67 },
    {  50, -65 },
    {  60, -60 },
    {  70, -50 },
    {  LINE_BREAK, LINE_BREAK },
    /* Now the same shape, but with y displacement to make bottom of blimp */
    { -70, -50 }, /* tip of nose */
    { -60, -40 },
    { -50, -35 },
    { -40, -33 },
    { -30, -30 }, /* bottom left */
    {  30, -30 }, /* bottom right */
    {  40, -33 },
    {  50, -35 },
    {  60, -40 },
    {  70, -50 }, /* back point */
    {  LINE_BREAK, LINE_BREAK },
    {  60, -60 }, /* top tail */
    {  70, -70 },
    {  90, -70 },
    {  90, -60 },
    {  70, -50 }, /* back point */
    {  90, -40 },
    {  90, -30 },
    {  70, -30 },
    {  60, -40 },
    {  LINE_BREAK, LINE_BREAK },  /* central tail fin */
    {  60, -50 } ,
    {  90, -50 },
    {  LINE_BREAK, LINE_BREAK },  /* Gondola */
    {  -10, -30 },
    {    -5, -20 },
    {  10, -20 },
    {  10, -30 },
};

struct my_point_t balloon_points[] = {
    { 0, -100 },
    { -20, -98 }, 
    { -35, -90 },
    { -45, -80 },
    { -47, -70 },
    { -47, -60 },
    { -40, -50 },
    { -8, -20 },
    { -8, -10 },
    {  8, -10 },
    {  8, -20 },
    {  40, -50 }, 
    {  47, -60 },
    {  47, -70 },
    {  45, -80 },
    {  35, -90 },
    {  20, -98 }, 
    { 0, -100 },
    {  LINE_BREAK, LINE_BREAK },  /* Gondola strings */
    { -8, -10 },
    { -5, 5, },
    { 5, 5, },
    { 8, -10 },
    {  LINE_BREAK, LINE_BREAK },  /* Gondola */
    { -5, 5,},
    { -5, 0,},
    {  5, 0,},
    {  5, 5,},
};


struct my_point_t spark_points[] = {
    { -1,-1 },
    { -1, 1},
    { 1, 1 },
    { 1, -1 },
    { -1, -1 },
#if 0
    { 0, 0 },
    { 0, 10 },
    { 10, 10 },
    { 10, 0 },
    { 0, 0 },
#endif
};

struct my_point_t cron_points[] = {
    { -10, -15 },   
    { 10, -15 },    
    { 15, -8 }, 
    { 10, 0 },  
    { -10, 0 }, 
    { -15, -8 },    
    { -10, -15 },   
    { LINE_BREAK, LINE_BREAK },
    { 0, 10 },
    { 7, 5 },
    { 7, 17 },
    { 10, 30 },
    { 7, 37 },
    { LINE_BREAK, LINE_BREAK },
    { 0, 10 },
    { -7, 5 },
    { -7, 17 },
    { -10, 30 },
    { -7, 37 },
    { LINE_BREAK, LINE_BREAK },
    { 0, 0 },
    { 0, 30 },
    { LINE_BREAK, LINE_BREAK },
    { 7, -15 },
    { 7, -25 },
    { LINE_BREAK, LINE_BREAK },
    { 9, -15 },
    { 9, -35 },
};

struct my_point_t jammer_points[] = {
    { -10, 0 },
    { 10, 0 },
    { 10, -10 },
    { 5, -10 },
    { 3, -15 },
    { -3, -15 },
    { -5, -10 },
    { -10, -10 },
    { -10, 0 },
};

struct my_point_t jammer_debris_points[] = {
    { -10, 0 },
    { 10, 0 },
    { 10, -6 },
    { 5, -2 },
    { 3, -7 },
    { -3, -0 },
    { -7, -3 },
    { -10, -4 },
    { -10, 0 },
};

struct my_point_t present_points[] = {
    { -10, -5 },
    { -10,  5 },
    {   0, 10 },
    {  10,  5 },
    {  10, -5 },
    {  0, -10 },
    { -10, -5 },
    {   0,  0 },
    {  10, -5 },
    { LINE_BREAK, LINE_BREAK },
    {   0,  0 },
    {   0, 10 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, RED },
    { -5, 7 },
    { -5, -3 },
    {  5, -7 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, RED },
    { 5, 7 },
    { 5, -3 },
    { -5, -7 },
};

struct my_point_t house_points[] = {
    { -30, -10 },
    { -30, 25 },
    { 30, 25 },
    { 30, -10},
    { LINE_BREAK, LINE_BREAK },
    { -5, 25 }, /* door */
    { -5, 0 },
    { 5, 0 },
    { 5, 25 },
    { LINE_BREAK, LINE_BREAK },
    { 13, 15 }, /* window */
    { 23, 15 },
    { 23, 5 },
    { 13, 5 },
    { 13, 15 },
    { LINE_BREAK, LINE_BREAK },
    { -20, -10 }, /* left front corner vertical */
    { -20, 25 },
    { LINE_BREAK, LINE_BREAK },
    { 20, -25 }, /* chimney */
    { 20, -30 },
    { 24, -30 },
    { 24, -25 }, 
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, RED },
    { 30, -10}, /* roof */
    { 25, -25},
    { -25, -25},
    { -30, -10 },
    { 30, -10 },
    { LINE_BREAK, LINE_BREAK },
    { -25, -25 }, /* roof */
    { -20, -10 },
};

struct my_point_t house_green_points[] = {
    { -30, -10 },
    { -30, 25 },
    { 30, 25 },
    { 30, -10},
    { LINE_BREAK, LINE_BREAK },
    { -5, 25 }, /* door */
    { -5, 0 },
    { 5, 0 },
    { 5, 25 },
    { LINE_BREAK, LINE_BREAK },
    { 13, 15 }, /* window */
    { 23, 15 },
    { 23, 5 },
    { 13, 5 },
    { 13, 15 },
    { LINE_BREAK, LINE_BREAK },
    { -20, -10 }, /* left front corner vertical */
    { -20, 25 },
    { LINE_BREAK, LINE_BREAK },
    { 20, -25 }, /* chimney */
    { 20, -30 },
    { 24, -30 },
    { 24, -25 }, 
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, GREEN },
    { 30, -10}, /* roof */
    { 25, -25},
    { -25, -25},
    { -30, -10 },
    { 30, -10 },
    { LINE_BREAK, LINE_BREAK },
    { -25, -25 }, /* roof */
    { -20, -10 },
};

struct my_point_t fuel_points[] = {
    { -30, -15 },

    { -25, -18 }, /* top far curve */
    { -20, -19 },
    {  20, -19 },
    {  25, -18 },

    { 30, -15 },

    {  25, -12 }, /* top near curve */
    {  20, -11 },
    { -20, -11 },
    { -25, -12 },
    { -30, -15 },

    { LINE_BREAK, LINE_BREAK },

    { 30, -15 },
    { 30, 30 },
    
    { 25, 33 }, /* bottom curve */
    { 20, 34 },
    { -20, 34 },
    { -25, 33 },

    { -30, 30 },
    { -30, -15 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, WHITE },
    { -25, 25 },
    { -25, -5 },
    { -15, -5 },
    { LINE_BREAK, LINE_BREAK },
    { -25, 15 },
    { -15, 15 },
    { LINE_BREAK, LINE_BREAK },
    { -10, -5 },
    { -10, 25 },
    { 0, 25 },
    { 0, -5 },
    { LINE_BREAK, LINE_BREAK },
    { 15, -5 },
    { 5, -5 },
    { 5, 25 },
    { 15, 25 },
    { LINE_BREAK, LINE_BREAK },
    { 5, 15 },
    { 10, 15 },
    { LINE_BREAK, LINE_BREAK },
    { 18, -5 },
    { 18, 25 },
    { 25, 25 },
};

struct my_point_t fuel_debris_points[] = {
    
    { 25, 30 },
    { 30, 25 },
    { 30, 30 },
    
    { 25, 33 }, /* bottom curve */
    { 20, 34 },
    { -20, 34 },
    { -25, 33 },

    { -30, 30 },
    { -30, 23 },
    { -27, 30 },
    { LINE_BREAK, LINE_BREAK },
    { -25, 30 },
    { -20, 20 },
    { -18, 27 },
    { -10, 22 },
    { -4,  29 },
    {  0,  23 },
    { 7, 30 },
    { 9, 24 },
    { 15, 28 },
    { 19, 16 },
    { 26, 27 },

         
};

struct my_point_t right_laser_beam_points[] = {
    { -100, 0 },
    { -80, 0 },
    { LINE_BREAK, LINE_BREAK },
    { -75, 0 },
    { -50, 0 },
    { LINE_BREAK, LINE_BREAK },
    { -45, 0 },
    { 0, 0 },
};

struct my_point_t player_ship_points[] = {
    { 9, 0 }, /* front of hatch */
    { 0,0 },
    { -3, -6 }, /* top of hatch */
    { -12, -12 },
    { -18, -12 },
    { -15, -4 }, /* bottom of tail */
    { -3, -4 }, /* bottom of tail */
    { -15, -4 }, /* bottom of tail */
    { -15, 3 }, /* back top of wing */
    { 0, 3 }, /* back top of wing */
    { -15, 3 }, /* back top of wing */
    { -18, 9 }, /* back bottom of wing */
    { -12, 9 }, /* back front of wing */
    { 0, 3 },
    { -6, 6 },
    { 20, 4 },
    { 24, 2 }, /* tip of nose */
    { 9, 0 }, /* front of hatch */
    { -3, -6 }, /* top of hatch */ 
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, RED },
    { 20, 2 },
    { -17, 2 }, /* red stripe */
#if 0
    { LINE_BREAK, LINE_BREAK }, /* Just for testing */
    { -30, -20 },
    { 30, -20 },
    { 30, 20 },
    { -30, 20 },
    { -30, -20 },
#endif
};

struct my_point_t sleigh_points[] = {
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, RED },
    { 2, -3 }, /* top of santa's thigh */
    { 5, -3 },
    { 6, 2 }, /* santa's shin */
    { 3, 2 },/* bottom of santa's pantleg */
    { LINE_BREAK, LINE_BREAK },
    { -5, -10 }, /* santa's back */
    { -3, -14 },
    { 0, -12 },
    { LINE_BREAK, LINE_BREAK }, /* santa's collar + beard */
    { COLOR_CHANGE, WHITE },
    { -3, -15 },
    { 0, -13 },
    { 4, -7 },
    { 2, -14 },
    { -3, -15 },
    { 1, -19 }, /* santa's hat brim */
    { LINE_BREAK, LINE_BREAK }, /* santa's face */
    { COLOR_CHANGE, ORANGE },
    { 1, -14 },
    { 1, -18 },
    { LINE_BREAK, LINE_BREAK }, /* santa's hat */
    { COLOR_CHANGE, RED },
    { -3, -16 },
    { 0, -20 },
    { -3, -21 },
    { -12, -18 },
    { -3, -16 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, WHITE }, /* santa's hat ball */
    { -12, -18 },
    { -12, -17 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, RED },
    { 0, -12 }, /* Santa's upper arm */
    { 3, -7 },
    { 6, -7 },
    { 6, -4 },
    { 0, -4 },
    { LINE_BREAK, LINE_BREAK }, /* santa's cuff */
    { COLOR_CHANGE, WHITE },
    { 7, -7 },
    { 7, -4 },
    { LINE_BREAK, LINE_BREAK }, /* santa's hand */
    { COLOR_CHANGE, GREEN },
    { 8, -7 },
    { 10, -7 },
    { 10, -5 },
    { 8, -5 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, WHITE }, /* sleigh's rails */
    { -20, 10 },
    { 20, 10 },
    { 23, 7 },
    { 23, 4 },
    { 21, 3 }, 
    { LINE_BREAK, LINE_BREAK },
    { -10, 10 },
    { -10, 5 },
    { LINE_BREAK, LINE_BREAK },
    { 10, 10 },
    { 10, 5 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, RED },
    { 3, 5 },
    { 0, -8 },
    { -25, -10 },
    { -20, 5 },
    { 13, 5 },
    { 15, 3 },
    { 13, -10 },
    { 15, -12 },
    { 17, -12 },
    { 18, -10 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, GREEN },
    { -23, -10 },
    { -15, -15 },
    { -12, -16 },
    { -9, -15 },
    { -7, -10 },
};

struct my_point_t left_sleigh_points[ARRAY_SIZE(sleigh_points)];

struct my_point_t reindeer_points[] = {
    { -5, 13 },
    { -7, 8 },
    { -5, 3 },
    { 0, 5 },
    { 5, 3 },
    { 7, 8 },
    { 5, 13 },
    { LINE_BREAK, LINE_BREAK },
    { 5, 3 },
    { 9, -4 },
    { 14, -4 },
    { 14, -6 },
    { 9, -8 },
    { 9, -12 },
    { LINE_BREAK, LINE_BREAK },
    { 9, -6 },
    { 9, -6 },
    { 5, -2 },
    { -5, -2 },
    { -7, -4 },
    { -7, -8 },
    { LINE_BREAK, LINE_BREAK },
    { -5, -2 },
    { -6, -1 },
    { -5, 3 },
};

struct my_point_t reindeer_points2[] = {
    { -2, 12 },
    { -5, 8 },
    { -5, 3 },
    { 0, 5 },
    { 5, 3 },
    { 5, 8 },
    { 2, 12 },
    { LINE_BREAK, LINE_BREAK },
    { 5, 3 },
    { 9, -4 },
    { 14, -4 },
    { 14, -6 },
    { 9, -8 },
    { 9, -12 },
    { LINE_BREAK, LINE_BREAK },
    { 9, -6 },
    { 9, -6 },
    { 5, -2 },
    { -5, -2 },
    { -7, -4 },
    { -7, -8 },
    { LINE_BREAK, LINE_BREAK },
    { -5, -2 },
    { -6, -1 },
    { -5, 3 },
};

struct my_point_t left_reindeer_points2[] = {
    { -2, 12 },
    { -5, 8 },
    { -5, 3 },
    { 0, 5 },
    { 5, 3 },
    { 5, 8 },
    { 2, 12 },
    { LINE_BREAK, LINE_BREAK },
    { 5, 3 },
    { 9, -4 },
    { 14, -4 },
    { 14, -6 },
    { 9, -8 },
    { 9, -12 },
    { LINE_BREAK, LINE_BREAK },
    { 9, -6 },
    { 9, -6 },
    { 5, -2 },
    { -5, -2 },
    { -7, -4 },
    { -7, -8 },
    { LINE_BREAK, LINE_BREAK },
    { -5, -2 },
    { -6, -1 },
    { -5, 3 },
};

struct my_point_t socket_points[] = {
    { 9, 0 }, /* front of hatch */
    { -3, -6 }, /* top of hatch */
    { -12, -12 },
    { -18, -12 },
    { -15, -4 }, /* bottom of tail */
    /* { -3, -4 }, */ /* bottom of tail */
    /* { -15, -4 },*/ /* bottom of tail */
    { -15, 3 }, /* back top of wing */
    /*{ 0, 3 },*/ /* back top of wing */
    /*{ -15, 3 },*/ /* back top of wing */
    { -18, 9 }, /* back bottom of wing */
    { -12, 9 }, /* back front of wing */
    /* { LINE_BREAK, LINE_BREAK }, */
    /* { 0, 3 }, */
    { -6, 6 },
    { 20, 4 },
    { 24, 2 }, /* tip of nose */
    { 9, 0 }, /* front of hatch */
    { LINE_BREAK, LINE_BREAK },
    { -30, -25 },
    { 30, -25 },
    { 30, 25 },
    { -30, 25 },
    { -30, -25 },
    /* { -3, -6 },*/ /* top of hatch */ 
#if 0
    { LINE_BREAK, LINE_BREAK }, /* Just for testing */
    { -30, -20 },
    { 30, -20 },
    { 30, 20 },
    { -30, 20 },
    { -30, -20 },
#endif
};

struct my_point_t left_player_ship_points[] = {
    /* Data is reversed algorithmically */
    { 9, 0 }, /* front of hatch */
    { 0,0 },
    { -3, -6 }, /* top of hatch */
    { -12, -12 },
    { -18, -12 },
    { -15, -4 }, /* bottom of tail */
    { -3, -4 }, /* bottom of tail */
    { -15, -4 }, /* bottom of tail */
    { -15, 3 }, /* back top of wing */
    { 0, 3 }, /* back top of wing */
    { -15, 3 }, /* back top of wing */
    { -18, 9 }, /* back bottom of wing */
    { -12, 9 }, /* back front of wing */
    { 0, 3 },
    { -6, 6 },
    { 20, 4 },
    { 24, 2 }, /* tip of nose */
    { 9, 0 }, /* front of hatch */
    { -3, -6 }, /* top of hatch */ 
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, RED },
    { 20, 2 },
    { -17, 2 }, /* red stripe */
    
#if 0
    { LINE_BREAK, LINE_BREAK }, /* Just for testing */
    { -30, -20 },
    { 30, -20 },
    { 30, 20 },
    { -30, 20 },
    { -30, -20 },
#endif
};

struct my_point_t bomb_points[] = {
    { -2, -6 },
    { 2, -6 },
    { 2, -4 },
    { 0, -3 },
    { -2, -4 },
    { -2, -6 },
    { LINE_BREAK, LINE_BREAK },
    { 0, -3 },
    { 2, -1 },
    { 2, 8 },
    { 0, 9 },
    { -2, 8 },
    { -2, -1 },
    { 0, -3 },
};

struct my_point_t big_rocket_points[] = {
    { 0, -35 },
    { -5, -25 },
    { -5, 5 },
    { 0, 15 },
    { 5, 5 },
    { 5, -25 },
    { 0, -35 },
    { LINE_BREAK, LINE_BREAK },
    { -5, 5 },
    { -10, 15 },
    { 10, 15 },
    { 5, 5 },
};

struct my_point_t rocket_points[] = {
    { -2, 3 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, RED },
    { -2, 3 },
    { -4, 7 },
    { -2, 7 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, WHITE },
    { -2, 7 },
    { -2, -14 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, RED },
    { -2, -14 },
    { 0, -20 },
    { 2, -14 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, WHITE },
    { 2, -14 },
    { 2, 7},
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, RED },
    { 2, 7},
    { 4, 7},
    { 2, 3}, 
};

#ifdef OPENLASE
#define BRIDGE_SIZE 20 
#else
#define BRIDGE_SIZE 8 
#endif

#define BRIDGEOFF (BRIDGE_SIZE / 2)

struct my_point_t bridge_points[] = {  /* square with an x through it, 8x8 */
    { -BRIDGEOFF, BRIDGEOFF },
    { -BRIDGEOFF, -BRIDGEOFF },
    { BRIDGEOFF, -BRIDGEOFF },
    { BRIDGEOFF, BRIDGEOFF },
//#ifndef OPENLASE
    { -BRIDGEOFF, BRIDGEOFF },
    { BRIDGEOFF, -BRIDGEOFF },
#ifndef OPENLASE
    { LINE_BREAK, LINE_BREAK },
    { -BRIDGEOFF, -BRIDGEOFF },
    { BRIDGEOFF, BRIDGEOFF },
#endif
};

struct my_point_t jet_points[] = {
    { -20, 0 },
    { -13, -3 },
    { -10, -5 },
    { 20, -3 },
    { 35, -15 },
    { 43, -15 },
    { 37, -3 },
    { 37, 0 },
    { -10, 2 },
    { -20, 0 },
    { LINE_BREAK, LINE_BREAK },
    { 37, -3 },
    { 20, -3 },
};

struct my_point_t ships_hull_points[] = {
    /* bowsprit */
    { -60, -14 },
    { -15, -10 },
    {  -15, -5 },
    {   21, -5 },
    {   25, -13 },
    {  60, -15 },
    {  55, 0 },
    {  51, -1 },
    {  51, 20 },
    {  -12, 15 },
    {  -27, 0 },
    {  -30, -7 },
    {  -60, -14 },  
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, GREEN }, /* wine bottle */
    { -135, -120 }, 
    { 110, -120 }, 
    { 110, 23 }, 
    { -135, 23 }, 
    { -160, 5 }, 
    { -175, -25 }, 
    { -300, -25 }, 
    { -300, -65 }, 
    { -175, -65 }, 
    { -160, -102 }, 
    { -135, -120 }, 
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, ORANGE },
    { 51, 15 },
    { -12, 15 }, /* keel */
    { LINE_BREAK, LINE_BREAK },
    { 5, -5 },
    { 8, -110 }, /* main mast */
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, RED },
    { 8, -110 }, /* main mast */
    { 0, -108 }, /* flag */
    { 8, -105 },
    
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, ORANGE },
    { 39, -14 }, /* aft mast */
    { 41, -95 },
    { LINE_BREAK, LINE_BREAK },
    { -22, -12 }, /* fore mast */
    { -20, -83 },
    { LINE_BREAK, LINE_BREAK },
    { COLOR_CHANGE, WHITE },
    { -20, -83 },
    { -60, -14 }, /* fore sail */
    { -30, -25 },
    { -20, -83 },
    { 8, -100 }, /* lines on top */
    { 41, -90 },
    { 60, -70 },
    { LINE_BREAK, LINE_BREAK },
    { 39, -20 }, /* aft boom */
    { 75, -20 },
    { LINE_BREAK, LINE_BREAK },
    { 39, -60 }, /* upper aft boom */
    { 60, -70 },
    { 75, -20 },
    { LINE_BREAK, LINE_BREAK },
    { -30, -75 }, /* foretopsail */
    { -14, -75 },
    { -17, -68 },
    { -14, -58 },
    { -27, -61 },
    { -30, -58 },
    { -32, -65 },
    { -30, -75 },  
    { LINE_BREAK, LINE_BREAK },
    { -30, -58 }, /* foremidsail */
    { -14, -58 },
    { -14, -49 },
    { -10, -40 },
    { -24, -45 },
    { -35, -40 },
    { -36, -47 },
    { -30, -58 },
    { LINE_BREAK, LINE_BREAK },
    { -35, -40 }, /* forelowersail */
    { -10, -40 },
    { -10, -26 },
    { -5, -12 },
    { -25, -17 },
    { -39, -12 },
    { -40, -22 },
    { -35, -40 },
    { LINE_BREAK, LINE_BREAK },
    { -5, -100 }, /* main top sail */
    {  20, -100 },
    {  20, -95 },
    {  25, -80 },
    {  20, -83 },
    {  -6, -83 },
    { -10, -80 },
    { -10, -85 },
    { -5, -100 },
    { LINE_BREAK, LINE_BREAK },
    {  25, -80 }, /* 2nd main sail */
    { -10, -80 },
    { -15, -63 },
    { -15,  -57 },
    { -10,  -60 },
    {  25,  -60 },
    {  30,  -57 },
    {  25,  -75 },
    {  25, -80 },
    { LINE_BREAK, LINE_BREAK },
    { -15,  -57 }, /* 3rd main sail */
    {  30,  -57 },
    {  30,  -50 },
    {  33,  -37 },
    {  28,  -40 },
    {  -13,  -40 },
    {  -18,  -37 },
    {  -18,  -43 },
    { -15,  -57 },
    { LINE_BREAK, LINE_BREAK },
    {  33,  -37 }, /* 4th main sail */
    {  -18,  -37 },
    {  -20,  -18 },
    {  -20,  -13 },
    {  -15,  -17 },
    {  30,  -17 },
    {  35,  -13 },
    {  33,  -32 },
    {  33,  -37 },
    { LINE_BREAK, LINE_BREAK },
    {  28,  -90 }, /* aft top sail */
    {  52,  -90 },
    {  52,  -85 },
    {  55,  -70 },
    {  50,  -73 },
    {  30,  -73 },
    {  25,  -70 },
    {  25,  -75 },
    {  28,  -90 },
    { LINE_BREAK, LINE_BREAK },
    {  55,  -70 },
    {  25,  -70 },
    {  22,  -55 },
    {  22,  -50 },
    {  25,  -53 },
    {  52,  -53 },
    {  57,  -50 },
    {  55,  -65 },
    {  55,  -70 },
    { LINE_BREAK, LINE_BREAK },
    {  22,  -50 },
    {  57,  -50 },
    {  57,  -45 },
    {  60,  -30 },
    {  55,  -33 },
    {  25,  -33 },
    {  20,  -30 },
    {  20,  -35 },
    {  22,  -50 },
    
};

/* These debris things will get filled in procedurally */
/* these drawings are used when something explodes, to add */
/* flying debris objects into the explosion. See make_debris_forms().  */
#define NDEBRIS_FORMS 25
struct my_point_t *debris_point[NDEBRIS_FORMS] = { 0 };
struct my_vect_obj *debris_vect[NDEBRIS_FORMS] = { 0 };

/* Just a bunch of random snippets of lisp code for the emacs */
/* blimps to spew out into core memory space as "memory leaks" */
/* wish I could find some humorous lisp code, but, */
/* who the hell would even recognize it? */
char randomlisp[] = "(let ((arg (if (consp (car args)) (caar args) (car args)))"
    "(cond ((= chr ?s)"
    "(if addr (concat (car addr) \" <\" (cdr addr) \">\")"
    "(or (and (boundp 'report-xemacs-bug-beta-address)"
    "report-xemacs-bug-beta-address)"
    "(if (eq (car-safe calc-last-kill) (car kill-ring-yank-pointer))"
    "(cdr calc-last-kill)"
    "(if (stringp (car kill-ring-yank-pointer))"
    "(semantic-bovinate-from-nonterminal-full" /* semantic-bovinate? */
    "   (if value (list (car value) (cdr value))))"
    ":value-to-external (lambda (widget value)"
    "(if value (append (list (car value)) (cadr value)))))";

/* contains instructions on how to draw all the objects */
struct my_vect_obj player_vect;
struct my_vect_obj left_player_vect;
struct my_vect_obj reindeer2_vect;
struct my_vect_obj left_reindeer2_vect;
struct my_vect_obj sleigh_vect;
struct my_vect_obj left_sleigh_vect;
struct my_vect_obj rocket_vect;
struct my_vect_obj big_rocket_vect;
struct my_vect_obj jet_vect;
struct my_vect_obj spark_vect;
struct my_vect_obj right_laser_vect;
struct my_vect_obj fuel_vect;
struct my_vect_obj fuel_debris_vect;
struct my_vect_obj house_vect;
struct my_vect_obj present_vect;
struct my_vect_obj house_green_vect;
struct my_vect_obj jammer_vect;
struct my_vect_obj jammer_debris_vect;
struct my_vect_obj cron_vect;
struct my_vect_obj ship_vect;
struct my_vect_obj bomb_vect;
struct my_vect_obj bridge_vect;
struct my_vect_obj kgun_vect;
struct my_vect_obj inverted_kgun_vect;
struct my_vect_obj airship_vect;
struct my_vect_obj balloon_vect;
struct my_vect_obj SAM_station_vect;
struct my_vect_obj SAM_station_debris_vect;
struct my_vect_obj humanoid_vect;
struct my_vect_obj jetpilot_vect_right;
struct my_vect_obj jetpilot_vect_left;
struct my_vect_obj socket_vect;
struct my_vect_obj bullet_vect;
struct my_vect_obj gdb_vect_right;
struct my_vect_obj gdb_vect_left;
struct my_vect_obj octopus_vect;
struct my_vect_obj ships_hull;
struct my_vect_obj tesla_vect;

/* There are 4 home-made "fonts" in the game, all the same "typeface", but 
 * different sizes */
struct my_vect_obj **gamefont[4];
/* indexes into the gamefont array */
#define BIG_FONT 0
#define SMALL_FONT 1
#define TINY_FONT 2
#define NANO_FONT 3

/* sizes of the fonts... in arbitrary units */
#define BIG_FONT_SCALE 14 
#define SMALL_FONT_SCALE 5 
#define TINY_FONT_SCALE 3 
#define NANO_FONT_SCALE 2 

/* spacing of letters between the fonts, pixels */
#define BIG_LETTER_SPACING (10)
#define SMALL_LETTER_SPACING (5)
#define TINY_LETTER_SPACING (3)
#define NANO_LETTER_SPACING (2)

/* max text lines that fit on the screen, used in attract mode */
#define MAXTEXTLINES 20
/* State of the game text printing system for the attract mode */
int current_color = WHITE;
int current_font = BIG_FONT;
int cursorx = 0;
int cursory = 0;
int livecursorx = 0;
int livecursory = 0;

/* for getting at the font scales and letter spacings, given  only font numbers */
int font_scale[] = { BIG_FONT_SCALE, SMALL_FONT_SCALE, TINY_FONT_SCALE, NANO_FONT_SCALE };
int letter_spacing[] = { BIG_LETTER_SPACING, SMALL_LETTER_SPACING, TINY_LETTER_SPACING, NANO_LETTER_SPACING };

/* more stuff for the attract mode's idea of how the "text screen" works. */
int ntextlines = 0;
struct text_line_t {
    int x, y, font;
    char string[80];
} textline[20];

/* This is just stored in memory... I'm too lazy to make this */
/* run setgid in order to write it out to a "games" owned score file, and */
/* anyway, this is an old school shooter -- when you pull the plug on the */
/* (imagined) coin op machine, high scores are cleared.  Big deal. */
#define MAXHIGHSCORES 10
struct score_data {
    char name[4];
    int score;
} highscore[MAXHIGHSCORES];
int highscore_letter = 0;
int highscore_buttonpress = 0;
char highscore_initials[4];
int highscore_current_initial = 0;
int autopilot_mode = 0;
int high_score_file_descriptor = -1; /* not currently implemented. */

#define FINAL_MSG1 "Where is your"
#define FINAL_MSG2 "editor now???"

/* text line entries are just fixed... */
#define GAME_OVER 1
#define CREDITS 0

/* I can switch out the line drawing function with these macros */
/* in case I come across something faster than gdk_draw_line */
#define DEFAULT_LINE_STYLE current_draw_line
#define DEFAULT_RECTANGLE_STYLE current_draw_rectangle
#define DEFAULT_BRIGHT_LINE_STYLE current_bright_line

#define wwvi_draw_line DEFAULT_LINE_STYLE
#define wwvi_draw_rectangle DEFAULT_RECTANGLE_STYLE
#define wwvi_bright_line DEFAULT_BRIGHT_LINE_STYLE

/* #define wwvi_draw_line gdk_draw_line */
/* #define wwvi_draw_rectangle gdk_draw_rectangle */

float xscale_screen;
float yscale_screen;
int real_screen_width;
int real_screen_height;

#ifdef OPENLASE
static int last_x = -1;
static int last_y = -1;
static int openlase_line_count = 0;
#ifndef OPENLASEGRAYSCALE
static int openlase_color = 0;

void gdk_gc_set_foreground_hook(GdkGC *gc, const GdkColor *color)
{
    gdk_gc_set_foreground(gc, color);
    openlase_color = ((color->blue) >> 8) +
             (((color->green) >> 8) << 8) +
             (((color->red) >> 8)<< 16);
}
#define gdk_gc_set_foreground(x, y) gdk_gc_set_foreground_hook(x, y)
#else

static int openlase_color = 0x5f;

#endif /* not OPENLASEGRAYSCALE */

#define DEBRIS_AMOUNT 3
#else
#define DEBRIS_AMOUNT 16
#endif

#ifdef OPENLASE

#define olVertex

#define FIDDLE_OFFSET 4000

int min = 1000000;
int max = -1000000;
static void openlase_drawline(int x1, int y1, int x2, int y2, int color)
{
    if ( openlase_line_count > LASERMAXLINES ) 
        return;

#define MIN(a,b) ((a<=b)?(a):(b))

#define mima(a) {min = MIN(min, a);max = MAX(max, a);}

//    mima(x1);mima(x2);mima(y1);mima(y2);
    
//printf("Line: min:%i, max%i \n", min, max);
//printf("Pre Line: %i,%i -> %i,%i \n", (int)x1, (int)y1, (int)x2, (int)y1);    

//    x1 = (x1<<5) -32768/2+FIDDLE_OFFSET;
//    x2 = (x2<<5) -32768/2+FIDDLE_OFFSET;
//    y1 = ((y1<<5)*1.333) -32768/2-FIDDLE_OFFSET;
//    y2 = ((y2<<5)*1.333) -32768/2-FIDDLE_OFFSET;
    
    x1 = x1*30-12000;
    x2 = x2*30-12000;
    y1 = y1*44-14000;
    y2 = y2*44-14000;
    
//printf("Post Line: %i,%i -> %i,%i \n", (int)x1, (int)-y1, (int)x2, (int)-y1);    
    
    v_directDraw32((int)x1, (int)-y1, (int)x2, (int)-y2, color);
    ++openlase_line_count;
}
static void openlase_rectangle(int x1, int y1, int width, int height)
{
    int x2 = x1 + width;
    int y2 = y1 + height;

    openlase_drawline(x1, y1, x2, y1, 0x5f);
    openlase_drawline(x2, y1, x2, y2, 0x5f);
    openlase_drawline(x2, y2, x1, y2, 0x5f);
    openlase_drawline(x1, y2, x1, y1, 0x5f);
}

#else
#define openlase_drawline(a, b, c, d, e)
#define openlase_rectangle(a, b, c, d)
#endif

static inline void openlase_startframe(void)
{
#ifdef OPENLASE
    openlase_line_count = 0;
#endif
}

static inline void openlase_renderframe(void)
{
#ifdef OPENLASE
#endif
}

static void openlase_shutdown(void)
{
#ifdef OPENLASE
#endif
}



typedef void line_drawing_function(gint x1, gint y1, gint x2, gint y2);
typedef void bright_line_drawing_function(gint x1, gint y1, gint x2, gint y2, int color);
typedef void rectangle_drawing_function(gboolean filled, gint x, gint y, gint width, gint height);
typedef void explosion_function(int x, int y, int ivx, int ivy, int v, int nsparks, int time);
typedef void add_spark_function(int x, int y, int vx, int vy, int time);

void unscaled_bright_line(gint x1, gint y1, gint x2, gint y2, int color);

void __explosion_function(int x, int y, int ivx, int ivy, int v, int nsparks, int time)
{
  printf("WARNING!!!\nUnhandled function: explosion_function();");
}


#define gdk_gc_set_foreground(a, b)

void vec_draw_line_count(gint x1, gint y1, gint x2, gint y2);
void vec_draw_rectangle(gboolean filled, int x, int y, int width, int height);

unsigned long total_line_count = 0;
line_drawing_function *current_draw_line = vec_draw_line_count;
rectangle_drawing_function *current_draw_rectangle = vec_draw_rectangle;
bright_line_drawing_function *current_bright_line = unscaled_bright_line;
explosion_function *explosion = __explosion_function;
explosion_function *bright_explosion = __explosion_function;




void vec_draw_line_count(gint x1, gint y1, gint x2, gint y2)
{
    total_line_count++; 
    openlase_drawline(x1, y1, x2, y2,0x5f);
}

void vec_draw_rectangle(__attribute__ ((unused)) gboolean filled, int x, int y, int width, int height)
{
    openlase_rectangle(x, y, width, height);
    total_line_count+=4; 
}

void scaled_line(gint x1, gint y1, gint x2, gint y2)
{
//  gdk_draw_line(drawable, gc, x1*xscale_screen, y1*yscale_screen, x2*xscale_screen, y2*yscale_screen);
    openlase_drawline(x1*xscale_screen, y1*yscale_screen, x2*xscale_screen, y2*yscale_screen,0x5f);
}

void thick_scaled_line(gint x1, gint y1, gint x2, gint y2)
{
    int sx1,sy1,sx2,sy2,dx,dy;

    if (abs(x1-x2) > abs(y1-y2)) {
        dx = 0;
        dy = 1;
    } else {
        dx = 1;
        dy = 0;
    }
    sx1 = x1*xscale_screen;
    sx2 = x2*xscale_screen;
    sy1 = y1*yscale_screen; 
    sy2 = y2*yscale_screen; 
    
//  gdk_draw_line(drawable, gc, sx1,sy1,sx2,sy2);
//  gdk_draw_line(drawable, gc, sx1-dx,sy1-dy,sx2-dx,sy2-dy);
//  gdk_draw_line(drawable, gc, sx1+dx,sy1+dy,sx2+dx,sy2+dy);
    openlase_drawline(sx1, sy1, sx2, sy2,0x5f);
}

void scaled_rectangle(gboolean filled, gint x, gint y, gint width, gint height)
{
    openlase_rectangle(x * xscale_screen, y * yscale_screen, width * xscale_screen, height * yscale_screen); 
}

void scaled_bright_line(gint x1, gint y1, gint x2, gint y2, int color)
{
    int sx1,sy1,sx2,sy2,dx,dy;

    if (abs(x1-x2) > abs(y1-y2)) {
        dx = 0;
        dy = 1;
    } else {
        dx = 1;
        dy = 0;
    }
    sx1 = x1*xscale_screen;
    sx2 = x2*xscale_screen;
    sy1 = y1*yscale_screen; 
    sy2 = y2*yscale_screen; 
    
//  gdk_gc_set_foreground(gc, &huex[WHITE]);
//  gdk_draw_line(drawable, gc, sx1,sy1,sx2,sy2);
//  gdk_gc_set_foreground(gc, &huex[color]);
//  gdk_draw_line(drawable, gc, sx1-dx,sy1-dy,sx2-dx,sy2-dy);
//  gdk_draw_line(drawable, gc, sx1+dx,sy1+dy,sx2+dx,sy2+dy);
    openlase_drawline(x1, y1, x2, y2, 0x7f);
}

void unscaled_bright_line(gint x1, gint y1, gint x2, gint y2, int color)
{
    int dx,dy;

    if (abs(x1-x2) > abs(y1-y2)) {
        dx = 0;
        dy = 1;
    } else {
        dx = 1;
        dy = 0;
    }
    
//  gdk_gc_set_foreground(gc, &huex[WHITE]);
//  gdk_draw_line(drawable, gc, x1,y1,x2,y2);
//  gdk_gc_set_foreground(gc, &huex[color]);
//  gdk_draw_line(drawable, gc, x1-dx,y1-dy,x2-dx,y2-dy);
//  gdk_draw_line(drawable, gc, x1+dx,y1+dy,x2+dx,y2+dy);
    openlase_drawline(x1, y1, x2, y2, 0x7f);
}

void crazy_line(gint x1, gint y1, gint x2, gint y2)
{
    int angle;

    int x3,y3, x4,y4;
    angle = timer % 360;

    x3 = ((x1 - (SCREEN_WIDTH/2)) * sine[angle]) + SCREEN_WIDTH/2;
    x4 = ((x2 - (SCREEN_WIDTH/2)) * sine[angle]) + SCREEN_WIDTH/2;
    y3 = ((y1 - (SCREEN_HEIGHT/2)) * cosine[angle]) + SCREEN_HEIGHT/2;
    y4 = ((y2 - (SCREEN_HEIGHT/2)) * cosine[angle]) + SCREEN_HEIGHT/2;
    openlase_drawline(x3, y3, x4, y4,0x5f);
}

void crazy_rectangle(gint x, gint y, gint width, gint height)
{
    int x3, y3, x4, y4;

    x3 = x;
    y3 = y;
    x4 = x + width;
    y4 = y + height;

    crazy_line(x3, y3, x4, y3);
    crazy_line(x3, y4, x4, y4);
    crazy_line(x3, y3, x3, y4);
    crazy_line(x4, y3, x4, y4);
}

void init_vxy_2_dxy()
{
    int x, y;
    double dx, dy, angle;

    /* Given a velocity, (vx, vy), precompute offsets */
    /* for a fixed magnitude */

    for (x=0;x<=MAX_VELOCITY_TO_COMPUTE;x++) {
        for (y=0;y<=MAX_VELOCITY_TO_COMPUTE;y++) {
            if (x == 0) {
                vxy_2_dxy[x][y].y = 
                    ((double) -y / (double) MAX_VELOCITY_TO_COMPUTE) * V_MAGNITUDE;
                vxy_2_dxy[x][y].x = 0;
                continue;
            }
            dx = x;
            dy = y;
            angle = atan(dy/dx);
            dx = cos(angle) * V_MAGNITUDE;
            dy = -sin(angle) * V_MAGNITUDE;
            vxy_2_dxy[x][y].x = (int) dx;
            vxy_2_dxy[x][y].y = (int) dy;
        }
    }
}

/* These two functions use the vxy_2_dxy array, but correct for negative x or y */
static inline int dx_from_vxy(int vx, int vy) 
{
    if ((abs(vx) > MAX_VELOCITY_TO_COMPUTE) || (abs(vy) > MAX_VELOCITY_TO_COMPUTE))
        return 0;
    return (vx < 0) ?  -vxy_2_dxy[abs(vx)][abs(vy)].x : vxy_2_dxy[abs(vx)][abs(vy)].x;
}

static inline int dy_from_vxy(int vx, int vy) 
{
    if ((abs(vx) > MAX_VELOCITY_TO_COMPUTE) || (abs(vy) > MAX_VELOCITY_TO_COMPUTE))
        return 0;
    return (vy < 0) ?  -vxy_2_dxy[abs(vx)][abs(vy)].y : vxy_2_dxy[abs(vx)][abs(vy)].y;
}


/* attract mode text screen related functions */
void set_font(int fontnumber)
{
    current_font = fontnumber;
}

void gotoxy(int x, int y)
{
    cursorx = (x+1) * font_scale[current_font]*3;
    cursory = (y+1) * font_scale[current_font]*7;
}

void cleartext()
{
    ntextlines = 0;
}

void gameprint(char *s)
{
    int n;

    /* printf("Printing '%s'\n", s); */
    n = ntextlines;
    if (n>=MAXTEXTLINES)
        n = 0;
    textline[n].x = cursorx;
    textline[n].y = cursory;
    textline[n].font = current_font;
    strcpy(textline[n].string, s);
    ntextlines++;
    if (ntextlines >=MAXTEXTLINES)
        ntextlines = MAXTEXTLINES-1;
}

int current_font_scale = BIG_FONT_SCALE;

/* every object in the game is a game_obj_t, defined below... */
struct game_obj_t;

typedef void (*get_coords_func)(struct game_obj_t *parent, struct game_obj_t *child,  int *x, int *y);

/* some function pointers which game_obj_t's may have */
typedef void obj_move_func(struct game_obj_t *o);       /* moves and object, called once per frame */
typedef void obj_draw_func(struct game_obj_t *o); /* draws object, called 1/frame, if onscreen */
typedef void obj_destroy_func(struct game_obj_t *o);        /* called when an object is killed */

/* Various object type specific data structures are defined below. */
/* these are all put into a union in the game_obj_t structure. */

struct extra_player_data {  /* these are used when drawing the player zooming into the game */
    int count;      /* at the beginning of levels. */
    int count2;
};

struct reindeer_data {
    int count; /* must match extra_player_data */
    int count2;
    int direction;
    int sleigh;
    int rudolph;
}; 

struct harpoon_data {       
    struct game_obj_t *gdb; /* which gdb this harpoon came from */
};

struct octopus_data {
    int awake;          /* is octopus awake? */
    int tx, ty;         /* target x and y to which he's moving, in game coords */
    struct game_obj_t *tentacle[8]; /* the octopus's tentacles are separate objs. */
};

struct gdb_data {
    int awake;          /* is gdb awake? */
    int tx, ty;         /* target x and y, game coords */
};

struct cron_data {
    int beam_speed;         /* speed at which beam is sweeping across ground. */
    int beam_pos;           /* x coord, in game coords of beam */
    int tmp_ty_offset;      /* if destination is a long way off, ty may be offset randomly */
                    /* this keeps cron job from just sitting at a constant altitude */
                    /* which would make him an easy target */
    int tx, ty;         /* destination x,y, in game coords */
    int eyepos;         /* the cron job has a little eye, this traks the eye's movement */
    struct game_obj_t *myhuman; /* the vi .swp file which the cron job is cleaning up (abducting) */

    int pissed_off_timer;
    int state;          /* initially, SEEKING_HUMAN, then when he finds a human */
                    /* he carries him to the volcano, then seeks another human. */
                    /* occasionally he'll get pissed off at the player and chase */
                    /* him for awhile (until pissed_off_timer gets to zero. */
                    /* then, depending if he's carrying a human, back to seeking, */
                    /* or back to the volcano. */
#define CRON_STATE_SEEKING_HUMAN 1
#define CRON_STATE_CARRYING_HUMAN_TO_VOLCANO 2
#define CRON_STATE_PISSED_OFF 3
};

struct tentacle_seg_data {
    int angle;          /* angle of this tentacle segment relative to previous one, in degrees */
    int length;         /* length of tentacle seg, pixels */
    int angular_v;          /* angular velocity, degrees/frame */
    int dest_angle;         /* target angle towards which tentacle seg is moving. */
};

struct tentacle_data {
    struct game_obj_t *attached_to; /* the octopus's head */
    int angle;          /* angle of attachment of first segment */
    int nsegs;          /* how many segments? */
    int upper_angle, lower_angle;   /* limits on attachment angle (keeps tentacles mostly pointing down) */
                    /* array of tentacle segments */
    int color_scheme;
    int other_color;
    struct tentacle_seg_data seg[MAX_TENTACLE_SEGS];    
};

struct floating_message_data {      /* for those floating text messages, like "Help!", "Woohoo!", and scores */
    int font;
    unsigned char msg[21];
};

struct human_data {
    struct game_obj_t *abductor;    /* cron job which has abducted him */
    int picked_up;          /* is he currently picked up? */
    int on_ground;          /* is he on the ground? */
    int human_number;       /* there are a number of humans, he must know which one he is. */
    int seat_number;
};

struct jammer_data {            /* these are used in the drawing of radar jammers */
    int width;          /* the spinning antenna uses these values. */
    int direction;          /* expanding or contracting */
};

struct fuel_data {
    int level;          /* how much fuel is in the fuel tank? */
};

struct airship_data {
    int bannerline;         /* blimps have scrolling text on the side */
                    /* bannerline keeps track of where in the */
                    /* script of text that scrolls by we are. */
    int pressure;           /* shooting at the blimps increases the */
                    /* pressure, makes the blimp shoot out more */
                    /* leaking lisp code, and makes it fatter */
};

struct worm_data {
#define NWORM_TIME_UNITS 5 
    struct game_obj_t *parent;  /* next worm segment up towards the head */
    struct game_obj_t *child;   /* next worm segment back towards the tail */
    int x[NWORM_TIME_UNITS];    /* list of this segments position for the last */
    int y[NWORM_TIME_UNITS];    /* few time units... x[i], y[i] is current pos */
    int i;              /* current index into x,y arrays */
    int tx, ty;         /* short term destination x,y */
    int ltx, lty;           /* long term destination x,y */
    int coloroffset;
};

struct debris_data {
    int debris_type;        /* controls what sounds debris makes when */
#define DEBRIS_TYPE_STONE 0     /* it bounces. */
#define DEBRIS_TYPE_METAL 1
};

struct health_data {
    int health;
    int prevhealth;
    int maxhealth;
};

struct truss_data {
    struct game_obj_t *above, *below;
    int height, top_width, bottom_width; /*  to allow different shaped trusses */
};

struct tesla_data {
    int bank_shot_factor; /* has to match bomb. */
    int invert;
    int discharging;
    int close_enough;
    int time_to_discharge;
    int targetx, targety;
    struct game_obj_t *attached_to;
};

struct kgun_data {
    int bank_shot_factor; /* has to match bomb. */
    int invert; /* 1 means hanging upside down, -1, means mounted on ground. */
    int size;   /* between 0 and 31, 31 being the biggest. */
    int velocity_factor; /* smaller guns shoot slower, unless corrected by velocity factor */
    int recoil_amount; /* when shooting, set to 10, then decrement to 0 each frame. */
    struct game_obj_t *attached_to; /* thing gun is attached to */
};

struct gunwheel_data {
    int size;
    struct game_obj_t *left;
    struct game_obj_t *right;
    struct game_obj_t *attached_to;
    int inverted;
    int rotation_multiplier;
};

struct house_data {
    int santa_came;
};

struct bomb_data {
    int bank_shot_factor;
};

struct laser_data {
    int tail_x;
};

union type_specific_data {      /* union of all the typs specific data */
    struct harpoon_data harpoon;
    struct gdb_data gdb;
    struct octopus_data octopus;
    struct extra_player_data epd;
    struct reindeer_data reindeer;
    struct tentacle_data tentacle;
    struct floating_message_data floating_message;
    struct cron_data cron;
    struct human_data human;
    struct jammer_data jammer;
    struct fuel_data fuel;
    struct airship_data airship;
    struct debris_data debris;
    struct worm_data worm;
    struct truss_data truss;
    struct kgun_data kgun;
    struct house_data house;
    struct bomb_data bomb;
    struct laser_data laser;
    struct gunwheel_data gunwheel;
    struct tesla_data tesla;
};

struct game_obj_t {
    int number;         /* offset into the go[] (game object) array */
    obj_move_func *move; 
    obj_draw_func *draw;
    obj_destroy_func *destroy;
    struct my_vect_obj *v;      /* drawing instructions */
    int x, y;           /* current position, in game coords */
    int vx, vy;         /* velocity */
    int above_target_y;
    int below_target_y;
    int color;          /* initial color */
    int alive;          /* alive?  Or dead? */
    int otype;          /* object type */
    struct game_obj_t *bullseye;    /* point to object this object is chasing */
    int last_xi;            /* the last x index into the terrain array which */
                    /* corresponds to the segment directly underneath */
                    /* this object -- used for detecting when an object */
                    /* smacks into the ground. */
    int counter;            /* a counter various object types used for var. purposes */
    union type_specific_data tsd;   /* the Type Specific Data for this object */
    int missile_timer;      /* to keep missiles from firing excessively rapidly */
    int radar_image;        /* Does this object show up on radar? */ 
    int uses_health;
    struct health_data health;
    struct game_obj_t *next;    /* These pointers, next, prev, are used to construct the */
    struct game_obj_t *prev;    /* target list, the list of things which may be hit by other things */
    int ontargetlist;       /* this list keeps of from having to scan the entire object list. */
    get_coords_func gun_location;   /* for attaching guns to objects */
    struct game_obj_t *attached_gun;/* For attaching guns to objects */
    struct my_vect_obj *debris_form;/* How to draw it when destroyed. */
    int lastx, lasty;       /* in case we want to calc velocity by verlet integration */
};

struct game_obj_t *target_head = NULL;

struct terrain_t {          /* x,y points of the ground, in game coords */
    int npoints;
    int x[TERRAIN_LENGTH];
    int y[TERRAIN_LENGTH];
    int slope[TERRAIN_LENGTH];
} terrain;

struct game_state_t {
    int x;              /* x position of left edge of screen, in game coords */
    int y;              /* y position of top of screen, in game coords */
    int vx;             /* x velocity of viewport.  Usually the same as player's vx */
    int vy;             /* y velocity of viewport.  Usually the same as player's vy */
    int lives;          /* How many lives the player has left in this game. */
    int nobjs;          /* max number of objects in the game */
    int direction;          /* direction player is facing. 1 = left, -1 = right. */
    int health;         /* Think of this as the player's hit points. */
    int max_player_health;      /* maximum player health, affected by difficulty setting. */
    int score;          /* Player's score */
    int prev_score;         /* Used to detect changes in score for drawing routine efficiency. */
    int nbombs;         /* number of bombs in the player's possession */
    int ngbombs;            /* number of bombs in the player's possession */
    int prev_bombs;         /* Used to detect changes in bombs for drawing routine efficiency */
    int humanoids;          /* Number of humanoids the player has picked up */
    int missile_locked;     /* Indicates whether a missile is after the player, for alarm sound */
    int corrosive_atmosphere;
    struct timeval start_time,  /* Used to calculate how long player took to finish a level */
        finish_time;
    struct game_obj_t go[MAXOBJS];  /* all the objects in the game are in this array. */
    int cmd_multiplier;     /* If you prefix keystrokes with a number... like editing in vi. */
    int sound_effects_on;       /* Whether sound effects are turned on or off */
    int sound_effects_manually_turned_off; /* whether sound effects were manually turned off */
                    /* rather than turned off by the goofy intro music. */
    int radar_state;        /* Whether the radar is booting up, fritzed, or operational */
#define     RADAR_RUNNING (0)
#define     RADAR_FRITZED (-1)
#define     RADAR_BOOTUP (3 * frame_rate_hz) /* How long it takes the radar to boot up. */
    int nextbombtime;
    int nextgbombtime;
    int nextlasertime;
    int nextlasercolor;
    int nextchafftime;

    int key_up_pressed;
    int key_down_pressed;
    int key_left_pressed;
    int key_right_pressed;
    int key_bomb_pressed;
    int key_present_pressed;
    int key_gbomb_pressed;
    int key_chaff_pressed;
    int key_laser_pressed;
    int x_joystick_axis;
    int y_joystick_axis;
    int rumble_wanted;
    int houses_gifted;

} game_state;
int highest_object_number = 1;
int last_user_input_time = 0;
int user_inaction_audio_pause = 0;


char rumbledevicestring[PATH_MAX];
char *rumbledevice = NULL;

struct game_obj_t * human[MAXHUMANS];   /* Keep a special array of just the humans so we can scan it quickly */

struct game_obj_t *volcano_obj;
struct game_obj_t *player = &game_state.go[0];  /* The player is object zero. */
struct game_obj_t *player_target;
int lasthuman = 0;              /* debug code... for 'n' key. */

GdkGC *gc = NULL;       /* our graphics context. */
gint timer_tag;         /* for our gtk 30 times per second timer function */
int next_quarter_time = -1; /* Used to limit rate at which quarters can be put in. */
int next_thunder_time = -1;

int destiny_facedown = 0;       /* is goofy intro playing? */
int destiny_facedown_timer1 = 0;    /* when goofy intro stops, turn on sound effects */
int destiny_facedown_timer2 = 0;    /* when to start goofy intro again. */

int old_window_width = 0;
int old_window_height = 0;
int fullscreen = 0;
int initial_random_seed = 31415927;
int want_missile_alarm = 1;
#define LEVELWARP
#ifdef LEVELWARP
int levelwarp = 1;
#endif

#define STAR_SHIFT 3
#define NSTARS 600 
#ifdef OPENLASE
int number_of_stars = 20;
#else
int number_of_stars = 150;
#endif
int starshift = STAR_SHIFT; 
struct star_t {
    short x; 
    short y;
    int last_xi;
    char bright;
} star[NSTARS];
short star_x_offset = 0;
short star_y_offset = 0;

static inline int randomn(int n);
int want_starfield = 1;
void init_stars()
{
    int i;

    for (i=0;i<number_of_stars;i++) {
        star[i].x = randomn(SCREEN_WIDTH) << starshift;
        star[i].y = randomn(SCREEN_HEIGHT) << starshift;
        star[i].bright = randomn(100) & 0x01;
        star[i].last_xi = -1;
    }
}

int ground_level(int x, int *xi, int *slope);
int interpolate(int x, int x1, int y1, int x2, int y2);


/* given world coords, x, and y, and pointer to last index into the
 * terrain array which we were above, return an "approximate horizon"
 * which is to say, return a y value which is either the true y value
 * of the ground directly below the given x value, or, if it is 
 * faster to compute, give a value which is > y if tf the horizon is
 * below y, or < y, if the horizon is above y. 
 *
 * Useful for instance to know if stars are above or below the horizon
 * to know if they should be drawn. 
 *
 * Also, the value of *last_xi is updated with the terrain index 
 * corresponding to the x value given.  If this is saved, and reused 
 * between calls, then an expensive search is avoided.
 *
 */
int approximate_horizon(int x, int y, int *last_xi) 
{
    /* optimized way to find the ground level (y value) at an object's */
    /* x position.  Each object tracks the terrain segment it was last at. */
    /* This means, the linear search of the terrain array is mostly eliminated */

    int xi1, xi2, i;
    int slope;
    xi1 = *last_xi; /* try the terrain segment we used last time. */
    xi2 = xi1 + 1;

    if (xi1 < 0 || xi2 >= TERRAIN_LENGTH) /* Do we have a hint of the last one? No? */
        return ground_level(x, last_xi, &slope); /* do it the hard way. */

    /* Is the last terrain segment the correct one? */
    if (terrain.x[xi1] <= x && terrain.x[xi2] >= x) {
    
        /* if both endpoints are below us, then no need ti find the exact point */
        /* below us, it is sufficient to know that it must be _somewhere_ */
        /* below us, and not above us. */
        if (terrain.y[xi1] > y && terrain.y[xi2] > y)
            return y + 10;
        /* similarly if both endpoints above us... */
        if (terrain.y[xi1] < y && terrain.y[xi2] < y)
            return y - 10;

        /* vast majority will hit one of above 2 cases, but close to the ground... */
        return interpolate(x, terrain.x[xi1], terrain.y[xi1],  /* do it the hard way. */
                terrain.x[xi2], terrain.y[xi2]);
    }

    /* The last one wasn't correct.  Have to search. */
    if (terrain.x[xi1] > x) {
        /* The correct one is to the left.  Search left. */
        for (i=xi1-1;i>=0;i--) {
            if (x >= terrain.x[i] && x < terrain.x[i+1]) {

                /* remember i for next time so we don't have to search. */
                *last_xi = i;

                /* Two trivial cases...  ground is trivially below us? */
                if (terrain.y[i] > y && terrain.y[i+1] > y)
                    return y + 10;
                /* or trivially above us? */
                if (terrain.y[i] < y && terrain.y[i+1] < y) 
                    return y - 10;
                /* Close to the ground, so, ok, do it the hard way. */
                return interpolate(x, terrain.x[i], terrain.y[i],
                        terrain.x[i+1], terrain.y[i+1]);
            }
        }
    } else if (terrain.x[xi2] < x) { /* Correct one is to the right. */
        for (i=xi1+1;i<=TERRAIN_LENGTH-10;i++) { /* search to the right. */
            if (x >= terrain.x[i] && x < terrain.x[i+1]) {

                /* remember i for next time so we don't have to search. */
                *last_xi = i;

                /* Two trivial cases...  ground is trivially below us? */
                if (terrain.y[i] > y && terrain.y[i+1] > y)
                    return y + 10;
                /* or trivially above us? */
                if (terrain.y[i] < y && terrain.y[i+1] < y) 
                    return y - 10;
                /* Close to the ground, so, ok, do it the hard way. */
                return interpolate(x, terrain.x[i], terrain.y[i],
                        terrain.x[i+1], terrain.y[i+1]);
            }
        }
    }

    /* What?  there *is* no correct answer.  Object must have fallen off */
    /* the edge of the planet. (it happens sometimes -- though that is a bug. ) */
    *last_xi = -1;
    return GROUND_OOPS;
}

void do_strong_rumble()
{
    if (credits > 0 && game_state.rumble_wanted)
        play_rumble_effect(RUMBLE_STRONG_RUMBLE_EFFECT);
}

void do_weak_rumble()
{
    if (credits > 0 && game_state.rumble_wanted)
        play_rumble_effect(RUMBLE_WEAK_RUMBLE_EFFECT);
}

void incorrect_draw_stars()
{
    short i;
    short worldx, worldy;
    short gl, sx, sy;

    gdk_gc_set_foreground(gc, &huex[WHITE]);
    for (i=0;i<number_of_stars;i++) 
    {
        if (randomn(100) < 3)
            continue;

        /* extra SCREEN_DIMENSTION << starshift to correct for wraparound */
        /* is already added into star_xy_offset, after this loop. */
        /* Careful with precision here, don't shift right too early. */
        sx = (star[i].x + star_x_offset) % (SCREEN_WIDTH << starshift);
        sy = (star[i].y + star_y_offset) % (SCREEN_HEIGHT << starshift);
        worldx = ((game_state.x << starshift)  + sx) >> starshift;
        worldy = ((sy + (game_state.y << starshift)) - 
            ((SCREEN_HEIGHT/2) << starshift)) >> starshift; 
        sx = sx >> starshift;
        sy = sy >> starshift;
        gl = approximate_horizon(worldx, worldy, &star[i].last_xi);
        if (worldy < gl) {
            if (star[i].bright && randomn(100) > 10) {
                wwvi_draw_line(sx, sy-1, sx+1, sy-1);
            }
            wwvi_draw_line(sx, sy, sx+1, sy);
        }
    }
    /* move stars */
    star_x_offset = (star_x_offset + (SCREEN_WIDTH << starshift) - game_state.vx) 
            % (SCREEN_WIDTH << starshift) + (SCREEN_WIDTH << starshift);
    star_y_offset = (star_y_offset + (SCREEN_HEIGHT << starshift) - game_state.vy) 
            % (SCREEN_HEIGHT << starshift) + (SCREEN_HEIGHT << starshift);
}

void correct_draw_stars()
{
    short i;
    short worldx, worldy;
    short gl, sx, sy;

    gdk_gc_set_foreground(gc, &huex[WHITE]);
    for (i=0;i<number_of_stars;i++) 
    {
        if (randomn(100) < 3)
            continue;

        sx = (star[i].x + star_x_offset) % (SCREEN_WIDTH);
        sy = (star[i].y + star_y_offset) % (SCREEN_HEIGHT);
        worldx = ((game_state.x)  + sx);
        worldy = ((sy + (game_state.y)) - ((SCREEN_HEIGHT/2))); 
        gl = approximate_horizon(worldx, worldy, &star[i].last_xi);
        if (worldy < gl) 
        {
            if (star[i].bright && randomn(100) > 10) 
            {
                wwvi_draw_line(sx, sy-1, sx+1, sy-1);
            }
            wwvi_draw_line(sx, sy, sx+1, sy);
        }
    }
}

void (*draw_stars)() = incorrect_draw_stars;


/* add an object to the list of targets... */
struct game_obj_t *add_target(struct game_obj_t *o)
{
#ifdef DEBUG_TARGET_LIST
    struct game_obj_t *t;

    for (t = target_head; t != NULL; t = t->next) {
        if (t == o) {
            printf("Object already in target list!\n");
            return NULL;
        }
    }
    if (o->ontargetlist) {
        printf("Object claims to be on target list, but isn't.\n");
    }
#endif

    o->next = target_head;
    o->prev = NULL;
    if (target_head)
        target_head->prev = o;
    target_head = o;
    o->ontargetlist = 1;

    return target_head;
}

/* for debugging... */
void print_target_list()
{
    struct game_obj_t *t;
    printf("Targetlist:\n");
    for (t=target_head; t != NULL;t=t->next) {
        printf("%c: %d,%d\n", t->otype, t->x, t->y);
    }
    printf("end of list.\n");
}

/* remove an object from the target list */
struct game_obj_t *remove_target(struct game_obj_t *t)
{

    struct game_obj_t *next;
    if (!t)
        return NULL;
#ifdef DEBUG_TARGET_LIST
    if (!t->ontargetlist) {
        for (next = target_head; next != NULL; next = next->next) {
            if (next == t) {
                printf("Remove, object claims not to be on target list, but it is.\n");
                goto do_it_anyway;
            }
        }
        return NULL;
    }
do_it_anyway:
#endif
    next = t->next;
    if (t == target_head)
        target_head = t->next;
    if (t->next)
        t->next->prev = t->prev;
    if (t->prev)
        t->prev->next = t->next;
    t->next = NULL;
    t->prev = NULL;
    t->ontargetlist=0;
    return next;
}

/* get a random number between 0 and n-1... fast and loose algorithm.  */
static inline int randomn(int n)
{
    return random() % n;
}

#define min(a,b) ((a) > (b) ? (b) : (a))

/* get a random number between a and b. */
static inline int randomab(int a, int b)
{
    return randomn(abs(a - b)) + min(a,b);
}

void explode(int x, int y, int ivx, int ivy, int v, int nsparks, int time);
void explode_pixie_dust(int x, int y, int ivx, int ivy, int v, int nsparks, int time);
void round_explode(int x, int y, int ivx, int ivy, int v, int nsparks, int time);
static inline void kill_object(struct game_obj_t *o);
static inline void age_object(struct game_obj_t *o);


void laserbolt_move(struct game_obj_t *o)
{
    int dy;
    if (!o->alive)
        return;
    dy = (o->y - player_target->y);
    if (dy < -1000) { /* if laser bolt is very far away, just get forget about it. */
        kill_object(o);
        o->destroy(o);
        return;
    }
    if (abs(dy) < 14 && abs(player_target->x - o->x) < 20) { /* hit the player? */
        explosion(o->x, o->y, o->vx, 1, 70, 20, 20);
        wwviaudio_add_sound(LASER_EXPLOSION_SOUND);
        game_state.health -= LASER_BOLT_DAMAGE;
        do_strong_rumble();
        kill_object(o);
        o->destroy(o);
    }
    o->x += o->vx;
    o->y += o->vy;
    age_object(o);
}

void make_bomb_sound()
{
    /* Prevent more than two BOMB_IMPACT_SOUNDS from firing off */
    /* in the same frame because allowing more is too loud. */
    static int lasttime = -1;
    static int nbombs_this_frame = 0;

    if (lasttime == timer) {
        if (nbombs_this_frame > 2)
            return;
        nbombs_this_frame++;
    } else {
        lasttime = timer;
        nbombs_this_frame = 1;
    }
    wwviaudio_add_sound(BOMB_IMPACT_SOUND);
}

void house_move(__attribute__ ((unused)) struct game_obj_t *o)
{
    return;
}

void fuel_move(struct game_obj_t *o)
{
    int xdist, ydist;

    xdist = abs(player_target->x - (o->x + 20));
    ydist = abs(player_target->y - o->y);
    if (xdist <= HUMANOID_DIST*3 && 
        ydist <= HUMANOID_DIST*5 &&     /* player close enough? */
        (timer % REFUEL_RATE) == 0 &&       /* control refuel rate... */
        o->tsd.fuel.level > 0 &&        /* there is some fuel to dispense? */
        game_state.health < game_state.max_player_health ) {    /* player's tank not full? */
            o->tsd.fuel.level--;        /* ...give player some fuel. */
            game_state.health++;
    } else 
        /* tanks refill at one unit per 3 secs */
        if (o->tsd.fuel.level < FUELTANK_CAPACITY && 
            (timer % REFILL_RATE == 0))
            o->tsd.fuel.level++;
}

/* Given target o, and shooter, sx,sy, and laserbolt velocity v, */
/* Compute correct *vx, *vy */
static void aim_vx_vy(struct game_obj_t *target, 
        struct game_obj_t *shooter,
        int v, int leadtime, 
        int *vx, int *vy)
{
    int dx, dy;
    dx = target->x + leadtime * target->vx - shooter->x;
    dy = target->y + leadtime * target->vy - shooter->y;

    /* whichever is farther, x or y, make that vx or vy be the max */
    /* then calculate the other one by similar triangles. */

    if (abs(dx) >= abs(dy)) {
        if (dx < 0) {
            *vx = -v;
            *vy = (*vx * dy) / dx;
        } else {
            if (dx > 0) {
                *vx = v;
                *vy = (*vx * dy) / dx;
            } else {
                /* shooter and target are the same. */
                *vx = 0;
                *vy = 0;
            }
        }
    } else {
        if (dy < 0) {
            *vy = -v;
            *vx = (*vy * dx) / dy;
        } else {
            if (dy > 0) {
                *vy = v;
                *vx = (*vy * dx) / dy;
            } else {
                *vy = 0;
                *vx = 0;
            }
        }
    }
}

int hot_metal_color(int health, int maxhealth)
{
    return NCOLORS + ((maxhealth - health) * NSPARKCOLORS) / maxhealth;
}

static void add_laserbolt(int x, int y, int vx, int vy, int color, int time);
void kgun_move(struct game_obj_t *o)
{
    int xdist, ydist;
    int dx, dy;
    int y1;
    int xi, yi;
    int adx, ady;
    int vx, vy;
    int crossbeam_x1, crossbeam_y, crossbeam_x2, xoffset;
    int guntipx, guntipy;
    int chance;
    int size = o->tsd.kgun.size;
    int velocityfactor = o->tsd.kgun.velocity_factor;
    int invert = o->tsd.kgun.invert;
    int recoilx, recoily;
    struct game_obj_t *holder;

    o->lastx = o->x;
    o->lasty = o->y;

    /* if gun is attached to something, make it move with that something */
    holder = o->tsd.kgun.attached_to;
    if (holder != NULL)
        holder->gun_location(holder, o, &o->x, &o->y);

    if (o->health.prevhealth != o->health.health)
        o->color = hot_metal_color(o->health.health, o->health.maxhealth);
    o->health.prevhealth = o->health.health;
    if ((timer & 0x01f) == 0x01f && 
        o->health.health < o->health.maxhealth)
        o->health.health++;

    xdist = abs(o->x - player_target->x); /* in range? */
    ydist = abs((o->y + 20*invert) - player_target->y);

    if (xdist < 600)
        chance = 100 + xdist;
    else 
        chance = 1000;

    if (xdist < SCREEN_WIDTH && ydist < SCREEN_HEIGHT-200 && 
        randomn(chance) < level.laser_fire_chance) {
        /* we're going to fire the laser... */
        wwviaudio_add_sound(FLAK_FIRE_SOUND);
        o->tsd.kgun.recoil_amount = 7;

        /* Find x,y dist to player... */
        dx = player_target->x+LASERLEADX*player_target->vx - o->x;
        dy = player_target->y+LASERLEADY*player_target->vy - (o->y + 20*invert);

        adx = abs(dx);
        ady = abs(dy);

        y1 = o->y+(5*invert);  
        crossbeam_y = y1 + (20*invert);

        /* First, scale the larger of dx,dy to 32, and the other by */
        /* a proportional amount (by similar triangles).  This is in */
        /* order to index into the vxy_2_dxy array. */
        
        /* Calculate indices into vxy_2_dxy array, xi, and yi */
        if (adx > ady) {
            xi = size;
            yi = (size * ady) / adx;
        } else {
            if (ady > adx) {
                yi = size;
                xi = (size * adx) / ady;
            } else { /* abs(dx) == abs(dy) */
                xi = size;
                yi = size;
            }
        }

        /* notice, using y component for x offset... */
        crossbeam_x1 = vxy_2_dxy[xi][yi].y;
        crossbeam_x2 = -crossbeam_x1;

        crossbeam_x1 += o->x;
        crossbeam_x2 += o->x;

        xoffset = crossbeam_x2 - crossbeam_x1;

        /* guntipx = dx_from_vxy(xi, yi); */
        /* guntipy = dy_from_vxy(xi, yi); */

        guntipx = vxy_2_dxy[xi][yi].x;
        guntipy = vxy_2_dxy[xi][yi].y;

        if (dy > 20)
            guntipy = -guntipy;
        if (dx < 0)
            guntipx = -guntipx;

        recoilx = -guntipx;
        recoily = -guntipy;

        if (o->tsd.kgun.recoil_amount != 0) {
            recoilx = (recoilx * (o->tsd.kgun.recoil_amount)) >> 3;
            recoily = (recoily * (o->tsd.kgun.recoil_amount)) >> 3;
        }

        vx = guntipx * velocityfactor;
        vy = guntipy * velocityfactor;

        guntipx = guntipx << 1; /* gun barrel is 2x as long as the butt of the gun. */
        guntipy = guntipy << 1;

        guntipx += crossbeam_x1;
        guntipy += crossbeam_y;
        
        add_laserbolt(guntipx + recoilx, guntipy + recoily, vx, vy, RED, 50);
        add_laserbolt(guntipx+xoffset + recoilx, guntipy + recoily, vx, vy, RED, 50);
    }
}

void tesla_tower_move(struct game_obj_t *o)
{
    int dx, dy;

    o->lastx = o->x;
    o->lasty = o->y;

    /* discharge some... */
    if (o->tsd.tesla.discharging > 0) 
        o->tsd.tesla.discharging--;

    dx = player_target->x - o->x;
    dy = player_target->y - o->y;

    /* If we've discharged recently, charge up... */
    if (o->tsd.tesla.time_to_discharge <= timer) {
        o->tsd.tesla.discharging = frame_rate_hz / 4;
        o->tsd.tesla.time_to_discharge = timer + frame_rate_hz + randomn(7);
        if (abs(dx) + abs(dy) <= 650)
            wwviaudio_add_sound(TESLA_FIRE);
    }

    if (!(o->tsd.tesla.discharging > 0) && o->tsd.tesla.time_to_discharge > timer)
        return;

    /* We're charged.  How close is the player? */
    if (abs(dx) + abs(dy) > 650) {
        o->tsd.tesla.close_enough = 0; 
        return; /* not close enough. */
    } else
        o->tsd.tesla.close_enough = 1; 

    if (!o->tsd.tesla.discharging)
        return;

    o->tsd.tesla.targetx = player_target->x + randomn(20)-10;
    o->tsd.tesla.targety = player_target->y + randomn(20)-10;
    if (abs(o->tsd.tesla.targetx - player_target->x) < 5 &&
        abs(o->tsd.tesla.targety - player_target->y) < 5) {
        explosion(player_target->x, player_target->y, player_target->vx, player_target->vy, 70, 20, 20);
        wwviaudio_add_sound(LASER_EXPLOSION_SOUND);
        game_state.health -= LASER_BOLT_DAMAGE;
        do_strong_rumble();
    }

    return;
}

static void add_missile(int x, int y, int vx, int vy, 
    int time, int color, struct game_obj_t *bullseye);
static void add_harpoon(int x, int y, int vx, int vy, 
    int time, int color, struct game_obj_t *bullseye,
    struct game_obj_t *gdb);
int find_ground_level(struct game_obj_t *o, int *slope);

void rocket_move(struct game_obj_t *o)
{
    int xdist, ydist, gl;
    if (!o->alive)
        return;

    /* Should probably make a rocket-station, which launches rockets */
    /* instead of just having bare rockets sitting on the ground which */
    /* launch once, blow up, then that's the end of them. */

    /* see if rocket should launch... */
    xdist = abs(o->x - player_target->x);
    if (xdist < LAUNCH_DIST && o->alive != 2 && randomn(100) < 20) {
        ydist = o->y - player_target->y;
        if (((xdist<<1) <= ydist && ydist > 0) || o->vy != 0) {
            if (o->vy == 0) { /* only add the sound once. */
                wwviaudio_add_sound(ROCKET_LAUNCH_SOUND);
                o->alive = 2; /* launched. */
                o->vy = -6; /* give them a little boost. */
            }
        }
    }
    if (o->alive == 2) {
        if (o->vy > MAX_ROCKET_SPEED - (o->number % 5))
            o->vy--;

        /* let the rockets veer slightly left or right. */
        if (player_target->x < o->x && player_target->vx < 0)
            o->vx = -2;
        else if (player_target->x > o->x && player_target->vx > 0)
            o->vx = 2;
        else 
            o->vx = 0;

        /* It's possible a gravity bomb smashes the rocket */
        /* into the ground. */
        gl = find_ground_level(o, NULL);
        if (o->y > gl) {
            wwviaudio_add_sound(ROCKET_EXPLOSION_SOUND);
            explosion(o->x, o->y, o->vx, 1, 70, 150, 20);
            remove_target(o);
            kill_object(o);
            return;
        }

        ydist = o->y - player_target->y;
        if ((ydist*ydist + xdist*xdist) < 400) { /* hit the player? */
            wwviaudio_add_sound(ROCKET_EXPLOSION_SOUND);
            do_strong_rumble();
            explosion(o->x, o->y, o->vx, 1, 70, 150, 20);
            game_state.health -= 20;
            remove_target(o);
            kill_object(o);
            return;
        }
    }


    /* move the rocket... */
    o->x += o->vx;
    o->y += o->vy;
    if (o->vy != 0) {
        explode(o->x, o->y, 0, 9, 8, 7, 13); /* spray out some exhaust */
        /* a gravity bomb might pick up the rocket... this prevents */
        /* it from being left stranded in space, not moving. */
        if (o->alive == 1)
            o->alive = 2;
    }
    if (o->y - player->y < -1000 && o->vy != 0) {
        /* if the rocket is way off the top of the screen, just forget about it. */
        remove_target(o);
        kill_object(o);
        o->destroy(o);
    }
}

static void add_bullet(int x, int y, int vx, int vy, 
    int time, int color, struct game_obj_t *bullseye);
void big_rocket_move(struct game_obj_t *o)
{
    int xdist, ydist, gl, i;
    if (!o->alive)
        return;

    /* Should probably make a rocket-station, which launches rockets */
    /* instead of just having bare rockets sitting on the ground which */
    /* launch once, blow up, then that's the end of them. */

    /* see if rocket should launch... */
    xdist = abs(o->x - player_target->x);
    if (xdist < BIG_ROCKET_LAUNCH_DIST && o->alive != 2 && randomn(100) < 20) {
        /* ydist = o->y - player_target->y; */
        /* if (((xdist<<2) <= ydist && ydist > 0)) { */
            if (o->vy == 0) { /* only add the sound once. */
                wwviaudio_add_sound(ROCKET_LAUNCH_SOUND);
                o->alive = 2; /* launched. */
                o->vy = -6; /* give them a little boost. */
            }
        /* } */
    }
    if (o->alive == 2) {
        if (o->vy > MAX_ROCKET_SPEED - (o->number % 5))
            o->vy--;

        /* let the rockets veer slightly left or right. */
        if (player_target->x < o->x && player_target->vx < 0)
            o->vx = -2;
        else if (player_target->x > o->x && player_target->vx > 0)
            o->vx = 2;
        else 
            o->vx = 0;

        /* It's possible a gravity bomb smashes the rocket */
        /* into the ground. */
        gl = find_ground_level(o, NULL);
        if (o->y > gl) {
            wwviaudio_add_sound(ROCKET_EXPLOSION_SOUND);
            explosion(o->x, o->y, o->vx, 1, 70, 150, 20);
            remove_target(o);
            kill_object(o);
            return;
        }

        ydist = o->y - player_target->y;
        if ((ydist*ydist + xdist*xdist) < 16000) { /* hit the player? */
            wwviaudio_add_sound(ROCKET_EXPLOSION_SOUND);
            do_strong_rumble();
            explosion(o->x, o->y, o->vx, 1, 70, 150, 20);
            /* game_state.health -= 10; */
            remove_target(o);
            kill_object(o);

            for (i=0;i<15;i++) {
                add_bullet(o->x, o->y, 
                    randomn(40)-20, randomn(40)-20, 
                    30, YELLOW, player_target);
            }

            return;
        }
    }


    /* move the rocket... */
    o->x += o->vx;
    o->y += o->vy;
    if (o->vy != 0) {
        explode(o->x, o->y+15, 0, 9, 8, 7, 13); /* spray out some exhaust */
        /* a gravity bomb might pick up the rocket... this prevents */
        /* it from being left stranded in space, not moving. */
        if (o->alive == 1)
            o->alive = 2;
    }
    if (o->y - player->y < -1000 && o->vy != 0) {
        /* if the rocket is way off the top of the screen, just forget about it. */
        remove_target(o);
        kill_object(o);
        o->destroy(o);
    }
}

void jet_move(struct game_obj_t *o)
{
    int xdist, desired_y;
    if (!o->alive)
        return; 

    xdist = o->x - player_target->x;
    if (xdist > SCREEN_WIDTH * 2)
        return;

    if (xdist > 0 && xdist < SCREEN_WIDTH * 2) {
        if (o->vx == 0) {
            o->y = player_target->y - 50 - randomn(200);
            o->vx = -18;
            o->vy = 0;
            o->radar_image = 1;
            wwviaudio_add_sound(JETWASH_SOUND);
        }

        if (xdist < SCREEN_WIDTH && randomn(100) < 10 && o->missile_timer <= timer) {
            wwviaudio_add_sound(SAM_LAUNCH_SOUND);
            add_missile(o->x, o->y, o->vx, 0, 300, RED, player_target);
            o->missile_timer = timer + MISSILE_FIRE_PERIOD;
        }

    }

    desired_y = player_target->y - 15 - randomn(25);
    if (desired_y < KERNEL_Y_BOUNDARY)
        desired_y = KERNEL_Y_BOUNDARY + 30;
    if (o->y > desired_y)
        o->vy = -1;
    if (o->y < desired_y)
        o->vy = 1;
 
    o->x += o->vx;
    o->y += o->vy;

    explode(o->x+40, o->y, 9, 0, 8, 7, 13); /* spray out some exhaust */

    if (xdist < -SCREEN_WIDTH * 2) {
        remove_target(o);
        kill_object(o);
        o->destroy(o);
    }
}

/* move sam stations. */
void sam_move(struct game_obj_t *o)
{
    int xdist, ydist;
    if (!o->alive)
        return;

    xdist = abs(o->x - player_target->x);
    if (xdist < SAM_LAUNCH_DIST) { /* player in range? */
        ydist = o->y - player_target->y;
        /* should we launch?  And not too many at once... */
        if (ydist > 0 && randomn(1000) < SAM_LAUNCH_CHANCE && timer >= o->missile_timer) {
            /* launching a missile... */
            wwviaudio_add_sound(SAM_LAUNCH_SOUND);
            add_missile(o->x+20, o->y-30, 0, -10, 300, GREEN, player_target);
            o->missile_timer = timer + MISSILE_FIRE_PERIOD;
        }
    }
}

void draw_generic(struct game_obj_t *o);
void draw_scaled_generic(struct game_obj_t *o, float xscale, float yscale);

void fuel_draw(struct game_obj_t *o)
{
  int x1, y1, x2, y2;
    int xdist, ydist;

    xdist = abs(player_target->x - o->x);
    ydist = abs(player_target->y - o->y);
        
    /* draw the fuel in the tank. */
    x1 = o->x - game_state.x - 25;
    y1 = o->y - game_state.y + (SCREEN_HEIGHT/2) + 
        30 - (45 * o->tsd.fuel.level / FUELTANK_CAPACITY);
    if (o->tsd.fuel.level > 0) {
        gdk_gc_set_foreground(gc, &huex[DARKGREEN]);
        wwvi_draw_rectangle(TRUE, x1, y1, 50, o->tsd.fuel.level * 45 / FUELTANK_CAPACITY);
    }

    draw_generic(o); /* draw the fuel tank in the usual way... */

    /* if the player is close, draw a refueling hose... */
    if (xdist <= HUMANOID_DIST*3 && ydist <= HUMANOID_DIST*5) {
        x1 = o->x - game_state.x;
        y1 = o->y - game_state.y + (SCREEN_HEIGHT/2);
        x2 = player_target->x - game_state.x;
        y2 = player_target->y - game_state.y + (SCREEN_HEIGHT/2);
        wwvi_draw_line(x1, y1, x2, y2);
    }
}

/* draw a radar jammer */
void jammer_draw(struct game_obj_t *o)
{
    int x1, x2, y1, y2, x3, x4, x5;
    draw_generic(o); /* will set the color too. */

    /* this draws a sort of spinning rectangular gridded radar dish */
    o->tsd.jammer.width += o->tsd.jammer.direction;
    if (o->tsd.jammer.width > 8) {
        o->tsd.jammer.direction = -1;
    } else if (o->tsd.jammer.width < 2) {
        o->tsd.jammer.direction = 1;
    }
    y1 = o->y - game_state.y + (SCREEN_HEIGHT/2) - 15;  
    y2 = y1 - 16;
    x5 = (o->x - game_state.x);
    x1 = x5 - o->tsd.jammer.width;
    x2 = x5 + o->tsd.jammer.width;
    x3 = x5 - (o->tsd.jammer.width >> 1);
    x4 = x5 + (o->tsd.jammer.width >> 1);
    wwvi_draw_line(x1, y1, x2, y1); 
    wwvi_draw_line(x1, y2, x2, y2); 
    wwvi_draw_line(x1, y1, x1, y2); 
    wwvi_draw_line(x2, y1, x2, y2); 
    wwvi_draw_line(x3, y1, x3, y2); 
    wwvi_draw_line(x4, y1, x4, y2); 
    wwvi_draw_line(x5, y1, x5, y2); 
    wwvi_draw_line(x1, y2+4, x2, y2+4); 
    wwvi_draw_line(x1, y2+8, x2, y2+8); 
    wwvi_draw_line(x1, y2+12, x2, y2+12); 
}

void cron_draw(struct game_obj_t *o)
{
    int x1, y1, x2, y2, gy, xi;

    draw_generic(o);

#if 0
    /* this is debug code */
    dist2 = (o->tsd.cron.tx - o->x)* (o->tsd.cron.tx - o->x) + (o->tsd.cron.ty - o->y)* (o->tsd.cron.ty - o->y);
    if (dist2 < 20000) {
        gdk_gc_set_foreground(gc, &huex[randomn(NCOLORS+NSPARKCOLORS)]);
        x1 = o->x - game_state.x;
        y1 = o->y + 7 - game_state.y + (SCREEN_HEIGHT/2);  
        x2 = o->tsd.cron.tx - game_state.x;
        y2 = o->tsd.cron.ty + 7 - game_state.y + (SCREEN_HEIGHT/2);  
        if (x1 > 0 && x2 > 0)
            wwvi_draw_line(x1, y1, x2, y2); 
    }
#endif

    /* draw the cron job's eye... */
    x1 = o->x - game_state.x + o->tsd.cron.eyepos;
    y1 = o->y - game_state.y + (SCREEN_HEIGHT/2) - 5;  
    x2 = x1 + 3;
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    wwvi_draw_line(x1, y1, x2, y1); /* draw eye */

    /* move the cron job's eye */
    o->tsd.cron.eyepos +=1;
    if (o->tsd.cron.eyepos > 10)
        o->tsd.cron.eyepos = -10;
    
    /* draw the cron job's scanning beam... */
    if (o->tsd.cron.beam_speed != 0) { /* beam on? */
        x1 = o->x - game_state.x;
        x2 = o->x + o->tsd.cron.beam_pos - game_state.x; 
        if ((x1 > 0 && x2 > 0) && 
            (x1 < SCREEN_WIDTH && x2 < SCREEN_WIDTH)) { /* on screen? */
            gy = ground_level(o->x + o->tsd.cron.beam_pos, &xi, NULL);
            if (xi != -1) {
                y1 = o->y + 7 - game_state.y + (SCREEN_HEIGHT/2);  
                y2 = gy + (SCREEN_HEIGHT/2) - game_state.y;
                wwvi_bright_line(x1, y1, x2, y2,
                    randomn(NCOLORS+NSPARKCOLORS+NRAINBOWCOLORS));  /* draw beam. */
                explode(o->x + o->tsd.cron.beam_pos, gy, 0, 1, 20, 3, 10); /* make sparks. */
            }
        }
    }
}

void gdb_draw(struct game_obj_t *o) /* draw a gdb. */
{
    /* make him point left or right, depending which way he's going. */
    if (o->vx < 0)      
        o->v = &gdb_vect_left;
    else if (o->vx > 0)
        o->v = &gdb_vect_right;
    draw_generic(o);
}

int recCount =0;
/* recursive routine to draw a fractal lightning bolt between x1,y1 and x2,y2 */
void lightning_draw(int x1, int y1, int x2, int y2) 
{
    int x3, y3, dx, dy;
recCount++;

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);

    /* terminal case, x1, y1, and x2, y2 are very close, just draw a line. */
//printf("Lightning(%i): x1=%i,y1=%i <-> x2=%i,y2=%i (dx:%i, dy%i)\n",recCount, x1, y1, x2, y2, dx, dy);
    if (dx < 35 && dy < 35) 
    {
        gdk_gc_set_foreground(gc, &huex[WHITE]);
        wwvi_draw_line(x1, y1, x2, y2); 
        if (dx < dy) {
            wwvi_draw_line(x1-1, y1, x2-1, y2); 
            wwvi_draw_line(x1+1, y1, x2+1, y2); 
            gdk_gc_set_foreground(gc, &huex[BLUE]);
            wwvi_draw_line(x1-2, y1, x2-2, y2); 
            wwvi_draw_line(x1+2, y1, x2+2, y2); 
        } else {
            wwvi_draw_line(x1, y1-1, x2, y2-1); 
            wwvi_draw_line(x1, y1+1, x2, y2+1); 
            gdk_gc_set_foreground(gc, &huex[BLUE]);
            wwvi_draw_line(x1, y1-2, x2, y2-1); 
            wwvi_draw_line(x1, y1+2, x2, y2+1); 
        }
//printf("Lightning terminating (%i)!\n", recCount);
        recCount--;
        return;
    }
    if (dy < 9)
        dy = 9;
    if (dx < 9)
        dx = 9;

    /* find the midpoint between x1,y1, x2, y2.  Call this midpoint x3,y3. */
    if (y2 > y1)
        y3 = (y2 - y1) / 2 + y1;
    else if (y2 < y1)
        y3 = (y1 - y2) / 2 + y2;
    else 
        y3 = y1;
    if (x2 > x1)
        x3 = (x2 - x1) / 2 + x1;
    else if (x2 < x1)
        x3 = (x1 - x2) / 2 + x2;
    else
        x3 = x1;

    /* perturb the midpoint a bit */
    x3 += (int) (0.4 * (randomn(2*dx) - dx)); 
    y3 += (int) (0.4 * (randomn(2*dy) - dy)); 

    /* draw lightning between x1,y1 and x3,y3, and between x3,y3, and x2,y2. */
    lightning_draw(x1, y1, x3, y3);  
    lightning_draw(x3, y3, x2, y2);  
    recCount--;
}


static void xy_draw_string(char *s, int font, int x, int y) ;
void floating_message_draw(struct game_obj_t *o)
{
    /* draw a floating message -- a game object that is text living in game coords. */
    /* this is for bonus scores, "Help!" and "Woohoo!" messages the .swp files "say." */
    gdk_gc_set_foreground(gc, &huex[o->color]);
    xy_draw_string((char *) o->tsd.floating_message.msg, 
        o->tsd.floating_message.font, o->x, o->y) ;
}

void tentacle_draw(struct game_obj_t *o)
{
    int i;
    int x1, y1, x2, y2;
    int angle = 0;
    int thickness = o->tsd.tentacle.nsegs >> 1;

    x2 = 0; /* make compiler happy */
    y2 = 0; /* make compiler happy */

    /* draw each tentacle, accumulating relative angles as you go, */
    /* alternating colors of segments between yellow and blue.  */
    gdk_gc_set_foreground(gc, &huex[o->color]);
    x1 = o->x - game_state.x;
    y1 = o->y - game_state.y + (SCREEN_HEIGHT/2);  
    for (i=0;i<o->tsd.tentacle.nsegs;i++) {
        angle = angle + o->tsd.tentacle.seg[i].angle;
        if (angle < 0)
            angle += 360;
        else if (angle > 360)
            angle -= 360;
        x2 = x1 + cosine[angle] * o->tsd.tentacle.seg[i].length; 
        y2 = y1 -   sine[angle] * o->tsd.tentacle.seg[i].length; 
    
        switch (o->tsd.tentacle.color_scheme) {
        case 0: 
            if ((i % 2) == 1)
                gdk_gc_set_foreground(gc, &huex[o->color]);
            else
                gdk_gc_set_foreground(gc, &huex[o->tsd.tentacle.other_color]);
            break;
        case 1:
            gdk_gc_set_foreground(gc, &huex[((1000-i+(timer >> 1)) % NSPARKCOLORS) + NCOLORS]);
            break;
        case 2:
            /* gdk_gc_set_foreground(gc, &huex[NCOLORS + NSPARKCOLORS + (i * NRAINBOWCOLORS)/o->tsd.tentacle.nsegs]); */
            gdk_gc_set_foreground(gc, &huex[NCOLORS + NSPARKCOLORS + (((1000-i+(timer >> 2)) + ((i & 0x01)*4)) % NRAINBOWCOLORS)]);
            break;
        default: /* constant color */
            break;
        }
#ifndef OPENLASE
        if (thickness > 0) {
            wwvi_draw_line(x1-thickness, y1, x2-thickness, y2); 
            wwvi_draw_line(x1+thickness, y1, x2+thickness, y2); 
            wwvi_draw_line(x1, y1-thickness, x2, y2-thickness); 
            wwvi_draw_line(x1, y1+thickness, x2, y2+thickness); 
            if (i & 0x01) 
                thickness--;
        } else 
#endif
            wwvi_draw_line(x1, y1, x2, y2); 
        x1 = x2;
        y1 = y2;
    }

    /* Shoot lightning at the player occasionally if he gets too close. */
    if ((timer & 0x01) && 
        randomn(1000) < 45 && 
        abs(o->x - player_target->x) < 200 && 
        abs(o->y - player_target->y) < 500) {

        lightning_draw(x2, y2, player_target->x - game_state.x, 
            player_target->y - game_state.y + (SCREEN_HEIGHT/2));
        do_weak_rumble();
        explode(player_target->x, player_target->y, player_target->vx*1.5, 1, 20, 20, 15);
        game_state.health -= 1; /* just take off one, the octo's are bad... */
        /* keep thunder from going oof too much */
        if ((next_thunder_time) < timer) {
            next_thunder_time = timer + frame_rate_hz * 2;
            wwviaudio_add_sound(THUNDER_SOUND);
        }
    }
}

void tentacle_move(struct game_obj_t *o) 
{

    struct tentacle_seg_data *t;
    int i, gy;

    /* if we're off screen, bail right away. */
    if (abs(o->x - game_state.x) > SCREEN_WIDTH*1.5)
        return;

    /* loose tentacle? */
    if (o->tsd.tentacle.attached_to == NULL) {  
        o->x += o->vx;
        o->y += o->vy;

        o->vy += 1; /* make loose tentacle fall to the ground. */

        gy = find_ground_level(o, NULL);
        if (o->y >= gy) {
            o->vy = 0;
            o->vx = 0;
            o->y = gy;
        }

        /* keep tentacle from jumping off the edges of the earth. */
        if (o->x < 0) {
            o->x = 0;
            o->vx = 20;
        }
        if (o->x > terrain.x[TERRAIN_LENGTH-1]) {
            o->x = terrain.x[TERRAIN_LENGTH-1];
            o->vx = -20;
        }
    } else {
        /* attached tentacles don't fall, but remain with whatever they're */
        /* attached to, usually an octopus head. */
        o->x = o->tsd.tentacle.attached_to->x;
        o->y = o->tsd.tentacle.attached_to->y;
    }

    for (i=0;i<o->tsd.tentacle.nsegs;i++) { /* for each segment of the tentacle.... */
        int da;

        /* set the angular velocity to move the segment towards it's desired angle. */
        t = &o->tsd.tentacle.seg[i];
        if (i==0)
            t->dest_angle = o->tsd.tentacle.angle;

        da = (t->dest_angle - t->angle);
        if (da > 20)
            t->angular_v = 2;
        else if (da < -20)
            t->angular_v = -2;
        else if (da > 5)
            t->angular_v = 1;
        else if (da < 5)
            t->angular_v = -1;
        else
            t->angular_v = 0;
    #if 0   
        if (abs(da) > 10) {
            if (t->angle < t->dest_angle)
                t->angular_v = 2;
            else if (t->angle > t->dest_angle)
                t->angular_v = -2;
        } else {
            if (t->angle < t->dest_angle)
                t->angular_v = 1;
            else if (t->angle > t->dest_angle)
                t->angular_v = -1;
            else
                t->angular_v = 0;
        }
    #endif


        /* don't hyper-rotate segment... */
        if (i != 0 && t->angle > MAX_SEG_ANGLE)
            t->angle = MAX_SEG_ANGLE;
        else if (i != 0 && t->angle < -MAX_SEG_ANGLE)
            t->angle = -MAX_SEG_ANGLE;

        if (i > 15 && (timer & 0x03) == 3)
            continue;
        t->angle += t->angular_v; /* move segment towards desired angle... */
    }

    if (randomn(1000) < 50) { /* every once in awhile... */

        /* change the tentacle's idea of what a desirable angle is. */
        o->tsd.tentacle.angle = TENTACLE_RANGE(o->tsd.tentacle);
        if (o->tsd.tentacle.angle < 0)
            o->tsd.tentacle.angle += 360; 
        if (o->tsd.tentacle.angle > 360)
            o->tsd.tentacle.angle -= 360; 

        /* set the tentacle's segments' desired angles... */
        switch (randomn(11)) {

            /* mostly, set tentacle segment desired angles  */
            /* to random values, but in range... */
        case 4:
            for (i=1;i<o->tsd.tentacle.nsegs;i++) {
                t = &o->tsd.tentacle.seg[i];
                t->dest_angle = randomn(2 * MAX_SEG_ANGLE) - MAX_SEG_ANGLE;
            }
            break;
        case 10: if (o->tsd.tentacle.attached_to == NULL) {
                /* sometimes, straighten a tentacle out, */
                /* and make it jump in the direction */
                /* it's pointing... very weird. */
                for (i=1;i<o->tsd.tentacle.nsegs;i++) {
                    t = &o->tsd.tentacle.seg[i];
                    t->dest_angle = 0;
                }
                o->vx = cosine[o->tsd.tentacle.angle] * 20;
                o->vy = -sine[o->tsd.tentacle.angle] * 20;
            }
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 7:
        case 5:
        case 9:
        case 8: {
                float temper;
                int curvedir = randomn(100) < 50 ? -1 : 1;
                int delta = randomn(10);
                temper = (randomn(40) + 60.0) / 100.0;
                t = &o->tsd.tentacle.seg[o->tsd.tentacle.nsegs-1];
                t->dest_angle = curvedir * MAX_SEG_ANGLE; 
                for (i=o->tsd.tentacle.nsegs-1;i>0;i--) {
                    t = &o->tsd.tentacle.seg[i-1];
                    t[0].dest_angle = t[1].dest_angle * temper + -curvedir * delta; 
                }
                break;
            }
        }
    }
}

void octopus_gun_location(struct game_obj_t *parent, 
    __attribute__ ((unused)) struct game_obj_t *child, int *x, int *y)
{
    *x = parent->x;
    *y = parent->y-65;
}

void octopus_move(struct game_obj_t *o)
{
    int xdist;
    int dvx, dvy, tx, ty;
    int gy, i;

    if (!o->alive)
        return;

    gy = find_ground_level(o, NULL);
    

    if (o->tsd.octopus.awake) {
        dvx = 0; /* make compiler happy */
        dvy = 0; /* make compiler happy */

        {
            int angle = ((timer * 3 + o->number*47) % 360);
            tx = player_target->x + sine[angle] * 250;  
            ty = player_target->y + cosine[angle] * 250;
    
            if (ty > gy)
                ty -= 100;

            o->tsd.octopus.tx = tx;
            o->tsd.octopus.ty = ty;
        }
        /* compute a "desired" vx, vy which is towards the player. */
        if (o->x < tx && tx - o->x > GDB_DX_THRESHOLD)
            dvx = GDB_MAX_VX;
        else if (o->x < tx)
            dvx = 0;
        else if (o->x > tx && o->x - tx > GDB_DX_THRESHOLD)
            dvx = -GDB_MAX_VX;
        else if (o->x > tx)
            dvx = 0;
        if (o->y < ty && ty - o->y > GDB_DY_THRESHOLD)
            dvy = GDB_MAX_VY;
        else if (o->y < ty)
            dvy = 0;
        else if (o->y > ty && o->y - ty > GDB_DY_THRESHOLD)
            dvy = -GDB_MAX_VY;
        else if (o->y > ty)
            dvy = 0;

        /* if we're not already close enough, sometimes choose a destination near */
        /* the player's location. */
        if (abs(player_target->x - tx) > GDB_DX_THRESHOLD ||
            abs(player_target->y - ty) > GDB_DY_THRESHOLD || randomn(100) < 3) {
            o->tsd.octopus.tx = player_target->x + randomn(300)-150;
            o->tsd.octopus.ty = player_target->y + randomn(300)-150;
        }

        /* don't sink through the ground... */
        if (o->y > gy - 100 && dvy > -3)
            dvy = -10;  

        /* adjust velocity to make it close to the desired vx,vy computed earlier. */
        if (o->vx < dvx)
            o->vx++;
        else if (o->vx > dvx)
            o->vx--;
        if (o->vy < dvy)
            o->vy++;
        else if (o->vy > dvy)
            o->vy--;

        o->x += o->vx; /* move... */
        o->y += o->vy;

        /* put out some exhaust in the direction opposite what we're moving. */ 
        explode(o->x - dvx + randomn(4)-2, o->y - dvy + randomn(4)-2, -dvx, -dvy, 4, 8, 9);
    }

    /* see if we're close to the player, and if so, and we're not awake, then wake up. */   
    xdist = abs(o->x - player_target->x);
    if (xdist < GDB_LAUNCH_DIST) {
#if 1
        if (randomn(1000) < SAM_LAUNCH_CHANCE && timer >= o->missile_timer) {
            /* wwviaudio_add_sound(SAM_LAUNCH_SOUND);*/
            /*add_harpoon(o->x+10, o->y, 0, 0, 300, MAGENTA, player, o);*/
            /* o->missile_timer = timer + MISSILE_FIRE_PERIOD;*/
            if (!o->tsd.octopus.awake) {
                o->tsd.octopus.awake = 1;
                o->tsd.octopus.tx = player_target->x + randomn(200)-100;
                o->tsd.octopus.ty = player_target->y + randomn(200)-100;
            }
        }
#endif
    }

    /* move the tentacles along with us. */
    for (i=0;i<8;i++) {
        if (o->tsd.octopus.tentacle[i]) {
            o->tsd.octopus.tentacle[i]->x = o->x + ((i-3)*4);
            o->tsd.octopus.tentacle[i]->y = o->y + 5;
        }
    }
}

static void add_floater_message(int x, int y, char *msg);
void cron_move(struct game_obj_t *o)
{
    int xdist, ydist;
    int dvx, dvy, tx, ty;
    int gy, dgy, xi;
    int done, dist2, i;

    if (!o->alive)
        return;

    /* Try to find a human that you want to abduct... */
    if (o->tsd.cron.state == CRON_STATE_SEEKING_HUMAN && 
        (o->tsd.cron.myhuman == NULL) &&
        ((timer & 0x0f) == 0)) {

        /* check for unpursued humans. */
        for (i=0;i<level.nhumanoids;i++) {
            if (human[i] == NULL)
                continue;
            if (human[i]->tsd.human.abductor == NULL) {
                int hvx;

                /* If the human is already at the volcano, don't consider him. */
                hvx = abs(human[i]->x - volcano_obj->x);
                if (hvx < 20)
                    continue;
            
                /* Found an un-abducted human, not too close to the volcano */  
                /* go after him. */
                o->tsd.cron.myhuman = human[i];
                human[i]->tsd.human.abductor = o;
                human[i]->tsd.human.picked_up = 0;
                o->tsd.cron.tx = human[i]->x;
                o->tsd.cron.ty = human[i]->y;
                break;
            }
        }
    }

    done = 0;
    gy = find_ground_level(o, NULL);

    tx = o->tsd.cron.tx;
    ty = o->tsd.cron.ty;

    /* See how far we still have to travel... */
    xdist = abs(o->x - tx);

    if (xdist < CRON_DX_THRESHOLD * 2)
        /* It's not far, don't screw around, just go there... */
        o->tsd.cron.tmp_ty_offset = 0;
    else {
        /* Still pretty far away, so, sometimes... */
        if (randomn(100) <= 30)
            /* screw around a bit and offset our y dest by a bit. */
            /* This makes us bob up and down a bit, a less easy target. */
            o->tsd.cron.tmp_ty_offset = -randomn(150);
    }

    /* if we made it to our dest, pick a new dest. */
    if (xdist <= CRON_DX_THRESHOLD  && abs(o->y - ty) <= CRON_DY_THRESHOLD ) {
        /* pick a new target location */
        done = 1;


        /* See if we have carried a human to the volcano... */
        if (o->tsd.cron.state == CRON_STATE_CARRYING_HUMAN_TO_VOLCANO &&
            o->tsd.cron.myhuman != NULL &&      /* carrying human? */
            o->tsd.cron.myhuman->tsd.human.picked_up == 1 &&    /* human is being carried? */
            o->tsd.cron.myhuman->tsd.human.abductor == o && /* by me? */
            o->tsd.cron.tx == volcano_obj->x) { /* we're headed to the volcano? */

            /* We have carried a human to the volcano.  Drop him in. */

            o->tsd.cron.myhuman->tsd.human.abductor = NULL; /* human isn't abducted anymore. */
            o->tsd.cron.myhuman->x = o->x;        /* drop him off _here_ */
            o->tsd.cron.myhuman->y = o->y;
            o->tsd.cron.myhuman->tsd.human.picked_up = 0;   /* he's not picked up. */
            o->tsd.cron.myhuman->tsd.human.on_ground = 0;   /* he's not on the ground. */
            o->tsd.cron.myhuman = NULL;
            o->tsd.cron.state = CRON_STATE_SEEKING_HUMAN; /* start looking for another one. */
            wwviaudio_add_sound(SCREAM_SOUND);
        } else {
        
            /* Pick a new random destination... */  
            o->tsd.cron.tx = terrain.x[randomn(TERRAIN_LENGTH-MAXBUILDING_WIDTH-1)];
            tx = o->tsd.cron.tx;
            dgy = ground_level(tx, &xi, NULL);
            o->tsd.cron.ty = dgy-140;
            ty = o->tsd.cron.ty;
            o->tsd.cron.tmp_ty_offset = 0;

            /* If we are seeking a human... */
            if (o->tsd.cron.myhuman != NULL && 
                o->tsd.cron.state == CRON_STATE_SEEKING_HUMAN) {
                /* Check to see if we've reached him. */
                ydist = abs(o->y - o->tsd.cron.myhuman->y); 
                xdist = abs(o->x - o->tsd.cron.myhuman->x); 
                if (o->tsd.cron.myhuman->tsd.human.picked_up == 0 &&
                    xdist <= CRON_DX_THRESHOLD && ydist <= CRON_DY_THRESHOLD) {

                    /* We got him... pick him up. */
                    o->tsd.cron.myhuman->tsd.human.picked_up = 1;
                    o->tsd.cron.myhuman->tsd.human.on_ground = 0;
                    o->tsd.cron.state = CRON_STATE_CARRYING_HUMAN_TO_VOLCANO;
                    o->tsd.cron.tx = volcano_obj->x;
                    o->tsd.cron.ty = volcano_obj->y - 150;
                    o->tsd.cron.myhuman->tsd.human.seat_number = 0;
                    wwviaudio_add_sound(ABDUCTED_SOUND);

                    /* Take him to the volcano. */
                    o->tsd.cron.state = CRON_STATE_CARRYING_HUMAN_TO_VOLCANO;
                }
            }
        }
    }


    /* If it's a ways off to our destination, screw around a little bit */
    /* with the y coord of the destination to make us bob up and down */
    if (xdist > CRON_DX_THRESHOLD) {
        ty += o->tsd.cron.tmp_ty_offset;

        /* avoid the ground */
        if (ty > gy-15 && xdist)
            ty = gy-15;
    }
        else ty = o->tsd.cron.ty;

    
    /* move towards the destination */  
    if (o->x < tx && tx - o->x > CRON_DX_THRESHOLD)
        dvx = CRON_MAX_VX;
    else if (o->x > tx && o->x - tx > CRON_DX_THRESHOLD)
        dvx = -CRON_MAX_VX;
    else
        dvx = 0;
    if (o->y < ty && ty - o->y > CRON_DY_THRESHOLD)
        dvy = CRON_MAX_VY;
    else if (o->y > ty && o->y - ty > CRON_DY_THRESHOLD)
        dvy = -CRON_MAX_VY;
    else
        dvy = 0;


    /* watch out for bug...  This isn't fixed, but don't know how we get in here.. */
    if (done && dvx == 0 && dvy == 0) {
        /* printf("Arg!  Stuck.  tx=%d,ty=%d, x=%d, y=%d\n", 
            tx, ty, o->x, o->y); */
        dvy = -CRON_MAX_VY;
        dvx = -CRON_MAX_VX;
    }

    /* should we take a shot at the player? */
    if (randomn(100) <= CRON_SHOT_CHANCE && timer % 10 == 0) {
        dist2 = (player_target->x - o->x) * (player_target->x - o->x) + 
            (player_target->y - o->y) * (player_target->y - o->y);
        if (dist2 <= CRON_SHOT_DIST_SQR) {
            int dx, dy, vx, vy;
            /* calculate vx, vy by similar triangles */
            dx = (player_target->x + player_target->vx * BULLET_LEAD_TIME) - o->x;
            dy = (player_target->y + player_target->vy * BULLET_LEAD_TIME) - o->y;
            if (dx == 0 && dy == 0) {
                vx = 0;
                vy = 0;
            } else if (abs(dx) > abs(dy)) {
                vx = ((dx < 0) ? -1 : 1) * BULLET_SPEED;
                vy = (dy * BULLET_SPEED)/(abs(dx));
            } else {
                vy = ((dy < 0) ? -1 : 1) * BULLET_SPEED;
                vx = (dx * BULLET_SPEED)/(abs(dy));
            }
            explode(o->x, o->y, o->vx, o->vy, 4, 8, 9);
            add_bullet(o->x, o->y, vx, vy, 40, WHITE, player_target);
            /* add_floater_message(o->x, o->y, "Bang!"); */
            wwviaudio_add_sound(CRONSHOT);
        }
    }
    
#if 0
    if (abs(player->x - tx) > CRON_DX_THRESHOLD ||
        abs(player->y - ty) > CRON_DY_THRESHOLD || randomn(100) < 3) {
        o->tsd.gdb.tx = player->x + randomn(300)-150;
        o->tsd.gdb.ty = player->y + randomn(300)-150;
    }
#endif

    /* if (o->y > gy - 100 && dvy > -3)
        dvy = -10;   */

    /* adjust velocity towards desired velocity. */
    if (o->vx < dvx)
        o->vx++;
    else if (o->vx > dvx)
        o->vx--;
    if (o->vy < dvy)
        o->vy++;
    else if (o->vy > dvy)
        o->vy -= 2;

    o->x += o->vx; /* move... */
    o->y += o->vy;

    o->vy++; /* gravity */

    /* move the beam around */
    if (randomn(100) <= 3) {
        switch (randomn(10)) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5: o->tsd.cron.beam_speed = randomn(24) - 12;
                break;
            default:
                o->tsd.cron.beam_speed = 0; /* turn off beam */
        }
    }
    /* don't let the beam get too far to the left or right of the cron job. */
    if (((o->tsd.cron.beam_speed < 0 && o->tsd.cron.beam_pos < -300)) || 
        ((o->tsd.cron.beam_speed > 0 && o->tsd.cron.beam_pos > 300)))
        o->tsd.cron.beam_speed = -o->tsd.cron.beam_speed;

    /* move the beam. */
    o->tsd.cron.beam_pos += o->tsd.cron.beam_speed;
        
    /* explode(o->x - dvx + randomn(4)-2, o->y - dvy + randomn(4)-2, -dvx, -dvy, 4, 8, 9); */
#if 0   
    xdist = abs(o->x - player->x);
    if (xdist < GDB_LAUNCH_DIST) {
        ydist = o->y - player->y;
        if (randomn(1000) < SAM_LAUNCH_CHANCE && timer >= o->missile_timer) {
            wwviaudio_add_sound(SAM_LAUNCH_SOUND);
            add_harpoon(o->x+10, o->y, 0, 0, 300, MAGENTA, player, o);
            o->missile_timer = timer + MISSILE_FIRE_PERIOD;
            if (!o->tsd.gdb.awake) {
                o->tsd.gdb.awake = 1;
                o->tsd.gdb.tx = player->x + randomn(200)-100;
                o->tsd.gdb.ty = player->y + randomn(200)-100;
            }
        }
    }
#endif

#if 0
    if (o->y >= gy + 3) {
        o->alive = 0;
        explode(o->x, o->y, o->vx, 1, 70, 150, 20);
        o->destroy(o);
    }
#endif
}

void pilot_move(struct game_obj_t *o)
{
    int dvx, dvy, tx, ty;
    int gy = 32756; /* big number. */
    int exhaust_dx;

    if (!o->alive)
        return;

    gy = find_ground_level(o, NULL);
    dvx = 0; /* make compiler happy */
    dvy = 0; /* make compiler happy */  

    /* compute a desired velocity which will move towards target coords */
    tx = o->tsd.gdb.tx;
    ty = o->tsd.gdb.ty;
    if (o->x < tx && tx - o->x > GDB_DX_THRESHOLD)
        dvx = JETPILOT_MAX_VX;
    else if (o->x < tx)
        dvx = 0;
    else if (o->x > tx && o->x - tx > GDB_DX_THRESHOLD)
        dvx = -JETPILOT_MAX_VX;
    else if (o->x > tx)
        dvx = 0;
    if (o->y < ty && ty - o->y > JETPILOT_BIG_DY_THRESHOLD)
        dvy = JETPILOT_MAX_VY;
    else if (o->y < ty && ty - o->y > JETPILOT_SMALL_DY_THRESHOLD)
        dvy = 3;
    else if (o->y < ty)
        dvy = 0;
    else if (o->y > ty && o->y - ty > JETPILOT_BIG_DY_THRESHOLD)
        dvy = -5;
    else if (o->y > ty && o->y - ty > JETPILOT_SMALL_DY_THRESHOLD)
        dvy = -3;
    else if (o->y > ty)
        dvy = 0;

#if 0
        /* If we aren't close to the destination, every once in awhile, */
        /* change the distination a bit.  This makes it a little less predictable */
        if (abs(player->x - tx) > GDB_DX_THRESHOLD ||
            abs(player->y - ty) > GDB_DY_THRESHOLD || randomn(100) < 3) {
            o->tsd.gdb.tx = player->x + randomn(300)-150;
            o->tsd.gdb.ty = player->y + randomn(300)-150;
        }
#endif

    {
        int angle = (((timer >> (o->number & 0x01)) * (3 + (o->number & 0x01)) + o->number) % 360);

        if (o->number & 0x01)
            angle = 360 - angle;

        tx = player_target->x + sine[angle] * (250 + (o->number & 0x3f));
        ty = player_target->y + cosine[angle] * (250 + (o->number & 0x3f));

        if (ty > gy)
            ty -= 100;

        o->tsd.gdb.tx = tx;
        o->tsd.gdb.ty = ty;
    }

    /* avoid the ground. */
    if (o->y > gy - 200 && o->vy > 0)
        dvy = -JETPILOT_MAX_VY; 

    /* adjust velocity towards desired velocity. */
    if (o->vx < dvx)
        o->vx++;
    else if (o->vx > dvx)
        o->vx--;
    /* let gravity do this. */
    /* if (o->vy < dvy)
        o->vy++; */
    else if (o->vy > dvy) {
        o->vy -= 2;
        if (o->vy - dvy > 5)
            o->vy -= 2;
        if (o->vy - dvy > 10)
            o->vy -= 2;
    }

    o->x += o->vx; /* move... */
    o->y += o->vy;

    o->vy++; /* gravity */

    if (randomn(5) == 1) /* just to make things non-deterministic */
        o->vy--;

    /* make him face the right way, the way he's trying to go. */
    if (dvx >= 0) {
        o->v = &jetpilot_vect_right;
        exhaust_dx = 3;
    } else if (dvx < 0) {
        o->v = &jetpilot_vect_left;
        exhaust_dx = 3;
    }

    /* shoot out some exhaust, only if we're trying to go "more up." */ 
    if (o->vy > dvy)
        explode(o->x + exhaust_dx, o->y, -o->vx, 7, 4, 8, 9);

    /* See if we should take a shot at the player... */
    if (abs(o->x - player_target->x) < 1000 && 
        abs(o->y - player_target->y) < 800 &&
        randomn(1000) < (level.jetpilot_firechance)) {
        int vx, vy;
        aim_vx_vy(player_target, o, 28, 10, &vx, &vy);
        add_laserbolt(o->x, o->y, vx, vy, GREEN, 50);
        wwviaudio_add_sound(FLAK_FIRE_SOUND);
    }

#if 0
    /* launch a missile? */ 
    xdist = abs(o->x - player->x);
    if (xdist < GDB_LAUNCH_DIST) {
        ydist = o->y - player->y;
        if (randomn(1000) < SAM_LAUNCH_CHANCE && timer >= o->missile_timer) {
            wwviaudio_add_sound(SAM_LAUNCH_SOUND);
            add_harpoon(o->x+10, o->y, 0, 0, 300, RED, player, o);
            o->missile_timer = timer + MISSILE_FIRE_PERIOD;
            if (!o->tsd.gdb.awake) {
                /* if we weren't awake when we fired the missile, we are now. */
                o->tsd.gdb.awake = 1;
                o->tsd.gdb.tx = player->x + randomn(200)-100;
                o->tsd.gdb.ty = player->y + randomn(200)-100;
            }
        }
    }
#endif
    /* If GDB's smash into the ground... they die. */
    /* Somewhat counteracts their bad-assitude.... */
    if (o->y >= gy + 3) {
        remove_target(o);
        kill_object(o);
        explosion(o->x, o->y, o->vx, 1, 70, 150, 20);
        o->destroy(o);
    }
}

void gdb_move(struct game_obj_t *o)
{
    int xdist;
    int dvx, dvy, tx, ty;
    int gy = 32756; /* big number. */

    if (!o->alive)
        return;

    /* If a gravity bomb has picked up the gdb, wake him up. */
    if ((o->vx != 0 || o->vy != 0) && !o->tsd.gdb.awake)
        o->tsd.gdb.awake = 1;

    if (o->tsd.gdb.awake) {
        gy = find_ground_level(o, NULL);
    
        dvx = 0; /* make compiler happy */
        dvy = 0; /* make compiler happy */  

        /* compute a desired velocity which will move towards target coords */
        tx = o->tsd.gdb.tx;
        ty = o->tsd.gdb.ty;
        if (o->x < tx && tx - o->x > GDB_DX_THRESHOLD)
            dvx = GDB_MAX_VX;
        else if (o->x < tx)
            dvx = 0;
        else if (o->x > tx && o->x - tx > GDB_DX_THRESHOLD)
            dvx = -GDB_MAX_VX;
        else if (o->x > tx)
            dvx = 0;
        if (o->y < ty && ty - o->y > GDB_DY_THRESHOLD)
            dvy = GDB_MAX_VY;
        else if (o->y < ty)
            dvy = 0;
        else if (o->y > ty && o->y - ty > GDB_DY_THRESHOLD)
            dvy = -GDB_MAX_VY;
        else if (o->y > ty)
            dvy = 0;
#if 0
        /* If we aren't close to the destination, every once in awhile, */
        /* change the distination a bit.  This makes it a little less predictable */
        if (abs(player->x - tx) > GDB_DX_THRESHOLD ||
            abs(player->y - ty) > GDB_DY_THRESHOLD || randomn(100) < 3) {
            o->tsd.gdb.tx = player->x + randomn(300)-150;
            o->tsd.gdb.ty = player->y + randomn(300)-150;
        }
#endif

        {
            int angle = ((timer * 3 + o->number*47) % 360);
            tx = player_target->x + sine[angle] * 250;  
            ty = player_target->y + cosine[angle] * 250;
    
            if (ty > gy)
                ty -= 100;

            o->tsd.gdb.tx = tx;
            o->tsd.gdb.ty = ty;
        }

        /* avoid the ground. */
        if (o->y > gy - 100 && dvy > -3)
            dvy = -10;  

        /* adjust velocity towards desired velocity. */
        if (o->vx < dvx)
            o->vx++;
        else if (o->vx > dvx)
            o->vx--;
        if (o->vy < dvy)
            o->vy++;
        else if (o->vy > dvy)
            o->vy -= 2;

        o->x += o->vx; /* move... */
        o->y += o->vy;

        o->vy++; /* gravity */

        /* shoot out some exhaust. */   
        explode(o->x - dvx + randomn(4)-2, o->y - dvy + randomn(4)-2, -dvx, -dvy, 4, 8, 9);
    }

    /* launch a missile? */ 
    xdist = abs(o->x - player_target->x);
    if (xdist < GDB_LAUNCH_DIST) {
        if (randomn(1000) < SAM_LAUNCH_CHANCE && timer >= o->missile_timer) {
            wwviaudio_add_sound(SAM_LAUNCH_SOUND);
            add_harpoon(o->x+10, o->y, 0, 0, 300, RED, player_target, o);
            o->missile_timer = timer + MISSILE_FIRE_PERIOD;
            if (!o->tsd.gdb.awake) {
                /* if we weren't awake when we fired the missile, we are now. */
                o->tsd.gdb.awake = 1;
                o->tsd.gdb.tx = player_target->x + randomn(200)-100;
                o->tsd.gdb.ty = player_target->y + randomn(200)-100;
            }
        }
    }

    /* If GDB's smash into the ground... they die. */
    /* Somewhat counteracts their bad-assitude.... */
    if (o->y >= gy + 3) {
        remove_target(o);
        kill_object(o);
        explosion(o->x, o->y, o->vx, 1, 70, 150, 20);
        o->destroy(o);
    }
}

void humanoid_move(struct game_obj_t *o)
{
    int xdist, ydist;
    struct game_obj_t *abductor;

    if (!o->alive) {
        printf("Bug... Dead human %d\n", o->tsd.human.human_number);
        return;
    }
    if (o->alive != 1) printf("human%d, alive=%d\n", o->tsd.human.human_number, o->alive);

    /* humans move around with their abductors. */
    abductor = o->tsd.human.abductor;
    if (o->tsd.human.picked_up && abductor != NULL) {
        if (abductor->otype == OBJ_TYPE_PLAYER) {
            o->x = 10 + abductor->x + (o->tsd.human.seat_number * 8) 
                - (game_state.humanoids * 4);
            o->y = o->tsd.human.abductor->y + 20;
        } else if (abductor->otype == OBJ_TYPE_REINDEER) {
            o->x = 10 + abductor->x + (o->tsd.human.seat_number * 8) 
            - (game_state.humanoids * 4);
            o->y = o->tsd.human.abductor->y + 3;
        } else {
            o->x = abductor->x;
            o->y = o->tsd.human.abductor->y + 30;
        }
    }

    /* not on ground, not picked up -- we're falling... */
    if (o->tsd.human.on_ground == 0 && o->tsd.human.picked_up == 0) {
        int gy;
        /* we got dropped */
        if ((timer & 0x03) == 0)
            o->vy++; /* make them easier to catch. */
        o->y += o->vy;
        o->x += o->vx;
        gy = find_ground_level(o, NULL);
        if (gy == GROUND_OOPS || o->y >= gy) {
            o->y = gy;
            o->tsd.human.on_ground = 1;
            wwviaudio_add_sound(BODYSLAM_SOUND);
            if (abs(o->x - volcano_obj->x) > 20)
                wwviaudio_add_sound(OOF_SOUND);
            else {
                wwviaudio_add_sound(IT_BURNS);
                remove_target(o);
                kill_object(o);
                game_state.score -= score_table[OBJ_TYPE_HUMAN];
                return;
            }
        }
    }

    xdist = abs(o->x - player_target->x);
    ydist = abs(o->y - player_target->y);
    if (xdist < HUMANOID_DIST && o->tsd.human.abductor != player_target) {
        if (ydist < HUMANOID_DIST && o->tsd.human.picked_up == 0 && /* close enough for pickup? */
            game_state.health > 0) {
            if (o->tsd.human.on_ground == 0) { /* midair catch */
                add_floater_message(o->x, o->y + 25, "Nice Catch! +1000!");
                game_state.score += 1000;
            }
            wwviaudio_add_sound(CARDOOR_SOUND);
            wwviaudio_add_sound(WOOHOO_SOUND);
            add_floater_message(o->x, o->y, "Woohoo!");
            o->x = -1000; /* take him off screen. */
            o->y = -1000;
            o->tsd.human.abductor = player_target;
            o->tsd.human.picked_up = 1;
            o->tsd.human.seat_number = game_state.humanoids;
            o->tsd.human.on_ground = 0;
            game_state.score +=  score_table[OBJ_TYPE_HUMAN];
            game_state.humanoids++;
        } else {
            /* counter tracks if player has left the general vicinity of the human lately. */
            /* keeps human from saying "Help up/down here!" more than once at a time. */
            /* If the player is in the middle of falling, he doesn't say anything. */ 
            /* Too busy screaming. */
            if (o->counter == 0 && (o->tsd.human.on_ground != 0 || 
                o->tsd.human.picked_up != 0)) {
                add_floater_message(o->x, o->y, "Help!");
                if (o->y > player_target->y)
                    wwviaudio_add_sound(HELPDOWNHERE_SOUND);
                else
                    wwviaudio_add_sound(HELPUPHERE_SOUND);
                o->counter = 1;
            }
        }
    } else if (xdist > 1000) 
        o->counter = 0; /* reset counter if player is far enough away. */
}

void advance_level();
void socket_move(struct game_obj_t *o)
{
    int xdist, ydist;
    if (!o->alive)
        return;

    /* see if player entered ths socket... */
    xdist = abs(o->x - player->x);
    ydist = abs(o->y - player->y);
    /* HUMANOID_DIST is close enough */
    if (xdist < HUMANOID_DIST*3 && ydist < HUMANOID_DIST*2  && timer_event != START_INTERMISSION_EVENT) 
    {
        gettimeofday(&game_state.finish_time, NULL);
        wwviaudio_add_sound(WOOHOO_SOUND);
        player->tsd.epd.count = 50;
        player->tsd.epd.count2 = 0;
        player->vx = 0;
        player->vy = 0;
        player->x = 0; /* so we won't enter the socket again immediately */
        timer_event = START_INTERMISSION_EVENT;
        next_timer = timer + 1;
        /* advance_level(); */
    }
    if (xdist < SCREEN_WIDTH)
        o->color = randomn(NCOLORS + NSPARKCOLORS + NRAINBOWCOLORS);
}

static inline void free_object(int i)
{
    game_state.go[i].alive = 0;
#ifdef DEBUG_TARGET_LIST
    if (game_state.go[i].ontargetlist ||
        game_state.go[i].next) {
            printf("x\n");
    }
#endif
    free_obj_bitmap[i >> 5] &= ~(1 << (i % 32)); /* clear the proper bit. */
}

static inline void age_object(struct game_obj_t *o)
{
    o->alive--;
    if (o->alive <= 0) {
        if (o->ontargetlist)
            remove_target(o);
        free_object(o->number); 
    }
}

static inline void kill_object(struct game_obj_t *o)
{
    o->alive = 0;
    free_object(o->number);
}

static void kill_and_debris_object(struct game_obj_t *o)
{
    if (o->debris_form != NULL) {
        o->v = o->debris_form;
        o->alive = 1;
        o->otype = OBJ_TYPE_SCENERY;
        o->move = NULL;
        o->draw = draw_generic;
        return;
    }
    kill_object(o);
}

int find_free_obj();

void laser_move(struct game_obj_t *o);
void laser_draw(struct game_obj_t *o);
void generic_destroy_func(struct game_obj_t *o);

void player_fire_laser()
{
    int i;
    struct game_obj_t *o, *p;
    int j;
    int y;

    if (timer < game_state.nextlasertime)
        return;
    game_state.nextlasertime = timer + (frame_rate_hz / 12);

    p = player_target;

    /* Fire laser... cmd muliplier times. */
    y = p->y - ((game_state.cmd_multiplier-1)/2) * 10;
    for (j=0;j<game_state.cmd_multiplier;j++) {
        i = find_free_obj();
        if (i < 0)
            return;
        o = &game_state.go[i];

        o->last_xi = -1;
        o->x = p->x+(0 * game_state.direction);
        o->y = y;
        y += 10;
        o->vx = p->vx + LASER_SPEED * 
            (player_target == player ? game_state.direction : 
            player_target->tsd.reindeer.direction);
        o->vy = 0;
        o->v = &right_laser_vect;
        o->draw = laser_draw;
        o->move = laser_move;
        o->destroy = generic_destroy_func;
        o->otype = OBJ_TYPE_LASER;
        o->color = game_state.nextlasercolor;
        o->alive = 20;
        o->next = NULL;
        o->prev = NULL;
        o->tsd.laser.tail_x = o->x;
    
        game_state.nextlasercolor++;
        if  (game_state.nextlasercolor >= NCOLORS + NSPARKCOLORS + NRAINBOWCOLORS)
            game_state.nextlasercolor = NCOLORS + NSPARKCOLORS;
    }
    game_state.cmd_multiplier = 1;
    wwviaudio_add_sound(PLAYER_LASER_SOUND);
}

int interpolate(int x, int x1, int y1, int x2, int y2)
{
    /* return corresponding y on line x1,y1,x2,y2 for value x */
    /*
        (y2 -y1)/(x2 - x1) = (y - y1) / (x - x1)     by similar triangles.
        (x -x1) * (y2 -y1)/(x2 -x1) = y - y1         a little algebra...
        y = (x - x1) * (y2 - y1) / (x2 -x1) + y1;    I think there's one more step
                                                         which would optimize this a bit more.
                                 but I forget how it goes. 
    */
    if (x2 == x1)
        return y1;
    else
        return (x - x1) * (y2 - y1) / (x2 -x1) + y1;
}

int ground_level(int x, int *xi, int *slope)
{

/* Find the level (y value) of the ground at position x, 
 * return index into terrain array in *xi.  */
    int deepest, i;

    *xi = -1;
    deepest = GROUND_OOPS;

    /* find the terrain segment spanning x. */
    for (i=0;i<TERRAIN_LENGTH-1;i++) {
        if (x >= terrain.x[i] && x < terrain.x[i+1]) {
            *xi = i; /* tell caller the index, and find the exact y value. */
            deepest = interpolate(x, terrain.x[i], terrain.y[i],
                    terrain.x[i+1], terrain.y[i+1]);

            if (slope != NULL) {
                /* calculate the slope of the ground at point of impact. */
                /* we use this later when bouncing things, and making them */
                /* slide downhill. */
                *slope = terrain.slope[i];
            }
            break;
        }
    }
    return deepest;
}

int find_ground_level(struct game_obj_t *o, int *slope)
{

    /* optimized way to find the ground level (y value) at an object's */
    /* x position.  Each object tracks the terrain segment it was last at. */
    /* This means, the linear search of the terrain array is mostly eliminated */

    int xi1, xi2, i;
    xi1 = o->last_xi; /* try the terrain segment we used last time. */
    xi2 = xi1 + 1;

    if (xi1 < 0 || xi2 >= TERRAIN_LENGTH) /* Do we have a last one? No? */
        return ground_level(o->x, &o->last_xi, slope); /* do it the hard way. */

    /* Is the last terrain segment the correct one? */
    if (terrain.x[xi1] <= o->x && terrain.x[xi2] >= o->x) {
        if (slope != NULL) {
            /* calculate the slope of the ground at point of impact. */
            /* we use this later when bouncing things, and making them */
            /* slide downhill. */
            *slope = terrain.slope[xi1];
        }
        return interpolate(o->x, terrain.x[xi1], terrain.y[xi1],  /* do it the easy way. */
                terrain.x[xi2], terrain.y[xi2]);
    }

    /* The last one wasn't correct.  Have to search. */
    /* The correct one is to the left.  Search left. */
    if (terrain.x[xi1] > o->x) {
        for (i=xi1;i>=0;i--) {
            if (o->x >= terrain.x[i] && o->x < terrain.x[i+1]) {
                o->last_xi = i;
                if (slope != NULL) {
                    /* calculate the slope of the ground at point of impact. */
                    /* we use this later when bouncing things, and making them */
                    /* slide downhill. */
                    *slope = terrain.slope[i];
                }
                return interpolate(o->x, terrain.x[i], terrain.y[i],
                        terrain.x[i+1], terrain.y[i+1]);
            }
        }
    } else if (terrain.x[xi2] < o->x) { /* Correct one is to the right. */
        for (i=xi1;i<=TERRAIN_LENGTH-10;i++) { /* search to the right. */
            if (o->x >= terrain.x[i] && o->x < terrain.x[i+1]) {
                o->last_xi = i;
                if (slope != NULL) {
                    /* calculate the slope of the ground at point of impact. */
                    /* we use this later when bouncing things, and making them */
                    /* slide downhill. */
                    *slope = terrain.slope[i];
                }
                return interpolate(o->x, terrain.x[i], terrain.y[i],
                        terrain.x[i+1], terrain.y[i+1]);
            }
        }
    }

    /* What?  there *is* no correct answer.  Object must have fallen off */
    /* the edge of the planet. (it happens sometimes -- though that is a bug. ) */
    o->last_xi = GROUND_OOPS;
    return GROUND_OOPS;
    
}

void generic_destroy_func(__attribute__((unused)) struct game_obj_t *o)
{
    /* so far, nothing needs to be done in this. */
    return;
}

void octopus_destroy(struct game_obj_t *o)
{
    int i;

    /* when an octopus is destroyed, his tentacles aren't.  They fall off */
    /* and proceed on their own. */
    for (i=0;i<8;i++) {
        struct game_obj_t *tentacle;
        tentacle = o->tsd.octopus.tentacle[i];
        if (tentacle) {
            tentacle->tsd.tentacle.attached_to = NULL;
            tentacle->vx = randomn(40)-20;
            tentacle->vy = randomn(40)-20;
            tentacle->tsd.tentacle.upper_angle = 170;
            tentacle->tsd.tentacle.lower_angle = 10;
            o->tsd.octopus.tentacle[i] = NULL;
        }
    }
}

void cron_destroy(struct game_obj_t *o)
{
    /* Drop any human the cron job is carrying. */
    if (o->tsd.cron.myhuman != NULL && 
        o->tsd.cron.myhuman->tsd.human.picked_up &&
        o->tsd.cron.myhuman->tsd.human.abductor == o) {
        o->tsd.cron.myhuman->tsd.human.abductor = NULL;
        o->tsd.cron.myhuman->tsd.human.picked_up = 0;
        o->tsd.cron.myhuman->tsd.human.on_ground = 0;
        /* printf("Human released, alive=%d, x=%d, y=%d.\n", o->tsd.cron.myhuman->x, o->tsd.cron.myhuman->y,
            o->tsd.cron.myhuman->alive); */
        o->tsd.cron.myhuman->vx = o->vx; /* <-- this doesn't seem to be working... FIXME */
        o->tsd.cron.myhuman = NULL;
        wwviaudio_add_sound(SCREAM_SOUND);
    }
    generic_destroy_func(o);
}

void bridge_move(struct game_obj_t *o);
void no_move(struct game_obj_t *o);
static void add_score_floater(int x, int y, int score);
static void spray_debris(int x, int y, int vx, int vy, int r, struct game_obj_t *victim, int metal);
void truss_cut_loose_whats_below(struct game_obj_t *o, struct game_obj_t *bomb);
static void add_pilot(int pilotx, int piloty, int pilotvx, int pilotvy);

void present_move(struct game_obj_t *o)
{
    struct game_obj_t *t;
    int deepest;
    int dist2;
    int do_remove_present;

    if (!o->alive)
        return;
    
    do_remove_present = 0;

    o->x += o->vx;
    o->y += o->vy;
    o->vy++; /* gravity */

    /* Scan the target list to see if we've hit anything. */
    for (t=target_head;t != NULL; t = t->next) {
        if (t == o || t->otype != OBJ_TYPE_HOUSE) /* presents only care about houses */
            continue;
        /* find distance squared... don't take square root. */
        dist2 = (o->x - t->x)*(o->x - t->x) + 
            (o->y - t->y)*(o->y - t->y);
        if (dist2 < LASER_PROXIMITY && t->tsd.house.santa_came == 0) { /* a hit (LASER_PROXIMITY is already squared.) */
            t->tsd.house.santa_came = 1;
            t->v = &house_green_vect;
            game_state.score += DELIVER_PRESENTS_SCORE;
            do_remove_present = 1;
            game_state.houses_gifted++;
            add_floater_message(t->x, t->y, "1000");
            if (game_state.houses_gifted == NHOUSES) {
                add_floater_message(t->x, t->y + 25, "All Houses visited! +1000000!");
                game_state.score += 1000000;
            }
            wwviaudio_add_sound(YAY_SANTA);
            goto out;
        }
    }

    /* Detect smashing into the ground */
    deepest = find_ground_level(o, NULL);
    if (deepest != GROUND_OOPS && o->y > deepest) {
        do_remove_present = 1;
        /* find nearby targets */
        for (t=target_head;t != NULL; t = t-> next) {
            if (t->otype != OBJ_TYPE_HOUSE) /* presents only care for houses */
                continue;
            dist2 = (o->x - t->x)*(o->x - t->x) + 
                (o->y - t->y)*(o->y - t->y);
            if (dist2 < BOMB_PROXIMITY && t->tsd.house.santa_came == 0) { /* a hit */
                t->tsd.house.santa_came = 1;
                game_state.houses_gifted++;
                game_state.score += DELIVER_PRESENTS_SCORE;
                t->v = &house_green_vect;
                add_floater_message(t->x, t->y, "1000");
                if (game_state.houses_gifted == NHOUSES) {
                    add_floater_message(t->x, t->y + 25, "All Houses visited! +1000000!");
                    game_state.score += 1000000;
                }
                wwviaudio_add_sound(YAY_SANTA);
                goto out;
            }
        }
    }

out:
    if (do_remove_present) {
        remove_target(o);
        kill_object(o);
        o->destroy(o);
    }
}

void nice_bank_shot(struct game_obj_t *bomb)
{
    static int bank_shot_sound_time = 0;
    if (bomb == NULL)
        return;
    if (bomb->tsd.bomb.bank_shot_factor == 1)
        return;
    add_floater_message(player_target->x, player_target->y - 20, "Nice Bank Shot!");
    if (timer > bank_shot_sound_time) {
        bank_shot_sound_time = timer + 30;
        wwviaudio_add_sound(NICE_BANK_SHOT);
    }
}

void timpani_boing(struct game_obj_t *bomb)
{
    static int timpani_time = 0;
    if (bomb->vy > -6) 
        return;
    bomb->tsd.bomb.bank_shot_factor = 10;
    if (timpani_time > timer)
        return;
    timpani_time = timer + 5;
    wwviaudio_add_sound(TIMPANI_BOING);
}

void bomb_move(struct game_obj_t *o);
void disconnect_any_attached_gun(struct game_obj_t *t)
{
    /* Disconnect any attached gun */
    if (t->attached_gun != NULL) {
        t->attached_gun->move = bomb_move; 
        t->tsd.bomb.bank_shot_factor = 1;
        t->attached_gun->tsd.kgun.attached_to = NULL; 
        t->attached_gun = NULL;
    }
    /* if we hit a gun disconnect it */
    if (t->otype == OBJ_TYPE_KGUN && 
        t->tsd.kgun.attached_to != NULL) {
        t->tsd.kgun.attached_to->attached_gun = NULL;
        if (t->tsd.kgun.attached_to->otype == OBJ_TYPE_TRUSS)
            t->tsd.kgun.attached_to->tsd.truss.below = NULL;
        t->tsd.kgun.attached_to = NULL;
    }
    /* actually, the below tesla code never executes because the */
    /* laser and bomb currently cannot hit a tesla coil */
    if (t->otype == OBJ_TYPE_TESLA && 
        t->tsd.tesla.attached_to != NULL) {
        t->tsd.tesla.attached_to->attached_gun = NULL;
        if (t->tsd.tesla.attached_to->otype == OBJ_TYPE_TRUSS)
            t->tsd.tesla.attached_to->tsd.truss.below = NULL;
        t->tsd.tesla.attached_to = NULL;
    }
}

void bomb_move(struct game_obj_t *o)
{
    struct game_obj_t *t, *next;
    int deepest;
    int dist2;
    int removed;
    int do_remove_bomb;
    int addpilot = 0;
    int pilotx, piloty, pilotvx, pilotvy;

    pilotx = piloty = pilotvx = pilotvy = 0;

    if (!o->alive)
        return;
    
    do_remove_bomb = 0;

    o->x += o->vx;
    o->y += o->vy;
    o->vy++; /* gravity */

    /* Scan the target list to see if we've hit anything. */
    for (t=target_head;t != NULL;) {
        if (t == o || t->otype == 'p') { /* bomb_move things can't bomb them selves. */
            t = t-> next;
            continue;
        }
        if (!t->alive) {
            t=t->next;
            continue;
        }
        removed = 0;
        switch (t->otype) {
            case OBJ_TYPE_AIRSHIP:
                if (abs(o->x - t->x) < 3*60 &&  /* hit a blimp or clipper ship? */
                    o->y - t->y <= 0 &&
                    o->y - t->y > -50*3) {
                    if (o->vy > 0) {
                        o->vy = -o->vy * 0.8;
                        timpani_boing(o);
                    }
                    o->vx = o->vx + randomn(6)-3;
                }
                break;
                
            case OBJ_TYPE_KGUN:
            case OBJ_TYPE_TRUSS:
            case OBJ_TYPE_ROCKET:
            case OBJ_TYPE_BIG_ROCKET:
            case OBJ_TYPE_JET:
            case OBJ_TYPE_MISSILE:
            case OBJ_TYPE_HARPOON:
            case OBJ_TYPE_CRON:
            case OBJ_TYPE_GDB:
            case OBJ_TYPE_OCTOPUS:
            case OBJ_TYPE_FUEL:
            case OBJ_TYPE_JAMMER:
            case OBJ_TYPE_GUN:
            case OBJ_TYPE_JETPILOT:
            /* case OBJ_TYPE_BOMB:  no, bomb can't bomb himself... */
            case OBJ_TYPE_SAM_STATION:  {
                /* find distance squared... don't take square root. */
                dist2 = (o->x - t->x)*(o->x - t->x) + 
                    (o->y - t->y)*(o->y - t->y);
                if (dist2 < LASER_PROXIMITY) { /* a hit (LASER_PROXIMITY is already squared.) */

                    if (t->uses_health) {
                        t->health.health -= 1;
                        if (t->health.health > 0) {
                            do_remove_bomb = 1;
                            make_bomb_sound();
                            explosion(t->x, t->y, t->vx, 1, 70, 150, 20);
                            spray_debris(t->x, t->y, t->vx, t->vy, 70, t, 1);
                            /* target object not dead yet. */
                            break;
                        }
                        if (t->otype == OBJ_TYPE_TRUSS)
                            truss_cut_loose_whats_below(t, o);

                        /* if it's a gdb or a jet, then pilot ejects. */
                        if (t->otype == OBJ_TYPE_JET || t->otype == OBJ_TYPE_GDB) {
                            addpilot = 1;
                            pilotx = t->x;
                            piloty = t->y;
                            pilotvx = t->vx;
                            pilotvy = t->vy - 20;
                        }
                    }

                    disconnect_any_attached_gun(t);

                    if (score_table[t->otype] != 0) {
                        game_state.score += score_table[t->otype] * o->tsd.bomb.bank_shot_factor;
                        add_score_floater(t->x, t->y, score_table[t->otype] * 
                            o->tsd.bomb.bank_shot_factor);
                        nice_bank_shot(o);
                    }
                    kill_tally[t->otype]++;
                    make_bomb_sound();
                    explosion(t->x, t->y, t->vx, 1, 70, 150, 20);
                    spray_debris(t->x, t->y, t->vx, t->vy, 70, t, 1);
                    /* t->alive = 0; */
                    next = remove_target(t);
                    kill_and_debris_object(t);
                    t->destroy(t);
                    t = next;
                    removed = 1;
                    do_remove_bomb = 1;
                }
            }
            default:
                break;
        }
        /* Careful.  if we removed a target, then the current one *is* the "next" one. */
        if (!removed)  
            t=t->next;
    }

    /* Detect smashing into the ground */
    deepest = find_ground_level(o, NULL);
    if (deepest != GROUND_OOPS && o->y > deepest) {
        make_bomb_sound();
        explosion(o->x, o->y, o->vx, 1, 90, 150, 20);
        do_remove_bomb = 1;
        /* find nearby targets */
        for (t=target_head;t != NULL;) {
            if (t == o || t->otype == 'p') { /* bomb_move things can't bomb them selves. */
                t = t-> next;
                continue;
            }
            if (!t->alive) {
                t=t->next;
                continue;
            }
            removed = 0;
            switch (t->otype) {
                case OBJ_TYPE_TRUSS: 
                case OBJ_TYPE_TENTACLE: 
                case OBJ_TYPE_ROCKET:
                case OBJ_TYPE_BIG_ROCKET:
                case OBJ_TYPE_HARPOON:
                case OBJ_TYPE_MISSILE:
                case OBJ_TYPE_GDB:
                case OBJ_TYPE_CRON:
                case OBJ_TYPE_OCTOPUS:
                case OBJ_TYPE_GUN:
                case OBJ_TYPE_KGUN:
                case OBJ_TYPE_BOMB:
                case OBJ_TYPE_JAMMER:
                case OBJ_TYPE_SAM_STATION:
                case OBJ_TYPE_JETPILOT:
                case OBJ_TYPE_FUEL: {
                    dist2 = (o->x - t->x)*(o->x - t->x) + 
                        (o->y - t->y)*(o->y - t->y);
                    if (dist2 < BOMB_PROXIMITY) { /* a hit */

                        if (t->uses_health) {
                            t->health.health -= 1;
                            if (t->health.health > 0) {
                                make_bomb_sound();
                                explosion(t->x, t->y, t->vx, 1, 70, 150, 20);
                                spray_debris(t->x, t->y, t->vx, t->vy, 70, t, 1);
                                /* object not dead yet. */
                                break;
                            }
                            if (t->otype == OBJ_TYPE_TRUSS)
                                truss_cut_loose_whats_below(t, o);
                        }

                        disconnect_any_attached_gun(t);

                        /* FIXME -- need to adjust kill counts. */

                        if (score_table[t->otype] != 0) {
                            game_state.score += score_table[t->otype] * 
                                o->tsd.bomb.bank_shot_factor;
                            add_score_floater(t->x, t->y, score_table[t->otype] * 
                                o->tsd.bomb.bank_shot_factor);
                            nice_bank_shot(o);
                        }
                        explosion(t->x, t->y, t->vx, 1, 70, 150, 20);
                        spray_debris(t->x, t->y, t->vx, t->vy, 70, t, 1);
                        next = remove_target(t);
                        kill_and_debris_object(t);
                        /* t->alive = 0; */
                        t->destroy(t);
                        /* if (t->otype == OBJ_TYPE_FUEL) {
                            game_state.health += 10;
                            if (game_state.health > game_state.max_player_health)
                                game_state.health = game_state.max_player_health;
                        } */
                        t = next;
                        removed = 1;
                    }
                }
                break;

                /* this is where pieces of bridges hit by bombs get set in motion. */
                case OBJ_TYPE_BRIDGE:
                case OBJ_TYPE_DEBRIS: /* <--- this doesn't seem to work.  FIXME */
                    if (abs(o->x - t->x) < BOMB_X_PROXIMITY) { /* a hit */
                        /* "+=" instead of "=" in case multiple bombs */
                        if (t->move == no_move) /* only get the points once. */
                            game_state.score += score_table[OBJ_TYPE_BRIDGE];
                        t->vx += ((t->x < o->x) ? -1 : 1) * randomn(6);
                        t->vy += ((t->y < o->y) ? -1 : 1) * randomn(6);
                        t->move = bridge_move;
                        t->alive = 20;
                    }
                    break;
                default:
                    break;
            }
            /* careful, if target removed, next == current. */
            if (!removed)
                t = t->next;
        }
    }
    if (do_remove_bomb) {
        remove_target(o);
        kill_object(o);
        o->destroy(o);
    }
    if (addpilot)
        add_pilot(pilotx, piloty, pilotvx, pilotvy);
}

static void add_spark(int x, int y, int vx, int vy, int time);
static void add_bright_spark(int x, int y, int vx, int vy, int time);
static void add_pixie_dust(int x, int y, int vx, int vy, int time);

#define GRAVITY_BOMB_X_INFLUENCE 400
#define GRAVITY_BOMB_Y_INFLUENCE 400
#define GRAVITY_BOMB_HIT_DIST2 (30*30) 

void gravity_bomb_move(struct game_obj_t *o)
{
    int deepest;
    int dist2;
    struct game_obj_t *t;
    int xdist, ydist;
    int i;

    if (!o->alive)
        return;
    o->x += o->vx;
    o->y += o->vy;
    if (timer & 0x01)
        o->vy++; /* light gravity */

    for (i=0;i<8;i++) {
        int vx, vy, rx, ry;
        /* vx = randomn(10) - (10>>1) + o->vx; */
        /* vy = randomn(10) - (10>>1) + o->vy; */
        rx = randomn(400) - (400>>1);
        ry = randomn(400) - (400>>1);
        vx = ry >> 3;
        vy = rx >> 3;
        add_spark(o->x + rx, o->y + ry, vx, vy, 8); 
    }

    /* start with 1, skip the player. */
    for (i=1;i<MAXOBJS;i++) {
        t = &game_state.go[i];
        if (t == player)
            continue;
        if (t == o) /* bomb_move things can't bomb themselves. */
            continue;
        if (!t->alive)
            continue;

        xdist = t->x - o->x;
        if (abs(xdist) > GRAVITY_BOMB_X_INFLUENCE)
            continue;

        ydist = t->y - o->y;
        if (abs(ydist) > GRAVITY_BOMB_Y_INFLUENCE)
            continue;
        
        switch (t->otype) {
            case OBJ_TYPE_BRIDGE:
                t->move = bridge_move;
            case OBJ_TYPE_SPARK:
            case OBJ_TYPE_PIXIE_DUST:
            case OBJ_TYPE_ROCKET:
            case OBJ_TYPE_BIG_ROCKET:
            case OBJ_TYPE_MISSILE:
            case OBJ_TYPE_HARPOON:
            case OBJ_TYPE_CRON:
            case OBJ_TYPE_DEBRIS:
            case OBJ_TYPE_GDB:
            case OBJ_TYPE_SYMBOL:
            case OBJ_TYPE_OCTOPUS: {
                int dvx, dvy;

                aim_vx_vy(o, t, 10, 0, &dvx, &dvy);

                t->vx += dvx;
                t->vy += dvy;
                t->x += dvx;
                t->y += dvy;

                /* find distance squared... don't take square root. */
                dist2 = (xdist * xdist + ydist * ydist);
                if (dist2 < GRAVITY_BOMB_HIT_DIST2) {

                    disconnect_any_attached_gun(t);

                    if (score_table[t->otype] != 0) {
                        game_state.score += score_table[t->otype];
                        add_score_floater(t->x, t->y, score_table[t->otype]);
                    }
                    kill_tally[t->otype]++;
                    /* make_bomb_sound();*/
                    /* explode(t->x, t->y, t->vx, 1, 70, 150, 20);*/
                    /* spray_debris(t->x, t->y, t->vx, t->vy, 70, t, 1);*/
                    remove_target(t);
                    t->destroy(t);
                    kill_object(t);
                }
            }
            default:
                break;
        }
    }

    /* Detect smashing into the ground */
    deepest = find_ground_level(o, NULL);
    if ((deepest != GROUND_OOPS && o->y > deepest) || !o->alive) {
        remove_target(o);
        kill_object(o);
        o->destroy(o);
    }
}

/* static void corrosive_atmosphere_sound(); */
void volcano_move(struct game_obj_t *o)
{
    int player_dist;

    player_dist = abs(player->x - o->x);
    if (timer % (frame_rate_hz*2) == 0) {
        if (randomn(100) < 25)  {
            spray_debris(o->x, o->y, 0, -40, 40, o, 0);
            spray_debris(o->x, o->y, 0, -30, 30, o, 0);
            spray_debris(o->x, o->y, 0, -20, 20, o, 0);
            spray_debris(o->x, o->y, 0, -10, 10, o, 0);
            if (player_dist < 1500)
                wwviaudio_add_sound(VOLCANO_ERUPTION);
        } else {
            if (randomn(100) < 50) {
                spray_debris(o->x, o->y, 0, -10, 10, o, 0);
                if (player_dist < 1500)
                    wwviaudio_add_sound(VOLCANO_ERUPTION);
            }
        }
    }
    if (player_dist < 250) {
        game_state.corrosive_atmosphere = 1;
        /* corrosive_atmosphere_sound(); */
        if ((timer & 0x03) == 3 && game_state.max_player_health >= 50) 
            game_state.health--;
    } else
        game_state.corrosive_atmosphere = 0;
}

/* Consider removing the entire concept of chaff from the game. */
/* I think it may harm game play fun, by confusing the missiles */
/* too easily.  At the very least, fix the missiles so they */
/* aren't *permanently* confused by the chaff. */
void chaff_move(struct game_obj_t *o)
{
    int deepest;

    if (!o->alive)
        return;
    o->x += o->vx;
    o->y += o->vy;
    o->vy++; /* gravity */
    age_object(o);

    /* air resistance reduces horizontal velocity with time. */
    if (o->vx > 0)
        o->vx--;
    else if (o->vx < 0)
        o->vx++;

    explode(o->x, o->y, 0, 0, 10, 7, 19);   /* throw some sparks.*/
    deepest = find_ground_level(o, NULL);   /* Detect smashing into the ground */
    if (deepest != GROUND_OOPS && o->y > deepest) {
        kill_object(o);
        o->destroy(o);
    }
    if (!o->alive) {
        remove_target(o);
    }
}

void drop_chaff()
{
    int j, i[3];
    struct game_obj_t *o;
    struct game_obj_t *t;

    if (game_state.nextchafftime > timer)
        return;
    game_state.nextchafftime = timer + (frame_rate_hz);

    /* Throw three pieces of chaff each time... */
    for (j=0;j<3;j++) {
        i[j] = find_free_obj();
        if (i[j] < 0)
            return;
        o = &game_state.go[i[j]];
        o->last_xi = -1;
        o->x = player->x;
        o->y = player->y;
        o->vx = player->vx + ((j-1) * 7); /* -7, 0, 7 are x velocities */
        o->vy = player->vy + 7;
        o->v = &spark_vect;
        o->draw = draw_generic;
        o->move = chaff_move;
        o->otype = OBJ_TYPE_CHAFF;
        add_target(o);
        o->color = ORANGE;
        o->alive = 25;
    }
    /* find any missiles in the vicinity */
    for (t=target_head;t != NULL;t=t->next) {
        if (t->otype != OBJ_TYPE_MISSILE)
            continue;
        /* make the missile chase the chaff instead of the player -- sometimes. */
        if (t->bullseye == player) {
            j = randomn(3);
            if (j >= 0 && j <= 3 && i[j] > 0 && randomn(100) < 50)
                t->bullseye = &game_state.go[i[j]];
        }
    }
    /* FIXME Bug: when (bullseye->alive == 0) some new object will allocate there
       and become the new target... probably a spark.  The missiles will not 
       re-aquire the proper target. */
}

void drop_bomb()
{
    int i, j;
    struct game_obj_t *o;

    if (game_state.nextbombtime > timer)
        return;
    game_state.nextbombtime = timer + (frame_rate_hz >> 2);

    /* Player drops cmd_multiplier bombs.  */
    for (j=0;j<game_state.cmd_multiplier;j++) {
        if (game_state.nbombs == 0)
            return;
        game_state.nbombs--;
    
        i = find_free_obj();
        if (i < 0)
            return;

        o = &game_state.go[i];
        o->last_xi = -1;
        o->x = player_target->x+(5 * game_state.direction);
        o->y = player_target->y;
        o->vx = player->vx + BOMB_SPEED * 
            (player_target == player ? game_state.direction :
                player_target->tsd.reindeer.direction) + (j*3*game_state.direction);
        o->vy = player->vy;
        o->v = &bomb_vect;
        o->move = bomb_move;
        o->draw = draw_generic;
        o->destroy = generic_destroy_func;
        o->otype = OBJ_TYPE_BOMB;
        add_target(o);
        o->color = ORANGE;
        o->alive = 20;
        o->tsd.bomb.bank_shot_factor = 1;
    }
    game_state.cmd_multiplier = 1;
}

void drop_present()
{
    int i, j;
    static int hohoho_time = 0;
    struct game_obj_t *o;

    if (game_state.nextbombtime > timer)
        return;
    game_state.nextbombtime = timer + (frame_rate_hz >> 2);

    /* Player drops cmd_multiplier presents.  */
    for (j=0;j<game_state.cmd_multiplier;j++) {
    
        i = find_free_obj();
        if (i < 0)
            return;

        o = &game_state.go[i];
        o->last_xi = -1;
        o->x = player_target->x+(5 * game_state.direction);
        o->y = player_target->y;
        o->vx = player->vx + BOMB_SPEED * 
            (player_target == player ? game_state.direction :
                player_target->tsd.reindeer.direction) + (j*3*game_state.direction);
        o->vy = player->vy;
        o->v = &present_vect;
        o->move = present_move;
        o->draw = draw_generic;
        o->destroy = generic_destroy_func;
        o->otype = OBJ_TYPE_PRESENT;
        add_target(o);
        o->color = GREEN;
        o->alive = 20;
    }
    if (randomn(100) < 30 && hohoho_time < timer) {
        wwviaudio_add_sound(HOHOHO);
        hohoho_time = timer + 60;
    } else if (randomn(100) < 30 && hohoho_time < timer) {
        wwviaudio_add_sound(HOHOHO_MERRY_XMAS);
        hohoho_time = timer + 60;
    }
    game_state.cmd_multiplier = 1;
}

void drop_gravity_bomb()
{
    int i, j;
    struct game_obj_t *o;

    if (game_state.nextgbombtime > timer)
        return;
    game_state.nextgbombtime = timer + (frame_rate_hz >> 2);

    /* Player drops cmd_multiplier bombs.  */
    for (j=0;j<game_state.cmd_multiplier;j++) {
        if (game_state.ngbombs == 0)
            return;
        game_state.ngbombs--;
    
        i = find_free_obj();
        if (i < 0)
            return;

        o = &game_state.go[i];
        o->last_xi = -1;
        o->x = player->x+(5 * game_state.direction);
        o->y = player->y;
        o->vx = player->vx + BOMB_SPEED * game_state.direction + (j*3*game_state.direction);
        o->vy = player->vy;
        o->v = NULL;
        o->move = gravity_bomb_move;
        o->draw = draw_generic;
        o->destroy = generic_destroy_func;
        o->otype = OBJ_TYPE_BOMB;
        add_target(o);
        o->color = WHITE;
        o->alive = 20;
        wwviaudio_add_sound(GRAVITYBOMB_SOUND);
    }
    game_state.cmd_multiplier = 1;
}

void player_draw(struct game_obj_t *o)
{

    int i, countdir;
    double scale, scalefactor;
#ifndef OPENLASE
    if (player->tsd.epd.count == 0) /* normal case, just draw the player normally. */
#else
    if (1)
#endif
        draw_generic(o);
    else {
        /* this is the insanity that makes the player "zoom" into the game */
        /* at the start of levels. */
        if (player->tsd.epd.count > 0) {
            scale = 1.07;
            scalefactor = 1.07;
            countdir = 1;
        } else {
            scale = 1.07;
            scalefactor = 1.07;
            countdir = -1;
        }

        /* Each frame, we draw a bunch of images of the player, each scaled */
        /* to a different size.  With each frame, the number of images reduces by 1.  */
        /* o->tsd.epd.count2 is this number of images. */
        /* o->tsd.epd.count is the number of frame iterations it takes to reduce */
        /* things down to normal. It's a bit funky, but it works. */
        for (i = 0; i<o->tsd.epd.count2; i++) {
            int j;
            int x1, y1, x2, y2;
            gdk_gc_set_foreground(gc, &huex[o->color]);
            for (j=0;j<o->v->npoints-1;j++) {
                if (o->v->p[j+1].x == LINE_BREAK) /* Break in the line segments. */
                    j+=2;
                if (o->v->p[j].x == COLOR_CHANGE) {
                    gdk_gc_set_foreground(gc, &huex[o->v->p[j].y]);
                    j+=1;
                }
                x1 = o->x + o->v->p[j].x*scale - game_state.x;
                y1 = o->y + o->v->p[j].y*scale - game_state.y + (SCREEN_HEIGHT/2);  
                x2 = o->x + o->v->p[j+1].x*scale - game_state.x; 
                y2 = o->y + o->v->p[j+1].y*scale+(SCREEN_HEIGHT/2) - game_state.y;
                wwvi_draw_line(x1, y1, x2, y2); 
            }
            scale = scale * scalefactor;
        }
        o->tsd.epd.count2 += countdir;
        if (o->tsd.epd.count2 == 0 && countdir == -1)
            o->tsd.epd.count = 0;
        if (o->tsd.epd.count != 0 && o->tsd.epd.count2 == o->tsd.epd.count && countdir == 1)
            o->tsd.epd.count = 0;
    }
}

void xmas_player_draw(struct game_obj_t *o)
{

    int i, countdir, x1, y1, x2, y2;
    double scale, scalefactor;

    if (o->tsd.epd.count == 0) {  /* normal case, just draw the player normally. */
        draw_generic(o);
        if (o->bullseye != NULL) {
            gdk_gc_set_foreground(gc, &huex[RED]);
            x1 = o->x - game_state.x;
            y1 = o->y - game_state.y + (SCREEN_HEIGHT/2);  
            x2 = o->bullseye->x - game_state.x;
            y2 = o->bullseye->y - game_state.y + (SCREEN_HEIGHT/2);  
            if (x1 > 0 && x1 < SCREEN_WIDTH && y1 < SCREEN_HEIGHT && y1 > 0) {
                wwvi_draw_line(x1, y1, x2, y2); 
            }
        }
        if (o->tsd.reindeer.rudolph && (timer & 0x08)) { /* blink rudolph's nose. */
            x1 = o->x - game_state.x + (game_state.direction * 15);
            y1 = o->y - 4 -game_state.y +(SCREEN_HEIGHT/2);  
            y2 = y1 - 2;  
            x2 = x1 + game_state.direction;
            gdk_gc_set_foreground(gc, &huex[WHITE]);
            wwvi_draw_line(x1, y1, x2, y1); 
            wwvi_draw_line(x1, y2, x2, y2); 
            --y1;
            gdk_gc_set_foreground(gc, &huex[RED]);
            wwvi_draw_line(x1, y1, x2, y1); 
        }
    } else {
        /* this is the insanity that makes the player "zoom" into the game */
        /* at the start of levels. */
        if (o->tsd.epd.count > 0) {
            scale = 1.07;
            scalefactor = 1.07;
            countdir = 1;
        } else {
            scale = 1.07;
            scalefactor = 1.07;
            countdir = -1;
        }

        /* Each frame, we draw a bunch of images of the player, each scaled */
        /* to a different size.  With each frame, the number of images reduces by 1.  */
        /* o->tsd.epd.count2 is this number of images. */
        /* o->tsd.epd.count is the number of frame iterations it takes to reduce */
        /* things down to normal. It's a bit funky, but it works. */
        for (i = 0; i<o->tsd.epd.count2; i++) {
            int j;
            int x1, y1, x2, y2;
            gdk_gc_set_foreground(gc, &huex[o->color]);
            for (j=0;j<o->v->npoints-1;j++) {
                if (o->v->p[j+1].x == LINE_BREAK) /* Break in the line segments. */
                    j+=2;
                if (o->v->p[j].x == COLOR_CHANGE) {
                    gdk_gc_set_foreground(gc, &huex[o->v->p[j].y]);
                    j+=1;
                }
                x1 = o->x + o->v->p[j].x*scale - game_state.x;
                y1 = o->y + o->v->p[j].y*scale - game_state.y + (SCREEN_HEIGHT/2);  
                x2 = o->x + o->v->p[j+1].x*scale - game_state.x; 
                y2 = o->y + o->v->p[j+1].y*scale+(SCREEN_HEIGHT/2) - game_state.y;
                wwvi_draw_line(x1, y1, x2, y2); 
            }
            scale = scale * scalefactor;
        }
        o->tsd.epd.count2 += countdir;
        if (o->tsd.epd.count2 == 0 && countdir == -1)
            o->tsd.epd.count = 0;
        if (o->tsd.epd.count != 0 && o->tsd.epd.count2 == o->tsd.epd.count && countdir == 1)
            o->tsd.epd.count = 0;
    }
}

static void add_stone_sound()
{
    int i;

    i = randomn(8);
    wwviaudio_add_sound_low_priority(STONEBANG1 + i);
}

static void add_metal_sound()
{
    int i;

    i = randomn(16);
    wwviaudio_add_sound_low_priority(METALBANG1 + i);
}

#if 0
static void corrosive_atmosphere_sound()
{
    static int nexttime = 0;

    /* Only allow this to enter the sound queue once per every 3 seconds */
    if (timer > nexttime) {
        nexttime = timer + frame_rate_hz*10;
        wwviaudio_add_sound(CORROSIVE_SOUND);
    }
}
#endif

static void ground_smack_sound()
{
    static int nexttime = 0;

    /* Only allow this to enter the sound queue once per every 3 seconds */
    if (timer > nexttime) {
        nexttime = timer + frame_rate_hz*3;
        wwviaudio_add_sound(GROUND_SMACK_SOUND);
        wwviaudio_add_sound(OWMYSPINE_SOUND);
    }
}

/* For automatically moving the player in "attract mode." */
void autopilot()
{
    int i;

    autopilot_mode = 1;
    for (i=0;i<TERRAIN_LENGTH;i++) {
        /* adjust player's vy to make him move towards a desired altitude. */
        /* which is MIN_ALT above the section of ground 100 units ahead of */
        /* the player */
        if (terrain.x[i] - player->x > 100 && (terrain.x[i] - player->x) < 300) {
            if (terrain.y[i] - player->y > MAX_ALT) {
                player->vy += 1;
                if (player->vy > 9)
                    player->vy = 9;
            } else if (terrain.y[i] - player->y < MIN_ALT) {
                player->vy -= 1;
                if (player->vy < -9)
                player->vy = -9;
            } else if (player->vy > 0) player->vy--;
            else if (player->vy < 0) player->vy++;
            break;
        }
        /* keep moving */
        if (player->vx < PLAYER_SPEED)
            player->vx++; 
    }
    if (randomn(40) < 4)    /* fire laser randomly */
        player_fire_laser();
    if (randomn(100) < 2)   /* drop bombs randomly */
        drop_bomb();
}

void set_player_vect()
{
    if (!xmas_mode) {  
        player->v = (game_state.direction == 1) ? 
            &player_vect : 
            &left_player_vect;
        return;
    }

    if (game_state.direction == 1) {
        if (timer & 0x8)
            player->v = &player_vect;
        else
            player->v = &reindeer2_vect;
        return;
    }

    if (timer & 0x8)
        player->v = &left_player_vect;
    else
        player->v = &left_reindeer2_vect;
    return;
}

int new_high_score(int newscore);
void no_draw(struct game_obj_t *o);
void player_move(struct game_obj_t *o)
{
    int deepest, i;
    static int was_healthy = 1; /* notice this is static. */
    o->x += o->vx; /* move him... */
    o->y += o->vy;

    if (game_state.health > 0) {
        if (credits > 0 && o->vy > 0) /* damp y velocity. */
            o->vy--;
        if (credits > 0 && o->vy < 0)
            o->vy++;
        was_healthy = 1;
    } else if (was_healthy) { /* has player _just_ died, just this frame? */
        was_healthy = 0;  /* remember he's died, for next time. */

        /* force 10 secs to elapse before another quarter can go in. */
        next_quarter_time = timer + (frame_rate_hz * 10);

        /* drop humans */
        for (i=0;i<level.nhumanoids;i++) {
            if (human[i]->tsd.human.picked_up &&
                human[i]->tsd.human.abductor == player) {
                human[i]->tsd.human.picked_up = 0;
                human[i]->tsd.human.abductor = NULL;
                human[i]->vx = player->vx + randomn(6)-3;
                human[i]->vy = player->vy + randomn(6)-3;
            }
        }

        player->move = bridge_move; /* bridge move makes the player fall. */
        bright_explosion(player->x, player->y, player->vx, player->vy,
            90, 350, 30); /* bunch of sparks. */
        player->draw = no_draw;             /* Make player invisible. */
        spray_debris(o->x, o->y, o->vx, o->vy, 70, o, 1);/* Throw hunks of metal around, */
        spray_debris(o->x, o->y, o->vx, o->vy, 50, o, 1);
        spray_debris(o->x, o->y, o->vx, o->vy, 30, o, 1);
        spray_debris(o->x, o->y, o->vx, o->vy, 15, o, 1);
        wwviaudio_add_sound(LARGE_EXPLOSION_SOUND); /* and make a lot of noise */
        /* printf("decrementing lives %d.\n", game_state.lives); */
        game_state.lives--;             /* lost a life. */
        strcpy(textline[CREDITS].string, "");
        if (game_state.lives <= 0 || credits <= 0) { /* last life? */
            if (credits > 0) 
                credits--;          /* reduce credits */
            if (credits <= 0) {
                if (!autopilot_mode && 
                    new_high_score(game_state.score) != MAXHIGHSCORES)
                    timer_event = NEW_HIGH_SCORE_PRE_EVENT;
                else
                    timer_event = GAME_ENDED_EVENT; /* game is over. */
                next_timer = timer + 30;
            } else {                /* game not over... */
                timer_event = READY_EVENT;  /* back to beginning of level. */
                game_state.lives = 3;       /* 3 more lives, on next credit. */
                next_timer = timer + 60;    /* give them a little breather. */
            }
        } else {                    /* same game, no new credit, no new lives */
            next_timer = timer + frame_rate_hz * 4; /* 4 sec breather */
            timer_event = READY_EVENT;      /* back to beginning of level. */
        }
    } 
    deepest = find_ground_level(player, NULL);

    /* if the player flies too slow, gravity starts pulling him down. */
    if (game_state.health <= 0) {
        if (deepest != GROUND_OOPS) {
            if (o->y < deepest) 
                o->vy+=1;
            else if (o->vx < 0)
                o->vx += 1;
            else if (o->vx > 0)
                o->vx -= 1;
        }
        
        if (o->vy > MAX_VY)
            o->vy = MAX_VY;
        if (was_healthy && !xmas_mode)
            explode(o->x-(13 * game_state.direction), o->y, -(7*game_state.direction), 0, 7, 10, 9);
    } else
        if (was_healthy && !xmas_mode)
            /* make some exhaust */
            explode(o->x-(13 * game_state.direction), o->y, -((abs(o->vx)+7)*game_state.direction), -o->vy, 10, 10, 9);

    /* Detect smashing into the ground */
    if (deepest != GROUND_OOPS && player->y >= deepest) {
        /* keep player from sinking through the ground. */
        player->y = deepest;
        if (abs(player->vy) > 7) 
            player->vy = -0.65 * abs(player->vy);
        else
            player->vy = 0;
        if (player->vy < -15) {
            player->vy = -15;
        }

        /* if player smacks the ground too hard, sparks, noise, damage ensue. */
        if (abs(player->vx) > 5 || abs(player->vy) > 5) {
            ground_smack_sound();
            do_strong_rumble();
            explode(player->x, player->y, player->vx*1.5, 1, 20, 20, 15);
            game_state.health -= 4 - player->vy * 0.3 -abs(player->vx) * 0.1;
            player->vy = -5;
            player->vx = player->vx >> 1;
        }
    }

    /* Detect smashing into sides and roof, similar to above ground detecting code */
    if (player->y < KERNEL_Y_BOUNDARY) {
        player->y = KERNEL_Y_BOUNDARY + 10;
        if (abs(player->vy) > 7) 
            player->vy = 0.65 * abs(player->vy);
        else
            player->vy = 0;
        player->vx = 0.65 * player->vx;
        if (player->vy > 15)
            player->vy = 15;
        if (abs(player->vx) > 5 || abs(player->vy) > 5) {
            ground_smack_sound();
            do_strong_rumble();
            explode(player->x, player->y, player->vx*1.5, 1, 20, 20, 15);
            game_state.health -= 4 - player->vy * 0.3 -abs(player->vx) * 0.1;
        }
    }
    if (player->x < 0) {
        ground_smack_sound();
        do_strong_rumble();
        explode(player->x, player->y, player->vx*1.5, 1, 20, 20, 15);
        game_state.health -= 4 - player->vy * 0.3 -abs(player->vx) * 0.1;
        player->x = 20;
        player->vx = 5;
    } else if (player->x > terrain.x[TERRAIN_LENGTH - 1]) {
        ground_smack_sound();   
        do_strong_rumble();
        explode(player->x, player->y, player->vx*1.5, 1, 20, 20, 15);
        game_state.health -= 4 - player->vy * 0.3 -abs(player->vx) * 0.1;
        player->x = terrain.x[TERRAIN_LENGTH - 1] - 20;
        player->vx = -5;
    }

    /* This stuff is to make the viewport track the player */
    /* if he's facing right, then the viewport puts him at 1/3 */
    /* the way across the screen, if left, then 2/3rds across the */
    /* screen.  If he switches direction, the transition is not abrupt */
    /* instead the velocity of the viewport is adjusted relative to the */
    /* player's velocity so that the viewport and player kind of slide */
    /* relative to one another to get the viewport in the right place. */
    if (game_state.direction == 1) {
        if (player->x - game_state.x > SCREEN_WIDTH/3) {
            /* going off screen to the right... rein back in */
            /* game_state.x = player->x - 2*SCREEN_WIDTH/3; */
            game_state.vx = player->vx + 7;
        } else {
            game_state.x = player->x - SCREEN_WIDTH/3;
            game_state.vx = player->vx;
        }
    } else {
        if (player->x - game_state.x < 2*SCREEN_WIDTH/3) {
        /* going off screen to the left... rein back in */
            game_state.vx = player->vx - 7;
        } else {
            game_state.x = player->x - 2*SCREEN_WIDTH/3;
            game_state.vx = player->vx;
        }
    }

    /* This is the y-axis viewport tracking... */
    if (player->vy < 0) {
        /* moving up... */  
        if (player->y - game_state.y - SCREEN_HEIGHT/2  < -3*SCREEN_HEIGHT/8) {
            /* are we above 1/4th the way from the bottom of screen...? */
            /* scoot the viewport up to reveal more of what's above */
            game_state.vy = player->vy - 3;
        } else {
            /* leave the player 3/4s the way down the screen */
            game_state.y = player->y + 3*SCREEN_HEIGHT/8 - SCREEN_HEIGHT/2;
            game_state.vy = player->vy;
        }
    } else if (player->vy > 0) {
        /* moving down... */
        if (player->y - game_state.y - SCREEN_HEIGHT/2 > -5*SCREEN_HEIGHT/8) {
            /* are we below 1/4th the way from top of screen...? */
            /* scoot the viewport down to reveal more of what's below */
            game_state.vy = player->vy + 3;
        } else {
            /* leave the viewport 1/4th of the way down the screen */
            game_state.y = player->y + 5*SCREEN_HEIGHT/8 - SCREEN_HEIGHT/2;
            game_state.vy = player->vy;
        }
    } else {
        game_state.vy = 0;
        /* are we above 1/4th the way from top of screen...? */
        /* scoot the viewport to keep the viewport on screen */
        if (player->y - game_state.y - SCREEN_HEIGHT/2 < -5*SCREEN_HEIGHT/8)
            game_state.y = 5*SCREEN_HEIGHT/8 - SCREEN_HEIGHT/2 + player->y;
        else
            /* are we below 1/4th the way from the bottom of screen...? */
            /* scoot the viewport to keep the player on screen */
            if (player->y - game_state.y - SCREEN_HEIGHT/2  > -3*SCREEN_HEIGHT/8)
                game_state.y = 3*SCREEN_HEIGHT/8 - SCREEN_HEIGHT/2 + player->y;
    }

    /* Check to see if radar is fritzed, or unfritzed. */
    if (game_state.health > RADAR_FRITZ_HEALTH) 
    {
        if (game_state.radar_state == RADAR_FRITZED) 
        {
            game_state.radar_state = RADAR_BOOTUP;
        }
    } 
    else if (game_state.health <= RADAR_FRITZ_HEALTH) 
    {
        if (game_state.radar_state != RADAR_FRITZED) 
        {
            game_state.radar_state = RADAR_FRITZED;
            wwviaudio_add_sound(RADAR_FAIL);
        }
    }

    /* Autopilot, "attract mode", if credits <= 0 */
    if (credits <= 0) {
        autopilot();
        /* If two minutes pass with no user intaraction, shut the hell up. */
        if (timer > last_user_input_time + 30*120) {
            if (!user_inaction_audio_pause) {
                wwviaudio_silence_sound_effects();
                wwviaudio_silence_music();
                user_inaction_audio_pause = 1;
            }
        } else if (user_inaction_audio_pause) {
            last_user_input_time = timer;
            user_inaction_audio_pause = 0;
            if (game_state.sound_effects_on)
                wwviaudio_resume_sound_effects();
            wwviaudio_resume_music();
        }
        if (!destiny_facedown && timer > destiny_facedown_timer2 &&
            timer_event != NEW_HIGH_SCORE_EVENT &&
            timer_event != NEW_HIGH_SCORE_PRE_EVENT &&
            timer_event != NEW_HIGH_SCORE_PRE2_EVENT ) {

            wwviaudio_cancel_all_sounds();
            game_state.sound_effects_on = 0;
            wwviaudio_silence_sound_effects();
            wwviaudio_play_music(DESTINY_FACEDOWN);
            destiny_facedown = 1;
            destiny_facedown_timer1 = timer + (frame_rate_hz * 46);
            destiny_facedown_timer2 = timer + (frame_rate_hz * 3*30);
        }
        if (destiny_facedown && timer > destiny_facedown_timer1) {
            destiny_facedown = 0;
            if (!game_state.sound_effects_manually_turned_off) {
                game_state.sound_effects_on = 1;
                if (!user_inaction_audio_pause)
                    wwviaudio_resume_sound_effects();
            }
        }
    }
    set_player_vect();
}

void bounce(int *vx, int *vy, int slope, double bouncefactor)
{

    /* When flaming hunks of debris from an explosion hit the ground, they bounce. */

    /* thought about doing the whole "find the normal, calculate a reflected angle */
    /* thing, but then I figured, what's bouncing is flaming hunks of metal wreckage */
    /* a simpler, faster algorithm with a bit of randomness thrown in will work just */
    /* as well.  Might have a bit too much bounciness in this algorithm, may want to */
    /* damp it just a bit. */

    if (slope > 25) {
        *vx = (*vx * bouncefactor) + (randomn(7));  /* bounce a bit to the right */
        *vy = -(*vy * bouncefactor); 
    } else if (slope < -25) {
        *vx = (*vx * bouncefactor) - (randomn(7));  /* bounce a bit to the left */
        *vy = -(*vy * bouncefactor); 
    } else {
        *vy = -(*vy * bouncefactor); 
        *vx = (*vx * bouncefactor) + (randomn(10)-6); /* bounce a bit left or right */
    }
    
    return; 
}

void bridge_move(struct game_obj_t *o) /* move bridge pieces when hit by bomb */
{
    int deepest;
    int slope = 0;

    o->x += o->vx;
    o->y += o->vy;
    o->vy++;        /* gravity */
    if (o->alive >1)    /* don't age all the way down to one, stop at 2. */
        age_object(o);

    /* Detect smashing into the ground */
    deepest = find_ground_level(o, &slope);
    if (deepest == GROUND_OOPS)
        return;

    if (o->otype == OBJ_TYPE_DEBRIS) {
        /* debris doesn't last forever, and disappears when age == 2. */
        if  (o->alive == 2) {
            remove_target(o);
            kill_object(o); 
        }
        /* flying debris will throw sparks for 4 seconds or until it hits the ground. */
#ifdef OPENLASE
        if (o->alive > frame_rate_hz*4 || o->y < deepest-2 && randomn(100) < 10)
#else
        if (o->alive > frame_rate_hz*4 || o->y < deepest-2) 
#endif
            explode(o->x, o->y, 0, 0, 16, 3, 10);
    }       

    /* hit the ground? */
    if (o->y >= deepest) 
    {
        /* if it's falling pretty fast, then bounce, taking slope into account... */
        if (o->vy > 4) 
        {
            o->y = deepest-2;
            if (o->vy > 12 && o->otype == OBJ_TYPE_DEBRIS && abs(o->x - player->x) < 800) 
            {
                if (o->tsd.debris.debris_type == DEBRIS_TYPE_METAL) /* metal? */
                    add_metal_sound();
                else
                    add_stone_sound();
            }
            bounce(&o->vx, &o->vy, slope, 0.3);
        } 
        else 
        {
            /* hit the ground, no bounce, stop. */
            o->y = deepest;
            o->vx = 0;
            o->vy = 0;

            /* But slide downhill if it's steep... */
            if (slope > 25 && o->alive > 1) {
                o->vx = 3;
                o->vy += 1;
            } else if (slope < -25 && o->alive > 1) {
                o->vx = -3;
                o->vy += 1;
            }
        }
        if (o->alive == 1) 
        {
            /* FIXME, does this ever execute?  I think maybe it doesn't. */
            o->move = NULL;
            o->radar_image = 0;
        }
    }
}

void no_move(__attribute__((unused)) struct game_obj_t *o)
{
    return;
}

void laser_draw(struct game_obj_t *o)
{
    int x1, y1, x2;
    gdk_gc_set_foreground(gc, &huex[o->color]);
    x1 = o->x - game_state.x;
    y1 = o->y - game_state.y + (SCREEN_HEIGHT/2);  

    if (o->vx > 0)
        x2 = x1 - (15) * (21 - o->alive);
    else
        x2 = x1 + (15) * (21 - o->alive);


    wwvi_bright_line(x1, y1, x2, y1, o->color);

#if DEBUG_HITZONE
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    wwvi_draw_line(x1, y1 - LASER_Y_PROXIMITY, x1, y1 + LASER_Y_PROXIMITY);
    wwvi_draw_line(x2, y1 - LASER_Y_PROXIMITY, x2, y1 + LASER_Y_PROXIMITY);
#endif
}

static inline int laser_hit_target(struct game_obj_t *laser, struct game_obj_t *target)
{
    if (laser->vx > 0) {
        if (laser->x <= target->x || 
            laser->tsd.laser.tail_x >= target->x)
            return 0;
    } else {
        if (laser->x >= target->x || 
            laser->tsd.laser.tail_x <= target->x)
            return 0;
    }

    if (laser->y <= target->y + target->above_target_y || 
        laser->y >= target->y + target->below_target_y) 
        return 0;
    return 1;
}

void laser_move(struct game_obj_t *o)
{
    struct game_obj_t *t, *next;
    int removed;
    int addpilot = 0;
    int pilotx, piloty, pilotvx, pilotvy;

    pilotx = piloty = pilotvx = pilotvy = 0;
    if (!o->alive)
        return;
    o->x += o->vx;
    o->y += o->vy;
    if (o->vx > 0)
        o->tsd.laser.tail_x = o->x - (15) * (21 - o->alive);
    else
        o->tsd.laser.tail_x = o->x + (15) * (21 - o->alive);

    /* search the list of potential targets and see if we've hit anything. */
    for (t=target_head;t != NULL;) {
        if (!t->alive) {
            t = t->next;
            continue;
        }
        removed = 0;
        switch (t->otype) {
            case OBJ_TYPE_SHIP:
            case OBJ_TYPE_AIRSHIP:
                if (abs(o->x - t->x) < 3*60 &&  /* hit a blimp or clipper ship? */
                    o->y - t->y <= -30 &&
                    o->y - t->y > -50*3) {
                    if (t->tsd.airship.pressure < 40)
                        t->tsd.airship.pressure += 2;
                    explode(o->x, o->y, o->vx/2, 1, 70, 20, 20); /* make sparks */
                    kill_object(o);     /* get rid of laser beam object. */
                    o->destroy(o);
                    t->alive -= PLAYER_LASER_DAMAGE;    /* damage enemy */
                    if (t->alive <= 0) {            /* killed him? */
                        kill_tally[t->otype]++;
                        next = remove_target(t);
                        kill_object(t);
                        t->destroy(t);
                        bright_explosion(t->x, t->y, t->vx, 1, 70, 50, 20);
                        bright_explosion(t->x-50, t->y, t->vx, 1, 70, 25, 20);
                        bright_explosion(t->x+50, t->y, t->vx, 1, 70, 25, 20);
                        bright_explosion(t->x-100, t->y, t->vx, 1, 70, 25, 20);
                        bright_explosion(t->x+100, t->y, t->vx, 1, 70, 25, 20);
                        spray_debris(t->x, t->y, t->vx, t->vy, 30, t, 1);
                        wwviaudio_add_sound(LASER_EXPLOSION_SOUND);
                        t = next;
                        removed = 1;
                    }
                }
                break;
            case OBJ_TYPE_JET:
            case OBJ_TYPE_BIG_ROCKET:
            case OBJ_TYPE_ROCKET:
            case OBJ_TYPE_HARPOON:
            case OBJ_TYPE_GDB:
            case OBJ_TYPE_CRON:
            case OBJ_TYPE_OCTOPUS:
            case OBJ_TYPE_FUEL:
            case OBJ_TYPE_JAMMER:
            case OBJ_TYPE_GUN:
            case OBJ_TYPE_KGUN:
            case OBJ_TYPE_BOMB:
            case OBJ_TYPE_SAM_STATION:
            case OBJ_TYPE_TRUSS:
            case OBJ_TYPE_JETPILOT:
            case OBJ_TYPE_MISSILE:{

                if (!laser_hit_target(o,t))
                    break;

                if (t->uses_health) {
                    t->health.health--;
                    if (t->health.health > 0) {
                        /* throw a few sparks... */
                        explode(t->x, t->y, t->vx, t->vy, 26, 10, 10);
                        break;
                    }
                }

                disconnect_any_attached_gun(t);

                if (t->otype == OBJ_TYPE_TRUSS)
                    truss_cut_loose_whats_below(t, NULL);
                if (t->otype == OBJ_TYPE_JET || t->otype == OBJ_TYPE_GDB) {
                    addpilot = 1;
                    pilotx = t->x;
                    piloty = t->y;
                    pilotvx = t->vx;
                    pilotvy = t->vy - 20;
                }
                if (t->otype == OBJ_TYPE_JET &&
                    (randomn(100) < 50)) {
                    /* half the time, jets will drop out */
                    /* of the sky like a bomb when hit, instead of */
                    /* exploding. */
                    wwviaudio_add_sound(LASER_EXPLOSION_SOUND);
                    explosion(t->x, t->y, t->vx, 1, 70, 150, 20);
                    t->move = bomb_move;
                    t->tsd.bomb.bank_shot_factor = 1;
                    break;
                }
                if (score_table[t->otype] != 0) {
                    game_state.score += (score_table[t->otype]);
                    add_score_floater(t->x, t->y, score_table[t->otype]);
                }
                kill_tally[t->otype]++;
                wwviaudio_add_sound(LASER_EXPLOSION_SOUND);
                explosion(t->x, t->y, t->vx, 1, 70, 150, 20);
                spray_debris(t->x, t->y, t->vx, t->vy, 70, t, 1);
                next = remove_target(t);
                kill_and_debris_object(t);
                /* if (t->otype == OBJ_TYPE_FUEL) {
                    game_state.health += 10;
                    if (game_state.health > game_state.max_player_health)
                        game_state.health = game_state.max_player_health;
                } */
                t->destroy(t);
                t = next;
                removed = 1;
                kill_object(o);
                o->destroy(o);
            }
            break;
            case OBJ_TYPE_WORM:
                if (!laser_hit_target(o, t))
                    break;
                explode(t->x, t->y, t->vx, 1, 70, 10, 20);
                /* cut the worm in half where it's hit. */
                /* Disconnect the top half: */
                if (t->tsd.worm.parent) {
                    struct game_obj_t *parent;
                    parent = t->tsd.worm.parent;

                    parent->tsd.worm.child = NULL;
                    if (parent->tsd.worm.parent == NULL) { /* just a head? */
                        remove_target(parent);
                        kill_object(parent);
                        parent->destroy(parent);
                        explosion(t->x, t->y, t->vx, 1, 70, 25, 20);
                        spray_debris(t->x, t->y, t->vx, t->vy, 70, t, 1);
                    }
                    t->tsd.worm.parent->tsd.worm.child = NULL;
                }
            
                /* Disconnect the bottom half */
                if (t->tsd.worm.child) {
                    /* make new head become autonomous ... see worm_move() */
                    struct game_obj_t *child;

                    child = t->tsd.worm.child;

                    child->tsd.worm.parent = NULL;
                    child->tsd.worm.tx = child->x;
                    child->tsd.worm.ty = child->y;
                    child->tsd.worm.ltx = child->x;
                    child->tsd.worm.lty = child->y;

                    if (child->tsd.worm.child == NULL) {    
                        remove_target(child);
                        kill_object(child);
                        child->destroy(child);
                        explosion(t->x, t->y, t->vx, 1, 70, 25, 20);
                        spray_debris(t->x, t->y, t->vx, t->vy, 70, t, 1);
                    }
                }

                /* blow away the segment we just hit. */
                wwviaudio_add_sound(LASER_EXPLOSION_SOUND);

                next = remove_target(t);
                kill_object(t);
                t->destroy(t);
                t = next;
                removed = 1;
                kill_object(o);
                o->destroy(o);
                break;
            default:
                break;
        }
        /* careful, if we removed something, next == current */
        if (!removed)
            t=t->next;
    }
    if (o->alive)
        age_object(o);
    if (addpilot)
        add_pilot(pilotx, piloty, pilotvx, pilotvy);
}

void move_obj(struct game_obj_t *o)
{
    o->x += o->vx; /* generic move function */
    o->y += o->vy;
}

void bright_spark_draw(struct game_obj_t *o)
{
    int x1, y1, x2, y2;

    /* draw sparks using their velocities as offsets. */
    x2 = o->x - game_state.x; 
    if (x2 < 0 || x2 > SCREEN_WIDTH)
        return;

    x1 = x2 + o->vx;
    if (x1 < 0 || x1 > SCREEN_WIDTH)
        return;

    y2 = o->y + (SCREEN_HEIGHT/2) - game_state.y;
    y1 = y2 + o->vy;
    wwvi_bright_line(x1, y1, x2, y2, o->color);
}

void spark_draw(struct game_obj_t *o)
{
    int x1, y1, x2, y2;

    /* draw sparks using their velocities as offsets. */
    x2 = o->x - game_state.x; 
    if (x2 < 0 || x2 > SCREEN_WIDTH)
        return;

    x1 = x2 + o->vx;
    if (x1 < 0 || x1 > SCREEN_WIDTH)
        return;

    y2 = o->y + (SCREEN_HEIGHT/2) - game_state.y;
    y1 = y2 + o->vy;
    gdk_gc_set_foreground(gc, &huex[o->color]);
    wwvi_draw_line(x1, y1, x2, y2); 
}

void pixie_dust_draw(struct game_obj_t *o)
{
    int x1, y1, x2, y2;

    x2 = o->x - game_state.x; 
    if (x2 < 0 || x2 > SCREEN_WIDTH)
        return;
    x1 = x2 + randomn(4);
    if (x1 < 0 || x1 > SCREEN_WIDTH)
        return;
    y2 = o->y + (SCREEN_HEIGHT/2) - game_state.y;
    y1 = y2 + randomn(4);
    gdk_gc_set_foreground(gc, &huex[o->color]);
    wwvi_draw_line(x1, y1, x2, y2);
}

void laserbolt_draw(struct game_obj_t *o)
{
    int x1, y1, x2, y2;

    /* very much like sparks... */
    x2 = o->x - game_state.x; 
    if (x2 < 0 || x2 > SCREEN_WIDTH)
        return;

    x1 = x2 - o->vx;
    if (x1 < 0 || x1 > SCREEN_WIDTH)
        return;

    y2 = o->y + (SCREEN_HEIGHT/2) - game_state.y;
    y1 = y2 - o->vy;

    wwvi_bright_line(x1, y1, x2, y2, o->color);
}

void missile_draw(struct game_obj_t *o)
{
    int x1, y1;
    int dx, dy;
    int rdx, rdy, rdx2, rdy2;

    /* similar to sparks, but with a constant length.  Offsets are precomputed */
    x1 = o->x - game_state.x;
    y1 = o->y - game_state.y + (SCREEN_HEIGHT/2);  
    dx = dx_from_vxy(o->vx, o->vy);
    dy = -dy_from_vxy(o->vx, o->vy);
    rdx = -dy / 16;
    rdy = dx / 16;
    rdx2 = -dy / 4;
    rdy2 = dx / 4;
    gdk_gc_set_foreground(gc, &huex[o->color]);
    if (x1 > 0 && x1+dx*2 > 0 && x1 < SCREEN_WIDTH &&  x1+dx*2 < SCREEN_WIDTH) {
        wwvi_draw_line(x1+rdx, y1+rdy, x1+dx*2+rdx, y1+dy*2+rdy); 
        wwvi_draw_line(x1-rdx, y1-rdy, x1+dx*2-rdx, y1+dy*2-rdy); 
        wwvi_draw_line(x1+dx*2+rdx, y1+dy*2+rdy, x1+dx*2-rdx, y1+dy*2-rdy); 
        wwvi_draw_line(x1-rdx2, y1-rdy2, x1+rdx2, y1+rdy2); 
        wwvi_draw_line(x1-rdx2, y1-rdy2, x1+(dx/2), y1+(dy/2)); 
        wwvi_draw_line(x1+rdx2, y1+rdy2, x1+(dx/2), y1+(dy/2)); 
    }
}

void harpoon_draw(struct game_obj_t *o)
{
    int x1, y1;
    int dx, dy;

    /* exactly like missiles, used to have a line back to the gdb which */
    /* fired the missile... but I didn't really like it, so commented it out. */
    x1 = o->x - game_state.x;
    y1 = o->y - game_state.y + (SCREEN_HEIGHT/2);  
    dx = dx_from_vxy(o->vx, o->vy);
    dy = -dy_from_vxy(o->vx, o->vy);
    gdk_gc_set_foreground(gc, &huex[o->color]);
    wwvi_draw_line(x1, y1, x1+dx, y1+dy); 
}

void reindeer_move(struct game_obj_t *o)
{
    int dx, dy, direction;
    int dvx, dvy, vx, vy;
    struct my_vect_obj *left = NULL;
    struct my_vect_obj *right = NULL;


    /* Find out which direction the reindeer ahead of us is facing */
    if (o->bullseye == player) {
        direction = game_state.direction;
        if (timer & 0x8) {
            right = &player_vect;
            left = &left_player_vect;
        } else {
            right = &reindeer2_vect;
            left = &left_reindeer2_vect;
        }
    } else {
        direction = o->bullseye->tsd.reindeer.direction;
        if (o->tsd.reindeer.sleigh) {
            right = &sleigh_vect;
            left = &left_sleigh_vect;
            explode_pixie_dust(o->x-(13 * direction), o->y, -(7*direction), 0, 7, 3, 30 + randomn(15));
        } else {
            if (timer & 0x8) {
                right = &player_vect;
                left = &left_player_vect;
            } else {
                right = &reindeer2_vect;
                left = &left_reindeer2_vect;
            }
            /* right = &player_vect; */
            /* left = &left_player_vect; */
        }
    }

    /* Calculate desired position */
    dx = o->bullseye->x -direction * 20;
    dy = o->bullseye->y;

    dvx = dx - o->x;
    dvy = dy - o->y;

    if (abs(dvx) < 4)
        vx = dvx;
    else
        vx = (dvx >> 1);
    if (abs(dvy) < 4)
        vy = dvy;
    else
        vy = (dvy >> 1);

    if (vx < -MAX_VX*2)
        vx = -MAX_VX*2;
    else if (vx > MAX_VX*2)
        vx = MAX_VX*2;

    if (vy < -MAX_VY*2)
        vy = -MAX_VY*2;
    else if (vy > MAX_VY*2)
        vy = MAX_VY*2;

    if (direction > 0) {
        if (o->x < o->bullseye->x - 10) {
            o->v = right;
            o->tsd.reindeer.direction = 1;
        } else {
            o->v = left;
            o->tsd.reindeer.direction = -1;
        }
    } else {
        if (o->x > o->bullseye->x + 10) {
            o->v = left;
            o->tsd.reindeer.direction = -1;
        } else {
            o->v = right;
            o->tsd.reindeer.direction = 1;
        }
    }

    o->x += vx;
    o->y += vy;
}

void missile_move(struct game_obj_t *o)
{
    struct game_obj_t *target_obj;
    int dx, dy, desired_vx, desired_vy;
    int exvx,exvy,deepest;

    /* move one step... */
    o->x += o->vx;
    o->y += o->vy;

    /* detect smashing into the ground. */  
    deepest = find_ground_level(o, NULL);
    if (deepest != GROUND_OOPS && o->y > deepest) {
        wwviaudio_add_sound(ROCKET_EXPLOSION_SOUND);
        explosion(o->x, o->y, o->vx, o->vy, 70, 150, 20);
        remove_target(o);
        kill_object(o);
        o->destroy(o);
        return;
    }

    /* missiles don't last forever... when they run out of fuel, they explode. */
    age_object(o);
    if (o->alive <= 0) { 
        wwviaudio_add_sound(ROCKET_EXPLOSION_SOUND);
        explosion(o->x, o->y, o->vx, o->vy, 70, 150, 20);
        remove_target(o);
        kill_object(o);
        o->destroy(o);
        return;
    }

    /* Figure out where we're trying to go, dx,dy */
    target_obj = o->bullseye;
    if (target_obj == player_target) {
        /* this is so we know to sound the alarm. */
        game_state.missile_locked = 1; 
        /* printf("mlock1\n"); */
    }
    dx = target_obj->x + target_obj->vx - o->x;
    dy = target_obj->y + target_obj->vy - o->y;

    /* have we hit the target? */
    if ((abs(dx) < MISSILE_PROXIMITY) && (abs(dy) < MISSILE_PROXIMITY)) {
        /* We've hit the target */
        if (player_target == o->bullseye) {
            game_state.health -= MISSILE_DAMAGE;
            do_strong_rumble();
        } else
            target_obj->alive -= MISSILE_DAMAGE;
        remove_target(o);
        kill_object(o);
        /* target_obj->alive -= MISSILE_DAMAGE; */
        if (target_obj->alive <= 0)
            kill_object(target_obj);
        wwviaudio_add_sound(ROCKET_EXPLOSION_SOUND);
        explosion(o->x, o->y, o->vx, o->vy, 70, 150, 20);
        o->destroy(o);
        return;
    }

    /* by similar triangles, find desired vx/vy from dx/dy... */
    if (abs(dx) < abs(dy)) {
        desired_vy = ((dy < 0) ? -1 : 1 ) * MAX_MISSILE_VELOCITY;
        if (dy != 0)
            desired_vx = desired_vy * dx / dy;
        else /* shouldn't happen */
            desired_vx = ((dx < 0) ? -1 : 1 ) * MAX_MISSILE_VELOCITY;
    } else {
        desired_vx = ((dx < 0) ? -1 : 1 ) * MAX_MISSILE_VELOCITY;
        if (dx != 0)
            desired_vy = desired_vx * dy / dx;
        else /* shouldn't happen */
            desired_vy = ((dy < 0) ? -1 : 1 ) * MAX_MISSILE_VELOCITY;
    }

    /* Adjust velocity towards desired vx,vy, but, only once every other clock tick */
    if ((timer & 0x3) != 0) {
        if (o->vx < desired_vx)
            o->vx++;
        else if (o->vx > desired_vx)
            o->vx--;
        if (o->vy < desired_vy)
            o->vy++;
        else if (o->vy > desired_vy)
            o->vy--;

        /* keep missile from going too slow. */
        if ((abs(o->vx) + abs(o->vy)) < (MAX_MISSILE_VELOCITY - 10)) {
            if (abs(dx) < abs(dy)) {
                if (o->vx < 0)
                    o->vx -= 2;
                else
                   o->vx +=2;
            } else {
                if (o->vy < 0)
                    o->vy -= 2;
                else
                    o->vy +=2;
            }
        }
    }

    /* make some exhaust sparks. */
    exvx = -dx_from_vxy(o->vx, o->vy) + randomn(2)-1;
    exvy = dy_from_vxy(o->vx, o->vy) + randomn(2)-1;
    explode(o->x, o->y, exvx, exvy, 4, 8, 9);
}

void harpoon_move(struct game_obj_t *o)
{
    struct game_obj_t *target_obj;
    int dx, dy, desired_vx, desired_vy;
    int exvx,exvy,deepest;

    /* this is exactly like the missile code above. */
    /* Was trying to make the gdb's have a different weapon */
    /* but for now, it's just missiles. */

    /* move one step... */
    o->x += o->vx;
    o->y += o->vy;
    
    /* detect smashing into the ground. */  
    deepest = find_ground_level(o, NULL);
    if (deepest != GROUND_OOPS && o->y > deepest) {
        wwviaudio_add_sound(ROCKET_EXPLOSION_SOUND);
        explosion(o->x, o->y, o->vx, o->vy, 70, 150, 20);
        remove_target(o);
        kill_object(o);
        o->destroy(o);
        return;
    }

    /* missiles don't last forever... when they run out of fuel, they explode. */
    age_object(o);
    if (o->alive <= 0) { 
        wwviaudio_add_sound(ROCKET_EXPLOSION_SOUND);
        explosion(o->x, o->y, o->vx, o->vy, 70, 150, 20);
        remove_target(o);
        kill_object(o);
        o->destroy(o);
        return;
    }

    /* Figure out where we're trying to go, dx,dy */
    target_obj = o->bullseye;
    if (target_obj == player_target) {
        /* this is so we know to sound the alarm. */
        game_state.missile_locked = 1;
        /* printf("mlock2\n"); */
    }
    dx = target_obj->x + target_obj->vx - o->x;
    dy = target_obj->y + target_obj->vy - o->y;

    /* have we hit the target? */
    if ((abs(dx) < MISSILE_PROXIMITY) && (abs(dy) < MISSILE_PROXIMITY)) {
        /* We've hit the target */
        if (player_target == o->bullseye) {
            do_strong_rumble();
            game_state.health -= MISSILE_DAMAGE;
        } else
            target_obj->alive -= MISSILE_DAMAGE;
        remove_target(o);
        kill_object(o);
        if (target_obj->alive <= 0)
            kill_object(target_obj);
        wwviaudio_add_sound(ROCKET_EXPLOSION_SOUND);
        explosion(o->x, o->y, o->vx, o->vy, 70, 150, 20);
        o->destroy(o);
        return;
    }

    /* by similar triangles, find desired vx/vy from dx/dy... */
    if (abs(dx) < abs(dy)) {
        desired_vy = ((dy < 0) ? -1 : 1 ) * MAX_MISSILE_VELOCITY;
        if (dy != 0)
            desired_vx = desired_vy * dx / dy;
        else /* shouldn't happen */
            desired_vx = ((dx < 0) ? -1 : 1 ) * MAX_MISSILE_VELOCITY;
    } else {
        desired_vx = ((dx < 0) ? -1 : 1 ) * MAX_MISSILE_VELOCITY;
        if (dx != 0)
            desired_vy = desired_vx * dy / dx;
        else /* shouldn't happen */
            desired_vy = ((dy < 0) ? -1 : 1 ) * MAX_MISSILE_VELOCITY;
    }

    /* Adjust velocity towards desired vx,vy, */
    /* unlike missiles -- every clock tick. */
    /* This might be a little too ferocious... */
    /* if ((timer % 2) == 0) { */
        if (o->vx < desired_vx)
            o->vx++;
        else if (o->vx > desired_vx)
            o->vx--;
        if (o->vy < desired_vy)
            o->vy++;
        else if (o->vy > desired_vy)
            o->vy--;
    /* } */

    /* make some exhaust sparks. */
    exvx = -dx_from_vxy(o->vx, o->vy) + randomn(2)-1;
    exvy = dy_from_vxy(o->vx, o->vy) + randomn(2)-1;
    explode(o->x, o->y, exvx, exvy, 4, 8, 9);
}

void bullet_move(struct game_obj_t *o)
{
    struct game_obj_t *target_obj;
    int dx, dy;
    int deepest;

    /* move one step... */
    o->x += o->vx;
    o->y += o->vy;

    /* bullets don't last forever... */
    age_object(o);  
    deepest = find_ground_level(o, NULL);
    if (o->alive <= 0 || (deepest != GROUND_OOPS && o->y > deepest)) {
        remove_target(o);
        kill_object(o);
        o->destroy(o);
        return;
    }

    /* Figure out where we were trying to go, dx,dy */
    target_obj = o->bullseye;
    dx = target_obj->x + target_obj->vx - o->x;
    dy = target_obj->y + target_obj->vy - o->y;

    /* throw some sparks. */
    explode(o->x, o->y, 0, 0, 10, 10, 10);

    /* did we hit? */
    if ((abs(dx) < BULLET_PROXIMITY) && (abs(dy) < BULLET_PROXIMITY)) {
        /* We've hit the target */
        if (player_target == o->bullseye) {
            game_state.health -= BULLET_DAMAGE;
            do_strong_rumble();
        } else
            target_obj->alive -= BULLET_DAMAGE;
        remove_target(o);
        kill_object(o);
        if (target_obj->alive <= 0)
            kill_object(target_obj);
        wwviaudio_add_sound(CLANG_SOUND);
        explosion(o->x, o->y, o->vx, o->vy, 70, 10, 10);
        o->destroy(o);
        return;
    }
}

static struct game_obj_t *add_generic_object(int x, int y, int vx, int vy,
    obj_move_func *move_func,
    obj_draw_func *draw_func,
    int color, 
    struct my_vect_obj *vect, 
    int target,  /* can this object be a target? hit by laser, etc? */
    char otype, 
    int alive)
{
    int j;
    struct game_obj_t *o;

    j = find_free_obj();
    if (j < 0)
        return NULL;
    o = &game_state.go[j];
    o->last_xi = -1; /* don't know, so use -1 at first, this gets fixed up later */
    o->x = x;
    o->y = y;
    o->vx = vx;
    o->vy = vy;
    o->move = move_func;
    o->draw = draw_func;
    o->destroy = generic_destroy_func;
    o->color = color;
    if (target)
        add_target(o);
    else {
        o->prev = NULL;
        o->next = NULL;
    }
    o->v = vect;
    o->otype = otype;
    o->alive = alive;
    o->bullseye = NULL;
    o->missile_timer = 0;
    o->counter = 0;
    o->radar_image = 0;
    o->above_target_y = - LASER_Y_PROXIMITY;
    o->below_target_y = LASER_Y_PROXIMITY;
    o->attached_gun = NULL;
    return o;
}

static void add_bullet(int x, int y, int vx, int vy, 
    int time, int color, struct game_obj_t *bullseye)
{
    int i;
    struct game_obj_t *o;

    i = find_free_obj();
    if (i<0)
        return;
    o = &game_state.go[i];
    o->x = x;
    o->y = y;
    o->vx = vx;
    o->vy = vy;
    o->move = bullet_move;
    o->draw = draw_generic;
    o->v = &bullet_vect;
    o->destroy = generic_destroy_func;
    o->bullseye = bullseye;
    add_target(o);
    o->otype = OBJ_TYPE_BULLET;
    o->color = color;
    o->alive = time;
    o->counter = 0;
}

static void add_missile(int x, int y, int vx, int vy, 
    int time, int color, struct game_obj_t *bullseye)
{
    int i;
    struct game_obj_t *o;

    i = find_free_obj();
    if (i<0)
        return;
    o = &game_state.go[i];
    o->last_xi = -1;
    o->x = x;
    o->y = y;
    o->vx = vx;
    o->vy = vy;
    o->move = missile_move;
    o->draw = missile_draw;
    o->destroy = generic_destroy_func;
    o->bullseye = bullseye;
    add_target(o);
    o->otype = OBJ_TYPE_MISSILE;
    o->color = color;
    o->alive = time;
    o->counter = 0;
}

static void add_harpoon(int x, int y, int vx, int vy, 
    int time, int color, struct game_obj_t *bullseye,
    struct game_obj_t *gdb)
{
    int i;
    struct game_obj_t *o;

    i = find_free_obj();
    if (i<0)
        return;
    o = &game_state.go[i];
    o->last_xi = -1;
    o->x = x;
    o->y = y;
    o->vx = vx;
    o->vy = vy;
    o->move = harpoon_move;
    o->draw = harpoon_draw;
    o->destroy = generic_destroy_func;
    o->bullseye = bullseye;
    add_target(o);
    o->otype = OBJ_TYPE_HARPOON;
    o->color = color;
    o->alive = time;
    o->counter = 0;
    o->tsd.harpoon.gdb = gdb;
}
void balloon_move(struct game_obj_t *o)
{
    int deepest;

    o->x += o->vx;
    o->y += o->vy;

    if (randomn(1000) < 5) {
        o->vx = randomab(1, 3) - 2;
        o->vy = randomab(1, 3) - 2;
    }

    deepest = find_ground_level(o, NULL);
    if (deepest != GROUND_OOPS && o->y > deepest - MIN_BALLOON_HEIGHT)
        o->vy = -1;
    else if (deepest != GROUND_OOPS && o->y < deepest - MAX_BALLOON_HEIGHT)
        o->vy = 1;
}

void floating_message_move(struct game_obj_t *o)
{

    /* this is a trivial move, if anything else needs this, factor it out */
    if (!o->alive)
        return;
    age_object(o);
    if (o->alive <= 0)
        o->destroy(o);
    o->x += o->vx;
    o->y += o->vy;
}

void symbol_move(struct game_obj_t *o) /* move bridge pieces when hit by bomb */
{
    int vx,vy;
    if (!o->alive)
        return;
    o->x += o->vx;
    o->y += o->vy;
    if ((timer % 80) == 1) {
        o->vx += randomab(1,4)-2;
        o->vy += randomab(1,4)-2;
    }
    if (abs(o->x - player->x) < 20 && abs(o->y - player->y) < 20) {
        vx = o->vx;
        vy = o->vy;
        o->vx = player->vx/2;
        o->vy = player->vy/2;
        player->vx -= vx/3;
        player->vy -= vy/3;
    }
    if (o->alive>0) {
        age_object(o);
        /* if (!o->alive) 
            remove_target(o); */
    } else
        o->destroy(o);
}

static void aim_vx_vy(struct game_obj_t *target, 
        struct game_obj_t *shooter,
        int v, int leadtime, 
        int *vx, int *vy);

static void flying_thing_shoot_missile(struct game_obj_t *o)
{
    int gambling, xdist, ydist;

    xdist = abs(o->x - player_target->x);
    ydist = abs(o->y - player_target->y);
    if (xdist < SAM_LAUNCH_DIST && ydist < SAM_LAUNCH_DIST) {
        if (xdist < SAM_LAUNCH_DIST/3)
            gambling = 4;
        else if (xdist < SAM_LAUNCH_DIST/2)
            gambling = 2;
        else
            gambling = 0;
        
        if (randomn(2000) < (SAM_LAUNCH_CHANCE+gambling) && timer >= o->missile_timer) {
            wwviaudio_add_sound(SAM_LAUNCH_SOUND);
            add_missile(o->x, o->y, 0, 0, 300, RED, player_target);
            o->missile_timer = timer + MISSILE_FIRE_PERIOD;
        }

        if (randomn(2000) < (SAM_LAUNCH_CHANCE*2+gambling)) {
            int vx, vy;
            aim_vx_vy(player_target, o, 28, 10, &vx, &vy);
            add_laserbolt(o->x, o->y, vx, vy, RED, 50);
            wwviaudio_add_sound(FLAK_FIRE_SOUND);
        }
    }
}

static void add_symbol(int c, int myfont, int x, int y, int vx, int vy, int time);
void flying_thing_move(struct game_obj_t *o)
{
    int deepest;

    o->x += o->vx;
    o->y += o->vy;

    /* keep it in bounds */
    if (o->x < 150)
        o->vx = 1;
    if (o->x > terrain.x[TERRAIN_LENGTH-1]-150)
        o->vx = -1;

    if (randomn(1000) < 5) {
        o->vx = randomab(1, 3) - 2;
        o->vy = randomab(1, 3) - 2;
    }

    deepest = find_ground_level(o, NULL);
    if (deepest != GROUND_OOPS && o->y > deepest - MIN_BALLOON_HEIGHT)
        o->vy = -1;
    else if (deepest != GROUND_OOPS && o->y < deepest - MAX_BALLOON_HEIGHT)
        o->vy = 1;
}

void airship_leak_lisp(struct game_obj_t *o)
{
    /* The airships have "memory leaks" in which they spew "lisp code" */
    /* out into core memory... */
    if ((timer % 8) && randomn(100) < 5 + (o->tsd.airship.pressure)) {
        if (randomlisp[o->counter] != ' ') /* skip putting out "space" objects. */
            add_symbol(randomlisp[o->counter], TINY_FONT, o->x + 70*3, o->y - 30*3, 
                o->vx+4, 0, 200);
        o->counter--;
        if (o->counter < 0)
            o->counter = strlen(randomlisp)-1;
    }
}

static void abs_xy_draw_string(char *s, int font, int x, int y);

/* this is the text which scrolls up on the side of the blimp. */
char *blimp_message[] = {
"",
"",
"        vi vi vi...",
"",
"   The editor of the",
"",
"       B E A S T !",
"",
"",
"    MMMMMMMMMMMMMMMMMMMMM     ", /* Richard Stallman as ascii art. */
" MMMMMMMMMWX0dxxKNWMMMMMMMMM  ", /* Done from a copyright-free photo */
"MMMMMMMM0:   'xO0kdXMMMMMMMMM ", /* found on wikipedia run through gimp */
"MMMMMMX,   .,lONWMxokWMMMMMMMM", /* to adjust contrast, and then jp2a */
"MMMMM0.   .,:cx0NMWlllNMMMMMMM", /* to convert the jpeg to ascii. */
"MMMMN.     ..:,.;0WXcloKWMMMMM", /* It looks better with light text */
"MMMMO    ... d0clKWMx,dloXMMMM", /* dark background -- hightlight it */
"MMMMO   .....;docOMMW:,;::OMMM", /* with the mouse to see for yourself. */
"MMMX,    ..   .,:dKKd;.::;;xMM",
"MMMK.         .'':cc,'..,,,;lX",
"l:,         ..'':;,;,',.;:,,:;",
"            ..',,,'cc;...,::c:",
"            .......co.... ',:;",
"                 .''.. .  ...;",
"                  .  .     . .",
"",
"       Bow down before",
"        St. IGNUcius,",
"         heathen!!!",
"",
"",
"",
"  Using vi is not a sin,",
"",
"    It is a penance!",
"",
"",
"",
"",
"    G N U   E M A C S",
"",
"        Rules!!!!",
"",
"     vi drools!!!!",
"",
"",
"",
"",
"       XXX  X  X X   X",
"      X   X XX X X   X",
"      X     XX X X   X",
"      X XXX X XX X   X",
"      X   X X XX X   X",
"       XXX  X  X  XXX",
"",
"XXXX X   X  XXX   XXX   XXX  ",
"X    XX XX X   X X   X X   X ",
"XXX  X X X XXXXX X     XXX    ",
"X    X X X X   X X        X   ",
"X    X   X X   X X   X X   X  ",
"XXXX X   X X   X  XXX   XXX   ",
"",
"",
"wordwarvi is free software;",
"you can redistribute it and/or",
"modify it under the terms of ",
"the GNU General Public License",
"as published by the Free ",
"Software Foundation; either",
"version 2 of the License, or",
"(at your option) any later",
"version.",
"",
"wordwarvi is distributed in",
"the hope that it will be useful,",
"but WITHOUT ANY WARRANTY; ",
"without even the implied",
"warranty of MERCHANTABILITY",
"or FITNESS FOR A PARTICULAR",
"PURPOSE.  See the GNU General",
"Public License for more ",
"details.",
"",
"You should have received a ",
"copy of the GNU General ",
"Public License along with ",
"wordwarvi; if not, write to ",
"the Free Software Foundation,",
"Inc.,",
" 51 Franklin St, Fifth Floor,",
"Boston, MA 02110-1301  USA",
"",
NULL, /* mark the end. */
}; 
void airship_draw(struct game_obj_t *o)
{
    int x, y, i, line;

    if (o->tsd.airship.pressure == 0)
        draw_generic(o);
    else
        draw_scaled_generic(o, 1.0, 1.0 + 0.01 * o->tsd.airship.pressure);
#ifndef OPENLASE
    /* the -90 and -130 were arrived at empirically, as were the dimensions */
    /* of the text field.  The commented out code below with 0123456... is */
    /* part of the testing done to figure that out. */
    x = o->x - game_state.x - 90;
    y = o->y - game_state.y + (SCREEN_HEIGHT/2) - 130;

    /* 
    for (i=0;i<7;i++) {
        abs_xy_draw_string(w, "012345678901234567890123456789", NANO_FONT, x, y);
        y = y + 15;
    } */

    /* Write out 7 lines of text from the loop, starting at our current position. */
    line = o->tsd.airship.bannerline;
    for (i=0;i<7;i++) {
        if (blimp_message[line] == NULL)
            line = 0;
        abs_xy_draw_string(blimp_message[line], NANO_FONT, x, y);
        y = y + 15;
        line++;
    }

    /* advance to the next line 2x per second */
    if ((timer % 15) == 0) {
        o->tsd.airship.bannerline++;
        if (blimp_message[o->tsd.airship.bannerline] == NULL)
            o->tsd.airship.bannerline = 0;
    }
#endif
}

void worm_draw(struct game_obj_t *o)
{
    int x, y, x2,y2;
    int color;

    x = o->x - game_state.x;
    y = o->y - game_state.y + (SCREEN_HEIGHT/2);
    if (o->tsd.worm.parent == NULL) {
        /* int x3, y3; */
        gdk_gc_set_foreground(gc, &huex[o->color]);
        wwvi_draw_line(x-2, y, x, y-2); 
        wwvi_draw_line(x, y-2, x+2, y); 
        wwvi_draw_line(x+2, y, x, y+2); 
        wwvi_draw_line(x, y+2, x-2, y); 
/*
        x3 = o->tsd.worm.tx - game_state.x;
        y3 = o->tsd.worm.ty - game_state.y + (SCREEN_HEIGHT/2);
        wwvi_draw_line(x, y, x3, y3); 
        x3 = o->tsd.worm.ltx - game_state.x;
        y3 = o->tsd.worm.lty - game_state.y + (SCREEN_HEIGHT/2);
        wwvi_draw_line(x, y, x3, y3); 
*/
    } else {
        color = NCOLORS + NSPARKCOLORS + ((timer + o->tsd.worm.coloroffset) % NRAINBOWCOLORS);
        gdk_gc_set_foreground(gc, &huex[color]);
        if (o->tsd.worm.child == NULL) {
            wwvi_draw_line(x-2, y, x, y-2); 
            wwvi_draw_line(x, y-2, x+2, y); 
            wwvi_draw_line(x+2, y, x, y+2); 
            wwvi_draw_line(x, y+2, x-2, y); 
        }
    }

#if 0
    if (o->tsd.worm.parent == NULL) {
        x2 = o->tsd.worm.tx - game_state.x;
        y2 = o->tsd.worm.ty - game_state.y + (SCREEN_HEIGHT/2);
        wwvi_draw_line(x, y, x2, y2);
        x2 = o->tsd.worm.ltx - game_state.x;
        y2 = o->tsd.worm.lty - game_state.y + (SCREEN_HEIGHT/2);
        wwvi_draw_line(x, y, x2, y2);
    } else {
#endif
    if (o->tsd.worm.parent != NULL) {
        int delta1 = 7;
        int delta2 = 7;

        if (o->tsd.worm.parent->tsd.worm.parent == NULL)
            delta2 = 2;
        if (o->tsd.worm.child == NULL)
            delta1 = 2;

        x2 = o->tsd.worm.parent->x - game_state.x;
        y2 = o->tsd.worm.parent->y - game_state.y + (SCREEN_HEIGHT/2);
        wwvi_draw_line(x-delta1, y, x2-delta2, y2);
        wwvi_draw_line(x+delta1, y, x2+delta2, y2);
        wwvi_draw_line(x, y-delta1, x2, y2-delta2);
        wwvi_draw_line(x, y+delta1, x2, y2+delta2);
    }
}

void worm_move(struct game_obj_t *o)
{
    int xi, n, gy, pdx, pdy;
    int ldx, ldy, sdx, sdy, dvx, dvy;

    pdx = abs(o->x - player_target->x);
    pdy = abs(o->y - player_target->y);
    /* is this the head of a worm? Or just a body segment? */
    if (o->tsd.worm.parent == NULL) {
        /* It's a head...  think about where to go */

        /* long term destination reached?  Pick a new one. */
        ldx = o->tsd.worm.ltx - o->x;
        ldy = o->tsd.worm.lty - o->y;
        if (abs(ldx) < 20 && abs(ldy) < 20) {
                        o->tsd.worm.ltx = terrain.x[randomn(TERRAIN_LENGTH-MAXBUILDING_WIDTH-1)];
                        o->tsd.worm.lty = ground_level(o->tsd.worm.ltx, &xi, NULL) - 800 + randomn(700);

            /* printf("New long term dest %d,%d\n", o->tsd.worm.ltx, o->tsd.worm.lty); */

            /* trigger short term destination reached code, below. */
            o->tsd.worm.tx = o->x;
            o->tsd.worm.ty = o->y;
        }

        /* short term destination reached?  Pick a new one. */
        sdx = o->tsd.worm.tx - o->x;
        sdy = o->tsd.worm.ty - o->y;
        if (abs(sdx) < 5 && abs(sdy) < 5) {


            /* If player is close, follow him... */
            if (pdx < 170 && pdy < 170) {
                o->tsd.worm.tx = player_target->x + player_target->vx * 10 + randomn(30) - 15;
                o->tsd.worm.ty = player_target->y + player_target->vy * 10 + randomn(30) - 15;

                /* Touching player?  Zap him. */
                if (pdx < 8 && pdy < 8) {
                    wwviaudio_add_sound(ZZZT_SOUND);
                    do_weak_rumble();
                    explode(player_target->x, player_target->y, player_target->vx*1.5, 1, 20, 20, 15);
                    game_state.health -= WORM_DAMAGE;
                }

            } else {
                /* Aim vaguely towards the destination... */
                if (o->x < o->tsd.worm.ltx)
                    o->tsd.worm.tx = o->x + randomn(200) - 30; 
                else
                    o->tsd.worm.tx = o->x - randomn(200) + 30; 

                if (o->y < o->tsd.worm.lty)
                    o->tsd.worm.ty = o->y + randomn(60) - 10; 
                else
                    o->tsd.worm.ty = o->y - randomn(60) + 10; 
            }

            /* Avoid the ground. */
            gy = ground_level(o->tsd.worm.tx, &xi, NULL);   
            if (o->tsd.worm.ty >= gy)
                o->tsd.worm.ty = gy - 50 - randomn(200);
            if (o->tsd.worm.ty <= KERNEL_Y_BOUNDARY)
                o->tsd.worm.ty = KERNEL_Y_BOUNDARY + 50 + randomn(200);
            
        }

        /* Calculate desired velocity... */
        sdx = o->tsd.worm.tx - o->x;
        sdy = o->tsd.worm.ty - o->y;
        if (abs(sdx) > abs(sdy)) {
            if (sdx > 0) {
                dvx = MAX_WORM_VX;
                dvy = (sdy * MAX_WORM_VY) / sdx;
            } else if (sdx < 0) {
                dvx = -MAX_WORM_VX;
                dvy = (sdy * MAX_WORM_VY) / abs(sdx);
            } else {
                dvx = 0;
                dvy = 0;
            }
        } else {
            if (sdy > 0) {
                dvy = MAX_WORM_VY;
                dvx = (sdx * MAX_WORM_VX) / sdy;
            } else if (sdy < 0) {
                dvy = -MAX_WORM_VY;
                dvx = (sdx * MAX_WORM_VX) / abs(sdy);
            } else { /* sdy == 0 */
                dvx = 0;
                dvy = 0;
            }
        }

        /* Adjust velocity towards desired velocity dvx, dvy */
        if (o->vx < dvx)
            o->vx++;
        else if (o->vx > dvx)
            o->vx--;
        if (o->vy < dvy)
            o->vy++;
        else if (o->vy > dvy)
            o->vy--;

        /* Store our movements so tail segments can ape them. */
        n = o->tsd.worm.i;

        o->tsd.worm.x[n] = o->x;
        o->tsd.worm.y[n] = o->y;
        o->tsd.worm.i++;
        if (o->tsd.worm.i >= NWORM_TIME_UNITS)
            o->tsd.worm.i = 0;

        /* finally, move */
        o->x += o->vx;
        o->y += o->vy;

    } else {
        /* we're in the tail (body segment), just move as the guy in front of you did. */
        int n;

        /* Store our movements so tail segments can ape them. */
        n = o->tsd.worm.i;
        o->tsd.worm.x[n] = o->x;
        o->tsd.worm.y[n] = o->y;
        o->tsd.worm.i++;
        if (o->tsd.worm.i >= NWORM_TIME_UNITS)
            o->tsd.worm.i = 0;

        /* ape the parents motion... */
        n = o->tsd.worm.parent->tsd.worm.i+1;
        if (n >= NWORM_TIME_UNITS)
            n = 0;

        o->x = o->tsd.worm.parent->tsd.worm.x[n];
        o->y = o->tsd.worm.parent->tsd.worm.y[n];

        /* touching the player?  Zap him. */
        if (pdx < 8 && pdy < 8) {
            wwviaudio_add_sound(ZZZT_SOUND);
            do_weak_rumble();
            explode(player_target->x, player_target->y, player_target->vx*1.5, 1, 20, 20, 15);
            game_state.health -= WORM_DAMAGE;
        }
    }
}

void airship_move(struct game_obj_t *o)
{
    if (!o->alive)
        return;

    flying_thing_move(o);
    flying_thing_shoot_missile(o);
    airship_leak_lisp(o);
    if (o->tsd.airship.pressure > 0)
        o->tsd.airship.pressure--;
    
}

void gunwheel_move(struct game_obj_t *o)
{
    struct game_obj_t *holder;

    o->lastx = o->x;
    o->lasty = o->y;

    /* if gunwheel is attached to something, make it move with that something */
    holder = o->tsd.gunwheel.attached_to;
    if (holder != NULL)
        holder->gun_location(holder, o, &o->x, &o->y);
    return;
}

void gunwheel_gun_location(struct game_obj_t *parent, 
    struct game_obj_t *child, int *x, int *y)
{
    int angle, x1, y1;

    angle = (timer * parent->tsd.gunwheel.rotation_multiplier) % 360;

    x1 = cosine[angle] * parent->tsd.gunwheel.size;
    y1 = sine[angle] * parent->tsd.gunwheel.size;

    if (child == parent->tsd.gunwheel.left) {
        *x = parent->x + x1;
        *y = parent->y + y1;
    } else {
        *x = parent->x - x1;
        *y = parent->y - y1;
    }
}

void make_gunwheel_sound()
{
    static int time_to_make_sound = 0;
    if (timer > time_to_make_sound) {
        time_to_make_sound = timer + FRAME_RATE_HZ;
        wwviaudio_add_sound(GUNWHEEL);
    }
}

void gunwheel_draw(struct game_obj_t *o)
{
    int angle, x1, y1, x2, y2, rx, ry;

    angle = (timer * o->tsd.gunwheel.rotation_multiplier) % 360;

    x1 = cosine[angle] * o->tsd.gunwheel.size;
    y1 = sine[angle] * o->tsd.gunwheel.size;
    rx = y1 >> 4; 
    ry = -x1 >> 4;
    x2 = o->x - x1;
    y2 = o->y - y1;
    x1 += o->x;
    y1 += o->y;
    x1 -= game_state.x;
    x2 -= game_state.x;
    y1 = y1 - game_state.y + (SCREEN_HEIGHT/2);  
    y2 = y2 - game_state.y + (SCREEN_HEIGHT/2);  

    gdk_gc_set_foreground(gc, &huex[o->color]);

    wwvi_draw_line(x1-rx, y1-ry, x2-rx, y2-ry);
    wwvi_draw_line(x1+rx, y1+ry, x2+rx, y2+ry);
    wwvi_draw_line(x1-rx, y1-ry, x1+rx, y1+ry);
    wwvi_draw_line(x2-rx, y2-ry, x2+rx, y2+ry);

    make_gunwheel_sound();
}

void ship_move(struct game_obj_t *o)
{
    if (!o->alive)
        return;

    flying_thing_move(o);
    /* flying_thing_shoot_missile(o); */
}

/* this is called VERY often.  Don't do anything slow in here. */
void spark_move(struct game_obj_t *o)
{
    /* printf("x=%d,y=%d,vx=%d,vy=%d, alive=%d\n", o->x, o->y, o->vx, o->vy, o->alive); */
    o->x += o->vx;
    o->y += o->vy;

        o->alive--;
        if (o->alive <= 0)
                kill_object(o);

    /* printf("x=%d,y=%d,vx=%d,vy=%d, alive=%d\n", o->x, o->y, o->vx, o->vy, o->alive); */

    /* Fade the colors. */
    if (o->alive >= NSPARKCOLORS)
        o->color = NCOLORS + NSPARKCOLORS - 1; 
    else if (o->alive < 0)
        o->color = NCOLORS;
    else
        o->color = NCOLORS + o->alive;

    /* air resistance */
    if (o->vx > 0)
        o->vx--;
    else if (o->vx < 0)
        o->vx++;
    if (o->vy > 0)
        o->vy--;
    else if (o->vy < 0)
        o->vy++;

    /* no gravity on the sparks, I've tried it, I like it better without it. */
    /*o->vy += (timer & 0x01); */

    /* If the sparks reach speed of zero due to air resistance, kill them. */
    if (o->vx == 0 && o->vy == 0) {
        kill_object(o);
        o->draw = draw_generic;
    }
    /* If the sparks are too far away, kill them so we won't have to process them. */
    if (abs(o->y - player->y) > 2000 || o->x > 2000+WORLDWIDTH || o->x < -2000) {
        kill_object(o);
        o->draw = draw_generic;
    }
}

/* this is called VERY often.  Don't do anything slow in here. */
void pixie_dust_move(struct game_obj_t *o)
{
    /* printf("x=%d,y=%d,vx=%d,vy=%d, alive=%d\n", o->x, o->y, o->vx, o->vy, o->alive); */
    o->x += o->vx;
    o->y += o->vy;

    o->vx = randomn(10)-5;
    o->vy = randomn(10)-7;

        o->alive--;
        if (o->alive <= 0)
                kill_object(o);

    /* If the pixie dust is too far away, kill them so we won't have to process them. */
    if (abs(o->y - player->y) > 2000 || o->x > 2000+WORLDWIDTH || o->x < -2000) {
        kill_object(o);
        o->draw = draw_generic;
    }
}

/* Make a bunch of sparks going in random directions. 
 * x, and y are where, ivx,ivy are initial vx, vy, v is the
 * radial velocity, think of it as the "power" of the explosion
 * and nsparks is how many sparks to make.  time is how long
 * the sparks are set to live. 
 */
void vexplode(int x, int y, int ivx, int ivy, int v, int nsparks, int time,
    add_spark_function *sparky)
{
    int vx, vy, i;

    for (i=0;i<nsparks;i++) {
        /* vx = (int) ((-0.5 + random() / (0.0 + RAND_MAX)) * (v + 0.0) + (0.0 + ivx));*/
        /* vy = (int) ((-0.5 + random() / (0.0 + RAND_MAX)) * (v + 0.0) + (0.0 + ivy));*/
        vx = randomn(v) - (v>>1) + ivx;
        vy = randomn(v) - (v>>1) + ivy;
        sparky(x, y, vx, vy, time); 
        /* printf("%d,%d, v=%d,%d, time=%d\n", x,y, vx, vy, time); */
    }
}

/* Make a bunch of sparks going in random directions. 
 * x, and y are where, ivx,ivy are initial vx, vy, v is the
 * radial velocity, think of it as the "power" of the explosion
 * and nsparks is how many sparks to make.  time is how long
 * the sparks are set to live. 
 */
void explode(int x, int y, int ivx, int ivy, int v, int nsparks, int time)
{
    vexplode(x, y, ivx, ivy, v, nsparks, time, add_spark);
}

/* same as explode, but with bright sparks. */
void bright_explode(int x, int y, int ivx, int ivy, int v, int nsparks, int time)
{
    vexplode(x, y, ivx, ivy, v, nsparks, time, add_bright_spark);
}


/* Make a bunch of pixie dust going in random directions. 
 * x, and y are where, ivx,ivy are initial vx, vy, v is the
 * radial velocity, think of it as the "power" of the explosion
 * and nsparks is how many sparks to make.  time is how long
 * the sparks are set to live. 
 */
void explode_pixie_dust(int x, int y, int ivx, int ivy, int v, int nsparks, int time)
{
    int vx, vy, i;

    for (i=0;i<nsparks;i++) {
        vx = randomn(v) - (v>>1) + ivx;
        vy = randomn(v) - (v>>1) + ivy;
        add_pixie_dust(x, y, vx, vy, time); 
    }
}

/* round_explode is just like explode() above, except make the distribution 
 * of velocities result in a round, rather than square explosion. 
 */
void vround_explode(int x, int y, int ivx, int ivy, int v, int nsparks, int time,
    add_spark_function *sparky)
{
    int vx, vy, i;
    int angle, tmpv; 

    for (i=0;i<nsparks * explosion_factor;i++) {
        /* vx = (int) ((-0.5 + random() / (0.0 + RAND_MAX)) * (v + 0.0) + (0.0 + ivx));*/
        /* vy = (int) ((-0.5 + random() / (0.0 + RAND_MAX)) * (v + 0.0) + (0.0 + ivy));*/
        /* vx = randomn(v) - (v>>1) + ivx;*/
        /* vy = randomn(v) - (v>>1) + ivy;*/
        angle = (randomn(360)+1) % 360;
        /* multiply by square root of 2 so the "energy" of the explosion */
        /* will be the same as that of the rectangular version. */
        tmpv = (randomn(v >> 1) * 1.4142135623);    
        vx = cosine[angle] * tmpv + ivx;
        vy = sine[angle] * tmpv + ivy;
        sparky(x, y, vx, vy, time); 
        /* printf("%d,%d, v=%d,%d, time=%d\n", x,y, vx, vy, time); */
    }
}

/* sphere_explode is just like explode() above, except make the distribution
 * of velocities result in a round, symmetrical and spherical
 * rather than square explosion.
 */
void vsphere_explode(int x, int y, int ivx, int ivy, int v,
    int nsparks, int time,
    add_spark_function *sparky)
{
    int vx, vy;
    int angle;
    int shell;
    int velocity;
    int delta_angle;
    float point_dist;
    int angle_offset;

    angle_offset = randomn(360);

    if (nsparks < 30)
        nsparks = 40;
    point_dist = v * 2 * M_PI / (nsparks / 4);
    delta_angle = (int) ((point_dist / (2.0 * M_PI * v)) * 360.0);

    v = v / (1.0 + randomn(100) / 100.0);
    for (shell = 0; shell < 90; shell += 7) {
        for (angle = 0; angle < 360; angle += delta_angle) {
            velocity = (int) (cosine[shell] * (double) v);
            vx = cosine[(angle + angle_offset) % 360] * velocity + ivx;
            vy = sine[(angle + angle_offset) % 360] * velocity + ivy;
            sparky(x, y, vx, vy, time);
        }
        delta_angle = (int) ((point_dist / (2.0 * velocity * M_PI)) * 360.0);
    }
}

void sphere_explode(int x, int y, int ivx, int ivy, int v, int nsparks, int time)
{
    vsphere_explode(x, y, ivx, ivy, v, nsparks, time, add_spark);
}

void bright_sphere_explode(int x, int y, int ivx, int ivy, int v, int nsparks, int time)
{
    vsphere_explode(x, y, ivx, ivy, v, nsparks, time, add_bright_spark);
}

void round_explode(int x, int y, int ivx, int ivy, int v, int nsparks, int time)
{
    vround_explode(x, y, ivx, ivy, v, nsparks, time, add_spark);
}

void bright_round_explode(int x, int y, int ivx, int ivy, int v, int nsparks, int time)
{
    vround_explode(x, y, ivx, ivy, v, nsparks, time, add_bright_spark);
}

void init_object_numbers()
{
    int i;
    /* every object must known its position in the array, because */
    /* it needs to be able to free itself, and that involves knowing */
    /* which bit to clear -- which involves knowing the position in */
    /* the array. */
    for (i=0;i<MAXOBJS;i++)
        game_state.go[i].number = i;
}

void mirror_points(struct my_vect_obj *points, struct my_vect_obj *mirror)
{
    int i;
    mirror->npoints = points->npoints;
    for (i = 0; i < points->npoints; i++) {
        mirror->p[i].y = points->p[i].y;
        mirror->p[i].x = points->p[i].x;
        if (points->p[i].x != COLOR_CHANGE &&
            points->p[i].x != LINE_BREAK)
            mirror->p[i].x = -points->p[i].x ;
    }
}

/* this is just cramming arrays, and counts into structures... it's kind of
 * stupid.  There are a few times when some mirror images are computed, and
 * some things are scaled, since when I made the lists of points, mostly
 * I just kind of imagined the thing in my head and typed in the numbers,
 * (quite a lot of trial and error and correcting of points in that 
 * process, but after awhile, you get pretty good at it.)  But, sometimes
 * I'd get the scale off, and have to scale things a bit to make them the
 * right size -- big enough or small enough, or long and skinny enough
 * or whatever.
 */

#define setup_vect(v, a) { v.p = a; v.npoints = ARRAY_SIZE(a); } 

void init_vects()
{
    int i;

/* Stuck this precalculation of sines and cosines in here as I know
 * this gets called exactly once.
 */
    for (i=0;i<=360;i++) {
        sine[i] = sin((double)i * 3.1415927 * 2.0 / 360.0);
        cosine[i] = cos((double)i * 3.1415927 * 2.0 / 360.0);
        /* printf("%d, %g, %g\n", i, cosine[i], sine[i]);*/
    }

    /* memset(&game_state.go[0], 0, sizeof(game_state.go[0])*MAXOBJS); */
    if (!xmas_mode) {
        setup_vect(player_vect, player_ship_points);
    } else {
        setup_vect(player_vect, reindeer_points);
        setup_vect(reindeer2_vect, reindeer_points2);
    }
    /* The left player ship oints are a copy of the right player ship points. */
    /* I just duplicated them in the source.  So we have to flip them */
    setup_vect(left_player_vect, left_player_ship_points);
    mirror_points(&player_vect, &left_player_vect);
    if (xmas_mode) {
        setup_vect(left_reindeer2_vect, left_reindeer_points2);
        mirror_points(&reindeer2_vect, &left_reindeer2_vect);
    }

    setup_vect(sleigh_vect, sleigh_points);
    setup_vect(left_sleigh_vect, left_sleigh_points);
    mirror_points(&sleigh_vect, &left_sleigh_vect);
#if 0
    for (i=0;i<left_player_vect.npoints;i++) {
        if (left_player_ship_points[i].x != COLOR_CHANGE &&
            left_player_ship_points[i].x != LINE_BREAK)
            left_player_ship_points[i].x *= -1;
    }
#endif

    setup_vect(rocket_vect, rocket_points);
    setup_vect(big_rocket_vect, big_rocket_points);
    setup_vect(jetpilot_vect_left, jetpilot_points_left);
    jetpilot_vect_right.p = jetpilot_points_right;
    mirror_points(&jetpilot_vect_left, &jetpilot_vect_right);
    setup_vect(jet_vect, jet_points);
    setup_vect(spark_vect, spark_points);
    setup_vect(right_laser_vect, right_laser_beam_points);
    setup_vect(fuel_vect, fuel_points);
    setup_vect(fuel_debris_vect, fuel_debris_points);
    setup_vect(house_vect, house_points);
    setup_vect(house_green_vect, house_green_points);
    setup_vect(present_vect, present_points);
    setup_vect(jammer_vect, jammer_points);
    setup_vect(jammer_debris_vect, jammer_debris_points);
    setup_vect(cron_vect, cron_points);
    setup_vect(ship_vect, ships_hull_points);

    /* This scaling was here when I was typing in the ship point numbers just to */
    /* make it easier to see where things were a little off. */
    /* for (i=0;i<ship_vect.npoints;i++) {
        if (ship_vect.p[i].x != LINE_BREAK && ship_vect.p[i].x != COLOR_CHANGE) {
            ship_vect.p[i].x *= 2;
            ship_vect.p[i].y = (ship_vect.p[i].y+20) * 2;
        }
    } */

    setup_vect(tesla_vect, tesla_points);

    setup_vect(octopus_vect, octopus_points);
    for (i=0;i<octopus_vect.npoints;i++) {
        if (octopus_vect.p[i].x != LINE_BREAK && 
            octopus_vect.p[i].x != COLOR_CHANGE) {
            octopus_vect.p[i].x *= 1.5;
            octopus_vect.p[i].y *= 1.5;
        }
    }

    setup_vect(bullet_vect, bullet_points);
    setup_vect(gdb_vect_right, gdb_points_right);
    setup_vect(gdb_vect_right, gdb_points_right);
    setup_vect(gdb_vect_left, gdb_points_left);
    mirror_points(&gdb_vect_left, &gdb_vect_right);

    setup_vect(bomb_vect, bomb_points);
    setup_vect(bridge_vect, bridge_points);
    setup_vect(kgun_vect, kgun_points);
    setup_vect(inverted_kgun_vect, inverted_kgun_points);

    /* I had to scale and translate the airship points as I made it too small */
    /* and picked a poor origin. */
    setup_vect(airship_vect, airship_points);
    for (i=0;i<airship_vect.npoints;i++) {
        if (airship_vect.p[i].x != LINE_BREAK && airship_vect.p[i].x != COLOR_CHANGE) {
            airship_vect.p[i].x *= 3;
            airship_vect.p[i].y = (airship_vect.p[i].y+20) * 3;
        }
    }

    setup_vect(balloon_vect, balloon_points);
    setup_vect(SAM_station_vect, SAM_station_points);
    setup_vect(SAM_station_debris_vect, SAM_station_debris_points);
    setup_vect(humanoid_vect, humanoid_points);
    setup_vect(socket_vect, socket_points);

    /* Make the line segment lists from the stroke_t structures. */
    wwvi_make_font(&gamefont[BIG_FONT], font_scale[BIG_FONT], font_scale[BIG_FONT]);
    wwvi_make_font(&gamefont[SMALL_FONT], font_scale[SMALL_FONT], font_scale[SMALL_FONT]);
    wwvi_make_font(&gamefont[TINY_FONT], font_scale[TINY_FONT], font_scale[TINY_FONT]);
    wwvi_make_font(&gamefont[NANO_FONT], font_scale[NANO_FONT], font_scale[NANO_FONT]);
    set_font(BIG_FONT);
}

void no_draw(__attribute__((unused)) struct game_obj_t *o)
{
    return;
}

void draw_abs_scaled_generic(int x, int y, 
    struct my_vect_obj *v, float xscale, float yscale)
{
    int j;
    int x1, y1, x2, y2;
    x1 = x + (v->p[0].x * xscale);
    y1 = y + (v->p[0].y * yscale);  
    for (j=0;j<v->npoints-1;j++) {
        if (v->p[j+1].x == LINE_BREAK) { /* Break in the line segments. */
            j+=2;
            x1 = x + (v->p[j].x * xscale);
            y1 = y + (v->p[j].y * yscale);  
        }
        if (v->p[j].x == COLOR_CHANGE) {
            gdk_gc_set_foreground(gc, &huex[v->p[j].y]);
            j+=1;
            x1 = x + (v->p[j].x * xscale);
            y1 = y + (v->p[j].y * yscale);  
        }
        x2 = x + (v->p[j+1].x * xscale); 
        y2 = y + (v->p[j+1].y * yscale);
        if (x1 > 0 && x2 > 0)
            wwvi_draw_line(x1, y1, x2, y2); 
        x1 = x2;
        y1 = y2;
    }
}

/* this is what can draw a list of line segments with line
 * breaks and color changes...   This one scales in x, y.
 * It's used for drawing blimps which are overpressurized.
 */
void draw_scaled_generic(struct game_obj_t *o, float xscale, float yscale)
{
    int j;
    int x1, y1, x2, y2;
    gdk_gc_set_foreground(gc, &huex[o->color]);
    x1 = o->x + (o->v->p[0].x * xscale) - game_state.x;
    y1 = o->y + (o->v->p[0].y * yscale) - game_state.y + (SCREEN_HEIGHT/2);  
    for (j=0;j<o->v->npoints-1;j++) {
        if (o->v->p[j+1].x == LINE_BREAK) { /* Break in the line segments. */
            j+=2;
            x1 = o->x + (o->v->p[j].x * xscale) - game_state.x;
            y1 = o->y + (o->v->p[j].y * yscale) - game_state.y + (SCREEN_HEIGHT/2);  
        }
        if (o->v->p[j].x == COLOR_CHANGE) {
            gdk_gc_set_foreground(gc, &huex[o->v->p[j].y]);
            j+=1;
            x1 = o->x + (o->v->p[j].x * xscale) - game_state.x;
            y1 = o->y + (o->v->p[j].y * yscale) - game_state.y + (SCREEN_HEIGHT/2);  
        }
        x2 = o->x + (o->v->p[j+1].x * xscale) - game_state.x; 
        y2 = o->y + (o->v->p[j+1].y * yscale) +(SCREEN_HEIGHT/2) - game_state.y;
        if (x1 > 0 && x2 > 0)
            wwvi_draw_line(x1, y1, x2, y2); 
        x1 = x2;
        y1 = y2;
    }
}

/* this is what can draw a list of line segments with line
 * breaks and color changes...  This gets called quite a lot,
 * so try to make sure it's fast.  There is an inline version
 * of this in draw_objs(), btw. 
 */
void draw_generic(struct game_obj_t *o)
{
    int j;
    int x1, y1, x2, y2;

    if (o->v == NULL)
        return;

#if DEBUG_HITZONE
    if (o->ontargetlist) {
        gdk_gc_set_foreground(gc, &huex[WHITE]);
        x1 = o->x - game_state.x;
        y1 = o->y - game_state.y + (SCREEN_HEIGHT/2);  
        if (x1 > 0 && x1 < SCREEN_WIDTH && y1 < SCREEN_HEIGHT && y1 > 0) {
            /* wwvi_draw_line(x1-10, y1, x1+10, y1); */
            wwvi_draw_line(x1, y1-10, x1, y1+10); 
            wwvi_draw_line(x1-10, y1 + o->above_target_y, x1+10, y1 + o->above_target_y); 
            wwvi_draw_line(x1-10, y1 + o->below_target_y, x1+10, y1 + o->below_target_y); 
        }
    }
#endif
    gdk_gc_set_foreground(gc, &huex[o->color]);
    x1 = o->x + o->v->p[0].x - game_state.x;
    y1 = o->y + o->v->p[0].y - game_state.y + (SCREEN_HEIGHT/2);  
    for (j=0;j<o->v->npoints-1;j++) {
        if (o->v->p[j+1].x == LINE_BREAK) { /* Break in the line segments. */
            j+=2;
            x1 = o->x + o->v->p[j].x - game_state.x;
            y1 = o->y + o->v->p[j].y - game_state.y + (SCREEN_HEIGHT/2);  
        }
        if (o->v->p[j].x == COLOR_CHANGE) {
            gdk_gc_set_foreground(gc, &huex[o->v->p[j].y]);
            j+=1;
            x1 = o->x + o->v->p[j].x - game_state.x;
            y1 = o->y + o->v->p[j].y - game_state.y + (SCREEN_HEIGHT/2);  
        }
        x2 = o->x + o->v->p[j+1].x - game_state.x; 
        y2 = o->y + o->v->p[j+1].y+(SCREEN_HEIGHT/2) - game_state.y;
        if (x1 > 0 && x2 > 0)
            wwvi_draw_line(x1, y1, x2, y2); 
        x1 = x2;
        y1 = y2;
    }
}

void truss_gun_location(struct game_obj_t *parent, 
    __attribute__ ((unused)) struct game_obj_t *child, int *x, int *y)
{
    *x = parent->x;
    *y = parent->y + 10 + 25;
}

void truss_move(struct game_obj_t *o)
{
    if (o->health.prevhealth != o->health.health)
        o->color = hot_metal_color(o->health.health, o->health.maxhealth);
    o->health.prevhealth = o->health.health;
    if ((timer & 0x0f) == 0x0f && 
        o->health.health < o->health.maxhealth)
        o->health.health++;
}

void gunwheel_cut_loose(struct game_obj_t *o, struct game_obj_t *child)
{
    if (child == NULL)
        return;

    if (child->otype == OBJ_TYPE_KGUN && child->tsd.kgun.attached_to == o) {
        child->tsd.kgun.attached_to = NULL;
        /* verlet integration to get velocity... */
        child->vx = child->x - child->lastx;
        child->vy = child->y - child->lasty;
        child->move = bomb_move;
        child->tsd.bomb.bank_shot_factor = 1;
    } else if (child->otype == OBJ_TYPE_GUNWHEEL) {
        child->vx = child->x - child->lastx;
        child->vy = child->y - child->lasty;
        child->move = bomb_move;
        child->tsd.bomb.bank_shot_factor = 1;
        gunwheel_cut_loose(child, child->tsd.gunwheel.left);
        child->tsd.gunwheel.left = NULL;
        gunwheel_cut_loose(child, child->tsd.gunwheel.right);
        child->tsd.gunwheel.right = NULL;
    }
}

void truss_cut_loose_whats_below(struct game_obj_t *o, struct game_obj_t *bomb)
{
    struct game_obj_t *i;
    struct game_obj_t *p;

    p = NULL;
    /* Cut loose everything below */
    for (i=o; i != NULL; i = i->tsd.truss.below) {
        if (i->otype == OBJ_TYPE_TRUSS) {
            if (i->tsd.truss.above) {
                i->tsd.truss.above->tsd.truss.below = NULL;
                i->tsd.truss.above = NULL;
            }
            i->move = bridge_move;
            i->vx = randomn(6)-3;
            i->vy = randomn(6)-3;
        } else if (i->otype == OBJ_TYPE_GUNWHEEL) {
            gunwheel_cut_loose(i, i->tsd.gunwheel.left);
            i->tsd.gunwheel.left = NULL;
            gunwheel_cut_loose(i, i->tsd.gunwheel.right);
            i->tsd.gunwheel.right = NULL;
            i->tsd.gunwheel.attached_to->tsd.truss.below = NULL;
            i->tsd.gunwheel.attached_to = NULL;
            i->move = bomb_move;
            i->tsd.bomb.bank_shot_factor = 1;
            i->vx = randomn(6)-3;
            i->vy = randomn(6)-3;
            i->vx += i->x - i->lastx;
            i->vy += i->y - i->lasty;
            i->y += 15;
            break;
        } else if (i->otype == OBJ_TYPE_KGUN || i->otype == OBJ_TYPE_TESLA) {
            /* make the gun blow up when it lands */
            i->move = bomb_move; 
            i->vx = randomn(6)-3;
            i->vy = randomn(6)-3;

            /* verlet integration to make the thing more or less continue on its path. */
            i->vx += (i->x - i->lastx);
            i->vy += (i->y - i->lasty);

            i->tsd.bomb.bank_shot_factor = 1;
            nice_bank_shot(bomb);
            i->y += 5;

            /* If the tesla tower is discharging... stop it. */
            if (i->otype == OBJ_TYPE_TESLA)
                i->tsd.tesla.discharging = 0;
            
            break;
        } else {
            /* We should never get here. */
            printf("%s:%d: unexpected object type '%c'.  Probably about to crash.\n", 
                __FILE__, __LINE__, i->otype);
            fflush(stdout);
        }
        if (p && p->otype == OBJ_TYPE_TRUSS)
            p->tsd.truss.below = NULL;
        p = i;
    }
    if (p && p->otype == OBJ_TYPE_TRUSS)
        p->tsd.truss.below = NULL;
}

void truss_draw(struct game_obj_t *o)
{
    int x, y, x1, y1, x2, y2, x3, y3, x4, y4;
    gdk_gc_set_foreground(gc, &huex[o->color]);

    x = o->x - game_state.x;
    y = o->y - game_state.y + (SCREEN_HEIGHT/2);  

    x1 = x - o->tsd.truss.top_width / 2; /* top left corner */
    y1 = y - o->tsd.truss.height / 2;

    x2 = x + o->tsd.truss.top_width / 2; /* top right corner */
    y2 = y - o->tsd.truss.height / 2;

    x3 = x - o->tsd.truss.bottom_width / 2; /* bottom left corner */
    y3 = y + o->tsd.truss.height / 2;

    x4 = x + o->tsd.truss.bottom_width / 2; /* bottom right corner */
    y4 = y + o->tsd.truss.height / 2;

    wwvi_draw_line(x3, y3, x1, y1);
    wwvi_draw_line(x1, y1, x2, y2);
    wwvi_draw_line(x2, y2, x4, y4);
    wwvi_draw_line(x4, y4, x1, y1);
#ifndef OPENLASE_FUCK
    wwvi_draw_line(x2, y2, x3, y3);
    wwvi_draw_line(x3, y3, x4, y4);
#endif
}

void tesla_tower_draw(struct game_obj_t *o)
{
    int i, x1, y1, x2, y2, x;

    draw_generic(o);

    x1 = o->x - game_state.x;
    y1 = o->y + 40 - game_state.y + (SCREEN_HEIGHT/2);

    x = (timer & 0x01) * randomn(4);

    for (i = 0; i < x; i++) {
        x2 = o->x + randomn(140)-70 - game_state.x; 
        y2 = o->y + 40 + randomn(140)-70 - game_state.y + (SCREEN_HEIGHT/2);    
        lightning_draw(x1, y1, x2, y2);
    }

    if (!o->tsd.tesla.discharging || !o->tsd.tesla.close_enough)
        return;

    for (i = 0; i < 5; i++) {

        x2 = o->tsd.tesla.targetx + randomn(40)-20 - game_state.x;  
        y2 = o->tsd.tesla.targety + randomn(40)-20 - game_state.y + (SCREEN_HEIGHT/2);  
        lightning_draw(x1, y1, x2, y2);
    }
}

void kgun_draw(struct game_obj_t *o)
{
    int dx, dy;
    int x1, y1;
    int xi, yi;
    int adx, ady;
    int crossbeam_x1, crossbeam_y, crossbeam_x2, xoffset;
    int guntipx, guntipy;
    int gunbackx, gunbacky;
    int thickxo, thickyo;
    int size = o->tsd.kgun.size;
    int invert = o->tsd.kgun.invert;
    int recoilx, recoily;

    draw_generic(o);
    /* Find x and y dist to player.... */
    dx = player_target->x+LASERLEADX*player_target->vx - o->x;
    dy = player_target->y+LASERLEADY*player_target->vy - (o->y + 20*invert);

    x1 = o->x-5 - game_state.x;
    y1 = o->y+(5*invert) - game_state.y + (SCREEN_HEIGHT/2);  
    crossbeam_y = y1 + (20*invert);

    wwvi_draw_line(x1, y1, x1+5, crossbeam_y); 
    wwvi_draw_line(x1+10, y1, x1+6, crossbeam_y);

    /* We're going to draw a swivelling dual gun turret.  Two */
    /* guns mounted on a crossbeam that kind of swivel around */
    /* in a 3-d looking way (but it's fake 3-d, of course.) */
    /* Kind of like this:
     *
     *                                         *  *
     *                                         *  *     
     *                                          **  
     *                                          **
     *                                   ***    **  ***   
     *                                   ***    **  ***     
     *                                    ***   **   ***
     *                                    **************
     *                                     ***        ***
     *                                     ***        ***
     *                                      ***        ***
     *                                       *          * 
     *                                        *          * 
     *                                        *          * 
     *                                         *          * 
     *                                         *          * 
     *
     */

    /* First, scale the larger of dx,dy to 32, and the other by */
    /* a proportional amount (by similar triangles).  This is in */
    /* order to index into the vxy_2_dxy array. */
    
    adx = abs(dx);
    ady = abs(dy);

    /* Calculate indices into vxy_2_dxy array, xi, and yi */
    if (adx > ady) {
        xi = size;
        yi = (size * ady) / adx;
    } else {
        if (ady > adx) {
            yi = size;
            xi = (size * adx) / ady;
        } else { /* abs(dx) == abs(dy) */
            xi = size;
            yi = size;
        }
    }

    /* notice, using y component for x offset... */
    crossbeam_x1 = vxy_2_dxy[xi][yi].y;
    crossbeam_x2 = -crossbeam_x1;

    crossbeam_x1 += o->x - game_state.x;
    crossbeam_x2 += o->x - game_state.x;

    xoffset = crossbeam_x2 - crossbeam_x1;

    guntipx = vxy_2_dxy[xi][yi].x;
    guntipy = vxy_2_dxy[xi][yi].y;

    if (dy > 20)
        guntipy = -guntipy;
    if (dx < 0)
        guntipx = -guntipx;

    gunbackx = -guntipx;
    gunbacky = -guntipy;

    recoilx = 0;
    recoily = 0;

    if (o->tsd.kgun.recoil_amount != 0) {
        recoilx = gunbackx;
        recoily = gunbacky;

        recoilx = (recoilx * (o->tsd.kgun.recoil_amount)) >> 3;
        recoily = (recoily * (o->tsd.kgun.recoil_amount)) >> 3;
        o->tsd.kgun.recoil_amount--;
    }

    thickxo = -gunbacky >> 2; /* notice reversed x,y coords here, to get normal (right angle). */
    thickyo = gunbackx >> 2;  /* gun butt is 1/4 as thick as it is long. */

    guntipx = guntipx << 1; /* gun barrel is 2x as long as the butt of the gun. */
    guntipy = guntipy << 1;

    guntipx += crossbeam_x1 + recoilx;
    gunbackx += crossbeam_x1 + recoilx;
    guntipy += crossbeam_y + recoily;
    gunbacky += crossbeam_y + recoily;

    /* draw the crossbeam */    
    wwvi_draw_line(crossbeam_x1, crossbeam_y, crossbeam_x2, crossbeam_y);

    /* draw the main barrels */
    wwvi_draw_line(gunbackx, gunbacky, guntipx, guntipy);
    wwvi_draw_line(gunbackx+xoffset, gunbacky, guntipx+xoffset, guntipy);

    /* draw the back edges of the gun butts */
    wwvi_draw_line(gunbackx-thickxo, gunbacky-thickyo, 
            gunbackx+thickxo, gunbacky+thickyo);
    wwvi_draw_line(gunbackx-thickxo+xoffset, gunbacky-thickyo, 
            gunbackx+thickxo+xoffset, gunbacky+thickyo);
    /* draw the front edges of the gun butts */
    wwvi_draw_line(crossbeam_x1-thickxo+recoilx, crossbeam_y-thickyo+recoily, 
            crossbeam_x1+thickxo+recoilx, crossbeam_y+thickyo+recoily);
    wwvi_draw_line(crossbeam_x1-thickxo+xoffset+recoilx, 
            crossbeam_y-thickyo+recoily, 
            crossbeam_x1+thickxo+xoffset+recoilx, 
            crossbeam_y+thickyo+recoily);

    /* draw the long sides of the gun butts */
    wwvi_draw_line(gunbackx-thickxo, gunbacky-thickyo, 
            crossbeam_x1-thickxo+recoilx, crossbeam_y-thickyo+recoily);
    wwvi_draw_line(gunbackx-thickxo+xoffset, gunbacky-thickyo, 
            crossbeam_x1-thickxo+xoffset+recoilx, crossbeam_y-thickyo+recoily);
    wwvi_draw_line(gunbackx+thickxo, gunbacky+thickyo, 
            crossbeam_x1+thickxo+recoilx, crossbeam_y+thickyo+recoily);
    wwvi_draw_line(gunbackx+thickxo+xoffset, gunbacky+thickyo, 
            crossbeam_x1+thickxo+xoffset+recoilx, crossbeam_y+thickyo+recoily);
    
}



/* this is an array of random values for use in picking
 * coordinates and colors for noise in the little radar 
 * screen.  We have to precompute and store this because
 * it's like snow on a tv tuned to no channel.  There's
 * a lot of it and it has to be painted FAST, and it's
 * CPU intensive.
 *
 * Why so much, why those values?  Empirically, these values
 * produce believable noise with no discernable pattern.
 * (smaller values had patterns I could see sometimes,
 * not that these values are ideal, but they work.)
 */
#define XRADARN 3313 /* should be 3 unequal primes */
#define YRADARN 223 
#define CRADARN 47

int xradarnoise[XRADARN];
int yradarnoise[YRADARN]; 
int cradarnoise[CRADARN];

void init_radar_noise()
{
    int i;

    for (i=0;i<XRADARN;i++) {
        xradarnoise[i] = randomn(SCREEN_WIDTH - 
            RADAR_LEFT_MARGIN - RADAR_RIGHT_MARGIN) + RADAR_LEFT_MARGIN;
    }
    for (i=0;i<YRADARN;i++) {
        yradarnoise[i] = randomn(RADAR_HEIGHT) + 
            SCREEN_HEIGHT - RADAR_YMARGIN - RADAR_HEIGHT;
    }
    for (i=0;i<CRADARN;i++) {
        cradarnoise[i] = randomn(NCOLORS);
    }
}

static inline void draw_on_radar(struct game_obj_t *o, int y_correction)
{
    int radarx, radary;
    int x1, x2, y1, y2; 

#ifndef OPENLASE_FUCK
    if (!o->radar_image)
        return;

    if (game_state.radar_state != RADAR_RUNNING)
        return;
    
    if (credits <= 0) 
        return;


    /* project game coordinates onto the radar coordiantes. */
    radary = SCREEN_HEIGHT - (RADAR_HEIGHT >> 1) - RADAR_YMARGIN + ((o->y * RADAR_HEIGHT) / 1500);
    radary = radary + y_correction;

    /* If object is off top or bottom of radar screen, don't draw it. */ 
    /* However, if it is a radar jammer, don't bail just yet. as we */
    /* still have to draw the noise that it makes, otherwise, the player */
    /* can subvert the jammer just by flying high enough that the jammer */
    /* goes off the bottom of the radar screen. */
    if (radary < SCREEN_HEIGHT - RADAR_HEIGHT - RADAR_YMARGIN 
        && o->otype != OBJ_TYPE_JAMMER)
        return;
    if (radary > SCREEN_HEIGHT - RADAR_YMARGIN 
        && o->otype != OBJ_TYPE_JAMMER)
        return;

    radarx = ((SCREEN_WIDTH - (RADAR_LEFT_MARGIN + RADAR_RIGHT_MARGIN)) * o->x) / WORLDWIDTH + RADAR_LEFT_MARGIN;

    /* o->radar image is mostly a boolean 0 or 1, indicating whether something's */
    /* on the radar at all, but I used value 2 to make bigger items on the radar. */
    /* and us it in calculating how to draw the little 'x' */
    x1 = radarx - o->radar_image;
    x2 = radarx + o->radar_image;
    y1 = radary - o->radar_image;
    y2 = radary + o->radar_image;


v_setCustomClipping(0, -12000, Y_LOW_CLIP, 12000, Y_HIGH_CLIP);
    /* Don't draw anything for radar jammer's themselves, */
    /* but do draw other stuff.. */ 
    if (o->otype != OBJ_TYPE_JAMMER) 
    {
        /* draw a little x.... */   
        if (o->otype == OBJ_TYPE_HUMAN || o->otype == OBJ_TYPE_PLAYER) 
        {
            /* for the player and the humans, make them a little more */
            /* prominent. */
            if ((timer & 0x04) == 0x04) /* make 'em blink */
            {
              
                v_setCustomClipping(1, -12000, Y_LOW_CLIP, 12000, Y_HIGH_CLIP);
                return;
            }
            gdk_gc_set_foreground(gc, &huex[o->color]);
            wwvi_draw_line(x1, y1, x2, y2); 
            wwvi_draw_line(x1, y2, x2, y1); 
            wwvi_draw_line(x1, y2, x1, y1); 
            wwvi_draw_line(x1, y1, x2, y1); 
            wwvi_draw_line(x2, y1, x2, y2); 
            wwvi_draw_line(x2, y2, x1, y2); 
            v_setCustomClipping(1, -12000, Y_LOW_CLIP, 12000, Y_HIGH_CLIP);
            return;
        }
        gdk_gc_set_foreground(gc, &huex[o->color]);
        wwvi_draw_line(x1, y1, x2, y2); 
        wwvi_draw_line(x1, y2, x2, y1); 
    }

    /* Here's where we put noise on the screen. total_radar_noise */
    /* keeps track of how much noise we've already put on, as there's */
    /* no sense, once the maximum amount of noise has been reached, */
    /* to paint more noise over something already completely obscured */
    /* by noise.  Save the time by skipping, in that case. */
    if (o->otype == OBJ_TYPE_JAMMER && total_radar_noise < MAX_RADAR_NOISE) 
    {
        int xdist, noisecount, i, nx, ny, nc;

        nc = randomn(CRADARN);
        nx = randomn(XRADARN);
        ny = randomn(YRADARN);
        if (nx >= XRADARN) nx = 0;
        if (ny >= YRADARN) ny = 0;
        if (nc >= CRADARN) nc = 0;

        /* based on distance from jammer, figure how much noise to paint. */
        xdist = abs(player_target->x - o->x);
        noisecount = 2000 - xdist;
        /* paint the noise... */
        for (i=0;i<noisecount;i++) 
        {
            int x1, y1, x2, y2;
            gdk_gc_set_foreground(gc, &huex[cradarnoise[nc]]);
            x1 = xradarnoise[nx]-1;
            x2 = xradarnoise[nx]+1;
            y1 = yradarnoise[ny]-1;
            y2 = yradarnoise[ny]+1;
            wwvi_draw_line(x1, y1, x2, y2);
            wwvi_draw_line(x1, y2, x2, y1);
            nx++; if (nx >= XRADARN) nx = 0;
            ny++; if (ny >= YRADARN) ny = 0;
            nc++; if (nc >= CRADARN) nc = 0;
            total_radar_noise++;
            if (total_radar_noise >= MAX_RADAR_NOISE)
                break;
        }
    }
                v_setCustomClipping(1, -12000, Y_LOW_CLIP, 12000, Y_HIGH_CLIP);
#endif
}

static inline int is_off_screen(struct game_obj_t *o)
{
    return ((o->x < (game_state.x - (SCREEN_WIDTH/3))) ||
        (o->x > (game_state.x + 4*(SCREEN_WIDTH/3))) ||
        (o->y < (game_state.y - (SCREEN_HEIGHT))) ||
        (o->y > (game_state.y + (SCREEN_HEIGHT))));
}

#ifdef DEBUG_TARGET_LIST
static inline void debug_adjust_target_list_count(struct game_obj_t *o, int *nitems_on_targetlist)
{
        /* some code for debugging the target list */
        if (o->ontargetlist)
            (*nitems_on_targetlist)++;
}

static inline void debug_draw_target_guidelines(struct game_obj_t *o)
{
    int x1, y1;

    /* some code for debugging the target list */
    if (!o->ontargetlist)
        return;
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    x1 = o->x - game_state.x;
    y1 = o->y - game_state.y + (SCREEN_HEIGHT/2);  
    wwvi_draw_line(x1-5, y1, x1+5, y1);
    wwvi_draw_line(x1, y1-5, x1, y1+5);
} 

static inline void debug_display_target_list_count(int total)
{
    char count[15];
    sprintf(count, "TL:%d", total);
    gdk_gc_set_foreground(gc, &huex[RED]);
    abs_xy_draw_string(count, TINY_FONT, 20, 20);
} 
#else
#define debug_adjust_target_list_count(x, y)
#define debug_draw_target_guidelines(x)
#define debug_display_target_list_count(x)
#endif


/* This gets called frame_rate_hz times every second (normally 30 times a sec) */
/* by main_da_expose().  Don't do anything slow in here. */
void draw_objs()
{
    int i, radary;
#ifdef DEBUG_TARGET_LIST
    int nitems_on_targetlist = 0;
#endif

    /* figure where the center y of the radar screen is */
    radary = SCREEN_HEIGHT - (RADAR_HEIGHT >> 1) - RADAR_YMARGIN + ((player->y * RADAR_HEIGHT) / 1500);
    radary = (SCREEN_HEIGHT - (RADAR_HEIGHT >> 1)) - radary;
    total_radar_noise = 0; /* Each frame gets it's own allotment of noise. */
    for (i = 0; i <= highest_object_number; i++) 
    {
        struct game_obj_t *o = &game_state.go[i];

        debug_adjust_target_list_count(o, &nitems_on_targetlist);

        if (!o->alive)
            continue; /* skip dead things. */

        draw_on_radar(o, radary);

        if (is_off_screen(o)) /* Don't draw offscreen things. */
            continue;

        debug_draw_target_guidelines(o);
        o->draw(o); /* Call object's specialized drawing function. */
    }
    debug_display_target_list_count(nitems_on_targetlist);
}

/* This is used by the "attract mode" screen for drawing the letters */
/* livecursorx, and livecursory are coords of a "cursor" which this */
/* routine automatically advances. */
static void draw_letter(struct my_vect_obj **font, unsigned char letter)
{
    int i, x1, y1, x2, y2;

    if (letter == ' ') {
        livecursorx += font_scale[current_font]*2 + letter_spacing[current_font];
        return;
    }
    if (letter == '\n') {
        livecursorx = font_scale[current_font];
        livecursory += font_scale[current_font];
        return;
    }
    if (font[letter] == NULL) {
        return;
    }

    x1 = livecursorx + font[letter]->p[0].x;
    y1 = livecursory + font[letter]->p[0].y;
    for (i=0;i<font[letter]->npoints-1;i++) {
        if (font[letter]->p[i+1].x == LINE_BREAK) {
            i+=2;
            x1 = livecursorx + font[letter]->p[i].x;
            y1 = livecursory + font[letter]->p[i].y;
        }
        x2 = livecursorx + font[letter]->p[i+1].x;
        y2 = livecursory + font[letter]->p[i+1].y;
        wwvi_draw_line(x1, y1, x2, y2); 
        /*wwvi_draw_line(x1-1, y1+1, x2-1, y2+1); */
        x1 = x2;
        y1 = y2;
    }
    livecursorx += font_scale[current_font]*2 + letter_spacing[current_font];
}

/* Draws a string for the "attract mode", */
static void draw_string(unsigned char *s) 
{

    unsigned char *i;

    for (i=s;*i;i++)
        draw_letter(gamefont[current_font], *i);  
}

/* draws a bunch of strings for the "attract mode." */
static void draw_strings()
{
    int i;

    gdk_gc_set_foreground(gc, &huex[WHITE]); /*&huex[current_color]);*/
    for (i=0;i<ntextlines;i++) {
        /* printf("Drawing '%s' color=%d, x=%d, y=%d\n", 
            textline[i].string, current_color, 
            textline[i].x, textline[i].y); */
        livecursorx = textline[i].x;
        livecursory = textline[i].y;
        set_font(textline[i].font);
        draw_string((unsigned char *) textline[i].string);
    }
}

/* Draws a letter in the given font at an absolute x,y coords on the screen. */
static void abs_xy_draw_letter(struct my_vect_obj **font, 
        unsigned char letter, int x, int y)
{
    int i, x1, y1, x2, y2;

    if (letter == ' ' || letter == '\n' || letter == '\t' || font[letter] == NULL)
        return;

    for (i=0;i<font[letter]->npoints-1;i++) {
        if (font[letter]->p[i+1].x == LINE_BREAK)
            i+=2;
        x1 = x + font[letter]->p[i].x;
        y1 = y + font[letter]->p[i].y;
        x2 = x + font[letter]->p[i+1].x;
        y2 = y + font[letter]->p[i+1].y;
        if (x1 > 0 && x2 > 0)
            wwvi_draw_line(x1, y1, x2, y2); 
    }
}

/* Used for floating labels in the game. */
/* Draws a string at an absolute x,y position on the screen. */ 
static void abs_xy_draw_string(char *s, int font, int x, int y) 
{

    int i;  
    int deltax = font_scale[font]*2 + letter_spacing[font];

    for (i=0;s[i];i++)
        abs_xy_draw_letter(gamefont[font], s[i], x + deltax*i, y);  
}

/* Used for floating labels in the game. */
/* Draws a string at an absolute x,y position on the screen. */ 
/* Exactly like abs_xy_draw_string, but uses rainbow colors. */
static void rainbow_abs_xy_draw_string(char *s, int font, int x, int y) 
{

    int i;  
    int deltax = font_scale[font]*2 + letter_spacing[font];
    int color;


        
    for (i=0;s[i];i++) {
        color = ((((x + deltax*i) * NRAINBOWCOLORS) / (SCREEN_WIDTH*2) + 
            ((y * NRAINBOWCOLORS)/ (SCREEN_HEIGHT*2)) + timer) % NRAINBOWCOLORS) + NCOLORS + NSPARKCOLORS;
        gdk_gc_set_foreground(gc, &huex[color]);
        abs_xy_draw_letter(gamefont[font], s[i], x + deltax*i, y);  
    }
}


/* FIXME: what is the difference between thise, and the "abs" versions?  I forget. */
static void xy_draw_letter(struct my_vect_obj **font, 
        unsigned char letter, int x, int y)
{
    int i, x1, y1, x2, y2;

    if (letter == ' ' || letter == '\n' || letter == '\t' || font[letter] == NULL)
        return;

    for (i=0;i<font[letter]->npoints-1;i++) {
        if (font[letter]->p[i+1].x == LINE_BREAK)
            i+=2;
        x1 = x + font[letter]->p[i].x - game_state.x;
        y1 = y + font[letter]->p[i].y - game_state.y + (SCREEN_HEIGHT/2);
        x2 = x + font[letter]->p[i+1].x - game_state.x;
        y2 = y + font[letter]->p[i+1].y - game_state.y + (SCREEN_HEIGHT/2);
        if (x1 > 0 && x2 > 0)
            wwvi_draw_line(x1, y1, x2, y2); 
        /* wwvi_draw_line(x1-1, y1+1, x2-1, y2+1); */
    }
}

/* FIXME, what is the diff between this and the "abs" version? */
static void xy_draw_string(char *s, int font, int x, int y) 
{

    int i;  
    int deltax = font_scale[font]*2 + letter_spacing[font];

    for (i=0;s[i];i++)
        xy_draw_letter(gamefont[font], (unsigned char) s[i], x + deltax*i, y);  
}


static void add_laserbolt(int x, int y, int vx, int vy, int color, int time)
{
    add_generic_object(x, y, vx, vy, laserbolt_move, laserbolt_draw,
        color, &spark_vect, 0, OBJ_TYPE_SPARK, time);
}

static void add_bright_spark(int x, int y, int vx, int vy, int time)
{
#ifdef OPENLASE
    if (randomn(100) < 20)
#endif
    add_generic_object(x, y, vx, vy, spark_move, bright_spark_draw,
        YELLOW, &spark_vect, 0, OBJ_TYPE_SPARK, time);
}

static void add_spark(int x, int y, int vx, int vy, int time)
{
#ifndef OPENLASE
    if (brightsparks) 
#else
    if (brightsparks && randomn(100) < 20) 
#endif
        add_generic_object(x, y, vx, vy, spark_move, bright_spark_draw,
            YELLOW, &spark_vect, 0, OBJ_TYPE_SPARK, time);
    else
#ifdef OPENLASE
        if (randomn(100) < 20)
#endif
        add_generic_object(x, y, vx, vy, spark_move, spark_draw,
            YELLOW, &spark_vect, 0, OBJ_TYPE_SPARK, time);
}

static void add_pixie_dust(int x, int y, int vx, int vy, int time)
{
    add_generic_object(x, y, vx, vy, pixie_dust_move, pixie_dust_draw,
        WHITE, &spark_vect, 0, OBJ_TYPE_PIXIE_DUST, time);
}

/* Modify *value by a random amount that is a percentage of a difference. */
/* This is used by the recursive routine which makes the ground fractal */
/* to displace the midpoints */
void perturb(int *value, int lower, int upper, double percent)
{
    double perturbation;

    perturbation = percent * (lower - upper) * ((0.0 + random()) / (0.0 + RAND_MAX) - 0.5);
    *value += perturbation;
    return;
}

/* Used to add the lisp code that leaks out of the blimps. */
static void add_symbol(int c, int myfont, int x, int y, int vx, int vy, int time)
{   
    struct my_vect_obj **font = gamefont[myfont];
    if (font[c] != NULL)
        add_generic_object(x, y, vx, vy, symbol_move, draw_generic,
            WHITE, font[c], 0, OBJ_TYPE_SYMBOL, time);
}

/* Used to add the floating scores, "Woohoo!" and "Help!" messages */
static void add_floating_message(char *msg, int font, int x, int y, int vx, int vy, int time)
{
    struct game_obj_t *o;
    o = add_generic_object(x, y, vx, vy, floating_message_move, 
        floating_message_draw, WHITE, NULL, 0,
        OBJ_TYPE_FLOATING_MESSAGE, time);
    if (o == NULL)
        return;
    strncpy((char *) o->tsd.floating_message.msg, msg, 20);
    o->tsd.floating_message.font = font;
}

static void add_score_floater(int x, int y, int score)
{
    char message[20];
    int vx, vy;
    sprintf(message, "%d", score);
    vx = game_state.go[0].vx * 2;
    vy = -7;
/*
    if (x is offscreen)
        return;
    if (y is offscreen)
        return;
    if (x is mostly to the left)
        vx = -4;
    else
        vx = 4;

    if (y is mostly to the bottom)
        vy = -4; 
    else
        vy = +4;
*/
    add_floating_message(message, SMALL_FONT, x, y, vx, vy, 20);
        
}

static void add_floater_message(int x, int y, char *msg)
{
    add_floating_message(msg, SMALL_FONT, x, y, game_state.go[0].vx * 2, -7, 20);
}

/* recurseive function to generate a terrain map between indexes
 * xi1 and xi2 in the terrain array. */ 
void generate_sub_terrain(struct terrain_t *t, int xi1, int xi2)
{
    int midxi;
    int y1, y2, y3, tmp;
    int x1, x2, x3;


    /* find the middle index, if there is no in between */
    /* index, we're done, it's a terminal case. */
    midxi = (xi2 - xi1) / 2 + xi1;
    if (midxi == xi2 || midxi == xi1)
        return;

    /* Find the midpoint, x3,y3 between the given end points, x1,y1, x2,y2. */
    y1 = t->y[xi1];
    y2 = t->y[xi2];
    if (y1 > y2) {
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    y3 = ((y2 - y1) / 2) + y1;

    x1 = t->x[xi1];
    x2 = t->x[xi2];
    if (x1 > x2) {
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }
    x3 = ((x2 - x1) / 2) + x1;

    /* if there is a relatively large scale distance between the given */
    /* endpoints, perturb the midpoint by a factor apprpriate to the */
    /* large scale, otherwise by a factor apppropriate for the small scale. */
    if ((x2 - x1) > 1000) { 
        perturb(&x3, x2, x1, level.large_scale_roughness);
        do { 
            perturb(&y3, x2, x1, level.large_scale_roughness);
        } while ((KERNEL_Y_BOUNDARY - y3) > -250);
    } else {
        perturb(&x3, x2, x1, level.small_scale_roughness);
        do { 
            perturb(&y3, x2, x1, level.small_scale_roughness);
        } while ((KERNEL_Y_BOUNDARY - y3) > -250);
    }

    t->x[midxi] = x3;
    t->y[midxi] = y3;
    /* printf("gst %d %d\n", x3, y3);*/

    /* recursively generate more terrain between x1,y1, x3,y3, */
    /* and between x3,y3, and x2,y2 */
    generate_sub_terrain(t, xi1, midxi);
    generate_sub_terrain(t, midxi, xi2);
}

static struct game_obj_t *add_volcano(int x, int y);
void generate_terrain(struct terrain_t *t)
{
    int volcanox, volcanoi;
    int i, vi1, vi2, vi3, vi4, vi5;

    /* Set up the intial endpoints of the terrain. */
    t->npoints = TERRAIN_LENGTH;
    t->x[0] = 0;
    t->y[0] = 100;
    t->x[t->npoints-1] = WORLDWIDTH;
    t->y[t->npoints-1] = t->y[0];

    /* generate_sub_terrain(t, 0, t->npoints-1);*/
    
    /* put a volcano in... */
    volcanox = WORLDWIDTH / VOLCANO_XFRACTION;
    volcanoi = t->npoints / VOLCANO_XFRACTION;

    vi1 = volcanoi - 30;
    vi2 = volcanoi - 3;
    vi3 = volcanoi;
    vi4 = volcanoi + 3;
    vi5 = volcanoi + 30;

    t->x[vi1] = volcanox - (WORLDWIDTH * 30 / TERRAIN_LENGTH);
    t->x[vi2] = volcanox - (WORLDWIDTH * 5 / TERRAIN_LENGTH);
    t->x[vi3] = volcanox;
    t->x[vi4] = volcanox + (WORLDWIDTH * 5 / TERRAIN_LENGTH);
    t->x[vi5] = volcanox + (WORLDWIDTH * 30 / TERRAIN_LENGTH);
    
    t->y[vi1] = 100;
    t->y[vi2] = -550;
    t->y[vi3] = -500;
    t->y[vi4] = -550;
    t->y[vi5] = 100;

    /* generate the terrain around and including the volcano. */
    generate_sub_terrain(t, 0, vi1);
    generate_sub_terrain(t, vi1, vi2);
    generate_sub_terrain(t, vi2, vi3);
    generate_sub_terrain(t, vi3, vi4);
    generate_sub_terrain(t, vi4, vi5);
    generate_sub_terrain(t, vi5, t->npoints-1);

    /* Put the volcano where he needs to be. */
    volcano_obj = add_volcano(volcanox, t->y[vi3]);

    /* calculate slopes */
    for (i=0;i<t->npoints-2;i++)
        t->slope[i] = (100*(t->y[i+1] - t->y[i]) / (t->x[i+1] - t->x[i]) );
    t->slope[t->npoints-1] = 0;
}

/* allocate and create a set of random connected lines near a point
 * to use as the shapes for debris. */
static struct my_vect_obj *init_debris_vect(struct my_vect_obj **v, struct my_point_t **p)
{
    int i, n;

    n = randomab(2,5);
    *v = (struct my_vect_obj *) malloc(sizeof(**v));
    *p = (struct my_point_t *) malloc(sizeof(**p) * n);
    if (!*v || !*p) {
        if (*v)
            free(*v);
        if (*p)
            free (*p);
        return NULL;
    }

    (*v)->p = *p;
    (*v)->npoints = n;
    
    for (i=0;i<n;i++) {
        (*p)[i].x = randomn(20)-10;
        (*p)[i].y = randomn(10)-5;
    }
    return *v;
}

static void make_debris_forms()
{
    int i;

    for (i=0;i<NDEBRIS_FORMS;i++)
        init_debris_vect(&debris_vect[i], &debris_point[i]);
}

static void free_debris_forms()
{
    int i;

    for (i=0;i<NDEBRIS_FORMS;i++) {
        free(debris_vect[i]);
        free(debris_point[i]);
    }
}

/* Spray chunks of debris from a given point x,y, with initial velocity vx,vy
 * and with radiating velocity r.  Used in explosions. */
static void spray_debris(int x, int y, int vx, int vy, int r, struct game_obj_t *victim, int metal)
{
    int i, z; 
    struct game_obj_t *o;

    for (i = 0; i <= DEBRIS_AMOUNT; i++) {
        z = find_free_obj();
        if (z < 0)
            return;
        o = &game_state.go[z];
        o->last_xi = -1;
        o->x = x;
        o->y = y;
        o->otype = OBJ_TYPE_DEBRIS;
        o->move = bridge_move;
        o->draw = draw_generic;
        o->destroy = generic_destroy_func;
        o->alive = 5*frame_rate_hz + randomn(frame_rate_hz);    
        o->color = victim->color;
        o->radar_image = 1;
        /* add_target(o);*/
        /* o->vx = (int) ((-0.5 + random() / (0.0 + RAND_MAX)) * (r + 0.0) + (0.0 + vx));*/
        /* o->vy = (int) ((-0.5 + random() / (0.0 + RAND_MAX)) * (r + 0.0) + (0.0 + vy));*/
        o->vx = randomn(r) - (r >> 1) + vx; 
        o->vy = randomn(r) - (r >> 1) + vy; 
        o->tsd.debris.debris_type = metal;
        o->v = debris_vect[randomn(NDEBRIS_FORMS)];
    }
}

/* for picking random locations of objects at level startup. */
static int initial_x_location(struct level_obj_descriptor_entry *entry, int i) 
{
    int x;

    if (entry == NULL || entry->x == DO_IT_RANDOMLY) {
        /* don't put things on or near the volcano. */  
        do {
            x = randomn(TERRAIN_LENGTH - 40) + 40;
        } while (abs(volcano_obj->x - terrain.x[x]) < 800);
        return x;
    }

    x = ((entry->x * (TERRAIN_LENGTH - 40))/100) + 40;
    x += i * (entry->xoffset * (TERRAIN_LENGTH - 40))/100;
    return x;
}

static struct game_obj_t *add_flak_gun(int x, int y, int laser_speed, int color, struct game_obj_t *attached_to) 
{
    struct game_obj_t *o;
    o = add_generic_object(x, y, 0, 0, kgun_move, kgun_draw, 
        color, &inverted_kgun_vect, 1, OBJ_TYPE_KGUN, 1);
    if (o) {
        o->tsd.kgun.size = 31;
        o->tsd.kgun.velocity_factor = laser_speed;
        o->tsd.kgun.invert = -1;
        o->tsd.kgun.bank_shot_factor = 1;
        o->tsd.kgun.attached_to = attached_to;
        o->uses_health = 1;
        o->health.maxhealth = level.kgun_health;
        if (attached_to != NULL) {
            /* make sure guns attached to octopi and airships are */
            /* not trivially easy to kill. */
            if (o->health.maxhealth < 4)
                o->health.maxhealth = 4;
        }
        o->health.health = o->health.maxhealth;
        o->health.prevhealth = -1;
        o->above_target_y = -27;
        o->below_target_y = 20;
        level.nflak++;
    }
    return o;
}

static void add_flak_guns(struct terrain_t *t, 
    struct level_obj_descriptor_entry *entry, int laser_speed)
{
    int i, xi;
    struct game_obj_t *o;

    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        o = add_flak_gun(t->x[xi], t->y[xi] - 7, laser_speed, RED, NULL);
        if (o)
            o->debris_form = o->v;  /* for the guns, just the base... */
    }
}

static struct game_obj_t *add_truss(int x, int y, int height, 
    int bottom_width, int top_width, int color)
{
    struct game_obj_t *o;
    o = add_generic_object(x, y, 0, 0, truss_move, truss_draw, 
        color, NULL, 1, OBJ_TYPE_TRUSS, 1);
    if (o == NULL)
        return o;
    o->tsd.truss.top_width = top_width;
    o->tsd.truss.bottom_width = bottom_width;
    o->tsd.truss.height = height;
    o->above_target_y = -height/2;
    o->below_target_y = height/2;
    o->gun_location = truss_gun_location;
    return o;
} 

struct game_obj_t *add_truss_tower(int x, int y, int ntrusses, int inverted, 
    int taper, int color, int width, int height, int health)
{
    int i;
    struct game_obj_t *o, *p;
    int top_width, bottom_width;
    int ty;


    top_width = width;
    bottom_width = width;
    if (inverted) {
        inverted = -1;
        top_width -= taper;
    } else {
        inverted = 1;
        bottom_width -= taper;
    }

    o = NULL;
    p = NULL;
    for (i = 0; i < ntrusses; i++) {
        ty = y + (inverted * (height/2 + (height*i)));
        o = add_truss(x, ty, height, bottom_width, top_width, color);
        if (o == NULL) {
            struct game_obj_t *x, *next;

            for (x = p; x != NULL; x = next ) {
                next = x->tsd.truss.above;
                kill_object(x);
            }
            p = NULL;
            break;
        }
        o->uses_health = 1;
        o->health.maxhealth = health;
        o->health.health = o->health.maxhealth;
        o->health.prevhealth = -1;
        if (i != 0) {
            o->tsd.truss.above = p;
            p->tsd.truss.below = o;
        } else 
            o->tsd.truss.above = NULL;
        p = o;
        p->tsd.truss.below = NULL;

        bottom_width -= taper;
        top_width -= taper;
    }
    return p;
}

static void add_kernel_guns(struct terrain_t *t, 
    struct level_obj_descriptor_entry *entry, int laser_speed)
{
    int i, xi;
    int ntrusses;
    struct game_obj_t *o, *p;


    o = NULL;
    p = NULL;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        ntrusses = randomn(9)+3;

        p = add_truss_tower(t->x[xi], KERNEL_Y_BOUNDARY, ntrusses, 0,
            0, RED, 20, 20, level.kgun_health);
        if (p == NULL)
            break;

        o = add_generic_object(t->x[xi], p->y + 10 + 25, 0, 0, 
            kgun_move, kgun_draw, RED, &kgun_vect, 1, OBJ_TYPE_KGUN, 1);
        if (o != NULL) {
            level.nkguns++;
            o->uses_health = 1;
            o->health.prevhealth = -1;
            o->health.maxhealth = level.kgun_health;
            o->health.health = o->health.maxhealth;
            o->tsd.kgun.size = 31;  /* biggest size */
            o->tsd.kgun.invert = 1; /* this means hanging upside down */
            o->tsd.kgun.velocity_factor = laser_speed;
            o->tsd.kgun.bank_shot_factor = 1;
            o->tsd.kgun.attached_to = p;
            o->below_target_y = 27;
            o->above_target_y = -20;
        }
        if (p != NULL)
            p->tsd.truss.below = o;
    }
}

static void add_tesla_towers(struct terrain_t *t, 
    struct level_obj_descriptor_entry *entry)
{
    int i, xi;
    int ntrusses;
    struct game_obj_t *o, *p;

    o = NULL;
    p = NULL;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        ntrusses = randomn(9)+3;

        p = add_truss_tower(t->x[xi], KERNEL_Y_BOUNDARY, ntrusses, 0,
            2, RED, 40, 20, 25);
        if (p == NULL)
            break;

        o = add_generic_object(t->x[xi], p->y + 10 + 25, 0, 0, 
            tesla_tower_move, tesla_tower_draw, WHITE, &tesla_vect, 1, OBJ_TYPE_TESLA, 1);
        if (o != NULL) {
            o->uses_health = 1;
            o->health.prevhealth = -1;
            o->health.maxhealth = level.kgun_health;
            o->health.health = o->health.maxhealth;
            o->tsd.tesla.invert = 0;
            o->tsd.tesla.targetx = o->x;
            o->tsd.tesla.targety = o->y;
            o->tsd.tesla.discharging = 0;
            o->tsd.tesla.close_enough = 0;
            o->tsd.tesla.time_to_discharge = 0;
            o->tsd.tesla.attached_to = p;
            o->below_target_y = 27;
            o->above_target_y = -20;
        }
        if (p != NULL)
            p->tsd.truss.below = o;
    }
}

static void add_rockets(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    int i, xi;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        add_generic_object(t->x[xi], t->y[xi] - 7, 0, 0, 
            rocket_move, draw_generic, WHITE, &rocket_vect, 1, OBJ_TYPE_ROCKET, 1);
        level.nrockets++;
    }
}

static void add_big_rockets(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    int i, xi;
    struct game_obj_t *o;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        o = add_generic_object(t->x[xi], t->y[xi] - 15, 0, 0, 
            big_rocket_move, draw_generic, WHITE, &big_rocket_vect, 1, OBJ_TYPE_BIG_ROCKET, 1);
        if (o != NULL) {
            o->above_target_y = -35;
            o->below_target_y = 15;
            level.nbigrockets++;
        }
    }
}

static void add_pilot(int pilotx, int piloty, int pilotvx, int pilotvy) 
{
    (void) add_generic_object(pilotx, piloty, pilotvx, pilotvy, 
        pilot_move, draw_generic, WHITE, 
        &jetpilot_vect_left, 1, OBJ_TYPE_JETPILOT, 1);
}

static void add_jets(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    struct game_obj_t *o;
    int i, xi;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        o = add_generic_object(t->x[xi], KERNEL_Y_BOUNDARY + 5, 0, 0, 
            jet_move, draw_generic, WHITE, &jet_vect, 1, OBJ_TYPE_JET, 1);
        if (o) {
            /* in case jets get shot down, they turn into bombs, so need this... */
            o->tsd.bomb.bank_shot_factor = 1;
            level.njets++;
        }
    }
}

/* Insert the points in one list into the middle of another list */
/* This is used by the routines which create the building shapes to */
/* embelish the buildings with various ornamentation. */
static void insert_points(struct my_point_t host_list[], int *nhost, 
        struct my_point_t injection[], int ninject, 
        int injection_point)
{
    /* make room for the injection */
    memmove(&host_list[injection_point + ninject], 
        &host_list[injection_point], 
        (*nhost - injection_point) * sizeof(host_list[0]));

    /* make the injection */
    memcpy(&host_list[injection_point], &injection[0], (ninject * sizeof(injection[0])));
    *nhost += ninject;
}

static void embellish_roof(struct my_point_t *building, int *npoints, int left, int right);
static void indent_roof(struct my_point_t *building, int *npoints, int left, int right);

static struct my_point_t mosquey_roof[] = {
        { -10, 0 },
        { -15, -5 },
        { -17, -10 },
        { -15, -15 },
        {  -5, -25 },
        {  0, -35 },
        {  5, -25 },
        { 15, -15 },
        { 17, -10 },
        { 15, -5 },
        { 10, 0 },
    };
static void add_mosquey_roof(struct my_point_t *building, int *npoints, int left, int right)
{
    struct my_point_t p[ sizeof(mosquey_roof)/  sizeof(mosquey_roof[0])];
    unsigned int i;
    int width;
    double factor;

    if (building[right].x - building[left].x > 30) {
        /* only looks good on narraw buildings, if it's too wide */
        /* skip the mosquey roof and just indent */
        indent_roof(building, npoints, left, right);
        return;
    }

    memcpy(p, mosquey_roof, sizeof(mosquey_roof));

    width = building[right].x - building[left].x; 
    factor = (0.0 + width) / 20.0;

    for (i=0;i<ARRAY_SIZE(p);i++) {
        p[i].x = (p[i].x * factor) + building[left].x + (10 * factor);
        p[i].y = (p[i].y * factor) + building[left].y;
    }
    insert_points(building, npoints, p, ARRAY_SIZE(mosquey_roof), right);
    
    return;
}

static void peak_roof(struct my_point_t *building, int *npoints, int left, int right)
{
    struct my_point_t p;
    int height;
    height = randomab(20,140);
    height = (int) (height * (building[right].x-building[left].x) / 100.0);
    if (height > 60)
        height = 60;

    p.x = ((building[right].x - building[left].x) / 2) + building[left].x;
    p.y = building[right].y - height;

    insert_points(building, npoints, &p, 1, right);
    
    return;
}

static void turret_roof(struct my_point_t *building, int *npoints, int left, int right)
{
    struct my_point_t p[8];
    int height, indent, width;
    int old_npoints;

    width = randomab(15,30);
    height = randomab(20,60);
    indent = randomab(0,10);
    height = (int) (height * (building[right].x-building[left].x) / 100.0);
    indent = (int) (indent * (building[right].x-building[left].x) / 100.0);
    width = (int) (width * (building[right].x-building[left].x) / 100.0);
    
    p[0].x = building[left].x - indent;
    p[0].y = building[left].y - indent;

    p[1].x = p[0].x;
    p[1].y = p[0].y - height;

    p[2].x = p[1].x + width;
    p[2].y = p[1].y;

    p[3].x = p[2].x;
    p[3].y = p[0].y;

    p[4].x = building[right].x - width + indent; 
    p[4].y = p[3].y;

    p[5].x = p[4].x; 
    p[5].y = p[4].y - height;

    p[6].x = p[5].x + width; 
    p[6].y = p[5].y;

    p[7].x = p[6].x; 
    p[7].y = p[6].y + height;

    insert_points(building, npoints, p, 8, right);
    old_npoints = *npoints;

    embellish_roof(building, npoints, left+2, left+3);

    embellish_roof(building, npoints, left+6 + *npoints - old_npoints, left+7 + *npoints -old_npoints);
    embellish_roof(building, npoints, left+4 + *npoints - old_npoints, left+5 + *npoints -old_npoints);

    return;
}

static void indent_roof(struct my_point_t *building, int *npoints, int left, int right)
{
    struct my_point_t p[4];
    int indent;
    int height;
    indent = randomab(5,25);
    height = randomab(-20,30);

    height = (int) (height * (building[right].x-building[left].x) / 100.0);
    indent = (int) (indent * (building[right].x-building[left].x) / 100.0);
    
    p[0].x = building[left].x + indent;
    p[0].y = building[left].y;

    p[1].x = p[0].x;
    p[1].y = building[left].y - height;
    p[2].x = building[right].x - indent;
    p[2].y = p[1].y;
    p[3].x = p[2].x;
    p[3].y = building[right].y;

    insert_points(building, npoints, p, 4, right);
    embellish_roof(building, npoints, left+4, left+5);
    embellish_roof(building, npoints, left+2, left+3);

    return;
}

static void addtower_roof(__attribute__((unused)) struct my_point_t *building,
    __attribute__((unused)) int *npoints,
    __attribute__((unused)) int left,
    __attribute__((unused)) int right)
{

    return;
}

static void crenellate_roof(__attribute__((unused)) struct my_point_t *building,
    __attribute__((unused)) int *npoints,
    __attribute__((unused)) int left,
    __attribute__((unused)) int right)
{
    return;
}


static void embellish_roof(struct my_point_t *building, int *npoints, int left, int right)
{
    int r = randomn(10);

    switch (r) {
        case 1: peak_roof(building, npoints, left, right);
            break;
        case 2:turret_roof(building, npoints, left, right);
            break;
        case 3: add_mosquey_roof(building, npoints, left, right);
            break;
        case 4:
        case 5: indent_roof(building, npoints, left, right);
            break;
        case 8: crenellate_roof(building, npoints, left, right);
            break;
        case 9: addtower_roof(building, npoints, left, right);
            break;
        default:
            break;
    }
}

static void add_window(struct my_point_t *building, int *npoints, 
        int x1, int y1, int x2, int y2)
{
    /* printf("aw: npoints = %p\n", npoints); fflush(stdout); */
    if (*npoints > 1000) {
        /* printf("npoints = %d\n", *npoints); fflush(stdout); */
        return;
    }
    building[*npoints].x = LINE_BREAK; 
    building[*npoints].y = LINE_BREAK; *npoints += 1;
    building[*npoints].x = x1; 
    building[*npoints].y = y1; *npoints += 1;
    building[*npoints].x = x1; 
    building[*npoints].y = y2; *npoints += 1;
    building[*npoints].x = x2; 
    building[*npoints].y = y2; *npoints += 1;
    building[*npoints].x = x2; 
    building[*npoints].y = y1; *npoints += 1;
    building[*npoints].x = x1; 
    building[*npoints].y = y1; *npoints += 1;
}

static void add_windows(struct my_point_t *building, int *npoints, 
        int x1, int y1, int x2, int y2)
{
    int nwindows;
    int xindent, yindent;
    int spacing;
    int width;
    int i;

    /* printf("aws: npoints = %p\n", npoints); fflush(stdout); */
    xindent = randomab(5, 20);
    yindent = randomab(5, 20);
    spacing = randomab(3, 15);
    x2 -= xindent;
    x1 += xindent;
    y1 -= yindent;
    y2 += yindent;
    /* printf("add_windows, %d,%d  %d,%d\n", x1, y1, x2, y2); */

    if (x2 - x1 < 30)
        return;
    if (y1 - y2 < 20)
        return;

    nwindows = randomab(1, 5);
    width = (x2-x1) / nwindows; 
    /* printf("adding %d windows, *npoints = %d\n", nwindows, *npoints); */
    for (i=0;i<nwindows;i++) {
        /* printf("Adding window -> %d, %d, %d, %d, npoints = %d\n",
            x1 + (i*width), y1, x1 + (i+1)*width - spacing, y2, *npoints);
        fflush(stdout); */
        add_window(building, npoints,
            x1 + (i*(width)), y1, x1 + (i+1)*(width) - spacing, y2);
    }
    return;
}

static void embellish_building(struct my_point_t *building, int *npoints)
{
    int x1, y1, x2, y2;

    x1 = building[0].x;
    y1 = building[0].y;
    x2 = building[2].x;
    y2 = building[2].y;
    embellish_roof(building, npoints, 1, 2);
    add_windows(building, npoints, x1, y1, x2, y2);
    return;
}

#if 0
int find_free_obj()
{
    int i;
    for (i=0;i<MAXOBJS;i++)
        if (!game_state.go[i].alive)
            return i;
    return -1;
}

static inline void clearbit(unsigned int *value, unsigned char bit)
{
    *value &= ~(1 << bit);
}
#endif


int find_free_obj()
{
    int i, j, answer;
    unsigned int block;

    /* this might be optimized by find_first_zero_bit, or whatever */
    /* it's called that's in the linux kernel.  But, it's pretty */
    /* fast as is, and this is portable without writing asm code. */
    /* Er, portable, except for assuming an int is 32 bits. */

    for (i=0;i<NBITBLOCKS;i++) {
        if (free_obj_bitmap[i] == 0xffffffff) /* is this block full?  continue. */
            continue;

        /* I tried doing a preliminary binary search using bitmasks to figure */
        /* which byte in block contains a free slot so that the for loop only */
        /* compared 8 bits, rather than up to 32.  This resulted in a performance */
        /* drop, (according to the gprof) perhaps contrary to intuition.  My guess */
        /* is branch misprediction hurt performance in that case.  Just a guess though. */

        /* undoubtedly the best way to find the first empty bit in an array of ints */
        /* is some custom ASM code.  But, this is portable, and seems fast enough. */
        /* profile says we spend about 3.8% of time in here. */
    
        /* Not full. There is an empty slot in this block, find it. */
        block = free_obj_bitmap[i];         
        for (j=0;j<32;j++) {
            if (block & 0x01) { /* is bit j set? */
                block = block >> 1;
                continue;   /* try the next bit. */
            }

            /* Found free bit, bit j.  Set it, marking it non free.  */
            free_obj_bitmap[i] |= (1 << j);
            answer = (i * 32 + j);  /* return the corresponding array index, if in bounds. */
            if (answer < MAXOBJS) {
                if (game_state.go[answer].next != NULL || game_state.go[answer].prev != NULL ||
                    game_state.go[answer].ontargetlist) {
                        printf("T%c ", game_state.go[answer].otype);
                }
                game_state.go[answer].ontargetlist=0;
                if (answer > highest_object_number)
                    highest_object_number = answer;
                return answer;
            }
            return -1;
        }
    }
    return -1;
}

static void add_building(struct terrain_t *t, int xi)
{
    int npoints = 0;
    int height;
    int width;
    struct my_point_t scratch[1000];
    struct my_point_t *building;
    struct my_vect_obj *bvec; 
    int objnum;
    struct game_obj_t *o;
    int i, x, y;
    int roofy;

    memset(scratch, 0, sizeof(scratch[0]) * 1000);
    objnum = find_free_obj();
    if (objnum == -1)
        return;

    height = randomab(50, 300); 
    width = randomab(5,MAXBUILDING_WIDTH);
    scratch[0].x = t->x[xi];    
    scratch[0].y = t->y[xi];    

    /* choose the roof level to be based on the highest of the left 
       or right side of terrain on which the building is set. */
    roofy = t->y[xi] < t->y[xi+width] ? t->y[xi] - height : t->y[xi+width] - height;

    scratch[1].x = scratch[0].x;
    scratch[1].y = roofy;
    scratch[2].x = t->x[xi+width];
    scratch[2].y = roofy;        /* make roof level, even if ground isn't. */
                     /* FIXME< roof level may be _below_ of */
                     /*of the gournd points, not good.. */
    scratch[3].x = scratch[2].x;
    scratch[3].y = t->y[xi+width];
    npoints = 4;

    y = scratch[1].y;
    x = ((scratch[2].x - scratch[0].x) / 2) + scratch[0].x;

    for (i=0;i<npoints;i++) {
        scratch[i].x -= x;
        scratch[i].y -= y;
    }

    embellish_building(scratch, &npoints);

    building = malloc(sizeof(scratch[0]) * npoints);
    bvec = malloc(sizeof(*bvec));
    if (building == NULL || bvec == NULL)
        return;

    memcpy(building, scratch, sizeof(scratch[0]) * npoints);
    bvec->p = building;
    bvec->npoints = npoints;

    o = &game_state.go[objnum];
    o->x = x;
    o->y = y;
    o->vx = 0;
    o->vy = 0;
    o->v = bvec;
    o->move = no_move;
    o->otype = OBJ_TYPE_BUILDING;
    add_target(o);
    o->color = BLUE;
    o->alive = 1;
    o->draw = draw_generic;
    o->move = NULL;
    o->destroy = generic_destroy_func;
    /* printf("b, x=%d, y=%d\n", x, y); */
}

void free_buildings()
{
    int i;
    for (i=0;i<MAXOBJS;i++) {
        if (game_state.go[i].otype == OBJ_TYPE_BUILDING && game_state.go[i].alive) {
            free(game_state.go[i].v->p);
            free(game_state.go[i].v);
            game_state.go[i].v = NULL;
        }
    }
}

static void add_buildings(struct terrain_t *t)
{
    int xi, i;
    int volcanoi;

    volcanoi = t->npoints / VOLCANO_XFRACTION;

    for (i=0;i<level.nbuildings;i++) {
        xi = volcanoi + 5 + 
            randomn(TERRAIN_LENGTH-MAXBUILDING_WIDTH-volcanoi-6);
        add_building(t, xi);
    }
}

static int find_dip(struct terrain_t *t, int n, int *px1, int *px2, int minlength)
{
    /*  Find the nth dip in the terrain, over which we might put a bridge. */

    int i, j, x1, x2, found, highest, lowest;
    /* printf("top of find_dip, n=%d\n", n); */

    found=0;

    /* make sure we don't put a bridge across the caldera of the volcano */
    x1 = t->npoints / VOLCANO_XFRACTION + 5;
    x2 = x1; /* make the compiler quit bitching. */

    for (i=0;i<n;i++) {
        for (;x1<TERRAIN_LENGTH-100;x1++) {
            for (x2=x1+5; t->x[x2] - t->x[x1] < minlength; x2++)
                /* do nothing in body of for loop */ 
                ;
            highest = 60000; /* highest altitude, lowest y value */
            lowest = -1; /* lowest altitude, highest y value */
            
            for (j=x1+1; j<x2-1;j++) {
                if (t->y[j] < highest)
                    highest = t->y[j];
                if (t->y[j] > lowest)
                    lowest = t->y[j];
            }

            /* Lowest point between x1,x2 must be at least 100 units deep */
            if (lowest - t->y[x1] < 100 || lowest - t->y[x2] < 100)
                continue; /* not a dip */

            /* highest point between x1,x2 must be lower than both */
            if (t->y[x1] >= highest || t->y[x2] >= highest)
                continue; /* not a dip */
    
            /* printf("found a dip x1=%d, x2=%d.\n", x1, x2); */
            break;
        }
        if (x1 >= TERRAIN_LENGTH-100)
            break;
        found++;
        if (found == n)
            break;
        x1 = x2;
    }
    /* printf("found %d dips.\n", found); */
    if (found == n) {
        *px1 = x1;
        *px2 = x2;
        return 0;
    }
    return -1;
}

static void add_bridge_piece(int x, int y)
{
    add_generic_object(x, y, 0, 0, no_move, draw_generic, RED, &bridge_vect, 1, OBJ_TYPE_BRIDGE, 1);
}

static void add_bridge_column(struct terrain_t *t, 
    int rx, int ry,  /* real coords */
    int x1, int x2) /* index into terrain */
{
    int i, terminal_y;
    i = x1;

    /* printf("add_bridge_column, rx =%d, ry=%d, x1 = %d, x2=%d\n", rx, ry, x1, x2); */
    while (t->x[i] <= rx && i <= x2)
        i++;
    if (i>x2)  /* we're at the rightmost end of the bridge, that is, done. */
        return;

    terminal_y = interpolate(rx, t->x[i-1], t->y[i-1], t->x[i], t->y[i]);
    /* printf("term_y = %d, intr(%d, %d, %d, %d, %d)\n", 
        terminal_y, rx, t->y[i-1], t->x[i-1], t->y[i], t->x[i]); */

    if (ry > terminal_y) /* shouldn't happen... */
        return;

    do {
        add_bridge_piece(rx, ry);
        ry += BRIDGE_SIZE;
    } while (ry <= terminal_y);
    return;
}

static void add_bridge(struct terrain_t *t, int x1, int x2)
{
    int x, y;
    int rx1, rx2, ry1, ry2;

    ry1 = t->y[x1], 
    rx1 = t->x[x1], 
    ry2 = t->y[x2], 
    rx2 = t->x[x2], 

    /* Make the bridge span */
    x = rx1 - (rx1 % 40);
    while (x < rx2) {
        y = interpolate(x, rx1, ry1, rx2, ry2); 
        add_bridge_piece(x, y);
        if ((x % 40) == 32) {
            add_bridge_column(t, x, y + BRIDGE_SIZE, x1, x2);
        }
        x += BRIDGE_SIZE;
    }
}

static void add_bridges(struct terrain_t *t)
{
    int i, n;
    int x1, x2;

    for (i=0;i<level.nbridges;i++) {
        n = find_dip(t, i+1, &x1, &x2, 600);
        if (n != 0) 
            n = find_dip(t, i+1, &x1, &x2, 500);
        if (n != 0)
            n = find_dip(t, i+1, &x1, &x2, 400);
        if (n != 0)
            n = find_dip(t, i+1, &x1, &x2, 300);
        if (n != 0)
            n = find_dip(t, i+1, &x1, &x2, 200);
        if (n == 0)
            add_bridge(t, x1, x2);
        else
            break;
    }
}

static void add_cron(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    int xi, i;
    struct game_obj_t *o;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        o = add_generic_object(t->x[xi], t->y[xi]-30, 0, 0, 
            cron_move, cron_draw, GREEN, &cron_vect, 1, OBJ_TYPE_CRON, 1);
        if (o != NULL) {
            o->destroy = cron_destroy;
            o->tsd.cron.tx = o->x;
            o->tsd.cron.ty = o->y;
            o->tsd.cron.myhuman = NULL;
            o->tsd.cron.eyepos = 0;
            o->radar_image = 1;
            o->tsd.cron.state = CRON_STATE_SEEKING_HUMAN;
            o->tsd.cron.pissed_off_timer = 0;
            o->above_target_y = -17;
            o->below_target_y = 2;
            level.ncron++;
        }
    }
}

static struct game_obj_t *add_volcano(int x, int y)
{
    return add_generic_object(x, y, 0, 0, volcano_move, no_draw, RED, NULL, 0, OBJ_TYPE_VOLCANO, 1);
}

static void add_jammers(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    int xi, i;
    struct game_obj_t *o;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        o = add_generic_object(t->x[xi], t->y[xi], 0, 0, 
            no_move, jammer_draw, GREEN, &jammer_vect, 1, OBJ_TYPE_JAMMER, 1);
        if (o) {
            o->tsd.jammer.width = 0;
            o->tsd.jammer.direction = 2;
            o->radar_image = 1;
            o->above_target_y = -20;
            o->below_target_y = 0;
            o->debris_form = &jammer_debris_vect;
            level.njammers++;
        }
        
    }
}

static void add_houses(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    int xi, i;
    struct game_obj_t *o;

    if (!xmas_mode) 
        return;

    /* for (i=0;i<entry->nobjs;i++) {*/
    for (i=0;i<NHOUSES;i++) {
        xi = initial_x_location(entry, i);
        o = add_generic_object(t->x[xi], t->y[xi]-25, 0, 0, 
            house_move, draw_generic, WHITE, &house_vect, 1, OBJ_TYPE_HOUSE, 1);
        if (o) {
            o->above_target_y = -2 * LASER_Y_PROXIMITY;
            o->below_target_y = 3 * LASER_Y_PROXIMITY;
            o->tsd.house.santa_came = 0;
        }
    }
}

static void add_fuel(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    int xi, i;
    struct game_obj_t *o;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        o = add_generic_object(t->x[xi], t->y[xi]-30, 0, 0, 
            fuel_move, fuel_draw, ORANGE, &fuel_vect, 1, OBJ_TYPE_FUEL, 1);
        if (o) {
            o->tsd.fuel.level = FUELTANK_CAPACITY;
            level.nfueltanks++;
            o->above_target_y = -2 * LASER_Y_PROXIMITY;
            o->below_target_y = 3 * LASER_Y_PROXIMITY;
            o->debris_form = &fuel_debris_vect;
        }
    }
}

static void add_ships(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    struct game_obj_t *o;
    int xi, i;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        o = add_generic_object(t->x[xi], t->y[xi]-30, 0, 0, 
            ship_move, draw_generic, ORANGE, &ship_vect, 1, OBJ_TYPE_SHIP, 50*PLAYER_LASER_DAMAGE);
        if (o) {
            o->radar_image = 1;
            level.nships++;
        }
    }
}

static void add_octopi(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    int xi, i, j, k, count;
    struct game_obj_t *o;
    int color_scheme;
    int other_color;
    int color;

    count = 0;
    for (i=0;i<entry->nobjs;i++) {
        color_scheme = randomn(100) % 4;
        color = randomn(NCOLORS);
        if (color == BLACK) /* exlude black */
            color = YELLOW;
        other_color = randomn(NCOLORS);
        if (other_color == BLACK) /* exlude black */
            other_color = YELLOW;
        xi = initial_x_location(entry, i);
        o = add_generic_object(t->x[xi], t->y[xi]-50 - randomn(100), 0, 0, 
            octopus_move, draw_generic, color, &octopus_vect, 1, OBJ_TYPE_OCTOPUS, 1);
        if (o == NULL)
            continue;
        level.noctopi++;
        count++;
        o->destroy = octopus_destroy;
        o->tsd.octopus.awake = 0;
        o->radar_image = 1;
        o->above_target_y = -25;
        o->below_target_y = 5;
        o->gun_location = octopus_gun_location;
        o->attached_gun = add_flak_gun(o->x, o->y, FAST_LASER, YELLOW, o); 
        o->uses_health = 1;
        o->health.maxhealth = level.kgun_health;
        o->health.health = o->health.maxhealth;
        o->health.prevhealth = -1;
        /* Make the tentacles. */
        for (j=0;j<8;j++) {
            struct game_obj_t *t;
            int length = randomn(5) + 25;
            double length_factor = 0.90;
            o->tsd.octopus.tentacle[j] = add_generic_object(o->x, o->y, 0, 0,
                tentacle_move, tentacle_draw, CYAN, NULL, 1, OBJ_TYPE_TENTACLE, 1);
            if (o->tsd.octopus.tentacle[j] == NULL)
                continue;
            t = o->tsd.octopus.tentacle[j];
            t->tsd.tentacle.attached_to = o;
            t->tsd.tentacle.upper_angle = 359;
            t->tsd.tentacle.lower_angle = 181;
            t->tsd.tentacle.nsegs = MAX_TENTACLE_SEGS;
            t->tsd.tentacle.angle = 0;
            t->tsd.tentacle.color_scheme = color_scheme;
            t->tsd.tentacle.other_color = other_color;
            t->color = o->color;
            for (k=0;k<MAX_TENTACLE_SEGS;k++) {
                t->tsd.tentacle.seg[k].angle = 
                    TENTACLE_RANGE(t->tsd.tentacle);
                t->tsd.tentacle.seg[k].length = length;
                t->tsd.tentacle.seg[k].angular_v = 0;
                t->tsd.tentacle.seg[i].dest_angle = 
                    TENTACLE_RANGE(t->tsd.tentacle);
                length = length * length_factor;
                length_factor += 0.01;
                if (length_factor > 0.97) 
                    length_factor = 0.97;
                if (length == 1) {
                    t->tsd.tentacle.nsegs = k;
                    break;
                }
            }
        }
    }
}

static void add_gdbs(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    int xi, i; /*, j, k, count;*/

    /* count = 0;*/
    struct game_obj_t *o;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        o = add_generic_object(t->x[xi], t->y[xi]-30, 0, 0, 
            gdb_move, gdb_draw, CYAN, &gdb_vect_left, 1, OBJ_TYPE_GDB, 1);
        if (o != NULL) {
            o->destroy = generic_destroy_func;
            o->tsd.gdb.awake = 0;
            o->radar_image = 1;
            level.ngdbs++;
            o->above_target_y = -20;
            o->below_target_y = 0;
        }
    
    }
}

/* Adds a bunch of loose tentacles sprinkled around the terrain. */
static void add_tentacles(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    int xi, i,j, length;
    struct game_obj_t *o;
    double length_factor;

    for (j=0;j<entry->nobjs;j++) {
        length = randomn(30) + 9;
        length_factor = 0.90;
        xi = initial_x_location(entry, j);
        o = add_generic_object(t->x[xi], t->y[xi], 0, 0,
            tentacle_move, tentacle_draw, CYAN, NULL, 1, OBJ_TYPE_TENTACLE, 1);
        if (o != NULL) {
            level.ntentacles++;
            o->tsd.tentacle.color_scheme = randomn(100) % 3;
            o->tsd.tentacle.upper_angle = 160;
            o->tsd.tentacle.lower_angle = 20;
            o->tsd.tentacle.nsegs = MAX_TENTACLE_SEGS;
            o->tsd.tentacle.angle = 0;
            for (i=0;i<MAX_TENTACLE_SEGS;i++) {
                o->tsd.tentacle.seg[i].angle = randomn(60)-30;
                o->tsd.tentacle.seg[i].length = length;
                o->tsd.tentacle.seg[i].angular_v = 0;
                o->tsd.tentacle.seg[i].dest_angle = TENTACLE_RANGE(o->tsd.tentacle);
                length = length * length_factor;
                length_factor += 0.01;
                if (length_factor > 0.97) 
                    length_factor = 0.97;
                if (length == 1) {
                    o->tsd.tentacle.nsegs = i;
                    break;
                }
            }
        }
    }

}

static void add_SAMs(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    int xi, i;
    struct game_obj_t *o;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        o = add_generic_object(t->x[xi], t->y[xi], 0, 0, 
            sam_move, draw_generic, WHITE, &SAM_station_vect, 1, OBJ_TYPE_SAM_STATION, 1);
        if (o) {
            o->above_target_y = -50;
            o->below_target_y = 0;
            o->debris_form = &SAM_station_debris_vect;
            level.nsams++;
        }
    }
}

static void add_reindeer()
{
    int i;
    struct game_obj_t *o, *previous;

    if (!xmas_mode) {
        player_target = player;
        return;
    }

    previous = player;
    player->tsd.reindeer.rudolph = 1;
    for (i=0;i<4;i++) {
        if (i != 3) {
            o = add_generic_object(previous->x - 20, previous->y, 0, 0,
                reindeer_move, xmas_player_draw, ORANGE, &player_vect, 1, OBJ_TYPE_REINDEER, 1);
        } else {
            o = add_generic_object(previous->x - 20, previous->y, 0, 0,
                reindeer_move, xmas_player_draw, WHITE, &sleigh_vect, 1, OBJ_TYPE_REINDEER, 1);
            player_target = o;
        }
        if (o == NULL)
            return;
        o->tsd.reindeer.sleigh = (i == 3);
        o->tsd.reindeer.rudolph = 0;
        o->bullseye = previous;
        o->tsd.reindeer.count = previous->tsd.reindeer.count;
        o->tsd.reindeer.count2 = previous->tsd.reindeer.count2;
        o->tsd.reindeer.direction = 1;
        o->radar_image = 0;
        previous = o;
    }
}

static void add_humanoids(struct terrain_t *t)
{
    int xi, i;
    struct game_obj_t *o;

    for (i=0;i<level.nhumanoids;i++) {
        xi = initial_x_location(NULL, 0);
        o = add_generic_object(t->x[xi], t->y[xi], 0, 0, 
            humanoid_move, draw_generic, MAGENTA, &humanoid_vect, 1, OBJ_TYPE_HUMAN, 1);
        human[i] = o;
        if (o != NULL) {
            o->tsd.human.abductor = NULL;
            o->tsd.human.picked_up = 0;
            o->tsd.human.on_ground = 1;
            o->tsd.human.human_number = i;
            o->radar_image = 2;
        }
    }
}

struct game_obj_t *add_worm_tail(struct game_obj_t *parent, int coloroffset)
{
    int j;
    struct game_obj_t *o = NULL;

    if (parent->tsd.worm.parent != NULL && randomn(100) > 90)
        return NULL;

    o = add_generic_object(parent->x, parent->y, 0,0,
        worm_move, worm_draw, CYAN, NULL, 1, OBJ_TYPE_WORM, 1);
    if (o == NULL)
        return NULL;

    o->tsd.worm.parent = parent;
    o->counter = 0;
    o->radar_image = 1;
    o->tsd.worm.i = 0;
    o->tsd.worm.tx = o->x;
    o->tsd.worm.ty = o->y;
    o->tsd.worm.ltx = o->x;
    o->tsd.worm.lty = o->y;
    o->tsd.worm.coloroffset = coloroffset;
    for (j=0;j<NWORM_TIME_UNITS;j++) {
        o->tsd.worm.x[j] = o->x;
        o->tsd.worm.y[j] = o->y;
    }

    o->tsd.worm.child = add_worm_tail(o, coloroffset - 2);
    return o;
}

static void add_worms(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    int xi, i, j;
    struct game_obj_t *o;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        o = add_generic_object(t->x[xi], t->y[xi]-50, 0, 0, 
            worm_move, worm_draw, GREEN, NULL, 1, OBJ_TYPE_WORM, 1);
        if (o) {
            o->tsd.worm.parent = NULL;
            o->counter = 0;
            o->radar_image = 1;
            o->tsd.worm.i = 0;
            o->tsd.worm.tx = o->x;
            o->tsd.worm.ty = o->y;
            o->tsd.worm.ltx = o->x;
            o->tsd.worm.lty = o->y;
            o->tsd.worm.coloroffset = randomn(100)+100;
            for (j=0;j<NWORM_TIME_UNITS;j++) {
                o->tsd.worm.x[j] = o->x;
                o->tsd.worm.y[j] = o->y;
            }
            level.nworms++;
        }
        o->tsd.worm.child = add_worm_tail(o, o->tsd.worm.coloroffset - 2);
    }
}

void airship_gun_location(struct game_obj_t *parent, 
    __attribute__ ((unused)) struct game_obj_t *child,  int *x, int *y)
{
    *x = parent->x-60;
    *y = parent->y-5;
}

static void add_airships(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    int xi, i;
    struct game_obj_t *o;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        o = add_generic_object(t->x[xi], t->y[xi]-50, 0, 0, 
            airship_move, airship_draw, CYAN, &airship_vect, 1, OBJ_TYPE_AIRSHIP, 300*PLAYER_LASER_DAMAGE);
        if (o) {
            o->counter = 0;
            o->radar_image = 4;
            o->tsd.airship.bannerline = 0;
            o->tsd.airship.pressure = 0;
            o->gun_location = airship_gun_location;
            o->attached_gun = add_flak_gun(o->x, o->y, FAST_LASER, YELLOW, o); 
            o->attached_gun->tsd.kgun.invert = 1;
            o->attached_gun->v = &kgun_vect;
            level.nairships++;
        }
    }
}

static void add_gunwheels(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    int xi, i, y, towery;
    int inverted;
    struct game_obj_t *tower, *o, *left, *right;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);

        inverted = randomn(100) & 0x01;
        if (inverted) {
            towery = KERNEL_Y_BOUNDARY;
            y = KERNEL_Y_BOUNDARY + 250;
        } else {
            y = t->y[xi]-250;
            towery = t->y[xi];
        }

        tower = add_truss_tower(t->x[xi], towery, 5, !inverted, 14, RED, 100, 62, 25);

        if (tower == NULL)
            continue;

        o = add_generic_object(t->x[xi], y, 0, 0, 
            gunwheel_move, gunwheel_draw, RED, NULL, 1, OBJ_TYPE_GUNWHEEL, 1);
        if (o == NULL)
            continue;

        tower->tsd.truss.below = o;

        o->counter = 0;
        o->radar_image = 4;
        o->tsd.gunwheel.size = 180;
        o->tsd.gunwheel.attached_to = tower;
        o->tsd.gunwheel.inverted = inverted;
        o->tsd.gunwheel.rotation_multiplier = 4;
        o->gun_location = gunwheel_gun_location;

        left = add_generic_object(t->x[xi], t->y[xi]-250, 0, 0, 
            gunwheel_move, gunwheel_draw, RED, NULL, 1, OBJ_TYPE_GUNWHEEL, 1);
        o->tsd.gunwheel.left = left;
        if (left == NULL)
            continue;

        left->tsd.gunwheel.attached_to = o;
        left->tsd.gunwheel.size = 100;
        left->radar_image = 0;
        left->tsd.gunwheel.left = add_flak_gun(0,0, FAST_LASER, RED, left);
        left->tsd.gunwheel.right = add_flak_gun(0,0, FAST_LASER, RED, left);
        left->gun_location = gunwheel_gun_location;
        left->tsd.gunwheel.inverted = 0;
        left->tsd.gunwheel.rotation_multiplier = 8;

        right = add_generic_object(t->x[xi], t->y[xi]-250, 0, 0, 
            gunwheel_move, gunwheel_draw, RED, NULL, 1, OBJ_TYPE_GUNWHEEL, 1);
        o->tsd.gunwheel.right = right;
        if (right == NULL)
            continue;

        right->tsd.gunwheel.attached_to = o;
        right->tsd.gunwheel.size = 100;
        right->radar_image = 0;
        right->tsd.gunwheel.left = add_flak_gun(0,0, FAST_LASER, RED, right);
        right->tsd.gunwheel.right = add_flak_gun(0,0, FAST_LASER, RED, right);
        right->gun_location = gunwheel_gun_location;
        right->tsd.gunwheel.inverted = 0;
        right->tsd.gunwheel.rotation_multiplier = 8;
    }
}

static void add_socket(struct terrain_t *t)
{
    add_generic_object(t->x[TERRAIN_LENGTH-1] - 250, t->y[TERRAIN_LENGTH-1] - 250, 
        0, 0, socket_move, draw_generic, CYAN, &socket_vect, 0, OBJ_TYPE_SOCKET, 1);
}

static void add_balloons(struct terrain_t *t, struct level_obj_descriptor_entry *entry)
{
    struct game_obj_t *o;
    int xi, i;
    for (i=0;i<entry->nobjs;i++) {
        xi = initial_x_location(entry, i);
        o = add_generic_object(t->x[xi], t->y[xi]-50, 0, 0, 
            balloon_move, draw_generic, CYAN, &balloon_vect, 1, OBJ_TYPE_BALLOON, 1);
        if (o) {
            o->radar_image = 1;
            level.nballoons++;
        }
    }
}

static void draw_strings();
void setup_text();

int they_used_the_source()
{
#ifndef __WIN32__
    /*
     * A little joke for the programmers.  If *you* built it
     * from source less than 1 hour ago you get....
     * 
     * a million bonus points.  
         *
     * (Hey, the _very first_ thing the game tells you is 
     * "Uuuuse the Soooource!!!!"  It's not kidding. :-)
     *
     */

    struct timeval now;
    int uid;

    gettimeofday(&now, NULL);
    uid = 0;//getuid();

    return (uid == builder && 
        now.tv_sec - builtat > 0 && 
        now.tv_sec - builtat < 60*60);
#else
    /* FIXME: make this work on windows too. */
    return 0;
#endif
}

void sort_high_scores()
{
    struct score_data tmp;
    int i, sorted;

    /* simple bubble sort is good enough for 10 items. */
    do {
        sorted = 1;
        for (i=0;i<MAXHIGHSCORES-1;i++) {
            if (highscore[i].score < highscore[i+1].score) {
                sorted = 0;
                tmp = highscore[i];
                highscore[i] = highscore[i+1];
                highscore[i+1] = tmp;
            }
        }
    } while (!sorted);
}

int new_high_score(int newscore) 
{
    int i;
    for (i=0;i<MAXHIGHSCORES;i++)
        if (newscore > highscore[i].score) {
            /* printf("new_high_score returns new high score = %d\n", i);*/
            return i;
        }
    /* printf("new_high_score returns %d, bad score\n", MAXHIGHSCORES);*/
    return MAXHIGHSCORES;
}

void write_out_high_score_file();
void cancel_sound(int queue_entry);
static void draw_quit_screen();
static void do_newhighscore()
{
    int slot;
    static int highscore_timer = 0;
    static int finished = 0;
    static int finish_timer = 0;
    char message[20];
    int i, x, y;
    int timeleft;

    if (highscore_timer == 0) {
        highscore_timer = timer + 60 * frame_rate_hz;
        strcpy(highscore_initials, "   ");
        highscore_current_initial = 0;
        highscore_letter = 0;
        finished = 0;
        finish_timer = 0;
        wwviaudio_cancel_music();
        wwviaudio_play_music(HIGH_SCORE_MUSIC);
    }

    slot = new_high_score(game_state.score);
    if (slot >= MAXHIGHSCORES) {
        timer_event = GAME_ENDED_EVENT; 
        next_timer = timer + 1;
        printf("bug at %s:%d!\n", __FILE__, __LINE__);
        /* bug;*/
    }

    /* Draw the high score message */
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    rainbow_abs_xy_draw_string("New High Score!", BIG_FONT, 100, 70);
#ifndef OPENLASE
    rainbow_abs_xy_draw_string("New High Score!", BIG_FONT, 101, 71);
#endif
    if (!finished)
        rainbow_abs_xy_draw_string("Enter your initials:", SMALL_FONT, 240, 135);

    y = 200;
    /* Print the current set of initials */
    for (i=0;i<3;i++) {
        message[0] = highscore_initials[i];
        message[1] = '\0';
        x = 60 + ((i+3)%9)*80;
        rainbow_abs_xy_draw_string(message, BIG_FONT, x, y);
#ifndef OPENLASE
        rainbow_abs_xy_draw_string(message, BIG_FONT, x+1, y+1);
#endif
        wwvi_draw_line(x-25, y+25, x+55, y+25);
    }

    /* Print the alphabet. */
    if (!finished) {
        y = 200;
        strcpy(message, "A");   
        for (i=0;i<26;i++) {
            message[0] = 'A' + i;
            if ((i%9) == 0)
                y += 100;
            rainbow_abs_xy_draw_string(message, BIG_FONT, 60 + (i%9)*80, y);
#ifndef OPENLASE
            rainbow_abs_xy_draw_string(message, BIG_FONT, 1+60 + (i%9)*80, y+1);
#endif
            if (i == highscore_letter) {
                x = 60 + (i%9)*80;
                wwvi_draw_line(x-25, y-65, x-25, y+25);
                wwvi_draw_line(x-25, y-65, x+55, y-65);
                wwvi_draw_line(x-25, y+25, x+55, y+25);
                wwvi_draw_line(x+55, y-65, x+55, y+25);
            }
        }
        rainbow_abs_xy_draw_string("Done", SMALL_FONT, 60 + 8*80, 80+100*4);
    } else {
        int len;
        y = 300;
        sprintf(message, "%d", game_state.score);
        len = strlen(message);
        x = (SCREEN_WIDTH/2) - ((len * 2 * (font_scale[BIG_FONT] + letter_spacing[BIG_FONT])))/2;
        rainbow_abs_xy_draw_string(message, BIG_FONT, x, y);
#ifndef OPENLASE
        rainbow_abs_xy_draw_string(message, BIG_FONT, x+1, y+1);
#endif
    }

    if (highscore_letter == 26 && !finished) {
        x = 60 + 8*80;
        y = 80+100*4;
        wwvi_draw_line(x-25, y-65, x-25, y+25);
        wwvi_draw_line(x-25, y-65, x+55, y-65);
        wwvi_draw_line(x-25, y+25, x+55, y+25);
        wwvi_draw_line(x+55, y-65, x+55, y+25);
    }

    if (highscore_current_initial == 3 && finish_timer == 0) {
        /* player is finished entering initials. */
        highscore_initials[3] = '\0'; /* superfluous paranoia. */
        for (i=MAXHIGHSCORES-1; i > slot; i--)
            highscore[i] = highscore[i-1];
        strcpy(highscore[slot].name, highscore_initials);
        highscore[slot].score = game_state.score;
        finish_timer = timer + frame_rate_hz * 4;
        finished = 1;
        write_out_high_score_file();
    }

    if (timer > highscore_timer || (highscore_current_initial == 3 &&
            finish_timer != 0 && timer > finish_timer)) {
        /* get us out of here. */
        timer_event = GAME_ENDED_EVENT; 
        next_timer = timer + 1;
        highscore_timer = 0;
        wwviaudio_cancel_music();

        /* Prevent final button press from putting in a quarter. */
        next_quarter_time = timer + (frame_rate_hz);
    }

    if (finish_timer == 0) {
        timeleft = (highscore_timer - timer) / frame_rate_hz;
        sprintf(message, "Time remaining: %d", timeleft);
        rainbow_abs_xy_draw_string(message, SMALL_FONT, 60, 80+100*5);
    }
    if (in_the_process_of_quitting)
        draw_quit_screen();
}

struct intermission_star {
    float x, y, lx, ly, vx, vy;
};

void init_intermission_star(struct intermission_star *star)
{
    float dx, dy;
    star->x = (float) randomn(SCREEN_WIDTH);
    star->y = (float) randomn(SCREEN_HEIGHT);

    /* cheesy avoid divide by zero. */
    if (fabsf(star->x) < 0.00001)
        star->x += 0.003;
    if (fabsf(star->y) < 0.00001)
        star->y += 0.003;

    dx = star->x - (SCREEN_WIDTH/2);
    dy = star->y - (SCREEN_HEIGHT/2);
    if (fabsf(dx) > fabsf(dy)) {
        star->vx = dx / fabsf(dx);
        star->vy = dy / fabsf(dx); 
    } else {
        star->vx = dx / fabsf(dy);
        star->vy = dy / fabsf(dy); 
    }
    star->lx = star->x;
    star->ly = star->y;
}

void update_intermission_starfield()
{
#ifdef OPENLASE
#define INTERMISSION_STARS 20
#else
#define INTERMISSION_STARS 300
#endif
    static int initialized = 0;
    static struct intermission_star star[INTERMISSION_STARS];
    int x, y, x2, y2, i;
    if (!initialized) {
        for (i = 0; i < INTERMISSION_STARS; i++) {
            init_intermission_star(&star[i]);
        }       
        initialized = 1;
    }
    gdk_gc_set_foreground(gc, &huex[NCOLORS + NSPARKCOLORS + (((timer >> 2)) % NRAINBOWCOLORS)]);
    for (i = 0; i < INTERMISSION_STARS; i++) {
        star[i].lx = star[i].x;
        star[i].x += star[i].vx;
        star[i].ly = star[i].y;
        star[i].y += star[i].vy;
        if (star[i].x < 0 || star[i].x > SCREEN_WIDTH ||
            star[i].y < 0 || star[i].y > SCREEN_HEIGHT)
            init_intermission_star(&star[i]);
        star[i].vx *= 1.2;
        star[i].vy *= 1.2;
        x = (int) star[i].x;
        y = (int) star[i].y;
        x2 = (int) star[i].lx;
        y2 = (int) star[i].ly;
        wwvi_draw_line(x, y, x2, y2);
    }
}

int bonus_points_this_round;
static int do_intermission()
{
#define LAST_INTERMISSION_STAGE 12
    static int intermission_stage = 0;
    static int intermission_timer = 0;
    static int add_bonus;
    int bonus_points = 0;
    int inc_bonus = 0;
    char s[100];
    int elapsed_secs;

    /* stop other events, if any... */
    next_timer = 0;
    timer_event = START_INTERMISSION_EVENT; 
    
    if (intermission_timer == 0) {
        wwviaudio_play_music(INTERMISSION_MUSIC_SOUND);
    }

    if (timer >= intermission_timer) {
        wwviaudio_add_sound(LASER_EXPLOSION_SOUND);
        intermission_stage++;
        if (intermission_stage == LAST_INTERMISSION_STAGE-1
            && !they_used_the_source())
            intermission_stage++;
        /* printf("intermission_stage  %d -> %d\n", 
 *          intermission_stage-1,intermission_stage); */
        intermission_timer = timer + 
            (intermission_stage == LAST_INTERMISSION_STAGE ? 4 : 1) * 
            frame_rate_hz;
    }

    /* printf("timer=%d, timer_event=%d, intermission_timer=%d, stage=%d\n", 
        timer, timer_event, intermission_timer, intermission_stage); */

    update_intermission_starfield();
    cleartext();
    set_font(SMALL_FONT);
    switch (intermission_stage) {
        /* Cases are listed backwards to take advantage of fall-thru */
        case LAST_INTERMISSION_STAGE:
            timer_event = END_INTERMISSION_EVENT;
            next_timer = timer + 1;
            intermission_timer = 0;
            intermission_stage = 0;
            game_state.score += bonus_points_this_round;
            /* printf("added %d points to score --> %d\n", 
                bonus_points_this_round, game_state.score); */
            cleartext();
            gotoxy(0,0);
            break;
        case LAST_INTERMISSION_STAGE-1:
            if (they_used_the_source()) {
                gotoxy(5, LAST_INTERMISSION_STAGE-1+2);
                inc_bonus = 1000000; /* million bonus points */
                sprintf(s, "Using The Source                   %6d",
                    inc_bonus);
                bonus_points += inc_bonus;  
                add_bonus = bonus_points;
                gameprint(s);
            }
        case 10: 
            gotoxy(5, 10+2);
            elapsed_secs = game_state.finish_time.tv_sec - game_state.start_time.tv_sec;
            if (elapsed_secs < 90)
                inc_bonus = (90 - elapsed_secs) * 100;
            else
                inc_bonus = 0;
            sprintf(s, "Elapsed time               %02d:%02d    %5d",
                elapsed_secs / 60, elapsed_secs % 60, inc_bonus);
            bonus_points += inc_bonus;  
            add_bonus = bonus_points;
            gameprint(s);
        case 9: 
            gotoxy(5, 9+2);
            if (kill_tally[OBJ_TYPE_SAM_STATION] >= level.nsams)
                inc_bonus = 5000;
            else
                inc_bonus = 0;
            bonus_points += inc_bonus;
            sprintf(s, "SAM stations destroyed:    %2d/%2d    %5d",
                kill_tally[OBJ_TYPE_SAM_STATION], level.nsams, inc_bonus);
            bonus_points += inc_bonus;  
            gameprint(s);
        case 8: 
            gotoxy(5, 8+2);
            if (kill_tally[OBJ_TYPE_GUN] + kill_tally[OBJ_TYPE_KGUN] >= 
                    level.nflak + level.nkguns)
                inc_bonus = 5000;
            else
                inc_bonus = 0;
            sprintf(s, "Laser turrets killed:      %2d/%2d    %5d", 
                kill_tally[OBJ_TYPE_GUN] + kill_tally[OBJ_TYPE_KGUN], 
                    level.nflak + level.nkguns, inc_bonus);
            bonus_points += inc_bonus;  
            gameprint(s);
        case 7: 
            gotoxy(5, 7+2);
            sprintf(s, "Missiles killed:              %2d    %5d",
                kill_tally[OBJ_TYPE_MISSILE] + 
                kill_tally[OBJ_TYPE_HARPOON], 0);
            bonus_points += inc_bonus;  
            gameprint(s);
        case 6: 
            gotoxy(5, 6+2);
            if (kill_tally[OBJ_TYPE_ROCKET] >= level.nrockets)
                inc_bonus = 50;
            else
                inc_bonus = 0;
            sprintf(s, "Rockets killed:            %2d/%2d    %5d",
                kill_tally[OBJ_TYPE_ROCKET], level.nrockets, inc_bonus);
            bonus_points += inc_bonus;  
            gameprint(s);
        case 5: 
            gotoxy(5, 5+2);
            if (kill_tally[OBJ_TYPE_OCTOPUS] >= level.noctopi)
                inc_bonus = 50;
            else
                inc_bonus = 0;
            sprintf(s, "Octo-viruses killed:       %2d/%2d    %5d",
                kill_tally[OBJ_TYPE_OCTOPUS], level.noctopi, inc_bonus);
            bonus_points += inc_bonus;  
            gameprint(s);
        case 4: 
            gotoxy(5, 4+2);
            if (kill_tally[OBJ_TYPE_GDB] >= level.ngdbs)
                inc_bonus = 5000;
            else
                inc_bonus = 10*kill_tally[OBJ_TYPE_GDB];
            sprintf(s, "gdb processes killed:      %2d/%2d    %5d",
                kill_tally[OBJ_TYPE_GDB], level.ngdbs, inc_bonus);
            bonus_points += inc_bonus;  
            gameprint(s);
        case 3: 
            gotoxy(5, 3+2);
            inc_bonus = kill_tally[OBJ_TYPE_AIRSHIP] * 100;
            if (kill_tally[OBJ_TYPE_AIRSHIP] >= level.nairships)
                inc_bonus += 20000;
            sprintf(s, "Emacs processes killed:    %2d/%2d    %5d",
                kill_tally[OBJ_TYPE_AIRSHIP], level.nairships, inc_bonus);
            bonus_points += inc_bonus;  
            gameprint(s);
        case 2: 
            gotoxy(5, 2+2);
            inc_bonus = game_state.humanoids * 1000;
            if (game_state.humanoids == level.nhumanoids)
                inc_bonus = game_state.humanoids * 5000;
            sprintf(s, "vi .swp files rescued:     %2d/%2d    %5d", 
                game_state.humanoids, level.nhumanoids, inc_bonus);
            bonus_points += inc_bonus;  
            gameprint(s);
        case 1: 
            gotoxy(5, 1);
            sprintf(s, "Node cleared! Total bonus points:  %6d\n", bonus_points);
            bonus_points_this_round = bonus_points;
            gameprint( s);
    }
    if (intermission_stage != LAST_INTERMISSION_STAGE)
        draw_strings();
    else {
        game_state.score += add_bonus;
        printf("added %d points to score --> %d\n", add_bonus, game_state.score);
        setup_text();
    }
    /* printf("i, timer_event = %d\n", timer_event); */
    if (in_the_process_of_quitting)
        draw_quit_screen();
    return 0;
}

char spinner[] = "||||////----\\\\\\\\";
char radar_msg1[] = "  Sirius Cybernetics Corp. RADAR -- firmware v. 1.05 (bootleg)";
char radar_msg2[] = "  Fly Safe!!! Fly Siriusly Safe!!!";

static void draw_radar()
{
#ifndef OPENLASE_FUCK
    int xoffset, height, yoffset; 
    int x1, y1, x2, y2;
    char statusstr[100]; 

    
    
    
    int viewport_left, viewport_right, viewport_top, viewport_bottom;
    int y_correction;

    height = RADAR_HEIGHT;
    xoffset = RADAR_LEFT_MARGIN;
    yoffset = RADAR_YMARGIN;

    y2 = SCREEN_HEIGHT - yoffset;
    y1 = y2 - height;
    x1 = xoffset;
    x2 = SCREEN_WIDTH - RADAR_RIGHT_MARGIN;

    gdk_gc_set_foreground(gc, &huex[RED]);


    /* draw health bar */
    gdk_gc_set_foreground(gc, &huex[ORANGE]);
    if (game_state.health > 0)
        wwvi_draw_rectangle(TRUE, RADAR_LEFT_MARGIN, 600, ((SCREEN_WIDTH - RADAR_LEFT_MARGIN - RADAR_RIGHT_MARGIN) * game_state.health / game_state.max_player_health), - 5);
        
        
    if (credits <= 0) 
    {
      return;
    }
        
    /* draw radar border */
    wwvi_draw_line(x1, y1, x1, y2);
    wwvi_draw_line(x1, y2, x2, y2);
    wwvi_draw_line(x2, y2, x2, y1);
    wwvi_draw_line(x2, y1, x1, y1);
        
#if 1
    if (game_state.radar_state == RADAR_RUNNING) 
    {
        /* calculate viewport edges projected onto radar screen. */

        viewport_left = ((SCREEN_WIDTH - (RADAR_LEFT_MARGIN + RADAR_RIGHT_MARGIN)) * game_state.x) / WORLDWIDTH + RADAR_LEFT_MARGIN;
        viewport_right = ((SCREEN_WIDTH - (RADAR_LEFT_MARGIN + RADAR_RIGHT_MARGIN)) * (game_state.x + SCREEN_WIDTH)) / WORLDWIDTH + RADAR_LEFT_MARGIN;
        viewport_top =    SCREEN_HEIGHT - (RADAR_HEIGHT >> 1) - RADAR_YMARGIN + (((game_state.y - (SCREEN_HEIGHT/2)) * RADAR_HEIGHT) / 1500);
        viewport_bottom = SCREEN_HEIGHT - (RADAR_HEIGHT >> 1) - RADAR_YMARGIN + (((game_state.y + (SCREEN_HEIGHT/2)) * RADAR_HEIGHT) / 1500);

        y_correction = SCREEN_HEIGHT - (RADAR_HEIGHT >> 1) - RADAR_YMARGIN + ((player->y * RADAR_HEIGHT) / 1500);
        y_correction = (SCREEN_HEIGHT - (RADAR_HEIGHT >> 1)) - y_correction;
//y_correction=0;
//player "grid" in radar not correct

//radar does not show enemies anymore

//with current sizing???
        
        viewport_bottom += y_correction;
        viewport_top += y_correction;
/*
        if (viewport_top < y1) 
            viewport_top = y1;
        if (viewport_top > y2)
            viewport_top = y2;
        if (viewport_bottom < y1) 
            viewport_bottom = y1;
        if (viewport_bottom > y2)
            viewport_bottom = y2;
*/
        
int dif = viewport_bottom-viewport_top;
        if (viewport_top < y1) 
        {
            viewport_top = y1;
//            viewport_bottom = viewport_top+dif;
          
        }
        if (viewport_top > y2)
        {
            viewport_top = y2;
//            viewport_bottom = viewport_top+dif;
          
        }
        if (viewport_bottom < y1) 
        {
            viewport_bottom = y1;
//            viewport_top = viewport_bottom-dif;
        }
        
        if (viewport_bottom > y2)
        {
            viewport_bottom = y2;
//            viewport_top = viewport_bottom-dif;
        }        
        
        
        
        
        
        if (viewport_left < x1) 
            viewport_left = x1;
        if (viewport_left > x2)
            viewport_left = x2;
        if (viewport_right < x1) 
            viewport_right = x1;
        if (viewport_right > x2)
            viewport_right = x2;

        gdk_gc_set_foreground(gc, &huex[WHITE]);

        /* draw upper left corner */
        wwvi_draw_line(viewport_left, viewport_top, viewport_left, viewport_top + 5);
        wwvi_draw_line(viewport_left, viewport_top, viewport_left+5, viewport_top);

        /* draw upper right corner */
        wwvi_draw_line(viewport_right, viewport_top, viewport_right, viewport_top + 5);
        wwvi_draw_line(viewport_right, viewport_top, viewport_right-5, viewport_top);

        /* draw lower left corner */
        wwvi_draw_line(viewport_left, viewport_bottom, viewport_left, viewport_bottom - 5);
        wwvi_draw_line(viewport_left, viewport_bottom, viewport_left+5, viewport_bottom);

        /* draw lower right corner */
        wwvi_draw_line(viewport_right, viewport_bottom, viewport_right, viewport_bottom - 5);
        wwvi_draw_line(viewport_right, viewport_bottom, viewport_right-5, viewport_bottom);

    #if 0
        wwvi_draw_line(viewport_left, viewport_top, viewport_left, viewport_bottom);
        wwvi_draw_line(viewport_right, viewport_top, viewport_right, viewport_bottom);
        wwvi_draw_line(viewport_left, viewport_top, viewport_right, viewport_top);
        wwvi_draw_line(viewport_left, viewport_bottom, viewport_right, viewport_bottom);
    #endif
    }
#endif

    if (game_state.corrosive_atmosphere && game_state.radar_state == RADAR_RUNNING && credits > 0) 
    {
        gdk_gc_set_foreground(gc, &huex[GREEN]);
        abs_xy_draw_string("Corrosive atmosphere detected!  Vacate the area immediately!", TINY_FONT, x1 + 50, y1 + RADAR_HEIGHT/2);
    }
/*
    gdk_gc_set_foreground(gc, &huex[RED]);
    sprintf(statusstr, "Score:   %7d", game_state.score);
    abs_xy_draw_string(statusstr, TINY_FONT, x2 + 5, y1 + 7);
    sprintf(statusstr, "Bombs:   %7d", game_state.nbombs);
    abs_xy_draw_string(statusstr, TINY_FONT, x2 + 5, y1 + 21);
    sprintf(statusstr, "Lives:   %7d", game_state.lives);
    abs_xy_draw_string(statusstr, TINY_FONT, x2 + 5, y1 + 35);
    sprintf(statusstr, "Credits: %7d", credits);
    abs_xy_draw_string(statusstr, TINY_FONT, x2 + 5, y1 + 49);
    sprintf(statusstr, "Humans:      %d/%d", game_state.humanoids, level.nhumanoids);
    abs_xy_draw_string(statusstr, TINY_FONT, x2 + 5, y1 + 63);
*/
    if (game_pause) 
    {
        gdk_gc_set_foreground(gc, &huex[GREEN]);

        /* Remember Parsec on the ti99/4a? */
        abs_xy_draw_string("Time warp activated.", TINY_FONT, x1 + 50, y1 + RADAR_HEIGHT/2-10);
        return;
    }

    if (game_state.radar_state == RADAR_RUNNING) 
    {
        if ((timer & 0x04) == 0) 
        {
            if (game_state.missile_locked) 
            {
                gdk_gc_set_foreground(gc, &huex[RED]);
                abs_xy_draw_string("*MISSILE LOCK*", 
                    TINY_FONT, x1 + 210-18+80, y1 + RADAR_HEIGHT-10);
            }   
            if (credits <= 0) 
            {
                gdk_gc_set_foreground(gc, &huex[GREEN]);
                abs_xy_draw_string("Button 4 Start Game", 
                    TINY_FONT, x1 + 200+60, y1 + RADAR_HEIGHT-30);
            }
        }
        return;
    }

    if (game_state.radar_state == RADAR_FRITZED) 
    {
        gdk_gc_set_foreground(gc, &huex[randomn(NCOLORS+NSPARKCOLORS+NRAINBOWCOLORS)]);
        wwvi_draw_line(x1, y1 + timer % RADAR_HEIGHT, x2,  y1 + timer % RADAR_HEIGHT);
    } 
    else if (game_state.radar_state <= RADAR_BOOTUP) 
    { 
        /* radar is booting up, display bootup message. */
        gdk_gc_set_foreground(gc, &huex[GREEN]);
        radar_msg1[0] = spinner[timer % 16];
        abs_xy_draw_string(radar_msg1, TINY_FONT, x1 + 50, y1 + RADAR_HEIGHT/2-10);
        abs_xy_draw_string(radar_msg2, TINY_FONT, x1 + 80, y1 + RADAR_HEIGHT/2+10);
        game_state.radar_state --;
        if (game_state.radar_state == RADAR_RUNNING)
            wwviaudio_add_sound(RADAR_READY);
    }
#endif /* ndef OPENLASE */

  
  
}

/* call back for configure_event (for window resize) */
static gint main_da_configure()
{
    return TRUE;
}

static void draw_quit_screen()
{
    int x;
    static int quittimer = 0;

    quittimer++;

    gdk_gc_set_foreground(gc, &huex[BLACK]);
    wwvi_draw_rectangle(TRUE, 100, 100, SCREEN_WIDTH-200, SCREEN_HEIGHT-200);
    gdk_gc_set_foreground(gc, &huex[RED]);
    wwvi_draw_rectangle(FALSE, 100, 100, SCREEN_WIDTH-200, SCREEN_HEIGHT-200);

    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string("Quit?", BIG_FONT, 300, 280);

    if (current_quit_selection == 1) {
        x = 130;
        gdk_gc_set_foreground(gc, &huex[WHITE]);
    } else {
        x = 480;
        gdk_gc_set_foreground(gc, &huex[RED]);
    }
    abs_xy_draw_string("Quit Now", SMALL_FONT, 150, 450);
    if (current_quit_selection == 0)
        gdk_gc_set_foreground(gc, &huex[WHITE]);
    else
        gdk_gc_set_foreground(gc, &huex[RED]);
    abs_xy_draw_string("Don't Quit", SMALL_FONT, 500, 450);

    if ((quittimer & 0x04)) {
        gdk_gc_set_foreground(gc, &huex[WHITE]);
        wwvi_draw_rectangle(FALSE, x, 420, 200, 50);
    }
}



static void draw_help_screen();

/* This is the expose function of the main drawing area.  This is */
/* where everything gets drawn. */
static int main_da_expose()
{
    int i;
    int sx1, sx2;
    static int last_lowx = 0, last_highx = TERRAIN_LENGTH-1;
    /* int last_lowx = 0, last_highx = TERRAIN_LENGTH-1;*/
v_setCustomClipping(1, -12000, Y_LOW_CLIP, 12000, Y_HIGH_CLIP);

    if (timer_event == START_INTERMISSION_EVENT) 
    {
        openlase_startframe();
        do_intermission();
        openlase_renderframe();
        return 0;
    }

    if (timer_event == NEW_HIGH_SCORE_EVENT) {
        openlase_startframe();
        do_newhighscore();
        openlase_renderframe();
        return 0;
    }
    openlase_startframe();

    sx1 = game_state.x - 20;
    sx2 = game_state.x + SCREEN_WIDTH + 20;


    while (terrain.x[last_lowx] < sx1 && last_lowx+1 < TERRAIN_LENGTH)
        last_lowx++;
    while (terrain.x[last_lowx] > sx1 && last_lowx > 0)
        last_lowx--;
    while (terrain.x[last_highx] > sx2 && last_highx > 0)
        last_highx--;
    while (terrain.x[last_highx] < sx2 && last_highx+1 < TERRAIN_LENGTH) {
        last_highx++;
    }

    gdk_gc_set_foreground(gc, &huex[planet_color[level.ground_color]]);
    for (i=last_lowx;i<last_highx;i++) {
        wwvi_draw_line(terrain.x[i] - game_state.x, terrain.y[i]+(SCREEN_HEIGHT/2) - game_state.y,  terrain.x[i+1] - game_state.x, terrain.y[i+1]+(SCREEN_HEIGHT/2) - game_state.y);
    }
    gdk_gc_set_foreground(gc, &huex[RED]);

    /* draw "system memory boundaries" (ha!) */
    if (game_state.x > terrain.x[0] - SCREEN_WIDTH && game_state.x < terrain.x[0] + SCREEN_WIDTH)
        wwvi_draw_line(terrain.x[0] - game_state.x, 0, 
            terrain.x[0] - game_state.x, SCREEN_HEIGHT);
    if (game_state.x > terrain.x[TERRAIN_LENGTH - 1] - SCREEN_WIDTH &&
        game_state.x < terrain.x[TERRAIN_LENGTH - 1] + SCREEN_WIDTH)
        wwvi_draw_line(terrain.x[TERRAIN_LENGTH-1] - game_state.x, 0, 
             terrain.x[TERRAIN_LENGTH-1] - game_state.x, SCREEN_HEIGHT); 
    set_font(SMALL_FONT);
    if (game_state.y < KERNEL_Y_BOUNDARY + SCREEN_HEIGHT) 
    {
        wwvi_draw_line(0, KERNEL_Y_BOUNDARY  - game_state.y + SCREEN_HEIGHT/2, 
            SCREEN_WIDTH, KERNEL_Y_BOUNDARY - game_state.y + SCREEN_HEIGHT/2);
        livecursorx = (SCREEN_WIDTH - abs(game_state.x) % SCREEN_WIDTH);
        livecursory = KERNEL_Y_BOUNDARY - game_state.y + SCREEN_HEIGHT/2 - 10;
        draw_string((unsigned char *) "Kernel Space");
    }

    if (want_starfield)
        draw_stars();
    draw_objs();
    draw_strings();
v_setCustomClipping(0, -12000, Y_LOW_CLIP, 12000, Y_HIGH_CLIP);
    draw_radar();
v_setCustomClipping(1, -12000, Y_LOW_CLIP, 12000, Y_HIGH_CLIP);


    if (in_the_process_of_quitting)
        draw_quit_screen();

    if (game_pause_help)
        draw_help_screen();

    openlase_renderframe();
    return 0;
}
static void do_game_pause(void)
{
    if (game_pause) {
        game_pause = 0;
        wwviaudio_resume_audio();
    } else {
        game_pause = 1;
        wwviaudio_pause_audio();
        /* Queue a redraw, otherwise, we won't necessarily get a */
        /* redraw. Need this for the "Time warp activated."      */
        /* message to appear in the radar screen. */
//        gtk_widget_queue_draw(main_da);
    }
}

static void do_game_pause_help(void)
{
    if (game_pause_help) {
        game_pause_help = 0;
        game_pause = 0;
        wwviaudio_resume_audio();
    } else {
        game_pause_help = 1;
        game_pause = 1;
        wwviaudio_pause_audio();
        /* Queue a redraw, otherwise, we won't necessarily get a */
        /* redraw. Need this for the "Time warp activated."      */
        /* message to appear in the radar screen. */
//        gtk_widget_queue_draw(main_da);
    }
}

void game_ended();
void start_level();
void timer_expired()
{
    static int game_over_count = 0;

    /* printf("timer expired, %d\n", timer_event); */
    switch (timer_event) {
    case BLINK_EVENT:
        setup_text();
        if (credits >= 1) {
            game_over_count = 0;
            next_timer = timer + 1;
            timer_event = GAME_ENDED_EVENT;
            strcpy(textline[GAME_OVER].string, " Game Over");
            break;
        }
        game_over_count++;
        timer_event = BLANK_GAME_OVER_1_EVENT;
        next_timer = timer + 20;
        strcpy(textline[GAME_OVER].string, " Game Over");
        break;
    case BLANK_GAME_OVER_1_EVENT:
        timer_event = INSERT_COIN_EVENT;
        next_timer = timer + 20;
        strcpy(textline[GAME_OVER].string, "");
        break;
    case INSERT_COIN_EVENT:
        timer_event = BLANK_GAME_OVER_2_EVENT;
        next_timer = timer + 20;
        strcpy(textline[GAME_OVER].string, "Insert Coin");
        break;
    case BLANK_GAME_OVER_2_EVENT:
        if (game_over_count >= 3) {
            static int wherenext = 0;
            wherenext = (wherenext+1) % 4; 
            switch (wherenext) {
            case 0: timer_event = CREDITS1_EVENT;
                break;
            case 1: timer_event = INTRO1_EVENT;
                break;
            case 2: timer_event = START_HIGHSCORES_EVENT;
                break;
            case 3: 
            default: timer_event = KEYS1_EVENT;
                break;
            }
        } else
            timer_event = BLINK_EVENT;
        next_timer = timer + 20;
        strcpy(textline[GAME_OVER].string, "");
        break;
    case CREDITS1_EVENT: {
        int yline = 6;
        int x = 12;
        ntextlines = 1;
        set_font(SMALL_FONT);
        gotoxy(x,yline++);
        yline++;
        gotoxy(x,yline++);
        gameprint("Programming:   Stephen Cameron");
        gotoxy(x,yline++);
        gameprint("Game design:   Stephen Cameron");
        gotoxy(x,yline++);
        gameprint("Music:         Stephen Cameron,");
        gotoxy(x,yline++);
        gameprint("               Marty Kiel and");
        gotoxy(x,yline++);
        gameprint("               Skip Kavanaugh");
        gotoxy(x,yline++);
        gameprint("Sound effects: Stephen Cameron.");
        gotoxy(x,yline++);
        gotoxy(x,yline++);
        timer_event = CREDITS2_EVENT;
        next_timer = timer + 100;
        game_over_count = 0;
        break;
        }
    case CREDITS2_EVENT:
        ntextlines = 1;
        setup_text();
        timer_event = BLINK_EVENT;
        timer_event = BLANK_GAME_OVER_1_EVENT;
        next_timer = timer + 1;
        break;

    case INTRO1_EVENT: {
        int yline = 5;
        int x = 12;
        ntextlines = 1;

        set_font(SMALL_FONT);
        gotoxy(x,yline++);
        gameprint("In the beginning, there was ed."); gotoxy(x,yline++);
        gameprint("Ed is the standard text editor."); gotoxy(x,yline++);
        gameprint("Then there was vi, and it was good."); gotoxy(x,yline++);
        gameprint("Then came emacs, and disharmony."); gotoxy(x,yline++);
        gameprint("Your mission is to traverse core"); gotoxy(x,yline++);
        gameprint("memory and rid the host of emacs."); gotoxy(x,yline++);
        gameprint("It will not be an easy mission, as"); gotoxy(x,yline++);
        gameprint("there are many emacs friendly"); gotoxy(x, yline++);
        gameprint("processes."); gotoxy(x,yline++);

        timer_event = INTRO2_EVENT;
        next_timer = timer + 8 * frame_rate_hz;
        game_over_count = 0;
        break;
        }
    case INTRO2_EVENT: {
        ntextlines = 1;
        setup_text();
        timer_event = BLINK_EVENT;
        timer_event = BLANK_GAME_OVER_1_EVENT;
        /* timer_event = KEYS1_EVENT;*/
        next_timer = timer + 1;
        break;
        }
    case START_HIGHSCORES_EVENT: {
        int yline = 2;
        int x = 17;
        int h;
        char score[20];
        ntextlines = 1;
        set_font(SMALL_FONT);
        gotoxy(x+4,1);
        gameprint("High Scores");
        for (h = 0; h<MAXHIGHSCORES; h++) {
            gotoxy(x,yline + h);
            sprintf(score, "%c%c%c   %12d", 
                highscore[h].name[0],
                highscore[h].name[1],
                highscore[h].name[2],
                highscore[h].score);
            gameprint(score);
        }
        timer_event = END_HIGHSCORES_EVENT;
        next_timer = timer + 8 * frame_rate_hz;
        game_over_count = 0;
        break;
        }
    case END_HIGHSCORES_EVENT: {
        ntextlines = 1;
        setup_text();
        timer_event = BLINK_EVENT;
        timer_event = BLANK_GAME_OVER_1_EVENT;
        /* timer_event = KEYS1_EVENT;*/
        next_timer = timer + 1;
        break;
        }
    case KEYS1_EVENT: {
        int yline = 2;
        int x = 18;
        ntextlines = 1;
        set_font(SMALL_FONT);
        gotoxy(x,yline++);
        gameprint("Controls:"); yline++; gotoxy(x,yline++);
        gameprint("h  move left"); gotoxy(x,yline++);
        gameprint("l  move right"); gotoxy(x,yline++);
        gameprint("k  move up"); gotoxy(x,yline++);
        gameprint("j  move down"); gotoxy(x,yline++);
        gameprint("z  fires laser"); gotoxy(x,yline++);
        gameprint("b  drops bomb"); gotoxy(x,yline++);
        gameprint("c  drops chaff"); gotoxy(x,yline++);
        yline++;
        gameprint("Q  Insert Quarter"); gotoxy(x-8, yline++);
        gameprint("Esc to Exit -- F1 for Help"); gotoxy(x,yline++);
        next_timer = timer + 8 * frame_rate_hz;
        game_over_count = 0;
        timer_event = KEYS2_EVENT;
        break;
        }
    case KEYS2_EVENT: {
        ntextlines = 1;
        setup_text();
        timer_event = BLANK_GAME_OVER_1_EVENT;
        next_timer = timer + 1;
        break;
        }
    case READY_EVENT:
        /* This whole text thing needs major revamping... ugly as hell. */
        set_font(BIG_FONT);
        start_level();
        wwviaudio_play_music(MUSIC_SOUND);
        strcpy(textline[CREDITS].string, "");
        sprintf(textline[GAME_OVER].string, "Level %d...", 
            level.level_number);
        textline[GAME_OVER].font = BIG_FONT;
        textline[GAME_OVER].x = (4+1) * font_scale[current_font]*3;
        textline[GAME_OVER].y = (3+1) * font_scale[current_font]*7;
        gettimeofday(&game_state.start_time, NULL);
        next_timer += frame_rate_hz;
        timer_event = SET_EVENT;
        ntextlines = 2;
        game_state.x = 0;
        game_state.y = 0;
        game_state.vy = 0;
        game_state.vx = 0;
        break;
    case SET_EVENT:
        sprintf(textline[GAME_OVER].string, "%s", 
            leveld[level.level_number]->level_name);
        /* strcpy(textline[GAME_OVER].string, "Ready,Set..."); */
        ntextlines = 2;
        next_timer += frame_rate_hz;
        timer_event = GO_EVENT;
        break;
    case GO_EVENT:
        if (!xmas_mode)
            strcpy(textline[GAME_OVER].string, "Prepare to die!");
        else
            strcpy(textline[GAME_OVER].string, "Go Santa, Go!");
        next_timer += frame_rate_hz;
        timer_event = BLANK_EVENT;
        break;
    case BLANK_EVENT:
        ntextlines = 1;
        game_state.vx = PLAYER_SPEED;
        break;
    case GAME_ENDED_EVENT:
        timer_event = GAME_ENDED_EVENT_2;
        next_timer = timer + frame_rate_hz;
        if (credits <= 0) {
            setup_text(); /* <--- sets next_timer, timer_event */
            timer_event = GAME_ENDED_EVENT_2;
            next_timer = timer + frame_rate_hz;
            strcpy(textline[GAME_OVER+1].string, FINAL_MSG1);
            strcpy(textline[GAME_OVER].string, FINAL_MSG2);
            ntextlines = 4;
            next_timer = timer + 4*frame_rate_hz;
        } else {
            strcpy(textline[GAME_OVER].string, "");
            timer_event = GAME_ENDED_EVENT_2;
            ntextlines = 2;
        }
        break;
    case GAME_ENDED_EVENT_2:
        next_timer = timer + 1;
        if (credits <= 0) {
            game_ended();
            start_level();
            timer_event = BLINK_EVENT;
            strcpy(textline[GAME_OVER+1].string, FINAL_MSG1);
            strcpy(textline[GAME_OVER].string, FINAL_MSG2);
            ntextlines = 4;
            next_timer = timer + 2*frame_rate_hz;
        } else {
            timer_event = READY_EVENT; 
            ntextlines = 2;
        }
        game_ended();
        break;
    case START_INTERMISSION_EVENT:
        /* drawing area expose event handler handles this directly */
        break;
    case NEW_HIGH_SCORE_PRE_EVENT:
        if (credits <= 0) {
            setup_text();
            strcpy(textline[GAME_OVER+1].string, "NEW HIGH");
            strcpy(textline[GAME_OVER].string, "SCORE!!!");
            ntextlines = 4;
        } else {
            strcpy(textline[GAME_OVER].string, "");
            ntextlines = 2;
        }
        timer_event = NEW_HIGH_SCORE_PRE2_EVENT; /* undo some of what setup_text() did. */
        next_timer = timer + frame_rate_hz*3;
        break;
    case NEW_HIGH_SCORE_PRE2_EVENT:
        /* This just transitions to NEW_HIGH_SCORE_EVENT */
        /* Need this state because advance game and joystick directly */
        /* trigger off timer_event value. */
        timer_event = NEW_HIGH_SCORE_EVENT;
        next_timer = timer;
        break;
    case NEW_HIGH_SCORE_EVENT:
        /* drawing area expose event handler handles this directly */
        break;
    case END_INTERMISSION_EVENT:
        advance_level();
        timer_event = READY_EVENT;
        ntextlines = 2;
        next_timer = timer+1;
        break;
    default: 
        break;
    }
}

void really_quit()
{
    gettimeofday(&end_time, NULL);
    printf("%d frames / %d seconds, %g frames/sec\n", 
        nframes, (int) (end_time.tv_sec - start_time.tv_sec),
        (0.0 + nframes) / (0.0 + end_time.tv_sec - start_time.tv_sec));
    printf("Total lines = %lu, lines/sec = %f\n", total_line_count,
             (double) total_line_count /
            (double) end_time.tv_sec - start_time.tv_sec);
}

void deal_with_joystick();
void deal_with_keyboard();

gint advance_game()
{
    int i;


    if (game_pause == 1) {
        if (game_pause_help) {
            deal_with_keyboard();
        }
        return TRUE;
    }

    if (jsfd >= 0)
        deal_with_joystick();

    deal_with_keyboard();

    if (in_the_process_of_quitting) {
        really_quit();
        return TRUE;
    }

    game_state.x += game_state.vx;
    game_state.y += game_state.vy; 


    timer++;
    if (timer == next_timer)
        timer_expired();

    if (timer_event == END_INTERMISSION_EVENT)
        return TRUE;

    game_state.missile_locked = 0;
    if (timer_event != START_INTERMISSION_EVENT && 
        timer_event != NEW_HIGH_SCORE_EVENT) {
        int temp_highest_obj = 0;
        for (i=0;i<=highest_object_number;i++) {
            if (game_state.go[i].alive) {
                if (game_state.go[i].move != NULL)
                    game_state.go[i].move(&game_state.go[i]);
                temp_highest_obj = i;   
            }
            /* if (game_state.go[i].alive && game_state.go[i].move == NULL)*/
                /* printf("NULL MOVE!\n");*/
        }
        highest_object_number = temp_highest_obj;
        if (game_state.missile_locked && timer % 10 == 0 && want_missile_alarm)
            wwviaudio_add_sound(MISSILE_LOCK_SIREN_SOUND);
    }
    nframes++;
#if 0
    if (WORLDWIDTH - game_state.x < 100)
        return FALSE;
    else
        return TRUE;
#endif
    return TRUE;
}

void setup_text()
{
    cleartext();
    gotoxy(4,1);
    gameprint("");
    set_font(BIG_FONT);
    gotoxy(4,3);
    gameprint(" Game Over\n");
    gotoxy(4,2);
    gameprint("Word War vi\n");
    set_font(SMALL_FONT);
    gotoxy(15,13);
    gameprint("(c) 2007 Stephen Cameron\n");
    timer_event = BLINK_EVENT;
    next_timer = timer + 30;
#if 0
    gotoxy(1,6);
    gameprint("abcdefghijklmn");
    gotoxy(1,7);
    gameprint("opqrstuvwxyz");
    gotoxy(1,8);
    gameprint("0123456789,.+-");
#endif
}

void initialize_game_state_new_level()
{
    game_state.lives = 3;
    game_state.humanoids = 0;
    game_state.prev_score = 0;
    game_state.health = game_state.max_player_health;
    game_state.nbombs = leveld[level.level_number]->nbombs;
    game_state.ngbombs = leveld[level.level_number]->ngbombs;
    game_state.prev_bombs = -1;
    init_kill_tally();
    game_state.cmd_multiplier = 1;
    game_state.radar_state = RADAR_BOOTUP;
    game_state.nextlasertime = timer;
    game_state.nextlasercolor = NCOLORS + NSPARKCOLORS;
    game_state.nextbombtime = timer;
    game_state.nextchafftime = timer;
    game_state.key_up_pressed = 0;
    game_state.key_down_pressed = 0;
    game_state.key_left_pressed = 0;
    game_state.key_right_pressed = 0;
    game_state.key_bomb_pressed = 0;
    game_state.key_present_pressed = 0;
    game_state.key_gbomb_pressed = 0;
    game_state.key_laser_pressed = 0;
    game_state.key_chaff_pressed = 0;
    game_state.houses_gifted = 0;
}

void start_level()
{
    int i;
    struct level_obj_descriptor_entry *objdesc;

    if (timer != 0)
        free_buildings();
    for (i=0;i<MAXOBJS;i++) {
        game_state.go[i].alive = 0;
        game_state.go[i].vx = 0;
        game_state.go[i].vy = 0;
        game_state.go[i].move = move_obj;
    }
    memset(&game_state.go[0], 0, sizeof(game_state.go));
    target_head = NULL;
    memset(free_obj_bitmap, 0, sizeof(int) * NBITBLOCKS);
    init_object_numbers();
    init_stars();
    free_obj_bitmap[0] = 0x01;  
    game_state.humanoids = 0;
    game_state.direction = 1;
    player = &game_state.go[0];
    if (xmas_mode)
        player->draw = xmas_player_draw;
    else
        player->draw = player_draw;
    player->move = player_move;
    player->v = (game_state.direction == 1) ? &player_vect : &left_player_vect;
    player->x = 200;
    player->y = -100;
    player->vx = PLAYER_SPEED;
    player->vy = 0;
    add_target(player);
    player->alive = 1;
    player->bullseye = NULL;
    if (xmas_mode)
        player->color = ORANGE;
    else
        player->color = WHITE;
    
    player->destroy = generic_destroy_func;
    player->tsd.epd.count = -1;
    player->tsd.epd.count2 = 50;
    player->radar_image = 2;
    player->otype = OBJ_TYPE_PLAYER;
    game_state.health = game_state.max_player_health;
    game_state.nbombs = leveld[level.level_number]->nbombs;
    game_state.ngbombs = leveld[level.level_number]->ngbombs;
    game_state.prev_bombs = -1;
    game_state.nobjs = MAXOBJS-1;
    game_state.x = 0;
    game_state.y = 0;
    game_state.radar_state = RADAR_BOOTUP;
    game_state.houses_gifted = 0;

    add_reindeer();

    srandom(level.random_seed);
    generate_terrain(&terrain);

    add_buildings(&terrain);/* Some FreeBSD users report that */
                /*add_buildings() causes crashes. */
                /* Commenting this out on FreeBSD */
                /* may help, but, no buildings. */
                /* I've looked at the code, but */
                /* don't see anything wrong with it. */
    add_humanoids(&terrain);
    add_bridges(&terrain);
    add_socket(&terrain);

    level.nrockets = 0;
    level.nbigrockets = 0;
    level.njets = 0;
    level.nflak = 0;
    level.nfueltanks = 0;
    level.njammers = 0;
    level.ncron = 0;
    level.nships = 0;
    level.ngdbs = 0;
    level.noctopi = 0;
    level.nballoons = 0;
    level.ntentacles = 0;
    level.nsams = 0;
    level.nairships = 0;
    level.nworms = 0;
    level.nkguns = 0;

    objdesc = leveld[level.level_number]->objdesc;

    for (i=0;i<leveld[level.level_number]->nobj_desc_entries;i++) {
        switch (objdesc[i].obj_type) {
        case OBJ_TYPE_ROCKET: 
            add_rockets(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_BIG_ROCKET: 
            add_big_rockets(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_JET:
            add_jets(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_FUEL:
            add_fuel(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_JAMMER:
            add_jammers(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_CRON:
            add_cron(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_SHIP:
            add_ships(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_SAM_STATION:
            add_SAMs(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_GUN:
            add_flak_guns(&terrain, &objdesc[i], 
                leveld[level.level_number]->laser_velocity_factor);
            break;
        case OBJ_TYPE_KGUN:
            add_kernel_guns(&terrain, &objdesc[i],
                leveld[level.level_number]->laser_velocity_factor);
            break;
        case OBJ_TYPE_TESLA:
            add_tesla_towers(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_AIRSHIP:
            add_airships(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_WORM:
            add_worms(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_BALLOON:
            add_balloons(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_GDB:
            add_gdbs(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_OCTOPUS:
            add_octopi(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_TENTACLE:
            add_tentacles(&terrain, &objdesc[i]);
            break;
        case OBJ_TYPE_GUNWHEEL:
            add_gunwheels(&terrain, &objdesc[i]);
            break;
        }
    }
    add_houses(&terrain, NULL);

    if (credits == 0)
        setup_text();
    wwviaudio_add_sound(USETHESOURCE_SOUND);
    /* printf("use the force.\n"); */

}

void init_levels_to_beginning()
{
    int i;
    if (credits <= 0) {
        level.random_seed = random();
        level.level_number = randomn((ARRAY_SIZE(leveld))-1);
    } else {
        level.level_number = 0;
        level.random_seed = initial_random_seed;
    }

    level.nbridges = NBRIDGES;
    level.nbuildings = NBUILDINGS;
    level.nhumanoids = NHUMANOIDS;

    level.kgun_health = leveld[level.level_number]->max_kgun_health;
    level.laser_fire_chance = leveld[level.level_number]->laser_fire_chance;
    level.jetpilot_firechance = leveld[level.level_number]->jetpilot_firechance;
    level.large_scale_roughness = leveld[level.level_number]->large_scale_roughness;
    level.small_scale_roughness = leveld[level.level_number]->small_scale_roughness;

#ifdef LEVELWARP
    for (i=0;i<levelwarp;i++)
        advance_level();
#endif
}

void game_ended()
{
    init_levels_to_beginning();
    initialize_game_state_new_level();
    game_state.score = 0;
    /* start_level(); */
}

void advance_level()
{
    srandom(level.random_seed);
    level.random_seed = random(); /* deterministic */

    level.level_number++;
    if (leveld[level.level_number] == NULL)
        level.level_number--;

    if (!xmas_mode)
        level.ground_color = (level.ground_color + 1) %
            ARRAY_SIZE(planet_color);
    else
        level.ground_color = 6;  /* white */

    level.large_scale_roughness = leveld[level.level_number]->large_scale_roughness;
    level.small_scale_roughness = leveld[level.level_number]->small_scale_roughness;
    initialize_game_state_new_level();
}


void insert_quarter()
{
    credits++;
    autopilot_mode = 0;
    if (credits == 1) {
        wwviaudio_cancel_all_sounds();
        destiny_facedown = 0;
        game_state.sound_effects_on = 1;
        wwviaudio_resume_sound_effects();
        wwviaudio_resume_music();
        wwviaudio_add_sound(INSERT_COIN_SOUND);
        sleep(2);
        ntextlines = 1;
        game_ended();
        /* initialize_game_state_new_level();
        init_levels_to_beginning();
        start_level(); */
        init_levels_to_beginning();
        timer_event = READY_EVENT;
        next_timer = timer+1;
    } else
        wwviaudio_add_sound(INSERT_COIN_SOUND);
    strcpy(textline[CREDITS].string, "");
}

/* this is just a table to map joystick positions to numbers.  Notice there are more
 * lower numbers than higher numbers.  The idea is motion is finer in the middle, and
 * coarser at the edges.  I just pulled these numbers out of my ass, really.
 *
 * The -32767 to 0 range, and the 0 to 32767 range are mapped onto this array,
 * linearly, then the values in the array are used -- mapping it to nonlinear values.
 * This is used for the y axis.  
 *
 * The x axis of the joystick mostly has a big dead zone in the center, with
 * only things happening at the extreme edges of the stick's travel.
 *
 * Not sure that's the best way, but it seems to be sort of working ok,
 * which my previous schemes weren't really ok.
 */
int yjstable[] = { 1,1,1,1,1,1,1,1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,  3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10,
    11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 
    19, 20, 21, 22, 23, 24, 25 };
int nyjstable = ARRAY_SIZE(yjstable);

enum keyaction { keynone, keydown, keyup, keyleft, keyright, 
        keylaser, keybomb, keychaff, keygravitybomb,
        keyquarter, keypause, key2, key3, key4, key5, key6,
        key7, key8, keysuicide, keyfullscreen, keythrust, 
        keysoundeffects, keymusic, keyquit, keytogglemissilealarm,
        keypausehelp, keyreverse, key_droppresent
};

char *keyactionstring[] = {
    "none", "down", "up", "left", "right", 
    "laser", "bomb", "chaff", "gravitybomb",
    "quarter", "pause", "2x", "3x", "4x", "5x", "6x",
    "7x", "8x", "suicide", "fullscreen", "thrust", 
    "soundeffect", "music", "quit", "missilealarm", "help", "reverse", "gift"
};

enum keyaction jsbuttonaction[11] = {
        /* default joystick button assignments. */
        key_droppresent, /* button 0 */
        keybomb,     /* button 1 */
        keybomb,     /* button 2 */
        keylaser,    /* button 3 */
        keylaser,    /* button 4 */
        keybomb,     /* button 5 */
        keylaser,    /* button 6 */
        keylaser,    /* button 7 */
        /* keyquarter,*/ /* button 8 */ /* <-- This is no good on xbox360 controller */
        keylaser,    /* button 8 */
        keygravitybomb,  /* button 9 */
        keylaser,    /* button 10 */
    };

/* 
void deal_with_joystick()
{
  // todo handle joystick!
  
    int rc;
    // why can I get away with this kind of initializer 
    // in the linux kernel, but not here?  Well, not without the 
    // compiler moaning, anyway. 
    static struct wwvi_js_event jse; 
    int index = 0;
    int newvy, diff;
    static int next_joystick_motion_timer = 0;
    static int next_joystick_button_timer = 0;
    int  i;
    int moved;

    int do_bomb = 0;
    int do_droppresent = 0;
    int do_quarter = 0;
    int do_laser = 0;
    int do_chaff = 0;
    int do_gbomb = 0;
    int do_thrust = 0;
    int do_reverse = 0;
    int do_suicide = 0;

    int *xaxis = &jse.stick_x;
    int *yaxis = &jse.stick_y;

    xaxis = &jse.stick_x;
    yaxis = &jse.stick_y;

#define JOYSTICK_SENSITIVITY 5000
#define XJOYSTICK_THRESHOLD 20000

    memset(&jse.button[0], 0, sizeof(jse.button[0]*10));
    rc = get_joystick_status(&jse);
    if (rc != 0) return;

    if (in_the_process_of_quitting) 
    {
        if (*xaxis < -XJOYSTICK_THRESHOLD) 
        {
            last_user_input_time = timer;
            current_quit_selection = 1;
        } 
        else if (*xaxis > XJOYSTICK_THRESHOLD) 
        {
                last_user_input_time = timer;
                current_quit_selection = 0;
        }
        for (i=0;i<10;i++)
        {
          
            if (jse.button[i] == 1 && jsbuttonaction[i] == keylaser) 
            {
                final_quit_selection = current_quit_selection;
                if (!final_quit_selection) 
                {
                    in_the_process_of_quitting = 0;
                    last_user_input_time = timer;
                    wwviaudio_resume_audio();
                    // prevent just selecting "don't quit" from 
                    // putting in a quarter. 
                    next_quarter_time = timer + frame_rate_hz;
                }
            }
        }
        return;
    }

    if (game_pause_help) 
    {
        return;
    }

    highscore_buttonpress = 0;
    
    if (timer_event == NEW_HIGH_SCORE_EVENT) 
    {
        moved = 0;
    
        // user is entering high scores. 
        if (timer > next_joystick_motion_timer) 
        {
            next_joystick_motion_timer = timer + (frame_rate_hz >> 3);
            if (*xaxis < -XJOYSTICK_THRESHOLD) 
            {
                highscore_letter--;
                if (highscore_letter < 0)
                    highscore_letter = 26;
                moved = 1;
            }
            if (*xaxis > XJOYSTICK_THRESHOLD) 
            {
                highscore_letter++;
                if (highscore_letter > 26)
                    highscore_letter = 0;
                moved = 1;
            }
            if (*yaxis < -XJOYSTICK_THRESHOLD) 
            {
                highscore_letter -= 9;
                if (highscore_letter < 0)
                    highscore_letter += 27;
                moved = 1;
            }
            if (*yaxis > XJOYSTICK_THRESHOLD) 
            {
                highscore_letter += 9;
                if (highscore_letter > 26)
                    highscore_letter -= 27;
                moved = 1;
            }
        }
        if (timer > next_joystick_button_timer) 
        {
            next_joystick_button_timer = timer + (frame_rate_hz >> 3);
            for (i=0;i<10;i++) 
            {
                if (jse.button[i] && jsbuttonaction[i] == keylaser) 
                {
                    highscore_buttonpress = 1;
                    if (highscore_letter >= 0 && highscore_letter < 26) 
                    {
                        highscore_initials[highscore_current_initial] = highscore_letter + 'A';
                        highscore_current_initial++;
                    } else if (highscore_letter == 26) { // done. 
                        highscore_current_initial = 3; // ends it. 
                    }
                    moved = 1;
                }
                if (jse.button[i] && jsbuttonaction[i] == keyquit) 
                {
                    in_the_process_of_quitting = !in_the_process_of_quitting;
                    if (in_the_process_of_quitting)
                        wwviaudio_pause_audio();
                    else
                        wwviaudio_resume_audio();
                }
            }
        }
        if (moved) 
        {
            last_user_input_time = timer; 
            wwviaudio_add_sound(PLAYER_LASER_SOUND);
        }
        return;
    }

    if (game_state.health <= 0 && credits >= 1) 
    {
        return;
    }

    if (credits <= 0) {
        if (*xaxis < -XJOYSTICK_THRESHOLD ||
            *xaxis > XJOYSTICK_THRESHOLD ||
            *yaxis < -XJOYSTICK_THRESHOLD ||
            *yaxis > XJOYSTICK_THRESHOLD)
            last_user_input_time = timer; 
        for (i=0;i<10;i++)
            if (jse.button[i])
                last_user_input_time = timer; 
        goto no_credits;
    }

    // xaxis, horizontal movement
    if (*xaxis < -XJOYSTICK_THRESHOLD) 
    {
        if (game_state.direction != -1) 
        {
            player->vx = player->vx / 2;
            game_state.direction = -1;
            set_player_vect();
        } 
        else if (abs(player->vx + game_state.direction) < MAX_VX)
            player->vx += game_state.direction;
        
        last_user_input_time = timer; 
    } 
    else if (*xaxis > XJOYSTICK_THRESHOLD) 
    {
        if (game_state.direction != 1) 
        {
            player->vx = player->vx / 2;
            game_state.direction = 1;
            set_player_vect();
        } 
        else if (abs(player->vx + game_state.direction) < MAX_VX)
            player->vx += game_state.direction;
        last_user_input_time = timer; 
    } 
    else 
    {
        ;
    }

    // yaxis vertical movement 
    if (*yaxis > JOYSTICK_SENSITIVITY) 
    {
        if (player->vy < MAX_VY)
            index = nyjstable * *yaxis / 32767;
        if (index < 0)
            index = 0;
        else if (index > nyjstable-1)
            index = nyjstable-1;
        newvy = yjstable[index];
        diff = newvy - player->vy;
        if (abs(diff) > 4)
            player->vy = player->vy + (diff >> 1);
        else 
            player->vy = player->vy + 2;
        last_user_input_time = timer; 
    } 
    else if (*yaxis < -JOYSTICK_SENSITIVITY) 
    {
        if (player->vy > -MAX_VY)
            index = -nyjstable * *yaxis / 32767;
        if (index < 0)
            index = 0;
        else if (index > nyjstable-1)
            index = nyjstable-1;
        newvy = -yjstable[index];
        diff = newvy - player->vy;
        if (abs(diff) > 4)
            player->vy = player->vy + (diff >> 1);
        else 
            player->vy = player->vy - 2;
        last_user_input_time = timer; 
    } 
    else 
    {
        if (player->vy > 0)
            player->vy--;
        else if (player->vy < 0)
            player->vy++;
    }

    for (i=0;i<11;i++) 
    {
        if (jse.button[i] == 1) 
        {
            last_user_input_time = timer; 
            switch(jsbuttonaction[i]) 
            {
              case keysuicide:
                  do_suicide = 1;
              case keylaser:
                  do_laser = 1;
                  break;
              case keybomb:
                  do_bomb = 1;
                  break;
              case key_droppresent:
                  do_droppresent = 1;
                  break;
              case keypause:
                  break;
              case keypausehelp:
                  break;
              case keychaff:
                  do_chaff = 1;
                  break;
              case keygravitybomb:
                  do_gbomb = 1;
                  break;
              case keyquarter:
                  do_quarter = 1;
                  break;
              case keythrust:
                  do_thrust = 1;
                  break;
              case keyquit:
                  in_the_process_of_quitting = !in_the_process_of_quitting;
                  if (in_the_process_of_quitting)
                      wwviaudio_pause_audio();
                  else
                      wwviaudio_resume_audio();
                  break;
              case keytogglemissilealarm:
                  want_missile_alarm = !want_missile_alarm;
                  break;
              case keymusic:
                  wwviaudio_toggle_music();
                  break;
              case keyreverse:
                  do_reverse = 1;
                  break;
              case keysoundeffects:
                  game_state.sound_effects_on = !game_state.sound_effects_on;
                  game_state.sound_effects_manually_turned_off = (!game_state.sound_effects_on);
                  wwviaudio_toggle_sound_effects();
                  break;
              default:
                  break;
            }
        }
    }
            
    // Buttons... 
    if (do_bomb)
        drop_bomb();

    if (do_droppresent && xmas_mode)
        drop_present();

    if (do_laser)
        player_fire_laser();

    if (do_chaff)
        drop_chaff();

    if (do_gbomb)
        drop_gravity_bomb();

    if (do_reverse) 
    {
        if (timer > next_joystick_button_timer) 
        {
            game_state.direction = -game_state.direction;
            set_player_vect();
            next_joystick_button_timer = timer + frame_rate_hz / 8;
        }
    }

    if (do_thrust)
        if (abs(player->vx + game_state.direction) < MAX_VX)
            player->vx += game_state.direction;

    if ((do_quarter) && timer > next_quarter_time) {
        insert_quarter();
        next_quarter_time = timer + (frame_rate_hz);
    }

    if (do_suicide) {
        if (credits > 0)
            game_state.health = -1;
    }
    return;

no_credits:

    // check to see if quit button pressed. 
    for (i=0;i<10;i++) {
        if (jse.button[i] && jsbuttonaction[i] == keyquit) {
            in_the_process_of_quitting = 1;
            wwviaudio_pause_audio();
            return;
        }
    }

    // If credits are zero, or health is zero -- ANY button on the joystick 
    // will put in a quarter. 
    if (timer > next_quarter_time) {
        for (i=0;i<10;i++)
            if (jse.button[i]) {
                insert_quarter();
                next_quarter_time = timer + frame_rate_hz;
                break;
            }
    }
}
    */

void deal_with_keyboard()
{
    static int lastvy_inc = 0;
    int moved;
    static int next_keyboard_motion_timer = 0;

    if (in_the_process_of_quitting) 
    {
        if (game_state.key_left_pressed)
            current_quit_selection = 1;
        if (game_state.key_right_pressed)
            current_quit_selection = 0;
        if (game_state.key_laser_pressed) 
        {
            final_quit_selection = current_quit_selection;
            if (!final_quit_selection) 
            {
                in_the_process_of_quitting = 0;
                wwviaudio_resume_audio();
            }
        }
    }

    if (timer_event == NEW_HIGH_SCORE_EVENT) 
    {
        moved = 0;
        if (timer > next_keyboard_motion_timer) 
        {
            next_keyboard_motion_timer = timer + (frame_rate_hz >> 3);
            if (game_state.key_left_pressed) 
            {
                highscore_letter--;
                if (highscore_letter < 0)
                    highscore_letter = 26;
                moved = 1;
            }
            if (game_state.key_right_pressed) 
            {
                highscore_letter++;
                if (highscore_letter > 26)
                    highscore_letter = 0;
                moved = 1;
            }
            if (game_state.key_up_pressed) 
            {
                highscore_letter -= 9;
                if (highscore_letter < 0)
                    highscore_letter += 27;
                moved = 1;
            }
            if (game_state.key_down_pressed) 
            {
                highscore_letter += 9;
                if (highscore_letter > 26)
                    highscore_letter -= 27;
                moved = 1;
            }
            if (game_state.key_laser_pressed) 
            {
                highscore_buttonpress = 1;
                if (highscore_letter >= 0 && highscore_letter < 26) 
                {
                    highscore_initials[highscore_current_initial] = highscore_letter + 'A';
                    highscore_current_initial++;
                } 
                else if (highscore_letter == 26) 
                { // done. 
                    highscore_current_initial = 3; // ends it. 
                }
                moved = 1;
            }
        }
        if (moved) 
            wwviaudio_add_sound(PLAYER_LASER_SOUND);
        return;
    }

    if (game_state.health <= 0)
        return;

    if (credits <= 0)
        return;
#if 0
    printf("u%d d%d l%d r%d\n",
        game_state.key_up_pressed,
        game_state.key_down_pressed,
        game_state.key_left_pressed,
        game_state.key_right_pressed);
#endif
    if (game_state.key_left_pressed) 
    {
        if (game_state.direction != -1) 
        {
            player->vx = player->vx / 2;
            game_state.direction = -1;
            player->v = &left_player_vect;
        } 
        else if (abs(player->vx + game_state.direction) < MAX_VX)
                player->vx += game_state.direction;
    } 
    else if (game_state.key_right_pressed) 
    {
        if (game_state.direction != 1) 
        {
            player->vx = player->vx / 2;
            game_state.direction = 1;
            player->v = &player_vect;
        } 
        else if (abs(player->vx + game_state.direction) < MAX_VX)
                player->vx += game_state.direction;
    }

    if (game_state.key_up_pressed) 
    {
        if (lastvy_inc >= -1)
            lastvy_inc = -2;
        if (player->vy > 0) {
            lastvy_inc--;
        }
        player->vy += lastvy_inc;
        if ((timer & 0x03) == 3)
            lastvy_inc--;
        if (player->vy < -MAX_VY)
            player->vy = -MAX_VY;
    } 
    else if (game_state.key_down_pressed) 
    {
        if (lastvy_inc <= 1)
            lastvy_inc = 2;
        if (player->vy > 0) 
        {
            lastvy_inc++;
        }
        player->vy += lastvy_inc;
        if ((timer & 0x03) == 3)
            lastvy_inc++;
        if (player->vy > MAX_VY)
            player->vy = MAX_VY;
    } 
    else 
    {
        /* Damp vy, but, only if they don't have a joystick. */
        /* the joystick code will damp vy, and we don't want */
        /* to damp it 2x if they have a joystick.  But, if they */
        /* don't have a joystick... damp it extra hard. */
        /* simply because the joystic has better control. */
        if (player->vy != 0 && jsfd < 0) 
        {
            if (player->vy < -2)
                player->vy += 2;
            else if (player->vy > 2)
                player->vy -= 2;
            else
                player->vy = 0;
        }
        lastvy_inc = 0;
    }

    if (game_state.key_bomb_pressed)
        drop_bomb();

    if (game_state.key_present_pressed && xmas_mode)
        drop_present();

    if (game_state.key_gbomb_pressed)
        drop_gravity_bomb();

    if (game_state.key_laser_pressed)
        player_fire_laser();

    if (game_state.key_chaff_pressed)
        drop_chaff();
}



struct keyname_value_entry {
    char *name;
    unsigned int value;
} keyname_value_map[] = {
    { "a", 0 }, //a }, 
    { "b", 0 }, //b }, 
    { "c", 0 }, //c }, 
    { "d", 0 }, //d }, 
    { "e", 0 }, //e }, 
    { "f", 0 }, //f }, 
    { "g", 0 }, //g }, 
    { "h", 0 }, //h }, 
    { "i", 0 }, //i }, 
    { "j", 0 }, //j }, 
    { "k", 0 }, //k }, 
    { "l", 0 }, //l }, 
    { "m", 0 }, //m }, 
    { "n", 0 }, //n }, 
    { "o", 0 }, //o }, 
    { "p", 0 }, //p }, 
    { "q", 0 }, //q }, 
    { "r", 0 }, //r }, 
    { "s", 0 }, //s }, 
    { "t", 0 }, //t }, 
    { "u", 0 }, //u }, 
    { "v", 0 }, //v }, 
    { "w", 0 }, //w }, 
    { "x", 0 }, //x }, 
    { "y", 0 }, //y }, 
    { "z", 0 }, //z }, 
    { "A", 0 }, //A }, 
    { "B", 0 }, //B }, 
    { "C", 0 }, //C }, 
    { "D", 0 }, //D }, 
    { "E", 0 }, //E }, 
    { "F", 0 }, //F }, 
    { "G", 0 }, //G }, 
    { "H", 0 }, //H }, 
    { "I", 0 }, //I }, 
    { "J", 0 }, //J }, 
    { "K", 0 }, //K }, 
    { "L", 0 }, //L }, 
    { "M", 0 }, //M }, 
    { "N", 0 }, //N }, 
    { "O", 0 }, //O }, 
    { "P", 0 }, //P }, 
    { "Q", 0 }, //Q }, 
    { "R", 0 }, //R }, 
    { "S", 0 }, //S }, 
    { "T", 0 }, //T }, 
    { "U", 0 }, //U }, 
    { "V", 0 }, //V }, 
    { "W", 0 }, //W }, 
    { "X", 0 }, //X }, 
    { "Y", 0 }, //Y }, 
    { "Z", 0 }, //Z }, 
    { "0", 0 }, //0 }, 
    { "1", 0 }, //1 }, 
    { "2", 0 }, //2 }, 
    { "3", 0 }, //3 }, 
    { "4", 0 }, //4 }, 
    { "5", 0 }, //5 }, 
    { "6", 0 }, //6 }, 
    { "7", 0 }, //7 }, 
    { "8", 0 }, //8 }, 
    { "9", 0 }, //9 }, 
    { "-", 0 }, //minus }, 
    { "+", 0 }, //plus }, 
    { "=", 0 }, //equal }, 
    { "?", 0 }, //question }, 
    { ".", 0 }, //period }, 
    { ",", 0 }, //comma }, 
    { "<", 0 }, //less }, 
    { ">", 0 }, //greater }, 
    { ":", 0 }, //colon }, 
    { ";", 0 }, //semicolon }, 
    { "@", 0 }, //at }, 
    { "*", 0 }, //asterisk }, 
    { "$", 0 }, //dollar }, 
    { "%", 0 }, //percent }, 
    { "&", 0 }, //ampersand }, 
    { "'", 0 }, //apostrophe }, 
    { "(", 0 }, //parenleft }, 
    { ")", 0 }, //parenright }, 
    { "space", 0 }, //space }, 
    { "enter", 0 }, //Return }, 
    { "return", 0 }, //Return }, 
    { "backspace", 0 }, //BackSpace }, 
    { "delete", 0 }, //Delete }, 
    { "pause", 0 }, //Pause }, 
    { "scrolllock", 0 }, //Scroll_Lock }, 
    { "escape", 0 }, //Escape }, 
    { "sysreq", 0 }, //Sys_Req }, 
    { "left", 0 }, //Left }, 
    { "right", 0 }, //Right }, 
    { "up", 0 }, //Up }, 
    { "down", 0 }, //Down }, 
    { "kp_home", 0 }, //KP_Home }, 
    { "kp_down", 0 }, //KP_Down }, 
    { "kp_up", 0 }, //KP_Up }, 
    { "kp_left", 0 }, //KP_Left }, 
    { "kp_right", 0 }, //KP_Right }, 
    { "kp_end", 0 }, //KP_End }, 
    { "kp_delete", 0 }, //KP_Delete }, 
    { "kp_insert", 0 }, //KP_Insert }, 
    { "home", 0 }, //Home }, 
    { "down", 0 }, //Down }, 
    { "up", 0 }, //Up }, 
    { "left", 0 }, //Left }, 
    { "right", 0 }, //Right }, 
    { "end", 0 }, //End }, 
    { "delete", 0 }, //Delete }, 
    { "insert", 0 }, //Insert }, 
    { "kp_0", 0 }, //KP_0 }, 
    { "kp_1", 0 }, //KP_1 }, 
    { "kp_2", 0 }, //KP_2 }, 
    { "kp_3", 0 }, //KP_3 }, 
    { "kp_4", 0 }, //KP_4 }, 
    { "kp_5", 0 }, //KP_5 }, 
    { "kp_6", 0 }, //KP_6 }, 
    { "kp_7", 0 }, //KP_7 }, 
    { "kp_8", 0 }, //KP_8 }, 
    { "kp_9", 0 }, //KP_9 }, 
    { "f1", 0 }, //F1 }, 
    { "f2", 0 }, //F2 }, 
    { "f3", 0 }, //F3 }, 
    { "f4", 0 }, //F4 }, 
    { "f5", 0 }, //F5 }, 
    { "f6", 0 }, //F6 }, 
    { "f7", 0 }, //F7 }, 
    { "f9", 0 }, //F9 }, 
    { "f9", 0 }, //F9 }, 
    { "f10", 0 }, //F10 }, 
    { "f11", 0 }, //F11 }, 
    { "f12", 0 }, //F12 }, 
};

enum keyaction keymap[256];
enum keyaction ffkeymap[256];
unsigned char *keycharmap[256];

static int helpscreen_pixel_row = -40;
line_drawing_function *old_line_draw;

void draw_line_help(gint x1, gint y1, gint x2, gint y2)
{
    y1 = y1 - helpscreen_pixel_row;
    y2 = y2 - helpscreen_pixel_row;

    if (    x1 < 50 || x1 > SCREEN_WIDTH-100 || 
        y1 < 100 || y1 > SCREEN_HEIGHT-100 ||
        x2 < 50 || x2 > SCREEN_WIDTH-100 ||
        y2 < 100 || y2 > SCREEN_HEIGHT-100) 
        return;
    old_line_draw(x1, y1, x2, y2);
}

static void draw_help_screen()
{
    enum keyaction k;
    int row, column;
    int max_y;
    int y;


    gdk_gc_set_foreground(gc, &huex[BLACK]);
    wwvi_draw_rectangle(TRUE, 50, 50, SCREEN_WIDTH-100, SCREEN_HEIGHT-100);
    gdk_gc_set_foreground(gc, &huex[RED]);
    wwvi_draw_rectangle(FALSE, 50, 50, SCREEN_WIDTH-100, SCREEN_HEIGHT-100);

    if (helpscreen_pixel_row > -30)
        abs_xy_draw_string("-- More Above -- (press 'up' arrow)", TINY_FONT, 80, 70);

    old_line_draw = current_draw_line; 
    current_draw_line = draw_line_help; 



    y = 90;
    gdk_gc_set_foreground(gc, &huex[GREEN]);
    abs_xy_draw_string("Object of the game:", SMALL_FONT, 80, y);
    y += 50;
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string("Rescue your four lost .swp files and transport them to the end of", TINY_FONT, 80, y);
    y += 20;
    abs_xy_draw_string("the level and take the socket to the next node in the cluster.  Avoid", TINY_FONT, 80, y);
    y += 20;
    abs_xy_draw_string("OS defenses and watch out for cron jobs which will attempt to destroy", TINY_FONT, 80, y);
    y += 20;
    abs_xy_draw_string("your .swp files.  Use laser fire and bombs to destroy enemy processes.", TINY_FONT, 80, y);
    y += 50;

    gdk_gc_set_foreground(gc, &huex[GREEN]);
    abs_xy_draw_string("Things you may encounter:", SMALL_FONT, 80, y);
    /* abs_xy_draw_string("Help?", BIG_FONT, 300, 280);*/

    y += 50;
    gdk_gc_set_foreground(gc, &huex[GREEN]);
    draw_abs_scaled_generic(80, y, &cron_vect, 0.5, 0.5);
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string("Cron Job", TINY_FONT, 110, y);
    gdk_gc_set_foreground(gc, &huex[CYAN]);

    y += 50;
    draw_abs_scaled_generic(80, y, &gdb_vect_left, 0.5, 0.5);
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string("gdb", TINY_FONT, 110, y);
    gdk_gc_set_foreground(gc, &huex[CYAN]);

    y += 50;
    draw_abs_scaled_generic(80, y, &rocket_vect, 0.5, 0.5);
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string("Rocket", TINY_FONT, 110, y);
    gdk_gc_set_foreground(gc, &huex[WHITE]);

    y += 50;
    draw_abs_scaled_generic(80, y, &SAM_station_vect, 0.5, 0.5);
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string("SAM station", TINY_FONT, 110, y);

    y -= 150;
    gdk_gc_set_foreground(gc, &huex[CYAN]);
    draw_abs_scaled_generic(260, y, &airship_vect, 0.1, 0.1);
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string("EMACS (Stallman's airship)", TINY_FONT, 310, y);
    gdk_gc_set_foreground(gc, &huex[WHITE]);

    y += 50;
    draw_abs_scaled_generic(260, y, &jet_vect, 0.5, 0.5);
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string("Jet", TINY_FONT, 310, y);
    gdk_gc_set_foreground(gc, &huex[RED]);

    y += 50;
    draw_abs_scaled_generic(260, y, &kgun_vect, 0.5, 0.5);
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string("Laser turret", TINY_FONT, 310, y);
    gdk_gc_set_foreground(gc, &huex[CYAN]);

    y += 50;
    draw_abs_scaled_generic(260, y, &balloon_vect, 0.2, 0.2);
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string("fetchmail", TINY_FONT, 310, y);
    abs_xy_draw_string("(Raymond's Gasbag)", TINY_FONT, 310, 300);

    y -= 150;   
    gdk_gc_set_foreground(gc, &huex[ORANGE]);
    draw_abs_scaled_generic(570, y, &fuel_vect, 0.5, 0.5);
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string("Fuel tank", TINY_FONT, 620, y);
    gdk_gc_set_foreground(gc, &huex[MAGENTA]);

    y += 50;
    draw_abs_scaled_generic(570, y, &humanoid_vect, 1.0, 1.0);
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string(".swp file", TINY_FONT, 620, y);
    gdk_gc_set_foreground(gc, &huex[WHITE]);

    y += 50;
    draw_abs_scaled_generic(570, y, &socket_vect, 0.5, 0.5);
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string("socket", TINY_FONT, 620, y);
    gdk_gc_set_foreground(gc, &huex[GREEN]);

    y += 50;
    draw_abs_scaled_generic(570, y, &jetpilot_vect_right, 1.0, 1.0);
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string("Jet pilot", TINY_FONT, 620, y);

    y += 50;
    row = 0;
    column = 0;
    gdk_gc_set_foreground(gc, &huex[GREEN]);
    abs_xy_draw_string("Controls:", SMALL_FONT, 80, y);
    gdk_gc_set_foreground(gc, &huex[WHITE]);

    y += 50;

    for (k=keydown;k<=key_droppresent;k++) {
        char str[50];
        unsigned int i, j, foundit;

        foundit = 0;
        for (i=0;i<ARRAY_SIZE(keymap);i++) {
            if (keymap[i] == k) {
                for (j=0;j<ARRAY_SIZE(keyname_value_map);j++) {
                    if (keyname_value_map[j].value == i) {
                        sprintf(str, "%15s %s", keyactionstring[k],
                            keyname_value_map[j].name);

                        abs_xy_draw_string(str, TINY_FONT, 80 + 200*column, y + row*20);
                        column++;
                        if (column > 2) {
                            column = 0;
                            row++;
                        }
                        /* foundit = 1;*/
                        /* break;*/
                    }
                }
            } else if (ffkeymap[i] == k) {
                for (j=0;j<ARRAY_SIZE(keyname_value_map);j++) {
                    if ((keyname_value_map[j].value & 0x00ff) == i &&
                        (keyname_value_map[j].value & 0xff00) != 0) {
                        sprintf(str, "%15s %s", keyactionstring[k],
                            keyname_value_map[j].name);

                        abs_xy_draw_string(str, TINY_FONT, 80 + 200*column, y + row*20);
                        column++;
                        if (column > 2) {
                            column = 0;
                            row++;
                        }
                        /* foundit = 1;*/
                        /* break;*/
                    }
                }
            }
            if (foundit)
                break;
        }
    }
    max_y = y + row*20 + 30;

    y = max_y + 20;
    gdk_gc_set_foreground(gc, &huex[WHITE]);
    abs_xy_draw_string("Additionally, you may use a gamepad such as an Xbox 360", TINY_FONT, 80, y);
    y += 20;
    abs_xy_draw_string("controller (recommended) or a Logitech Dual Action Rumble.", TINY_FONT, 80, y);

    y += 40;

    gdk_gc_set_foreground(gc, &huex[GREEN]);
    abs_xy_draw_string("About Word War vi:", SMALL_FONT, 80, y);
    gdk_gc_set_foreground(gc, &huex[WHITE]);

    y += 40;
    abs_xy_draw_string("This is Word War vi version " WORDWARVI_VERSION, 
        TINY_FONT, 80, y); 

    y += 20;
    abs_xy_draw_string("Be sure to read the wordwarvi man page as well.", 
        TINY_FONT, 80, y); 

    y += 20;
    abs_xy_draw_string("(yes there is a man page.  Type 'man wordwarvi'.)", 
        TINY_FONT, 80, y); 

    y += 20;
    abs_xy_draw_string("Check http://smcameron.github.com/wordwarvi for updates.", 
        TINY_FONT, 80, y); 

    max_y += 180;

    if (game_state.key_up_pressed  && helpscreen_pixel_row > -40) {
            helpscreen_pixel_row -= 5;
    }
    if ((game_state.key_down_pressed || game_state.key_laser_pressed)
        && helpscreen_pixel_row < (max_y - (SCREEN_HEIGHT-100)) ) {
            helpscreen_pixel_row += 5;
    }

    current_draw_line = old_line_draw; 

    gdk_gc_set_foreground(gc, &huex[RED]);
    if (helpscreen_pixel_row < (max_y - (SCREEN_HEIGHT-100)))
        abs_xy_draw_string("-- More Below -- (press 'down' arrow)", TINY_FONT, 80, SCREEN_HEIGHT-65);

#if 0
    if (current_help_selection == 1) {
        x = 130;
        gdk_gc_set_foreground(gc, &huex[WHITE]);
    } else {
        x = 480;
        gdk_gc_set_foreground(gc, &huex[RED]);
    }
    abs_xy_draw_string("Yes help", SMALL_FONT, 150, 450);
    if (current_quit_selection == 0)
        gdk_gc_set_foreground(gc, &huex[WHITE]);
    else
        gdk_gc_set_foreground(gc, &huex[RED]);
    abs_xy_draw_string("no help", SMALL_FONT, 500, 450);

    if ((quittimer & 0x04)) {
        gdk_gc_set_foreground(gc, &huex[WHITE]);
        wwvi_draw_rectangle(FALSE, x, 420, 200, 50);
    }
#endif
}

void init_keymap()
{
  // todo ?
  
    memset(keymap, 0, sizeof(keymap));
    memset(ffkeymap, 0, sizeof(ffkeymap));
    memset(keycharmap, 0, sizeof(keycharmap));
/*
    keymap[GDK_j] = keydown;
    ffkeymap[GDK_Down & 0x00ff] = keydown;

    keymap[GDK_k] = keyup;
    ffkeymap[GDK_Up & 0x00ff] = keyup;

    keymap[GDK_l] = keyright;
    ffkeymap[GDK_Right & 0x00ff] = keyright;
    keymap[GDK_period] = keyright;
    keymap[GDK_greater] = keyright;

    keymap[GDK_h] = keyleft;
    ffkeymap[GDK_Left & 0x00ff] = keyleft;
    keymap[GDK_comma] = keyleft;
    keymap[GDK_less] = keyleft;

    keymap[GDK_space] = keylaser;
    keymap[GDK_z] = keylaser;

    keymap[GDK_b] = keybomb;
    keymap[GDK_v] = key_droppresent;
    keymap[GDK_g] = keygravitybomb;
    keymap[GDK_c] = keychaff;
    keymap[GDK_x] = keythrust;
    keymap[GDK_p] = keypause;
    ffkeymap[GDK_F1 & 0x00ff] = keypausehelp;
    keymap[GDK_q] = keyquarter;
    keymap[GDK_m] = keymusic;
    keymap[GDK_s] = keysoundeffects;
    ffkeymap[GDK_Escape & 0x00ff] = keyquit;
    keymap[GDK_1] = keytogglemissilealarm;

    keymap[GDK_2] = key2;
    keymap[GDK_3] = key3;
    keymap[GDK_4] = key4;
    keymap[GDK_5] = key5;
    keymap[GDK_6] = key6;
    keymap[GDK_7] = key7;
    keymap[GDK_8] = key8;
    keymap[GDK_9] = keysuicide;

    ffkeymap[GDK_F11 & 0x00ff] = keyfullscreen;
    */
}

int remap_joystick_button(int button, char *actionname)
{
    enum keyaction i;

    if (button < 0 || button > 9)
        return -1;

    for (i=keynone;i<=key_droppresent;i++) {
        if (strcmp(keyactionstring[i], actionname) == 0) {
            printf("remapped joystick button %d to %s\n", 
                button, actionname);
            jsbuttonaction[button] = i;
            return 0;
        }
    }
    return -1;
}

int remapkey(char *keyname, char *actionname)
{
    enum keyaction i;
    unsigned int j;
    int index;

    for (i=keynone;i<=key_droppresent;i++) {
        if (strcmp(keyactionstring[i], actionname) == 0) {
            for (j=0;j<ARRAY_SIZE(keyname_value_map);j++) {
                if (strcmp(keyname_value_map[j].name, keyname) == 0) {
                    if ((keyname_value_map[j].value & 0xff00) != 0) {
                        index = keyname_value_map[j].value & 0x00ff;
                        ffkeymap[index] = i;
                    } else
                        keymap[keyname_value_map[j].value] = i;
                    return 0;
                }
            }
        }
    }
    return 1;
}
/*
static gint key_release_cb( GdkEventKey* event,
    __attribute__((unused)) gpointer data)
{

    enum keyaction ka;

    if ((event->keyval & 0xff00) == 0)
        ka = keymap[event->keyval];
    else
        ka = ffkeymap[event->keyval & 0x00ff];
    last_user_input_time = timer;
    switch (ka) {
    case keydown:
        game_state.key_down_pressed = 0;
        return TRUE;
    case keyup:
        game_state.key_up_pressed = 0;
        return TRUE;
    case keyright:
        game_state.key_right_pressed = 0;
        return TRUE;
    case keyleft:
        game_state.key_left_pressed = 0;
        return TRUE;
    case keylaser:
        game_state.key_laser_pressed = 0;
        return TRUE;
    case keychaff:
        game_state.key_chaff_pressed = 0;
        return TRUE;
    case keybomb:
        game_state.key_bomb_pressed = 0;
        return TRUE;
    case key_droppresent:
        game_state.key_present_pressed = 0;
        return TRUE;
    case keygravitybomb:
        game_state.key_gbomb_pressed = 0;
        return TRUE;
    default:
        break;
    }
    return FALSE;
}
*/

/*
static gint key_press_cb(GdkEventKey* event,
    __attribute__((unused)) gpointer data)
{
    enum keyaction ka;
    // char *x = (char *) data; 
#if 0
    if (event->length > 0)
        printf("The key event's string is `%s'\n", event->string);

    printf("The name of this keysym is `%s'\n", 
        gdk_keyval_name(event->keyval));
#endif

    if ((event->keyval & 0xff00) == 0)
        ka = keymap[event->keyval];
    else
        ka = ffkeymap[event->keyval & 0x00ff];

    last_user_input_time = timer;
    switch (ka) {
    case key2:
            game_state.cmd_multiplier = 2;
            return TRUE;
    case key3:
            game_state.cmd_multiplier = 3;
            return TRUE;
    case key4:
            game_state.cmd_multiplier = 4;
            return TRUE;
    case key5:
            game_state.cmd_multiplier = 5;
            return TRUE;
    case key6:
            game_state.cmd_multiplier = 6;
            return TRUE;
    case key8:
            player->tsd.epd.count = 50;
            player->tsd.epd.count2 = 0;
            break;
    case key7:
            player->tsd.epd.count = -1;
            player->tsd.epd.count2 = 50;
            break;
    case keysuicide:
        if (credits > 0)
            game_state.health = -1;
        return TRUE;
    case keysoundeffects:
        game_state.sound_effects_on = !game_state.sound_effects_on;
        game_state.sound_effects_manually_turned_off = (!game_state.sound_effects_on);
        wwviaudio_toggle_sound_effects();
        return TRUE;
    case keymusic:
        wwviaudio_toggle_music();
        return TRUE;
    case keytogglemissilealarm:
        want_missile_alarm = !want_missile_alarm;
        return TRUE;
    case keyquit:
        if (game_pause_help)
            do_game_pause_help();
        else {
            in_the_process_of_quitting = !in_the_process_of_quitting;
            if (in_the_process_of_quitting)
                wwviaudio_pause_audio();
            else
                wwviaudio_resume_audio();
        }
        return TRUE;    
    case keyquarter:
        insert_quarter();
        return TRUE;
    case keyfullscreen: {
            if (fullscreen) {
                gtk_window_unfullscreen(GTK_WINDOW(window));
                fullscreen = 0;
                // configure_event takes care of resizing drawing area, etc. 
            } else {
                gtk_window_fullscreen(GTK_WINDOW(window));
                fullscreen = 1;
                // configure_event takes care of resizing drawing area, etc. 
            }
        }
        break;
    case keydown:
        game_state.key_down_pressed = 1;
#if 0
        if (player->vy < MAX_VY && game_state.health > 0 && credits > 0)
            player->vy += 4 * game_state.cmd_multiplier;
        game_state.cmd_multiplier = 1;
#endif
        return TRUE;
    case keyup:
        game_state.key_up_pressed = 1;
#if 0
        if (player->vy > -MAX_VY && game_state.health > 0 && credits > 0)
            player->vy -= 4 * game_state.cmd_multiplier;
#endif
        return TRUE;
    case keyright: {
        game_state.key_right_pressed = 1;
#if 0
        int i;

        for (i=0;i<game_state.cmd_multiplier;i++) {
            if (game_state.health <= 0 || credits <= 0)
                return TRUE;
            if (game_state.direction != 1) {
                game_state.direction = 1;
                player->vx = player->vx / 2;
                player->v = &player_vect;
            } else if (abs(player->vx + game_state.direction) < MAX_VX)
                    player->vx += game_state.direction;
        }
        game_state.cmd_multiplier = 1;
#endif
        return TRUE;
    }
    case keyleft: {
        game_state.key_left_pressed = 1;
#if 0
        int i;

        for (i=0;i<game_state.cmd_multiplier;i++) {
            if (game_state.health <= 0 || credits <= 0)
                return TRUE;
            if (game_state.direction != -1) {
                player->vx = player->vx / 2;
                game_state.direction = -1;
                player->v = &left_player_vect;
            } else if (abs(player->vx + game_state.direction) < MAX_VX)
                    player->vx += game_state.direction;
        }
        game_state.cmd_multiplier = 1;
#endif
        return TRUE;
    }
    case keylaser:
        game_state.key_laser_pressed = 1;
        return TRUE;
    case keythrust:
        if (game_state.health <= 0 || credits <= 0)
            return TRUE;
        if (abs(player->vx + game_state.direction) < MAX_VX)
            player->vx += game_state.direction;
        return TRUE;

    case keychaff:
        game_state.key_chaff_pressed = 1;
        return TRUE;
    case keybomb: 
        game_state.key_bomb_pressed = 1;
        return TRUE;
    case key_droppresent: 
        game_state.key_present_pressed = 1;
        return TRUE;
    case keygravitybomb: 
        game_state.key_gbomb_pressed = 1;
        return TRUE;
    case keypause:
        // if (game_state.health <= 0 || credits <= 0)
        //  return TRUE;
        do_game_pause();
        return TRUE;

    case keypausehelp:
        do_game_pause_help();
        return TRUE;
#if 0
    // just for debugging. 
    case GDK_n:
        if (game_state.health <= 0 || credits <= 0)
            return TRUE;
        player->x = human[lasthuman]->x;
        player->y = human[lasthuman]->y - 50;
        game_state.x = player->x;
        game_state.y = player->y - 50;
        lasthuman++;
        if (lasthuman >= level.nhumanoids)
            lasthuman = 0;
        break;
    case GDK_i:
        gettimeofday(&game_state.finish_time, NULL);
        timer_event = START_INTERMISSION_EVENT;
        next_timer = timer + 1;
        return TRUE;
    case GDK_R:
        start_level();
        break;
    case GDK_A:
        advance_level();
        break;
// The above just for testing 
#endif
    default:
        break;
    }

#if 0
    printf("Keypress: GDK_%s\n", gdk_keyval_name(event->keyval));
    if (gdk_keyval_is_lower(event->keyval)) {
        printf("A non-uppercase key was pressed.\n");
    } else if (gdk_keyval_is_upper(event->keyval)) {
        printf("An uppercase letter was pressed.\n");
    }
#endif
    return FALSE;
}
*/


/***********************************************************************/
/* Beginning of AUDIO related code                                     */
/***********************************************************************/

#ifdef WITHAUDIOSUPPORT
int init_clips()
{
    printf("Decoding audio data..."); fflush(stdout);

#endif

/***********************************************************************/
/* End of AUDIO related code                                     */
/***********************************************************************/

void setup_rainbow_colors()
{

    int i, r, g, b, dr, dg, db, c;

    rainbow_color = &huex[NCOLORS + NSPARKCOLORS];

    
    r = 32766*2;
    g = 0;
    b = 0;

    dr = -r / NRAINBOWSTEPS;
    dg = r / NRAINBOWSTEPS;
    db = 0;

    c = 0;

    for (i=0;i<NRAINBOWSTEPS;i++) {
        rainbow_color[c].red = (unsigned short) r;
        rainbow_color[c].green = (unsigned short) g;
        rainbow_color[c].blue = (unsigned short) b;

        r += dr;
        g += dg;
        b += db;

        c++;
    }

    dg = (-32766*2) / NRAINBOWSTEPS;
    db = -dg;
    dr = 0;

    for (i=0;i<NRAINBOWSTEPS;i++) {
        rainbow_color[c].red = (unsigned short) r;
        rainbow_color[c].green = (unsigned short) g;
        rainbow_color[c].blue = (unsigned short) b;

        r += dr;
        g += dg;
        b += db;

        c++;
    }

    db = (-32766*2) / NRAINBOWSTEPS;
    dr = -db;
    dg = 0;

    for (i=0;i<NRAINBOWSTEPS;i++) {
        rainbow_color[c].red = (unsigned short) r;
        rainbow_color[c].green = (unsigned short) g;
        rainbow_color[c].blue = (unsigned short) b;

        r += dr;
        g += dg;
        b += db;

        c++;
    }
}

void setup_spark_colors()
{

/* Set up an array of colors that fade nicely from bright
   yellow to orange to red.  */

    int i, r,g,b, dr, dg, db;

    sparkcolor = &huex[NCOLORS];

    r = 32766*2;
    g = 32766*2;
    b = 32766 * 1.5;

    dr = 0;
    dg = (-(2500) / NSPARKCOLORS);
    db = 3 * dg;
    
    for (i=NSPARKCOLORS-1; i>=0; i--) {
        sparkcolor[i].red = (unsigned short) r;
        sparkcolor[i].green = (unsigned short) g;
        sparkcolor[i].blue = (unsigned short) b;
        sparkcolor[i].pixel = 0;

        r += dr;
        g += dg;
        b += db;

        if (r < 0) r = 0;
        if (g < 0) g = 0;
        if (b < 0) b = 0;

        dg *= 1.27;
        db *= 1.27;
    }
}

void paint_it_black()
{
    int i;
    unsigned int avg;
    for (i=0;i<NCOLORS + NSPARKCOLORS + NRAINBOWCOLORS;i++) {
        avg = huex[i].red + huex[i].green + huex[i].blue;
        avg = avg / 3;
        if (avg < 2000)
            avg = 32767*2;
        else if (avg < 20000)
            avg = 32767;
        else
            avg = 15000;
        huex[i].red = avg;
        huex[i].green = avg;
        huex[i].blue = avg;
    }
}

void paint_it_blue()
{
    int i;
    unsigned int avg;
    for (i=0;i<NCOLORS + NSPARKCOLORS + NRAINBOWCOLORS;i++) {
        avg = huex[i].red + huex[i].green + huex[i].blue;
        avg = avg / 3;
        if (avg > 200) {
            huex[i].red = 32767*2;
            huex[i].green = 32767*2;
            huex[i].blue = 32767*2;
        } else {
            huex[i].red = 0;
            huex[i].green = 0;
            huex[i].blue = 32767/2;
        }
    }
}

void paint_it_green()
{
    int i;
    unsigned int avg;
    for (i=0;i<NCOLORS + NSPARKCOLORS + NRAINBOWCOLORS;i++) {
        avg = huex[i].red + huex[i].green + huex[i].blue;
        avg = avg / 3;
        if (avg > 40000) {
            huex[i].red = 32767;
            huex[i].green = 32767*2;
            huex[i].blue = 32767;
        } else if (avg > 30000) {
            huex[i].red = 0;
            huex[i].green = 32767*2;
            huex[i].blue = 0;
        } else if (avg > 100) {
            huex[i].red = 0;
            huex[i].green = 40000;
            huex[i].blue = 0;
        } else {
            huex[i].red = 0;
            huex[i].green = 0;
            huex[i].blue = 0;
        }
    }
}

static struct option wordwarvi_options[] = {
    { "bw", 0, NULL, 0 },
    { "sounddevice", 1, NULL, 1 },
    { "version", 0, NULL, 2 },
    { "brightsparks", 0, NULL, 3 },
    { "width", 1, NULL, 4 },
    { "height", 1, NULL, 5 },
    { "framerate", 1, NULL, 6 },
    { "nostarfield", 0, NULL, 7 },
    { "blueprint", 0, NULL, 8 },
    { "fullscreen", 0, NULL, 9 },
    { "nomusic", 0, NULL, 10 },
    { "joystick", 1, NULL, 11 },
    { "retrogreen", 0, NULL, 12 },
    { "randomize", 0, NULL, 13 },
    { "randomseed", 1, NULL, 14 },
    { "nomissilealarm", 0, NULL, 16 },
    { "difficulty", 1, NULL, 17 },
    { "norumble", 0, NULL, 18 },
    { "rumbledevice", 1, NULL, 19 },
    { "starmotion", 1, NULL, 20 },
    { "nstars", 1, NULL, 21 },
    { "thicklines", 0, NULL, 22 },
    { "squareexplosions", 0, NULL, 23 },
    { "explosionfactor", 1, NULL, 24 },
    { "xmas", 0, NULL, 25 },
    { "sphericalexplosions", 0, NULL, 26 },
    { "help", 0, NULL, 27 },
#ifdef LEVELWARP
    { "levelwarp", 1, NULL, 15 },
#endif
    { NULL, 0, NULL, 0 },
};

void usage()
{
    fprintf(stderr, "wordwarvi:  usage:\n");
    fprintf(stderr, "wordwarvi [options]\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "--help            Show this help.\n");
    fprintf(stderr, "--bw              Render in black and white.\n");
    fprintf(stderr, "--blueprint       Render in the style of a blueprint.\n");
    fprintf(stderr, "--brightsparks    Render sparks brighter than usual.\n");
    fprintf(stderr, "--difficulty      Sets difficulty level: easy, medium, hard, insane, batshit-insane.\n");
    fprintf(stderr, "--explosionfactor Multiplier for number of sparks in explosions.\n");
    fprintf(stderr, "--framerate n     Attempt to render the game at n frames per second.\n");
    fprintf(stderr, "--fullscreen      Render the game in full screen mode.\n");
    fprintf(stderr, "--height y        Render the game y pixels high.\n");
    fprintf(stderr, "--joystick dev    Use joystick input device dev.\n");
#ifdef LEVELWARP
    fprintf(stderr, "--levelwarp n     Warp ahead n levels.\n");
#endif
    fprintf(stderr, "--nomusic         Do not play, or even decode music data.\n");
    fprintf(stderr, "--nomissilealarm  Do not sound alarm for missile lock on.\n");
    fprintf(stderr, "--norumble        Do not use joystick rumble effects.\n");
    fprintf(stderr, "--nostarfield     Do not render the background starfield.\n");
    fprintf(stderr, "--nstars n        How many stars to draw.  Default=150\n");
    fprintf(stderr, "--retrogreen      Render in the manner of a vector display from the '70's.\n");
    fprintf(stderr, "--randomize       Use a clock generated random seed to initialize levels.\n");
    fprintf(stderr, "--randomseed n    Use the specified random seed to initialize levels.\n");
    fprintf(stderr, "--squareexplosions Make explosions square rather than round.\n");
    fprintf(stderr, "--sphericaleexplosions Make explosions spherical rather than round.\n");
    fprintf(stderr, "--rumbledevice d  Use the device file d for rumble effects. (default is /dev/input/event5)\n");
    fprintf(stderr, "--sounddevice n   Use the nth sound device for audio output.\n");
    fprintf(stderr, "--starmotion x    Set how starfield should move.  Possile values are:\n");
    fprintf(stderr, "                  astronomically-correct, wrong, wronger, and wrongest.\n");
    fprintf(stderr, "--thicklines      Render everything with thick lines.\n");
    fprintf(stderr, "--version         Print the version number and exit.\n");
    fprintf(stderr, "--width x         Render the game x pixels wide.\n");
    fprintf(stderr, "--xmas            Merry Christmas.\n");
    exit(1);
}

void write_out_high_score_file()
{
    /* Here's where you'd write out the high_score[] array */
    /* to the file descriptor high_score_file_descriptor */
    /* The .score element is a uint32_t, and should be run */
    /* through htonl() before writing it out, as the high score. */
    /* file should be endian neutral. */

    int fd;
    int i, n;
    uint32_t bigendian_score;
    off_t pos;

    fd = high_score_file_descriptor;

    if (fd == -1)
        return;

    pos = lseek(fd, 0, SEEK_SET);
    if (pos != 0)
        return;

    for (i=0;i<MAXHIGHSCORES;i++) {
        n = write(fd, highscore[i].name, 3);
        if (n != 3)
            break;
        bigendian_score = highscore[i].score; //htonl(highscore[i].score);
        n = write(fd, &bigendian_score, sizeof(bigendian_score));
        if (n != sizeof(bigendian_score))
            break;
    }
}

int open_high_score_file_and_lose_permissions()
{
/* Here is where you'd open a high score file,
 * lose any setgid permission you might have to access
 * (in case it's a system wide file in /var somewhere) 
 * and read its contents into high_score[] array, 
 *
 * Note: the .score element is a uint32_t, and should be run
 * through ntohl() after reading it in, as the high score file
 * should be endian neutral.
 *
 */

    char *homedir;
    char filename[PATH_MAX+1];
    int i, n, fd;
    off_t pos;
    uint32_t bigendian_score;

    homedir = getenv("HOME");
    if (homedir == NULL)
        return -1;

    sprintf(filename, "%s/.wordwarvi/.highscores", homedir);
    
    /* TODO implement Highscore saving!
    
    fd = open(filename, O_CREAT | O_RDWR | O_BINARY, 0644);
    if (fd < 0)
        return -1;

    pos = lseek(fd, 0, SEEK_SET);
    if (pos != 0) {
        close(fd);
        return -1;
    }

    for (i=0;i<MAXHIGHSCORES;i++) {
        n = read(fd, highscore[i].name, 3);
        if (n != 3)
            break;
        n = read(fd, &bigendian_score, sizeof(bigendian_score));
        if (n != sizeof(bigendian_score))
            break;
        highscore[i].name[3] = '\0';
        highscore[i].score = bigendian_score; //ntohl(bigendian_score);
    }
    */
    return fd;
}

void init_highscores()
{
    int i;
    for (i=0;i<MAXHIGHSCORES;i++) {
        strcpy(highscore[i].name, "vi ");
        highscore[i].score = 666; /* vi vi vi */
    }

    /* put in a few funky scores for laughs. */
    strcpy(highscore[0].name, "RMS"); /* as in RMS. */
    highscore[0].score = 3010700;     
    strcpy(highscore[1].name, "B_M"); /* As in Bram Moolenaar */
    highscore[1].score = 2170300;
    strcpy(highscore[2].name, "JOY"); /* as in Bill */
    highscore[2].score = 2051000;
    strcpy(highscore[2].name, "JWZ"); /* xemacs... */
    highscore[2].score = 2043000;
}

char *trim_whitespace(char *s)
{
    char *x, *z;

    for (x=s;*x == ' ' || *x == '\t'; x++)
        ;
    z = x + (strlen(x) - 1);

    while (z >= x && (*z == ' ' ||  *z == '\t' || *z == '\n')) {
        *z = '\0';
        z--;
    }
    return x;
}

int set_difficulty(char *difficulty)
{
    if (strcmp(difficulty, "easy")==0) {
        game_state.max_player_health = EASY_MAXHEALTH;
        return 0;
    } else if (strcmp(difficulty, "medium")==0) {
        game_state.max_player_health = MEDIUM_MAXHEALTH;
        return 0;
    } else if (strcmp(difficulty, "hard")==0) {
        game_state.max_player_health = HARD_MAXHEALTH;
        return 0;
    } else if (strcmp(difficulty, "insane")==0) {
        game_state.max_player_health = INSANE_MAXHEALTH;
        return 0;
    } else if (strcmp(difficulty, "batshit-insane")==0) {
        game_state.max_player_health = BATSHIT_INSANE_MAXHEALTH;
        return 0;
    }
    return 1;
}

void read_exrc_file(int *bw, int *blueprint, int *retrogreen,
    int *framerate, int *levelwarp, int *randomseed, 
    __attribute__((unused)) int *sounddevice,
    char *joystickdevice)
{
    char line[256];
    char word[256];
    char *homedir;
    char filename[PATH_MAX];
    char js[256];
    FILE *exrcfile;
    char *s;
    int rc;
    int fr, lw, rs, sd, h, w, xa, ya, button;
    int lineno = 0;
    int nstars;
    char keyname[256], actionname[256];
    char difficulty[256];
    char starmotion[256];

    game_state.x_joystick_axis = 0;
    game_state.y_joystick_axis = 1;

    homedir = getenv("HOME");
    if (homedir == NULL)
        return;
    sprintf(filename, "%s/.wordwarvi/.exrc", homedir);

    exrcfile = fopen(filename, "r");
    if (exrcfile == NULL)
        return;

    while (!feof(exrcfile)) {
        s = fgets(line, 256, exrcfile);
        if (s == NULL)
            break;
        s = trim_whitespace(s);
        lineno++;
        if (strcmp(s, "") == 0)
            continue;
        if (s[0] == '#') /* comment? */
            continue;
        if (s[0] == '\"') /* comment? */
            continue;
        rc = sscanf(s, "set framerate=%d\n", &fr);
        if (rc == 1) {
            *framerate = fr;
            continue;
        }
        rc = sscanf(s, "set levelwarp=%d\n", &lw);
        if (rc == 1) {
            *levelwarp = lw;
            continue;
        }
        rc = sscanf(s, "set randomseed=%d\n", &rs);
        if (rc == 1) {
            *randomseed = rs;
            continue;
        }
        rc = sscanf(s, "set sounddevice=%d\n", &sd);
        if (rc == 1) {
            sound_device = sd;
            continue;
        }
        rc = sscanf(s, "set rumbledevice=%s\n", filename);
        if (rc == 1) {
            strcpy(rumbledevicestring, filename);   
            rumbledevice = rumbledevicestring;
            continue;
        }
        rc = sscanf(s, "set starmotion=%s\n", starmotion);
        if (rc == 1) {
            if (strcmp(starmotion, "astronomically-correct") == 0) {
                draw_stars = correct_draw_stars;
                continue;
            }
            if (strcmp(starmotion, "wrong") == 0) {
                draw_stars = incorrect_draw_stars;
                starshift = 3;
                continue;
            }
            if (strcmp(starmotion, "wronger") == 0) {
                draw_stars = incorrect_draw_stars;
                starshift = 2;
                continue;
            }
            if (strcmp(starmotion, "wrongest") == 0) {
                draw_stars = incorrect_draw_stars;
                starshift = 1;
                continue;
            }
            fprintf(stderr, "~/.wordwarvi/.exrc:%d Bad starmotion '%s',"
                " will use default value.\n", lineno, starmotion);
            continue;
        }
        rc = sscanf(s, "set nstars=%d\n", &nstars);
        if (rc == 1) {
            if (nstars <= NSTARS && nstars >= 0) {
                number_of_stars = nstars;
                continue;
            }
            fprintf(stderr, "~/.wordwarvi/.exrc:%d Bad nstars value %d\n", 
                lineno, nstars);
            continue;
        }
        
        rc = sscanf(s, "set height=%d\n", &h);
        if (rc == 1) 
        {
/*        
            real_screen_height = 600;
            current_draw_line = gdk_draw_line_count;
            current_draw_rectangle = gdk_draw_rectangle;
            current_bright_line = unscaled_bright_line;
            if (h >= 100 && h <= 2000) {
                real_screen_height = h;
                current_draw_line = scaled_line;
                current_draw_rectangle = scaled_rectangle;
                current_bright_line = scaled_bright_line;
            } else {
                fprintf(stderr, "~/.wordwarvi/.exrc:%d Bad height using 800.\n", lineno);
                real_screen_width = 800;
                real_screen_height = 600;
                current_draw_line = gdk_draw_line_count;
                current_draw_rectangle = gdk_draw_rectangle;
                current_bright_line = unscaled_bright_line;
            }
*/        
            continue;
        }
        rc = sscanf(s, "set width=%d\n", &w);
        if (rc == 1) 
        {
/*        
            real_screen_width = 800;
            current_draw_line = gdk_draw_line_count;
            current_draw_rectangle = gdk_draw_rectangle;
                current_bright_line = unscaled_bright_line;
            if (w >= 100 && w <= 3000) {
                real_screen_width = w;
                current_draw_line = scaled_line;
                current_draw_rectangle = scaled_rectangle;
                current_bright_line = scaled_bright_line;
            } else {
                fprintf(stderr, "~/.wordwarvi/.exrc:%d: bad width"
                    ", using 800.\n", lineno);
                real_screen_width = 800;
                real_screen_height = 600;
                current_draw_line = gdk_draw_line_count;
                current_draw_rectangle = gdk_draw_rectangle;
                current_bright_line = unscaled_bright_line;
            }
*/        
            continue;
        }
        rc = sscanf(s, "set explosionfactor=%d\n", &xa);
        if (rc == 1) {
            if (xa >= 1 && xa <= 5)
                explosion_factor = xa;
            else
                explosion_factor = 1;
            continue;
        }
        rc = sscanf(s, "set difficulty=%s\n", difficulty);
        if (rc == 1) {
            if (set_difficulty(difficulty) == 0)
                continue;
        }
        rc = sscanf(s, "set joystick-x-axis=%d\n", &xa);
        if (rc == 1 && xa >= -1 && xa < 8) {
            game_state.x_joystick_axis = xa;
            continue;
        }
        rc = sscanf(s, "set joystick-y-axis=%d\n", &ya);
        if (rc == 1 && ya >= -1 && ya < 8) {
            game_state.y_joystick_axis = ya;
            continue;
        }
        rc = sscanf(s, "set joystick=%s\n", js);
        if (rc == 1) {
            strcpy(joystickdevice, js);
            continue;
        }
        rc = sscanf(s, "set %s\n", word);
        /* printf("%s: rc = %d\n", line, rc); */
        if (rc == 1) {
            if (strcmp(word, "xmas") == 0) {
                xmas_mode = !xmas_mode;
                continue;
            }
            if (strcmp(word, "bw") == 0) {
                *bw = 1;
                continue;
            }
            if (strcmp(word, "brightsparks") == 0) {
                brightsparks = 1;
                continue;
            }
            if (strcmp(word, "squareexplosions") == 0) {
                round_explosions = 0;
                spherical_explosions = 0;
                continue;
            }
            if (strcmp(word, "sphericalexplosions") == 0) {
                round_explosions = 0;
                spherical_explosions =1;
                continue;
            }
            if (strcmp(word, "thicklines") == 0) {
                thicklines = 1;
                continue;
            }
            if (strcmp(word, "blueprint") == 0) {
                *blueprint = 1;
                continue;
            }
            if (strcmp(word, "retrogreen") == 0) {
                *retrogreen = 1;
                continue;
            }
            if (strcmp(word, "nostarfield") == 0) {
                want_starfield = 0;
                continue;
            }
            if (strcmp(word, "fullscreen") == 0) {
                fullscreen = 1;
                continue;
            }
            if (strcmp(word, "nomusic") == 0) {
                nomusic = 1;
                continue;
            }
            if (strcmp(word, "nomissilealarm") == 0) {
                want_missile_alarm = 0;
                continue;
            }
            if (strcmp(word, "norumble") == 0) {
                game_state.rumble_wanted = 0;
                continue;
            }
        }
        rc = sscanf(s, "map button %d %s", &button, actionname);
        if (rc == 2) {
            if (remap_joystick_button(button, actionname) == 0)
                continue;
        }
        rc = sscanf(s, "map %s %s", keyname, actionname);
        if (rc == 2) {
            if (remapkey(keyname, actionname) == 0)
                continue;
        }
        fprintf(stderr, "~/.wordwarvi/.exrc: syntax error at line %d:'%s'\n", lineno, line);
    }
    fclose(exrcfile);
}

void check_xmastime()
{
    struct timeval tv;
    struct tm *t;

    xmas_mode = 0;

    gettimeofday(&tv, NULL);
    t = localtime(&tv.tv_sec);
    if (t != NULL) {
        if (t->tm_mon == 11 && t->tm_mday >= 20)
            xmas_mode = 1;
    }
}

void check_for_screensaver()
{
    static char *cmd = "ps -efa | grep screensaver | grep -v grep >/dev/null 2>&1";
    if (system(cmd) != 0)
        return;

    /* if there were a sane, universal means to inhibit screensavers, */
    /* here is where it would go.  Instead, a message: */

    fprintf(stderr, "\n\n\n");
    fprintf(stderr, "  Screen saver and joystick detected.  Since joystick events\n");
    fprintf(stderr, "  aren't going through X11, there is a good chance your screen saver\n");
    fprintf(stderr, "  will unpleasantly interrupt your game.  There are several\n");
    fprintf(stderr, "  popular screen savers (xscreensaver, gnome-screensaver and\n");
    fprintf(stderr, "  kscreensaver being the three main ones).  Each of these\n");
    fprintf(stderr, "  has its own methods by which an application may temporarily\n");
    fprintf(stderr, "  inhibit it.  If the screen saver guys can come up with a sane\n");
    fprintf(stderr, "  method for an application to inhibit a screen saver to which\n");
    fprintf(stderr, "  they can all agree, then I might add such code to wordwarvi.\n");
    fprintf(stderr, "  As things currently stand, you should probably disable your\n");
    fprintf(stderr, "  screen saver manually, by whatever means they've given you,\n");
    fprintf(stderr, "  or at least be prepared to have your game interrupted by the\n");
    fprintf(stderr, "  screen saver from time to time.\n");
    fprintf(stderr, "\n  -- steve\n");
    fprintf(stderr, "\n\n\n");

    /* Another possibility might be to get joystick events through X somehow. */
    /* A quick google indicates there is such a thing as an Xorg X11 joystick */
    /* input driver.  However, getting at this through gtk might not be possible */
    /* and doing plain old X code is not something I want to mess with.  Also, */
    /* strongly suspect that this joystick driver isn't in most distros, and in*/
    /* none which I use.  So this is probably a dead end.  It would probably be */
    /* the cleanest solution in the long run, however, as then the joystick events */
    /* would inhibit the screen saver in precisely the same way that keyboard and */
    /* mouse events do. */

    /* Another possibility would be synthesizing events somehow.  There is a libXTst */
    /* for testing X which could probably be used to synthesize events and thus */
    /* possibly inhibit the screensaver, but I don't really know how to do it. */ 

    /* Then there is crap like what's described here: */
    /* http://www.jwz.org/xscreensaver/faq.html#dvd */
    /* which advocates doing this, triggered by a timer,  while asserting */
    /* that it's fast enough:
 
        if (playing && !paused) {
            system ("xscreensaver-command -deactivate >&- 2>&- &");
        }  */

    /* I should probably cut jwz some slack though, because he's jwz, and because */
    /* he did a lot of work on xemacs.  And it may be fast enough, for all I know */
    /* but I'm not interested in a screen saver specific solution.  This just papers */
    /* over the current mess and makes it seem ok, when it isn't ok. */

    /* Similarly for gnome-screensaver there is a "gnome-screensaver-command --poke"  */
    /* command.  There's quite likely something similar (but different) for kscreensaver. */
    /* which I can't be bothered to look up, on account of I hate KDE, because it */
    /* seems designed by insane people who actually *like* the way the Windows UI works. */

    /* All of that is of course bullshit up with which I will not put.  There needs to be */ 
    /* a universal, screen saver agnostic, sane way that doesn't involve starting a new process. */
    /* Nothing less is acceptable. */

    /* Getting joystick events through X and gtk is probably the */
    /* correct answer (and currently impossible, AFAICT, or at least very */
    /* impractical) */
}

static int setup_openlase(void)
{
#ifdef OPENLASE
/*  
    OLRenderParams params;

    memset(&params, 0, sizeof params);
    params.rate = 48000;
    params.on_speed = 2.0/100.0;
    params.off_speed = 2.0/20.0;
    params.start_wait = 6;
    params.start_dwell = 2;
    params.curve_dwell = 0;
    params.corner_dwell = 5;
    params.curve_angle = cosf(30.0*(M_PI/180.0)); // 30 deg
    params.end_dwell = 2;
    params.end_wait = 6;
    params.snap = 1/100000.0;
    params.render_flags = RENDER_GRAYSCALE; // | RENDER_NOREORDER; 

    if (olInit(3, 60000) < 0) {
        fprintf(stderr, "Failed to initialized openlase\n");
        return -1;
    }
    olSetRenderParams(&params);
*/
    /* window is now 0,0 - 1,1 with y increasing down, x increasing right */
#endif
    return 0;
}

void v_addIRQMultiSample(char *buf, int size, int mode);
void initMultiSamples(int rate);

typedef struct {
    char *name;
    int size;
    unsigned char *data;
} WWVI_OneSample;


WWVI_OneSample wwviSamples[NCLIPS] = 
{
  {"samples/ww_vi/synthetic_laser.raw", 0, 0},  // 0
  {"samples/ww_vi/bombexplosion.raw", 0, 0}, // 1
  {"samples/ww_vi/rocket_exhaust_1.raw", 0, 0}, // 2
  {"samples/ww_vi/flak_gun_sound.raw", 0, 0}, // 3
  {"samples/ww_vi/big_explosion.raw", 0, 0}, // 4
  {"samples/ww_vi/missile_explosion.raw", 0, 0}, // 5
  {"samples/ww_vi/flak_hit.raw", 0, 0}, // 6
  {"samples/ww_vi/new_ground_smack.raw", 0, 0}, // 7
  {"samples/ww_vi/us_quarter.raw", 0, 0}, // 8
  {"samples/ww_vi/lucky13-steve-mono-mix.raw", 0, 0}, // 9
  {"samples/ww_vi/missile_launch_2.raw", 0, 0} , // 10
  {"samples/ww_vi/synthetic_thunder_short.raw", 0, 0}, // 11
  {"samples/ww_vi/dtox3monomix.raw", 0, 0}, // 12
  {"samples/ww_vi/missile_alarm_2.raw", 0, 0}, // 13
  {"samples/ww_vi/toyota_celica_cardoor_sample.raw", 0, 0}, // 14
  {"samples/ww_vi/woohoo.raw", 0, 0}, // 15
  {"samples/ww_vi/ow_my_spine.raw", 0, 0}, // 16
  {"samples/ww_vi/help_down_here.raw", 0, 0}, // 17
  {"samples/ww_vi/synthetic_gunshot_2.raw", 0, 0}, // 18
  {"samples/ww_vi/help_up_here.raw", 0, 0}, // 19
  {"samples/ww_vi/abducted.raw", 0, 0}, // 20
  {"samples/ww_vi/clang.raw", 0, 0}, // 21
  {"samples/ww_vi/fallingscreamhi.raw", 0, 0}, // 22
  {"samples/ww_vi/bodyslam.raw", 0, 0}, // 23
  {"samples/ww_vi/UseTheSource.raw", 0, 0}, // 24
  {"samples/ww_vi/ooooof.raw", 0, 0},// 25
  {"samples/ww_vi/metalbang1.raw", 0, 0},// 26
  {"samples/ww_vi/metalbang2.raw", 0, 0},// 27
  {"samples/ww_vi/metalbang3.raw", 0, 0},// 28
  {"samples/ww_vi/metalbang4.raw", 0, 0},// 29
  {"samples/ww_vi/metalbang5.raw", 0, 0},// 30
  {"samples/ww_vi/metalbang6.raw", 0, 0},// 31
  {"samples/ww_vi/metalbang7.raw", 0, 0},// 32
  {"samples/ww_vi/metalbang8.raw", 0, 0},// 33
  {"samples/ww_vi/stonebang1.raw", 0, 0},// 34
  {"samples/ww_vi/stonebang2.raw", 0, 0},// 35
  {"samples/ww_vi/stonebang3.raw", 0, 0},// 36
  {"samples/ww_vi/stonebang4.raw", 0, 0},// 37
  {"samples/ww_vi/stonebang5.raw", 0, 0},// 38
  {"samples/ww_vi/stonebang6.raw", 0, 0},// 39
  {"samples/ww_vi/stonebang7.raw", 0, 0},// 40
  {"samples/ww_vi/stonebang8.raw", 0, 0},// 41
  {"samples/ww_vi/stonebang8.raw", 0, 0},// 42 // CORROSIVE -> no sound???
  {"samples/ww_vi/volcano_eruption.raw", 0, 0},// 43
  {"samples/ww_vi/aaaah_it_burns.raw", 0, 0},// 44
  {"samples/ww_vi/zzzt.raw", 0, 0},// 45
  {"samples/ww_vi/gravity_bomb.raw", 0, 0},// 46
  {"samples/ww_vi/destiny_facedown.raw", 0, 0},// 47
  {"samples/ww_vi/highscoremusic.raw", 0, 0},// 48
  {"samples/ww_vi/jetwash.raw", 0, 0},// 49
  {"samples/ww_vi/timpani_boing.raw", 0, 0}, // 50
  {"samples/ww_vi/nice_bank_shot.raw", 0, 0},// 51
  {"samples/ww_vi/hohoho.raw", 0, 0},// 52
  {"samples/ww_vi/hohoho_merry_xmas.raw", 0, 0},// 53
  {"samples/ww_vi/yay_santa.raw", 0, 0},// 54
  {"samples/ww_vi/radar_fail.raw", 0, 0},// 55
  {"samples/ww_vi/radar_ready.raw", 0, 0},// 56
  {"samples/ww_vi/gunwheel_sound.raw", 0, 0},// 57
  {"samples/ww_vi/tesla_tower.raw", 0, 0}// 58
};

void loadOneSamples(WWVI_OneSample *sample)
{
  FILE *fileRead;
  fileRead = fopen(sample->name, "rb");
  if (fileRead == 0) return;
  int size = __filelength(fileRead);
  unsigned char * data = (unsigned char *) malloc(size);
  if (data == 0)
  {
    fclose(fileRead);
    return;
  }
  fclose(fileRead);
  
  int size2 = v_loadRAW(sample->name, data);
  if (size2 != size) return;
  
  sample->data = data;
  sample->size = size;
}
void loadAllSamples()
{
  for (int i=0;i<NCLIPS;i++)
    loadOneSamples(&wwviSamples[i]);
}

int wwviaudio_add_sound(int which_sound)
{
  if (wwviSamples[which_sound].size != 0)
    v_addIRQMultiSample(wwviSamples[which_sound].data, wwviSamples[which_sound].size, PLAY_ONCE);
  return 0;
}

int wwviaudio_play_music(int which_sound)
{
  if (wwviSamples[which_sound].size != 0)
;//    v_addIRQMultiSample(wwviSamples[which_sound].data, wwviSamples[which_sound].size, PLAY_ONCE);
  return 0;
}


int main(int argc, char *argv[])
{
    /* GtkWidget is the storage type for widgets */
//    GdkRectangle cliprect;
    int i;
    int blueprint = 0;
    int retrogreen = 0;
    int opt;
    char joystick_device[PATH_MAX+1];

    struct timeval tm;
    char difficulty[256];

    
  vectrexinit(1);
  v_init();

  
  v_noSound();
  // special vectrex  interface settings for asteroids
  v_setRefresh(50);
  v_setClientHz(50); 
  v_setupIRQHandling();
  v_enableJoystickDigital(1,1,0,0);
  v_setCustomClipping(1, -13000, Y_LOW_CLIP, 13000, Y_HIGH_CLIP);

  sizeX = 1.4;
  sizeY = 1.4;
  
  v_enableSoundOut(1);
  v_enableButtons(1);


  loadAllSamples();
  initMultiSamples(12000);
//  v_addIRQMultiSample(destiny_facedownSample, destiny_facedownSize, PLAY_ONCE);
  
  
//  keepDotsTogether = 0;
//  useDoubleTimer = 1;
//  checkExternal = checkForSpecialVectors;
//  useSamples = 1;

//  int pv = 0;
//  if (pv = ini_parse("asteroid_sbt.ini", astIniHandler, 0) < 0)  
//    printf("asteroid_sbt.ini not loaded!\n\r");
//  else printf("asteroid_sbt.ini loaded!\n\r");

    
    
    // todo
//    check_xmastime();
    setup_openlase();
    







    init_highscores();
    high_score_file_descriptor = open_high_score_file_and_lose_permissions();

//    strcpy(joystick_device, JOYSTICK_DEVNAME);
    real_screen_width = SCREEN_WIDTH;
    real_screen_height = SCREEN_HEIGHT;
    frame_rate_hz = FRAME_RATE_HZ;
    /* current_draw_line = crazy_line;*/
    /* current_draw_rectangle = crazy_rectangle;*/

    init_keymap();
    game_state.max_player_health = MEDIUM_MAXHEALTH;
    game_state.rumble_wanted = 1;
    read_exrc_file(&no_colors_any_more, &blueprint, &retrogreen, &frame_rate_hz, &levelwarp, &initial_random_seed, &sound_device, joystick_device);

    while (1) {
        int rc, n; 
        rc = getopt_long_only(argc, argv, "", wordwarvi_options, &opt);
        if (rc == -1)
            break;
        switch (rc) {
            case 0: no_colors_any_more = 1; /* --bw option */
                break;

            case 1: n = sscanf(optarg, "%d", &sound_device); /* --sounddevice option */
                if (n != 1) {
                    fprintf(stderr, "wordwarvi: Bad sound device argument"
                        " '%s', using 0.\n", optarg);
                    sound_device = 0;
                }
                break;
            case 2: /* --version option */
                printf("Wordwarvi, version %s, (c) 2007,2008 Stephen M. Cameron.\n",
                    WORDWARVI_VERSION);
                printf("Released under the GNU GPL v. 2.0 or later.  See the file\n");
                printf("COPYING, which should have accompanied this program, for\n");
                printf("information about redistributing this program.\n");
                printf("See http://smcameron.github.com/wordwarvi for more information\n");
                printf("about this program.\n");
                exit(0);
            case 3: /* brightsparks */
                brightsparks = 1;
                break;
            case 4: /* width */
/*                
                real_screen_width = 800;
                current_draw_line = gdk_draw_line_count;
                current_draw_rectangle = gdk_draw_rectangle;
                current_bright_line = unscaled_bright_line;
                n = sscanf(optarg, "%d", &real_screen_width);
                if (n != 1) {
                    fprintf(stderr, "wordwarvi: Bad width argument"
                        " '%s', using 800.\n", optarg);
                    real_screen_width = 800;
                    current_draw_line = gdk_draw_line_count;
                    current_draw_rectangle = gdk_draw_rectangle;
                    current_bright_line = unscaled_bright_line;
                    break;
                }
                if (real_screen_width >= 100 && real_screen_width <= 3000) {
                    current_draw_line = scaled_line;
                    current_draw_rectangle = scaled_rectangle;
                    current_bright_line = scaled_bright_line;
                } else {
                    fprintf(stderr, "wordwarvi: Bad width argument"
                        " '%s', using 800.\n", optarg);
                    real_screen_width = 800;
                    real_screen_height = 600;
                    current_draw_line = gdk_draw_line_count;
                    current_draw_rectangle = gdk_draw_rectangle;
                    current_bright_line = unscaled_bright_line;
                }
*/                
                break;  
            case 5: /* height */
/*              
                real_screen_height = 600;
                current_draw_line = gdk_draw_line_count;
                current_draw_rectangle = gdk_draw_rectangle;
                current_bright_line = unscaled_bright_line;
                n = sscanf(optarg, "%d", &real_screen_height);
                if (n != 1) {
                    fprintf(stderr, "wordwarvi: Bad height argument"
                        " '%s', using 600.\n", optarg);
                    real_screen_height = 600;
                    current_draw_line = gdk_draw_line_count;
                    current_draw_rectangle = gdk_draw_rectangle;
                    current_bright_line = unscaled_bright_line;
                    break;
                } 
                if (real_screen_height >= 100 && real_screen_height <= 2000) {
                    current_draw_line = scaled_line;
                    current_draw_rectangle = scaled_rectangle;
                    current_bright_line = scaled_bright_line;
                } else {
                    fprintf(stderr, "wordwarvi: Bad height argument"
                        " '%s', using 800.\n", optarg);
                    real_screen_width = 800;
                    real_screen_height = 600;
                    current_draw_line = gdk_draw_line_count;
                    current_draw_rectangle = gdk_draw_rectangle;
                    current_bright_line = unscaled_bright_line;
                }
*/                
                break;  
            case 6: /* frame rate */
                n = sscanf(optarg, "%d", &frame_rate_hz);
                if (n != 1 || frame_rate_hz < 3 || frame_rate_hz > 300) {
                    fprintf(stderr, "wordwarvi: Bad framerate argument"
                        " '%s', using default %d frames/sec.\n", 
                            optarg, FRAME_RATE_HZ);
                    frame_rate_hz = FRAME_RATE_HZ;
                }
                break;
            case 7: /* --nostarfield */
                want_starfield = 0;
                break;
            case 8: /* --blueprint */
                blueprint = 1;
                break;
            case 9: /* --fullscreen */
                fullscreen = 1;
                break;
            case 10: /* --fullscreen */
                nomusic = 1;
                break;
            case 11: /* --fullscreen */
                strncpy(joystick_device, optarg, PATH_MAX); 
                break;
            case 12: /* --retrogreen */
                retrogreen = 1;
                break;
            case 13: /* --randomize */
                gettimeofday(&tm, NULL);
                initial_random_seed = tm.tv_usec;   
                break;
            case 14: /* --randomseed n */
                n = sscanf(optarg, "%d", &initial_random_seed);
                if (n != 1) {
                    fprintf(stderr, "Bad random seed value '%s'\n", optarg);
                    exit(1);
                }
                break;
#ifdef LEVELWARP
            case 15: /* --levelwarp */
                n = sscanf(optarg, "%d", &levelwarp);
                if (n != 1) {
                    fprintf(stderr, "Bad level warp value '%s'\n", optarg);
                    exit(1);
                }
                break;
#endif
            case 16: /* no missile alarm */
                want_missile_alarm = 0;
                break;
            case 17: /* difficulty */
                n = sscanf(optarg, "%s", difficulty);
                if (n != 1) {
                    fprintf(stderr, "Bad difficulty level setting\n");
                    exit(1);
                }
                if (set_difficulty(difficulty) != 0) {
                    fprintf(stderr, "Bad difficulty level setting: '%s'\n", difficulty);
                    exit(1);
                }
                break;
            case 18: /* norumble */
                game_state.rumble_wanted = 0;
                break;
            case 19: /* rumbledevice */
                n = sscanf(optarg, "%s", rumbledevicestring);
                if (n != 1) {
                    fprintf(stderr, "Bad rumbledevice setting\n");
                    exit(1);
                }
                rumbledevice = rumbledevicestring;
                break;
            case 20:{
                    char starmotion[256];
                    n = sscanf(optarg, "%s", starmotion);
                    if (n != 1) {
                        fprintf(stderr, "Bad starmotion value, will use default value.\n");
                        break;
                    }
                    if (strcmp(starmotion, "astronomically-correct") == 0) {
                        draw_stars = correct_draw_stars;
                        break;
                    }
                    if (strcmp(starmotion, "wrong") == 0) {
                        draw_stars = incorrect_draw_stars;
                        starshift = 3;
                        break;
                    }
                    if (strcmp(starmotion, "wronger") == 0) {
                        draw_stars = incorrect_draw_stars;
                        starshift = 2;
                        break;
                    }
                    if (strcmp(starmotion, "wrongest") == 0) {
                        draw_stars = incorrect_draw_stars;
                        starshift = 1;
                        break;
                    }
                    fprintf(stderr, "Bad starmotion '%s',"
                        " will use default value.\n", starmotion);
                    break;
                }
            case 21:{
                    int nstars;
                    n = sscanf(optarg, "%d", &nstars);
                    if (n == 1) {
                        if (nstars >= 0 && nstars <= NSTARS) {
                            number_of_stars = nstars;
                            break;
                        }
                    }
                    fprintf(stderr, "Bad nstars value\n");
                    break;
                }
            case 22: /* thicklines */
                thicklines = 1;
                break;
            case 23: /* squareexplosions */
                round_explosions = 0;
                spherical_explosions = 0;
                break;
            case 24:/* explosionfactor */ {
                    int x;
                    n = sscanf( optarg, "%d", &x);
                    if (n == 1) {
                        if (x >= 1 && x <= 5) {
                            explosion_factor = x;
                            break;
                        }
                    }
                    explosion_factor = 1;
                    fprintf(stderr, "Bad explosionfactor value.\n");
                    break;
                }
            case 25: /* --xmas */ xmas_mode = !xmas_mode;
                break;
            case 26: /* sphericalexplosions */
                round_explosions = 0;
                spherical_explosions = 1;
                break;
            case 27:  /* help. */ usage();
            case '?':usage(); /* exits. */
            default:printf("Unexpected return value %d from getopt_long_only()\n", rc);
                exit(0);
        }
    }

    level.random_seed = initial_random_seed;

//   set_joystick_x_axis(game_state.x_joystick_axis);
//    set_joystick_y_axis(game_state.y_joystick_axis);

#ifdef WITHAUDIOSUPPORT
    if (nomusic)
        wwviaudio_set_nomusic();
    wwviaudio_set_sound_device(sound_device);
    if (wwviaudio_initialize_portaudio(MAX_CONCURRENT_SOUNDS, NCLIPS) != 0)
        printf("Guess sound's not working...\n");
    else
        init_clips();
#endif
/*
    gtk_set_locale();
    gtk_init (&argc, &argv);
   
    gdk_color_parse("white", &huex[WHITE]);
    gdk_color_parse("blue", &huex[BLUE]);
    gdk_color_parse("black", &huex[BLACK]);
    gdk_color_parse("green", &huex[GREEN]);
    gdk_color_parse("darkgreen", &huex[DARKGREEN]);
    gdk_color_parse("yellow", &huex[YELLOW]);
    gdk_color_parse("red", &huex[RED]);
    gdk_color_parse("orange", &huex[ORANGE]);
    gdk_color_parse("cyan", &huex[CYAN]);
    gdk_color_parse("MAGENTA", &huex[MAGENTA]);
*/
    /* Set up the spark colors. */
    setup_spark_colors();
    setup_rainbow_colors();

    if (no_colors_any_more)
        paint_it_black();
    if (blueprint)
        paint_it_blue();
    if (retrogreen)
        paint_it_green();
 
    if (game_state.rumble_wanted) {
        if (get_ready_to_rumble(rumbledevice) != 0) {
            printf("No rumble...\n");
            game_state.rumble_wanted = 0;
        }
    } 

    /* create a new window */
//    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

/*
    if (fullscreen) {
        // This overrides --width and --height options 
        GdkScreen* screen = NULL;

        screen = gtk_window_get_screen(GTK_WINDOW(window));

        gtk_window_fullscreen(GTK_WINDOW (window));
        gtk_window_set_decorated( GTK_WINDOW (window), FALSE); 

        real_screen_width = gdk_screen_get_width(screen);
        real_screen_height = gdk_screen_get_height(screen);

        current_draw_line = scaled_line;
        current_draw_rectangle = scaled_rectangle;
        current_bright_line = scaled_bright_line;
    }
*/    
    xscale_screen = (float) real_screen_width / (float) SCREEN_WIDTH;
    yscale_screen = (float) real_screen_height / (float) SCREEN_HEIGHT;
    
    /* When the window is given the "delete_event" signal (this is given
     * by the window manager, usually by the "close" option, or on the
     * titlebar), we ask it to call the delete_event () function
     * as defined above. The data passed to the callback
     * function is NULL and is ignored in the callback function. */
//    g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (delete_event), NULL);
    
    /* Here we connect the "destroy" event to a signal handler.  
     * This event occurs when we call gtk_widget_destroy() on the window,
     * or if we return FALSE in the "delete_event" callback. */
//    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);
   
#if 0 
    /* Sets the border width of the window. */
    if (!fullscreen)
        /* this throws off gtk_window_get_size to see what size*/
        /* we ACTUALLY got, so, don't do it.*/
        gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    else
        gtk_container_set_border_width (GTK_CONTAINER (window), 0);
#endif
//    gtk_container_set_border_width (GTK_CONTAINER (window), 0);
   
//    vbox = gtk_vbox_new(FALSE, 0); 
//    main_da = gtk_drawing_area_new();
//    gtk_widget_modify_bg(main_da, GTK_STATE_NORMAL, &huex[WHITE]);
    /* gtk_widget_set_size_request(main_da, real_screen_width, real_screen_height);*/

    
// todo
// ENTER CONFIG ETC    
    
    
    
//    g_signal_connect(G_OBJECT (main_da), "expose_event", G_CALLBACK (main_da_expose), NULL);
//    g_signal_connect(G_OBJECT (main_da), "configure_event", G_CALLBACK (main_da_configure), NULL);

//    gtk_container_add (GTK_CONTAINER (window), vbox);
//    gtk_box_pack_start(GTK_BOX (vbox), main_da, TRUE /* expand */, TRUE /* fill */, 0);
    init_score_table(); 
    init_vects();
    init_vxy_2_dxy();
    init_object_numbers();



// todo
// ENTER KEY EVENTS
//    g_signal_connect(G_OBJECT (window), "key_press_event", G_CALLBACK (key_press_cb), "window");
//    g_signal_connect(G_OBJECT (window), "key_release_event", G_CALLBACK (key_release_cb), "window");


    /* print_target_list();*/

//    for (i=0;i<NCOLORS+NSPARKCOLORS + NRAINBOWCOLORS;i++)
//        gdk_colormap_alloc_color(gtk_widget_get_colormap(main_da), &huex[i], FALSE, FALSE);
//    gtk_widget_modify_bg(main_da, GTK_STATE_NORMAL, &huex[BLACK]);


    v_setClientHz(frame_rate_hz); 
    make_debris_forms();
    init_levels_to_beginning();
    initialize_game_state_new_level();
    init_radar_noise();
    game_state.score = 0;
    game_state.sound_effects_on = 1;
    wwviaudio_resume_sound_effects();
    start_level();

/*    
    gtk_window_set_default_size(GTK_WINDOW(window), real_screen_width, real_screen_height);

    gtk_widget_show (vbox);
    gtk_widget_show (main_da);
    
    // and the window 
    gtk_widget_show (window);
    set_cursor(window);
    
    inhibit_screensaver(window);


    jsfd = open_joystick(joystick_device, window->window);
    if (jsfd < 0) {
        printf("No joystick...\n");
    } else
        check_for_screensaver();
    gc = gdk_gc_new(GTK_WIDGET(main_da)->window);
    gdk_gc_set_foreground(gc, &huex[BLUE]);
    gdk_gc_set_foreground(gc, &huex[WHITE]);

    gdk_gc_set_clip_origin(gc, 0, 0);
    cliprect.x = 0; 
    cliprect.y = 0; 
    cliprect.width = real_screen_width; 
    cliprect.height = real_screen_height;   
    gdk_gc_set_clip_rectangle(gc, &cliprect);

    gtk_window_get_size(GTK_WINDOW (window), &real_screen_width, &real_screen_height);
    if (real_screen_width == 800 && real_screen_height == 600) {
        current_draw_line = gdk_draw_line_count;
        current_draw_rectangle = gdk_draw_rectangle;
        current_bright_line = unscaled_bright_line;
    } else {
        current_draw_line = scaled_line;
        current_draw_rectangle = scaled_rectangle;
        current_bright_line = scaled_bright_line;
    }
*/
    jsfd = 1;
    real_screen_width = SCREEN_WIDTH;
    real_screen_height= SCREEN_HEIGHT;

    xscale_screen = (float) real_screen_width / (float) SCREEN_WIDTH;
    yscale_screen = (float) real_screen_height / (float) SCREEN_HEIGHT;


    if (thicklines)
        current_draw_line = thick_scaled_line;

    explosion = explode;
    bright_explosion = bright_explode;
    if (round_explosions) {
        bright_explosion = bright_round_explode;
        explosion = round_explode;
    }
    if (spherical_explosions) {
        bright_explosion = bright_sphere_explode;
        explosion = sphere_explode;
    }

//    timer_tag = g_timeout_add(1000 / frame_rate_hz, advance_game, NULL);

//#ifndef GLIB_VERSION_2_32
//    /* this is only needed in glibc versions before 2.32 */
//    
//    /* Apparently (some versions of?) portaudio calls g_thread_init(). */
//    /* It may only be called once, and subsequent calls abort, so */
//    /* only call it if the thread system is not already initialized. */
//    if (!g_thread_supported ()) 
//    g_thread_init(NULL);
//#endif
//
//    gdk_threads_init();

    gettimeofday(&start_time, NULL);

    /* All GTK applications must have a gtk_main(). Control ends here
     * and waits for an event to occur (like a key press or
     * mouse event). */
//    gtk_main ();



  while (1)
  {
    advance_game();
    main_da_expose();
//    v_playAllSFX();
    v_doSound();    // not needed in IRQ Mode
    v_readButtons(); // not neededin IRQ mode
    v_readJoystick1Digital();
    v_WaitRecal();
//printf(".");
      
  }
    
    
    
    
    
    
    openlase_shutdown();
    wwviaudio_stop_portaudio();
    free_debris_forms();
//    close_joystick();
    close_rumble_fd();
    return 0;
}












struct wwvi_js_event {
    int button[11];
    int stick_x;
    int stick_y;
};

void deal_with_joystick()
{
  // todo handle joystick!
  
 
    int rc;
    // why can I get away with this kind of initializer 
    // in the linux kernel, but not here?  Well, not without the 
    // compiler moaning, anyway. 
    static struct wwvi_js_event jse; 
    int index = 0;
    int newvy, diff;
    static int next_joystick_motion_timer = 0;
    static int next_joystick_button_timer = 0;
    int  i;
    int moved;

    int do_bomb = 0;
    int do_droppresent = 0;
    int do_quarter = 0;
    int do_laser = 0;
    int do_chaff = 0;
    int do_gbomb = 0;
    int do_thrust = 0;
    int do_reverse = 0;
    int do_suicide = 0;

    int *xaxis = &jse.stick_x;
    int *yaxis = &jse.stick_y;
    

    xaxis = &jse.stick_x;
    yaxis = &jse.stick_y;

#define JOYSTICK_SENSITIVITY 1
#define XJOYSTICK_THRESHOLD 1

    memset(&jse.button[0], 0, sizeof(jse.button[0]*10));
//    rc = get_joystick_status(&jse);
//    if (rc != 0) return;

jse.button[0] = (currentButtonState & 1)?1:0; // present
jse.button[1] = (currentButtonState & 4)?1:0; // bomb
jse.button[8] = (currentButtonState & 8)?1:0; // laser / credit
jse.button[9] = (currentButtonState & 2)?1:0; // gravity bomb
    jse.stick_x = currentJoy1X;
    jse.stick_y = -currentJoy1Y;


    if (in_the_process_of_quitting) 
    {
        if (*xaxis <= -XJOYSTICK_THRESHOLD) 
        {
            last_user_input_time = timer;
            current_quit_selection = 1;
        } 
        else if (*xaxis >= XJOYSTICK_THRESHOLD) 
        {
                last_user_input_time = timer;
                current_quit_selection = 0;
        }
        for (i=0;i<10;i++)
        {
          
            if (jse.button[i] == 1 && jsbuttonaction[i] == keylaser) 
            {
                final_quit_selection = current_quit_selection;
                if (!final_quit_selection) 
                {
                    in_the_process_of_quitting = 0;
                    last_user_input_time = timer;
                    wwviaudio_resume_audio();
                    // prevent just selecting "don't quit" from 
                    // putting in a quarter. 
                    next_quarter_time = timer + frame_rate_hz;
                }
            }
        }
        return;
    }

    if (game_pause_help) 
    {
        return;
    }

    highscore_buttonpress = 0;
    
    if (timer_event == NEW_HIGH_SCORE_EVENT) 
    {
        moved = 0;
    
        // user is entering high scores. 
        if (timer > next_joystick_motion_timer) 
        {
            next_joystick_motion_timer = timer + (frame_rate_hz >> 3);
            if (*xaxis <= -XJOYSTICK_THRESHOLD) 
            {
                highscore_letter--;
                if (highscore_letter < 0)
                    highscore_letter = 26;
                moved = 1;
            }
            if (*xaxis >= XJOYSTICK_THRESHOLD) 
            {
                highscore_letter++;
                if (highscore_letter > 26)
                    highscore_letter = 0;
                moved = 1;
            }
            if (*yaxis <= -XJOYSTICK_THRESHOLD) 
            {
                highscore_letter -= 9;
                if (highscore_letter < 0)
                    highscore_letter += 27;
                moved = 1;
            }
            if (*yaxis >= XJOYSTICK_THRESHOLD) 
            {
                highscore_letter += 9;
                if (highscore_letter > 26)
                    highscore_letter -= 27;
                moved = 1;
            }
        }
        if (timer > next_joystick_button_timer) 
        {
            next_joystick_button_timer = timer + (frame_rate_hz >> 3);
            for (i=0;i<10;i++) 
            {
                if (jse.button[i] && jsbuttonaction[i] == keylaser) 
                {
                    highscore_buttonpress = 1;
                    if (highscore_letter >= 0 && highscore_letter < 26) 
                    {
                        highscore_initials[highscore_current_initial] = highscore_letter + 'A';
                        highscore_current_initial++;
                    } else if (highscore_letter == 26) { // done. 
                        highscore_current_initial = 3; // ends it. 
                    }
                    moved = 1;
                }
                if (jse.button[i] && jsbuttonaction[i] == keyquit) 
                {
                    in_the_process_of_quitting = !in_the_process_of_quitting;
                    if (in_the_process_of_quitting)
                        wwviaudio_pause_audio();
                    else
                        wwviaudio_resume_audio();
                }
            }
        }
        if (moved) 
        {
            last_user_input_time = timer; 
            wwviaudio_add_sound(PLAYER_LASER_SOUND);
        }
        return;
    }

    if (game_state.health <= 0 && credits >= 1) 
    {
        return;
    }

    if (credits <= 0) {
        if (*xaxis <= -XJOYSTICK_THRESHOLD ||
            *xaxis >= XJOYSTICK_THRESHOLD ||
            *yaxis <= -XJOYSTICK_THRESHOLD ||
            *yaxis >= XJOYSTICK_THRESHOLD)
            last_user_input_time = timer; 
        for (i=0;i<10;i++)
            if (jse.button[i])
                last_user_input_time = timer; 
        goto no_credits;
    }

    // xaxis, horizontal movement
    if (*xaxis <= -XJOYSTICK_THRESHOLD) 
    {
        if (game_state.direction != -1) 
        {
            player->vx = player->vx / 2;
            game_state.direction = -1;
            set_player_vect();
        } 
        else if (abs(player->vx + game_state.direction) < MAX_VX)
            player->vx += game_state.direction;
        
        last_user_input_time = timer; 
    } 
    else if (*xaxis >= XJOYSTICK_THRESHOLD) 
    {
        if (game_state.direction != 1) 
        {
            player->vx = player->vx / 2;
            game_state.direction = 1;
            set_player_vect();
        } 
        else if (abs(player->vx + game_state.direction) < MAX_VX)
            player->vx += game_state.direction;
        last_user_input_time = timer; 
    } 
    else 
    {
        ;
    }

    // yaxis vertical movement 
    if (*yaxis >= JOYSTICK_SENSITIVITY) 
    {
/*      
        if (player->vy < MAX_VY)
            index = nyjstable * *yaxis / 32767;
        if (index < 0)
            index = 0;
        else if (index > nyjstable-1)
            index = nyjstable-1;
        newvy = yjstable[index];
        diff = newvy - player->vy;
        if (abs(diff) > 4)
            player->vy = player->vy + (diff >> 1);
        else 
*/          
            player->vy = player->vy + 2;
        last_user_input_time = timer; 
    } 
    else if (*yaxis <= -JOYSTICK_SENSITIVITY) 
    {
/*      
        if (player->vy > -MAX_VY)
            index = -nyjstable * *yaxis / 32767;
        if (index < 0)
            index = 0;
        else if (index > nyjstable-1)
            index = nyjstable-1;
        newvy = -yjstable[index];
        diff = newvy - player->vy;
        if (abs(diff) > 4)
            player->vy = player->vy + (diff >> 1);
        else 
*/          
            player->vy = player->vy - 2;
        last_user_input_time = timer; 
    } 
    else 
    {
        if (player->vy > 0)
            player->vy--;
        else if (player->vy < 0)
            player->vy++;
    }

    for (i=0;i<11;i++) 
    {
        if (jse.button[i] == 1) 
        {
            last_user_input_time = timer; 
            switch(jsbuttonaction[i]) 
            {
              case keysuicide:
                  do_suicide = 1;
              case keylaser:
                  do_laser = 1;
                  break;
              case keybomb:
                  do_bomb = 1;
                  break;
              case key_droppresent:
                  do_droppresent = 1;
                  break;
              case keypause:
                  break;
              case keypausehelp:
                  break;
              case keychaff:
                  do_chaff = 1;
                  break;
              case keygravitybomb:
                  do_gbomb = 1;
                  break;
              case keyquarter:
                  do_quarter = 1;
                  break;
              case keythrust:
                  do_thrust = 1;
                  break;
              case keyquit:
                  in_the_process_of_quitting = !in_the_process_of_quitting;
                  if (in_the_process_of_quitting)
                      wwviaudio_pause_audio();
                  else
                      wwviaudio_resume_audio();
                  break;
              case keytogglemissilealarm:
                  want_missile_alarm = !want_missile_alarm;
                  break;
              case keymusic:
                  wwviaudio_toggle_music();
                  break;
              case keyreverse:
                  do_reverse = 1;
                  break;
              case keysoundeffects:
                  game_state.sound_effects_on = !game_state.sound_effects_on;
                  game_state.sound_effects_manually_turned_off = (!game_state.sound_effects_on);
                  wwviaudio_toggle_sound_effects();
                  break;
              default:
                  break;
            }
        }
    }
            
    // Buttons... 
    if (do_bomb)
        drop_bomb();

    if (do_droppresent && xmas_mode)
        drop_present();

    if (do_laser)
        player_fire_laser();

    if (do_chaff)
        drop_chaff();

    if (do_gbomb)
        drop_gravity_bomb();

    if (do_reverse) 
    {
        if (timer > next_joystick_button_timer) 
        {
            game_state.direction = -game_state.direction;
            set_player_vect();
            next_joystick_button_timer = timer + frame_rate_hz / 8;
        }
    }

    if (do_thrust)
        if (abs(player->vx + game_state.direction) < MAX_VX)
            player->vx += game_state.direction;

    if ((do_quarter) && timer > next_quarter_time) {
        insert_quarter();
        next_quarter_time = timer + (frame_rate_hz);
    }

    if (do_suicide) {
        if (credits > 0)
            game_state.health = -1;
    }
    return;

no_credits:

    // check to see if quit button pressed. 
    for (i=0;i<10;i++) {
        if (jse.button[i] && jsbuttonaction[i] == keyquit) {
            in_the_process_of_quitting = 1;
            wwviaudio_pause_audio();
            return;
        }
    }

    // If credits are zero, or health is zero -- ANY button on the joystick 
    // will put in a quarter. 
    if (timer > next_quarter_time) {
        for (i=0;i<10;i++)
            if (jse.button[i]) {
                insert_quarter();
                next_quarter_time = timer + frame_rate_hz;
                break;
            }
    }
}
