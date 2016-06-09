/* UserST.h */

#ifndef GT_USERST_H
#define GT_USERST_H


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


#define HANDLE_SIZE 41
#define PASSWORD_SIZE 11


struct kl_stats
{
    unsigned int short kills_day;
    unsigned int short kills_month;
    unsigned int short kills_total;
    unsigned int short slow_kills_day;
    unsigned int short slow_kills_month;
    unsigned int short slow_kills_total;


    /* KILL STUFF */

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



struct rl_info
{

    char name[51];
    char street[51];
    char city[13];
    char state[3];
    char zip[10];
    char phone[11];
    char phone2[11];

};

struct exp_date
{
   char day;
   char month;
   unsigned short int year;

};

struct user_data
{
    int short number;
    char handle[HANDLE_SIZE];
    char password[PASSWORD_SIZE];

    unsigned char privs[10];
    unsigned char toggles[10];
    time_t expiration;
    time_t conception;
    time_t last_call;
    unsigned short int time;
    unsigned short int added_time;
    char staple[4];
    int short enable;
    int short priority;
    int num_calls;
    struct kl_stats killstats;
    struct rl_info real_info;
    struct other_stats stats;
    unsigned char width;
    unsigned char line_out;
    unsigned char num_eat_lines;
    struct exp_date birth_date;
    time_t credit;
    time_t starting_date;
    short int priv_give_limit;
    char no_fuck_with;
    struct kl_stats killedstats;
    unsigned char reset_color;
    unsigned char user_type;
    char dummy[68];
} ;





#endif /* GT_USERST_H */
