#ifndef VECTOR_H
#define VECTOR_H

#ifndef NO_PI
#define MUL_FAC 32
#define MUL_SHIFT 5
#define X_OFFSET_BASE -14000
#define Y_OFFSET_BASE -12000
#else
#define MUL_FAC 1
#define MUL_SHIFT 0
#define X_OFFSET_BASE 0;
#define Y_OFFSET_BASE 0;
#endif

//void draw_points(void);
void draw_lines(void);
void draw_texs(void);
void draw_color_vectors();
void add_color_line(float sx, float sy, float ex, float ey, int r, int g, int b);
void add_color_point(float sx, float sy, int r, int g, int b);
void add_line(float sx, float sy, float ex, float ey, int zvalue, float gc, float mod);
void add_line_yr(float sx, float sy, float ex, float ey, int zvalue, float gc, float mod);


void add_point(float sx, float sy ,int zvalue, float gc, float mod , float adj);
void add_point_yr(float sx, float sy ,int zvalue, float gc, float mod , float adj);


void cache_line(float startx, float starty, float endx, float endy, int zvalue, float gc, float mod);
void cache_point(float pointx, float pointy ,int zvalue, float gc, float mod , float adj);
void cache_txt(float pointx, float pointy ,int size, int color);

void cache_end(void);
void cache_clear(void);


static inline void add_color_line2(float sx, float sy, float ex, float ey, int r, int g, int b)
{
  int intensity = (r+g+b)/3/2;///3/2;
  extern int starWarsZoom; 

  if (starWarsZoom) 
  {
    if (intensity <= 10) intensity = 30;
  }

    
  if (intensity<20) intensity = 0;
//  printf("Int: %i\n", intensity);
   v_directDraw32((((int)sx)<<MUL_SHIFT)+X_OFFSET_BASE, (((int)sy)<<MUL_SHIFT)+Y_OFFSET_BASE, (((int)ex)<<MUL_SHIFT)+X_OFFSET_BASE,(((int)ey)<<MUL_SHIFT)+Y_OFFSET_BASE,intensity);
}

#endif

