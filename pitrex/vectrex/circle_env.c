#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <vectrex/osWrapper.h>
#include <vectrex/circle_env.h>



void LogWrite (const char *pSource,     // short name of module
           unsigned    Severity,        // see above
           const char *pMessage, ...)  // uses printf format options
{
    if (Severity>=3) return;
    va_list var;
    va_start (var, pMessage);
  printf("%s: Log:%i : %s - ignoring args!\n\r",pSource, Severity, pMessage);
    va_end (var);
}


typedef struct TPropertyBuffer
{
    u32 nBufferSize;            // bytes
    u32 nCode;
    #define CODE_REQUEST        0x00000000
    #define CODE_RESPONSE_SUCCESS   0x80000000
    #define CODE_RESPONSE_FAILURE   0x80000001
    u8  Tags[0];
    // end tag follows
}
TPropertyBuffer;





void BcmMailBoxFlush (TBcmMailBox *pThis);
unsigned BcmMailBoxRead (TBcmMailBox *pThis);
void BcmMailBoxWrite (TBcmMailBox *pThis, unsigned nData);

void BcmMailBox (TBcmMailBox *pThis, unsigned nChannel)
{
    pThis->m_nChannel = nChannel;
}

void _BcmMailBox (TBcmMailBox *pThis)
{
}

unsigned BcmMailBoxWriteRead (TBcmMailBox *pThis, unsigned nData)
{
    DataMemBarrier ();
    BcmMailBoxFlush (pThis);
    BcmMailBoxWrite (pThis, nData);
    unsigned nResult = BcmMailBoxRead (pThis);
    DataMemBarrier ();
    return nResult;
}



u32 read32 (u32 nAddress)
{
    return *(volatile u32 *) nAddress;
}

void write32 (u32 nAddress, u32 nValue)
{
    *(volatile u32 *) nAddress = nValue;
}

void BcmMailBoxFlush (TBcmMailBox *pThis)
{
    while (!(read32 (MAILBOX0_STATUS) & MAILBOX_STATUS_EMPTY))
    {
        read32 (MAILBOX0_READ);
        MsDelay (20);
    }
}

unsigned BcmMailBoxRead (TBcmMailBox *pThis)
{
    unsigned nResult;
    
    do
    {
        while (read32 (MAILBOX0_STATUS) & MAILBOX_STATUS_EMPTY)
        {
            // do nothing
        }
        
        nResult = read32 (MAILBOX0_READ);
    }
    while ((nResult & 0xF) != pThis->m_nChannel);       // channel number is in the lower 4 bits

    return nResult & ~0xF;
}

void BcmMailBoxWrite (TBcmMailBox *pThis, unsigned nData)
{
    while (read32 (MAILBOX1_STATUS) & MAILBOX_STATUS_FULL)
    {
        // do nothing
    }

    write32 (MAILBOX1_WRITE, pThis->m_nChannel | nData);    // channel number is in the lower 4 bits
}


void BcmPropertyTags (TBcmPropertyTags *pThis)
{
    BcmMailBox (&pThis->m_MailBox, BCM_MAILBOX_PROP_OUT);
}

void _BcmPropertyTags (TBcmPropertyTags *pThis)
{
    _BcmMailBox (&pThis->m_MailBox);
}

boolean BcmPropertyTagsGetTag (TBcmPropertyTags *pThis, u32 nTagId,
                   void *pTag, unsigned nTagSize, unsigned  nRequestParmSize)
{
    unsigned nBufferSize = sizeof (TPropertyBuffer) + nTagSize + sizeof (u32);

    // cannot use malloc() here because this is used before mem_init() is called
    u8 Buffer[nBufferSize + 15];
    TPropertyBuffer *pBuffer = (TPropertyBuffer *) (((u32) Buffer + 15) & ~15);
    
    pBuffer->nBufferSize = nBufferSize;
    pBuffer->nCode = CODE_REQUEST;
    memcpy (pBuffer->Tags, pTag, nTagSize);
    
    TPropertyTag *pHeader = (TPropertyTag *) pBuffer->Tags;
    pHeader->nTagId = nTagId;
    pHeader->nValueBufSize = nTagSize - sizeof (TPropertyTag);
    pHeader->nValueLength = nRequestParmSize & ~VALUE_LENGTH_RESPONSE;

    u32 *pEndTag = (u32 *) (pBuffer->Tags + nTagSize);
    *pEndTag = PROPTAG_END;

    CleanDataCache ();
    DataSyncBarrier ();

    u32 nBufferAddress = BUS_ADDRESS ((u32) pBuffer);
    if (BcmMailBoxWriteRead (&pThis->m_MailBox, nBufferAddress) != nBufferAddress)
    {
        return FALSE;
    }
    
    InvalidateDataCache ();
    DataSyncBarrier ();

    if (pBuffer->nCode != CODE_RESPONSE_SUCCESS)
    {
        return FALSE;
    }
    
    if (!(pHeader->nValueLength & VALUE_LENGTH_RESPONSE))
    {
        return FALSE;
    }
    
    pHeader->nValueLength &= ~VALUE_LENGTH_RESPONSE;
    if (pHeader->nValueLength == 0)
    {
        return FALSE;
    }

    memcpy (pTag, pBuffer->Tags, nTagSize);

    return TRUE;
}





unsigned StartKernelTimer (unsigned nDelay, TKernelTimerHandler *pHandler, void *pParam, void *pContext)
{
// MALBAN
// delay and jump to function
  
#ifndef HZ  
#define HZ 100   
#endif  
  
MsDelay(nDelay*1000/HZ);
pHandler(0, pParam, pContext);

  return 0;
}

void CancelKernelTimer (unsigned hTimer)
{
   // TimerCancelKernelTimer (TimerGet (), hTimer);
}










int SetPowerStateOn (unsigned nDeviceId)
{
    TBcmPropertyTags Tags;
    BcmPropertyTags (&Tags);
    TPropertyTagPowerState PowerState;
    PowerState.nDeviceId = nDeviceId;
    PowerState.nState = POWER_STATE_ON | POWER_STATE_WAIT;
    if (   !BcmPropertyTagsGetTag (&Tags, PROPTAG_SET_POWER_STATE, &PowerState, sizeof PowerState, 8)
        ||  (PowerState.nState & POWER_STATE_NO_DEVICE)
        || !(PowerState.nState & POWER_STATE_ON))
    {
        _BcmPropertyTags (&Tags);

        return 0;
    }
    
    _BcmPropertyTags (&Tags);

    return 1;
}

int GetMACAddress (unsigned char Buffer[6])
{
    TBcmPropertyTags Tags;
    BcmPropertyTags (&Tags);
    TPropertyTagMACAddress MACAddress;
    if (!BcmPropertyTagsGetTag (&Tags, PROPTAG_GET_MAC_ADDRESS, &MACAddress, sizeof MACAddress, 0))
    {
        _BcmPropertyTags (&Tags);

        return 0;
    }

    memcpy (Buffer, MACAddress.Address, 6);
    
    _BcmPropertyTags (&Tags);

    return 1;
}