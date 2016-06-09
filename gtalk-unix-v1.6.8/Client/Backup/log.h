


/**********************************

              LOG.H

***********************************/

#ifndef _LOG_GT_H
#define _LOG_GT_H

#define LOG_FILENAME "log/errorlog.cli"

void log_error(char *format, ...);
int log_event(char *filename,char *event,...);
int log_system_event(char *filename, char *event,...);

#define log_system_event_for_user(a,b,c) log_user_event((a),(c),(b))

#endif  /* _LOG_GT_H */
