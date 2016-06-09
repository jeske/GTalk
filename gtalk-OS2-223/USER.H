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

#define NO_SAVE_USER -1

#define LUERR_FREAD_FAILED 2
#define LUERR_CLOSE_FAILED 3
#define LUERR_START_BLOCK_READ_ERROR 4
#define LUERR_FILE_OPEN_ERROR 5
#define LUERR_INVALID_USER_FILE_RECORD_LENGTH 6
#define LUERR_NO_SUCH_USER 10


int load_user(int number, struct user_data *user_ptr);
int save_user(int number, struct user_data *user_ptr);
int load_class(int number,struct class_data *class_ptr);
int save_class(int number,struct class_data *class_ptr);
int load_class_by_name(char *name,struct class_data *class_ptr);
int save_class_by_name(char *name,struct class_data *class_ptr);
int add_class(struct class_data *class_ptr);


void set_user_class_info(struct class_defined_data_struct *class,
       struct user_class_information_struct *mod);

int load_user_info(int number, struct user_data *user_ptr);
int load_info_by_class(char *name, struct user_data *user_ptr);
int load_info_by_class_number(int number, struct user_data *user_ptr);

void set_bit(unsigned char *privs,int bit,int state);
int test_bit(unsigned char *privs, int bit);

void update_members_list(void);
int load_access_of_class(char *class_name,struct u_parameters *user,int portnum);


/* extern declarations */
typedef struct user_data user_dat;
typedef struct u_parameters user_p;

extern user_dat user_lines[MAXPORTS];
extern user_p user_options[MAXPORTS];


#endif /* GT_USER_H */
