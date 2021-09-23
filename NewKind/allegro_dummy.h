
int install_sound(int digi, int midi, const char *cfg_path);
void * load_sample(const char *filename);
int play_sample(void *spl, int vol, int pan, int freq, int loop);
void stop_sample(void *spl);
void destroy_sample(void *spl);
void *load_midi(const char *filename);
int play_midi(void *midi, int loop);
void stop_midi(void);
void xor_mode(int on);
int install_joystick(int type);


#ifndef TRUE
#define TRUE (1==1)
#define FALSE (1==0)
#endif

// dumies
#define DIGI_AUTODETECT 0
#define MIDI_AUTODETECT 0


#define JOY_TYPE_AUTODETECT 0

extern int num_joysticks;








#define MAX_JOYSTICK_AXIS 2
#define MAX_JOYSTICK_STICKS 2
#define MAX_JOYSTICK_BUTTONS 2





/* information about a single joystick axis */
typedef struct JOYSTICK_AXIS_INFO
{
   int pos;
   int d1, d2;
   char *name;
} JOYSTICK_AXIS_INFO;


/* information about one or more axis (a slider or directional control) */
typedef struct JOYSTICK_STICK_INFO
{
   int flags;
   int num_axis;
   JOYSTICK_AXIS_INFO axis[MAX_JOYSTICK_AXIS];
   char *name;
} JOYSTICK_STICK_INFO;


/* information about a joystick button */
typedef struct JOYSTICK_BUTTON_INFO
{
   int b;
   char *name;
} JOYSTICK_BUTTON_INFO;


typedef struct JOYSTICK_INFO
{
   int flags;
   int num_sticks;
   int num_buttons;
   JOYSTICK_STICK_INFO stick[MAX_JOYSTICK_STICKS];
   JOYSTICK_BUTTON_INFO button[MAX_JOYSTICK_BUTTONS];
} JOYSTICK_INFO;

extern JOYSTICK_INFO joy[5];


