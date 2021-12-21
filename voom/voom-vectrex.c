/*
 *  Copyright (C) 2015 Jeroen Domburg <jeroen at spritesmods.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <vectrex/vectrexInterface.h>

#include "voom.h"
#include "voom.c"

static int voomInited=0;
//unsigned char *lvlData; //ptr to storage for level data
unsigned char lvlData[48*1024];

//Array to keep lines in
typedef struct {
	int x;
	int y;
} Vertex;

typedef struct {
	Vertex p[2];
} Line2d;

#define LINEMAX 200
static Line2d lines[LINEMAX];
static int lineIdx;


void veLineAdd(int xs, int ys, int xe, int ye) 
{
	int t;
	if (lineIdx==LINEMAX) 
	  return;
	if (xs>xe) 
	{
		t=xs; xs=xe; xe=t;
		t=ys; ys=ye; ye=t;
	}

	if (xs<0 || xs>SIZEX) return;
	if (ys<0 || ys>SIZEX) return;
	if (xe<0 || xe>SIZEX) return;
	if (ye<0 || ye>SIZEX) return;

	if (xs==xe && ys==ye) return;

	lines[lineIdx].p[0].x=xs;
	lines[lineIdx].p[0].y=ys;
	lines[lineIdx].p[1].x=xe;
	lines[lineIdx].p[1].y=ye;
	lineIdx++;
}

void vline(int32_t xStart, int32_t yStart, int32_t xEnd, int32_t yEnd, uint8_t brightness)
{
  xStart -=128;
  yStart -=128;
  xEnd -=128;
  yEnd -=128;

  xStart *=100;
  yStart *=100;
  xEnd *=100;
  yEnd *=100;

  yStart = -yStart;
  yEnd = -yEnd;
  v_directDraw32(xStart, yStart,xEnd, yEnd, brightness);

}

static void linesDraw() 
{
	int i, j, k, d, dx, dy;
	int cd, ci, inv;
	int x=SIZEX/2,y=SIZEY/2;
	int nwr=0;

//	xprintf("Drawing %d lines.\n", lineIdx);
//printf("--\n",d);
	for (i=0; i<lineIdx; i++) 
	{
		nwr++;
		//Find closest line
		cd=99999; ci=0;
		for (j=0; j<lineIdx; j++) 
		{
			if (lines[j].p[0].x!=-1) 
			{
				for (k=0; k<2; k++) 
				{
					dx=abs(lines[j].p[k].x-x);
					dy=abs(lines[j].p[k].y-y);
					if (dx>dy) d=dx; else d=dy;
					if (d<cd) 
					{
						cd=d;
						ci=j;
						inv=k;
					}
				}
			}
		}

		dx=lines[ci].p[inv].x-x;
		dy=lines[ci].p[inv].y-y;
//printf("d: %i\n",cd);

		if (dx>SIZEX/2 || dy>SIZEY/2 || nwr>3) 
		{
			//Zero integrators.
			nwr=0;
			x=(SIZEX/2);
			y=(SIZEY/2);
			dx=lines[ci].p[inv].x-x;
			dy=lines[ci].p[inv].y-y;
		}
		if (dx!=0 || dy!=0) 
		{
		  x+=dx;
  		  y+=dy;
		}
int b=0x60;
/*
b=b/4;
b = 0x60 -b;
if (b<0) b=0;
*/
		
		vline(lines[ci].p[inv].x, lines[ci].p[inv].y,lines[ci].p[inv^1].x, lines[ci].p[inv^1].y, b);



		dx=lines[ci].p[inv^1].x-x;
		dy=lines[ci].p[inv^1].y-y;
		x+=dx;
		y+=dy;

		//Poison line
		lines[ci].p[0].x=-1;
	}
//	printf("Drew %i lines in %i bytes\n", lineIdx, p-cartSpace);
}



static void doInitVoom() {
	char *p=lvlData;
	const char *names[]={
		"e1l1/VERTEXES",
		"e1l1/LINEDEFS",
		"e1l1/SECTORS",
		"e1l1/SSECTORS",
		"e1l1/SEGS",
		"e1l1/SIDEDEFS",
		"e1l1/NODES"
	};
	FILE *f;
	FRESULT fr;
	char *dptr[7];
	int dsize[7];
	int n, i, l, t;
	l=64*1024-0x800;
	for (i=0; i<7; i++) 
	{
		f=fopen(names[i], "r");
		if (!f) return;
		n = fread(p, 1, 100000, f);
		
		printf("Loaded %s: %d\n", names[i], n);
		fclose(f);
		dptr[i]=p;
		dsize[i]=n;
		p+=n;
		l=l-n;
	}

	//NOTE: This load is pretty much endian dependent and only works on any CPU
	//with the same endian-ness as the x86 Doom originally was made for.

	voomInit(dptr, dsize);
	printf("Loaded %d bytes.\n", n);
	voomInited=1;
}
void voomVectrexFrame(int keys) 
{
	if (!voomInited) doInitVoom();
	voomHandleKeys(keys);
	lineIdx=0;
	voomDraw();
	linesDraw();
}

int main( void )
{
  vectrexinit(1);
  v_init();

  
  v_noSound();
  // special vectrex  interface settings for asteroids
  v_setRefresh(50);
  v_setClientHz(62); // 62.5
  v_setupIRQHandling();
  v_enableJoystickDigital(1,1,0,0);
  v_enableSoundOut(1);
  v_enableButtons(1);
  keepDotsTogether = 0;
  useDoubleTimer = 1;

  
  while(1)  
  {
    int key = 0;
    v_WaitRecal();
    if (currentJoy1Y==1) key = key |VOOM_KEY_UP;
    if (currentJoy1Y==-1) key = key |VOOM_KEY_DOWN;
    if (currentJoy1X==-1) key = key |VOOM_KEY_LEFT;
    if (currentJoy1X==1) key = key |VOOM_KEY_RIGHT;
    
    voomVectrexFrame(key);
  }
}
