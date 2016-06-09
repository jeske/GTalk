/* OS2SER.C */

#ifndef GT_OS2SER_H
#define GT_OS2SER_H


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


void set_baud_rate_os2(int port_num, unsigned int baud,
     int databits, int stopbits, char parity);

int get_first_char_os2(int portnum);
int get_nchar_os2(int portnum);
void put_char_in_buffer_os2(char temp,int portnum);

void get_char_os2(int portnum, int *charput, int *isthere);
void send_char_os2(int portnum, char charput);
void send_chars_os2(int portnum, char *charptr, int length);
int chars_in_buffer_os2(int portnum);
int dcd_detect_os2(int port_num);
int char_in_buf_os2(int portnum);
void empty_inbuffer_os2(int portnum);
void wait_for_xmit_os2(int portnum, int ticks);
void empty_outbuffer_os2(int portnum);
void change_dtr_state_os2(int portnum, int state);


int init_os2_port(int port_num,unsigned int baud, int databits,
       int stopbits, char parity);


#endif /* GT_OS2SER_H */
