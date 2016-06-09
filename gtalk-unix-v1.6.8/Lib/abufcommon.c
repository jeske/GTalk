
/*********************************

   Handle messaging over pipes 
         Client Side

**********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

#include "types.h"
#include "abuf.h"
#include "common.h"
#include "states.h"
#include "bufio.h"

char msg_id_ar[4] = { (MSG_ID & 0xFF), 
		      ((MSG_ID >> 8) & 0xFF),
		      ((MSG_ID >> 16) & 0xFF),
		      ((MSG_ID >> 24) & 0xFF) };

g_system_t my_ip = 0;

void init_abuffers(void)
{
  char s[100];
  struct hostent *ent;

  if (gethostname(s, 99) < 0)
    return;
  if (!(ent=gethostbyname(s)))
    return;
  my_ip = ntohl(*((g_uint32 *) (*ent->h_addr_list)));
}

int socket_read(int fd, char *data, int length)
{
  int res;
  int len = 0;

  do
    {
      if ((res = read(fd, data, length)) < 0)
	return (res);
      if (!res)
	return (len);
      data += res;
      len += res;
      length -= res;
    } while (length > 0);
  return (len);
}

int socket_write(int fd, char *data, int length)
{
  int res;
  int len = 0;

  do
    {
      if ((res = write(fd, data, length)) < 0)
	{
	  if (res != EINTR)
	    return (res);
	  else
	    continue;
	}
      if (!res)
	return (len);
      data += res;
      len += res;
      length -= res;
    } while (length > 0);
  return (len);
}

g_uint32 abuffer_checksum(char *buffer, g_uint32 length)
{
  g_uint32 checksum = 0;

  while (length-- > 0)
    checksum += (*((g_uint8 *)buffer)++);
  return (checksum);
}

