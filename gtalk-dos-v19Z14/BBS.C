

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* BBS.C */

#include "include.h"
#include "gtalk.h"
#include "structs.h"

#define MAX_MESG_LIMIT 50
#define MAX_BOARDS 999
#define MESSAGE_SIZE 4096
#define BBS_PAGING 1

#undef DEBUG

/* INTERNAL PROTOTYPES */

int load_bbs_user(char *directory,int number, struct bbs_user_account *bbs_ptr);
int save_bbs_user(char *directory, int number, struct bbs_user_account *bbs_ptr);
int check_for_bbs_board(int board_num);



void delete_bbs_message(int board_number,int message_no, long int entry_no)
{
  char command[7];
  int num;
  char *point;
  struct mail_pass_data data;
  int flag = !islocked(DOS_SEM);

  if (entry_no==-1)
  {
      if (message_no==-1)
      {
        prompt_get_string("Delete Post Number: ",command,4);
        num = str_to_num(command,&point);
      }
      else
        num = message_no;
  }

  if ((num>0) || (entry_no!=-1))
  {
    if (flag) lock_dos(47);
    open_bbs_base(board_number,&data,0);

    if (entry_no==-1)
       entry_no = convert_to_entry_num(&data,num);

    if (num<=data.entry.max_mesg)
       delete_msg_from_base(&data,entry_no,0);

    close_base(&data);
    if (flag) unlock_dos();
 }
}


void create_blank_bbs_acct(struct bbs_user_account *buser)
{
   int loop;

   buser->newscan=1;
   buser->is_moderator=0;
   buser->moderator_level=0;
   buser->newscan_pointer=0;
   for (loop=0;loop<20;loop++)
     buser->privs[loop]=0;
}

void reset_bbs_newscan_pointer(int board_num)
{
  int user_number = user_lines[tswitch].number;
  struct bbs_user_account buser;
  struct mail_pass_data data;

  lock_dos(103);
  if (!read_bbs_base(board_num,&data,NO_CREATE))
    { unlock_dos();
      print_str_cr(" Unable To open Base");
      return;
    }
  if (load_bbs_user(data.basename,user_number,&buser))
    { unlock_dos();
      print_str_cr(" Unable To Load BBS USER");
      return;
    }

  buser.newscan_pointer = data.entry.head_entry_no;

  if (save_bbs_user(data.basename,user_number,&buser))
    { unlock_dos();
      print_str_cr(" Unable To Save BBS USER");
      return;
    }

  close_base(&data);
  unlock_dos();
  print_str_cr("Newscan Pointer Reset");
  return;

}



int toggle_global_newscan(int board_num,int state)
{
  int user_number = user_lines[tswitch].number;
  struct bbs_user_account buser;
  struct mail_pass_data data;

  lock_dos(105);
  if (!read_bbs_base(board_num,&data,NO_CREATE))
    { unlock_dos();
      print_str_cr(" Unable To open Base");
      return;
    }
  if (load_bbs_user(data.basename,user_number,&buser))
    {
      unlock_dos();
      print_str_cr(" Unable To Load BBS USER");
      return;
    }
  if (state==-1)
    state = !buser.newscan;

  buser.newscan = state;
  if (save_bbs_user(data.basename,user_number,&buser))
    {
      unlock_dos();
      print_str_cr(" Unable To Save BBS USER");
      return;
    }

  close_base(&data);
  unlock_dos();
  return (buser.newscan);

}


int load_bbs_user(char *directory,int number, struct bbs_user_account *bbs_ptr)
{
    int flag=islocked(DOS_SEM);
    int number_users;
    FILE *fileptr;
    char bbs_user_file[120];

    create_blank_bbs_acct(bbs_ptr);
    sprintf(bbs_user_file,"%s\\BUSER.DAT",directory);

    if (!flag) lock_dos(91);

    if (!(fileptr=g_fopen(bbs_user_file,"rb","BBS#6")))
       {
        log_error("*bbs file wouldn't open");
        log_error(bbs_user_file);
        if (!flag) unlock_dos();
        return 1;
       }
    fseek(fileptr,0,SEEK_SET);
    fscanf(fileptr,"%d\n",&number_users);
    if (number>number_users)
       {
        log_error("*LOAD BBS ACCT : system tried to read past end of user file");
        g_fclose(fileptr);
        if (!flag) unlock_dos();
        return 1;
       }
    else
        fseek(fileptr,
         (long int)sizeof(struct bbs_user_account)*(number+1),SEEK_SET);
        if (!fread(bbs_ptr, sizeof(struct bbs_user_account), 1, fileptr))
             {  log_error("* fread() failed on file ");
                log_error(bbs_user_file);
                g_fclose(fileptr);
                if (!flag) unlock_dos();
                return 1;
             }
        if (g_fclose(fileptr))
           {
             log_error("fclose failed");
             log_error(bbs_user_file);
             if (!flag) unlock_dos();
             return 1;
           }
    if (!flag) unlock_dos();
 return 0;

}

int save_bbs_user(char *directory, int number, struct bbs_user_account *bbs_ptr)
{
    int flag=islocked(DOS_SEM);
    int number_users;
    int putit;
    struct bbs_user_account temp;
    FILE *fileptr;
    char bbs_user_file[120];

    sprintf(bbs_user_file,"%s\\BUSER.DAT",directory);

    if (!flag) lock_dos(92);

    if (!(fileptr=g_fopen(bbs_user_file,"rb+","BBS#7")))
       {
        if (!(fileptr=g_fopen(bbs_user_file,"wb","BBS#8")))
         {
          log_error(bbs_user_file);
          if (!flag) unlock_dos();
          return 1;
         };
        fseek(fileptr,0,SEEK_SET);
        fprintf(fileptr,"0\n");
       }
    fseek(fileptr,0,SEEK_SET);
    fscanf(fileptr,"%d\n",&number_users);

    if (number>=number_users)
        {
           log_error("* SYSTEM tried to add user that exhisted");

        create_blank_bbs_acct(&temp);
        fseek(fileptr,
         (long int)sizeof(struct bbs_user_account)*
         (number_users),SEEK_SET);
        for (putit=number_users;putit<number;putit++)
         if (!fwrite(&temp, sizeof(struct bbs_user_account), 1, fileptr))
              { log_error(bbs_user_file);
                g_fclose(fileptr);
                 if (!flag) unlock_dos();
                 return 1;
              }
        fseek(fileptr,0,SEEK_SET);
        fprintf(fileptr,"%d\n",number+1);
        }
        
        fseek(fileptr,
         (long int)sizeof(struct bbs_user_account)*(number+1),SEEK_SET);
        if (!fwrite(bbs_ptr, sizeof(struct bbs_user_account), 1, fileptr))
             {  log_error(bbs_user_file);
                log_error("*tried to write user and failed");
                g_fclose(fileptr);
                if (!flag) unlock_dos();
                return 1;
             }
   fflush(fileptr);
   if (g_fclose(fileptr))
        {
          log_error(bbs_user_file);
          if (!flag) unlock_dos();
          return 1;
        }
    if (!flag) unlock_dos();
 return 0;

};

void list_boards(void)
{
    print_file("bbs\\boards.lst");

}


int jump_to_new_bbs_board(void)
{
 char s[6];
 char *dummy;
 int num;
 int flag=1;

 while (flag)
 {
     print_cr();

     print_string("Enter New Board Number (? for list) : ");
     do { get_string(s,3); } while (!*s);
     if (*s=='?')
       list_boards();
     else
     {
       num = str_to_num(s,&dummy);
       flag=0;
     }
 }
 if (num<0) { print_str_cr("Invalid Entry");
              return -1;
            }
 if (!check_for_bbs_board(num))
   { print_str_cr("Sorry, that Board not available");
     return -1;
   }
 else
   return(num);

}

long int read_new_bbs_message(int board_num,struct bbs_user_account *buser)
{

  struct mail_pass_data data;
  long int success;
  unsigned long int entry_num;

  lock_dos(93);
  open_bbs_base(board_num,&data,0);
  unlock_dos();

  entry_num=buser->newscan_pointer;

  if ((entry_num<data.entry.tail_entry_no) ||
                  (entry_num>=data.entry.head_entry_no))
     success = 0;
  else
  {
      if (success = read_single_message(&data,entry_num))
           if (entry_num>=buser->newscan_pointer)
              {
                buser->newscan_pointer=entry_num+1;
                save_bbs_user(data.basename,user_lines[tswitch].number,buser);
              }
  }

  lock_dos(94);
  close_base(&data);
  unlock_dos();

  if (success)
      return ((long int)(entry_num));
  else
    return -1;

}



long int read_a_bbs_message(int board_num,int num,struct bbs_user_account *buser)
{
  struct mail_pass_data data;
  int success;
  unsigned long int entry_num;

  lock_dos(95);
  open_bbs_base(board_num,&data,0);
  unlock_dos();

  entry_num=convert_to_entry_num(&data,num);

  if (success = read_single_message(&data,entry_num))
      if (entry_num>=buser->newscan_pointer)
          {
            buser->newscan_pointer=entry_num+1;
            save_bbs_user(data.basename,user_lines[tswitch].number,buser);
          }

  lock_dos(96);
  close_base(&data);
  unlock_dos();

  if (success)
      return entry_num;
  else
    return -1;
}

void reply_to_message(struct mail_pass_data *data, struct bbs_user_account *buser,
                      unsigned long int entry_number,int board_number)
{
  char s[4];
  struct mesg_entry mentry;
  struct file_entry file;

  file.openfile=0;

  read_entry(data,entry_number,&mentry,&file);

  print_cr();
  print_string("Will This be a private Reply (y/n)? ");
  do { get_string(s,2); } while (!*s);
  if (toupper(*s)=='Y')
   { send_mail_subj(mentry.user_no);
    return;
   }
  post_message_on_board(board_number);

}

void jump_to_message(struct mail_pass_data *data, unsigned long int *entry_num)
{
  char input[4];
  int num;
  char *dummy;
  unsigned long int temp_entry_num;
  print_cr();
  print_string("Jump To: ");
  do {get_string(input,3);} while (!*input);
  if ((num=str_to_num(input,&dummy))<0)
   { print_str_cr("Aborted");

     return;
    }
  temp_entry_num = convert_to_entry_num(data,num);
  if ((temp_entry_num<data->entry.head_entry_no) &&
               (temp_entry_num>=data->entry.tail_entry_no))
     *entry_num = temp_entry_num;
  print_str_cr("Jumped");
  print_cr();

}

int scan_bbs_messages(int board_num, struct mail_pass_data *data,
                    struct bbs_user_account *buser,
                    unsigned long int start_entry_num)
{
  char s[180];
  int keep_going=1;
  int again=0;

  do
  {

      if (start_entry_num<data->entry.tail_entry_no)
          start_entry_num = data->entry.tail_entry_no;

      if (read_single_message(data,start_entry_num))
        {  if (start_entry_num>=buser->newscan_pointer)
              {
                buser->newscan_pointer=start_entry_num+1;
                save_bbs_user(data->basename,user_lines[tswitch].number,buser);
              }
        }
      else
       { print_str_cr("Error Reading Message");
         return 0;
       }


        sprintf(s,"[|*f2|*h1%02d|*r1 of |*f2|*h1%02d|*r1] [|*h1A|*r1]gain [|*h1P|*r1]rev [|*h1N|*r1]ext [|*h1J|*r1]ump [|*h1R|*r1]eply [|*h1Q|*r1]uit: ",
               convert_from_entry_num(data,start_entry_num),
               convert_from_entry_num(data,data->entry.head_entry_no) - 1);
        do
        {
            check_for_privates();
            again=0;
            print_cr();
            switch(get_hot_key_prompt(s,"ANJRPQ",'N',1))
            {
                default:
                case 'N' : start_entry_num++;
                           break;
                case 'A' : break;
                case 'J' : jump_to_message(data,&start_entry_num);
                           break;
                case 'Q' : keep_going=0;
                           break;
                case 'R' : reply_to_message(data,buser,start_entry_num,board_num);
                           start_entry_num++;
                           break;
                case 'P' : if (start_entry_num>data->entry.tail_entry_no)
                              start_entry_num--;
                           break;


            }
        } while (again);

      if (start_entry_num>=data->entry.head_entry_no)
       keep_going = 0;
    }
    while (keep_going);

   return 1;
}

int ask_for_post(int board_num)
{

    check_for_privates();

    print_cr();

    switch(get_hot_key_prompt("Post on board? [Y/N/Q] ","YNQ",'N',0))
    {
       case 'Q' : return 0; // stop scanning
       case 'Y' : post_message_on_board(board_num);
                  break;
       case 'N' : break;

    }

   return 1; // keep scanning

}

int newscan_base(int board_num, struct mail_pass_data *data,
                    struct bbs_user_account *buser)
{
    int num;
    char s[130];


    if (data->entry.tail_entry_no>buser->newscan_pointer)
        buser->newscan_pointer = data->entry.tail_entry_no;

    if (data->entry.head_entry_no<buser->newscan_pointer)
      return 1;                        // continue onto next base

    if (!(num = (data->entry.head_entry_no) - (buser->newscan_pointer)))
      return 1;

    check_for_privates();

    sprintf(s,"#|*f2|*h1%02d|*r1 [|*f2|*h1%s|*r1] New: |*f4|*h1%02d|*r1 [|*h1R|*r1]ead [|*h1S|*r1]kip [|*h1Q|*r1]uit: ",board_num,
              data->entry.name,num);


    print_cr();
    switch(get_hot_key_prompt(s,"RSQ",'R',1))
    {
        default:
        case 'R' : scan_bbs_messages(board_num,data,buser,
                               buser->newscan_pointer);
                   if (!ask_for_post(board_num))
                     return 0;
        case 'S' : break;
        case 'Q' :
                   return 0;

    }
    return 1;
}

void global_newscan(void)
{
  struct mail_pass_data data;
  int base_loaded=0;
  struct bbs_user_account buser;
  int board_num = 1;
  int flag=0;

  print_cr();
  print_str_cr("Scanning...");

  do
  {
     if (base_loaded) { close_base(&data); base_loaded=0; }

     if (!read_bbs_base(board_num,&data,NO_CREATE))
        { print_cr();
          print_str_cr("End of Newscan.");
          return;
        }
     else
       base_loaded=1;

    if (load_bbs_user(data.basename,user_lines[tswitch].number,&buser))
       {   print_str_cr(" Could Not Load BBS/User File");
           return;
        }

   if (buser.newscan)
       flag = newscan_base(board_num,&data,&buser);

   board_num++;

   }
   while (flag);

   print_cr();
   print_str_cr("End of Newscan.");

   close_base(&data);
}

void list_bbs_messages(int board_number,int mode, int show_deleted,
                       struct bbs_user_account *buser)
{

  struct mail_pass_data data;
  struct mesg_entry mentry;
  struct file_entry file;
  unsigned long int cur;
  unsigned long int stop_message_number;
  int showed_message=NO;
  int flag = !islocked(DOS_SEM);
  char s[120];
  int num_lines=20;
  int cur_line=0;
  int abort=0;
  int show_message=0;

  print_cr();

  if (flag) lock_dos(97);

  if (!read_bbs_base(board_number,&data,NO_CREATE))
  {
    if (flag) unlock_dos();
    print_str_cr("--> No BBS index.. ");
    return;
  }


   cur = data.entry.tail_entry_no;
   stop_message_number = data.entry.head_entry_no;


  while ((cur<stop_message_number) && (!abort))
  {
    file.openfile = 0;
    get_entry(&data,cur,&mentry,&file);

    show_message=1;

    switch(mode)
    {
     case LIST_NEW_MAIL :  if (mentry.entry_no<buser->newscan_pointer)
                             show_message=0;
                           break;
     case LIST_OLD_MAIL :  if (mentry.entry_no>=buser->newscan_pointer)
                             show_message=0;
                           break;
     default:
     case LIST_ALL_MAIL :  break;
    }

    if ((!(mentry.deleted) || show_deleted) && show_message)
    {
        unlock_dos();

        showed_message=YES;

        special_code(1,tswitch);
        if (mentry.entry_no>=buser->newscan_pointer)
          print_string("N ");
        else
          print_string("O ");

        sprintf(s,"|*h1|*f4%02d  |*f7(#%03d) ", convert_from_entry_num(&data,cur),mentry.user_no);
        print_string(s);

        print_string_of_len(mentry.username,18);

        print_string("|*r1 |*h1|*f4");

        print_string_of_len(mentry.subject,25);

        if (mentry.deleted)
           print_string("|*r1|*f1|*h1 deleted|*r1");

        print_cr();
        special_code(0,tswitch);

            if (++cur_line>num_lines)
             {
               abort=do_page_break();
               cur_line=0;
              }

        lock_dos(98);
    }

    cur++;
  }

  close_base(&data);

  if (!showed_message)
   { unlock_dos();
         switch (mode)
     {
       case LIST_NEW_MAIL: print_str_cr("No New Messages");
                           break;
       default:
       case LIST_ALL_MAIL: print_str_cr("No Messages to Display");
                           break;
       case LIST_OLD_MAIL: print_str_cr("No Old Messages to Display");
                           break;
      }
      lock_dos(99);
    }

  if (flag) unlock_dos();
}

void get_new_board_info(int board_number,char *name, char *directory)
{
  struct mail_pass_data data;

  if (read_bbs_base(board_number,&data,NO_CREATE))
    {
     if (name) strcpy_n(name,data.entry.name,BOARD_NAME);
     if (directory) strcpy_n(directory,data.basename,FILENAME_LEN);
    }
  else
    {print_str_cr("Could Not Read Base");
     strcpy(name,"NONE");
     strcpy(directory,"");
    }
  close_base(&data);

}


int check_for_bbs_board(int board_num)
{
 struct mail_pass_data data;
 char s[120];

 if (!open_bbs_base(board_num,&data,NO_CREATE))
   {
      print_str_cr("Base Unavailable");
      if (user_lines[tswitch].priority)
        return 0;

      print_cr();
      print_string("Would you Like to Create BBS Board #");
      sprintf(s,"%d : ",board_num);
      print_string(s);

      do {get_string(s,2); } while (!*s);

      if (toupper(*s)=='Y')
       {
         if (open_bbs_base(board_num,&data,CREATE))
           { print_string("Base Created  : ");
             sprintf(data.entry.name,"Board #%02d",board_num);
             *(data.entry.bbs_base_code)=0;

             print_str_cr(data.entry.name);

              print_string("  Operating on Directory: ");
              print_str_cr(data.basename);

              print_str_cr("   - Adding Messages");
              do_assimilation(&data,"M0*.");

              print_str_cr("  DONE.");

             close_base(&data);
             return(1);
            }
          else
           { print_str_cr("Base Creation Failed.");
            return 0;
           }
       }
       else
       return 0;

   }

  close_base(&data);
  return (1);

}

int put_message_in_bbs(struct mesg_entry *mentry, struct file_entry *file,
     int board_number)
{
  struct file_entry file2;
  struct mail_pass_data data;
  int error = 1;
  int flag = !islocked(DOS_SEM);
  char s[80];
  int user_no = user_lines[tswitch].number;

  file2.openfile = 1;
  if (flag) lock_dos(100);
  if (!open_bbs_base(board_number,&data,1)) error = 0;
   else if (!add_msg_to_base(&data,0,&file2,mentry,1)) error = 0;
    else
    { fprintf(file2.fp,"|*h1|*f4Board:    |*f7%s",
                         data.entry.name);
      if (*(data.entry.bbs_base_code))
        fprintf(file2.fp,"(%s)",data.entry.bbs_base_code);
      fprintf(file2.fp,"\r\n");

      fprintf(file2.fp,"|*h1|*f4From:     |*f7");
      if (user_no < 0) fprintf(file2.fp,"(%%GST)");
       else fprintf(file2.fp,"(#%03d)",mentry->user_no);
      fprintf(file2.fp," %s|*r1\r\n",mentry->username);
      fprintf(file2.fp,"|*h1|*f4System:   |*f7(BBS) Gtalk-%02d:%s|*r1\r\n",
        mentry->system_no,mentry->systemname);
      fprintf(file2.fp,"|*h1|*f4Subject:  |*f7%s|*r1\r\n",mentry->subject);
      strftime(s,39,"%m/%d/%y %I:%M:%S %p",localtime(&mentry->ent_date));
      fprintf(file2.fp,"|*h1|*f4Date:     |*f7%s [%lu]|*r1\r\n",
            s,mentry->ent_date);
      fprintf(file2.fp,"|*h1|*f4Contents:|*r1\r\n\r\n");
      copy_a_file(&file2,file,0);
      g_fclose(file2.fp);
    }
  close_base(&data);
  if (flag) unlock_dos();
  return (error);
}


int post_message_on_board(int board_number)
{
  struct mesg_entry mentry;
  struct file_entry file;
  char board_name[BOARD_NAME+1];
  int user_no = -1;


  get_new_board_info(board_number,board_name,NULL);

  print_cr();
  special_code(1,tswitch);
  print_string("|*h1From:|*r1    ");
  print_str_cr(user_lines[tswitch].handle);
  print_string("|*r1|*h1Board:|*r1   ");
  print_str_cr(board_name);
  print_string("|*r1|*h1Subject:|*r1 ");
  special_code(0,tswitch);
  get_string_cntrl(mentry.subject,SUBJECT_LEN,0,0,0,0,1,0,0);
  print_cr();
  if (!(*mentry.subject))
      {  print_cr();
         print_str_cr("Post Aborted.");
         return (0);
      }
  file.openfile = 0;
  if (!create_temp_file(&file,MAX_EDITOR_LEN,0))
    {    print_cr();
         print_str_cr("Post Aborted.");
         return (0);
    }
  mentry.dest_user_no = user_no;
  mentry.system_no = sys_info.system_number;
  mentry.user_no = user_lines[tswitch].number;
  mentry.dest_no = sys_info.system_number;
  strcpy_n(mentry.username,user_lines[tswitch].handle,USER_NAME_LEN);
  strcpy_n(mentry.systemname,sys_info.system_name,SYSTEM_NAME_LEN);
  lock_dos(101);
  mentry.ent_date=time(NULL);
  unlock_dos();
  put_message_in_bbs(&mentry,&file,board_number);
  print_cr();
  print_str_cr("Message Posted.");
  return (1);
}

void examine_bbs_board(void)
{
  struct mail_pass_data data;
  char *dat;
  char s[7];
  int board_number;

  print_cr();
  print_string("Which BBS board to read: ");
  get_editor_string(s,5);
  board_number=str_to_num(s,&dat);

  if (!open_bbs_base(board_number,&data,0)) return;

  print_fifo_info(&data);

  close_base(&data);
}

void sysop_base_configuration(int board_num,struct bbs_user_account *buser)
{
   struct mail_pass_data data;
   char name[BOARD_NAME+1] = "";
   char input[2];
   int flag=1;
   do
   {
       print_str_cr("Sysop Base Configuration");
       print_str_cr("1) Name ");
       print_str_cr("Q) Quit ");
       print_cr();
       print_string("Enter Selection: ");
       do { get_string(input,1); } while (!*input);
       switch (toupper(*input))
       {
         case '1' :  print_string("Enter new name: ");
                     get_string(name,BOARD_NAME);
                     break;
         case 'Q' :  flag=0;
                     break;
       }
   }
   while (flag);

   lock_dos(102);

   if (!open_bbs_base(board_num,&data,0))
      { unlock_dos();
        print_str_cr("Error Saving Changes");
        return;
      }

   if (*name) strcpy(data.entry.name,name);

   close_base(&data);
   unlock_dos();

}

void read_forward(int board_num,int message_num)
{
  char input[4];
  char *dummy;
  struct mail_pass_data data;
  int num;
  unsigned long int start_entry_num;
  struct bbs_user_account buser;

  if (message_num<0)
  {
      print_cr();
      print_string("Read From Message Number: ");
      do {get_string(input,3); } while (!*input);
      if ( ( num = str_to_num(input,&dummy) ) < 0 )
        { print_str_cr("Aborted.");
          return;
        }
  }
  else
    num = message_num;

  if (!read_bbs_base(board_num,&data,NO_CREATE))
    { print_cr();
      print_str_cr("Error Starting Scan.");
      return;
    }
  start_entry_num = convert_to_entry_num(&data,num);
  if ((start_entry_num<data.entry.tail_entry_no) ||
			(start_entry_num>=data.entry.head_entry_no))
   {  close_base(&data);
	  print_str_cr("Message Number Out of Range");
	  return;
   }


  if (load_bbs_user(data.basename,user_lines[tswitch].number,&buser))
    { unlock_dos();
      print_str_cr(" Unable To Load BBS USER");
      return;
    }

  scan_bbs_messages(board_num, &data, &buser,start_entry_num);


  close_base(&data);

}

void bbs_system(const char *str, const char *name, int portnum)
{  struct bbs_user_account bbs_user;
   int flag=1,num,temp_int;
   int current_board_number=1;
   int board_changed=1;
   char command[10];
   char s[150];
   char current_board_name[BOARD_NAME+1];
   char current_base_directory[FILENAME_LEN];
   char *temp_chr,*point;

   print_string("Entering BBS...");

  while (flag)
  {
    if (board_changed)
      {

        if (!check_for_bbs_board(current_board_number))
          { print_str_cr("--> Sorry: BBS Unavailable");
            return;
          }

        get_new_board_info(current_board_number,current_board_name,
                         current_base_directory);

        if (load_bbs_user(current_base_directory,user_lines[tswitch].number,
                         &bbs_user))
          {  // then he needs a blank account
            create_blank_bbs_acct(&bbs_user);
            if (save_bbs_user(current_base_directory,user_lines[tswitch].number,
                           &bbs_user))
              { print_str_cr(" Unable To add you as a BBS user");
                return;
              }
            next_task();

           }
      }


    check_for_privates();
    sprintf(s,"#|*h1|*f2%02d|*r1 [|*h1|*f2%s|*r1] BBS Command (? for Menu): ",current_board_number,
                       current_board_name);

    print_cr();
    prompt_get_string(s,command,4);
    temp_chr=command;
    while (*temp_chr)
      {*temp_chr = toupper(*temp_chr);
       temp_chr++;
      }

    switch (*command)
      {
        case '?': print_file("help\\BBS.hlp");
                  break;
        case 'J': if ((temp_int=jump_to_new_bbs_board())!=-1)
                    { board_changed=1;
                      current_board_number=temp_int;
                    }
                  break;
        case 'R': if ((num=str_to_num(command+1,&point)) < 0 )
                    read_forward(current_board_number,-1);
                  else
                    read_forward(current_board_number,num);
                  break;
        case 'D':
                  if (test_bit(user_lines[tswitch].privs,BBS_MESSAGE_DELETE_PRV))
                  {
                    delete_bbs_message(current_board_number,-1,-1);
                    board_changed=1;
                  }
                  break;
        case 'S': if (!test_bit(user_lines[tswitch].privs,BBS_BASE_MODIFICATION_PRV))
                      break;
                  sysop_base_configuration(current_board_number,&bbs_user);
                  board_changed=1;
                  break;
        case 'L':
                 switch (*(command+1))
                   {
                     default  :
                     case 'N' : list_bbs_messages(current_board_number,
                                               LIST_NEW_MAIL,NO, &bbs_user);
                                break;
                     case 'A' : list_bbs_messages(current_board_number,
                                               LIST_ALL_MAIL,YES, &bbs_user);
                                break;
                     case 'O' : list_bbs_messages(current_board_number,
                                                    LIST_OLD_MAIL,NO, &bbs_user);
                                break;
                   }

                  break;
        case 'E': if (!user_lines[tswitch].priority)
                    examine_bbs_board();
                  break;
        case 'N': read_new_bbs_message(current_board_number,&bbs_user);
                  break;
        case 'G': global_newscan();
                  board_changed=1; // just so that it loads back in
                                   // the newscan pointers and such
                  break;
        case 'P': post_message_on_board(current_board_number);
                  break;
        case 'T': print_string("Newscan toggled to ");
                  if (toggle_global_newscan(current_board_number,-1))
                    print_str_cr("On");
                  else
                    print_str_cr("Off");
                  board_changed=1;
                  break;
        case 'Q': flag=0;
                  break;
        case 'X': reset_bbs_newscan_pointer(current_board_number);
                  board_changed=1;
                  break;
        default : if ((num=str_to_num(command,&point))>0)
                       read_a_bbs_message(current_board_number,num,&bbs_user);
                  break;

      }
  }

}

