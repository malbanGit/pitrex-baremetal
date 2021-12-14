// BZ2.cpp : Defines the entry point for the application.
//
#include "defines.h"


#include "BZ2.h"
#include "scene.h"

scene oglScene;

int mWidth, mHeight;



extern "C" 
{
  void drawLine(float x0, float y0, float x1, float y1, int c)
  {
//    printf("Line: %f, %f, %f,%f, Color: %i\n", x0,y1,x1,y1,c);
    
    if (c<64) c*=2;
    v_directDraw32(x0*V_MUL, y0*V_MUL, x1*V_MUL, y1*V_MUL, c);

  }
}


// for now INI setting just stupidly overwrite other saved settings!
static int bz2IniHandler(void* user, const char* section, const char* name, const char* value)
{
  // cascading ini files
  // first check if there are "general" entries
  if (iniHandler(user, section, name, value) == 1) return 1;

  
  #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
  #define MATCH_NAME(n) strcmp(name, n) == 0
  /*
  if (MATCH_NAME("T_WIDTH")) T_WIDTH = atoi(value); else 
  if (MATCH_NAME("T_HEIGHT")) T_HEIGHT = atoi(value); else 
  if (MATCH_NAME("Y_START")) Y_START = atoi(value); else 
  if (MATCH_NAME("MAX_VLINES")) MAX_VLINES = atoi(value); else 
  if (MATCH_NAME("MAX_VROWS")) MAX_VROWS = atoi(value); else 
  if (MATCH_NAME("VECTREX_MODE")) vectrexMode = atoi(value); else 
  {
      printf("Ini-Handler, unkown entry: %s = %s", name, value);
      return 0;  // unknown section/name, error 
  }
  */
/*
 * No special values as of yet
  if (MATCH("VECTREX_EXACT", "ROM_FILE")) strcpy(romName, value); else 
  {
      printf("Ini-Handler, unkown entry: %s = %s", name, value);
      return 0;  / * unknown section/name, error * /
  }
*/  
  return 1;
}


// constructor init!!!
// see: https://embeddedartistry.com/blog/2019/04/17/exploring-startup-implementations-newlib-arm/
/* These magic symbols are provided by the linker.  */
extern void (*__preinit_array_start []) (void) __attribute__((weak));
extern void (*__preinit_array_end []) (void) __attribute__((weak));
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));

int main(int argc, char *argv[])
{

  // initialize global/static classes!
  size_t count;
  size_t i;
  count = __preinit_array_end - __preinit_array_start;
  for (i = 0; i < count; i++)
    __preinit_array_start[i] ();

  count = __init_array_end - __init_array_start;
  for (i = 0; i < count; i++)
    __init_array_start[i] ();

  int pv = 0;
  if (pv = ini_parse("bz2.ini", bz2IniHandler, 0) < 0)  
    printf("bz2.ini not loaded!\n\r");
  else printf("bz2.ini loaded!\n\r");
  
  vectrexinit(1);
  v_init();
//    v_initKeyboard();
  v_noSound();
  v_setRefresh(50);
  v_setClientHz(30);
  v_setupIRQHandling();
  v_enableJoystickDigital(1,1,1,1);
  v_enableSoundOut(1);
  v_enableExtendedSoundOut(1);
  v_enableButtons(1);
  v_setCustomClipping(1,-17000,-19000,17000,19000);
  v_enablePipelineCleanup(1);
  
  
/*    
  if (usbKeyboardAvailable != 1)
  {
    char msg[] = "ERROR! USB KEYBOARD NOT FOUND!";
    v_message(msg);
  }
*/
  while (1)
  {
    v_playAllSFX();
    v_doSound();    // not needed in IRQ Mode
    v_readButtons(); // not neededin IRQ mode
    v_readJoystick1Analog(); // not neededin IRQ mode
    v_WaitRecal();
    
    oglScene.run();
    oglScene.draw(scene::RENDERPASS_PRIMARY);
  }


  return 0;
}

