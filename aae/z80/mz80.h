
/* General z80 based defines */

#ifdef NO

#ifndef	_MZ80_H_
#define	_MZ80_H_

#include "../osd_cpu.h"

#ifndef NO_PI
//; let us try little endian  check endian on ARM
#endif


// intel is LITTLE ENDIAN
#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#endif


struct z80PortWrite
{
	UINT16 lowIoAddr;
	UINT16 highIoAddr;
	void (*IOCall)(UINT16, UINT8, struct z80PortWrite *);
	void *pUserArea;
};

struct z80PortRead
{
	UINT16 lowIoAddr;
	UINT16 highIoAddr;
	UINT16 (*IOCall)(UINT16, struct z80PortRead *);
	void *pUserArea;
};	

struct z80TrapRec
{
  	UINT16 trapAddr;
	UINT8  skipCnt;
	UINT8  origIns;
};


// These are the enumerations used for register access. DO NOT ALTER THEIR
// ORDER! It must match the same order as in the mz80.c/mz80.asm files!



extern UINT32 mz80exec(UINT32);
extern UINT32 mz80GetContextSize(void);
extern UINT32 mz80GetElapsedTicks(UINT32);
extern void mz80ReleaseTimeslice(void);
extern void mz80GetContext(void *);
extern void mz80SetContext(void *);
extern void mz80reset(void);
extern void mz80ClearPendingInterrupt(void);
extern UINT32 mz80int(UINT32);
extern UINT32 mz80nmi(void);
extern void mz80init(void);
extern UINT32 z80intAddr;
extern UINT32 z80nmiAddr;

// Debugger useful routines

extern UINT8 mz80SetRegisterValue(void *, UINT32, UINT32);
extern UINT32 mz80GetRegisterValue(void *, UINT32);
extern UINT32 mz80GetRegisterTextValue(void *, UINT32, UINT8 *);
extern UINT8 *mz80GetRegisterName(UINT32);

// Memory/IO read/write commands

#ifndef VALUE_BYTE
#define	VALUE_BYTE	0
#endif

#ifndef VALUE_WORD
#define	VALUE_WORD	1
#endif

#ifndef VALUE_DWORD
#define	VALUE_DWORD	2
#endif

#ifndef VALUE_IO
#define	VALUE_IO	3
#endif

//extern void mz80WriteValue(UINT8 bWhat, UINT32 dwAddr, UINT32 dwData);
//extern UINT32 mz80ReadValue(UINT8 bWhat, UINT32 dwAddr);


typedef struct mz80context CONTEXTMZ80;




/*************************************
 *
 *  Interrupt line constants
 *
 *************************************/

enum
{
    /* line states */
    CLEAR_LINE = 0,             /* clear (a fired, held or pulsed) line */
    ASSERT_LINE,                /* assert an interrupt immediately */
    HOLD_LINE,                  /* hold interrupt line until acknowledged */
    PULSE_LINE,                 /* pulse interrupt line for one instruction */

    /* internal flags (not for use by drivers!) */
    INTERNAL_CLEAR_LINE = 100 + CLEAR_LINE,
    INTERNAL_ASSERT_LINE = 100 + ASSERT_LINE,

    /* input lines */
    MAX_INPUT_LINES = 32+3,
    INPUT_LINE_IRQ0 = 0,
    INPUT_LINE_IRQ1 = 1,
    INPUT_LINE_IRQ2 = 2,
    INPUT_LINE_IRQ3 = 3,
    INPUT_LINE_IRQ4 = 4,
    INPUT_LINE_IRQ5 = 5,
    INPUT_LINE_IRQ6 = 6,
    INPUT_LINE_IRQ7 = 7,
    INPUT_LINE_IRQ8 = 8,
    INPUT_LINE_IRQ9 = 9,
    INPUT_LINE_NMI = MAX_INPUT_LINES - 3,

    /* special input lines that are implemented in the core */
    INPUT_LINE_RESET = MAX_INPUT_LINES - 2,
    INPUT_LINE_HALT = MAX_INPUT_LINES - 1,

    /* output lines */
    MAX_OUTPUT_LINES = 32
};






enum {
    Z80_PC=1, Z80_SP,
    Z80_A, Z80_B, Z80_C, Z80_D, Z80_E, Z80_H, Z80_L,
    Z80_AF, Z80_BC, Z80_DE, Z80_HL,
    Z80_IX, Z80_IY, Z80_AF2, Z80_BC2, Z80_DE2, Z80_HL2,
    Z80_R, Z80_I, Z80_IM, Z80_IFF1, Z80_IFF2, Z80_HALT,
    Z80_DC0, Z80_DC1, Z80_DC2, Z80_DC3
};

enum {
    Z80_TABLE_op,
    Z80_TABLE_cb,
    Z80_TABLE_ed,
    Z80_TABLE_xy,
    Z80_TABLE_xycb,
    Z80_TABLE_ex    /* cycles counts for taken jr/jp/call and interrupt latency (rst opcodes) */
};
/*
enum
{
    CPUINFO_PTR_Z80_CYCLE_TABLE = CPUINFO_PTR_CPU_SPECIFIC,
    CPUINFO_PTR_Z80_CYCLE_TABLE_LAST = CPUINFO_PTR_Z80_CYCLE_TABLE + Z80_TABLE_ex
};
*/
#endif	// _MZ80_H_

#endif