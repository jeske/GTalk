
/********************************

          extcon.h

 ********************************/

#ifndef _GTALK_EXTCON_H
#define _GTALK_EXTCON_H

#include "common.h"

int extrn_connect_shm(void);
int extrn_connect_pipe(char *host);

extern common_struct *c;
extern int shmid;
extern int mypipe;

#endif   /* _GTALK_EXTCON_H */
