
/* FUNCTION.C */



/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* INCLUDES */
#include "include.h"
#include "gtalk.h"

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


void str_cpy(char *to,char *from)
{
     do
     {
       *to++=*from++;
     }
     while (*from);
     *to=0;
}


void repeat_chr(char chr,int times,char print_a_cr)
{

   if (times<0) return;

   while (times--)
    print_chr(chr);

   if (print_a_cr)
     print_cr();
}


int nodes_free(void)
{
 int loop;
 int count=0;
 int can_see_lurk=test_bit(user_lines[tswitch].privs,LURK_PRV);


 for (loop=0;loop<=sys_info.max_nodes;loop++)
    if (line_status[loop].online &&  ( (!line_status[loop].lurking) || (can_see_lurk) || (loop==tswitch)))
         if ((port[loop].dial_in_line) && (!is_console_node(loop)))
             count++;

 return (sys_toggles.num_dial_ins-count);

}


#define GREP_BUF_SIZE 512

void grep_file(char *filename, char *match_string)
{
  FILE *fp;
  int lockflag = !islocked(DOS_SEM);
  char *buffer = g_malloc(GREP_BUF_SIZE,"GREP");
  char *endbuf;
  char *curbufptr = buffer;
  unsigned long int absolute_position=0;
  unsigned long int absolute_pos_of_begin_line=0;
  char *curmatchptr = match_string;
  char *substringptr = match_string;
  char *substringptr2 = match_string;
  char *substringsave;
  int flag;
  char *match_stringend = match_string;

  /* find the end of the match_string first */
  while (*match_stringend)
    match_stringend++;

  if (!buffer) return;

  if (lockflag) lock_dos(790);
  if (!(fp=g_fopen(filename,"rb+","GREP")))
  {
    if (lockflag) unlock_dos();
    print_str_cr("Failed to open file");
    g_free(buffer);
    return;
  }
  memset(buffer,0,GREP_BUF_SIZE);

  flag = fread(buffer,sizeof(char),GREP_BUF_SIZE,fp);
  endbuf = &buffer[flag];

  if (lockflag) unlock_dos();

  while(flag)
  {

          while (flag && (curmatchptr < match_stringend))
          {
               if (*curbufptr==10)
                 absolute_pos_of_begin_line = absolute_position;


               if (*curbufptr == *curmatchptr)
                   {
                          curmatchptr++;
                        /* advance the pointers */
                        curbufptr++;
                        absolute_position++;

                   }
               else
                {
                    /* try for a substring match */


                    substringsave = match_string;

                    do
                    {

                        substringptr2 = match_string;
                        substringptr = ++substringsave;

                        while (*substringptr != *substringptr2)
                         substringptr++;

                        substringsave = substringptr;

                        while ((*substringptr==*substringptr2) && (substringptr<curmatchptr))
                         { substringptr++;
                           substringptr2++;
                         }

                    }
                    while ((substringptr < curmatchptr));

                    curmatchptr = substringptr2;


               }

               if ((curmatchptr==match_string) && (curbufptr<endbuf))
                {
                  curbufptr++;
                  absolute_position++;
                }

                        if (curbufptr==endbuf) /* read the next block */
                          {
                              if (lockflag) lock_dos(791);
                              flag = fread(buffer,sizeof(char),GREP_BUF_SIZE,fp);
                              if (lockflag) unlock_dos();
                              endbuf = &buffer[flag];
                              curbufptr = buffer;
                          }

             }

     if (curmatchptr!=match_stringend)
      {
        g_free(buffer);
        g_fclose(fp);
        return;
      }

     curmatchptr = match_string;

     /* ok, we found the string, now read starting at that position */

     if (lockflag) lock_dos(792);
     fseek(fp,absolute_pos_of_begin_line,SEEK_SET);
     absolute_position = absolute_pos_of_begin_line;
     flag = fread(buffer,sizeof(char),GREP_BUF_SIZE,fp);
     if (lockflag) unlock_dos();
     endbuf = &buffer[flag];

     curbufptr = buffer;

     special_code(1,tswitch);

     while (((*curbufptr==10) || (*curbufptr==13)) && (curbufptr<endbuf))
     {
      curbufptr++;
      absolute_position++;
     }

     while ((*curbufptr!=10) && (curbufptr<endbuf))
      {
      print_chr(*(curbufptr++));
      absolute_position++;
      }

     print_cr();

      special_code(0,tswitch);

    } /* end of file, end of search */

     g_free(buffer);
     g_fclose(fp);
     return;

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




int g_rename(char *file1,char *file2)
{  int temp;
   int is_locked=islocked(DOS_SEM);
   char temp1[120],temp2[120];

   strcpy(temp1,file1);
   strcpy(temp2,file2);

   if (!is_locked) lock_dos(226);
   temp=rename(temp1,temp2);
   if (!is_locked) unlock_dos();
   if (temp)
    {
        log_error(file1);
    }
   return (temp);
}




void edit_file(char *str,char *name,int portnum)
{
   char s[40];

   print_string("Edit File: ");
   get_string(s,39);
   if (*s) line_editor(s,16384);
    else print_cr();
};

void view_file(char *str,char *name,int portnum)
{
   char s[41];

   print_string("Enter Filename : ");
   *s=0;
   while (!*s)
     get_string(s,40);
   print_file(s);

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




ÿ