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

#define RING_BUFFER_SIZE 10000
unsigned char ringBuffer[RING_BUFFER_SIZE];
int rb_overflow_flag=0;
int writePosition=0;
int readPosition =-1;
int dataAvailable =0;

// blocking
// receives data in bursts"
int _newgetOne8BitValue()
{
  while (1)
  {
    DEBUG_OUT("\n");
    if (auxillary->MU_LSR & AUX_MULSR_RX_OVERRUN)
      INFO_OUT("!!! UART INPUT BUFFER OVERRUN! !!!\n");

    while (RPI_AuxMiniUartReadPending())
    {
      ringBuffer[writePosition++] = (unsigned char) RPI_AuxMiniUartRead();
      dataAvailable++;

      if (readPosition<0) readPosition = writePosition-1;
      
      if (writePosition>=RING_BUFFER_SIZE) writePosition = 0;

      if (writePosition == readPosition) rb_overflow_flag = 1;

      if (rb_overflow_flag)
      {
        DEBUG_OUT("\n\nBUFFER OVERFLOW!\n\n");
        rb_overflow_flag = 0;
      }
      DEBUG_OUT("+");
    }

    DEBUG_OUT("Every coming out? (%i)\n", dataAvailable);

    unsigned char retValue = 0;
//    if (dataAvailable!=0) 
    while (dataAvailable!=0) 
    {
      dataAvailable--;
      retValue = ringBuffer[readPosition++];
      if (dataAvailable==0) readPosition=-1;
      else if (readPosition>=RING_BUFFER_SIZE) readPosition = 0;

      receiveCount++      ;
      DEBUG_OUT("\nR: $%02x(%i) ",retValue, receiveCount);  
      return retValue;
    }
  }
  return 1000; // anything > 255 is 'error'
  
}
int getOne8BitValue()
{
  uint32_t startTime = v_micros();
  while (1)
  {
    if (auxillary->MU_LSR & AUX_MULSR_RX_OVERRUN)
      DEBUG_OUT("UART INPUT BUFFER OVERRUN!\n");

    while (RPI_AuxMiniUartReadPending())
    {
      unsigned char r = (unsigned char) RPI_AuxMiniUartRead();
      receiveCount++      ;
      DEBUG_OUT("\nR: $%02x(%i) ",r, receiveCount);  
      return r;
    }
/*
    if (v_micros()-startTime>TIME_OUT_VALUE)
    {
      break;
    }
*/    
  }
  return 1000; // anything > 255 is 'error'
} 

#define VTERM_WAIT_RECAL 0
#define VTERM_BRIGHNESS 1
#define VTERM_DOT 2
#define VTERM_SINGLE_LINE 3
#define VTERM_START_OF_PATH 4
#define VTERM_END_LIST 5
#define VTERM_SET_BITCOUNT 6
#define VTERM_SET_BRIGHTNESS_IGNORE_IN_PATH 7


unsigned char readBuffer[100000];
int readBufferLength=0;
int UARTGetBinaryData(int size, unsigned char *romBuffer);


//#define NEXTBYTE() getOne8BitValue()
#define NEXTBYTE readBuffer[readpos++]


// 8 bit only
int receiveData()
{
  int hi = getOne8BitValue();
  int lo = getOne8BitValue();
  int size = 256*hi+lo;
DEBUG_OUT("Reading Bulk Data: %i\n", size);
  UARTGetBinaryData(size, readBuffer);
  RPI_AuxMiniUartWrite('k');
DEBUG_OUT("Ack sent.\n");
  for (int i=0;i<size; i++)
  {
    DEBUG_OUT("$%02x ",readBuffer[i]);
    if (((i+1)%16) == 0) DEBUG_OUT("\n");
  }

//  return VTERM_CONNECTED;
int readpos = 0;


  while (1)
  {
    uint32_t startTime = v_micros();
DEBUG_OUT("\n(%i->%i): ", readpos, readBuffer[readpos]);
    int v = NEXTBYTE;
    if (v==1000)
    {
      DEBUG_OUT("Read Timeout!\n");
      continue;
    }
    unsigned char r = (unsigned char) v;

    switch (r)
    {

      case VTERM_BRIGHNESS:
      {
        termBrightness = (NEXTBYTE)&255;
        break;
      }
      case VTERM_SINGLE_LINE:
      {
        int x0 = ((signed char)NEXTBYTE)*128;
        int y0 = ((signed char)NEXTBYTE)*128;
        int x1 = ((signed char)NEXTBYTE)*128;
        int y1 = ((signed char)NEXTBYTE)*128;
        v_directDraw32(x0,y0,x1,y1, termBrightness);
      break;
      }
      case VTERM_START_OF_PATH:
      {
        int count = NEXTBYTE-1;
        int x0 = ((signed char)NEXTBYTE)*128;
        int y0 = ((signed char)NEXTBYTE)*128;
        int x1 = ((signed char)NEXTBYTE)*128;
        int y1 = ((signed char)NEXTBYTE)*128;
        v_directDraw32(x0,y0,x1,y1, termBrightness);
        
        for (int i=0;i<count;i++)
        {
          int x2 = ((signed char)NEXTBYTE)*128;
          int y2 = ((signed char)NEXTBYTE)*128;
          v_directDraw32(x1,y1,x2,y2, termBrightness);
          x1 = x2;
          y1 = y2;
        }          
        break;
      }
      
      case VTERM_DOT:
      {
        int x0 = ((signed char)NEXTBYTE)*128;
        int y0 = ((signed char)NEXTBYTE)*128;
        v_directDraw32(x0,y0,x0,y0, termBrightness);
        break;
      }
      case VTERM_END_LIST:
      {
        DEBUG_OUT("LIST END");

        return VTERM_CONNECTED;
      }
      
      default:
      {
        DEBUG_OUT("UNKOWN VTerm Command: %02x ignoring\n", r);
        //return VTERM_NO_CONNECTED;
        break;
      }
    }
    
    if (v_micros()-startTime>TIME_OUT_VALUE)
    {
   //   DEBUG_OUT("VTerm Timeout ignoring\n");
   //   return VTERM_NO_CONNECTED;
    }
  }
  DEBUG_OUT("VTerm Error receiving\n");
  return VTERM_NO_CONNECTED;
}
int waitForStartSignal()
{
  uint32_t startTime = v_micros();
  while (1)
  {
    int v = getOne8BitValue();
    if (v==1000)
    {
      DEBUG_OUT("Read Timeout!\n");
      continue;
    }
    unsigned char r = (unsigned char) v;
    switch (r)
    {
      case VTERM_WAIT_RECAL:
      {
DEBUG_OUT("WAIT_RECAL", r);
        receiveCount = 0;
        return VTERM_WAIT_FOR_DATA;
      }
      default:
      {
        DEBUG_OUT("VTerm Waiting for Start - unexpected Signal $%02x\n", r);
        //return VTERM_NO_CONNECTED;
        break;
      }
    }
    
    if (v_micros()-startTime>TIME_OUT_VALUE)
    {
      DEBUG_OUT("VTerm Waiting for Start - Timeout ignoring\n");
   //   return VTERM_NO_CONNECTED;
    }
  }
  DEBUG_OUT("VTerm Error receiving\n");
  return VTERM_NO_CONNECTED;
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
  bufferType = 2;

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
