


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* User.h */

#include "userst.h"

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
typedef struct user_data far user_dat;
typedef struct u_parameters user_p;

extern user_dat far user_lines[MAXPORTS];
extern user_p far user_options[MAXPORTS];

