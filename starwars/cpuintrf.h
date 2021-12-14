#ifndef CPUINTRF_H
#define CPUINTRF_H


void cpu_init(void);
void cpu_run(void);
void cpu_start(void);
/* Use this function to reset the machine */
void machine_reset(void);

/* Use this function to stop and restart CPUs */
void cpu_halt(int cpunum,int running);
/* This function returns CPUNUM current status (running or halted) */
int  cpu_getstatus(int cpunum);

int cpu_getpc(void);
int cpu_getpreviouspc(void);  /* -RAY- */
int cpu_getreturnpc(void);
/* Returns the number of CPU cycles since the last reset of the CPU */
int cpu_gettotalcycles(void);
/* Returns the number of CPU cycles before the next interrupt handler call */
int cpu_geticount(void);
/* Returns the number of CPU cycles before the end of the current video frame */
int cpu_getfcount(void);
/* Returns the number of CPU cycles in one video frame */
int cpu_getfperiod(void);
void cpu_seticount(int cycles);
int cpu_scale_by_cycles(int val);

/*
  Returns the number of times the interrupt handler will be called before
  the end of the current video frame. This is can be useful to interrupt
  handlers to synchronize their operation. If you call this from outside
  an interrupt handler, add 1 to the result, i.e. if it returns 0, it means
  that the interrupt handler will be called once.
*/
int cpu_getiloops(void);

int cpu_readmem(int address);
void cpu_writemem(int address,int data);

int cpu_readport(int Port);
void cpu_writeport(int Port,int Value);


/* cause an interrupt on a CPU */
void cpu_cause_interrupt(int cpu,int type);
void cpu_clear_pending_interrupts(int cpu);
void interrupt_enable_w(int offset,int data);
void interrupt_vector_w(int offset,int data);
int interrupt(void);
int nmi_interrupt(void);
int ignore_interrupt(void);

#endif
