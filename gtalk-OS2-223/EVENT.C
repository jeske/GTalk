
#include "include.h"
#include "gtalk.h"
#include "event.h"

/* EVENT.C */

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#define INCL_DOSEXCEPTIONS
#define INCL_DOSFILEMGR
#define INCL_NOPMAPI
#define INCL_DOS
#define INCL_DOSPROCESS
#include <os2.h>
#include "except.h"

#include "event.h"
/* #include "module.h" */


void clear_all_old_pids_event(void)
{
   EXCEPTIONREGISTRATIONRECORD er = { NULL, (_ERR *)&ExceptionHandler }; /* To register the handler */

   DosSetExceptionHandler (&er);                      /* Add our routine to the chain */
   /*

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
  */
  end_task();
}


void console_alarm_event(void)
{
   EXCEPTIONREGISTRATIONRECORD er = { NULL, (_ERR *)&ExceptionHandler }; /* To register the handler */

   DosSetExceptionHandler (&er);                      /* Add our routine to the chain */

    console_alarm();
    end_task();
}
void link_node_event(void)
{
   int node=(int)schedule_data[tswitch];
   char s[80];
   EXCEPTIONREGISTRATIONRECORD er = { NULL, (_ERR *)&ExceptionHandler }; /* To register the handler */

   DosSetExceptionHandler (&er);                      /* Add our routine to the chain */

   if ((node<0) || (node>sys_info.max_nodes))
    end_task();

   user_options[node].warning_prefix='|';
   remote(node);

   sprintf(s,"--> Node [%d] Linked",node);
   aput_into_buffer(server,s,0,5,0,5,0);
   end_task();
}

void g_link_node_event(void)
{
   int node=(int)schedule_data[tswitch];
   int was_online=1;
   char s[80];
   EXCEPTIONREGISTRATIONRECORD er = { NULL, (_ERR *)&ExceptionHandler }; /* To register the handler */

   DosSetExceptionHandler (&er);                      /* Add our routine to the chain */

   if (!was_online)
     {
      end_task();
     }

   user_options[node].warning_prefix='|';
   g_remote(node);

   sprintf(s,"--> Node [%d] G-Linked",node);
   aput_into_buffer(server,s,0,5,0,5,0);
   end_task();
}

void kill_node_event(void)
{
   void *value;
   char s[80];
   int node;
   int who_killed;
   EXCEPTIONREGISTRATIONRECORD er = { NULL, (_ERR *)&ExceptionHandler }; /* To register the handler */

   DosSetExceptionHandler (&er);                      /* Add our routine to the chain */

   value=(void *)schedule_data[tswitch];

   node=(int)((unsigned long int)value & 0xFF);
   who_killed=(int)((unsigned long int)value>>16);

   if ((node<0) || (node>sys_info.max_nodes))
     end_task();

   if (!line_status[node].online)
     {sprintf(s,"--> Node [%02d] Offline - Kill Aborted",node);
      aput_into_buffer(server,s,0,5,0,1,0);
      end_task();
     }

   /* INCREMENT KILLED STATISTICS */
   user_lines[node].user_info.killedstats.kills_day++;
   user_lines[node].user_info.killedstats.kills_month++;
   user_lines[node].user_info.killedstats.kills_total++;

   unlog_user(node);

   pause(node);
   print_cr_to(node);
   print_str_cr_to("--> You have been KILLED",node);
   print_cr_to(node);
   print_file_to_cntrl("TEXT\\KILLED.TXT",node,1,0,0,0);

   wait_for_xmit(node,30);
   log_kill(node,who_killed);
   sprintf(s,"--> Node [%d] killed by node [%d]",node,who_killed);
   aput_into_buffer(server,s,0,5,0,2,0);

   log_off(node,0);
  /* aput_into_buffer(server,"--> Kill Mark",0,5,0,2,0); */
   kill_task(node);

   end_task();
}

void relog_node_event(void)
{
   int node=(int)schedule_data[tswitch];
   int was_online=line_status[node].connect;
   char s[80];
   EXCEPTIONREGISTRATIONRECORD er = { NULL, (_ERR *)&ExceptionHandler }; /* To register the handler */

   DosSetExceptionHandler (&er);                      /* Add our routine to the chain */

   if (!was_online)
     {
      end_task();
     }
   user_options[node].warning_prefix='+';
   re_log(node);

   sprintf(s,"--> Node [%d] Relogged",node);
   aput_into_buffer(server,s,0,5,0,4,0);
   end_task();
}

