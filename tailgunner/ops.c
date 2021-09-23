#include <stdio.h> /* ONLY for use of stderr at the end of this file.  Can be removed */
#include <stdlib.h> /* ONLY for use of exit() at the end of this file.  Can be removed */
#include "externs.h"
#include <vectrex/vectrexInterface.h>
/*
void showregs(void) {
  int x,y;
  for (y = 0; y < 16; y++) {
    for (x = 0; x < 16; x++) {
      fprintf(stdout, " %03x", ram[(y<<4)+x]);
    }
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
  fflush(stdout);
}
*/

extern int plaqueCounter;
void cineExecute(void) {
  static int old_PC = 0x0000;

    switch (register_PC) {
#include "tailgunr-ops.c"
default:
      /* Jumping to any illegal address or end of eprom will come here */
      /* we ought to reinitialise or something */
      fprintf(stderr, "*** Uncaught jump to 0x%04x from 0x%04x!\n", register_PC, old_PC);
#ifndef FREESTANDING
      exit(0);
#endif
    }
}