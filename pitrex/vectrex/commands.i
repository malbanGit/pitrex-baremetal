

extern int pendingReturnToPiTrex;

/*
extern int currentSwitch;
extern int currentRead;
extern int currentWrite;
extern int currentWait;
extern int currentStage;
extern int currentT1;
extern int currentt1Counter;
void iCommand(void)
{
    printf("SMP debug values\r\n");
    printf("currentSwitch: %i \r\n", currentSwitch);
    printf("currentRead: %i \r\n", currentRead);
    printf("currentWrite: %i \r\n", currentWrite);
    printf("currentWait: %i \r\n", currentWait);
    printf("currentStage: %i \r\n", currentStage);
    printf("currentT1: %08x \r\n", currentT1);
    printf("currentt1Counter: %i \r\n", currentt1Counter);
    printf("irqButtons: %i \r\n", irqButtons);
    printf("irqSound: %i \r\n", irqSound);
    printf("irqJoyAnalog: %i \r\n", irqJoyAnalog);
    printf("irqJoyDigital: %i \r\n", irqJoyDigital);
    printf("currentButtonState: %i \r\n", currentButtonState);
}
*/

void resetCommand(void)
{



/*
    v_removeIRQHandling();
    void *progSpace = (void *) 0x8000; 
    char *FILE_NAME = "pitrex.img";
    
    FRESULT rc_rd = FR_DISK_ERR;
    FIL file_object_rd;
    rc_rd = f_open(&file_object_rd, FILE_NAME, (unsigned char) FA_READ);
    
    v_noSound();
    
    printf("Loading: %s \r\n", FILE_NAME);
    unsigned int fsize = 1000000;

    rc_rd = f_read(&file_object_rd, progSpace, fsize, &fsize);
    if ( rc_rd!= FR_OK)
    {
      printf("loaderMain.c: loadAndStart(): File not loaded (%s) (size got = %i)\r\n", FILE_NAME, rc_rd);
      f_close(&file_object_rd);
    }
    else
    {
        f_close(&file_object_rd);

        printf("Starting loaded file... \r\n");
        isb();
        dsb();
        dmb();
        mmu_disable(); 
        cache_flush(); // flush I/D-cache 
    // correct start registers and jump to 8000
    __asm__ __volatile__(
        "mov r5, #0x0080   \n\t"
        "ldr r0, [r5]      \n\t"
        "mov r5, #0x0084   \n\t"
        "ldr r1, [r5]      \n\t"
        "mov r5, #0x0088   \n\t"
        "ldr r2, [r5]      \n\t"
        "ldr pc, = 0x8000  \n\t"
      );
    }
*/












  if (!isIRQMode)
  {
    printf("Restarting kernel...\r\n");
    uint32_t progSapce = LOADER_START;
    void (*progStart)(void) = (void (*)(void))progSapce;
    progStart();
  }
  pendingReturnToPiTrex = 1;
}

void saveCommand(void)
{
  printf("Saving config...\r\n");
  v_saveIni();
//  v_saveSettings(knownName, knownBlob, knownBlobSize);
}
void setMaxStrengthCommand(void)
{
  unsigned int sms = bm_atoi(getParameter(0),10);
  if (sms == -1410065407)
  {
    printf("SetMaxStrength value: %i \r\n", MAX_USED_STRENGTH);
    return;
  }
  if ((sms<0) || (sms>127))
  {
    printf("SetMaxStrength invalid value: %i (%i)\r\n", sms, MAX_USED_STRENGTH);
    return;
  }
  printf("SetMaxStrength set to: %i\r\n", sms);
  
  MAX_USED_STRENGTH = sms;
}

void setCrankyValueCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("SetCrankyValue value: $%02x \r\n", crankyFlag);
    return;
  }
  if ((c<0) || (c>255))
  {
    printf("SetCrankyValue invalid value: $%02x (%02x)\r\n", c, crankyFlag);
    return;
  }
  printf("SetCrankyValue set to: $%02x\r\n", c);
  
  crankyFlag = c;
}
void setRefreshCommand(void)
{
  int hz = 1500000 / ((Vec_Rfrsh&0xff)*256 +  ((Vec_Rfrsh>>8)&0xff));
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("SetRefresh value: %i \r\n", hz);
    return;
  }
  if ((c<23) || (c>200))
  {
    printf("SetRefresh invalid value: %i (%i)\r\n", c, hz);
    return;
  }
  printf("SetRefresh set to: %i\r\n", c);
  
  v_setRefresh(c);
}

void setMaximumConsecutiveDrawsCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("SetMaximumConsecutiveDraws value: %i \r\n", MAX_CONSECUTIVE_DRAWS);
    return;
  }
  if ((c<0) || (c>999))
  {
    printf("SetMaximumConsecutiveDraws invalid value: %i (%i)\r\n", c, MAX_CONSECUTIVE_DRAWS);
    return;
  }
  printf("SetMaximumConsecutiveDraws set to: %i\r\n", c);
  
  MAX_CONSECUTIVE_DRAWS = c;
}
void setDelayZeroMaxCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("SetDelayZeroMax value: %i \r\n", DELAY_ZERO_VALUE);
    return;
  }
  if ((c<0) || (c>255))
  {
    printf("SetDelayZeroMax invalid value: %i (%i)\r\n", c, DELAY_ZERO_VALUE);
    return;
  }
  printf("SetDelayZeroMax set to: %i\r\n", c);
  
  DELAY_ZERO_VALUE = c;
}
void setT1OffDelayCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("SetT1OffDelay value: %i \r\n", DELAY_AFTER_T1_END_VALUE);
    return;
  }
  if ((c<0) || (c>255))
  {
    printf("SetT1OffDelay invalid value: %i (%i)\r\n", c, DELAY_AFTER_T1_END_VALUE);
    return;
  }
  printf("SetT1OffDelay set to: %i\r\n", c);
  
  DELAY_AFTER_T1_END_VALUE = c;
}
void setCycleEQCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("SetCycleEQ value: %i \r\n", getCycleEquivalent());
    return;
  }
  if ((c<100) || (c>999))
  {
    printf("SetCycleEQ invalid value: %i (%i)\r\n", c, getCycleEquivalent());
    return;
  }
  printf("SetCycleEQ set to: %i\r\n", c);
  
  setCycleEquivalent(c);
}


void setOrientationCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("SetOrientation value: %i \r\n", orientation);
    return;
  }
  if ((c<0) || (c>3))
  {
    printf("SetOrientation invalid value: %i (%i)\r\n", c, orientation);
    return;
  }
  printf("SetOrientation set to: %i\r\n", c);
  
  orientation = c;
}
void setMyDebugCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("SetMyDebug value: %i \r\n", myDebug);
    return;
  }
  if ((c<0) || (c>1))
  {
    printf("SetMyDebug invalid value: %i (%i)\r\n", c, myDebug);
    return;
  }
  printf("SetMyDebug set to: %i\r\n", c);
  
  myDebug = c;
}

// pstate is used in wait recal!
void setPipelineCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("SetPipeline value: %i \r\n", usePipeline);
    return;
  }
  if ((c<0) || (c>1))
  {
    printf("SetPipeline invalid value: %i (%i)\r\n", c, usePipeline);
    return;
  }
  printf("SetPipeline set to: %i\r\n", c);
  
  usePipeline = c;
}

void setDoubleTimerCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("SetDoubleTimer value: %i \r\n", useDoubleTimer);
    return;
  }
  if ((c<0) || (c>1))
  {
    printf("SetDoubleTimer invalid value: %i (%i)\r\n", c, useDoubleTimer);
    return;
  }
  printf("SetDoubleTimer set to: %i\r\n", c);
  
  useDoubleTimer = c;
}


void setBeamOffInDrawCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("SetBeamOffInDraw value: %i \r\n", beamOffBetweenConsecutiveDraws);
    return;
  }
  if ((c<0) || (c>1))
  {
    printf("SetBeamOffInDraw invalid value: %i (%i)\r\n", c, beamOffBetweenConsecutiveDraws);
    return;
  }
  printf("SetBeamOffInDraw set to: %i\r\n", c);
  
  beamOffBetweenConsecutiveDraws = c;
}
void setScaleStrengthCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("SetScaleStrength value: %i \r\n", SCALE_STRENGTH_DIF);
    return;
  }
  if ((c<0) || (c>11))
  {
    printf("SetScaleStrength invalid value: %i (%i)\r\n", c, SCALE_STRENGTH_DIF);
    return;
  }
  printf("SetScaleStrength set to: %i\r\n", c);
  
  SCALE_STRENGTH_DIF = c;
}

void setMinimumScaleCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("MinimumScale value: %i \r\n", MINSCALE);
    return;
  }
  if ((c<0) || (c>20))
  {
    printf("MinimumScale invalid value: %i (%i)\r\n", c, MINSCALE);
    return;
  }
  printf("MinimumScale set to: %i\r\n", c);
  
  MINSCALE = c;
}
void setBufferTypeCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("SetBufferType value: %i \r\n", bufferType);
    return;
  }
  if ((c<0) || (c>3))
  {
    printf("SetBufferType invalid value: %i (%i)\r\n", c, bufferType);
    return;
  }
  printf("SetBufferType set to: %i\r\n", c);
  
  bufferType = c;
}
extern volatile int pendingDisableInterrupts;
extern volatile int pendingEnableInterrupts;
void irqCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("irqMode value: %i \r\n", isIRQMode);
    return;
  }
  if ((c<0) || (c>32001))
  {
    printf("irqMode invalid value: %i (%i)\r\n", c, isIRQMode);
    return;
  }
  printf("irqMode set to: %i\r\n", c);
  if ((isIRQMode == 1) && (c==0))
  {
    pendingDisableInterrupts = 1;
  }
  else
  if ((isIRQMode == 0) && (c==1))
  {
#if RASPPI != 1  
    if (isSMPMode)
    {
      pendingDisableMultiCore = 1;
      pendingEnableInterrupts = 1;
    }
    else
#endif    
    {
      v_setupIRQHandling();
    }
  }
}

extern volatile int pendingDisableMultiCore;
extern volatile int pendingEnableMultiCore;

void smpCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("smp value: %i \r\n", isSMPMode);
    return;
  }
  if ((c<0) || (c>32001))
  {
    printf("smp invalid value: %i (%i)\r\n", c, isSMPMode);
    return;
  }
  printf("smp set to: %i\r\n", c);
  if ((isSMPMode == 1) && (c==0))
  {
    pendingDisableMultiCore = 1;
  }
  else
  if ((isSMPMode == 0) && (c==1))
  {
    v_setupSMPHandling();
  }
}


void setZeroDifMaxCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("SetZeroDifMax value: %i \r\n", resetToZeroDifMax);
    return;
  }
  if ((c<0) || (c>32001))
  {
    printf("SetZeroDifMax invalid value: %i (%i)\r\n", c, resetToZeroDifMax);
    return;
  }
  printf("SetZeroDifMax set to: %i\r\n", c);
  
  resetToZeroDifMax = c;
}

extern uint32_t waitClientHzWait;

void setClientRefreshCommand(void)
{
  unsigned int c = bm_atoi(getParameter(0),10);
  if (c == -1410065407)
  {
    printf("ClientHz value: %i \r\n", clientHz);
    return;
  }
  if ((c<0) || (c>1000))
  {
    printf("ClientHz invalid value: %i (%i)\r\n", c, clientHz);
    return;
  }
  printf("ClientHz set to: %i\r\n", c);
  if (c==0) isSynchronizedWithIRQ = 1; else isSynchronizedWithIRQ = 0;
  clientHz = c;
  waitClientHzWait = 0;
}

void getPipelineCount(void)
{
    printf("Pipeline size: %i \r\n", lastPLSize);
}

extern unsigned int roundCycles;
void outputRoundCyclesCommand(void)
{
  printf("Cycles used last round: %i (%i)\r\n", roundCycles/666, clientRate);
}

void setBrowseModeCommand(void)
{
  printf("Entering browse mode\r\n");
  browseMode = 1;
  browseSkip = 0;
  currentBrowsline = 0;
  currentDisplayedBrowseLine = -1;
}
void debugCommand(void)
{
  printf("Entering debug mode\r\n");
  executeDebugger(3);// break
}

void helpCommand(void);
Command commandList[] =
{ 
        {1,"help", "h",             "help             | h               -> display all command\r\n" ,  helpCommand },
        {1,"reset", "r",            "reset            | r               -> reset to loader\r\n" ,  resetCommand },
        {1,"save", "s",             "save             | s               -> save config\r\n" ,  saveCommand },
        {1,"setMaxStrength", "sms", "setMaxStrength   | sms xx (1-127)  -> set maximum strength for optimal scales\r\n" ,  setMaxStrengthCommand },
        {1,"setCrankyValue", "scv", "setCrankyValue   | scv xx (0-255)  -> set value for additional cranky delay (flaged)\r\n" ,  setCrankyValueCommand },
        {1,"setRefresh", "sr",      "setRefresh       | sr  xx (25-100) -> set value refresh rate in Hz\r\n" ,  setRefreshCommand },
        {1,"setMaxDraws", "smd",    "setMaxDraws      | smd xx (0-999)  -> how many vectors are drawn till zero ref\r\n" ,  setMaximumConsecutiveDrawsCommand },
        {1,"setDelayZero", "sdz",   "setDelayZero     | sdz xx (0-255)  -> how long must be waited after a zero ref\r\n" ,  setDelayZeroMaxCommand },
        {1,"setT1OffDelay", "sto",  "setT1OffDelay    | sto xx (0-25)   -> after a draw - how many cycles till light is switched off \r\n" ,  setT1OffDelayCommand },
        {1,"setCycleEQ", "sc",      "setCycleEQ       | sc  xx (100-999)-> vectrex cycles / microseconds \r\n" ,  setCycleEQCommand },
        {1,"setOrientation", "so",  "setOrientation   | s0  xx (0-3)    -> orientation of output \r\n" ,  setOrientationCommand },
        {1,"myDebug", "md",         "myDebug          | md  xx (0-1)    -> debug on off \r\n" ,  setMyDebugCommand },
        {1,"usePipeline", "up",     "usePipeline      | up  xx (0-1)    -> pipeline on off \r\n" ,  setPipelineCommand },
        {1,"beamOffInDraw", "bo",   "beamOffInDraw    | bo  xx (0-1)    -> in consecutive draws, switch beam off \r\n" ,  setBeamOffInDrawCommand },
        {1,"setScaleStrength","sss","setScaleStrength | sss xx (0-10)   -> set correction value to scale/strength conversion \r\n" ,  setScaleStrengthCommand },
        {1,"setMinimumScale","smc" ,"setMinimumScale  | smc xx (0-10)   -> set minimum value to scale \r\n" ,  setMinimumScaleCommand },
        {1,"setBufferType","bt",    "setBufferType    | bt  xx (0-3)    -> 0 = no buffer, 1 = doublebuffer, 2 = autobuffer, 3 = no display \r\n" ,  setBufferTypeCommand },
        {1,"setZeroDifMax","zdm",   "setZeroDifMax    | zdm xx (0-32000)-> margin when a zero ref is forced \r\n" ,  setZeroDifMaxCommand },
        {1,"browserMode","bm",      "browserMode      | bm              -> enter browse mode \r\n" ,  setBrowseModeCommand },
        {1,"doubleTimer","dt",      "doubleTimer      | dt              -> 0 no, 1 yes \r\n" ,  setDoubleTimerCommand },
        {1,"pipelineCount","pc",    "pipelineCount    | pc              -> print length of current pipeline \r\n" ,  getPipelineCount },
        {1,"pipelineAsVList","pas", "pipelineAsVList  | pas             -> save current pipeline as screenshort (db) \r\n" ,  saveScreenshot },

//        {1,"info","i",              "i                | i               -> some SMP debug info \r\n" ,  iCommand },
        {1,"smp","smp",             "smp              | smp             -> use multicore vector processing \r\n" ,  smpCommand },
        {1,"irqMode","irq",         "irqMode          | irq             -> switch between display modes \r\n" ,  irqCommand },
        {1,"setClientRefresh","scr","setClientRefresh | scr             -> set Refresh of client program (0 = sync) \r\n" ,  setClientRefreshCommand },
        {1,"outputCycles","oc",     "outputCycles     | oc              -> output cycles used for 1 round (only usefull if larger than Hz) \r\n" ,  outputRoundCyclesCommand },

        {1,"debug","dbg",           "debug            | dbg             -> enter debug mode (if supported)\r\n" ,  debugCommand },
        {0,"", "", "" ,  (void (*)(void)) 0 }
};


Command dummyCommandList[] =
{
        {0,"", "", "" ,  (void (*)(void)) 0 }
};
void helpCommand(void)
{
        // show help:
        int commandListCounter = 0;
        printf("List of API-Commands\r\n");
        printf("--------------------\r\n");
        while (commandList[commandListCounter].id != 0)
        {
                printf(commandList[commandListCounter].commandHelp);
                commandListCounter++;
        }
        
        commandListCounter = 0;
        printf("\r\nList of User-Commands\r\n");
        printf("---------------------\r\n");
        while (userCommandList[commandListCounter].id != 0)
        {
                printf(userCommandList[commandListCounter].commandHelp);
                commandListCounter++;
        }
}




static char commandBuffer[80];
static char *commandBufferPointer;
static int commandBufferCounter;
void v_initDebug()
{
        commandBufferPointer = commandBuffer;
        commandBufferCounter = 0;

// can be set from user program
        userCommandList = dummyCommandList;


        while (RPI_AuxMiniUartReadPending()) RPI_AuxMiniUartRead();
}

// increases the pointer until it stays on the first character after the first space
char *skipPastSpace(char *p)
{
        while ((*p != ' ') && (*p != (char) 0)&& (*p != '\r')&& (*p != '\n')&& (*p != '\t') )
        {
                p++;
        }
        while (*p == ' ') p++;
        return p;
}

// copies from source to destination, until the first space/0
// changed source is return, placed after SPACE
char *untilSpace(char *source, char *destination)
{
        while ((*source != ' ') && (*source != '\r')&& (*source != '\n')&& (*source != '\t') &&(*source != (char) 0))
        {
          *destination++ = *source++;
        }
        while ((*source == ' ') || (*source == '\r')|| (*source == '\n')|| (*source == '\t')) source++;
        *destination = (char)0;
        return source;
}

char parameter1[10];
char parameter2[10];
char parameter3[10];
char parameter4[10];
char parameter5[10];
char command[10];

char *paramters[] = 
{
        parameter1,
        parameter2,
        parameter3,
        parameter4,
        parameter5
};
// max 5
char *getCommandParameter(int parameterNo)
{
        char *result = paramters[parameterNo];
        *result=(char)0;
        if ((parameterNo<0) || (parameterNo>4))
        {
                return result;
        }
        
        char *place = commandBuffer;
        while ((*place == ' ') || (*place == '\r')|| (*place == '\n')|| (*place == '\t')) place++; // trim
        place = skipPastSpace(place);  // first string is command itself
        if (*place == (char) 0) return result;
        int counter = 0;
        while (counter <= parameterNo)
        {
                place = untilSpace(place, result);
                if (*place == (char) 0) 
                {
                    if (parameterNo>counter)
                      *result=(char)0;
                    break;
                }
                counter++;
        }
        return result;
}
void fillParameters()
{
  getCommandParameter(0);
  getCommandParameter(1);
  getCommandParameter(2);
  getCommandParameter(3);
  getCommandParameter(4);
}
char *getCommand()
{
  char *place = commandBuffer;
  int cc = 0;
  command[cc] = (char)0;
  while ((*place == ' ') || (*place == '\r')|| (*place == '\n')|| (*place == '\t')) place++; // trim
  if (*place == (char) 0) return command;
  place = untilSpace(place, command);
  return command;
}

char *getParameter(int p)
{
  if ((p<0)||(p>4))
  {
    return "invalid parameter";
  }
  fillParameters();
  return paramters[p];
}

void handleCommand()
{
        if (strlen(commandBuffer) == 0) return;

        char *cmd = getCommand();
        if (strlen(cmd) == 0) return;
        
        int commandListCounter = 0;
        while (commandList[commandListCounter].id != 0)
        {
                if (strlen(cmd) != strlen(commandList[commandListCounter].command)) {commandListCounter++;continue;}
                
                if (strncasecmp(cmd, commandList[commandListCounter].command,strlen(commandList[commandListCounter].command)) == 0)
                {
                        commandList[commandListCounter].commandHandler();
                        return;
                }
                commandListCounter++;
        }

        commandListCounter = 0;
        while (commandList[commandListCounter].id != 0)
        {
                if (strlen(cmd) != strlen(commandList[commandListCounter].commandShort)) {commandListCounter++;continue;}
                if ((strncasecmp(cmd, commandList[commandListCounter].commandShort, strlen(commandList[commandListCounter].commandShort)) == 0) )
                {
                        commandList[commandListCounter].commandHandler();
                        return;
                } 
                commandListCounter++;
        }
        
        
        commandListCounter = 0;
        while (userCommandList[commandListCounter].id != 0)
        {
                if (strlen(cmd) != strlen(commandList[commandListCounter].command)) {commandListCounter++;continue;}
                if (strncasecmp(cmd, userCommandList[commandListCounter].command,strlen(userCommandList[commandListCounter].command)) == 0)
                {
                        userCommandList[commandListCounter].commandHandler();
                        return;
                }
                commandListCounter++;
        }
        commandListCounter = 0;
        while (userCommandList[commandListCounter].id != 0)
        {
                if (strlen(cmd) != strlen(commandList[commandListCounter].commandShort)) {commandListCounter++;continue;}
                if (strncasecmp(cmd, userCommandList[commandListCounter].commandShort, strlen(userCommandList[commandListCounter].commandShort)) == 0) 
                {
                        userCommandList[commandListCounter].commandHandler();
                        return;
                }
                commandListCounter++;
        }
        printf("unkown command: '%s'\r\n",commandBuffer );
}
void handleUARTInterface()
{
        while (RPI_AuxMiniUartReadPending())
        {
          char r = RPI_AuxMiniUartRead();

          if (!browseMode)
          {
            RPI_AuxMiniUartWrite(r);
            if (r != '\n')
            {
                    if (commandBufferCounter<78)
                    {
                            *commandBufferPointer++ = r;
                            *commandBufferPointer = (char) 0;
                            commandBufferCounter++;
                    }
            }
            else
            {
                  if (commandBufferCounter != 0)
                  {
                          handleCommand();
                  }
                    commandBufferPointer = commandBuffer;
                    commandBufferCounter = 0;
            }
          }
          else
          {

            if (r=='r') currentDisplayedBrowseLine = -1; // redisplay
            if (r=='n') currentBrowsline++; // next
            if (r=='N') currentBrowsline+=10; // next
            if (r=='p') currentBrowsline--; // previous
            if (r=='P') currentBrowsline-=10; // previous
            if (r=='s') {browseSkip = 1;currentDisplayedBrowseLine = -1;} // 1 round of emulation
            if (r=='x') browseMode = 0; // exit
            if (currentBrowsline<0) currentBrowsline=0;
          }


        }
}
