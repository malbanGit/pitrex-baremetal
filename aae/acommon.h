#pragma once
#ifndef ACOMMON_H
#define ACOMMON_H

#include "osd_cpu.h"

//#include <allegro.h>
//#include "winalleg.h"

void set_aae_leds(int a,int b, int c );


void NoWrite(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
	

void Pokey0Write(unsigned int address, unsigned char data, struct MemoryWriteByte *psMemWrite);
void Pokey1Write(unsigned int address, unsigned char data, struct MemoryWriteByte *psMemWrite);
unsigned char randRead (unsigned int address, struct MemoryReadByte *psMemRead);

void do_sound(void);
void center_window(void);
void Set_ForeGround(void);
int load_state6502();
int save_state6502();
int load_file(char *filename, unsigned char *buf, int size);
int save_file(char *filename, unsigned char *buf, int size);
int save_file_char(char *filename, unsigned char *buf, int size);
void msg_loop(void);
void video_loop(void);
void return_to_menu(void);
void sanity_check_config(void);
void setup_ambient(int style);

void do_AYsound(void);
void playstreamedsample(int channel,signed char *data,int len,int vol);

int DSRead_1(int some);
int DSRead_2(int some);
int vector_timer (int deltax, int deltay);
#endif