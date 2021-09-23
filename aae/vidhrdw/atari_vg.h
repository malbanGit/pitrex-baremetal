#ifndef _atari_vg_h
#define _atari_vg_h

/***************************************************************************

  atari_vg.h

  Generic functions used by the Atari Vector games

***************************************************************************/

void atari_vg_init_colors (unsigned char *palette, unsigned char *colortable,const unsigned char *color_prom);
void sw_vg_init_colors(unsigned char *palette, unsigned char *colortable,const unsigned char *color_prom);
void atari_vg_stop(void);
void atari_vg_screenrefresh(struct osd_bitmap *bitmap);
void atari_vg_colorram_w(int offset, int data);
int atari_vg_avg_start(void);
int atari_vg_dvg_start(void);
int atari_vg_avg_flip_start(void);

#endif
