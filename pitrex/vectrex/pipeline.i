// in IRQMode in Hyperspace
// you MUST NOT changed variables used to calculate the pipeline, this includes
// currentScale, 
//extern int16_t currentPortA; // == portA (also X SH)
//extern int16_t currentYSH; // Y SH
//extern int16_t currentZSH; // Brightness



// pipeline section of IRQ handler
// ugly, fast - and ALL in one function, so it is
// all reachable via a "jump"


#undef USE_TIME

// a in arm cycles
#define USE_TIME(a) \
do{\
 MAIN_TAKEOVER(((a)/ONE_WAIT_UNIT)) \
} while(0)

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

{
  static int c;
  static unsigned int t1OffMark;
  static VectorPipeline *dpl;
  static int delayedBeamOff;
  static int lineNo;
  static int pCount;

  c = 0;
  t1OffMark = 0; // this is the value of counter0 as a MARK, where the T1 was finished!, in ARM cycles
  vectorPipeLineIsRead = vectorPipeLineNextRead;
  dpl = _VPL[vectorPipeLineIsRead];
  delayedBeamOff=0;
  lineNo = 0;
  pCount = 0;

  while (dpl[c].type != PL_END)
  {
// this might get rid of some "dots" during sample playing...
// not tryied it though,
// I am afraid samples will not play as well
// if (delayedBeamOff == 0)
{
    PLAY_SAMPLE();
}
    LINE_DEBUG_OUT("---- %i ----\r\n", lineNo);
    switch (dpl[c].type)
    {
      case PL_DEFLOK:
      {
        LINE_DEBUG_OUT("PL DEFLOK\r\n");
        // loop ensures that there are no pending draws/moves
        if (dpl[c].flags & PL_SWITCH_BEAM_OFF) SWITCH_BEAM_OFF();




////////////////////////////////////////////////////////////      
//v_deflok();


//  ZERO_AND_WAIT();
    ZERO_AND_CONTINUE(); 
    waitCounter1MarkLocal(DELAY_ZERO_VALUE);
    PLAY_SAMPLE_IGNORE_A();

    UNZERO();
//    v_setScale(255);
//  SET_YSH_IMMEDIATE_8(127);
//  SET_XSH_IMMEDIATE_8(127);
    I_SET(VIA_port_b, 0x80); 
    DELAY_PORT_B_BEFORE_PORT_A(); 
    I_SET_A(((uint8_t)(127))); 
    DELAY_YSH();

    I_SET(VIA_port_b, 0x81); 
    DELAY_XSH();
  
//  START_WAIT_T1();
//    START_T1_TIMER();

    SETW_inverse(VIA_t1, 255); setCounter1Mark(); 

    waitCounter1MarkLocal(255+DELAY_AFTER_T1_END_VALUE);
    PLAY_SAMPLE_IGNORE_A();

//  ZERO_AND_WAIT();
    ZERO_AND_CONTINUE(); 
    waitCounter1MarkLocal(DELAY_ZERO_VALUE);
    PLAY_SAMPLE_IGNORE_A();

    UNZERO();
//  SET_YSH_IMMEDIATE_8(-127);
//  SET_XSH_IMMEDIATE_8(-127);
    I_SET(VIA_port_b, 0x80); 
    DELAY_PORT_B_BEFORE_PORT_A(); 
    I_SET_A(((uint8_t)(-127))); 
    DELAY_YSH();
    I_SET(VIA_port_b, 0x81); 
    DELAY_XSH();
  
//  START_WAIT_T1();
//    START_T1_TIMER();
    SET(VIA_t1_cnt_hi, 0); setCounter1Mark(); 
    waitCounter1MarkLocal(255+DELAY_AFTER_T1_END_VALUE);
    PLAY_SAMPLE_IGNORE_A();

//  ZERO_AND_WAIT();
    ZERO_AND_CONTINUE(); 
    waitCounter1MarkLocal(DELAY_ZERO_VALUE);
    PLAY_SAMPLE_IGNORE_A();
////////////////////////////////////////////////////////////      



////////////////////////////////////////////////////////////
// v_resetIntegratorOffsets0();
  LINE_DEBUG_OUT("PL DEFLOK CALIB \r\n");
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
//  currentPortA=0x100;// non regular value!
  PLAY_SAMPLE_IGNORE_A();
////////////////////////////////////////////////////////////















        break;
      }
      case PL_ZERO:
      {
        // waitfor last draw??? -> was done at the end of last draw
        ZERO_AND_CONTINUE();
        LINE_DEBUG_OUT("PL ZERO: %i, %i\r\n", dpl[c+1].last_timing, dpl[c].this_timing);
        {
          static int timeDone;
          timeDone =24;
          // TODO enable calib again when zeroing!
////////////////////////////////////////////////////////////
// v_resetIntegratorOffsets0();
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
//  currentPortA=0x100;// non regular value!
  PLAY_SAMPLE_IGNORE_A();
////////////////////////////////////////////////////////////



          dpl[c+1].last_timing = (dpl[c].this_timing - timeDone);
          if (dpl[c+1].last_timing<0) dpl[c+1].last_timing=0;
        }
        break;
      }

      case PL_RASTER_TEXT:
      {
        LINE_DEBUG_OUT("PL RASTER DISPLAY AT x,y: %i,%i\r\n", dpl[c].x, dpl[c].y);
//        printf("\r\nPL RASTER DISPLAY AT x,y: %i,%i\r\n", dpl[c].x, dpl[c].y);

        // reverse "translation"
        static char* _string ;
        static uint8_t movementScale; //movementScale;
        static int8_t printScale;
        static int x;
        static int y;
        static int halfOffset; // halfOffset
        static int8_t xSize; // width of font
        static int8_t ySize; // height of font
        static int8_t yoffset;
        static int i;
        static unsigned char *currentRasterline;


// todo remove the statics
        _string = dpl[c].debug;
        movementScale =  dpl[c].yRest; //movementScale;
        x = dpl[c].x;
        y = dpl[c].y;
        halfOffset = dpl[c].pattern; // halfOffset
        xSize = dpl[c].x0; // width of font
        ySize = dpl[c].y0; // height of font
        yoffset = 0;
        i=0;
        currentRasterline = dpl[c].rlines[i++];

        while (currentRasterline!=0)
        {
            ZERO_AND_CONTINUE(); 
            //waitCounter1Mark(DELAY_ZERO_VALUE);
            waitCounter1MarkLocal(DELAY_ZERO_VALUE);
            PLAY_SAMPLE_IGNORE_A();

            static unsigned char* string;
            string = (unsigned char*)_string;
            UNZERO();

            // move to position
//            v_setScale(movementScale);
////////////////////////////////////////////////////////////////////
// v_moveToImmediate8(x, y);
    I_SET(VIA_port_b, 0x80); 
    DELAY_PORT_B_BEFORE_PORT_A(); 
    I_SET_A(((uint8_t)(y))); 
    DELAY_YSH();

    I_SET(VIA_port_b, 0x81); 
    DELAY_PORT_B_BEFORE_PORT_A(); 
    I_SET_A(((uint8_t)(x))); 
    DELAY_XSH();

    SETW_inverse(VIA_t1, movementScale); setCounter1Mark(); 

//    START_T1_TIMER();
    waitCounter1MarkLocal(movementScale+DELAY_AFTER_T1_END_VALUE);
    PLAY_SAMPLE_IGNORE_A();
////////////////////////////////////////////////////////////////////



            // move to line in String (only y-movement)
            if (yoffset!=0)
            {

              if (halfOffset)
              {
//v_setScale(10);
printScale= 10;
////////////////////////////////////////////////////////////////////
//SET_YSH_IMMEDIATE_8(yoffset<<1);
//SET_XSH_IMMEDIATE_8(0);

    I_SET(VIA_port_b, 0x80); 
    DELAY_PORT_B_BEFORE_PORT_A(); 
    I_SET_A(((uint8_t)((yoffset<<1)))); 
    DELAY_YSH();

    I_SET(VIA_port_b, 0x81); 
    DELAY_PORT_B_BEFORE_PORT_A(); 
    I_SET_A(0); 
    DELAY_XSH();
////////////////////////////////////////////////////////////////////

              }
              else
              {
//v_setScale(19);
printScale= 19;
////////////////////////////////////////////////////////////////////
    I_SET(VIA_port_b, 0x80); 
    DELAY_PORT_B_BEFORE_PORT_A(); 
    I_SET_A(((uint8_t)(yoffset))); 
    DELAY_YSH();

    I_SET(VIA_port_b, 0x81); 
    DELAY_PORT_B_BEFORE_PORT_A(); 
    I_SET_A(0); 
    DELAY_XSH();
////////////////////////////////////////////////////////////////////
              }
//START_T1_TIMER();
    SETW_inverse(VIA_t1, printScale); setCounter1Mark(); 
//WAIT_T1_END();
waitCounter1MarkLocal(printScale+DELAY_AFTER_T1_END_VALUE);


            }
            else
            {
              I_SET_A(0x00);
              DELAY_XSH();
            }
            #define RASTER_WAIT 17
            // disable ramp and set y moevemtn to 0
            I_SET(VIA_port_b, 0x80); // disable ramp, mux = y integrator, enable mux
            DELAY_YSH();
            I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
            DELAY_PORT_B_BEFORE_PORT_A();

            // set width of text 
            I_SET_A(xSize);
            DELAY_XSH();
            
            // prepare for raster output
            I_SET(VIA_aux_cntl, 0x18);

            I_SET(VIA_port_b, 0x01); // enable ramp, mux = y integrator, disable mux
            
            // the vectorbeam is moving... now fill the shiftregister ASAP with the bitmap, and wait for 18 cycles to pass
            // print this rasterline!
            static unsigned char charBitmap;
            do 
            {
                ADD_DELAY_CYCLES(RASTER_WAIT); // wait for enable ramp, and printing the last letter

                // draw one char bitmap
                charBitmap = currentRasterline[*string-0x20];
                string++;
                I_SET(VIA_shift_reg, charBitmap);
            } while (*string != 0);

            ADD_DELAY_CYCLES(6); // disabling ramp takes a certain amount of time! - reduce that from the original shift wait

            I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
            ADD_DELAY_CYCLES(4+4    +1);
            currentRasterline = dpl[c].rlines[i++];

            // enable non raster output
    #ifdef BEAM_LIGHT_BY_SHIFT
        I_SET (VIA_aux_cntl, 0x98); //Shift Reg. Enabled, T1 PB7 Enabled
    #endif
    #ifdef BEAM_LIGHT_BY_CNTL
        I_SET (VIA_aux_cntl, 0x80); // Shift reg mode = 000 free disable, T1 PB7 enabled 
    #endif
            ADD_DELAY_CYCLES(2);
            yoffset=yoffset+(ySize);
        }

        I_SET(VIA_shift_reg, 0);
        ZERO_AND_CONTINUE();
        PLAY_SAMPLE_IGNORE_A();
        break;
      }


















      case PL_RASTER_TEXT_BI_DIRECTIONAL:
      {
//printf("RasterPrint start \n\r");
        LINE_DEBUG_OUT("PL_RASTER_TEXT_BI_DIRECTIONAL DISPLAY AT x,y: %i,%i\r\n", dpl[c].x, dpl[c].y);
//        printf("\r\nPL RASTER DISPLAY AT x,y: %i,%i\r\n", dpl[c].x, dpl[c].y);

        // reverse "translation"
        static char* _string ;
        static unsigned char* string;
        static unsigned char charBitmap;
        static unsigned char charBitmapr;

        static uint8_t movementScale; //movementScale;
        static int x;
        static int y;
        static int8_t xSize; // width of font
        static int8_t ySize; // height of font
        static int i;
        static int ci;
        static int ci2;
        static unsigned char *currentRasterline;
        static int this_rasterDown;

        this_rasterDown = dpl[c].xRest;

        _string = &dpl[c].debug[0];
        string = (unsigned char*)_string;

        movementScale =  dpl[c].yRest; //movementScale;
        x = dpl[c].x;
        y = dpl[c].y;
        xSize = dpl[c].x0; // width of font
        ySize = dpl[c].y0; // height of font

        ZERO_AND_CONTINUE(); 
        waitCounter1MarkLocal(DELAY_ZERO_VALUE);
        PLAY_SAMPLE_IGNORE_A();
        UNZERO();

        // move to position
////////////////////////////////////////////////////////////////////
// v_moveToImmediate8(x, y);
    I_SET(VIA_port_b, 0x80); 
    DELAY_PORT_B_BEFORE_PORT_A(); 
    I_SET_A(((uint8_t)(y))); 
    DELAY_YSH();

    I_SET(VIA_port_b, 0x81); 
    DELAY_PORT_B_BEFORE_PORT_A(); 
    I_SET_A(((uint8_t)(x))); 
    DELAY_XSH();

    SETW_inverse(VIA_t1, movementScale); 
    setCounter1Mark(); 
    waitCounter1MarkLocal(movementScale+DELAY_AFTER_T1_END_VALUE);
    PLAY_SAMPLE_IGNORE_A();
////////////////////////////////////////////////////////////////////
    // prepare for raster output
    I_SET(VIA_aux_cntl, 0x18);

////////////////////////////////////////////////////////////
// clear calibration / offsets
  I_SET (VIA_port_b, 0x81); 
  DELAY_PORT_B_BEFORE_PORT_A(); 
  I_SET_A(0x00); 
  ADD_DELAY_CYCLES(4); 
  I_SET (VIA_port_b, 0x80); 
  ADD_DELAY_CYCLES(6); 
  I_SET (VIA_port_b, 0x82); 
  ADD_DELAY_CYCLES(6); 
  I_SET (VIA_port_b, 0x81); 
  ADD_DELAY_CYCLES(4);
////////////////////////////////////////////////////////////

        i=0;
        currentRasterline = dpl[c].rlines[i++];
        while (currentRasterline!=0)
        {

          #define RASTER_WAIT 17
          // disable ramp and set y moevemtn to 0
          I_SET(VIA_port_b, 0x80); // disable ramp, mux = y integrator, enable mux
    DELAY_PORT_B_BEFORE_PORT_A(); 
    I_SET_A(0); 
          DELAY_YSH();
          I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
          DELAY_PORT_B_BEFORE_PORT_A();

          // set width of text 
          I_SET_A(xSize);
          DELAY_XSH();
    DELAY_XSH();
          
          I_SET(VIA_port_b, 0x01); // enable ramp, mux = y integrator, disable mux
          
          // the vectorbeam is moving... now fill the shiftregister ASAP with the bitmap, and wait for 18 cycles to pass
          // print this rasterline!
          do 
          {
              ADD_DELAY_CYCLES(RASTER_WAIT); // wait for enable ramp, and printing the last letter

              // draw one char bitmap
              charBitmap = currentRasterline[*string-0x20];
              string++;
              I_SET(VIA_shift_reg, charBitmap);
          } while (*string != 0);

          ADD_DELAY_CYCLES(6); // disabling ramp takes a certain amount of time! - reduce that from the original shift wait

ADD_DELAY_CYCLES(biRasterReturnDelayLeft);

          I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
          ADD_DELAY_CYCLES(4+4    +1);
          currentRasterline = dpl[c].rlines[i++];

          if (currentRasterline!=0)
          {
        // move down 
        ////////////////////////////////////////////////////////////////////
            I_SET(VIA_port_b, 0x80); 
            DELAY_PORT_B_BEFORE_PORT_A(); 
            I_SET_A(((uint8_t)(ySize))); 
            DELAY_YSH();

            I_SET(VIA_port_b, 0x81); 
            DELAY_PORT_B_BEFORE_PORT_A(); 
            I_SET_A(0); 
            DELAY_XSH();

          I_SET(VIA_port_b, 0x01); // enable ramp, mux = y integrator, disable mux
          ADD_DELAY_CYCLES(this_rasterDown);
          I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux

            PLAY_SAMPLE();
        ////////////////////////////////////////////////////////////////////
        // and print reverse raster



            // disable ramp and set y moevemtn to 0
            I_SET(VIA_port_b, 0x80); // disable ramp, mux = y integrator, enable mux
            DELAY_YSH();
            I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
            DELAY_PORT_B_BEFORE_PORT_A();
            // set width of text 
            I_SET_A(-xSize); // moving backward
            DELAY_XSH();
    DELAY_XSH();
            
            I_SET(VIA_port_b, 0x01); // enable ramp, mux = y integrator, disable mux
            
            // the vectorbeam is moving... now fill the shiftregister ASAP with the bitmap, and wait for 18 cycles to pass
            // print this rasterline!
            do 
            {
                string--;

                // draw one char bitmap
                charBitmap = currentRasterline[*string-0x20];
                charBitmapr = 0;
                for(ci=0;ci<8;ci++)
                {
//                  charBitmapr = charBitmapr << 1; // actually placed here is "more" correct. But leaving the last bit "forced" 0, ensures there is no light left "on"
                                                    // when a letters bitmap actually "starts" with a set bit
                                                    // this means we actually lose the last bit, but in any "good" charset, the last bit is also always 0, for the same reasons!
                                                    // thus the "back" bitmap is actually one bit shifted to the right"
                                                    // we must compensate that with the "biRasterReturnDelay" value
                  if (charBitmap & 1) charBitmapr +=1;
                  charBitmapr = charBitmapr << 1;
                  charBitmap = charBitmap >> 1;
                }

                ADD_DELAY_CYCLES(RASTER_WAIT); // wait for enable ramp, and printing the last letter
                I_SET(VIA_shift_reg, charBitmapr);
            } while ((char*)string != _string);

            ADD_DELAY_CYCLES(6); // disabling ramp takes a certain amount of time! - reduce that from the original shift wait
ADD_DELAY_CYCLES(biRasterReturnDelayRight);

            I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
            ADD_DELAY_CYCLES(4+4    +1);
            currentRasterline = dpl[c].rlines[i++];
            if (currentRasterline != 0)
            {
              // move down 
              ////////////////////////////////////////////////////////////////////
                  I_SET(VIA_port_b, 0x80); 
                  DELAY_PORT_B_BEFORE_PORT_A(); 
                  I_SET_A(((uint8_t)(ySize))); 
                  DELAY_YSH();

                  I_SET(VIA_port_b, 0x81); 
                  DELAY_PORT_B_BEFORE_PORT_A(); 
                  I_SET_A(0); 
                  DELAY_XSH();

                  I_SET(VIA_port_b, 0x01); // enable ramp, mux = y integrator, disable mux
    ADD_DELAY_CYCLES(this_rasterDown);
                  I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
                  PLAY_SAMPLE();
            }
          }
        }

          // enable non raster output
      #ifdef BEAM_LIGHT_BY_SHIFT
          I_SET (VIA_aux_cntl, 0x98); //Shift Reg. Enabled, T1 PB7 Enabled
      #endif
      #ifdef BEAM_LIGHT_BY_CNTL
          I_SET (VIA_aux_cntl, 0x80); // Shift reg mode = 000 free disable, T1 PB7 enabled 
      #endif
          ADD_DELAY_CYCLES(2);

        I_SET(VIA_shift_reg, 0);
        ZERO_AND_CONTINUE();
        PLAY_SAMPLE_IGNORE_A();
        break;
      }






























      case PL_SET_BRIGHTNESS:
      {
        LINE_DEBUG_OUT("PL Brightness  A = %x\r\n", dpl[c].intensity);
        LINE_DEBUG_OUT("     IMUX MUST BE SET\r\n");
        I_SET(VIA_port_b, 0x84); // MUX to intensity

        if (dpl[c].flags & PL_I_A_MUST_BE_SET)
        {
          LINE_DEBUG_OUT("     IA MUST BE SET\r\n");
          DELAY_PORT_B_BEFORE_PORT_A_BRIGHTNESS(); 
          I_SET_A(dpl[c].intensity); 
        }
        DELAY_ZSH();

        if (crankyFlag & CRANKY_BETWEEN_VIA_B)
        {
          // TODO
          I_SET(VIA_port_b, 0x81); // MUX to intensity
          DELAY_PORT_B_BEFORE_PORT_A(); 
          I_SET_A(0); 
          I_SET(VIA_port_b, 0x81); // MUX to intensity
          DELAY_PORT_B_BEFORE_PORT_A(); 
          I_SET_A(0); 
        }

        break;
      }
      case PL_DRAW_DOT:
      {
        LINE_DEBUG_OUT ("PL DOT  %i, %i :%i\r\n    %s, time %i\r\n", dpl[c].x*dpl[c].this_timing, dpl[c].y*dpl[c].this_timing, dpl[c].this_timing, dpl[c].debug, dpl[c].this_timing);
        if (dpl[c].flags & PL_SWITCH_BEAM_ON)
        {
          SWITCH_BEAM_ON();
        }
        break;
      }
      case PL_MOVE:
      {
        LINE_DEBUG_OUT("PL MOVE: xStart: %i, x:%i x*s:%i, yStart: %i, y:%i y*s:%i scale:%i\r\n", dpl[c].xStart, dpl[c].x,dpl[c].x*dpl[c].this_timing, dpl[c].yStart, dpl[c].y, dpl[c].y*dpl[c].this_timing, dpl[c].this_timing);
        LINE_DEBUG_OUT("PL MOVE: X0: %i, Y0: %i\r\n",dpl[c].x0,dpl[c].y0);
        LINE_DEBUG_OUT("PL MOVE: Restx(%i), Resty(%i)\r\n",dpl[c].xRest,dpl[c].yRest);
      }
      case PL_DRAW_PATTERN:
      {
        if (dpl[c].type ==PL_DRAW_PATTERN)
          LINE_DEBUG_OUT ("PL DRAW PATTERN \r\n    %s\r\n", dpl[c].debug);
      }
      case PL_DRAW:
      {
      
        // todo if consecutive draws
        // handle them in one loop
        // and draw them in one go!      
        // would that be faster???        
        if (dpl[c].type ==PL_DRAW)
        {
          LINE_DEBUG_OUT("PL DRAW: xStart: %i, x:%i x*s:%i, yStart: %i, y:%i y*s:%i -> xEnd: %i, yEnd: %i scale:%i\r\n",dpl[c].xStart, dpl[c].x,dpl[c].x*dpl[c].this_timing, dpl[c].yStart ,dpl[c].y, dpl[c].y*dpl[c].this_timing,dpl[c].xEnd,dpl[c].yEnd, dpl[c].this_timing);
          LINE_DEBUG_OUT("PL DRAW: X0: %i, Y0: %i\r\n",dpl[c].x0,dpl[c].y0);
          LINE_DEBUG_OUT("PL DRAW: %s", dpl[c].debug);
        }

        if (dpl[c].flags & PL_CALIBRATE_0)
        {
          // TODO
          LINE_DEBUG_OUT ("    PL D CALIB 0\r\n");
////////////////////////////////////////////////////////////
	// v_resetIntegratorOffsets0();
//printf("CALIBRATION 0\r\n");
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
	//  currentPortA=0x100;// non regular value!
	  PLAY_SAMPLE_IGNORE_A();
////////////////////////////////////////////////////////////
        }
        else if (dpl[c].flags & PL_CALIBRATE)
        {
          LINE_DEBUG_OUT ("    PL D CALIB \r\n");
//////////////////////////////////////////////////////////////////////////
	  //v_resetIntegratorOffsets();
//printf("CALIBRATION x\r\n");
	  {
	    I_SET (VIA_port_b, 0x81);
	    DELAY_PORT_B_BEFORE_PORT_A();

	    if (calibrationValue==0)
	    {
	      I_SET_A(0x00);
	      ADD_DELAY_CYCLES(4);
	      // reset integrators
	      I_SET (VIA_port_b, 0x82);    // mux=1, enable mux - integrator offset = 0
	      ADD_DELAY_CYCLES(6);
	      I_SET (VIA_port_b, 0x81);    // disable mux
	    }
	    else
	    {

	      I_SET_A(0);
	      ADD_DELAY_CYCLES(6);
	      I_SET (VIA_port_b, 0x82);
	      ADD_DELAY_CYCLES(6);
	      I_SET (VIA_port_b, 0x81);    // disable mux
	      ADD_DELAY_CYCLES(6);
	      
	      
	      I_SET_A(calibrationValue);
	      ADD_DELAY_CYCLES(6);
	      I_SET (VIA_port_b, 0x82);
	      DELAY_PORT_B_BEFORE_PORT_A();

	      I_SET_A(0xff);

	      ADD_DELAY_CYCLES(2);

	  //to test CALIB!!!
	      waitUntilAndUse(setWaitMin*ONE_WAIT_UNIT - 205);
	      vectrexwrite(VIA_port_b, 0x81); // don't ask wghy twice... tempest looks better in parts... and it is just 2 cycls!!!
	      vectrexwrite(VIA_port_b, 0x81);
	      PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET);

	      lastViaWrite = (0);
	      setWaitMin = 0;
	    }
	    ADD_DELAY_CYCLES(4);
	  //  currentPortA=0x100;// non regular value!
	    PLAY_SAMPLE_IGNORE_A();
	  }
//////////////////////////////////////////////////////////////////////////
          if (calibrationValue !=0)
          {
            // TODO Costs time :-()
            // isn't that too much???
            I_SET_A(0x00);
            DELAY_YSH();
            I_SET(VIA_port_b, 0x80);
            DELAY_YSH();
            I_SET(VIA_port_b, 0x81);
            DELAY_PORT_B_BEFORE_PORT_A();
          }
        }
        
        if (dpl[c].flags & PL_Y_MUST_BE_SET)
        {
          LINE_DEBUG_OUT("     Y MUST BE I_SET (%i)\r\n", dpl[c].y);
          if (dpl[c].flags & PL_Y_A_MUST_BE_SET)
          {
            LINE_DEBUG_OUT("     YA MUST BE I_SET\r\n");
            if (dpl[c].flags & PL_MUX_Y_MUST_BE_SET)
            {
              LINE_DEBUG_OUT("     YMUX MUST BE I_SET\r\n");
              I_SET(VIA_port_b, 0x80);
              DELAY_PORT_B_BEFORE_PORT_A();
            }
            //what was set before? -> waiting for T1 timer
            I_SET_A(dpl[c].y);

            if (dpl[c].flags & PL_Y_DELAY_TO_NULL)
            {
                LINE_DEBUG_OUT("     PL_Y_DELAY_TO_NULL\r\n");
                if (crankyFlag & CRANKY_NULLING_WAIT)
                {
                    LINE_DEBUG_OUT("     CRANKY_NULLING_WAIT\r\n");
                    // some crankies need additional waits here!
                    ADD_DELAY_CYCLES(CRANKY_DELAY_Y_TO_NULL_VALUE);
                }
            }
          }
          else
          {
            if (dpl[c].flags & PL_MUX_Y_MUST_BE_SET)
            {
              //what was set before? -> waiting for T1 timer
              I_SET(VIA_port_b, 0x80); // MUX to y integrator
            }
          }
          DELAY_YSH();
        }
        else
          LINE_DEBUG_OUT("     Y NEED NOT BE I_SET\r\n");

        
        // for the beam to be switched off from last drawing we have
        // to wait alltogether for "DELAY_AFTER_T1_END_VALUE" cycles
        if (delayedBeamOff)
        {
          waitT1OffMark(DELAY_AFTER_T1_END_VALUE);
          SWITCH_BEAM_OFF();
        }
        else
        {
          if (dpl[c].flags & PL_DEACTIVATE_ZERO)
          {
              // attention!
              // UNZERO is also a BEAM_OFF!
              UNZERO();
          }
        }

        // not checking - since we also do ramp with B
        // if (dpl[c].flags & PL_MUX_X_MUST_BE_SET;
        I_SET(VIA_port_b, 0x81);
        if (dpl[c].flags & PL_X_A_MUST_BE_SET)
        {
          // to test only if cranky?dpl[c].x
          DELAY_PORT_B_BEFORE_PORT_A();
          I_SET_A(dpl[c].x);
          DELAY_XSH(); 
        }
        // start T1 timer
        if (dpl[c].flags & PL_T1_LO_EQUALS)
        {
          SET(VIA_t1_cnt_hi, (dpl[c].this_timing)>>8);
          setCounter1Mark(); 
        }
        else
        {
          SETW_inverse(VIA_t1, dpl[c].this_timing);  /* scale due to "enlargement" is 16 bit! */
          setCounter1Mark(); 
        }

        if (dpl[c].type ==PL_DRAW)
        {
            LINE_DEBUG_OUT ("     x:%i, y:%i scale:%i \r\n", dpl[c].x*dpl[c].this_timing, dpl[c].y*dpl[c].this_timing, dpl[c].this_timing);
            if (dpl[c].flags & PL_SWITCH_BEAM_ON)
            {
              LINE_DEBUG_OUT ("     beam switched on \r\n");
              SWITCH_BEAM_ON();
            }
        }
        else if (dpl[c].type == PL_DRAW_PATTERN)// this must be  (dpl[c].type == PL_DRAW_PATTERN))
        {
            LINE_DEBUG_OUT ("     %i, %i :%i - $%02x\r\n", dpl[c].x*dpl[c].this_timing, dpl[c].y*dpl[c].this_timing, dpl[c].this_timing, dpl[c].pattern);
            static int patternAnds[] = {128,64,32,16,8,4,2,1};
            pCount = 0;
            while ((GET(VIA_int_flags) & 0x40) == 0)
            {
#ifdef BEAM_LIGHT_BY_CNTL
                if (dpl[c].pattern & patternAnds[pCount])
                    SWITCH_BEAM_ON();
                else
                    SWITCH_BEAM_OFF();
                pCount=pCount+1;
                if (pCount==8) pCount=0;
#endif
#ifdef BEAM_LIGHT_BY_SHIFT
                if (pCount==0)
                {
                    SET_SHIFT_REG(dpl[c].pattern);
                    pCount=18;
                }
                else
                    pCount-=2;
#endif
            }
            static int delayT1;
            delayT1 = DELAY_AFTER_T1_END_VALUE;
            while (delayT1>0)
            {
#ifdef BEAM_LIGHT_BY_CNTL
                if (dpl[c].pattern & patternAnds[pCount])
                    SWITCH_BEAM_ON();
                else
                    SWITCH_BEAM_OFF();
                pCount=pCount+1;
                if (pCount==8) pCount=0;
#endif
#ifdef BEAM_LIGHT_BY_SHIFT
                if (pCount==0)
                {
                    SET_SHIFT_REG(dpl[c].pattern);
                    pCount=18;
                }
                else
                    pCount-=2;
#endif
                delayT1-=2;
            }
            SWITCH_BEAM_OFF();
        }
        break;
      }
      default:
        break;
    }
    c++;
    delayedBeamOff = 0;
    CCNT0(t1OffMark); // reset T1 "off" marker
    
    if (dpl[c].flags & PL_LAST_MUST_FINISH )
    {
      if (dpl[c].flags & PL_LAST_IS_RAMPING)
      {


//free T1 time for client
//    waitCounter1MarkLocal(dpl[c-1].this_timing);


/* it seems as soon as we do ANY
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

  static unsigned int value;
  static int v1;
  CCNT0(value);
  if (value < (piCounterMark+((dpl[c-1].this_timing)*ONE_WAIT_UNIT) ) )
  {
    USE_TIME( (piCounterMark+((dpl[c-1].this_timing)*ONE_WAIT_UNIT) ) - value);
  }

#if RASPPI != 1 
// In PiZero2 T1 interrupt flag is sometimes not set/read
// and the following line than is an endless loop!

  static unsigned volatile int preventer=0;
  preventer = 0;
// 2 cycles = 1 GET
// lets say the lengthiest vector is 512
// that would mean 512 /2 ? 256 max
  
  while (((GET(VIA_int_flags) & 0x40) == 0) && (preventer++ <256));
  if (preventer>=256) printf("TIMER T1 FAIL!!!\n");
#else
  while ((GET(VIA_int_flags) & 0x40) == 0);
#endif





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

    if ((dpl[c].flags & PL_SWITCH_BEAM_OFF)/* && (dpl[c].type != PL_ZERO)*/)
    {
      // last was with light on, now switch it off
      // or at least prepare switch off
      if (((dpl[c].type == PL_MOVE) || (dpl[c].type == PL_DRAW)|| (dpl[c].type == PL_DRAW_PATTERN) ) && (dpl[c-1].type != PL_DRAW_DOT)) 
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
        waitT1OffMark(DELAY_AFTER_T1_END_VALUE+5);
        LINE_DEBUG_OUT ("Beam off: \n\r");
        
        
        SWITCH_BEAM_OFF();
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
//  currentPortA=0x100;// non regular value!
////////////////////////////////////////////////////////////
//          ZERO_AND_WAIT();

    ZERO_AND_CONTINUE(); 
    //waitCounter1Mark(DELAY_ZERO_VALUE);
    waitT1OffMark(DELAY_ZERO_VALUE);
    PLAY_SAMPLE_IGNORE_A();


        }        
      }
    }
  }

  // savety only
  SWITCH_BEAM_OFF();
  ZERO_AND_CONTINUE();
    
//printf("D_END\r\n");
}
