
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - squelch.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "abuf.h"
#include "common.h"
#include "shared.h"
#include "str.h"
#include "list.h"
#include "squelch.h"
#include "command.h"

list squelched_nodes;

int compare_squelch_node(squelch_node *s1, squelch_node *s2)
{
  if (s1->system_no != s2->system_no)
    return (s1->system_no - s2->system_no);
  return (s1->node - s2->node);
}

int create_squelched_node_list(void)
{
  if (!new_list(&squelched_nodes, sizeof(squelch_node)))
    return (-1);
  if (!add_index(&squelched_nodes, compare_squelch_node))
    {
      free_list(&squelched_nodes);
      return (-1);
    }
  return (0);
}

int find_num_squelched_node(g_system_t system, node_id node)
{
  squelch_node s;

  s.system_no = system;
  s.node = node;
  return (search_list(&squelched_nodes, 0, &s));
}

squelch_node *find_squelched_node(g_system_t system, node_id node)
{
  int ind = find_num_squelched_node(system, node);
  
  if (ind >= 0)
    return(element_of_index(squelch_node, &squelched_nodes, ind, 0));
  return (NULL);
}

int add_squelch_node(g_system_t system, node_id node)
{
  squelch_node s;

  if (find_num_squelched_node(system, node) >= 0)
    return (-1);
  s.system_no = system;
  s.node = node;
  if (!add_list(&squelched_nodes, &s))
    return (-1);
  return (0);
}

int delete_squelch_node(g_system_t system, node_id node)
{
  int ind;

  if ((ind=find_num_squelched_node(system, node)) < 0)
    return (-1);
  ind = real_index_no(&squelched_nodes, ind, 0);
  if (!delete_list(&squelched_nodes, ind))
    return (-1);
  return (0);
}

int cmd_squelch_node(com_struct *com,char *string)
{
  g_system_t system;
  node_id node;
  int identified=1;
  int old_state;
  char *result = NULL;
  struct _user_perm *temp_data;

  old_state = ansi_on(1);

  if (get_system_no_and_node(&string, &system, &node) < 0)
    return (-1);
  string = skip_blanks(string);

  if (system == 0)
    system = my_ip;   /* this is wrong for links!!!! */
  
  if (add_squelch_node(system, node) < 0)
    if (delete_squelch_node(system, node) < 0)
      printf_ansi("--> Squelch Error\r\n");
    else
      result = "Unsquelched";
  else
    result = "Squelched";

  if (result)
    {
      if (identified)
	{
	  temp_data = &(c_nodes(node)->userdata);
	  printf_ansi("--> %s #%02d:%c%s|*r1%c\n", result, node,
		      temp_data->online_info.class_info.staple[0],
		      temp_data->user_info.handle,
		      temp_data->online_info.class_info.staple[1]);
	}
      else
	printf_ansi("--> %s #%02d\n", result, node);
    }
  ansi_on(old_state);
  return 0;
}

