#include <stdio.h>
#include <ff.h>
#include <pitrex/pitrexio-gpio.h>

#include "baremetalUtil.h"
#include "vectrexInterface.h"



// also disables data cache
void mmu_disable(void)
{
   clean_data_cache();
    // disable MMU
    uint32_t control;
    asm volatile ("mrc p15, 0, %0, c1, c0,  0" : "=r" (control));
    control = control & (~MMU_MODE);
    asm volatile ("mcr p15, 0, %0, c1, c0,  0" : : "r" (control) : "memory");
}

void vfp_deinit(void) {
	unsigned cacr;		// Coprocessor Access Control Register
	__asm volatile ("mrc p15, 0, %0, c1, c0, 2" : "=r" (cacr));
	cacr &= ~(3 << 20);	// bit 20/21, Full Access, CP10
	cacr &= ~(3 << 22);	// bit 22/23, Full Access, CP11
	__asm volatile ("mcr p15, 0, %0, c1, c0, 2" : : "r" (cacr));
	isb();
}

// 32 bit
int peek(unsigned int address)
{
  int val;
  __asm__ __volatile__(
      "mov r3, %[value] \n\t"
      "ldr %[result], [r3]\n\t"
      : [result]"=r" (val) : [value]"r" (address): "r3", "cc");
  return val;
}

// 8 bit
unsigned char peekByte(unsigned int address)
{
  unsigned char val;
  __asm__ __volatile__(
    "mov r3, %[value] \n\t"
    "ldrb %[result], [r3]\n\t"
    : [result]"=r" (val) : [value]"r" (address): "r3", "cc");
  return val;
}



// 8 bit
void pokeByte(unsigned int address, unsigned char c)
{
  *((unsigned char *)address) = (unsigned char)c;
}

// 32 bit
void/*__attribute__ ((noinline)) */poke(unsigned int address, unsigned int value)
{
     *((unsigned int *)address) = value;
}
char *getLoadParameter(int i)
{
  switch (i)
  {
    case 0: return settings->parameter1;
    case 1: return settings->parameter2;
    case 2: return settings->parameter3;
    case 3: return settings->parameter4;
  }
  return 0;
}

// for file accesss
char * getErrorText(int errNo)
{
  switch (errNo)
  {
	case FR_OK:				/* (0) Succeeded */
	  return "OK";
	case FR_DISK_ERR:			/* (1) A hard error occurred in the low level disk I/O layer */
	  return "FR_DISK_ERR";
	case FR_INT_ERR:				/* (2) Assertion failed */
	  return "FR_INT_ERR";
	case FR_NOT_READY:			/* (3) The physical drive cannot work */
	  return "FR_NOT_READY";
	case FR_NO_FILE:				/* (4) Could not find the file */
	  return "FR_NO_FILE";
	case FR_NO_PATH:				/* (5) Could not find the path */
	  return "FR_NO_PATH";
	case FR_INVALID_NAME:		/* (6) The path name format is invalid */
	  return "FR_INVALID_NAME";
	case FR_DENIED:				/* (7) Access denied due to prohibited access or directory full */
	  return "FR_DENIED";
	case FR_EXIST:				/* (8) Access denied due to prohibited access */
	  return "FR_EXIST";
	case FR_INVALID_OBJECT:		/* (9) The file/directory object is invalid */
	  return "FR_INVALID_OBJECT";
	case FR_WRITE_PROTECTED:		/* (10) The physical drive is write protected */
	  return "FR_WRITE_PROTECTED";
	case FR_INVALID_DRIVE:		/* (11) The logical drive number is invalid */
	  return "FR_INVALID_DRIVE";
	case FR_NOT_ENABLED:			/* (12) The volume has no work area */
	  return "FR_NOT_ENABLED";
	case FR_NO_FILESYSTEM:		/* (13) There is no valid FAT volume */
	  return "FR_NO_FILESYSTEM";
	case FR_MKFS_ABORTED:		/* (14) The f_mkfs() aborted due to any problem */
	  return "FR_MKFS_ABORTED";
	case FR_TIMEOUT:				/* (15) Could not get a grant to access the volume within defined period */
	  return "FR_TIMEOUT";
	case FR_LOCKED:				/* (16) The operation is rejected according to the file sharing policy */
	  return "FR_LOCKED";
	case FR_NOT_ENOUGH_CORE:		/* (17) LFN working buffer could not be allocated */
	  return "FR_NOT_ENOUGH_CORE";
	case FR_TOO_MANY_OPEN_FILES:	/* (18) Number of open files > _FS_LOCK */
	  return "FR_TOO_MANY_OPEN_FILES";
	case FR_INVALID_PARAMETER:	/* (19) Given parameter is invalid */
	  return "FR_INVALID_PARAMETER";
  }
  char *t = "UNKOWN ERROR:      \r\n";
  sprintf(t, "UNKOWN ERROR: %i", errNo);
  return t;
}
