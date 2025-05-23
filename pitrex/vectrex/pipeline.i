//Why does calling a FUNCTION (playSample()) work in IRQ mode?

/*
todo: 
 - last line in cluster to short
 - find a sweet spot so display is nice!
 - setting with IRQ

 
 cluster scales
 
 samples
 
 
Test:
 SMP
 DOTS
 Browse mode display in irq
*/

// correct sample macro for the pipeline we use
// either "normal" -> no sample playing
// or IRQ/SMP
#undef PLAY_SAMPLE
#undef PLAY_SAMPLE_IGNORE_A

#ifdef SUPPORT_SAMPLES
  #ifdef __NORMAL_PIPELINE__
    #define PLAY_SAMPLE() 
    #define PLAY_SAMPLE_IGNORE_A()
  #else // now IRQ + SMP
    // same as in "samples.c"
    #define PLAY_SAMPLE() do {v_playOneSample();} while(0)
    #define PLAY_SAMPLE_IGNORE_A() do {v_playOneSample_ignoreA();} while(0)
  #endif // __NORMAL_PIPELINE__
#else
  #define PLAY_SAMPLE() 
  #define PLAY_SAMPLE_IGNORE_A()
#endif



#define ENSURE_T1_USAGE \
  I_SET (VIA_aux_cntl, 0x98); \
  ADD_DELAY_CYCLES(2); \


#define RESET_T1_USAGE \
  I_SET (VIA_aux_cntl, 0x18); \
  ADD_DELAY_CYCLES(2); \

  
/******************************************/
/*************** SMP **********************/
/******************************************/
  
#ifdef __SMP_PIPELINE__      
  #undef USE_TIME
  #define USE_TIME(a) do{;} while(0)

  #undef waitT1OffMark
  #define waitT1OffMark(waiter) \
  do {\
    volatile unsigned int value1;\
    do\
    {\
      CCNT0(value1);\
    } while (value1 < (t1OffMark+((waiter)*ONE_WAIT_UNIT) ) );\
  } while (0)

  #ifdef NO_DEBUG
  #else
  #undef LINE_DEBUG_OUT
  #define LINE_DEBUG_OUT(...) \
          if (((browseMode) && (lineNo==currentBrowsline) && (currentDisplayedBrowseLine != currentBrowsline)) || (myDebug))  \
          { \
            printf(__VA_ARGS__); \
          }
  #endif

#endif //__SMP_PIPELINE__      

/******************************************/
/*************** IRQ **********************/
/******************************************/

#ifdef __IRQ_PIPELINE__
  #undef USE_TIME
  // a in arm cycles
  #define USE_TIME(a) \
  do{\
  MAIN_TAKEOVER(((a)/ONE_WAIT_UNIT)) \
  } while(0)



// in IRQMode in Hyperspace
// you MUST NOT changed variables used to calculate the pipeline, this includes
// currentScale, 
//extern int16_t currentPortA; // == portA (also X SH)
//extern int16_t currentYSH; // Y SH
//extern int16_t currentZSH; // Brightness
//
// pipeline section of IRQ handler
// ugly, fast - and ALL in one function, so it is
// all reachable via a "jump"

  #ifdef NO_DEBUG
  #else
  #define LINE_DEBUG_OUT(...) \
          if (((browseMode) && (lineNo==currentBrowsline) && (currentDisplayedBrowseLine != currentBrowsline)) || (myDebug))  \
          { \
            printf(__VA_ARGS__); \
          }
  #endif

  #undef waitT1OffMark
  #define waitT1OffMark(waiter) \
  do {\
    unsigned int value;\
    /* Read CCNT Register */ \
    CCNT0(value);\
    if (value < (t1OffMark+((waiter)*ONE_WAIT_UNIT) ) )\
    {\
      USE_TIME( (t1OffMark+((waiter)*ONE_WAIT_UNIT) ) -value);\
    }\
    unsigned int value1;\
    do\
    {\
      CCNT0(value1);\
    } while (value1 < (t1OffMark+((waiter)*ONE_WAIT_UNIT) ) );\
  } while (0)

#endif //__IRQ_PIPELINE__

/******************************************/
/*************** NORMAL *******************/
/******************************************/
#ifdef __NORMAL_PIPELINE__

  #undef USE_TIME
  #define USE_TIME(a) do{if ((signed int)(a)>0) {;} } while(0)

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

  #undef waitT1OffMark
  #define waitT1OffMark(waiter) \
  do {\
    unsigned int value;\
    /* Read CCNT Register */ \
    do\
    {\
      CCNT0(value);\
      USE_TIME( (t1OffMark+((waiter)*ONE_WAIT_UNIT) ) -value);\
    } while (value < (t1OffMark+((waiter)*ONE_WAIT_UNIT) ) );\
  } while (0)

  int lineNo = 0;
  int c = 0;
  unsigned int t1OffMark = 0;
  vectorPipeLineIsRead = vectorPipeLineNextRead;
  VectorPipeline *dpl = _VPL[vectorPipeLineIsRead];
  int delayedBeamOff=0;

  uint32_t startValue;
  CCNT0(startValue);
  #ifdef __IRQ_PIPELINE__
  static uint32_t irqBreakCheckValue;
  irqBreakCheckValue=startValue;
  #endif

#ifdef MY_DEBUG
  if (myDebug) printf("Display pipeline started...!\r\n");

  if (browseMode)
  {
    v_setBrightness(50);
  }
#endif // MY_DEBUG
#endif //__NORMAL_PIPELINE__

{
  static int c;
  static unsigned int t1OffMark;
  static VectorPipeline *dpl;
  static int delayedBeamOff;
  static int lineNo;
  static int pCount;

  c = 0;
  t1OffMark = 0; // this is the value of counter0 as a MARK, where the T1 was finished!, in ARM cycles


#if RASPPI != 1 
  getLock();
#endif

  vectorPipeLineIsRead = vectorPipeLineNextRead;
  dpl = _VPL[vectorPipeLineIsRead];

#if RASPPI != 1 
  rpl = dpl;
  releaseLock();
#endif


  delayedBeamOff=0;
  lineNo = 0;
  pCount = 0;

#ifdef MY_DEBUG
  if (myDebug) printf("Display pipeline started...!\r\n");
  uint32_t startValue;
  if (myDebug)  
  {
    CCNT0(startValue);
  }
  if (browseMode)
  {
    v_setBrightness(50);
  }
#endif


  while (dpl[c].type != PL_END)
  {
#ifdef __SMP_PIPELINE__      
    if (vectorPipeLineWriting == vectorPipeLineIsRead) printf("DANGER!!!!!!!!!\n");
    if (wpl == rpl) printf("DANGER EVOLVED - READ == WRITE!!!\n");
#endif
    PLAY_SAMPLE();

#ifdef PITREX_DEBUG
    LINE_DEBUG_OUT("---- %i ----\r\n", lineNo);
    if (browseMode)
    {
        if (lineNo ==currentBrowsline)
        {
          v_setBrightness(127);
        I_SET (VIA_port_b, 0x81);
        DELAY_PORT_B_BEFORE_PORT_A();
        I_SET (VIA_port_a, 0x00);
        DELAY_YSH();
        DELAY_YSH();
        DELAY_YSH();
        }
        if (lineNo ==currentBrowsline+1)
        {
          v_setBrightness(50);
        }
    }
#endif

#include "pipelineSwitch.i"	
    



    c++;
    delayedBeamOff = 0;
    LINE_DEBUG_OUT ("Switch Done\n\r");
    CCNT0(t1OffMark); // reset T1 "off" marker

    if (dpl[c].flags & PL_LAST_MUST_FINISH )
    {
      LINE_DEBUG_OUT ("(c+1) Line must finish\n\r");
      if (dpl[c].flags & PL_LAST_IS_RAMPING)
      {
        /* Comment from SMP Pipeline code 
        It seems as soon as we do ANY
        IRQ freeing instead of only T1 timer
        screen will have  garbage mixed in as soon as it is crowded???

        synced - treten keienrlei fehler auf!
        liegt es daran, das das switchen einer zu schnellen pipeline nicht funktioniert???


        Mögliche Ursachen
        Fehler tritt NUR bei non sync auf
        Fehler tritt nur bei client Hz > Vectrex Hz (wenn die tatsächlcih erreichte Client Hz auch größer Vectrex Hz ist)

        a) Waiter hat nicht die korrekt Zeit
        b) es wird "irgendwie" zweimal die gleiche Zeit gewaitet
        c) read/write pipeleine zuordnung is nicht korrekt
        d) Fehler tritt auch bei nicht "USE TIME" auf

        es seiht so aus, als ob
        a) der IRQ manchmal nicht rechtzeitig zurückkommt - wobei das unsinn ist, da die "längen" der Vectoren nicht vom IRQ abhängen sondern von T1, und das bleibt immer gleich
        b) ZERO 
        c) Calib?, Fehler tritt auch ohne calibration auf
        d) "delays" der üblichen Art habe scheins keine Auswirkungen
        e) sound hat wahrsch. keine Auswirkungen, da Gravitar BAD ist -> keine Sound Ausgabe
        */
#ifdef __SMP_PIPELINE__      
        {
          static unsigned int value;
          static int v1;

          {
            // waiting makes no difference to SMP problem  
            unsigned int waiteeer = (piCounterMark+((dpl[c-1].this_timing)*ONE_WAIT_UNIT) );
          
          
            // CHANGE !!!
            do
            {
              CCNT0(value);
            } 
            while (value < (waiteeer) );
            //    Vectors still SHAKING! SMP
            //    What todo?
          }
          /*
          !!!
          Reset IFlags:
          - READ to counter T1 1o (counter, not latch) 0x04
          - WRITE to counter T1 hi (counter, not latch) 0x05
          - WRITE 0x40 to IRQ Flag register
          */

        
          //In SMP/PiZero2 T1 interrupt flag is sometimes not set/read
          //and the following line than is an endless loop!
          
          //  while ((GET(VIA_int_flags) & 0x40) == 0);

        }
        //  while ((GET(VIA_int_flags) & 0x40) == 0);
        // CHANGE TO HERE !!!
#else // __SMP_PIPELINE__



        static unsigned int value;
        static int v1;
        CCNT0(value);
        if (value < (piCounterMark+((dpl[c-1].this_timing)*ONE_WAIT_UNIT) ) )
        {
          USE_TIME( (piCounterMark+((dpl[c-1].this_timing)*ONE_WAIT_UNIT) ) - value);
        }
        while ((GET(VIA_int_flags) & 0x40) == 0);
#endif

        //switch on dot beam earlier off -> starwars stars

        // set T1 off marker 
        CCNT0(t1OffMark);
        if ((dpl[c-1].type == PL_MOVE) && (dpl[c].type == PL_DRAW_DOT)) // same as !((dpl[c].flags & PL_SWITCH_BEAM_OFF)
        {
          // only needed for dots?
          // only needed, when (dpl[c].flags & PL_SWITCH_BEAM_ON)?
          waitT1OffMark(DELAY_AFTER_T1_END_VALUE);

        }
      }
      else
      {
        // e.g. zeroing
        // dots
        LINE_DEBUG_OUT ("Dot/Zero delay: %i\n\r", dpl[c].last_timing);
        ADD_DELAY_CYCLES(dpl[c].last_timing);
      }
    }

#ifdef PITREX_DEBUG
    if (dpl[c].flags & PL_LAST_MUST_FINISH )
    {
      if ((browseMode) && ((dpl[c-1].type ==PL_DRAW)||(dpl[c-1].type ==PL_DRAW_DOT)||(dpl[c-1].type ==PL_DRAW_PATTERN)   ))
      {
        if (lineNo==currentBrowsline)
          currentDisplayedBrowseLine = currentBrowsline;
        lineNo++;
      }
    }
#endif      
     
    if ((dpl[c].flags & PL_SWITCH_BEAM_OFF)/* && (dpl[c].type != PL_ZERO)*/)
    {
      LINE_DEBUG_OUT ("(c+1) PL_SWITCH_BEAM_OFF\n\r");
      LINE_DEBUG_OUT ("DEBUG: %s", dpl[c].debug);
      // last was with light on, now switch it off
      // or at least prepare switch off

#ifdef __SMP_PIPELINE__      
      if (((dpl[c].type == PL_MOVE) ||         (dpl[c].type == PL_DRAW)|| (dpl[c].type == PL_DRAW_PATTERN) ) && (dpl[c-1].type != PL_DRAW_DOT)) 
#else
      if (( /* (dpl[c].type == PL_MOVE)  || */ (dpl[c].type == PL_DRAW)|| (dpl[c].type == PL_DRAW_PATTERN) ) && (dpl[c-1].type != PL_DRAW_DOT)) 
#endif
      {
        // we were drawing now - and next we will either move or draw again
        delayedBeamOff = 1;
        LINE_DEBUG_OUT ("Delayed beam off: \n\r");
      }
      else
      {
        // we are last was a dot or a zero
        // pls don't ask why +5
        // it just is !)(/"(!=!??!!!
        // e.g. this is a move, last time was a draw -> switch off

#ifdef __SMP_PIPELINE__      
        waitT1OffMark(DELAY_AFTER_T1_END_VALUE+5);
#else
        waitT1OffMark(DELAY_AFTER_T1_END_VALUE+5 +8);
#endif

        LINE_DEBUG_OUT ("Beam off: \n\r");
        LINE_DEBUG_OUT ("DEBUG: %s", dpl[c].debug);
        SWITCH_BEAM_OFF();
#ifdef __IRQ_PIPELINE__
vCritical = 0;
#endif
        
        if ((dpl[c-1].flags & PL_DEFLOK_AFTER)/* && (dpl[c].type != PL_ZERO)*/)
        {
          LINE_DEBUG_OUT ("Deflok after: \n\r");
          ////////////////////////////////////////////////////////////
          // v_resetIntegratorOffsets0();
          //  printf("CALIBRATION 0\r\n");
          I_SET (VIA_port_b, 0x81);
          DELAY_PORT_B_BEFORE_PORT_A();
          I_SET_A(0x00);
          ADD_DELAY_CYCLES(4);
          I_SET (VIA_port_b, 0x80);
          ADD_DELAY_CYCLES(6);

          // reset integrators
          I_SET (VIA_port_b, 0x82);    // mux=1, enable mux - integrator offset = 0
          ADD_DELAY_CYCLES(6);
          I_SET (VIA_port_b, 0x81);    // disable mux
          ADD_DELAY_CYCLES(4);

          ////////////////////////////////////////////////////////////
          //          ZERO_AND_WAIT();
          ZERO_AND_CONTINUE(); 
#ifdef __SMP_PIPELINE__      
          waitT1OffMark(DELAY_ZERO_VALUE);
#else
          ADD_DELAY_CYCLES(DELAY_ZERO_VALUE);
#endif
          PLAY_SAMPLE_IGNORE_A();
        }        
      }
    }
  }

  // savety only
  SWITCH_BEAM_OFF();
#ifdef __IRQ_PIPELINE__
vCritical = 0;
#endif
  ZERO_AND_CONTINUE();
#ifdef MY_DEBUG
  static uint32_t value;
  CCNT0(value);

  value = (value-startValue) / 666; // in vectrex cycles
  if (myDebug)   printf("Pipeline display: %i\r\n", value);
#endif    

}
