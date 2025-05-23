void displayPipeline()
{
  int c = 0;
  unsigned int t1OffMark = 0;
  vectorPipeLineIsRead = vectorPipeLineNextRead;
  VectorPipeline *dpl = _VPL[vectorPipeLineIsRead];
  int delayedBeamOff=0;
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



  int lineNo = 0;

  while (dpl[c].type != PL_END)
  {
    LINE_DEBUG_OUT("---- %i ----\r\n", lineNo);
#ifdef PITREX_DEBUG
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
    switch (dpl[c].type)
    {
      case PL_DEFLOK:
      {
        LINE_DEBUG_OUT("PL DEFLOK\r\n");
        // loop ensures that there are no pending draws/moves
        if (dpl[c].flags & PL_SWITCH_BEAM_OFF) SWITCH_BEAM_OFF();
        v_deflok();
        LINE_DEBUG_OUT("PL DEFLOK CALIB \r\n");
        v_resetIntegratorOffsets0();
        break;
      }
      case PL_ZERO:
      {
        // waitfor last draw??? -> was done at the end of last draw
        ZERO_AND_CONTINUE();
        LINE_DEBUG_OUT("PL ZERO: %i, %i\r\n", dpl[c+1].last_timing, dpl[c].this_timing);
        {
          int timeDone =24;
          // TODO enable calib again when zeroing!
          v_resetIntegratorOffsets0();
          
          dpl[c+1].last_timing = (dpl[c].this_timing - timeDone);
          if (dpl[c+1].last_timing<0) dpl[c+1].last_timing=0;
        }
        break;
      }
      
      case PL_RASTER_TEXT:
      {
        LINE_DEBUG_OUT("PL RASTER DISPLAY AT x,y: %i,%i\r\n", dpl[c].x, dpl[c].y);

        // reverse "translation"
        char* _string = dpl[c].debug;
        int8_t movementScale =  dpl[c].yRest; //movementScale;
        int x = dpl[c].x;
        int y = dpl[c].y;
        int halfOffset = dpl[c].pattern; // halfOffset
        int8_t xSize = dpl[c].x0; // width of font
        int8_t ySize = dpl[c].y0; // height of font

        int8_t yoffset = 0;
        
        int i=0;
        unsigned char *currentRasterline = dpl[c].rlines[i++];
        while (currentRasterline!=0)
        {
            ZERO_AND_WAIT();
            unsigned char* string = (unsigned char*)_string;
            UNZERO();

            // move to position
            v_setScale(movementScale);
            v_moveToImmediate8(x, y);

            // move to line in String (only y-movement)
            if (yoffset!=0)
            {
              if (halfOffset)
              {
                v_setScale(10);
                SET_YSH_IMMEDIATE_8(yoffset<<1);
                SET_XSH_IMMEDIATE_8(0);
              }
              else
              {
                v_setScale(19);
                SET_YSH_IMMEDIATE_8(yoffset);
                SET_XSH_IMMEDIATE_8(0);
              }
              START_T1_TIMER();
              WAIT_T1_END();
            }
            else
            {
              I_SET(VIA_port_a, 0x00);
              DELAY_XSH();
            }
    #define RASTER_WAIT 17
            // disable ramp and set y moevemtn to 0
            I_SET(VIA_port_b, 0x80); // disable ramp, mux = y integrator, enable mux
            DELAY_YSH();
            I_SET(VIA_port_b, 0x81); // disable ramp, mux = y integrator, disable mux
            DELAY_PORT_B_BEFORE_PORT_A();

            // set width of text 
            I_SET(VIA_port_a, xSize);
            DELAY_XSH();
            
            // prepare for raster output
            I_SET(VIA_aux_cntl, 0x18);

            I_SET(VIA_port_b, 0x01); // enable ramp, mux = y integrator, disable mux
            
            // the vectorbeam is moving... now fill the shiftregister ASAP with the bitmap, and wait for 18 cycles to pass

            // print this rasterline!
            do 
            {
                ADD_DELAY_CYCLES(RASTER_WAIT); // wait for enable ramp, and printing the last letter

                // draw one char bitmap
                unsigned char charBitmap = currentRasterline[*string-0x20];
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
        break;
      }
      
      
      case PL_SET_BRIGHTNESS:
      {
        LINE_DEBUG_OUT("PL Brightness  A = %x\r\n", dpl[c].intensity);
#ifdef PITREX_DEBUG
        if (browseMode) break;
#endif
        LINE_DEBUG_OUT("     IMUX MUST BE SET\r\n");
        I_SET(VIA_port_b, 0x84); // MUX to intensity

        if (dpl[c].flags & PL_I_A_MUST_BE_SET)
        {
          LINE_DEBUG_OUT("     IA MUST BE SET\r\n");
          DELAY_PORT_B_BEFORE_PORT_A_BRIGHTNESS(); 
          I_SET(VIA_port_a, dpl[c].intensity); 
        }
        DELAY_ZSH();

        if (crankyFlag & CRANKY_BETWEEN_VIA_B)
        {
          // TODO
          
          I_SET(VIA_port_b, 0x81); // MUX to intensity
          DELAY_PORT_B_BEFORE_PORT_A(); 
          I_SET(VIA_port_a, 0); 

          I_SET(VIA_port_b, 0x81); // MUX to intensity
          DELAY_PORT_B_BEFORE_PORT_A(); 
          I_SET(VIA_port_a, 0); 
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
          v_resetIntegratorOffsets0();
        }
        else if (dpl[c].flags & PL_CALIBRATE)
        {
          LINE_DEBUG_OUT ("    PL D CALIB \r\n");
          v_resetIntegratorOffsets();
        }
      
        if (dpl[c].flags & PL_Y_MUST_BE_SET)
        {
          LINE_DEBUG_OUT("     Y MUST BE I_SET (%i)\r\n", dpl[c].y);
          if (dpl[c].flags & PL_Y_A_MUST_BE_SET)
          {
            LINE_DEBUG_OUT("     YA MUST BE I_SET\r\n");

            //what was set before? -> waiting for T1 timer
            I_SET(VIA_port_a, dpl[c].y);

            if ((dpl[c].y == 0) && (dpl[c].flags & PL_Y_DELAY_TO_NULL))
            {
                LINE_DEBUG_OUT("     CRANKY_NULLING_WAIT\r\n");
                LINE_DEBUG_OUT("     PL_Y_DELAY_TO_NULL\r\n");
                // some crankies need additional waits here!
                ADD_DELAY_CYCLES(CRANKY_DELAY_Y_TO_NULL_VALUE);
            }
          }
          
          
          
          if (dpl[c].flags & PL_MUX_Y_MUST_BE_SET)
          {
            //what was set before? -> waiting for T1 timer
            I_SET(VIA_port_b, 0x80); // MUX to y integrator
ADD_DELAY_CYCLES(4); // wait for enable ramp, and printing the last letter
          }
          DELAY_YSH();
        }
        else
          LINE_DEBUG_OUT("     Y NEED NOT BE I_SET\r\n");


// not checking - since we also do ramp with B
        // if (dpl[c].flags & PL_MUX_X_MUST_BE_SET;
        I_SET(VIA_port_b, 0x81);

        
       int usedDelays = 0;
        // for the beam to be switched off from last drawing we have
        // to wait alltogether for "DELAY_AFTER_T1_END_VALUE" cycles
        if (delayedBeamOff)
        {
          LINE_DEBUG_OUT("     DELAYED BEAM OFF\r\n");
          int d = DELAY_AFTER_T1_END_VALUE;
          d -= 12;
          if (d>0)
          waitT1OffMark(d);
          SWITCH_BEAM_OFF4();
          if (d>0) 
            usedDelays = d;
          usedDelays += 2;
        }
        else
        {
          if (dpl[c].flags & PL_DEACTIVATE_ZERO)
          {
              // attention!
              // UNZERO is also a BEAM_OFF!
              LINE_DEBUG_OUT("     UNZERO\r\n");
              UNZERO();
              usedDelays += 2;
          }
        }
if (8-usedDelays>0)
  ADD_DELAY_CYCLES(8-usedDelays);
        
        
        
        
//        next test should be ramp enable by VIA B
        
        

        if (dpl[c].flags & PL_X_A_MUST_BE_SET)
        {
          LINE_DEBUG_OUT("     PL_X_A_MUST_BE_SET %i\r\n",dpl[c].x);
          // to test only if cranky?dpl[c].x
          DELAY_PORT_B_BEFORE_PORT_A();
          I_SET(VIA_port_a, dpl[c].x);
ADD_DELAY_CYCLES(8); // wait for enable ramp, and printing the last letter

          if (dpl[c].type !=PL_DRAW) DELAY_XSH(); 
        }

        if (dpl[c].type ==PL_DRAW)
        {
            LINE_DEBUG_OUT ("     x:%i, y:%i scale:%i \r\n", dpl[c].x*dpl[c].this_timing, dpl[c].y*dpl[c].this_timing, dpl[c].this_timing);
            if (dpl[c].flags & PL_SWITCH_BEAM_ON)
            {
              LINE_DEBUG_OUT ("     beam switched on \r\n");
//              SWITCH_BEAM_ON();
              SWITCH_BEAM_ON4();
            }
        }
        
        
        
        // start T1 timer
        if (dpl[c].flags & PL_T1_LO_EQUALS)
        {
          SET(VIA_t1_cnt_hi, (dpl[c].this_timing)>>8);
        }
        else
        {
          SETW_inverse(VIA_t1, dpl[c].this_timing);  // scale due to "enlargement" is 16 bit! 
        }
        
        

        if (dpl[c].type == PL_DRAW_PATTERN)// this must be  (dpl[c].type == PL_DRAW_PATTERN))
        {
  
          
          LINE_DEBUG_OUT ("     %i, %i :%i - $%02x\r\n", dpl[c].x*dpl[c].this_timing, dpl[c].y*dpl[c].this_timing, dpl[c].this_timing, dpl[c].pattern);
            int patternAnds[] = {128,64,32,16,8,4,2,1};
            int pCount = 0;
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
            int delayT1 = DELAY_AFTER_T1_END_VALUE;
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
#ifdef PITREX_DEBUG
            if ((browseMode) && (dpl[c].type == PL_DRAW_PATTERN))
            {
              if (lineNo==currentBrowsline)
                currentDisplayedBrowseLine = currentBrowsline;
              lineNo++;
            }
#endif            
        }
        break;
      }
      default:
        break;
    }
    c++;
#ifdef MY_DEBUG
    if (myDebug)   printf("handled %i display elements...!\r\n", c);
#endif
    delayedBeamOff = 0;
    CCNT0(t1OffMark); // reset T1 "off" marker
    


    
    if (dpl[c].flags & PL_LAST_MUST_FINISH )
    {
// better!
// waitUntilAndUse(50*ONE_WAIT_UNIT);
      if (dpl[c].flags & PL_LAST_IS_RAMPING)
      {
//working?

        while ((GET(VIA_int_flags) & 0x40) == 0);
        // set T1 off marker
        CCNT0(t1OffMark);
        
// better!
// waitUntilAndUse(50*ONE_WAIT_UNIT);
        
// not better if if commented out!        
        if ((dpl[c-1].type == PL_MOVE) && (dpl[c].type == PL_DRAW_DOT)) // same as !((dpl[c].flags & PL_SWITCH_BEAM_OFF)
        {
          // only needed for dots?
          // only needed, when (dpl[c].flags & PL_SWITCH_BEAM_ON)?
// not better          
//s waitUntilAndUse(50*ONE_WAIT_UNIT);
          waitT1OffMark(DELAY_AFTER_T1_END_VALUE);
        }
      }
      else
      {
        // e.g. zeroing
        // dots
        // RASTER
        LINE_DEBUG_OUT ("Dot/Zero/Raster delay: %i\n\r", dpl[c].last_timing);
        ADD_DELAY_CYCLES(dpl[c].last_timing);
      }
#ifdef PITREX_DEBUG
      if ((browseMode) && ((dpl[c-1].type ==PL_DRAW)||(dpl[c-1].type ==PL_DRAW_DOT)||(dpl[c-1].type ==PL_DRAW_PATTERN)   ))
      {
        if (lineNo==currentBrowsline)
          currentDisplayedBrowseLine = currentBrowsline;
        lineNo++;
      }
#endif      
    }

    if ((dpl[c].flags & PL_SWITCH_BEAM_OFF)/* && (dpl[c].type != PL_ZERO)*/)
    {
      // last was with light on, now switch it off
      // or at least prepare switch off
//&& (!(dpl[c].flags & PL_CALIBRATE))      
      if (((dpl[c].type == PL_MOVE) || (dpl[c].type == PL_DRAW)|| (dpl[c].type == PL_DRAW_PATTERN) ) && (dpl[c-1].type != PL_DRAW_DOT)) 
      {
        // we were drawing now - and next we will either move or draw again

// not better        
// waitUntilAndUse(50*ONE_WAIT_UNIT);
        delayedBeamOff = 1;
        LINE_DEBUG_OUT ("Delayed beam off: \n\r");
      }
      else
      {
        // we are last was a dot or a zero
        // pls don't ask why +5
        // it just is !)(/"(!=!??!!!
        // e.g. this is a move, last time was a draw -> switch off
        int d = DELAY_AFTER_T1_END_VALUE;
        d -= 12;
        if (d>0) waitT1OffMark(d);
        
        LINE_DEBUG_OUT ("Beam off: \n\r");
        
        SWITCH_BEAM_OFF();
        if ((dpl[c-1].flags & PL_DEFLOK_AFTER)/* && (dpl[c].type != PL_ZERO)*/)
        {
          LINE_DEBUG_OUT ("Deflok after: \n\r");
          v_resetIntegratorOffsets0();
          ZERO_AND_WAIT();
        }        
      }
    }
  }

  // savety only
  SWITCH_BEAM_OFF();
  ZERO_AND_CONTINUE();
  
 
#ifdef MY_DEBUG
  uint32_t value;
  CCNT0(value);

  value = (value-startValue) / 666; // in vectrex cycles
  if (myDebug)   printf("Pipeline display: %i\r\n", value);
#endif    
}
