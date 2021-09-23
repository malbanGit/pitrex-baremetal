#ifndef GAMEROMS_H
#define GAMEROMS_H

#include "globals.h"

/*
const struct roms 
{
	char *filename;
	UINT loadAddr;
	UINT16 romSize;
	int loadtype;
	
};
*/
#define ROM_LOAD_NORMAL 0
#define ROM_LOAD_16BYTE    1
#define ROM_START(name)  static const struct roms name[] = {
#define ROM_REGION( romSize, loadtype) { NULL, 0x999, romSize, loadtype }, 
#define ROM_LOAD(filename,loadAddr,romSize) { filename,loadAddr,romSize, ROM_LOAD_NORMAL },

#define ROM_LOAD_16B(filename,loadAddr,romSize) { filename,loadAddr,romSize, ROM_LOAD_16BYTE },
#define ROM_RELOAD(loadAddr,romSize) { (char *)-1, loadAddr,romSize, ROM_LOAD_NORMAL },
#define ROM_CONTINUE(loadAddr,romSize) { (char *)-2, loadAddr,romSize, ROM_LOAD_NORMAL },
#define ROM_END {NULL, 0, 0, 0}};




ROM_START( alphaone )
	ROM_REGION( 0x22000, CPU0 )
	/* Vector Generator ROM */
	ROM_LOAD( "vec5000.tw",   0x05000, 0x1000 )
	/* Program ROM */
	ROM_LOAD( "8000.tw",      0x08000, 0x2000 )
	ROM_LOAD( "a000.tw",      0x0a000, 0x2000 )
	ROM_LOAD( "twjk1.bin",    0x0c000, 0x2000 )
	ROM_LOAD( "e000.tw",      0x0e000, 0x1000 )
	ROM_LOAD( "e000.tw",      0x0f000, 0x1000 )
	/* Paged Program ROM - switched to 2000-3fff */
	ROM_LOAD( "page01.tw",    0x10000, 0x4000 )
	/* Paged Vector Generator ROM */
	ROM_LOAD( "vec_pg01.tw",  0x18000, 0x4000 )
	ROM_LOAD( "vec_pg23.tw",  0x1c000, 0x4000 )
	/* the last 0x1000 is used for the 2 RAM pages */
ROM_END


ROM_START( alphaona )
	ROM_REGION( 0x22000, CPU0 )
	/* Vector Generator ROM */
	ROM_LOAD( "vec5000.tw",   0x05000, 0x1000 )
	/* Program ROM */
	ROM_LOAD( "8000.tw",      0x08000, 0x2000 )
	ROM_LOAD( "a000.tw",      0x0a000, 0x2000 )
	ROM_LOAD( "c000.tw",    0x0c000, 0x2000 )
	ROM_LOAD( "e000.tw",      0x0e000, 0x1000 )
	ROM_LOAD( "e000.tw",      0x0f000, 0x1000 )
	/* Paged Program ROM - switched to 2000-3fff */
	ROM_LOAD( "page01.tw",    0x10000, 0x4000 )
	/* Paged Vector Generator ROM */
	ROM_LOAD( "vec_pg01.tw",  0x18000, 0x4000 )
	ROM_LOAD( "vec_pg23.tw",  0x1c000, 0x4000 )
	/* the last 0x1000 is used for the 2 RAM pages */
ROM_END

ROM_START( mhavoc2 )
	/* Alpha Processor ROMs */
	 ROM_REGION(0x22000,CPU0)
	/* Vector Generator ROM */
	ROM_LOAD( "136025.110",   0x05000, 0x2000 )
	/* Program ROM */
	ROM_LOAD( "136025.103",   0x08000, 0x4000 )
	ROM_LOAD( "136025.104",   0x0c000, 0x4000 )
	/* Paged Program ROM - switched to 2000-3fff */
	ROM_LOAD( "136025.101",   0x10000, 0x4000 ) /* page 0+1 */
	ROM_LOAD( "136025.109",   0x14000, 0x4000 ) /* page 2+3 */
	/* Paged Vector Generator ROM */
	ROM_LOAD( "136025.106",   0x18000, 0x4000 ) /* page 0+1 */
	ROM_LOAD( "136025.107",   0x1c000, 0x4000 ) /* page 2+3 */
    /* Gamma Processor ROM */
	ROM_REGION(0x10000,CPU1)
	ROM_LOAD( "136025.108",   0x8000, 0x4000 )
	ROM_LOAD( "136025.108",   0xc000, 0x4000 ) /* reset+interrupt vectors */
ROM_END

ROM_START( mhavocrv )
	/* Alpha Processor ROMs */
	 ROM_REGION(0x22000,CPU0)
	/* Vector Generator ROM */
	ROM_LOAD( "136025.210",   0x05000, 0x2000 )
	/* Program ROM */
	ROM_LOAD( "136025.916",   0x08000, 0x4000 )
	ROM_LOAD( "136025.917",   0x0c000, 0x4000 )
	/* Paged Program ROM */
	ROM_LOAD( "136025.915",   0x10000, 0x4000 ) /* page 0+1 */
	ROM_LOAD( "136025.918",   0x14000, 0x4000 ) /* page 2+3 */
	/* Paged Vector Generator ROM */
	ROM_LOAD( "136025.106",   0x18000, 0x4000 ) /* page 0+1 */
	ROM_LOAD( "136025.907",   0x1c000, 0x4000 ) /* page 2+3 */
	/* Gamma Processor ROM */
	ROM_REGION(0x10000,CPU1)
	ROM_LOAD( "136025.908",    0x8000, 0x4000 )
	ROM_LOAD( "136025.908",    0xc000, 0x4000 ) /* reset+interrupt vectors */
ROM_END

ROM_START( mhavocp )
	/* Alpha Processor ROMs */
	 ROM_REGION(0x22000,CPU0)
	/* Vector Generator ROM */
	ROM_LOAD( "136025.010",   0x05000, 0x2000 )
	/* Program ROM */
	ROM_LOAD( "136025.016",   0x08000, 0x4000 )
	ROM_LOAD( "136025.017",   0x0c000, 0x4000 )
    /* Paged Program ROM - switched to 2000-3fff */
	ROM_LOAD( "136025.015",   0x10000, 0x4000 )
	ROM_LOAD( "136025.018",   0x14000, 0x4000 )
	/* Paged Vector Generator ROM */
	ROM_LOAD( "136025.006",   0x18000, 0x4000 )
	ROM_LOAD( "136025.007",   0x1c000, 0x4000 )
	/* Gamma Processor ROM */
	ROM_REGION(0x10000,CPU1)
	ROM_LOAD( "136025.008",   0x8000, 0x4000 )
	ROM_LOAD( "136025.008",   0xc000, 0x4000 ) /* reset+interrupt vectors */
ROM_END

ROM_START(mhavoc)  
   ROM_REGION(0x22000,CPU0)
   /* Vector Generator ROM */ 
   ROM_LOAD( "136025.210",   0x05000, 0x2000 )
   /* Program ROM */
   ROM_LOAD( "136025.216",   0x08000, 0x4000 )
   ROM_LOAD( "136025.217",   0x0c000, 0x4000 )
   /* Paged Program ROM */
   ROM_LOAD( "136025.215",   0x10000, 0x4000 ) /* page 0+1 */
   ROM_LOAD( "136025.318",   0x14000, 0x4000 ) /* page 2+3 */
   /* Paged Vector Generator ROM */
   ROM_LOAD( "136025.106",   0x18000, 0x4000 ) /* page 0+1 */
   ROM_LOAD( "136025.107",   0x1c000, 0x4000 ) /* page 2+3 */
   ROM_REGION(0x10000,CPU1)
   //GAMMA CPU
   ROM_LOAD( "136025.108",   0x08000, 0x4000 )
   ROM_LOAD( "136025.108",   0x0c000, 0x4000 ) /* reset+interrupt vectors */
ROM_END

ROM_START(meteorts)
 ROM_REGION(0x10000,CPU0)
 ROM_LOAD("mv_np3.bin", 0x5000, 0x800 )
 ROM_LOAD("m0_c1.bin",  0x6800, 0x800 )
 ROM_LOAD("m1_f1.bin",  0x7000, 0x800 )
 ROM_LOAD("m2_j1.bin",  0x7800, 0x800 )
 ROM_RELOAD( 0xf800, 0x800 )
 ROM_END

ROM_START(asteroib) //Bootleg
 ROM_REGION(0x10000,CPU0)
 ROM_LOAD("035127.02",    0x5000, 0x800 )
 ROM_LOAD("035145ll.bin", 0x6800, 0x800 )
 ROM_LOAD("035144ll.bin", 0x7000, 0x800 )
 ROM_LOAD("035143ll.bin", 0x7800, 0x800 )
 ROM_RELOAD( 0xf800, 0x800 )
ROM_END

ROM_START(asterock)    //ASTEROCK
 ROM_REGION(0x10000,0)
 ROM_LOAD("sidamas.0", 0x5000, 0x400 )
 ROM_LOAD("sidamas.1", 0x5400, 0x400 )
 ROM_LOAD("sidamas.2", 0x6800, 0x400 )
 ROM_LOAD("sidamas.3", 0x6c00, 0x400 )
 ROM_LOAD("sidamas.4", 0x7000, 0x400 )
 ROM_LOAD("sidamas.5", 0x7400, 0x400 )
 ROM_LOAD("sidamas.6", 0x7800, 0x400 )
 ROM_LOAD("sidamas.7", 0x7c00, 0x400 )
 ROM_RELOAD(0xfc00, 0x400 )
ROM_END

ROM_START(asteroi1) //VERSION 1
 ROM_REGION(0x10000,0)
 ROM_LOAD("035127.01", 0x5000, 0x800 )
 ROM_LOAD("035145.01", 0x6800, 0x800 )
 ROM_LOAD("035144.01", 0x7000, 0x800 )
 ROM_LOAD("035143.01", 0x7800, 0x800 )
 ROM_RELOAD( 0xf800, 0x800 )
ROM_END

ROM_START(asteroid) //VERSION 2
 ROM_REGION(0x10000,0)
 ROM_LOAD("035127.02", 0x5000, 2048 )
 ROM_LOAD("035145.02", 0x6800, 2048 )
 ROM_LOAD("035144.02", 0x7000, 2048 )
 ROM_LOAD("035143.02", 0x7800, 2048 )
 ROM_RELOAD( 0xf800, 2048 )
ROM_END

ROM_START( astdelux )
 ROM_REGION(0x10000,0)	
 ROM_LOAD( "036430.02",    0x6000, 0x0800 ) 
 ROM_LOAD( "036431.02",    0x6800, 0x0800 ) 
 ROM_LOAD( "036432.02",    0x7000, 0x0800 ) 
 ROM_LOAD( "036433.03",    0x7800, 0x0800 ) 
 ROM_RELOAD( 0xf800, 0x800 )
 // Vector ROM 
 ROM_LOAD( "036800.02",    0x4800, 0x0800 ) 
 ROM_LOAD( "036799.01",    0x5000, 0x0800 ) 
ROM_END

ROM_START( astdelu2 )
 ROM_REGION(0x10000,0)
 ROM_LOAD( "036430.01",    0x6000, 0x0800 ) 
 ROM_LOAD( "036431.01",    0x6800, 0x0800 ) 
 ROM_LOAD( "036432.01",    0x7000, 0x0800 ) 
 ROM_LOAD( "036433.02",    0x7800, 0x0800 ) 
 ROM_RELOAD( 0xf800, 0x800)
 // Vector ROM 
 ROM_LOAD( "036800.01",    0x4800, 0x0800 ) 
 ROM_LOAD( "036799.01",    0x5000, 0x0800 ) 
ROM_END

ROM_START(astdelu1) //VERSION 1
 ROM_REGION(0x10000,0)
 ROM_LOAD ("036800.01", 0x4800, 0x800)
 ROM_LOAD ("036799.01", 0x5000, 0x800)
 ROM_LOAD ("036430.01", 0x6000, 0x800)
 ROM_LOAD ("036431.01", 0x6800, 0x800)
 ROM_LOAD ("036432.01", 0x7000, 0x800)
 ROM_LOAD ("036433.01", 0x7800, 0x800)
 ROM_RELOAD( 0xf800, 0x800)
ROM_END

ROM_START( bwidow )
	ROM_REGION(0x10000,0)
	/* Vector ROM */
	ROM_LOAD( "136017.107",   0x2800, 0x0800)
	ROM_LOAD( "136017.108",   0x3000, 0x1000)
	ROM_LOAD( "136017.109",   0x4000, 0x1000)
	ROM_LOAD( "136017.110",   0x5000, 0x1000)
	/* Program ROM */
	ROM_LOAD( "136017.101",   0x9000, 0x1000)
	ROM_LOAD( "136017.102",   0xa000, 0x1000)
	ROM_LOAD( "136017.103",   0xb000, 0x1000)
	ROM_LOAD( "136017.104",   0xc000, 0x1000)
	ROM_LOAD( "136017.105",   0xd000, 0x1000)
	ROM_LOAD( "136017.106",   0xe000, 0x1000)
	ROM_RELOAD(               0xf000, 0x1000) // for reset/interrupt vectors 
ROM_END

ROM_START( gravitar )
	ROM_REGION(0x10000,0)
	/* Vector ROM */
	ROM_LOAD( "136010.210",   0x2800, 0x0800)
	ROM_LOAD( "136010.207",   0x3000, 0x1000)
	ROM_LOAD( "136010.208",   0x4000, 0x1000)
	ROM_LOAD( "136010.309",   0x5000, 0x1000)
	/* Program ROM */
	ROM_LOAD( "136010.301",   0x9000, 0x1000)
	ROM_LOAD( "136010.302",   0xa000, 0x1000)
	ROM_LOAD( "136010.303",   0xb000, 0x1000)
	ROM_LOAD( "136010.304",   0xc000, 0x1000)
	ROM_LOAD( "136010.305",   0xd000, 0x1000)
	ROM_LOAD( "136010.306",   0xe000, 0x1000)
	ROM_RELOAD(               0xf000, 0x1000 ) // for reset/interrupt vectors 
ROM_END

ROM_START( gravitr2 )
	ROM_REGION(0x10000,0)
	/* Vector ROM */
	ROM_LOAD( "136010.210",   0x2800, 0x0800)
	ROM_LOAD( "136010.207",   0x3000, 0x1000)
	ROM_LOAD( "136010.208",   0x4000, 0x1000)
	ROM_LOAD( "136010.209",   0x5000, 0x1000)
	/* Program ROM */
	ROM_LOAD( "136010.201",   0x9000, 0x1000)
	ROM_LOAD( "136010.202",   0xa000, 0x1000)
	ROM_LOAD( "136010.203",   0xb000, 0x1000)
	ROM_LOAD( "136010.204",   0xc000, 0x1000)
	ROM_LOAD( "136010.205",   0xd000, 0x1000)
	ROM_LOAD( "136010.206",   0xe000, 0x1000)
	ROM_RELOAD(               0xf000, 0x1000) //for reset/interrupt vectors
ROM_END

ROM_START( gravp )
	ROM_REGION(0x10000,0)
	/* Vector ROM */
	ROM_LOAD( "l7.bin",   0x2800, 0x0800)
	ROM_LOAD( "mn7.bin",  0x3000, 0x1000)
	ROM_LOAD( "np7.bin",  0x4000, 0x1000)
	ROM_LOAD( "r7.bin",   0x5000, 0x1000)
	/* Program ROM */
	ROM_LOAD( "d1.bin",   0x9000, 0x1000)
	ROM_LOAD( "ef1.bin",  0xa000, 0x1000)
	ROM_LOAD( "h1.bin",   0xb000, 0x1000)
	ROM_LOAD( "j1.bin",   0xc000, 0x1000)
	ROM_LOAD( "kl1.bin",  0xd000, 0x1000)
	ROM_LOAD( "m1.bin",   0xe000, 0x1000)
	ROM_RELOAD(           0xf000, 0x1000)	// for reset/interrupt vectors 
ROM_END

ROM_START( lunarbat )
	ROM_REGION(0x10000,0)
	/* Vector ROM */
	ROM_LOAD( "010.010",      0x2800, 0x0800)
	ROM_LOAD( "007.010",      0x3000, 0x1000)
	ROM_LOAD( "008.010",      0x4000, 0x1000)
	/* Program ROM */
	ROM_LOAD( "001.010",      0x9000, 0x1000)
	ROM_LOAD( "002.010",      0xa000, 0x1000)
	ROM_LOAD( "003.010",      0xb000, 0x1000)
	ROM_LOAD( "004.010",      0xc000, 0x1000)
	ROM_LOAD( "005.010",      0xd000, 0x1000)
	ROM_LOAD( "006.010",      0xe000, 0x1000)
	ROM_RELOAD(               0xf000, 0x1000) // for reset/interrupt vectors 
ROM_END

ROM_START( lunarba1 )
	ROM_REGION(0x10000,0)
	/* Vector ROM */
	ROM_LOAD( "vrom1.bin",   0x2800, 0x0800)
	ROM_LOAD( "vrom2.bin",   0x3000, 0x1000)
	/* Program ROM */
	ROM_LOAD( "rom0.bin",    0x4000, 0x1000)
	ROM_LOAD( "rom1.bin",    0x5000, 0x1000)
	ROM_LOAD( "rom2.bin",    0x6000, 0x1000)
	ROM_LOAD( "rom3.bin",    0x7000, 0x1000)
	ROM_LOAD( "rom4.bin",    0x8000, 0x1000)
	ROM_LOAD( "rom5.bin",    0x9000, 0x1000)
	ROM_LOAD( "rom5.bin",    0xa000, 0x1000)
	ROM_LOAD( "rom5.bin",    0xb000, 0x1000)
	ROM_LOAD( "rom5.bin",    0xc000, 0x1000)
	ROM_LOAD( "rom5.bin",    0xd000, 0x1000)
	ROM_LOAD( "rom5.bin",    0xe000, 0x1000)
	ROM_LOAD( "rom5.bin",    0xf000, 0x1000) // for reset/interrupt vectors 
ROM_END

ROM_START( spacduel )
	ROM_REGION(0x10000,0)
	/* Vector ROM */
	ROM_LOAD( "136006.106",   0x2800, 0x0800)
	ROM_LOAD( "136006.107",   0x3000, 0x1000)
	/* Program ROM */
	ROM_LOAD( "136006.201",   0x4000, 0x1000)
	ROM_LOAD( "136006.102",   0x5000, 0x1000)
	ROM_LOAD( "136006.103",   0x6000, 0x1000)
	ROM_LOAD( "136006.104",   0x7000, 0x1000)
	ROM_LOAD( "136006.105",   0x8000, 0x1000)
	ROM_LOAD( "136006.105",   0x9000, 0x1000 )
	ROM_LOAD( "136006.105",   0xa000, 0x1000 )
	ROM_LOAD( "136006.105",   0xb000, 0x1000 )
	ROM_LOAD( "136006.105",   0xc000, 0x1000 )
	ROM_LOAD( "136006.105",   0xd000, 0x1000 )
	ROM_LOAD( "136006.105",   0xe000, 0x1000 )
	ROM_RELOAD(               0xf000, 0x1000 )	/* for reset/interrupt vectors */
ROM_END

ROM_START(alienst) //ALIENS PROTO
 ROM_REGION(0x10000,0)
 ROM_LOAD("aliens_d1.bin", 0x9000, 0x0800)
 ROM_LOAD("aliens_e1.bin", 0x9800, 0x0800)
 ROM_LOAD("aliens_f1.bin", 0xa000, 0x0800)
 ROM_LOAD("aliens_h1.bin", 0xa800, 0x0800)
 ROM_LOAD("aliens_j1.bin", 0xb000, 0x0800)
 ROM_LOAD("aliens_k1.bin", 0xb800, 0x0800)
 ROM_LOAD("aliens_l1.bin", 0xc000, 0x0800)
 ROM_LOAD("aliens_n1.bin", 0xc800, 0x0800)
 ROM_LOAD("aliens_p1.bin", 0xd000, 0x0800)
 ROM_LOAD("aliens_r1.bin", 0xd800, 0x0800)  
 ROM_RELOAD(                0xf800, 0x0800) //Reload 
 //Vector Rom
 ROM_LOAD("aliens_n3.bin", 0x3000, 0x0800)
 ROM_LOAD("aliens_r3.bin", 0x3800, 0x0800)
ROM_END

ROM_START(vortex)//VORTEX ROMSET
 ROM_REGION(0x10000,0) 
 ROM_LOAD("d1.bin",  0x9000, 0x0800)
 ROM_LOAD("e1.bin",  0x9800, 0x0800)
 ROM_LOAD("f1.bin",  0xa000, 0x0800)
 ROM_LOAD("h1.bin",  0xa800, 0x0800)
 ROM_LOAD("j1.bin",  0xb000, 0x0800)  
 ROM_LOAD("k1.bin",  0xb800, 0x0800)
 ROM_LOAD("lm1.bin", 0xc000, 0x0800)
 ROM_LOAD("mn1.bin", 0xc800, 0x0800)
 ROM_LOAD("p1.bin",  0xd000, 0x0800)
 ROM_LOAD("r1.bin",  0xd800, 0x0800)  
 ROM_RELOAD(         0xf800, 0x0800)//Reload  
 //Vector Rom
 ROM_LOAD("n3.bin",  0x3000, 0x0800)
 ROM_LOAD("r3.bin",  0x3800, 0x0800)
ROM_END

ROM_START(vbreak)//VECTOR BREAKOUT
 ROM_REGION(0x10000,0)
 ROM_LOAD("vb_d1.bin",  0x9000, 0x0800)
 ROM_LOAD("vb_e1.bin",  0x9800, 0x0800)
 ROM_LOAD("vb_f1.bin",  0xa000, 0x0800)
 ROM_LOAD("136002.113", 0xa800, 0x0800)
 ROM_LOAD("136002.114", 0xb000, 0x0800)  
 ROM_LOAD("136002.115", 0xb800, 0x0800)
 ROM_LOAD("136002.116", 0xc000, 0x0800)
 ROM_LOAD("136002.117", 0xc800, 0x0800)
 ROM_LOAD("136002.118", 0xd000, 0x0800)
 ROM_LOAD("vb_r1.bin",  0xd800, 0x0800)  
 ROM_RELOAD(            0xf800, 0x0800)//Reload   
 //Wector Rom
 ROM_LOAD("vb_n3.bin",  0x3000, 0x0800)
 ROM_LOAD("vb_r3.bin",  0x3800, 0x0800)
ROM_END

ROM_START(temptube)//TEMPEST TUBES
 ROM_REGION(0x10000,0)
 ROM_LOAD( "136002-113.d1",   0x9000, 0x0800)
 ROM_LOAD( "136002-114.e1",   0x9800, 0x0800)
 ROM_LOAD( "136002-115.f1",   0xa000, 0x0800)
 ROM_LOAD( "136002-316.h1",   0xa800, 0x0800)
 ROM_LOAD( "136002-217.j1",   0xb000, 0x0800)
 ROM_LOAD( "tube-118.k1",     0xb800, 0x0800)
 ROM_LOAD( "136002-119.lm1",  0xc000, 0x0800)
 ROM_LOAD( "136002-120.mn1",  0xc800, 0x0800)
 ROM_LOAD( "136002-121.p1",   0xd000, 0x0800)
 ROM_LOAD( "136002-222.r1",   0xd800, 0x0800)
 ROM_RELOAD(                  0xf800, 0x0800)//Reload 
 // Vector ROM 
 ROM_LOAD( "136002-123.np3",  0x3000, 0x0800)
 ROM_LOAD( "136002-124.r3",   0x3800, 0x0800)
ROM_END

///////////////////////////////////////////////////////////////////////////////////////////////////////////
ROM_START( tempest ) // rev 3 
	ROM_REGION(0x10000,0)
    // Roms are for Tempest Analog Vector-Generator PCB Assembly A037383-01 or A037383-02 
	ROM_LOAD( "136002-113.d1",   0x9000, 0x0800)
	ROM_LOAD( "136002-114.e1",   0x9800, 0x0800)
	ROM_LOAD( "136002-115.f1",   0xa000, 0x0800)
	ROM_LOAD( "136002-316.h1",   0xa800, 0x0800)
	ROM_LOAD( "136002-217.j1",   0xb000, 0x0800)
	ROM_LOAD( "136002-118.k1",   0xb800, 0x0800)
	ROM_LOAD( "136002-119.lm1",  0xc000, 0x0800)
	ROM_LOAD( "136002-120.mn1",  0xc800, 0x0800)
	ROM_LOAD( "136002-121.p1",   0xd000, 0x0800)
	ROM_LOAD( "136002-222.r1",   0xd800, 0x0800)
	ROM_RELOAD(                  0xf800, 0x0800)//Reload 
	// Vector ROM 
	ROM_LOAD( "136002-123.np3",  0x3000, 0x0800)
	ROM_LOAD( "136002-124.r3",   0x3800, 0x0800)
ROM_END

ROM_START( tempest1 ) /* rev 1 */
	ROM_REGION(0x10000,0)
    // Roms are for Tempest Analog Vector-Generator PCB Assembly A037383-01 or A037383-02 
	ROM_LOAD( "136002-113.d1",   0x9000, 0x0800)
	ROM_LOAD( "136002-114.e1",   0x9800, 0x0800)
	ROM_LOAD( "136002-115.f1",   0xa000, 0x0800)
	ROM_LOAD( "136002-116.h1",   0xa800, 0x0800)
	ROM_LOAD( "136002-117.j1",   0xb000, 0x0800)
	ROM_LOAD( "136002-118.k1",   0xb800, 0x0800)
	ROM_LOAD( "136002-119.lm1",  0xc000, 0x0800)
	ROM_LOAD( "136002-120.mn1",  0xc800, 0x0800)
	ROM_LOAD( "136002-121.p1",   0xd000, 0x0800)
	ROM_LOAD( "136002-122.r1",   0xd800, 0x0800)
	ROM_RELOAD(                  0xf800, 0x0800) //Reload 
	// Vector ROM 
	ROM_LOAD( "136002-123.np3",  0x3000, 0x0800)
	ROM_LOAD( "136002-124.r3",   0x3800, 0x0800)
ROM_END
ROM_START( tempest2 ) // rev 2 
	ROM_REGION(0x10000,0)
    // Roms are for Tempest Analog Vector-Generator PCB Assembly A037383-01 or A037383-02 
	ROM_LOAD( "136002-113.d1",   0x9000, 0x0800)
	ROM_LOAD( "136002-114.e1",   0x9800, 0x0800)
	ROM_LOAD( "136002-115.f1",   0xa000, 0x0800)
	ROM_LOAD( "136002-116.h1",   0xa800, 0x0800)
	ROM_LOAD( "136002-217.j1",   0xb000, 0x0800)
	ROM_LOAD( "136002-118.k1",   0xb800, 0x0800)
	ROM_LOAD( "136002-119.lm1",  0xc000, 0x0800)
	ROM_LOAD( "136002-120.mn1",  0xc800, 0x0800)
	ROM_LOAD( "136002-121.p1",   0xd000, 0x0800)
	ROM_LOAD( "136002-222.r1",   0xd800, 0x0800)
	ROM_RELOAD(                  0xf800, 0x0800)//Reload 
	// Vector ROM 
	ROM_LOAD( "136002-123.np3",  0x3000, 0x0800)
	ROM_LOAD( "136002-124.r3",   0x3800, 0x0800)
ROM_END

ROM_START( tempest3 ) // rev 2 
	ROM_REGION(0x10000,0)
    // Roms are for Tempest Analog Vector-Generator PCB Assembly A037383-03 or A037383-04 
	ROM_LOAD( "136002-237.p1",  0x9000, 0x1000)
	ROM_LOAD( "136002-136.lm1", 0xa000, 0x1000)
	ROM_LOAD( "136002-235.j1",  0xb000, 0x1000)
	ROM_LOAD( "136002-134.f1",  0xc000, 0x1000)
	ROM_LOAD( "136002-133.d1",  0xd000, 0x1000)
	ROM_RELOAD(                 0xf000, 0x1000)//Reload 
	// Vector ROM 
	ROM_LOAD( "136002-138.np3", 0x3000, 0x1000)
ROM_END
                  
ROM_START(zektor)
ROM_REGION(0x10000,0)
 ROM_LOAD("1611.cpu-u25",  0x0000, 0x0800)
 ROM_LOAD("1586.prom-u1",  0x0800, 0x0800)
 ROM_LOAD("1587.prom-u2",  0x1000, 0x0800)
 ROM_LOAD("1588.prom-u3",  0x1800, 0x0800)
 ROM_LOAD("1589.prom-u4",  0x2000, 0x0800)
 ROM_LOAD("1590.prom-u5",  0x2800, 0x0800)
 ROM_LOAD("1591.prom-u6",  0x3000, 0x0800)
 ROM_LOAD("1592.prom-u7",  0x3800, 0x0800)
 ROM_LOAD("1593.prom-u8",  0x4000, 0x0800)
 ROM_LOAD("1594.prom-u9",  0x4800, 0x0800)
 ROM_LOAD("1595.prom-u10", 0x5000, 0x0800)
 ROM_LOAD("1596.prom-u11", 0x5800, 0x0800)
 ROM_LOAD("1597.prom-u12", 0x6000, 0x0800)
 ROM_LOAD("1598.prom-u13", 0x6800, 0x0800)
 ROM_LOAD("1599.prom-u14", 0x7000, 0x0800)
 ROM_LOAD("1600.prom-u15", 0x7800, 0x0800)
 ROM_LOAD("1601.prom-u16", 0x8000, 0x0800)
 ROM_LOAD("1602.prom-u17", 0x8800, 0x0800)
 ROM_LOAD("1603.prom-u18", 0x9000, 0x0800)
 ROM_LOAD("1604.prom-u19", 0x9800, 0x0800)
 ROM_LOAD("1605.prom-u20", 0xa000, 0x0800)
 ROM_LOAD("1606.prom-u21", 0xa800, 0x0800)
 ROM_REGION(0x10000,1)
 ROM_LOAD( "s-c.xyt-u39", 0x0000, 0x0400)
ROM_END	

ROM_START(tacscan)
ROM_REGION(0x10000,0)
 ROM_LOAD("1711a.cpu-u25", 0x0000,2048)
 ROM_LOAD("1670c.prom-u1", 0x0800,2048)
 ROM_LOAD("1671a.prom-u2", 0x1000,2048)
 ROM_LOAD("1672a.prom-u3", 0x1800,2048)
 ROM_LOAD("1673a.prom-u4", 0x2000,2048)
 ROM_LOAD("1674a.prom-u5", 0x2800,2048)
 ROM_LOAD("1675a.prom-u6", 0x3000,2048)
 ROM_LOAD("1676a.prom-u7", 0x3800,2048)
 ROM_LOAD("1677a.prom-u8", 0x4000,2048)
 ROM_LOAD("1678b.prom-u9", 0x4800,2048)
 ROM_LOAD("1679a.prom-u10", 0x5000,2048)
 ROM_LOAD("1680a.prom-u11", 0x5800,2048)
 ROM_LOAD("1681a.prom-u12", 0x6000,2048)
 ROM_LOAD("1682a.prom-u13", 0x6800,2048)
 ROM_LOAD("1683a.prom-u14", 0x7000,2048)
 ROM_LOAD("1684a.prom-u15", 0x7800,2048)
 ROM_LOAD("1685a.prom-u16", 0x8000,2048)
 ROM_LOAD("1686a.prom-u17", 0x8800,2048)
 ROM_LOAD("1687a.prom-u18", 0x9000,2048)
 ROM_LOAD("1688a.prom-u19", 0x9800,2048)
 ROM_LOAD("1709a.prom-u20", 0xa000,2048)
 ROM_LOAD("1710a.prom-u21", 0xa800,2048)
 ROM_REGION(0x10000,1)
 ROM_LOAD( "s-c.xyt-u39", 0x0000, 0x0400)
ROM_END	

ROM_START(spacfura)
ROM_REGION(0x10000,0)
 ROM_LOAD("969a.cpu-u25",0x0000, 2048)
 ROM_LOAD("960a.prom-u1", 0x0800, 2048)
 ROM_LOAD("961a.prom-u2", 0x1000, 2048)
 ROM_LOAD("962a.prom-u3", 0x1800, 2048)
 ROM_LOAD("963a.prom-u4", 0x2000, 2048)
 ROM_LOAD("964a.prom-u5", 0x2800, 2048)
 ROM_LOAD("965a.prom-u6", 0x3000, 2048)
 ROM_LOAD("966a.prom-u7", 0x3800, 2048)
 ROM_LOAD("967a.prom-u8", 0x4000, 2048)
 ROM_LOAD("968a.prom-u9", 0x4800, 2048)
 ROM_REGION(0x10000,1)
 ROM_LOAD( "s-c.xyt-u39", 0x0000, 0x0400)
ROM_END	

ROM_START(spacfury)
ROM_REGION(0x10000,0)
 ROM_LOAD("969c.cpu-u25", 0x0000, 2048)
 ROM_LOAD("960c.prom-u1", 0x0800, 2048)
 ROM_LOAD("961c.prom-u2", 0x1000, 2048)
 ROM_LOAD("962c.prom-u3", 0x1800, 2048)
 ROM_LOAD("963c.prom-u4", 0x2000, 2048)
 ROM_LOAD("964c.prom-u5", 0x2800, 2048)
 ROM_LOAD("965c.prom-u6", 0x3000, 2048)
 ROM_LOAD("966c.prom-u7", 0x3800, 2048)
 ROM_LOAD("967c.prom-u8", 0x4000, 2048)
 ROM_LOAD("968c.prom-u9", 0x4800, 2048)
ROM_REGION(0x10000,1)
 ROM_LOAD( "s-c.xyt-u39", 0x0000, 0x0400)
ROM_END

ROM_START(spacfurb)
ROM_REGION(0x10000,0)
 ROM_LOAD("969a.cpu-u25", 0x0000, 2048)
 ROM_LOAD("960b.prom-u1", 0x0800, 2048)
 ROM_LOAD("961b.prom-u2", 0x1000, 2048)
 ROM_LOAD("962b.prom-u3", 0x1800, 2048)
 ROM_LOAD("963b.prom-u4", 0x2000, 2048)
 ROM_LOAD("964b.prom-u5", 0x2800, 2048)
 ROM_LOAD("965b.prom-u6", 0x3000, 2048)
 ROM_LOAD("966b.prom-u7", 0x3800, 2048)
 ROM_LOAD("967b.prom-u8", 0x4000, 2048)
 ROM_LOAD("968b.prom-u9", 0x4800, 2048)
 ROM_REGION(0x10000,1)
 ROM_LOAD( "s-c.xyt-u39", 0x0000, 0x0400)
ROM_END	

ROM_START(startrek)
ROM_REGION(0x10000,0)
 ROM_LOAD("1873.cpu-u25", 0x0000, 0x0800)
 ROM_LOAD("1848.prom-u1", 0x0800, 0x0800)
 ROM_LOAD("1849.prom-u2", 0x1000, 0x0800)
 ROM_LOAD("1850.prom-u3", 0x1800, 0x0800)
 ROM_LOAD("1851.prom-u4", 0x2000, 0x0800)
 ROM_LOAD("1852.prom-u5", 0x2800, 0x0800)
 ROM_LOAD("1853.prom-u6", 0x3000, 0x0800)
 ROM_LOAD("1854.prom-u7", 0x3800, 0x0800)
 ROM_LOAD("1855.prom-u8", 0x4000, 0x0800)
 ROM_LOAD("1856.prom-u9", 0x4800, 0x0800)
 ROM_LOAD("1857.prom-u10", 0x5000, 0x0800)
 ROM_LOAD("1858.prom-u11", 0x5800, 0x0800)
 ROM_LOAD("1859.prom-u12", 0x6000, 0x0800)
 ROM_LOAD("1860.prom-u13", 0x6800, 0x0800)
 ROM_LOAD("1861.prom-u14", 0x7000, 0x0800)
 ROM_LOAD("1862.prom-u15", 0x7800, 0x0800)
 ROM_LOAD("1863.prom-u16", 0x8000, 0x0800)
 ROM_LOAD("1864.prom-u17", 0x8800, 0x0800)
 ROM_LOAD("1865.prom-u18", 0x9000, 0x0800)
 ROM_LOAD("1866.prom-u19", 0x9800, 0x0800)
 ROM_LOAD("1867.prom-u20", 0xa000, 0x0800)
 ROM_LOAD("1868.prom-u21", 0xa800, 0x0800)
 ROM_LOAD("1869.prom-u22", 0xb000, 0x0800)
 ROM_LOAD("1870.prom-u23", 0xb800, 0x0800)
 ROM_REGION(0x10000,1)
 ROM_LOAD( "s-c.xyt-u39", 0x0000, 0x0400)
ROM_END	

ROM_START(elim2)
ROM_REGION(0x10000,0)
 ROM_LOAD("969.cpu-u25",   0x0000, 0x0800)
 ROM_LOAD("1333.prom-u1",  0x0800, 0x0800)
 ROM_LOAD("1334.prom-u2",  0x1000, 0x0800)
 ROM_LOAD("1335.prom-u3",  0x1800, 0x0800)
 ROM_LOAD("1336.prom-u4",  0x2000, 0x0800)
 ROM_LOAD("1337.prom-u5",  0x2800, 0x0800)
 ROM_LOAD("1338.prom-u6",  0x3000, 0x0800)
 ROM_LOAD("1339.prom-u7",  0x3800, 0x0800)
 ROM_LOAD("1340.prom-u8",  0x4000, 0x0800)
 ROM_LOAD("1341.prom-u9",  0x4800, 0x0800)
 ROM_LOAD("1342.prom-u10", 0x5000, 0x0800)
 ROM_LOAD("1343.prom-u11", 0x5800, 0x0800)
 ROM_LOAD("1344.prom-u12", 0x6000, 0x0800)
 ROM_LOAD("1345.prom-u13", 0x6800, 0x0800)
 ROM_REGION(0x10000,1)
 ROM_LOAD( "s-c.xyt-u39", 0x0000, 0x0400)
ROM_END	

ROM_START(elim2a)
ROM_REGION(0x10000,0)
 ROM_LOAD("969.cpu-u25",   0x0000, 0x0800)
 ROM_LOAD("1333.prom-u1",  0x0800, 0x0800)
 ROM_LOAD("1334.prom-u2",  0x1000, 0x0800)
 ROM_LOAD("1335.prom-u3",  0x1800, 0x0800)
 ROM_LOAD("1336.prom-u4",  0x2000, 0x0800)
 ROM_LOAD("1337.prom-u5",  0x2800, 0x0800)
 ROM_LOAD("1338.prom-u6",  0x3000, 0x0800)
 ROM_LOAD("1339.prom-u7",  0x3800, 0x0800)
 ROM_LOAD("1340.prom-u8",  0x4000, 0x0800)
 ROM_LOAD("1341.prom-u9",  0x4800, 0x0800)
 ROM_LOAD("1342.prom-u10", 0x5000, 0x0800)
 ROM_LOAD("1343.prom-u11", 0x5800, 0x0800)
 ROM_LOAD("1344.prom-u12", 0x6000, 0x0800)
 ROM_LOAD("1345.prom-u13", 0x6800, 0x0800)
 ROM_REGION(0x10000,1)
 ROM_LOAD( "s-c.xyt-u39", 0x0000, 0x0400)
ROM_END	

ROM_START(elim2c)
ROM_REGION(0x10000,0)
 ROM_LOAD("969t.cpu-u25",   0x0000, 0x0800)
 ROM_LOAD("1200.prom-u1",  0x0800, 0x0800)
 ROM_LOAD("1200.prom-u2",  0x1000, 0x0800)
 ROM_LOAD("1200.prom-u3",  0x1800, 0x0800)
 ROM_LOAD("1200.prom-u4",  0x2000, 0x0800)
 ROM_LOAD("1200.prom-u5",  0x2800, 0x0800)
 ROM_LOAD("1200.prom-u6",  0x3000, 0x0800)
 ROM_LOAD("1200.prom-u7",  0x3800, 0x0800)
 ROM_LOAD("1200.prom-u8",  0x4000, 0x0800)
 ROM_LOAD("1200.prom-u9",  0x4800, 0x0800)
 ROM_LOAD("1200.prom-u10", 0x5000, 0x0800)
 ROM_LOAD("1200.prom-u11", 0x5800, 0x0800)
 ROM_LOAD("1200.prom-u12", 0x6000, 0x0800)
 ROM_LOAD("1200.prom-u13", 0x6800, 0x0800)
 ROM_REGION(0x10000,1)
 ROM_LOAD( "s-c.xyt-u39", 0x0000, 0x0400)
ROM_END	

ROM_START(elim4)
ROM_REGION(0x10000,0)
 ROM_LOAD( "1390.cpu-u25",  0x0000, 0x0800)
 ROM_LOAD( "1347.prom-u1",  0x0800, 0x0800)
 ROM_LOAD( "1348.prom-u2",  0x1000, 0x0800)
 ROM_LOAD( "1349.prom-u3",  0x1800, 0x0800)
 ROM_LOAD( "1350.prom-u4",  0x2000, 0x0800)
 ROM_LOAD( "1351.prom-u5",  0x2800, 0x0800)
 ROM_LOAD( "1352.prom-u6",  0x3000, 0x0800)
 ROM_LOAD( "1353.prom-u7",  0x3800, 0x0800)
 ROM_LOAD( "1354.prom-u8",  0x4000, 0x0800)
 ROM_LOAD( "1355.prom-u9",  0x4800, 0x0800)
 ROM_LOAD( "1356.prom-u10", 0x5000, 0x0800)
 ROM_LOAD( "1357.prom-u11", 0x5800, 0x0800)
 ROM_LOAD( "1358.prom-u12", 0x6000, 0x0800)
 ROM_LOAD( "1359.prom-u13", 0x6800, 0x0800)
 ROM_LOAD( "1360.prom-u14", 0x7000, 0x0800)
 ROM_REGION(0x10000,1)
 ROM_LOAD( "s-c.xyt-u39", 0x0000, 0x0400)
ROM_END	

ROM_START(elim4p)
ROM_REGION(0x10000,0)
 ROM_LOAD( "1390.cpu-u25",  0x0000, 0x0800)
 ROM_LOAD( "sw1.prom-u1",  0x0800, 0x0800)
 ROM_LOAD( "sw1.prom-u2",  0x1000, 0x0800)
 ROM_LOAD( "sw1.prom-u3",  0x1800, 0x0800)
 ROM_LOAD( "sw1.prom-u4",  0x2000, 0x0800)
 ROM_LOAD( "sw1.prom-u5",  0x2800, 0x0800)
 ROM_LOAD( "sw1.prom-u6",  0x3000, 0x0800)
 ROM_LOAD( "sw1.prom-u7",  0x3800, 0x0800)
 ROM_LOAD( "sw1.prom-u8",  0x4000, 0x0800)
 ROM_LOAD( "sw1.prom-u9",  0x4800, 0x0800)
 ROM_LOAD( "sw1.prom-u10", 0x5000, 0x0800)
 ROM_LOAD( "sw1.prom-u11", 0x5800, 0x0800)
 ROM_LOAD( "sw1.prom-u12", 0x6000, 0x0800)
 ROM_LOAD( "sw1.prom-u13", 0x6800, 0x0800)
 ROM_LOAD( "sw1.prom-u14", 0x7000, 0x0800)
 ROM_REGION(0x10000,1)
 ROM_LOAD( "s-c.xyt-u39", 0x0000, 0x0400)
ROM_END	

ROM_START(bzone)
 ROM_REGION(0x10000,0)
 ROM_LOAD("036422.01", 0x3000, 0x800)
 ROM_LOAD("036421.01", 0x3800, 0x800)
 ROM_LOAD("036414.01", 0x5000, 0x800)
 ROM_LOAD("036413.01", 0x5800, 0x800)
 ROM_LOAD("036412.01", 0x6000, 0x800)
 ROM_LOAD("036411.01", 0x6800, 0x800)
 ROM_LOAD("036410.01", 0x7000, 0x800)
 ROM_LOAD("036409.01", 0x7800, 0x800)
 ROM_LOAD("036409.01", 0xf800, 0x800)//reload
 ROM_RELOAD(                 0xf800, 0x800)//Reload 
ROM_END

ROM_START(barrier)
ROM_REGION(0x10000,0)
 ROM_LOAD_16B("barrier.t7", 0x0000, 0x800)
 ROM_LOAD_16B ("barrier.p7", 0x0000, 0x800 )
ROM_END

ROM_START(starhawk)
ROM_REGION(0x1000,0)
 ROM_LOAD_16B("starhawk.u7", 0x0000, 0x800)
 ROM_LOAD_16B ("starhawk.r7", 0x0000, 0x800 )
ROM_END

ROM_START(speedfrk)
ROM_REGION(0x10000,0)
 ROM_LOAD_16B("speedfrk.t7", 0x0000, 0x800)
 ROM_LOAD_16B ("speedfrk.p7", 0x0000, 0x800 )
 ROM_LOAD_16B("speedfrk.u7", 0x1000, 0x800)
 ROM_LOAD_16B ("speedfrk.r7", 0x1000, 0x800 )
ROM_END

ROM_START(solarq)
ROM_REGION(0x10000,0)
 ROM_LOAD_16B("solar.6t", 0x0000, 0x1000)
 ROM_LOAD_16B ("solar.6p", 0x0000, 0x1000 )
 ROM_LOAD_16B("solar.6u", 0x2000, 0x1000)
 ROM_LOAD_16B ("solar.6r", 0x2000, 0x1000 )
ROM_END

ROM_START(sundance)
ROM_REGION(0x10000,0)
 ROM_LOAD_16B("sundance.t7", 0x0000, 0x800)
 ROM_LOAD_16B ("sundance.p7", 0x0000, 0x800 )
 ROM_LOAD_16B("sundance.u7", 0x1000, 0x800)
 ROM_LOAD_16B ("sundance.r7", 0x1000, 0x800 )
ROM_END

ROM_START(spacewar)
ROM_REGION(0x10000,0)
 ROM_LOAD_16B("spacewar.1l", 0x0000, 0x800)
 ROM_LOAD_16B ("spacewar.2r", 0x0000, 0x800 )
ROM_END

ROM_START( wotw )
	ROM_REGION( 0x10000,0 )	/* 16k for code */
	ROM_LOAD_16B( "wow_le.t7", 0x0000, 0x1000)
	ROM_LOAD_16B ( "wow_lo.p7", 0x0000, 0x1000 )
	ROM_LOAD_16B( "wow_ue.u7", 0x2000, 0x1000)
	ROM_LOAD_16B ( "wow_uo.r7", 0x2000, 0x1000 )
ROM_END

ROM_START(starcas)
ROM_REGION(0x10000,0)
 ROM_LOAD_16B("starcas3.t7", 0x0000, 0x800)
 ROM_LOAD_16B ("starcas3.p7", 0x0000, 0x800 )
 ROM_LOAD_16B("starcas3.u7", 0x1000, 0x800)
 ROM_LOAD_16B ("starcas3.r7", 0x1000, 0x800 )
 ROM_END

ROM_START(tailg)
ROM_REGION(0x2000,0)
 ROM_LOAD_16B("tgunner.t70",  0x0000, 0x800)
 ROM_LOAD_16B ("tgunner.p70", 0x0000, 0x800 )
 ROM_LOAD_16B("tgunner.t71",  0x1000, 0x800)
 ROM_LOAD_16B ("tgunner.p71", 0x1000, 0x800 )
ROM_END

ROM_START(ripoff)
ROM_REGION(0x2000,0)
 ROM_LOAD_16B("ripoff.t7",  0x0000, 0x800)
 ROM_LOAD_16B ("ripoff.p7", 0x0000, 0x800 )
 ROM_LOAD_16B("ripoff.u7",  0x1000, 0x800)
 ROM_LOAD_16B ("ripoff.r7", 0x1000, 0x800 )
ROM_END

ROM_START(armora)
ROM_REGION(0x4000,0)
 ROM_LOAD_16B("ar414le.t6",  0x0000, 0x1000)
 ROM_LOAD_16B ("ar414lo.p6", 0x0000, 0x1000 )
 ROM_LOAD_16B("ar414ue.u6",  0x2000, 0x1000)
 ROM_LOAD_16B ("ar414uo.r6", 0x2000, 0x1000 )
ROM_END

ROM_START(warrior)
ROM_REGION(0x10000,0)
 ROM_LOAD_16B("warrior.t7", 0x0000, 0x800)
 ROM_LOAD_16B ("warrior.p7", 0x0000, 0x800 )
 ROM_LOAD_16B("warrior.u7", 0x1000, 0x800)
 ROM_LOAD_16B ("warrior.r7", 0x1000, 0x800 )
ROM_END

ROM_START(demon)
ROM_REGION(0x10000,0)
 ROM_LOAD_16B("demon.7t", 0x0000, 0x1000)
 ROM_LOAD_16B ("demon.7p", 0x0000, 0x1000)
 ROM_LOAD_16B("demon.7u", 0x2000, 0x1000)
 ROM_LOAD_16B ("demon.7r", 0x2000, 0x1000)
ROM_END

ROM_START(boxingb)
ROM_REGION(0x10000,0)
 ROM_LOAD_16B( "u1a", 0x0000, 0x1000)
 ROM_LOAD_16B ( "u1b", 0x0000, 0x1000)
 ROM_LOAD_16B( "u2a", 0x2000, 0x1000)
 ROM_LOAD_16B ( "u2b", 0x2000, 0x1000)
 ROM_LOAD_16B( "u3a", 0x4000, 0x1000)
 ROM_LOAD_16B ( "u3b", 0x4000, 0x1000)
 ROM_LOAD_16B( "u4a", 0x6000, 0x1000)
 ROM_LOAD_16B ( "u4b", 0x6000, 0x1000)
ROM_END

ROM_START(omegrace)
ROM_REGION(0x10000,0)
 ROM_LOAD("omega.m7", 0x0000, 0x1000)
 ROM_LOAD("omega.l7", 0x1000, 0x1000)
 ROM_LOAD("omega.k7", 0x2000, 0x1000)
 ROM_LOAD("omega.j7", 0x3000, 0x1000)
 ROM_LOAD("omega.e1", 0x9000, 0x0800)
 ROM_LOAD("omega.f1", 0x9800, 0x0800)
 ROM_REGION(0x10000,1)
 ROM_LOAD("sound.k5", 0x0000, 0x0800 )
ROM_END

ROM_START(deltrace)
ROM_REGION(0x10000,0)
 ROM_LOAD("omega.m7", 0x0000, 0x1000)
 ROM_LOAD("omega.l7", 0x1000, 0x1000)
 ROM_LOAD("omega.k7", 0x2000, 0x1000)
 ROM_LOAD("delta.j7", 0x3000, 0x1000)
 ROM_LOAD("omega.e1", 0x9000, 0x0800)
 ROM_LOAD("omega.f1", 0x9800, 0x0800)
 ROM_REGION(0x10000,1)
 ROM_LOAD("sound.k5", 0x0000, 0x0800 )
ROM_END


ROM_START( quantum )
 ROM_REGION(0x14000,0)
 ROM_LOAD_16B ("136016.201", 0x0000, 0x2000 )
 ROM_LOAD_16B ("136016.206", 0x0000, 0x2000 )
 ROM_LOAD_16B ("136016.102", 0x4000, 0x2000 )
 ROM_LOAD_16B ("136016.107", 0x4000, 0x2000 )
 ROM_LOAD_16B ("136016.203", 0x8000, 0x2000 )
 ROM_LOAD_16B ("136016.208", 0x8000, 0x2000 )
 ROM_LOAD_16B ("136016.104", 0xC000, 0x2000 )
 ROM_LOAD_16B ("136016.109", 0xC000, 0x2000 )
 ROM_LOAD_16B ("136016.105", 0x10000, 0x2000 )
 ROM_LOAD_16B ("136016.110", 0x10000, 0x2000 )
ROM_END

ROM_START( quantum1 )
ROM_REGION(0x14000,0)
 ROM_LOAD_16B("136016.101", 0x0000, 0x2000 )
 ROM_LOAD_16B ("136016.106", 0x0000, 0x2000 )
 ROM_LOAD_16B("136016.102", 0x4000, 0x2000 )
 ROM_LOAD_16B ("136016.107", 0x4000, 0x2000 )
 ROM_LOAD_16B("136016.103", 0x8000, 0x2000 )
 ROM_LOAD_16B ("136016.108", 0x8000, 0x2000 )
 ROM_LOAD_16B("136016.104", 0xC000, 0x2000 )
 ROM_LOAD_16B ("136016.109", 0xC000, 0x2000 )
 ROM_LOAD_16B("136016.105", 0x10000, 0x2000 )
 ROM_LOAD_16B ("136016.110", 0x10000, 0x2000 )
ROM_END

ROM_START( quantump )
 ROM_REGION(0x14000,0)
 ROM_LOAD_16B( "quantump.2e",  0x000000, 0x002000 )
 ROM_LOAD_16B ( "quantump.3e",  0x000000, 0x002000 )
 ROM_LOAD_16B( "quantump.2f",  0x004000, 0x002000 )
 ROM_LOAD_16B ( "quantump.3f",  0x004000, 0x002000 )
 ROM_LOAD_16B( "quantump.2h",  0x008000, 0x002000 )
 ROM_LOAD_16B ( "quantump.3h",  0x008000, 0x002000 )
 ROM_LOAD_16B( "quantump.2k",  0x00C000, 0x002000 )
 ROM_LOAD_16B ( "quantump.3k",  0x00C000, 0x002000 )
 ROM_LOAD_16B( "quantump.2l",  0x010000, 0x002000 )
 ROM_LOAD_16B ( "quantump.3l",  0x010000, 0x002000 )
ROM_END

ROM_START(llander)
 ROM_REGION(0x10000,0)
 ROM_LOAD("034599.01", 0x4800, 0x800)
 ROM_LOAD("034598.01", 0x5000, 0x800)
 ROM_LOAD("034597.01", 0x5800, 0x800)
 ROM_LOAD("034572.02", 0x6000, 0x800)
 ROM_LOAD("034571.02", 0x6800, 0x800)
 ROM_LOAD("034570.01", 0x7000, 0x800)
 ROM_LOAD("034569.02", 0x7800, 0x800)
 ROM_LOAD("034569.02", 0xf800, 0x800)
ROM_END

ROM_START(llander1)
 ROM_REGION(0x10000,0)
 ROM_LOAD("034599.01", 0x4800, 0x800)
 ROM_LOAD("034598.01", 0x5000, 0x800)
 ROM_LOAD("034597.01", 0x5800, 0x800)
 ROM_LOAD("034572.01", 0x6000, 0x800)
 ROM_LOAD("034571.01", 0x6800, 0x800)
 ROM_LOAD("034570.01", 0x7000, 0x800)
 ROM_LOAD("034569.01", 0x7800, 0x800)
 ROM_LOAD("034569.01", 0xf800, 0x800)
ROM_END

ROM_START(bzone2)
 ROM_REGION(0x10000,0)
 ROM_LOAD("036414a.01", 0x5000, 0x800)
 ROM_LOAD("036413.01", 0x5800, 0x800)
 ROM_LOAD("036412.01", 0x6000, 0x800)
 ROM_LOAD("036411.01", 0x6800, 0x800)
 ROM_LOAD("036410.01", 0x7000, 0x800)
 ROM_LOAD("036409.01", 0x7800, 0x800)
 ROM_LOAD("036409.01", 0xf800, 0x800)//reload
 ROM_LOAD("036422.01", 0x3000, 0x800)
 ROM_LOAD("036421.01", 0x3800, 0x800)
ROM_END

ROM_START(bzonec)
 ROM_REGION(0x10000,0)
 ROM_LOAD("bz1g4800", 0x4800, 0x800)
 ROM_LOAD("bz1f5000", 0x5000, 0x800)
 ROM_LOAD("bz1e5800", 0x5800, 0x800)
 ROM_LOAD("bz1d6000", 0x6000, 0x800)
 ROM_LOAD("bz1c6800", 0x6800, 0x800)
 ROM_LOAD("bz1b7000", 0x7000, 0x800)
 ROM_LOAD("bz1a7800", 0x7800, 0x800)//reload
 ROM_LOAD("bz1a7800", 0xf800, 0x800)//reload
 ROM_LOAD("036422.01",0x3000, 0x800)
 ROM_LOAD("bz3b3800", 0x3800, 0x800)
ROM_END

ROM_START(bzonep)
ROM_REGION(0x10000,0)
 ROM_LOAD("036414.01.bin", 0x5000, 0x800)
 ROM_LOAD("036413.01.bin", 0x5800, 0x800)
 ROM_LOAD("036412.01.bin", 0x6000, 0x800)
 ROM_LOAD("036411.01.bin", 0x6800, 0x800)
 ROM_LOAD("036410.01.bin", 0x7000, 0x800)
 ROM_LOAD("036409.01.bin", 0x7800, 0x800)
 ROM_LOAD("036408.01.bin", 0x8000, 0x800)
 ROM_LOAD("036408.01.bin", 0xf800, 0x800)//reload
 ROM_LOAD("036422.01.bin", 0x3000, 0x800)
 ROM_LOAD("036421.01.bin", 0x3800, 0x800)
ROM_END

ROM_START(redbaron)
ROM_REGION(0x10000,0)
 ROM_LOAD("037006.01e", 0x3000, 0x0800)
 ROM_LOAD("037007.01e", 0x3800, 0x0800)
 ROM_LOAD("037587.01",  0x4800, 0x1000)
 ROM_LOAD("037587.01",  0x5000, 0x1000)
 ROM_LOAD("037000.01e", 0x5000, 0x0800)
 ROM_LOAD("036998.01e", 0x6000, 0x0800)
 ROM_LOAD("036997.01e", 0x6800, 0x0800)  
 ROM_LOAD("036996.01e", 0x7000, 0x0800)
 ROM_LOAD("036995.01e", 0x7800, 0x0800)
 ROM_LOAD("036995.01e", 0xf800, 0x0800)
ROM_END

ROM_START( bradley )
	ROM_REGION(0x10000,0)
	// Mathbox ROMs 
	ROM_LOAD( "btb3.bin",   0x3000, 0x0800)
	ROM_LOAD( "bta3.bin",   0x3800, 0x0800)
	//Main Roms
	ROM_LOAD( "btc1.bin",   0x4000, 0x0800)
	ROM_LOAD( "btd1.bin",   0x4800, 0x0800)
	ROM_LOAD( "bte1.bin",   0x5000, 0x0800)
	ROM_LOAD( "bth1.bin",   0x5800, 0x0800)
	ROM_LOAD( "btj1.bin",   0x6000, 0x0800)
	ROM_LOAD( "btk1.bin",   0x6800, 0x0800)
	ROM_LOAD( "btlm.bin",   0x7000, 0x0800)
	ROM_LOAD( "btn1.bin",   0x7800, 0x0800)
	ROM_LOAD( "btn1.bin",   0xf800, 0x0800)
	
ROM_END


ROM_START(tempestm) //TEMPEST MENU
	ROM_REGION(0x90000,0)
	ROM_LOAD("menu_D1.bin", 0x9000, 0x0800)
	ROM_LOAD("menu_E1.bin", 0x9800, 0x0800)
	ROM_LOAD("menu_F1.bin", 0xa000, 0x0800)
	ROM_LOAD("136002.113",  0xa800, 0x0800)
	ROM_LOAD("136002.114",  0xb000, 0x0800) 
	ROM_LOAD("136002.115",  0xb800, 0x0800)
	ROM_LOAD("136002.116",  0xc000, 0x0800)
	ROM_LOAD("136002.117",  0xc800, 0x0800)
	ROM_LOAD("136002.118",  0xd000, 0x0800)
	ROM_LOAD("menu_R1.bin", 0xd800, 0x0800)
	ROM_RELOAD(               0xf800, 0x0800) //Reload
	//Vector Rom
	ROM_LOAD("menu_N3.bin", 0x3000, 0x0800)
	ROM_LOAD("menu_R3.bin", 0x3800, 0x0800)
    //ALIENS PROTO
	ROM_LOAD("alienst/aliens_d1.bin", 0x19000, 0x0800)
	ROM_LOAD("alienst/aliens_e1.bin", 0x19800, 0x0800)
	ROM_LOAD("alienst/aliens_f1.bin", 0x1a000, 0x0800)
	ROM_LOAD("alienst/aliens_h1.bin", 0x1a800, 0x0800)
	ROM_LOAD("alienst/aliens_j1.bin", 0x1b000, 0x0800)
	ROM_LOAD("alienst/aliens_k1.bin", 0x1b800, 0x0800)
	ROM_LOAD("alienst/aliens_l1.bin", 0x1c000, 0x0800)
	ROM_LOAD("alienst/aliens_n1.bin", 0x1c800, 0x0800)
	ROM_LOAD("alienst/aliens_p1.bin", 0x1d000, 0x0800)
	ROM_LOAD("alienst/aliens_r1.bin", 0x1d800, 0x0800)  
	ROM_RELOAD(                0x1f800, 0x0800) //Reload 
	ROM_LOAD("alienst/aliens_n3.bin", 0x13000, 0x0800)
	ROM_LOAD("alienst/aliens_r3.bin", 0x13800, 0x0800)
   
	//VECTOR BREAKOUT
	ROM_LOAD("vbreak/vb_d1.bin",  0x29000, 0x0800)
	ROM_LOAD("vbreak/vb_e1.bin",  0x29800, 0x0800)
	ROM_LOAD("vbreak/vb_f1.bin",  0x2a000, 0x0800)
	ROM_LOAD("vbreak/136002.113", 0x2a800, 0x0800)
	ROM_LOAD("vbreak/136002.114", 0x2b000, 0x0800)  
	ROM_LOAD("vbreak/136002.115", 0x2b800, 0x0800)
	ROM_LOAD("vbreak/136002.116", 0x2c000, 0x0800)
	ROM_LOAD("vbreak/136002.117", 0x2c800, 0x0800)
	ROM_LOAD("vbreak/136002.118", 0x2d000, 0x0800)
	ROM_LOAD("vbreak/vb_r1.bin",  0x2d800, 0x0800)  
	ROM_RELOAD(            0x2f800, 0x0800)//Reload   
	//Vector Rom
	ROM_LOAD("vbreak/vb_n3.bin",  0x23000, 0x0800)
	ROM_LOAD("vbreak/vb_r3.bin",  0x23800, 0x0800)
	
	 //VORTEX ROMSET
	ROM_LOAD("vortex/d1.bin",  0x39000, 0x0800)
	ROM_LOAD("vortex/e1.bin",  0x39800, 0x0800)
	ROM_LOAD("vortex/f1.bin",  0x3a000, 0x0800)
	ROM_LOAD("vortex/h1.bin",  0x3a800, 0x0800)
	ROM_LOAD("vortex/j1.bin",  0x3b000, 0x0800)  
	ROM_LOAD("vortex/k1.bin",  0x3b800, 0x0800)
	ROM_LOAD("vortex/lm1.bin", 0x3c000, 0x0800)
	ROM_LOAD("vortex/mn1.bin", 0x3c800, 0x0800)
	ROM_LOAD("vortex/p1.bin",  0x3d000, 0x0800)
	ROM_LOAD("vortex/r1.bin",  0x3d800, 0x0800)  
	ROM_RELOAD(         0x3f800, 0x0800)//Reload  
	//Vector Rom
	ROM_LOAD("vortex/n3.bin",  0x33000, 0x0800)
	ROM_LOAD("vortex/r3.bin",  0x33800, 0x0800)
	//TEMPEST TUBES
	ROM_LOAD( "temptube/136002-113.d1",   0x49000, 0x0800)
	ROM_LOAD( "temptube/136002-114.e1",   0x49800, 0x0800)
	ROM_LOAD( "temptube/136002-115.f1",   0x4a000, 0x0800)
	ROM_LOAD( "temptube/136002-316.h1",   0x4a800, 0x0800)
	ROM_LOAD( "temptube/136002-217.j1",   0x4b000, 0x0800)
	ROM_LOAD( "temptube/tube-118.k1",     0x4b800, 0x0800)
	ROM_LOAD( "temptube/136002-119.lm1",  0x4c000, 0x0800)
	ROM_LOAD( "temptube/136002-120.mn1",  0x4c800, 0x0800)
	ROM_LOAD( "temptube/136002-121.p1",   0x4d000, 0x0800)
	ROM_LOAD( "temptube/136002-222.r1",   0x4d800, 0x0800)
    ROM_RELOAD(                  0x4f800, 0x0800)//Reload 
    // Vector ROM 
    ROM_LOAD( "temptube/136002-123.np3",  0x43000, 0x0800)
    ROM_LOAD( "temptube/136002-124.r3",   0x43800, 0x0800)
/* rev 1 */
	ROM_LOAD( "tempest1/136002-113.d1",   0x59000, 0x0800)
	ROM_LOAD( "tempest1/136002-114.e1",   0x59800, 0x0800)
	ROM_LOAD( "tempest1/136002-115.f1",   0x5a000, 0x0800)
	ROM_LOAD( "tempest1/136002-116.h1",   0x5a800, 0x0800)
	ROM_LOAD( "tempest1/136002-117.j1",   0x5b000, 0x0800)
	ROM_LOAD( "tempest1/136002-118.k1",   0x5b800, 0x0800)
	ROM_LOAD( "tempest1/136002-119.lm1",  0x5c000, 0x0800)
	ROM_LOAD( "tempest1/136002-120.mn1",  0x5c800, 0x0800)
	ROM_LOAD( "tempest1/136002-121.p1",   0x5d000, 0x0800)
	ROM_LOAD( "tempest1/136002-122.r1",   0x5d800, 0x0800)
	ROM_RELOAD(                  0x5f800, 0x0800) //Reload 
	// Vector ROM 
	ROM_LOAD( "tempest1/136002-123.np3",  0x53000, 0x0800)
	ROM_LOAD( "tempest1/136002-124.r3",   0x53800, 0x0800)
 // rev 2 
	ROM_LOAD( "tempest2/136002-113.d1",   0x69000, 0x0800)
	ROM_LOAD( "tempest2/136002-114.e1",   0x69800, 0x0800)
	ROM_LOAD( "tempest2/136002-115.f1",   0x6a000, 0x0800)
	ROM_LOAD( "tempest2/136002-116.h1",   0x6a800, 0x0800)
	ROM_LOAD( "tempest2/136002-217.j1",   0x6b000, 0x0800)
	ROM_LOAD( "tempest2/136002-118.k1",   0x6b800, 0x0800)
	ROM_LOAD( "tempest2/136002-119.lm1",  0x6c000, 0x0800)
	ROM_LOAD( "tempest2/136002-120.mn1",  0x6c800, 0x0800)
	ROM_LOAD( "tempest2/136002-121.p1",   0x6d000, 0x0800)
	ROM_LOAD( "tempest2/136002-222.r1",   0x6d800, 0x0800)
	ROM_RELOAD(                  0x6f800, 0x0800)//Reload 
	ROM_LOAD( "tempest2/136002-123.np3",  0x63000, 0x0800)
	ROM_LOAD( "tempest2/136002-124.r3",   0x63800, 0x0800)
// rev 3 
	ROM_LOAD( "tempest/136002-113.d1",   0x79000, 0x0800)
	ROM_LOAD( "tempest/136002-114.e1",   0x79800, 0x0800)
	ROM_LOAD( "tempest/136002-115.f1",   0x7a000, 0x0800)
	ROM_LOAD( "tempest/136002-316.h1",   0x7a800, 0x0800)
	ROM_LOAD( "tempest/136002-217.j1",   0x7b000, 0x0800)
	ROM_LOAD( "tempest/136002-118.k1",   0x7b800, 0x0800)
	ROM_LOAD( "tempest/136002-119.lm1",  0x7c000, 0x0800)
	ROM_LOAD( "tempest/136002-120.mn1",  0x7c800, 0x0800)
	ROM_LOAD( "tempest/136002-121.p1",   0x7d000, 0x0800)
	ROM_LOAD( "tempest/136002-222.r1",   0x7d800, 0x0800)
	ROM_RELOAD(                  0x7f800, 0x0800)//Reload 
	ROM_LOAD( "tempest/136002-123.np3",  0x73000, 0x0800)
	ROM_LOAD( "tempest/136002-124.r3",   0x73800, 0x0800)
ROM_END

ROM_START(starwars)
    ROM_REGION(0x14000,0)
	ROM_LOAD( "136021.105",   0x3000, 0x1000 ) /* 3000-3fff is 4k vector rom */
	
	ROM_LOAD( "136021.102",   0x8000, 0x2000 ) /*  8k ROM 1 bank */
	ROM_LOAD( "136021.203",   0xa000, 0x2000 ) /*  8k ROM 2 bank */
	ROM_LOAD( "136021.104",   0xc000, 0x2000 ) /*  8k ROM 3 bank */
	ROM_LOAD( "136021.206",   0xe000, 0x2000 ) /*  8k ROM 4 bank */

	/* Load the Mathbox PROM's temporarily into the Vector RAM area */
	/* During initialisation they will be converted into useable form */
	/* and stored elsewhere. */
	ROM_LOAD( "136021.110",   0x0000, 0x0400 ) /* PROM 0 */
	ROM_LOAD( "136021.111",   0x0400, 0x0400 ) /* PROM 1 */
	ROM_LOAD( "136021.112",   0x0800, 0x0400 ) /* PROM 2 */
	ROM_LOAD( "136021.113",   0x0c00, 0x0400 ) /* PROM 3 */

    ROM_LOAD( "136021.214",   0x10000, 0x4000 )   /* ROM 0 bank pages 0 and 1 */

	/* Sound ROMS */
	//ROM_REGION( 0x10000, REGION_CPU2 )     /* Really only 32k, but it looks like 64K */
	ROM_REGION(0x10000,1)
	ROM_LOAD( "136021.107",   0x4000, 0x2000 ) /* Sound ROM 0 */
	ROM_LOAD( "136021.107",   0xc000, 0x2000 ) /* Copied again for */
	ROM_LOAD( "136021.208",   0x6000, 0x2000 ) /* Sound ROM 0 */
	ROM_LOAD( "136021.208",   0xe000, 0x2000 ) /* proper int vecs */
ROM_END

ROM_START(starwar1)
    ROM_REGION(0x14000,0)
	ROM_LOAD( "136021.105",   0x3000, 0x1000 ) /* 3000-3fff is 4k vector rom */
	
	ROM_LOAD( "136021.102",   0x8000, 0x2000 ) /*  8k ROM 1 bank */
	ROM_LOAD( "136021.203",   0xa000, 0x2000 ) /*  8k ROM 2 bank */
	ROM_LOAD( "136021.104",   0xc000, 0x2000 ) /*  8k ROM 3 bank */
	ROM_LOAD( "136021.206",   0xe000, 0x2000 ) /*  8k ROM 4 bank */

	/* Load the Mathbox PROM's temporarily into the Vector RAM area */
	/* During initialisation they will be converted into useable form */
	/* and stored elsewhere. */
	ROM_LOAD( "136021.110",   0x0000, 0x0400 ) /* PROM 0 */
	ROM_LOAD( "136021.111",   0x0400, 0x0400 ) /* PROM 1 */
	ROM_LOAD( "136021.112",   0x0800, 0x0400 ) /* PROM 2 */
	ROM_LOAD( "136021.113",   0x0c00, 0x0400 ) /* PROM 3 */

    ROM_LOAD( "136021.114",   0x10000, 0x4000 )   /* ROM 0 bank pages 0 and 1 */

	/* Sound ROMS */
	ROM_REGION(0x10000,1)      /* Really only 32k, but it looks like 64K */
	ROM_LOAD( "136021.107",   0x4000, 0x2000 ) /* Sound ROM 0 */
	ROM_LOAD( "136021.107",   0xc000, 0x2000 ) /* Copied again for */
	ROM_LOAD( "136021.208",   0x6000, 0x2000 ) /* Sound ROM 0 */
	ROM_LOAD( "136021.208",   0xe000, 0x2000 ) /* proper int vecs */
ROM_END

ROM_START(galaga)
    ROM_REGION(0x10000,CPU0)  //Main CPU Code
	ROM_LOAD( "gg1-1b.3p", 0x0000, 0x1000 )
	ROM_LOAD( "gg1-2b.3m", 0x1000, 0x1000 )
	ROM_LOAD( "gg1-3.2m",  0x2000, 0x1000 )
	ROM_LOAD( "gg1-4b.2l", 0x3000, 0x1000 )

	ROM_REGION(0x10000,CPU1)     //Video CPU   
	ROM_LOAD( "gg1-5b.3f", 0x0000, 0x1000 )

	ROM_REGION(0x10000,CPU2)     // Sound CPU  
	ROM_LOAD( "gg1-7b.2c", 0x0000, 0x1000 )

	ROM_REGION(0x10000,CPU3)     // temporary space for graphics (disposed after conversion) 
	ROM_LOAD( "gg1-9.4l",  0x0000, 0x1000 ) //Chars
	ROM_LOAD( "gg1-11.4d", 0x1000, 0x1000 ) //Sprites
	ROM_LOAD( "gg1-10.4f", 0x2000, 0x1000 ) //Sprites
	
	ROM_LOAD( "prom-5.5n", 0x3000, 0x20 )  //PROM0
	ROM_LOAD( "prom-4.2n", 0x3020, 0x100 ) //PROM1
	ROM_LOAD( "prom-3.1c", 0x3120, 0x100 ) //PROM1
   
	ROM_LOAD( "prom-1.1d", 0x4000, 0x100 ) //SND1
	ROM_LOAD( "prom-2.5c", 0x4100, 0x100 ) //SND2

ROM_END


/*
ROM_START( esb )
	ROM_REGION( 0x22000, 0 )     
	ROM_LOAD( "136031.111",   0x03000, 0x1000 )    // 3000-3fff is 4k vector rom 
	ROM_LOAD( "136031.101",   0x06000, 0x2000 )
	ROM_CONTINUE(             0x10000, 0x2000 )
	// $8000 - $9fff : slapstic page 
	ROM_LOAD( "136031.102",   0x0a000, 0x2000 )
	ROM_CONTINUE(             0x1c000, 0x2000 )
	ROM_LOAD( "136031.203",   0x0c000, 0x2000 )
	ROM_CONTINUE(             0x1e000, 0x2000 )
	ROM_LOAD( "136031.104",   0x0e000, 0x2000 )
	ROM_CONTINUE(             0x20000, 0x2000 )

	ROM_LOAD( "136031.105",   0x14000, 0x4000 ) // slapstic 0, 1 
	ROM_LOAD( "136031.106",   0x18000, 0x4000 ) // slapstic 2, 3 

    // Load the Mathbox PROM's temporarily into the Vector RAM area 
	// During initialisation they will be converted into useable form 
	// and stored elsewhere. 
	ROM_LOAD( "136021.110",   0x0000, 0x0400 ) // PROM 0 
	ROM_LOAD( "136021.111",   0x0400, 0x0400 ) // PROM 1 
	ROM_LOAD( "136021.112",   0x0800, 0x0400 ) // PROM 2 
	ROM_LOAD( "136021.113",   0x0c00, 0x0400, ROM_LOAD_NORMAL ) // PROM 3 

	// Sound ROMS 
	ROM_REGION( 0x10000, 1 )
	ROM_LOAD( "136031.113",   0x4000, 0x2000,ROM_LOAD_2ND ) // Sound ROM 0 
	ROM_CONTINUE(             0xc000, 0x2000 ) // Copied again for 
	ROM_LOAD( "136031.112",   0x6000, 0x2000 ) // Sound ROM 1 
	ROM_CONTINUE(             0xe000, 0x2000 ) // proper int vecs 

ROM_END
*/

ROM_START( aztarac )
	ROM_REGION(0xc000,0)
	ROM_LOAD_16B( "l8_6.bin", 0x000000, 0x001000 )
	ROM_LOAD_16B ( "n8_0.bin", 0x000001, 0x001000 )
	ROM_LOAD_16B( "l7_7.bin", 0x002000, 0x001000 )
	ROM_LOAD_16B ( "n7_1.bin", 0x002001, 0x001000 )
	ROM_LOAD_16B( "l6_8.bin", 0x004000, 0x001000 )
	ROM_LOAD_16B ( "n6_2.bin", 0x004001, 0x001000 )
	ROM_LOAD_16B( "l5_9.bin", 0x006000, 0x001000 )
	ROM_LOAD_16B ( "n5_3.bin", 0x006001, 0x001000 )
	ROM_LOAD_16B( "l4_a.bin", 0x008000, 0x001000 )
	ROM_LOAD_16B ( "n4_4.bin", 0x008001, 0x001000 )
	ROM_LOAD_16B( "l3_b.bin", 0x00a000, 0x001000)
	ROM_LOAD_16B ( "n3_5.bin", 0x00a001, 0x001000 )

	ROM_REGION(0x10000,1)
	ROM_LOAD_16B( "j4_c.bin", 0x0000, 0x1000 )
	ROM_LOAD_16B ( "j3_d.bin", 0x1000, 0x1000 )
ROM_END

#endif