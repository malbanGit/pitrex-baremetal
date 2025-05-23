#ifndef __VECX_CONFIG_H
#define __VECX_CONFIG_H

#define CUTOM_INIT
#define DIRECT_EMULATION
#define USE_VIA_QUICK_ACCESS
#define EMULATE_KNOWN_PRINTSTR

#define THE_SPIKE_MUST_FLOW
#define BEDLAM_T2_HANDLING


#define WAIT_EMULATION // without this - speedy vectrex sprites "jitter"
//#define FILE_PLAYER


#include <vectrex/osWrapper.h>
#include <vectrex/vectrexInterface.h>

#include "../vectrexBase/e6522.h"
#include "../vectrexBase/e6809.h"
#include "../vectrexBase/edac.h"

extern uint8_t cart[32768*9]; 
void loadVectrexBin(char *selectedName, uint8_t *loadMem);

// if 1 is returned, the normal vecx init is also run!
static inline int customInit()
{
  char *p = getLoadParameter(0);
  if (p == 0) return 1;
  if (*p == 0) return 1;
  loadVectrexBin(p, cart);
  return 0;
}



#endif

