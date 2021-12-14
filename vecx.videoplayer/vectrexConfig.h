#ifndef __VECX_CONFIG_H
#define __VECX_CONFIG_H

#define CUTOM_INIT
#define DIRECT_EMULATION
#define USE_VIA_QUICK_ACCESS
#define WAIT_EMULATION
#define FILE_PLAYER


#include <vectrex/osWrapper.h>
#include <vectrex/vectrexInterface.h>

#include "../vectrexBase/e6522.h"
#include "../vectrexBase/e6809.h"
#include "../vectrexBase/edac.h"

extern uint8_t cart[32768*2*4]; 
void loadVectrexBin(char *selectedName, uint8_t *loadMem);

// if 1 is returned, the normal vecx init is also run!
static inline int customInit()
{
  loadVectrexBin("MOVIEPLAYER.NIB", cart);
  return 0;
}



#endif

