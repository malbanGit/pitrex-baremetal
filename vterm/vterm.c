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

int receiveCount = 0;

#define PRINT_BUFFER_SIZE 100000
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

//#define DEBUG_START 
//#define DEBUG_END 
#define DEBUG_OUT
#define DEBUG_OUT2
#define DEBUG_OUT3
#define DEBUG_OUT4
#define DEBUG_OUT5

#define DEBUG_END2
#define DEBUG_END3


#define DEBUG_START do {soffset=0;sbuffer[0]=0;} while (0)
#define DEBUG_END do {soffset=0;FILE_OUT(sbuffer);sbuffer[0]=0;} while (0)
#define DEBUG_OUT2(...) \
  do {\
  soffset+=snprintf(sbuffer+soffset, PRINT_BUFFER_SIZE-soffset,__VA_ARGS__);\
  } while(0)
#define DEBUG_OUT3(...) \
  do {\
  soffset+=snprintf(sbuffer+soffset, PRINT_BUFFER_SIZE-soffset,__VA_ARGS__);\
  } while(0)
#define DEBUG_OUT4(...) \
  do {\
  soffset+=snprintf(sbuffer+soffset, PRINT_BUFFER_SIZE-soffset,__VA_ARGS__);\
  } while(0)


#define DEBUG_OUT
#define DEBUG_OUT2
#define DEBUG_OUT3
#define DEBUG_OUT4
  
  
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
  

// Function for packing/unpacking etc
int compare_buffers(const unsigned char* a, const unsigned char* b, int length);
int miniLZ4_compress(const unsigned char* src, int srcSize, unsigned char* dst, int dstCapacity);
int miniLZ4_decompress(const unsigned char* src, int compressedSize, unsigned char* dst, int dstCapacity);
int huffman_compress(const unsigned char* input, int length, unsigned char* output, int outMax);
int huffman_decompress(const unsigned char* input, int length, unsigned char* output, int outMax);
int dif   (const unsigned char* prevBuffer, int prevBufferLength, const unsigned char* src, int srcSize,        unsigned char* dst, int dstCapacity);
int undiff(const unsigned char* prevBuffer, int prevBufferLength, const unsigned char* src, int compressedSize, unsigned char* dst, int dstCapacity);


// -------------------
// generated after ~350 frames
const unsigned char lunar_huffman_bitlengths[256] = {
  4,   2,   3,   4,   6,   5,   7,   6,   9,   7,   7,   6,   7,   7,   7,   8,
 10,   8,   9,  10,   9,   8,   7,   7,   6,   7,   5,   5,   7,   8,   8,   8,
  9,   8,   9,   9,  10,  10,  10,  10,  10,   9,  10,  11,  10,  10,   9,   9,
  9,   9,  10,  10,   9,  10,  10,  10,  10,  10,  10,   9,  10,  11,  11,  12,
 11,   6,  10,  10,   9,   9,  10,   9,   9,  10,  10,  10,  10,   9,  10,   9,
  9,   9,   9,   9,  10,  10,   8,  10,  10,  10,  10,  10,   9,   9,   9,  10,
 11,  12,  12,  12,  12,  12,  12,  11,  10,  11,  10,  11,  12,  12,  12,  12,
 12,  12,  11,  11,  10,  10,  10,  10,  10,  11,   9,  12,   9,   6,  10,  10,
  9,  10,   8,   9,   8,  10,  12,  12,  12,  12,  11,  10,  11,  10,  11,  11,
 12,  12,  11,   9,   9,   9,  10,  10,   9,   9,   9,   9,   9,   9,   9,   9,
 10,  10,   9,   9,   9,  11,  12,  11,  12,  12,  12,  12,  12,  12,  12,  12,
 12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,
 12,  12,  12,  11,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  11,
 11,  11,  11,  10,  10,  10,  10,  10,  10,  10,   9,  10,  10,  10,  10,  10,
 10,  11,  11,  12,  12,  12,  11,  11,  11,  11,  12,  12,  12,  12,  12,  12,
 12,  12,  12,  12,  12,  11,  12,  11,  11,  11,   8,  10,  11,   9,   9,   7,
};
const unsigned char starwars_huffman_bitlengths[256] = {
  3,   3,   3,   5,   6,   5,   5,   5,   6,   6,   6,   7,   7,   7,   7,   8,
  8,   8,   8,   8,   8,   8,   6,   6,   8,   9,   9,   9,   8,   8,   9,   9,
  7,   9,   9,   9,   9,   9,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
  9,  10,  10,  10,  10,  10,  10,  11,  10,  11,  11,  10,  11,  10,  10,   9,
 10,  10,  11,  11,  11,  11,  11,  11,  11,  10,  11,  11,  11,  11,  11,  11,
 11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  10,  11,  11,  11,  11,  11,
 11,  11,  11,  10,  10,  11,  10,  10,  11,  11,  11,  11,  10,  10,  11,  11,
  9,  11,  11,  11,  11,  10,  11,  11,  10,  10,  11,  10,  11,   8,  11,  11,
 10,  10,  10,  10,  10,  11,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
 10,  10,  10,  10,  11,  10,  10,  10,  11,  10,  10,  10,  11,  10,  11,  10,
 10,  11,   8,   9,  10,  10,  10,   9,  10,  10,  11,  10,  10,  10,  10,  11,
 11,  11,  11,  11,  11,  11,  11,  10,  11,  11,  11,  11,  11,  11,  11,  10,
 11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  10,  10,  10,  10,
 10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
 10,  10,   9,  10,   9,  10,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,
  9,   9,   9,   8,   8,   8,   8,   8,   8,   7,   7,   7,   6,   6,   6,   5,

};







unsigned char recieveBuffer[20000];
unsigned char unpackBuffer[20000];
signed char readBuffer[100000];
int readBufferLength=0;
int diffError = 0;
#define NEXTBYTE readBuffer[readpos++]


// DIFF
static int hasPrevBuffer = 0;
static unsigned char prevBuffer[100000];
static int prevBufferLength = 0;

// Bit 0b0000 ddmm : mm 00 = full data, 01 = half delta, 10 = full delta
//                   dd 00 = no dif, 01 = dif (previous buffer must be available), 10 = dif with Huffman (fixed, depending on game - or general)
//                   Hufman is chosen in the beginning with MAME communication  
int dataMode = 0;
unsigned char *fixedHuffman=NULL; // pointer to game dependend Huffman dictionary
  

  
  
  
void v_printString_scale(int8_t x, int8_t y, char* string, uint8_t scale, uint8_t size, uint8_t brightness);
int UARTGetBinaryData(int size, unsigned char *romBuffer);
void UARTGetString(char *buffer);







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
// 0 != error
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
    unsigned char data;
    while (readUart(&data))
    {
      romBuffer[counter++] = data;
      if (counter == size)
      {
        DEBUG_OUT2("ok: %i, %i\n", (size-counter), counter);
        return 0;
      }
    }
    if ((getOtherTimer()-clockStart)>timeOutDif)
    {
      FILE_OUT("Timeout, missing: %i.\n", (size-counter));
      return (size-counter);
    }
  }
  return size+1; // NOK
}



// only one ACK
int mrp_receive_blocknotwokring(unsigned char* buffer, int maxlen, int* out_len) 
{
    int expected_len = 0;
    unsigned char retries = 0;
    while (retries++ < MRP_MAX_RETRIES)  
    {
        unsigned char crc; 
        unsigned char lo;
        unsigned char hi;

        getByteBlocking(&hi);
        getByteBlocking(&lo);

        getByteBlocking(&crc);
        expected_len = hi*256+lo;

// TODO!!!        es "verschwinden" daten beim ersten try - mitten in dem "getBytes"
/*        
        for (int i=0; i< expected_len; i++)
        {
          getByteBlocking(buffer+i);
        }
*/        
        sendByte(MRP_ACK); 
DEBUG_OUT4("Stupid Ack Sent\n");
        
        // Read chunk payload
        if (!getBytes2(buffer, expected_len))
        {
DEBUG_OUT4("length: %i, crc: %i\n", expected_len, crc);
DEBUG_OUT5("GetBytes = false (%i)\n", expected_len);
/*
for (int i=0;i<expected_len;i++)
  DEBUG_OUT4("$%02x ", (unsigned char) buffer[i]);
*/
DEBUG_OUT4("\n");
DEBUG_END3;
  

            sendByte(MRP_NACK); 
            continue;
        }
        
        
DEBUG_OUT4("length: %i, crc: %i\n", expected_len, crc);
DEBUG_OUT4("GetBytes = true\n");
        
        // CRC check
        if (crc != crc8(buffer, expected_len)) 
        {
DEBUG_OUT5("Error: CRC mismatch %i != %i\n", crc, crc8(buffer, expected_len));
DEBUG_END3;
            sendByte(MRP_NACK); 
            continue;
        }
        sendByte(MRP_ACK); 
DEBUG_OUT2("Receive block successfull!...\n");
DEBUG_END2;
        *out_len = expected_len;
        return 1;
    }
DEBUG_OUT5("Error: Max retries reached...\n");
DEBUG_END3;
  diffError = 1;
  return 0;
}

// 0 error
// 1 success
// very secure and clean
// but slow, each ACK takes about 15ms
// 1500 byte = 12*15ms
// VERY SLOW!
int mrp_receive_block(unsigned char* buffer, int maxlen, int* out_len) {
    unsigned char seq = 0;
    int offset = 0;
    int expected_len = 0;
    unsigned char retries = 0;
    while (retries++ < MRP_MAX_RETRIES)  
    {
        unsigned char b;

        // Wait for Start Byte
        do 
        {
            getByteBlocking(&b);
//DEBUG_OUT4("Byte received: $%02x\n", b);
        } while (b != MRP_START_DATA);
 
        // Read packet header
        unsigned char pkt_seq, chunk_len, crc;
        getByteBlocking(&pkt_seq);
DEBUG_OUT4("Seq: %i\n", pkt_seq);
        getByteBlocking(&chunk_len);
DEBUG_OUT4("chunk_len: %i\n", chunk_len);
        getByteBlocking(&crc);
DEBUG_OUT4("crc: %i\n", crc);

/*
 * Opposite is not expecting to get ACK or NACK
        if (chunk_len > MRP_CHUNK_SIZE) 
        {
DEBUG_OUT4("Chunk Len to large!\n");
            sendByte(MRP_NACK); 
            sendByte(pkt_seq);
            continue;
        }
*/
        // Read chunk payload
        unsigned char chunk[MRP_CHUNK_SIZE];
        if (!getBytes(chunk, chunk_len))
        {

DEBUG_OUT5("GetBytes = false (%i)\n", chunk_len);
DEBUG_END3;
          sendByte(MRP_NACK); sendByte(pkt_seq);
            continue;
        }
DEBUG_OUT4("GetBytes = true\n");
        
        // CRC check
        if (crc != crc8(chunk, chunk_len)) {
DEBUG_OUT5("Error: CRC mismatch %i != %i\n", crc, crc8(chunk, chunk_len));
DEBUG_END3;
          sendByte(MRP_NACK); sendByte(pkt_seq);
            continue;
        }

        // Sequence check
        if (pkt_seq != seq) {
DEBUG_OUT5("Error: Sequence mismatch %i != %i\n", pkt_seq, seq);
DEBUG_END3;
          sendByte(MRP_NACK); sendByte(pkt_seq);
            continue;
        }
        if (seq == 0) {
            // Extract total expected length from first two bytes
            expected_len = ((int)chunk[0] << 8) | chunk[1];
            if (expected_len > maxlen) 
            {
DEBUG_OUT5("Error: Data would overflow buffer...\n");
DEBUG_END3;
              return 0;
            }

            unsigned char data_bytes = chunk_len - 2;
            for (unsigned char i = 0; i < data_bytes; i++)
                buffer[i] = chunk[2 + i];
            offset = data_bytes;
        } 
        else 
        {
            if (offset + chunk_len > expected_len) 
            {
DEBUG_OUT5("Error: More data than expected... %i != %i\n", offset + chunk_len , expected_len);
DEBUG_END3;
              return 0;
            }
            for (unsigned char i = 0; i < chunk_len; i++)
                buffer[offset + i] = chunk[i];
            offset += chunk_len;
        }

        sendByte(MRP_ACK); sendByte(seq);
        seq++;

        if (offset >= expected_len) 
        {
DEBUG_OUT2("Receive block successfull!...\n");
DEBUG_END2;
            *out_len = expected_len;
            return 1;
        }
        retries = 0;
        continue;
    }
DEBUG_OUT5("Error: Leaving Receive without a cause...\n");
DEBUG_END3;
  return 0;
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
    while (RPI_AuxUartReadPending())
    {
      char r = RPI_AuxUartRead();
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

      
      if (strcasecmp(currentMameName, "llander")) 
          fixedHuffman= (uint8_t *)lunar_huffman_bitlengths;
      if (strcasecmp(currentMameName, "starwars")) 
          fixedHuffman= (uint8_t *)starwars_huffman_bitlengths;
      

      DEBUG_OUT("Vectrex Herz set: %i\n",currentMameRefresh);
      return VTERM_CONNECTED;
    }
  } 
  return VTERM_NO_CONNECTED;
}

// 8 bit only
int receiveData()
{
  vt_setTimerStart();


// Bit 0b0000 ddmm : mm 00 = full data, 01 = half delta, 10 = full delta
//                   dd 00 = no dif, 01 = dif (previous buffer must be available), 10 = dif with Huffman (fixed, depending on game - or general)
//                   Hufman is chosen in the beginning with MAME communication  
  dataMode = getOne8BitValue();
  int hi = getOne8BitValue();
  int lo = getOne8BitValue();
  
  int lineMode = dataMode&0b0000011;
  int size = 256*hi+lo;
  soffset=0;
  DEBUG_OUT4("Data mode: %i\n", dataMode);
  DEBUG_OUT4("Expecting size: %i\n", size);
  
  vt_setTimerStart();
  unsigned char *finalBuffer=readBuffer;

  if ((dataMode & 0b00001100) == 0)
  {
    DEBUG_OUT4("Expecting unpacked data\n");
    // not packed
    vt_UARTGetBinaryDataChunk(size, readBuffer);
  }
  else
  {
    unsigned char *workBuffer=recieveBuffer;
    int workSize = size;
    DEBUG_OUT4("Expecting packed data (%i)\n",size );
    int loadDif = vt_UARTGetBinaryDataChunk(size, recieveBuffer);
    DEBUG_OUT4("Packed data received: %i\n", loadDif);

DEBUG_OUT4("\n<%04x: ", 0);
    for (int i=0;i<size; i++) 
{
  DEBUG_OUT4("$%02x ", (unsigned char) recieveBuffer[i]);
  if (((i+1)%16) == 0) {DEBUG_OUT4("\n<%04x: ", i+1);DEBUG_END;}
}

    
    
       // packed
    if ((dataMode & 0b00001000) == 0b00001000)
    {
       // Huffman
       workSize = huffman_decompress((const unsigned char*)recieveBuffer, size, (unsigned char *)unpackBuffer, 100000);
       workBuffer=unpackBuffer;
       DEBUG_OUT4("UnHuf Size: %i\n", workSize);
       size = workSize;
       finalBuffer=workBuffer;
       
       // attention!
       // JUST Huffman ist not supported!
       // readbuffer is not filled!
    }
    if ((dataMode & 0b00000100) == 0b00000100)
    {
       // undiff
        int undiffedSize = undiff(
            (const unsigned char*) prevBuffer, prevBufferLength,
            (const unsigned char*) workBuffer, workSize, 
            (unsigned char*) readBuffer, 100000);
        DEBUG_OUT4("\nUNDIF Size: %i\n", undiffedSize);
        size = undiffedSize;
        finalBuffer=readBuffer;
        
        if (undiffedSize<0)
        {
          FILE_OUT("Diff Error!\n");
          diffError = 1;
          DEBUG_OUT4("Prev Buffer\n");
          for (int i=0;i<prevBufferLength; i++) 
          {
            DEBUG_OUT4("$%02x ", (unsigned char) prevBuffer[i]);
            if (((i+1)%16) == 0) DEBUG_OUT4("\n");
          }

          DEBUG_OUT4("\nReceived (worksize = %i)\n", workSize);
DEBUG_OUT4("\n>%04x: ", 0);
          for (int i=0;i<workSize; i++) 
          {
            DEBUG_OUT4("$%02x ", (unsigned char) recieveBuffer[i]);
            if (((i+1)%16) == 0) {DEBUG_OUT4("\n>%04x: ", i+1);DEBUG_END;}
          }
          
          DEBUG_OUT2("DIF breakoff\n");
          DEBUG_END;
          prevBufferLength = 0;
          return VTERM_CONNECTED;
        }
    }
  }

  DEBUG_OUT2("\nReceive time: %f\n", vt_getTimerDifMilli());

  // previous buffer is always the real big one thing!
  for (int i=0;i<size; i++) prevBuffer[i] = finalBuffer[i];
  prevBufferLength = size;
  hasPrevBuffer = 1;
  
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
    signed char r = NEXTBYTE;

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
    signed char x0=0;
    signed char x1=0;
    signed char y0=0;
    signed char y1=0;
// 0 = default line with xy coordinates
// 1 = line delta, second coordinate as delta
// 2 = full delta, even start coordinates as delta from the last known position

    
//    something still wrong with LINE_DATA_TYPE = 1
//    LINE_DATA_TYPE = 2 untested


    signed char lx=0;
    signed char ly=0;

// each of these must leave with
// x0, y0 absolute coordinates!    
#define RECEIVE_COORD0 \
do{ \
    if (lineMode==0) \
    { \
        x0 = ((signed char)(NEXTBYTE));               \
        y0 = ((signed char)(NEXTBYTE));  \
        DEBUG_OUT3(" $%02x ", x0);               \
        DEBUG_OUT3(" $%02x ", y0);               \
    } \
    else if (lineMode==1) \
    { \
        x0 = ((signed char)(NEXTBYTE));               \
        y0 = ((signed char)(NEXTBYTE));  \
        DEBUG_OUT3(" $%02x ", x0);               \
        DEBUG_OUT3(" $%02x ", y0);               \
        lx = x0;                                           \
        ly = y0;                                           \
    } \
    else if (lineMode==2) \
    { \
        x0 = ((signed char)(NEXTBYTE));               \
        y0 = ((signed char)(NEXTBYTE));  \
        DEBUG_OUT3(" $%02x ", x0);               \
        DEBUG_OUT3(" $%02x ", y0);               \
        x0 = ((signed char)(x0+lx));               \
        y0 = ((signed char)(x0+ly));  \
        lx = x0;                                           \
        ly = y0;                                           \
    } \
}while (0)

#define RECEIVE_COORD1 \
do{ \
    if (lineMode==0) \
    { \
        x1 = ((signed char)(NEXTBYTE));               \
        y1 = ((signed char)(NEXTBYTE));  \
        DEBUG_OUT3(" $%02x ", x1);               \
        DEBUG_OUT3(" $%02x ", y1);               \
    } \
    else if (lineMode==1) \
    { \
        x1 = ((signed char)(NEXTBYTE));               \
        y1 = ((signed char)(NEXTBYTE));  \
        DEBUG_OUT3(" $%02x ", x1);               \
        DEBUG_OUT3(" $%02x ", y1);               \
        x1 = ((signed char)(lx+x1));               \
        y1 = ((signed char)(ly+y1));  \
    } \
    else if (lineMode==2) \
    { \
        x1 = ((signed char)(NEXTBYTE));               \
        y1 = ((signed char)(NEXTBYTE));  \
        DEBUG_OUT3(" $%02x ", x1);               \
        DEBUG_OUT3(" $%02x ", y1);               \
        x1 = ((signed char)(lx+x1));               \
        y1 = ((signed char)(ly+y1));  \
        lx = x1;                                           \
        ly = y1;                                           \
    } \
}while (0)
    
   
    
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
        RECEIVE_COORD0;
        x1=x0;
        y1=y0;
        v_directDraw32(x0*128,y0*128,x1*128,y1*128, termBrightness);
      break;
      }
      case VTERM_DOTS:
      {
        DEBUG_OUT2("DOTS (%i)", count);
        count-=1; // first full vector definition is "part" of the count
        RECEIVE_COORD0;
        x1=x0;
        y1=y0;
        v_directDraw32(x0*128,y0*128,x1*128,y1*128, termBrightness);
        for (int i=0;i<count;i++)
        {
          RECEIVE_COORD0;
          x1=x0;
          y1=y0;
          v_directDraw32(x0*128,y0*128,x1*128,y1*128, termBrightness);
        }          
      break;
      }     
      case VTERM_SINGLE_LINE:
      {
        DEBUG_OUT2("LINE ");
        RECEIVE_COORD0;
        RECEIVE_COORD1;
        v_directDraw32(x0*128,y0*128,x1*128,y1*128, termBrightness);
      break;
      }
      case VTERM_SINGLE_LINES:
      {
        DEBUG_OUT2("LINES (%i)", count);
        count-=1; // first full vector definition is "part" of the count
        RECEIVE_COORD0;
        RECEIVE_COORD1;
        v_directDraw32(x0*128,y0*128,x1*128,y1*128, termBrightness);
        for (int i=0;i<count;i++)
        {
          RECEIVE_COORD0;
          RECEIVE_COORD1;
          v_directDraw32(x0*128,y0*128,x1*128,y1*128, termBrightness);
        }          
      break;
      }     
      case VTERM_START_OF_PATH:
      {
        DEBUG_OUT2("PATH (%i)", count);
        count-=1; // first full vector definition is "part" of the count
        RECEIVE_COORD0;
        RECEIVE_COORD1;
        v_directDraw32(x0*128,y0*128,x1*128,y1*128, termBrightness);
        
        for (int i=0;i<count;i++)
        {
          x0=x1;
          y0=y1;
          RECEIVE_COORD1;
          v_directDraw32(x0*128,y0*128,x1*128,y1*128, termBrightness);
        }          
        break;
      }
      case VTERM_END_LIST:
      {
        DEBUG_OUT2("LIST END\n");
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
  
  
  FILE_OUT("VTerm Error receiving\n");
  return VTERM_NO_CONNECTED;
}




int waitRecalGot = 0;
int waitRecalFailed = 0;
#define _ERR_ 1
#define _ACK_ 255

int waitForStartSignal()
{
  uint32_t startTime = v_micros();
  while (1)
  {
    if (diffError)
    {
      UARTWriteTimed(_ERR_, TIME_OUT_VALUE);
      diffError = 0;
    }
    else
    {
      UARTWriteTimed(_ACK_,TIME_OUT_VALUE);
    }
    
    
    UARTWriteTimed(currentButtonState, TIME_OUT_VALUE);
    UARTWriteTimed(currentJoy1X, TIME_OUT_VALUE);
    UARTWriteTimed(currentJoy1Y, TIME_OUT_VALUE);


    int v = getOne8BitValueTimed(TIME_OUT_VALUE);

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
 
  
//  v_setupIRQHandling();
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
  v_setCustomClipping(1, -16000, -15000, 14400, 16000);
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
    DEBUG_OUT2("Round End!\n");
    DEBUG_END;
  }
  return 0;
}
#include "unpacker.c"
