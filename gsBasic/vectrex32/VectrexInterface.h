/* 
 * File:   VectrexInterface.h
 * Author: Bob
 *
 * Created on February 13, 2016, 1:23 PM
 */

#ifndef VECTREXINTERFACE_H
#define	VECTREXINTERFACE_H
 
#include <stdint.h>
//#include "../../SmartCart/SmartCart/firmware/src/Debugging.h"
#include "CommandDataQueue.h"

#define END_COMPILING_CODE_SIZE 3

#ifdef __cplusplus
extern "C"
{
#endif
    
void InitializeVectrexInterface(uint32_t majorVersion, uint32_t minorVersion, int hwversion, int firstHWVersionUsing6522Signal);
void VectrexInterruptReceived();
void VectrexSetMusicFlagReceivedCallback(void (*callback)());
void VectrexSetExecutionDoneCallback(void (*callback)());

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <stddef.h>
#include <stdint.h>
#include <string.h>
//#include "VectrexWriter.h"

struct __attribute__ ((__packed__)) Point
{
    int8_t y, x;
};

struct __attribute__ ((__packed__)) PatternLine
{
    uint8_t pattern;
    int8_t y, x;
};

struct __attribute__ ((__packed__)) Text
{
    int8_t y, x;
    char text[];
};

class VectrexInterface 
{
public:
    static VectrexInterface &Singleton() {return *_vi;}
    
    struct __attribute__ ((__packed__)) SoundByte
    {
        int8_t reg, data;
    };

    ~VectrexInterface();
    
    typedef enum {UNINITIALIZED, INITIALIZED, VECTREXWAITING, VECTREXEXECUTING} State_t;
    typedef enum {Highest = 0, High = 1, Good = 2, OK = 4, MediumHigh = 8, 
                MediumLow = 16, Coarse = 32, Rough = 64, Lowest = 128,
                Invalid = 255} JoystickResolution;
    ////////////////////////////////////////////////////////////////////////////
    // Miscellaneous
    typedef enum
    {
        Berzerk = 0xFD1D,
        MelodyMaster = 0xFEF8,
        Music1 = 0xFD81,
        Music2 = 0xFE76,
        Music3 = 0xFEC6,
        Music4 = 0xFF26,
        Music5 = 0xFF44,
        Music6 = 0xFF62,
        Music7 = 0xFF7a,
        Music8 = 0xFF8F,
        PowerOn = 0xFD0D,
        Scramble = 0xFDD3,
        SolarQuest = 0xFE38,
    } Tunes;
    
    typedef enum
    {
        BerzerkTwang = 0xFD79,
        FlatTwang = 0xFEB6,
        ScrambleTwang = 0xFD79,
    } Twangs;
    
    typedef enum
    {
        BerserkADSR = 0xFD69,
        MelodyMasterADSR = 0xFF16,
        Music1ADSR = 0xFDC3,
        Music2ADSR = 0xFEB2,
        Music3ADSR = 0xFEE8,
        Music4ADSR = 0xFE28,
        Music5ADSR = 0xFE28,
        Music6ADSR = 0xFE28,
        Music7ADSR = 0xFD69,
        Music8ADSR = 0xFDC3,
        ScrambleADSR = 0xFE28,
        SolarQuestADSR = 0xFE66,
    } ADSRs;

    // Musical notes
    typedef enum
    {
        G2  = 0x00, //G = 1.5 octaves below middle C
        GS2 = 0x01, // G sharp (second octave) , etc.
        A2  = 0x02,
        AS2 = 0x03,
        B2  = 0x04,
        C3  = 0x05,
        CS3 = 0x06,
        D3  = 0x07,
        DS3 = 0x08,
        E3  = 0x09,
        F3  = 0x0A,
        FS3 = 0x0B,
        G3  = 0x0C,
        GS3 = 0x0D,
        A3  = 0x0E,
        AS3 = 0x0F,
        B3  = 0x10,
        C4  = 0x11,
        CS4 = 0x12,
        D4  = 0x13,
        DS4 = 0x14,
        E4  = 0x15,
        F4  = 0x16,
        FS4 = 0x17,
        G4  = 0x18,
        GS4 = 0x19,
        A4  = 0x1A,
        AS4 = 0x1B,
        B4  = 0x1C,
        C5  = 0x1D,
        CS5 = 0x1E,
        D5  = 0x1F,
        DS5 = 0x20,
        E5  = 0x21,
        F5  = 0x22,
        FS5 = 0x23,
        G5  = 0x24,
        GS5 = 0x25,
        A5  = 0x26,
        AS5 = 0x27,
        B5  = 0x28,
        C6  = 0x29,
        CS6 = 0x2A,
        D6  = 0x2B,
        DS6 = 0x2C,
        E6  = 0x2D,
        F6  = 0x2E,
        FS6 = 0x2F,
        G6  = 0x30,
        GS6 = 0x31,
        A6  = 0x32,
        AS6 = 0x33,
        B6  = 0x34,
        C7  = 0x35,
        CS7 = 0x36,
        D7  = 0x37,
        DS7 = 0x38,
        E7  = 0x39,
        F7  = 0x3A,
        FS7 = 0x3B,
        G7  = 0x3C,
        GS7 = 0x3D,
        A7  = 0x3E,
        AS7 = 0x3F
    } MusicalNote;
    
private:
    friend void InitializeVectrexInterface(uint32_t majorVersion, uint32_t minorVersion, int hwversion, int firstHWVersionUsing6522Signal);
    friend void VectrexInterruptReceived();
    
    VectrexInterface(uint32_t majorVersion, uint32_t minorVersion, int hwversion, int firstHWVersionUsing6522Signal);
    VectrexInterface(const VectrexInterface& orig);
    
    void InterruptReceived();
    
    static VectrexInterface *_vi;
    volatile State_t _state;
    
    uint8_t _buttonData;
    uint16_t _joystickData[2];
    mutable uint8_t _musicFlag;
    
    void (*_buttonsCallback)(void *);
    void (*_joysticksCallback)(void *);
    void (*_musicFlagReceivedCallback)();
    void (*_executionDoneCallback)();
    void *_buttonsParam, *_joysticksParam, *_executionDoneParam;
    
    mutable int _peekBlockSize;
    mutable uint8_t _peekData[16];
    mutable void (*_peekCallback)(uint8_t data[]);
    
    char _dualPortRAMError[50];
    
    uint8_t _soundRegisters[16];
    uint32_t _modifiedSoundRegisters;
    
    mutable uint8_t _vectrexScale;
};

#endif

#endif	/* VECTREXINTERFACE_H */

