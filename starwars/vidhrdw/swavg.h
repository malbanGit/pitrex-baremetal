#ifndef swavg_h
#define swavg_h

#define USE_AVG	0
#define USE_DVG 1
/* #define VG_DEBUG */

extern unsigned char *vectorram;
extern int vectorram_size;

void vg_go (int cyc);
void vg_reset (int offset, int data);
int vg_init (int len, int usingDvg, int flip);
void vg_stop (void);
void vg_halt (void);
int vg_done(int);

#endif
