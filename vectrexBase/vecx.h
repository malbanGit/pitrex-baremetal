#ifndef __VECX_H
#define __VECX_H
#include <vectrexConfig.h>
// todo - 
// check for disturbances with speedy vectrex

#define V_SET(address, val) vectrexwrite(address, val)
#define V_GET(address) vectrexread(address)

enum
{
    VECTREX_MHZ = 1500000, /* speed of the vectrex being emulated */
    VECTREX_COLORS = 128,     /* number of possible colors ... grayscale */

    VECTREX_PAD1_BUTTON1 = 0,
    VECTREX_PAD1_BUTTON2 = 1,
    VECTREX_PAD1_BUTTON3 = 2,
    VECTREX_PAD1_BUTTON4 = 3,
    VECTREX_PAD1_X = 4,
    VECTREX_PAD1_Y = 5,

    VECTREX_PAD2_BUTTON1 = 6,
    VECTREX_PAD2_BUTTON2 = 7,
    VECTREX_PAD2_BUTTON3 = 8,
    VECTREX_PAD2_BUTTON4 = 9,
    VECTREX_PAD2_X = 10,
    VECTREX_PAD2_Y = 11,
};

typedef struct vector_type
{
    int32_t x0, y0; /* start coordinate */
    int32_t x1, y1; /* end coordinate */

                 /* color [0, VECTREX_COLORS - 1], if color = VECTREX_COLORS, then this is
                 * an invalid entry and must be ignored.
                 */
    uint8_t color;
} vector_t;

extern void(*vecx_render) (void);

extern uint8_t rom[8192];
extern uint8_t cart[32768*9]; // lineart
extern uint8_t ram[1024];
#ifdef FILE_PLAYER
extern uint8_t movieRom[1024*1024*20]; // maximum movie size 20 MB
extern int movieLength;
#endif

extern int is64kBankSwitch;
extern int currentBank;

extern uint8_t snd_select;
extern int vectrexButtonState;

extern size_t vector_draw_cnt;
extern vector_t vectors[];

void vecx_input(uint8_t key, uint8_t value);
void vecx_reset(void);
void vecx_direct(int32_t cycles);
void vecx_emulate(int32_t cycles);
void write8_port_a(uint8_t data);
void write8_port_b(uint8_t data);

extern int directEmulation;


extern int viaAccessCounter;
extern uint16_t delayVia[2];
extern uint8_t delayData[2];
extern uint32_t cycleCount;
extern uint32_t mustWait;


extern int doExtraWait;


#endif
