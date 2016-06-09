
/*****************************************
    Routines to maintain shared memory
 *****************************************/

#ifdef LINUX
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#endif

#include "shared.h"

int shmid;
common_struct *c;

int free_shared_memory(void)
{
  if (shmctl(shmid, IPC_RMID, 0) < 0)
    {
      log_error("Could not free shared memory block %d ",shmid);
      return (-1);
    }
  return (0);
}

int create_shared_memory(void)
{
  int key = ftok(SHARED_MEMORY_TOKEN, SHARED_MEMORY_ID);

  if ((shmid = shmget(key, MEMORY_SIZE, 0700 | IPC_CREAT)) < 0)
    {
      log_error("Server: Could not create new shared memory block");
      return (-1);
    }
  if ((c = (common_struct *) shmat(shmid, 0, 0)) == (common_struct *) -1)
    {
      free_shared_memory();
      log_error("Server: Could not attach to shared memory block %d", shmat);
      return (-1);
    }
  memset(c, 0, MEMORY_SIZE);
  return (0);
}

int connect_to_shm(void)
{
  int key = ftok(SHARED_MEMORY_TOKEN, SHARED_MEMORY_ID);

  if ((shmid = shmget(key, MEMORY_SIZE, 0700)) < 0)
    {
      log_error("Server: Could not create new shared memory block");
      return (-1);
    }
  if ((c = (common_struct *) shmat(shmid, 0, 0)) == (common_struct *) -1)
    {
      free_shared_memory();
      log_error("Server: Could not attach to shared memory block %d", shmat);
      return (-1);
    }
  return (0);
}
