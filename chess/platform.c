
#include "platform.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef PITREX
// Pitrex includes

#include <pitrex/pitrexio-gpio.h>
#include <vectrex/vectrexInterface.h>
#include "window.h"

static int screen_width = 0;
static int screen_height = 0;

#elif PITREX_PLAYGROUND
// Pitrex calls forwarded to playground

int currentButtonState = 0;
int currentJoy1X = 0;
int currentJoy1Y = 0;
static int usePipeline = 1;
static int screen_width = 0;
static int screen_height = 0;

extern void platform_set_size(int width, int height);

int vectrexinit(char viaconfig);
void v_setName(char *name);
void v_init(void);
void v_setRefresh(int hz);
void v_setBrightness(int color) {}
void v_WaitRecal(void);
void v_readButtons(void);
void v_readJoystick1Analog(void);

void v_window(int left, int bottom, int right, int top, boolean clipping) { platform_set_size(right-left, top-bottom); screen_width = right-left; screen_height = top-bottom; }
void v_directDraw32(int x1, int y1, int x2, int y2, int color);
void v_printString(int x, int y, char* str, int textSize, int color);
int  v_printStringRaster(int8_t x, int8_t y, char* str, int8_t xSize, int8_t ySize, unsigned char delimiter);

#else
#error Either PITREX or PITREX_PLAYGROUND must defined
#endif

#define MAX_DAC 32768
#define DAC MAX_DAC/2

static boolean platform_wait = false;
static int platform_wait_count = 0;

// MARK: - Platform

void platform_init(char* name, int width, int height) {
    screen_width = width;
    screen_height = height;
    
    vectrexinit(1);
#ifndef FREESTANDING
    v_setName(name);
#endif    
    v_init();
	usePipeline = 1;
    v_setRefresh(50);
	v_setBrightness(DEFAULT_COLOR);
#ifdef FREESTANDING
  v_setupIRQHandling();
//  v_enableJoystickDigital(1,1,0,0);
  v_enableJoystickAnalog(1,1,0,0);
//  v_enableSoundOut(1);
  v_enableButtons(1);
#else
#endif

    v_window(0, 0, width, height, false);
}

void platform_frame(void) {
    v_WaitRecal();
    v_readButtons();
    v_readJoystick1Analog();

    if (platform_wait) {
        platform_wait_count++;

        if (platform_wait_count >= DEFAULT_INPUT_WAIT_TIME) {
            platform_wait = false;
        }
    }
}

// MARK: - Input handling

void platform_input_wait(void) {
#if PITREX
    platform_wait = true;
    platform_wait_count = 0;
#endif
}

boolean platform_input_is_left(void) {
    if (platform_wait)
        return false;
        
    if (currentJoy1X < -50) {
        // printf("Left\n");
        return true;
    }
    
    return false;
}

boolean platform_input_is_right(void) {
    if (platform_wait)
        return false;

    if (currentJoy1X > 50) {
        // printf("Right\n");
        return true;
    }
    
    return false;
}

boolean platform_input_is_up(void) {
    if (platform_wait)
        return false;

    if (currentJoy1Y > 50) {
        // printf("Up\n");
        return true;
    }
    
    return false;
}

boolean platform_input_is_down(void) {
    if (platform_wait)
        return false;

    if (currentJoy1Y < -50) {
        // printf("Down\n");
        return true;
    }
    
    return false;
}

boolean platform_button_is_pressed(int number) {
    if (platform_wait)
        return false;
    
    switch (number) {
        case 1:
            if ((currentButtonState&0x01) == (0x01))
                return true;
            break;
        case 2:
            if ((currentButtonState&0x02) == (0x02))
                return true;
            break;
        case 3:
            if ((currentButtonState&0x04) == (0x04))
                return true;
            break;
        case 4:
            if ((currentButtonState&0x08) == (0x08))
                return true;
            break;

        default:
            return false;
    }
    
    return false;
}

// MARK: - Drawing

void platform_msg(char* msg, int x, int y, int size, int color) {
    v_printString(x, y, msg, size, color);
}

void platform_raster_msg(char* msg, int x, int y, int size, int color) {
    v_setBrightness(color);
    v_printStringRaster(x, y, msg, size, -7, 0);
}

void platform_draw_line(int x1, int y1, int x2, int y2, int color) {
#ifdef PITREX
    // int xx1 = MAX_DAC * x1 / 400 - DAC;
    // int yy1 = MAX_DAC * y1 / 400 - DAC;
    // int xx2 = MAX_DAC * x2 / 400 - DAC;
    // int yy2 = MAX_DAC * y2 / 400 - DAC;

    // v_directDraw32(xx1, yy1, xx2, yy2, color);
	v_line(x1, y1, x2, y2, color);
#else
    v_directDraw32(x1, y1, x2, y2, color);
#endif
}

void platform_draw_lines(int* points, int count, int color) {
    int index = 2;
    int* offset = points;

    //printf("Draw points: %d (%d)\n", count, color);
    
    if (count < 2)
        return;

    if (count % 2 != 0)
        return;

    int x1 = *offset;
    offset++;
    int y1 = *offset;
    offset++;

    if (count == 2) {
        platform_draw_line(x1, y1, x1, y1, color);
        return;
    }

    while (index < count) {
        int x2 = *offset;
        x2 += x1;
        offset++;
        
        int y2 = *offset;
        y2 += y1;
        offset++;

        // printf(" > %d,%d,%d,%d\n", x1, y1, x2, y2);

        platform_draw_line(x1, y1, x2, y2, color);
        
        x1 = x2;
        y1 = y2;
        
        index += 2;
    }
}
