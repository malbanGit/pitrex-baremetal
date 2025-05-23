#include <stdio.h>
#include "pi_support.h"

extern int main(int argc, char *argv[]);

extern unsigned int auxControl;

/** Main function - we'll never return from here */
void  kernelMain( unsigned int r0, unsigned int r1, unsigned int atags )
{
    sys_time_init();
    (void) lib_bcm2835_vc_set_power_state(BCM2835_VC_POWER_ID_SDCARD, BCM2835_VC_SET_POWER_STATE_ON_WAIT);
    FRESULT result = f_mount(&fat_fs, (const TCHAR *) "", (BYTE) 1);
    if (result != FR_OK) 
    {
      char buffer[32];
      snprintf(buffer, 31, "f_mount failed! %d\n", (int) result);
      printf("NO filesystem...: %s\n\r", buffer);
    }
    else
    {
      printf("FAT filesystem found!\n\r");
    }
    
    
    /* Never exit as there is no OS to exit to! */
    while(1)
    {
      main(0,0); // starting the "normal" main
    }
}


