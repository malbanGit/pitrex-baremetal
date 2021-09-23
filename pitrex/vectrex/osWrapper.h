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
//extern int errno;
extern int errno;
#include <ff.h>

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
char * ff_getErrorText(int errNo);
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
/*
static inline int isspace(int c)
{
   return c == ' ' || c == '\t'; // || whatever other char you consider space
}
static inline int islower(int c) {
  return (c >= (int) 'a' && c <= (int) 'z') ? 1 : 0;
}
static inline int toupper(int c) {
  return ((islower(c) != 0) ? (c - 32) : c);
}
*/

int usleep(useconds_t microSeconds);

int crc32(const void *data, size_t n_bytes, uint32_t* crc);
#include "junzip.h"


#define PACKED      __attribute__ ((packed))
#define ALIGN(n)    __attribute__ ((aligned (n)))
#define NOOPT       __attribute__ ((optimize (0)))
#define MAXOPT      __attribute__ ((optimize (3)))

// big endian (to be used for constants only)
#define BE(value)   ((((value) & 0xFF00) >> 8) | (((value) & 0x00FF) << 8))

/* circle */
// some same defines... but I don't care!

#define EnableInterrupts()  __asm volatile ("cpsie i")
#define DisableInterrupts() __asm volatile ("cpsid i")

//
// Cache control
//
#define InvalidateInstructionCache()    \
                __asm volatile ("mcr p15, 0, %0, c7, c5,  0" : : "r" (0) : "memory")
#define FlushPrefetchBuffer()   __asm volatile ("mcr p15, 0, %0, c7, c5,  4" : : "r" (0) : "memory")
#define FlushBranchTargetCache()    \
                __asm volatile ("mcr p15, 0, %0, c7, c5,  6" : : "r" (0) : "memory")
#define InvalidateDataCache()   __asm volatile ("mcr p15, 0, %0, c7, c6,  0" : : "r" (0) : "memory")
#define CleanDataCache()    __asm volatile ("mcr p15, 0, %0, c7, c10, 0" : : "r" (0) : "memory")

//
// Barriers
//
#define DataSyncBarrier()   __asm volatile ("mcr p15, 0, %0, c7, c10, 4" : : "r" (0) : "memory")
#define DataMemBarrier()    __asm volatile ("mcr p15, 0, %0, c7, c10, 5" : : "r" (0) : "memory")

#define InstructionSyncBarrier() FlushPrefetchBuffer()
#define InstructionMemBarrier() FlushPrefetchBuffer()

#define CompilerBarrier()   __asm volatile ("" ::: "memory")


void EnterCritical (void);
void LeaveCritical (void);

/* do I need anown implementation?
void mem_init (unsigned long ulBase, unsigned long ulSize);
unsigned long mem_get_size (void);

void *__malloc (unsigned long ulSize);
void __free (void *pBlock);

#define malloc __malloc
#define free __free
*/

void MsDelay (unsigned nMilliSeconds);  
void usDelay (unsigned nMicroSeconds);

#include <sys/time.h> // for __gettimeofday
#define gettimeofday __gettimeofday
int __gettimeofday ( struct timeval *tp ,  struct timezone *tz );


#define sleep __sleep
unsigned int __sleep(unsigned int seconds);



