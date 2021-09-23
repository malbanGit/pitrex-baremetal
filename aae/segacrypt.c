#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif


#include "z80/z80.h"

#include "SegaG80.h"
#include "cpu_control.h"
#include "globals.h"
#include "log.h"
 
static void sega_decrypt64(int pc,unsigned int *lo)
{
	unsigned int i = 0;
	unsigned int b = *lo;

	switch (pc & 0x03)
	{
		case 0x00:
			/* A */
			i=b;
			break;
		case 0x01:
			/* B */
			i=b & 0x03;
			i+=((b    & 0x80) >> 1);
			i+=((b    & 0x60) >> 3);
			i+=((~b) & 0x10);
			i+=((b    & 0x08) << 2);
			i+=((b    & 0x04) << 5);
			i &= 0xFF;
			break;
		case 0x02:
			/* C */
			i=b & 0x03;
			i+=((b    & 0x80) >> 4);
			i+=(((~b) & 0x40) >> 1);
			i+=((b    & 0x20) >> 1);
			i+=((b    & 0x10) >> 2);
			i+=((b    & 0x08) << 3);
			i+=((b    & 0x04) << 5);
			i &= 0xFF;
			break;
		case 0x03:
			/* D */
			i=b & 0x23;
			i+=((b    & 0xC0) >> 4);
			i+=((b    & 0x10) << 2);
			i+=((b    & 0x08) << 1);
			i+=(((~b) & 0x04) << 5);
			i &= 0xFF;
			break;
		default: i=b;log_it("OH NO______________________");break;
	}

	*lo=i;
}
/****************************************************************************/
/* MB 971025 - Emulate Sega G80 security chip 315-0070                      */
/****************************************************************************/
static void sega_decrypt70(int pc,unsigned int *lo)
{
	unsigned int i = 0;
	unsigned int b = *lo;

	switch (pc & 0x09)
	{
		case 0x00:
			/* B */
			i=b & 0x03;
			i+=((b    & 0x80) >> 1);
			i+=((b    & 0x60) >> 3);
			i+=((~b) & 0x10);
			i+=((b    & 0x08) << 2);
			i+=((b    & 0x04) << 5);
			i &= 0xFF;
			break;
		case 0x01:
			/* A */
			i=b;
			break;
		case 0x08:
			/* D */
			i=b & 0x23;
			i+=((b    & 0xC0) >> 4);
			i+=((b    & 0x10) << 2);
			i+=((b    & 0x08) << 1);
			i+=(((~b) & 0x04) << 5);
			i &= 0xFF;
			break;
		case 0x09:
			/* C */
			i=b & 0x03;
			i+=((b    & 0x80) >> 4);
			i+=(((~b) & 0x40) >> 1);
			i+=((b    & 0x20) >> 1);
			i+=((b    & 0x10) >> 2);
			i+=((b    & 0x08) << 3);
			i+=((b    & 0x04) << 5);
			i &= 0xFF;
			break;
	}

	*lo=i;
}

/****************************************************************************/
/* MB 971025 - Emulate Sega G80 security chip 315-0076                      */
/****************************************************************************/
static void sega_decrypt76(int pc,unsigned int *lo)
{
	unsigned int i = 0;
	unsigned int b = *lo;

	switch (pc & 0x09)
	{
		case 0x00:
			/* A */
			i=b;
			break;
		case 0x01:
			/* B */
			i=b & 0x03;
			i+=((b    & 0x80) >> 1);
			i+=((b    & 0x60) >> 3);
			i+=((~b) & 0x10);
			i+=((b    & 0x08) << 2);
			i+=((b    & 0x04) << 5);
			i &= 0xFF;
			break;
		case 0x08:
			/* C */
			i=b & 0x03;
			i+=((b    & 0x80) >> 4);
			i+=(((~b) & 0x40) >> 1);
			i+=((b    & 0x20) >> 1);
			i+=((b    & 0x10) >> 2);
			i+=((b    & 0x08) << 3);
			i+=((b    & 0x04) << 5);
			i &= 0xFF;
			break;
		case 0x09:
			/* D */
			i=b & 0x23;
			i+=((b    & 0xC0) >> 4);
			i+=((b    & 0x10) << 2);
			i+=((b    & 0x08) << 1);
			i+=(((~b) & 0x04) << 5);
			i &= 0xFF;
			break;
	}

	*lo=i;
}

/****************************************************************************/
/* MB 971025 - Emulate Sega G80 security chip 315-0082                      */
/****************************************************************************/
static void sega_decrypt82(int pc,unsigned int *lo)
{
	unsigned int i = 0;
	unsigned int b = *lo;

	switch (pc & 0x11)
	{
		case 0x00:
			/* A */
			i=b;
			break;
		case 0x01:
			/* B */
			i=b & 0x03;
			i+=((b    & 0x80) >> 1);
			i+=((b    & 0x60) >> 3);
			i+=((~b) & 0x10);
			i+=((b    & 0x08) << 2);
			i+=((b    & 0x04) << 5);
			i &= 0xFF;
			break;
		case 0x10:
			/* C */
			i=b & 0x03;
			i+=((b    & 0x80) >> 4);
			i+=(((~b) & 0x40) >> 1);
			i+=((b    & 0x20) >> 1);
			i+=((b    & 0x10) >> 2);
			i+=((b    & 0x08) << 3);
			i+=((b    & 0x04) << 5);
			i &= 0xFF;
			break;
		case 0x11:
			/* D */
			i=b & 0x23;
			i+=((b    & 0xC0) >> 4);
			i+=((b    & 0x10) << 2);
			i+=((b    & 0x08) << 1);
			i+=(((~b) & 0x04) << 5);
			i &= 0xFF;
			break;
	}

	*lo=i;
}

/****************************************************************************/
/* MB 971031 - Emulate no Sega G80 security chip                            */
/****************************************************************************/
static void sega_decrypt0(int pc,unsigned int *lo)
{     

	unsigned int i = 0;
	unsigned int b = *lo;
    
	*lo=i;
	
}


void sega_security(int chip)
{
	switch (chip)
	{
		//case 62:
			//sega_decrypt=sega_decrypt62;
			//break;
		//case 63:
			//sega_decrypt=sega_decrypt63;
			//break;
		case 64:
			sega_decrypt=sega_decrypt64;
			break;
		case 70:
			sega_decrypt=sega_decrypt70;
			break;
		case 76:
			sega_decrypt=sega_decrypt76;
			break;
		case 82:
			sega_decrypt=sega_decrypt82;
			break;
		default:
			sega_decrypt=sega_decrypt64;
			break;
	}
}

void Decrypt(int gamenum)
{//960a
switch (gamenum){
	
case 6:
cMZ80[0].z80Base[0x443b]=0x21;
cMZ80[0].z80Base[0xa3e]=0x21;
cMZ80[0].z80Base[0xa41]=0x31;
cMZ80[0].z80Base[0xa52]=0x20;
cMZ80[0].z80Base[0x32e7]=0x23;
cMZ80[0].z80Base[0x32ea]=0x24;
cMZ80[0].z80Base[0x32f2]=0x25;
cMZ80[0].z80Base[0x32f5]=0x28;
cMZ80[0].z80Base[0xb7a]=0x40;
cMZ80[0].z80Base[0xb7d]=0x3a;
cMZ80[0].z80Base[0xbb2]=0x1c;
cMZ80[0].z80Base[0xbb5]=0x1d;
cMZ80[0].z80Base[0xbb8]=0x1e;
cMZ80[0].z80Base[0x4423]=0x21;
cMZ80[0].z80Base[0x4495]=0x21;
cMZ80[0].z80Base[0x44bb]=0x21;
cMZ80[0].z80Base[0xbd2]=0x36;
cMZ80[0].z80Base[0xbd5]=0x37;
cMZ80[0].z80Base[0x1fa4]=0x0;
cMZ80[0].z80Base[0x2c44]=0x15;
cMZ80[0].z80Base[0x1fc9]=0x0;
cMZ80[0].z80Base[0xb69]=0x20;
cMZ80[0].z80Base[0x443b]=0x21;
cMZ80[0].z80Base[0xb88]=0x21;
cMZ80[0].z80Base[0xb8b]=0x22;
cMZ80[0].z80Base[0xa90]=0x20;
cMZ80[0].z80Base[0xc46]=0x24;
cMZ80[0].z80Base[0xc4a]=0x23;
cMZ80[0].z80Base[0x334c]=0x28;
cMZ80[0].z80Base[0x3366]=0x2e;
cMZ80[0].z80Base[0x336b]=0x24;
cMZ80[0].z80Base[0x331c]=0x2e;
cMZ80[0].z80Base[0x3324]=0x2e;
cMZ80[0].z80Base[0x343c]=0x29;
cMZ80[0].z80Base[0x343f]=0x25;
cMZ80[0].z80Base[0x347c]=0x24;
cMZ80[0].z80Base[0x342d]=0x29;
cMZ80[0].z80Base[0x326d]=0xc;
cMZ80[0].z80Base[0x327d]=0xc;
cMZ80[0].z80Base[0x4031]=0x20;
cMZ80[0].z80Base[0x348c]=0x34;
cMZ80[0].z80Base[0x3490]=0x23;
cMZ80[0].z80Base[0x3493]=0x24;
cMZ80[0].z80Base[0x34ac]=0x23;
cMZ80[0].z80Base[0x34b6]=0x34;
cMZ80[0].z80Base[0x34f0]=0x2e;
cMZ80[0].z80Base[0x34f5]=0x24;
cMZ80[0].z80Base[0x3510]=0x2e;
cMZ80[0].z80Base[0x3517]=0x2e;
cMZ80[0].z80Base[0x3529]=0x24;
cMZ80[0].z80Base[0x352f]=0x23;
cMZ80[0].z80Base[0x3532]=0x24;
cMZ80[0].z80Base[0x213e]=0x31;
cMZ80[0].z80Base[0x217d]=0x21;
break;

case 5:{//SPACEFURY

cMZ80[0].z80Base[0x824]=0xaf; 
cMZ80[0].z80Base[0x825]=0xdd;
cMZ80[0].z80Base[0x826]=0xb2;
cMZ80[0].z80Base[0x827]=0xab;
cMZ80[0].z80Base[0x834]=0xd7;
cMZ80[0].z80Base[0x835]=0xc7;
cMZ80[0].z80Base[0x839]=0xe6;
cMZ80[0].z80Base[0x83a]=0x9f;
cMZ80[0].z80Base[0x83b]=0x66;
cMZ80[0].z80Base[0x83c]=0x93;
cMZ80[0].z80Base[0x847]=0xc4;
cMZ80[0].z80Base[0x848]=0xfd;
cMZ80[0].z80Base[0x849]=0xeb;
cMZ80[0].z80Base[0x84a]=0xf9;
cMZ80[0].z80Base[0x84e]=0xaa;
cMZ80[0].z80Base[0x84f]=0x45;
cMZ80[0].z80Base[0x857]=0x24;
cMZ80[0].z80Base[0x858]=0x03;

cMZ80[0].z80Base[0xa3c]=0x21;
cMZ80[0].z80Base[0xa3f]=0x31;
cMZ80[0].z80Base[0xa50]=0x20;
cMZ80[0].z80Base[0xa8e]=0x20;
cMZ80[0].z80Base[0xb67]=0x20;
cMZ80[0].z80Base[0xb78]=0x40;
cMZ80[0].z80Base[0xb7b]=0x3a;
cMZ80[0].z80Base[0xb86]=0x21;
cMZ80[0].z80Base[0xbb0]=0x1c;
cMZ80[0].z80Base[0xbb3]=0x1d;
cMZ80[0].z80Base[0xbb6]=0x1e;
cMZ80[0].z80Base[0xbd0]=0x36;
cMZ80[0].z80Base[0xbd3]=0x37;
cMZ80[0].z80Base[0xc44]=0x24;
cMZ80[0].z80Base[0xc48]=0x23;
//962a
cMZ80[0].z80Base[0x1fa2]=0x00;
cMZ80[0].z80Base[0x1fc7]=0x00;
//963a

cMZ80[0].z80Base[0x213c]=0x31;
cMZ80[0].z80Base[0x2147]=0x31;
cMZ80[0].z80Base[0x216b]=0x21;
cMZ80[0].z80Base[0x217b]=0x21;
cMZ80[0].z80Base[0x21d0]=0x20;
cMZ80[0].z80Base[0x2672]=0x34;
cMZ80[0].z80Base[0x2677]=0x35;
cMZ80[0].z80Base[0x2687]=0x34;
//965
cMZ80[0].z80Base[0x326a]=0x0c;
cMZ80[0].z80Base[0x327a]=0x0c;
cMZ80[0].z80Base[0x32e4]=0x23;
cMZ80[0].z80Base[0x32e7]=0x24;
cMZ80[0].z80Base[0x32ef]=0x25;
cMZ80[0].z80Base[0x32f2]=0x28;
cMZ80[0].z80Base[0x3363]=0x2e;
cMZ80[0].z80Base[0x3368]=0x24;
cMZ80[0].z80Base[0x342a]=0x29;
cMZ80[0].z80Base[0x343c]=0x25;
cMZ80[0].z80Base[0x3490]=0x24;
cMZ80[0].z80Base[0x34b3]=0x34;
cMZ80[0].z80Base[0x34f2]=0x24;
cMZ80[0].z80Base[0x3514]=0x2e;
cMZ80[0].z80Base[0x3526]=0x24;
cMZ80[0].z80Base[0x352c]=0x23;
cMZ80[0].z80Base[0x352f]=0x24;
//966
cMZ80[0].z80Base[0x39a8]=0xf8;
cMZ80[0].z80Base[0x39ab]=0xf9;
cMZ80[0].z80Base[0x39ae]=0xfa;
//967
cMZ80[0].z80Base[0x4412]=0x21;
cMZ80[0].z80Base[0x4423]=0x21;
cMZ80[0].z80Base[0x443b]=0x21;
cMZ80[0].z80Base[0x44bb]=0x21;

break;
				  
				  }

				   case 0:{//SPACEFURY




cMZ80[0].z80Base[0xa5f]=0x21;
cMZ80[0].z80Base[0xa62]=0x31;
cMZ80[0].z80Base[0xa73]=0x20;
cMZ80[0].z80Base[0xb8a]=0x20;

cMZ80[0].z80Base[0xba5]=0x3a;
cMZ80[0].z80Base[0xbda]=0x1c;
cMZ80[0].z80Base[0xbdd]=0x1d;
cMZ80[0].z80Base[0xbfd]=0x37;


cMZ80[0].z80Base[0xbb0]=0x21;
cMZ80[0].z80Base[0xbb3]=0x22;
cMZ80[0].z80Base[0xbda]=0x1c;
cMZ80[0].z80Base[0xbe0]=0x1e;
cMZ80[0].z80Base[0xbfa]=0x36;
cMZ80[0].z80Base[0xc86]=0x24;
cMZ80[0].z80Base[0xc8a]=0x23;
cMZ80[0].z80Base[0x1fc0]=0x0;

cMZ80[0].z80Base[0x215a]=0x31;
cMZ80[0].z80Base[0x21a7]=0x21;
cMZ80[0].z80Base[0x21b7]=0x21;
cMZ80[0].z80Base[0x26ae]=0x34;
cMZ80[0].z80Base[0x26b3]=0x35;
cMZ80[0].z80Base[0x26c3]=0x34;
cMZ80[0].z80Base[0x2c7d]=0x15;
cMZ80[0].z80Base[0x32bb]=0xc;
cMZ80[0].z80Base[0x32cb]=0xc;
cMZ80[0].z80Base[0x3335]=0x23;

cMZ80[0].z80Base[0x3338]=0x24;
cMZ80[0].z80Base[0x3340]=0x25;
cMZ80[0].z80Base[0x3343]=0x28;
cMZ80[0].z80Base[0x339a]=0x28;
cMZ80[0].z80Base[0x33b4]=0x2e;
cMZ80[0].z80Base[0x336a]=0x2e;
cMZ80[0].z80Base[0x3372]=0x2e;
cMZ80[0].z80Base[0x348a]=0x29;
cMZ80[0].z80Base[0x34ca]=0x24;
cMZ80[0].z80Base[0x347b]=0x29;
cMZ80[0].z80Base[0x34da]=0x34;
cMZ80[0].z80Base[0x34de]=0x23;
cMZ80[0].z80Base[0x34fa]=0x23;
cMZ80[0].z80Base[0x3504]=0x34;
cMZ80[0].z80Base[0x353e]=0x2e;
cMZ80[0].z80Base[0x3543]=0x24;
cMZ80[0].z80Base[0x355e]=0x2e;
cMZ80[0].z80Base[0x3577]=0x24;
cMZ80[0].z80Base[0x3580]=0x24;

cMZ80[0].z80Base[0x4024]=0x20;
cMZ80[0].z80Base[0x4412]=0x21;
cMZ80[0].z80Base[0x4484]=0x21;
cMZ80[0].z80Base[0x44aa]=0x21;
cMZ80[0].z80Base[0x442a]=0x21;

break;}

				   case 2:{ //zektor
cMZ80[0].z80Base[0x9c90]=0x21;
cMZ80[0].z80Base[0xcbb]=0x4c;
cMZ80[0].z80Base[0xccc]=0x20;
cMZ80[0].z80Base[0x8ff1]=0x34;
cMZ80[0].z80Base[0x8ff4]=0x35;
cMZ80[0].z80Base[0xe3a]=0x51;
cMZ80[0].z80Base[0xe72]=0x31;
cMZ80[0].z80Base[0xe75]=0x32;
cMZ80[0].z80Base[0xe78]=0x33;
cMZ80[0].z80Base[0x8e1b]=0x5;
cMZ80[0].z80Base[0xdb9]=0x1;
cMZ80[0].z80Base[0x9c78]=0x21;
cMZ80[0].z80Base[0x9cea]=0x21;
cMZ80[0].z80Base[0x9d10]=0x21;
cMZ80[0].z80Base[0x6b5b]=0x0;
cMZ80[0].z80Base[0x6b77]=0x0;
cMZ80[0].z80Base[0x79bc]=0x4;
cMZ80[0].z80Base[0xf8a]=0x35;
cMZ80[0].z80Base[0xf8e]=0x34;
cMZ80[0].z80Base[0x9055]=0x38;
cMZ80[0].z80Base[0x905a]=0x35;
cMZ80[0].z80Base[0x9019]=0x38;
cMZ80[0].z80Base[0x907d]=0x35;
cMZ80[0].z80Base[0x8df0]=0x5;
cMZ80[0].z80Base[0x8e00]=0x5;
cMZ80[0].z80Base[0x890c]=0x4b;
cMZ80[0].z80Base[0x8948]=0x2;
cMZ80[0].z80Base[0x8ed7]=0x23;
cMZ80[0].z80Base[0x7010]=0x23;
cMZ80[0].z80Base[0x6d59]=0x1;
cMZ80[0].z80Base[0x70da]=0x3;
cMZ80[0].z80Base[0x988a]=0x20;
cMZ80[0].z80Base[0x90a0]=0x4d;
cMZ80[0].z80Base[0x90a4]=0x34;
cMZ80[0].z80Base[0x90c0]=0x34;
cMZ80[0].z80Base[0x90ca]=0x4d;
cMZ80[0].z80Base[0x9104]=0x38;
cMZ80[0].z80Base[0x9124]=0x38;
cMZ80[0].z80Base[0x913d]=0x35;
cMZ80[0].z80Base[0x9146]=0x35;

						 break; }

case 3:{//startrek
cMZ80[0].z80Base[0x6a58]=0x3;
cMZ80[0].z80Base[0x6a5e]=0x4;
cMZ80[0].z80Base[0x486c]=0x2a;
cMZ80[0].z80Base[0xe93]=0x35;
cMZ80[0].z80Base[0xe96]=0x36;
cMZ80[0].z80Base[0xe9b]=0x6;
cMZ80[0].z80Base[0xeb0]=0x17;
cMZ80[0].z80Base[0xeb3]=0x18;
cMZ80[0].z80Base[0xeb6]=0x1a;
cMZ80[0].z80Base[0xec2]=0xd9;
cMZ80[0].z80Base[0x2d7b]=0x2a;
cMZ80[0].z80Base[0x2d7f]=0x29;
cMZ80[0].z80Base[0x48ce]=0x2a;
cMZ80[0].z80Base[0xf6c]=0x6;
cMZ80[0].z80Base[0xf76]=0x7;
cMZ80[0].z80Base[0x105e]=0x15;
cMZ80[0].z80Base[0x12f2]=0xd9;
cMZ80[0].z80Base[0x290c]=0x8;
cMZ80[0].z80Base[0x2912]=0x5;
cMZ80[0].z80Base[0x291c]=0x7;
cMZ80[0].z80Base[0x2d9b]=0x22;
cMZ80[0].z80Base[0x2cc8]=0x6e;
cMZ80[0].z80Base[0x29ab]=0xa;
cMZ80[0].z80Base[0x350b]=0x18;
cMZ80[0].z80Base[0x3537]=0x3b;
cMZ80[0].z80Base[0x3576]=0x3b;
cMZ80[0].z80Base[0x6abe]=0x3;
cMZ80[0].z80Base[0x6ace]=0x4;
cMZ80[0].z80Base[0x3564]=0x4f;
cMZ80[0].z80Base[0x2d3a]=0x23;
cMZ80[0].z80Base[0x2d3f]=0x50;
cMZ80[0].z80Base[0x2d42]=0x5a;
cMZ80[0].z80Base[0x2d47]=0x59;
cMZ80[0].z80Base[0x2d4a]=0x63;
cMZ80[0].z80Base[0x2d07]=0x59;
cMZ80[0].z80Base[0x2d0a]=0x63;
cMZ80[0].z80Base[0x2ed0]=0x17;
cMZ80[0].z80Base[0x300b]=0x1d;
cMZ80[0].z80Base[0x1470]=0x2;
cMZ80[0].z80Base[0x32ea]=0x17;
cMZ80[0].z80Base[0x32f3]=0x18;
cMZ80[0].z80Base[0x334a]=0x9a;
cMZ80[0].z80Base[0x33df]=0x2;
cMZ80[0].z80Base[0x2e9b]=0x22;
cMZ80[0].z80Base[0x2f33]=0x6;
cMZ80[0].z80Base[0x48fc]=0x2a;
cMZ80[0].z80Base[0x491f]=0x30;
cMZ80[0].z80Base[0x495e]=0x2a;
cMZ80[0].z80Base[0x4980]=0x2d;
cMZ80[0].z80Base[0x4992]=0x2a;
cMZ80[0].z80Base[0x4998]=0x29;
cMZ80[0].z80Base[0x499b]=0x2a;
cMZ80[0].z80Base[0x3b38]=0x32;
cMZ80[0].z80Base[0x3d2a]=0x32;
cMZ80[0].z80Base[0x3d3b]=0x94;
cMZ80[0].z80Base[0x3dab]=0x17;
cMZ80[0].z80Base[0x3dae]=0x18;
cMZ80[0].z80Base[0x3db7]=0x2;
cMZ80[0].z80Base[0x3dbf]=0x6;
cMZ80[0].z80Base[0x3dc7]=0x10;
cMZ80[0].z80Base[0x3e0a]=0x10;
cMZ80[0].z80Base[0x3eee]=0x17;
cMZ80[0].z80Base[0x4053]=0x13;
cMZ80[0].z80Base[0x4058]=0x0;
cMZ80[0].z80Base[0x4064]=0xa;
cMZ80[0].z80Base[0x4070]=0x3c;
cMZ80[0].z80Base[0x4073]=0x5a;
break;
	   }



}
}