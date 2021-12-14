/***************************************************************************

  machine.c

  Functions to emulate general aspects of the machine (RAM, ROM, interrupts,
  I/O ports)

***************************************************************************/

#include "../driver.h"
#include "matari_vg.h"
#include "../vidhrdw/swavg.h"

static int earom_offset;
static int earom_data;
static char earom[64];

void atari_vg_go(int offset, int data)
{
//  printf("matari_vg - atari_vg_go() called \n");
	vg_go(cpu_gettotalcycles());
}


int atari_vg_earom_r (int offset)
{
	if (errorlog) 
		fprintf (errorlog, "read earom: %d\n",offset);
	return (earom_data);
}

void atari_vg_earom_w (int offset, int data)
{
	if (errorlog)
		fprintf (errorlog, "write earom: %d:%d\n",offset, data);
	earom_offset=offset;
	earom_data=data;
}

/* 0,8 and 14 get written to this location, too.
 * Don't know what they do exactly 
 */
void atari_vg_earom_ctrl (int offset, int data)
{
	if (errorlog)
		fprintf (errorlog, "earom ctrl: %d:%d\n",offset, data);

	if (data == 9)
		earom_data=earom[earom_offset];
	if (data == 12)
		earom[earom_offset]=earom_data;
}


int atari_vg_earom_load(const char *name)
{
	/* We read the EAROM contents from disk */
        /* No check necessary */
	FILE *f;

	if ((f = fopen(name,"rb")) != 0)
	{
		if (fread(&earom[0],1,0x40,f));
		fclose(f);
	}
	return 1;
}

void atari_vg_earom_save(const char *name)
{
	FILE *f;

	if ((f = fopen(name,"wb")) != 0)
	{
		fwrite(&earom[0],1,0x40,f);
		fclose(f);
	}
}
