/*
   Log.c
*/

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "log.h"

#define STRFTIME_CONFIG_STRING "(%a) %m/%d/%y %H:%M:%S"

int file_log_message(char *filename,char *format, va_list ap)
{
  FILE *fp;
  time_t tim = time(NULL);
  char time_string[50];
  int retval;


  strftime(time_string,33,STRFTIME_CONFIG_STRING,localtime(&tim));

  if ((fp=fopen(filename,"a")))
    {
      fprintf(fp,"%s (pid %d) ",time_string,getpid());
      vfprintf(fp,format,ap);
      fprintf(fp,"\n");
      fclose(fp);
    }
  else
    retval=-1;
  

  
  return (retval);
}

void log_error(char *format,...)
{
  va_list ap;

  va_start(ap, format);
  file_log_message(LOG_FILENAME,format,ap);
  va_end(ap);
}
