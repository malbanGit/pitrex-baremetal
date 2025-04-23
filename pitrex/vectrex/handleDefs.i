#define IRESET \
		/* v_resetIntegratorOffsets0(); */ \
        I_SET (VIA_port_b, 0x81); \
        DELAY_PORT_B_BEFORE_PORT_A(); \
        I_SET (VIA_port_a, 0x00); \
        ADD_DELAY_CYCLES(4); \
        I_SET (VIA_port_b, 0x80); \
        ADD_DELAY_CYCLES(6); \
        /* reset integrators \
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
        /* v_doSound(); \
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

#define HANDLE_BUTTONS \
      PLAY_SAMPLE_IGNORE_A(); \
      if (irqButtons) \
      { \
        /*v_readButtons(); */ \
        ioDone |= V_BUTTONS_READ; \
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

        internalButtonState = currentButtonState; \
        if (inCalibration) \
        { \
          currentButtonState = 0; \
        } \
      }

#define HANDLE_JOYSTICK \
      PLAY_SAMPLE_IGNORE_A(); \
      if (irqJoyAnalog)  \
      { \
        /* v_readJoystick1Analog(); */ \
        ioDone |= V_JOY_ANALOG_READ; \
        static int difx; \
        static int dify; \
        static uint8_t compareBit; \
        static int8_t thisInternalJoy1Y; \
        static int8_t thisInternalJoy1X; \
 \
        if (irqJoyAnalog & (1<<1) )  \
        { \
          I_SET(VIA_port_a, 0x00); /* clear VIA port A */ \
          ADD_DELAY_CYCLES(4); \
          I_SET(VIA_port_b, 0x82); /* set VIA port B mux enabled, mux sel = 01 (vertical pot port 0)*/  \
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
        if (irqJoyAnalog & (1) ) \
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
        if (irqJoyAnalog & (1<<3) ) \
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
          compareBit = 0x80; \
          do \
          { \
            I_SET(VIA_port_a, currentJoy2Y); /* load a with test value (positive y), test value to DAC */ \
            ADD_DELAY_CYCLES(4); \
            if ((I_GET(VIA_port_b) & 0x20) == 0) \
            { \
              currentJoy2Y = currentJoy2Y ^ compareBit; \
            } \
            ADD_DELAY_CYCLES(4); \
            compareBit = compareBit>>1; \
            currentJoy2Y = currentJoy2Y | compareBit; \
          } while (compareBit!=0); \
        } \
 \
        if (irqJoyAnalog & (1<<2) ) \
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
          compareBit = 0x80; \
          do \
          { \
            I_SET(VIA_port_a, currentJoy2X); /* load a with test value (positive y), test value to DAC */ \
            ADD_DELAY_CYCLES(4); \
            if ((I_GET(VIA_port_b) & 0x20) == 0) \
            { \
              currentJoy2X = currentJoy2X ^ compareBit; \
            } \
            ADD_DELAY_CYCLES(4); \
            compareBit = compareBit>>1; \
            currentJoy2X = currentJoy2X | compareBit; \
          } while (compareBit!=0); \
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
 \
      } \
      else if (irqJoyDigital)  \
      { \
        /*v_readJoystick1Digital(); \
        ioDone |= V_JOY_DIGITAL_READ; \
        if (irqJoyDigital & (1<<1) ) \
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
        if (irqJoyDigital & (1) ) \
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
        if (irqJoyDigital & (1<<3) ) \
        { \
          /* Y  */ \
          I_SET(VIA_port_a, 0x00); /* clear VIA port A */ \
          ADD_DELAY_CYCLES(4); \
          I_SET(VIA_port_b, 0x82+0x04); /* set VIA port B mux enabled, mux sel = 03 (vertical pot port 1) */  \
          /* wait for joystick comparators to "settle" */ \
          ADD_DELAY_CYCLES(60); /* must be tested! can probably be less? */ \
			 \
          currentJoy2Y = -1; /* default down */ \
          I_SET(VIA_port_b, 0x83); /* set VIA port B mux \
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
        if (irqJoyDigital & (1<<2) ) \
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
	    if (invertYAxis)  \
	    { \
		  currentJoy1Y = ((currentJoy1Y==-128)?127:-currentJoy1Y); \
		  currentJoy2Y = ((currentJoy2Y==-128)?127:-currentJoy2Y); \
  	    } \
        /* set port A reference value to unkown */ \
        currentYSH = currentPortA=0x100; /* reset saved current values to unkown state */ \
 \
        PLAY_SAMPLE_IGNORE_A(); \
        IRESET \
 \
      }
	  
#define DEFLOCK \
/*v_deflok(); */ \
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
