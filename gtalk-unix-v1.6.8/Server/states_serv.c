/***************************************

          Server state list

 ***************************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "gt.h"
#include "str.h"
#include "list.h"
#include "states.h"

#include "srv_channel.h"
#include "gt.h"
#include "ddial_serv.h"
#include "srv_term.h"

list parent_state_list;

state_machine default_parent_state_list[] =
{
  { STATE_CHANNEL, channel_process },
  { STATE_DDIAL, ddial_distribute_message }, 
  { STATE_TERM, server_term_process },
  { -1, NULL }
};





