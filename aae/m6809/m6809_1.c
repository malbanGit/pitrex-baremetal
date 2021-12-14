/*** m6809_1_1: Portable 6809 emulator ******************************************

	Copyright (C) John Butler 1997

	References:

		6809 Simulator V09, By L.C. Benschop, Eidnhoven The Netherlands.

		m6809_1_1: Portable 6809 emulator, DS (6809 code in MAME, derived from
			the 6809 Simulator V09)

		6809 Microcomputer Programming & Interfacing with Experiments"
			by Andrew C. Staugaard, Jr.; Howard W. Sams & Co., Inc.

	System dependencies:	word must be 16 bit unsigned int
							byte must be 8 bit unsigned int
							long must be more than 16 bits
							arrays up to 65536 bytes must be supported
							machine must be twos complement

*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "m6809_1.h"

void (**m6809_1_writeHandler)(int,int);
int (**m6809_1_readHandler)(int);

 
int cpu_interrupt_1(void);
static void m6809_1_FIRQ( void );

/* public globals */
int	m6809_1_IPeriod=50000;
int	m6809_1_ICount=50000;
int	m6809_1_IRequest=INT_NONE;
int m6809_1_Flags;	/* flags for speed optimization */

/* these are re-defined in m6809_1.h TO RAM, ROM or functions in cpuintrf.c */
#define M_RDMEM(A)      M6809_1_RDMEM(A)
#define M_WRMEM(A,V)    M6809_1_WRMEM(A,V)
#define M_RDOP(A)       M6809_1_RDOP(A)
#define M_RDOP_ARG(A)   M6809_1_RDOP_ARG(A)

#define INLINE_STATIC static __inline
INLINE_STATIC void fetch_effective_address( void );

/* 6809 registers */
static byte cc,dpreg,areg,breg;
static word xreg,yreg,ureg,sreg,pcreg;

static word eaddr; /* effective address */

/* flag, handlers for speed optimization */
static int fastopcodes;
static int (*rd_op_handler)();
static int (*rd_op_handler_wd)();
static int (*rd_u_handler)();
static int (*rd_u_handler_wd)();
static int (*rd_s_handler)();
static int (*rd_s_handler_wd)();
static void (*wr_u_handler)();
static void (*wr_u_handler_wd)();
static void (*wr_s_handler)();
static void (*wr_s_handler_wd)();


/* macros to access memory */
#define IMMBYTE(b)	{b=(*rd_op_handler)(pcreg);pcreg++;}
#define IMMWORD(w)	{w=(*rd_op_handler_wd)(pcreg);pcreg+=2;}
#define PUSHBYTE(b) {--sreg;(*wr_s_handler)(sreg,b);}
#define PUSHWORD(w) {sreg-=2;(*wr_s_handler_wd)(sreg,w);}
#define PULLBYTE(b) {b=(*rd_s_handler)(sreg);sreg++;}
#define PULLWORD(w) {w=(*rd_s_handler_wd)(sreg);sreg+=2;}
#define PSHUBYTE(b) {--ureg;(*wr_u_handler)(ureg,b);}
#define PSHUWORD(w) {ureg-=2;(*wr_u_handler_wd)(ureg,w);}
#define PULUBYTE(b) {b=(*rd_u_handler)(ureg);ureg++;}
#define PULUWORD(w) {w=(*rd_u_handler_wd)(ureg);ureg+=2;}

/* CC masks						  H  NZVC
								7654 3210	*/
#define CLR_HNZVC	cc&=0xd0
#define CLR_NZV		cc&=0xf1
#define CLR_HNZC	cc&=0xd2
#define CLR_NZVC	cc&=0xf0
#define CLR_Z		cc&=0xfb
#define CLR_NZC		cc&=0xf2
#define CLR_ZC		cc&=0xfa

/* macros for CC -- CC bits affected should be reset before calling */
#define SET_Z(a)		if(!a)SEZ
#define SET_Z8(a)		SET_Z((byte)a)
#define SET_Z16(a)		SET_Z((word)a)
#define SET_N8(a)		cc|=((a&0x80)>>4)
#define SET_N16(a)		cc|=((a&0x8000)>>12)
#define SET_H(a,b,r)	cc|=(((a^b^r)&0x10)<<1)
#define SET_C8(a)		cc|=((a&0x100)>>8)
#define SET_C16(a)		cc|=((a&0x10000)>>16)
#define SET_V8(a,b,r)	cc|=(((a^b^r^(r>>1))&0x80)>>6)
#define SET_V16(a,b,r)	cc|=(((a^b^r^(r>>1))&0x8000)>>14)
/* combos */
#define SET_NZ8(a)			{SET_N8(a);SET_Z(a);}
#define SET_NZ16(a)			{SET_N16(a);SET_Z(a);}
#define SET_FLAGS8(a,b,r)	{SET_N8(r);SET_Z8(r);SET_V8(a,b,r);SET_C8(r);}
#define SET_FLAGS16(a,b,r)	{SET_N16(r);SET_Z16(r);SET_V16(a,b,r);SET_C16(r);}

/* for treating an unsigned byte as a signed word */
#define SIGNED(b) ((word)(b&0x80?b|0xff00:b))

/* macros to access dreg */
#define GETDREG ((areg<<8)|breg)
#define SETDREG(n) {areg=(n)>>8;breg=(n);}

/* macros for addressing modes (postbytes have their own code) */
#define DIRECT {IMMBYTE(eaddr);eaddr|=(dpreg<<8);}
#define IMM8 eaddr=pcreg++
#define IMM16 {eaddr=pcreg;pcreg+=2;}
#define EXTENDED IMMWORD(eaddr)

/* macros to set status flags */
#define SEC cc|=0x01
#define CLC cc&=0xfe
#define SEZ cc|=0x04
#define CLZ cc&=0xfb
#define SEN cc|=0x08
#define CLN cc&=0xf7
#define SEV cc|=0x02
#define CLV cc&=0xfd
#define SEH cc|=0x20
#define CLH cc&=0xdf

/* macros for convenience */
#define DIRBYTE(b) {DIRECT;b=M_RDMEM(eaddr);}
#define DIRWORD(w) {DIRECT;w=M_RDMEM_WORD(eaddr);}
#define EXTBYTE(b) {EXTENDED;b=M_RDMEM(eaddr);}
#define EXTWORD(w) {EXTENDED;w=M_RDMEM_WORD(eaddr);}

/* macros for branch instructions */
#define BRANCH(f) {IMMBYTE(t);if(f)pcreg+=SIGNED(t);}
#define LBRANCH(f) {IMMWORD(t);if(f)pcreg+=t;}
#define NXORV  ((cc&0x08)^((cc&0x02)<<2))

/* macros for setting/getting registers in TFR/EXG instructions */
#define GETREG(val,reg) switch(reg) {\
                         case 0: val=GETDREG;break;\
                         case 1: val=xreg;break;\
                         case 2: val=yreg;break;\
                    	 case 3: val=ureg;break;\
                    	 case 4: val=sreg;break;\
                    	 case 5: val=pcreg;break;\
                    	 case 8: val=areg;break;\
                    	 case 9: val=breg;break;\
                    	 case 10: val=cc;break;\
                    	 case 11: val=dpreg;break;}

#define SETREG(val,reg) switch(reg) {\
			 case 0: SETDREG(val); break;\
			 case 1: xreg=val;break;\
			 case 2: yreg=val;break;\
			 case 3: ureg=val;break;\
			 case 4: sreg=val;break;\
			 case 5: pcreg=val;break;\
			 case 8: areg=val;break;\
			 case 9: breg=val;break;\
			 case 10: cc=val;break;\
			 case 11: dpreg=val;break;}


static unsigned char haspostbyte[] = {
  /*0*/      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  /*1*/      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  /*2*/      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  /*3*/      1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
  /*4*/      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  /*5*/      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  /*6*/      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /*7*/      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  /*8*/      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  /*9*/      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  /*A*/      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /*B*/      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  /*C*/      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  /*D*/      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  /*E*/      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /*F*/      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            };

/* timings for 1-byte opcodes */
static unsigned char cycles[] =
{
		/*	0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
  /*0*/		6, 0, 0, 6, 6, 0, 6, 6, 6, 6, 6, 0, 6, 6, 3, 6,
  /*1*/		0, 0, 2, 2, 0, 0, 5, 9, 0, 2, 3, 0, 3, 2, 8, 7,
  /*2*/		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  /*3*/		4, 4, 4, 4, 5, 5, 5, 5, 0, 5, 3, 6, 0,11, 0,19,
  /*4*/		2, 0, 0, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2, 2, 0, 2,
  /*5*/		2, 0, 0, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2, 2, 0, 2,
  /*6*/		6, 0, 0, 6, 6, 0, 6, 6, 6, 6, 6, 0, 6, 6, 3, 6,
  /*7*/		7, 0, 0, 7, 7, 0, 7, 7, 7, 7, 7, 0, 7, 7, 4, 7,
  /*8*/		2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 2, 4, 7, 3, 0,
  /*9*/		4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 4, 6, 7, 5, 5,
  /*A*/		4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 4, 6, 7, 5, 5,
  /*B*/		5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 5, 7, 8, 6, 6,
  /*C*/		2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 2, 3, 0, 3, 3,
  /*D*/		4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5,
  /*E*/		4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5,
  /*F*/		5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6
};

/* timings for 2-byte opcodes */
static unsigned char cycles2[] =
{
		/*	0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
  /*0*/		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /*1*/		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /*2*/		0, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  /*3*/		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,20,
  /*4*/		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /*5*/		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /*6*/		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /*7*/		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /*8*/		0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 4, 0,
  /*9*/		0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 6, 6,
  /*A*/		0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 6, 6,
  /*B*/		0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 7, 7,
  /*C*/		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0,
  /*D*/		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6,
  /*E*/		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6,
  /*F*/		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7
};

//////////////////////////////////////////////////////////////////////////



INLINE_STATIC unsigned M_RDMEM_WORD (dword A)
{
	int i;

    i = M_RDMEM(A)<<8;
    i |= M_RDMEM(((A)+1)&0xFFFF);
	return i;
}

INLINE_STATIC void M_WRMEM_WORD (dword A,word V)
{
	M_WRMEM (A,V>>8);
	M_WRMEM (((A)+1)&0xFFFF,V&255);
}
/*

HNZVC

? = undefined
* = affected
- = unaffected
0 = cleared
1 = set
# = ccr directly affected by instruction
@ = special - carry set if bit 7 is set

*/

INLINE_STATIC void illegal( void )
{
//	if(errorlog)fprintf(errorlog, "M6809: illegal opcode\n");
}


/* $00 NEG direct ?**** */
INLINE_STATIC void neg_di( void )
{
	word r,t;
	DIRBYTE(t); r=-t;
	CLR_NZVC; SET_FLAGS8(0,t,r);
	M_WRMEM(eaddr,r);
}

/* $01 ILLEGAL */

/* $02 ILLEGAL */

/* $03 COM direct -**01 */
INLINE_STATIC void com_di( void )
{
	byte t;
	DIRBYTE(t); t = ~t;
	CLR_NZV; SET_NZ8(t); SEC;
	M_WRMEM(eaddr,t);
}

/* $04 LSR direct -0*-* */
INLINE_STATIC void lsr_di( void )
{
	byte t;
	DIRBYTE(t); CLR_NZC; cc|=(t&0x01);
	t>>=1; SET_Z8(t);
	M_WRMEM(eaddr,t);
}

/* $05 ILLEGAL */

/* $06 ROR direct -**-* */
INLINE_STATIC void ror_di( void )
{
	byte t,r;
	DIRBYTE(t); r=(cc&0x01)<<7;
	CLR_NZC; cc|=(t&0x01);
	r |= t>>1; SET_NZ8(r);
	M_WRMEM(eaddr,r);
}

/* $07 ASR direct ?**-* */
INLINE_STATIC void asr_di( void )
{
	byte t;
	DIRBYTE(t); CLR_NZC; cc|=(t&0x01);
	t>>=1; t|=((t&0x40)<<1);
	SET_NZ8(t);
	M_WRMEM(eaddr,t);
}

/* $08 ASL direct ?**** */
INLINE_STATIC void asl_di( void )
{
	word t,r;
	DIRBYTE(t); r=t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	M_WRMEM(eaddr,r);
}

/* $09 ROL direct -**** */
INLINE_STATIC void rol_di( void )
{
	word t,r;
	DIRBYTE(t); r = cc&0x01; r |= t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	M_WRMEM(eaddr,r);
}

/* $0A DEC direct -***- */
INLINE_STATIC void dec_di( void )
{
	byte t;
	DIRBYTE(t); --t;
	CLR_NZV; if(t==0x7F) SEV; SET_NZ8(t);
	M_WRMEM(eaddr,t);
}

/* $0B ILLEGAL */

/* $OC INC direct -***- */
INLINE_STATIC void inc_di( void )
{
	byte t;
	DIRBYTE(t); ++t;
	CLR_NZV; if(t==0x80) SEV; SET_NZ8(t);
	M_WRMEM(eaddr,t);
}

/* $OD TST direct -**0- */
INLINE_STATIC void tst_di( void )
{
	byte t;
	DIRBYTE(t); CLR_NZV; SET_NZ8(t);
}

/* $0E JMP direct ----- */
INLINE_STATIC void jmp_di( void )
{
	DIRECT; pcreg=eaddr;
}

/* $0F CLR direct -0100 */
INLINE_STATIC void clr_di( void )
{
	DIRECT; M_WRMEM(eaddr,0);
	CLR_NZVC; SEZ;
}

#if macintosh
#pragma mark ____1x____
#endif

/* $10 FLAG */

/* $11 FLAG */

/* $12 NOP inherent ----- */
INLINE_STATIC void nop( void )
{
	;
}

/* $13 SYNC inherent ----- */
INLINE_STATIC void sync( void )
{
	/* SYNC should stop processing instructions until an interrupt occurs.
	   A decent fake is probably to force an immediate IRQ. */
	m6809_1_ICount = 0;
}

/* $14 ILLEGAL */

/* $15 ILLEGAL */

/* $16 LBRA relative ----- */
INLINE_STATIC void lbra( void )
{
	IMMWORD(eaddr); pcreg+=eaddr;
}

/* $17 LBSR relative ----- */
INLINE_STATIC void lbsr( void )
{
	IMMWORD(eaddr); PUSHWORD(pcreg); pcreg+=eaddr;
}

/* $18 ILLEGAL */

#if 0
/* $19 DAA inherent (areg) -**0* */
INLINE_STATIC void daa( void )
{
	byte msn, lsn;
	word t, cf = 0;
	msn=areg & 0xf0; lsn=areg & 0x0f;
	if( lsn>0x09 || cc&0x20 ) cf |= 0x06;
	if( msn>0x80 && lsn>0x09 ) cf |= 0x60;
	if( msn>0x90 || cc&0x01 ) cf |= 0x60;
	t = cf + areg;
	CLR_NZV; /* keep carry from previous operation */
	SET_NZ8(t); SET_C8(t);
	areg = t;
}
#else
/* $19 DAA inherent (areg) -**0* */
INLINE_STATIC void daa( void )
{
	word t;
	t=areg;
	if (cc&0x20) t+=0x06;
	if ((t&0x0f)>9) t+=0x06;
	if (cc&0x01) t+=0x60;
	if ((t&0xf0)>0x90) t+=0x60;
	if (t&0x100) SEC;
	areg=t;
}
#endif

/* $1A ORCC immediate ##### */
INLINE_STATIC void orcc( void )
{
	byte t;
	IMMBYTE(t); cc|=t;
}

/* $1B ILLEGAL */

/* $1C ANDCC immediate ##### */
INLINE_STATIC void andcc( void )
{
	byte t;
	IMMBYTE(t); cc&=t;
}

/* $1D SEX inherent -**0- */
INLINE_STATIC void sex( void )
{
	word t;
	t = SIGNED(breg); SETDREG(t);
	CLR_NZV; SET_NZ16(t);
}

/* $1E EXG inherent ----- */
INLINE_STATIC void exg( void )
{
	word t1=0,t2=0;
	byte tb;

	IMMBYTE(tb); GETREG(t1,tb>>4); GETREG(t2,tb&15);
	SETREG(t2,tb>>4); SETREG(t1,tb&15);
}

/* $1F TFR inherent ----- */
INLINE_STATIC void tfr( void )
{
	byte tb;
	word t=0;

	IMMBYTE(tb); GETREG(t,tb>>4); SETREG(t,tb&15);
}

#if macintosh
#pragma mark ____2x____
#endif

/* $20 BRA relative ----- */
INLINE_STATIC void bra( void )
{
	byte t;
	BRANCH(1);
	/* JB 970823 - speed up busy loops */
	if (t==0xfe) m6809_1_ICount = 0;
}

/* $21 BRN relative ----- */
INLINE_STATIC void brn( void )
{
	byte t;
	BRANCH(0);
}

/* $1021 LBRN relative ----- */
INLINE_STATIC void lbrn( void )
{
	word t;
	LBRANCH(0);
}

/* $22 BHI relative ----- */
INLINE_STATIC void bhi( void )
{
	byte t;
	BRANCH(!(cc&0x05));
}

/* $1022 LBHI relative ----- */
INLINE_STATIC void lbhi( void )
{
	word t;
	LBRANCH(!(cc&0x05));
}

/* $23 BLS relative ----- */
INLINE_STATIC void bls( void )
{
	byte t;
	BRANCH(cc&0x05);
}

/* $1023 LBLS relative ----- */
INLINE_STATIC void lbls( void )
{
	word t;
	LBRANCH(cc&0x05);
}

/* $24 BCC relative ----- */
INLINE_STATIC void bcc( void )
{
	byte t;
	BRANCH(!(cc&0x01));
}

/* $1024 LBCC relative ----- */
INLINE_STATIC void lbcc( void )
{
	word t;
	LBRANCH(!(cc&0x01));
}

/* $25 BCS relative ----- */
INLINE_STATIC void bcs( void )
{
	byte t;
	BRANCH(cc&0x01);
}

/* $1025 LBCS relative ----- */
INLINE_STATIC void lbcs( void )
{
	word t;
	LBRANCH(cc&0x01);
}

/* $26 BNE relative ----- */
INLINE_STATIC void bne( void )
{
	byte t;
	BRANCH(!(cc&0x04));
}

/* $1026 LBNE relative ----- */
INLINE_STATIC void lbne( void )
{
	word t;
	LBRANCH(!(cc&0x04));
}

/* $27 BEQ relative ----- */
INLINE_STATIC void beq( void )
{
	byte t;
	BRANCH(cc&0x04);
}

/* $1027 LBEQ relative ----- */
INLINE_STATIC void lbeq( void )
{
	word t;
	LBRANCH(cc&0x04);
}

/* $28 BVC relative ----- */
INLINE_STATIC void bvc( void )
{
	byte t;
	BRANCH(!(cc&0x02));
}

/* $1028 LBVC relative ----- */
INLINE_STATIC void lbvc( void )
{
	word t;
	LBRANCH(!(cc&0x02));
}

/* $29 BVS relative ----- */
INLINE_STATIC void bvs( void )
{
	byte t;
	BRANCH(cc&0x02);
}

/* $1029 LBVS relative ----- */
INLINE_STATIC void lbvs( void )
{
	word t;
	LBRANCH(cc&0x02);
}

/* $2A BPL relative ----- */
INLINE_STATIC void bpl( void )
{
	byte t;
	BRANCH(!(cc&0x08));
}

/* $102A LBPL relative ----- */
INLINE_STATIC void lbpl( void )
{
	word t;
	LBRANCH(!(cc&0x08));
}

/* $2B BMI relative ----- */
INLINE_STATIC void bmi( void )
{
	byte t;
	BRANCH(cc&0x08);
}

/* $102B LBMI relative ----- */
INLINE_STATIC void lbmi( void )
{
	word t;
	LBRANCH(cc&0x08);
}

/* $2C BGE relative ----- */
INLINE_STATIC void bge( void )
{
	byte t;
	BRANCH(!NXORV);
}

/* $102C LBGE relative ----- */
INLINE_STATIC void lbge( void )
{
	word t;
	LBRANCH(!NXORV);
}

/* $2D BLT relative ----- */
INLINE_STATIC void blt( void )
{
	byte t;
	BRANCH(NXORV);
}

/* $102D LBLT relative ----- */
INLINE_STATIC void lblt( void )
{
	word t;
	LBRANCH(NXORV);
}

/* $2E BGT relative ----- */
INLINE_STATIC void bgt( void )
{
	byte t;
	BRANCH(!(NXORV||cc&0x04));
}

/* $102E LBGT relative ----- */
INLINE_STATIC void lbgt( void )
{
	word t;
	LBRANCH(!(NXORV||cc&0x04));
}

/* $2F BLE relative ----- */
INLINE_STATIC void ble( void )
{
	byte t;
	BRANCH(NXORV||cc&0x04);
}

/* $102F LBLE relative ----- */
INLINE_STATIC void lble( void )
{
	word t;
	LBRANCH(NXORV||cc&0x04);
}

#if macintosh
#pragma mark ____3x____
#endif

/* $30 LEAX indexed --*-- */
INLINE_STATIC void leax( void )
{
	xreg=eaddr; CLR_Z; SET_Z(xreg);
}

/* $31 LEAY indexed --*-- */
INLINE_STATIC void leay( void )
{
	yreg=eaddr; CLR_Z; SET_Z(yreg);
}

/* $32 LEAS indexed ----- */
INLINE_STATIC void leas( void )
{
	sreg=eaddr;
}

/* $33 LEAU indexed ----- */
INLINE_STATIC void leau( void )
{
	ureg=eaddr;
}

/* $34 PSHS inherent ----- */
INLINE_STATIC void pshs( void )
{
	byte t;
	IMMBYTE(t);
	if(t&0x80) PUSHWORD(pcreg);
	if(t&0x40) PUSHWORD(ureg);
	if(t&0x20) PUSHWORD(yreg);
	if(t&0x10) PUSHWORD(xreg);
	if(t&0x08) PUSHBYTE(dpreg);
	if(t&0x04) PUSHBYTE(breg);
	if(t&0x02) PUSHBYTE(areg);
	if(t&0x01) PUSHBYTE(cc);
}

/* 35 PULS inherent ----- */
INLINE_STATIC void puls( void )
{
	byte t;
	IMMBYTE(t);
	if(t&0x01) PULLBYTE(cc);
	if(t&0x02) PULLBYTE(areg);
	if(t&0x04) PULLBYTE(breg);
	if(t&0x08) PULLBYTE(dpreg);
	if(t&0x10) PULLWORD(xreg);
	if(t&0x20) PULLWORD(yreg);
	if(t&0x40) PULLWORD(ureg);
	if(t&0x80) PULLWORD(pcreg);
}

/* $36 PSHU inherent ----- */
INLINE_STATIC void pshu( void )
{
	byte t;
	IMMBYTE(t);
	if(t&0x80) PSHUWORD(pcreg);
	if(t&0x40) PSHUWORD(sreg);
	if(t&0x20) PSHUWORD(yreg);
	if(t&0x10) PSHUWORD(xreg);
	if(t&0x08) PSHUBYTE(dpreg);
	if(t&0x04) PSHUBYTE(breg);
	if(t&0x02) PSHUBYTE(areg);
	if(t&0x01) PSHUBYTE(cc);
}

/* 37 PULU inherent ----- */
INLINE_STATIC void pulu( void )
{
	byte t;
	IMMBYTE(t);
	if(t&0x01) PULUBYTE(cc);
	if(t&0x02) PULUBYTE(areg);
	if(t&0x04) PULUBYTE(breg);
	if(t&0x08) PULUBYTE(dpreg);
	if(t&0x10) PULUWORD(xreg);
	if(t&0x20) PULUWORD(yreg);
	if(t&0x40) PULUWORD(sreg);
	if(t&0x80) PULUWORD(pcreg);
}

/* $38 ILLEGAL */

/* $39 RTS inherent ----- */
INLINE_STATIC void rts( void )
{
	PULLWORD(pcreg);
}

/* $3A ABX inherent ----- */
INLINE_STATIC void abx( void )
{
	xreg += breg;
}

/* $3B RTI inherent ##### */
INLINE_STATIC void rti( void )
{
	byte t;
	t=cc&0x80;
	PULLBYTE(cc);
	if(t)
	{
		m6809_1_ICount -= 9;
		PULLBYTE(areg);
		PULLBYTE(breg);
		PULLBYTE(dpreg);
		PULLWORD(xreg);
		PULLWORD(yreg);
		PULLWORD(ureg);
	}
	PULLWORD(pcreg);
}

/* $3C CWAI inherent ----1 */
INLINE_STATIC void cwai( void )
{
	byte t;
	IMMBYTE(t); cc&=t;
	/* CWAI should stack the entire machine state on the hardware stack,
		then wait for an interrupt. A poor fake is to force an IRQ. */
	m6809_1_ICount = 0;
}

/* $3D MUL inherent --*-@ */
INLINE_STATIC void mul( void )
{
	word t;
	t=areg*breg;
	CLR_ZC; SET_Z16(t); if(t&0x80) SEC;
	SETDREG(t);
}

/* $3E ILLEGAL */

/* $3F SWI (SWI2 SWI3) absolute indirect ----- */
INLINE_STATIC void swi( void )
{
	cc|=0x80;
	PUSHWORD(pcreg);
	PUSHWORD(ureg);
	PUSHWORD(yreg);
	PUSHWORD(xreg);
	PUSHBYTE(dpreg);
	PUSHBYTE(breg);
	PUSHBYTE(areg);
	PUSHBYTE(cc);
	cc|=0x50;
	pcreg = M_RDMEM_WORD(0xfffa);
}

/* $103F SWI2 absolute indirect ----- */
INLINE_STATIC void swi2( void )
{
	cc|=0x80;
	PUSHWORD(pcreg);
	PUSHWORD(ureg);
	PUSHWORD(yreg);
	PUSHWORD(xreg);
	PUSHBYTE(dpreg);
	PUSHBYTE(breg);
	PUSHBYTE(areg);
	PUSHBYTE(cc);
	pcreg = M_RDMEM_WORD(0xfff4);
}

/* $113F SWI3 absolute indirect ----- */
INLINE_STATIC void swi3( void )
{
	cc|=0x80;
	PUSHWORD(pcreg);
	PUSHWORD(ureg);
	PUSHWORD(yreg);
	PUSHWORD(xreg);
	PUSHBYTE(dpreg);
	PUSHBYTE(breg);
	PUSHBYTE(areg);
	PUSHBYTE(cc);
	pcreg = M_RDMEM_WORD(0xfff2);
}

#if macintosh
#pragma mark ____4x____
#endif

/* $40 NEGA inherent ?**** */
INLINE_STATIC void nega( void )
{
	word r;
	r=-areg;
	CLR_NZVC; SET_FLAGS8(0,areg,r);
	areg=r;
}

/* $41 ILLEGAL */

/* $42 ILLEGAL */

/* $43 COMA inherent -**01 */
INLINE_STATIC void coma( void )
{
	areg = ~areg;
	CLR_NZV; SET_NZ8(areg); SEC;
}

/* $44 LSRA inherent -0*-* */
INLINE_STATIC void lsra( void )
{
	CLR_NZC; cc|=(areg&0x01);
	areg>>=1; SET_Z8(areg);
}

/* $45 ILLEGAL */

/* $46 RORA inherent -**-* */
INLINE_STATIC void rora( void )
{
	byte r;
	r=(cc&0x01)<<7;
	CLR_NZC; cc|=(areg&0x01);
	r |= areg>>1; SET_NZ8(r);
	areg=r;
}

/* $47 ASRA inherent ?**-* */
INLINE_STATIC void asra( void )
{
	CLR_NZC; cc|=(areg&0x01);
	areg>>=1; areg|=((areg&0x40)<<1);
	SET_NZ8(areg);
}

/* $48 ASLA inherent ?**** */
INLINE_STATIC void asla( void )
{
	word r;
	r=areg<<1;
	CLR_NZVC; SET_FLAGS8(areg,areg,r);
	areg=r;
}

/* $49 ROLA inherent -**** */
INLINE_STATIC void rola( void )
{
	word t,r;
	t = areg; r = cc&0x01; r |= t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	areg=r;
}

/* $4A DECA inherent -***- */
INLINE_STATIC void deca( void )
{
	--areg;
	CLR_NZV; if(areg==0x7F) SEV; SET_NZ8(areg);
}

/* $4B ILLEGAL */

/* $4C INCA inherent -***- */
INLINE_STATIC void inca( void )
{
	++areg;
	CLR_NZV; if(areg==0x80) SEV; SET_NZ8(areg);
}

/* $4D TSTA inherent -**0- */
INLINE_STATIC void tsta( void )
{
	CLR_NZV; SET_NZ8(areg);
}

/* $4E ILLEGAL */

/* $4F CLRA inherent -0100 */
INLINE_STATIC void clra( void )
{
	areg=0;
	CLR_NZVC; SEZ;
}

#if macintosh
#pragma mark ____5x____
#endif

/* $50 NEGB inherent ?**** */
INLINE_STATIC void negb( void )
{
	word r;
	r=-breg;
	CLR_NZVC; SET_FLAGS8(0,breg,r);
	breg=r;
}

/* $51 ILLEGAL */

/* $52 ILLEGAL */

/* $53 COMB inherent -**01 */
INLINE_STATIC void comb( void )
{
	breg = ~breg;
	CLR_NZV; SET_NZ8(breg); SEC;
}

/* $54 LSRB inherent -0*-* */
INLINE_STATIC void lsrb( void )
{
	CLR_NZC; cc|=(breg&0x01);
	breg>>=1; SET_Z8(breg);
}

/* $55 ILLEGAL */

/* $56 RORB inherent -**-* */
INLINE_STATIC void rorb( void )
{
	byte r;
	r=(cc&0x01)<<7;
	CLR_NZC; cc|=(breg&0x01);
	r |= breg>>1; SET_NZ8(r);
	breg=r;
}

/* $57 ASRB inherent ?**-* */
INLINE_STATIC void asrb( void )
{
	CLR_NZC; cc|=(breg&0x01);
	breg>>=1; breg|=((breg&0x40)<<1);
	SET_NZ8(breg);
}

/* $58 ASLB inherent ?**** */
INLINE_STATIC void aslb( void )
{
	word r;
	r=breg<<1;
	CLR_NZVC; SET_FLAGS8(breg,breg,r);
	breg=r;
}

/* $59 ROLB inherent -**** */
INLINE_STATIC void rolb( void )
{
	word t,r;
	t = breg; r = cc&0x01; r |= t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	breg=r;
}

/* $5A DECB inherent -***- */
INLINE_STATIC void decb( void )
{
	--breg;
	CLR_NZV; if(breg==0x7F) SEV; SET_NZ8(breg);
}

/* $5B ILLEGAL */

/* $5C INCB inherent -***- */
INLINE_STATIC void incb( void )
{
	++breg;
	CLR_NZV; if(breg==0x80) SEV; SET_NZ8(breg);
}

/* $5D TSTB inherent -**0- */
INLINE_STATIC void tstb( void )
{
	CLR_NZV; SET_NZ8(breg);
}

/* $5E ILLEGAL */

/* $5F CLRB inherent -0100 */
INLINE_STATIC void clrb( void )
{
	breg=0;
	CLR_NZVC; SEZ;
}

#if macintosh
#pragma mark ____6x____
#endif

/* $60 NEG indexed ?**** */
INLINE_STATIC void neg_ix( void )
{
	word r,t;
	t=M_RDMEM(eaddr); r=-t;
	CLR_NZVC; SET_FLAGS8(0,t,r);
	M_WRMEM(eaddr,r);
}

/* $61 ILLEGAL */

/* $62 ILLEGAL */

/* $63 COM indexed -**01 */
INLINE_STATIC void com_ix( void )
{
	byte t;
	t = ~M_RDMEM(eaddr);
	CLR_NZV; SET_NZ8(t); SEC;
	M_WRMEM(eaddr,t);
}

/* $64 LSR indexed -0*-* */
INLINE_STATIC void lsr_ix( void )
{
	byte t;
	t=M_RDMEM(eaddr); CLR_NZC; cc|=(t&0x01);
	t>>=1; SET_Z8(t);
	M_WRMEM(eaddr,t);
}

/* $65 ILLEGAL */

/* $66 ROR indexed -**-* */
INLINE_STATIC void ror_ix( void )
{
	byte t,r;
	t=M_RDMEM(eaddr); r=(cc&0x01)<<7;
	CLR_NZC; cc|=(t&0x01);
	r |= t>>1; SET_NZ8(r);
	M_WRMEM(eaddr,r);
}

/* $67 ASR indexed ?**-* */
INLINE_STATIC void asr_ix( void )
{
	byte t;
	t=M_RDMEM(eaddr); CLR_NZC; cc|=(t&0x01);
	t>>=1; t|=((t&0x40)<<1);
	SET_NZ8(t);
	M_WRMEM(eaddr,t);
}

/* $68 ASL indexed ?**** */
INLINE_STATIC void asl_ix( void )
{
	word t,r;
	t=M_RDMEM(eaddr); r=t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	M_WRMEM(eaddr,r);
}

/* $69 ROL indexed -**** */
INLINE_STATIC void rol_ix( void )
{
	word t,r;
	t=M_RDMEM(eaddr); r = cc&0x01; r |= t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	M_WRMEM(eaddr,r);
}

/* $6A DEC indexed -***- */
INLINE_STATIC void dec_ix( void )
{
	byte t;
	t=M_RDMEM(eaddr)-1;
	CLR_NZV; if(t==0x7F) SEV; SET_NZ8(t);
	M_WRMEM(eaddr,t);
}

/* $6B ILLEGAL */

/* $6C INC indexed -***- */
INLINE_STATIC void inc_ix( void )
{
	byte t;
	t=M_RDMEM(eaddr)+1;
	CLR_NZV; if(t==0x80) SEV; SET_NZ8(t);
	M_WRMEM(eaddr,t);
}

/* $6D TST indexed -**0- */
INLINE_STATIC void tst_ix( void )
{
	byte t;
	t=M_RDMEM(eaddr); CLR_NZV; SET_NZ8(t);
}

/* $6E JMP indexed ----- */
INLINE_STATIC void jmp_ix( void )
{
	pcreg=eaddr;
}

/* $6F CLR indexed -0100 */
INLINE_STATIC void clr_ix( void )
{
	M_WRMEM(eaddr,0);
	CLR_NZVC; SEZ;
}

#if macintosh
#pragma mark ____7x____
#endif

/* $70 NEG extended ?**** */
INLINE_STATIC void neg_ex( void )
{
	word r,t;
	EXTBYTE(t); r=-t;
	CLR_NZVC; SET_FLAGS8(0,t,r);
	M_WRMEM(eaddr,r);
}

/* $71 ILLEGAL */

/* $72 ILLEGAL */

/* $73 COM extended -**01 */
INLINE_STATIC void com_ex( void )
{
	byte t;
	EXTBYTE(t); t = ~t;
	CLR_NZV; SET_NZ8(t); SEC;
	M_WRMEM(eaddr,t);
}

/* $74 LSR extended -0*-* */
INLINE_STATIC void lsr_ex( void )
{
	byte t;
	EXTBYTE(t); CLR_NZC; cc|=(t&0x01);
	t>>=1; SET_Z8(t);
	M_WRMEM(eaddr,t);
}

/* $75 ILLEGAL */

/* $76 ROR extended -**-* */
INLINE_STATIC void ror_ex( void )
{
	byte t,r;
	EXTBYTE(t); r=(cc&0x01)<<7;
	CLR_NZC; cc|=(t&0x01);
	r |= t>>1; SET_NZ8(r);
	M_WRMEM(eaddr,r);
}

/* $77 ASR extended ?**-* */
INLINE_STATIC void asr_ex( void )
{
	byte t;
	EXTBYTE(t); CLR_NZC; cc|=(t&0x01);
	t>>=1; t|=((t&0x40)<<1);
	SET_NZ8(t);
	M_WRMEM(eaddr,t);
}

/* $78 ASL extended ?**** */
INLINE_STATIC void asl_ex( void )
{
	word t,r;
	EXTBYTE(t); r=t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	M_WRMEM(eaddr,r);
}

/* $79 ROL extended -**** */
INLINE_STATIC void rol_ex( void )
{
	word t,r;
	EXTBYTE(t); r = cc&0x01; r |= t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	M_WRMEM(eaddr,r);
}

/* $7A DEC extended -***- */
INLINE_STATIC void dec_ex( void )
{
	byte t;
	EXTBYTE(t); --t;
	CLR_NZV; if(t==0x7F) SEV; SET_NZ8(t);
	M_WRMEM(eaddr,t);
}

/* $7B ILLEGAL */

/* $7C INC extended -***- */
INLINE_STATIC void inc_ex( void )
{
	byte t;
	EXTBYTE(t); ++t;
	CLR_NZV; if(t==0x80) SEV; SET_NZ8(t);
	M_WRMEM(eaddr,t);
}

/* $7D TST extended -**0- */
INLINE_STATIC void tst_ex( void )
{
	byte t;
	EXTBYTE(t); CLR_NZV; SET_NZ8(t);
}

/* $7E JMP extended ----- */
INLINE_STATIC void jmp_ex( void )
{
	EXTENDED; pcreg=eaddr;
}

/* $7F CLR extended -0100 */
INLINE_STATIC void clr_ex( void )
{
	EXTENDED; M_WRMEM(eaddr,0);
	CLR_NZVC; SEZ;
}


#if macintosh
#pragma mark ____8x____
#endif

/* $80 SUBA immediate ?**** */
INLINE_STATIC void suba_im( void )
{
	word	t,r;
	IMMBYTE(t); r = areg-t;
	CLR_NZVC; SET_FLAGS8(areg,t,r);
	areg = r;
}

/* $81 CMPA immediate ?**** */
INLINE_STATIC void cmpa_im( void )
{
	word	t,r;
	IMMBYTE(t); r = areg-t;
	CLR_NZVC; SET_FLAGS8(areg,t,r);
}

/* $82 SBCA immediate ?**** */
INLINE_STATIC void sbca_im( void )
{
	word	t,r;
	IMMBYTE(t); r = areg-t-(cc&0x01);
	CLR_NZVC; SET_FLAGS8(areg,t,r);
	areg = r;
}

/* $83 SUBD (CMPD CMPU) immediate -**** */
INLINE_STATIC void subd_im( void )
{
	dword r,d,b;
	IMMWORD(b); d = GETDREG; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
	SETDREG(r);
}

/* $1083 CMPD immediate -**** */
INLINE_STATIC void cmpd_im( void )
{
	dword r,d,b;
	IMMWORD(b); d = GETDREG; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $1183 CMPU immediate -**** */
INLINE_STATIC void cmpu_im( void )
{
	dword r,b;
	IMMWORD(b); r = ureg-b;
	CLR_NZVC; SET_FLAGS16(ureg,b,r);
}

/* $84 ANDA immediate -**0- */
INLINE_STATIC void anda_im( void )
{
	byte t;
	IMMBYTE(t); areg &= t;
	CLR_NZV; SET_NZ8(areg);
}

/* $85 BITA immediate -**0- */
INLINE_STATIC void bita_im( void )
{
	byte t,r;
	IMMBYTE(t); r = areg&t;
	CLR_NZV; SET_NZ8(r);
}

/* $86 LDA immediate -**0- */
INLINE_STATIC void lda_im( void )
{
	IMMBYTE(areg);
	CLR_NZV; SET_NZ8(areg);
}

/* is this a legal instruction? */
/* $87 STA immediate -**0- */
INLINE_STATIC void sta_im( void )
{
	CLR_NZV; SET_NZ8(areg);
	IMM8; M_WRMEM(eaddr,areg);
}

/* $88 EORA immediate -**0- */
INLINE_STATIC void eora_im( void )
{
	byte t;
	IMMBYTE(t); areg ^= t;
	CLR_NZV; SET_NZ8(areg);
}

/* $89 ADCA immediate ***** */
INLINE_STATIC void adca_im( void )
{
	word t,r;
	IMMBYTE(t); r = areg+t+(cc&0x01);
	CLR_HNZVC; SET_FLAGS8(areg,t,r); SET_H(areg,t,r);
	areg = r;
}

/* $8A ORA immediate -**0- */
INLINE_STATIC void ora_im( void )
{
	byte t;
	IMMBYTE(t); areg |= t;
	CLR_NZV; SET_NZ8(areg);
}

/* $8B ADDA immediate ***** */
INLINE_STATIC void adda_im( void )
{
	word t,r;
	IMMBYTE(t); r = areg+t;
	CLR_HNZVC; SET_FLAGS8(areg,t,r); SET_H(areg,t,r);
	areg = r;
}

/* $8C CMPX (CMPY CMPS) immediate -**** */
INLINE_STATIC void cmpx_im( void )
{
	dword r,d,b;
	IMMWORD(b); d = xreg; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $108C CMPY immediate -**** */
INLINE_STATIC void cmpy_im( void )
{
	dword r,d,b;
	IMMWORD(b); d = yreg; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $118C CMPS immediate -**** */
INLINE_STATIC void cmps_im( void )
{
	dword r,d,b;
	IMMWORD(b); d = sreg; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $8D BSR ----- */
INLINE_STATIC void bsr( void )
{
	byte t;
	IMMBYTE(t); PUSHWORD(pcreg); pcreg += SIGNED(t);
}

/* $8E LDX (LDY) immediate -**0- */
INLINE_STATIC void ldx_im( void )
{
	IMMWORD(xreg);
	CLR_NZV; SET_NZ16(xreg);
}

/* $108E LDY immediate -**0- */
INLINE_STATIC void ldy_im( void )
{
	IMMWORD(yreg);
	CLR_NZV; SET_NZ16(yreg);
}

/* is this a legal instruction? */
/* $8F STX (STY) immediate -**0- */
INLINE_STATIC void stx_im( void )
{
	CLR_NZV; SET_NZ16(xreg);
	IMM16; M_WRMEM_WORD(eaddr,xreg);
}

/* is this a legal instruction? */
/* $108F STY immediate -**0- */
INLINE_STATIC void sty_im( void )
{
	CLR_NZV; SET_NZ16(yreg);
	IMM16; M_WRMEM_WORD(eaddr,yreg);
}

#if macintosh
#pragma mark ____9x____
#endif

/* $90 SUBA direct ?**** */
INLINE_STATIC void suba_di( void )
{
	word	t,r;
	DIRBYTE(t); r = areg-t;
	CLR_NZVC; SET_FLAGS8(areg,t,r);
	areg = r;
}

/* $91 CMPA direct ?**** */
INLINE_STATIC void cmpa_di( void )
{
	word	t,r;
	DIRBYTE(t); r = areg-t;
	CLR_NZVC; SET_FLAGS8(areg,t,r);
}

/* $92 SBCA direct ?**** */
INLINE_STATIC void sbca_di( void )
{
	word	t,r;
	DIRBYTE(t); r = areg-t-(cc&0x01);
	CLR_NZVC; SET_FLAGS8(areg,t,r);
	areg = r;
}

/* $93 SUBD (CMPD CMPU) direct -**** */
INLINE_STATIC void subd_di( void )
{
	dword r,d,b;
	DIRWORD(b); d = GETDREG; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
	SETDREG(r);
}

/* $1093 CMPD direct -**** */
INLINE_STATIC void cmpd_di( void )
{
	dword r,d,b;
	DIRWORD(b); d = GETDREG; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $1193 CMPU direct -**** */
INLINE_STATIC void cmpu_di( void )
{
	dword r,b;
	DIRWORD(b); r = ureg-b;
	CLR_NZVC; SET_FLAGS16(ureg,b,r);
}

/* $94 ANDA direct -**0- */
INLINE_STATIC void anda_di( void )
{
	byte t;
	DIRBYTE(t); areg &= t;
	CLR_NZV; SET_NZ8(areg);
}

/* $95 BITA direct -**0- */
INLINE_STATIC void bita_di( void )
{
	byte t,r;
	DIRBYTE(t); r = areg&t;
	CLR_NZV; SET_NZ8(r);
}

/* $96 LDA direct -**0- */
INLINE_STATIC void lda_di( void )
{
	DIRBYTE(areg);
	CLR_NZV; SET_NZ8(areg);
}

/* $97 STA direct -**0- */
INLINE_STATIC void sta_di( void )
{
	CLR_NZV; SET_NZ8(areg);
	DIRECT; M_WRMEM(eaddr,areg);
}

/* $98 EORA direct -**0- */
INLINE_STATIC void eora_di( void )
{
	byte t;
	DIRBYTE(t); areg ^= t;
	CLR_NZV; SET_NZ8(areg);
}

/* $99 ADCA direct ***** */
INLINE_STATIC void adca_di( void )
{
	word t,r;
	DIRBYTE(t); r = areg+t+(cc&0x01);
	CLR_HNZVC; SET_FLAGS8(areg,t,r); SET_H(areg,t,r);
	areg = r;
}

/* $9A ORA direct -**0- */
INLINE_STATIC void ora_di( void )
{
	byte t;
	DIRBYTE(t); areg |= t;
	CLR_NZV; SET_NZ8(areg);
}

/* $9B ADDA direct ***** */
INLINE_STATIC void adda_di( void )
{
	word t,r;
	DIRBYTE(t); r = areg+t;
	CLR_HNZVC; SET_FLAGS8(areg,t,r); SET_H(areg,t,r);
	areg = r;
}

/* $9C CMPX (CMPY CMPS) direct -**** */
INLINE_STATIC void cmpx_di( void )
{
	dword r,d,b;
	DIRWORD(b); d = xreg; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $109C CMPY direct -**** */
INLINE_STATIC void cmpy_di( void )
{
	dword r,d,b;
	DIRWORD(b); d = yreg; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $119C CMPS direct -**** */
INLINE_STATIC void cmps_di( void )
{
	dword r,d,b;
	DIRWORD(b); d = sreg; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $9D JSR direct ----- */
INLINE_STATIC void jsr_di( void )
{
	DIRECT; PUSHWORD(pcreg);
	pcreg = eaddr;
}

/* $9E LDX (LDY) direct -**0- */
INLINE_STATIC void ldx_di( void )
{
	DIRWORD(xreg);
	CLR_NZV; SET_NZ16(xreg);
}

/* $109E LDY direct -**0- */
INLINE_STATIC void ldy_di( void )
{
	DIRWORD(yreg);
	CLR_NZV; SET_NZ16(yreg);
}

/* $9F STX (STY) direct -**0- */
INLINE_STATIC void stx_di( void )
{
	CLR_NZV; SET_NZ16(xreg);
	DIRECT; M_WRMEM_WORD(eaddr,xreg);
}

/* $109F STY direct -**0- */
INLINE_STATIC void sty_di( void )
{
	CLR_NZV; SET_NZ16(yreg);
	DIRECT; M_WRMEM_WORD(eaddr,yreg);
}

#if macintosh
#pragma mark ____Ax____
#endif


/* $a0 SUBA indexed ?**** */
INLINE_STATIC void suba_ix( void )
{
	word	t,r;
	t = M_RDMEM(eaddr); r = areg-t;
	CLR_NZVC; SET_FLAGS8(areg,t,r);
	areg = r;
}

/* $a1 CMPA indexed ?**** */
INLINE_STATIC void cmpa_ix( void )
{
	word	t,r;
	t = M_RDMEM(eaddr); r = areg-t;
	CLR_NZVC; SET_FLAGS8(areg,t,r);
}

/* $a2 SBCA indexed ?**** */
INLINE_STATIC void sbca_ix( void )
{
	word	t,r;
	t = M_RDMEM(eaddr); r = areg-t-(cc&0x01);
	CLR_NZVC; SET_FLAGS8(areg,t,r);
	areg = r;
}

/* $a3 SUBD (CMPD CMPU) indexed -**** */
INLINE_STATIC void subd_ix( void )
{
	dword r,d,b;
	b = M_RDMEM_WORD(eaddr); d = GETDREG; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
	SETDREG(r);
}

/* $10a3 CMPD indexed -**** */
INLINE_STATIC void cmpd_ix( void )
{
	dword r,d,b;
	b = M_RDMEM_WORD(eaddr); d = GETDREG; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $11a3 CMPU indexed -**** */
INLINE_STATIC void cmpu_ix( void )
{
	dword r,b;
	b = M_RDMEM_WORD(eaddr); r = ureg-b;
	CLR_NZVC; SET_FLAGS16(ureg,b,r);
}

/* $a4 ANDA indexed -**0- */
INLINE_STATIC void anda_ix( void )
{
	areg &= M_RDMEM(eaddr);
	CLR_NZV; SET_NZ8(areg);
}

/* $a5 BITA indexed -**0- */
INLINE_STATIC void bita_ix( void )
{
	byte r;
	r = areg&M_RDMEM(eaddr);
	CLR_NZV; SET_NZ8(r);
}

/* $a6 LDA indexed -**0- */
INLINE_STATIC void lda_ix( void )
{
	areg = M_RDMEM(eaddr);
	CLR_NZV; SET_NZ8(areg);
}

/* $a7 STA indexed -**0- */
INLINE_STATIC void sta_ix( void )
{
	CLR_NZV; SET_NZ8(areg);
	M_WRMEM(eaddr,areg);
}

/* $a8 EORA indexed -**0- */
INLINE_STATIC void eora_ix( void )
{
	areg ^= M_RDMEM(eaddr);
	CLR_NZV; SET_NZ8(areg);
}

/* $a9 ADCA indexed ***** */
INLINE_STATIC void adca_ix( void )
{
	word t,r;
	t = M_RDMEM(eaddr); r = areg+t+(cc&0x01);
	CLR_HNZVC; SET_FLAGS8(areg,t,r); SET_H(areg,t,r);
	areg = r;
}

/* $aA ORA indexed -**0- */
INLINE_STATIC void ora_ix( void )
{
	areg |= M_RDMEM(eaddr);
	CLR_NZV; SET_NZ8(areg);
}

/* $aB ADDA indexed ***** */
INLINE_STATIC void adda_ix( void )
{
	word t,r;
	t = M_RDMEM(eaddr); r = areg+t;
	CLR_HNZVC; SET_FLAGS8(areg,t,r); SET_H(areg,t,r);
	areg = r;
}

/* $aC CMPX (CMPY CMPS) indexed -**** */
INLINE_STATIC void cmpx_ix( void )
{
	dword r,d,b;
	b = M_RDMEM_WORD(eaddr); d = xreg; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $10aC CMPY indexed -**** */
INLINE_STATIC void cmpy_ix( void )
{
	dword r,d,b;
	b = M_RDMEM_WORD(eaddr); d = yreg; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $11aC CMPS indexed -**** */
INLINE_STATIC void cmps_ix( void )
{
	dword r,d,b;
	b = M_RDMEM_WORD(eaddr); d = sreg; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $aD JSR indexed ----- */
INLINE_STATIC void jsr_ix( void )
{
	PUSHWORD(pcreg);
	pcreg = eaddr;
}

/* $aE LDX (LDY) indexed -**0- */
INLINE_STATIC void ldx_ix( void )
{
	xreg = M_RDMEM_WORD(eaddr);
	CLR_NZV; SET_NZ16(xreg);
}

/* $10aE LDY indexed -**0- */
INLINE_STATIC void ldy_ix( void )
{
	yreg = M_RDMEM_WORD(eaddr);
	CLR_NZV; SET_NZ16(yreg);
}

/* $aF STX (STY) indexed -**0- */
INLINE_STATIC void stx_ix( void )
{
	CLR_NZV; SET_NZ16(xreg);
	M_WRMEM_WORD(eaddr,xreg);
}

/* $10aF STY indexed -**0- */
INLINE_STATIC void sty_ix( void )
{
	CLR_NZV; SET_NZ16(yreg);
	M_WRMEM_WORD(eaddr,yreg);
}

#if macintosh
#pragma mark ____Bx____
#endif

/* $b0 SUBA extended ?**** */
INLINE_STATIC void suba_ex( void )
{
	word	t,r;
	EXTBYTE(t); r = areg-t;
	CLR_NZVC; SET_FLAGS8(areg,t,r);
	areg = r;
}

/* $b1 CMPA extended ?**** */
INLINE_STATIC void cmpa_ex( void )
{
	word	t,r;
	EXTBYTE(t); r = areg-t;
	CLR_NZVC; SET_FLAGS8(areg,t,r);
}

/* $b2 SBCA extended ?**** */
INLINE_STATIC void sbca_ex( void )
{
	word	t,r;
	EXTBYTE(t); r = areg-t-(cc&0x01);
	CLR_NZVC; SET_FLAGS8(areg,t,r);
	areg = r;
}

/* $b3 SUBD (CMPD CMPU) extended -**** */
INLINE_STATIC void subd_ex( void )
{
	dword r,d,b;
	EXTWORD(b); d = GETDREG; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
	SETDREG(r);
}

/* $10b3 CMPD extended -**** */
INLINE_STATIC void cmpd_ex( void )
{
	dword r,d,b;
	EXTWORD(b); d = GETDREG; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $11b3 CMPU extended -**** */
INLINE_STATIC void cmpu_ex( void )
{
	dword r,b;
	EXTWORD(b); r = ureg-b;
	CLR_NZVC; SET_FLAGS16(ureg,b,r);
}

/* $b4 ANDA extended -**0- */
INLINE_STATIC void anda_ex( void )
{
	byte t;
	EXTBYTE(t); areg &= t;
	CLR_NZV; SET_NZ8(areg);
}

/* $b5 BITA extended -**0- */
INLINE_STATIC void bita_ex( void )
{
	byte t,r;
	EXTBYTE(t); r = areg&t;
	CLR_NZV; SET_NZ8(r);
}

/* $b6 LDA extended -**0- */
INLINE_STATIC void lda_ex( void )
{
	EXTBYTE(areg);
	CLR_NZV; SET_NZ8(areg);
}

/* $b7 STA extended -**0- */
INLINE_STATIC void sta_ex( void )
{
	CLR_NZV; SET_NZ8(areg);
	EXTENDED; M_WRMEM(eaddr,areg);
}

/* $b8 EORA extended -**0- */
INLINE_STATIC void eora_ex( void )
{
	byte t;
	EXTBYTE(t); areg ^= t;
	CLR_NZV; SET_NZ8(areg);
}

/* $b9 ADCA extended ***** */
INLINE_STATIC void adca_ex( void )
{
	word t,r;
	EXTBYTE(t); r = areg+t+(cc&0x01);
	CLR_HNZVC; SET_FLAGS8(areg,t,r); SET_H(areg,t,r);
	areg = r;
}

/* $bA ORA extended -**0- */
INLINE_STATIC void ora_ex( void )
{
	byte t;
	EXTBYTE(t); areg |= t;
	CLR_NZV; SET_NZ8(areg);
}

/* $bB ADDA extended ***** */
INLINE_STATIC void adda_ex( void )
{
	word t,r;
	EXTBYTE(t); r = areg+t;
	CLR_HNZVC; SET_FLAGS8(areg,t,r); SET_H(areg,t,r);
	areg = r;
}

/* $bC CMPX (CMPY CMPS) extended -**** */
INLINE_STATIC void cmpx_ex( void )
{
	dword r,d,b;
	EXTWORD(b); d = xreg; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $10bC CMPY extended -**** */
INLINE_STATIC void cmpy_ex( void )
{
	dword r,d,b;
	EXTWORD(b); d = yreg; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $11bC CMPS extended -**** */
INLINE_STATIC void cmps_ex( void )
{
	dword r,d,b;
	EXTWORD(b); d = sreg; r = d-b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
}

/* $bD JSR extended ----- */
INLINE_STATIC void jsr_ex( void )
{
	EXTENDED; PUSHWORD(pcreg);
	pcreg = eaddr;
}

/* $bE LDX (LDY) extended -**0- */
INLINE_STATIC void ldx_ex( void )
{
	EXTWORD(xreg);
	CLR_NZV; SET_NZ16(xreg);
}

/* $10bE LDY extended -**0- */
INLINE_STATIC void ldy_ex( void )
{
	EXTWORD(yreg);
	CLR_NZV; SET_NZ16(yreg);
}

/* $bF STX (STY) extended -**0- */
INLINE_STATIC void stx_ex( void )
{
	CLR_NZV; SET_NZ16(xreg);
	EXTENDED; M_WRMEM_WORD(eaddr,xreg);
}

/* $10bF STY extended -**0- */
INLINE_STATIC void sty_ex( void )
{
	CLR_NZV; SET_NZ16(yreg);
	EXTENDED; M_WRMEM_WORD(eaddr,yreg);
}


#if macintosh
#pragma mark ____Cx____
#endif

/* $c0 SUBB immediate ?**** */
INLINE_STATIC void subb_im( void )
{
	word	t,r;
	IMMBYTE(t); r = breg-t;
	CLR_NZVC; SET_FLAGS8(breg,t,r);
	breg = r;
}

/* $c1 CMPB immediate ?**** */
INLINE_STATIC void cmpb_im( void )
{
	word	t,r;
	IMMBYTE(t); r = breg-t;
	CLR_NZVC; SET_FLAGS8(breg,t,r);
}

/* $c2 SBCB immediate ?**** */
INLINE_STATIC void sbcb_im( void )
{
	word	t,r;
	IMMBYTE(t); r = breg-t-(cc&0x01);
	CLR_NZVC; SET_FLAGS8(breg,t,r);
	breg = r;
}

/* $c3 ADDD immediate -**** */
INLINE_STATIC void addd_im( void )
{
	dword r,d,b;
	IMMWORD(b); d = GETDREG; r = d+b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
	SETDREG(r);
}

/* $c4 ANDB immediate -**0- */
INLINE_STATIC void andb_im( void )
{
	byte t;
	IMMBYTE(t); breg &= t;
	CLR_NZV; SET_NZ8(breg);
}

/* $c5 BITB immediate -**0- */
INLINE_STATIC void bitb_im( void )
{
	byte t,r;
	IMMBYTE(t); r = breg&t;
	CLR_NZV; SET_NZ8(r);
}

/* $c6 LDB immediate -**0- */
INLINE_STATIC void ldb_im( void )
{
	IMMBYTE(breg);
	CLR_NZV; SET_NZ8(breg);
}

/* is this a legal instruction? */
/* $c7 STB immediate -**0- */
INLINE_STATIC void stb_im( void )
{
	CLR_NZV; SET_NZ8(breg);
	IMM8; M_WRMEM(eaddr,breg);
}

/* $c8 EORB immediate -**0- */
INLINE_STATIC void eorb_im( void )
{
	byte t;
	IMMBYTE(t); breg ^= t;
	CLR_NZV; SET_NZ8(breg);
}

/* $c9 ADCB immediate ***** */
INLINE_STATIC void adcb_im( void )
{
	word t,r;
	IMMBYTE(t); r = breg+t+(cc&0x01);
	CLR_HNZVC; SET_FLAGS8(breg,t,r); SET_H(breg,t,r);
	breg = r;
}

/* $cA ORB immediate -**0- */
INLINE_STATIC void orb_im( void )
{
	byte t;
	IMMBYTE(t); breg |= t;
	CLR_NZV; SET_NZ8(breg);
}

/* $cB ADDB immediate ***** */
INLINE_STATIC void addb_im( void )
{
	word t,r;
	IMMBYTE(t); r = breg+t;
	CLR_HNZVC; SET_FLAGS8(breg,t,r); SET_H(breg,t,r);
	breg = r;
}

/* $cC LDD immediate -**0- */
INLINE_STATIC void ldd_im( void )
{
	word t;
	IMMWORD(t); SETDREG(t);
	CLR_NZV; SET_NZ16(t);
}

/* is this a legal instruction? */
/* $cD STD immediate -**0- */
INLINE_STATIC void std_im( void )
{
	word t;
	IMM16; t=GETDREG;
	CLR_NZV; SET_NZ16(t);
	M_WRMEM_WORD(eaddr,t);
}

/* $cE LDU (LDS) immediate -**0- */
INLINE_STATIC void ldu_im( void )
{
	IMMWORD(ureg);
	CLR_NZV; SET_NZ16(ureg);
}

/* $10cE LDS immediate -**0- */
INLINE_STATIC void lds_im( void )
{
	IMMWORD(sreg);
	CLR_NZV; SET_NZ16(sreg);
}

/* is this a legal instruction? */
/* $cF STU (STS) immediate -**0- */
INLINE_STATIC void stu_im( void )
{
	CLR_NZV; SET_NZ16(ureg);
	IMM16; M_WRMEM_WORD(eaddr,ureg);
}

/* is this a legal instruction? */
/* $10cF STS immediate -**0- */
INLINE_STATIC void sts_im( void )
{
	CLR_NZV; SET_NZ16(sreg);
	IMM16; M_WRMEM_WORD(eaddr,sreg);
}


#if macintosh
#pragma mark ____Dx____
#endif

/* $d0 SUBB direct ?**** */
INLINE_STATIC void subb_di( void )
{
	word	t,r;
	DIRBYTE(t); r = breg-t;
	CLR_NZVC; SET_FLAGS8(breg,t,r);
	breg = r;
}

/* $d1 CMPB direct ?**** */
INLINE_STATIC void cmpb_di( void )
{
	word	t,r;
	DIRBYTE(t); r = breg-t;
	CLR_NZVC; SET_FLAGS8(breg,t,r);
}

/* $d2 SBCB direct ?**** */
INLINE_STATIC void sbcb_di( void )
{
	word	t,r;
	DIRBYTE(t); r = breg-t-(cc&0x01);
	CLR_NZVC; SET_FLAGS8(breg,t,r);
	breg = r;
}

/* $d3 ADDD direct -**** */
INLINE_STATIC void addd_di( void )
{
	dword r,d,b;
	DIRWORD(b); d = GETDREG; r = d+b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
	SETDREG(r);
}

/* $d4 ANDB direct -**0- */
INLINE_STATIC void andb_di( void )
{
	byte t;
	DIRBYTE(t); breg &= t;
	CLR_NZV; SET_NZ8(breg);
}

/* $d5 BITB direct -**0- */
INLINE_STATIC void bitb_di( void )
{
	byte t,r;
	DIRBYTE(t); r = breg&t;
	CLR_NZV; SET_NZ8(r);
}

/* $d6 LDB direct -**0- */
INLINE_STATIC void ldb_di( void )
{
	DIRBYTE(breg);
	CLR_NZV; SET_NZ8(breg);
}

/* $d7 STB direct -**0- */
INLINE_STATIC void stb_di( void )
{
	CLR_NZV; SET_NZ8(breg);
	DIRECT; M_WRMEM(eaddr,breg);
}

/* $d8 EORB direct -**0- */
INLINE_STATIC void eorb_di( void )
{
	byte t;
	DIRBYTE(t); breg ^= t;
	CLR_NZV; SET_NZ8(breg);
}

/* $d9 ADCB direct ***** */
INLINE_STATIC void adcb_di( void )
{
	word t,r;
	DIRBYTE(t); r = breg+t+(cc&0x01);
	CLR_HNZVC; SET_FLAGS8(breg,t,r); SET_H(breg,t,r);
	breg = r;
}

/* $dA ORB direct -**0- */
INLINE_STATIC void orb_di( void )
{
	byte t;
	DIRBYTE(t); breg |= t;
	CLR_NZV; SET_NZ8(breg);
}

/* $dB ADDB direct ***** */
INLINE_STATIC void addb_di( void )
{
	word t,r;
	DIRBYTE(t); r = breg+t;
	CLR_HNZVC; SET_FLAGS8(breg,t,r); SET_H(breg,t,r);
	breg = r;
}

/* $dC LDD direct -**0- */
INLINE_STATIC void ldd_di( void )
{
	word t;
	DIRWORD(t); SETDREG(t);
	CLR_NZV; SET_NZ16(t);
}

/* $dD STD direct -**0- */
INLINE_STATIC void std_di( void )
{
	word t;
	DIRECT; t=GETDREG;
	CLR_NZV; SET_NZ16(t);
	M_WRMEM_WORD(eaddr,t);
}

/* $dE LDU (LDS) direct -**0- */
INLINE_STATIC void ldu_di( void )
{
	DIRWORD(ureg);
	CLR_NZV; SET_NZ16(ureg);
}

/* $10dE LDS direct -**0- */
INLINE_STATIC void lds_di( void )
{
	DIRWORD(sreg);
	CLR_NZV; SET_NZ16(sreg);
}

/* $dF STU (STS) direct -**0- */
INLINE_STATIC void stu_di( void )
{
	CLR_NZV; SET_NZ16(ureg);
	DIRECT; M_WRMEM_WORD(eaddr,ureg);
}

/* $10dF STS direct -**0- */
INLINE_STATIC void sts_di( void )
{
	CLR_NZV; SET_NZ16(sreg);
	DIRECT; M_WRMEM_WORD(eaddr,sreg);
}

#if macintosh
#pragma mark ____Ex____
#endif


/* $e0 SUBB indexed ?**** */
INLINE_STATIC void subb_ix( void )
{
	word	t,r;
	t = M_RDMEM(eaddr); r = breg-t;
	CLR_NZVC; SET_FLAGS8(breg,t,r);
	breg = r;
}

/* $e1 CMPB indexed ?**** */
INLINE_STATIC void cmpb_ix( void )
{
	word	t,r;
	t = M_RDMEM(eaddr); r = breg-t;
	CLR_NZVC; SET_FLAGS8(breg,t,r);
}

/* $e2 SBCB indexed ?**** */
INLINE_STATIC void sbcb_ix( void )
{
	word	t,r;
	t = M_RDMEM(eaddr); r = breg-t-(cc&0x01);
	CLR_NZVC; SET_FLAGS8(breg,t,r);
	breg = r;
}

/* $e3 ADDD indexed -**** */
INLINE_STATIC void addd_ix( void )
{
	dword r,d,b;
	b = M_RDMEM_WORD(eaddr); d = GETDREG; r = d+b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
	SETDREG(r);
}

/* $e4 ANDB indexed -**0- */
INLINE_STATIC void andb_ix( void )
{
	breg &= M_RDMEM(eaddr);
	CLR_NZV; SET_NZ8(breg);
}

/* $e5 BITB indexed -**0- */
INLINE_STATIC void bitb_ix( void )
{
	byte r;
	r = breg&M_RDMEM(eaddr);
	CLR_NZV; SET_NZ8(r);
}

/* $e6 LDB indexed -**0- */
INLINE_STATIC void ldb_ix( void )
{
	breg = M_RDMEM(eaddr);
	CLR_NZV; SET_NZ8(breg);
}

/* $e7 STB indexed -**0- */
INLINE_STATIC void stb_ix( void )
{
	CLR_NZV; SET_NZ8(breg);
	M_WRMEM(eaddr,breg);
}

/* $e8 EORB indexed -**0- */
INLINE_STATIC void eorb_ix( void )
{
	breg ^= M_RDMEM(eaddr);
	CLR_NZV; SET_NZ8(breg);
}

/* $e9 ADCB indexed ***** */
INLINE_STATIC void adcb_ix( void )
{
	word t,r;
	t = M_RDMEM(eaddr); r = breg+t+(cc&0x01);
	CLR_HNZVC; SET_FLAGS8(breg,t,r); SET_H(breg,t,r);
	breg = r;
}

/* $eA ORB indexed -**0- */
INLINE_STATIC void orb_ix( void )
{
	breg |= M_RDMEM(eaddr);
	CLR_NZV; SET_NZ8(breg);
}

/* $eB ADDB indexed ***** */
INLINE_STATIC void addb_ix( void )
{
	word t,r;
	t = M_RDMEM(eaddr); r = breg+t;
	CLR_HNZVC; SET_FLAGS8(breg,t,r); SET_H(breg,t,r);
	breg = r;
}

/* $eC LDD indexed -**0- */
INLINE_STATIC void ldd_ix( void )
{
	word t;
	t = M_RDMEM_WORD(eaddr); SETDREG(t);
	CLR_NZV; SET_NZ16(t);
}

/* $eD STD indexed -**0- */
INLINE_STATIC void std_ix( void )
{
	word t;
	t=GETDREG;
	CLR_NZV; SET_NZ16(t);
	M_WRMEM_WORD(eaddr,t);
}

/* $eE LDU (LDS) indexed -**0- */
INLINE_STATIC void ldu_ix( void )
{
	ureg = M_RDMEM_WORD(eaddr);
	CLR_NZV; SET_NZ16(ureg);
}

/* $10eE LDS indexed -**0- */
INLINE_STATIC void lds_ix( void )
{
	sreg = M_RDMEM_WORD(eaddr);
	CLR_NZV; SET_NZ16(sreg);
}

/* $eF STU (STS) indexed -**0- */
INLINE_STATIC void stu_ix( void )
{
	CLR_NZV; SET_NZ16(ureg);
	M_WRMEM_WORD(eaddr,ureg);
}

/* $10eF STS indexed -**0- */
INLINE_STATIC void sts_ix( void )
{
	CLR_NZV; SET_NZ16(sreg);
	M_WRMEM_WORD(eaddr,sreg);
}

#if macintosh
#pragma mark ____Fx____
#endif

/* $f0 SUBB extended ?**** */
INLINE_STATIC void subb_ex( void )
{
	word	t,r;
	EXTBYTE(t); r = breg-t;
	CLR_NZVC; SET_FLAGS8(breg,t,r);
	breg = r;
}

/* $f1 CMPB extended ?**** */
INLINE_STATIC void cmpb_ex( void )
{
	word	t,r;
	EXTBYTE(t); r = breg-t;
	CLR_NZVC; SET_FLAGS8(breg,t,r);
}

/* $f2 SBCB extended ?**** */
INLINE_STATIC void sbcb_ex( void )
{
	word	t,r;
	EXTBYTE(t); r = breg-t-(cc&0x01);
	CLR_NZVC; SET_FLAGS8(breg,t,r);
	breg = r;
}

/* $f3 ADDD extended -**** */
INLINE_STATIC void addd_ex( void )
{
	dword r,d,b;
	EXTWORD(b); d = GETDREG; r = d+b;
	CLR_NZVC; SET_FLAGS16(d,b,r);
	SETDREG(r);
}

/* $f4 ANDB extended -**0- */
INLINE_STATIC void andb_ex( void )
{
	byte t;
	EXTBYTE(t); breg &= t;
	CLR_NZV; SET_NZ8(breg);
}

/* $f5 BITB extended -**0- */
INLINE_STATIC void bitb_ex( void )
{
	byte t,r;
	EXTBYTE(t); r = breg&t;
	CLR_NZV; SET_NZ8(r);
}

/* $f6 LDB extended -**0- */
INLINE_STATIC void ldb_ex( void )
{
	EXTBYTE(breg);
	CLR_NZV; SET_NZ8(breg);
}

/* $f7 STB extended -**0- */
INLINE_STATIC void stb_ex( void )
{
	CLR_NZV; SET_NZ8(breg);
	EXTENDED; M_WRMEM(eaddr,breg);
}

/* $f8 EORB extended -**0- */
INLINE_STATIC void eorb_ex( void )
{
	byte t;
	EXTBYTE(t); breg ^= t;
	CLR_NZV; SET_NZ8(breg);
}

/* $f9 ADCB extended ***** */
INLINE_STATIC void adcb_ex( void )
{
	word t,r;
	EXTBYTE(t); r = breg+t+(cc&0x01);
	CLR_HNZVC; SET_FLAGS8(breg,t,r); SET_H(breg,t,r);
	breg = r;
}

/* $fA ORB extended -**0- */
INLINE_STATIC void orb_ex( void )
{
	byte t;
	EXTBYTE(t); breg |= t;
	CLR_NZV; SET_NZ8(breg);
}

/* $fB ADDB extended ***** */
INLINE_STATIC void addb_ex( void )
{
	word t,r;
	EXTBYTE(t); r = breg+t;
	CLR_HNZVC; SET_FLAGS8(breg,t,r); SET_H(breg,t,r);
	breg = r;
}

/* $fC LDD extended -**0- */
INLINE_STATIC void ldd_ex( void )
{
	word t;
	EXTWORD(t); SETDREG(t);
	CLR_NZV; SET_NZ16(t);
}

/* $fD STD extended -**0- */
INLINE_STATIC void std_ex( void )
{
	word t;
	EXTENDED; t=GETDREG;
	CLR_NZV; SET_NZ16(t);
	M_WRMEM_WORD(eaddr,t);
}

/* $fE LDU (LDS) extended -**0- */
INLINE_STATIC void ldu_ex( void )
{
	EXTWORD(ureg);
	CLR_NZV; SET_NZ16(ureg);
}

/* $10fE LDS extended -**0- */
INLINE_STATIC void lds_ex( void )
{
	EXTWORD(sreg);
	CLR_NZV; SET_NZ16(sreg);
}

/* $fF STU (STS) extended -**0- */
INLINE_STATIC void stu_ex( void )
{
	CLR_NZV; SET_NZ16(ureg);
	EXTENDED; M_WRMEM_WORD(eaddr,ureg);
}

/* $10fF STS extended -**0- */
INLINE_STATIC void sts_ex( void )
{
	CLR_NZV; SET_NZ16(sreg);
	EXTENDED; M_WRMEM_WORD(eaddr,sreg);
}



static int rd_slow( int addr )
{
	return M_RDMEM(addr);
}

static int rd_slow_wd( int addr )
{
	return( (M_RDMEM(addr)<<8) | (M_RDMEM((addr+1)&0xffff)) );
}

static int rd_fast( int addr )
{
	return RAM[addr];
}

static int rd_fast_wd( int addr )
{
	return( (RAM[addr]<<8) | (RAM[(addr+1)&0xffff]) );
}

static void wr_slow( int addr, int v )
{
	M_WRMEM(addr,v);
}

static void wr_slow_wd( int addr, int v )
{
	M_WRMEM(addr,v>>8);
	M_WRMEM(((addr)+1)&0xFFFF,v&255);
}

static void wr_fast( int addr, int v )
{
	RAM[addr] = v;
}

static void wr_fast_wd( int addr, int v )
{
	RAM[addr] = v>>8;
	RAM[(addr+1)&0xffff] = v&255;
}


/****************************************************************************/
/* Set all registers to given values                                        */
/****************************************************************************/
void m6809_1_SetRegs(m6809_Regs *Regs)
{
	pcreg = Regs->pc;
	ureg = Regs->u;
	sreg = Regs->s;
	xreg = Regs->x;
	yreg = Regs->y;
	dpreg = Regs->dp;
	areg = Regs->a;
	breg = Regs->b;
	cc = Regs->cc;
}

/****************************************************************************/
/* Get all registers in given buffer                                        */
/****************************************************************************/
void m6809_1_GetRegs(m6809_Regs *Regs)
{
	Regs->pc = pcreg;
	Regs->u = ureg;
	Regs->s = sreg;
	Regs->x = xreg;
	Regs->y = yreg;
	Regs->dp = dpreg;
	Regs->a = areg;
	Regs->b = breg;
	Regs->cc = cc;
}

/****************************************************************************/
/* Return program counter                                                   */
/****************************************************************************/
unsigned m6809_1_GetPC(void)
{
	return pcreg;
}

/* Generate an IRQ */
void m6809_1_Interrupt()
{
	if( !(cc&0x10) )
	{
		/* standard IRQ */
		PUSHWORD(pcreg);
		PUSHWORD(ureg);
		PUSHWORD(yreg);
		PUSHWORD(xreg);
		PUSHBYTE(dpreg);
		PUSHBYTE(breg);
		PUSHBYTE(areg);
		PUSHBYTE(cc);
		cc|=0x90;
		pcreg=M_RDMEM_WORD(0xfff8);
		m6809_1_ICount -= 19;
	}
	else
		m6809_1_IRequest = INT_IRQ;
}

/* Generate a fast IRQ */
static void m6809_1_FIRQ( void )
{
	/* FIRQ */
	if( !(cc&0x40) )
	{
		/* fast IRQ */
		PUSHWORD(pcreg);
		PUSHBYTE(cc);
		cc&=0x7f;
		cc|=0x50;
		pcreg=M_RDMEM_WORD(0xfff6);
		m6809_1_ICount -= 10;
	}
}

void m6809_1_reset(void)
{
	pcreg = M_RDMEM_WORD(0xfffe);

	dpreg = 0x00;		/* Direct page register = 0x00 */
	cc = 0x00;			/* Clear all flags */
	cc |= 0x10;			/* IRQ disabled */
	cc |= 0x40;			/* FIRQ disabled */
	areg = 0x00;		/* clear accumulator a */
	breg = 0x00;		/* clear accumulator b */
	m6809_1_ICount=m6809_1_IPeriod;
	m6809_1_IRequest=INT_NONE;

	/* default to unoptimized memory access */
	fastopcodes = FALSE;
	rd_op_handler = rd_slow;
	rd_op_handler_wd = rd_slow_wd;
	rd_u_handler = rd_slow;
	rd_u_handler_wd = rd_slow_wd;
	rd_s_handler = rd_slow;
	rd_s_handler_wd = rd_slow_wd;
	wr_u_handler = wr_slow;
	wr_u_handler_wd = wr_slow_wd;
	wr_s_handler = wr_slow;
	wr_s_handler_wd = wr_slow_wd;

	/* optimize memory access according to flags */
	if( m6809_1_Flags & M6809_FAST_OP )
	{
		fastopcodes = TRUE;
		rd_op_handler = rd_fast; rd_op_handler_wd = rd_fast_wd;
	}
	if( m6809_1_Flags & M6809_FAST_U )
	{
		rd_u_handler=rd_fast; rd_u_handler_wd=rd_fast_wd;
		wr_u_handler=wr_fast; wr_u_handler_wd=wr_fast_wd;
	}
	if( m6809_1_Flags & M6809_FAST_S )
	{
		rd_s_handler=rd_fast; rd_s_handler_wd=rd_fast_wd;
		wr_s_handler=wr_fast; wr_s_handler_wd=wr_fast_wd;
	}
}

//#include "6809ops.c"

/* execute instructions on this CPU until icount expires */
void m6809_1_execute(void)
{
	byte ireg;
	extern int saved_icount; /* JB 970824 */

	do
	{
		#if 0
		asg_6809Trace(RAM, pcreg);
		#endif

		if( fastopcodes ) ireg=M_RDOP(pcreg);
		else ireg=M_RDMEM(pcreg);
		pcreg++;
//		log_it("CPU %i: %i,",pcreg-1,ireg);
		if( ireg!=0x10 && ireg!=0x11 )
		{
			if(haspostbyte[ireg]) fetch_effective_address();

			switch( ireg )
			{
				case 0x00: neg_di(); break;
				case 0x01: illegal(); break;
				case 0x02: illegal(); break;
				case 0x03: com_di(); break;
				case 0x04: lsr_di(); break;
				case 0x05: illegal(); break;
				case 0x06: ror_di(); break;
				case 0x07: asr_di(); break;
				case 0x08: asl_di(); break;
				case 0x09: rol_di(); break;
				case 0x0a: dec_di(); break;
				case 0x0b: illegal(); break;
				case 0x0c: inc_di(); break;
				case 0x0d: tst_di(); break;
				case 0x0e: jmp_di(); break;
				case 0x0f: clr_di(); break;
				case 0x10: illegal(); break;
				case 0x11: illegal(); break;
				case 0x12: nop(); break;
				case 0x13: sync(); break;
				case 0x14: illegal(); break;
				case 0x15: illegal(); break;
				case 0x16: lbra(); break;
				case 0x17: lbsr(); break;
				case 0x18: illegal(); break;
				case 0x19: daa(); break;
				case 0x1a: orcc(); break;
				case 0x1b: illegal(); break;
				case 0x1c: andcc(); break;
				case 0x1d: sex(); break;
				case 0x1e: exg(); break;
				case 0x1f: tfr(); break;
				case 0x20: bra(); break;
				case 0x21: brn(); break;
				case 0x22: bhi(); break;
				case 0x23: bls(); break;
				case 0x24: bcc(); break;
				case 0x25: bcs(); break;
				case 0x26: bne(); break;
				case 0x27: beq(); break;
				case 0x28: bvc(); break;
				case 0x29: bvs(); break;
				case 0x2a: bpl(); break;
				case 0x2b: bmi(); break;
				case 0x2c: bge(); break;
				case 0x2d: blt(); break;
				case 0x2e: bgt(); break;
				case 0x2f: ble(); break;
				case 0x30: leax(); break;
				case 0x31: leay(); break;
				case 0x32: leas(); break;
				case 0x33: leau(); break;
				case 0x34: pshs(); break;
				case 0x35: puls(); break;
				case 0x36: pshu(); break;
				case 0x37: pulu(); break;
				case 0x38: illegal(); break;
				case 0x39: rts(); break;
				case 0x3a: abx(); break;
				case 0x3b: rti(); break;
				case 0x3c: cwai(); break;
				case 0x3d: mul(); break;
				case 0x3e: illegal(); break;
				case 0x3f: swi(); break;
				case 0x40: nega(); break;
				case 0x41: illegal(); break;
				case 0x42: illegal(); break;
				case 0x43: coma(); break;
				case 0x44: lsra(); break;
				case 0x45: illegal(); break;
				case 0x46: rora(); break;
				case 0x47: asra(); break;
				case 0x48: asla(); break;
				case 0x49: rola(); break;
				case 0x4a: deca(); break;
				case 0x4b: illegal(); break;
				case 0x4c: inca(); break;
				case 0x4d: tsta(); break;
				case 0x4e: illegal(); break;
				case 0x4f: clra(); break;
				case 0x50: negb(); break;
				case 0x51: illegal(); break;
				case 0x52: illegal(); break;
				case 0x53: comb(); break;
				case 0x54: lsrb(); break;
				case 0x55: illegal(); break;
				case 0x56: rorb(); break;
				case 0x57: asrb(); break;
				case 0x58: aslb(); break;
				case 0x59: rolb(); break;
				case 0x5a: decb(); break;
				case 0x5b: illegal(); break;
				case 0x5c: incb(); break;
				case 0x5d: tstb(); break;
				case 0x5e: illegal(); break;
				case 0x5f: clrb(); break;
				case 0x60: neg_ix(); break;
				case 0x61: illegal(); break;
				case 0x62: illegal(); break;
				case 0x63: com_ix(); break;
				case 0x64: lsr_ix(); break;
				case 0x65: illegal(); break;
				case 0x66: ror_ix(); break;
				case 0x67: asr_ix(); break;
				case 0x68: asl_ix(); break;
				case 0x69: rol_ix(); break;
				case 0x6a: dec_ix(); break;
				case 0x6b: illegal(); break;
				case 0x6c: inc_ix(); break;
				case 0x6d: tst_ix(); break;
				case 0x6e: jmp_ix(); break;
				case 0x6f: clr_ix(); break;
				case 0x70: neg_ex(); break;
				case 0x71: illegal(); break;
				case 0x72: illegal(); break;
				case 0x73: com_ex(); break;
				case 0x74: lsr_ex(); break;
				case 0x75: illegal(); break;
				case 0x76: ror_ex(); break;
				case 0x77: asr_ex(); break;
				case 0x78: asl_ex(); break;
				case 0x79: rol_ex(); break;
				case 0x7a: dec_ex(); break;
				case 0x7b: illegal(); break;
				case 0x7c: inc_ex(); break;
				case 0x7d: tst_ex(); break;
				case 0x7e: jmp_ex(); break;
				case 0x7f: clr_ex(); break;
				case 0x80: suba_im(); break;
				case 0x81: cmpa_im(); break;
				case 0x82: sbca_im(); break;
				case 0x83: subd_im(); break;
				case 0x84: anda_im(); break;
				case 0x85: bita_im(); break;
				case 0x86: lda_im(); break;
				case 0x87: sta_im(); break; /* ILLEGAL? */
				case 0x88: eora_im(); break;
				case 0x89: adca_im(); break;
				case 0x8a: ora_im(); break;
				case 0x8b: adda_im(); break;
				case 0x8c: cmpx_im(); break;
				case 0x8d: bsr(); break;
				case 0x8e: ldx_im(); break;
				case 0x8f: stx_im(); break; /* ILLEGAL? */
				case 0x90: suba_di(); break;
				case 0x91: cmpa_di(); break;
				case 0x92: sbca_di(); break;
				case 0x93: subd_di(); break;
				case 0x94: anda_di(); break;
				case 0x95: bita_di(); break;
				case 0x96: lda_di(); break;
				case 0x97: sta_di(); break;
				case 0x98: eora_di(); break;
				case 0x99: adca_di(); break;
				case 0x9a: ora_di(); break;
				case 0x9b: adda_di(); break;
				case 0x9c: cmpx_di(); break;
				case 0x9d: jsr_di(); break;
				case 0x9e: ldx_di(); break;
				case 0x9f: stx_di(); break;
				case 0xa0: suba_ix(); break;
				case 0xa1: cmpa_ix(); break;
				case 0xa2: sbca_ix(); break;
				case 0xa3: subd_ix(); break;
				case 0xa4: anda_ix(); break;
				case 0xa5: bita_ix(); break;
				case 0xa6: lda_ix(); break;
				case 0xa7: sta_ix(); break;
				case 0xa8: eora_ix(); break;
				case 0xa9: adca_ix(); break;
				case 0xaa: ora_ix(); break;
				case 0xab: adda_ix(); break;
				case 0xac: cmpx_ix(); break;
				case 0xad: jsr_ix(); break;
				case 0xae: ldx_ix(); break;
				case 0xaf: stx_ix(); break;
				case 0xb0: suba_ex(); break;
				case 0xb1: cmpa_ex(); break;
				case 0xb2: sbca_ex(); break;
				case 0xb3: subd_ex(); break;
				case 0xb4: anda_ex(); break;
				case 0xb5: bita_ex(); break;
				case 0xb6: lda_ex(); break;
				case 0xb7: sta_ex(); break;
				case 0xb8: eora_ex(); break;
				case 0xb9: adca_ex(); break;
				case 0xba: ora_ex(); break;
				case 0xbb: adda_ex(); break;
				case 0xbc: cmpx_ex(); break;
				case 0xbd: jsr_ex(); break;
				case 0xbe: ldx_ex(); break;
				case 0xbf: stx_ex(); break;
				case 0xc0: subb_im(); break;
				case 0xc1: cmpb_im(); break;
				case 0xc2: sbcb_im(); break;
				case 0xc3: addd_im(); break;
				case 0xc4: andb_im(); break;
				case 0xc5: bitb_im(); break;
				case 0xc6: ldb_im(); break;
				case 0xc7: stb_im(); break; /* ILLEGAL? */
				case 0xc8: eorb_im(); break;
				case 0xc9: adcb_im(); break;
				case 0xca: orb_im(); break;
				case 0xcb: addb_im(); break;
				case 0xcc: ldd_im(); break;
				case 0xcd: std_im(); break; /* ILLEGAL? */
				case 0xce: ldu_im(); break;
				case 0xcf: stu_im(); break; /* ILLEGAL? */
				case 0xd0: subb_di(); break;
				case 0xd1: cmpb_di(); break;
				case 0xd2: sbcb_di(); break;
				case 0xd3: addd_di(); break;
				case 0xd4: andb_di(); break;
				case 0xd5: bitb_di(); break;
				case 0xd6: ldb_di(); break;
				case 0xd7: stb_di(); break;
				case 0xd8: eorb_di(); break;
				case 0xd9: adcb_di(); break;
				case 0xda: orb_di(); break;
				case 0xdb: addb_di(); break;
				case 0xdc: ldd_di(); break;
				case 0xdd: std_di(); break;
				case 0xde: ldu_di(); break;
				case 0xdf: stu_di(); break;
				case 0xe0: subb_ix(); break;
				case 0xe1: cmpb_ix(); break;
				case 0xe2: sbcb_ix(); break;
				case 0xe3: addd_ix(); break;
				case 0xe4: andb_ix(); break;
				case 0xe5: bitb_ix(); break;
				case 0xe6: ldb_ix(); break;
				case 0xe7: stb_ix(); break;
				case 0xe8: eorb_ix(); break;
				case 0xe9: adcb_ix(); break;
				case 0xea: orb_ix(); break;
				case 0xeb: addb_ix(); break;
				case 0xec: ldd_ix(); break;
				case 0xed: std_ix(); break;
				case 0xee: ldu_ix(); break;
				case 0xef: stu_ix(); break;
				case 0xf0: subb_ex(); break;
				case 0xf1: cmpb_ex(); break;
				case 0xf2: sbcb_ex(); break;
				case 0xf3: addd_ex(); break;
				case 0xf4: andb_ex(); break;
				case 0xf5: bitb_ex(); break;
				case 0xf6: ldb_ex(); break;
				case 0xf7: stb_ex(); break;
				case 0xf8: eorb_ex(); break;
				case 0xf9: adcb_ex(); break;
				case 0xfa: orb_ex(); break;
				case 0xfb: addb_ex(); break;
				case 0xfc: ldd_ex(); break;
				case 0xfd: std_ex(); break;
				case 0xfe: ldu_ex(); break;
				case 0xff: stu_ex(); break;
			}
			m6809_1_ICount -= cycles[ireg];
		}
		else
		{
			word iregw;
			iregw = ireg; iregw<<=8;
			if( fastopcodes ) iregw|=M_RDOP(pcreg);
			else iregw|=M_RDMEM(pcreg);
			pcreg++;

			if(haspostbyte[iregw&0xff]) fetch_effective_address();

			switch( iregw )
			{
				case 0x1021: lbrn(); break;
				case 0x1022: lbhi(); break;
				case 0x1023: lbls(); break;
				case 0x1024: lbcc(); break;
				case 0x1025: lbcs(); break;
				case 0x1026: lbne(); break;
				case 0x1027: lbeq(); break;
				case 0x1028: lbvc(); break;
				case 0x1029: lbvs(); break;
				case 0x102a: lbpl(); break;
				case 0x102b: lbmi(); break;
				case 0x102c: lbge(); break;
				case 0x102d: lblt(); break;
				case 0x102e: lbgt(); break;
				case 0x102f: lble(); break;
				case 0x103f: swi2(); break;
				case 0x1083: cmpd_im(); break;
				case 0x108c: cmpy_im(); break;
				case 0x108e: ldy_im(); break;
				case 0x108f: sty_im(); break; /* ILLEGAL? */
				case 0x1093: cmpd_di(); break;
				case 0x109c: cmpy_di(); break;
				case 0x109e: ldy_di(); break;
				case 0x109f: sty_di(); break;
				case 0x10a3: cmpd_ix(); break;
				case 0x10ac: cmpy_ix(); break;
				case 0x10ae: ldy_ix(); break;
				case 0x10af: sty_ix(); break;
				case 0x10b3: cmpd_ex(); break;
				case 0x10bc: cmpy_ex(); break;
				case 0x10be: ldy_ex(); break;
				case 0x10bf: sty_ex(); break;
				case 0x10ce: lds_im(); break;
				case 0x10cf: sts_im(); break; /* ILLEGAL? */
				case 0x10de: lds_di(); break;
				case 0x10df: sts_di(); break;
				case 0x10ee: lds_ix(); break;
				case 0x10ef: sts_ix(); break;
				case 0x10fe: lds_ex(); break;
				case 0x10ff: sts_ex(); break;
				case 0x113f: swi3(); break;
				case 0x1183: cmpu_im(); break;
				case 0x118c: cmps_im(); break;
				case 0x1193: cmpu_di(); break;
				case 0x119c: cmps_di(); break;
				case 0x11a3: cmpu_ix(); break;
				case 0x11ac: cmps_ix(); break;
				case 0x11b3: cmpu_ex(); break;
				case 0x11bc: cmps_ex(); break;
				default: illegal(); break;
			}
			m6809_1_ICount -= cycles2[iregw&0xff];
		}

		if( m6809_1_IRequest==INT_FIRQ )
		{
			m6809_1_IRequest = INT_NONE;
			m6809_1_FIRQ();
		}
	}
	while( m6809_1_ICount>0 );

	m6809_1_IRequest = INT_NONE;
	if( saved_icount ) 
		m6809_1_ICount = saved_icount; 
	else /* JB 970824 */
	m6809_1_ICount = m6809_1_IPeriod;
	saved_icount = 0;	/* JB 970824 */

	/* Interrupt if needed  */
	if(cpu_interrupt_1()==INT_IRQ) m6809_1_Interrupt();
}

INLINE_STATIC void fetch_effective_address( void )
{
	byte postbyte;

	if( fastopcodes ) postbyte=M_RDOP(pcreg);
	else postbyte = M_RDMEM(pcreg);
	pcreg++;
	switch(postbyte)
	{
	    case 0x00: eaddr=xreg;break;
	    case 0x01: eaddr=xreg+1;break;
	    case 0x02: eaddr=xreg+2;break;
	    case 0x03: eaddr=xreg+3;break;
	    case 0x04: eaddr=xreg+4;break;
	    case 0x05: eaddr=xreg+5;break;
	    case 0x06: eaddr=xreg+6;break;
	    case 0x07: eaddr=xreg+7;break;
	    case 0x08: eaddr=xreg+8;break;
	    case 0x09: eaddr=xreg+9;break;
	    case 0x0A: eaddr=xreg+10;break;
	    case 0x0B: eaddr=xreg+11;break;
	    case 0x0C: eaddr=xreg+12;break;
	    case 0x0D: eaddr=xreg+13;break;
	    case 0x0E: eaddr=xreg+14;break;
	    case 0x0F: eaddr=xreg+15;break;
	    case 0x10: eaddr=xreg-16;break;
	    case 0x11: eaddr=xreg-15;break;
	    case 0x12: eaddr=xreg-14;break;
	    case 0x13: eaddr=xreg-13;break;
	    case 0x14: eaddr=xreg-12;break;
	    case 0x15: eaddr=xreg-11;break;
	    case 0x16: eaddr=xreg-10;break;
	    case 0x17: eaddr=xreg-9;break;
	    case 0x18: eaddr=xreg-8;break;
	    case 0x19: eaddr=xreg-7;break;
	    case 0x1A: eaddr=xreg-6;break;
	    case 0x1B: eaddr=xreg-5;break;
	    case 0x1C: eaddr=xreg-4;break;
	    case 0x1D: eaddr=xreg-3;break;
	    case 0x1E: eaddr=xreg-2;break;
	    case 0x1F: eaddr=xreg-1;break;
	    case 0x20: eaddr=yreg;break;
	    case 0x21: eaddr=yreg+1;break;
	    case 0x22: eaddr=yreg+2;break;
	    case 0x23: eaddr=yreg+3;break;
	    case 0x24: eaddr=yreg+4;break;
	    case 0x25: eaddr=yreg+5;break;
	    case 0x26: eaddr=yreg+6;break;
	    case 0x27: eaddr=yreg+7;break;
	    case 0x28: eaddr=yreg+8;break;
	    case 0x29: eaddr=yreg+9;break;
	    case 0x2A: eaddr=yreg+10;break;
	    case 0x2B: eaddr=yreg+11;break;
	    case 0x2C: eaddr=yreg+12;break;
	    case 0x2D: eaddr=yreg+13;break;
	    case 0x2E: eaddr=yreg+14;break;
	    case 0x2F: eaddr=yreg+15;break;
	    case 0x30: eaddr=yreg-16;break;
	    case 0x31: eaddr=yreg-15;break;
	    case 0x32: eaddr=yreg-14;break;
	    case 0x33: eaddr=yreg-13;break;
	    case 0x34: eaddr=yreg-12;break;
	    case 0x35: eaddr=yreg-11;break;
	    case 0x36: eaddr=yreg-10;break;
	    case 0x37: eaddr=yreg-9;break;
	    case 0x38: eaddr=yreg-8;break;
	    case 0x39: eaddr=yreg-7;break;
	    case 0x3A: eaddr=yreg-6;break;
	    case 0x3B: eaddr=yreg-5;break;
	    case 0x3C: eaddr=yreg-4;break;
	    case 0x3D: eaddr=yreg-3;break;
	    case 0x3E: eaddr=yreg-2;break;
	    case 0x3F: eaddr=yreg-1;break;
	    case 0x40: eaddr=ureg;break;
	    case 0x41: eaddr=ureg+1;break;
	    case 0x42: eaddr=ureg+2;break;
	    case 0x43: eaddr=ureg+3;break;
	    case 0x44: eaddr=ureg+4;break;
	    case 0x45: eaddr=ureg+5;break;
	    case 0x46: eaddr=ureg+6;break;
	    case 0x47: eaddr=ureg+7;break;
	    case 0x48: eaddr=ureg+8;break;
	    case 0x49: eaddr=ureg+9;break;
	    case 0x4A: eaddr=ureg+10;break;
	    case 0x4B: eaddr=ureg+11;break;
	    case 0x4C: eaddr=ureg+12;break;
	    case 0x4D: eaddr=ureg+13;break;
	    case 0x4E: eaddr=ureg+14;break;
	    case 0x4F: eaddr=ureg+15;break;
	    case 0x50: eaddr=ureg-16;break;
	    case 0x51: eaddr=ureg-15;break;
	    case 0x52: eaddr=ureg-14;break;
	    case 0x53: eaddr=ureg-13;break;
	    case 0x54: eaddr=ureg-12;break;
	    case 0x55: eaddr=ureg-11;break;
	    case 0x56: eaddr=ureg-10;break;
	    case 0x57: eaddr=ureg-9;break;
	    case 0x58: eaddr=ureg-8;break;
	    case 0x59: eaddr=ureg-7;break;
	    case 0x5A: eaddr=ureg-6;break;
	    case 0x5B: eaddr=ureg-5;break;
	    case 0x5C: eaddr=ureg-4;break;
	    case 0x5D: eaddr=ureg-3;break;
	    case 0x5E: eaddr=ureg-2;break;
	    case 0x5F: eaddr=ureg-1;break;
	    case 0x60: eaddr=sreg;break;
	    case 0x61: eaddr=sreg+1;break;
	    case 0x62: eaddr=sreg+2;break;
	    case 0x63: eaddr=sreg+3;break;
	    case 0x64: eaddr=sreg+4;break;
	    case 0x65: eaddr=sreg+5;break;
	    case 0x66: eaddr=sreg+6;break;
	    case 0x67: eaddr=sreg+7;break;
	    case 0x68: eaddr=sreg+8;break;
	    case 0x69: eaddr=sreg+9;break;
	    case 0x6A: eaddr=sreg+10;break;
	    case 0x6B: eaddr=sreg+11;break;
	    case 0x6C: eaddr=sreg+12;break;
	    case 0x6D: eaddr=sreg+13;break;
	    case 0x6E: eaddr=sreg+14;break;
	    case 0x6F: eaddr=sreg+15;break;
	    case 0x70: eaddr=sreg-16;break;
	    case 0x71: eaddr=sreg-15;break;
	    case 0x72: eaddr=sreg-14;break;
	    case 0x73: eaddr=sreg-13;break;
	    case 0x74: eaddr=sreg-12;break;
	    case 0x75: eaddr=sreg-11;break;
	    case 0x76: eaddr=sreg-10;break;
	    case 0x77: eaddr=sreg-9;break;
	    case 0x78: eaddr=sreg-8;break;
	    case 0x79: eaddr=sreg-7;break;
	    case 0x7A: eaddr=sreg-6;break;
	    case 0x7B: eaddr=sreg-5;break;
	    case 0x7C: eaddr=sreg-4;break;
	    case 0x7D: eaddr=sreg-3;break;
	    case 0x7E: eaddr=sreg-2;break;
	    case 0x7F: eaddr=sreg-1;break;
	    case 0x80: eaddr=xreg;xreg++;break;
	    case 0x81: eaddr=xreg;xreg+=2;break;
	    case 0x82: xreg--;eaddr=xreg;break;
	    case 0x83: xreg-=2;eaddr=xreg;break;
	    case 0x84: eaddr=xreg;break;
	    case 0x85: eaddr=xreg+SIGNED(breg);break;
	    case 0x86: eaddr=xreg+SIGNED(areg);break;
	    case 0x87: eaddr=0;break; /*ILLEGAL*/
	    case 0x88: IMMBYTE(eaddr);eaddr=xreg+SIGNED(eaddr);break;
	    case 0x89: IMMWORD(eaddr);eaddr+=xreg;break;
	    case 0x8A: eaddr=0;break; /*ILLEGAL*/
	    case 0x8B: eaddr=xreg+GETDREG;break;
	    case 0x8C: IMMBYTE(eaddr);eaddr=pcreg+SIGNED(eaddr);break;
	    case 0x8D: IMMWORD(eaddr);eaddr+=pcreg;break;
	    case 0x8E: eaddr=0;break; /*ILLEGAL*/
	    case 0x8F: IMMWORD(eaddr);break;
	    case 0x90: eaddr=xreg;xreg++;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0x91: eaddr=xreg;xreg+=2;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0x92: xreg--;eaddr=xreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0x93: xreg-=2;eaddr=xreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0x94: eaddr=xreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0x95: eaddr=xreg+SIGNED(breg);eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0x96: eaddr=xreg+SIGNED(areg);eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0x97: eaddr=0;break; /*ILLEGAL*/
	    case 0x98: IMMBYTE(eaddr);eaddr=xreg+SIGNED(eaddr);
	               eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0x99: IMMWORD(eaddr);eaddr+=xreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0x9A: eaddr=0;break; /*ILLEGAL*/
	    case 0x9B: eaddr=xreg+GETDREG;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0x9C: IMMBYTE(eaddr);eaddr=pcreg+SIGNED(eaddr);
	               eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0x9D: IMMWORD(eaddr);eaddr+=pcreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0x9E: eaddr=0;break; /*ILLEGAL*/
	    case 0x9F: IMMWORD(eaddr);eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xA0: eaddr=yreg;yreg++;break;
	    case 0xA1: eaddr=yreg;yreg+=2;break;
	    case 0xA2: yreg--;eaddr=yreg;break;
	    case 0xA3: yreg-=2;eaddr=yreg;break;
	    case 0xA4: eaddr=yreg;break;
	    case 0xA5: eaddr=yreg+SIGNED(breg);break;
	    case 0xA6: eaddr=yreg+SIGNED(areg);break;
	    case 0xA7: eaddr=0;break; /*ILLEGAL*/
	    case 0xA8: IMMBYTE(eaddr);eaddr=yreg+SIGNED(eaddr);break;
	    case 0xA9: IMMWORD(eaddr);eaddr+=yreg;break;
	    case 0xAA: eaddr=0;break; /*ILLEGAL*/
	    case 0xAB: eaddr=yreg+GETDREG;break;
	    case 0xAC: IMMBYTE(eaddr);eaddr=pcreg+SIGNED(eaddr);break;
	    case 0xAD: IMMWORD(eaddr);eaddr+=pcreg;break;
	    case 0xAE: eaddr=0;break; /*ILLEGAL*/
	    case 0xAF: IMMWORD(eaddr);break;
	    case 0xB0: eaddr=yreg;yreg++;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xB1: eaddr=yreg;yreg+=2;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xB2: yreg--;eaddr=yreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xB3: yreg-=2;eaddr=yreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xB4: eaddr=yreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xB5: eaddr=yreg+SIGNED(breg);eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xB6: eaddr=yreg+SIGNED(areg);eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xB7: eaddr=0;break; /*ILLEGAL*/
	    case 0xB8: IMMBYTE(eaddr);eaddr=yreg+SIGNED(eaddr);
	               eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xB9: IMMWORD(eaddr);eaddr+=yreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xBA: eaddr=0;break; /*ILLEGAL*/
	    case 0xBB: eaddr=yreg+GETDREG;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xBC: IMMBYTE(eaddr);eaddr=pcreg+SIGNED(eaddr);
	               eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xBD: IMMWORD(eaddr);eaddr+=pcreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xBE: eaddr=0;break; /*ILLEGAL*/
	    case 0xBF: IMMWORD(eaddr);eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xC0: eaddr=ureg;ureg++;break;
	    case 0xC1: eaddr=ureg;ureg+=2;break;
	    case 0xC2: ureg--;eaddr=ureg;break;
	    case 0xC3: ureg-=2;eaddr=ureg;break;
	    case 0xC4: eaddr=ureg;break;
	    case 0xC5: eaddr=ureg+SIGNED(breg);break;
	    case 0xC6: eaddr=ureg+SIGNED(areg);break;
	    case 0xC7: eaddr=0;break; /*ILLEGAL*/
	    case 0xC8: IMMBYTE(eaddr);eaddr=ureg+SIGNED(eaddr);break;
	    case 0xC9: IMMWORD(eaddr);eaddr+=ureg;break;
	    case 0xCA: eaddr=0;break; /*ILLEGAL*/
	    case 0xCB: eaddr=ureg+GETDREG;break;
	    case 0xCC: IMMBYTE(eaddr);eaddr=pcreg+SIGNED(eaddr);break;
	    case 0xCD: IMMWORD(eaddr);eaddr+=pcreg;break;
	    case 0xCE: eaddr=0;break; /*ILLEGAL*/
	    case 0xCF: IMMWORD(eaddr);break;
	    case 0xD0: eaddr=ureg;ureg++;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xD1: eaddr=ureg;ureg+=2;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xD2: ureg--;eaddr=ureg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xD3: ureg-=2;eaddr=ureg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xD4: eaddr=ureg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xD5: eaddr=ureg+SIGNED(breg);eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xD6: eaddr=ureg+SIGNED(areg);eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xD7: eaddr=0;break; /*ILLEGAL*/
	    case 0xD8: IMMBYTE(eaddr);eaddr=ureg+SIGNED(eaddr);
	               eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xD9: IMMWORD(eaddr);eaddr+=ureg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xDA: eaddr=0;break; /*ILLEGAL*/
	    case 0xDB: eaddr=ureg+GETDREG;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xDC: IMMBYTE(eaddr);eaddr=pcreg+SIGNED(eaddr);
	               eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xDD: IMMWORD(eaddr);eaddr+=pcreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xDE: eaddr=0;break; /*ILLEGAL*/
	    case 0xDF: IMMWORD(eaddr);eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xE0: eaddr=sreg;sreg++;break;
	    case 0xE1: eaddr=sreg;sreg+=2;break;
	    case 0xE2: sreg--;eaddr=sreg;break;
	    case 0xE3: sreg-=2;eaddr=sreg;break;
	    case 0xE4: eaddr=sreg;break;
	    case 0xE5: eaddr=sreg+SIGNED(breg);break;
	    case 0xE6: eaddr=sreg+SIGNED(areg);break;
	    case 0xE7: eaddr=0;break; /*ILLEGAL*/
	    case 0xE8: IMMBYTE(eaddr);eaddr=sreg+SIGNED(eaddr);break;
	    case 0xE9: IMMWORD(eaddr);eaddr+=sreg;break;
	    case 0xEA: eaddr=0;break; /*ILLEGAL*/
	    case 0xEB: eaddr=sreg+GETDREG;break;
	    case 0xEC: IMMBYTE(eaddr);eaddr=pcreg+SIGNED(eaddr);break;
	    case 0xED: IMMWORD(eaddr);eaddr+=pcreg;break;
	    case 0xEE: eaddr=0;break; /*ILLEGAL*/
	    case 0xEF: IMMWORD(eaddr);break;
	    case 0xF0: eaddr=sreg;sreg++;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xF1: eaddr=sreg;sreg+=2;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xF2: sreg--;eaddr=sreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xF3: sreg-=2;eaddr=sreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xF4: eaddr=sreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xF5: eaddr=sreg+SIGNED(breg);eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xF6: eaddr=sreg+SIGNED(areg);eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xF7: eaddr=0;break; /*ILLEGAL*/
	    case 0xF8: IMMBYTE(eaddr);eaddr=sreg+SIGNED(eaddr);
	               eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xF9: IMMWORD(eaddr);eaddr+=sreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xFA: eaddr=0;break; /*ILLEGAL*/
	    case 0xFB: eaddr=sreg+GETDREG;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xFC: IMMBYTE(eaddr);eaddr=pcreg+SIGNED(eaddr);
	               eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xFD: IMMWORD(eaddr);eaddr+=pcreg;eaddr=M_RDMEM_WORD(eaddr);break;
	    case 0xFE: eaddr=0;break; /*ILLEGAL*/
	    case 0xFF: IMMWORD(eaddr);eaddr=M_RDMEM_WORD(eaddr);break;
	}
}
