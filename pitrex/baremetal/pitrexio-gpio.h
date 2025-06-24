// PiTrex I/O functions for initialising Raspberry Pi GPIO, and Reading/Writing from/to Vectrex
// Kevin Koster, 2019.
// V. 0.2.1

#ifndef PITREXIO_GPIO_H
#define PITREXIO_GPIO_H



#ifdef USE_PL011_UART
#undef USE_MINI_UART
#else
#define USE_MINI_UART 1
#endif





// PiTrex Discrete Read & Write Functions, via Raspberry Pi GPIO:
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//Return a byte read from the Vectrex at the specified address
volatile unsigned char vectrexread (unsigned int address);

// Write a byte to the Vectrex at the specified address
// Returns after write is completed.
void vectrexwrite (unsigned int address, unsigned char data);


// Initialise GPIO for communication with the Vectrex
// Vectrex can be initialised after the GPIO if viaconfig == 1,
// which sets it to the same initialisation state as the Vectrex BIOS would.
// With viaconfig == 0, the VIA and PSG registers are unchanged.
int vectrexinit(char viaconfig);

// End communication with the Vectrex and end claim on Parallel Port
int vectrexclose (void);

#endif /* PITREXIO_GPIO_H */
