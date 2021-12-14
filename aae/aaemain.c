/*
Omega race not working in demo (ONLY on pitrex, windows is ok!)

4 button with AAE OFTEN crashes! why? memory overwrite? IRQ non reset?

EVERY game
every now and than draws everything TWICE in each frame,
this has to do with the video ticks + count
sometimes the DVG is just called twice per round

that must be prohibited!!!

config whether game is y switched / x switched...

VBREAK not working

*/
#include <stdio.h>
#include <stdlib.h>

#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

#include "aaemain.h"



#include "loaders.h"
#include "globals.h"
#include "input.h"

#include "starwars.h"
#include "mhavoc.h"
#include "omegrace.h"
#include "cinemat.h"
#include "asteroid.h"
#include "quantum.h"
#include "llander.h"
#include "bzone.h"
#include "spacduel.h"
#include "tempest.h"
#include "SegaG80.h"

#include "gameroms.h"
#include "gamekeys.h"
#include "gamedips.h"
#include "gamesamp.h"
#include "gameart.h"
#include "dips.h"
#include "samples.h"
#include "driver.h"
#include "cpu_control.h"
#include "aae_avg.h"

 UINT32 dwElapsedTicks;
 glist gamelist[256];

 GAMEKEYS *MK;
 GAMEKEYS *GK;
 GAMEKEYS *FOO;

//RAM Variables
 unsigned char *membuffer;
 unsigned char vec_ram[0x1fff];
 unsigned char *GI[5]; //Global 6502/Z80/6809 GameImage 

 CONTEXTM6502 *c6502[MAX_ACPU];
 CONTEXTMZ80 cMZ80[MAX_ACPU];

 int art_loaded[6];

//TEMPORARY GRAPHICS GLOBALS
 int msx,msy,esx,esy; //Main full screen adjustments for debug
 int b1sx,b1sy,b2sx,b2sy; //bezel full screen adjustments
 float bezelzoom;
 int bezelx;
 int bezely;
 float overalpha;
 struct game_rect GameRect;

//GLOBAL AUDIO VARIABLES
 int gammaticks; //Needed for Pokey Sound for Major Havoc
 int chip;  //FOR POKEY            
 int gain;  //FOR POKEY  
 int BUFFER_SIZE;  //FOR POKEY

//SAMPLE *game_sounds[60]; //Global Samples
//AUDIOSTREAM *stream; //Global Streaming Sound 1
//AUDIOSTREAM *stream2; //Global Streaming Sound 2
//unsigned char  *soundbuffer;
//signed char *aybuffer;




 int in_gui;
 int frames; //Global Framecounter
 int frameavg;
 int testsw; //testswitch for many games

//Shared variable for GUI

 int gamenum; //Global Gamenumber (really need this one)
 int have_error; //Global Error handler
 int showinfo; //Global info handler
 int done; //End of emulation indicator
 int paused; //Paused indicator
 double fps_count; //FPS Counter
 int showfps;   //ShowFPS Toggle
 int show_menu; //ShowMenu Toggle
 int showifo; //No clue what this does
 int scalef; //SCALING FACOR FOR RASTER GAMES
 int gamefps; //GAME REQUIRED FPS
 int num_games; //Total number of games ?? needed?
 int num_samples; //Total number of samples for selected game

//KEY VARIABLES
 int mouseb[5];
 int WATCHDOG;
 int menulevel;//Top Level
 int menuitem; //TOP VAL
 int key_set_flag;
 int total_length;

 colors vec_colors[1024];

//Video VARS
 int sx,ex,sy,ey;
 int testblend;
 aae_settings config;


struct AAEDriver driver[] =
{
        { 
        "llander1", "Lunar Lander (Revision 1)", llander, 
        &init_llander,0, &run_llander,&end_llander,     
        llander1_dips,llander_keys,
        llander_samples, noart,
        {CPU_6502,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {6,0,0,0},
        {1,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0}, 
        40,VEC_BW_16,0,
        {0,1024,0,812} 
        },
        { 
        "llander", "Lunar Lander (Revision 2)", llander, 
        &init_llander,0,&run_llander,&end_llander, 
        llander_dips,llander_keys,
        llander_samples, noart,
        {CPU_6502,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {6,0,0,0},
        {1,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0}, 
        40,VEC_BW_16,0,
        {0,1024,0,812} 
        },
       
        
        { 
        "meteorts", "Meteorites (Asteroids Bootleg)", meteorts, 
        &init_asteroid,0,&run_asteroids,&end_asteroids,
        asteroid_dips,asteroid_keys,
        asteroidsamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {4,0,0,0},
        {1,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0}, 
        60,VEC_BW_16 ,0,
        {0,1024,0,812}
        },
        {
        "asterock", "Asterock (Asteroids Bootleg)", asterock,
        &init_asteroid,0,&run_asteroids, &end_asteroids, 
        asteroid_dips,asteroid_keys,
        asteroidsamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {4,0,0,0},
        {1,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0},
        60,VEC_BW_16,0,
        {0,1024,0,812} 
        },
        {
        "asteroib", "Asteroids (Bootleg on Lunar Lander Hardware)",  asteroib, 
        &init_asteroid,0,&run_asteroids,&end_asteroids, 
        asteroid_dips,asteroid_keys,
        asteroidsamples,noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {4,0,0,0},
        {1,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0},
        60,VEC_BW_16,0,
        {0,1024,0,812} 
        },
        {
        "asteroi1", "Asteroids (Revision 1)", asteroi1,
        &init_asteroid,0,&run_asteroids,&end_asteroids, 
        asteroid_dips,asteroid_keys,
        asteroidsamples,noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {4,0,0,0},
        {1,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0},
        60,VEC_BW_16,0,
        {0,1024,0,812}
        },
        { "asteroid", "Asteroids (Revision 2)", asteroid 
        ,&init_asteroid,0, &run_asteroids,&end_asteroids,
        asteroid_dips,asteroid_keys,
        asteroidsamples,noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {4,0,0,0},
        {1,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0},
        60,VEC_BW_16,0,
        {0,1024,0,812}
        },
        { 
        "astdelu1", "Asteroids Deluxe (Revision 1)", astdelu1, 
        &init_astdelux,0, &run_asteroids,&end_astdelux, 
        astdelux_dips,astdelux_keys,
        deluxesamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {40,0,0,0},
        {10,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0},
        60,VEC_BW_16,0,
        {0,1024,0,812} 
        },
        { 
        "astdelu2", "Asteroids Deluxe (Revision 2)", astdelu2, 
        &init_astdelux,0, &run_asteroids,&end_astdelux, 
        astdelux_dips,astdelux_keys,
        deluxesamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {40,0,0,0},
        {10,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0},
        60,VEC_BW_16,0,
        {0,1024,0,812} 
        },
        {
        "astdelux", "Asteroids Deluxe (Revision 3)", astdelux, 
        &init_astdelux,0, &run_asteroids,&end_astdelux, 
        astdelux_dips,astdelux_keys,
        deluxesamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {40,0,0,0},
        {10,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0},
        60,VEC_BW_16,0,
        {0,1024,0,812}
        },
        
        { 
        "omegrace", "Omega Race", omegrace, 
        &init_omega,0,&run_omega, &end_omega, 
        omega_dips,omega_keys,
        omega_samples,noart,
        {CPU_MZ80,CPU_MZ80,CPU_NONE,CPU_NONE},
        {3020000,1512000,0,0},

        {25,25,0,0},

        {4,4,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        40,VEC_BW_16,0 
        },

        { 
        "deltrace", "Delta Race (Omega Race Bootleg)", deltrace, 
        &init_omega,0,&run_omega, &end_omega, 
        omega_dips,omega_keys,
        omega_samples,noart,
        {CPU_MZ80,CPU_MZ80,CPU_NONE,CPU_NONE},
        {3020000,1512000,0,0},
        {25,25,0,0},
        {4,4,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        40,VEC_BW_16,0 
        },
        { 
        "bzone", "Battlezone (Revision 1)", bzone, 
        &init_bzone,0,&run_bzone, &end_bzone, 
        bzone_dips, bzone_keys, 
        bzonesamples, noart, 
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {189,0,0,0},
        {31,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0},
        40 ,VEC_BW_16,0 
        },
        {
        "bzone2", "Battlezone (Revision 2)", bzone2, 
        &init_bzone,0,&run_bzone, &end_bzone, 
        bzone_dips, bzone_keys, 
        bzonesamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {189,0,0,0},
        {31,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0},
        40 ,VEC_BW_16,0 
        },
        { 
        "bzonec", "Battlezone Cocktail Proto", bzonec, 
        &init_bzone,0,&run_bzone, &end_bzone, 
        bzone_dips, bzone_keys, 
        bzonesamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {189,0,0,0},
        {31,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0},
        40 ,VEC_BW_16,0 
        },
        { 
        "bzonep", "Battlezone Plus (Clay Cowgill)", bzonep, 
        &init_bzone,0,&run_bzone, &end_bzone, 
        bzone_dips, bzone_keys, 
        bzonesamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {189,0,0,0},
        {31,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0},
        40 ,VEC_BW_16,0 
        },
        { 
        "redbaron", "Red Baron", redbaron, 
        &init_bzone,0,&run_bzone, &end_bzone, 
        redbaron_dips, redbaron_keys, 
        redbaron_samples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE}, 
        {1512000,0,0,0},
        {41,0,0,0},
        {10,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0},
        60,VEC_BW_16,0 
        },
        { 
        "bradley", "Bradley Trainer", bradley, 
        &init_bzone,0,&run_bzone, &end_bzone, 
        bradley_dips, bradley_keys, 
        bzonesamples, noart, 
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {189,0,0,0},
        {31,0,0,0},
        {INT_TYPE_NMI,0,0,0},
        {0,0,0,0},
        40 ,VEC_BW_16,0 
        },
        {
        "spacduel", "Space Duel", spacduel, 
        &init_spacduel,0,&run_spacduel,&end_spacduel,//&set_sd
        spacduel_dips,spacduel_keys, 
        nosamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE}, 
        {1519000,0,0,0},
        {168,0,0,0},
        {31,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        45,VEC_COLOR,0
        },
        { 
        "bwidow",   "Black Widow", bwidow, 
        &init_spacduel,0,&run_spacduel,&end_spacduel, 
        bwidow_dips,bwidow_keys, 
        nosamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE}, 
        {1512000,0,0,0},
        {41,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60,VEC_COLOR,0 
        },
        { 
        "gravitar", "Gravitar (Revision 3)", gravitar, 
        &init_spacduel,0,&run_spacduel,&end_spacduel, 
        gravitar_dips,gravitar_keys, 
        nosamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1515000,0,0,0},
        {41,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60,VEC_COLOR,0 
        },
        { 
        "gravitr2", "Gravitar (Revision 2)", gravitr2, 
        &init_spacduel,0,&run_spacduel,&end_spacduel, 
        gravitar_dips,gravitar_keys, 
        nosamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1515000,0,0,0},
        {41,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60,VEC_COLOR,0 
        },
        { 
        "gravp", "Gravitar (Prototype)", gravp, 
        &init_spacduel,0,&run_spacduel,&end_spacduel, 
        gravitar_dips,gravitar_keys, 
        nosamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1515000,0,0,0},
        {41,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60,VEC_COLOR,0
        },
        {
        "lunarbat", "Lunar Battle (Prototype, Late)", lunarbat, 
        &init_spacduel,0,&run_spacduel,&end_spacduel, 
        gravitar_dips, gravitar_keys, 
        nosamples, noart, 
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {55,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        45,VEC_COLOR,0 
        },
        { 
        "lunarba1", "Lunar Battle (Prototype, Early)", lunarba1, 
        &init_spacduel,0,&run_spacduel,&end_spacduel, 
        gravitar_dips, gravitar_keys, 
        nosamples, noart, 
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {55,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        45,VEC_COLOR ,0
        },
            
        { 
        "tempestm", "Tempest Multigame (1999 Clay Cowgill)", tempestm, 
        &init_tempest,&set_tempest_video,&run_tempest,&end_tempest, 
        tempest_dips, tempestm_keys, 
        nosamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {41,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60, VEC_COLOR,0 
        },
        {
        "tempest", "Tempest (Revision 3)", tempest, 
        &init_tempest,&set_tempest_video,&run_tempest,&end_tempest, 
        tempest_dips, tempest_keys, 
        nosamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1515000,0,0,0},
        {41,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60, VEC_COLOR,0 
        },
        {
        "tempest3", "Tempest (Revision 2B)", tempest3, 
        &init_tempest,&set_tempest_video,&run_tempest,&end_tempest, 
        tempest_dips, tempest_keys, 
        nosamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {41,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60, VEC_COLOR ,0
        },
        {
        "tempest2", "Tempest (Revision 2A)", tempest2, 
        &init_tempest,&set_tempest_video,&run_tempest,&end_tempest,
        tempest_dips, tempest_keys, 
        nosamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {41,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60, VEC_COLOR,0
        },
        { 
        "tempest1", "Tempest (Revision 1)", tempest1, 
        &init_tempest,&set_tempest_video,&run_tempest,&end_tempest, 
        tempest_dips, tempest_keys, 
        nosamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {41,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60, VEC_COLOR,0 
        },
        {
        "temptube", "Tempest (Revision 1)", temptube, 
        &init_tempest,&set_tempest_video,&run_tempest,&end_tempest, 
        tempest_dips, tempest_keys, 
        nosamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {41,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60, VEC_COLOR,0 
        },
        { 
        "alienst", "Aliens (Tempest Alpha)", alienst, 
        &init_tempest,&set_tempest_video,&run_tempest,&end_tempest, 
        tempest_dips, tempest_keys, 
        nosamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {41,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60, VEC_COLOR,0 
        },
        {
        "vbreak", "Vector Breakout (1999 Clay Cowgill)", vbreak,
        &init_tempest,&set_tempest_video,&run_tempest,&end_tempest,
        tempest_dips, tempest_keys, 
        nosamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {41,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60, VEC_COLOR,0 
        },
        {
        "vortex", "Vortex (Tempest Beta)", vortex, 
        &init_tempest,&set_tempest_video,&run_tempest,&end_tempest, 
        tempest_dips, tempest_keys, 
        nosamples, noart,
        {CPU_6502Z,CPU_NONE,CPU_NONE,CPU_NONE},
        {1512000,0,0,0},
        {41,0,0,0},
        {10,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60, VEC_COLOR ,0
        },
        {
        "zektor", "Zektor", zektor, 
        &init_segag80,0,&run_segag80,&end_segag80, 
        zektor_dips, zektor_keys, 
        zektor_samples, noart, 
        {CPU_MZ80,CPU_NONE,CPU_NONE,CPU_NONE},
        {3000000,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        40, VEC_COLOR,0 
        },
        {
        "tacscan", "Tac/Scan", tacscan, 
        &init_segag80,0,&run_segag80,&end_segag80, 
        tacscan_dips, tacscan_keys, 
        tacscan_samples, noart, 
        {CPU_MZ80,CPU_NONE,CPU_NONE,CPU_NONE},
        {3000000,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        40, VEC_COLOR,0 
        },
        {
        "startrek", "Star Trek", startrek, 
        &init_segag80,0,&run_segag80,&end_segag80, 
        startrek_dips, startrek_keys, 
        startrek_samples, noart,
        { CPU_MZ80,CPU_NONE,CPU_NONE,CPU_NONE},
        {3000000,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        40, VEC_COLOR,0 
        },
        {
        "spacfury", "Space Fury (Revision C)", spacfury, 
        &init_segag80,0,&run_segag80,&end_segag80, 
        spacfury_dips, spacfury_keys, 
        spacfury_samples, noart,
        { CPU_MZ80,CPU_NONE,CPU_NONE,CPU_NONE},
        {3000000,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        40, VEC_COLOR ,0
        },
        { 
        "spacfura", "Space Fury (Revision A)", spacfura, 
        &init_segag80,0,&run_segag80,&end_segag80, 
        spacfury_dips, spacfury_keys, 
        spacfury_samples, noart,
        {CPU_MZ80,CPU_NONE,CPU_NONE,CPU_NONE},
        {3000000,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        40, VEC_COLOR,0 
        },
        {
        "spacfurb", "Space Fury (Revision B)", spacfurb, 
        &init_segag80,0,&run_segag80,&end_segag80, 
        spacfury_dips, spacfury_keys, 
        spacfury_samples, noart,
        {CPU_MZ80,CPU_NONE,CPU_NONE,CPU_NONE},
        {3000000,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        40, VEC_COLOR,0 
        },
        { 
        "elim2", "Eliminator (2 Player Set 1)", elim2, 
        &init_segag80,0,&run_segag80,&end_segag80, 
        elim_dips, elim_keys, 
        elim_samples, noart,
        {CPU_MZ80,CPU_NONE,CPU_NONE,CPU_NONE},
        {3000000,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        40, VEC_COLOR ,0
        },
        { 
        "elim2a", "Eliminator (2 Player Set 2A)", elim2a, 
        &init_segag80,0,&run_segag80,&end_segag80, 
        elim_dips, elim_keys, 
        elim_samples, noart,
        {CPU_MZ80,CPU_NONE,CPU_NONE,CPU_NONE},
        {3000000,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        40, VEC_COLOR,0 
        },
        {
        "elim2c", "Eliminator (2 Player Set 2C)", elim2c, 
        &init_segag80,0,&run_segag80,&end_segag80,
        elim_dips, elim_keys, 
        elim_samples, noart,
        {CPU_MZ80,CPU_NONE,CPU_NONE,CPU_NONE},
        {3000000,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        40, VEC_COLOR ,0
        },
        { 
        "elim4", "Eliminator (4 Player)", elim4, 
        &init_segag80,0,&run_segag80,&end_segag80, 
        elim_dips, elim_keys, 
        elim_samples, noart,
        {CPU_MZ80,CPU_NONE,CPU_NONE,CPU_NONE},
        {3000000,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        40, VEC_COLOR,0
        },
        { 
        "elim4p", "Eliminator (4 Player Prototype)", elim4p, 
        &init_segag80,0,&run_segag80,&end_segag80, 
        elim_dips, elim_keys, 
        elim_samples, noart,
        { CPU_MZ80,CPU_NONE,CPU_NONE,CPU_NONE},
        {3000000,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        40, VEC_COLOR,0 
        },
        
        {
        "mhavoc", "Major Havoc (Revision 3)", mhavoc, 
        &init_mhavoc,0,&run_mhavoc,&end_mhavoc, 
        mhavoc_dips,mhavoc_keys, 
        nosamples, noart, 
        {CPU_NONE,CPU_NONE,CPU_NONE,CPU_NONE},
        {2500000,1250000,0,0},//1250000//2505000,1260000,
        {400,400,0,0},
        {0,0,0,0},
        {INT_TYPE_NONE,INT_TYPE_NONE,0,0},
        {0,&mhavoc_interrupt,0,0},
        50,VEC_COLOR,0
        },
        
        {
        "mhavoc2", "Major Havoc (Revision 2)", mhavoc2, 
        &init_mhavoc,0,&run_mhavoc,&end_mhavoc, 
        mhavoc_dips,mhavoc_keys, 
        nosamples, noart,
        {CPU_NONE,CPU_NONE,CPU_NONE,CPU_NONE},
        {2500000,1250000,0,0},
        {400,400,0,0},
        {31,31,0,0},
        {INT_TYPE_INT,INT_TYPE_INT,0,0},
        {0,0,0,0},
        50,VEC_COLOR,0
        },
        { 
        "mhavocrv", "Major Havoc (Return To VAX - Mod by Jeff Askey)", mhavocrv, 
        &init_mhavoc,0,&run_mhavoc,&end_mhavoc, 
        mhavoc_dips,mhavoc_keys,
        nosamples, noart,
        {CPU_NONE,CPU_NONE,CPU_NONE,CPU_NONE},
        {2500000,1250000,0,0},
        {400,400,0,0},
        {31,31,0,0},
        {INT_TYPE_INT,INT_TYPE_INT,0,0},
        {0,0,0,0},
        50,VEC_COLOR,0
        },
        { 
        "mhavocp", "Major Havoc (Prototype)", mhavocp, 
        &init_mhavoc,0,&run_mhavoc,&end_mhavoc, 
        mhavocp_dips,mhavoc_keys, 
        nosamples, noart, 
        {CPU_NONE,CPU_NONE,CPU_NONE,CPU_NONE},
        {2500000,1250000,0,0},
        {400,400,0,0},
        {31,31,0,0},
        {INT_TYPE_INT,INT_TYPE_INT,0,0},
        {0,0,0,0},
        50,VEC_COLOR,0
        },
        {
        "alphaone", " Alpha One (Major Havoc Prototype - 3 Lives)", alphaone, 
        &init_mhavoc,0,&run_mhavoc,&end_mhavoc, 
        no_dips, alphaone_keys, 
        nosamples, noart, 
        {CPU_NONE,CPU_NONE,CPU_NONE,CPU_NONE},
        {2500000,1250000,0,0},
        {400,400,0,0},
        {31,31,0,0},
        {INT_TYPE_INT,INT_TYPE_INT,0,0},
        {0,0,0,0},
        50,VEC_COLOR,0
        },
        { 
        "alphaona", " Alpha One (Major Havoc Prototype - 5 Lives)", alphaona, 
        &init_mhavoc,0,&run_mhavoc,&end_mhavoc, 
        no_dips ,alphaone_keys, 
        nosamples, noart,
        {CPU_NONE,CPU_NONE,CPU_NONE,CPU_NONE},
        {2500000,1250000,0,0},
        {400,400,0,0},
        {31,31,0,0},
        {INT_TYPE_INT,INT_TYPE_INT,0,0},
        {0,0,0,0},
        50,VEC_COLOR,0
        },
            { 
        "starwar1", "Star Wars (Revision 1)", starwar1, 
        &init_starwars,0,&run_starwars,&end_starwars, 
        starwars_dips,starwars_keys, 
        nosamples, noart,
        {CPU_6809,CPU_6809,CPU_NONE,CPU_NONE},
        {2500000,1250000,0,0},
        {400,400,0,0},
        {31,31,0,0},
        {INT_TYPE_INT,INT_TYPE_INT,0,0},
        {0,0,0,0},
        30,VEC_COLOR,0
        },
         {
        "starwars", "Star Wars (Revision 2)", starwars, 
        &init_starwars,0,&run_starwars,&end_starwars, 
        starwars_dips,starwars_keys,
        nosamples, noart,
        {CPU_6809,CPU_6809,CPU_NONE,CPU_NONE}, 
        {2500000,1250000,0,0},
        {400,400,0,0},
        {31,31,0,0},
        {INT_TYPE_INT,INT_TYPE_INT,0,0},
        {0,0,0,0},
        30,VEC_COLOR,0
        },
        {
        "quantum1", "Quantum (Revision 1)", quantum1, 
        &init_quantum,0,&run_quantum, &end_quantum, 
        quantum_dips,quantum_keys, 
        nosamples,noart,
        {CPU_68000,CPU_NONE,CPU_NONE,CPU_NONE},
        {6048000,0,0,0},
        {10,0,0,0},
        {1,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60,VEC_COLOR,0
        },
        {
        "quantum", "Quantum (Revision 2)", quantum, 
        &init_quantum,0,&run_quantum, &end_quantum, 
        quantum_dips,quantum_keys, 
        nosamples,noart,
        {CPU_68000,CPU_NONE,CPU_NONE,CPU_NONE},
        {6000000,0,0,0},
        {126,0,0,0},
        {31,0,0,0},
        {INT_TYPE_68K1,0,0,0},
        {0,0,0,0},
        60,VEC_COLOR,0
        },
        {
        "quantump", "Quantum (Prototype)", quantump, 
        &init_quantum,0,&run_quantum, &end_quantum, 
        quantum_dips,quantum_keys, 
        nosamples,noart,
        {CPU_68000,CPU_NONE,CPU_NONE,CPU_NONE},
        {6048000,0,0,0},
        {10,0,0,0},
        {1,0,0,0},
        {INT_TYPE_INT,0,0,0},
        {0,0,0,0},
        60,VEC_COLOR,0
        },
///////////////////////////////////////CINEMATRONICS////////////////////////////////////////////////

        {
        "solarq", "Solar Quest", solarq, 
        &init_cinemat,0,&run_cinemat, &end_cinemat, 
        cinemat_dips, solarq_keys, 
        solarq_samples, noart, 
        {CPU_CCPU,CPU_NONE,CPU_NONE,CPU_NONE},
        {4980750,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        38 ,VEC_BW_64,0 //VEC_BW_64
        },
        { 
        "starcas", "Star Castle", starcas, 
        &init_cinemat,0,&run_cinemat, &end_cinemat,
        cinemat_dips, starcas_keys,
        starcas_samples, noart, 
        {CPU_CCPU,CPU_NONE,CPU_NONE,CPU_NONE},
        {4980750,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        38 ,VEC_BW_64,0
        },
        {
        "armora", "Armor Attack", armora,
        &init_cinemat,0,&run_cinemat, &end_cinemat, 
        armora_dips, armora_keys, 
        armora_samples, noart,
        {CPU_CCPU,CPU_NONE,CPU_NONE,CPU_NONE},
        {4980750,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        38 ,VEC_BW_64,2 
        },
        { 
        "barrier", "Barrier", barrier, 
        &init_cinemat,0,&run_cinemat, &end_cinemat, 
        cinemat_dips, barrier_keys, 
        barrier_samples, noart,
        {CPU_CCPU,CPU_NONE,CPU_NONE,CPU_NONE},
        {4980750,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        38 ,VEC_BW_64,0 
        },
        {
        "sundance", "Sundance", sundance, 
        &init_cinemat,0,&run_cinemat, &end_cinemat, 
        cinemat_dips, sundance_keys, sundance_samples, noart,
        {CPU_CCPU,CPU_NONE,CPU_NONE,CPU_NONE},
        {4980750,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        38 ,VEC_BW_64,1 
        },
        { 
        "warrior", "Warrior", warrior, 
        &init_cinemat,0,&run_cinemat, &end_cinemat, 
        warrior_dips, warrior_keys, warrior_samples, noart,
        {CPU_CCPU,CPU_NONE,CPU_NONE,CPU_NONE},
        {4980750,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        38 ,VEC_BW_64,0 
        },
        {
        "ripoff", "RipOff", ripoff, 
        &init_cinemat,0,&run_cinemat, &end_cinemat, 
        ripoff_dips, ripoff_keys, ripoff_samples, noart,
        {CPU_CCPU,CPU_NONE,CPU_NONE,CPU_NONE},
        {4980750,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        38 ,VEC_BW_64,0 
        },
        {
        "tailg", "TailGunner", tailg, 
        &init_cinemat,0,&run_cinemat, &end_cinemat,
        tailg_dips, tailg_keys, tailg_samples, noart,
        {CPU_CCPU,CPU_NONE,CPU_NONE,CPU_NONE},
        {4980750,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        38 ,VEC_BW_64,0 
        },
        {
        "starhawk", "StarHawk (Missing Some Samples)", starhawk, 
        &init_cinemat,0,&run_cinemat, &end_cinemat, 
        starhawk_dips, starhawk_keys, 
        starhawk_samples, noart,
        {CPU_CCPU,CPU_NONE,CPU_NONE,CPU_NONE},
        {4980750,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0}, 
        38 ,VEC_BW_64,0 
        },
        {
        "spacewar", "SpaceWar", spacewar, 
        &init_cinemat,0,&run_cinemat, &end_cinemat, 
        cinemat_dips, spacewar_keys, 
        spacewar_samples, noart,
        {CPU_CCPU,CPU_NONE,CPU_NONE,CPU_NONE},
        {4980750,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        38 ,VEC_BW_64,0 
        },
        {
        "speedfrk", "Speed Freak (No Sound, Input)", speedfrk, 
        &init_cinemat,0,&run_cinemat, &end_cinemat, 
        cinemat_dips, speedfrk_keys, 
        speedfrk_samples, noart,
        {CPU_CCPU,CPU_NONE,CPU_NONE,CPU_NONE},
        {4980750,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        38 ,VEC_BW_64,0 
        },
        { 
        "demon", "Demon (No Sound)", demon, 
        &init_cinemat,0,&run_cinemat, &end_cinemat,
         cinemat_dips, demon_keys, 
         demon_samples, noart,
        {CPU_CCPU,CPU_NONE,CPU_NONE,CPU_NONE},
        {4980750,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        38 ,VEC_BW_64,2 
        },
        { 
        "boxingb", "Boxing Bugs (No Sound, Input)", boxingb, 
        &init_cinemat,0,&run_cinemat, 
        &end_cinemat, cinemat_dips, boxingb_keys, 
        boxingb_samples, noart, 
        {CPU_CCPU,CPU_NONE,CPU_NONE,CPU_NONE},
        {4980750,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        38 ,VEC_BW_64,0 
        },
        { 
        "wotw", "War of the Worlds (No Sound)", wotw, 
        &init_cinemat,0,&run_cinemat, &end_cinemat, 
        cinemat_dips, wotw_keys, 
        nosamples, noart,
        {CPU_CCPU,CPU_NONE,CPU_NONE,CPU_NONE},
        {4980750,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        38 ,VEC_BW_64,0 
        },
/*
        {
        "galaga", "Galaga (Namco)", galaga, 
        &init_galaga,0,&run_galaga,&end_galaga, 
        no_dips, galaga_keys, 
        nosamples, noart, 
        {CPU_MZ80,CPU_MZ80,CPU_MZ80,CPU_NONE},
        {3000000,3000000,3000000,0},
        {100,1,1,0},
        {8,1,1,0},
        {INT_TYPE_INT,INT_TYPE_INT,INT_TYPE_INT,0},
        {0,0,0,0},
        60, RASTER_32, 2 
        },
*/

        { 0,0,0,0,0,0,0,0,0,0}// end of array 
        };


int mystrcmp(const char *s1, const char *s2)
{
    while(*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

// from x_interface
void init_graphics (int argc, char *argv[], int p_smallwindow, int p_use_pixmap, int p_line_width, char *window_name);
void xopen_page (int step);
void xclose_page (void);


extern uint32_t inputStart;
extern uint32_t inputEnd;
extern int lastPLSize;

void run_game(void)
{
  int goodload=99;
// TODO load ini files  
//  setup_game_config();

  goodload=load_roms(driver[gamenum].name, driver[gamenum].rom);
  if (goodload==0) 
  {
    log_it("Rom loading failure, exiting...");have_error=10;gamenum=0;gamefps = 60;
    if(!in_gui){exit(1);}
  }

  
  dips =driver[gamenum].game_dips;
  retrieve_dips();

  init_keys(gamenum);
  MK = default_keys;
  gamefps = driver[gamenum].fps;
  
//  millsec = (double) 1000 / (double) driver[gamenum].fps;
/* MALBAN
  goodload=load_samples(driver[gamenum].game_samples, 0);
  if (goodload==0) {log_it("Samples loading failure, bad with no sound..."); }
  log_it("Number of samples for this game is %d",num_samples);
*/
  init_cpu_config(); ////////////////////-----------
  driver[gamenum].init_game();
  set_input_defaults();
  WATCHDOG=0;
  log_it("\n\n\n----END OF INIT -----!\n\n\n");

  while (!done)
  {  
    //log_it("FRAME");
    set_new_frame(); //This is for the AVG Games.
#ifndef NO_PI
#else
xopen_page(0);
#endif    
    update_input();
		
    if (driver[gamenum].cpu_type[0]==CPU_6502Z || driver[gamenum].cpu_type[0]==CPU_6502 || driver[gamenum].cpu_type[0]==CPU_MZ80 || driver[gamenum].cpu_type[0]==CPU_68000)
    {
      if (!paused && have_error==0) 
      {
        if (driver[gamenum].pre_run) 
          driver[gamenum].pre_run();
      }
      if (!paused && have_error==0) 
      {
        run_cpus_to_cycles();
      }
    }
    if (!paused && have_error==0) 
    {
      driver[gamenum].run_game();
    }
    
    
/*    
    
    
    
    
  int iii=0;
  extern int pipelineCounter; // ONE which is currently written to!

while ((((volatile unsigned char)currentButtonState)&0x0f) == (0x01)) 
{
  if (pipelineCounter == 0) break;
printf(".");

  m6502zpexec(10000);



int plc = pipelineCounter;
extern VectorPipelineBase *_BPL[];
extern int basePipeLineWriting;
extern VectorPipelineBase *  pb;

VectorPipelineBase *  _pb = _BPL[basePipeLineWriting?0:1]; // current base pipeline 


  for (int i=0; i<pipelineCounter; i++)
  {
    VectorPipelineBase *work = &pb[i];
    VectorPipelineBase *cpb_N = &_pb[i];
    
    cpb_N->yStart =  work->yStart;
    cpb_N->xStart =  work->xStart;
    cpb_N->yEnd =  work->yEnd;
    cpb_N->xEnd =  work->xEnd;
    cpb_N->y0 =  work->y0;
    cpb_N->x0 =  work->x0;
    cpb_N->y1 =  work->y1;
    cpb_N->x1 =  work->x1;
    cpb_N->intensity =  work->intensity;
    cpb_N->pattern =  work->pattern;
    cpb_N->sms =  work->sms;
    cpb_N->timingForced =  work->timingForced;
    cpb_N->force =  work->force;
    cpb_N->rlines =  work->rlines;
    if (i==pipelineCounter-1)
      cpb_N->next =  (VectorPipelineBase *)0;
    else
      cpb_N->next = &_pb[i+1]; 
    if (i==0)
      cpb_N->previous =  (VectorPipelineBase *)0;
    else
      cpb_N->next = &_pb[i-1]; 
    cpb_N->debug[0] = 0;
    
  }

    v_playAllSFX();
    v_doSound();    // not needed in IRQ Mode
    v_readButtons(); // not neededin IRQ mode
    v_readJoystick1Analog(); // not neededin IRQ mode
    v_WaitRecal();
pipelineCounter = plc;    
printf("pl2: %i\n", pipelineCounter);    
    
    
printf(":");
//  if (pipelineCounter<800) v_directDraw32(1000+iii++, 100+iii++, 20000+iii++, 10000+iii++, 17);
}
//printf("While exited\n");    
*/    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
#ifndef NO_PI
// with this we can time correctly, "better"is in swavg
    v_playAllSFX();
    v_doSound();    // not needed in IRQ Mode
    v_readButtons(); // not neededin IRQ mode
    v_readJoystick1Analog(); // not neededin IRQ mode
    v_WaitRecal();
    //    log_it("WR PC: %i", lastPLSize);
#else
xclose_page();
#endif    
//printf("Close Frame\n");
  }
  
//  reset_for_new_game();
}
void skipInPipeline(VectorPipelineBase ** cpbPointer, int count)
{
  VectorPipelineBase *cpb = *cpbPointer;
  for (int i=0; i<count;i++)
  {
    cpb = cpb->next;
    if (cpb==0) break;
  }
  *cpbPointer = cpb;
}
/*
 
 // star wars first of cockpit
PL DRAW: xStart: -5904, x:28 x*s:420, yStart: -14208, y:84 y*s:1260 -> xEnd: -5424, yEnd: -12768 scale:15

PL DRAW: xStart: -4272, x:26 x*s:390, yStart: -14624, y:84 y*s:1260 -> xEnd: -3824, yEnd: -13184 scale:15

PL DRAW: xStart: -5040, x:28 x*s:420, yStart: -13888, y:82 y*s:1230 -> xEnd: -4560, yEnd: -12480 scale:15



PL DRAW: xStart: -5168, x:28 x*s:420, yStart: -13984, y:84 y*s:1260 -> xEnd: -4688, yEnd: -12544 scale:15
second
PL DRAW: xStart: -4688, x:68 x*s:340, yStart: -12544, y:68 y*s:340 -> xEnd: -4208, yEnd: -12064 scale:5
third
PL DRAW: xStart: -4208, x:70 x*s:560, yStart: -12064, y:48 y*s:384 -> xEnd: -3504, yEnd: -11584 scale:8
 */

//int normalZero;


int noDots = 0;


// this is AFTER clipping!!!
int checkForSpecialVectors(VectorPipelineBase ** cpbPointer,VectorPipeline **plPointer, int *fpc, int *cx, int *cy) 
{
  VectorPipeline *pl=*plPointer;
  VectorPipelineBase *cpb = *cpbPointer;
  
  // TODO: remove double dots line double dots (on death star
/*
  if (*fpc == 0)
  {
    noDots = 0;
  }
  if ((cpb->xEnd == cpb->xStart) && (cpb->yEnd == cpb->yStart) )
  {
    if (noDots)
      return CV_SPECIAL_SKIP;
  }
*/
  
  // skip cockpit if
  if ( ((cpb->xEnd-cpb->xStart == 480) || (cpb->xEnd-cpb->xStart == 448) )
    && ((cpb->yEnd-cpb->yStart == 1440)||(cpb->yEnd-cpb->yStart == 1408))) // pre scale, rotation coordinates
  {
    skipInPipeline(cpbPointer, 191);
    return CV_SPECIAL_SKIP; //skip this entry
  }
  

// extended titel copyright  
//  PL DRAW: xStart: -4112, x:0 x*s:0, yStart: -8608, y:57 y*s:171 -> xEnd: -4112, yEnd: -8320 scale:3
  if ( ((cpb->xStart == -4112) && (cpb->yStart == -8608) )
    && ((cpb->xEnd == -4112)   && (cpb->yEnd == -8320))) // pre scale, rotation coordinates
  {
      skipInPipeline(cpbPointer, 358);
      return CV_SPECIAL_SKIP; //skip this entry
  }
/*  
  // stop here - instructions, no dots
//  PL DRAW: xStart: -11472, x:96 x*s:288, yStart: 8608, y:0 y*s:0 -> xEnd: -11184, yEnd: 8608 scale:3
  if ( ((cpb->xStart == -11472) && (cpb->yStart == 8608) )
    && ((cpb->xEnd == -11184)   && (cpb->yEnd == 8608))) // pre scale, rotation coordinates
  {
    noDots = 1;
    return CV_SPECIAL_NONE; // do not skip
  }

  // scoring
//  PL DRAW: xStart: -3056, x:0 x*s:0, yStart: 8128, y:96 y*s:288 -> xEnd: -3056, yEnd: 8416 scale:3
  if ((cpb->xStart == -3056)  && (cpb->xEnd == -3056)   && (cpb->yEnd-cpb->yStart == 8416-8128) ) // pre scale, rotation coordinates
  {
      noDots = 1;
     return CV_SPECIAL_NONE; // do not skip
  }
  */
  return CV_SPECIAL_NONE; // do not skip
}

int main(int argc, char *argv[])
{
  int smallwindow = 1;
  int use_pixmap = 1;
  int line_width = 0;
#ifndef NO_PI
  vectrexinit(1);
  v_init();
  v_noSound();
    
  gamenum = getLoadParameter(0)[0];
  // special vectrex  interface settings for asteroids
  v_setRefresh(50);
  v_setClientHz(driver[gamenum].fps); // 62.5
//  v_setClientHz(1000);
printf("AAE(%i) Client Hz: %i\n\r", gamenum,driver[gamenum].fps);  
  v_setupIRQHandling();
//  v_enableJoystickDigital(1,1,0,0);
  v_enableJoystickAnalog(1,1,1,1);
  v_enableSoundOut(1);
  v_enableButtons(1);
  useDoubleTimer = 0;

  #else
  init_graphics (argc, argv, smallwindow, use_pixmap, line_width, "Starwars");
#endif    

//  gamenum = MHAVOC;
  if (gamenum == STARWARS)
  {
    // star wars
    v_setRefresh(41);
    v_setClientHz(driver[gamenum].fps); // 62.5
    v_setupIRQHandling();
    v_setCustomClipping(1, -16000, -14000, 14400, 15000);
    checkExternal = checkForSpecialVectors;
    keepDotsTogether = 1;
  }
  
  run_game();
  return 0;   
}
