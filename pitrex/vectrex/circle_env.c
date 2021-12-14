#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <vectrex/osWrapper.h>
#include <vectrex/circle_env.h>

/*
#define LOG_ERROR   1
#define LOG_WARNING 2
#define LOG_NOTICE  3
#define LOG_DEBUG   4
*/
char logBuf[1024];
void LogWrite (const char *pSource,     // short name of module
           unsigned    Severity,        // see above
           const char *pMessage, ...)  // uses printf format options
{
#ifdef NDEBUG
    if (Severity>=3) return;
#endif    
    va_list var;
    va_start (var, pMessage);

    sprintf(logBuf, pMessage, var);
    
    printf("%s: Log:%i : %s\n\r",pSource, Severity, logBuf);
    va_end (var);
}


typedef struct TPropertyBuffer
{
    u32 nBufferSize;            // bytes
    u32 nCode;
    #define CODE_REQUEST            0x00000000
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

//uint32_t lib_bcm2835_mailbox_write_read(uint8_t channel, uint32_t data);
unsigned BcmMailBoxWriteRead (TBcmMailBox *pThis, unsigned nData)
{
 //return lib_bcm2835_mailbox_write_read(pThis->m_nChannel, nData);
  
    DataMemBarrier ();
    
    BcmMailBoxFlush (pThis);
    MsDelay (20);
    
    BcmMailBoxWrite (pThis, nData);
    MsDelay (20);
    unsigned nResult = BcmMailBoxRead (pThis);
    DataMemBarrier ();
    return nResult;
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
	assert (pThis != 0);

	assert (pTag != 0);
	assert (nTagSize >= sizeof (TPropertyTagSimple));
	unsigned nBufferSize = sizeof (TPropertyBuffer) + nTagSize + sizeof (u32);
	assert ((nBufferSize & 3) == 0);

	TPropertyBuffer *pBuffer = (TPropertyBuffer *) MEM_COHERENT_REGION;

	
	pBuffer->nBufferSize = nBufferSize;
	pBuffer->nCode = CODE_REQUEST;
	memcpy (pBuffer->Tags, pTag, nTagSize);
	
	TPropertyTag *pHeader = (TPropertyTag *) pBuffer->Tags;
	pHeader->nTagId = nTagId;
	pHeader->nValueBufSize = nTagSize - sizeof (TPropertyTag);
	pHeader->nValueLength = nRequestParmSize & ~VALUE_LENGTH_RESPONSE;

	u32 *pEndTag = (u32 *) (pBuffer->Tags + nTagSize);
	*pEndTag = PROPTAG_END;

#if RASPPI != 3
//	CleanDataCache ();
//	DataSyncBarrier ();
#endif
	u32 nBufferAddress = BUS_ADDRESS ((u32) pBuffer);
	if (BcmMailBoxWriteRead (&pThis->m_MailBox, nBufferAddress) != nBufferAddress)
	{
		return FALSE;
	}
	
#if RASPPI != 3
//	InvalidateDataCache ();
//	DataSyncBarrier ();
#endif
	DataMemBarrier ();

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





#ifndef NDEBUG

void assertion_failed (const char *pExpr, const char *pFile, unsigned nLine)
{
	unsigned int ulStackPtr;
	asm volatile ("mov %0,sp" : "=r" (ulStackPtr));

	char buf[1024] ;
	sprintf (buf, "%s(%u)", pFile, nLine);

//	debug_stacktrace ((unsigned int *) ulStackPtr, StringGet (&Source));
	
	printf("%s: assertion failed: %s",buf, pExpr);

//	_String (&Source);
}
void uspi_assertion_failed (const char *pExpr, const char *pFile, unsigned nLine)
{
	assertion_failed (pExpr, pFile, nLine);
}



static const char FromDebug[] = "debug";

void debug_hexdump (const void *pStart, unsigned nBytes, const char *pSource)
{
	u8 *pOffset = (u8 *) pStart;

	if (pSource == 0)
	{
		pSource = FromDebug;
	}

	printf("%s: Dumping 0x%X bytes starting at 0x%X", pSource, nBytes, (unsigned) pOffset);
	
	while (nBytes > 0)
	{
		printf("%s: %04X: %02X %02X %02X %02X %02X %02X %02X %02X-%02X %02X %02X %02X %02X %02X %02X %02X", pSource,
				(unsigned) pOffset & 0xFFFF,
				(unsigned) pOffset[0],  (unsigned) pOffset[1],  (unsigned) pOffset[2],  (unsigned) pOffset[3],
				(unsigned) pOffset[4],  (unsigned) pOffset[5],  (unsigned) pOffset[6],  (unsigned) pOffset[7],
				(unsigned) pOffset[8],  (unsigned) pOffset[9],  (unsigned) pOffset[10], (unsigned) pOffset[11],
				(unsigned) pOffset[12], (unsigned) pOffset[13], (unsigned) pOffset[14], (unsigned) pOffset[15]);

		pOffset += 16;

		if (nBytes >= 16)
		{
			nBytes -= 16;
		}
		else
		{
			nBytes = 0;
		}
	}
}
/*Ü
void debug_stacktrace (const u32 *pStackPtr, const char *pSource)
{
	if (pSource == 0)
	{
		pSource = FromDebug;
	}
	
	for (unsigned i = 0; i < 64; i++, pStackPtr++)
	{
		extern unsigned char _etext;

		if (   *pStackPtr >= MEM_KERNEL_START
		    && *pStackPtr < (u32) &_etext)
		{
			LoggerWrite (LoggerGet (), pSource, LogDebug, "stack[%u] is 0x%X", i, (unsigned) *pStackPtr);
		}
	}
}
*/

void DebugHexdump (const void *pBuffer, unsigned nBufLen, const char *pSource)
{
	debug_hexdump (pBuffer, nBufLen, pSource);
}

#endif






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