
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - squelch.h
 *
 */


#ifndef _GTALK_SQUELCH_H
#define _GTALK_SQUELCH_H

#include "types.h"
#include "comparse.h"
#include "list.h"

typedef struct _squelch_node
{
  g_system_t system_no;
  node_id node;
} squelch_node;

int cmd_squelch_node(com_struct *com, char *line);
int delete_squelch_node(g_system_t system, node_id node);
int add_squelch_node(g_system_t system, node_id node);
squelch_node *find_squelched_node(g_system_t system, node_id node);
int find_num_squelched_node(g_system_t system, node_id node);
int create_squelched_node_list(void);

extern list squelched_nodes;

#endif _GTALK_SQUELCH_H









