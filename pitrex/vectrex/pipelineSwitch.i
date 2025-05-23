    switch (dpl[c].type)
    {
      case PL_DEFLOK:
      {
        LINE_DEBUG_OUT("PL DEFLOK\r\n");
        if (dpl[c].flags & PL_SWITCH_BEAM_OFF) 
		{
			SWITCH_BEAM_OFF();
		}

		DEFLOCK   
		IRESET
		PLAY_SAMPLE_IGNORE_A();
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
 		  IRESET
		  PLAY_SAMPLE_IGNORE_A();

          dpl[c+1].last_timing = (dpl[c].this_timing - timeDone);
          if (dpl[c+1].last_timing<0) dpl[c+1].last_timing=0;
        }
        break;
      }

      case PL_RASTER_TEXT:
      {
        LINE_DEBUG_OUT("PL RASTER DISPLAY AT x,y: %i,%i\r\n", dpl[c].x, dpl[c].y);

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

				waitCounter1MarkLocal(movementScale+DELAY_AFTER_T1_END_VALUE);
				PLAY_SAMPLE_IGNORE_A();
			////////////////////////////////////////////////////////////////////

            // move to line in String (only y-movement)
            if (yoffset!=0)
            {

              if (halfOffset)
              {
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
			  SETW_inverse(VIA_t1, printScale); setCounter1Mark(); 
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
//          SWITCH_BEAM_ON();
              SET_SHIFT_REG(1+2+4+8+16);
        }
        break;
      }
      case PL_MOVE:
      {
        LINE_DEBUG_OUT("PL MOVE: xStart: %i, x:%i x*s:%i, yStart: %i, y:%i y*s:%i scale:%i\r\n", dpl[c].xStart, dpl[c].x,dpl[c].x*dpl[c].this_timing, dpl[c].yStart, dpl[c].y, dpl[c].y*dpl[c].this_timing, dpl[c].this_timing);
        LINE_DEBUG_OUT("PL MOVE: X0: %i, Y0: %i\r\n",dpl[c].x0,dpl[c].y0);
        LINE_DEBUG_OUT("PL MOVE: Restx(%i), Resty(%i)\r\n",dpl[c].xRest,dpl[c].yRest);
        // fall thru
      }
      case PL_DRAW_PATTERN:
      {
        if (dpl[c].type ==PL_DRAW_PATTERN)
          LINE_DEBUG_OUT ("PL DRAW PATTERN \r\n    %s\r\n", dpl[c].debug);
        // fall thru
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
		//  printf("CALIBRATION 0\r\n");
		  I_SET (VIA_port_b, 0x81);
		  DELAY_PORT_B_BEFORE_PORT_A();
		  I_SET_A(0x00);
		  ADD_DELAY_CYCLES(4);
		  I_SET (VIA_port_b, 0x80);
		  ADD_DELAY_CYCLES(6);
		// Change
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
          LINE_DEBUG_OUT("Scale %i, Calibration value %i\r\n", dpl[c].this_timing, calibrationValues[dpl[c].this_timing]);
  		  //////////////////////////////////////////////////////////////////////////
		  //v_resetIntegratorOffsets();
		  {
		    I_SET (VIA_port_b, 0x81);
		    DELAY_PORT_B_BEFORE_PORT_A();
            if (calibrationValues[dpl[c].this_timing] == 0) // only if calibration value is not 0
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
				
				I_SET_A(calibrationValues[dpl[c].this_timing]);
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
			  PLAY_SAMPLE_IGNORE_A();
			}

//          if (calibrationValue !=0)
          {
            // TODO Costs time :-()
            // isn't that too much???
            I_SET_A(0x00);
			ADD_DELAY_CYCLES(4);
            I_SET(VIA_port_b, 0x80);
			ADD_DELAY_CYCLES(4);
            I_SET(VIA_port_b, 0x81);
    		ADD_DELAY_CYCLES(4);
          }
        }


		// should there be instead a "delayed ramp off"          -> YES!
		// new
		// if THIS is a move than we can use the later -> saved some cycles
		if (dpl[c].flags & PL_LAST_MUST_FINISH )
		{
		  if (dpl[c].flags & PL_LAST_IS_RAMPING)
		  {
			  waitT1OffMark(DELAY_AFTER_T1_END_VALUE);
		  }
		}    
		// new          
        
        if (dpl[c].flags & PL_Y_MUST_BE_SET)
        {
          LINE_DEBUG_OUT("     Y MUST BE I_SET (%i)\r\n", dpl[c].y);
          if (dpl[c].flags & PL_Y_A_MUST_BE_SET)
          {
            LINE_DEBUG_OUT("     YA MUST BE I_SET\r\n");
            I_SET_A(dpl[c].y);
            ADD_DELAY_CYCLES(YSH_A_DELAY); // wait for enable ramp, and printing the last letter
          }
          if (dpl[c].flags & PL_MUX_Y_MUST_BE_SET)
          {
            //what was set before? -> waiting for T1 timer
            I_SET(VIA_port_b, 0x80); // MUX to y integrator
            ADD_DELAY_CYCLES(YSH_B_DELAY); // wait for enable ramp, and printing the last letter
          }
        }
        else
          LINE_DEBUG_OUT("     Y NEED NOT BE I_SET\r\n");

        		
        // for the beam to be switched off from last drawing we have
        // to wait alltogether for "DELAY_AFTER_T1_END_VALUE" cycles
        if (delayedBeamOff)
        {
          LINE_DEBUG_OUT ("     Delayed beam off \r\n");
          
//          SWITCH_BEAM_OFF();
          SET_SHIFT_REG(128+64+32+16+8+4);
//          SET_SHIFT_REG(128+64);
#ifdef __IRQ_PIPELINE__
vCritical = 0;
#endif
        }
        if (dpl[c].flags & PL_DEACTIVATE_ZERO)
        {
            // attention!
            // UNZERO is also a BEAM_OFF!
            LINE_DEBUG_OUT ("     UNZERO \r\n");
            UNZERO();
        }

        I_SET(VIA_port_b, 0x81);
        ADD_DELAY_CYCLES(XSH_B_DELAY); // wait for enable ramp, and printing the last letter
		
		
        
        if (dpl[c].flags & PL_X_A_MUST_BE_SET)
        {
		  // PLAY_SAMPLE_IGNORE_A();          
          LINE_DEBUG_OUT ("     X SET: %i\r\n", dpl[c].x);
          I_SET_A(dpl[c].x);
          ADD_DELAY_CYCLES(XSH_A_DELAY); // 
        }
        else
        {
			//PLAY_SAMPLE();          
        }
        
#ifdef __IRQ_PIPELINE__
        if (v_forcedBreakInMicroSeconds>0)
        {
          static uint32_t testValue;
          CCNT0(testValue);
          if ((testValue-irqBreakCheckValue)>v_forcedBreakInMicroSeconds)
          {
              static int light;
              light = 0;
              if (dpl[c].type ==PL_DRAW)
                if (!dpl[c].flags & PL_SWITCH_BEAM_ON) light=1;

              if (light == 1) SET_SHIFT_REG(0);
              waitUntilAndUse(v_forcedBreakInMicroSeconds*1000); // waiter in Nano
              if (light == 1) SET_SHIFT_REG(1+2+4+8+16+32);
              CCNT0(irqBreakCheckValue);
          }
        }
#endif


		if (dpl[c].flags & PL_T1_LO_EQUALS)
		{
            LINE_DEBUG_OUT ("     x:%i, y:%i scale:%i \r\n", dpl[c].x*dpl[c].this_timing, dpl[c].y*dpl[c].this_timing, dpl[c].this_timing);
#ifdef __IRQ_PIPELINE__
vCritical = 1;
#endif
            if (dpl[c].flags & PL_SWITCH_BEAM_ON)
            {
              LINE_DEBUG_OUT ("     beam switched on \r\n");
              SET_SHIFT_REG(1+2+4+8+16+32);
            }
		    SET(VIA_t1_cnt_hi, (dpl[c].this_timing)>>8);
			setCounter1Mark(); 
		}
		else
		{
#ifdef __IRQ_PIPELINE__
vCritical = 1;
#endif
            LINE_DEBUG_OUT ("     x:%i, y:%i scale:%i \r\n", dpl[c].x*dpl[c].this_timing, dpl[c].y*dpl[c].this_timing, dpl[c].this_timing);
            if (dpl[c].flags & PL_SWITCH_BEAM_ON)
            {
              LINE_DEBUG_OUT ("     beam switched on \r\n");
//              SWITCH_BEAM_ON();
              SET_SHIFT_REG(1+2+4+8+16+32);
            }
  		    SETW_inverse(VIA_t1, dpl[c].this_timing);  /* scale due to "enlargement" is 16 bit! */
		    setCounter1Mark(); 
		}

		if (dpl[c].type == PL_DRAW_PATTERN)// this must be  (dpl[c].type == PL_DRAW_PATTERN))
        {
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
