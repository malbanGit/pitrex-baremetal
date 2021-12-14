/*********************************************************************

  common.h

  Generic functions, mostly ROM and graphics related.

*********************************************************************/

#ifndef MAMECOMMON_H
#define MAMECOMMON_H

#include "osdepend.h"


struct RomModule
{
	const char *name;	/* name of the file to load */
	unsigned int offset;			/* offset to load it to */
	unsigned int length;			/* length of the file */
	unsigned int checksum;		/* our custom checksum */
};

/* there are some special cases for the above. name, offset and size all set to 0 */
/* mark the end of the aray. If name is 0 and the others aren't, that means "continue */
/* reading the previous from from this address". If length is 0 and offset is not 0, */
/* that marks the start of a new memory region. Confused? Well, don't worry, just use */
/* the macros below. */

/* start of table */
//#define ROM_START(name) static struct RomModule name[] = {
/* start of memory region */
//#define ROM_REGION(length) { 0, length, 0, 0 },
/* OBSOLETE - don't use in new code, use ROM_LOAD() instead */
//#define ROM_OBSOLETELOAD(name,offset,length) { name, offset, length, -1 },
/* ROM to load */
//#define ROM_LOAD(name,offset,length,checksum) { name, offset, length, checksum },
/* continue loading the previous ROM to a new address */
//#define ROM_CONTINUE(offset,length) { 0, offset, length, 0 },
/* restart loading the previous ROM to a new address */
//#define ROM_RELOAD(offset,length) { (char *)-1, offset, length, 0 },
/* end of table */
//#define ROM_END { 0, 0, 0, 0 } };



struct GameSample
{
	int length;
        int smpfreq;
        unsigned char resolution;
        unsigned char volume;
	char data[1];	/* extendable */
};

struct GameSamples
{
	int total;	/* total number of samples */
	struct GameSample *sample[1];	/* extendable */
};



struct GfxLayout
{
	int width,height;	/* width and height of chars/sprites */
	int total;	/* total numer of chars/sprites in the rom */
	int planes;	/* number of bitplanes */
	int planeoffset[8];	/* start of every bitplane */
	int xoffset[64];	/* coordinates of the bit corresponding to the pixel */
	int yoffset[64];	/* of the given coordinates */
	int charincrement;	/* distance between two consecutive characters/sprites */
};



struct GfxElement
{
	int width,height;

	struct osd_bitmap *gfxdata;	/* graphic data */
	int total_elements;	/* total number of characters/sprites */

	int color_granularity;	/* number of colors for each color code */
							/* (for example, 4 for 2 bitplanes gfx) */
	unsigned char *colortable;	/* map color codes to screen pens */
								/* if this is 0, the function does a verbatim copy */
	int total_colors;
};



struct rectangle
{
	int min_x,max_x;
	int min_y,max_y;
};



#define TRANSPARENCY_NONE 0
#define TRANSPARENCY_PEN 1
#define TRANSPARENCY_COLOR 2
#define TRANSPARENCY_THROUGH 3

void showdisclaimer(void);

int readroms(const struct RomModule *romp,const char *basename);
void printromlist(const struct RomModule *romp,const char *basename);
struct GameSamples *readsamples(const char **samplenames,const char *basename);
void freesamples(struct GameSamples *samples);
void decodechar(struct GfxElement *gfx,int num,const unsigned char *src,const struct GfxLayout *gl);
struct GfxElement *decodegfx(const unsigned char *src,const struct GfxLayout *gl);
void freegfx(struct GfxElement *gfx);
void drawgfx(struct osd_bitmap *dest,const struct GfxElement *gfx,
		unsigned int code,unsigned int color,int flipx,int flipy,int sx,int sy,
		const struct rectangle *clip,int transparency,int transparent_color);
void copybitmap(struct osd_bitmap *dest,struct osd_bitmap *src,int flipx,int flipy,int sx,int sy,
		const struct rectangle *clip,int transparency,int transparent_color);
void copyscrollbitmap(struct osd_bitmap *dest,struct osd_bitmap *src,
		int rows,const int *rowscroll,int cols,const int *colscroll,
		const struct rectangle *clip,int transparency,int transparent_color);
void clearbitmap(struct osd_bitmap *bitmap);

#endif
