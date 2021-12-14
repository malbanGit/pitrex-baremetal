/*
    6809.h   	-	header for monitor and simulator
    Copyright (C) 2001  Arto Salmi

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#ifndef __M6809_H_
#define __M6809_H_ 1

#define R_A 1
#define R_B (1<<1)
#define R_AB (R_A|R_B)
#define R_D (1<<2)
#define R_X (1<<3)
#define R_Y (1<<4)
#define R_S (1<<5)
#define R_U (1<<6)
#define R_PC (1<<7)
#define R_DP (1<<8)
#define R_CC (1<<9)
#define R_H (1<<10)
#define R_N (1<<11)
#define R_Z (1<<12)
#define R_V (1<<13)
#define R_C (1<<14)
#define R_HNZVC (R_H|R_N|R_Z|R_V|R_C|R_E) /*TESTING*/
#define R_EFI (1<<15)
#define R_VAL (1<<16)
#define R_RES (1<<17)
#define R_MSN (1<<18)
#define R_LSN (1<<19)
#define R_EA  (1<<20)
#define R_CLOCKTICKS (1<<21)
#define R_ARG (1<<22)
#define R_E (1<<23)
#define R_F (1<<24)
#define R_I (1<<25)
#define R_Always (1<<26)
// remember to recompile 6809dasm.c whenever R_Always changes!
/* Only used in flush() code.  Don't flush pseudo-regs that are always lost after translating a single opcode. */


//#define R_FLUSH (((unsigned int)-1) & (~R_RES) & (~R_VAL) & (~R_EA) & (~R_ARG_ )
// #define R_FLUSH (R_A|R_B|R_X|R_Y|R_S|R_U|R_PC|R_DP|R_HNZVC|R_Always)
#define R_FLUSH (R_A|R_B|R_X|R_Y|R_S|R_U|R_PC|R_DP|R_HNZVC|R_CLOCKTICKS|R_Always)
typedef unsigned char UINT8;
typedef signed char INT8;

typedef unsigned short UINT16;
typedef signed short INT16;

typedef unsigned int UINT32;
typedef signed int INT32;

// CC=EFHINZVC
#define E_FLAG 0x80
#define F_FLAG 0x40
#define H_FLAG 0x20
#define I_FLAG 0x10
#define N_FLAG 0x08
#define Z_FLAG 0x04
#define V_FLAG 0x02
#define C_FLAG 0x01

extern UINT8 *memory;
extern int Gen_C;
extern int Gen_Switch;
extern int Do_Timing;
extern int Single_Step;

extern int Accessible;

/* 6809.c */
extern int VECTREX;
extern void translate6809 (int CodePC, int NextPC, int cycles);
extern void cpu_reset (void);
extern void dumpf(int def, int use, char *s, ...);

/* vectrex-regs.c */
extern char *Name[0x10000];
extern int In[0x10000], Out[0x10000], Trash[0x10000];
extern unsigned char DP_In[0x10000], DP_Out[0x10000];

extern void init_vectrex_bios(void);

#endif /* __M6809_H_ */
