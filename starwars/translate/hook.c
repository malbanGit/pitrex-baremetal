#include "6809.h"
#include "6809dasm.h"
extern unsigned NextPC;

#ifndef FALSE
#define FALSE (0!=0)
#define TRUE  (0==0)
#endif

#define STARWARS

#ifdef STARWARS
// hook to the start of the instruction at address = iPC
// "case" has been output, but 
// nothing else, also no clock check yet
void hookInstructionStart(int _iPC)
{
}

void hookInstructionEnd(int _iPC)
{
  // check for bankswitch
  // check after the instruction that accessed MPAGE
  if (  
      (_iPC == 0xCCC2) ||
      (_iPC == 0xCCC8) ||
      (_iPC == 0xCCCE) ||
      (_iPC == 0xCCD4) ||
      (_iPC == 0xCCDA) ||
      (_iPC == 0xCCE0) ||
      (_iPC == 0xCCE6) ||
      (_iPC == 0xCCEC) ||
      (_iPC == 0xCCF2) ||
      (_iPC == 0xCCF8) ||
      (_iPC == 0xCCFE) ||
      (_iPC == 0xCD04) ||
      (_iPC == 0xCD0A) ||
      (_iPC == 0xCD10) ||
      (_iPC == 0xCD16) ||
      (_iPC == 0xCD1C) ||
      (_iPC == 0xCD22) ||
      (_iPC == 0xCD28) ||
      (_iPC == 0xCD2E) ||
      (_iPC == 0xCD34) ||
      (_iPC == 0xCD3A) ||
      (_iPC == 0xCD40) ||
      (_iPC == 0xCD46) ||
      (_iPC == 0xCD4C) ||
      (_iPC == 0xCD52) ||
      (_iPC == 0xCD58) ||
      (_iPC == 0xCD5E) ||
      (_iPC == 0xCD64) ||
      (_iPC == 0xCD6A) ||
      (_iPC == 0xCD70) ||
      (_iPC == 0xCD76) ||
      (_iPC == 0xCD7C) ||
      (_iPC == 0xCD82) ||
      (_iPC == 0xCD88) ||
      (_iPC == 0xCD8E) ||
      (_iPC == 0xCD94) ||
      
      (_iPC == 0xF263) || // RESET
      (_iPC == 0xF4F1) ||
      (_iPC == 0xF50C) )
      {
	if (!Single_Step) 
	{
	  dumpf(R_Always|R_PC,     0,                         "PC = 0x%04x;", (NextPC)&0xffff);
	  F_[NextPC] = F_[NextPC] | F_LABEL; // ensure there is an entry point after the bankswitch check!
	} 
        dumpf (R_Always, 0, "goto bankswitch_check;"); 
     }
}
#else
void hookInstructionStart(int _iPC) {}
void hookInstructionEnd(int _iPC)   {}
#endif

