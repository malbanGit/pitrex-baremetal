#ifndef CINEMAT_H
#define CINEMAT_H

#include "osd_cpu.h"



#define CCPU_MEMSIZE_4K        	0
#define CCPU_MEMSIZE_8K        	1
#define CCPU_MEMSIZE_16K       	2
#define CCPU_MEMSIZE_32K       	3

#define CCPU_MONITOR_BILEV  	0
#define CCPU_MONITOR_16LEV  	1
#define CCPU_MONITOR_64LEV  	2
#define CCPU_MONITOR_WOWCOL 	3


/*************************************
 *
 *  Constants
 *
 *************************************/

enum
{
	COLOR_BILEVEL,
	COLOR_16LEVEL,
	COLOR_64LEVEL,
	COLOR_RGB,
	COLOR_QB3
};

extern UINT8 bSwapXY;

int init_cinemat(void);
void run_cinemat();
void end_cinemat();

/* nicer config function */
void ccpu_Config (int jmi, int msize, int monitor);

void video_type_set(int type);
void cinemat_vector_callback(INT16 sx, INT16 sy, INT16 ex, INT16 ey, UINT8 shift);
void cini_sound_control_w(int offset, int data);
void vec_control_w(int cinemat_vector_control_w);
int cinevid_up();

void CinemaVectorData (int fromx, int fromy, int tox, int toy, int color);
unsigned long int cineExec ( unsigned long int a );

//void ExecCinemat(void);
void cineSetMSize(int m);
void cineSetMonitor(int m);
void cineSetJMI(int j);

UINT32 cineGetElapsedTicks ( UINT32 dwClearIt );

void ripoff_sound(UINT8 sound_val, UINT8 bits_changed);
//void cinemat_shift (UINT8 sound_val, UINT8 bits_changed, UINT8 A1, UINT8 CLK);
void cinemat_writeport (int offset, int data);
void init_snd(void);
void ripoff_sound(UINT8 sound_val, UINT8 bits_changed);
void armora_sound(UINT8 sound_val, UINT8 bits_changed);
void null_sound(UINT8 sound_val, UINT8 bits_changed);
void starcas_sound(UINT8 sound_val, UINT8 bits_changed);
void solarq_sound(UINT8 sound_val, UINT8 bits_changed);
void spacewar_sound(UINT8 sound_val, UINT8 bits_changed);
void warrior_sound(UINT8 sound_val, UINT8 bits_changed);
void tailg_sound(UINT8 sound_val, UINT8 bits_changed);
void starhawk_sound(UINT8 sound_val, UINT8 bits_changed);
void barrier_sound(UINT8 sound_val, UINT8 bits_changed);
void sundance_sound(UINT8 sound_val, UINT8 bits_changed);
void demon_sound(UINT8 sound_val, UINT8 bits_changed);
#endif