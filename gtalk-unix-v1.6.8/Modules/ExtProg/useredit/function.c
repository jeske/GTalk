
/* FUNCTION.C */



/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

/* INCLUDES */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int get_yes_no(char *string)
 {
   char command[5];
   int flag = 1;
   while (flag)
    {
      print_string(string);
	  print_string(" (Y/N): ");
      do
        {
         get_string(command,1);
        } while (!(*command));
      if (*command>'Z') *command -= 32;
      if ((*command=='Y') || (*command=='N')) flag=0;
    };
   return (*command=='Y');
 };


int g_strncmp(char *string1,char *string2,int len)
{
  if (!len)
   return 0;

  while (((*string1)==(*string2)) && (--len))
   {
	 string1++;
	 string2++;
  }

  return ((*string1) - (*string2));

}

void shorten(char *string,int len)
{
  int temp_len;

            /* LIMIT THE LENGTH but only printed legnth */
        temp_len=strlen(string)-1;

        while (ansi_strlen(string)>len)
         { temp_len--;
           string[temp_len]=0;
         }

}


char valid_class_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_-1234567890";

void fix_classname(char *str)
{
	char temp;

	while (*str)
	{

	  temp = toupper(*str);

 	  if (strchr(valid_class_chars,temp)==NULL)
	   {
		str_cpy(str,str+1);
	   }
	  else
	   {
		 *str = temp;
		 str++;
	   }
	}

}

void repeat_chr(char chr,int times,char print_a_cr)
{
   if (times<0) return;

   while (times--)
    print_chr_noflush(chr);

   if (print_a_cr)
     print_cr();

   fflush(stdout);
}


int wait_for_return(void)
{
  int count;
  int abort=0;

    print_string("[ Press Return ]");
    do
     {
       count = wait_ch();
       if (((count == 27) || (count == 3)))
        {
         if (count==27)
            abort=1;
         count = 13;
        };
     } while (count != 13);
  print_cr();
  return abort;
}

int ansi_strlen(char *str)
{
   int len = 0;

   while (*str)
    {
        if ((*str=='|')&&(*(str+1)=='*') && (*(str+2)!=0) && (*(str+3)!=0))
           str += 4;
        else
        if ((*str=='|')&&(*(str+1)=='+') && (*(str+2)!=0) && (*(str+3)!=0))
           {
             str += 4;
             len += 1;
           }
        else
           {len++;
            str++;
           }
    }
  return len;
}


/* string time function */

void str_time(char *str, int legnth,struct tm *now)
{ int hour;
  char light='a';
  char s[250];

  hour=now->tm_hour;

  if (hour>=12)
    {
      light='p';
      if (hour>12)
         hour=hour-12;
    }

  if (!hour) hour=12;

  sprintf(s,"% 2d:%02d:%02d %cm ",hour,now->tm_min,now->tm_sec,light);
  strncpy(str,s,legnth);
  str[legnth-1]=0;


}


