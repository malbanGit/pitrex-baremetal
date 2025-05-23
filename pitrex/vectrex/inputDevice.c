

/***********************************************************************/

/*      Reads the vectrex button states (port 1 and port 2).
 N eeds only be called once per round *(or ev*en only every second round)
 result also in variable: currentButtonState
 bit 0 = button 1 port 1
 bit 1 = button 2 port 1
 bit 2 = button 3 port 1
 bit 3 = button 4 port 1
 bit 4 = button 1 port 2
 bit 5 = button 2 port 2
 bit 6 = button 3 port 2
 bit 7 = button 4 port 2
 if bit == 1, than button is pressed
   if bit == 0, than button is not pressed
     */
uint8_t v_directReadButtons()
{
  int ibs=0;

  I_SET(VIA_port_a, 0x0e); // prepare access of psg port A (0x0e) by writing the register value to VIA port A
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x99); // set VIA port B to settings: sound BDIR on, BC1 on, mux off (latch Address to PSG)
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  ADD_DELAY_CYCLES(4);

  SET(VIA_DDR_a, 0x00); // set VIA DDR A to input
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x89); // set VIA port B to settings: sound BDIR on, BC1 on, mux off (Read from PSG)
  ADD_DELAY_CYCLES(6);
  ibs = ~I_GET(VIA_port_a); // Read buttons
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  ADD_DELAY_CYCLES(4);
  SET(VIA_DDR_a, 0xff); // set VIA DDR A to output
  ADD_DELAY_CYCLES(4);
  currentPortA = 0x100;

  if (swapPorts==1)
  {
    int buf = ibs;
    buf = buf & (0xf0);
    buf = buf >> 4;
    ibs = ibs & 0x0f;
    ibs = ibs << 4;
    ibs = ibs | buf;
  }
  
  
  return ibs;
}

// only once changed per round
uint8_t v_readButtons()
{
  if (isIRQMode) return currentButtonState;
#if RASPPI != 1 
  if (isSMPMode) return currentButtonState;
#endif  
  if (ioDone & V_BUTTONS_READ) return currentButtonState;

  READ_BUTTONS // defined in "handleDefs.i"
/*  
  ioDone |= V_BUTTONS_READ;
  // read of buttons goes thru the PSG sound chip, PSG port A
  //
  I_SET(VIA_port_a, 0x0e); // prepare access of psg port A (0x0e) by writing the register value to VIA port A
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x99); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  ADD_DELAY_CYCLES(4);

  SET(VIA_DDR_a, 0x00); // set VIA DDR A to input
  ADD_DELAY_CYCLES(4);
  I_SET(VIA_port_b, 0x89); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
  ADD_DELAY_CYCLES(6);
  currentButtonState = ~I_GET(VIA_port_a); // Read buttons
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  ADD_DELAY_CYCLES(4);
  SET(VIA_DDR_a, 0xff); // set VIA DDR A to output
  ADD_DELAY_CYCLES(4);
  currentPortA = 0x100;

  if (swapPorts==1)
  {
    int buf = currentButtonState;
    buf = buf & (0xf0);
    buf = buf >> 4;
    currentButtonState = currentButtonState & 0x0f;
    currentButtonState = currentButtonState << 4;
    currentButtonState = currentButtonState | buf;
  }
  
  
  internalButtonState = currentButtonState;
  if (inCalibration)
  {
    currentButtonState = 0;
  }
  */
  return currentButtonState;
}

/***********************************************************************/

/*      Reads the vectrex joystick states (port 1) DIGITAL.
 ( Faster than analog call!)          *      *
 Needs only be called once per round (or even only every second round)
 result in variable:
 currentJoy1Y = -1 down
 currentJoy1Y = 0 centered
 currentJoy1Y = 1 up

 currentJoy1X = -1 left
 currentJoy1X = 0 centered
 currentJoy1X = 1 right
 */
void v_readJoystick1Digital()
{
  if (isIRQMode) return;
#if RASPPI != 1 
  if (isSMPMode) return;
#endif

  if (ioDone & V_JOY_DIGITAL_READ) return;
  int tmp = irqJoyDigital;
  irqJoyDigital = 255;
  JOYSTICK_DIGITAL // defined in "handleDefs.i"
  irqJoyDigital = tmp;
}

// called from Vecx Only!!!
void v_readJoystick1Digital1(unsigned int x1_enabled,unsigned int y1_enabled,unsigned int x2_enabled,unsigned int y2_enabled)
{
  printf ("i:%i, s:%i, d:%i\n",isIRQMode, isSMPMode, (ioDone & V_BUTTONS_READ) );
  printf ("Enabled: -> x1:%i, y1:%i, x2:%i, y2:%i\n",x1_enabled, y1_enabled, x2_enabled, y2_enabled );

  if (isIRQMode) return;
#if RASPPI != 1 
  if (isSMPMode) return;
#endif  
  if (ioDone & V_JOY_ANALOG_READ) return;
  ioDone |= V_JOY_ANALOG_READ;
  
  printf ("Digital CALLED\n");


  currentJoy1X = 0;
  currentJoy1Y = 0;

  currentJoy2X = 0;
  currentJoy2Y = 0;

  
  
  if (swapPorts == 1)
  {
    int x1=x1_enabled;
    int y1=y1_enabled;
    if (x2_enabled)  x1_enabled = 1;
    if (y2_enabled)  y1_enabled = 3;
    if (x1==1) x2_enabled = 5;
    if (y1==3) y2_enabled = 7;
  }
  
  
    // if joyport 2  needed....
  if (y2_enabled == 7)
  {
    // Y ANALOG
    I_SET(VIA_port_a, 0x00); // clear VIA port A
    ADD_DELAY_CYCLES(4);
    I_SET(VIA_port_b, 0x82+0x04); // set VIA port B mux enabled, mux sel = 03 (vertical pot port 1)

    // wait for joystick comparators to "settle"
    ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

    currentJoy2Y = -1; // default down
    I_SET(VIA_port_b, 0x83); // set VIA port B mux
    DELAY_PORT_B_BEFORE_PORT_A();
    I_SET(VIA_port_a, 0x40); // load a with test value (positive y), test value to DAC
    ADD_DELAY_CYCLES(4);
    if ((I_GET(VIA_port_b) & 0x20) == 0x20)
    {
      currentJoy2Y = 1; //up
    }
    else
    {
      I_SET(VIA_port_a, -0x40); // load a with test value (negative y), test value to DAC
      ADD_DELAY_CYCLES(4);
      if ((I_GET(VIA_port_b) & 0x20) == 0x20)
      {
    currentJoy2Y = 0; // no direction
      }
    }
//    printf("Y2=%i | ", (signed char)currentJoy2Y);
  }

  if (y1_enabled == 3)
  {
    // Y ANALOG
    I_SET(VIA_port_a, 0x00); // clear VIA port A
    ADD_DELAY_CYCLES(4);
    I_SET(VIA_port_b, 0x82); // set VIA port B mux enabled, mux sel = 01 (vertical pot port 0)

    // wait for joystick comparators to "settle"
    ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

    currentJoy1Y = -1; // default down
    I_SET(VIA_port_b, 0x83); // set VIA port B mux
    DELAY_PORT_B_BEFORE_PORT_A();
    I_SET(VIA_port_a, 0x40); // load a with test value (positive y), test value to DAC
    ADD_DELAY_CYCLES(4);
    if ((I_GET(VIA_port_b) & 0x20) == 0x20)
    {
      currentJoy1Y = 1; //up
    }
    else
    {
      I_SET(VIA_port_a, -0x40); // load a with test value (negative y), test value to DAC
      ADD_DELAY_CYCLES(4);
      if ((I_GET(VIA_port_b) & 0x20) == 0x20)
      {
    currentJoy1Y = 0; // no direction
      }
    }
//    printf("Y1=%i | ", (signed char)currentJoy1Y);
  }

  if (x1_enabled == 1)
  {
    // X ANALOG
    I_SET(VIA_port_a, 0x00); // clear VIA port A
    ADD_DELAY_CYCLES(4);
    I_SET(VIA_port_b, 0x80); // set VIA port B mux enabled, mux sel = 00 (horizontal pot port 0)
    // wait for joystick comparators to "settle"
    ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

    currentJoy1X = -1; // default left
    I_SET(VIA_port_b, 0x83); // set VIA port B mux
    DELAY_PORT_B_BEFORE_PORT_A();
    I_SET(VIA_port_a, 0x40); // load a with test value (positive y), test value to DAC
    ADD_DELAY_CYCLES(2);
    if ((I_GET(VIA_port_b) & 0x20) == 0x20)
    {
      currentJoy1X = 1; //right
    }
    else
    {
      ADD_DELAY_CYCLES(4);
      I_SET(VIA_port_a, -0x40); // load a with test value (negative y), test value to DAC
      ADD_DELAY_CYCLES(4);
      if ((I_GET(VIA_port_b) & 0x20) == 0x20)
      {
    currentJoy1X = 0; // no direction
      }
      ADD_DELAY_CYCLES(4);
    }
//    printf("X1=%i | ", (signed char)currentJoy1X);
  }

  if (x2_enabled == 5)
  {
    // X ANALOG
    I_SET(VIA_port_a, 0x00); // clear VIA port A
    ADD_DELAY_CYCLES(4);
    I_SET(VIA_port_b, 0x84); // set VIA port B mux enabled, mux sel = 2 (horizontal pot port 1)
    // wait for joystick comparators to "settle"
    ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

    currentJoy2X = -1; // default left
    I_SET(VIA_port_b, 0x83); // set VIA port B mux
    DELAY_PORT_B_BEFORE_PORT_A();
    I_SET(VIA_port_a, 0x40); // load a with test value (positive y), test value to DAC
    ADD_DELAY_CYCLES(2);
    if ((I_GET(VIA_port_b) & 0x20) == 0x20)
    {
      currentJoy2X = 1; //right
    }
    else
    {
      ADD_DELAY_CYCLES(4);
      I_SET(VIA_port_a, -0x40); // load a with test value (negative y), test value to DAC
      ADD_DELAY_CYCLES(4);
      if ((I_GET(VIA_port_b) & 0x20) == 0x20)
      {
    currentJoy2X = 0; // no direction
      }
      ADD_DELAY_CYCLES(4);
    }
//    printf("X2=%i | ", (signed char)currentJoy2X);
  }
  if (swapPorts == 1)
  {
    int x1=currentJoy1Y;
    int y1=currentJoy1X;
    currentJoy1Y = currentJoy2Y;
    currentJoy1X = currentJoy2X;
    currentJoy2Y = x1;
    currentJoy2X = y1;
  }


//  printf("\n");
  
  // set port A reference value to unkown
  currentYSH = currentPortA=0x100; // reset saved current values to unkown state
}

/***********************************************************************/

/*      Reads the vectrex joystick states (port 1) ANALOG.
 ( Slower than digital call!)         *      *
 Needs only be called once per round (or even only every second round)
 result in variable:
 currentJoy1Y = -1 - -127 down
 currentJoy1Y = 0 centered
 currentJoy1Y = +1 - +127 up

 currentJoy1X = -1 - -127  left
 currentJoy1X = 0 centered
 currentJoy1X = +1 - +127 right
 */



void v_readJoystick1Analog()
{
  if (isIRQMode) return;
#if RASPPI != 1 
  if (isSMPMode) return;
#endif  
  if (ioDone & V_JOY_ANALOG_READ) return;
  
  int tmp = irqJoyAnalog;
  irqJoyAnalog = 255;
  JOYSTICK_ANALOG // defined in "handleDefs.i"
  irqJoyAnalog = tmp;
}

// called from Vecx Only!!!
void v_readJoystick1Analog1(unsigned int x1_enabled,unsigned int y1_enabled,unsigned int x2_enabled,unsigned int y2_enabled)
{
  //printf ("i:%i, s:%i, d:%i\n",isIRQMode, isSMPMode, (ioDone & V_BUTTONS_READ) );

  if (isIRQMode) return;
#if RASPPI != 1 
  if (isSMPMode) return;
#endif  
  if (ioDone & V_JOY_ANALOG_READ) return;
  ioDone |= V_JOY_ANALOG_READ;
  
//  printf ("ANALOG CALLED\n");
  
  currentJoy1X = 0;
  currentJoy1Y = 0;

  currentJoy2X = 0;
  currentJoy2Y = 0;
  
  if (swapPorts == 1)
  {
    int x1=x1_enabled;
    int y1=y1_enabled;
    if (x2_enabled)  x1_enabled = 1;
    if (y2_enabled)  y1_enabled = 3;
    if (x1==1) x2_enabled = 5;
    if (y1==3) y2_enabled = 7;
  }
  
  int8_t thisInternalJoy1Y = 0;
  uint8_t compareBit = 0x80;
  if (y1_enabled == 3)
  {
    I_SET(VIA_port_a, 0x00); // clear VIA port A
    ADD_DELAY_CYCLES(4);
    I_SET(VIA_port_b, 0x82); // set VIA port B mux enabled, mux sel = 01 (vertical pot port 0)
    // wait for joystick comparators to "settle"
    ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

    currentJoy1Y = 0; // default centered
    I_SET(VIA_port_b, 0x83); // set VIA port B mux disabled
    DELAY_PORT_B_BEFORE_PORT_A();

    do
    {
      I_SET(VIA_port_a, thisInternalJoy1Y); // load a with test value (positive y), test value to DAC
      ADD_DELAY_CYCLES(4);
      if ((I_GET(VIA_port_b) & 0x20) == 0)
      {
    thisInternalJoy1Y = thisInternalJoy1Y ^ compareBit;
      }
      ADD_DELAY_CYCLES(4);
      compareBit = compareBit>>1;
      thisInternalJoy1Y = thisInternalJoy1Y | compareBit;
    } while (compareBit!=0);  
    
  }

  int8_t thisInternalJoy1X = 0;
  if (x1_enabled == 1)
  {
    I_SET(VIA_port_a, 0x00); // clear VIA port A
    ADD_DELAY_CYCLES(4);
    I_SET(VIA_port_b, 0x80); // set VIA port B mux enabled, mux sel = 01 (horizontal pot port 0)
    // wait for joystick comparators to "settle"
    ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

    currentJoy1X = 0; // default centered
    I_SET(VIA_port_b, 0x81); // set VIA port B mux disabled
    DELAY_PORT_B_BEFORE_PORT_A();
    compareBit = 0x80;

    
    do
    {
      I_SET(VIA_port_a, thisInternalJoy1X); // load a with test value (positive y), test value to DAC
      ADD_DELAY_CYCLES(4);
      if ((I_GET(VIA_port_b) & 0x20) == 0)
      {
    thisInternalJoy1X = thisInternalJoy1X ^ compareBit;
      }
      ADD_DELAY_CYCLES(4);
      compareBit = compareBit>>1;
      thisInternalJoy1X = thisInternalJoy1X | compareBit;
    } while (compareBit!=0);
  }


  int difx = thisInternalJoy1X - internalJoy1X;
  int dify = thisInternalJoy1Y - internalJoy1Y;

  internalJoy1X = internalJoy1X+(difx>>1);
  internalJoy1Y = internalJoy1Y+(dify>>1);

  if (!inCalibration)
  {
    currentJoy1X = internalJoy1X;
    currentJoy1Y = internalJoy1Y;
  }

  // todo
  // if joyport 2  needed....
  int8_t thisInternalJoy2Y = 0;
  if (y2_enabled == 7)
  {
    I_SET(VIA_port_a, 0x00); // clear VIA port A
    ADD_DELAY_CYCLES(4);
    I_SET(VIA_port_b, 0x82+0x04); // set VIA port B mux enabled, mux sel = 03 (vertical pot port 1)
    // wait for joystick comparators to "settle"
    ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

    currentJoy2Y = 0; // default centered
    I_SET(VIA_port_b, 0x83); // set VIA port B mux disabled
    DELAY_PORT_B_BEFORE_PORT_A();
    compareBit = 0x80;

    do
    {
      I_SET(VIA_port_a, thisInternalJoy2Y); // load a with test value (positive y), test value to DAC
      ADD_DELAY_CYCLES(4);
      if ((I_GET(VIA_port_b) & 0x20) == 0)
      {
    thisInternalJoy2Y = thisInternalJoy2Y ^ compareBit;
      }
      ADD_DELAY_CYCLES(4);
      compareBit = compareBit>>1;
      thisInternalJoy2Y = thisInternalJoy2Y | compareBit;
    } while (compareBit!=0);  
    
  }
  int8_t thisInternalJoy2X = 0;
  if (x2_enabled == 5)
  {
    I_SET(VIA_port_a, 0x00); // clear VIA port A
    ADD_DELAY_CYCLES(4);
    I_SET(VIA_port_b, 0x84); // set VIA port B mux enabled, mux sel = 2 (horizontal pot port 1)
    // wait for joystick comparators to "settle"
    ADD_DELAY_CYCLES(60); // must be tested! can probably be less?

    currentJoy2X = 0; // default centered
    I_SET(VIA_port_b, 0x81); // set VIA port B mux disabled
    DELAY_PORT_B_BEFORE_PORT_A();
    compareBit = 0x80;

    do
    {
      I_SET(VIA_port_a, thisInternalJoy2X); // load a with test value (positive y), test value to DAC
      ADD_DELAY_CYCLES(4);
      if ((I_GET(VIA_port_b) & 0x20) == 0)
      {
    thisInternalJoy2X = thisInternalJoy2X ^ compareBit;
      }
      ADD_DELAY_CYCLES(4);
      compareBit = compareBit>>1;
      thisInternalJoy2X = thisInternalJoy2X | compareBit;
    } while (compareBit!=0);  
    
  }

  
  difx = thisInternalJoy2X - internalJoy2X;
  dify = thisInternalJoy2Y - internalJoy2Y;

  internalJoy2X = internalJoy2X+(difx>>1);
  internalJoy2Y = internalJoy2Y+(dify>>1);

  if (!inCalibration)
  {
    currentJoy2X = internalJoy2X;
    currentJoy2Y = internalJoy2Y;
  }
  

  
  
  
  if (invertYAxis) 
  {
    currentJoy1Y = ((currentJoy1Y==-128)?127:-currentJoy1Y);
    currentJoy2Y = ((currentJoy2Y==-128)?127:-currentJoy2Y);
  }
  if (swapPorts == 1)
  {
    int x1=currentJoy1Y;
    int y1=currentJoy1X;
    currentJoy1Y = currentJoy2Y;
    currentJoy1X = currentJoy2X;
    currentJoy2Y = x1;
    currentJoy2X = y1;

    x1=internalJoy1X;
    y1=internalJoy1Y;
    internalJoy1Y = internalJoy2Y;
    internalJoy1X = internalJoy2X;
    internalJoy2Y = x1;
    internalJoy2X = y1;
    
    
  }


  // set port A reference value to unkown
  currentYSH = currentPortA=0x100; // reset saved current values to unkown state
  v_resetIntegratorOffsets0();
}





void v_enableButtons(int yesNo)
{
  irqButtons = yesNo;
}
void v_enableJoystickDigital(int yesNoX1,int yesNoY1,int yesNoX2,int yesNoY2)
{
  if (irqJoyAnalog & yesNoX1) irqJoyAnalog-=yesNoX1;
  if (irqJoyAnalog & (yesNoY1<<1)) irqJoyAnalog-=(yesNoY1<<1);
  if (irqJoyAnalog & (yesNoX2<<2)) irqJoyAnalog-=(yesNoX2<<2);
  if (irqJoyAnalog & (yesNoY2<<3)) irqJoyAnalog-=(yesNoY2<<3);

  irqJoyDigital = yesNoX1+(yesNoY1<<1)+(yesNoX2<<2)+(yesNoY2<<3);
//  printf("Digitial Joystick Init: %02x\n", irqJoyDigital);
//  printf("Analog Joystick Init: %02x\n", irqJoyAnalog);
}
void v_enableJoystickAnalog(int yesNoX1,int yesNoY1,int yesNoX2,int yesNoY2)  // resets digital
{
  if (irqJoyDigital & yesNoX1) irqJoyDigital-=yesNoX1;
  if (irqJoyDigital & (yesNoY1<<1)) irqJoyDigital-=(yesNoY1<<1);
  if (irqJoyDigital & (yesNoX2<<2)) irqJoyDigital-=(yesNoX2<<2);
  if (irqJoyDigital & (yesNoY2<<3)) irqJoyDigital-=(yesNoY2<<3);
  
  irqJoyAnalog = yesNoX1+(yesNoY1<<1)+(yesNoX2<<2)+(yesNoY2<<3);
//  printf("Digitial Joystick Init: %02x\n", irqJoyDigital);
//  printf("Analog Joystick Init: %02x\n", irqJoyAnalog);
}