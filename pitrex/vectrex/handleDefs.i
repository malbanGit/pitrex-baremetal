/*
todo 
- unify pipeline
  a) SMP
  b) IRQ
  c) "normal"
- impl. new drawing method  
  */


#define IRESET \
      /* v_resetIntegratorOffsets0(); */ \
      I_SET (VIA_port_b, 0x81); \
      DELAY_PORT_B_BEFORE_PORT_A(); \
      I_SET (VIA_port_a, 0x00); \
      ADD_DELAY_CYCLES(4); \
      I_SET (VIA_port_b, 0x80); \
      ADD_DELAY_CYCLES(6); \
      /* reset integrators */ \
      I_SET (VIA_port_b, 0x82);    /* mux=1, enable mux - integrator offset = 0 */ \
      ADD_DELAY_CYCLES(6); \
      I_SET (VIA_port_b, 0x81);    /* disable mux */ \
      ADD_DELAY_CYCLES(4); 

#define HANDLE_SOUND \
      PLAY_SAMPLE_IGNORE_A(); \
      if (irqSound)  \
      { \
        if (extendedIRQSound) \
        { \
          v_playYM(); \
          v_playAllSFX(); \
        } \
        /* v_doSound(); */ \
        static int i; \
        for (i=0;i<15;i++) \
        { \
          static uint8_t data; \
          data = psgDoubleBuffer[i]; \
          if (psgShadow[i] == data) continue; \
          psgShadow[i] = data; \
          I_SET(VIA_port_a, i); /* prepare access of psg port A (0x0e) by writing the register value to VIA port A */ \
          I_SET(VIA_port_b, 0x99); /* set VIA port B to settings: sound BDIR on, BC1 on, mux off */ \
          I_SET(VIA_port_b, 0x81); /* set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive) */ \
 \
	  I_SET(VIA_port_a, data); /* write data to port a of via -> and than to psg */ \
          I_SET(VIA_port_b, 0x91); /* set VIA port B to settings: sound BDIR on, BC1 on, mux off, write to PSG */ \
          I_SET(VIA_port_b, 0x81); /* set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive) */ \
        } \
      } 
#define READ_BUTTONS \
        /*v_readButtons(); */ \
        ioDone |= V_BUTTONS_READ; \
 \
        /* read of buttons goes thru the PSG sound chip, PSG port A */ \
        I_SET(VIA_port_a, 0x0e); /* prepare access of psg port A (0x0e) by writing the register value to VIA port A */ \
        ADD_DELAY_CYCLES(4); \
        I_SET(VIA_port_b, 0x99); /* set VIA port B to settings: sound BDIR on, BC1 on, mux off */ \
        ADD_DELAY_CYCLES(4); \
        I_SET(VIA_port_b, 0x81); /* set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive) */ \
        ADD_DELAY_CYCLES(4); \
		 \
        SET(VIA_DDR_a, 0x00); /* set VIA DDR A to input */  \
        ADD_DELAY_CYCLES(4); \
        I_SET(VIA_port_b, 0x89); /* set VIA port B to settings: sound BDIR on, BC1 on, mux off */ \
        ADD_DELAY_CYCLES(6); \
        currentButtonState = ~I_GET(VIA_port_a); /* Read buttons */ \
        I_SET(VIA_port_b, 0x81); /* set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive) */ \
        ADD_DELAY_CYCLES(4); \
        SET(VIA_DDR_a, 0xff); /* set VIA DDR A to output */ \
        ADD_DELAY_CYCLES(4); \
		currentPortA = 0x100; \
		if (swapPorts==1) \
		{ \
		  static int buf; \
		  buf = currentButtonState; \
		  buf = buf & (0xf0); \
		  buf = buf >> 4; \
		  currentButtonState = currentButtonState & 0x0f; \
		  currentButtonState = currentButtonState << 4; \
		  currentButtonState = currentButtonState | buf; \
		} \
        internalButtonState = currentButtonState; \
        if (inCalibration) \
        { \
          currentButtonState = 0; \
        } \

#define HANDLE_BUTTONS \
      PLAY_SAMPLE_IGNORE_A(); \
      if (irqButtons) \
      { \
		READ_BUTTONS \
      }

#define JOYSTICK_DIGITAL \
        /* v_readJoystick1Digital();*/ \
        ioDone |= V_JOY_DIGITAL_READ; \
	static int irqJoyDigital_tmp; \
	irqJoyDigital_tmp = irqJoyDigital; \
	if (swapPorts==1) \
	{ \
	  irqJoyDigital_tmp = ((irqJoyDigital >> 2)&(1+2)); \
	  irqJoyDigital_tmp |= ((irqJoyDigital << 2)&(4+8)); \
	} \
	if (irqJoyDigital_tmp & (1<<1) ) \
        { \
          /* Y */ \
          I_SET(VIA_port_a, 0x00); /* clear VIA port A */ \
          ADD_DELAY_CYCLES(4); \
          I_SET(VIA_port_b, 0x82); /* set VIA port B mux enabled, mux sel = 01 (vertical pot port 0) */ \
 \
          /* wait for joystick comparators to "settle" */ \
          ADD_DELAY_CYCLES(60); /* must be tested! can probably be less? */ \
 \
          currentJoy1Y = -1; /* default down */ \
          I_SET(VIA_port_b, 0x83); /* set VIA port B mux */ \
          DELAY_PORT_B_BEFORE_PORT_A(); \
          I_SET(VIA_port_a, 0x40); /* load a with test value (positive y), test value to DAC */ \
          ADD_DELAY_CYCLES(4); \
          if ((I_GET(VIA_port_b) & 0x20) == 0x20) \
          { \
            currentJoy1Y = 1; /*up */ \
          } \
          else \
          { \
            I_SET(VIA_port_a, -0x40); /* load a with test value (negative y), test value to DAC */ \
            ADD_DELAY_CYCLES(4); \
            if ((I_GET(VIA_port_b) & 0x20) == 0x20) \
            { \
              currentJoy1Y = 0; /* no direction */ \
            } \
          } \
        } \
        if (irqJoyDigital_tmp & (1) ) \
        { \
          /* X  */ \
          I_SET(VIA_port_a, 0x00); /* clear VIA port A */ \
          ADD_DELAY_CYCLES(4); \
          I_SET(VIA_port_b, 0x80); /* set VIA port B mux enabled, mux sel = 00 (horizontal pot port 0) */ \
          /* wait for joystick comparators to "settle"int no_back = 0; */ \
          ADD_DELAY_CYCLES(60); /* must be tested! can probably be less? */ \
 \
          currentJoy1X = -1; /* default left */ \
          I_SET(VIA_port_b, 0x83); /* set VIA port B mux */ \
          DELAY_PORT_B_BEFORE_PORT_A(); \
          I_SET(VIA_port_a, 0x40); /* load a with test value (positive y), test value to DAC*/ \
          ADD_DELAY_CYCLES(2); \
          if ((I_GET(VIA_port_b) & 0x20) == 0x20) \
          { \
            currentJoy1X = 1; /*right */ \
          } \
          else \
          { \
            ADD_DELAY_CYCLES(4); \
            I_SET(VIA_port_a, -0x40); /* load a with test value (negative y), test value to DAC */ \
            ADD_DELAY_CYCLES(4); \
            if ((I_GET(VIA_port_b) & 0x20) == 0x20) \
            { \
              currentJoy1X = 0; /* no direction */ \
            } \
            ADD_DELAY_CYCLES(4); \
          } \
        } \
 \
        /* if joyport 2  needed.... */ \
        if (irqJoyDigital_tmp & (1<<3) ) \
        { \
          /* Y  */ \
          I_SET(VIA_port_a, 0x00); /* clear VIA port A */ \
          ADD_DELAY_CYCLES(4); \
          I_SET(VIA_port_b, 0x82+0x04); /* set VIA port B mux enabled, mux sel = 03 (vertical pot port 1) */  \
          /* wait for joystick comparators to "settle" */ \
          ADD_DELAY_CYCLES(60); /* must be tested! can probably be less? */ \
			 \
          currentJoy2Y = -1; /* default down */ \
          I_SET(VIA_port_b, 0x83); /* set VIA port B mux */ \
          DELAY_PORT_B_BEFORE_PORT_A(); \
          I_SET(VIA_port_a, 0x40); /* load a with test value (positive y), test value to DAC */ \
          ADD_DELAY_CYCLES(4); \
          if ((I_GET(VIA_port_b) & 0x20) == 0x20) \
          { \
            currentJoy2Y = 1; /*up */ \
          } \
          else \
          { \
            I_SET(VIA_port_a, -0x40); /* load a with test value (negative y), test value to DAC */ \
            ADD_DELAY_CYCLES(4); \
            if ((I_GET(VIA_port_b) & 0x20) == 0x20) \
            { \
              currentJoy2Y = 0; /* no direction*/  \
            } \
          } \
        } \
 \
        if (irqJoyDigital_tmp & (1<<2) ) \
        { \
          /* X */  \
          I_SET(VIA_port_a, 0x00); /* clear VIA port A */ \
          ADD_DELAY_CYCLES(4); \
          I_SET(VIA_port_b, 0x84); /* set VIA port B mux enabled, mux sel = 2 (horizontal pot port 1) */ \
          /* wait for joystick comparators to "settle" */ \
          ADD_DELAY_CYCLES(60); /* must be tested! can probably be less? */ \
 \
          currentJoy2X = -1; /* default left */ \
          I_SET(VIA_port_b, 0x83); /* set VIA port B mux */ \
          DELAY_PORT_B_BEFORE_PORT_A(); \
          I_SET(VIA_port_a, 0x40); /* load a with test value (positive y), test value to DAC */ \
          ADD_DELAY_CYCLES(2); \
          if ((I_GET(VIA_port_b) & 0x20) == 0x20) \
          { \
            currentJoy2X = 1; /*right */  \
          } \
          else \
          { \
            ADD_DELAY_CYCLES(4); \
            I_SET(VIA_port_a, -0x40); /* load a with test value (negative y), test value to DAC */ \
            ADD_DELAY_CYCLES(4); \
            if ((I_GET(VIA_port_b) & 0x20) == 0x20) \
            { \
              currentJoy2X = 0; /* no direction */ \
            } \
            ADD_DELAY_CYCLES(4); \
          } \
        } \
		if (swapPorts == 1) \
		{ \
		  static int x1; \
		  static int y1; \
		  x1=currentJoy1X; \
		  y1=currentJoy1Y; \
		  currentJoy1Y = currentJoy2Y; \
		  currentJoy1X = currentJoy2X; \
		  currentJoy2Y = y1; \
		  currentJoy2X = x1; \
		} \
		if (invertYAxis)  \
		{ \
		  currentJoy1Y = ((currentJoy1Y==-128)?127:-currentJoy1Y); \
		  currentJoy2Y = ((currentJoy2Y==-128)?127:-currentJoy2Y); \
		} \
        /* set port A reference value to unkown */ \
        currentYSH = currentPortA=0x100; /* reset saved current values to unkown state */ \
 \
        PLAY_SAMPLE_IGNORE_A(); \
        IRESET 
 	  
#define JOYSTICK_ANALOG \
      ioDone |= V_JOY_ANALOG_READ; \
      static int irqJoyAnalalog_tmp; \
      irqJoyAnalalog_tmp = irqJoyAnalog; \
      if (swapPorts==1) \
      { \
	  irqJoyAnalalog_tmp = ((irqJoyAnalog >> 2)&(1+2)); \
	  irqJoyAnalalog_tmp |= ((irqJoyAnalog << 2)&(4+8)); \
      } \
      if (irqJoyAnalalog_tmp)  \
      { \
        /* v_readJoystick1Analog(); */ \
        ioDone |= V_JOY_ANALOG_READ; \
        static int difx; \
        static int dify; \
        static uint8_t compareBit; \
        static int8_t thisInternalJoy1Y; \
        static int8_t thisInternalJoy1X; \
        static int8_t thisInternalJoy2Y; \
        static int8_t thisInternalJoy2X; \
 \
        if (irqJoyAnalalog_tmp & (1<<1) )  \
        { \
          I_SET(VIA_port_a, 0x00); /* clear VIA port A */ \
          ADD_DELAY_CYCLES(4); \
          I_SET(VIA_port_b, 0x82); /* set VIA port B mux enabled, mux sel = 01 (vertical pot port 0) */  \
          /* wait for joystick comparators to "settle" */ \
          ADD_DELAY_CYCLES(60); /* must be tested! can probably be less? */ \
 \
          currentJoy1Y = 0; /* default centered */ \
          I_SET(VIA_port_b, 0x83); /* set VIA port B mux disabled */ \
          DELAY_PORT_B_BEFORE_PORT_A(); \
          compareBit = 0x80; \
          thisInternalJoy1Y = 0; \
 \
          do \
          { \
            I_SET(VIA_port_a, thisInternalJoy1Y); /* load a with test value (positive y), test value to DAC */ \
            ADD_DELAY_CYCLES(4); \
            if ((I_GET(VIA_port_b) & 0x20) == 0) \
            { \
              thisInternalJoy1Y = thisInternalJoy1Y ^ compareBit; \
            } \
            ADD_DELAY_CYCLES(4); \
            compareBit = compareBit>>1; \
            thisInternalJoy1Y = thisInternalJoy1Y | compareBit; \
          } while (compareBit!=0); \
        } \
 \
        if (irqJoyAnalalog_tmp & (1) ) \
        { \
          I_SET(VIA_port_a, 0x00); /* clear VIA port A */ \
          ADD_DELAY_CYCLES(4); \
          I_SET(VIA_port_b, 0x80); /* set VIA port B mux enabled, mux sel = 01 (horizontal pot port 0) */ \
          /* wait for joystick comparators to "settle" */ \
          ADD_DELAY_CYCLES(60); /* must be tested! can probably be less? */ \
 \
          currentJoy1X = 0; /* default centered */ \
          I_SET(VIA_port_b, 0x81); /* set VIA port B mux disabled */ \
          DELAY_PORT_B_BEFORE_PORT_A(); \
          compareBit = 0x80; \
 \
          thisInternalJoy1X = 0; \
          do \
          { \
            I_SET(VIA_port_a, thisInternalJoy1X); /* load a with test value (positive y), test value to DAC */ \
            ADD_DELAY_CYCLES(4); \
            if ((I_GET(VIA_port_b) & 0x20) == 0) \
            { \
              thisInternalJoy1X = thisInternalJoy1X ^ compareBit; \
            } \
            ADD_DELAY_CYCLES(4); \
            compareBit = compareBit>>1; \
            thisInternalJoy1X = thisInternalJoy1X | compareBit; \
          } while (compareBit!=0); \
        } \
 \
        /* if joysticks disabled */ \
        /* the values might contain nonsense! */ \
        difx = thisInternalJoy1X - internalJoy1X; \
        dify = thisInternalJoy1Y - internalJoy1Y; \
 \
	internalJoy1X = internalJoy1X+(difx>>1); \
        internalJoy1Y = internalJoy1Y+(dify>>1); \
 \
        if (!inCalibration) \
        { \
          currentJoy1X = internalJoy1X; \
          currentJoy1Y = internalJoy1Y; \
        } \
 \
        /* if joyport 2  needed.... */ \
        if (irqJoyAnalalog_tmp & (1<<3) ) \
        { \
          I_SET(VIA_port_a, 0x00); /* clear VIA port A */ \
          ADD_DELAY_CYCLES(4); \
          I_SET(VIA_port_b, 0x82+0x04); /* set VIA port B mux enabled, mux sel = 03 (vertical pot port 1) */ \
          /* wait for joystick comparators to "settle" */ \
          ADD_DELAY_CYCLES(60); /* must be tested! can probably be less? */ \
 \
          currentJoy2Y = 0; /* default centered */ \
          I_SET(VIA_port_b, 0x83); /* set VIA port B mux disabled */ \
          DELAY_PORT_B_BEFORE_PORT_A(); \
          thisInternalJoy2Y = 0; \
          compareBit = 0x80; \
          do \
          { \
            I_SET(VIA_port_a, thisInternalJoy2Y); /* load a with test value (positive y), test value to DAC */ \
            ADD_DELAY_CYCLES(4); \
            if ((I_GET(VIA_port_b) & 0x20) == 0) \
            { \
              thisInternalJoy2Y = thisInternalJoy2Y ^ compareBit; \
            } \
            ADD_DELAY_CYCLES(4); \
            compareBit = compareBit>>1; \
            thisInternalJoy2Y = thisInternalJoy2Y | compareBit; \
          } while (compareBit!=0); \
        } \
 \
        if (irqJoyAnalalog_tmp & (1<<2) ) \
        { \
          I_SET(VIA_port_a, 0x00); /* clear VIA port A */ \
          ADD_DELAY_CYCLES(4); \
          I_SET(VIA_port_b, 0x84); /* set VIA port B mux enabled, mux sel = 2 (horizontal pot port 1)*/  \
          /* wait for joystick comparators to "settle" */ \
          ADD_DELAY_CYCLES(60); /* must be tested! can probably be less?*/  \
 \
          currentJoy2X = 0; /* default centered */ \
          I_SET(VIA_port_b, 0x81); /* set VIA port B mux disabled */ \
          DELAY_PORT_B_BEFORE_PORT_A(); \
          thisInternalJoy2X = 0; /* default centered */ \
          compareBit = 0x80; \
          do \
          { \
            I_SET(VIA_port_a, thisInternalJoy2X); /* load a with test value (positive y), test value to DAC */ \
            ADD_DELAY_CYCLES(4); \
            if ((I_GET(VIA_port_b) & 0x20) == 0) \
            { \
              thisInternalJoy2X = thisInternalJoy2X ^ compareBit; \
            } \
            ADD_DELAY_CYCLES(4); \
            compareBit = compareBit>>1; \
            thisInternalJoy2X = thisInternalJoy2X | compareBit; \
          } while (compareBit!=0); \
        } \
 \
        /* if joysticks disabled */ \
        /* the values might contain nonsense! */ \
        difx = thisInternalJoy2X - internalJoy2X; \
        dify = thisInternalJoy2Y - internalJoy2Y; \
 \
        internalJoy2X = internalJoy2X+(difx>>1); \
        internalJoy2Y = internalJoy2Y+(dify>>1); \
 \
        if (!inCalibration) \
        { \
          currentJoy2X = internalJoy2X; \
          currentJoy2Y = internalJoy2Y; \
        } \
\
	if (swapPorts == 1) \
	{ \
	  static int x1; \
	  static int y1; \
	  x1=currentJoy1X; \
	  y1=currentJoy1Y; \
	  currentJoy1Y = currentJoy2Y; \
	  currentJoy1X = currentJoy2X; \
	  currentJoy2Y = y1; \
	  currentJoy2X = x1; \
\
	  x1=internalJoy1X; \
	  y1=internalJoy1Y; \
	  internalJoy1Y = internalJoy2Y; \
	  internalJoy1X = internalJoy2X; \
	  internalJoy2Y = y1; \
	  internalJoy2X = x1; \
	} \
\
	if (invertYAxis)  \
	{ \
	  currentJoy1Y = ((currentJoy1Y==-128)?127:-currentJoy1Y); \
	  currentJoy2Y = ((currentJoy2Y==-128)?127:-currentJoy2Y); \
	} \
 \
	PLAY_SAMPLE_IGNORE_A(); \
	IRESET \
      } 

#define HANDLE_JOYSTICK \
      PLAY_SAMPLE_IGNORE_A(); \
      JOYSTICK_ANALOG \
      if (irqJoyDigital)  \
      { \
	  JOYSTICK_DIGITAL \
      }
	  

	  
	  
#define DEFLOCK \
/* v_deflok(); */ \
    ZERO_AND_CONTINUE();  \
    PLAY_SAMPLE_IGNORE_A(); \
    waitCounter1MarkLocal(DELAY_ZERO_VALUE); \
 \
    UNZERO(); \
    I_SET(VIA_port_b, 0x80);  \
    DELAY_PORT_B_BEFORE_PORT_A();  \
    I_SET(VIA_port_a, ((uint8_t)(127)));  \
    DELAY_YSH(); \
 \
    I_SET(VIA_port_b, 0x81);  \
    DELAY_XSH(); \
 \
    SETW_inverse(VIA_t1, 255);   \
    setCounter1Mark();  \
 \
    PLAY_SAMPLE_IGNORE_A(); \
    waitCounter1MarkLocal(255+DELAY_AFTER_T1_END_VALUE); \
 \
    ZERO_AND_CONTINUE();  \
    waitCounter1MarkLocal(DELAY_ZERO_VALUE); \
    PLAY_SAMPLE_IGNORE_A(); \
 \
    UNZERO(); \
    I_SET(VIA_port_b, 0x80);  \
    DELAY_PORT_B_BEFORE_PORT_A();  \
    I_SET(VIA_port_a, ((uint8_t)(-127)));  \
    DELAY_YSH(); \
    I_SET(VIA_port_b, 0x81);  \
    DELAY_XSH(); \
 \
    SET(VIA_t1_cnt_hi, 0);  \
    setCounter1Mark();  \
    PLAY_SAMPLE_IGNORE_A(); \
    waitCounter1MarkLocal(255+DELAY_AFTER_T1_END_VALUE); \
    PLAY_SAMPLE_IGNORE_A(); \
 \
    ZERO_AND_CONTINUE();  \
    waitCounter1MarkLocal(DELAY_ZERO_VALUE); \
    PLAY_SAMPLE_IGNORE_A(); 
