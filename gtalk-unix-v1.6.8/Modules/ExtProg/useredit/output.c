
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - output.c
 *
 * General Output routines
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>

#include <pwd.h>

#include "ansi.h"
#include "output.h"

int more_file(char *progname)
{
  FILE *fp;

  if (fp=fopen(progname,"r"))
    {
      ansi_on(1);
      while (!feof(fp))
	print_chr(fgetc(fp));
      fclose(fp);
      ansi_on(0);
      return (0);
    }
  return (-1);
}

void print_file(char *string)
{
  printf("-------: %s\r\n",string);
  more_file(string);

}

void set_scrolling_region(int x1,int x2,int portnum)
{
  char s[20];

  /* if (is_console_node(portnum)) */

  sprintf(s,"%c[%d;%dr",27,x1,x2);
  send_string_noflush(portnum,s);

}

void erase_region(int x1,int x2,int portnum)
{ char s[20];

 if (x1==x2)
   return;

 if (x1>x2)
  { int temp = x1;
    x1 = x2;
    x2 = temp;
  }



 set_scrolling_region(x1,x2,portnum);
 position(x2,0);
 for (;x1<x2;x1++)
 {
   print_cr();
 }
 print_cr();
 set_scrolling_region(0,24,portnum);
 position(x1,0);

}











