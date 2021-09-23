#ifndef LOG_H
#define LOG_H

#include "stdio.h"

FILE  *errorlog;
int open_log(char *filename);
int log_it(char *format, ...);
int close_log(void);

#endif