#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "ansi.h"
#include "input.h"
#include "output.h"



int get_input_cntrl(char *dest,int len, unsigned long int flags) 
{
  int next_char;
  int pos=0;

  while (1) 
    {
      next_char = getc(stdin);
      switch(next_char) 
	{
	case 8: 
	case 127:
	  if ((flags & GI_FLAG_NO_EMPTY) || (flags & GI_FLAG_NO_ABORT)) {
	    if (pos==0) {
	      break;
	    }
	  }
	  if (pos>0) {
	    pos--;
	    len++;
	    if (!(flags & GI_FLAG_NO_ECHO))
	      printf(backspace_string);
	  } else {
	    pos=0;
	    dest[0] = 0;
	    return 0;
	  }
	  break;
	case 27:
	  if ((flags & GI_FLAG_NO_EMPTY) || (flags & GI_FLAG_NO_ABORT)) {
	    if (pos==0) {
	      break;
	    }
	  }
	  if (!(flags & GI_FLAG_NO_ECHO))
	    {
	      printf("\\\r\n");
	    }
	  dest[0]=0;
	  return 0;
	  break;
	case 10:
	case 13:
	  if (flags & GI_FLAG_NO_EMPTY) {
	    if (pos==0) {
	      break;
	    }
	  }
	  dest[pos]=0;
	  if (pos>0) {
	    if (!(flags & GI_FLAG_NO_ECHO)) {
	      printf("\r\n");
	    }
	}
	  return 0;

	default:
          if ((next_char < ' ') || (next_char > '~'))
            break;
	  if (!len)
	    break;
	  if (flags & GI_FLAG_TOUPPER)
	    next_char = toupper(next_char);
	  dest[pos] = next_char;
	  if (!(flags & GI_FLAG_NO_ECHO))
	    {
	      if (flags & GI_FLAG_MASK_ECHO)
		putc('.',stdout);
	      else
                {
                  if (next_char == '+')
                    fwrite("+\b+", sizeof(char), 3, stdout);
                  else
                    putc(next_char,stdout);
                }

	    }
	  pos++;
	  len--;
	  break;
	  
	}
      
    }
  
}

int get_input(char *string,int len)
{
  get_input_cntrl(string,len,0);
}


/* get input with editing and command control */
/* string = pointer to where to edit */
/* limit = max number of characters to enter */


int get_input_cntrl_pos(char *string, int limit, char echo, char back_to_end,
					  char escape, char noblankline, char cr_on_blankline,
					  char upcase, char onlynum, int start_pos)
 {
   int pos = start_pos;
   int key;
   int flag = 1;
   int reason=0;

   while (flag) 			/* wait while editing */
	{
	  key = wait_ch();
	  if (((key == 8) || (key == 127)))
	  if (pos>0)
	   {
		 pos--; 		/* if an edit key is pressed and there's more to */
		 print_string(backspace_string);   /* erase, erase the character */
	   }			   /* and go back one */
	  else
	   if (back_to_end) { flag = 0; reason=1; }

	  if ((key == 27) && escape)  /* if we abort, then clear all characters */
	   {
		 flag = 0;
		 reason=2;
		 if (pos)		/* print a backslash to indicate abort */
		  {
			print_chr('\\');
			print_chr(13);
			print_chr(10);
			pos = 0;
		  };
	   };
	  if (key == 13)	/* finish the line */
	   {
		 if ((!noblankline) || (pos))
		  {
			flag = 0;
			if ((pos) || (cr_on_blankline))
			 {
			  print_chr(13);
			  print_chr(10);
			 };
		  };
	   };
	  if (((key >= 32) && (key <= 126)) && (pos < limit))
	   {
		 if ((upcase) && (key>='a') && (key<='z')) key -= 32;
		 if (!((onlynum) && ((key<'0') || (key>'9'))))
		  {
								/* insert the character if there's room */
		   *(string+pos) = key;
		   if (key == '+')        /* if +, don't let it be typed normally */
			{
			  print_chr(key);	  /* print the character with a space */
			  print_chr(32);	  /* and a backspace */
			  print_chr(8);
			}
			else
			if (echo) print_chr(echo);
			 else print_chr(key);		/* otherwise, print it normally */
		   pos++;
		  };
	   };
	};
   *(string+pos) = 0;			/* mark end of the string */
   return reason;
 };

int get_input_prompt_cntrl(char *prompt, char *str, int len, 
			    unsigned int flags)
{
 int pos=0;
 int next_char;
 char *dest = str;
 int def=1;

 if (flags & GIPC_FLAG_DEFAULT) {
   printf_ansi("%s [%s]: ",prompt,str);
 } else {
   printf_ansi("%s: ",prompt);
 }

 flags |= GI_FLAG_NO_EMPTY;

 while (1) {
   next_char = getc(stdin);
   switch(next_char) 
     {
     case 8: 
     case 127:
       if (flags & GI_FLAG_NO_EMPTY) {
	 if (pos==0) {
	   break;
	 }
       }
       if (pos>0) {
	 pos--;
	 len++;
	 if (!(flags & GI_FLAG_NO_ECHO))
	   printf(backspace_string);
       } else {
	 pos=0;
	 dest[0] = 0;
	 return 0;
       }
       break;
     case 27:
       if (flags & GI_FLAG_NO_EMPTY) {
	 if (pos==0) {
	   break;
	 }
       }
       if (!(flags & GI_FLAG_NO_ECHO))
	 {
	   printf("\\\r\n");
	 }
       dest[0]=0;
       return 0;
       break;
     case 10:
     case 13:
       if (flags & GIPC_FLAG_DEFAULT) {
	 if (def) {    /* if it has not been touched yet */
	   printf("Default Chosen: %s\r\n",str);
	   return 0;
	 }
       }
       if (flags & GI_FLAG_NO_EMPTY) {
	 if (pos==0) {
	   break;
	 }
       }
       dest[pos]=0;
       if (pos>0) {
	 if (!(flags & GI_FLAG_NO_ECHO)) {
	   printf("\r\n");
	 }
       }
       return 0;
       
     default:
       if ((next_char < ' ') || (next_char > '~'))
	 break;
       if (!len)
	 break;
       if (flags & GI_FLAG_TOUPPER)
	 next_char = toupper(next_char);
       dest[pos] = next_char;
       def = 0;
       if (!(flags & GI_FLAG_NO_ECHO))
	 {
	   if (flags & GI_FLAG_MASK_ECHO)
	     putc('.',stdout);
	   else
	     {
	       if (next_char == '+')
		 fwrite("+\b+", sizeof(char), 3, stdout);
	       else
		 putc(next_char,stdout);
	     }
	   
	 }
       pos++;
       len--;
       break;
       
     } 
 }

}

void empty_inbuffer(void)
{
   fd_set read_fd;
   int temp;
   struct timeval timeout;
   char buf[30];
   int num_read=0;

   fflush(0);
   fflush(stdin);
   do {
     FD_ZERO(&read_fd);
     FD_SET(0, &read_fd);

     timeout.tv_sec=0;
     timeout.tv_usec=50;
     temp = select(2, &read_fd, NULL, NULL, &timeout);    
     if (temp>0) {
       num_read = read(0,buf,30);
       fflush(stdout);
     }
   } while (temp!=0);
 }
