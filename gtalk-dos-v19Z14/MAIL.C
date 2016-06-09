


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#include "include.h"
#include "gtalk.h"
#include "structs.h"

#undef DEBUG_LOCKS

#undef DEBUG
#undef DEBUG_BAD
void undelete_a_message(struct mail_pass_data *data,unsigned long int entry_num);

int mail_base_locks[MAX_THREADS];
int own_mail_base_locks[MAX_THREADS];
int num_mail_base_locks = 0;
int bbs_base_locks[MAX_THREADS];
int own_bbs_base_locks[MAX_THREADS];
int num_bbs_base_locks = 0;

#ifdef DEBUG_LOCKS
int print_locks(int *locks_list, int *owners, int *num_locks)
{
  int flag = !islocked(DOS_SEM);
  int count;
  char s[20];

  if (!flag) unlock_dos();
  print_string("Locks:");
  for (count=0;count<*num_locks;count++)
  {
    sprintf(s," %d/%d",locks_list[count],owners[count]);
    print_string(s);
  }
  print_cr();
  if (!flag) lock_dos(1301);
}
#endif

int check_for_lock(int lock, int *locks_list, int *num_locks)
{
  int count = 0;
  while (count < *num_locks)
  {
    if (*locks_list == lock) return (count);
    locks_list++;
    count++;
  }
  return (-1);
}

int lock_base(int portnum, int lock, int timeout, int *locks_list,
                 int *lock_owner, int *num_locks)
{
  unsigned int init_time = dans_counter;
  int flag = islocked(DOS_SEM);
  int who;

#ifdef DEBUG_LOCKS
  print_locks(locks_list,lock_owner,num_locks);
#endif

  if (*num_locks >= MAX_THREADS) return (0);

  disable();
  unlock_dos();

  while ((timeout == -1) || ((dans_counter - init_time) <= timeout))
  {
    who = check_for_lock(lock,locks_list,num_locks);
    if (who == -1)
    {
      locks_list[*num_locks] = lock;
      lock_owner[*num_locks] = pid_of(portnum);
      (*num_locks)++;
      enable();
#ifdef DEBUG_LOCKS
      print_locks(locks_list,lock_owner,num_locks);
#endif
      if (flag) lock_dos(999);
      return(1);
    } else if (does_pid_exist(lock_owner[who]) == -1)
    {
      lock_owner[who] = pid_of(portnum);
      enable();
#ifdef DEBUG_LOCKS
      print_locks(locks_list,lock_owner,num_locks);
#endif
      if (flag) lock_dos(999);
      return (1);
    }
    enable();
    next_task();
    disable();
  }
  if (flag) lock_dos(999);
  enable();
  return(0);
}

int lock_base_verbose(int lock, int *locks_list,
                int *lock_owner, int *num_locks)
{
  int flag=islocked(DOS_SEM);
  int attempts = 0;
  int key;
  char s[20];

  unlock_dos();
#ifdef DEBUG_LOCKS
  print_str_cr("Lock attempt");
#endif

  while (attempts < 60)
  {
    if (lock_base(tswitch, lock, 18, locks_list, lock_owner, num_locks))
    {
      if (attempts)
        print_cr();

      if (flag) lock_dos(999);
      return (1);
    }
    if (!attempts)
      print_string("Locking Base (Press ESCAPE to abort) [  ]");
    sprintf(s,"\010\010\010\010[%02d]",attempts);
    print_string(s);
    if (get_nchar(tswitch) == 27) attempts = 60;
    attempts++;
  }
  print_cr();

  if (flag) lock_dos(999);
  return (0);
}


int unlock_base(int portnum, int lock, int *locks_list,
                   int *lock_owner, int *num_locks)
{
  int entry;

  disable();
  if ((entry=check_for_lock(lock,locks_list,num_locks)) != -1)
  {
    if (lock_owner[entry] == pid_of(tswitch))
    {
      (*num_locks)--;
      while (entry < *num_locks)
      {
        locks_list[entry] = locks_list[entry+1];
        lock_owner[entry] = lock_owner[entry+1];
        entry++;
      }
      enable();
#ifdef DEBUG_LOCKS
      print_locks(locks_list,lock_owner,num_locks);
#endif
      return (1);
    }
  }
  enable();
  return (0);
}

void free_all_locks(int portnum, int *locks_list, int *lock_owner,
        int *num_locks)
{
  int entry = *num_locks;
  int killpid;
  int move;

  disable();
  killpid = pid_of(portnum);
  while (entry > 0)
  {
    entry--;
    if (lock_owner[entry] == killpid)
    {
      move = entry;
      (*num_locks)--;
      while (move < *num_locks)
      {
        locks_list[move] = locks_list[move+1];
        lock_owner[move] = lock_owner[move+1];
        move++;
      }
    }
  }
  enable();
}

void free_all_bbsmail_locks(int portnum)
{
  free_all_locks(portnum,mail_base_locks,own_mail_base_locks,
                 &num_mail_base_locks);
  free_all_locks(portnum,bbs_base_locks,own_bbs_base_locks,
                 &num_bbs_base_locks);
}

void print_string_n(char *string,int len)
{
  char *end = string+len;

  while ((*string) && (string<end))
    print_chr(*string++);

}


/* OLD MAIL ROUTINES */

void mail_line(char *string, int len, int abs_len, FILE *fileptr)
 {
  char ch;
  int lim=len;
  char *begin=string;
  char *end=string;
  int flag = !islocked(DOS_SEM);
  int loop;

  if (flag) lock_dos(21);

  while ((!feof(fileptr)) && (fgetc(fileptr) != ':'));
  while ( ((ch=fgetc(fileptr)) == ' ') && (!feof(fileptr)));
  while ((ch != 13) && (!feof(fileptr)) && (lim>0) && (abs_len-->0))
   {

     *string++ = ch;
     *string=0;
     lim=len-ansi_strlen(begin);

     ch = fgetc(fileptr);
   };
  while ((ch != 13) && (!feof(fileptr)))
   ch = fgetc(fileptr);

  while ((lim-->0) && (abs_len-->0)) *string++ = ' ';
  *string = 0;

  end=string;
  for (loop=0;loop<3;loop++)
    if (*(end-loop)=='|')
       *(end-loop)=0;

  if (flag) unlock_dos();

 };




/* MAIL.C */

int convert_from_entry_num(struct mail_pass_data *data,unsigned long int entry_num)
{
  return (entry_num - data->entry.tail_entry_no + 1);
}

unsigned long int convert_to_entry_num(struct mail_pass_data *data, long int num)
{
  return (num + data->entry.tail_entry_no - 1);

}


void strcpy_n(char *outstr, char *instr, int length)
{
  while ((*instr) && (length>0))
  {
    *outstr++ = *instr++;
    length--;
  }
  *outstr = 0;
}

int load_base(struct mail_pass_data *data, int create, int mode)
{
  char filename[FILENAME_LEN];
  char temp_fl[FILENAME_LEN];
  int flag=!islocked(DOS_SEM);
  static char modes[][4] = {"rb+", "rb"};
  static char mode_desc[][10] = {"LOADBASE","READBASE"};
  int mode_index=0;

  switch (mode)
  {

   case RW_BASE : mode = RW_BASE;
                  mode_index = 0;
                  break;
   default:
   case R_BASE  : mode = R_BASE;
                  mode_index = 1;
                  break;
  }

#ifdef DEBUG
  unlock_dos();
  print_string("load_base:  ");
  print_str_cr(modes[mode_index]);
#endif

  lock_dos(22);


  sprintf(filename,"%s\\basedata",data->basename);
  if (!(data->fp=g_fopen(filename,modes[mode_index],mode_desc[mode_index])))
  {

    if (!create)
       {if (flag) unlock_dos();
        return (0);
        }
    mkdir(data->basename);
    if (!(data->fp=g_fopen(filename,"wb+","CREBASE")))
     { if (flag) unlock_dos();

       return (0);
     }

    if (data->mail != -1)
    {
      sprintf(temp_fl,"Mailbox #%03d",data->mail);
      strcpy(data->entry.name,temp_fl);
    } else strcpy(data->entry.name,"blank");
    data->entry.head_entry_no = 0;
    data->entry.tail_entry_no = 0;
    data->entry.new_msg_no = 0;
    data->entry.priority = 255;
    data->entry.post_priority = 255;
    data->entry.max_mesg = 50;  //DEBUG
    fwrite(&data->entry,sizeof(struct first_entry),1,data->fp);
    fseek(data->fp,0,SEEK_SET);
    g_fclose(data->fp);

  if (!(data->fp=g_fopen(filename,modes[mode_index],mode_desc[mode_index])))
    {
       if (flag) unlock_dos();
#ifdef DEBUG
       print_str_cr("Can't open after create");
#endif
       return 0;
      }

  }
  fread(&data->entry,sizeof(struct first_entry),1,data->fp);
  data->is_open=1;
  data->open_mode=mode;

  if (flag) unlock_dos();
  return (1);
}

void flush_base(struct mail_pass_data *data)
{
  int flag=!islocked(DOS_SEM);

  if (flag) lock_dos(23);
  fseek(data->fp,0,SEEK_SET);
  fwrite(&data->entry,sizeof(struct first_entry),1,data->fp);
  if (flag) unlock_dos();
}

void close_base(struct mail_pass_data *data)
{
  int flag=islocked(DOS_SEM);

  if ((data->open_mode==RW_BASE) && data->is_open)
    flush_base(data);

#ifdef DEBUG
  unlock_dos();
  print_str_cr("close_base:");
  if (data->is_open) print_str_cr("Base IS open");
  if (data->open_mode==RW_BASE) print_str_cr("Base Open r/w");
  if (flag) lock_dos(24);
#endif

  if (data->is_open)
    {
     g_fclose(data->fp);
     data->is_open=0;
    }
  unlock_dos();
  if (data->mail == -1)
    unlock_base(tswitch,data->bbs_num,bbs_base_locks,
                own_bbs_base_locks,&num_bbs_base_locks);
    else
    unlock_base(tswitch,data->mail,mail_base_locks,
                own_mail_base_locks,&num_mail_base_locks);
}


int open_mail_base(int user_no, struct mail_pass_data *data, int create_mail)
{                 /* Current user #=user_lines[tswitch].number */
  int result;

  if (user_no<0) /* if they are trying to lock a guest base for some reason */
      return(0);

  if (!lock_base_verbose(user_no,mail_base_locks,own_mail_base_locks,
                        &num_mail_base_locks))
       return (0);

  sprintf(data->basename,"MAIL\\MAIL%03d",user_no);
  data->mail = user_no;
  data->bbs_num = -1;

  if (!(result=(load_base(data,create_mail,RW_BASE))))
    unlock_base(tswitch,data->mail,mail_base_locks,
                own_mail_base_locks,&num_mail_base_locks);
  return (result);
}



int open_bbs_base(int bbs_no, struct mail_pass_data *data, int create_bbs)
{                 /* Current user #=user_lines[tswitch].number */
  int result;
  if (!lock_base_verbose(bbs_no,bbs_base_locks,own_bbs_base_locks,
                        &num_bbs_base_locks))
       return (0);

  sprintf(data->basename,"BBS\\BBS%03d",bbs_no);
  data->mail = -1;
  data->bbs_num = bbs_no;
  if (!(result=(load_base(data,create_bbs,RW_BASE))))
    unlock_base(tswitch,data->bbs_num,bbs_base_locks,
                own_bbs_base_locks,&num_bbs_base_locks);
  return (result);
}

int read_bbs_base(int bbs_no, struct mail_pass_data *data, int create_bbs)
{  int ret_val;
#ifdef DEBUG
   int flag = islocked(DOS_SEM);
#endif

   if (!lock_base_verbose(bbs_no,bbs_base_locks,own_bbs_base_locks,
                        &num_bbs_base_locks))
       return (0);

   sprintf(data->basename,"BBS\\BBS%03d",bbs_no);
   data->mail = -1;
   data->bbs_num = bbs_no;
   ret_val = load_base(data,create_bbs,R_BASE);
   data->is_open=0;
   g_fclose(data->fp);

#ifdef DEBUG
   unlock_dos();
   print_str_cr("Base Closed");
   if (flag) lock_dos(25);
#endif

   unlock_base(tswitch,bbs_no,bbs_base_locks,
                own_bbs_base_locks,&num_bbs_base_locks);
   return ret_val;

}

int read_mail_base(int user_no, struct mail_pass_data *data, int create_mail)
{  int ret_val;
#ifdef DEBUG
   int flag = islocked(DOS_SEM);
#endif

  if (!lock_base_verbose(user_no,mail_base_locks,own_mail_base_locks,
                        &num_mail_base_locks))
       return (0);

   sprintf(data->basename,"MAIL\\MAIL%03d",user_no);
   data->mail = user_no;
   data->bbs_num = -1;
   ret_val = load_base(data,create_mail,R_BASE);
   data->is_open=0;
   g_fclose(data->fp);

#ifdef DEBUG
   unlock_dos();
   print_str_cr("Base Closed");
   if (flag) lock_dos(26);
#endif

   unlock_base(tswitch,user_no,mail_base_locks,
                own_mail_base_locks,&num_mail_base_locks);
   return ret_val;
}

int create_temp_file(struct file_entry *file, unsigned int editor, int gen)
{
  int flag=!islocked(DOS_SEM);

  sprintf(file->filename,"TEMP%02d%01d",tswitch,gen);
  if (editor)
  {
    if (flag) lock_dos(27);
    remove(file->filename);
    if (flag) unlock_dos();

    if (!line_editor(file->filename,editor)) return (0);
    if (file->openfile)
     if (!(file->fp=g_fopen(file->filename,"rb+","EDITORFL"))) return (0);
    return (1);
  }
  if (!(file->fp=g_fopen(file->filename,"wb+","EDITORFL"))) return (0);
  return (1);
}

void generate_id(struct mail_pass_data *data, char *filename,
                 struct file_entry *file)
{
  struct ffblk look_up;
  int flag=!islocked(DOS_SEM);
  unsigned int index=dans_counter;
  if (flag) lock_dos(28);

  do
  {
    sprintf(filename,"%02X%04X",sys_info.system_number,(unsigned)
            index++);
    sprintf(file->filename,"%s\\%s",data->basename,filename);
  }
  while (!findfirst(file->filename,&look_up,FA_NORMAL));

  if (flag) unlock_dos();
}

int copy_a_file(struct file_entry *outfile,
                struct file_entry *infile, int append)
{
  char buffer[512];
  int flag=!islocked(DOS_SEM);
  int length;
  int delay;

    if (flag) lock_dos(29);

  if (!infile->openfile)
   if (!(infile->fp=g_fopen(infile->filename,"rb","INCOPY"))) return (0);
  if (!outfile->openfile)
   if (!(outfile->fp=g_fopen((char *)outfile,append ? "ab+" : "wb+","OUTCOPY")))
    {
      if (!infile->openfile) g_fclose(infile->fp);
      return (0);
    }
  do
  {
    length = fread(buffer,sizeof(char),512,infile->fp);
    fwrite(buffer,sizeof(char),length,outfile->fp);
    unlock_dos();
    for (delay=0;delay<STALL_TASKS;delay++) next_task();
    lock_dos(30);
  } while (length);


  if (!infile->openfile) g_fclose(infile->fp);
  if (!outfile->openfile) g_fclose(outfile->fp);

  if (flag) unlock_dos();
  return (1);
}

int scan_for_next_message(struct mail_pass_data *data,
                          struct mesg_entry *mentry)
{
  unsigned long int cur = mentry->entry_no;
  unsigned long int pos;
  struct mesg_entry temp_mentry;
  int flag=!islocked(DOS_SEM);
  int find_tail;
  int find_new;
  unsigned long int real_tail,real_new;


  if (!data->entry.max_mesg)
   {
    log_error("* scan_for_next_message has a NULL max_mesg");
    print_str_cr("Corrupt Mail Base");
    return (0);
   }

  find_tail = (cur == data->entry.tail_entry_no);
  find_new = ((data->mail != 1) && (cur == data->entry.new_msg_no));
  if ((!find_tail) && (!find_new)) return (1);
  real_tail = -1;
  real_new = -1;


  if (flag) lock_dos(31);

  while ((cur<data->entry.head_entry_no) &&
         (((find_tail) && (real_tail == -1))
      || ((find_new) && (real_new == -1))))
  {
    pos = sizeof(struct first_entry)+
           (sizeof(struct mesg_entry)*(cur % data->entry.max_mesg));
    fseek(data->fp,pos,SEEK_SET);
    if (fread(&temp_mentry,sizeof(struct mesg_entry),1,data->fp) == 1)
    {
      if (!temp_mentry.deleted)
      {
        if ((find_tail) && (real_tail == -1)) real_tail = cur;
        if ((!temp_mentry.read) && (find_new) &&
              (real_new == -1)) real_new = cur;
      }
    }
    unlock_dos();
    next_task();
    lock_dos(32);
    cur++;
  }

  if (find_tail)
  {
    if (real_tail != -1) data->entry.tail_entry_no = real_tail;
     else data->entry.tail_entry_no = data->entry.head_entry_no;
  }
  if (find_new)
  {
    if (real_new != -1) data->entry.new_msg_no = real_new;
     else data->entry.new_msg_no = data->entry.head_entry_no;
  }
  fseek(data->fp,0,SEEK_SET);
  fwrite(&data->entry,sizeof(struct first_entry),1,data->fp);

  if (flag) unlock_dos();
  return (1);
}

int add_msg_to_base(struct mail_pass_data *data, int dest_no,
                    struct file_entry *file, struct mesg_entry *mentry,
                    int overwrite)
{
  unsigned long int pos;
  struct mesg_entry mentryold;
  char temp[200];
  char filename[FILENAME_LEN];
  int flag=!islocked(DOS_SEM);

  if (!data->entry.max_mesg)
   {
    log_error("* add_msg_to_base has a NULL max_mesg");
    print_str_cr("Corrupt Mail Base");
    return (0);
   }

  if (!data->entry.max_mesg)
    return (0);

  if (abs((data->entry.head_entry_no - data->entry.tail_entry_no)) > data->entry.max_mesg)
    {     log_error("*TAIL OUT OF WACK IN BBS BASE");
          print_str_cr(" BBS head/tail pointer error, resetting base");
          data->entry.tail_entry_no = data->entry.head_entry_no;
          data->entry.new_msg_no = data->entry.tail_entry_no;
          flush_base(data);
    }

#ifdef DEBUG
  unlock_dos();
  print_str_cr("add_msg_to_base:");
#endif

  if (flag) lock_dos(33);

  pos = sizeof(struct first_entry)+ (sizeof(struct mesg_entry)*
       (data->entry.head_entry_no % data->entry.max_mesg));


  fseek(data->fp,pos,SEEK_SET);
  if (fread(&mentryold,sizeof(struct mesg_entry),1,data->fp) == 1)
  {
    if (!mentryold.deleted)
    {
      if (overwrite)
      {
        sprintf(filename,"%s\\%s",data->basename,mentryold.filename);
        remove(filename);
        mentryold.deleted = 1;
        fseek(data->fp,pos,SEEK_SET);
        fwrite(&mentryold,sizeof(struct mesg_entry),1,data->fp);
        scan_for_next_message(data,&mentryold);
      }
   else
     {  if (flag) unlock_dos();
        return(0);
     }
   }
  }
  fseek(data->fp,pos,SEEK_SET);
  mentry->entry_no = data->entry.head_entry_no;
  mentry->read = 0;
  mentry->deleted = 0;
  mentry->total_erase = 0;
  mentry->ent_date = time(NULL);
  mentry->dest_no = dest_no;
  generate_id(data,mentry->filename,file);
  fwrite(mentry,sizeof(struct mesg_entry),1,data->fp);
  data->entry.head_entry_no++;
  if (file->openfile)
   if (!(file->fp = g_fopen(file->filename,"wb+","ADDMSG")))
    { if (flag) unlock_dos();
      return (0);
    }

if (!user_options[tswitch].priority)
 {
  sprintf(temp,"Head: %lu   Tail: %lu  ",data->entry.head_entry_no,data->entry.tail_entry_no);
  print_str_cr(temp);
 }
  flush_base(data);

#ifdef DEBUG_BAD
  unlock_dos();
  print_str_cr("Successfull");
  lock_dos(34);
#endif
  if (flag) unlock_dos();
  return (1);
}

int delete_msg_from_base(struct mail_pass_data *data,
       unsigned long int mesg_no, int do_erase)
{
  unsigned long int pos;
  struct mesg_entry mentry;
  int flag = !islocked(DOS_SEM);
  char filename[FILENAME_LEN];


  if (!data->entry.max_mesg)
   {
    log_error("* delete_message_from_base has a NULL max_mesg");
    print_str_cr("Corrupt Mail Base");
    return (0);
   }

  pos  = sizeof(struct first_entry)+
       (sizeof(struct mesg_entry)*
       (mesg_no % data->entry.max_mesg));

  if (flag) lock_dos(35);

  fseek(data->fp,pos,SEEK_SET);
  if (fread(&mentry,sizeof(struct mesg_entry),1,data->fp) != 1)
      { if (flag) unlock_dos();
        return (0);
      }
  if ((!mentry.deleted) && (do_erase))
  {
    sprintf(filename,"%s\\%s",data->basename,mentry.filename);
    remove(filename);
    mentry.total_erase = 1;
  }
  mentry.deleted = 1;
  mentry.read = 1;
  fseek(data->fp,pos,SEEK_SET);
  fwrite(&mentry,sizeof(struct mesg_entry),1,data->fp);
  if (data->mail == -1) scan_for_next_message(data,&mentry);
  if (flag) unlock_dos();
  return (1);
}

int undelete_msg_from_base(struct mail_pass_data *data,
       unsigned long int mesg_no, int do_erase)
{
  unsigned long int pos;
  int flag = !islocked(DOS_SEM);
  struct mesg_entry mentry;

  if (!data->entry.max_mesg)
   {
    log_error("* undelete_msg_from_base has a NULL max_mesg");
    print_str_cr("Corrupt Mail Base");
    return (0);
   }

  pos = sizeof(struct first_entry)+
       (sizeof(struct mesg_entry)*
       (mesg_no % data->entry.max_mesg));


  if (data->mail == -1) return (0);

  if (flag) lock_dos(36);

  fseek(data->fp,pos,SEEK_SET);
  if (fread(&mentry,sizeof(struct mesg_entry),1,data->fp) != 1)
     { if (flag) unlock_dos();
       return (0);
     }
  if (!mentry.deleted)
     {  if (flag) unlock_dos();
        return (1);
     }

  if (mentry.total_erase)
    { if (flag) unlock_dos();
      return (0);
    }
  mentry.deleted = 0;
  fseek(data->fp,pos,SEEK_SET);
  fwrite(&mentry,sizeof(struct mesg_entry),1,data->fp);
  if (flag) unlock_dos();

  return (1);
}

int get_entry(struct mail_pass_data *data, unsigned long int mesg_no,
     struct mesg_entry *mentry, struct file_entry *file)
{
  unsigned long int pos;
  int flag = !islocked(DOS_SEM);
  int need_to_open_base = (!(data->is_open));
  char s[FILENAME_LEN];


  if (!data->entry.max_mesg)
    {
     log_error("* get_entry ERROR, NULL max_mesg");
     print_str_cr("Corrupt Mailbox");
     return (0);
    }

  pos = sizeof(struct first_entry)+
       (sizeof(struct mesg_entry)*
       (mesg_no % data->entry.max_mesg));


  sprintf(s,"%s\\basedata",data->basename);
  if (flag) lock_dos(37);

  if (need_to_open_base)
  if (!(data->fp=g_fopen(s,"rb","GET_ENTRY")))
       { if (flag) unlock_dos();
         return 0;
       }

  fseek(data->fp,pos,SEEK_SET);

  if (fread(mentry,sizeof(struct mesg_entry),1,data->fp) != 1)
     { if (flag) unlock_dos();
       if (need_to_open_base)
              g_fclose(data->fp);
       return (0);
     }

  sprintf(file->filename,"%s\\%s",data->basename,mentry->filename);

  if (file->openfile)
    if (!(file->fp=g_fopen(file->filename,"rb","GETBBS")))
       { if (flag) unlock_dos();
         if (need_to_open_base)
              g_fclose(data->fp);
         return (0);
       }

  if (flag) unlock_dos();
  if (need_to_open_base)
       g_fclose(data->fp);
  return (1);
}


int read_entry(struct mail_pass_data *data, unsigned long int mesg_no,
     struct mesg_entry *mentry, struct file_entry *file)
{

  unsigned long int pos;
  int flag = !islocked(DOS_SEM);
  int need_to_open_base = !(data->is_open);
  char s[FILENAME_LEN];


  if (!data->entry.max_mesg)
    {
     log_error("* read_entry ERROR, NULL max_mesg");
     print_str_cr("Corrupt Mailbox");
     return (0);
    }

  pos = sizeof(struct first_entry)+
       (sizeof(struct mesg_entry)*
       (mesg_no % data->entry.max_mesg));

  sprintf(s,"%s\\basedata",data->basename);

  if (flag) lock_dos(38);

  if (need_to_open_base)
  if (!(data->fp=g_fopen(s,"rb+","READ_ENTRY")))
       { if (flag) unlock_dos();
         return;
       }


  fseek(data->fp,pos,SEEK_SET);
  if (fread(mentry,sizeof(struct mesg_entry),1,data->fp) != 1)
      { if (flag) unlock_dos();
        if (need_to_open_base)
             g_fclose(data->fp);
        return (0);
      }
  sprintf(file->filename,"%s\\%s",data->basename,mentry->filename);
  if (!mentry->deleted)
  {
    if (data->mail != -1)
    {
      mentry->read = 1;
      fseek(data->fp,pos,SEEK_SET);
      fwrite(mentry,sizeof(struct mesg_entry),1,data->fp);
    }
    scan_for_next_message(data,mentry);
  }
  if (file->openfile)
    if (!(file->fp=g_fopen(file->filename,"rb","READ_ENTRY2")))
     { if (flag) unlock_dos();
       if (need_to_open_base)
             g_fclose(data->fp);
        return (0);
      }

  if (flag) unlock_dos();

  if (need_to_open_base)
     g_fclose(data->fp);

  return (1);
}

void compress_base(struct mail_pass_data *data,char echo)
{
  unsigned long int new_tail = data->entry.head_entry_no;
  unsigned long int move_tail = new_tail;
  unsigned long int pos, pos2;
  int flag = !islocked(DOS_SEM);
  int taskcount;
  struct mesg_entry mentry;
  char filename[FILENAME_LEN];
  int found = 1;

  if (!data->entry.max_mesg)
   {
    log_error("* compress_base has a NULL max_mesg");
    print_str_cr("Corrupt Mail Base");
    return (0);
   }


  while ((move_tail > data->entry.tail_entry_no) && (found))
  {
    unlock_dos();
    next_task();
    if (echo)
      print_chr(echo);
    new_tail--;
    if (move_tail > new_tail) move_tail = new_tail;
    pos = sizeof(struct first_entry)+
       (sizeof(struct mesg_entry)*
       (new_tail % data->entry.max_mesg));
    lock_dos(999);
    fseek(data->fp,pos,SEEK_SET);
    if (fread(&mentry,sizeof(struct mesg_entry),1,data->fp)==1)
    {
      if (mentry.deleted)
      {
        if (!mentry.total_erase)
        {
          sprintf(filename,"%s\\%s",data->basename,mentry.filename);
          remove(filename);
        }
        found = 0;
        while ((move_tail > data->entry.tail_entry_no) && (!found))
        {
          unlock_dos();
          for(taskcount=0;taskcount<100;taskcount++)
             next_task();
          lock_dos(999);
          move_tail--;
          pos2 = sizeof(struct first_entry)+
           (sizeof(struct mesg_entry)*
           (move_tail % data->entry.max_mesg));
          fseek(data->fp,pos2,SEEK_SET);
          if (fread(&mentry,sizeof(struct mesg_entry),1,data->fp)==1)
          {
            if (!mentry.deleted)
            {
              unlock_dos();
              if (echo)
                print_chr(echo);
              lock_dos(999);
              mentry.entry_no = new_tail;
              fseek(data->fp,pos,SEEK_SET);
              fwrite(&mentry,sizeof(struct mesg_entry),1,data->fp);
              mentry.deleted = 1;
              mentry.read = 1;
              mentry.total_erase = 1;
              fseek(data->fp,pos2,SEEK_SET);
              fwrite(&mentry,sizeof(struct mesg_entry),1,data->fp);
              if (move_tail == data->entry.new_msg_no)
                   data->entry.new_msg_no = new_tail;

              if (data->entry.tail_entry_no == move_tail)
                  data->entry.tail_entry_no = new_tail;

              found = 1;
            }
            else
              if (!mentry.total_erase)
                {
                  sprintf(filename,"%s\\%s",data->basename,mentry.filename);
                  remove(filename);
                  mentry.total_erase = 1;
                  fseek(data->fp,pos2,SEEK_SET);
                  fwrite(&mentry,sizeof(struct mesg_entry),1,data->fp);
                }
          }
        }
      }
    }
  }
//  if (found) data->entry.tail_entry_no = new_tail;

  if (flag) unlock_dos();
  mentry.entry_no = data->entry.tail_entry_no;
  scan_for_next_message(data,&mentry);
}

void print_fifo_info(struct mail_pass_data *data)
{
  char s[160];


  sprintf(s,"Basename: %s",data->basename);
  print_str_cr(s);
  sprintf(s,"Mail:     %d",data->mail);
  print_str_cr(s);
  sprintf(s,"Max mesg: %d",data->entry.max_mesg);
  print_str_cr(s);
  sprintf(s,"Priority: %d",data->entry.priority);
  print_str_cr(s);
  sprintf(s,"Head/Tail/New (entrynum): %ld %ld %ld",data->entry.head_entry_no,
        data->entry.tail_entry_no,data->entry.new_msg_no);
  print_str_cr(s);
  sprintf(s,"Name:     %s",data->entry.name);
  print_str_cr(s);
  sprintf(s,"Dir:      %s",data->basename);
  print_str_cr(s);

}


void examine_board(void)
{
  struct mail_pass_data data;
  char *dat;
  char s[7];
  int user_num;

  print_cr();
  print_string("Which user number to read: ");
  get_editor_string(s,5);
  user_num=str_to_num(s,&dat);

  if (!open_mail_base(user_num,&data,0)) return;

  print_fifo_info(&data);

  close_base(&data);
}

void notify_user(int user_no)
{
 int node = is_user_online(user_no);

 if (node<0) return;

   aput_into_buffer(node,"--> \007\007|*f1New Mail|*r1 Has Arrived",255,12,tswitch,0,0);

}


int send_mail_to(struct mesg_entry *mentry, struct file_entry *file,
     int user_no)
{
  struct file_entry file2;
  struct mail_pass_data data;
  int error = 1;
  int error_code=0;
  int flag = !islocked(DOS_SEM);
  char s[80];

#ifdef DEBUG
  print_str_cr("send_mail_to:");
#endif

  file2.openfile = 1;
  lock_dos(40);

  if (!open_mail_base(user_no,&data,CREATE))
     { error = 0;
       error_code = 1;
     }

   else if (!add_msg_to_base(&data,0,&file2,mentry,1))
           { error = 0;
             error_code = 2;
           }
    else
    {
      fprintf(file2.fp,"|*h1|*f4From:     |*f7");
      if (user_no < 0) fprintf(file2.fp,"(%%GST)");
       else fprintf(file2.fp,"(#%03d)",mentry->user_no);
      fprintf(file2.fp," %s|*r1\r\n",mentry->username);
      fprintf(file2.fp,"|*h1|*f4System:   |*f7(MAIL) Gtalk-%02d:%s|*r1\r\n",
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

#ifdef DEBUG
  switch (error_code)
   {
   case 1:  print_str_cr(" Could Not open base for writing");
            break;
   case 2:  print_str_cr(" Could not add_msg_to_base");
            break;
   default: if (error) print_str_cr(" No Error");
            print_string("Filename : ");
            print_str_cr(file2.filename);
            break;
   }
#endif
  if (error) notify_user(user_no);
  return (error);
}

int send_mail_subj(int user_no)
{
  int error = 1;
  struct mesg_entry mentry;
  struct file_entry file;
  struct user_data temp_data;
  int flag = !islocked(DOS_SEM);
  char s[200];

  if ((user_no>=0) && (user_no<=999))
   if (!load_user(user_no,&temp_data)) error = 0;

  if ((error) || (temp_data.number<0))
  {
    print_str_cr("That user does NOT EXIST");
    return (0);
  }
  print_cr();
  special_code(1,tswitch);
  sprintf(s,"|*f4|*h1     To: |*f7(#%03d) %c|*r1%s|*r1|*f7|*h1%c ",user_no,temp_data.staple[2],temp_data.handle,temp_data.staple[3]);
  print_str_cr(s);
  print_string("Subject: ");
  special_code(0,tswitch);
  get_string_cntrl(mentry.subject,SUBJECT_LEN,0,0,0,0,1,0,0);
  print_cr();
  if (!(*mentry.subject))
     return (0);
  file.openfile = 0;
  if (!create_temp_file(&file,MAX_EDITOR_LEN,0))
     { print_str_cr("Mail Aborted.");
       return (0);
     }
  mentry.dest_user_no = user_no;
  mentry.system_no = sys_info.system_number;
  mentry.user_no = user_lines[tswitch].number;
  mentry.dest_no = sys_info.system_number;
  strcpy_n(mentry.username,user_lines[tswitch].handle,USER_NAME_LEN);
  strcpy_n(mentry.systemname,sys_info.system_name,SYSTEM_NAME_LEN);
  if (flag)  lock_dos(41);
  mentry.ent_date=time(NULL);
  if (flag)  unlock_dos();
  send_mail_to(&mentry,&file,user_no);
  return (1);
}

void send_mail_message(void)
{
  char s[10];
  char *data;
  int user_num;

  print_cr();
  print_string("Which user number to send to: ");
  get_editor_string(s,5);
  user_num=str_to_num(s,&data);
  send_mail_subj(user_num);
}

int read_single_message(struct mail_pass_data *data,
                                   unsigned long int message_no)
{
  struct mesg_entry mentry;
  struct file_entry file;
  char s[80];
  int flag = !islocked(DOS_SEM);

  if ((message_no<data->entry.tail_entry_no) ||
                (message_no>=data->entry.head_entry_no))
    {print_str_cr(" No Such Message");
     return 0;
    }

    file.openfile=0;
    lock_dos(42);
    if (!read_entry(data,message_no,&mentry,&file))
      { unlock_dos();
        print_str_cr("Read Failed");
        if (!flag) lock_dos(43);
        return -1;
      }

    unlock_dos();

    if (mentry.deleted)
     {
           if (data->mail<0)  // IF it's a BBS message
             {
              print_cr();
              sprintf(s,"|*h1|*f4Message |*f7%02d |*f4of |*f7%02d|*r1 |*f1|*h1DELETED",
                 convert_from_entry_num(data,message_no),
                 convert_from_entry_num(data,data->entry.head_entry_no) - 1);
              special_code(1,tswitch);
              print_str_cr(s);
              special_code(0,tswitch);
              if (!flag) lock_dos(44);
              return (message_no);
             }

           print_string(" That message is deleted, undelete (y/n) ? ");
           do
            { get_string(s,2);
            } while (!*s);
           if (toupper(*s) == 'Y')
                { print_str_cr("  --> UNDELETED <--");
                  undelete_a_message(data,message_no);
                }
           else
           {
           if (!flag) lock_dos(45);
           return 0;
           }
      }

      print_cr();
      sprintf(s,"|*h1|*f4Message   |*f7%02d |*f4of |*f7%02d|*r1",
         convert_from_entry_num(data,message_no),
         convert_from_entry_num(data,data->entry.head_entry_no) - 1);
      special_code(1,tswitch);
      print_str_cr(s);
      special_code(0,tswitch);

   // print_str_cr(file.filename);
      print_file_to_cntrl(file.filename,tswitch,1,1,1,MAIL_PAGING);
      g_fclose(file.fp);
      if (!flag) lock_dos(46);
    return 1;
}


long int read_new_message(void)
{
  struct mail_pass_data data;
  long int return_value;
  unsigned long int message_no;


  if (!read_mail_base(user_lines[tswitch].number,&data,0))
    { print_str_cr("Base Read Error");
      return -1;
    }

  message_no = data.entry.new_msg_no;

  if ((message_no<data.entry.tail_entry_no) ||
                  (message_no>=data.entry.head_entry_no))
     return_value = -1;
  else
    {
       return_value =  data.entry.new_msg_no;

     read_single_message(&data,message_no);
    }



  return ((long int)(return_value));
}



long int read_a_mail_message(int num)
{
  struct mail_pass_data data;
  int success;
  unsigned long int entry_num;

  if (!read_mail_base(user_lines[tswitch].number,&data,0))
    { print_str_cr("Base Read Error");
      return -1;
    }

  entry_num=convert_to_entry_num(&data,num);

  success = read_single_message(&data,entry_num);
  close_base(&data);

  if (success)
    return entry_num;
  else
    return -1;
}

long int read_mail_message(void)
{
    char s[5];
    int num;
    char *dummy;

    print_cr();
    print_string("Enter Number to Read: ");
    get_string(s,4);
    num=str_to_num(s,&dummy);
    if (num>=0)
       return (read_a_mail_message(num));
    else
      return -1;
}

void delete_mail(long int entry_no)
{
  char command[7];
  int num;
  char *point;
  struct mail_pass_data data;
  int flag = !islocked(DOS_SEM);

  if (entry_no==-1)
  {
      prompt_get_string("Delete message: ",command,4);
      num = str_to_num(command,&point);
  }

  if ((num>0) || (entry_no!=-1))
  {
    if (flag) lock_dos(47);
    open_mail_base(user_lines[tswitch].number,&data,0);

    if (entry_no==-1)
       entry_no = convert_to_entry_num(&data,num);

    if (num<=data.entry.max_mesg)
       delete_msg_from_base(&data,entry_no,0);

    close_base(&data);
    if (flag) unlock_dos();
  }
}


void undelete_a_message(struct mail_pass_data *data,unsigned long int entry_num)
{
  int flag = !islocked(DOS_SEM);
  char filename[FILENAME_LEN];

    sprintf(filename,"%s\\basedata",data->basename);

    lock_dos(49);

    if ((data->fp = g_fopen(filename,"rb+","UNDEL")))
      {
       undelete_msg_from_base(data,entry_num,NO);
       g_fclose(data->fp);
      }
    else
     { unlock_dos();
       print_str_cr("Message Un-deletion failed");
       lock_dos(50);
     }
    if (flag) unlock_dos();

}

void undelete_mail(void)
{
  char command[7];
  int num;
  char *point;
  struct mail_pass_data data;

  prompt_get_string("Un-delete Which message # ? : ",command,4);
  if ((num=str_to_num(command,&point))<0)
    { print_str_cr("Message Out of Range");
      return;
    }

  lock_dos(51);
  if (read_mail_base(user_lines[tswitch].number,&data,NO_CREATE))
   {
     undelete_a_message(&data,convert_to_entry_num(&data,num));
     close_base(&data);
     unlock_dos();
   }
   else
   { unlock_dos();
     print_str_cr("Message Un-Deletion Failed");
    }

}

void print_str_nchr(char *string, int length)
{
  while ((*string) && (length>0))
  {
    print_chr(*string++);
    length--;
  }
  while (length>0)
  {
    print_chr(' ');
    length--;
  }
}
char *scan_str_for_chr(char *string,char character)
{
    char *begin=string;

    while ((*string) && (*string!=character))
      string++;

    if (*string)
      return (string);
    else
      return (begin);

}

void truncate_string(char *string)
{
  while (*string)
   string++;

  while (*(string-1)==' ')
    string--;
  *string=0;
}


void do_assimilation(struct mail_pass_data *data,char *file_pattern)
{
  char wildcard[120];
  struct mesg_entry mentry;
  struct file_entry in_file;
  struct file_entry dest_file;
  struct ffblk file_ptr;
  int isfile;
  char temp_str[100];
  char *dummy;

  sprintf(wildcard,"%s\\%s",data->basename,file_pattern);

  mentry.dest_user_no = user_lines[tswitch].number;
  mentry.system_no = sys_info.system_number;
  mentry.user_no = 999;
  mentry.dest_no = sys_info.system_number;
  strcpy_n(mentry.username,"*OLD* Mail",USER_NAME_LEN);
  strcpy_n(mentry.subject,"Mail From Old Mail System",SUBJECT_LEN);
  strcpy_n(mentry.systemname,sys_info.system_name,SYSTEM_NAME_LEN);

  lock_dos(52);
  isfile = findfirst(wildcard,&file_ptr, FA_NORMAL);
  while((!isfile))
   {
     unlock_dos();
     print_string("               Adding: ");
     print_string(file_ptr.ff_name);

     sprintf(in_file.filename,"%s\\%s",data->basename,file_ptr.ff_name);
      if ((in_file.fp = g_fopen(in_file.filename,"rb+","ROLDMAIL")))
      { char *end;
        mail_line(temp_str,USER_NAME_LEN-5,USER_NAME_LEN-5,in_file.fp);
        strcpy(mentry.username,scan_str_for_chr(temp_str,')')+1);
        truncate_string(mentry.username);
        mentry.user_no = str_to_num(scan_str_for_chr(temp_str,'#')+1,&dummy);
        mail_line(mentry.subject,SUBJECT_LEN-5,SUBJECT_LEN-5,in_file.fp);
        truncate_string(mentry.subject);
        g_fclose(in_file.fp);

        // truncate the garbage off the subject

        end = mentry.subject;
        while (*end)
          end++;
        while ((*(end-1)=='|') || (*(end-2)=='|' && *(end-1)=='*') ||
             (*(end-3)=='|' && *(end-2)=='*'))
          end--;
        *end=0;

      }
     else
      {

        strcpy_n(mentry.username,"Unknown User",USER_NAME_LEN);
        mentry.user_no = 999;
        strcpy_n(mentry.subject,"Unknown Subject",SUBJECT_LEN);
      }

     in_file.openfile=0;
     lock_dos(53);
     dest_file.openfile=0;
     if (!add_msg_to_base(data,sys_info.system_number, &dest_file, &mentry,0))
        { unlock_dos();
          print_str_cr("   Error Adding Mail");
        }
     else
        {
         dest_file.openfile=0;

         copy_a_file(&dest_file,&in_file,0);
         unlock_dos();
         print_string("     As: ");
         print_str_cr(mentry.filename);
        }

     next_task();
     lock_dos(54);
     isfile = findnext(&file_ptr);
   }
   unlock_dos();
}



void assimilate_old_mail(int user_number,struct mail_pass_data *data)
{

  if (!open_mail_base(user_lines[tswitch].number,data,CREATE))
   {
     print_str_cr("--> Error Creating New Mail Index");
     return;
   }

  if (user_lines[tswitch].priority)
    data->entry.max_mesg=60;
  else
    data->entry.max_mesg=350;

  print_string("  Operating on Directory: ");
  print_str_cr(data->basename);
  print_str_cr("   - Deleting Extra Files");

  delete_files_function(data->basename,"0*.*");

  print_str_cr("   - Add OLD mail");
  do_assimilation(data,"OLD*.");

  print_str_cr("   - Add NEW mail");
  do_assimilation(data,"MSG*.");

  print_str_cr("  DONE.");
  close_base(data);

}

void print_string_of_len(char *string, int len)
{
  int temp_len;

  if (ansi_strlen(string)>len)
   {
    //print_string_n(string,len);
    print_string(string);
    return;
   }
  else
   {
     temp_len = ansi_strlen(string);
     print_string(string);
   }

   repeat_chr(' ',len-temp_len,0);
}

void list_mail_special(int mode,int show_deleted)
{

  struct mail_pass_data data;
  struct mesg_entry mentry;
  struct file_entry file;
  unsigned long int cur;
  unsigned long int stop_message_number;
  int showed_message=NO;
  char s[80];
  int num_lines=20;
  int cur_line=0;
  int key_hit=-1;
  int abort=0;
  int show_message=0;
  int flag = !islocked(DOS_SEM);
  int entry_read;

  if (flag) lock_dos(55);

  if (!read_mail_base(user_lines[tswitch].number,&data,NO_CREATE))
  {
    if (flag) unlock_dos();
    print_str_cr("--> No mail index.. assimilating old mail");
    assimilate_old_mail(user_lines[tswitch].number,&data);
    return;
  }

  switch (mode)
    {
     case LIST_NEW_MAIL :  cur = data.entry.new_msg_no;
                           stop_message_number = data.entry.head_entry_no;
                           break;
     default:
     case LIST_OLD_MAIL :
     case LIST_ALL_MAIL :  cur = data.entry.tail_entry_no;
                           stop_message_number = data.entry.head_entry_no;
                           break;

     /*    THIS IS BAD IDEA
      *
      *      case LIST_OLD_MAIL :  cur = data.entry.tail_entry_no;
      *                            stop_message_number = data.entry.new_msg_no;
      *                            break;
      */
    }





  while ((cur<stop_message_number) && (!abort))
  {

    file.openfile = 0;
    entry_read = get_entry(&data,cur,&mentry,&file);

    show_message=1;

    switch(mode)
    {
     case LIST_NEW_MAIL :  if (mentry.read)
                             show_message=0;
                           break;
     case LIST_OLD_MAIL :  if (!mentry.read)
                             show_message=0;
                           break;
     default:
     case LIST_ALL_MAIL :  break;
    }

    if ((!(mentry.deleted) || show_deleted) && show_message)
    {
        unlock_dos();
        if (!entry_read) print_str_cr("Bad Read");
        showed_message=YES;

        special_code(1,tswitch);
        if (mentry.read)
          print_string("O ");
        else
          print_string("N ");

        truncate_string(mentry.username);
        truncate_string(mentry.subject);

        if (mentry.user_no<0)
           sprintf(s,"|*h1|*f4%02d  |*f7(%%GST) ", convert_from_entry_num(&data,cur));
        else
           sprintf(s,"|*h1|*f4%02d  |*f7(#%03d) ", convert_from_entry_num(&data,cur),mentry.user_no);
        print_string(s);

        print_string_of_len(mentry.username,20);

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
    }
    else
      unlock_dos();

    while ((key_hit=int_char(tswitch))!=-1)
      if ((key_hit==27) || (key_hit==32))
        abort=1;

    next_task();
    lock_dos(56);
    cur++;
  }
  close_base(&data);

  if (!showed_message)
   { unlock_dos();
     print_cr();
     switch (mode)
     {
       case LIST_NEW_MAIL: print_str_cr("No New Mail");
                           break;
       default:
       case LIST_ALL_MAIL: print_str_cr("No Mail to Display");
                           break;
       case LIST_OLD_MAIL: print_str_cr("No Old Mail to Display");
                           break;

      }
    print_cr();
    lock_dos(57);
   }

   if (flag) unlock_dos();
}

void send_mail(void)
{
}

void user_feedback(char *string, char *name, int portnum)
{
    send_mail_subj(0);
}

void is_new_mail(void)
{
  struct mail_pass_data data;
  int flag = !islocked(DOS_SEM);
  int result;

  if (user_lines[tswitch].number < 0)
      return;


  if (flag) lock_dos(65);

  if (!read_mail_base(user_lines[tswitch].number,&data,NO_CREATE))
    { unlock_dos();
      print_str_cr("NO MAIL INDEX!");
      if (!flag) lock_dos(66);
      return;
    }

   if (data.entry.new_msg_no<data.entry.head_entry_no)
           result = YES;
   else
           result = NO;
   close_base(&data);
   unlock_dos();

   if (result)
    {
     special_code(1,tswitch);
     print_str_cr("|*f4|*h1*** You have |*f1NEW|*f4 mail ***|*r1");
     special_code(0,tswitch);
    }
   else
     print_str_cr("No New Mail");
   if (!flag) lock_dos(67);

}

void auto_reply_to_message(unsigned long int entry_num)
{
  char s[10];
  struct mail_pass_data data;
  struct mesg_entry mentry;
  struct file_entry file;
  int message_num;

  open_mail_base(user_lines[tswitch].number,&data,0);

  message_num = convert_from_entry_num(&data,entry_num);

  print_string(" --> Auto Reply to Message #");
  sprintf(s,"%d",message_num);
  print_string(s);
  print_str_cr(" <--");
  file.openfile=0;
  read_entry(&data,entry_num,&mentry,&file);

  close_base(&data);

  send_mail_subj(mentry.user_no);

  print_cr();
  if (get_hot_key_prompt("Delete original Message? (y/n) ","YN",'Y',1) == 'Y')
    delete_mail(entry_num);

  return;
}

void auto_reply(void)
{
    print_string(" Enter Message # to Auto Reply to: ");
    print_str_cr("UNFINISHED");
    return;
}

void mail_system(const char *str,const char *name, int portnum)
{
  int flag = 1;
  char command[7];
  int num;
  long int temp;
  unsigned long int last_message_read = 0;
  int  has_read_message = NO;
  int  need_to_compress = 0;
  int  read_temp;
  char last_command[7] = "";
  char *point;
  char *temp_chr;
  struct mail_pass_data data;


  list_mail_special(LIST_NEW_MAIL,NO);

  while (flag)
  {
    check_for_privates();
    print_cr();
    prompt_get_string("Mail Command (|*h1?|*h0 for Menu): ",command,5);
    temp_chr=command;
    while (*temp_chr)
      {*temp_chr = toupper(*temp_chr);
       temp_chr++;
      }

    read_temp = 0;

    switch (*command)
      {
        case 'A': if (has_read_message)
                    {  auto_reply_to_message(last_message_read);
                       need_to_compress = YES;
                    }
                  else
                     print_str_cr(" No Current Message");
                  break;
        case 'R': temp = read_mail_message();
                  if (temp!=-1)
                   { read_temp = YES;
                    last_message_read = (unsigned long int) temp;
                   }
                  break;
        case 'D': if ((*(command+1)=='L') && has_read_message)
                   { // DELETE LAST MESSAGE
                     delete_mail(last_message_read);
                     print_str_cr("Last Message Read Deleted.");
                     need_to_compress = YES;
                     break;
                   }

                  if ((num = str_to_num((command+1),&point))<0)
                     delete_mail(-1);
                  else
                     if (read_mail_base(user_lines[tswitch].number,&data,0))
                         delete_mail(convert_to_entry_num(&data,num));
                     else
                        print_str_cr("Mail Deletion Error");
                  need_to_compress = YES;
                  break;
        case 'U': undelete_mail();
                  break;
        case 'Q': flag = 0;
                  break;
        case 'S': temp = str_to_num(command+1,&point);
                  if (temp!=-1)
                    send_mail_subj(temp);
                  else
                    send_mail_message();
                  break;
        case 'N': temp = read_new_message();
                  if (temp!=-1)
                   { read_temp = YES;
                    last_message_read = (unsigned long int) temp;
                   }
                  break;
        case 'E': if (!user_lines[tswitch].priority)
                       examine_board();
                  break;
        case 'L': switch (*(command+1))
                   {
                     default  :
                     case 'N' : list_mail_special(LIST_NEW_MAIL,NO);
                                break;
                     case 'A' : list_mail_special(LIST_ALL_MAIL,YES);
                                break;
                     case 'O' : list_mail_special(LIST_OLD_MAIL,NO);
                                break;
                   }
                  break;
        case '?': print_file("help\\mail.hlp");
                  break;
        default : if ((num=str_to_num(command,&point))>0)
                       temp = read_a_mail_message(num);
                  if (temp!=-1)
                   { read_temp = YES;
                    last_message_read = (unsigned long int) temp;
                   }
                  break;
      }
      if (read_temp)
       has_read_message = YES;
      else
       has_read_message = NO;
      strncpy(last_command,command,6);
      last_command[6]=0;
  }

  if (need_to_compress)
  {
      print_string("Purging Deleted Messages.");

      if (open_mail_base(user_lines[tswitch].number,&data,NO_CREATE))
       {compress_base(&data,'.');
        close_base(&data);
        print_str_cr("Done.");
       }
  }
}


