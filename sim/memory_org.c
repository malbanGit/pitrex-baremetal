/*
 * memory.c: memory and I/O functions for Atari Vector game simulator
 *
 * Copyright 1991, 1993, 1996, 2003 Hedley Rainnie and Eric Smith
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
 * $Id: memory.c 2 2003-08-20 01:51:05Z eric $
 */

#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "display.h"
#include "game.h"
#include "sim6502.h"
#include "mathbox.h"
#include "pokey.h"

int breakflag = 0;


byte optionreg [MAX_OPT_REG] = { 0xff, 0xff, 0xff };


int bank = 0; /* RAM bank select */
int self_test = 0;

/* input switch counters */
int cslot_left = 0;
int cslot_right = 0;
int cslot_util = 0;

int slam = 0;
int start1 = 0;
int start2 = 0;

switch_rec switches [2] = {{ 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 }};

joystick_rec joystick = { 0x80, 0x80 };


/*
 * This used to decrement the switch variable if it was non-zero, so that
 * they would automatically release.  This has been changed to increment
 * it if less than zero, so switches set by the debugger will release, but
 * to leave it alone if it is greater than zero, for keyboard handling.
 */
int check_switch_decr (int *sw)
{
  if ((*sw) < 0)
    {
      (*sw)++;
      if ((*sw) == 0)
    printf ("switch released\n");
    }
  return ((*sw) != 0);
}

elem *mem;

byte MEMRD(unsigned addr, int PC, unsigned long cyc)
{
  register byte tag,result=0;

  if(!(tag=mem[addr].tagr)) 
    return(mem[addr].cell);
  
  if(tag & BREAKTAG)
    {
      printf("BreakTag in MemAccess: %04x", addr);
      breakflag = 1;
    }

  switch(tag & 0x3f) 
    {
    case MEMORY:
      result = mem[addr].cell;
      break;
    case COININ:
      result =
    ((! check_switch_decr (& cslot_right))) |
    ((! check_switch_decr (& cslot_left)) << 1) |
        ((! check_switch_decr (& cslot_util)) << 2) |
        ((! check_switch_decr (& slam)) << 3) |
    ((! self_test) << 4) |
    (1 << 5) | /* signature analysis */
    (vg_done (cyc) << 6) |
      /* clock toggles at 3 KHz */
        ((cyc >> 1) & 0x80);

      break;
    case EAROMRD:
      result = 0;
      break;
    case OPTSW1:
      result = optionreg [0];
      break;
    case OPTSW2:
      result = optionreg [1];
      break;
    case OPT1_2BIT:
      result = 0xfc | ((optionreg [0] >> (2 * (3 - (addr & 0x3)))) & 0x3);
      break;
    case POKEY1:
      result = pokey_read (0, addr & 0x0f, PC, cyc);
      break;
    case POKEY2:
      result = pokey_read (1, addr & 0x0f, PC, cyc);
      break;
    case POKEY3:
      result = pokey_read (2, addr & 0x0f, PC, cyc);
      break;
    case POKEY4:
      result = pokey_read (3, addr & 0x0f, PC, cyc);
      break;
    case MBLO:
      result = mb_result & 0xff;
      break;
    case MBHI:
      result = (mb_result >> 8) & 0xff;
      break;
    case MBSTAT:
      result = 0x00;  /* always done! */
      break;
    case GRAVITAR_IN1:
      result =
    ((optionreg [2] & 0x07) << 5) |
    ((! switches [0].thrust) << 4) |
    ((! switches [0].left) << 3) |
    ((! switches [0].right) << 2) |
        ((! switches [0].fire) << 1) |
    ((! switches [0].shield));
      break;
    case GRAVITAR_IN2:
      result =
    0x80 | /* upright cabinet */
    ((! check_switch_decr (& start2)) << 6) |
    ((! check_switch_decr (& start1)) << 5) |
    ((! switches [1].thrust) << 4) |
    ((! switches [1].left) << 3) |
    ((! switches [1].right) << 2) |
        ((! switches [1].fire) << 1) |
    ((! switches [1].shield));
      break;
    case SD_INPUTS:
      switch (addr & 0x07)
    {
    case 0:
      return ((switches[0].shield << 7) | (switches [0].fire << 6));
    case 1:
      return ((switches[1].shield << 7) | (switches [1].fire << 6));
    case 2:
      return ((switches[0].left << 7) | (switches [0].right << 6));
    case 3:
      return ((switches[1].left << 7) | (switches [1].right << 6));
    case 4:
      return ((switches [0].thrust << 7) | (check_switch_decr (& start1) << 6));
    case 5:
      return ((switches [1].thrust << 7) | ((optionreg [2] << 6) & 0x40));
    case 6:
      return (((check_switch_decr (& start2)) << 7) |
          ((optionreg [2] << 5) & 0x40));
    case 7:
      return (0x00 /* upright */ | ((optionreg [2] << 4) & 0x40));
    }
      result = 0x00;
      break;
    case BZ_INPUTS:
      result =
    (check_switch_decr (& start1) << 5) |
    (switches [0].fire << 4) |
    (switches [0].leftfwd << 3) |
    (switches [0].leftrev << 2) |
    (switches [0].rightfwd << 1) |
    (switches [0].rightrev << 0);
      break;
    case LUNAR_MEM:
      result = mem [addr & 0xff].cell;
      break;
    case LUNAR_SW1:
      result = 0x80 | /* DIAG STEP */
        ((cyc >> 2) & 0x40) | /* 3 KHz */
    ((! check_switch_decr (& slam)) << 2) |
    ((! self_test) << 1) |
    vg_done (cyc);
      break;
    case LUNAR_SW2:
      switch (addr & 0x07)
    {
    case 0:
      result = check_switch_decr (& start1) << 7;
      break;
    case 1:
      result = (! check_switch_decr (& cslot_left)) << 7;
      break;
    case 2:
      result = (check_switch_decr (& cslot_util)) << 7;
      break;
    case 3:
      result = (! check_switch_decr (& cslot_right)) << 7;
      break;
    case 4: /* game select */
      result = (! check_switch_decr (& start2)) << 7;
      break;
    case 5:
      result = switches [0].abort << 7;
      break;
    case 6:
      result = switches [0].right << 7;
      break;
    case 7:
      result = switches [0].left << 7;
      break;
    }
      break;
    case LUNAR_POT:
      result = 255 - joystick.y;
      break;
    case ASTEROIDS_SW1:
      switch (addr & 0x07)
    {
    case 0: /* nothing */
      result = 0;
      break;
    case 1: /* 3 KHz */
      /* clock toggles at 3 KHz */
      result = ((cyc >> 2) & 0x80);
      break;
    case 2: /* vector generator halt */
      result = (! vg_done (cyc)) << 7;
      break;
    case 3: /* hyperspace */
      if (game == ASTEROIDS)
        result = switches [0].hyper << 7;
      else
        result = switches [0].shield << 7;
      break;
    case 4: /* fire */
      result = switches [0].fire << 7;
      break;
    case 5: /* diag step */
      result = 0;
      break;
    case 6: /* slam */
      result = check_switch_decr (& slam) << 7;
      break;
    case 7: /* self test */
      result = self_test << 7;
      break;
    }
      break;
    case ASTEROIDS_SW2:
      switch (addr & 0x07)
    {
    case 0: /* left coin */
      result = !(check_switch_decr (& cslot_left) << 7);
      break;
    case 1: /* center coin */
      result = !(check_switch_decr (& cslot_util) << 7);
      break;
    case 2: /* right coin */
      result = !(check_switch_decr (& cslot_right) << 7);
      break;
    case 3: /* 1P start */
      result = check_switch_decr (& start1) << 7;
      break;
    case 4: /* 2P start */
      result = check_switch_decr (& start2) << 7;
      break;
    case 5: /* thrust */
      result = switches [0].thrust << 7;
      break;
    case 6: /* rot right */
      result = switches [0].right << 7;
      break;
    case 7: /* rot left */
      result = switches [0].left << 7;
      break;
    }
      break;
    case RB_SW:
      result =
    ((switches [0].fire) << 7) |
    ((! check_switch_decr (& start1)) << 6) |
    0x3f;
      break;
    case RB_JOY:
      if (mem[0x1808].cell & 0x01)  /* POTSEL signal in RB_SND output register */
    result = joystick.x;
      else
    result = joystick.y;
      break;
    case UNKNOWN:
      /* Battlezone has a bogus cmp (00,x) instruction at 6b79 that causes
         lots of stray reads */
      if ((game != BATTLEZONE) || (PC != 0x6b7b))
    {
      breakflag = 1;
      printf("@%04x Unknown rd addr %04x data %02x tag %02x\n",
         PC, addr,mem[addr].cell, mem[addr].tagr);
    }
      result = 0xff;
      break;
    default:
      breakflag = 1;
      printf("@%04x Why are we here rd addr %04x data %02x tag %02x\n",
         PC, addr,mem[addr].cell, mem[addr].tagr);
      result = 0xff;
      break;
    }

  if(tag & BREAKTAG)
    {
      printf ("@%04x Breakpoint read %04x, data %02x\n", PC, addr, result);
    }

  return(result);
}


void MEMWR(unsigned addr, int val, int PC, unsigned long cyc)
{
  register byte tag;
  int newbank;
  int i;
  byte temp;

  if(!(tag=mem[addr].tagw)) mem[addr].cell = val;
  else 
    {
      if(tag & BREAKTAG) 
    {
      breakflag = 1;
      printf ("@%04x Breakpoint write %04x, data %02x\n", PC, addr, val);
    }

      switch(tag & 0x3f) 
    {
    case MEMORY:
      mem[addr].cell = val;
      break;
#if (MEMORY != VECRAM)
    case VECRAM:
      mem[addr].cell = val;
      break;
#endif
    case COINOUT:
      newbank = (val>>2) & 1;
      if (newbank != bank)
        printf ("Bank select %d\n", newbank);
      bank = newbank;
      break;
    case INTACK:
      irq_cycle = cyc + 6144;
      break;
    case WDCLR:
    case EAROMCON:
    case EAROMWR:
      /* none of these are implemented yet, but they're OK. */
      break;
    case VGRST:
      vg_reset (cyc);
      break;
    case VGO:
      vg_go (cyc);
      break;
    case DMACNT:
      printf ("@%04x write to DMACNT!!!\n", PC);
      breakflag = 1;
      break;
    case POKEY1:
      pokey_write (0, addr & 0x0f, val, PC, cyc);
      break;
    case POKEY2:
      pokey_write (1, addr & 0x0f, val, PC, cyc);
      break;
    case POKEY3:
      pokey_write (2, addr & 0x0f, val, PC, cyc);
      break;
    case POKEY4:
      pokey_write (3, addr & 0x0f, val, PC, cyc);
      break;
    case MBSTART:
      /* printf("@%04x MBSTART wr addr %04x val %02x\n", PC, addr & 0x1f, val); */
      mb_go (addr & 0x1f, val);
      break;
    case COLORRAM:
      break;
    case TEMP_OUTPUTS:
      break;
    case BZ_SOUND:
      if ((val & 0x04) && ! ((mem [addr].cell & 0x0c) == 0x04))
        bell ();
      mem [addr].cell = val;
      break;
    case RB_SND:
      mem[addr].cell = val;
      break;
    case RB_SND_RST:
      break;
    case LUNAR_MEM:
      mem [addr & 0xff].cell = val;
      break;
    case LUNAR_OUT:
      break;
    case LUNAR_SND:
    case LUNAR_SND_RST:
      break;
    case ASTEROIDS_OUT:
      newbank = (val >> 2) & 1;
      if (newbank != bank)
        printf ("Bank select %d\n", newbank);
      for (i = 0; i < 0x100; i++)
        {
          temp = mem [0x200+i].cell;
          mem [0x200+i].cell = mem [0x300+i].cell;
          mem [0x300+i].cell = temp;
        }
      bank = newbank;
      break;
    case ASTEROIDS_EXP:
    case ASTEROIDS_THUMP:
    case ASTEROIDS_SND:
    case ASTEROIDS_SND_RST:
      break;
        case AST_DEL_OUT:
      switch (addr & 7)
        {
        case 0: /* player 1 start LED */
        case 1: /* player 2 start LED */
        case 2:
        case 3: /* thrust sound */
          break;
        case 4:  /* bank switching */
          newbank = (val >> 7) & 1;
          if (newbank != bank)
        printf ("Bank select %d\n", newbank);
          for (i = 0; i < 0x100; i++)
        {
          temp = mem [0x200+i].cell;
          mem [0x200+i].cell = mem [0x300+i].cell;
          mem [0x300+i].cell = temp;
        }
          bank = newbank;
          break;
        case 5: /* left coin counter */
        case 6: /* center coin counter */
        case 7: /* right coin counter */
          break;
        }
      break;
    case IGNWRT:
      break;
    case ROMWRT:
      printf("@%04x ROM write addr %04x val %02x data %02x tag %02x\n",
         PC, addr, val, mem[addr].cell, mem[addr].tagw);
      break;
    case UNKNOWN:
      printf("@%04x Unknown wr addr %04x val %02x data %02x tag %02x\n",
         PC, addr, val, mem[addr].cell, mem[addr].tagw);
      breakflag = 1;
      break;
    default:
      printf("@%04x Why are we here wr addr %04x val %02x data %02x tag %02x\n",
         PC, addr, val, mem[addr].cell, mem[addr].tagw);
      breakflag = 1;
      break;
    }
    }
}


void read_rom_image (char *fn, unsigned faddr, unsigned len, unsigned offset)
{
  FILE *fp;
  byte core[4096];
  unsigned j;

  fprintf(stderr, "Reading %s at addr %04x\n", fn ,faddr);
  fp = fopen(fn, "rb");
  if(!fp) 
    {
      fprintf(stderr, "Can't open %s\n", fn);
      exit(1);
    }
  if(fseek(fp, offset, 0) != 0)
    {
      fprintf(stderr, "Can't seek to %d in %s\n", offset, fn);
      exit(1);
    }
  if(fread(core,1,len,fp) != len)
    {
      fprintf(stderr, "Read of %s fails\n", fn);
      exit(1);
    }
  fclose (fp);
  for(j=0; j < len; j++) 
    {
      mem[faddr].cell = core[j];
      mem[faddr].tagr = 0;
      mem[faddr].tagw = ROMWRT;
      faddr++;
    }
}


void tag_area (unsigned addr, unsigned len, int dir, int tag)
{
  unsigned i;

  for (i = 0; i < len; i++)
    {
      if (dir & RD)
    mem [addr].tagr = tag;
      if (dir & WR)
    mem [addr].tagw = tag;
      addr++;
    }
}


void setup_roms_and_tags (rom_info *rom_list, tag_info *tag_list)
{
  while (rom_list->name != NULL)
    {
      read_rom_image (rom_list->name, rom_list->addr, rom_list->len, rom_list->offset);
      rom_list++;
    }
  while (tag_list->len != 0)
    {
      tag_area (tag_list->addr, tag_list->len, tag_list->dir, tag_list->tag);
      tag_list++;
    }
}


void copy_rom (unsigned source, unsigned dest, unsigned len)
{
  unsigned i;

  for(i = 0; i < len; i++)
    {
      mem[dest].cell = mem[source].cell;
      mem[dest].tagr = mem[source].tagr;
      mem[dest].tagw = mem[source].tagw;
      dest++;
      source++;
    }

}


