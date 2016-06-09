
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - login_cli.h
 *
 */


#ifndef _GTALK_LOGIN_CLI_H
#define _GTALK_LOGIN_CLI_H

#include "types.h"
#include "abuf.h"

typedef int (* login_func)(abuffer *buf, char *message);

int client_login_process(abuffer *abuf, char *message);
int logout_message(abuffer *abuf, char *message);

#define T_LI_LOGOUT 1

#endif _GTALK_SQUELCH_H









