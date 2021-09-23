#include "Utilities.h"
#include "GSBString.h"

void Print(FILE *file, const char *s)
{
	fputs(s, file);
}

void Print(FILE *file, const char *s, int i)
{
	fputs(s, file);
	fputs(String::itoa(i).c_str(), file);
}

void Print(FILE *file, const char *s1, const char *s2)
{
	fputs(s1, file);
	fputs(s2, file);
}

void Print(FILE *file, const char *s, float f)
{
	fputs(s, file);
	fputs(String::ftoa(f).c_str(), file);
}

void Print(FILE *file, const char *s1, int i, const char *s2) 
{
	Print(file, s1, i);
	Print(file, s2);
}

void Print(FILE *file, const char *s1, float f, const char *s2) 
{
	Print(file, s1, f);
	Print(file, s2);
}

void Print(FILE *file, const char *s1, int i1, const char *s2, int i2)
{
	Print(file, s1, i1);
	Print(file, s2, i2);
}

void Print(FILE *file, const char *s1, float f1, const char *s2, float f2)
{
	Print(file, s1, f1);
	Print(file, s2, f2);
}

void Print(FILE *file, const char *s1, int i1, const char *s2, int i2, const char *s3)
{
	Print(file, s1, i1);
	Print(file, s2, i2, s3);
}

void Print(FILE *file, const char *s1, float f1, const char *s2, float f2, const char *s3)
{
	Print(file, s1, f1);
	Print(file, s2, f2, s3);
}
