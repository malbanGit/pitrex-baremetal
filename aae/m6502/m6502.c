

#ifdef ZERO_DIRECT
int active_CPU = 0;
#else
extern int active_CPU;
#endif

/* Multi-6502 CPU emulator
 *
 * Copyright (C) 1999, Brian Stern
 * 
 * Revision history:
 *      05 Feb 1999     1.0     BGS     Original version
 *
 * This 6502 CPU emulator may be distributed in unmodified form to any
 * medium.  It may not be sold, or used in commercial or shareware software
 * without written permission from the author (Brian Stern, bstern@synthcom.com).
 *
 * If you use this CPU emulator in a freeware application, please credit Brian 
 * Stern as the author in the application or documentation.
 *
 * Brian Stern will not be held responsible for any damage caused by the use
 * of this CPU emulator.  It is purely "as is".  
 * 
 * Every effort was made to make this emulator accurate.  However, if you find
 * a bug or have a suggestion, please contact the author so the appropriate 
 * changes can be made.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <ctype.h>
#include "m6502.h"




#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#ifdef BOOL
#undef BOOL
#endif

typedef enum
{ 
    FALSE = 0x0,
    TRUE  = 0x1
} BOOL;



#define HAVE_6502

#ifdef HAVE_6502

// Useful defines

#define C_FLAG      ((bRegP & C_FLAG_MASK) >> C_FLAG_BIT)
#define Z_FLAG      ((bRegP & Z_FLAG_MASK) >> Z_FLAG_BIT)
#define I_FLAG      ((bRegP & I_FLAG_MASK) >> I_FLAG_BIT)
#define D_FLAG      ((bRegP & D_FLAG_MASK) >> D_FLAG_BIT)
#define B_FLAG      ((bRegP & B_FLAG_MASK) >> B_FLAG_BIT)
#define V_FLAG      ((bRegP & V_FLAG_MASK) >> V_FLAG_BIT)
#define N_FLAG      ((bRegP & N_FLAG_MASK) >> N_FLAG_BIT)

#define PREG_MASKED_SET(bMask)      bRegP |= (bMask);
#define PREG_MASKED_CLEAR(bMask)    bRegP &= ~(bMask);

#define PREG_MASKED_SETCLEAR(wCond, bData)  \
{                                           \
    if ((wCond))                            \
        PREG_MASKED_SET((bData))            \
    else                                    \
        PREG_MASKED_CLEAR((bData))          \
}

#define GET_WORD_ADDR(wAddr)            (((UINT16) bMemoryBase[(wAddr) + 1] << 8) | bMemoryBase[(wAddr)])
#define GET_EXTERNAL_WORD_ADDR(wAddr)   (UINT16) (((UINT16) Get6502Memory((UINT16) ((wAddr + 1) << 8))) | (UINT16) Get6502Memory(wAddr))
#define GET_ZERO_PAGE_WORD_ADDR(wAddr)  (UINT16) (((UINT16) ZERO_PAGE_READ((wAddr) + 1) << 8) | (UINT16) ZERO_PAGE_READ((wAddr)))
//#define NORMALIZE_PC (UINT16) ((UINT32) pbFetchByte - (UINT32) pbLastBase)
#define NORMALIZE_PC (UINT16) (((int)((void *) pbFetchByte) - ((int)(void *) pbLastBase)))
#define PC_TO_FETCH(x)      pbFetchByte = (pbLastBase + x);
        
// Addressing mode macros

#define GET_IMMEDIATE_DATA(bData)   \
{                                   \
    bData = IMMEDIATE_BYTE;\
}                                   \

#define GET_ABSOLUTE_ADDR(wAddr)    \
{                                   \
    wAddr = IMMEDIATE_BYTE; \
    wAddr |= ((UINT16) IMMEDIATE_BYTE << 8); \
}

#define GET_RELATIVE_OFFSET(wOffset)    \
{                                       \
    wOffset = IMMEDIATE_BYTE;   \
    if (wOffset & 0x80)                 \
    {\
        wOffset |= 0xffffff80;\
    }\
}

#define GET_INDIRECT_ADDR(wAddr)                        \
{                                                       \
    wAddr = (UINT16) IMMEDIATE_BYTE; \
    wAddr |= ((UINT16) IMMEDIATE_BYTE << 8); \
    wAddr = GET_EXTERNAL_WORD_ADDR(wAddr);  \
}

#define GET_ABSOLUTE_X_ADDR(wAddr)      \
{                                       \
    wAddr = IMMEDIATE_BYTE; \
    wAddr |= ((UINT16) IMMEDIATE_BYTE << 8); \
    wAddr += bRegX; \
}

#define GET_ABSOLUTE_Y_ADDR(wAddr)      \
{                                       \
    wAddr = IMMEDIATE_BYTE; \
    wAddr |= ((UINT16) IMMEDIATE_BYTE << 8); \
    wAddr += bRegY; \
}

#define GET_ZERO_PAGE_ADDR(wAddr)   \
{                                   \
    wAddr = IMMEDIATE_BYTE; \
}

#define GET_ZERO_PAGE_X_ADDR(wAddr)             \
{                                               \
    wAddr = (IMMEDIATE_BYTE + bRegX) & 0xff;\
}

#define GET_ZERO_PAGE_Y_ADDR(wAddr)             \
{                                               \
    wAddr = (IMMEDIATE_BYTE + bRegY) & 0xff;\
}

#define GET_INDIRECT_X_ADDR(wAddr)                  \
{                                                   \
    bMacroTemp = (UINT8) ((UINT8) IMMEDIATE_BYTE + (UINT8) bRegX);          \
    wAddr = GET_ZERO_PAGE_WORD_ADDR(bMacroTemp);    \
}


#define GET_INDIRECT_Y_ADDR(wAddr)                          \
{                                                           \
    bMacroTemp = IMMEDIATE_BYTE;                            \
    wAddr = (UINT16) (GET_ZERO_PAGE_WORD_ADDR(bMacroTemp) + bRegY); \
}


// Opcode macros

#define ADC(bData)                                                                                      \
{                                                                                                       \
    if (0 == D_FLAG)                                                                                    \
    {                                                                                                   \
        wMacroTemp = bRegA + (bData) + C_FLAG;                                                          \
        PREG_MASKED_SETCLEAR(((~(bRegA ^ (bData))) & (bRegA ^ wMacroTemp) & 0x80) != 0, V_FLAG_MASK)    \
        PREG_MASKED_SETCLEAR((wMacroTemp & 0x100) != 0, C_FLAG_MASK)                                    \
        bRegA = wMacroTemp & 0xff;                                                                      \
    }                                                                                                   \
    else                                                                                                \
    {                                                                                                   \
        bMacroTemp = (bRegA & 0xf) + ((bData) & 0xf) + C_FLAG;                                          \
        bMacroTemp2 = (bRegA & 0xf0) + ((bData) & 0xf0);                                                \
                                                                                                        \
        if (bMacroTemp > 9)                                                                             \
        {                                                                                               \
            bMacroTemp2 += 0x10;                                                                        \
            bMacroTemp = (bMacroTemp + 6) & 0xf;                                                        \
        }                                                                                               \
                                                                                                        \
        PREG_MASKED_SETCLEAR(((~(bRegA ^ (bData))) & (bRegA ^ bMacroTemp2) & 0x80) != 0, V_FLAG_MASK)   \
                                                                                                        \
        if (bMacroTemp2 > 0x90)                                                                         \
        {                                                                                               \
            bMacroTemp2 = (bMacroTemp2 + 0x60) & 0xf0;                                                  \
            PREG_MASKED_SET(C_FLAG_MASK)                                                                \
        }                                                                                               \
        else                                                                                            \
            PREG_MASKED_CLEAR(C_FLAG_MASK)                                                              \
                                                                                                        \
        bRegA = bMacroTemp | bMacroTemp2;                                                               \
    }                                                                                                   \
                                                                                                        \
    PREG_SET_ZERO_SIGN(bRegA)                                                                           \
}

#define AND(bData)              \
{                               \
    bRegA &= (bData);           \
    PREG_SET_ZERO_SIGN(bRegA)   \
}

#define ASL(bData)                                  \
{                                                   \
    PREG_MASKED_SETCLEAR(bData & 0x80, C_FLAG_MASK) \
    bData <<= 1;                                    \
    PREG_SET_ZERO_SIGN(bData)                       \
}

#define BIT(bData)                                              \
{                                                               \
    PREG_MASKED_SETCLEAR((bData) & 0x80, N_FLAG_MASK)           \
    PREG_MASKED_SETCLEAR((bData) & 0x40, V_FLAG_MASK)           \
    PREG_MASKED_SETCLEAR(0 == ((bData) & bRegA), Z_FLAG_MASK)   \
}

#define CMP(bData)                  \
{                                   \
    wMacroTemp = bRegA - (bData);   \
    COMPARE_SET_FLAGS(wMacroTemp)   \
}

#define CPX(bData)                  \
{                                   \
    wMacroTemp = bRegX - (bData);   \
    COMPARE_SET_FLAGS(wMacroTemp)   \
}

#define CPY(bData)                  \
{                                   \
    wMacroTemp = bRegY - (bData);   \
    COMPARE_SET_FLAGS(wMacroTemp)   \
}

#define DEC(bData)              \
{                               \
    bData--;                    \
    PREG_SET_ZERO_SIGN(bData)   \
}

#define EOR(bData)              \
{                               \
    bRegA ^= (bData);           \
    PREG_SET_ZERO_SIGN(bRegA)   \
}

#define INC(bData)              \
{                               \
    bData++;                    \
    PREG_SET_ZERO_SIGN(bData)   \
}

#define LDA(bData)              \
{                               \
    bRegA = (bData);            \
    PREG_SET_ZERO_SIGN(bRegA)   \
}


#define LDX(bData)              \
{                               \
    bRegX = (bData);            \
    PREG_SET_ZERO_SIGN(bRegX)   \
}

#define LDY(bData)              \
{                               \
    bRegY = (bData);            \
    PREG_SET_ZERO_SIGN(bRegY)   \
}

#define LSR(bData)                                  \
{                                                   \
    PREG_MASKED_SETCLEAR(bData & 1, C_FLAG_MASK)    \
    bData >>= 1;                                    \
    PREG_SET_ZERO_SIGN(bData)                       \
}

#define ORA(bData)              \
{                               \
    bRegA |= (bData);           \
    PREG_SET_ZERO_SIGN(bRegA)   \
}   

#define ROL(bData)                                      \
{                                                       \
    bMacroTemp = C_FLAG;                                \
    PREG_MASKED_SETCLEAR(bData & 0x80, C_FLAG_MASK)     \
    bData <<= 1;                                        \
                                                        \
    if (bMacroTemp != 0)                                \
        bData |= 1;                                     \
                                                        \
    PREG_SET_ZERO_SIGN(bData)                           \
}

#define ROR(bData)                                  \
{                                                   \
    bMacroTemp = C_FLAG;                            \
    PREG_MASKED_SETCLEAR(bData & 1, C_FLAG_MASK)    \
    bData >>= 1;                                    \
                                                    \
    if (bMacroTemp != 0)                            \
        bData |= 0x80;                              \
                                                    \
    PREG_SET_ZERO_SIGN(bData)                       \
}


#define SBC(bData)                                                                              \
{                                                                                               \
    wMacroTemp = bRegA - (bData) - (C_FLAG ^ 1);                                                \
    PREG_MASKED_SETCLEAR(((bRegA ^ (bData)) & (bRegA ^ wMacroTemp) & 0x80) != 0, V_FLAG_MASK)   \
                                                                                                \
    if (0 == D_FLAG)                                                                            \
    {                                                                                           \
        bRegA = wMacroTemp & 0xff;                                                              \
    }                                                                                           \
    else                                                                                        \
    {                                                                                           \
        bMacroTemp = (bRegA & 0xf) - ((bData) & 0xf) - (C_FLAG ^ 1);                            \
        bMacroTemp2 = (bRegA >> 4) - ((bData) >> 4);                                            \
                                                                                                \
        if (bMacroTemp & 0xf0)                                                                  \
            bMacroTemp -= 6;                                                                    \
                                                                                                \
        if (bMacroTemp & 0x80)                                                                  \
            bMacroTemp2--;                                                                      \
                                                                                                \
        if (bMacroTemp2 & 0xf0)                                                                 \
            bMacroTemp2 -= 6;                                                                   \
                                                                                                \
        bRegA = ((bMacroTemp & 0xf) | ((bMacroTemp2 & 0xf) << 4)) & 0xff;                       \
    }                                                                                           \
                                                                                                \
    PREG_MASKED_SETCLEAR(0 == (wMacroTemp & 0x100), C_FLAG_MASK)                                \
    PREG_SET_ZERO_SIGN(bRegA)                                                                   \
}

// Utility macros

#define PREG_SET_ZERO_SIGN(bData)                   \
{                                                   \
    PREG_MASKED_CLEAR(N_FLAG_MASK | Z_FLAG_MASK)    \
    PREG_MASKED_SET(bZeroSignTable[(bData)])        \
}

#define COND_JUMP(wCond, bAddrOffset)   \
{                                       \
    if ((wCond))                        \
    {\
        pbFetchByte += (bAddrOffset);\
    }\
    else\
    {\
        dwCycles++;\
        dwElapsedTicks--;\
    }\
}

#define STACK_PUSH(bData)                                   \
{                                                           \
    bMemoryBase[M6502_STACK_BASE + (bRegS--)] = (bData);    \
}

#define STACK_POP(bData)                                \
{                                                       \
    bData = bMemoryBase[M6502_STACK_BASE + (++bRegS)];  \
}

#define COMPARE_SET_FLAGS(wSignedData)                                  \
{                                                                       \
    PREG_MASKED_SETCLEAR(0 == ((wSignedData) & 0x8000), C_FLAG_MASK)    \
    PREG_SET_ZERO_SIGN((wSignedData) & 0xff)                            \
}

// If ZERO_DIRECT is defined, accesses to the zero page do
// not go through the memory handlers, for performance reasons.
//
#ifdef ZERO_DIRECT
#define ZERO_PAGE_READ(bAddr)   (bMemoryBase[(UINT8) (bAddr)])
#define ZERO_PAGE_WRITE(bAddr, bData)   \
{                                       \
    bMemoryBase[(bAddr)] = (bData);     \
}                                       
#else // !ZERO_DIRECT
#define ZERO_PAGE_READ(bAddr)   (Get6502Memory((UINT16) (bAddr)))
#define ZERO_PAGE_WRITE(bAddr, bData)   \
{                                       \
    Set6502Memory((bAddr), (bData));    \
}
#endif // ZERO_DIRECT

#define HANDLE_IRQ()                \
{                                   \
    PC = NORMALIZE_PC; \
    STACK_PUSH(PC >> 8)             \
    STACK_PUSH(PC & 0xff)           \
    STACK_PUSH(bRegP)               \
    PREG_MASKED_SET(I_FLAG_MASK)    \
    PREG_MASKED_CLEAR(B_FLAG_MASK)  \
    PC = GET_WORD_ADDR(IRQ_VECTOR); \
    PC_TO_FETCH(PC); \
    bIRQPending = FALSE;            \
}


static UINT8 *pbLastBase = NULL;
static UINT8 *pbFetchByte = NULL;   // Here for program counter fetch
static INT32 sdwOffset = 0;             // Offset for relative instructions
static UINT8 bReleaseTimeslice = FALSE; // Indicates that the 6502 must terminate its exec loop
static UINT8 bTemp = 0;                 // Scratch variable (1 byte)
static UINT8 bTemp2 = 0;                // Scratch variable (1 byte)
static UINT8 bMacroTemp = 0;            // Scratch variable for macro expansion (1 byte)
static UINT8 bMacroTemp2 = 0;           // Scratch variable for macro expansion (1 byte)
static UINT8 bZeroSignTable[0x100];     // Pre-computed values for zero and sign flags
static UINT16 wTemp = 0;                // Scratch variable (2 bytes)
static UINT16 wMacroTemp = 0;           // Scratch variable for macro expansion (2 bytes)



static UINT32 dwElapsedTicks = 0;       // Cycles executed in the current timeslice
static struct MemoryReadByte *psMemReadInfo;
static struct MemoryWriteByte *psMemWriteInfo;


// Context variables
//
static UINT8 bRegA;             // The accumulator
static UINT8 bRegP;             // The P (flags) register
static UINT8 bRegX;             // The X index register
static UINT8 bRegY;             // The Y index register
static UINT8 bRegS;             // The stack pointer 
static UINT8 bIRQPending;       // Indicates if an interrupt is pending
static UINT8 *bMemoryBase;      // Pointer to base memory
static struct MemoryReadByte *ps6502MemoryRead;     // Pointer to memory read handler
static struct MemoryWriteByte *ps6502MemoryWrite;   // Pointer to memory write handler

// The program counter
//
#ifdef ZERO_DIRECT
UINT16 m6502zppc;
#define PC m6502zppc
#else
UINT16 m6502pc;
#define PC m6502pc
#endif

#define IMMEDIATE_BYTE  (*pbFetchByte++)

static UINT8 bTimingTable[0x100] =
{
    0x07, 0x06, 0x02, 0x02, 0x03, 0x03, 0x05, 0x02, 0x03, 0x02, 0x02, 0x02, 0x04, 0x04, 0x06, 0x02, 
    0x03, 0x05, 0x03, 0x02, 0x03, 0x04, 0x06, 0x02, 0x02, 0x04, 0x02, 0x02, 0x04, 0x04, 0x07, 0x02, 
    0x06, 0x06, 0x02, 0x02, 0x03, 0x03, 0x05, 0x02, 0x04, 0x02, 0x02, 0x02, 0x04, 0x04, 0x06, 0x02, 
    0x03, 0x05, 0x03, 0x02, 0x04, 0x04, 0x06, 0x02, 0x02, 0x04, 0x02, 0x02, 0x04, 0x04, 0x07, 0x02, 
    0x06, 0x06, 0x02, 0x02, 0x02, 0x03, 0x05, 0x02, 0x03, 0x02, 0x02, 0x02, 0x03, 0x04, 0x06, 0x02, 
    0x03, 0x05, 0x03, 0x02, 0x02, 0x04, 0x06, 0x02, 0x02, 0x04, 0x03, 0x02, 0x02, 0x04, 0x07, 0x02, 
    0x06, 0x06, 0x02, 0x02, 0x03, 0x03, 0x05, 0x02, 0x04, 0x02, 0x02, 0x02, 0x05, 0x04, 0x06, 0x02, 
    0x03, 0x05, 0x03, 0x02, 0x04, 0x04, 0x06, 0x02, 0x02, 0x04, 0x04, 0x02, 0x06, 0x04, 0x07, 0x02, 
    0x02, 0x06, 0x02, 0x02, 0x03, 0x03, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0x04, 0x04, 0x02, 
    0x03, 0x06, 0x03, 0x02, 0x04, 0x04, 0x04, 0x02, 0x02, 0x05, 0x02, 0x02, 0x04, 0x05, 0x05, 0x02, 
    0x02, 0x06, 0x02, 0x02, 0x03, 0x03, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0x04, 0x04, 0x02, 
    0x03, 0x05, 0x03, 0x02, 0x04, 0x04, 0x04, 0x02, 0x02, 0x04, 0x02, 0x02, 0x04, 0x04, 0x04, 0x02, 
    0x02, 0x06, 0x02, 0x02, 0x03, 0x03, 0x05, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0x04, 0x06, 0x02, 
    0x03, 0x05, 0x03, 0x02, 0x02, 0x04, 0x06, 0x02, 0x02, 0x04, 0x03, 0x02, 0x02, 0x04, 0x06, 0x02, 
    0x02, 0x06, 0x02, 0x02, 0x03, 0x03, 0x05, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0x04, 0x06, 0x02, 
    0x03, 0x05, 0x03, 0x02, 0x02, 0x04, 0x06, 0x02, 0x02, 0x04, 0x04, 0x02, 0x02, 0x04, 0x07, 0x02
};


// Generic memory byte read/write handlers.

static UINT8 Get6502Memory(UINT16 wAddr)
{
    psMemReadInfo = ps6502MemoryRead;
    
    while ((psMemReadInfo->lowAddr & 0xffffffff) != 0xffffffff )
    {
        if ((wAddr >= psMemReadInfo->lowAddr) && (wAddr <= psMemReadInfo->highAddr))
        {
            PC = NORMALIZE_PC;
            return psMemReadInfo->memoryCall(wAddr, psMemReadInfo);
        }
            
        psMemReadInfo++;
    }
     
    return bMemoryBase[wAddr];
}

static void Set6502Memory(UINT16 wAddr, UINT8 bData)
{
  psMemWriteInfo = ps6502MemoryWrite;
    
    while ( (psMemWriteInfo->lowAddr & 0xffffffff) != 0xffffffff )
    {
        if ((wAddr >= psMemWriteInfo->lowAddr) && (wAddr <= psMemWriteInfo->highAddr))
        {

          PC = NORMALIZE_PC;
          psMemWriteInfo->memoryCall(wAddr, bData, psMemWriteInfo);
            return;
        }
        
        psMemWriteInfo++;
    }
    
    bMemoryBase[wAddr] = bData;
}


#ifdef ZERO_DIRECT
void m6502zpreset(void)
#else
void m6502reset(void)
#endif
{
    bRegA = 0;
    bRegX = 0;
    bRegY = 0;
    bRegP = 0x22;   // Zero flag and unused bit are set
    bRegS = 0xff;   // Stack starts at top and builds down

    PC = GET_WORD_ADDR(RESET_VECTOR);
}                       


#ifdef ZERO_DIRECT
void m6502zpinit(void)
#else
void m6502init(void)
#endif
{
    UINT16 wCount;
    printf ("init 6502");
    // Pre-compute the zero and sign flag values for all possible values
    // of a byte
    //
    for (wCount = 0; wCount < 0x100; wCount++)
        bZeroSignTable[wCount] = wCount & N_FLAG_MASK;

    bZeroSignTable[0] |= Z_FLAG_MASK; 
}

#include "../log.h"

static long long int allcyc = 0;

#ifdef ZERO_DIRECT
UINT32 m6502zpexec(UINT32 dwCycles)
#else
UINT32 m6502exec(UINT32 dwCycles)
#endif
{
  UINT8 bOpcode = 0;
  UINT8 bOpcodeCycles = 0;
  UINT32 mycycles;
  pbFetchByte = bMemoryBase + PC;
  pbLastBase = bMemoryBase;

//printf("Supposed to run for %i cycles: ", dwCycles);  
#ifndef SINGLE_STEP 
 mycycles = 0;
  while ((0 == (dwCycles & 0x80000000)) && (FALSE == bReleaseTimeslice))
#endif
  {
 /*
//(*pbFetchByte++)
 if (allcyc>=1160000)
 {
     char buf[100];
     int A = bRegA;
     sprintf(buf,"$%04x: [%i] ", NORMALIZE_PC, (int)allcyc);
//     sprintf(buf,"A=$%02x $%04x: [%i] ",A, NORMALIZE_PC, allcyc);
  //   log_it("%s\n",buf);
 }

 allcyc++;
 */

 bOpcode = IMMEDIATE_BYTE;
//printf("%02x, x:%02x \n", bOpcode, (UINT8) bRegX);
    bOpcodeCycles = bTimingTable[bOpcode];
    mycycles+=bTimingTable[bOpcode];
    dwCycles -= bOpcodeCycles;
    dwElapsedTicks += bOpcodeCycles;

    switch (bOpcode)
    {
      case 0x00:    // BRK
          PC = NORMALIZE_PC;
          PC++;
          PREG_MASKED_SET(B_FLAG_MASK)
          STACK_PUSH(PC >> 8)
          STACK_PUSH(PC & 0xff)
          STACK_PUSH(bRegP)             
          PREG_MASKED_SET(I_FLAG_MASK)
          PC = GET_WORD_ADDR(IRQ_VECTOR);
          PC_TO_FETCH(PC);
          break;
      
      case 0x01:    // ORA ($addr8,X)
          GET_INDIRECT_X_ADDR(wTemp)
          ORA(Get6502Memory(wTemp)) 
          break;
          
      case 0x05:    // ORA $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          ORA(ZERO_PAGE_READ(bTemp))
          break;
      
      case 0x06:    // ASL $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          ASL(bTemp2)
          ZERO_PAGE_WRITE(bTemp, bTemp2);
          break;
          
      case 0x08:    // PHP
          STACK_PUSH(bRegP)
          break;
      
      case 0x09:    // ORA #$data
          GET_IMMEDIATE_DATA(bTemp)
          ORA(bTemp)
          break;
      
      case 0x0a:    // ASL A
          ASL(bRegA)
          break;
          
      case 0x0d:    // ORA $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          ORA(Get6502Memory(wTemp))
          break;
      
      case 0x0e:    // ASL $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          ASL(bTemp)
          Set6502Memory(wTemp, bTemp);
          break;
      
      case 0x10:    // BPL
          GET_RELATIVE_OFFSET(sdwOffset)
          COND_JUMP(0 == N_FLAG, sdwOffset)
          break;
      
      case 0x11:    // ORA ($addr8),Y
          GET_INDIRECT_Y_ADDR(wTemp)
          ORA(Get6502Memory(wTemp))
          break;
      
      case 0x15:    // ORA $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          ORA(ZERO_PAGE_READ(bTemp))
          break;
      
      case 0x16:    // ASL $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          ASL(bTemp2)
          ZERO_PAGE_WRITE(bTemp, bTemp2)
          break;
      
      case 0x18:    // CLC
          PREG_MASKED_CLEAR(C_FLAG_MASK)
          break;
      
      case 0x19:    // ORA $addr16,Y
          GET_ABSOLUTE_Y_ADDR(wTemp)
          ORA(Get6502Memory(wTemp))
          break;
      
      case 0x1d:    // ORA $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          ORA(Get6502Memory(wTemp))
          break;
      
      case 0x1e:    // ASL $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          ASL(bTemp)
          Set6502Memory(wTemp, bTemp);
          break;
      
      case 0x20:    // JSR
          PC = NORMALIZE_PC;
          wTemp = GET_WORD_ADDR(PC);
          PC++;                         // JSR pushes the PC of 1 byte before the next instruction
          STACK_PUSH(PC >> 8);
          STACK_PUSH(PC & 0xff);
          PC_TO_FETCH(wTemp);
    
          break;
      
      case 0x21:    // AND ($addr,X)
          GET_INDIRECT_X_ADDR(wTemp)
          AND(Get6502Memory(wTemp))
          break;
          
      case 0x24:    // BIT $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          BIT(bTemp2)
          break;

      case 0x25:    // AND $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          AND(ZERO_PAGE_READ(bTemp))
          break;
          
      case 0x26:    // ROL $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          ROL(bTemp2)
          ZERO_PAGE_WRITE(bTemp, bTemp2);
          break;
      
      case 0x28:    // PLP
          STACK_POP(bRegP)
          break;
      
      case 0x29:    // AND #$data
          GET_IMMEDIATE_DATA(bTemp)
          AND(bTemp)
          break;
          
      case 0x2a:    // ROL A
          ROL(bRegA)
          break;
      
      case 0x2c:    // BIT $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          BIT(bTemp)
          break;
      
      case 0x2d:    // AND $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          AND(Get6502Memory(wTemp))
          break;
      
      case 0x2e:    // ROL $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          ROL(bTemp)
          Set6502Memory(wTemp, bTemp);
          break;
      
      case 0x30:    // BMI
          GET_RELATIVE_OFFSET(sdwOffset)
          COND_JUMP(N_FLAG != 0, sdwOffset)
          break;
      
      case 0x31:    // AND ($addr),Y
          GET_INDIRECT_Y_ADDR(wTemp)
          AND(Get6502Memory(wTemp))
          break;
      
      case 0x35:    // AND $addr,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          AND(ZERO_PAGE_READ(bTemp))
          break;
      
      case 0x36:    // ROL $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          ROL(bTemp2)
          ZERO_PAGE_WRITE(bTemp, bTemp2)
          break;
      
      case 0x38:    // SEC
          PREG_MASKED_SET(C_FLAG_MASK)
          break;
      
      case 0x39:    // AND $addr16,Y
          GET_ABSOLUTE_Y_ADDR(wTemp)
          AND(Get6502Memory(wTemp))
          break;
      
      case 0x3d:    // AND $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          AND(Get6502Memory(wTemp))
          break;
      
      case 0x3e:    // ROL $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          ROL(bTemp)
          Set6502Memory(wTemp, bTemp);
          break;
      
      case 0x40:    // RTI
          STACK_POP(bRegP)
          STACK_POP(bTemp)
          STACK_POP(bTemp2)
          PC = ((UINT16) bTemp2 << 8) | bTemp;
          PC_TO_FETCH(PC);
          if ((0 == I_FLAG) && (bIRQPending != FALSE))
              HANDLE_IRQ()
          break;
      
      case 0x41:    // EOR ($addr8,X)
          GET_INDIRECT_X_ADDR(wTemp)
          EOR(Get6502Memory(wTemp))
          break;
      
      case 0x45:    // EOR $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          EOR(ZERO_PAGE_READ(bTemp))
          break;
      
      case 0x46:    // LSR $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          LSR(bTemp2)
          ZERO_PAGE_WRITE(bTemp, bTemp2)
          break;

      case 0x48:    // PHA
          STACK_PUSH(bRegA)
          break;
      
      case 0x49:    // EOR #$data
          GET_IMMEDIATE_DATA(bTemp)
          EOR(bTemp)
          break;
      
      case 0x4a:    // LSR A
          LSR(bRegA)
          break;
      
      case 0x4c:    // JMP $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          PC = wTemp;
          PC_TO_FETCH(PC);
          break;
          
      case 0x4d:    // EOR $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          EOR(Get6502Memory(wTemp))
          break;

      case 0x4e:    // LSR $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          LSR(bTemp)
          Set6502Memory(wTemp, bTemp);
          break;
      
      case 0x50:    // BVC
          GET_RELATIVE_OFFSET(sdwOffset)
          COND_JUMP(0 == V_FLAG, sdwOffset)
          break;
      
      case 0x51:    // EOR ($addr8),Y
          GET_INDIRECT_Y_ADDR(wTemp)
          EOR(Get6502Memory(wTemp))
          break;
          
      case 0x55:    // EOR $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          EOR(ZERO_PAGE_READ(bTemp))
          break;
      
      case 0x56:    // LSR $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          LSR(bTemp2)
          ZERO_PAGE_WRITE(bTemp, bTemp2)
          break;
      
      case 0x58:    // CLI
          PREG_MASKED_CLEAR(I_FLAG_MASK)
          
          if (bIRQPending != FALSE)
              HANDLE_IRQ()
              
          break;
      
      case 0x59:    // EOR $addr16,Y
          GET_ABSOLUTE_Y_ADDR(wTemp)
          EOR(Get6502Memory(wTemp))
          break;
      
      case 0x5d:    // EOR $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          EOR(Get6502Memory(wTemp))
          break;
      
      case 0x5e:    // LSR $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          LSR(bTemp)
          Set6502Memory(wTemp, bTemp);
          break;
      
      case 0x60:    // RTS
          STACK_POP(bTemp)
          STACK_POP(bTemp2)
          PC = (((UINT16) bTemp2 << 8) | bTemp) + 1;    // PC on stack was 1 byte before next instruction
          PC_TO_FETCH(PC);
          break;
      
      case 0x61:    // ADC ($addr8,X)
          GET_INDIRECT_X_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          ADC(bTemp)
          break;
      
      case 0x65:    // ADC $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          ADC(bTemp2)
          break;
      
      case 0x66:    // ROR $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          ROR(bTemp2)
          ZERO_PAGE_WRITE(bTemp, bTemp2)
          break;
      
      case 0x68:    // PLA
          STACK_POP(bRegA)
          PREG_SET_ZERO_SIGN(bRegA)
          break;
      
      case 0x69:    // ADC #$data
          GET_IMMEDIATE_DATA(bTemp)
          ADC(bTemp)
          break;
      
      case 0x6a:    // ROR A
          ROR(bRegA)
          break;
      
      case 0x6c:    // JMP ($addr16)
          GET_INDIRECT_ADDR(wTemp)
          PC = wTemp;
          PC_TO_FETCH(PC);
          break;
      
      case 0x6d:    // ADC $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          ADC(bTemp)
          break;
      
      case 0x6e:    // ROR $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          ROR(bTemp)
          Set6502Memory(wTemp, bTemp);
          break;
      
      case 0x70:    // BVS
          GET_RELATIVE_OFFSET(sdwOffset)
          COND_JUMP(V_FLAG != 0, sdwOffset)
          break;
      
      case 0x71:    // ADC ($addr8),Y
          GET_INDIRECT_Y_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          ADC(bTemp)
          break;
      
      case 0x75:    // ADC $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          ADC(bTemp2)
          break;
      
      case 0x76:    // ROR $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          ROR(bTemp2)
          ZERO_PAGE_WRITE(bTemp, bTemp2)
          break;
      
      case 0x78:    // SEI
          PREG_MASKED_SET(I_FLAG_MASK)
          break;
      
      case 0x79:    // ADC $addr16,Y
          GET_ABSOLUTE_Y_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          ADC(bTemp)
          break;
      
      case 0x7d:    // ADC $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          ADC(bTemp)
          break;
      
      case 0x7e:    // ROR $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          ROR(bTemp)
          Set6502Memory(wTemp, bTemp);
          break;
      
      case 0x81:    // STA ($addr8,X)
          GET_INDIRECT_X_ADDR(wTemp)
          Set6502Memory(wTemp, bRegA);
          break;

      case 0x84:    // STY $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          ZERO_PAGE_WRITE(bTemp, bRegY)
          break;
      
      case 0x85:    // STA $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          ZERO_PAGE_WRITE(bTemp, bRegA)
          break;
          
      case 0x86:    // STX $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          ZERO_PAGE_WRITE(bTemp, bRegX)
          break;
      
      case 0x88:    // DEY
          bRegY--;
          PREG_SET_ZERO_SIGN(bRegY)
          break;
      
      case 0x8a:    // TXA
          bRegA = bRegX;
          PREG_SET_ZERO_SIGN(bRegA)
          break;
      
      case 0x8c:    // STY $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          Set6502Memory(wTemp, bRegY);
          break;
      
      case 0x8d:    // STA $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          Set6502Memory(wTemp, bRegA);
          break;
      
      case 0x8e:    // STX $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          Set6502Memory(wTemp, bRegX);
          break;
      
      case 0x90:    // BCC
          GET_RELATIVE_OFFSET(sdwOffset)
          COND_JUMP(0 == C_FLAG, sdwOffset)
          break;
      
      case 0x91:    // STA ($addr8),Y
          GET_INDIRECT_Y_ADDR(wTemp)
          Set6502Memory(wTemp, bRegA);
          break;
          
      case 0x94:    // STY $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          ZERO_PAGE_WRITE(bTemp, bRegY)
          break;
      
      case 0x95:    // STA $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          ZERO_PAGE_WRITE(bTemp, bRegA)
          break;
          
      case 0x96:    // STX $addr8,Y
          GET_ZERO_PAGE_Y_ADDR(bTemp)
          ZERO_PAGE_WRITE(bTemp, bRegX)
          break;
      
      case 0x98:    // TYA
          bRegA = bRegY;
          PREG_SET_ZERO_SIGN(bRegA)
          break;
      
      case 0x99:    // STA $addr16,Y
          GET_ABSOLUTE_Y_ADDR(wTemp)
          Set6502Memory(wTemp, bRegA);
          break;
      
      case 0x9a:    // TXS
          bRegS = bRegX;
          break;
      
      case 0x9d:    // STA $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          Set6502Memory(wTemp, bRegA);
          break;
      case 0xa0:    // LDY #$data
          GET_IMMEDIATE_DATA(bTemp)
          LDY(bTemp)
          break;

      case 0xa1:    // LDA ($addr8,X)
          GET_INDIRECT_X_ADDR(wTemp)
          LDA(Get6502Memory(wTemp))
          break;
      
      case 0xa2:    // LDX #$data
          GET_IMMEDIATE_DATA(bTemp)
          LDX(bTemp)
          break;
          
      case 0xa4:    // LDY $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          LDY(ZERO_PAGE_READ(bTemp))
          break;
      
      case 0xa5:    // LDA $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          LDA(ZERO_PAGE_READ(bTemp))
          break;
      
      case 0xa6:    // LDX $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          LDX(ZERO_PAGE_READ(bTemp))
          break;
      
      case 0xa8:    // TAY
          bRegY = bRegA;
          PREG_SET_ZERO_SIGN(bRegY)
          break;
      
      case 0xa9:    // LDA #$data
          GET_IMMEDIATE_DATA(bTemp)
          LDA(bTemp)
          break;
      
      case 0xaa:    // TAX
          bRegX = bRegA;
          PREG_SET_ZERO_SIGN(bRegX)
          break;
      
      case 0xac:    // LDY $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          LDY(Get6502Memory(wTemp))
          break;
      
      case 0xad:    // LDA $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          LDA(Get6502Memory(wTemp))
          break;
      
      case 0xae:    // LDX $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          LDX(Get6502Memory(wTemp))
          break;

      case 0xb0:    // BCS
          GET_RELATIVE_OFFSET(sdwOffset)
          COND_JUMP(C_FLAG != 0, sdwOffset)
          break;
      
      case 0xb1:    // LDA ($addr8),Y
          GET_INDIRECT_Y_ADDR(wTemp)
          LDA(Get6502Memory(wTemp))
          break;
      
      case 0xb4:    // LDY $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          LDY(ZERO_PAGE_READ(bTemp))
          break;
      
      case 0xb5:    // LDA $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          LDA(ZERO_PAGE_READ(bTemp))
          break;

      case 0xb6:    // LDX $addr8,Y
          GET_ZERO_PAGE_Y_ADDR(bTemp)
          LDX(ZERO_PAGE_READ(bTemp))
          break;
          
      case 0xb8:    // CLV
          PREG_MASKED_CLEAR(V_FLAG_MASK)
          break;
      
      case 0xb9:    // LDA $addr16,Y
          GET_ABSOLUTE_Y_ADDR(wTemp)
          LDA(Get6502Memory(wTemp))
          break;
      
      case 0xba:    // TSX
          bRegX = bRegS;
          PREG_SET_ZERO_SIGN(bRegX)
          break;
      
      case 0xbc:    // LDY $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          LDY(Get6502Memory(wTemp))
          break;
      
      case 0xbd:    // LDA $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          LDA(Get6502Memory(wTemp))
          break;
      
      case 0xbe:    // LDX $addr16,Y
          GET_ABSOLUTE_Y_ADDR(wTemp)
          LDX(Get6502Memory(wTemp))
          break;
      
      case 0xc0:    // CPY #$data
          GET_IMMEDIATE_DATA(bTemp)
          CPY(bTemp)
          break;
      
      case 0xc1:    // CMP ($addr8,X)
          GET_INDIRECT_X_ADDR(wTemp)
          CMP(Get6502Memory(wTemp))
          break;
      
      case 0xc4:    // CPY $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          CPY(ZERO_PAGE_READ(bTemp))
          break;
      
      case 0xc5:    // CMP $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          CMP(ZERO_PAGE_READ(bTemp))
          break;
      
      case 0xc6:    // DEC $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          DEC(bTemp2)
          ZERO_PAGE_WRITE(bTemp, bTemp2)
          break;
      
      case 0xc8:    // INY
          bRegY++;
          PREG_SET_ZERO_SIGN(bRegY)
          break;
      
      case 0xc9:    // CMP #$data
          GET_IMMEDIATE_DATA(bTemp)
          CMP(bTemp)
          break;

      case 0xca:    // DEX
          bRegX--;
          PREG_SET_ZERO_SIGN(bRegX)
          break;
          
      case 0xcc:    // CPY $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          CPY(Get6502Memory(wTemp))
          break;
      
      case 0xcd:    // CMP $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          CMP(Get6502Memory(wTemp))
          break;

      case 0xce:    // DEC $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          DEC(bTemp)
          Set6502Memory(wTemp, bTemp);
          break;
      
      case 0xd0:    // BNE
          GET_RELATIVE_OFFSET(sdwOffset)
          COND_JUMP(0 == Z_FLAG, sdwOffset)             
          break;
      
      case 0xd1:    // CMP ($addr8),Y
          GET_INDIRECT_Y_ADDR(wTemp)
          CMP(Get6502Memory(wTemp))
          break;
      
      case 0xd5:    // CMP $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          CMP(ZERO_PAGE_READ(bTemp))
          break;
      
      case 0xd6:    // DEC $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          DEC(bTemp2)
          ZERO_PAGE_WRITE(bTemp, bTemp2)
          break;
          
      case 0xd8:    // CLD
          PREG_MASKED_CLEAR(D_FLAG_MASK)
          break;
      
      case 0xd9:    // CMP $addr16,Y
          GET_ABSOLUTE_Y_ADDR(wTemp)
          CMP(Get6502Memory(wTemp))
          break;
      
      case 0xdd:    // CMP $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          CMP(Get6502Memory(wTemp))
          break;
      
      case 0xde:    // DEC $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          DEC(bTemp)
          Set6502Memory(wTemp, bTemp);
          break;
      
      case 0xe0:    // CPX #$data
          GET_IMMEDIATE_DATA(bTemp)
          CPX(bTemp)
          break;
      
      case 0xe1:    // SBC ($addr8,X)
          GET_INDIRECT_X_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          SBC(bTemp)
          break;
      
      case 0xe4:    // CPX $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          CPX(ZERO_PAGE_READ(bTemp))
          break;
      
      case 0xe5:    // SBC $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          SBC(bTemp2)
          break;
      
      case 0xe6:    // INC $addr8
          GET_ZERO_PAGE_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          INC(bTemp2)
          ZERO_PAGE_WRITE(bTemp, bTemp2)
          break;
      
      case 0xe8:    // INX
          bRegX++;
          PREG_SET_ZERO_SIGN(bRegX)
          break;
      
      case 0xe9:    // SBC #$data
          GET_IMMEDIATE_DATA(bTemp)
          SBC(bTemp)
          break;
      
      case 0xea:    // NOP
          break;
      
      case 0xec:    // CPX $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          CPX(Get6502Memory(wTemp))
          break;
      
      case 0xed:    // SBC $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          SBC(bTemp)
          break;
      
      case 0xee:    // INC $addr16
          GET_ABSOLUTE_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          INC(bTemp)
          Set6502Memory(wTemp, bTemp);
          break;
      
      case 0xf0:    // BEQ
          GET_RELATIVE_OFFSET(sdwOffset)
          COND_JUMP(Z_FLAG != 0, sdwOffset)
          break;
      
      case 0xf1:    // SBC ($addr8),Y
          GET_INDIRECT_Y_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          SBC(bTemp);
          break;
      
      case 0xf5:    // SBC $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          SBC(bTemp2)
          break;
      
      case 0xf6:    // INC $addr8,X
          GET_ZERO_PAGE_X_ADDR(bTemp)
          bTemp2 = ZERO_PAGE_READ(bTemp);
          INC(bTemp2)
          ZERO_PAGE_WRITE(bTemp, bTemp2)
          break;
      
      case 0xf8:    // SED
          PREG_MASKED_SET(D_FLAG_MASK)
          break;

      case 0xf9:    // SBC $addr16,Y
          GET_ABSOLUTE_Y_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          SBC(bTemp)
          break;
      
      case 0xfd:    // SBC $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          SBC(bTemp)
          break;
      
      case 0xfe:    // INC $addr16,X
          GET_ABSOLUTE_X_ADDR(wTemp)
          bTemp = Get6502Memory(wTemp);
          INC(bTemp)
          Set6502Memory(wTemp, bTemp);
          break;
      
      default:
          PC = NORMALIZE_PC;
          return PC - 1;
    }
  }

  PC = NORMALIZE_PC;

  bReleaseTimeslice = FALSE;

//printf("Exit Execute\n");
  return 0x80000000;
}


#ifdef ZERO_DIRECT
UINT32 m6502zpint(UINT32 dwIntNumber)
#else
UINT32 m6502int(UINT32 dwIntNumber)
#endif
{
    if (I_FLAG != 0)
    {
        // Interrupts are disabled
        bIRQPending = TRUE;
        return 0xffffffff;
    }
    else
    {
        HANDLE_IRQ()
    
      return 0;
    }
}

#ifdef ZERO_DIRECT
UINT32 m6502zpnmi(void)
#else
UINT32 m6502nmi(void)
#endif
{
    STACK_PUSH(PC >> 8)             
    STACK_PUSH(PC & 0xff)           
    STACK_PUSH(bRegP)               
    PREG_MASKED_SET(I_FLAG_MASK)    
    PREG_MASKED_CLEAR(B_FLAG_MASK)  
    PC = GET_WORD_ADDR(NMI_VECTOR); 
    return 0;
}


#ifdef ZERO_DIRECT
UINT32 m6502zpGetContextSize(void)
#else
UINT32 m6502GetContextSize(void)
#endif
{
    return sizeof(CONTEXTM6502);
}


#ifdef ZERO_DIRECT
void m6502zpSetContext(void *pNew)
#else
void m6502SetContext(void *pNew)
#endif
{
    CONTEXTM6502 *pNewContext = pNew;
    bMemoryBase         = pNewContext->m6502Base;



    ps6502MemoryWrite   = pNewContext->m6502MemoryWrite;
    ps6502MemoryRead    = pNewContext->m6502MemoryRead;
    bRegA               = pNewContext->m6502af & 0xff;
    bRegP               = pNewContext->m6502af >> 8;
    bRegX               = pNewContext->m6502x;
    bRegY               = pNewContext->m6502y;
    bRegS               = pNewContext->m6502s;
    PC                  = pNewContext->m6502pc;
    bIRQPending         = pNewContext->irqPending;
    dwElapsedTicks = pNewContext->m6502dwElapsedTicks;

pbLastBase = bMemoryBase;
pbFetchByte = bMemoryBase + PC;
  
    
}


#ifdef ZERO_DIRECT
void m6502zpGetContext(void *pOld)
#else
void m6502GetContext(void *pOld)
#endif
{
    CONTEXTM6502 *pOldContext = pOld;
    pOldContext->m6502Base          = bMemoryBase;
    pOldContext->m6502MemoryWrite   = ps6502MemoryWrite;
    pOldContext->m6502MemoryRead    = ps6502MemoryRead;
    pOldContext->m6502af            = ((UINT16) bRegP << 8) | bRegA;
    pOldContext->m6502x             = bRegX;
    pOldContext->m6502y             = bRegY;
    pOldContext->m6502s             = bRegS;
    pOldContext->m6502pc            = PC;
    pOldContext->irqPending         = bIRQPending;
    pOldContext->m6502dwElapsedTicks            = dwElapsedTicks;
    
    
    
}


#ifdef ZERO_DIRECT
UINT32 m6502zpGetElapsedTicks(UINT32 dwClear)
#else
UINT32 m6502GetElapsedTicks(UINT32 dwClear)
#endif
{
    UINT32 dwTemp;

    dwTemp = dwElapsedTicks;

    if (dwClear)
        dwElapsedTicks = 0;

    return dwTemp;
}

#ifdef ZERO_DIRECT
void m6502zpReleaseTimeslice(void)
#else
void m6502ReleaseTimeslice(void)
#endif
{
    bReleaseTimeslice = TRUE;
}

#endif // HAVE_6502