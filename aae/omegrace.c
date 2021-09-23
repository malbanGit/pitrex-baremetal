#include <string.h>
#include <stdlib.h>

#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif


#include "globals.h"
#include "omegrace.h"
#include "samples.h"
#include "vector.h"
#include "input.h"
#include "cpu_control.h"
//#include "dxinput.h"
#include "acommon.h"
#include "log.h"

//extern char *gamename[];
//int a=10;
static int oticks=0;
int ovec_time=0;

int scrflip=0;
int angle=1;
int angle2=1;
//int sbit=0;
int vecwrite1 = 0x80;// Vector Processor Flag, 0=busy
int soundlatch=0;
int vec_total=0;

 


static int load_nvram(void)
{
    int num=0;
	int x=0;
    char fullpath[180];
	unsigned char *membuffer;
	FILE *fd; 
    membuffer = malloc(0xff);
    memset(membuffer, 0, 0xff); 
  	
    strcpy (fullpath,"hi");
	strcat (fullpath, "\\");
	strcat (fullpath,driver[gamenum].name);
	strcat (fullpath, ".aae");
	
	fd = fopen(fullpath, "rb");
	if(!fd) {log_it("file not found");return 1;}
    fread(membuffer, 1, 0xff, fd);
    for (x=0;x <= 0xff;x++)
	{
	 cMZ80[0].z80Base[0x5c00+x]=membuffer[x];
	}
	
	fclose(fd);
	free (membuffer);
	return 1;
}

static void save_nvram(void)
{
    int num=0xff;
	int x=0;
    char fullpath[180];
	unsigned char *membuffer;

	membuffer = malloc(0xff);
    memset(membuffer, 0, 0xff);
	
	strcpy (fullpath,"hi");
	strcat (fullpath, "\\");
	strcat (fullpath,driver[gamenum].name);
	strcat (fullpath, ".aae");
	log_it("Saving NVRAM");
	for (x=0;x <= 0xff;x++)
	{membuffer[x]=cMZ80[0].z80Base[0x5c00+x];}
	
    save_file(fullpath, membuffer, 0xff);
	free (membuffer);
}
static void set_omega_colors(void)
{
	int i=0;
	
	vec_colors[0].r=0;
	vec_colors[0].g=0;
	vec_colors[0].b=0;

	for (i=1;i<17;i++)
	 {
	  vec_colors[i].r=i*16;
      vec_colors[i].g=i*16;
      vec_colors[i].b=i*16;
	 }
}

static void dvg_vector_timer (int scale)
{
  vec_total += scale;
  
}

static void Odvg_generate_vector_list(void)
{
        //BYTE part1=0;
        //BYTE part2=0;
        int pc = 0x8000;
        int  sp=0;
        int stack [4];
        int scale = 0;
        float gc=0;
        int done = 0;
        int firstwd,secondwd;
        int opcode;
        int  x, y;
        int temp;
        int z=0;
        int deltax, deltay=0;
        int currentx, currenty = 0;

//log_it("Building List");        
while (!done)
{    
            
           // part1=z80.z80Base[pc];
            //pc++;
            //part2=z80.z80Base[pc];
            //firstwd = (((part1) | (part2<<8)) & 0xffff);
            //opcode = firstwd & 0xf000;
           // pc=pc++;
     firstwd = memrdwd(pc);opcode = firstwd & 0xf000;pc++;pc++;
        switch (opcode)
        {
            case 0: 
            case 0x1000:
            case 0x2000:
            case 0x3000:
            case 0x4000:
            case 0x5000:
            case 0x6000:
            case 0x7000:
            case 0x8000:
            case 0x9000:
                
              // compute raw X and Y values //
                //part1=z80.z80Base[pc];
                // pc++;
                // part2=z80.z80Base[pc];
                // secondwd = ((part1) | (part2<<8));
                // pc=pc++;
                secondwd =  memrdwd(pc);pc++;pc++;
                z = secondwd >> 12;
                y = firstwd & 0x03ff;
                x = secondwd & 0x03ff;
            
                // Check for Sign and Adjust
                if (firstwd & 0x0400)
                    {y = -y;}
                if (secondwd & 0x400)
                    {x= -x;}
                if (scrflip)
                    {
                    x=-x;
                    y=-y;
                    }
                temp = ((scale + (opcode>>12)) & 0x0f);
                vec_total += temp;
//               dvg_vector_timer(temp);
               deltax = (x << VEC_SHIFT) >> (9-temp);
               deltay = (y << VEC_SHIFT) >> (9-temp);   
            
DRAWCODE:       
                if (z > 1)
                { 
                 gc=(float) z/16.00;
                 gc=gc+.05;
                 if (gc>1){gc=1.0;} 
                 // z=z*16+42;
                 //if (z>255) z=255;
                 //if (config.overlay){ glColor3ub(z,z,z/3);}
                 //else { glColor3ub(z,z,z);}
                
   int x0 = currentx>>11;
   int y0 = -(currenty>>11);
   int x1 = (currentx+deltax)>>11;
   int y1 = -((currenty-deltay)>>11);
   int zz;

                  if ((currentx==(currentx)+deltax) && (currenty==(currenty)-deltay))
                  { 
//                    add_point_yr(currentx>>VEC_SHIFT,currenty>>VEC_SHIFT,z,gc,0,1);
                    zz = z;
//  v_directDraw32(x0-17000, y0+17000, x1-17000,y1+17000, zz);
                  } 
                  else
                  {
     //                 add_line_yr(currentx>>VEC_SHIFT, currenty>>VEC_SHIFT, (currentx+deltax)>>VEC_SHIFT, (currenty-deltay)>>VEC_SHIFT, z,gc,0);
// MALBAN                       add_point(currentx>>VEC_SHIFT,currenty>>VEC_SHIFT,z,gc,0,0);
// MALBAN                       add_point((currentx+deltax)>>VEC_SHIFT,(currenty-deltay)>>VEC_SHIFT,z,gc,0,0); 
zz =(z<<3)%128;
  v_directDraw32(x0-17000, y0+17000, x1-17000,y1+17000, zz);
                  }                    

                 }
                currentx += deltax;
                currenty -= deltay;
                break;
                
            case 0xa000: // Set start of drawing
                 //part1=z80.z80Base[pc];
                 //pc++;
                 //part2=z80.z80Base[pc];
                 //secondwd = ((part1) | (part2<<8));
                 //pc=pc++; 
                secondwd =  memrdwd(pc);
                pc++;
                pc++;
                x = twos_comp_val (secondwd, 12);
                y = twos_comp_val (firstwd, 12);
                //Do overall draw scaling
                if (scrflip) {x=1024-x;y=1024-y;}
                
                scale = (secondwd >> 12) & 0x0f; //This doesn't seem to matter for LL
                y=1024-y;
                currenty = (y)<<VEC_SHIFT; 
                currentx = x<<VEC_SHIFT;
                break;
                
            case 0xb000: //Halt
                 done = 1; 
                 break;

            case 0xc000:
                
                   stack [sp] = pc; //save current address
                   pc = 0x8000 + ((firstwd & 0x1fff)*2) ; // Set new adress
                   sp++; //Increment Stack
                   break; 

            case 0xd000:
                  sp--; //Decrement Stack
                 pc = stack [sp]; //Load Program Counter
                 break;
            case 0xe000: // Jump 
                
                 pc = 0x8000 + ((firstwd & 0x1fff) *2);
                 break;
           
            case 0xf000: 
                        
                // compute raw X and Y values //
            
                //z = (firstwd & 0xf0) >> 4;
                z = (firstwd >> 4) & 0x0f;
                y = firstwd & 0x0300;
                x = (firstwd & 0x03) << 8;
                //Check Sign Values and adjust as necessary
                if (firstwd & 0x0400) {y = -y;}
                if (firstwd & 0x04)   {x = -x;}
                if (scrflip)         {x=-x;y=-y;}
                
                temp = 2 + ((firstwd >> 2) & 0x02) + ((firstwd >>11) & 0x01);
                temp = ((scale + temp) & 0x0f);
                vec_total += temp;
//               dvg_vector_timer(temp);
                //Compute Deltas
               deltax = (x << VEC_SHIFT) >> (9-temp);
               deltay = (y << VEC_SHIFT) >> (9-temp);   
              
               goto DRAWCODE; //This seems bad, but it really was the best way.
               break;        
         }
    }
 
}



PORT_WRITE_HANDLER(omegrace_soundlatch_w)
{
	soundlatch=data;
	//callint=1;
	log_it("Port set pending interrupt (nmi)");
	set_pending_interrupt(INT_TYPE_NMI, 1);
	//cpu_cause_interrupt (1, 0xff);
}
PORT_READ_HANDLER(omegrace_soundlatch_r)
{
	return soundlatch;
}


PORT_WRITE_HANDLER(bs)
{
	log_it("AY Port Write");
}

PORT_READ_HANDLER(BWVG)
{
//	log_it("----AVG GO Check CALLED %d");
  if (vecwrite1==0x80) return 0x80;
  if (pipelineCounter<10) // HACK so that the pipeline is not "double" filled, every X rounds (with this game, a crash can occure, since pipeline overflows!)
  {
    Odvg_generate_vector_list(); 
  }
  if (vec_total ) 
  {
    vecwrite1=0x80;
    oticks=get_video_ticks(0xff);
  } 
  else 
  {
    vecwrite1=0;
  }
  return 0x0;//vecwrite1;
}

WRITE_HANDLER(VectorRam)
{
	cMZ80[0].z80Base[address] = data;
}
WRITE_HANDLER(NVRAM_W)
{
//log_it("NVRAM WRITE address %x data %x", address & 0xffff,data);
//GI[CPU0][address & 0xffff]=data;
//z80.z80Base[address & 0xffff]=data;

//crap->z80Base[address & 0xffff]=data;
}
PORT_WRITE_HANDLER(omega_reset)
{
	log_it("DVG reset.");
	//vecwrite1=0;
       // ocount=0;
}


PORT_READ_HANDLER(omegrace_watchdog_r)
{return 0;}

PORT_READ_HANDLER(omegrace_vg_status_r)
{ 
	float me;
	

    me = (((4500 * vec_total)/ 1000000) * 1512); //4500
	
	//log_it("Total LENGTH HERE %f and TOTAL TICKS %d",me,ticks );
//log_it("1:(%i) vidTick: %i, me: %f (vec_total: %i)",running_cpu, get_video_ticks(0), me, vec_total);	
	if (get_video_ticks(0) > me && vecwrite1==0x80 ) 
    {
//log_it("1: to zero");   
      vecwrite1=0;
      vec_total=0;
    }
	return vecwrite1;

}

PORT_READ_HANDLER(input_port_2_r)
{
	int c = 0xff;
	  
	// aae_mouse(&mickeyx, &mickeyy);
    if (INPUT_COIN1)         bitclr(c,0x01); //coin1
    if (INPUT_COIN2)         bitclr(c,0x02); //coin2
//    if (INPUT_P1_BUTTON4)                 bitclr(c,0x10); //tilt
    if (INPUT_P1_BUTTON1)  bitclr(c,0x40); //button1
    if (INPUT_P1_BUTTON2)  bitclr(c,0x20); //button2
    /*
    if (key[config.kcoin1])			bitclr(c,0x01); //coin1
    if (key[config.kcoin2])			bitclr(c,0x02); //coin2
    if (key[KEY_T])			        bitclr(c,0x10); //tilt
	if (key[config.kp1but1]  || mouseb[1])  bitclr(c,0x40); //button1
	if (key[config.kp1but2]  || mouseb[2])  bitclr(c,0x20); //button2
	*/
	if (testsw)           	        bitclr(c,0x80); //service
    
	return c;
}

PORT_READ_HANDLER(input_port_3_r)
{
	int c=0x33;

    if (INPUT_START1)       bitset(c,0x40);
    if (INPUT_START2)       bitset(c,0x80);
    if (INPUT_P2_BUTTON1)       bitclr(c,0x02);  // 2 fire
    if (INPUT_P2_BUTTON2)       bitclr(c,0x01);  //2 thrust

    /*
   	if (key[config.kstart1])       bitset(c,0x40);
	if (key[config.kstart2])       bitset(c,0x80);
	if (key[config.kstart3])       bitset(c,0x04);
	if (key[config.kstart4])	   bitset(c,0x08);
	if (key[config.kp2but2])       bitclr(c,0x02);  // 2 fire
    if (key[config.kp2but1])       bitclr(c,0x01);  //2 thrust
*/       
  return c;
}

 
PORT_READ_HANDLER(input_port_0_r)
{return z80dip1;}

PORT_READ_HANDLER(input_port_1_r)
{return z80dip2;}
PORT_READ_HANDLER(input_port_5_r )
{
	static int toggle=0;
    toggle^=1;
    if (toggle)
    {
      if (INPUT_P2_DIGITAL_LEFT) {angle2-=1; if (angle2 < 0) angle2 =63;}  
      if (INPUT_P2_DIGITAL_RIGHT) {angle2+=1; if (angle2 >63) angle2 =0; }  
//      if (key[config.kp2left]) {angle2-=2; if (angle2 < 0) angle2 =63;}  
//      if (key[config.kp2right]) {angle2+=2; if (angle2 >63) angle2 =0; }  
    }
    
    return ~(angle2 ^ (angle2 >>1));
 }


PORT_READ_HANDLER(omegrace_spinner1_r)
{
	int x;
	static int toggle=0;

	toggle^=1;
	      //    aae_mouse(&mickeyx, &mickeyy);
            //position_mouse(SCREEN_W/2, SCREEN_H/2);     
           // if (mickeyx > 0 )
		    //	{angle+=2; if (angle >63) angle =0; }  
	       // if (mickeyx < 0 )
	    	//	{angle-=2; if (angle < 0) angle =63;} 

	     if (toggle)
         { 
           if (INPUT_P1_DIGITAL_LEFT) {angle-=1; if (angle < 0) angle =63;}  
           if (INPUT_P1_DIGITAL_RIGHT){angle+=1; if (angle >63) angle =0; }  
//           if (key[config.kp1left]) {angle-=2; if (angle < 0) angle =63;}  
//           if (key[config.kp1right]){angle+=2; if (angle >63) angle =0; }  
         }

    x= spinnerTable[angle];
    return x;
}

PORT_WRITE_HANDLER(omegrace_leds_w)
{/*
    static lastled1=0;
    static lastled2=0;
    static lastled3=0;
	 //20 = 2 player 2 credit start LED
	 //10 = 2 player 1 credit start LED
	 //8 = 1 player 2 credit start LED
	 //4 = 1 player 1 credit start LED
	
	if ((~data & 0x04)!= lastled1) {lastled1=(~data & 0x04);}
	if ((~data & 0x08)!= lastled2) {lastled2=(~data & 0x08);}
	if ((~data & 0x10)!= lastled3) {lastled3=(~data & 0x10);}
   */
	//set_aae_leds(lastled1,lastled2,lastled3);
	set_aae_leds(~data & 0x04,~data & 0x08,~data & 0x10);
	
	if (!(data & 0x40)) scrflip=1; else scrflip=0;
	//if (data & 0x40)    {scrflip=0;}
}

PORT_WRITE_HANDLER(SoundWrite)
{
  
switch (data)
		{
            case 0:	{sample_stop(7);sample_stop(5); break;  }//sound off
	    	case 0x1:sample_start(2, 1, 0);break;  // my ship explosion
			case 0x2:sample_start(2, 2, 0);break; //empty ship explosion
			case 0x3:sample_start(2, 3, 0);break;  //Triangle Destruction
			case 0x4:sample_start(1, 4, 0);break; //Level complete music
			case 0x5:log_it("SOUND 5??");break; //?no sound? off sound?
			case 0x6:log_it("SOUND 6??");break; // ?no sound? off sound?
			case 0x7:sample_start(4, 7, 0);break; //ship fire 
			case 0x8:sample_start(3, 8, 0);break; //wall hit
			case 0x9:sample_start(5, 0x09, 1);sample_start(6, 0x0a, 0);break; // thrust hiss/snap start
		    case 0xa:sample_stop(5);break; //stop thrust
			case 0xb:sample_start(7, 0xb, 1);break; //slow bump bump sound loops
			case 0xc:sample_start(7, 0xc, 1);break; //Triggers bump bump after small ship die
			case 0xd:sample_start(7, 0xd, 1);break; //med bump bump sound
			case 0xe:sample_start(7, 0xe, 1);break; //ship spawn fast sound verified
			case 0xf:sample_start(7, 0xf, 1);break; //ship spawn fast sound????????????????
			case 0x10:sample_start(2, 0x10, 0);break; //manned ship explosion
			case 0x11:sample_start(1, 0x11, 0);break; //sweep sound
			case 0x12:sample_start(1, 0x12, 0);break; //coin 
			case 0x13:sample_start(1, 0x13, 0);break; //TILT
			case 0x14:sample_start(1, 0x14, 0);break; //beeping sound
			case 0x15:sample_start(2, 0x15, 0);break; //little ship die
			case 0x16:sample_start(7, 0x16, 1);break; //test mode sweep sound

			default: log_it("!!!! Unhandled sound effect!!!! %x ",data);
		}
		
 soundlatch=data;

}

/////////////////////////////VECTOR GENERATOR//////////////////////////////////

/////////////////////END VECTOR GENERATOR///////////////////////////////////////

///////////////////////  MAIN LOOP /////////////////////////////////////
void run_omega()
{
	/*
	 int passes=25;
	 int cycles = (3024000 / gamefps) / passes; //250hz
	  int cycles2 = (1512000 / gamefps) / passes; //250hz
	 int x=0;
	 
	 UINT32 dwElapsedTicks = 0;
	 UINT32 dwResult=0;
	 UINT32 dwDisplayInterval=0;
	 static int flag=0;

	
//	dwElapsedTicks = mz80GetElapsedTicks(0xff);

//0x408b  0x01 = PLAY screen
//        0x08 = 1st text screen
//	      0x04 = 2nd text screen
 //       0x0b = credit screens
 //       0x0a points per object screen
            //log_it("-------------------- FRAME START --------------------");
           
            for (x=0; x < passes; x++) //12 or 8 6 actual set at 5
		    {
			 log_it("CPU 0");	
			 mz80SetContext(&cMZ80[0]);
			 log_it("Context Set");	
	 		 dwResult = mz80exec(cycles); //6250 //8000 last //9375 //cycles v//15500
             		
			 omega_int();
			 mz80GetContext(&cMZ80[0]);
		   
			 mz80SetContext(&cMZ80[1]);
			  log_it("CPU 1");
	 		 dwResult = mz80exec(cycles2); //6250 //8000 last //9375 //cycles v//15500
			 if (callint) {mz80nmi();callint=0;}
			 mz80GetContext(&cMZ80[1]);
			
			}
			*/   		

            

            if (KeyCheck(config.ktest))   {testsw^=1;} //service
            if (KeyCheck(config.kreset))  {cpu_needs_reset(0);} //reset

		
}


MEM_READ(OmegaRead)
MEM_END
MEM_WRITE(OmegaWrite)
 MEM_ADDR(0x0000, 0x3fff, NoWrite)
 //MEM_ADDR(0x5c00, 0x5cff, NVRAM_W)
 //MEM_ADDR(0x8000, 0x8fff, VectorRam)
 MEM_ADDR(0x9000, 0x9fff, NoWrite)
MEM_END

MEM_READ(SoundMemRead)
MEM_END

MEM_WRITE(SoundMemWrite)
 MEM_ADDR(0x0000, 0x07ff, NoWrite)
MEM_END

PORT_READ(OmegaPortRead)
 PORT_ADDR(0x08, 0x08, BWVG)
 PORT_ADDR(0x09, 0x09, omegrace_watchdog_r)
 PORT_ADDR(0x0b, 0x0b, omegrace_vg_status_r) // vg_halt 
 PORT_ADDR(0x10, 0x10, input_port_0_r)  //DIP SW C4 
 PORT_ADDR(0x17, 0x17, input_port_1_r) // DIP SW C6 
 PORT_ADDR(0x11, 0x11, input_port_2_r) // Player 1 input 
 PORT_ADDR(0x12, 0x12, input_port_3_r) // Player 2 input 
 PORT_ADDR(0x15, 0x15, omegrace_spinner1_r)  //1st controller 
 PORT_ADDR(0x16, 0x16, input_port_5_r) // 2nd controller (cocktail) 
PORT_END

PORT_WRITE(OmegaPortWrite)
 PORT_ADDR( 0x21, 0x21, omega_reset)
 PORT_ADDR(0x13, 0x13, omegrace_leds_w) // coin counters, leds, flip screen 
 PORT_ADDR(0x14, 0x14, SoundWrite) //Sound command 
PORT_END

PORT_READ(SoundPortRead)
 PORT_ADDR(0x00, 0x00, omegrace_soundlatch_r)
PORT_END

PORT_WRITE(SoundPortWrite)
	{ 0x00, 0x00, bs },
	{ 0x01, 0x01, bs },
	{ 0x02, 0x02, bs },
	{ 0x03, 0x03, bs },
PORT_END
/*
PORT_WRITE(SoundPortWrite)
	{ 0x00, 0x00, AY8910_control_port_0_w },
	{ 0x01, 0x01, AY8910_write_port_0_w },
	{ 0x02, 0x02, AY8910_control_port_1_w },
	{ 0x03, 0x03, AY8910_write_port_1_w },
PORT_END
*/
/////////////////// MAIN() for program ///////////////////////////////////////////////////
int init_omega(void)
{/*
	log_it("init 0");
    cMZ80[0].z80Base=GI[CPU0];
    cMZ80[0].z80IoRead=OmegaPortRead;
    cMZ80[0].z80IoWrite=OmegaPortWrite;
	cMZ80[0].z80MemRead=OmegaRead;
    cMZ80[0].z80MemWrite=OmegaWrite;
    cMZ80[0].z80intAddr=0x38;
	cMZ80[0].z80nmiAddr=0x66;
	mz80SetContext(&cMZ80[0]);
	mz80reset();
	mz80GetContext(&cMZ80[0]);
	cMZ80[0].z80intAddr=0x38;
	cMZ80[0].z80nmiAddr=0x66;
	mz80SetContext(&cMZ80[0]);
	mz80reset();
	mz80GetContext(&cMZ80[1]);
	log_it("init 1");
	cMZ80[1].z80Base=GI[1];
    cMZ80[1].z80IoRead=SoundPortRead;
    cMZ80[1].z80IoWrite= SoundPortWrite;
	.z80MemRead=SoundMemRead;
    cMZ80[1].z80MemWrite=SoundMemWrite;
	mz80SetContext(&cMZ80[1]);
	mz80reset();
	mz80GetContext(&cMZ80[1]);
	cMZ80[1].z80intAddr=0x38;
	cMZ80[1].z80nmiAddr=0x66;
	mz80SetContext(&cMZ80[1]);
	mz80reset();
	mz80GetContext(&cMZ80[1]);
	log_it("init ok");
	*/
   initz80N(OmegaRead,OmegaWrite,OmegaPortRead, OmegaPortWrite,0);
   initz80N(SoundMemRead,SoundMemWrite,SoundPortRead, SoundPortWrite,1);
   vecwrite1=0;	
   set_omega_colors();
//   cache_clear();
   //AY8910_sh_start(&ay8910_interface);
	
    //load_hi_aae(0x5c00, 100, 0);
   memset(GI[CPU0]+0x5c00, 0xff, 0xff);
  // load_nvram();
   log_it("running cpu");
	return 0;
}

void end_omega()
{
	//save_nvram();

	//save_hi_aae(0x5c00, 100, 0);
	
		
}
