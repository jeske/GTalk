/* GTST.H */

#ifndef GT_GTST_H
#define GT_GTST_H

#include "userst.h"

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


#include "protocol.h"

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


 struct call_statistics
 {
   unsigned long int total;
   unsigned long int baud[10];
 };

struct login_checks_struct {
    int (*check_login)(int portnum);
};


 struct system_toggles
 {
   char should_reboot;
   long int checksum;
   char shutdown_on_checksum_failure;
   int  perodic_checksum_task_id;
   char checksum_failed;
   char  timeout_flag;
   char shutdown_system;
   unsigned long int total_starting_memory;
   unsigned long int total_dos_starting_memory;
   char system_update;
   char num_dial_ins;
   char is_validated;
   char logins_disabled;
   unsigned char calls_updated;
   char system_booting;

 };


 struct system_information
 {
   short int lock_priority;
   char user_edit_password[40];
   char shutdown_password[40];
   short int max_channels;
   short int max_nodes;
   struct call_statistics calls;
   struct call_statistics day_calls;
   short int paging;
   char system_name[40];
   char checksum_task;
   time_t uptime;
   time_t last_uptime;
   char command_toggle_password[40];
   char master_password[40];
   char system_number;
   char page_console_password[40];
   struct call_statistics record_calls;
   struct call_statistics yesterday_calls;
   time_t current_time;
   time_t down_time;
   unsigned long int call_back_delay;
   char guest_login_channel;
   struct call_statistics month_calls;
   struct call_statistics last_month_calls;
   char this_month_number;
   char last_month_number;
   char sys_phone[USER_PHONE_LEN];
   short int message_mode;
   char exception_debug_level;
   char reboot_on_all_exceptions;
   int num_exceptions_trapped;
   int last_num_exceptions_trapped;
 };


 struct line_outs
   {
    short int warnings_left;
    short int lines_typed;
    short int lines_allowed;
   } ;

struct link_stuff
{
   char send_sp_now;
   char repeat_sp_lists;
   char auto_sp_minutes;
   char can_see_guests;
};

struct vt100_conv_type
 {
   char *vt100_key_string;
   int key_code_len;
   char *vt100_alternate_string;
   int alt_code_len;
   unsigned char keycode;
   short scan_code;
   short extra_keys;
 };

 struct vt100_escape_type
 {
   short num_keycodes;
   char keycodes[KEYCODE_SIZE_MAX+1];
 };

 struct ln_type
 {
   struct line_outs lo;

   unsigned char numchannels;
   unsigned char channels[MAX_MONITOR];
   unsigned char mainchannel;

   signed char line_lock[MAX_CHANNEL_ITEMS];

   unsigned char timeout;
   unsigned char should_timeout;

   unsigned char ansi;
   unsigned char full_screen_ansi;

   unsigned short int width;

   char baud[10];

   unsigned char handlelinechanged;
   char handleline[100];

   short int online;
   short int connect;
   short int restart;
   time_t time_online;

   int chat_with;
   time_t chat_issued_at;
   char wants_to_chat;
   char ready_to_chat;

   short int watcher;
   short int paging;
   short int lurking;
   short int safe_lurking;

   short int silenced;

   void *call_on_exit;
   void *call_on_exit_data;
   char no_fucking;


   char link;
   unsigned short int slowdown_value;
   struct link_stuff link_info;
   struct caller_id_struct  *call_info;

   /* transfer stuff */
   struct transfer_struct *transfer;
   short int transfer_pid;
   char feedbacks;
   char apps;

   struct vt100_escape_type vtkey;

   char num_feedbacks_allowed;
 } ;

struct sync_number_storage
{
  unsigned char calls_updated;
  char handlelinechanged[5];
  unsigned short int handlelinechanged_at_tick;
  unsigned short int last_screen_redraw;

};

// if MAXPORTS grows > 32, fix the number above.

struct abuf_type
 {
   unsigned short int used;
   unsigned short int active;
   char *abuffer;
   char *abufend;
   char *abufread;
   char *abufwrite;
   short int  num_buffer;
   short int  max_buffer;
 } ;


 #endif /* GT_GTST_H */
