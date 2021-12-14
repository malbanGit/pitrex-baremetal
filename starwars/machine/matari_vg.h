#ifndef _matari_vg_h
#define _matari_vg_h

/***************************************************************************

  atari_vg.h

  Generic functions used by the Atari Vector games

***************************************************************************/

void atari_vg_go(int offset, int data);
int atari_vg_earom_r(int offset);
void atari_vg_earom_w(int offset,int data);
void atari_vg_earom_ctrl(int offset, int data);
int atari_vg_earom_load(const char *name);
void atari_vg_earom_save(const char *name);

#endif
