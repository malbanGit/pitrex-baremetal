#ifndef AAE_AVG_H
#define AAE_AVG_H


//#include <allegro.h>
//#include "winalleg.h"

#define VCTR 0
#define HALT 1
#define SVEC 2
#define STAT 3
#define CNTR 4
#define JSRL 5
#define RTSL 6
#define JMPL 7
#define SCAL 8

void set_new_frame();
void avg_init();
void AVG_RUN();
int avg_go();
int avg_clear();
int avg_check();

#endif