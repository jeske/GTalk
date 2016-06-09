



/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* define.h */

/* software settings : */


#ifndef DEFINE_HEADER
#define DEFINE_HEADER

#define SOFTWARE_VERSION_NUMBER (10)
#undef  CONSOLE



#define CHANNEL_CONFIG_FILE "CHANNEL.CFG"
#define SYSTEM_CONFIG_FILE "SYSTEM.CFG"
#define SYSOP_LOG_FILE     "LOG\\SYSOP.LOG"
#define PASSWORD_LOG_FILE  "LOG\\PASS.LOG"
#define XFER_LOG_FILE      "LOG\\XFER.LOG"
#define ACCOUNT_LOG_FILE   "LOG\\ACCT.LOG"
#define USER_LOG_FILE      "LOG\\USER.LOG"


#ifdef CONSOLE
#define VERSION_NO (SOFTWARE_VERSION_NUMBER | 0x80)
#else
#define VERSION_NO (SOFTWARE_VERSION_NUMBER)
#endif

#define ALL_BITS_SET     0xFF
#define ALL_BITS_CLEARED 0x00
#define HANDLELINE_SPRINTF 0
#define CONSOLE_REWRITE    1




/* CHANNEL.H */

#define MAX_MONITOR 10
#define MAX_STORE_CHANNELS 10
#define MAX_CHANNEL_ITEMS 8
#define MAX_CHANNELS 15

/* TASK.H */

#define MAX_THREADS 26

/* MODULE.H */

#define GLM_INFO_LINE_LEN 20



#endif
