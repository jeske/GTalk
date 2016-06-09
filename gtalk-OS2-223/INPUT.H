/* INPUT.H */

#ifndef GT_INPUT_H
#define GT_INPUT_H

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#include "gtst.h"

extern struct vt100_conv_type vt100_key_list[];

void get_string_echo(char *string,int limit,char echo);
int wait_ch(void);

void prompt_get_string(const char *prompt,char *string,int limit);
void get_string(char *string,int limit);
int get_hot_key_prompt(char *prompt,char *chars_allowed,char def,char commands);

void check_for_privates(void);

int get_string_cntrl(char *string, int limit, char echo, char back_to_end,
                      char escape, char noblankline, char cr_on_blankline,
                      char upcase, char onlynum);
int get_string_cntrl_pos(char *string, int limit, char echo, char back_to_end,
                      char escape, char noblankline, char cr_on_blankline,
                      char upcase, char onlynum, int start_pos);

void get_no_echo(char *string, int limit);


#endif /* GT_INPUT_H */
