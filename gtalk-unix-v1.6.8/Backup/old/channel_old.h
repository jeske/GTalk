
/*********************************

         Channel.h

 *********************************/

#ifndef _GTALK_CHANNEL_H
#define _GTALK_CHANNEL_H

#include "common.h"
#include "types.h"

#define CHANNEL_NAME_LEN 20
#define CHANNEL_TITLE 40

#define USER_NAME_LEN 20

typedef struct _user_channel_entry
{
  g_uint32 system;
  char name[USER_NAME_LEN+1];
} user_channel_entry;

typedef struct _channel
{
  struct _channel           *next;
  char                       name[CHANNEL_NAME_LEN+1];
  char                       title[CHANNEL_TITLE+1];
  int                        channel_num;
  nodeset                    monitors;
  int                        num_monitor;
  nodeset                    onchannel;
  int                        num_onchannel;
  nodeset                    moderator;
  int                        num_moderator;
  nodeset                    barred;
  int                        num_barred;
} channel;

int channel_process(g_uint32 type, abuf *abuf, char *message);

#endif   /* _GTALK_CHANNEL_H */
