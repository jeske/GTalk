
#include "include.h"
#include "gtalk.h"
#include "event.h"

/* EVENT.C */

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


#include "event.h"
#include "module.h"


void clear_all_old_pids_event(void)
{
  int search, pident;
  struct shared_glm_entry *entry;
  int found;
  int taskno;

  lock_dos(4000);
  search = num_shared;
  while (search>0)
  {
    search--;
    entry = glm_entries[search];
    pident = entry->number_of_tasks;
    while (pident>0)
    {
      pident--;
      found = 0;
      taskno = MAX_THREADS;
      while ((!found) && (taskno>0))
      {
        taskno--;
        if (is_alive(taskno))
         if (entry->pid_list[pident] == pid_of(taskno))
         {
           found = 1;
           break;
         }
      }
      if (!found)
      {
        entry->number_of_tasks--;
        for (taskno=pident;taskno<entry->number_of_tasks;taskno++)
          entry->pid_list[taskno] = entry->pid_list[taskno+1];
      }
    }
    if (!entry->number_of_tasks)
    {
      g_free(entry);
      num_shared--;
      for (taskno=search;taskno<num_shared;taskno++)
        glm_entries[taskno] = glm_entries[taskno+1];
    }
  }
  unlock_dos();
  end_task();
}
