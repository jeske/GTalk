

/*******************************

    Fork functions definition
             file

********************************/

#ifndef _GTALK_FORK_H
#define _GTALK_FORK_H

#include "common.h"
#include "gt.h"
#include "shared.h"

int gtalk_fork(gtalk_socketpair *fds);
int my_socket(gtalk_socketpair *fds);
int create_common_area(void);

#endif  /* _GTALK_FORK_H */
