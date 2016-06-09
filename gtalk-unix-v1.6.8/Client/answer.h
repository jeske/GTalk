/***************************************

               Answer.c

 ***************************************/

#ifndef _GTALK_ANSWER_H
#define _GTALK_ANSWER_H

int set_clocal(int clocal);
int drop_dtr(void);
int wait_for(char *string, int time);
int set_baud_rate(int baud);
int get_baud_rate(void);
int send_slow(char *string);
int answer_properly(void);
int set_special_canonical(void);
int set_special_non_canonical(void);

#endif  /* _GTALK_ANSWER_H */

