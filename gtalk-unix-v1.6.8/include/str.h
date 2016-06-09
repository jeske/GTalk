/*******************************

             Str.h

 *******************************/

#ifndef _GTALK_STR_H
#define _GTALK_STR_H

#define TOKEN_LENGTH 20
#define D_TYPE_END   0
#define D_TYPE_NUM   1 
#define D_TYPE_TOKEN 2
#define D_TYPE_CHAR  3

typedef struct _token_entry_type
{
  char token[TOKEN_LENGTH+1];
  void *data;
  int  tok_no;
} token_entry_type;

typedef struct _token_list
{
  int number;
  struct _token_entry_type *list;
} token_list;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define is_num(x) (((x) >= '0') && ((x) <= '9'))
#define is_alpha(x)   ((((x) >= 'A') && ((x) <= 'Z')) || \
		       (((x) >= 'a') && ((x) <= 'z')))

#define upcase(x) ((((x) >= 'a') && ((x) <= 'z')) ? ((x) - ' ') : (x))

char *strcpy_n(register char *dest, register char *src, register int len);
char *strcat_n(register char *dest, register char *src, register int len);
char *next_space(register char *string);
char *next_char(register char *string, register char ch);
char *skip_blanks(register char *string);
int strcmp_case(char *s1, char *s2);
void get_string(register char *dest, char **src_d,
		 int len, int trail_blanks,
		 int upcase, int space);
int get_number(char **src_d, unsigned long int *num);
void *get_token(char **src_d, token_list *tlist, int *tok_no);
void get_any_type(char **src_d, token_list *tlist, int *type,
	     unsigned long int *data, void **ptr);
char *read_line_from_file(register char *s, int size, register FILE *fp);
char *get_alphanum(char *dest, char *src, int len);

#endif  /* _GTALK_STR_H */



