#ifndef SPACDUEL_H
#define SPACDUEL_H

#include "aaemain.h"

extern char *gamename[];

int init_spacduel();
void run_spacduel();
void run_bwidow();
void end_spacduel();

/*
void PokeyWrite1(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void PokeyWrite2(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void AVGgo(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void draw_sd_screen(void);
void BW_generate_vector_list(void);
void led_write(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void avgdvg_reset_w(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
UINT8 DSW1 (UINT32 address, struct MemoryReadByte *psMemRead);
UINT8 DSW2 (UINT32 address, struct MemoryReadByte *psMemRead);
UINT8 SDControls (UINT32 address, struct MemoryReadByte *psMemRead);
UINT8 GravControls2 (UINT32 address, struct MemoryReadByte *psMemRead);
UINT8 GravControls (UINT32 address, struct MemoryReadByte *psMemRead);
UINT8 BWControls2 (UINT32 address, struct MemoryReadByte *psMemRead);
UINT8 BWControls (UINT32 address, struct MemoryReadByte *psMemRead);
UINT8 IN0read (UINT32 address, struct MemoryReadByte *psMemRead);
void VectorWrite(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
*/

#endif