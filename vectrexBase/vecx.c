#include <stdint.h>
#include <unistd.h>
#include "vecx.h"

#include <vectrex/vectrexInterface.h>


#include "e6809.h"
#include "e6522.h"
#include "e8910.h"
#include "edac.h"

#include "bios.i"

// since we use the cycle counter "differently" - we define our
// own DIRECT setter and getter




int isShallowRead;
void(*vecx_render) (void);

//uint8_t cartBig[32768*2*4]; // Vectorblade
//uint8_t cart[32768];

uint8_t cart[32768*9]; // lineart


int is64kBankSwitch = 0;
int currentBank =3;
uint8_t ram[1024];
#ifdef FILE_PLAYER
uint8_t movieRom[1024*1024*20]; // maximum movie size 20 MB
int movieLength=0;
#endif


/* the sound chip registers */
uint8_t snd_select;
uint32_t cycleCount;
uint32_t lastAddLine;
uint32_t oldT2Trigger;

// direct emulation
int viaAccessCounter = 0;
uint16_t delayVia[2];
uint8_t delayData[2];

uint32_t cycleCount;
uint32_t mustWait = 0;
int WRMissed = 0;

int mustWork = 1;
int isRamping = 0;


#ifdef DIRECT_EMULATION

int vectrexButtonState = 0x1ff;
int directEmulation = 1;
#else
int directEmulation = 0;
#endif

enum
{
	VECTREX_PDECAY = 30,      /* phosphor decay rate */

	/* number of 6809 cycles before a frame redraw */

	FCYCLES_INIT = VECTREX_MHZ / VECTREX_PDECAY,

	/* max number of possible vectors that maybe on the screen at one time.
	 * one only needs VECTREX_MHZ / VECTREX_PDECAY but we need to also store
	 * deleted vectors in a single table
	 */

	VECTOR_MAX_CNT = VECTREX_MHZ / VECTREX_PDECAY,
};

size_t vector_draw_cnt;
vector_t vectors[VECTOR_MAX_CNT];

int32_t fcycles;

/* update the snd chips internal registers when VIA.ora/VIA.orb changes */

static void snd_update(void)
{
	switch (VIA.orb & 0x18)
	{
	case 0x00:
		/* the sound chip is disabled */
		break;
	case 0x08:
		/* the sound chip is sending data */
		break;
	case 0x10:
		/* the sound chip is recieving data */

		if (snd_select != 14)
		{
            if (!directEmulation) return;
            v_writePSG(snd_select, VIA.ora);
			e8910_write(snd_select, VIA.ora);
		}
		break;
	case 0x18:
		/* the sound chip is latching an address */
		if ((VIA.ora & 0xf0) == 0x00)
        {
            snd_select = VIA.ora & 0x0f;
        }
		break;
	}
}

// only emulated
static uint8_t read8_port_a()
{
	if ((VIA.orb & 0x18) == 0x08)
	{
		/* the snd chip is driving port a */
		uint8_t directData = v_readPSG(snd_select);

		return directData;
	}
	else
	{
		return VIA.ora;
	}
}

// only emulated
static uint8_t read8_port_b()
{
	/* compare signal is an input so the value does not come from
	* VIA.orb.
	*/
	if (VIA.acr & 0x80)
	{
		/* timer 1 has control of bit 7 */
		return (uint8_t)((VIA.orb & 0x5f) | VIA.t1pb7 | DAC.compare);
	}
	else
	{
		/* bit 7 is being driven by VIA.orb */
		return (uint8_t)((VIA.orb & 0xdf) | DAC.compare);
	}
}

// used by both
void write8_port_a(uint8_t data)
{
	/* output of port a feeds directly into the dac which then
	* feeds the x axis sample and hold.
	*/
    snd_update();
	DAC.xsh = data ^ 0x80;
	dac_update();
}

// used by both
void write8_port_b(uint8_t data)
{
	(void)data;
    snd_update();
	dac_update();
}

static uint8_t vx_read8(uint16_t address)
{
	uint8_t data = 0xff;

	if ((address & 0xe000) == 0xe000)
	{
		/* rom */
		data = rom[address & 0x1fff];
	}
	else if ((address & 0xe000) == 0xc000)
	{
	  if (address & 0x800)
	  {
		  /* ram */
		  data = ram[address & 0x3ff];
	  }
	  else if (address & 0x1000)
	  {
			/* io */
            if (directEmulation)
            {
              if (isShallowRead)  
              {
                if ((address &0xf) == 0)
                  data = VIA.orb;
                if ((address &0xf) == 1)
                  data = VIA.ora;
                else
                  data =  V_GET(address);
              }
                
              else
              {
                data =  V_GET(address);
              }
#ifdef DIRECT_EMULATION
#ifndef WAIT_EMULATION
	      // a T1 "end" 
	      // must wait even, when not "wait emulation"
	      // otherwise lines will be TOO short!!!
	      if ((address &0xf) == (VIA_int_flags&0xf))              
	      {
		// reading the Interrupt flags
		if ((data & 0x40) == 0x40) // if interrupt is set
		{
		  // and either rega or reg b might be used to compare it with
		  if ((CPU.reg_a == 0x40) ||(CPU.reg_b == 0x40) )
		  {
	      //        ADD_DELAY_CYCLES(DELAY_AFTER_T1_END_VALUE_DIRECT-2);
	      setCounter1Mark();
	      waitCounter1Mark((DELAY_AFTER_T1_END_VALUE_DIRECT-2));



		      
		  }
		}
	      }
    
#endif
#endif              
#ifdef DIRECT_EMULATION
              if (VIA.ddra == 0)
              {
                if ((VIA.orb & 0x7f) == 0x09)
                {
                  if (snd_select == 0xe)
                  {
                    vectrexButtonState = data;
//printf("%04x: vectrexButtonState = $%02x\n\r", CPU.reg_pc, vectrexButtonState);
                  }
                }
              }
#endif              
            }
              
            else
              data = via_read(address);
	  }
	}
	else if (address < 0xc000)
	{
	    /* cartridge */
	    data = cart[address+currentBank*32768];
	}
	return data;
}





#ifdef FILE_PLAYER
int pos = 0;
void writeExtreme(int addr, unsigned char data)
{
    int i;
    if ((addr&0xff)==0xff) 
    {
            if (data==1) 
            {
                printf("Cart: extreme multicard -> Unimplemented: multicart\r\n");
            } 
            else if (data==2) 
            {
                if (!movieLength)
                {
                  printf("Movie data not loaded\r\n");
                  return;
                }
            
              
                if (movieLength<pos+1024+512) pos = 0;
                for (int ii=0; ii< 1024+512;ii++)
                {
                    cart[(0x4000+ii)+currentBank*32768*2] = movieRom[pos];
//System.out.println("Extreme bank switch");                    
                    pos++;
                }
//                printf("Movie data moved to card...\r\n");
            } 
            else if (data==66) 
            {
                printf("Cart: extreme multicard -> doom not supported\r\n");
                printf("Doom Boom!\r\n");
            }
        }
}
#endif      
static void vx_write8(uint16_t address, uint8_t data)
{
	if ((address & 0xe000) == 0xe000)
	{
		/* rom */
	}
	else if ((address & 0xe000) == 0xc000)
	{
		/* it is possible for both ram and io to be written at the same! */

		if (address & 0x800)
		{
          // if bedlam
          // do not allow write to VecRfsr
//          if ((CPU.reg_pc<0x100) && ((address == 0xc83d) || (address == 0xc83e)) ) return;
          
			ram[address & 0x3ff] = data;
		}

		if (address & 0x1000)
		{
		  via_write(address, data);
		}
	}
	else if (address < 0xbfff)
	{
		/* cartridge */
    #ifdef FILE_PLAYER
          writeExtreme(address, data);
    #endif      
          
          
#ifdef ALLOW_ROM_WRITE          
        cart[address+currentBank*32768*2] = data;
#endif
	}
}
int lastX;
int lastY;
#ifndef DIRECT_EMULATION
static void addline(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint8_t color)
{
  if (directEmulation) return;
  
/*
 printf("%i, %i, %i, %i, %u\n\r",  x0,  y0,  x1,  y1, cycleCount-lastAddLine);
lastX = x1;
lastY = y1;

  lastAddLine = cycleCount;
*/
	vectors[vector_draw_cnt].x0 = x0;
	vectors[vector_draw_cnt].y0 = y0;
	vectors[vector_draw_cnt].x1 = x1;
	vectors[vector_draw_cnt].y1 = y1;
	vectors[vector_draw_cnt].color = color;
	vector_draw_cnt++;
}
#endif

void vecx_input(uint8_t key, uint8_t value)
{
    switch (key)
    {
    case VECTREX_PAD1_X: DAC.jch0 = value; break;
    case VECTREX_PAD1_Y: DAC.jch1 = value; break;
    case VECTREX_PAD2_X: DAC.jch2 = value; break;
    case VECTREX_PAD2_Y: DAC.jch3 = value; break;
    }
}
void vecx_reset(void)
{
	/* ram */
	for (int r = 0; r < 1024; r++)
		ram[r] = (uint8_t)r;//and();

	e8910_reset();

	snd_select = 0;
#ifndef DIRECT_EMULATION
	dac_add_line = addline;
#else
    vectrexButtonState = 0x1ff;
    currentBank =3;
#endif    
#ifdef LINEART
    currentBank = 5;
#endif
    
    
	dac_reset();

	vector_draw_cnt = 0;
	lastAddLine = 0;
	fcycles = FCYCLES_INIT;

	via_read8_port_a = read8_port_a;
	via_read8_port_b = read8_port_b;
	via_write8_port_a = write8_port_a;
	via_write8_port_b = write8_port_b;

	via_reset();

	e6809_read8 = vx_read8;
	e6809_write8 = vx_write8;
	oldT2Trigger = 0;
	e6809_reset();
    
// direct emulation
    viaAccessCounter = 0;
    cycleCount = 0;
    WRMissed = 0;
    isRamping = 0;
    isShallowRead = 0;
  
}
 
static char printbuffer[40];
int32_t PrintStr(int32_t cycles)
{
    int x = -1+ (DAC.curr_x-32768/2)/128;
    int y = -(1+ (DAC.curr_y-32768/2)/128);

    int address = CPU.reg_u;
    int counter=0;
    unsigned char c = vx_read8(address++);
    while ((c & 0x80) != 0x80)
    {
      if (counter < 39)
        printbuffer[counter++] = c;
      c = vx_read8(address++);
    }
    printbuffer[counter++] = 0x80;

   int cyclesemulated  = 0;
   
   
    // brightness must be set
    // since otherwise brightness if only set 
    // with lines (in emulated vecx) -> pipeline
    if (!directEmulation)
    {
        uint8_t brightness = (uint8_t)DAC.zsh;
        // if the same brightness is already set - v_setBrightness wil know!
        v_setBrightness(brightness);
    }
   
    //Vec_Text_Height EQU     $C82A   ;Default text height
    //Vec_Text_Width  EQU     $C82B   ;Default text width
    cyclesemulated  = v_printStringRaster(x, y, printbuffer, vx_read8(0xC82B), vx_read8(0xC82A), 0x80);

    cycles -= cyclesemulated;
    cycleCount += cyclesemulated;

    DAC.curr_x = DAC_MAX_X / 2; // Origin!
    DAC.curr_y = DAC_MAX_Y / 2;
    VIA.pcr = 0xcc; //Shift Reg. Enabled, T1 PB7 Enabled
    VIA.ora = 0;
    VIA.orb = 1;
    DAC.ysh = DAC.xsh = 0;
    VIA.acr = 0x98;
    VIA.ca2 = 0;

    CPU.reg_u = address;
    CPU.reg_pc = 0xF354; // Reset 0 Ref 
#ifdef BEAM_LIGHT_BY_CNTL
    V_SET (VIA_aux_cntl, 0x98); //Shift Reg. Enabled, T1 PB7 Enabled
#endif
    return cycles;
}
extern int v_printStringRasterFont(int8_t x, int8_t y, char* _string, int8_t xSize, int8_t ySize, unsigned char delimiter, unsigned char *rlines[]);

// for now this is special for Vectorbalde!
int32_t PrintStr_d(int32_t cycles, int width, int height, unsigned char *rlines[])
{
    signed char x = (signed char) CPU.reg_b;
    signed char y = (signed char) CPU.reg_a;

    int address = CPU.reg_u;
    int counter=0;
    unsigned char c = vx_read8(address++);
    while ((c & 0x80) != 0x80)
    {
      if (counter < 39)
        printbuffer[counter++] = c;
      c = vx_read8(address++);
    }
    printbuffer[counter++] = 0x80;
    int cyclesemulated  = 0;
    v_resetIntegratorOffsets0(); 
   
    //Vec_Text_Height EQU     $C82A   ;Default text height
    //Vec_Text_Width  EQU     $C82B   ;Default text width
    cyclesemulated  = v_printStringRasterFont(x, y, printbuffer, width, height, 0x080, rlines);
    cycles -= cyclesemulated;
    cycleCount += cyclesemulated;

    DAC.curr_x = DAC_MAX_X / 2; // Origin!
    DAC.curr_y = DAC_MAX_Y / 2;
    VIA.pcr = 0xcc; //Shift Reg. Enabled, T1 PB7 Enabled
    VIA.ora = 0;
    VIA.orb = 1;
    DAC.ysh = DAC.xsh = 0;
    VIA.acr = 0x98;
    VIA.ca2 = 0;

    CPU.reg_u = address;
    CPU.reg_pc = 0xF354; // Reset 0 Ref 
    V_SET(VIA_t1_cnt_lo,VIA.t1ll);  
#ifdef BEAM_LIGHT_BY_CNTL
    V_SET (VIA_aux_cntl, 0x98); //Shift Reg. Enabled, T1 PB7 Enabled
#endif
    mustWork = 0;
    return cycles;
}

// naturaolly the Wait Recak
// emulation only works with games which actually USE WaitRecak!

void vecx_emulate(int32_t cycles)
{
	while (cycles > 0)
	{
        switch ((long)CPU.reg_pc)
        {
          case 0xF434: // Draw_Pat_VL_a for intro screen
          {
            uint8_t pattern = vx_read8(0xC829);             
            int count = CPU.reg_a;        
            uint16_t listStartAdr = (uint16_t) CPU.reg_x;
            uint8_t scale = (uint8_t)VIA.t1ll; 
            uint8_t brightness = (uint8_t)DAC.zsh;
            int x0 =  (DAC.curr_x-32768/2);
            int y0 = -(DAC.curr_y-32768/2);

            CPU.reg_x += 2*(count+1);
      
            int x1=0;
            int y1=0;
            for (;count>=0;count--)
            {
              y1 = y0+(((signed char)vx_read8(listStartAdr++))*scale);
              x1 = x0+(((signed char)vx_read8(listStartAdr++))*scale);
              
              v_directDraw32Patterned(x0, y0, x1, y1, brightness, pattern);
              x0 = x1;
              y0 = y1;
            }
            
            CPU.reg_pc = 0xF34F;// check 0 ref // 0xF36A; // any RTS          
            break;
          }
          case 0xF192:// Wait_Recal
          {
            WRMissed = 0;
            oldT2Trigger = cycleCount;
            vecx_render();
            vector_draw_cnt = 0;
            fcycles = FCYCLES_INIT;

            int c = ((uint16_t)vx_read8(0xC825))*256 + ((uint16_t)vx_read8(0xC826)); // Vec_Loop_Count
            c++;
            vx_write8(0xC825, (c>>8)&0xff);
            vx_write8(0xC826, (c)&0xff);

            CPU.reg_pc = 0xF36A; // any RTS          
            CPU.reg_dp = 0xd0;
            break;
          }
          case 0xF495: // printstr
          {
            cycles = PrintStr(cycles);
        
          }
          default:
          {      
            uint16_t icycles = e6809_sstep(VIA.ifr & 0x80, 0);
            for (uint16_t c = 0; c < icycles; c++)
            {
                via_sstep0();
                dac_sstep();
                via_sstep1();
            }
            cycles -= (int32_t)icycles;
            fcycles -= (int32_t)icycles;
            cycleCount += icycles;
            break;
          }
        }
        
        // try a T2 timer approach, will not work for non T2 games
        if ((WRMissed) && ((VIA.ifr & 0x20) == 0x20))
        {
                if ((cycleCount - oldT2Trigger)>20000)
                {
                    oldT2Trigger = cycleCount;
                    vecx_render();
                    vector_draw_cnt = 0;
                    fcycles += FCYCLES_INIT;
                }
        }

        // savety net for no WaitRecal
        if (fcycles < -20000)
        {
            printf("WaitRecal missed!\n\r");
            WRMissed = 1;
            fcycles += FCYCLES_INIT;
            oldT2Trigger = cycleCount;
            vecx_render();
            vector_draw_cnt = 0;
        }
	}
}

inline static void t2miniStep(int c)
{
    if (VIA.t2on && (VIA.acr & 0x20) == 0x00)
    {
        if ((((signed int)VIA.t2c) - c) >= 0)
        {
            VIA.t2c-=c;
            return;
        }
        VIA.t2c-=c;
        /* one shot mode */
        if (VIA.t2int)
        {
          VIA.ifr |= 0x20;
          if ((VIA.ifr & 0x7f) & (VIA.ier & 0x7f))
          {
              VIA.ifr |= 0x80;
          }
          else
          {
              VIA.ifr &= 0x7f;
          }
          VIA.t2int = 0;
        }
    }
}
inline static void t1MiniStep(int c)
{
    if (!VIA.t1on)
    {
      return;
    }

    if ((((signed int)VIA.t1c) - c) >= 0)
    {
        VIA.t1c-=c;
        return;
    }
    VIA.t1c-=c;
    // one shot mode 
    if (VIA.t1int)
    {
        VIA.ifr |= 0x40;
        if ((VIA.ifr & 0x7f) & (VIA.ier & 0x7f))
        {
            VIA.ifr |= 0x80;
        }
        else
        {
            VIA.ifr &= 0x7f;
        }
        VIA.t1pb7 = 0x80;
        VIA.t1int = 0;
    }
}

inline static void miniStep(int c)
{
#ifdef BEDLAM_T2_HANDLING    
    t2miniStep(c);
#endif    
    if (!VIA.t1on)
    {
      if (VIA.ca2 == 0)
      {
          DAC.curr_x = DAC_MAX_X >> 1;
          DAC.curr_y = DAC_MAX_Y >> 1;
          return;
      }
      if (  ((VIA.acr & 0x80) && (VIA.t1pb7 == 0)) || (!(VIA.acr & 0x80)) && ((VIA.orb & 0x80) == 0)  )
      {
        DAC.curr_x += c*DAC.dx;
        DAC.curr_y += c*DAC.dy;
      }
      return;
    }
    if ((((signed int)VIA.t1c) - c) >= 0)
    {
        VIA.t1c-=c;
        if (VIA.ca2 == 0)
        {
            DAC.curr_x = DAC_MAX_X >> 1;
            DAC.curr_y = DAC_MAX_Y >> 1;
            return;
        }
        if (  ((VIA.acr & 0x80) && (VIA.t1pb7 == 0)) || (!(VIA.acr & 0x80)) && ((VIA.orb & 0x80) == 0)  )
        {
          DAC.curr_x += c*DAC.dx;
          DAC.curr_y += c*DAC.dy;
        }
        return;
    }
    for (int i=0; i<c;i++)
    {
  
          VIA.t1c--;
          if ((VIA.t1c & 0xffff) == 0xffff)
          {
              /* counter just rolled over */
              if (VIA.acr & 0x40)
              {
                  // continuous interrupt mode 
                  VIA.ifr |= 0x40;
                  if ((VIA.ifr & 0x7f) & (VIA.ier & 0x7f))
                  {
                      VIA.ifr |= 0x80;
                  }
                  else
                  {
                      VIA.ifr &= 0x7f;
                  }
                  VIA.t1pb7 = 0x80 - VIA.t1pb7;

                  // reload counter 
                  VIA.t1c = (VIA.t1lh << 8) | VIA.t1ll;
              }
              else
              {
                  // one shot mode 

                  if (VIA.t1int)
                  {
                      VIA.ifr |= 0x40;
                      if ((VIA.ifr & 0x7f) & (VIA.ier & 0x7f))
                      {
                          VIA.ifr |= 0x80;
                      }
                      else
                      {
                          VIA.ifr &= 0x7f;
                      }
                      VIA.t1pb7 = 0x80;
                      VIA.t1int = 0;
                  }
              }
          }
          if (VIA.ca2 == 0)
          {
              /* need to force the current point to the 'orgin' so just
              * calculate distance to origin and use that as dx,dy.
              */
              DAC.curr_x = DAC_MAX_X >> 1;
              DAC.curr_y = DAC_MAX_Y >> 1;
              return;
          }
          if (  ((VIA.acr & 0x80) && (VIA.t1pb7 == 0)) || (!(VIA.acr & 0x80)) && ((VIA.orb & 0x80) == 0)  )
          {
            DAC.curr_x += DAC.dx;
            DAC.curr_y += DAC.dy;
          }
    }

}
inline static void miniStep2(int c)
{
#ifdef BEDLAM_T2_HANDLING    
    t2miniStep(c);
#endif    

    if (!VIA.t1on)
    {
      if (VIA.ca2 == 0)
      {
          DAC.curr_x = DAC_MAX_X >> 1;
          DAC.curr_y = DAC_MAX_Y >> 1;
      }
      else if (  ((VIA.acr & 0x80) && (VIA.t1pb7 == 0)) || (!(VIA.acr & 0x80)) && ((VIA.orb & 0x80) == 0)  )
      {
        DAC.curr_x += c*DAC.dx;
        DAC.curr_y += c*DAC.dy;
      }
    }
    else
    if ( (VIA.t1c & 0xffff) >= c)
    {
        VIA.t1c-=c;

        if (VIA.ca2 == 0)
        {
            DAC.curr_x = DAC_MAX_X >> 1;
            DAC.curr_y = DAC_MAX_Y >> 1;
        }
        if ( ((VIA.acr & 0x80) && (VIA.t1pb7 == 0)) || (!(VIA.acr & 0x80)) && ((VIA.orb & 0x80) == 0)  )
        {
          DAC.curr_x += c*DAC.dx;
          DAC.curr_y += c*DAC.dy;
        }
    }
    else
 
    for (int i=0; i<c;i++)
    {
          VIA.t1c--;
          if ((VIA.t1c & 0xffff) == 0xffff)
          {
              /* counter just rolled over */
              if (VIA.acr & 0x40)
              {
                  // continuous interrupt mode 
                  VIA.ifr |= 0x40;
                  if ((VIA.ifr & 0x7f) & (VIA.ier & 0x7f))
                  {
                      VIA.ifr |= 0x80;
                  }
                  else
                  {
                      VIA.ifr &= 0x7f;
                  }
                  VIA.t1pb7 = 0x80 - VIA.t1pb7;

                  // reload counter 
                  VIA.t1c = (VIA.t1lh << 8) | VIA.t1ll;
              }
              else
              {
                  // one shot mode 

                  if (VIA.t1int)
                  {
                      VIA.ifr |= 0x40;
                      if ((VIA.ifr & 0x7f) & (VIA.ier & 0x7f))
                      {
                          VIA.ifr |= 0x80;
                      }
                      else
                      {
                          VIA.ifr &= 0x7f;
                      }
                      VIA.t1pb7 = 0x80;
                      VIA.t1int = 0;
                  }
              }
          }
          if (VIA.ca2 == 0)
          {
              /* need to force the current point to the 'orgin' so just
              * calculate distance to origin and use that as dx,dy.
              */
              DAC.curr_x = DAC_MAX_X >> 1;
              DAC.curr_y = DAC_MAX_Y >> 1;
          }
          if (  ((VIA.acr & 0x80) && (VIA.t1pb7 == 0)) || (!(VIA.acr & 0x80)) && ((VIA.orb & 0x80) == 0)  )
          {
            DAC.curr_x += DAC.dx;
            DAC.curr_y += DAC.dy;
          }
    }
}


//int fdoExtraWait = 0;
void v_immediateDraw32Patterned(int8_t xEnd, int8_t yEnd, uint8_t pattern);

//do more bios functions!


void Draw_Line_d()
{
    SET_YSH_IMMEDIATE_8(CPU.reg_a);
     ADD_DELAY_CYCLES(DELAY_Y_SET_DIRECT);

    SET_XSH_IMMEDIATE_8(CPU.reg_b);
    I_SET(VIA_t1_cnt_hi, 0);
    SET_SHIFT_REG(0xff);
    while ((GET (VIA_int_flags) & 0x40) == 0);
    ADD_DELAY_CYCLES(DELAY_AFTER_T1_END_VALUE_DIRECT);
    SET_SHIFT_REG(0x00);
    CPU.reg_x += 2;
    CPU.reg_pc = 0xf3fb; // End of line draw in draw_line_d
    mustWork = 0;
    cycleCount += 100;
}


static int NANO_WAIT;
void vecx_direct(int32_t cycles)
{
    int ll = 0;
    NANO_WAIT = (cycleEquivalent-11);
    while (cycles > 0)
    {
        uint16_t icycles;
        mustWork = 1;
#ifdef PC_TESTS
        PC_TESTS
#endif
//        if (CPU.reg_pc < 0xe000) 
//        {
//          printf("%04x:\n\r", CPU.reg_pc);
//        }
// VecMania with its YM-Player
// each round DISABLES Port A of the PSG chip
// that is why joystick reading return always 0 == all buttons pressed!
// joystick button reading does not work here! /unless changing reg 7 of PSG, which I will not do)

#ifdef LINEART
// not implemented is a "real" vecFlash bankswitch
// but I know the cart :-)
        if (CPU.reg_pc == 0xc889) // lineart bankswitch
	{
	  currentBank = ram[0x81];
          CPU.reg_pc = 0x002f; // flash start
	  mustWork = 1;
	}
#endif

        if (CPU.reg_pc == 0xf192) //0xF192) // WaitRecal
        {
            if (vectrexButtonState == 0x1ff)
            {
              // if we have not read the joystick this round
              // we try to intercept WR
              vectrexButtonState = ~v_directReadButtons();
//              {
//                printf("%04x: (!)vectrexButtonState = = $%02x\n\r", CPU.reg_pc, vectrexButtonState);
//              }
            }
        }

#ifdef EMULATE_KNOWN_PRINTSTR 

        if (CPU.reg_pc == 0xF495) // printstr
        {
            cycles = PrintStr(cycles);
            icycles = 0;
        }
/*
        if (CPU.reg_pc == 0xF3DF) // Draw_Line_d
        {
            Draw_Line_d(cycles);
            icycles = 0;
        }
*/        
        
// vectrex on speed gets its
// own pattern draw routine
// the title screen looks better :-)
        else if (CPU.reg_pc == 0xF35B) // Reset_Pen
        {
/*
;-----------------------------------------------------------------------;
;       F312    Moveto_d                                                ;
;                                                                       ;
; This routine uses the current scale factor, and moves the pen to the  ;
; (y,x) position specified in D register.                               ;
;                                                                       ;
; ENTRY DP = $D0                                                        ;
;       A-reg = Y coordinate                                            ;
;       B-reg = X coordinate                                            ;
;                                                                       ;
;       D-reg trashed                                                   ;
;-----------------------------------------------------------------------;
;-----------------------------------------------------------------------;
;       F308    Moveto_ix_FF                                            ;
;       F30C    Moveto_ix_7F                                            ;
;       F30E    Moveto_ix_b                                             ;
;                                                                       ;
; These routines force the scale factor to 0xFF, 0X7F, or the           ;
; A register, and then move the pen to the (y,x) position pointed to    ;
; by the X-register.  The X-register is then incremented by 2.          ;
;                                                                       ;
; ENTRY DP = $D0                                                        ;
;       X-reg points to the (y,x) coordinate pair                       ;
;       B-reg contains the scale factor (Moveto_ix_b only)              ;
;                                                                       ;
; EXIT: X-reg has been incremented by 2                                 ;
;                                                                       ;
;       D-reg trashed                                                   ;
;-----------------------------------------------------------------------;
;-----------------------------------------------------------------------;
;       F35B    Reset_Pen                                               ;
;                                                                       ;
;       Reset the pen to the origin.                                    ;
;                                                                       ;
; ENTRY DP = $D0                                                        ;
;                                                                       ;
;       D-reg trashed                                                   ;
;-----------------------------------------------------------------------;
*/
// not NOT enought zero time
// not cranky related
// not cycle equivalent related
// DELAY_NOW(1000);
        }
        
        else if (CPU.reg_pc == 0xF312) // Moveto_d
	{
/*	  
;-----------------------------------------------------------------------;
;       F312    Moveto_d                                                ;
;                                                                       ;
; This routine uses the current scale factor, and moves the pen to the  ;
; (y,x) position specified in D register.                               ;
;                                                                       ;
; ENTRY DP = $D0                                                        ;
;       A-reg = Y coordinate                                            ;
;       B-reg = X coordinate                                            ;
;                                                                       ;
;       D-reg trashed                                                   ;
;-----------------------------------------------------------------------;
*/
UNZERO();
lastScale = currentScale = VIA.t1ll; // just start the timer

// 
	  v_moveToImmediate8(CPU.reg_b, CPU.reg_a);
          CPU.reg_pc = 0xf57d; // RTS - any
//          CPU.reg_pc = 0xF345; 
//          CPU.reg_b = 0x40; 
// printf("M %02x, %02x, %04x \n", CPU.reg_b, CPU.reg_a, currentScale);
          
          mustWork = 0;

	}
        
        

        else if (CPU.reg_pc == 0xF434) // pattern draw_a
        {
          uint8_t pattern = ram[0xC829&0x3ff];
          int count = CPU.reg_a;        
          uint16_t listStartAdr = (uint16_t) CPU.reg_x;
          uint8_t brightness = (uint8_t)DAC.zsh;

          v_setBrightness( brightness);
          v_setScaleForced(VIA.t1ll);
          CPU.reg_x += 2*(count+1);

          int x1;
          int y1;
          for (;count>=0;count--)
          {
            if (listStartAdr<0xc000)
            {
              y1 = ((signed char)cart[(listStartAdr++)+currentBank]);
              x1 = ((signed char)cart[(listStartAdr++)+currentBank]);
            }
            else
            {
              y1 = ((signed char)rom[((listStartAdr++)&0x1fff)  ]);
              x1 = ((signed char)rom[((listStartAdr++)&0x1fff)  ]);
            }
            v_immediateDraw32Patterned(x1, y1, pattern);
          }
//          ZERO_AND_CONTINUE();
//	  DELAY_NOW(10);
	  ZERO_AND_WAIT();

          CPU.reg_pc = 0xF34F;// check 0 ref 
          CPU.reg_pc = 0xf57d; // RTS
          mustWork = 0;
        }
        
#endif
        if (mustWork)
        {
//            isRamping = (( (VIA.acr & 0x80)?(VIA.t1pb7):(VIA.orb & 0x80)) == 0);
            // skip interrupt handling?
            // skip interrupt check when ramp is enabled
            // otherwise the adition cycles will "destroy" correct drawing
          
            // todo think of a similar cheat for 3d imager games.
            // as it is now, they will most likely be VERY slow! 
//            if ((( CPU.reg_cc & FLAG_I) == 0) && (( (VIA.acr & 0x80)?(VIA.t1pb7):(VIA.orb & 0x80)) != 0))

// lightpen interrupt only
// this does not work for scanlines!          
// for scanlines - ramping must be "ON"          
//          if ((( CPU.reg_cc & FLAG_I) == 0) && (( VIA.ier & 2 ) == 2) && ((( (VIA.acr & 0x80)?(VIA.t1pb7):(VIA.orb & 0x80)) != 0)) ))
          if ((( CPU.reg_cc & FLAG_I) == 0) && (( VIA.ier & 2 ) == 2))
          {
                icycles = e6809_sstep(V_GET(0xD00D) & 0x80, 0);
          }
          else
            
/*            
#ifdef THE_SPIKE_MUST_FLOW      
            
// T1 interrupt (Spike)            
          if ( (( CPU.reg_cc & FLAG_I) == 0) &&  (( VIA.ier & 0x40 ) == 0x40))
                icycles = e6809_sstep(VIA.ifr & 0x80, 0);
            else
#endif              

// T2 Bedlam runs in a T2 interrup loop with CWAI            
              
              
              icycles = e6809_sstep(0, 0);
*/            
                icycles = e6809_sstep(VIA.ifr & 0x80, 0);
            
 
#ifdef EMULATE_KNOWN_PRINTSTR
/*
 * TARGET should be to emulate EACH 6809 instruction within 2 vectrex cycles! (excluding VIA access)
 * at the moment each instrucziopn takes about 1500 cycles - we must reduce this to 1200 cycles (this time ARM cycles)
 * 
 * With below that is to high... ... otherwise it seems ok 99.9% of the time

  This stuff is realy only needed, when 
 coordinates must be intercepted (captured printStr_d e.g.)
 with wait emulation atm we do not interecpt print_str
 we are by now DAMN exact!!!
*/            
            miniStep(icycles);
#ifndef WAIT_EMULATION
#endif
            
//#ifdef WAIT_EMULATION
//            miniStep(icycles);
//#else
//            miniStep2(icycles);
/*
             for (uint16_t c = 0; c < icycles; c++)
            {
                via_sstep0();
                dac_sstep();
                via_sstep1();
            }
*/            
//#endif              

#else
#ifdef THE_SPIKE_MUST_FLOW      
  t1MiniStep(icycles);  
#endif
  
#ifdef BEDLAM_T2_HANDLING
  t2miniStep(icycles);
  
#endif            
#endif            
            
            
#ifdef WAIT_EMULATION   

// TODO
// EACH VIA READ happens two cycles to early,
// since the read is "direct" and not delayed
// since HERE we always wait for "absolut" time (not relative),
// to be more exact, we could ALLWAYS wait 2 cycles
// before we do a VIA - READ,
// this should be always correct! Since 6809 instructions need to cycles to "start"
// also the read itself would take 2 cycles, so
// even a CLR <VIA_PortB
// type instruction would be back to "write" in 4 cycles... and use another 2 cycles for the write
// -> CORRECT!

              if (viaAccessCounter==0) 
              {
                // theoretically only when
                // ramp active
                // or the first "X" cycles of zeroing!
                // zeroing seems to be not relevant (yet???)
#ifndef EXACT_WAIT_EMULATION
                if (( (VIA.acr & 0x80)?(VIA.t1pb7):(VIA.orb & 0x80)) == 0) // only if ramping  // 
#endif                
                {
                  waitUntil((icycles)*NANO_WAIT);
                }
                  
                  
                // TODO
                // if we don't "overstep" the waiting if no VIA is used (ramp/zero)
                // THAN
                // we could sync with VIA for every TWO cylces we wait
                // with a VIA read!!!
                // provided we have about 1200 cycles left for syncing!
              }
              else if (viaAccessCounter==1) 
              {
                // one VIA access takes 2 cycles
                // so before the wait for the VIA
                // we wait 2 cycles less!
                
                // must not be less than zero!!!

                waitUntil((icycles-2)*NANO_WAIT);
                V_SET(delayVia[0], delayData[0]);
                viaAccessCounter=0;
              }
              else if (viaAccessCounter==2) // this is something like std <VIA_portB
              {
                // This is a "double" write to VIA
                // the two writes take 4 cycles, so we wait even
                // less time before we start writing to VIA

                // due to experiments... this must be
                // 3 cycles less instead of 4
                // 4 cycles less is too fast, 
                // e.g. in print rouines 5 font
                // y integrator is not
                // completely reset to 0 if it is -4
                // to replicate the behaviour of the correct vectrex internal timing here use -3!!!
                
                // must not be less than zero!!!
                waitUntil((icycles-3)*NANO_WAIT);
                V_SET(delayVia[0], delayData[0]);
                V_SET(delayVia[1], delayData[1]);
                viaAccessCounter=0;
              }
              // reset cycle counter
              // at the END of the current instruction-> the new one starts
              PMNC(CYCLE_COUNTER_ENABLE|CYCLE_COUNTER_RESET);
#endif            
            cycles -= icycles;
//            cycleCount += icycles;
        }
    }
}

