/*
todo

- deathstar explosion
- death star writing when zoomed out...
- init file
- more PSG

- leaving star ware sometimes endless loops
  (Tie fighters not leaving???) -> savetyNet???









// jumped to by "DPbyte_41"*2

Jump_Table_1:   fdb sub_6275            ; 0 ; Game initialisation
ROM:6044                 fdb sub_64E2            ; 1
ROM:6044                 fdb sub_64F1            ; 2
ROM:6044                 fdb sub_6513            ; 3
ROM:6044                 fdb sub_6532            ; 4
ROM:6044                 fdb sub_6708            ; 5
ROM:6044                 fdb sub_676B            ; 6
ROM:6044                 fdb sub_62E4            ; 7
ROM:6044                 fdb sub_6306            ; 8
ROM:6044                 fdb sub_6326            ; 9
ROM:6044                 fdb sub_6348            ; 10
ROM:6044                 fdb sub_6459            ; 11
ROM:6044                 fdb sub_6483            ; 12
ROM:6044                 fdb sub_656C            ; 13
ROM:6044                 fdb sub_659F            ; 14
ROM:6044                 fdb sub_6670            ; 15
ROM:6044                 fdb sub_66AC            ; 16
ROM:6044                 fdb sub_6D3B            ; 17
ROM:6044                 fdb sub_6D54            ; 18
ROM:6044                 fdb sub_6D80            ; 19
ROM:6044                 fdb sub_6D86            ; 20
ROM:6044                 fdb sub_6D95            ; 21
ROM:6044                 fdb sub_6D98            ; 22
ROM:6044                 fdb sub_6708            ; 23
ROM:6044                 fdb sub_676B            ; 24
ROM:6044                 fdb sub_6787            ; 25
ROM:6044                 fdb sub_679A            ; 26
ROM:6044                 fdb sub_67E5            ; 27
ROM:6044                 fdb loc_67FD            ; 28
ROM:6044                 fdb sub_6802            ; 29
ROM:6044                 fdb loc_682F            ; 30
ROM:6044                 fdb sub_6838            ; 31
ROM:6044                 fdb loc_6859            ; 32 ; Space Wave
ROM:6044                 fdb sub_68D0            ; 33 ; once Entering Death Star
ROM:6044                 fdb loc_68D5            ; 34 ; Entering Death Star - waiting for tie fighters to leave
ROM:6044                 fdb sub_6912            ; 35
ROM:6044                 fdb sub_6933            ; 36
ROM:6044                 fdb sub_6953            ; 37
ROM:6044                 fdb sub_6968            ; 38
ROM:6044                 fdb sub_6A50            ; 39
ROM:6044                 fdb sub_6A7E            ; 40
ROM:6044                 fdb sub_69A9            ; 41
ROM:6044                 fdb sub_69F4            ; 42
ROM:6044                 fdb sub_6A50            ; 43
ROM:6044                 fdb sub_6A89            ; 44
ROM:6044                 fdb sub_6AAB            ; 45
ROM:6044                 fdb loc_6ABF            ; 46
ROM:6044                 fdb sub_6B22            ; 47
ROM:6044                 fdb loc_6B32            ; 48
ROM:6044                 fdb sub_6AFF            ; 49
ROM:6044                 fdb loc_6B1D            ; 50
ROM:6044                 fdb sub_6BDB            ; 51
ROM:6044                 fdb sub_6BF1            ; 52
ROM:6044                 fdb loc_6C76            ; 53
ROM:6044                 fdb sub_6C84            ; 54
ROM:6044                 fdb loc_6CB6            ; 55
ROM:6044                 fdb sub_6CC4            ; 56
ROM:6044                 fdb loc_6CE1            ; 57
ROM:6044                 fdb sub_6CEF            ; 58
ROM:6044                 fdb sub_6D0C            ; 59
ROM:6044                 fdb sub_6D15            ; 60




*/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
 
#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

/* STARWARS Hardware */
#include "globals.h"
#include "vector.h"
#include "dips.h"
#include "input.h"
#include "loaders.h"
#include "driver.h"

#include "sndhrdw/pokyintf.h"
#include "earom.h"

#include "driver.h"
#include "sndhrdw/starwars.h"
#include "vidhrdw/swavg.h"
#include "vidhrdw/atari_vg.h"
#include "machine/swmathbx.h"
#include "machine/starwars.h"
#include "machine/matari_vg.h"

#include "machine/starwars.c"
#include "sndhrdw/starwars.c"
#include "loaders.h"

#define ROM_LOAD_NORMAL 0
#define ROM_LOAD_16BYTE    1
#define ROM_START(name)  static const struct roms name[] = {
#define ROM_REGION( romSize, loadtype) { NULL, 0x999, romSize, loadtype }, 
#define ROM_LOAD(filename,loadAddr,romSize) { filename,loadAddr,romSize, ROM_LOAD_NORMAL },

#define ROM_LOAD_16B(filename,loadAddr,romSize) { filename,loadAddr,romSize, ROM_LOAD_16BYTE },
#define ROM_RELOAD(loadAddr,romSize) { (char *)-1, loadAddr,romSize, ROM_LOAD_NORMAL },
#define ROM_CONTINUE(loadAddr,romSize) { (char *)-2, loadAddr,romSize, ROM_LOAD_NORMAL },
#define ROM_END {NULL, 0, 0, 0}};

// optimization values
int noCopyright = 0;
int noCoinInfo = 0;
int countOfStars = 0; // 0 == all 0x32
int countOfStarsIngame = 0; // 0 == all 0x32
int noShieldGrafik = 0;
int noCockpit = 0;
int enablePSGSound = 1; // if yes, than play_sw_sound(int a) is called from CPU to start MUS/SFX
int usePSGSamples = 0;// if yes, than PSG is "played" via sampling (multisampling) (overwrites above)
int displayReducedHighscore = 0; // max 10 if 0, than all are display (10)
int reduceLinesDisplayTo = 0; // 0 = all, textlines in attract screens
int reduceDeathStarExplosion = 0;
int savetyNet = 1;

// internals
int gameRunning = 0; // only if countOfStarsIngame!=0
int starWarsZoom = 0; // used internally, is one, when we zoom in on death star
int currentLineCount=0; // internally used with reduceLinesDisplayTo 
int displayedLineCount=0; // internally used with reduceLinesDisplayTo 
int inAttract4 = 0; // internally used with reduceLinesDisplayTo 
int inAttract3 = 0;// internally used with reduceLinesDisplayTo 
int savetyNetCount = 0; // when savefty net active




ROM_START(starwars)
    ROM_REGION(0x14000,0)
	ROM_LOAD( "136021.105",   0x3000, 0x1000 ) /* 3000-3fff is 4k vector rom */
	
	ROM_LOAD( "136021.102",   0x8000, 0x2000 ) /*  8k ROM 1 bank */
	ROM_LOAD( "136021.203",   0xa000, 0x2000 ) /*  8k ROM 2 bank */
	ROM_LOAD( "136021.104",   0xc000, 0x2000 ) /*  8k ROM 3 bank */
	ROM_LOAD( "136021.206",   0xe000, 0x2000 ) /*  8k ROM 4 bank */

	/* Load the Mathbox PROM's temporarily into the Vector RAM area */
	/* During initialisation they will be converted into useable form */
	/* and stored elsewhere. */
	ROM_LOAD( "136021.110",   0x0000, 0x0400 ) /* PROM 0 */
	ROM_LOAD( "136021.111",   0x0400, 0x0400 ) /* PROM 1 */
	ROM_LOAD( "136021.112",   0x0800, 0x0400 ) /* PROM 2 */
	ROM_LOAD( "136021.113",   0x0c00, 0x0400 ) /* PROM 3 */

    ROM_LOAD( "136021.214",   0x10000, 0x4000 )   /* ROM 0 bank pages 0 and 1 */

	/* Sound ROMS */
	//ROM_REGION( 0x10000, REGION_CPU2 )     /* Really only 32k, but it looks like 64K */
	ROM_REGION(0x10000,1)
	ROM_LOAD( "136021.107",   0x4000, 0x2000 ) /* Sound ROM 0 */
	ROM_LOAD( "136021.107",   0xc000, 0x2000 ) /* Copied again for */
	ROM_LOAD( "136021.208",   0x6000, 0x2000 ) /* Sound ROM 0 */
	ROM_LOAD( "136021.208",   0xe000, 0x2000 ) /* proper int vecs */
ROM_END

#define IN_NOT  -2

#define GKEY_START(name) static GAMEKEYS name[] = {
#define GKEY_END  {-1,-1,"NONE",-1,-1,-1,-1,-1,-1 }};

GKEY_START(starwars_keys)
 GKEY_SET(0,0xdf,"Default",         0,IN_DEFAULT) 
 GKEY_SET(0,0x02,"Coin 1",        IN_COIN1,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x01,"Coin 2",        IN_COIN2,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x10,"Test Switch",   IN_TEST,    INF_TOGGLE | IN_ACTIVE_LOW )
 GKEY_SET(0,0x04,"Test Advance",  IN_TEST_ADV,	   		   IN_ACTIVE_LOW)
 GKEY_SET(0,0x40,"Right Trigger", MOUSE_B1 | IN_P1B1,      IN_ACTIVE_LOW)
 GKEY_SET(0,0x80,"Left Trigger",  MOUSE_B1 | IN_P1B2,      IN_ACTIVE_LOW)
 GKEY_SET(1,0x34,"Default",     0,IN_DEFAULT) 
 GKEY_SET(1,0x10,"Right Thumb",   MOUSE_B2 | IN_P1B3,      IN_ACTIVE_LOW)
 GKEY_SET(1,0x20,"Left Thumb",    MOUSE_B2 | IN_P1B4,      IN_ACTIVE_LOW)
 GKEY_SET(2,0x01,"Analog Up",     IN_P1UP,      IN_ACTIVE_HIGH)
 GKEY_ANALOG(3,100,"Analog Y",IN_AD_STICK_Y, 0x01, 0xfe,  INF_CENTER | IN_ANALOG ,30,0x80,0xff)
 GKEY_SET(4,0x01,"Analog Down",   IN_P1DOWN,    IN_ACTIVE_HIGH)
 GKEY_SET(7,0x01,"Analog Left",   IN_P1LEFT,    IN_ACTIVE_HIGH)
 GKEY_ANALOG(6,100,"Analog X ",IN_AD_STICK_X, 0x01, 0xfe,  INF_CENTER | IN_ANALOG ,30,0x80,0xff)
 GKEY_SET(5,0x01,"Analog Right",  IN_P1RIGHT,   IN_ACTIVE_HIGH)	
GKEY_END
  
DIP_START(starwars_dips)
   { "PLAY MODE ", {"1 COIN 1 PLAY","2 COINS 1 PLAY","FREE PLAY","1 COIN 2 PLAYS"},
   3, 2, 0xfc, 0, 0, {0, 1, 2, 3 }},	 
   { "BONUS AT", {"10,000", "20,000", "30,000", "NONE "},
   3, 1, 0xc0, 1, 1,{0, 0x40  ,0x80 ,0xc0}},
   { "DIFFICULTY", {"EASY", "HARD", "", ""},
   1, 1, 0xcf, 0, 0,{0x10,  0 , 0, 0}},
   { "SHIPS", {"3", "4", "5", "6"},
   3, 1, 0xf3, 0, 0,{0, 0x04, 0x08, 0x0c }},
DIP_END

#define ART_START(name)  static const struct artworks name[] = {
#define ART_LOAD(zipfile, filename, type, target) { zipfile, filename, type, target },
#define ART_END {NULL, NULL, 0}};

ART_START(noart)
ART_END

int init_starwars(void);
void run_starwars();
void end_starwars();

struct AAEDriver driver[] =
{
         {
        "starwars", "Star Wars (Revision 2)", starwars, 
        &init_starwars,0,&run_starwars,&end_starwars, 
        starwars_dips,starwars_keys,
        0, noart,
        {CPU_6809,CPU_6809,CPU_NONE,CPU_NONE}, 
        {2500000,1250000,0,0},
        {400,400,0,0},
        {31,31,0,0},
        {INT_TYPE_INT,INT_TYPE_INT,0,0},
        {0,0,0,0},
        30,VEC_COLOR,0
        },
	
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

extern uint32_t inputStart;
extern uint32_t inputEnd;
extern int lastPLSize;

 glist gamelist[256];

 GAMEKEYS *MK;
 GAMEKEYS *GK;
 GAMEKEYS *FOO;

//RAM Variables
 unsigned char *membuffer;
 unsigned char vec_ram[0x1fff];
 unsigned char *GI[5]; //Global 6502/Z80/6809 GameImage 
 UINT32 dwElapsedTicks;

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



extern struct GameDriver starwars_driver;
const struct GameDriver *drivers[] =
{	
    &starwars_driver,
	0	
};

static struct RunningMachine machine;
struct RunningMachine *Machine = &machine;
static const struct GameDriver *gamedrv;
static const struct MachineDriver *drv;
unsigned char *RAM;
unsigned char *ROM;

//////////////////////////////////////////////////

/* Star Wars READ memory map */
static struct MemoryReadAddress readmem[] =
{
       { 0x0000, 0x2fff, MRA_RAM, &vectorram },   /* vector_ram */
       { 0x3000, 0x3fff, MRA_ROM },  /* vector_rom */
/*     { 0x4800, 0x4fff, MRA_RAM },   cpu_ram   */
/*     { 0x5000, 0x5fff, MRA_RAM },    (math_ram_r) math_ram */
/*     { 0x0000, 0x3fff, MRA_RAM, &vectorram}, *//* Vector RAM and ROM */
        { 0x4800, 0x5fff, MRA_RAM },  /* CPU and Math RAM */
        { 0x6000, 0x7fff, banked_rom_r }, /* banked ROM */
        { 0x8000, 0xffff, MRA_ROM },  /* rest of main_rom */
        { 0x4300, 0x431f, input_bank_0_r }, /* Memory mapped input port 0 */
        { 0x4320, 0x433f, input_bank_1_r }, /* Memory mapped input port 1 */
        { 0x4340, 0x435f, opt_0_r }, /* DIP switches bank 0 */
        { 0x4360, 0x437f, opt_1_r }, /* DIP switches bank 1 */
        { 0x4380, 0x439f, adc_r },   /* ADC read */
        { 0x4400, 0x4400, main_read_r },
        { 0x4401, 0x4401, main_ready_flag_r },
        { 0x4500, 0x45ff, MRA_RAM }, /* nov_ram */
        { 0x4700, 0x4700, reh },
        { 0x4701, 0x4701, rel },
        { 0x4703, 0x4703, prng }, /* pseudo random number generator */
	{ -1 }	/* end of table */
};

/* Star Wars Sound READ memory map */
static struct MemoryReadAddress readmem2[] =
{
        { 0x0800, 0x0fff, sin_r }, /* SIN Read */

        { 0x1000, 0x107f, MRA_RAM },  /* 6532 RAM */
        { 0x1080, 0x109f, m6532_r },

        { 0x2000, 0x27ff, MRA_RAM }, /* program RAM */
        { 0x4000, 0x7fff, MRA_ROM }, /* sound roms */
        { 0xc000, 0xffff, MRA_ROM }, /* load last rom twice */
                                     /* for proper int vec operation */

        { -1 }  /* end of table */
};

/* Star Wars WRITE memory map */
static struct MemoryWriteAddress writemem[] =
{
	{ 0x0000, 0x2fff, MWA_RAM, &vectorram }, /* vector_ram */
	{ 0x3000, 0x3fff, MWA_ROM },  /* vector_rom */
/*  { 0x4800, 0x4fff, MWA_RAM },   cpu_ram */
/*  { 0x5000, 0x5fff, MWA_RAM },  (math_ram_w) math_ram */
	{ 0x4800, 0x5fff, MWA_RAM }, /* CPU and Math RAM */
	{ 0x6000, 0xffff, MWA_ROM }, /* main_rom */
	{ 0x4400, 0x4400, main_wr_w },
	{ 0x4500, 0x45ff, MWA_RAM }, /* nov_ram */
	{ 0x4600, 0x461f, atari_vg_go },  /* evggo(mine) or vg2_go */
	{ 0x4620, 0x463f, vg_reset }, /* evgres(mine) or vg_reset */
	{ 0x4640, 0x465f, MWA_NOP }, /*  (wdclr) Watchdog clear */
	{ 0x4660, 0x467f, irqclr },  /* clear periodic interrupt */
/*  { 0x4680, 0x4680, MWA_NOP },  (coin_ctr2) Coin counter 1 */
/*  { 0x4681, 0x4681, MWA_NOP },  (coin_ctr1) Coin counter 2 */
	{ 0x4680, 0x4681, MWA_NOP },  /*  Coin counters */
	{ 0x4682, 0x4682, led3 }, /* led3 */
	{ 0x4683, 0x4683, led2 }, /* led2 */
	{ 0x4684, 0x4684, mpage },  /* Page select for ROM0 */
	{ 0x4685, 0x4685, prngclr }, /* Reset PRNG */
	{ 0x4686, 0x4686, led1 },    /* led1 */
	{ 0x4687, 0x4687, recall },
	{ 0x46a0, 0x46bf, nstore },
	{ 0x46c0, 0x46c0, adcstart0 },
	{ 0x46c1, 0x46c1, adcstart1 },
	{ 0x46c2, 0x46c2, adcstart2 },
	{ 0x46e0, 0x46e0, soundrst },
    { 0x4700, 0x4707, swmathbx },
	{ -1 }	/* end of table */
};

/* Star Wars sound WRITE memory map */
static struct MemoryWriteAddress writemem2[] =
{
        { 0x0000, 0x07ff, sout_w },
        { 0x1000, 0x107f, MWA_RAM }, /* 6532 ram */
        { 0x1080, 0x109f, m6532_w },
		{ 0x1800, 0x183f, quad_pokey_w },//starwars_pokey_sound_w },
        { 0x2000, 0x27ff, MWA_RAM }, /* program RAM */
        { 0x4000, 0x7fff, MWA_ROM }, /* sound rom */
        { 0xc000, 0xffff, MWA_ROM }, /* sound rom again, for intvecs */


        { -1 }  /* end of table */
};


static struct MachineDriver machine_driver =
{
	/* basic machine hardware */
	{
                /* Main CPU */
		{
			CPU_M6809,
                        1512000,                        /* 1.5 Mhz CPU clock (Don't know what speed it should be) */
			0,  				/* Memory region #0 */
			readmem,writemem,0,0,
            
// Malban: this is interrupts AFTER X frames, not X interrupts PER frame!!!            
                        interrupt,6            /* Interrupt handler, and interrupts per frame (usually 1) */
                        /* Starwars should be 183Hz interrupts */
                        /* Increasing number of interrupts per frame speeds game up */
                },
                /* Sound CPU */
            {
                 CPU_M6809 | CPU_AUDIO_CPU,
                 1512000,                        /* 1.5 Mhz CPU clock (Don't know what speed it should be) */
                 2,                              /* Memory region #2 */
                 readmem2,writemem2,0,0,
                 starwars_snd_interrupt,24       /* Interrupt handler, and interrupts per frame (usually 1) */
                 /* Interrupts are to attempt to get */
                 /* resolution for the PIA Timer */
                 /* Approx. 2048 PIA clocks (@1.5 Mhz) */
              }

	},
        41, // MALBAN30,                     /* Target Frames per Second */
        0,  /* Name of initialisation handler */

	/* video hardware */
	512, 384, { 0, 240, 0, 280 },
	0,
        256,256, /* Number of colours, length of colour lookup table */
	sw_vg_init_colors,

	VIDEO_TYPE_VECTOR,
        0,  /* Handler to initialise video handware */
	atari_vg_avg_flip_start,	/* Start video hardware */
	atari_vg_stop,			/* Stop video hardware */
	atari_vg_screenrefresh,		/* Do a screen refresh */

	/* sound hardware */
	0,
    0,                              /* Initialise audio hardware */
    starwars_sh_start,              /* Start audio  */
    starwars_sh_stop,				/* Stop audio   */
    starwars_sh_update				/* Update audio */
};


/* in machine/swmathbx.c */
void translate_proms(void);

struct GameDriver starwars_driver =
{
	"Star Wars",
	"starwars",
        "STEVE BAINES\nBRAD OLIVER\nFRANK PALAZZOLO",
	   &machine_driver,

        0,
        translate_proms, 0,  /* ROM decryption, Opcode decryption */
        0,     /* Sample Array (optional) */

        0, 0, 0, 0, 0,

        0, /* Colour PROM */
        0,          /* palette */
        0,          /* colourtable */
	ORIENTATION_DEFAULT,

       0, 0 /* Highscore load, save */
};

//////////////////////////////////////////////////////////////////////////


void run_starwars()
{   
//    int x=0;
//    int f=0;

    cpu_run();

    if (drv->sh_update)
    {
      (*drv->sh_update)();
	    
    }
   // log_it("------------FRAME START --------------"); 
}

/////////////////// MAIN() for program ///////////////////////////////////////////////////
int init_starwars(void)
{
  
   BUFFER_SIZE=256; chip=4;gain=8;
   
    
    Machine->gamedrv = gamedrv = drivers[0];
    Machine->drv = drv = gamedrv->drv;
	
    RAM = GI[CPU0];//Machine->memory_region[drv->cpu[0].memory_region];
    ROM = RAM;

    /* decrypt the ROMs if necessary */
    if (gamedrv->rom_decode) (*gamedrv->rom_decode)();
    cpu_init();
//    load_hi_aae(0x4500, 0x100, 0);
    cpu_start();
    atari_vg_avg_flip_start();
    starwars_sh_start();
    return 0;
}

void end_starwars()
{
// save_hi_aae(0x4500, 0x100, 0);
}

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

  init_keys();
//  MK = default_keys;
  gamefps = driver[gamenum].fps;

  driver[gamenum].init_game();
  set_input_defaults();
  WATCHDOG=0;
  log_it("\n\n\n----END OF INIT -----!\n\n\n");

  while (!done)
  {  
    //log_it("FRAME");
    update_input();
		
    if (!paused && have_error==0) 
    {
      driver[gamenum].run_game();
    }
    v_playAllSFX();
    v_doSound();    // not needed in IRQ Mode
    v_readButtons(); // not neededin IRQ mode
    v_readJoystick1Analog(); // not neededin IRQ mode
    v_WaitRecal();
  }
}
void setAnalogOffsets();
static char ymBuf[10001];
char jingle1[10001];
char jingle2[10001];
char cantina[10001];
int jingle1Rows = 0;
int jingle2Rows = 0;
int cantinaRows = 0;


void initMultiSamples(int rate);

// for now INI setting just stupidly overwrite other saved settings!
static int starwarsIniHandler(void* user, const char* section, const char* name, const char* value)
{
  // cascading ini files
  // first check if there are "general" entries
  if (iniHandler(user, section, name, value) == 1) return 1;

  
  #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
  #define MATCH_NAME(n) strcmp(name, n) == 0
  int i=0;
  if (MATCH_NAME("NO_COPYRIGHT")) noCopyright = atoi(value);
  else if (MATCH_NAME("NO_COIN_INFO")) noCoinInfo = atoi(value);
  else if (MATCH_NAME("STAR_COUNT_TITLE")) countOfStars = atoi(value);
  else if (MATCH_NAME("STAR_COUNT_GAME")) countOfStarsIngame = atoi(value);
  else if (MATCH_NAME("NO_SHIELD_GRAFIK")) noShieldGrafik = atoi(value);
  else if (MATCH_NAME("NO_COCKPIT")) noCockpit = atoi(value);
  else if (MATCH_NAME("ENABLE_PSG_SOUND")) enablePSGSound = atoi(value);
  else if (MATCH_NAME("USE_PSG_SAMPLES")) usePSGSamples = atoi(value);
  else if (MATCH_NAME("COUNT_OF_HS_ENTRIES")) displayReducedHighscore = atoi(value);
  else if (MATCH_NAME("COUNT_TEXT_LINES")) reduceLinesDisplayTo = atoi(value);
  else if (MATCH_NAME("REDUCE_DEATHSTAR_EXPLOSION")) reduceDeathStarExplosion = atoi(value);
  else if (MATCH_NAME("SAVETY_NET")) savetyNet = atoi(value);
  else 
  {
  }
/*
 * No special values as of yet
  if (MATCH("VECTREX_EXACT", "ROM_FILE")) strcpy(romName, value); else 
  {
      printf("Ini-Handler, unkown entry: %s = %s", name, value);
      return 0;  / * unknown section/name, error * /
  }
*/  
  return 1;
}

int main(int argc, char *argv[]) 
{
  int smallwindow = 1;
  int use_pixmap = 1;
  int line_width = 0;
  vectrexinit(1);
  v_init();
  v_noSound();

  int pv = 0;
  if (pv = ini_parse("starwars.ini", starwarsIniHandler, 0) < 0)  
    printf("starwars.ini not loaded!\n\r");
  else printf("starwars.ini loaded!\n\r");


  gamenum = 0; // only star wars
  // special vectrex  interface settings for asteroids
  v_setRefresh(50);
  v_setClientHz(driver[gamenum].fps); // 62.5
  printf("AAE Client Hz: %i\n\r",driver[gamenum].fps);  
  
  v_setupIRQHandling();
//  v_setupSMPHandling();
  v_enableJoystickAnalog(1,1,1,1);
  v_enableSoundOut(1);
  v_enableButtons(1);
  v_enableExtendedSoundOut(1); // plays ayfx / ym in irq
  useDoubleTimer = 0;

  // star wars
  v_setRefresh(41);
  v_setRefresh(50);
  v_setClientHz(driver[gamenum].fps); // 62.5
  v_setupIRQHandling();
  v_setCustomClipping(1, -16000, -14000, 14400, 15000);
  keepDotsTogether = 1;
  
  if (usePSGSamples) initMultiSamples(10000);
  
  jingle1Rows = v_loadYM("sw_jingle1.ym", 10000 ,jingle1, ymBuf);
  jingle2Rows = v_loadYM("sw_jingle2.ym", 10000 ,jingle2, ymBuf);
  cantinaRows = v_loadYM("cantina.ym", 10000 ,cantina, ymBuf);
  run_game();
  return 0;   
}

 // AYFX - Data of file[]= "/Users/chrissalo/NetBeansProjects/Vide/tmp/AstSound/Ast_PlayerShot.afx"
unsigned char playerShot_data[]=
{
	0x6F, 0x00, 0x03, 0x01, 0x2F, 0x31, 0x01, 0x2E, 0xC0, 0x00, 
	0x2E, 0xD6, 0x02, 0x2D, 0x20, 0x01, 0x2D, 0xB5, 0x00, 0x2C, 
	0xAC, 0x02, 0x2C, 0x10, 0x01, 0x2B, 0xAB, 0x00, 0x2B, 0x86, 
	0x02, 0x2A, 0x00, 0x01, 0x2A, 0xA2, 0x00, 0xD0, 0x20, 
};
unsigned char deathStar1_data[]=
{
	0x6F, 0x12, 0x07, 0x0A, 0x2F, 0xCE, 0x02, 0x2F, 0xC4, 0x01, 
	0x2F, 0x66, 0x01, 0x2F, 0xE2, 0x00, 0x2F, 0x12, 0x07, 0x2F, 
	0xCE, 0x02, 0x2F, 0xC4, 0x01, 0x2F, 0x66, 0x01, 0x2F, 0xE2, 
	0x00, 0x2F, 0x12, 0x07, 0x2F, 0xCE, 0x02, 0x2F, 0xC4, 0x01, 
	0x2F, 0x66, 0x01, 0x2F, 0xE2, 0x00, 0x2F, 0x12, 0x07, 0x2F, 
	0xCE, 0x02, 0x2F, 0xC4, 0x01, 0x2F, 0x12, 0x07, 0x2F, 0x66, 
	0x01, 0x2F, 0xCE, 0x02, 0x2F, 0xC4, 0x01, 0x2F, 0x12, 0x07, 
	0x2F, 0xCE, 0x02, 0x0E, 0x2E, 0xC4, 0x01, 0x2E, 0x12, 0x07, 
	0x2E, 0xCE, 0x02, 0x2E, 0xC4, 0x01, 0x0D, 0x2D, 0x12, 0x07, 
	0x2D, 0xCE, 0x02, 0x2D, 0xC4, 0x01, 0x2C, 0x12, 0x07, 0x0C, 
	0x0C, 0x2C, 0xCE, 0x02, 0x2B, 0xC4, 0x01, 0x2B, 0x12, 0x07, 
	0x0B, 0x0B, 0x2A, 0xCE, 0x02, 0x2A, 0xC4, 0x01, 0x2A, 0x12, 
	0x07, 0x0A, 0x2A, 0xCE, 0x02, 0x29, 0xC4, 0x01, 0x29, 0x12, 
	0x07, 0x29, 0xCE, 0x02, 0x09, 0x08, 0x28, 0xC4, 0x01, 0x28, 
	0x12, 0x07, 0x28, 0xCE, 0x02, 0x07, 0x07, 0x27, 0xC4, 0x01, 
	0x27, 0x12, 0x07, 0x26, 0xCE, 0x02, 0x06, 0x26, 0xC4, 0x01, 
	0x26, 0x12, 0x07, 0x26, 0xCE, 0x02, 0xD0, 0x20, 
};

unsigned char deathStar2_data[]=
{
	0xED, 0x70, 0x01, 0x06, 0xAD, 0x00, 0x02, 0xAD, 0x70, 0x01, 
	0xAD, 0x00, 0x02, 0x7F, 0x00, 0x00, 0x1A, 0x1F, 0x5F, 0x14, 
	0x1F, 0x10, 0x10, 0x58, 0x04, 0x18, 0x18, 0x1F, 0x1F, 0x1F, 
	0x5E, 0x0A, 0x1E, 0x1E, 0x1C, 0x1C, 0x1C, 0x5C, 0x08, 0x1C, 
	0x1C, 0x5B, 0x0A, 0x1B, 0x1B, 0x5C, 0x08, 0x1C, 0x1C, 0x5C, 
	0x06, 0x1C, 0x1C, 0x1A, 0x1A, 0x1A, 0x19, 0x19, 0x19, 0x18, 
	0x18, 0x18, 0x17, 0x17, 0x17, 0xD0, 0x20, 
};

unsigned char longerShot_data[]=
{
	0x6E, 0x77, 0x00, 0x1F, 0x0D, 0x0F, 0x2F, 0x84, 0x03, 0x2E, 
	0x77, 0x00, 0x0C, 0x0E, 0x0C, 0x0C, 0x0E, 0x0D, 0x0B, 0x0D, 
	0x0C, 0x0A, 0x0A, 0x0C, 0x0B, 0x0B, 0x0C, 0x0B, 0x09, 0x0C, 
	0x0B, 0x09, 0x09, 0x0A, 0x09, 0x08, 0x0A, 0x0A, 0x08, 0x0A, 
	0x09, 0x07, 0x07, 0x09, 0x08, 0x07, 0x08, 0x08, 0x06, 0x09, 
	0x08, 0x06, 0x06, 0x07, 0x06, 0x05, 0x07, 0x06, 0x07, 0x07, 
	0x05, 0x04, 0x06, 0x05, 0xD0, 0x20, 
};

unsigned char fighterShot_data[]=
{
	0x6F, 0x4C, 0x06, 0x0F, 0x2F, 0x7E, 0x02, 0x2F, 0x92, 0x01, 
	0x2F, 0xF2, 0x05, 0x2E, 0x5B, 0x02, 0x2E, 0x7C, 0x01, 0x2E, 
	0x9D, 0x05, 0x2D, 0x3A, 0x02, 0x2D, 0x66, 0x01, 0x2D, 0x4B, 
	0x05, 0x2C, 0x19, 0x02, 0x2C, 0x52, 0x01, 0x2C, 0xFE, 0x04, 
	0x2B, 0xFB, 0x01, 0x2B, 0x3E, 0x01, 0x2B, 0xB8, 0x04, 0x2A, 
	0xDE, 0x01, 0x2A, 0x2D, 0x01, 0x2A, 0x74, 0x04, 0x2A, 0xC4, 
	0x01, 0x29, 0x1D, 0x01, 0x29, 0x34, 0x04, 0x29, 0xAA, 0x01, 
	0x28, 0x0C, 0x01, 0x28, 0xF8, 0x03, 0x28, 0x92, 0x01, 0x28, 
	0xFD, 0x00, 0xD0, 0x20, 
};
unsigned char shieldHit_data[]=
{
	0x6F, 0xE7, 0x05, 0x10, 0x2F, 0xD0, 0x05, 0x2F, 0xB9, 0x05, 
	0x2F, 0xA3, 0x05, 0x2F, 0x8D, 0x05, 0x2F, 0x77, 0x05, 0x2F, 
	0x62, 0x05, 0x2F, 0x4D, 0x05, 0x2E, 0x38, 0x05, 0x2E, 0x24, 
	0x05, 0x2E, 0x10, 0x05, 0x2E, 0xFC, 0x04, 0x2E, 0xE9, 0x04, 
	0x2E, 0xD6, 0x04, 0x2E, 0xC3, 0x04, 0x2E, 0xB0, 0x04, 0x2D, 
	0x9E, 0x04, 0x2D, 0x8C, 0x04, 0x2D, 0x7A, 0x04, 0x2D, 0x69, 
	0x04, 0x2D, 0x58, 0x04, 0x2D, 0x47, 0x04, 0x2D, 0x36, 0x04, 
	0x2D, 0x26, 0x04, 0x2C, 0x16, 0x04, 0x2C, 0x06, 0x04, 0x2C, 
	0xF6, 0x03, 0x2C, 0xE7, 0x03, 0x2C, 0xD8, 0x03, 0x2C, 0xC9, 
	0x03, 0x2C, 0xBA, 0x03, 0x2C, 0xAC, 0x03, 0x2B, 0x9E, 0x03, 
	0x2B, 0x90, 0x03, 0x2B, 0x82, 0x03, 0x2B, 0x74, 0x03, 0x2B, 
	0x67, 0x03, 0x2B, 0x5A, 0x03, 0x2B, 0x4D, 0x03, 0x2B, 0x40, 
	0x03, 0x2A, 0x33, 0x03, 0x2A, 0x27, 0x03, 0x2A, 0x1B, 0x03, 
	0x2A, 0x0F, 0x03, 0x2A, 0x03, 0x03, 0x2A, 0xF7, 0x02, 0x2A, 
	0xEC, 0x02, 0x2A, 0xE1, 0x02, 0x29, 0xD6, 0x02, 0x29, 0xCB, 
	0x02, 0x29, 0xC0, 0x02, 0x29, 0xB5, 0x02, 0x29, 0xAB, 0x02, 
	0x29, 0xA1, 0x02, 0x29, 0x97, 0x02, 0x29, 0x8D, 0x02, 0x28, 
	0x83, 0x02, 0x28, 0x79, 0x02, 0x28, 0x70, 0x02, 0x28, 0x67, 
	0x02, 0x28, 0x5E, 0x02, 0x28, 0x55, 0x02, 0x28, 0x4C, 0x02, 
	0x28, 0x43, 0x02, 0x27, 0x3A, 0x02, 0x27, 0x32, 0x02, 0x27, 
	0x2A, 0x02, 0x27, 0x22, 0x02, 0x27, 0x1A, 0x02, 0x27, 0x12, 
	0x02, 0x27, 0x0A, 0x02, 0x27, 0x02, 0x02, 0x26, 0xFA, 0x01, 
	0x26, 0xF3, 0x01, 0x26, 0xEC, 0x01, 0x26, 0xE5, 0x01, 0x26, 
	0xDE, 0x01, 0x26, 0xD7, 0x01, 0x26, 0xD0, 0x01, 0x26, 0xC9, 
	0x01, 0x25, 0xC2, 0x01, 0x25, 0xBB, 0x01, 0x25, 0xB5, 0x01, 
	0x25, 0xAF, 0x01, 0x25, 0xA9, 0x01, 0x25, 0xA3, 0x01, 0x25, 
	0x9D, 0x01, 0x25, 0x97, 0x01, 0x24, 0x91, 0x01, 0x24, 0x8B, 
	0x01, 0x24, 0x85, 0x01, 0x24, 0x7F, 0x01, 0x24, 0x7A, 0x01, 
	0x24, 0x75, 0x01, 0x24, 0x70, 0x01, 0x24, 0x6B, 0x01, 0xD0, 
	0x20, 
};
unsigned char enemyHit_data[]=
{
	0x6F, 0xE7, 0x05, 0x10, 0x2F, 0xD0, 0x05, 0x2F, 0xB9, 0x05, 
	0x2F, 0xA3, 0x05, 0x2F, 0x8D, 0x05, 0x2F, 0x77, 0x05, 0x2F, 
	0x62, 0x05, 0x2F, 0x4D, 0x05, 0x2E, 0x38, 0x05, 0x2E, 0x24, 
	0x05, 0x2E, 0x10, 0x05, 0x2E, 0xFC, 0x04, 0x2E, 0xE9, 0x04, 
	0x2E, 0xD6, 0x04, 0x2E, 0xC3, 0x04, 0x2E, 0xB0, 0x04, 0x2D, 
	0x9E, 0x04, 0x2D, 0x8C, 0x04, 0x2D, 0x7A, 0x04, 0x2D, 0x69, 
	0x04, 0x2D, 0x58, 0x04, 0x2D, 0x47, 0x04, 0x2D, 0x36, 0x04, 
	0x2D, 0x26, 0x04, 0x2C, 0x16, 0x04, 0x2C, 0x06, 0x04, 0x2C, 
	0xF6, 0x03, 0x2C, 0xE7, 0x03, 0x2C, 0xD8, 0x03, 0x2C, 0xC9, 
	0x03, 0x2C, 0xBA, 0x03, 0x2C, 0xAC, 0x03, 0x2B, 0x9E, 0x03, 
	0x2B, 0x90, 0x03, 0x2B, 0x82, 0x03, 0x2B, 0x74, 0x03, 0x2B, 
	0x67, 0x03, 0x2B, 0x5A, 0x03, 0x2B, 0x4D, 0x03, 0x2B, 0x40, 
	0x03, 0x2A, 0x33, 0x03, 0x2A, 0x27, 0x03, 0x2A, 0x1B, 0x03, 
	0x2A, 0x0F, 0x03, 0x2A, 0x03, 0x03, 0x2A, 0xF7, 0x02, 0x2A, 
	0xEC, 0x02, 0x2A, 0xE1, 0x02, 0x29, 0xD6, 0x02, 0x29, 0xCB, 
	0x02, 0x29, 0xC0, 0x02, 0x29, 0xB5, 0x02, 0x29, 0xAB, 0x02, 
	0x29, 0xA1, 0x02, 0x29, 0x97, 0x02, 0x29, 0x8D, 0x02, 0x28, 
	0x83, 0x02, 0x28, 0x79, 0x02, 0x28, 0x70, 0x02, 0x28, 0x67, 
	0x02, 0x28, 0x5E, 0x02, 0x28, 0x55, 0x02, 0x28, 0x4C, 0x02, 
	0x28, 0x43, 0x02, 0x27, 0x3A, 0x02, 0x27, 0x32, 0x02, 0x27, 
	0x2A, 0x02, 0x27, 0x22, 0x02, 0x27, 0x1A, 0x02, 0x27, 0x12, 
	0x02, 0x27, 0x0A, 0x02, 0x27, 0x02, 0x02, 0x26, 0xFA, 0x01, 
	0x26, 0xF3, 0x01, 0x26, 0xEC, 0x01, 0x26, 0xE5, 0x01, 0x26, 
	0xDE, 0x01, 0x26, 0xD7, 0x01, 0x26, 0xD0, 0x01, 0x26, 0xC9, 
	0x01, 0x25, 0xC2, 0x01, 0x25, 0xBB, 0x01, 0x25, 0xB5, 0x01, 
	0x25, 0xAF, 0x01, 0x25, 0xA9, 0x01, 0x25, 0xA3, 0x01, 0x25, 
	0x9D, 0x01, 0x25, 0x97, 0x01, 0x24, 0x91, 0x01, 0x24, 0x8B, 
	0x01, 0x24, 0x85, 0x01, 0x24, 0x7F, 0x01, 0x24, 0x7A, 0x01, 
	0x24, 0x75, 0x01, 0x24, 0x70, 0x01, 0x24, 0x6B, 0x01, 0xD0, 
	0x20, 
};

void play_sw_sound(int a)
{
  if (usePSGSamples) return;
  int channel = 0;
  
//  printf("Sound: %02x\n", a);
  
  if (a == 0x1c) v_initYM(cantina, cantinaRows, 0);
  if (a == 0x3a) v_playSFXStart(playerShot_data, channel, 0);
  if (a == 0x24) v_initYM(jingle1, jingle1Rows, 0);
  if (a == 0x22) v_initYM(jingle2, jingle2Rows, 0);
  if (a == 0x26) v_playSFXStart(deathStar1_data, 1, 0);
  if (a == 0x27) {v_playSFXStart(deathStar1_data, 0, 0);v_playSFXStart(deathStar2_data, 1, 0);}
  if (a == 0x33) v_playSFXStart(shieldHit_data, 1, 0);
  if (a == 0x34) v_playSFXStart(enemyHit_data, 1, 0);
  if (a == 0x35) v_playSFXStart(deathStar2_data, 1, 0);
  if (a == 0x36) v_playSFXStart(fighterShot_data, 2, 0);
  if (a == 0x39) v_playSFXStart(longerShot_data, 2, 0);
  
  // 1b - high score
  // 1d - Imperial March
  // 20 - Towers 1 music
  // 21 - Towers 2 music
  // 22 - Trench music
  // 24 - Space wave 1 music
  // 25 - Space Wave 2 music
  // 26 - Explosion
  // 32 - R2 beeps entering Death Star
  
/* http://www.wardclan.f9.co.uk/136021_Sound.html
 * see also: https://github.com/historicalsource/star-wars/blob/main/SNDPBX.MAC
00 ROM:7F61 off_7F61:       fdb Reset_Vector        ; Sound/speech function pointer table
01 ROM:7F63                 fdb FX_loc_743C
02 ROM:7F65                 fdb Mus_sub_7686
03 ROM:7F67                 fdb Spch_loc_7BAB
04 ROM:7F69                 fdb Spch_loc_7C0D
05 ROM:7F6B                 fdb Spch_loc_7BBA
06 ROM:7F6D                 fdb Spch_loc_7BB3
07 ROM:7F6F                 fdb Spch_loc_7C14
08 ROM:7F71                 fdb Spch_loc_7BC2
09 ROM:7F73                 fdb Spch_loc_7BDB
0a ROM:7F75                 fdb Spch_loc_7C1B
0b ROM:7F77                 fdb Spch_loc_7B7E
0c ROM:7F79                 fdb Spch_loc_7BFF
0d ROM:7F7B                 fdb Spch_loc_7BE3
0e ROM:7F7D                 fdb Spch_loc_7BD4
0f ROM:7F7F                 fdb Spch_loc_7B6D
10 ROM:7F81                 fdb Spch_sub_7B65       ; $10 Red five standing by
11 ROM:7F83                 fdb Spch_loc_7BA3       ; Remember
12 ROM:7F85                 fdb Spch_loc_7BF1       ; I can't shake him
13 ROM:7F87                 fdb Spch_loc_7C06       ; Look at the size of that thing
14 ROM:7F89                 fdb sub_7C40            ; Stay in attack formation
15 ROM:7F8B                 fdb Spch_loc_7BCB
16 ROM:7F8D                 fdb Spch_loc_7B90       ; Force is strong in this one
17 ROM:7F8F                 fdb Spch_loc_7B76       ; Red five I'm going in
18 ROM:7F91                 fdb Spch_loc_7BF8       ; $18 Luke trust me
19 ROM:7F93                 fdb Spch_loc_7B88       ; Use the force Luke
1a ROM:7F95                 fdb Spch_loc_7B9A       ; Yahoo you're all clear kid
1b ROM:7F97                 fdb Mus_sub_753C        ; High score
1c ROM:7F99                 fdb Mus_sub_755D        ; High score Cantina
1d ROM:7F9B                 fdb Mus_sub_7665        ; Imperial march
1e ROM:7F9D                 fdb Mus_sub_7644        ; Enter Death Star
1f ROM:7F9F                 fdb Mus_sub_757E        ; Death Star destroyed
20 ROM:7FA1                 fdb Mus_sub_7602        ; $20 Towers 1
21 ROM:7FA3                 fdb Mus_sub_759F        ; Towers 2
22 ROM:7FA5                 fdb Mus_sub_75C0        ; Trench
23 ROM:7FA7                 fdb Mus_sub_751B        ; Exhaust port hit torpedo
24 ROM:7FA9                 fdb Mus_sub_75E1        ; Space wave 1
25 ROM:7FAB                 fdb Mus_sub_7623        ; Space wave 2

26 ROM:7FAD                 fdb FX_loc_7401         ; Explosion
27 ROM:7FAF                 fdb FX_loc_73F5         ; Death Star explosion
28 ROM:7FB1                 fdb FX_loc_740D         ; $28 R2 dead
29 ROM:7FB3                 fdb FX_loc_7435         ; Memory fail boop
2a ROM:7FB5                 fdb FX_loc_73C5         ; Tie fighter pass
2b ROM:7FB7                 fdb FX_loc_73CB	    ; STOP PASSBY SOUND
2c ROM:7FB9                 fdb FX_loc_73D7         ; Tower pass
2d ROM:7FBB                 fdb FX_loc_7419         ; Laser fire ;PORTHOLE (ASSISTS PMSF2)
2e ROM:7FBD                 fdb FX_loc_73FB         ; Tie pass 2
2f ROM:7FBF                 fdb FX_loc_7407         ; R2 increasing power
30 ROM:7FC1                 fdb FX_loc_73B7         ; $30 R2 answer
31 ROM:7FC3                 fdb FX_loc_7420         ; R2
32 ROM:7FC5                 fdb FX_sub_73B0         ; R2 enter Death Star
33 ROM:7FC7                 fdb FX_loc_73D1         ; Shield hit
34 ROM:7FC9                 fdb FX_loc_7413		;PLAYER SHOT ALIEN SHOTS
35 ROM:7FCB                 fdb FX_loc_73EF		;ALIEN SHIP EXPLODE
36 ROM:7FCD                 fdb FX_loc_73E3		;TIE FIGHTER CANNON
37 ROM:7FCF                 fdb FX_loc_73E9		;TIE FIGHTER CANNON END SOUND
38 ROM:7FD1                 fdb FX_loc_7427		;THRUST TO DETH STAR
39 ROM:7FD3                 fdb FX_loc_73DD		;TOWER LAZARS
3a ROM:7FD5                 fdb snd_Fire_Guns		;X-WING LAZARS
3b ROM:7FD7                 fdb FX_loc_742E;YAY - TEST PASS
  */  
}

