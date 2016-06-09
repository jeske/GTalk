/* OUTPUT.H */

#ifndef GT_OUTPUT_H
#define GT_OUTPUT_H

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



 /* extern declarations */

void string_cr(char *out);
void string_out(char *out);
void char_out(char out);
void print_file(const char *filename);
void print_file_to(const char *filename,int portnum);
void print_file_to_cntrl(const char *filename,int portnum,int ansi,
                   int pause,int abort,int paging);

unsigned int convstring(char *string, int encrypt);

int do_page_break(void);



void clear_screen(void);
int find_ansi(void);

void print_chr(char temp);
void print_chr_to(char temp,int portnum);
void print_string(char *string);
void print_string_noansi(char *string);
void print_str_cr_noansi(char *str);
void print_string_to(char *string,int portnum);
void print_str_cr(char *string);
void print_str_cr_to(char *string,int portnum);
void print_cr(void);
void print_cr_to(int portnum);
void print_sys_mesg(char *string);

int special_code(int state, int portnum);
void reset_attributes(int portnum);
void blink_video(int portnum);
void bold_video(int portnum);
void wrap_line(char *string);
char cur_video_state(int portnum);
void print_chr_to_noflush(char temp, int portnum);
void position(int y, int x);

extern char system_arrow[];

#endif /* GT_OUTPUT_H */
