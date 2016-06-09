

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* GTST.H */


 struct call_statistics
 {
   unsigned long int total;
   unsigned long int baud[10];
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
 };


 struct system_information
 {
   int lock_priority;
   char user_edit_password[40];
   char shutdown_password[40];
   int max_channels;
   int max_nodes;
   struct call_statistics calls;
   struct call_statistics day_calls;
   int paging;
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
 };


 struct line_outs
   {
    int warnings_left;
    int lines_typed;
    int lines_allowed;
   } ;

struct link_stuff
{
   char send_sp_now;
   char repeat_sp_lists;
   char auto_sp_minutes;
   char can_see_guests;
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

   unsigned int width;

   char baud[10];

   unsigned char handlelinechanged;
   char handleline[100];

   int online;
   int connect;
   int restart;
   time_t time_online;

   int chat_with;
   time_t chat_issued_at;
   char wants_to_chat;
   char ready_to_chat;

   int watcher;
   int paging;
   int lurking;
   int safe_lurking;

   int silenced;

   void far *call_on_exit;
   void far *call_on_exit_data;
   char no_fucking;


   char link;
   unsigned int slowdown_value;
   struct link_stuff link_info;

 } ;

struct sync_number_storage
{
  unsigned char calls_updated;
  char handlelinechanged[5];
  unsigned int handlelinechanged_at_tick;
  unsigned int last_screen_redraw;

};

// if MAXPORTS grows > 32, fix the number above.

struct abuf_type
 {
   unsigned int used;
   unsigned int active;
   char far *abuffer;
   char far *abufend;
   char far *abufread;
   char far *abufwrite;
   int  num_buffer;
   int  max_buffer;
 } ;
