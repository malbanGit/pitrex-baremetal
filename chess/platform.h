
#ifndef platform_h
#define platform_h

// MARK: - Platform types

#ifndef boolean
    #define boolean int
    #define true 1
    #define false 0
#endif

// MARK: - Platform

void platform_init(char* name, int width, int height);
void platform_frame(void);

// MARK: - Input handling

#define BUTTON_ONE      1
#define BUTTON_TWO      2
#define BUTTON_THREE    3
#define BUTTON_FOUR     4

boolean platform_input_is_left(void);
boolean platform_input_is_right(void);
boolean platform_input_is_up(void);
boolean platform_input_is_down(void);
boolean platform_button_is_pressed(int number);
void platform_input_wait(void);

// MARK: - Drawing

#define DEFAULT_COLOR           80
#define HIGHLIGHT_COLOR     	127
#define LOWLIGHT_COLOR          60
#define DEFAULT_TEXT_SIZE 		7
#define DEFAULT_TEXT_SMALL_SIZE 6
#define DEFAULT_INPUT_WAIT_TIME 30

void platform_draw_line(int x1, int y1, int x2, int y2, int color);
void platform_draw_lines(int* points, int count, int color);
void platform_msg(char* msg, int x, int y, int size, int color);
void platform_raster_msg(char* msg, int x, int y, int size, int color);

#endif
