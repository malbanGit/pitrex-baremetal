/*
done: FILE *fopen(const char *filename, const char *mode);
done: int fclose(FILE *stream);
done: size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
done: size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
done: int fseek(FILE *stream, long int offset, int whence);
done: int _chdir(char *_dir)

int fflush(FILE *stream);
int fprintf(FILE *stream, const char *format, ...);
int fgetc(FILE *stream);
char *fgets(char *str, int n, FILE *stream);
int fputc(int char, FILE *stream);
int fputs(const char *str, FILE *stream);
*/
#pragma once
#include <stdio.h>
#include <errno.h>
#undef errno

// from cstubs
extern int ___errno___;
#define errno ___errno___

extern char *__env[1];
extern char **__environ__;
#define environ __environ__

/*
extern int ___fork___( void );
#define fork ___fork___

extern int ___wait___( int *status );
#define wait ___wait___
*/

/* Required include for fstat() */
#include <sys/stat.h>

/* Required include for times() */
#include <sys/times.h>

/* Prototype for the UART write function */
#include <baremetal/pi_support.h>

/* A pointer to a list of environment variables and their values. For a minimal
   environment, this empty list is adequate: */

#include <baremetal/ff.h>

#define MAX_FILE_OPEN 10

#ifdef PATH_MAX
#undef PATH_MAX
#endif

#define PATH_MAX 80
#define PATH_MAXDEPTH 10
#define MODE_LEN 10

#define SEPERATOR '/'

#ifndef SEEK_SET
#define	SEEK_SET	0	/* set file offset to offset */
#define	SEEK_CUR	1	/* set file offset to current plus offset */
#define	SEEK_END	2	/* set file offset to EOF plus offset */
#endif

#ifdef NAME_MAX
#undef NAME_MAX
#endif
#define NAME_MAX 128

#define DT_REG 0
#define DT_DIR 1


#undef dirent
struct _dirent {
   // long            d_ino;  /* Inode Nummer */
   //  off_t           d_off;  /* Offset zum nächsten dirent */
   //  unsigned short  d_reclen;/* Länge dieses Eintrags */
    char            d_name[NAME_MAX+1]; /* Dateiname */
    unsigned char            d_type; /* type of file */
  };
typedef struct _dirent dirent;

typedef enum {
    E_OK = 0,        ///<
    E_DIR_NOT_FOUND,       ///<
    E_SYSTEM_BOUNDS,
    E_FILE_NOT_FOUND,
    E_FILE_NOT_OPEN,
    E_READ_ERROR,
    E_WRITE_ERROR,
    E_SEEK_ERROR,
    E_OTHER
} ErrnoErrors;

typedef struct {
    FRESULT result;
    int pathCounter;
    char path[PATH_MAXDEPTH][PATH_MAX]; // path part of the file (without the name)
    char name[PATH_MAX]; // only the name part
    char mode[MODE_LEN];
    unsigned char modeTranslation;
    FIL file;
    int used;
    int pos;
} FF_WRAP;

#ifdef feof
#undef feof
#endif

#define fopen __fopen
#define fclose __fclose
#define fread __fread
#define fwrite __fwrite
#define fseek __fseek
#define ftell __ftell
#define fflush __fflush
#define chdir __chdir
#define getcwd __getcwd
#define opendir __opendir
#define readdir __readdir
#define closedir __closedir
#define fgets __fgets
#define fgetc __fgetc
#define rename __rename
#define remove __remove
#define fputs __fputs
#define fputc __fputc
#define feof __feof





int __rename(const char *oldName, const char *newName);
int __remove(const char *name);
int __remove(const char *name);

extern int fsInit;
FILE *__fopen(const char *filename, const char *mode);
int __fclose(FILE *stream);
size_t __fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t __fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int __fseek(FILE *stream, long int offset, int whence);
int __ftell(FILE *stream);
int __chdir(char *_dir);
int __fflush(FILE *stream);
char *__getcwd(char *buf, size_t size);
DIR *__opendir(char *name);
dirent *__readdir(DIR *cd);
int __closedir(DIR *cd);
char *__fgets(char *str, int n, FILE *stream);
//char * ff_getErrorText(int errNo);
int __fgetc(FILE *_f);
int __feof(FILE *_f);
int __fputs(const char *str, FILE *stream);
int __fputc(char str, FILE *stream);


void outputCurrentDirectory();
  
int __filelength(FILE *fd);

void v_errori(char *message, int i);

void initFileSystem();

// __attribute__((unused))
//static char* error_msg="ERROR!"; ++

//#define exit(a) do{ char* error_msg="ERROR!"; v_errori(error_msg, a);} while (0)

static void __exit(int a)
{
  char* error_msg=(char*)"ERROR!"; v_errori(error_msg, a);
}

#define exit __exit

extern int (*old_fprintf)(FILE *stream, const char *format, ...);

#define fprintf(fd,...) \
do  \
{ \
  char buf[2048]; \
  sprintf(buf, __VA_ARGS__); \
  if ((((int) fd) == (int)stdout) || (((int) fd) == (int)stderr)) \
  { \
    old_fprintf(fd,"%s", buf); \
  } \
  else \
  { \
    f_printf(&((FF_WRAP *)fd)->file, "%s", buf ); \
  } \
} while (0)
/*
#define fputs(message, fd) \
do  \
{ \
  fprintf(fd, "%s", message); \
} while (0)

#define fputc(message, fd) \
do  \
{ \
  fprintf(fd, "%c", message); \
} while (0)
*/



static inline int bm_isupper(int c) {
  return (c >= (int) 'A' && c <= (int) 'Z') ? 1 : 0;
}

static inline int bm_tolower(int c) {
  return ((bm_isupper(c) != 0) ? (c + 32) : c);
}
static inline int stricmp(const char *s1, const char *s2) {
  unsigned char *p1 = (unsigned char *) s1;
  unsigned char *p2 = (unsigned char *) s2;

  for (; bm_tolower((int) *p1) == bm_tolower((int) *p2); p1++, p2++) {
    if (*p1 == (unsigned char) '\0') {
      return 0;
    }
  }

  return (int) (bm_tolower((int) *p1) - bm_tolower((int) *p2));
}

// returns -1 for "out of radix"
int bm_atoi(char* str, int radix);

int usleep(useconds_t microSeconds);

int crc32(const void *data, size_t n_bytes, uint32_t* crc);
#include "junzip.h"


#define PACKED      __attribute__ ((packed))
#define ALIGN(n)    __attribute__ ((aligned (n)))
#define NOOPT       __attribute__ ((optimize (0)))
#define MAXOPT      __attribute__ ((optimize (3)))

// big endian (to be used for constants only)
#define BE(value)   ((((value) & 0xFF00) >> 8) | (((value) & 0x00FF) << 8))

extern void bcm2835_delayMicroseconds(uint64_t micros);
#define usDelay bcm2835_delayMicroseconds
#define MsDelay(millis) bcm2835_delayMicroseconds((uint64_t)((uint64_t)1000 * (uint64_t)millis))


#include <sys/time.h> // for __gettimeofday
#define gettimeofday __gettimeofday
int __gettimeofday ( struct timeval *tp ,  struct timezone *tz );


#define sleep __sleep
unsigned int __sleep(unsigned int seconds);



