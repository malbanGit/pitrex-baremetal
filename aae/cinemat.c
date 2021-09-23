#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

/* Cinematronics Emu */
#include "ccpu.h"
#include "cinemat.h"
#include "globals.h"
#include "samples.h"
#include "vector.h"
//#include "glcode.h"
#include "dips.h"
//#include "keysets.h"
#include "input.h"
#include "ccpu.h"
#include "log.h"


extern char *gamename[];
extern int gamenum;

static int cframe=0;

/////NEW VARIABLES AND STUFF
static UINT16 *rambase;

static UINT8 coin_detected;
static UINT8 coin_last_reset;
static UINT8 mux_select;

/*************************************
 *
 *  Coin handlers
 *
 *************************************/
static void input_changed( int coin_inserted )
{
	/* on the falling edge of a new coin, set the coin_detected flag */
	if (coin_inserted == 0)
		coin_detected = 1;
}

static int coin_read( int coin_input_r )
{
	return !coin_detected;
}



static int speedfrk_wheel_r( int offset )
{
    static UINT8 speedfrk_steer[] = {0xe, 0x6, 0x2, 0x0, 0x3, 0x7, 0xf};
	static int last_wheel=0, delta_wheel, last_frame=0, gear=0xe0;
	int  current_frame; //val,
	
/*
	// check the fake gear input port and determine the bit settings for the gear 
	if ((input_port_5_r(0) & 0xf0) != 0xf0)
        gear = input_port_5_r(0) & 0xf0;

    val = (input_port_1_word_r(0, 0) & 0xff00) | gear;

	// add the start key into the mix 
	if (input_port_1_word_r(0, 0) & 0x80)
        val |= 0x80;
	else
        val &= ~0x80; 
*/
	// and for the cherry on top, we add the scrambled analog steering 
    current_frame = cframe;//cpu_getcurrentframe();
    if (current_frame != last_frame)
    {
		
        // the shift register is cleared once per 'frame' 
        delta_wheel = getport(4) - last_wheel;
		log_it("DELTA WHEEL %d",delta_wheel);
        last_wheel += delta_wheel;
        if (delta_wheel > 3)
            delta_wheel = 3;
        else if (delta_wheel < -3)
            delta_wheel = -3;
    }
    last_frame = current_frame;

	return (speedfrk_steer[delta_wheel + 3] >> offset) & 1;
    //val |= speedfrk_steer[delta_wheel + 3];

	//return val;
}



/*************************************
 *
 *  Speed Freak inputs
 *
 *************************************/
/*
static speedfrk_wheel_r( int offset )
{
	static const UINT8 speedfrk_steer[] = {0xe, 0x6, 0x2, 0x0, 0x3, 0x7, 0xf};
	int delta_wheel;

    // the shift register is cleared once per 'frame' 
    delta_wheel = (INT8) getport(4) / 8;//readinputportbytag("WHEEL") / 8;
    if (delta_wheel > 3)
        delta_wheel = 3;
    else if (delta_wheel < -3)
        delta_wheel = -3;

    return (speedfrk_steer[delta_wheel + 3] >> offset) & 1;
}

*/
static int speedfrk_gear_r( int offset )
{
	static int gear = 0x0e;
    int gearval = getport(2);

	offset -=4; //Adjust offset from Mame values
	// check the fake gear input port and determine the bit settings for the gear 
	if ((gearval & 0x0f) != 0x0f)
        gear = gearval & 0x0f;

	// add the start key into the mix -- note that it overlaps 4th gear 
	if (!(getport (0) & 0x80))
        gear &= ~0x08;
    
	return (gear >> offset) & 1;
}



/**************************************
 *
 *  Boxing Bugs inputs
 *
 *************************************/

static  int boxingb_dial_r( int offset )
{
	UINT16 value = getport(3);//3
    offset -= 0x0b;
   
    if (!MUX_VAL) offset += 4;
	return (value >> offset) & 1;
}


/*************************************
 *
 *  General input handlers
 *
 *************************************/

UINT8 get_ccpu_inputs( int offset)
{ 

 switch (gamenum)
 {
 
 case BOXINGB:  {
				 if (offset > 0xb )  return  boxingb_dial_r(offset); 
		         else 
		           return (getport(0) >> offset) & 1;
				 break;
				}
 
 
 
 case SPEEDFRK: {
	             if (offset < 0x04 )  return  speedfrk_wheel_r(offset); 
		         else if (offset > 0x03 && offset < 0x07 )  return  speedfrk_gear_r(offset);
		         else if (offset > 0x06 ) return (getport(0) >> offset) & 1; 
				 break;
				}

	  
 default: return (getport(0) >> offset) & 1;
 
 
 }
	
	 return 0;

}


UINT8 get_ccpu_switches( int offset )
{
	static const UINT8 switch_shuffle[8] = { 2,5,4,3,0,1,6,7 };
	static int coindown=0;
	UINT16 test = getport(1) - z80dip1; //Add dip switches on
	
	if (offset ==7)
	{
	if ((test & 0x80) && coindown) {coindown--;} //Clear Countdown
	else if ((test & 0x80) == 0 && coindown) {bitset(test,0x80);} //Ignore Coin down. 
	else if ((test & 0x80) == 0 && coindown==0) {coindown=2;} //Set coin countdown
	 
	}
	 
	return (test >> switch_shuffle[offset]) & 1;
}



/*************************************
 *
 *  General output handlers
 *
 *************************************/

void coin_handler( int data )//coin_reset_w
{
	//log_it("Coin handler write %x",data);
	 //logerror("Coin port write %x\n",data);
	/* on the rising edge of a coin reset, clear the coin_detected flag */
	if (coin_last_reset != data && data != 0)
	{
		coin_detected = 0;
		//log_it("CLEARING COIN DETECTED");
	}
	coin_last_reset = data;
	// log_it("Coin_detected value HERE %d ",coin_detected);
}

static void mux_set( int data ) // mux_select_w
{
	mux_select = data;
	//log_it("MUX SELECT");
	//cinemat_sound_control_w(machine, 0x07, data);
}
UINT8 joystick_read(void)
{
	int xval = (INT16)(cpunum_get_reg(0, CCPU_X) << 4) >> 4;
	//log_it("joystick read");
	
	if (MUX_VAL) return (getport(6)  - xval) < 0x800;
	else return (getport(3)  - xval) < 0x800;
	
	return 0;
}


/*************************************
 *
 *  Sundance inputs
 *
 *************************************/

static const struct
{
	int portname;//const char *portname;
	UINT16 bitmask;
} sundance_port_map[16] =
{
	{ 1, 0x155 },	/* bit  0 is set if P1 1,3,5,7,9 is pressed */
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },

	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },

	{ 2, 0x1a1 },	/* bit  8 is set if P2 1,6,8,9 is pressed */
	{ 1, 0x1a1 },	/* bit  9 is set if P1 1,6,8,9 is pressed */
	{ 2, 0x155 },	/* bit 10 is set if P2 1,3,5,7,9 is pressed */
	{ 0, 0 },

	{ 1, 0x093 },	/* bit 12 is set if P1 1,2,5,8 is pressed */
	{ 2, 0x093 },	/* bit 13 is set if P2 1,2,5,8 is pressed */
	{ 1, 0x048 },	/* bit 14 is set if P1 4,8 is pressed */
	{ 2, 0x048 },	/* bit 15 is set if P2 4,8 is pressed */
};


static int sundance_read(int offset, int value ) //static READ8_HANDLER( sundance_inputs_r )
{
	// handle special keys first 
	if (sundance_port_map[offset].portname)
		return (getport(sundance_port_map[offset].portname) & sundance_port_map[offset].bitmask) ? 0 : 1;
	else
		return (getport(0) >> offset) & 1;
}

/*************************************
 *
 *  Boxing Bugs inputs
 *
 *************************************/

static int bbug_read(int offset, int value )// boxingb_dial_r
{
	//int value = readinputportbytag("DIAL");
	if (!mux_select) offset += 4;
	return (value >> offset) & 1;
}

/*************************************
 *
 *  CPU configurations
 *
 *************************************/

static struct CCPUConfig config_nojmi =
{
	joystick_read,
	cinemat_vector_callback
	
};

static struct CCPUConfig config_jmi =
{
	NULL,
	cinemat_vector_callback
	
};
////////////////////////////////////////////////////
/////END OF NEW CODE//////
////////////////////////////////////////////////////


void (*sound_write) (unsigned char,unsigned char);
void (*get_cini_input) ();
//int vb=0;
int cinemat_outputs=0xff;

int thisframe=0;
int lastframe=0;


UINT8 bFlipX;
UINT8 bFlipY;

int ioInputs = 0xffff; 
int ioSwitches = 0xfff0;
unsigned int ioOutputs = 0xff;

UINT8 bNewFrame;
UINT32 current_frame = 0;




///////////////////////////////////////////////////////////////////////////////////////
//Input Handlers
/*
void sundance_input()
{
	int x=1;
	int y=1;
 //SUNDANCE
    ioInputs = 0xffff; 
    ioSwitches = 0xffee; //fff0 = 2 min c0 = 1.30 min  d0 = 5 min e0 = 3 min
//    if (GkeyCheck(config.kcoin1)){ioSwitches &= ~SW_COIN;}	
   
    if (key[config.kstart1]) {ioInputs -= 0x0004;} //start 1
    if (key[config.kstart2]) {ioInputs -= 0x0008;} //start2
    if (key[config.kp1but1]) {ioInputs -= 0x0020;} //grid
	if (key[config.kp1but2]) {ioInputs -= 0x0800;} //2suns
	if (key[config.kp1but3]) {ioInputs -= 0x0010;} //3 suns 
    if (key[config.kp1but4]) {ioInputs -= 0x0040;} //4 suns 
    //Player 1
	if (key[config.pad0])   {ioInputs -= 0x0002;}
	if (x){if (key[config.pad9])   {ioInputs -= 0x0001;x=0;}}
    if (x){if (key[config.pad8])   {ioInputs -= 0x1000;x=0;}}   
	if (x){if (key[config.pad7])   {ioInputs -= 0x1201;x=0;}}
	if (x){if (key[config.pad6])   {ioInputs -= 0x0200;x=0;}}
	if (x){if (key[config.pad5])   {ioInputs -= 0x1001;x=0;}}
	if (x){if (key[config.pad4])   {ioInputs -= 0x4000;x=0;}}
	if (x){if (key[config.pad3])   {ioInputs -= 0x0201;x=0;}}
	if (x){if (key[config.pad2])   {ioInputs -= 0x1200;x=0;}}
	if (x){if (key[config.pad1])   {ioInputs -= 0x4001;x=0;}}
	//Player 2
	if (key[config.kp2but6])          {ioInputs -= 0x0080;}
	if (y){if (key[config.kp2right])  {ioInputs -= 0x8400;y=0;}}//2500 1
    if (y){if (key[config.kp2left])   {ioInputs -= 0x2100;y=0;}}//2000 2  
	if (y){if (key[config.kp2up])     {ioInputs -= 0x0500;y=0;}}//0400 3
	if (y){if (key[config.kp2down])   {ioInputs -= 0x8000;y=0;}}//8000 4
	if (y){if (key[config.kp2but1])   {ioInputs -= 0x2400;y=0;}}//2400 5
	if (y){if (key[config.kp2but2])   {ioInputs -= 0x0100;y=0;}}//0100 6 
	if (y){if (key[config.kp2but3])   {ioInputs -= 0x2500;y=0;}}//8400 7
	if (y){if (key[config.kp2but4])   {ioInputs -= 0x2000;y=0;}}//2100 8
	if (y){if (key[config.kp2but5])   {ioInputs -= 0x0400;y=0;}}//0500 9

}

void solarq_input()
{
   //Solar Quest
   ioInputs = 0xffff; 
   ioSwitches = 0x00bb; //9 bit 2 of 3rd switch is diag mode
   if (testsw){ioSwitches+=0x40;}

  // if (GkeyCheck(config.kcoin1)){ioSwitches &= ~SW_COIN;}
   if (key[config.kstart1]) {ioInputs -=0x0008;}
   if (key[config.kstart2]) {ioInputs -=0x0001;} 

    if (key[config.kp1left])   {ioInputs -= 0x0020;}
	if (key[config.kp1right])  {ioInputs -= 0x0010;}
	if (key[config.kp1but1])   {ioInputs -= 0x0002;}
	if (key[config.kp1but2])   {ioInputs -= 0x0004;}
	if (key[config.kp1but3])   {ioInputs -= 0x0008;}
	if (key[config.kp1but4])   {ioInputs -= 0x0001;}
}



void warrior_input()
{
   //WARRIOR
   ioInputs = 0xffff; 
   ioSwitches = 0x009e; //00ff=2min 00ef=1min  00df 1:30 00cf 45sec
  // if (GkeyCheck(config.kcoin1)){ioSwitches &= ~SW_COIN;}
    if (key[config.kstart1])   {ioInputs -= 0x4000;}
    if (key[config.kstart2])   {ioInputs -= 0x2000;}
    //P1
    if (key[config.kp1right])  {ioInputs -= 0x0100;}
	if (key[config.kp1left])   {ioInputs -= 0x0200;}
	if (key[config.kp1up])     {ioInputs -= 0x0400;}
	if (key[config.kp1down])   {ioInputs -= 0x0800;}
	if (key[config.kp1but1])   {ioInputs -= 0x1000;}
	//P2
	if (key[config.kp2right])  {ioInputs -= 0x0001;}
	if (key[config.kp2left])   {ioInputs -= 0x0002;}
	if (key[config.kp2up])     {ioInputs -= 0x0004;}
	if (key[config.kp2down])   {ioInputs -= 0x0008;}
	if (key[config.kp2but1])   {ioInputs -= 0x0010;}
}


void spacewar_input()
{
  //Spacewar inputs
   ioInputs = 0xffff; 
   ioSwitches = 0x00cf; //00ff=45 00ef=2min  00df 1:30 00cf 1min
  // if (GkeyCheck(config.kcoin1)){ioSwitches &= ~SW_COIN;}
 
    if (key[config.kp1left])   {ioInputs -= 0x0100;}
	if (key[config.kp1right])  {ioInputs -= 0x2000;}
	if (key[config.kp1but1])   {ioSwitches -= 0x04;}
	if (key[config.kp1but2])   {ioInputs -= 0x8000;}
	if (key[config.kp1but3])   {ioSwitches -= 0x02;}

	if (key[config.kp2left])   {ioInputs -= 0x4000;}
	if (key[config.kp2right])  {ioInputs -= 0x1000;}
	if (key[config.kp2but1])   {ioSwitches -= 0x01;}
	if (key[config.kp2but2])   {ioInputs -= 0x0200;}
	if (key[config.kp2but3])   {ioSwitches -= 0x08;}
    
    if (key[config.pad9])   {ioInputs -= 0x0008;}
	if (key[config.pad8])   {ioInputs -= 0x0002;}
	if (key[config.pad7])   {ioInputs -= 0x0080;}
	if (key[config.pad6])   {ioInputs -= 0x0020;}
	if (key[config.pad5])   {ioInputs -= 0x0400;}
	if (key[config.pad4])   {ioInputs -= 0x0004;}
	if (key[config.pad3])   {ioInputs -= 0x0001;}
	if (key[config.pad2])   {ioInputs -= 0x0040;}
	if (key[config.pad1])   {ioInputs -= 0x0010;}
	if (key[config.pad0])   {ioInputs -= 0x0800;}
	
}

*/
/*

static void speedfrk_input()
{  
	static UINT8 speedfrk_steer[] = {0xe, 0x6, 0x2, 0x0, 0x3, 0x7, 0xf};
    static int last_wheel=0, delta_wheel, last_frame=0, gear=0xe0;
	int val, current_frame;

    ioInputs = 0xff00; 
    ioSwitches = 0x00f0; 
   if (testsw){ioSwitches+=0x40;}
//   if (GkeyCheck(config.kcoin1)){ioSwitches &= ~SW_COIN;}
   if (key[config.kstart1])   {ioInputs &= ~0x80;}
   
}


void barrier_input()
{  //Barrier inputs
   ioInputs = 0xffff; 
   ioSwitches = 0xfff0; //00ff=45 00ef=2min  00df 1:30 00cf 1min
//   if (GkeyCheck(config.kcoin1)){ioSwitches &= ~SW_COIN;}
 
    if (key[config.kstart1])   {ioInputs -= 0x0800;}
    if (key[config.kstart2])   {ioInputs -= 0x0010;}
    
	if (key[config.kp1left])   {ioInputs -= 0x4000;}
	if (key[config.kp1right])  {ioInputs -= 0x0200;}
	if (key[config.kp1up])     {ioInputs -= 0x1000;}
    if (key[config.kp1down])   {ioInputs -= 0x0008;}
	//Skill
	if (key[config.kp1but1])   {ioInputs -= 0x0001;}
	if (key[config.kp1but2])   {ioInputs -= 0x0004;}
	if (key[config.kp1but3])   {ioInputs -=0x0040;}

	if (key[config.kp2left])   {ioInputs -= 0x0100;}
	if (key[config.kp2right])  {ioInputs -= 0x8000;}
	if (key[config.kp2up])     {ioInputs -= 0x2000;}
    if (key[config.kp2down])   {ioInputs -= 0x0400;}
}

void demon_input()
{
   //DEMON
   ioInputs = 0xffff; 
   ioSwitches = 0xffff; //00ff=2min 00ef=1min  00df 1:30 00cf 45sec
//   if (GkeyCheck(config.kcoin1)){ioSwitches &= ~SW_COIN;}
   
   if (key[config.kstart1]) {ioInputs -=0x01;}
   if (key[config.kstart2]) {ioInputs -=0x02;}
    //p1  
   if (key[config.kp1left])   {ioInputs -= 0x0004;}
   if (key[config.kp1right])  {ioInputs -= 0x0008;}
   if (key[config.kp1but1])   {ioInputs -= 0x0020;}
   if (key[config.kp1but2])   {ioInputs -= 0x0010;}
   if (key[config.kp1but3])   {ioInputs -= 0x0200;}
   //p2
   if (key[config.kp2left])   {ioInputs -= 0x0800;}
   if (key[config.kp2right])  {ioInputs -= 0x1000;}
   if (key[config.kp2but1])   {ioInputs -= 0x4000;}
   if (key[config.kp2but2])   {ioInputs -= 0x2000;}
   if (key[config.kp2but3])   {ioInputs -= 0x0400;}
    
}

*/

void run_cinemat(void)
{
	
	int b=1;
	//log_it("frame start");
	//log_it("GETPORT MOUSE %x",getport(3));
   	b=run_ccpu(19923000/4/38);//131072);19923000	
	cinevid_up();	
	
	cframe++;
	if (cframe > 70) cframe=0;

    if (KeyCheck(config.kreset))  {ccpu_reset();}
	//if (KeyCheck(config.ktest))      {testsw^=1;}
    // if (KeyCheck(config.kreset))     {cineReset();}
		
}


 
int init_cinemat(void)
{
 	 log_it("start init");	
     //SET DEFAULTS
	 bSwapXY = 0;    // (for vector generation)
//     cache_clear();
	 ioInputs = 0xffff; ioSwitches = 0xfff0;  //(cleared values)
     cinemat_outputs=0xff; //0xff
	 ioOutputs = 0xff;
	 init_snd();
     sound_write=&null_sound;
 
	// reset the coin states
	coin_detected = 0;
	coin_last_reset = 0;

	// reset mux select 
	mux_select = 0;
    MUX_VAL = 0;
    SOUNDBITS = 0;
	//Set Some Defaults
	CCPUROMSIZE=4;
	//init_ccpu(0);
	video_type_set(COLOR_BILEVEL); 

    switch (gamenum)
	{

    case STARHAWK: sound_write=&null_sound;//sound_write=&starhawk_sound;
		           video_type_set(COLOR_BILEVEL);
		          // ccpu_Config(1, CCPU_MEMSIZE_4K, CCPU_MONITOR_BILEV);
				   CCPUROMSIZE=4;
				   init_ccpu(0);
		           break;
	case RIPOFF:   sound_write=&ripoff_sound;
				   video_type_set(COLOR_BILEVEL);
				   //ccpu_Config(1, CCPU_MEMSIZE_8K, CCPU_MONITOR_BILEV);
				   CCPUROMSIZE=8;
				   init_ccpu(0);
				   break;

	case SOLARQ: sound_write=&solarq_sound;
				// ccpu_Config(1, CCPU_MEMSIZE_16K, CCPU_MONITOR_64LEV);
				 video_type_set(COLOR_64LEVEL);
				 CCPUROMSIZE=16;
				 init_ccpu(0);
                 
				 break;
	case STARCAS: sound_write=&starcas_sound;
				  //ccpu_Config(1, CCPU_MEMSIZE_8K, CCPU_MONITOR_BILEV);
				  CCPUROMSIZE=8;
				  video_type_set(COLOR_BILEVEL);
				  init_ccpu(0);
				  break;
	case ARMORA: sound_write=&armora_sound;
				 //ccpu_Config(1, CCPU_MEMSIZE_16K, CCPU_MONITOR_BILEV);
				 CCPUROMSIZE=16;
				 video_type_set(COLOR_BILEVEL);
				 init_ccpu(0);
				 break;
	case BARRIER: bSwapXY = 1; sound_write=&barrier_sound;
				  video_type_set(COLOR_BILEVEL);
				  init_ccpu(0);
				  CCPUROMSIZE=8;
				  break;
	case SUNDANCE: bSwapXY = 1;sound_write=&sundance_sound;
				   video_type_set(COLOR_16LEVEL);
				   CCPUROMSIZE=8;
				   init_ccpu(0);
				   break;
	case WARRIOR: sound_write=&warrior_sound;
				  CCPUROMSIZE=8;
				  init_ccpu(0);
				  break;
	
	case TAILG: sound_write=&tailg_sound;
				video_type_set(COLOR_BILEVEL);
				CCPUROMSIZE=8;
				init_ccpu(1);
				break;
	
	case SPACEWAR: sound_write=&spacewar_sound;
				   CCPUROMSIZE=4;
				   video_type_set(COLOR_BILEVEL);
				   init_ccpu(1);
				   break;

	case SPEEDFRK: sound_write=&null_sound;
				   CCPUROMSIZE=8;
				   video_type_set(COLOR_BILEVEL);
				   init_ccpu(1);
				   break;

	case DEMON:    sound_write=&demon_sound;
				   CCPUROMSIZE=16;
				   video_type_set(COLOR_BILEVEL);
				   init_ccpu(0);
				   break;

	case BOXINGB:  sound_write=&null_sound;
				   CCPUROMSIZE=32; 
				   video_type_set(COLOR_RGB);
				   init_ccpu(0);
				   break;

	case WOTW:     sound_write=&null_sound;
				   CCPUROMSIZE=16;
				   video_type_set(COLOR_RGB);
				   init_ccpu(0);
				   break;
	}

	   
		
    log_it("Z80DIP 1 %x",z80dip1);



	return 0;   
}

void end_cinemat()
{
//cineReset(); 	

//cache_clear();
}
