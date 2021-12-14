/***************************************************************************

  atari_vg.c

  Generic functions used by the Atari Vector games

***************************************************************************/
#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

#include "../driver.h"
#include "../vector.h"
#include "swavg.h"

/*
 * This initialises the colors for all atari games
 * We must have the basic palette (8 colors) in color_prom[],
 * and the rest is set up to the correct intensity
 * This code is derived from Brad
 */

void atari_vg_init_colors (unsigned char *palette, unsigned char *colortable,const unsigned char *color_prom)
{
	;
}


void sw_vg_init_colors (unsigned char *palette, unsigned char *colortable,const unsigned char *color_prom)
{
	;
}


/* If you want to use this, please make sure that you have
 * a fake GfxLayout, otherwise you'll crash */
void atari_vg_colorram_w (int offset, int data)
{
	;
}


/***************************************************************************

  Stop the video hardware emulation.

***************************************************************************/
void atari_vg_stop(void)
{
	vg_stop ();
}

/***************************************************************************

  Draw the game screen in the given osd_bitmap.
  Do NOT call osd_update_display() from this function, it will be called by
  the main emulation engine.

***************************************************************************/
void atari_vg_screenrefresh(struct osd_bitmap *bitmap)
{
/* This routine does nothing - writes to vg_go start the drawing process */
printf("Do atari_vg_screenrefresh?\n");
}


/*
 * AVG-games start like this
 */
int atari_vg_avg_start(void)
{
	if (vg_init (0x1000, USE_AVG,0))
		return 1;
	return 0;
}

/*
 * DVG-games start like this
 */
int atari_vg_dvg_start(void)
{

	if (vg_init (0x800, USE_DVG,0))
		return 1;
	return 0;
}

/*
 * Starwars starts like this
 */
int atari_vg_avg_flip_start(void)
{
	if (vg_init(0x4000,USE_AVG,1))
		return 1;
	return 0;
}

