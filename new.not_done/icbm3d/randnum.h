/*
  randnum.h
  
  Returns a random number.
  
  By Bill Kendrick
  kendrick@zippy.sonoma.edu
  http://zippy.sonoma.edu/~kendrick/
  
  September 8, 1996 - September 10, 1996
*/

/* Call first to randomly (time-based) seed the random number generator */

void randinit();


/* Returns a random number between 0 and mod-1 */

int randnum(int mod);
