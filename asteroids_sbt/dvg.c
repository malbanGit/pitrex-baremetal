/*
 * display.c: Atari DVG and AVG simulators
 *
 * Copyright 1991, 1992, 1996, 2003 Eric Smith
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: display.c 2 2003-08-20 01:51:05Z eric $


 2006-10-31 modified by dwelch

 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include <vectrex/vectrexInterface.h>

#define MAXSTACK 4

#define VCTR 0
#define HALT 1
#define SVEC 2
#define STAT 3
#define CNTR 4
#define JSRL 5
#define RTSL 6
#define JMPL 7
#define SCAL 8

#define DVCTR 0x01
#define DLABS 0x0a
#define DHALT 0x0b
#define DJSRL 0x0c
#define DRTSL 0x0d
#define DJMPL 0x0e
#define DSVEC 0x0f

#define twos_comp_val(num,bits) ((num&(1<<(bits-1)))?(num|~((1<<bits)-1)):(num&((1<<bits)-1)))

extern unsigned char ram[0x8000];
#define ReadMemory(x)  (ram[x])
int endGameRound(); // from gmain.c

  
#define SCREEN_H 1024
#define SCREEN_W 1024

#define OFFSET_X 500
#define OFFSET_Y 400
#define MUL_X 35
#define MUL_Y 50


void draw_line ( long oldx, long oldy, long currentx, long currenty, int xyz, int z)
{
  if(z==0) return;
  oldy=SCREEN_H-oldy-250;
  currenty=SCREEN_H-currenty-250;
  v_directDraw32(oldx=(oldx - OFFSET_X)*MUL_X, oldy=(oldy - OFFSET_Y)*MUL_Y, currentx=(currentx - OFFSET_X)*MUL_X,currenty=(currenty - OFFSET_Y)*MUL_Y, z*8);
}

void draw_brightDot ( long oldx, long oldy, long currentx, long currenty, int xyz, int z)
{
    if(z != 15)
    {
      draw_line (  oldx,  oldy,  currentx,  currenty,  xyz,  z);
      return ;
    }
    oldy=SCREEN_H-oldy-250;
    currenty=SCREEN_H-currenty-250;
    
    int olddwell = v_dotDwell;
    int ran = random()%200;
    v_dotDwell = 200+ran;
    v_directDraw32Hinted(oldx=(oldx - OFFSET_X)*MUL_X, oldy=(oldy - OFFSET_Y)*MUL_Y, currentx=(currentx - OFFSET_X)*MUL_X,currenty=(currenty - OFFSET_Y)*MUL_Y, z*8, PL_BASE_FORCE_USE_DOT_DWELL);
    v_dotDwell = olddwell;
}

int dvg_draw_screen (void)
{
    int pc;
    int sp;
    int stack [MAXSTACK];

    long scale;
    int statz;

    long currentx;
    long currenty;

    int done = 0;

    int firstwd, secondwd;
    int opcode;

    long x, y;
    int z, temp;
    int a;

    long oldx, oldy;
    long deltax, deltay;

    pc = 0;
    sp = 0;
    scale = 0;
    statz = 0;
    currentx = 1023 * 8192;
    currenty = 512 * 8192;

    while (!done)
    {
        firstwd = ReadMemory(0x4000+(pc<<1)+1); firstwd<<=8; firstwd|=ReadMemory(0x4000+(pc<<1)+0);
        opcode = firstwd >> 12;
        pc++;
        if ((opcode >= 0 /* DVCTR */) && (opcode <= DLABS))
        {
            secondwd = ReadMemory(0x4000+(pc<<1)+1); secondwd<<=8; secondwd|=ReadMemory(0x4000+(pc<<1)+0);
            pc++;
        }


        switch (opcode)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                y = firstwd & 0x03ff; if (firstwd & 0x0400) y = -y;
                x = secondwd & 0x03ff;if (secondwd & 0x400) x = -x;
                z = secondwd >> 12;
                oldx = currentx; oldy = currenty;
                temp = (scale + opcode) & 0x0f;
                if (temp > 9) temp = -1;
                deltax = (x << 21) >> (30 - temp);
                deltay = (y << 21) >> (30 - temp);
                currentx += deltax;
                currenty -= deltay;
                draw_brightDot (oldx, oldy, currentx, currenty, 7, z);
                break;

            case DLABS:
                x = twos_comp_val (secondwd, 12);
                y = twos_comp_val (firstwd, 12);
                /*
                x = secondwd & 0x07ff;
                if (secondwd & 0x0800)
                x = x - 0x1000;
                y = firstwd & 0x07ff;
                if (firstwd & 0x0800)
                y = y - 0x1000;
                */
                scale = secondwd >> 12;
                currentx = x;
                currenty = (896 - y);
                break;

            case DHALT:
                done = 1;
                break;

            case DJSRL:
                a = firstwd & 0x0fff;
                stack [sp] = pc;
                if (sp == (MAXSTACK - 1))
                {
                    printf ("\n*** Vector generator stack overflow! ***\n");
                    done = 1;
                    sp = 0;
                }
                else
                    sp++;
                pc = a;
                break;

            case DRTSL:
                if (sp == 0)
                {
                    printf ("\n*** Vector generator stack underflow! ***\n");
                    done = 1;
                    sp = MAXSTACK - 1;
                }
                else
                    sp--;
                pc = stack [sp];
                break;

            case DJMPL:
                a = firstwd & 0x0fff;
                pc = a;
                break;

            case DSVEC:
                y = firstwd & 0x0300; if (firstwd & 0x0400) y = -y;
                x = (firstwd & 0x03) << 8; if (firstwd & 0x04) x = -x;
                z = (firstwd >> 4) & 0x0f;
                temp = 2 + ((firstwd >> 2) & 0x02) + ((firstwd >> 11) & 0x01);
                oldx = currentx; oldy = currenty;
                temp = (scale + temp) & 0x0f;
                if (temp > 9) temp = -1;
                deltax = (x << 21) >> (30 - temp);
                deltay = (y << 21) >> (30 - temp);
                currentx += deltax;
                currenty -= deltay;
                draw_line (oldx, oldy, currentx, currenty, 7, z);
                break;

            default:
                printf("internal error\n");
                done = 1;
        }
    }
    return endGameRound();
}
