/***************************************

      String Manipulation Routines

 ***************************************/

#include <stdio.h>
#include <stdlib.h>
#include "str.h"

char *strcpy_n(register char *dest, register char *src, register int len)
{
  while ((*src) && (len > 0))
    {
      *dest++ = *src++;
      len--;
    }
  *dest = '\000';
  return (src);
}

char *strcat_n(register char *dest, register char *src, register int len)
{
  int str = strlen(dest);
  
  len -= str;
  dest += str;
  while ((*src) && (len > 0))
    {
      *dest++ = *src++;
      len--;
    }
  *dest = '\000';
  return (src);
}

char *next_space(register char *string)
{
  while ((*string != ' ') && (*string))
    string++;
  return (string);
}

char *next_char(register char *string, register char ch)
{
  while ((*string != ch) && (*string))
    string++;
  return (string);
}

char *skip_blanks(register char *string)
{
  while ((*string == ' ') || (*string == '\t'))
    string++;
  return (string);
}

int strcmp_case(char *s1, char *s2)
{
  register int x;

  while ((*s1) || (*s2))
    {
      x = upcase(*s1) - upcase(*s2);
      if (x)
	return (x);
      s1++;
      s2++;
    }
  return (0);
}

void get_string(register char *dest, char **src_d,
		 int len, int trail_blanks,
		 int upcase, int space)
{
  register char *src = *src_d;

  if (trail_blanks)
    src = skip_blanks(src);
  if (upcase)
    {
      while ((*src) && (len>0) && ((!space) || (*src != ' ')))
	{
	  *dest++ = upcase(*src);
	  src++;
	  len--;
	}
    } else
      {
	while ((*src) && (len>0) && ((!space) || (*src != ' ')))
	  {
	    *dest++ = *src++;
	    len--;
	  }
      }
  if (space)
    src = next_space(src);
  *dest = '\000';
  *src_d = src;
}

char *get_alphanum(char *dest, char *src, int len)
{
  src = skip_blanks(src);
  while (((is_alpha(*src)) || (is_num(*src))) && (len > 0))
    {
      *dest++ = *src++;
      len--;
    }
  *dest = '\000';
  return (src);
}

void *get_token(char **src_d, token_list *tlist, int *tok_no)
{
  int len = TOKEN_LENGTH;
  char *src = *src_d;
  char *c;
  int first, last, mid, lmid;
  char temp_tok[TOKEN_LENGTH+1];

  if (tlist->number < 0)
    {
      token_entry_type *tent = tlist->list;
      tlist->number = 0;
      while ((*tent->token) && (tent->tok_no != -1))
	{
	  tlist->number++;
	  tent++;
	}
    }
  c = temp_tok;
  src = skip_blanks(src);
  while ((is_alpha(*src)) && (len > 0))
    {
      *c++ = upcase(*src);
      src++;
      len--;
    }
  while (is_alpha(*src))
    src++;
  *c = '\000';
  *src_d = src;
  lmid = -1;
  mid = first = 0;
  last = tlist->number;
  for (;;)
    {
      if (lmid == mid)
	{
	  if (tok_no)
	    *tok_no = -1;
	  return (NULL);
	}
      lmid = mid;
      mid = (first + last) >> 1;
      if (!(len=strcmp(temp_tok, tlist->list[mid].token)))
	{
	  if (tok_no)
	    *tok_no = tlist->list[mid].tok_no;
	  return (tlist->list[mid].data);
	}
      else
	if (len < 0)
	  last = mid;
	else
	  first = mid;
    }
}

int get_number(char **src_d, unsigned long int *num)
{
  char *src = *src_d;
  unsigned long int temp;

  src = skip_blanks(src);
  if (!(is_num(*src)))
    {
      *src_d = src;
      return (FALSE);
    }
  temp = 0;
  do
    {
      temp = (10l * temp) + (*src++ - '0');
    } while (is_num(*src));
  *src_d = src;
  *num = temp;
  return (TRUE);
}

void get_any_type(char **src_d, token_list *tlist, int *type,
	     unsigned long int *data, void **ptr)
{
  char *src = *src_d;
  char *src2;
  int temp;
  unsigned long int temp2;
  void *temp3;

  src = skip_blanks(src);
  if (!(*src))
    {
      *src_d = src;
      *type = D_TYPE_END;
      return;
    }
  if (is_num(*src))
    {
      src2 = src;
      if (get_number(&src2, &temp2))
	{
	  *src_d = src2;
	  *type = D_TYPE_NUM;
	  *data = temp2;
	  return;
	}
    }
  if ((tlist) && (is_alpha(*src)))
    {
      temp3 = get_token(&src, tlist, &temp);
      *src_d = src;
      *data = temp;
      *type = D_TYPE_TOKEN;
      if (ptr)
	*ptr = temp3;
      return;
    }
  *type = D_TYPE_CHAR;
  *data = ((unsigned char) *src);
  *src_d = ++src;
  return;
}

char *read_line_from_file(register char *s, int size, register FILE *fp)
{
  register char ch;

  for (;;)
    {
      if (!size)
	{
	  while ((getc(fp) != '\n') && (!feof(fp)));
	  break;
	}
      if (((ch = getc(fp)) != '\n') && (!feof(fp)))
	{
	  *s++ = ch;
	  size--;
	}
      else
	break;
    }
  *s = '\000';
  return (s);
}








