
/*********************************

         srv_channel.h

 *********************************/

#ifndef GT_SRV_CHANNEL_H
#define GT_SRV_CHANNEL_H

#include "common.h"
#include "types.h"
#include "list.h"
#include "states.h"
#include "channel.h"

#define CHACCESS_READ 0x01
#define CHACCESS_WRITE 0x02
#define CHACCESS_MODERATE 0x04
#define CHACCESS_BANNED 0x08
#define CHACCESS_INVITED 0x10

typedef struct _channel_user
{
  g_system_t system;
  node_id    node;
  int        access_flags;
  int        lineout_counter;
} channel_user;

#define CHANNEL_ALLJOIN 0x01
#define CHANNEL_STAYOPEN 0x02
#define CHANNEL_ALLPOST 0x04

typedef struct _channel
{
  char                       name[CHANNEL_NAME_LEN+1];
  char                       title[CHANNEL_TITLE+1];
  list                       channel_users;
  int                        channel_flags;
  int                        max_lineout_counter;
} channel;

extern list channels;

int read_system_node(char **c, g_system_t *system, node_id *node);

int channel_process(abuffer *abuf, char *message);
int setperm_channel_message(abuffer *abuf, char *message);
int setchan_channel_message(abuffer *abuf, char *message);
int leave_channel_message(abuffer *abuf, char *message);
int join_channel_message(abuffer *abuf, char *message);
int login_channel_message(abuffer *abuf, char *message);
int distribute_message(abuffer *abuf, char *message);
void take_off_channels(node_id node, char *reason);
int binary_msg(abuffer *abuf, char *message, char *msg);
int bye_message(abuffer *abuf, char *message);
int wall_message(abuffer *abuf, char *message);
int walla_message(abuffer *abuf, char *message);
int loginlurk_message(abuffer *abuf, char *message);
int lurk_message(abuffer *abuf, char *message);


#endif   /* GT_SRV_CHANNEL_H */





