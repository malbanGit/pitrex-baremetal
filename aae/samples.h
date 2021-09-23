#ifndef SAMPLES_H
#define SAMPLES_H

void voice_init(int num);
void sample_start(int channel,int samplenum,int loop);
void sample_set_freq(int channel,int freq);
void sample_set_volume(int channel,int volume);
void sample_stop(int channel);
void sample_end(int channel);
int sample_playing(int channel);
void free_samples(void);
void sample_adjust(int channel, int mode);
void mute_sound();
void restore_sound();
void aae_play_streamed_sample(int channel, unsigned char  *data,int len,int freq,int volume);

#endif