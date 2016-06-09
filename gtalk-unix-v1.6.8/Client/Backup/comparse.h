
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - comparse.h
 *
 */
 


#ifndef _GTALK_COMPARSE_H
#define _GTALK_COMPARSE_H

#include "types.h"
#include "list.h"

#define MAX_COMMAND_LEN 15

/* command options */

#define COM_NONE 0
#define COM_NOCHROOT 0x01


/* this constitutes a command list entry */

typedef struct _com_struct
{
  char command[MAX_COMMAND_LEN+1];
  const char *description;
  const char *location;
  const char *flag_name;
  int (* cfunc)(struct _com_struct *com, char *line);
  char *run_command;
  unsigned long int options;
} com_struct;

typedef int (* command_func)(struct _com_struct *com, char *line);
com_struct *find_command(char **src_d);

extern list commands;

#endif /* _GTALK_COMPARSE_H */




















