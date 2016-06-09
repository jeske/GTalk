
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - usercommon.h
 *
 * This contains the code for reading and writing to the userfile as 
 * well as general use routines for handling security checks and sets.
 *
 */

#ifndef _GT_USERCOMMON_H
#define _GT_USERCOMMON_H

#include "types.h"
#include "userst.h"

struct flag_map_struct 
{
  char *flagname;
  int flagnum;
};

void setbit(char *set, int bit, int on);
int testbit(char *set, int bit);
void clearset(char *set, int bits);
int findFlagNumber(char *flag_name);
int testFlag(node_struct *node, char *flag_name);

extern struct flag_map_struct flags[];

#endif   /* _GT_USERCOMMON_H */
