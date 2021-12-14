/***************************************************************************
machine\starwars.h

This file is Copyright 1997, Steve Baines.

Release 2.0 (6 August 1997)

See drivers\starwars.c for notes

***************************************************************************/


int starwars_trakball_r(int data);
int starwars_interrupt(void);

int banked_rom_r(int);
int input_bank_0_r(int);
int input_bank_1_r(int);
int opt_0_r(int);
int opt_1_r(int);
int adc_r(int);
void evggo(int, int);
void evgres(int, int);
void wdclr(int, int);
void irqclr(int, int);
void led3(int, int);
void led2(int, int);
void mpage(int, int);
void led1(int,int);
void recall(int, int);
void nstore(int, int);
void adcstart0(int, int);
void adcstart1(int, int);
void adcstart2(int, int);


