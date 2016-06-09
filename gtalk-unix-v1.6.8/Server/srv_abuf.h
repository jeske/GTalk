/*****************************

   Interprocess buffers

******************************/

#ifndef _GTALK_ABUF_H
#define _GTALK_ABUF_H

#include "types.h"
#include "bufio.h"
#include "abuf.h"

#define ABUF_STATE_NONE 0
#define ABUF_STATE_HEADER 1

#define ABUF_IO_NOTREADY 0
#define ABUF_IO_INPUT 1
#define ABUF_IO_READY 2
#define ABUF_IO_ERROR 3

int read_iobuf_abuffer(iobuf *buf, abuffer_state *abuf_state, 
		       char *buffer, g_uint32 maxsize);
int write_iobuf_abuffer(iobuf *buf, abuffer *abuf, char *buffer);

int server_abuf_writef(g_system_t system, node_id node, 
		       g_uint32 type, char *format, ...);
int server_abuf_write(g_system_t system, node_id node, g_uint32 type,
			 char *data, int len);

#endif  /* _GTALK_ABUF_H */
