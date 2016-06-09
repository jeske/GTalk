/***********************************

     Channel List Maintenance

 ***********************************/

#ifndef _CHUSER_H
#define _CHUSER_H

#include "channel.h"

int init_channels(void);
void delete_user_from_channels(g_system_t system, node_id node);
int find_num_channel_by_name(char *channame);
int find_num_channel_user_by_name(channel *chan, 
				  g_system_t system, node_id node);
channel *find_channel_by_name(char *channame);
channel_user *find_channel_user_by_name(channel *chan,
					g_system_t system, node_id node);
int delete_user_from_channel(channel *chan, g_system_t system, node_id node);
int add_user_to_channel(channel *chan, g_system_t system, int node);
int add_channel(char *name);
int delete_channel(char *name);

#endif  /* _CHUSER_H */

