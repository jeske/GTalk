/*****************************

   Interprocess buffers

******************************/

#ifndef _GTALK_ABUFCOMMON_H
#define _GTALK_ABUFCOMMON_H

#include "types.h"
/* #include "bufio.h" */

#define MSG_ID 0x123455AAl
#define SERVER_PROCESS 0xFFFFFFFF
#define ABUF_STRING_LEN 2048

typedef struct _abuffer_format
{
  g_uint32 type;
  g_uint32 source_machine;
  g_uint32 source_process;
  g_uint32 dest_machine;
  g_uint32 dest_process;
  g_uint32 payload_length;
  g_uint32 checksum;
} abuffer;

typedef struct _abuffer_state
{
  struct _abuffer_format abuf;
  int state;
} abuffer_state;

void init_abuffers(void);
int socket_write(int fd, char *data, int length);
int socket_read(int fd, char *data, int length);
g_uint32 abuffer_checksum(char *buffer, g_uint32 length);

extern g_system_t my_ip;
extern char msg_id_ar[4];

#endif  /* _GTALK_ABUFCOMMON_H */
