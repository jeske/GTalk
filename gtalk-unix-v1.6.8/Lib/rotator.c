
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - rotator.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/file.h>

#include "types.h"
#include "str.h"
#include "list.h"
#include "abuf.h"
#include "states.h"
#include "common.h"
#include "usercommon.h"
#include "rotator.h"

void rotator_set_bit(char *array, int bit, int val)
{
  if (val)
    array[bit >> 3] |= 1 << (bit & 0x07);
  else
    array[bit >> 3] &= ~(1 << (bit & 0x07));
}

char *rotator_entry_name(int rec)
{
  static char ren[200];

  sprintf(ren, ROTATOR_FILE_NAME, rec);
  return (ren);
}

int rotator_test_bit(char *array, int bit)
{
  return ((array[bit >> 3] & (1 << (bit & 0x07))) != 0);
}

int open_rotator_file(rotator_file *rfl, int rdonly)
{
  char *c;
  unsigned long int num;

  if (!(rfl->fp=fopen(ROTATOR_INDEX_FILE, rdonly ? "rb" : "rb+")))
    return (-1);
  if (!rdonly)
    flock(fileno(rfl->fp), LOCK_EX);
  else
    flock(fileno(rfl->fp), LOCK_SH);
  if (fread(&rfl->rfh, sizeof(rotator_file_header), 1, rfl->fp) < 1)
    {
      fclose(rfl->fp);
      return (-1);
    }
  c = rfl->rfh.rotator_file_length;
  if (!get_number(&c, &num))
    rfl->num_entries = 0;
  else
    rfl->num_entries = num;
  rfl->rdonly = rdonly;
  return (0);
}

int save_rotator_file(rotator_file *rfl)
{
  if (!rfl->rdonly)
    {
      sprintf(rfl->rfh.rotator_file_length, "%d\n", rfl->num_entries);
      fseek(rfl->fp, 0, SEEK_SET);
      fwrite(&rfl->rfh, sizeof(rotator_file_header), 1, rfl->fp);
    }
  flock(fileno(rfl->fp), LOCK_UN);
  fclose(rfl->fp);
  return (0);
}

int new_rotator_file_entry(int rec, rotator_file_entry *rie)
{
  rie->entry_num = rec;
  rie->active = 0;
  rie->usr_number = 0;
  rie->should_rotate = 0;
  rie->max_length = 256;
  strcpy(rie->name,"<Untitled>");
  return (0);
}

int read_rotator_file_entry(rotator_file *rfl, int rec, 
			    rotator_file_entry *rie)
{
  if (fseek(rfl->fp, rentry_offset(rec), SEEK_SET) < 0)
    return (-1);

  if (fread(rie, sizeof(rotator_file_entry), 1, rfl->fp) < 0)
    return (-1);
  return (0);
}
  
int save_rotator_file_entry(rotator_file *rfl, int rec,
			    rotator_file_entry *rie)
{
  unsigned long int end_offset, cur_offset, l;
  char buffer[512];

  if (rfl->rdonly)
    return (-1);
  rotator_set_bit(rfl->rfh.bit_array, rec, rie->should_rotate);
  end_offset = rentry_offset(rec);
  fseek(rfl->fp, 0, SEEK_END);
  memset(buffer, 0, sizeof(buffer));
  for (;;)
    {
      if ((cur_offset = ftell(rfl->fp)) >= end_offset)
	break;
      l = end_offset - cur_offset;
      if (l > sizeof(buffer))
	l = sizeof(buffer);
      fwrite(buffer, 1, sizeof(buffer), rfl->fp);
    }
  fseek(rfl->fp, rentry_offset(rec), SEEK_SET);
  fwrite(rie, sizeof(rotator_file_entry), 1, rfl->fp);
  if (rec > rfl->num_entries)
    rec = rfl->num_entries;
  return (0);
}

int load_rotator_entry(int num, rotator_file_entry *entry)
{



}

