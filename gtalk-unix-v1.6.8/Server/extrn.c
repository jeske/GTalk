
/*************************

  Dan's external modules

*************************/

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
#include <sys/ioctl.h>


#include "bufio.h"
#include "types.h"
#include "gt.h"
#include "common.h"
#include "fork.h"
#include "telnd.h"
#include "extrn.h"

#define EXTRN_COPY_BUF_LEN 1024

static int goodbye = 0;

static void extrn_got_sig_pipe(int signo)
{
  goodbye = 1;
}

void copy_extrn_socket(int sockfd, int spipe, int nid)
{
  fd_set read_fd;
  int temp;
  int maxfd = (sockfd > spipe) ? (sockfd+1) : (spipe+1);
  char msg[EXTRN_COPY_BUF_LEN];
  int goodbye = 0;
  int last_cr = 0;

  signal(SIGPIPE, extrn_got_sig_pipe);

  fcntl(sockfd, F_SETFL, O_NONBLOCK); 
  fcntl(spipe, F_SETFL, O_NONBLOCK); 

  // temp = 1;
  // setsockopt(sockfd,IPPROTO_TCP,TCP_NODELAY,(char *) &temp,sizeof(temp)); 
  temp = 1;
  setsockopt(sockfd,SOL_SOCKET,SO_OOBINLINE,(char *) &temp,sizeof(temp));

  while (!goodbye)
    {
      FD_ZERO(&read_fd);
      FD_SET(sockfd, &read_fd);
      FD_SET(spipe, &read_fd);

      temp = select(maxfd,&read_fd,NULL,NULL,NULL);
      
      if (temp > 0)
	{
	  if (FD_ISSET(sockfd,&read_fd))
	    {
	      if ((temp = recv(sockfd,msg,EXTRN_COPY_BUF_LEN,0)) > 0)
		{
		  if (*msg == 0)
		    {
		      if (last_cr)
			temp = 0;
		    }
		  last_cr = (*msg == 0x0D);
		  if (write(spipe,msg,temp,0) < 0)
		    goodbye = 1;
		}
	      else
		if (temp <= 0) 
		  goodbye = 1;
	    }
	  if (FD_ISSET(spipe,&read_fd))
	    {
	      if ((temp = read(spipe,msg,EXTRN_COPY_BUF_LEN)) > 0)
		{
		  if (write(sockfd,msg,temp) < 0)
		    goodbye = 1;
		} else
		  goodbye = 1;
	    }
	} 
    }
}

int find_next_highest_extra_node(void)
{
  int highnode = c_devices_used;
  int dcount;
  int higher = 1;

  while (higher)
    {
      higher = 0;
      for (dcount=0;dcount<c_nodes_used;dcount++)
	{
	  if (c_nodes(dcount)->num == highnode)
	    {
	      higher = 1;
	      highnode++;
	      break;
	    }
	}
    }
  return (highnode);
} 

void extrn_accept_connections_on(int fd)
{
  int cli_fd, temp;
  struct sockaddr_in clisock;
  node_struct *n;
  gtalk_socketpair spair;
  node_id nid;

  temp = sizeof(struct sockaddr_in);

  if ((cli_fd = accept(fd, (struct sockaddr *) &clisock, 
		       &temp)) < 0) {
    log_error("Did not accept connection on port %d",GTALK_ACCEPT_PORT);
    return;
  }
  else
    {
      int ptyfd, pid;
      node_id node = find_next_highest_extra_node();

      nid=next_empty_node();

      if (nid < 0)
	{
	  close(cli_fd);
	  return;
	}
      n = c_nodes(nid);

      if ((pid=gtalk_fork(&spair)) < 0)
	{
	  log_error("Could not create process in extrn module");
	  close(cli_fd);
	  return;
        } else
      if (!pid)
	{
	  close_all_files_except(spair.mypipe,spair.mypipe,
				 spair.mypipe,cli_fd); 
	  copy_extrn_socket(cli_fd, spair.mypipe, nid);
	  exit(1);
	}
      close(cli_fd);
      new_node_and_device(n, -1, node+1, pid, spair.mypipe);
      if (is_pid_alive(n->pid) < 0)
	{
	  kill_with_pid(n->pid, 0);
	  return;
	}
      if (fd_buffer(n->fd_pipe, &n->pipebuf, BUFIO_RSIZE,
		    BUFIO_WSIZE, 1) < 0)
	kill(n->pid, SIGHUP);
      add_buffer_to_select(&n->pipebuf);
    }
}




