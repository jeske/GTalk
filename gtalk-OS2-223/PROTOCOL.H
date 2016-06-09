


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



/* Header file for X and Y modem protocols */

#ifndef PROTOCOL_HEADER
#define PROTOCOL_HEADER

#define X_MODEM_PROTOCOL     0
#define X_MODEM_CRC_PROTOCOL 1
#define X_MODEM_1K_PROTOCOL  2
#define Y_MODEM_PROTOCOL     3
#define Y_MODEM_G_PROTOCOL   4

/* directions */
#define TO_CHAT     0
#define FROM_CHAT   1

/* events */
#define PROTO_NOT_STARTED 0
#define PROTO_FILE_START  10
#define PROTO_END         100
#define PROTO_ERROR_END   101


struct transfer_struct
{
  char *filename;
  char protocol;
  long int total_bytes;
  long int current_bytes;
  unsigned int total_blocks;
  unsigned int current_block;
  unsigned int current_packet_no;
  unsigned int cps;
  char direction;
  char event;
  unsigned int num_errors;
  time_t start;
  unsigned int file_number;
} ;


int send_files(char **filenames, int count, int mode);

int receive_files(char *directory, char **filenames, int *count, int mode);

void add_pathnames(char *fullname, char *directory, char *filename, int maxlen);



#endif

