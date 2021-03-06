/*********************************

   GT Main.c

 *********************************/

#ifndef _GTALK_GTMAIN_H
#define _GTALK_GTMAIN_H

#include "common.h"

struct login_checks_struct {
    int (*check_login)(int portnum);
};

#define GENERIC_PASSWD  "eatlots"
#define USEREDIT_PASSWD "horton"
#define CREDIT_PASSWD   "sneety"


extern int mynum;
extern node_struct *mynode;
extern device_struct *mydev;
extern int mypid;
extern int mypipe;
extern int ml_logout;

void set_online_info(node_struct *a_node,
		     struct unique_information_struct *user_data,
		     struct class_data *class_info);
void make_manual_user(struct class_data *cptr);
#endif   /* _GTALK_GTMAIN_H */

