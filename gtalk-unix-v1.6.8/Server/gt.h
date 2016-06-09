

/**********************************

              GT.H
          Main structures

***********************************/

#ifndef _GTALK_GT_H
#define _GTALK_GT_H

#include "log.h"
#include "types.h"
#include "common.h"
#include "channel.h"

#ifdef LINUX

typedef struct _gtalk_socketpair
{
  int mypipe;
  int otherpipe;
} gtalk_socketpair;

#endif   /* LINUX */

typedef struct _gtalk_process
{
  g_int32 pid;
  struct _gtalk_socketpair pipes;
} gtalk_process;

int is_pid_alive(int pid);
void new_node_and_device(struct _node_struct *n, node_id dev_no, 
			 int node, int new_pid, int fd_pipe);
void kill_with_pid(int pid, int exit_stat);

extern int check_nodes;
extern int child_dead;


#define GTALK_ACCEPT_PORT 4000       /* Default accept port */
#define EXTRN_GTALK_ACCEPT_PORT 4001 /* Default ext. accept port */
#define MAX_RETRIES 10
int gtalk_accept_port;               /* Global accept port setting */

#endif  /* _GTALK_GT_H */
