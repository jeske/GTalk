/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/*****************************
 *   editor subsystem        *
 *****************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>

#include "types.h"
#include "ansi.h"

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

#define EDITOR_BUFFER_LENGTH 16384
#define EDITOR_WELCOME_FILE "text\\edtwlc.hdr"
#define END_OF_LINE_BYTES 4

char *backspacestring = "\010 \010";

/* Get a string in the editor buffer. This differs from get_string by */
/*  1. It echos a RETURN at the end of blank lines */
/*  2. It doesn't do anything with escape */
/*  3. It puts a CR and LF at the end of the string, t
    herefore the string allocated */
/*     for the buffer should be 3 bytes longer than the expected length */

void get_editor_string(char *string, int limit) 
     /* get a string with editing */
 {
   int pos = 0;
   int key;
   int flag = 1;

   while (flag)             /* wait while editing */
    {

      key = wait_ch();

      if (((key == 8) || (key == 127)) && (pos > 0))
       {
         pos--;         /* if an edit key is pressed and there's more to */
         print_string(backspacestring);   /* erase, erase the character */
       }                /* and go back one */

      if (key == 13)    /* finish the line */
       {
         flag = 0;
         print_cr();
       }

      if (key == 24)
       {
         while (pos>0)
          {
            pos--;
            print_string(backspacestring);
          }
       }

      if (((key >= 32) && (key <= 126)) && (pos < limit))
       {                        /* insert the character if there's room */
         *(string+pos) = key;
         if (key == '+')        /* if +, don't let it be typed normally */
          {
            print_chr(key);     /* print the character with a space */
            print_chr(32);      /* and a backspace */
            print_chr(8);
          }
          else
          print_chr(key);       /* otherwise, print it normally */
         pos++;
       }
       else
       {
        if ((key==9) && (pos<(limit-6)))
          {
	    int loop=5-(pos%5);
            while (loop--)
	      {
		*(string+pos)=' ';
		print_chr(' ');
		pos++;
	      }
          }
       }
    }
   *(string+pos) = 10;
   *(string+pos+1) = 0;           /* mark end of the string */
 }

/* Get a string in the editor buffer. This differs from get_string by */
/*  1. It echos a RETURN at the end of blank lines */
/*  2. It doesn't do anything with escape */
/*  3. It puts a CR and LF at the end of the string, therefore the string allocated */
/*     for the buffer should be 3 bytes longer than the expected length */

void get_editor_string_wordwrap(char *string, char *outstr, int limit)
                                 /* get a string with editing */
 {
   int pos = 0;
   int key;
   int flag = 1;
   int flag_found_space;
   int decspaces;
   char *printflag = string;

   *outstr = 0;
   while (*printflag)
    {
      print_chr(*printflag++);
      pos++;
    };
   while (flag)             /* wait while editing */
    {
      key = wait_ch();
      if (((key == 8) || (key == 127)) && (pos > 0))
       {
         pos--;         /* if an edit key is pressed and there's more to */
         print_string(backspacestring);   /* erase, erase the character */
       };               /* and go back one */
      if (key == 13)    /* finish the line */
       {
         flag = 0;
         print_cr();
       }
      if (key == 24)
       {
         while (pos>0)
          {
            pos--;
            print_string(backspacestring);
          }
       }
      if ((key >= 32) && (key <= 126) && (pos<limit))
        {                        /* insert the character if there's room */
          *(string+pos) = key;
           if (key == '+')        /* if +, don't let it be typed normally */
           {
             print_chr(key);     /* print the character with a space */
             print_chr(32);      /* and a backspace */
             print_chr(8);
           }
             else
	       print_chr(key);       /* otherwise, print it normally */
          pos++;
          if (pos == limit)
           {
            flag_found_space = 1;
            printflag = string + pos - 1;
            *(printflag+1) = 0;
            decspaces = 0;
            while ((printflag >= string) && (flag_found_space))
             if (*printflag == ' ') flag_found_space = 0;
               else
                {
                  printflag--;
                  decspaces++;
                }
            if (!flag_found_space)
             {
              if (decspaces < 20)
               {
                pos -= (1+decspaces);
                for (;decspaces>0;decspaces--)
		  print_string(backspacestring);
                *printflag++ = 0;
                strcpy(outstr,printflag);
	      }
              flag = 0;
              print_cr();
	    }
	  }
        }
      else
	  {
	  if ((key==9) && (pos<(limit-6)))
          {
	    int loop=5-(pos%5);
            while (loop--)
	      { 
		*(string+pos)=' ';
		print_chr(' ');
		pos++;
	      }
          }
      }
    }
   *(string+pos) = 10;
   *(string+pos+1) = 0;           /* mark end of the string */
 }


/* This returns the pointer to the beginning of the line numbered line */

char *location_of_line(int line,char *editor_buffer)
{
   char *tmp = editor_buffer;

   while ((*tmp) && (line>0))   /* Char 10 is end of line, count how many to */
    if ((*tmp++)==10) 
       line--;                  /* line number desired */

   return (tmp);
 }

/* This routine counts the number of lines 
    in the file (essentially #10 chars) */

unsigned int number_of_lines(char *editor_buffer)
{
   char *tmp = editor_buffer;
   unsigned int line = 0;

   while (*tmp)            /* Count number of 10 characters */
    if ((*tmp++)==10) line++;

   return (line);
}

/* This figures out the length of a line terminate by LF or null */

int length_of_line(char *tempstr)
 {
   int temp = 0;
   int flag = 1;
   char  *tmp = tempstr;

   while ((*tmp) && flag)
    {
      if ((*tmp++) == 10) flag = 0;
      temp++;
    }
   return (temp);
 }

/* this figures out the length of a null-terminated line (or length of file) */

int length_of_null(char *tempstr)
{
   char  *tmp = tempstr;
   int temp = 0;

   while (*tmp++) temp++;
   return (temp);
}

/* This inserts a line before line <line> of the character <string> */
/* 0 = buffer full, 1 = inserted */

int insert_line(int line, char *string,char *editor_buffer,
		     unsigned int *editor_length, unsigned int limit_buf)
 {
   int len = length_of_null(string);
   char *source = editor_buffer + *editor_length;
   char *dest = source + len;
   char *target = location_of_line(line, editor_buffer);

   if ((*editor_length + len) > limit_buf) 
     return 0; 
   /* abort if too much */

   while (target <= source) 
     *dest-- = *source--;   /* in the file by moving over data */
   while (*string) *target++ = *string++;      /* copy string into file */

   *editor_length += len;              /* add to recorded buffer length */
   return 1;
 };

/* This adds a line to the end of the file */
/* 0 = not enough room, 1 = added */

int add_line(char *string, char *editor_buffer, 
	     unsigned int *editor_length, unsigned int limit_buf)
 {
   int len = length_of_null(string);
   char *source = editor_buffer + *editor_length;
   char *dest = source + len;
   char *target = editor_buffer + *editor_length - 1;

   if ((*editor_length + len) > limit_buf) 
     return 0;	/* if it will overflow, quit */

   while (target <= source) *dest-- = *source--;   /* number of characters */
   while (*string) *target++ = *string++;      /* insert string into file */

   *editor_length += len;              /* add length to file */
   return 1;
 };

/* Find the next end of line character (line terminated by LF or null) */
/* from current line. returns pointer to place */

char *next_eol(char *cur_eol_temp)
 {
   int flag = 1;
   char *cur_eol = cur_eol_temp;

   if (!(*cur_eol))                /* are we at end of file? */
      return (cur_eol_temp);       /* just return same position */

   cur_eol++;
   while ((*cur_eol) && flag)      /* look for next LF or null */
    if ((*cur_eol++) == 10) flag = 0;

   return (cur_eol);               /* return that character */
 };

/* This deletes line numbers from line1 to line2 out of the file or just line1 if */
/* line2 is zero. */

void delete_line(int line, int line2, char *editor_buffer, 
		 unsigned int *editor_length)
 {
   char *dest = location_of_line(line, editor_buffer);    /* find beginning to delete */
   char *source;
   char *enddata = editor_buffer + *editor_length;

   if (line2) 
     source = location_of_line(line2,editor_buffer);	
              /* if line2=0 then delete 1 */
    else 
      source = next_eol(dest);    /* line */

   if (dest >= source) return;

   while (source < enddata) 
     *dest++ = *source++;   /* close up the gap */
   *editor_length -= (unsigned int)(source-dest);  
                            /* update editor length */
 };

/* This replaces one line with another. line # = <line>, 
   string = <string>. if successful, 1, if not 0 */

int edit_line(int line, char *string, char *editor_buffer,
	      unsigned int *editor_length, unsigned int limit_buf)
 {
   char *work_line = location_of_line(line, editor_buffer);
   		/* beginning of line to work on */
   char *endptr = editor_buffer + *editor_length;
   		/* end of file ptr */
   int line_length = length_of_line(work_line);
   		/* length of line we want to replace */
   int str_length = length_of_null(string);
   		/* length of line we want to stick in there */
   
   if (line_length > str_length)
     {              
       /* if the line we're putting in is smaller */
       int back = line_length - str_length;
       char *dest = work_line;
       char *source = work_line + back;
       while (source < endptr) 
	 *dest++ = *source++; /* close up gap */
       *editor_length -= back;              /* update length */
     }
   else
     {
       if (line_length < str_length)
	 {
	   int forward = str_length - line_length; 
	            /* otherwise add space into file */
	   char  *source = endptr;
	   char  *dest = endptr + forward;
	   if ((*editor_length+forward) > limit_buf) 
	           /* if we overflow space, abort */
	     return 0;
	   while (work_line<=source) 
	     *dest-- = *source--;  
	           /* move the file over a bit */
	   *editor_length += forward;
	 }
     }
   while (*string) *work_line++ = *string++;     
           /* copy line into file */

   return 1;
 };

/* show a region out of the editor buffer, 
   with line numbers starting at linenum if */
/* linenum != 0 */

void show_buffer(char *editor_start, char *editor_end, int linenum)
 {
   char *ptr = editor_start;
   int ischar, isthere;
   int flag = 1;

   while ((*ptr) && (flag) && (ptr < editor_end))
    {                  /* if we're printing line numbers */
      if (linenum)
	printf("%d> ",linenum++); /* print one here */
      while ((*ptr) && (flag) && (ptr < editor_end))
       {
	 /* print a line out of the file */
        ischar = in_char();
	         /* if escape is pressed, abort printing */
        if (*ptr == 10) 
	  {
	    /* if LF, go to next line */
	    print_cr();
	    ptr++;
	    break;
	  }
        if (ischar == 27)
          {
            flag = 0;
	    tcflush(STDOUT_FILENO, TCOFLUSH);
            print_cr();
            print_cr();
          } else print_chr(*ptr++);     /* print the character */
       }
    }
 }

/* This parses a number out of the command string starting 
   with character <bchar> */
/* and ending with the first nonnumeric character */

int get_edit_int(char *string, char bchar)
 {
   char s[10];
   char *put = s;
   int num = 0;
   int chr_in = 0;
   while (*string)         /* while there's more string to look at */
    {
      if ((*string++) == bchar)    /* if string is bchar */
       {
         while ((*string >= '0') && (*string <= '9') && (chr_in<4))
          {
            *put++ = *string++;      /* get the string bytes */
            chr_in++;
          };
         *put = 0;
         num = atoi(s);        /* and convert it to a number */
       };
    };
   return(num);            /* return the number done */
 };

/* This is an interface to list lines out of the buffer */

void list_buffer(char *command, char *editor_buffer, int *editor_length)
 {
   int line1 = get_edit_int(command,'L'); /* get first number */
   int line2;
   char *beginptr;
   char *endptr;
   char start_char = *(command+1);
   int lineflag = (start_char=='N'); 
        /* if command is 'N', then list line numbers */
   int proflag = (start_char=='M');  
        /* if command is 'E', show with emulation */

   if (proflag) 
     ansi_on(1);

   if (!line1)     /* show whole buffer if no line numbers are entered */
     show_buffer(editor_buffer,editor_buffer+*editor_length,lineflag != 0);
   else
     {
       line2 = get_edit_int(command,'-'); 
          /* see if there's a second line number */
       beginptr = location_of_line(line1-1,editor_buffer);
       if (!line2)
	 endptr = next_eol(beginptr);
       else
	 endptr = location_of_line(line2,editor_buffer);  /* lines */
       show_buffer(beginptr,endptr,lineflag ? line1 : 0); /* show the lines */
    }
    if (proflag) 
      ansi_on(0);
 };

/* This is an interface to the insert buffer command */

void insert_buffer(char *command,char *t, char *editor_buffer, 
		   int *editor_length, unsigned int limit_buf,int width)
 {
   int line = get_edit_int(command,'I');
                  /* get the line number to insert before */
   char *s=command;
   int flag = 1;

   *t = '\000';
   if (!line) 
     return;          /* if there's no beginning line, quit */
   line--;
   print_cr();
   print_str_cr("-->Enter lines here, type '.' alone to finish.");
   while (flag)
    {
      strcpy(s,t);
      get_editor_string_wordwrap(s,t,width);     /* get a line */
      if (*s=='.') flag = 0;       /* abort with a period */
      else
      {
        if (!insert_line(line++,s,editor_buffer,editor_length,limit_buf))
         {             /* otherwise insert the line */
           print_str_cr("-->Buffer is full, no longer inserting");
           return;
         };
      };
    };
   print_str_cr("-->Finished inserting lines.");
 };

/* This is a front end to the delete command */

void delete_buffer(char *command, char *editor_buffer, int *editor_length)
{
   int line1 = get_edit_int(command,'D');  /* get the first line number */
   int line2;

   if (line1)
     {
       line2 = get_edit_int(command,'-');  /* get the second line number if applicable */
       if (!line2)
        {
          delete_line(line1-1,0,editor_buffer,editor_length);  /* delete the one line */
          printf_ansi("-->Line %u deleted",line1);
	  print_cr();
        }
        else
        {
          delete_line(line1-1,line2,editor_buffer,editor_length);/* otherwise delete range */
          printf("-->Line %u to %u deleted",line1,line2);
        }
     }
}

/* Edit a line in the buffer */

void edit_lines_in_buffer(char *command,
       char *editor_buffer, int *editor_length, unsigned int limit_buf,
       int width)
{
   int linea;
   char *s = command;
   char *beginptr, *endptr;

   if (*(command+1)=='R')          /* if command is "R" then edit last line */
    linea = number_of_lines(editor_buffer);
    else
    linea = get_edit_int(command,'E');     
              /* otherwise get the number from line */
   if (linea)
     {
       print_cr();
       printf_ansi("-->Editing line %d:",linea);
       print_cr();
       beginptr = location_of_line(linea-1,editor_buffer);
       endptr = next_eol(beginptr);
       show_buffer(beginptr,endptr,0);
       get_editor_string(s,width);        /* get the line */
       edit_line(linea-1,s,editor_buffer,editor_length,limit_buf); 
              /* change it */
     };
   return;
 };

/* Show the status of what's in the buffer */

void show_stats(char *editor_buffer, int *editor_length, unsigned int limit_buf, int indent)
 {
   if (indent) print_string("       ");
   printf_ansi("In buffer: Characters %u, Lines %u, Limit %u characters",
	  *editor_length, number_of_lines(editor_buffer),limit_buf);
   print_cr();
 };

/* Save what's in the editor buffer */

void save_editor_file(char *filename, char *editor_buffer, int *editor_length)
 {
   FILE *fileptr;
   int point;

   if ((fileptr=fopen(filename,"wb"))==NULL)   
          /* open the file */
    {
      log_error(filename);
      return;
    }
   point = fwrite(editor_buffer, 1, *editor_length-1, fileptr);
                                /* write the file out */
   if (point != (*editor_length-1))
      log_error("Entire file \"%s\" was not written to disk", filename);

   fclose(fileptr);          /* close the file */
 };

/* Load a file into the editor */

void load_editor_file(char *filename, char *editor_buffer,
		      int *editor_length, unsigned int limit_buf)
 {
   FILE *fileptr;
   unsigned int point;

   *editor_buffer = 0;
   *editor_length = 1;

   if ((fileptr=fopen(filename,"rb"))==NULL)   /* open the file */
     return;

   point = fread(editor_buffer, 1, limit_buf-1, fileptr);
                                /* read the file in */
   *(editor_buffer+point) = 0;
   *editor_length = point+1;
   if (*editor_length != 1)
     show_stats(editor_buffer,editor_length,limit_buf,1);
   fclose(fileptr);          /* close the file */
 };

/* Main loop that does command recognition and enters lines */

void enter_lines(char *editor_buffer, int width, 
		 int *editor_length, char *filename, 
		 int *abort, unsigned int limit_buf)
 {
   char *s = editor_buffer;
   char *t = s+width+END_OF_LINE_BYTES;
   char c;
   char *upper;
   int flag = 1;
   int ed_width = width- (width % 5) - 1;

   editor_buffer=t + width + END_OF_LINE_BYTES;
   *t = 0;

   while (flag)
    {
      strcpy(s,t);
      get_editor_string_wordwrap(s,t,ed_width); /* get the command */
      if (*s=='.')
       {
         upper = s;
         while (*upper)
          {
            if (*upper>95) *upper -= 32;
            upper++;
          };
         c = *(s+1);
         switch (c)
          {
            case 'A': if (!get_yes_no("Abort?")) break;
                      flag = 0;         /* abort routine */
                      *abort = 0;
                      print_str_cr("--> Aborted");
                      break;
            case 'C': if (!get_yes_no("Clear?")) break;
                      *editor_buffer = 0;
                      *editor_length = 1;
                      break;
            case 'E': edit_lines_in_buffer(s,editor_buffer,editor_length,
					   limit_buf,ed_width);
                      break;           /* call edit routine for "E" and "R" */
            case 'R': edit_lines_in_buffer(s,editor_buffer,editor_length,
					   limit_buf,ed_width);
                      break;
            case 'L': list_buffer(s,editor_buffer,editor_length);
                      break;           /* list lines in buffer */
            case 'D': delete_buffer(s,editor_buffer,editor_length);
                      break;           /* delete from buffer */
            case 'I': insert_buffer(s,t,editor_buffer,editor_length,
				    limit_buf,ed_width);
                      break;           /* insert into buffer */
            case 'B': show_stats(editor_buffer,editor_length,limit_buf,0);
                      break;           /* show the buffer status */
            case '?': /* print_file("HELP\\EDITOR.HLP"); */
                      break;
            case 'S': save_editor_file(filename,editor_buffer,editor_length);
                      *abort = 1;      /* save editor file */
                      flag = 0;
                      break;
            case 'V': load_editor_file(filename,editor_buffer,
				       editor_length,limit_buf);
                      break;           /* revert to saved */
            case 'N': if (*(s+2)=='L')
                        {
                         list_buffer(s,editor_buffer,editor_length);
                         break;
                         };
            case 'M': if (*(s+2)=='L')
                        {
                         list_buffer(s,editor_buffer,editor_length);
                         break;
                         };
            default:  print_str_cr("--> Enter .? for help");
          };
       }
       else
       if (!add_line(s,editor_buffer,editor_length,limit_buf))
         print_str_cr("--> Buffer is full");    /* buffer is full */
    };
 };

/* front end to editor */

void print_short_welcome(int limit)
{
  char EdtWlc1[]="[|*f4GTalk|*f1  Editor|*f7]";
  char EdtWlc2[]="[|*f2.?|*f7 for help]";
  char temp[120];

  if (limit<30)
  {
    print_centered(EdtWlc1);
    print_centered(EdtWlc2);
    return;
  }

  /* WIDTH IS GREATER THAN 30, so... be it */
  sprintf(temp,"%s=%s",EdtWlc1,EdtWlc2);

  /* CENTER it */
  print_centered(temp);

}

int line_editor(char *filename, unsigned int limit_buf)
 {
   int loop;
   int width=80;
   char *editor_buffer;
   char *actual_editor_buffer;
   unsigned int editor_length;
   int abort;

   if (save_termios() < 0)
     return (-1);

   tty_raw(STDIN_FILENO);

   editor_buffer = malloc((limit_buf)+((width+END_OF_LINE_BYTES)<<1));
      /* Allocate the necessary memory */

   if ((!editor_buffer))
    {
      print_cr();
      print_str_cr("--> Not enough memory available to edit.");
      print_cr();
      restore_termios();
      return (-1);
    }

   actual_editor_buffer=(editor_buffer+((width+END_OF_LINE_BYTES)<<1));

   print_cr();                
   print_short_welcome(width);
   
   load_editor_file(filename,actual_editor_buffer,&editor_length,limit_buf);
   print_cr();                 /* load the file */
   for (loop=0;loop<(width/5);loop++)
    print_string("+----");
   print_cr();
   enter_lines(editor_buffer,width,&editor_length,filename,&abort,limit_buf);
   /* clear_call_on_logoff(); */        /* edit the buffer */
   free(editor_buffer);         /* deallocate */
   restore_termios();
   return (0);
 };





