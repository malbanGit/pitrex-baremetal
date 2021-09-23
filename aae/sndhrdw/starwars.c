     /***************************************************************************
sndhrdw\starwars.c

STARWARS MACHINE FILE

This file created by Frank Palazzolo. (palazzol@tir.com)

Release 2.0 (6 August 1997)

See drivers\starwars.c for notes

***************************************************************************/

#include "../driver.h"
#include "../sndhrdw/pokyintf.h"
#include "starwars.h"
#include "5220intf.h"

#include <vectrex/vectrexInterface.h>

#define SNDDEBUG 0
#define FIFODEBUG 0

/* Sound commands from the main CPU are stored in a single byte */
/* register.  However, because the main CPU cannot interrupt    */
/* the sound CPU in MAME, the sound CPU cannot respond fast     */
/* enough to new commands.  I have implemented a FIFO to        */
/* replace the byte register as a workaround.                   */

#define MAX_COMMANDS 8          /* will probably never need to be this big */
int mainwrite[MAX_COMMANDS];    /* Main CPU write register */
int mainread = 0;               /* Main CPU read register */
int fifo_head = 0;
int fifo_tail = 0;
int items_in_fifo = 0;

int port_A = 0;          /* 6532 port A data register */

                         /* Configured as follows:           */
                         /* d7 (in)  Main Ready Flag         */
                         /* d6 (in)  Sound Ready Flag        */
                         /* d5 (out) Mute Speech             */
                         /* d4 (in)  Not Sound Self Test     */
                         /* d3 (out) Hold Main CPU in Reset? */
                         /*          + enable delay circuit? */
                         /* d2 (in)  TMS5220 Not Ready       */
                         /* d1 (out) TMS5220 Not Read        */
                         /* d0 (out) TMS5220 Not Write       */

int port_B = 0;          /* 6532 port B data register        */
                         /* (interfaces to TMS5220 data bus) */

unsigned char timer = 0; /* # of snd_interrupts until timeout */
int timer_running = 0;   /* is the timer running ? */
int interrupt_flags = 0; /* 6532 interrupt flag register */

int port_A_ddr = 0;      /* 6532 Data Direction Register A */
int port_B_ddr = 0;      /* 6532 Data Direction Register B */
                         /* for each bit, 0 = input, 1 = output */



static struct TMS5220interface tms5220_interface =
{
    640000,     /* clock speed (80*samplerate) */
    255,        /* volume */
    0           /* IRQ handler */
};
static struct POKEYinterface pokey_interface =
{
	4,			/* 4 chips */
	1512000,	/* 1.5 MHz? */
    10,    /* volume */
	6,//POKEY_DEFAULT_GAIN/4,
	USE_CLIP,
	/* The 8 pot handlers */
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	/* The allpot handler */
	{ 0, 0, 0, 0 },
};

int starwars_sh_start(void)
{
	int rv;

    rv = pokey_sh_start (&pokey_interface);
    if (rv) return rv;
    return(tms5220_sh_start (&tms5220_interface));
}


void starwars_sh_stop(void)
{
    pokey_sh_stop ();
    tms5220_sh_stop ();
}

extern uint32_t soundStart;
extern uint32_t soundEnd;

void starwars_sh_update()
{
  CCNT0(soundStart);
    pokey_sh_update ();
    tms5220_sh_update();
  CCNT0(soundEnd);
}

/********************************************************/

int starwars_snd_interrupt(void)
{
	/* if the timer is running, decrement it, and check if it */
	/* has expired.  If so - IRQ, otherwise ignore            */
	if (timer_running)
	{
		if (--timer == 0)
		{
			timer_running = 0;
			interrupt_flags |= 0x80; /* set timer interrupt flag */
			return interrupt();
		}
	}

	return ignore_interrupt();
}

/********************************************************/

int m6532_r(int offset)
{
    static int temp;

    switch (offset)
    {
        case 0: /* 0x80 - Read Port A */

            /* if the fifo is empty, the main ready flag (bit 7) looks */
            /* reset to the sound board.  If not, flag appears set     */
            
            /* Note: bit 4 is always set to avoid sound self test */

            if (items_in_fifo == 0)
                return (port_A&0x7f)|0x10|(!tms5220_ready_r()<<2);
            else
                return (port_A&0x7f)|0x90|(!tms5220_ready_r()<<2);

        case 1: /* 0x81 - Read Port A DDR */
            return port_A_ddr;

        case 2: /* 0x82 - Read Port B */
            return port_B;  /* speech data read? */

        case 3: /* 0x83 - Read Port B DDR */
            return port_B_ddr;

        case 5: /* 0x85 - Read Interrupt Flag Register */
            temp = interrupt_flags;
            interrupt_flags = 0;   /* Clear int flags */
            return temp;

        default:
            return 0;
    }

    return 0; /* will never execute this */
}
/********************************************************/

void m6532_w(int offset, int data)
{
    switch (offset)
    {
        case 0: /* 0x80 - Port A Write */
            
			if((port_A&0x01)==1)
            {
            	port_A = (port_A&(~port_A_ddr))|(data&port_A_ddr);
                if ((port_A&0x01)==0)
                tms5220_data_w(0,port_B);
            }
            else
            	port_A = (port_A&(~port_A_ddr))|(data&port_A_ddr);

            return;

        case 1: /* 0x81 - Port A DDR Write */
            port_A_ddr = data;
            return;

        case 2: /* 0x82 - Port B Write */
            /* TMS5220 Speech Data on port B */

            /* ignore DDR for now */
            port_B = data;

            return;

        case 3: /* 0x83 - Port B DDR Write */
            port_B_ddr = data;
            return;

        case 7: /* 0x87 - Enable Interrupt on PA7 Transitions */

            /* This feature is not emulated.  When the Main CPU  */
            /* writes to mainwrite, it should send an IRQ to the */
            /* sound CPU.  Because this is not supported in MAME */
            /* yet, a small FIFO is used for mainwrite in place  */
            /* of the single byte buffer in the actual game.     */
            /* The net effect is that the PA7 main_ready_flag is */
            /* no longer a single bit.  Each CPU reads it's own  */
            /* version of the flag based on the state of the     */
            /* FIFO:                                             */
            /*                                                   */
            /* FIFO State:   Main CPU   Sound CPU                */
            /*               Reads:      Reads:                  */
            /* FULL            1           1                     */
            /* PART FULL       0           1                     */
            /* EMPTY           0           0                     */

            return;


        case 0x1f: /* 0x9f - Set Timer to decrement every n*1024 clocks, */
                   /*        With IRQ enabled on countdown               */

            /* Should be decrementing every data*1024 6532 clock cycles */
            /* 6532 runs at 1.5 Mhz */

            /* instead we are decrementing at half that rate, so we'll */
            /* cut the time in half */

            timer = data>>1;

            timer_running = 1;
            return;

       default:  return;
     }

    return; /* will never execute this */

}

/********************************************************/
/* These routines are called by the Sound CPU to        */
/* communicate with the Main CPU                        */
/********************************************************/

int sin_r(int offset)
{
    static int temp;

    if (items_in_fifo == 0)
    {
    #if(FIFODEBUG==1)
        printf("FIFO underrun!\n");
    #endif
        return mainwrite[fifo_head];
    }

    items_in_fifo--;
    temp = mainwrite[fifo_head];
    fifo_head = (fifo_head+1)%MAX_COMMANDS;

    #if(FIFODEBUG==1)
    printf("Items in FIFO - %d\n",items_in_fifo);
    #endif

    return temp;
}

void sout_w(int offset, int data)
{
    mainread = data;
    port_A |= 0x40;  /* set sound ready flag */
    return;
}

/********************************************************/
/* The following routines are called from the Main CPU, */
/* not the Sound CPU.                                   */
/* They are here because they are all related to sound  */
/* CPU communications                                   */
/********************************************************/

int main_read_r(int offset)
   {

   #if(SNDDEBUG==1)
   printf("main_read_r\n");
   #endif

   port_A &= 0xbf;  /* reset flag for sound ready */
   return mainread;
   }

/********************************************************/

int main_ready_flag_r(int offset)
   {
   #if(SNDDEBUG==1)
   printf("main_ready_flag_r\n");
   #endif

   /* The only time the main CPU thinks it can't write to */
   /* the sound one is if the FIFO is full.               */

   if (items_in_fifo == MAX_COMMANDS)
       return (port_A & 0x40) | 0x80; /* only upper two flag bits mapped */
   else
       return (port_A & 0x40);
   }

/********************************************************/

void main_wr_w(int offset, int data)
   {
   #if(SNDDEBUG==1)
   printf("main_wr_w\n");
   #endif

   if (items_in_fifo == MAX_COMMANDS)
   {
   #if(FIFODEBUG==1)
       printf("FIFO overrun!\n");
   #endif
       items_in_fifo = 1;
       mainwrite[fifo_head] = data;
       fifo_tail = (fifo_head+1)%MAX_COMMANDS;
       return;
   }

   mainwrite[fifo_tail] = data;
   fifo_tail = (fifo_tail+1)%MAX_COMMANDS;
   items_in_fifo++;

   #if(FIFODEBUG==1)
   printf("Items in FIFO - %d\n",items_in_fifo);
   #endif
   }

/********************************************************/

void soundrst(int offset, int data)
   {

   #if(SNDDEBUG==1)
   printf("soundrst\n");
   #endif

   mainread = 0;
   fifo_head = fifo_tail = 0;
   port_A &= 0x3f;
   /* should reset sound CPU here  */
   }



