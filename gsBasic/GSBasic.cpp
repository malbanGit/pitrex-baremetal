// V32Basic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GSBCommon.h"
#include "Executive.h"


//#include "vectrex32/Camera.h"



#ifdef __cplusplus
extern "C" {
#endif
#include <vectrex/vectrexInterface.h>
int v_printStringRasterFont(int8_t x, int8_t y, char* _string, int8_t xSize, int8_t ySize, unsigned char delimiter, unsigned char *rlines[]);
#include "vio.h"

  #ifdef __cplusplus
}
#endif

bool ctrlC = false;

// TODO:
extern "C" void ClearVectrex();
//extern "C" void GSBASICNewState(State_t state);
//extern "C" void DelayReset();
extern bool breakOnControlC;


extern void RegisterVectrexBuiltInConstants();
extern void RegisterVectrexBuiltInFunctions();


const char *Greeting = "Galactic Studios BASIC\r\nCopyright (c)2016-2020 By Robert E. Alexander\r\n\r\nReady\r\n\nCurrent directory: ";

class Notifications : public GSBASICNotifications
{
public:
	void RegisterConstants()
	{
      RegisterVectrexBuiltInConstants();
      RegisterVectrexBuiltInFunctions();
    }

    void DisplayGreeting() 
	{
      if (displayed) return;
      displayed = 1;
		char currentDirectory[PATH_MAX];
		FPUTS(Greeting, stdout);

		getcwd(currentDirectory, PATH_MAX);
        
		FPUTS(currentDirectory, stdout);
	}
	
    void NewState(State_t state) 
    {
// TODO:
//        GSBASICNewState(state);
    }
    
    virtual void RuntimeReset() 
    {
        ClearVectrex();
        breakOnControlC = true;
    }
    
    virtual void PrintMemoryUsage()
    {
    }
private:
  int displayed = 0;
};
// for now INI setting just stupidly overwrite other saved settings!
static int gsbasicIniHandler(void* user, const char* section, const char* name, const char* value)
{
  // cascading ini files
  // first check if there are "general" entries
  if (iniHandler(user, section, name, value) == 1) return 1;

  
  #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
  #define MATCH_NAME(n) strcmp(name, n) == 0
  if (MATCH_NAME("T_WIDTH")) T_WIDTH = atoi(value); else 
  if (MATCH_NAME("T_HEIGHT")) T_HEIGHT = atoi(value); else 
  if (MATCH_NAME("Y_START")) Y_START = atoi(value); else 
  if (MATCH_NAME("MAX_VLINES")) MAX_VLINES = atoi(value); else 
  if (MATCH_NAME("MAX_VROWS")) MAX_VROWS = atoi(value); else 
  if (MATCH_NAME("VECTREX_MODE")) vectrexMode = atoi(value); else 
  {
      printf("Ini-Handler, unkown entry: %s = %s", name, value);
      return 0;  /* unknown section/name, error */
  }
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

Notifications _notifications;
Notifications *notifications = &_notifications;

// constructor init!!!
// see: https://embeddedartistry.com/blog/2019/04/17/exploring-startup-implementations-newlib-arm/
/* These magic symbols are provided by the linker.  */
extern void (*__preinit_array_start []) (void) __attribute__((weak));
extern void (*__preinit_array_end []) (void) __attribute__((weak));
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));



extern char **linebuffer;

int main(int argc, char *argv[])
{

  size_t count;
  size_t i;
  count = __preinit_array_end - __preinit_array_start;
  for (i = 0; i < count; i++)
    __preinit_array_start[i] ();

  count = __init_array_end - __init_array_start;
  for (i = 0; i < count; i++)
    __init_array_start[i] ();

  int pv = 0;
  if (pv = ini_parse("gsbasic.ini", gsbasicIniHandler, 0) < 0)  
    printf("gsbasic.ini not loaded!\n\r");
  else printf("gsbasic.ini loaded!\n\r");
  
//  linebuffer = new char[MAX_VLINES][MAX_VROWS];

  linebuffer = new char*[MAX_VLINES];
  for (int l=0;l<MAX_VLINES;l++)
  {
    linebuffer[l] = new char [MAX_VROWS]; 
  }
  
  
  
    bool echoOn = false;
    char consoleNewlineChar = '\n';
    chdir((char *)"/basic");

  
  
    vectrexinit(1);
    v_init();
    v_initKeyboard();
    v_noSound();
    v_setRefresh(50);
    v_setClientHz(30);
    v_setupIRQHandling();
    v_enableJoystickAnalog(1,1,0,0);
    v_enableSoundOut(1);
    v_enableExtendedSoundOut(1);
    v_enableButtons(1);
    
    ::notifications = notifications;
	static Executive exec(notifications, echoOn, consoleNewlineChar);
	exec.Main(argc, argv);

	return 0;
}

extern "C" void OutOfMemoryHandler()
{
//    int used, free, max;
//    debug_heap(&used, &free, &max);
    notifications->FatalError("Out of memory");
}
