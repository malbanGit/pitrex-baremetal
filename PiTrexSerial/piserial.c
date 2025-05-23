#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <termios.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h> 
#include <unistd.h>
#include <time.h>

#define MAX_FILE_SIZE (1024*1024*10) // 10 MB Max

bool piTrex_open_connection(void);
void piTrex_close_connection(void);
bool piTrex_get_byte(unsigned char *data);
void piTrex_send_bytes(unsigned char *data, int length);
void piTrex_send_string(unsigned char *data);
void piTrex_send_bytes_slow(unsigned char *data, int length);

void terminalReceive();
void terminalReceiveBlocking(double seconds);

int transferFile(char *filename, char *outpath);
int transferCart(char *filename);
int transferBIOS(char *filename);

int waitForString(char *waitFor);
int waitForStringTime(char *waitFor, double timeout);

unsigned char data[MAX_FILE_SIZE]; 

static void dump_run(void);
static void dump_match(void);
static void find_match(off_t to);
void flushReceiveBuffer();

char *filename = NULL;
char *outpath = NULL;
int action = -1;
int vaction = -1;
int reset = 0;
int boot = 0;
int stay = 0;
int startVectrex = 0;
char *device = NULL;

int main(int argc, char *argv[]) 
{
    int opt;
    // Optionen f (string), a (integer), d (string)
    while ((opt = getopt(argc, argv, "o:i:a:d:hrbts")) != -1) {
        switch (opt) {
            case 't':
                stay = 1; 
                break;
            case 'b':
                boot = 1; 
                break;
            case 'r':
                reset = 1; 
                break;
            case 'i':
                filename = optarg; // Wert für -f speichern
                break;
            case 'o':
                outpath = optarg; // Wert für -f speichern
                break;
            case 'h':
				fprintf(stderr, "-i FILE_IN\n");
				fprintf(stderr, "-o FILE_OUT\n");
				fprintf(stderr, "-a ACTION (1=file transfer)\n");
				fprintf(stderr, "-v ACTION assumes vecx running (1=cart transfer, 2 BIOS transfer)\n");
				fprintf(stderr, "-s start vectrex\n");
				fprintf(stderr, "-r send reset\n");
				fprintf(stderr, "-b send reboot\n");
				fprintf(stderr, "-t stay in terminal mode\n");
				fprintf(stderr, "-d DEVICE (default: '/dev/ttyUSB0')\n");
				fprintf(stderr, "If no parameter given, then terminal display\n");
		return 0;
                break;
            case 'a':
                action = atoi(optarg); // Wert für -a in Integer konvertieren
                break;
            case 'v':
                vaction = atoi(optarg); // Wert für -v in Integer konvertieren
                break;
            case 'd':
                device = optarg; // Wert für -d speichern
                break;
            default: // Bei unbekannten Optionen
//                 fprintf(stderr, "Usage: %s -i FILE_IN -o FILE_OUT -a ACTION -d DEVICE (optional, default: '/dev/ttyUSB0')\n", argv[0]);
                return EXIT_FAILURE;
        }
    }
    
    if (!piTrex_open_connection()) 
    {
        return EXIT_FAILURE;
    }
    if ((action==-1) && (vaction==-1))
    {
      terminalReceiveBlocking(0);
      piTrex_close_connection();
      return EXIT_SUCCESS;
    }

    if (vaction==1)
    {
      if (!filename)
      {
		  fprintf(stderr, "FILE_IN must be given!\n");
		  piTrex_close_connection();
		  return EXIT_FAILURE;
      }
      transferCart(filename);
      terminalReceiveBlocking(0.3); // does PiTrex want to say something?
    }
    if (vaction==2)
    {
      if (!filename)
      {
		  fprintf(stderr, "FILE_IN must be given!\n");
		  piTrex_close_connection();
		  return EXIT_FAILURE;
      }
      transferBIOS(filename);
      terminalReceiveBlocking(0.3); // does PiTrex want to say something?
    }
    
    if (action==1)
    {
      if ((!filename) || (!outpath))
      {
		  fprintf(stderr, "Both FILE_IN and FILE_OUT must be given!\n");
		  piTrex_close_connection();
		  return EXIT_FAILURE;
      }
      
      transferFile(filename, outpath);
      terminalReceiveBlocking(0.3); // does PiTrex want to say something?
    }

	if (boot == 1)
	{
		piTrex_send_string("reboot\n");
		printf("Reboot sent...\n");
	}
	if (reset == 1)
	{
		piTrex_send_string("reset\n");
		printf("Reset sent...\n");
	}
	if (startVectrex == 1)
	{
		piTrex_send_string("vplayBuffer\n");
		printf("Play vectrex sent...\n");
	}
	if (stay == 1)
	{
		terminalReceiveBlocking(0);
	}
	piTrex_close_connection();
    return EXIT_SUCCESS;
}

int transferFile(char *filename, char *outpath)
{
    FILE *rom;
    assert(sizeof(unsigned int)==4);

    rom = fopen((const char*)filename,"r");
    if (rom == 0 ) 
    {
      printf("ERROR: unable to open file..\n");
      return EXIT_FAILURE;
    } 
    int romsize = 0;
    while (!feof(rom)) 
    {
	fread(&data[romsize++], 1, 1, rom);
	if (romsize >= MAX_FILE_SIZE)
	{
	  fclose (rom);
	  printf("error: File to large!");
	  return EXIT_FAILURE;
	}
    }
    fclose (rom);
    romsize -= 1;
    
    flushReceiveBuffer();
    
    printf("romsize: %i\n",romsize);

    unsigned char retval = -1;
       
    piTrex_send_string("filecopy\n");
    if (!waitForString("Pi:Ready"))
    {
	terminalReceive(); // flush
	return EXIT_FAILURE;
    }
    terminalReceive(); // flush
    
    char buffer[255]; // Array to store the converted string
    sprintf(buffer, "%d\n", romsize); // Convert to decimal string
    piTrex_send_bytes(buffer, strlen(buffer));

    sprintf(buffer, "%s\n", outpath); 
    piTrex_send_bytes(buffer, strlen(buffer));
    terminalReceive();// flush

    piTrex_send_bytes(data, romsize);
    if (!waitForStringTime("Pi:Data received",20.0))
    {
	return EXIT_FAILURE;
    }
    terminalReceive();// flush
    
    printf("Data sent.\n");
    return EXIT_SUCCESS;
}
// 1 = ok
// 0 = failure
int waitForString(char *waitFor)
{
  return waitForStringTime(waitFor, 2.0);
}
int waitForStringTime(char *waitFor, double timeout)
{
  clock_t start = clock();

  #define MAX_ACK_BUF 30
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
	  printf("PiTrex did not ackowledge command '%s'(timeout) (got: '%s').\n", waitFor, receiveBuffer);
	  return 0;
      }    
      if (!piTrex_get_byte(&c))
      {
	  printf("PiTrex did not ackowledge command '%s' (char timeout) (got: '%s').\n",waitFor, receiveBuffer);
	  return 0;
      }
      *(receiveBuffer+receiveLen)=c;
//  printf("%c=%02x",c,c);fflush(stdout);
      receiveLen++;
      if (receiveLen>=MAX_ACK_BUF-1)
      {
	    printf("Receive buffer overflow in ACK (got: '%s', %i, '%c').\n", receiveBuffer, receiveLen, c);
	    return 0;
      }
      *(receiveBuffer+receiveLen)=0;
      if (strcmp(waitFor,receiveBuffer) ==0)
      {
        printf("%s\n", receiveBuffer);
	return 1; // ok
      }
  }
  printf("ACK not received (got: '%s').\n", receiveBuffer);
  return 0;
}

int kbhit() {
    struct timeval tv = {0, 0}; // Kein Timeout
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    // Prüfen, ob ein Zeichen im Eingabepuffer ist
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0;
}

void set_non_blocking_mode(int enable) {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);

    if (enable) {
        tty.c_lflag &= ~(ICANON | ECHO); // Kanonischen Modus und Echo deaktivieren
    } else {
        tty.c_lflag |= (ICANON | ECHO); // Kanonischen Modus und Echo aktivieren
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}
void terminalReceiveBlocking(double seconds)
{
  if (seconds == 0)
  {
    char ch;
    set_non_blocking_mode(1);
    while (1)
    {
      if (kbhit()) 
      {
	ch = getchar(); // Zeichen lesen

	if (ch == 'x') 
	{
	  set_non_blocking_mode(0);
	  printf("\n"); 
	  return;
	} 

	if (ch != 0)
	{
	  printf("%c",ch);fflush(stdout);
	  piTrex_send_bytes_slow(&ch, 1);
	}
      }
      terminalReceive();
    }
  }
  else
  {
    clock_t start = clock();
    double timeout = seconds; // Timeout in seconds  
    
    char ch;
    set_non_blocking_mode(1);
    while (1)
    {
      double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
      if (elapsed >= timeout) 
	break;
      else
      {
	//printf(" %f ", elapsed);
	fflush(stdout);
      }
      if (kbhit()) 
      {
	ch = getchar(); // Zeichen lesen

	if (ch == 'x') 
	{
	  set_non_blocking_mode(0);
	  printf("\n");
	  return;
	} 
      }
      terminalReceive();
    }
    set_non_blocking_mode(0);
    printf("\n");
    return;
  }
}

void terminalReceive()
{
    unsigned char retval = -1;
    while (piTrex_get_byte(&retval))
    {
	printf("%c",retval);fflush(stdout);
    }
}

void flushReceiveBuffer()
{
    clock_t start = clock();
    double timeout = 0.1D; // Timeout in seconds  
    
    char ch;
    while (1)
    {
      unsigned char retval = -1;
      while (piTrex_get_byte(&retval));
      
      double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
      if (elapsed >= timeout) 
	break;
    }
}


static int piTrex_set_interface_attribs(int fd, int speed)
{
    struct termios tty;
    
    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }
    
    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);
    
    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */
    
    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;
    
    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;
    
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int piTrex_fd;

  
bool piTrex_get_byte(unsigned char *data) {
    int bailout = 100;  // 100*1000 microseconds = 0.1 second
    do {
        if (read (piTrex_fd, data, 1) > 0)
            return (true);
        usleep (1000);
    } while (--bailout > 0);
    return (false);
}

void piTrex_send_bytes(unsigned char *data, int length) {
    int serial_bytes = length;

    int timeout = 0;
    do {
        int wlen = write (piTrex_fd, data+length-serial_bytes, serial_bytes);
        if (wlen > 0) serial_bytes -= wlen; else timeout++;
        if (!serial_bytes) break;
    //} while (timeout < 1000 serial_bytes);
    } while (serial_bytes);
    tcdrain(piTrex_fd);
}

// slow, because pitrex is not in receive mode yet,
// uart active while pitrex does other things
void piTrex_send_bytes_slow(unsigned char *data, int length) 
{
    int serial_bytes = length;
    int timeout = 0;
    do {
        int wlen = write(piTrex_fd, data, 1);
	tcdrain(piTrex_fd);
	serial_bytes -= wlen; 
	data += wlen;
        usleep (1000);
    } while (serial_bytes);
}

void piTrex_send_string(unsigned char *data)
{
  piTrex_send_bytes_slow(data, strlen(data));
}

bool piTrex_open_connection(void) {
    const char *portname = "/dev/ttyUSB0";
    const char *portname1 = "/dev/ttyUSB1";
    
    char *usedPortName = NULL;
    int initDone = 0;
    if (device != NULL) 
    {
	usedPortName = device;
	piTrex_fd = open(usedPortName, O_RDWR | O_NOCTTY | O_NDELAY /*O_SYNC*/);
	if (!(piTrex_fd < 0))
	  initDone = 1;
    }
    if (!initDone) printf("Serial device %s - failed to connect.\n", usedPortName);

    if (!initDone)
    {
	usedPortName = (char *)portname;
	piTrex_fd = open(usedPortName, O_RDWR | O_NOCTTY | O_NDELAY /*O_SYNC*/);
	if (!(piTrex_fd < 0))
	  initDone = 1;
    }
    if (!initDone) printf("Serial device %s - failed to connect.\n", usedPortName);

    if (!initDone)
    {
	usedPortName = (char *)portname1;
	piTrex_fd = open(usedPortName, O_RDWR | O_NOCTTY | O_NDELAY /*O_SYNC*/);
	if (!(piTrex_fd < 0))
	  initDone = 1;
    }
    if (!initDone) 
    {
	printf("Serial device %s - failed to connect.\n", usedPortName);
	return false;
    }
    if (piTrex_set_interface_attribs (piTrex_fd, 921600))  // set speed to 921,600 bps, 8n1
    {
	printf("Serial device %s - failed to connect [attributes].\n", usedPortName);
        close (piTrex_fd);
        return false;
    }
    printf("Serial device %s - connected.\n", usedPortName);
    usleep (10000);
    return true;
}

void piTrex_close_connection(void) {
    close (piTrex_fd);
}



int transferCart(char *filename)
{
    FILE *rom;
    assert(sizeof(unsigned int)==4);

    rom = fopen((const char*)filename,"r");
    if (rom == 0 ) 
    {
      printf("ERROR: unable to open file..\n");
      return EXIT_FAILURE;
    } 
    int romsize = 0;
    while (!feof(rom)) 
    {
		fread(&data[romsize++], 1, 1, rom);
		if (romsize >= MAX_FILE_SIZE)
		{
		  fclose (rom);
		  printf("error: File to large!");
		  return EXIT_FAILURE;
		}
    }
    fclose (rom);
    romsize -= 1;
    
    flushReceiveBuffer();
    
    printf("romsize: %i\n",romsize);

    unsigned char retval = -1;
       
    piTrex_send_string("vloadBuffer\n");
    if (!waitForString("Pi:Ready"))
    {
		terminalReceive(); // flush
		return EXIT_FAILURE;
    }
    terminalReceive(); // flush
    
    char buffer[255]; // Array to store the converted string
    sprintf(buffer, "%d\n", romsize); // Convert to decimal string
    piTrex_send_bytes(buffer, strlen(buffer));

    sprintf(buffer, "%s\n", outpath); 
    piTrex_send_bytes(buffer, strlen(buffer));
    terminalReceive();// flush

    piTrex_send_bytes(data, romsize);
    if (!waitForStringTime("Pi:Data received",20.0))
    {
		return EXIT_FAILURE;
    }
    terminalReceive();// flush
    
    printf("Data sent.\n");
    return EXIT_SUCCESS;
}

int transferBIOS(char *filename)
{
    FILE *rom;
    assert(sizeof(unsigned int)==4);

    rom = fopen((const char*)filename,"r");
    if (rom == 0 ) 
    {
      printf("ERROR: unable to open file..\n");
      return EXIT_FAILURE;
    } 
    int romsize = 0;
    while (!feof(rom)) 
    {
		fread(&data[romsize++], 1, 1, rom);
		if (romsize >= MAX_FILE_SIZE)
		{
		  fclose (rom);
		  printf("error: File to large!");
		  return EXIT_FAILURE;
		}
    }
    fclose (rom);

	if ((size != 4096) && (size != 4096*2))
	{
	  fclose (rom);
	  printf("error: BIOS file size not correct!");
	  return EXIT_FAILURE;
	}
		
    romsize -= 1;
    
    flushReceiveBuffer();
    
    printf("romsize: %i\n",romsize);

    unsigned char retval = -1;

	if (romsize==4096-1)
		piTrex_send_string("vBios4K\n");
	else
		piTrex_send_string("vBios8K\n");
    if (!waitForString("Pi:Ready"))
    {
		terminalReceive(); // flush
		return EXIT_FAILURE;
    }
    terminalReceive(); // flush
    
    char buffer[255]; // Array to store the converted string
    sprintf(buffer, "%d\n", romsize); // Convert to decimal string
    piTrex_send_bytes(buffer, strlen(buffer));

    sprintf(buffer, "%s\n", outpath); 
    piTrex_send_bytes(buffer, strlen(buffer));
    terminalReceive();// flush

    piTrex_send_bytes(data, romsize);
    if (!waitForStringTime("Pi:Data received",20.0))
    {
		return EXIT_FAILURE;
    }
    terminalReceive();// flush
    
    printf("Data sent.\n");
    return EXIT_SUCCESS;
}
