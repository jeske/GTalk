/**********************************

            User.c

 *********************************/

#ifndef _GTALK_USER_H
#define _GTALK_USER_H

#include "types.h"

#define USER_FILE "/home/dmarks/userfile"

int testbit(char *set, int bit);
void setbit(char *set, int bit, int on);
void clearset(char *set, int bits);

#define LAST_NAME_LEN 40
#define MIDDLE_NAME_LEN 40 
#define FIRST_NAME_LEN 40
#define PHONE_LEN 20
#define NUM_PHONE 5
#define ADDRESS_LEN 80
#define NUM_ADDR 3
#define CITY_LEN 40
#define STATE_LEN 5
#define ZIP_LEN 15

typedef struct _real_information
{
  char                        first_name[FIRST_NAME_LEN+1];     /* 41 */
  char                        middle_name[MIDDLE_NAME_LEN+1];   /* 41 */
  char                        last_name[LAST_NAME_LEN+1];       /* 41 */
  char                        phone[PHONE_LEN+1][NUM_PHONE];    /* 105 */
  char                        address[ADDRESS_LEN+1][NUM_ADDR]; /* 243 */
  char                        city[CITY_LEN+1];                 /* 41 */
  char                        state[STATE_LEN+1];               /* 6 */
  char                        zip[ZIP_LEN+1];                   /* 16 */
  char                        filler[490];                      
} real_information;                                             /* 1024 */

#define LOGIN_LEN 20
#define USER_NAME_LEN 80
#define NUM_PRIVS 256
#define NUM_OPTIONS 256
#define HANDLE_LEN 80
#define CLASS_NAME_LEN 20
#define PASSWORD_LEN 20

typedef struct _user_perm
{
  g_uint32                    user_no;                          /* 4 */
  char                        login[LOGIN_LEN+1];               /* 21 */
  char                        password[PASSWORD_LEN+1];         /* 21 */
  g_uint32                    password_changed;                 /* 4 */
  char                        class_name[CLASS_NAME_LEN+1];     /* 21 */
  char                        handle[HANDLE_LEN+1];             /* 81 */
  char                        user_name[USER_NAME_LEN+1];       /* 81 */
  char                        privs[(NUM_PRIVS+7)/8];           /* 32 */
  char                        class_privs[(NUM_PRIVS+7)/8];     /* 32 */
  char                        options[(NUM_OPTIONS+7)/8];       /* 32 */
  char                        class_options[(NUM_OPTIONS+7)/8]; /* 32 */
  g_uint32                    priority;                         /* 4 */
  char                        class_priority;                   /* 1 */
} user_perm;
                                                                /* 366 */
typedef struct _user_record_struct
{
  union {
  struct _user_perm           user_data;                       
  char                        filler[1024];                    
  } user_section;
  union {
  struct _real_information    real_info;                       
  char                        filler2[3072];                   
  } real_section;
} user_record_struct;

typedef struct _user_struct
{
  struct _user_perm           user_data;                        /* 366 */
  struct _real_information    real_info;                        /* 1024 */
} user_struct;

                                                                /* 4096 */
#endif   /* _GTALK_USER_H */










