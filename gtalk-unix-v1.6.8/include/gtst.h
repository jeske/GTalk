/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#ifndef GT_GTST_H
#define GT_GTST_H

#define SYSTEM_PHONE_LEN 15
 struct sys_toggles_struct
 {
 };

struct call_statistics {
	int total;
};

 struct sys_info_struct
 {
   short int lock_priority;
   short int lock_priority_telnet;
   char system_name[40];
   char system_number;
   char sys_phone[SYSTEM_PHONE_LEN];

   time_t uptime;
   time_t last_uptime;
   time_t down_time;

   struct call_statistics record_calls;
   struct call_statistics yesterday_calls;
   struct call_statistics calls;
   struct call_statistics day_calls;
   struct call_statistics month_calls;
   struct call_statistics last_month_calls;

   unsigned long int call_back_delay;

   char this_month_number;
   char last_month_number;
 };




 #endif /* GT_GTST_H */
