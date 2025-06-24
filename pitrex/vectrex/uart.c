
// TODO!!!

#define DEBUG_OUT1
#define DEBUG_OUT2
#define DEBUG_OUT3
#define DEBUG_OUT4
#define DEBUG_OUT5

#define DEBUG_END2
#define DEBUG_END3











////////////////////////////////////////////////////////////////





#define MAX_STRING_SIZE 256
#define MAX_FILE_SIZE (1024*1024*10) // 10 MB Max
unsigned char data[MAX_FILE_SIZE]; 

// in nano seconds 
//uint64_t getSystemTimerNano(); // NOT WORKING!
//void RPI_WaitMicroSeconds( uint32_t us );
// #define __CLOCKS_PER_SEC__ ((uint64_t)1000000000)
#define __CLOCKS_PER_SEC__ ((uint64_t)1000000) // in microseconds
unsigned long long getOtherTimer(); // pi_support.c


////////////////////////////////////////////////////////////////
/*
 * COMMAND UART functions
 * File Transfer ETC
 */
////////////////////////////////////////////////////////////////
// blocking function
// terminated ("\n") ascii number 
// 0 = success
// -1 = error
int UARTGetAsciiInt()
{
  // expect fileSize in ASCII + return
  int ret = -1; // error
#define MAXINT_CHAR_SIZE 20
  char buffer[MAXINT_CHAR_SIZE];
  int counter = 0;
  buffer[0] = 0;
  
  while (1)
  {
    while (RPI_AuxUartReadPending())
    {
      //printf("Command reading UART INT\n");

      char r = RPI_AuxUartRead();
      
      
	if (r != '\n')
	{
	    if (counter<MAXINT_CHAR_SIZE-1)
	    {
		buffer[counter++] = r;
		buffer[counter] = (char) 0;
	    }
	    else return ret; // error
	}
	else if (r == 0x03) // CTRL C -> Abort
	{
	    return ret; // error
	}
	else
	{
	  ret = atoi(buffer);
	  if (ret == 0)
	    return -1;
	  printf("Pi:Expecting: %s\n",buffer );
	  return ret;
	}
    }
  }
  return ret;
}

// blocking function
// terminated ("\n") string
void UARTGetString(char *buffer)
{
  // expect name in ASCII + return
  int ret = -1; // error
  int counter = 0;
  buffer[0] = 0;
  
  while (1)
  {
    while (RPI_AuxUartReadPending())
    {
      // printf("Command reading UART String\n");
      char r = RPI_AuxUartRead();
      if (r == '\n')
      {
        return;
      }
      if (r != '\n')
      {
          if (counter<MAX_STRING_SIZE-1)
          {
            buffer[counter++] = r;
            buffer[counter] = (char) 0;
          }
          else return; // error
      }
    }
  } 
}

// 0 = ok
// 1 = error
int UARTGetBinaryData(int size, unsigned char *romBuffer)
{
  int ret = -1;
  int counter = 0; 

  uint64_t expectedTime = __CLOCKS_PER_SEC__ * ((uint64_t)(size*10)); // 10 bits, 8 data, 1 start one stop bit
           expectedTime = expectedTime / ((uint64_t)912600); // size is in bytes -> bits, speed of serial is 912600 baud -> bits per second
  uint64_t timeOutDif = expectedTime*3;
  if (timeOutDif<__CLOCKS_PER_SEC__) timeOutDif = __CLOCKS_PER_SEC__;
  
  uint64_t clockCurrent;
  uint64_t clockStart;
  clockCurrent = getOtherTimer();
  clockStart = clockCurrent;
  
  while (1)
  {
    while (RPI_AuxUartReadPending())
    {
      unsigned char r = (unsigned char) RPI_AuxUartRead();

      romBuffer[counter++] = r;
      if (counter == size)
      {
    //	printf("Pi:Bin transfered.\n");
        return 0;
      }
    }
    
    clockCurrent = getOtherTimer();
    if ((clockCurrent-clockStart)>timeOutDif)
    {
      printf("Pi:Recieve Timeout, Missing: %i.\n", (size-counter));
      return 1;
    }
  }
  return 0; // OK
}

///////////////////////////////////////////
///////////////// UTILITY /////////////////
///////////////////////////////////////////
unsigned char crc8(const unsigned char* data, unsigned char len) {
    unsigned char crc = 0;
    for (unsigned char i = 0; i < len; i++) {
        crc ^= data[i];
        for (unsigned char j = 0; j < 8; j++)
            crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);
    }
    return crc;
}


///////////////////////////////////////////
///////////////// WRITING /////////////////
///////////////////////////////////////////
// Non Blocking
void UARTWriteTimed(char c, int timeOut)
{
    uint32_t startTime = v_micros();
    // Wait until the UART has an empty space in the FIFO 
    
    
    while( RPI_Aux_UartWritePossible() ) 
    { 
      if (v_micros()-startTime>timeOut)
      {
        DEBUG_OUT1("Write Byte timeout!\n");
        return;
      }
    }
    // Write the character to the FIFO for transmission */
    RPI_AuxUartWrite(c);
}

void UARTSendBulk(const signed char* data, size_t length) 
{
  for (int i=0;i<length; i++)
  {
    RPI_AuxUartWrite(data[i]);
  }
}

bool sendByte(unsigned char c)
{
    UARTSendBulk(((const signed char*)&c), 1);
    return true;
}

bool sendBytes(const signed char* c, int len)
{
    UARTSendBulk(c, len);
    return true;
}


int mrp_send_block(const unsigned char* data, int len) {
 
  
  unsigned char seq = 0;
    int offset = 0;

    while (offset < len /*+ 2*/) { // +2 für Längeninfo im ersten Paket
        unsigned char retries = 0;
        unsigned char chunk[MRP_CHUNK_SIZE]; 
        unsigned char chunk_len;

        if (seq == 0) {
            // Erster Chunk enthält Länge als Prefix
            chunk[0] = (len >> 8) & 0xFF;
            chunk[1] = len & 0xFF;

            unsigned char to_copy = (len > MRP_CHUNK_SIZE - 2) ? (MRP_CHUNK_SIZE - 2) : len;
            for (unsigned char i = 0; i < to_copy; i++)
                chunk[2 + i] = data[i];
            chunk_len = to_copy + 2;
        } else {
            int remaining = len - offset;
            chunk_len = (remaining > MRP_CHUNK_SIZE) ? MRP_CHUNK_SIZE : remaining;
            for (unsigned char i = 0; i < chunk_len; i++)
                chunk[i] = data[offset + i]; 
        }

        while (retries++ < MRP_MAX_RETRIES) 
        {
            sendByte(MRP_START_DATA);
            sendByte(seq);
            sendByte(chunk_len);
            sendByte(crc8(chunk, chunk_len));
            sendBytes((const signed char*)chunk, chunk_len);
//            for (unsigned char i = 0; i < chunk_len; i++)
//                piTrex_send_byte(chunk[i]);

            unsigned char ack, ack_seq;
            if (getByteBlocking(&ack) && getByteBlocking(&ack_seq)) 
            {
                if (ack == MRP_ACK && ack_seq == seq)
                    goto chunkSent;
            }
        }
        if (retries >= MRP_MAX_RETRIES) return 0;
chunkSent:

        offset += (seq == 0) ? (chunk_len - 2) : chunk_len;
        seq++;
    }

    return 1;
}

//is im smp noc das uart command read eingeschaltet?

///////////////////////////////////////////
///////////////// READING /////////////////
///////////////////////////////////////////
// buffer UART
unsigned char uartBuffer[100000];
int readPos=0;
int writePos=0;


// using huge UART Buffer!
// 1 = read
// 0 = not read
int readUart(unsigned char *data)
{
  // read everything that is available;
//  int enter = 0;
  while (RPI_AuxUartReadPending())
  {
//    if (enter++==0) FILE_OUT("Read: ");
    unsigned char r = (unsigned char) RPI_AuxUartRead();
    uartBuffer[writePos++] = r;
//FILE_OUT("$%02x ", r);
    if (writePos== 100000) writePos = 0;
  }
//  if (enter) FILE_OUT("\n");
  if (writePos==readPos) return 0; // no data available
  *data = uartBuffer[readPos++];
  if (readPos== 100000) readPos = 0;
  return 1;
}

// returns # of bytes "filled"
// but none is "used"
int fillUart()
{
  int enter=0;
  // read everything that is available;
  while (RPI_AuxUartReadPending())
  {
    unsigned char r = (unsigned char) RPI_AuxUartRead();
    uartBuffer[writePos++] = r;
    enter++;
    if (writePos== 100000) writePos = 0;
  }
  if (readPos== 100000) readPos = 0;
  return enter;
}


// blocking
int getOne8BitValue()
{
  unsigned char r;
  while (!readUart(&r));
  return r;
} 

// return 1000 on timeout
int getOne8BitValueTimed(int timeOut)
{
  uint32_t startTime = v_micros();
  unsigned char r;
  while (!readUart(&r))
  {
    if (v_micros()-startTime>timeOut)
    {
      return 1000;
    }
  }
  return r;
} 

// name convention wraps
inline bool getByteBlocking(unsigned char *c)
{
    *c = (unsigned char) getOne8BitValue();
}


bool getBytes(unsigned char* data, int len) {

  int counter = 0; 
  uint64_t clockStart;
  uint64_t expectedTime = __CLOCKS_PER_SEC__ * ((uint64_t)(len*10)); // 10 bits, 8 data, 1 start, 1 stop bit
  expectedTime = expectedTime / ((uint64_t)912600); // size is in bytes -> bits, speed of serial is 912600 baud -> bits per second
  uint64_t timeOutDif = expectedTime*3.0f;//1.1f; // tolerance

  if (len <10) timeOutDif*=10;
  
  clockStart = getOtherTimer();
  while (1)
  {
    unsigned char r;
    while (readUart(&r))
    {
      data[counter++] = r;
      if (counter == len)
      {
        DEBUG_OUT2("ok: %i, %i\n", (len-counter), counter);
        return true;
      }
    }
    if ((getOtherTimer()-clockStart)>timeOutDif)
    {
      DEBUG_OUT5("Timeout, missing: %i.\n", (len-counter));
      return false;
    }
  }
  DEBUG_OUT5("Leaving get bytes without a cause...\n");
  return false; // NOK
  
  
}

bool getBytes2(unsigned char* data, int len) 
{
  int counter = 0; 
  uint64_t clockStart;
  uint64_t expectedTime = __CLOCKS_PER_SEC__ * ((uint64_t)(len*10)); // 10 bits, 8 data, 1 start, 1 stop bit
  expectedTime = expectedTime / ((uint64_t)912600); // size is in bytes -> bits, speed of serial is 912600 baud -> bits per second
  uint64_t timeOutDif = expectedTime*1.1f; // tolerance

  if (len < 10) timeOutDif *=10;
  
  clockStart = getOtherTimer();
  unsigned char r;
  while (1)
  {
    while (readUart(&r)) // as long as no data is available
    {
      data[counter++] = r;
      if (counter == len)
      {
        DEBUG_OUT2("ok: %i, %i\n", (len-counter), counter);
        return true;
      }
    }

    if ((getOtherTimer()-clockStart)>timeOutDif)
    {
      DEBUG_OUT5("Timeout, missing: %i.\n", (len-counter));
      return false;
    }
  }
  DEBUG_OUT5("Leaving get bytes without a cause...\n");
  return false; // NOK
}