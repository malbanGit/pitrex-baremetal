/* STARWARS Hardware */
#include "starwars.h"
#include "globals.h"
#include "samples.h"
#include "vector.h"
#include "dips.h"
#include "input.h"

#include "sndhrdw/pokyintf.h"
#include "earom.h"


#include "driver.h"
#include "sndhrdw/starwars.h"
#include "vidhrdw/swavg.h"
#include "vidhrdw/atari_vg.h"
#include "machine/swmathbx.h"
#include "machine/starwars.h"
#include "machine/matari_vg.h"

#include "machine/starwars.c"
#include "sndhrdw/starwars.c"
#include "loaders.h"

extern struct GameDriver starwars_driver;
const struct GameDriver *drivers[] =
{	
    &starwars_driver,
	0	
};

static struct RunningMachine machine;
struct RunningMachine *Machine = &machine;
static const struct GameDriver *gamedrv;
static const struct MachineDriver *drv;
unsigned char *RAM;
unsigned char *ROM;

//////////////////////////////////////////////////

/* Star Wars READ memory map */
static struct MemoryReadAddress readmem[] =
{
       { 0x0000, 0x2fff, MRA_RAM, &vectorram },   /* vector_ram */
       { 0x3000, 0x3fff, MRA_ROM },  /* vector_rom */
/*     { 0x4800, 0x4fff, MRA_RAM },   cpu_ram   */
/*     { 0x5000, 0x5fff, MRA_RAM },    (math_ram_r) math_ram */
/*     { 0x0000, 0x3fff, MRA_RAM, &vectorram}, *//* Vector RAM and ROM */
        { 0x4800, 0x5fff, MRA_RAM },  /* CPU and Math RAM */
        { 0x6000, 0x7fff, banked_rom_r }, /* banked ROM */
        { 0x8000, 0xffff, MRA_ROM },  /* rest of main_rom */
        { 0x4300, 0x431f, input_bank_0_r }, /* Memory mapped input port 0 */
        { 0x4320, 0x433f, input_bank_1_r }, /* Memory mapped input port 1 */
        { 0x4340, 0x435f, opt_0_r }, /* DIP switches bank 0 */
        { 0x4360, 0x437f, opt_1_r }, /* DIP switches bank 1 */
        { 0x4380, 0x439f, adc_r },   /* ADC read */
        { 0x4400, 0x4400, main_read_r },
        { 0x4401, 0x4401, main_ready_flag_r },
        { 0x4500, 0x45ff, MRA_RAM }, /* nov_ram */
        { 0x4700, 0x4700, reh },
        { 0x4701, 0x4701, rel },
        { 0x4703, 0x4703, prng }, /* pseudo random number generator */
	{ -1 }	/* end of table */
};

/* Star Wars Sound READ memory map */
static struct MemoryReadAddress readmem2[] =
{
        { 0x0800, 0x0fff, sin_r }, /* SIN Read */

        { 0x1000, 0x107f, MRA_RAM },  /* 6532 RAM */
        { 0x1080, 0x109f, m6532_r },

        { 0x2000, 0x27ff, MRA_RAM }, /* program RAM */
        { 0x4000, 0x7fff, MRA_ROM }, /* sound roms */
        { 0xc000, 0xffff, MRA_ROM }, /* load last rom twice */
                                     /* for proper int vec operation */

        { -1 }  /* end of table */
};

/* Star Wars WRITE memory map */
static struct MemoryWriteAddress writemem[] =
{
	{ 0x0000, 0x2fff, MWA_RAM, &vectorram }, /* vector_ram */
	{ 0x3000, 0x3fff, MWA_ROM },  /* vector_rom */
/*  { 0x4800, 0x4fff, MWA_RAM },   cpu_ram */
/*  { 0x5000, 0x5fff, MWA_RAM },  (math_ram_w) math_ram */
	{ 0x4800, 0x5fff, MWA_RAM }, /* CPU and Math RAM */
	{ 0x6000, 0xffff, MWA_ROM }, /* main_rom */
	{ 0x4400, 0x4400, main_wr_w },
	{ 0x4500, 0x45ff, MWA_RAM }, /* nov_ram */
	{ 0x4600, 0x461f, atari_vg_go },  /* evggo(mine) or vg2_go */
	{ 0x4620, 0x463f, vg_reset }, /* evgres(mine) or vg_reset */
	{ 0x4640, 0x465f, MWA_NOP }, /*  (wdclr) Watchdog clear */
	{ 0x4660, 0x467f, irqclr },  /* clear periodic interrupt */
/*  { 0x4680, 0x4680, MWA_NOP },  (coin_ctr2) Coin counter 1 */
/*  { 0x4681, 0x4681, MWA_NOP },  (coin_ctr1) Coin counter 2 */
	{ 0x4680, 0x4681, MWA_NOP },  /*  Coin counters */
	{ 0x4682, 0x4682, led3 }, /* led3 */
	{ 0x4683, 0x4683, led2 }, /* led2 */
	{ 0x4684, 0x4684, mpage },  /* Page select for ROM0 */
	{ 0x4685, 0x4685, prngclr }, /* Reset PRNG */
	{ 0x4686, 0x4686, led1 },    /* led1 */
	{ 0x4687, 0x4687, recall },
	{ 0x46a0, 0x46bf, nstore },
	{ 0x46c0, 0x46c0, adcstart0 },
	{ 0x46c1, 0x46c1, adcstart1 },
	{ 0x46c2, 0x46c2, adcstart2 },
	{ 0x46e0, 0x46e0, soundrst },
    { 0x4700, 0x4707, swmathbx },
	{ -1 }	/* end of table */
};

/* Star Wars sound WRITE memory map */
static struct MemoryWriteAddress writemem2[] =
{
        { 0x0000, 0x07ff, sout_w },
        { 0x1000, 0x107f, MWA_RAM }, /* 6532 ram */
        { 0x1080, 0x109f, m6532_w },
		{ 0x1800, 0x183f, quad_pokey_w },//starwars_pokey_sound_w },
        { 0x2000, 0x27ff, MWA_RAM }, /* program RAM */
        { 0x4000, 0x7fff, MWA_ROM }, /* sound rom */
        { 0xc000, 0xffff, MWA_ROM }, /* sound rom again, for intvecs */


        { -1 }  /* end of table */
};


static struct MachineDriver machine_driver =
{
	/* basic machine hardware */
	{
                /* Main CPU */
		{
			CPU_M6809,
                        1512000,                        /* 1.5 Mhz CPU clock (Don't know what speed it should be) */
			0,  				/* Memory region #0 */
			readmem,writemem,0,0,
            
// Malban: this is interrupts AFTER X frames, not X interrupts PER frame!!!            
                        interrupt,6            /* Interrupt handler, and interrupts per frame (usually 1) */
                        /* Starwars should be 183Hz interrupts */
                        /* Increasing number of interrupts per frame speeds game up */
                },
                /* Sound CPU */
            {
                 CPU_M6809 | CPU_AUDIO_CPU,
                 1512000,                        /* 1.5 Mhz CPU clock (Don't know what speed it should be) */
                 2,                              /* Memory region #2 */
                 readmem2,writemem2,0,0,
                 starwars_snd_interrupt,24       /* Interrupt handler, and interrupts per frame (usually 1) */
                 /* Interrupts are to attempt to get */
                 /* resolution for the PIA Timer */
                 /* Approx. 2048 PIA clocks (@1.5 Mhz) */
              }

	},
        41, // MALBAN30,                     /* Target Frames per Second */
        0,  /* Name of initialisation handler */

	/* video hardware */
	512, 384, { 0, 240, 0, 280 },
	0,
        256,256, /* Number of colours, length of colour lookup table */
	sw_vg_init_colors,

	VIDEO_TYPE_VECTOR,
        0,  /* Handler to initialise video handware */
	atari_vg_avg_flip_start,	/* Start video hardware */
	atari_vg_stop,			/* Stop video hardware */
	atari_vg_screenrefresh,		/* Do a screen refresh */

	/* sound hardware */
	0,
    0,                              /* Initialise audio hardware */
    starwars_sh_start,              /* Start audio  */
    starwars_sh_stop,				/* Stop audio   */
    starwars_sh_update				/* Update audio */
};


/* in machine/swmathbx.c */
void translate_proms(void);

struct GameDriver starwars_driver =
{
	"Star Wars",
	"starwars",
        "STEVE BAINES\nBRAD OLIVER\nFRANK PALAZZOLO",
	   &machine_driver,

        0,
        translate_proms, 0,  /* ROM decryption, Opcode decryption */
        0,     /* Sample Array (optional) */

        0, 0, 0, 0, 0,

        0, /* Colour PROM */
        0,          /* palette */
        0,          /* colourtable */
	ORIENTATION_DEFAULT,

       0, 0 /* Highscore load, save */
};

//////////////////////////////////////////////////////////////////////////


void run_starwars()
{   
//    int x=0;
//    int f=0;

    cpu_run();

    if (drv->sh_update)
    {
      (*drv->sh_update)();
	    
    }
   // log_it("------------FRAME START --------------"); 
}

/////////////////// MAIN() for program ///////////////////////////////////////////////////
int init_starwars(void)
{
  
   BUFFER_SIZE=256; chip=4;gain=8;
   
    
    Machine->gamedrv = gamedrv = drivers[0];
    Machine->drv = drv = gamedrv->drv;
	
    RAM = GI[CPU0];//Machine->memory_region[drv->cpu[0].memory_region];
    ROM = RAM;

    /* decrypt the ROMs if necessary */
    if (gamedrv->rom_decode) (*gamedrv->rom_decode)();
    cpu_init();
    load_hi_aae(0x4500, 0x100, 0);
    cpu_start();
    atari_vg_avg_flip_start();
    starwars_sh_start();
    //translate_proms();
//    cache_clear();
    return 0;
}

void end_starwars()
{
 save_hi_aae(0x4500, 0x100, 0);
}

// 