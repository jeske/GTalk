



/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */




/* TERMINAL.C */


/* headers */

#define INCL_DOS
#include <os2.h>

#include "include.h"
#include "gtalk.h"
#include "terminal.h"
#include "event.h"
#include "console.h"




#define MAX_FILENAMES 8





int monitor_download(int portnum)
{
  int file_number=0;
  int packet_number;
  int key;
  struct transfer_struct *transfer_ptr;
  char s[20];

  if (!line_status[portnum].transfer)
    {
        print_str_cr("Error: No download on that line.");
        return 0;
    }
  if (!does_pid_exist(line_status[portnum].transfer_pid))
    {
        print_str_cr("Error: No download on that line.");
        return 0;
    }

  transfer_ptr = line_status[portnum].transfer;

  print_str_cr("<-- Monitor Download -->");

  do
  {

   if (file_number!=transfer_ptr->file_number)
        {
         while ((transfer_ptr->event == PROTO_FILE_START))
           {
            delay(1);
            key = int_char(tswitch);
            if ((key=='Q'))
                 return 1;
            if (transfer_ptr!=line_status[portnum].transfer)
			  return 0;
			if (!does_pid_exist(portnum))
			  return 0;
           }

         file_number = transfer_ptr->file_number;
         sprintf(s,"[%02d]:",file_number);
         print_cr();
         print_string(s);
         print_string(transfer_ptr->filename);
         packet_number = 0;
         print_string("Blk: 0000");
        }

   if (packet_number!=transfer_ptr->current_packet_no)
   {
    repeat_chr(8,4,0);
    sprintf(s,"%04d",packet_number);
    print_string(s);
    packet_number = transfer_ptr->current_packet_no;
   }
   key = int_char(tswitch);
   if ((key=='Q'))
     return 1;

   delay(25);
  } while ((transfer_ptr==line_status[portnum].transfer)  && does_pid_exist(portnum));

  return 0;

}


void modem_terminal_task(void)
{

	delay(4);

    end_task();

}

struct start_download_struct
{
    int pid_of_starter;
    int pid_of_task;
    int data_ready;
    int download_started;
    int download_error;
};


void modem_download_task(void)
{
  struct start_download_struct *temp=(struct start_download_struct *)schedule_data[tswitch];
  struct start_download_struct download_info;
  int maxfiles = MAX_FILES_TRANSFER;
  char *filebuffer = g_malloc((FILENAME_LEN+1)*MAX_FILENAMES,"RCVYNMS");
  char *filepointers[MAX_FILENAMES];
  int num_names;
  int count=0;

  while (!(temp->data_ready))
    next_task();

  delay(18);
  download_info = *temp;
  if (!filebuffer)
  {
    if (does_pid_exist(download_info.pid_of_starter))
    temp->download_error=1;
    end_task();
  }

  for (num_names=0;num_names<MAX_FILENAMES;num_names++)
    filepointers[num_names] = &filebuffer[num_names*(FILENAME_LEN+1)];

  if ((tswitch>num_ports) || (tswitch<0) || is_console_node(tswitch))
   {
    if (does_pid_exist(download_info.pid_of_starter))
       temp->download_error=1;
    g_free(filebuffer);
    end_task();
   }

  else maxfiles = MAX_FILES_TRANSFER;

  if (does_pid_exist(download_info.pid_of_starter))
     temp->download_started=1;

  receive_files("", filepointers, &maxfiles, Y_MODEM_PROTOCOL);
  g_free(filebuffer);

  while (count<300)
  {
  if (!does_pid_exist(download_info.pid_of_starter))
    end_task();
  if (!dcd_detect(tswitch))
    end_task();

  if (count>50)
    { if (!(count % 10))
        aput_into_buffer(task_of_pid(download_info.pid_of_starter),
                          "--> Download Completed (/term+ to regain line)",
                          0,5,tswitch,10,0);
    }
  delay(18);
  }

  end_task();
}

/*
  add_task_to_scheduler((task_type) run_bot_event,
      (void *)&transfer_struct, REL_SHOT_TASK, 0,1,1024, filename);
   */

/* allows a sysop to pick up a modem for dialout */
/* Control-E is the start command Char, which
   would be followed by : */
/* Control-B toggles the baud rate */
/* Control-A exits */
/* Control-L Links the node*/
/* Control-R Relogs the user */
/* Control-D Toggles Data Bits */
/* Control-T Transfer Files           NOT DONE */
/* Control-H Handup the Modem         NOT DONE */
/* ?         Help                     NOT DONE */

void modem_terminal(char *str,char *name,int portnum)
{
    char *point;
    int line = str_to_num(str,&point);
    int ischar,isthere;
    int flag = 1;
    int link=0;
    unsigned int baud = 2400;
    char parity = 'N';
    int databits = 8;
    int stopbits = 1;
    int killafter=1;
    int command_code = 0;
    unsigned long timeout = user_options[portnum].time_sec;
    int warn = user_options[portnum].warnings;
    struct start_download_struct download;
    char s[90];

    if (line<0 || line>sys_info.max_nodes)
     { print_sys_mesg(NodeOutOfRange);
        return;
     }
    if (!port[line].active)
    { print_sys_mesg("Port Not Active");
      return;
    }

    if (((*point!='+') || !test_bit(user_options[tswitch].privs,TERMPLUS_PRV)) && ((line<0) || (line>=(MAXPORTS)) || ispaused(line) || (line == portnum) || (line_status[line].connect)) )
      {
        if (ispaused(line) && !line_status[line].connect)
          {
             sprintf(s,"--> Node [%d] already in terminal with node [%d]",who_paused(line),line);
             print_str_cr_to(s,portnum);
          }
        else
           print_str_cr_to("--> Node in Use",portnum);
        return;
      };

    if (line_status[line].online && (*point!='+'))
      {
        print_str_cr_to("--> Line not available",portnum);
        return;
      };



    killafter=!line_status[line].connect;

    // log_off_no_restart(line);
    if (line==tswitch)
     { print_sys_mesg("Cannot Terminal with your own node");
        return;
    }
    print_str_cr_to("--> Securing Line",portnum);


    wait_for_death(line);

    if (*point=='+')
      pause(line);
    else
      {
       pause(line);
       lock_dos(341);
       kill_task(line);
       make_task((task_type) modem_terminal_task, TASK_STACK_SIZE, line, 1,"TERMINAL");
       pause(line);
       unlock_dos();
      }

    // set_baud_rate(line,baud,8,1,'N');
    user_options[portnum].time_sec = 0;
    user_options[portnum].warnings = 1;
    print_str_cr_to("--> Entering Terminal",portnum);
    print_str_cr_to("--> Ctrl-E Ctrl-A aborts",portnum);
    empty_inbuffer(line);
    print_str_cr_to("AT",line);
    while (flag)
     {
       if (!dcd_detect(portnum)) log_off(portnum,1);

       isthere=0;
       in_char(line,&ischar,&isthere);

       if (isthere)
        print_chr_to(ischar,portnum);
       /* { char st[8];
          sprintf(st,"%u ",ischar);
          print_string_to(st,portnum);
        }*/


        else next_task();

       isthere=0;
       in_char(portnum,&ischar,&isthere);

       if ((command_code) && (isthere))
        {
            switch (ischar)
             {
                case   1  : flag =  0;           // control - A
                            break;
                case   7  :
                            flag=0;              // Control - G
                            link=3;
                            break;


                case   12 : flag=0;              // Control - L
                            link=1;
                            break;

                case   15 : if (is_console())    // Control - O ??
                                 flag = 0;
                            break;

                case   18 :
                            if (test_bit(user_options[tswitch].privs,SHUTDOWN_PRV))
                               { flag = 0;           // Control - R
                                 link = 2;
                               }
                            break;

                case   20 : /* control - T */
                           download.download_started=0;
                           download.download_error=0;
                           download.data_ready=0;
                           download.pid_of_starter=MY_PID();

                           lock_dos(349);
                           schedule_data[line] = (void *)&download;
                           kill_task(line);
                           make_task((task_type) modem_download_task, TASK_STACK_SIZE, line, 1,"DOWNLOAD");
                           download.pid_of_task = pid_of(line);
                           download.data_ready=1;
                           unlock_dos();

                           print_str_cr("[Waiting for download to start]");
                           while ((!(download.download_started) && !(download.download_error)) && does_pid_exist(download.pid_of_task))
                             delay(1);

                           if (download.download_error)
                             print_str_cr("[Download Error]");
                           else
                             {print_str_cr("[Download Started]");
                              if (monitor_download(line))
                                { print_str_cr("--> Exiting Terminal, download continuing");
                                  clear_call_on_logoff();
                                  return;
                                }
                              print_cr();
                              print_str_cr("[Download Completed]");
                             }

                            pause(line);
                            lock_dos(341);
                            kill_task(line);
                            make_task((task_type) modem_terminal_task, TASK_STACK_SIZE, line, 1,"TERMINAL");
                            pause(line);
                            unlock_dos();
                            print_str_cr("[Returned to Terminal]");

                           break;
                case  9 /* 16 */:
                             switch (parity)
                              {
                                 case 'N':  parity = 'E';
                                            break;
                                 case 'E':  parity = 'O';
                                            break;
                                 case 'O':  parity = 'N';
                                            break;
                               };
                              set_baud_rate(line,baud,databits,stopbits,parity);
                              print_cr_to(portnum);
                              sprintf(s,"Parity is %c",parity);
                              print_str_cr_to(s,portnum);
                              break;

                case   4   :
                              databits = 15 - databits;
                              set_baud_rate(line,baud,databits,stopbits,parity);
                              print_cr_to(portnum);
                              sprintf(s,"Databits is %d",databits);
                              print_str_cr_to(s,portnum);
                              break;

                case   19   :
                              stopbits = 3 - stopbits;
                              set_baud_rate(line,baud,databits,stopbits,parity);
                              print_cr_to(portnum);
                              sprintf(s,"Stopbits is %d",stopbits);
                              print_str_cr_to(s,portnum);
                              break;
                case   2  :
                              switch (baud)
                               {
                                 case 1200: baud = 2400;
                                            break;
                                 case 2400: baud = 9600;
                                            break;
                                 case 9600: baud = 300;
                                            break;
                                 case 300:  baud = 1200;
                                            break;
                               };
                              set_baud_rate(line,baud,databits,stopbits,parity);
                              print_cr_to(portnum);
                              sprintf(line_status[line].baud,"%u",(unsigned int)baud);
                              sprintf(s,"Baud rate is %d",baud);
                              print_str_cr_to(s,portnum);
                              break;

          }  // end of BIG SWITCH
          // command has been processed
        command_code = 0;
       }
       else
        {
          if (isthere) print_chr_to(ischar,line);
        };
       if ((isthere) && (ischar==5))
        command_code = 1;
     };
    clear_call_on_logoff();
    if (ischar==15)
      { /* leave port online */
        print_cr();
        print_sys_mesg("Leaving Port Online");
        return;
      }

    switch (link)
    {

       case  1 :
                 print_cr_to(portnum);
                 print_str_cr_to("--> Making Link",portnum);
                 /* log off the line */
                 add_task_to_scheduler((task_type) link_node_event, (void *)line,
                   REL_SHOT_TASK, 0, 1, 1024, "LINKNODE");
                 delay(2);
                 break;
       case  2 :
                 print_cr_to(portnum);

                 print_str_cr_to("--> Relogging Line",portnum);
                 line_status[line].connect=1;
                   add_task_to_scheduler((task_type) relog_node_event, (void *)line,
                    REL_SHOT_TASK, 0, 1, 1024, "RELOGNODE");
                 delay(2);
                 break;
       case  3 :
                 print_cr_to(portnum);
                 print_str_cr_to("--> Making G-Link",portnum);
                 /* log off the line */
                 add_task_to_scheduler((task_type) g_link_node_event, (void *)line,
                   REL_SHOT_TASK, 0, 1, 1024, "GLNKNODE");
                 delay(2);
                 break;
       default :
                if (killafter)
                  kill_task(line);
                else
                  unpause(line);
                print_cr_to(portnum);
                print_str_cr_to("--> End Terminal",portnum);

                user_options[portnum].time_sec = timeout;
                user_options[portnum].warnings = warn;
                break;
    }


    // done with terminal, so exit
    reset_attributes(tswitch);
}



