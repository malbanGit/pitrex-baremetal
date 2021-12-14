#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif


#include "z80/z80.h"

#include "SegaG80.h"
#include "samples.h"
#include "globals.h"
#include "log.h" 

#define MAX_SPEECH	16		/* Number of speech samples which can be queued */
#define NOT_PLAYING	-1		/* The queue holds the sample # so -1 will indicate no sample */

static int queue[MAX_SPEECH];
static int queuePtr = 0;
static int roarPlaying;	/* Is the ship roar noise playing? */


int sega_sh_start (void) {
	int i;
	
	for (i = 0; i < MAX_SPEECH; i ++)
		queue[i] = NOT_PLAYING;
	
	return 0;
}

void sega_sh_update (void) {
	int sound;

	/* if a sound is playing, return */
	if (sample_playing(9)) return;

	/* Check the queue position. If a sound is scheduled, play it */
	if (queue[queuePtr] != NOT_PLAYING)
   	{
		sound = queue[queuePtr];
		// log_it("Sound Played %d",(sound));
		sound=sound-1;
		sample_start(9, sound, 0);
        
		/* Update the queue pointer to the next one */
		queue[queuePtr] = NOT_PLAYING;
		++ queuePtr;
		if (queuePtr >= MAX_SPEECH)
			queuePtr = 0;
		}
	
	}

void sega_sh_speech_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW)
{ 
	//int loop;
static int sound=0;
static int newPtr=0;
static int soundlatch=0;	
	
  if (data > 0x17)
	{soundlatch =data;return;}
	
	if (soundlatch==0xbf || soundlatch ==0x8a)
	{return;}
	/* The sound numbers start at 1 but the sample name array starts at 0 */
	//if (sound > 0x80 || sound ==0x97)
	//{presound =1;return;}
if (soundlatch==0x88 ||soundlatch > 0x96 );
if (data==0x0a && soundlatch !=0x8c){return;}
	
			/* Queue the new sound */
	//log_it("num samples %d",NUM_SPEECH_SAMPLES);
	sound=data;
	if (sound > NUM_SPEECH_SAMPLES){return;} 
	
    	newPtr = queuePtr;
		while (queue[newPtr] != NOT_PLAYING) {
			newPtr ++;
			if (newPtr >= MAX_SPEECH)
				newPtr = 0;
			if (newPtr == queuePtr) {
				/* The queue has overflowed. Oops. */
//				allegro_message("Queue overflow");
				return;
				}
			}
			
		queue[newPtr] = sound;
       // log_it("Sound queued %d",(sound));
/*
//allegro_message("Main Speech call");	

        if (data > 0x17)
	{soundlatch =data;return;}
	
	if (soundlatch==0xbf || soundlatch ==0x8a)
	{return;}
	// The sound numbers start at 1 but the sample name array starts at 0 
	//if (sound > 0x80 || sound ==0x97)
	//{presound =1;return;}
	if (soundlatch==0x88 ||soundlatch > 0x96 );
	if (data==0x0a && soundlatch !=0x8c){return;}
	
	
//	allegro_message("data %x",data);
			// Queue the new sound //
	sound=data;
	
    	newPtr = queuePtr;
		while (queue[newPtr] != NOT_PLAYING) {
			newPtr ++;
			if (newPtr >= MAX_SPEECH)
				newPtr = 0;
			if (newPtr == queuePtr) {
				// The queue has overflowed. Oops. //
				allegro_message("Queue overflow");
				return;
				}
			}
			
		queue[newPtr] = sound;
		*/
	
}
void spacfury1_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW)
{

data ^= 0xff;

	/* craft growing */
	if (data & 0x01)
		sample_start (1, 0x15, 0);

	/* craft moving */
	if (data & 0x02)
   	{
		if (!sample_playing(2))
			sample_start (2, 0x16, 1);
	}
	else
		sample_stop (2);

	/* Thrust */
	if (data & 0x04)
   	{
		if (!sample_playing(3))
			sample_start (3, 0x19, 1);
	}
	else
		sample_end (3);

	/* star spin */
	if (data & 0x40)
		sample_start (4, 0x1d, 0);

	/* partial warship? */
	if (data & 0x80)
		sample_start (4, 0x1e, 0);



}
void spacfury2_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW)
{ 
data ^= 0xff;

	/* craft joining */
	if (data & 0x01)
		sample_start (5, 0x17, 0);

	/* ship firing */
	if (data & 0x02)
   	{
		if (sample_playing(6))
			sample_stop(6);
		sample_start(6, 0x18, 0);

        }

	/* fireball */
	if (data & 0x04)
		sample_start (7, 0x1b, 0);

	/* small explosion */
	if (data & 0x08)
		sample_start (7, 0x1b, 0);
	/* large explosion */
	if (data & 0x10)
		sample_start (7, 0x1a, 0);

	/* docking bang */
	if (data & 0x20)
		sample_start (8, 0x1c, 0);
}

void elim1_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW)
{
data ^= 0xff;

	/* Play fireball sample */
	if (data & 0x02)
		sample_start (0, 0, 0);

	/* Play explosion samples */
	if (data & 0x04)
		sample_start (1, 10, 0);
	if (data & 0x08)
		sample_start (1, 9, 0);
	if (data & 0x10)
		sample_start (1, 8, 0);

	/* Play bounce sample */
	if (data & 0x20)
   	{
		if (sample_playing(2))
			sample_stop (2);
		sample_start (2, 1, 0);
	}

	/* Play lazer sample */
	if (data & 0xc0)
   	{
		if (sample_playing(3))
			sample_stop (3);
		sample_start (3, 5, 0);
	}

}
void elim2_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW)
{
data ^= 0xff;

	/* Play thrust sample */
	if (data & 0x0f)
		sample_start (4, 6, 0);
	else
		sample_stop (4);

	/* Play skitter sample */
	if (data & 0x10)
		sample_start (5, 2, 0);

	/* Play eliminator sample */
	if (data & 0x20)
		sample_start (6, 3, 0);

	/* Play electron samples */
	if (data & 0x40)
		sample_start (7, 7, 0);
	if (data & 0x80)
		sample_start (7, 4, 0);

}


void Zektor_AY8910_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW)
{

	static int soundlatch=0;
    static int lastdata=0;
	static int toggle=0;


    if (data !=soundlatch)
	{soundlatch=data;toggle=0;lastdata=data;return;}
	else
	{ if (data==8 && toggle ==0) 
	   {if (!sample_playing(8)){sample_start (8, 30, 0);data=0;toggle=1;}}
	 
	 if (data==9 && toggle ==0) 
	   {if (!sample_playing(9)){sample_start (8, 31, 0);data=0;toggle=1;}}


	}
	
	
	//log_it("Data Zektor AY %x",data);
	

}

void Zektor1_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW)
{
    //log_it("Data Zektor sh 1 %x",data);
	data ^= 0xff;
   
	/* Play fireball sample */
	if (data & 0x02)
                sample_start (0, 19, 0);

	/* Play explosion samples */
	if (data & 0x04)
                sample_start (1, 29, 0);
 	if (data & 0x08)
                  sample_start (1, 28, 0);
 	if (data & 0x10)
                  sample_start (1, 27, 0);

	/* Play bounce sample */
	if (data & 0x20)
   	{
                if (sample_playing(2))
                       sample_stop (2);
                sample_start (2, 20, 0);
	}

	/* Play lazer sample */
	if (data & 0xc0)
   	{
		if (sample_playing(3))
			sample_stop (3);
                sample_start (3, 24, 0);
	}

}
void Zektor2_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW)
{
    static int lastdata;
	
	log_it("Data Zektor sh 2 %x",data);
	data ^= 0xff;
    
	/* Play thrust sample */
	if (data & 0x0f)
	{ if (data!=lastdata){sample_start (4, 25, 1); lastdata=data; }
	}
	else
	{sample_end(4);lastdata=data;}
	
	/* Play skitter sample */
	if (data & 0x10)
                sample_start (5, 21, 0);

	/* Play eliminator sample */
	if (data & 0x20)
                sample_start (6, 22, 0);

	/* Play electron samples */
	if (data & 0x40)
                sample_start (7, 40, 0);
	if (data & 0x80)
                sample_start (7, 41, 0);

}
void StarTrek_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW)
{
	switch (data)
   	{
		case 0x08: /* phaser - trek1.wav */
			sample_start (1, 0x17, 0);
			break;
		case 0x0a: /* photon - trek2.wav */
			sample_start (1, 0x18, 0);
			break;
		case 0x0e: /* targeting - trek3.wav */
			sample_start (1, 0x19, 0);
			break;
		case 0x10: /* dent - trek4.wav */
			sample_start (2, 0x1a, 0);
			break;
		case 0x12: /* shield hit - trek5.wav */
			sample_start (2, 0x1b, 0);
			break;
		case 0x14: /* enterprise hit - trek6.wav */
			sample_start (2, 0x1c, 0);
			break;
		case 0x16: /* enterprise explosion - trek7.wav */
			sample_start (2, 0x1d, 0);
			break;
		case 0x1a: /* klingon explosion - trek8.wav */
			sample_start (2, 0x1e, 0);
			break;
		case 0x1c: /* dock - trek9.wav */
			sample_start (1, 0x1f, 0);
			break;
		case 0x1e: /* starbase hit - trek10.wav */
			sample_start (1, 0x20, 0);
			break;
		case 0x11: /* starbase red - trek11.wav */
			sample_start (1, 0x21, 0);
			break;
		case 0x22: /* starbase explosion - trek12.wav */
			sample_start (2, 0x22, 0);
			break;
		case 0x24: /* small bonus - trek13.wav */
			sample_start (3, 0x23, 0);
			break;
		case 0x25: /* large bonus - trek14.wav */
			sample_start (3, 0x24, 0);
			break;
		case 0x26: /* starbase intro - trek15.wav */
			sample_start (1, 0x25, 0);
			break;
		case 0x27: /* klingon intro - trek16.wav */
			sample_start (1, 0x26, 0);
			break;
		case 0x28: /* enterprise intro - trek17.wav */
			sample_start (1, 0x27, 0);
			break;
		case 0x29: /* player change - trek18.wav */
			sample_start (1, 0x28, 0);
			break;
		case 0x2e: /* klingon fire - trek19.wav */
			sample_start (2, 0x29, 0);
			break;
		case 0x04: /* impulse start - trek20.wav */
			sample_start (3, 0x2a, 0);
			break;
		case 0x06: /* warp start - trek21.wav */
			sample_start (3, 0x2b, 0);
			break;
		case 0x0c: /* red alert start - trek22.wav */
			sample_start (4, 0x2c, 0);
			break;
		case 0x18: /* warp suck - trek23.wav */
			sample_start (4, 0x2d, 0);
			break;
		case 0x19: /* saucer exit - trek24.wav */
			sample_start (4, 0x2e, 0);
			break;
		case 0x2c: /* nomad motion - trek25.wav */
			sample_start (5, 0x2f, 0);
			break;
		case 0x2d: /* nomad stopped - trek26.wav */
			sample_start (5, 0x30, 0);
			break;
		case 0x2b: /* coin drop music - trek27.wav */
			sample_start (1, 0x31, 0);
			break;
		case 0x2a: /* high score music - trek28.wav */
			sample_start (1, 0x32, 0);
			break;
		}
}


 void TacScan_sh_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW)
{
	static int lastsound=0;
	//static int lastvoice=0;
	//static int lastkv=0;
	int sound;   /* index into the sample name array in drivers/sega.c */
	int voice=0; /* which voice to play the sound on */
	int loop;    /* is this sound continuous? */

	loop = 0;
	switch (data)
   	{
		case shipRoar:
			/* Play the increasing roar noise */
			voice = kVoiceShipRoar;
			sound = 0;
			roarPlaying = 1;
			break;
		case shipStop:
			/* Play the decreasing roar noise */
			voice = kVoiceShipRoar;
			sound = 2;
			roarPlaying = 0;
			break;
		case shipLaser:
			voice = kVoiceShip;
			sound = 3;
			break;
		case shipExplosion:
			voice = kVoiceShip;
			sound = 4;
			break;
		case shipDocking:
			voice = kVoiceShip;
			sound = 5;
			break;
		case tunnelHigh:
			voice = kVoiceTunnel;
			sound = 6;
			break;
		case stingerThrust:
			voice = kVoiceStinger;
			sound = 7;
                        loop = 0; //leave off sound gets stuck on
			break;
		case stingerLaser:
			voice = kVoiceStinger;
			sound = 8;
                        loop = 0;
			break;
		case stingerExplosion:
			voice = kVoiceStinger;
			sound = 9;
			break;
		case stingerStop:
			voice = kVoiceStinger;
			sound = -1;
			break;
		case enemyBullet0:
		case enemyBullet1:
		case enemyBullet2:
			voice = kVoiceEnemy;
			sound = 10;
			break;
		case enemyExplosion0:
		case enemyExplosion1:
		case enemyExplosion2:
			voice = kVoiceTunnel;
			sound = 11;
			break;
                case tunnelw: voice = kVoiceShip;
                              sound = 12;
                              break;
                case flight1: voice = kVoiceExtra;
                              sound = 13;
                              break;
                case flight2: voice = kVoiceExtra;
                              sound = 14;
                              break;
                case flight3: voice = kVoiceExtra;
                              sound = 15;
                              break;
                case flight4: voice = kVoiceExtra;
                              sound = 16;
                              break;
                case flight5: voice = kVoiceExtra;
                              sound = 17;
                              break;
		case formation:
                              voice = kVoiceForm;
              	              sound = 18;
			      break;
	       	case warp:    voice = kVoiceExtra;
                              sound = 19;
                              break;
                case extralife: voice = kVoiceExtralife;
                                sound = 20;
                                break;
                case credit:    voice = kVoiceExtra;
                                sound = 21;
                                break;

                default:

			/* don't play anything */
			sound = -1;
			break;
                      }
	if (sound != -1)
   	{
		if (voice==kVoiceExtra && sound==lastsound){return;}
		else {lastsound=sound;}
		sample_stop (voice);
		// sample_adjust(voice, PLAYMODE_PLAY); 
		/* If the game is over, turn off the stinger noise */
		if (data == shipStop)
		{sample_stop (kVoiceStinger);}
		sample_start (voice, sound, loop);
	}
}

void tacscan_sh_update (void)
{
	/* If the ship roar has started playing but the sample stopped */
	/* play the intermediate roar noise */

	if ((roarPlaying) && (!sample_playing(kVoiceShipRoar)))
		sample_start (kVoiceShipRoar, 1, 1);
}

