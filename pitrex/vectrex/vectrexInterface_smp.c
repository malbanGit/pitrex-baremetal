// not done port swap!


// todo enable game configuarion while in multicore mode

// todo time of VIA is only used in IRQ pipeline, no in e.g.  sound function or prelude!
// since as of now the USE_TIME is empty "normally"
// and get redefined just within "pipeline.i"

// todo 
// normal must ensure not to have a USE_TIME active



#if RASPPI != 1 
extern int _is_smp;
int enableCoreCheck = 0;;
unsigned int leave=0;
unsigned int enter=0;
unsigned int all=0;
void smpPipelineStart();

void v_removeMultiCore()
{
  if (isSMPMode==0) return;
  if (thisCore() != 1)
  {
    pendingDisableMultiCore=1;
    while (isSMPMode==1);
    return;
  }
  // capture cores and put them in loader wait state
  pendingDisableMultiCore = 0;
  pendingEnableMultiCore = 0;
  resetPipeline();
  isSMPMode = 0;
  _is_smp = 0;
  parkCore1();
}

void v_setupSMPHandling()
{
  if (isSMPMode == 1) return;
  if (isIRQMode == 1)
  {
    printf("start removing IRQ\n");
    pendingDisableInterrupts = 1;
    pendingEnableMultiCore = 1;
    return;
  }
  pendingEnableMultiCore = 0;
  printf("About to start Core 1\n");
  releaseLock(); // for safety
  startCore1((unsigned int) (&smpPipelineStart));
  PMNC(CYCLE_COUNTER_ENABLE|COUNTER_ZERO|CYCLE_COUNTER_RESET);
}

void smpPipelineStart()
{
  releaseLock(); // for safety
  _is_smp = 1;
  isSMPMode = 1;
  while (1)
  {
    if (pendingDisableMultiCore)
    {
      v_removeMultiCore();
    }
    handleVectrexOutputSMP();
  }
}


// called from "user" (emulator) core (core 0)
// this ONLY handles the client frame rate in multicore
// also the pipeline generation
// on multi core to park - this might be the sync point
void handleClient_SMP_WR()
{
  if (pendingDisableMultiCore)
  {
    // wait for core to actually switch off
    while (isSMPMode);
    printf("SMP switched off\n");
    if (pendingReturnToPiTrex)
    {
      printf("SMP: handleClient_SMP_WR() calling returnToPiTrex() 1\n");
      returnToPiTrex();
    }
    if (pendingEnableInterrupts)
    {
      pendingEnableInterrupts = 0;
      v_setupIRQHandling();
      printf("IRQ switched on\n");
    }
    return;
  }
  #ifdef PITREX_DEBUG
  if (serialCommandsAllowed) handleUARTInterface();
  #endif      

  if (pendingReturnToPiTrex)
  {
    pendingDisableMultiCore = 1;
    while (isSMPMode) ;

    printf("SMP: handleClient_SMP_WR() calling returnToPiTrex() 2\n");
    returnToPiTrex();
  }
  if (useUSB == 1)
    if (usbIRQHandler != 0)
      usbIRQHandler (usbIRQParam);
  
  // handle the pipeline
  handlePipeline();

  // Hz in cycle counter
  // 1 second in nano = 1000*1000*1000
  unsigned int nanoClientHz = (1000*1000*1000)/clientHz;
  unsigned int nanoCounter = 0;

  // code to wait for client HZ
  do
  {
      CCNT0(nanoCounter);
  } while (nanoCounter < nanoClientHz);

  // reset core 0 cycle counting
  PMNC(CYCLE_COUNTER_ENABLE|COUNTER_ZERO|CYCLE_COUNTER_RESET);
}

// following is the "output" to vectrex function

#undef LINE_DEBUG_OUT

#ifdef NO_DEBUG
#else
#define LINE_DEBUG_OUT(...) 
#endif

// switch of "USE_TIME"
// in a multi core we do more or less an active wait while drawing


#undef USE_TIME
#define USE_TIME(a) do{;} while(0)
 
#undef waitT1OffMark
#define waitT1OffMark(waiter) \
do {\
  unsigned int value1;\
  do\
  {\
    CCNT0(value1);\
  } while (value1 < (t1OffMark+((waiter)*ONE_WAIT_UNIT) ) );\
} while (0)

#undef waitCounter1MarkLocal
#define waitCounter1MarkLocal(waiter) \
do {\
  static unsigned int value;\
  /* Read CCNT Register */ \
  do\
  {\
    CCNT0(value);\
  } while (value < (piCounterMark+((waiter)*ONE_WAIT_UNIT) ) );\
} while (0)

int handleVectrexOutputSMP()
{
      HANDLE_SOUND
      HANDLE_BUTTONS
      HANDLE_JOYSTICK

      PLAY_SAMPLE_IGNORE_A();
      if ((currentButtonState&0xf) == 0xf) // button 1+ 2 + 3+4 -> go menu
      {
        pendingReturnToPiTrex = 1;
        v_removeMultiCore();
      }

      // todo add in game settings mode
/*
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
*/

      //******************************************************************
      if (v_sampleEnabled)
      {
        ioDone = 0;
        SWITCH_BEAM_OFF();
        CCNT0(roundCycles);
        // unsigned int drawTime = roundCycles/666;
        // printf("s Core1 draw: %i\n", drawTime);

        // wait for Via T2 to expire -> 50Hz
        while ((GET(VIA_int_flags) & 0x20) == 0) 
          PLAY_SAMPLE_IGNORE_A();
      }
      else
      {
        ioDone = 0;
        SWITCH_BEAM_OFF();
        CCNT0(roundCycles);
        // unsigned int drawTime = roundCycles/666;
        // printf("ns Core1 draw: %i\n", drawTime);
	
        // wait for Via T2 to expire -> 50Hz
        while ((GET(VIA_int_flags) & 0x20) == 0) ;
      }
    
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
      
      
    ////////////////////////////////////////////////////////////      
    DEFLOCK
    IRESET
      
     // perhaps add a sample in between?
     consecutiveDraws = 0; 
      
#define __SMP_PIPELINE__      
      #include "pipeline.i" 
#undef __SMP_PIPELINE__      
}
#else
void v_setupSMPHandling() {}
void v_removeMultiCore() {}
#endif // #if RASPPI != 1 






