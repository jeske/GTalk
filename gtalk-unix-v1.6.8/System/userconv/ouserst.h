/* UserST.h */

#ifndef GT_OLD_USERST_H
#define GT_OLD_USERST_H

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


#define HANDLE_LEN 80
#define PASSWORD_LEN 20

#define HANDLE_SIZE HANDLE_LEN
#define PASSWORD_SIZE PASSWORD_LEN

#define CLASS_NAME_LEN 120

struct old_unique_information_struct {
    int number;
	int short enable;
	char is_class_template;

	char handle[HANDLE_LEN + 1];
	char password[PASSWORD_LEN + 1];

	unsigned char toggles[10];
	unsigned char reset_color;

	time_t expiration;
	time_t conception;
	time_t last_call;
	time_t credit;
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
};

#define MAX_NUM_PRIV_CHARS 120


struct old_user_class_information_struct
{
    a_class class_name;

    unsigned char enable_privs[MAX_NUM_PRIV_CHARS];
    unsigned char disable_privs[MAX_NUM_PRIV_CHARS];
    char staple[4];

	unsigned short int time;
	unsigned short int added_time;
	unsigned char line_out;

	int short priority;
	int short class_index;
    int short login_channel;
};

struct old_class_defined_data_struct {

	a_class class_name;
    char verbose_class_name[120];

	unsigned char privs[MAX_NUM_PRIV_CHARS];
    char staple[4];

	unsigned short int time;
	unsigned short int added_time;
	unsigned char line_out;

	int short priority;
	int short class_index;
    int short login_channel;

};


struct old_user_data
{
    struct old_unique_information_struct user_info;
    struct old_class_defined_data_struct class_info;
    struct old_user_class_information_struct class_mod_info;
    struct rl_info real_info;
};

struct old_user_data_record
{
    union {
      struct old_unique_information_struct info;
      char dummy1[512];
    } user;

    union {
    struct old_user_class_information_struct info;
    char dummy2[512];
    } class_mod;

    union {
    struct rl_info info;
    char dummy3[512];
    } real;

};

#endif /* GT_USERST_H */
