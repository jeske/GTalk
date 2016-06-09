
/* FUNCTION.C */



/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* INCLUDES */

#define INCL_DOS
#include <os2.h>
#include "include.h"
#include "gtalk.h"
#include "console.h"

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
   int portnum = tswitch;

   if (times<0) return;

   while (times--)
    print_chr_to_noflush(chr,portnum);

   if (print_a_cr)
     print_cr();

   flush_output_buffer(portnum);
}


int system_nodes_free(void)
{
  int loop;
 int count=0;


 for (loop=0;loop<=sys_info.max_nodes;loop++)
	if (line_status[loop].online)
		 if ((port[loop].dial_in_line) && (!is_console_node(loop)))
			 count++;

 return (sys_toggles.num_dial_ins-count);

}

int nodes_free(void)
{
 int loop;
 int count=0;
 int can_see_lurk=test_bit(user_lines[tswitch].class_info.privs,LURK_PRV);


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
  char *buffer = g_malloc(GREP_BUF_SIZE+2,"GREP");
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


/* STRING / BINARY CONVERSION ROTINES */


/*
    6 bit conversion code 
*/

#define CTABLE_MAX 128

char conv_to_6bit[] =
{
   '0', '1', '2', '3', '4', '5', '6', '7',
   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
   'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
   'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
   'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
   'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
   'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
   'u', 'v', 'w', 'x', 'y', 'z', '.', '='
};

char conv_from_6bit[CTABLE_MAX];

void create_conv_from_table(char *conv_to, char *conv_from)
{
  int count;
  char *zero = conv_from;

  for (count=0;count<CTABLE_MAX;count++) *zero++ = -1;
  for (count=0;count<64;count++)
     conv_from[conv_to[count]] = count;
}

void init_conversion(void)
{
 create_conv_from_table(conv_to_6bit,conv_from_6bit);
}

void add_short(char **string, int number, char *conv_to)
{
  if (!conv_to)
    conv_to = conv_to_6bit;
  *(*string)++ = conv_to[number & 0x3F];
}

int read_short(char **string, int *number, char *conv_from)
{
  int total;
  if (!conv_from)
    conv_from = conv_from_6bit;

  total = conv_from[**string];

 
  if (total == -1) return 0;
  (*string)++;
  *number = total;
  return (1);
}

void add_medium(char **string, int number, char *conv_to)
{
  if (!conv_to)
    conv_to = conv_to_6bit;


  *(*string)++ = conv_to[number & 0x3F];
  *(*string)++ = conv_to[(number >> 6) & 0x3F];
}

int read_medium(char **string, int *number, char *conv_from)
{
  int total;
  int sum;

  if (!conv_from)
    conv_from = conv_from_6bit;

  total = conv_from[**string];

  if (total == -1) return 0;
  if ((sum = conv_from[*(++(*string))]) == -1) return (0);
  total += (sum << 6);
  (*string)++;
  *number = total;
  return (1);
}

void add_int(char **string, int number, char *conv_to)
{
  if (!conv_to)
    conv_to = conv_to_6bit;

  *(*string)++ = conv_to[number & 0x3F];
  *(*string)++ = conv_to[(number >> 6) & 0x3F];
  *(*string)++ = conv_to[(number >> 12) & 0x0F];
}

int read_int(char **string, int *number, char *conv_from)
{
  int total;
  int sum;

  if (!conv_from)
    conv_from = conv_from_6bit;
  total = conv_from[**string];

  if (total == -1) return 0;
  if ((sum = conv_from[*(++(*string))]) == -1) return (0);
  total |= (sum << 6);
  if ((sum = conv_from[*(++(*string))]) == -1) return (0);
  total |= (sum << 12);
  (*string)++;
  *number = total;
  return (1);
}

void add_string(char **string, char *instr, int length, char *conv_to, int *wlen)
{  
  int left;

  if (!conv_to)
    conv_to = conv_to_6bit;

  for (;;)
  {
    if (!length) break;
    length--;
    *(*string)++ = conv_to[*instr & 0x3F];
    (*wlen)++;
    left = (*instr++ >> 2) & 0x30;
    if (!length) 
    {
      *(*string)++ = conv_to[left];
      (*wlen)++;
      break;
    }
    length--;
    *(*string)++ = conv_to[(*instr & 0x0F) | left];
    (*wlen)++;
    left = (*instr++ >> 2) & 0x3C;
    if (!length)
    {
      *(*string)++ = conv_to[left];
      (*wlen)++;
      break;
    }
    length--;
    *(*string)++ = conv_to[(*instr & 0x03) | left];
    *(*string)++ = conv_to[(*instr++ >> 2) & 0x3F];
    (*wlen) += 2;
  }
  *(*string)++ = '#';
  (*wlen)++;
}
    
int read_string(char *readstr, char **string, int *length, char *conv_from,
                int maxlen)
{
  char ch;
  int left;

  if (!conv_from)
    conv_from = conv_from_6bit;

  *length = 0;
  for (;;)
  {
    if ((ch = *(*string)++) == '#') return (1);
    if ((*length) >= maxlen) return (0);
    if ((ch = conv_from[ch]) == -1) return (0);
    left = ch;
    if ((ch = *(*string)++) == '#') return (0);
    if ((ch = conv_from[ch]) == -1) return (0);
    *readstr++ = left | ((ch & 0x30) << 2);
    (*length)++;
    left = ch & 0x0F;
    if ((ch = *(*string)++) == '#') return (1);
    if ((*length) >= maxlen) return (0);
    if ((ch = conv_from[ch]) == -1) return (0);
    *readstr++ = left | ((ch & 0x3C) << 2);
    (*length)++;
    left = ch & 0x03;
    if ((ch = *(*string)++) == '#') return (1);
    if ((*length) >= maxlen) return (0);
    if ((ch = conv_from[ch]) == -1) return (0);
    *readstr++ = left | (ch << 2);
    (*length)++;
  }
}


/* END OF STRING / BINARY CONVERSION ROUTINES */

void print_lurk_message_from(char *str, int portnum)
{
   int loop;
   int channel=line_status[portnum].mainchannel;

   char strtemp[STRING_SIZE+5];

   strcpy(strtemp,"|*f5L");
   strcpy(strtemp+strlen(strtemp),str);

   for(loop=0;loop<=sys_info.max_nodes;loop++)
      if ( (line_status[loop].lurking || test_bit(user_lines[loop].class_info.privs,LURK_PRV)) &&
                (channel==line_status[loop].mainchannel) )
          aput_into_buffer(loop,strtemp,channel,8,tswitch,loop,8);

}


