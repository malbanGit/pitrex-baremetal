/***************************************************************************
machine\starwars.c

STARWARS MACHINE FILE

This file is Copyright 1997, Steve Baines.
Modified by Frank Palazzolo for sound support

Release 2.0 (6 August 1997)

See drivers\starwars.c for notes

***************************************************************************/
#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

#include "../driver.h"
#include "../cpuintrf.h"
#include "../machine/swmathbx.h"
#include "../vidhrdw/swavg.h"
#include "../input.h"
#include "../log.h"

static int bankaddress = 0x10000; /* base address of banked ROM */

/*   If 1 then log functions called */
#define MACHDEBUG 0
#define SNDDEBUG 0

static unsigned char ADC_VAL=0;

/**********************************************************/
/**********************************************************/
/************** Read Handlers *****************************/

/*
int math_ram_r(int offset)
   {
   printf("MRAM_RD: (%x), %x\n",0x5000+offset, RAM[0x5000+offset]);
   return RAM[0x5000+offset];
   }
*/

/********************************************************/

/*
void math_ram_w(int offset, int data)
   {
   printf("MRAM_WR: (%x), %x\n",0x5000+offset, data);
   RAM[0x5000+offset]=data;
   }
*/

/********************************************************/

/* Read from ROM 0. Use bankaddress as base address */
int banked_rom_r(int offset)
   {
     return RAM[bankaddress + offset];
   }
/************************************************
PORT_BIT ( 0x01, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BIT ( 0x02, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT ( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 )
	PORT_BIT ( 0x08, IP_ACTIVE_LOW, IPT_TILT )
	PORT_SERVICE( 0x10, IP_ACTIVE_LOW )
	PORT_BIT ( 0x20, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT ( 0x40, IP_ACTIVE_LOW, IPT_BUTTON4 )
	PORT_BIT ( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 )

    PORT_BIT ( 0x01, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT ( 0x02, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BITX( 0x04, IP_ACTIVE_LOW, IPT_SERVICE, "Diagnostic Step", KEYCODE_F1, IP_JOY_NONE )
	PORT_BIT ( 0x08, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT ( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 )
	PORT_BIT ( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 )
	// Bit 6 is MATH_RUN - see machine/starwars.c 
	//PORT_BIT ( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	// Bit 7 is VG_HALT - see machine/starwars.c 
	//PORT_BIT ( 0x80, IP_ACTIVE_HIGH, IPT_UNUSED )
********/
int input_bank_0_r(int offset)
   {
   int x=getport(0);
  // static int service=0;
   // x=x&0xdf; /* Clear out bit 5 (SPARE 1) */
   
   //x=input_port_0_r(0); /* Read memory mapped port 1 */
  // log_it("Input Port 0 val %x",x);
#ifndef NO_PI
  
  if ((currentButtonState & (1+2))==1+2)  x=x & (~(0x02));  //Coin 1
 // if (key[config.kcoin2]) x-=0x01;  //Coin 2
//  if (key[KEY_B]) x-=0x04;  //Coin 2
  //if (KeyCheck(KEY_F2)) service^=1;
  //if (service)x-=0x10; //Service
  
  
//  if (key[KEY_A]) x-=0x08; //Tilt
  //if (key[config.kp1but4]) x-=0x40; //Button 4
 // if (key[config.kp1but1]) x-=0x80; //Button 1

  if (currentButtonState & 8) x=x & (~(0x40)); //Button 4
  if (currentButtonState & 1) x=x & (~(0x80)); //Button 1


//printf("CB 0; %02x", currentButtonState);   
#endif
   #if(MACHDEBUG==1)
   printf("(%x)input_bank_0_r   (returning %xh)\n", cpu_getpc(), x);
   #endif
//   printf("(%x)input_bank_0_r   (returning %xh)\n", cpu_getpc(), x);
     
   return x;
   }
/********************************************************/
int input_bank_1_r(int offset)
   {
   int x=getport(1);//0x34;
  
   //x=input_port_1_r(0); // Read memory mapped port 2
   // log_it("Input Port 1 val %x",x);
   
  // x=x&0x34; /* Clear out bit 3 (SPARE 2), and 0 and 1 (UNUSED) */
             /* MATH_RUN (bit 7) set to 0 */
 

#if 0
   x=x|(0x40);  /* Set bit 6 to 1 (VGHALT) */
#endif

/* Kludge to enable Starwars Mathbox Self-test                  */
/* The mathbox looks like it's running, from this address... :) */
   if (cpu_getpc() == 0xf978)
        x|=0x80; 

   if (vg_done(cpu_gettotalcycles()))
	x|=0x40;
   else
	x&=~0x40;
   #if(MACHDEBUG==1)
   printf("(%x)input_bank_1_r   (returning %xh)\n", cpu_getpc(), x);
   #endif
   #ifndef NO_PI
  // if (key[KEY_F3]) x-=0x04; //Service
   //if (key[config.kp1but3])  x-=0x10; //Button 3
   //if (key[config.kp1but2])  x-=0x20; //Button 2

  if (currentButtonState & 4) x=x & (~(0x10)); //Button 3
  if (currentButtonState & 2) x=x & (~(0x20)); //Button 1
//printf("CB 1; %02x", currentButtonState);   
#endif
   return x;
   }
/*********************************************************/

/* Dip switch bank zero */
int opt_0_r(int offset)
{
   int x;
   x= 0x80;//input_port_2_r(0); /* Read DIP switch bank 0 */
   #if(MACHDEBUG==1)
   printf("(%x)opt_0_r   (Returning: %xh\n", cpu_getpc(), x);
   #endif
//   printf("(%x)opt_0_r   (Returning: %xh\n", cpu_getpc(), x);
   //return x;
   return 0xc0;
   
}

/********************************************************/

/* Dip switch bank 1 */
int opt_1_r(int offset)
   {
//   int x;
//   x= 0x0e;//input_port_3_r(0); /* Read DIP switch bank 1 */
   #if(MACHDEBUG==1)
   printf("(%x)opt_1_r   (Returning: %xh\n", cpu_getpc(), x);
   #endif
   //return x;
   return 0x02;
   }

/********************************************************/
int adc_r(int offset)
   {
   #if(MACHDEBUG==1)
   printf("adc_r [Returning %d]\n",ADC_VAL);
   #endif
   return ADC_VAL;
   }

 
/************************************************************/
/************************************************************/
/************** Write Handlers ******************************/

/********************************************************/
void irqclr(int offset, int data)
   {
   #if(MACHDEBUG==1)
   printf("irqclr\n");
   #endif
   }
/**************************************************/
void led3(int offset, int data)
   {
   #if(MACHDEBUG==1)
   printf("led3 [%d]\n",((data&0x80)>>7) );
   #endif
   }
/**************************************************/
void led2(int offset, int data)
   {
   #if(MACHDEBUG==1)
   printf("led2 [%d]\n",((data&0x80)>>7) );
   #endif
   }
/**************************************************/

/* Switch bank of ROM 0 */
void mpage(int offset, int data) /* MSB toggles bank */
{
if ((data & 0x80)==0)
    bankaddress = 0x10000; /* First half of ROM */
else
    bankaddress = 0x12000; /* Second half of ROM */

   #if(MACHDEBUG==1)
   printf("mpage [Page %d]\n",((data&0x80)>>7) );
   #endif
}

/**************************************************/
void led1(int offset, int data)
   {
   #if(MACHDEBUG==1)
   printf("led1 [%d]\n",((data&0x80)>>7) );
   #endif
   }
/**************************************************/
void recall(int offset, int data)
   {
   #if(MACHDEBUG==1)
   printf("recall\n");
   #endif
   }
/********************************************************/
void nstore(int offset, int data)
   {
   #if(MACHDEBUG==1)
   printf("nstore\n");
   #endif
   }

/********************************************************/
/********************************************************/

void adcstart0(int offset, int data) /* PITCH control */
{
//      static int PITCH=167;//A7

      //int delta=0;
     // #define KEYMOVE 2
	 // if (key[KEY_UP])   {if(PITCH-KEYMOVE>=0) PITCH -= KEYMOVE;}
	 // if (key[KEY_DOWN]) { if(PITCH+KEYMOVE<=255) PITCH += KEYMOVE;}
	
	  
     // PITCH=joy[0].stick[0].axis[0].pos;
/* 	keyboard & joystick support	
        if(osd_key_pressed(OSD_KEY_DOWN) || osd_joy_pressed(OSD_JOY_UP))
                if(PITCH-KEYMOVE>=0) PITCH -= KEYMOVE;
        if(osd_key_pressed(OSD_KEY_UP) || osd_joy_pressed(OSD_JOY_DOWN))
                if(PITCH+KEYMOVE<=255) PITCH += KEYMOVE;
 */
/* 	mouse support */
        //delta = readtrakport(1);

       // if( ((PITCH-delta)>=0) & ((PITCH-delta)<=255)) PITCH -= delta;

ADC_VAL=getport(3);//PITCH;
ADC_VAL = currentJoy1Y+128;
}

/********************************************************/

void adcstart1(int offset, int data) // YAW control
{
//        static int YAW=169; //A9

        //#define KEYMOVE 2

      // int delta=0;

	   //if (key[KEY_RIGHT])   {if(YAW+KEYMOVE<=255) YAW += KEYMOVE;}
	   //if (key[KEY_LEFT])    { if(YAW-KEYMOVE>=0) YAW -= KEYMOVE;}
       //if (joy[0].stick[0].flags & JOYFLAG_ANALOGUE)	{YAW=joy[0].stick[0].axis[1].pos;}
	  // YAW=joy[0].stick[0].axis[1].pos;
/* 	keyboard & joystick support	
        if(osd_key_pressed(OSD_KEY_RIGHT) || osd_joy_pressed(OSD_JOY_RIGHT))
                if(YAW+KEYMOVE<=255) YAW += KEYMOVE;
        if(osd_key_pressed(OSD_KEY_LEFT) || osd_joy_pressed(OSD_JOY_LEFT))
                if(YAW-KEYMOVE>=0) YAW -= KEYMOVE;
 */
/* 	mouse support */
       // delta = readtrakport(0);

		//if( ((YAW+delta)>=0) & ((YAW+delta)<=255)) YAW += delta;

ADC_VAL=getport(6);//YAW;
ADC_VAL = currentJoy1X+128;


}

/********************************************************/
void adcstart2(int offset, int data)
   {
/* Unfortunately, the game doesn't ever seem to look at this
   so despite the schematic labelling, there isn't a thrust
   control after all. Oh well. */

   static unsigned int THRUST=0;
   #if(MACHDEBUG==1)
   printf("adcstart2\n");
   #endif
   ADC_VAL=THRUST;
   }
/********************************************************/

int starwars_trakball_r(int data)
{
    #define MAXMOVE 8

	data = data >> 1;
	if(data > MAXMOVE)
		data = MAXMOVE;
	else if(data < -MAXMOVE)
		data = -MAXMOVE;
	return data;
}

int starwars_interrupt(void)
   {
#if(VIDEBUG==1)
printf("starwars_interrupt\n");
#endif
/*   vblank=1;  */
   return interrupt();
   }


