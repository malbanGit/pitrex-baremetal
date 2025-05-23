
// ALSO in PIPELINE.I
#ifdef SUPPORT_SAMPLES
#define PLAY_SAMPLE() do {v_playOneSample();} while(0)
#define PLAY_SAMPLE_IGNORE_A() do {v_playOneSample_ignoreA();} while(0)
#else
#define PLAY_SAMPLE() 
#define PLAY_SAMPLE_IGNORE_A()
#endif



#ifdef MULTI_SAMPLES
#define SAMPLE_MAX 5

typedef struct {
    char *data;
    unsigned int pos;
    unsigned int size;
    int mode;
    int used;
} OneSample;

int v_multiSampleEnabled = 0;
OneSample allSamples[SAMPLE_MAX];
#endif

void *ptr;
  
char *v_sampleBuffer = 0;
int v_samplePosition = 0;
int v_sampleRate = 0;
int v_sampleSize = 0;
int v_sampleInterval = 0;
int v_sampleEnabled = 0;
int v_playMode = PLAY_ONCE;
uint32_t v_nextSampleAt=0;

void v_playIRQSample(char *buf, int size, int rate, int mode)
{
  if (size<=0) return;
  v_sampleEnabled = 1;
  v_sampleBuffer = buf;
  v_samplePosition = 0;
  v_sampleRate = rate;
  v_sampleSize = size;
  v_playMode = mode;
  v_sampleInterval = 1*1000*1000 / rate;
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}

#ifdef ENABLE_SAMPLE_MIXER
char *v_sample2Buffer = 0;
int v_sample2Position = 0;
int v_sample2Size = 0;
int v_sample2Enabled = 0;
int v_playMode2 = PLAY_ONCE;

void v_playIRQSample2(char *buf, int size, int rate, int mode)
{
  if (size<=0) return;
  v_sample2Enabled = 1;
  v_sample2Buffer = buf;
  v_sample2Position = 0;
  v_sample2Size = size;
  v_playMode2 = mode;
}
char *v_getSample2Playing()
{
  if (v_sample2Enabled == 0)return 0;
  return v_sample2Buffer;
}
static inline void v_playOneSample()
{
  int s=0;
  if ((!v_sampleEnabled) && (!v_sample2Enabled)) return;
  if (v_nextSampleAt > rpiSystemTimer->counter_lo) return;

  v_samplePosition++;
  if (v_samplePosition>=v_sampleSize) 
  {
    v_samplePosition = 0;
    if (v_playMode == PLAY_ONCE)
    {
      v_sampleEnabled =0;
      v_sampleBuffer =0;
    }
  }
  
  v_sample2Position++;
  if (v_sample2Position>=v_sample2Size) 
  {
    v_sample2Position = 0;
    if (v_playMode2 == PLAY_ONCE)
    {
      v_sample2Enabled =0;
      v_sample2Buffer =0;
    }
  }
  if ((v_sampleBuffer== 0) && (v_sample2Buffer== 0)) return;

  if (v_sample2Buffer!=0)
    s +=  (signed int) ( (signed char)( *(v_sample2Buffer+v_sample2Position)));
  if (v_sampleBuffer!=0)
    s +=  (signed int) ( (signed char)( *(v_sampleBuffer+v_samplePosition)));

  if (s>127) s = 127;
  if (s<-128) s = -128;

  unsigned char a = lastPortA;
  I_SET_A((signed char) s);
  I_SET(VIA_port_b, 0x86);
  I_SET(VIA_port_b, 0x81);
  ADD_DELAY_CYCLES(2);
  I_SET_A(a);
  ADD_DELAY_CYCLES(2);
  
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}
static inline void v_playOneSample_ignoreA()
{
  int s=0;
  if ((!v_sampleEnabled) && (!v_sample2Enabled)) return;
  if (v_nextSampleAt > rpiSystemTimer->counter_lo) return;

//printf("Play sample!\n");  
  
  v_samplePosition++;
  if (v_samplePosition>=v_sampleSize) 
  {
    v_samplePosition = 0;
    if (v_playMode == PLAY_ONCE)
    {
      v_sampleEnabled =0;
      v_sampleBuffer =0;
    }
  }
  
  v_sample2Position++;
  if (v_sample2Position>=v_sample2Size) 
  {
    v_sample2Position = 0;
    if (v_playMode2 == PLAY_ONCE)
    {
      v_sample2Enabled =0;
      v_sample2Buffer =0;
    }
  }
  if ((v_sampleBuffer== 0) && (v_sample2Buffer== 0)) return;

  if (v_sample2Buffer!=0)
    s +=  (signed int) ( (signed char)( *(v_sample2Buffer+v_sample2Position)));
  if (v_sampleBuffer!=0)
    s +=  (signed int) ( (signed char)( *(v_sampleBuffer+v_samplePosition)));

  if (s>127) s = 127;
  if (s<-128) s = -128;
  
  I_SET_A((signed char) s);
  I_SET(VIA_port_b, 0x86);
  I_SET(VIA_port_b, 0x81);
  ADD_DELAY_CYCLES(2);
  
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}
#else // ENABLE_SAMPLE_MIXER


#ifdef MULTI_SAMPLES

static inline int hasSamplePending_m()
{
  int s=0;
  if (!v_multiSampleEnabled) return 0;

  for (int i=0;i<SAMPLE_MAX; i++)
  {
      if (allSamples[i].used == 1)
      {
        allSamples[i].pos++;
        if (allSamples[i].pos>=allSamples[i].size) 
        {
          allSamples[i].pos = 0;
          if (allSamples[i].mode == PLAY_ONCE)
          {
            allSamples[i].used =0;
          }
        }
        else
        {
          s += (signed int) ( (signed char)( *(allSamples[i].data+allSamples[i].pos)));
        }
      }
  }
  return abs(s)>50;  
}

static inline void v_playOneSample_m()
{
  int s=0;
  if (!v_multiSampleEnabled)return;
  if (v_nextSampleAt > rpiSystemTimer->counter_lo) return;

  for (int i=0;i<SAMPLE_MAX; i++)
  {
      if (allSamples[i].used == 1)
      {
        allSamples[i].pos++;
        if (allSamples[i].pos>=allSamples[i].size) 
        {
          allSamples[i].pos = 0;
          if (allSamples[i].mode == PLAY_ONCE)
          {
            allSamples[i].used =0;
          }
        }
        else
        {
          s += (signed int) ( (signed char)( *(allSamples[i].data+allSamples[i].pos)));
        }
      }
  }
  if (s>127) s = 127;
  if (s<-128) s = -128;

  
//  s = 0;

// it is not the value that is written to port a
// that is causing the shaking in SMP
// it is calling this function AT ALL.  
  
  unsigned char a = lastPortA;

  I_SET_A((signed char) s);
  I_SET(VIA_port_b, 0x86);
  I_SET(VIA_port_b, 0x81);
  ADD_DELAY_CYCLES(2);
  I_SET_A(a);
  ADD_DELAY_CYCLES(2);
  
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}

static inline void v_playOneSample_ignoreA_m()
{
  // hack
  // cant remember for what... word war vi?
  v_playOneSample_m(); return;

  int s=0;
  if (!v_multiSampleEnabled)return;
  if (v_nextSampleAt > rpiSystemTimer->counter_lo) return;

  for (int i=0;i<SAMPLE_MAX; i++)
  {
      if (allSamples[i].used == 1)
      {
        allSamples[i].pos++;
        if (allSamples[i].pos>=allSamples[i].size) 
        {
          allSamples[i].pos = 0;
          if (allSamples[i].mode == PLAY_ONCE)
          {
            allSamples[i].used =0;
          }
        }
        else
        {
          s +=  (signed int) ( (signed char)( *(allSamples[i].data+allSamples[i].pos)));
        }
      }
  }
  if (s>127) s = 127;
  if (s<-128) s = -128;
  
  I_SET_A((signed char) s);
  I_SET(VIA_port_b, 0x86);
  I_SET(VIA_port_b, 0x81);
  ADD_DELAY_CYCLES(2);
  
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}
#endif // MULTI_SAMPLES

static inline void v_playOneSample()
{
#ifdef MULTI_SAMPLES
  if (v_multiSampleEnabled)
  {
    v_playOneSample_m();
    return;
  }
#endif  
  if (!v_sampleEnabled) return;
  if (v_nextSampleAt > rpiSystemTimer->counter_lo) return;

  v_samplePosition++;
  if (v_samplePosition>=v_sampleSize) 
  {
    v_samplePosition = 0;
    if (v_playMode == PLAY_ONCE)
    {
      v_sampleEnabled =0;
      v_sampleBuffer =0;
      return;
    }
  }
  unsigned char a = lastPortA;
  I_SET_A(*(v_sampleBuffer+v_samplePosition));
  I_SET(VIA_port_b, 0x86);
  I_SET(VIA_port_b, 0x81);
  ADD_DELAY_CYCLES(2);
  I_SET_A(a);
  ADD_DELAY_CYCLES(2);
  
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}
static inline void v_playOneSample_ignoreA()
{
#ifdef MULTI_SAMPLES
  if (v_multiSampleEnabled)
  {
    v_playOneSample_ignoreA_m();
    return;
  }
#endif  
  if (!v_sampleEnabled) return;
  if (v_nextSampleAt > rpiSystemTimer->counter_lo) return;

  v_samplePosition++;
  if (v_samplePosition>=v_sampleSize) 
  {
    v_samplePosition = 0;
    if (v_playMode == PLAY_ONCE)
    {
      v_sampleEnabled =0;
      v_sampleBuffer =0;
      return;
    }
  }
  
  I_SET_A(*(v_sampleBuffer+v_samplePosition));
  I_SET(VIA_port_b, 0x86);
  I_SET(VIA_port_b, 0x81);
  ADD_DELAY_CYCLES(2);
  
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}
#endif

// returns startAddress of current sample / or 0
char *v_getSamplePlaying()
{
  if (v_sampleEnabled == 0) return 0;
  return v_sampleBuffer;
}
void v_stopSamplePlaying()
{
  v_sampleEnabled=0;
  v_sampleBuffer =0;
#ifdef MULTI_SAMPLES
  v_multiSampleEnabled=0;
#else
#ifdef ENABLE_SAMPLE_MIXER
  v_sample2Enabled=0;
  v_sample2Buffer =0;
#endif  
#endif  
}

#ifdef MULTI_SAMPLES
void initMultiSamples(int rate)
{
  for (int i=0;i<SAMPLE_MAX; i++)
    allSamples[i].used = 0;
  v_multiSampleEnabled = 1;
  v_sampleEnabled = 0;

  v_sampleInterval = 1*1000*1000 / rate;
  v_nextSampleAt = rpiSystemTimer->counter_lo + v_sampleInterval;
}
void deinitMultiSamples()
{
  v_multiSampleEnabled = 0;
}
void v_addIRQMultiSample(char *buf, int size, int mode)
{ 
  if (size<=0) return;

  for (int i=0;i<SAMPLE_MAX; i++)
  {
      if (allSamples[i].used == 0) 
      {
        allSamples[i].used = 1;
        allSamples[i].pos = 0;
        allSamples[i].data = buf;
        allSamples[i].size = size;
        allSamples[i].mode = mode;
        break;
      }
  }
}
#else
void initMultiSamples(int rate)
{
}
void deinitMultiSamples()
{
}
void v_addIRQMultiSample(char *buf, int size, int mode)
{
}
#endif

void v_playDirectSampleAll(char *ymBufferLoad, int fsize, int rate);

// does play a complete sample on vectrex, does not return in between... nothing is displayed!
// sample rate is e.g. 15000
// one sample each 0,000066 seconds
// = 0,0666 milli seconds
// = each 66 micro seconds
void v_playDirectSampleAll(char *ymBufferLoad, int fsize, int rate)
{
  if (isIRQMode) return;
#if RASPPI != 1 
  if (isSMPMode) return;
#endif  
  int counter = 0;
  int microsToWait = (1000*1000) / rate;

  I_SET_A(0);
  ADD_DELAY_CYCLES(4);

  I_SET(VIA_port_b, 0x86);
  ADD_DELAY_CYCLES(4);
  while (counter < fsize)
  {
    char b = *ymBufferLoad++;
    I_SET_A(b);
    int endTime = v_micros()+microsToWait;
    counter++;
    while (v_micros()<endTime) ;
  }
  I_SET(VIA_port_b, 0x81);
  ADD_DELAY_CYCLES(4);
}