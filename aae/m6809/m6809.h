/*** m6809: Portable 6809 emulator ******************************************/

#ifndef _M6809_H
#define _M6809_H


/****************************************************************************/
/* sizeof(byte)=1, sizeof(word)=2, sizeof(dword)>=4                         */
/****************************************************************************/
//#include "..\types.h"	/* -NS- */

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned      dword;
typedef signed char offset;

/* 6809 Registers */
typedef struct
{
	word		pc;		/* Program counter */
	word		u, s;	/* Stack pointers */
	word		x, y;	/* Index registers */
	byte		dp;		/* Direct Page register */
	byte		a, b;	/* Accumulator */
	byte		cc;
} m6809_Regs;

#ifndef INLINE
#define INLINE  __inline
#endif


#define INT_NONE  0			/* No interrupt required */
#define INT_IRQ	  1 		/* Standard IRQ interrupt */
#define INT_FIRQ  2			/* Fast IRQ */


extern byte *RAM;
extern byte *ROM;

#define M6809_FAST_NONE	0x00	/* no memory optimizations */
#define M6809_FAST_OP	0x01	/* opcode fetching */
#define M6809_FAST_S	0x02	/* stack */
#define M6809_FAST_U	0x04	/* user stack */

#ifndef FALSE
#    define FALSE 0
#endif
#ifndef TRUE
#    define TRUE (!FALSE)
#endif

#endif /* _M6809_H */
