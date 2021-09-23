#include "cppWrapper.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <vectrex/vectrexInterface.h>
  
  
void *__dso_handle WEAK;

void __aeabi_atexit (void *pThis, void (*pFunc)(void *pThis), void *pHandle) WEAK;

void __aeabi_atexit (void *pThis, void (*pFunc)(void *pThis), void *pHandle)
{
    // TODO
}

#if AARCH == 64

void __cxa_atexit (void *pThis, void (*pFunc)(void *pThis), void *pHandle) WEAK;

void __cxa_atexit (void *pThis, void (*pFunc)(void *pThis), void *pHandle)
{
    // TODO
}

#endif
void __sync_synchronize (void)
{
    DataSyncBarrier ();
}

unsigned RandomSeed() 
{
  unsigned int cc;
  CCNT(cc);
  return (unsigned) cc; 
}


#ifdef __cplusplus
}
#endif

