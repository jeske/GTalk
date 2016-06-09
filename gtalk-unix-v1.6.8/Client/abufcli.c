
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

#include "types.h"
#include "abuf.h"
#include "gtmain.h"
#include "common.h"
#include "states.h"
#include "bufio.h"
#include "abufcli.h"

int read_abuffer(int fd, abuffer *abuf, char *buffer, g_uint32 maxsize)
{
  g_uint8 byte;
  g_uint32 msg_id = MSG_ID;
  g_uint32 length;
  int error;
  int res;

  while (msg_id)
    {
      if ((res=read(fd, &byte, 1)) < 0)
	return (-1);
      if (byte != (msg_id & 0xFF))
	return (0);
      msg_id >>= 8;
    }
  if ((error=read(fd, abuf, sizeof(abuffer))) < 0)
    {
      return (-1);
    }
  if (error < sizeof(abuffer))
    {
      return (0);
    }
  abuf->type = ntohl(abuf->type);
  abuf->source_machine = ntohl(abuf->source_machine);
  abuf->source_process = ntohl(abuf->source_process);
  abuf->dest_machine = ntohl(abuf->dest_machine);
  abuf->dest_process = ntohl(abuf->dest_process);
  abuf->payload_length = ntohl(abuf->payload_length);
  abuf->checksum = ntohl(abuf->checksum);
  length = (abuf->payload_length > maxsize) ? maxsize :
    abuf->payload_length;
  if ((error=read(fd, buffer, length)) < 0)
    {
      return (-1);
    }
  if (error < length)
    {
      return (0);
    }
  if (abuf->checksum != abuffer_checksum(buffer,length))
    return (0);
  return (1);
}

int write_abuffer(int fd, abuffer *abuf, char *buffer)
{
  abuffer temp = *abuf;

  temp.type = htonl(temp.type);
  temp.source_machine = htonl(temp.source_machine);
  temp.source_process = htonl(temp.source_process);
  temp.dest_machine = htonl(temp.dest_machine);
  temp.dest_process = htonl(temp.dest_process);
  temp.payload_length = htonl(temp.payload_length);
  temp.checksum = htonl(abuffer_checksum(buffer,abuf->payload_length));

  if (socket_write(fd, msg_id_ar, sizeof(msg_id_ar)) < sizeof(msg_id_ar))
    return (-1);
  if (socket_write(fd, &temp, sizeof(temp)) < sizeof(temp))
    return (-1);
  if (socket_write(fd, buffer, abuf->payload_length) < abuf->payload_length)
    return (-1);
  return (0);
}

int write_abuf_string(int fd, abuffer *abuf, char *string)
{
  abuf->payload_length = strlen(string)+1;
  return (write_abuffer(fd, abuf, string));
}

int writef_abuf(int fd, abuffer *abuf, char *format, ...)
{
  va_list ap;
  char s[ABUF_STRING_LEN];

  va_start(ap, format);
  vsprintf(s, format, ap);
  va_end(ap);
  return (write_abuf_string(fd, abuf, s));
}

void abuf_to_server(abuffer *abuf)
{
  abuf->dest_machine = abuf->source_machine = my_ip;
  abuf->source_process = mynum;
  abuf->dest_process = SERVER_PROCESS;
}

int client_abuf_write(g_system_t system, node_id node, g_uint32 type,
			 char *data, int len)
{
  abuffer abuf;

  abuf.type = type;
  abuf.source_process = mynum;
  abuf.source_machine = my_ip;
  abuf.dest_process = node;
  abuf.dest_machine = system;
  abuf.payload_length = len;
  return (write_abuffer(mypipe, &abuf, data));
}

int ping_server(void)
{
  client_abuf_write(my_ip, SERVER_PROCESS, STATE_UNDEFINED, "", 0);
}

int client_abuf_writef(g_system_t system, node_id node, 
		       g_uint32 type, char *format, ...)
{
  va_list(ap);
  char line[ABUF_STRING_LEN];

  va_start(ap, format);
  vsprintf(line, format, ap);
  va_end(ap);
  return (client_abuf_write(system, node, type, line, strlen(line)+1));
}



