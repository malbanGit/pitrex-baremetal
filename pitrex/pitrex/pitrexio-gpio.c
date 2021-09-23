


// PiTrex I/O functions for initialising Raspberry Pi GPIO, and Reading/Writing from/to Vectrex
// Kevin Koster, 2019.
// V. 0.2.1

// PiTrex Discrete Read & Write Functions

#include <unistd.h>
#include <stdio.h>

#include "bcm2835.h"
#include "pitrexio-gpio.h"
#include "via6522.h"

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


static inline uint32_t _bcm2835_peri_read(volatile uint32_t* paddr)
{
    uint32_t ret;
//    __sync_synchronize();
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
static inline uint8_t _bcm2835_gpio_lev(uint8_t pin)
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

// Read a byte from the Vectrex at the specified address
unsigned char vectrexread (unsigned int address)
{
    unsigned long shortaddress = (0x30 | (address & 0x0F)) << 7;
    _bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPCLR0/4), 0xFFFFFFFF);    //Set all outputs Low

    //Set Data bus to Input mode:
    INIT_VIA_READ1;

    _bcm2835_gpio_set (13); // R/#W High
    // Set Address Bus
    _bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPSET0/4), shortaddress); // Set Highs

    char status;
/*
    do
    {
     status = _bcm2835_gpio_lev (24);
    }
    while (!status); // Loop until RDY goes High
*/    
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

    if (result)
    {
     //Set Initial State (ideally set to configured defaults at power-on):
     //NOTE: This will disable other functions of these GPIO pins (except for the serial terminal on the UART)
      //Input/Output Modes:
     bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPFSEL0/4), 0x9240000);    // GPIO Pins 0-5   = INPUT | 6-9   = OUTPUT
     bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPFSEL1/4), 0x24249);
     bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPFSEL2/4), 0x8000);       // GPIO Pins 20-24 = INPUT | 25    = OUTPUT | 26-29 = INPUT

     //Output Levels:
     bcm2835_peri_write ((bcm2835_gpio + BCM2835_GPCLR0/4), 0xFFFFDFFF);    //Set all outputs Low except R/#W
     bcm2835_gpio_set_pud (27, BCM2835_GPIO_PUD_UP);  // Enable Pull-Up on #HALT
     bcm2835_gpio_set_pud (24, BCM2835_GPIO_PUD_OFF); // Disable PU/D on RDY
     bcm2835_gpio_set (13); // R/#W High

     //Check that Vectrex CPU is halted.
     if (bcm2835_gpio_lev (27))
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
