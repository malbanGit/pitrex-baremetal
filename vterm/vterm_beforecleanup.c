/*
entweder ein signal senden
"Bereits für WR"
und/oder

max Vectoren angeben

eventuell mit "selbst Snchro" wieviel "passen"



expected time passt nun sehr genau
Ab und zu gehen 1-9 byte verloren, aber nicht wgen zu wenig Zeit (doppelte Zeit sollte MEHR als aucreichend sein)
Frage:
Überlauf?


Nicht gesendet
Sonstwie verloren gegangen? 
Anderes

-> Antwort es ist das Interrupt driven display!
  

*/



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

#define PRINT_BUFFER_SIZE 10000
static char sbuffer[PRINT_BUFFER_SIZE];
int soffset = 0;

/*
static char _outbuffer[PRINT_BUFFER_SIZE];
#define DEBUG_OUT(...) do \
  { \
    sprintf(_outbuffer,__VA_ARGS__);\
    { \
      if (!__debugf) {__debugf = fopen("../../../printf.log", "a"); \
        {if (!__debugf) __debugf = fopen("../../printf.log", "a"); \
          {if (!__debugf) __debugf = fopen("../printf.log", "a"); \
              if (!__debugf) __debugf = fopen("printf.log", "a"); }}}\
      if (__debugf) \
      { \
        fprintf(__debugf,_outbuffer); \
        fflush(__debugf); \
      } \
    }\
  } while(0)
*/

#define FILE_OUT(...) do \
  { \
    { \
      if (!__debugf) {__debugf = fopen("../../../printf.log", "a"); \
        {if (!__debugf) __debugf = fopen("../../printf.log", "a"); \
          {if (!__debugf) __debugf = fopen("../printf.log", "a"); \
              if (!__debugf) __debugf = fopen("printf.log", "a"); }}}\
      if (__debugf) \
      { \
        fprintf(__debugf,__VA_ARGS__); \
        fflush(__debugf); \
      } \
    }\
  } while(0)

/*
#define DEBUG_START do {soffset=0;sbuffer[0]=0;} while (0)
#define DEBUG_END do {soffset=0;FILE_OUT(sbuffer);sbuffer[0]=0;} while (0)
#define DEBUG_OUT(...) \
  do {\
  soffset+=snprintf(sbuffer+soffset, PRINT_BUFFER_SIZE-soffset,__VA_ARGS__);\
  } while(0)
*/

#define DEBUG_START 
#define DEBUG_END 
#define DEBUG_OUT
  

  
  
void v_printString_scale(int8_t x, int8_t y, char* string, uint8_t scale, uint8_t size, uint8_t brightness);

int tryToConnect()
{
  // non blocking function
  #define MAX_STRING_SIZE 256
   
  int ret = -1; // error
  int counter = 0;
  char buffer[MAX_STRING_SIZE]; // Array to store the converted string
  buffer[0] = 0;

  uint32_t startTime = v_micros();
  
  static int intensity = 20;  
  static int intAdd = 1;  
  while (1)
  {
    if (v_micros()-startTime>(ONCE_PER_ROUND/2)) 
    {
      intensity+=intAdd;
      if (intensity==127) intAdd =-1;
      if (intensity==30) intAdd =+1;
      v_printString_scale(-70, 10, "AWAITING MAME!", 10, 10, intensity);
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
  {
    romBuffer[counter++] = RPI_AuxMiniUartReadInlined();
  }
}

int UARTGetBinaryData(int size, unsigned char *romBuffer);



/* TIMING */
#define __CLOCKS_PER_SEC__ ((uint64_t)1000000) // in microseconds
unsigned long long getOtherTimer(); // pi_support.c
uint64_t clockCurrent;
uint64_t clockStart;
void inline vt_setTimerStart()
{
  clockStart = getOtherTimer();
}
uint64_t inline vt_getTimerDifMicro()
{
  clockCurrent = getOtherTimer();
  return clockCurrent-clockStart;
}
double inline vt_getTimerDifMilli()
{
  return ((double)vt_getTimerDifMicro())/((double)1000.0);
}
double inline vt_getTimerDifSeconds()
{
  return ((double)vt_getTimerDifMilli())/((double)1000.0);
}
/* TIMING END*/

// 0 = ok
// 1 = error
int vt_UARTGetBinaryDataChunk(int size, unsigned char *romBuffer)
{
  int counter = 0; 
  uint64_t clockStart;
  uint64_t expectedTime = __CLOCKS_PER_SEC__ * ((uint64_t)(size*10)); // 10 bits, 8 data, 1 start, 1 stop bit
  expectedTime = expectedTime / ((uint64_t)912600); // size is in bytes -> bits, speed of serial is 912600 baud -> bits per second
  uint64_t timeOutDif = expectedTime*1.1f; // tolerance

//  DEBUG_OUT("(");
  clockStart = getOtherTimer();
  while (1)
  {
    while (RPI_AuxMiniUartReadPending())
    {
      unsigned char r = (unsigned char) RPI_AuxMiniUartRead();
//      DEBUG_OUT("$%02x ",r);
      romBuffer[counter++] = r;
      if (counter == size)
      {
//        DEBUG_OUT(")\n");
        return 0;
      }
    }
    if ((getOtherTimer()-clockStart)>timeOutDif)
    {
//      DEBUG_OUT(")\n");
      DEBUG_OUT("Pi:Recieve Timeout, Missing: %i.\n", (size-counter));
      return (size-counter);
    }
  }
  return size+1; // NOK
}

#define CHUNKSIZE 50
int vt_UARTGetBinaryData(int size, unsigned char *romBuffer)
{
  int chunkSize = CHUNKSIZE;
  while (size>0)
  {
    if (chunkSize>size) chunkSize = size;

//    while (vCritical); // interrupt changes critical
//    DisableInterrupts();
    vt_UARTGetBinaryDataChunk(chunkSize, romBuffer);
//    EnableInterrupts(); // function defined in out baremetal.S
    
    romBuffer+=chunkSize;
    size -=chunkSize;
  }
}

unsigned char readBuffer[100000];
int readBufferLength=0;
//#define NEXTBYTE() getOne8BitValue()
#define NEXTBYTE readBuffer[readpos++]

// 8 bit only
int receiveData()
{
  vt_setTimerStart();
  int hi = getOne8BitValue();
  int lo = getOne8BitValue();
  int size = 256*hi+lo;
  soffset=0;
  DEBUG_OUT("Expecting size: %i\n", size);
  DEBUG_END;
  
/*
//DEBUG_OUT("getBulkSize (%i) time: %f\n",size,vt_getTimerDifMilli());
// never a problem
//  soffset+=sprintf(sbuffer+soffset, "getBulkSize (%i) time: %f\n",size,vt_getTimerDifMilli());

//DEBUG_OUT("Reading Bulk Data: %f\n", size);
  vt_UARTGetBinaryData(size, readBuffer);
//DEBUG_OUT("Read Bulk time: %f\n",vt_getTimerDifMilli());
  soffset+=sprintf(sbuffer+soffset, "Read Bulk time (Size: %i): %f\n",size,vt_getTimerDifMilli());

  
DEBUG_OUT("%s",sbuffer);
*/  
  
  /*  
    RPI_AuxMiniUartWrite('k');
DEBUG_OUT("Ack sent.\n");
  DEBUG_OUT("\n%i: ", 0);
  for (int i=0;i<size; i++)
  {
    DEBUG_OUT("$%02x ",readBuffer[i]);
    if (((i+1)%16) == 0) DEBUG_OUT("\n%i: ", i);
  }
DEBUG_OUT("\n");
*/
//  return VTERM_CONNECTED;
/*
for(int i=0;i<size;i++)
{
//    while (vCritical); 
//  DisableInterrupts();
  readBuffer[i]=getOne8BitValue();
//  EnableInterrupts(); // function defined in out baremetal.S
}

for(int i=0;i<size;i++)
{
//    while (vCritical); 
//  DisableInterrupts();
  readBuffer[i]=getOne8BitValue();
//  EnableInterrupts(); // function defined in out baremetal.S
}

*/
// 16 byte with 10 bits each takes at 912000 baud about: 175.44 μs ->  263 Vectrex cycles


//v_irqForceBreakMikroSeconds(20000);
//  int frame = 0;
  vt_setTimerStart();
  
  for(int i=0;i<size;)
  {
    int len = 16;
    if (i+len>size) len = size-i;
len = size;

  //  vt_UARTGetBinaryData(len, readBuffer+i);
    //vt_UARTGetBinaryData(len, readBuffer+i);
    int success = 0;
    do 
    {
      
//      char inBuffer[16+1];
//      success = vt_UARTGetBinaryDataChunk(len+1, inBuffer);
//      success = 
      vt_UARTGetBinaryDataChunk(len, readBuffer+i);

/*      
      if (success!=0) // nok
      {
        DEBUG_OUT("Failed chunk load(pos=%i, len=%i)\n",i, len+1);
        RPI_AuxMiniUartWrite(frame & 0x7f);
      } 
      else 
      {
        if (inBuffer[0] != (frame&0xff))
        {
          DEBUG_OUT("Frame signature fail (got=%i, expected=%i)\n",inBuffer[0]&0xff, (frame&0xff));
          RPI_AuxMiniUartWrite(frame & 0x7f);
          success=1; // fail
        }
        else
        {
          for (int ii=0;ii<len;ii++)
            readBuffer[i+ii]=inBuffer[ii+1];
          
          DEBUG_OUT("Chunk load ok, pos %i, len: %i.\n",i, len);
          RPI_AuxMiniUartWrite(frame | 0x80); // ok
          frame++;
        }
      }
*/      
    } while (success!=0);
    i = i+len;
  }
//v_irqForceBreakMikroSeconds(0);

  DEBUG_END;
  DEBUG_OUT("Receive time: %f\n", vt_getTimerDifMilli());

  int readpos = 0;
  while (1)
  {
    DEBUG_END;
    uint32_t startTime = v_micros();
    DEBUG_OUT("\n(%i): ", readpos, readBuffer[readpos]);
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
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
    DEBUG_OUT("INT ");
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
        termBrightness = (NEXTBYTE)&255;
        break;
      }
      case VTERM_SINGLE_LINE:
      {
    DEBUG_OUT("LINE ");
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
        int x0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
        int y0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
        int x1 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
        int y1 = ((signed char)NEXTBYTE)*128;
        v_directDraw32(x0,y0,x1,y1, termBrightness);
      break;
      }
      case VTERM_START_OF_PATH:
      {
    DEBUG_OUT("PATH ");
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
        int count = NEXTBYTE-1;
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
        int x0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
        int y0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
        int x1 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
        int y1 = ((signed char)NEXTBYTE)*128;
        v_directDraw32(x0,y0,x1,y1, termBrightness);
        
        for (int i=0;i<count;i++)
        {
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
          int x2 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
          int y2 = ((signed char)NEXTBYTE)*128;
          v_directDraw32(x1,y1,x2,y2, termBrightness);
          x1 = x2;
          y1 = y2;
        }          
        break;
      }
      
      case VTERM_DOT:
      {
    DEBUG_OUT("DOT ");
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
        int x0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
        int y0 = ((signed char)NEXTBYTE)*128;
        v_directDraw32(x0,y0,x0,y0, termBrightness);
        break;
      }
      case VTERM_END_LIST:
      {
        DEBUG_OUT("LIST END\n");
        DEBUG_END;
/*        
        After this -> a crash!
        stack or program corrupted?
*/        
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


int getOne8BitValueTimed()
{
  uint32_t startTime = v_micros();
  while (1)
  {
    while (RPI_AuxMiniUartReadPending())
    {
      return (unsigned char) RPI_AuxMiniUartRead();
    }
    if (v_micros()-startTime>TIME_OUT_VALUE)
    {
      break;
    }
  }
  return 1000; // anything > 255 is 'error'
} 

void RPI_AuxMiniUartWriteTimed(char c)
{
    uint32_t startTime = v_micros();
    /* Wait until the UART has an empty space in the FIFO */
    while( ( auxillary->MU_LSR & AUX_MULSR_TX_EMPTY ) == 0 ) 
    { 
      if (v_micros()-startTime>TIME_OUT_VALUE)
      {
        DEBUG_OUT("Write Byte timeout!\n");
        return;
      }
    }
    /* Write the character to the FIFO for transmission */
    auxillary->MU_IO = c;
}

int waitRecalGot = 0;
int waitRecalFailed = 0;
int waitForStartSignal()
{
  uint32_t startTime = v_micros();
  while (1)
  {
    RPI_AuxMiniUartWriteTimed('k');
    int v = getOne8BitValueTimed();

    // trying to detect MAME shutdown
    if (v==1000)
    {
      DEBUG_OUT("WR Read Timeout!\n");
      DEBUG_END;
      if (waitRecalGot == 1) waitRecalFailed++;
      if (waitRecalFailed>20) return VTERM_NO_CONNECTED;
      continue;
    }
    unsigned char r = (unsigned char) v;
    switch (r)
    {
      case VTERM_WAIT_RECAL:
      {
        waitRecalGot = 1;
        receiveCount = 0;
        DEBUG_OUT("WR gotten!\n");
        DEBUG_END;
        return VTERM_WAIT_FOR_DATA;
      }
      default:
      {
        DEBUG_OUT("VTerm Waiting for Start - unexpected Signal $%02x\n", r);
        DEBUG_END;
        //return VTERM_NO_CONNECTED;
        break;
      }
    }
    
    if (v_micros()-startTime>TIME_OUT_VALUE)
    {
      DEBUG_OUT("VTerm Waiting for Start - Timeout ignoring\n");
      DEBUG_END;
   //   return VTERM_NO_CONNECTED;
    }
  }
  DEBUG_OUT("VTerm Error receiving\n");
  return VTERM_NO_CONNECTED;
}
int main(int argc, char **argv) 
{
  DEBUG_START;
  vectrexinit(1);
  v_init();
  v_noSound();
  uartOutput=0; // MUST BE ZERO
  logOutput=1; // if you want ANY logging, then only to file!

  
  v_setRefresh(50);
  v_setClientHz(50); // should be negotiated with mame
  
  
  v_setupIRQHandling();
  v_removeIRQHandling();  
  
  #if RASPPI != 1 
    v_setupSMPHandling();
  #endif  
//  usePipeline = 1;
//  bufferType = 2;

  v_enableSerialCommands(0); // no disruption of serial communication with terminal access
        
  v_enableJoystickDigital(0,0,0,0);
  v_enableJoystickAnalog(0,0,0,0);
  v_enableSoundOut(0);
  v_enableButtons(1); // allow "return to pi"
  useDoubleTimer = 1;
  v_setCustomClipping(1, -16000, -14000, 14400, 15000);
  keepDotsTogether = 1;

  int pv = 0;
  if (pv = ini_parse("vterm.ini", iniHandler, 0) < 0) 
  {
    DEBUG_OUT("vterm.ini not loaded!\n\r");
  }
  else
    DEBUG_OUT("vterm.ini loaded!\n\r");

  
  DEBUG_END;
  int state = VTERM_NO_CONNECTED;
  while (1)
  {
    DEBUG_START;
    v_WaitRecal();
    if (state == VTERM_NO_CONNECTED)
    {
      waitRecalGot = 0;
      waitRecalFailed = 0;
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
    DEBUG_OUT("Round End!\n");
    DEBUG_END;
  }
  return 0;
}
