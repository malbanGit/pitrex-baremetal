
void v_enableExtendedSoundOut(int yesNo)
{
  extendedIRQSound = yesNo;
}
void v_enableSoundOut(int yesNo)
{
  irqSound = yesNo;
}



////////////////////////////////////////////////////////////////////////////
// PSG and samples -> all stuff sound
////////////////////////////////////////////////////////////////////////////
/*
 A ccessing PSG is "expensive" (to set* one r*egister 6/8 (write/read) VIA access are necessary!)!

 To "directly" access PSG registers use functions:
 void            v_writePSG(uint8_t reg, uint8_t data)
 uint8_t         v_readPSG(uint8_t reg)

 However since it is so "expansive" there is a buffer thru which all PSG acccess should be done:
 Therefor we use a buffer system, if all reads writes are done using the buffer functions
 void v_writePSG_buffered(uint8_t reg, uint8_t data)
 uint8_t v_readPSG_buffered(uint8_t reg)

 we create two significant improvements:
 a) all reads are done via the buffer (saves 8 VIA accesses)
 b) only data is written to the PSG, that is not the same as in the buffer already (and thus in the PSG)

 Even more however:
 Also a double buffer is implemented!

 This can be used if you
 e.g.
 - play a YM file
 - and want to "overlay" distinct channels with a sound effect

 The functions are:
 void v_writePSG_double_buffered(uint8_t reg, uint8_t data)
 uint8_t v_readPSG_double_buffered(uint8_t reg)
 void v_PSG_writeDoubleBuffer()

 The read/write functions are more or less the same as the "buffered" ones, but the data is not
 written to VIA when necessary but written to another (double) buffer.
 So you can "overwrite" PSG data in the order you intend, and not actually put the data to the PSG directly.

 When all PSG stuff is done in your "round" - than a call to "v_PSG_writeDoubleBuffer()" must be made, which
 transports the complete contents of double buffer to the PSG (with respect to already buffered data - meaning
 if the PSG already contains a value, that also is not overwritten!)

   -------------------

   In conclusion:
   - use the double buffer functions!
   - once per round write the double buffer!
   */

static unsigned int psgDoubleBuffer[16];
static unsigned int psgShadow[16];
static uint16_t ymLength;
static uint16_t ymPos;
static uint8_t *ymBuffer;
static int ymloop;
float sfx_mul_1=1.0;
float sfx_mul_2=1.0;
float sfx_mul_3=1.0;
unsigned int sfx_status_1;
unsigned int sfx_status_2;
unsigned int sfx_status_3;
uint8_t *sfx_pointer_1;
uint8_t *sfx_pointer_2;
uint8_t *sfx_pointer_3;

uint8_t *sfx_pointer_1_org;
uint8_t *sfx_pointer_2_org;
uint8_t *sfx_pointer_3_org;
int sfx_priority1;
int sfx_priority2;
int sfx_priority3;

typedef struct {
    unsigned int *status;
    int *priority;
} SFX;

SFX ayfx[3] =
{
    {&sfx_status_1, &sfx_priority1},
    {&sfx_status_2, &sfx_priority2},
    {&sfx_status_3, &sfx_priority3}
};

// used to cut of thrust in asteroids
int v_addSFXForced(unsigned char *buffer, int channel, int loop)
{
    v_playSFXCont(buffer, channel, loop);
    *ayfx[channel].priority=0;
    return channel;
}

// allows the same effect to be played multiple times on// different channels
// return -1 on failure
// channel on success
int v_addSFX(unsigned char *buffer, int priority, int loop, int whenEqualPriorityPlayNew)
{
    int use = -1;
    int possibleUse1 = -1;
    int possibleUse2 = -1;
    for (int i=0;i<3;i++)
    {
        if (*ayfx[i].status == NOT_PLAYING)
        {
            use = i;
            break;
        }
        if (*ayfx[i].priority < priority)
        {
            possibleUse1 = i;
        }
        if (whenEqualPriorityPlayNew)
        {
            if (*ayfx[i].priority == priority)
            {
                possibleUse2 = i;
            }
        }
    }
    int reallyUse = -1;
    if (use != -1) reallyUse = use;
    else if (possibleUse1 != -1) reallyUse = possibleUse1;
    else if (possibleUse2 != -1) reallyUse = possibleUse2;
    if (reallyUse == -1) return -1;

    v_playSFXCont(buffer, reallyUse, loop);
    return reallyUse;
}

void v_noSound();

void v_writePSG_double_buffered(uint8_t reg, uint8_t data);
void v_writePSG_buffered(uint8_t reg, uint8_t data);
void v_writePSG(uint8_t reg, uint8_t data);
uint8_t v_readPSG_double_buffered(uint8_t reg);
uint8_t v_readPSG_buffered(uint8_t reg);
uint8_t v_readPSG(uint8_t reg);
void v_PSG_writeDoubleBuffer();

void v_initYM(uint8_t *b, uint16_t length, int l);
int v_playYM();
int play_sfx1();
int play_sfx2();
int play_sfx3();

// set PSG sound output to "none"
void v_noSound()
{
  v_writePSG_double_buffered(8,0); // volume 0
  v_writePSG_double_buffered(9,0);
  v_writePSG_double_buffered(10,0);
  v_writePSG_double_buffered(7,0x3f); // all channel off
  
  if (isIRQMode) return;
#if RASPPI != 1 
  if (isSMPMode) return;
#endif  
  v_writePSG_buffered(8,0); // volume 0
  v_writePSG_buffered(9,0);
  v_writePSG_buffered(10,0);
  v_writePSG_buffered(7,0x3f); // all channel off
}

void v_noSound_channel(int c)
{
  if (c == 0)
  {
    v_writePSG_buffered(8,0); // volume 0
    v_writePSG_buffered(7, psgShadow[7] | (1+8) ); // all channel off

    v_writePSG_double_buffered(8,0); // volume 0
    v_writePSG_double_buffered(7, psgDoubleBuffer[7] | (1+8) ); // all channel off
  }
  if (c == 1)
  {
    v_writePSG_buffered(9,0); // volume 0
    v_writePSG_buffered(7, psgShadow[7] | (2+16) ); // all channel off

    v_writePSG_double_buffered(9,0); // volume 0
    v_writePSG_double_buffered(7, psgDoubleBuffer[7] | (2+16) ); // all channel off
  }
  if (c == 2)
  {
    v_writePSG_buffered(10,0); // volume 0
    v_writePSG_buffered(7, psgShadow[7] | (4+32) ); // all channel off

    v_writePSG_double_buffered(10,0); // volume 0
    v_writePSG_double_buffered(7, psgDoubleBuffer[7] | (4+32) ); // all channel off
  }
}


void v_doSound()
{
  if (isIRQMode) return;
#if RASPPI != 1 
  if (isSMPMode) return;
#endif  
 
  v_PSG_writeDoubleBuffer();
}

void v_initSound()
{
  for (int i=0;i<16;i++)
  {
    psgShadow[i] = -1;
    psgDoubleBuffer[i] = 0;
  }
  sfx_status_1 = NOT_PLAYING;
  sfx_status_2 = NOT_PLAYING;
  sfx_status_3 = NOT_PLAYING;
  sfx_pointer_1 = (uint8_t *) 0;
  sfx_pointer_2 = (uint8_t *) 0;
  sfx_pointer_3 = (uint8_t *) 0;
  sfx_pointer_1_org = (uint8_t *) 0;
  sfx_pointer_2_org = (uint8_t *) 0;
  sfx_pointer_3_org = (uint8_t *) 0;
  sfx_mul_1=1.0;
  sfx_mul_2=1.0;
  sfx_mul_3=1.0;
  sfx_priority1 =0;
  sfx_priority2 =0;
  sfx_priority3 =0;
  ymLength = 0;
  ymBuffer = 0;
  ymloop = 0;
  v_noSound();
}

/***********************************************************************/
uint8_t v_readPSG_double_buffered(uint8_t reg)
{
  return psgDoubleBuffer[reg];
}

void v_writePSG_double_buffered(uint8_t reg, uint8_t data)
{
  psgDoubleBuffer[reg] = data;
  if (reg == 13)
    psgShadow[reg] = data+1; // something different,I don't know why this is needed!
    
}

void v_PSG_writeDoubleBuffer()
{
  if (isIRQMode) return;
#if RASPPI != 1 
  if (isSMPMode) return;
#endif  
  for (int i=0;i<15;i++)
  {
    uint8_t data = psgDoubleBuffer[i];
    if (psgShadow[i] == data) continue;
    psgShadow[i] = data;

    I_SET(VIA_port_a, i); // prepare access of psg port A (0x0e) by writing the register value to VIA port A
    I_SET(VIA_port_b, 0x99); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
    I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)

    I_SET(VIA_port_a, data); // write data to port a of via -> and than to psg
    I_SET(VIA_port_b, 0x91); // set VIA port B to settings: sound BDIR on, BC1 on, mux off, write to PSG
    I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  }
  currentPortA = 0x100; // undefined
}

void v_writePSG_buffered(uint8_t reg, uint8_t data)
{
  if (isIRQMode) return;
#if RASPPI != 1 
  if (isSMPMode) return;
#endif  
  if (psgShadow[reg] == data) return;
  psgShadow[reg] = data;

  I_SET(VIA_port_a, reg); // prepare access of psg port A (0x0e) by writing the register value to VIA port A
  I_SET(VIA_port_b, 0x99); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)

  I_SET(VIA_port_a, data); // write data to port a of via -> and than to psg
  I_SET(VIA_port_b, 0x91); // set VIA port B to settings: sound BDIR on, BC1 on, mux off, write to PSG
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  currentPortA = 0x100; // undefined
}
uint8_t v_readPSG_buffered(uint8_t reg)
{
  return psgShadow[reg];
}

void v_writePSG(uint8_t reg, uint8_t data)
{
  if (isIRQMode) return;
#if RASPPI != 1 
  if (isSMPMode) return;
#endif  
  I_SET(VIA_port_a, reg); // prepare access of psg port A (0x0e) by writing the register value to VIA port A
  I_SET(VIA_port_b, 0x99); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)

  I_SET(VIA_port_a, data); // write data to port a of via -> and than to psg
  I_SET(VIA_port_b, 0x91); // set VIA port B to settings: sound BDIR on, BC1 on, mux off, write to PSG
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  currentPortA = 0x100; // undefined
}

/***********************************************************************/

uint8_t v_readPSG(uint8_t reg)
{
  if (isIRQMode) return 0;
#if RASPPI != 1 
  if (isSMPMode) return 0;
#endif  
  I_SET(VIA_port_a, reg); // prepare access of psg port A (0x0e) by writing the register value to VIA port A
  I_SET(VIA_port_b, 0x99); // set VIA port B to settings: sound BDIR on, BC1 on, mux off
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)

  SET(VIA_DDR_a, 0x00); // set VIA DDR A to input
  I_SET(VIA_port_b, 0x89); // set VIA port B to settings: sound BDIR on, BC1 on, mux off, read from psg
  uint8_t directData = GET(VIA_port_a); // Read buttons
  I_SET(VIA_port_b, 0x81); // set VIA Port B = 81, mux disabled, RAMP disabled, BC1/BDIR = 00 (PSG inactive)
  SET(VIA_DDR_a, 0xff); // set VIA DDR A to output
  currentPortA = 0x100; // undefined
  return directData;
}

void v_initYM(uint8_t *b, uint16_t length, int l)
{
  ymBuffer = b;
  ymloop = l;
  ymLength = length;
  ymPos = 0;
}

void v_setymPos(int i)
{
  ymPos = i;
}

int v_playYM()
{
  if (ymBuffer==0) return 0;
  if (ymPos>=ymLength)
  {
    if (ymloop==1)
      ymPos = 0;
    else
    {
      ymBuffer = 0;
      return 0;
    }
  }
  uint8_t *currentPointer = ymBuffer+ymPos*14;
  ymPos++;
  for (int i=0; i<14; i++)
  {
    v_writePSG_double_buffered(i, *currentPointer);
    currentPointer++;
  }
  return 1;
}
int v_stopYM()
{
  ymLength = 0;
  ymBuffer = 0;
  ymloop = 0;
  v_noSound();
}
int v_isYMPlaying()
{
  return ymBuffer!=0;
}
void v_playSFXLoudness(int channel, float mul)
{
  if (channel == 0) sfx_mul_1 = mul;
  if (channel == 1) sfx_mul_2 = mul;
  if (channel == 2) sfx_mul_3 = mul;
}

// plays a SFX
// if the sfx is already playing... does nothing
void v_playSFXCont(unsigned char *buffer, int channel, int loop)
{
  if (channel == 0)
  {
    sfx_mul_1=1.0;
    if (sfx_pointer_1_org == buffer) return;
    sfx_pointer_1_org = buffer;
    sfx_pointer_1 = buffer;
    if (loop) sfx_status_1 = PLAY_LOOP; else  sfx_status_1 = PLAY_END;
  }
  if (channel == 1)
  {
    sfx_mul_2=1.0;
    if (sfx_pointer_2_org == buffer) return;
    sfx_pointer_2_org = buffer;
    sfx_pointer_2 = buffer;
    if (loop) sfx_status_2 = PLAY_LOOP; else  sfx_status_2 = PLAY_END;
  }
  if (channel == 2)
  {
    sfx_mul_3=1.0;
    if (sfx_pointer_3_org == buffer) return;
    sfx_pointer_3_org = buffer;
    sfx_pointer_3 = buffer;
    if (loop) sfx_status_3 = PLAY_LOOP; else  sfx_status_3 = PLAY_END;
  }
}
// plays a SFX
// if the sfx is already playing... restart it
void v_playSFXStart(unsigned char *buffer, int channel, int loop)
{
 // printf("Play start %d, %p\n", channel +1, buffer);
  if (channel == 0)
  {
    sfx_pointer_1_org = buffer;
    sfx_pointer_1 = buffer;
    if (loop) sfx_status_1 = PLAY_LOOP; else  sfx_status_1 = PLAY_END;
    sfx_mul_1=1.0;
 // printf("sfx_status_1 %d\n",sfx_status_1);
  }
  if (channel == 1)
  {
    sfx_pointer_2_org = buffer;
    sfx_pointer_2 = buffer;
    if (loop) sfx_status_2 = PLAY_LOOP; else  sfx_status_2 = PLAY_END;
    sfx_mul_2=1.0;
  }
  if (channel == 2)
  {
    sfx_pointer_3_org = buffer;
    sfx_pointer_3 = buffer;
    if (loop) sfx_status_3 = PLAY_LOOP; else  sfx_status_3 = PLAY_END;
    sfx_mul_3=1.0;
  }
}
void v_playSFXStopForced(int channel)
{
  if (channel == 0) v_playSFXStop(sfx_pointer_1_org, channel);
  if (channel == 1) v_playSFXStop(sfx_pointer_2_org, channel);
  if (channel == 2) v_playSFXStop(sfx_pointer_3_org, channel);
}

void v_playSFXStop(unsigned char *buffer, int channel)
{
  if (channel == 0)
  {
    if (sfx_pointer_1_org != buffer) return;
    sfx_pointer_1_org = 0;
    sfx_status_1 = NOT_PLAYING;
    v_writePSG_double_buffered(0+8, 0);

    uint8_t enable = v_readPSG_double_buffered(7);
    // disable tone
    enable = enable | (1<<0); // channel 0
    // disable noise
    enable = enable | (1<<(3+0)); // channel 0
    v_writePSG_double_buffered(7, enable);
  }
  if (channel == 1)
  {
    if (sfx_pointer_2_org != buffer) return;
    sfx_pointer_2_org = 0;
    sfx_status_2 = NOT_PLAYING;
    v_writePSG_double_buffered(1+8, 0);
    uint8_t enable = v_readPSG_double_buffered(7);
    // disable tone
    enable = enable | (1<<1); // channel 0
    // disable noise
    enable = enable | (1<<(3+1)); // channel 0
    v_writePSG_double_buffered(7, enable);
  }
  if (channel == 2)
  {
    if (sfx_pointer_3_org != buffer) return;
    sfx_pointer_3_org = 0;
    sfx_status_3 = NOT_PLAYING;
    v_writePSG_double_buffered(2+8, 0);
    uint8_t enable = v_readPSG_double_buffered(7);
    // disable tone
    enable = enable | (1<<2); // channel 0
    // disable noise
    enable = enable | (1<<(3+2)); // channel 0
    v_writePSG_double_buffered(7, enable);
  }
}

int v_isPlayingSFX(int channel)
{
    if (channel == 0) return sfx_pointer_1_org!=0;
    if (channel == 1) return sfx_pointer_2_org!=0;
    if (channel == 2) return sfx_pointer_3_org!=0;
    return 0;
}
int v_isPlayingSFXSound(int channel, unsigned char *buffer)
{
    if (channel == 0) return sfx_pointer_1_org==buffer;
    if (channel == 1) return sfx_pointer_2_org==buffer;
    if (channel == 2) return sfx_pointer_3_org==buffer;
    return 0;
}

#undef PSG_CHANNEL
#undef SFX_STATUS
#undef SFX_POINTER
#undef SFX_POINTER_ORG
#define PSG_CHANNEL 0
#define SFX_STATUS sfx_status_1
#define SFX_POINTER sfx_pointer_1
#define SFX_POINTER_ORG sfx_pointer_1_org
// return 0 on finish or nothing
int play_sfx1()
{
//  printf("Playing 1");
//  printf("2 sfx_status_1 %d\n",sfx_status_1);
  if (!SFX_STATUS) return 0;

  uint8_t b = *SFX_POINTER++;

  if (b == 0xd0)
  {
    if ((*SFX_POINTER) == 0x20)
    {
      if (SFX_STATUS == PLAY_LOOP)
      {
        SFX_POINTER = SFX_POINTER_ORG;
        b = *SFX_POINTER++;
      }
      else
      {
        SFX_POINTER_ORG = 0;
        SFX_STATUS = 0;
        v_writePSG_double_buffered(PSG_CHANNEL+8, 0);
        uint8_t enable = v_readPSG_double_buffered(7);
        enable = enable | (1<<PSG_CHANNEL); // channel 0
        enable = enable | (1<<(3+PSG_CHANNEL)); // channel 0
        v_writePSG_double_buffered(7, enable);
        return 0;
      }
    }
  }
  if ((b & (1 << 5)) == (1 << 5))
  {
    // tone frequency
    v_writePSG_double_buffered(PSG_CHANNEL*2+0, *SFX_POINTER++);
    v_writePSG_double_buffered(PSG_CHANNEL*2+1, *SFX_POINTER++);
  }
  if ((b & (1 << 6)) == (1 << 6))
  {
    // noise frequency
    v_writePSG_double_buffered(6, *SFX_POINTER++);
  }
  uint8_t volume = b&0xf;
  volume = (uint8_t) (((float)volume)*sfx_mul_1);
  if (volume>0x0f) volume = 0x0f;
  v_writePSG_double_buffered(PSG_CHANNEL+8, volume);
  uint8_t enable = v_readPSG_double_buffered(7);
  if ((b & (1 << 4)) == (1 << 4))
  {
    // disable tone
    enable = enable | (1<<PSG_CHANNEL); // channel 0
  }
  else
  {
    // enable tone
    enable = enable & (0xff - (1<<PSG_CHANNEL)); // channel 0
  }

  if ((b & (1 << 7)) == (1 << 7))
  {
    // disable noise
    enable = enable | (1<<(3+PSG_CHANNEL)); // channel 0
  }
  else
  {
    // enable tone
    enable = enable & (0xff - (1<<(3+PSG_CHANNEL))); // channel 0
  }
  v_writePSG_double_buffered(7, enable);
  return 1;
}


#undef PSG_CHANNEL
#undef SFX_STATUS
#undef SFX_POINTER
#undef SFX_POINTER_ORG
#define PSG_CHANNEL 1
#define SFX_STATUS sfx_status_2
#define SFX_POINTER sfx_pointer_2
#define SFX_POINTER_ORG sfx_pointer_2_org
// return 0 on finish or nothing
int play_sfx2()
{
  if (!SFX_STATUS) return 0;
//  printf("Playing 2\n");

  uint8_t b = *SFX_POINTER++;

  if (b == 0xd0)
  {
    if ((*SFX_POINTER) == 0x20)
    {
      if (SFX_STATUS == PLAY_LOOP)
      {
        SFX_POINTER = SFX_POINTER_ORG;
        b = *SFX_POINTER++;
      }
      else
      {
        SFX_POINTER_ORG = 0;
        SFX_STATUS = 0;
        v_writePSG_double_buffered(PSG_CHANNEL+8, 0);
        uint8_t enable = v_readPSG_double_buffered(7);
        enable = enable | (1<<PSG_CHANNEL); // channel 0
        enable = enable | (1<<(3+PSG_CHANNEL)); // channel 0
        v_writePSG_double_buffered(7, enable);
        return 0;
      }
    }
  }
  if ((b & (1 << 5)) == (1 << 5))
  {
    // tone frequency
    v_writePSG_double_buffered(PSG_CHANNEL*2+0, *SFX_POINTER++);
    v_writePSG_double_buffered(PSG_CHANNEL*2+1, *SFX_POINTER++);
  }
  if ((b & (1 << 6)) == (1 << 6))
  {
    // noise frequency
    v_writePSG_double_buffered(6, *SFX_POINTER++);
  }
  uint8_t volume = b&0xf;
  volume = (uint8_t) (((float)volume)*sfx_mul_2);
  if (volume>0x0f) volume = 0x0f;
  v_writePSG_double_buffered(PSG_CHANNEL+8, volume);
  uint8_t enable = v_readPSG_double_buffered(7);
  if ((b & (1 << 4)) == (1 << 4))
  {
    // disable tone
    enable = enable | (1<<PSG_CHANNEL); // channel 0
  }
  else
  {
    // enable tone
    enable = enable & (0xff - (1<<PSG_CHANNEL)); // channel 0
  }

  if ((b & (1 << 7)) == (1 << 7))
  {
    // disable noise
    enable = enable | (1<<(3+PSG_CHANNEL)); // channel 0
  }
  else
  {
    // enable tone
    enable = enable & (0xff - (1<<(3+PSG_CHANNEL))); // channel 0
  }
  v_writePSG_double_buffered(7, enable);
  return 1;
}



#undef PSG_CHANNEL
#undef SFX_STATUS
#undef SFX_POINTER
#undef SFX_POINTER_ORG
#define PSG_CHANNEL 2
#define SFX_STATUS sfx_status_3
#define SFX_POINTER sfx_pointer_3
#define SFX_POINTER_ORG sfx_pointer_3_org
// return 0 on finish or nothing
int play_sfx3()
{
  if (!SFX_STATUS) return 0;
//  printf("Playing 3\n");

  uint8_t b = *SFX_POINTER++;

  if (b == 0xd0)
  {
    if ((*SFX_POINTER) == 0x20)
    {
      if (SFX_STATUS == PLAY_LOOP)
      {
        SFX_POINTER = SFX_POINTER_ORG;
        b = *SFX_POINTER++;
      }
      else
      {
        SFX_POINTER_ORG = 0;
        SFX_STATUS = 0;
        v_writePSG_double_buffered(PSG_CHANNEL+8, 0);
        uint8_t enable = v_readPSG_double_buffered(7);
        enable = enable | (1<<PSG_CHANNEL); // channel 0
        enable = enable | (1<<(3+PSG_CHANNEL)); // channel 0
        v_writePSG_double_buffered(7, enable);
        return 0;
      }
    }
  }
  if ((b & (1 << 5)) == (1 << 5))
  {
    // tone frequency
    v_writePSG_double_buffered(PSG_CHANNEL*2+0, *SFX_POINTER++);
    v_writePSG_double_buffered(PSG_CHANNEL*2+1, *SFX_POINTER++);
  }
  if ((b & (1 << 6)) == (1 << 6))
  {
    // noise frequency
    v_writePSG_double_buffered(6, *SFX_POINTER++);
  }
  uint8_t volume = b&0xf;
  volume = (uint8_t) (((float)volume)*sfx_mul_3);
  if (volume>0x0f) volume = 0x0f;
  v_writePSG_double_buffered(PSG_CHANNEL+8, volume);
  uint8_t enable = v_readPSG_double_buffered(7);
  if ((b & (1 << 4)) == (1 << 4))
  {
    // disable tone
    enable = enable | (1<<PSG_CHANNEL); // channel 0
  }
  else
  {
    // enable tone
    enable = enable & (0xff - (1<<PSG_CHANNEL)); // channel 0
  }

  if ((b & (1 << 7)) == (1 << 7))
  {
    // disable noise
    enable = enable | (1<<(3+PSG_CHANNEL)); // channel 0
  }
  else
  {
    // enable tone
    enable = enable & (0xff - (1<<(3+PSG_CHANNEL))); // channel 0
  }
  v_writePSG_double_buffered(7, enable);
  return 1;
}
#undef PSG_CHANNEL 
#undef SFX_STATUS
#undef SFX_POINTER
#undef SFX_POINTER_ORG

void v_playAllSFX()
{
//  printf("PALY ALL\n\r");
  play_sfx1();
  play_sfx2();
  play_sfx3();
}