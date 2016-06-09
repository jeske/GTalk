
/*********************************

         Channel.h

 *********************************/

#ifndef _GTALK_CHANNELCLI_H
#define _GTALK_CHANNELCLI_H

#include "common.h"
#include "types.h"
#include "list.h"
#include "states.h"
#include "channel.h"

int single_error(abuffer *abuf, char *message);
int double_error(abuffer *abuf, char *message);
int join_channel_msg(abuffer *abuf, char *message);
int leave_channel_msg(abuffer *abuf, char *message);
int receive_message(abuffer *abuf, char *message);
int private_message(abuffer *abuf, char *message);
int client_message(abuffer *abuf, char *message);
int client_channel_process(abuffer *abuf, char *message);
int write_to_channel(char *channel, char *message);

#endif   /* _GTALK_CHANNELCLI_H */
