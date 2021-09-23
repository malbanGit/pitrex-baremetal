#ifndef MAME_H
#define MAME_H

//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#include "osdepend.h"
#include "mamecommon.h"


#define MAX_GFX_ELEMENTS 10
#define MAX_MEMORY_REGIONS 10
#define MAX_PENS 256	/* can't handle more than 256 colors on screen */

struct RunningMachine
{
	unsigned char *memory_region[MAX_MEMORY_REGIONS];
	struct osd_bitmap *scrbitmap;	/* bitmap to draw into */
	struct GfxElement *gfx[MAX_GFX_ELEMENTS];	/* graphic sets (chars, sprites) */
	struct GfxElement *uifont;	/* font used by DisplayText() */
	unsigned char pens[MAX_PENS];	/* remapped palette pen numbers. When you write */
								/* directly to a bitmap never use absolute values, */
								/* use this array to get the pen number. For example, */
								/* if you want to use color #6 in the palette, use */
								/* pens[6] instead of just 6. */
	const struct GameDriver *gamedrv;	/* contains the definition of the game machine */
	const struct MachineDriver *drv;	/* same as gamedrv->drv */
	struct GameSamples *samples;	/* samples loaded from disk */
	struct NewInputPort *input_ports;	/* the input ports definition from the driver */
								/* is copied here and modified (load settings from disk, */
								/* remove cheat commands, and so on) */
	int orientation;	/* see #defines in driver.h */
};


extern struct RunningMachine *Machine;
extern unsigned char *RAM;	/* pointer to the memory region of the active CPU */
extern unsigned char *ROM;





#endif
