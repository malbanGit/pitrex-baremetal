#ifndef _FONT_H_
#define _FONT_H_

// Good sizes are:
// 6.2
// 7.5
// 9.7
// 10
// 11.7
// 12.5
// 13.8
// 15.1

#include "vector.h"

namespace font
{

    enum { ALIGN_LEFT=-1, ALIGN_CENTER=0, ALIGN_RIGHT=1 };

    int AlphanumericsGetNumLines(const char* string);
    void AlphanumericsMeasureString(const char* string, float scale, float* width, float* height);
    int AlphanumericsGetLineLen(const char* string);

    void AlphanumericsPrint(bool glowPass, int alignment, float scale, float x, float y, vector::pen* color, const char *fmt, ...);

    void AlphanumericsSetAngle(float topAscent, float bottomAscent);

    char* formatStringWithCommas(char* string);

//    extern float AlphanumericsPrintVScale;

};

#endif

