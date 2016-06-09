
/*********************************

   Handle messaging over pipes 

**********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "types.h"
#include "srv_abuf.h"
#include "common.h"
#include "fork.h"
#include "states.h"
#include "bufio.h"

int read_iobuf_abuffer(iobuf *buf, abuffer_state *abuf_state, 
		       char *buffer, g_uint32 maxsize)
{
  int byte;
  g_uint32 msg_id = MSG_ID;
  g_uint32 length;
  int error;
  int res;

  if (abuf_state->state == ABUF_STATE_NONE)
    {
      if (buffer_input_used(buf) < (sizeof(g_uint32)+sizeof(abuffer)))
	return (ABUF_IO_NOTREADY);
      while (msg_id)
	{
	  if ((msg_id & 0xFF) != read_ch_buffer(buf))
	    return (ABUF_IO_INPUT);
	  msg_id >>= 8;
	}
      if (read_buffer(buf, &abuf_state->abuf, sizeof(abuf_state->abuf)) !=
	  sizeof(abuf_state->abuf))
	return (ABUF_IO_ERROR);
      abuf_state->state = ABUF_STATE_HEADER;
      abuf_state->abuf.type = ntohl(abuf_state->abuf.type);
      abuf_state->abuf.source_machine = ntohl(abuf_state->abuf.source_machine);
      abuf_state->abuf.source_process = ntohl(abuf_state->abuf.source_process);
      abuf_state->abuf.dest_machine = ntohl(abuf_state->abuf.dest_machine);
      abuf_state->abuf.dest_process = ntohl(abuf_state->abuf.dest_process);
      abuf_state->abuf.payload_length = ntohl(abuf_state->abuf.payload_length);
      abuf_state->abuf.checksum = ntohl(abuf_state->abuf.checksum);
    }
  if (abuf_state->state == ABUF_STATE_HEADER)
    {
      length = (abuf_state->abuf.payload_length > maxsize) ? maxsize :
	abuf_state->abuf.payload_length;
      if (buffer_input_used(buf) < length)
	return (ABUF_IO_NOTREADY);
      abuf_state->state = ABUF_STATE_NONE;
      if (read_buffer(buf, buffer, length) < length)
	return (ABUF_IO_ERROR);
      if (abuf_state->abuf.checksum != abuffer_checksum(buffer,length))
	return (ABUF_IO_ERROR);
      return (ABUF_IO_READY);
    }
  abuf_state->state = ABUF_STATE_NONE;
  return (ABUF_IO_INPUT);
}

int write_iobuf_abuffer(iobuf *buf, abuffer *abuf, char *buffer)
{
  abuffer temp = *abuf;

  temp.type = htonl(temp.type);
  temp.source_machine = htonl(temp.source_machine);
  temp.source_process = htonl(temp.source_process);
  temp.dest_machine = htonl(temp.dest_machine);
  temp.dest_process = htonl(temp.dest_process);
  temp.payload_length = htonl(temp.payload_length);
  temp.checksum = htonl(abuffer_checksum(buffer,abuf->payload_length));

  if (write_buffer(buf, msg_id_ar, sizeof(msg_id_ar)) < sizeof(msg_id_ar))
    return (-1);
  if (write_buffer(buf, &temp, sizeof(temp)) < sizeof(temp))
    return (-1);
  if (write_buffer(buf, buffer, abuf->payload_length) < abuf->payload_length)
    return (-1);
  return (0);
}

int server_abuf_write(g_system_t system, node_id node, g_uint32 type,
			 char *data, int len)
{
  abuffer abuf;

  if (system == my_ip)
    {
      if (node < c_nodes_used)
	{
	  if (is_node_online(c_nodes(node)))
	    {
	      abuf.type = type;
	      abuf.dest_process = node;
	      abuf.source_process = SERVER_PROCESS;
	      abuf.source_machine = abuf.dest_machine = my_ip;
	      abuf.payload_length = len;
	      return (write_iobuf_abuffer(&(c_nodes(node)->pipebuf),
					  &abuf, 
					  data)); 
	    }
	}
      return (-1);
    }

  /* insert distribute code here */

  return (-1);
}

int server_abuf_writef(g_system_t system, node_id node, 
		       g_uint32 type, char *format, ...)
{
  va_list(ap);
  char line[ABUF_STRING_LEN];

  va_start(ap, format);
  vsprintf(line, format, ap);
  va_end(ap);
  return (server_abuf_write(system, node, type, line, strlen(line)+1));
}





