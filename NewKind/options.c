/*
 * Elite - The New Kind.
 *
 * Reverse engineered from the BBC disk version of Elite.
 * Additional material by C.J.Pinder.
 *
 * The original Elite code is (C) I.Bell & D.Braben 1984.
 * This version re-engineered in C by C.J.Pinder 1999-2001.
 *
 * email: <christian@newkind.co.uk>
 *
 *
 */

/*
 * Options.c
 */

#include <stdlib.h>
#include <string.h>

#include <vectrex/vectrexInterface.h>

#include "elite.h"
#include "config.h"
#include "gfx.h"
#include "options.h"
#include "main.h"
#include "docked.h"
#include "keyboard.h"
#include "file.h" 

static int hilite_item;
 
#define NUM_OPTIONS 4
#define NUM_SETTINGS 6

#define OPTION_BAR_WIDTH	(400)
#define OPTION_BAR_HEIGHT	(15)

struct option
{
	char *text;
	int docked_only;
};

static struct option option_list[NUM_OPTIONS] =
{
	{"Save Commander",	1},
	{"Load Commander",	1},
	{"Change Name",	0},
	{"Quit",			0}	
};

struct setting
{
	char *name;
	char *value[5];
};

static struct setting setting_list[NUM_SETTINGS] =
{
	{"Graphics:",		{"Solid", "Wireframe", "", "", ""}},
	{"Anti Alias:",		{"Off", "On", "", "", ""}},		
	{"Planet Style:",	{"Wireframe", "Green", "SNES", "Fractal", ""}},
	{"Planet Desc.:",	{"BBC", "MSX", "", "", ""}},
	{"Instant Dock:",	{"Off", "On", "", "", ""}},	
	{"Save Settings",	{"", "", "", "", ""}}
};


void quit_screen (void)
{
	current_screen = SCR_QUIT;

	gfx_clear_display();
	gfx_display_centre_text_b (10, "GAME OPTIONS", 140, GFX_COL_GOLD);
	gfx_draw_line (0, 36, 511, 36);

	gfx_display_centre_text_b (175, "QUIT GAME (Y/N)?", 140, GFX_COL_GOLD);		
}





void display_setting_item (int item)
{
	int x,y;
	int v;

	if (item == (NUM_SETTINGS - 1))
	{
		y = ((NUM_SETTINGS + 1) / 2) * 30 + 96 + 32;
		gfx_display_centre_text (y, setting_list[item].name, 120, GFX_COL_WHITE);
		return;
	}
	
	switch (item)
	{
		case 0:
			v = wireframe;
			break;
		
		case 1:
			v = anti_alias_gfx;
			break;
		
		case 2:
			v = planet_render_style;
			break;
		
		case 3:
			v = hoopy_casinos;
			break;
		
		case 4:
			v = instant_dock;
			break;

		default:
			v = 0;
			break;
	}
	
	x = (item & 1) * 250 + 32; 
	y = (item / 2) * 30 + 96;
	
	gfx_display_colour_text (x, y, setting_list[item].name, GFX_COL_WHITE);
	gfx_display_colour_text (x + 120, y, setting_list[item].value[v], GFX_COL_WHITE);
}


void highlight_setting (int item)
{
	int x,y;
	int width;
	
	if ((hilite_item != -1) && (hilite_item != item))
	{
		if (hilite_item == (NUM_SETTINGS - 1))
		{
			x = GFX_X_CENTRE - (OPTION_BAR_WIDTH / 2);
			y = ((NUM_SETTINGS + 1) / 2) * 30 + 96 + 32;
			width = OPTION_BAR_WIDTH;
		}
		else
		{
			x = (hilite_item & 1) * 250 + 32 + 120; 
			y = (hilite_item / 2) * 30 + 96;
			width = 100;
		}

		gfx_clear_area (x, y, x + width, y + OPTION_BAR_HEIGHT);
		display_setting_item (hilite_item);		
	}

	if (item == (NUM_SETTINGS - 1))
	{
		x = GFX_X_CENTRE - (OPTION_BAR_WIDTH / 2);
		y = ((NUM_SETTINGS + 1) / 2) * 30 + 96 + 32;
		width = OPTION_BAR_WIDTH;
	}
	else
	{
		x = (item & 1) * 250 + 32 + 120; 
		y = (item / 2) * 30 + 96;
		width = 100;
	}
	
	gfx_draw_rectangle (x, y, x + width, y + OPTION_BAR_HEIGHT, GFX_COL_DARK_RED);
	display_setting_item (item);		
	hilite_item = item;
}



void select_left_setting (void)
{
	if ((hilite_item & 1) != 0)
		highlight_setting (hilite_item - 1);
}

void select_right_setting (void)
{
	if (((hilite_item & 1) == 0) && (hilite_item < (NUM_SETTINGS - 1)))
		highlight_setting (hilite_item + 1);
}


void select_up_setting (void)
{
	if (hilite_item == (NUM_SETTINGS - 1))
	{
		highlight_setting (NUM_SETTINGS - 2);
		return;
	}

	if (hilite_item > 1)
		highlight_setting (hilite_item - 2);
}


void select_down_setting (void)
{
	if (hilite_item == (NUM_SETTINGS - 2))
	{
		highlight_setting (NUM_SETTINGS - 1);
		return;
	}
	
	if (hilite_item < (NUM_SETTINGS - 2))
		highlight_setting (hilite_item + 2);
}

void toggle_setting (void)
{
	if (hilite_item == (NUM_SETTINGS - 1))
	{
		write_config_file();
		display_options();
		return;
	}

	switch (hilite_item)
	{
		case 0:
			wireframe ^= 1;
			break;
		
		case 1:
			anti_alias_gfx ^= 1;
			break;
		
		case 2:
			planet_render_style = (planet_render_style + 1) % 4;
			break;
		
		case 3:
			hoopy_casinos ^= 1;
			break;

		case 4:
			instant_dock ^= 1;
			break;
	}

	highlight_setting (hilite_item);
}


void changeName_screen (void)
{
	int i;
    char str[80];
//	current_screen = SCR_SETTINGS;
    int lastDel = 0;
    int enter=1;
    do
    {
      gfx_clear_display();
      gfx_display_centre_text (10, "Change Name", 140, 0x5f);
      gfx_draw_line (0, 36, 511, 36);

      sprintf (str, "Name: %s_", cmdr.name);
      v_setBrightness(0x50);        /* set intensity of vector beam... */
      gfx_display_text (16, 300, str);
      gfx_update_screen();
      kbd_poll_keyboard();
      int len =strlen(cmdr.name);
      if (kbd_any==0) enter=0;
      if ((kbd_backspace_pressed) && (lastDel==0))
      {
        if (len>0) cmdr.name[len-1] = 0;
        lastDel=1;
        enter=0;
      }
      else
      {
        lastDel=0;
        if (kbd_ascii != 0)
        {
          if (len<31)
          {
            cmdr.name[len] = kbd_ascii;
            cmdr.name[len+1] = 0;
          }
        }
      }
        
    } while (!kbd_enter_pressed);
}


void display_option_item (int i)
{
	int y;
	int col;
	
	y = (384 - (30 * NUM_OPTIONS)) / 2;
	y += i * 30;
	col = ((!docked) && option_list[i].docked_only) ? 0x20 : 0x50;

//	gfx_display_centre_text (y, option_list[i].text, 120, col);
    itemListAdd(option_list[i].text, col);
}


void highlight_option (int i)
{
	int y;
	int x;
	
	if ((hilite_item != -1) && (hilite_item != i))
	{
		x = GFX_X_CENTRE - (OPTION_BAR_WIDTH / 2);
		y = (384 - (30 * NUM_OPTIONS)) / 2;
		y += hilite_item * 30;
		gfx_clear_area (x, y, x + OPTION_BAR_WIDTH, y + OPTION_BAR_HEIGHT);
		display_option_item (hilite_item);		
	}

	x = GFX_X_CENTRE - (OPTION_BAR_WIDTH / 2);
	y = (384 - (30 * NUM_OPTIONS)) / 2;
	y += i * 30;
	
	gfx_draw_rectangle (x, y, x + OPTION_BAR_WIDTH, y + OPTION_BAR_HEIGHT,
						GFX_COL_DARK_RED);
	display_option_item (i);		

	hilite_item = i;
}

void select_previous_option (void)
{
	if (hilite_item > 0)
		highlight_option (hilite_item - 1);
}

void select_next_option (void)
{
	if (hilite_item < (NUM_OPTIONS - 1))
		highlight_option (hilite_item + 1);
}


void do_option (void)
{
	if ((!docked) && option_list[hilite_item].docked_only)
		return;

	switch (hilite_item)
	{
		case 0:
			save_commander_screen();
			break;
			
		case 1:
			load_commander_screen();
			break;
		
		case 2:
			changeName_screen();
			break;
		
		case 3:
			quit_screen();
			break;
	}
}

static int iTimer = 0;
static int iText = 0;
static int iBrightness = 0;
static int iDirection = 0;
extern int last_screen;

void display_options (void)
{
	int i;
	gfx_clear_display();
	gfx_display_centre_text (10, "GAME OPTIONS", 140, 0x5f);
	gfx_draw_line (0, 36, 511, 36);
	
    if (last_screen != current_screen)
    {
      itemListClear();
      defaultDisplayList.fade = 0;
      defaultDisplayList.autoRaster = 1;
      defaultDisplayList.displayRectangle = 220;
      defaultDisplayList.fontWidth = 40;
      defaultDisplayList.fontHeight = -4;
      defaultDisplayList.itemDisplayCount = 5;
      defaultDisplayList.lineSpacing = 3;
      defaultDisplayList.rlines = alternate_raster5;
      defaultDisplayList.rasterlineCount = 5;

      itemListSetPos(-100, 50);

      
      for (i = 0; i < NUM_OPTIONS; i++)
          display_option_item (i);

      hilite_item = -1;
//      highlight_option (0);
      
    iTimer = 0;
    iText = 0;
    iBrightness = 0x10;
    iDirection = 0;
      
      
    }

    itemListDisplay();
    
    if (iDirection == 0)
    {
      iBrightness+=5;
      if (iBrightness >= 0x6f) iDirection = 1;
    }
    else if (iDirection == 1)
    {
      iBrightness-=5;
      if (iBrightness <= 0x10) 
      {
        iDirection = 0;
        iText++;
        if (iText == 5) iText = 0;
      }
    }
    
    if (iText == 0)
      gfx_display_centre_text_b (380, "Version: Release 1.0", 120, iBrightness);
    else if (iText == 1)
      gfx_display_centre_text_b (380, "www.newkind.co.uk", 120, iBrightness);
    else if (iText == 2)
      gfx_display_centre_text_b (380,"By Christian Pinder 1999-2001", 140, iBrightness);
    else if (iText == 3)
      gfx_display_centre_text_b (380,"Based on original code by", 140, iBrightness);
    else if (iText == 4)
      gfx_display_centre_text_b (380,"Ian Bell & David Braben", 140, iBrightness);

    hilite_item = defaultDisplayList.currentMainItem;

    
    
  //  gfx_display_centre_text (300, "Version: Release 1.0", 120, GFX_COL_WHITE);
//	gfx_display_centre_text (320, "www.newkind.co.uk", 120, GFX_COL_WHITE);
//	gfx_display_centre_text (340, "By Christian Pinder 1999-2001", 120, GFX_COL_WHITE);
//    gfx_display_centre_text (360, "Based on original code by", 120, GFX_COL_WHITE);
//    gfx_display_centre_text (380, "Ian Bell & David Braben", 120, GFX_COL_WHITE);
	
}
