

/**********************************

              LOG.H

***********************************/

#ifndef _LOG_GT_H
#define _LOG_GT_H

#define LOG_FILENAME "log/errorlog.srv"

void log_error(char *format, ...);
int log_event(char *filename,char *event,...);

#endif  /* _LOG_GT_H */
