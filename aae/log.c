#include <stdio.h>
#include <stdarg.h>

#ifndef NO_PI
#include <vectrex/vectrexInterface.h>
#endif

#include "log.h"


FILE *logfile;

int open_log(char *filename)
{
 if ((logfile = fopen(filename, "w")) != NULL) return 0; /* Opened succesfully. */
 
 return -1; /* Failed. */
}


char logbuf[1024];
int log_it(char *format, ...)
{
 va_list ptr; /* get an arg pointer */
 int status = -1;
 va_start(ptr, format);
 vsprintf (logbuf, format, ptr); 

 printf("%s\n", logbuf); // New line..

 if (logfile != NULL)
 {
  /* initialize ptr to point to the first argument after the format string */
 
  /* Write to logfile. */
  /*status = */fprintf(logfile, "%s\n", logbuf); // Write passed text.
 
  va_end(ptr);
 
  /* Flush file. */
  fflush(logfile);
 }

 return status;
}

int close_log(void)
{
 int status = -1;
 
 log_it("Closing log, ending program.");
 if (logfile != NULL)
 {
  fflush(logfile);
  fclose(logfile);
 }
 
 return status;
}