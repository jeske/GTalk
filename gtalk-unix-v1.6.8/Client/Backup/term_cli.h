

/**********************************

           srv_term.h

***********************************/

#ifndef _GTALK_TERM_CLI_H
#define _GTALK_TERM_CLI_H

#include "abuf.h"
#include "comparse.h"

typedef int (* termcli_func)(abuffer *buf, char *message);

int endterm_message(abuffer *abuf, char *message);
int term_message(abuffer *abuf, char *message);
int client_term_process(abuffer *abuf, char *message);
int cmd_terminal(com_struct *com, char *string);
int cmd_force_terminal(com_struct *com, char *string);

#define T_TM_TERM 1
#define T_TM_ENDTERM 2

#define TERM_STATE_INIT 0x00
#define TERM_STATE_FIRST 0x01
#define TERM_STATE_QUIT 0x02

#endif  /* _TERM_CLI_H */

