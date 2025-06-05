/*
https://github.com/historicalsource/tempest
https://arcarc.xmission.com/Web%20Archives/ionpool.net%20(Dec-31-2020)/arcade/tempest_code_project/tempest_code_project.html
https://github.com/Threetwosevensixseven/ayfxedit-improved
;
; POKEY DEFINITIONS
;
AUDF1	=POKEY
AUDC1	=POKEY+1
AUDCTL	=POKEY+8
ALLPOT	=POKEY+8
RANDOM	=POKEY+0A
POTGO	=POKEY+0B
SKCTL	=POKEY+0F
AUDF2	=POKEY2
AUDC2	=POKEY2+1
AUD2CTL	=POKEY2+8
ALLPO2	=POKEY2+8
RANDO2	=POKEY2+0A
POTGO2	=POKEY2+0B
SKCTL2	=POKEY2+0F

CPEXPL:	LDA I,SIDDI		;PLAYER DIES
	JMP SNDON
 -> 
CCB0 A9:5F      LDA:imm    #5F
CCB2 4C:C3 CC   JMP:abs    $CCC3



;EXPLOSION SOUND
;
T51F:	.BYTE 0C0,8,4,10
	.BYTE 0,0
T51A:	.BYTE 0A6,20,0F8,4
	.BYTE 0,0
T52F:	.BYTE 40,8,4,10
	.BYTE 0,0
T52A:	.BYTE 0A6,20,0FE,4
	.BYTE 0,0

CBD1 C0:08      CPY:imm    #08
CBD3 04:        Illegal Opcode
CBD4 10:00      BPL:rel    Branch->$CBD6
CBD6 00:        BRK:imp    BREAK
CBD7 A6:20      LDX:zp     Zp RAM 0020
CBD9 F8:        SED:imp    Set Decimal
CBDA 04:        Illegal Opcode
CBDB 00:        BRK:imp    BREAK
CBDC 00:        BRK:imp    BREAK
CBDD 40:        RTI:imp    Ret from Int
CBDE 08:        PHP:imp    Push P
CBDF 04:        Illegal Opcode
CBE0 10:00      BPL:rel    Branch->$CBE2
CBE2 00:        BRK:imp    BREAK
CBE3 A6:20      LDX:zp     Zp RAM 0020
CBE5 FE:04 00   INC:abs,x  $0004,X
CBE8 00:        BRK:imp    BREAK



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
  int smallwindow = 1;
  int use_pixmap = 1;
  int line_width = 0;
  ProgName = "tempest"; // defined in display.h

  init_graphics (smallwindow, use_pixmap, line_width, game_name (game));
  v_setCustomClipping(1, -16000, -18000, 14500, 18000);

  // special vectrex  interface settings for Bzone
  v_setRefresh(40);
  v_setClientHz(60); // this should be 62.5 
  v_setupIRQHandling();
  v_enableSoundOut(1);
  v_enableButtons(1);
  v_enableJoystickAnalog(1,1,0,0);    
  
  keepDotsTogether = 1;

  
  // see if there is a directory "roms"
  mem = (elem *) gameMemory;
  game = pick_game (SINGLE_GAME);

 // srand(getpid());
  setup_game();

  save_PC = (memrd(0xfffd,0,0) << 8) | memrd(0xfffc,0,0);
  save_A = 0;
  save_X = 0;
  save_Y = 0;
  save_flags = 0;
  save_totcycles = 0;
        irq_cycle = 4173;
  sim_6502 ();
  while (1)
  {
    ; // do not "return" - never call _libc_fini_array etc...
  }
}
