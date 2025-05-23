#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <vectrex/vectrexInterface.h>

#define VTERM_NO_CONNECTED 0
#define VTERM_CONNECTED 1
#define VTERM_WAIT_FOR_DATA 2

#define ONCE_PER_ROUND (1000000/50)
#define TIME_OUT_VALUE (3*ONCE_PER_ROUND)
int termBrightness = 0x7f;
static aux_t* auxillary = (aux_t*)AUX_BASE;

int receiveCount = 0;

/*
#define DEBUG_OUT(...) do \
  { \
    char __buff[256];\
    sprintf(__buff,__VA_ARGS__);\
    { \
      if (!__debugf) {__debugf = fopen("../../../printf.log", "a"); \
        {if (!__debugf) __debugf = fopen("../../printf.log", "a"); \
          {if (!__debugf) __debugf = fopen("../printf.log", "a"); \
              if (!__debugf) __debugf = fopen("printf.log", "a"); }}}\
      if (__debugf) \
      { \
        fprintf(__debugf,__buff); \
        fflush(__debugf); \
      } \
    }\
  } while(0)
*/
#define DEBUG_OUT

#define INFO_OUT(...) do \
  { \
    char __buff[256];\
    sprintf(__buff,__VA_ARGS__);\
    { \
      if (!__debugf) {__debugf = fopen("../../../printf.log", "a"); \
        {if (!__debugf) __debugf = fopen("../../printf.log", "a"); \
          {if (!__debugf) __debugf = fopen("../printf.log", "a"); \
              if (!__debugf) __debugf = fopen("printf.log", "a"); }}}\
      if (__debugf) \
      { \
        fprintf(__debugf,__buff); \
        fflush(__debugf); \
      } \
    }\
  } while(0)



// todo print vectrex logo VTERM
int tryToConnect()
{
  // non blocking function
  #define MAX_STRING_SIZE 256
   
  int ret = -1; // error
  int counter = 0;
  char buffer[MAX_STRING_SIZE]; // Array to store the converted string
  buffer[0] = 0;

  uint32_t startTime = v_micros();
  
  while (1)
  {
    if (v_micros()-startTime>ONCE_PER_ROUND) 
    {
      DEBUG_OUT("Pi:VTerm connect timeout.\n");
      return VTERM_NO_CONNECTED;
    }
    
    
    while (RPI_AuxMiniUartReadPending())
    {
      char r = RPI_AuxMiniUartRead();
      if (r == '\n')
      {
        break;
      }
      if (r != '\n')
      {
          if (counter<MAX_STRING_SIZE-1)
          {
            buffer[counter++] = r;
            buffer[counter] = (char) 0;
          }
          else 
            break; // error
      }
    }
    if (strcmp("PiTrex VTerm Connect", buffer) ==0)
    {
      // send connected
      // force output to uart stdout     
      fprintf(stdout,"Pi:VTerm connect acknowledged.\n");
      DEBUG_OUT("Pi:VTerm connect acknowledged.\n");
      return VTERM_CONNECTED;
    }
  } 
  return VTERM_NO_CONNECTED;
}


#define VTERM_WAIT_RECAL 0
#define VTERM_BRIGHNESS 1
#define VTERM_DOT 2
#define VTERM_SINGLE_LINE 3
#define VTERM_START_OF_PATH 4
#define VTERM_END_LIST 5
#define VTERM_SET_BITCOUNT 6
#define VTERM_SET_BRIGHTNESS_IGNORE_IN_PATH 7



// blocking till char read
static inline unsigned char RPI_AuxMiniUartReadInlined()
{
    while (!(auxillary->MU_LSR & AUX_MULSR_DATA_READY));
    return auxillary->MU_IO;
}
static inline void UARTGetBinaryDataForced(int size, unsigned char *romBuffer)
{
  int counter = 0; 
  while (counter != size)
    romBuffer[counter++] = RPI_AuxMiniUartReadInlined();
}

unsigned char getOne8BitValue()
{
  if (auxillary->MU_LSR & AUX_MULSR_RX_OVERRUN) INFO_OUT("UART INPUT BUFFER OVERRUN!\n");
  return RPI_AuxMiniUartReadInlined();
} 

unsigned char readBuffer[100000];
int readBufferLength=0;
#define NEXTBYTE readBuffer[readpos++]

// 8 bit only
int receiveData()
{
  int hi = getOne8BitValue();
  int lo = getOne8BitValue();
  int size = 256*hi+lo;
  DEBUG_OUT("Reading Bulk Data: %i\n", size);
  UARTGetBinaryDataForced(size, readBuffer);
  RPI_AuxMiniUartWrite('k');
  DEBUG_OUT("Ack sent.\n");

  int readpos = 0;
  while (1)
  {
    unsigned char r = NEXTBYTE;
    switch (r)
    {
      case VTERM_BRIGHNESS:
      {
        termBrightness = NEXTBYTE;
        break;
      }
      case VTERM_SINGLE_LINE:
      {
        int x0 = NEXTBYTE;
        int y0 = NEXTBYTE;
        int x1 = NEXTBYTE;
        int y1 = NEXTBYTE;
        
        y0 = (y0-128)*80;
        x0 = (x0-128)*128;
        y1 = (y1-128)*80;
        x1 = (x1-128)*128;
        v_directDraw32(x0,y0,x1,y1, termBrightness);
      break;
      }
      case VTERM_START_OF_PATH:
      {
        int count = NEXTBYTE-1;
        int x0 = NEXTBYTE;
        int y0 = NEXTBYTE;
        int x1 = NEXTBYTE;
        int y1 = NEXTBYTE;
        
        y0 = (y0-128)*80;
        x0 = (x0-128)*128;
        y1 = (y1-128)*80;
        x1 = (x1-128)*128;
        v_directDraw32(x0,y0,x1,y1, termBrightness);
        
        for (int i=0;i<count;i++)
        {
          int x2 = NEXTBYTE;
          int y2 = NEXTBYTE;
        
          y2 = (y2-128)*80;
          x2 = (x2-128)*128;
          v_directDraw32(x1,y1,x2,y2, termBrightness);
          x1 = x2;
          y1 = y2;
        }          
        break;
      }
      
      case VTERM_DOT:
      {
        int x0 = NEXTBYTE;
        int y0 = NEXTBYTE;
        
        y0 = (y0-128)*80;
        x0 = (x0-128)*128;

        v_directDraw32(x0,y0,x0,y0, termBrightness);
        break;
      }
      case VTERM_END_LIST:
      {
        return VTERM_CONNECTED;
      }
      
      default:
      {
        INFO_OUT("UNKOWN VTerm Command: %02x ignoring\n", r);
        break;
      }
    }
  }
  INFO_OUT("VTerm Error receiving\n");
  return VTERM_NO_CONNECTED;
}

int waitForStartSignal()
{
  unsigned char r = getOne8BitValue();
  switch (r)
  {
    case VTERM_WAIT_RECAL:
    {
      return VTERM_WAIT_FOR_DATA;
    }
    default:
    {
      INFO_OUT("WR MISMATCH $%02x\n", r);
      break;
    }
  }
  return VTERM_CONNECTED;
}

int main(int argc, char **argv) 
{
  vectrexinit(1);
  v_init();
  v_noSound();

  int pv = 0;
  if (pv = ini_parse("vterm.ini", iniHandler, 0) < 0) 
  {
        DEBUG_OUT("vterm.ini not loaded!\n\r");
  }
  else
    DEBUG_OUT("vterm.ini loaded!\n\r");
  
  v_setRefresh(50);
  v_setClientHz(50); 
//  v_setupIRQHandling();
  v_removeIRQHandling();  
  usePipeline = 1;
  bufferType = 1;

  v_enableSerialCommands(0);
        
  v_enableJoystickDigital(0,0,0,0);
  v_enableJoystickAnalog(0,0,0,0);
  v_enableSoundOut(0);
  v_enableButtons(1);
  useDoubleTimer = 1;
  v_setCustomClipping(1, -16000, -14000, 14400, 15000);
  keepDotsTogether = 1;
  
  int state = VTERM_NO_CONNECTED;
    
  while (1)
  {
    if (state == VTERM_NO_CONNECTED)
    {
      state = tryToConnect();
    }
    if (state == VTERM_CONNECTED)
    {
      state = waitForStartSignal();
    }
    if (state == VTERM_WAIT_FOR_DATA)
    {
      state = receiveData();
    }
    v_WaitRecal();
  }
  return 0;
}
