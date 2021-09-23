/**********************************************************************************************

     TMS5220 interface

     Written for MAME by Frank Palazzolo
     With help from Neill Corlett
     Additional tweaking by Aaron Giles

***********************************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../driver.h"
#include "tms5220.h"
#include "5220intf.h"
#include "../globals.h"
#include "../samples.h"
//#include "generic.h"

#include <vectrex/vectrexInterface.h>

/* these describe the current state of the output buffer */
#define MIN_SLICE 100	/* minimum update step for TMS5220 */
static int sample_pos;
static int buffer_len=50000;
static int emulation_rate;
static unsigned char buffer[50000];

static struct TMS5220interface *intfa;
static int channel;


/* static function prototypes */
static void tms5220_update (int force);



/**********************************************************************************************

     tms5220_sh_start -- allocate buffers and reset the 5220

***********************************************************************************************/

int tms5220_sh_start (struct TMS5220interface *iinterface)
{
    intfa = iinterface;

    /* determine the output sample rate and buffer size */
	if (gamenum == MHAVOCRV){
	buffer_len = intfa->clock / 80 / 50;
	emulation_rate = buffer_len * 50;
	}
	else{
    buffer_len = intfa->clock / 80 / Machine->drv->frames_per_second;
    emulation_rate = buffer_len * Machine->drv->frames_per_second;
	}
    sample_pos = 0;
    
    
    
printf("Starting TMS5220\n\r");    
    
    /* allocate the buffer */
//    if ((buffer = malloc (buffer_len)) == 0)
//        return 1;
	memset (buffer, 0x80, buffer_len);
// MALBAN    stream2 = play_audio_stream(buffer_len, 8, FALSE, emulation_rate , config.mainvol, 128); //450  13500
//   v_play_audio_stream(buffer_len, 8, emulation_rate); //450  13500
    /* reset the 5220 */
  	tms5220_reset ();
    tms5220_set_irq (iinterface->irq);

    /* request a sound channel */
    //channel = get_play_channels (1);
	channel=1;
    return 0;
}



/**********************************************************************************************

     tms5220_sh_stop -- free buffers

***********************************************************************************************/

void tms5220_sh_stop (void)
{
//    if (buffer)
//        free (buffer);
//    buffer = 0;
}



/**********************************************************************************************

     tms5220_sh_update -- update the sound chip

***********************************************************************************************/

void tms5220_sh_update (void)
{
//    if (Machine->sample_rate == 0) return;

    /* finish filling the buffer if there's still more to go */
    if (sample_pos < buffer_len)
        tms5220_process (buffer + sample_pos, buffer_len - sample_pos);
    sample_pos = 0;

    /* play this sample */
// MALBAN    
//aae_play_streamed_sample (1, buffer, buffer_len, emulation_rate, intfa->volume);
v_playIRQSample(buffer, buffer_len, 10000, PLAY_ONCE);


}



/**********************************************************************************************

     tms5220_data_w -- write data to the sound chip

***********************************************************************************************/

void tms5220_data_w (int offset, int data)
{
    /* bring up to date first */
    tms5220_update (0);
    tms5220_data_write (data);
}



/**********************************************************************************************

     tms5220_status_r -- read status from the sound chip

***********************************************************************************************/

int tms5220_status_r (int offset)
{
    /* bring up to date first */
    tms5220_update (1);
    return tms5220_status_read ();
}



/**********************************************************************************************

     tms5220_ready_r -- return the not ready status from the sound chip

***********************************************************************************************/

int tms5220_ready_r (void)
{
    /* bring up to date first */
    tms5220_update (0);
    return tms5220_ready_read ();
}



/**********************************************************************************************

     tms5220_int_r -- return the int status from the sound chip

***********************************************************************************************/

int tms5220_int_r (void)
{
    /* bring up to date first */
    tms5220_update (0);
    return tms5220_int_read ();
}



/**********************************************************************************************

     tms5220_update -- update the sound chip so that it is in sync with CPU execution

***********************************************************************************************/

static void tms5220_update (int force)
{
    int totcycles, leftcycles, newpos;

    /* determine the new buffer positon */
	if (gamenum == MHAVOCRV)
	       {
			     totcycles = 25000;
                 leftcycles = gammaticks;
	         }
	else {
    totcycles = cpu_getfperiod ();
    leftcycles = cpu_getfcount ();
    }
	newpos = buffer_len * (totcycles - leftcycles) / totcycles;
    /* if we need more than MIN_SLICE samples, or if we're not yet talking, generate them now */
    if (newpos > buffer_len)
        newpos = buffer_len;
    if (newpos - sample_pos < MIN_SLICE && !force)
        return;
    tms5220_process (buffer + sample_pos, newpos - sample_pos);
    sample_pos = newpos;
}
