// PiTrex Discrete Read & Write Functions

#include <unistd.h>
#include <stdio.h>

#include "pitrexio-gpio.h"
#include "via6522.h"

#define VECTREX_HALT_NOT_ASSERTED				-1

//VIA Interrupt Flag Register Bits:
#define VECTREX_VIA_IFR_CA2_INTERRUPT_FLAG			0
#define VECTREX_VIA_IFR_CA1_INTERRUPT_FLAG			1
#define VECTREX_VIA_IFR_SHIFT_REGISTER_INTERRUPT_FLAG		2
#define VECTREX_VIA_IFR_CB2_INTERRUPT_FLAG			3
#define VECTREX_VIA_IFR_CB1_INTERRUPT_FLAG			4
#define VECTREX_VIA_IFR_TIMER_2_INTERRUPT_FLAG			5
#define VECTREX_VIA_IFR_TIMER_1_INTERRUPT_FLAG			6
#define VECTREX_VIA_IFR_IRQ_STATUS_FLAG				7


#define MAP_FAILED	((void *) -1)

/*! This means pin HIGH, true, 3.3volts on a pin. */
#define HIGH 0x1
/*! This means pin LOW, false, 0volts on a pin. */
#define LOW  0x0

#if RASPPI == 1
    #define BCM2835_PERI_BASE     0x20000000UL 
#else
    #if RASPPI == 4
	#define BCM2835_PERI_BASE     0xFE000000UL
    #else // 3 and zero 2
	#define BCM2835_PERI_BASE     0x3F000000UL
    #endif
#endif



/*! Size of the peripherals block on RPi 1 */
#define BCM2835_PERI_SIZE               0x01000000

/*! Offsets for the bases of various peripherals within the peripherals block
  /   Base Address of the System Timer registers
*/
#define BCM2835_ST_BASE					0x3000
/*! Base Address of the Pads registers */
#define BCM2835_GPIO_PADS               0x100000
/*! Base Address of the Clock/timer registers */
#define BCM2835_CLOCK_BASE              0x101000
/*! Base Address of the GPIO registers */
#define BCM2835_GPIO_BASE               0x200000
/*! Base Address of the SPI0 registers */
#define BCM2835_SPI0_BASE               0x204000
/*! Base Address of the BSC0 registers */
#define BCM2835_BSC0_BASE 				0x205000
/*! Base Address of the PWM registers */
#define BCM2835_GPIO_PWM                0x20C000
/*! Base Address of the AUX registers */
#define BCM2835_AUX_BASE				0x215000
/*! Base Address of the AUX_SPI1 registers */
#define BCM2835_SPI1_BASE				0x215080
/*! Base Address of the AUX_SPI2 registers */
#define BCM2835_SPI2_BASE				0x2150C0
/*! Base Address of the BSC1 registers */
#define BCM2835_BSC1_BASE				0x804000

/*! GPIO register offsets from BCM2835_GPIO_BASE. 
  Offsets into the GPIO Peripheral block in bytes per 6.1 Register View 
*/
#define BCM2835_GPFSEL0                      0x0000 /*!< GPIO Function Select 0 */
#define BCM2835_GPFSEL1                      0x0004 /*!< GPIO Function Select 1 */
#define BCM2835_GPFSEL2                      0x0008 /*!< GPIO Function Select 2 */
#define BCM2835_GPFSEL3                      0x000c /*!< GPIO Function Select 3 */
#define BCM2835_GPFSEL4                      0x0010 /*!< GPIO Function Select 4 */
#define BCM2835_GPFSEL5                      0x0014 /*!< GPIO Function Select 5 */
#define BCM2835_GPSET0                       0x001c /*!< GPIO Pin Output Set 0 */
#define BCM2835_GPSET1                       0x0020 /*!< GPIO Pin Output Set 1 */
#define BCM2835_GPCLR0                       0x0028 /*!< GPIO Pin Output Clear 0 */
#define BCM2835_GPCLR1                       0x002c /*!< GPIO Pin Output Clear 1 */
#define BCM2835_GPLEV0                       0x0034 /*!< GPIO Pin Level 0 */
#define BCM2835_GPLEV1                       0x0038 /*!< GPIO Pin Level 1 */
#define BCM2835_GPEDS0                       0x0040 /*!< GPIO Pin Event Detect Status 0 */
#define BCM2835_GPEDS1                       0x0044 /*!< GPIO Pin Event Detect Status 1 */
#define BCM2835_GPREN0                       0x004c /*!< GPIO Pin Rising Edge Detect Enable 0 */
#define BCM2835_GPREN1                       0x0050 /*!< GPIO Pin Rising Edge Detect Enable 1 */
#define BCM2835_GPFEN0                       0x0058 /*!< GPIO Pin Falling Edge Detect Enable 0 */
#define BCM2835_GPFEN1                       0x005c /*!< GPIO Pin Falling Edge Detect Enable 1 */
#define BCM2835_GPHEN0                       0x0064 /*!< GPIO Pin High Detect Enable 0 */
#define BCM2835_GPHEN1                       0x0068 /*!< GPIO Pin High Detect Enable 1 */
#define BCM2835_GPLEN0                       0x0070 /*!< GPIO Pin Low Detect Enable 0 */
#define BCM2835_GPLEN1                       0x0074 /*!< GPIO Pin Low Detect Enable 1 */
#define BCM2835_GPAREN0                      0x007c /*!< GPIO Pin Async. Rising Edge Detect 0 */
#define BCM2835_GPAREN1                      0x0080 /*!< GPIO Pin Async. Rising Edge Detect 1 */
#define BCM2835_GPAFEN0                      0x0088 /*!< GPIO Pin Async. Falling Edge Detect 0 */
#define BCM2835_GPAFEN1                      0x008c /*!< GPIO Pin Async. Falling Edge Detect 1 */
#define BCM2835_GPPUD                        0x0094 /*!< GPIO Pin Pull-up/down Enable */
#define BCM2835_GPPUDCLK0                    0x0098 /*!< GPIO Pin Pull-up/down Enable Clock 0 */
#define BCM2835_GPPUDCLK1                    0x009c /*!< GPIO Pin Pull-up/down Enable Clock 1 */

#define BCM2835_ST_CS 			0x0000 /*!< System Timer Control/Status */
#define BCM2835_ST_CLO 			0x0004 /*!< System Timer Counter Lower 32 bits */
#define BCM2835_ST_CHI 			0x0008 /*!< System Timer Counter Upper 32 bits */

/* Physical address and size of the peripherals block
// May be overridden on RPi2
*/
uint32_t *bcm2835_peripherals_base = (uint32_t *)BCM2835_PERI_BASE;
uint32_t bcm2835_peripherals_size = BCM2835_PERI_SIZE;

/* Virtual memory address of the mapped peripherals block
 */
//uint32_t *bcm2835_peripherals = (uint32_t *)MAP_FAILED;

/* And the register bases within the peripherals block
 */
/*
volatile uint32_t *bcm2835_gpio        = (uint32_t *)MAP_FAILED;
volatile uint32_t *bcm2835_pwm         = (uint32_t *)MAP_FAILED;
volatile uint32_t *bcm2835_clk         = (uint32_t *)MAP_FAILED;
volatile uint32_t *bcm2835_pads        = (uint32_t *)MAP_FAILED;
volatile uint32_t *bcm2835_spi0        = (uint32_t *)MAP_FAILED;
volatile uint32_t *bcm2835_bsc0        = (uint32_t *)MAP_FAILED;
volatile uint32_t *bcm2835_bsc1        = (uint32_t *)MAP_FAILED;
volatile uint32_t *bcm2835_st	       = (uint32_t *)MAP_FAILED;
volatile uint32_t *bcm2835_aux	       = (uint32_t *)MAP_FAILED;
volatile uint32_t *bcm2835_spi1        = (uint32_t *)MAP_FAILED;
*/
volatile uint32_t *    bcm2835_peripherals = (volatile uint32_t*)BCM2835_PERI_BASE;
volatile uint32_t *    bcm2835_pads = ((uint32_t*)(BCM2835_PERI_BASE)) + BCM2835_GPIO_PADS/4;
volatile uint32_t *    bcm2835_clk  = ((uint32_t*)(BCM2835_PERI_BASE)) + BCM2835_CLOCK_BASE/4;
volatile uint32_t *    bcm2835_gpio = ((uint32_t*)(BCM2835_PERI_BASE)) + BCM2835_GPIO_BASE/4;
volatile uint32_t *    bcm2835_pwm  = ((uint32_t*)(BCM2835_PERI_BASE)) + BCM2835_GPIO_PWM/4;
volatile uint32_t *    bcm2835_spi0 = ((uint32_t*)(BCM2835_PERI_BASE)) + BCM2835_SPI0_BASE/4;
volatile uint32_t *    bcm2835_bsc0 = ((uint32_t*)(BCM2835_PERI_BASE)) + BCM2835_BSC0_BASE/4;
volatile uint32_t *    bcm2835_bsc1 = ((uint32_t*)(BCM2835_PERI_BASE)) + BCM2835_BSC1_BASE/4;
volatile uint32_t *    bcm2835_st   = ((uint32_t*)(BCM2835_PERI_BASE)) + BCM2835_ST_BASE/4;
volatile uint32_t *    bcm2835_aux  = ((uint32_t*)(BCM2835_PERI_BASE)) + BCM2835_AUX_BASE/4;
volatile uint32_t *    bcm2835_spi1 = ((uint32_t*)(BCM2835_PERI_BASE)) + BCM2835_SPI1_BASE/4;

typedef enum
{
    BCM2835_GPIO_PUD_OFF     = 0x00,   /*!< Off ? disable pull-up/down 0b00 */
    BCM2835_GPIO_PUD_DOWN    = 0x01,   /*!< Enable Pull Down control 0b01 */
    BCM2835_GPIO_PUD_UP      = 0x02    /*!< Enable Pull Up control 0b10  */
} bcm2835PUDControl;


//Comment this out to poll RDY input instead of using Event Detection Register:
//Note: *_end functions may not work reliably without using the EDR.
//#define USE_EDR

//Set this to configure GPIO to use UART0 instead of UART1 on bits 14 and 15.
//This is the default config used by Linux when run on Raspberry Pis without WiFi support
//(but changing this is only required if the serial terminal is going to be used).
//#define UART0

/*
  Port Assignments:
  ^^^^^^^^^^^^^^^^^
Descrip.|Bit    |Pin    |
D0      |16     |36     |
D1      |17     |11     |
D2      |18     |12     |
D3      |19     |35     |
D4      |20     |38     |
D5      |21     |40     |
D6      |22     |15     |
D7      |23     |16     |

A0      |7      |26     |
A1      |8      |24     |
A2      |9      |21     |
A3      |10     |19     |
A12     |11     |23     |
A14     |12     |32     |
R/#W    |13     |33     |

PB6     |5      |29     |
IRQ_LATCH |6    |31     |

RDY(#OE)|24     |18     |
LATCH_EN|25     |22     |
IRQ     |26     |37     |
#HALT   |27 |13     | -Jumper to GND
#D/A    |N/A    |9      | -GND

3V3     |N/A    |1      |
5V      |N/A    |2      |
GND     |N/A    |6      |

Note: Pins counted from 1, bits counted from 0.
*/

#define CARTRIDGE_RW_GPIO         13
#define CARTRIDGE_LATCH_EN_GPIO   25
#define CARTRIDGE_READY_GPIO      24


volatile uint32_t *bcm2835_gpio_GPCLR0;
volatile uint32_t *bcm2835_gpio_GPFSEL1;
volatile uint32_t *bcm2835_gpio_GPFSEL2;
volatile uint32_t *bcm2835_gpio_GPSET0;
volatile uint32_t *bcm2835_gpio_GPLEV0;



// -1 unkown
// 1 = yes we are in write mode
// 0 = no we are not in write mode
static int isWriteMode = -1;

volatile static inline uint32_t _bcm2835_peri_read(volatile uint32_t* paddr)
{
    uint32_t ret;
    ret = *paddr;
    return ret;
}
static inline void _bcm2835_peri_write(volatile uint32_t* paddr, uint32_t value)
{
    __sync_synchronize();
    *paddr = value;
}
static inline void _bcm2835_gpio_set(uint8_t pin)
{
    volatile uint32_t* paddr = bcm2835_gpio + BCM2835_GPSET0/4 + pin/32;
    uint8_t shift = pin % 32;
    _bcm2835_peri_write(paddr, 1 << shift);
}
volatile static inline uint8_t _bcm2835_gpio_lev(uint8_t pin)
{
    volatile uint32_t* paddr = bcm2835_gpio + BCM2835_GPLEV0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = _bcm2835_peri_read(paddr);
    return (value & (1 << shift)) ? HIGH : LOW;
}

// READ: For ALT0 (UART0) on 14-15, use 0x24249. For ALT5 (UART1), use 0x12249.
// GPIO Pins 10-13 = OUTPUT| 14-15 = TTY | 16-19 = INPUT
// WRITE: For ALT0 (UART0) on 14-15, use 0x9264249. For ALT5 (UART1), use 0x9252249.
// UART 1
#define PIN_READ_DEF1 do{_bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPFSEL1/4), 0x12249);_bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPFSEL2/4), 0x8000);} while(0)
#define PIN_WRITE_DEF1 do{_bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPFSEL1/4), 0x9252249);_bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPFSEL2/4), 0x8249);} while(0)

#define INIT_VIA_READ1 \
    do \
    { \
        if (isWriteMode != 0) \
        { \
            isWriteMode = 0; \
            PIN_READ_DEF1; \
        } \
    } while(0)

#define INIT_VIA_WRITE1 \
    do \
    { \
        if (isWriteMode != 1) \
        { \
            isWriteMode = 1; \
            PIN_WRITE_DEF1; \
        } \
    } while(0)

    
int _is_smp = 0;
#define CORES 4
unsigned _thisCore (void)			// returns number of current core (0..CORES-1)
{
	unsigned int nMPIDR;
	asm volatile ("mrc p15, 0, %0, c0, c0, 5" : "=r" (nMPIDR));
	return nMPIDR & (CORES-1);
}




// Read a byte from the Vectrex at the specified address
volatile unsigned char vectrexread (unsigned int address)
{
   
if ((_is_smp) && (_thisCore () != 1)) printf("Shit hits the fan\n");
      unsigned long shortaddress = (0x30 | (address & 0x0F)) << 7;
    _bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPCLR0/4), 0xFFFFFFFF);    //Set all outputs Low

    //Set Data bus to Input mode:
    INIT_VIA_READ1;

    _bcm2835_gpio_set (13); // R/#W High
    // Set Address Bus
    _bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPSET0/4), shortaddress); // Set Highs

    char status;
    _bcm2835_gpio_set (25); // LATCH EN High

    unsigned char datain;
    // Poll RDY for LOW to HIGH transistion:

    do
    {
     status = _bcm2835_gpio_lev (24);
    }
    while (status); // Loop until RDY goes Low

    do
    {
     status = _bcm2835_gpio_lev (24);
    }
    while (!status); // Loop until RDY goes High

    datain = ( _bcm2835_peri_read (bcm2835_gpio + BCM2835_GPLEV0/4) >> 16) & 0xFF; // Read data byte
    _bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPCLR0/4), 0xFFFFDFFF); //LATCH EN Low, IRQ LATCH Low, R/#W High



    return datain;
}

// Write a byte to the Vectrex at the specified address.
// Returns after write is completed.
void vectrexwrite (unsigned int address, unsigned char data)
{
if ((_is_smp) && (_thisCore () != 1)) printf("Shit hits the fan\n");
    unsigned long shortaddress = (0x30 | (address & 0x0F)) << 7;
    _bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPCLR0/4), 0xFFFFFFFF); //LATCH EN Low  R/#W Low

    // Data Output Mode:
    INIT_VIA_WRITE1;

    _bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPSET0/4), (data << 16) | shortaddress ); // Data on Data Bus and Address on Address Bus
    _bcm2835_gpio_set (25); //LATCH EN High

    char status;
    // Poll RDY for LOW to HIGH transistion:

    do
    {
    status = _bcm2835_gpio_lev (24);
    }
    while (status); // Loop until RDY goes Low

    do
    {
     status = _bcm2835_gpio_lev (24);
    }
    while (!status); // Loop until RDY goes High

    _bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPCLR0/4), 0xFFFFFFFF); //LATCH EN Low IRQ LATCH Low  R/#W Low
    _bcm2835_gpio_set (13); // R/#W High
}


/* Read the System Timer Counter (64-bits) */
volatile uint64_t bcm2835_st_read(void)
{
    volatile uint32_t* paddr;
    uint32_t hi, lo;
    uint64_t st;

    if (bcm2835_st==MAP_FAILED)
    {
	printf ("Read system time failed - try running with SUDO \r\n");
	return 0;
    }

    paddr = bcm2835_st + BCM2835_ST_CHI/4;
    hi = _bcm2835_peri_read(paddr);

    paddr = bcm2835_st + BCM2835_ST_CLO/4;
    lo = _bcm2835_peri_read(paddr);

    paddr = bcm2835_st + BCM2835_ST_CHI/4;
    st = _bcm2835_peri_read(paddr);

    /* Test for overflow */
    if (st == hi)
    {
        st <<= 32;
        st += lo;
    }
    else
    {
        st <<= 32;
        paddr = bcm2835_st + BCM2835_ST_CLO/4;
        st += _bcm2835_peri_read(paddr);
    }
    return st;
}


/* Delays for the specified number of microseconds with offset */
void bcm2835_st_delay(uint64_t offset_micros, uint64_t micros)
{
  volatile uint64_t cu=0;
    uint64_t compare = offset_micros + micros;
    while((cu=bcm2835_st_read()) < compare)
	;
}

/* microseconds */
void bcm2835_delayMicroseconds(uint64_t micros)
{
	uint64_t start = bcm2835_st_read();
	bcm2835_st_delay(start, micros);
}

/* Some convenient arduino-like functions
// milliseconds
*/
void bcm2835_delay(unsigned int millis)
{
  bcm2835_delayMicroseconds((uint64_t)((uint64_t)1000 * (uint64_t)millis));
}

/* Set pullup/down */
void bcm2835_gpio_pud(uint8_t pud)
{
    volatile uint32_t* paddr = bcm2835_gpio + BCM2835_GPPUD/4;
    _bcm2835_peri_write(paddr, pud);
}

/* Pullup/down clock
// Clocks the value of pud into the GPIO pin
*/
void bcm2835_gpio_pudclk(uint8_t pin, uint8_t on)
{
    volatile uint32_t* paddr = bcm2835_gpio + BCM2835_GPPUDCLK0/4 + pin/32;
    uint8_t shift = pin % 32;
    _bcm2835_peri_write(paddr, (on ? 1 : 0) << shift);
}

void bcm2835_gpio_set_pud(uint8_t pin, uint8_t pud)
{
    bcm2835_gpio_pud(pud);
    bcm2835_delayMicroseconds(10);
    bcm2835_gpio_pudclk(pin, 1);
    bcm2835_delayMicroseconds(10);
    bcm2835_gpio_pud(BCM2835_GPIO_PUD_OFF);
    bcm2835_gpio_pudclk(pin, 0);
}


int bcm2835_init(void)
{
/*  
    bcm2835_peripherals = (uint32_t*)BCM2835_PERI_BASE;
    bcm2835_pads = bcm2835_peripherals + BCM2835_GPIO_PADS/4;
    bcm2835_clk  = bcm2835_peripherals + BCM2835_CLOCK_BASE/4;
    bcm2835_gpio = bcm2835_peripherals + BCM2835_GPIO_BASE/4;
    bcm2835_pwm  = bcm2835_peripherals + BCM2835_GPIO_PWM/4;
    bcm2835_spi0 = bcm2835_peripherals + BCM2835_SPI0_BASE/4;
    bcm2835_bsc0 = bcm2835_peripherals + BCM2835_BSC0_BASE/4;
    bcm2835_bsc1 = bcm2835_peripherals + BCM2835_BSC1_BASE/4;
    bcm2835_st   = bcm2835_peripherals + BCM2835_ST_BASE/4;
    bcm2835_aux  = bcm2835_peripherals + BCM2835_AUX_BASE/4;
    bcm2835_spi1 = bcm2835_peripherals + BCM2835_SPI1_BASE/4;
*/    
    return 1; /* Success */
}

/* Close this library and deallocate everything */
int bcm2835_close(void)
{
/*  
    bcm2835_peripherals = MAP_FAILED;
    bcm2835_gpio = MAP_FAILED;
    bcm2835_pwm  = MAP_FAILED;
    bcm2835_clk  = MAP_FAILED;
    bcm2835_pads = MAP_FAILED;
    bcm2835_spi0 = MAP_FAILED;
    bcm2835_bsc0 = MAP_FAILED;
    bcm2835_bsc1 = MAP_FAILED;
    bcm2835_st   = MAP_FAILED;
    bcm2835_aux  = MAP_FAILED;
    bcm2835_spi1 = MAP_FAILED;
*/    
    return 1; /* Success */
}


// Initialise GPIO for communication with the Vectrex
// Vectrex can be initialised after the GPIO if viaconfig == 1,
// which sets it to the same initialisation state as the Vectrex BIOS would.
// With viaconfig == 0, the VIA and PSG registers are unchanged.
int vectrexinit (char viaconfig)
{
    unsigned char psgaddress;
//  bcm2835_debug(1); //Set debugging mode (GPIO not set, but actions reported)
    int result = bcm2835_init();

    isWriteMode = -1;
    bcm2835_gpio_GPCLR0 =  (bcm2835_gpio + BCM2835_GPCLR0  /4);
    bcm2835_gpio_GPFSEL1 = (bcm2835_gpio + BCM2835_GPFSEL1 /4);
    bcm2835_gpio_GPFSEL2 = (bcm2835_gpio + BCM2835_GPFSEL2 /4);
    bcm2835_gpio_GPSET0 =  (bcm2835_gpio + BCM2835_GPSET0  /4);
    bcm2835_gpio_GPLEV0 =  (bcm2835_gpio + BCM2835_GPLEV0  /4);
// 
    if (result)
    {
     //Set Initial State (ideally set to configured defaults at power-on):
     //NOTE: This will disable other functions of these GPIO pins (except for the serial terminal on the UART)
      //Input/Output Modes:
     _bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPFSEL0/4), 0x9240000);    // GPIO Pins 0-5   = INPUT | 6-9   = OUTPUT
     _bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPFSEL1/4), 0x24249);
     _bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPFSEL2/4), 0x8000);       // GPIO Pins 20-24 = INPUT | 25    = OUTPUT | 26-29 = INPUT

     //Output Levels:
     _bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPCLR0/4), 0xFFFFDFFF);    //Set all outputs Low except R/#W
     bcm2835_gpio_set_pud (27, BCM2835_GPIO_PUD_UP);  // Enable Pull-Up on #HALT
     bcm2835_gpio_set_pud (24, BCM2835_GPIO_PUD_OFF); // Disable PU/D on RDY
     _bcm2835_gpio_set (13); // R/#W High

     //Check that Vectrex CPU is halted.
     if (_bcm2835_gpio_lev (27))
      result = VECTREX_HALT_NOT_ASSERTED;
     else
     {
      //Initialise 6522 (VIA) and AY3-8912 (PSG) in standard Vectrex start-up state (with reference to Vectrex BIOS "INITALL" routine):
      if (viaconfig == 1)
      {
       vectrexwrite (VIA_DDR_b, 0x9F); // All Outputs except COMPARE input and PB6 at Cartridge Port
       vectrexwrite (VIA_DDR_a, 0xFF); // All Outputs
       vectrexwrite (VIA_aux_cntl, 0x98); //Shift Reg. Enabled, T1 PB7 Enabled
       vectrexwrite (VIA_t1_cnt_lo, 0x7F); // default scale = 0x7f
       vectrexwrite (VIA_t2_lo, 0x30);
       vectrexwrite (VIA_t2_hi, 0x75); // T2 set to Refresh = MSEC20. Reset T2 Interrupt Flag and start Frame Timer.

       // Clear Programmable Sound Generator (PSG) registers
       //  Should be done by the hardware reset signal anyway. But monkey see, monkey do.
       for (psgaddress = 0x0E; psgaddress > 0; psgaddress--)
       {
        vectrexwrite (VIA_port_a, psgaddress); // Address
        vectrexwrite (VIA_port_b, 0x19);
        vectrexwrite (VIA_port_b, 1); // PSG Address Latch
        vectrexwrite (VIA_port_a, 0); // Data
        vectrexwrite (VIA_port_b, 0x11);
        vectrexwrite (VIA_port_b, 1); // PSG Data Latch
       }
       
      }
      //Add alternative initialisation states here?
     }
    }
    return result;
}

int vectrexclose (void)
{
    int result = bcm2835_close();
    return result;
}
