#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <stdio.h>

void Print(FILE *file, const char *s);
void Print(FILE *file, const char *s, int i);
void Print(FILE *file, const char *s1, const char *s2);
void Print(FILE *file, const char *s, float f);
void Print(FILE *file, const char *s1, int i, const char *s2);
void Print(FILE *file, const char *s1, float f, const char *s2);
void Print(FILE *file, const char *s1, int i1, const char *s2, int i2);
void Print(FILE *file, const char *s1, float f1, const char *s2, float f2);
void Print(FILE *file, const char *s1, int i1, const char *s2, int i2, const char *s3);
void Print(FILE *file, const char *s1, float f1, const char *s2, float f2, const char *s3);

#endif
