

/* Multitasking Kernel */


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#include "include.h"
#include "gtalk.h"

/* #define shit */

#define DEAD 0
#define ALIVE 1
#define FLAGGED 1
#define UNFLAGGED 0
#define MAXSEMAPHORES 256

#define abs_adr(x) ((((unsigned long int)FP_SEG(x)) << 4) + \
                     ((unsigned long int)FP_OFF(x)))

#undef DEBUG
#undef INT8_SWITCH_ONLY



/* PROTOTYPES */

unsigned int lock_dos_offset;

void interrupt dv_int8_task_switch(void);

void init_star_smart_port(int port_num,unsigned int baud, int databits,
       int stopbits, char parity);

int near emm_page_mapping[] = { 0,0,1,1,2,2,3,3 };



typedef int (far *task_type) (void);

typedef struct task_struct near *task_struct_ptr;
task_struct_ptr near task_fast[MAX_THREADS];
unsigned int last_pid=0;

void interrupt (*old_int8) (void);
void interrupt (*old_int0) (void);


int numTasksOpen;

void loadSystemVars(void);

typedef struct int_regs      /* This is a mirror of the top of stack of */
 {                           /* a task, so that the default register values */
   unsigned int bp;          /* can be set. */
   unsigned int di;
   unsigned int si;
   unsigned int ds;
   unsigned int es;
   unsigned int dx;
   unsigned int cx;
   unsigned int bx;
   unsigned int ax;
   unsigned int ip;
   unsigned int cs;
   unsigned int flags;
};
/* This keeps track of info on each task */
struct task_struct tasks[MAX_THREADS];

struct task_struct near *begin_task_struct;   /* these are pointers to */
struct task_struct near *end_task_struct;     /* make searching tasks[] */
                                              /* fast */

unsigned int oldss, oldsp;   /* keep track of caller's stack segment */
int tswitch;                 /* next task to switch to */
int old_tswitch;
char tasking = 0;
int curMaxTasks;
int switchTasks;
signed char semaphores[MAXSEMAPHORES];
int dontKeepNextTask;
unsigned int dans_counter;

#define MAX_TIMER 18

unsigned char timer_section=0;
unsigned long int num_task_switches=1;
unsigned long int max_task_switches=1;
unsigned long int system_load=0;

/*************
 * OUR STUFF *
 *************/


int does_pid_exist(unsigned int find_pid)
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
  kill_task(tswitch);
};


/* kill_task will end the thread of execution with number "id" */

void kill_task(int id)          /* this kills a task */
{
  int loop;
  int lockflag = !islocked(DOS_SEM);
#ifdef DEBUG
  char s[80];
#endif

  if (lockflag) lock_dos(890);
  disable();
  dealloc_abuf(id);
  free_semaphores(id);
  g_free_all_handles(id);
  unregister_bot(tasks[id].pid);


  /* why was this in there? */
//  if (id==tswitch)
//    unlock_dos();

  if (tasks[id].status)         /* kill only if we're alive */
   {
    tasks[id].status = DEAD;    /* mark status as DEAD */
    tasks[id].paused = 0;       /* mark task as NOT paused */
    tasks[id].who_paused_me=-1;
#ifdef DEBUG
    sprintf(s,"Free memory %p",tasks[id].stck);
    direct_screen(2,0,0x17,s);
    sprintf(s,"%p",*((char *)tasks[id].stck-4));
    direct_screen(2,40,0x17,s);
#endif
    g_free_from_who(tasks[id].stck,id);    /* free our stack memory */
    numTasksOpen--;             /* let tasker know there's one less task */
    if (id == tswitch) switchTasks = 1;  /* if the current task is dead, */
   };                                    /* make sure we switch */
   /* ALSO: make sure any tasks we paused get restarted */

   for (loop=0;loop<MAX_THREADS;loop++)
     if (who_paused(loop)==id)
             unpause(loop);


  enable();
  unlock_dos();
  next_task();                  /* just in case we're the dead task */
};

void switch_to_ems_context(int portnum)
{
  struct task_struct near *cur_task_struct = &tasks[portnum];

  if (cur_task_struct->is_ems)
  {
    _DX = cur_task_struct->ems_handle;
    _CX = cur_task_struct->mapped_pages;
    _SI = (unsigned int) emm_page_mapping;
    _AX = 0x5000;
    geninterrupt(0x67);
  }
}

/* next_task is called by a thread only to yield the rest of its */
/* timer tick */


void _saveregs flip_ems_page(void)
{
    _DX = task_fast[tswitch]->ems_handle;
    _CX = task_fast[tswitch]->mapped_pages;
    _SI = (unsigned int) (emm_page_mapping);
    _AX = 0x5000;
    geninterrupt(0x67);

}
void _saveregs flip_a_ems_page(struct task_struct *task_ptr)
{
    _DX = task_ptr->ems_handle;
    _CX = task_ptr->mapped_pages;
    _SI = (unsigned int) (emm_page_mapping);
    _AX = 0x5000;
    geninterrupt(0x67);
}

void relog_node_event(void);

void interrupt int0_task(void)
{
  struct task_struct near *cur_task_struct;
  int ttemp;

  direct_screen_override(0,52,0xF0,"DZERODZERODZERO");

  if (!tasking)
     reboot();
  if (tswitch >= sys_info.max_nodes)
     reboot();
  if ((line_status[tswitch].connect) &&
     (line_status[tswitch].online))
  {
    tasks[tswitch].paused = 1;
    add_task_to_scheduler((task_type) relog_node_event, (void *)tswitch,
      REL_SHOT_TASK, 0, 1, 1024, "RELOGNODE");
  } else
    tasks[tswitch].status = 0;

  switchTasks = 1;

  disable();

  old_tswitch = tswitch;

  task_fast[tswitch]->ss = _SS;
  task_fast[tswitch]->sp = _SP;

  ttemp = tswitch;
  cur_task_struct = (struct task_struct near *) task_fast[ttemp];
  dontKeepNextTask = 0;
  if (!numTasksOpen)            /* if no more tasks, leave multitasking */
  {
    switchTasks = 0;            /* don't switch task */
    tasking = 0;                /* turn multitasking off */
  }
  if (switchTasks)              /* if we are going to switch tasks */
  {
    do
    {
      ttemp++;                /* look for an alive task */
      cur_task_struct++;
      if (cur_task_struct == end_task_struct)
      {
        ttemp = 0;
        cur_task_struct = begin_task_struct;
      }
    } while (((!cur_task_struct->status) || (cur_task_struct->paused)));
    num_task_switches++;
    tswitch = ttemp;
  }

  if ((task_fast[tswitch]->is_ems) && (tswitch != old_tswitch))
    flip_ems_page();
  disable();

  _SS = task_fast[tswitch]->ss;
  _SP = task_fast[tswitch]->sp;

  enable();   /* on leaving this routine, the IRET and POP instructions */
};            /* will take all of the new registers off the stack, including */
              /* CS:IP and start the new task on the new stack */



/********************************
 *     THIS IS THE NEXT TASK    *
 ********************************/


void interrupt next_task(void)
{
  struct task_struct near *cur_task_struct;
  int ttemp;


#ifdef INT8_SWITCH_ONLY
  if (task_fast[tswitch]->status == 1) return;
#endif

  if ((tasking) && (!switchTasks))
    return;  /* if dos is locked, just return */

  disable();

  old_tswitch = tswitch;

  task_fast[tswitch]->ss = _SS;
  task_fast[tswitch]->sp = _SP;

  ttemp = tswitch;
  cur_task_struct = (struct task_struct near *) task_fast[ttemp];
  dontKeepNextTask = 0;
  if (!numTasksOpen)            /* if no more tasks, leave multitasking */
  {
    switchTasks = 0;            /* don't switch task */
    tasking = 0;                /* turn multitasking off */
  }
  if (switchTasks)              /* if we are going to switch tasks */
  {
    do
    {
      ttemp++;                /* look for an alive task */
      cur_task_struct++;
      if (cur_task_struct == end_task_struct)
      {
        ttemp = 0;
        cur_task_struct = begin_task_struct;
      }
    } while (((!cur_task_struct->status) || (cur_task_struct->paused)));
    num_task_switches++;
    tswitch = ttemp;
  }

  if (!tasking)
   {
     disable();                 /* set up stack that called our stack */
     _SS = oldss;
     _SP = oldsp;
     setvect(8, old_int8);      /* reset timer tick */
     setvect(0, old_int0);
     enable();
     return;
   }

  if ((task_fast[tswitch]->is_ems) && (tswitch != old_tswitch))
    flip_ems_page();
  disable();

  _SS = task_fast[tswitch]->ss;
  _SP = task_fast[tswitch]->sp;

  enable();   /* on leaving this routine, the IRET and POP instructions */
};            /* will take all of the new registers off the stack, including */
              /* CS:IP and start the new task on the new stack */


/* This intercepts timer interrupt 8 and performs a task switch */

void interrupt dv_int8_task_switch(void)
{

  (*old_int8)();    /* call old int8 function */
  dans_counter++;    /* increment counter used for 1/18 second timing */
  timer_section++;   /* increment counter for system_load */


  if ((!tasking) || (!numTasksOpen))
   {
     disable();
     /* switch to stack of old task */
     _SS = oldss;
     _SP = oldsp;
     setvect(8, old_int8);  /* reset timer tick */
     setvect(0, old_int0);
     enable();
     return;
   }

   if (timer_section==MAX_TIMER)
    {
      if (num_task_switches>max_task_switches)
          max_task_switches=num_task_switches;
      if (system_load)
      system_load= (unsigned long int) ((((unsigned long int)system_load*7l) +
                         (unsigned long int)(num_task_switches))>>3);
      else
        system_load= (num_task_switches);
      timer_section=0;
      num_task_switches=0;

    }

   return;
}


void interrupt int8_task_switch(void)
{
  struct task_struct near *cur_task_struct;
  int ttemp;

  (*old_int8)();    /* call old int8 function */
  dans_counter++;    /* increment counter used for 1/18 second timing */
  timer_section++;   /* increment counter for system_load */


  if (timer_section==MAX_TIMER)
  {
    if (num_task_switches>max_task_switches)
       max_task_switches=num_task_switches;

    if (system_load)
       system_load= (unsigned long int) ((((unsigned long int)system_load*7l) +
                          (num_task_switches))>>3);
      else
       system_load= (num_task_switches);

    timer_section=0;
    num_task_switches=0;
  }


 disable();
 old_tswitch = tswitch;

 if ((!tasking) || (!numTasksOpen))
 {
   disable();
   /* switch to stack of old task */
   _SS = oldss;
   _SP = oldsp;
   setvect(8, old_int8);  /* reset timer tick */
   setvect(0, old_int0);
   enable();
   return;
 }

 if (!dontKeepNextTask)
 {
   dontKeepNextTask = 1;
   enable();
   return;
 }
                /* if we kept the current task this tick, */
                /* make sure we change next tick */

  if (!switchTasks)
  {
     enable();
     return;
  }

  task_fast[tswitch]->ss = _SS;
  task_fast[tswitch]->sp = _SP;

  ttemp = tswitch;
  cur_task_struct = (struct task_struct near *) task_fast[ttemp];

  do
  {
    ttemp++;                /* look for a task that's alive */
    cur_task_struct++;
    if (cur_task_struct == end_task_struct)
    {
      ttemp = 0;
      cur_task_struct = begin_task_struct;
    }
  } while (((!cur_task_struct->status) || (cur_task_struct->paused)));

  tswitch = ttemp;

  if ((task_fast[tswitch]->is_ems) && (tswitch != old_tswitch))
     flip_ems_page();
  disable();

  _SS = task_fast[tswitch]->ss;
  _SP = task_fast[tswitch]->sp;

  // SEE NOTES IN VERSION.TXT BEFORE CHANGING THIS
  //  outp(0x20,0x20);
  enable();
}
    /* see next_task for details of routine IRET exit */

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
  struct int_regs *r;
  struct task_struct *cur_task_struct;
  task_type new_task;
  static time_t now;

  int testid;
  int id;
  char t[12];

#ifdef DEBUG
  static char s[80];
#endif

  disable();    /* disable so we don't accidenally jump to a task */
                /* with no stack, and some other task doesn't */
                /* trash our static variables */

  id = -1;

  now=time(NULL);

  if ((reqid != -1) && (!tasks[reqid].status)) id = reqid;
   else     /* force the task number if reqid != -1 */
    {
     for (testid=(MAX_THREADS-1);(testid>=0) && (id == -1);testid--)
      if (!(tasks[testid].status))  /* otherwise look for an dead thread */
       id = testid;
    }

  if (id==-1)
  {
    enable();
    return (-1);
  }

  sprintf(t,"STACK%02d",id);

  cur_task_struct = &tasks[id];
  new_task = task;


    /* ALLOCATE STACKS FROM MAIN ONLY */

   cur_task_struct->stck = g_malloc_with_owner
    (stck_size + sizeof(struct int_regs),t,id,0,0);


#ifdef DEBUG
  sprintf(s,"Allocated memory %p",tasks[id].stck);
  direct_screen(3,0,0x17,s);
  sprintf(s,"%p",*((char *)tasks[id].stck-4));
  direct_screen(3,40,0x17,s);
#endif



  if (!cur_task_struct->stck)
  {
    enable();
    return (-1);
  }   /* return -1 if we couldn't allocate a stack */

  r = (struct int_regs far *) ((long int) (cur_task_struct->stck)
    | ( stck_size - sizeof(struct int_regs)));
  /* Initialize task stack */
  cur_task_struct->taskchar = taskchar;
  cur_task_struct->sp = FP_OFF((struct int_regs *) r);
         /* set new stack location */
  cur_task_struct->ss = FP_SEG((struct int_regs *) r);
         /* to registers in array */

  /* set up task code segment and IP */
  r->cs = FP_SEG(new_task); /* set up CS:IP of new task to function start */
  r->ip = FP_OFF(new_task);

  /* set up DS and ES segments */
  r->ds = _DS;              /* set it for same DS and ES to use same data */
  r->es = _ES;

  /* enable interrupts - see text */
  r->flags = 0x200;         /* set up flags for interrupt enable */

  cur_task_struct->status = ALIVE;
  cur_task_struct->paused = 0;
  cur_task_struct->who_paused_me=-1;
  cur_task_struct->time_created=now;
  cur_task_struct->pid = calc_next_pid();
  strncpy(cur_task_struct->name,name,9);
  cur_task_struct->name[9]=0;
  numTasksOpen++;

  enable();
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
  begin_task_struct = (struct task_struct near *) &tasks;  /* initialize */
  end_task_struct = begin_task_struct + MAX_THREADS;  /* quick boundary ptrs */
  for (count=0;count<MAX_THREADS;count++)    /* mark all tasks as currently */
   {
     tasks[count].status = 0;                  /* dead */
     tasks[count].paused = 0;                  /* and NOT paused */
     tasks[count].who_paused_me=-1;
     tasks[count].is_ems = 0;
     task_fast[count] = (struct task_struct near *) &tasks[count];
                               /* set up task struct loc table */
   };
  for (count=0;count<MAXSEMAPHORES;count++)  /* mark all semaphores as not */
   semaphores[count]=-1;                     /* used */
  numTasksOpen=0;                            /* tell tasker no tasks are open */
};

/* Start up the multitasking kernel */

void interrupt multitask(void)
{
  int checksw = 0;

  dans_counter = 0;
  ctrlbrk(ctrl_brk_handler);        /* make sure ctrl-brk doesn't interrupt! */
  disable();                        /* don't accidentally switch */
  tswitch = -1;                     /* start at task 0 (really) */
  dontKeepNextTask = 1;             /* make sure we keep this task at least */
                                    /* a tick */
  while ((tswitch == -1) && (checksw < MAX_THREADS))
   {
    if (tasks[checksw].status && !tasks[checksw].paused)
               tswitch=checksw;  /* search for an open thread */
    checksw++;
   };
  if (tswitch == -1) return;        /* if no tasks yet, bomb */
  tasking = 1; /* we will start tasking */
  switchTasks = 1;

 /* switch_to_ems_context(tswitch);*/   /* switch into EMS context of task */

  if (task_fast[tswitch]->is_ems) flip_ems_page();

  /* Reset interrupt 8 */
  old_int8 = getvect(8);            /* set interrupt 8 for tasking */
  old_int0 = getvect(0);

  if (dv_loaded)
    setvect(8, dv_int8_task_switch);
  else
    setvect(8, int8_task_switch);

  setvect(0, int0_task);
  /* save original stack and pointer */
  oldss = _SS;                      /* save our original stack */
  oldsp = _SP;

  /* reroute stack to first task */
  _SS = tasks[tswitch].ss;          /* go to first task's stack */
  _SP = tasks[tswitch].sp;

  enable();

  /* see next_task for details of routine exit */
};

/* locks a semaphore with # sem */

void lock(int sem)
{
 disable();                 /* make sure someone else isn't locking */
 while (semaphores[sem] != -1)    /* if we don't have semaphores yet */
  {
    enable();               /* go to the next task to wait for semaphore */
    next_task();
    disable();
  };
 semaphores[sem] = tswitch; /* flag our semaphore! */
 enable();
};

void unlock(int sem)
{
 disable();                     /* unflag the semaphore */
 semaphores[sem] = -1;
 enable();
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
 char s[100];
 int loop;

 for (loop=0;loop<4;loop++)
   lock_dos_record[loop] = lock_dos_record[loop+1];

 lock_dos_record[4].lock_num = index;
 lock_dos_record[4].task_num = tswitch;
 lock_dos_record[4].was_unlocked = 0;


 direct_screen_override(0,0,0x17,"Locks: ");
 for (loop=0;loop<5;loop++)
 {
   sprintf(s,"% 3d:%02d:%d ",lock_dos_record[loop].lock_num,
            lock_dos_record[loop].task_num,lock_dos_record[loop].was_unlocked);
   direct_screen_override(0,6+(loop*9),0x17,s);
 }

 disable();                     /* see lock for details */
 semaphores[DOS_SEM] = tswitch;
 switchTasks = 0;
 enable();
};

/* unlock_dos will start other tasks running */

void unlock_dos(void)
{

 direct_screen_override(0,0,0x17,"UnLCK");

 lock_dos_record[4].was_unlocked = 1;

 disable();
 semaphores[DOS_SEM] = -1;
 switchTasks = 1;
 enable();
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
     }

  return;
};

void unpause(int task_num)
{
  tasks[task_num].paused=0;
  tasks[task_num].who_paused_me=-1;
  return;
};

/* this initializes the tasks for ginsu talk, essentially */



void main(int argc, char **argv)
{
  allocate_resources(argv);
  multitask();              /* start multitasking! */
  if (sys_toggles.should_reboot)
                    reboot();
  de_allocate_resources();

};

