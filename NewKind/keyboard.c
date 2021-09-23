/*
 * Elite - The New Kind.
 *
 * Allegro version of the keyboard routines.
 *
 * The code in this file has not been derived from the original Elite code.
 * Written by C.J.Pinder 1999-2001.
 * email: <christian@newkind.co.uk>
 *
 */

/*
 * keyboard.c
 *
 * Code to handle keyboard input.
 */

#include <stdlib.h>
#include <string.h>
 
#include <vectrex/vectrexInterface.h>

#include "allegro_dummy.h"

int kbd_F1_pressed;
int kbd_F2_pressed;
int kbd_F3_pressed;
int kbd_F4_pressed;
int kbd_F5_pressed;
int kbd_F6_pressed;
int kbd_F7_pressed;
int kbd_F8_pressed;
int kbd_F9_pressed;
int kbd_F10_pressed;
int kbd_F11_pressed;
int kbd_F12_pressed;
int kbd_y_pressed;
int kbd_n_pressed;
int kbd_fire_pressed;
int kbd_ecm_pressed;
int kbd_energy_bomb_pressed;
int kbd_hyperspace_pressed;
int kbd_ctrl_pressed;
int kbd_jump_pressed;
int kbd_escape_pressed;
int kbd_dock_pressed;
int kbd_d_pressed;
int kbd_origin_pressed;
int kbd_find_pressed;
int kbd_fire_missile_pressed;
int kbd_target_missile_pressed;
int kbd_unarm_missile_pressed;
int kbd_pause_pressed;
int kbd_resume_pressed;
int kbd_inc_speed_pressed;
int kbd_dec_speed_pressed;
int kbd_up_pressed;
int kbd_down_pressed;
int kbd_left_pressed;
int kbd_right_pressed;
int kbd_enter_pressed;
int kbd_backspace_pressed;
int kbd_space_pressed;
int kbd_ascii;
int kbd_any;


int kbd_keyboard_startup (void)
{
//	set_keyboard_rate(2000, 2000);
    v_initKeyboard();
	return 0;
}

int kbd_keyboard_shutdown (void)
{
	return 0;
}

extern int physicalKeyDown[127]; // usbkeyboard.c


void kbd_poll_keyboard (void)
{
    kbd_any = 0;
    unsigned char key =USBKeyboardLastPhysicalKeyDown();
    if (key != 0) 
    {
      kbd_any = 1;
      printf ("Key: %i\n\r",key);
    }
    
//    the key map has the "STATES" (pressed/not pressed) in them
//    I have to implement that in USPI lib
    kbd_ascii = 0;
    
    char *cc = USPiKeyboardPhysicalToKeyboard(key,0);
    char c = toupper(cc[0]);
    if ((c>=32) && (c<=126)) kbd_ascii =c;
    
    
    
    kbd_F1_pressed = physicalKeyDown[58];
    kbd_F2_pressed = physicalKeyDown[59];
    kbd_F3_pressed = physicalKeyDown[60];
    kbd_F4_pressed = physicalKeyDown[61];
    kbd_F5_pressed = physicalKeyDown[62];
    kbd_F6_pressed = physicalKeyDown[63];
    kbd_F7_pressed = physicalKeyDown[64];
    kbd_F8_pressed = physicalKeyDown[65];
    kbd_F9_pressed = physicalKeyDown[66];
    kbd_F10_pressed = physicalKeyDown[67];
    kbd_F11_pressed = physicalKeyDown[68];
    kbd_F12_pressed = physicalKeyDown[69];
    
    kbd_y_pressed = physicalKeyDown[28];
    kbd_n_pressed = physicalKeyDown[17];

    kbd_fire_pressed = physicalKeyDown[4];
    kbd_ecm_pressed = physicalKeyDown[8];
    kbd_energy_bomb_pressed = physicalKeyDown[43];
    kbd_hyperspace_pressed = physicalKeyDown[11];
    kbd_jump_pressed = physicalKeyDown[13];
    kbd_escape_pressed = physicalKeyDown[41];

    kbd_dock_pressed = physicalKeyDown[6];
    kbd_d_pressed = physicalKeyDown[7];
    kbd_origin_pressed = physicalKeyDown[18];
    kbd_find_pressed = physicalKeyDown[9];

    kbd_fire_missile_pressed = physicalKeyDown[16];
    kbd_target_missile_pressed = physicalKeyDown[23];
    kbd_unarm_missile_pressed = physicalKeyDown[24];
    
    kbd_pause_pressed = physicalKeyDown[19];
    kbd_resume_pressed = physicalKeyDown[21];
    
    kbd_inc_speed_pressed = physicalKeyDown[44];
    kbd_dec_speed_pressed = physicalKeyDown[56];

    kbd_enter_pressed = physicalKeyDown[40];
    kbd_backspace_pressed = physicalKeyDown[42];
    kbd_space_pressed = physicalKeyDown[44];

    kbd_up_pressed = physicalKeyDown[22] || physicalKeyDown[82];
    kbd_down_pressed = physicalKeyDown[27] || physicalKeyDown[81];
    kbd_left_pressed = physicalKeyDown[51] || physicalKeyDown[80];
    kbd_right_pressed = physicalKeyDown[55] || physicalKeyDown[79];
    
    kbd_ctrl_pressed = (USPiKeyboardGetModifiers()&(1 << 0)) || (USPiKeyboardGetModifiers()&(1 << 4));
/*
  poll_keyboard();

	kbd_F1_pressed = key[KEY_F1];
	kbd_F2_pressed = key[KEY_F2];
	kbd_F3_pressed = key[KEY_F3];
	kbd_F4_pressed = key[KEY_F4];
	kbd_F5_pressed = key[KEY_F5];
	kbd_F6_pressed = key[KEY_F6];
	kbd_F7_pressed = key[KEY_F7];
	kbd_F8_pressed = key[KEY_F8];
	kbd_F9_pressed = key[KEY_F9];
	kbd_F10_pressed = key[KEY_F10];
	kbd_F11_pressed = key[KEY_F11];
	kbd_F12_pressed = key[KEY_F12];

	kbd_y_pressed = key[KEY_Y];
	kbd_n_pressed = key[KEY_N];

    kbd_fire_pressed = key[KEY_A];
	kbd_ecm_pressed = key[KEY_E];
    kbd_energy_bomb_pressed = key[KEY_TAB];
	kbd_hyperspace_pressed = key[KEY_H];
	kbd_ctrl_pressed = (key[KEY_LCONTROL]) || (key[KEY_RCONTROL]);
	kbd_jump_pressed = key[KEY_J];
	kbd_escape_pressed = key[KEY_ESC];

    kbd_dock_pressed = key[KEY_C];
	kbd_d_pressed = key[KEY_D];
	kbd_origin_pressed = key[KEY_O];
	kbd_find_pressed = key[KEY_F];

	kbd_fire_missile_pressed = key[KEY_M];
	kbd_target_missile_pressed = key[KEY_T];
	kbd_unarm_missile_pressed = key[KEY_U];
	
	kbd_pause_pressed = key[KEY_P];
	kbd_resume_pressed = key[KEY_R];
	
	kbd_inc_speed_pressed = key[KEY_SPACE];
	kbd_dec_speed_pressed = key[KEY_SLASH];
	
	kbd_up_pressed = key[KEY_S] || key[KEY_UP];
	kbd_down_pressed = key[KEY_X] || key[KEY_DOWN];
	kbd_left_pressed = key[KEY_COMMA] || key[KEY_LEFT];
	kbd_right_pressed = key[KEY_STOP] || key[KEY_RIGHT];
	
	kbd_enter_pressed = key[KEY_ENTER];
	kbd_backspace_pressed = key[KEY_BACKSPACE];
	kbd_space_pressed = key[KEY_SPACE];

	while (keypressed())
		readkey();
*/    
}


int kbd_read_key (void)
{
  /*
	int keynum;
	int keycode;
	int keyasc;

	kbd_enter_pressed = 0;
	kbd_backspace_pressed = 0;
	
	keynum = readkey();
	keycode = keynum >> 8;
	keyasc = keynum & 255;

	if (keycode == KEY_ENTER)
	{
		kbd_enter_pressed = 1;
		return 0;
	} 

	if (keycode == KEY_BACKSPACE)
	{
		kbd_backspace_pressed = 1;
		return 0;
	} 

	return keyasc;
*/
  return 0;
}


void kbd_clear_key_buffer (void)
{
//	while (keypressed())
//		readkey();
    
}
 
