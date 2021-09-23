/* 
 * File:   VectrexSound.h
 * Author: Bob
 *
 * Created on May 10, 2016, 7:57 PM
 */

#ifndef VECTREXSOUND_H
#define	VECTREXSOUND_H

#include "../Value.h"
#include "VectrexInterface.h"

class VectrexMusic : public GSDataObject
{
public:
    VectrexMusic() {}
    virtual ~VectrexMusic() {}
    
private:
    VectrexMusic(const VectrexMusic& orig);
};

class VectrexMusicBuiltin : public VectrexMusic
{
public:
    VectrexMusicBuiltin(VectrexInterface::Tunes tune) :
        _tune(tune) {}
        
protected:
    VectrexMusicBuiltin() : _tune(VectrexInterface::Tunes(0)) {}
    VectrexInterface::Tunes _tune;
};

class VectrexMusicNotes : public VectrexMusicBuiltin
{
public:
    VectrexMusicNotes(VectrexInterface::ADSRs adsr, VectrexInterface::Twangs twang, const Value &notes);
    ~VectrexMusicNotes();
    
private:
    uint16_t _dpramAddress;
};

class VectrexExplosion : public VectrexMusicBuiltin
{
public:
    VectrexExplosion(int byte0, int byte1, int byte2, int byte3);
    ~VectrexExplosion();
    
private:
    uint16_t _dpramAddress;
};

class VMusic : public GSDataObject
{
public:

  VMusic() {}
  VMusic(unsigned char *a, signed char *t, int s, unsigned char* m)
  : adsr(a), twang(t), size(s), music(m)
  {}

  unsigned char* adsr;  // pointer to 16 byte
  signed char* twang; // pointer to 16 byte
  int size;
  unsigned char* music;// arbitrary number of bytes
};

class VYMMusic : public GSDataObject
{
public:
  VYMMusic(unsigned char *filename);
  ~VYMMusic()
  {
    if (loaded) delete data;
    loaded = 0;
  }
  bool isLoaded(){return loaded;}
  unsigned char* getData(){return data;}
  int getSize(){return size;}  // in VBL not byte!
private:
  unsigned char* data;// arbitrary number of bytes
  unsigned int size;
  bool loaded;
};

class VSample : public GSDataObject
{
public:
  VSample(unsigned char *filename, int r);
  ~VSample()
  {
    if (loaded) delete data;
    loaded = 0;
  }
  bool isLoaded(){return loaded;}
  unsigned char* getData(){return data;}
  int getRate(){return rate;}
  int getSize(){return size;}
private:
  int rate;
  unsigned char* data;// arbitrary number of bytes
  unsigned int size;
  bool loaded;
};

// this is a simply container
class VAYSFX : public GSDataObject
{
public:
  VAYSFX(Value a) : aysfx(a), data(0) 
  {
    int dataI[aysfx.NumberOfElements()];
    ErrorCode error = aysfx.GetArrayValues(dataI);
    if (error) 
    {
      printf("AYFX init, Array extract error!");
      return;
    }
    data = new unsigned char[aysfx.NumberOfElements()];
    for (int i=0;i<aysfx.NumberOfElements();i++)
    {
      data[i] = (unsigned char)dataI[i];
    }
  }
  ~VAYSFX()
  {
    if (data) delete data;
    data = 0;
  }
  unsigned char* getData(){return data;}
  

  
private:
  Value aysfx;
  unsigned char *data;
};




extern VMusic VS_PowerOnMusic;
extern VMusic VS_BerzerkMusic;
extern VMusic VS_MelodyMasterMusic;
extern VMusic VS_Music1Music;
extern VMusic VS_Music2Music;
extern VMusic VS_Music3Music;
extern VMusic VS_Music4Music;
extern VMusic VS_Music5Music;
extern VMusic VS_Music6Music;
extern VMusic VS_Music7Music;
extern VMusic VS_Music8Music;
extern VMusic VS_ScrambleMusic;
extern VMusic VS_SolarQuestMusic;

extern unsigned char PowerOnADSR[];
extern signed char PowerOnTwang[];

// it doesn't realy make sense to have more than one of 
// these at any time
// since vectrex can only play ONE music at any given time...
class VectrexMusicPlayer : public GSDataObject
{
public:

    VectrexMusicPlayer(VMusic *m);
    VectrexMusicPlayer(const Value &notes, unsigned char* a=PowerOnADSR, signed char *t=PowerOnTwang);
    VectrexMusicPlayer(int b0, int b1, int b2, int b3):byte0(b0),byte1(b1),byte2(b2),byte3(b3), 
                                                      isExplosion(1), is_Playing(0), Vec_Music_Flag(0), 
                                                      explosionStarted(0), explosionFinished(0), Vec_Expl_Flag(-1) 
                                                      {}

    virtual ~VectrexMusicPlayer() 
    {
      is_Playing = 0;
      Vec_Music_Flag = 0;
      if (music != 0)
      {
        if (music->music != 0)
          delete [] music->music;
        delete [] music;
      }
    }
    
    int play() 
    { 
      if (isExplosion == 0)
      {
        if ((is_Playing) && (Vec_Music_Flag!=0)) 
          Init_Music(); 
        return Vec_Music_Flag;
      }
      if (explosionFinished) 
        return 0;
      playExplosion();
      return explosionFinished;
    }
    
    
    void setPlaying(bool b)
    {
      if (b)
      {
        is_Playing = 1;
        explosionFinished = 0;
        explosionStarted = 0;
        Vec_Expl_Flag = -1;
        Vec_Music_Flag = 1;
      }
      else
      {
        stop();
      }
    }

    bool isPlaying()
    {
      return is_Playing;
    }
    void stop() 
    {
      is_Playing = 0;
      Vec_Expl_Flag = -1;
      Vec_Music_Flag = 1;
      explosionFinished = 1;
      explosionStarted = 0;
      v_noSound();
    }
    
private:
  void Init_Music();
  void playExplosion();

  VMusic *music=0;
  bool is_Playing = false;
  int Vec_Music_Flag;     // no sound
                          // 1 new sound
                          // -1 sound in progress
  int Vec_Music_Chan=0;
  unsigned char *Vec_Music_Ptr = 0;

  unsigned char Vec_ADSR_Timers[3];
  unsigned char Vec_Music_Twang_Count[3];

  signed char Vec_Music_Twang_Value[3];
  signed char Vec_Duration;
  int Vec_Music_Freq[3];
  
  unsigned char Vec_ADSR_Table[16];
  signed char Vec_Twang_Table[16];
  
  // explosion data follows

  bool isExplosion;  
  bool explosionStarted;
  bool explosionFinished;
  int byte0; // 00BCAbca channels used in effect ABC = noise, abc = sound
  int byte1; // frequency sweep up (0), down (1), stay (-1)
  int byte2; // volume sweep down (0), up (1), stay (-1)
  int byte3; // determines length of "explosion", the timer is decreased by this values, thus the higher the shorter the sound
  
  signed char Vec_Expl_Flag; // positive, explosion is running, negative explosion must be init
  unsigned char Vec_Expl_1[4];
  unsigned char Vec_Expl_Chans; // contains channels that are used (either sound/noise) 0000 0abc, bit values of a,b,c
  unsigned char Vec_Expl_ChanA; // contains NOISE channels used in explosion 00cb a000 , bit values of a,b,c
  unsigned char Vec_Expl_ChanB; // contains SOUND channels using in explosion 0000 0cba, bitvalues of a,b,c
  unsigned char Vec_Expl_Chan;  // loop counter for channels 0 or 1 or 2
  signed char Vec_Expl_Timer;   // timer of explosion sound, starts always with 0x7f, and is decreased each round by "byte3"
                                // devided by 4, this also provides the noise peroid for the PSG (Reg6) (that is frequency swwop)
                                // devided by 8, this also provides the volume for the PSG (Reg8,9,10) (that is volume swwop)
                                
};



#endif	/* VECTREXSOUND_H */

