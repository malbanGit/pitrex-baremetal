/*******************************************************************************
*																			   *
*	Define size independent data types and operations.						   *
*																			   *
*   The following types must be supported by all platforms:					   *
*																			   *
*	UINT8  - Unsigned 8-bit Integer		INT8  - Signed 8-bit integer           *
*	UINT16 - Unsigned 16-bit Integer	INT16 - Signed 16-bit integer          *
*	UINT32 - Unsigned 32-bit Integer	INT32 - Signed 32-bit integer          *
*	UINT64 - Unsigned 64-bit Integer	INT64 - Signed 64-bit integer          *
*																			   *
*																			   *
*   The macro names for the artithmatic operations are composed as follows:    *
*																			   *
*   XXX_R_A_B, where XXX - 3 letter operation code (ADD, SUB, etc.)			   *
*					 R   - The type	of the result							   *
*					 A   - The type of operand 1							   *
*			         B   - The type of operand 2 (if binary operation)		   *
*																			   *
*				     Each type is one of: U8,8,U16,16,U32,32,U64,64			   *
*																			   *
*******************************************************************************/

#pragma once

#ifndef OSD_CPU1_H
#define OSD_CPU1_H


#include <stdint.h>

#define INLINE static inline

#define ssize_t SSIZE_T
//#define uint32_t UINT32
//#define strncasecmp _strnicmp
//#define strcasecmp _stricmp


#ifndef INT64
typedef int64_t			INT64;
#endif // INT64
#ifndef UINT64
typedef uint64_t			UINT64;
#endif // UINT64


#ifndef UINT32
typedef uint32_t        UINT32;
#endif // UINT32

#ifndef INT32
typedef int32_t     INT32;
#endif // INT32

#ifndef UINT16
typedef uint16_t        UINT16;
#endif // UINT16

#ifndef UINT8
typedef uint8_t         UINT8;
#endif // UINT 8

#ifndef INT16
typedef int16_t     INT16;
#endif // INT16

#ifndef INT8
typedef int8_t          INT8;
#endif // INT8


#ifndef _MEMORYREADWRITEBYTE_
#define _MEMORYREADWRITEBYTE_

struct MemoryWriteByte
{
    UINT32 lowAddr;
    UINT32 highAddr;
    void (*memoryCall)(UINT32, UINT8, struct MemoryWriteByte *);
    void *pUserArea;
};      

struct MemoryReadByte
{
    UINT32 lowAddr;
    UINT32 highAddr;
    UINT8 (*memoryCall)(UINT32, struct MemoryReadByte *);
    void *pUserArea;
};      

#endif // _MEMORYREADWRITEBYTE_



/* Combine two 32-bit integers into a 64-bit integer */
#define COMBINE_64_32_32(A,B)     ((((UINT64)(A))<<32) | (UINT32)(B))
#define COMBINE_U64_U32_U32(A,B)  COMBINE_64_32_32(A,B)

/* Return upper 32 bits of a 64-bit integer */
#define HI32_32_64(A)		  (((UINT64)(A)) >> 32)
#define HI32_U32_U64(A)		  HI32_32_64(A)

/* Return lower 32 bits of a 64-bit integer */
#define LO32_32_64(A)		  ((A) & 0xffffffff)
#define LO32_U32_U64(A)		  LO32_32_64(A)

#define DIV_64_64_32(A,B)	  ((A)/(B))
#define DIV_U64_U64_U32(A,B)  ((A)/(UINT32)(B))

#define MOD_32_64_32(A,B)	  ((A)%(B))
#define MOD_U32_U64_U32(A,B)  ((A)%(UINT32)(B))

#define MUL_64_32_32(A,B)	  ((A)*(INT64)(B))
#define MUL_U64_U32_U32(A,B)  ((A)*(UINT64)(UINT32)(B))


/******************************************************************************
 * Union of UINT8, UINT16 and UINT32 in native endianess of the target
 * This is used to access bytes and words in a machine independent manner.
 * The upper bytes h2 and h3 normally contain zero (16 bit CPU cores)
 * thus PAIR.d can be used to pass arguments to the memory system
 * which expects 'int' really.
 ******************************************************************************/

// MALBAN
#define LSB_FIRST 1
typedef union {
#ifdef LSB_FIRST
	struct { UINT8 l,h,h2,h3; } b;
	struct { UINT16 l,h; } w;
#else
	struct { UINT8 h3,h2,h,l; } b;
	struct { UINT16 h,l; } w;
#endif
	UINT32 d;
}	PAIR;

#endif	/* defined OSD_CPU_H */
