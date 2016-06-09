/* FILES.H */

#ifndef GT_FILES_H
#define GT_FILES_H


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



#define USER_LOG_FILE "LOG\\USER.LOG"
#define USER_ENTER_LOG_FILE "LOG\\ENTER.LOG"

void file_section(char *str,char *name,int portnum);
void close_file_list(void *file_ptr);
void log_user_is_leaving(int portnum,const char *log_file);
void close_last_ten_callers(void *fileptr);
void last_ten_callers(char *string, char *name, int portnum);

#endif /* GT_FILES_H */
