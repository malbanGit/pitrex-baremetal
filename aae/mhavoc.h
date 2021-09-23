#ifndef MHAVOC_H
#define MHAVOC_H

#include "aaemain.h"


//extern char *gamename[];
int MHscale_by_cycles(int val, int clock);
int init_mhavoc();
void run_mhavoc();
void run_alphaone();
void end_mhavoc();
void mhavoc_interrupt();
//void MH_generate_vector_list(void);
//void MHled_write(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);

//UINT8 MHDSW1 (UINT32 address, struct MemoryReadByte *psMemRead);
//UINT8 MHDSW2 (UINT32 address, struct MemoryReadByte *psMemRead);
//UINT8 MHControls2 (UINT32 address, struct MemoryReadByte *psMemRead);
//
//UINT8 MHIN0read (UINT32 address, struct MemoryReadByte *psMemRead);

#endif