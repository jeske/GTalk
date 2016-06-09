/* COM.H */

#ifndef GT_COM_H
#define GT_COM_H


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#define MAXPORTS 30
#define MAXCOMP 29  /* Should always be MAXPORTS - 1 */
#define INPUT_BUF_LEN 1024
#define OUTPUT_BUF_LEN 256

#define is_console() (is_a_console[tswitch])
#define is_console_node(node) ((is_a_console[(node)]))

#ifdef INCL_DOS
struct port_info    /* stores information about a port */
 {
   /* NEW stuff from serial.cfg file */

   char init_string[60];
   char de_init_string[60];

   unsigned int baud_rate;
   char board_type;
   unsigned int io_address;
   unsigned int digi_lookup_address;
   char port_number;
   char dial_in_line;
   char fixed_dte_speed;
   char should_die;

   char rts_cts;
   char os2_com_name[14];
   HFILE os2_PortHandle;
   TID   recv_thread_id;

   /* NEW MODEM RESPONSE CODE */

   char modem_responding;
   char ignore_response;
   unsigned int num_retries;
   unsigned int seconds_between_retries;

   /* END OF NEW STUFF */

   int active;      /* if port is used, active = 1 */
   int console;     /* if port is the console, console = 1 */
   int no_dcd_detect;      /* should we cancel dcd? */

   char verify_node;
   char restrict_level;

   char *output_cur_pos;
   char *output_end_buf;
   char output_buffer[OUTPUT_BUF_LEN];
 };

 typedef struct port_info *port_info_ptr;
 extern struct port_info port[MAXPORTS];
 extern port_info_ptr port_fast[MAXPORTS];

#endif

struct input_struct {
#ifdef INCL_DOSPROCESS
   HEV         *wakeup_sleeper;
#else
   void        *wakeup_sleeper;
#endif
   unsigned char        buffer_start[INPUT_BUF_LEN];
   unsigned char        *buffer_read;
   unsigned char        *buffer_write;
   unsigned char        *buffer_end;
   char        used;
};

void put_in_input_buf(int bufnum,char key);
 
 /* extern declarations */


 extern struct input_struct port_input[MAXPORTS];
 extern int port_active[MAXPORTS];
 extern int num_ports;

            /* INITIALIZATION rountines */

void start_com(int numstart);
void end_com(void);
void initport(int portnum, int baud, int stopbits, int databits, char parity);

            /* GENERAL COM ROUNTINES */

int wait_for_dcd_state(int port_num, int delay);
void send_string(int portnum, char *string);
void set_baud_rate(int portnum, unsigned int baud, int databits,
                   int stopbits, char parity);

// int get_nkey(int portnum);


int wait_for_modem_result_portnum(char *string,int ticks,int portnum);
int wait_for_modem_result(char *string,int ticks);


            /* SERIAL MACROS */

#define chars_in_buffer(portnum) (*a_chars_in_buffer[(portnum)])((portnum))
#define dcd_detect(portnum) (*a_dcd_detect[(portnum)])((portnum))
#define put_char_in_buffer(temp,portnum) (*a_put_char_in_buffer[(portnum)])((temp),(portnum))
#define get_char(portnum,charput,isthere) (*a_get_char[(portnum)])((portnum),(charput),(isthere))
#define send_char(portnum,charput) (*a_send_char[(portnum)])((portnum),(charput))
#define send_chars(portnum,charptr,length) (*a_send_chars[(portnum)])((portnum),(charptr),(length))
#define empty_inbuffer(portnum) (*a_empty_inbuffer[(portnum)])((portnum))
#define char_in_buf(portnum) (*a_char_in_buf[(portnum)])((portnum))
#define get_first_char(portnum) (*a_get_first_char[(portnum)])((portnum))
#define get_nchar(portnum) (*a_get_nchar[(portnum)])((portnum))
#define wait_for_xmit(portnum,ticks) (*a_wait_for_xmit[(portnum)])((portnum),(ticks))
#define empty_outbuffer(portnum) (*a_empty_outbuffer[(portnum)])((portnum))
#define change_dtr_state(portnum,state) (*a_change_dtr_state[(portnum)])((portnum),(state))

#define in_char(portnum,charput,isthere) get_char(portnum,charput,isthere)
#define int_char(portnum) get_nchar(portnum)

extern int (*a_chars_in_buffer[MAXPORTS])(int portnum);
extern int (*a_dcd_detect[MAXPORTS])(int portnum);
extern void (*a_put_char_in_buffer[MAXPORTS])(char temp, int portnum);
extern void (*a_get_char[MAXPORTS])(int portnum,int *charput, int *isthere);
extern void (*a_send_char[MAXPORTS])(int portnum,char charput);
extern void (*a_send_chars[MAXPORTS])(int portnum,char *charptr,int length);
extern void (*a_empty_inbuffer[MAXPORTS])(int portnum);
extern int (*a_char_in_buf[MAXPORTS])(int portnum);
extern int (*a_get_first_char[MAXPORTS])(int portnum);
extern int (*a_get_nchar[MAXPORTS])(int portnum);
extern void (*a_wait_for_xmit[MAXPORTS])(int portnum,int ticks);
extern void (*a_empty_outbuffer[MAXPORTS])(int portnum);
extern void (*a_change_dtr_state[MAXPORTS])(int portnum, int state);

de_initport(int port_num);
void de_init_os2_port(int port_num);
void hang_up(int port_num);
void queue_char(int portnum, char charput);
void flush_output_buffer(int portnum);
void dump_output_buffer(int portnum);
void send_string_noflush(int portnum, char *string);

#endif /* GT_COM_H */
