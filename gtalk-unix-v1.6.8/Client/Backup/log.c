/*
   Log.c
*/

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "log.h"
#include "gtmain.h"

#define STRFTIME_CONFIG_STRING "(%a) %m/%d/%y %H:%M:%S"

int file_log_message(char *filename,char *format, va_list ap)
{
  FILE *fp;
  time_t tim = time(NULL);
  char time_string[50];
  int retval;

  if ((fp=fopen(filename,"a")))
    {
      vfprintf(fp,format,ap);
      fprintf(fp,"\n");
      fclose(fp);
    }
  else
    retval=-1;
  
  return (retval);
}


int log_event(char *filename,char *format, ...)
{
  va_list ap;
  FILE *fp;
  time_t tim = time(NULL);
  char time_string[50];
  int retval;

  strftime(time_string,33,STRFTIME_CONFIG_STRING,localtime(&tim));

  if ((fp=fopen(filename,"a")))
    {
      fprintf(fp,"[%02d] %s ",mynum,time_string,getpid());
      va_start(ap,format);
      vfprintf(fp,format,ap);
      va_end(ap);
      fprintf(fp,"\n");
      fclose(fp);
    }
  else
    retval=-1;
  

  
  return (retval);
}

int log_system_event(char *filename,char *format, ...)
{
  va_list ap;
  FILE *fp;
  time_t tim = time(NULL);
  char time_string[50];
  int retval;

  strftime(time_string,33,STRFTIME_CONFIG_STRING,localtime(&tim));

  if ((fp=fopen(filename,"a")))
    {
      fprintf(fp,"[%02d] %s (pid %d) ",mynum,time_string,getpid());
      va_start(ap,format);
      vfprintf(fp,format,ap);
      va_end(ap);
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
  FILE *fp;
  time_t tim = time(NULL);
  char time_string[50];
  int retval;

  strftime(time_string,33,STRFTIME_CONFIG_STRING,localtime(&tim));

  if ((fp=fopen(LOG_FILENAME,"a")))
    {
      fprintf(fp,"[%02d] %s ",mynum,time_string,getpid());
      va_start(ap,format);
      vfprintf(fp,format,ap);
      va_end(ap);
      fprintf(fp,"\n");
      fclose(fp);
    }
  else
    retval=-1;
  
}
