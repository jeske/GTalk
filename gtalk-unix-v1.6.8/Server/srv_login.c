
/**********************************

           srv_login.c

***********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "shared.h"
#include "common.h"
#include "types.h"
#include "list.h"
#include "states.h"
#include "srv_login.h"

int broadcast_login_message(char *format, ...)
{
  int i, len;
  va_list ap;
  node_struct *n;
  char line[LOGIN_MSG_LEN+1];

  va_start(ap, format);
  len=vsprintf(line, format, ap)+1;
  va_end(ap);

  for (i=0;i<c_nodes_used;i++)
    {
      n=c_nodes(i);
      if (n->status == NODE_ACTIVE)
	server_abuf_write(my_ip, i, STATE_LOGIN, line, len);
    }
}
