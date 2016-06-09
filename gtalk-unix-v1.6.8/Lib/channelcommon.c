/***********************************

        Channel Common Commands

 ***********************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <termios.h>
#include <arpa/telnet.h>
#include <sys/wait.h>

#include "str.h"
#include "list.h"
#include "common.h"
#include "abuf.h"
#include "states.h"

int read_system_node(char **c, g_system_t *system, node_id *node)
{
  unsigned long int n;

  if (!get_number(c, &n))
    return (-1);
  *system = n;
  *c = skip_blanks(*c);
  if (**c != '/')
    return (-1);
  (*c)++;
  if (!get_number(c, &n))
    return (-1);
  *node = n;
  return (0);
}

