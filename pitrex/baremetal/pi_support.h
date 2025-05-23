#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h> 
#include "pitrexio-gpio.h"
#include "globalVars.h"





#define SD_WRITE_SUPPORT
extern volatile int printSemaphore;
                                     
                                     
extern int __bss_start__;
extern int __bss_end__;
#define CYCLE_COUNTER_ENABLE       (1<<0)  // enable all three counters   - E
#define COUNTER_ZERO               (1<<1)  // reset two count registers   - P
#define CYCLE_COUNTER_RESET        (1<<2)  // reset cycle counter register- C
#define CYCLE_COUNTER_DIVIDE_64    (1<<3)  // cycle count divider (64)    - D


#if RASPPI == 1
// https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
// https://www.keil.com/support/man/docs/armasm/armasm_dom1359731126962.htm

#define isb() asm volatile ("mcr p15, #0, %[zero], c7, c5,  #4" : : [zero] "r" (0) )
#define dsb() asm volatile ("mcr p15, #0, %[zero], c7, c10, #4" : : [zero] "r" (0) )
#define dmb() asm volatile ("mcr p15, #0, %[zero], c7, c10, #5" : : [zero] "r" (0) )

#define invalidate_instruction_cache()	asm volatile ("mcr p15, #0, %[zero], c7, c5,  #0" : : [zero] "r" (0) )
#define flush_prefetch_buffer()		asm volatile ("mcr p15, #0, %[zero], c7, c5,  #4" : : [zero] "r" (0) )
#define flush_branch_target_cache()	asm volatile ("mcr p15, #0, %[zero], c7, c5,  #6" : : [zero] "r" (0) )

#define invalidate_data_cache()		asm volatile ("mcr p15, 0, %0, c7, c6,  0\n" \
						      "mcr p15, 0, %0, c7, c10, 4\n" : : "r" (0) : "memory")

// same as flush_dcache_all
#define clean_data_cache		invalidate_data_cache
//
// Cache control
//
#define InvalidateInstructionCache 	invalidate_instruction_cache
#define FlushPrefetchBuffer 		flush_prefetch_buffer
#define FlushBranchTargetCache 		flush_branch_target_cache
#define InvalidateDataCache 		invalidate_data_cache
#define CleanDataCache 			clean_data_cache
#define imb				FlushPrefetchBuffer 

// https://www.raspberrypi.org/forums/viewtopic.php?f=63&t=155830
// https://developer.arm.com/documentation/ddi0301/h/DDI0301H_arm1176jzfs_r0p7_trm.pdf#page=270
// download PDF look at page 270

//%0000 11111111 11111111 0000 0000 0000
// 0x0ffff000


#define PMNC(v)  asm volatile("mcr p15, 0, %0, c15, c12, 0" :: "r"((v|0x0ffff000)))
#define CCNT(v)  asm volatile("mrc p15, 0, %0, c15, c12, 1" : "=r"(v))

// read COUNTER (not cycl counter) 0
// counter 0 and counter 1 both also count cycles
#define CCNT0(v)  asm volatile("mrc p15, 0, %0, c15, c12, 2" : "=r"(v))
#define CCNT1(v)  asm volatile("mrc p15, 0, %0, c15, c12, 3" : "=r"(v))
/*
static inline void cache_flush(void)
{
    unsigned long i = 0;
    // clean entire data cache 
    asm volatile("mcr p15, 0, %0, c7, c10, 0" : : "r" (i));
    // invalidate both caches and flush btb 
    asm volatile("mcr p15, 0, %0, c7, c7, 0" : : "r" (i));
    // mem barrier to sync things 
    asm volatile("mcr p15, 0, %0, c7, c10, 4" : : "r" (i));
}
*/
#else
#define isb() asm volatile ("isb" ::: "memory")
#define dsb() asm volatile ("dsb" ::: "memory")
#define dmb() asm volatile ("dmb" ::: "memory")
#define imb	isb 

#define invalidate_instruction_cache()	asm volatile ("mcr p15, #0, %[zero], c7, c5,  #0" : : [zero] "r" (0) : "memory")
#define flush_prefetch_buffer()		asm volatile ("isb" ::: "memory")
#define flush_branch_target_cache() 	asm volatile ("mcr p15, #0, %[zero], c7, c5,  #6" : : [zero] "r" (0) : "memory")

extern void invalidate_data_cache(void) __attribute__ ((optimize (3)));
extern void clean_data_cache(void) __attribute__ ((optimize (3)));
extern void invalidate_data_cache_l1_only(void) __attribute__ ((optimize (3)));

//
// Cache control
//
#define InvalidateInstructionCache 	invalidate_instruction_cache
#define FlushPrefetchBuffer 		flush_prefetch_buffer
#define FlushBranchTargetCache 		flush_branch_target_cache
#define InvalidateDataCache 		invalidate_data_cache
#define CleanDataCache 			clean_data_cache



// https://www.raspberrypi.org/forums/viewtopic.php?f=63&t=155830
// https://developer.arm.com/documentation/ddi0301/h/DDI0301H_arm1176jzfs_r0p7_trm.pdf#page=270
// download PDF look at page 270
#define CYCLE_COUNTER_ENABLE       (1<<0)  // enable all three counters   - E
#define COUNTER_ZERO               (1<<1)  // reset two count registers   - P
#define CYCLE_COUNTER_RESET        (1<<2)  // reset cycle counter register- C
#define CYCLE_COUNTER_DIVIDE_64    (1<<3)  // cycle count divider (64)    - D

#define CCNT(v)  asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(v))	// read cycle counter
#define PMNC(v)  asm volatile("mcr p15, 0, %0, c9, c12, 0" :: "r"((v))) // reset counter

// PMEVCNTR<n>, Performance Monitors Event Count Registers, n = 0 - 30
// Page: 7113
// read COUNTER# (not cycl counter) 0
#define CCNT0(v)  asm volatile("mrc p15, 0, %0, c14, c8, 0" : "=r"(v))
#define CCNT1(v)  asm volatile("mrc p15, 0, %0, c14, c8, 1" : "=r"(v))

#endif

#define ARM_CONTROL_MMU				(1 << 0)
#define ARM_CONTROL_STRICT_ALIGNMENT		(1 << 1)
#define ARM_CONTROL_L1_CACHE			(1 << 2)
#define ARM_CONTROL_BRANCH_PREDICTION		(1 << 11) // RES1 in ARM v8
#define ARM_CONTROL_L1_INSTRUCTION_CACHE 	(1 << 12)

/*
7085 event id

7097
SET DISABLE Counter
is enabled
READ asm volatile("mrc p15, 0, %0, c9, c12, 2" : "=r"(v))	// read ebale status of counter, 1 = enable
if 0x80000000 == 1 cycle counter enabled
if 0x00000001 == 1 counter 0 enabled
if 0x00000002 == 1 counter 1 enabled

SET Enable Counter
read same as above

WRITE
if 0x80000000 == 1 cycle counter enabled
if 0x00000001 == 1 counter 0 enabled
if 0x00000002 == 1 counter 1 enabled
WRITE  asm volatile("mcr p15, 0, %0, c9, c12, 1" :: "r"((v))) // reset counter
  

how many counters are implemented
N, bits [15:11]
READ asm volatile("mrc p15, 0, %0, c9, c12, 0" : "=r"(v))	// read ebale status of counter, 1 = enable




#define COUNTER_COUNT(v)  asm volatile("mrc p15, 0, %0, c9, c12, 0" : "=r"(v))
#define EVENT_ENABLED(v) asm volatile("mrc p15, 0, %0, c9, c12, 2" : "=r"(v))	// read ebale status of counter, 1 = enable
*/




#if RASPPI == 1
#define ARM_CONTROL_UNALIGNED_PERMITTED		(1 << 22) // RES1 in ARM v8
#define ARM_CONTROL_EXTENDED_PAGE_TABLE		(1 << 23)
#define ARM_AUX_CONTROL_CACHE_SIZE		(1 << 6)	// restrict cache size to 16K (no page coloring)
#endif


#if RASPPI == 1
#define MMU_MODE	(  ARM_CONTROL_MMU			\
			 | ARM_CONTROL_L1_CACHE			\
			 | ARM_CONTROL_L1_INSTRUCTION_CACHE	\
			 | ARM_CONTROL_BRANCH_PREDICTION	\
			 | ARM_CONTROL_EXTENDED_PAGE_TABLE)
#else
#define MMU_MODE	(  ARM_CONTROL_MMU			\
			 | ARM_CONTROL_L1_CACHE			\
			 | ARM_CONTROL_L1_INSTRUCTION_CACHE	\
			 | ARM_CONTROL_BRANCH_PREDICTION)
#endif


// todo
// possibly I must use assembler

#define EnableInterrupts()  __asm volatile ("cpsie i")
#define DisableInterrupts() __asm volatile ("cpsid i")
// possible in baremetalEntry.S
// extern void _enable_interrupts(void);
// extern void _disable_interrupts(void);

//
// Barriers
//
#define DataSyncBarrier 	dsb
#define DataMemBarrier 		dmb
#define InstructionSyncBarrier 	isb
#define InstructionMemBarrier   imb
#define CompilerBarrier()   __asm volatile ("" ::: "memory")

void EnterCritical (void);
void LeaveCritical (void);

//void icache_disable(void);
//void dcache_disable(void);




// https://www.raspberrypi.org/forums/viewtopic.php?f=63&t=155830
static inline void init_perfcounters (int do_reset, int enable_divider)
{
  // in general enable all counters (including cycle counter)
  int value = CYCLE_COUNTER_ENABLE;

  // peform reset:
  if (do_reset)
  {
    value |= COUNTER_ZERO;     // reset all counters to zero.
    value |= CYCLE_COUNTER_RESET;     // reset cycle counter to zero.
  }

  if (enable_divider)
    value |= CYCLE_COUNTER_DIVIDE_64;     // enable "by 64" divider for CCNT.

  PMNC(value);

}
// http://infocenter.arm.com/help/topic/com.arm.doc.ddi0301h/DDI0301H_arm1176jzfs_r0p7_trm.pdf#page=270
//[6] ECCUsed to enable and disable Cycle Counter interrupt reporting:0 = Disable interrupt, reset value1 = Enable interrupt.

static inline unsigned int get_cyclecount (void)
{
  unsigned int value;
  // Read CCNT Register
  CCNT(value);
  return value;
}

// waiter in arm cycles
// that means in nano seconds
static inline unsigned int waitUntil(unsigned int waiter)
{
  unsigned int value;
  // Read CCNT Register
  do
  {
    CCNT(value);
  } while (value < waiter);
  return value;
}



void mmu_disable(void);
char * getErrorText(int errNo);
void vfp_deinit(void);

int peek(unsigned int address);
unsigned char peekByte(unsigned int address);
void pokeByte(unsigned int address, unsigned char c);
char *getLoadParameter(int i);


void/*__attribute__ ((noinline)) */poke(unsigned int address, unsigned int value);














#if RASPPI == 1
    #define PERIPHERAL_BASE     0x20000000UL 
#else
    #if RASPPI == 4
	#define PERIPHERAL_BASE     0xFE000000UL
    #else // 3 and zero 2
	#define PERIPHERAL_BASE     0x3F000000UL
    #endif                            
#endif

#if RASPPI >= 2

  #if RASPPI <= 3
  #define ARM_LOCAL_BASE			0x40000000
  #else
  #define ARM_LOCAL_BASE			0xFF800000
  #endif

#define ARM_LOCAL_CONTROL		(ARM_LOCAL_BASE + 0x000)
#define ARM_LOCAL_PRESCALER		(ARM_LOCAL_BASE + 0x008)
#define ARM_LOCAL_GPU_INT_ROUTING	(ARM_LOCAL_BASE + 0x00C)
#define ARM_LOCAL_PM_ROUTING_SET	(ARM_LOCAL_BASE + 0x010)
#define ARM_LOCAL_PM_ROUTING_CLR	(ARM_LOCAL_BASE + 0x014)
#define ARM_LOCAL_TIMER_LS		(ARM_LOCAL_BASE + 0x01C)
#define ARM_LOCAL_TIMER_MS		(ARM_LOCAL_BASE + 0x020)
#define ARM_LOCAL_INT_ROUTING		(ARM_LOCAL_BASE + 0x024)
#define ARM_LOCAL_AXI_COUNT		(ARM_LOCAL_BASE + 0x02C)
#define ARM_LOCAL_AXI_IRQ		(ARM_LOCAL_BASE + 0x030)
#define ARM_LOCAL_TIMER_CONTROL		(ARM_LOCAL_BASE + 0x034)
#define ARM_LOCAL_TIMER_WRITE		(ARM_LOCAL_BASE + 0x038)

#define ARM_LOCAL_TIMER_INT_CONTROL0	(ARM_LOCAL_BASE + 0x040)
#define ARM_LOCAL_TIMER_INT_CONTROL1	(ARM_LOCAL_BASE + 0x044)
#define ARM_LOCAL_TIMER_INT_CONTROL2	(ARM_LOCAL_BASE + 0x048)
#define ARM_LOCAL_TIMER_INT_CONTROL3	(ARM_LOCAL_BASE + 0x04C)

#define ARM_LOCAL_MAILBOX_INT_CONTROL0	(ARM_LOCAL_BASE + 0x050)
#define ARM_LOCAL_MAILBOX_INT_CONTROL1	(ARM_LOCAL_BASE + 0x054)
#define ARM_LOCAL_MAILBOX_INT_CONTROL2	(ARM_LOCAL_BASE + 0x058)
#define ARM_LOCAL_MAILBOX_INT_CONTROL3	(ARM_LOCAL_BASE + 0x05C)

#define ARM_LOCAL_IRQ_PENDING0		(ARM_LOCAL_BASE + 0x060)
#define ARM_LOCAL_IRQ_PENDING1		(ARM_LOCAL_BASE + 0x064)
#define ARM_LOCAL_IRQ_PENDING2		(ARM_LOCAL_BASE + 0x068)
#define ARM_LOCAL_IRQ_PENDING3		(ARM_LOCAL_BASE + 0x06C)

#define ARM_LOCAL_FIQ_PENDING0		(ARM_LOCAL_BASE + 0x070)
#define ARM_LOCAL_FIQ_PENDING1		(ARM_LOCAL_BASE + 0x074)
#define ARM_LOCAL_FIQ_PENDING2		(ARM_LOCAL_BASE + 0x078)
#define ARM_LOCAL_FIQ_PENDING3		(ARM_LOCAL_BASE + 0x07C)

#define ARM_LOCAL_MAILBOX0_SET0		(ARM_LOCAL_BASE + 0x080)
#define ARM_LOCAL_MAILBOX1_SET0		(ARM_LOCAL_BASE + 0x084)
#define ARM_LOCAL_MAILBOX2_SET0		(ARM_LOCAL_BASE + 0x088)
#define ARM_LOCAL_MAILBOX3_SET0		(ARM_LOCAL_BASE + 0x08C)

#define ARM_LOCAL_MAILBOX0_SET1		(ARM_LOCAL_BASE + 0x090)
#define ARM_LOCAL_MAILBOX1_SET1		(ARM_LOCAL_BASE + 0x094)
#define ARM_LOCAL_MAILBOX2_SET1		(ARM_LOCAL_BASE + 0x098)
#define ARM_LOCAL_MAILBOX3_SET1		(ARM_LOCAL_BASE + 0x09C)

#define ARM_LOCAL_MAILBOX0_SET2		(ARM_LOCAL_BASE + 0x0A0)
#define ARM_LOCAL_MAILBOX1_SET2		(ARM_LOCAL_BASE + 0x0A4)
#define ARM_LOCAL_MAILBOX2_SET2		(ARM_LOCAL_BASE + 0x0A8)
#define ARM_LOCAL_MAILBOX3_SET2		(ARM_LOCAL_BASE + 0x0AC)

#define ARM_LOCAL_MAILBOX0_SET3		(ARM_LOCAL_BASE + 0x0B0)
#define ARM_LOCAL_MAILBOX1_SET3		(ARM_LOCAL_BASE + 0x0B4)
#define ARM_LOCAL_MAILBOX2_SET3		(ARM_LOCAL_BASE + 0x0B8)
#define ARM_LOCAL_MAILBOX3_SET3		(ARM_LOCAL_BASE + 0x0BC)

#define ARM_LOCAL_MAILBOX0_CLR0		(ARM_LOCAL_BASE + 0x0C0)
#define ARM_LOCAL_MAILBOX1_CLR0		(ARM_LOCAL_BASE + 0x0C4)
#define ARM_LOCAL_MAILBOX2_CLR0		(ARM_LOCAL_BASE + 0x0C8)
#define ARM_LOCAL_MAILBOX3_CLR0		(ARM_LOCAL_BASE + 0x0CC)

#define ARM_LOCAL_MAILBOX0_CLR1		(ARM_LOCAL_BASE + 0x0D0)
#define ARM_LOCAL_MAILBOX1_CLR1		(ARM_LOCAL_BASE + 0x0D4)
#define ARM_LOCAL_MAILBOX2_CLR1		(ARM_LOCAL_BASE + 0x0D8)
#define ARM_LOCAL_MAILBOX3_CLR1		(ARM_LOCAL_BASE + 0x0DC)

#define ARM_LOCAL_MAILBOX0_CLR2		(ARM_LOCAL_BASE + 0x0E0)
#define ARM_LOCAL_MAILBOX1_CLR2		(ARM_LOCAL_BASE + 0x0E4)
#define ARM_LOCAL_MAILBOX2_CLR2		(ARM_LOCAL_BASE + 0x0E8)
#define ARM_LOCAL_MAILBOX3_CLR2		(ARM_LOCAL_BASE + 0x0EC)

#define ARM_LOCAL_MAILBOX0_CLR3		(ARM_LOCAL_BASE + 0x0F0)
#define ARM_LOCAL_MAILBOX1_CLR3		(ARM_LOCAL_BASE + 0x0F4)
#define ARM_LOCAL_MAILBOX2_CLR3		(ARM_LOCAL_BASE + 0x0F8)
#define ARM_LOCAL_MAILBOX3_CLR3		(ARM_LOCAL_BASE + 0x0FC)


#endif




extern void tweakVectors();


typedef volatile uint32_t rpi_reg_rw_t;
typedef volatile const uint32_t rpi_reg_ro_t;
typedef volatile uint32_t rpi_reg_wo_t;

typedef volatile uint64_t rpi_wreg_rw_t;
typedef volatile const uint64_t rpi_wreg_ro_t;

#define BCM2835_ST_CS_M0		((uint32_t)(1 << 0))	///< System Timer Match 0. DO NOT USE; is used by GPU.
#define BCM2835_ST_CS_M1		((uint32_t)(1 << 1))	///< System Timer Match 1
#define BCM2835_ST_CS_M2		((uint32_t)(1 << 2))	///< System Timer Match 2. DO NOT USE; is used by GPU.
#define BCM2835_ST_CS_M3		((uint32_t)(1 << 3))	///< System Timer Match 3
extern const uint64_t lib_bcm2835_st_read(void);

/// Function select modes for \ref lib_bcm2835_gpio_fsel
typedef enum {
	BCM2835_GPIO_FSEL_INPT = 0x00,	///< 0b000,	Input
	BCM2835_GPIO_FSEL_OUTP = 0x01,	///< 0b001,	Output
	BCM2835_GPIO_FSEL_ALT0 = 0x04,	///< 0b100,	Alternate function 0
	BCM2835_GPIO_FSEL_ALT1 = 0x05,	///< 0b101,	Alternate function 1
	BCM2835_GPIO_FSEL_ALT2 = 0x06,	///< 0b110,	Alternate function 2
	BCM2835_GPIO_FSEL_ALT3 = 0x07,	///< 0b111,	Alternate function 3
	BCM2835_GPIO_FSEL_ALT4 = 0x03,	///< 0b011,	Alternate function 4
	BCM2835_GPIO_FSEL_ALT5 = 0x02,	///< 0b010,	Alternate function 5
	BCM2835_GPIO_FSEL_MASK = 0x07	///< 0b111, Function select bits mask
} bcm2835FunctionSelect;

/// Pullup/Pulldown defines for \ref lib_bcm2835_gpio_set_pud
typedef enum {
	BCM2835_GPIO_PUD_OFF 	= 0x00,	///< 0b00, Off ? disable pull-up/down
	BCM2835_GPIO_PUD_DOWN 	= 0x01,	///< 0b01, Enable Pull Down control
	BCM2835_GPIO_PUD_UP 	= 0x02	///< 0b10, Enable Pull Up control
} bcm2835PUDControl;
#define BCM2835_PERI_SET_BITS(a, v, m)		a = ((a) & ~(m)) | ((v) & (m));

/**
 * @ingroup GPIO
 *
 * Defined in file lib_bcm2835_gpio_fsel.S
 *
 * Sets the Function Select register for the given pin, which configures
 * the pin as Input, Output or one of the 6 alternate functions.
 *
 * @param pin GPIO number.
 * @param mode Mode to set the pin to, one of BCM2835_GPIO_FSEL_* from \ref bcm2835FunctionSelect
 */
extern void lib_bcm2835_gpio_fsel(const uint8_t, const uint8_t);





#define RPI_GPIO_BASE       ( PERIPHERAL_BASE + 0x200000UL )




typedef enum {
    FS_INPUT = 0,
    FS_OUTPUT,
    FS_ALT5,
    FS_ALT4,
    FS_ALT0,
    FS_ALT1,
    FS_ALT2,
    FS_ALT3,
    } rpi_gpio_alt_function_t;

/* A mask to be able to clear the bits in the register before setting the
   value we require */
#define FS_MASK     (7)

typedef enum {
    RPI_GPIO0 = 0,
    RPI_GPIO1,
    RPI_GPIO2,
    RPI_GPIO3,
    RPI_GPIO4,
    RPI_GPIO5,
    RPI_GPIO6,
    RPI_GPIO7,
    RPI_GPIO8,
    RPI_GPIO9,
    RPI_GPIO10 = 10,
    RPI_GPIO11,
    RPI_GPIO12,
    RPI_GPIO13,
    RPI_GPIO14,
    RPI_GPIO15,
    RPI_GPIO16,
    RPI_GPIO17,
    RPI_GPIO18,
    RPI_GPIO19,
    RPI_GPIO20 = 20,
    RPI_GPIO21,
    RPI_GPIO22,
    RPI_GPIO23,
    RPI_GPIO24,
    RPI_GPIO25,
    RPI_GPIO26,
    RPI_GPIO27,
    RPI_GPIO28,
    RPI_GPIO29,
    RPI_GPIO30 = 30,
    RPI_GPIO31,
    RPI_GPIO32,
    RPI_GPIO33,
    RPI_GPIO34,
    RPI_GPIO35,
    RPI_GPIO36,
    RPI_GPIO37,
    RPI_GPIO38,
    RPI_GPIO39,
    RPI_GPIO40 = 40,
    RPI_GPIO41,
    RPI_GPIO42,
    RPI_GPIO43,
    RPI_GPIO44,
    RPI_GPIO45,
    RPI_GPIO46,
    RPI_GPIO47,
    RPI_GPIO48,
    RPI_GPIO49,
    RPI_GPIO50 = 50,
    RPI_GPIO51,
    RPI_GPIO52,
    RPI_GPIO53,
    } rpi_gpio_pin_t;


/** The GPIO Peripheral is described in section 6 of the BCM2835 Peripherals
    documentation.

    There are 54 general-purpose I/O (GPIO) lines split into two banks. All
    GPIO pins have at least two alternative functions within BCM. The
    alternate functions are usually peripheral IO and a single peripheral
    may appear in each bank to allow flexibility on the choice of IO voltage.
    Details of alternative functions are given in section 6.2. Alternative
    Function Assignments.

    The GPIO peripheral has three dedicated interrupt lines. These lines are
    triggered by the setting of bits in the event detect status register. Each
    bank has its’ own interrupt line with the third line shared between all
    bits.

    The Alternate function table also has the pull state (pull-up/pull-down)
    which is applied after a power down. */
typedef struct {
    rpi_reg_rw_t    GPFSEL0;
    rpi_reg_rw_t    GPFSEL1;
    rpi_reg_rw_t    GPFSEL2;
    rpi_reg_rw_t    GPFSEL3;
    rpi_reg_rw_t    GPFSEL4;
    rpi_reg_rw_t    GPFSEL5;
    rpi_reg_ro_t    Reserved0;
    rpi_reg_wo_t    GPSET0;
    rpi_reg_wo_t    GPSET1;
    rpi_reg_ro_t    Reserved1;
    rpi_reg_wo_t    GPCLR0;
    rpi_reg_wo_t    GPCLR1;
    rpi_reg_ro_t    Reserved2;
    rpi_reg_wo_t    GPLEV0;
    rpi_reg_wo_t    GPLEV1;
    rpi_reg_ro_t    Reserved3;
    rpi_reg_wo_t    GPEDS0;
    rpi_reg_wo_t    GPEDS1;
    rpi_reg_ro_t    Reserved4;
    rpi_reg_wo_t    GPREN0;
    rpi_reg_wo_t    GPREN1;
    rpi_reg_ro_t    Reserved5;
    rpi_reg_wo_t    GPFEN0;
    rpi_reg_wo_t    GPFEN1;
    rpi_reg_ro_t    Reserved6;
    rpi_reg_wo_t    GPHEN0;
    rpi_reg_wo_t    GPHEN1;
    rpi_reg_ro_t    Reserved7;
    rpi_reg_wo_t    GPLEN0;
    rpi_reg_wo_t    GPLEN1;
    rpi_reg_ro_t    Reserved8;
    rpi_reg_wo_t    GPAREN0;
    rpi_reg_wo_t    GPAREN1;
    rpi_reg_ro_t    Reserved9;
    rpi_reg_wo_t    GPAFEN0;
    rpi_reg_wo_t    GPAFEN1;
    rpi_reg_ro_t    Reserved10;
    rpi_reg_wo_t    GPPUD;
    rpi_reg_wo_t    GPPUDCLK0;
    rpi_reg_wo_t    GPPUDCLK1;
    rpi_reg_ro_t    Reserved11;
    } rpi_gpio_t;


typedef enum {
    RPI_IO_LO = 0,
    RPI_IO_HI,
    RPI_IO_ON,
    RPI_IO_OFF,
    RPI_IO_UNKNOWN,
    } rpi_gpio_value_t;


extern rpi_gpio_t* RPI_GetGpio(void);
extern void RPI_SetGpioPinFunction( rpi_gpio_pin_t gpio, rpi_gpio_alt_function_t func );




#define BCM2835_MAILBOX_SUCCESS	(uint32_t)0x80000000	///< Request successful
#define BCM2835_MAILBOX_ERROR	(uint32_t)0x80000001	///< Error parsing request buffer (partial response)

/**
 * @brief The following lists the currently defined mailbox channels.
 */
typedef enum {
	BCM2835_MAILBOX_POWER_CHANNEL 	= 0,	///< For use by the power management interface
	BCM2835_MAILBOX_FB_CHANNEL 		= 1,	///< https://github.com/raspberrypi/firmware/wiki/Mailbox-framebuffer-interface
	BCM2835_MAILBOX_VCHIQ_CHANNEL 	= 3,    ///< For use by the VCHIQ interface
	BCM2835_MAILBOX_PROP_CHANNEL 	= 8		///< https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
} bcm2835MailboxChannels;

extern void lib_bcm2835_mailbox_flush(void);
extern uint32_t lib_bcm2835_mailbox_read(uint8_t);
extern void lib_bcm2835_mailbox_write(uint8_t, uint32_t);
extern uint32_t lib_bcm2835_mailbox_write_read(uint8_t channel, uint32_t data);






#define RPI_SYSTIMER_BASE       ( PERIPHERAL_BASE + 0x3000 )
typedef struct {
    volatile uint32_t control_status;
    volatile uint32_t counter_lo;
    volatile uint32_t counter_hi;
    volatile uint32_t compare0;
    volatile uint32_t compare1;
    volatile uint32_t compare2;
    volatile uint32_t compare3;
    } rpi_sys_timer_t;


/* Although these values were originally from the BCM2835 Arm peripherals PDF
   it's clear that was rushed and has some glaring errors - so these values
   may appear to be different. These values have been changed due to data on
   the elinux BCM2835 datasheet errata:
   http://elinux.org/BCM2835_datasheet_errata */

#define AUX_BASE    ( PERIPHERAL_BASE + 0x215000 )

#define AUX_ENA_MINIUART            ( 1 << 0 )
#define AUX_ENA_SPI1                ( 1 << 1 )
#define AUX_ENA_SPI2                ( 1 << 2 )

#define AUX_IRQ_SPI2                ( 1 << 2 )
#define AUX_IRQ_SPI1                ( 1 << 1 )
#define AUX_IRQ_MU                  ( 1 << 0 )

#define AUX_MULCR_8BIT_MODE         ( 3 << 0 )  /* See errata for this value */
#define AUX_MULCR_BREAK             ( 1 << 6 )
#define AUX_MULCR_DLAB_ACCESS       ( 1 << 7 )

#define AUX_MUMCR_RTS               ( 1 << 1 )

#define AUX_MULSR_DATA_READY        ( 1 << 0 )
#define AUX_MULSR_RX_OVERRUN        ( 1 << 1 )
#define AUX_MULSR_TX_EMPTY          ( 1 << 5 )
#define AUX_MULSR_TX_IDLE           ( 1 << 6 )

#define AUX_MUMSR_CTS               ( 1 << 5 )

#define AUX_MUCNTL_RX_ENABLE        ( 1 << 0 )
#define AUX_MUCNTL_TX_ENABLE        ( 1 << 1 )
#define AUX_MUCNTL_RTS_FLOW         ( 1 << 2 )
#define AUX_MUCNTL_CTS_FLOW         ( 1 << 3 )
#define AUX_MUCNTL_RTS_FIFO         ( 3 << 4 )
#define AUX_MUCNTL_RTS_ASSERT       ( 1 << 6 )
#define AUX_MUCNTL_CTS_ASSERT       ( 1 << 7 )

#define AUX_MUSTAT_SYMBOL_AV        ( 1 << 0 )
#define AUX_MUSTAT_SPACE_AV         ( 1 << 1 )
#define AUX_MUSTAT_RX_IDLE          ( 1 << 2 )
#define AUX_MUSTAT_TX_IDLE          ( 1 << 3 )
#define AUX_MUSTAT_RX_OVERRUN       ( 1 << 4 )
#define AUX_MUSTAT_TX_FIFO_FULL     ( 1 << 5 )
#define AUX_MUSTAT_RTS              ( 1 << 6 )
#define AUX_MUSTAT_CTS              ( 1 << 7 )
#define AUX_MUSTAT_TX_EMPTY         ( 1 << 8 )
#define AUX_MUSTAT_TX_DONE          ( 1 << 9 )
#define AUX_MUSTAT_RX_FIFO_LEVEL    ( 7 << 16 )
#define AUX_MUSTAT_TX_FIFO_LEVEL    ( 7 << 24 )


#define FSEL0(x)        ( x )
#define FSEL1(x)        ( x << 3 )
#define FSEL2(x)        ( x << 6 )
#define FSEL3(x)        ( x << 9 )
#define FSEL4(x)        ( x << 12 )
#define FSEL5(x)        ( x << 15 )
#define FSEL6(x)        ( x << 18 )
#define FSEL7(x)        ( x << 21 )
#define FSEL8(x)        ( x << 24 )
#define FSEL9(x)        ( x << 27 )

#define FSEL10(x)       ( x )
#define FSEL11(x)       ( x << 3 )
#define FSEL12(x)       ( x << 6 )
#define FSEL13(x)       ( x << 9 )
#define FSEL14(x)       ( x << 12 )
#define FSEL15(x)       ( x << 15 )
#define FSEL16(x)       ( x << 18 )
#define FSEL17(x)       ( x << 21 )
#define FSEL18(x)       ( x << 24 )
#define FSEL19(x)       ( x << 27 )

#define FSEL20(x)       ( x )
#define FSEL21(x)       ( x << 3 )
#define FSEL22(x)       ( x << 6 )
#define FSEL23(x)       ( x << 9 )
#define FSEL24(x)       ( x << 12 )
#define FSEL25(x)       ( x << 15 )
#define FSEL26(x)       ( x << 18 )
#define FSEL27(x)       ( x << 21 )
#define FSEL28(x)       ( x << 24 )
#define FSEL29(x)       ( x << 27 )

#define FSEL30(x)       ( x )
#define FSEL31(x)       ( x << 3 )
#define FSEL32(x)       ( x << 6 )
#define FSEL33(x)       ( x << 9 )
#define FSEL34(x)       ( x << 12 )
#define FSEL35(x)       ( x << 15 )
#define FSEL36(x)       ( x << 18 )
#define FSEL37(x)       ( x << 21 )
#define FSEL38(x)       ( x << 24 )
#define FSEL39(x)       ( x << 27 )

#define FSEL40(x)       ( x )
#define FSEL41(x)       ( x << 3 )
#define FSEL42(x)       ( x << 6 )
#define FSEL43(x)       ( x << 9 )
#define FSEL44(x)       ( x << 12 )
#define FSEL45(x)       ( x << 15 )
#define FSEL46(x)       ( x << 18 )
#define FSEL47(x)       ( x << 21 )
#define FSEL48(x)       ( x << 24 )
#define FSEL49(x)       ( x << 27 )

#define FSEL50(x)       ( x )
#define FSEL51(x)       ( x << 3 )
#define FSEL52(x)       ( x << 6 )
#define FSEL53(x)       ( x << 9 )


typedef struct {
    volatile unsigned int IRQ;
    volatile unsigned int ENABLES;

    volatile unsigned int reserved1[((0x40 - 0x04) / 4) - 1];

    volatile unsigned int MU_IO;
    volatile unsigned int MU_IER;
    volatile unsigned int MU_IIR;
    volatile unsigned int MU_LCR;
    volatile unsigned int MU_MCR;
    volatile unsigned int MU_LSR;
    volatile unsigned int MU_MSR;
    volatile unsigned int MU_SCRATCH;
    volatile unsigned int MU_CNTL;
    volatile unsigned int MU_STAT;
    volatile unsigned int MU_BAUD;

    volatile unsigned int reserved2[(0x80 - 0x68) / 4];

    volatile unsigned int SPI0_CNTL0;
    volatile unsigned int SPI0_CNTL1;
    volatile unsigned int SPI0_STAT;
    volatile unsigned int SPI0_IO;
    volatile unsigned int SPI0_PEEK;

    volatile unsigned int reserved3[(0xC0 - 0x94) / 4];

    volatile unsigned int SPI1_CNTL0;
    volatile unsigned int SPI1_CNTL1;
    volatile unsigned int SPI1_STAT;
    volatile unsigned int SPI1_IO;
    volatile unsigned int SPI1_PEEK;
    } aux_t;

extern aux_t* RPI_GetAux( void );
extern void RPI_AuxMiniUartInit( int baud, int bits , int mhz);
extern void RPI_AuxMiniUartWrite( char c );

extern char RPI_AuxMiniUartRead();
extern int RPI_AuxMiniUartReadPending();
extern void RPI_AuxMiniUartFlush();























//#define FUNC_PREFIX(x) 			lib_bcm2835_##x

#define BCM2835_PERI_BASE		PERIPHERAL_BASE

#define GPU_IO_BASE			0x7E000000	///<
#define GPU_CACHED_BASE			0x40000000	///<
#define GPU_UNCACHED_BASE		0xC0000000	///<

#if RASPPI != 1
#define GPU_MEM_BASE	GPU_UNCACHED_BASE
#else
#define GPU_MEM_BASE	GPU_CACHED_BASE
#endif


///< Speed of the core clock core_clk
#define BCM2835_CORE_CLK_HZ		400000000	///< 300 MHz
#define SYSFREQ BCM2835_CORE_CLK_HZ

#define HIGH 0x1				///< HIGH state
#define LOW  0x0				///< LOW state

#define RPI_V2_GPIO_P1_03	2	///< Version 2, Pin P1-03, SDA when I2C in use
#define RPI_V2_GPIO_P1_05	3	///< Version 2, Pin P1-05, SCL when I2C in use
#define RPI_V2_GPIO_P1_07	4	///< Version 2, Pin P1-07
#define RPI_V2_GPIO_P1_08	14  ///< Version 2, Pin P1-08, defaults to ALT function 0 PL011_TXD
#define RPI_V2_GPIO_P1_10	15  ///< Version 2, Pin P1-10, defaults to ALT function 0 PL011_RXD
#define RPI_V2_GPIO_P1_11	17  ///< Version 2, Pin P1-11, CE1 when SPI1 in use
#define RPI_V2_GPIO_P1_12	18  ///< Version 2, Pin P1-12, CE0 when SPI1 in use
#define RPI_V2_GPIO_P1_13	27  ///< Version 2, Pin P1-13
#define RPI_V2_GPIO_P1_15	22  ///< Version 2, Pin P1-15
#define RPI_V2_GPIO_P1_16	23  ///< Version 2, Pin P1-16
#define RPI_V2_GPIO_P1_18	24  ///< Version 2, Pin P1-18
#define RPI_V2_GPIO_P1_19	10  ///< Version 2, Pin P1-19, MOSI when SPI0 in use
#define RPI_V2_GPIO_P1_21	9	///< Version 2, Pin P1-21, MISO when SPI0 in use
#define RPI_V2_GPIO_P1_22	25	///< Version 2, Pin P1-22
#define RPI_V2_GPIO_P1_23	11  ///< Version 2, Pin P1-23, CLK when SPI0 in use
#define RPI_V2_GPIO_P1_24	8	///< Version 2, Pin P1-24, CE0 when SPI0 in use
#define RPI_V2_GPIO_P1_26	7	///< Version 2, Pin P1-26, CE1 when SPI0 in use
#define RPI_V2_GPIO_P1_29	5	///< Version 2, Pin P1-29,
#define RPI_V2_GPIO_P1_31	6	///< Version 2, Pin P1-31,
#define RPI_V2_GPIO_P1_32	12	///< Version 2, Pin P1-32,
#define RPI_V2_GPIO_P1_33	13	///< Version 2, Pin P1-33,
#define RPI_V2_GPIO_P1_35	19	///< Version 2, Pin P1-35, MISO when SPI1 in use
#define RPI_V2_GPIO_P1_36	16	///< Version 2, Pin P1-36, CE2 when SPI1 in use
#define RPI_V2_GPIO_P1_37	26	///< Version 2, Pin P1-37,
#define RPI_V2_GPIO_P1_38	20	///< Version 2, Pin P1-38, MOSI when SPI1 in use
#define RPI_V2_GPIO_P1_40	21	///< Version 2, Pin P1-40, CLK when SPI1 in use

#define BCM2835_ST_BASE			(BCM2835_PERI_BASE + 0x003000)	///<
#define	BCM2835_DMA0_BASE		(BCM2835_PERI_BASE + 0x007000)	///<
#define	BCM2835_DMA1_BASE		(BCM2835_PERI_BASE + 0x007100)	///<
#define	BCM2835_DMA2_BASE		(BCM2835_PERI_BASE + 0x007200)	///<
#define	BCM2835_DMA3_BASE		(BCM2835_PERI_BASE + 0x007300)	///<
#define	BCM2835_DMA4_BASE		(BCM2835_PERI_BASE + 0x007400)	///<
#define	BCM2835_DMA5_BASE		(BCM2835_PERI_BASE + 0x007500)	///<
#define	BCM2835_DMA6_BASE		(BCM2835_PERI_BASE + 0x007600)	///<
#define BCM2835_IRQ_BASE		(BCM2835_PERI_BASE + 0x00B200)	///<
#define BCM2835_MAILBOX_BASE	(BCM2835_PERI_BASE + 0x00B880)	///<
#define BCM2835_PM_WDOG_BASE	(BCM2835_PERI_BASE + 0x100000)	///<
#define BCM2835_HW_RNG_BASE		(BCM2835_PERI_BASE + 0x104000)	///<
#define BCM2835_GPIO_BASE		(BCM2835_PERI_BASE + 0x200000)	///<
#define BCM2835_SPI0_BASE		(BCM2835_PERI_BASE + 0x204000)	///< Base Physical Address of the SPI0 registers
#define BCM2835_BSC0_BASE 		(BCM2835_PERI_BASE + 0x205000)	///< Base Physical Address of the BSC0 registers
#define BCM2835_PL011_BASE		(BCM2835_PERI_BASE + 0x201000)	///< Base Physical Address of the PL011 registers
#define BCM2835_AUX_BASE		(BCM2835_PERI_BASE + 0x215000)	///< Base Physical Address of the AUX registers
#define BCM2835_UART1_BASE		(BCM2835_PERI_BASE + 0x215000)	///< Base Physical Address of the AUX_UART1 registers
#define BCM2835_SPI1_BASE		(BCM2835_PERI_BASE + 0x215080)	///< Base Physical Address of the AUX_SPI1 registers
#define BCM2835_SPI2_BASE		(BCM2835_PERI_BASE + 0x2150C0)	///< Base Physical Address of the AUX_SPI2 registers
#define BCM2835_EMMC_BASE		(BCM2835_PERI_BASE + 0x300000)	///< Base Physical Address of the EMMC registers
#define BCM2835_BSC1_BASE		(BCM2835_PERI_BASE + 0x804000)	///< Base Physical Address of the BSC1 registers
#define BCM2835_BSC2_BASE		(BCM2835_PERI_BASE + 0x805000)	///< Base Physical Address of the BSC2 registers
#define	BCM2835_USB_BASE		(BCM2835_PERI_BASE + 0x980000)	///<
#define	BCM2835_DMA15_BASE		(BCM2835_PERI_BASE + 0xE05000)	///<

// https://github.com/raspberrypi/linux/blob/rpi-3.6.y/arch/arm/mach-bcm2708/include/mach/platform.h
#define ARM_IRQ1_BASE		0						///<
#define INTERRUPT_TIMER1	(ARM_IRQ1_BASE + 1)		///<
#define INTERRUPT_TIMER3	(ARM_IRQ1_BASE + 3)		///<
#define INTERRUPT_AUX		(ARM_IRQ1_BASE + 29)	///<

#define ARM_IRQ2_BASE		32						///<
#define INTERRUPT_GPIO0		(ARM_IRQ2_BASE + 17)
#define INTERRUPT_GPIO1		(ARM_IRQ2_BASE + 18)
#define INTERRUPT_GPIO2		(ARM_IRQ2_BASE + 19)
#define INTERRUPT_GPIO3		(ARM_IRQ2_BASE + 20)
#define INTERRUPT_VC_UART	(ARM_IRQ2_BASE + 25)	///< UART Interrupt

typedef enum {
	// ARM_IRQ1_BASE
	BCM2835_TIMER1_IRQn = 1 << (INTERRUPT_TIMER1 - ARM_IRQ1_BASE),		///<
	BCM2835_TIMER3_IRQn = 1	<< (INTERRUPT_TIMER3 - ARM_IRQ1_BASE),		///<
	BCM2835_UART1_IRQn  = 1 << (INTERRUPT_AUX - ARM_IRQ1_BASE),			///<
	// ARM_IRQ2_BASE
	BCM2835_GPIO0_IRQn    = 1 << (INTERRUPT_GPIO0 - ARM_IRQ2_BASE),		///<
	BCM2835_VC_UART_IRQn  = 1 << (INTERRUPT_VC_UART - ARM_IRQ2_BASE)	///<
} BCM2835_IRQn_TypeDef;



#define		__I		volatile const	///< defines 'read only' permissions
#define		__O		volatile		///< defines 'write only' permissions
#define		__IO	volatile		///< defines 'read / write' permissions

typedef struct {
	__IO uint32_t CS;			///< 0x00	System Timer Control/Status 
	__IO uint32_t CLO;			///< 0x04	System Timer Counter Lower 32 bits
	__IO uint32_t CHI;			///< 0x08	System Timer Counter Higher 32 bits
	__I uint32_t C0;			///< 0x0C	System Timer Compare 0.  DO NOT USE; is used by GPU.
	__IO uint32_t C1;			///< 0x10	System Timer Compare 1
	__I uint32_t C2;			///< 0x14	System Timer Compare 2.  DO NOT USE; is used by GPU.
	__IO uint32_t C3;			///< 0x18	System Timer Compare 3
} BCM2835_ST_TypeDef;

typedef struct {
	__IO uint32_t CS;			///< 0x00, Control and Status
	__IO uint32_t CONBLK_AD;	///< 0x04, Control Block Address
	__IO uint32_t TI;			///< 0x08, Transfer Information
	__IO uint32_t SOURCE_AD;	///< 0x0C, Source Address
	__IO uint32_t DEST_AD;		///< 0x10, Destination Address
	__IO uint32_t TXFR_LEN;		///< 0x14, Transfer Length
	__IO uint32_t STRIDE;		///< 0x18, 2D Stride
	__IO uint32_t NEXTCONBK;	///< 0x1C, Next CB Address
	__IO uint32_t DBG;			///< 0x20, Debug
} BCM2835_DMA_TypeDef;

/// 2.1.1 AUX registers
typedef struct {
	__I uint32_t IRQ;			///< 0x00
	__IO uint32_t ENABLE;		///< 0x04
} BCM2835_AUX_TypeDef;

/// 2.2.2 Mini UART register details.
typedef struct {
	__I uint32_t IRQ;			///< 0x00
	__IO uint32_t ENABLE;		///< 0x04
	__IO uint32_t PAD[14];		///< 0x08
	__IO uint32_t IO;			///< 0x40
	__IO uint32_t IER;			///< 0x44
	__IO uint32_t IIR;			///< 0x48
	__IO uint32_t LCR;			///< 0x4C
	__IO uint32_t MCR;			///< 0x50
	__IO uint32_t LSR;			///< 0x54
	__IO uint32_t MSR;			///< 0x58
	__IO uint32_t SCR;			///< 0x5C
	__IO uint32_t CNTL;			///< 0x60
	__I uint32_t STAT;			///< 0x64
	__IO uint32_t BAUD;			///< 0x68
} BCM2835_UART_TypeDef;

/// 2.3.4 SPI register details.
typedef struct {
	__IO uint32_t CNTL0;		///< 0x00
	__IO uint32_t CNTL1;		///< 0x04
	__IO uint32_t STAT;			///< 0x08
	__I uint32_t PEEK;			///< 0x0C, Read but do not take from FF
	__IO uint32_t PAD1[4];		///< 0x10, Padding
	__IO uint32_t IO;			///< 0x20, Write = TX, read=RX
	__IO uint32_t PAD2[3];		///< 0x24, Padding
	__IO uint32_t TXHOLD;		///< 0x30, Write = TX keep CS, read=RX
} BCM2835_AUX_SPI_TypeDef;

/// 13.4 Register View
typedef struct {
	__IO uint32_t DR;			///< 0x00, Data Register
	__IO uint32_t RSRECR;		///< 0x04, Receive status register/error clear register
	__IO uint32_t PAD[4];		///< 0x08, Padding
	__IO uint32_t FR;			///< 0x18, Flag register
	__IO uint32_t RES1;			///< 0x1C, Reserved
	__IO uint32_t ILPR;			///< 0x20, not in use
	__IO uint32_t IBRD;			///< 0x24
	__IO uint32_t FBRD;			///< 0x28
	__IO uint32_t LCRH;			///< 0x2C
	__IO uint32_t CR;			///< 0x30
	__IO uint32_t IFLS;			///< 0x34
	__IO uint32_t IMSC;			///< 0x38
	__IO uint32_t RIS;			///< 0x3C
	__I uint32_t MIS;			///< 0x40
	__IO uint32_t ICR;			///< 0x44
	__IO uint32_t DMACR;		///< 0x48
} BCM2835_PL011_TypeDef;

/// Defines for GPIO\n
/// The BCM2835 has 54 GPIO pins.\n
/// BCM2835 data sheet, Page 90 onwards.\n
typedef struct {
	__IO uint32_t GPFSEL0;		///< 0x00, GPIO Function Select 0
	__IO uint32_t GPFSEL1;		///< 0x04, GPIO Function Select 1
	__IO uint32_t GPFSEL2;		///< 0x08, GPIO Function Select 2
	__IO uint32_t GPFSEL3;		///< 0x0C, GPIO Function Select 3
	__IO uint32_t GPFSEL4;		///< 0x10, GPIO Function Select 4
	__IO uint32_t GPFSEL5;		///< 0x14, GPIO Function Select 5
	__IO uint32_t RES1;			///< 0x18, Reserved
	__O uint32_t GPSET0;		///< 0x1C, GPIO Pin Output Set 0
	__O uint32_t GPSET1;		///< 0x20, GPIO Pin Output Set 1
	__IO uint32_t RES2;			///< 0x24, Reserved
	__O uint32_t GPCLR0;		///< 0x28, GPIO Pin Output Clear 0
	__O uint32_t GPCLR1;		///< 0x2C, GPIO Pin Output Clear 1
	__IO uint32_t RES3;			///< 0x30, Reserved
	__I uint32_t GPLEV0;		///< 0x34, GPIO Pin Level 0
	__I uint32_t GPLEV1;		///< 0x38, GPIO Pin Level 1
	__IO uint32_t RES4;			///< 0x3C, Reserved
	__IO uint32_t GPEDS0;		///< 0x40, GPIO Pin Event Detect Status 0
	__IO uint32_t GPEDS1;		///< 0x44, GPIO Pin Event Detect Status 1
	__IO uint32_t RES5;			///< 0x48, Reserved
	__IO uint32_t GPREN0;		///< 0x4C, GPIO Pin Rising Edge Detect Enable 0
	__IO uint32_t GPREN1;		///< 0x50, GPIO Pin Rising Edge Detect Enable 1
	__IO uint32_t RES6;			///< 0x54, Reserved
	__IO uint32_t GPFEN0;		///< 0x58, GPIO Pin Falling Edge Detect Enable 0
	__IO uint32_t GPFEN1;		///< 0x5C, GPIO Pin Falling Edge Detect Enable 1
	__IO uint32_t RES7;			///< 0x60, Reserved
	__IO uint32_t GPHEN0;		///< 0x64, GPIO Pin High Detect Enable 0
	__IO uint32_t GPHEN1;		///< 0x68, GPIO Pin High Detect Enable 1
	__IO uint32_t RES8;			///< 0x6C, Reserved
	__IO uint32_t GPLEN0;		///< 0x70, GPIO Pin Low Detect Enable 0
	__IO uint32_t GPLEN1;		///< 0x74, GPIO Pin Low Detect Enable 1
	__IO uint32_t RES9;			///< 0x78, Reserved
	__IO uint32_t AREN[2];		///< 0x7C
	__IO uint32_t RES10;		///< 0x84, Reserved
	__IO uint32_t AFEN[2];		///< 0x88
	__IO uint32_t RES11;		///< 0x90
	__IO uint32_t GPPUD; 		///< 0x94, GPIO Pin Pull-up/down Enable
	__IO uint32_t GPPUDCLK0;	///< 0x98, GPIO Pin Pull-up/down Enable Clock 0
	__IO uint32_t GPPUDCLK1;	///< 0x9C, GPIO Pin Pull-up/down Enable Clock 1
} BCM2835_GPIO_TypeDef;


/// Defines for SPI\n
/// SPI register offsets from \ref BCM2835_SPI0_BASE.\n
/// per 10.5 SPI Register Map
typedef struct {
	__IO uint32_t CS;			///< 0x00, SPI Master Control and Status
	__IO uint32_t FIFO;			///< 0x04, SPI Master TX and RX FIFOs
	__IO uint32_t CLK;			///< 0x08, SPI Master Clock Divider
	__IO uint32_t DLEN;			///< 0x0C, SPI Master Data Length
	__IO uint32_t LTOH;			///< 0x10, SPI LOSSI mode TOH
	__IO uint32_t DC;			///< 0x14, SPI DMA DREQ Controls
} BCM2835_SPI_TypeDef;

/// Defines for I2C\n
/// BSC register offsets from BCM2835_BSC*_BASE.\n
/// per 3.1 BSC Register Map
typedef struct {
	__IO uint32_t C;		///< 0x00, BSC Master Control
	__IO uint32_t S;		///< 0x04, BSC Master Status
	__IO uint32_t DLEN;		///< 0x08, BSC Master Data Length
	__IO uint32_t A;		///< 0x0C, BSC Master Slave Address
	__IO uint32_t FIFO;		///< 0x10, BSC Master Data FIFO
	__IO uint32_t DIV;		///< 0x14, BSC Master Clock Divider
	__IO uint32_t DEL;		///< 0x18, BSC Master Data Delay
	__IO uint32_t CLKT;		///< 0x1C, BSC Master Clock Stretch Timeout
} BCM2835_BSC_TypeDef;

/// Defines for IRQ\n
typedef struct {
	__I uint32_t IRQ_BASIC_PENDING;		///< 0x00
	__I uint32_t IRQ_PENDING1;			///< 0x04
	__I uint32_t IRQ_PENDING2;			///< 0x08
	__IO uint32_t FIQ_CONTROL;			///< 0x0C
	__IO uint32_t IRQ_ENABLE1;			///< 0x10
	__IO uint32_t IRQ_ENABLE2;			///< 0x14
	__IO uint32_t IRQ_BASIC_ENABLE;		///< 0x18
	__IO uint32_t IRQ_DISABLE1;			///< 0x1C
	__IO uint32_t IRQ_DISABLE2;			///< 0x20
	__IO uint32_t IRQ_BASIC_DISABLE;	///< 0x24
} BCM2835_IRQ_TypeDef;

/// Defines for MAILBOX\n
typedef struct {
	__I uint32_t READ;		///< 0x00
	__I uint32_t RES1;		///< 0x04
	__I uint32_t RES2;		///< 0x08
	__I uint32_t RES3;		///< 0x0C
	__I uint32_t PEEK;		///< 0x10
	__I uint32_t SENDER;	///< 0x14
	__IO uint32_t STATUS;	///< 0x18
	__I uint32_t CONFIG;	///< 0x1C
	__O uint32_t WRITE;		///< 0x20
} BCM2835_MAILBOX_TypeDef;

/// Defines for WATCHDOG\n
typedef struct {
	__I uint32_t UNKWOWN0[7];	///< 0x00
	__IO uint32_t RSTC;			///< 0x1C
	__I uint32_t UNKWOWN1;		///< 0x20
	__IO uint32_t WDOG;			///< 0x24
} BCM2835_PM_WDOG_TypeDef;

/// Defines for Hardware Random Generator\n
typedef struct {
	__IO uint32_t CTRL;		///< 0x00
	__IO uint32_t STATUS;	///< 0x04
	__I uint32_t DATA;		///< 0x08
} BCM2835_HW_RNG_TypeDef;


/// Defines for EMMC\n
typedef struct {
	__IO uint32_t ARG2;				///< 0x00
	__IO uint32_t BLKSIZECNT;		///< 0x04
	__IO uint32_t ARG1;				///< 0x08
	__IO uint32_t CMDTM;			///< 0x0C
	__O uint32_t RESP0;				///< 0x10
	__O uint32_t RESP1;				///< 0x14
	__O uint32_t RESP2;				///< 0x18
	__O uint32_t RESP3;				///< 0x1C
	__IO uint32_t DATA;				///< 0x20
	__O uint32_t STATUS;			///< 0x24
	__IO uint32_t CONTROL0;			///< 0x28
	__IO uint32_t CONTROL1;			///< 0x2C
	__IO uint32_t INTERRUPT;		///< 0x30
	__IO uint32_t IRPT_MASK;		///< 0x34
	__IO uint32_t IRPT_EN;			///< 0x38
	__IO uint32_t CONTROL2;			///< 0x3C
	__IO uint32_t CAPABILITIES_0;	///< 0x40
	__IO uint32_t CAPABILITIES_1;	///< 0x44
	__IO uint32_t NOTINUSE1[2];
	__IO uint32_t FORCE_IRPT;		///< 0x50
	__IO uint32_t NOTINUSE2[7];
	__IO uint32_t BOOT_TIMEOUT;		///< 0x70
	__IO uint32_t DBG_SEL;			///< 0x74
	__IO uint32_t NOTINUSE3[2];
	__IO uint32_t EXRDFIFO_CFG;		///< 0x80
	__IO uint32_t EXRDFIFO_EN;		///< 0x84
	__IO uint32_t TUNE_STEP;		///< 0x88
	__IO uint32_t TUNE_STEPS_STD;	///< 0x8C
	__IO uint32_t TUNE_STEPS_DDR;	///< 0x90
	__IO uint32_t NOTINUSE4[23];
	__IO uint32_t SPI_INT_SPT;		///< 0xF0
	__IO uint32_t NOTINUSE5[2];
	__IO uint32_t SLOTISR_VER;		///< 0xFC
} BCM2835_EMMC_TypeDef;


#define BCM2835_FIQ_ENABLE	(1 << 7)///< 0x80


#define BCM2835_ST		((BCM2835_ST_TypeDef *)   BCM2835_ST_BASE)			///< Base register address for SYSTEM TIMER
#define BCM2835_DMA0	((BCM2835_DMA_TypeDef) *) BCM2835_DMA0_BASE)		///< Base register address for DMA Channel 0
#define BCM2835_DMA1	((BCM2835_DMA_TypeDef) *) BCM2835_DMA1_BASE)		///< Base register address for DMA Channel 1
#define BCM2835_DMA2	((BCM2835_DMA_TypeDef) *) BCM2835_DMA2_BASE)		///< Base register address for DMA Channel 2
#define BCM2835_DMA3	((BCM2835_DMA_TypeDef) *) BCM2835_DMA3_BASE)		///< Base register address for DMA Channel 3
#define BCM2835_DMA4	((BCM2835_DMA_TypeDef) *) BCM2835_DMA4_BASE)		///< Base register address for DMA Channel 4
#define BCM2835_DMA5	((BCM2835_DMA_TypeDef) *) BCM2835_DMA5_BASE)		///< Base register address for DMA Channel 5
#define BCM2835_DMA6	((BCM2835_DMA_TypeDef) *) BCM2835_DMA6_BASE)		///< Base register address for DMA Channel 6
#define BCM2835_IRQ		((BCM2835_IRQ_TypeDef *)  BCM2835_IRQ_BASE)			///< Base register address for IRQ
#define BCM2835_MAILBOX	((BCM2835_MAILBOX_TypeDef *) BCM2835_MAILBOX_BASE)	///< Base register address for MAILBOX
#define BCM2835_PM_WDOG	((BCM2835_PM_WDOG_TypeDef *) BCM2835_PM_WDOG_BASE)	///< Base register address for WATCHDOG
#define BCM2835_HW_RNG	((BCM2835_HW_RNG_TypeDef *) BCM2835_HW_RNG_BASE)	///< Base register address for HW RNG
#define BCM2835_GPIO	((BCM2835_GPIO_TypeDef *) BCM2835_GPIO_BASE)		///< Base register address for GPIO
#define BCM2835_SPI0	((BCM2835_SPI_TypeDef *)  BCM2835_SPI0_BASE)		///< Base register address for SPI
#define BCM2835_PL011	((BCM2835_PL011_TypeDef *) BCM2835_PL011_BASE)		///< Base register address for PL011
#define BCM2835_AUX		((BCM2835_AUX_TypeDef *) BCM2835_AUX_BASE)			///< Base register address for AUX
#define BCM2835_UART1	((BCM2835_UART_TypeDef *) BCM2835_UART1_BASE)		///< Base register address for AUX_UART1
#define BCM2835_SPI1	((BCM2835_AUX_SPI_TypeDef *) BCM2835_SPI1_BASE)		///< Base register address for AUX_SPI1
#define BCM2835_SPI2	((BCM2835_AUX_SPI_TypeDef *) BCM2835_SPI2_BASE)		///< Base register address for AUX_SPI2
#define BCM2835_EMMC	((BCM2835_EMMC_TypeDef *) BCM2835_EMMC_BASE)		///< Base register address for EMMC
#define BCM2835_BSC0	((BCM2835_BSC_TypeDef *)  BCM2835_BSC0_BASE)		///< Base register address for I2C (BSC0)
#define BCM2835_BSC1	((BCM2835_BSC_TypeDef *)  BCM2835_BSC1_BASE)		///< Base register address for I2C (BSC1)


// pitrexio-gpio.c
extern void bcm2835_delayMicroseconds(uint64_t micros);
#define lib_bcm2835_delay(millis) bcm2835_delayMicroseconds((uint64_t)((uint64_t)1000 * (uint64_t)millis))
#define usDelay bcm2835_delayMicroseconds
#define MsDelay(millis) bcm2835_delayMicroseconds((uint64_t)((uint64_t)1000 * (uint64_t)millis))




/// Unique clock ID
typedef enum {
	BCM2835_VC_CLOCK_ID_RESERVED = 0,	///<
	BCM2835_VC_CLOCK_ID_EMMC = 1,		///<
	BCM2835_VC_CLOCK_ID_UART = 2,		///<
	BCM2835_VC_CLOCK_ID_ARM = 3,		///<
	BCM2835_VC_CLOCK_ID_CORE = 4,		///<
	BCM2835_VC_CLOCK_ID_V3D = 5,		///<
	BCM2835_VC_CLOCK_ID_H264 = 6,		///<
	BCM2835_VC_CLOCK_ID_ISP = 7,		///<
	BCM2835_VC_CLOCK_ID_SDRAM = 8,		///<
	BCM2835_VC_CLOCK_ID_PIXEL = 9,		///<
	BCM2835_VC_CLOCK_ID_PWM = 10		///<
} bcm2835VideoCoreClockId;

/// Unique power ID
typedef enum {
	BCM2835_VC_POWER_ID_SDCARD = 0,	///<
	BCM2835_VC_POWER_ID_UART0 = 1,	///<
	BCM2835_VC_POWER_ID_UART1 = 2,	///<
	BCM2835_VC_POWER_ID_USBHCD = 3,	///<
	BCM2835_VC_POWER_ID_I2C0 = 4,	///<
	BCM2835_VC_POWER_ID_I2C1 = 5,	///<
	BCM2835_VC_POWER_ID_I2C2 = 6,	///<
	BCM2835_VC_POWER_ID_SPI = 7,	///<
	BCM2835_VC_POWER_ID_CCP2TX = 8 	///<
} bcm2835VideoCorePowerId;

/**
 *  Bit 0: 0=off, 1=on
 *  Bit 1: 0=do not wait, 1=wait
 *  Bits 2-31: reserved for future use (set to 0)
 */
typedef enum {
	BCM2835_VC_SET_POWER_STATE_OFF_NO_WAIT = 0,	///<
	BCM2835_VC_SET_POWER_STATE_OFF_WAIT = 2,	///<
	BCM2835_VC_SET_POWER_STATE_ON_NO_WAIT = 1,	///<
	BCM2835_VC_SET_POWER_STATE_ON_WAIT = 3		///<
} bcm2835SetPowerState;

#define BCM2835_VC_SET_POWER_STATE_REQ_OFF		(0 << 0)	///< Bit 0: 0=off
#define BCM2835_VC_SET_POWER_STATE_REQ_ON		(1 << 0)	///< Bit 0: 1=on
#define BCM2835_VC_SET_POWER_STATE_REQ_WAIT		(1 << 1)	///< Bit 1: 1=wait

#define BCM2835_VC_POWER_STATE_RESP_ON			(1 << 0)	///< Bit 0: 0=off, 1=on
#define BCM2835_VC_POWER_STATE_RESP_NODEV		(1 << 1)	///< Bit 1: 0=device exists, 1=device does not exist

// Tag VideoCore
#define BCM2835_VC_TAG_GET_FIRMWARE_REV			0x00000001	///<
// Tag Hardware
#define BCM2835_VC_TAG_GET_BOARD_MODEL			0x00010001	///<
#define BCM2835_VC_TAG_GET_BOARD_REV			0x00010002	///<
#define BCM2835_VC_TAG_GET_BOARD_MAC_ADDRESS	0x00010003	///<
#define BCM2835_VC_TAG_GET_BOARD_SERIAL			0x00010004	///<
#define BCM2835_VC_TAG_GET_ARM_MEMORY			0x00010005	///<
#define BCM2835_VC_TAG_GET_VC_MEMORY			0x00010006	///<
#define BCM2835_VC_TAG_GET_CLOCKS				0x00010007	///<
// Tag Clock
#define BCM2835_VC_TAG_GET_CLOCK_STATE			0x00030001	///<
#define BCM2835_VC_TAG_GET_CLOCK_RATE 			0x00030002	///<
#define BCM2835_VC_TAG_GET_MAX_CLOCK_RATE 		0x00030004	///<
#define BCM2835_VC_TAG_GET_MIN_CLOCK_RATE 		0x00030007	///<
#define BCM2835_VC_TAG_GET_TURBO		 		0x00030009	///<
#define BCM2835_VC_TAG_SET_CLOCK_STATE			0x00038001	///<
#define BCM2835_VC_TAG_SET_CLOCK_RATE 			0x00038002	///<
#define BCM2835_VC_TAG_SET_TURBO	 			0x00038009	///<
// Tag Power
#define BCM2835_VC_TAG_GET_POWER_STATE			0x00020001	///<
#define BCM2835_VC_TAG_SET_POWER_STATE			0x00028001	///<
// Tag Temperatures
#define BCM2835_VC_TAG_GET_TEMP					0x00030006	///<
#define BCM2835_VC_TAG_GET_MAX_TEMP				0x0003000A	///<
// Tag Framebuffer
#define BCM2835_VC_TAG_ALLOCATE_BUFFER			0x00040001	///<
#define BCM2835_VC_TAG_RELEASE_BUFFER			0x00048001	///<
#define BCM2835_VC_TAG_BLANK_SCREEN				0x00040002	///<
#define BCM2835_VC_TAG_GET_PHYS_WH				0x00040003	///<
#define BCM2835_VC_TAG_TEST_PHYS_WH				0x00044003	///<
#define BCM2835_VC_TAG_SET_PHYS_WH				0x00048003	///<
#define BCM2835_VC_TAG_GET_VIRT_WH				0x00040004	///<
#define BCM2835_VC_TAG_TEST_VIRT_WH				0x00044004	///<
#define BCM2835_VC_TAG_SET_VIRT_WH				0x00048004	///<
#define BCM2835_VC_TAG_GET_DEPTH				0x00040005	///<
#define BCM2835_VC_TAG_TEST_DEPTH				0x00044005	///<
#define BCM2835_VC_TAG_SET_DEPTH				0x00048005	///<
#define BCM2835_VC_TAG_GET_PIXEL_ORDER			0x00040006	///<
#define BCM2835_VC_TAG_TEST_PIXEL_ORDER			0x00044006	///<
#define BCM2835_VC_TAG_SET_PIXEL_ORDER			0x00048006	///<
#define BCM2835_VC_TAG_GET_ALPHA_MODE			0x00040007	///<
#define BCM2835_VC_TAG_TEST_ALPHA_MODE			0x00044007	///<
#define BCM2835_VC_TAG_SET_ALPHA_MODE			0x00048007	///<
#define BCM2835_VC_TAG_GET_PITCH				0x00040008	///<
#define BCM2835_VC_TAG_GET_VIRT_OFFSET			0x00040009	///<
#define BCM2835_VC_TAG_TEST_VIRT_OFFSET			0x00044009	///<
#define BCM2835_VC_TAG_SET_VIRT_OFFSET			0x00048009	///<
#define BCM2835_VC_TAG_GET_OVERSCAN				0x0004000a	///<
#define BCM2835_VC_TAG_TEST_OVERSCAN			0x0004400a	///<
#define BCM2835_VC_TAG_SET_OVERSCAN				0x0004800a	///<
#define BCM2835_VC_TAG_GET_PALETTE				0x0004000b	///<
#define BCM2835_VC_TAG_TEST_PALETTE				0x0004400b	///<
#define BCM2835_VC_TAG_SET_PALETTE				0x0004800b	///<


extern int32_t lib_bcm2835_vc_get_clock_rate(uint32_t);
extern int32_t lib_bcm2835_vc_set_clock_rate(uint32_t, uint32_t);
extern int32_t lib_bcm2835_vc_get_memory(uint32_t tag_id);
extern int32_t lib_bcm2835_vc_get_power_state(uint32_t dev_id);
extern int32_t lib_bcm2835_vc_set_power_state(uint32_t dev_id, uint32_t state);


#define	__enable_irq()	asm volatile ("cpsie i")
#define	__disable_irq()	asm volatile ("cpsid i")
#define	__enable_fiq()	asm volatile ("cpsie f")
#define	__disable_fiq()	asm volatile ("cpsid f")






#define BCM2835_BSC_C_I2CEN 		0x00008000 ///< I2C Enable, 0 = disabled, 1 = enabled
#define BCM2835_BSC_C_INTR 			0x00000400 ///< Interrupt on RX
#define BCM2835_BSC_C_INTT 			0x00000200 ///< Interrupt on TX
#define BCM2835_BSC_C_INTD 			0x00000100 ///< Interrupt on DONE
#define BCM2835_BSC_C_ST 			0x00000080 ///< Start transfer, 1 = Start a new transfer
#define BCM2835_BSC_C_CLEAR_1 		0x00000020 ///< Clear FIFO Clear
#define BCM2835_BSC_C_CLEAR_2 		0x00000010 ///< Clear FIFO Clear
#define BCM2835_BSC_C_READ 			0x00000001 ///<	Read transfer

#define BCM2835_BSC_S_CLKT 			0x00000200 ///< Clock stretch timeout
#define BCM2835_BSC_S_ERR 			0x00000100 ///< ACK error
#define BCM2835_BSC_S_RXF 			0x00000080 ///< RXF FIFO full, 0 = FIFO is not full, 1 = FIFO is full
#define BCM2835_BSC_S_TXE 			0x00000040 ///< TXE FIFO full, 0 = FIFO is not full, 1 = FIFO is full
#define BCM2835_BSC_S_RXD 			0x00000020 ///< RXD FIFO contains data
#define BCM2835_BSC_S_TXD 			0x00000010 ///< TXD FIFO can accept data
#define BCM2835_BSC_S_RXR 			0x00000008 ///< RXR FIFO needs reading (full)
#define BCM2835_BSC_S_TXW 			0x00000004 ///< TXW FIFO needs writing (full)
#define BCM2835_BSC_S_DONE 			0x00000002 ///< Transfer DONE
#define BCM2835_BSC_S_TA 			0x00000001 ///< Transfer Active

#define BCM2835_BSC_FIFO_SIZE   			16 ///< BSC FIFO size

/// Specifies the divider used to generate the I2C clock from the system clock.\n
/// Clock divided is based on nominal base clock rate of 250MHz
typedef enum {
	BCM2835_I2C_CLOCK_DIVIDER_2500	= 2500,		///< 2500 = 10us = 100 kHz
	BCM2835_I2C_CLOCK_DIVIDER_626	= 626,		///< 626 = 2.504us = 399.3610 kHz
	BCM2835_I2C_CLOCK_DIVIDER_150	= 150,		///< 150 = 60ns = 1.666 MHz (default at reset)
	BCM2835_I2C_CLOCK_DIVIDER_148	= 148,		///< 148 = 59ns = 1.689 MHz
} bcm2835I2CClockDivider;

/// Specifies the reason codes for the \ref lib_bcm2835_i2c_write and \ref lib_bcm2835_i2c_read functions.
typedef enum {
	BCM2835_I2C_REASON_OK			= 0x00,		///< Success
	BCM2835_I2C_REASON_ERROR_NACK 	= 0x01,		///< Received a NACK
	BCM2835_I2C_REASON_ERROR_CLKT 	= 0x02,		///< Received Clock Stretch Timeout
	BCM2835_I2C_REASON_ERROR_DATA 	= 0x04		///< Not all data is sent / received
} bcm2835I2CReasonCodes;

extern void lib_bcm2835_i2c_begin(void);
extern void lib_bcm2835_i2c_end(void);
extern uint8_t lib_bcm2835_i2c_write(/*@null@*/const char *, uint32_t);
extern uint8_t lib_bcm2835_i2c_read(/*@out@*/char *, uint32_t);
extern void lib_bcm2835_i2c_set_baudrate(uint32_t);

/**
 * @ingroup I2C
 *
 * Sets the I2C slave address
 * @param addr buffer for read.
 */
/*@unused@*/inline static void lib_bcm2835_i2c_setSlaveAddress(uint8_t addr) {
	BCM2835_BSC1->A = addr;
}

/**
 * @ingroup I2C
 *
 * Sets the I2C clock divider and therefore the I2C clock speed.
 * @param divider The desired I2C clock divider, one of \ref bcm2835I2CClockDivider
 */
/*@unused@*/inline static void lib_bcm2835_i2c_setClockDivider(uint16_t divider) {
	BCM2835_BSC1->DIV = divider;
}



typedef enum {
	I2C_NORMAL_SPEED = 100000,
	I2C_FULL_SPEED = 400000
} I2CBaudrate;

#define FUNC_PREFIX(x) lib_bcm2835_##x

inline static void i2c_set_address(uint8_t address) {
	lib_bcm2835_i2c_setSlaveAddress(address);
}

inline static bool i2c_begin(void) {
	FUNC_PREFIX(i2c_begin());
	return true;
}

inline static void i2c_set_baudrate(uint32_t baudrate) {
	FUNC_PREFIX(i2c_set_baudrate(baudrate));
}

inline static uint8_t i2c_read(char *buf, uint32_t len) {
	return FUNC_PREFIX(i2c_read((char *) buf, len));
}

extern uint8_t i2c_read_uint8(void);
extern uint16_t i2c_read_uint16(void);
extern uint16_t i2c_read_reg_uint16(uint8_t);
extern uint16_t i2c_read_reg_uint16_delayus(uint8_t, uint32_t);

extern void i2c_write(uint8_t);

inline static void i2c_write_nb(const char *data, uint32_t length) {
	(void) FUNC_PREFIX(i2c_write(data, length));
}

extern void i2c_write_reg_uint8(uint8_t, uint8_t);
extern void i2c_write_uint16(uint16_t);
extern void i2c_write_reg_uint16(uint8_t, uint16_t);
extern void i2c_write_reg_uint16_mask(uint8_t, uint16_t, uint16_t);

extern bool i2c_is_connected(uint8_t);

extern /*@observer@*/const char *i2c_lookup_device(uint8_t);






#define MCP7941X_OK								0
#define MCP7941X_ERROR							1

#define MCP7941X_DEFAULT_SLAVE_ADDRESS			0x6F

struct rtc_time {
	int tm_sec;		///< Seconds.		[0-60]	(1 leap second)
	int tm_min;		///< Minutes.		[0-59]
	int tm_hour;	///< Hours.			[0-23]
	int tm_mday;	///< Day.		 	[1-31]
	int tm_mon;		///< Month.			[0-11]
	int tm_year;	///< Year - 1900
	int tm_wday;	///< Day of week.	[0-6]
	int tm_yday;	///< Days in year.	[0-365]
	int tm_isdst;	///< DST.			[-1/0/1]
};

extern uint8_t mcp7941x_start(uint8_t);
extern void mcp7941x_get_date_time(struct rtc_time *tm);
extern void mcp7941x_set_date_time(/*@out@*/const struct rtc_time *tm);

void sys_time_init(void);

#include "ff.h"
extern FATFS fat_fs;		/* File system object */






















typedef volatile uint32_t rpi_reg_rw_t;
typedef volatile const uint32_t rpi_reg_ro_t;
typedef volatile uint32_t rpi_reg_wo_t;

typedef volatile uint64_t rpi_wreg_rw_t;
typedef volatile const uint64_t rpi_wreg_ro_t;



/** @brief See the documentation for the ARM side timer (Section 14 of the
    BCM2835 Peripherals PDF) */
#define RPI_ARMTIMER_BASE               ( PERIPHERAL_BASE + 0xB400UL )

/** @brief 0 : 16-bit counters - 1 : 23-bit counter */
#define RPI_ARMTIMER_CTRL_23BIT         ( 1 << 1 )

#define RPI_ARMTIMER_CTRL_PRESCALE_1    ( 0 << 2 )
#define RPI_ARMTIMER_CTRL_PRESCALE_16   ( 1 << 2 )
#define RPI_ARMTIMER_CTRL_PRESCALE_256  ( 2 << 2 )

/** @brief 0 : Timer interrupt disabled - 1 : Timer interrupt enabled */
#define RPI_ARMTIMER_CTRL_INT_ENABLE    ( 1 << 5 )
#define RPI_ARMTIMER_CTRL_INT_DISABLE   ( 0 << 5 )

/** @brief 0 : Timer disabled - 1 : Timer enabled */
#define RPI_ARMTIMER_CTRL_ENABLE        ( 1 << 7 )
#define RPI_ARMTIMER_CTRL_DISABLE       ( 0 << 7 )


/** @brief Section 14.2 of the BCM2835 Peripherals documentation details
    the register layout for the ARM side timer */
typedef struct {

    /** The timer load register sets the time for the timer to count down.
        This value is loaded into the timer value register after the load
        register has been written or if the timer-value register has counted
        down to 0. */
    volatile uint32_t Load;

    /** This register holds the current timer value and is counted down when
        the counter is running. It is counted down each timer clock until the
        value 0 is reached. Then the value register is re-loaded from the
        timer load register and the interrupt pending bit is set. The timer
        count down speed is set by the timer pre-divide register. */
    volatile uint32_t Value;

    /** The standard SP804 timer control register consist of 8 bits but in the
        BCM implementation there are more control bits for the extra features.
        Control bits 0-7 are identical to the SP804 bits, albeit some
        functionality of the SP804 is not implemented. All new control bits
        start from bit 8 upwards. */
    volatile uint32_t Control;

    /** The timer IRQ clear register is write only. When writing this register
        the interrupt-pending bit is cleared. When reading this register it
        returns 0x544D5241 which is the ASCII reversed value for "ARMT". */
    volatile uint32_t IRQClear;

    /** The raw IRQ register is a read-only register. It shows the status of
        the interrupt pending bit. 0 : The interrupt pending bits is clear.
        1 : The interrupt pending bit is set.
        The interrupt pending bits is set each time the value register is
        counted down to zero. The interrupt pending bit can not by itself
        generates interrupts. Interrupts can only be generated if the
        interrupt enable bit is set. */
    volatile uint32_t RAWIRQ;

    /** The masked IRQ register is a read-only register. It shows the status
        of the interrupt signal. It is simply a logical AND of the interrupt
        pending bit and the interrupt enable bit. 0 : Interrupt line not
        asserted. 1 :Interrupt line is asserted, (the interrupt pending and
        the interrupt enable bit are set.)  */
    volatile uint32_t MaskedIRQ;

    /** This register is a copy of the timer load register. The difference is
        that a write to this register does not trigger an immediate reload of
        the timer value register. Instead the timer load register value is
        only accessed if the value register has finished counting down to
        zero. */
    volatile uint32_t Reload;

    /** The Pre-divider register is not present in the SP804. The pre-divider
        register is 10 bits wide and can be written or read from. This
        register has been added as the SP804 expects a 1MHz clock which we do
        not have. Instead the pre-divider takes the APB clock and divides it
        down according to:
        timer_clock = apb_clock/(pre_divider+1)
        The reset value of this register is 0x7D so gives a divide by 126. */
    volatile uint32_t PreDivider;

    /** The free running counter is not present in the SP804. The free running
        counter is a 32 bits wide read only register. The register is enabled
        by setting bit 9 of the Timer control register. The free running
        counter is incremented immediately after it is enabled. The timer can
        not be reset but when enabled, will always increment and roll-over.
        The free running counter is also running from the APB clock and has
        its own clock pre-divider controlled by bits 16-23 of the timer
        control register.
        This register will be halted too if bit 8 of the control register is
        set and the ARM is in Debug Halt mode. */
    volatile uint32_t FreeRunningCounter;

    } rpi_arm_timer_t;


extern rpi_arm_timer_t* RPI_GetArmTimer(void);
extern void RPI_ArmTimerInit(void);

extern rpi_sys_timer_t* RPI_GetSystemTimer(void);
extern void RPI_WaitMicroSeconds( uint32_t us );
extern rpi_sys_timer_t* rpiSystemTimer;



/** @brief See Section 7.5 of the BCM2836 ARM Peripherals documentation, the base
    address of the controller is actually xxxxB000, but there is a 0x200 offset
    to the first addressable register for the interrupt controller, so offset the
    base to the first register */
#define RPI_INTERRUPT_CONTROLLER_BASE   ( PERIPHERAL_BASE + 0xB200UL )

/** @brief Bits in the Enable_Basic_IRQs register to enable various interrupts.
    See the BCM2835 ARM Peripherals manual, section 7.5 */
#define RPI_BASIC_ARM_TIMER_IRQ         (1 << 0)
#define RPI_BASIC_ARM_MAILBOX_IRQ       (1 << 1)
#define RPI_BASIC_ARM_DOORBELL_0_IRQ    (1 << 2)
#define RPI_BASIC_ARM_DOORBELL_1_IRQ    (1 << 3)
#define RPI_BASIC_GPU_0_HALTED_IRQ      (1 << 4)
#define RPI_BASIC_GPU_1_HALTED_IRQ      (1 << 5)
#define RPI_BASIC_ACCESS_ERROR_1_IRQ    (1 << 6)
#define RPI_BASIC_ACCESS_ERROR_0_IRQ    (1 << 7)


extern void RPI_EnableGICInterrupts(void);

/** @brief The interrupt controller memory mapped register set */
typedef struct {
    volatile uint32_t IRQ_basic_pending;
    volatile uint32_t IRQ_pending_1;
    volatile uint32_t IRQ_pending_2;
    volatile uint32_t FIQ_control;
    volatile uint32_t Enable_IRQs_1;
    volatile uint32_t Enable_IRQs_2;
    volatile uint32_t Enable_Basic_IRQs;
    volatile uint32_t Disable_IRQs_1;
    volatile uint32_t Disable_IRQs_2;
    volatile uint32_t Disable_Basic_IRQs;
    } rpi_irq_controller_t;
    

/** @brief The BCM2835 Interupt controller peripheral at it's base address */
extern rpi_irq_controller_t* rpiIRQController;

/**
    @brief Return the IRQ Controller register set
*/
rpi_irq_controller_t* RPI_GetIrqController( void );
void RPI_EnableARMTimerInterrupt(void);


typedef struct {
  unsigned int _reset;
  unsigned int _undefined;
  unsigned int _swi;
  unsigned int _prefetched;
  unsigned int _data;
  unsigned int _unused;
  unsigned int _irq;
  unsigned int _fiq;

  unsigned int _reset_pointer;
  unsigned int _undefined_pointer;
  unsigned int _swi_pointer;
  unsigned int _prefetched_pointer;
  unsigned int _data_pointer;
  unsigned int _unused_pointer;
  unsigned int _irq_pointer;
  unsigned int _fiq_pointer;
  } pi_vectors;

// 0x0000008c is a pointer to a structur of->
#define LOADER_STARTING 0
//#define RESET_TYPE_RESTART_LOADER 1
#define RESET_TYPE_RESTART_SELECTION 2
#define RESET_TYPE_RESTART_BOOT_NAKED 3
#define RESET_TYPE_CONTINUE_LOADER 4

#define FLAG_IN_MENU 1
#define FLAG_SELECTION_MADE 2
#define FLAG_LOADER_RUNNING 4
  
  typedef struct {
  unsigned char flags;
  void (*loader)(void);
  unsigned char orientation;
  void *lastSelection; // in loader menu
  char parameter1[127];
  char parameter2[127];
  char parameter3[127];
  char parameter4[127];
  int resetType;
} GlobalMemSettings;

extern GlobalMemSettings *settings;

void cache_flush(void);
























// unsigned int 32 bit

#define ARM_OPCODE_BRANCH(distance)	(0xEA000000 | (distance))
#define ARM_DISTANCE(from, to)		((unsigned int *) &(to) - (unsigned int *) &(from) - 2)

typedef struct TExceptionTable
{
	//unsigned int Reset;
	unsigned int UndefinedInstruction;
	unsigned int SupervisorCall;
	unsigned int PrefetchAbort;
	unsigned int DataAbort;
	unsigned int Unused;
	unsigned int IRQ;
	unsigned int FIQ;
}
TExceptionTable;

#define ARM_EXCEPTION_TABLE_BASE	0x00000004

typedef struct TAbortFrame
{
	unsigned int	sp_irq;
	unsigned int	lr_irq;
	unsigned int	r0;
	unsigned int	r1;
	unsigned int	r2;
	unsigned int	r3;
	unsigned int	r4;
	unsigned int	r5;
	unsigned int	r6;
	unsigned int	r7;
	unsigned int	r8;
	unsigned int	r9;
	unsigned int	r10;
	unsigned int	r11;
	unsigned int	r12;
	unsigned int	sp;
	unsigned int	lr;
	unsigned int	spsr;
	unsigned int	pc;
}
TAbortFrame;

void UndefinedInstructionStub (void);
void PrefetchAbortStub (void);
void DataAbortStub (void);
void IRQStub (void);

void ExceptionHandler (unsigned int nException, TAbortFrame *pFrame);
void InterruptHandler (void);
#define exception_out(PC_OFFSET, EXCEPTION_TYPE) \
  do{ \
   /* static, so stack operations (in assembler), do not willfully destroy these things */ \
   static struct pt_regs *p; \
   static unsigned int *lnk_ptr; /* 32 bit mode - pointer to last executed instruction */ \
   static unsigned int *stack;   /* 32 bit mode - pointer to "old" stack frame */ \
   static unsigned int SPSR; \
   static unsigned int ccr;	 \
   static unsigned int *usr_stack; \
   static unsigned int *fiq_stack; \
   static unsigned int *irq_stack; \
   static unsigned int *svc_stack; \
   static unsigned int *abt_stack; \
   static unsigned int *und_stack; \
   static unsigned int *sys_stack; \
   static int x; \
    \
   /* save all registers! */ \
   /*  save current stack as "pointer" to exception registers */ \
   asm __volatile__ ( "stmfd	sp, {r0-r14}\n" \
		      "sub	sp, sp, #4*15\n"  \
		      "mov %0,  sp\n"  : "=r" (p) : : "memory" );   \
 \
   asm __volatile__ ( "mov %0, r14\n"  : "=r" (r14) );  /* save the link, since above statement gets confused, since it saves "over" the stack */ \
   asm __volatile__ (" mrs %0, SPSR" : "=r" (SPSR) );   /* get the PSR "Before" the exception! */ \
    \
   /*  On data abort exception the LR points to PC+8  */ \
   /*  On IRQ exception the LR points to PC+4  */ \
   /*  get the last executed PC to lnk_ptr */ \
   /*  see DDI0487G_b_armv8_arm.pdf page 6052 */ \
   asm __volatile__ ( /*"sub lr, lr, #8\n" */ \
		      "mov %0, lr" : "=r" (lnk_ptr) ); lnk_ptr -= (PC_OFFSET/4); /* address is 4 byte pointer, so add only offset divided by 4 */\
    \
    /* printf("!%i!!at %p:  !!!\r\n",thisCore(), lnk_ptr); printSemaphore = 1;while (1); */\
    \
   /*  switch to supervisor mode - our "run mode" */ \
   /*  switch back and force ONE one assembler, otherwise the values are not saved correctly to our OWN stack */ \
   asm __volatile__ (	"cps	#0x13\n"   \
			"mov %0, sp\n"  \
			"cps #0x17\n": "=r" (stack) );   \
   svc_stack = stack;    \
 \
  /*  save current stack as "pointer" to access the "user stack" */ \
  /* asm __volatile__ (	"cps	#0x10\n"  */ \
  /*			"mov %0, sp\n"  */\
  /*			"cps #0x17\n": "=r" (usr_stack) );  */ \
  /*  save current stack as "pointer" to access the "sys stack" */ \
  /* asm __volatile__ (	"cps	#0x1f\n"  */ \
  /*			"mov %0, sp\n"  */\
  /*			"cps #0x17\n": "=r" (sys_stack) );  */ \
 \
   asm __volatile__ (	"cps	#0x11\n"   \
			"mov %0, sp\n"  \
			"cps #0x17\n": "=r" (fiq_stack) );   \
 \
   asm __volatile__ (	"cps	#0x12\n"   \
			"mov %0, sp\n"  \
			"cps #0x17\n": "=r" (irq_stack) );   \
 \
   asm __volatile__ (	"cps	#0x1b\n"   \
			"mov %0, sp\n"  \
			"cps #0x17\n": "=r" (und_stack) );   \
 \
   asm __volatile__ ("mov %0, sp\n"  : "=r" (abt_stack) );    \
   \
    printf( "\n!!!!!!!! %s !!!!!!!!\n", EXCEPTION_TYPE); \
    printf( "at %p:  code (dump): 0x%08lX \r\n", lnk_ptr,  *(lnk_ptr)); \
    /*  some sanity check  */ \
    if (((unsigned int )stack) > 0x4000000) \
    { \
      printf( "Stack  %p: -> INVALID\r\n", stack ); \
    } \
    else \
    { \
      unsigned int *stackLoop = stack; \
      for (int i=0; i<10;i++) \
      {printf( "Stack  %p: -> 0x%08lX\r\n", stackLoop, *(stackLoop) );stackLoop++;} \
    } \
 \
  asm __volatile__ (  "MRC  p15,0,r0,c1,c0,0\n" \
		      "mov %0, r0\n"  : "=r" (ccr) );   \
  unsigned int FSR = 0, FAR = 0, core=0; \
  asm volatile ("mrc p15, 0, %0, c5, c0,  0" : "=r" (FSR));\
  asm volatile ("mrc p15, 0, %0, c6, c0,  0" : "=r" (FAR));\
  asm volatile ("mrc p15, 0, %0, c0, c0,  5" : "=r" (core));\
  core = core & 3;\
 \
  printf("========================================\r\n"); \
  printf("R0  (A1): %08X    R8  (V5): %08X\r\n", p->uregs[ PTRACE_R0_idx ] , p->uregs[ PTRACE_R8_idx ]); \
  printf("R1  (A2): %08X    R9  (SB): %08X\r\n", p->uregs[ PTRACE_R1_idx ] , p->uregs[ PTRACE_R9_idx ]); \
  printf("R2  (A3): %08X    R10 (SL): %08X\r\n", p->uregs[ PTRACE_R2_idx ] , p->uregs[ PTRACE_R10_idx ]); \
  printf("R3  (A4): %08X    R11 (FP): %08X\r\n", p->uregs[ PTRACE_R3_idx ] , p->uregs[ PTRACE_R11_idx ]); \
  printf("R4  (V1): %08X    R12 (IP): %08X\r\n", p->uregs[ PTRACE_R4_idx ] , p->uregs[ PTRACE_R12_idx ]); \
  printf("R5  (V2): %08X    R13 (SP): %08X\r\n", p->uregs[ PTRACE_R5_idx ] , stack ); \
  printf("R6  (V3): %08X    R14 (LR): %08X\r\n", p->uregs[ PTRACE_R6_idx ] , r14); \
  printf("R7  (V4): %08X    R15 (PC): %08X\r\n", p->uregs[ PTRACE_R7_idx ] , lnk_ptr); \
  printf("\nusr_SP:   n/a\r\n"/*,usr_stack*/); \
  printf("fiq_SP:   %08X\r\n",fiq_stack); \
  printf("irq_SP:   %08X\r\n",irq_stack); \
  printf("svc_SP:   %08X\r\n",svc_stack); \
  printf("abt_SP:   %08X\r\n",abt_stack); \
  printf("und_SP:   %08X\r\n",und_stack); \
  printf("sys_SP:   n/a\r\n"/*,sys_stack*/); \
  printf("\nSPSR:     %08X\r\n", SPSR ); \
  \
  /*  now decode the flags  */ \
  x = SPSR; \
  printf("    FLAGS     %c", ( x & ARM_CPSR_N_BIT ) ? 'N' : 'n' ); \
  printf("%c", ( x & ARM_CPSR_Z_BIT ) ? 'Z' : 'z' ); \
  printf("%c", ( x & ARM_CPSR_C_BIT ) ? 'C' : 'c' ); \
  printf("%c", ( x & ARM_CPSR_V_BIT ) ? 'V' : 'v' ); \
  printf("..."); \
  printf("%c", ( x & ARM_CPSR_F_BIT ) ? 'F' : 'f' ); \
  printf("%c", ( x & ARM_CPSR_I_BIT ) ? 'I' : 'i' ); \
  printf("%c\n    ARM-Mode  ", ( x & ARM_CPSR_T_BIT ) ? 'T' : 't' ); \
  switch( ARM_MODE_MASK & x ) \
  { \
  case ARM_USR_MODE: \
    printf("user-mode\r\n"); \
    break; \
  case ARM_FIQ_MODE: \
    printf("fiq-mode\r\n"); \
    break; \
  case ARM_IRQ_MODE: \
    printf("irq-mode\r\n"); \
    break; \
  case ARM_SVC_MODE: \
    printf("svc-mode\r\n"); \
    break; \
  case ARM_ABT_MODE: \
    /* these are not implemented yet!  */ \
    /* if( p->uregs[ PTRACE_FRAMETYPE_idx ] == PTRACE_FRAMETYPE_pfa ) */ \
    /* {printf("pfa-mode\r\n");} */  \
    /* else if( p->uregs[ PTRACE_FRAMETYPE_idx ] == PTRACE_FRAMETYPE_da  ) */ \
    /* {printf("da-mode\r\n");}  */ \
    /* else  */ \
    {printf("unknown-abort-mode\r\n");} \
    break; \
  case ARM_UND_MODE: \
    printf("und-mode\r\n"); \
    break; \
  case ARM_SYS_MODE: \
    printf("sys-mode\r\n"); \
    break; \
  } \
  /* alignment check, see: /DDI0487G_b_armv8_arm.pdf page 4312 */ \
  printf("\r\n"); \
  printf("Core:     %01X\r\n", core ); \
  printf("CCR:      %08X\r\n", ccr ); /* see: DDI0487G_b_armv8_arm.pdf 6810 */ \
  printf("    MMU                  : "); if (ccr & (1<<0))  printf("(1) enabled");else printf("(0) disabled"); \
  printf("\n    alignment fault check: "); if (ccr & (1<<1))  printf("(1) enabled");else printf("(0) disabled"); \
  printf("\n    data cache           : "); if (ccr & (1<<2))  printf("(1) enabled");else printf("(0) disabled"); \
  printf("\n    instruction cache    : "); if (ccr & (1<<12))  printf("(1) enabled");else printf("(0) disabled"); \
  printf("\n    endian               : "); if (ccr & (1<<25))  printf("(1) big");else printf("(0) little"); \
  printf("\n    access permission    : "); if (ccr & (1<<29))  printf("(1) AP[0] is the Access flag");else printf("(0) AP[0] is access permission bit"); \
  printf("\n========================================\r\n"); \
  if (PC_OFFSET == 8) /* Data Abort */ \
  {\
    printf("\nAdditional Data Abort information\n"); \
    printf("FAR:      %08X (Modified Virtual Address (MVA))\r\n", FAR); \
    printf("DFSR:     %08X\r\n", FSR ); /* see: https://developer.arm.com/documentation/ddi0211/k/system-control-coprocessor/system-control-coprocessor-register-descriptions/c5--data-fault-status-register--dfsr*/\
    char *fsr_typ[] =  \
    { \
      "No function, reset value",               /*  0 */ \
      "Alignment fault",			/*  1 */ \
      "Debug event fault",			/*  2 */ \
      "Access Flag fault on Section",		/*  3 */ \
      "Cache maintenance operation fault",	/*  4 */ \
      "Translation fault on Section",		/*  5 */ \
      "Access Flag fault on Page",		/*  6 */ \
      "Translation fault on Page",		/*  7 */ \
      "Precise External Abort",			/*  8 */ \
      "Domain fault on Section",		/*  9 */ \
      "No function",				/* 10 */ \
      "Domain fault on Page",			/* 11 */ \
      "External abort on translation, first level",/* 12 */ \
      "Permission fault on Section",		/* 13 */ \
      "External abort on translation, second level",/* 14 */ \
      "Permission fault on Page",		/* 15 */ \
      "No function",				/* 16 */ \
      "No function",				/* 17 */ \
      "No function",				/* 18 */ \
      "No function",				/* 19 */ \
      "No function",				/* 20 */ \
      "No function",				/* 21 */ \
      "Imprecise External Abort",		/* 22 */ \
      "No function",				/* 23 */ \
      "No function",				/* 24 */ \
      "No function",				/* 25 */ \
      "No function",				/* 26 */ \
      "No function",				/* 27 */ \
      "No function",				/* 28 */ \
      "No function",				/* 29 */ \
      "No function",				/* 30 */ \
      "No function",				/* 31 */ \
    }; \
    int typ = FSR & 0xf; \
    if (FSR & 1024) typ += 16; \
    int domain = FSR >> 4; \
    domain = domain & 0xf; \
     \
    printf("  typ:    %s\r\n", fsr_typ[typ] ); \
    printf("  domain: %01X\r\n", domain ); \
    if (FSR & 2048)  \
    printf("  on:     WRITE\r\n", domain ); \
    else  \
    printf("  on:     READ\r\n", domain ); \
  }\
   \
   asm __volatile__ ("add	sp, sp, #4*15\n");   \
  }while (0);
  
  
/// \brief Read 8-bit value from MMIO address
static inline unsigned char read8 (unsigned int nAddress)
{
	return *(unsigned char volatile *) nAddress;
}

/// \brief Write 8-bit value to MMIO address
static inline void write8 (unsigned int nAddress, unsigned char uchValue)
{
	*(unsigned char volatile *) nAddress = uchValue;
}

/// \brief Read 16-bit value from MMIO address
static inline unsigned short read16 (unsigned int nAddress)
{
	return *(unsigned short volatile *) nAddress;
}

/// \brief Write 16-bit value to MMIO address
static inline void write16 (unsigned int nAddress, unsigned short usValue)
{
	*(unsigned short volatile *) nAddress = usValue;
}

/// \brief Read 32-bit value from MMIO address
static inline unsigned int read32 (unsigned int nAddress)
{
	return *(unsigned int volatile *) nAddress;
}

/// \brief Write 32-bit value to MMIO address
static inline void write32 (unsigned int nAddress, unsigned int nValue)
{
	*(unsigned int volatile *) nAddress = nValue;
}
unsigned thisCore (void);			// returns number of current core (0..CORES-1)
void getLock();
void releaseLock();
