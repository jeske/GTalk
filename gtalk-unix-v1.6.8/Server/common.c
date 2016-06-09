
/*****************************************
    Routines for maintain common
    structures in parent 
 *****************************************/

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <time.h>

#include "gt.h"
#include "common.h"
#include "fork.h"

void init_common_area(common_struct *c)
{
  int i;

  c_nodes_used = MAX_NODES;
  c_devices_used = MAX_DEVICES;
}

node_id next_empty_console_node(void)
{
   int i;

   for (i=0;i<c_nodes_used;i++)
     if (c_nodes(i)->status == NODE_EMPTY)
       return (i);
   return (-1);
}  

node_id next_empty_node(void)
{
   int i;

   for (i=1;i<c_nodes_used;i++)
     if (c_nodes(i)->status == NODE_EMPTY)
       return (i);
   return (-1);
}  

node_id next_empty_device(void)
{
   int i;

   for (i=0;i<c_devices_used;i++)
     if (c_devices(i)->status == DEVICE_EMPTY)
       return (i);
   return (-1);
} 

node_id add_default_modem_device(char *dev, int baud, int lock_dte)
{
   node_id dev_no = next_empty_device();
   device_struct *n;

   if (dev_no < 0)
     return (-1);

   n = c_devices(dev_no);

   n->node_type = MODEM_NODE_TYPE;
   n->status = DEVICE_UNUSED;
   n->term_pid = -1;
   n->owner_pid = -1;
   n->assist_pid = -1;
   strncpy(n->name, dev, DEVICE_NAME_LEN);
   n->name[DEVICE_NAME_LEN] = '\000';
   strcpy(n->init1,"AT");
   strcpy(n->init2,"ATX4M1S0=1");
   n->baud_rate = baud;
   n->lock_dte = lock_dte;
   n->rts_cts = lock_dte;
   n->retries = 0;
   return (dev_no);
}

node_id add_telnet_device(char *dev)
{
   node_id dev_no = next_empty_device();
   device_struct *n;

   if (dev_no < 0)
     return (-1);

   n = c_devices(dev_no);

   n->node_type = TELNET_NODE_TYPE;
   n->status = DEVICE_UNUSED;
   n->term_pid = -1;
   n->owner_pid = -1;
   n->assist_pid = -1;
   strncpy(n->name, dev, DEVICE_NAME_LEN);
   n->name[DEVICE_NAME_LEN] = '\000';
   return (dev_no);
}

node_id add_serial_device(char *dev, int baud)
{
   node_id dev_no = next_empty_device();
   device_struct *n;

   if (dev_no < 0)
     return (-1);

   n = c_devices(dev_no);

   n->node_type = SERIAL_NODE_TYPE;
   n->status = DEVICE_UNUSED;
   n->term_pid = -1;
   n->owner_pid = -1;
   n->assist_pid = -1;
   strncpy(n->name, dev, DEVICE_NAME_LEN);
   n->name[DEVICE_NAME_LEN] = '\000';
   n->lock_dte = 0;
   n->rts_cts = 0;
   n->retries = 0;
   return (dev_no);
}

node_id add_direct_device(char *dev)
{
   node_id dev_no = next_empty_device();
   device_struct *n;

   if (dev_no < 0)
     return (-1);

   n = c_devices(dev_no);

   n->node_type = DIRECT_NODE_TYPE;
   n->status = DEVICE_UNUSED;
   n->term_pid = -1;
   n->owner_pid = -1;
   n->assist_pid = -1;
   strncpy(n->name, dev, DEVICE_NAME_LEN);
   n->name[DEVICE_NAME_LEN] = '\000';
   n->retries = 0;
   return (dev_no);
}

void delete_device(node_id dev_no)
{
  device_struct *n;

  if ((dev_no < 0) || (dev_no >= c_devices_used))
    return;
  n = c_devices(dev_no);
  n->node_type = 0;
  n->status = DEVICE_EMPTY;
}
