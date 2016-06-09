
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - rotator.h
 *
 */

#ifndef _GTALK_ROTATOR_H
#define _GTALK_ROTATOR_H

#include "types.h"
#include "list.h"

#define ROTATOR_FILE_DIRECTORY "/home/gtalk/ROOT/home/gtalk/rotator"
#define ROTATOR_INDEX_FILE "/home/gtalk/ROOT/home/gtalk/rotator/rotate.n"
#define ROTATOR_FILE_NAME "/home/gtalk/ROOT/home/gtalk/rotator/rot%03d"

#define ROTATOR_BIT_ARRAY_LOCATION 10
#define ROTATOR_STRUCT_LOCATION 150
#define ROTATOR_BIT_ARRAY_LENGTH 128

#define rentry_offset(en) ((((en)+1)*sizeof(rotator_file_entry))+ \
           ROTATOR_STRUCT_LOCATION)

#define MAX_ROTATOR_MESG (ROTATOR_BIT_ARRAY_LENGTH*8)

typedef struct _rotator_file_header
{
  char rotator_file_length[10];
  char bit_array[ROTATOR_BIT_ARRAY_LENGTH];
} rotator_file_header;

typedef struct _rotator_file
{
  FILE *fp;
  int rdonly;
  int num_entries;
  struct _rotator_file_header rfh;
} rotator_file;

typedef struct _rotator_file_entry
{
  g_uint16 entry_num;
  g_uint16 usr_number;
  g_uint16 active;
  g_uint16 should_rotate;
  g_uint16 max_length;
  g_uint8  name[120];
  g_uint16 lines;
  g_uint8  dummy[302];
} rotator_file_entry;

int load_rotator_entry(int num, rotator_file_entry *entry);

#endif _GTALK_ROTATOR_H



