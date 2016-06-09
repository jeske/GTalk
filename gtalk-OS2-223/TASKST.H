/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


#ifndef GT_TASKST_H
#define GT_TASKST_H

struct task_struct
{
   unsigned int status;
   unsigned int paused;
   int who_paused_me;
   unsigned char taskchar;
   unsigned int for_who;
   unsigned char num_int_switches;
   time_t time_created;
   char name[10];
   unsigned int pid;
   unsigned long int tid;
   HEV	wait_sem;
   char has_child;
   PID  child_pid;
};
typedef struct task_struct *task_struct_ptr;


#endif
