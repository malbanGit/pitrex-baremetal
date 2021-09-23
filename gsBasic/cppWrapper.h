#ifndef CPPWRAPPER_H
#define CPPWRAPPER_H

#define PACKED      __attribute__ ((packed))
#define ALIGN(n)    __attribute__ ((aligned (n)))
#define NORETURN    __attribute__ ((noreturn))
#define NOOPT       __attribute__ ((optimize (0)))
#define STDOPT      __attribute__ ((optimize (2)))
#define MAXOPT      __attribute__ ((optimize (3)))
#define WEAK        __attribute__ ((weak))



#endif
