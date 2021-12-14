#include <string.h>

#include "dips.h"
#include "globals.h"
#include "stdio.h"
#include "log.h"

int z80dip1;
int z80dip2;
int z80dip3;
int z80dip4;
DIPSWITCH *dips;


static void set_default_dips()

{
  int i=0;
   
  while (dips[i].Switch !=0)
  {
	     dips[i].Current = dips[i].Default; //Set Current value to default
         switch(dips[i].Switch)

		 {
		 case 1:      {z80dip1 &= dips[i].Mask;z80dip1 |= dips[i].Value[dips[i].Default]; break;}
		 case 2:      {z80dip2 &= dips[i].Mask; z80dip2 |= dips[i].Value[dips[i].Default];break;}
		 case 3:      {z80dip3 &= dips[i].Mask; z80dip2 |= dips[i].Value[dips[i].Default];break;}
		 case 4:      {z80dip4 &= dips[i].Mask; z80dip2 |= dips[i].Value[dips[i].Default];break;}
		 case 0x7777: config.cocktail=dips[i].Value[dips[i].Default];break;
         case 0x8888: config.hack=dips[i].Value[dips[i].Default];break;

		 default: { GI[CPU0][dips[i].Switch] &= dips[i].Mask;
                    GI[CPU0][dips[i].Switch] |= dips[i].Value[dips[i].Current];//Current
                    break;}
		 }
	  
  i++;
  }
}

void reset_to_default_dips()
{
int i=0;
  
  while (dips[i].Switch !=0)
  {
	  dips[i].Current= dips[i].Default; 
	  i++;
  }

}

int retrieve_dips()
{
  FILE *fp;
  int i=0;
  int d=0;
//  int r=0;
  char fullpath[180];
  
  z80dip1=0;
  z80dip2=0;
  z80dip3=0;
  z80dip4=0;


	strcpy (fullpath, "dips\\");
    strcat (fullpath,driver[gamenum].name);
	strcat (fullpath, ".dip");
   
   fp = fopen(fullpath,"r");

 if (!fp){log_it("Dipswitch save not found! - Setting Defaults");set_default_dips();return 0;}
   else { log_it("Loading dipswitches");
	      while (fscanf(fp, "%d ",&d) != EOF) {
                          dips[i].Current=d;
						 // log_it("val loaded %d",d);
						  i++;
		                  }
        }
 fclose(fp);


 //SET DIPS IF FILE FOUND
 i=0;
 do
 {   
   switch(dips[i].Switch)

		 {
		 case 0: break;
		 case 1: z80dip1 &= dips[i].Mask; z80dip1 |= dips[i].Value[dips[i].Current];break;
		 case 2: z80dip2 &= dips[i].Mask; z80dip2 |= dips[i].Value[dips[i].Current];break;
		 case 0x7777: config.cocktail=dips[i].Value[dips[i].Current];break;
         case 0x8888: config.hack=dips[i].Value[dips[i].Current];break;
		 default: { GI[CPU0][dips[i].Switch] &= dips[i].Mask;
                    GI[CPU0][dips[i].Switch] |= dips[i].Value[dips[i].Current];
                   break;}
		 }
	  
  /*
  if (dips[i].Switch==0x7777) {  if (dips[i].Value[dips[i].Current]) //CHECK FOR COCKTAIL MODE
								 {config.cocktail=1;}
								 else
								 {config.cocktail=0;}
                               } 

  else{
	   GI[dips[i].Switch] &= dips[i].Mask;
       GI[dips[i].Switch] |= dips[i].Value[dips[i].Current];
      }
	  */
  if (dips[i].Switch==1)log_it("1 val written %s data %d" ,dips[i].Heading,z80dip1);
   if (dips[i].Switch==2)log_it("2 val written %s data %d" ,dips[i].Heading,z80dip2);
  i++;
 }while (dips[i].Switch !=0);
 
 
 
 
 return 1;
}

int save_dips()
{
  FILE *fp;
  
  int i=0;
  int d=0;
//  int r=0;
  char fullpath[180];
	
	strcpy (fullpath, "dips\\");
    strcat (fullpath,driver[gamenum].name);
	strcat (fullpath, ".dip");
   
   fp = fopen(fullpath,"w");

 if (!fp){log_it("Dipswitch save not found!");return 0;}
   else {
	   do {
	       
			d=dips[i].Current;
			fprintf(fp, "%d ", d);
          
			i++;
		   
			}while (dips[i].Switch !=0);
         }
 fclose(fp);
 return 1;
}
