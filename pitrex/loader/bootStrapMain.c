///////////////////////////////////////////////////////////

/* This file is loaded as a kernel image to 0x8000
   and reloads the main "loader" - and positions it at 
   0x4000000 -> where it should not interfere with anything.
   
   If another program is loaded -> it will be loaded to 0x8000 and act as 
   normal kernel.
   
   The new "normal" kernel must do all setups again (mmu, stacks, handlers).
   
   But with this kind of indirection, any sort of kernel can be (re) started - even
   a normal raspbian!
*/

///////////////////////////////////////////////////////////

#include <stdio.h>
#include <baremetal/pi_support.h>

#define MAX_LOAD (1024*1024*100) // 100 MB


#if RASPPI != 1 
#define IMG_FILE_PREFIX "piZero2/"
#else
#define IMG_FILE_PREFIX "piZero1/"
#endif

void main()
{
	// mailboxes can not communicate when called from there (to high memory ranges)
	// mailboxes are important for e.g. sd cart init
	uint32_t progSpace = LOADER_START;
	
	static const TCHAR FILE_NAME_PARAMS[] = IMG_FILE_PREFIX "loader.pit";
	FRESULT rc_rd = FR_DISK_ERR;
	FIL file_object_rd;
	rc_rd = f_open(&file_object_rd, FILE_NAME_PARAMS, (BYTE) FA_READ);

	if (rc_rd != FR_OK)
	{
		printf("Could not open file %s (%i), %s \r\n", FILE_NAME_PARAMS, rc_rd, getErrorText(rc_rd));
	}
	else
	{
		unsigned int fsize = MAX_LOAD;
		rc_rd = f_read(&file_object_rd, (void *)progSpace, fsize, &fsize);
		if ( rc_rd!= FR_OK)
		{
			printf("File not loaded (size got = %i)\r\n", rc_rd);
			f_close(&file_object_rd);
		}
		else
		{
			f_close(&file_object_rd);
			// file is loaded
			printf("Starting loader...\r\n");
			void (*progStart)(void) = (void (*)(void))progSpace;
			progStart();
		}
	}
        while(1) {;}
}
