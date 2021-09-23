#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

#include "vector.h"
#include "globals.h"
#include "log.h"


void draw_line (int x1, int y1, int x2, int y2, int color, int z); // x_interface


int X_OFFSET = X_OFFSET_BASE;
int Y_OFFSET = Y_OFFSET_BASE;




void add_color_line(float sx, float sy, float ex, float ey, int r, int g, int b)
{
//printf("draw line\n\r");
#ifndef NO_PI

   v_directDraw32(sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, ex*MUL_FAC+X_OFFSET,ey*MUL_FAC+Y_OFFSET,(r+g+b)/3/2);
#else    
  draw_line (sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, ex*MUL_FAC+X_OFFSET, ey*MUL_FAC+Y_OFFSET, 100, 102);
#endif
}

void add_color_line_xr(float sx, float sy, float ex, float ey, int r, int g, int b)
{
//printf("draw line\n\r");
#ifndef NO_PI

   v_directDraw32(sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, ex*MUL_FAC+X_OFFSET,ey*MUL_FAC+Y_OFFSET,(r+g+b)/3/2);
#else    
  draw_line (sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, ex*MUL_FAC+X_OFFSET, ey*MUL_FAC+Y_OFFSET, 100, 102);
#endif
}

void add_line(float sx, float sy, float ex, float ey, int zvalue, float gc, float mod)
{
 int  Y_OFFSET= Y_OFFSET_BASE+1;
 int  X_OFFSET= X_OFFSET_BASE+1;
//printf("draw line\n\r");
#ifndef NO_PI
//printf("zvalue %i \n\r", zvalue);
  zvalue = zvalue<<3;
  if (gamenum == ASTEROID) // asteroids deluxe
  {
    sy=-sy;
    ey=-ey;  

    Y_OFFSET=-Y_OFFSET_BASE+10000;
    X_OFFSET= X_OFFSET_BASE-4000;
    
  }
  v_directDraw32(sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, ex*MUL_FAC+X_OFFSET,ey*MUL_FAC+Y_OFFSET, zvalue%128);
#else    
  draw_line (sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, ex*MUL_FAC+X_OFFSET, ey*MUL_FAC+Y_OFFSET, 100, 102);
#endif
}

void add_line_yr(float sx, float sy, float ex, float ey, int zvalue, float gc, float mod)
{
 int  Y_OFFSET= Y_OFFSET_BASE+1;
 int  X_OFFSET= X_OFFSET_BASE+1;
//printf("draw line\n\r");
#ifndef NO_PI
//printf("zvalue %i \n\r", zvalue);
  zvalue = zvalue<<3;
  //if (gamenum == ASTEROID) // asteroids deluxe
  {
    sy=-sy;
    ey=-ey;  

    Y_OFFSET=-Y_OFFSET_BASE+10000;
    X_OFFSET= X_OFFSET_BASE-4000;
    
  }
  v_directDraw32(sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, ex*MUL_FAC+X_OFFSET,ey*MUL_FAC+Y_OFFSET, zvalue%128);
#else    
  draw_line (sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, ex*MUL_FAC+X_OFFSET, ey*MUL_FAC+Y_OFFSET, 100, 102);
#endif
}
void add_point_yr(float sx, float sy ,int zvalue, float gc, float mod , float adj)
{
  v_directDraw32((((int)sx)<<5)+(X_OFFSET), -((((int)sy)<<5)+(Y_OFFSET)), (((int)sx)<<5)+(X_OFFSET),-((((int)sy)<<5)+(Y_OFFSET)), zvalue%128);
}



void add_color_point(float sx, float sy, int r, int g, int b)
{
//printf("draw point\n\r");
  #ifndef NO_PI
    v_directDraw32(sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, sx*MUL_FAC+X_OFFSET,sy*MUL_FAC+Y_OFFSET, (r+g+b)/3/2);
#else    
  draw_line (sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, 100, 102);
#endif
}
void add_point(float sx, float sy ,int zvalue, float gc, float mod , float adj)
{
//printf("draw point\n\r");
  #ifndef NO_PI
    v_directDraw32(sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, sx*MUL_FAC+X_OFFSET,sy*MUL_FAC+Y_OFFSET, zvalue%128);
#else    
  draw_line (sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, 100, 102);
#endif
  
}

void draw_color_vectors()
{
  printf("draw_color_vectors\n\r");
}

void draw_lines(void)
{
  printf("draw_color_lines\n\r");
}
void draw_texs(void)
{
  printf("draw_texs\n\r");
}