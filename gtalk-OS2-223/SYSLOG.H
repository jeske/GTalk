/* SYSLOG.H */

#ifndef GT_SYSLOG_H
#define GT_SYSLOG_H

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#define STRFTIME_CONFIG_STRING "(%a) %m/%d/%y %H:%M:%S"

void log_kill(int node,int who_killed);
void last_ten_kills(int num_last,int portnum);
void mark_user_log_on(void);
void log_error(const char *filename);
void log_event(const char *filename,char *event);

#endif /* GT_SYSLOG_H */
