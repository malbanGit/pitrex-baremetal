#ifndef SEGAG80_H
#define SEGAG80_H


#include "osd_cpu.h"

#define SEGA_SHIFT 10	/* do not use a higher value. Values will overflow */


#define NUM_SPACFURY_SPEECH 21 //31
#define NUM_ZEKTOR_SPEECH 19
#define NUM_STARTREK_SPEECH 23

extern int NUM_SPEECH_SAMPLES;

//static int roarPlaying;	/* Is the ship roar noise playing? */

//TACSCAN SOUND CONSTANTS
#define	shipStop 0x10
#define shipLaser 0x18
#define	shipExplosion 0x20
#define	shipDocking 0x28
#define	shipRoar 0x40
#define	tunnelHigh 0x48
#define	stingerThrust 0x50
#define	stingerLaser 0x51
#define	stingerStop 0x52
#define	stingerExplosion 0x54
#define	enemyBullet0 0x61
#define	enemyBullet1 0x62
#define	enemyBullet2 0x63
#define	enemyExplosion0 0x6c
#define	enemyExplosion1 0x6d
#define	enemyExplosion2 0x6e
#define tunnelw   0x09
#define flight1   0x36
#define flight2   0x3b
#define flight3   0x3d
#define flight4   0x3e
#define flight5   0x3f
#define warp      0x37
#define formation 0x0b
#define nothing1  0x1a
#define nothing2  0x1b
#define extralife 0x1c
#define credit    0x2c


#define	kVoiceShip 1
#define	kVoiceTunnel 2
#define	kVoiceStinger 3
#define	kVoiceEnemy 4
#define	kVoiceShipRoar 5
#define kVoiceWarp 6
#define kVoiceForm 7
#define kVoiceExtra 8
#define kVoiceExtralife 9


void end_segag80();
int init_segag80();
void run_segag80(void);

int sega_sh_start (void);
void tacscan_sh_update (void);
void sega_sh_speech_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
static void Decrypt(int gamenum);
static void sega_security(int chip);
void sega_sh_update(void);
void elim1_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
void elim2_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
void spacfury1_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
void StarTrek_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
void spacfury2_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
//int sega_fix_dips(int port, int val);
void Zektor1_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
void Zektor2_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
void Zektor_AY8910_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
//void sega_switch_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
void TacScan_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
extern void (*sega_decrypt)(int,unsigned int *);
//int check_sega_hi(int gamenum);
/*
int zektor_loadhi();
void zektor_savehi();
int spacfury_loadhi();
void spacfury_savehi();
int tacscan_loadhi();
void tacscan_savehi();
int elim2_loadhi();
void elim2_savehi();
int startrek_loadhi();
void startrek_savehi();
void save_sega_hi();
*/

#endif