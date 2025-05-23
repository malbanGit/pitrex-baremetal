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

#define MAX_STRING_SIZE 256
char currentMameName[MAX_STRING_SIZE];
unsigned char currentMameRefresh =0;
unsigned char currentMameFlags =0;
unsigned char currentMameOrientation =0;


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
#define DEBUG_START do {soffset=0;sbuffer[0]=0;} while (0)
#define DEBUG_END do {soffset=0;FILE_OUT(sbuffer);sbuffer[0]=0;} while (0)
#define DEBUG_OUT2(...) \
  do {\
  soffset+=snprintf(sbuffer+soffset, PRINT_BUFFER_SIZE-soffset,__VA_ARGS__);\
  } while(0)
*/  

#define DEBUG_START 
#define DEBUG_END 
#define DEBUG_OUT
#define DEBUG_OUT2
#define DEBUG_OUT3


//general: highest bit set -> then lowest 5 bits are a counter 
// 1xxc cccc
// 4 commands that can be countered
// xx:
// 00 - DOTS            1-31 times 2 values for x,y
// 01 - PATH LINES      1 times 4 values x0,y0,x1,y1 and 1-30 times 2 values for x2,y2
// 10 - CONT PATH LINES (continue path) 1-31 times 2 values for x2,y2
// 11 - SINGLE LINES    1-31 times 4 values for x0,y0,x1,y1

// highest bit not set - no counting
// 0xxx tttt 
// 8 possible commands (xxx), 4 bits of possible data for command
// as yet defined:
// 0000 ???? Wait Recal (???? should be 0000)
// 0001 cccc Set Brightness, we assume Vectrex has only 16 different brightness stages. Values are shifted 3 times to the left, thus lowest "real" brightness is "8" - which is pretty dim!
// 0010 ???? DOT Single Dot, 2 values for x,y
// 0011 ???? SINGLE LINE, 4 values for x0,y0,x1,y1
// 0100 ???? n/a -> this might be some "set" command to configue the game/piTrex
// 0101 ???? n/a -> this might be some "get" command to ask the PiTrex for information
// 0110 ???? n/a -> this might be a special command to "order" something, like a ZERO or a DEFLOK, which would then be specified in the ????
// 0111 ???? END_LIST (end of sending data to PiTrex), (???? should be 0000)
#define VTERM_WAIT_RECAL        (0b00000000)
#define VTERM_BRIGHNESS         (0b00010000) // 0001 xxxx no bit set, xxxx are brightness (value = 3 bit shift left) -> Color "black" (move) is a Wait Recal
#define VTERM_SINGLE_LINE       (0b00110000) // 0011 ???? SINGLE LINE, 4 values for x0,y0,x1,y1
#define VTERM_DOT               (0b00100000) // 0010 ???? DOT Single Dot, 2 values for x,y
#define VTERM_START_OF_PATH     (0b10100000) // 101 - PATH LINES        1 times 4 values x0,y0,x1,y1 and 1-30 times 2 values for x2,y2
#define VTERM_END_LIST          (0b01110000) // 0111 ???? END_LIST (end of sending data to PiTrex), (???? should be 0000)
#define VTERM_DOTS              (0b10000000) // 100  ???? DOTS collection of dots, count in command, then count pairs of X Y
#define VTERM_SINGLE_LINES      (0b11100000) // 111 - SINGLE LINES    1-31 times 4 values for x0,y0,x1,y1
  
  
void v_printString_scale(int8_t x, int8_t y, char* string, uint8_t scale, uint8_t size, uint8_t brightness);
int UARTGetBinaryData(int size, unsigned char *romBuffer);
void UARTGetString(char *buffer);

int getOne8BitValue()
{
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
  }
  return 1000; // anything > 255 is 'error'
} 

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

int tryToConnect()
{
  // non blocking function
  currentMameName[0]=0;
   
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
      if (intensity==10) intAdd =+1;
      v_printString_scale(-70, 10, "AWAITING MAME!", 10, 10, intensity);
      // DEBUG_OUT("Pi:VTerm connect timeout.\n");
      return VTERM_NO_CONNECTED;
    }
    
    int stringEnd=0; // make sure string is terminated with a "\n"
    while (RPI_AuxMiniUartReadPending())
    {
      char r = RPI_AuxMiniUartRead();
      if (r == '\n')
      {
        DEBUG_OUT("String end Found: %s\n, buffer");
        stringEnd = 1;
        break;
      }
      else 
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
    
    if ((stringEnd) && (strcmp("PiTrex VTerm Connect", buffer) ==0))
    {
      // send connected
      // force output to uart stdout     
      fprintf(stdout,"Pi:VTerm connect acknowledged.\n");
      DEBUG_OUT("Pi:VTerm connect acknowledged.\n");

      UARTGetString(currentMameName);
      // to upper
      char *str=currentMameName;
      while (*str) { *str = toupper((unsigned char)*str); str++;}
      
      currentMameRefresh = getOne8BitValue();
      currentMameFlags = getOne8BitValue();
      currentMameOrientation = getOne8BitValue(); 

      
      DEBUG_OUT("currentMameName: %s\n", currentMameName);
      DEBUG_OUT("currentMameRefresh: %i\n", currentMameRefresh);
      DEBUG_OUT("currentMameFlags: %i\n", currentMameFlags);
      DEBUG_OUT("currentMameOrientation: %i\n", currentMameOrientation);
      
      char herz[10];
      sprintf(herz, "%i",currentMameRefresh);
      
      for (int i=0;i<100;i++)
      {
        v_WaitRecal();
        v_printString_scale(-70, 50, "GAME: ", 10, 10, 70);
        v_printString_scale(0, 50, currentMameName, 10, 10, 70);
        v_printString_scale(-70, 0, "HZ:", 10, 10, 70);
        sprintf(herz, "%i",currentMameRefresh);
        v_printString_scale(0, 0, herz, 10, 10, 70);        
      }
      v_setClientHz(currentMameRefresh); // should be negotiated with mame
//      v_setRefresh(currentMameRefresh);


      DEBUG_OUT("Vectrex Herz set: %i\n",currentMameRefresh);
      return VTERM_CONNECTED;
    }
  } 
  return VTERM_NO_CONNECTED;
}

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

  clockStart = getOtherTimer();
  while (1)
  {
    while (RPI_AuxMiniUartReadPending())
    {
      unsigned char r = (unsigned char) RPI_AuxMiniUartRead();
      romBuffer[counter++] = r;
      if (counter == size)
      {
      DEBUG_OUT2("ok\n", (size-counter));
        return 0;
      }
    }
    if ((getOtherTimer()-clockStart)>timeOutDif)
    {
      DEBUG_OUT2("Timeout, missing: %i.\n", (size-counter));
      return (size-counter);
    }
  }
  return size+1; // NOK
}

unsigned char readBuffer[100000];
int readBufferLength=0;
#define NEXTBYTE readBuffer[readpos++]

// 8 bit only
int receiveData()
{
  vt_setTimerStart();
  int hi = getOne8BitValue();
  int lo = getOne8BitValue();
  int size = 256*hi+lo;
  soffset=0;
  DEBUG_OUT2("Expecting size: %i\n", size);
  
  vt_setTimerStart();
  vt_UARTGetBinaryDataChunk(size, readBuffer);

  DEBUG_OUT("Receive time: %f\n", vt_getTimerDifMilli());

  int readpos = 0;
  while (1)
  {
    if (readpos>size)
    {
      DEBUG_OUT2("Receive breakoff\n");
      return VTERM_CONNECTED;
    }
    
    uint32_t startTime = v_micros();
    DEBUG_OUT3("\n(%i): ", readpos, readBuffer[readpos]);
    DEBUG_OUT3(" $%02x ", readBuffer[readpos]);
    unsigned char r = NEXTBYTE;

    unsigned char command=0;
    unsigned char count=0;
    unsigned char data=0;
    if ((r & 0x80) == 0x80)
    {
      // counting
      count = r & 0b00011111;
      command = r & (0b11100000);
    }
    else
    {
      data = r & (0b00001111);
      command = r & (0b11110000);
    }
    
    
    switch (command)
    {
      case VTERM_BRIGHNESS:
      {
        data = data << 3;
        DEBUG_OUT2("INT ");
        DEBUG_OUT2(" $%02x ", data);
        termBrightness = data;
        break;
      }
      case VTERM_DOT:
      {
    DEBUG_OUT3("DOT ");
    DEBUG_OUT3(" $%02x ", readBuffer[readpos]);
        int x0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT3(" $%02x ", readBuffer[readpos]);
        int y0 = ((signed char)NEXTBYTE)*128;
        v_directDraw32(x0,y0,x0,y0, termBrightness);
      break;
      }
      case VTERM_DOTS:
      {
    DEBUG_OUT2("DOTS (%i)", count);
        count-=1; // first full vector definition is "part" of the count
    DEBUG_OUT3(" $%02x ", readBuffer[readpos]);
        int x0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT3(" $%02x ", readBuffer[readpos]);
        int y0 = ((signed char)NEXTBYTE)*128;
        v_directDraw32(x0,y0,x0,y0, termBrightness);
        for (int i=0;i<count;i++)
        {
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
          x0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
          y0 = ((signed char)NEXTBYTE)*128;
          v_directDraw32(x0,y0,x0,y0, termBrightness);
        }          
      break;
      }     
      case VTERM_SINGLE_LINE:
      {
    DEBUG_OUT2("LINE ");
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
        int x0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
        int y0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
        int x1 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
        int y1 = ((signed char)NEXTBYTE)*128;
        v_directDraw32(x0,y0,x1,y1, termBrightness);
      break;
      }
      case VTERM_SINGLE_LINES:
      {
    DEBUG_OUT2("LINES (%i)", count);
        count-=1; // first full vector definition is "part" of the count
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
        int x0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
        int y0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
        int x1 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
        int y1 = ((signed char)NEXTBYTE)*128;
        v_directDraw32(x0,y0,x1,y1, termBrightness);
        for (int i=0;i<count;i++)
        {
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
          int x0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
          int y0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
          int x1 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
          int y1 = ((signed char)NEXTBYTE)*128;
          v_directDraw32(x0,y0,x1,y1, termBrightness);
        }          
      break;
      }     
  
      
      
      
      case VTERM_START_OF_PATH:
      {
    DEBUG_OUT2("PATH (%i)", count);
        count-=1; // first full vector definition is "part" of the count
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
        int x0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
        int y0 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
        int x1 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
        int y1 = ((signed char)NEXTBYTE)*128;
        v_directDraw32(x0,y0,x1,y1, termBrightness);
        
        for (int i=0;i<count;i++)
        {
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
          int x2 = ((signed char)NEXTBYTE)*128;
    DEBUG_OUT2(" $%02x ", readBuffer[readpos]);
          int y2 = ((signed char)NEXTBYTE)*128;
          v_directDraw32(x1,y1,x2,y2, termBrightness);
          x1 = x2;
          y1 = y2;
        }          
        break;
      }
/*      
      case VTERM_DOT:
      {
//    DEBUG_OUT("DOT ");
//    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
        int x0 = ((signed char)NEXTBYTE)*128;
//    DEBUG_OUT(" $%02x ", readBuffer[readpos]);
        int y0 = ((signed char)NEXTBYTE)*128;
        v_directDraw32(x0,y0,x0,y0, termBrightness);
        break;
      }
*/      
      case VTERM_END_LIST:
      {
        DEBUG_OUT2("LIST END\n");
/*        
        After this -> a crash!
        stack or program corrupted? -> when full "debug" - at least at some time... - what do I know???
*/        
        return VTERM_CONNECTED;
      }
      
      default:
      {
        DEBUG_OUT("(%i) UNKOWN VTerm Command: %02x ignoring\n",readpos, r);
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
    RPI_AuxMiniUartWriteTimed(currentButtonState);
    RPI_AuxMiniUartWriteTimed(currentJoy1X);
    RPI_AuxMiniUartWriteTimed(currentJoy1Y);


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
        DEBUG_OUT2("WR gotten!\n");
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
//  v_removeIRQHandling();  
  
  #if RASPPI != 1 
    v_setupSMPHandling();
  #endif  
//  usePipeline = 1;
//  bufferType = 2;

  v_enableSerialCommands(0); // no disruption of serial communication with terminal access
        
  v_enableJoystickDigital(0,0,0,0);
  v_enableJoystickAnalog(1,1,0,0);
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
