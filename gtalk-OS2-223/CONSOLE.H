/* VIDEO.H */

#ifndef GT_VIDEO_H
#define GT_VIDEO_H

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



void init_local_os2_video(void);
void end_local_os2_video(void);

#define WIDTH 80				/* width for screen */
#define START_ANSI_CHAR '|'     /* Start of special emulation codes */
#define MAX_ANSI_ELEMENTS 16

void console_alarm(void);

void position(int x,int y);
void scroll_view(int virt_cons);
void foreground(int color, int portnum);
void background(int color, int portnum);

void create_bar(int virt_cons);
void direct_screen(int y,int x,unsigned int attrib,unsigned char *string);
void init_display(int num_rows);

void beep_console(unsigned int pitch, unsigned int length);
void a_send_console_char(int portnum, int temp);

int allocate_a_console(int portnum);
int index_of_console(int number);

void move_cursor(int portnum);
void set_scroll_region(int virt_cons, int low, int high);
void scroll_view(int virt_cons);
void scroll_up_at_cursor(int virt_cons, int dir);
void move_back_line(int virt_cons, unsigned int char_move);
void position_console(int virt_cons, int x_pos, int y_pos, int rel);
void clear_to_eol_console(int virt_cons, int mode);
void clear_console(int virt_cons, int mode);
void send_chars_keyboard(int portnum, char *charptr, int length);


/*
extern unsigned int base_seg;
extern int avail_screen;
extern int cur_console;
extern struct video_screen *port_screen[MAX_THREADS];
*/
extern char is_a_console[MAX_THREADS];
extern unsigned char width;




void switch_virtual_console(int virt_cons);



			/* KEYBOARD routines */

void get_char_keyboard(int portnum, int *charput, int *isthere);
void send_char_keyboard(int portnum, char charput);
void empty_inbuffer_keyboard(int portnum);
void empty_outbuffer_keyboard(int portnum);
int dcd_detect_keyboard(int port_num);
int char_in_buf_keyboard(int portnum);
int get_first_char_keyboard(int portnum);
int get_nchar_keyboard(int portnum);
void put_char_in_buffer_keyboard(char temp,int portnum);
int chars_in_buffer_keyboard(int portnum);
void wait_for_xmit_keyboard(int portnum,int ticks);
void change_dtr_state_keyboard(int portnum, int state);




#endif /* GT_VIDEO_H */

