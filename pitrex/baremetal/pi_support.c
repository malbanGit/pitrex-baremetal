// this is a mixture of all different BCM files -> all put into one
// it looks horrible
//
// but imho still better than to seek out every single file in 50 "libs"
// if one does a cleanup - it shouldn't be too bad.
// only thing which comes "extra" is the fat filesystem


#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "pi_support.h"

#include "ff.h"
FATFS fat_fs;       /* File system object */


//static volatile __attribute__ ((aligned (0x4000))) uint32_t memmi[4096];

void vfp_init(void) {
	unsigned cacr;		// Coprocessor Access Control Register
	__asm volatile ("mrc p15, 0, %0, c1, c0, 2" : "=r" (cacr));
	cacr |= 3 << 20;	// bit 20/21, Full Access, CP10
	cacr |= 3 << 22;	// bit 22/23, Full Access, CP11
	__asm volatile ("mcr p15, 0, %0, c1, c0, 2" : : "r" (cacr));
	isb();

#define VFP_FPEXC_EN	(1 << 30)
	__asm volatile ("fmxr fpexc, %0" : : "r" (VFP_FPEXC_EN));
	__asm volatile ("fmxr fpscr, %0" : : "r" (0));
}
void vfp_deinit(void) {
	unsigned cacr;		// Coprocessor Access Control Register
	__asm volatile ("mrc p15, 0, %0, c1, c0, 2" : "=r" (cacr));
	cacr &= ~(3 << 20);	// bit 20/21, Full Access, CP10
	cacr &= ~(3 << 22);	// bit 22/23, Full Access, CP11
	__asm volatile ("mcr p15, 0, %0, c1, c0, 2" : : "r" (cacr));
	isb();
}

// also disables data cache
void mmu_disable(void)
{
   clean_data_cache();
    // disable MMU
    uint32_t control;
    asm volatile ("mrc p15, 0, %0, c1, c0,  0" : "=r" (control));
    control = control & (~MMU_MODE);
    asm volatile ("mcr p15, 0, %0, c1, c0,  0" : : "r" (control) : "memory");
}

// 32 bit
int peek(unsigned int address)
{
  int val;
  __asm__ __volatile__(
      "mov r3, %[value] \n\t"
      "ldr %[result], [r3]\n\t"
      : [result]"=r" (val) : [value]"r" (address): "r3", "cc");
  return val;
}
// 8 bit
unsigned char peekByte(unsigned int address)
{
  unsigned char val;
  __asm__ __volatile__(
    "mov r3, %[value] \n\t"
    "ldrb %[result], [r3]\n\t"
    : [result]"=r" (val) : [value]"r" (address): "r3", "cc");
  return val;
}

// 8 bit
void pokeByte(unsigned int address, unsigned char c)
{
  *((unsigned char *)address) = (unsigned char)c;
}

// 32 bit
void/*__attribute__ ((noinline)) */poke(unsigned int address, unsigned int value)
{
     *((unsigned int *)address) = value;
}

GlobalMemSettings *settings = 0; /* should be called once on startup / reset */
//extern GlobalMemSettings *settings;
char *getLoadParameter(int i)
{
  
  if (settings == 0) 
  {
    settings = (GlobalMemSettings *)SETTING_STORE;
  }
 
  // setting should be a little over LOADER_START
  if ( ((long) settings > LOADER_START) && ((long) settings < LOADER_START + 1500000)) // assuming loader always < 1500kb
    ;//printf("c) settings: %08x\n", settings->loader);
  else
    return 0; // no loader, no parameters present
  switch (i)
  {
    case 0: return settings->parameter1;
    case 1: return settings->parameter2;
    case 2: return settings->parameter3;
    case 3: return settings->parameter4;
  }
  return 0;
}

// for file accesss
char * getErrorText(int errNo)
{
  switch (errNo)
  {
	case FR_OK:				/* (0) Succeeded */
	  return "OK";
	case FR_DISK_ERR:			/* (1) A hard error occurred in the low level disk I/O layer */
	  return "FR_DISK_ERR";
	case FR_INT_ERR:				/* (2) Assertion failed */
	  return "FR_INT_ERR";
	case FR_NOT_READY:			/* (3) The physical drive cannot work */
	  return "FR_NOT_READY";
	case FR_NO_FILE:				/* (4) Could not find the file */
	  return "FR_NO_FILE";
	case FR_NO_PATH:				/* (5) Could not find the path */
	  return "FR_NO_PATH";
	case FR_INVALID_NAME:		/* (6) The path name format is invalid */
	  return "FR_INVALID_NAME";
	case FR_DENIED:				/* (7) Access denied due to prohibited access or directory full */
	  return "FR_DENIED";
	case FR_EXIST:				/* (8) Access denied due to prohibited access */
	  return "FR_EXIST";
	case FR_INVALID_OBJECT:		/* (9) The file/directory object is invalid */
	  return "FR_INVALID_OBJECT";
	case FR_WRITE_PROTECTED:		/* (10) The physical drive is write protected */
	  return "FR_WRITE_PROTECTED";
	case FR_INVALID_DRIVE:		/* (11) The logical drive number is invalid */
	  return "FR_INVALID_DRIVE";
	case FR_NOT_ENABLED:			/* (12) The volume has no work area */
	  return "FR_NOT_ENABLED";
	case FR_NO_FILESYSTEM:		/* (13) There is no valid FAT volume */
	  return "FR_NO_FILESYSTEM";
	case FR_MKFS_ABORTED:		/* (14) The f_mkfs() aborted due to any problem */
	  return "FR_MKFS_ABORTED";
	case FR_TIMEOUT:				/* (15) Could not get a grant to access the volume within defined period */
	  return "FR_TIMEOUT";
	case FR_LOCKED:				/* (16) The operation is rejected according to the file sharing policy */
	  return "FR_LOCKED";
	case FR_NOT_ENOUGH_CORE:		/* (17) LFN working buffer could not be allocated */
	  return "FR_NOT_ENOUGH_CORE";
	case FR_TOO_MANY_OPEN_FILES:	/* (18) Number of open files > _FS_LOCK */
	  return "FR_TOO_MANY_OPEN_FILES";
	case FR_INVALID_PARAMETER:	/* (19) Given parameter is invalid */
	  return "FR_INVALID_PARAMETER";
  }
  char *t = "UNKOWN ERROR:      \r\n";
  sprintf(t, "UNKOWN ERROR: %i", errNo);
  return t;
}


/*****
 * Old file "rpi-gpio.c.h"
 * 
 * 
 ******/

static rpi_gpio_t* rpiGpio = (rpi_gpio_t*)RPI_GPIO_BASE;


rpi_gpio_t* RPI_GetGpio(void)
{
    return rpiGpio;
}


void RPI_SetGpioPinFunction( rpi_gpio_pin_t gpio, rpi_gpio_alt_function_t func )
{
    rpi_reg_rw_t* fsel_reg = &((rpi_reg_rw_t*)rpiGpio)[ gpio / 10 ];
    rpi_reg_rw_t fsel_copy = *fsel_reg;
    fsel_copy &= ~( FS_MASK << ( ( gpio % 10 ) * 3 ) );
    fsel_copy |= (func << ( ( gpio % 10 ) * 3 ) );
    *fsel_reg = fsel_copy;
}



/*****
 * Old file "rpi-aux.c.h"
 * 
 ******/
 

static aux_t* auxillary = (aux_t*)AUX_BASE;


aux_t* RPI_GetAux( void )
{
    return auxillary;
}

static inline void RPI_AuxMiniUartInit( int baud, int bits , int mhz)
{
    volatile int i;
// baud = 115200 ;
    /* As this is a mini uart the configuration is complete! Now just
       enable the uart. Note from the documentation in section 2.1.1 of
       the ARM peripherals manual:

       If the enable bits are clear you will have no access to a
       peripheral. You can not even read or write the registers */
    auxillary->ENABLES = AUX_ENA_MINIUART;

    /* Disable interrupts for now */
    /* auxillary->IRQ &= ~AUX_IRQ_MU; */

    auxillary->MU_IER = 0;

    /* Disable flow control,enable transmitter and receiver! */
    auxillary->MU_CNTL = 0;

    /* Decide between seven or eight-bit mode */
    if( bits == 8 )
        auxillary->MU_LCR = AUX_MULCR_8BIT_MODE;
    else
        auxillary->MU_LCR = 0;

    auxillary->MU_MCR = 0;

    /* Disable all interrupts from MU and clear the fifos */
    auxillary->MU_IER = 0;

    auxillary->MU_IIR = 0xC6;

    /* Transposed calculation from Section 2.2.1 of the ARM peripherals
       manual system_clock_freq  */
    // https://github.com/s-matyukevich/raspberry-pi-os/blob/master/docs/lesson01/rpi-os.md
    auxillary->MU_BAUD = ( mhz / ( 8 * baud ) ) - 1;

     /* Setup GPIO 14 and 15 as alternative function 5 which is
        UART 1 TXD/RXD. These need to be set before enabling the UART */
    RPI_SetGpioPinFunction( RPI_GPIO14, FS_ALT5 );
    RPI_SetGpioPinFunction( RPI_GPIO15, FS_ALT5 );

    RPI_GetGpio()->GPPUD = 0;
    for( i=0; i<150; i++ ) { }
    RPI_GetGpio()->GPPUDCLK0 = ( 1 << 14 );
    for( i=0; i<150; i++ ) { }
    RPI_GetGpio()->GPPUDCLK0 = 0;

    /* Disable flow control,enable transmitter and receiver! */
    auxillary->MU_CNTL = AUX_MUCNTL_TX_ENABLE | AUX_MUCNTL_RX_ENABLE;
}

// not possible != 0
// possible = 0
static inline int RPI_AuxMiniUartWritePossible()
{
  return (( auxillary->MU_LSR & AUX_MULSR_TX_EMPTY ) == 0 );
}

// blocking, till write is possible
static inline void RPI_AuxMiniUartWrite( char c )
{
    /* Wait until the UART has an empty space in the FIFO */
    while (RPI_AuxMiniUartWritePossible()) 
    { 
      ;// wait       
    }

    /* Write the character to the FIFO for transmission */
    auxillary->MU_IO = c;
}

// 0 not pending
// != 0 some data pending on IO to read
static inline int RPI_AuxMiniUartReadPending()
{
    return (auxillary->MU_LSR & AUX_MULSR_DATA_READY);
}

// blocking till char read
static inline char RPI_AuxMiniUartRead()
{
    while (1 == 1) 
    {	
      if (RPI_AuxMiniUartReadPending()) break;
    }
    return auxillary->MU_IO;
}


static inline void RPI_AuxMiniUartFlush()
{
//  while ((auxillary->MU_STAT & AUX_MUSTAT_TX_DONE));
//    while ( ((volatile uint32_t)readRegister(UartRegister::FR)) & (1 << 3) ) ; // bit 3 == Tx Fifo BUSY flag
}

/*
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
*/

#define UART0 ((BCM2835_PL011_TypeDef*)(BCM2835_PL011_BASE))


// bits and hz not used
// bits assumed 8
// hz assumed to be default 48Mhz
static inline void RPI_AuxPL011UartInit( int baud, int bits , int hz)
{
    volatile int i;

    // UART0 has its own clock - it was never changed - so we assume the default here
    hz = 48000000;
    dsb();
    // Disable UART0
    UART0->CR = 0;

    // === GPIO Function Setup ===
    RPI_SetGpioPinFunction(RPI_GPIO14, FS_ALT0);  // TXD0 = ALT0
    RPI_SetGpioPinFunction(RPI_GPIO15, FS_ALT0);  // RXD0 = ALT0

    // Pull-up/down disable for GPIO14+15
    void udelay(uint32_t d);

    RPI_GetGpio()->GPPUD = 0;
    udelay(150);
//    for (i = 0; i < 150; i++) { }
    RPI_GetGpio()->GPPUDCLK0 = (1 << 14) | (1 << 15);
    udelay(150);
//    for (i = 0; i < 150; i++) { }
    RPI_GetGpio()->GPPUDCLK0 = 0;
    udelay(150);

    // === Baud rate divisor calculation ===
    uint64_t scaled_div = ((uint64_t)hz * 64 + baud * 8) / (16 * baud);
    uint32_t ibrd = scaled_div / 64;
    uint32_t fbrd = scaled_div % 64;    
    
    
    UART0->IBRD = ibrd;
    UART0->FBRD = fbrd;

    // 8-bit, FIFO enabled
    UART0->LCRH = (1 << 4) | (1 << 5) | (1 << 6);  // 8N1, FIFO

    // Clear interrupts
    UART0->ICR = 0x7FF;

    // Mask all interrupts.
    UART0->IMSC = (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10);
    
    // Disable flow control explicitly (optional safety)
    UART0->CR &= ~((1 << 14) | (1 << 15));  // CTSEN / RTSEN

    // Enable UART0, TX and RX
    UART0->CR = (1 << 0) | (1 << 8) | (1 << 9); // UARTEN, TXE, RXE
    dmb();
/*
    FILE_OUT("[UART DEBUG] IBRD=%u FBRD=%u\r\n", ibrd, fbrd);

    FILE_OUT("[UART DEBUG] CR=0x%08X LCRH=0x%08X\r\n", UART0->CR, UART0->LCRH);

    uint32_t gpfsel1 = RPI_GetGpio()->GPFSEL1;
    FILE_OUT( "[UART DEBUG] GPFSEL1=0x%08X (TX func=%u RX func=%u)\r\n",
            gpfsel1, (gpfsel1 >> 12) & 7, (gpfsel1 >> 15) & 7);

    FILE_OUT( "[UART DEBUG] UART clock=%u baud=%u\r\n", uartclk_hz, baud);
    FILE_OUT( "[UART DEBUG] UART BASE=$%08x\r\n", UART0);
*/     
    
    
}

// not possible != 0
// possible = 0
static inline int RPI_AuxPL011UartWritePossible()
{
  // Wait until the transmit FIFO is not full (bit 5 = TXFF)
  return (UART0->FR & (1 << 5));
}

// blocking, till write is possible
static inline void RPI_AuxPL011UartWrite( char c )
{
    // Wait until the transmit FIFO is not full (bit 5 = TXFF)
    while (RPI_AuxPL011UartWritePossible()) 
    { 
      ;// wait 
    }

    // Write the character to the data register
    UART0->DR = c;
}

// 0 not pending
// != 0 some data pending on IO to read
static inline int RPI_AuxPL011UartReadPending()
{
    // If RXFE (bit 4) is 0, data is available to read
    return (UART0->FR & (1 << 4)) == 0;
  
}

// blocking till char read
static inline char RPI_AuxPL011UartRead()
{
    // Wait until the receive FIFO is not empty (bit 4 = RXFE)
    while (!RPI_AuxPL011UartReadPending()) 
    {
        // wait
    }

    // Read and return the received character
    return (char)(UART0->DR & 0xFF);
}


static inline void RPI_AuxPL011UartFlush()
{
/*  
    // Wait until the transmit FIFO is empty and UART is idle
    while (!(UART0->FR & (1 << 7))) {
        // Wait for TXFE (bit 7) = 1: Transmit FIFO empty
    }

    // Wait until the transmission has fully completed
    while (UART0->FR & (1 << 3)) {
        // Wait for BUSY (bit 3) = 0: UART not transmitting
    }
*/  
  
}

void RPI_AuxUartInit( int baud, int bits , int hz)
{
#ifdef USE_PL011_UART
  RPI_AuxPL011UartInit(baud, bits, hz);
#else
  ddöledldS
  RPI_AuxMiniUartInit(baud, bits, hz);
#endif
}

// not possible != 0
// possible = 0
int RPI_Aux_UartWritePossible()
{
#ifdef USE_PL011_UART
  return RPI_AuxPL011UartWritePossible();
#else
  return RPI_AuxMiniUartWritePossible();
#endif
  
}

// blocking, till write is possible
void RPI_AuxUartWrite( char c )
{
#ifdef USE_PL011_UART
  RPI_AuxPL011UartWrite(c);
#else
  RPI_AuxMiniUartWrite(c);
#endif
}

// 0 not pending
// != 0 some data pending on IO to read
int RPI_AuxUartReadPending()
{
#ifdef USE_PL011_UART
  return RPI_AuxPL011UartReadPending();
#else
  return RPI_AuxMiniUartReadPending();
#endif
}
char RPI_AuxUartRead()
{
#ifdef USE_PL011_UART
  return RPI_AuxPL011UartRead();
#else
  return RPI_AuxMiniUartRead();
#endif
}
void RPI_AuxUartFlush()
{
#ifdef USE_PL011_UART
  RPI_AuxPL011UartFlush();
#else
  RPI_AuxMiniUartFlush();
#endif
}


/*
idle = 1 (transmit done
AUX_MU_STAT_REG

9 Transmitter done This bi
*/



/*****
 * Old file "Vectors.h"
 * 
 ******/

/*
 * Names are identical, so that this 'plays nicely with' linux
 * documentation and other associated code.
 */

// see: http://www.ethernut.de/en/documents/arm-exceptions.html

#define PTRACE_R0_idx        0
#define PTRACE_R1_idx        1
#define PTRACE_R2_idx        2
#define PTRACE_R3_idx        3
#define PTRACE_R4_idx        4
#define PTRACE_R5_idx        5
#define PTRACE_R6_idx        6
#define PTRACE_R7_idx        7
#define PTRACE_R8_idx        8
#define PTRACE_R9_idx        9
#define PTRACE_R10_idx       10
#define PTRACE_R11_idx       11
#define PTRACE_R12_idx       12
#define PTRACE_R13_idx       13
#define PTRACE_SP_idx        PTRACE_R13_idx
#define PTRACE_R14_idx       14
#define PTRACE_LR_idx        PTRACE_R14_idx
#define PTRACE_R15_idx       15
#define PTRACE_PC_idx        PTRACE_R15_idx
#define PTRACE_CPSR_idx      16
#define PTRACE_R0_retval_idx 17
#define PTRACE_FRAMETYPE_idx 18
#define PTRACE_SWICODE_idx   19
#define PTRACE_FRAME_size    20 /* in bytes */

#define ARM_CPSR_T_BIT   (0x20)
#define ARM_CPSR_F_BIT   (0x40)
#define ARM_CPSR_I_BIT   (0x80)
#define ARM_CPSR_V_BIT   (1<<28)
#define ARM_CPSR_C_BIT   (1<<29)
#define ARM_CPSR_Z_BIT   (1<<30)
#define ARM_CPSR_N_BIT   (1<<31)
#define ARM_USR_MODE     (0x10) /**< no priv mode */
#define ARM_FIQ_MODE     (0x11) /**< fiq occured */
#define ARM_IRQ_MODE     (0x12) /**< irq occured */
#define ARM_SVC_MODE     (0x13) /**< swi occured */
#define ARM_ABT_MODE     (0x17) /**< pfa or udf  */
#define ARM_UND_MODE     (0x1b) /**< bad opcode  */
#define ARM_SYS_MODE     (0x1f) /**< normal supervisor */
#define ARM_MODE_MASK    (0x1f)




#define PTRACE_FRAME_isirq(X)   ( (X)->uregs[ PTRACE_FRAMETYPE ] < 32 )
#define PTRACE_FRAMETYPE_reset   32 /* in vector order */
#define PTRACE_FRAMETYPE_udf     33
#define PTRACE_FRAMETYPE_swi     34
#define PTRACE_FRAMETYPE_pfa     35
#define PTRACE_FRAMETYPE_da      36
#define PTRACE_FRAMETYPE_notused 37
/* #define PTRACE_FRAMETYPE_irq is above, ie: 0..32 for each irq vector */
#define PTRACE_FRAMETYPE_fiq     38
#define PTRACE_FRAMETYPE_thread  39 /* thread saved state */

struct pt_regs {
  unsigned int uregs[ 20 ];
};

void ARM_UDF_Handler( struct pt_regs  *p );
void ARM_SWI_Handler( struct pt_regs  *p );
void ARM_PFA_Handler( struct pt_regs  *p );
void ARM_DA_Handler ( struct pt_regs  *p );
void ARM_IRQ_Handler( struct pt_regs  *p );
void ARM_FIQ_Handler( struct pt_regs  *p );
void ARM_NOTUSED_Handler( struct pt_regs  *p );
void ARM_COMMON_Handler_crash( struct pt_regs  *p, const char *name );







//void ptrace_dump_regs( struct pt_regs *p );
//void ptrace_stackdump_from( const int *frame_ptr );
//void ptrace_stackdump_regs( struct pt_regs *p );
//void ptrace_stackdump_here( void );
//void print_data(void) __attribute__ ((naked));

#if RASPPI != 1
extern unsigned int systemConfigControl;
#else
unsigned int systemConfigControl;
#endif
unsigned int r14 = 0;   // 32 bit mode - pointer to "old" stack frame

void print_reset()
{
    printf( "Reset EXCEPTION...\r\n" );
    uint32_t progSpace = 0x8000;
    void (*progStart)(void) = (void (*)(void))progSpace;
    progStart();
}
void print_halt()
{
  exception_out(4, "Halt Exception");
  for(;;); 
}
void print_undefined()
{
  exception_out(4, "Undefined instruction Exception");
  for(;;); 
}
void print_prefetch()
{

  // todo
  /*
  u32 FSR = 0, FAR = 0;
  asm volatile ("mrc p15, 0, %0, c5, c0,  0" : "=r" (FSR));
  asm volatile ("mrc p15, 0, %0, c6, c0,  0" : "=r" (FAR));
  */

  exception_out(4, "PREFETCH ABORT Exception");
  for(;;); 
}

// alignemnt info: DDI0487G_b_armv8_arm.pdf 4312
void print_data()
{
//  asm __volatile__ ("push {r0-r12, lr}\n"  );   
  exception_out(8, "DATA ABORT Exception");
//  asm __volatile__ ("pop {r0-r12, lr}\n"  );   
//  asm __volatile__ ("subs pc, lr, #8\n"  );   
  for(;;); 
  // to return:
  // Just push {r0-r12, lr} at the start and pop {r0-r12, lr}, subs pc, lr, #4 at the end. The subs instruction will restore the CPSR for you.
}

void print_hang()
{
  exception_out(4, "HANG Exception");
  for(;;); 
}
void print_irq()
{
  exception_out(4, "IRQ Exception");
  for(;;); 
}
void print_fiq()
{
  exception_out(4, "FIQ Exception");
  for(;;); 
}
void print_swi()
{
  exception_out(4, "SWI Exception");
  for(;;); 
}
void print_unused()
{
  exception_out(4, "Unused Exception");
  for(;;); 
}

void tweakVectors()
{
  pi_vectors *pv ;
  // circumvent gcc UDF
  asm("ldr %[result],= 0\n\t" : [result]"=r" (pv) :: "cc");

  pv->_reset_pointer = (unsigned int)print_reset;
  pv->_undefined_pointer = (unsigned int)print_undefined;
  pv->_swi_pointer = (unsigned int)print_swi;
  pv->_prefetched_pointer = (unsigned int)print_prefetch;
  pv->_data_pointer = (unsigned int)print_data;
  pv->_unused_pointer = (unsigned int)print_unused;
  pv->_irq_pointer = (unsigned int)print_irq;
  pv->_fiq_pointer = (unsigned int)print_fiq;
  
}





/*****
 * Old file "rpi-systimer.c"
 * 
 ******/


rpi_arm_timer_t* rpiArmTimer = (rpi_arm_timer_t*)RPI_ARMTIMER_BASE;
rpi_sys_timer_t* rpiSystemTimer = (rpi_sys_timer_t*)RPI_SYSTIMER_BASE;
rpi_irq_controller_t* rpiIRQController = (rpi_irq_controller_t*)RPI_INTERRUPT_CONTROLLER_BASE;


rpi_sys_timer_t* RPI_GetSystemTimer(void)
{
    return rpiSystemTimer;
}

void RPI_WaitMicroSeconds( uint32_t us )
{
    volatile uint32_t ts = rpiSystemTimer->counter_lo;

    while( ( rpiSystemTimer->counter_lo - ts ) < us )
    {
        /* BLANK */
    }
}


/**
    @brief Return the IRQ Controller register set
*/
rpi_irq_controller_t* RPI_GetIrqController( void )
{
    return rpiIRQController;
}

void RPI_EnableARMTimerInterrupt(void)
{
    RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;
}

unsigned long long getOtherTimer()
{
  unsigned long long v;
  v = BCM2835_ST->CHI;
  v = v<<32;
  v = v+BCM2835_ST->CLO;
  return v;
}


void udelay(uint32_t d) 
{
	const uint32_t clo = BCM2835_ST->CLO;
	while ((BCM2835_ST->CLO - clo) < d) 
	{
	}
}

#define BCM2835_MAILBOX_STATUS_WF	0x80000000	///< Write full
#define	BCM2835_MAILBOX_STATUS_RE	0x40000000	///< Read empty

void lib_bcm2835_mailbox_flush(void) 
{
  while (!(BCM2835_MAILBOX->STATUS & BCM2835_MAILBOX_STATUS_RE)) 
  {
    (void) BCM2835_MAILBOX->READ;
    udelay(20);
  }
}

uint32_t lib_bcm2835_mailbox_read(uint8_t channel) 
{
    uint32_t data;

    do 
    {
	while (BCM2835_MAILBOX->STATUS & BCM2835_MAILBOX_STATUS_RE)
		; // do nothing

	data = BCM2835_MAILBOX->READ;

    } while ((uint8_t) (data & 0xf) != channel);

    return (data & ~0xf);
}

void lib_bcm2835_mailbox_write(uint8_t channel, uint32_t data) 
{
	while (BCM2835_MAILBOX->STATUS & BCM2835_MAILBOX_STATUS_WF)
		; // do nothing
	BCM2835_MAILBOX->WRITE = (data & ~0xf) | (uint32_t) (channel & 0xf);
}

// these "random" waits look like I don't know what I am doing
// that is RIGHT - I don't
// but without them the communication sometimes fails!
uint32_t lib_bcm2835_mailbox_write_read(uint8_t channel, uint32_t data) 
{
#if RASPPI != 1
    clean_data_cache(); 
#endif
//    udelay(20);
    dsb();
#if RASPPI != 1
    clean_data_cache(); 
#endif
//    udelay(20);
    lib_bcm2835_mailbox_flush();
    udelay(20);
    lib_bcm2835_mailbox_write(channel, data);
    udelay(20);
    uint32_t address = lib_bcm2835_mailbox_read(channel);
//    udelay(20);
#if RASPPI != 1
	dmb();
	invalidate_data_cache();
#endif
//    udelay(20);

    dmb();
    return address;
}

/*
unsigned BcmMailBoxWriteRead (TBcmMailBox *pThis, unsigned nData)
{
	assert (pThis != 0);

	DataMemBarrier ();

	BcmMailBoxFlush (pThis);

	BcmMailBoxWrite (pThis, nData);

	unsigned nResult = BcmMailBoxRead (pThis);

	DataMemBarrier ();

	return nResult;
}
*/


struct vc_msg_tag_uint32 
{
	uint32_t tag_id;				///< the message id
	uint32_t buffer_size;			///< size of the buffer (which in this case is always 8 bytes)
	uint32_t data_size;				///< amount of data being sent or received
	uint32_t dev_id;				///< the ID of the clock/voltage to get or set
	uint32_t val;					///< the value (e.g. rate (in Hz)) to set
};

struct vc_msg_uint32 
{
	uint32_t msg_size;				///< simply, sizeof(struct vc_msg)
	uint32_t request_code;			///< holds various information like the success and number of bytes returned (refer to mailboxes wiki)
	struct vc_msg_tag_uint32 tag;	///< the tag structure above to make
	uint32_t end_tag;				///< an end identifier, should be set to NULL
};

inline static int32_t lib_bcm2835_vc_get(uint32_t tag_id, uint32_t dev_id) 
{
	struct vc_msg_uint32 *vc_msg = (struct vc_msg_uint32 *)MEM_COHERENT_REGION;
	
	vc_msg->msg_size = sizeof(struct vc_msg_uint32);
	vc_msg->request_code = 0;
	vc_msg->tag.tag_id = tag_id;
	vc_msg->tag.buffer_size = 8;
	vc_msg->tag.data_size = 4;
	vc_msg->tag.dev_id = dev_id;
	vc_msg->tag.val = 0;
	vc_msg->end_tag = 0;

	(void) lib_bcm2835_mailbox_write_read(BCM2835_MAILBOX_PROP_CHANNEL, (uint32_t) vc_msg);

	if (vc_msg->request_code != BCM2835_MAILBOX_SUCCESS) {
		return -1;
	}

	if (vc_msg->tag.dev_id != dev_id) {
		return -1;
	}

	return (int32_t) vc_msg->tag.val;

}

inline static int32_t lib_bcm2835_vc_set(uint32_t tag_id, uint32_t dev_id, uint32_t val) 
{
	struct vc_msg_uint32 *vc_msg = (struct vc_msg_uint32 *)MEM_COHERENT_REGION;

	vc_msg->msg_size = sizeof(struct vc_msg_uint32);
	vc_msg->request_code = 0;
	vc_msg->tag.tag_id = tag_id;
	vc_msg->tag.buffer_size = 8;
	vc_msg->tag.data_size = 8;
	vc_msg->tag.dev_id = dev_id;
	vc_msg->tag.val = val;
	vc_msg->end_tag = 0;

//	(void) lib_bcm2835_mailbox_write_read(BCM2835_MAILBOX_PROP_CHANNEL, GPU_MEM_BASE + (uint32_t) vc_msg);
	(void) lib_bcm2835_mailbox_write_read(BCM2835_MAILBOX_PROP_CHANNEL, (uint32_t) vc_msg);

	if (vc_msg->request_code != BCM2835_MAILBOX_SUCCESS) {
		return -1;
	}

	if (vc_msg->tag.dev_id != dev_id) {
		return -1;
	}

	return (int32_t) vc_msg->tag.val;
}

int32_t lib_bcm2835_vc_get_clock_rate(uint32_t clock_id) 
{
	return lib_bcm2835_vc_get(BCM2835_VC_TAG_GET_CLOCK_RATE, clock_id);
}


struct vc_msg_tag_set_clock_rate 
{
	uint32_t tag_id;				///< the message id
	uint32_t buffer_size;			///< size of the buffer (which in this case is always 8 bytes)
	uint32_t data_size;				///< amount of data being sent or received
	uint32_t dev_id;				///< the ID of the clock
	uint32_t val;					///< the value rate (in Hz) to set
	uint32_t skip_turbo;			///<
};

struct vc_msg_set_clock_rate 
{
	uint32_t msg_size;				///< simply, sizeof(struct vc_msg)
	uint32_t request_code;			///< holds various information like the success and number of bytes returned (refer to mailboxes wiki)
	struct vc_msg_tag_set_clock_rate tag;	///< the tag structure above to make
	uint32_t end_tag;				///< an end identifier, should be set to NULL
};

int32_t lib_bcm2835_vc_set_clock_rate(uint32_t clock_id, uint32_t clock_rate) 
{
	struct vc_msg_set_clock_rate *vc_msg = (struct vc_msg_set_clock_rate *)MEM_COHERENT_REGION;

	vc_msg->msg_size = sizeof(struct vc_msg_uint32);
	vc_msg->request_code = 0;
	vc_msg->tag.tag_id = BCM2835_VC_TAG_SET_CLOCK_RATE;
	vc_msg->tag.buffer_size = 12;
	vc_msg->tag.data_size = 12;
	vc_msg->tag.dev_id = clock_id;
	vc_msg->tag.val = clock_rate;
	vc_msg->tag.skip_turbo = 0;
	vc_msg->end_tag = 0;

//	(void) lib_bcm2835_mailbox_write_read(BCM2835_MAILBOX_PROP_CHANNEL, GPU_MEM_BASE + (uint32_t) vc_msg);
	(void) lib_bcm2835_mailbox_write_read(BCM2835_MAILBOX_PROP_CHANNEL, (uint32_t) vc_msg);

	if (vc_msg->request_code != BCM2835_MAILBOX_SUCCESS) {
		return -1;
	}

	if (vc_msg->tag.dev_id != clock_id) {
		return -1;
	}

	return (int32_t) vc_msg->tag.val;
}

struct vc_msg_tag_ram {
	uint32_t tag_id;			///< the message id
	uint32_t buffer_size;		///< size of the buffer (which in this case is always 8 bytes)
	uint32_t data_size;			///< amount of data being sent or received
	uint32_t base_address;		///< base address in bytes
	uint32_t size;				///< size in bytes
};

struct vc_msg_board_ram {
	uint32_t msg_size;			///< simply, sizeof(struct vc_msg)
	uint32_t request_code;		///< holds various information like the success and number of bytes returned (refer to mailboxes wiki)
	struct vc_msg_tag_ram tag;	///< the tag structure above to make
	uint32_t end_tag;			///< an end identifier, should be set to NULL
};

int32_t lib_bcm2835_vc_get_memory(uint32_t tag_id) {
	struct vc_msg_board_ram *vc_msg = (struct vc_msg_board_ram *)MEM_COHERENT_REGION;

	if ((tag_id != BCM2835_VC_TAG_GET_ARM_MEMORY) && (tag_id != BCM2835_VC_TAG_GET_VC_MEMORY)) {
		return -1;
	}

	vc_msg->msg_size = sizeof(struct vc_msg_board_ram);
	vc_msg->request_code = 0;
	vc_msg->tag.tag_id = tag_id;
	vc_msg->tag.buffer_size = 8;
	vc_msg->tag.data_size = 0;
	vc_msg->tag.base_address = 0;
	vc_msg->end_tag = 0;

//	(void) lib_bcm2835_mailbox_write_read(BCM2835_MAILBOX_PROP_CHANNEL, GPU_MEM_BASE + (uint32_t) vc_msg);
	(void) lib_bcm2835_mailbox_write_read(BCM2835_MAILBOX_PROP_CHANNEL, (uint32_t) vc_msg);

	if (vc_msg->request_code != BCM2835_MAILBOX_SUCCESS) {
		return -1;
	}

	return (int32_t) vc_msg->tag.size;
}


/**
 * @param dev_id
 *
 * @return
 *   Bit 0: 0=off, 1=on
 *   Bit 1: 0=device exists, 1=device does not exist
 *   Bits 2-31: reserved for future use
 */
int32_t lib_bcm2835_vc_get_power_state(uint32_t dev_id) {
	return (lib_bcm2835_vc_get(BCM2835_VC_TAG_GET_POWER_STATE, dev_id) & 0x3);
}

/**
 * @param dev_id
 *
 * @param state
 *  Bit 0: 0=off, 1=on
 *  Bit 1: 0=do not wait, 1=wait
 *  Bits 2-31: reserved for future use (set to 0)
 *
 * @return
 *  Bit 0: 0=off, 1=on
 *  Bit 1: 0=device exists, 1=device does not exist
 *  Bits 2-31: reserved for future use
 *
 */
int32_t lib_bcm2835_vc_set_power_state(uint32_t dev_id, uint32_t state) 
{
    return lib_bcm2835_vc_set(BCM2835_VC_TAG_SET_POWER_STATE, dev_id, state);
}
// dummy
void __attribute__((weak)) __attribute__((interrupt("IRQ"))) c_irq_handler(void) {}
void __attribute__((weak)) __attribute__((interrupt("FIQ"))) c_fiq_handler(void) {}

#define MEGABYTE		0x100000

// Access permissions (AP[11:10])
#define AP_NO_ACCESS		0	// 00
#define AP_SYSTEM_ACCESS	1	// 01
#define AP_USER_RO_ACCESS	2	// 10
#define AP_ALL_ACCESS		3	// 11

// Access permissions extended (APX)
#define APX_RW_ACCESS		0
#define APX_RO_ACCESS		1

// Domains
#define DOMAIN_NO_ACCESS	0
#define DOMAIN_CLIENT		1
#define DOMAIN_MANAGER		3


//  Auxiliary Control register
#if defined (RPI1)
#define ARM_AUX_CONTROL_CACHE_SIZE	(1 << 6)	// restrict cache size to 16K (no page coloring)
#else
#define ARM_AUX_CONTROL_SMP		(1 << 6)
#endif

#define ARM_TTBR_INNER_NON_CACHEABLE	((0 << 6) | (0 << 0))
#define ARM_TTBR_INNER_WRITE_ALLOCATE	((1 << 6) | (0 << 0))
#define ARM_TTBR_INNER_WRITE_THROUGH	((0 << 6) | (1 << 0))
#define ARM_TTBR_INNER_WRITE_BACK	((1 << 6) | (1 << 0))

#define ARM_TTBR_OUTER_NON_CACHEABLE	(0 << 3)
#define ARM_TTBR_OUTER_WRITE_ALLOCATE	(1 << 3)
#define ARM_TTBR_OUTER_WRITE_THROUGH	(2 << 3)
#define ARM_TTBR_OUTER_WRITE_BACK	(3 << 3)

#define TTBR_MODE	(ARM_TTBR_INNER_WRITE_BACK | ARM_TTBR_OUTER_WRITE_BACK)


static volatile uint32_t arm_ram = 0;

uint32_t mmu_get_arm_ram(void) 
{
    return arm_ram;
}

uint32_t * page_table=(uint32_t *)0x4000;
const uint32_t *mmu_get_page_table(void) 
{
    return (uint32_t *) page_table;
}

/*
An Alignment fault can occur on an access for which the stage of address translation is disabled.
Any unaligned access to memory region with any Device memory type attribute generates an Alignment fault


 Device memory
The Device memory type attributes define memory locations where an access to the location can cause side-effects, 
or where the value returned for a load can vary depending on the number of loads performed. Typically, the Device 
memory attributes are used for memory-mapped peripherals and similar locations.
The attributes for Armv8 Device memory are:

Gathering Identified as G or nG, see Gathering on page E2-4324.

Reordering Identified as R or nR, see Reordering on page E2-4325.

Early Write Acknowledgement 
Identified as E or nE, see Early Write Acknowledgement on page E2-4326.
The Armv8 Device memory types are:
Device-nGnRnE Device non-Gathering, non-Reordering, No Early Write Acknowledgement.
Equivalent to the Strongly-ordered memory type in earlier versions of the architecture.
Device-nGnRE Device non-Gathering, non-Reordering, Early Write Acknowledgement.
Equivalent to the Device memory type in earlier versions of the architecture.
Device-nGRE Device non-Gathering, Reordering, Early Write Acknowledgement.
Armv8 adds this memory type to the translation table formats found in earlier versions of 
the architecture. The use of barriers is required to order accesses to Device-nGRE memory.
The Device-nGRE memory type is introduced into the AArch32 translation table formats 
when the PE is using the Long Descriptor Translation Table format.
Device-GRE Device Gathering, Reordering, Early Write Acknowledgement.
The AArch32 Application Level Memory Model 
E2.8 Memory types and attributes
ARM DDI 0487G.b Copyright © 2013-2021 Arm Limited or its affiliates. All rights reserved. E2-4323
ID072021 Non-Confidential
Armv8 adds this memory type to the translation table formats found in earlier versions of 
the architecture. Device-GRE memory has the fewest constraints. It behaves similar to 
Normal memory, with the restriction that speculative accesses to Device-GRE memory is 
forbidden.
The Device-GRE memory type is introduced into the AArch32 translation table formats 
when the PE is using the Long Descriptor Translation Table format.
*/


void pageSetup()
{
/*    
/////////////////////////////////////////////////////////////// 
// for savety - disable everything before    
/////////////////////////////////////////////////////////////// 
    clean_data_cache();
    dmb();
    isb();

#ifndef ARM_ALLOW_MULTI_CORE
    invalidate_data_cache();
#else
    invalidate_data_cache_l1_only();
#endif
    invalidate_instruction_cache();
    flush_branch_target_cache();
    dsb();
    flush_prefetch_buffer();
  
    // disable mmu 
    asm volatile ("mrc p15, 0, %0, c1, c0,  0" : "=r" (nControl));
    nControl = nControl & (~MMU_MODE);
    asm volatile ("mcr p15, 0, %0, c1, c0,  0" : : "r" (nControl) : "memory");
        
        
    clean_data_cache();
    dmb();
    isb();

#ifndef ARM_ALLOW_MULTI_CORE
    invalidate_data_cache();
#else
    invalidate_data_cache_l1_only();
#endif
    invalidate_instruction_cache();
    flush_branch_target_cache();
    dsb();
    flush_prefetch_buffer();
///////////////////////////////////////////////////////////////  
*/  
  
  
  
  
  
  
  
  
  
    uint32_t entry;
    uint32_t entries;
    arm_ram = lib_bcm2835_vc_get_memory(BCM2835_VC_TAG_GET_ARM_MEMORY) / 1024 / 1024;	///< MB
    unsigned int gpu_ram = lib_bcm2835_vc_get_memory(BCM2835_VC_TAG_GET_VC_MEMORY) / 1024 / 1024;	///< MB
    
    
printf("Pi ARM memory: %iMB\n", arm_ram);
printf("Pi GPU memory: %iMB\n", gpu_ram);
printf("Page table at: %p\n", page_table);


// file:///C:/Users/salom/Downloads/DDI0487G_b_armv8_arm.pdf page 6284 ff
// 6281
// [1:0] 	1 PXN section (Privileged execute-never field)
// [2]		B
// [3]		C
// [4]		XN / SBZ (0 = execute allowed, execute Never bit)
// [8:5]	Domain
// [9]		IM
// [11:10]	AP Access Permissions
// [12:14]	TEX
// [15]		APX Access Permissions (extended?)
// [16]		S    Shareable bit
// [17]		NG  (not global)
// [19:18]	0 0 (19 - non secure bit 0 = non secure, 18 Section/Supersection)
// [31:20]	base address

// access permission:
// AP[2] AP[1:0] PL1 access Unprivileged access Description
// 0 00 No access No access All accesses generate Permission faults
// 0 01 Read/write No access Access only at PL1 
// 0 10 Read/write Read-only Writes at EL0 generate Permission faults
// 0 11 Read/write Read/write Full access
// ... page 6311



#define MT_DEVICE_NS  0x10412					//  device no share (strongly ordered)
#define MT_DEVICE     0x10416        // S      AP    	 CB10	//  device + shareable
#define MT_NORMAL     0x1040E	//	1 0000 0100 0000 1110	// S=1+(AP = 01) B=1+C=1+10 normal cache + shareable BC == 11(and TEX = 000) = Outer and Inner Write-Back, Read-Allocate No Write-Allocate
#define MT_NORMAL_NS  0x1040A					//  normal cache no share
#define MT_NORMAL_XN  0x1041E					//  normal cache + shareable and execute never


/*
0010 device
1110 normal
CB
0x10412

0x1041a
0x10416
*/
// normal memory DDI0487G_b_armv8_arm.pdf 4318
// Device memory DDI0487G_b_armv8_arm.pdf 4322add

// table descriptoin DDI0487G_b_armv8_arm.pdf 6281 ff
    
//1)    
// everything is unused    
    // unused rest of address space)
    for (entry = 0; entry < 4096; entry++) 
    {
	*(page_table+entry) = 0;
    }

// 2.    
// ARM RAM
    for (entry = 0; entry < arm_ram; entry++) 
    {

#if RASPPI == 1
//      #ifndef ARM_ALLOW_MULTI_CORE	// S = 0                                       S      AP        CB10
							///< 31   27   23   19   15   11   7    3
							///<   28   24   20   16   12    8    4    0
	*(page_table+entry) = entry << 20 | 0x0040E;	///< 0000 0000 0000 0000 0000 0100 0000 1110
#else							
							///< 31   27   23   19   15   11   7    3
							///<   28   24   20   16   12    8    4    0
	*(page_table+entry) = entry << 20 | 0x1040E;	///< 0000 0000 0000 0001 0000 0100 0000 1110
							//  		       S = 1, needed for spin locks // SHARE flag
#endif
//printf("page entry adr (%i): %p", entry, &(*(page_table+entry)));
    }
    

// 3.    
// GPU RAM
    for (; entry < gpu_ram+ arm_ram; entry++) 
    {
	// shared device, never execute
							///< 31   27   23   19   15   11   7    3
							///<   28   24   20   16   12    8    4    0
	*(page_table+entry) = entry << 20 | 0x10416;	///< 0000 0000 0000 0001 0000 0100 0001 0110
    }
    
    
//4)    
// Peri
    
    //B = 0, C = 0, TEX = 0, S = 0
    entry = BCM2835_PERI_BASE / MEGABYTE;
    entries = entry + (0x1000000 / MEGABYTE);

    for (; entry < entries; entry++) 
    {
								///< 31   27   23   19   15   11   7    3
								///<   28   24   20   16   12    8    4    0
	    *(page_table+entry) = entry << 20 | 0x00412;	///< 0000 0000 0000 0000 0000 0100 0001 0010
//	    *(page_table+entry) = entry << 20 | 0x00012;	///< 0000 0000 0000 0000 0000 0000 0001 0010
    }
    
    
//5 MEM COHERENT    
    //B = 0, C = 0, TEX = 0, S = 1
    entry = MEM_COHERENT_REGION / MEGABYTE ;
    
								///< 31   27   23   19   15   11   7    3
								///<   28   24   20   16   12    8    4    0
   *(page_table+entry) = entry << 20 | 0x10412;		///< 0000 0000 0000 0001 0000 0100 0001 0010


#if RASPPI == 1
#else							
// global exection
// GLOBAL_CODE_AREA MEMORY!!!
    entry = GLOBAL_CODE_AREA / MEGABYTE ;
	*(page_table+entry) = entry << 20 | 0x1040E;	
#endif   
   
// irq stack   
// DEVICE MEMORY!!!
// ATTENTION memory alignment is enabled!
// NO! Stack usage in IRQ
    entry = STACK_LOWEST_IRQ / MEGABYTE ;
   *(page_table+entry) = entry << 20 | 0x10412 /**/;		///< 0000 0000 0000 0001 0000 0100 0001 0010
   

// svc stack   
// normal memory, 
// not shared
// not executable
//6 Stacks area
    entry = STACK_LOWEST / MEGABYTE ;
   *(page_table+entry) = entry << 20 | 0x0041E /**/;		///< 0000 0000 0000 0001 0000 0100 0001 0010

   
   
    // 1 MB mailboxes at 0x40000000 Multi-core
    // shared device, never execute
							///< 31   27   23   19   15   11   7    3
//7 MEM mailboxes ???    
    //B = 0, C = 0, TEX = 0, S = 1
    entry = 0x40000000 / MEGABYTE ;
				///<   28   24   20   16   12    8    4    0
    *(page_table+entry) = entry << 20 | 0x10416;	///< 0000 0000 0000 0001 0000 0100 0001 0110

    
// good write up_: https://www.snaums.de/informatik/mmu-on-raspberry-pi.html    
    
    
    // 1 MB mailboxes at 0x40000000 Multi-core
    // shared device, never execute
							///< 31   27   23   19   15   11   7    3
//
//8 MEM mailboxes ???    
    //B = 0, C = 0, TEX = 0, S = 1
    entry = CORE1_NON_SHAREABLE / MEGABYTE ;
				///<   28   24   20   16   12    8    4    0
//    *(page_table+entry) = entry << 20 | (0x0041E);	///< 0000 0000 0000 0000 0100 0001 1110
    *(page_table+entry) = entry << 20 | (0x00416);	///< 0000 0000 0000 0000 0100 0001 0110
//    int entryhigh = (0x20000000 +CORE1_NON_SHAREABLE) / MEGABYTE ;
//    *(page_table+entryhigh) = entry << 20 | 0x0041E;	///< 0000 0000 0000 0000 0100 0001 1110
// virtual address (0x20000000 +CORE1_NON_SHAREABLE) points to -> (CORE1_NON_SHAREABLE)
// I had hopes, that the high address would use another BUS type, but if so - there is still no difference
    

    
    
    /*
int cs = 0;
for (int i=0; i<4096;i++)
    cs += *(page_table+i);
printf("\nPageTableCS: %i\n", cs);
*/
printf("Page table end at: %p\n", page_table+4096);
      
}

#if RASPPI == 1

void mmu_enable(void) {
  pageSetup();
  	clean_data_cache();
	dmb();

	uint32_t auxctrl;
	asm volatile ("mrc p15, 0, %0, c1, c0,  1" : "=r" (auxctrl));
	auxctrl |= ARM_AUX_CONTROL_CACHE_SIZE;
	asm volatile ("mcr p15, 0, %0, c1, c0,  1" :: "r" (auxctrl));

	// set domain 0 to client
	asm volatile ("mcr p15, 0, %0, c3, c0, 0" :: "r" (DOMAIN_CLIENT << 0));

	// always use TTBR0
	asm volatile ("mcr p15, 0, %0, c2, c0, 2" :: "r" (0));

	// set TTBR0
	asm volatile ("mcr p15, 0, %0, c2, c0, 0" :: "r" (TTBR_MODE | (uint32_t) 0x4000));

	isb();

	invalidate_data_cache();

	// required if MMU was previously enabled and not properly reset
	invalidate_instruction_cache();
	flush_branch_target_cache();
	asm volatile ("mcr p15, 0, %0, c8, c7,  0" : : "r" (0)); // invalidate unified TLB
	dsb();
	flush_prefetch_buffer();

	// enable MMU
	uint32_t control;
	asm volatile ("mrc p15, 0, %0, c1, c0,  0" : "=r" (control));
	control |= MMU_MODE;
// done by assembler part
//	control &= (~2); 	// A (no unaligned access fault)
//	control |= (1 << 22); 	// U (v6 unaligned access model)
	asm volatile ("mcr p15, 0, %0, c1, c0,  0" : : "r" (control) : "memory");
}
	
#else
void mmu_enable_only()
{
    uint32_t nControl;
    clean_data_cache();
    dmb();

    uint32_t auxctrl;
    asm volatile ("mrc p15, 0, %0, c1, c0,  1" : "=r" (auxctrl));
    auxctrl |= ARM_AUX_CONTROL_SMP;
    asm volatile ("mcr p15, 0, %0, c1, c0,  1" :: "r" (auxctrl));

    // set domain 0 to client - trying manger
    asm volatile ("mcr p15, 0, %0, c3, c0, 0" :: "r" (DOMAIN_CLIENT << 0));
//    asm volatile ("mcr p15, 0, %0, c3, c0, 0" :: "r" (DOMAIN_MANAGER << 0));

    // always use TTBR0
    asm volatile ("mcr p15, 0, %0, c2, c0, 2" :: "r" (0));

    // set TTBR0
    asm volatile ("mcr p15, 0, %0, c2, c0, 0" :: "r" (TTBR_MODE | (uint32_t) 0x4000));

    isb();

    invalidate_data_cache_l1_only();

    // required if MMU was previously enabled and not properly reset
    invalidate_instruction_cache();
    flush_branch_target_cache();
    asm volatile ("mcr p15, 0, %0, c8, c7,  0" : : "r" (0)); // invalidate unified TLB
    dsb();
    flush_prefetch_buffer();

    // enable MMU
    asm volatile ("mrc p15, 0, %0, c1, c0,  0" : "=r" (nControl));
  #ifdef ARM_STRICT_ALIGNMENT
    nControl |= ARM_CONTROL_STRICT_ALIGNMENT;
  #else
    nControl &= ~ARM_CONTROL_STRICT_ALIGNMENT;
  #endif
    
    nControl |= (MMU_MODE); // 4 is data cache
    asm volatile ("mcr p15, 0, %0, c1, c0,  0" : : "r" (nControl) : "memory");
    systemConfigControl = nControl;    
    
    
// ALIGNMENT done by MMU    
// https://developer.arm.com/documentation/ddi0500/j/System-Control/AArch32-register-descriptions/System-Control-Register?lang=en    
//    // file:///C:/Users/salom/Downloads/DDI0487G_b_armv8_arm.pdf, page 6816
//    @ enable unaliogned memory access. Library mmu_enable doesn't do that!
//    mrc	p15, 0, r0, c1, c0, 0	@ read SCTLR
//    bic	r0, r0, #2		@ A (no unaligned access fault)
//    orr	r0, r0, #1 << 22	@ U (v6 unaligned access model)
//    mcr	p15, 0, r0, c1, c0, 0	@ write SCTLR	

    
// CACHE done by MMU    
    // from https://github.com/LdB-ECM/Raspberry-Pi/blob/master/Arm32_64_USB/SmartStart32.S
    //=========================================================================
    //                              Enable L1 cache
    //=========================================================================
//.equ SCTLR_ENABLE_DATA_CACHE,		0x4
//.equ SCTLR_ENABLE_BRANCH_PREDICTION,	0x800 // RES 1 on ARM v8
//.equ SCTLR_ENABLE_INSTRUCTION_CACHE,	0x1000
//    mrc p15,0,r0,c1,c0,0								;@ R0 = System Control Register

    /* Enable caches and branch prediction */
//    orr r0, #SCTLR_ENABLE_BRANCH_PREDICTION				;@ Branch prediction on
//    orr r0, #SCTLR_ENABLE_DATA_CACHE					;@ Data cache on
//    orr r0, #SCTLR_ENABLE_INSTRUCTION_CACHE				;@ Instruction cache on

//    mcr p15,0,r0,c1,c0,0								;@ System Control Register = R0
	
//    // https://developer.arm.com/documentation/ddi0360/e/control-coprocessor-cp15/register-descriptions/c1--control-register?lang=en	
//    ldr r11, =systemConfigControl
//    str r0, [r11]
  
}
#define CORES 4
unsigned thisCore (void)            // returns number of current core (0..CORES-1)
{
    unsigned int nMPIDR;
    asm volatile ("mrc p15, 0, %0, c0, c0, 5" : "=r" (nMPIDR));
    return nMPIDR & (CORES-1);
}

extern unsigned int multicore_start;
void mmu_enable(void) 
{
    pageSetup();
    mmu_enable_only();

//    printf("mmu_enable() called with core: %i\n", thisCore());


    // setup some more multi core stuff
    // Route IRQ and FIQ to core 0
    
    // only core0 should come here!
    
    unsigned int nRouting = read32 (ARM_LOCAL_GPU_INT_ROUTING);
    nRouting &= ~0x0F;
    write32 (ARM_LOCAL_GPU_INT_ROUTING, nRouting);

    write32 (ARM_LOCAL_MAILBOX_INT_CONTROL0, 1);		// enable IPI on core 0
    
    // wake up core 1
    for (unsigned nCore = 1; nCore < 2; nCore++)
    {
      if (nCore == 1)
      {
//        printf("Core %u state: %i\n", nCore, *((unsigned int *)CORE1_STATE));
        if (*((unsigned int *)CORE1_STATE) != CORE_UNKOWN) continue;
      }
      if (nCore == 2)
      {
//        printf("Core %u state: %i\n", nCore, *((unsigned int *)CORE2_STATE));
        if (*((unsigned int *)CORE2_STATE) != CORE_UNKOWN) continue;
      }
      if (nCore == 3)
      {
//        printf("Core %u state: %i\n", nCore, *((unsigned int *)CORE3_STATE));
        if (*((unsigned int *)CORE3_STATE) != CORE_UNKOWN) continue;
      }

      unsigned int nMailBoxClear = ARM_LOCAL_MAILBOX3_CLR0 + 0x10 * nCore;

	  DataSyncBarrier ();

      unsigned nTimeout = 100;
      while (read32 (nMailBoxClear) != 0)
      {
          if (--nTimeout == 0)
          {
              printf("CPU core %u does not respond\n", nCore);
              return;
          }
          MsDelay (1);
      }
      printf("CPU core %u responded\n", nCore);
      
      write32 (ARM_LOCAL_MAILBOX3_SET0 + 0x10 * nCore, (unsigned int) &multicore_start);
      nTimeout = 500;
      do
      {
          asm volatile ("sev");

          if (--nTimeout == 0)
          {
              printf("CPU core %u did not start\n", nCore);
              return;
          }

          MsDelay (1);
          DataMemBarrier ();
      }
      while (read32 (nMailBoxClear) != 0);
      printf("CPU core %u started\n", nCore);
    }
}

void recognizeCore()
{
    int core = thisCore();
    if (core == 1)
      if (*((unsigned int *)CORE1_STATE) != CORE_GRABBED) return;
    if (core == 2)
      if (*((unsigned int *)CORE2_STATE) != CORE_GRABBED) return;
    if (core == 3)
      if (*((unsigned int *)CORE3_STATE) != CORE_GRABBED) return;
      
    vfp_init();
    mmu_enable_only();
    DisableInterrupts();
    write32 (ARM_LOCAL_MAILBOX3_CLR0 + 0x10*core, 0);
    write32 (ARM_LOCAL_MAILBOX_INT_CONTROL0 + 4*core, 1);
    printf("Core %i initialized\n", core);
}

void startCore1(unsigned int coreStart)
{
  if (*((unsigned int *)CORE1_STATE) != CORE_PARKING) return;
  
  
  unsigned int nCore = 1;
  
	unsigned int nMailBoxClear = ARM_LOCAL_MAILBOX3_CLR0 + 0x10 * nCore;

	DataSyncBarrier ();

	unsigned nTimeout = 100;
	while (read32 (nMailBoxClear) != 0)
	{
	    if (--nTimeout == 0)
	    {
		    printf("CPU core %u does not respond\n", nCore);
		    return;
	    }

	    MsDelay (1);
	}
	printf("CPU core %u responded\n", nCore);

	write32 (ARM_LOCAL_MAILBOX3_SET0 + 0x10 * nCore, (unsigned int) coreStart);

	nTimeout = 500;
	do
	{
	    asm volatile ("sev");

	    if (--nTimeout == 0)
	    {
		    printf("CPU core %u did not start\n", nCore);
		    return;
	    }

	    MsDelay (1);
	    DataMemBarrier ();
	}
	while (read32 (nMailBoxClear) != 0);
	printf("CPU core %u started\n", nCore);
  
} 

void __attribute__ ((noreturn)) __attribute__ ((naked)) parkCore1() 
{
  
    printf("Core parked!\n");
    void * (*parking1)() = (void * (*)()) CORE1_PARK;
    parking1();

  // cleanup stack???
  // branch to GLOBAL part routine
  // asm volatile ("\tb CORE1_PARK\n");
}
#endif

/*
PiTrex starting...
Raspberry Pi Zero 2
Serial: Mini UART 921600 Baud 8n1
Speed arm: 1000000000, core: 250000000
BSS start: 36174, end: 36734
Pi ARM memory: 448MB
Pi GPU memory: 64MB
Page table at: 0x4000
Page table end at: 0x8000
CPU core 1 responded
CPU core 1 started
Core 1 initialized
FAT filesystem found!

*/

#define _BAUD_ 921600

void firstInit()
{
    tweakVectors();
    lib_bcm2835_vc_set_clock_rate(BCM2835_VC_CLOCK_ID_ARM, 1000000000);
    int32_t arm_clock = lib_bcm2835_vc_get_clock_rate(BCM2835_VC_CLOCK_ID_ARM);
    int32_t core_clock = lib_bcm2835_vc_get_clock_rate(BCM2835_VC_CLOCK_ID_CORE);
    RPI_AuxUartInit( _BAUD_, 8, core_clock);
/*
    RPI_AuxPL011UartInit( _BAUD_, 8, core_clock);
    RPI_AuxPL011UartWrite('H'); 
    RPI_AuxPL011UartWrite('H'); 
    RPI_AuxPL011UartWrite('H'); 
    RPI_AuxPL011UartWrite('H'); 
    RPI_AuxPL011UartWrite('H'); 
    RPI_AuxPL011UartWrite('\n');
*/    
    
    /* Print to the UART using the standard libc functions */
    printf("PiTrex starting...\r\n" );
    
#if RASPPI != 1
    printf("Raspberry Pi Zero 2\r\n" );
#else
    printf("Raspberry Pi Zero 1\r\n" );
#endif    
#ifdef USE_PL011_UART
    printf("Serial: UART (PL011) %i Baud 8n1\r\n", _BAUD_);
#else
    printf("Serial: Mini UART _BAUD_ Baud 8n1\r\n", _BAUD_);
#endif


    printf("Speed arm: %i, core: %i\r\n", arm_clock, core_clock);
    printf("BSS start: %X, end: %X\r\n", &__bss_start__, &__bss_end__);
}


bool i2c_is_connected(uint8_t address) 
{
	uint8_t ret;
	char buf;

	i2c_set_address(address);

	if ((address >= 0x30 && address <= 0x37) || (address >= 0x50 && address <= 0x5F)) {
		ret = FUNC_PREFIX(i2c_read(&buf, 1));
	} else {
		/* This is known to corrupt the Atmel AT24RF08 EEPROM */
		ret = FUNC_PREFIX(i2c_write(NULL, 0));
	}

	return (ret == 0) ? true : false;
}


struct device_details 
{
	const uint8_t slave_address;
	const char *name;
} 

static const devices[] __attribute__((aligned(4))) = {
		{ 0x18, "MCP9808 {Temperature Sensor}" },
		{ 0x20, "MCP23017 {16-bit I/O port}"},
		{ 0x23, "BH1750FVI {Ambient Light Sensor}" },
		{ 0x27, "PCF8574T {8-bit I/O Expander}" },
		{ 0x3C, "SSD1306 {128 x 64 Dot Matrix}" },
		{ 0x40, "HTU21D {Humidity and Temperature Sensor} | INA219 {Current Sensor} | PCA9685 {16-channel, 12-bit PWM}" },
		{ 0x41, "BW:LCD" },
		{ 0x42, "BW:7 digital IO lines (DIO)" },
		{ 0x43, "BW:Servo" },
		{ 0x44, "BW:7 FETs" },
		{ 0x45, "BW:3 FETs" },
		{ 0x46, "BW:Temp" },
		{ 0x47, "BW:Relay" },
		{ 0x48, "PCF8591 {8-bit A/D and D/A} | ADS1x1x {A/D} | BW:Motor" },
		{ 0x4A, "BW:User Interface (UI)" },
		{ 0x4B, "BW:7 segment" },
		{ 0x4D, "BW:Pushbutton" },
		{ 0x4E, "BW:Bigrelay" },
		{ 0x4F, "BW:Dimmer" },
		{ 0x52, "BW:Raspberry juice" },
		{ 0x57, "MCP7941X {EEPROM}" },
		{ 0x6F, "MCP7941X {SRAM RTCC}" },
		{ 0x70, "TCA9548A {I2C Multiplexer} | PCA9685 {16-channel, 12-bit PWM}"}
		};

/*@observer@*/const char *i2c_lookup_device(uint8_t slave_address) {
	int i = 0;
	int j = (int) (sizeof(devices) / sizeof(struct device_details));

	for (i = 0; i < j; i++) {
		if (devices[i].slave_address == slave_address) {
			return devices[i].name;
		}
	}

	return "Unknown device";
}




uint8_t i2c_read_uint8(void) {
	uint8_t buf[1] = { 0 };

	(void) FUNC_PREFIX(i2c_read((char *) buf, (uint32_t) 1));

	return buf[0];
}

uint16_t i2c_read_uint16(void) {
	uint8_t buf[2] = { 0, 0 };

	(void) FUNC_PREFIX(i2c_read((char *) buf, (uint32_t) 2));

	return (uint16_t) ((uint16_t) buf[0] << 8 | (uint16_t) buf[1]);
}

uint16_t i2c_read_reg_uint16(uint8_t reg) {
	uint8_t buf[2] = { 0, 0 };

	buf[0] = reg;

	(void) FUNC_PREFIX(i2c_write((char *) &buf[0], (uint32_t) 1));

	return i2c_read_uint16();
}

uint16_t i2c_read_reg_uint16_delayus(uint8_t reg, uint32_t delayus) {
	uint8_t buf[2] = { 0, 0 };

	buf[0] = reg;

	(void) FUNC_PREFIX(i2c_write((char *) &buf[0], (uint32_t) 1));

	udelay(delayus);

	return i2c_read_uint16();
}


void i2c_write(uint8_t data) {
	BCM2835_BSC1->C = BCM2835_BSC_C_CLEAR_1;
	BCM2835_BSC1->S = (uint32_t) (BCM2835_BSC_S_CLKT | BCM2835_BSC_S_ERR | BCM2835_BSC_S_DONE);

	BCM2835_BSC1->DLEN = (uint32_t) 1;
	BCM2835_BSC1->FIFO = (uint32_t) data;

	BCM2835_BSC1->C = (uint32_t) (BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST);

	while ((BCM2835_BSC1->S & BCM2835_BSC_S_DONE) != BCM2835_BSC_S_DONE) {
	}

	BCM2835_BSC1->S = BCM2835_BSC_S_DONE;
}

void i2c_write_uint16(uint16_t data) {
	BCM2835_BSC1->C = BCM2835_BSC_C_CLEAR_1;
	BCM2835_BSC1->S = (uint32_t) (BCM2835_BSC_S_CLKT | BCM2835_BSC_S_ERR | BCM2835_BSC_S_DONE);

	BCM2835_BSC1->DLEN = (uint32_t) 2;
	BCM2835_BSC1->FIFO = (uint32_t) data >> 8;
	BCM2835_BSC1->FIFO = (uint32_t) data & 0xFF;

	BCM2835_BSC1->C = (uint32_t) (BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST);

	while ((BCM2835_BSC1->S & BCM2835_BSC_S_DONE) != BCM2835_BSC_S_DONE) {
	}

	BCM2835_BSC1->S = BCM2835_BSC_S_DONE;
}

void i2c_write_reg_uint8(uint8_t reg, uint8_t data) {
	BCM2835_BSC1->C = BCM2835_BSC_C_CLEAR_1;
	BCM2835_BSC1->S = (uint32_t) (BCM2835_BSC_S_CLKT | BCM2835_BSC_S_ERR | BCM2835_BSC_S_DONE);

	BCM2835_BSC1->DLEN = (uint32_t) 2;
	BCM2835_BSC1->FIFO = (uint32_t) reg;
	BCM2835_BSC1->FIFO = (uint32_t) data;

	BCM2835_BSC1->C = (uint32_t) (BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST);

	while ((BCM2835_BSC1->S & BCM2835_BSC_S_DONE) != BCM2835_BSC_S_DONE) {
	}

	BCM2835_BSC1->S = BCM2835_BSC_S_DONE;
}

void i2c_write_reg_uint16(uint8_t reg, uint16_t data) {
	BCM2835_BSC1->C = BCM2835_BSC_C_CLEAR_1;
	BCM2835_BSC1->S = (uint32_t) (BCM2835_BSC_S_CLKT | BCM2835_BSC_S_ERR | BCM2835_BSC_S_DONE);

	BCM2835_BSC1->DLEN = (uint32_t) 3;
	BCM2835_BSC1->FIFO = (uint32_t) reg;
	BCM2835_BSC1->FIFO = (uint32_t) data >> 8;
	BCM2835_BSC1->FIFO = (uint32_t) data & 0xFF;

	BCM2835_BSC1->C = (uint32_t) (BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST);

	while ((BCM2835_BSC1->S & BCM2835_BSC_S_DONE) != BCM2835_BSC_S_DONE) {
	}

	BCM2835_BSC1->S = BCM2835_BSC_S_DONE;
}

void i2c_write_reg_uint16_mask(uint8_t reg, uint16_t data, uint16_t mask) {
	uint16_t current;
	uint16_t new;

	current = i2c_read_reg_uint16(reg);

	new = (current & ~mask) | (data & mask);

	i2c_write_reg_uint16(reg, new);
}

/**
 * @ingroup I2C
 *
 * Start I2C operations.
 * Forces BSC1 pins P1-3 (SDA), P1-5 (SCL)
 * alternate function ALT0, which enables those pins for I2C interface.
 * Default the I2C speed to 100 kHz.
 */
void lib_bcm2835_i2c_begin(void) {
	/* BSC1 is on GPIO 2 & 3 */
	lib_bcm2835_gpio_fsel(RPI_V2_GPIO_P1_03, BCM2835_GPIO_FSEL_ALT0);
	lib_bcm2835_gpio_fsel(RPI_V2_GPIO_P1_05, BCM2835_GPIO_FSEL_ALT0);

	BCM2835_BSC1->DIV = BCM2835_I2C_CLOCK_DIVIDER_2500; // Default the I2C speed to 100 kHz
}

/**
 * @ingroup I2C
 *
 * End I2C operations.
 * BSC1 pins pins P1-3 (SDA), P1-5 (SCL)
 * are returned to their default INPUT behavior.
 */
void lib_bcm2835_i2c_end(void) {
	/* BSC1 is on GPIO 2 & 3 */
	lib_bcm2835_gpio_fsel(RPI_V2_GPIO_P1_03, BCM2835_GPIO_FSEL_INPT);
	lib_bcm2835_gpio_fsel(RPI_V2_GPIO_P1_05, BCM2835_GPIO_FSEL_INPT);
}

/**
 * @ingroup I2C
 *
 * Write a data to I2C device.
 * @param buf buffer to write
 * @param len size of the buffer
 * @return ::BCM2835_I2C_REASON_OK if successful; BCM2835_I2C_REASON_ERROR_* otherwise. Reference \ref bcm2835I2CReasonCodes
 */
uint8_t lib_bcm2835_i2c_write(/*@null@*/ const char *buf, uint32_t len) 
{
	uint32_t remaining = len;
	uint32_t i = 0;
	uint8_t reason = BCM2835_I2C_REASON_OK;

    // Clear FIFO
    BCM2835_BSC1->C = BCM2835_BSC_C_CLEAR_1;
    // Clear Status
    BCM2835_BSC1->S = (uint32_t)(BCM2835_BSC_S_CLKT | BCM2835_BSC_S_ERR | BCM2835_BSC_S_DONE);
	// Set Data Length
    BCM2835_BSC1->DLEN = len;
    // pre populate FIFO with max buffer
	while ((remaining != (uint32_t)0)  && (i < BCM2835_BSC_FIFO_SIZE)) {
		BCM2835_BSC1 ->FIFO = (uint32_t)buf[i];
		i++;
		remaining--;
	}

    // Enable device and start transfer
    BCM2835_BSC1->C = (uint32_t)(BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST);

	// Transfer is over when BCM2835_BSC_S_DONE
	while (!(BCM2835_BSC1 ->S & BCM2835_BSC_S_DONE)) {
		while ((remaining != (uint32_t)0) && (BCM2835_BSC1 ->S & BCM2835_BSC_S_TXD)) {
			BCM2835_BSC1 ->FIFO = (uint32_t)buf[i];
			i++;
			remaining--;
    	}
    }

	// Received a NACK
	if (BCM2835_BSC1 ->S & BCM2835_BSC_S_ERR) {
		BCM2835_BSC1 ->S = BCM2835_BSC_S_ERR;
		reason = BCM2835_I2C_REASON_ERROR_NACK;
	}

	// Received Clock Stretch Timeout
	else if (BCM2835_BSC1 ->S & BCM2835_BSC_S_CLKT) {
		reason = BCM2835_I2C_REASON_ERROR_CLKT;
	}

	// Not all data is sent
	else if (remaining != (uint32_t)0) {
		reason = BCM2835_I2C_REASON_ERROR_DATA;
	}

	BCM2835_BSC1->S = BCM2835_BSC_S_DONE;

    return reason;
}

/**
 * @ingroup I2C
 *
 * Read data from I2C device.
 * @param buf buffer for read
 * @param len size of the buffer
 * @return ::BCM2835_I2C_REASON_OK if successful; BCM2835_I2C_REASON_ERROR_* otherwise. Reference \ref bcm2835I2CReasonCodes
 */
uint8_t lib_bcm2835_i2c_read(char *buf, uint32_t len) 
{
	uint32_t remaining = len;
	uint8_t reason = BCM2835_I2C_REASON_OK;
	uint8_t *p = (uint8_t *)buf;

    // Clear FIFO
    BCM2835_BSC1->C = BCM2835_BSC_C_CLEAR_1;
    // Clear Status
    BCM2835_BSC1->S = (uint32_t)(BCM2835_BSC_S_CLKT | BCM2835_BSC_S_ERR | BCM2835_BSC_S_DONE);
	// Set Data Length
    BCM2835_BSC1->DLEN = len;
    // Start read
    BCM2835_BSC1->C = (uint32_t)(BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST | BCM2835_BSC_C_READ);

	// wait for transfer to complete
	while (!(BCM2835_BSC1->S & BCM2835_BSC_S_DONE)) {
		// we must empty the FIFO as it is populated and not use any delay
		while (BCM2835_BSC1->S & BCM2835_BSC_S_RXD) {
			*p++ = (uint8_t) (BCM2835_BSC1->FIFO & 0xFF);
			remaining--;
		}
	}

	// transfer has finished - grab any remaining stuff in FIFO
	while ((remaining != (uint32_t)0) && (BCM2835_BSC1 ->S & BCM2835_BSC_S_RXD)) {
		*p++ = (uint8_t) (BCM2835_BSC1->FIFO & 0xFF);
		remaining--;
	}

	// Received a NACK
	if (BCM2835_BSC1 ->S & BCM2835_BSC_S_ERR) {
		BCM2835_BSC1 ->S = BCM2835_BSC_S_ERR;
		reason = BCM2835_I2C_REASON_ERROR_NACK;
	}

	// Received Clock Stretch Timeout
	else if (BCM2835_BSC1 ->S & BCM2835_BSC_S_CLKT) {
		reason = BCM2835_I2C_REASON_ERROR_CLKT;
	}

	// Not all data is received
	else if (remaining != (uint32_t)0) {
		reason = BCM2835_I2C_REASON_ERROR_DATA;
	}

	BCM2835_BSC1->S = BCM2835_BSC_S_DONE;

    return reason;
}

void lib_bcm2835_i2c_set_baudrate(uint32_t baudrate) 
{
	uint32_t divider = ((uint32_t)BCM2835_CORE_CLK_HZ / baudrate);
	lib_bcm2835_i2c_setClockDivider((uint16_t) divider);
}




static volatile unsigned s_nCriticalLevel = 0;
static volatile int s_bWereEnabled;

void EnterCritical (void)
{
    uint32_t nFlags;
    __asm volatile ("mrs %0, cpsr" : "=r" (nFlags));

    DisableInterrupts ();

    if (s_nCriticalLevel++ == 0)
    {
        s_bWereEnabled = nFlags & 0x80 ? 0 : 1;
    }

    DataMemBarrier ();
}

void LeaveCritical (void)
{
    DataMemBarrier ();
    if (--s_nCriticalLevel == 0)
    {
        if (s_bWereEnabled)
        {
            EnableInterrupts ();
        }
    }
}


void cache_flush(void)
{
  InvalidateInstructionCache();
  FlushPrefetchBuffer();
  FlushBranchTargetCache();
  InvalidateDataCache();
  CleanDataCache();
}
#if RASPPI != 1

unsigned int m_nLocked = 0;
void getLock()
{
  // See: ARMv7-A Architecture Reference Manual, Section D7.3
  asm volatile
  (
	  "mov r1, %0\n"
	  "mov r2, #1\n"
	  "1: ldrex r3, [r1]\n"
	  "cmp r3, #0\n"
	  "strexeq r3, r2, [r1]\n"
	  "cmpeq r3, #0\n"
	  "bne 1b\n"
	  "dmb\n"

	  : : "r" ((unsigned int) &m_nLocked) : "r1", "r2", "r3"
  );
}
void releaseLock()
{
  // See: ARMv7-A Architecture Reference Manual, Section D7.3
  asm volatile
  (
	  "mov r1, %0\n"
	  "mov r2, #0\n"
	  "dmb\n"
	  "str r2, [r1]\n"

	  : : "r" ((unsigned int) &m_nLocked) : "r1", "r2"
  );
}



unsigned int m_nLocked2 = 0;
void getLock2()
{
  // See: ARMv7-A Architecture Reference Manual, Section D7.3
  asm volatile
  (
	  "mov r1, %0\n"
	  "mov r2, #1\n"
	  "1: ldrex r3, [r1]\n"
	  "cmp r3, #0\n"
	  "strexeq r3, r2, [r1]\n"
	  "cmpeq r3, #0\n"
	  "bne 1b\n"
	  "dmb\n"

	  : : "r" ((unsigned int) &m_nLocked2) : "r1", "r2", "r3"
  );
}
void releaseLock2()
{
  // See: ARMv7-A Architecture Reference Manual, Section D7.3
  asm volatile
  (
	  "mov r1, %0\n"
	  "mov r2, #0\n"
	  "dmb\n"
	  "str r2, [r1]\n"

	  : : "r" ((unsigned int) &m_nLocked2) : "r1", "r2"
  );
}
#endif



