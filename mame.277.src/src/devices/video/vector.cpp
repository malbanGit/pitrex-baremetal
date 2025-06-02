/*
todo:
- started: 2 frame (send only 1/2 frame per frame -> looks bad... also "disturbances" 

- not done: 2 frame interlaced (only show half frame per frame)

- done with dots - but will look bad -> cancled
- not done: reduce count (hardcore)

todo:
bit /line count path count color
sort vectors
sort cluster
angle reduction
*/

#define DEBUG_OUT printf
//#define DEBUG_OUT(...) do{} while (0)

// make CC="ccache gcc" CXX="ccache g++" -j8 FASTBUILD=1
#define VTERM_SUPPORT 1


//#define IGNORE_SERIAL 1 // if compiling, when no vectrex/pitrex is around...



// ORIGINAL MAME START

// license:BSD-3-Clause
// copyright-holders:Brad Oliver,Aaron Giles,Bernd Wiebelt,Allard van der Bas
/******************************************************************************
 *
 * vector.c
 *
 *        anti-alias code by Andrew Caldwell
 *        (still more to add)
 *
 * 040227 Fixed miny clip scaling which was breaking in mhavoc. AREK
 * 010903 added support for direct RGB modes MLR
 * 980611 use translucent vectors. Thanks to Peter Hirschberg
 *        and Neil Bradley for the inspiration. BW
 * 980307 added cleverer dirty handling. BW, ASG
 *        fixed antialias table .ac
 * 980221 rewrote anti-alias line draw routine
 *        added inline assembly multiply fuction for 8086 based machines
 *        beam diameter added to draw routine
 *        beam diameter is accurate in anti-alias line draw (Tcosin)
 *        flicker added .ac
 * 980203 moved LBO's routines for drawing into a buffer of vertices
 *        from avgdvg.c to this location. Scaling is now initialized
 *        by calling vector_init(...). BW
 * 980202 moved out of msdos.c ASG
 * 980124 added anti-alias line draw routine
 *        modified avgdvg.c and sega.c to support new line draw routine
 *        added two new tables Tinten and Tmerge (for 256 color support)
 *        added find_color routine to build above tables .ac
 *
 * Vector Team
 *
 *        Brad Oliver
 *        Aaron Giles
 *        Bernd Wiebelt
 *        Allard van der Bas
 *        Al Kossow (VECSIM)
 *        Hedley Rainnie (VECSIM)
 *        Eric Smith (VECSIM)
 *        Neil Bradley (technical advice)
 *        Andrew Caldwell (anti-aliasing)
 *
 **************************************************************************** */

#include "vector.h"
#include "emuopts.h"
#include "render.h"
#include "screen.h"
#include "emu.h"

#define VECTOR_WIDTH_DENOM 512

// 20000 is needed for mhavoc (see MT 06668) 10000 is enough for other games
#define MAX_POINTS 20000

float vector_options::s_flicker = 0.0f;
float vector_options::s_beam_width_min = 0.0f;
float vector_options::s_beam_width_max = 0.0f;
float vector_options::s_beam_dot_size = 0.0f;
float vector_options::s_beam_intensity_weight = 0.0f;

/**************************************************************/
/*************************** VTERM ****************************/
/**************************************************************/
#ifdef VTERM_SUPPORT
// -------------------
// generated after ~350 frames
const uint8_t lunar_huffman_bitlengths[256] = {
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
const uint8_t starwars_huffman_bitlengths[256] = {
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

#include <string.h>
#include <list>
#include "inputdev.h"

typedef struct {
    float x0;
    float x1;
    float y0;
    float y1;
    int brightness;
    int count; // == 0 > is last
    int type;
} SimpleVTermLine;
static std::list<SimpleVTermLine> vTermLineBuffer;
struct PathLengths {
    float drawLength;
    float moveLength;
    float totalLength() const { return drawLength + moveLength; }
};

// ini support data in "vector.ini" file
#include "fileio.h"
#define VTERM_COMPORT_PARAMETER "vterm_serialport"

#define VTERM_DATA_MODE "vterm_data_mode"

// Bit 0b0000 ddmm : mm 00 = full data, 01 = half delta, 10 = full delta
//                   dd 00 = no dif, 01 = dif (previous buffer must be available), 10 = dif with Huffman (fixed, depending on game - or general)
//					 Hufman is chosen in the beginning with MAME communication	
int dataMode = 0;

static bool vtermInit = false;
static bool vtermIgnoreBrightnessChange = true;

#ifdef _WIN32
#include <windows.h>
#include <conio.h> // Include for _kbhit and getchar
#define strcasecmp _stricmp

COMSTAT comStat;
DWORD errors;

static HANDLE hSerial = NULL;
options_entry const vterm_options[] =
{
	// vterm options
	{ nullptr,                                           nullptr,     core_options::option_type::HEADER,     "VTERM OPTIONS" },
	{ VTERM_COMPORT_PARAMETER,                           "COM5",      core_options::option_type::STRING,     "Serial port used for vector terminal output" },
	{ VTERM_DATA_MODE,                                   "0",         core_options::option_type::STRING,     "Data mode, full, half delta, full delta" },

	{ nullptr }
};
#endif // _WIN32
#ifdef __linux__
// NOTE!
// Since I cannot compile MAME with linux at the moment
// the pitrex code for unix is also not included
// but it exists and works in another project
options_entry const vterm_options[] =
{
	// vterm options
	{ nullptr,                                           nullptr,     core_options::option_type::HEADER,     "VTERM OPTIONS" },
	{ VTERM_COMPORT_PARAMETER,                           "/dev/ttyUSB0",      core_options::option_type::STRING,     "Serial port used for vector terminal output" },
	{ VTERM_DATA_MODE,                                   "0",         core_options::option_type::STRING,     "Data mode, full, half delta, full delta" },

	{ nullptr }
};
#endif // __linux__

#ifndef CBR_921600
#define CBR_921600 921600
#endif

uint8_t *fixedHuffman=nullptr;

#define ORIENTATION_MASK (ORIENTATION_FLIP_X | ORIENTATION_FLIP_Y | ORIENTATION_SWAP_XY)
static int VTERM_BIT_MULTIPLY = 256; // default 8 bit -> nothing else ever will, to much data!

// command values SENT to PiTrex
// general: highest bit set -> then lowest 5 bits are a counter 
// 1xxc cccc
// 4 commands that can be counted
// xx:
// 00 - DOTS 			1-31 times 2 values for x,y
// 01 - PATH LINES		1 times 4 values x0,y0,x1,y1 and 1-30 times 2 values for x2,y2
// 10 - CONT PATH LINES	not done: (continue path) 1-31 times 2 values for x2,y2
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
#define VTERM_WAIT_RECAL 		(0b00000000)
#define VTERM_BRIGHNESS 		(0b00010000) // 0001 xxxx no bit set, xxxx are brightness (value = 3 bit shift left) -> Color "black" (move) is a Wait Recal
#define VTERM_SINGLE_LINE 		(0b00110000) // 0011 ???? SINGLE LINE, 4 values for x0,y0,x1,y1
#define VTERM_DOT 				(0b00100000) // 0010 ???? DOT Single Dot, 2 values for x,y
#define VTERM_START_OF_PATH 	(0b10100000) // 101 - PATH LINES		1 times 4 values x0,y0,x1,y1 and 1-30 times 2 values for x2,y2
#define VTERM_END_LIST 			(0b01110000) // 0111 ???? END_LIST (end of sending data to PiTrex), (???? should be 0000)
#define VTERM_DOTS 				(0b10000000) // 100  ???? DOTS collection of dots, count in command, then count pairs of X Y
#define VTERM_SINGLE_LINES 		(0b11100000) // 111 - SINGLE LINES    1-31 times 4 values for x0,y0,x1,y1

// values in preparation of command queue to send
#define SINGLE_LINE 0
#define START_OF_PATH 1
#define CONTINUE_OF_PATH 2
#define CONTINUE_LINE_COLLECTION 3
#define SINGLE_LINE_COLLECTION 4
#define DOT 5
#define DOT_COLLECTION 6
#define CONTINUE_DOT_COLLECTION 7

// mode of "sending" to PiTrex
// the half modes are only in the beginning
// not properly working
// I think they don't really make sense
#define PITREX_ALL 0
#define PITREX_HALF_FRAME 1
#define PITREX_HALF_FRAME_BUFFERED 2
int sendMode = PITREX_ALL;

unsigned char sendBuffer[100000];
unsigned char sendBufferLength=0;
int sendCount = 0;
running_machine *vMachine; // "global" functions use this, they don't need vector_device class, no changes to vector.h!
int vTermInitDone = 0;
int frameCount =0;
int maxDotCount = 100000;
int vectrexButtons;
int joystickX;
int joystickY;
bool m_analog_enabled = false;

// Function for sending receiving over serial
bool piTrex_open_connection(const char* portname);
void piTrex_close_connection(void);
bool piTrex_get_byte(unsigned char* data);
void piTrex_send_bytes(const unsigned char* data, size_t length);
void piTrex_send_string(const char* data);
int waitForString(const char* waitFor);
int waitForStringTime(const char* waitFor, double timeout);

// Function for packing/unpacking etc
int compare_buffers(const unsigned char* a, const unsigned char* b, int length);
int miniLZ4_compress(const unsigned char* src, int srcSize, unsigned char* dst, int dstCapacity);
int miniLZ4_decompress(const unsigned char* src, int compressedSize, unsigned char* dst, int dstCapacity);
int huffman_compress(const unsigned char* input, int length, unsigned char* output, int outMax);
int huffman_decompress(const unsigned char* input, int length, unsigned char* output, int outMax);
int dif   (const unsigned char* prevBuffer, int prevBufferLength, const unsigned char* src, int srcSize,        unsigned char* dst, int dstCapacity);
int undiff(const unsigned char* prevBuffer, int prevBufferLength, const unsigned char* src, int compressedSize, unsigned char* dst, int dstCapacity);

PathLengths calculatePathLengths(const std::list<SimpleVTermLine>& lines);


//////////////// IMPLEMENTATIONS
//
// Note! Serial functions must be altered for UNIX/LINUX

void flush_serial_buffers(void) {
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
}
bool piTrex_open_connection(const char* portname) {

#ifdef IGNORE_SERIAL
   return true;
#else   
	if (hSerial == NULL) 
	{
		hSerial = CreateFile(portname,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
	}
	/*
	// Set timeouts (even in overlapped mode, COMMTIMEOUTS can apply)
	COMMTIMEOUTS timeouts = {0};
	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;  // 0 = no total timeout (wait indefinitely for data)
	SetCommTimeouts(hSerial, &timeouts);
	*/

    if (hSerial == INVALID_HANDLE_VALUE) 
	{
		hSerial = CreateFile("COM5",
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		if (hSerial == INVALID_HANDLE_VALUE) 
		{
			hSerial = CreateFile("COM4",
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);
			
		}
		if (hSerial == INVALID_HANDLE_VALUE) 
		{
			fprintf(stderr, "Error opening serial port: %s\n", portname);
			return false;
		}
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_921600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error setting COM port parameters\n");
        CloseHandle(hSerial);
        return false;
    }

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        fprintf(stderr, "Error setting COM port timeouts\n");
        CloseHandle(hSerial);
        return false;
    }

    EscapeCommFunction(hSerial, SETDTR);
    EscapeCommFunction(hSerial, SETRTS);
    EscapeCommFunction(hSerial, CLRDTR);
    EscapeCommFunction(hSerial, CLRRTS);
    flush_serial_buffers();
    printf("Serial device %s - connected.\n", portname);
    return true;
#endif
}
void piTrex_close_connection(void) {
    CloseHandle(hSerial);
}
bool piTrex_get_byte(unsigned char* data) {

	if (ClearCommError(hSerial, &errors, &comStat)) 
	{
		if (comStat.cbInQue > 0) 
		{
			// At least one byte is available to read
		} 
		else 
		{
			return false;
			// No data available
		}
	} 
	else 
	{
		// Handle ClearCommError failure
		return false;
	}


    DWORD bytesRead;
    if (ReadFile(hSerial, data, 1, &bytesRead, NULL) && bytesRead == 1) {
        return true;
    }
    return false;

}
inline void piTrex_send_bulk(const signed char* data, size_t length) {
    DWORD bytesWritten;
    WriteFile(hSerial, data, (DWORD)length, &bytesWritten, NULL);
	FlushFileBuffers(hSerial);
	if ((DWORD)bytesWritten != (DWORD)length)
		DEBUG_OUT("SENT MISSMATCH! -> piTrex_send_bulk: written: %li (from: %lli)", bytesWritten, length);
}
inline void piTrex_send_byte(const unsigned char data) {
	// debugging puposes only!
	DEBUG_OUT("\nW: $%02x(%i) ", data, sendCount);	
	sendCount++;
	return piTrex_send_bulk(((const signed char*)&data), 1);
}
inline void piTrex_send_bytes(const unsigned char* data, size_t length) {
	for (int i=0;i<length;i++)
		piTrex_send_byte(*(data+i)); 
}
void piTrex_send_string(const char* data) {
	// strings must be sent with trailing "\n"
	// the trailing \n must also be read!
    piTrex_send_bytes((const unsigned char*)data, strlen(data));
}
int waitForStringTime(const char* waitFor, double timeout){
  clock_t start = clock();

  #define MAX_ACK_BUF 1000 // as it turns out, sometimes one can see ARM Pi Exceptions... for that reason a larger buffer :-)
  char receiveBuffer[MAX_ACK_BUF];
  int receiveLen=0;
  receiveBuffer[0]=0;
  
  while (1)
  {
      unsigned char c;
      // Calculate elapsed time
      double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;

      // Break the loop if the timeout is reached
      if (elapsed >= timeout) 
      {
		  DEBUG_OUT("PiTrex did not send string '%s'(timeout) (got: '%s').\n", waitFor, receiveBuffer);
		  return 0;
      }    
      if (!piTrex_get_byte(&c)) continue;

      *(receiveBuffer+receiveLen)=c;
      receiveLen++;
      if (receiveLen>=MAX_ACK_BUF-1)
      {
	    DEBUG_OUT("Receive buffer overflow in ACK (got: '%s', %i, '%c').\n", receiveBuffer, receiveLen, c);
	    return 0;
      }
      *(receiveBuffer+receiveLen)=0;
      if (strcmp(waitFor,receiveBuffer) ==0)
      {
        DEBUG_OUT("%s\n", receiveBuffer);
		return 1; // ok
      }
	  else
	  {
        DEBUG_OUT("Receive Compare: %s\n", receiveBuffer);
	  }
  }
  DEBUG_OUT("ACK not received (got: '%s').\n", receiveBuffer);
  return 0;
}
int waitForString(const char* waitFor){
	// 1 = ok
	// 0 = failure
  return waitForStringTime(waitFor, 0.1);
}
void reconnectPitrex(){
	while (1)
	{
		piTrex_send_string("PiTrex VTerm Connect\n");
		DEBUG_OUT("Connection string sent...\n");
		if (waitForString("Pi:VTerm connect acknowledged.") ) 
		{
			screen_device *screen = screen_device_enumerator(vMachine->root_device()).first();

			if (screen) 
			{
				char buffer[100];
				DEBUG_OUT("Game name: %s\n", vMachine->system().name);

				char name[40];
				strcpy (name, vMachine->system().name);
//if (strcasecmp(name, "tempest")) maxDotCount = 10; - every "player" shot consists of X dots... and there can be x shots... so if we reduce, placer shots are not vivisble!

if (strcasecmp(name, "llander")) 
	fixedHuffman= (uint8_t *)lunar_huffman_bitlengths;
if (strcasecmp(name, "starwars")) 
	fixedHuffman= (uint8_t *)starwars_huffman_bitlengths;



				sprintf(buffer,"%s\n", vMachine->system().name);
				piTrex_send_string(buffer);
				
				//printf("Full name: %s\n", machine().system().description);
				int r = ATTOSECONDS_TO_HZ(screen->refresh_attoseconds());
				DEBUG_OUT("Refresh rate: %i Hz\n", r);
				piTrex_send_byte((unsigned char)(r&0xff));

				unsigned char flags=0;
				piTrex_send_byte((unsigned char)(flags&0xff));
				DEBUG_OUT("Flags (vterm, to be defined): %02x\n", (flags&0xff));

				//printf("Resolution: %dx%d\n", screen->visible_area().width(), screen->visible_area().height());
				DEBUG_OUT("ORIENTATION: %i\n", vMachine->system().flags & ORIENTATION_MASK);

				if ((vMachine->system().flags & ORIENTATION_SWAP_XY) ==ORIENTATION_SWAP_XY)
					DEBUG_OUT("Flip: ORIENTATION_SWAP_XY\n"); 
				if ((vMachine->system().flags & ORIENTATION_FLIP_X) ==ORIENTATION_FLIP_X)
					DEBUG_OUT("Flip: ORIENTATION_FLIP_X\n"); 
				if ((vMachine->system().flags & ORIENTATION_FLIP_Y) ==ORIENTATION_FLIP_Y)
					DEBUG_OUT("Flip: ORIENTATION_FLIP_Y\n"); 

				// this is just for information,
				// flipping is done on the MAME side (below)
				piTrex_send_byte(((unsigned char)vMachine->system().flags & ORIENTATION_MASK)&0xff);
			}		
			else
			{
				DEBUG_OUT("Screen is still NULL!\n");
			}
			fflush(stdout);
			return; // ok
		}	
	}
}
void initVTerm(emu_options& options) {
	if (vTermInitDone) return;
	vTermInitDone = 1;

    // init vterm buffer
    for (int i = 0; i < MAX_POINTS; ++i)
    {
        vTermLineBuffer.push_back({0,0,0,0,0,0,0});  
    }

	// base name of ini file, reuse of vector
	const char *basename = "vector";

	// add options for vterm to be recognized when parsing ini file
    options.add_entries(vterm_options);

	// open the file; if we fail, that's ok
	emu_file file(options.ini_path(), OPEN_FLAG_READ);
	osd_printf_verbose("Attempting load of %s.ini\n", basename);
	std::error_condition const filerr = file.open(std::string(basename) + ".ini");
	if (filerr)
	{
		osd_printf_verbose("Error reading vector.ini\n");
	}
	else
	{
		// parse the file
		osd_printf_verbose("Parsing %s.ini (access for vterm ini)\n", basename);
		try
		{
			options.parse_ini_file((util::core_file&)file, OPTION_PRIORITY_NORMAL, false, false);
		}
		catch (options_exception &ex)
		{
			osd_printf_verbose("Error parsing %s.ini, Exception: \n%s\n", basename, ex.message());
		}
	}

	// establish serial connection to serial device
	const char* raw_value = options.value(VTERM_COMPORT_PARAMETER);
	const char* mode = options.value(VTERM_DATA_MODE);
	
	
	dataMode = atoi(mode);

printf("Data mode gotten from ini file: %i\n", dataMode);

#ifdef IGNORE_SERIAL
	return;
#endif	

	if (raw_value != nullptr) 
	{
		std::string port = raw_value;

		if (port.empty()) 
		{
			// Option was NOT set by the user — you can act accordingly
			osd_printf_info("No serial port has been set for PiTrex terminal. Cancelling VTerm processing.\n");
			vtermInit = false;
		}	
		else
		{
			osd_printf_info("Option: %s: %s has been processed.\n", VTERM_COMPORT_PARAMETER, port.c_str());

			vtermInit = piTrex_open_connection(port.c_str());
			if (vtermInit)
			{
				osd_printf_info("Serial port opened for PiTrex terminal.\n");
			}
			else
			{
				osd_printf_info("Serial Connection could not be established. Cancelling VTerm processing.\n");
			}
		}
	}
	if (vtermInit)
	{
		// establish connection to PiTrex
		reconnectPitrex();
		osd_printf_verbose("Connections with PiTrex acknowledged.\n");
	}
}
// -------------------

/**************************************************************/
/************************ SUPPORT INPUT ***********************/
/**************************************************************/

// note following items should better be added to the
// vector_device, or even better be added to an own input device
// but my goal was to ONLY edit this one file "vector.cpp"
// for this I went a little "dirty"
int get_state_up(void *device, void *item)        { if (m_analog_enabled != 0) return 0; return (joystickY> 20*512) ? 1 : 0; }
int get_state_down(void *device, void *item)      { if (m_analog_enabled != 0) return 0; return (joystickY<-20*512) ? 1 : 0; }
int get_state_left(void *device, void *item)      { if (m_analog_enabled != 0) return 0; return (joystickX> 20*512) ? 1 : 0; }
int get_state_right(void *device, void *item)     { if (m_analog_enabled != 0) return 0; return (joystickX<-20*512) ? 1 : 0; }

int get_state_button1(void *device, void *item)   { return (vectrexButtons & 0x01) ? 1 : 0; }
int get_state_button2(void *device, void *item)   { return (vectrexButtons & 0x02) ? 1 : 0;; }
int get_state_button3(void *device, void *item)   { return (vectrexButtons & 0x04) ? 1 : 0;; }
int get_state_button4(void *device, void *item)   { return (vectrexButtons & 0x08) ? 1 : 0;; }

// should be in range -65535 - +65535
int get_state_analog_x(void *device, void *item)  { if (m_analog_enabled != 0) return joystickX;return 0;} 
int get_state_analog_y(void *device, void *item)  { if (m_analog_enabled != 0) return joystickY;return 0;}

// Just display whether analog mode is enabled
s32 get_analog_enabled_state(void *device, void *cookie){return m_analog_enabled ? 1 : 0;}

#endif //VTERM_SUPPORT


void vector_options::init(emu_options& options){
	s_beam_width_min = options.beam_width_min();
	s_beam_width_max = options.beam_width_max();
	s_beam_dot_size = options.beam_dot_size();
	s_beam_intensity_weight = options.beam_intensity_weight();
	s_flicker = options.flicker();
}
// device type definition
DEFINE_DEVICE_TYPE(VECTOR, vector_device, "vector_device", "VECTOR")
vector_device::vector_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: device_t(mconfig, VECTOR, tag, owner, clock),
		device_video_interface(mconfig, *this),
		m_vector_list(nullptr),
		m_min_intensity(255),
		m_max_intensity(0)
{
}
void vector_device::device_start(){
	vector_options::init(machine().options());
#ifdef VTERM_SUPPORT
	// remember machine, to be usable in global functions. Dirty, I know!
	vMachine=&machine();
	initVTerm(machine().options());
	
	printf("Starting VTerm stuff\n"); fflush(stdout);
	input_class &joy_class = machine().input().device_class(DEVICE_CLASS_JOYSTICK);
	input_device &vectrex_joy = joy_class.add_device("PiTrex Input (1)", "Vectrex Serial Controller", this);
	vectrex_joy.add_item("Up",       "up",       ITEM_ID_UP,      get_state_up, this);
	vectrex_joy.add_item("Down",     "down",     ITEM_ID_DOWN,    get_state_down, this);
	vectrex_joy.add_item("Left",     "left",     ITEM_ID_LEFT,    get_state_left, this);
	vectrex_joy.add_item("Right",    "right",    ITEM_ID_RIGHT,   get_state_right, this);
	vectrex_joy.add_item("Button 1", "button1",  ITEM_ID_BUTTON1, get_state_button1, this);
	vectrex_joy.add_item("Button 2", "button2",  ITEM_ID_BUTTON2, get_state_button2, this);
	vectrex_joy.add_item("Button 3", "button3",  ITEM_ID_BUTTON3, get_state_button3, this);
	vectrex_joy.add_item("Button 4", "button4",  ITEM_ID_BUTTON4, get_state_button4, this);
	vectrex_joy.add_item("X Axis",   "xaxis",    ITEM_ID_XAXIS,   get_state_analog_x, this);
	vectrex_joy.add_item("Y Axis",   "yaxis",    ITEM_ID_YAXIS,   get_state_analog_y, this);
	vectrex_joy.add_item("Analog Mode Enabled (Toggle:BUTTON 1,2,3)", "mode_display", ITEM_ID_OTHER_SWITCH, get_analog_enabled_state, this);
#endif

	m_vector_index = 0;

	/* allocate memory for tables */
	m_vector_list = std::make_unique<point[]>(MAX_POINTS);
}

/*
 * www.dinodini.wordpress.com/2010/04/05/normalized-tunable-sigmoid-functions/
 */
float vector_device::normalized_sigmoid(float n, float k)
{
	// valid for n and k in range of -1.0 and 1.0
	return (n - n * k) / (k - fabs(n) * 2.0f * k + 1.0f);
}

/*
 * Adds a line end point to the vertices list. The vector processor emulation
 * needs to call this.
 */
void vector_device::add_point(int x, int y, rgb_t color, int intensity){
	point *newpoint;
	intensity = std::clamp(intensity, 0, 255);
	m_min_intensity = intensity > 0 ? std::min(m_min_intensity, intensity) : m_min_intensity;
	m_max_intensity = intensity > 0 ? std::max(m_max_intensity, intensity) : m_max_intensity;

	if (vector_options::s_flicker && (intensity > 0))
	{
		float random = (float)(machine().rand() & 255) / 255.0f; // random value between 0.0 and 1.0
		intensity -= (int)(intensity * random * vector_options::s_flicker);
		intensity = std::clamp(intensity, 0, 255);
	}

	newpoint = &m_vector_list[m_vector_index];
	newpoint->x = x;
	newpoint->y = y;
	newpoint->col = color;
	newpoint->intensity = intensity;

	m_vector_index++;
	if (m_vector_index >= MAX_POINTS)
	{
		m_vector_index--;
		logerror("*** Warning! Vector list overflow!\n");
	}
}

/*
 * The vector CPU creates a new display list. We save the old display list,
 * but only once per refresh.
 */
void vector_device::clear_list(void){
	m_vector_index = 0;
}
uint32_t vector_device::screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect)
{
	uint32_t flags = PRIMFLAG_ANTIALIAS(1) | PRIMFLAG_BLENDMODE(BLENDMODE_ADD) | PRIMFLAG_VECTOR(1);
	const rectangle &visarea = screen.visible_area();
	float xscale = 1.0f / (65536 * visarea.width());
	float yscale = 1.0f / (65536 * visarea.height());
	float xoffs = (float)visarea.min_x;
	float yoffs = (float)visarea.min_y;

	point *curpoint;
	int lastx = 0;
	int lasty = 0;

	curpoint = m_vector_list.get();

	screen.container().empty();
	screen.container().add_rect(0.0f, 0.0f, 1.0f, 1.0f, rgb_t(0xff,0x00,0x00,0x00), PRIMFLAG_BLENDMODE(BLENDMODE_ALPHA) | PRIMFLAG_VECTORBUF(1));

#ifdef VTERM_SUPPORT
	// prepare PiTrex Stuff
	//
	// debug, timing measurement
    LARGE_INTEGER frequency;

    LARGE_INTEGER startTime; // start of "frame"
    LARGE_INTEGER endTime;   // diverse end times

    LARGE_INTEGER lastTime;  // exit of this function to get values for all "mame" time between calls
	LARGE_INTEGER ackTime;   // timing for the Ack from Vectrex to reach us (start of WR)
	LARGE_INTEGER sendTime;  // time it took for transfer of current frame data to PiTrex

	printf("Screen Update %i\n", frameCount);	
	frameCount++;

    // Get the high-resolution counter's frequency
    QueryPerformanceFrequency(&frequency);

    double elapsedTimeMs;
	(void)elapsedTimeMs; // avoiding stupid compiler warning, when DEBUG_OUT is naught

	QueryPerformanceCounter(&startTime);
    // Calculate elapsed time in milliseconds
    elapsedTimeMs = (double)(startTime.QuadPart - lastTime.QuadPart) * 1000.0 / frequency.QuadPart;
	DEBUG_OUT("Between Screen Update: %.3f ms\n", elapsedTimeMs);

    auto vtermIT = vTermLineBuffer.begin();
    SimpleVTermLine* currentVTermLine = nullptr;
	
	SimpleVTermLine* lastVTermLine = nullptr;
    SimpleVTermLine* collectorVTermLine = nullptr;
    
    float lastDrawPointX=0;
    float lastDrawPointY=0;
    int lastBrightness = 0;
	int dotCount=0;

	// DIFF
	static int hasPrevBuffer = 0;
	static unsigned char prevBuffer[100000];
	static int prevBufferLength = 0;


	(void) lastVTermLine;
	(void) lastDrawPointX;
	(void) lastDrawPointY;
	(void) dotCount;
	(void) vtermIgnoreBrightnessChange;
	(void) collectorVTermLine;

#endif
	for (int i = 0; i < m_vector_index; i++)
	{
		render_bounds coords;

		float intensity = (float)curpoint->intensity / 255.0f;
		float intensity_weight = normalized_sigmoid(intensity, vector_options::s_beam_intensity_weight);

		// check for static intensity
		float beam_width = m_min_intensity == m_max_intensity
			? vector_options::s_beam_width_min
			: vector_options::s_beam_width_min + intensity_weight * (vector_options::s_beam_width_max - vector_options::s_beam_width_min);

		// normalize width
		beam_width *= 1.0f / (float)VECTOR_WIDTH_DENOM;

		// apply point scale for points
		if (lastx == curpoint->x && lasty == curpoint->y)
			beam_width *= vector_options::s_beam_dot_size;

		coords.x0 = ((float)lastx - xoffs) * xscale;
		coords.y0 = ((float)lasty - yoffs) * yscale;
		coords.x1 = ((float)curpoint->x - xoffs) * xscale;
		coords.y1 = ((float)curpoint->y - yoffs) * yscale;

		if (curpoint->intensity != 0)
		{
			screen.container().add_line(
				coords.x0, coords.y0, coords.x1, coords.y1,
				beam_width,
				(curpoint->intensity << 24) | (curpoint->col & 0xffffff),
				flags);
#ifdef VTERM_SUPPORT

			// todo: use a correct line cut, instead of just the one coordinate!
			if (coords.y0<0) coords.y0=0;
			if (coords.y1<0) coords.y1=0;
			if (coords.x0<0) coords.x0=0;
			if (coords.x1<0) coords.x1=0;

			if (coords.y0>1) coords.y0=1;
			if (coords.y1>1) coords.y1=1;
			if (coords.x0>1) coords.x0=1;
			if (coords.x1>1) coords.x1=1;

			if ((vMachine->system().flags & ORIENTATION_SWAP_XY) ==ORIENTATION_SWAP_XY)
			{
				float x0t = coords.x0;
				float x1t = coords.x1;
				coords.x0 = coords.y0;
				coords.x1 = coords.y1;
				coords.y0 = x0t;
				coords.y1 = x1t;
			}

			if ((vMachine->system().flags & ORIENTATION_FLIP_X) ==ORIENTATION_FLIP_X)
			{
				coords.x0 = 1-coords.x0;
				coords.x1 = 1-coords.x1;
			}
			
			// vectrex ALWAYS needs flipping, so we do the opposite!
			if ((vMachine->system().flags & ORIENTATION_FLIP_Y) !=ORIENTATION_FLIP_Y)
			{
				coords.y0 = 1-coords.y0;
				coords.y1 = 1-coords.y1;
			}

            //DEBUG_OUT("Adding line to VTerm\n");
            currentVTermLine = &(*vtermIT);
            currentVTermLine->x0 = (coords.x0-0.5)*VTERM_BIT_MULTIPLY;
            currentVTermLine->y0 = (coords.y0-0.5)*VTERM_BIT_MULTIPLY;
            currentVTermLine->x1 = (coords.x1-0.5)*VTERM_BIT_MULTIPLY;
            currentVTermLine->y1 = (coords.y1-0.5)*VTERM_BIT_MULTIPLY;
            currentVTermLine->count = 1;
            currentVTermLine->type = SINGLE_LINE;
            currentVTermLine->brightness = ((curpoint->intensity)&255)>>(1); // only 7 bit

			// with above calculation
			// values of +128 are still possible!
			// I don't think -128 though
			// still - just to be on the save side!
			if (currentVTermLine->x0<-128) currentVTermLine->x0 = -128;
			if (currentVTermLine->x1<-128) currentVTermLine->x1 = -128;
			if (currentVTermLine->y0<-128) currentVTermLine->y0 = -128;
			if (currentVTermLine->y1<-128) currentVTermLine->y1 = -128;

			if (currentVTermLine->x0>127) currentVTermLine->x0 = 127;
			if (currentVTermLine->x1>127) currentVTermLine->x1 = 127;
			if (currentVTermLine->y0>127) currentVTermLine->y0 = 127;
			if (currentVTermLine->y1>127) currentVTermLine->y1 = 127;


            // todo
            // for color games?
            currentVTermLine->brightness &= (64+32+16+8);

			bool bcheck = true;

			// recognize DOTS
			if ((currentVTermLine->x0 == currentVTermLine->x1) && (currentVTermLine->y0==currentVTermLine->y1))
			{
				dotCount++;
				if (dotCount>maxDotCount)
				{
					curpoint++;
					continue;
				}
				currentVTermLine->type = DOT;
			}

            //////////////////////////////////////////////
            // Optimizing serial output -> collecting paths into "paths" to send only single coordinates

			// within one path, brightness change may be ignored!
			bcheck = lastBrightness==currentVTermLine->brightness;
			if (vtermIgnoreBrightnessChange) bcheck=true;

			if (lastVTermLine != nullptr)
			{
				if ((currentVTermLine->type == DOT) && (bcheck))
				{
					if (collectorVTermLine!=nullptr) if (collectorVTermLine->type == SINGLE_LINE_COLLECTION) collectorVTermLine=nullptr;
					if (collectorVTermLine!=nullptr) if (collectorVTermLine->type == START_OF_PATH) collectorVTermLine=nullptr;

					// collect dots
					if ((lastVTermLine->type == DOT) || (lastVTermLine->type == CONTINUE_DOT_COLLECTION))
					{
						if (collectorVTermLine==nullptr)
							collectorVTermLine = lastVTermLine;
						
						if (collectorVTermLine!=nullptr)
						{
							if (collectorVTermLine->count==31) // maximum of 5 bit count!
							{
								collectorVTermLine=nullptr;
							}
							else
							{
								collectorVTermLine->type = DOT_COLLECTION;
								collectorVTermLine->count++;
								currentVTermLine->type = CONTINUE_DOT_COLLECTION;
								if (vtermIgnoreBrightnessChange)
									currentVTermLine->brightness = collectorVTermLine->brightness;
							}
						}
					}
					else				
					{
						collectorVTermLine=nullptr;
					}
				}

				if (currentVTermLine->type == SINGLE_LINE) 
				{
					// collect a PATH
					if (collectorVTermLine!=nullptr) if (collectorVTermLine->type == DOT_COLLECTION) collectorVTermLine=nullptr;
					
					if (   (currentVTermLine->x0==lastDrawPointX && currentVTermLine->y0==lastDrawPointY && bcheck) 
						&& ((lastVTermLine->type == SINGLE_LINE)  || (lastVTermLine->type == CONTINUE_OF_PATH))  )
					{
						if (collectorVTermLine!=nullptr) if (collectorVTermLine->type == SINGLE_LINE_COLLECTION) collectorVTermLine=nullptr;
						if (collectorVTermLine==nullptr) collectorVTermLine = lastVTermLine;
						if (collectorVTermLine!=nullptr)
						{
							if (collectorVTermLine->count==31) // maximum of 5 bit count!
							{
								collectorVTermLine=nullptr;
							}
							else
							{
								collectorVTermLine->type = START_OF_PATH;
								collectorVTermLine->count++;
								currentVTermLine->type = CONTINUE_OF_PATH;
								if (vtermIgnoreBrightnessChange)
									currentVTermLine->brightness = collectorVTermLine->brightness;
							}
						}
					}
					else				
					{
						if (collectorVTermLine != nullptr) if (collectorVTermLine->type == START_OF_PATH) collectorVTermLine=nullptr;
						
						// collect single lines
						if ((lastVTermLine->type == SINGLE_LINE) || (lastVTermLine->type == CONTINUE_LINE_COLLECTION))
						{
							if (collectorVTermLine==nullptr) collectorVTermLine = lastVTermLine;
							if (collectorVTermLine!=nullptr)
							{
								if (collectorVTermLine->count==31) // maximum of 5 bit count!
								{
									collectorVTermLine=nullptr;
								}
								else
								{
									collectorVTermLine->type = SINGLE_LINE_COLLECTION;
									collectorVTermLine->count++;
									currentVTermLine->type = CONTINUE_LINE_COLLECTION;
									if (vtermIgnoreBrightnessChange)
										currentVTermLine->brightness = collectorVTermLine->brightness;
								}
							}
						}
						else				
						{
							collectorVTermLine=nullptr;
						}
					}
				}
			}
				

            lastDrawPointX=currentVTermLine->x1;
            lastDrawPointY=currentVTermLine->y1;
            lastBrightness=currentVTermLine->brightness;

            lastVTermLine = currentVTermLine;
            
            if (vtermIT != vTermLineBuffer.end())
            {
                // Prevent overflow
                vtermIT++;
            }            
            //////////////////////////////////////////////
#endif
		}
		lastx = curpoint->x;
		lasty = curpoint->y;
		curpoint++;
	}
#ifdef VTERM_SUPPORT
    currentVTermLine = &(*vtermIT);
    if (currentVTermLine!=nullptr)
        currentVTermLine->count = 0; // end

    // now send to PiTrex
	signed char sendBuffer[100000];
	int sendBufferLength=0;
	static int prvButton = 0;


#ifndef IGNORE_SERIAL
	#define _ERR_ 1
	#define _ACK_ 255

	DEBUG_OUT("Wait for Ack\n");
	unsigned char charRead=0;
	do
	{
		while (!piTrex_get_byte(&charRead));

		if ((charRead != _ACK_) && (charRead != _ERR_))
		{
			DEBUG_OUT("%c", charRead);	
		}
	} while ((charRead != _ACK_) && (charRead != _ERR_));

	if (charRead == _ERR_)
	{
		// reset diff sent
		DEBUG_OUT("\nError gotten - resetting Prev Buffer!\n");
		hasPrevBuffer = 0;
	}



	while (!piTrex_get_byte(&charRead));
	vectrexButtons = charRead;
	
	while (!piTrex_get_byte(&charRead));
	joystickX =  (signed char) charRead;
	
	while (!piTrex_get_byte(&charRead));
	joystickY =  (signed char) charRead;
#endif	

	if (joystickX==-128) joystickX=-127;
	if (joystickY==-128) joystickY=-127;

	// toggle analog mode of vectrex joystick
	if (vectrexButtons!=prvButton)
	{
		if ((vectrexButtons&(0x01+0x02+0x04)) == (0x01+0x02+0x04) )
		{
			m_analog_enabled = !m_analog_enabled;
		}
	}
	prvButton = vectrexButtons;

	// MAME expects -65536 to +65536
	joystickX*=512;
	joystickY*=512;

	flush_serial_buffers();
    QueryPerformanceCounter(&ackTime);
    elapsedTimeMs = (double)(ackTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;
	
    DEBUG_OUT("Waiting for Ack took: %.3f ms\n", elapsedTimeMs);

    QueryPerformanceCounter(&sendTime);
	piTrex_send_byte(VTERM_WAIT_RECAL);
    DEBUG_OUT("WAIT RECAL\n");

    vtermIT = vTermLineBuffer.begin();
    currentVTermLine = &(*vtermIT);

	// prepare some analytic things
    lastBrightness = 1000;
	int overallItemCount = 0;
	int overallByteCount = 0;
	while (currentVTermLine->count != 0)
	{
		overallItemCount++;
		if (lastBrightness != currentVTermLine->brightness)
		{
			overallByteCount++;
			lastBrightness = currentVTermLine->brightness;
		}

		if (currentVTermLine->type == DOT)
		{
			overallByteCount+=3;
		}
		else if (currentVTermLine->type == DOT_COLLECTION)
		{
			overallByteCount+=3;
		}
		else if (currentVTermLine->type == CONTINUE_DOT_COLLECTION)
		{
			overallByteCount+=2;
		}
		else if (currentVTermLine->type == SINGLE_LINE)
		{
			overallByteCount+=5;
		}
		else if (currentVTermLine->type == SINGLE_LINE_COLLECTION)
		{
			overallByteCount+=5;
		}
		else if (currentVTermLine->type == CONTINUE_LINE_COLLECTION)
		{
			overallByteCount+=4;
		}
		else if (currentVTermLine->type == START_OF_PATH)
		{
			overallByteCount+=5;
		}
		else if (currentVTermLine->type == CONTINUE_OF_PATH)
		{
			overallByteCount+=2;
		}
		vtermIT++;
		currentVTermLine = &(*vtermIT);
	} 
	overallByteCount+=1; // finish the list!
	DEBUG_OUT("Pipeline itemcount: %i, bytecount: %i\n", overallItemCount, overallByteCount);

	// following is for the "half mode" 
	// not really used, since I think I won't follow that route further
	int sendStart=0;
	int sendEnd=100000;
	if (sendMode != PITREX_ALL)
	{
		if (frameCount%2==0)
		{
			// send first half
			sendEnd = overallItemCount/2; // exclusive
		}
		else
		{
			// send second half
			sendStart=overallItemCount/2; // inclusive
		}
	}

	// how the line data is represented when sent

	// 0 = default line with xy coordinates
	// 1 = line delta, second coordinate as delta
	// 2 = full delta, even start coordinates as delta from the last known position
//	#define LINE_DATA_TYPE 0

	float lx=0;
	float ly=0;

	#define SEND_COORD0 \
	do{ \
		if (dataMode==0) \
		{ \
			sendBuffer[sendBufferLength++] = currentVTermLine->x0;               \
			DEBUG_OUT("$%02x ", (unsigned char)sendBuffer[sendBufferLength-1]);  \
			sendBuffer[sendBufferLength++] = currentVTermLine->y0;               \
			DEBUG_OUT("$%02x ", (unsigned char)sendBuffer[sendBufferLength-1]);  \
		} \
		else if (dataMode==1) \
		{ \
			sendBuffer[sendBufferLength++] = currentVTermLine->x0;               \
			DEBUG_OUT("$%02x ", (unsigned char)sendBuffer[sendBufferLength-1]);  \
			sendBuffer[sendBufferLength++] = currentVTermLine->y0;               \
			DEBUG_OUT("$%02x ", (unsigned char)sendBuffer[sendBufferLength-1]);  \
			lx = currentVTermLine->x0;                                           \
			ly = currentVTermLine->y0;                                           \
		} \
		else if (dataMode==2) \
		{ \
			sendBuffer[sendBufferLength++] = currentVTermLine->x0-lx;            \
			DEBUG_OUT("$%02x ", (unsigned char)sendBuffer[sendBufferLength-1]);  \
			sendBuffer[sendBufferLength++] = currentVTermLine->y0-ly;            \
			DEBUG_OUT("$%02x ", (unsigned char)sendBuffer[sendBufferLength-1]);  \
			lx = currentVTermLine->x0;                                           \
			ly = currentVTermLine->y0;                                           \
		} \
	}while (0)

	#define SEND_COORD1 \
	do{ \
		if (dataMode==0) \
		{ \
			sendBuffer[sendBufferLength++] = currentVTermLine->x1;               \
			DEBUG_OUT("$%02x ", (unsigned char)sendBuffer[sendBufferLength-1]);  \
			sendBuffer[sendBufferLength++] = currentVTermLine->y1;               \
			DEBUG_OUT("$%02x ", (unsigned char)sendBuffer[sendBufferLength-1]);  \
		} \
		else if (dataMode==1) \
		{ \
			sendBuffer[sendBufferLength++] = currentVTermLine->x1-lx;            \
			DEBUG_OUT("$%02x ", (unsigned char)sendBuffer[sendBufferLength-1]);  \
			sendBuffer[sendBufferLength++] = currentVTermLine->y1-ly;            \
			DEBUG_OUT("$%02x ", (unsigned char)sendBuffer[sendBufferLength-1]);  \
		} \
		else if (dataMode==2) \
		{ \
			sendBuffer[sendBufferLength++] = currentVTermLine->x1-lx;            \
			DEBUG_OUT("$%02x ", (unsigned char)sendBuffer[sendBufferLength-1]);  \
			sendBuffer[sendBufferLength++] = currentVTermLine->y1-ly;            \
			DEBUG_OUT("$%02x ", (unsigned char)sendBuffer[sendBufferLength-1]);  \
		} \
	}while (0)

	overallItemCount = 0;
    lastBrightness = 1000;
    vtermIT = vTermLineBuffer.begin();
    currentVTermLine = &(*vtermIT);

    while (currentVTermLine->count != 0)
    {
		int itemOkToSend = 1;
		if ((overallItemCount) < sendStart) itemOkToSend = 0;
		if ((overallItemCount) >= sendEnd) itemOkToSend = 0;
		overallItemCount++;
		if (!itemOkToSend)
		{
			vtermIT++;
			currentVTermLine = &(*vtermIT);
			continue;
		}
		
		unsigned char command = 0;
		if (currentVTermLine->type != CONTINUE_OF_PATH)	DEBUG_OUT("\n(%i):", sendBufferLength);
        if (lastBrightness != currentVTermLine->brightness)
        {
			command = currentVTermLine->brightness;
			command = command >>3;
			command = (command | (VTERM_BRIGHNESS));
			command = (command & (0b00011111)); // 0001 cccc Set Brightness
			sendBuffer[sendBufferLength++] = command;
            lastBrightness = currentVTermLine->brightness;
DEBUG_OUT("$%02x INT(%i)\n", (unsigned char)sendBuffer[sendBufferLength-1],(currentVTermLine->brightness>>3)*8 );
DEBUG_OUT("(%i):", sendBufferLength);
        }
        if (currentVTermLine->type == DOT)
        {
			command = VTERM_DOT; // 0010 ???? DOT Single Dot, 2 values for x,y
			sendBuffer[sendBufferLength++] = command;
DEBUG_OUT("$%02x DOT ", (unsigned char)sendBuffer[sendBufferLength-1]);
			SEND_COORD0;
		}
	
        else if (currentVTermLine->type == DOT_COLLECTION)
        {
			command = VTERM_DOTS; // (0b10000000) // 100  ???? DOTS collection of dots, count in command, then count pairs of X Y
			command = command | ((currentVTermLine->count)&0b00011111); // 5 bit count!
			sendBuffer[sendBufferLength++] = command;
DEBUG_OUT("$%02x DOTS ", (unsigned char)sendBuffer[sendBufferLength-1]);

			SEND_COORD0;
        }
        else if (currentVTermLine->type == CONTINUE_DOT_COLLECTION)
        {
			SEND_COORD0;
        }
        else if (currentVTermLine->type == SINGLE_LINE)
        {
			command = VTERM_SINGLE_LINE; // 0011 ???? SINGLE LINE, 4 values for x0,y0,x1,y1
			sendBuffer[sendBufferLength++] = command;
DEBUG_OUT("$%02x LINE ", (unsigned char)sendBuffer[sendBufferLength-1]);

			SEND_COORD0;
			SEND_COORD1;
        }
        else if (currentVTermLine->type == SINGLE_LINE_COLLECTION)
        {
			command = VTERM_SINGLE_LINES; // 111 - SINGLE LINES    1-31 times 4 values for x0,y0,x1,y1
			command = command | ((currentVTermLine->count)&0b00011111); // 5 bit count!
			sendBuffer[sendBufferLength++] = command;
DEBUG_OUT("$%02x LINES ", (unsigned char)sendBuffer[sendBufferLength-1]);

			SEND_COORD0;
			SEND_COORD1;
        }
        else if (currentVTermLine->type == CONTINUE_LINE_COLLECTION)
        {
			SEND_COORD0;
			SEND_COORD1;
        }
        else if (currentVTermLine->type == START_OF_PATH)
        {
			command = VTERM_START_OF_PATH; // // 101 - PATH LINES		1 times 4 values x0,y0,x1,y1 and 1-30 times 2 values for x2,y2
			command = command | ((currentVTermLine->count)&0b00011111); // 5 bit count!
			sendBuffer[sendBufferLength++] = command;
DEBUG_OUT("$%02x PATH(%i) ", (unsigned char)sendBuffer[sendBufferLength-1], currentVTermLine->count);

			SEND_COORD0;
			SEND_COORD1;
        }
        else if (currentVTermLine->type == CONTINUE_OF_PATH)
        {
			SEND_COORD1;
        }
		lx=currentVTermLine->x1;
		ly=currentVTermLine->y1;


        vtermIT++;
        currentVTermLine = &(*vtermIT);
    } 

	sendBuffer[sendBufferLength++] = VTERM_END_LIST;
	
		printf("Pre DIF Size 1: %i\n", sendBufferLength);
	
DEBUG_OUT("$%02x ", (unsigned char)sendBuffer[sendBufferLength-1]);
	sendCount = 0;


PathLengths lengths = calculatePathLengths(vTermLineBuffer);
printf("\nBeam move - Draw length: %i, Move length: %i, total length: %i \n",(int) lengths.drawLength,(int) lengths.moveLength,(int) lengths.totalLength());



// PACKING
unsigned char difCompressedBuffer[100000];
unsigned char difHufCompressedBuffer[100000];
unsigned char compressedBuffer[100000];
unsigned char decompressedBuffer[100000];


/*

int originalSize = sendBufferLength;

int compressedSize = miniLZ4_compress((const unsigned char*)sendBuffer, sendBufferLength, (unsigned char *)compressedBuffer, 100000);
printf("LZ Original size: %i, compressed size: %i\n", originalSize, compressedSize);

int compressedLength = huffman_compress((const unsigned char*)compressedBuffer, compressedSize, (unsigned char *)sendBuffer, 100000);
printf("HUFFMAN Original size: %i, compressed size: %i\n", originalSize, compressedLength);

int decompressedLength = huffman_decompress((const unsigned char*)sendBuffer, compressedLength, (unsigned char *)compressedBuffer, 100000);
printf("HUFFMAN Original decompressed size: %i\n", decompressedLength);

decompressedLength = miniLZ4_decompress((const unsigned char*)compressedBuffer, decompressedLength, (unsigned char *)sendBuffer, 100000);
printf("LZ  Original decompressed size: %i\n", decompressedLength);
*/
static int round = 0;
static int lz4SizeSum = 0;
static int difSizeSum = 0;
static int hufSizeSum = 0;
static int difHufSizeSum = 0;
round++;

int lz4CompressedSize = miniLZ4_compress((const unsigned char*)sendBuffer, sendBufferLength, (unsigned char *)compressedBuffer, 100000);
//printf("LZ4 compressed size: %i\n", lz4CompressedSize);
(void) lz4CompressedSize ;

int hufCompressedSize = huffman_compress((const unsigned char*)sendBuffer, sendBufferLength, (unsigned char *)compressedBuffer, 100000);
//printf("HUF compressed size: %i\n", hufCompressedSize);
(void)hufCompressedSize;

//int decompressedLength = huffman_decompress((const unsigned char*)compressedBuffer, compressedLength, (unsigned char *)sendBuffer, 100000);
//printf("HUF Original decompressed size: %i\n", decompressedLength);

	int diffedCompressedSize = 0;
	int difHufCompressedSize = 0;
	int undiffedSize = 0;
	if (hasPrevBuffer)
	{
		printf("Pre DIF Size: %i\n", sendBufferLength);
		
		diffedCompressedSize = dif(
			(const unsigned char*) prevBuffer, prevBufferLength, 
			(const unsigned char*) sendBuffer, sendBufferLength, 
			(unsigned char*) difCompressedBuffer, 100000);
		printf("DIF Size: %i\n", diffedCompressedSize);


		undiffedSize = undiff(
			(const unsigned char*) prevBuffer, prevBufferLength,
			(const unsigned char*) difCompressedBuffer, diffedCompressedSize, 
			(unsigned char*) decompressedBuffer, 100000);
		printf("UNDIF Size: %i\n", undiffedSize);


// to determine a fixed dictionary
// not needed in final version
void huffman_collect_bitlengths(const unsigned char* compressedBuffer, int diffedCompressedSize);
huffman_collect_bitlengths((const unsigned char*)compressedBuffer, diffedCompressedSize);


difHufCompressedSize = huffman_compress((const unsigned char*)difCompressedBuffer, diffedCompressedSize, (unsigned char *)difHufCompressedBuffer, 100000);
//printf("Dif HUF compressed size: %i\n", difHufCompressedSize);

int hufDecompressedLength = huffman_decompress((const unsigned char*)difHufCompressedBuffer, difHufCompressedSize, (unsigned char *)difCompressedBuffer, 100000);
//printf("HUF Original decompressed size: %i\n", hufDecompressedLength);


		int hufUndiffedSize = undiff(
			(const unsigned char*) prevBuffer, prevBufferLength,
			(const unsigned char*) difCompressedBuffer, hufDecompressedLength, 
			(unsigned char*) decompressedBuffer, 100000);
//		printf("HUF UNDIF Size: %i\n", hufUndiffedSize);
		(void) hufUndiffedSize;
		
		
		if (-1== compare_buffers((const unsigned char*) sendBuffer, (const unsigned char*) decompressedBuffer, sendBufferLength) )
			printf("Undif correct!\n");
		else
			printf("Undif failed!\n");



		printf("----\n");
		if ((dataMode&0b00000011)==0)
			printf("Mode: Full Lines\n");
		else if ((dataMode&0b00000011)==1)
			printf("Mode: Half delta\n");
		else if ((dataMode&0b00000011)==2)
			printf("Mode: Full delta\n");
		(void) lz4SizeSum;
		(void) difSizeSum;
		(void) hufSizeSum;
		(void) difHufSizeSum;
/*
		lz4SizeSum += lz4CompressedSize;
		printf("Lz4 average size: %i!\n",lz4SizeSum/round );
		hufSizeSum += hufCompressedSize;
		printf("Huf average size: %i!\n",hufSizeSum/round );
		difSizeSum += diffedCompressedSize;
		printf("Dif average size: %i!\n",difSizeSum/round );
		difHufSizeSum += difHufCompressedSize;
		printf("Dif Huf average size: %i!\n",difHufSizeSum/round );
		printf("----\n");
*/
	}


// Bit 0b0000 hdmm : mm 00 = full data, 01 = half delta, 10 = full delta
//                   d 0 = no dif, 1 = dif (previous buffer must be available), 
//					 h 0 = dif without Huffman, = 1 dif with Huffman (fixed dictionary)
//					 Hufman is chosen in the beginning with MAME communication	
// int dataMode = 0;
	int sendMode = dataMode & 0b00001111; 

any diff - does not work yet

everything else is "Baud" unstable :-(



	// ensure no huffman if not unknown fixed dictionary
	if (fixedHuffman==nullptr)
	{
		sendMode = sendMode&0b00000111;
	}

	if ((hasPrevBuffer == 0) || (undiffedSize<0)) // for whatever reason undiff can fail???
	{
		// ensure no dif Mode, since there is no previous buffer
		sendMode = sendMode&0b00000011;
		printf("No prevBuffer!\n");
	}


	piTrex_send_byte((sendMode)&0xff); //
	DEBUG_OUT("Mode: %i\n", sendMode);
	
	if ((sendMode & 0b00001100) == 0b00000000)
	{
		// no compression at all!
		piTrex_send_byte((sendBufferLength>>8)&0xff); // hi
		DEBUG_OUT("Hi\n");
		piTrex_send_byte((sendBufferLength)&0xff); 	  // lo
		DEBUG_OUT("Lo\n");
		DEBUG_OUT("Start sending buffer, size: %i\n", sendBufferLength);
		piTrex_send_bulk((const signed char*)sendBuffer, sendBufferLength);
	}
	else if ((sendMode & 0b00001100) == 0b00000100) // just dif
	{
		// dif
		piTrex_send_byte((diffedCompressedSize>>8)&0xff); // hi
		DEBUG_OUT("Just Dif\nHi\n");
		piTrex_send_byte((diffedCompressedSize)&0xff); 	  // lo
		DEBUG_OUT("Lo\n");
		DEBUG_OUT("Start sending buffer, size: %i\n", diffedCompressedSize);
		piTrex_send_bulk((const signed char*)difCompressedBuffer, diffedCompressedSize);
	}
	else if ((sendMode & 0b00001100) == 0b00001100) // dif + Huf
	{
		// dif + Huf
		piTrex_send_byte((difHufCompressedSize>>8)&0xff); // hi
		DEBUG_OUT("HufDif\nHi\n");
		piTrex_send_byte((difHufCompressedSize)&0xff); 	  // lo
		DEBUG_OUT("Lo\n");
		DEBUG_OUT("Start sending buffer, size: %i\n", difHufCompressedSize);
		piTrex_send_bulk((const signed char*)difHufCompressedBuffer, difHufCompressedSize);
	}

	if (hasPrevBuffer)
	{
	  DEBUG_OUT("\nPrev Buffer:\n");
	  for (int i=0;i<prevBufferLength; i++) 
	  {
		DEBUG_OUT("$%02x ", (unsigned char) prevBuffer[i]);
		if (((i+1)%16) == 0) DEBUG_OUT("\n");
	  }

	  DEBUG_OUT("\nDif Sent data:\n");
	  for (int i=0;i<diffedCompressedSize; i++) 
	  {
		DEBUG_OUT("$%02x ", (unsigned char) difCompressedBuffer[i]);
		if (((i+1)%16) == 0) DEBUG_OUT("\n");
	  }
	}
	
	

	// previous buffer is always the real big one thing!
	for (int i=0;i<sendBufferLength; i++) prevBuffer[i] = sendBuffer[i];
	prevBufferLength = sendBufferLength;
	hasPrevBuffer = 1;
	
	printf("\nNew previous Buffer was built, length: %i\n", prevBufferLength);



    QueryPerformanceCounter(&endTime);
    elapsedTimeMs = (double)(endTime.QuadPart - sendTime.QuadPart) * 1000.0 / frequency.QuadPart;
    DEBUG_OUT("\nSending data took: %.3f ms\n", elapsedTimeMs);

// for IRQ mode this is pretty important...
// for multi core - this slows things down...
// waiting for vectrex to process...
//Sleep(10); // working good!
//Sleep(5);

    // Get ending time
    QueryPerformanceCounter(&endTime);
    elapsedTimeMs = (double)(endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;
    DEBUG_OUT("\nComplete PiTrex processing time: %.3f ms\n", elapsedTimeMs);

	sendBufferLength = 0;
	QueryPerformanceCounter(&lastTime);
#endif

	return 0;
}

/*****************************************************************/
/****************************** LZ 4 *****************************/
/*****************************************************************/


#define MINMATCH 2
#define HASH_SIZE 256  // Small for 1-byte offset

static inline unsigned char hashN(const unsigned char* p) {
    return (unsigned char)(((p[0] << 3) ^ (p[1] << 2)) & 0xFF);
}
int miniLZ4_compress(const unsigned char* src, int srcSize, unsigned char* dst, int dstCapacity) {
    const unsigned char* ip = src;
    const unsigned char* anchor = ip;
    const unsigned char* iend = src + srcSize;
    const unsigned char* mflimit = iend - MINMATCH;

    unsigned char* op = dst;
    unsigned char* oend = dst + dstCapacity;

    const unsigned char* hashTable[HASH_SIZE] = { 0 };

    while (ip < mflimit) {
        unsigned char hash = hashN(ip);
        const unsigned char* match = hashTable[hash];
        hashTable[hash] = ip;

        if (match && (ip - match <= 255) && memcmp(match, ip, MINMATCH) == 0) {
            // Match found
            int literalLength = ip - anchor;

            unsigned char* token = op++;
            *token = (literalLength >= 15 ? 15 << 4 : literalLength << 4);

            // Encode literal length
            if (literalLength >= 15) {
                int len = literalLength - 15;
                while (len >= 255) { *op++ = 255; len -= 255; }
                *op++ = (unsigned char)len;
            }

            // Copy literals
            memcpy(op, anchor, literalLength);
            op += literalLength;

            // Write 1-byte offset
            *op++ = (unsigned char)(ip - match);



			ip += MINMATCH;
			match += MINMATCH;

			while (ip < iend && *ip == *match) 
			{
				ip++;
				match++;
			}

			int matchLength = ip - anchor - literalLength - MINMATCH;

            if (matchLength >= 15) {
                *token |= 15;
                matchLength -= 15;
                while (matchLength >= 255) { *op++ = 255; matchLength -= 255; }
                *op++ = (unsigned char)matchLength;
            } else {
                *token |= matchLength;
            }

            anchor = ip;
        } else {
            ip++;
        }
    }

    // Encode last literals
    int literalLength = iend - anchor;
    if (op + literalLength + 2 >= oend) return 0;

    unsigned char* token = op++;
    *token = (literalLength >= 15 ? 15 << 4 : literalLength << 4);

    if (literalLength >= 15) {
        int len = literalLength - 15;
        while (len >= 255) { *op++ = 255; len -= 255; }
        *op++ = (unsigned char)len;
    }

    memcpy(op, anchor, literalLength);
    op += literalLength;

    return (int)(op - dst);
}
int miniLZ4_decompress(const unsigned char* src, int compressedSize, unsigned char* dst, int dstCapacity) {
    const unsigned char* ip = src;
    const unsigned char* iend = src + compressedSize;
    unsigned char* op = dst;
    unsigned char* oend = dst + dstCapacity;

    while (ip < iend) {
        int token = *ip++;

        // Literal length
        int literalLength = token >> 4;
        if (literalLength == 15) {
            unsigned char len;
            do { len = *ip++; literalLength += len; } while (len == 255);
        }

        if (op + literalLength > oend || ip + literalLength > iend) return -1;
        memcpy(op, ip, literalLength);
        ip += literalLength;
        op += literalLength;

        if (ip >= iend) break;

        int offset = *ip++;  // 1-byte offset
        if (offset == 0 || op - offset < dst) return -2;

        unsigned char* match = op - offset;

        int matchLength = token & 0x0F;
        if (matchLength == 15) {
            unsigned char len;
            do { len = *ip++; matchLength += len; } while (len == 255);
        }

        matchLength += MINMATCH;

        if (op + matchLength > oend) return -3;

        while (matchLength--) *op++ = *match++;
    }

    return op - dst;
}

/*****************************************************************/
/*************************** HUFFMAN *****************************/
/*****************************************************************/
#define MAX_SYMBOLS 256
#define MAX_TREE_NODES (((MAX_SYMBOLS)*2)-1)
#define MAX_BITS 12
#define GLOBAL_HUF 1

typedef struct {
    uint32_t bits;
    int bitlen;
} HuffCode;
typedef struct {
    unsigned char* data;
    int bitpos;
    int bytepos;
    int maxlen;
} BitWriter;
typedef struct {
    const unsigned char* data;
    int bitpos;
    int bytepos;
    int length;
} BitReader;
typedef struct {
    int symbol;
    int freq;
} SymbolFreq;
typedef struct Node {
	int freq;
	int depth;
	struct Node* left;
	struct Node* right;
	int symbol;
} Node;
typedef struct {
    int symbol;
    int bitlen;
} SymbolLen;

HuffCode global_codes[256];
uint8_t global_bitlens[256] = {0};
static void bitwriter_init(BitWriter* bw, unsigned char* buf, int maxlen) {
    bw->data = buf;
    bw->bitpos = 0;
    bw->bytepos = 0;
    bw->maxlen = maxlen;
}
static int compare_symbol_len(const void* a, const void* b) {
    const SymbolLen* sa = (const SymbolLen*)a;
    const SymbolLen* sb = (const SymbolLen*)b;
    if (sa->bitlen != sb->bitlen)
        return sa->bitlen - sb->bitlen;
    return sa->symbol - sb->symbol;
}
static void bitwriter_write(BitWriter* bw, uint32_t bits, int count) {
    while (count--) {
        if (bw->bytepos >= bw->maxlen) return;
        if (bw->bitpos == 0) bw->data[bw->bytepos] = 0;

        if (bits & (1 << count)) bw->data[bw->bytepos] |= (1 << (7 - bw->bitpos));
        bw->bitpos++;
        if (bw->bitpos == 8) { bw->bitpos = 0; bw->bytepos++; }
    }
}
static int bitwriter_bytes_written(BitWriter* bw) {
    return bw->bytepos + (bw->bitpos > 0 ? 1 : 0);
}
static void bitreader_init(BitReader* br, const unsigned char* buf, int len) {
    br->data = buf;
    br->bitpos = 0;
    br->bytepos = 0;
    br->length = len;
}
static int bitreader_read(BitReader* br) {
    if (br->bytepos >= br->length) return -1;
    int bit = (br->data[br->bytepos] >> (7 - br->bitpos)) & 1;
    br->bitpos++;
    if (br->bitpos == 8) { br->bitpos = 0; br->bytepos++; }
    return bit;
}
void assign_lengths(Node* n, int depth, uint8_t* bitlens) {
    if (depth > MAX_BITS) depth = MAX_BITS;  // cap to 15 or user-defined limit

	if (!n->left && !n->right) {
		bitlens[n->symbol] = depth;
	} else {
		assign_lengths(n->left, depth + 1, bitlens);
		assign_lengths(n->right, depth + 1, bitlens);
	}
}
void free_tree(Node* n) {
	if (!n) return;
	free_tree(n->left);
	free_tree(n->right);
	free(n);
}
static void generate_bitlengths(int *freq, uint8_t* bitlens) {

    SymbolFreq symbols[MAX_SYMBOLS];
    int count = 0;
    for (int i = 0; i < MAX_SYMBOLS; ++i) {
        if (freq[i]) {
            symbols[count].symbol = i;
            symbols[count].freq = freq[i];
            count++;
        }
    }

    if (count == 0) return;
    if (count == 1) {
        bitlens[symbols[0].symbol] = 1;
        return;
    }


    Node* nodes[MAX_SYMBOLS * 2];
    int nodeCount = 0;

    for (int i = 0; i < count; ++i) {
        Node* n = (Node*)malloc(sizeof(Node));
        n->freq = symbols[i].freq;
        n->depth = 0;
        n->left = n->right = NULL;
        n->symbol = symbols[i].symbol;
        nodes[nodeCount++] = n;
    }

    while (nodeCount > 1) {
        // Sort nodes by frequency
        for (int i = 0; i < nodeCount - 1; ++i) {
            for (int j = i + 1; j < nodeCount; ++j) {
                if (nodes[i]->freq > nodes[j]->freq) {
                    Node* tmp = nodes[i];
                    nodes[i] = nodes[j];
                    nodes[j] = tmp;
                }
            }
        }

        Node* left = nodes[0];
        Node* right = nodes[1];

        Node* parent = (Node*)malloc(sizeof(Node));
        parent->freq = left->freq + right->freq;
        parent->left = left;
        parent->right = right;
        parent->depth = 0;
        parent->symbol = -1;

        nodes[0] = parent;
        nodes[1] = nodes[nodeCount - 1];
        nodeCount--;
    }


    assign_lengths(nodes[0], 0, bitlens);

    free_tree(nodes[0]);
}
static void generate_codes(const uint8_t* bitlens, HuffCode* codes) {
    SymbolLen sorted[MAX_SYMBOLS];
    int count = 0;

    for (int i = 0; i < MAX_SYMBOLS; ++i) {
        if (bitlens[i] > 0) {
            sorted[count].symbol = i;
            sorted[count].bitlen = bitlens[i];
            count++;
        }
    }

    qsort(sorted, count, sizeof(SymbolLen), compare_symbol_len);

    uint32_t code = 0;
    int prev_len = 0;

    for (int i = 0; i < count; ++i) {
        int len = sorted[i].bitlen;
        if (len != prev_len) {
            code <<= (len - prev_len);
            prev_len = len;
        }
        codes[sorted[i].symbol].bits = code;
        codes[sorted[i].symbol].bitlen = len;
        code++;
    }
}
int huffman_compress(const unsigned char* input, int length, unsigned char* output, int outMax) {
#if GLOBAL_HUF	
    HuffCode codes[256] = {0};
	uint8_t *local_bitlens=global_bitlens;
	if (fixedHuffman!=nullptr)
	{
		local_bitlens = fixedHuffman;
	}

    generate_codes(local_bitlens, codes);

	// Write original length (uint16_t)
	output[0] = (length >> 8) & 0xFF;
	output[1] = length & 0xFF;

	// ✅ Start bitstream after dictionary + 2-byte length
	BitWriter bw;
	bitwriter_init(&bw, output + 2, outMax - 2);



    for (int i = 0; i < length; ++i) {
        HuffCode c = codes[input[i]];
        bitwriter_write(&bw, c.bits, c.bitlen);
    }

	return 2 + bitwriter_bytes_written(&bw);
	
#else
    HuffCode codes[256] = {0};

    int freq[MAX_SYMBOLS] = {0};
    for (int i = 0; i < length; ++i) freq[input[i]]++;
	generate_bitlengths(freq, bitlens);
	
    generate_codes(bitlens, codes);

    if (outMax < 256) return -1;

	memcpy(output, bitlens, 256);

	// Write original length (uint16_t)
	output[256] = (length >> 8) & 0xFF;
	output[257] = length & 0xFF;

	// ✅ Start bitstream after dictionary + 2-byte length
	BitWriter bw;
	bitwriter_init(&bw, output + 258, outMax - 258);



    for (int i = 0; i < length; ++i) {
        HuffCode c = codes[input[i]];
        bitwriter_write(&bw, c.bits, c.bitlen);
    }

	return 258 + bitwriter_bytes_written(&bw);
#endif

}
int huffman_decompress(const unsigned char* input, int length, unsigned char* output, int outMax) {
#if GLOBAL_HUF	
    HuffCode codes[256];
	uint8_t *local_bitlens=global_bitlens;

	if (fixedHuffman!=nullptr)
	{
		local_bitlens = fixedHuffman;
	}

    generate_codes(local_bitlens, codes);

	// Read original length
	int expectedLength = (input[0] << 8) | input[1];

	BitReader br;
	bitreader_init(&br, input + 2, length - 2);

    int outPos = 0;
    while (outPos < expectedLength) {
        uint32_t code = 0;
        for (int bits = 1; bits <= MAX_BITS; ++bits) {
            int bit = bitreader_read(&br);
            if (bit < 0) return outPos;
            code = (code << 1) | bit;

            for (int i = 0; i < 256; ++i) {
                if (local_bitlens[i] == bits && codes[i].bits == code) {
                    output[outPos++] = (unsigned char)i;
                    goto found;
                }
            }
        }
    found:;
    }
    return outPos;
#else
    if (length < 256) return -1;

    uint8_t bitlens[256];
    HuffCode codes[256];
    memcpy(bitlens, input, 256);
    generate_codes(bitlens, codes);

	// Read original length
	int expectedLength = (input[256] << 8) | input[257];

	BitReader br;
	bitreader_init(&br, input + 258, length - 258);

    int outPos = 0;
    while (outPos < expectedLength) {
        uint32_t code = 0;
        for (int bits = 1; bits <= MAX_BITS; ++bits) {
            int bit = bitreader_read(&br);
            if (bit < 0) return outPos;
            code = (code << 1) | bit;

            for (int i = 0; i < 256; ++i) {
                if (bitlens[i] == bits && codes[i].bits == code) {
                    output[outPos++] = (unsigned char)i;
                    goto found;
                }
            }
        }
    found:;
    }
    return outPos;
#endif	
}


/*****************************************************************/
/****************************** Huffman Tool *********************/
/*****************************************************************/

static int global_histogram[256] = {0};
/*
static int build_tree(const int histogram[256], int sorted[256]) {
    int count = 0;
    for (int i = 0; i < 256; ++i) {
        if (histogram[i] > 0) {
            sorted[count++] = i;
        }
    }
    for (int i = 0; i < count - 1; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (histogram[sorted[j]] < histogram[sorted[i]]) {
                int tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }
    return count;
}
*/
void print_bitlength_table(const uint8_t bitlens[256]) {
    printf("const uint8_t huffman_bitlengths[256] = {\n");
    for (int i = 0; i < 256; ++i) {
        printf("%3d%s", bitlens[i], (i % 16 == 15) ? ",\n" : ", ");
    }
    printf("\n};\n\n");
}
void huffman_collect_bitlengths(const unsigned char* src, int srcLen) {
	
    for (int i = 0; i < srcLen; ++i) {
        global_histogram[src[i]]++;
    }
	for (int i=0;i<256;i++)
		if (global_histogram[i]==0) global_histogram[i]=1;

    generate_bitlengths(global_histogram, global_bitlens);
    print_bitlength_table(global_bitlens);
}

/*****************************************************************/
/*************************** FRAME DIFS 256 **********************/
/*****************************************************************/

// REALY BAD!
/*
// Compact and efficient byte-stream diff/undiff implementation using 256-byte chunks
// No temporary buffers or copying. All chunk handling is done via direct pointers.

#include <stdint.h>
#include <string.h>

#define DIFF_OP_END    0x00
#define DIFF_OP_COPY   0x01
#define DIFF_OP_INSERT 0x02
#define CHUNK_SIZE     256

// Create diff for one 256-byte chunk (respects prev_len and curr_len)
static int encode_chunk(const uint8_t *prev, int prev_len,
                        const uint8_t *curr, int curr_len,
                        uint8_t *diff, int diff_capacity) {
    int dp = 0;
    for (int i = 0; i < curr_len;) {
        int max_match = 0;
        int best_offset = 0;

        for (int off = 0; off < prev_len; ++off) {
            int match_len = 0;
            while (match_len < 255 &&
                   i + match_len < curr_len &&
                   off + match_len < prev_len &&
                   prev[off + match_len] == curr[i + match_len]) {
                match_len++;
            }
            if (match_len > max_match) {
                max_match = match_len;
                best_offset = off;
            }
        }

        if (max_match >= 4) {
            if (dp + 3 > diff_capacity) return -1;
            diff[dp++] = DIFF_OP_COPY;
            diff[dp++] = (uint8_t)best_offset;
            diff[dp++] = (uint8_t)max_match;
            i += max_match;
        } else {
            int insert_len = 1;
            while (insert_len < 255 && i + insert_len < curr_len) {
                int j = 0;
                while (j < 4 &&
                       i + insert_len + j < curr_len &&
                       j < prev_len &&
                       prev[i + insert_len + j] == curr[i + insert_len + j]) j++;
                if (j >= 4) break;
                insert_len++;
            }
            if (dp + 2 + insert_len > diff_capacity) return -2;
            diff[dp++] = DIFF_OP_INSERT;
            diff[dp++] = (uint8_t)insert_len;
            memcpy(&diff[dp], &curr[i], insert_len);
            dp += insert_len;
            i += insert_len;
        }
    }
    if (dp + 1 > diff_capacity) return -3;
    diff[dp++] = DIFF_OP_END;
    return dp;
}


// Apply diff to one 256-byte chunk
static int decode_chunk(const uint8_t *prev, int prevSize,
                        const uint8_t *diff, int diffSize,
                        uint8_t *out, int outCapacity,
                        int *bytes_read, int *bytes_written) {
    int dp = 0, cp = 0;

    while (dp < diffSize) {
        uint8_t op = diff[dp++];
        if (op == DIFF_OP_END) break;

        if (op == DIFF_OP_COPY) {
            if (dp + 2 > diffSize) 
			{
				printf("Decode Error -1\n");
				return -1;
			}	
            uint8_t offset = diff[dp++];
            uint8_t length = diff[dp++];
            if (offset + length > prevSize || cp + length > outCapacity) 
			{
				printf("Decode Error -2\n");
				return -2;
			}	
            memcpy(&out[cp], &prev[offset], length);
            cp += length;
        } else if (op == DIFF_OP_INSERT) {
            if (dp >= diffSize) 
			{
				printf("Decode Error -3\n");
				return -3;
			}	
			uint8_t length = diff[dp++];
            if (dp + length > diffSize || cp + length > outCapacity) 
			{
				printf("Decode Error -4\n");
				return -4;
			}	
            memcpy(&out[cp], &diff[dp], length);
            dp += length;
            cp += length;
        } else 
		{
			printf("Decode Error -5\n");
			return -5;
		}	
    }

    if (bytes_read) *bytes_read = dp;
    if (bytes_written) *bytes_written = cp;
    return 0;
}

// Top-level diff: process src vs. prev in 256-byte chunks
int dif(const unsigned char* prevBuffer, int prevBufferLength,
        const unsigned char* src, int srcSize,
        unsigned char* dst, int dstCapacity) {
    int processed = 0;
    int total_out = 0;

    while (processed < srcSize) {
        int chunk_len = (srcSize - processed >= CHUNK_SIZE) ? CHUNK_SIZE : (srcSize - processed);
        const uint8_t* currChunk = src + processed;

        const uint8_t* prevChunk = prevBuffer + processed;
        int prev_len = prevBufferLength - processed;
        if (prev_len > CHUNK_SIZE) prev_len = CHUNK_SIZE;
        if (prev_len < 0) prev_len = 0;

        int written = encode_chunk(prevChunk, prev_len, currChunk, chunk_len, dst + total_out, dstCapacity - total_out);
        if (written < 0) return written;

        processed += chunk_len;
        total_out += written;
    }
    return total_out;
}

// Top-level undiff: reconstruct full output from diff stream
int undiff(const unsigned char* prevBuffer, int prevBufferLength,
          const unsigned char* src, int srcSize,
          unsigned char* dst, int dstCapacity) {
    int dp = 0;
    int total_out = 0;
    int chunk_index = 0;

    while (dp < srcSize && total_out < dstCapacity) {
        const uint8_t* prevChunk = prevBuffer + chunk_index * CHUNK_SIZE;
        int prev_len = prevBufferLength - chunk_index * CHUNK_SIZE;
        if (prev_len > CHUNK_SIZE) prev_len = CHUNK_SIZE;
        if (prev_len < 0) prev_len = 0;

        int bytes_read = 0, bytes_written = 0;

        int result = decode_chunk(
            prevChunk, prev_len,
            src + dp, srcSize - dp,
            dst + total_out, dstCapacity - total_out,
            &bytes_read, &bytes_written);

        if (result < 0) return result;

        dp += bytes_read;
        total_out += bytes_written;
        chunk_index++;
    }
    return total_out;
}
*/

/*****************************************************************/
/*************************** FRAME DIFS 16 bit *******************/
/*****************************************************************/
/* working good!

#define DIFF_OP_END    0x00
#define DIFF_OP_COPY   0x01
#define DIFF_OP_INSERT 0x02

#define MIN_MATCH      4
#define HASH_SIZE2      4096
#define HASH_MASK      (HASH_SIZE2 - 1)

static inline uint16_t hash4(const uint8_t *p) {
    return ((p[0] << 4) ^ (p[1] << 2) ^ p[2] ^ (p[3] << 1)) & HASH_MASK;
}

int dif(const uint8_t* prev, int prevSize, const uint8_t* curr, int currSize,
        uint8_t* out, int outCapacity) {
    int op = 0;
    int ip = 0;

    uint16_t hash_table[HASH_SIZE2];
    memset(hash_table, 0xFF, sizeof(hash_table));

    // Index hash table for prevBuffer
    for (int i = 0; i < prevSize - MIN_MATCH; ++i) {
        uint16_t h = hash4(&prev[i]);
        hash_table[h] = i;
    }

    while (ip < currSize) {
        int match_len = 0;
        int match_offset = 0;

        if (ip < currSize - MIN_MATCH) {
            uint16_t h = hash4(&curr[ip]);
            int ref = hash_table[h];
            if (ref != 0xFFFF && ref < prevSize) {
                while (match_len < 255 &&
                       ip + match_len < currSize &&
                       ref + match_len < prevSize &&
                       curr[ip + match_len] == prev[ref + match_len]) {
                    match_len++;
                }
                if (match_len >= MIN_MATCH) {
                    match_offset = ref;
                }
            }
        }

        if (match_len >= MIN_MATCH) {
            if (op + 4 > outCapacity) return -1;
            out[op++] = DIFF_OP_COPY;
            out[op++] = (uint8_t)(match_offset >> 8);
            out[op++] = (uint8_t)(match_offset & 0xFF);
            out[op++] = (uint8_t)match_len;
            ip += match_len;
        } else {
            // INSERT single byte or literal run
            int lit_start = ip;
            ip++;
            while (ip < currSize) {
                uint16_t h = hash4(&curr[ip]);
                int ref = hash_table[h];
                int len = 0;
                if (ref != 0xFFFF && ref < prevSize) {
                    while (len < 255 &&
                           ip + len < currSize &&
                           ref + len < prevSize &&
                           curr[ip + len] == prev[ref + len]) {
                        len++;
                    }
                }
                if (len >= MIN_MATCH) break;
                ip++;
            }
            int lit_len = ip - lit_start;
            if (op + 2 + lit_len > outCapacity) return -2;
            out[op++] = DIFF_OP_INSERT;
            out[op++] = (uint8_t)lit_len;
            memcpy(&out[op], &curr[lit_start], lit_len);
            op += lit_len;
        }
    }

    if (op + 1 > outCapacity) return -3;
    out[op++] = DIFF_OP_END;
    return op;
}

int undiff(const uint8_t* prev, int prevSize,
           const uint8_t* src, int srcSize,
           uint8_t* dst, int dstCapacity) {
    int ip = 0, op = 0;

    while (ip < srcSize) {
        uint8_t opcode = src[ip++];
        if (opcode == DIFF_OP_END) break;

        if (opcode == DIFF_OP_COPY) {
            if (ip + 3 > srcSize) return -1;
            int offset = (src[ip++] << 8);
            offset |= src[ip++];
            int length = src[ip++];
            if (offset + length > prevSize || op + length > dstCapacity) return -2;
            memcpy(&dst[op], &prev[offset], length);
            op += length;
        } else if (opcode == DIFF_OP_INSERT) {
            if (ip >= srcSize) return -3;
            int length = src[ip++];
            if (ip + length > srcSize || op + length > dstCapacity) return -4;
            memcpy(&dst[op], &src[ip], length);
            ip += length;
            op += length;
        } else {
            return -5;
        }
    }
    return op;
} // end

*/

/*****************************************************************/
/*************************** FRAME DIFS + LZ *********************/
/*****************************************************************/
/*
Working good

#define DIFF_OP_END    0x00
#define DIFF_OP_COPY   0x01
#define DIFF_OP_INSERT 0x02

#include <string.h>
#include <stdint.h>

#define MIN_MATCH       4
#define MAX_OFFSET   65535
#define MAX_LENGTH     255
#define HASH_SIZE3    32768
#define HASH_DEPTH      8  // Max matches per hash bucket

typedef struct {
    int pos[HASH_DEPTH];
    int count;
} HashBucket;

static inline uint16_t hash4(const uint8_t *p) {
    return ((p[0] << 5) ^ (p[1] << 3) ^ (p[2] << 1) ^ p[3]) & (HASH_SIZE3 - 1);
}

int dif(const unsigned char* prevBuffer, int prevLen,
        const unsigned char* src, int srcLen,
        unsigned char* dst, int dstCap) {

    int dp = 0, sp = 0;

    HashBucket hashTable[HASH_SIZE3] = {0};

    for (int i = 0; i <= prevLen - MIN_MATCH; ++i) {
        uint16_t h = hash4(&prevBuffer[i]);
        HashBucket* b = &hashTable[h];
        if (b->count < HASH_DEPTH) {
            b->pos[b->count++] = i;
        }
    }

    while (sp < srcLen) {
        int best_len = 0;
        int best_offset = 0;

        if (sp <= srcLen - MIN_MATCH) {
            uint16_t h = hash4(&src[sp]);
            HashBucket* b = &hashTable[h];
            for (int k = 0; k < b->count; ++k) {
                int off = b->pos[k];
                int ml = 0;
                while (ml < MAX_LENGTH && sp + ml < srcLen &&
                       off + ml < prevLen &&
                       src[sp + ml] == prevBuffer[off + ml]) {
                    ml++;
                }
                if (ml > best_len) {
                    best_len = ml;
                    best_offset = off;
                }
            }
        }

        if (best_len >= MIN_MATCH) {
            if (dp + 4 > dstCap) return -1;
            dst[dp++] = DIFF_OP_COPY;
            dst[dp++] = (best_offset >> 8) & 0xFF;
            dst[dp++] = best_offset & 0xFF;
            dst[dp++] = best_len;
            sp += best_len;
        } else {
            int lit_start = sp++;
            while (sp < srcLen) {
                int lz_len = 0;
                if (sp <= srcLen - MIN_MATCH) {
                    uint16_t h = hash4(&src[sp]);
                    HashBucket* b = &hashTable[h];
                    for (int k = 0; k < b->count; ++k) {
                        int off = b->pos[k];
                        int ml = 0;
                        while (ml < MAX_LENGTH && sp + ml < srcLen &&
                               off + ml < prevLen &&
                               src[sp + ml] == prevBuffer[off + ml]) {
                            ml++;
                        }
                        if (ml >= MIN_MATCH) {
                            lz_len = ml;
                            break;
                        }
                    }
                }
                if (lz_len >= MIN_MATCH) break;
                sp++;
            }

            int lit_len = sp - lit_start;
            if (dp + 2 + lit_len > dstCap) return -2;
            dst[dp++] = DIFF_OP_INSERT;
            dst[dp++] = lit_len;
            memcpy(&dst[dp], &src[lit_start], lit_len);
            dp += lit_len;
        }
    }

    if (dp + 1 > dstCap) return -3;
    dst[dp++] = DIFF_OP_END;
    return dp;
}

int undiff(const unsigned char* prevBuffer, int prevLen,
          const unsigned char* src, int srcLen,
          unsigned char* dst, int dstCap) {

    int sp = 0, dp = 0;

    while (sp < srcLen && dp < dstCap) {
        uint8_t op = src[sp++];
        if (op == DIFF_OP_END) break;

        if (op == DIFF_OP_COPY) {
            if (sp + 3 > srcLen) return -1;
			int high = src[sp++];
			int low  = src[sp++];
			int offset = (high << 8) | low;
			
            uint8_t length = src[sp++];
            if (offset + length > prevLen || dp + length > dstCap) return -2;
            memcpy(&dst[dp], &prevBuffer[offset], length);
            dp += length;
        } else if (op == DIFF_OP_INSERT) {
            if (sp >= srcLen) return -3;
            uint8_t length = src[sp++];
            if (sp + length > srcLen || dp + length > dstCap) return -4;
            memcpy(&dst[dp], &src[sp], length);
            sp += length;
            dp += length;
        } else {
            return -5;
        }
    }
    return dp;
}
*/




/*****************************************************************/
/*************************** FRAME DIFS 16 bit *******************/
/*****************************************************************/
/*
COPY-Kommandos werden erst ab einer Mindestlänge (COPY_MIN_LEN) ausgeführt, um ineffiziente kurze COPYs zu vermeiden.

Literale werden nun gebündelt und erst beim nächsten COPY oder am Ende geschrieben („Literal Buffer“).

Das verbessert die Kompression merklich bei kleinen Veränderungen.

Ein paar kleine byts besser! (Lunar Lander, star wars ist schlechter

*/

/* working but not on "big" diffs
#define DIFF_OP_END    0x00
#define DIFF_OP_COPY   0x01
#define DIFF_OP_INSERT 0x02

#include <string.h>
#include <stdint.h>

#define MIN_MATCH       4
#define COPY_MIN_LEN    6  // Avoid short COPYs that waste space
#define MAX_OFFSET   65535
#define MAX_LENGTH     255
#define HASH_SIZE3    32768
#define HASH_DEPTH      8  // Max matches per hash bucket

typedef struct {
    int pos[HASH_DEPTH];
    int count;
} HashBucket;

static inline uint16_t hash4(const uint8_t *p) {
    return ((p[0] << 5) ^ (p[1] << 3) ^ (p[2] << 1) ^ p[3]) & (HASH_SIZE3 - 1);
}

int dif(const unsigned char* prevBuffer, int prevLen,
        const unsigned char* src, int srcLen,
        unsigned char* dst, int dstCap) {

    int dp = 0, sp = 0;
    HashBucket hashTable[HASH_SIZE3] = {0};

    for (int i = 0; i <= prevLen - MIN_MATCH; ++i) {
        uint16_t h = hash4(&prevBuffer[i]);
        HashBucket* b = &hashTable[h];
        if (b->count < HASH_DEPTH) {
            b->pos[b->count++] = i;
        }
    }

    uint8_t lit_buffer[256];
    int lit_len = 0;

    while (sp < srcLen) {
        int best_len = 0;
        int best_offset = 0;

        if (sp <= srcLen - MIN_MATCH) {
            uint16_t h = hash4(&src[sp]);
            HashBucket* b = &hashTable[h];
            for (int k = 0; k < b->count; ++k) {
                int off = b->pos[k];
                int ml = 0;
                while (ml < MAX_LENGTH && sp + ml < srcLen &&
                       off + ml < prevLen &&
                       src[sp + ml] == prevBuffer[off + ml]) {
                    ml++;
                }
                if (ml > best_len) {
                    best_len = ml;
                    best_offset = off;
                }
            }
        }

        if (best_len >= COPY_MIN_LEN) {
            // Flush any pending literals
            if (lit_len > 0) {
                if (dp + 2 + lit_len > dstCap) return -2;
                dst[dp++] = DIFF_OP_INSERT;
                dst[dp++] = lit_len;
                memcpy(&dst[dp], lit_buffer, lit_len);
                dp += lit_len;
                lit_len = 0;
            }
            if (dp + 4 > dstCap) return -1;
            dst[dp++] = DIFF_OP_COPY;
            dst[dp++] = (best_offset >> 8) & 0xFF;
            dst[dp++] = best_offset & 0xFF;
            dst[dp++] = best_len;
            sp += best_len;
        } else {
            if (lit_len < sizeof(lit_buffer)) {
                lit_buffer[lit_len++] = src[sp++];
            } else {
                // Flush buffer
                if (dp + 2 + lit_len > dstCap) return -3;
                dst[dp++] = DIFF_OP_INSERT;
                dst[dp++] = lit_len;
                memcpy(&dst[dp], lit_buffer, lit_len);
                dp += lit_len;
                lit_len = 0;
            }
        }
    }

    // Final flush
    if (lit_len > 0) {
        if (dp + 2 + lit_len > dstCap) return -4;
        dst[dp++] = DIFF_OP_INSERT;
        dst[dp++] = lit_len;
        memcpy(&dst[dp], lit_buffer, lit_len);
        dp += lit_len;
    }

    if (dp + 1 > dstCap) return -5;
    dst[dp++] = DIFF_OP_END;
    return dp;
}

int undiff(const unsigned char* prevBuffer, int prevLen,
          const unsigned char* src, int srcLen,
          unsigned char* dst, int dstCap) {

    int sp = 0, dp = 0;

    while (sp < srcLen && dp < dstCap) {
        uint8_t op = src[sp++];
        if (op == DIFF_OP_END) break;

        if (op == DIFF_OP_COPY) {
            if (sp + 3 > srcLen) return -1;
            int high = src[sp++];
            int low  = src[sp++];
            int offset = (high << 8) | low;

            uint8_t length = src[sp++];
            if (offset + length > prevLen || dp + length > dstCap) return -2;
            memcpy(&dst[dp], &prevBuffer[offset], length);
            dp += length;
        } else if (op == DIFF_OP_INSERT) {
            if (sp >= srcLen) return -3;
            uint8_t length = src[sp++];
            if (sp + length > srcLen || dp + length > dstCap) return -4;
            memcpy(&dst[dp], &src[sp], length);
            sp += length;
            dp += length;
        } else {
            return -5;
        }
    }
    return dp;
}
*/

#include <string.h>
#include <stdint.h>

#define DIFF_OP_END    0x00
#define DIFF_OP_COPY   0x01
#define DIFF_OP_INSERT 0x02

#define MIN_MATCH       4
#define COPY_MIN_LEN    6
#define MAX_OFFSET   65535
#define MAX_LENGTH     255
#define HASH_SIZE3    32768
#define HASH_DEPTH       8

typedef struct {
    int pos[HASH_DEPTH];
    int count;
} HashBucket;

static inline uint16_t hash4(const uint8_t *p) {
    return ((p[0] << 5) ^ (p[1] << 3) ^ (p[2] << 1) ^ p[3]) & (HASH_SIZE3 - 1);
}

int dif(const unsigned char* prevBuffer, int prevLen,
        const unsigned char* src, int srcLen,
        unsigned char* dst, int dstCap) {

    int dp = 0, sp = 0;
    HashBucket hashTable[HASH_SIZE3] = {0};

    for (int i = 0; i <= prevLen - MIN_MATCH; ++i) {
        uint16_t h = hash4(&prevBuffer[i]);
        HashBucket* b = &hashTable[h];
        if (b->count < HASH_DEPTH) {
            b->pos[b->count++] = i;
        }
    }

    uint8_t lit_buffer[255];
    int lit_len = 0;

    while (sp < srcLen) {
        int best_len = 0;
        int best_offset = 0;

        if (sp <= srcLen - MIN_MATCH) {
            uint16_t h = hash4(&src[sp]);
            HashBucket* b = &hashTable[h];
            for (int k = 0; k < b->count; ++k) {
                int off = b->pos[k];
                int ml = 0;
                while (ml < MAX_LENGTH &&
                       sp + ml < srcLen &&
                       off + ml < prevLen &&
                       src[sp + ml] == prevBuffer[off + ml]) {
                    ml++;
                }
                if (ml > best_len) {
                    best_len = ml;
                    best_offset = off;
                }
            }
        }

        if (best_len >= COPY_MIN_LEN) {
            if (lit_len > 0) {
                if (dp + 2 + lit_len > dstCap) return -2;
                dst[dp++] = DIFF_OP_INSERT;
                dst[dp++] = (uint8_t)lit_len;
                memcpy(&dst[dp], lit_buffer, lit_len);
                dp += lit_len;
                lit_len = 0;
            }
            if (dp + 4 > dstCap) return -1;
            dst[dp++] = DIFF_OP_COPY;
            dst[dp++] = (best_offset >> 8) & 0xFF;
            dst[dp++] = best_offset & 0xFF;
            dst[dp++] = (uint8_t)best_len;
            sp += best_len;
        } else {
            lit_buffer[lit_len++] = src[sp++];
            if (lit_len == 255) {
                if (dp + 2 + lit_len > dstCap) return -3;
                dst[dp++] = DIFF_OP_INSERT;
                dst[dp++] = (uint8_t)lit_len;
                memcpy(&dst[dp], lit_buffer, lit_len);
                dp += lit_len;
                lit_len = 0;
            }
        }
    }

    if (lit_len > 0) {
        if (dp + 2 + lit_len > dstCap) return -4;
        dst[dp++] = DIFF_OP_INSERT;
        dst[dp++] = (uint8_t)lit_len;
        memcpy(&dst[dp], lit_buffer, lit_len);
        dp += lit_len;
    }

    if (dp + 1 > dstCap) return -5;
    dst[dp++] = DIFF_OP_END;
    return dp;
}

int undiff(const unsigned char* prevBuffer, int prevLen,
           const unsigned char* src, int srcLen,
           unsigned char* dst, int dstCap) {

    int sp = 0, dp = 0;

    while (sp < srcLen && dp < dstCap) {
        uint8_t op = src[sp++];
        if (op == DIFF_OP_END) break;

        if (op == DIFF_OP_COPY) {
            if (sp + 3 > srcLen) return -1;
            int offset = (src[sp] << 8) | src[sp + 1];
            uint8_t length = src[sp + 2];
            sp += 3;
            if (offset + length > prevLen || dp + length > dstCap) return -2;
            memcpy(&dst[dp], &prevBuffer[offset], length);
            dp += length;

        } else if (op == DIFF_OP_INSERT) {
            if (sp >= srcLen) return -3;
            uint8_t length = src[sp++];
            if (sp + length > srcLen || dp + length > dstCap) return -4;
            memcpy(&dst[dp], &src[sp], length);
            sp += length;
            dp += length;

        } else {
            return -5;  // unknown opcode
        }
    }
    return dp;
}


/*****************************************************************/
/****************************** HELPER ***************************/
/*****************************************************************/

int compare_buffers(const unsigned char* a, const unsigned char* b, int length) {
    for (int i = 0; i < length; ++i) {
        if (a[i] != b[i])
            return i;  // Index des ersten Unterschieds
    }
    return -1; // -1 bedeutet: vollständig identisch
}

PathLengths calculatePathLengths(const std::list<SimpleVTermLine>& lines) {
    float drawLength = 0.0f;
    float moveLength = 0.0f;
    float prevX = 0.0f;
    float prevY = 0.0f;

    for (const auto& line : lines) {
        // Move distance
        float moveDx = line.x0 - prevX;
        float moveDy = line.y0 - prevY;
        float moveDist = std::sqrt(moveDx * moveDx + moveDy * moveDy);
        moveLength += moveDist;

        // Draw distance
        float dx = line.x1 - line.x0;
        float dy = line.y1 - line.y0;
        float drawDist = std::sqrt(dx * dx + dy * dy);
        drawLength += drawDist;

        prevX = line.x1;
        prevY = line.y1;

        if (line.count == 0)
            break;
    }
    return { drawLength, moveLength };
}
