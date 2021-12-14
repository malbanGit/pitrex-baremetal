////////////////////////////////////////////////////////////      
// CONFIG
////////////////////////////////////////////////////////////      

// in ARM cycles
// this default version does NOT
// use any IRQ time
#define USE_TIME(a) do{;} while(0)

////////////////////////////////////////////////////////////      
// CONFIG END
////////////////////////////////////////////////////////////      







////////////////////////////////////////////////////////////      
// IRQ and general Waiter helper
////////////////////////////////////////////////////////////      

// one "round" is measured with COUNTER 1
// one vectrex WRITE (a,b, shift) is measured with CYCLE COUNTER
// T1 "marks" are also measured with T1 counters -> but with mark and difference!
// T1 also marks the start of a ZERO    

// sets piCounterMark to the current cycle count
#define setCounter1Mark() CCNT0(piCounterMark)


// waits from ast set counter mark
// up to that mark + waiter
// waiter is in Vectrex cycles and will be be multiplied by "ONE_WAIT_UNIT" - which is the cycle count equivalent (default 666)
#define waitCounter1Mark(waiter) \
do {\
  static unsigned int value;\
  /* Read CCNT Register */ \
  do\
  {\
    CCNT0(value);\
    USE_TIME( (piCounterMark+((waiter)*ONE_WAIT_UNIT) ) -value);\
  } while (value < (piCounterMark+((waiter)*ONE_WAIT_UNIT) ) );\
} while (0)

// waiter in arm cycles
// that means in nano seconds
// waiter always in relation to last cleared cycle counter
// this is usually the last VIA write
#define waitUntil(waiter) \
do {\
  unsigned int value;\
  /* Read CCNT Register */ \
  do\
  {\
    CCNT(value);\
  } while (value < (waiter) );\
} while (0)

// IRQ - this one does apply USE_TIME 
#define waitUntilAndUse(waiter) \
do {\
  unsigned int value;\
  /* Read CCNT Register */ \
  CCNT(value);\
  if ((waiter) >value)\
  {\
    USE_TIME((waiter)-value);\
  }\
  unsigned int value1;\
  do\
  {\
    CCNT(value1);\
  } while (value1 < (waiter) );\
} while (0)

#define ADD_DELAY_CYCLES(c) do{setWaitMin += (c);} while (0)
#define DELAY_PORT_B_BEFORE_PORT_A() do{ADD_DELAY_CYCLES(DELAY_PORT_B_BEFORE_PORT_A_VALUE);} while (0)
#define DELAY_PORT_B_BEFORE_PORT_A_BRIGHTNESS() do{ADD_DELAY_CYCLES(DELAY_PORT_B_BEFORE_PORT_A_VALUE+1);} while (0)
#define DELAY_CRANKY_BETWEEN_VIA_B() do{if (crankyFlag & CRANKY_BETWEEN_VIA_B){ADD_DELAY_CYCLES(crankyFlag&0x07);}} while (0)// 
#define DELAY_CRANKY_Y_TO_NULL() do{if (crankyFlag & CRANKY_NULLING_WAIT){ADD_DELAY_CYCLES (CRANKY_DELAY_Y_TO_NULL_VALUE);}} while (0)// only VERY strange vectrex
#define DELAY_XSH() do{ADD_DELAY_CYCLES(DELAY_AFTER_XSH_VALUE);} while (0)
#define DELAY_ZSH() DELAY_YSH()
#define DELAY_YSH()  \
    do{ADD_DELAY_CYCLES(DELAY_AFTER_YSH_VALUE); \
    DELAY_CRANKY_BETWEEN_VIA_B();} while (0)

#define START_T1_TIMER()  \
do { \
    if (lastScale == currentScale) \
    { \
        SET(VIA_t1_cnt_hi, currentScale>>8);  \
    } \
    else \
    { \
        SETW_inverse(VIA_t1, currentScale);  /* scale due to "enlargement" is 16 bit! */ \
    }  \
    setCounter1Mark(); \
} while (0)
#define WAIT_T1_END_LAST() do{waitCounter1Mark(lastScale+DELAY_AFTER_T1_END_VALUE);   } while (0)
#define WAIT_T1_END()      do{waitCounter1Mark(currentScale+DELAY_AFTER_T1_END_VALUE);} while (0)
#define START_WAIT_T1() do{START_T1_TIMER();WAIT_T1_END();} while (0)


////////////////////////////////////////////////////////////      
// End Waiter helper
////////////////////////////////////////////////////////////      
















// all setter WAIT for "setWaitMin*ONE_WAIT_UNIT"
// all things starting with I_ also initialize the setWaitMin to 0

// some sets should "overstep" the waiter
// they should wait, but not reset the waiter
// e.g. if we change some register while "waiting" for another
// IRQ - this one does apply USE_TIME 
#define SET(address, val) \
do\
{\
  waitUntilAndUse(setWaitMin*ONE_WAIT_UNIT);\
  vectrexwrite(address, val);\
  lastViaWrite = (address&0xf);\
} while(0)

#define GET(address) vectrexread(address)


// usually "getters" do not need a delay
// I think in case of joystick read (orPSG in general?)
// there might be one needed
// since the last B- MUX is DIRECTLY in front
// therefore we also have a "wait getter" (which also resets)
// a "return" can not be done with a #define
// therefor here an inline function
// IRQ - this one does apply USE_TIME 
static inline unsigned char I_GET(int address)
{
  waitUntilAndUse(setWaitMin*ONE_WAIT_UNIT);
  setWaitMin = 0;
  return vectrexread(address);
}

// some sets should "overstep" the waiter
// they should wait, but not reset the waiter
// e.g. if we change some register while "waiting" for another
#define SET_noUse(address, val) \
do\
{\
  waitUntil(setWaitMin*ONE_WAIT_UNIT);\
  vectrexwrite(address, val);\
  lastViaWrite = (address&0xf);\
} while(0)


// IRQ - this one does apply USE_TIME 
#define I_SET(address, val) \
do\
{\
  waitUntilAndUse(setWaitMin*ONE_WAIT_UNIT);\
  vectrexwrite(address, val);\
  PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET);\
  setWaitMin = 0;\
} while(0)



#ifdef SUPPORT_SAMPLES

// saving register A
// in case we play a sample and must restore A
// IRQ - this one does apply USE_TIME 
#define I_SET_A(val) \
do\
{\
  waitUntilAndUse(setWaitMin*ONE_WAIT_UNIT);\
  vectrexwrite(0xd001, val);\
  PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET);\
  lastPortA=val;\
  setWaitMin = 0;\
} while(0)

#else
// IRQ - this one does apply USE_TIME 
#define I_SET_A(a) I_SET(VIA_port_a,a)
#endif


#define I_SET_noUse(address, val) \
do\
{\
  waitUntil(setWaitMin*ONE_WAIT_UNIT);\
  vectrexwrite(address, val);\
  PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET);\
  lastViaWrite = (address&0xf);\
  setWaitMin = 0;\
} while(0)

// IRQ - this one does apply USE_TIME 
#define I_SET_WORD_REVERSE(adr, v1, v2) do {I_SET( (adr)+1, v2); DELAY_CRANKY_BETWEEN_VIA_B(); I_SET(adr, v1); } while(0)
// IRQ - this one does apply USE_TIME 
#define I_SET_WORD_ORDERED(adr, v1, v2) do {I_SET( (adr), v1); DELAY_PORT_B_BEFORE_PORT_A(); I_SET(adr+1, v2); } while(0)

// IRQ - this one does apply USE_TIME 
#define SETW_inverse(address, val) do {SET_noUse((address), (val)&255);SET_noUse((address)+1, ((val)>>8)&255); } while(0)
// IRQ - this one does apply USE_TIME 
#define SETW(address, val) do {SET_noUse(address, ((val)>>8)&255); SET_noUse((address)+1, (val)&255);} while(0)

// IRQ - this one does apply USE_TIME 
#define I_SETW_inverse(address, val) do {I_SET_noUse((address), (val)&255);I_SET_noUse((address)+1, ((val)>>8)&255); } while(0)
// IRQ - this one does apply USE_TIME 
#define I_SETW(address, val) do {I_SET_noUse(address, ((val)>>8)&255); I_SET_noUse((address)+1, (val)&255);} while(0)














////////////////////////////////////////////////////////////      
// Higher level macros, still primitives though!
////////////////////////////////////////////////////////////      



#define UNZERO() SET(VIA_cntl, 0xCE) // disable zeroing, otherwise no positioning can be done

// sets counter mark
#define ZERO_AND_CONTINUE() do{SET(VIA_cntl, 0xCC);setCounter1Mark();currentCursorX = 0; currentCursorY = 0;} while (0)

#define ZERO_AND_WAIT() do{ \
    ZERO_AND_CONTINUE(); \
    waitCounter1Mark(DELAY_ZERO_VALUE);} while (0)

#define SET_YSH_IMMEDIATE_8(v) do{ \
    I_SET(VIA_port_b, 0x80); \
    DELAY_PORT_B_BEFORE_PORT_A(); \
    currentYSH=currentPortA=(uint8_t)(v); \
    I_SET(VIA_port_a, currentPortA); \
    DELAY_YSH();} while (0)

#define SET_XSH_IMMEDIATE_8(v) do{ \
    I_SET(VIA_port_b, 0x81); \
    DELAY_PORT_B_BEFORE_PORT_A(); \
    currentPortA=(uint8_t)(v); \
    I_SET(VIA_port_a, currentPortA); \
    DELAY_XSH();} while (0)

////////////////////////////////////////////////////////////      
// END Higher level macros, still primitives though!
////////////////////////////////////////////////////////////      



// from here on "samples" can be configured

////////////////////////////////////////////////////////////      
// DEFLOK
////////////////////////////////////////////////////////////
//v_deflok();
#define V_DEFLOK_SAM() do{\
    ZERO_AND_CONTINUE(); \
    waitCounter1MarkLocal(DELAY_ZERO_VALUE);\
    PLAY_SAMPLE_IGNORE_A();\
\
    UNZERO();\
    I_SET(VIA_port_b, 0x80); \
    DELAY_PORT_B_BEFORE_PORT_A(); \
    I_SET_A(((uint8_t)(127))); \
    DELAY_YSH();\
\
    I_SET(VIA_port_b, 0x81); \
    DELAY_XSH();\
\
    SETW_inverse(VIA_t1, 255); setCounter1Mark(); \
    waitCounter1MarkLocal(255+DELAY_AFTER_T1_END_VALUE);\
    PLAY_SAMPLE_IGNORE_A();\
\
    ZERO_AND_CONTINUE(); \
    waitCounter1MarkLocal(DELAY_ZERO_VALUE);\
    PLAY_SAMPLE_IGNORE_A();\
\
    UNZERO();\
    I_SET(VIA_port_b, 0x80); \
    DELAY_PORT_B_BEFORE_PORT_A(); \
    I_SET_A(((uint8_t)(-127))); \
    DELAY_YSH();\
    I_SET(VIA_port_b, 0x81); \
    DELAY_XSH();\
\
    SET(VIA_t1_cnt_hi, 0); setCounter1Mark(); \
    waitCounter1MarkLocal(255+DELAY_AFTER_T1_END_VALUE);\
    PLAY_SAMPLE_IGNORE_A();\
\
    ZERO_AND_CONTINUE(); \
    waitCounter1MarkLocal(DELAY_ZERO_VALUE);\
    PLAY_SAMPLE_IGNORE_A();\
    } while (0)
////////////////////////////////////////////////////////////      


////////////////////////////////////////////////////////////
// v_resetIntegratorOffsets0();
#define V_INTEGRATOR_RESET0_SAM() do{ \
  I_SET (VIA_port_b, 0x81); \
  DELAY_PORT_B_BEFORE_PORT_A(); \
  I_SET_A(0x00); \
  ADD_DELAY_CYCLES(4); \
  I_SET (VIA_port_b, 0x80); \
  ADD_DELAY_CYCLES(6); \
  // reset integrators \
  I_SET (VIA_port_b, 0x82); \
  ADD_DELAY_CYCLES(6); \
  I_SET (VIA_port_b, 0x81); \
  ADD_DELAY_CYCLES(4); \
  PLAY_SAMPLE_IGNORE_A(); \
   } while (0)
////////////////////////////////////////////////////////////



