#ifndef OMEGRACE_H
#define OMEGRACE_H

#include "aaemain.h"

static unsigned char spinnerTable[64] =
{
	0x00, 0x04, 0x14, 0x10, 0x18, 0x1c, 0x5c, 0x58,
	0x50, 0x54, 0x44, 0x40, 0x48, 0x4c, 0x6c, 0x68,
	0x60, 0x64, 0x74, 0x70, 0x78, 0x7c, 0xfc, 0xf8,
	0xf0, 0xf4, 0xe4, 0xe0, 0xe8, 0xec, 0xcc, 0xc8,
	0xc0, 0xc4, 0xd4, 0xd0, 0xd8, 0xdc, 0x9c, 0x98,
	0x90, 0x94, 0x84, 0x80, 0x88, 0x8c, 0xac, 0xa8,
	0xa0, 0xa4, 0xb4, 0xb0, 0xb8, 0xbc, 0x3c, 0x38,
	0x30, 0x34, 0x24, 0x20, 0x28, 0x2c, 0x0c, 0x08
};


int init_omega(void);
void run_omega();
void end_omega();


/*
UINT16 BWVG(UINT16 port, struct z80PortRead *pPR);
UINT16 omegrace_vg_status_r (UINT16 port, struct z80PortRead *pPR);

void VectorRam(UINT32 address, UINT8 data, struct MemoryWriteByte *psMemWrite);
static void omegrace_leds_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
static void Odvg_generate_vector_list(void);
UINT16 omegrace_watchdog_r (UINT16 port, struct z80PortRead *pPR);
UINT16 input_port_2_r (UINT16 port, struct z80PortRead *pPR);
UINT16 input_port_3_r (UINT16 port, struct z80PortRead *pPR);
void SoundWrite(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
void Oavgdvg_reset_w(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
UINT16 omegrace_spinner1_r (UINT16 port, struct z80PortRead *pPR);
UINT16 input_port_0_r (UINT16 port, struct z80PortRead *pPR);
UINT16 input_port_1_r (UINT16 port, struct z80PortRead *pPR);
UINT16 input_port_5_r (UINT16 port, struct z80PortRead *pPR);
void save_nvram(void);
int load_nvram(void);
void set_omega_colors(void);
*/
/*

UINT16 OmegaVectorGeneratorInternal(UINT16 port, struct z80PortRead *pPR);
UINT16 omegrace_vg_status_r (UINT16 port, struct z80PortRead *pPR);
void reset_all(void);
void draw_omega_screen(void);
void set_omega_colors(void);
static void o_dvg_generate_vector_list(void);
UINT16 omegrace_watchdog_r (UINT16 port, struct z80PortRead *pPR);
UINT16 input_port_2_r (UINT16 port, struct z80PortRead *pPR);
UINT16 input_port_3_r (UINT16 port, struct z80PortRead *pPR);
void SoundWrite(UINT16 port, UINT8 data, struct z80PortWrite *pPW);
UINT16 omegrace_spinner1_r (UINT16 port, struct z80PortRead *pPR);
UINT16 input_port_0_r (UINT16 port, struct z80PortRead *pPR);
UINT16 input_port_1_r (UINT16 port, struct z80PortRead *pPR);
*/


#endif