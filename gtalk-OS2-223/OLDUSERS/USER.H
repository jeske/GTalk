/* USER.H */

#ifndef GT_USER_H
#define GT_USER_H


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#include "com.h"
#include "userst.h"
#include "userst2.h"

#define NUMDEFAULT 7
#define DEF_GUEST -1
#define DEF_REG_GUEST -2
#define DEF_USER -3
#define DEF_BABY_CO -4
#define DEF_CO -5
#define DEF_SYSOP -6
#define EMPTY -7

int load_user(int number, struct user_data *user_ptr);
int save_user(int number, struct user_data *user_ptr);
void set_bit(unsigned char *privs,int bit,int state);
int test_bit(unsigned char *privs, int bit);

void update_members_list(void);
int load_access_of_user(int user_num,struct u_parameters *user,int portnum);


/* extern declarations */
typedef struct user_data user_dat;
typedef struct u_parameters user_p;

extern user_dat user_lines[MAXPORTS];
extern user_p user_options[MAXPORTS];


#endif /* GT_USER_H */
