//#include "ccpu.h"
#include <stdlib.h>

#include "cinemat.h"
#include "aaemain.h"
#include "samples.h"
#include "globals.h"
#include "ccpu.h"
#include "log.h"
/*************************************
 *
 *  Macros
 *
 *************************************/

#define RISING_EDGE(bit, changed, val)	(((changed) & (bit)) && ((val) & (bit)))
#define FALLING_EDGE(bit, changed, val)	(((changed) & (bit)) && !((val) & (bit)))

#define SOUNDVAL_RISING_EDGE(bit)		RISING_EDGE(bit, bits_changed, sound_val)
#define SOUNDVAL_FALLING_EDGE(bit)		FALLING_EDGE(bit, bits_changed, sound_val)

#define SHIFTREG_RISING_EDGE(bit)		RISING_EDGE(bit, (last_shift ^ current_shift), current_shift)
#define SHIFTREG_FALLING_EDGE(bit)		FALLING_EDGE(bit, (last_shift ^ current_shift), current_shift)

#define SHIFTREG2_RISING_EDGE(bit)		RISING_EDGE(bit, (last_shift2 ^ current_shift), current_shift)
#define SHIFTREG2_FALLING_EDGE(bit)		FALLING_EDGE(bit, (last_shift2 ^ current_shift), current_shift)

// circular queue with read and write pointers for demon
#define QUEUE_ENTRY_COUNT  10
static int sound_latch_rp = 0;
static int sound_latch_wp = 0;
static int sound_latch[QUEUE_ENTRY_COUNT];

extern char *gamename[];



extern void (*sound_write) (unsigned char,unsigned char);
extern int cinemat_outputs;
extern unsigned int ioOutputs;
extern UINT32 current_frame;
static UINT32 current_shift = 0;
static UINT32 last_shift = 0;
static UINT32 last_shift16= 0;
static UINT32 last_shift2= 0;
static UINT32 current_pitch = 0x20000;
static UINT32 last_frame = 0;

static UINT8 sound_control;

void init_snd(void)
{
	sound_control = 0x9f;

	/* reset shift register values */
   
    current_shift = 0xffff;
    last_shift = 0xffff;
    last_shift16 = 0xffff;
	last_shift2 = 0xffff;
    current_pitch = 0x20000;

}


void cini_sound_control_w(int offset, int data)
{
	UINT8 oldval = sound_control;
	
	/* form an 8-bit value with the new bit */
	sound_control = (sound_control & ~(1 << offset)) | ((data & 1) << offset);

	/* if something changed, call the sound subroutine */
	if ((sound_control != oldval) && sound_write)
		(*sound_write)(sound_control, sound_control ^ oldval);
}

/*
void cinemat_writeport (int offset, int data)
 {      	 
            if ((ioOutputs ^ SOUNDBITS) & 0x9f) //cinemat_outputs
            {
                //log_it("Data output %x bent %x",data & 0x9f,(cinemat_outputs ^ data) & 0x9f);    
		   (*sound_write)(SOUNDBITS & 0x9f, (ioOutputs ^ SOUNDBITS) & 0x9f);
 
            }
            //cinemat_outputs = data;
			ioOutputs=SOUNDBITS;
}
*/
static void cinemat_shift (UINT8 sound_val, UINT8 bits_changed, UINT8 A1, UINT8 CLK)
{
	// See if we're latching a shift

    if ((bits_changed & CLK) && (0 == (sound_val & CLK)))
	{
		current_shift <<= 1;
		if (sound_val & A1)
            current_shift |= 1;
	}
}

 void ripoff_sound(UINT8 sound_val, UINT8 bits_changed)
{
	UINT8 shift_diff, current_bg_sound;
    static UINT8 last_bg_sound;

	
    cinemat_shift (sound_val, bits_changed, 0x01, 0x02);

	// Now see if it's time to act upon the shifted data

	if ((bits_changed & 0x04) && (0 == (sound_val & 0x04)))
	{
		// Yep. Falling edge! Find out what has changed.

		shift_diff = current_shift ^ last_shift;

        current_bg_sound = ((current_shift & 0x1) << 2) | (current_shift & 0x2) | ((current_shift & 0x4) >> 2);
        if (current_bg_sound != last_bg_sound) // use another background sound ?
        {
            shift_diff |= 0x08;
            sample_stop(5);
            last_bg_sound = current_bg_sound;
        }

		if (shift_diff & 0x08)
		{
			if (current_shift & 0x08)
				sample_stop(5);
			else
               sample_start(5, 5+last_bg_sound, 1);	// Background
			   sample_set_volume(5,65);
		}

		if ((shift_diff & 0x10) && (0 == (current_shift & 0x10)))
			sample_start(2, 2, 0);	// Beep

		if (shift_diff & 0x20)
		{
			if (current_shift & 0x20)
				sample_stop(1);	// Stop it!
			else
				sample_start(1, 1, 1);	// Motor
		}

		last_shift = current_shift;
	}

	if ((bits_changed & 0x08) && (0 == (sound_val & 0x08)))
		sample_start(4, 4, 0);			// Torpedo

	if ((bits_changed & 0x10) && (0 == (sound_val & 0x10)))
		sample_start(0, 0, 0);			// Laser

	if ((bits_changed & 0x80) && (0 == (sound_val & 0x80)))
	   {sample_start(3, 3, 0);}			// Explosion

}

void null_sound(UINT8 sound_val, UINT8 bits_changed)
{
//do nothing ;(

}

void starcas_sound(UINT8 sound_val, UINT8 bits_changed)
{
    UINT32 target_pitch;
	UINT8 shift_diff;

    cinemat_shift (sound_val, bits_changed, 0x80, 0x10);

	// Now see if it's time to act upon the shifted data

	if ((bits_changed & 0x01) && (0 == (sound_val & 0x01)))
	{
		// Yep. Falling edge! Find out what has changed.

		shift_diff = current_shift ^ last_shift;

		if ((shift_diff & 1) && (0 == (current_shift & 1)))
			sample_start(2, 2, 0);	// Castle fire

		if ((shift_diff & 2) && (0 == (current_shift & 2)))
			sample_start(5, 5, 0);	// Shield hit

		if (shift_diff & 0x04)
		{
			if (current_shift & 0x04)
				sample_start(6, 6, 1);	// Star sound
			else
				sample_stop(6);	// Stop it!
		}

		if (shift_diff & 0x08)
		{
			if (current_shift & 0x08)
				sample_stop(7);	// Stop it!
			else
				sample_start(7, 7, 1);	// Thrust sound
		}

		if (shift_diff & 0x10)
		{
			if (current_shift & 0x10)
				sample_stop(4);
			else
				sample_start(4, 4, 1);	// Drone
		}

		// Latch the drone pitch

        target_pitch = (current_shift & 0x60) >> 3;
        target_pitch |= ((current_shift & 0x40) >> 5);
        target_pitch |= ((current_shift & 0x80) >> 7);

        // target_pitch = (current_shift & 0x60) >> 3;
        // is the the target drone pitch to rise and stop at.

        target_pitch = 0x10000 + (target_pitch << 12);

        // 0x10000 is lowest value the pitch will drop to
        // Star Castle drone sound

        if (current_frame > last_frame)
        {
            if (current_pitch > target_pitch)
                current_pitch -= 300;
            if (current_pitch < target_pitch)
                current_pitch += 200;
            sample_set_freq(4, current_pitch);
            last_frame = current_frame;
        }

		last_shift = current_shift;
	}

	if ((bits_changed & 0x08) && (0 == (sound_val & 0x08)))
		sample_start(3, 3, 0);			// Player fire

	if ((bits_changed & 0x04) && (0 == (sound_val & 0x04)))
		sample_start(1, 1, 0);			// Soft explosion

	if ((bits_changed & 0x02) && (0 == (sound_val & 0x02)))
		sample_start(0, 0, 0);			// Loud explosion

}

/*************************************
 *
 *	Armor Attack
 *
 *************************************/


void armora_sound(UINT8 sound_val, UINT8 bits_changed)
{
	/* on the rising edge of bit 0x10, clock bit 0x80 into the shift register */
	if (SOUNDVAL_RISING_EDGE(0x10))
		current_shift = ((current_shift >> 1) & 0x7f) | (sound_val & 0x80);

	/* execute on the rising edge of bit 0x01 */
	if (SOUNDVAL_RISING_EDGE(0x01))
	{
		/* bits 0-4 control the tank sound speed */

		/* lo explosion - falling edge */
		if (SHIFTREG_FALLING_EDGE(0x10))
			sample_start(0, 0, 0);

		/* jeep fire - falling edge */
		if (SHIFTREG_FALLING_EDGE(0x20))
			sample_start(1, 1, 0);

		/* hi explosion - falling edge */
		if (SHIFTREG_FALLING_EDGE(0x40))
			sample_start(2, 2, 0);

		/* tank fire - falling edge */
		if (SHIFTREG_FALLING_EDGE(0x80))
			sample_start(3, 3, 0);

		/* remember the previous value */
		last_shift = current_shift;
	}

	/* tank sound - 0=on, 1=off */
	/* still not totally correct - should be multiple speeds based on remaining bits in shift reg */
	if (SOUNDVAL_FALLING_EDGE(0x02))
		sample_start(4, 4, 1);
	if (SOUNDVAL_RISING_EDGE(0x02))
		sample_stop(4);

	/* beep sound - 0=on, 1=off */
	if (SOUNDVAL_FALLING_EDGE(0x04))
		sample_start(5, 5, 1);
	if (SOUNDVAL_RISING_EDGE(0x04))
		sample_stop(5);

	/* chopper sound - 0=on, 1=off */
	if (SOUNDVAL_FALLING_EDGE(0x08))
		sample_start(6, 6, 1);
	if (SOUNDVAL_RISING_EDGE(0x08))
		sample_stop(6);
}

void solarq_sound(UINT8 sound_val, UINT8 bits_changed)
{
	UINT32 shift_diff, shift_diff16;
    static int target_volume, current_volume;

    cinemat_shift (sound_val, bits_changed, 0x80, 0x10);

	if ((bits_changed & 0x01) && (0 == (sound_val & 0x01)))
    {
		shift_diff16 = current_shift ^ last_shift16;

		if ((shift_diff16 & 0x1) && (current_shift & 0x1))
        {
            switch (current_shift & 0xffff)
            {
            case 0xceb3:
                sample_start(7, 7, 0);	// Hyperspace
                break;
            case 0x13f3:
                sample_start(7, 8, 0);	// Extra
                break;
            case 0xfdf3:
                sample_start(7, 9, 0);	// Phase
                break;
            case 0x7bf3:
                sample_start(7, 10, 0);	// Enemy fire
                break;
            default:
                log_it("Unknown sound starting with: %x\n", current_shift & 0xffff);
                break;
            }
        }

		last_shift16 = current_shift;
    }

	// Now see if it's time to act upon the shifted data

	if ((bits_changed & 0x02) && (0 == (sound_val & 0x02)))
	{
		// Yep. Falling edge! Find out what has changed.

		shift_diff = current_shift ^ last_shift;

		if ((shift_diff & 0x01) && (0 == (current_shift & 0x01)))
			sample_start(0, 0, 0);	// loud expl.

		if ((shift_diff & 0x02) && (0 == (current_shift & 0x02)))
			sample_start(1, 1, 0);	// soft expl.

		if (shift_diff & 0x04) // thrust
		{
			if (current_shift & 0x04)
				{target_volume = 0;
				sample_stop(2);}
			else
            {
                target_volume = config.mainvol;
                current_volume = 0;
				sample_start(2, 2, 1);
            }
        }

        if (sample_playing(2) && (last_frame < current_frame))
        {
            if (current_volume > target_volume)
                current_volume -= 20;
            if (current_volume < target_volume)
                current_volume += 20;
            if (current_volume > 0)
                sample_set_volume(2, current_volume);
            else
                sample_stop(2);
            last_frame = current_frame;
        }

		if ((shift_diff & 0x08) && (0 == (current_shift & 0x08)))
			sample_start(3, 3, 0);	// Fire

		if ((shift_diff & 0x10) && (0 == (current_shift & 0x10)))
			sample_start(4, 4, 0);	// Capture

		if (shift_diff & 0x20)
		{
			if (current_shift & 0x20)
				sample_start(6, 6, 1);	// Nuke +
			else
				sample_stop(6);
		}

		if ((shift_diff & 0x40) && (0 == (current_shift & 0x40)))
			sample_start(5, 5, 0);	// Photon

		last_shift = current_shift;
	}
}

void spacewar_sound(UINT8 sound_val, UINT8 bits_changed)
{

	// Explosion

	if (bits_changed & 0x01)
	{
		if (sound_val & 0x01)
		{
            if (rand() & 1)
                sample_start(0, 0, 0);
            else
                sample_start(0, 6, 0);
		}
	}
	// Fire sound

	if ((sound_val & 0x02) && (bits_changed & 0x02))
	{
       if (rand() & 1)
           sample_start(1, 1, 0);
       else
           sample_start(1, 7, 0);
	}

	// Player 1 thrust

	if (bits_changed & 0x04)
	{
		if (sound_val & 0x04)
			sample_stop(3);
		else
			sample_start(3, 3, 1);
	}

	// Player 2 thrust

	if (bits_changed & 0x08)
	{
		if (sound_val & 0x08)
			sample_stop(4);
		else
			sample_start(4, 4, 1);
	}

	// Sound board shutoff (or enable)

	if (bits_changed & 0x10)
	{
		// This is a toggle bit. If sound is enabled, shut everything off.

		if (sound_val & 0x10)
		{
            int i;

			for (i = 0; i < 5; i++)
			{
				if (i != 2)
					sample_stop(i);
			}

			sample_start(2, 5, 0);	// Pop when board is shut off
		}
		else
			sample_start(2, 2, 1);	// Otherwise play idle sound
	}
}

void warrior_sound(UINT8 sound_val, UINT8 bits_changed)
{

	if ((bits_changed & 0x10) && (0 == (sound_val & 0x10)))
	{
		sample_start(0, 0, 0);			// appear
	}

	if ((bits_changed & 0x08) && (0 == (sound_val & 0x08)))
		sample_start(3, 3, 0);			// fall

	if ((bits_changed & 0x04) && (0 == (sound_val & 0x04)))
		sample_start(4, 4, 0);			// explosion (kill)

	if (bits_changed & 0x02)
	{
		if ((sound_val & 0x02) == 0)
			sample_start(2, 2, 1);			// hi level
		else
			sample_stop(2);
	}

	if (bits_changed & 0x01)
	{
		if ((sound_val & 0x01) == 0)
			sample_start(1, 1, 1);			// normal level
		else
			sample_stop(1);
	}
}

void tailg_sound(UINT8 sound_val, UINT8 bits_changed)
{
	/*logerror ("Error %d soundval %d bitschanged\n",sound_val,bits_changed);*/

	static UINT8 OldOutReg = 0;
	static UINT8 XRreg = 0;
	static UINT8 OldXRreg = 0x22;

	UINT8   outReg;
	UINT8   outDiff;		/* changed bits */
	UINT8	outLow;			/* changed bits that have just gone low */
	UINT8	xrDiff;			/* changed bits */
	UINT8   xrHigh;			/* changed bits that have just gone high */
	UINT8	xrLow;			/* changed bits that have just gone low */
	UINT8	mask;

	/* outReg = New value of the CCPU's OUT register. */
	outReg = sound_val;

	outDiff = outReg ^ OldOutReg;		/* get bits that have changed state */
	outLow = outDiff & ~outReg;			/* get bits that have just gone low */

	//logerror ("xrDiff %d XRreg %d \n",xrDiff,XRreg);

	if (outLow & 0x10)
	{
		mask = 0x01 << (outReg & 0x07);	/* get address of bit as a mask */

		if (outReg & 0x08)
			XRreg |= mask;		/* If DATA set, set bit */
		else
			XRreg &= ~mask;		/* else reset bit */

		/* check for new MUX sounds */

		xrDiff = XRreg ^ OldXRreg;	/* get diff's */
		xrLow = xrDiff & ~XRreg;	/* get bits that just went low */
		xrHigh = xrDiff & XRreg;	/* get bits that just went high */

		/*
		HYPERSPACE	0x20
		BOUNCE	    0x10
		SHIELD		0x08
		LASER		0x04
		RUMBLE	    0x02
		EXPLOSION   0x01
		*/

		if (xrLow & 0x20)
			sample_start(0, 0, 0);

		if (xrLow & 0x10)
			sample_start(4, 4, 0);

		if (xrLow & 0x08)
			sample_start(3, 3, 1);

		if (xrHigh & 0x08)
			sample_stop(3);

		if (xrLow & 0x04)
			sample_start(2, 2, 0);

		if (xrHigh & 0x04)
			sample_end(2);

		if (xrLow & 0x02)
			sample_start(5, 5, 1);

		if (xrHigh & 0x02)
			sample_stop(5);

		if (xrLow & 0x01)
			sample_start(1, 1, 0);

		OldXRreg = XRreg;
	}

	OldOutReg = outReg;	/* save new OUT register */
}

void starhawk_sound(UINT8 sound_val, UINT8 bits_changed)
{
	/* explosion - falling edge */
	if (SOUNDVAL_FALLING_EDGE(0x01))
		sample_start(0, 0, 0);

	/* right laser - falling edge */
	if (SOUNDVAL_FALLING_EDGE(0x02))
		sample_start(1, 1, 0);

	/* left laser - falling edge */
	if (SOUNDVAL_FALLING_EDGE(0x04))
		sample_start(2, 2, 0);

	/* K - 0=on, 1=off */
	if (SOUNDVAL_FALLING_EDGE(0x08))
		sample_start(3, 3, 1);
	if (SOUNDVAL_RISING_EDGE(0x08))
		sample_stop(3);

	/* master - 0=on, 1=off */
	if (SOUNDVAL_FALLING_EDGE(0x10))
		sample_start(4, 4, 1);
	if (SOUNDVAL_RISING_EDGE(0x10))
		sample_stop(4);

	/* K exit - 1=on, 0=off */
	if (SOUNDVAL_RISING_EDGE(0x80))
		sample_start(3, 5, 1);
	if (SOUNDVAL_FALLING_EDGE(0x80))
		sample_stop(3);
}

void barrier_sound(UINT8 sound_val, UINT8 bits_changed)
{
	/* Player die - rising edge */
	if (SOUNDVAL_RISING_EDGE(0x01))
		sample_start(0, 0, 0);

	/* Player move - falling edge */
	if (SOUNDVAL_FALLING_EDGE(0x02))
		sample_start(1, 1, 0);

	/* Enemy move - falling edge */
	if (SOUNDVAL_FALLING_EDGE(0x04))
		sample_start(2, 2, 0);
}

 void sundance_sound(UINT8 sound_val, UINT8 bits_changed)
{
	/* bong - falling edge */
	if (SOUNDVAL_FALLING_EDGE(0x01))
		sample_start(0, 0, 0);

	/* whoosh - falling edge */
	if (SOUNDVAL_FALLING_EDGE(0x02))
		sample_start(1, 1, 0);

	/* explosion - falling edge */
	if (SOUNDVAL_FALLING_EDGE(0x04))
		sample_start(2, 2, 0);

	/* ping - falling edge */
	if (SOUNDVAL_FALLING_EDGE(0x08))
		sample_start(3, 3, 0);

	/* ping - falling edge */
	if (SOUNDVAL_FALLING_EDGE(0x10))
		sample_start(4, 4, 0);

	/* hatch - falling edge */
	if (SOUNDVAL_FALLING_EDGE(0x80))
		sample_start(5, 5, 0);
}

 /*************************************
 *
 *	Demon
 *
 *************************************/



void demon_sound(UINT8 sound_val, UINT8 bits_changed)
{
	int pc = 0;//(register_PC);//activecpu_get_pc();

	pc =pc & 0xffff;
	//log_it("Writing Sound Latch %x ", pc & 0xffff);
	if (pc == 0x0fbc ||
		pc == 0x1fed ||
		pc == 0x2ff1 ||
		pc == 0x3fd3)
	{
		sound_latch[sound_latch_wp] = ((sound_val & 0x07) << 3);
		//log_it("Writing Sound Latch 1 %04x data = %x",pc,sound_latch[sound_latch_wp] );
	}
	if (pc == 0x0fc8 ||
		pc == 0x1ff9 ||
		pc == 0x2ffd ||
		pc == 0x3fdf)
	{
		sound_latch[sound_latch_wp] |= (sound_val & 0x07);

		//log_it("Writing Sound Latch 2 %04x data = %x",pc,sound_latch[sound_latch_wp] );

		sound_latch_wp++;
		if (sound_latch_wp == QUEUE_ENTRY_COUNT)  sound_latch_wp = 0;
	}
}
