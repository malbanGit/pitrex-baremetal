
#ifndef VOOM_H
#define VOOM_H

#define SIZEX 256
#define SIZEY 256
#define FOV (M_PI/2)
#define FARPT 350 
//250
#define NEARPT 10

#define PLHEIGHT 34
#define PLMAXJUMP 24
#define PLWIDTH 17


#define VOOM_KEY_UP 1
#define VOOM_KEY_DOWN 2
#define VOOM_KEY_LEFT 4
#define VOOM_KEY_RIGHT 8

void voomHandleKeys(int keys);
void voomInit(char** lvlData, int *chunkSz);
void voomDraw();



#endif