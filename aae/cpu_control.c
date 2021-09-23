#include <stdlib.h>
#include <string.h>

#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

#include "log.h"
#include "m6502/m6502.h"
#include "z80/z80.h"
#include "globals.h"
#include "musashi/m68kcpu.h"

#include "cpu_control.h"

extern int get_ccpu_ticks();

static int cpu_configured=0;
static int num_cpus=0;
int running_cpu=0;
static int cyclecount[4];
static int addcycles[4];
static int reset_cpu_status[4];
////Time counters
static int hrzcounter=0; //Only on CPU 0
static int hertzflip=0;
static int tickcount[4];
static int eternaticks[4];
static int vid_tickcount;
////Interrupt Variables
static int enable_interrupt[4];
static int interrupt_vec[4] = {0xff,0xff,0xff,0xff};
static int interrupt_count[4];
static int interrupt_pending[4];
static int framecnt=0;
static int intcnt=0;
//CONTEXTM6502 cont6502[4];

//int tracker=1;
void init6502Z(struct MemoryReadByte *read, struct MemoryWriteByte *write, int cpunum)
{	
  log_it("Configuring CPU");
  m6502zpinit();
  c6502[cpunum] = malloc(m6502zpGetContextSize());
  memset( c6502[cpunum], 0, m6502zpGetContextSize());
  
  c6502[cpunum]->m6502Base = GI[cpunum];
  c6502[cpunum]->m6502MemoryRead = read;
  c6502[cpunum]->m6502MemoryWrite = write;
  m6502zpSetContext(c6502[cpunum]);
  m6502zpreset();
  
  m6502zpGetContext(c6502[cpunum]);
  log_it("Finished Configuring CPU");
}
void init6502(struct MemoryReadByte *read, struct MemoryWriteByte *write, int cpunum)
{   
    log_it("Configuring CPU");
  m6502init();
  c6502[cpunum] = malloc(m6502GetContextSize());
  memset( c6502[cpunum], 0, m6502GetContextSize());
  
  c6502[cpunum]->m6502Base = GI[cpunum];
  c6502[cpunum]->m6502MemoryRead = read;
  c6502[cpunum]->m6502MemoryWrite = write;
  m6502SetContext(c6502[cpunum]);
  m6502reset();
  
  m6502GetContext(c6502[cpunum]);
  log_it("Finished Configuring CPU");
 

}

void initz80N(struct MemoryReadByte *read, struct MemoryWriteByte *write, struct z80PortRead *portread, struct z80PortWrite *portwrite, int cpunum)
{
    memset(&cMZ80[cpunum], 0, sizeof(struct mz80context));
    cMZ80[cpunum].z80Base=GI[cpunum];
    cMZ80[cpunum].z80IoRead=portread;
    cMZ80[cpunum].z80IoWrite=portwrite;
    cMZ80[cpunum].z80MemRead=read;
    cMZ80[cpunum].z80MemWrite=write;
    cMZ80[cpunum].z80intAddr=0x38;
    cMZ80[cpunum].z80nmiAddr=0x66;
    mz80SetContext(&cMZ80[cpunum]);
    mz80init();

    mz80reset();
    mz80GetContext(&cMZ80[cpunum]);
    
    //CONTEXTMZ80 *temp;

    //cMZ80[cpunum] = malloc(mz80GetContextSize());
    
    //temp = cMZ80[cpunum];
    //memset( &temp, 0, mz80GetContextSize());
    
   // cMZ80[cpunum]->z80Base=GI[cpunum];
    //cMZ80[cpunum]->z80IoRead=portread;
    //cMZ80[cpunum]->z80IoWrite=portwrite;
    //cMZ80[cpunum]->z80MemRead=read;
    //cMZ80[cpunum]->z80MemWrite=write;
    //mz80SetContext(cMZ80[cpunum]);
    //mz80reset();
    //mz80GetContext(cMZ80[cpunum]);
    
}

void initz80(struct MemoryReadByte *read, struct MemoryWriteByte *write, struct z80PortRead *portread, struct z80PortWrite *portwrite, int cpunum)
{
    /*
    memset(&z80, 0, sizeof(struct mz80context));
    z80.z80Base=GI[cpunum];
    z80.z80IoRead=portread;
    z80.z80IoWrite=portwrite;
    z80.z80MemRead=read;
    z80.z80MemWrite=write;
    mz80SetContext(&z80);
    mz80reset();
    mz80GetContext(&z80);
    z80.z80intAddr=0x38;
    z80.z80nmiAddr=0x66;
    mz80SetContext(&z80);
    mz80reset();
    mz80GetContext(&z80);
    */
}


void init68k()
{
    m68k_init();
    m68k_set_cpu_type(M68K_CPU_TYPE_68000);
    m68k_pulse_reset();
}

/*
void initz80(struct MemoryReadByte *read, struct MemoryWriteByte *write, struct z80PortRead *portread, struct z80PortWrite *portwrite, int cpunum)
{
}

void init8080(struct MemoryReadByte *read, struct MemoryWriteByte *write, struct z80PortRead *portread, struct z80PortWrite *portwrite, int cpunum)
{
}


void init68k(struct STARSCREAM_PROGRAMREGION *fetch, struct STARSCREAM_DATAREGION *readbyte, struct STARSCREAM_DATAREGION *readword, struct STARSCREAM_DATAREGION *writebyte, struct STARSCREAM_DATAREGION *writeword)
{
      s68000init();
      s68000context.fetch=fetch;
	  s68000context.s_fetch=fetch;
      s68000context.u_fetch=fetch;
      s68000context.s_readbyte=readbyte;
      s68000context.u_readbyte=readbyte;
      s68000context.s_readword=readword;
      s68000context.u_readword=readword;
      s68000context.s_writebyte=writebyte;
      s68000context.u_writebyte=writebyte;
      s68000context.s_writeword=writeword;
      s68000context.u_writeword=writeword;
	  s68000SetContext(&s68000context);
      s68000reset();
	  s68000exec(100);
      s68000reset();
	  log_it("68000 Initialized: Initial PC is %06X\n",s68000context.pc);
}
*/

int return_tickcount( int reset)
{
  int val=0;

  switch  (driver[gamenum].cpu_type[running_cpu])
  {
                  case CPU_MZ80:
					   		
					        break;

    case CPU_6502Z:
      if (reset) val = m6502zpGetElapsedTicks(0xff);
      else val = m6502zpGetElapsedTicks(0);
      break;
  }
  return val;
}


void add_hertz_ticks(int cpunum, int ticks)
{
  if (cpunum > 0) return;
  hrzcounter+=ticks;
  //log_it("HRTZ COunter %d",hrzcounter);
  if (hrzcounter > 100) {hrzcounter-=100;hertzflip^=1;}
}

int get_hertz_counter()
{
  return hertzflip;
}

void add_eterna_ticks(int cpunum, int ticks)
{
  eternaticks[cpunum]+=ticks;
  if (eternaticks[cpunum] > 0xffffff) 
  {
    eternaticks[cpunum]=0;
  }
}

int get_eterna_ticks(int cpunum)
{
  return eternaticks[cpunum];
}

int get_elapsed_ticks(int cpunum)
{
  return tickcount[cpunum];
}

int get_video_ticks(int reset)
{
	
  int v=0;
  int temp=0;
//  static int startnumber=0;

  if (reset == 0xff) //Reset Tickcount;
  {
   vid_tickcount=0;
    switch  (driver[gamenum].cpu_type[0])
    {
      case CPU_MZ80:  vid_tickcount-=mz80GetElapsedTicks(1);break;  //Make vid_tickcount a negative number to check for reset later;
      case CPU_6502Z: vid_tickcount-=m6502zpGetElapsedTicks(1);break;
      case CPU_6502:  vid_tickcount-=m6502GetElapsedTicks(1);break;
      case CPU_68000: vid_tickcount-=m68k_cycles_run(); m68k_end_timeslice();break;
    }
    return 0;
  }
  
  v = vid_tickcount;
  switch  (driver[gamenum].cpu_type[0])
  {
    case CPU_MZ80:  temp=mz80GetElapsedTicks(0);break;  //Make vid_tickcount a negative number to check for reset later;
    case CPU_6502Z: temp=m6502zpGetElapsedTicks(0);break;
    case CPU_6502:  temp=m6502GetElapsedTicks(0);break;
    case CPU_68000: temp=m68k_cycles_run();break;
  }
  
  //if (temp <= cyclecount[running_cpu]) temp = cyclecount[running_cpu]-m6502zpGetElapsedTicks(0);
  //else log_it("Video CYCLE Count ERROR occured, check code.");
  v+=temp;
  return v;
}

void init_cpu_config()
{
  int x;
  
  num_cpus=0;
  cpu_configured=0;
  running_cpu=0;

  //log_it("Starting up cpu settings, defaults");

  for(x=0; x<4 ; x++) { if (driver[gamenum].cpu_type[x])  num_cpus++;}
  for(x=0; x < num_cpus; x++) 
  {
    cyclecount[x] = ((int) driver[gamenum].cpu_freq[x] / (int) driver[gamenum].fps) / (int) driver[gamenum].cpu_divisions[x];  
  }
  for(x=0; x<4 ; x++) {addcycles[x]=0;interrupt_count[x]=0; interrupt_pending[x]=0;enable_interrupt[x] = 1;}
    
	
  interrupt_vec[x] = 0xff;
  vid_tickcount=0;//Initalize video tickcount;
  hertzflip=0;
  log_it("NUMBER OF CPU'S to RUN %d",num_cpus);

  //log_it("Finished starting up cpu settings, defaults");
}


int get_current_cpu()
{
 return running_cpu;
}

void cpu_disable_interrupts(int cpunum,int val)
{
  enable_interrupt[cpunum] = val;
}

int cpu_getcycles(int reset) //Only returns cycles from current context cpu
{
  int ticks=0;
  switch  (driver[gamenum].cpu_type[running_cpu])
  {
    case CPU_MZ80:  ticks=mz80GetElapsedTicks(reset);break;
    case CPU_6502Z: ticks=m6502zpGetElapsedTicks(reset);break;
    case CPU_6502:  ticks=m6502GetElapsedTicks(reset);break;
              case CPU_68000: ticks=m68k_cycles_run();
                  if (reset) m68k_end_timeslice();
                  break;
  }
  return ticks;
}

void cpu_setcontext(int cpunum)
{
  switch  (driver[gamenum].cpu_type[cpunum])
  {
    case CPU_MZ80:  mz80SetContext(&cMZ80[running_cpu]); break;
    case CPU_6502Z: m6502zpSetContext(c6502[cpunum]); break;
    case CPU_6502:  m6502SetContext(c6502[cpunum]); break;
//			  case CPU_68000: s68000SetContext(&c68k[running_cpu]);break;

  }
}

void cpu_getcontext(int cpunum)
{
  switch  (driver[gamenum].cpu_type[cpunum])
  {
    case CPU_MZ80:  mz80GetContext(&cMZ80[running_cpu]); break;
    case CPU_6502Z: m6502zpGetContext(c6502[cpunum]); break;
    case CPU_6502:  m6502GetContext(c6502[cpunum]); break;
//			  case CPU_68000: s68000GetContext(&c68k[running_cpu]);break;
  }
}


void cpu_do_interrupt(int int_type, int cpunum)
{
  if (enable_interrupt[cpunum] == 0) {log_it("Interrupts Disabled");return;}
   
  interrupt_count[cpunum]++;
  //log_it("Interrupt count %d", interrupt_count[cpunum]);
  if (interrupt_count[cpunum] == driver[gamenum].cpu_intpass_per_frame[running_cpu]) 
  {
    intcnt++;
    // log_it("Interrupt count %d", interrupt_count[cpunum]);
    switch  (driver[gamenum].cpu_type[running_cpu])
    {
/*	
      case CPU_68000: 
	s68000interrupt(driver[gamenum].cpu_int_type[running_cpu],-1);
	s68000flushInterrupts();
	break;
*/
      case CPU_MZ80:
        if (int_type==INT_TYPE_NMI)  
        {
          mz80nmi();
    //	  log_it("NMI Taken"); 
        }
        else {
          mz80int(1); 
    //	  log_it("INT Taken");
        }   
        break;
      case CPU_6502Z:
        if (int_type==INT_TYPE_NMI)  
        {
          m6502zpnmi();
          //log_it("NMI Taken"); 
        }
        else {
          m6502zpint(1); 
          //log_it("INT Taken");
        }   
        break;
      case CPU_6502:
        if (int_type==INT_TYPE_NMI)  
        {
          m6502nmi();
          //log_it("NMI Taken"); 
        }
        else 
        {
          m6502int(1); 
          //log_it("INT Taken");
        }   
        break;
        case CPU_68000: 
          m68k_set_irq(driver[gamenum].cpu_int_type[running_cpu]);
//                   s68000interrupt(driver[gamenum].cpu_int_type[running_cpu],-1);
//                                s68000flushInterrupts();
            
          break;
    }
    interrupt_count[cpunum]=0;
  } 
//  check type of cpu (cpu_current)
//  if > 0 switch context, generate int of type
//   mz80GetContext(&context[CurrentCPU]);
//   mz80SetContext(&context[cpu]);
//   mz80nmi();
//   mz80GetContext(&context[cpu]);
//   mz80SetContext(&context[CurrentCPU]);
}

void exec_cpu()
{
  UINT32 dwResult = 0;
  switch  (driver[gamenum].cpu_type[running_cpu])
  {
    case CPU_MZ80:   dwResult = mz80exec(cyclecount[running_cpu]);break;
    case CPU_6502Z:  dwResult = m6502zpexec(cyclecount[running_cpu]);break;
    case CPU_6502:   dwResult = m6502exec(cyclecount[running_cpu]);break;
    case CPU_68000:  dwResult = m68k_execute(cyclecount[running_cpu]);break;
  }
//  if (0x80000000 != dwResult)
//  {	
//    printf("Invalid instruction at <I don't know>\n");
//  }
}
void run_cpus_to_cycles()
{
//  UINT32 dwElapsedTicks = 0;
//  UINT32 dwResult = 0;
  int  x; 
  int cycles_ran=0;

//  int adj=0;
  
  tickcount[0]=0;
  tickcount[1]=0;
  tickcount[2]=0;
  tickcount[3]=0;

  
  //log_it("Starting cpu run %d",cyclecount[running_cpu]);
  //log_it("Starting cpu run %d",cyclecount[running_cpu+1]);


//log_it("CPU(%i) - cycles to Run: %i * %i\n", running_cpu,driver[gamenum].cpu_divisions[0], cyclecount[0]);
  for (x=0; x < driver[gamenum].cpu_divisions[0]; x++)
  {
    for (running_cpu=0; running_cpu < num_cpus; running_cpu++)
    {
      if (num_cpus > 1) 
      {
        cpu_setcontext(running_cpu);
      }
      /*dwElapsedTicks=*/cpu_getcycles(0xff);

      cpu_resetter(running_cpu); //Check for CPU Reset
      process_pending_interrupts(running_cpu); //Check and see if there is a pending interrupt request outstanding

      exec_cpu();

      cycles_ran = cpu_getcycles(0);
      tickcount[running_cpu]+=cycles_ran; //Add cycles this pass to frame cycle counter;
      add_eterna_ticks(running_cpu,cycles_ran);

      if (running_cpu==0)
      {
        if ( vid_tickcount < 1) 
          vid_tickcount = cycles_ran - vid_tickcount; //Play catchup after reset
        vid_tickcount+=cycles_ran;
        add_hertz_ticks(0, cycles_ran);
      }

      if ( driver[gamenum].int_cpu[running_cpu])  
      {
        driver[gamenum].int_cpu[running_cpu]();
      } 
      else 
      { 
        if ( driver[gamenum].cpu_int_type[running_cpu] )//Is there an int to run?
        {
          // log_it("WARNING --- CALLING STANDARD INTERRUPT HANDLER!!!!");
          cpu_do_interrupt( driver[gamenum].cpu_int_type[running_cpu] ,running_cpu);
        }
      }
      if (num_cpus > 1) 
      {
        cpu_getcontext(running_cpu);
      }
    }
//log_it("Starting cpu run %d",cyclecount[running_cpu+1]);
	
  }

  framecnt++;
  if (framecnt == driver[gamenum].fps)
  {
//log_it("-------------------------------------------------INTERRUPTS PER FRAME %d",intcnt);
  framecnt=0;intcnt=0;
  }
	
// log_it("CPU CYCLES RAN %d CPU CYCLES REQUESTED %d", tickcount[0],driver[gamenum].cpu_freq[0]/driver[gamenum].fps);	
}


void cpu_resetter(int cpunum)
{
  if (reset_cpu_status[cpunum])
  {
    log_it("RESETTING CPU 0 NOW --------------------------------------");
    switch  (driver[gamenum].cpu_type[running_cpu])
    {
      case CPU_MZ80:  mz80reset();break;
      case CPU_6502Z: m6502zpreset(); break;
      case CPU_6502:  m6502reset(); break;
      case CPU_68000: m68k_pulse_reset();break;
    }
	
	//m6502zpreset();
    tickcount[cpunum]=0;
    interrupt_count[cpunum]=0;
    reset_cpu_status[cpunum]=0;
    if (cpunum==0)vid_tickcount=0;
  }
}



void cpu_needs_reset(int cpunum)
{
   log_it("CPU RESET CALLED!!!!!!!!!!!!!!!!!!!!!!___________________________________!!!!!!!!!!!!!!!");
    reset_cpu_status[cpunum]=1;
}

void cpu_clear_pending_int(int int_type, int cpunum)
{
  switch  (driver[gamenum].cpu_type[running_cpu])
  {
    case CPU_MZ80:  mz80reset();break;
    case CPU_6502Z: m6502zpreset(); break;
    case CPU_6502:  m6502reset(); break;
  }
}


void process_pending_interrupts(int cpunum)
{
  if (interrupt_pending[cpunum])
  {     
    switch  (driver[gamenum].cpu_type[cpunum])
    {
      case CPU_MZ80:
        if (interrupt_pending[cpunum]==INT_TYPE_NMI)  
        {
          mz80nmi();
    //	  log_it("NMI Taken"); 
        }
        else 
        {
          mz80int(1); 
    //	  log_it("INT Taken");
        }   
        break;
/*      
      case CPU_68000:
	if (interrupt_pending[cpunum])  
	{
	  m6502nmi();!!!
	  log_it("NMI Taken"); 
	}
	break;	
*/	
      case CPU_6502Z:
        if (interrupt_pending[cpunum]==INT_TYPE_NMI) 
        { 
          m6502zpnmi();
          // log_it("Delayed NMI SET CPU #%d",cpunum); 
        }
        else 
        {
          m6502zpint(1);
          //log_it("Delayed INT SET CPU #%d",cpunum);
        }
        break;
		      
      case CPU_6502:
        if (interrupt_pending[cpunum]==INT_TYPE_NMI)  
        {
          m6502nmi();
          log_it("NMI Taken"); 
        }
        else 
        {
          m6502int(1); 
          log_it("INT Taken");
          }   
        break;	
      case CPU_68000:
        if (interrupt_pending[cpunum])  
              {
                  m68k_set_irq(7);
//                                m6502nmi();
              log_it("NMI Taken"); 
              }
        break;  
    }
    interrupt_pending[cpunum]=0;
  }
}

void set_pending_interrupt(int int_type, int cpunum) //Interrrupt to execute on next cpu cycle
{
  interrupt_pending[cpunum] = int_type;
}


//TO DO - ADD CPU SPEED TO MAIN DRIVER and below. Also Multiple CPUs
int cpu_scale_by_cycles(int val)
{
  float temp;
  int k;
  int current=0;
  int max;
  int clock = driver[gamenum].cpu_freq[0];

  switch  (driver[gamenum].cpu_type[running_cpu])
  {
    case CPU_MZ80:  current=mz80GetElapsedTicks(0);
      current+=tickcount[running_cpu];
      break;
    case CPU_68000: current=m68k_cycles_run();
      current+=tickcount[running_cpu];
        break;
    case CPU_6502Z: current=m6502zpGetElapsedTicks(0);
      current+=tickcount[running_cpu];
      break;
    case CPU_6502: current=m6502GetElapsedTicks(0);
      current+=tickcount[running_cpu];
      break;
    //case CPU_CCPU: current = get_ccpu_ticks();break;
  }

  max= clock / driver[gamenum].fps;
  // log_it(" Clock  %d divided by FPS: %d is equal to value: %d",clock,driver[gamenum].fps,max);
  //k = val * (float)((float)current / (float) max); //BUFFER_SIZE  * 


  temp=(  (float) current / (float) max);
  if (temp > 1) temp=.95;
  
  //log_it(" Current %d divided by MAX: %d is equal to value: %f",current,max,temp);
  
  temp= val * temp;
  k=temp;

  if (driver[gamenum].cpu_type[0] == CPU_CCPU) return val;
  return k;
}
