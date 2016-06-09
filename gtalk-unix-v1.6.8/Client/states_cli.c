/***********************************

      Client States List

 ***********************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "str.h"
#include "list.h"
#include "states.h"

#include "channel.h"
#include "ddial.h"
#include "channelcli.h"
#include "login_cli.h"
#include "term_cli.h"

#include "states_cli.h"

list child_state_list;

state_machine default_child_state_list[] = 
{
  { STATE_CHANNEL, client_channel_process },
  { STATE_MESSAGE, client_message },
  { STATE_PRIVATE, private_message },
  { STATE_DDIAL, client_ddial_process }, 
  { STATE_LOGIN, client_login_process },
  { STATE_TERM, client_term_process },
  { -1, NULL }
};

