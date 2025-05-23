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

int emulating = 1;
int useSamples=1;
int debounce=0;
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

unsigned char Thrust_A_data[]=
{
    0x78, 0x1F, 0x00, 0x00, 0x17, 0x16, 0xD0, 0x20, 
};
unsigned char Thrust_B_data[]=
{
    0xEB, 0xE7, 0x0B, 0x00, 0x8D, 0x8F, 0xD0, 0x20, 
};
unsigned char Thrust_C_data[]=
{
    0xEF, 0x4D, 0x0D, 0x00, 0x8D, 0x8B, 0xD0, 0x20, 
};

unsigned char Explosion_data[]=
{
    0x7E, 0x00, 0x00, 0x10, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 
    0x1E, 0x1E, 0x1E, 0x5D, 0x12, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 
    0x1D, 0x1D, 0x1D, 0x1D, 0x5C, 0x14, 0x1C, 0x1C, 0x1C, 0x1C, 
    0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x5E, 0x10, 0x1E, 0x1E, 0x1E, 
    0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x5E, 0x12, 0x1E, 0x1E, 
    0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x5E, 0x14, 0x1E, 
    0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x5E, 0x16, 
    0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1D, 0x1D, 
    0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x5D, 0x1A, 
    0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x5C, 
    0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 
    0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 
    0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 
    0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 
    0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 
    0x59, 0x1E, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 
    0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 
    0x19, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
    0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
    0x18, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 
    0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 
    0x17, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 
    0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 
    0x16, 0xD0, 0x20, 
};

unsigned char Fuel_data[]=
{
    0xEF, 0x48, 0x00, 0x0C, 0xAF, 0x36, 0x00, 0xAF, 0x29, 0x00, 
    0xAF, 0x1F, 0x00, 0xD0, 0x20, 
};

unsigned char Slam_data[]=
{
    0xEF, 0xC0, 0x00, 0x0C, 0xAF, 0x60, 0x00, 0xAF, 0xC0, 0x00, 
    0xAF, 0x60, 0x00, 0xAF, 0x30, 0x00, 0xAF, 0x18, 0x00, 0xD0, 
    0x20, 
};

extern void (*gameCallback)(int);
int oldDiscreteSound = 0;
static inline void handleDiscretSound()
{
//  { 0x3C00,      1,      WR, LUNAR_SND },
//    m_discrete->write(LLANDER_THRUST_DATA, data & 0x07);      /* Thrust volume */
//    m_discrete->write(LLANDER_TONE3K_EN, data & 0x10);        /* Tone 3KHz enable */
//    m_discrete->write(LLANDER_TONE6K_EN, data & 0x20);        /* Tone 6KHz enable */
//    m_discrete->write(LLANDER_EXPLOD_EN, data & 0x08);        /* Explosion */  
  static int explosionTriggered = 0;
  unsigned char currentDiscreteSound = mem[0x3C00].cell;
  signed char *currentPlaying = (signed char *) v_getSamplePlaying();
  int otherSamplePlaying=((currentPlaying!=0) && (currentPlaying != thrustSamplePlay));
    
  if ((currentDiscreteSound&0x3f) == 0) // no sound playing
  {
    explosionTriggered = 0;
    v_stopSamplePlaying();
    v_playSFXStopForced(0);
    v_playSFXStopForced(1);
    v_playSFXStopForced(2);
    currentDiscreteSound=0;
    oldDiscreteSound = currentDiscreteSound; 
    return;
  }
  else if ((currentDiscreteSound&0x08) == 0x08) // explosion
  {
    if (useSamples==0)
    {
      if ((!v_isPlayingSFX(2)) && (explosionTriggered==0))
      {
        explosionTriggered = 1;
        v_playSFXStopForced(0);
        v_playSFXStopForced(1);
        v_playSFXStopForced(2);
        v_stopSamplePlaying();
        v_playSFXCont(Explosion_data, 2, 0);
      }      
    }    
    else if ((currentPlaying != explosionSample) && (explosionTriggered==0))
    {
      explosionTriggered = 1;
      v_playSFXStopForced(0);
      v_playSFXStopForced(1);
      v_playSFXStopForced(2);
      v_stopSamplePlaying();
      v_playIRQSample(explosionSample, explosionSize, explosionRate, PLAY_ONCE);
    }
    oldDiscreteSound = 0x08; 
    return;
  }
  else if ((currentDiscreteSound&0x10) == 0x10) // BEEP -> Fuel
  {
    if (useSamples==0)
    {
      v_playSFXCont(Fuel_data, 1, 0);
      v_playSFXLoudness(1,0.6);
    }    
    else if (currentPlaying != tone3khzSample)
    {
      v_stopSamplePlaying();
      v_playIRQSample(tone3khzSample, tone3khzSize, tone3khzRate, PLAY_ONCE);
    }
    oldDiscreteSound = 0x10; 
    return;
  }
  else if ((currentDiscreteSound&0x20) == 0x20) // BEEP -> SLAM
  {
    if (useSamples==0)
    {
      v_playSFXCont(Slam_data, 1, 0);
    }    
    
    else if (currentPlaying != tone3khzSample)
    {
      v_stopSamplePlaying();
      v_playIRQSample(tone3khzSample, tone3khzSize, tone3khzRate*2, PLAY_ONCE);
      mem[0x3C00].cell = mem[0x3C00].cell&(0xff-0x20);
    }
    oldDiscreteSound = 0x20; 
    return;
  }
  else if ((currentDiscreteSound&0xf) != (oldDiscreteSound &0xf)) // Thurst to varying degrees
  {
    int index = ((currentDiscreteSound&0x7)-1)+3;
    index = joystick.y/12;
    float muls[11] = {0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2};
    float fullMul = muls[index];
    
    if (useSamples==0)
    {
      if (!v_isPlayingSFXSound(0,Explosion_data))
      {
        if (!v_isPlayingSFX(1))
        {
          v_playSFXCont(Thrust_B_data, 1, PLAY_LOOP);
          v_playSFXLoudness(1,fullMul);
        }
        else
        {
            if (v_isPlayingSFXSound(1, Thrust_B_data))
              v_playSFXLoudness(1,fullMul);
        }

        v_playSFXCont(Thrust_A_data, 0, PLAY_LOOP);
        v_playSFXCont(Thrust_C_data, 2, PLAY_LOOP);
        v_playSFXLoudness(0,fullMul);
        v_playSFXLoudness(2,fullMul);
      }
    }
    else
    {
      // between 0-7
      //printf("Adjust Thrust Volumne to: %i!\n", ((currentDiscreteSound&0x7)-1));
      for (int i=0;i<thrustSize;i++)
      {
        int s = (thrustSample[i]*fullMul);
        if (s>127) s = 127;
        if (s<-128) s = -128;
        thrustSamplePlay[i] = (signed char) s;
      }
    }
    
    if (currentPlaying != thrustSamplePlay)
    {
      explosionTriggered=0;
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
int angleOptimization(VectorPipelineBase *pip, int minAngle);
int overlappingOptimization(VectorPipelineBase *base, VectorPipelineBase *pip);

/*
There are "slightly" to many vectors drawn in Lunar Lander - and for the Vectrex I optimize a little.
In general to acchieve more "stability" on screen, following for doucmention purposes...

Strategie for "optimization".

To check which part of the screen is drawn - we count vectors and watch moves and positions.
- 1. drawn is "normal" Text (data on top)
- 2. drawn is landscape
- 3. drawn are stars
- 4. drawn is lander and propulsion
- 5. drawn are landing sites
- 6. drawn is "crash" text and the following message

Some of these stages are tracked.
Linecount is tracked, the linecount is set to zero by a callBack from "openpage"
Variable "stage":
stage = 0; initital state
stage = 1; is reached the first time a vector is drawn below Y = -2000 - it is assumed the landscape starts
stage = 2; is reached when the landscape is finished, the landscape is drawn in one huge vectorlist. The first time a move is found -> the landscape is done.
           we take this as an indicator.

a) General reduction:
  If landscape is drawn reduce vectors that have an angle to each other smaller 14, if the vector is NOT horizontal (potential landing site).
  This is only done when the full landscape is drawn (not zoomed in).
  Indicator is flag "lunarState".
  This also only happens when in game, Indicator is flag "inGame".
  Lunar Lander Ram:
    lunarState = LUNARNUM zero page address: 0x4e, if == 0x40, than major landscape is displaying, other wise it is zooming in 
    blinkState = FRAME zero page address:    0x86, if == 0x10, than blink state is "ON", otherwise "OFF", this is just a frame counter, which is checked for bit 4
    inGame     = GAMODE zero page address:   0x22, if == 0x40, than a "normal" game is ongoing (otherwise attract mode, or game over...)  
  
b) In game, during landscape drawing - complete landscape (lunarState==1) we insert a zero every 14 vectors to ensure a stable draw.
   We do not do this in "NOT" game mode (attract-mode) since upon scrolling the zero point would change and it would occur "unsmooth".
   If we zoom in (lunarState==0), we insert a Zero prior any horizontal line, so that the landing place will be drawn very accurately.
   (This results usually in a small gap - that can't be helped)
   
c) In stage 2 (finished drawing the landscape)
   We leave out 1/3 of the stars (easily recognized, as these are "dots")
   The first vector after "dots" is the start of drawing the lander.
   The lander is always 13 vectors, but there might be propulsions - these enlarge the vector list.
   The indication that lander drawing is done is:
   - landerDrawn count > 12
   - a "large" move was found (the next drawing after lander/propulsion, will be landing platforms)
     If the lander is very NEAR a landing site - this check may fail!
     
d) The blinking landing sites "shrink" and "expand" the vectrex screen, this is ugly and I wanted to prevent that.
   The thing is in the "dark" phase the vectors are not drawn at all. Displaying/not displaying 40-50 vectors every 16 cycles or not
   changes the screen dynamic and the vectrex "shakes" a little.
   Thus I want to display these vectors "always" but with less intensity.
   For that I hat to change the ROM code:
   i)
    mem[0x6854].cell = 0xea;
    mem[0x6855].cell = 0xea;
    Insert 2 NOP instead of a branch (to not display)
   ii)
    mem[0x6851].cell = 0x1f; // not NO intensity, but 1 (change from 0x0f to 0x1f, upper nibble is intensity)

   With these changes the site vectors are always displayed, but with very little intensity.
   Well... nearly.
   This is only true for "vectors" - meaning the one horitzontal landing vector.
   The "x2, x3, x4, x5" are text displays.
   The ROM routines of Lunar Lander display text always with an intensity of 70. Hardcoded.
   But - by now I have all that I need.
   I know when all other drawings are done, I know when the landing sites start.
   I (litteraly) double check though.
   The landing sites are always vectors, which "overlap" a vector in the landscape.
   If I find a horizontal vector after drawing the lunar lander, it is a possible indicator for a landing site.
   (except, when the lander is flying upright, than the lander can have horizontal vectors)
   BUT - If I find a vector that completely overlaps the current horizontal vector (prior in the vectorlist of all drawn vectors) 
   - than I can be sure it is the start of a landing site. I remember this in tha variable "doubleFound".
   If blink state is 0 - than all following vectors (the multiplyer texts) - are drawn with very low intensity (10).
   ... and thus the screen shakes less...
   
   
   

*/
int checkForSpecialVectors(VectorPipelineBase ** cpbPointer,VectorPipeline **plPointer, int *fpc, int *cx, int *cy) 
{
  if (!emulating) return 0;
  VectorPipeline *pl=*plPointer;
  VectorPipelineBase *cpb = *cpbPointer;
  static int state = 0;
  static int y=20000;
  static int x=20000;
  static int landscapeCount = 0;
  static int starCount = 0;
  static int doubleFound = 0;
  static int lunarState = 1; // displaying major landscape
  static int blinkState = 0;
  static int inGame=0;
  static int landerDrawn = 0;
  static int largeMoveFound = 0;
  
  signed int x0,x1,y0,y1;
  if ((orientation&1)==0)
  {
    x0 = cpb->x0;
    x1 = cpb->x1;
    y0 = cpb->y0;
    y1 = cpb->y1;
  }
  else
  {
    x0 = cpb->y0;
    x1 = cpb->y1;
    y0 = cpb->x0;
    y1 = cpb->x1;
  }
 
  
  if (lineCount==0) 
  {
    largeMoveFound = 0;
    landscapeCount = 0;
    starCount = 0;
    doubleFound = 0;
    landerDrawn = 0;
    state=0;
    y=20000;
    x=20000;
    lunarState = (MEMRD(0x4e, 0,0) & 64) == 64;     // "LUNARNUM" if true displaying major landscape, false -> zoom in
    blinkState = (MEMRD(0x86, 0,0) & 0x10) == 0x10; // "FRAME", 0 = off, 1 = on
    inGame = (MEMRD(0x22, 0,0) & 0x40) == 0x40;     // "GAMODE"
  }
  lineCount++;
  
  if (state == 0) // searching fpr landscapte draw state 0 looking for move y>  10000 to y <-2000
  {
    if (y0<-2000) 
    {
      state =1;
    }
    y=y1;
    return 0;
  }
  else if (state == 1) // state=1 we are drawing the landscape!
  {
    if (y!=y0) // old y != new y -> there was a move! -> not in landscape draw anymore
    {
      state = 2;
      return 0;
    }
    y=y1;
    landscapeCount++;
    // we are drawing the moon landscape

    if (inGame)
    {
      if (lunarState==0)
      {
        // this resets to zero upon every horizontal line in the landscape, about 20 times
        // this "resets" about 20 locations - instead of only 4
        // but I can'T think of a strategy to reduce this!    

        // but lunarState has much less horizontal lines...
        if ((y0 == y1))
        {
          // on nearly every horizontal line we force a ZERO
          int width = abs(x1-x0);
          if (((y0<2000) && (width>130)) ||(width>1000))
          {
            cpb->force = cpb->force | PL_BASE_FORCE_ZERO;
            return 0;
          }
        }
      }
      else
      {
        // enter every xth vector in the landscape a zero ref...
        if ((landscapeCount % 14) == 0)
        {
            cpb->force = cpb->force | PL_BASE_FORCE_ZERO;
            return 0;
        }
      }
      // we do not optimize away potential landing spots! (horizontal lines)
      if (lunarState==1) 
      {
        // only on major landscape!
        if ((y0 != y1))
        {
          // this removes all vectors that have an angle of below x degrees to each other
          if (angleOptimization(cpb, 14)) 
          {
            // 
            y=y0;
            return CV_SPECIAL_NEXT;
          }
        }
      }
    }

  }
  else if (state == 2) // state=2 landscape is done... we might be drawing stars now
  {
      if ((y0 == y1) &&(x0 == x1))
      {
        starCount++;
        if ((starCount%3)==0)
        {
            return CV_SPECIAL_NEXT;
        }
      }
      else
      {
        if (starCount)
        {
          landerDrawn++;
        }
        if (landerDrawn)
        {
          // I try not to REMOVE the vector in the "dark" blinking stage
          // but to keep them on screen
          // but darker
          // in the hopes the screen does not "shrink"/"expand" so much...

          // not a star, either the lander or a landing spot
          if (!blinkState)
          {
            if (inGame)
            {
              // no display
              if (!doubleFound)
              {
                // from vectrexInterface_pipeline.c
                int isPreviousOverlapping(VectorPipelineBase *base);
                // search for double
                if (landerDrawn>12)
                {
                  if (!largeMoveFound)
                  {
                    if ((abs(y-y0)>1000) || (abs(x-x0)>1000))
                      largeMoveFound = 1;
                  }
                  if (largeMoveFound)
                  {
                    
                    doubleFound = isPreviousOverlapping(cpb);
                  }

                }
              }
              if (doubleFound)
              {
                // change brightness
                cpb->intensity = 10;
              }
            }
          }
        }
      }
      y=y0;
      x=x0;

  }
/*  

  if in game
    if passed landscape
        if blink = 0
          if double vector found
          draw everything following DARK
*/  
  
  
  return 0;
}

unsigned char dipLanguageSetting = 0;   // english
unsigned char dipFuelSetting = 3;       // 900
unsigned char dipOtherSetting = 1;      // coin
unsigned char dipSoundSetting = 1;         // samples
unsigned char dipCreditSetting = 0;         // coin value
DipSwitchSetting dipCredit = 
{
  "CREDIT RIGHT COIN",
  &dipCreditSetting,
  { 
    {"1 CREDIT", 0}, 
    {"4 CREDITS", 1}, 
    {"5 CREDITS", 2}, 
    {"6 CREDITS", 3}, 
    {0,0}
  }
};

DipSwitchSetting dipSound = 
{
  "SOUND",
  &dipSoundSetting,
  { 
    {"PSG", 0}, 
    {"SAMPLES", 1}, 
    {0,0}
  }
};
DipSwitchSetting dipOther = 
{
  "OTHER",
  &dipOtherSetting,
  { 
    {"FREEPLAY", 0}, 
    {"COINPLAY", 1}, 
    {0,0}
  }
};

DipSwitchSetting dipFuel = 
{
  "FUEL",
  &dipFuelSetting,
  { 
    {"450", 0},
    {"600", 1}, 
    {"750", 2},
    {"900", 3}, 
    {"1100", 4},
    {"1300", 5}, 
    {"1550", 6},
    {"1800", 7}, 
    {0,0}
  }
};
DipSwitchSetting dipLanguage = 
{
  "LANGUAGE",
  &dipLanguageSetting,
  { 
    {"ENGLISH", 0}, 
    {"FRENCH",  1},
    {"SPANISH", 2},
    {"GERMAN",  3},
    {0,0}
  }
};

DipSwitchSetting *lunarSwitches[] = 
{
  &dipLanguage,
  &dipFuel,
  &dipCredit,
  &dipOther,
  &dipSound,
  0
};


void applyDips()
{
/*
INVERS 0 and 1!!!
    8 SWITCH DIP (P8) with -02 ROMs on PCB
    87654321
    --------
    11x1xxxx   450 fuel units per coin
    10x1xxxx   600 fuel units per coin
    01x1xxxx   750 fuel units per coin  $
    00x1xxxx   900 fuel units per coin
    11x0xxxx   1100 fuel units per coin
    10x0xxxx   1300 fuel units per coin
    01x0xxxx   1550 fuel units per coin
    00x0xxxx   1800 fuel units per coin
    xx0xxxxx   Free play
    xx1xxxxx   Coined play as determined by toggles 5, 7, & 8  $
    xxxx00xx   German instructions
    xxxx01xx   Spanish instructions
    xxxx10xx   French instructions
    xxxx11xx   English instructions  $
    xxxxxx11   Right coin == 1 credit/coin  $
    xxxxxx10   Right coin == 4 credit/coin
    xxxxxx01   Right coin == 5 credit/coin
    xxxxxx00   Right coin == 6 credit/coin
               (Left coin always registers 1 credit/coin)
* 
*/

  // These can be written "directly", since they go to different memory locations
  // beware other machines, where settings might need "OR" or "AND" 
  unsigned char dipSetting = 0;
  
  if (dipCreditSetting==0)      {dipSetting=dipSetting|(0b00);}// 1 Credit
  if (dipCreditSetting==1)      {dipSetting=dipSetting|(0b01);}// 4 Credit
  if (dipCreditSetting==2)      {dipSetting=dipSetting|(0b10);}// 5 Credit
  if (dipCreditSetting==3)      {dipSetting=dipSetting|(0b11);}// 6 Credit

  if (dipLanguageSetting==0)  {dipSetting=dipSetting|(0b0000);}// ENGLISH
  if (dipLanguageSetting==1)  {dipSetting=dipSetting|(0b0100);}// FRENCH
  if (dipLanguageSetting==2)  {dipSetting=dipSetting|(0b1000);}// SPANISH
  if (dipLanguageSetting==3)  {dipSetting=dipSetting|(0b1100);}// GERMAN
  MEMWR(0x21, dipLanguageSetting, 0,0); //LANG
  
  if (dipOtherSetting==0) {dipSetting=dipSetting|(0b00100000);}// FREEPLAY
  if (dipOtherSetting==1) {dipSetting=dipSetting|(0b00000000);}// COIN

  if (dipFuelSetting==0)  {dipSetting=dipSetting|(0b00000000);}// 400
  if (dipFuelSetting==1)  {dipSetting=dipSetting|(0b01000000);}// 600
  if (dipFuelSetting==2)  {dipSetting=dipSetting|(0b10000000);}// 750
  if (dipFuelSetting==3)  {dipSetting=dipSetting|(0b11000000);}// 900
  if (dipFuelSetting==4)  {dipSetting=dipSetting|(0b00010000);}// 1100
  if (dipFuelSetting==5)  {dipSetting=dipSetting|(0b01010000);}// 1300
  if (dipFuelSetting==6)  {dipSetting=dipSetting|(0b10010000);}// 1550
  if (dipFuelSetting==7)  {dipSetting=dipSetting|(0b11010000);}// 1800
    
  useSamples=dipSoundSetting;
  optionreg [0] = dipSetting; 
}

signed char enterDip=0;
int handleInput(void) 
{
    if (enterDip)
    {
      emulating = 0;
      v_getDipSettings(lunarSwitches, "LUNAR DIP SWITCHES");
      emulating = 1;
      applyDips();
      enterDip = 0;
      debounce = 20;
    }
}
extern void (*customInputHandler) (void);
void lunarInputHandler()
{
  #define JOYSTICK_CENTER_MARGIN 20
  //  if (uses_analog_joystick(game))
  joystick.x = currentJoy1X;  // 0x80   0  0x7f
  if ((currentJoy1X>0) && (currentJoy1X<JOYSTICK_CENTER_MARGIN)) joystick.x= 0;
  if ((currentJoy1X<0) && (currentJoy1X>-JOYSTICK_CENTER_MARGIN)) joystick.x=  0;
  joystick.y = currentJoy1Y;  // 0x80   0  0x7f
  if ((currentJoy1Y>0) && (currentJoy1Y<JOYSTICK_CENTER_MARGIN)) joystick.y=  0;
  if ((currentJoy1Y<0) && (currentJoy1Y>-JOYSTICK_CENTER_MARGIN)) joystick.y=  0;

  switches [0].left = currentJoy1X<-JOYSTICK_CENTER_MARGIN;
  switches [0].right = currentJoy1X>JOYSTICK_CENTER_MARGIN;
  if (((signed char)joystick.y)<0) joystick.y = 0;

  int buttonDone = 0;
  if (debounce >0)
  {
    debounce--;
    return;
  }
  
  // it is IMPORTANT
  // to hit and relase after a time
  // since Lunar Lander "samples" the button state for five rounds
  // only if button is pressed long enough
  // the coin counter will accept it
  if ((!buttonDone) && ((currentButtonState & 0x01) && (currentButtonState & 0x08))) // Button 1+4 -> enter Dip switch settings
  {
    v_stopSamplePlaying();
    v_playSFXStopForced(0);
    v_playSFXStopForced(1);
    v_playSFXStopForced(2);
    enterDip = 1;
    buttonDone = 1;
  }
  if ((!buttonDone) && ((currentButtonState & 0x0a) == 0x0a)) // button 2+4 together -> SELF TEST - > CRASH? -> disabled!
  {
//    printf("SELF TEST\n");
    // self_test=1;
    buttonDone = 1;
  }
  else
  {
    self_test=0;
  }
  
  if ((!buttonDone) && ((currentButtonState & 0x0c) == 0x0c)) // button 3+4 together -> SLAM
  {
//    printf("SLAM\n");
    slam=1;
    buttonDone = 1;
  }
  else
  {
    slam=0;
  }
  if ((!buttonDone) && ((currentButtonState & 0x02) == 0x02)) // button 2 ABORT / START 2
  {
//    printf("ABORT\n");
    switches[0].abort=1;
    start2 = 1;
    buttonDone = 1;
  }
  else
  {
    switches[0].abort=0;
    start2 = 0;
  }
  if ((!buttonDone) && ((currentButtonState & 0x08) == 0x08)) // button 4 COIN LEFT
  {
    cslot_left=1;
    buttonDone = 1;
  }
  else
  {
    cslot_left=0;
  }
  if ((!buttonDone) && ((currentButtonState & 0x04) == 0x04)) // button 3 COIN RIGHT
  {
    cslot_right=1;
    buttonDone = 1;
  }
  else
  {
    cslot_right=0;
  }
  if ((!buttonDone) && ((currentButtonState & 0x01) == 0x01)) // button 1 START 1
  {
    start1 = 1;
    buttonDone = 1;
  }
  else
  {
    start1 = 0;
  }
  
  handleInput(); // dipswitches
}


int main(int argc, char *argv[])
{
  vectrexinit(1);
  v_init();
  v_noSound();


  int smallwindow = 1;
  int use_pixmap = 1;
  int line_width = 0;
  ProgName = "lunar"; // defined in display.h

  setDimensions(500, 250, 35, 50);

  // within "init" normal ini files are read
  // sim.ini and also lunar.ini
  init_graphics (smallwindow, use_pixmap, line_width, game_name (game));



  v_setCustomClipping(1, -17000, -22500, 16000, 17000);
  v_setRefresh(50);
  v_setClientHz(41); // this s
  v_setupIRQHandling();
  v_enableSoundOut(1);
  v_enableButtons(1);
  useDoubleTimer = 1;
  v_enableJoystickAnalog(1,1,0,0);
  
  thrustSize = v_loadRAW("samples/lunar/Thrust.raw", thrustSample);
  tone3khzSize = v_loadRAW("samples/lunar/BEEP3khz.raw", tone3khzSample);
  tone6khzSize = v_loadRAW("samples/lunar/BEEP6khz.raw", tone6khzSample);
  explosionSize = v_loadRAW("samples/lunar/EXPLODE1.raw", explosionSample);
  
  
  mem = (elem *) gameMemory;
  game = pick_game (SINGLE_GAME);

 // srand(getpid());
  checkExternal = checkForSpecialVectors;
  customInputHandler = lunarInputHandler;
  gameCallback = lunarCallback;
  setup_game ();
  applyDips();

  // todo 
  // insert nops in SITES
  // again, and print sites with low intensity
  // to compensate the little screen wobble
  save_PC = (memrd(0xfffd,0,0) << 8) | memrd(0xfffc,0,0);
  printf("Lunar Start: %04x\n",save_PC);
  save_A = 0;
  save_X = 0;
  save_Y = 0;
  save_flags = 0;
  save_totcycles = 0;
  irq_cycle = 6136;//8192;

  // hack to display landing site in 
  // dark intensity
  // instead of not drawing them at all!
  mem[0x6854].cell = 0xea;
  mem[0x6855].cell = 0xea;
  //-> 6854: 10 1A bpl $6870;ALWAYS
  //-> 6854: EA    NOP
  //-> 6855: EA    NOP

  //6850: 29 0F    and #$0f
  mem[0x6851].cell = 0x1f; // not NO intensity, but 1


//VGBRIT -> 0x8f, brightness 

  sim_6502 ();
  while (1)
  {
    ; // do not "return" - never call _libc_fini_array etc...
  }
}
