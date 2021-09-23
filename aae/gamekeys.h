#ifndef GAMEKEYS_H
#define GAMEKEYS_H

#include "globals.h"
#include "input.h"

/*
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

#define IP_NAME_DEFAULT ((const char *)-1)
#define GKEY_IMP(Port,Mask,Heading,Default,Type,Arg1)  {Port,Mask,Heading,Default,0,0,0,Type,Arg1,-1, -1},
#define GKEY_SET(Port,Mask,Heading,Default,Type)  {Port,Mask,Heading,Default,0,0,0,Type,-1,-1, -1},
#define GKEY_ANALOG(Port,Mask,Heading,Default,Current,Joystick,Type,Arg1,Arg2,Amask)  {Port,Mask,Heading,Default,Current,Joystick,0,Type,Arg1,Arg2,Amask},

GKEY_START(tacscan_keys)
 GKEY_SET(0,0xe0,"Default",0,       IN_DEFAULT)
 GKEY_IMP(0,0x20,"Coin 1",          IN_COIN1,              IN_ACTIVE_LOW | INF_IMPULSE, 3)
 GKEY_IMP(0,0x40,"Coin 2",          IN_COIN2,              IN_ACTIVE_LOW | INF_IMPULSE, 3)
 GKEY_SET(1,0xf0,"Default",0,       IN_DEFAULT)
 GKEY_SET(2,0xf0,"Default",0,       IN_DEFAULT)
 GKEY_SET(3,0xf0,"Default",0,       IN_DEFAULT)
 GKEY_SET(4,0x01,"Player 1 Start",  IN_START1,             IN_ACTIVE_HIGH)
 GKEY_SET(4,0x02,"Player 2 Start",  IN_START2,             IN_ACTIVE_HIGH)
 GKEY_SET(4,0x04,"Fire",            IN_P1B1,               IN_ACTIVE_HIGH)
 GKEY_SET(4,0x08,"Grab",            IN_P1B2,               IN_ACTIVE_HIGH)
 GKEY_SET(5,0x01,"Tac Left",        IN_P1LEFT,             IN_ACTIVE_HIGH)
 GKEY_ANALOG(6,25,"Track Analog X", MOUSE_X, 0x00, 0xff,  INF_PORTRESET | IN_ANALOG , 10, 0,0xff )
 GKEY_SET(7,0x01,"Tac Right",   IN_P1RIGHT,            IN_ACTIVE_HIGH)
GKEY_END

 * */
#define IN_NOT  -2

#define GKEY_START(name) static GAMEKEYS name[] = {
//#define GKEY_ANALOG(Port,Sensitivity,Heading,Default,Type,Min,Max,KeyDelta,Center) 
#define GKEY_END  {-1,-1,"NONE",-1,-1,-1,-1,-1,-1 }};

//#define PORT_ANALOG(mask,default,type,sensitivity,delta,min,max) 

 GKEY_START(default_keys)

    GKEY_SET(14,0x00,"Default",           0,IN_DEFAULT)
    GKEY_SET(14,0x04,"UI Up",			  IN_UIUP,		IN_ACTIVE_HIGH | INF_COUNTER)
	GKEY_SET(14,0x08,"UI Down",		      IN_UIDOWN,	IN_ACTIVE_HIGH | INF_COUNTER)
	GKEY_SET(14,0x10,"UI Right",		  IN_UILEFT,	IN_ACTIVE_HIGH | INF_COUNTER)
   	GKEY_SET(14,0x20,"UI Left",			  IN_UIRIGHT,   IN_ACTIVE_HIGH | INF_COUNTER)
	GKEY_IMP(14,0x40,"UI Select",		  IN_UISELECT,	IN_ACTIVE_HIGH | INF_IMPULSE, 1)
	GKEY_IMP(14,0x80,"UI Cancel",		  IN_UICANCEL,	IN_ACTIVE_HIGH | INF_IMPULSE, 1)
	
	//GKEY_SET(15,0x00,"Default",           0,IN_DEFAULT)
	GKEY_SET(15,0x01,"Test Advance",	  IN_TEST_ADV,	IN_ACTIVE_HIGH)
	GKEY_SET(15,0x02,"Test Switch",		  IN_TEST,		INF_TOGGLE |IN_ACTIVE_HIGH)
	GKEY_SET(15,0x00,"Default",           0,IN_DEFAULT)
	GKEY_IMP(15,0x04,"Reset",			  IN_RESET,		IN_ACTIVE_HIGH | INF_IMPULSE, 1)
	GKEY_IMP(15,0x08,"SnapShot",		  IN_SNAP,		IN_ACTIVE_HIGH | INF_IMPULSE, 1)
	GKEY_IMP(15,0x10,"Show FPS",		  IN_SHOW_FPS,	IN_ACTIVE_HIGH | INF_IMPULSE, 1)
   	GKEY_IMP(15,0x20,"MENU",			  IN_MENU,      IN_ACTIVE_HIGH | INF_IMPULSE, 1)
	GKEY_IMP(15,0x40,"Pause",			  IN_PAUSE,		IN_ACTIVE_HIGH | INF_IMPULSE, 1)
	GKEY_IMP(15,0x80,"Quit",			  IN_QUIT,		IN_ACTIVE_HIGH | INF_IMPULSE, 1)

	GKEY_SET(16,1,"Player 1 Start",    IN_START1,	IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 2 Start",    IN_START2,	IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 3 Start",    IN_START3,	IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 4 Start",    IN_START4,	IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Coin 1",			   IN_COIN1,	IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Coin 2",			   IN_COIN2,	IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 1 Right",    IN_P1RIGHT,	IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 1 Left",	   IN_P1LEFT,	IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 1 Up",	   IN_P1UP,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 1 Down",	   IN_P1DOWN,	IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 1 Button 1", IN_P1B1,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 1 Button 2", IN_P1B2,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 1 Button 3", IN_P1B3,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 1 Button 4", IN_P1B4,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 1 Button 5", IN_P1B5,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 1 Button 6", IN_P1B6,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 1 Button 7", IN_P1B7,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 1 Button 8", IN_P1B8,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 1 Button 9", IN_P1B9,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 1 Button 10", IN_P1B10,	IN_ACTIVE_HIGH)
	

	GKEY_SET(16,1,"Player 2 Right",	  IN_P2RIGHT,	IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 2 Left",	  IN_P2LEFT,	IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 2 Up",		  IN_P2UP,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 2 Down",	  IN_P2DOWN,	IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 2 Button 1", IN_P2B1,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 2 Button 2", IN_P2B2,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 2 Button 3", IN_P2B3,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 2 Button 4", IN_P2B4,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 2 Button 5", IN_P2B5,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 2 Button 6", IN_P2B6,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 2 Button 7", IN_P2B7,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 2 Button 8", IN_P2B8,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 2 Button 9", IN_P2B9,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Player 2 Button 10", IN_P2B10,	IN_ACTIVE_HIGH)
  
	GKEY_SET(16,1,"Pad 0",			  IN_PAD_0,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Pad 1",			  IN_PAD_1,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Pad 2",			  IN_PAD_2,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Pad 3",			  IN_PAD_3,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Pad 4",			  IN_PAD_4,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Pad 5",			  IN_PAD_5,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Pad 6",			  IN_PAD_6,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Pad 7",			  IN_PAD_7,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Pad 8",			  IN_PAD_8,		IN_ACTIVE_HIGH)
	GKEY_SET(16,1,"Pad 9",			  IN_PAD_9,		IN_ACTIVE_HIGH)
  GKEY_END


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
  
  
// major havoc does not use this config!
GKEY_START(mhavoc_keys)
 GKEY_SET(0,0xf0,"Default",         0,IN_DEFAULT)
 GKEY_SET(0,0x20,"Test Advance",  IN_TEST_ADV,	   		   IN_ACTIVE_LOW)
 GKEY_SET(0,0x40,"Coin 1",          IN_COIN1,      IN_ACTIVE_LOW)
 GKEY_SET(0,0x80,"Coin 2",          IN_COIN2,      IN_ACTIVE_LOW)
 
 
 GKEY_SET(1,0xff,"Default",         0,IN_DEFAULT)
 GKEY_SET(1,0x80,"Test Switch",     IN_TEST,       INF_TOGGLE | IN_ACTIVE_LOW )
 GKEY_SET(2,0xfb,"Default",         0,IN_DEFAULT) 
 GKEY_SET(2,0x80,"P1 Start/Jump",    MOUSE_B1 |IN_P1B1,       IN_ACTIVE_LOW)
 GKEY_SET(2,0x40,"P1 Shield",        MOUSE_B2 |IN_P1B2,       IN_ACTIVE_LOW)
 GKEY_SET(2,0x20,"P2 Start/Jump",   IN_P2B1,       IN_ACTIVE_LOW)
 GKEY_SET(2,0x10,"P2 Shield",       IN_P2B2,       IN_ACTIVE_LOW)
 
 GKEY_SET(3,0x01,"Digi Roll Left",  IN_P1LEFT,     IN_ACTIVE_HIGH)
 GKEY_ANALOG(4,100,"Analog Roller", MOUSE_X,0, 0, INF_REVERSE|IN_ANALOG , 40 , 0, 0xff )
 GKEY_SET(5,0x01,"Digi Roll Right", IN_P1RIGHT,    IN_ACTIVE_HIGH)
GKEY_END


// omega race does not use this config!
GKEY_START(omega_keys)
 GKEY_SET(0,0x01,"Player 1 Start", IN_START1,		IN_ACTIVE_HIGH)
 GKEY_SET(0,0x01,"Player 2 Start", IN_START2,		IN_ACTIVE_HIGH)
 GKEY_SET(0,0x01,"Coin 1",		   IN_COIN1,		IN_ACTIVE_HIGH)
 GKEY_SET(0,0x01,"Rotate Left",	   IN_P1LEFT,		IN_ACTIVE_HIGH)
 GKEY_SET(0,0x01,"Rotate Right",   IN_P1RIGHT,		IN_ACTIVE_HIGH)
 GKEY_SET(0,0x01,"Fire",		   IN_P1B1,			IN_ACTIVE_HIGH)
 GKEY_SET(0,0x01,"Thrust",		   IN_P1B2,			IN_ACTIVE_HIGH)
GKEY_END


GKEY_START(tailg_keys)
 GKEY_SET(0,0xffff,"Default",0,     IN_DEFAULT)
 GKEY_SET(0,0x0080,"Player 1 Start",IN_START1,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0020,"P1 Fire",      MOUSE_B1 | IN_P1B1,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0040,"P1 Shield",    MOUSE_B2 | IN_P1B2,       IN_ACTIVE_LOW)
 GKEY_SET(1,0xffff,"Default",0,     IN_DEFAULT)
 
 GKEY_SET(1,0x80,"Coin 1",        IN_COIN1,      IN_ACTIVE_LOW)
 GKEY_SET(2,0x01,"Analog Up",     IN_P1UP,      IN_ACTIVE_HIGH)
 GKEY_ANALOG(3,100,"Analog Y",IN_AD_STICK_Y, 0x200, 0xe00,   INF_REVERSE | IN_ANALOG ,80,0x800,0xfff)
 GKEY_SET(4,0x01,"Analog Down",   IN_P1DOWN,    IN_ACTIVE_HIGH)
 GKEY_SET(7,0x01,"Analog Left",   IN_P1LEFT,    IN_ACTIVE_HIGH)
 GKEY_ANALOG(6,100,"Analog X ",IN_AD_STICK_X, 0x200, 0xe00,   IN_ANALOG ,80,0x800,0xfff)
 GKEY_SET(5,0x01,"Analog Right",  IN_P1RIGHT,   IN_ACTIVE_HIGH) 
GKEY_END










GKEY_START(gui_keys)
    GKEY_SET(0,0x04,"Move Up",			IN_P1UP,	IN_ACTIVE_HIGH | INF_COUNTER)
	GKEY_SET(0,0x08,"Move Down",		IN_P1DOWN,	IN_ACTIVE_HIGH | INF_COUNTER)
	GKEY_SET(0,0x10,"Move Right",		IN_P1LEFT,	IN_ACTIVE_HIGH | INF_COUNTER)
   	GKEY_SET(0,0x20,"Move Left",		IN_P1RIGHT, IN_ACTIVE_HIGH | INF_COUNTER)
	GKEY_IMP(0,0x40,"Fire",				IN_P1B1 | MOUSE_B1 |IN_P1B1,	IN_ACTIVE_HIGH | INF_IMPULSE, 1)
	//GKEY_IMP(0,0x80,"Exit",			    MOUSE_B2 | IN_P1B2,	IN_ACTIVE_HIGH | INF_IMPULSE, 1)
	GKEY_SET(1,0x01,"Track Left",       IN_PAD_4,   IN_ACTIVE_HIGH)
    GKEY_ANALOG(2,100,"Track Analog X", MOUSE_X,0, 0, IN_ANALOG ,1 , 0, 0x0f )
    GKEY_SET(3,0x01,"Track Right",      IN_PAD_6,   IN_ACTIVE_HIGH)	
    GKEY_SET(4,0x01,"Track Up",         IN_PAD_8,   IN_ACTIVE_HIGH)
    GKEY_ANALOG(5,100,"Track Analog Y", MOUSE_Y, 0, 0, INF_REVERSE | IN_ANALOG,1 , 0,0x0f )
    GKEY_SET(6,0x01,"Track Down",       IN_PAD_2,   IN_ACTIVE_HIGH)
GKEY_END	


GKEY_START(asteroid_keys)
 GKEY_SET(0,0x08,"Hyperspace",		IN_P1B3,		IN_ACTIVE_HIGH)
 GKEY_SET(0,0x10,"Fire",			IN_P1B1,		IN_ACTIVE_HIGH)
 GKEY_SET(0,0x80,"Test",			IN_TEST,		INF_TOGGLE | IN_ACTIVE_HIGH )
 GKEY_SET(0,0x80,"Reset",			IN_RESET,		INF_RESETCPU)
 
 GKEY_SET(1,0x01,"Coin 1",			IN_COIN1,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x02,"Coin 2",		    IN_COIN2,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x08,"Player 1 Start",  IN_START1,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x10,"Player 2 Start",  IN_START2,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x20,"Thrust",			IN_P1B2,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x40,"Rotate Right",	IN_P1RIGHT,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x80,"Rotate Left",		IN_P1LEFT,		IN_ACTIVE_HIGH)
GKEY_END

GKEY_START(astdelux_keys)
 GKEY_SET(0,0x08,"Shield",			IN_P1B3,		IN_ACTIVE_HIGH)
 GKEY_SET(0,0x10,"Fire",			IN_P1B1,		IN_ACTIVE_HIGH)
 GKEY_SET(0,0x80,"Test",			IN_TEST,		INF_TOGGLE | IN_ACTIVE_HIGH )
 GKEY_SET(0,0x80,"Reset",			IN_RESET,		INF_RESETCPU )

 GKEY_SET(1,0x01,"Coin 1",			IN_COIN1,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x02,"Coin 2",			IN_COIN2,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x08,"Player 1 Start",	IN_START1,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x10,"Player 2 Start",	IN_START2,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x20,"Thrust",			IN_P1B2,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x40,"Rotate Right",	IN_P1RIGHT,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x80,"Rotate Left",		IN_P1LEFT,		IN_ACTIVE_HIGH)
GKEY_END

GKEY_START(bwidow_keys)
 GKEY_SET(0,0x7f,"Default",0,	   IN_DEFAULT)
 GKEY_SET(0,0x01,"Coin 1",		   IN_COIN1,			IN_ACTIVE_LOW)
 GKEY_SET(0,0x02,"Coin 2",         IN_COIN2,			IN_ACTIVE_LOW)
 GKEY_SET(1,0xff,"Default",0,      IN_DEFAULT)
 GKEY_SET(1,0x08,"Move Up",		   IN_LEFT_P1UP,		IN_ACTIVE_LOW)
 GKEY_SET(1,0x01,"Move Right",	   IN_LEFT_P1RIGHT,		IN_ACTIVE_LOW)
 GKEY_SET(1,0x02,"Move Left",	   IN_LEFT_P1LEFT,		IN_ACTIVE_LOW)
 GKEY_SET(1,0x04,"Move Down",	   IN_LEFT_P1DOWN,		IN_ACTIVE_LOW)
 GKEY_SET(2,0xff,"Default",0,      IN_DEFAULT)
 GKEY_SET(2,0x01,"Fire Right",	   IN_RIGHT_P2RIGHT, 	IN_ACTIVE_LOW)
 GKEY_SET(2,0x02,"Fire Left",	   IN_RIGHT_P2LEFT,		IN_ACTIVE_LOW)
 GKEY_SET(2,0x04,"Fire Down",	   IN_RIGHT_P2DOWN,		IN_ACTIVE_LOW)
 GKEY_SET(2,0x08,"Fire Up",		   IN_RIGHT_P2UP,		IN_ACTIVE_LOW)
 GKEY_SET(2,0x20,"Player 1 Start", IN_START1,			IN_ACTIVE_LOW)
 GKEY_SET(2,0x40,"Player 2 Start", IN_START2,			IN_ACTIVE_LOW)
GKEY_END

GKEY_START(llander_keys)
 GKEY_SET(0,0xff,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x01,"Player Start",	IN_START1,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x10,"Game Select",		IN_START2,		IN_ACTIVE_HIGH)
 GKEY_SET(0,0x80,"Test",			IN_TEST,		INF_TOGGLE | IN_ACTIVE_LOW )
 GKEY_SET(0,0x80,"Reset",			IN_RESET,		INF_RESETCPU )
 GKEY_SET(1,0x02,"Coin 1",			IN_COIN1,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x20,"Abort",			IN_P1B3,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x80,"Rotate Left",		IN_P1LEFT,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x40,"Rotate Right",	IN_P1RIGHT,		IN_ACTIVE_HIGH)
 GKEY_SET(4,0x01,"Thrust Up",		IN_P1B1,		IN_ACTIVE_HIGH)
 GKEY_ANALOG(5,100,"Thrust Analog ",IN_AD_STICK_Y, 0x01, 0xfe,  INF_CENTER | INF_REVERSE | IN_ANALOG ,1,0,0xff)
 GKEY_SET(6,0x01,"Thrust Down",		IN_P1B2,		IN_ACTIVE_HIGH)
GKEY_END

GKEY_START(gravitar_keys)
 GKEY_SET(0,0x7f,"Default",0,		IN_DEFAULT)
 GKEY_SET(0,0x01,"Coin 1",			IN_COIN1,IN_ACTIVE_LOW)
 GKEY_SET(0,0x02,"Coin 2",			IN_COIN2,IN_ACTIVE_LOW)
 GKEY_SET(1,0xff,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x01,"Tractor",			IN_P1B3,IN_ACTIVE_LOW)
 GKEY_SET(1,0x02,"Fire",			IN_P1B1,IN_ACTIVE_LOW)
 GKEY_SET(1,0x04,"Rotate Right",	IN_P1RIGHT,IN_ACTIVE_LOW)
 GKEY_SET(1,0x08,"Rotate Left",		IN_P1LEFT,IN_ACTIVE_LOW)
 GKEY_SET(1,0x10,"Thrust",			IN_P1B2,IN_ACTIVE_LOW)
 GKEY_SET(2,0xff,"Default",0,		IN_DEFAULT)
 GKEY_SET(2,0x20,"Player 1 Start",	IN_START1,IN_ACTIVE_LOW)
 GKEY_SET(2,0x40,"Player 2 Start",	IN_START2,IN_ACTIVE_LOW)
GKEY_END


GKEY_START(redbaron_keys)
 GKEY_SET(0,0x3f,"Default",0,       IN_DEFAULT)
 GKEY_SET(0,0x10,"Test",			IN_TEST,        INF_TOGGLE | IN_ACTIVE_LOW)
 GKEY_SET(0,0x20,"Test Advance",	IN_TEST_ADV,	IN_ACTIVE_LOW)
 GKEY_SET(0,0x80,"Reset",			IN_RESET,		INF_RESETCPU )
 GKEY_SET(0,0x01,"Coin 1",			IN_COIN1,		IN_ACTIVE_LOW)
 GKEY_SET(1,0x7f,"Default",0,       IN_DEFAULT)
 GKEY_SET(1,0x40,"Player 1 Start",  IN_START1,		IN_ACTIVE_LOW)
 GKEY_SET(1,0x80,"Fire",			MOUSE_B1 | IN_P1B1,		IN_ACTIVE_HIGH)
 GKEY_SET(2,0x01,"Stick Up",        IN_P1UP,        IN_ACTIVE_HIGH)
 GKEY_ANALOG(3,25,"Analog Y",       IN_AD_STICK_Y, 64, 190,   IN_ANALOG ,10,0x80,0xff)//0x80 center
 GKEY_SET(4,0x01,"Stick Down",      IN_P1DOWN,      IN_ACTIVE_HIGH)
 GKEY_SET(5,0x01,"Stick Left",      IN_P1LEFT,      IN_ACTIVE_HIGH)
 GKEY_ANALOG(6,25,"Analog X ",      IN_AD_STICK_X, 64, 190,   IN_ANALOG ,10,0x80,0xff)
 GKEY_SET(7,0x01,"Stick Right",     IN_P1RIGHT,     IN_ACTIVE_HIGH)	
GKEY_END

GKEY_START(bzone_keys)
 GKEY_SET(0,0x3f,"Default",0,			IN_DEFAULT)
 GKEY_SET(0,0x10,"Test",				IN_TEST,          INF_TOGGLE | IN_ACTIVE_LOW)
 GKEY_SET(0,0x20,"Test Advance",		IN_TEST_ADV,	  IN_ACTIVE_LOW)
 GKEY_SET(0,0x80,"Reset",				IN_RESET,		  INF_RESETCPU )
 GKEY_SET(0,0x01,"Coin 1",			    IN_COIN1,		  IN_ACTIVE_LOW)
 GKEY_SET(0,0x02,"Coin 2",			    IN_COIN2,		  IN_ACTIVE_LOW)
 
 GKEY_SET(1,0x80,"Default",0,			IN_DEFAULT)
 GKEY_SET(1,0x20,"Player 1 Start",		IN_START1,		  IN_ACTIVE_HIGH)
 GKEY_SET(1,0x40,"Player 2 Start",		IN_START2,		  IN_ACTIVE_HIGH)
 GKEY_SET(1,0x10,"Fire",				IN_P1B1,		  IN_ACTIVE_HIGH)
 GKEY_SET(1,0x02,"Right/Up",			IN_RIGHT_P2UP,	  IN_ACTIVE_HIGH)
 GKEY_SET(1,0x01,"Right/Down",			IN_RIGHT_P2DOWN,  IN_ACTIVE_HIGH)
 GKEY_SET(1,0x08,"Left/Up",				IN_LEFT_P1UP,	  IN_ACTIVE_HIGH)
 GKEY_SET(1,0x04,"Left/Down",			IN_LEFT_P1DOWN,	  IN_ACTIVE_HIGH)
 GKEY_SET(2,0x01,"Single Stick Up",		IN_P1UP,		  IN_ACTIVE_HIGH)
 GKEY_SET(2,0x02,"Single Stick Down",	IN_P1DOWN,		  IN_ACTIVE_HIGH)
 GKEY_SET(2,0x04,"Single Stick Left",	IN_P1LEFT,		  IN_ACTIVE_HIGH)
 GKEY_SET(2,0x08,"Single Stick Right",	IN_P1RIGHT,		  IN_ACTIVE_HIGH)
GKEY_END

GKEY_START(tempest_keys)
 GKEY_SET(0,0x7f,"Default",0,			IN_DEFAULT)
 GKEY_SET(0,0x10,"Test",				IN_TEST,   INF_TOGGLE | IN_ACTIVE_LOW)
 GKEY_SET(0,0x20,"Test Advance",		IN_TEST_ADV,			IN_ACTIVE_LOW)
 GKEY_SET(0,0x01,"Reset",				IN_RESET,				INF_RESETCPU )
 GKEY_SET(0,0x04,"Coin 1",				IN_COIN1,				IN_ACTIVE_LOW)
 GKEY_SET(0,0x02,"Coin 2",				IN_COIN2,				IN_ACTIVE_LOW)
 GKEY_SET(1,0x20,"Player 1 Start",		IN_START1,				IN_ACTIVE_HIGH)
 GKEY_SET(1,0x40,"Player 2 Start",		IN_START2,				IN_ACTIVE_HIGH)
 GKEY_SET(1,0x10,"Fire",				MOUSE_B1 | IN_P1B1,     IN_ACTIVE_HIGH)
 GKEY_SET(1,0x08,"Zapper",				MOUSE_B2 | IN_P1B2,     IN_ACTIVE_HIGH)
 GKEY_SET(2,0x01,"Rotate Left",			IN_P1LEFT,				IN_ACTIVE_HIGH)
 GKEY_ANALOG(3,25,"Track Analog X",	MOUSE_X, 0xf, 0x0,  INF_REVERSE | IN_ANALOG ,10, 0,0x0f )//
 GKEY_SET(4,0x01,"Rotate Right",		IN_P1RIGHT,				IN_ACTIVE_HIGH)
GKEY_END

GKEY_START(tempestm_keys)
 GKEY_SET(0,0x7f,"Default",0,      IN_DEFAULT)
 GKEY_SET(0,0x10,"Test",           IN_TEST,    INF_TOGGLE |  IN_ACTIVE_LOW)
 GKEY_SET(0,0x20,"Test Advance",   IN_TEST_ADV,             IN_ACTIVE_LOW)
 GKEY_SET(0,0x01,"Reset",          IN_RESET,                INF_RESETCPU )
 GKEY_SET(0,0x04,"Coin 1",         IN_COIN1,                IN_ACTIVE_LOW)
 GKEY_SET(0,0x02,"Coin 2",         IN_COIN2,                IN_ACTIVE_LOW)
 GKEY_SET(1,0x20,"Player 1 Start", IN_START1,               IN_ACTIVE_HIGH)
 GKEY_SET(1,0x40,"Player 2 Start", IN_START2,               IN_ACTIVE_HIGH)
 GKEY_SET(1,0x10,"Fire",           MOUSE_B1 | IN_P1B1,      IN_ACTIVE_HIGH)
 GKEY_SET(1,0x08,"Zapper",         MOUSE_B2 | IN_P1B2,      IN_ACTIVE_HIGH)
 GKEY_SET(2,0x01,"Rotate Left",    IN_P1LEFT,               IN_ACTIVE_HIGH)
 GKEY_ANALOG(3,25,"Track Analog X",MOUSE_X, 0xf, 0x0,  INF_REVERSE | IN_ANALOG ,10, 0,0x0f )//
 GKEY_SET(4,0x01,"Rotate Right",   IN_P1RIGHT,              IN_ACTIVE_HIGH)
 GKEY_SET(5,0x01, "Reset to Menu", IN_TILT,   IN_ACTIVE_HIGH)
GKEY_END

GKEY_START(zektor_keys)
 GKEY_SET(0,0xe0,"Default",0,		IN_DEFAULT)
 GKEY_IMP(0,0x80,"Coin 1",			IN_COIN1,      IN_ACTIVE_LOW | INF_IMPULSE, 3)
 GKEY_IMP(0,0x40,"Coin 2",			IN_COIN2,      IN_ACTIVE_LOW | INF_IMPULSE, 3)
 GKEY_SET(1,0xf0,"Default",0,		IN_DEFAULT)
 GKEY_SET(2,0xf0,"Default",0,		IN_DEFAULT)
 GKEY_SET(3,0xf0,"Default",0,		IN_DEFAULT)
 GKEY_SET(4,0x01,"Player 1 Start",  IN_START1,     IN_ACTIVE_HIGH)
 GKEY_SET(4,0x02,"Player 2 Start",  IN_START2,     IN_ACTIVE_HIGH)
 GKEY_SET(4,0x04,"Fire",			MOUSE_B1 | IN_P1B1,       IN_ACTIVE_HIGH)
 GKEY_SET(4,0x08,"Thrust",			MOUSE_B2 | IN_P1B2,       IN_ACTIVE_HIGH)
 GKEY_SET(5,0x01,"Rotate Left",		IN_P1LEFT,             IN_ACTIVE_HIGH)
 GKEY_ANALOG(6,25,"Track Analog X", MOUSE_X, 0xff, 0x00,  INF_PORTRESET | IN_ANALOG , 12, 0,0xff )
 GKEY_SET(7,0x01,"Rotate Right",	IN_P1RIGHT,            IN_ACTIVE_HIGH)
GKEY_END

GKEY_START(tacscan_keys)
 GKEY_SET(0,0xe0,"Default",0,		IN_DEFAULT)
 GKEY_IMP(0,0x20,"Coin 1",			IN_COIN1,              IN_ACTIVE_LOW | INF_IMPULSE, 3)
 GKEY_IMP(0,0x40,"Coin 2",			IN_COIN2,              IN_ACTIVE_LOW | INF_IMPULSE, 3)
 GKEY_SET(1,0xf0,"Default",0,		IN_DEFAULT)
 GKEY_SET(2,0xf0,"Default",0,		IN_DEFAULT)
 GKEY_SET(3,0xf0,"Default",0,		IN_DEFAULT)
 GKEY_SET(4,0x01,"Player 1 Start",	IN_START1,             IN_ACTIVE_HIGH)
 GKEY_SET(4,0x02,"Player 2 Start",	IN_START2,             IN_ACTIVE_HIGH)
 GKEY_SET(4,0x04,"Fire",			IN_P1B1,               IN_ACTIVE_HIGH)
 GKEY_SET(4,0x08,"Grab",			IN_P1B2,               IN_ACTIVE_HIGH)
 GKEY_SET(5,0x01,"Tac Left",		IN_P1LEFT,             IN_ACTIVE_HIGH)
 GKEY_ANALOG(6,25,"Track Analog X", MOUSE_X, 0x00, 0xff,  INF_PORTRESET | IN_ANALOG , 10, 0,0xff )
 GKEY_SET(7,0x01,"Tac Right",	IN_P1RIGHT,            IN_ACTIVE_HIGH)
GKEY_END

GKEY_START(solarq_keys)
 GKEY_SET(0,0xffff,"Default",0,		IN_DEFAULT)
 GKEY_SET(0,0x0008,"Player 1 Start",	IN_START1,          IN_ACTIVE_LOW)
 GKEY_SET(0,0x10,"Player 2 Start",	IN_START2,          IN_ACTIVE_LOW)
 GKEY_SET(0,0x0020,"Move Left",		IN_P1LEFT,          IN_ACTIVE_LOW)
 GKEY_SET(0,0x0010,"Move Right",		IN_P1RIGHT,         IN_ACTIVE_LOW)
 GKEY_SET(0,0x0002,"Fire",			IN_P1B1,            IN_ACTIVE_LOW)
 GKEY_SET(0,0x0004,"Thrust",			IN_P1B2,            IN_ACTIVE_LOW)
 GKEY_SET(0,0x0008,"Hyper",			IN_P1B3,            IN_ACTIVE_LOW)
 GKEY_SET(0,0x0001,"NovaBomb",			IN_P1B4,            IN_ACTIVE_LOW)
 GKEY_SET(1,0x00a0,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x80,"Coin 1",          IN_COIN1,      IN_ACTIVE_LOW)
GKEY_END

GKEY_START(spacfury_keys)

 GKEY_SET(0,0xe0,"Default",0,		IN_DEFAULT)
 GKEY_IMP(0,0x80,"Coin 1",			IN_COIN1,      IN_ACTIVE_LOW | INF_IMPULSE, 3)
 GKEY_IMP(0,0x40,"Coin 2",			IN_COIN2,      IN_ACTIVE_LOW | INF_IMPULSE, 3)
 GKEY_SET(1,0xf0,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x20,"Player 1 Start", IN_START1,     IN_ACTIVE_LOW)
 GKEY_SET(1,0x10,"Player 2 Start", IN_START2,     IN_ACTIVE_LOW)
 GKEY_SET(2,0xf0,"Default",0,		IN_DEFAULT)
 GKEY_SET(2,0x20,"Rotate Left",	IN_P1LEFT,     IN_ACTIVE_LOW)
 GKEY_SET(2,0x10,"Rotate Right",	IN_P1RIGHT,    IN_ACTIVE_LOW)	
 GKEY_SET(3,0xf0,"Default",0,		IN_DEFAULT)
 GKEY_SET(3,0x10,"Fire",			IN_P1B1,       IN_ACTIVE_LOW)
 GKEY_SET(3,0x20,"Thrust",			IN_P1B2,       IN_ACTIVE_LOW)
GKEY_END

GKEY_START(startrek_keys)
 GKEY_SET(0,0xe0,"Default",0,		IN_DEFAULT)
 GKEY_IMP(0,0x20,"Coin 1",			IN_COIN1,              IN_ACTIVE_LOW | INF_IMPULSE, 3)
 GKEY_IMP(0,0x40,"Coin 2",			IN_COIN2,              IN_ACTIVE_LOW | INF_IMPULSE, 3)
 GKEY_SET(1,0xf0,"Default",0,		IN_DEFAULT)
 GKEY_SET(2,0xf0,"Default",0,		IN_DEFAULT)
 GKEY_SET(3,0xf0,"Default",0,		IN_DEFAULT)
 GKEY_SET(4,0x01,"Player 1 Start",	IN_START1,             IN_ACTIVE_HIGH)
 GKEY_SET(4,0x02,"Player 2 Start",	IN_START2,             IN_ACTIVE_HIGH)
 GKEY_SET(4,0x08,"Phasers",			IN_P1B1,               IN_ACTIVE_HIGH)
 GKEY_SET(4,0x04,"Photon Torpedos",	IN_P1B2,               IN_ACTIVE_HIGH)
 GKEY_SET(4,0x10,"Impulse Power",	IN_P1B3,       IN_ACTIVE_HIGH)
 GKEY_SET(4,0x20,"Warp Drive",		IN_P1B4,       IN_ACTIVE_HIGH)
 GKEY_SET(5,0x01,"Tac Left",		IN_P1LEFT,             IN_ACTIVE_HIGH)
 GKEY_ANALOG(6,50,"Track Analog X", MOUSE_X, 0x00, 0xff,  INF_PORTRESET | IN_ANALOG , 15, 0,0xff )
 GKEY_SET(7,0x01,"Tac Right",	IN_P1RIGHT,            IN_ACTIVE_HIGH) 
GKEY_END

GKEY_START(elim_keys)
 GKEY_SET(0,0,"Player 1 Start", IN_START1,     IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"Player 2 Start", IN_START2,     IN_ACTIVE_HIGH)
 GKEY_IMP(0,0,"Coin 1",			IN_COIN1,      IN_ACTIVE_HIGH | INF_IMPULSE, 3)
 GKEY_IMP(0,0,"Coin 2",			IN_COIN2,      IN_ACTIVE_HIGH | INF_IMPULSE, 3)
 GKEY_SET(0,0,"P1 Rotate Left",	IN_P1LEFT,     IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Rotate Right",IN_P1RIGHT,    IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Fire",		IN_P1B1,       IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Thrust",		IN_P1B2,       IN_ACTIVE_HIGH)
 
 GKEY_SET(0,0,"P2 Rotate Left", IN_P2LEFT,     IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P2 Rotate Right",IN_P2RIGHT,    IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P2 Fire",		IN_P2B1,       IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P2 Thrust",		IN_P2B2,       IN_ACTIVE_HIGH)
GKEY_END



GKEY_START(starcas_keys)
GKEY_SET(0,0xffff,"Default",0,		IN_DEFAULT)
 GKEY_SET(0,0x0001,"Player 1 Start", IN_START1,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0004,"Player 2 Start", IN_START2,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0040,"Move Left",		IN_P1LEFT,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0100,"Move Right", 	IN_P1RIGHT,    IN_ACTIVE_LOW)
 GKEY_SET(0,0x1000,"Fire",			IN_P1B1,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0400,"Thrust",			IN_P1B2,       IN_ACTIVE_LOW)
 GKEY_SET(1,0x0080,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x0080,"Coin 1",			IN_COIN1,      IN_ACTIVE_LOW)
GKEY_END

GKEY_START(speedfrk_keys)
GKEY_SET(0,0xffef,"Default",0,			IN_DEFAULT)
 GKEY_SET(0,0x0080,"Start",				IN_START1,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0100,"Gas",				IN_P1B1,       IN_ACTIVE_LOW)
 GKEY_SET(1,0xffff,"Default",0,			IN_DEFAULT)
 GKEY_SET(1,0x0080,"Coin",				IN_COIN1,      IN_ACTIVE_LOW)
 GKEY_SET(2,0x01,"1st Gear",			IN_P2UP,		IN_ACTIVE_LOW)
 GKEY_SET(2,0x02,"2nd Gear",			IN_P2RIGHT,		IN_ACTIVE_LOW)
 GKEY_SET(2,0x04,"3rd Gear",			IN_P2LEFT,		IN_ACTIVE_LOW)
 GKEY_SET(2,0x08,"4th Gear",			IN_P2DOWN,		IN_ACTIVE_LOW)
 GKEY_SET(3,0x01,"Rotate Left",			IN_P1LEFT,				IN_ACTIVE_HIGH)
 GKEY_ANALOG(4,25,"Track Analog X",	MOUSE_X, 0, 0,   IN_ANALOG ,10, 0,0xff )//
 GKEY_SET(5,0x01,"Rotate Right",		IN_P1RIGHT,				IN_ACTIVE_HIGH)
GKEY_END

GKEY_START(wotw_keys)
 GKEY_SET(0,0xffff,"Default",0,		IN_DEFAULT)
 GKEY_SET(0,0x0001,"Player 1 Start", IN_START1,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0004,"Player 2 Start", IN_START2,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0040,"Move Left",		IN_P1LEFT,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0100,"Move Right",		IN_P1RIGHT,    IN_ACTIVE_LOW)
 GKEY_SET(0,0x1000,"Fire",			IN_P1B1,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0400,"Thrust",			IN_P1B2,       IN_ACTIVE_LOW)
 GKEY_SET(1,0x00c0,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x40,"Service",           IN_TEST,    INF_TOGGLE |  IN_ACTIVE_LOW)
 GKEY_SET(1,0x80,"Coin 1",     IN_COIN1,      IN_ACTIVE_LOW)
GKEY_END

GKEY_START(boxingb_keys)
GKEY_SET(0,0x0fff,"Default",0,		IN_DEFAULT)
 GKEY_SET(0,0x0008,"Player 1 Start", IN_START1,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0004,"Player 2 Start", IN_START2,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0020,"Shoot",			MOUSE_B1 |IN_P1B1,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0010,"Box",			MOUSE_B2 |IN_P1B2,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0008,"Panic",			IN_P1B3,       IN_ACTIVE_LOW)
 GKEY_SET(1,0x00d3,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x40,"Service",           IN_TEST,    INF_TOGGLE |  IN_ACTIVE_LOW)
 GKEY_SET(1,0x80,"Coin 1",     IN_COIN1,      IN_ACTIVE_LOW)
 GKEY_SET(2,0x01,"Rotate Left",			IN_P1LEFT,				IN_ACTIVE_HIGH)
 GKEY_ANALOG(3,100,"Track Analog X",	MOUSE_X, 0, 0,  INF_REVERSE | IN_ANALOG ,5, 0,0xff )//
 GKEY_SET(4,0x01,"Rotate Right",		IN_P1RIGHT,				IN_ACTIVE_HIGH)
GKEY_END

GKEY_START(armora_keys)
 GKEY_SET(0,0xffff,"Default",0,		IN_DEFAULT)
 GKEY_SET(0,0x0002,"Player 1 Start",IN_START1,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0008,"Player 2 Start",IN_START2,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x1000,"P1 Left",	   IN_P1LEFT,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x4000,"P1 Right",	   IN_P1RIGHT,    IN_ACTIVE_LOW)
 GKEY_SET(0,0x8000,"P1 Shoot",	   IN_P1B1,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x2000,"P1 Drive",	   IN_P1B2,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0001,"P2 Left",	   IN_P2LEFT,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0004,"P2 Right",	   IN_P2RIGHT,    IN_ACTIVE_LOW)
 GKEY_SET(0,0x0010,"P2 Shoot",	   IN_P2B1,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0020,"P2 Drive",	   IN_P2B2,       IN_ACTIVE_LOW)
 GKEY_SET(1,0x00ac,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x40,"Service",           IN_TEST,    INF_TOGGLE |  IN_ACTIVE_HIGH)
 GKEY_SET(1,0x80,"Coin 1",     IN_COIN1,      IN_ACTIVE_LOW)
GKEY_END



GKEY_START(ripoff_keys)
 GKEY_SET(0,0xffff,"Default",0,		IN_DEFAULT)
 GKEY_SET(0,0x0002,"Player 1 Start",IN_START1,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0008,"Player 2 Start",IN_START2,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x1000,"P1 Left",	   IN_P1LEFT,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x4000,"P1 Right",	   IN_P1RIGHT,    IN_ACTIVE_LOW)
 GKEY_SET(0,0x2000,"P1 Shoot",	   IN_P1B1,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x8000,"P1 Drive",	   IN_P1B2,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0001,"P2 Left",	   IN_P2LEFT,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0004,"P2 Right",	   IN_P2RIGHT,    IN_ACTIVE_LOW)
 GKEY_SET(0,0x0020,"P2 Shoot",	   IN_P2B1,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0010,"P2 Drive",	   IN_P2B2,       IN_ACTIVE_LOW)
 GKEY_SET(1,0x00ff,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x40,"Service",           IN_TEST,    INF_TOGGLE |  IN_ACTIVE_LOW)
 GKEY_SET(1,0x80,"Coin 1",     IN_COIN1,      IN_ACTIVE_LOW)
GKEY_END

 
GKEY_START(sundance_keys)
GKEY_SET(0,0xffff,"Default",0,		 IN_DEFAULT)
 GKEY_SET(0,0x0004,"1 Player Start", IN_START1,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0008,"2 Player Start", IN_START2,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0020,"Grid",		     IN_P1B1,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0800,"2 Suns",		 IN_P1B2,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0010,"3 Suns",		 IN_P1B3,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0040,"4 Suns",		 IN_P1B4,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0002,"P1 Fire",	     IN_PAD_0,      IN_ACTIVE_LOW)
 GKEY_SET(0,0x0080,"P2 Fire",	     IN_P2B6,       IN_ACTIVE_LOW)

 GKEY_SET(1,0x0083,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x80,"Coin 1",     IN_COIN1,      IN_ACTIVE_LOW)

 GKEY_SET(0,0x4001,"P1 Pad 1",	   IN_PAD_1,      IN_ACTIVE_LOW)
 GKEY_SET(0,0x1200,"P1 Pad 2",	   IN_PAD_2,      IN_ACTIVE_LOW)
 GKEY_SET(0,0x0201,"P1 Pad 3",	   IN_PAD_3,      IN_ACTIVE_LOW)
 GKEY_SET(0,0x4000,"P1 Pad 4",	   IN_PAD_4,      IN_ACTIVE_LOW)
 GKEY_SET(0,0x1001,"P1 Pad 5",	   IN_PAD_5,      IN_ACTIVE_LOW)
 GKEY_SET(0,0x0200,"P1 Pad 6",	   IN_PAD_6,      IN_ACTIVE_LOW)
 GKEY_SET(0,0x1201,"P1 Pad 7",	   IN_PAD_7,      IN_ACTIVE_LOW)
 GKEY_SET(0,0x1000,"P1 Pad 8",	   IN_PAD_8,      IN_ACTIVE_LOW)
 GKEY_SET(0,0x0001,"P1 Pad 9",	   IN_PAD_9,      IN_ACTIVE_LOW)

 
 GKEY_SET(3,0,"P2 Pad 1",	   IN_P2RIGHT,    IN_ACTIVE_LOW)
 GKEY_SET(3,0,"P2 Pad 2",	   IN_P2LEFT,     IN_ACTIVE_LOW)
 GKEY_SET(3,0,"P2 Pad 3",	   IN_P2UP,       IN_ACTIVE_LOW)
 GKEY_SET(3,0,"P2 Pad 4",	   IN_P2DOWN,     IN_ACTIVE_LOW)
 GKEY_SET(3,0,"P2 Pad 5",	   IN_P2B1,       IN_ACTIVE_LOW)
 GKEY_SET(3,0,"P2 Pad 6",	   IN_P2B2,       IN_ACTIVE_LOW)
 GKEY_SET(3,0,"P2 Pad 7",	   IN_P2B3,       IN_ACTIVE_LOW)
 GKEY_SET(3,0,"P2 Pad 8",	   IN_P2B4,       IN_ACTIVE_LOW)
 GKEY_SET(3,0,"P2 Pad 9",	   IN_P2B5,       IN_ACTIVE_LOW)

GKEY_END

GKEY_START(spacewar_keys)
GKEY_SET(0,0xffff,"Default",0,		IN_DEFAULT)
 GKEY_SET(0,0,"Coin",     IN_COIN1,      IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Left",  IN_P1LEFT,     IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Right", IN_P1RIGHT,    IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Fire",  IN_P1B1,       IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Thrust",IN_P1B2,       IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Hyper", IN_P1B3,       IN_ACTIVE_HIGH)

 GKEY_SET(0,0,"P2 Left",  IN_P2LEFT,     IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P2 Right", IN_P2RIGHT,    IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P2 Fire",  IN_P2B1,       IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P2 Thrust",IN_P2B2,       IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P2 Hyper", IN_P2B3,       IN_ACTIVE_HIGH)

 GKEY_SET(0,0,"P1 Pad 1", IN_PAD_1,      IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Pad 2", IN_PAD_2,      IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Pad 3", IN_PAD_3,      IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Pad 4", IN_PAD_4,      IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Pad 5", IN_PAD_5,      IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Pad 6", IN_PAD_6,      IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Pad 7", IN_PAD_7,      IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Pad 8", IN_PAD_8,      IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Pad 9", IN_PAD_9,      IN_ACTIVE_HIGH)
  GKEY_SET(1,0xffff,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x80,"Coin 1",     IN_COIN1,      IN_ACTIVE_LOW)
GKEY_END

	
GKEY_START(warrior_keys)
 GKEY_SET(0,0xffff,"Default",0,		IN_DEFAULT)
 GKEY_SET(0,0x100,"P1 Right",IN_P1RIGHT,    IN_ACTIVE_LOW)
 GKEY_SET(0,0x200,"P1 Left", IN_P1LEFT,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x400,"P1 Up",   IN_P1UP,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x800,"P1 Down", IN_P1DOWN,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x1000,"P1 Swing",IN_P1B1,      IN_ACTIVE_LOW)
 GKEY_SET(0,0x01,"P2 Right",IN_P2RIGHT,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x02,"P2 Left", IN_P2LEFT,      IN_ACTIVE_LOW)
 GKEY_SET(0,0x04,"P2 Up",   IN_P2UP,        IN_ACTIVE_LOW)
 GKEY_SET(0,0x08,"P2 Down", IN_P2DOWN,      IN_ACTIVE_LOW)
 GKEY_SET(0,0x10,"P2 Swing",IN_P2B1,        IN_ACTIVE_LOW)
 GKEY_SET(1,0x00ff,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x80,"Coin 1",  IN_COIN1,       IN_ACTIVE_LOW)
GKEY_END

GKEY_START(starhawk_keys)
 GKEY_SET(0,0xffff,"Default",0,		IN_DEFAULT)
 GKEY_SET(0,0x0002,"P1 Move Left",  IN_P1LEFT,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0004,"P1 Move Right", IN_P1RIGHT,    IN_ACTIVE_LOW)
 GKEY_SET(0,0x0001,"P1 Move Up",    IN_P1UP,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0008,"P1 Move Down",  IN_P1DOWN,     IN_ACTIVE_LOW)
 
 GKEY_SET(0,0x4000,"P1 Slow",       IN_P1B2,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x1000,"P1 Medium",     IN_P1B3,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0200,"P1 Fast",       IN_P1B4,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0400,"P2 Move Left",  IN_P2LEFT,     IN_ACTIVE_LOW)
 GKEY_SET(0,0x0010,"P2 Move Right", IN_P2RIGHT,    IN_ACTIVE_LOW)
 GKEY_SET(0,0x0800,"P2 Move Up",    IN_P2UP,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x0020,"P2 Move Down",  IN_P2DOWN,     IN_ACTIVE_LOW)

 GKEY_SET(0,0x0100,"P2 Slow",       IN_P2B2,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x2000,"P2 Medium",     IN_P2B3,       IN_ACTIVE_LOW)
 GKEY_SET(0,0x8000,"P2 Fast",       IN_P2B4,       IN_ACTIVE_LOW)
 GKEY_SET(1,0xffff,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x04,"Player 1 Start",IN_START1,  IN_ACTIVE_LOW)
 GKEY_SET(1,0x08,"P1 Fire",       IN_P1B1,       IN_ACTIVE_LOW)
 GKEY_SET(1,0x10,"Player 2 Start",IN_START2,  IN_ACTIVE_LOW)
 GKEY_SET(1,0x20,"P2 Fire",       IN_P2B1,       IN_ACTIVE_LOW)
 GKEY_SET(1,0x80,"Coin 1",     IN_COIN1,      IN_ACTIVE_LOW)

GKEY_END



GKEY_START(barrier_keys)
GKEY_SET(0,0xffff,"Default",0,		IN_DEFAULT)
 GKEY_SET(0,0,"Player 1 Start",IN_START1,     IN_ACTIVE_LOW)
 GKEY_SET(0,0,"Player 2 Start",IN_START2,     IN_ACTIVE_LOW)
 GKEY_SET(0,0,"P1 Move Left",  IN_P1LEFT,     IN_ACTIVE_LOW)
 GKEY_SET(0,0,"P1 Move Right", IN_P1RIGHT,    IN_ACTIVE_LOW)
 GKEY_SET(0,0,"P1 Move Up",    IN_P1UP,       IN_ACTIVE_LOW)
 GKEY_SET(0,0,"P1 Move Down",  IN_P1DOWN,     IN_ACTIVE_LOW)
 GKEY_SET(0,0,"Skill A",       IN_P1B1,       IN_ACTIVE_LOW)
 GKEY_SET(0,0,"Skill B",       IN_P1B2,       IN_ACTIVE_LOW)
 GKEY_SET(0,0,"Skill C",       IN_P1B3	,     IN_ACTIVE_LOW)
 GKEY_SET(0,0,"P2 Move Left",  IN_P2LEFT,     IN_ACTIVE_LOW)
 GKEY_SET(0,0,"P2 MoveRight",  IN_P2RIGHT,    IN_ACTIVE_LOW)
 GKEY_SET(0,0,"P2 Move Up",    IN_P2UP,       IN_ACTIVE_LOW)
 GKEY_SET(0,0,"P2 Move Down",  IN_P2DOWN,     IN_ACTIVE_LOW)
 GKEY_SET(1,0xffff,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x80,"Coin 1",        IN_COIN1,   IN_ACTIVE_LOW)
GKEY_END

GKEY_START(demon_keys)
 GKEY_SET(0,0,"Player 1 Start",IN_START1,     IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"Player 2 Start",IN_START2,     IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"Coin 1",        IN_COIN1,      IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Move Left",  IN_P1LEFT,     IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Move Right", IN_P1RIGHT,    IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Fire",       IN_P1B1,       IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Slow",       IN_P1B2,       IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Medium",     IN_P1B3,       IN_ACTIVE_HIGH)
 
 GKEY_SET(0,0,"P2 Move Left",IN_P2LEFT,       IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P2 Move Right",IN_P2RIGHT,     IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P2 Fire",IN_P2B1,              IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P2 Slow",IN_P2B2,			  IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P2 Medium",IN_P2B3,			  IN_ACTIVE_HIGH)
GKEY_END


GKEY_START(alphaone_keys)
 GKEY_SET(0,0,"Coin 1",       IN_COIN1,      IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"Coin 2",       IN_COIN2,      IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"Coin 3",       IN_COIN2,      IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"Roll Left",    IN_P1LEFT,     IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"Roll Right",   IN_P1RIGHT,    IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Start/Fire",IN_P1B1,       IN_ACTIVE_HIGH)
 GKEY_SET(0,0,"P1 Shield",    IN_P1B2,       IN_ACTIVE_HIGH)
GKEY_END


GKEY_START(quantum_keys)
 GKEY_SET(0,0xfe,"Default",         0,IN_DEFAULT)
 GKEY_SET(0,0x80,"Test",            IN_TEST,    INF_TOGGLE | IN_ACTIVE_LOW )
 GKEY_SET(0,0x04,"Player 1 Start",  IN_START1,  IN_ACTIVE_LOW)
 GKEY_SET(0,0x08,"Player 2 Start",  IN_START2,  IN_ACTIVE_LOW)
 GKEY_SET(0,0x20,"Coin 1",          IN_COIN1,   IN_ACTIVE_LOW)
 GKEY_SET(0,0x10,"Coin 2",          IN_COIN2,   IN_ACTIVE_LOW)
 GKEY_SET(1,0x01,"Track Left",      IN_P1LEFT,  IN_ACTIVE_HIGH)
 GKEY_ANALOG(2,10,"Track Analog X", MOUSE_X,0, 0, IN_ANALOG ,14 , 0, 0x0f )
 GKEY_SET(3,0x01,"Track Right",     IN_P1RIGHT, IN_ACTIVE_HIGH)	
 GKEY_SET(4,0x01,"Track Up",        IN_P1UP,    IN_ACTIVE_HIGH)
 GKEY_ANALOG(5,10,"Track Analog Y", MOUSE_Y, 0, 0, INF_REVERSE | IN_ANALOG, 14 , 0,0x0f )
 GKEY_SET(6,0x01,"Track Down",      IN_P1DOWN,  IN_ACTIVE_HIGH)
GKEY_END

GKEY_START(bradley_keys)
 GKEY_SET(0,0x3f,"Default",0,			IN_DEFAULT)
 GKEY_SET(0,0x10,"Test",				IN_TEST,          INF_TOGGLE | IN_ACTIVE_LOW)
 GKEY_SET(0,0x20,"Test Advance",		IN_TEST_ADV,	  IN_ACTIVE_LOW)
 GKEY_SET(0,0x80,"Reset",				IN_RESET,		  INF_RESETCPU )
 GKEY_SET(0,0x01,"Coin 1",			    IN_COIN1,		  IN_ACTIVE_LOW)
 GKEY_SET(0,0x02,"Coin 2",			    IN_COIN2,		  IN_ACTIVE_LOW)
 
 GKEY_SET(1,0x80,"Default",0,			IN_DEFAULT)
 GKEY_SET(1,0x20,"Player 1 Start",		IN_START1,		  IN_ACTIVE_HIGH)
 GKEY_SET(1,0x40,"Player 2 Start",		IN_START2,		  IN_ACTIVE_HIGH)
 GKEY_SET(1,0x10,"Fire",				IN_P1B1,		  IN_ACTIVE_HIGH)
 GKEY_SET(1,0x02,"Right/Up",			IN_RIGHT_P2UP,	  IN_ACTIVE_HIGH)
 GKEY_SET(1,0x01,"Right/Down",			IN_RIGHT_P2DOWN,  IN_ACTIVE_HIGH)
 GKEY_SET(1,0x08,"Left/Up",				IN_LEFT_P1UP,	  IN_ACTIVE_HIGH)
 GKEY_SET(1,0x04,"Left/Down",			IN_LEFT_P1DOWN,	  IN_ACTIVE_HIGH)
 GKEY_SET(2,0x01,"Single Stick Up",		IN_P1UP,		  IN_ACTIVE_HIGH)
 GKEY_SET(2,0x02,"Single Stick Down",	IN_P1DOWN,		  IN_ACTIVE_HIGH)
 GKEY_SET(2,0x04,"Single Stick Left",	IN_P1LEFT,		  IN_ACTIVE_HIGH)
 GKEY_SET(2,0x08,"Single Stick Right",	IN_P1RIGHT,		  IN_ACTIVE_HIGH)
 
 GKEY_SET(4,0xff,"Default",0,			IN_DEFAULT)
 GKEY_SET(4,0x04, "Armor Piercing, Single Shot",KEY_0_PAD,IN_ACTIVE_LOW)
 GKEY_SET(4,0x08,"High Explosive, Single Shot", KEY_1_PAD, IN_ACTIVE_LOW)
 GKEY_SET(4,0x10,"Armor Piercing, Low Rate",    KEY_2_PAD,IN_ACTIVE_LOW)
 GKEY_SET(4,0x20,"High Explosive, Low Rate",    KEY_3_PAD, IN_ACTIVE_LOW)
 GKEY_SET(4,0x40,"Armor Piercing, High Rate",   KEY_4_PAD, IN_ACTIVE_LOW)
 GKEY_SET(4,0x80,"High Explosive, High Rate",   KEY_5_PAD, IN_ACTIVE_LOW)

 GKEY_SET(5,0x1c,"Default",0,			IN_DEFAULT)
 GKEY_SET(5,0x04,"Select TOW Missiles", IN_P1B4, IN_ACTIVE_LOW)
 GKEY_SET(5,0x08,"7.62 mm Machine Gun", IN_P1B5, IN_ACTIVE_LOW)
 GKEY_SET(5,0x10,"Magnification Toggle",IN_P1B6, INF_TOGGLE | IN_ACTIVE_LOW)
 GKEY_ANALOG(6,25,"Turret X ",IN_AD_STICK_X, 0x48, 0xc8,   IN_ANALOG ,10,0x88,0xff) 
 GKEY_ANALOG(7,25,"Turret Y ",IN_AD_STICK_Y, 0x46, 0xc6,   IN_ANALOG ,10,0x86,0xff) 
 GKEY_ANALOG(8,25,"Shell Range ",IN_AD_STICK_Y, 0x10, 0xf0,   IN_ANALOG ,10,0x80,0xff) 

GKEY_END

GKEY_START(spacduel_keys)
 GKEY_SET(0,0x7f,"Default",0,		IN_DEFAULT)
 GKEY_SET(0,0x02,"Coin 1",			IN_COIN1,		IN_ACTIVE_LOW)
 GKEY_SET(0,0x01,"Coin 2",			IN_COIN2,		IN_ACTIVE_LOW)
 GKEY_SET(0,0x10,"Test",			IN_TEST,		INF_TOGGLE | IN_ACTIVE_LOW)
 GKEY_SET(1,0x00,"Default",0,		IN_DEFAULT)
 GKEY_SET(1,0x01,"P1 Rotate Left",	IN_P1LEFT,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x02,"P1 Rotate Right",	IN_P1RIGHT,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x04,"P1 Fire",			IN_P1B1,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x08,"P1 Shield",		IN_P1B3,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x10,"P1 Thrust",		IN_P1B2,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x20,"Start",			IN_START1,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x40,"Select",			IN_START2,		IN_ACTIVE_HIGH)
 GKEY_SET(2,0x00,"Default",0,		IN_DEFAULT)
 GKEY_SET(2,0x01,"P2 Rotate Left",	IN_P2LEFT,		IN_ACTIVE_HIGH)
 GKEY_SET(2,0x02,"P2 Rotate Right",	IN_P2RIGHT,		IN_ACTIVE_HIGH)
 GKEY_SET(2,0x04,"P2 Fire",			IN_P2B1,		IN_ACTIVE_HIGH)
 GKEY_SET(2,0x08,"P2 Shield",		IN_P2B3,		IN_ACTIVE_HIGH)
 GKEY_SET(2,0x10,"P2 Thrust",		IN_P2B2,		IN_ACTIVE_HIGH)
GKEY_END


GKEY_START(galaga_keys)
 GKEY_SET(0,0x08,"Hyperspace",		IN_P1B3,		IN_ACTIVE_HIGH)
 GKEY_SET(0,0x10,"Fire",			IN_P1B1,		IN_ACTIVE_HIGH)
 GKEY_SET(0,0x80,"Test",			IN_TEST,		INF_TOGGLE | IN_ACTIVE_HIGH )
 GKEY_SET(0,0x80,"Reset",			IN_RESET,		INF_RESETCPU)
 
 GKEY_SET(1,0x01,"Coin 1",			IN_COIN1,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x02,"Coin 2",		    IN_COIN2,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x08,"Player 1 Start",  IN_START1,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x10,"Player 2 Start",  IN_START2,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x20,"Thrust",			IN_P1B2,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x40,"Rotate Right",	IN_P1RIGHT,		IN_ACTIVE_HIGH)
 GKEY_SET(1,0x80,"Rotate Left",		IN_P1LEFT,		IN_ACTIVE_HIGH)
GKEY_END

#endif