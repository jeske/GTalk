
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - gamecon.c
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <grp.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <termios.h>
#include <arpa/telnet.h>

#include "types.h"
#include "str.h"
#include "list.h"
#include "abuf.h"
#include "comparse.h"
#include "command.h"
#include "gtmain.h"
#include "states.h"
#include "common.h"
#include "gamecon.h"


static g_uint8 pheader[] = { 0x03, 0x02, 0x01, 0x21, 0x00 };

static void debugging_info(char *channel, char *header, g_uint8 *ch, int len)
{
  char head[1000];

  sprintf(head,"MESSAGE %s %s ", channel, header);
  while (len > 0)
    {
      sprintf(&head[strlen(head)]," %02X", *ch++);
      len--;
    }
  client_abuf_write(my_ip, SERVER_PROCESS, STATE_CHANNEL,
		    head, strlen(head)+1);
}

void send_out_gcp_packet(char *bheader, int bheaderlen,
			 gamecon_header *hdr, int is_7_bit,
			 int is_one_extra)
{
  char *data = ((char *)hdr) + (is_7_bit ? 2 : 0);
  int len = sizeof(*hdr) - 256 - (is_7_bit ? 2 : 0) + hdr->data_length +
    is_one_extra;
  g_uint8 s[512];
 
  memcpy(s, bheader, bheaderlen);
  memcpy(&s[bheaderlen], data, len);
  bheaderlen += len;
  client_abuf_write(my_ip, SERVER_PROCESS, STATE_CHANNEL,
		    s, bheaderlen);
}

static void print_hex(g_uint8 *ch, int len)
{
  while (len > 0)
    {
      printf("%02X ",*ch++);
      len--;
    }
  printf("\r\n");
}


int cmd_gamecon(com_struct *com, char *string)
{
  fd_set read_fd;
  g_uint8 ch;
  int is_7_bit = 0, is_one_extra = 0, debug = 0;
  int quit, len, temp, fcntl_save_state, bheaderlen;
  abuffer abuf;
  gamecon_header *hdr;
  gamecon_state_struct state;
  char bheader[40];
  char buf[512], buf2[512];

  string = skip_blanks(string);
  while ((*string) && (*string != ' '))
    {
      switch (*string)
	{
           case '8': is_7_bit = 0;
	             break;
	   case '7': is_7_bit = 1;
	             break;
	   case 'X':
	   case 'x': is_one_extra = 1;
	             break;
  	   case 'N':
	   case 'n': is_one_extra = 0;
	             break;
	   case 'D':
	   case 'd': debug = 1;
	             break;
	}
      string++;
    }
  
  if (!(*mynode->cur_chan))
    {
      printf_ansi("--> Must be on a channel to play Game Connection\r\n");
      return (-1);
    }
  printf_ansi("--> Now entering Game Connection mode on channel %s\r\n",
	      mynode->cur_chan);
  printf_ansi("--> Type Control-X 15 times to exit\r\n");
  if ((fcntl_save_state = fcntl(0, F_GETFL, 0)) < 0)
    {
      printf_ansi("--> Internal Error\r\n");
      return (-1);
    }
  fcntl_save_state &= O_ACCMODE;
  save_termios();
  tty_raw(0);
  state.major_state = GAMECON_RECEIVING_HEADER;
  state.header_index_no = is_7_bit ? 2 : 0;
  state.identity = 0xFF;
  state.aborts = 0x00;
  bheaderlen = sprintf(bheader,"BINARY %s ", mynode->cur_chan);
  quit = 0;
  while (!quit)
    {
      FD_ZERO(&read_fd);
      FD_SET(mypipe, &read_fd);
      FD_SET(0, &read_fd);
      
      temp = select(mypipe+1, &read_fd, NULL, NULL, NULL);

      if (temp > 0)
	{
	  if (FD_ISSET(0, &read_fd))
	    {
	      if (debug)
		{
		  sprintf(buf,"State: temp = %d state = %d %d %d", 
			  temp, state.major_state,
			  state.header_index_no, state.data_amount);
		  debugging_info(mynode->cur_chan, buf, NULL, 0);
		}
	      switch (state.major_state)
		{
		  case GAMECON_RECEIVING_HEADER:
		    if (read(0, &ch, sizeof(ch)) == sizeof(ch))
		      {
			if (debug)
			  debugging_info(mynode->cur_chan, "HDR:", &ch, 1);
			if (ch == '\030')
			  {
			    if ((++state.aborts) == 10)
			      quit = 1;
			    break;
			  } else
			    state.aborts = 0;
			if ((state.hdr.header[state.header_index_no] = ch)
			    == pheader[state.header_index_no])
			  {
			    if ((++state.header_index_no) == 4)
			      {
				state.major_state = 
				  GAMECON_RECEIVING_SEND_PLAYER;
				break;
			      }
			  } else
			    state.header_index_no = is_7_bit ? 2 : 0;
		      }
		    break;
		  case GAMECON_RECEIVING_SEND_PLAYER:
		    if (read(0, &ch, sizeof(ch)) == sizeof(ch))
		      {
			if (debug)
			  debugging_info(mynode->cur_chan, "SEND:", &ch, 1);
			state.hdr.sending_player = ch;
			if ((ch != state.identity) && (state.identity == 0xFF))
			  state.identity = ch;
			state.major_state = 
			  GAMECON_RECEIVING_DEST_PLAYER;
		      }
		    break;
		  case GAMECON_RECEIVING_DEST_PLAYER:
		    if (read(0, &ch, sizeof(ch)) == sizeof(ch))
		      {
			if (debug)
			  debugging_info(mynode->cur_chan, "DEST:", &ch, 1);
			state.hdr.destination_player = ch;
			state.major_state = 
			  GAMECON_RECEIVING_DATA_LENGTH;
		      }
		    break;
		  case GAMECON_RECEIVING_DATA_LENGTH:
		    if (read(0, &ch, sizeof(ch)) == sizeof(ch))
		      {
			if (debug)
			  debugging_info(mynode->cur_chan, "LEN:", &ch, 1);
			state.hdr.data_length = ch;
			state.major_state =
			  GAMECON_RECEIVING_DATA;
			state.data_amount = 0;
		      }
		    break;
		  case GAMECON_RECEIVING_DATA:
		    len = state.hdr.data_length - state.data_amount + 
		      is_one_extra;
		    if ((temp=read(0, &state.hdr.data[state.data_amount], 
				   len)) > 0)
		      {
			state.data_amount += temp;
			if (state.data_amount == 
			    (state.hdr.data_length + is_one_extra))
			  {
			    send_out_gcp_packet(bheader, bheaderlen,
						&state.hdr, is_7_bit,
						is_one_extra);
			    state.major_state =
			      GAMECON_RECEIVING_HEADER;
			    state.header_index_no = is_7_bit ? 2 : 0;
			  }
		      }
		    break;
		}
	    }
	  if (FD_ISSET(mypipe, &read_fd))
	    {
	      if (read_abuffer(mypipe, &abuf, buf, sizeof(buf)-1) > 0)
		{
		  if (abuf.type == STATE_CHANNEL)
		    {
		      if (!memcmp(buf, bheader, bheaderlen))
			{
			  if (is_7_bit)
			    {
			      hdr = (gamecon_header *) &buf[bheaderlen-2];
			      if (((hdr->destination_player >= 0x7F) ||
  			        (hdr->destination_player == state.identity)) &&
				  ((hdr->sending_player != state.identity) ||
				   (state.identity == 0xFF)))
				write(1, &buf[bheaderlen], 
				      abuf.payload_length - bheaderlen);
			    }
			  else
			    {
			      hdr = (gamecon_header *) &buf[bheaderlen];
			      if (((hdr->destination_player == 0xFF) ||
				   (hdr->destination_player == state.identity)) &&
				  ((hdr->sending_player != state.identity) ||
				   (state.identity == 0xFF)))
				write(1, hdr, abuf.payload_length - bheaderlen);
			    }
			  if (debug)
			    {
			      sprintf(buf2, "State message type: %02X", 
				      abuf.type);
			      debugging_info(mynode->cur_chan, buf2, buf,
					     abuf.payload_length);
			    }
			}
		    }
		}
	    }
	}
    }
  fcntl(0, F_SETFL, fcntl_save_state);
  restore_termios();
  printf_ansi("--> Return from Game Connection Mode\r\n");    
}









