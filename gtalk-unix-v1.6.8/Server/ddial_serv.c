/************************************

    D-dial processor server code

 ************************************/

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

#include "ddial_serv.h"

int ddial_distribute_message(abuffer *abuf, char *message)
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
  message = skip_blanks(message);

  for (i=0;i<elements(&(chan->channel_users));i++)
    {
      ncuser = element_of(channel_user,&(chan->channel_users),i);

      if (is_on_channel(cuser, ncuser)) 
        {
	 server_abuf_writef(ncuser->system, ncuser->node,
           STATE_DDIAL,
	  "%s %lu/%d %s",
	   channel, cuser->system, cuser->node, message);
         ncuser->lineout_counter = 0;
        }
    }
  return (0);
}


