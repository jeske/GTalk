/* GT.H */

#ifndef GT_GT_H
#define GT_GT_H

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



#include "gtst.h"

#define CLIENT_BUFFER 2048
#define CLIENT_BUFFER_1 2047
#define SERVER_BUFFER 8192
#define SERVER_BUFFER_1 8191
#define STRING_SIZE 450

// LOWER numbers mean less delay
#define CONSOLE_PRESTATUS_DELAY 45

// LOWER number mean FASTER updates
#define CONSOLE_STATUS_UPDATE 35

/***************
 ***  GT.C   ***
 ***************/
 


 
 /* extern declarations */
extern char backspacestring[4];     /* sent with a backspace */
extern struct abuf_type abuf_status[MAX_THREADS];
extern struct ln_type line_status[MAX_THREADS];
extern struct sync_number_storage sync_status[MAX_THREADS];
extern int server;
extern int timeout_server;
extern char cr_lf[];
extern struct system_information sys_info;
extern struct system_toggles sys_toggles;


extern const char version_title[];    /* version message */

void calc_time_for_node(int portnum);

void hangup_user(int portnum);
void log_out_user(int portnum);

void show_log_off(int portnum);
void show_log_in(int portnum);

void filter_ansi(char *str,char *newstr);
void filter_flashing(char *str,char *newstr);

void leave(void);
int ansi_strlen(char *str);
void delay(unsigned int ticks);
void set_temp_user_info(int portnum);
void string_cr(char *out);
void string_out(char *out);
void char_out(char out);
void get_string(char *string,int limit);
void ginsu(void);
void relogged(void);
void linked(void);
void str_time(char *str, int legnth,struct tm *now);
void leave(void);
int pass_prompt(void);
void start_server(void);
void start_timeout_server(void);
void print_log_in(int portnum);
void print_log_off(int portnum);
void initrestart(void);
void log_off(int portnum,int should_unlog);
int has_channel(int portnum, int channel);
void re_log(int portnum);
void remote(int portnum);
void get_result_code(char *string, int length);
void sendslow(int port_num, char *string);
void init_login_vars(int portnum);
void shutdown_server(void);
void shutdown_timeout_server(void);
void clear_call_on_logoff();
void call_on_logoff(void *function, void *function_data);
void broadcast_message(char *string);
void set_death_off(void);
void set_death_on(void);
void wait_for_death(int portnum);
void save_sys_info(void);
int nodes_online(void);
void shutdown_task(void);
void shutdown_node(int portnum);
void unlog_user(int portnum);
void clear_call_on_logoff(void);
void save_channel_info_function(void);

int remake_handleline(void);

void save_sys_info_function(void);

void update_display_one(int initial_draw);

#endif /* GT_GT_H */
