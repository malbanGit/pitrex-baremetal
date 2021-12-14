#pragma once
#include <baremetal/pi_support.h>



typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef int     boolean;
#ifndef FALSE
#define FALSE       0
#define TRUE        1
#endif


#define LOG_ERROR   1
#define LOG_WARNING 2
#define LOG_NOTICE  3
#define LOG_DEBUG   4

void LogWrite (const char *pSource,     // short name of module
           unsigned    Severity,        // see above
           const char *pMessage, ...);  // uses printf format options



int SetPowerStateOn (unsigned nDeviceId);
int GetMACAddress (unsigned char Buffer[6]);


typedef unsigned long TKernelTimerHandle;
typedef void TKernelTimerHandler (TKernelTimerHandle hTimer, void *pParam, void *pContext);

// returns the timer handle (hTimer)
unsigned StartKernelTimer (unsigned         nHzDelay,   // in HZ units (see "system configuration" above)
               TKernelTimerHandler *pHandler,
               void *pParam, void *pContext);   // handed over to the timer handler

void CancelKernelTimer (unsigned hTimer);


#ifdef NDEBUG
	#define assert(expr)	((void) 0)
#else
	void assertion_failed (const char *pExpr, const char *pFile, unsigned nLine);

	#define assert(expr)	((expr)	? ((void) 0) : assertion_failed (#expr, __FILE__, __LINE__))
#endif













typedef struct TBcmMailBox
{
    unsigned m_nChannel;
}
TBcmMailBox;



#define PROPTAG_END         0x00000000

#define PROPTAG_GET_FIRMWARE_REVISION   0x00000001
#define PROPTAG_GET_BOARD_MODEL     0x00010001
#define PROPTAG_GET_BOARD_REVISION  0x00010002
#define PROPTAG_GET_MAC_ADDRESS     0x00010003
#define PROPTAG_GET_BOARD_SERIAL    0x00010004
#define PROPTAG_GET_ARM_MEMORY      0x00010005
#define PROPTAG_GET_VC_MEMORY       0x00010006
#define PROPTAG_SET_POWER_STATE     0x00028001
#define PROPTAG_GET_CLOCK_RATE      0x00030002
#define PROPTAG_GET_TEMPERATURE     0x00030006
#define PROPTAG_GET_EDID_BLOCK      0x00030020
#define PROPTAG_GET_DISPLAY_DIMENSIONS  0x00040003
#define PROPTAG_GET_COMMAND_LINE    0x00050001

typedef struct TPropertyTag
{
    u32 nTagId;
    u32 nValueBufSize;          // bytes, multiple of 4
    u32 nValueLength;           // bytes
    #define VALUE_LENGTH_RESPONSE   (1 << 31)
    //u8    ValueBuffer[0];         // must be padded to be 4 byte aligned
}
TPropertyTag;

typedef struct TPropertyTagSimple
{
    TPropertyTag    Tag;
    u32     nValue;
}
TPropertyTagSimple;

typedef struct TPropertyTagMACAddress
{
    TPropertyTag    Tag;
    u8      Address[6];
    u8      Padding[2];
}
TPropertyTagMACAddress;

typedef struct TPropertyTagSerial
{
    TPropertyTag    Tag;
    u32     Serial[2];
}
TPropertyTagSerial;

typedef struct TPropertyTagMemory
{
    TPropertyTag    Tag;
    u32     nBaseAddress;
    u32     nSize;          // bytes
}
TPropertyTagMemory;

typedef struct TPropertyTagPowerState
{
    TPropertyTag    Tag;
    u32     nDeviceId;
    #define DEVICE_ID_SD_CARD   0
    #define DEVICE_ID_USB_HCD   3
    u32     nState;
    #define POWER_STATE_OFF     (0 << 0)
    #define POWER_STATE_ON      (1 << 0)
    #define POWER_STATE_WAIT    (1 << 1)
    #define POWER_STATE_NO_DEVICE   (1 << 1)    // in response
}
TPropertyTagPowerState;

typedef struct TPropertyTagClockRate
{
    TPropertyTag    Tag;
    u32     nClockId;
    #define CLOCK_ID_EMMC       1
    #define CLOCK_ID_UART       2
    u32     nRate;          // Hz
}
TPropertyTagClockRate;

typedef struct TPropertyTemperature
{
    TPropertyTag    Tag;
    u32     nTemperatureId;
    #define TEMPERATURE_ID      0
    u32     nValue;         // degree Celsius * 1000
}
TPropertyTemperature;

typedef struct TPropertyTagEDIDBlock
{
    TPropertyTag    Tag;
    u32     nBlockNumber;
    #define EDID_FIRST_BLOCK    0
    u32     nStatus;
    #define EDID_STATUS_SUCCESS 0
    u8      Block[128];
}
TPropertyTagEDIDBlock;

typedef struct TPropertyTagDisplayDimensions
{
    TPropertyTag    Tag;
    u32     nWidth;
    u32     nHeight;
}
TPropertyTagDisplayDimensions;

typedef struct TPropertyTagCommandLine
{
    TPropertyTag    Tag;
    u8      String[1024];
}
TPropertyTagCommandLine;

typedef struct TBcmPropertyTags
{
    TBcmMailBox m_MailBox;
}
TBcmPropertyTags;

boolean BcmPropertyTagsGetTag (TBcmPropertyTags *pThis, 
                   u32    nTagId,           // tag identifier
                   void  *pTag,             // pointer to tag struct
                   unsigned  nTagSize,          // size of tag struct
                   unsigned  nRequestParmSize /* = 0 */);   // number of parameter bytes


#if RASPPI == 1
#define ARM_IO_BASE		0x20000000
#else
#define ARM_IO_BASE		0x3F000000
#endif

#define GPU_IO_BASE		0x7E000000

#define GPU_CACHED_BASE		0x40000000
#define GPU_UNCACHED_BASE	0xC0000000

#if RASPPI == 1
	#ifdef GPU_L2_CACHE_ENABLED
		#define _GPU_MEM_BASE	GPU_CACHED_BASE
	#else
		#define _GPU_MEM_BASE	GPU_UNCACHED_BASE
	#endif
#else
	#define _GPU_MEM_BASE	GPU_UNCACHED_BASE
#endif



// Convert physical ARM address into bus address
// (does even work, if a bus address is provided already)
#define BUS_ADDRESS(phys)   (((phys) & ~0xC0000000) | _GPU_MEM_BASE)

//
// General Purpose I/O
//
#define ARM_GPIO_BASE       (ARM_IO_BASE + 0x200000)

#define ARM_GPIO_GPFSEL0    (ARM_GPIO_BASE + 0x00)
#define ARM_GPIO_GPFSEL1    (ARM_GPIO_BASE + 0x04)
#define ARM_GPIO_GPFSEL4    (ARM_GPIO_BASE + 0x10)
#define ARM_GPIO_GPSET0     (ARM_GPIO_BASE + 0x1C)
#define ARM_GPIO_GPCLR0     (ARM_GPIO_BASE + 0x28)
#define ARM_GPIO_GPLEV0     (ARM_GPIO_BASE + 0x34)
#define ARM_GPIO_GPEDS0     (ARM_GPIO_BASE + 0x40)
#define ARM_GPIO_GPREN0     (ARM_GPIO_BASE + 0x4C)
#define ARM_GPIO_GPFEN0     (ARM_GPIO_BASE + 0x58)
#define ARM_GPIO_GPHEN0     (ARM_GPIO_BASE + 0x64)
#define ARM_GPIO_GPLEN0     (ARM_GPIO_BASE + 0x70)
#define ARM_GPIO_GPAREN0    (ARM_GPIO_BASE + 0x7C)
#define ARM_GPIO_GPAFEN0    (ARM_GPIO_BASE + 0x88)
#define ARM_GPIO_GPPUD      (ARM_GPIO_BASE + 0x94)
#define ARM_GPIO_GPPUDCLK0  (ARM_GPIO_BASE + 0x98)

//
// UART0
//
#define ARM_UART0_BASE      (ARM_IO_BASE + 0x201000)

#define ARM_UART0_DR        (ARM_UART0_BASE + 0x00)
#define ARM_UART0_FR        (ARM_UART0_BASE + 0x18)
#define ARM_UART0_IBRD      (ARM_UART0_BASE + 0x24)
#define ARM_UART0_FBRD      (ARM_UART0_BASE + 0x28)
#define ARM_UART0_LCRH      (ARM_UART0_BASE + 0x2C)
#define ARM_UART0_CR        (ARM_UART0_BASE + 0x30)
#define ARM_UART0_IFLS      (ARM_UART0_BASE + 0x34)
#define ARM_UART0_IMSC      (ARM_UART0_BASE + 0x38)
#define ARM_UART0_RIS       (ARM_UART0_BASE + 0x3C)
#define ARM_UART0_MIS       (ARM_UART0_BASE + 0x40)
#define ARM_UART0_ICR       (ARM_UART0_BASE + 0x44)

//
// System Timers
//
#define ARM_SYSTIMER_BASE   (ARM_IO_BASE + 0x3000)

#define ARM_SYSTIMER_CS     (ARM_SYSTIMER_BASE + 0x00)
#define ARM_SYSTIMER_CLO    (ARM_SYSTIMER_BASE + 0x04)
#define ARM_SYSTIMER_CHI    (ARM_SYSTIMER_BASE + 0x08)
#define ARM_SYSTIMER_C0     (ARM_SYSTIMER_BASE + 0x0C)
#define ARM_SYSTIMER_C1     (ARM_SYSTIMER_BASE + 0x10)
#define ARM_SYSTIMER_C2     (ARM_SYSTIMER_BASE + 0x14)
#define ARM_SYSTIMER_C3     (ARM_SYSTIMER_BASE + 0x18)

//
// Platform DMA Controller
//
#define ARM_DMA_BASE        (ARM_IO_BASE + 0x7000)

//
// Interrupt Controller
//
#define ARM_IC_BASE     (ARM_IO_BASE + 0xB000)

#define ARM_IC_IRQ_BASIC_PENDING  (ARM_IC_BASE + 0x200)
#define ARM_IC_IRQ_PENDING_1      (ARM_IC_BASE + 0x204)
#define ARM_IC_IRQ_PENDING_2      (ARM_IC_BASE + 0x208)
#define ARM_IC_FIQ_CONTROL    (ARM_IC_BASE + 0x20C)
#define ARM_IC_ENABLE_IRQS_1      (ARM_IC_BASE + 0x210)
#define ARM_IC_ENABLE_IRQS_2      (ARM_IC_BASE + 0x214)
#define ARM_IC_ENABLE_BASIC_IRQS  (ARM_IC_BASE + 0x218)
#define ARM_IC_DISABLE_IRQS_1     (ARM_IC_BASE + 0x21C)
#define ARM_IC_DISABLE_IRQS_2     (ARM_IC_BASE + 0x220)
#define ARM_IC_DISABLE_BASIC_IRQS (ARM_IC_BASE + 0x224)

//
// ARM Timer
//
#define ARM_TIMER_BASE      (ARM_IO_BASE + 0xB000)

#define ARM_TIMER_LOAD      (ARM_TIMER_BASE + 0x400)
#define ARM_TIMER_VALUE     (ARM_TIMER_BASE + 0x404)
#define ARM_TIMER_CTRL      (ARM_TIMER_BASE + 0x408)
#define ARM_TIMER_IRQCLR    (ARM_TIMER_BASE + 0x40C)
#define ARM_TIMER_RAWIRQ    (ARM_TIMER_BASE + 0x410)
#define ARM_TIMER_MASKIRQ   (ARM_TIMER_BASE + 0x414)
#define ARM_TIMER_RELOAD    (ARM_TIMER_BASE + 0x418)
#define ARM_TIMER_PREDIV    (ARM_TIMER_BASE + 0x41C)
#define ARM_TIMER_CNTR      (ARM_TIMER_BASE + 0x420)

//
// Mailbox
//
#define MAILBOX_BASE        (ARM_IO_BASE + 0xB880)

#define MAILBOX0_READ       (MAILBOX_BASE + 0x00)
#define MAILBOX0_STATUS     (MAILBOX_BASE + 0x18)
    #define MAILBOX_STATUS_EMPTY    0x40000000
#define MAILBOX1_WRITE      (MAILBOX_BASE + 0x20)
#define MAILBOX1_STATUS     (MAILBOX_BASE + 0x38)
    #define MAILBOX_STATUS_FULL 0x80000000

#define MAILBOX_CHANNEL_PM  0           // power management
#define MAILBOX_CHANNEL_FB  1           // frame buffer
#define BCM_MAILBOX_PROP_OUT    8           // property tags (ARM to VC)

//
// Pulse Width Modulator
//
#define ARM_PWM_BASE        (ARM_IO_BASE + 0x20C000)

#define ARM_PWM_CTL     (ARM_PWM_BASE + 0x00)
#define ARM_PWM_STA     (ARM_PWM_BASE + 0x04)
#define ARM_PWM_DMAC        (ARM_PWM_BASE + 0x08)
#define ARM_PWM_RNG1        (ARM_PWM_BASE + 0x10)
#define ARM_PWM_DAT1        (ARM_PWM_BASE + 0x14)
#define ARM_PWM_FIF1        (ARM_PWM_BASE + 0x18)
#define ARM_PWM_RNG2        (ARM_PWM_BASE + 0x20)
#define ARM_PWM_DAT2        (ARM_PWM_BASE + 0x24)

//
// Clock Manager
//
#define ARM_CM_BASE     (ARM_IO_BASE + 0x101000)

#define ARM_CM_GP0CTL       (ARM_CM_BASE + 0x70)
#define ARM_CM_GP0DIV       (ARM_CM_BASE + 0x74)

#define ARM_CM_PASSWD       (0x5A << 24)

//
// USB Host Controller
//
#define ARM_USB_BASE        (ARM_IO_BASE + 0x980000)

#define ARM_USB_CORE_BASE   ARM_USB_BASE
#define ARM_USB_HOST_BASE   (ARM_USB_BASE + 0x400)
#define ARM_USB_POWER       (ARM_USB_BASE + 0xE00)

//
// External Mass Media Controller (SD Card)
//
#define ARM_EMMC_BASE       (ARM_IO_BASE + 0x300000)

//
// Power Manager (?)
//
#define ARM_PM_BASE     (ARM_IO_BASE + 0x100000)

#define ARM_PM_RSTC     (ARM_PM_BASE + 0x1C)
#define ARM_PM_WDOG     (ARM_PM_BASE + 0x24)

#define ARM_PM_PASSWD       (0x5A << 24)



/* circle end */
