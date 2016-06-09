

/* Multitasking Kernel */


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#include "include.h"
#include "gtalk.h"
#define INCL_DOSEXCEPTIONS
#define INCL_DOSFILEMGR
#define INCL_NOPMAPI
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#include <os2.h>
#include <process.h>
#include "except.h"


#undef MAKE_TASK_DEBUG
#undef WAIT_SEM_CLOSE

#define DEAD 0
#define ALIVE 1
#define FLAGGED 1
#define UNFLAGGED 0
#define MAXSEMAPHORES 256

/*
#define abs_adr(x) ((((unsigned long int)FP_SEG(x)) << 4) + \
                     ((unsigned long int)FP_OFF(x)))

                     */

#undef DEBUG
#undef INT8_SWITCH_ONLY


/* PROTOTYPES */


#include "taskst.h"

extern task_struct_ptr task_fast[MAX_THREADS];

/* extern declarations */

extern struct task_struct tasks[MAX_THREADS];
extern struct task_struct *begin_task_struct;
extern struct task_struct *end_task_struct;

unsigned int lock_dos_offset;


void init_star_smart_port(int port_num,unsigned int baud, int databits,
       int stopbits, char parity);

int  emm_page_mapping[] = { 0,0,1,1,2,2,3,3 };




typedef struct task_struct  *task_struct_ptr;
task_struct_ptr  task_fast[MAX_THREADS];
unsigned int last_pid=0;
unsigned int dans_counter=0;


int numTasksOpen;

void loadSystemVars(void);

/* This keeps track of info on each task */
struct task_struct tasks[MAX_THREADS];

struct task_struct *begin_task_struct;   /* these are pointers to */
struct task_struct *end_task_struct;     /* make searching tasks[] */
                                              /* fast */

unsigned int oldss, oldsp;   /* keep track of caller's stack segment */
int tswitch;                 /* next task to switch to */
int old_tswitch;
char tasking = 0;
int curMaxTasks;
int switchTasks;
signed char semaphores[MAXSEMAPHORES];
int dontKeepNextTask;

#define MAX_TIMER 18

unsigned char timer_section=0;
unsigned long int num_task_switches=1;
unsigned long int max_task_switches=1;
unsigned long int system_load=0;
unsigned char tswitch_lookup_ready = 0;

/*************
 * OUR STUFF *
 *************/

 void init_tswitch_lookup(void)
 {
	int loop;


	for (loop=0;loop<MAX_THREADS;loop++)
       { tasks[loop].tid=0;
         tasks[loop].wait_sem=0;
         tasks[loop].has_child=0;
       }
    tswitch_lookup_ready = 1;

 }

void register_new_child(PID child_pid)
{
    tasks[tswitch].child_pid = child_pid;
    tasks[tswitch].has_child = 1;
}

void unregister_child()
{
   tasks[tswitch].has_child = 0;
}

int lookup_my_tswitch(void)
{
  PTIB tib_blk;
  PPIB pib_blk;
  int num;
  char s[80];
  int flag=1;

  if (sys_toggles.system_booting)
   return 0;

  DosGetInfoBlocks(&tib_blk,&pib_blk);

  num=0;
  while (flag)
  {

	  if ((tasks[num].status) && (tasks[num].tid==tib_blk->tib_ptib2->tib2_ultid))
	   flag=0;
	  else
	  {if (num>MAX_THREADS)
		  {num=-1;
		   flag=0;
		}
		else
		   num++;
	  }
  }

  if ((num) <0)
	{
      EXCEPTIONREPORTRECORD exception;

      fprintf(stderr,"GTERR: invalid tid lookup TID: %d",tib_blk->tib_ptib2->tib2_ultid);

      exception.ExceptionNum = GT_XCPT_INVALID_TID_LOOKUP;
      exception.fHandlerFlags = 0;
      exception.NestedExceptionReportRecord = NULL;
      exception.ExceptionAddress = NULL;
      exception.cParameters = 0;
      DosRaiseException(&exception);
	}

	return(num);

}

int task_of_pid(unsigned int find_pid)
{
int loop=0;

while (loop<MAX_THREADS)
 {
   if (tasks[loop].pid == find_pid)
	 return (loop);
   else
    loop++;
 }
 return (-1);
}

int does_pid_exist(unsigned int pid)
{
 if (task_of_pid(pid)==-1)
   return 0;
 else
   return 1;
}

unsigned int calc_next_pid(void)
{
  int loop=0;
  unsigned int next_pid = last_pid++;
  if (last_pid >= 0x8000) last_pid = 0;

  while (loop<MAX_THREADS)
   if (tasks[loop].pid == next_pid)
      {
         next_pid++;
         if (next_pid >= 0x8000) next_pid = 0;
         loop=0;
      }
   else
	 loop++;

  return (last_pid = next_pid);


}

/***********************************************/

/* end_task kills the caller's thread of execution */

void end_task(void)             /* this ends the task that calls this */
{
   int loop;
   int flag=0;
   int id = tswitch;
#ifdef DEBUG
  char s[80];
#endif
 if (!DosExitCritSec())
  {
	while(!DosExitCritSec());
	printf("DosCritSec on end_task\n");
  }

  dealloc_abuf(id);
  watcher_dealloc(id);
  free_semaphores(id);
  g_free_all_handles(id);
  check_com(id);


  /* unregister_bot(tasks[id].pid); */

  DosEnterCritSec();


  if (tasks[id].status)         /* kill only if we're alive */
   {
	tasks[id].status = DEAD;    /* mark status as DEAD */
	tasks[id].paused = 0;       /* mark task as NOT paused */
	tasks[id].who_paused_me=-1;
	tasks[id].tid = 0;
    if (tasks[id].has_child)
      { DosKillProcess(0,tasks[id].child_pid);
        tasks[id].has_child=0;
      }

#ifdef WAIT_SEM_CLOSE
    if (tasks[id].wait_sem)
     if (DosCloseEventSem(tasks[id].wait_sem))
        printf("Event Semaphore Close Error\n");

	tasks[id].wait_sem = 0;
#endif

   };

   /* ALSO: make sure any tasks we paused get restarted */

   for (loop=0;loop<MAX_THREADS;loop++)
	 if (who_paused(loop)==id)
			 unpause(loop);
  DosExitCritSec();

 DosExit(EXIT_THREAD,0);
};


/* kill_task will end the thread of execution with number "id" */

void kill_task(int id)          /* this kills a task */
{
  int loop;
   int flag=0;
   int rc;
#ifdef DEBUG
  char s[80];
#endif
  if (id==tswitch)
	end_task();


  dealloc_abuf(id);
  watcher_dealloc(id);
  free_semaphores(id);
  g_free_all_handles(id);
  check_com(id);


  /* unregister_bot(tasks[id].pid); */

  DosEnterCritSec();


  if (tasks[id].status)         /* kill only if we're alive */
   {
	 rc = DosExitCritSec();
	 DosKillThread(tasks[id].tid);
	 // DosWaitThread(&(tasks[id].tid),DCWW_WAIT);
	 DosEnterCritSec();

	tasks[id].status = DEAD;    /* mark status as DEAD */
    unpause(id);                 /* mark task as NOT paused */
	tasks[id].who_paused_me=-1;
	tasks[id].tid = 0;
    if (tasks[id].has_child)
      { DosKillProcess(0,tasks[id].child_pid);
        tasks[id].has_child=0;
      }

#ifdef WAIT_SEM_CLOSE
    if (tasks[id].wait_sem)
      if (DosCloseEventSem(tasks[id].wait_sem))
         printf("Event Semaphore Close Error\n");

	tasks[id].wait_sem = 0;
#endif

   };

   /* ALSO: make sure any tasks we paused get restarted */

   for (loop=0;loop<MAX_THREADS;loop++)
	 if (who_paused(loop)==id)
			 unpause(loop);
  DosExitCritSec();
};

void relog_node_event(void);

/********************************
 *     THIS IS THE NEXT TASK    *
 ********************************/


void next_task(void)
{
 dans_counter++;
 if (!DosExitCritSec())
	{
	 while (!DosExitCritSec());

	// printf("Error, CritSect Lockup tswitch: %d\n",tswitch);
	end_task();
	}

 DosSleep(1);

 while (tasks[tswitch].paused)
   DosSleep(3);

 return;
};


/* Define a new task */
/* task_type task  is the pointer to the function to start */
/* wth. it must have no parameters (be declared void func(void)) */
/* stck_size is the bytes of stack frame to create */
/* reqid is the id of the task to create */
/* id == -1, it creates a task number for you and returns it */
/* id != -1, you specify the task number. (it must NOT be used!) */
/* if this function returns -1, thread creation was NOT successful */



int make_task(task_type task, unsigned int stck_size, int reqid,
				char taskchar,char *name)
{

  struct task_struct *cur_task_struct;
  int id,testid;
  time_t now;
  TID newThreadTid;

  id = -1;

  now=time(NULL);
  DosEnterCritSec();
  if ((reqid != -1) && (!tasks[reqid].status)) id = reqid;
   else     /* force the task number if reqid != -1 */
	{
	 for (testid=(MAX_THREADS-1);(testid>=0) && (id == -1);testid--)
	  if (!(tasks[testid].status))  /* otherwise look for an dead thread */
	   id = testid;
	}

  if (id==-1)
  {
	DosExitCritSec();
	return (-1);
  }

  cur_task_struct = &tasks[id];

  /*
  * your not supposed to use this and library functions, So I'll
  * obey the rules
  *
  *   if (DosCreateThread(&newThreadTid,(PFNTHREAD)task,0,0,(4096*4)))
  */

  if ((newThreadTid = _beginthread((task_type)task,(4096*4),NULL))==-1)
	{ DosExitCritSec();
	  return -1;
	}

  cur_task_struct->status = ALIVE;
  cur_task_struct->paused = 0;
  cur_task_struct->who_paused_me=-1;
  cur_task_struct->time_created=now;
  cur_task_struct->pid = calc_next_pid();
  cur_task_struct->num_int_switches=0;
  cur_task_struct->tid = newThreadTid;

  strcpy(cur_task_struct->name,name);

	  /* strncpy(cur_task_struct->name,name,9); */
  cur_task_struct->name[9]=0;
  cur_task_struct->taskchar = taskchar;

  if (cur_task_struct->wait_sem == 0)
  {
        /* create an event semaphore, and start it cleared */
      if (DosCreateEventSem(NULL,&(cur_task_struct->wait_sem),0,0))
       {
        printf("Event Semaphore Creation Error\n");
        cur_task_struct->wait_sem=0;
       }
  }


#ifdef MAKE_TASK_DEBUG
  printf("Made new TID: %d gt_id:%d\n",newThreadTid,id);
#endif
  DosExitCritSec();
  return(id);
};



/* This routine is called by Borland C to determine whether a Ctrl-C */
/* results in a program exit. a return of 1 means don't exit */

int ctrl_brk_handler(void)
{
  return 1;
};

/* Init the multitasking adder */

void initMultitask(void)
{
  int count;
  begin_task_struct = (struct task_struct  *) &tasks;  /* initialize */
  end_task_struct = begin_task_struct + MAX_THREADS;  /* quick boundary ptrs */
  for (count=0;count<MAX_THREADS;count++)    /* mark all tasks as currently */
   {
     tasks[count].status = 0;                  /* dead */
     tasks[count].paused = 0;                  /* and NOT paused */
     tasks[count].who_paused_me=-1;
     task_fast[count] = (struct task_struct  *) &tasks[count];
                               /* set up task struct loc table */
   };
  for (count=0;count<MAXSEMAPHORES;count++)  /* mark all semaphores as not */
   semaphores[count]=-1;                     /* used */
  numTasksOpen=0;                            /* tell tasker no tasks are open */
  tasking=1;
};


/* locks a semaphore with # sem */

void lock(int sem)
{
 while (semaphores[sem] != -1)    /* if we don't have semaphores yet */
  {
    next_task();
  };
 semaphores[sem] = tswitch; /* flag our semaphore! */
};

void unlock(int sem)
{
 semaphores[sem] = -1;
};

void free_semaphores(int task_num)
{
 int count;
 for (count=0;count<MAXSEMAPHORES;count++)
  if (semaphores[count] == task_num) semaphores[count] = -1;
};

int islocked(int sem)
{
 return (semaphores[sem] != - 1);  /* see if this semaphore is locked */
};

/* this is a special semaphore to lock our task temporarily */
/* after calling, your task will be the only one running */
/* until unlock_dos is called. this is because dos routines are */
/* not reentrant, so we make sure we don't have a dos call */
/* interrupted */



struct data_keep
{
  int lock_num;
  int task_num;
  int was_unlocked;
} lock_dos_record[5];

void lock_dos(int index)
{
};

/* unlock_dos will start other tasks running */

void unlock_dos(void)
{
};

/* this function returns whether a certain task is dead */
/* task_num is the task to find out */
/* 1 = dead, 0 = alive ! */

int iskilled(int task_num)
{
  return(!tasks[task_num].status);
};

int ispaused(int task_num)
{
  return(tasks[task_num].paused);
};

int who_paused(int task_num)
{
  return(tasks[task_num].who_paused_me);
};

void pause(int task_num)
{
  if (task_num!=tswitch)
     {
        wait_for_death(task_num);
        tasks[task_num].paused=1;
        tasks[task_num].who_paused_me=tswitch;
        DosSuspendThread(tasks[task_num].tid);
     }

  return;
};

void unpause(int task_num)
{
  tasks[task_num].paused=0;
  tasks[task_num].who_paused_me=-1;
  if (tasks[task_num].tid)
    DosResumeThread(tasks[task_num].tid);
  return;
};

/* this initializes the tasks for ginsu talk, essentially */



int main(int argc, char **argv)
{
   EXCEPTIONREGISTRATIONRECORD er = { NULL, (_ERR *)&ExceptionHandler }; /* To register the handler */
   PTIB ptib; /* thread info block */
   PPIB ppib; /* process info block */
   DosGetInfoBlocks(&ptib,&ppib);

   sys_toggles.system_booting=1;
//   printf("Installing Exception Handler.\n");
   DosSetExceptionHandler (&er);                      /* Add our routine to the chain */

  allocate_resources(argv);

  /*
  if (sys_toggles.should_reboot)
					reboot();
					*/
  DosEnterCritSec();
  de_allocate_resources();
  DosExitCritSec();
  DosKillProcess(0,ppib->pib_ulpid);

};


void show_task_info(char *str,char *name,int portnum)
{
  int loop=0;
  char s[220];
  time_t now;


  lock_dos(321);
  now=time(NULL);
  unlock_dos();
  repeat_chr('-',68,1);
  print_str_cr("PID    Num   OS/2 TID  TChar    PAUSED    TIME     NAME");
  repeat_chr('-',68,1);
  for (loop=0;loop<MAX_THREADS;loop++)
   if (tasks[loop].status)
    {
	  sprintf(s,"%06u [%02d]   % 4u      %02d        %u  ",tasks[loop].pid,
			   loop,tasks[loop].tid,tasks[loop].taskchar,tasks[loop].paused);
      print_string(s);


      sprintf(s," % 4u:",((unsigned int)((now-tasks[loop].time_created)/3600)));
      print_string(s);

      sprintf(s,"%02u:",(((unsigned int)((now-tasks[loop].time_created)/60))%60));
      print_string(s);

      sprintf(s,"%02u   ",((unsigned int)(now-tasks[loop].time_created)%60));
      print_string(s);

      print_str_cr(tasks[loop].name);
    }
  print_cr();
}

int get_taskchar(int tasknum)
{
 return (tasks[tasknum].taskchar);
}

void wait_for_death(int portnum)
 {
   if (!tasks[portnum].status)  /* if the task is not alive... */
	return;

   while (!line_status[portnum].timeout)
	next_task();
 };

int get_task_pid(int tasknum)
{
 return (tasks[tasknum].pid);
}

 int get_my_pid(void)
 {
  return (tasks[tswitch].pid);
 }

 int get_task_status(int tasknum)
 {
  return (tasks[tasknum].status);
 }


 void task_wake(int tasknum)
 {
   if (tasks[tasknum].wait_sem)
	  DosPostEventSem(tasks[tasknum].wait_sem);

 }

 void task_sleep(void)
 {
	ULONG count;
    if (!tasks[tswitch].wait_sem)
      return;

	DosWaitEventSem(tasks[tswitch].wait_sem, 1000l);
	DosResetEventSem(tasks[tswitch].wait_sem,&count);
 }

 void task_sleep_timeout(long int timeout)
 {
	ULONG count;
    if (!tasks[tswitch].wait_sem)
       return;

	DosWaitEventSem(tasks[tswitch].wait_sem, timeout);
	DosResetEventSem(tasks[tswitch].wait_sem,&count);
 }
