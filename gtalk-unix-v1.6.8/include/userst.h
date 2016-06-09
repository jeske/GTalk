/* UserST.h */

#ifndef GT_USERST_H
#define GT_USERST_H

#include "types.h"

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


#define HANDLE_LEN 80
#define GUEST_HANDLE_LEN 25
#define PASSWORD_LEN 20

#define HANDLE_SIZE HANDLE_LEN
#define GUEST_HANDLE_SIZE GUEST_HANDLE_LEN
#define PASSWORD_SIZE PASSWORD_LEN

#define CLASS_NAME_LEN 120

#define LOGIN_LEN 20
#define USER_NAME_LEN 80

typedef char a_class[CLASS_NAME_LEN];

struct time_rel_struct {
    time_t    time_unit;
    char      time_flags[6];
    time_t    calcd_at;
    time_t    effective_time;
};

struct auto_class_movement_struct {
    struct      time_rel_struct at_time;
    a_class     to_class;
};


#ifndef GT_USERST_COMMON_H
#define GT_USERST_COMMON_H

struct kl_stats
{
    /* KILL STUFF */

    unsigned int short kills_day;
    unsigned int short kills_month;
    unsigned int short kills_total;
    unsigned int short slow_kills_day;
    unsigned int short slow_kills_month;
    unsigned int short slow_kills_total;
};

struct other_stats
{
     unsigned int calls_total;
     unsigned int time_total;
     unsigned int short calls_month;
     unsigned int short time_month;
     unsigned int short calls_day;
     unsigned int short time_day;

     unsigned int short num_validates;
     unsigned int short give_times;

};


struct exp_date
{
   char day;
   char month;
   unsigned short int year;

};


#endif /* common */





#define USER_REAL_NAME_LEN 80
#define USER_STREET_LEN 120
#define USER_CITY_LEN   120
#define USER_STATE_LEN  80
#define USER_POSTAL_CODE_LEN    25
#define USER_PHONE_LEN  20
#define USER_COUNTRY_LEN 20
#define TERMTYPE_LEN 20

struct rl_info
{
    char name[USER_REAL_NAME_LEN + 1];
    char street[USER_STREET_LEN + 1];
    char city[USER_CITY_LEN + 1];
    char state_or_province[USER_STATE_LEN + 1];
    char country[USER_COUNTRY_LEN + 1];
    char postal_code[USER_POSTAL_CODE_LEN + 1];
    char phone[USER_PHONE_LEN + 1];
    char phone2[USER_PHONE_LEN + 1];
    struct exp_date birth_date;
};

#define USER_NOT_VALIDATED          0
#define USER_CALLER_ID_VALIDATED    1
#define USER_VOICE_VALIDATED        2


struct unique_information_struct {
  g_uint32                    user_no;                          /* 4 */
  char                        login[LOGIN_LEN+1];               /* 21 */
  char                        password[PASSWORD_LEN+1];         /* 21 */
  g_uint32                    password_changed;                 /* 4 */
  g_uint32			unix_userid;
  
  int short enable;
  char is_class_template;
  
  char handle[HANDLE_LEN + 1];
  
  unsigned char toggles[10];
  unsigned char reset_color;
  
  time_t expiration;
  time_t conception;
  time_t last_call;
  int    account_balance;
  time_t starting_date;
  
  struct kl_stats killstats;
  struct kl_stats killedstats;
  struct other_stats stats;
  
  unsigned char width;
  unsigned char num_eat_lines;
  short int created_by;
  short int validated_by;
  short int login_channel;
  short int message_mode;
  char validate_info;
  a_class class_name;
  char termtype[TERMTYPE_LEN];

  /* new accounting system stuff */

  time_t last_wage_payment_date;         /* last date wage was paid to user */
  int free_credits;                      /* reset every month */
  int credit_card_balance;               /* credit card balance */
};

#define MAX_NUM_PRIV_CHARS 120
#define NUM_PRIVS ((MAX_NUM_PRIV_CHARS)*8)

struct user_class_information_struct
{
  unsigned char enable_privs[MAX_NUM_PRIV_CHARS];
  unsigned char disable_privs[MAX_NUM_PRIV_CHARS];
  
  unsigned short int time;
  unsigned short int added_time;
  unsigned char line_out;
  
  int short priority;
  int short class_index;
};

struct class_defined_data_struct {
  
  a_class class_name;
  char verbose_class_name[120];
  
  unsigned char privs[MAX_NUM_PRIV_CHARS];
  char staple[4];
  
  unsigned short int time;
  unsigned short int added_time;
  unsigned char line_out;
  
  int short priority;
  int short class_index;
  
  /* for new credit system */
  int account_overdraft_limit;             /* limit for personal overdraft */
  int credit_card_limit;                   /* limit for sysop credit card */
  int monthly_wage;                        /* monthly wage for class */
  int monthly_free_credits;                /* number of free credits
                                              sysop can give away per month */

  int class_cost_per_month;                
  int class_cost_per_year;
  int class_cost_per_day;
  a_class unpaid_class;                    /* when unpaid, load this */
};

struct class_movement_data_struct
{
	a_class paid_class_move;
	struct auto_class_movement_struct promote_info;
	struct auto_class_movement_struct demote_info;
};




struct user_data
{
    struct unique_information_struct user_info;
    struct user_class_information_struct class_mod_info;
    struct rl_info real_info;
};

struct user_data_record
{
    union {
      struct unique_information_struct info;
      char dummy1[512];
    } user;

    union {
    struct user_class_information_struct info;
    char dummy2[512];
    } class_mod;

    union {
    struct rl_info info;
    char dummy3[512];
    } real;

};

struct class_data
{
  struct class_defined_data_struct class_info;
  struct class_movement_data_struct move_info;
};

struct class_data_record
{
    union {
    struct class_defined_data_struct info;
    char dummy1[2048];
    } class;

    union {
    struct class_movement_data_struct info;
    char dummy2[512];
    } move;
};


#define USER_START_BLOCK_SIZE 2048

struct user_data_start_block_struct {

    /* this structure must not exceed the size
       of the "USER_START_BLOCK_SIZE" */

    unsigned long int num_users;
    unsigned short int record_length;

};

union user_data_start_block {
    struct user_data_start_block_struct data;
    char   dummy_space[USER_START_BLOCK_SIZE];
};

#endif /* GT_USERST_H */
