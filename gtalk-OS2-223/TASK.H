
/* TASK.H */

#ifndef GT_TASK_H
#define GT_TASK_H


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


typedef void (*task_type) (void *);
/* #define shit */

#define DEAD 0
#define ALIVE 1
#define FLAGGED 1
#define UNFLAGGED 0
#define MAXSEMAPHORES 256

#define TASK_STACK_SIZE 8192

#define DOS_SEM 0
#define HANDLE_SEM 1
#define CHANNEL_SEM 2
#define SCHEDULE_SEM 3
#define INVITE_SEM 4

#define is_alive(task_id) tasks[(task_id)].status
#define MY_PID()        (get_my_pid())
#define pid_of(task_id) (get_task_pid(task_id))
#define our_task_id() (tswitch)

#define tswitch (lookup_my_tswitch())

/* I guess this was a bad idea
 *
 * #define	 dans_counter	(time(NULL))
 */


int lookup_my_tswitch(void);


 /*************
  ** task.c  **
  *************/


  void task_sleep_timeout(long int timeout);
 void task_wake(int tasknum);
  void task_sleep(void);
int get_taskchar(int tasknum);
int get_task_status(int tasknum);

extern char tasking;
extern switchTasks;
extern int numTasksOpen;
extern unsigned int dans_counter;


extern unsigned char timer_section;
extern unsigned long int num_task_switches;
extern unsigned long int max_task_switches;
extern unsigned long int system_load;


void multitask(void);
void int0_task(void);
void int8_task_switch(void);
int make_task(task_type task, unsigned stck, int reqid, char taskchar,char *name);
void initMultitask(void);
void lock(int sem);
void unlock(int sem);
int islocked(int sem);
void end_task(void);
void kill_task(int id);
void next_task(void);
void lock_dos(int index);
void unlock_dos(void);
int ctrl_brk_handler(void);
int iskilled(int task_num);
void free_semaphores(int task_num);
void midnight_task(void);

int ispaused(int task_num);
int who_paused(int task_num);
void pause(int task_num);

void unpause(int task_num);
int does_pid_exist(unsigned int find_pid);
 int get_my_pid(void);


void unregister_child();
void register_new_child(unsigned long child_pid);

#endif /* GT_TASK_H */
