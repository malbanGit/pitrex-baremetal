/***************************************************************************

    ccpu.h
    Core implementation for the portable Cinematronics CPU emulator.

    Written by Aaron Giles
    Special thanks to Zonn Moore for his detailed documentation.

***************************************************************************/

#ifndef _CCPU_H_
#define	_CCPU_H_


//#include "basetsd.h"
#include "osd_cpu.h"


/***************************************************************************
    REGISTER ENUMERATION
***************************************************************************/


enum
{
	CCPU_PC=1,
	CCPU_FLAGS,
	CCPU_A,
	CCPU_B,
	CCPU_I,
	CCPU_J,
	CCPU_P,
	CCPU_X,
	CCPU_Y,
	CCPU_T
};



/***************************************************************************
    CONFIG STRUCTURE
***************************************************************************/

struct CCPUConfig
{
	UINT8		(*external_input)(void);		/* if NULL, assume JMI jumper is present */
	void		(*vector_callback)(INT16 sx, INT16 sy, INT16 ex, INT16 ey, UINT8 shift);
};
/**************************************************************************
    PUBLIC VARS
**************************************************************************/
extern  UINT8 MUX_VAL;
extern  UINT8 SOUNDBITS;
extern  UINT8 CCPUROMSIZE;

/***************************************************************************
    PUBLIC FUNCTIONS
***************************************************************************/

//void ccpu_get_info(UINT32 state, cpuinfo *info);
void ccpu_wdt_timer_trigger(void);
int run_ccpu(int cycles);
int cpunum_get_reg(int cpunum, int reg);
int cpunum_set_reg(int cpunum, int reg, INT16 val);
void vec_control_write (int data);
void coin_handler( int data );
void init_ccpu( int val);
void ccpu_reset();
UINT8 joystick_read(void);
UINT8 get_ccpu_inputs( int offset);
UINT8 get_ccpu_switches( int offset );
int get_ccpu_ticks();

#endif
