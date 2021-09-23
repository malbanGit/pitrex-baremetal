#include <vectrex/vectrexInterface.h>

#include "allegro_dummy.h"


int num_joysticks=0;
JOYSTICK_INFO joy[5];



int install_sound(int digi, int midi, const char *cfg_path){return 1;}
void * load_sample(const char *filename){return 0;}
int play_sample(void *spl, int vol, int pan, int freq, int loop){return 0;}
void stop_sample(void *spl){}
void destroy_sample(void *spl){}
void *load_midi(const char *filename){return 0;}
int play_midi(void *midi, int loop){}
void stop_midi(void){}
void xor_mode(int on){}
int install_joystick(int type){}


