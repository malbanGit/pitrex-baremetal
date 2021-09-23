#ifndef TEMPEST_H
#define TEMPEST_H

#include "aaemain.h"

extern char *gamename[];
extern int gamenum;



int init_tempest();
void run_tempest();
void end_tempest();
void set_tempest_video();
/*

void Temp_PokeyWrite(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void Temp_PokeyWrite2(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void coin_write(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void colorram_w(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void draw_temp_screen(void);
void TempGo(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void AVGclear(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void Sounds2(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void ProgRom(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void VramWrite(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void tempest_led_w(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void avg_reset_w(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
void tempest_vector_list (void);
void Load_Roms(void);
void SwitchGame(void);
void CloseProgram(void);
UINT8 TempestIN0read (UINT32 address , struct MemoryReadByte *psMemRead);
UINT8 TempestDSW1 (UINT32 address, struct MemoryReadByte *psMemRead);
UINT8 TempestDSW2 (UINT32 address, struct MemoryReadByte *psMemRead);
UINT8 Pokey1Read (UINT32 address, struct MemoryReadByte *psMemRead);
UINT8 Pokey2Read (UINT32 address, struct MemoryReadByte *psMemRead);
//UINT8 MathboxStatusRead (UINT32 address, struct MemoryReadByte *psMemRead);
//UINT8 MathboxLowbitRead (UINT32 address, struct MemoryReadByte *psMemRead);
//UINT8 MathboxHighbitRead (UINT32 address, struct MemoryReadByte *psMemRead);
UINT8 TempestControls (UINT32 address, struct MemoryReadByte *psMemRead);
*/


#endif