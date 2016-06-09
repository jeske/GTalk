/**********************************

            User.c

 *********************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/file.h>

#include "user.h"

int findFlagNumber(char *flag_name)
{

}

void setFlag(user_struct *user, char *flag_name, int setting)
{
  int number = findFlagNumber(flag_name);

}

int testFlag(user_struct *user, char *flag_name)
{
  int number = findFlagNumber(flag_name);

}


void setbit(char *set, int bit, int on)
{
  if (on)
    set[bit >> 3] |= (1 << (bit & 0x07));
  else
    set[bit >> 3] &= ~(1 << (bit & 0x07));
}

int testbit(char *set, int bit)
{
  return (set[bit >> 3] & (1 << (bit & 0x07)));
}

void clearset(char *set, int bits)
{
  bits = (bits + 7) >> 3;
  while (bits > 0)
    {
      *set++ = '\000';
      bits--;
    }
}

int read_user_record(int user_no, user_struct *user_str)
{
  int fd;
  user_record_struct read_rec;

  if ((fd=open(USER_FILE, O_RDONLY)) < 0)
    return (-1);
  if (flock(fd, LOCK_SH) < 0)
    {
      close(fd);
      return (-1);
    }
  if (lseek(fd, sizeof(user_record_struct) * user_no, SEEK_SET) < 0)
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  if (read(fd, read_rec, sizeof(user_record_struct)) < sizeof(user_record_struct))
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  flock(fd, LOCK_UN);
  close(fd);
  user_str->user_data = read_rec.user_section.user_data;
  user_str->real_info = read_rec.real_section.real_info;
  
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

int save_user_record(int user_no, user_struct *user_str)
{
  int fd;
  g_int32 offset;
  g_int32 target;
  user_record_struct write_rec;

  write_rec.user_section.user_data = user_str->user_data;
  write_rec.real_section.real_info = user_str->real_info;

  if ((fd=open(USER_FILE, O_RDWR)) < 0)
    return (-1);
  if (flock(fd, LOCK_EX) < 0)
    {
      close(fd);
      return(-1);
    }
  target = sizeof(user_record_struct) * user_no;
  if ((offset=lseek(fd, target, SEEK_END)) < 0)
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
  if (write(fd, write_rec, sizeof(user_record_struct)) < sizeof(user_record_struct))
    {
      flock(fd, LOCK_UN);
      close(fd);
      return (-1);
    }
  flock(fd, LOCK_UN);
  close(fd);
  return (0);
}


