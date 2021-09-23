#ifndef GAMESAMP_H
#define GAMESAMP_H

#include "globals.h"

static char *nosamples[]={"nosamples.zip","test.zip","one.zip","NULL"};
static char *omega_samples[]={"omegrace.zip","0.wav","1.wav","2.wav","3.wav","4.wav","5.wav","6.wav","7.wav",
                               "8.wav","9.wav","a.wav","b.wav","c.wav","d.wav","e.wav","f.wav",
							   "10.wav","11.wav","12.wav","13.wav","14.wav","15.wav","16.wav",
							   "NULL"};


//static char *guisamples[]={"aae.zip","error.wav","hiss.wav","psnoise.wav","flyback.wav","opening.wav","key.wav","idle.wav","explode1.wav","fire.wav","NULL"};
static char *guisamples[]={"aae.zip","error.wav","opening.wav","key.wav","idle.wav","explode1.wav","fire.wav","NULL"};
static char *asteroidsamples[]={"asteroid.zip","fire.wav","sfire.wav","thrust.wav","explode1.wav","explode2.wav",
"explode3.wav","thumphi.wav","thumplo.wav","ssaucer.wav","lsaucer.wav","life.wav","NULL"};
static char *deluxesamples[]={"astdelux.zip","thrust.wav","explode1.wav","explode2.wav","explode3.wav","NULL"};
static char *bzonesamples[]={"bzone.zip","fire1.wav","fire2.wav","engine1.wav","engine2.wav","explode1.wav","explode2.wav","NULL"};

static char *redbaron_samples[]={"redbaron.zip","explode.wav","shot.wav","spin.wav","NULL"};
static char *llander_samples[]={"llander.zip","lthrust.wav","beep.wav","lexplode.wav","lander6k.wav","NULL"};
static char *zektor_samples[]={
	"zektor.zip",
	"zk01.wav",  // 1 
	"zk02.wav",
	"zk03.wav",
	"zk04.wav",
	"zk05.wav",
	"zk06.wav",
	"zk07.wav",
	"zk08.wav",
	"zk09.wav",
	"zk0a.wav",
	"zk0b.wav",
	"zk0c.wav",
	"zk0d.wav",
	"zk0e.wav",
	"zk0f.wav",
	"zk10.wav",
	"zk11.wav",
	"zk12.wav",
	"zk13.wav",
	"elim1.wav",  // 19 fireball 
	"elim2.wav",  // 20 bounce 
	"elim3.wav",  // 21 Skitter 
	"elim4.wav",  // 22 Eliminator 
	"elim5.wav",  // 23 Electron 
	"elim6.wav",  // 24 fire 
	"elim7.wav",  // 25 thrust 
	"elim8.wav",  // 26 Electron 
	"elim9.wav",  // 27 small explosion 
	"elim10.wav", // 28 med explosion 
	"elim11.wav", // 29 big explosion 
	"zizzer.wav", // Zizzer 
	"ping.wav",   // PING 
	"cityfly.wav",// City fly by 
	"rrings.wav", // Rotation Rings 
	  "NULL"
};

static char *tacscan_samples[] =
{
	"tacscan.zip",
	// Player ship thrust sounds 
	"01.wav",
	"02.wav",
	"03.wav",
	"plaser.wav",
	"pexpl.wav",
	"pship.wav",
	"tunnelh.wav",
	"sthrust.wav",
	"slaser.wav",
	"sexpl.wav",
	"eshot.wav",
	"eexpl.wav",
	"tunnelw.wav",
	"flight1.wav",
	"flight2.wav",
	"flight3.wav",
	"flight4.wav",
	"flight5.wav",
	"formatn.wav",
	"warp.wav",
	"credit.wav",
	"1up.wav",
    "NULL"
};

static char *startrek_samples[] =
{   "startrek.zip",
	"st01.wav",
	"st02.wav",
	"st03.wav",
	"st04.wav",
	"st05.wav",
	"st06.wav",
	"st07.wav",
	"st08.wav",
	"st09.wav",
	"st0a.wav",
	"st0b.wav",
	"st0c.wav",
	"st0d.wav",
	"st0e.wav",
	"st0f.wav",
	"st10.wav",
	"st11.wav",
	"st12.wav",
	"st13.wav",
	"st14.wav",
	"st15.wav",
	"st16.wav",
	"st17.wav",
	// Sound samples 
	"trek1.wav",
	"trek2.wav",
	"trek3.wav",
	"trek4.wav",
	"trek5.wav",
	"trek6.wav",
	"trek7.wav",
	"trek8.wav",
	"trek9.wav",
	"trek10.wav",
	"trek11.wav",
	"trek12.wav",
	"trek13.wav",
	"trek14.wav",
	"trek15.wav",
	"trek16.wav",
	"trek17.wav",
	"trek18.wav",
	"trek19.wav",
	"trek20.wav",
	"trek21.wav",
	"trek22.wav",
	"trek23.wav",
	"trek24.wav",
	"trek25.wav",
	"trek26.wav",
	"trek27.wav",
	"trek28.wav",
	"NULL"
};

static char *elim_samples[]={
	"elim2p.zip",
	"elim1.wav",  // 19 fireball 
	"elim2.wav",  // 20 bounce 
	"elim3.wav",  // 21 Skitter 
	"elim4.wav",  // 22 Eliminator 
	"elim5.wav",  // 23 Electron 
	"elim6.wav",  // 24 fire 
	"elim7.wav",  // 25 thrust 
	"elim8.wav",  // 26 Electron 
	"elim9.wav",  // 27 small explosion 
	"elim10.wav", // 28 med explosion 
	"elim11.wav", // 29 big explosion 
    "elim12.wav", // 29 big explosion 
	"NULL"
};

static char *spacfury_samples[]={
    "spacfury.zip",
	"SF01.WAV",
    "SF02.WAV",
	"SF03.WAV",
	"SF04.WAV",
	"SF05.WAV",
	"SF06.WAV",
	"SF07.WAV",
	"SF08.WAV",
	"SF09.WAV",
	"SF0A.WAV",
	"SF0B.WAV",
	"SF0C.WAV",
	"SF0D.WAV",
	"SF0E.WAV",
	"SF0F.WAV",
	"SF10.WAV",
	"SF11.WAV",
	"SF12.WAV",
	"SF13.WAV",
	"SF14.WAV",
	"SF15.WAV",
	"sfury1.wav",
	"sfury2.wav",
	"sfury3.wav",
	"sfury4.wav",
	"sfury5.wav",
	"sfury6.wav",
	"sfury7.wav",
	"sfury8.wav",
	"sfury9.wav",
	"sfury10.wav",
	"NULL"
	};

///SAMPLE DATA BECAUSE I AM NOT A GOOD ENOUGH PROGRAMMER
static char *ripoff_samples[] =
{"ripoff.zip","efire.wav","eattack.wav","bonuslvl.wav","explosn.wav","shipfire.wav","bg1.wav","bg2.wav",
 "bg3.wav","bg4.wav","bg5.wav","bg6.wav","bg7.wav","bg8.wav","NULL"};
static char  *armora_samples[] =   
{"armora.zip","loexp.wav","jeepfire.wav","hiexp.wav","tankfire.wav","tankeng.wav","beep.wav","chopper.wav","NULL"};
static char  *starcas_samples[] =
{"starcas.zip","lexplode.wav","sexplode.wav","cfire.wav","pfire.wav","drone.wav","shield.wav","star.wav","thrust.wav","NULL"};
static char  *solarq_samples[] =
{"solarq.zip","bigexpl.wav","smexpl.wav","lthrust.wav","slaser.wav","pickup.wav","nuke1.wav","nuke2.wav","hypersp.wav",
 "extra.wav","phase.wav","efire.wav","NULL"};
static char  *spacewar_samples[] =
{"spacewar.zip","explode1.wav","fire1.wav","idle.wav","thrust1.wav","thrust2.wav","pop.wav","explode2.wav","fire2.wav","NULL"};
static char  *warrior_samples[] =
{"warrior.zip","appear.wav","bgmhum1.wav","bgmhum2.wav","fall.wav","killed.wav","NULL"};
static char  *tailg_samples[] =
{"tailg.zip","hypersp.wav","sexplode.wav","slaser.wav","shield.wav","bounce.wav","thrust1.wav","NULL"};
static char  *barrier_samples[] =
{"barrier.zip","playrdie.wav","playmove.wav","enemmove.wav","NULL"};
static char  *starhawk_samples[] =
{"starhawk.zip","explode.wav","rlaser.wav","llaser.wav","k.wav","master.wav","kexit.wav","NULL"};
static char  *boxingb_samples[] =
{"starhawk.zip","explode.wav","rlaser.wav","llaser.wav","k.wav","master.wav","kexit.wav","NULL"};
static char  *speedfrk_samples[] =
{"starhawk.zip","explode.wav","rlaser.wav","llaser.wav","k.wav","master.wav","kexit.wav","NULL"};
static char  *demon_samples[] =
{"starhawk.zip","explode.wav","rlaser.wav","llaser.wav","k.wav","master.wav","kexit.wav","NULL"};
static char  *sundance_samples[] =
{"sundance.zip","bong.wav","whoosh.wav","explsion.wav","ping1.wav","ping2.wav","hatch.wav","NULL",};


#endif