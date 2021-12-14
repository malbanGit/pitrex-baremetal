#include <stdlib.h>

#include "rand.h"

// dummy time
#define time(a) ((a)*2) 

//use this first function to seed the random number generator, 
//call this before any of the other functions 
void initrand() 
{ 
    srand((unsigned)(time(0))); 
    //srand(rand()*GetTickCount());

} 

//generates a psuedo-random integer between 0 and 32767 
int randint() 
{ 
    return rand(); 
} 

//generates a psuedo-random integer between 0 and max 
int randintm(int max) 
{ 
   return (int)(rand()%(max+1)); 
} 

//generates a psuedo-random integer between min and max 
int randintmm(int min, int max) 
{ 
    if (min>max) 
    { 
        return (int)(rand()%(min-max+1)); 
    } 
    else 
    { 
        return (int)(rand()%(max-min+1)); 
    } 
} 

//generates a psuedo-random float between 0.0 and 0.999... 
float randfloat() 
{ 
    return rand()/((float)(RAND_MAX)+1); 
} 

//generates a psuedo-random float between 0.0 and max 
float randfloatm(float max) 
{ 
    return randfloat()*max; 
} 

//generates a psuedo-random float between min and max 
float randfloatmm(float min, float max) 
{ 
    if (min>max) 
    { 
        return randfloat()*(min-max)+max;     
    } 
    else 
    { 
        return randfloat()*(max-min)+min; 
    }     
} 

//generates a psuedo-random double between 0.0 and 0.999... 
double randdouble() 
{ 
    return rand()/((double)(RAND_MAX)+1); 
} 

//generates a psuedo-random double between 0.0 and max 
double randdoublem(double max) 
{ 
    return randdouble()*max; 
} 

//generates a psuedo-random double between min and max 
double randdoublemm(double min, double max) 
{ 
    if (min>max) 
    { 
        return randdouble()*(min-max)+max;     
    } 
    else 
    { 
        return randdouble()*(max-min)+min; 
    } 
} 
