/*
     This program has two separate but related uses.

     Firstly it is a simple disassembler and at some point we hope it will
     generate ASM listings that can be reassembled to the original binary,
     in such a way that changes to the asm (eg inserting a NOP anywhere in
     the code) will not break any code or data labels.

     Secondly, it is a static binary translator, capable of generating
     a C source file that will execute the same logic as an emulator would
     for the same bin file.

     The code is based on Sean Riddle's disassembler.  The biggest change to
     Sean's code is that we now attempt a tree-walk through the binary, starting
     at known executable entry points, augmented by hints from the user saved
     in a .dat file.  I should point out that because this code has taken such
     huge liberties with Sean's program, it has become quite messy and close
     to unmaintainable. (Apologies to Sean!)  Probably would be better to design
     a tree-walk program from scratch, specifically for that purpose.  However
     at the time of writing, the learning curve to get all the decoding correct
     was so steep that it was considered worth taking the shortcut of building
     on existing code.

     In SBT mode, identified code is passed to 6809.c ( http://gtoal.com/vectrex/6809sbt/6809.c.html )
     to be converted to C.  6809.c only handles one opcode at a time in order
     to keep its code simple and separate from this.

     What is *not* yet done is integration with an emulator, which would help
     significantly in determining what is code and what is data, and the addresses
     and types of data structures embedded in the binary.  These can't always 
     be determined automatically but they can be guessed at better than this
     code currently does.  Or more accurately, doesn't.

     GT.  2020/03/12 (Last edited: $Date: 2021/06/12 15:18:04 $)

     $Log: 6809dasm.c,v $
     Revision 1.5  2021/06/12 15:18:04  gtoal
     better comment handling

     Revision 1.4  2021/06/11 22:12:00  gtoal
     Adding RCS headers

 */

const char *Dasm_Version = "6809dasm.c: $Revision: 1.5 $";
extern const char *SBT_Version;
/*
   TO DO: 
   Embed default DP for vectrex, set DP when OS call made to set DP
   Get document that lists what registers are preserved vs trashed
   when calling vectrex BIOS functions, and tweak JSR def/use values
   so that registers are not forced to have values if they are not
   used in a call but are trashed by that call.
   Pick up .dat file automatically from binary file name less extension.
   Detect negative hex constants and represent them better in the
   generated disassembly text.  Current output is not wrong, just
   a little verbose, eg $fffe,X should be -2,X
*/

// disasm can be called in two contexts - where it is known that the code
// is valid, and where it is examining unknown code.  We want the former
// to add symbolic labels when it sees a fixed address being used as an
// operand to particular instructions (eg JMP $1234 or LDA $1234,X etc)
// - but not if it is a tentative decode.  Not yet supported.

// dont follow addresses that are outside the cartridge

// cache DP

// print DP accesses using '>' notation

// create 'setdp' instruction for config file in case DP_to_?? auto detection fails
// highlight code with DP in operands

// fill in holes after tree-walking *as comments* if plausibly opcodes

// - do above PER BYTE to catch offset sequences

// web interface? click on commented opcode to decode from that point

// remainder is data.  determine data types if possible, use config file for rest

// add --vectrex flag, adds bios vars, handles rom header (bios config separate from rom config)

// final display pass, make sure no gaps

// warn if overlapping opcode and operands

// where code is currently printed, extract details and save
// in an array instead.  At that point set F_OPCODE and F_OPERAND

// have separate types F_TENTATIVE_OPCODE and F_TENTATIVE_OPERAND for
// use after definitive scan is complete?

/* 6809dasm.c - a 6809 opcode disassembler */
/* Version 1.6 27-DEC-95 */
/* Copyright © 1995 Sean Riddle */

/* thanks to Franklin Bowen for bug fixes, ideas */

/* Freely distributable on any medium given all copyrights are retained */
/* by the author and no charge greater than $7.00 is made for obtaining */
/* this software */

/* Please send all bug reports, update ideas and data files to: */
/* seanriddle <at> airosurf <dot> com */

/* latest version at: */
/* http://www.seanriddle.com */

/* the data files must be kept compatible across systems! */

/* usage: 6809dasm <file> [<data file>] */
/* output to stdout, so use redirection */

/* The optional data file contains 7 types of lines: */
/* o Remarks - these are lines beginning with a semi-colon (;) */
/*   they are completely ignored. */
/* o 1 ORG line - gives the origin of the code; this is the starting */
/*   address to be used for the disassembly. */
/* o COMMENT lines - used to add comments to the end of lines of the */
/*   disassembly. */

// NOTE: comments from the config file are not currently making it into the listing.

/* o COMMENTLINE lines - provide full-line comments to be included before */
/*   a given address in the disassembly. */
/* o DATA lines - mark sections as data.  These sections will not be */
/*   disassembled, but dumped as hex data instead. */
/* o ASCII lines - mark sections as text.  These sections will not be */
/*   disassembled, but printed as text instead. */
/* o WTEXT lines - interprets section as text encoded as in Joust, */
/*   Bubbles, Sinistar (0x0=0,...,0xa=space,0xb=A,...,0x24=Z,...,0x32=:*/
/* See sample data files (*.dat) for examples. */

// Additions:
// * CODE lines - safe locations to start a tree walk from
// * SETDP lines - hints for expanding direct page accesses to full addresses

/* current limitations: */
/* o number of LABEL, DATA/ASCII, COMMENT and COMMENTLINE lines determined */
/*   at compile-time - see MAXLABEL, MAXDATA, MAXCOMMENT and MAXCOMMLINE */
/* o all DATA/ASCII lines in data file must be sorted in ascending */
/*   address order */
/* o ditto for COMMENT and COMMENTLINE lines */
/* o if a DATA/ASCII area is preceded by what 6809dasm thinks is code */
/*   that continues into the DATA/ASCII area, the data will not be marked */
/*   as such, and an error will be printed.  If this is the case, mark the */
/*   line before the data as data also. */

/* to do: */
/* o sort comment, ascii, data lines */
/* o look at JMP and JSR addresses- set to code unless overridden */
/*   in data file */
/* o perhaps a 'scan' that creates a first-guess .dat file? */
/*   generate dummy labels, mark code, find ASCII, etc. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "6809.h"
#include "6809dasm.h"

char *Progname;

//extern unsigned char *memory;
extern void flush(long regs);

#define TRUE (1==1)
#define FALSE (!TRUE)
typedef short BOOL;             /* boolean quantity */
typedef unsigned char UBYTE;    /* unsigned 8-bit quantity */

/* array sizes for labels, DATA/ASCII definitions and COMMENT/COMMENTLINE */
#define MAXLABEL 0x10000
#define MAXDATA 0x10000         /* both DATA and ASCII lines */

// queue of recursive calls for branch destinations
#define MAXQUEUE 0x10000

int next_queue_item = 0;        // exclusive
int queue[MAXQUEUE];

#define MAXSEED 5000
int next_seed = 0;
int seed[MAXSEED];

int SBT_DEBUG = FALSE;
int DEBUG_COMMENTS = FALSE;
int ALT_LABEL = FALSE;
int ADD_INSTRUCTION_MODE = 0;  // 0 = none (default), 1 = allways, 2 = only on breaks, 3 = always, PC and labels are generated

unsigned int F_[0x10000];

//#define F_VALID_DECODE 1        // Valid instruction
//#define F_INVALID_DECODE 2      // shouldn't get any of these during initial scan from known starting point
//#define F_VISITED 4
//#define F_LABEL 8               // this instruction should have a label before it
//#define F_ENDSECTION 16
//#define F_NEED_DECODE 32        // print something whether code or data
//#define F_OUTSIDE_FILE 64       // exploration stopper
//#define F_DATA 128              // not code

int DP[0x10000];                // inferred value of DP at this opcode

int codesize[0x10000];          // how many bytes this opcode and operands take up

char *hex[0x10000];             // the disassembly, for putting back in order
char *decode[0x10000];          // the disassembly, for putting back in order
char *comment[0x10000];         // ditto

/* output listing spacing */
#define TABOPNAME 22
#define TABOPERAND 28
#define TABCOMM 43

#define OPNAMETAB (TABOPNAME-1)
#define OPERANDTAB (TABOPERAND-1)
#define COMMTAB (TABCOMM-1)

typedef struct {                /* opcode structure */
  UBYTE opcode;                 /* 8-bit opcode value */
  UBYTE numoperands;
  char name[6];                 /* opcode name */
  UBYTE mode;                   /* addressing mode */
  UBYTE numcycles;              /* number of cycles - not used */
} opcodeinfo;

/* 6809 ADDRESSING MODES */
#define INH 0
#define DIR 1
#define IND 2
#define REL 3
#define EXT 4
#define IMM 5
#define LREL 6
#define PG2 7                   /* PAGE SWITCHES - Page 2 */
#define PG3 8                   /* Page 3 */

/* number of opcodes in each page */
#define NUMPG1OPS 223
#define NUMPG2OPS 38
#define NUMPG3OPS 9

int numops[3] = {
  NUMPG1OPS, NUMPG2OPS, NUMPG3OPS,
};

char modenames[9][14] = {
  "inherent",  "direct",  "indexed",  "relative",  "extended",  "immediate",  "long relative",  "page 2",  "page 3",
};

opcodeinfo pg1opcodes[NUMPG1OPS] = {    /* page 1 ops */
  {0, 1, "NEG", DIR, 6},  {3, 1, "COM", DIR, 6},  {4, 1, "LSR", DIR, 6},  {6, 1, "ROR", DIR, 6},
  {7, 1, "ASR", DIR, 6},  {8, 1, "ASL", DIR, 6},  {9, 1, "ROL", DIR, 6},  {10, 1, "DEC", DIR, 6},
  {12, 1, "INC", DIR, 6},  {13, 1, "TST", DIR, 6},  {14, 1, "JMP", DIR, 3},  {15, 1, "CLR", DIR, 6},

  {16, 1, "page2", PG2, 0},  {17, 1, "page3", PG3, 0},  {18, 0, "NOP", INH, 2},  {19, 0, "SYNC", INH, 4},
  {22, 2, "LBRA", LREL, 5},  {23, 2, "LBSR", LREL, 9},  {25, 0, "DAA", INH, 2},  {26, 1, "ORCC", IMM, 3},
  {28, 1, "ANDCC", IMM, 3},  {29, 0, "SEX", INH, 2},  {30, 1, "EXG", IMM, 8},  {31, 1, "TFR", IMM, 6},

  {32, 1, "BRA", REL, 3},  {33, 1, "BRN", REL, 3},  {34, 1, "BHI", REL, 3},  {35, 1, "BLS", REL, 3},
  {36, 1, "BCC", REL, 3},  {37, 1, "BCS", REL, 3},  {38, 1, "BNE", REL, 3},  {39, 1, "BEQ", REL, 3},
  {40, 1, "BVC", REL, 3},  {41, 1, "BVS", REL, 3},  {42, 1, "BPL", REL, 3},  {43, 1, "BMI", REL, 3},
  {44, 1, "BGE", REL, 3},  {45, 1, "BLT", REL, 3},  {46, 1, "BGT", REL, 3},  {47, 1, "BLE", REL, 3},

  {48, 1, "LEAX", IND, 2},  {49, 1, "LEAY", IND, 2},  {50, 1, "LEAS", IND, 2},  {51, 1, "LEAU", IND, 2},
  {52, 1, "PSHS", INH, 5},  {53, 1, "PULS", INH, 5},  {54, 1, "PSHU", INH, 5},  {55, 1, "PULU", INH, 5},
  {57, 0, "RTS", INH, 5},  {58, 0, "ABX", INH, 3},  {59, 0, "RTI", INH, 6},  {60, 1, "CWAI", IMM, 20},
  {61, 0, "MUL", INH, 11},  {63, 0, "SWI", INH, 19},

  {64, 0, "NEGA", INH, 2},  {67, 0, "COMA", INH, 2},  {68, 0, "LSRA", INH, 2},  {70, 0, "RORA", INH, 2},
  {71, 0, "ASRA", INH, 2},  {72, 0, "ASLA", INH, 2},  {73, 0, "ROLA", INH, 2},  {74, 0, "DECA", INH, 2},
  {76, 0, "INCA", INH, 2},  {77, 0, "TSTA", INH, 2},

  {79, 0, "CLRA", INH, 2},  {80, 0, "NEGB", INH, 2},  {83, 0, "COMB", INH, 2},  {84, 0, "LSRB", INH, 2},
  {86, 0, "RORB", INH, 2},  {87, 0, "ASRB", INH, 2},  {88, 0, "ASLB", INH, 2},  {89, 0, "ROLB", INH, 2},
  {90, 0, "DECB", INH, 2},  {92, 0, "INCB", INH, 2},  {93, 0, "TSTB", INH, 2},  {95, 0, "CLRB", INH, 2},

  {96, 1, "NEG", IND, 6},  {99, 1, "COM", IND, 6},  {100, 1, "LSR", IND, 6},  {102, 1, "ROR", IND, 6},
  {103, 1, "ASR", IND, 6},  {104, 1, "ASL", IND, 6},  {105, 1, "ROL", IND, 6},  {106, 1, "DEC", IND, 6},
  {108, 1, "INC", IND, 6},  {109, 1, "TST", IND, 6},  {110, 1, "JMP", IND, 3},  {111, 1, "CLR", IND, 6},

  {112, 2, "NEG", EXT, 7},  {115, 2, "COM", EXT, 7},  {116, 2, "LSR", EXT, 7},  {118, 2, "ROR", EXT, 7},
  {119, 2, "ASR", EXT, 7},  {120, 2, "ASL", EXT, 7},  {121, 2, "ROL", EXT, 7},  {122, 2, "DEC", EXT, 7},
  {124, 2, "INC", EXT, 7},  {125, 2, "TST", EXT, 7},  {126, 2, "JMP", EXT, 4},  {127, 2, "CLR", EXT, 7},

  {128, 1, "SUBA", IMM, 2},  {129, 1, "CMPA", IMM, 2},  {130, 1, "SBCA", IMM, 2},  {131, 2, "SUBD", IMM, 4},
  {132, 1, "ANDA", IMM, 2},  {133, 1, "BITA", IMM, 2},  {134, 1, "LDA", IMM, 2},  {136, 1, "EORA", IMM, 2},
  {137, 1, "ADCA", IMM, 2},  {138, 1, "ORA", IMM, 2},  {139, 1, "ADDA", IMM, 2},  {140, 2, "CMPX", IMM, 4},
  {141, 1, "BSR", REL, 7},  {142, 2, "LDX", IMM, 3},

  {144, 1, "SUBA", DIR, 4},  {145, 1, "CMPA", DIR, 4},  {146, 1, "SBCA", DIR, 4},  {147, 1, "SUBD", DIR, 6},
  {148, 1, "ANDA", DIR, 4},  {149, 1, "BITA", DIR, 4},  {150, 1, "LDA", DIR, 4},  {151, 1, "STA", DIR, 4},
  {152, 1, "EORA", DIR, 4},  {153, 1, "ADCA", DIR, 4},  {154, 1, "ORA", DIR, 4},  {155, 1, "ADDA", DIR, 4},
  {156, 1, "CPX", DIR, 6},  {157, 1, "JSR", DIR, 7},  {158, 1, "LDX", DIR, 5},  {159, 1, "STX", DIR, 5},

  {160, 1, "SUBA", IND, 4},  {161, 1, "CMPA", IND, 4},  {162, 1, "SBCA", IND, 4},  {163, 1, "SUBD", IND, 6},
  {164, 1, "ANDA", IND, 4},  {165, 1, "BITA", IND, 4},  {166, 1, "LDA", IND, 4},  {167, 1, "STA", IND, 4},
  {168, 1, "EORA", IND, 4},  {169, 1, "ADCA", IND, 4},  {170, 1, "ORA", IND, 4},  {171, 1, "ADDA", IND, 4},
  {172, 1, "CPX", IND, 6},  {173, 1, "JSR", IND, 7},  {174, 1, "LDX", IND, 5},  {175, 1, "STX", IND, 5},

  {176, 2, "SUBA", EXT, 5},  {177, 2, "CMPA", EXT, 5},  {178, 2, "SBCA", EXT, 5},  {179, 2, "SUBD", EXT, 7},
  {180, 2, "ANDA", EXT, 5},  {181, 2, "BITA", EXT, 5},  {182, 2, "LDA", EXT, 5},  {183, 2, "STA", EXT, 5},
  {184, 2, "EORA", EXT, 5},  {185, 2, "ADCA", EXT, 5},  {186, 2, "ORA", EXT, 5},  {187, 2, "ADDA", EXT, 5},
  {188, 2, "CPX", EXT, 7},  {189, 2, "JSR", EXT, 8},  {190, 2, "LDX", EXT, 6},  {191, 2, "STX", EXT, 6},
  
  {192, 1, "SUBB", IMM, 2},  {193, 1, "CMPB", IMM, 2},  {194, 1, "SBCB", IMM, 2},  {195, 2, "ADDD", IMM, 4},
  {196, 1, "ANDB", IMM, 2},  {197, 1, "BITB", IMM, 2},  {198, 1, "LDB", IMM, 2},  {200, 1, "EORB", IMM, 2},
  {201, 1, "ADCB", IMM, 2},  {202, 1, "ORB", IMM, 2},  {203, 1, "ADDB", IMM, 2},  {204, 2, "LDD", IMM, 3},
  {206, 2, "LDU", IMM, 3},

  {208, 1, "SUBB", DIR, 4},  {209, 1, "CMPB", DIR, 4},  {210, 1, "SBCB", DIR, 4},  {211, 1, "ADDD", DIR, 6},
  {212, 1, "ANDB", DIR, 4},  {213, 1, "BITB", DIR, 4},  {214, 1, "LDB", DIR, 4},  {215, 1, "STB", DIR, 4},
  {216, 1, "EORB", DIR, 4},  {217, 1, "ADCB", DIR, 4},  {218, 1, "ORB", DIR, 4},  {219, 1, "ADDB", DIR, 4},
  {220, 1, "LDD", DIR, 5},  {221, 1, "STD", DIR, 5},  {222, 1, "LDU", DIR, 5},  {223, 1, "STU", DIR, 5},

  {224, 1, "SUBB", IND, 4},  {225, 1, "CMPB", IND, 4},  {226, 1, "SBCB", IND, 4},  {227, 1, "ADDD", IND, 6},
  {228, 1, "ANDB", IND, 4},  {229, 1, "BITB", IND, 4},  {230, 1, "LDB", IND, 4},  {231, 1, "STB", IND, 4},
  {232, 1, "EORB", IND, 4},  {233, 1, "ADCB", IND, 4},  {234, 1, "ORB", IND, 4},  {235, 1, "ADDB", IND, 4},
  {236, 1, "LDD", IND, 5},  {237, 1, "STD", IND, 5},  {238, 1, "LDU", IND, 5},  {239, 1, "STU", IND, 5},

  {240, 2, "SUBB", EXT, 5},  {241, 2, "CMPB", EXT, 5},  {242, 2, "SBCB", EXT, 5},  {243, 2, "ADDD", EXT, 7},
  {244, 2, "ANDB", EXT, 5},  {245, 2, "BITB", EXT, 5},  {246, 2, "LDB", EXT, 5},  {247, 2, "STB", EXT, 5},
  {248, 2, "EORB", EXT, 5},  {249, 2, "ADCB", EXT, 5},  {250, 2, "ORB", EXT, 5},  {251, 2, "ADDB", EXT, 5},
  {252, 2, "LDD", EXT, 6},  {253, 2, "STD", EXT, 6},  {254, 2, "LDU", EXT, 6},  {255, 2, "STU", EXT, 6}
};

opcodeinfo pg2opcodes[NUMPG2OPS] = {    /* page 2 ops 10xx */
  {33, 3, "LBRN", LREL, 5},  {34, 3, "LBHI", LREL, 5},  {35, 3, "LBLS", LREL, 5},  {36, 3, "LBCC", LREL, 5},
  {37, 3, "LBCS", LREL, 5},  {38, 3, "LBNE", LREL, 5},  {39, 3, "LBEQ", LREL, 5},  {40, 3, "LBVC", LREL, 5},
  {41, 3, "LBVS", LREL, 5},  {42, 3, "LBPL", LREL, 5},  {43, 3, "LBMI", LREL, 5},  {44, 3, "LBGE", LREL, 5},
  {45, 3, "LBLT", LREL, 5},  {46, 3, "LBGT", LREL, 5},  {47, 3, "LBLE", LREL, 5},  {63, 1, "SWI2", INH, 20},     // 2->1 fix from Malban
  {131, 3, "CMPD", IMM, 5},  {140, 3, "CMPY", IMM, 5},  {142, 3, "LDY", IMM, 4},  {147, 2, "CMPD", DIR, 7},
  {156, 2, "CMPY", DIR, 7},  {158, 2, "LDY", DIR, 6},  {159, 2, "STY", DIR, 6},  {163, 2, "CMPD", IND, 7},
  {172, 2, "CMPY", IND, 7},  {174, 2, "LDY", IND, 6},  {175, 2, "STY", IND, 6},  {179, 3, "CMPD", EXT, 8},
  {188, 3, "CMPY", EXT, 8},  {190, 3, "LDY", EXT, 7},  {191, 3, "STY", EXT, 7},  {206, 3, "LDS", IMM, 4},
  {222, 2, "LDS", DIR, 6},  {223, 2, "STS", DIR, 6},  {238, 2, "LDS", IND, 6},  {239, 2, "STS", IND, 6},
  {254, 3, "LDS", EXT, 7},  {255, 3, "STS", EXT, 7},
};

opcodeinfo pg3opcodes[NUMPG3OPS] = {    /* page 3 ops 11xx */
  {63, 1, "SWI3", INH, 20},  {131, 3, "CMPU", IMM, 5},  {140, 3, "CMPS", IMM, 5},  {147, 2, "CMPU", DIR, 7},
  {156, 2, "CMPS", DIR, 7},  {163, 2, "CMPU", IND, 7},  {172, 2, "CMPS", IND, 7},  {179, 3, "CMPU", EXT, 8},
  {188, 3, "CMPS", EXT, 8},};

opcodeinfo *pgpointers[3] = {
  pg1opcodes, pg2opcodes, pg3opcodes,
};

int count;                      /* current program counter for disasm */

/* getbyte() - get a byte from a file, and increment the byte counter */

// rom_start <= addr < rom_end is used in optimising jumps and the use of labels.
// jumps outside the current rom use PC=$FFFF; JUMP; but jumps within the same file can be
// implemented with a "goto LFFFF;"
int rom_start = 0, rom_end = 0;

unsigned char mem[0x10000];

int getbyte (int *fp) {         // file is actually in ram already
  int c;

  if ((*fp) >= rom_end)
    return -1;
  // GT: DEBUGGING: I moved count++ to after the rom_end test rather than before.
  count++;                      // watch out for 0/1 base for first byte in display code...
  c = mem[(*fp)++];
  return (c & 255);
}

int getword (int *fp) {
  int low, high;

  high = getbyte (fp);
  low = getbyte (fp);
  return (high << 8) + low;
}

void save (char *s) {
  char *commentp;
  unsigned int addr, junk, bytes;

  // xxxx: nn nn nn nn nn OPC OPD ;comment
  if (s[20] != ' ') {
    fprintf(stderr, "%s\n                    ^\n", s);
    assert (s[20] == ' ');
  }
  s[20] = '\0';
  (void) sscanf (s, "%04X: ", &addr);
  decode[addr] = strdup (s + 21);
  if ((commentp = strchr (decode[addr], ';')) != NULL) {
    if (commentp > decode[addr]) {
      if (*(commentp - 1) == ' ' && *commentp == ';') {
        commentp -= 1;
        *commentp++ = '\0';
        comment[addr] = commentp;
      }
    }
  }
  s = s + 6;
  hex[addr] = strdup (s);
  for (bytes = 0; bytes < 5; bytes++) {
    if (*s == ' ')
      break;
    (void) sscanf (s, "%02X ", &junk);;
    s += 3;
  }
  codesize[addr] = bytes;
}

const char *regs[5] = { "X", "Y", "U", "S", "PC" };

const char *teregs[16] = {
  "D", "X", "Y", "U", "S", "PC",
  "inv", "inv", "A", "B", "CC",
  "DP", "inv", "inv", "inv", "inv"
};

BOOL PC = FALSE;                /* to see if a PUL instr is pulling PC */

#define LABELSIZE 40
/* label structure */
struct lastruct {
  unsigned short lab;           /* label address */
  char label[LABELSIZE];        /* label text */
}  *labarray = NULL;
int numlab = 0;                 /* number of labels defined */

char labtemp[LABELSIZE];               /* global return for checklabs() - tsk */
//GTDEBUG: char *fulladdr = "$%04hX";
char *fulladdr = "$%04X";
char *shortaddr = "$%02hX";

/* checklabs() - check the defined labels from data file */
/* substitute label for address if found */
char *checklabs (char *opname, int address, BOOL lab2, BOOL printdollar, BOOL dosmall) {
  int i;
  char *printaddr;

  address &= 0xffff;
  //GTDEBUG  if (address >= 0x3ff) fprintf(stderr, "checklabs @ %04x  F_LABEL=%s\n", address, F_[address]&F_LABEL ? "Y" : "N");
  labtemp[0] = '\0';
  if (*opname && ((address >= 256) || dosmall)) {
    // This could be better done with a lookup table of flag bits.
    if ((!strcasecmp (opname, "BRA"))
        || (!strcasecmp (opname, "BRN"))
        || (!strcasecmp (opname, "BHI"))
        || (!strcasecmp (opname, "BLS"))
        || (!strcasecmp (opname, "BCC"))
        || (!strcasecmp (opname, "BCS"))
        || (!strcasecmp (opname, "BNE"))
        || (!strcasecmp (opname, "BEQ"))
        || (!strcasecmp (opname, "BVC"))
        || (!strcasecmp (opname, "BVS"))
        || (!strcasecmp (opname, "BPL"))
        || (!strcasecmp (opname, "BMI"))
        || (!strcasecmp (opname, "BGE"))
        || (!strcasecmp (opname, "BLT"))
        || (!strcasecmp (opname, "BGT"))
        || (!strcasecmp (opname, "BLE"))
        || (!strcasecmp (opname, "BSR"))
        || (!strcasecmp (opname, "LBRA"))
        || (!strcasecmp (opname, "LBRN"))
        || (!strcasecmp (opname, "LBHI"))
        || (!strcasecmp (opname, "LBLS"))
        || (!strcasecmp (opname, "LBCC"))
        || (!strcasecmp (opname, "LBCS"))
        || (!strcasecmp (opname, "LBNE"))
        || (!strcasecmp (opname, "LBEQ"))
        || (!strcasecmp (opname, "LBVC"))
        || (!strcasecmp (opname, "LBVS"))
        || (!strcasecmp (opname, "LBPL"))
        || (!strcasecmp (opname, "LBMI"))
        || (!strcasecmp (opname, "LBGE"))
        || (!strcasecmp (opname, "LBLT"))
        || (!strcasecmp (opname, "LBGT"))
        || (!strcasecmp (opname, "LBLE"))
        || (!strcasecmp (opname, "LBSR"))
        || (!strcasecmp (opname, "JMP"))
        || (!strcasecmp (opname, "JSR"))
     ) {
      if ((next_queue_item < MAXQUEUE) && (address < 0x8000)) queue[next_queue_item++] = address;
      F_[address] |= F_LABEL;
      // TO DO: Add DP register to direct 8bit addresses
    }
    for (i = 0; i < numlab; i++)
      if (address == labarray[i].lab) {
        sprintf (labtemp, "%s", labarray[i].label);
        if (lab2) strcat (labtemp, ":\n");
        i = numlab;
      }
    printaddr = (address < 256 ? shortaddr : fulladdr);
    if (!strlen (labtemp) && !lab2) {
      if (!printdollar) printaddr++;
      sprintf (labtemp, printaddr, address);
    }
  } else if (*opname == '\0') 
  {
if (ALT_LABEL)
    sprintf(labtemp, "L%04X_", address); 
else
    sprintf(labtemp, "L%04X", address); 

if (lab2) strcat (labtemp, ":\n");
    
    
    for (i = 0; i < numlab; i++)
      if (address == labarray[i].lab) {
        sprintf (labtemp, "%s", labarray[i].label);
        if (lab2) strcat (labtemp, ":\n");
        i = numlab;
      }
  }
  //GTDEBUG:  if (address >= 0x3ff) fprintf(stderr, "checklabs @ %04x -> %s\n", address, labtemp);
  return (labtemp);
}

/* printoperands() - print operands for the given opcode */
void printoperands (int current_address, int opcode, UBYTE numoperands, UBYTE * operandarray, UBYTE mode, int *fp, char *opname, char *str) {
  int i, rel, pb, offset = 0, reg, pb2;
  BOOL comma;
  char out2[80];
  int sp;

  PC = FALSE;
  if ((opcode != 0x1f) && (opcode != 0x1e)) {
    switch (mode) {             /* print before operands */
    case IMM:
      strcat (str, "#");
      break;
    case DIR:
      break;
    case EXT:
      break;
    default:
      break;
    }
  }
  switch (mode) {
  case REL:                    /* 8-bit relative */
    rel = operandarray[0];
    sprintf (out2, "%s", checklabs (opname, (short) (count + ((rel < 128) ? rel : rel - 256)), FALSE, TRUE, TRUE));
    strcat (str, out2);
    break;
  case LREL:                   /* 16-bit long relative */
    rel = (operandarray[0] << 8) + operandarray[1];
    sprintf (out2, "%s", checklabs (opname, count + ((rel < 32768) ? rel : rel - 65536), FALSE, TRUE, TRUE));
    strcat (str, out2);
    break;
  case IND:                    /* indexed- many flavors */
    pb = operandarray[0];
    reg = (pb >> 5) & 0x3;
    // READ THIS CODE AND USE HERE!
    // http://vectrex.malban.de/preliminary/eb765dd8.html

    // filter out not allowed combinations
    // 1xxx 0111
    // 1xxx 1010
    // 1xxx 1110
    if (((pb & 0x8f) == 0x87) || ((pb & 0x8f) == 0x8a) || ((pb & 0x8f) == 0x8e)) {
      // return incorrectDisassembleFoundAt(currentPC, "Illegal postbyte value for indexed addressing, bit combination of 4 lower bits.");
      F_[current_address] |= F_INVALID_DECODE;
    }
    // filter out not allowed combinations
    // 1xx1 0010
    // 1xx1 0000
    if (((pb & 0x9f) == 0x92) || ((pb & 0x9f) == 0x90)) {
      // return incorrectDisassembleFoundAt(currentPC, "Illegal postbyte value ,-R or ,R+ not allowed for indirect indexed addressing.");
      F_[current_address] |= F_INVALID_DECODE;
    }
    // filter out not allowed combinations
    // 1xx0 1111
    if (((pb & 0x9f) == 0x8f)) {
      // return incorrectDisassembleFoundAt(currentPC, "Illegal postbyte value 16 bit extended not allowed in indexed (non indirect) addressing.");
      F_[current_address] |= F_INVALID_DECODE;
    }
    // filter out not allowed combinations
    // 1011 1111
    // 1101 1111
    // 1111 1111
    if (((pb & 0xff) == 0xbf) || ((pb & 0xff) == 0xdf) || ((pb & 0xff) == 0xff)) {
      // return incorrectDisassembleFoundAt(currentPC, "Illegal postbyte value 16 bit indirect indexed addressing expects bits 5 and 6 to be 0");
      F_[current_address] |= F_INVALID_DECODE;
    }
    // 1xxx 1100
    // 1xxx 1101
    if (((pb & 0x8f) == 0x8d) || ((pb & 0x8f) == 0x8c)) {
      // reg == PC!
      reg = 4;

      if (((pb >> 4) & 0x6) != 0) {
        /* 
           1XX? ???? X bits can be anything, asmj converts "sta <$30,pc" to "ec 8c 30" which is correct but "ec ec 30" but "ec dc 30" but "ec bc 30" are also correct, but only
           the first 100% ressembles old binary therefor the below three variants must be converted to DB, with comment not to convert to code! this is true for both 16 bit or 8
           bit variant! */
        // return incorrectDisassembleFoundAt(currentPC, "Ambiguous pc index addressing, postbytes bit 5 and 6 are non zero (assi generates 0).");
        F_[current_address] |= F_INVALID_DECODE; // ... Maybe?
      }
    }

    pb2 = pb & 0x8f;
    if ((pb2 == 0x88) || (pb2 == 0x8c)) {       /* 8-bit offset */
      offset = getbyte (fp);
      sprintf (out2, "%02hX ", offset);
      strcat (str, out2);
      if (offset > 127)         /* convert to signed */
        offset = offset - 256;
      if (pb == 0x8c)
        reg = 4;
      for (sp = strlen (str); sp < OPNAMETAB; sp++)
        strcat (str, " ");
      sprintf (out2, "%s", opname);
      strcat (str, out2);
      for (sp = strlen (str); sp < OPERANDTAB; sp++)
        strcat (str, " ");
      if ((pb & 0x90) == 0x90)
        strcat (str, "[");
      if (pb == 0x8c)
        sprintf (out2, "%s,%s", checklabs (opname, offset, FALSE, TRUE, TRUE), regs[reg]);
      else if ((char)offset >= 0)        // cast to signed short?
        sprintf (out2, "$%02X,%s", offset, regs[reg]);
      else
        sprintf (out2, "-$%02X,%s", -(char)offset, regs[reg]);
      strcat (str, out2);
      if (pb == 0x8c) {
        sprintf (out2, " ; ($%04X)", offset + count);
        strcat (str, out2);
      }
    } else if ((pb2 == 0x89) || (pb2 == 0x8d) || (pb2 == 0x8f)) {       /* IND 16-bit */
      offset = (getbyte (fp) << 8);
      sprintf (out2, "%02X ", offset >> 8);
      strcat (str, out2);
      offset += getbyte (fp);
      sprintf (out2, "%02X ", offset & 0xff);
      strcat (str, out2);
      if ((pb != 0x8f) && (offset > 32767))
        offset = offset - 65536;
      offset &= 0xffff;
      if (pb == 0x8d)
        reg = 4;
      for (sp = strlen (str); sp < OPNAMETAB; sp++)
        strcat (str, " ");
      sprintf (out2, "%s", opname);
      strcat (str, out2);
      for (sp = strlen (str); sp < OPERANDTAB; sp++)
        strcat (str, " ");
      if ((pb & 0x90) == 0x90)
        strcat (str, "[");
      if (pb == 0x9f)
        sprintf (out2, "%s", checklabs (opname, offset, FALSE, TRUE, TRUE));
      else if (pb == 0x8d)
        sprintf (out2, "%s,%s", checklabs (opname, offset, FALSE, TRUE, TRUE), regs[reg]);
      else if ((short)offset >= 0)
        sprintf (out2, "$%04X,%s", offset, regs[reg]);
      else
        sprintf (out2, "-$%04X,%s", -(short)offset, regs[reg]);   // WAIT A MINUTE!  CHECK THIS!  Maybe -offset ??? BUG FIX NEEDED - TEST FIRST...
      strcat (str, out2);
      if (pb == 0x8d) {
        sprintf (out2, " ; ($%04X)", offset + count);
        strcat (str, out2);
      }
    } else if (pb & 0x80) {
      for (sp = strlen (str); sp < OPNAMETAB; sp++)
        strcat (str, " ");
      sprintf (out2, "%s", opname);
      strcat (str, out2);
      for (sp = strlen (str); sp < OPERANDTAB; sp++)
        strcat (str, " ");
      if ((pb & 0x90) == 0x90)
        strcat (str, "[");
      if ((pb & 0x8f) == 0x80) {
        sprintf (out2, ",%s+", regs[reg]);
        strcat (str, out2);
      } else if ((pb & 0x8f) == 0x81) {
        sprintf (out2, ",%s++", regs[reg]);
        strcat (str, out2);
      } else if ((pb & 0x8f) == 0x82) {
        sprintf (out2, ",-%s", regs[reg]);
        strcat (str, out2);
      } else if ((pb & 0x8f) == 0x83) {
        sprintf (out2, ",--%s", regs[reg]);
        strcat (str, out2);
      } else if ((pb & 0x8f) == 0x84) {
        sprintf (out2, ",%s", regs[reg]);
        strcat (str, out2);
      } else if ((pb & 0x8f) == 0x85) {
        sprintf (out2, "B,%s", regs[reg]);
        strcat (str, out2);
      } else if ((pb & 0x8f) == 0x86) {
        sprintf (out2, "A,%s", regs[reg]);
        strcat (str, out2);
      } else if ((pb & 0x8f) == 0x8b) {
        sprintf (out2, "D,%s", regs[reg]);
        strcat (str, out2);
      }
    } else {                    /* IND 5-bit offset */
      offset = pb & 0x1f;
      if (offset > 15) offset = offset - 32;
      for (sp = strlen (str); sp < OPNAMETAB; sp++) strcat (str, " ");
      sprintf (out2, "%s", opname);
      strcat (str, out2);
      for (sp = strlen (str); sp < OPERANDTAB; sp++) strcat (str, " ");
      {        // LEAX $FFFF,Y GT DEBUG
        char *s = checklabs (opname, offset, FALSE, TRUE, TRUE);
        char buff1[16], buff2[16];
        sprintf(buff1, "$%02X", (offset&0xff));
        sprintf(buff2, "$%04X", (offset&0xffff));
        //fprintf(stdout, "// %s %s %s\n", s, buff1, buff2);
        if (((short)offset < 0) && (strcmp(s, buff1) == 0 || strcmp(s, buff2) == 0)) {
          sprintf (out2, "-%d,%s", -(short)offset, regs[reg]);
        } else {
          sprintf (out2, "%s,%s", s, regs[reg]);
        }
        }
      strcat (str, out2);
    }
    if ((pb & 0x90) == 0x90) strcat (str, "]");
    break;
  default:
    if ((opcode == 0x1f) || (opcode == 0x1e)) { /* TFR/EXG */
      sprintf (out2, "%s,%s", teregs[(operandarray[0] >> 4) & 0xf], teregs[operandarray[0] & 0xf]);
      strcat (str, out2);
    } else if ((opcode == 0x34) || (opcode == 0x36)) {  /* PUSH */
      comma = FALSE;
      if (operandarray[0] & 0x80) {
        strcat (str, "PC");
        comma = TRUE;
        PC = TRUE;
      }
      if (operandarray[0] & 0x40) {
        if (comma) strcat (str, ",");
        if ((opcode == 0x34) || (opcode == 0x35))
          strcat (str, "U");
        else
          strcat (str, "S");
        comma = TRUE;
      }
      if (operandarray[0] & 0x20) {
        if (comma) strcat (str, ",");
        strcat (str, "Y");
        comma = TRUE;
      }
      if (operandarray[0] & 0x10) {
        if (comma) strcat (str, ",");
        strcat (str, "X");
        comma = TRUE;
      }
      if (operandarray[0] & 0x8) {
        if (comma) strcat (str, ",");
        strcat (str, "DP");
        comma = TRUE;
      }
      if (operandarray[0] & 0x4) {
        if (comma) strcat (str, ",");
        strcat (str, "B");
        comma = TRUE;
      }
      if (operandarray[0] & 0x2) {
        if (comma) strcat (str, ",");
        strcat (str, "A");
        comma = TRUE;
      }
      if (operandarray[0] & 0x1) {
        if (comma) strcat (str, ",");
        strcat (str, "CC");
      }
    } else if ((opcode == 0x35) || (opcode == 0x37)) {  /* PULL */
      comma = FALSE;
      if (operandarray[0] & 0x1) {
        strcat (str, "CC");
        comma = TRUE;
      }
      if (operandarray[0] & 0x2) {
        if (comma) strcat (str, ",");
        strcat (str, "A");
        comma = TRUE;
      }
      if (operandarray[0] & 0x4) {
        if (comma) strcat (str, ",");
        strcat (str, "B");
        comma = TRUE;
      }
      if (operandarray[0] & 0x8) {
        if (comma) strcat (str, ",");
        strcat (str, "DP");
        comma = TRUE;
      }
      if (operandarray[0] & 0x10) {
        if (comma) strcat (str, ",");
        strcat (str, "X");
        comma = TRUE;
      }
      if (operandarray[0] & 0x20) {
        if (comma) strcat (str, ",");
        strcat (str, "Y");
        comma = TRUE;
      }
      if (operandarray[0] & 0x40) {
        if (comma) strcat (str, ",");
        if ((opcode == 0x34) || (opcode == 0x35))
          strcat (str, "U");
        else
          strcat (str, "S");
        comma = TRUE;
      }
      if (operandarray[0] & 0x80) {
        if (comma) strcat (str, ",");
        strcat (str, "PC");
        strcat (str, " ;(PUL? PC=RTS)");
        PC = TRUE;
      }
    } else {
      /* 
         _8bitPrefix="<"; _5bitPrefix="<<"; _16bitPrefix=">"; */
      if (numoperands == 0) ;
      else if (numoperands == 2) {
        /* if (mode != IMM) strcat(str, ">"); */// 16bit prefix
        strcat (str, checklabs (opname, (operandarray[0] << 8) + operandarray[1], FALSE, TRUE, TRUE));
      } else if (numoperands == 1) {    /* ((numoperands==1)&&(mode!=IMM)) */
        // give indicate 1 operand = 8 bit
        if (mode != IMM) strcat (str, "<");    // 8bit prefix
        strcat (str, checklabs (opname, operandarray[0], FALSE, TRUE, TRUE));
      } else {
        /* if (mode != IMM) strcat(str, "<<"); */// 5bit prefix. not generally supported by assemblers.
        strcat (str, "$");
        for (i = 0; i < numoperands; i++) {
          sprintf (out2, "%02X", operandarray[i]);
          strcat (str, out2);
        }
      }
    }
    break;
  }
}

#define DATA 0                  /* type of data to display */
#define ASCII 1
#define WTEXT 2

/* DATA/ASCII structure definition */
struct dastruct {
  unsigned short start;         /* beginning address of DATA/ASCII */
  unsigned short end;           /* ending address */
  unsigned short per_line;      /* values to print on a line */
  short type;                   /* DATA or ASCII ? */
}  *dataarray = NULL;

/* COMMENT/COMMENTLINE structure definition */

// Changing the way we handle these - previously it was a flexible array of
// struct elements, with a field for the address.  I'm changing it so that
// now there is a commarray item for each code address.  Doesn't yet support
// multiple commentlines for one address, but I'll add that next.

char *commarray[0x10000], *commlinearray[0x10000];

char out[512], out2[80];        /* temp string buffers */

void readdatafile (char *filename, int *org, int *numlab, int *numdata) {
  char line2[256];
  char *line;
  FILE *fp;
  int dataline, data, data2, per_line, i, k;

  if ((fp = fopen (filename, "r"))) {   /* read data file */
    /* fprintf(stderr, "DEBUG: reading %s\n", filename); */
    while (fgets (line2, 255, fp)) {
      while (strlen (line2) && !isprint (line2[strlen (line2) - 1]))    /* strip cr */
        line2[strlen (line2) - 1] = '\0';
      for (i = 0; i < (int) strlen (line2); i++)
        if ((line2[i] == ';') || isalpha (line2[i])) {
          line = &(line2[i]);
          i = strlen (line2);
        }
      if (!strncasecmp (line, "ORG ", 4)) {
        int neworg;
        sscanf (&line[4], "%X", &neworg);
        if ((*org != -1) && (*org != 0)) {
          if (*org != neworg) {
            fprintf (stderr, "%s: More than one ORG line - prev was %X\n", Progname, *org);
            fprintf (stderr, "    ORG set to %X\n", neworg);
          }
        }
        *org = neworg;
      } else if (!strncasecmp (line, "LABEL ", 6)) {
        if (*numlab < MAXLABEL) {
          sscanf (&line[6], "%X", &dataline);
          labarray[*numlab].lab = dataline;
          k = 6;
          while (line[k] == ' ') k++;
          while (line[k] != ' ') k++;
          while (line[k] == ' ') k++;
          strncpy (labarray[*numlab].label, &line[k], LABELSIZE);
          labarray[*numlab].label[LABELSIZE - 1] = 0;   /* just in case */

          for (i = 0; i < *numlab; i++)
            if (!strcmp (labarray[i].label, labarray[*numlab].label)) {
              /* GT: (temp removed until new DP mechanism) fprintf(stderr, "duplicate label: %s\n",labarray[i].label); */
              break;
            }
          if (i >= *numlab)
            (*numlab)++;
          //fprintf(stderr, "DEBUG!!!  Added %s %04x  Set F_[%04x] |= F_LABEL\n", labarray[(*numlab)-1].label, labarray[(*numlab)-1].lab, labarray[(*numlab)-1].lab);
          // GTDEBUG:
          F_[labarray[(*numlab)-1].lab] |= F_LABEL;
          
        } else
          fprintf (stderr, "%s: Too many labels\n", Progname);
      } else if (!strncasecmp (line, "CODE ", 5)) {
        if (next_seed < MAXSEED) {
          sscanf (&line[5], "%X", &data);
          seed[next_seed++] = data;
          ///fprintf (stderr, "DEBUG: Adding code seed $%04x\n", data);
        } else
          fprintf (stderr, "%s: Too many CODE lines\n", Progname);
      } else if (!strncasecmp (line, "SETDP ", 6)) {
        sscanf (&line[6], "%X", &data);
        /* fprintf(stderr, "DEBUG: Adding setdp $%04x\n", data); */
      } else if (!strncasecmp (line, "DATA ", 5)) {
        if (*numdata < MAXDATA) {
          data2 = 0;
          sscanf (&line[5], "%X-%X", &data, &data2);
          k = 5;
          while (line[k] == ' ')
            k++;
          while (line[k] != ' ')
            k++;
          while (line[k] == ' ')
            k++;
          if (k < i)
            sscanf (&(line[k]), "%d", &per_line);
          else
            per_line = 0;
          if (data2 < data)
            data2 = data;
          dataarray[*numdata].type = DATA;
          dataarray[*numdata].start = data;
          dataarray[*numdata].end = data2;
          dataarray[*numdata].per_line = per_line;
          if (((*numdata) && (dataarray[*numdata - 1].start < dataarray[*numdata].start)) || !*numdata)
            (*numdata)++;
          else
            fprintf (stderr, "%s: DATA out of order\n", Progname);

          {int i; for (i = data; i <= data2; i++) F_[i] |= F_DATA; } // not to be explored as code. 

        } else
          fprintf (stderr, "%s: Too many DATA/ASCII lines\n", Progname);
      } else if (!strncasecmp (line, "ASCII ", 6)) {
        if (*numdata < MAXDATA) {
          data2 = 0;
          per_line = 0;
          sscanf (&line[6], "%X-%X %d", &data, &data2, &per_line);
          if (data2 < data)
            data2 = data;
          dataarray[*numdata].type = ASCII;
          dataarray[*numdata].start = data;
          dataarray[*numdata].end = data2;
          dataarray[*numdata].per_line = per_line;
          if (((*numdata) && (dataarray[*numdata - 1].start < dataarray[*numdata].start)) || !*numdata)
            (*numdata)++;
          else if (*numdata)
            fprintf (stderr, "%s: '%s'\nASCII out of order\n\n", Progname, line);
        } else
          fprintf (stderr, "%s: Too many DATA/ASCII lines\n", Progname);
      } else if (!strncasecmp (line, "WTEXT ", 6)) {
        if (*numdata < MAXDATA) {
          data2 = 0;
          per_line = 0;
          sscanf (&line[6], "%X-%X %d", &data, &data2, &per_line);
          if (data2 < data)
            data2 = data;
          dataarray[*numdata].type = WTEXT;
          dataarray[*numdata].start = data;
          dataarray[*numdata].end = data2;
          dataarray[*numdata].per_line = per_line;
          if (((*numdata) && (dataarray[*numdata - 1].start < dataarray[*numdata].start)) || !*numdata)
            (*numdata)++;
          else if (*numdata)
            fprintf (stderr, "%s: '%s'\nWTEXT out of order\n\n", Progname, line);
        } else
          fprintf (stderr, "%s: Too many DATA/ASCII lines\n", Progname);
      } else if (!strncasecmp (line, "COMMENT ", 8)) {
          sscanf (&line[8], "%X", &dataline);
          k = 8;
          while (line[k] == ' ')
            k++;
          while (line[k] != ' ')
            k++;
          while (line[k] == ' ')
            k++;
          if (commarray[dataline]) {
            char *temp = malloc(strlen(commarray[dataline]) + 2 + strlen(&line[k]) + 1);
            sprintf(temp, "%s, %s", commarray[dataline], &line[k]);
            commarray[dataline] = temp;
          } else {
            commarray[dataline] = strdup(&line[k]);
          }
      } else if (!strncasecmp (line, "COMMENTLINE ", 12)) {
          sscanf (&line[12], "%X", &dataline);
          k = 12;
          while (line[k] == ' ')
            k++;
          while (line[k] != ' ')
            k++;
          while (line[k] == ' ')
            k++;
          if (commlinearray[dataline]) {
            char *temp = malloc(strlen(commlinearray[dataline]) + 1 + strlen(&line[k]) + 1);
            sprintf(temp, "%s\n%s", commlinearray[dataline], &line[k]);
            commlinearray[dataline] = temp;
          } else {
            commlinearray[dataline] = strdup(&line[k]);
          }
      } else if (line[0] == ';')        /* remark in data file */
        ;
      else if (strlen (line))
        fprintf (stderr, "%s: '%s'\nError in file '%s'\n\n", Progname, line, filename);
    }
    fclose (fp);
  } else
    fprintf (stderr, "%s: Can't open file '%s'\n", Progname, filename);
}

BOOL newl = FALSE;

char wchars[15] = {
  "<=-?!()',./&\":"
};

char wtext (int data) {
  if (data > 127) {
    newl = TRUE;       data -= 128;
  } else newl = FALSE;
  if (data < 10)       data += 48;
  else if (data == 10) data = 32;
  else if (data < 37)  data += 54;
  else if (data < 51)  data = (int) wchars[data - 37];
  else                 data = 39;
  return ((char) data);
}

BOOL diddata = FALSE;

// output the bytes of the complete opcode to asm or C file
int dumpdata (int opcode, int *curdata, int numdata, int *fp) {
  int pnum, tnum;
  int numoperands;
  int k;
  int numchars = 0;

  numoperands = 2 + dataarray[*curdata].end - count;
  pnum = dataarray[*curdata].per_line;  /* print up to pnum bytes data */
  if (dataarray[*curdata].type == DATA) {
    sprintf (out2, "%02X ", (UBYTE) opcode);
    strcat (out, out2);
    if ((pnum < 1) || (pnum > 24)) pnum = 16;                /* no more than 24 bytes hex data */
  } else {
    out2[0] = (dataarray[*curdata].type == ASCII) ? opcode : wtext (opcode);
    out2[1] = '\0';
    strcat (out, out2);
    if ((pnum < 1) || (pnum > 70)) pnum = 32;                /* no more than 70 bytes ASCII data */
  }
  newl = FALSE;
  tnum = (pnum > numoperands) ? numoperands - 2 : pnum - 1; // what was this for once?
  (void)tnum; // (now unused)
  for (k = 0; k < numoperands - 1; k++) {       /* print the data */
    if ((++numchars >= pnum) || newl) {
      sprintf (out, "%04X: ", count);
      numchars = 0;
      newl = FALSE;
    }
    if (dataarray[*curdata].type == DATA) sprintf (out2, "%02X ", getbyte (fp));    /* hex */
    else {
      out2[0] = (dataarray[*curdata].type == ASCII) ? getbyte (fp) : wtext (getbyte (fp)); /* ASCII */
      out2[1] = '\0';
    }
    strcat (out, out2);
  }
  if (*curdata < numdata) (*curdata)++;
  diddata = TRUE;
  return (numoperands - 2);
}

void freearrays (void) {
  if (labarray)      free (labarray);
  if (dataarray)     free (dataarray);
}

BOOL mallocarrays (void) {
  BOOL gotit = FALSE;
  int addr;
  for (addr = 0; addr < 0x10000; addr++) {
    commlinearray[addr] = NULL; commarray[addr] = NULL;
  }
  if ((labarray = (struct lastruct *) malloc (sizeof (struct lastruct) * MAXLABEL)))
    if ((dataarray = (struct dastruct *)malloc (sizeof (struct dastruct) * MAXDATA)))
      gotit = TRUE;
  if (!gotit)
    freearrays ();
  return (gotit);
}

char *fullequaddr = "%s EQU $%04X\n";
char *shortequaddr = "%s EQU $%02X\n";
int VECTREX = FALSE;          // Is target the Vectrex?
int Gen_C = FALSE;            // static binary translation to C wanted?
int Gen_Switch = FALSE;       // If generating C, do we write case labels or standard labels?
int Do_Timing = FALSE;
int Single_Step = FALSE;

int Accessible = FALSE; // shared with 6809.c

int main (int argc, char *argv[]) {
  int music, height, width, rel_y, rel_x, c;
  int i, j, k;
  int opcode, page;
  UBYTE operand[4];
  FILE *bin;
  int fetch_offset = 0;
  int *fp = &fetch_offset;
  int org = ~0; // GTDEBUG: aka -1. should never get to code generation with it set as this...
  opcodeinfo *op;
  int numoperands;
  int sp;
  int numdata = 0, curdata = 0;
  BOOL dobreak;
  char *s;

  Progname = argv[0];
  if ((s=strrchr(Progname, '/')) != NULL) Progname = s+1;

  for (;;) {

  if (argc == 1) {
    fprintf (stderr, "syntax: %s [--vectrex] [--sbt] [--single-step] [--timing] [--org 0xnnnn] file.bin [file.dat]\n\n", Progname); // does %X need xxxx or 0xnnnn ?
    exit (1);
  }

  if ( (strcmp (argv[1], "-h") == 0) || (strcmp (argv[1], "--help") == 0)) { // add full -d1 and -d 1 etc options?
    fprintf (stderr, "Usage: %s [--vectrex] [--sbt] [--single-step] [--timing] [--org 0xnnnn] file.bin [file.dat]\n", Progname);
    fprintf (stderr, "        %s disassembles 6809 binary files and optionally outputs\n", Progname);
    fprintf (stderr, "        an equivalent C program, which executes more quickly than.\n");
    fprintf (stderr, "        the same rom under a traditional emulator.\n\n\n");
    fprintf (stderr, "Examples:\n");
    fprintf (stderr, "        %s --vectrex polar_rescue.bin\n", Progname);
    fprintf (stderr, "        %s --sbt --vectrex polar_rescue.bin polar_rescue.dat\n", Progname);
    fprintf (stderr, "        %s --sbt --single-step --timing arcaderom.bin arcaderom.dat\n", Progname);
    fprintf (stderr, "\nOptions:\n");
    fprintf (stderr, "        --sbt         Generate C code (otherwise default to asm listing)\n");
    fprintf (stderr, "        --vectrex     Extra support for Vectrex cartridge roms\n");
    fprintf (stderr, "        --single-step or -ss  Support single-stepping\n");
    fprintf (stderr, "        --timing      Generate cycle timing\n");
    fprintf (stderr, "        --org 0xNNNN  Set the address of first byte\n");
    fprintf (stderr, "        --list        Show all opcodes supported\n");
    fprintf (stderr, "        -v            Report source module versions\n");
    fprintf (stderr, "        -d            Turn on debugging\n");
    fprintf (stderr, "        -l            Lables have an additional underscore\n");
    fprintf (stderr, "        -i0           INSTRUCTION_START not generated (default)\n");
    fprintf (stderr, "        -i1           INSTRUCTION_START allways generated\n");
    fprintf (stderr, "        -i2           INSTRUCTION_START generated on a natural break\n");
    fprintf (stderr, "        -i3           INSTRUCTION_START allways generated, if needed labels and PC are inserted\n");
    fprintf (stderr, "        -h            This help message\n\n");
    exit (1);
  }

  if (strcmp (argv[1], "--list") == 0) {        /* show all instructions (debug aid.) */
    for (i = 0; i < numops[0]; i++)
      printf ("opcode %02X, operands %d, name %6s, mode %s, cycles %d\n",
              pg1opcodes[i].opcode, pg1opcodes[i].numoperands, pg1opcodes[i].name, modenames[pg1opcodes[i].mode], pg1opcodes[i].numcycles);
    for (i = 0; i < numops[1]; i++)
      printf ("opcode 10 %02X, operands %d, name %6s, mode %s, cycles %d\n",
              pg2opcodes[i].opcode, pg2opcodes[i].numoperands, pg2opcodes[i].name, modenames[pg2opcodes[i].mode], pg2opcodes[i].numcycles);
    for (i = 0; i < numops[2]; i++)
      printf ("opcode 11 %02X, operands %d, name %6s, mode %s, cycles %d\n",
              pg3opcodes[i].opcode, pg3opcodes[i].numoperands, pg3opcodes[i].name, modenames[pg3opcodes[i].mode], pg3opcodes[i].numcycles);
    exit (0);
  }

  if (strcmp (argv[1], "--dump-comments") == 0) {        /* show all instructions (debug aid.) */
    DEBUG_COMMENTS = TRUE; // will dump comments and exit after reading dat file.
    argv += 1;
    argc -= 1;
    continue;
  }

  if (strcmp (argv[1], "-v") == 0) {
    fprintf(stderr, "%s\n", Dasm_Version);
    fprintf(stderr, "%s\n", SBT_Version);
    exit(EXIT_SUCCESS);
  }
  if (strcmp (argv[1], "-i0") == 0) {
    ADD_INSTRUCTION_MODE = 0;
    argv += 1;
    argc -= 1;
    continue;
  }
  if (strcmp (argv[1], "-i1") == 0) {
    ADD_INSTRUCTION_MODE = 1;
    argv += 1;
    argc -= 1;
    continue;
  }
  if (strcmp (argv[1], "-i2") == 0) {
    ADD_INSTRUCTION_MODE = 2;
    argv += 1;
    argc -= 1;
    continue;
  }
  if (strcmp (argv[1], "-i3") == 0) {
    ADD_INSTRUCTION_MODE = 3;
    argv += 1;
    argc -= 1;
    continue;
  }
  
  if (strcmp (argv[1], "-d") == 0) { // add full -d1 and -d 1 etc options?
    SBT_DEBUG = TRUE;
    argv += 1;
    argc -= 1;
    continue;
  }
  if (strcmp (argv[1], "-l") == 0) { // add full -d1 and -d 1 etc options?
    ALT_LABEL = TRUE;
    argv += 1;
    argc -= 1;
    continue;
  }
  
  if (strcmp (argv[1], "--vectrex") == 0) {
    VECTREX = TRUE;
    argv += 1;
    argc -= 1;
    continue;
  } else if (strcmp (argv[1], "--no-vectrex") == 0) {   /* is target using Vectrex BIOS or is it stand-alone? */
    VECTREX = FALSE;
    argv += 1;
    argc -= 1;
    continue;
  }

  if (strcmp (argv[1], "--sbt") == 0) {
    Gen_C = TRUE;
    argv += 1;
    argc -= 1;
    continue;
  } else if (strcmp (argv[1], "--no-sbt") == 0) {       /* generate C rather than disassemble (disassembly isn't reassemblable yet - still crude) */
    Gen_C = FALSE;
    argv += 1;
    argc -= 1;
    continue;
  }

  if ((strcmp (argv[1], "--single-step") == 0) || (strcmp (argv[1], "--ss") == 0)) {
    Single_Step = TRUE;
    argv += 1;
    argc -= 1;
    continue;
  } else if ((strcmp (argv[1], "--no-single-step") == 0) || (strcmp (argv[1], "--no-ss") == 0)) {       /* single-step support */
    Single_Step = FALSE;
    argv += 1;
    argc -= 1;
    continue;
  }

  if (strcmp (argv[1], "--timing") == 0) {
    Do_Timing = TRUE;
    argv += 1;
    argc -= 1;
    continue;
  } else if (strcmp (argv[1], "--no-timing") == 0) {       /* instruction timing. May be by single-step or agrregated in larger units */
    Do_Timing = FALSE;
    argv += 1;
    argc -= 1;
    continue;
  }
  
  if (strcmp (argv[1], "--org") == 0) {
    sscanf (argv[2], "%x", &org);
    count = org;
    argv += 2;
    argc -= 2;
    continue;
  }

  if (*argv[1] == '-') {
    fprintf(stderr, "%s: unknown option %s\n", Progname, argv[1]);
    exit(EXIT_FAILURE);
  } else {
    break;
  }
  
  }

  if (argc > 3) {
    fprintf (stderr, "syntax: %s [--vectrex] [--sbt] [--single-step] [--timing] [--org 0xnnnn] file.bin [file.dat]\n\n", Progname); // does %X need xxxx or 0xnnnn ?
    exit (1);
  }

  if (!mallocarrays ()) {
    fprintf (stderr, "%s: Can't get memory for arrays\n\n", Progname);
    exit (20);
  }
  
  if (VECTREX)
    readdatafile ("bios.dat", &org, &numlab, &numdata); // add bios vars


  // GTDEBUG moving this here in case flags were being overwritten at the point below where
  // this used to be done...
  for (i = 0; i <= 0xFFFF; i++) {
    F_[i] = F_OUTSIDE_FILE;
    DP[i] = -1;                 // inferred value of DP at this opcode
    codesize[i] = 0;            // how many bytes this opcode and operands take up
    hex[i] = "";                // the disassembly, for putting back in order
    decode[i] = "";             // the disassembly, for putting back in order
    comment[i] = "";            // ditto
  }

  if (org > -1) {               /* int PC to ORG val or 0 */
    count = org; // TO DO: start disassembling from ORG???
  } else {
    // GTDEBUG: fix org FFFF (unset) here...
    count = org = 0;
  }

  if (argc > 3) {
    fprintf (stderr, "Usage: %s <file> [<datafile>]\n", Progname);
    exit (10);
  }

  if (argc == 3) {
    readdatafile (argv[2], &org, &numlab, &numdata);    // rom-specific labels etc
    if (DEBUG_COMMENTS) {
      int addr;
      for (addr = 0; addr < 0x10000; addr++) {
        if (commarray[addr]) {
          fprintf(stdout, "%04X: %s\n", addr, commarray[addr]);
        }
        if (commlinearray[addr]) {
          fprintf(stdout, "%04X: %s\n", addr, commlinearray[addr]);
        }
      }
      exit(EXIT_SUCCESS);
    }
  }

  dumpf (R_Always, 0, "%19s ORG   $%04X", (Gen_C ? "//" : "  "), (org == -1 ? 0 : org));

  /* GT: for(k=0;k<numlab;k++) { // print labels first if(labarray[k].lab<256) printaddr=shortequaddr; else printaddr=fullequaddr;
     printf(printaddr,labarray[k].label,labarray[k].lab); }

     if(numlab) printf("\n"); */

  if ((bin = fopen ("system.img", "rb")) != NULL) {
    rom_end = 0x8000;           // exclusive
    for (;;) {
      int c = fgetc (bin);
      if (c == EOF)
        break;
      F_[rom_end] &= ~F_OUTSIDE_FILE;
      mem[rom_end++] = c;
    }
    fclose (bin);
  }

  if ((bin = fopen (argv[1], "rb")) == NULL) {
    // open binary file
    fprintf (stderr, "Can't open file '%s'\n", argv[1]);
    exit (1);
  }

  // GT: DEBUG when would org be < 0? Is this because of a signed 16 bit org?
  // maybe need to use unsigned or mask with ffff?  Should add a check here
  // to examine, if org < 0 ...
  if (org < 0) {
    fprintf(stderr, "BUG CHECK: why is org == 0x%04x ?\n", org);
  }

  // for now, this is not behaving as I hoped. I need to study the reentrancy of
  // scanning again with these flags non-default...
  //if (load_tags(argv[1])) { // should probably store this in the .dat file rather than using a .tags suffix...
  //}
    
  rom_end = (org < 0 ? 0 : org);                  // exclusive
  rom_start = rom_end;
  for (;;) {
    int c = fgetc (bin);
    if (c == EOF)
      break;
    F_[rom_end] &= ~F_OUTSIDE_FILE;
    F_[rom_end] |= F_NEED_DECODE;
    mem[rom_end++] = c;
  }
  fclose (bin);

  if (VECTREX) {
    char *s;
    char temp[1024];

    init_vectrex_bios();
    s = temp;
    s += sprintf (s, "%19s FCC   \"", (Gen_C ? "//" : "  "));
    for (;;) {                  // getstring80 (fp) Copyright
      c = getbyte (fp);
      if (c == 0x80)
        break;
      s += sprintf (s, "%c", c);
    }
    s += sprintf (s, "%c", '"');
    dumpf (R_Always, 0, "%s", temp);
    dumpf (R_Always, 0, "%19s FCB   $80", (Gen_C ? "//" : "  "));
    music = getword (fp);
    /* 
       music1 EQU 0xFD0D ;
       music2 EQU 0xFD1D ;
       music3 EQU 0xFD81 ;
       music4 EQU 0xFDD3 ;
       music5 EQU 0xFE38 ;
       music6 EQU 0xFE76 ;
       music7 EQU 0xFEC6 ;
       music8 EQU 0xFEF8 ;
       music9 EQU 0xFF26 ;
       musica EQU 0xFF44 ;
       musicb EQU 0xFF62 ;
       musicc EQU 0xFF7A ;
       musicd EQU 0xFF8F ;
     */
    s = checklabs ("", music, FALSE, FALSE, FALSE);
    if (*s == '\0') {
      dumpf (R_Always, 0, "%19s FDB   $%04X", (Gen_C ? "//" : "  "), music);
    } else {
      dumpf (R_Always, 0, "%19s FDB   %s", (Gen_C ? "//" : "  "), s);
    }
    for (;;) {
      height = getbyte (fp);
      if (height == 0) break;
      width = getbyte (fp);
      dumpf (R_Always, 0, "%19s FCB   $%02X, $%02X                ; h,w of raster image", (Gen_C ? "//" : "  "), height, width);
      rel_y = getbyte (fp);
      rel_x = getbyte (fp);
      dumpf (R_Always, 0, "%19s FCB   $%02X, $%02X                ; y,x of raster image", (Gen_C ? "//" : "  "), rel_y, rel_x);
      sprintf (temp, "%19s FCC   \"", (Gen_C ? "//" : "  "));
      s = temp + strlen(temp);
      for (;;) {
        c = getbyte (fp);
        if (c == 0x80) break;
        *s++ = c;
        //s += sprintf (s, "%c", c);
      }                         // getstring80 (fp) Text message
      *s++ = '"'; *s = '\0';
      //s += sprintf (s, "%c", '"');
      dumpf (R_Always, 0, "%s", temp);
      dumpf (R_Always, 0, "%19s FCB   $80", (Gen_C ? "//" : "  "));
    }
  dumpf (R_Always, 0, "%19s FCB   $00", (Gen_C ? "//" : "  "));
  {int i = *fp;
    do {
      i -= 1;
      F_[i] &= ~F_NEED_DECODE;
    } while (i != 0);
  }
  queue[next_queue_item++] = (*fp);     // start of code is first seed. (on Vectrex. With arcade games, use start vector etc))
  F_[(*fp)] |= F_LABEL;
  dobreak = FALSE;              // used for end of a straight-line recursion
  } else {
    // not vectrex
    (*fp) = org;
    queue[next_queue_item++] = org;     // start of code is first seed.
    // NOTE: when translating Atari vector arcade cabinets, use the [RESET] vector contents as the first entry point.
    F_[org] |= F_LABEL;
    dobreak = FALSE;              // used for end of a straight-line recursion
  }

  while (next_seed > 0) {
    queue[next_queue_item++] = seed[--next_seed];
    //    F_[seed[next_seed]] |= F_LABEL; // WRONG!!! "code nnnn" does not imply a label!
  }

  while (next_queue_item > 0) {
    *fp = queue[--next_queue_item];
    //fprintf(stderr, "DEBUG: explore %4X\n", *fp);
    if (F_[*fp] & (F_DATA | F_OUTSIDE_FILE | F_INVALID_DECODE | F_VALID_DECODE)) { continue; }
    while ((opcode = getbyte (fp)) != EOF) {
      int current_address = (*fp) - 1;

      if (F_[current_address] & (F_DATA | F_OUTSIDE_FILE | F_INVALID_DECODE | F_VALID_DECODE)) { break; } // recursion stoppers
      count = current_address + 1;
      op = NULL;

      // printf("%s", checklabs("",count-1,TRUE,FALSE,FALSE)); /* if add lab, print */
      sprintf (out, "%04X: ", count - 1);       /* print PC */

#ifdef REMOVED_WHILE_LOCATING_SPURIOUS_ERRORS
      if (numdata) {
        while (((count - 1) > dataarray[curdata].end) && (curdata < numdata)) {
          fprintf (stderr, "%s: missed a data line, start=%X, end=%X\n", Progname, dataarray[curdata].start, dataarray[curdata].end);
          curdata++;
        }
      }
#endif
      if (numdata && ((count - 1) >= dataarray[curdata].start) && ((count - 1) <= dataarray[curdata].end)) { /* data? */
        numoperands = dumpdata (opcode, &curdata, numdata, fp);
        i = numops[0] + 1;      /* skip decoding as an opcode */
      } else {                  /* not data - search for opcode */
        sprintf (out2, "%02X ", (UBYTE) opcode);
        strcat (out, out2);
        for (i = 0; (i < numops[0]) && (pg1opcodes[i].opcode != opcode); i++) ;

        if (i < numops[0]) {    /* opcode found */
          if (pg1opcodes[i].mode >= PG2) {      /* page switch */
            opcode = getbyte (fp);
            sprintf (out2, "%02X ", (UBYTE) opcode);
            strcat (out, out2);
            page = pg1opcodes[i].mode - PG2 + 1;        /* get page # */
            for (k = 0; (k < numops[page]) && (opcode != pgpointers[page][k].opcode); k++) ;
            if (k != numops[page]) {    /* opcode found */
              op = (opcodeinfo *) & (pgpointers[page][k]);
              numoperands = pgpointers[page][k].numoperands;
              for (j = 0; j < numoperands - 1; j++) {
                sprintf (out2, "%02X ", (operand[j] = getbyte (fp)));
                strcat (out, out2);
              }

              if (pgpointers[page][k].mode != IND) {
                for (sp = strlen (out); sp < OPNAMETAB; sp++) strcat (out, " ");
                sprintf (out2, "%s", pgpointers[page][k].name);
                strcat (out, out2);
                for (sp = strlen (out); sp < OPERANDTAB; sp++) strcat (out, " ");
              }
              printoperands (current_address, opcode, numoperands - 1, operand, pgpointers[page][k].mode, fp, pgpointers[page][k].name, out);
              F_[current_address] |= F_VALID_DECODE;
            } else {            /* not found in alternate page */
              for (sp = strlen (out); sp < OPNAMETAB; sp++) strcat (out, " ");
              strcat (out, "Illegal Opcode");
              F_[current_address] |= F_INVALID_DECODE;
              break;
            }
          } else {              /* page 1 opcode */
            op = (opcodeinfo *) & (pg1opcodes[i]);
            numoperands = pg1opcodes[i].numoperands;
            for (j = 0; j < numoperands; j++) {
              sprintf (out2, "%02X ", (operand[j] = getbyte (fp)));
              strcat (out, out2);
            }
            if (pg1opcodes[i].mode != IND) {
              for (sp = strlen (out); sp < OPNAMETAB; sp++) strcat (out, " ");
              sprintf (out2, "%s", pg1opcodes[i].name);
              strcat (out, out2);
              for (sp = strlen (out); sp < OPERANDTAB; sp++) strcat (out, " ");
            }
            printoperands (current_address, opcode, numoperands, operand, pg1opcodes[i].mode, fp, pg1opcodes[i].name, out);
            F_[current_address] |= F_VALID_DECODE;
          }
        } else if (i == numops[0]) {    /* not found in page 1 */
          for (sp = strlen (out); sp < OPNAMETAB; sp++) strcat (out, " ");
          strcat (out, "Illegal Opcode");
          F_[current_address] |= F_INVALID_DECODE;
          break;
        }
        F_[current_address] |= F_VALID_DECODE;

      }
      if (op) {
        if ((!strcasecmp (op->name, "BRA")) ||  /* extra space - branch */
            (!strcasecmp (op->name, "LBRA")) ||
            (!strcasecmp (op->name, "RTS")) ||
            (!strcasecmp (op->name, "JMP")) ||
            (!strcasecmp (op->name, "RTI")) ||
            (!strncasecmp (op->name, "PUL", 3) && PC) ||   /* PUL? PC=RTS */
            (!strcasecmp (op->name, "WAI"))) {
          if (strlen (out) && (out[strlen (out) - 1] != '\n'))
            /* strcat(out,"\n") */
            F_[current_address] |= F_ENDSECTION; // add in output code
          dobreak = TRUE;
        }
      }
      if (diddata) {
        if (strlen (out) && (out[strlen (out) - 1] != '\n')) strcat (out, "\n");
      }
      save (out);
      PC = FALSE;
      diddata = FALSE;
      if (dobreak) break;
    }
    dobreak = FALSE;
  }
  memory = mem;                 // for SBT

#ifdef NEVER
  // when the code is stabilised we might insert the actual working header here...
  if (Gen_C) {
    dumpf (R_Always, 0, "#include <stdint.h>");
    dumpf (R_Always, 0, "int main(int argc, char **argv) {");
    dumpf (R_Always, 0, "  typedef uint32_t UINT32;");
    dumpf (R_Always, 0, "  typedef int32_t  SINT32;");
    dumpf (R_Always, 0, "  typedef uint16_t UINT16;");
    dumpf (R_Always, 0, "  typedef int16_t  SINT16;");
    dumpf (R_Always, 0, "  typedef uint8_t  UINT8;");
    dumpf (R_Always, 0, "  typedef int8_t   SINT8;");
    dumpf (R_Always, 0, "");
    dumpf (R_Always, 0, "  UINT8 memory[0x10000], *memory_DP = &memory[0xD000];");
    dumpf (R_Always, 0, "");
    dumpf (R_Always, 0, "  // Modify this header to suit your target...");
    dumpf (R_Always, 0, "  SINT32 res, A, B, D, C;");
    dumpf (R_Always, 0, "  UINT16 PC, X, Y, S, U, Z, DP = 0xD0, arg, ea, msn, lsn, val;");
    dumpf (R_Always, 0, "  UINT8  N, H, V, CC;");
    dumpf (R_Always, 0, "");
    dumpf (R_Always, 0, "#define JUMP do {} while (0)");
  }
#endif
  
  // GT DEBUG: need to skip over the number of bytes used by each instruction:
  // for (i = org; i <= 0xFFFF; i++) {
  // !! This seemed to fix some of the test examples...
  if ((org&0xFFFF) == 0xFFFF) {
    fprintf(stderr, "%s: DEBUG - Org FFFF should never happen\n", Progname);
    org = 0;
  }
  for (i = org; i <= 0xFFFF; i+=(codesize[i] <= 0 ? 1 : codesize[i])) { 
    static char line[512], *lp;
    char *s;

    if ((i < rom_end) && commlinearray[i]) { // whole-line omments...
      char *s = commlinearray[i];
      for (;;) {
        char *line = s;
        s = strchr(s, '\n');
        if (s) *s++ = '\0';
        dumpf (R_Always, 0, "%s %s", (Gen_C ? "//" : ";"), line);
        if (!s) break;
      }
    }

    line[0] = '\0';
    lp = line;
    if (codesize[i] > 0) {
      {int j; for (j = 1; j < codesize[i]; j++) F_[i+j] &= ~F_NEED_DECODE;}
      s = checklabs ("", i, FALSE/*TRUE*/, FALSE, TRUE/*FALSE*/); // third param adds ":\n" if true; last param says allow decode of 00-ff whether zp or not
      if (Single_Step) {
        if (Accessible) {
          Accessible = FALSE; //lp += sprintf (lp, "PC = 0x%04X; JUMP; // sstep\n", i);
        }
      }
      if ((F_[i] & F_LABEL) || Single_Step) 
      {
        assert(F_[i]&F_VALID_DECODE);
        // flush((unsigned long)-1L); don't need to flush for a label, only at the jump *to* a label...
        if (Gen_C) 
        {
          Accessible = TRUE; 
          lp += sprintf (lp, "case 0x%04X:\n", i); // skip this if checklabs came up with one???
          if (!Single_Step) 
	  {
	    if (ALT_LABEL)
	      lp += sprintf (lp, "L%04X_:\n", i);      // skip this if checklabs came up with one???
	    else
	      lp += sprintf (lp, "L%04X:\n", i);      // skip this if checklabs came up with one???
	    
	  }

	  {
            char copy[32];
	    if (ALT_LABEL)
	      sprintf (copy, "L%04X_", i);
	    else
	      sprintf (copy, "L%04X", i);
            if ((*s != '\0') && (strncasecmp(copy,s,5) != 0)) 
            {
                char *t; 
                t = strchr(s, ' '); 
                if (t) 
                  *t = '\0'; // TEMP HACK starwars BUG
                if (!Single_Step) 
		{
		  if (ALT_LABEL)
		    lp += sprintf (lp, "%s_:\n", s);      // skip this if checklabs came up with one???
		  else
		    lp += sprintf (lp, "%s:\n", s);      // skip this if checklabs came up with one???
		}
		  
		  
		  
            }
          }
          lp += sprintf (lp, "               ");//align...
        } 
        else 
        {
          if (*s == '\0') 
          {
            lp += sprintf (lp, "L%04X         ", i);      // skip this if checklabs came up with one???
          } 
          else 
          {
            lp += sprintf (lp, "%-12s ", s);      // skip this if checklabs came up with one???
          }
        }
      } 
      else 
      {
        lp += sprintf (lp, "             ");
      }
      if (Gen_C) 
      {//gtdebug
        char *labname = checklabs ("", i, FALSE/*TRUE*/, FALSE, TRUE/*FALSE*/);
        // previously the asm was just a comment, now it is wrapped by mon() which can
        // optionally be implemented as a macro which throws its parameter away, or as
        // a procedure that prints its parameter to a log file or a cyclic buffer...
        lp += sprintf (lp, "                                      #ifdef DEBUG\n");
        lp += sprintf (lp, "                                      mon(\"%-8s %-30s; %04X: %s\\n\");\n", F_[i]&F_LABEL ? labname : "", s = decode[i], i, hex[i]);
        lp += sprintf (lp, "                                      #endif");

        if (commarray[i]) { // comments next to instructions
          lp += sprintf(lp, " // %s", commarray[i]);
        }
        
        // GTDEBUG:
        if ((i & 0xFFFF) == 0xFFFF) 
        {
          fprintf(stderr, "%s: DEBUG INFO: i = 0x%08x\n", Progname, i);
          lp += sprintf (lp, " /* DBG:%08x */", i);
        }
        
      } 
      else 
      {
        lp += sprintf (lp, "       %-30s; %04X: %s", s = decode[i], i, hex[i]);
        if (commarray[i]) 
        { // comments next to instructions
          lp += sprintf(lp, "  // %s", commarray[i]);
        }
      }
      if ((strncmp (s, "BSR", 3) == 0)
          || (strncmp (s, "LBSR", 4) == 0)
          || (strncmp (s, "JSR", 3) == 0)
          || (strncmp (s, "SWI", 3) == 0)
          || (strncmp (s, "CWAI", 4) == 0)
          || (strncmp (s, "SYNC", 4) == 0)) {
        F_[i + codesize[i]] |= F_LABEL;  // just in time :-) For SBT
      }
      if (*comment[i] != '\0') 
        lp += sprintf (lp, "    // %s", (*comment[i] == ';' ? comment[i]+1 : comment[i])); // comments added by this code, not .dat file

      dumpf (R_Always, 0, "%s", line);

      if (Gen_C) 
      {
        translate6809 (i, i + codesize[i], 999); // codesize isn't just a simple static lookup, needs a full inst decode.
      }
        
      if (F_[i] & F_ENDSECTION) 
      {
        flush(R_FLUSH);
        dumpf (R_Always, 0, "");        // optional newline for JMP/BRA/RTS etc
      }
    } 
    else if (F_[i]&F_NEED_DECODE) 
    {
      extern char *disasm(unsigned char *memory, unsigned int *pc, int showhex, int showaddr);
      unsigned int pc_copy = i;
      // ignore data we know is data - print data that is probably undecoded code...
      if ((F_[i]&F_DATA) == 0) dumpf (R_Always, 0, "%19s FCB   $%02X%22s %04X: %s '%c' %s ; %s", (Gen_C ? "//" : "  "),
             memory[i], ";", i, hex[i],
             isprint(memory[i]) ? memory[i] : ' ',
             disasm(memory, &pc_copy, TRUE, FALSE),
             (commarray[i] == NULL ? "" : commarray[i]) /* comments next to data*/);
    }
  }
  flush (R_Always);
  
  // save for use by fallback interpreter.  currently not for use by this program.
  save_tags(argv[1]);

  freearrays ();
  return 0;
}
