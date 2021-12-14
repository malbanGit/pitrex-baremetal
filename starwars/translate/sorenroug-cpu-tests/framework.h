#include "cputest.h"

/* Allocate enough memory to run the instructions in that allows
 * for jumps back and forth. Stack pointer is set at top.
 * initregs must follow immediately.
 */

char *currtest;


#define CODESTRT 0

/* Small code segment to save registers and call test.
 */
extern unsigned char *ctlmemory; /* WARNING! Host byte-sex sensitive */
/* ctlmemory = &memory[MEMSIZE]; */

/* Store the initial value of the stack pointer. */
unsigned short *save_s;
unsigned short *dpLoc;

extern void systemf1(const char *command, const char *param);
/**
 * Copy the instructions to test.
 * Instructions must begin at CODESTRT
 * The instructions must end with an RTS
 */
void copydata(int start,
              char *insv,
              int insc);

void writebyte(int loc,
               char value);

unsigned setMem(int addr,
                char value);

void writeword(int loc,
               unsigned value);

/*
 * Start location is the argument on the stack.
 */
void jsrtest(int startloc);

void runcode(char *ctest,
             char *insv,
             int insc);


void runtest(char *ctest);

int setupCtl(void);

void printMem(int start, int  size);

void printCtl(int ctlsize);

/**
 * Read a value from the direct page.
 */
int readDPloc(int offset);

/**
 * Write a value into the direct page.
 */
void writeDPloc(int offset,
                unsigned char value);
