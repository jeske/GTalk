/*****************************

   Interprocess buffers

******************************/

#ifndef _GTALK_ABUFCLI_H
#define _GTALK_ABUFCLI_H

#include "types.h"
#include "bufio.h"
#include "abuf.h"

int read_abuffer(int fd, abuffer *abuf, char *buffer, g_uint32 maxsize);
int write_abuffer(int fd, abuffer *abuf, char *buffer);
int write_abuf_string(int fd, abuffer *abuf, char *string);
int writef_abuf(int fd, abuffer *abuf, char *format, ...);
void abuf_to_server(abuffer *abuf);
int client_abuf_writef(g_system_t system, node_id node, 
		       g_uint32 type, char *format, ...);
int client_abuf_write(g_system_t system, node_id node, g_uint32 type,
			 char *data, int len);
int ping_server(void);

#endif  /* _GTALK_ABUFCLI_H */
