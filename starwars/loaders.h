#ifndef LOADERS_H
#define LOADERS_H

#include "globals.h"


int load_zip(char *archname, char *filename, char *buf, int size);
int load_roms(const char *archname , const struct roms p[]);
int verify_rom(const char *archname , const struct roms p[],int romnum);
int verify_sample(const char *p[], int num);
int load_samples(const char *p[], int val);
int file_exist(char *filename);
void snapshot( char* filename);
int load_hi_aae(int start, int size, int image);
int save_hi_aae(int start, int size, int image);

#endif