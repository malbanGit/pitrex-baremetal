#include "samples.h"
#include "globals.h"

#define MAX_VOICE 16
int testvoc=0;
int testvol;
int game_voice[MAX_VOICE]; //Global Voice For Samples
int sample_vol[MAX_VOICE];


void voice_init(int num)
{
}

void sample_start(int channel,int samplenum,int loop)
{
}


void sample_set_freq(int channel,int freq)
{
}


void sample_set_volume(int channel,int volume)
{
}

void sample_adjust(int channel, int mode)
{
}

void sample_stop(int channel)
{   
}
void sample_end(int channel) //For looped samples to avoid crackle.
{
}
int sample_playing(int channel)
{
 return 0;
}


void free_samples(void)
{
}

void mute_sound()
{
}


void restore_sound()
{
}


void aae_play_streamed_sample(int channel, unsigned char *data,int len,int freq,int volume)
{
}