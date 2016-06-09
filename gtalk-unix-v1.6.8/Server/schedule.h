/* SCHEDULE.H */

#ifndef _GT_SCHEDULE_H
#define _GT_SCHEDULE_H

#include "list.h"
#include "types.h"

/* Gtalk */
/* Copyright (C) 1993, 1995, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#define SECS_IN_DAY    86400l
#define SECS_IN_HOUR   3600l
#define SCHEDULE_DESC_LENGTH 10

#define ONE_SHOT_TASK  0
#define DAILY_TASK     1
#define PERIODIC_TASK  2
#define REL_SHOT_TASK  3
#define HOURLY_TASK    4
#define REL_NOW_TASK   5

typedef struct _schedule_task
{
  int               (*call_function)(struct _schedule_task *, void *);
  void               *task_data;     /* pointer to task function */
  short int           int_type;      /* schedule type */
  short int           active;       
  unsigned long int   task_time;     /* task time increment */
  time_t              last_event;    /* time of last event */
  time_t              next_event;    /* next event time */
  char                task_name[SCHEDULE_DESC_LENGTH];
                                     /* task name */
} schedule_task;

typedef int (* schedule_func)(schedule_task *, void *);

extern list schedule;

int create_schedule_list(void);
void see_if_event_occurs(void);
int calc_next_event_sec(void);
int add_task_to_scheduler(char *desc,
			     int int_type,
			     unsigned long int task_time,
			     int active,
			     schedule_func sfunc,
			     void *task_data);
int change_task_in_scheduler(char *desc,
			     int int_type,
			     unsigned long int task_time,
			     int active,
			     schedule_func sfunc,
			     void *task_data);

#endif /* _GT_SCHEDULE_H */





