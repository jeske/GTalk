/***********************************

           Channel List

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

list channels;

#define STRFTIME_CONFIG_STRING "%H:%M:%S"

token_entry_type server_ch_tokens[] =
{
  { "BINARY",  binary_msg, T_CH_BINARY },
  { "BYE",     bye_message, T_CH_BYE },
  { "JOIN",    join_channel_message, T_CH_JOIN },
  { "LEAVE",   leave_channel_message, T_CH_LEAVE },
  { "MESSAGE", distribute_message, T_CH_MESSAGE },
  { "SETCHAN", setchan_channel_message, T_CH_SETCHAN },
  { "SETPERM", setperm_channel_message, T_CH_SETPERM },
  { "WALL",    wall_message, T_WALL },
  { "WALLA",   walla_message, T_WALLA },
  { "MSGLURK", lurk_message, T_MSG_LURK },
  { "LOGINLURK", loginlurk_message, T_LOGIN_LURK }
};

/*
sizeof(server_ch_tokens)/sizeof(token_entry_type)
*/
token_list server_channel_tok = { 11, server_ch_tokens };

int is_on_channel(channel_user *cuser, channel_user *cuser2)
{
  if ((!(cuser->access_flags & CHACCESS_READ)) ||
      (cuser->access_flags & CHACCESS_BANNED))
    return (0);
  return (1);
}

int channel_stat_change(char *name, char *reason, g_system_t system, 
                        node_id node,
			g_system_t e_system, node_id e_node,
			int newchannel, int newuser,
			int read, int write, int moderate,
			int invited, int banned, int forcemsg)
{
  int i, is_new_chan = 0;

  time_t tim = time(NULL);
  char time_string[50];
  char user_no_info[4+1];

  channel *chan;
  channel_user *cuser, *ncuser;
  channel_user oldcuser;

  node_struct *nd;

  strftime(time_string,33,STRFTIME_CONFIG_STRING,localtime(&tim));

  if ((system == my_ip) && (node < c_nodes_used) &&
      (is_node_online(c_nodes(node))))
    nd = c_nodes(node);
  else
    nd = NULL;

  if (nd->userdata.user_info.enable==0)
        sprintf(user_no_info,"(Guest)");
  else
        sprintf(user_no_info,"#%03d", nd->userdata.user_info.user_no);
 
  if (!(chan=find_channel_by_name(name)))
    {
      if (!newchannel)
	{
	  if (e_node >= 0)
	    server_abuf_writef(e_system, e_node, STATE_CHANNEL,
	     "NOPERMCREATE %s No permission to create channel %s", name, name);
	  return (-1);
	}
      if (nd && (!(testFlag(nd, "CHN_ADDCHANNEL"))))
	{
	  if (e_node >= 0)
	    server_abuf_writef(e_system, e_node, STATE_CHANNEL,
	     "NOPERMCREATE %s No permission to create channel %s", name, name);
	  return (-1);
	}
      if (add_channel(name) < 0)
	{
	  if (e_node >= 0)
	    server_abuf_writef(e_system, e_node, STATE_CHANNEL,
	     "CHANNELERROR %s Unable to add channel error channel %s", 
			       name, name);
	  return (-1);
	}
      if (!(chan=find_channel_by_name(name)))
	{
	  if (e_node >= 0)
	    server_abuf_writef(e_system, e_node, STATE_CHANNEL,
	     "CHANNELERROR %s Unable to find channel error channel %s", 
			       name, name);
	  return (-1);
	}
      is_new_chan = 1;
    } 
  if (cuser=find_channel_user_by_name(chan, system, node))
    {
      if ((!nd) || (!testFlag(nd, "CHN_ALLMODERATE")))
	{
	  if ((banned < 0) && (cuser->access_flags & CHACCESS_BANNED))
	    {
	      if (e_node >= 0)
		server_abuf_writef(e_system, e_node, STATE_CHANNEL,
			 "BANNED %s You are banned from channel %s", name,
			  name);
	      return (-1);
	    }
	  if ((!(chan->channel_flags & CHANNEL_ALLJOIN)) &&
	      (!(cuser->access_flags & CHACCESS_INVITED)))
	    {
	      if (e_node >= 0)
		server_abuf_writef(e_system, e_node, STATE_CHANNEL,
			"NOTINVITED %s You are not invited to channel %s",
			 name, name);
	      return (-1);
	    }
	} else
	  banned = 0;
  } else
    {
      if (!newuser)
	{
	  if (e_node >= 0)
	    server_abuf_writef(e_system, e_node, STATE_CHANNEL,
			       "NOBELONG %s You do not belong to channel %s", name, name);
	  return (-1);
	}
      if (!(chan->channel_flags & CHANNEL_ALLJOIN))
	{
	  if (e_node >= 0)
	    server_abuf_writef(e_system, e_node, STATE_CHANNEL,
			       "PRIVATE %s Channel %s is a private channel", name, name);
	  return (-1);
	}
      if (add_user_to_channel(chan, system, node) < 0)
	{
	  if (e_node >= 0)
	    server_abuf_writef(e_system, e_node, STATE_CHANNEL,
			       "CHANNELERROR %s Unable to add you to channel %s", 
			       name, name);
	    return (-1);
	  }
	if (!(cuser=find_channel_user_by_name(chan, system, node)))
	  {
	    if (e_node >= 0)
	      server_abuf_writef(e_system, e_node, STATE_CHANNEL,
		"CHANNELERROR %s Unable to find you on channel %s", 
				 name, name);
	    return (-1);
	  }
      }
  oldcuser = *cuser;
  if (read >= 0)
    {
      if (read)
	cuser->access_flags |= CHACCESS_READ;
      else
	cuser->access_flags &= ~CHACCESS_READ;
    }
  if (write >= 0)
    {
      if (write)
	cuser->access_flags |= CHACCESS_WRITE;
      else
	cuser->access_flags &= ~CHACCESS_WRITE;
    }
  if (moderate >= 0)
    {
      if (moderate && (((is_new_chan && (nd && testFlag(nd,"CHN_MODERATE")))) ||
		       (nd && testFlag(nd,"CHN_ALLMODERATE"))))
	cuser->access_flags |= CHACCESS_MODERATE;
      else
	cuser->access_flags &= ~CHACCESS_MODERATE;
    }
  if (invited >= 0)
    {
      if (invited)
	{
	  if (!(cuser->access_flags & CHACCESS_INVITED))
	    {
	      server_abuf_writef(system, node, STATE_CHANNEL,
		"INVITED %s You are invited to channel %s", 
				 chan->name, chan->name);
	      cuser->access_flags |= CHACCESS_INVITED;
	    }
	} else
	  if (cuser->access_flags & CHACCESS_INVITED)
	    {
	      server_abuf_writef(system, node, STATE_CHANNEL,
		"UNINVITED %s You are uninvited to channel %s", 
				 chan->name, chan->name);
	      cuser->access_flags &= ~CHACCESS_INVITED;
	    }
    }
  if (banned >= 0)
    {
      if (banned)
	{
	  if (!(cuser->access_flags & CHACCESS_BANNED))
	    {
	      server_abuf_writef(system, node, STATE_CHANNEL,
		"BANNED %s You are banned from channel %s",
				 chan->name, chan->name);
	      cuser->access_flags |= CHACCESS_BANNED;
	      cuser->access_flags &= ~CHACCESS_READ;
	    }
        } else
	  if (cuser->access_flags & CHACCESS_BANNED)
	    {
	      server_abuf_writef(system, node, STATE_CHANNEL,
		 "UNBANNED %s You are unbanned from channel %s",
				 chan->name, chan->name);
	      cuser->access_flags &= ~CHACCESS_BANNED;
	    }
    }
  for (i=0;i<elements(&(chan->channel_users));i++)
    {
      ncuser = element_of(channel_user, &(chan->channel_users), i);

      if (is_on_channel(ncuser, NULL))
	{
	  if ((forcemsg || !is_on_channel(&oldcuser,ncuser)) && 
	      (is_on_channel(cuser,ncuser)))
	    {
	      if (nd)
		{
		  if ((reason) && (reason[0])) {
			if (!(strcmp(reason,"Login"))) {
			   server_abuf_writef(ncuser->system, ncuser->node,
			   	STATE_CHANNEL,
				"JOINCHANNEL %s %lu/%d "
				"\007|*ffNode [%02d]: Login (%s) at %s",
				chan->name, system, node, node, chan->name,
				time_string);
			   server_abuf_writef(ncuser->system, ncuser->node,
				STATE_CHANNEL,
				"JOINCHANNEL %s %lu/%d "
				"|*ff%s:%c|*r1%s|*ff%c",
				chan->name, system, node,
				user_no_info,
				nd->userdata.online_info.class_info.staple[2],
				nd->userdata.user_info.handle,
				nd->userdata.online_info.class_info.staple[3]);
			} else {
		    	   server_abuf_writef(ncuser->system, ncuser->node, 
		       		STATE_CHANNEL,
		       		"JOINCHANNEL %s %lu/%d "
		       		"#%02d:%c%s|*r1%c joined (%s|*r1) from %s",
		       		chan->name, system, node, node,
		       		nd->userdata.online_info.class_info.staple[2],
		       		nd->userdata.user_info.handle, 
		       		nd->userdata.online_info.class_info.staple[3],
		       		chan->name, reason);
			}
		  } else {
		    server_abuf_writef(ncuser->system, ncuser->node, 
		       STATE_CHANNEL,
		       "JOINCHANNEL %s %lu/%d "
		       "#%02d:%c%s|*r1%c joined (%s|*r1)",
		       chan->name, system, node, node,
		       nd->userdata.online_info.class_info.staple[2],
		       nd->userdata.user_info.handle, 
		       nd->userdata.online_info.class_info.staple[3],
		       chan->name);
		  }
	      } else
		{
		  server_abuf_writef(ncuser->system, ncuser->node, 
			STATE_CHANNEL,
			"JOINCHANNEL %s %lu/%d",
			chan->name, system, node);
		}
	    }
	  if ((forcemsg || is_on_channel(&oldcuser,ncuser))
	      && (!is_on_channel(cuser,ncuser)))
	    {
	      if (nd)
		{
		  if ((reason) && (reason[0])) {
                        if (!strcmp(reason,"Logout")) {
                           server_abuf_writef(ncuser->system, ncuser->node,
                                STATE_CHANNEL,
                                "LEAVECHANNEL %s %lu/%d "
                                "|*r1|*f1Node [%02d]: Logout (%s) at %s",
                                chan->name, system, node, node, chan->name,
				time_string);
                           server_abuf_writef(ncuser->system, ncuser->node,
                                STATE_CHANNEL,
                                "LEAVECHANNEL %s %lu/%d "
                                "|*r1|*f1%s:%c|*r1%s|*r1|*f1%c|*r1",
                                chan->name, system, node,
                                user_no_info,
                                nd->userdata.online_info.class_info.staple[2],
                                nd->userdata.user_info.handle,
                                nd->userdata.online_info.class_info.staple[3]);
                        } else if (!strcmp(reason,"Killed")) {
                           server_abuf_writef(ncuser->system, ncuser->node,
                                STATE_CHANNEL,
                                "LEAVECHANNEL %s %lu/%d "
                                "|*r1|*f1Node [%02d]: |*f9*TOASTED*|*r1|*f1 at %s",
                                chan->name, system, node, node,
				time_string);
                           server_abuf_writef(ncuser->system, ncuser->node,
                                STATE_CHANNEL,
                                "LEAVECHANNEL %s %lu/%d "
                                "|*r1|*f1%s:%c|*r1%s|*r1|*f1%c|*r1",
                                chan->name, system, node,
                                user_no_info,
                                nd->userdata.online_info.class_info.staple[2],
                                nd->userdata.user_info.handle,
                                nd->userdata.online_info.class_info.staple[3]);
                        } else if (!strcmp(reason,"Timeout")) {
                           server_abuf_writef(ncuser->system, ncuser->node,
                                STATE_CHANNEL,
                                "LEAVECHANNEL %s %lu/%d "
                                "|*r1|*f1Node [%02d]: Timeout at %s",
                                chan->name, system, node, node,
				time_string);
                           server_abuf_writef(ncuser->system, ncuser->node,
                                STATE_CHANNEL,
                                "LEAVECHANNEL %s %lu/%d "
                                "|*r1|*f1%s:%c|*r1%s|*r1|*f1%c|*r1",
                                chan->name, system, node,
                                user_no_info,
                                nd->userdata.online_info.class_info.staple[2],
                                nd->userdata.user_info.handle,
                                nd->userdata.online_info.class_info.staple[3]);
			} else {
			   server_abuf_writef(ncuser->system, 
		      		ncuser->node, STATE_CHANNEL,
		      		"LEAVECHANNEL %s %lu/%d "
		      		"#%02d:%c%s|*r1%c left (%s) to %s",
		      		chan->name, system, node, node, 
 		      		nd->userdata.online_info.class_info.staple[2],
                      		nd->userdata.user_info.handle, 
                      		nd->userdata.online_info.class_info.staple[3],
                      		chan->name, reason);
			}
		  } else {
		    server_abuf_writef(ncuser->system, 
		      ncuser->node, STATE_CHANNEL,
		      "LEAVECHANNEL %s %lu/%d "
		      "#%02d:%c%s|*r1%c left (%s)",
		      chan->name, system, node, node, 
 		      nd->userdata.online_info.class_info.staple[2],
                      nd->userdata.user_info.handle, 
                      nd->userdata.online_info.class_info.staple[3],
                      chan->name);
		  }

	      } else
		{
		  server_abuf_writef(ncuser->system, 
                      ncuser->node, STATE_CHANNEL,
		      "LEAVECHANNEL %s %lu/%d",
		      chan->name, system, node);
		}
	    }
	}
    }
  if (!(cuser->access_flags))
    {
      if (delete_user_from_channel(chan, system, node) < 0)
	{
	  if (e_node >= 0)
	    {
	      server_abuf_writef(e_system, e_node, STATE_CHANNEL,
		"CHANNELERROR %s Could not delete user from channel %s",
				 name, name);
	      return (-1);
	    }
	}
    }
  return (0);
}

void take_off_channels(node_id node, char *reason)
{
  int i;
  channel *chan;
  channel_user *cuser;

  for (i=0;i<elements(&channels);i++)
    {
      chan = element_of(channel, &channels, i);

      channel_stat_change(chan->name, reason, my_ip, node, -1, -1,
			  0, 0, 0, 0, 0, 0, 0, 0);
    }
}

int get_channel_st(char *channame, abuffer *abuf,
		   channel **chan, channel_user **cuser)
{
  if (!(*chan = find_channel_by_name(channame)))
    {
      server_abuf_writef(abuf->source_machine, abuf->source_process,
	STATE_CHANNEL,
	"NOCHANNEL %s The channel %s does not exist", channame, channame);
      return (-1);
    }
  if (!(*cuser = find_channel_user_by_name(*chan, abuf->source_machine,
                     abuf->source_process)))
    {
      server_abuf_writef(abuf->source_machine, abuf->source_process,
	STATE_CHANNEL,
	"NOBELONG %s You do not belong to channel %s", channame, channame);
      return (-1);
    }
  return (0);
}

int check_banned_from(channel_user *cuser, channel *chan, abuffer *abuf)
{
  if (cuser->access_flags & CHACCESS_BANNED)
    {
      server_abuf_writef(abuf->source_machine, abuf->source_process,
	STATE_CHANNEL,
	"BANNED %s You are banned from channel %s", chan->name, chan->name);
      return (-1);
    }
  return (0);
}

int check_write_to(channel_user *cuser, channel *chan, abuffer *abuf)
{
  if (!(cuser->access_flags & CHACCESS_WRITE))
    {
      server_abuf_writef(abuf->source_machine, abuf->source_process,
	STATE_CHANNEL,
	"NOWRITE %s You are unable to write to channel %s", 
	chan->name, chan->name);
      return (-1);
    }
  return (0);
}

int check_moderate(channel_user *cuser, channel *chan, abuffer *abuf)
{
  if (!(cuser->access_flags & CHACCESS_MODERATE))
    {
      server_abuf_writef(abuf->source_machine, abuf->source_process,
	STATE_CHANNEL,
	"NOMODERATE %s You do not moderate channel %s", 
	chan->name, chan->name);
      return (-1);
    }
  return (0);
}

int setchan_channel_message(abuffer *abuf, char *message)
{
  channel *chan;
  channel_user *cuser;
  char ch;
  unsigned long int num;
  char channel[CHANNEL_NAME_LEN+1];

  get_string(channel, &message, sizeof(channel)-1, 1, 0, 1);
  if (get_channel_st(channel, abuf, &chan, &cuser) < 0)
    return (-1);
  if (check_banned_from(cuser, chan, abuf) < 0)
    return (-1);
  if (check_moderate(cuser, chan, abuf) < 0)
    return (-1);
  message = skip_blanks(message);

  while (*message)
    {
      ch = (*message > 'Z') ? (*message - ' ') : *message;
      switch (ch)
	{
	  case 'A': chan->channel_flags |= CHANNEL_ALLJOIN;
	            break;
	  case 'a': chan->channel_flags &= ~CHANNEL_ALLJOIN;
	            break;
	  case 'S': chan->channel_flags |= CHANNEL_STAYOPEN;
	            break;
	  case 's': chan->channel_flags &= ~CHANNEL_STAYOPEN;
	            break;
	  case 'P': chan->channel_flags |= CHANNEL_ALLPOST;
	            break;
	  case 'p': chan->channel_flags &= ~CHANNEL_ALLPOST;
	            break;
	  case 'L':
	  case 'l': message++;
	            if (get_number(&message, &num))
		      {
			if ((num >= 3) && (num <= 10000000))
			  chan->max_lineout_counter = num;
			message--;
		      }
	            break;
	}
      message++;
    }
}

int setperm_channel_message(abuffer *abuf, char *message)
{
  channel *chan;
  channel_user *cuser, *ncuser;
  g_system_t system;
  node_id node;
  char ch;
  int x_read = -1, x_write = -1;
  int x_banned = -1, x_invite = -1;
  int x_lurklv = -1, x_moderate = -1;
  int adduser = 0;
  int *modpriv = NULL;
  int *modnum = NULL;
  unsigned long int num;
  char channel[CHANNEL_NAME_LEN+1];
  char reason[CHANNEL_NAME_LEN+1];

  reason[0]=0;

  get_string(channel, &message, sizeof(channel)-1, 1, 0, 1);
  if (get_channel_st(channel, abuf, &chan, &cuser) < 0)
    return (-1);
  if (check_banned_from(cuser, chan, abuf) < 0)
    return (-1);
  if (check_moderate(cuser, chan, abuf) < 0)
    return (-1);
  message = skip_blanks(message);
  if (read_system_node(&message, &system, &node) < 0)
    return (-1);
  message = skip_blanks(message);
  while (*message)
    {
      ch = (*message > 'Z') ? (*message - ' ') : *message;
      switch (ch)
	{
	  case 'R': modpriv = &x_read;
	            break;
	  case 'W': modpriv = &x_write;
	            break;
	  case 'B': modpriv = &x_banned;
	            adduser = 1;
	            break;
	  case 'I': modpriv = &x_invite;
	            adduser = 1;
	            break;
	  case 'M': modpriv = &x_moderate;
	            break;
	  case 'L': modnum = &x_lurklv;
	            break;
	  case '+': if (modpriv)
	              *modpriv = 1;
	            break;
	  case '-': if (modpriv)
	              *modpriv = 0;
	            break;
	  default:  if ((*message >= '0') && (*message <= '9'))
	              {
			if (get_number(&message, &num))
			  {
			    if (modnum)
			      *modnum = num;
			  }
			message--;
		      }
	            break;
	}
      message++;
    }
  return (channel_stat_change(channel,reason, system, node,
			      system, node,
			      0, adduser,
			      x_read, x_write, x_moderate,
			      x_invite, x_banned, 0));
}

int distribute_message(abuffer *abuf, char *message)
{
  int i, visusr;
  channel *chan;
  channel_user *cuser, *ncuser;
  char channel[CHANNEL_NAME_LEN+1];

  get_string(channel, &message, sizeof(channel)-1, 1, 0, 1);
  if (get_channel_st(channel, abuf, &chan, &cuser) < 0)
    return (-1);
  if (check_banned_from(cuser, chan, abuf) < 0)
    return (-1);
  if (check_write_to(cuser, chan, abuf) < 0)
    return (-1);

  message = skip_blanks(message);

  visusr = 0;
  for (i=0;i<elements(&(chan->channel_users));i++)
    {
      ncuser = element_of(channel_user,&(chan->channel_users),i);

      if (is_on_channel(ncuser, cuser))
	{
	  visusr++;
	  server_abuf_writef(ncuser->system, ncuser->node,
			     STATE_CHANNEL,
			     "MESSAGE %s %lu/%d %s",
			     channel, cuser->system, cuser->node, message);
	  if ((ncuser->system != cuser->system) ||
	      (ncuser->node != cuser->node)) 
	    ncuser->lineout_counter = 0;
	  else
	    ncuser->lineout_counter++;
	}
    }
  if (visusr < 2)
    cuser->lineout_counter = 0;
  if ((cuser->lineout_counter >= chan->max_lineout_counter) &&
      (!(cuser->access_flags & CHACCESS_MODERATE)))
    {
      channel_stat_change(channel, "lineout", cuser->system, cuser->node,
			  cuser->system, cuser->node,
			  0, 0, -1, -1, -1, -1, 1, 0);
      c_nodes(cuser->node)->sigusr1_action=SIG1_ACTION_LINEOUT;
      kill(c_devices(c_nodes(cuser->node)->dev_no)->owner_pid,SIGUSR1);
      return (0);
    }
  return (0);
}

int join_channel_message(abuffer *abuf, char *message)
{
  char channel[CHANNEL_NAME_LEN+1];
  char reason[CHANNEL_NAME_LEN+1];

  reason[0]=0;

  get_string(channel, &message, sizeof(channel)-1, 1, 0, 1);

  message = skip_blanks(message);

  if (*message) {
    strncpy(reason,message,sizeof(reason)-1);
    reason[sizeof(reason)-1]=0;
  }
  
  return (channel_stat_change(channel, reason, abuf->source_machine,
	   abuf->source_process, abuf->source_machine,
           abuf->source_process, 
           1, 1, 1, 1, 1, -1, -1, 1));
}

int login_channel_message(abuffer *abuf, char *message)
{
  char channel[CHANNEL_NAME_LEN+1];
  char reason[CHANNEL_NAME_LEN+1];

  reason[0]=0;

  get_string(channel, &message, sizeof(channel)-1, 1, 0, 1);

  message = skip_blanks(message);

  if (*message) {
    strncpy(reason,message,sizeof(reason)-1);
    reason[sizeof(reason)-1]=0;
  }

  return (channel_stat_change(channel, reason, abuf->source_machine,
           abuf->source_process, abuf->source_machine,
           abuf->source_process,
           1, 1, 1, 1, 1, -1, -1, 1));
}


int bye_message(abuffer *abuf, char *message)
{
  message = skip_blanks(message);
  if (abuf->source_machine == my_ip)
    take_off_channels(abuf->source_process, message);
  return (0);
}

int leave_channel_message(abuffer *abuf, char *message)
{
  char channel[CHANNEL_NAME_LEN+1];
  char reason[CHANNEL_NAME_LEN+1];

  reason[0]=0;

  get_string(channel, &message, sizeof(channel)-1, 1, 0, 1);
  message = skip_blanks(message);
  if (*message) {
    strncpy(reason,message,sizeof(reason)-1);
    reason[sizeof(reason)-1]=0;
  }
  return (channel_stat_change(channel, reason, abuf->source_machine,
	   abuf->source_process, abuf->source_machine,
           abuf->source_process, 
           0, 0, 0, 0, 0, 0, -1, 1));
}

int binary_msg(abuffer *abuf, char *message, char *msg)
{
  int i;
  channel *chan;
  channel_user *cuser, *ncuser;
  char channel[CHANNEL_NAME_LEN+1];

  get_string(channel, &message, sizeof(channel)-1, 1, 0, 1);
  if (get_channel_st(channel, abuf, &chan, &cuser) < 0)
    return (-1);
  if (check_banned_from(cuser, chan, abuf) < 0)
    return (-1);
  if (check_write_to(cuser, chan, abuf) < 0)
    return (-1);

  for (i=0;i<elements(&(chan->channel_users));i++)
    {
      ncuser = element_of(channel_user,&(chan->channel_users),i);

      if ((ncuser->node != abuf->source_process) && 
	  (is_on_channel(ncuser, cuser)))
	server_abuf_write(ncuser->system, ncuser->node,
           STATE_CHANNEL, msg, abuf->payload_length);
    }
  return (0);
}

int channel_process(abuffer *abuf, char *message)
{
  char *msg = message;
  channel_func ten;

/*  log_error("in channel_process(). finding msg type."); Basic Debugging */
  if (ten = get_token(&message, &server_channel_tok, NULL)) 
    {
/*      log_error("In if (ten = get_token(......)"); */
      if (ten == (channel_func)binary_msg)
	{
/*	log_error("Type = Binary msg!"); */
	binary_msg(abuf, message, msg);
	}
      else
	{
/*	log_error("Msg Type = |%d|",ten); */
	(ten)(abuf, message);
	}
    }
  else {
	log_error("ten is NULL! Return from get_token()!");
  }
};

int wall_message(abuffer *abuf, char *message)
{
  int i, visusr;
  channel *chan;
  channel_user *cuser, *ncuser;
  char channel[CHANNEL_NAME_LEN+1];
  g_system_t system;
  node_id node;
  node_struct *nd;

  time_t tim = time(NULL);
  char time_string[50];
  strftime(time_string,33,STRFTIME_CONFIG_STRING,localtime(&tim));

  if ((system == my_ip) && (node < c_nodes_used) &&
      (is_node_online(c_nodes(node))))
    nd = c_nodes(node);
  else
    nd = NULL;

  get_string(channel, &message, sizeof(channel)-1, 1, 0, 1);
  if (get_channel_st(channel, abuf, &chan, &cuser) < 0)
    return (-1);
/*
  if (read_system_node(&message, &system, &node) < 0)
    return (-1);
*/

/*message = skip_blanks(message); */

  visusr = 0;
  for (i=0;i<elements(&(chan->channel_users));i++)
  for (i=0;i<=c_nodes_used;i++)
    {
      ncuser = element_of(channel_user,&(chan->channel_users),i);

      if (is_on_channel(ncuser, cuser))
        {
          visusr++;
          server_abuf_writef(ncuser->system, ncuser->node,
		STATE_CHANNEL,
		"WALL %s %lu/%d "
		"\007\007### Broadcasted Message from Node [%02d] at %s ###\r\n%s",
		channel, cuser->system, cuser->node,
		cuser->node,
        	time_string, message);

/*
		"\007\007### Broadcasted Message from #%02d:%c%s%c (%s) at %s ###\r\n%s",
		channel, cuser->system, cuser->node,
		cuser->node,
	        nd->userdata.online_info.class_info.staple[2],
        	nd->userdata.user_info.handle,
        	nd->userdata.online_info.class_info.staple[3],
        	channel, time_string, message);
*/
        }
    }
  return (0);
}

int walla_message(abuffer *abuf, char *message)
{
  int i, visusr;
  channel *chan;
  channel_user *cuser, *ncuser;
  char channel[CHANNEL_NAME_LEN+1];
  node_struct *nd;
  node_id node;

  time_t tim = time(NULL);
  char time_string[50];
  strftime(time_string,33,STRFTIME_CONFIG_STRING,localtime(&tim));

/*
  log_error("We're in walla_message()!");
*/

  if ((system == my_ip) && (node < c_nodes_used) &&
      (is_node_online(c_nodes(node))))
    nd = c_nodes(node);
  else
    nd = NULL;

  get_string(channel, &message, sizeof(channel)-1, 1, 0, 1);
  if (get_channel_st(channel, abuf, &chan, &cuser) < 0)
    return (-1);
  if (check_banned_from(cuser, chan, abuf) < 0)
    return (-1);
  if (check_write_to(cuser, chan, abuf) < 0)
    return (-1);

/*message = skip_blanks(message); */

  visusr = 0;
  for (i=0;i<elements(&(chan->channel_users));i++)
    {
      ncuser = element_of(channel_user,&(chan->channel_users),i);

      if (is_on_channel(ncuser, cuser))
        {
          visusr++;
          server_abuf_writef(ncuser->system, ncuser->node,
		STATE_CHANNEL,
		"WALLA %s %lu/%d %s",
		channel, cuser->system, cuser->node,
        	message);
        }
    }
  return (0);
}

int loginlurk_message(abuffer *abuf, char *message)
{
  int i, visusr;
  channel *chan;
  channel_user *cuser, *ncuser;
  char channel[CHANNEL_NAME_LEN+1];
  g_system_t system;
  node_id node;
  node_struct *nd;

  time_t tim = time(NULL);
  char time_string[50];
  strftime(time_string,33,STRFTIME_CONFIG_STRING,localtime(&tim));

/*log_error("We're in loginlurk_message! File: %s Line: %d", __FILE__, __LINE__); */

  if ((system == my_ip) && (node < c_nodes_used) &&
      (is_node_online(c_nodes(node))))
    nd = c_nodes(node);
  else
    nd = NULL;

  get_string(channel, &message, sizeof(channel)-1, 1, 0, 1);
  if (get_channel_st(channel, abuf, &chan, &cuser) < 0)
    return (-1);
  if (read_system_node(&message, &system, &node) < 0)
    return (-1);

/*message = skip_blanks(message); */

  visusr = 0;
  for (i=0;i<elements(&(chan->channel_users));i++)
  for (i=0;i<=c_nodes_used;i++)
    {
      ncuser = element_of(channel_user,&(chan->channel_users),i);

      if (is_on_channel(ncuser, cuser))
        {
          visusr++;
          server_abuf_writef(ncuser->system, ncuser->node,
		STATE_CHANNEL,
		"WALLA %s %lu/%d "
		"|*fc--> Login/LURK (%s) [%02d] at %s ###\r\n%s",
		channel, cuser->system, cuser->node,
		channel,
		cuser->node,
        	time_string, message);
        }
    }
  return (0);
}



#ifdef SCRAP

int scrap_lurk_message(abuffer *abuf, char *message)
{
  int i, visusr;
  channel *chan;
  channel_user *cuser, *ncuser;
  char channel[CHANNEL_NAME_LEN+1];
  node_struct *nd;
  node_id node;

  time_t tim = time(NULL);
  char time_string[50];
  log_error("We're in lurk_message()!!!");
  strftime(time_string,33,STRFTIME_CONFIG_STRING,localtime(&tim));

/* log_error("#0 srv_channel.c - We're in!  %s %s",__FILE__, __LINE__); */
  if ((system == my_ip) && (node < c_nodes_used) &&
      (is_node_online(c_nodes(node))))
    nd = c_nodes(node);
  else
    nd = NULL;

/* log_error("#1 srv_channel.c - We're in!  %s %s",__FILE__, __LINE__); */
  get_string(channel, &message, sizeof(channel)-1, 1, 0, 1);
  if (get_channel_st(channel, abuf, &chan, &cuser) < 0)
    return (-1);
  if (check_banned_from(cuser, chan, abuf) < 0)
    return (-1);
  if (check_write_to(cuser, chan, abuf) < 0)
    return (-1);

log_error("#2 srv_channel.c - We're in!  %s %s",__FILE__, __LINE__);

/*message = skip_blanks(message); */

  visusr = 0;
  for (i=0;i<elements(&(chan->channel_users));i++)
    {
      ncuser = element_of(channel_user,&(chan->channel_users),i);

      if (is_on_channel(ncuser, cuser))
        {
          visusr++;
          server_abuf_writef(ncuser->system, ncuser->node,
		STATE_CHANNEL,
		"MSGLURK %s %lu/%d %s",
		channel, cuser->system, cuser->node,
        	message);
        }
    }
  return (0);
}
#endif


int lurk_message(abuffer *abuf, char *message)
{
  int i, visusr;
  channel *chan;
  channel_user *cuser, *ncuser;
  char channel[CHANNEL_NAME_LEN+1];
  g_system_t system;
  node_id node;
  node_struct *nd;

  time_t tim = time(NULL);
  char time_string[50];
log_error("#1 ARE WE IN lurk_message() YET!?");
  strftime(time_string,33,STRFTIME_CONFIG_STRING,localtime(&tim));

log_error("#2 ARE WE IN lurk_message() YET!?");
  if ((system == my_ip) && (node < c_nodes_used) &&
      (is_node_online(c_nodes(node))))
    nd = c_nodes(node);
  else
    nd = NULL;

  get_string(channel, &message, sizeof(channel)-1, 1, 0, 1);
  if (get_channel_st(channel, abuf, &chan, &cuser) < 0)
    return (-1);
/*
  if (read_system_node(&message, &system, &node) < 0)
    return (-1);
*/

/*message = skip_blanks(message); */

  visusr = 0;
  for (i=0;i<elements(&(chan->channel_users));i++)
    {
      ncuser = element_of(channel_user,&(chan->channel_users),i);

      if (is_on_channel(ncuser, cuser))
        {
          visusr++;
          server_abuf_writef(ncuser->system, ncuser->node,
		STATE_CHANNEL,
		"WALL %s %lu/%d "
		"WE'RE IN LURK_MESSAGE()! Node [%02d] at %s ###\r\n%s",
		channel, cuser->system, cuser->node,
		cuser->node,
        	time_string, message);
        }
    }
  return (0);
}


