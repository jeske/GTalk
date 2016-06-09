

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#define INCL_DOS
#define INCL_DOSDEVIOCTL
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#include <os2.h>

/* headers */
#include "include.h"
#include "gtalk.h"
#include "console.h"
#include "os2ser.h"


#include "com.h"
/* this MUST be the first procedure in the program */

void init_input_buf(int loop);


void check_com(int id)
{
    if (id < num_ports)
    {
     if (port[id].active)
       {
         DosResumeThread(port[id].recv_thread_id);
       }
    }
}

void start_com(int numstart)
{
   int count;
   ULONG dummy;

   num_ports = numstart;            /* store the number of ports */

   for (count=0;count<numstart;count++)
     if (port[count].active)
         initport(count,port[count].baud_rate,8,1,'N');

}
void end_com(void)
{
   int count;

   for (count=0;count<num_ports;count++)
     if (port[count].active)
         de_initport(count);
}

int (*a_chars_in_buffer[MAXPORTS])(int portnum);
int (*a_dcd_detect[MAXPORTS])(int portnum);
void (*a_put_char_in_buffer[MAXPORTS])(char temp, int portnum);
void (*a_get_char[MAXPORTS])(int portnum,int *charput, int *isthere);
void (*a_send_char[MAXPORTS])(int portnum,char charput);
void (*a_send_chars[MAXPORTS])(int portnum,char *charptr, int length);
void (*a_empty_inbuffer[MAXPORTS])(int portnum);
int (*a_char_in_buf[MAXPORTS])(int portnum);
int (*a_get_first_char[MAXPORTS])(int portnum);
int (*a_get_nchar[MAXPORTS])(int portnum);
void (*a_wait_for_xmit[MAXPORTS])(int portnum,int ticks);
void (*a_empty_outbuffer[MAXPORTS])(int portnum);
void (*a_change_dtr_state[MAXPORTS])(int portnum, int state);

void initport(int portnum, int baud, int stopbits, int databits, char parity);


    /* BOARD TYPES */

#define CONSOLE_TYPE            0
#define STANDARD_COM_TYPE       1
#define DIGIBOARD_DUMB_TYPE     2
#define DIGIBOARD_SMART_TYPE    3
#define STARGATE_SMART_TYPE     4
#define STS_BOARD_TYPE          5
#define OS2_BOARD_TYPE          6


    /* GENERAL USE ROUNTINES */

void send_string(int portnum, char *string);
void get_key(int portnum, int *charput, int *isthere);
void set_baud_rate(int portnum, unsigned int baud, int databits, int stopbits, char parity);
void hang_up(int port_num);

typedef struct port_info *port_info_ptr;

port_info_ptr port_fast[MAXPORTS];

int num_ports;

/* stores information about a port */
struct port_info  port[MAXPORTS];
struct input_struct port_input[MAXPORTS];

de_initport(int port_num)
{
   switch (port[port_num].board_type)
   {
     case CONSOLE_TYPE:
                        break;
#ifndef CONSOLE
     case OS2_BOARD_TYPE:
             de_init_os2_port(port_num);
             break;
#endif
   }
}

/* Initializes a port for usage. CALL WITH INTERRUPTS DISABLED! */
/* port_num is the port, baud is the baud rate 300-115200 */
/* databits is the number of databits */
/* stopbits is the number of stopbits */
/* this has no meaning for the console */

void initport(int port_num, int baud, int databits, int stopbits,
	char parity)
{
   port_fast[port_num] = (struct port_info  *)
        &port[port_num];

   port_fast[port_num]->output_cur_pos =
	  port_fast[port_num]->output_buffer;
   port_fast[port_num]->output_end_buf =
	  &port_fast[port_num]->output_buffer[OUTPUT_BUF_LEN];
   port_fast[port_num]->modem_responding = 1;
   port_fast[port_num]->ignore_response = 0;
   port_fast[port_num]->num_retries = 0;
   port_fast[port_num]->seconds_between_retries = 9;
   switch (port[port_num].board_type)
   {
     case CONSOLE_TYPE:
       a_chars_in_buffer[port_num] = chars_in_buffer_keyboard;
       a_dcd_detect[port_num] = dcd_detect_keyboard;
       a_put_char_in_buffer[port_num] = put_char_in_buffer_keyboard;
       a_get_char[port_num] = get_char_keyboard;
       a_send_char[port_num] = send_char_keyboard;
	   a_send_chars[port_num] = send_chars_keyboard;
	   a_empty_inbuffer[port_num] = empty_inbuffer_keyboard;
       a_char_in_buf[port_num] = char_in_buf_keyboard;
       a_get_first_char[port_num] = get_first_char_keyboard;
       a_get_nchar[port_num] = get_nchar_keyboard;
	   a_wait_for_xmit[port_num] = wait_for_xmit_keyboard;
       a_empty_outbuffer[port_num] = empty_outbuffer_keyboard;
       a_change_dtr_state[port_num] = change_dtr_state_keyboard;
       break;
#ifndef CONSOLE
     case OS2_BOARD_TYPE:
       a_chars_in_buffer[port_num] = chars_in_buffer_os2;
       a_dcd_detect[port_num] = dcd_detect_os2;
       a_put_char_in_buffer[port_num] = put_char_in_buffer_os2;
       a_get_char[port_num] = get_char_os2;
	   a_send_char[port_num] = send_char_os2;
	   a_empty_inbuffer[port_num] = empty_inbuffer_os2;
	   a_send_chars[port_num] = send_chars_os2;
	   a_char_in_buf[port_num] = char_in_buf_os2;
       a_get_first_char[port_num] = get_first_char_os2;
	   a_get_nchar[port_num] = get_nchar_os2;
       a_wait_for_xmit[port_num] = wait_for_xmit_os2;
       a_empty_outbuffer[port_num] = empty_outbuffer_os2;
       a_change_dtr_state[port_num] = change_dtr_state_os2;
       break;
#endif
    default:
	port[port_num].active=0;
        break;
   }
   switch (port[port_num].board_type)
   {
     case CONSOLE_TYPE:
                        if (allocate_a_console(port_num)==(-1))
							port[port_num].active=0;

                        break; /* don't call init_8250_port
                                  for console */
#ifndef CONSOLE
     case OS2_BOARD_TYPE:
             if (!init_os2_port(port_num,baud,databits,stopbits,parity))
              {
                port[port_num].active=0;
              }
             break;
#endif
   }
}


/* hangs up port determined by port_num by dropping DTR */


void change_dtr_state_keyboard(int port_num, int state)
{
}

#ifdef CONSOLE
void hang_up(int port_num)
{
    return;
}
#else

void hang_up(int port_num)
{
  time_t now;

  if (is_console_node(port_num)) return;

  empty_outbuffer(port_num); // if it's a smart port
							 // FLUSH IT so the modem dosnt
							 // keep getting characters stuffed
							 // at it.

  now=time(NULL);
  change_dtr_state(port_num,0);
  while ((time(NULL)-now)<2)
   {
	 next_task();
   }
  change_dtr_state(port_num,1);
  now=time(NULL);
  while ((time(NULL)-now)<2)
   {
	 next_task();
   }
}
#endif

/* set the baud rate on the port designated by port_num */
/* see init_port for implementation details */
/* interrupts need not be disabled, but its a good idea */

#ifdef CONSOLE
void set_baud_rate(int port_num, unsigned int baud,
	 int databits, int stopbits, char parity)
{
	return;
}
#else
void set_baud_rate(int port_num, unsigned int baud,
	 int databits, int stopbits, char parity)
{

  switch(port[port_num].board_type)
  {
	default:break;
	case OS2_BOARD_TYPE: set_baud_rate_os2(port_num,baud,
					databits,stopbits,parity);
					break;
  }

}
#endif


int wait_for_modem_result(char *string,int ticks)
{
  return (wait_for_modem_result_portnum(string,ticks,tswitch));
}





#ifdef CONSOLE

int wait_for_modem_result_portnum(char *string,int ticks,int portnum)
{
  return 1;
}

#else

int wait_for_modem_result_portnum(char *string,int ticks,int portnum)
{
  int match=0;
  int next_char;
  int seconds = (ticks/18) + 1;
  char temp_str[60];
  int len=strlen(string);
  int loop;
  char *end=temp_str+len-1;
  time_t start = time(NULL);



  while (((time(NULL)-start)<seconds) && !(match))
   {

	next_char = get_nchar(portnum);

	if (next_char>27)
	  {
		for (loop=0;loop<len-1;loop++)
		  temp_str[loop]=temp_str[loop+1];

		*end=next_char;

		if (!strncmp(string,temp_str,len))
			match = 1;

	   // aput_into_buffer(server,temp_str,0,5,portnum,9,0);

	  }

	next_task();
	DosSleep(10l);
   }

   return match;
}

#endif

int chars_in_buffer_keyboard(int port_num)
{
  return((char_in_buf_keyboard(port_num) ? 1 : 0));
}

/* see if someone's online by DCD in port_num */
/* this must be changed before it will recognize anyone logging on properly */

int dcd_detect_keyboard(int port_num)
{
  return 1;
}

void send_string(int portnum, char *string)
{
  while (*string) queue_char(portnum,*string++);
  flush_output_buffer(portnum);
}

void send_string_noflush(int portnum, char *string)
{
  while (*string) queue_char(portnum,*string++);
}


/* get a key from either the console or the serial */

int wait_for_dcd_state(int port_num, int delay)
{
  int state;
  int flag = 1;
  time_t time_count;

  while (flag)
   {
	 state = dcd_detect(port_num);
	 time_count = time(NULL);
	 flag = 0;
	 while ((!flag) && ((time(NULL)-time_count)<delay))
	  {
		if (state != dcd_detect(port_num)) flag = 1;
		next_task();
	  };
   };
 return (state);
};


void put_in_input_buf(int bufnum,char key)
{
   struct input_struct *pins;
   char *cur_write_loc;
   char *next_write_loc;

   if (bufnum<0 || (bufnum > MAXPORTS))
	 return;
   if (!port[bufnum].active)
	 return;

   pins = &port_input[bufnum];
   cur_write_loc = pins->buffer_write;
   next_write_loc = cur_write_loc + 1;



   if (next_write_loc == pins->buffer_end)
	 next_write_loc = pins->buffer_start;

   pins->used=1;

   if (next_write_loc != pins->buffer_read)
	{
	   *(cur_write_loc) = key;

	   pins->buffer_write = next_write_loc;
	   if (pins->wakeup_sleeper)
		  DosPostEventSem(*(pins->wakeup_sleeper));
	   task_wake(bufnum);
	}

   pins->used=0;

}

int get_from_input_buf(int bufnum,int remove)
{
   struct input_struct *pins;
   char *cur_read_loc;
   char *next_read_loc;
   char key;

   if (bufnum<0 || (bufnum > MAXPORTS))
     return -1;
   if (!port[bufnum].active)
     return -1;

   pins = &port_input[bufnum];
   cur_read_loc = pins->buffer_read;
   next_read_loc = cur_read_loc+1;

   if (next_read_loc>=pins->buffer_end)
     next_read_loc = pins->buffer_start;

   if (cur_read_loc != pins->buffer_write)
    {
       key = *(cur_read_loc);
       if (remove)
         pins->buffer_read = next_read_loc;
	   return ((unsigned char)key);
	}
    return -1;
}

void empty_input_buf(int bufnum)
{
  struct input_struct *pins;

  if (bufnum<0 || (bufnum > MAXPORTS))
	 return;
  if (!port[bufnum].active)
	 return;

   pins = &port_input[bufnum];

   pins->buffer_read = pins->buffer_write;

}

int char_in_input_buf(int bufnum)
{
   struct input_struct *pins;
   char *cur_read_loc;
   char *cur_write_loc;
   int chars;

   if (bufnum<0 || (bufnum > MAXPORTS))
	 return -1;
   if (!port[bufnum].active)
	 return -1;

   pins = &port_input[bufnum];

   cur_read_loc = pins->buffer_read;
   cur_write_loc = pins->buffer_write;

   if (cur_write_loc>=cur_read_loc)
     return (cur_write_loc - cur_read_loc);
   else
     return (INPUT_BUF_LEN - (cur_read_loc - cur_write_loc));


}


void init_input_buf(int loop)
{
	 port_input[loop].wakeup_sleeper=0;
     port_input[loop].buffer_read = port_input[loop].buffer_write
                = port_input[loop].buffer_start;
     port_input[loop].buffer_end = port_input[loop].buffer_start + INPUT_BUF_LEN;
     port_input[loop].used=0;
}

void queue_char(int portnum, char charput)
{
	port_info_ptr cport = port_fast[portnum];

	if (cport->output_cur_pos == cport->output_end_buf)
	{
	   send_chars(portnum,cport->output_buffer,OUTPUT_BUF_LEN);
	   cport->output_cur_pos = cport->output_buffer;
	}
	*cport->output_cur_pos++ = charput;
}

void flush_output_buffer(int portnum)
{
 	port_info_ptr cport = port_fast[portnum];

	if (cport->output_cur_pos != cport->output_buffer)
	{
	   send_chars(portnum,cport->output_buffer,(unsigned long int)(cport->output_cur_pos) -
					(unsigned long int)(cport->output_buffer));
	   cport->output_cur_pos = cport->output_buffer;
	}
}

void dump_output_buffer(int portnum)
{
 	port_info_ptr cport = port_fast[portnum];

	if (cport->output_cur_pos != cport->output_buffer)
	{
	   cport->output_cur_pos = cport->output_buffer;
	}
}


