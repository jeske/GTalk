/* USERST2.H */

#ifndef GT_USERST2_H
#define GT_USERST2_H

#include "userst.h"
#include <sys/types.h>
#include <pwd.h>


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#define LOCATION_LEN 30
#define CID_DATE_LEN  4
#define CID_TIME_LEN  4
#define CID_NUM_LEN   14
#define CID_NAME_LEN  50
#define CID_MESSAGE_LEN 120
#define MODEM_STRING_LEN 80

#define KEYCODE_SIZE_MAX 5

struct caller_id_struct {
    char num_rings;
    char date[CID_DATE_LEN+1];
    char time[CID_TIME_LEN+1];
    char number[CID_NUM_LEN+1];
    char name[CID_NAME_LEN+1];
    char message[CID_MESSAGE_LEN+1];
    char modem_connect_string[MODEM_STRING_LEN];
};


struct u_parameters
{
  int add_time_counter;
  unsigned char toggles[10];
  unsigned char system[10];
  char noansi_handle[50];
  unsigned long int time_sec;           /* time online */
  unsigned long int time_warning_sec;   /* time until warning */
  char warning_prefix;
  time_t login_time;
  int warnings;
  char location[LOCATION_LEN+1];
  unsigned char width;
  int logged_in_flag;
  struct passwd unix_passinfo;
  char newuser_apps;                    /* new user application count */
  struct caller_id_struct *call_info;                       /* callerid call information available? */
  struct class_defined_data_struct class_info;
};

struct _user_perm 
{
  struct u_parameters online_info;
  struct unique_information_struct user_info;
};

#endif /* GT_USERST2_H */













