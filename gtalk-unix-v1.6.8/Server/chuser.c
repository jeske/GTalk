/***********************************

     Channel List Maintenance

 ***********************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <termios.h>
#include <arpa/telnet.h>
#include <sys/wait.h>

#include "gt.h"
#include "fork.h"
#include "str.h"
#include "list.h"
#include "common.h"
#include "srv_abuf.h"
#include "telnd.h"
#include "srv_channel.h"
#include "states.h"
#include "chuser.h"

int channels_by_name(channel *c1, channel *c2)
{
  return (strcmp(c1->name, c2->name));
}

int channel_user_by_node(channel_user *c1, channel_user *c2)
{
  if (c1->system != c2->system)
    return ((int)(c1->system - c2->system));
  return (c1->node - c2->node);
}

int init_channels(void)
{
  if (!new_list(&channels, sizeof(channel)))
    {
      log_error("Could not create channel list!");
      return (-1);
    }
  if (!add_index(&channels, channels_by_name))
    {
      log_error("Could not add channel name index!");
      return (-1);
    }
  return (0);
} 

int find_num_channel_by_name(char *channame)
{
  channel chtest;

  get_string(chtest.name, &channame, sizeof(chtest.name)-1, 1, 1, 1);
  return (search_list(&channels, 0, &chtest));
}

channel *find_channel_by_name(char *channame)
{
  int ind;
  
  if ((ind=find_num_channel_by_name(channame)) < 0)
    return (NULL);
  return (element_of_index(channel, &channels, ind, 0));
}

int find_num_channel_user_by_name(channel *chan, 
				  g_system_t system, node_id node)
{
  channel_user cl;
  
  cl.node = node;
  cl.system = system;
  return search_list(&(chan->channel_users), 0, &cl);
}

channel_user *find_channel_user_by_name(channel *chan,
					g_system_t system, node_id node)
{
  int ind;

  if ((ind=find_num_channel_user_by_name(chan, system, node)) < 0)
    return (NULL);
  return (element_of_index(channel_user, &(chan->channel_users),
			   ind, 0));
}

int delete_user_from_channel(channel *chan, g_system_t system, node_id node)
{
  int ind;

  if ((ind = find_num_channel_user_by_name(chan, system, node)) >= 0)
    {
      ind = real_index_no(&(chan->channel_users), ind, 0);
      if (delete_list(&(chan->channel_users), ind))
	return (0);
      if (!elements(&(chan->channel_users)))
	{
	  if (!(chan->channel_flags & CHANNEL_STAYOPEN))
	    delete_channel(chan->name);
	}
    }
  return (-1);
}

void delete_user_from_channels(g_system_t system, node_id node)
{
  int i;
  channel *chan;

  for (i=0;i<elements(&channels);i++)
    {
      chan = element_of(channel, &channels, i);
      delete_user_from_channel(chan, system, node);
    }
}

int add_user_to_channel(channel *chan, g_system_t system, int node)
{
  int ind;
  channel_user cl;

  if ((ind = find_num_channel_user_by_name(chan, system, node)) >= 0)
    return (-1);
  cl.node = node;
  cl.system = system;
  cl.access_flags = 0;
  cl.lineout_counter=0;
  if (!add_list(&(chan->channel_users), &cl))
    return (-1);
  return (0);
}
 
int add_channel(char *name)
{
  channel ch;

  if (find_num_channel_by_name(name) >= 0)
    return (-1);
  get_string(ch.name, &name, sizeof(ch.name)-1, 1, 1, 1);
  if (!new_list(&ch.channel_users, sizeof(channel_user)))
    return (-1);
  if (!add_index(&ch.channel_users, channel_user_by_node))
    {
      free_list(&ch.channel_users);
      return (-1);
    }
  ch.channel_flags = CHANNEL_ALLJOIN|CHANNEL_ALLPOST;
  ch.max_lineout_counter=9;
  *ch.title = '\000';
  if (!add_list(&channels, &ch))
    return (-1);
  return (0);
}
    
int delete_channel(char *name)
{
  int ind;
  channel *chan;

  if ((ind=find_num_channel_by_name(name)) < 0)
    return (-1);
  ind = real_index_no(&channels, ind, 0);
  chan = element_of(channel, &channels, ind);
  free_list(&(chan->channel_users));
  if (!delete_list(&channels, ind))
    return (-1);
  return (0);
}


  

