

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

/* IO.C */
/* Ginsu I/O system */

#define INCL_DOSPROCESS
#define INCL_DOS
#include <os2.h>

/* headers */
#include "include.h"
#include "gtalk.h"
#include <time.h>
#include "console.h"


#undef DEBUG
struct ext_video       /* Structure to keep track of special ANSI */
 {             /* codes */
    char terminal;      /* = Is the terminal using special codes */
   char stage;         /* = What Code are we currently reading? */
   char data;          /* = Does this have any data we should save */
   char color;         /* = Have the colors been changed? */
   char bk_color;
   char curchr;
 };

struct ext_video ext_mode[MAX_THREADS];


#define circleplus(x,y) ((x) ^ ((y) & 0x07))

/* functions */

unsigned int convstring(char *string, int encrypt)
{
    char prev = 0x05;
    char temp;
    unsigned int checksum = 0;

    while (*string)
    {
       if (encrypt)
       {
		   checksum += (unsigned int) *string;
           prev = *string = circleplus(*string,prev);
	   }
	   else
       {
           temp = *string;
           *string = circleplus(*string,prev);
           checksum += (unsigned int) *string;
           prev = temp;
       };
       string++;
    };
    return(checksum);
};
/* this waits for a character from port port_num */

int do_page_break(void)
{
  int count;
  int abort=0;

	print_string("[ Press Return ]");
	do
	 {
	   count = wait_ch();
	   if ((count == 27) || (count == 3))
		{
		 count = 13;
		 abort=1;
	   };
	 } while (count != 13);

	for (count=0;count<16;count++)
	  print_string(backspacestring);

  return (abort);
}





/* print a file to the task that's the caller */
/* the filename is the file to print */

void print_file(const char *filename)
 {
  print_file_to(filename,tswitch);

 };


/* PRINT FILE TO */

void print_file_to(const char *filename,int portnum)
 {
   FILE *fileptr;
   long int location = 0;
   int count=0;
   char buf[512];
   int loop, point = 1;
   int ischar;
   int is_ansi_file=0;
   char *temp_file=(char *)filename;
   char temp_buf[80];

   if ((strlen(filename)<75) && (line_status[portnum].ansi) &&
                (line_status[portnum].full_screen_ansi))
	{  char *end;
	   strcpy(temp_buf,filename);
	   end=temp_buf+strlen(temp_buf);

	   while ((*end!='\\') && (*end!='.') && (end>=temp_buf))
		 end--;

	   if (*end=='.') *end=0;

	   strcat(temp_buf,".ANS");
	   temp_file=temp_buf;
	   is_ansi_file=1;
	   lock_dos(498);
	   if ((fileptr=g_fopen(temp_file,"rb","IO#3"))==NULL)   /* ANSI */
		 { is_ansi_file=0;
		   temp_file=(char *)filename;
		 }
	   else
		 g_fclose(fileptr);
	   unlock_dos();
	}

   special_code(!is_ansi_file,portnum);
   while (point)
	{
	  fileptr=0;
	  while (!fileptr)
	   {
		 lock_dos(499);
		 if( (fileptr=g_fopen(temp_file,"rb","IO#3"))==NULL)   /* open the file */
		  {
			log_error(filename);
			unlock_dos();
			special_code(0,portnum);
			return;
		  };
		 unlock_dos();
		 if (!fileptr) next_task();
	   };
	  lock_dos(500);
	  fseek(fileptr,location,SEEK_SET);         /* go to the portion of */
				/* the file we want to read */
	  point = fread(&buf, 1, 512, fileptr);     /* read 512 bytes */
	  g_fclose(fileptr);          /* close the file */
	  unlock_dos();
	  location += 512;          /* go 512 bytes further */
	  for (loop=0;loop<point;loop++)     /* print what's in the buffer */
	   {
		 print_chr_to_noflush(buf[loop],portnum);
		 ischar = get_first_char(portnum);
		 if (ischar==19)
		  {
			wait_ch();
			wait_ch();
		  };
		 if (!dcd_detect(portnum))  leave();
		 if ((ischar==3) || (ischar==27))
		  {
			point=0;
			int_char(portnum);
			empty_outbuffer(portnum);
			special_code(0,portnum);
			print_cr();
			print_str_cr("--> Aborted");
		  };
		  /*
	   if ((count++)>120)
		 {wait_for_xmit(portnum,30);
		  count=0;
		 }
		 */
	   };

	};
   special_code(0,portnum);
   flush_output_buffer(portnum);
   wait_for_xmit(portnum,30);
 };

/* PRINT FILE TO */

void print_file_to_cntrl(const char *filename,int portnum,int ansi,
				   int pause,int abort,int paging)
 {
   FILE *fileptr;
   long int location = 0;
   char buf[512];
   int xmitwait=0;
   int loop, point = 1;
   int ischar;
   int page_break = 0;
   int count;
   char *temp_file=(char *)filename;
   char temp_buf[80];
   int pagebreaks = is_console() ? 15 : 20;

   /* code for .ans files */
   if ((strlen(filename)<75) && (line_status[portnum].ansi) &&
		 (line_status[portnum].full_screen_ansi))
	{  char *end;
	   strcpy(temp_buf,filename);
	   end=temp_buf+strlen(temp_buf);

	   while ((*end!='\\') && (*end!='.') && (end>=temp_buf))
		 end--;

	   if (*end=='.') *end=0;

	   strcat(temp_buf,".ANS");
	   temp_file=temp_buf;
	   lock_dos(501);
	   if ((fileptr=g_fopen(temp_file,"rb","IO#3"))==NULL)   /* ANSI */
		   temp_file=(char *)filename;
	   else
		 {g_fclose(fileptr);
		  ansi=0;
		  paging=0;
		  }
	   unlock_dos();
	}


   /* end of code for .ansi files */






   if (ansi) special_code(1,portnum);

   while (point)
	{
	  fileptr=0;
	  while (!fileptr)
	   {
		 lock_dos(502);
		 if( (fileptr=g_fopen(temp_file,"rb","IO#4"))==NULL)   /* open the file */
		  {
			log_error(filename);
			unlock_dos();
			if (ansi) special_code(0,portnum);
			return;
		  };
		 unlock_dos();
		 if (!fileptr) next_task();
	   };
	  lock_dos(503);
	  fseek(fileptr,location,SEEK_SET);         /* go to the portion of */
				/* the file we want to read */
	  point = fread(&buf, 1, 512, fileptr);     /* read 512 bytes */
	  g_fclose(fileptr);          /* close the file */
	  unlock_dos();
	  location += 512;          /* go 512 bytes further */
	  for (loop=0;loop<point;loop++)     /* print what's in the buffer */
	   {
		 print_chr_to_noflush(buf[loop],portnum);
		 if (paging)
		  {
			if (buf[loop] == 10)
			 {
			   if (!cur_video_state(portnum))
				{
				 page_break++;
				 if (page_break == pagebreaks)
				  {
					print_string("[ Press Return ]");
					do
					 {
					   count = wait_ch();
					   if (((count == 27) || (count == 3)) && abort)
						{
						 count = 13;
						 point = 0;
					   };
					 } while (count != 13);
					for (count=0;count<16;count++)
					 print_string(backspacestring);
					page_break = 0;
				  };
				};
			 };
		  };
		 ischar = get_first_char(portnum);
		 if ((ischar==19) && pause)
		  {
			wait_ch();
			wait_ch();
		  };
		 if (!dcd_detect(portnum)) leave();
		 if (((ischar==3) || (ischar==27)) && abort)
		  {
			point=0;
			int_char(portnum);
			empty_outbuffer(portnum);
			if (ansi) special_code(0,portnum);
			print_cr();
			print_str_cr("--> Aborted");
		  };
		  /*
		if ((xmitwait++)>120)
		   {wait_for_xmit(portnum,30);
			xmitwait=0;
		   }
		   */
	   };
	};
   if (ansi) special_code(0,portnum);
   flush_output_buffer(portnum);
   wait_for_xmit(portnum,30);
 };

 /**************************
  * STRING output routines *
  **************************/


void print_string(char *string)
 {
   int portnum = tswitch;
   while (*string) print_chr_to_noflush(*string++,portnum);
   flush_output_buffer(tswitch);
 };

void print_string_len(char *string,unsigned char filler,int start,int len)
{  int portnum = tswitch;
   int empty=0;

    while ((start--) && (*string))
    {
     string++;
    }

    while (len--)
    {

      if (*string)
       {
         print_chr_to_noflush(*string++,portnum);
       }
      else
       {
          print_chr_to_noflush(filler,portnum);
       }

    }

}

void print_string_noansi(char *string)
{
   while (*string)
   {
   if (*string=='|')
	 { if ( (*(string+1)=='*') && (*(string+2)!=0) && (*(string+3)!=0) )
		string+=4;
	   else
	   string++;
	 }
   else
	 print_chr_to_noflush(*string++,tswitch);

   }
   flush_output_buffer(tswitch);
}

void print_str_cr_noansi(char *str)
{
	print_string_noansi(str);
	print_cr();
	flush_output_buffer(tswitch);
}

/* Print a string to the port specified */

void print_string_to(char *string,int portnum)
 {
   while (*string) print_chr_to_noflush(*string++,portnum);
   flush_output_buffer(portnum);
 };


/* Print system message (basically print a */
/* string to the screen prefaced by a -->) to the current user */

char system_arrow[]="--> ";

void print_sys_mesg(char *string)
{
  print_string(system_arrow);
  print_str_cr(string);
  flush_output_buffer(tswitch);
}

/* Prints a string with a carriage return */

void print_str_cr(char *string)
 {
   while (*string) print_chr_to_noflush(*string++,tswitch);
   print_cr();
   flush_output_buffer(tswitch);
 };

/* Prints a string to a specific port with a carriage return */

void print_str_cr_to(char *string,int portnum)
 {
   while (*string) print_chr_to_noflush(*string++,portnum);
   print_cr_to(portnum);
   flush_output_buffer(portnum);
 };

/* Prints a carriage return AND line feed, in that order */

void print_cr(void)
{
	 print_chr_to_noflush(13,tswitch);
	 print_chr_to_noflush(10,tswitch);
	 flush_output_buffer(tswitch);
}

/* Prints a carriage return AND line feed to the specified port */

void print_cr_to(int portnum)
{
	print_chr_to_noflush(13,portnum);
	print_chr_to_noflush(10,portnum);
	flush_output_buffer(portnum);
}

/* Prints a character to the current task */

void print_chr(char temp)
 {
   print_chr_to_noflush(temp,tswitch);
   flush_output_buffer(tswitch);
 };

/* Sets the state of whether the ANSI codes should be */
/* used or not. state=1 is YES to ANSI. note: if the color */
/* has been changed since the special codes have been turned */
/* on, the color will be reset to black and white */

int special_code(int state, int portnum)
 {
   struct ext_video *extptr = &ext_mode[portnum];
   int old_code = extptr->terminal;

   extptr->terminal = state;   /* Set the terminal state */
   if (!state)         /* If we're turning off ANSI */
	{
	  if (extptr->stage == 1) print_chr_to_noflush(START_ANSI_CHAR,portnum);
	  if (extptr->color)
	   {             /* and the colors are changed, reset them */
		 reset_attributes(portnum);
	   };
	};
   extptr->stage = 0;      /* Reset the special code stage and color */
   extptr->color = 0;      /* codes */
   extptr->bk_color = 0;
   flush_output_buffer(portnum);
   return (old_code);
 };


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

void print_chr_to(char temp, int portnum)
{
	print_chr_to_noflush(temp,portnum);
	flush_output_buffer(portnum);
}

void print_chr_to_noflush(char temp, int portnum)
 {
   struct ext_video  *extptr = &ext_mode[portnum];
			   /* Pointer for fast access to ANSI struct */
   unsigned char extended;

   if (!DosExitCritSec())
	 end_task();


   if (portnum>num_ports)
	 return;

   if (!port[portnum].active)
	return;

	  if (extptr->terminal)    /* If the ANSI codes are turned on */
	   {
		switch (extptr->stage)
		 {
		   case 0: if (temp == START_ANSI_CHAR)	/* If its the start char */
                    {
                      extptr->stage = 1;   /* Indicate to continue */
                      return;
                    }
                   break;
           case 1: if ((temp != '*') && (temp != '\\') && (temp != '+'))
                    {
                      extptr->stage = 0;   /* If not *, print char */
                      extptr->terminal = 0;    /* Send missed character */
                      print_chr_to_noflush(START_ANSI_CHAR,portnum);
					  extptr->terminal = 1;    /* And print current one */
                      if (temp == START_ANSI_CHAR) return;
                      break;
					}
                    else
                    {
					  if (temp == '*') extptr->stage = 2;
                       else if (temp == '+') extptr->stage = 7;
                        else extptr->stage = 3;
                      return;
                    };
           case 2: if (temp>'Z') temp -= 32;   /* Make command uppercase */
                   if ((temp<'A') || (temp>'Z')) extptr->stage = 0;
                     else extptr->stage = temp;	/* and next stage should be */
                   return;         /* the actual letter */
           case 3: if ((temp>='0') && (temp<='7'))
                    {
					  extptr->stage = 4;
                      extptr->data = (temp - '0') * 10;
                      return;
					} else extptr->stage = 0;
                   return;
           case 4: if ((temp>='0') && (temp<='9'))
					{
                      extptr->data += (temp - '0');
                      if (!test_bit(&user_options[portnum].privs,extptr->data))
                       {
                        extptr->stage = 5;
                        return;
                       };
                    };
                   extptr->stage = 0;
                   return;
           case 5: if (temp == '\\') extptr->stage = 6;
				   return;
           case 6: extptr->stage = 0;
                   return;
		   case 7: extptr->curchr = chex_digit(temp);
                   extptr->stage = 8;
                   return;
		   case 8: extptr->stage = 0;
                   if (extptr->curchr == -1)
                   {
                     temp = ' ';
                     break;
                   }
                   if ((temp=chex_digit(temp)) != -1)
                   {
                     extended = (extptr->curchr << 4) | temp;
                     if (extended < 0x20)
                     {
						if (barred_controls[extended] == 'X') temp = ' ';
                         else
                          if (!(line_status[portnum].ansi & 0x02)) temp = barred_controls[extended];
						   else
                             temp = extended;
                        break;
					 }
                     if (extended == 0x7F)
                     {
                       temp = ' ';
                       break;
                     }
                     if ((!(line_status[portnum].ansi & 0x02)) && (extended >= 0x80))
                       temp = ascii_convtable[extended - 0x80];
                     else
                       temp = extended;
                     break;
				   } else return;
		   case 'B': if ((temp>='0') && (temp<='7'))	/* if its background */
					  {
						background(temp-'0',portnum);    /* that's valid */
						extptr->bk_color = (int) (temp-'0');
					  };
					 extptr->stage = 0;        /* set it */
					 extptr->color = 1;
					 return;
		   case 'F': extptr->stage = 0;
					 if ((temp = chex_digit(temp)) == -1) return;
					 foreground(temp & 0x07,portnum);
					 if (temp > 7) bold_video(portnum);
					 extptr->color = 1;
					 return;
		   case 'H': if (temp=='0') reset_attributes(portnum);
					  else bold_video(portnum);
					 extptr->stage = 0;
					 extptr->color = 1;
					 return;
		   case 'P': if (temp=='0') reset_attributes(portnum);
					  else blink_video(portnum);
					 extptr->stage = 0;
					 extptr->color = 1;
					 return;
		   case 'R': if /* (temp=='1')*/ (1) reset_attributes(portnum);
					 extptr->stage = 0;
					 extptr->color = 0;
					 extptr->bk_color = 0;
					 return;
		   default:  reset_attributes(portnum);
					 extptr->stage = 0;        /* otherwise, reset */
					 extptr->color = 0;
					 return;
		 };
	   };

   if (line_status[portnum].watcher>=0)
	 print_chr_to(temp,line_status[portnum].watcher);

   queue_char(portnum,temp);        /* otherwise, send character */

};


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


void send_attribute(unsigned char attrib,int portnum)
{
  char s[20];

 /* if (is_console_node(portnum)) */

   sprintf(s,"%c[%dm",27,attrib);
  send_string_noflush(portnum,s);
}

/* reset attributes video routine */

void reset_attributes(int portnum)
{
  char s[20];
  unsigned char color=user_lines[portnum].user_info.reset_color;

  if (!line_status[portnum].ansi) return;   /* No ANSI, don't change color */
  sprintf(s,"%c[0m",27);  /* Otherwise send ANSI Code */
  send_string_noflush(portnum,s);
  if (color)
  {
	foreground(color & 0x07,portnum);
	if (color>>3) bold_video(portnum);
  }
}


/* blink video routine */

void blink_video(int portnum)
{
  char s[20];

  if (!line_status[portnum].ansi) return;   /* No ANSI, don't change color */
  sprintf(s,"%c[5m",27);  /* Otherwise send ANSI Code */
  send_string_noflush(portnum,s);
};

/* bold video routine */

void bold_video(int portnum)
{
  char s[20];

  if (!line_status[portnum].ansi) return;   /* No ANSI, don't change color */
  sprintf(s,"%c[1m",27);  /* Otherwise send ANSI Code */
  send_string_noflush(portnum,s);
}

/* Change the foreground color */

void foreground(int color, int portnum)
{
  char s[20];

  if (!line_status[portnum].ansi) return;   /* No ANSI, don't change color */
  sprintf(s,"%c[%dm",27,color+30);  /* Otherwise send ANSI Code */
  send_string_noflush(portnum,s);
};

/* Changes background color */

void background(int color, int portnum)
{
  char s[20];

  if (!line_status[portnum].ansi) return;   /* No ANSI, don't change color */
  sprintf(s,"%c[%dm",27,color+40);  /* Send ANSI code */
  send_string_noflush(portnum,s);
}


void wrap_line(char *string)
 {
  int col = 0;
  char *next_ch = string;
  char temp;
  char *ansi_strp;
  unsigned int slow=line_status[tswitch].slowdown_value;
  int loop;
  int width = user_options[tswitch].width - 1;
  int dif;
  struct ext_video  *extptr = &ext_mode[tswitch];

  special_code(1,tswitch);


     while (*string=='^')
      {
		print_cr();
		print_chr_to_noflush(32,tswitch);
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
        { if ((*(ansi_strp+1)=='*') && (((int)ansi_strp+3)<(int)next_ch))
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
               background(0,tswitch);
              print_cr();
              print_chr_to_noflush(32,tswitch);
              if (extptr->bk_color)
               background(extptr->bk_color,tswitch);
              col = 1;
            };
           print_chr_to_noflush(*string++,tswitch);
           col++;
         };
	  }
	  else
      {
        col += dif;
        if ((col >= width))
		 {
           if (extptr->bk_color)
            background(0,tswitch);
           print_cr();
           if (extptr->bk_color)
            background(extptr->bk_color,tswitch);
           col = dif;
         };
		while (string < next_ch)
		 {print_chr_to_noflush(*string++,tswitch);
          if (slow)
			{
			  flush_output_buffer(tswitch);
			  wait_for_xmit(tswitch,10);      /* for /slow stuff */
			  for (loop=0;loop<slow;loop++)
			  next_task();
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
           background(0,tswitch);
		  print_cr();
          if (extptr->bk_color)
		   background(extptr->bk_color,tswitch);
		  string++;
          next_ch++;
		  col = 0;
		 };
	  };
	 while (*string=='^')
	  {
		if (extptr->bk_color)
		 background(0,tswitch);
		print_cr();
		print_chr_to_noflush(32,tswitch);
		if (extptr->bk_color)
		 background(extptr->bk_color,tswitch);
		string++;
		next_ch++;
		col = 1;
	  };


  };
  special_code(0,tswitch);
  print_cr();
  flush_output_buffer(tswitch);
};


void clear_screen(void)
{

   if (!line_status[tswitch].ansi) /* If no ANSI, send a character 12 */
   {
	  print_chr(12);
	  return;
   }
   send_char(tswitch,27);      /* Otherwise, send special code */
   send_string(tswitch,"[2J");
}

/* This routine positions the cursor at a specific location */

void position(int y, int x)
 {
	char s[30];
	sprintf(s,"%c[%d;%dH",27,y,x);       /* Send position codes */
	send_string(tswitch,s);
 };


/* This routine determines whether ANSI is present at the terminal or not */

int find_ansi(void)
 {
   char s[10];
   time_t myt = time(NULL);
   int inchar;
   int isthere;
   int flag = 1;

   if (is_console())
       {
          if (console_is_mono()) return 0;
               /* If we're the console, then emulate */
            else return 1;
	   }

   sprintf(s,"%c[6n",27);  /* Send ANSI check code */
   send_string(tswitch,s); /* Send the code over */
   while (((time(NULL)-myt)<2) && flag)
	{
	  in_char(tswitch,&inchar,&isthere);
	  if ((isthere) && (inchar==27)) flag = 0; /* If escape code, +ANSI */
	  if (!isthere) next_task();
	};
   print_cr();         /* Print two carriage returns to clean up */
   print_cr();
   if (!flag) reset_attributes(tswitch);
   return (!flag);
 };


char cur_video_state(int portnum)
 {
   return (ext_mode[portnum].stage);
 };


