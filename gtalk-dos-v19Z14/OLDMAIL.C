


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#include "include.h"
#include "gtalk.h"
#include "structs.h"

#undef DEBUG

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

  if (flag) lock_dos();

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
  int flag=!islocked(DOS_SEM);
  char modes[][4] = {"rb+", "rb"};
  char mode_desc[][10] = {"LOADBASE","READBASE"};
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

  lock_dos();


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
      sprintf(filename,"Mailbox #%03d",data->mail);
      strcpy(data->entry.name,filename);
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

  if (flag) lock_dos();
  fseek(data->fp,0,SEEK_SET);
  fwrite(&data->entry,sizeof(struct first_entry),1,data->fp);
  if (flag) unlock_dos();
}

void close_base(struct mail_pass_data *data)
{
  int flag=!islocked(DOS_SEM);

  if ((data->open_mode==RW_BASE) && data->is_open)
    flush_base(data);

#ifdef DEBUG
  unlock_dos();
  print_str_cr("close_base:");
  if (data->is_open) print_str_cr("Base IS open");
  if (data->open_mode==RW_BASE) print_str_cr("Base Open r/w");
  if (flag) lock_dos();
#endif

  if (data->is_open)
    {
     g_fclose(data->fp);
     data->is_open=0;
    }
}

int open_mail_base(int user_no, struct mail_pass_data *data, int create_mail)
{                 /* Current user #=user_lines[tswitch].number */
  sprintf(data->basename,"MAIL\\MAIL%03d",user_no);
  data->mail = user_no;
  return (load_base(data,create_mail,RW_BASE));
}


int open_bbs_base(int bbs_no, struct mail_pass_data *data, int create_bbs)
{                 /* Current user #=user_lines[tswitch].number */
  sprintf(data->basename,"BBS\\BBS%03d",bbs_no);
  data->mail = -1;
  return (load_base(data,create_bbs,RW_BASE));
}

int read_bbs_base(int bbs_no, struct mail_pass_data *data, int create_bbs)
{  int ret_val;

   sprintf(data->basename,"BBS\\BBS%03d",bbs_no);
   data->mail = -1;
   ret_val = load_base(data,create_bbs,R_BASE);

   return ret_val;

}

int read_mail_base(int user_no, struct mail_pass_data *data, int create_mail)
{  int ret_val;

   sprintf(data->basename,"MAIL\\MAIL%03d",user_no);
   data->mail = user_no;
   ret_val = load_base(data,create_mail,R_BASE);
   return ret_val;
}

int create_temp_file(struct file_entry *file, unsigned int editor, int gen)
{
  int flag=!islocked(DOS_SEM);

  sprintf(file->filename,"TEMP%02d%01d",tswitch,gen);
  if (editor)
  {
    if (flag) lock_dos();
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

  if (flag) lock_dos();

  do
  {
    sprintf(filename,"%02X%04X",sys_info.system_number,(unsigned)
            dans_counter);
    sprintf(file->filename,"%s\\%s",data->basename,filename);
    unlock_dos();
    delay(1);
    lock_dos();
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


  if (!infile->openfile)
   if (!(infile->fp=g_fopen(infile->filename,"rb","INCOPY"))) return (0);
  if (!outfile->openfile)
   if (!(outfile->fp=g_fopen((char *)outfile,append ? "a+" : "wb+","OUTCOPY")))
    {
      if (!infile->openfile) g_fclose(infile->fp);
      return (0);
    }
  do
  {
    if (flag) lock_dos();
    length = fread(buffer,sizeof(char),512,infile->fp);
    fwrite(buffer,sizeof(char),length,outfile->fp);
    unlock_dos();
    for (delay=0;delay<STALL_TASKS;delay++) next_task();
    lock_dos();
  } while (length);

  if (flag) unlock_dos();

  if (!infile->openfile) g_fclose(infile->fp);
  if (!outfile->openfile) g_fclose(outfile->fp);
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

  find_tail = (cur == data->entry.tail_entry_no);
  find_new = ((data->mail != 1) && (cur == data->entry.new_msg_no));
  if ((!find_tail) && (!find_new)) return (1);
  real_tail = -1;
  real_new = -1;


  if (flag) lock_dos();

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
    lock_dos();
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
  char filename[FILENAME_LEN];
  int flag=!islocked(DOS_SEM);


  if (!data->entry.max_mesg)
    return (0);

#ifdef DEBUG
  unlock_dos();
  print_str_cr("add_msg_to_base:");
#endif

  pos = sizeof(struct first_entry)+ (sizeof(struct mesg_entry)*
       (data->entry.head_entry_no % data->entry.max_mesg));

  lock_dos();

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
#ifdef DEBUG
  unlock_dos();
  print_str_cr("Successfull");
  lock_dos();
#endif

  if (flag) unlock_dos();
  return (1);
}

int delete_msg_from_base(struct mail_pass_data *data,
       unsigned long int mesg_no, int do_erase)
{
  unsigned long int pos = sizeof(struct first_entry)+
       (sizeof(struct mesg_entry)*
       (mesg_no % data->entry.max_mesg));
  struct mesg_entry mentry;
  int flag = !islocked(DOS_SEM);
  char filename[FILENAME_LEN];

  if (flag) lock_dos();

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
  unsigned long int pos = sizeof(struct first_entry)+
       (sizeof(struct mesg_entry)*
       (mesg_no % data->entry.max_mesg));
  int flag = !islocked(DOS_SEM);
  struct mesg_entry mentry;

  if (data->mail == -1) return (0);

  if (flag) lock_dos();

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
  unsigned long int pos = sizeof(struct first_entry)+
       (sizeof(struct mesg_entry)*
       (mesg_no % data->entry.max_mesg));
  int flag = !islocked(DOS_SEM);
  int need_to_open_base = (!(data->is_open));


  if (flag) lock_dos();

  if (need_to_open_base)
  if (!(data->fp=g_fopen(data->basename,"rb","GET_ENTRY")))
       {
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

  unsigned long int pos = sizeof(struct first_entry)+
       (sizeof(struct mesg_entry)*
       (mesg_no % data->entry.max_mesg));
  int flag = !islocked(DOS_SEM);
  int need_to_open_base = !(data->is_open);

  if (need_to_open_base)
  if (!(data->fp=g_fopen(data->basename,"rb","READ_ENTRY")))
       {
         return;
       }

  if (flag) lock_dos();

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
    if (!(file->fp=g_fopen(file->filename,"rb","READBBS")))
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

void compress_base(struct mail_pass_data *data)
{
  unsigned long int new_tail = data->entry.head_entry_no;
  unsigned long int move_tail = new_tail;
  unsigned long int pos, pos2;
  struct mesg_entry mentry;
  char filename[FILENAME_LEN];
  int found = 1;
  int flag = !islocked(DOS_SEM);


  if (flag) lock_dos();
  while ((move_tail > data->entry.tail_entry_no) && (found))
  {
    new_tail--;
    if (move_tail > new_tail) move_tail = new_tail;
    pos = sizeof(struct first_entry)+
       (sizeof(struct mesg_entry)*
       (new_tail % data->entry.max_mesg));
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
          move_tail--;
          pos2 = sizeof(struct first_entry)+
           (sizeof(struct mesg_entry)*
           (move_tail % data->entry.max_mesg));
          fseek(data->fp,pos2,SEEK_SET);
          if (fread(&mentry,sizeof(struct mesg_entry),1,data->fp)==1)
          {
            if (!mentry.deleted)
            {
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
              found = 1;
            }
          }
        }
      }
    }
  }
  if (found) data->entry.tail_entry_no = new_tail;

  if (flag) unlock_dos();
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
  sprintf(s,"Head/Tail: %ld %ld %ld",data->entry.head_entry_no,
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
  if (flag)  lock_dos();
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
        mentry->user_no,mentry->systemname);
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

  if (error)
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
  if (flag)  lock_dos();
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


    if (flag) lock_dos();
    read_entry(data,message_no,&mentry,&file);
    if (flag) unlock_dos();

    if (mentry.deleted)
     { print_string(" That messsages is deleted, undelete (y/n) ? ");
       do
        { get_string(s,2);
        } while (!*s);
       if (toupper(*s) == 'Y')
            { print_str_cr("  --> UNDELETED <--");
              undelete_msg_from_base(data,message_no, NO);
            }
       else
       return 0;
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
    return 1;
}


long int read_new_message(void)
{
  struct mail_pass_data data;
  long int return_value;
  unsigned long int message_no;

  open_mail_base(user_lines[tswitch].number,&data,0);

  message_no = data.entry.new_msg_no;

  if ((message_no<data.entry.tail_entry_no) ||
                  (message_no>=data.entry.head_entry_no))
     return_value = -1;
  else
    {
       return_value =  data.entry.new_msg_no;

     read_single_message(&data,message_no);
    }

  close_base(&data);


  return ((long int)(return_value));
}



long int read_a_mail_message(int num)
{
  struct mail_pass_data data;
  int success;
  unsigned long int entry_num;

  open_mail_base(user_lines[tswitch].number,&data,0);

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

void delete_mail(void)
{
  char command[7];
  int num;
  char *point;
  struct mail_pass_data data;
  int flag = !islocked(DOS_SEM);

  prompt_get_string("Delete message: ",command,4);
  if ((num=str_to_num(command,&point))>0)
  {
    if (flag) lock_dos();
    open_mail_base(user_lines[tswitch].number,&data,0);
    if (num<=data.entry.max_mesg)
     delete_msg_from_base(&data, convert_to_entry_num(&data,num),0);
    close_base(&data);
    if (flag) unlock_dos();
  }
}

void undelete_mail(void)
{
  char command[7];
  int num;
  char *point;
  struct mail_pass_data data;
  int flag = !islocked(DOS_SEM);

  prompt_get_string("Un-delete Which message # ? : ",command,4);
  if ((num=str_to_num(command,&point))>0)
  {
    if (flag)  lock_dos();
    open_mail_base(user_lines[tswitch].number,&data,0);
    if (num<=data.entry.max_mesg)
     undelete_msg_from_base(&data,data.entry.head_entry_no -
        data.entry.max_mesg + num - 1,0);
    close_base(&data);
    if (flag) unlock_dos();
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

  lock_dos();
  isfile = findfirst(wildcard,&file_ptr, FA_NORMAL);
  while((!isfile))
   {
     unlock_dos();
     print_string("               Adding: ");
     print_string(file_ptr.ff_name);

     sprintf(in_file.filename,"%s\\%s",data->basename,file_ptr.ff_name);
      if ((in_file.fp = g_fopen(in_file.filename,"rb+","ROLDMAIL")))
      {
        mail_line(temp_str,USER_NAME_LEN-5,USER_NAME_LEN-5,in_file.fp);
        strcpy(mentry.username,temp_str+11);
        mentry.user_no = str_to_num(temp_str+6,&dummy);
        mail_line(mentry.subject,SUBJECT_LEN-5,SUBJECT_LEN-5,in_file.fp);
        g_fclose(in_file.fp);
      }
     else
      {

        strcpy_n(mentry.username,"Unknown User",USER_NAME_LEN);
        mentry.user_no = 999;
        strcpy_n(mentry.subject,"Unknown Subject",SUBJECT_LEN);
      }

     in_file.openfile=0;
     lock_dos();
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
     lock_dos();
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
    print_string_n(string,len);
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
  int abort=0;
  int show_message=0;
  int flag = !islocked(DOS_SEM);


  if (flag) lock_dos();

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
    get_entry(&data,cur,&mentry,&file);

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

        showed_message=YES;

        special_code(1,tswitch);
        if (mentry.read)
          print_string("O ");
        else
          print_string("N ");

        if (mentry.user_no<0)
           sprintf(s,"|*h1|*f4%02d  |*f7(%%GST) ", convert_from_entry_num(&data,cur));
        else
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
    }
    else
      unlock_dos();

    next_task();
    lock_dos();
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
    lock_dos();
   }

   if (flag) unlock_dos();
}

void send_mail(void)
{
}

void user_feedback(char *string, char *name, int portnum)
{
}

void is_new_mail(void)
{
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

  read_entry(&data,entry_num,&mentry,&file);

  close_base(&data);

  send_mail_subj(mentry.user_no);

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
  int  read_temp;
  char last_command[7] = "";
  char *point;
  char *temp_chr;
  struct mail_pass_data data;


  list_mail_special(LIST_NEW_MAIL,NO);

  while (flag)
  {
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
                     auto_reply_to_message(last_message_read);
                  else
                     print_str_cr(" No Current Message");
                     //auto_reply();
                  break;
        case 'R': temp = read_mail_message();
                  if (temp!=-1)
                   { read_temp = YES;
                    last_message_read = (unsigned long int) temp;
                   }
                  break;
        case 'D': delete_mail();
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
  lock_dos();
  open_mail_base(user_lines[tswitch].number,&data,0);
  compress_base(&data);
  close_base(&data);
  unlock_dos();
}


