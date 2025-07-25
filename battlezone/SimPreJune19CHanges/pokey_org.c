/*
 * pokey.c: POKEY chip simulation functions
 *
 * Copyright 1991, 1992, 1993, 1996 Hedley Rainnie and Eric Smith
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
 * $Id: pokey.c 2 2003-08-20 01:51:05Z eric $
 */

#include <stdlib.h>
#include <stdio.h>

#include "memory.h"
#include "pokey.h"


#define MAX_REG 16


/* read registers */
#define POT0 0x0
#define POT1 0x1
#define POT2 0x2
#define POT3 0x3
#define POT4 0x4
#define POT5 0x5
#define POT6 0x6
#define POT7 0x7
#define ALLPOT 0x8
#define KBCODE 0x9
#define RANDOM 0xa
#define IRQSTAT 0xe
#define SKSTAT 0xf

/* write registers */
#define AUDF1 0x0
#define AUDC1 0x1
#define AUDF2 0x2
#define AUDC2 0x3
#define AUDF3 0x4
#define AUDC3 0x5
#define AUDF4 0x6
#define AUDC4 0x7
#define AUDCTL 0x8
#define STIMER 0x9
#define SKRES 0xa
#define POTGO 0xb
#define SEROUT 0xd
#define IRQEN 0xe
#define SKCTL 0xf


char *pokey_rreg_name [] =
{
  "POT0", "POT1", "POT2", "POT3", 
  "POT4", "POT5", "POT6", "POT7", 
  "ALLPOT", "KBCODE", "RANDOM", "unused0xB", 
  "unused0xC", "unused0xD", "IRQSTAT", "SKSTAT"
};

char *pokey_wreg_name [] =
{
  "AUDF1", "AUDC1", "AUDF2", "AUDC2",
  "AUDF3", "AUDC3", "AUDF4", "AUDC4", 
  "AUDCTL", "STIMER", "SKRES", "POTGO",
  "unused0xC", "SEROUT", "IRQEN", "SKCTL" 
};


byte pokey_rreg [MAX_POKEY][MAX_REG];

byte pokey_wreg [MAX_POKEY][MAX_REG];

#ifdef POKEY_DEBUG
byte pokey_wreg_inited [MAX_POKEY][MAX_REG] = { { 0 } };
#endif


byte pokey_read (int pokeynum, int reg, int PC, unsigned long cyc)
{
  switch (reg)
    {
    case RANDOM:
      if ((pokey_wreg [pokeynum] [SKCTL] & 0x03) != 0x00)
    pokey_rreg [pokeynum] [RANDOM] = (rand () >> 12) & 0xff;
      return (pokey_rreg [pokeynum] [RANDOM]);
    default:
#ifdef POKEY_DEBUG
      printf ("pokey %d read reg %1x (%s)\n", pokeynum, reg, pokey_rreg_name [reg]);
#endif
      return (pokey_rreg [pokeynum] [RANDOM]);
    }
}

void pokey_write (int pokeynum, int reg, byte val, int PC, unsigned long cyc)
{
#ifdef POKEY_DEBUG
  if (! pokey_wreg_inited [pokeynum] [reg])
    {
      pokey_wreg_inited [pokeynum] [reg] = 1;
      pokey_wreg [pokeynum] [reg] = val + 1;  /* make sure we log it */
    }
  if (pokey_wreg [pokeynum] [reg] != val)
    {
      printf ("pokey %d reg %1x (%s) write data %02x\n", pokeynum, reg, pokey_wreg_name [reg], val);
    }
#endif
  pokey_wreg [pokeynum] [reg] = val;
}

