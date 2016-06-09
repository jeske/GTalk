/* USEREDIT.H */

#ifndef GT_USEREDIT_H
#define GT_USEREDIT_H

#include "comparse.h"

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* USEREDIT.H */

int start_user_edit(com_struct *command, char *str);
void edit_privs(unsigned char *privs,char *filename);

void print_user_info(struct user_data *edituser,int num);
int exist(int number);


#endif /* GT_USEREDIT_H */
