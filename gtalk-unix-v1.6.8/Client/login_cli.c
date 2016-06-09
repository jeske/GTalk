
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - login_cli.h
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "list.h"
#include "str.h"
#include "login_cli.h"
#include "channelcommon.h"
#include "squelch.h"

token_entry_type client_lg_tokens[] =
{
  { "LOGOUT", logout_message, T_LI_LOGOUT }
};

token_list client_login_tok = { 1, client_lg_tokens };

int logout_message(abuffer *abuf, char *message)
{
  g_system_t system;
  node_id node;

  if (read_system_node(&message, &system, &node) < 0)
    return (-1);
  delete_squelch_node(system, node);
  return (0);
}

int client_login_process(abuffer *abuf, char *message)
{
  login_func ten;
  
  if (ten=get_token(&message, &client_login_tok, NULL)) 
    (ten)(abuf, message);
}
