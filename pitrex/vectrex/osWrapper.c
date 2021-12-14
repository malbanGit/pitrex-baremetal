/* Required include for fstat() */
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include <sys/time.h> // for __gettimeofday

#include <baremetal/pitrexio-gpio.h>
#include "vectrexInterface.h"




//static FATFS fat_fs;            /* File system object - this is in pi_support.c now*/
int fsInit = 0;
int (*old_fprintf)(FILE *stream, const char *format, ...) = fprintf;

FF_WRAP ff_allFiles[MAX_FILE_OPEN];

// assuming for now only ONE directory open at one TIME
DIR currentDir;

void initFileSystem()
{
    if (fsInit!=0) return;
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	for (int i=0;i<MAX_FILE_OPEN; i++)
	{
		ff_allFiles[i].used = 0;
	}
    FRESULT result = f_mount(&fat_fs, (const TCHAR *) "", (unsigned char) 1);
    if (result != FR_OK)
    {
#ifndef DEBUG_TO_FILE
      printf("NO filesystem...!\r\n");
      printf("f_mount failed! %d\r\n", (int) result);
#endif
      v_error("FAT NOT FOUND");
    }
    else
    {
#ifndef DEBUG_TO_FILE
      printf("FAT filesystem found!\r\n");
#endif
    }
    fsInit=1;
}

// -1 on error
// number of file ide else
static int getFreeFilePos()
{
	for (int i=0;i<MAX_FILE_OPEN; i++)
	{
		if (ff_allFiles[i].used == 0) return i;
	}
	return -1;
}

static FF_WRAP *getFileWrapper(const char *filename, const char *mode)
{
	FF_WRAP * f = 0;
	int no = getFreeFilePos();
	if (no < 0)
    {
      return 0;
    }
	f = &ff_allFiles[no];

	strncpy(f->mode, mode,MODE_LEN-1);
	char *modePointer=f->mode;
	while (!modePointer!=0)
	{
		*modePointer = bm_tolower(*modePointer);
		modePointer++;
	}

	char sep[2];
	sep[0] =SEPERATOR;
	sep[1] =0;

	char tokenBuffer[PATH_MAX];
	strncpy(tokenBuffer, filename, PATH_MAX);

	f->pathCounter = 0;
	char *token = strtok(tokenBuffer, sep);
	strcpy(f->name, token);
	do
	{
		token = strtok(0, sep);
		if ((token == 0) ||(token[0]==0))
		{
			break;
		}
		strcpy(f->path[f->pathCounter], f->name);
		strcpy(f->name, token);
		f->pathCounter++;
        if (f->pathCounter>=PATH_MAX)
        {
          return 0;
        }
	} while(1); 
	f->modeTranslation = FA_OPEN_EXISTING;
	if (strstr(f->mode, "r") != 0) 
    {
      f->modeTranslation=FA_READ;
    }
	if (strstr(f->mode, "w") != 0) 
    {
      f->modeTranslation=FA_CREATE_ALWAYS|FA_WRITE;//FA_WRITE|FA_CREATE_ALWAYS|FA_CREATE_NEW;
    }
	if (strstr(f->mode, "a") != 0) 
    {
#define FA_SEEKEND  0x20    /* Seek to end of the file on file open */
      f->modeTranslation=FA_OPEN_APPEND | FA_SEEKEND | FA_WRITE;
    }
	if (strstr(f->mode, "+") != 0) 
    {
      f->modeTranslation=FA_READ|FA_WRITE|FA_OPEN_ALWAYS;
    }

/*
"r" Opens a file for reading. The file must exist.
"w" Creates an empty file for writing. If a file with the same name already exists, its content is erased and the file is considered as a new empty file.
"a" Appends to a file. Writing operations, append data at the end of the file. The file is created if it does not exist.
"r+" Opens a file to update both reading and writing. The file must exist.
"w+" Creates an empty file for both reading and writing.
"a+" Opens a file for reading and appending.

File access mode and open method flags (3rd argument of f_open)
#define	FA_READ				0x01
#define	FA_WRITE			0x02
#define	FA_OPEN_EXISTING	0x00
#define	FA_CREATE_NEW		0x04
#define	FA_CREATE_ALWAYS	0x08
#define	FA_OPEN_ALWAYS		0x10
#define	FA_OPEN_APPEND		0x30
*/
	f->used = 1;
	f->pos = 0;
	return f;
}

//int errno = 0;
int changeDirsTo(FF_WRAP *f)
{
	int pc = 0;
	while (pc<f->pathCounter)
	{
	  FRESULT rc_rd = f_chdir (f->path[pc]);
	  if (rc_rd != FR_OK)
	  {
		printf("NO directory found... (%s)!\r\n", f->path[pc]);
		return errno = 1;
	  }
	  pc++;
	}
	return errno = 0;
}

int changeDirsBack(FF_WRAP *f)
{
	int pc = 0;
	while (pc<f->pathCounter)
	{
	  f_chdir ("..");
	  pc++;
	}
	return errno = 0;
}
void outputCurrentDirectory()
{
  char buf[256];
  FRESULT result = f_getcwd (buf,256);
  if (result != FR_OK)
  {
    printf("f_getcwd failed (%i) \r\n", result);
    return ;
  }
  printf("Current directory: %s \r\n", buf);
  DIR dp;
  result = f_opendir (&dp, buf);
  if (result != FR_OK)
  {
    printf("f_opendir failed (%i) \r\n", result);
    return ;
  }
  printf("Contents: \r\n");
  printf("--------- \r\n");
  FILINFO finfo;
  while (result == FR_OK)
  {
    result =  f_readdir (&dp,&finfo);
    if (result == FR_OK)
    {
      /*
       *       typedef struct {
       *               FSIZE_t fsize;                  / * File size * /
       *               WORD    fdate;                  / * Modified date * /
       *               WORD    ftime;                  / * Modified time * /
       *               BYTE    fattrib;                / * File attribute * /
       *               TCHAR   altname[13];            / * Altenative file name * /
       *               TCHAR   fname[_MAX_LFN + 1];    / * Primary file name * /
    } FILINFO;
    */
    printf("\t %s\t%i\r\n",finfo.fname, finfo.fsize);
    }
  }
  printf("\r\n");
  f_closedir (&dp);
}
FILE *__fopen(const char *filename, const char *mode)
{
  initFileSystem();
	FF_WRAP *f = getFileWrapper(filename, mode);
	if (f == 0)
    {
      errno = E_SYSTEM_BOUNDS;
#ifndef DEBUG_TO_FILE
      printf("_fopen - wrapper error \r\n");
#endif      
      return 0;
    }
	f->result = FR_DISK_ERR; // default is error :-)
	changeDirsTo(f);
	f->result = f_open(&f->file, f->name,  f->modeTranslation);
	changeDirsBack(f);
	if (f->result != FR_OK)
	{
		f->used = 0;
		errno = E_FILE_NOT_FOUND;
#ifndef DEBUG_TO_FILE
		printf("Could not open file %s (%s) \r\n", filename, getErrorText(f->result));
#endif        
		return 0;
	}
	errno = E_OK;
	return (FILE *) f;
}
int __fclose(FILE *_f)
{
	FF_WRAP *f = (FF_WRAP *)_f;
	if (f->used==0)
    {
      errno = E_FILE_NOT_OPEN;
      return 0;
    }

	// close flushes
    f_close(&f->file);
	f->used = 0;
    errno = E_OK;
	return 0;
}

int __rename(const char *oldName, const char *newName)
{
  errno = E_OK;
  FRESULT r = f_rename (oldName, newName);
  if (r == FR_OK) return 0;
  errno = r;
  return -1;
}

int __remove(const char *name)
{
  errno = E_OK;
  FRESULT r = f_unlink(name);
  if (r == FR_OK) return 0;
  errno = r;
  return -1;
}


typedef void TInterruptHandler (void *pParam);

extern TInterruptHandler *usbIRQHandler;
extern void *usbIRQParam;
extern int useUSB;
extern int stdinEcho;
// unsigned char
int __fgetc(FILE *_f)
{
  unsigned char result;

  // this is a BLOCKING call!
  if (_f == stdin)
  {
    if ((usbKeyboardAvailable) && (useUSB == 1) && (usbIRQHandler != 0))
    {
      volatile int input = USPiKeyboardLastKeyDown();
      while (input == 0) 
      {
        usbIRQHandler (usbIRQParam);
        input = USPiKeyboardLastKeyDown();
      }
      if (stdinEcho) printf("%c", input);
      
      return input;
    }
  }

  int len = __fread(&result, 1, 1, _f);
  if (len == 0) return EOF;
  return result;
}

int __feof(FILE *_f)
{
    FF_WRAP *f = (FF_WRAP *)_f;
    return 0 == f->file.obj.objsize - f->file.fptr;
}



size_t __fread(void *ptr, size_t size, size_t nmemb, FILE *_f)
{
	FF_WRAP *f = (FF_WRAP *)_f;
	if (f->used==0)
	{
		errno = E_FILE_NOT_OPEN;
		return 0;
	}

	unsigned int lenLoaded=0;
	f->result = f_read(&f->file, ptr, size*nmemb, &lenLoaded);
	f->pos += lenLoaded;
	if ( f->result!= FR_OK)
	{
        if ((size != 1) && (nmemb != 1)) // fgetc()
          printf("fread() of %s fails (len loaded: %i/%i) (Error: %s)\r\n", f->name, lenLoaded,(size*nmemb), getErrorText(f->result));
		errno = E_READ_ERROR;
		return 0;
	}
    errno = E_OK;
	return lenLoaded/size; // return in number of "objects" read
}
size_t __fwrite(const void *ptr, size_t size, size_t nmemb, FILE *_f)
{
	FF_WRAP *f = (FF_WRAP *)_f;
	if (f->used==0)
	{
		errno = E_FILE_NOT_OPEN;
		return 0;
	}

	unsigned int lenSaved=0;
	f->result = f_write(&f->file, ptr, size*nmemb, &lenSaved);
	f->pos += lenSaved;
	if (( f->result!= FR_OK) || (size*nmemb != lenSaved))
	{
#ifndef DEBUG_TO_FILE
		printf("fwrite() of '%s' fails (len saved: %i/%i) (Error: %s)\r\n", f->name, lenSaved,(size*nmemb), getErrorText(f->result));
#endif        
		errno = E_WRITE_ERROR;
		return 0;
	}
    errno = E_OK;
	return lenSaved/size; // return in number of "objects" read
}

// different docomentation
// some say 0 on success and other is failure
// some say 0 is failure and the position is returned otherwise...
int __fseek(FILE *_f, long int offset, int whence)
{
	FF_WRAP *f = (FF_WRAP *)_f;
	if (f->used==0)
	{
		errno = E_FILE_NOT_OPEN;
		return -1;
	}
	if (whence==SEEK_SET) // SEEK_SET
	{
		// do nothing
	}
	else if (whence==SEEK_CUR) // SEEK_CUR
	{
		offset+=f->pos;
	}
	else if (whence==SEEK_END) // SEEK_END
	{
      changeDirsTo(f);
	  FILINFO finfo;
	  f->result = f_stat (f->name, &finfo);	/* Get file status */
      changeDirsBack(f);
	  if (f->result != FR_OK)
	  {
	      printf ("fseek(): could not open file %s (%s) \r\n", f->name, getErrorText (f->result));
	      errno = E_SEEK_ERROR;
	  }
	  offset = finfo.fsize-offset;
	}

    changeDirsTo(f);
	f->result =  f_lseek (&f->file, offset);
    changeDirsBack(f);
	if ( f->result!= FR_OK)
	{
		printf("Can't seek to %d in %s (error: %s)\r\n", offset, f->name, getErrorText(f->result));
		errno = E_SEEK_ERROR;
		return -1;
	}

	f->pos = offset;
	errno = E_OK;
	return 0;
}
int __ftell(FILE *_f)
{
    FF_WRAP *f = (FF_WRAP *)_f;
    return f->pos;
}

int __chdir(char *_dir)
{
  initFileSystem();
  FRESULT _result = f_chdir (_dir);
  if (_result != FR_OK)
  {
    printf("NO %s directory found...!\r\n", _dir);
    v_error("NO DIRECTORY");
  }
  errno = E_OK;
  return 0;
}

int __fflush(FILE *_f)
{
	FF_WRAP *f = (FF_WRAP *)_f;
	if (((int) f) == 1) // stdout
	{
	  // i assume the buffer is null
	  return 0;
	}
	if (((int) f) == 2) // stderr
	{
	  // i assume the buffer is null
	  return 0;
	}
	// file is expected
	f_sync (&f->file);
}
char *__getcwd(char *buf, size_t size)
{
    FRESULT result = f_getcwd (buf,256);
    if (result != FR_OK)
    {
      printf("f_getcwd failed (%i) \r\n", result);
      errno = E_OTHER;
      return 0;
    }
    errno = E_OK;
    return buf;
}
// assuming for now only ONE directory open at one TIME
// !!!
DIR *__opendir(char *name)
{
    FRESULT result = f_opendir (&currentDir, name);
    if (result != FR_OK)
    {
      printf("f_opendir failed '%s' (%i) \r\n",name, result);
      return 0;
    }
    return &currentDir;
}
// only 1 also!!!
FILINFO finfo;
dirent oneDirEntry;
dirent *__readdir(DIR *cd)
{
      FRESULT result  =  f_readdir (cd, &finfo);
      if (result != FR_OK)
      {
        return 0;
      }
      strncpy(oneDirEntry.d_name, finfo.fname, NAME_MAX);
      
      if (( finfo.fattrib& AM_DIR) == AM_DIR)
      {
        oneDirEntry.d_type = DT_DIR;
      }
      else
      {
        oneDirEntry.d_type = DT_REG;
      }
      
        
    return &oneDirEntry;
}
int __closedir(DIR *cd)
{
    FRESULT result = f_closedir (cd);
    if (result != FR_OK)
    {
      printf("f_closedir failed (%i) \r\n", result);
      return 0;
    }
    return 0;
}
int __filelength(FILE *fd)        // also used in inifile.c so not static
{
   int fsize;
   fseek (fd, 0, SEEK_END);
// my book says return 0 on success!
   fsize = ftell(fd); 
   (void) fseek (fd, 0, SEEK_SET);
   return fsize;
}

char *__fgets(char *str, int n, FILE *stream)
{
  FF_WRAP *f = (FF_WRAP *)stream;
  // printf("Trying to read max bytes: %i\r\n",n);  
  return (char *) f_gets (str, n, &f->file);
}

/* Put a string to the file */
int __fputs(const char *str, FILE *stream)
{
  if (stream == stdout)
  {
    fprintf(stdout, "%s", str); 
    errno = E_OK;
    return 1; // something positive not 0
  }
  
  FF_WRAP *f = (FF_WRAP *)stream;
  FRESULT result = f_puts (str, &f->file);
  if (result == FR_OK)
  {
    errno = E_OK;
    return 1; // something positive not 0
  }
  errno = result;
  return -1; // EOF?
}

/* Put a char to the file */
int __fputc(const char str, FILE *stream)
{
  if (stream == stdout)
  {
    fprintf(stdout, "%c", str); 
    errno = E_OK;
    return 1; // something positive not 0
  }
  
  FF_WRAP *f = (FF_WRAP *)stream;
  FRESULT result = f_putc (str, &f->file);
  if (result == FR_OK)
  {
    errno = E_OK;
    return 1; // something positive not 0
  }
  errno = result;
  return -1; // EOF?
}

//http://home.thep.lu.se/~bjorn/crc/

/* Simple public domain implementation of the standard CRC32 checksum.
 * Outputs the checksum for each file given as a command line argument.
 * Invalid file names and files that cause errors are silently skipped.
 * The program reads from stdin if it is called with no arguments. */

uint32_t crc32_for_byte(uint32_t r) 
{
  for(int j = 0; j < 8; ++j)
    r = (r & 1? 0: (uint32_t)0xEDB88320L) ^ r >> 1;
  return r ^ (uint32_t)0xFF000000L;
}

int crc32(const void *data, size_t n_bytes, uint32_t* crc) 
{
  static uint32_t table[0x100];
  if(!*table)
    for(size_t i = 0; i < 0x100; ++i)
      table[i] = crc32_for_byte(i);

  for(size_t i = 0; i < n_bytes; ++i)
    *crc = table[(uint8_t)*crc ^ ((uint8_t*)data)[i]] ^ *crc >> 8;
  return *crc; 
}


// returns -1410065407 on error
int bm_atoi(char* str, int radix)
{
    int doneSomething = 0;
    int res = 0; // Initialize result
    int mul = 1;
    int start = 0;
    while (str[start]==' ')start++; // trim
    if (str[start]=='+') {start++;}
    if (str[start]=='-') {start++; mul=-1;}

    // auto radix the "obvious" octal, binary, hex
    if (str[start]=='0')
    {
          doneSomething = 1;
        start++;
        radix = 8;
        if ((str[start]=='x') || (str[start]=='X') )
        {
            start++;
            radix = 16;
        }
    }
    if (str[start]=='$')
    {
        start++;
        radix = 16;
    }
    if (str[start]=='%')
    {
        start++;
        radix = 2;
    }

    for (int i = start; str[i] != '\0'; ++i)
    {
        char c = toupper(str[i]);
        if (c==' ') continue; // just ignore spaces
        if (c>'9') c -= 7;    // place alpha values next to digits
        c = c-'0';            // so we can just do a -'0'
        if (c>=radix)
        {
            //error;
                  return -1410065407;
        }
        res =res*radix+c;
                doneSomething=1;
    }
    if (!doneSomething) return -1410065407;
    return res*mul;
}

int usleep(useconds_t microSeconds)
{
  unsigned int t1OffMark = 0;
  CCNT0(t1OffMark);
  unsigned int value;
  t1OffMark += 1000*microSeconds;
  do
  {
    CCNT0(value);
  } while (value < t1OffMark );
  return 0;
}

void __libc_fini_array (void) {}

#include "puff.c"
#include "junzip.c"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif


#define BLOCK_ALIGN 16
#define ALIGN_MASK  (BLOCK_ALIGN-1)

#define PAGE_MASK   (PAGE_SIZE-1)

typedef struct TBlockHeader
{
    unsigned int    nMagic      PACKED;
#define BLOCK_MAGIC 0x424C4D43
    unsigned int    nSize       PACKED;
    struct TBlockHeader *pNext  PACKED;
    unsigned int    nPadding    PACKED;
    unsigned char   Data[0];
}
TBlockHeader;

typedef struct TBlockBucket
{
    unsigned int    nSize;
    TBlockHeader    *pFreeList;
}
TBlockBucket;


static unsigned char *s_pNextBlock;
static unsigned char *s_pBlockLimit;
static int memIsInit = 0;

extern unsigned char heap_low; /* Defined by the linker */
extern unsigned char heap_top; /* Defined by the linker */


static TBlockBucket s_BlockBucket[] = {{0x40}, {0x400}, {0x1000}, {0x4000}, {0x40000}, {0x80000}, {0}};

void mem_init (unsigned long ulBase, unsigned long ulSize);
#define INIT_MEM() mem_init((unsigned long) &heap_low, 1000*1000*10) // 10 MB


void mem_init (unsigned long ulBase, unsigned long ulSize)
{
    if (memIsInit) return;
    memIsInit = 1;
    unsigned long ulLimit = ulBase + ulSize;

    if (ulBase < (long unsigned int) &heap_low)
    {
        ulBase = (long unsigned int) &heap_low;
    }
    
    ulSize = ulLimit - ulBase;
    s_pNextBlock = (unsigned char *) ulBase;
    s_pBlockLimit = (unsigned char *) (ulBase + ulSize);
}

unsigned long mem_get_size (void)
{
    INIT_MEM();
    return (unsigned long) (s_pBlockLimit - s_pNextBlock);
}

void *__malloc (unsigned long ulSize)
{
    INIT_MEM();
    
    
    
    EnterCritical ();

    TBlockBucket *pBucket;
    for (pBucket = s_BlockBucket; pBucket->nSize > 0; pBucket++)
    {
        if (ulSize <= pBucket->nSize)
        {
            ulSize = pBucket->nSize;

            break;
        }
    }

    TBlockHeader *pBlockHeader;
    if (   pBucket->nSize > 0
        && (pBlockHeader = pBucket->pFreeList) != 0)
    {
        pBucket->pFreeList = pBlockHeader->pNext;
    }
    else
    {
        pBlockHeader = (TBlockHeader *) s_pNextBlock;

        s_pNextBlock += (sizeof (TBlockHeader) + ulSize + BLOCK_ALIGN-1) & ~ALIGN_MASK;
        if (s_pNextBlock > s_pBlockLimit)
        {
            LeaveCritical ();
printf("MALLOC PANIC!!!\n\r");
            return 0;       // TODO: system should panic here
        }
    
        pBlockHeader->nMagic = BLOCK_MAGIC;
        pBlockHeader->nSize = (unsigned) ulSize;
    }
    
    LeaveCritical ();

    pBlockHeader->pNext = 0;

    void *pResult = pBlockHeader->Data;
    return pResult;
}
void __free (void *pBlock)
{
    INIT_MEM();
    TBlockHeader *pBlockHeader = (TBlockHeader *) ((unsigned long) pBlock - sizeof (TBlockHeader));

    for (TBlockBucket *pBucket = s_BlockBucket; pBucket->nSize > 0; pBucket++)
    {
        if (pBlockHeader->nSize == pBucket->nSize)
        {
            EnterCritical ();

            pBlockHeader->pNext = pBucket->pFreeList;
            pBucket->pFreeList = pBlockHeader;
            LeaveCritical ();

            break;
        }
    }
}

unsigned int __sleep(unsigned int seconds)
{
  MsDelay (seconds*1000);
  return 0;
}

//#include <baremetal/rpi-systimer.h>

// this returns NOT the time since 1970
// but the time since start of the piTrex!
int __gettimeofday ( struct timeval *tp ,  __attribute__((unused)) struct timezone *tz )
{
  uint32_t microsSinceStart = v_micros();
  uint32_t secondsSinceStart = microsSinceStart / (1000*1000);
  
  tp->tv_sec = secondsSinceStart;
  tp->tv_usec = microsSinceStart- (secondsSinceStart*1000*1000);
  return 0;
}

