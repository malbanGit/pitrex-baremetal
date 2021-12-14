#ifndef __DASM6809__
#define __DASM6809__ 1

// The primary use of this array is within 6809dasm.c where
// 

// The results of code analysis will now be made available
// in a persistent blob.  The sbt execution can also use this
// to determine if it is being asked to execute code that has
// not been recognised and translated, and invoke a fallback
// emulator.  The fallback emulator will add to this data store
// so that the next translation will pick up those missing
// code segments.

// future plans for a new and more generic tree-walker will require a lot
// more than the current 8 flags.  So we'll use a 32-bit int to allow extension!
extern unsigned int F_[0x10000];

#define F_VALID_DECODE 1        // all executable addresses should have this set.
#define F_INVALID_DECODE 2      // shouldn't get any of these during initial scan from known starting point
#define F_VISITED 4
#define F_LABEL 8               // this instruction should have a label before it
#define F_ENDSECTION 16
#define F_NEED_DECODE 32        // print something whether code or data
#define F_OUTSIDE_FILE 64       // exploration stopper
#define F_DATA 128              // not code

// if ((F[PC]&F_VALID_DECODE) && (F[PC]&F_LABEL)) ... there is an entry point in the switch table for this address...

// The actual data array and the loading and saving of it have been split off into
// this separate module, to allow it to also be used by by the fallback interpreter

extern int load_tags(char *binname); // procedure will remove the .bin or whatever extension
extern int save_tags(char *binname);

#endif
