#include <math.h>

//#define M_PI   3.14159265358979323846264338327950288
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)


extern double rotX[4][4];
extern double rotY[4][4];
extern double rotZ[4][4];

struct _Vector3d {
  signed char y0;
  signed char x0;
  signed char z0;
  
  signed char y1;
  signed char x1;
  signed char z1;
};
typedef struct _Vector3d Vector3d;

struct _Vertex3d {
  signed char y;
  signed char x;
  signed char z;
};
typedef struct _Vertex3d Vertex3d;


inline static void initMatrix(double m[4][4]) 
{
  for (int x=0; x<4;x++)
  {
      for (int y=0; y<4;y++)
      {
          if (x==y)
              (m)[x][y] = 1;
          else
              (m)[x][y] = 0;
      }
  }
}

inline static void  buildRotationX(int angle)
{
  double radianse = degToRad(angle);
  initMatrix(rotX);
  rotX[1][1] = cos(radianse);
  rotX[2][1] = -sin(radianse);
  
  rotX[1][2] = sin(radianse);
  rotX[2][2] = cos(radianse);
}
inline static void  buildRotationY(int angle)
{
  double radianse = degToRad(angle);
  initMatrix(rotY);
  rotY[0][0] = cos(radianse);
  rotY[2][0] = sin(radianse);
  
  rotY[0][2] = -sin(radianse);
  rotY[2][2] = cos(radianse);
}
inline static void  buildRotationZ(int angle)
{
  double radianse = degToRad(angle);
  initMatrix(rotZ);
  rotZ[0][0] = cos(radianse);
  rotZ[1][0] = -sin(radianse);
  
  rotZ[0][1] = sin(radianse);
  rotZ[1][1] = cos(radianse);
}

inline static void copyVertex3d(Vertex3d origin, Vertex3d *target)
{
  target->x = origin.x;
  target->y = origin.y;
  target->z = origin.z;
}

inline static void  multiplyMatrix(double rotate[4][4], Vertex3d original, Vertex3d *target)
{
    target->x = rotate[0][0]*original.x + rotate[1][0]*original.y + rotate[2][0]*original.z + rotate[3][0]*1;
    target->y = rotate[0][1]*original.x + rotate[1][1]*original.y + rotate[2][1]*original.z + rotate[3][1]*1;
    target->z = rotate[0][2]*original.x + rotate[1][2]*original.y + rotate[2][2]*original.z + rotate[3][2]*1;
}
