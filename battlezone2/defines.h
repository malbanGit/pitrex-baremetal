#ifndef DEFINES_H
#define DEFINES_H

#pragma once

#include "targetver.h"

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
//#include <tchar.h>



#define PI (3.14159)
#define RADIAN (PI*2)

#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE (1==1)
#define FALSE (1==0)
#endif

#ifndef Uint8
#define Uint8 unsigned char
#endif

#ifndef Uint32
#define Uint32 unsigned int
#endif

#ifndef Sint16
#define Sint16 signed short int
#endif

extern "C" 
{
  #include <vectrex/vectrexInterface.h>
  void drawLine(float x0, float y0, float x1, float y1, int c);

}
#define V_MUL 19000    

#endif // DEFINES_H
