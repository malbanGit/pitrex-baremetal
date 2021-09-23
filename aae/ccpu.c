/***************************************************************************

    ccpu.c
    Core implementation for the portable Cinematronics CPU emulator.

    Written by Aaron Giles
    Special thanks to Zonn Moore for his detailed documentation.

***************************************************************************/

//#include "debugger.h"
//#include "deprecat.h"


//#include "globals.h"
//#include "samples.h"
//#include "vector.h"
//#include "glcode.h"
//#include "dips.h"
//#include "keysets.h"
#include "input.h"
//#include "cpuintfaae.h"

/***************************************************************************
    STRUCTURES & TYPEDEFS
***************************************************************************/

//#include "basetsd.h"
#include "ccpu.h"
#include "globals.h"

extern  void cinemat_vector_callback(INT16 sx, INT16 sy, INT16 ex, INT16 ey, UINT8 shift);
extern  void cini_sound_control_w(int offset, int data);

typedef struct
{
    UINT16		PC;
    UINT16		A;
    UINT16		B;
    UINT8		I;
    UINT16		J;
    UINT8		P;
    UINT16		X;
    UINT16		Y;
    UINT16		T;
    UINT16 *	acc;

    UINT16		a0flag, ncflag, cmpacc, cmpval;
    UINT16		miflag, nextmiflag, nextnextmiflag;
    UINT16		drflag;

	UINT8		(*external_input)(void);
	void		(*vector_callback)(INT16 sx, INT16 sy, INT16 ex, INT16 ey, UINT8 shift);

	UINT8		waiting;
	UINT8		watchdog;
} ccpuRegs;



#ifndef FALSE
#    define FALSE 0
#endif
#ifndef TRUE
#    define TRUE (!FALSE)
#endif
/***************************************************************************
    PRIVATE GLOBAL VARIABLES
***************************************************************************/

static ccpuRegs ccpu;
static int ccpu_icount;

UINT16 CCPURAM[256];

UINT8 outport[16];
/***************************************************************************
    MACROS
***************************************************************************/

#define READOP(a) 			(GI[CPU0][a])

#define RDMEM(a)			(CCPURAM[a])       //(data_read_word_16be((a) * 2) & 0xfff)
#define WRMEM(a,v)			(CCPURAM[a] = v )     //(data_write_word_16be((a) * 2, (v)))

#define READPORT(a)			(getport(a))
#define WRITEPORT(a,v)		(outport[a]=v)

#define SET_A0()			do { ccpu.a0flag = ccpu.A; } while (0)
#define SET_CMP_VAL(x)		do { ccpu.cmpacc = *ccpu.acc; ccpu.cmpval = (x) & 0xfff; } while (0)
#define SET_NC(a)			do { ccpu.ncflag = ~(a); } while (0)
#define SET_MI(a)			do { ccpu.nextnextmiflag = (a); } while (0)

#define TEST_A0()			(ccpu.a0flag & 1)
#define TEST_NC()			((ccpu.ncflag >> 12) & 1)
#define TEST_MI()			((ccpu.miflag >> 11) & 1)
#define TEST_LT()			(ccpu.cmpval < ccpu.cmpacc)
#define TEST_EQ()			(ccpu.cmpval == ccpu.cmpacc)
#define TEST_DR()			(ccpu.drflag != 0)

#define NEXT_ACC_A()		do { SET_MI(*ccpu.acc); ccpu.acc = &ccpu.A; } while (0)
#define NEXT_ACC_B()		do { SET_MI(*ccpu.acc); if (ccpu.acc == &ccpu.A) ccpu.acc = &ccpu.B; else ccpu.acc = &ccpu.A; } while (0)

#define CYCLES(x)			do { ccpu_icount -= (x); } while (0)

#define STANDARD_ACC_OP(resexp,cmpval) \
do { \
	UINT16 result = resexp; \
	SET_A0();						/* set the A0 bit based on the previous 'A' value */ \
	SET_CMP_VAL(cmpval);			/* set the compare values to the previous accumulator and the cmpval */ \
	SET_NC(result);					/* set the NC flag based on the unmasked result */ \
	*ccpu.acc = result & 0xfff;		/* store the low 12 bits of the new value */ \
} while (0)



/***************************************************************************
    CONTEXT SWITCHING
***************************************************************************/

static void ccpu_get_context(void *dst)
{
	/* copy the context */
	*(ccpuRegs *)dst = ccpu;
}


static void ccpu_set_context(void *src)
{
	/* copy the context */
	if (src)
		ccpu = *(ccpuRegs *)src;
	//change_pc(ccpu.PC);
}


/***************************************************************************
    INITIALIZATION AND SHUTDOWN
***************************************************************************/

static UINT8 read_jmi(void)
{
	/* this routine is called when there is no external input */
	/* and the JMI jumper is present */
	return TEST_MI();
}

int cpunum_get_reg(int cpunum, int reg)
{
	if (reg ==CCPU_X) return ccpu.X;
	else if (reg==CCPU_Y) return ccpu.Y;
	
	return 0;
}

int cpunum_set_reg(int cpunum, int reg, INT16 val)
{
	if (reg ==CCPU_X)  ccpu.X=val;
	else if (reg==CCPU_Y) ccpu.Y=val;

	return 0;
}

int get_ccpu_ticks()
{
return ccpu_icount;
}

void ccpu_wdt_timer_trigger(void)
{
	ccpu.waiting = 0;
	ccpu.watchdog++;
	if (gamenum != SUNDANCE)
	{

	 if (ccpu.watchdog >= 3)
		ccpu.PC = 0;
		}
}


static void ccpu_init(int index, int clock, const void *_config, int (*irqcallback)(int))
{
	const struct CCPUConfig *config = _config;

	/* copy input params */
	ccpu.external_input = config->external_input ? config->external_input : read_jmi;
	ccpu.vector_callback = config->vector_callback;
}



void ccpu_reset(void)
{
	/* zero registers */
	ccpu.PC = 0;
	ccpu.A = 0;
	ccpu.B = 0;
	ccpu.I = 0;
	ccpu.J = 0;
	ccpu.P = 0;
	ccpu.X = 0;
	ccpu.Y = 0;
	ccpu.T = 0;
	ccpu.acc = &ccpu.A;

	/* zero flags */
	ccpu.a0flag = 0;
	ccpu.ncflag = 0;
	ccpu.cmpacc = 0;
	ccpu.cmpval = 1;
	ccpu.miflag = ccpu.nextmiflag = ccpu.nextnextmiflag = 0;
	ccpu.drflag = 0;

	ccpu.waiting = 0;
	ccpu.watchdog = 0;
}

void init_ccpu( int val)
{
   
     ccpu_reset();
	/* copy input params */
	if (val) ccpu.external_input = joystick_read; else ccpu.external_input= read_jmi;//config->external_input ? config->external_input : read_jmi;

	ccpu.vector_callback = cinemat_vector_callback;
}

/***************************************************************************
    CORE EXECUTION LOOP
***************************************************************************/

static int ccpu_execute(int cycles)
{
	if (ccpu.waiting)
		return cycles;

	ccpu_icount = cycles;

	while (ccpu_icount >= 0)
	{
		UINT16 tempval;
   		UINT8 opcode;

		/* update the delayed MI flag */
		ccpu.miflag = ccpu.nextmiflag;
		ccpu.nextmiflag = ccpu.nextnextmiflag;

		/* fetch the opcode */
		
		opcode = READOP(ccpu.PC++);

		switch (opcode)
		{
			/* LDAI */
			case 0x00:	case 0x01:	case 0x02:	case 0x03:
			case 0x04:	case 0x05:	case 0x06:	case 0x07:
			case 0x08:	case 0x09:	case 0x0a:	case 0x0b:
			case 0x0c:	case 0x0d:	case 0x0e:	case 0x0f:
				tempval = (opcode & 0x0f) << 8;
				STANDARD_ACC_OP(tempval, tempval);
				NEXT_ACC_A(); CYCLES(1);
				break;

			/* INP */
			case 0x10:	case 0x11:	case 0x12:	case 0x13:
			case 0x14:	case 0x15:	case 0x16:	case 0x17:
			case 0x18:	case 0x19:	case 0x1a:	case 0x1b:
			case 0x1c:	case 0x1d:	case 0x1e:	case 0x1f:
				if (ccpu.acc == &ccpu.A)
				     {
					 tempval = get_ccpu_inputs (opcode & 0x0f) & 1;        //READPORT(opcode & 0x0f) & 1; //Read inputs
				     }
				else {
					  tempval = get_ccpu_switches((opcode & 0x07)) & 1; //log_it("Switch offset %x",opcode & 0x07);//READPORT(16 + (opcode & 0x07)) & 1; //Read Dip Switches
				      }
				STANDARD_ACC_OP(tempval, tempval);
				NEXT_ACC_A(); CYCLES(1);
				break;

			/* A8I */
			case 0x20:
				tempval = READOP(ccpu.PC++);
				STANDARD_ACC_OP(*ccpu.acc + tempval, tempval);
				NEXT_ACC_A(); CYCLES(3);
				break;

			/* A4I */
			case 0x21:	case 0x22:	case 0x23:
			case 0x24:	case 0x25:	case 0x26:	case 0x27:
			case 0x28:	case 0x29:	case 0x2a:	case 0x2b:
			case 0x2c:	case 0x2d:	case 0x2e:	case 0x2f:
				tempval = opcode & 0x0f;
				STANDARD_ACC_OP(*ccpu.acc + tempval, tempval);
				NEXT_ACC_A(); CYCLES(1);
				break;

			/* S8I */
			case 0x30:
				tempval = READOP(ccpu.PC++);
				STANDARD_ACC_OP(*ccpu.acc + (tempval ^ 0xfff) + 1, tempval);
				NEXT_ACC_A(); CYCLES(3);
				break;

			/* S4I */
			case 0x31:	case 0x32:	case 0x33:
			case 0x34:	case 0x35:	case 0x36:	case 0x37:
			case 0x38:	case 0x39:	case 0x3a:	case 0x3b:
			case 0x3c:	case 0x3d:	case 0x3e:	case 0x3f:
				tempval = opcode & 0x0f;
				STANDARD_ACC_OP(*ccpu.acc + (tempval ^ 0xfff) + 1, tempval);
				NEXT_ACC_A(); CYCLES(1);
				break;

			/* LPAI */
			case 0x40:	case 0x41:	case 0x42:	case 0x43:
			case 0x44:	case 0x45:	case 0x46:	case 0x47:
			case 0x48:	case 0x49:	case 0x4a:	case 0x4b:
			case 0x4c:	case 0x4d:	case 0x4e:	case 0x4f:
				tempval = READOP(ccpu.PC++);
				ccpu.J = (opcode & 0x0f) + (tempval & 0xf0) + ((tempval & 0x0f) << 8);
				NEXT_ACC_A(); CYCLES(3);
				break;

			/* T4K */
			case 0x50:
				//log_it("PC BEFORE BANKSWITCH %x", ccpu.PC);
				if (CCPUROMSIZE < 32)
				{
				tempval = (((ccpu.P & 0x03)-1) << 12); 
				ccpu.PC = (tempval ) + ccpu.J;
				if (CCPUROMSIZE > 8) ccpu.PC +=0x1000;
				}
				else ccpu.PC = (ccpu.P << 12) + ccpu.J;
				//log_it("PC AFTER BANKSWITCH %x", ccpu.PC);
				//change_pc(ccpu.PC);
				
				NEXT_ACC_B(); CYCLES(4);
				break;

			/* JMIB/JEHB */
			case 0x51:
				if ((*ccpu.external_input)()) { ccpu.PC = ((ccpu.PC - 1) & 0xf000) + ccpu.J; CYCLES(2); }
				NEXT_ACC_B(); CYCLES(2);
				break;

			/* JVNB */
			case 0x52:
				if (TEST_DR()) { ccpu.PC = ((ccpu.PC - 1) & 0xf000) + ccpu.J; CYCLES(2); }
				NEXT_ACC_B(); CYCLES(2);
				break;

			/* JLTB */
			case 0x53:
				if (TEST_LT()) { ccpu.PC = ((ccpu.PC - 1) & 0xf000) + ccpu.J; CYCLES(2); }
				NEXT_ACC_B(); CYCLES(2);
				break;

			/* JEQB */
			case 0x54:
				if (TEST_EQ()) { ccpu.PC = ((ccpu.PC - 1) & 0xf000) + ccpu.J; CYCLES(2); }
				NEXT_ACC_B(); CYCLES(2);
				break;

			/* JCZB */
			case 0x55:
				if (TEST_NC()) { ccpu.PC = ((ccpu.PC - 1) & 0xf000) + ccpu.J; CYCLES(2); }
				NEXT_ACC_B(); CYCLES(2);
				break;

			/* JOSB */
			case 0x56:
				if (TEST_A0()) { ccpu.PC = ((ccpu.PC - 1) & 0xf000) + ccpu.J; CYCLES(2); }
				NEXT_ACC_B(); CYCLES(2);
				break;

			/* SSA */
			case 0x57:
				NEXT_ACC_B(); CYCLES(2);
				break;

			/* JMP */
			case 0x58:
				ccpu.PC = ((ccpu.PC - 1) & 0xf000) + ccpu.J;
				NEXT_ACC_A(); CYCLES(4);
				break;

			/* JMI/JEH */
			case 0x59:
				if ((*ccpu.external_input)()) { ccpu.PC = ((ccpu.PC - 1) & 0xf000) + ccpu.J; CYCLES(2); }
				
				NEXT_ACC_A(); CYCLES(2);
				break;

			/* JVN */
			case 0x5a:
				if (TEST_DR()) { ccpu.PC = ((ccpu.PC - 1) & 0xf000) + ccpu.J; CYCLES(2); }
				NEXT_ACC_A(); CYCLES(2);
				break;

			/* JLT */
			case 0x5b:
				if (TEST_LT()) { ccpu.PC = ((ccpu.PC - 1) & 0xf000) + ccpu.J; CYCLES(2); }
				NEXT_ACC_A(); CYCLES(2);
				break;

			/* JEQ */
			case 0x5c:
				if (TEST_EQ()) { ccpu.PC = ((ccpu.PC - 1) & 0xf000) + ccpu.J; CYCLES(2); }
				NEXT_ACC_A(); CYCLES(2);
				break;

			/* JCZ */
			case 0x5d:
				if (TEST_NC()) { ccpu.PC = ((ccpu.PC - 1) & 0xf000) + ccpu.J; CYCLES(2); }
				NEXT_ACC_A(); CYCLES(2);
				break;

			/* JOS */
			case 0x5e:
				if (TEST_A0()) { ccpu.PC = ((ccpu.PC - 1) & 0xf000) + ccpu.J; CYCLES(2); }
				NEXT_ACC_A(); CYCLES(2);
				break;

			/* NOP */
			case 0x5f:
				NEXT_ACC_A(); CYCLES(2);
				break;

			/* ADD */
			case 0x60:	case 0x61:	case 0x62:	case 0x63:
			case 0x64:	case 0x65:	case 0x66:	case 0x67:
			case 0x68:	case 0x69:	case 0x6a:	case 0x6b:
			case 0x6c:	case 0x6d:	case 0x6e:	case 0x6f:
				ccpu.I = (ccpu.P << 4) + (opcode & 0x0f);
				tempval = RDMEM(ccpu.I);
				STANDARD_ACC_OP(*ccpu.acc + tempval, tempval);
				NEXT_ACC_A(); CYCLES(3);
				break;

			/* SUB */
			case 0x70:	case 0x71:	case 0x72:	case 0x73:
			case 0x74:	case 0x75:	case 0x76:	case 0x77:
			case 0x78:	case 0x79:	case 0x7a:	case 0x7b:
			case 0x7c:	case 0x7d:	case 0x7e:	case 0x7f:
				ccpu.I = (ccpu.P << 4) + (opcode & 0x0f);
				tempval = RDMEM(ccpu.I);
				STANDARD_ACC_OP(*ccpu.acc + (tempval ^ 0xfff) + 1, tempval);
				NEXT_ACC_A(); CYCLES(3);
				break;

			/* SETP */
			case 0x80:	case 0x81:	case 0x82:	case 0x83:
			case 0x84:	case 0x85:	case 0x86:	case 0x87:
			case 0x88:	case 0x89:	case 0x8a:	case 0x8b:
			case 0x8c:	case 0x8d:	case 0x8e:	case 0x8f:
				ccpu.P = opcode & 0x0f;
				NEXT_ACC_A(); CYCLES(1);
				break;

			/* OUT */
			case 0x90:	case 0x91:	case 0x92:	case 0x93:
			case 0x94:	case 0x95:	case 0x96:	case 0x97:
			case 0x98:	case 0x99:	case 0x9a:	case 0x9b:
			case 0x9c:	case 0x9d:	case 0x9e:	case 0x9f:
				if (ccpu.acc == &ccpu.A)
				{
					if ((opcode & 0x07)==5) coin_handler( ~*ccpu.acc & 1 );
					if ((opcode & 0x07)==6) vec_control_write( ~*ccpu.acc & 1);
                    if ((opcode & 0x07)==7) MUX_VAL = ~*ccpu.acc & 1;
					
					if ((opcode & 0x07) < 8) cini_sound_control_w(opcode & 0x07, ~*ccpu.acc & 1);
					//WRITEPORT(opcode & 0x07, ~*ccpu.acc & 1);
				
				}
				
				NEXT_ACC_A(); CYCLES(1);
				break;

			/* LDA */
			case 0xa0:	case 0xa1:	case 0xa2:	case 0xa3:
			case 0xa4:	case 0xa5:	case 0xa6:	case 0xa7:
			case 0xa8:	case 0xa9:	case 0xaa:	case 0xab:
			case 0xac:	case 0xad:	case 0xae:	case 0xaf:
				ccpu.I = (ccpu.P << 4) + (opcode & 0x0f);
				tempval = RDMEM(ccpu.I);
				STANDARD_ACC_OP(tempval, tempval);
				NEXT_ACC_A(); CYCLES(3);
				break;

			/* TST */
			case 0xb0:	case 0xb1:	case 0xb2:	case 0xb3:
			case 0xb4:	case 0xb5:	case 0xb6:	case 0xb7:
			case 0xb8:	case 0xb9:	case 0xba:	case 0xbb:
			case 0xbc:	case 0xbd:	case 0xbe:	case 0xbf:
				ccpu.I = (ccpu.P << 4) + (opcode & 0x0f);
				tempval = RDMEM(ccpu.I);
				{
					UINT16 result = *ccpu.acc + (tempval ^ 0xfff) + 1;
					SET_A0();
					SET_CMP_VAL(tempval);
					SET_NC(result);
					SET_MI(result);
				}
				NEXT_ACC_A(); CYCLES(3);
				break;

			/* WS */
			case 0xc0:	case 0xc1:	case 0xc2:	case 0xc3:
			case 0xc4:	case 0xc5:	case 0xc6:	case 0xc7:
			case 0xc8:	case 0xc9:	case 0xca:	case 0xcb:
			case 0xcc:	case 0xcd:	case 0xce:	case 0xcf:
				ccpu.I = (ccpu.P << 4) + (opcode & 0x0f);
				ccpu.I = RDMEM(ccpu.I) & 0xff;
				NEXT_ACC_A(); CYCLES(3);
				break;

			/* STA */
			case 0xd0:	case 0xd1:	case 0xd2:	case 0xd3:
			case 0xd4:	case 0xd5:	case 0xd6:	case 0xd7:
			case 0xd8:	case 0xd9:	case 0xda:	case 0xdb:
			case 0xdc:	case 0xdd:	case 0xde:	case 0xdf:
				ccpu.I = (ccpu.P << 4) + (opcode & 0x0f);
				WRMEM(ccpu.I, *ccpu.acc);
				NEXT_ACC_A(); CYCLES(3);
				break;

			/* DV */
			case 0xe0:
				{
					INT16 stopX = (INT16)(ccpu.A << 4) >> 4;
					INT16 stopY = (INT16)(ccpu.B << 4) >> 4;

					stopX = ((INT16)(stopX - ccpu.X) >> ccpu.T) + ccpu.X;
					stopY = ((INT16)(stopY - ccpu.Y) >> ccpu.T) + ccpu.Y;

					(*ccpu.vector_callback)(ccpu.X, ccpu.Y, stopX, stopY, ccpu.T);

					/* hack to make QB3 display semi-correctly during explosions */
					ccpu.A = ccpu.X & 0xfff;
					ccpu.B = ccpu.Y & 0xfff;
				}
				NEXT_ACC_A(); CYCLES(1);
				break;

			/* LPAP */
			case 0xe1:
				ccpu.J = RDMEM(ccpu.I);
				NEXT_ACC_A(); CYCLES(3);
				break;

			/* WSP */
			case 0xf1:
				ccpu.I = RDMEM(ccpu.I) & 0xff;
				NEXT_ACC_A(); CYCLES(3);
				break;

			/* LKP */
			case 0xe2:
			case 0xf2:
				tempval = READOP(((ccpu.PC - 1) & 0xf000) + *ccpu.acc);
				STANDARD_ACC_OP(tempval, tempval);
				NEXT_ACC_A(); CYCLES(7);
				ccpu.PC++;
				break;

			/* MUL */
			case 0xe3:
			case 0xf3:
				tempval = RDMEM(ccpu.I);
				SET_A0();
				ccpu.cmpval = tempval & 0xfff;
				if (ccpu.acc == &ccpu.A)
				{
					if (ccpu.A & 1)
					{
						UINT16 result;
						ccpu.cmpacc = ccpu.B;
						ccpu.A = (ccpu.A >> 1) | ((ccpu.B << 11) & 0x800);
						ccpu.B = ((INT16)(ccpu.B << 4) >> 5) & 0xfff;
						result = ccpu.B + tempval;
						SET_NC(result);
						SET_MI(result);
						ccpu.B = result & 0xfff;
					}
					else
					{
						UINT16 result;
						ccpu.cmpacc = ccpu.A;
						result = ccpu.A + tempval;
						ccpu.A = (ccpu.A >> 1) | ((ccpu.B << 11) & 0x800);
						ccpu.B = ((INT16)(ccpu.B << 4) >> 5) & 0xfff;
						SET_NC(result);
						SET_MI(result);
					}
				}
				else
				{
					UINT16 result;
					ccpu.cmpacc = ccpu.B;
					ccpu.B = ((INT16)(ccpu.B << 4) >> 5) & 0xfff;
					result = ccpu.B + tempval;
					SET_NC(result);
					SET_MI(result);
					if (ccpu.A & 1)
						ccpu.B = result & 0xfff;
				}
				NEXT_ACC_A(); CYCLES(2);
				break;

			/* NV */
			case 0xe4:
			case 0xf4:
				ccpu.T = 0;
				while (((ccpu.A & 0xa00) == 0x000 || (ccpu.A & 0xa00) == 0xa00) &&
					   ((ccpu.B & 0xa00) == 0x000 || (ccpu.B & 0xa00) == 0xa00) &&
					   ccpu.T < 16)
				{
					ccpu.A = (ccpu.A << 1) & 0xfff;
					ccpu.B = (ccpu.B << 1) & 0xfff;
					ccpu.T++;
					CYCLES(1);
				}
				NEXT_ACC_A(); CYCLES(1);
				break;

			/* FRM */
			case 0xe5:
			case 0xf5:
				ccpu.waiting = TRUE;
				NEXT_ACC_A();
				ccpu_icount = -1;

				/* some games repeat the FRM opcode twice; it apparently does not cause
                   a second wait, so we make sure we skip any duplicate opcode at this
                   point */
				if (READOP(ccpu.PC) == opcode)
					ccpu.PC++;
				break;

			/* STAP */
			case 0xe6:
			case 0xf6:
				WRMEM(ccpu.I, *ccpu.acc);
				NEXT_ACC_A(); CYCLES(2);
				break;

			/* CST */
			case 0xf7:
				ccpu.watchdog = 0;
			/* ADDP */
			case 0xe7:
				tempval = RDMEM(ccpu.I);
				STANDARD_ACC_OP(*ccpu.acc + tempval, tempval);
				NEXT_ACC_A(); CYCLES(2);
				break;

			/* SUBP */
			case 0xe8:
			case 0xf8:
				tempval = RDMEM(ccpu.I);
				STANDARD_ACC_OP(*ccpu.acc + (tempval ^ 0xfff) + 1, tempval);
				NEXT_ACC_A(); CYCLES(3);
				break;

			/* ANDP */
			case 0xe9:
			case 0xf9:
				tempval = RDMEM(ccpu.I);
				STANDARD_ACC_OP(*ccpu.acc & tempval, tempval);
				NEXT_ACC_A(); CYCLES(2);
				break;

			/* LDAP */
			case 0xea:
			case 0xfa:
				tempval = RDMEM(ccpu.I);
				STANDARD_ACC_OP(tempval, tempval);
				NEXT_ACC_A(); CYCLES(2);
				break;

			/* SHR */
			case 0xeb:
			case 0xfb:
				tempval = ((ccpu.acc == &ccpu.A) ? (ccpu.A >> 1) : ((INT16)(ccpu.B << 4) >> 5)) & 0xfff;
				tempval |= (*ccpu.acc + (0xb0b | (opcode & 0xf0))) & 0x1000;
				STANDARD_ACC_OP(tempval, 0xb0b | (opcode & 0xf0));
				NEXT_ACC_A(); CYCLES(1);
				break;

			/* SHL */
			case 0xec:
			case 0xfc:
				tempval = (*ccpu.acc << 1) & 0xfff;
				tempval |= (*ccpu.acc + (0xc0c | (opcode & 0xf0))) & 0x1000;
				STANDARD_ACC_OP(tempval, 0xc0c | (opcode & 0xf0));
				NEXT_ACC_A(); CYCLES(1);
				break;

			/* ASR */
			case 0xed:
			case 0xfd:
				tempval = ((INT16)(*ccpu.acc << 4) >> 5) & 0xfff;
				STANDARD_ACC_OP(tempval, 0xd0d | (opcode & 0xf0));
				NEXT_ACC_A(); CYCLES(1);
				break;

			/* SHRB */
			case 0xee:
			case 0xfe:
				if (ccpu.acc == &ccpu.A)
				{
					tempval = (ccpu.A >> 1) | ((ccpu.B << 11) & 0x800);
					ccpu.B = ((INT16)(ccpu.B << 4) >> 5) & 0xfff;
				}
				else
					tempval = ((INT16)(ccpu.B << 4) >> 5) & 0xfff;
				tempval |= (*ccpu.acc + (0xe0e | (opcode & 0xf0))) & 0x1000;
				STANDARD_ACC_OP(tempval, 0xe0e | (opcode & 0xf0));
				NEXT_ACC_A(); CYCLES(1);
				break;

			/* SHLB */
			case 0xef:
			case 0xff:
				if (ccpu.acc == &ccpu.A)
				{
					tempval = (ccpu.A << 1) & 0xfff;
					ccpu.B = (ccpu.B << 1) & 0xfff;
				}
				else
					tempval = (ccpu.B << 1) & 0xfff;
				tempval |= (*ccpu.acc + (0xf0f | (opcode & 0xf0))) & 0x1000;
				STANDARD_ACC_OP(tempval, 0xf0f | (opcode & 0xf0));
				NEXT_ACC_A(); CYCLES(1);
				break;

			/* IV */
			case 0xf0:
				ccpu.X = (INT16)(ccpu.A << 4) >> 4;
				ccpu.Y = (INT16)(ccpu.B << 4) >> 4;
				NEXT_ACC_A(); CYCLES(1);
				break;
		}
	}

	return cycles - ccpu_icount;
}


int run_ccpu(int cycles)
{
  return ccpu_execute( cycles);
}