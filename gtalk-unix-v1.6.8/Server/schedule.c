/* SCHEDULE.C */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#include "list.h"
#include "str.h"
#include "fork.h"
#include "common.h"
#include "states.h"
#include "schedule.h"

list schedule;

int compare_scheduler_events(schedule_task *s1, schedule_task *s2)
{
  return (strcmp(s1->task_name, s2->task_name));
}

int create_schedule_list(void)
{
  if (!new_list(&schedule, sizeof(schedule_task)))
    return (-1);
  if (!add_index(&schedule, compare_scheduler_events))
    {
      free_list(&schedule);
      return (-1);
    }
  return (0);
}

void calc_next_event(schedule_task *sch)
{
  time_t new_time;
  struct tm dep_time;
  struct tm last_event;
  struct tm *temp_time;
  unsigned long int task_time;
  unsigned long int ln_event;
  int adj_secs=0;

  switch (sch->int_type)
   {
     case ONE_SHOT_TASK: 
       sch->next_event = sch->task_time;
       break;
     case DAILY_TASK:
       temp_time = localtime(&sch->last_event);
       last_event = *temp_time;
       ln_event = ((unsigned long int)sch->last_event) 
                  + (SECS_IN_DAY);
       temp_time = localtime(&ln_event);
       if ((last_event.tm_isdst) && (!temp_time->tm_isdst))
         adj_secs = SECS_IN_HOUR;
       if ((!last_event.tm_isdst) && (temp_time->tm_isdst))
         adj_secs = -SECS_IN_HOUR;
       ln_event += adj_secs;
       temp_time = localtime(&ln_event);
       dep_time = *temp_time;
       task_time = sch->task_time;
       dep_time.tm_hour = (unsigned long int)(task_time / 3600);
       dep_time.tm_min = (unsigned long int) (((task_time) % 3600) / 60);
       dep_time.tm_sec = (unsigned long int) ((task_time) % 60);
       new_time = mktime(&dep_time);
       sch->next_event = new_time;
       break;
    case PERIODIC_TASK: 
       sch->next_event = (time_t)
         ((unsigned long int)(sch->last_event) + sch->task_time);
       break;
    case REL_SHOT_TASK: 
       sch->next_event = (time_t)
	 ((unsigned long int)(sch->last_event) + sch->task_time);
       sch->int_type = ONE_SHOT_TASK;
       break;
    case HOURLY_TASK:
       ln_event = (3600l - (sch->last_event % 3600l));
       sch->next_event = sch->last_event + ln_event;
       break;
    case REL_NOW_TASK: 
       time(&new_time);
       sch->last_event = new_time;
       sch->next_event = (time_t)
	 ((unsigned long int)(sch->last_event) + sch->task_time);
       sch->int_type = ONE_SHOT_TASK;
       break;
   }
}

int find_schedule_id(char *desc)
{
  schedule_task sch;

  strcpy_n(sch.task_name, desc, sizeof(sch.task_name)-1);
  return (search_list(&schedule, 0, &sch));
}

schedule_task *find_schedule_struct(char *desc)
{
  int ind;

  if ((ind=find_schedule_id(desc)) >= 0)
    return (element_of_index(schedule_task, &schedule, ind, 0));
  return (NULL);
}

int delete_task_from_scheduler(char *desc)
{
  int ind;

  if ((ind=find_schedule_id(desc)) >= 0)
    {
      delete_list(&schedule, real_index_no(&schedule, ind, 0));
      return (0);
    }
  return (1);
}

/*
void delete_sched_with_nodeno(node_id nodeno)
{
  int i;
  schedule_task *sch;

  for (i=0;i<elements(&schedule);)
    {
      sch = element_of(schedule_task, &schedule, i);
      if (sch->nodeno == nodeno)
	delete_list(&schedule, i);
      else
	i++;
    }
}
*/

int calc_next_event_sec(void)
{
  unsigned long int next_time = 0xFFFFFFFFl;
  int i;
  node_struct *n;
  schedule_task *sch;
  time_t ctime = time(NULL);

  for (i=0;i<c_nodes_used;i++)
    {
      n = c_nodes(i);
      if ((is_node_online(n)) || (n->status == NODE_LOGGING_IN))
	{
	  if ((n->timeout_status == TIMEOUT_WARNING) &&
	      (next_time > (n->timeout_time - TIMEOUT_WARNING_TIME)))
	    next_time = n->timeout_time - TIMEOUT_WARNING_TIME;
	  else
	    if ((n->timeout_status == TIMEOUT_KILL) &&
		(next_time > n->timeout_time))
	      next_time = n->timeout_time;
	}
    }
  for (i=0;i<elements(&schedule);i++)
    {
      sch = element_of(schedule_task, &schedule, i);

      if ((sch->active) && (next_time > sch->next_event))
	next_time = sch->next_event;
    }
  if ((next_time >= (60 + ctime)))
    return (60);           /* wait no more than 60 seconds */
  return (ctime > next_time ? 0 : next_time - ctime);
}

void see_if_event_occurs(void)
{
  int i;
  node_struct *n;
  schedule_task *sch;
  time_t ctime = time(NULL);

  for (i=0;i<c_nodes_used;i++)
    {
      n = c_nodes(i);
      if ((is_node_online(n)) || (n->status == NODE_LOGGING_IN))
	{
	  if ((n->timeout_status == TIMEOUT_WARNING) &&
	      (ctime >= (n->timeout_time - TIMEOUT_WARNING_TIME)))
	    {
	      server_abuf_writef(my_ip, i, STATE_MESSAGE, 
          	 "WARNING: Timeout in %d seconds",
		 TIMEOUT_WARNING_TIME);
	      n->timeout_status = TIMEOUT_KILL;
	    } else
	      if ((n->timeout_status == TIMEOUT_KILL) &&
		  (ctime >= n->timeout_time) && (n->pid >= 0))
		{
                  n->sigusr1_action = SIG1_ACTION_TIMEOUT;
		  kill(n->pid, SIGUSR1);
		  n->timeout_status = TIMEOUT_NONE;
		}
	}
    }
  for (i=0;i<elements(&schedule);)
    {
      sch = element_of(schedule_task, &schedule, i);

      if ((sch->active) && (ctime >= sch->next_event))
	{
	  if (sch->call_function)
	    (sch->call_function)(sch, sch->task_data);
	  if (sch->int_type == ONE_SHOT_TASK)
	    {
	      delete_list(&schedule,i);
	      continue;
	    }
	  sch->last_event = sch->next_event;
	  calc_next_event(sch);
	}
      i++;
    }
}

int add_task_to_scheduler(char *desc,
			     int int_type,
			     unsigned long int task_time,
			     int active,
			     schedule_func sfunc,
			     void *task_data)
{
  schedule_task sch;

  if (find_schedule_id(desc) >= 0)
    return (-1);
  strcpy_n(sch.task_name, desc, sizeof(sch.task_name)-1);
  sch.int_type = int_type;
  sch.task_time = task_time;
  sch.active = active;
  sch.call_function = sfunc;
  sch.task_data = task_data;
  if (!add_list(&schedule, &sch))
    return (-1);
  return (0);
}
  
int change_task_in_scheduler(char *desc,
			     int int_type,
			     unsigned long int task_time,
			     int active,
			     schedule_func sfunc,
			     void *task_data)
{
  schedule_task *sch;

  if (!(sch=find_schedule_struct(desc)))
    return (-1);
  if (int_type >= 0)
    {
      sch->int_type = int_type;
      sch->task_time = task_time;
    }
  if (active >= 0)
    sch->active = active;
  if (sfunc)
    sch->call_function = sfunc;
  if (task_data)
    sch->task_data = task_data;
  return (0);
}

