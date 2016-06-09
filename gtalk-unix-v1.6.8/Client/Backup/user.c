
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - user.c
 *
 * This contains the code for reading and writing to the userfile as 
 * well as general use routines for handling security checks and sets.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/file.h>
#include "log.h"

#include <stdarg.h>

#include "user.h"
#include "common.h"
#include "usercommon.h"

int file_log_message(char *filename,char *format, va_list ap);

void setFlag(node_struct *node, char *flag_name, int setting)
{
  int number = findFlagNumber(flag_name);

  if (number<0)
    {
      log_error("Flag not found: %s",flag_name);
      printf_ansi("[GTERR: %s not found]",flag_name);
      return;
    }

  setbit(node->userdata.online_info.class_info.privs,number,setting);
}

void list_classes(void)
{
  struct class_data tempclass;
  int index = 0;

  while (!read_class_record(index,&tempclass))
    {
      if ((tempclass.class_info.class_name[0]!=0) &&
	  (tempclass.class_info.class_index == index))
      printf_ansi("CLASS: %s\n",tempclass.class_info.class_name);
      index++;
    }

}

int read_class_record(int class_no, struct class_data *class_str)
{
  int fd;
  struct class_data_record read_rec;
  struct user_data_start_block_struct start_rec;

  if ((fd=open(CLASS_FILE, O_RDONLY)) < 0)
    return (-1);
  if (flock(fd, LOCK_SH) < 0)
    {
      close(fd);
      return (-1);
    }
  if (lseek(fd, (0), SEEK_SET) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  if (read(fd, &start_rec, sizeof(struct user_data_start_block_struct)) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return(-1);
    }

  if (lseek(fd, (sizeof(struct class_data_record) * class_no) + 
	    USER_START_BLOCK_SIZE, SEEK_SET) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  if (read(fd, &read_rec, sizeof(struct class_data_record)) < 
      sizeof(struct class_data_record))
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  flock(fd, LOCK_UN);
  close(fd);
  
  class_str->class_info = read_rec.class.info;
  class_str->class_info.class_index = class_no;
  class_str->move_info = read_rec.move.info;
  return (0);
}


int save_class_record(int class_no, struct class_data *class_str)
{
  int fd;
  g_int32 offset;
  g_int32 target;
  struct class_data_record write_rec;
  struct user_data_start_block_struct start_rec;

  class_str->class_info.class_index = class_no;

  write_rec.class.info = class_str->class_info;
  write_rec.move.info = class_str->move_info;
  

  if ((fd=open(CLASS_FILE, O_RDWR)) < 0)
    return (-1);
  if (flock(fd, LOCK_EX) < 0)
    {
      close(fd);
      return(-1);
    }

 if (lseek(fd, (0), SEEK_SET) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  if (read(fd, &start_rec, sizeof(struct user_data_start_block_struct)) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return(-1);
    }

  target = (sizeof(struct class_data_record) * class_no) + 
    USER_START_BLOCK_SIZE;

  if ((offset=lseek(fd, 0, SEEK_END)) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  if (offset < target)
    if (write_zeros(fd, target - offset) < 0)
      {
	flock(fd, LOCK_UN);
	close(fd);
	return (-1);
      }
  if (lseek(fd, target, SEEK_SET) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }  

  if (write(fd, &write_rec, sizeof(struct class_data_record)) < 
      sizeof(struct class_data_record))
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  flock(fd, LOCK_UN);
  close(fd);
  return (0);
}


int read_class_by_name(char *string, struct class_data *cptr)
{
  struct class_data tempclass;
  int index = 0;

  while (!read_class_record(index,&tempclass))
    {
      if (!strcmp(tempclass.class_info.class_name,string))
	{
	  *cptr = tempclass;
	  return 0;
	}
      index++;
    }
  return -1;
}



int class_exists(char *c_name)
{
  struct class_data temp_class;

  if (read_class_by_name(c_name,&temp_class))
    {
      return 0; /* NO, it dosn't exist */
    }
  else
    {
      return 1; /* YES, it exists */
    }
}

int save_class_by_name(char *string, struct class_data *cptr)
{
  struct class_data tempclass;
  
  if (read_class_by_name(string,&tempclass))
    {
      /* it dosn't already exist so create it */
      int index = 0;
      
      while (!read_class_record(index,&tempclass))
	{
	  if (tempclass.class_info.class_name[0]==0)
	    {
	      tempclass = *cptr;
	      return (save_class_record(index,&tempclass));
	    }

	  index++;
	} 

      tempclass = *cptr;
      return (save_class_record(index,&tempclass));
    }
  else
    {
      int index = 0;
      
      /* it already exists, so just save*/
      
      index = tempclass.class_info.class_index;
      tempclass = *cptr;
      return (save_class_record(index,&tempclass));
    }

  return (-1);
}


int read_user_record(int user_no, struct user_data *user_str)
{
  int fd;
  struct user_data_record read_rec;
  struct user_data_start_block_struct start_rec;

  if (!user_str)
    return (-1);

  bzero(user_str,sizeof(*user_str)); /* clean out the structure */

  if ((fd=open(USER_FILE, O_RDONLY)) < 0)
    return (-1);
  if (flock(fd, LOCK_SH) < 0)
    {
      close(fd);
      return (-1);
    }
  if (lseek(fd, (0), SEEK_SET) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  if (read(fd, &start_rec, sizeof(struct user_data_start_block_struct)) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return(-1);
    }

  if (lseek(fd, (sizeof(struct user_data_record) * user_no) + 
	    USER_START_BLOCK_SIZE, SEEK_SET) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  if (read(fd, &read_rec, sizeof(struct user_data_record)) < 
      sizeof(struct user_data_record))
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  flock(fd, LOCK_UN);
  close(fd);
  user_str->user_info = read_rec.user.info;
  user_str->real_info = read_rec.real.info;
  
  return (0);
}

int write_zeros(int fd, g_uint32 zeros)
{
  char buffer[512];
  int nwrite;

  memset(buffer, 0, sizeof(buffer));
  while (zeros > 0)
    {
      nwrite = (zeros > sizeof(buffer)) ? sizeof(buffer) : zeros;
      if (write(fd, buffer, nwrite) < nwrite)
	return (-1);
      zeros -= nwrite;
    }
  return (0);
}

int save_user_record(int user_no, struct user_data *user_str)
{
  int fd;
  g_int32 offset;
  g_int32 target;
  struct user_data_record write_rec;
  struct user_data_start_block_struct start_rec;

  bzero(&write_rec,sizeof(write_rec));
  write_rec.user.info = user_str->user_info;
  write_rec.real.info = user_str->real_info;

  if ((fd=open(USER_FILE, O_RDWR)) < 0)
    return (-1);
  if (flock(fd, LOCK_EX) < 0)
    {
      close(fd);
      return(-1);
    }

 if (lseek(fd, (0), SEEK_SET) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  if (read(fd, &start_rec, sizeof(struct user_data_start_block_struct)) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return(-1);
    }

 

  target = (sizeof(struct user_data_record) * user_no) + USER_START_BLOCK_SIZE;

  if ((offset=lseek(fd, 0, SEEK_END)) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  if (offset < target)
    if (write_zeros(fd, target - offset) < 0)
      {
	flock(fd, LOCK_UN);
	close(fd);
	return (-1);
      }
  if (lseek(fd, target, SEEK_SET) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }  

  if (write(fd, &write_rec, sizeof(struct user_data_record)) < 
      sizeof(struct user_data_record))
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  flock(fd, LOCK_UN);
  close(fd);
  return (0);
}


void delete_files_function(char *dir, char *files)
{
  printf_ansi("Should delete files in dir [%s] with mask [%s]\n",dir,files);

}

void make_user_dir(int number)
{
    char s[256];

    sprintf(s,"USER/USER%03d",number);

    if (mkdir(s))
    {
      if (mkdir("USER"))
       {
         /* the directory is already there so clean it out */
         delete_files_function(s,"*");

       }
      else
      { /* it was not already there so now make the new part */

        if (mkdir(s))
          log_error("* Unable to create user home directory!!");
      }

    }

    /*
     *   Directories are made/cleaned in the "USER" tree
     */

}

int log_user_event(char *filename,int number,char *event,...)
{
  va_list ap;
  char s[256];

  sprintf(s,"USER/USER%03d/%s",number,filename);
  
  va_start(ap,event);
  if (file_log_message(s,event,ap))
    {
	log_error("could not log user event");
    }
  va_end(ap);
}

/*
 * this routine should clean out all directories and data files which
 * have to do with a user
 */

void prep_user_dirs(int number)
{
    char s[256];

    sprintf(s,"USER/USER%03d",number);

    if (mkdir(s))
    {
      if (mkdir("USER"))
       {
         /* the directory is already there so clean it out */
         delete_files_function(s,"*");

       }
      else
      { /* it was not already there so now make the new part */

        if (mkdir(s))
          log_error("* Unable to create user home directory!!");
      }

    }

    /*
     *   Directories are made/cleaned in the "USER" tree
     */


    sprintf(s,"USER/USER%03d/MAIL",number);

    if (mkdir(s))
    {
      if (mkdir("USER"))
       {
         /* the directory is already there so clean it out */
         delete_files_function(s,"*");

       }
      else
      { /* it was not already there so now make the new part */

        if (mkdir(s))
          log_error("* Unable to create user mail directory!!");
      }

    }

    /* ok, the mail dirs are clean */

    log_system_event_for_user("AUDIT.LOG","User directory Cleaned",number);
}
