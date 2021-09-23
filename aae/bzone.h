#ifndef BZONE_H
#define BZONE_H

#include "aaemain.h"

extern char *gamename[];
extern int gamenum;

#define RBDACMAX 192                           /* +5v */
#define RBDACMIN 64                             /* gnd */
#define RBDACCTR 128
#define RESPONSE 2                             /* just a guess */


int init_bzone();
void run_bzone();
void end_bzone();

/*
char *bzonesamples[]={"bzone.zip","fire1.wav","fire2.wav","engine1.wav","engine2.wav","explode1.wav","explode2.wav","NULL"};
char *rbsamples[]={"redbaron.zip","explode.wav","shot.wav","spin.wav","NULL"};

ROM_START(bzone)
 ROM_LOAD("036422.01", 0x3000, 0x800, ROM_LOAD_NORMAL)
 ROM_LOAD("036421.01", 0x3800, 0x800, ROM_LOAD_NORMAL)
 ROM_LOAD("036414.01", 0x5000, 0x800, ROM_LOAD_NORMAL)
 ROM_LOAD("036413.01", 0x5800, 0x800, ROM_LOAD_NORMAL)
 ROM_LOAD("036412.01", 0x6000, 0x800, ROM_LOAD_NORMAL)
 ROM_LOAD("036411.01", 0x6800, 0x800, ROM_LOAD_NORMAL)
 ROM_LOAD("036410.01", 0x7000, 0x800, ROM_LOAD_NORMAL)
 ROM_LOAD("036409.01", 0x7800, 0x800, ROM_LOAD_NORMAL)
 ROM_LOAD("036409.01", 0xf800, 0x800, ROM_LOAD_NORMAL)//reload
ROM_END

struct roms bzone2[] =
{
	{"036414a.01", 0x5000, 0x800, ROM_LOAD_NORMAL},
	{"036413.01", 0x5800, 0x800, ROM_LOAD_NORMAL},
	{"036412.01", 0x6000, 0x800, ROM_LOAD_NORMAL},
	{"036411.01", 0x6800, 0x800, ROM_LOAD_NORMAL},
	{"036410.01", 0x7000, 0x800, ROM_LOAD_NORMAL},
	{"036409.01", 0x7800, 0x800, ROM_LOAD_NORMAL},
	{"036409.01", 0xf800, 0x800, ROM_LOAD_NORMAL},//reload
	{"036422.01", 0x3000, 0x800, ROM_LOAD_NORMAL},
	{"036421.01", 0x3800, 0x800, ROM_LOAD_NORMAL},
	{NULL, 0, 0, 0}
};

struct roms bzonec[] =
{
	{"bz1g4800", 0x4800, 0x800, ROM_LOAD_NORMAL},
	{"bz1f5000", 0x5000, 0x800, ROM_LOAD_NORMAL},
	{"bz1e5800", 0x5800, 0x800, ROM_LOAD_NORMAL},
	{"bz1d6000", 0x6000, 0x800, ROM_LOAD_NORMAL},
	{"bz1c6800", 0x6800, 0x800, ROM_LOAD_NORMAL},
	{"bz1b7000", 0x7000, 0x800, ROM_LOAD_NORMAL},
	{"bz1a7800", 0x7800, 0x800, ROM_LOAD_NORMAL},//reload
	{"bz1a7800", 0xf800, 0x800, ROM_LOAD_NORMAL},//reload
	{"036422.01", 0x3000, 0x800, ROM_LOAD_NORMAL},
	{"bz3b3800", 0x3800, 0x800, ROM_LOAD_NORMAL},
	{NULL, 0, 0, 0}
};

struct roms bzonep[] =
{
	{"036414.01.bin", 0x5000, 0x800, ROM_LOAD_NORMAL},
	{"036413.01.bin", 0x5800, 0x800, ROM_LOAD_NORMAL},
	{"036412.01.bin", 0x6000, 0x800, ROM_LOAD_NORMAL},
	{"036411.01.bin", 0x6800, 0x800, ROM_LOAD_NORMAL},
	{"036410.01.bin", 0x7000, 0x800, ROM_LOAD_NORMAL},
	{"036409.01.bin", 0x7800, 0x800, ROM_LOAD_NORMAL},
	{"036408.01.bin", 0x8000, 0x800, ROM_LOAD_NORMAL},
	{"036408.01.bin", 0xf800, 0x800, ROM_LOAD_NORMAL},//reload
	{"036422.01.bin", 0x3000, 0x800, ROM_LOAD_NORMAL},
	{"036421.01.bin", 0x3800, 0x800, ROM_LOAD_NORMAL},
	{NULL, 0, 0, 0}
};

struct roms bradley[] =
{
	{"btc1.bin", 0x4000, 0x800, ROM_LOAD_NORMAL},
	{"btd1.bin", 0x4800, 0x800, ROM_LOAD_NORMAL},
	{"bte1.bin", 0x5000, 0x800, ROM_LOAD_NORMAL},
	{"bth1.bin", 0x5800, 0x800, ROM_LOAD_NORMAL},
	{"btj1.bin", 0x6000, 0x800, ROM_LOAD_NORMAL},
	{"btk1.bin", 0x6800, 0x800, ROM_LOAD_NORMAL},
	{"btlm.bin", 0x7000, 0x800, ROM_LOAD_NORMAL},
	{"btn1.bin", 0x7800, 0x800, ROM_LOAD_NORMAL},
	//{"036408.01.bin", 0x8000, 0x800, ROM_LOAD_NORMAL},
	{"btn1.bin", 0xf800, 0x800, ROM_LOAD_NORMAL},//reload
	{"btb3.bin", 0x3000, 0x800, ROM_LOAD_NORMAL},
	{"bta3.bin", 0x3800, 0x800, ROM_LOAD_NORMAL},
	{NULL, 0, 0, 0}
};


struct roms redbaron[] =
{
  { "037006.01e", 0x3000, 0x0800, ROM_LOAD_NORMAL},
  { "037007.01e", 0x3800, 0x0800, ROM_LOAD_NORMAL },
  { "037587.01",  0x4800, 0x1000, ROM_LOAD_NORMAL },
  { "037587.01",  0x5000, 0x1000, ROM_LOAD_NORMAL },
  { "037000.01e", 0x5000, 0x0800, ROM_LOAD_NORMAL },
  { "036998.01e", 0x6000, 0x0800, ROM_LOAD_NORMAL },
  { "036997.01e", 0x6800, 0x0800, ROM_LOAD_NORMAL },  
  { "036996.01e", 0x7000, 0x0800, ROM_LOAD_NORMAL },
  { "036995.01e", 0x7800, 0x0800, ROM_LOAD_NORMAL },
  { "036995.01e", 0xf800, 0x0800, ROM_LOAD_NORMAL },
  {NULL, 0, 0, 0}
	
};

static GAMEKEYS bzone_keys[] = {
    { {"Player 1 Start"},START1,KEY_1,KEY_1,0,0,0,0,0 },
	{ {"Player 2 Start"},START2,KEY_2,KEY_2,0,0,0,0,0 },
	{ {"Coin 1"},COIN1,KEY_5,KEY_5,0,0,0,0,0 },
	{ {"Fire"},P1BUTTON1,KEY_LCONTROL,KEY_LCONTROL,0,0,0,0,0 },
	{ {"Right/Up"},P1BUTTON3,KEY_I,KEY_I,0,0,0,0,0 },
	{ {"Right/Down"},P1BUTTON4,KEY_K,KEY_K,0,0,0,0,0 },	
	{ {"Left/Up"},P1BUTTON5,KEY_E,KEY_E,0,0,0,0,0 },
	{ {"Left/Down"},P1BUTTON6,KEY_D,KEY_D,0,0,0,0,0 },	
	{ {"Single Stick Right"},P1RIGHT,KEY_RIGHT,KEY_RIGHT,0,0,0,0,0 },
	{ {"Single Stick Left"},P1LEFT,KEY_LEFT,KEY_LEFT,0,0,0,0,0 },	
	{ {"Single Stick Up"},P1UP,KEY_UP,KEY_UP,0,0,0,0,0 },
	{ {"Single Stick Down"},P1DOWN,KEY_DOWN,KEY_DOWN,0,0,0,0,0 },	
	{ {"NONE"},-1,-1,-1,-1,-1,-1,-1,-1 }
};
static GAMEKEYS redbaron_keys[] = {
    { {"Player 1 Start"},START1,KEY_1,KEY_1,0,0,0,0,0 },
	{ {"Coin 1"},COIN1,KEY_5,KEY_5,0,0,0,0,0 },
	{ {"Right"},P1RIGHT,KEY_RIGHT,KEY_RIGHT,0,0,0,0,0 },
	{ {"Left"},P1LEFT,KEY_LEFT,KEY_LEFT,0,0,0,0,0 },	
	{ {"Up"},P1UP,KEY_UP,KEY_UP,0,0,0,0,0 },
	{ {"Down"},P1DOWN,KEY_DOWN,KEY_DOWN,0,0,0,0,0 },	
	{ {"Fire"},P1BUTTON1,KEY_LCONTROL,KEY_LCONTROL,0,0,0,0,0 },
	
	{ {"NONE"},-1,-1,-1,-1,-1,-1 }
};

DIP_START(bzone_dips)
  
   { "PLAY MODE ", {"FREE PLAY", "1 COIN/2 PLAYS", "1 COIN/1 PLAY", "2 COINS/1 PLAY"},
   3, 0x0c00, 0x00, 2, 2, {0, 1, 2, 3 }},	 
   { "LIVES", {"2 TANKS", "3 TANKS", "4 TANKS ", "5 TANKS "},
   3, 0x0a00, 0xfc, 1, 1,{0, 1, 2, 3 }},
   { "MISSILE AT", {"5000 POINTS", "10000 POINTS", "20000 POINTS", "30000 POINTS"},
   3, 0x0a00, 0xf3, 1, 1,{0, 0x04, 0x08, 0x0c }},
   { "BONUS TANK", {"15K AND 100K", "25K AND 100K", "50K AND 100K", " "},
   2, 0x0a00, 0xcf, 0, 0,{0, 0x10, 0x20, 0x30 }},
   { "LANGUAGE", {"ENGLISH", "GERMAN", "FRENCH", "SPANISH"},
   3, 0x0a00, 0x3f, 0, 0,{0, 0x40, 0x80, 0xc0 }},
   { "COCKTAIL", {"NO", "YES", " ", " "},
   1, 0x7777, 0, 0, 0,{0, 1, 0, 0, }},
   { "SINGLE STICK", {"NO", "YES", " ", " "},
   1, 0x8888, 0, 0, 0,{0, 1,0,0 }},  
DIP_END

DIP_START(rbaron_dips)
  
   { "PLAY MODE ", {"FREE PLAY", "1 COIN/2 PLAYS", "1 COIN/1 PLAY", "2 COINS/1 PLAY"},
   3, 0x0a00, 0x00, 2, 2, {0xff, 0xfe, 0xfd, 0xfc }},	 
    { "LANGUAGE", {"GERMAN", "FRENCH", "SPANISH", "ENGLISH"},
   3, 0x0c00, 0xfc, 3, 3,{0, 0x1, 0x2, 0x3 }},
   { "BONUS PLANE", {"2k 10k 30k", "4k 15k 40k", "6k 20k 50k", "NONE "},
   2, 0x0c00, 0xf3, 2, 2,{0xc0, 0x8, 0x4, 0x00 }},
   { "PLANES", {"2", "3", "4 ", "5"},
   3, 0x0c00, 0xcf, 1, 1,{0x30, 0x20, 0x10, 0x0 }},
   { "MINIMUM PLAY", {"OFF", "ON", " ", " "},
   1, 0x0c00, 0xbf, 0, 0,{0x40, 0x00, 0, 0 }},
   { "SELF ADJUST", {"NO", "YES", " ", " "},
   1, 0x0c00, 0x7f, 0, 0,{0x80, 0x00, 0, 0 }},
DIP_END
*/

#endif