/* Multi-6502 32 Bit emulator */

/* Copyright 1996, 1997, 1998, Neil Bradley, All rights reserved
 *
 * License agreement:
 *
 * (M6502 Refers to both the assembly code emitted by make6502.c and make6502.c
 * itself)
 *
 * M6502 May be distributed in unmodified form to any medium.
 *
 * M6502 May not be sold, or sold as a part of a commercial package without
 * the express written permission of Neil Bradley (neil@synthcom.com). This
 * includes shareware.
 *
 * Modified versions of M6502 may not be publicly redistributed without author
 * approval (neil@synthcom.com). This includes distributing via a publicly
 * accessible LAN. You may make your own source modifications and distribute
 * M6502 in source or object form, but if you make modifications to M6502
 * then it should be noted in the top as a comment in make6502.c.
 *
 * M6502 Licensing for commercial applications is available. Please email
 * neil@synthcom.com for details.
 *
 * Synthcom Systems, Inc, and Neil Bradley will not be held responsible for
 * any damage done by the use of M6502. It is purely "as-is".
 *
 * If you use M6502 in a freeware application, credit in the following text:
 *
 * "Multi-6502 CPU emulator by Neil Bradley (neil@synthcom.com)"
 *
 * must accompany the freeware application within the application itself or
 * in the documentation.
 *
 * Legal stuff aside:
 *
 * If you find problems with M6502, please email the author so they can get
 * resolved. If you find a bug and fix it, please also email the author so
 * that those bug fixes can be propogated to the installed base of M6502
 * users. If you find performance improvements or problems with M6502, please
 * email the author with your changes/suggestions and they will be rolled in
 * with subsequent releases of M6502.
 *
 * The whole idea of this emulator is to have the fastest available 32 bit
 * Multi-6502 emulator for the PC, giving maximum performance. 
 */ 

/* General 6502 related goodies */

#ifndef _M6502_H_
#define _M6502_H_

#include "../osd_cpu.h"

// Bit locations in the P register (bit 5 is unused)
//
#define C_FLAG_BIT		0	// Carry flag
#define Z_FLAG_BIT		1	// Zero flag
#define I_FLAG_BIT		2	// Interrupt flag
#define D_FLAG_BIT		3	// Decimal flag
#define B_FLAG_BIT		4	// Break flag
#define V_FLAG_BIT		6	// Overflow flag
#define N_FLAG_BIT		7	// Sign (negative) flag

#define C_FLAG_MASK		0x01
#define Z_FLAG_MASK		0x02
#define I_FLAG_MASK		0x04
#define D_FLAG_MASK		0x08
#define B_FLAG_MASK		0x10
#define V_FLAG_MASK		0x40
#define N_FLAG_MASK		0x80

#define NMI_VECTOR		0xfffa
#define RESET_VECTOR	0xfffc
#define IRQ_VECTOR		0xfffe

#define M6502_STACK_BASE	0x100

extern UINT32 m6502nmi(void);
extern UINT32 m6502int(UINT32);
extern UINT32 m6502exec(UINT32);
extern void m6502reset(void);
extern UINT32 m6502GetElapsedTicks(UINT32);
extern void m6502ReleaseTimeslice();

extern UINT32 m6502zpnmi(void);
extern UINT32 m6502zpint(UINT32);
extern UINT32 m6502zpexec(UINT32);
extern void m6502zpreset(void);
extern UINT32 m6502zpGetElapsedTicks(UINT32);
extern void m6502zpReleaseTimeslice();

extern UINT32 m6502bsnmi(void);
extern UINT32 m6502bsint(UINT32);
extern UINT32 m6502bsexec(UINT32);
extern void m6502bsreset(void);
extern UINT32 m6502bsGetElapsedTicks(UINT32);
extern void m6502bsReleaseTimeslice();

extern UINT16 m6502bspc;
extern UINT16 m6502zppc;
extern UINT16 m6502pc;


/* WARNING: Do not use a sizeof() operator on this structure. The assembly
 * code will pack other context relevant data after this space. Use the
 * GetContextSize() call to obtain the specifics of the context.
 */

struct m6502context
{
	UINT8 *m6502Base;                               /* 32 Bit pointer to base memory address */

	/* 32 Bit pointers to user trap address structure array */

	struct MemoryReadByte *m6502MemoryRead;
	struct MemoryWriteByte *m6502MemoryWrite;

	/* This context's register dump */

	UINT16 m6502af;                 /* A & Flags */
	UINT16 m6502pc;                 /* Program counter */
	UINT8 m6502x;                   /* X register */
	UINT8 m6502y;                   /* Y register */
	UINT8 m6502s;                   /* S register */
	UINT8 irqPending;
	UINT32 m6502dwElapsedTicks;
};

struct m6502bscontext
{
	UINT8 *m6502Base;                               /* 32 Bit pointer to base memory address */

	/* 32 Bit pointers to user trap address structure array */

	struct MemoryReadByte *m6502MemoryRead;
	struct MemoryWriteByte *m6502MemoryWrite;

	/* Base address and page registers */

	UINT32 dwBaseAddr;
	UINT32 dwBasePage;

	UINT8 *pbBankSwitch[32];		/* 32 2K addresses */

	/* This context's register dump */

	UINT16 m6502af;                 /* A & Flags */
	UINT16 m6502pc;                 /* Program counter */
	UINT8 m6502x;                   /* X register */
	UINT8 m6502y;                   /* Y register */
	UINT8 m6502s;                   /* S register */
	UINT8 irqPending;
} ;

typedef struct m6502context CONTEXTM6502;
typedef struct m6502bscontext CONTEXTM6502BS;

extern void m6502SetContext(void *);
extern void m6502GetContext(void *);
extern UINT32 m6502GetContextSize(void);

extern void m6502zpSetContext(void *);
extern void m6502zpGetContext(void *);
extern UINT32 m6502zpGetContextSize(void);

extern void m6502bsSetContext(void *);
extern void m6502bsGetContext(void *);
extern UINT32 m6502bsGetContextSize(void);

extern void m6502init(void);
extern void m6502zpinit(void);
extern void m6502bsinit(void);

extern UINT8 *m6502bsBanks[32];	// Bank switching registers

#endif // _M6502_H_
