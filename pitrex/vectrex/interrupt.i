#include <stdint.h>



//#include "rpi-base.h"
/*
    Part of the Raspberry-Pi Bare Metal Tutorials
    https://www.valvers.com/rpi/bare-metal/
    Copyright (c) 2013-2018, Brian Sidebotham
    This software is licensed under the MIT License.
    Please see the LICENSE file included with this software.
*/

#ifndef RPI_BASE_H
#define RPI_BASE_H


#if defined( RPI0 ) || defined( RPI1 )
    #define PERIPHERAL_BASE       0x20000000UL
#elif defined( RPI2 ) || defined( RPI3 )
    #define PERIPHERAL_BASE       0x3F000000UL
#elif defined( RPI4 )
    #define PERIPHERAL_BASE       0xFE000000UL
    #define GIC400_BASE           (0xFF840000UL)
#else
    #error Unknown RPI Model!
#endif

/* System Frequencies From:
   https://www.raspberrypi.org/documentation/configuration/config-txt/overclocking.md */
#if defined( RPI0 ) || defined( RPI3 )
#define SYSFREQ 400000000UL
#elif defined( RPI1 ) || defined ( RPI2 )
#define SYSFREQ 250000000UL
#elif defined( RPI4 )
#define SYSFREQ 500000000UL
#else
    #error Unknown RPI Model!
#endif

typedef volatile uint32_t rpi_reg_rw_t;
typedef volatile const uint32_t rpi_reg_ro_t;
typedef volatile uint32_t rpi_reg_wo_t;

typedef volatile uint64_t rpi_wreg_rw_t;
typedef volatile const uint64_t rpi_wreg_ro_t;

#endif







//#include "rpi-armtimer.h"
/*
    Part of the Raspberry-Pi Bare Metal Tutorials
    https://www.valvers.com/rpi/bare-metal/
    Copyright (c) 2013-2018, Brian Sidebotham
    This software is licensed under the MIT License.
    Please see the LICENSE file included with this software.
*/

#ifndef RPI_ARMTIMER_H
#define RPI_ARMTIMER_H

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

#endif
/*
static rpi_arm_timer_t* rpiArmTimer = (rpi_arm_timer_t*)RPI_ARMTIMER_BASE;

rpi_arm_timer_t* RPI_GetArmTimer(void)
{
    return rpiArmTimer;
}

void RPI_ArmTimerInit(void)
{

}
*/






//#include "rpi-interrupts.h"

#ifndef RPI_INTERRUPTS_H
#define RPI_INTERRUPTS_H

extern void RPI_EnableARMTimerInterrupt(void);

#endif




#ifndef RPI_SYSTIMER_H
#define RPI_SYSTIMER_H

#define RPI_SYSTIMER_BASE       ( PERIPHERAL_BASE + 0x3000UL )

typedef struct {
    volatile uint32_t control_status;
    volatile uint32_t counter_lo;
    volatile uint32_t counter_hi;
    volatile uint32_t compare0;
    volatile uint32_t compare1;
    volatile uint32_t compare2;
    volatile uint32_t compare3;
    } rpi_sys_timer_t;


extern rpi_sys_timer_t* RPI_GetSystemTimer(void);
extern void RPI_WaitMicroSeconds( uint32_t us );

#endif

static rpi_sys_timer_t* rpiSystemTimer = (rpi_sys_timer_t*)RPI_SYSTIMER_BASE;
/*

rpi_sys_timer_t* RPI_GetSystemTimer(void)
{
    return rpiSystemTimer;
}

void RPI_WaitMicroSeconds( uint32_t us )
{
    volatile uint32_t ts = rpiSystemTimer->counter_lo;

    while( ( rpiSystemTimer->counter_lo - ts ) < us )
    {
         // BLANK * /
    }
}
*/


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
static rpi_irq_controller_t* rpiIRQController =
        (rpi_irq_controller_t*)RPI_INTERRUPT_CONTROLLER_BASE;


/**
    @brief Return the IRQ Controller register set
*/
static rpi_irq_controller_t* RPI_GetIrqController( void )
{
    return rpiIRQController;
}

void RPI_EnableARMTimerInterrupt(void)
{
    RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;
}





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












extern void _enable_interrupts(void);
extern void _disable_interrupts( void );
