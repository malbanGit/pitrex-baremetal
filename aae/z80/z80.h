#ifdef MAME_Z80


#ifndef _Z80_H_
#define _Z80_H_

#include "../osd_cpu.h"
#include "../cpuintrf.h"


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


/****************************************************************************/
/* The Z80 registers. HALT is set to 1 when the CPU is halted, the refresh  */
/* register is calculated as follows: refresh=(Z80.r&127)|(Z80.r2&128)      */
/****************************************************************************/
struct mz80context
{
    UINT32 dwElapsedTicks;

    UINT8 *z80Base;
    struct MemoryReadByte *z80MemRead;
    struct MemoryWriteByte *z80MemWrite;
    struct z80PortRead *z80IoRead;
    struct z80PortWrite *z80IoWrite;
  
  
  
    PAIR    prvpc,pc,sp,af,bc,de,hl,ix,iy;
    PAIR    af2,bc2,de2,hl2;
    UINT8   r,r2,iff1,iff2,halt,im,i;
    UINT8   nmi_state;          /* nmi line state */
    UINT8   irq_state;          /* irq line state */
//    const struct z80_irq_daisy_chain *daisy;
    int     (*irq_callback)(int irqline);
    int     extra_cycles;       /* extra cycles for interrupts */
    UINT32  z80intAddr;
    UINT32  z80nmiAddr;
};

typedef struct mz80context Z80_Regs;

typedef struct mz80context CONTEXTMZ80;


enum {
    Z80_PC=1, Z80_SP,
    Z80_A, Z80_B, Z80_C, Z80_D, Z80_E, Z80_H, Z80_L,
    Z80_AF, Z80_BC, Z80_DE, Z80_HL,
    Z80_IX, Z80_IY, Z80_AF2, Z80_BC2, Z80_DE2, Z80_HL2,
    Z80_R, Z80_I, Z80_IM, Z80_IFF1, Z80_IFF2, Z80_HALT,
    Z80_DC0, Z80_DC1, Z80_DC2, Z80_DC3
};
#define z80pc Z80_PC



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
//extern void z80_get_info(UINT32 state, union cpuinfo *info);

#ifdef MAME_DEBUG
extern unsigned z80_dasm(char *buffer, offs_t pc, UINT8 *oprom, UINT8 *opram, int bytes);
#endif

void z80_reset(void);
#define mz80reset z80_reset

void z80_init(void);
#define mz80init z80_init

int z80_execute(int cycles);
#define mz80exec z80_execute

UINT32 mz80int(UINT32 irq_vector);
void mz80nmi(void);


void z80_get_context (void *dst);
void z80_set_context (void *src);
#define mz80GetContext z80_get_context
#define mz80SetContext z80_set_context


UINT32 mz80GetElapsedTicks(UINT32 dwClear);
#ifndef CPUREG_PC
#define CPUREG_PC 0
#endif
extern UINT32 mz80GetRegisterValue(void *, UINT32);
#endif

#else
#include "../mz80/mz80.h"
#endif
