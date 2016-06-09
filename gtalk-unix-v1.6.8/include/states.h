
/****************************

        states.h

 ****************************/

#ifndef _GTALK_STATES_H
#define _GTALK_STATES_H

#include "list.h"
#include "abuf.h"

typedef int (* state_func)(abuffer *buf, char *message);

typedef struct _state_machines
{
  g_uint32 type;
  state_func stfunc;
  g_uint32 flags;
} state_machine;

#define STATE_UNDEFINED 0x00
#define STATE_MESSAGE   0x0FF
#define STATE_CHANNEL   0x100
#define STATE_PRIVATE   0x200
#define STATE_DDIAL     0x300
#define STATE_LOGIN     0x400
#define STATE_TERM      0x500

#define STATE_SERVER_FLAG 0x01
#define STATE_CLIENT_FLAG 0x02

extern list parent_state_list;
extern list child_state_list;

extern state_machine default_parent_state_list[];
extern state_machine default_child_state_list[];

void init_state_list(state_machine *s, list *mylist);
void call_state_machine(abuffer *abuf, char *abuf_data, list *statemach);

#endif   /* _GTALK_STATES_H */
