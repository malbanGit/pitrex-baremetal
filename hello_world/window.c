#include <stdio.h>
#include <vectrex/vectrexInterface.h>
#ifndef TRUE
#define TRUE (0==0)
#define FALSE (!TRUE)
#endif
void startFrame()
{
  v_WaitRecal();
  //v_doSound();
  v_setBrightness(64);        /* set intensity of vector beam... */
  v_readButtons();
  v_readJoystick1Analog();
  //v_playAllSFX();
}
static int64_t ScaleXMul=1LL, ScaleXDiv=1LL, ScaleXOffsetPre=0LL, ScaleXOffsetPost=0LL,
               ScaleYMul=1LL, ScaleYDiv=1LL, ScaleYOffsetPre=0LL, ScaleYOffsetPost=0LL;
int tx(int x) { // convert x from window to viewport
  return (int)(((((int64_t)x)+ScaleXOffsetPre)*ScaleXMul)/ScaleXDiv + ScaleXOffsetPost);
}
int ty(int y) { // and y
  return (int)(((((int64_t)y)+ScaleYOffsetPre)*ScaleYMul)/ScaleYDiv + ScaleYOffsetPost);
}
void window(int xl, int yb, int xr, int yt) {
  // Does not yet support flip/rotate
  // We will use normalised viewport coordinates of x: -18000 : 18000 and y: -24000 : 24000
  int64_t width, height;
  int xc, yc;
  int oxl = xl, oyb = yb, oxr = xr, oyt = yt;
  width = (int64_t)xr-(int64_t)xl;
  height = (int64_t)yt-(int64_t)yb;
  if (width*4 > height*3) {
    // window is wider than aspect ratio, so we will have black bars at the top and bottom
    height = (width * 4) / 3;
    yc = (yb+yt)/2;
    yb = yc - height/2;
    yt = yc + height/2;
  } else if (width*4 < height*3) {
    // window is taller than aspect ratio, so we will have black bars at the sides
    width = (height*3) / 4;
    xc = (xl+xr)/2;
    xl = xc - width/2;
    xr = xc + width/2;
  }
  ScaleXMul = 36000LL; ScaleXDiv = width; ScaleXOffsetPre = -width/2LL; ScaleXOffsetPost = 0LL; ScaleXOffsetPost = (tx(xr) - tx(oxr)) / 2LL;
  ScaleYMul = 48000LL; ScaleYDiv = height; ScaleYOffsetPre = -height/2LL; ScaleYOffsetPost = 0LL; ScaleYOffsetPost = (ty(yt) - ty(oyt)) / 2LL;
  // setCustomClipping(TRUE, tx(oxl), ty(oyb), tx(oxr), ty(oyt)); // transform world (window) coordinates to viewport (normalised device coordinates) before
                                                                  // clipping.  That way clipping code does not need to know about world coordinates.
  // not implemented as I don't have the updated library yet
}
void line(int xl, int yb, int xr, int yt, int col) {
  //fprintf(stdout, "line(%d,%d, %d,%d, %d);n", tx(xl),ty(yb), tx(xr),ty(yt), col);
  v_directDraw32(tx(xl),ty(yb), tx(xr),ty(yt), col);
}
int main(int argc, char **argv) {
  vectrexinit(1);
  v_init();
  usePipeline = 1;
  v_setRefresh(60);
  window(0,0, 360,480); // both scaled and offset...
  for (;;) {
    startFrame();
    line(  0,   0,   0, 480, 96);
    line(  0, 480, 360, 480, 96);
    line(360, 480, 360,   0, 96);
    line(360,   0,   0,   0, 96);
    line(  80, 140,  80,  340, 64);
    line(  80, 340, 280,  340, 64);
    line( 280, 340, 280,  140, 64);
    line( 280, 140,  80,  140, 64);
    v_printString(-10,0, "HELLO", 7, 64);
    v_setBrightness(60);
    v_printStringRaster(-8,-4, "WORLD", 5*8, -7, '\0');
  }
  return 0;
}
