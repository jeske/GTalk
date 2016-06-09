
/******************************************

            G-Talk main module

*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <termios.h>
#include <arpa/telnet.h>
#include <sys/wait.h>

#include "bufio.h"
#include "gt.h"
#include "fork.h"
#include "common.h"
#include "srv_abuf.h"
#include "telnd.h"
#include "srv_channel.h"
#include "states.h"
#include "chuser.h"
#include "schedule.h"
#include "srv_login.h"

int parent_pid;

int check_nodes = 1;               /* set whenever a node dies */
int child_dead = 0;

int is_pid_alive(int pid)
{
  char s[20];
  struct stat buf;

  if (kill(pid, 0) < 0)
    return (errno == ESRCH ? -1 : 0);
  return (0);
}

static void no_handler(int signo)
{

}

void new_node_and_device(node_struct *n, node_id dev, int node, 
			 int new_pid, int fd_pipe)
{
  device_struct *d;

  if (dev >= 0)
    d = c_devices(dev);
  n->num = node;
  n->abuf_state.state = ABUF_STATE_NONE;
  if (dev >= 0)
    {
      d->owner_pid = new_pid;
      d->term_pid = -1;
    }
  n->pid = new_pid;
  n->fd_pipe = fd_pipe;
  n->dev_no = dev;
  if (dev >= 0)
    d->status = DEVICE_USED;
  n->status = NODE_IDLE;
  n->timeout_status = TIMEOUT_NONE;
}

void clear_node_data(node_id node)
{
  node_struct *n = c_nodes(node);

  broadcast_login_message("LOGOUT %lu/%d", my_ip, node);
  switch (n->sigusr1_action)
	{
	case SIG1_ACTION_KILL:
		take_off_channels(node, "Killed");
		break;
	case SIG1_ACTION_TIMEOUT:
		take_off_channels(node, "Timeout");
		break;
	default:
		take_off_channels(node, "Logout");
		break;
	}
  delete_buffer_from_select(&n->pipebuf);
  kill_fd_buffer(&n->pipebuf);
  n->status = NODE_EMPTY;
  n->fd_pipe = n->pid = -1;
  check_nodes = 1;
}

void spawn_off_unused_ports(void)
{
  node_id i;
  device_struct *d;
  node_struct *n;
  int fds;
  int pid;
  gtalk_socketpair pipe_fd;
  node_id nid;

  for (i=0;i<c_devices_used;i++)
    {
      d=c_devices(i);
      if ((d->status == DEVICE_UNUSED) && 
	  (d->node_type != TELNET_NODE_TYPE))
	{
	  if (d->node_type == DIRECT_NODE_TYPE)
	    nid = next_empty_console_node();
	  else
	    nid = next_empty_node();
	  if (nid >= 0)
	    {
	      n = c_nodes(nid);
	      if ((pid=gtalk_fork(&pipe_fd)) < 0)
		return;
	      else
		if (pid == 0)
		  {
		    if ((fds=open(d->name,O_RDWR|O_NOCTTY|O_NONBLOCK)) < 0)
		      {
			log_error("Unsuccessful port open");
			exit(1);
		      }
		    if (setsid() < 0)
		      {
			log_error("Unsuccessful setsid()");
			exit(1);
		      }
		    if (ioctl(fds, TIOCSCTTY, (char *) 0) < 0)
		      {
			log_error("Unsuccessful TIOCSCTTY");
			exit(1);
		      }
		    fcntl(fds, F_SETFL, O_SYNC); 
		    if (dup2(fds, STDIN_FILENO) != STDIN_FILENO)
		      exit(1);
		    if (dup2(fds, STDOUT_FILENO) != STDOUT_FILENO)
		      exit(1);
		    if (dup2(fds, STDERR_FILENO) != STDERR_FILENO)
		      exit(1);
		    if (fds > STDERR_FILENO)
		      close(fds);
		    close_all_files_except(STDIN_FILENO, STDOUT_FILENO,
			  STDERR_FILENO, pipe_fd.mypipe);
		    gtalk_main(nid, pipe_fd.mypipe, 0);
		    exit(0);
		  }
	      d->assist_pid = -1;
	      new_node_and_device(n, i, i+1, pid, pipe_fd.mypipe);
	      if (is_pid_alive(n->pid) < 0)
		{
		  kill_with_pid(n->pid, 0);
		  continue;
		}
	      if (fd_buffer(n->fd_pipe, &n->pipebuf, BUFIO_RSIZE,
			    BUFIO_WSIZE, 1) < 0)
		kill(n->pid, SIGHUP);
	      add_buffer_to_select(&n->pipebuf);
	    }
	}
    }
}

void disable_nodes_with_pid(int pid)
{
  int i;

  for (i=0;i<c_nodes_used;i++)
    {
      node_struct *n = c_nodes(i);

      if ((n->pid == pid) && (n->status != NODE_EMPTY))
	{
	  int temp = n->fd_pipe;

	  clear_node_data(i);
	  close(temp);
#ifdef PROC_DEBUG
	  printf("closed down task for node %d pid=%d\n",i,n->pid);
#endif
	}
    }
}

static void set_child_dead(int signo)
{
  int sav = errno;

  child_dead = 1;
  signal(SIGCHLD, set_child_dead);
  errno = sav;
}

void kill_with_pid(int pid, int exit_stat)
{
  node_id i;

  disable_nodes_with_pid(pid);
  for (i=0;i<c_devices_used;i++)
    {
      device_struct *d = c_devices(i);
      
#ifdef PROC_DEBUG
      if (d->status)
	printf("d->owner=%d, d->assist=%d, d->status=%d\n",
	       d->owner_pid,d->assist_pid,d->status);
#endif
      if ((d->owner_pid == pid) && (d->status == DEVICE_USED))
	{
	  d->owner_pid = -1;
	  
	  if (exit_stat == 0)
	    d->retries = 0;
	  else
	    if (exit_stat == 1)
	      {
		d->retries++;
		if (d->retries == MAX_RETRIES)
		  d->status = DEVICE_OFF;
	      }
	  if (d->status != DEVICE_OFF)
	    {
	      d->status = (d->node_type == TELNET_NODE_TYPE) ?
		DEVICE_EMPTY : DEVICE_UNUSED;
	    }
	  check_nodes = 1;
	  d->term_pid = -1;
	  if (d->assist_pid != -1)
	    {
	      int temp = d->assist_pid;
	      int tstat;
	      
	      d->assist_pid = -1;
	      if (kill(temp, SIGHUP) == 0)
		waitpid(temp, &tstat, 0);
	    }
	}
      if ((d->term_pid == pid) && (d->status == DEVICE_TERM))
	{
	  if (d->status == DEVICE_TERM)
	    d->status = DEVICE_UNUSED;
	  check_nodes = 1;
	}
      if ((d->assist_pid == pid) && (d->status == DEVICE_USED))
	{  
	  int temp = d->owner_pid;
	  int tstat;
	  
	  d->owner_pid = -1;
	  
	  if (exit_stat == 0)
	    d->retries = 0;
	  else
	    if (exit_stat == 1)
	      {
		d->retries++;
		if (d->retries == MAX_RETRIES)
		  d->status = DEVICE_OFF;
	      }
	  if (d->status != DEVICE_OFF)
	    {
	      d->status = (d->node_type == TELNET_NODE_TYPE) ?
		DEVICE_EMPTY : DEVICE_UNUSED;
	    }
	  check_nodes = 1;
	  d->term_pid = -1;
	  d->assist_pid = -1;
	  if (temp != -1)
	    {
	      disable_nodes_with_pid(temp);
	      if (kill(temp, SIGHUP) == 0)
		waitpid(temp, &tstat, 0);
	    }
	}
    }
}
  
void crib_death(void)
{
  int pid, status;
  int exit_stat;

  while ((pid=waitpid(-1, &status, WNOHANG)) > 0)
    {
      exit_stat = WEXITSTATUS(status);
#ifdef PROC_DEBUG
      printf("got SIGCHLD pid = %d status = %d\n",pid,exit_stat);
#endif
      kill_with_pid(pid, exit_stat);
    }
}

static void gtalk_shutdown(int signo)
{
  int i;

  if (getpid() != parent_pid)
    return;

  printf("\nGtalk shutting down on a %s!\n",sys_siglist[signo]);
  log_error("Gtalk shutting down on a %s!",sys_siglist[signo]);

  for (i=0;i<c_nodes_used;i++)
    {
      node_struct *n = c_nodes(i);
      
      if (n->status != NODE_EMPTY)
	{
	  if (n->pid != -1)
	    kill(n->pid,SIGHUP);
	}
    }
  free_shared_memory();
  exit(1);
}

node_struct *node_with_fd(int fd)
{
  int i;
  
  for (i=0;i<c_nodes_used;i++)
    {
      if (c_nodes(i)->fd_pipe == fd)
	return (c_nodes(i));
    }
  return (NULL);
}  

int outward_bound(abuffer *abuf, char *cargo)
{
  int i;

  if (abuf->dest_machine == my_ip)
    {
      if (abuf->dest_process < c_nodes_used)
      {
	if (c_nodes(abuf->dest_process)->status == NODE_ACTIVE)
	  write_iobuf_abuffer(&(c_nodes(abuf->dest_process)->pipebuf),
			      abuf, cargo);
	return (1);
      } 
    } else
    {
      printf("outbound packet\n");
      /* linking crap goes here */
      return (1);
    }
  return (0);
}

void init_sys_vars(void)
{
  c_sys_info->lock_priority=0;
  c_sys_info->lock_priority_telnet=0;
  strcpy(c_sys_info->system_name,"Nuclear Greenhouse");
  c_sys_info->system_number=1;
  strcpy(c_sys_info->sys_phone,"(847)998-0008");

  c_sys_info->last_uptime = c_sys_info->uptime;
  c_sys_info->uptime = time(NULL);
}

struct modem_dev_st {
char *dev_name;
int baud;
int fixed_dte_rate;
} modemcfg[] ={ 
		/* { "/dev/ttyD1",  38400, 1}, /* Hayes 28.8k (Top) */
	 	/* { "/dev/ttyD17", 38400, 1}, /* Hayes 28.8k (Middle) */
		/* { "/dev/ttyD18", 38400, 1}, /* Hayes 28.8k (Bottom) */
		{ "/dev/ttyS0",	 38400, 1}, /* (NEW) Hayes 28.8k (1) */
                { "/dev/ttyS1",  38400, 1}, /* (NEW) Hayes 28.8k (2) */
                { "/dev/ttyS2",  38400, 1}, /* (NEW) Hayes 28.8k (3) */
	 	/* { "/dev/ttyD19", 2400 , 0}, /* Modem Rack (3) */
	 	/* { "/dev/ttyD20", 2400 , 0}, /* Modem Rack (4) */
	 	/* { "/dev/ttyD21", 2400 , 0}, /* Modem Rack (5) */
	 	/* { "/dev/ttyD22", 2400 , 0}, /* Modem Rack (6) */
	 	/* { "/dev/ttyD2",  2400 , 0}, /* Modem Rack (7) */
		/* { "/dev/ttyD23", 2400 , 0}, /* External Modem (Top/PP) */
		/* { "/dev/ttyD24", 2400 , 0}, /* External Modem (Bottom) */
/*	 	{ "/dev/ttyD04", 38400, 1}, /* BLOWN OUT */
/*	 	{ "/dev/ttyD05", 38400, 1}, /* BLOWN OUT */
		{ NULL, 0, 0 }};


int become_daemon(void)
{
  int pid;

  close(0);
  close(1);
  close(2);
  if ((pid=fork()) < 0)
    return (-1);
  if (pid)
    exit(0);
  setsid();
  umask(0);
  return (0);
}

void main(int argc, char **argv)
{
  int pid;
  int accept_fd;
  int extrn_accept_fd;
  gtalk_socketpair fds;
  struct modem_dev_st *modems = modemcfg;

  if (become_daemon() < 0)
    {
      log_error("Could not daemonize");
      exit(1);
    }

  parent_pid = getpid();
  
  init_abuffers();
  init_state_list(default_parent_state_list, &parent_state_list);
  if (init_channels() < 0)
    exit(1);

  if (create_schedule_list() < 0)
    exit(1);

  if (create_common_area() < 0)
    {
      log_error("Could not create shared memory block");
      exit(1);
    }

  init_sys_vars();

  if (add_direct_device("/dev/tty7") < 0)
    {
      log_error("Could not add console device: /dev/tty7");
      exit(1);
    }

  if (add_direct_device("/dev/tty8") < 0)
    {
      log_error("Could not add console device: /dev/tty8");
      exit(1);
    }
 
  while (modems->dev_name)
   {
  	if ((add_default_modem_device(modems->dev_name,modems->baud,
		modems->fixed_dte_rate)) < 0)
   	 {
   	   log_error("Could not add device [%s]",modems->dev_name);
	   exit(1);
    	}
	else
	{ 
	   log_error("Added [%s] B%d, DTE:%d.",modems->dev_name,
		modems->baud, modems->fixed_dte_rate);
	}
    modems++;
   }

  if ((accept_fd = bind_server_to(GTALK_ACCEPT_PORT)) < 0)
    {
      log_error("Could not bind telnet to port %d",GTALK_ACCEPT_PORT);
      exit(1);
    }
  if ((extrn_accept_fd = bind_server_to(EXTRN_GTALK_ACCEPT_PORT)) < 0)
    {
      log_error("Could not bind external to port %d",EXTRN_GTALK_ACCEPT_PORT);
      exit(1);
    }

  signal(SIGCHLD, set_child_dead);
  signal(SIGPIPE, SIG_IGN);
  signal(SIGTERM, gtalk_shutdown);
  signal(SIGHUP, gtalk_shutdown);
  signal(SIGINT, gtalk_shutdown);
  signal(SIGSEGV, gtalk_shutdown);
  signal(SIGBUS, gtalk_shutdown);

  for (;;)
    {
      int i;

      if (child_dead)
	{
	  child_dead = 0;
	  crib_death();
	}
      if (check_nodes)
	{
	  check_nodes = 0;
	  spawn_off_unused_ports();
	}

      i = poll_buffers(calc_next_event_sec(), accept_fd, extrn_accept_fd);

      if (i < 0)
	i = 0;
      if (!i)
	see_if_event_occurs();
      if (i & POLL_ACCEPT)
	accept_connections_on(accept_fd);
      if (i & POLL_ACCEPT2)
	extrn_accept_connections_on(extrn_accept_fd);
      if (i & POLL_READ)
	{
/* This stuff processes abuf messages, and is the primary
   purpose of this task */
	  static char abuf_data[4096];
	  int j,k;

	  for (j=0;j<c_nodes_used;j++)
	    {
	      node_struct *n = c_nodes(j);

	      if (n->status != NODE_EMPTY)
		{
		  if (!buffer_input_empty(&n->pipebuf))
		    {
		      int temp;
		      abuffer abuf;

		      while ((temp=read_iobuf_abuffer(&n->pipebuf,
			       &n->abuf_state, abuf_data,
             		       sizeof(abuf_data)-1)) !=
			     ABUF_IO_NOTREADY)
			{
			  if (temp == ABUF_IO_READY)
			    {
			      if (!outward_bound(&n->abuf_state.abuf, 
						 abuf_data))
				call_state_machine(&n->abuf_state.abuf,
						   abuf_data,
						   &parent_state_list);
			    }
			}
		    }
		}
	    }
	}
    }
}
