
/* All of the shared variables go into this structure */

#ifndef _GTALK_COMMON_H
#define _GTALK_COMMON_H

#include "types.h"
#include "abuf.h"
#include "bufio.h" 
#include "gtst.h"

#define MAX_NODES 50
#define MAX_DEVICES 50
#define BUFIO_RSIZE 8192
#define BUFIO_WSIZE 8192


#define GTALK_HOME_DIR	"/home/gtalk"
#define GTALK_ROOT_DIR	"/home/gtalk/ROOT"


#include "userst.h"
#include "userst2.h"

#define NODE_EMPTY 0
#define NODE_IDLE 1
#define NODE_CONN_WAITING 2
#define NODE_CONNECTING 3
#define NODE_CONNECTED 4
#define NODE_LOGGING_IN 5
#define NODE_ACTIVE 6

#define is_node_online(n) ((n)->status == NODE_ACTIVE)

#define DEVICE_NAME_LEN 20
#define MODEM_STR_LEN 100

#define DEVICE_EMPTY 0
#define DEVICE_UNUSED 1
#define DEVICE_USED 2
#define DEVICE_TERM 3
#define DEVICE_OFF 4

#define SERIAL_NODE_TYPE 0
#define TELNET_NODE_TYPE 1
#define DIRECT_NODE_TYPE 2
#define MODEM_NODE_TYPE 3

#define TIMEOUT_NONE 0
#define TIMEOUT_WARNING 1
#define TIMEOUT_KILL 2

#define TIMEOUT_WARNING_TIME 60
#define LOGIN_TIME 60

#define SIG1_ACTION_NONE     0 
#define SIG1_ACTION_TIMEOUT  1
#define SIG1_ACTION_KILL     2
#define SIG1_ACTION_RELOG    3
#define SIG1_ACTION_LINEOUT  4

/* Added by Gregg */
#define TRUE 1
#define FALSE 0

typedef struct _device_struct
{
  int device_no;                 /* device_no */
  int node_type;                 /* node type */
  int status;                    /* used/unused or status */
  int owner_pid;                 /* pid of owner */
  int term_pid;                  /* pid of suspended user for /term */
  int assist_pid;                /* assistant pid (e.g. telnet) */
  int retries;                   /* number of retries for device */
  
  char name[DEVICE_NAME_LEN+1];  /* name of device (e.g. /dev/modem) */
  char init1[MODEM_STR_LEN+1];   /* init string 1 */
  char init2[MODEM_STR_LEN+2];   /* init string 2 */
  int baud_rate;                 /* baud rate of modem */
  char lock_dte;                 /* lock data terminal rate */
  char rts_cts;                  /* uses rts/cts? */      
} device_struct;

typedef struct _node_struct
{
  int status;                    /* status of node */
  int pid;                       /* pid of process on node */
  int num;                       /* node number */
  int link;			 /* Linked or not (true/false)*/
  int fd_pipe;                   /* socket to abuf of process */
  iobuf pipebuf;                 /* bufio buffer for pipe */
  abuffer_state abuf_state;      /* abuffer current header */
  node_id dev_no;                /* device no of process */
  char ip[16];			 /* Nodes IP address */
  char new_chan[21];             /* next channel to change to */
  char cur_chan[21];             /* current channel */
  struct _user_perm userdata;    /* user logged into node */
                                 /* (only valid for status == NODE_ACTIVE) */

  time_t timeout_time;           /* timeout time */
  int timeout_status;            /* timeout status */
  int sigusr1_action;            /* action to take for client sigusr1 */
} node_struct;

typedef struct _common_struct 
{
  int nodes_used;
  int devices_used; 
  struct sys_info_struct sys_info;
  struct sys_toggles_struct sys_toggles;
  struct _node_struct nodes[MAX_NODES];        /* space for nodes */
  struct _device_struct devices[MAX_DEVICES];  /* devices */
} common_struct;

#define c_devices(x) (&(c->devices[(x)]))
#define c_nodes(x) (&(c->nodes[(x)]))
#define c_nodes_used (c->nodes_used)
#define c_devices_used (c->devices_used)
#define c_sys_info (&c->sys_info)
#define c_sys_toggles (&c->sys_toggles)

void init_common_area(common_struct *c);
node_id add_default_modem_device(char *dev, int baud, int lock_dte);
node_id add_telnet_device(char *dev);
node_id add_direct_device(char *dev);
node_id add_serial_device(char *dev, int baud);
node_id next_empty_node(void);
node_id next_empty_console_node(void);
node_id next_empty_device(void);

#endif /* _GTALK_COMMON_H */

