
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

int sprint_time(char *strbuf,time_t *time)
{   
    struct tm *temp_time;
    int temp;

    /* IF THE TIME is NULL then print - None - */
    if (!*time)
      { strcpy(strbuf,"- None -");
                return (strlen(strbuf));
      }

    temp_time=localtime(time);
    temp = strftime(strbuf,39,"%a %b %d, %Y at %I:%M ",temp_time);
   
    if ((temp_time->tm_hour>11) && (temp_time->tm_hour!=0))
	strcat(strbuf,"pm");
    else
        strcat(strbuf,"am");

    temp = strlen(strbuf);

    return temp;
}




int str_to_num(char *str, char **point)
{
    char s[256];
    char *new;


    *point=str;

    /* FIRST.. please remember to REMOVE all the space before it */
    while (**point==32)
      (*point)++;

    if ((((**point)<48) || ((**point)>57)))
      return -1;

    new=s;

    while(((**point>=48) && (**point<=57)) && ((new-s)<4) )
       {
         *new++=**point;
         (*point)++;

         }
    *new=0;
    return atoi(s);

}


int get_yes_no(char *string)
 {
   char command[5];
   int flag = 1;
   while (flag)
    {
      print_string(string);
      print_string(" Yes/No (Y/N): ");
      do
        {
         get_input(command,1);
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


char *limit_carrots(char *str,int max_carrots)
{
   char *temp=str;

   while (max_carrots && *temp)
    if (*(temp++)=='^') max_carrots--;

   while (*temp)
	if (*temp++=='^') *(temp-1)=' ';

  return str;
}

