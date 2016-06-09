

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


#include "include.h"
#include "gtalk.h"
#include "feedback.h"




int send_mail_to_and_about(int user_no,char *subject);
int verbose_get_yes_no(char *prompt);

#define MAX_FEEDBACK_NUM 10
#define FEEDBACK_HEADER_FILE "FDBK\\FDBK.HDR"

struct priv_main_entry
{
    char menu_key;
    char priv_filename[50];
    char item_title[80];
    char to_number[10];
};


void print_main_feedback_menu(struct priv_main_entry priv_entry[],int number,char *header)
{
  char s[180];
  int old_code = special_code(1,tswitch);
  int loop;

      print_cr();
      print_string("|*f4|*h1");
      print_str_cr(header);
      print_string("|*r1");
      print_cr();

        for (loop=0;loop<number;loop++)
          {
            sprintf(s,"|*r1|*f4[|*r1|*h1%c|*r1|*f4] |*h1%s",priv_entry[loop].menu_key,
                           priv_entry[loop].item_title);
            print_str_cr(s);

          }
       print_cr();

  special_code(old_code,tswitch);
}

void extended_feedback(char *filename)
{
  char next_filename[80];
  int to_user_number=-1;
  int old_code = special_code(1,tswitch);
  char subject[200];
  int exit=0;


  if (!line_status[tswitch].num_feedbacks_allowed)
  {
    if (!test_bit(user_options[tswitch].privs,SEND_MAIL_PRV))
     {
        print_sys_mesg("No more feedbacks Allowed this call.");
        return;
     }
  }


  while (!exit)
   {
      { int loop,flag;
        char s[120];
        char header[80];
        char strnumber[20];
        struct priv_main_entry priv_entry[MAX_FEEDBACK_NUM];


        int number;
        FILE *fileptr;

        lock_dos(471);
        if ((fileptr=g_fopen(filename,"r","FEEDBK #1"))==NULL)
           {
            log_error(filename);
            print_sys_mesg("Could not open feedback file");
            unlock_dos();
            return;
           }
        file_get_string(header,79,fileptr);
        file_get_string(strnumber,20,fileptr);
        strnumber[3]=0;
        number=atoi(strnumber);
        if (number>MAX_FEEDBACK_NUM)
          {
            sprintf(s,"* Too many entries in file %s",filename);
            log_error(s);
            print_str_cr(s);
            g_fclose(fileptr);
            unlock_dos();
            return;
          }

        for (loop=0;loop<number;loop++)
          {
            fscanf(fileptr,"%c*",&priv_entry[loop].menu_key);
             priv_entry[loop].menu_key=toupper(priv_entry[loop].menu_key);
            file_get_string(priv_entry[loop].item_title,79,fileptr);
            file_get_string(priv_entry[loop].priv_filename,39,fileptr);
            file_get_string(priv_entry[loop].to_number,9,fileptr);
            if feof(fileptr)
              { sprintf(strnumber,"* Incorrect format in file %s",filename);
                log_error(strnumber);
                print_str_cr(strnumber);
                g_fclose(fileptr);
                unlock_dos();
                return;
              }
          }

       g_fclose(fileptr);
       unlock_dos();

        /* file is read */

            print_main_feedback_menu(&priv_entry,number,header);


        flag=1;
        while(flag)
         {

           print_string("|*f4|*h1Feedback Selection Menu (|*f6?|*f4,|*f6Q|*f4=quit)|*r1|*h1:|*r1 ");
           *s=0;
           while (!*s)
              get_string(s,2);
           if (*s>'Z') *s-=32;
           if (*s=='?')
            print_main_feedback_menu(&priv_entry,number,header);
           else
           if (*s=='Q')
             { flag=0; exit=1; }
           else
           {
             loop=0;
             while (loop<number && *s!=priv_entry[loop].menu_key)
                loop++;
             if (loop!=number)
              { char *dummy;
                strcpy(next_filename,priv_entry[loop].priv_filename);
                to_user_number = str_to_num(priv_entry[loop].to_number,&dummy);
                sprintf(subject,"|*f4|*h1FDBK|*f7:|*r1%s",priv_entry[loop].item_title);
                flag=0;
              }

           }

         }
     }

     if ((!exit) && (to_user_number != -1))
     {

         print_file_to_cntrl(FEEDBACK_HEADER_FILE,tswitch,1,0,0,0);
         print_file_to_cntrl(next_filename,tswitch,1,1,0,1);

         if (verbose_get_yes_no("|*f4|*h1Are you |*f2|*h1SURE|*f4|*h1 you want to send Feedback? "))
         {
         /* do feedback here */
            if (line_status[tswitch].num_feedbacks_allowed)
            {
               if (!test_bit(user_options[tswitch].privs,SEND_MAIL_PRV))
                 line_status[tswitch].num_feedbacks_allowed--;
            }
            print_str_cr("Sending Feedback To:");

            send_mail_to_and_about(to_user_number,subject);
         }

         special_code(old_code,tswitch);
         return;
     }

 } // total exit

     special_code(old_code,tswitch);
}
