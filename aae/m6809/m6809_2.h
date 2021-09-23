/*** m6809: Portable 6809 emulator ******************************************/

#ifndef _M6809_2_H
#define _M6809_2_H

#include "m6809.h"

/* PUBLIC FUNCTIONS */
void m6809_2_SetRegs(m6809_Regs *Regs);
void m6809_2_GetRegs(m6809_Regs *Regs);
unsigned m6809_2_GetPC(void);
void m6809_2_reset(void);
void m6809_2_execute(void);

/* PUBLIC GLOBALS */
extern int	m6809_2_IPeriod;
extern int	m6809_2_ICount;
extern int	m6809_2_IRequest;


/****************************************************************************/
/* Read a byte from given memory location                                   */
/****************************************************************************/
//int cpu_readmem_2(int address);
//#define M6809_2_RDMEM(A) ((unsigned)cpu_readmem_2(A))

int (**m6809_2_readHandler)(int);
#define M6809_2_RDMEM(A) ((unsigned)(m6809_2_readHandler[A](A)) )

/****************************************************************************/
/* Write a byte to given memory location                                    */
/****************************************************************************/
//void cpu_writemem_2(int address, int data);
//#define M6809_2_WRMEM(A,V) (cpu_writemem_2(A,V))

void (**m6809_2_writeHandler)(int,int);
#define M6809_2_WRMEM(A,V) ((void)(m6809_2_writeHandler[A](A,V)) )

/****************************************************************************/
/* Z80_RDOP() is identical to Z80_RDMEM() except it is used for reading     */
/* opcodes. In case of system with memory mapped I/O, this function can be  */
/* used to greatly speed up emulation                                       */
/****************************************************************************/
#define M6809_2_RDOP(A) (ROM[A])

/****************************************************************************/
/* Z80_RDOP_ARG() is identical to Z80_RDOP() except it is used for reading  */
/* opcode arguments. This difference can be used to support systems that    */
/* use different encoding mechanisms for opcodes and opcode arguments       */
/****************************************************************************/
/*#define Z80_RDOP_ARG(A)		Z80_RDOP(A)*/
#define M6809_2_RDOP_ARG(A) (RAM[A])

/****************************************************************************/
/* Flags for optimizing memory access. Game drivers should set m6809_Flags  */
/* to a combination of these flags depending on what can be safely          */
/* optimized. For example, if M6809_FAST_OP is set, opcodes are fetched     */
/* directly from the ROM array, and cpu_readmem() is not called.            */
/* The flags affect reads and writes.                                       */
/****************************************************************************/
extern int m6809_2_Flags;

#endif /* _M6809_2_H */
