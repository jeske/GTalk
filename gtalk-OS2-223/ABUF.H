/* ABUF.h */

#ifndef GT_ABUF_H
#define GT_ABUF_H

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#define CLIENT_BUFFER 2048
#define CLIENT_BUFFER_1 2047
#define SERVER_BUFFER 8192
#define SERVER_BUFFER_1 8191

void aclear_buffer(int id);
int aback_abuffer(int id, int lines);
void initabuffer(int bufsize);
void dealloc_abuf(int portnum);
int aget_abuffer(int *sentby, int *channel, char *string, int *parm1,
                 int *parm2, int *parm3,int *parm4);
void aput_into_buffer(int id, char *string, int channel,
                 int parm1, int parm2, int parm3,int parm4);
void aput_append_into_buffer(int id, int channel, int parm1,
                      int parm2, int parm3, int parm4, int no_str, ...);
void aput_vargs_into_buffer(int id, int channel, int parm1,
                      int parm2, int parm3, int parm4, int no_str,
                      va_list *ap);
void aput_char(struct abuf_type *abuf, char c);
int is_abuf_empty(int id);


#endif /* GT_ABUF_H */
