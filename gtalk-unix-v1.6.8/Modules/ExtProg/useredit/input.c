#include <stdio.h>
#include <stdlib.h>
#include "ansi.h"
#include "input.h"



char backspace_string[]={8,' ',8,0};
char crlf_string[]={13,0};


int get_string(char *dest,int len, unsigned long int flags) 
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
	  if (!(flags & GI_FLAG_NO_ECHO))
	    {
	      printf("\\\r\n");
	    }
	  dest[0]=0;
	  return 0;
	  break;

	case 10:
	case 13:

	  dest[pos]=0;
	  if (pos>0) {
	    if (!(flags & GI_FLAG_NO_ECHO)) {
	      printf("\r\n");
	    }
	}
	  return 0;

	default:
	  if (!len)
	    break;
	  dest[pos] = next_char;
	  if (!(flags & GI_FLAG_NO_ECHO))
	    {
	      if (flags & GI_FLAG_MASK_ECHO)
		putc('.',stdout);
	      else
		putc(next_char,stdout);
	    }
	  pos++;
	  len--;
	  break;
	  
	}
      
    }
  
}
