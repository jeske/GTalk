
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - gamecon.h
 *
 */

#ifndef _GTALK_GAMECON_H
#define _GTALK_GAMECON_H

#include "types.h"

typedef struct _gamecon_header
{
  g_uint8 header[4];
  g_uint8 sending_player;
  g_uint8 destination_player;
  g_uint8 data_length;
  g_uint8 data[256];
} gamecon_header;

#define GAMECON_RECEIVING_HEADER 0
#define GAMECON_RECEIVING_SEND_PLAYER 1
#define GAMECON_RECEIVING_DEST_PLAYER 2
#define GAMECON_RECEIVING_DATA_LENGTH 3
#define GAMECON_RECEIVING_DATA 4

typedef struct _gamecon_state_struct
{
  struct _gamecon_header hdr;
  int major_state;
  int header_index_no;
  int data_amount;
  int is_7_bit;
  int aborts;
  g_uint8 identity;
} gamecon_state_struct;

int cmd_gamecon(com_struct *com, char *string);

#endif  /* _GTALK_GAMECON_H */
