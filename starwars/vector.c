#include <vectrex/vectrexInterface.h>

#include "vector.h"
#include "globals.h"
#include "log.h"

int X_OFFSET = X_OFFSET_BASE;
int Y_OFFSET = Y_OFFSET_BASE;

void add_color_line(float sx, float sy, float ex, float ey, int r, int g, int b)
{
   v_directDraw32(sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, ex*MUL_FAC+X_OFFSET,ey*MUL_FAC+Y_OFFSET,(r+g+b)/3/2);
}

void add_color_line_xr(float sx, float sy, float ex, float ey, int r, int g, int b)
{
   v_directDraw32(sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, ex*MUL_FAC+X_OFFSET,ey*MUL_FAC+Y_OFFSET,(r+g+b)/3/2);
}

void add_line(float sx, float sy, float ex, float ey, int zvalue, float gc, float mod)
{
 int  Y_OFFSET= Y_OFFSET_BASE+1;
 int  X_OFFSET= X_OFFSET_BASE+1;
  zvalue = zvalue<<3;
  v_directDraw32(sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, ex*MUL_FAC+X_OFFSET,ey*MUL_FAC+Y_OFFSET, zvalue%128);
}

void add_line_yr(float sx, float sy, float ex, float ey, int zvalue, float gc, float mod)
{
 int  Y_OFFSET= Y_OFFSET_BASE+1;
 int  X_OFFSET= X_OFFSET_BASE+1;
  zvalue = zvalue<<3;
  sy=-sy;
  ey=-ey;  

  Y_OFFSET=-Y_OFFSET_BASE+10000;
  X_OFFSET= X_OFFSET_BASE-4000;
  
  v_directDraw32(sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, ex*MUL_FAC+X_OFFSET,ey*MUL_FAC+Y_OFFSET, zvalue%128);
}
void add_point_yr(float sx, float sy ,int zvalue, float gc, float mod , float adj)
{
  v_directDraw32((((int)sx)<<5)+(X_OFFSET), -((((int)sy)<<5)+(Y_OFFSET)), (((int)sx)<<5)+(X_OFFSET),-((((int)sy)<<5)+(Y_OFFSET)), zvalue%128);
}
void add_color_point(float sx, float sy, int r, int g, int b)
{
    v_directDraw32(sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, sx*MUL_FAC+X_OFFSET,sy*MUL_FAC+Y_OFFSET, (r+g+b)/3/2);
}
void add_point(float sx, float sy ,int zvalue, float gc, float mod , float adj)
{
    v_directDraw32(sx*MUL_FAC+X_OFFSET, sy*MUL_FAC+Y_OFFSET, sx*MUL_FAC+X_OFFSET,sy*MUL_FAC+Y_OFFSET, zvalue%128);
}