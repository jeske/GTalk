
/******************************************

            G-Talk main module

*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>

#include "gt.h"
#include "fork.h"
#include "common.h"

int create_common_area(void)
{
  if (create_shared_memory() < 0)
    return (-1);
  init_common_area(c);
  return (0);
}

int my_socket(gtalk_socketpair *fds)
{
  return (fds->mypipe);
}

int reverse_socketpair(gtalk_socketpair *fds)
{
  int temp;

  temp = fds->mypipe;
  fds->mypipe = fds->otherpipe;
  fds->otherpipe = temp;
}

int new_socketpair(gtalk_socketpair *fds)
{
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, (int *) fds) < 0)
    {
      log_error("Server: Could not create socketpair\n");
      return (-1);
    }
  return (0);
}

int close_socketpair(gtalk_socketpair *fds)
{
  close(fds->mypipe);
  close(fds->otherpipe);
}

int gtalk_fork(gtalk_socketpair *fds)
{
  int pid;

  if (new_socketpair(fds) < 0)
    {
      log_error("Could not create socketpair");
      return (-1);
    }

  if ((pid = fork()) < 0)
    {
      log_error("Could not create new process");
      close_socketpair(fds);
      return (-1);
    }
  if (pid)
    {
      close(fds->otherpipe);
      return (pid);
    }
  reverse_socketpair(fds);
  close(fds->otherpipe);
#if 0
  if (connect_to_shm() < 0)
    exit(1);
#endif
  return (0);
}









