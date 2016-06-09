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

#include "str.h"
#include "list.h"
#include "common.h"
#include "gtmain.h"
#include "abuf.h"
#include "channel.h"
#include "gtmain.h"
#include "states.h"
#include "channelcli.h"

#include "input.h"
#include "ddial.h"
#include "shared.h"

token_entry_type client_ddial_ch_tokens[] =
{
  { "BANNED", single_error, T_CH_BANNED },
  { "CHANNELERROR", single_error, T_CH_CHANNELERROR },
  { "INVITED", single_error, T_CH_INVITED },
  { "JOINCHANNEL", join_channel_msg, T_CH_JOINCHANNEL },
  { "LEAVECHANNEL", leave_channel_msg, T_CH_LEAVECHANNEL },
  { "MESSAGE", ddial_receive_message, T_CH_MESSAGE },
  { "NOBELONG", single_error, T_CH_NOBELONG },
  { "NOCHANNEL", single_error, T_CH_NOCHANNEL },
  { "NOMODERATE", single_error, T_CH_NOMODERATE },
  { "NOPERMCREATE", single_error, T_CH_NOPERMCREATE },
  { "NOTINVITED", single_error, T_CH_NOTINVITED },
  { "NOWRITE", single_error, T_CH_NOWRITE },
  { "PRIVATE", single_error, T_CH_PRIVATE },
  { "UNBANNED", single_error, T_CH_UNBANNED },
  { "UNINVITED", single_error, T_CH_UNINVITED }
};

token_list client_ddial_channel_tok = { 15, client_ddial_ch_tokens };


list ddial_state_list;
state_machine ddial_link_state_list[] = 
{
  { STATE_CHANNEL, client_ddial_channel_process },
/*  { STATE_PRIVATE, client_private_process }, */
  { -1, NULL }
};

int link_write_to_channel(char *channel, char *message)
{
  client_abuf_writef(my_ip, SERVER_PROCESS, STATE_DDIAL,
		     "%s %s", channel, message);
}

int ddial_receive_message(abuffer *abuf, char *message)
{
  char channel[CHANNEL_NAME_LEN+1];
  g_system_t system;
  node_id node;
  node_struct *nd;
  char s[1000];

  get_string(channel, &message, sizeof(channel)-1, 1, 0, 1);
  if (read_system_node(&message, &system, &node) < 0)
    return (-1);
  message = skip_blanks(message);

  if (node==mynum) /* if it's from us, ignore it */
    return 0;

  if (system == my_ip)
    {
      if ((node >= c_nodes_used) || 
	  ((nd=c_nodes(node))->status != NODE_ACTIVE))
	return (-1); 
      printf("#%02d:[%s|*r1] %s |*r1\r\n",
	     node, nd->userdata.user_info.handle, message);
    }
  return (0);
}


int client_ddial_channel_process(abuffer *abuf, char *message)
{
  channel_func ten;
  
  if ((abuf->source_machine==my_ip) && (abuf->source_process==mynum))
     return 0;

  if (ten=get_token(&message, &client_ddial_channel_tok, NULL)) 
      (ten)(abuf, message);
}


void link_main_loop(void)
{
  fd_set read_fd;
  char s[1024];
  int temp;
  abuffer abuf;
  int ansi_state = ansi_on(0);

  init_state_list(ddial_link_state_list, &ddial_state_list);
  printf("\r\n--> Linked\r\n");

  ml_logout = 0;
  while (!ml_logout)
    {
      FD_ZERO(&read_fd);
      FD_SET(mypipe, &read_fd);
      FD_SET(0, &read_fd);
      
      temp = select(mypipe+1, &read_fd, NULL, NULL, NULL);
      if (temp > 0)
	{
	  if (FD_ISSET(0, &read_fd))
	    {
	      get_input_cntrl(s,sizeof(s)-1,GI_FLAG_NO_ECHO);
	      if (*s == '/')
		process_command(s);
	      else if (*s)
		{
		  link_write_to_channel(mynode->cur_chan, s);
		}
	    }
	  if (FD_ISSET(mypipe, &read_fd))
	    {
	      if (read_abuffer(mypipe, &abuf, s, sizeof(s)-1) > 0)
		  call_state_machine(&abuf, s, &ddial_state_list);
	    }
	}
    }

   ansi_on(ansi_state);
};      


int client_ddial_process(abuffer *abuf, char *message)
{  
  g_system_t system;
  node_id node;
  char channel[CHANNEL_NAME_LEN+1];
  node_struct *nd;
  char s[2000];

  get_string(channel, &message, sizeof(channel)-1, 1, 0, 1);
  if (read_system_node(&message, &system, &node) < 0)
    return (-1);
  message = skip_blanks(message);
  if (system == my_ip)
    {
      if ((node >= c_nodes_used) || 
	  ((nd=c_nodes(node))->status != NODE_ACTIVE))
	return (-1); 
      sprintf(s,"%02d%s |*r1",
	     node,message);
      wrap_line(s);
    }
  return (0);
}

