/* 
 * File:   VectrexSound.cpp
 * Author: Bob
 * 
 * Created on May 10, 2016, 7:57 PM
 */
#include <stdlib.h>

#include "VectrexSound.h"
#include "../Array.h"
#include "CommandDataQueue.h"

#ifdef __cplusplus
extern "C" {
#endif
char *ymBASICBuffer=0; 
char *ymBASICBufferLoad=0; 

  #include "ymStuff.i"
#ifdef __cplusplus
}
#endif


const static unsigned char Bit_Masks[8] ={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

//Music_Table_1:  FCB     $F7,$EF,$DF,$01,$02,$04         ;For noise
const static unsigned char Music_Table_1[] =
{
  0xF7, 0xEF, 0xDF, 0x01, 0x02, 0x04         //;For noise
};

//Music_Table_2:  FCB     $FE,$FD,$FB,$08,$10,$20         ;For music
const static unsigned char Music_Table_2[] =
{
  0xFE, 0xFD, 0xFB, 0x08, 0x10, 0x20        // ;For music
};

// these are in big endian (stored by the ARM in little endian)
const static unsigned int Freq_Table[]=
{
 0x03BD,0x0387,0x0354,0x0324,0x02F7,0x02CD,0x02A4,0x027E,
 0x025B,0x0239,0x0219,0x01FB,0x01DE,0x01C3,0x01AA,0x0192,
 0x017C,0x0166,0x0152,0x013F,0x012D,0x011C,0x010C,0x00FD,
 0x00EF,0x00E2,0x00D5,0x00C9,0x00BE,0x00B3,0x00A9,0x00A0,
 0x0097,0x008E,0x0086,0x007F,0x0078,0x0071,0x006B,0x0065,
 0x005F,0x005A,0x0055,0x0050,0x004B,0x0047,0x0043,0x003F,
 0x003C,0x0038,0x0035,0x0032,0x002F,0x002D,0x002A,0x0028,
 0x0026,0x0024,0x0022,0x0020,0x001E,0x001C,0x001B,0x0000
};

//       FEE8 = ADSR table for FEC6
//FDB     $EEFF,$FFEE,$EEDD,$CCBB,$AA99,$8888,$8888,$8888
unsigned char PowerOnADSR[] =
{
  0xEE, 0xFF ,0xFF, 0xEE, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88
};
//       FEB6 = "flat" twang table
//          FDB     $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
signed char PowerOnTwang[] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
};

unsigned char PowerOnData[] =
  {
    0x93, 0x1F, 0x0C, 
    0x93, 0x1F, 0x06, 
    0x98, 0x9F, 0x24, 0x3C, 
    0x11, 0x80
  };

//;       FD69 = ADSR table for Berzerk and FF7A
unsigned char DFD69_ADSR[] =
{
     0xEF,0xFF,0xFE,0xDC,0xBA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

//;       FD79 = twang table for Berzerk and Scramble
signed char DFD79_TWANG[] =
{
     0x00,0x01,0x02,0x01,0x00,(signed char)0xFF,(signed char)0xFE,(signed char)0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
  
  
// Berzerk
unsigned char BerzerkData[] =
{
    0x21,0x07,0x21,0x07,0x21,0x07,0x21,0x07,0x21,0x07,0x21,0x07, // 12
    0x21,0x0E,0x99,0x9F,0x24,0x0E,0x95,0x9B,0x20,0x0E,0x21,0x07,0x21,0x07,0x21,0x07, // 16
    0x21,0x07,0x21,0x07,0x21,0x07,0x9D,0xA3,0x28,0x0E,0xA0,0xA6,0x2B,0x0E,0x22,0x02, // 16
    0x28,0x02,0x2D,0x02,0x28,0x02,0x22,0x02,0x28,0x02,0x2D,0x02,0x28,0x02,0x22,0x02, // 16
    0x28,0x02,0x2D,0x02,0x28,0x02,0x2E,0x02,0x2D,0x28,0x21,0x80  // 12
};
  
  
//;       FEF8 = music for Melody Master
unsigned char MelodyMasterData[] =
{
     0x1C,0x06,0x1F,0x06,0x1C,0x06,0x18,0x06,0x1A,0x06,0x18,0x06, // 12
     0x15,0x06,0x13,0x06,0x18,0x06,0x13,0x06,0x17,0x06,0x18,0x1E,0x18,0x80 // 14
};

//;       FF16 = ADSR table for FEF8
unsigned char DFF16_ADSR[] =
{
    0xFF,0xFF,0xEE,0xEE,0xDD,0xDD,0xCC,0xCC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

unsigned char ScrambleData[] =
{
   0x98,0x1C,0x10,0x3F,0x08,0x98,0x1C,0x04,0x98,0x1C,0x04,0x98, // 12
   0x1C,0x10,0x3F,0x08,0x98,0x1C,0x04,0x98,0x1C,0x04,0x98,0x1C,0x08,0x93,0x18,0x08, // 16
   0x98,0x1C,0x08,0x9C,0x1F,0x08,0x98,0x1C,0x08,0x93,0x18,0x08,0x98,0x1C,0x08,0x93, // 16
   0x18,0x08,0x98,0x1C,0x08,0x9C,0x1F,0x08,0x98,0x1C,0x08,0x93,0x18,0x08,0x98,0x1C, // 16
   0x08,0x93,0x18,0x08,0x98,0x1C,0x08,0x9C,0x1F,0x08,0x98,0x1C,0x08,0x93,0x18,0x08, // 16
   0x9C,0x1F,0x30,0x1A,0x80 // 5
};

//;       FE28 = ADSR table for Scramble, FF26, FF44, FF62
unsigned char DFE28_ADSR[] =
{
     0xFF,0xFE,0xDC,0xBA,0x98,0x76,0x54,0x32,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

//;       FE38 = music for Solar Quest
unsigned char SolarQuestData[] =
{
     0x0C,0x18,0x11,0x18,0x0C,0x18,0x11,0x18,0x0C,0x18,0x11,0x18, // 12
     0x0C,0x12,0x0C,0x06,0x11,0x18,0x9D,0x21,0x18,0x9F,0x23,0x18,0xA1,0x24,0x18,0xA3, // 16
     0x26,0x18,0x9F,0xA4,0x28,0x18,0x07,0x12,0x07,0x06,0x00,0x3C,0x18,0x80 // 14
};

//;       FE66 = ADSR table for Solar Quest
unsigned char DFE66_ADSR[] =
{
     0xDE,0xEF,0xFE,0xDC,0xBA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

//;       FD81 = music
unsigned char Music1Data[] =
{
     0x51,0x24,0x50,0x06,0x50,0x06,0x50,0x0C,0x50,0x06,0x50,0x06, // 12
     0x50,0x04,0x50,0x04,0x50,0x04,0x50,0x18,0x50,0x04,0x50,0x04,0x50,0x04,0x50,0x0C, // 16
     0x50,0x0C,0x50,0x24,0x50,0x06,0x50,0x06,0x50,0x0C,0x50,0x06,0x50,0x06,0x50,0x04, // 16
     0x50,0x04,0x50,0x04,0x50,0x18,0x50,0x04,0x50,0x04,0x50,0x04,0x50,0x0C,0x50,0x18, // 16
     0x26,0x80 // 2
};

//;       FDC3 = ADSR table for FD81 and FF8F
unsigned char DFDC3_ADSR[] =
{
     0xFD,0xBA,0x98,0x76,0x55,0x44,0x33,0x22,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

//;       FE76 = music
unsigned char Music2Data[] =
{
     0x18,0x06,0x1A,0x06,0x1C,0x0C,0x18,0x0C,0x1A,0x24,0x23,0x18, // 12
     0x17,0x06,0x18,0x06,0x1A,0x0C,0x17,0x0C,0x18,0x24,0x24,0x18,0xA4,0x28,0x0C,0xA3, // 16
     0x26,0x0C,0xA1,0x24,0x0C,0x9F,0x23,0x0C,0x9D,0x21,0x18,0x9A,0x1F,0x18,0x17,0x06, // 16
     0x18,0x06,0x1A,0x0C,0x17,0x0C,0x18,0x24,0x24,0x24,0x18,0x80 // 12
};

//;       FEB2 = ADSR table for FE76
unsigned char DFEB2_ADSR[] =
{
     0xFF,0xEE,0xDD,0xCC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

//;       FEC6 = music
unsigned char Music3Data[] =
{
     0x96,0x9A,0x1D,0x1E,0x91,0x95,0x18,0x1E,0x94,0x98,0x1B,0x1E, // 12
     0x8F,0x94,0x18,0x14,0x16,0x0A,0x8C,0x91,0x15,0x14,0x16,0x0A,0x91,0x95,0x18,0x32, // 16
     0x18,0x80 // 2
};

//;       FEE8 = ADSR table for FEC6
unsigned char DFEE8_ADSR[] =
{
     0xEE,0xFF,0xFF,0xEE,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x88,0x88,0x88,0x88,0x88
};

//;       FF26 = music
unsigned char Music4Data[] =
{
     0x16,0x0F,0x16,0x05,0x16,0x05,0x16,0x05,0x1A,0x0F,0x16,0x0F, // 12
     0x1D,0x0F,0x1D,0x05,0x1D,0x05,0x1D,0x05,0x21,0x0F,0x1D,0x32,0x1D,0x80 // 14
};

//;       FF44 = music
unsigned char Music5Data[] =
{
     0x16,0x06,0x16,0x02,0x16,0x02,0x16,0x02,0x1A,0x06,0x16,0x06, // 12
     0x1D,0x06,0x1D,0x02,0x1D,0x02,0x1D,0x02,0x21,0x06,0x1D,0x32,0x11,0x80 // 14
};

// ;       FF62 = music
unsigned char Music6Data[] =
{
     0x1B,0x0F,0x16,0x05,0x16,0x05,0x16,0x05,0x17,0x30,0x16,0x05, // 12
     0x16,0x05,0x16,0x05,0x17,0x30,0x16,0x80 // 8
};

//;       FF7A = music
unsigned char Music7Data[] =
{
     0xA0,0x23,0x12,0xA0,0x23,0x0C,0x9C,0x20,0x06,0x9E,0x21,0x12, // 12
     0x9C,0x20,0x32,0x13,0x80 // 5
};

//;       FF8F = music
unsigned char Music8Data[] =
{
     0x16,0x04,0x16,0x04,0x16,0x04,0x16,0x04,0x1A,0x08,0x1C,0x80 // 12
};
  
VMusic VS_PowerOnMusic(PowerOnADSR, PowerOnTwang, 12, PowerOnData);
VMusic VS_BerzerkMusic(DFD69_ADSR, DFD79_TWANG, 16+16+16+12+12, BerzerkData);
VMusic VS_MelodyMasterMusic(DFF16_ADSR, PowerOnTwang, 12+14, MelodyMasterData);
VMusic VS_ScrambleMusic(DFE28_ADSR, DFD79_TWANG, 16+16+16+16+12+5, ScrambleData);
VMusic VS_SolarQuestMusic(DFE66_ADSR, PowerOnTwang, 16+14+12, SolarQuestData);
VMusic VS_Music1Music(DFDC3_ADSR, PowerOnTwang, 16+16+16+12+2, Music1Data);
VMusic VS_Music2Music(DFEB2_ADSR, PowerOnTwang, 16+16+12+12, Music2Data);
VMusic VS_Music3Music(DFEE8_ADSR, PowerOnTwang, 16+12+2, Music3Data);
VMusic VS_Music4Music(DFE28_ADSR, PowerOnTwang, 12+14, Music4Data);
VMusic VS_Music5Music(DFE28_ADSR, PowerOnTwang, 12+14, Music5Data);
VMusic VS_Music6Music(DFE28_ADSR, PowerOnTwang, 12+8, Music6Data);
VMusic VS_Music7Music(DFD69_ADSR, PowerOnTwang, 12+5, Music7Data);
VMusic VS_Music8Music(DFDC3_ADSR, PowerOnTwang, 12, Music8Data);

VectrexMusicPlayer::VectrexMusicPlayer(VMusic *m) : Vec_Music_Flag(1), isExplosion(0)
{
  music = new VMusic();  
  // ATTENTION: no deep copy!
  music->adsr = m->adsr;  // pointer to 16 byte
  music->twang = m->twang; // pointer to 16 byte
  music->size = m->size; // pointer to 16 byte
  music->music = new uint8_t[music->size+2];
  for (int i=0;i<music->size; i++)
    music->music[i] = m->music[i];
  
  for (int i=0;i<16;i++)
  {
    Vec_ADSR_Table[i] = m->adsr[i];
    Vec_Twang_Table[i] = m->twang[i];
  }
}


VectrexMusicPlayer::VectrexMusicPlayer(const Value &notes, unsigned char* a, signed char *t) : Vec_Music_Flag(1), isExplosion(0)
{
  music = new VMusic();  
  music->adsr = a;  // pointer to 16 byte
  music->twang = t; // pointer to 16 byte
  for (int i=0;i<16;i++)
  {
    Vec_ADSR_Table[i] = a[i];
    Vec_Twang_Table[i] = t[i];
  }

  // Figure out how much space we need
  size_t size = 0;
  const uint16_t *dims = notes.Dimensions();
  for (uint16_t i = 0; i < dims[0]; ++i)
  {
      size += 2; // A channel note and duration
      // The high 8 bits are 0, 1, or 2 indicating 1, 2, or 3 channels
      size += notes[i * 2].Integer() >> 24;
  }
  // Add the end bytes
  size += 2;
  music->size = size; // 

  // Reserve space for the music table and two end bytes
  uint8_t *data = new uint8_t[size + 2];
  uint8_t *dest = data;

  int channels = 0;
  for (uint16_t i = 0; i < dims[0]; ++i)
  {
      // Get the notes value, which can combine up to 3 channels
      channels = notes[i * 2].Integer();
      // Get the number of channels
      int count = (channels >> 24) + 1;
      // Write note(s) for the channel(s). The last channel has bit7 clear,
      // the previous channels need bit7 set
      int channelShift = channels;
      while (count--)
      {
//printf("%02x ",uint8_t((channelShift & 0x7f) | (count ? 128 : 0)));
          *dest++ = uint8_t((channelShift & 0x7f) | (count ? 128 : 0));
          channelShift >>= 8;
      }
      // Write the duration
      *dest++ = uint8_t(notes[i * 2 + 1].Integer());
//printf("%02x \n\r",uint8_t(notes[i * 2 + 1].Integer()));
  }
  // To end the music, reuse the last note and use a terminator for the duration
  *dest = *(dest - 2);
  *++dest = 0x80;
  music->music = data;
  
}

VectrexMusicNotes::VectrexMusicNotes(VectrexInterface::ADSRs adsr, VectrexInterface::Twangs twang, const Value &notes)
{
}

VectrexMusicNotes::~VectrexMusicNotes()
{
}


VectrexExplosion::VectrexExplosion(int byte0, int byte1, int byte2, int byte3)
{
}

VectrexExplosion::~VectrexExplosion()
{
}
                        
void VectrexMusicPlayer::Init_Music()
{
  
 int ch;
 if (!Vec_Music_Flag) return; // no music
 if (Vec_Music_Flag==1) // new Music
 {
//   printf ("Init Player \n\r");
//Init_Music:     LDX     #Freq_Table     ;Save pointer to frequency table
//                STX     <Vec_Freq_Table
//Init_Music_dft: LDA     #$80            ;Set sound active flag
//                STA     <Vec_Music_Flag
//   LDD     ,U++            ;Save address of ADSR table
//                STD     <Vec_ADSR_Table
//                LDD     ,U++            ;Save address of twang table
//                STD     <Vec_Twang_Table
//                STU     <Vec_Music_Ptr  ;Save pointer to music data
   Vec_Music_Flag = -1;
   Vec_Music_Ptr = music->music;
//                JSR     Init_Music_Buf  ;Initialize music buffer
   v_noSound();
//                LDD     #$1F1F
//                STD     <Vec_ADSR_Timers+1;Init ADSR timers of chans 2 & 3
//                LDD     #$0000
//                STD     <Vec_Music_Freq+2;Clear frequency of channel 2
//                STD     <Vec_Music_Freq+4;Clear frequency of channel 3
//                STA     <Vec_Music_Chan ;A-reg = 0 (sound channel number?)
   Vec_ADSR_Timers[0] =0x1f;
   Vec_ADSR_Timers[1] =0x1f;
   Vec_ADSR_Timers[2] =0x1f;
   Vec_Music_Freq[0] = 0;
   Vec_Music_Freq[1] = 0;
   Vec_Music_Freq[2] = 0;
   Vec_Music_Chan=0;
   Vec_Duration = 0;
//                BRA     LF6EC
 }
 else // continue music
 {
//   printf ("Music playing: \n\r");
   
   
//LF6B3:          LDU     #Vec_ADSR_Timers;Get address of ADSR timers
//                LDB     #$02            ;Count for three channels
   for (ch=2; ch>=0;ch--)
   {
//LF6B8:          LDA     B,U             ;Get the channel's ADSR timer
//                CMPA    #$1F
//                BEQ     LF6C0           ;Skip if at maximum
//                INC     B,U             ;Else increment the timer
      if (Vec_ADSR_Timers[ch] != 0x1f) // not maximum
      {
        Vec_ADSR_Timers[ch]++;
      }
   }
//LF6C0:          DECB                    ;Go back for the other channels
//                BPL     LF6B8

   for (ch=0; ch<3;ch++)
   {
//                LDU     #Vec_Music_Twang
//                LDA     #$07            ;Twang limit is 6-8 depending on channel
//LF6CA:          INC     ,U              ;Increment twang counter
//                CMPA    ,U              ;Check against limit
//                BGE     LF6D2
//                CLR     ,U              ;Clear it if limit exceeded
if (++Vec_Music_Twang_Count[ch] > ch+7) Vec_Music_Twang_Count[ch] = 0;

//                LDX     <Vec_Twang_Table
//LF6D2:          LDB     ,U+             ;Get twang count
//                ANDB    #$07            ;Mask out low 3 bits
//                LDB     B,X             ;Get twang value from table
//                STB     ,U+             ;Update current twang value
Vec_Music_Twang_Value[ch] = Vec_Twang_Table[(Vec_Music_Twang_Count[ch])&7];
   
//                INCA                    ;Increment twang limit
//                CMPA    #$09
//                BLS     LF6CA           ;Go back until all three channels done
   }
//  DEC     <Vec_Duration   ;Decrement the duration timer
//                BNE     LF74E           ;Update ADSR while note still playing
   Vec_Duration--;
   if (Vec_Duration>=0) goto LF74E;

//LF6E3:          LDA     <Vec_Music_Chan ;Go to next music channel
//                DECA
//                BPL     LF6EA           ;If < 0, set it to 2
//                LDA     #$02
//LF6EA:          STA     <Vec_Music_Chan
   Vec_Music_Chan--;
   if (Vec_Music_Chan<0) Vec_Music_Chan=2;
   
 }
 
 while (1)
 {
   

//LF6EC:          LDB     [Vec_Music_Ptr] ;Get next byte of music data
//                LDU     #Vec_ADSR_Timers;Clear ADSR timer for this channel
//                CLR     A,U
  unsigned char b = *Vec_Music_Ptr;   
//  printf ("Music byte: %02x\n\r", b);
//  printf ("(b=$%02x)", b);
  Vec_ADSR_Timers[Vec_Music_Chan] = 0;

//                BITB    #$40            ;If $40 bit of music data set,
//                BEQ     LF712           ;we're going to make some noise
  if (b&0x40)
  {
//    printf ("Noise\n\r");
    
    // noise
    //                ; Continue currently playing sound here
    //                LDX     #Music_Table_1  ;Get bit mask for this channel
    //                LDA     A,X
    //                ANDA    <Vec_Music_Wk_7 ;Turn channel bit off for register 7
    //                STA     <Vec_Music_Wk_7
    v_writePSG_double_buffered(7, Music_Table_1[Vec_Music_Chan] & v_readPSG_double_buffered(7));
    //                LDA     <Vec_Music_Chan ;Set current channel bit in register 7
    //                ADDA    #$03
    //                LDA     A,X
    //                ORA     <Vec_Music_Wk_7
    //                STA     <Vec_Music_Wk_7
    //                ANDB    #$1F            ;Mask off low 5 bits of music data
    //                STB     <Vec_Music_Wk_6 ;and store in register 6

//printf ("%01x:$%02x, ",7,  Music_Table_1[Vec_Music_Chan+3] | v_readPSG_double_buffered(7));
    v_writePSG_double_buffered(7,  Music_Table_1[Vec_Music_Chan+3] | v_readPSG_double_buffered(7));
//printf ("%01x:$%02x, ", 6,  (b&0x1f));
    v_writePSG_double_buffered(6,  b&0x1f);
  }
  else
  {
//    printf ("Sound\n\r");
      // sound
    //LF712:          LDX     #Music_Table_2  ;If $40 bit of music data was cleared,
    //                LDA     A,X             ;Get bit mask for this channel
    //                ANDA    <Vec_Music_Wk_7 ;Turn channel bit off for register 7
    //                STA     <Vec_Music_Wk_7
    v_writePSG_double_buffered(7, Music_Table_2[Vec_Music_Chan] & v_readPSG_double_buffered(7));
            
                    
    //                LDA     <Vec_Music_Chan ;Set current channel bit in register 7
    //                ADDA    #$03
    //                LDA     A,X
    //                ORA     <Vec_Music_Wk_7
    //                STA     <Vec_Music_Wk_7
//printf ("%01x:$%02x, ",7,  Music_Table_2[Vec_Music_Chan+3] | v_readPSG_double_buffered(7));
    v_writePSG_double_buffered(7,  Music_Table_2[Vec_Music_Chan+3] | v_readPSG_double_buffered(7));


    //                LDA     <Vec_Music_Chan ;Get $C855 * 2 + 3
    //                ASLA
    //                ADDA    #Vec_Music_Freq-Vec_ADSR_Timers
    //                LEAU    A,U             ;Point U-reg to #$C861 + $C855 * 2
    //                ANDB    #$3F            ;Mask off low 6 bits of music data,
    //                ASLB                    ;multiply by 2
    //                LDX     <Vec_Freq_Table ;Get pointer to note-to-frequency table
    //                LDD     B,X             ;Get note table data

                    
    //                STD     ,U              ;Store in word at $C861-$C866
    Vec_Music_Freq[Vec_Music_Chan] = Freq_Table[b & 0x3f];
    //((Freq_Table[b & 0x3f]>>8) & 0xff)  +  (((Freq_Table[b & 0x3f]) & 0xff)<<8); // to little endian!
  }
 

//LF735:          LDX     <Vec_Music_Ptr  ;Re-get byte of music data
//                LDB     ,X+
//                STX     <Vec_Music_Ptr  ;Update music data pointer
  b = *Vec_Music_Ptr++;   
//  printf ("(b=$%02x)", b);

  if (b&0x80)
  {
//                TSTB
//                BMI     LF6E3           ;If byte>=$80, advance to next channel
   Vec_Music_Chan--;
   if (Vec_Music_Chan<0) Vec_Music_Chan=2;
   continue;    
  }
                 
//                LDB     ,X+             ;Get second byte of music data
  b = *Vec_Music_Ptr++;   
//  printf ("(b=$%02x)", b);
//  printf ("Music byte: %02x\n\r", b);
//                BPL     LF748           ;If >=$80, (terminator)
//                JSR     Init_Music_Buf  ;  clear music buffer,
//                CLR     <Vec_Music_Flag ;  clear music flag,
//                RTS                     ;  and exit
  if (b&0x80)
  {
    // music finished completely!
    stop();
//printf ("Music END\n\r", b);
    return;
  }
//LF748:          STX     <Vec_Music_Ptr  ;Update music data pointer
//                ANDB    #$3F            ;Duration in low 6 bits of second byte
//                STB     <Vec_Duration   ;Store duration counter
  Vec_Duration = b&0x3f;
  break;
  
}   
  
  
  
LF74E:  
  for (ch = 2;ch>=0;ch--)
  {
    

//LF74E:          LDY     <Vec_ADSR_Table ;Get pointer to ADSR table
//                LDU     #Vec_ADSR_Timers;Point to ADSR timer table
//                LDX     #Vec_Music_Wk_A
//                LDA     #$02            ;Count for three channels
//LF759:          LDB     ,U+             ;Get channel timer?
//                BITB    #$01            ;Test low bit of ADSR index
//                BEQ     LF766
//                LSRB                    ;If odd, divide ADSR index by by 2
//                LDB     B,Y             ;Get low nibble from ADSR table
//                ANDB    #$0F
//                BRA     LF76D
//
//LF766:          LSRB                    ;If even, divide ADSR index by 2
//                LDB     B,Y             ;Get high nibble from ADSR table
//                LSRB
//                LSRB
//                LSRB
//                LSRB

  unsigned char nibble = Vec_ADSR_Table[Vec_ADSR_Timers[ch]>>1];
  if ((Vec_ADSR_Timers[ch] & 1) == 1)
    nibble = nibble>>4;
  nibble = nibble &0x0f;
  
//LF76D:          STB     A,X             ;Store ADSR value in regs 10-12
//                DECA                    ;Decrement channel counter
//                BPL     LF759           ;Go back for next channel
  v_writePSG_double_buffered(8+ch, nibble); // volume is decoded in ADSR nibbles
//printf ("%01x:$%02x, ", 8+ch, nibble);
  }
//unsigned char Vec_Music_Twang_Count[3];
//unsigned char Vec_Music_Twang_Value[3];
                
                
//                LDU     #Vec_Music_Freq+6;Point to base frequency table
//                LDX     #Vec_Music_Wk_5 ;Point to twang table
//LF778:          LDD     ,--U            ;Get next base frequency
//                TST     -8,U            ;Test twang value
//                BPL     LF788

 for (ch = 0;ch <3; ch++)
 {
  unsigned int baseFreq = Vec_Music_Freq[ch];
//                NEG     -8,U            ;If <0, negate twang table entry
//                SUBB    -8,U            ;Subtract negated value from frequency
//                SBCA    #$00            ;Propagate borrow to high byte
//                NEG     -8,U            ;Un-negate twang entry
//                BRA     LF78C
//LF788:          ADDB    -8,U            ;If >0 add twang to base frequency
//                ADCA    #$00            ;Propagate carry to high byte
//LF78C:          STD     ,X++            ;Store freq in regs 5/4, 3/2, 1/0
//                CMPX    #Vec_Music_Work+14
//                BNE     LF778
//LF793_RTS:      RTS

  baseFreq +=Vec_Music_Twang_Value[ch];
//printf ("%01x:$%02x, ", ch*2+0, baseFreq&0xff);
  v_writePSG_double_buffered(ch*2+0, baseFreq&0xff );
//printf ("%01x:$%02x, ",ch*2+1, (baseFreq>>8)&0x0f );
  v_writePSG_double_buffered(ch*2+1, (baseFreq>>8)&0x0f  );
 }

// printf ("\n\r----\n\r");

}




// I know this verbatim 
// asm copy could be
// easily beautyfied
// but I really can't be bothered
// for more info on used registers and their meaning
// look at the header file
void VectrexMusicPlayer::playExplosion()
{
 unsigned char a;
 unsigned char b;


//Explosion_Snd:  LDA     <Vec_Expl_Flag
//                BPL     LF95B

  if (Vec_Expl_Flag<0)
  {
    
//                ANDA    #$7F
//                STA     <Vec_Expl_Flag
  Vec_Expl_Flag= (Vec_Expl_Flag & 0x7f); // makeit positive
//                LDX     #Vec_Expl_1     ;Copy 4 bytes from U-reg to $C858
//                LDA     #$04
//                JSR     Move_Mem_a
  Vec_Expl_ChanB = Vec_Expl_ChanA = Vec_Expl_Chans = Vec_Expl_1[0] = byte0;                
  Vec_Expl_1[1] = byte1;                
  Vec_Expl_1[2] = byte2;                
  Vec_Expl_1[3] = byte3;                

//                LSRB                    ;Divide first byte by 8
//                LSRB
//                LSRB
//                ORB     <Vec_Expl_1     ;OR with first byte
//                ANDB    #$07            ;AND with 7
//                STB     <Vec_Expl_Chans ;store in $C854
  Vec_Expl_Chans= Vec_Expl_Chans>>3;
  Vec_Expl_Chans = Vec_Expl_Chans | Vec_Expl_1[0];
  Vec_Expl_Chans = Vec_Expl_Chans & 7;
  
  
  
//                LDB     <Vec_Expl_1     ;Get first byte
//                ANDB    #$38            ;Mask off bits 3-5
//                STB     <Vec_Expl_ChanA ;store in $C853
  Vec_Expl_ChanA = Vec_Expl_ChanA & 0x38;
 
//                LDB     <Vec_Expl_1     ;Get first byte
//                ANDB    #$07            ;AND with 7
//                STB     <Vec_Expl_ChanB ;store in $C85D
 Vec_Expl_ChanB = Vec_Expl_ChanB &7;
 
//                LDB     #$02            ;Start with channel number 2
//                STB     <Vec_Expl_Chan
 Vec_Expl_Chan = 2;                
//                LDA     #$7F            ;Initialize time count
//                BRA     LF968
 Vec_Expl_Timer = 0x7f;      
 explosionStarted = 1;
 explosionFinished = 0;
  }
  else
  {
   // already init, now continue
//    LF95B:          LDA     <Vec_Expl_Timer
//                BEQ     LF9C9_RTS
   if (Vec_Expl_Timer == 0)
   { 
     stop();
     return;  
   }
//                SUBA    <Vec_Expl_4
//                BPL     LF968
//                CLRB
//                STB     <Vec_Expl_Timer
//                BRA     LF9CA

   Vec_Expl_Timer -= Vec_Expl_1[3];
   if (Vec_Expl_Timer<0)
   {

     Vec_Expl_Timer=0;
     b=0;
          
          
      {
      //LF9CA:          LDA     <Vec_Expl_Chans
      //                LDX     #Vec_Music_Wk_7
      //LF9CF:          TSTA                    ;Exit if all channels done
      //                BEQ     LF9DB_RTS
      //                LEAX    -1,X            ;Point to next register (8-10)
      //                LSRA
      //                BCC     LF9CF
      //                STB     ,X              ;Store noise value if chan in use
      //                BRA     LF9CF
      //LF9DB_RTS:      RTS
        a = Vec_Expl_Chans;
        int AmpReg = 7;
        while (a!=0)
        {
            AmpReg++;
            if ((a&1) ==0) 
            {
              a = a>>1;
              continue;
            }
            a = a>>1;
            v_writePSG_double_buffered(AmpReg, b);
        }
      }
      return;     
     
     
     
   }
  }
  

  
//LF968:          STA     <Vec_Expl_Timer
//                LSRA
//                LSRA
 a = Vec_Expl_Timer>>2; 
  
//                LDB     <Vec_Expl_ChanA
 b = Vec_Expl_ChanA;
//                BEQ     continueWithSoundChannels
 if (b == 0) goto continueWithSoundChannels;
//                STA     <Vec_Music_Wk_6
 v_writePSG_double_buffered(6, a);
 //               LDB     <Vec_Expl_2
 b = Vec_Expl_1[1]; // frequency swoop
//                BMI     LF97B
 if (b&0x80) goto LF97B;
//                BEQ     continueWithSoundChannels
 if (!b) goto continueWithSoundChannels;
//                TFR     A,B
 b = a;
//                COMB
 b = ~b; 
LF97B:          
//                STB     <Vec_Music_Wk_6
 v_writePSG_double_buffered(6, b);

continueWithSoundChannels:
//                LSRA
 a = a >>1;
//                CMPA    #$07
//                BLS     LF987
 if (a>7)
 {
//                CMPA    #$0F
//                BEQ     LF987
    if (a!= 0xf)
    {
//                INCA
      a++;
    }
 }
LF987:  
//                LDB     <Vec_Expl_3
 b = Vec_Expl_1[2]; // volume swoop
//                BMI     LF991
 if (b&0x80) goto LF991; // stay
//                BEQ     LF98F
 if (!b) goto LF98F;
//                EORA    #$0F
 a= a^0xf;
LF98F:
//                TFR     A,B
 b= a;                
LF991:
//                BSR     LF9CA
 {
//LF9CA:          LDA     <Vec_Expl_Chans
//                LDX     #Vec_Music_Wk_7
//LF9CF:          TSTA                    ;Exit if all channels done
//                BEQ     LF9DB_RTS
//                LEAX    -1,X            ;Point to next register (8-10)
//                LSRA
//                BCC     LF9CF
//                STB     ,X              ;Store noise value if chan in use
//                BRA     LF9CF
//LF9DB_RTS:      RTS
   a = Vec_Expl_Chans;
   int AmpReg = 7;
   while (a!=0)
   {
      AmpReg++;
      if ((a&1) ==0) 
      {
        a = a>>1;
        continue;
      }
      a = a>>1;
      v_writePSG_double_buffered(AmpReg, b);
   }
 }
 
                
//                LDB     <Vec_Expl_ChanB
 b = Vec_Expl_ChanB;
//                BEQ     LF9C2
 if (!b) goto LF9C2;
 
LF997:
//                LDA     <Vec_Expl_Chan  ;Get channel number
 a=Vec_Expl_Chan;                
//                DECA                    ;Decrement channel number
 a--;                
//                BPL     LF99E
 if (!(a&0x80)) goto LF99E;  
//                LDA     #$02            ;Reset to 2 if less than zero
 a= 2;                
LF99E:
//                STA     <Vec_Expl_Chan  ;Save channel number
 Vec_Expl_Chan = a;                
//                JSR     Bitmask_a       ;Get bit mask of the channel

 a = Bit_Masks[a];
//                BITA    <Vec_Expl_ChanB
//                BEQ     LF997           ;Go back if not in for $C85D
 if ((Vec_Expl_ChanB & a) == 0) goto LF997;            
//                LDB     <Vec_Expl_Chan
 b =  Vec_Expl_Chan;
//                ASLB                    ;Negative of channel number ; 2
//                NEGB
//                LDX     #Vec_Music_Wk_1 ;  (registers 1, 3, and 5)
//                LEAX    B,X

//                JSR     Random
 a = (unsigned char)rand();
//                ANDA    #$0F
 a = a & 0xf;
//                CMPA    #$05
//                BHI     LF9BC
 if (a>5) goto LF9BC;
//                ASLA
//                ADDA    #$05
 a = a<<1;
 a += 5;
LF9BC:
//                STA     ,X
// store a to coarse freq of channel in b
  v_writePSG_double_buffered(b*2+1, a);

//                LDA     <Vec_Random_Seed+1
 a = (unsigned char)rand();

 // store a to fine freq of channel in b
//                STA     1,X
  v_writePSG_double_buffered(b*2, a);
                
                
                
                
LF9C2:         
//                LDA     <Vec_Expl_1
//                COMA
 a = Vec_Expl_1[0];
 a = ~a;
//                ANDA    <Vec_Music_Wk_7
 a = a & v_readPSG_double_buffered(7);                
                
//                STA     <Vec_Music_Wk_7
 v_writePSG_double_buffered(7, a);
//LF9C9_RTS:      RTS
 return;
}

VSample::VSample(unsigned char *filename, int r) :rate(r), loaded(0), size(0)
{
  FILE *fileRead;
  fileRead = fopen((const char*)filename, "rb");
  
  if (fileRead == 0)
  {
    printf("File (%s) could not be opened: %s!\r\n",filename, ff_getErrorText(errno));
    return;
  }
  int len = __filelength(fileRead);
  if (fileRead == 0)
  {
    printf("Ini (%s) file is larger than %i!\r\n",filename, 0);
    fclose(fileRead);
    return;
  }
  data = new unsigned char[len];

  if (data == null)
  {
    printf("Out of memory error during sample read!\r\n");
    fclose(fileRead);
    return;
  }
  int loadLen = fread(data, len, 1, fileRead);
  if (1 != loadLen)
  {
    delete data;
    printf("Read(1) of %s fails (len loaded: %i) (Error: %i)\r\n", filename, len, errno);
    fclose(fileRead);
    return;
  }
  fclose(fileRead);
  printf("File loaded successfully (%s, %i)!\r\n",filename,len);
  size = len;
  loaded = 1;
}

VYMMusic::VYMMusic(unsigned char *filename) : loaded(0), size(0)
{
  FILE *fileRead;
  fileRead = fopen((const char*)filename, "rb");
  if (fileRead == 0)
  {
    printf("File (%s) could not be opened: %s!\r\n",filename, ff_getErrorText(errno));
    return;
  }
  int len = __filelength(fileRead);
  if (fileRead == 0)
  {
    printf("YM (%s) file is larger than %i!\r\n",filename, 0);
    fclose(fileRead);
    return;
  }
  fclose(fileRead);
  ymBASICBufferLoad = new char[len];
  ymBASICBuffer = new char[len];

  int length = 1;
  length = loadYM(filename, len,ymBASICBuffer,ymBASICBufferLoad);
  if (length!=0)
  {
    data = (unsigned char*) ymBASICBuffer;
    loaded = 1;
    size = length;
    // done by PLAY
//    v_initYM((unsigned char *)data, size, 1);      
  }
  else
  {
    delete ymBASICBuffer;
    ymBASICBuffer=0; 
  }
  delete ymBASICBufferLoad;
  ymBASICBufferLoad=0; 
}