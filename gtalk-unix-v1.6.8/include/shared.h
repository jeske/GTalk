/*****************************************
    Routines to maintain shared memory
 *****************************************/

#ifndef _GT_SHARED_H
#define _GT_SHARED_H

#include "common.h" 

#define MEMORY_SIZE 0x18000l
#define SHARED_MEMORY_TOKEN "GTALK"
#define SHARED_MEMORY_ID 100

extern int shmid;
extern common_struct *c;

int free_shared_memory(void);
int create_shared_memory(void);
int connect_to_shm(void);

#endif /* _GT_SHARED_H */
