/*
 *
 * rotui.c
 *
 */

#include <sys/types.h>
#include <stdio.h>
#include "rotator.h"
#include "rotui.h"
#include "userst.h"
#include "gtmain.h"

void ask_question(char *string, int *num)
 {
   char s[10];
   char *data;
   print_string(string);
   do
     {
      get_editor_string(s,5);
     } while (!(*s));
   *num = str_to_num(s,&data);
 };

void list_rotator_box(rotator_file_entry *temp)
 {
   char s[50];
   sprintf(s,"Box #:%03d, (1) User #:%03d, (2) Active: ",
	   temp->entry_num,temp->usr_number);
   print_string(s);
   if (temp->active) print_str_cr("Yes");
    else print_str_cr("No");
   sprintf(s," (3) Max length: %d, (4) Should rotate: ",temp->max_length);
   print_string(s);
   if (temp->should_rotate) print_str_cr("Yes");
    else print_str_cr("No");
   print_string(" (5) Title: ");
   print_string(temp->name);
 };

/*

void read_box_contents(int num)
 {
   rotator_file_entry temp;
   int rotator_num;
   char filename[200];
   struct user_data userptr;

   if (num<0)
    {
      print_cr();
      ask_question("Which rotator box to read: ",&rotator_num);
    }
    else rotator_num = num;
   if ((rotator_num<0)|| (rotator_num>MAX_ROTATOR_MESG))
     return;
   if (load_rotator_entry(rotator_num,&temp))
    {
      print_sys_mesg("Message unavailable");
      return;
    };
   if (!temp.active)
    {
      print_sys_mesg("Message # NOT active");
      return;
    };

   ansi_on(1);
   sprintf(filename,"|*f7|*h1%sMessage #%03d : %s|*r1",
	   "-->",temp.entry_num,temp.name);
   print_str_cr(filename);
   ansi_on(0);

   print_cr();
   find_rotator_filename(filename,&temp);
   print_file(filename);
   print_cr();
   load_user_info(temp.usr_number,&userptr);
   next_task();
   if ((userptr.user_info.user_no<0) || (!userptr.user_info.enable))
      sprintf(filename,"|*f7|*h1%s<END> by #%03d : <AUTHOR UNKNOWN>|*r1",
	      "-->",temp.usr_number);
   else
      sprintf(filename,"|*f7|*h1%s<END> by #%03d : %s|*r1",
	      "-->",temp.usr_number,userptr.user_info.handle);

   ansi_on(1);
   print_str_cr(filename);
   ansi_on(0);
 };
*/


void modify_box_contents(void)
 {
   rotator_file_entry temp;
   int rotator_num;
   int ch;
   char filename[30];
   int is_empty = 1;
   FILE *fileptr;

   print_cr();
   ask_question("Which Message to change: ",&rotator_num);
   if ((rotator_num<0) || (rotator_num>MAX_ROTATOR_MESG)) return;
   if (load_rotator_entry(rotator_num,&temp))
    {
      print_str_cr("Message is blank.");
      return;
    };
   if (!temp.active)
    {
      print_str_cr("Message Inactive.");
      return;
    };
   if ((temp.usr_number != mynode->userdata.user_info.user_no) && 
       (!testFlag(mynode,"CMD_ROT")))
    {
      print_str_cr("Access to Message DENIED.");
      return;
    };
   find_rotator_filename(filename,&temp);
   line_editor(filename,temp.max_length);
   if (fileptr=fopen(filename,"rb"))
    {
      while ((!feof(fileptr)) && is_empty)
       {
         ch = fgetc(fileptr);
         if ((ch>' ') && (ch<127)) is_empty = 0;
       };
      fclose(fileptr);
    };
   if (is_empty)
    {
      temp.should_rotate = 0;
      save_rotator_entry(rotator_num,&temp);
      print_str_cr("The box is blank, disabling rotation.");
    };
 };

void modify_rotator_box(void)
 {
   int rotator_num;
   rotator_file_entry temp;
   int flag = 1;
   int tnum;
   char command[5];

   if (!testFlag(mynode,"CMD_ROT"))
    {
      print_str_cr("--> Invalid Command: Enter /? for Help");
      return;
    };
   print_cr();
   ask_question("Which Message to Modify: ",&rotator_num);
   if ((rotator_num<0) || (rotator_num>MAX_ROTATOR_MESG)) return;
   if (load_rotator_entry(rotator_num,&temp))
     assign_blank_rotator_info(rotator_num,&temp);
   while (flag)
    {
     list_rotator_box(&temp);
     print_cr();
     print_string("Modify (1-5): ");
     get_editor_string(command,1);
     print_cr();
     if ((*command == 13) || (*command == 'q')) flag = 0;
     if (*command == '1')
      {
        ask_question("User number to ASSIGN to: ",&tnum);
        if (tnum>=0) temp.usr_number = tnum;
         else print_str_cr("Invalid User #.");
      };
     if (*command == '2')
      temp.active=get_yes_no("Active");
      if (!temp.active) temp.should_rotate = 0;
     if (*command == '3')
      {
        ask_question("Max length of buffer: ",&tnum);
        if ((tnum<16) || (tnum>8192)) print_str_cr("Invalid buffer length.");
         else temp.max_length = tnum;
      };
     if (*command == '4')
      temp.should_rotate=get_yes_no("Rotate Active");
     if (*command == '5')
      { print_string("Enter New Title: ");
        do
         { get_input(temp.name,70);
         } while (!*(temp.name));
      }
    };
   print_str_cr("Message Modifications Saved.");
   save_rotator_entry(rotator_num,&temp);
 };

/*

void modify_rotate_params(void)
 {
   unsigned int minutes;
   unsigned int seconds;
   unsigned int rotator_temp;
   unsigned int ticks;

   print_cr();
   rotator_temp=get_yes_no("Turn the Rotator on? ");
   if (!rotator_temp)
    {
      change_task_to_scheduler(rotator_id,(task_type) rotate_message,
			       NULL,PERIODIC_TASK,0,0);
      rotator_delay = 0;
      return;
    };
   ask_question("Minutes to rotate messages? ",&minutes);
   ask_question("Seconds to rotate messages? ",&seconds);
   ticks = (minutes*60 + seconds);
   if ((minutes>45) || (seconds>59) || (ticks<3))
    print_string("--> Invalid time");
    else
    {
     rotator_delay = ticks;
     change_task_to_scheduler(rotator_id,(task_type) rotate_message,
     NULL,PERIODIC_TASK,rotator_delay,1);
    };
   check_no_rotator_messages();
 };
*/

void rotator_system(char *str,char *name, int portnum)
 {
   int flag = 1;
   char command[5];

   while (flag)
    {
      check_for_privates();
      print_cr();
      print_string("Rotator Commands: (R,W,M,Q,T,?): ");
      do
        {
         get_input(command,1);
        } while (!(*command));

      if (*command>'Z') *command -= 32;
      if (*command=='Q') flag = 0;
      else
      if (*command=='M') modify_rotator_box();
      else
      if (*command=='W') modify_box_contents();
      else
      if (*command=='R') read_box_contents(-1);
      else
      if (*command=='T') modify_rotate_params();
      else
      if (*command=='?')
       print_file("help\\rotator.hlp");
    };
    print_cr();
    print_str_cr("--> Returning to System");
 };


void modify_user_rotator_box(void)
 {
   int rotator_num;
   rotator_file_entry temp;
   char s[50];

   print_cr();
   ask_question("Which Message to Modify: ",&rotator_num);
   if ((rotator_num<0) || (rotator_num>MAX_ROTATOR_MESG)) return;
   if (load_rotator_entry(rotator_num,&temp))
     {
       print_str_cr("Message Inactive.");
       return;
     };
   if ((temp.usr_number != mynode->userdata.user_info.user_no) && 
       (!testFlag(mynode,"CMD_ROT")))
     {
       print_str_cr("Access to Message DENIED.");
       return;
     };
   sprintf(s,"Box #:%03d, User #:%03d,",temp.entry_num,temp.usr_number);
   print_string(s);
   sprintf(s," Max length: %d, Should rotate: ",temp.max_length);
   print_string(s);
   if (temp.should_rotate) print_str_cr("Yes");
    else print_str_cr("No");
   print_cr();
   temp.should_rotate=get_yes_no("Should this Message Rotate? ");
   print_string("Current Title: ");
   print_str_cr(temp.name);
   if (get_yes_no("Enter new title? "))
     {
       print_cr();
       print_string("New Title: ");
       do { get_input(temp.name,70); }
        while (!*temp.name);
     }
   save_rotator_entry(rotator_num,&temp);
 };

void rotator_menu_system(char *str,char *name, int portnum)
 {
   int flag = 1;
   char command[5];

   while (flag)
    {
      check_for_privates();
      print_cr();
      print_string("Message Command: ");
      do
         {
           get_input(command,1);
         } while (!(*command));


      switch (toupper(*command))
      {

       case 'Q'  :  flag = 0;
                    break;
       case 'M'  :  modify_user_rotator_box();
                    break;
       case 'W'  :  modify_box_contents();
                    break;
       case 'R'  :  read_box_contents(-1);
                    break;
       case '?'  :  print_file("help\\mesg.hlp");
                    break;
      }

    };
    print_cr();
    print_str_cr("--> Returning to System");
 };


