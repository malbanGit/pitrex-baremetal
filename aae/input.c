#include <math.h>
#include <string.h>

#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif


#include "input.h"
#include "globals.h"
//#include "gui.h"
#include "samples.h"
#include "cpu_control.h"
#include "vector.h"
//#include "gamekeys.h"

#define MAX_KEY 70
#define X_AXIS  0
#define Y_AXIS  1
#define MAX_INPUT_PORT 23
#define OSD_MAX_JOY_ANALOG 4

MY_Input the_input[256];		

//int mickeyx;   //MouseX var
//int mickeyy;   //MouseY var
int num_joysticks = 2; // this is ALEGRO - MALBAN!
void poll_joystick() // also alegro
{}


static char my_j[256];
//static int curr_timer=5;
extern int s_animate;
extern int errorsound;
extern int menulevel;

//NEW INPUT CODE

static unsigned int PORT[MAX_INPUT_PORT];//={0,0,0,0,0,0,0,0,0};
static int input_analog[MAX_INPUT_PORT];
static int input_analog_current_value[MAX_INPUT_PORT];
static int input_analog_previous_value[MAX_INPUT_PORT];
static int input_analog_init[MAX_INPUT_PORT];
static int input_analog_scale[MAX_INPUT_PORT];
static int input_port_value[MAX_INPUT_PORT];
static int analog_current_x[OSD_MAX_JOY_ANALOG], analog_current_y[OSD_MAX_JOY_ANALOG];
static int analog_previous_x[OSD_MAX_JOY_ANALOG], analog_previous_y[OSD_MAX_JOY_ANALOG];
//static int *in;
static int mouse_delta_X,mouse_delta_Y;

static int toggle_keys[2][256];
static int debounce[2][256];
static int impulsecount[2][256];
static int waspressed[2][256];	

/*
static void do_poll_joystick(void)
{
  printf("poll_joystick()\n\r");
	if (num_joysticks > 0)
		poll_joystick();
}
*/


int do_key_pressed(int keycode)
{
  printf("do_key_pressed()\n\r");
   return 0;
}

/* return a value in the range -128 .. 128 (yes, 128, not 127) */
static void do_analogjoy_read(int *analog_x, int *analog_y)
{
    *analog_x = *analog_y = 0;
    *analog_x = INPUT_ANALOG_P1_SIGNED_8BIT_HORIZONTAL;
    *analog_y = INPUT_ANALOG_P1_SIGNED_8BIT_VERTICAL;
}

void set_input_defaults()
{

  int i=0;

  for (i=0; i < MAX_INPUT_PORT; i++)
  {
    input_analog[i]=0;
    input_analog_current_value[i]=0;
    input_analog_previous_value[i]=0;
    input_analog_scale[i]=0;
    input_port_value[i]=0;
  
 	  /* reset the analog port on first access */
    if((GK[i].Type & 0xff) == IN_ANALOG && GK[i].Arg2 )
    {
        input_analog_init[i] = 0;
        input_analog_current_value[i] = input_analog_previous_value[i] = GK[i].Arg2;// * 100 / GK[i].Mask; // default /sensitivity
    }
  }

  for (i=0; i < 256; i++)
  {
   impulsecount[0][i]=0;
   waspressed[0][i]  =0;
   toggle_keys[0][i] =0;
   debounce[0][i]    =0;

   impulsecount[1][i]=0;
   waspressed[1][i]  =0;
   toggle_keys[1][i] =0;
   debounce[1][i]    =0;
  }
}

/* wait for a key press and return the keycode */
int do_read_key(void)
{
  printf("do_read_key()\n\r");
  return 0;
}


void check_joystick(void)
{ 

//  for (int i=0;i<256;i++)
//    my_j[i]=0;

  int x=0;
  int y=0;
//  ZeroMemory(my_j,256); //VALUE MUST BE CHAR for this to work right. 
  memset(my_j,0,256);

  x=0;
  y=0;
  if (INPUT_P1_DIGITAL_LEFT) {my_j[(x*44)+(4*y)+1]=1;}//printf("hit on my joy %d",((x*44)+(4*y)));}
  if (INPUT_P1_DIGITAL_RIGHT) {my_j[(x*44)+((4*y)+2)]=1;}//printf("hit on my joy %d",((x*44)+(4*y)));}     
  if (INPUT_P1_DIGITAL_UP) {my_j[(x*44)+((4*y)+3)]=1;}//printf("hit on my joy %d",((x*44)+(4*y)));}     
  if (INPUT_P1_DIGITAL_DOWN) {my_j[(x*44)+((4*y)+4)]=1;}//printf("hit on my joy %d",((x*44)+(4*y)));}    
  if (INPUT_P1_BUTTON1)  {my_j[(x*44)+33]=1;} //have to add 1 // J0B0
  if (INPUT_P1_BUTTON2)  {my_j[(x*44)+34]=1;} // J0B1
  if (INPUT_P1_BUTTON3)  {my_j[(x*44)+35]=1;} // J0B2
  if (INPUT_P1_BUTTON4)  {my_j[(x*44)+36]=1;} // J0B3
  if (0)  {my_j[(x*44)+37]=1;} // J0B4
  if (0)  {my_j[(x*44)+38]=1;} // J0B5
  if (0)  {my_j[(x*44)+39]=1;} // J0B6
  if (INPUT_START1)  {my_j[(x*44)+40]=1;} // J0B7
  if (INPUT_COIN1)  {my_j[(x*44)+41]=1;} // J0B8
  
  x++;
  if (INPUT_P2_DIGITAL_LEFT) {my_j[(x*44)+(4*y)+1]=1;}//printf("hit on my joy %d",((x*44)+(4*y)));}
  if (INPUT_P2_DIGITAL_RIGHT) {my_j[(x*44)+((4*y)+2)]=1;}//printf("hit on my joy %d",((x*44)+(4*y)));}     
  if (INPUT_P2_DIGITAL_UP) {my_j[(x*44)+((4*y)+3)]=1;}//printf("hit on my joy %d",((x*44)+(4*y)));}     
  if (INPUT_P2_DIGITAL_DOWN) {my_j[(x*44)+((4*y)+4)]=1;}//printf("hit on my joy %d",((x*44)+(4*y)));}    
  if (INPUT_P2_BUTTON1)  {my_j[(x*44)+33]=1;} //have to add 1
  if (INPUT_P2_BUTTON2)  {my_j[(x*44)+34]=1;}
  if (INPUT_P2_BUTTON3)  {my_j[(x*44)+35]=1;}
  if (INPUT_P2_BUTTON4)  {my_j[(x*44)+36]=1;}
  if (0)  {my_j[(x*44)+37]=1;} // J1B4
  if (0)  {my_j[(x*44)+38]=1;} // J1B5
  if (0)  {my_j[(x*44)+39]=1;} // J1B6
  if (INPUT_START2)  {my_j[(x*44)+40]=1;} // J1B7
  if (INPUT_COIN2)  {my_j[(x*44)+41]=1;} // J1B8
  
  /*
  for (x=0; x < num_joysticks; x++)
  {
    
    for (y=0; y < 4; y++)
    {
        if (joy[x].stick[y].axis[0].d1) {my_j[(x*44)+(4*y)+1]=1;}//printf("hit on my joy %d",((x*44)+(4*y)));}
        if (joy[x].stick[y].axis[0].d2) {my_j[(x*44)+((4*y)+2)]=1;}//printf("hit on my joy %d",((x*44)+(4*y)));}	 
        if (joy[x].stick[y].axis[1].d1) {my_j[(x*44)+((4*y)+3)]=1;}//printf("hit on my joy %d",((x*44)+(4*y)));}	 
        if (joy[x].stick[y].axis[1].d2) {my_j[(x*44)+((4*y)+4)]=1;}//printf("hit on my joy %d",((x*44)+(4*y)));}	
    }
    if (joy[x].button[0].b)  {my_j[(x*44)+33]=1;} //have to add 1
    if (joy[x].button[1].b)  {my_j[(x*44)+34]=1;}
    if (joy[x].button[2].b)  {my_j[(x*44)+35]=1;}
    if (joy[x].button[3].b)  {my_j[(x*44)+36]=1;}
    if (joy[x].button[4].b)  {my_j[(x*44)+37]=1;}
    if (joy[x].button[5].b)  {my_j[(x*44)+38]=1;}
    if (joy[x].button[6].b)  {my_j[(x*44)+39]=1;}
    if (joy[x].button[7].b)  {my_j[(x*44)+40]=1;}
    if (joy[x].button[8].b)  {my_j[(x*44)+41]=1;}
    if (joy[x].button[9].b)  {my_j[(x*44)+42]=1;}
    if (joy[x].button[10].b) {my_j[(x*44)+43]=1;}
    if (joy[x].button[11].b) {my_j[(x*44)+44]=1;}
    if (joy[x].button[12].b) {my_j[(x*44)+45]=1;}
  }
*/
  
}


static int is_pressed(int i)
{
  if (/*key[GK[i].Current] || */ my_j[GK[i].Joystick] || mouseb[GK[i].mouseb] )//Insert mouse button here
      return 1;
  else 
      return 0;
}


static void set_port(int i)
{
	int x = GK[i].Port;

	if ((GK[i].Type & ~INF_MASK) == IN_ACTIVE_LOW )    
    {
      PORT[x] &= ~GK[i].Mask; 
    }
	else  
      if ((GK[i].Type & ~INF_MASK) == IN_ACTIVE_HIGH ) 
      {
        PORT[x] |= GK[i].Mask;
      }
}

void Check_Input(int in, int state)
{  
/*
  //log_it("Checking input %d",inkey);  set_port(int i)
  int p = GK[in].Port;
  int val = p+in;
  //Is the key already pressed? Check to see if it's time for  keyup
  if (state)  
  {
      the_input[val].now=1; //Enable the key
      
      if (!the_input[val].s2) //Is the latch set? if not, start the countdown.
      {
          //Set latch and start process
          the_input[val].s2 = 5; //Ramp up Time       //5
          the_input[val].s3 = 5; //Final Speed Time   //5
          the_input[val].timer = (3 * the_input[val].s2);
          set_port(in); //Key is considered pressed at this point.
      }
  
      //Check for repeat input (PHASE 2)?
      if (the_input[val].s2) //Is this a repeater?
        {
          the_input[val].timer--;
          if ( the_input[val].timer <  1)
          {
          //Reset for next round;
              the_input[val].s2 --;
              //Cascading counters       
              if ( the_input[val].s2 < 1 )  
              {
                the_input[val].s3-=1;
                if (the_input[val].s3 < 1) 
                {
                  the_input[val].s3=1;
                }
                the_input[val].s2 = the_input[val].s3; //Shorten time each pass
              } 
              //Reset timer
              the_input[val].timer = (3 * the_input[val].s3);

            set_port(in);  //Key is returned press this time.
          }
      }
  } 
  else 
  { 
    the_input[val].now=0;
    the_input[val].s2=0;
  } //Key is not pressed, make sure variables are clear.
 */
}

void proc_input(int num)
{
  int i;

  if (num==0) //Only reset on the first pass to avoid clearing default keys.
  {
	  for (i=0; i < MAX_INPUT_PORT; i++)
      {
        PORT[i]=0;
      }
  }
  i=0; //Reset i so I can use it again.

  //printf("START INPUT PROCESSING-----");
 
  while (GK[i].Port > -1 )
  {
    //printf("GK PORT %d",GK[i].Port);
    //printf("GK ID %x",GK[i].Type & 0xff);
    //printf("here! %s ",GK[i].Heading );
    
    //First set default for port.
    if ((GK[i].Type & ~INF_MASK) == IN_DEFAULT) //Set any defaults for that port.
    { 
      PORT[GK[i].Port]= GK[i].Mask; 
    }
      
    if (toggle_keys[num][i]) 
      set_port(i);
    
    if (is_pressed(i))
    {           
      if (GK[i].Type & INF_COUNTER)  //Should CPU 0 be reset for this?
      { 
          Check_Input(i, 1);
      }
      //THEN CHECK if its a reset button 
      else if (GK[i].Type & INF_RESETCPU)  //Should CPU 0 be reset for this?
      { 
        if ( debounce[i]==0)
        {
          cache_clear();
          cpu_needs_reset(0);
          debounce[num][i]=1;
          //printf("Reset on CPU 0 Called by F3");
          set_port(i);
        }
      }

      else if (GK[i].Type & INF_IMPULSE ) //IS it an impulse key?
      {
        if (waspressed[num][i] == 0) //Reset if not inputted
        { 
          impulsecount[num][i] = GK[i].Arg1;
        }
              
      }
      else if  (GK[i].Type & INF_TOGGLE ) //IS this a toggle input?
      { 
        //Is the Key Pressed? //Toggle it.
        if ( debounce[num][i]==0)
        {
          toggle_keys[num][i] ^=1;  //Toggle it.
          debounce[num][i]=1;
          //printf("TOGGLE!!!!!!!");
          set_port(i);
        }
      }
      else 
      {
//printf("is pressed and set to port: %i\n\r",i );      
        set_port(i);
      }
      waspressed[num][i]=1;
    }
    else
    {
      if (GK[i].Type & INF_COUNTER)  //Should CPU 0 be reset for this?
      { 
          Check_Input(i, 0);
      }
      waspressed[num][i] = 0;
      debounce[num][i]=0;
      
    }

    if ((GK[i].Type & INF_IMPULSE)  && impulsecount[num][i] > 0) //Is it an impulse key and it was pressed?
    {
      impulsecount[num][i]--;
      waspressed[num][i]=1;
      set_port(i);
    }
    i++;
  }
//printf("INPUT END-----");
}



UINT8 in_port_0_r(UINT32 address, struct MemoryReadByte *psMemRead)  { return getport(0); }
UINT8 in_port_1_r(UINT32 address, struct MemoryReadByte *psMemRead)  { return getport(1); }
UINT8 in_port_2_r(UINT32 address, struct MemoryReadByte *psMemRead)  { return getport(2); }
UINT8 in_port_3_r(UINT32 address, struct MemoryReadByte *psMemRead)  { return getport(3); }
UINT8 in_port_4_r(UINT32 address, struct MemoryReadByte *psMemRead)  { return getport(4); }
UINT8 in_port_5_r(UINT32 address, struct MemoryReadByte *psMemRead)  { return getport(5); }
UINT8 in_port_6_r(UINT32 address, struct MemoryReadByte *psMemRead)  { return getport(6); }
UINT8 in_port_7_r(UINT32 address, struct MemoryReadByte *psMemRead)  { return getport(7); }
UINT8 in_port_8_r(UINT32 address, struct MemoryReadByte *psMemRead)  { return getport(8); }
UINT8 in_port_9_r(UINT32 address, struct MemoryReadByte *psMemRead)  { return getport(9); }
UINT8 in_port_10_r(UINT32 address, struct MemoryReadByte *psMemRead) { return getport(10); }
UINT8 in_port_11_r(UINT32 address, struct MemoryReadByte *psMemRead) { return getport(11); }
UINT8 in_port_12_r(UINT32 address, struct MemoryReadByte *psMemRead) { return getport(12); }
UINT8 in_port_13_r(UINT32 address, struct MemoryReadByte *psMemRead) { return getport(13); }
UINT8 in_port_14_r(UINT32 address, struct MemoryReadByte *psMemRead) { return getport(14); }




int KeyCheck(int keynum)
{
/*
    int i;
	static int hasrun=0;
	static int keys[256];

    if (hasrun==0) //INIT
   {
      for ( i=0; i<256; i++)
	  { keys[i]=0;}
         hasrun=1;
   }
   if (!keys[keynum]&&key[keynum]) //Return True if not in que
   {
      keys[keynum]=1;
      return 1;
   }
   else if (keys[keynum]&&!key[keynum]) //Return False if in que
      keys[keynum]=0;
   */
   return 0;
	
}


static void update_analog_port(int port) //actually input number not port really.
{
	
	int current, delta, type, sensitivity, min, max, default_value;
	int axis, is_stick, check_bounds;
		
	int keydelta;
	int player=0;
	
	int p=GK[port].Port;
	
   // printf("UPDATING analogport Number %s %d ",GK[port].Heading,port);
	//Get the type of port
	type=GK[port].Default;
      
    switch (type)
	{
		//case IPT_PADDLE:
		//	axis = X_AXIS; is_stick = 1; check_bounds = 1; break;
		//case IPT_PADDLE_V:
		//	axis = Y_AXIS; is_stick = 1; check_bounds = 1; break;
		//case IPT_DIAL:
		//	axis = X_AXIS; is_stick = 0; check_bounds = 0; break;
		//case IPT_DIAL_V:
		//	axis = Y_AXIS; is_stick = 0; check_bounds = 0; break;
		case MOUSE_X:
			axis = X_AXIS; is_stick = 0; check_bounds = 0; break;
		case MOUSE_Y:
			axis = Y_AXIS; is_stick = 0; check_bounds = 0; break;
		case IN_AD_STICK_X:
			axis = X_AXIS; is_stick = 1; check_bounds = 1; break;
		case IN_AD_STICK_Y:
			axis = Y_AXIS; is_stick = 1; check_bounds = 1; break;
		//case IPT_PEDAL:
		//	axis = Y_AXIS; is_stick = 0; check_bounds = 1; break;
		default:
			/* Use some defaults to prevent crash */
			axis = X_AXIS; is_stick = 0; check_bounds = 0;
			printf("Oops, polling non analog device in update_analog_port()????: TYPE %d",type);
	}


    sensitivity = GK[port].Mask;
	min = GK[port].Current;
	max = GK[port].Joystick;
	
	//Set Joustick Default Value
	default_value = GK[port].Arg2;
	//Save the previous value
	input_analog_previous_value[p] = input_analog_current_value[p];
	//Set the current value
	
	if (( GK[port].Type & INF_CENTER) && (!is_stick))
		input_analog_current_value[port] = default_value * 100 / sensitivity;//input_analog_current_value[port] = 0;

	if ( GK[port].Type & INF_PORTRESET) 
      current = 0;
	else 
      current = input_analog_current_value[p];

    //Delta starts at zero
	delta = 0;
    
	//Get the Keyboard button delta
	keydelta= GK[port].Arg1;
    
    // extremes can be either signed or unsigned
	if (min > max)
	{
      if (GK[port].Amask > 0xff) min = min - 0x10000; 
      else 
        min = min - 0x100;
	}

	if (axis == X_AXIS)	
    {
      delta = mouse_delta_X; 
      
    }//[player];
	else 
    {
      delta = mouse_delta_Y; 
      
    }//[player];
    
    //If the user is using Keys, Override the delta with the key delta
	if (PORT[GK[port-1].Port]) 
      delta -=keydelta;
	if (PORT[GK[port+1].Port]) 
      delta +=keydelta;
	
	//If its a reverse port, reverse it. This needs to be addressed with config changed later
    if (GK[port].Type  & INF_REVERSE) delta = -delta;
	
	//Is it an analog joystick? Then do the routine below.
 	if (is_stick)
	{
		int anew, prev;

		//printf("I think this is an analog stick?");
		/* center stick */
		if ((delta == 0) && (GK[port].Type & INF_CENTER))
		{                   
	     // printf("CENTERED STICK!!!");
          if (current > default_value)
            delta = -100 / sensitivity;
          if (current < default_value)
            delta = 100 / sensitivity;
		}

		/* An analog joystick which is not at zero position (or has just */
		/* moved there) takes precedence over all other computations */
		/* analog_x/y holds values from -128 to 128 (yes, 128, not 127) */

		if (axis == X_AXIS)
		{
			anew  = analog_current_x[player];
			prev = analog_previous_x[player];
		}
		else
		{
			anew  = analog_current_y[player];
			prev = analog_previous_y[player];
		}

		if ((anew != 0) || (anew-prev != 0))
		{
			delta=0;

			if (GK[port].Type & INF_REVERSE)
			{
				anew  = -anew;
				prev = -prev;
			}

			/* apply sensitivity using a logarithmic scale */
			if (GK[port].Mask > 0xff)
			{
				if (anew > 0)
				{
					current = (pow(anew / 32768.0, 100.0 / sensitivity) * (max-GK[port].Arg2)
							+ GK[port].Arg2) * 100 / sensitivity;
				}
				else
				{
					current = (pow(-anew / 32768.0, 100.0 / sensitivity) * (min-GK[port].Arg2)
							+ GK[port].Arg2) * 100 / sensitivity;
				}
			}
			else
			{
				if (anew > 0)
				{
					current = (pow(anew / 128.0, 100.0 / sensitivity) * (max-GK[port].Arg2)
							+ GK[port].Arg2) * 100 / sensitivity;
				}
				else
				{
					current = (pow(-anew / 128.0, 100.0 / sensitivity) * (min-GK[port].Arg2)
							+ GK[port].Arg2) * 100 / sensitivity;
				}
			}
		}
	}

	current += delta;
  //  printf("SCALED ANALOG DELTA is %d",current);
	if (check_bounds)
	{
//		printf("I am checking bounds for some reason");
		if ((current * sensitivity + 50) / 100 < min)
			current = (min * 100 + sensitivity/2) / sensitivity;
		if ((current * sensitivity + 50) / 100 > max)
			current = (max * 100 + sensitivity/2) / sensitivity;
	}
   // printf("delta this port %d",delta);
	input_analog_current_value[p] = current;
	//printf("Current value NOW for this port  %d",input_analog_current_value[p]);
}


static void scale_analog_port(int port)
{
	int delta,current,sensitivity;
	int p=GK[port].Port;
    int type=driver[gamenum].cpu_type[0];

	sensitivity = GK[port].Mask;
 	// printf("SCALING analogport Number %d with sensitivity %d",p, sensitivity);
	delta = input_analog_current_value[p] - input_analog_previous_value[p];
	//printf("delta pre scaling is %d",delta);

	if (driver[gamenum].cpu_freq[0]==2500000)
	{
	  /* MAJOR HAVOCS
		if (delta >= 0)
		delta = MHscale_by_cycles(delta, 2500000);
	    else
		delta = -MHscale_by_cycles(-delta, 2500000);
	    */
	 //printf("USING MH SCALE TO CYCLES!!!! DELTA = %d",delta);	
	}
	else 
	{
		if (delta >= 0)
			delta = cpu_scale_by_cycles(delta);  
		else
			delta = -cpu_scale_by_cycles(-delta);
	}

	//printf("delta POST scaling is %d",delta);
	current = input_analog_previous_value[p] + delta;

		if (current >= 0)
			current = (current * sensitivity + 50) / 100;
		else
			current = (-current * sensitivity + 50) / -100;

	input_port_value[p] &= ~GK[port].Amask; 
	input_port_value[p] |= current & GK[port].Amask; 
    PORT[p]= input_port_value[p];
}


int getport(int port)
{
  int i;

  for (i=0; i < MAX_INPUT_PORT; i++)
  {
    if (GK[i].Port==port  &&  (GK[i].Type & ~INF_MASK) == IN_ANALOG )
       scale_analog_port(i);
  }
  
//	printf("Game Get Port Called, Port %d Data %d",port,PORT[port]);
	return PORT[port];

}


void update_input()
{
	int i;
	int c=0;
    
    check_joystick(); //Update digital joystick settings.
	
    proc_input(0);
    proc_input(1);
    c=0;
	while (GK[c].Port > -1 )
    {
	 //printf("ROUND 4 -----Key Number %d : Setting Key %x Setting Joy %x",c,GK[c].Current,GK[c].Joystick);
	 c++;
    }

	/* update the analog devices */
	for (i = 0;i < OSD_MAX_JOY_ANALOG;i++)
	{
		/* update the analog joystick position */
		analog_previous_x[i] = analog_current_x[i];
		analog_previous_y[i] = analog_current_y[i];
		do_analogjoy_read ( &(analog_current_x[i]), &(analog_current_y[i]));
	}
    
	/* update mouse/trackball position */
//	aae_mouse ( &mouse_delta_X, &mouse_delta_Y);
	
	i=0;
	while (GK[i].Port > -1 )
	{
		//if (input_analog[i])
		if((GK[i].Type & 0xff) == IN_ANALOG)
		{
			update_analog_port(i);
			/*
				           printf("PORT NUMBER %d",GK[i].Port);
						   printf("PORT NUMBER %d",GK[i].Mask);
			               printf("PORT HEADING %s",GK[i].Heading);
						   printf("PORT NUMBER %d",GK[i].Default);
						   printf("PORT NUMBER %d",GK[i].Current);
						   printf("PORT NUMBER %d",GK[i].Joystick);
						   printf("PORT NUMBER %d",GK[i].Type);
						   printf("PORT NUMBER %d",GK[i].Arg1);
						   printf("PORT NUMBER %d",GK[i].Arg2);
			
			         */
		}
		i++;
	}
}


////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////
////////////////////////////////////////////////////////////////////
//////////////////////

void clear_input()
{
  
}

/*
typedef struct {

int s1;
int s2;
int timer;

} MY_Input; 
MY_Input the_input[256];			
*/


//////////////////////////////////////////////
// KEY INITILIZATION BELOW HERE //////////////
//////////////////////////////////////////////

void set_default_keys()

{
  int i=0;
  
  while (GK[i].Port > -1 && (i < MAX_KEY))
    {
		if (GK[i].Type != IN_DEFAULT)
		{
			if((GK[i].Type & ~INF_MASK) != IN_ANALOG)
			 {   
			  GK[i].Current= GK[i].Default & ~INF_MASK;
			  GK[i].Joystick = (GK[i].Default >> 8) & 0xff;
			  GK[i].mouseb = GK[i].Default >> 24;
			  //walk_keyset(i);
			  //printf("Setting Joy %x",GK[i].Joystick);
			   printf("SETTING DEFAULT KEY Number %d : Setting Key %x Setting Joy %x\r\n",i,GK[i].Current,GK[i].Joystick);
			 }
		}
	     i++;
	}

}


void force_keys()
{
   int i=0;
   // log_it("OK STARTING TO FORCE SET KEYS");
   while (GK[i].Port > -1 && (i < MAX_KEY))
   {
     if ((GK[i].Type & ~INF_MASK) !=IN_DEFAULT )
     {  if((GK[i].Type & ~INF_MASK) != IN_ANALOG)
         {   
          GK[i].Current= GK[i].Default & ~INF_MASK;
          GK[i].Joystick = (GK[i].Default >> 8) & 0xff;
          GK[i].mouseb = GK[i].Default >> 24;
          //walk_keyset(i);
          printf("Key Number %d : Setting Current %x Setting Joy %x\r\n",i,GK[i].Current,GK[i].Joystick);
         }
     }
     
     i++;
    }
}

void init_keys(int num)
{
   GK = &driver[gamenum].game_keys[0];
   force_keys();
//   retrieve_keys();	

}
