
/**********************

  Dan's mini telnetd

 **********************/

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

#define SHOW_ERRORS
#define ERROR_LOG_FILE "errlog"

char telnet_str[] = { IAC, WILL, TELOPT_ECHO, IAC, WILL, TELOPT_SGA };

int telnd_log_error(char *format, ...)
{
  va_list list;
  int temp;
  FILE *fp;
  time_t tim = time(NULL);

#ifndef SHOW_ERRORS
  return;
#endif

  if (!(fp=fopen(ERROR_LOG_FILE,"a")))
    fp = stderr;
  va_start(list, format);
  temp=vfprintf(fp,format,list);
  fprintf(fp," %s",ctime(&tim));
  va_end(list);
  if (fp != stderr)
    fclose(fp);
  return (temp);
}

int pty_pair(int fd[2], char *name)
{
  char *ptr1, *ptr2;
  struct group *grptr;
  int gid;

  gid =  ((grptr=getgrnam("tty")) == NULL) ? -1 : grptr->gr_gid;

  strcpy(name,"/dev/ptyXY");
  for (ptr1 = "pqrstuvwxyzPQRST"; *ptr1 != 0; ptr1++) {
    name[8] = *ptr1;
    for (ptr2 = "0123456789abcdef"; *ptr2 !=0 ; ptr2++) {
      name[5] = 'p';
      name[9] = *ptr2;
      if ((fd[0] = open(name, O_RDWR | O_NOCTTY)) < 0)
	{
	  if (errno == ENOENT)
	    {
	      log_error("Got ENOENT error in Open");
	      return (-1);
	    }
	  else
	    continue;
	}
      name[5] = 't';
      chown(name,getuid(),gid);
      chmod(name,S_IRUSR | S_IWUSR | S_IWGRP);
      if ( (fd[1] = open(name, O_RDWR | O_NOCTTY)) < 0) {
	log_error("Could not open slave pty %s",name);
	close(fd[0]);
	continue;
      }
      return (0);
    }
  }
  return (-1);
}

void close_all_files_except(int fd1, int fd2, int fd3, int fd4)
{
  int i;

  for (i=0;i<255;i++)
    if ((i != fd1) && (i != fd2) && (i != fd3) && (i != fd4))
      close(i);
}

int gtalk_coprocess(int *fd, struct termios *slave_termios,
	       struct winsize *slave_winsize, char *name,
	       gtalk_socketpair *pipe_fd)
{
  int slave;
  int cfd[2];

  if (pty_pair(cfd, name) < 0)
    {
      log_error("Could not create pty pair");
      return (-1);
    }

  if ((slave = gtalk_fork(pipe_fd)) < 0)
    return (-1);
  else
    if (!slave)         /* actually, if we ARE the slave */
      {
	int fds = cfd[1];

	close(cfd[0]);
	*fd = fds;
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
	if (slave_termios)
	  if (tcsetattr(fds, TCSANOW, slave_termios) < 0)
	    exit(1);
	if (slave_winsize)
	  if (ioctl(fds, TIOCSWINSZ, slave_winsize) < 0)
	    exit(1);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	if (dup2(fds, STDIN_FILENO) != STDIN_FILENO)
	  exit(1);
	if (dup2(fds, STDOUT_FILENO) != STDOUT_FILENO)
	  exit(1);
	if (dup2(fds, STDERR_FILENO) != STDERR_FILENO)
	  exit(1);
	if (fds > STDERR_FILENO)
	  close(fds);
	return (0);
      }
  close (cfd[1]);
  *fd = cfd[0];
  return (slave);
}

int bind_server_to(int port)
{
  struct sockaddr_in sock;
  int accept_fd;

  bzero((char *)&sock, sizeof(sock));
  sock.sin_family = AF_INET;
  sock.sin_addr.s_addr = htonl(INADDR_ANY);
  sock.sin_port = htons(port);

  if ((accept_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      log_error("Could not create socket for port %d",port);
      return(-1);
    }
  if (bind(accept_fd, (struct sockaddr *) &sock, sizeof(sock)) < 0)
    {
      log_error("Could not bind to port %d", port);
      close(accept_fd);
      return(-1);
    }
  listen(accept_fd, 5);
  return (accept_fd);
}

int send_all(register int fd, register char *data, register int bytes, int opt)
{
  int ret;

  while (bytes > 0)
    {
      ret = send(fd, data, bytes, opt);
      if (ret < 0)
	return (-1);
      data += ret;
      bytes -= ret;
    }
  return (0);
}

#define COPY_BUF_LEN 512

static int goodbye = 0;

static void got_sig_pipe(int signo)
{
  goodbye = 1;
}

void copy_between_socket(int sockfd, int ptyfd, int cli_pid)
{
  fd_set read_fd;
  int temp;
  int maxfd = (sockfd > ptyfd) ? (sockfd+1) : (ptyfd+1);
  char msg[COPY_BUF_LEN];
  int goodbye = 0;
  int last_cr = 0;

  signal(SIGHUP, got_sig_pipe);
  signal(SIGPIPE, got_sig_pipe);
  signal(SIGALRM, SIG_IGN);

  alarm(2);

  read(sockfd,msg,COPY_BUF_LEN);

  fcntl(sockfd, F_SETFL, O_NONBLOCK); 
  fcntl(ptyfd, F_SETFL, O_NONBLOCK); 

  temp = 1;
  setsockopt(sockfd,SOL_SOCKET,SO_OOBINLINE,(char *) &temp,sizeof(temp));

  while (!goodbye)
    {
      FD_ZERO(&read_fd);
      FD_SET(sockfd, &read_fd);
      FD_SET(ptyfd, &read_fd);

      temp = select(maxfd,&read_fd,NULL,NULL,NULL);
      
      if (temp > 0)
	{
	  if (FD_ISSET(sockfd,&read_fd))
	    {
	      if ((temp = recv(sockfd,msg,1,0)) > 0)
		{
		  if (*msg == 0)
		    {
		      if (last_cr)
			temp = 0;
		    }
		  last_cr = (*msg == 0x0D);
		  if (write(ptyfd,msg,temp,0) < 0)
		    goodbye = 1;
		}
	      else
		if (temp <= 0) 
		  goodbye = 1;
	    }
	  if (FD_ISSET(ptyfd,&read_fd))
	    {
	      if ((temp = read(ptyfd,msg,COPY_BUF_LEN)) > 0)
		{
		  if (write(sockfd,msg,temp) < 0)
		    goodbye = 1;
		} else
		  goodbye = 1;
	    }
	} 
    }
  kill(cli_pid,SIGHUP);
  close(ptyfd);
  shutdown(sockfd, 2);
  close(sockfd);
}

char no_available_devices[] = "\n\nNo ports available, try again later\n";

void write_no_avail(int fd)
{
  write(fd, no_available_devices, strlen(no_available_devices));
}

void accept_connections_on(int fd)
{
  int cli_fd, temp;
  struct sockaddr_in clisock;
  int copypid;
  device_struct *d;
  node_struct *n;
  char tty_name[20];
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
      node_id node;

      d = NULL;
      for (node=0;node<c_devices_used;node++)
	{
	  if (c_devices(node)->status == DEVICE_EMPTY)
	    {
	      d = c_devices(node);
	      break;
	    }
	}
	      
      nid=next_empty_node();

      if ((!d) || (nid < 0))
	{
	  write_no_avail(cli_fd);
	  close(cli_fd);
	  return;
	}

      n = c_nodes(nid);

      if ((pid=gtalk_coprocess(&ptyfd, NULL, NULL, tty_name, &spair)) < 0)
	{
	  log_error("Could not create coprocess");
	  write_no_avail(cli_fd);
	  close(cli_fd);
	  return;
        } else
      if (!pid)
        { close_all_files_except(0,1,2,spair.mypipe); 
          strncpy(c_nodes(nid)->ip,inet_ntoa(clisock.sin_addr),16);
	  gtalk_main(nid, spair.mypipe, 0);
	  exit(1);
	}
      if ((copypid=fork()) < 0)
	{
	  kill(pid,SIGKILL);
	  close(spair.mypipe);
	  close(cli_fd);
	  return;
	} else
      if (!copypid)
	{
	  close_all_files_except(cli_fd, ptyfd, ptyfd, ptyfd);
	  write(cli_fd, telnet_str, 6);  
	  copy_between_socket(cli_fd, ptyfd, pid);
	  exit(1);
	}
      close(ptyfd);
      close(cli_fd);
      d->assist_pid = copypid;
      d->node_type = TELNET_NODE_TYPE;
      d->retries = 0;
      strncpy(d->name,tty_name,DEVICE_NAME_LEN);
      new_node_and_device(n, node, node+1, pid, spair.mypipe);
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

void telnd_main(void)
{
  int fd;

  if ((fd=bind_server_to(gtalk_accept_port)) < 0)
    exit(1);
  for (;;)
    accept_connections_on(fd);
}




