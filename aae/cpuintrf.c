/***************************************************************************

  cpuintrf.c

  Don't you love MS-DOS 8+3 names? That stands for CPU interface.
  Functions needed to interface the CPU emulator with the other parts of
  the emulation.

***************************************************************************/

#include "driver.h"
#include "globals.h"
#include "m6809/m6809_1.h"
#include "m6809/m6809_2.h"

#include <vectrex/vectrexInterface.h>


int activecpu,totalcpu;
//static int activecpu,totalcpu;
static int iloops,iperiod_1,iperiod_2;
static int cpurunning[MAX_CPU];
static int totalcycles[MAX_CPU];
static int have_to_reset;


static int lookup_shift; /* LBO 090597 */
static int lookup_entries; /* LBO 090597 */

static const struct MemoryReadAddress *memoryread;
static const struct MemoryWriteAddress *memorywrite;

/* Lookup constants for CPUs using a 16-bit address space */
#define MH_SHIFT_16		8
#define MH_ENTRIES_16	(1<<(16-MH_SHIFT_16))

/*
static int (*memoryreadhandler[MH_ENTRIES_16])(int address);
static int memoryreadoffset[MH_ENTRIES_16];
static void (*memorywritehandler[MH_ENTRIES_16])(int address,int data);
static int memorywriteoffset[MH_ENTRIES_16];
*/

static int (*memoryreadhandler[4][65536])(int address);
static int memoryreadoffset[4][MH_ENTRIES_16];
static void (*memorywritehandler[4][65536])(int address,int data);
static int memorywriteoffset[4][MH_ENTRIES_16];

/* TODO: this should be static, but currently the Qix driver needs it */
static unsigned char cpucontext[MAX_CPU][100];	/* enough to accomodate the cpu status */
static unsigned char *ramptr[MAX_CPU],*romptr[MAX_CPU];


int yield_cpu;
int saved_icount;

struct m6809context
{
	m6809_Regs	regs;
	int	icount;
	int iperiod;
	int	irq;
};



/***************************************************************************

  Memory handling

***************************************************************************/
int mrh_error(int address)
{
//	if (errorlog) fprintf(errorlog,"CPU #%d PC %04x: warning - read unmapped memory address %04x\n",activecpu,cpu_getpc(),address);
//	log_it ("CPU #%d PC %04x: warning - read unmapped memory address %04x\n",activecpu,cpu_getpc(),address);
	return RAM[address];
}
int mrh_ram(int address)
{
//		log_it("RAM %i: %i,",address,RAM[address]);
	return RAM[address];
}
int mrh_nop(int address)
{
	return 0;
}
struct MemoryReadAddress *mra;
int mrh_readmem(int address)
{
    mra = memoryread;
    while (mra->start != -1)
    {
        if (address >= mra->start && address <= mra->end)
        {
            return mra->handler(address - mra->start);
        }
        mra++;
    }

    log_it("CPU #%d PC %04x: warning - read unmapped memory address %04x\n",activecpu,cpu_getpc(),address);
    return RAM[address];
}


void mwh_error(int address,int data)
{
	if (errorlog) fprintf(errorlog,"CPU #%d PC %04x: warning - write %02x to unmapped memory address %04x\n",activecpu,cpu_getpc(),data,address);
	log_it ("CPU #%d PC %04x: warning - write %02x to unmapped memory address %04x\n",activecpu,cpu_getpc(),data,address);
	RAM[address] = data;
}
void mwh_ramrom(int address,int data)
{
	RAM[address] = ROM[address] = data;
}
void mwh_rom(int address,int data)
{
	ROM[address] = data;
}
void mwh_ram(int address,int data)
{
	ROM[address] = data;
}
void mwh_nop(int address,int data)
{
}

struct MemoryWriteAddress *mwa;
void mwh_writemem(int address,int data)
{
    mwa = memorywrite;
    while (mwa->start != -1)
    {
        if (address >= mwa->start && address <= mwa->end)
        {
            mwa->handler(address - mwa->start,data);
            return;
        }
        mwa++;
    }
    log_it("CPU #%d PC %04x: warning - write %02x to unmapped memory address %04x\n",activecpu,cpu_getpc(),data,address);
}


static void _2initmemoryhandlers(int acpu)
{
    int i,s,e;
    const struct MemoryReadAddress *mra;
    const struct MemoryWriteAddress *mwa;


    memoryread = Machine->drv->cpu[acpu].memory_read;
    memorywrite = Machine->drv->cpu[acpu].memory_write;

    for (i = 0;i < 65536;i++)
    {
        memoryreadhandler[acpu][i] = mrh_error;
        memorywritehandler[acpu][i] = mwh_error;
    }

    mra = memoryread;
    while (mra->start != -1) mra++;
    mra--;

    /* go backwards because entries up in the memory array have greater priority than */
    /* the following ones. If an entry is duplicated, going backwards we overwrite */
    /* the handler set by the lower priority one. */
    while (mra >= memoryread)
    {
        s = mra->start;
        e = mra->end;

        /* first of all make all the entries point to the general purpose handler... */
        for (i = s;i <= e;i++)
        {
			int (*handler)() = mra->handler;
			if (handler == MRA_NOP) 
				memoryreadhandler[acpu][i] = mrh_nop;
			else if (handler == MRA_RAM || handler == MRA_ROM) 
				memoryreadhandler[acpu][i] = mrh_ram; // just read the array
			else
				memoryreadhandler[acpu][i] = mrh_readmem;//mra->handler;

// untill we know how to handle the offset better!!!			(*handler)(address - mra->start);
		}
        mra--;
    }


    mwa = memorywrite;
    while (mwa->start != -1) mwa++;
    mwa--;

    /* go backwards because entries up in the memory array have greater priority than */
    /* the following ones. If an entry is duplicated, going backwards we overwrite */
    /* the handler set by the lower priority one. */
    while (mwa >= memorywrite)
    {
        void (*handler)() = mwa->handler;
        s = mwa->start;
        e = mwa->end;

        for (i = s;i <= e;i++)
        {
        
			if (handler == MWA_NOP)
			{
				memorywritehandler[acpu][i] = mwh_nop;
			}
			else if (handler == MWA_RAM)
			{
				memorywritehandler[acpu][i] = mwh_ram;  /* special case handled by cpu_writemem() */
			}
			else if (handler == MWA_RAMROM)
			{
				memorywritehandler[acpu][i] = mwh_ramrom;
			}
			else if (handler != MWA_ROM)
			{
				memorywritehandler[acpu][i] = mwh_writemem;//mwa->handler;
// untill we know how to handle the offset better!!!			(*handler)(address - mwa->start,data);
	        }
        }

        mwa--;
    }
//log_it("Mem Handler init completed");
}


static void _initmemoryhandlers(int acpu)
{
    int i,s,e,a,b;
    const struct MemoryReadAddress *mra;
    const struct MemoryWriteAddress *mwa;


    memoryread = Machine->drv->cpu[acpu].memory_read;
    memorywrite = Machine->drv->cpu[acpu].memory_write;


    lookup_entries = MH_ENTRIES_16;
    lookup_shift = MH_SHIFT_16;
    

    for (i = 0;i < lookup_entries;i++)
    {
        memoryreadhandler[acpu][i] = mrh_error;
        memoryreadoffset[acpu][i] = 0;

        memorywritehandler[acpu][i] = mwh_error;
        memorywriteoffset[acpu][i] = 0;
    }

    mra = memoryread;
    while (mra->start != -1) mra++;
    mra--;

    /* go backwards because entries up in the memory array have greater priority than */
    /* the following ones. If an entry is duplicated, going backwards we overwrite */
    /* the handler set by the lower priority one. */
    while (mra >= memoryread)
    {
        s = mra->start >> lookup_shift;
        a = mra->start ? ((mra->start-1) >> lookup_shift) + 1 : 0;
        b = ((mra->end+1) >> lookup_shift) - 1;
        e = mra->end >> lookup_shift;

        /* first of all make all the entries point to the general purpose handler... */
        for (i = s;i <= e;i++)
        {
            memoryreadhandler[acpu][i] = mrh_readmem;
            memoryreadoffset[acpu][i] = 0;
        }
        /* ... and now make the ones containing only one handler point directly to the handler */
        for (i = a;i <= b;i++)
        {
            int (*handler)() = mra->handler;


            if (handler == MRA_NOP)
            {
                memoryreadhandler[acpu][i] = mrh_nop;
                memoryreadoffset[acpu][i] = 0;
            }
            else if (handler == MRA_RAM || handler == MRA_ROM)
            {
                memoryreadhandler[acpu][i] = 0;   /* special case handled by cpu_readmem() */
                memoryreadoffset[acpu][i] = 0;
            }
            else
            {
                memoryreadhandler[acpu][i] = mra->handler;
                memoryreadoffset[acpu][i] = mra->start;
            }
        }

        mra--;
    }


    mwa = memorywrite;
    while (mwa->start != -1) mwa++;
    mwa--;

    /* go backwards because entries up in the memory array have greater priority than */
    /* the following ones. If an entry is duplicated, going backwards we overwrite */
    /* the handler set by the lower priority one. */
    while (mwa >= memorywrite)
    {
        s = mwa->start >> lookup_shift;
        a = mwa->start ? ((mwa->start-1) >> lookup_shift) + 1 : 0;
        b = ((mwa->end+1) >> lookup_shift) - 1;
        e = mwa->end >> lookup_shift;

        /* first of all make all the entries point to the general purpose handler... */
        for (i = s;i <= e;i++)
        {
            memorywritehandler[acpu][i] = mwh_writemem;
            memorywriteoffset[acpu][i] = 0;
        }
        /* ... and now make the ones containing only one handler point directly to the handler */
        for (i = a;i <= b;i++)
        {
            void (*handler)() = mwa->handler;


            if (handler == MWA_NOP)
            {
                memorywritehandler[acpu][i] = mwh_nop;
                memorywriteoffset[acpu][i] = 0;
            }
            else if (handler == MWA_RAM)
            {
                memorywritehandler[acpu][i] = 0;  /* special case handled by cpu_writemem() */
                memorywriteoffset[acpu][i] = 0;
            }
            else if (handler == MWA_RAMROM)
            {
                memorywritehandler[acpu][i] = mwh_ramrom;
                memorywriteoffset[acpu][i] = 0;
            }
            else if (handler != MWA_ROM)
            {
                memorywritehandler[acpu][i] = mwa->handler;
                memorywriteoffset[acpu][i] = mwa->start;
            }
        }

        mwa--;
    }
//log_it("Mem Handler init completed");
}



void cpu_init(void)
{
	/* count how many CPUs we have to emulate */
	totalcpu = 0;

//	have_24bit_address_space = 0; /* LBO 090597 */

	while (totalcpu < MAX_CPU)
	{
		const struct MemoryReadAddress *mra;
		const struct MemoryWriteAddress *mwa;


		if (Machine->drv->cpu[totalcpu].cpu_type == 0) break;

		
		ramptr[totalcpu] = GI[totalcpu];//Machine->memory_region[Machine->drv->cpu[totalcpu].memory_region];

		/* opcode decryption is currently supported only for the first memory region */
		if (totalcpu == 0) romptr[totalcpu] = ROM;
		else romptr[totalcpu] = ramptr[totalcpu];

		/* initialize the memory base pointers for memory hooks */
		mra = Machine->drv->cpu[totalcpu].memory_read;
		while (mra->start != -1)
		{
			if (mra->base) *mra->base = &ramptr[totalcpu][mra->start];
			if (mra->size) *mra->size = mra->end - mra->start + 1;
			mra++;
		}
		mwa = Machine->drv->cpu[totalcpu].memory_write;
		while (mwa->start != -1)
		{
			if (mwa->base) *mwa->base = &ramptr[totalcpu][mwa->start];
			if (mwa->size) *mwa->size = mwa->end - mwa->start + 1;
			mwa++;
		}


      totalcpu++;
	}
	
	
log_it("CPU Init complete.");
}



int cpu_interrupt(void)
{
	return (*Machine->drv->cpu[activecpu].interrupt)();
}
int cpu_interrupt_1(void)
{
	return (*Machine->drv->cpu[0].interrupt)();
}
int cpu_interrupt_2(void)
{
	return (*Machine->drv->cpu[1].interrupt)();
}

void cpu_start(void)
{

	have_to_reset = 0;

	for (activecpu = 0;activecpu < totalcpu;activecpu++)
	{
		/* if sound is disabled, don't emulate the audio CPU */
		//if (play_sound == 0 && (Machine->drv->cpu[activecpu].cpu_type & CPU_AUDIO_CPU))
		//	cpurunning[activecpu] = 0;
		//else
			cpurunning[activecpu] = 1;

		   totalcycles[activecpu] = 0;
	}

	/* do this AFTER the above so init_machine() can use cpu_halt() to hold the */
	/* execution of some CPUs */
	if (Machine->drv->init_machine) (*Machine->drv->init_machine)();

	for (activecpu = 0;activecpu < totalcpu;activecpu++)
	{
		int cycles;


		cycles = Machine->drv->cpu[activecpu].cpu_clock /
				(Machine->drv->frames_per_second * Machine->drv->cpu[activecpu].interrupts_per_frame);

		RAM = ramptr[activecpu];
		ROM = romptr[activecpu];
	//	initmemoryhandlers();
_2initmemoryhandlers(activecpu);

if (activecpu==0)
{
	m6809_1_readHandler = memoryreadhandler[0];
	m6809_1_writeHandler= memorywritehandler[0];
}
else
{
	m6809_2_readHandler = memoryreadhandler[1];
	m6809_2_writeHandler= memorywritehandler[1];
}


      switch(Machine->drv->cpu[activecpu].cpu_type & ~CPU_FLAGS_MASK)
		{
					
			 case CPU_M6809:
				{
					struct m6809context *ctxt;

					ctxt = (struct m6809context *)cpucontext[activecpu];
				//	m6809_Flags |= M6809_FAST_OP |M6809_FAST_U|M6809_FAST_S;
				//	m6809_Flags |= M6809_FAST_U|M6809_FAST_S;

					if (activecpu==0)
					{
						m6809_1_IPeriod = cycles;
						m6809_1_reset();
						m6809_1_GetRegs(&ctxt->regs);
			      m6809_1_ICount = cycles;
			      iperiod_1 = m6809_1_IPeriod = cycles;
			      m6809_1_IRequest = INT_NONE;

					}
					if (activecpu==1)
					{
						m6809_2_IPeriod = cycles;
						m6809_2_reset();
						m6809_2_GetRegs(&ctxt->regs);
			      m6809_2_ICount = cycles;
			      iperiod_2 = m6809_2_IPeriod = cycles;
			      m6809_2_IRequest = INT_NONE;
					}
//					m6809_reset();
//					m6809_GetRegs(&ctxt->regs);
					ctxt->icount = cycles;
					ctxt->iperiod = cycles;
					ctxt->irq = INT_NONE;
					printf("Cpu Reset\n");
				}
				break;
			
		}
	}

}

extern uint32_t cpu1Start;
extern uint32_t cpu1End;
extern uint32_t cpu2Start;
extern uint32_t cpu2End;

void cpu_run(void)
{
  for (activecpu = 0;activecpu < totalcpu;activecpu++)
  {
    //if (have_to_reset) cpu_start();	/* machine_reset() was called, have to reset */

////printf("CPU %i\n\r", activecpu);    
    if (cpurunning[activecpu])
    {
      RAM = ramptr[activecpu];
      ROM = romptr[activecpu];
//      initmemoryhandlers();
//_initmemoryhandlers(activecpu);
    memoryread = Machine->drv->cpu[activecpu].memory_read;
    memorywrite = Machine->drv->cpu[activecpu].memory_write;


      switch(Machine->drv->cpu[activecpu].cpu_type & ~CPU_FLAGS_MASK)
      {
				      
	      case CPU_M6809:
	      {
			  if (activecpu == 0)
			  {
  CCNT0(cpu1Start);
				  for (iloops = Machine->drv->cpu[activecpu].interrupts_per_frame - 1; iloops >= 0;iloops--)
				  {
	//printf("iLoop:\n\r");    
					  m6809_1_execute();
					  totalcycles[activecpu] += iperiod_1;
				  }
  CCNT0(cpu1End);
			  }
			  else if (activecpu == 1)
			  {
  CCNT0(cpu2Start);
				  for (iloops = Machine->drv->cpu[activecpu].interrupts_per_frame - 1; iloops >= 0;iloops--)
				  {
	//printf("iLoop:\n\r");    
					  m6809_2_execute();
					  totalcycles[activecpu] += iperiod_2;
				  }
  CCNT0(cpu2End);
			  }

			  
/*			  
			  struct m6809context *ctxt;


		      ctxt = (struct m6809context *)cpucontext[activecpu];
		      m6809_SetRegs(&ctxt->regs);
		      m6809_ICount = ctxt->icount;
		      iperiod = m6809_IPeriod = ctxt->iperiod;
		      m6809_IRequest = ctxt->irq;
		      for (iloops = Machine->drv->cpu[activecpu].interrupts_per_frame - 1; iloops >= 0;iloops--)
		      {
			      m6809_execute();
			      totalcycles[activecpu] += iperiod;
		      }

		      m6809_GetRegs(&ctxt->regs);
		      ctxt->icount = m6809_ICount;
		      ctxt->iperiod = m6809_IPeriod;
		      ctxt->irq = m6809_IRequest;
*/
	      }
	      break;
	      
      }
      /* keep track of changes to RAM and ROM pointers (bank switching) */
      ramptr[activecpu] = RAM;
      romptr[activecpu] = ROM;
    }
  }

  if (yield_cpu)
  {
	  yield_cpu = FALSE;
  }
  else
  {
	  ;//usres = updatescreen();
  }
	
}



/***************************************************************************

  This function resets the machine (the reset will not take place
  immediately, it will be performed at the end of the active CPU's time
  slice)

***************************************************************************/
void machine_reset(void)
{
	

	have_to_reset = 1;
}



/***************************************************************************

  Use this function to stop and restart CPUs

***************************************************************************/
void cpu_halt(int cpunum,int running)
{
	if (cpunum >= MAX_CPU) return;

	cpurunning[cpunum] = running;
}



/***************************************************************************

  This function returns CPUNUM current status  (running or halted)

***************************************************************************/
int cpu_getstatus(int cpunum)
{
	if (cpunum >= MAX_CPU) return 0;

	return cpurunning[cpunum];
}



int cpu_getpc(void)
{
	switch(Machine->drv->cpu[activecpu].cpu_type & ~CPU_FLAGS_MASK)
	{
	
		case CPU_M6809:
			if (activecpu == 0)
				return m6809_1_GetPC();
//			if (activecpu == 1)
				return m6809_2_GetPC();
			break;
	
		default:
	if (errorlog) fprintf(errorlog,"cpu_getpc: unsupported CPU type %02x\n",Machine->drv->cpu[activecpu].cpu_type);
			return -1;
			break;
	}
}


/***************************************************************************

  This is similar to cpu_getpc(), but instead of returning the current PC,
  it returns the address of the opcode that is doing the read/write. The PC
  has already been incremented by some unknown amount by the time the actual
  read or write is being executed. This helps to figure out what opcode is
  actually doing the reading or writing, and therefore the amount of cycles
  it's taking. The Missile Command driver needs to know this.

***************************************************************************/
int cpu_getpreviouspc(void)  /* -RAY- */
{
	switch(Machine->drv->cpu[activecpu].cpu_type & ~CPU_FLAGS_MASK)
	{
		//case CPU_M6502:
			//return ((M6502 *)cpucontext[activecpu])->previousPC.W;
			//break;

		default:
	if (errorlog) fprintf(errorlog,"cpu_getpreviouspc: unsupported CPU type %02x\n",Machine->drv->cpu[activecpu].cpu_type);
			return -1;
			break;
	}
}


/***************************************************************************

  This is similar to cpu_getpc(), but instead of returning the current PC,
  it returns the address stored on the top of the stack, which usually is
  the address where execution will resume after the current subroutine.
  Note that the returned value will be wrong if the program has PUSHed
  registers on the stack.

***************************************************************************/
int cpu_getreturnpc(void)
{
	switch(Machine->drv->cpu[activecpu].cpu_type & ~CPU_FLAGS_MASK)
	{
		
		default:
	if (errorlog) fprintf(errorlog,"cpu_getreturnpc: unsupported CPU type %02x\n",Machine->drv->cpu[activecpu].cpu_type);
			return -1;
			break;
	}
}



/***************************************************************************

  Returns the number of CPU cycles since the last reset of the CPU

  IMPORTANT: this value wraps around in a relatively short time.
  For example, for a 6Mhz CPU, it will wrap around in
  2^32/6000000 = 716 seconds = 12 minutes.
  Make sure you don't do comparisons between values returned by this
  function, but only use the difference (which will be correct regardless
  of wraparound).

***************************************************************************/
int cpu_gettotalcycles(void)
{
  if (activecpu == 0)
	return totalcycles[activecpu] + iperiod_1 - cpu_geticount();
	return totalcycles[activecpu] + iperiod_2 - cpu_geticount();
}



/***************************************************************************

  Returns the number of CPU cycles before the next interrupt handler call

***************************************************************************/
int cpu_geticount(void)
{
	switch(Machine->drv->cpu[activecpu].cpu_type & ~CPU_FLAGS_MASK)
	{
		case CPU_M6809:
			if (activecpu == 0)
				return m6809_1_ICount;
//			if (activecpu == 1)
				return m6809_2_ICount;
			break;
	
		default:
	if (errorlog) fprintf(errorlog,"cpu_geticount: unsupported CPU type %02x\n",Machine->drv->cpu[activecpu].cpu_type);
			return 0;
			break;
	}
}



/***************************************************************************

  Returns the number of CPU cycles before the end of the current video frame

***************************************************************************/
int cpu_getfcount(void)
{
	switch(Machine->drv->cpu[activecpu].cpu_type & ~CPU_FLAGS_MASK)
	{
		
		case CPU_M6809:
			if (activecpu == 0)
				return m6809_1_ICount + iloops * iperiod_1;
//			if (activecpu == 1)
				return m6809_2_ICount + iloops * iperiod_2;
			break;
		
		default:
	if (errorlog) fprintf(errorlog,"cpu_geticycles: unsupported CPU type %02x\n",Machine->drv->cpu[activecpu].cpu_type);
			return -1;
			break;
	}
}



/***************************************************************************

  Returns the number of CPU cycles in one video frame

***************************************************************************/
int cpu_getfperiod(void)
{
		return Machine->drv->cpu[activecpu].cpu_clock / Machine->drv->frames_per_second;
}



void cpu_seticount(int cycles)
{
	switch(Machine->drv->cpu[activecpu].cpu_type & ~CPU_FLAGS_MASK)
	{
				
		case CPU_M6809:
			if (activecpu == 0)
				m6809_1_ICount = cycles;
			if (activecpu == 0)
				m6809_2_ICount = cycles;
			break;
		
		default:
	if (errorlog) fprintf(errorlog,"cpu_seticycles: unsupported CPU type %02x\n",Machine->drv->cpu[activecpu].cpu_type);
			break;
	}
}



/***************************************************************************

  Returns the number of times the interrupt handler will be called before
  the end of the current video frame. This is can be useful to interrupt
  handlers to synchronize their operation. If you call this from outside
  an interrupt handler, add 1 to the result, i.e. if it returns 0, it means
  that the interrupt handler will be called once.

***************************************************************************/
int cpu_getiloops(void)
{
	return iloops;
}



/***************************************************************************

  Interrupt handling

***************************************************************************/

/***************************************************************************

  Use this function to cause an interrupt immediately (don't have to wait
  until the next call to the interrupt handler)

***************************************************************************/
void cpu_cause_interrupt(int cpu,int type)
{


	switch(Machine->drv->cpu[cpu].cpu_type & ~CPU_FLAGS_MASK)
	{
		/*
		case CPU_M6809:
			if (cpu == activecpu)
				m6809_Cause_Interrupt(type);
			else
			{
				m6809_Regs regs;


				m6809_GetRegs(&regs);
				m6809_SetRegs((m6809_Regs *)cpucontext[cpu]);
				m6809_Cause_Interrupt(type);
				m6809_GetRegs((m6809_Regs *)cpucontext[cpu]);
				m6809_SetRegs(&regs);
			}
			break;
		*/
		default:
if (errorlog) fprintf(errorlog,"cpu_cause_interrupt: unsupported CPU type %02x\n",Machine->drv->cpu[activecpu].cpu_type);
			break;
	}
}



void cpu_clear_pending_interrupts(int cpu)
{
	switch(Machine->drv->cpu[activecpu].cpu_type & ~CPU_FLAGS_MASK)
	{
	
		default:
if (errorlog) fprintf(errorlog,"clear_pending_interrupts: unsupported CPU type %02x\n",Machine->drv->cpu[activecpu].cpu_type);
			break;
	}
}



/* start with interrupts enabled, so the generic routine will work even if */
/* the machine doesn't have an interrupt enable port */
static int interrupt_enable = 1;
static int interrupt_vector = 0xff;

void interrupt_enable_w(int offset,int data)
{
	interrupt_enable = data;

	/* make sure there are no queued interrupts */
	if (data == 0) cpu_clear_pending_interrupts(activecpu);
}



void interrupt_vector_w(int offset,int data)
{
	if (interrupt_vector != data)
	{
		interrupt_vector = data;

		/* make sure there are no queued interrupts */
		cpu_clear_pending_interrupts(activecpu);
	}
}



int interrupt(void)
{
	switch(Machine->drv->cpu[activecpu].cpu_type & ~CPU_FLAGS_MASK)
	{
			
		case CPU_M6809:
			if (interrupt_enable == 0) return INT_NONE;
			else return INT_IRQ;
			break;
		
		default:
if (errorlog) fprintf(errorlog,"interrupt: unsupported CPU type %02x\n",Machine->drv->cpu[activecpu].cpu_type);
			return -1;
			break;
	}
}



int nmi_interrupt(void)
{
	switch(Machine->drv->cpu[activecpu].cpu_type & ~CPU_FLAGS_MASK)
	{
			
		default:
if (errorlog) fprintf(errorlog,"nmi_interrupt: unsupported CPU type %02x\n",Machine->drv->cpu[activecpu].cpu_type);
			return -1;
			break;
	}
}



int ignore_interrupt(void)
{
	switch(Machine->drv->cpu[activecpu].cpu_type & ~CPU_FLAGS_MASK)
	{
		
		case CPU_M6809:
			return INT_NONE;
			break;
		
		default:
if (errorlog) fprintf(errorlog,"interrupt: unsupported CPU type %02x\n",Machine->drv->cpu[activecpu].cpu_type);
			return -1;
			break;
	}
}



/***************************************************************************

  Perform a memory read. This function is called by the CPU emulation.

***************************************************************************/
int cpu_readmem(int address)
{
//	int (*handler)() = memoryreadhandler[activecpu][address];
//	if (handler == 0) return RAM[address];	/* special case */
//	else return handler(address);
		return memoryreadhandler[activecpu][address](address);
}
int cpu_readmem_1(int address)
{
//	int (*handler)() = memoryreadhandler[activecpu][address];
//	if (handler == 0) return RAM[address];	/* special case */
//	else return handler(address);
		return memoryreadhandler[activecpu][address](address);
}
int cpu_readmem_2(int address)
{
//	int (*handler)() = memoryreadhandler[activecpu][address];
//	if (handler == 0) return RAM[address];	/* special case */
//	else return handler(address);
		return memoryreadhandler[activecpu][address](address);
}



/***************************************************************************

  Perform a memory write. This function is called by the CPU emulation.

***************************************************************************/
void cpu_writemem(int address,int data)
{
//	void (*handler)() = memorywritehandler[activecpu][address];
//	if (handler == 0) RAM[address] = data;	/* special case */
//	else handler(address,data);
		
		memorywritehandler[activecpu][address](address,data);
}
void cpu_writemem_1(int address,int data)
{
//	void (*handler)() = memorywritehandler[activecpu][address];
//	if (handler == 0) RAM[address] = data;	/* special case */
//	else handler(address,data);
		
		memorywritehandler[activecpu][address](address,data);
}
void cpu_writemem_2(int address,int data)
{
//	void (*handler)() = memorywritehandler[activecpu][address];
//	if (handler == 0) RAM[address] = data;	/* special case */
//	else handler(address,data);
		
		memorywritehandler[activecpu][address](address,data);
}



/***************************************************************************

  Perform an I/O port read. This function is called by the CPU emulation.

***************************************************************************/
int cpu_readport(int Port)
{
	const struct IOReadPort *iorp;


	iorp = Machine->drv->cpu[activecpu].port_read;
	if (iorp)
	{
		while (iorp->start != -1)
		{
			if (Port >= iorp->start && Port <= iorp->end)
			{
				int (*handler)() = iorp->handler;


				if (handler == IORP_NOP) return 0;
				else return (*handler)(Port - iorp->start);
			}

			iorp++;
		}
	}

	if (errorlog) fprintf(errorlog,"CPU #%d PC %04x: warning - read unmapped I/O port %02x\n",activecpu,cpu_getpc(),Port);
	return 0;
}



/***************************************************************************

  Perform an I/O port write. This function is called by the CPU emulation.

***************************************************************************/
void cpu_writeport(int Port,int Value)
{
	const struct IOWritePort *iowp;


	iowp = Machine->drv->cpu[activecpu].port_write;
	if (iowp)
	{
		while (iowp->start != -1)
		{
			if (Port >= iowp->start && Port <= iowp->end)
			{
				void (*handler)() = iowp->handler;


				if (handler == IOWP_NOP) return;
				else (*handler)(Port - iowp->start,Value);

				return;
			}

			iowp++;
		}
	}

	if (errorlog) fprintf(errorlog,"CPU #%d PC %04x: warning - write %02x to unmapped I/O port %02x\n",activecpu,cpu_getpc(),Value,Port);
}



/***************************************************************************

  Interrupt handler. This function is called at regular intervals
  (determined by IPeriod) by the CPU emulation.

***************************************************************************/



