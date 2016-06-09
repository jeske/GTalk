

/**********************************

           srv_term.h

***********************************/

#ifndef _GTALK_SRV_TERM_H
#define _GTALK_SRV_TERM_H

typedef int (* srvterm_func)(abuffer *buf, char *message);

int endterm_message(abuffer *abuf, char *message);
int force_message(abuffer *abuf, char *message);
int term_message(abuffer *abuf, char *message);
int server_term_process(abuffer *abuf, char *message);

#define T_TM_TERM 1
#define T_TM_ENDTERM 2
#define T_TM_FORCE 3

#endif  /* _SRV_TERM_H */

