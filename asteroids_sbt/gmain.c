//------------------------------------------------------------------------
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <vectrex/vectrexInterface.h>

#include "dvg.h"
//#include "rom.h"

unsigned char stack[256];

unsigned long wcount;

unsigned short lastopadd;

void game ( unsigned short opadd );

typedef unsigned char BYTE;
typedef unsigned short WORD;

BYTE a_reg,x_reg,y_reg,flag_reg,s_reg;
WORD pc_reg = 0;

BYTE RTI;

#define A a_reg
#define X x_reg
#define Y y_reg
#define S s_reg
#define PC pc_reg

unsigned char C;
unsigned char ZN;
unsigned char DEC;
unsigned char V;


unsigned long clockticks;
unsigned long instcount=0;
unsigned long nmi_count=0;

unsigned short temp;
unsigned short temp2;


unsigned char ram[0x8000];

int sum,saveflags;

unsigned char value;

unsigned long kcount;

int useSamples=1;
unsigned char sfireSample[10000];
int sfireSize;
int sfireRate = 10000;

unsigned char explosion1Sample[10000];
int explostion1Size;
int explosion1Rate = 10000;

unsigned char explosion2Sample[10000];
int explostion2Size;
int explosion2Rate = 12000;

unsigned char explosion3Sample[10000];
int explostion3Size;
int explosion3Rate = 14000;



#define WriteMemory(a,b) (ram[a]=b)
#define ReadMemory(x)  (ram[x])

static unsigned char buffer[0x100];
void WriteMemory3200(int data)
{
  static int asteroid_bank = 0;
  int asteroid_newbank;
  WriteMemory(0x3200,data);
  asteroid_newbank = (data >> 2) & 1;
//  SCRFLIP=GI[CPU0][0x18];
  if (asteroid_bank != asteroid_newbank) 
  {
      /* Perform bankswitching on page 2 and page 3 */
      asteroid_bank = asteroid_newbank;
      memcpy(buffer, (&ram[0]) + 0x200, 0x100);
      memcpy((&ram[0]) + 0x200, (&ram[0]) + 0x300, 0x100);
      memcpy((&ram[0]) + 0x300, buffer, 0x100);
  }
//  set_aae_leds ((~data & 0x02), (~data & 0x01),0 );
}




//------------------------------------------------------------------------
void ADC ( unsigned char value )
{
      //saveflags=(P & 0x01);
      sum= ((char) A) + ((char) value) + ( C & 0x01);
      if ((sum>0x7f) || (sum<-0x80)) V=0x40; else V=0x00; //P |= 0x40; else P &= 0xbf;
      sum= A + value + ( C & 0x01);
      if (sum>0xff) C = 0x01; else C = 0x00; //P |= 0x01; else P &= 0xfe;
      A=sum;
      if (DEC & 0x08)
      {
              //P &= 0xfe;
              C = 0x00;
              if ((A & 0x0f)>0x09)
                      A += 0x06;
              if ((A & 0xf0)>0x90)
              {
                      A += 0x60;
                      //P |= 0x01;
                      C = 0x01;
              }
      }
      else
      {
              clockticks++;
      }
      ZN=A; //if (A) P &= 0xfd; else P |= 0x02;  if (A & 0x80) P |= 0x80; else P &= 0x7f;
}
//------------------------------------------------------------------------
void SBC ( unsigned char value )
{
      value = value ^ 0xFF;
      //saveflags=(P & 0x01);
      sum= ((char) A) + ((char) value) + (( C & 0x01) << 4);
      if ((sum>0x7f) || (sum<-0x80)) V=0x01; else V=0x00; //P |= 0x40; else P &= 0xbf;
      sum= A + value + ( C & 0x01);
      if (sum>0xff) C=0x01; else C=0x00; //P |= 0x01; else P &= 0xfe;
      A=sum;
      //if (P & 0x08)
      if(DEC & 0x08)
      {
              A -= 0x66;
              //P &= 0xfe;
              C = 0x00;
              if ((A & 0x0f)>0x09)
                      A += 0x06;
              if ((A & 0xf0)>0x90)
              {
                      A += 0x60;
                      //P |= 0x01;
                      C = 0x01;
              }
      }
      else
      {
              clockticks++;
      }
      ZN=A; //if (A) P &= 0xfd; else P |= 0x02;  if (A & 0x80) P |= 0x80; else P &= 0x7f;
}
//------------------------------------------------------------------------

void showme ( unsigned short opadd, unsigned char opcode );


//----  ----
#include "game.c"
//----  ----

//------------------------------------------------------------------------
void showme ( unsigned short opadd, unsigned char opcode )
{
    if(clockticks>250)
    {
        clockticks-=250;
        ram [0x2001] ^= 0xff;
        if (++nmi_count == 24)
        {
            nmi_count = 0;
            stack[S--]=(opadd>>8)&0x7F;
            stack[S--]=opadd&0xFF;
            if(ZN) ZN|=0x02;
            stack[S--]=(ZN&0x82)|(DEC&0x08)|(V&0x40)|(C&0x01);
            PCSTART=NMISTART;
            RTI=0;
            while(1)
            {
                game(PCSTART);
                if(RTI)
                {
                    break;
                }
            }
        }
    }
}

void loadRom()
{
    int r = 0;
    if (loadFromZip ("roms/asteroid2.zip", "035143-02.j2", &ram[0x7800]) != 0)
      if (loadFromZip ("roms/asteroid.zip", "035143-02.j2", &ram[0x7800]) != 0) r-=1;
    if (loadFromZip ("roms/asteroid2.zip", "035127-02.np3", &ram[0x5000]) != 0)
      if (loadFromZip ("roms/asteroid.zip", "035127-02.np3", &ram[0x5000]) != 0) r-=1;

    r += loadFromZip ("roms/asteroid2.zip", "035145-02.ef2", &ram[0x6800]);
    r += loadFromZip ("roms/asteroid2.zip", "035144-02.h2", &ram[0x7000]);
    uint32_t crc;
    crc = 0;printf("CRC32 035145-02.ef2: %08X\r\n", crc32(&ram[0x6800], 0x800, &crc ));
    crc = 0;printf("CRC32 035144-02.h2: %08X\r\n",  crc32(&ram[0x7000], 0x800, &crc ));
    crc = 0;printf("CRC32 035143-02.j2: %08X\r\n",  crc32(&ram[0x7800], 0x800, &crc ));
    crc = 0;printf("CRC32 035127-02.np3: %08X\r\n", crc32(&ram[0x5000], 0x800, &crc ));
    
    crc = 0;if (crc32(&ram[0x6800], 0x800, &crc ) != 0x0CC75459) r -=1;
    crc = 0;if (crc32(&ram[0x7000], 0x800, &crc ) != 0x096ED35C) r -=1;
    crc = 0;if (crc32(&ram[0x7800], 0x800, &crc ) != 0x312CAA02) r -=1;
    crc = 0;if (crc32(&ram[0x5000], 0x800, &crc ) != 0x8B71FD9E) r -=1;
    
    if (r != 0)
    {
      v_errori("ROM LOAD ERROR", 1);
    }
    
    ram[0x2000]=0x7F;
    ram[0x2001]=0x7F;
    ram[0x2002]=0x7F;
    ram[0x2003]=0x7F;
    ram[0x2004]=0x7F;
    ram[0x2005]=0x7F;
    ram[0x2006]=0x7F;
    ram[0x2007]=0x7F;
    ram[0x2400]=0x7F;
    ram[0x2401]=0x7F;
    ram[0x2402]=0x7F;
    ram[0x2403]=0x7F;
    ram[0x2404]=0x7F;
    ram[0x2405]=0x7F;
    ram[0x2406]=0x7F;
    ram[0x2407]=0x7F;
    ram[0x2800]=0xFC;
    ram[0x2801]=0xFC;
    ram[0x2802]=0xFD;
    ram[0x2803]=0xFC;
    ram[0x2C08]=0x9D;
    ram[0x2C09]=0x09;
    ram[0x2C0A]=0xFF;
    ram[0x2C0E]=0xFF;
    ram[0x2C0F]=0xFF;
}

#include "sound.i"
int startCounter = 0;
int vpInit_start = 0;
int highscoreSaved = 1;
int highscoreLoaded = 0;

signed char rotateLeft=0;
signed char rotateRight=0;
signed char thrust=0;
signed char fire=0;
signed char hyperspace=0;
signed char start1=0;
signed char start2=0;
signed char enterDip=0;
InputMapping asteroidMappings[] = 
{
  {"ROTATE_LEFT"    , &rotateLeft, BUTTON_1_1|BUTTON_2_1|JOYPAD_1_ANALOG|JOYPAD_2_ANALOG|JOYPAD_LEFT},
  {"ROTATE_RIGHT"   , &rotateRight,BUTTON_1_2|BUTTON_2_2|JOYPAD_1_ANALOG|JOYPAD_2_ANALOG|JOYPAD_RIGHT},
  {"THRUST"         , &thrust,     BUTTON_1_3|BUTTON_2_3|JOYPAD_1_ANALOG|JOYPAD_2_ANALOG|JOYPAD_UP},
  {"FIRE"           , &fire,       BUTTON_1_4|BUTTON_2_4},
  {"HYPERSAPCE"     , &hyperspace, JOYPAD_1_ANALOG|JOYPAD_2_ANALOG|JOYPAD_DOWN},
  {"START1"         , &start1,     BUTTON_1_3|BUTTON_1_4|BUTTON_AND},
  {"START2"         , &start2,     BUTTON_2_3|BUTTON_2_4|BUTTON_AND},
  {"ENTER_DIP"      , &enterDip,   BUTTON_1_1|BUTTON_1_4|BUTTON_AND},
  {0,0,0}
};

unsigned char dipLanguageSetting = 0; // english
unsigned char dipLivesSetting = 2; // 4 lives

DipSwitchSetting dipLanguage = 
{
  //  2803 XXXXXXLL L = Language 0 = English, 1 = German, 2 = French, 3 = Spanish 
  "LANGUAGE",
  &dipLanguageSetting,
  { 
    {"ENGLISH", 0}, 
    {"GERMAN", 1},
    {"FRENCH", 2},
    {"SPANISH", 3},
    {0,0}
  }
};
DipSwitchSetting dipLives = 
{
  //  2802 XXXXXXLM M = center coin mul 0->*1, 1->*2, L = lives 1->4 lives, 0 -> 3 lives, X = don't care
  "LIVES",
  &dipLivesSetting,
  { 
    {"3", 0}, 
    {"4", 2},
    {0,0}
  }
};
DipSwitchSetting *asteroidSwitches[] = 
{
  &dipLanguage,
  &dipLives,
  0,
};

void applyDips()
{
  // These can be written "directly", since they go to different memory locations
  // beware other machines, where settings might need "OR" or "AND" 
  WriteMemory(0x2803,dipLanguageSetting); 
  WriteMemory(0x2802,dipLivesSetting); 

  // st upon reset
  // but overwritten by "load"
  if (dipLivesSetting ==0)
  {
    WriteMemory(0x56,3); 
  }
  else
  {
    WriteMemory(0x56,4); 
  }
}

// returns 1, if reset should be done
int handleInput(void) 
{
    v_doInputMapping(asteroidMappings);

    if (enterDip)
    {
      v_getDipSettings(asteroidSwitches, "ASTEROID DIP SWITCHES");
      applyDips();
      vpInit_start = 0; // reload "PUSH START" vectors for saving
    }
/*    
    if(rotateLeft)
        WriteMemory(0x2407,0x80); else WriteMemory(0x2407,0x00); //A left or Y
    if(rotateRight)
        WriteMemory(0x2406,0x80); else WriteMemory(0x2406,0x00); //B right or X
    if(thrust)
        WriteMemory(0x2405,0x80); else WriteMemory(0x2405,0x00); //volup thrust
    if(fire)
        WriteMemory(0x2004,0x80); else WriteMemory(0x2004,0x00); //voldn fire
    if(start1)
        WriteMemory(0x2403,0x80); else WriteMemory(0x2403,0x00); // start1
    if(start2)
        WriteMemory(0x2404,0x80); else WriteMemory(0x2404,0x00); // start2
    if(hyperspace)
        WriteMemory(0x2003,0x80); else WriteMemory(0x2003,0x00); // hyperspace
*/        
        
        
// this more complicated
// input "asking", is because I do for now particular reason, other than "because"
// analog joystick
        
        
    if ((rotateLeft<-80) || (rotateLeft == 1))
        WriteMemory(0x2407,0x80); else WriteMemory(0x2407,0x00); //A left or Y

    if ((rotateRight>80) || (rotateRight == 1))
        WriteMemory(0x2406,0x80); else WriteMemory(0x2406,0x00); //B right or X
        
    if ((thrust==1) || (thrust>80))
        WriteMemory(0x2405,0x80); else WriteMemory(0x2405,0x00); //volup thrust
        
    if(fire)
        WriteMemory(0x2004,0x80); else WriteMemory(0x2004,0x00); //voldn fire
        
    if(start1)
        WriteMemory(0x2403,0x80); else WriteMemory(0x2403,0x00); // start1
        
    if(start2)
        WriteMemory(0x2404,0x80); else WriteMemory(0x2404,0x00); // start2
        
    if((hyperspace == 1) || (hyperspace<-80))
        WriteMemory(0x2003,0x80); else WriteMemory(0x2003,0x00); // hyperspace
        return 0;
}


int endGameRound()
{
    handleSound();    
    v_playAllSFX();
    v_doSound();    // not needed in IRQ Mode
    v_readButtons(); // not neededin IRQ mode
    v_readJoystick1Analog(); // not neededin IRQ mode
    v_WaitRecal();
    return handleInput(); // possibly reset machine!
}
    
int skipAtariCopyright = 1;


NameReplacePair switchMap[] =
{
  {"ENGLISH", "0"},
  {"GERMAN", "1"},
  {"FRENCH", "2"},
  {"SPANISH", "3"},
  {0,0}
};
 
// for now INI setting just stupidly overwrite other saved settings!
static int astIniHandler(void* user, const char* section, const char* name, const char* value)
{
  // cascading ini files
  // first check if there are "general" entries
  if (iniHandler(user, section, name, value) == 1) return 1;

  
  #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
  #define MATCH_NAME(n) strcmp(name, n) == 0
  int i=0;
  while (asteroidMappings[i].destination!=0)
  {
    if (MATCH_NAME(asteroidMappings[i].name)) 
    {
      asteroidMappings[i].mapping = evaluateInputIniLine(value);
      return 1;
    }
    i++;
  }  
  i = 0;
  while (asteroidSwitches[i]!=0)
  {
    char buf[80];
    strcpy(buf, "AST_DISW_");
    strcpy(buf+strlen("AST_DISW_"), asteroidSwitches[i]->name) ;
    if (MATCH_NAME(buf)) 
    {
      *asteroidSwitches[i]->destination = evaluateLine(value, switchMap);
      return 1;
    }
    i++;
  }

  if (MATCH_NAME("USE_SAMPLES")) useSamples = atoi(value);
  
  if (MATCH_NAME("SKIP_ATARI_COPYRIGHT")) skipAtariCopyright = atoi(value);
  else 
  {
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

VectorPipelineBase start[100];

typedef struct 
{
  int count;
  signed int xOffset;
  signed int xStart;
  signed int yStart;
  signed int xEnd;
  signed int yEnd;
} SaveVectorTest;

SaveVectorTest pushStartLanguages[4] = 
{
  {34,0,     -3500,13500,-3500,14700}, // english
  {75,-3000, -3500,13500,-2940,13500}, // german
  {60,-2000, -3500,13500,-3500,14300}, // french
  {43,-500,  -3500,13500,-3500,14700}, // spanish
};

typedef struct 
{
  int count;
  signed int xStart;
  signed int yStart;
  signed int xEnd;
  signed int yEnd;
} GameOverTest;
GameOverTest gameOverLanguages[4] = 
{
  {388, -15540,7900,-15540,8700}, // english
  {512,-15820, 7900, -15260, 7900}, // german
  {427, -15820,9100,-15540,7900}, // french
  {413, -15820,7900,-15260,7900}, // spanish
};

static inline void saveStart(VectorPipelineBase *cpb)
{
  if (vpInit_start) return;
  vpInit_start = 1;
  
  for (int i=0;i<pushStartLanguages[dipLanguageSetting].count;i++)
  {
    // TODO apply offsets
    /*
    cpb->xStart;
    cpb->xEnd;
    cpb->x0;
    cpb->x1;
    */

    start[i].yStart = cpb->yStart;
    start[i].xStart = cpb->xStart;
    start[i].yEnd = cpb->yEnd;
    start[i].xEnd = cpb->xEnd;
    start[i].y0 = cpb->y0;
    start[i].x0 = cpb->x0;
    start[i].y1 = cpb->y1;
    start[i].x1 = cpb->x1;
    start[i].pattern = cpb->pattern;
    start[i].intensity = 10; //cpb->intensity;
    
    start[i].sms = cpb->sms;
    start[i].timingForced = cpb->timingForced;
    start[i].force = cpb->force;
    start[i].debug[0] = 0;
    
    if (i==pushStartLanguages[dipLanguageSetting].count-1)
      start[i].next = 0;
    else
      start[i].next = &start[i+1];

    if (i==0)
      start[i].previous = 0;
    else
      start[i].previous = &start[i-1];

    cpb = cpb->next;
  }
}
static inline void addStart(VectorPipelineBase ** cpbPointer)
{
  VectorPipelineBase *oldPrevious = (*cpbPointer)->previous;
  VectorPipelineBase *oldNext = (*cpbPointer);

  start[0].previous = oldPrevious;
  start[pushStartLanguages[dipLanguageSetting].count-1].next = oldNext;
  
  if (oldPrevious != 0)
    oldPrevious->next = &start[0];

  if (oldNext != 0)
    oldNext->previous = &start[pushStartLanguages[dipLanguageSetting].count-1];
  
  *cpbPointer = &start[0];
}


#define BIG_VC 10  
VectorPipelineBase bigUFO[BIG_VC];
int vpInit_big = 0;

static inline void saveBigUFOReverse(VectorPipelineBase *cpb)
{
 // if (vpInit_big) return;
  vpInit_big = 1;

  for (int i=0;i<BIG_VC-1;i++)
  {
    cpb = cpb->previous;
  }
  
  for (int i=0;i<BIG_VC;i++)
  {
    bigUFO[i].yStart = -cpb->yStart;
    bigUFO[i].xStart = -cpb->xStart;
    bigUFO[i].yEnd = -cpb->yEnd;
    bigUFO[i].xEnd = -cpb->xEnd;
    bigUFO[i].y0 = -cpb->y0;
    bigUFO[i].x0 = -cpb->x0;
    bigUFO[i].y1 = -cpb->y1;
    bigUFO[i].x1 = -cpb->x1;
    bigUFO[i].pattern = cpb->pattern;
    bigUFO[i].intensity = 0; //cpb->intensity;
    
    bigUFO[i].sms = cpb->sms;
    bigUFO[i].timingForced = cpb->timingForced;
    bigUFO[i].force = cpb->force;
    bigUFO[i].debug[0] = 0;
    
    if (i==BIG_VC-1)
      bigUFO[i].next = 0;
    else
      bigUFO[i].next = &bigUFO[i+1];

    if (i==0)
      bigUFO[i].previous = 0;
    else
      bigUFO[i].previous = &bigUFO[i-1];

    cpb = cpb->next;
  }
}
#define SMALL_VC 10  
VectorPipelineBase smallUFO[SMALL_VC];
int vpInit_small = 0;

static inline void saveSmallUFOReverse(VectorPipelineBase *cpb)
{
//  if (vpInit_small) return;
  vpInit_small = 1;
  for (int i=0;i<SMALL_VC-1;i++)
  {
    cpb = cpb->previous;
  }
  
  for (int i=0;i<SMALL_VC;i++)
  {
    smallUFO[i].yStart = -cpb->yStart;
    smallUFO[i].xStart = -cpb->xStart;
    smallUFO[i].yEnd = -cpb->yEnd;
    smallUFO[i].xEnd = -cpb->xEnd;
    smallUFO[i].y0 = -cpb->y0;
    smallUFO[i].x0 = -cpb->x0;
    smallUFO[i].y1 = -cpb->y1;
    smallUFO[i].x1 = -cpb->x1;
    smallUFO[i].pattern = cpb->pattern;
    smallUFO[i].intensity = 0; //cpb->intensity;
    
    smallUFO[i].sms = cpb->sms;
    smallUFO[i].timingForced = cpb->timingForced;
    smallUFO[i].force = cpb->force;
    smallUFO[i].debug[0] = 0;
    
    if (i==SMALL_VC-1)
      smallUFO[i].next = 0;
    else
      smallUFO[i].next = &smallUFO[i+1];

    if (i==0)
      smallUFO[i].previous = 0;
    else
      smallUFO[i].previous = &smallUFO[i-1];

    cpb = cpb->next;
  }
}

static inline void addBigAfter(VectorPipelineBase ** cpbPointer)
{
  VectorPipelineBase *oldNext = (*cpbPointer)->next;
  VectorPipelineBase *oldThis = (*cpbPointer);

  bigUFO[0].previous = oldThis;
  bigUFO[BIG_VC-1].next = oldNext;
  
  if (oldThis != 0)
    oldThis->next = &bigUFO[0];

  if (oldNext != 0)
    oldNext->previous = &bigUFO[BIG_VC-1];
}

static inline void addSmallAfter(VectorPipelineBase ** cpbPointer)
{
  VectorPipelineBase *oldNext = (*cpbPointer)->next;
  VectorPipelineBase *oldThis = (*cpbPointer);

  smallUFO[0].previous = oldThis;
  smallUFO[SMALL_VC-1].next = oldNext;
  
  if (oldThis != 0)
    oldThis->next = &smallUFO[0];

  if (oldNext != 0)
    oldNext->previous = &smallUFO[SMALL_VC-1];
}

static inline void saveHighScore()
{
  if (highscoreSaved)return;
  highscoreSaved = 1;
  FILE *out=0;
  out = fopen("asteroid.scr", "w"); 
  if (out == 0) 
  {
    printf("Can't save highscore. File Error.");
    return;
  }

  unsigned int lenSaved=0;
  lenSaved = fwrite(ram+0x1d, 64, 1, out); // 10 * 2 byte score
  fclose(out);
  printf("Highscore saved.");
}
static inline void loadHighScore()
{
  if (highscoreLoaded) return;
  
  printf("pre Load - $71 = %02x\n\r", ReadMemory(0x71));
  
  highscoreLoaded = 1;
  FILE *fileRead;
  fileRead = fopen("asteroid.scr", "rb");
  if (fileRead == 0)
  {
    unsigned char defaultHS[]= 
    {
      0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
      0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x17, 0x0B, 0x16, 0x17, 0x0B, 0x16, 0x17, 0x0B, 0x16, 
      0x17, 0x0B, 0x16, 0x17, 0x0B, 0x16, 0x17, 0x0B, 0x16, 0x17, 0x0B, 0x16, 0x17, 0x0B, 0x16, 0x17, 
      0x0B, 0x16, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD9
    };
    for (int i=0;i<64;i++)
    {
      ram[0x1d+i] = defaultHS[i];
    }
    return;
  }
  unsigned int lenLoaded=0;
  lenLoaded = fread(ram+0x1d, 64, 1, fileRead);   // 10 * 2 byte score
  fclose(fileRead);
  printf("Highscore loaded.");
}

void skipInPipeline(VectorPipelineBase ** cpbPointer, int count)
{
  VectorPipelineBase *cpb = *cpbPointer;
  for (int i=0; i<count;i++)
  {
    cpb = cpb->next;
    if (cpb==0) break;
  }
  *cpbPointer = cpb;
}

int shrinkCounter =0;    
int checkForSpecialVectors(VectorPipelineBase ** cpbPointer,VectorPipeline **plPointer, int *fpc, int *cx, int *cy) 
{
  VectorPipeline *pl=*plPointer;
  VectorPipelineBase *cpb = *cpbPointer;
  
  // Atari copyright sign END with
  if ((cpb->xStart == -3500) && (cpb->yStart == -19700)) // pre scale, rotation coordinates
  {
    if (skipAtariCopyright)
    {
        skipInPipeline(cpbPointer, 49);
        return CV_SPECIAL_SKIP; //skip this entry
    }
    
    return CV_SPECIAL_DEFLOK;
  }
  if (*fpc == 0)
  {
    // game over
    if ((cpb->xStart == gameOverLanguages[dipLanguageSetting].xStart) && (cpb->yStart == gameOverLanguages[dipLanguageSetting].yStart) && 
           (cpb->xEnd == gameOverLanguages[dipLanguageSetting].xEnd) && (cpb->yEnd == gameOverLanguages[dipLanguageSetting].yEnd) ) // pre scale, rotation coordinates
    {
      shrinkCounter = gameOverLanguages[dipLanguageSetting].count;
      highscoreSaved = 0;
    }
    else
    {
      saveHighScore();
    }
    // PUSH START
    if (    (cpb->xStart == pushStartLanguages[dipLanguageSetting].xStart) && (cpb->yStart == pushStartLanguages[dipLanguageSetting].yStart) 
         && (cpb->xEnd == pushStartLanguages[dipLanguageSetting].xEnd) && (cpb->yEnd == pushStartLanguages[dipLanguageSetting].yEnd) ) // pre scale, rotation coordinates
    {
      startCounter = 32;
      saveStart(cpb);
      return CV_SPECIAL_NONE; // do not skip
    }
    else
    {
      if (startCounter)
      {
        startCounter--;
        addStart(cpbPointer);
        return CV_SPECIAL_NONE; // do not skip
      }
    }
  }
  if (shrinkCounter)
  {
    shrinkCounter--;
    cpb->x0 = cpb->x0*0.8;
    cpb->y0 = cpb->y0*0.8;
    cpb->x1 = cpb->x1*0.8;
    cpb->y1 = cpb->y1*0.8;
    if (dipLanguageSetting == 1) // german
    {
      // skip "DRUECKEN
      if (   ((cpb->xStart == 6020) && (cpb->yStart == 4700)) 
          || ((cpb->xStart == 8540) && (cpb->yStart == 3100)) )
      {
        skipInPipeline(cpbPointer, 31);
        return CV_SPECIAL_SKIP; //skip this entry
      }
    }
    return CV_SPECIAL_NONE; // do not skip
  }

  // big UFO
  if ((cpb->xStart-cpb->xEnd  == -14840  +14280) && (cpb->yStart-cpb->yEnd  == -12100  +12100))
  {
    if (cpb->intensity == 0x68)
    {
      saveBigUFOReverse(cpb);
      addBigAfter(cpbPointer);
      return CV_SPECIAL_NONE;//0x80 + CV_SPECIAL_DEFLOK;
    }
  }

  // small UFO
  if ((cpb->xStart-cpb->xEnd  == -14805  + 14525) && (cpb->yStart-cpb->yEnd  == -750  +750))
  {
    if (cpb->intensity == 0x68)
    {
      saveSmallUFOReverse(cpb);
      addSmallAfter(cpbPointer);
      return CV_SPECIAL_NONE;//0x80 + CV_SPECIAL_DEFLOK;
    }
  }
  return CV_SPECIAL_NONE; // do not skip
}

 

int main( void )
{
  vectrexinit(1);
  v_init();

  
  v_noSound();
  // special vectrex  interface settings for asteroids
  v_setRefresh(50);
  v_setClientHz(62); // 62.5
  v_setupIRQHandling();
  v_enableJoystickDigital(1,1,0,0);
  v_enableJoystickAnalog(1,1,0,0);
  v_enableSoundOut(1);
  v_enableButtons(1);
  keepDotsTogether = 0;
  useDoubleTimer = 1;
  checkExternal = checkForSpecialVectors;
  useSamples = 1;

  int pv = 0;
  if (pv = ini_parse("asteroid_sbt.ini", astIniHandler, 0) < 0)  
    printf("asteroid_sbt.ini not loaded!\n\r");
  else printf("asteroid_sbt.ini loaded!\n\r");


  sfireSize = v_loadRAW("samples/asteroids/sfire.raw", sfireSample);
  explostion1Size = v_loadRAW("samples/asteroids/explosion1.raw", explosion1Sample);
  explostion2Size = v_loadRAW("samples/asteroids/explosion2.raw", explosion2Sample);
  explostion3Size = v_loadRAW("samples/asteroids/explosion3.raw", explosion3Sample);
  v_playIRQSample(explosion3Sample, explostion3Size, explosion3Rate, PLAY_ONCE);
  
  loadRom();

  A=X=Y=0;
  C=0;
  ZN=0;
  V=0;
  S=0xFF;
  clockticks=0;
  kcount=0;
  applyDips();  
  while(1) 
  {
    game(PCSTART);
  }
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
