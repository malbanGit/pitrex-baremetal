#undef MERGE_
#undef LABEL_
#undef MAIN_TAKEOVER



#define MERGE_(a,b)  a##b    
#define LABEL_(a,b) MERGE_(a, b)

// this macro
// "returns" from the interrupt
// before it saves a label 
// when the interrupt is active again right at the
// start the interrupt handler JUMPS to that label
// and thus "continues" to execute the
// interrupt where it was released!
// in Vectrex cycles
#define MAIN_TAKEOVER_IMPL(freeTime, n) \
  ptr = &&LABEL_(lbl_, n);              \
 /* printf("hy:jumpSet): %08x, %i\r\n", ptr, freeTime);*/  \
  return freeTime;			\
LABEL_(lbl_, n):    			\
 /* printf("hy:jumpTaken): %08x\r\n", ptr); */ \
  ptr = 0; 
    
// for "freetime" the main program may run its course
// free time in Vectrex cycles
#define MAIN_TAKEOVER(freeTime) \
  MAIN_TAKEOVER_IMPL(freeTime, __COUNTER__) 

// ensure the  
// usage of the "correct" USE_TIME
#undef USE_TIME
// a in arm cycles
#define USE_TIME(a) \
do{\
 MAIN_TAKEOVER(((a)/ONE_WAIT_UNIT)) \
} while(0)
  
  
// ensure the  
// usage of the "correct" local waiter
// this one accepts wait time in "vectrex" cycles
// and waits for as long
// as from the beginning of the local "marking" 
// plus the given wait duration
// the local mark is with another macro
// basically this is used by T1 timing and zeroing
#undef waitCounter1MarkLocal
#define waitCounter1MarkLocal(waiter) \
do {\
  static unsigned int value;\
  /* Read CCNT Register */ \
  CCNT0(value);\
  if (value < (piCounterMark+((waiter)*ONE_WAIT_UNIT) ) )\
  {\
    USE_TIME( (piCounterMark+((waiter)*ONE_WAIT_UNIT) ) -value);\
  }\
  do\
  {\
    CCNT0(value);\
  } while (value < (piCounterMark+((waiter)*ONE_WAIT_UNIT) ) );\
} while (0)


int handleVectrexOutput();

void __attribute__((interrupt("IRQ"))) v_interrupt_vector(void)
{
  // releasetime in Vectrex cycles!
/*  for debugging
  if (currentButtonState == 0x02)
  {
    struct pt_regs *p;
    u_long *lnk_ptr;

    // On IRQ exception the LR points to PC+4
    asm __volatile__ (
        "sub lr, lr, #4\n"
        "mov %0, lr" : "=r" (lnk_ptr)
    );
    asm __volatile__ ("mov %0, sp\n"  : "=r" (p) );  
    asm __volatile__ ("mov lr, pc\n"  );  
    printf( "IRQ EXCEPTION at %p 0x%08lX\r\n", lnk_ptr, *(lnk_ptr));
    
    // following does not work well
    // some bugs in there
    // but since I don't need it right now...
    // I did not debug it!
    ptrace_stackdump_regs( p );    
    for (;;);
  }
*/  
  inIRQ_MODE = 1;
  while (1)
  {
    //wait for release time - in vectrex cycles
    releaseIRQTime = handleVectrexOutput();
    
    if (releaseIRQTime > 3)
    {
      // free the IRQ
      uint32_t counter = rpiSystemTimer->counter_lo;

      // intervall must be at least 3 <= (releaseIRQTime-1)
      if (releaseIRQTime==4) releaseIRQTime = 5; // prevent interval = 1
      
      uint32_t interval = ((releaseIRQTime-1) * ONE_WAIT_UNIT) / 1000; // * 1000 = cycles
      rpiSystemTimer->control_status = 2; // acknoledge interrupt see: https://www.raspberrypi.org/forums/viewtopic.php?t=23969
                                          // BCM2837-ARM-Peripherals.-.Revised.-.V2-1-1.pdf page 172/173
      rpiSystemTimer->compare1 = counter+interval /*+ _10 _ */;
      inIRQ_MODE = 0;



      irqBreakCheckValue+=(releaseIRQTime-1) * ONE_WAIT_UNIT;



      return;
    }
    else
    {
    }
    // do a busy wait
    // or just nothing, since
    // the place we are returnning to (returning as in goto handleVectrexOutput() )
    // wait cycle exact
  }
  inIRQ_MODE = 0;
}

/* https://github.com/dwelch67/raspberrypi/blob/master/blinker07/blinker07.c
https://github.com/s-matyukevich/raspberry-pi-os/tree/master/src/lesson03/src

https://www.studica.com/blog/raspberry-pi-timer-embedded-environments

System Timer is in Micro seconds!
it is a 1Mhz timer
*/

void v_setupIRQHandling()
{
  if (isIRQMode==1) return;
  DisableInterrupts(); // function defined in out baremetal.S
  
  
#if RASPPI >= 2
  write32 (ARM_LOCAL_TIMER_INT_CONTROL0, 0);
#endif
  
  // reset everything interrupt
  rpiIRQController->FIQ_control = 0; // clear FIQ
  rpiIRQController->Disable_IRQs_1 = (uint32_t) -1; // clear all 1 interrupts
  rpiIRQController->Disable_IRQs_2 = (uint32_t) -1; // clear all 2 interrupts
  rpiIRQController->Disable_Basic_IRQs = (uint32_t) -1; // clear all Basic interrupts
  
  rpiIRQController->IRQ_basic_pending = rpiIRQController->IRQ_basic_pending; // acknoldedge all interrupts
  rpiIRQController->IRQ_pending_1 = rpiIRQController->IRQ_pending_1; // acknoldedge all interrupts
  rpiIRQController->IRQ_pending_2 = rpiIRQController->IRQ_pending_2; // acknoldedge all interrupts
  
  // get "vector table" from address 0
  pi_vectors *pv ;
  asm("ldr %[result],= 0\n\t" : [result]"=r" (pv) :: "cc");  // circumvent gcc UDF
  pv->_irq_pointer = (unsigned int)v_interrupt_vector;

  synqSignalIRQ = 0;
  ptr = 0;
  setWaitMin = 0;
  waitClientHzWait = 0;

  
  printf("IRQ is set up\r\n");  
  resetPipeline();
  uint32_t counter = rpiSystemTimer->counter_lo;
  uint32_t interval = 1000;
  rpiSystemTimer->compare1 = counter+interval;
  rpiSystemTimer->control_status = 2;
  rpiIRQController->Enable_IRQs_1 = 2; // Timer 1
  isIRQMode = 1;

  /* Enable interrupts! */
  EnableInterrupts(); // function defined in out baremetal.S
}

void v_removeIRQHandling()
{
  if (isIRQMode==0) return;
  isIRQMode = 0;
  DisableInterrupts(); // function defined in out baremetal.S
  resetPipeline();
  printf("IRQ is removed\r\n");  
  if (pendingEnableMultiCore)
  {
    v_setupSMPHandling();
  }
}
void v_irqForceBreakMikroSeconds(int c)
{
  v_forcedBreakInMicroSeconds=c;
}

uint32_t waitClientHzWait=0;

void handleClient_WR()
{
  // if an IRQ move is pending - do it at once
  // so we get rid of it...
  if (pendingDisableInterrupts)
  {
    pendingDisableInterrupts = 0;
    v_removeIRQHandling();
    resetPipeline();
    return;
  }
  if (pendingReturnToPiTrex)
  {
    pendingReturnToPiTrex = 0;
    v_removeIRQHandling();
    printf("IRQ: handleClient_WR() calling returnToPiTrex() 1\n");
    returnToPiTrex();
    return;
  }
  if (useUSB == 1)
    if (usbIRQHandler != 0)
      usbIRQHandler (usbIRQParam);
  
  // handle the pipeline
  handlePipeline();
  synqSignalIRQ = synqSignalIRQ | SYNC_CLIENT_FINISHED;
  
  
  if (isSynchronizedWithIRQ)
  {
    while (!(synqSignalIRQ & SYNC_DISPLAY_FINISHED) ) ; //printf("S");
    synqSignalIRQ = synqSignalIRQ & (~SYNC_DISPLAY_FINISHED); // reset signal

    clientRate = (1*1000*1000) / (((1*1000*1000)/clientHz) + (rpiSystemTimer->counter_lo - waitClientHzWait));
    waitClientHzWait = (rpiSystemTimer->counter_lo) + ((1*1000*1000)/clientHz);
    return;
  }
  
  while (rpiSystemTimer->counter_lo < waitClientHzWait) ;

  clientRate = (1*1000*1000) / (((1*1000*1000)/clientHz) + (rpiSystemTimer->counter_lo - waitClientHzWait));
  uint32_t counter = rpiSystemTimer->counter_lo;
  waitClientHzWait = counter + ((1*1000*1000)/clientHz);
}




/*
 The ugly thing about IRQ handling is, that due to the "non functional" nature we have to
 stay within ONE function - otherwise we would not be able
 to jump "back" to our destination after the IRQ is freed.
 So all "subroutine" calls, which should in any way "WAIT" (return the IRQ) must be
 resolved into this one function.
 This looks way ugly, but cannot be helped, unless we have different levels
 of IRQ dispatching - which would also be ugly.
 
 Also it would probably look nicer, if the old "function" could be "macrofied", this also does
 not work due to the current implemenation of the unique jump label, which takes the
 line number as an identifier. 
 Even a "multiline" macro has only one distinct __LINE__ number, so within one macro
 only one jump identifier can be created!
 
 WATCH OUT!
 All variables that must be accessed over a "IRQ return" MUST NOT be local variables (stack based) but
 must be static or global. Otherwise after a return from "normal space" the contents of the
 variable will be screwed!
*/ 

    
int handleVectrexOutput()
{
    if (ptr != 0)
    {
      goto *ptr;
    }
    // except for release to normal space
    // there is no reason to leave the hyper space
    while (1)
    {
      HANDLE_SOUND
      HANDLE_BUTTONS
      HANDLE_JOYSTICK
      
      PLAY_SAMPLE_IGNORE_A();

      if ((currentButtonState&0xf) == 0xf) // button 1+ 2 + 3+4 -> go menu
      {
        pendingReturnToPiTrex = 1;
      }

      if ((currentButtonState&0x20)== 0x20) // button 2 -> joypad 2
      {
        if (inGameSettingsAllowed)
        {
          if (wasIRQMode == 255)
          {
            inCalibration = 1;
            wasIRQMode = isIRQMode;
            pendingDisableInterrupts = 1;
          }
        }
      }

      //******************************************************************
      // Round is done
      //printf("hy:set up synchronization\r\n");  
      synqSignalIRQ = synqSignalIRQ | SYNC_DISPLAY_FINISHED; // only needed when in sync mode
      //******************************************************************
      if (v_sampleEnabled)
      {
        while ((GET(VIA_int_flags) & 0x20) == 0)
        {
          // the default entry is a WaitRecal
          int lo = GET(VIA_t2_cnt_lo); // reading lo, resets the T2 interrupt flag!
          int hi = GET(VIA_t2_cnt_hi);
                    if ((hi & 0xff) == 0xff) 
                            goto timeOut;
          int t2 = hi*256+lo;
    
          if (t2 < (((Vec_Rfrsh&0xff)*256) + ((Vec_Rfrsh>>8)&0xff)))
          {
            if (t2 > v_sampleInterval) t2 = v_sampleInterval;
            // must wait for timer to expire
            // if not here, the timer expired bedore we got here -> no wait!
            MAIN_TAKEOVER(t2);
            PLAY_SAMPLE_IGNORE_A();
          }
        }
        if (isSynchronizedWithIRQ)
        {
          while (!(synqSignalIRQ & SYNC_CLIENT_FINISHED) ) 
          {
          // give client time to finish its round
            MAIN_TAKEOVER(v_sampleInterval); // hundred vectrex cycles
            PLAY_SAMPLE_IGNORE_A();
          }
          synqSignalIRQ = synqSignalIRQ & (~SYNC_CLIENT_FINISHED); // reset signal
        }
        ioDone = 0;
        SWITCH_BEAM_OFF();
        CCNT0(roundCycles);
    
        // wait for Via T2 to expire -> 50Hz
        while ((GET(VIA_int_flags) & 0x20) == 0) 
        {
          PLAY_SAMPLE_IGNORE_A();
        }
      }
      else
      {
        if ((GET(VIA_int_flags) & 0x20) == 0)
        {
          // the default entry is a WaitRecal
          int lo = GET(VIA_t2_cnt_lo); // reading lo, resets the T2 interrupt flag!
          int hi = GET(VIA_t2_cnt_hi);
                    if ((hi & 0xff) == 0xff) 
                        goto timeOut;
          int t2 = hi*256+lo;
          if (t2 < (((Vec_Rfrsh&0xff)*256) + ((Vec_Rfrsh>>8)&0xff)))
          {
            // must wait for timer to expire
            // if not here, the timer expired bedore we got here -> no wait!
            MAIN_TAKEOVER(t2);
          }
        }
        if (isSynchronizedWithIRQ)
        {
          while (!(synqSignalIRQ & SYNC_CLIENT_FINISHED) ) 
          {
          // give client time to finish its round
            MAIN_TAKEOVER(100); // hundred vectrex cycles
          }
          synqSignalIRQ = synqSignalIRQ & (~SYNC_CLIENT_FINISHED); // reset signal
        }
        ioDone = 0;
        SWITCH_BEAM_OFF();
        CCNT0(roundCycles);
    
        // wait for Via T2 to expire -> 50Hz
        while ((GET(VIA_int_flags) & 0x20) == 0) ;
      }


timeOut:    
      //******************************************************************
      
      // reset T2 VIA timer to 50Hz
      SETW (VIA_t2, Vec_Rfrsh);

      // at the start of each "round" 
      // reset pi counter 1
      PMNC(CYCLE_COUNTER_ENABLE|COUNTER_ZERO);
#ifdef PITREX_DEBUG
      if (serialCommandsAllowed) handleUARTInterface();
      PLAY_SAMPLE_IGNORE_A();
#endif      
      
      DEFLOCK
      IRESET

      consecutiveDraws = 0; 
#define __IRQ_PIPELINE__      
      #include "pipeline.i"
#undef __IRQ_PIPELINE__      
    }
}
