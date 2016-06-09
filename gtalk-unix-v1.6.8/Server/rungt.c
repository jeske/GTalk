
/**************************************

    Module invokes g-talk client

 **************************************/

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

#include "common.h"
#include "rungt.h"

void gtalk_main(int nodenum, int pipe_fd, int relog)
{
  char s_nodenum[MAX_ENV_LN_LEN+1];
  char s_pipe_fd[MAX_ENV_LN_LEN+1];
  char s_relog[MAX_ENV_LN_LEN+1];

  sprintf(s_nodenum,"%d", nodenum);
  sprintf(s_pipe_fd,"%d", pipe_fd);
  sprintf(s_relog,"%d", relog);
  log_error("Executing %s: node %d, pipe_fd %d, relog %d",GTALK_CLIENT,
	nodenum, pipe_fd, relog);
  if (execl(GTALK_CLIENT, GTALK_CLIENT, s_nodenum, s_pipe_fd, s_relog, NULL) == -1) {
     log_error("Error in execl() in rungt.c");
  }
  /* shouldn't get down here */
  exit(1);
}

