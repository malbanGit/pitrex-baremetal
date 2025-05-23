// Source code available now at: 
// https://github.com/historicalsource/lunar-lander-international/tree/main


/*
wenn der anfang eines Vecktors komplett gleich ist
und der vector horizontal ist,
dann bei dem ersten Vektor vorher einen Reset machen
-> dann kann davor eine kleine Lücke sein - aber die Platt form sollte stimmen

damit das klappt muss die blinkende plattform auch im "aus" zustand gezeichnet werden (mit low light?)
JSR SITES       ;FLASH BONUS SITES

PLYINIT:LDA INTCNT      ;SET UP BONUS SITES
    AND I,3     ;FIRST SITE = 0 TO 3
    STA TABSIT

    
    
    ;
;LANDING SITES [DONE BY OVERLAY ON VECTOR]
;   TABLE OF SITES [TABSIT)
SITES:  LDX I,3
SITELP: STX EXP     ;DISPLAY BONUS SITES

...?
                ;MAKE LANDING SITE VCTR ZERO INTENSITY
    LDY I,3     ;TOP NIBBLE OF THIRD BYTE = VCTR INTENSITY
    LDA NY,POINTR
    AND     I,0F
    STA NY,POINTR
    BPL 40$     ;ALWAYS NOT jump? then vector with 0 intensity?
30$:    LDA I,0

must find that in "bin" rom




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

int useSamples=1;

signed char thrustSample[1000];
signed char thrustSamplePlay[1000];
int thrustSize;
int thrustRate = 13000;

signed char tone3khzSample[1000];
int tone3khzSize;
int tone3khzRate = 1000;

signed char tone6khzSample[1000];
int tone6khzSize;
int tone6khzRate = 1000;

signed char explosionSample[10000];
int explosionSize;
int explosionRate = 12000;


char gameMemory[4*65536];
void setDimensions(int offsetx, int  offsety, int mulx, int muly);

extern void (*gameCallback)(int);
int oldDiscreteSound = 0;
static inline void handleDiscretSound()
{
  static int explosionTriggered = 0;
  unsigned char currentDiscreteSound = mem[0x3C00].cell;
  signed char *currentPlaying = (signed char *) v_getSamplePlaying();
//  { 0x3C00,      1,      WR, LUNAR_SND },
//    m_discrete->write(LLANDER_THRUST_DATA, data & 0x07);      /* Thrust volume */
//    m_discrete->write(LLANDER_TONE3K_EN, data & 0x10);        /* Tone 3KHz enable */
//    m_discrete->write(LLANDER_TONE6K_EN, data & 0x20);        /* Tone 6KHz enable */
//    m_discrete->write(LLANDER_EXPLOD_EN, data & 0x08);        /* Explosion */


  /*
unsigned char thrustSample[1000];
unsigned char thrustSamplePlay[1000];
int thrustSize;
int thrustRate = 10000;

unsigned char tone3khzSample[1000];
int tone3khzSize;
int tone3khzRate = 1000;

unsigned char tone6khzSample[1000];
int tone6khzSize;
int tone6khzRate = 1000;

unsigned char explosionSample[10000];
int explosionSize;
int explosionRate = 10000;


*/


  if ((currentDiscreteSound&0x3f) == 0)
  {
    if (currentPlaying!=0)
    {
//      printf("Stop Discrete Sound!\n");
      explosionTriggered = 0;
      v_stopSamplePlaying();
    }
    currentDiscreteSound=0;
    oldDiscreteSound = currentDiscreteSound; 
    return;
  }
  
  else if ((currentDiscreteSound&0x08) == 0x08)
  {
    if ((currentPlaying != explosionSample) && (explosionTriggered==0))
    {
//printf("Start Explosion!\n");
      explosionTriggered = 1;
      v_stopSamplePlaying();
      v_playIRQSample(explosionSample, explosionSize, explosionRate, PLAY_ONCE);
    }
    oldDiscreteSound = 0x08; 
    return;
  }
  else if ((currentDiscreteSound&0x10) == 0x10)
  {
    if (currentPlaying != tone3khzSample)
    {
//printf("Start 3khz!\n");
      v_stopSamplePlaying();
      v_playIRQSample(tone3khzSample, tone3khzSize, tone3khzRate, PLAY_ONCE);
    }
    oldDiscreteSound = 0x10; 
    return;
  }
  else if ((currentDiscreteSound&0x20) == 0x20)
  {
    if (currentPlaying != tone3khzSample)
    {
// SLAM      
//printf("Start 6khz!\n");
      v_stopSamplePlaying();
      v_playIRQSample(tone3khzSample, tone3khzSize, tone3khzRate*2, PLAY_ONCE);
      mem[0x3C00].cell = mem[0x3C00].cell&(0xff-0x20);
    }
    oldDiscreteSound = 0x20; 
    return;
  }
  else if ((currentDiscreteSound&0xf) != (oldDiscreteSound &0xf))
  {
    // between 0-7
//printf("Adjust Thrust Volumne to: %i!\n", ((currentDiscreteSound&0x7)-1));
    for (int i=0;i<thrustSize;i++)
    {
      float muls[8] = {0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.2, 1.2};
      float secondmul = 0.1;
//      if (i==0) secondmul = 0;
//      if (i>=4) secondmul = 0.5;
      
      int s = (thrustSample[i]*(secondmul+muls[((currentDiscreteSound&0x7)-1)]));
      if (s>127) s = 127;
      if (s<-128) s = -128;
      thrustSamplePlay[i] = (signed char) s;
    }
    
    
    if (currentPlaying != thrustSamplePlay)
    {
      explosionTriggered=0;
//printf("Start Thrust\n");
      v_stopSamplePlaying();
      v_playIRQSample(thrustSamplePlay, thrustSize, thrustRate, PLAY_LOOP);
    }
    
    oldDiscreteSound = currentDiscreteSound; 
    return;
  }
}


int lineCount = 0;
void lunarCallback(int type)
{
  if (type == 999) 
  {
    // open page
//printf("linecount==0\n");
    lineCount = 0;
    return;
  }
  if (type == 1) // LUNAR_SND
  {
//  { 0x3C00,      1,      WR, LUNAR_SND },
//    m_discrete->write(LLANDER_THRUST_DATA, data & 0x07);      /* Thrust volume */
//    m_discrete->write(LLANDER_TONE3K_EN, data & 0x10);        /* Tone 3KHz enable */
//    m_discrete->write(LLANDER_TONE6K_EN, data & 0x20);        /* Tone 6KHz enable */
//    m_discrete->write(LLANDER_EXPLOD_EN, data & 0x08);        /* Explosion */
    handleDiscretSound();
    return;
  }
  if (type == 2) // LUNAR_SND_RST
  {
    // { 0x3E00,      1,      WR, LUNAR_SND_RST },
    oldDiscreteSound = 0;
    v_stopSamplePlaying();
    return;
  }


}


int checkForSpecialVectors(VectorPipelineBase ** cpbPointer,VectorPipeline **plPointer, int *fpc, int *cx, int *cy) 
{
  VectorPipeline *pl=*plPointer;
  VectorPipelineBase *cpb = *cpbPointer;
  static int state = 0;
  static int y=20000;
  if (lineCount==0) 
  {
//printf("state=0\n");
    state=0;
    y=20000;
  }
  lineCount++;
  // state 0 looking for move y>  10000 to y <-2000
  if (state == 0)
  {
    if (cpb->y0<-2000) 
    {
//printf("state=1\n");
      state =1;
    }
    y=cpb->y1;
    return 0;
  }
  else if (state == 1)
  {
    if (y!=cpb->y0) // old y != new y -> there was a move!
    {
//printf("state=2\n");
      state = 2;
      return 0;
    }
    y=cpb->y1;
//printf(".");
    
    // we are drawing the moon landscape
    
// this "resets" about 20 locations - instead of only 4
// but I can'T think of a strategy to reduce this!    
    if ((cpb->y0 == cpb->y1))
    {
      // on nearly every horizontal line we force a ZERO
      int width = abs(cpb->x1-cpb->x0);
      if (((cpb->y0<0) && (width>130)) ||(width>1000))
      {
        cpb->force = cpb->force | PL_BASE_FORCE_ZERO;
//        printf("Width: %i (y=%i)\n", abs(cpb->x1-cpb->x0),cpb->y0);    
        return 0;
      }
      
    }
  }
  return 0;
}
//first jump from y>  10000 to y <-2000





int main(int argc, char *argv[])
{
  int smallwindow = 1;
  int use_pixmap = 1;
  int line_width = 0;
  ProgName = "lunar"; // defined in display.h

  setDimensions(500, 250, 35, 50);
  
  init_graphics (smallwindow, use_pixmap, line_width, game_name (game));
  v_setCustomClipping(1, -17000, -22000, 17000, 18000);
  // special vectrex  interface settings for Bzone
  v_setRefresh(50);
  v_setClientHz(41); // this s
  v_setupIRQHandling();
  v_enableSoundOut(1);
  v_enableButtons(1);
  useDoubleTimer = 1;
  v_enableJoystickAnalog(1,1,0,0);

  
  
  useSamples = 1;

  thrustSize = v_loadRAW("samples/lunar/Thrust.raw", thrustSample);
  tone3khzSize = v_loadRAW("samples/lunar/BEEP3khz.raw", tone3khzSample);
  tone6khzSize = v_loadRAW("samples/lunar/BEEP6khz.raw", tone6khzSample);
  explosionSize = v_loadRAW("samples/lunar/EXPLODE1.raw", explosionSample);
  
  
  mem = (elem *) gameMemory;
  game = pick_game (SINGLE_GAME);

 // srand(getpid());
  checkExternal = checkForSpecialVectors;

  gameCallback = lunarCallback;
  setup_game ();

  mem[0x6854].cell=0xea; // nop
  mem[0x6855].cell=0xea;
    
  save_PC = (memrd(0xfffd,0,0) << 8) | memrd(0xfffc,0,0);
  save_A = 0;
  save_X = 0;
  save_Y = 0;
  save_flags = 0;
  save_totcycles = 0;
  irq_cycle = 6136;//8192;
  sim_6502 ();
  while (1)
  {
    ; // do not "return" - never call _libc_fini_array etc...
  }
}
