#ifndef EAROM_H
#define EAROM_H

#include "osd_cpu.h"

void EaromWrite(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void EaromCtrl(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
UINT8 EaromRead (UINT32 address, struct MemoryReadByte *psMemRead);
void LoadEarom(void);
void SaveEarom(void);

#endif
