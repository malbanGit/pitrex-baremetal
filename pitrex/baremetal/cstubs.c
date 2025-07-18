/* For more information about this file, please visit:
   https://sourceware.org/newlib/libc.html#Stubs

   These are the newlib C-Library stubs for the valvers Raspberry-Pi bare-metal
   tutorial */

/*
    Graceful failure is permitted by returning an error code. A minor
    complication arises here: the C library must be compatible with development
    environments that supply fully functional versions of these subroutines.
    Such environments usually return error codes in a global errno. However,
    the Red Hat newlib C library provides a macro definition for errno in the
    header file errno.h, as part of its support for reentrant routines (see
    Reentrancy).

    The bridge between these two interpretations of errno is straightforward:
    the C library routines with OS interface calls capture the errno values
    returned globally, and record them in the appropriate field of the
    reentrancy structure (so that you can query them using the errno macro from
    errno.h).

    This mechanism becomes visible when you write stub routines for OS
    interfaces. You must include errno.h, then disable the macro, like this:
*/
#include <errno.h>
#undef errno
int ___errno___;
#define errno ___errno___

/* Required include for fstat() */
#include <sys/stat.h>

/* Required include for times() */
#include <sys/times.h>

/* Prototype for the UART write function */
#include "pi_support.h"

/* A pointer to a list of environment variables and their values. For a minimal
   environment, this empty list is adequate: */
char *__env[1] = {0};
char **__environ__ = __env;

/* A helper function written in assembler to aid us in allocating memory */
extern caddr_t _get_stack_pointer(void);


/* Never return from _exit as there's no OS to exit to, so instead we trap
   here */
void _exit( int status )
{
    /* Stop the compiler complaining about unused variables by "using" it */
    (void)status;

    while(1)
    {
        /* TRAP HERE */
    }
}


/* There's currently no implementation of a file system because there's no
   file system! */
int _close( int file )
{
    return -1;
}

// This stub function is required by stdlib
int _open(const char *name, int flags, int mode)
{
    (void)(name);
    (void)(flags);
    (void)(mode);
    return -1;   // does the stub-return need to be a specific value??
}

/* Transfer control to a new process. Minimal implementation (for a system
   without processes): */
/*
int execve( char *name, char **argv, char **env )
{
    errno = ENOMEM;
    return -1;
}
*/
int _execve( char *name, char **argv, char **env )
{
    errno = ENOMEM;
    return -1;
}


/* Create a new process. Minimal implementation (for a system without
   processes): */
//int ___fork___( void )
/*
int fork( void )
{
    errno = EAGAIN;
    return -1;
}
*/
int _fork( void )
{
    errno = EAGAIN;
    return -1;
}


/* Status of an open file. For consistency with other minimal implementations
   in these examples, all files are regarded as character special devices. The
   sys/stat.h header file required is distributed in the include subdirectory
   for this C library. */
int _fstat( int file, struct stat *st )
{
    st->st_mode = S_IFCHR;
    return 0;
}


/* Process-ID; this is sometimes used to generate strings unlikely to conflict
   with other processes. Minimal implementation, for a system without
   processes: */
int getpid( void )
{
    return 1;
}
int _getpid( void ) // needed for c++?
{
    return 1;
}


/* Query whether output stream is a terminal. For consistency with the other
   minimal implementations, which only support output to stdout, this minimal
   implementation is suggested: */
int _isatty(int file)
{
    return 1;
}


/* Send a signal. Minimal implementation: */
int kill( int pid, int sig )
{
    errno = EINVAL;
    return -1;
}
int _kill( int pid, int sig ) // needed for c++?
{
    errno = EINVAL;
    return -1;
}


/* Establish a new name for an existing file. Minimal implementation: */
int link( char *old, char *new )
{
    errno = EMLINK;
    return -1;
}


/* Set position in a file. Minimal implementation: */
int _lseek(int file, int ptr, int dir)
{
    return 0;
}

/* Open a file. Minimal implementation: */
int open( const char *name, int flags, int mode )
{
    return -1;
}


/* Read from a file. Minimal implementation: */
int _read( int file, char *ptr, int len )
{
    return 0;
}
/*
// Read from a file. Minimal implementation: 
int read( int file, char *ptr, int len )
{
    return 0;
}
*/

/* Increase program data space. As malloc and related functions depend on this,
   it is useful to have a working implementation. The following suffices for a
   standalone system; it exploits the symbol _end automatically defined by the
   GNU linker. */
caddr_t sbrk( int incr )
{
    extern char _end;
    static char* heap_end = 0;
    char* prev_heap_end;

    if( heap_end == 0 )
        heap_end = &_end;

     prev_heap_end = heap_end;
     heap_end += incr;

     return (caddr_t)prev_heap_end;
}
caddr_t _sbrk( int incr )
{
    extern char _end;
    static char* heap_end = 0;
    char* prev_heap_end;

    if( heap_end == 0 )
        heap_end = &_end;

     prev_heap_end = heap_end;
     heap_end += incr;

     return (caddr_t)prev_heap_end;
}


/* Status of a file (by name). Minimal implementation: */
int stat( const char *file, struct stat *st )
{
    st->st_mode = S_IFCHR;
    return 0;
}


/* Timing information for current process. Minimal implementation: */
clock_t times( struct tms *buf )
{
    return -1;
}


/* Remove a file's directory entry. Minimal implementation: */
int unlink( char *name )
{
    errno = ENOENT;
    return -1;
}


/* Wait for a child process. Minimal implementation: */
//int ___wait___( int *status )
int wait( int *status )
{
    errno = ECHILD;
    return -1;
}
int _wait( int *status )
{
    errno = ECHILD;
    return -1;
}


void outbyte( char b )
{
    RPI_AuxUartWrite( b );
}

/* Write to a file. libc subroutines will use this system routine for output to
   all files, including stdout—so if you need to generate any output, for
   example to a serial port for debugging, you should make your minimal write
   capable of doing this. The following minimal implementation is an
   incomplete example; it relies on a outbyte subroutine (not shown; typically,
   you must write this in assembler from examples provided by your hardware
   manufacturer) to actually perform the output. */
int _write( int file, char *ptr, int len )
{
    int todo;
    for( todo = 0; todo < len; todo++ )
      outbyte(*ptr++);
    return len;
}



int _gettimeofday( struct timeval *tv, void *tzvp )
{
    uint64_t t = RPI_GetSystemTimer()->counter_hi;
    t = t<<32;
    t = t+RPI_GetSystemTimer()->counter_lo;
    t = t*1000;  // get uptime in nanoseconds
    tv->tv_sec = t / 1000000000;  // convert to seconds
    tv->tv_usec = ( t % 1000000000 ) / 1000;  // get remaining microseconds
    
    return 0;  // return non-zero for error
} // end _gettimeofday()

uint64_t getSystemTimerNano()
{
    uint64_t t = RPI_GetSystemTimer()->counter_hi;
    t = t<<32;
    t = t+RPI_GetSystemTimer()->counter_lo;
    t = t*1000;  // get uptime in nanoseconds
  
}
