
/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* UserST.h */


#define HANDLE_SIZE 41
#define PASSWORD_SIZE 11


struct kl_stats
{
    unsigned int kills_day;
    unsigned int kills_month;
    unsigned int kills_total;
    unsigned int slow_kills_day;
    unsigned int slow_kills_month;
    unsigned int slow_kills_total;


    /* KILL STUFF */

};

struct other_stats
{
     unsigned long int calls_total;
     unsigned long int time_total;
     unsigned int calls_month;
     unsigned int time_month;
     unsigned int calls_day;
     unsigned int time_day;

     unsigned int num_validates;
     unsigned int give_times;

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
   unsigned int year;

};

struct user_data
{
    int number;
    char handle[HANDLE_SIZE];
    char password[PASSWORD_SIZE];

    unsigned char privs[10];
    unsigned char toggles[10];
    time_t expiration;
    time_t conception;
    time_t last_call;
    unsigned int time;
    unsigned int added_time;
    char staple[4];
    int enable;
    int priority;
    long int num_calls;
    struct kl_stats killstats;
    struct rl_info real_info;
    struct other_stats stats;
    unsigned char width;
    unsigned char line_out;
    unsigned char num_eat_lines;
    struct exp_date birth_date;
    time_t credit;
    time_t starting_date;
    int priv_give_limit;
    char no_fuck_with;
    struct kl_stats killedstats;
    unsigned char reset_color;
    unsigned char user_type;
    char dummy[68];
} ;




struct u_parameters
{
    int add_time_counter;
    unsigned char privs[10];
    unsigned char toggles[10];
    unsigned char system[10];
    int priority;
    char staple[4];
    char noansi_handle[50];
    unsigned int time;
    unsigned long int time_sec;           /* time online */
    unsigned long int time_warning_sec;   /* time until warning */
    char warning_prefix;
    unsigned int added_time;
    time_t login_time;
    int warnings;
    char location;
    char v_location[15];
    int squelched[MAX_THREADS];
    int squelch_all;
    unsigned char width;

} ;
