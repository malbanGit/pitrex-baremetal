#ifndef _V32BASIC_H_
#define _V32BASIC_H_

#ifdef _MSC_VER

#define strcasecmp _stricmp

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stddef.h>

#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif

#else

#define __debugbreak() __asm__ volatile (" sdbbp 0")
#endif

#ifdef __DEBUG
#define _DEBUG
#endif

#define MAX_TOKEN_LEN 32 // This is also the maximum variable name length
#define MAX_ARRAY_DIMENSIONS 3 // The maximum number of dimensions in a BASIC array
#define MAX_LINE_LENGTH 200

#define countof(x) (sizeof(x) / sizeof((x)[0]))

#define STRINGIZE(x) __STRINGIZE(x)
#define __STRINGIZE(x) #x

#define null 0

#ifdef __cplusplus
extern "C"
#endif
void OutOfMemoryHandler();

#endif
