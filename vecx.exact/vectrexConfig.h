#ifndef __VECX_CONFIG_H
#define __VECX_CONFIG_H

#define CUTOM_INIT
#define DIRECT_EMULATION
#define WAIT_EMULATION
#define EXACT_WAIT_EMULATION
#define THE_SPIKE_MUST_FLOW
#define BEDLAM_T2_HANDLING

#define ALLOW_ROM_WRITE
//#define BANKS_48

//#define FILE_PLAYER

#include <vectrex/osWrapper.h>
#include <vectrex/vectrexInterface.h>

#include "../vectrexBase/e6522.h"
#include "../vectrexBase/e6809.h"
#include "../vectrexBase/edac.h"

extern uint8_t cart[32768*9]; 
extern char bootUpName[128];
void loadVectrexBin(char *selectedName, uint8_t *loadMem);

// if 1 is returned, the normal vecx init is also run!
static inline int customInit()
{
  char *p = getLoadParameter(0);
  
  if (bootUpName[0] != (char) 0)
  {
      // using the same function - but load a "forced" Cart
      loadVectrexBin(getLoadParameter(0), cart);
      return 0;
  }
  printf("LOADING: %s\n", p);
  
  if (p == 0) return 1;
  if (*p == 0) return 1;
  loadVectrexBin(getLoadParameter(0), cart);
  return 0;
}



#endif

