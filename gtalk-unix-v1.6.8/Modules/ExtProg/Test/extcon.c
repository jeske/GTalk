
/************************************

               extcon.c

 ************************************/

#include <stdio.h>
#include <stdlib.h>

#ifdef LINUX
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#endif

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

#include "types.h"
#include "fork.h"
#include "gt.h"
#include "abuf.h"
#include "extcon.h"

common_struct *c;
int shmid;
int mypipe;

int extrn_connect_shm(void)
{
  int key = ftok(SHARED_MEMORY_TOKEN, SHARED_MEMORY_ID);

  if ((shmid = shmget(key, 0, 0)) < 0)
    return (-1);
  if (((c = (common_struct *) shmat(shmid, 0, 0))) ==
      ((common_struct *)-1))
    return (-1);
  return (0);
}

int extrn_connect_pipe(char *host)
{
  struct hostent *ent;
  struct sockaddr_in addr;

  bzero((char *) &addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(EXTRN_GTALK_ACCEPT_PORT);
  if (host)
    {
      if (!(ent = gethostbyname(host)))
	return (-1);
      if (*ent->h_addr_list)
	bcopy((char *) *ent->h_addr_list,
	      (char *) &addr.sin_addr.s_addr,
	      ent->h_length);
    } else
      addr.sin_addr.s_addr = htonl(0x7F000001);
  if ((mypipe = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    return (-1);
  if (connect(mypipe, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
      close(mypipe);
      return (-1);
    }
  return (0);
}

