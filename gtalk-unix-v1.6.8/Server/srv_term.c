
/**********************************

           srv_login.c

***********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "list.h"
#include "str.h"
#include "shared.h"
#include "common.h"
#include "types.h"
#include "list.h"
#include "states.h"
#include "srv_term.h"
#include "gt.h"

token_entry_type server_term_tokens[] =
{
  { "ENDTERM", endterm_message, T_TM_ENDTERM },
  { "FORCE", force_message, T_TM_FORCE },
  { "TERM", term_message, T_TM_TERM }
};

token_list server_term_tok = { 3, server_term_tokens };

int get_active_node(abuffer *abuf, char **message, 
		    node_struct **n, device_struct **d, int *device)
{
  unsigned long int num;

  if (abuf->source_machine != my_ip)
    return (-1);
  if (abuf->source_process >= c_nodes_used)
    return (-1);
  *n = c_nodes(abuf->source_process);
  if ((*n)->status != NODE_ACTIVE)
    return (-1);
  if (!get_number(message, &num))
    return (-1);
  *device = num;
  if ((*device < 0) || (*device >= c_devices_used))
    return (-1);
  *d = c_devices(*device);
  return (0);
}

int force_message(abuffer *abuf, char *message)
{
  int device, on;
  node_struct *n;
  device_struct *d;

  if (get_active_node(abuf, &message, &n, &d, &device) < 0)
    return (-1);
  message = skip_blanks(message);
  if (*message == '+')
    on = 1;
  else
    if (*message == '-')
      on = 0;
    else
      return (-1);
  if (d->node_type == TELNET_NODE_TYPE)
    return (-1);
  if ((!on) && (d->status == DEVICE_UNUSED))
    d->status = DEVICE_OFF;
  if ((on) && (d->status == DEVICE_OFF))
    d->status = DEVICE_UNUSED;
  d->retries = 0;
  check_nodes = 1;
  return (0);
}

int term_message(abuffer *abuf, char *message)
{
  int device;
  node_struct *n;
  device_struct *d;

  if (get_active_node(abuf, &message, &n, &d, &device) < 0)
    return (-1);
  if (d->node_type == TELNET_NODE_TYPE)
    return (-1);
  if ((d->status != DEVICE_UNUSED) && (d->status != DEVICE_OFF))
    return (-1);
  d->status = DEVICE_TERM;
  d->term_pid = n->pid;
  d->retries = 0;
  server_abuf_writef(abuf->source_machine, 
		     abuf->source_process,
		     STATE_TERM, "TERM %d", device);
  return (0);
}

int endterm_message(abuffer *abuf, char *message)
{
  int device;
  node_struct *n;
  device_struct *d;

  if (get_active_node(abuf, &message, &n, &d, &device) < 0)
    return (-1);
  if (d->status != DEVICE_TERM)
    return (-1);
  if (d->term_pid != n->pid)
    return (-1);
  d->status = DEVICE_UNUSED;
  d->term_pid = -1;
  d->retries = 0;
  server_abuf_writef(abuf->source_machine,
		     abuf->source_process,
		     STATE_TERM, "ENDTERM %d", device);
  check_nodes = 1;
  return (0);
}

int server_term_process(abuffer *abuf, char *message)
{
  srvterm_func ten;
  
  if (ten=get_token(&message, &server_term_tok, NULL)) 
    (ten)(abuf, message);
}
