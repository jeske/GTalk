/* FUCNTION.H */

#ifndef GT_FUNCTION_H
#define GT_FUNCTION_H


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


int g_strncmp(char *string1,char *string2,int len);
void repeat_chr(char chr,int times,char print_a_cr);  /* repeat a character
                                                         several times */
int nodes_free(void);
void shorten(char *string,int len);
int ansi_strlen(char *str);

void str_time(char *str, int legnth,struct tm *now);
void str_cpy(char *to,char *from);
void fix_classname(char *str);


/* ASCII BINARY CONVERSION */

void init_conversion(void);



void add_short(char **string, int number, char *conv_to);
int read_short(char **string, int *number, char *conv_from);
void add_medium(char **string, int number, char *conv_to);
int read_medium(char **string, int *number, char *conv_from);
void add_int(char **string, int number, char *conv_to);
int read_int(char **string, int *number, char *conv_from);
void add_string(char **string, char *instr, int length, char *conv_to, int *wlen);
int read_string(char *readstr, char **string, int *length, char *conv_from,
                int maxlen);
/* END ASCII BINARY CONVERSION */

void print_lurk_message_from(char *str, int portnum);


#endif /* GT_FUNCTION_H */

