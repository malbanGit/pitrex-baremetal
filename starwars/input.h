
#ifndef INPUT_H
#define INPUT_H

#include "osd_cpu.h"

//////////////////////////////////////////////////////////////////////////
//DEFINES FOR INPUT SETTINGS

#define INT_UNUSED     IPF_UNUSED
#define INT_SPECIAL    IPT_UNUSED	/* special meaning handled by custom functions */

#define INF_MASK       0xffffff00
#define INF_UNUSED     0x80000000	/* The bit is not used by this game, but is used */
									/* by other games running on the same hardware. */
									/* This is different from IPT_UNUSED, which marks */
									/* bits not connected to anything. */
#define INF_COCKTAIL   IPF_PLAYER2	/* the bit is used in cocktail mode only */

#define INF_CHEAT      0x40000000	/* Indicates that the input bit is a "cheat" key */

									/* (providing invulnerabilty, level advance, and */
									/* so on). MAME will not recognize it when the */
									/* -nocheat command line option is specified. */

#define INF_PLAYERMASK 0x00030000	/* use IPF_PLAYERn if more than one person can */
#define INF_PLAYER1    0         	/* play at the same time. The IPT_ should be the same */
#define INF_PLAYER2    0x00010000	/* for all players (e.g. IPT_BUTTON1 | IPF_PLAYER2) */
#define INF_PLAYER3    0x00020000	/* IPF_PLAYER1 is the default and can be left out to */
#define INF_PLAYER4    0x00030000	/* increase readability. */

#define INF_8WAY       0         	/* Joystick modes of operation. 8WAY is the default, */
#define INF_4WAY       0x00080000	/* it prevents left/right or up/down to be pressed at */
#define INF_2WAY       0         	/* the same time. 4WAY prevents diagonal directions. */
									/* 2WAY should be used for joysticks wich move only */
                                 	/* on one axis (e.g. Battle Zone) */

#define INF_IMPULSE    0x00100000	/* When this is set, when the key corrisponding to */
									/* the input bit is pressed it will be reported as */
									/* pressed for a certain number of video frames and */
									/* then released, regardless of the real status of */
									/* the key. This is useful e.g. for some coin inputs. */
									/* The number of frames the signal should stay active */
									/* is specified in the "arg" field. */
#define INF_TOGGLE     0x00200000	/* When this is set, the key acts as a toggle - press */
									/* it once and it goes on, press it again and it goes off. */
									/* useful e.g. for sone Test Mode dip switches. */

#define INF_COUNTER    0x04000000	/* Countdown Timer */

#define INF_REVERSE    0x00400000	/* By default, analog inputs like IPT_TRACKBALL increase */
									/* when going right/up. This flag inverts them. */

#define INF_CENTER     0x00800000	/* always preload in->default, autocentering the STICK/TRACKBALL */

#define INF_PORTRESET     0x00040000

#define INF_CUSTOM_UPDATE 0x01000000 /* normally, analog ports are updated when they are accessed. */
									/* When this flag is set, they are never updated automatically, */
									/* it is the responsibility of the driver to call */
									/* update_analog_port(int port). */

#define INF_RESETCPU   0x02000000	/* when the key is pressed, reset the first CPU */


/* The "arg" field contains 4 bytes fields */
#define INF_SENSITIVITY(percent)	((percent & 0xff) << 8)
#define INF_DELTA(val)				((val & 0xff) << 16)

#define IN_GET_IMPULSE(port) (((port)->type >> 8) & 0xff)
#define IN_GET_SENSITIVITY(port) ((((port)+1)->type >> 8) & 0xff)
#define IN_SET_SENSITIVITY(port,val) ((port)+1)->type = ((port+1)->type & 0xffff00ff)|((val&0xff)<<8)
#define IN_GET_DELTA(port) ((((port)+1)->type >> 16) & 0xff)
#define IN_SET_DELTA(port,val) ((port)+1)->type = ((port+1)->type & 0xff00ffff)|((val&0xff)<<16)
#define IN_GET_MIN(port) ((port>mask)
#define IN_GET_MAX(port) ((port)->default_value)

#define IP_NAME_DEFAULT ((const char *)-1)
#define GKEY_IMP(Port,Mask,Heading,Default,Type,Arg1)  {Port,Mask,Heading,Default,0,0,0,Type,Arg1,-1, -1},
#define GKEY_SET(Port,Mask,Heading,Default,Type)  {Port,Mask,Heading,Default,0,0,0,Type,-1,-1, -1},
#define GKEY_ANALOG(Port,Mask,Heading,Default,Current,Joystick,Type,Arg1,Arg2,Amask)  {Port,Mask,Heading,Default,Current,Joystick,0,Type,Arg1,Arg2,Amask},
/*#define GKEY_ANALOG(Port,Mask,Heading,Default,Type,sensitivity,Min,Max) \
		{Port, Min, max,  INP_SENSITIVITY(sensitivity) | INF_DELTA(delta), IP_NAME_DEFAULT },
*/
/* Wrapper for compatibility */

#define MOUSE_B1         0x01000000
#define MOUSE_B2         0x02000000
#define MOUSE_B3         0x03000000
#define MOUSE_B4         0x04000000
#define MOUSE_X          0x050000
#define MOUSE_Y          0x060000

#define IN_KEY_DEFAULT CODE_DEFAULT
#define IN_JOY_DEFAULT CODE_DEFAULT
#define IN_KEY_PREVIOUS CODE_PREVIOUS
#define IN_JOY_PREVIOUS CODE_PREVIOUS
#define IN_KEY_NONE CODE_NONE
#define IN_JOY_NONE CODE_NONE

#define PORT0 0
#define PORT1 1
#define PORT2 2
#define PORT3 3
#define PORT4 4
#define PORT5 5
#define PORT6 6
#define PORT7 7
#define PORT8 8
#define PORT9 9
#define PORT10 10
#define PORT11 11
#define PORT12 12
#define PORT13 13
#define PORT14 14

#define IN_NORMAL       0
#define IN_IMPULSE      1
#define IN_REPEAT       2
#define IN_SHFT_REPEAT  3
#define IN_ACTIVE_HIGH 0x00
#define IN_ACTIVE_LOW  0xff
#define IN_DEFAULT     0x80 
#define IN_ANALOG      0xa0
#define IN_NONE         0
#define IN_AD_STICK_X 0x28
#define IN_AD_STICK_Y 0x38


//DEFINES FOR KEYS
#define J0S0_DigiLeft	   0x0100
#define J0S0_DigiRight     0x0200
#define J0S0_DigiUp	       0x0300
#define J0S0_DigiDown	   0x0400
#define J0S0_PlusX		   0x0500
#define J0S0_MinusX		   0x0600
#define J0S0_PlusY		   0x0700
#define J0S0_MinusY		   0x0800
#define J0S1_DigiLeft	0x0900
#define J0S1_DigiRight	0x0a00
#define J0S1_DigiUp		0x0b00
#define J0S1_DigiDown	0x0c00
#define J0S1_PlusX		0x0d00
#define J0S1_MinusX		0x0e00
#define J0S1_PlusY		0x0f00
#define J0S1_MinusY		0x1000
#define J0S2_DigiLeft	0x1100
#define J0S2_DigiRight	0x1200
#define J0S2_DigiUp		0x1300
#define J0S2_DigiDown	0x1400
#define J0S2_PlusX		0x1500
#define J0S2_MinusX		0x1600
#define J0S2_PlusY		0x1700
#define J0S2_MinusY		0x1800
#define J0S3_DigiLeft	0x1900
#define J0S3_DigiRight	0x1a00
#define J0S3_DigiUp		0x1b00
#define J0S3_DigiDown	0x1c00
#define J0S3_PlusX		0x1d00
#define J0S3_MinusX		0x1e00
#define J0S3_PlusY		0x1f00
#define J0S3_MinusY		0x2000
#define J0B0			0x2100
#define J0B1			0x2200
#define J0B2			0x2300
#define J0B3			0x2400
#define J0B4			0x2500
#define J0B5			0x2600
#define J0B6			0x2700
#define J0B7			0x2800
#define J0B8			0x2900
#define J0B9			0x2a00
#define J0B10			0x2b00
#define J0B11			0x2c00

#define J1S0_DigiLeft   0x2d00
#define J1S0_DigiRight	0x2e00
#define J1S0_DigiUp		0x2f00
#define J1S0_DigiDown	0x3000
#define J1S0_PlusX		0x3100
#define J1S0_MinusX		0x3200
#define J1S0_PlusY		0x3300
#define J1S0_MinusY		0x3400
#define J1S1_DigiLeft	0x3500
#define J1S1_DigiRight	0x3600
#define J1S1_DigiUp		0x3700
#define J1S1_DigiDown	0x3800
#define J1S1_PlusX		0x3900
#define J1S1_MinusX		0x3a00
#define J1S1_PlusY		0x3b00
#define J1S1_MinusY		0x3c00
#define J1S2_DigiLeft	0x3d00
#define J1S2_DigiRight	0x3e00
#define J1S2_DigiUp		0x3f00
#define J1S2_DigiDown	0x4000
#define J1S2_PlusX		0x4100
#define J1S2_MinusX		0x4200
#define J1S2_PlusY		0x4300
#define J1S2_MinusY		0x4400
#define J1S3_DigiLeft	0x4500
#define J1S3_DigiRight	0x4600
#define J1S3_DigiUp		0x4700
#define J1S3_DigiDown	0x4800
#define J1S3_PlusX		0x4900
#define J1S3_MinusX		0x4a00
#define J1S3_PlusY		0x4b00
#define J1S3_MinusY		0x4c00
#define J1B0			0x4d00
#define J1B1			0x4e00
#define J1B2			0x4f00
#define J1B3			0x5000
#define J1B4			0x5100
#define J1B5			0x5200
#define J1B6			0x5300
#define J1B7			0x5400
#define J1B8			0x5500
#define J1B9			0x5600
#define J1B10			0x5700
#define J1B11			0x5800

#define J2S0_DigiLeft	0x5900
#define J2S0_DigiRight	0x5a00
#define J2S0_DigiUp		0x5b00
#define J2S0_DigiDown	0x5c00
#define J2S0_PlusX		0x6300
#define J2S0_MinusX		0x6400
#define J2S0_PlusY		0x6500
#define J2S0_MinusY		0x9000
#define J2S1_DigiLeft	0x9100
#define J2S1_DigiRight	0x9200
#define J2S1_DigiUp		0x9300
#define J2S1_DigiDown	0x9400
#define J2S1_PlusX		0x9500
#define J2S1_MinusX		0x9600
#define J2S1_PlusY		0x9700
#define J2S1_MinusY		0x9800
#define J2S2_DigiLeft	0x9900
#define J2S2_DigiRight	0x9a00
#define J2S2_DigiUp		0x9b00
#define J2S2_DigiDown	0x9c00
#define J2S2_PlusX		0x9d00
#define J2S2_MinusX		0x9e00
#define J2S2_PlusY		0x9f00
#define J2S2_MinusY		0xa000
#define J2S3_DigiLeft	0xa100
#define J2S3_DigiRight	0xa200
#define J2S3_DigiUp		0xa300
#define J2S3_DigiDown	0xa400
#define J2S3_PlusX		0xa500
#define J2S3_MinusX		0xa600
#define J2S3_PlusY		0xa700
#define J2S3_MinusY		0xa800
#define J2Btn1			0xa900
#define J2Btn2			0xaa00
#define J2Btn3			0xab00
#define J2Btn4			0xac00
#define J2Btn5			0xad00
#define J2Btn6			0xae00
#define J2Btn7			0xaf00
#define J2Btn8			0xb000
#define J2Btn9			0xb100
#define J2Btn10			0xb200
#define J2Btn11			0xb300
#define J2Btn12			0xb400

#define J3S0_DigiLeft	0xb500
#define J3S0_DigiRight	0xb600
#define J3S0_DigiUp		0xb700
#define J3S0_DigiDown	0xb800
#define J3S0_PlusX		0xb900
#define J3S0_MinusX		0xba00
#define J3S0_PlusY		0xbb00
#define J3S0_MinusY		0xbc00
#define J3S1_DigiLeft	0xbd00
#define J3S1_DigiRight	0xbe00
#define J3S1_DigiUp		0xbf00
#define J3S1_DigiDown	0xc000
#define J3S1_PlusX		0xc100
#define J3S1_MinusX		0xc200
#define J3S1_PlusY		0xc300
#define J3S1_MinusY		0xc400
#define J3S2_DigiLeft	0xc500
#define J3S2_DigiRight	0xc600
#define J3S2_DigiUp		0xc700
#define J3S2_DigiDown	0xc800
#define J3S2_PlusX		0xc900
#define J3S2_MinusX		0xca00
#define J3S2_PlusY		0xcb00
#define J3S2_MinusY		0xcc00
#define J3S3_DigiLeft	0xcd00
#define J3S3_DigiRight	0xce00
#define J3S3_DigiUp		0xcf00
#define J3S3_DigiDown	0xd000
#define J3S3_PlusX		0xd100
#define J3S3_MinusX		0xd200
#define J3S3_PlusY		0xd300
#define J3S3_MinusY		0xd400
#define J3Btn1			0xd500
#define J3Btn2			0xd600
#define J3Btn3			0xd700
#define J3Btn4			0xd800
#define J3Btn5			0xd900
#define J3Btn6			0xda00
#define J3Btn7			0xdb00
#define J3Btn8			0xdc00
#define J3Btn9			0xdd00
#define J3Btn10			0xde00
#define J3Btn11			0xdf00
#define J3Btn12			0xe000


// dummy defines
#define KEY_UP 0
#define KEY_DOWN 0
#define KEY_RIGHT 0
#define KEY_LEFT 0
#define KEY_ENTER 0
#define KEY_ESC 0

#define KEY_LCONTROL 0
#define KEY_ALT 0
#define KEY_SPACE 0
#define KEY_RSHIFT 0
#define KEY_LSHIFT 0
#define KEY_TAB 0
#define KEY_F1 0
#define KEY_F2 0
#define KEY_F3 0
#define KEY_F4 0
#define KEY_F5 0
#define KEY_F6 0
#define KEY_F7 0
#define KEY_F8 0
#define KEY_F9 0
#define KEY_F10 0
#define KEY_F11 0
#define KEY_F12 0
#define KEY_A 0
#define KEY_B 0
#define KEY_C 0
#define KEY_D 0
#define KEY_E 0
#define KEY_F 0
#define KEY_G 0
#define KEY_H 0
#define KEY_I 0
#define KEY_J 0
#define KEY_K 0
#define KEY_L 0
#define KEY_M 0
#define KEY_N 0
#define KEY_O 0
#define KEY_P 0
#define KEY_Q 0
#define KEY_R 0
#define KEY_S 0
#define KEY_T 0
#define KEY_U 0
#define KEY_V 0
#define KEY_W 0
#define KEY_X 0
#define KEY_Y 0
#define KEY_Z 0
#define KEY_0_PAD 0
#define KEY_1_PAD 0
#define KEY_2_PAD 0
#define KEY_3_PAD 0
#define KEY_4_PAD 0
#define KEY_5_PAD 0
#define KEY_6_PAD 0
#define KEY_7_PAD 0
#define KEY_8_PAD 0
#define KEY_9_PAD 0

#define KEY_1 0
#define KEY_2 0
#define KEY_3 0
#define KEY_4 0
#define KEY_5 0
#define KEY_6 0
#define KEY_7 0
#define KEY_8 0
#define KEY_9 0
#define KEY_0 0



//KEY DEFINES, DEPENDANT ON JOYSTICK DEFINES ABOVE
#define IN_UIUP          KEY_UP
#define IN_UIDOWN        KEY_DOWN
#define IN_UIRIGHT       KEY_RIGHT
#define IN_UILEFT        KEY_LEFT
#define IN_UISELECT      KEY_ENTER
#define IN_UICANCEL      KEY_ESC

#define IN_START1		  J0B7 | KEY_1 // MALBAN this is defined as BUTTON 1 AND BUTTON 2
#define IN_START2		  J1B7 | KEY_2
#define IN_START3		  KEY_3
#define IN_START4		  KEY_4
#define IN_COIN1		  J0B8 | KEY_5 // MALBAN this is defined as BUTTON 3 AND BUTTON 3
#define IN_COIN2		  J1B8 | KEY_6
#define IN_P1RIGHT		  J0S0_DigiRight | KEY_RIGHT //RIGHT
#define IN_P1LEFT		  J0S0_DigiLeft | KEY_LEFT //LEFT
#define IN_P1UP			  J0S0_DigiUp | KEY_UP //UP
#define IN_P1DOWN		  J0S0_DigiDown | KEY_DOWN //DOWN
#define IN_P1B1			  J0B0 | KEY_LCONTROL //LCONTROL
#define IN_P1B2			  J0B1 | KEY_ALT //LALT
#define IN_P1B3			  J0B2 | KEY_SPACE //SPACE
#define IN_P1B4			  J0B3 | KEY_LSHIFT //LSHIFT
#define IN_P1B5			  J0B4 | KEY_Z //Z
#define IN_P1B6			  J0B5 | KEY_X //X
#define IN_P1B7			  0
#define IN_P1B8			  0
#define IN_P1B9			  0
#define IN_P1B10		  0

#define IN_TEST_ADV       KEY_F1
#define IN_TEST           KEY_F2
#define IN_RESET          KEY_F3
#define IN_SNAP           KEY_F10
#define IN_SHOW_FPS       KEY_F11
#define IN_PAUSE          KEY_P
#define IN_MENU           KEY_TAB
#define IN_QUIT           J0B6 | KEY_ESC

#define IN_P2RIGHT		  J1S0_DigiRight |KEY_G //G
#define IN_P2LEFT		  J1S0_DigiLeft |KEY_D //D
#define IN_P2UP		      J1S0_DigiUp |KEY_R //R
#define IN_P2DOWN		  J1S0_DigiDown |KEY_F //F
#define IN_P2B1			  J1B0 |KEY_A //A
#define IN_P2B2			  J1B1 |KEY_S //S
#define IN_P2B3			  J1B2 |KEY_Q //Q
#define IN_P2B4			  J1B3 |KEY_W //W
#define IN_P2B5			  J1B4 |KEY_L //L
#define IN_P2B6			  J1B5 |KEY_K //K
#define IN_P2B7			  0
#define IN_P2B8			  0
#define IN_P2B9			  0
#define IN_P2B10		  0

#define IN_PAD_0		  KEY_0_PAD
#define IN_PAD_1		  KEY_1_PAD
#define IN_PAD_2		  KEY_2_PAD
#define IN_PAD_3		  KEY_3_PAD
#define IN_PAD_4		  KEY_4_PAD
#define IN_PAD_5		  KEY_5_PAD
#define IN_PAD_6		  KEY_6_PAD
#define IN_PAD_7		  KEY_7_PAD
#define IN_PAD_8		  KEY_8_PAD
#define IN_PAD_9		  KEY_9_PAD
#define IN_LEFT_P1UP      KEY_E
#define IN_LEFT_P1RIGHT   KEY_F
#define IN_LEFT_P1LEFT    KEY_S
#define IN_LEFT_P1DOWN    KEY_D
#define IN_RIGHT_P2RIGHT  KEY_L
#define IN_RIGHT_P2LEFT   KEY_J
#define IN_RIGHT_P2DOWN   KEY_K
#define IN_RIGHT_P2UP     KEY_I
#define IN_TILT           KEY_T


typedef struct {

int now;
int s2;
int s3;
int timer;

} MY_Input; 
extern MY_Input the_input[256];		

extern UINT8 in_port_1_r(UINT32 address, struct MemoryReadByte *psMemRead);
extern UINT8 in_port_2_r(UINT32 address, struct MemoryReadByte *psMemRead);
extern UINT8 in_port_3_r(UINT32 address, struct MemoryReadByte *psMemRead);
extern UINT8 in_port_4_r(UINT32 address, struct MemoryReadByte *psMemRead);
extern UINT8 in_port_5_r(UINT32 address, struct MemoryReadByte *psMemRead);
extern UINT8 in_port_6_r(UINT32 address, struct MemoryReadByte *psMemRead);
extern UINT8 in_port_7_r(UINT32 address, struct MemoryReadByte *psMemRead);
extern UINT8 in_port_8_r(UINT32 address, struct MemoryReadByte *psMemRead);
extern UINT8 in_port_9_r(UINT32 address, struct MemoryReadByte *psMemRead);
extern UINT8 in_port_10_r(UINT32 address, struct MemoryReadByte *psMemRead);
extern UINT8 in_port_11_r(UINT32 address, struct MemoryReadByte *psMemRead);
extern UINT8 in_port_12_r(UINT32 address, struct MemoryReadByte *psMemRead);
extern UINT8 in_port_13_r(UINT32 address, struct MemoryReadByte *psMemRead);
extern UINT8 in_port_14_r(UINT32 address, struct MemoryReadByte *psMemRead);

int getport(int port);
void set_input_defaults();
void proc_input(int num);
void update_input();
int KeyCheck(int keynum);
void get_input(void);
void clear_input(void);
int check_gameskip(int direction);
void check_joystick(void);
//int Check_Input(int key);
int MenuCheck(int keynum);
void MenuInput(void);

void update_mouse(int *deltax ,int *deltay);
int do_read_key(void);
int do_key_pressed(int keycode);
void analog_update();

void switch_keys(int num);
void init_keys();
void set_default_keys();

#define JOY_MARGIN 30

#define INPUT_START1 ((currentButtonState & (1+2)) == 1+2)
#define INPUT_START2 ((currentButtonState & (16+32)) == 16+32)
#define INPUT_P1_BUTTON1 (currentButtonState & 1)
#define INPUT_P1_BUTTON2 (currentButtonState & 2)
#define INPUT_P1_BUTTON3 (currentButtonState & 4)
#define INPUT_P1_BUTTON4 (currentButtonState & 8)
#define INPUT_P1_DIGITAL_RIGHT ((currentJoy1X>0) && (currentJoy1X>JOY_MARGIN))
#define INPUT_P1_DIGITAL_LEFT ((currentJoy1X<0) && (currentJoy1X<-JOY_MARGIN))
#define INPUT_P1_DIGITAL_UP ((currentJoy1Y>0) && (currentJoy1Y>JOY_MARGIN))
#define INPUT_P1_DIGITAL_DOWN ((currentJoy1Y<0) && (currentJoy1Y<-JOY_MARGIN))
#define INPUT_P2_BUTTON1 (currentButtonState & 16)
#define INPUT_P2_BUTTON2 (currentButtonState & 32)
#define INPUT_P2_BUTTON3 (currentButtonState & 64)
#define INPUT_P2_BUTTON4 (currentButtonState & 128)
#define INPUT_P2_DIGITAL_RIGHT ((currentJoy2X>0) && (currentJoy2X>JOY_MARGIN))
#define INPUT_P2_DIGITAL_LEFT ((currentJoy2X<0) && (currentJoy2X<-JOY_MARGIN))
#define INPUT_P2_DIGITAL_UP ((currentJoy2Y>0) && (currentJoy2Y>JOY_MARGIN))
#define INPUT_P2_DIGITAL_DOWN ((currentJoy2Y<0) && (currentJoy2Y<-JOY_MARGIN))

#define INPUT_TEST 0
#define INPUT_COIN1 ((currentButtonState & (4+8)) == (4+8))
#define INPUT_COIN2 ((currentButtonState & (64+128)) == (64+128))
#define INPUT_RESET 0

#define INPUT_ANALOG_P1_SIGNED_8BIT_HORIZONTAL (currentJoy1X)
#define INPUT_ANALOG_P1_SIGNED_8BIT_VERTICAL (currentJoy1Y)
#define INPUT_ANALOG_P2_SIGNED_8BIT_HORIZONTAL (currentJoy2X)
#define INPUT_ANALOG_P2_SIGNED_8BIT_VERTICAL (currentJoy2Y)

#endif 