#ifndef DIPS_H
#define DIPS_H

extern int z80dip1;
extern int z80dip2;
extern int z80dip3;
extern int z80dip4;

typedef char *HEAD;
typedef char *OPT;

typedef struct DIPSWITCH
{
   HEAD Heading;
   OPT Options[8];
   int NumOptions;
   int Switch;
   int Mask;
   int Default;
   int Current;
   int Value[8];
} DIPSWITCH;

extern DIPSWITCH *dips;


#define DIPSW1 1
#define DIPSW2 2
#define COCKTAIL 0x7777
#define CHEAT 0x8888

#define DIP_START(name) DIPSWITCH name[] = {

#define DIP_SET(Heading,op1,op2,op3,op4,op5,op6,op7,op8,NumOptions,Switch,Mask,Default,Current,v1,v2,v3,v4,v5,v6,v7,v8) \
{ Heading,{op1,op2,op3,op4,op5,op6,op7,op8},NumOptions,Switch,Mask,Default,Current,{v1,v2,v3,v4,v5,v6,v7,v8}},

#define DIP_SET2(Switch,Mask,Heading,v1,op1,v2,op2,Default) \
{ Heading,{op1,op2," "," "," "," "," "," "},1,Switch,Mask,Default,0,{v1,v2,0,0,0,0,0,0}},

#define DIP_SET4(Switch,Mask,Heading,v1,op1,v2,op2,v3,op3,v4,op4,Default) \
{ Heading,{op1,op2,op3,op4," "," "," "," "},3,Switch,Mask,Default,0,{v1,v2,v3,v4,0,0,0,0}},

#define DIP_SET8(Switch,Mask,Heading,v1,op1,v2,op2,v3,op3,v4,op4,v5,op5,v6,op6,v7,op7,v8,op8,Default) \
{ Heading,{op1,op2,op3,op4,op5,op6,op7,op8},7,Switch,Mask,Default,0,{v1,v2,v3,v4,v5,v6,v7,v8}},

#define DIP_END {"NONE",{"NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE"},0,0,0,0,0,{0,0,0,0,0,0,0,0}}};


int retrieve_dips();
int save_dips();
void reset_to_default_dips();

#endif