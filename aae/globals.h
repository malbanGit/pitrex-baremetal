#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include "m6502/m6502.h"
#include "z80/z80.h"
//#include "m6809.h"
#include "dips.h"


extern UINT32 dwElapsedTicks;
#define str_eq(s1,s2)  (!strcmp ((s1),(s2))); //Equate string1 and sring2 true is equal
#define EPSILON 0.0001   // Define your own tolerance
#define FLOAT_EQ(x,v) (((v - EPSILON) < x) && (x <( v + EPSILON)))
#define twos_comp_val(num,bits) ((num&(1<<(bits-1)))?(num|~((1<<bits)-1)):(num&((1<<bits)-1)))

#define VECTOR 500
#define RASTER 501

#define CPU0 0
#define CPU1 1
#define CPU2 2
#define CPU3 3
#define CPU4 4



#define FUN_TEX   5
#define ART_TEX  10
#define GAME_TEX 15


#define MAXSTACK 8
#define VEC_SHIFT  16
#define NORMAL      1
#define FLIP        2
#define RRIGHT      3
#define RLEFT       4
// GENERAL FUNCTION DEFINES
#define SWAP(a, b)  {a ^= b; b ^= a; a ^= b;}
#define memrdwd_flip(address) ((GI[CPU0][pc+1]) | (GI[CPU0][pc]<<8)) 
#define memrdwd(address) ((GI[CPU0][pc]) | (GI[CPU0][pc+1]<<8)) /* LBO 062797 */
#define memrdwdf(address) ((vec_ram[pc+1]) | (vec_ram[pc]<<8)) 

#define bitget(p,m) ((p) & (m))
#define bitset(p,m) ((p) |= (m))
#define bitclr(p,m) ((p) &= ~(m))
#define bitflp(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))

//CPU and WRITE READ HANDLERS
#define READ_HANDLER(name)  static UINT8 name(UINT32 address, struct MemoryReadByte *psMemRead)
#define WRITE_HANDLER(name)  static void name(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite)
#define MEM_WRITE(name) struct MemoryWriteByte name[] = {
#define MEM_READ(name)  struct MemoryReadByte name[] = {
#define MEM_ADDR(start,end,routine) {start,end,routine},
#define MEM_END {(UINT32) -1,(UINT32) -1,NULL}};

//CPU PORT HANDLERS FOR the Z80
#define PORT_WRITE_HANDLER(name) static void name(UINT16 port, UINT8 data, struct z80PortWrite *pPW)
#define PORT_READ_HANDLER(name) static UINT16 name(UINT16 port, struct z80PortRead *pPR)
#define PORT_WRITE(name) struct z80PortWrite name[] = {
#define PORT_READ(name) struct z80PortRead name[] = {
#define PORT_ADDR(start,end,routine) {start,end,routine},
#define PORT_END { -1, -1,NULL}};

////////////////////////////////
//PALETTE SETTINGS 
#define VEC_BW_BI  0
#define VEC_BW_16  1
#define VEC_BW_64  2
#define VEC_BW_256 3
#define VEC_COLOR  4
#define RASTER_32  5

////////////////////////////////
//GAME DEFINES
////////////////////////////////
enum GameDef {  

//Lunar Lander Hardware
 LLANDER1,  
 LLANDER,   
//Asteroids Hardware
 METEORTS,  
 ASTEROCK,  
 ASTEROIB,  
 ASTEROI1,  
 ASTEROID,  
 ASTDELU1,  
 ASTDELU2,  
 ASTDELUX,  
//Midway Omega Race Hardware
 OMEGRACE,  
 DELTRACE,  
//BattleZone Hardware
 BZONE,     
 BZONE2,    
 BZONEC,    
 BZONEP,    
 REDBARON,  
 BRADLEY,   
//Spacduel Hardware
 SPACDUEL,  
 BWIDOW,    
 GRAVITAR,  
 GRAVITR2, 
 GRAVP,     
 LUNARBAT,  
 LUNARBA1,  
//Tempest Hardware
 TEMPESTM,  
 TEMPEST,   
 TEMPEST3,  
 TEMPEST2, 
 TEMPEST1,  
 TEMPTUBE,  
 ALIENST,   
 VBREAK,    
 VORTEX,    
//Sega G80 Vector Hardware
 ZEKTOR,    
 TACSCAN,   
 STARTREK,  
 SPACFURY, 
 SPACFURA,  
 SPACFURB,  
 ELIM2,     
 ELIM2A,    
 ELIM2C,    
 ELIM4,     
 ELIM4P,    
//Major Havoc Hardware
 MHAVOC,    
 MHAVOC2,   
 MHAVOCRV,  
 MHAVOCP,   
 ALPHAONE,  
 ALPHAONA,  
//Star Wars Hardware
 STARWARS,  
 STARWAR1,  
//Quantum Hardware
 QUANTUM1,  
 QUANTUM,  
 QUANTUMP,  
//Cinematronics Hardware
 SOLARQ,    
 STARCAS,   
 ARMORA,    
 BARRIER,   
 SUNDANCE,  
 WARRIOR,   
 RIPOFF,    
 TAILG,     
 STARHAWK,  
 SPACEWAR,  
 SPEEDFRK,  
 DEMON,     
 BOXINGB,   
 WOTW, 
 GALAGA
};
// STRUCTS AND GLOBAL VARIABLES START HERE


struct GameCPU
{
	int cpu_type;	/* see #defines below. */
	int cpu_freq;	/* in Hertz */
	int cpu_divisions; // Divisions per frame cycle
	int cpu_intpass_per_frame; //Passes from above before interrupt called.(Interrupt Period)
	int cpu_int_type;
	void (*int_cpu)(); //Interrupt Handler if a separate interrupt handler is required
	struct MemoryReadByte  *memory_read;
	struct MemoryWriteByte *memory_write;
	struct z80PortRead     *port_read;
	struct z80PortWrite    *port_write;
	
	//int (*vblank_interrupt)(void);
  //  int vblank_interrupts_per_frame;    /* usually 1 */
/* use this for interrupts which are not tied to vblank 	*/
/* usually frequency in Hz, but if you need 				*/
/* greater precision you can give the period in nanoseconds */
	//int (*timed_interrupt)(void);
	//int timed_interrupts_per_second;
/* pointer to a parameter to pass to the CPU cores reset function */
	//void *reset_param;
};

 struct game_rect
{
    int min_x,max_x;
    int min_y,max_y;
};


struct roms 
{
    char *filename;
    unsigned int loadAddr;
    int romSize;
    int loadtype;
};

struct artworks
{
    char *zipfile;
    char *filename;
    int type;
    int target;
};

//DEFINES FOR STRUCTS

#define HEADING_SIZE    (11)
#define OPTION_SIZE     (12)
#define NUM_SWITCHES    (5)

typedef char *HEADING;
typedef char *OPTIONS;



typedef struct {
      
    int next; // index of next entry in array
    int prev; // previous entry (if double-linked)    
	int  gamenum; 		//Short Name of game
    char glname[128];	    //Display name for Game  
	int extopt;   //Any extra options for each game  
	//int numbertag;
 } glist;                      //Only one gamelist at a time

extern glist gamelist[256];


typedef struct
{
 int Port;
 unsigned int Mask; //MIN | MAX when analog port
 HEADING Heading;
 unsigned int Default;  //Joystick | Keyboard Default values
 unsigned int Current;  //Key Value to Check
 unsigned int Joystick; //Joystick Value to Check
 unsigned int mouseb; // Mouse button to check
 int Type;     //KEY_NORMAL, KEY_IMPULSE, KEY_REPEAT, KEY_SHFT_REPEAT
 int Arg1;    //Lots of values | together
 int Arg2;    //Default for Analog Joystick
 int Amask;   //Mask for analog values
} GAMEKEYS;

//GAMEKEYS *gamekeys;
extern GAMEKEYS *MK;
extern GAMEKEYS *GK;
extern GAMEKEYS *FOO;

struct AAEDriver
{   
	const char *name;
	const char *desc;
	const struct roms *rom;
	
	int (*init_game)();
	void (*pre_run) (); //Things to set before each CPU run.
	void (*run_game)();
	void (*end_game)();
	
	DIPSWITCH *game_dips;
	GAMEKEYS *game_keys;
	
	char **game_samples;
	const struct artworks *artwork;
	
	int cpu_type[4];               //6502, etc...
	int cpu_freq[4];               // CPU Frequency
	int cpu_divisions[4];          // Divisions per frame cycle
	int cpu_intpass_per_frame[4];  // Passes from above before interrupt called.(Interrupt Period)
	int cpu_int_type[4];           // Main type of CPU interrupt  
	//int cpu_current[4]; 
    void (*int_cpu[4])(); //Interrupt Handler CPU 0/4
	
	const int fps;
	const int vid_type;
	const int rotation;
    const int gamerect[4];
    
 };
extern struct AAEDriver driver[];


#define MAX_ACPU 4
//RAM Variables
extern unsigned char *membuffer;
extern unsigned char vec_ram[0x1fff];
extern unsigned char *GI[5]; //Global 6502/Z80/6809 GameImage 

//CPU Contexts
extern CONTEXTM6502 *c6502[MAX_ACPU];
extern CONTEXTMZ80 cMZ80[MAX_ACPU];
//struct S68000CONTEXT c68k[MAX_ACPU];

extern int art_loaded[6];

//TEMPORARY GRAPHICS GLOBALS
extern int msx,msy,esx,esy; //Main full screen adjustments for debug
extern int b1sx,b1sy,b2sx,b2sy; //bezel full screen adjustments
extern float bezelzoom;
extern int bezelx;
extern int bezely;
extern float overalpha;
extern struct game_rect GameRect;

//GLOBAL AUDIO VARIABLES
extern int gammaticks; //Needed for Pokey Sound for Major Havoc
extern int chip;  //FOR POKEY            
extern int gain;  //FOR POKEY  
extern int BUFFER_SIZE;  //FOR POKEY

//SAMPLE *game_sounds[60]; //Global Samples
//AUDIOSTREAM *stream; //Global Streaming Sound 1
//AUDIOSTREAM *stream2; //Global Streaming Sound 2
//unsigned char  *soundbuffer;
//signed char *aybuffer;




extern int in_gui;
extern int frames; //Global Framecounter
extern int frameavg;
extern int testsw; //testswitch for many games

//Shared variable for GUI

extern int gamenum; //Global Gamenumber (really need this one)
extern int have_error; //Global Error handler
extern int showinfo; //Global info handler
extern int done; //End of emulation indicator
extern int paused; //Paused indicator
extern double fps_count; //FPS Counter
extern int showfps;   //ShowFPS Toggle
extern int show_menu; //ShowMenu Toggle
extern int showifo; //No clue what this does
extern int scalef; //SCALING FACOR FOR RASTER GAMES
extern int gamefps; //GAME REQUIRED FPS
extern int num_games; //Total number of games ?? needed?
extern int num_samples; //Total number of samples for selected game

//KEY VARIABLES
extern int mouseb[5];
extern int WATCHDOG;
extern int menulevel;//Top Level
extern int menuitem; //TOP VAL
extern int key_set_flag;
extern int total_length;

//Vector Video Variables
typedef struct colorsarray {int r,g,b;} colors; 
extern colors vec_colors[1024];

//Video VARS
extern int sx,ex,sy,ey;
extern int testblend;
typedef struct {
  
char rompath[256];

//int ksfps;
//int kquit;
int kreset;
int ktest;
int ktestadv;
//int kmenu;
int kcoin1;
int kcoin2;
int kstart1;
int kstart2;
int kstart3;
int kstart4;

int kpause;
int ksnap;
int klstate;
int ksstate;

int kp1left;
int kp1right;
int kp1up;
int kp1down;
int kp1but1;
int kp1but2;
int kp1but3;
int kp1but4;
int kp1but5;
int kp1but6;

int kp2left;
int kp2right;
int kp2up;
int kp2down;
int kp2but1;
int kp2but2;
int kp2but3;
int kp2but4;
int kp2but5;
int kp2but6;

int drawzero;
int widescreen;
int overlay;
int colordepth;
int screenw;
int screenh;
int windowed;
int language;
int translucent;
float translevel;    
int lives; 

int m_line;
int m_point;
int monitor;

float linewidth;
float pointsize;

int gamma;
int bright;
int contrast;
int gain;
int fire_point_size;
int explode_point_size;
//int colorhack;
int shotsize;
int cocktail;
int mainvol;
int pokeyvol;
int artwork;
int bezel;
int burnin;
int artcrop;
int vid_rotate;
int vecglow;
int vectrail;
int mouse1xs;
int mouse1ys;
int mouse1x_invert;
int mouse1y_invert;
int mouse1b1;
int mouse1b2;
int psnoise;
int hvnoise;
int pshiss;
int noisevol;
int snappng;
//int showfps;
int prescale;
int anisfilter;
int priority;
int forcesync;
int dblbuffer;
int showinfo; //Show readme info message
char *exrompath; //optional path for roms 
int hack;
int debug;
int kbleds;

}aae_settings;

extern aae_settings config;


#endif