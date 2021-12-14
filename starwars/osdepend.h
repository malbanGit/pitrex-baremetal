#ifndef OSDEPEND_H
#define OSDEPEND_H


struct osd_bitmap
{
	int width,height;	/* width and hegiht of the bitmap */
	void *_private;	/* don't touch! - reserved for osdepend use */
	unsigned char *line[0];	/* pointers to the start of each line */
};

   #define OSD_KEY_A             1
   #define OSD_KEY_B             2
   #define OSD_KEY_C             3
   #define OSD_KEY_D             4
   #define OSD_KEY_E             5
   #define OSD_KEY_F             6
   #define OSD_KEY_G             7
   #define OSD_KEY_H             8
   #define OSD_KEY_I             9
   #define OSD_KEY_J             10
   #define OSD_KEY_K             11
   #define OSD_KEY_L             12
   #define OSD_KEY_M             13
   #define OSD_KEY_N             14
   #define OSD_KEY_O             15
   #define OSD_KEY_P             16
   #define OSD_KEY_Q             17
   #define OSD_KEY_R             18
   #define OSD_KEY_S             19
   #define OSD_KEY_T             20
   #define OSD_KEY_U             21
   #define OSD_KEY_V             22
   #define OSD_KEY_W             23
   #define OSD_KEY_X             24
   #define OSD_KEY_Y             25
   #define OSD_KEY_Z             26
   #define OSD_KEY_0             27
   #define OSD_KEY_1             28
   #define OSD_KEY_2             29
   #define OSD_KEY_3             30
   #define OSD_KEY_4             31
   #define OSD_KEY_5             32
   #define OSD_KEY_6             33
   #define OSD_KEY_7             34
   #define OSD_KEY_8             35
   #define OSD_KEY_9             36
   #define OSD_KEY_0_PAD         37
   #define OSD_KEY_1_PAD         38
   #define OSD_KEY_2_PAD         39
   #define OSD_KEY_3_PAD         40
   #define OSD_KEY_4_PAD         41
   #define OSD_KEY_5_PAD         42
   #define OSD_KEY_6_PAD         43
   #define OSD_KEY_7_PAD         44
   #define OSD_KEY_8_PAD         45
   #define OSD_KEY_9_PAD         46
   #define OSD_KEY_F1            47
   #define OSD_KEY_F2            48
   #define OSD_KEY_F3            49
   #define OSD_KEY_F4            50
   #define OSD_KEY_F5            51
   #define OSD_KEY_F6            52
   #define OSD_KEY_F7            53
   #define OSD_KEY_F8            54
   #define OSD_KEY_F9            55
   #define OSD_KEY_F10           56
   #define OSD_KEY_F11           57
   #define OSD_KEY_F12           58
   #define OSD_KEY_ESC           59
   #define OSD_KEY_TILDE         60
   #define OSD_KEY_MINUS         61
   #define OSD_KEY_EQUALS        62
   #define OSD_KEY_BACKSPACE     63
   #define OSD_KEY_TAB           64
   #define OSD_KEY_OPENBRACE     65
   #define OSD_KEY_CLOSEBRACE    66
   #define OSD_KEY_ENTER         67
   #define OSD_KEY_COLON         68
   #define OSD_KEY_QUOTE         69
   #define OSD_KEY_BACKSLASH     70
   #define OSD_KEY_BACKSLASH2    71
   #define OSD_KEY_COMMA         72
   #define OSD_KEY_STOP          73
   #define OSD_KEY_SLASH         74
   #define OSD_KEY_SPACE         75
   #define OSD_KEY_INSERT        76
   #define OSD_KEY_DEL           77
   #define OSD_KEY_HOME          78
   #define OSD_KEY_END           79
   #define OSD_KEY_PGUP          80
   #define OSD_KEY_PGDN          81
   #define OSD_KEY_LEFT          82
   #define OSD_KEY_RIGHT         83
   #define OSD_KEY_UP            84
   #define OSD_KEY_DOWN          85
   #define OSD_KEY_SLASH_PAD     86
   #define OSD_KEY_ASTERISK      87
   #define OSD_KEY_MINUS_PAD     88
   #define OSD_KEY_PLUS_PAD      89
   #define OSD_KEY_DEL_PAD       90
   #define OSD_KEY_ENTER_PAD     91
   #define OSD_KEY_PRTSCR        92
   #define OSD_KEY_PAUSE         93
   #define OSD_KEY_ABNT_C1       94
   #define OSD_KEY_YEN           95
   #define OSD_KEY_KANA          96
   #define OSD_KEY_CONVERT       97
   #define OSD_KEY_NOCONVERT     98
   #define OSD_KEY_AT            99
   #define OSD_KEY_CIRCUMFLEX    100
   #define OSD_KEY_COLON2        101
   #define OSD_KEY_KANJI         102
   #define OSD_KEY_EQUALS_PAD    103  /* MacOS X */
   #define OSD_KEY_BACKQUOTE     104  /* MacOS X */
   #define OSD_KEY_SEMICOLON     105  /* MacOS X */
   #define OSD_KEY_COMMAND       106  /* MacOS X */
   #define OSD_KEY_UNKNOWN1      107
   #define OSD_KEY_UNKNOWN2      108
   #define OSD_KEY_UNKNOWN3      109
   #define OSD_KEY_UNKNOWN4      110
   #define OSD_KEY_UNKNOWN5      111
   #define OSD_KEY_UNKNOWN6      112
   #define OSD_KEY_UNKNOWN7      113
   #define OSD_KEY_UNKNOWN8      114

   #define OSD_KEY_MODIFIERS     115

   #define OSD_KEY_LSHIFT        115
   #define OSD_KEY_RSHIFT        116
   #define OSD_KEY_LCONTROL      117
   #define OSD_KEY_RCONTROL      118
   #define OSD_KEY_ALT           119
   #define OSD_KEY_ALTGR         120
   #define OSD_KEY_LWIN          121
   #define OSD_KEY_RWIN          122
   #define OSD_KEY_MENU          123
   #define OSD_KEY_SCRLOCK       124
   #define OSD_KEY_NUMLOCK       125
   #define OSD_KEY_CAPSLOCK      126

   #define OSD_MAX_KEY           127

#define OSD_JOY_LEFT	1
#define OSD_JOY_RIGHT	2
#define OSD_JOY_UP		3
#define OSD_JOY_DOWN	4
#define OSD_JOY_FIRE1	5
#define OSD_JOY_FIRE2	6
#define OSD_JOY_FIRE3	7
#define OSD_JOY_FIRE4	8
#define OSD_JOY_FIRE	9	/* any of the fire buttons */
#define OSD_MAX_JOY     9

#define X_AXIS          1
#define Y_AXIS          2
#define NO_TRAK         1000000


extern int play_sound;
extern int video_sync;


int osd_init(int argc,char **argv);
void osd_exit(void);
struct osd_bitmap *osd_create_bitmap(int width,int height);
void osd_free_bitmap(struct osd_bitmap *bitmap);
struct osd_bitmap *osd_create_display(int width,int height,int attributes);
void osd_close_display(void);
int osd_obtain_pen(unsigned char red, unsigned char green, unsigned char blue);
void osd_modify_pen(int pen,unsigned char red, unsigned char green, unsigned char blue);
void osd_get_pen(int pen,unsigned char *red, unsigned char *green, unsigned char *blue);
void osd_update_display(void);
void osd_update_audio(void);
void osd_play_sample(int channel,unsigned char *data,int len,int freq,int volume,int loop);
void osd_play_streamed_sample(int channel,unsigned char *data,int len,int freq,int volume);
void osd_adjust_sample(int channel,int freq,int volume);
void osd_stop_sample(int channel);
void osd_restart_sample(int channel);
int osd_get_sample_status(int channel);
void osd_ym2203_write(int n, int r, int v);
void osd_ym2203_update(void);
void osd_set_mastervolume(int volume);
int osd_key_pressed(int keycode);
int osd_read_key(void);
int osd_read_keyrepeat(void);
const char *osd_joy_name(int joycode);
const char *osd_key_name(int keycode);
void osd_poll_joystick(void);
int osd_joy_pressed(int joycode);

int osd_trak_read(int axis);
void osd_trak_center_x(void);
void osd_trak_center_y(void);

void open_page(int *x_res, int *y_res, int step);
void close_page(void);
void draw_to(int x, int y, int col);



/* file handling routines */
#define OSD_FILETYPE_ROM 1
#define OSD_FILETYPE_SAMPLE 2
#define OSD_FILETYPE_HIGHSCORE 3
#define OSD_FILETYPE_CONFIG 4

/* gamename holds the driver name, filename is only used for ROMs and samples. */
/* if 'write' is not 0, the file is opened for write. Otherwise it is opened */
/* for read. */
void *osd_fopen (const char *gamename,const char *filename,int filetype,int write);
int osd_fread (void *file,void *buffer,int length);
int osd_fwrite (void *file,const void *buffer,int length);
int osd_fseek (void *file,int offset,int whence);
void osd_fclose (void *file);

/* control keyboard leds or other indicators */
void osd_led_w(int led,int on);


#endif
