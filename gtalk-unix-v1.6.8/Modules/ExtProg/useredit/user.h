
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - user.h
 *
 */

#ifndef _GTALK_USER_H
#define _GTALK_USER_H

#include "../../types.h"
#include "../../userst.h"
#define USER_FILE "user.dat"
#define CLASS_FILE "class.dat"
#define GUEST_USER_NUMBER (-1)

int testbit(char *set, int bit);
void setbit(char *set, int bit, int on);
void clearset(char *set, int bits);
int read_user_record(int user_no, struct user_data *user_str);
int save_user_record(int user_no, struct user_data *user_str);
int read_class_record(int class_no, struct class_data *class_str);
int save_class_record(int class_no, struct class_data *class_str);
int read_class_by_name(char *class_name, struct class_data *class_str);

#endif   /* _GTALK_USER_H */

























