
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - ansi.c
 *
 * ANSI Code Interpretation
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "ansi.h"

#define START_ANSI_CHAR '|'
#define PRINTF_ANSI_LENGTH 2000

struct ext_video       /* Structure to keep track of special ANSI */
 {             /* codes */
   char terminal;      /* = Is the terminal using special codes */
   char stage;         /* = What Code are we currently reading? */
   char data;          /* = Does this have any data we should save */
   char color;         /* = Have the colors been changed? */
   char bk_color;
   char curchr;

   char see_controls;  /* = see control characters or not show them */
   char ncurses_sc;    /* = show an ncurses screen */
 };

static struct ext_video vids = { 0, 0, 0, 0, 0, 0, 0, 0 };

int chex_digit(char ch)
{
  if ((ch >= 'A') && (ch <= 'F')) return (ch - 'A' + 10);
  if ((ch >= 'a') && (ch <= 'f')) return (ch - 'a' + 10);
  if ((ch >= '0') && (ch <= '9')) return (ch - '0');
  return (-1);
}

char ascii_convtable[] =
"cueaaaaceeeiiiAAEaAooouuyoucfYPfaiounNao?[]24!<>###||||]]||]]]]][--|-+||[[==|=+=---[[[[++][#-||_aBgPEout@OOdioEn=+></\\-~...Vn2# ";

char barred_controls[] =
"XOOVo^^XXXXoXX/o><^!PS_^v><X[-^v";

static void outch(char ch)
{
  if (ch != '\n')
    putc(ch, stdout);
}


int ansi_on(int state)
 {
   int temp = vids.terminal;

   vids.terminal = state; /* Set the terminal state */
   if (!state)           /* If we're turning off ANSI */
    {
      if (vids.stage == 1) 
	print_chr(START_ANSI_CHAR);
      if (vids.color)
       {           
	 /* and the colors are changed, reset them */
         reset_attributes();
       }
    }
   vids.stage = 0;         /* Reset the special code stage and color */
   vids.color = 0;         /* codes */
   vids.bk_color = 0;
   return (temp);
 }


void print_chr_noflush(char temp)
 {
   unsigned char extended;

   if (vids.terminal)    /* If the ANSI codes are turned on */
     {
       switch (vids.stage)
         {
	    case 0: if (temp == START_ANSI_CHAR) /* If its the start char */
	              {
		        vids.stage = 1;   /* Indicate to continue */
		        return;
	              }
	             break;
	    case 1: if ((temp != '*') && (temp != '+'))
                      {
                        vids.stage = 0;   /* If not *, print char */
                        vids.terminal = 0;    /* Send missed character */
			outch(START_ANSI_CHAR);
                        vids.terminal = 1;    /* And print current one */
                        if (temp == START_ANSI_CHAR) 
			  return;
                        break;
                      } else
                        {
			  vids.stage = (temp == '*') ? 2 : 7;
			  return;
                        };
           case 2: if (temp > 'Z') temp -= 32;  /* Make command uppercase */
                   if ((temp<'A') || (temp>'Z')) vids.stage = 0;
                     else vids.stage = temp;	/* and next stage should be */
                   return;         /* the actual letter */
           case 7: vids.curchr = chex_digit(temp);
                   vids.stage = 8;
                   return;
           case 8: vids.stage = 0;
                   if (vids.curchr == -1)
		     {
		       temp = ' ';
		       break;
		     }
	           if ((temp=chex_digit(temp)) != -1)
		     {
		       extended = (vids.curchr << 4) | temp;
		       if (extended < 0x20)
			 {
			   temp = ((barred_controls[extended] == 'X') ||
				   (!vids.see_controls))
			     ? ' ' : extended;
			   break;
			 } else
		       if (extended == 0x7F)
			 {
			   temp = ' ';
			   break;
			 } else
		       if (extended >= 0x80)
			 {
                           temp = (vids.see_controls) ? 
			     ascii_convtable[extended - 0x80] : extended;
			 } else
                       temp = extended;
                     break;
                   } else return;
           case 'B': if ((temp>='0') && (temp<='7'))	/* if its background */
                      {
                        background(temp-'0');    /* that's valid */
                        vids.bk_color = (int) (temp-'0');
                      };
                     vids.stage = 0;        /* set it */
                     vids.color = 1;
                     return;
           case 'F': vids.stage = 0;
                     if ((temp = chex_digit(temp)) == -1) return;
                     foreground(temp & 0x07);
                     if (temp > 7) 
		       bold_video();
                     vids.color = 1;
                     return;
           case 'H': if (temp=='0') 
	                reset_attributes();
                        else bold_video();
                     vids.stage = 0;
                     vids.color = 1;
                     return;
           case 'P': if (temp=='0')
	                reset_attributes();
                        else blink_video();
                     vids.stage = 0;
                     vids.color = 1;
                     return;
           case 'R': reset_attributes();
                     vids.stage = 0;
                     vids.color = 0;
                     vids.bk_color = 0;
                     return;
           default:  reset_attributes();
                     vids.stage = 0;        /* otherwise, reset */
                     vids.color = 0;
                     return;
         }
       }
   if (temp == '\n')
     fputs("\n\r", stdout);
   else
     putc(temp, stdout);
}

void print_chr(char temp)
{
  print_chr_noflush(temp);
  fflush(stdout);
}

void print_string(register char *string)
{
  while (*string)
    print_chr_noflush(*string++);
  fflush(stdout);
}

void print_cr()
{
  printf("\r\n");
}

void print_str_cr(char *string)
{
  print_string(string);
  print_cr();
}

int printf_ansi(char *format, ...)
{
  va_list ap;
  char s[PRINTF_ANSI_LENGTH+1];

  va_start(ap, format);
  vsprintf(s, format, ap);
  va_end(ap);
  print_string(s);
  return (ansi_strlen(s));
}

void print_centered(char *str)
{
 int numchars = (80-ansi_strlen(str))/2;
 int temp;

 if (numchars>0)
   while(numchars--)
     print_chr(' ');
 temp = ansi_on(1);
 print_str_cr(str);
 ansi_on(temp);
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

int in_char(void)
{
  fd_set fdset;
  int temp;
  unsigned char ch;
  struct timeval tim = { 0l, 0l };

  FD_ZERO(&fdset);
  FD_SET(STDIN_FILENO, &fdset);
  temp = select(STDIN_FILENO+1, &fdset, NULL, NULL, &tim);
  if (temp > 0)
    {
      if (read(STDIN_FILENO, &ch, sizeof(ch)) > 0)
	return (ch);
    }
  return (-1);
} 

int wait_ch(void)
{
  int temp;
  unsigned char ch;

  if ((temp = read(STDIN_FILENO, &ch, sizeof(ch))) < 0)
    return (-1);
  return (ch);
}


void wrap_line(char *string)
{
  int col = 0;
  char *next_ch = string;
  char temp;
  char *ansi_strp;
  /*  unsigned int slow=line_status[tswitch].slowdown_value; */
  unsigned int slow=0;
  int loop;
  int width = 80 - 1; /* user_options[tswitch].width - 1; */
  int dif;
  struct ext_video  *extptr = &vids;
  int ansi_state = ansi_on(1);
  
  while (*string=='^')
    {
      print_cr();
      print_chr_noflush(32);
      string++;
      next_ch++;
      col = 1;
    };
  
  while (*string)
    {
      
      
      do
	{
	  temp = *(++next_ch);
	} while ((temp != ' ') && (temp) && (temp != '^') && (temp != 13));
      
      dif = (int)((int)next_ch - (int)string);
      
      /* we need to account for ANSI stuff */
      
      ansi_strp=string;
      while ((ansi_strp<next_ch) && ansi_strp)
	{
	  if (*ansi_strp=='|')
	    { if ( ((*(ansi_strp+1)=='*') || (*(ansi_strp+1)=='+')) 
		  && (((int)ansi_strp+3)<(int)next_ch))
		{ansi_strp+=4;
		 if (dif>4)
		   dif-=4;
		 else dif=1;
	       }
	    else
	      ansi_strp++;
	    }
	  else
	    ansi_strp++;
	}
      /* done accounting for ANSI */
      
      if (dif>width)
	{
	  while (string < next_ch)
	    {
	      if (col == width)
		{
		  if (extptr->bk_color)
		    background(0);
		  print_cr();
		  print_chr_noflush(32);
		  if (extptr->bk_color)
		    background(extptr->bk_color);
		  col = 1;
		};
	      print_chr_noflush(*string++);
	      col++;
	    };
	}
      else
	{
	  col += dif;
	  if ((col >= width))
	    {
	      if (extptr->bk_color)
		background(0);
	      print_cr();
	      if (extptr->bk_color)
		background(extptr->bk_color);
	      col = dif;
	    };
	  while (string < next_ch)
	    {print_chr_noflush(*string++);
	     if (slow)
	       {
		 fflush(stdout);
		 /*wait_for_xmit(tswitch,10); */ /* for /slow stuff */
		 for (loop=0;loop<slow;loop++)
		   sleep(1);
	       }
	     
	   }
	};
      
      while (*string==13)
	{
	  string++;
	  next_ch++;
	  if (*string==10)
	    {
	      if (extptr->bk_color)
		background(0);
	      print_cr();
	      if (extptr->bk_color)
		background(extptr->bk_color);
	      string++;
	      next_ch++;
	      col = 0;
	    };
	};
      while (*string=='^')
	{
	  if (extptr->bk_color)
	    background(0);
	  print_cr();
	  print_chr_noflush(32);
	  if (extptr->bk_color)
	    background(extptr->bk_color);
	  string++;
	  next_ch++;
	  col = 1;
	};
      
      
    };
  ansi_on(ansi_state);
  print_cr();
  fflush(stdout);
};












void remove_flashing(char *str)
{
   char *newstr = str;

   while (*str)
    {
        if ((*str=='|')&&(*(str+1)=='*') && ( (*(str+2)=='p') || (*(str+2)=='P') ) && (*(str+3)!=0))
           str += 4;
		else
           *newstr++ = *str++;
    }
  *newstr = 0;

}

void remove_ansi(char *str)
{
  char *newstr = str;

   while (*str)
    {
        if ((*str=='|') &&
           ((*(str+1)=='*') || (*(str+1)=='+'))&&
            (*(str+2)!=0) && (*(str+3)!=0))
           str+=4;
        else
		  if (*str=='^')  /* filter carrots also */
              str++;
        else
          *newstr++ = *str++;
    }
  *newstr = 0;

}


void filter_flashing(char *str,char *newstr)
{
   while (*str)
	{
        if ((*str=='|')&&(*(str+1)=='*') && ( (*(str+2)=='p') || (*(str+2)=='P') ) && (*(str+3)!=0))
           str += 4;
        else
           *newstr++ = *str++;
    }
  *newstr = 0;
}


void filter_ansi(char *str,char *newstr)
{
   if (!newstr)
     newstr = str;

   while (*str)
    {
        if ((*str=='|') &&
             ((*(str+1)=='*') || (*(str+1)=='+')) &&
               (*(str+2)!=0) && (*(str+3)!=0))
           str += 4;
        else
          if (*str=='^')  /* filter carrots also */
			  str++;
        else
          *newstr++ = *str++;
    }
  *newstr=0;
}

void ansi_end_fix(char *str)
{
  char *end = str;
  int back = 0;

  while (*end) end++;
  if (end == str) return;

  while ((end > str) && (back < 3))
  {
    end--;
    back++;
    if ((*end == '|') && ((end[1] == '*') || (end[1] == '+')))
    {
	  *end = 0;
      back = 0;
    }
  }
  return;
}
