/*
After acchieving a high score 

->
after entering initials

@4e8e Unknown wr addr 0800 val ff data 00 tag 03
breakpoint, PC=4e8e
A:ff X:f1 Y:01 Nvbdizc SP:1fc  wrap: 0  cyc: 190445020  irq_cyc: 190449371
*/


/*
 * main.c: Atari Vector game simulator
 *
 * Copyright 1991-1993, 1996, 2003 Hedley Rainnie, Doug Neubauer, and Eric Smith
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
 * $Id: main.c,v 1.1 2018/07/31 01:19:45 pi Exp $
 */

#ifndef __GNUC__
#define inline
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../sim/memory.h"
#include "../sim/game.h"
#include "../sim/display.h"
#include "../sim/sim6502.h"

#include <vectrex/vectrexInterface.h>

char gameMemory[4*65536];

void setDimensions(int offsetx, int offsety, int mulx, int muly);
int main(int argc, char *argv[])
{
  vectrexinit(1);
  v_init();
  v_noSound();

  ProgName = "spaceduel"; // defined in display.h
  int smallwindow = 1;
  int use_pixmap = 1;
  int line_width = 0;


  init_graphics (smallwindow, use_pixmap, line_width, game_name (game));
  setDimensions(500, 350, 32, 50);
  v_setCustomClipping(1, -17000, -21000, 17000, 20000);

  // special vectrex  interface settings for Bzone
  v_setRefresh(50);
  v_setClientHz(62); // this should be 62.5 
  v_setupIRQHandling();
  v_enableSoundOut(1);
  v_enableButtons(1);
  useDoubleTimer = 1;
  v_enableJoystickAnalog(1,1,1,1);;  
  
  mem = (elem *) gameMemory;
  game = pick_game (SINGLE_GAME);
  setup_game ();

  save_PC = (memrd(0xfffd,0,0) << 8) | memrd(0xfffc,0,0);
  save_A = 0;
  save_X = 0;
  save_Y = 0;
  save_flags = 0;
  save_totcycles = 0;
  irq_cycle = 8192;
  sim_6502 ();
  while (1)
  {
    ; // do not "return" - never call _libc_fini_array etc...
  }
}
