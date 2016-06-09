
/******************************************

            G-Talk state module

*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "str.h"
#include "list.h"
#include "states.h"

int compare_state_numbers(state_machine *s1, state_machine *s2)
{
  return (s1->type-s2->type);
}

void init_state_list(state_machine *s, list *mylist)
{
  if (!new_list(mylist, sizeof(state_machine)))
    return;
  while (s->stfunc)
    add_list(mylist, s++);
  add_index(mylist, compare_state_numbers);
}

void call_state_machine(abuffer *abuf, char *abuf_data, list *statemach)
{
  int ind;
  state_machine *stfound, stinst;

  stinst.type = abuf->type;
  if ((ind=search_list(statemach, 0, &stinst)) >= 0) 
    {
      stfound = element_of_index(state_machine,statemach,ind,0);
      (stfound->stfunc)(abuf, abuf_data);
    }
}
   


