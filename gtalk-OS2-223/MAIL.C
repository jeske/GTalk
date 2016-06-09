


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#include "include.h"
#include "gtalk.h"
#include "structs.h"

#define INCL_DOS
#define INCL_DOSDEVIOCTL
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#include <os2.h>

#define INCL_DOSPROCESS

#undef DEBUG_LOCKS
#undef HT_DEBUG 					   /* head/tail debug message */
#undef DEBUG
#undef DEBUG_BAD

#define LOCK_STR_LEN 80

void undelete_a_message(struct mail_pass_data *data,unsigned long int entry_num);

int unlock_base(struct mail_pass_data *data, int lock, char *lock_type);
int lck_base(struct mail_pass_data *data, int lock, int timeout, char *lock_type);
int lck_base_verbose(struct mail_pass_data *data, int lock, char *lock_type);

int lck_base(struct mail_pass_data *data, int lock, int timeout, char *lock_type)
{
  HMTX handle;
  char s[LOCK_STR_LEN+1];

  return 1;
  sprintf(s,"\\SEM32\\%sENTRY%03d",lock_type,lock);
#ifdef DEBUG_LOCKS
  print_string("Using semaphore ");
  print_str_cr(s);
#endif

  if (DosOpenMutexSem(s,&handle))
  {
	if (DosCreateMutexSem(s,&handle,0,0))
	  return (0);
  }
  if (DosRequestMutexSem((HMTX) handle, 1000))
	return (0);
  data->handle = handle;
  return (1);
}

int lck_base_verbose(struct mail_pass_data *data, int lock, char *lock_type)
{
  int attempts = 0;
  int key;
  char s[20];

#ifdef DEBUG_LOCKS
  print_str_cr("Lock attempt");
#endif

  while (attempts < 60)
  {
	if (lck_base(data, lock, 1000, lock_type))
	{
	  if (attempts)
		print_cr();

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

  return (0);
}


int unlock_base(struct mail_pass_data *data, int lock, char *lock_type)
{
  int entry;
  HEV handle = data->handle;
  char s[LOCK_STR_LEN+1];

  return;
  if (data->handle == -1) return;

  data->handle = -1;
  sprintf(s,"\\SEM32\\%sENTRY%03d",lock_type,lock);
#ifdef DEBUG_LOCKS
  print_string("unlocking semaphore ");
  print_str_cr(s);
#endif
  DosReleaseMutexSem(handle);
  DosCloseMutexSem(handle);
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
  int loop;

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

int load_base(struct mail_pass_data *data, int create, int mode,int *error)
{
  char filename[FILENAME_LEN];
  char temp_fl[FILENAME_LEN];
  static char modes[][4] = {"rb+", "rb"};
  static char mode_desc[][10] = {"LOADBASE","READBASE"};
  int mode_index=0;

  switch (mode)
  {

   case RW_BASE : mode = RW_BASE;
				  mode_index = 0;
				  break;
   default:
   case R_BASE	: mode = R_BASE;
				  mode_index = 1;
				  break;
  }

#ifdef DEBUG
  print_string("load_base:  ");
  print_str_cr(modes[mode_index]);
#endif


  sprintf(filename,"%s\\basedata",data->basename);
  if ((data->fp=g_fopen_excl(filename,modes[mode_index],mode_desc[mode_index],PRIVATE_ACCESS,error))==0)
  {

    if (!create)
	   return (0);
	mkdir(data->basename);
    if ((data->fp=g_fopen_excl(filename,"wb+","CREBASE",PRIVATE_ACCESS,error))==0)
	   return (0);
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
	data->handle = -1;
	data->entry.max_mesg = 50;	//DEBUG
	fwrite(&data->entry,sizeof(struct first_entry),1,data->fp);
	fseek(data->fp,0,SEEK_SET);
	g_fclose(data->fp);
    data->fp = 0;

  if ((data->fp=g_fopen_excl(filename,modes[mode_index],mode_desc[mode_index],PRIVATE_ACCESS,error))==0)
	{
#ifdef DEBUG
	   print_str_cr("Can't open after create");
#endif
	   return 0;
	  }

  }
  fread(&data->entry,sizeof(struct first_entry),1,data->fp);
  data->is_open=1;
  data->open_mode=mode;

  return (1);
}

void flush_base(struct mail_pass_data *data)
{
  fseek(data->fp,0,SEEK_SET);
  fwrite(&data->entry,sizeof(struct first_entry),1,data->fp);
}

void close_base(struct mail_pass_data *data)
{
  if ((data->open_mode==RW_BASE) && data->is_open)
	flush_base(data);

#ifdef DEBUG
  print_str_cr("close_base:");
  if (data->is_open) print_str_cr("Base IS open");
  if (data->open_mode==RW_BASE) print_str_cr("Base Open r/w");
#endif

  if (data->is_open)
	{
	 g_fclose(data->fp);
     data->fp = 0;
	 data->is_open=0;
	}
  if (data->mail == -1)
	unlock_base(data,data->bbs_num,"BBS");
	else
	unlock_base(data,data->mail,"MAIL");
}


int open_mail_base(int user_no, struct mail_pass_data *data, int create_mail)
{				  /* Current user #=user_lines[tswitch].number */
  int result;
  int error;

  if (user_no<0) /* if they are trying to lock a guest base for some reason */
      return(1);

  if (!lck_base_verbose(data,user_no,"MAIL"))
       return (1);

  sprintf(data->basename,"MAIL\\MAIL%03d",user_no);
  data->mail = user_no;
  data->bbs_num = -1;

  if ((result=(load_base(data,create_mail,RW_BASE,&error)))==0)
        unlock_base(data,data->mail,"MAIL");
  else
        error = 0;

  return (error);
}



int open_bbs_base(int bbs_no, struct mail_pass_data *data, int create_bbs)
{				  /* Current user #=user_lines[tswitch].number */
  int result;
  int error;
  if (!lck_base_verbose(data,bbs_no,"BBS"))
       return (1);

  sprintf(data->basename,"BBS\\BBS%03d",bbs_no);
  data->mail = -1;
  data->bbs_num = bbs_no;
  if ((result=(load_base(data,create_bbs,RW_BASE,&error)))==0)
	unlock_base(data,data->bbs_num,"BBS");
  else
    error = 0;

  return (error);
}

int read_bbs_base(int bbs_no, struct mail_pass_data *data, int create_bbs)
{
   int ret_val;
   int error;

   if (!lck_base_verbose(data,bbs_no,"BBS"))
       return (1);

   sprintf(data->basename,"BBS\\BBS%03d",bbs_no);
   data->mail = -1;
   data->bbs_num = bbs_no;
   ret_val = load_base(data,create_bbs,R_BASE,&error);
   if (ret_val==1)
     error=0;
   data->is_open=0;

   if (!error)
   {
     g_fclose(data->fp);
     data->fp = 0;
   }

#ifdef DEBUG
   print_str_cr("Base Closed");
#endif

   unlock_base(data,bbs_no,"BBS");
   return error;
}

int read_mail_base(int user_no, struct mail_pass_data *data, int create_mail)
{
  int ret_val;
  int error;

  if (!lck_base_verbose(data,user_no,"MAIL"))
       return (1);

   sprintf(data->basename,"MAIL\\MAIL%03d",user_no);
   data->mail = user_no;
   data->bbs_num = -1;
   ret_val = load_base(data,create_mail,R_BASE,&error);
    if (ret_val==1)
        error=0;
   data->is_open=0;
   if (!error)
   {
     g_fclose(data->fp);
     data->fp = 0;
   }


#ifdef DEBUG
   print_str_cr("Base Closed");
#endif

   unlock_base(data,user_no,"MAIL");
   return (error);
}

int create_temp_file(struct file_entry *file, unsigned int editor, int gen)
{
  int error;
  sprintf(file->filename,"TEMP\\TEMP%02d%01d",tswitch,gen);

  if (editor)
  {
	remove(file->filename);

	if (!line_editor(file->filename,editor)) return (0);
	if (file->openfile)
    {
      if ((file->fp=g_fopen_excl(file->filename,"rb+","EDITORFL",PUBLIC_ACCESS,&error))==0)
         return (0);
    }
    else
    file->fp = 0;

	return (1);
  }

  if (!(file->fp=g_fopen_excl(file->filename,"wb+","EDITORFL",PUBLIC_ACCESS,&error))==0)
    return (0);

  return (1);
}

void generate_id(struct mail_pass_data *data, char *filename,
				 struct file_entry *file)
{
  struct ffblk look_up;
  unsigned int index=time(NULL);

  do
  {
	sprintf(filename,"%02X%04X",sys_info.system_number,(unsigned)
			(unsigned short int) (index++));
	sprintf(file->filename,"%s\\%s",data->basename,filename);
  }
  while (!findfirst(file->filename,&look_up,FA_NORMAL));
}

int copy_a_file(struct file_entry *outfile,
				struct file_entry *infile, int append)
{
  char buffer[512];
  int length;
  int delay;
  int error;

  if (!infile->openfile)
  if ((infile->fp=g_fopen_excl(infile->filename,"rb","INCOPY",PRIVATE_WRITE_ACCESS,&error))==0)
     return (0);
  if (!outfile->openfile)
   if ((outfile->fp=g_fopen_excl((char *)outfile,append ? "ab+" : "wb+","OUTCOPY",PRIVATE_WRITE_ACCESS,&error))==0)
	{
      if (!infile->openfile)
      {
         g_fclose(infile->fp);
         infile->fp = 0;
      }
	  return (0);
	}
  do
  {
	length = fread(buffer,sizeof(char),512,infile->fp);
	fwrite(buffer,sizeof(char),length,outfile->fp);
	for (delay=0;delay<STALL_TASKS;delay++) next_task();
  } while (length);


  if (!infile->openfile)
  {
     g_fclose(infile->fp);
     infile->fp = 0;
  }

  if (!outfile->openfile)
  {
    g_fclose(outfile->fp);
    outfile->fp = 0;
  }

  return (1);
}

int scan_for_next_message(struct mail_pass_data *data,
						  struct mesg_entry *mentry)
{
  unsigned long int cur = mentry->entry_no;
  unsigned long int pos;
  struct mesg_entry temp_mentry;
  int find_tail;
  int find_new;
  long int real_tail,real_new;


  if (!data->entry.max_mesg)
   {
	log_error("* scan_for_next_message has a NULL max_mesg");
    print_str_cr("Corrupt Mail Base...<adjusting>.");
	return (0);
   }

  if (data->entry.tail_entry_no > data->entry.head_entry_no)
	 data->entry.tail_entry_no = data->entry.head_entry_no;
  if (data->entry.new_msg_no < data->entry.tail_entry_no)
	 data->entry.new_msg_no = data->entry.tail_entry_no;

  find_tail = (cur == data->entry.tail_entry_no);
  find_new = ((data->mail != 1) && (cur == data->entry.new_msg_no));
  if ((!find_tail) && (!find_new)) return (1);
  real_tail = -1;
  real_new = -1;


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

  return (1);
}

int add_msg_to_base(struct mail_pass_data *data, int dest_no,
					struct file_entry *file, struct mesg_entry *mentry,
					int overwrite)
{
  unsigned long int pos;
  struct mesg_entry mentryold;
#ifdef HT_DEBUG
  char temp[200];
#endif
  int error;
  char filename[FILENAME_LEN];

  if (!data->entry.max_mesg)
   {
	log_error("* add_msg_to_base has a NULL max_mesg");
	print_str_cr("Corrupt Mail Base");
	return (0);
   }

  if (!data->entry.max_mesg)
	return (0);

  if (abs((data->entry.head_entry_no - data->entry.tail_entry_no)) > data->entry.max_mesg)
	{	  log_error("*TAIL OUT OF WACK IN BBS BASE");
		  print_str_cr(" BBS head/tail pointer error, resetting base");
		  data->entry.tail_entry_no = data->entry.head_entry_no;
		  data->entry.new_msg_no = data->entry.tail_entry_no;
		  flush_base(data);
	}

#ifdef DEBUG
  print_str_cr("add_msg_to_base:");
#endif


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
	 {
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
   if ((file->fp = g_fopen_excl(file->filename,"wb+","ADDMSG",PRIVATE_WRITE_ACCESS,&error))==0)
    { file->openfile=0;
	  return (0);
	}
#ifdef HT_DEBUG
if (!user_options[tswitch].priority)
 {
  sprintf(temp,"Sysop H/T Debug - Head: %lu Tail: %lu",data->entry.head_entry_no,data->entry.tail_entry_no);
  print_str_cr(temp);
 }
#endif
  flush_base(data);

#ifdef DEBUG_BAD
  print_str_cr("Successfull");
#endif
  return (1);
}

int delete_msg_from_base(struct mail_pass_data *data,
	   unsigned long int mesg_no, int do_erase)
{
  unsigned long int pos;
  struct mesg_entry mentry;
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

  fseek(data->fp,pos,SEEK_SET);
  if (fread(&mentry,sizeof(struct mesg_entry),1,data->fp) != 1)
	  return (0);
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
  return (1);
}

int undelete_msg_from_base(struct mail_pass_data *data,
	   unsigned long int mesg_no, int do_erase)
{
  unsigned long int pos;
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

  fseek(data->fp,pos,SEEK_SET);
  if (fread(&mentry,sizeof(struct mesg_entry),1,data->fp) != 1)
	 return (0);
  if (!mentry.deleted)
	 return (1);
  if (mentry.total_erase)
	 return (0);
  mentry.deleted = 0;
  fseek(data->fp,pos,SEEK_SET);
  fwrite(&mentry,sizeof(struct mesg_entry),1,data->fp);

  return (1);
}

int get_entry(struct mail_pass_data *data, unsigned long int mesg_no,
	 struct mesg_entry *mentry, struct file_entry *file)
{
  unsigned long int pos;
  int error;
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

  if (need_to_open_base)
  if ((data->fp=g_fopen_excl(s,"rb","GET_ENTRY",PRIVATE_ACCESS,&error))==0)
	   return (0);

  fseek(data->fp,pos,SEEK_SET);

  if (fread(mentry,sizeof(struct mesg_entry),1,data->fp) != 1)
	 {
	   if (need_to_open_base)
       {
          g_fclose(data->fp);
          data->fp = 0;
       }
	   return (0);
	 }

  sprintf(file->filename,"%s\\%s",data->basename,mentry->filename);

  if (file->openfile)
    if ((file->fp=g_fopen_excl(file->filename,"rb","GETBBS",PRIVATE_WRITE_ACCESS,&error))==0)
	   {
		 if (need_to_open_base)
         {
			  g_fclose(data->fp);
              data->fp = 0;
         }
		 return (0);
	   }

  if (need_to_open_base)
  {
	   g_fclose(data->fp);
       data->fp = 0;
  }
  return (1);
}


int read_entry(struct mail_pass_data *data, unsigned long int mesg_no,
	 struct mesg_entry *mentry, struct file_entry *file)
{

  unsigned long int pos;
  int error;
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

  if (need_to_open_base)
  if ((data->fp=g_fopen_excl(s,"rb+","READ_ENTRY",PRIVATE_WRITE_ACCESS,&error))==0)
	   return;

  fseek(data->fp,pos,SEEK_SET);
  if (fread(mentry,sizeof(struct mesg_entry),1,data->fp) != 1)
	  {
		if (need_to_open_base)
        {
             g_fclose(data->fp);
             data->fp = 0;
        }
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
    if ((file->fp=g_fopen_excl(file->filename,"rb","READ_ENTRY2",PRIVATE_WRITE_ACCESS,&error))==0)
	 {
	   if (need_to_open_base)
       {
			 g_fclose(data->fp);
             data->fp = 0;
       }
		return (0);
	  }

  if (need_to_open_base)
  {
	 g_fclose(data->fp);
     data->fp = 0;
  }

  return (1);
}

void compress_base(struct mail_pass_data *data,char echo)
{
  unsigned long int new_tail = data->entry.head_entry_no;
  unsigned long int move_tail = new_tail;
  unsigned long int pos, pos2;
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
	if (echo)
	  print_chr(echo);
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
		  DosSleep(5l);
		  move_tail--;
		  pos2 = sizeof(struct first_entry)+
		   (sizeof(struct mesg_entry)*
		   (move_tail % data->entry.max_mesg));
		  fseek(data->fp,pos2,SEEK_SET);
		  if (fread(&mentry,sizeof(struct mesg_entry),1,data->fp)==1)
		  {
			if (!mentry.deleted)
			{
			  if (echo)
				print_chr(echo);
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
//	if (found) data->entry.tail_entry_no = new_tail;

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

  if (open_mail_base(user_num,&data,0))
    return;

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
  char s[80];

#ifdef DEBUG
  print_str_cr("send_mail_to:");
#endif


  file2.openfile = 1;
  if (open_mail_base(user_no,&data,CREATE))
	 { error = 0;
	   error_code = 1;
	 }
  else
     if (!add_msg_to_base(&data,0,&file2,mentry,1))
     { error = 0;
       error_code = 2;
     }
     else
     {
        fprintf(file2.fp,"|*h1|*f4From:     |*f7");
        if (mentry->user_no < 0) fprintf(file2.fp,"(%%GST)");
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
        file2.fp = 0;
     }
  close_base(&data);

#ifdef DEBUG
  switch (error_code)
   {
   case 1:	print_str_cr(" Could Not open base for writing");
			break;
   case 2:	print_str_cr(" Could not add_msg_to_base");
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
  char s[200];

  if ((user_no>=0) && (user_no<=999))
   if (!load_user_info(user_no,&temp_data)) error = 0;

  if ((error) || (temp_data.user_info.number<0))
  {
	print_str_cr("That user does NOT EXIST");
	return (0);
  }
  print_cr();
  special_code(1,tswitch);
  sprintf(s,"|*f4|*h1     To: |*f7(#%03d) %c|*r1%s|*r1|*f7|*h1%c ",user_no,temp_data.class_info.staple[2],temp_data.user_info.handle,temp_data.class_info.staple[3]);
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

  print_string("--> Sending Mail...");
  mentry.dest_user_no = user_no;
  mentry.system_no = sys_info.system_number;
  mentry.user_no = user_lines[tswitch].user_info.number;
  mentry.dest_no = sys_info.system_number;
  strcpy_n(mentry.username,user_lines[tswitch].user_info.handle,USER_NAME_LEN);
  strcpy_n(mentry.systemname,sys_info.system_name,SYSTEM_NAME_LEN);
  mentry.ent_date=time(NULL);
  send_mail_to(&mentry,&file,user_no);
  print_str_cr("<Done>");

  g_fclose(file.fp);
  file.fp = 0;

  return (1);
}

int send_mail_to_and_about(int user_no,char *subject)
{
  int error = 1;
  struct mesg_entry mentry;
  struct file_entry file;
  struct user_data temp_data;
  char s[200];

  if ((user_no>=0) && (user_no<=999))
   if (!load_user_info(user_no,&temp_data)) error = 0;

  if ((error) || (temp_data.user_info.number<0))
  {
	print_str_cr("That user does NOT EXIST");
	return (0);
  }
  print_cr();
  special_code(1,tswitch);
  sprintf(s,"|*f4|*h1     To: |*f7(#%03d) %c|*r1%s|*r1|*f7|*h1%c ",user_no,temp_data.class_info.staple[2],temp_data.user_info.handle,temp_data.class_info.staple[3]);
  print_str_cr(s);
  print_string("|*f4|*h1Subject: |*f7|*h1");
  strncpy(mentry.subject,subject,SUBJECT_LEN);
  mentry.subject[SUBJECT_LEN]=0;
  print_string(mentry.subject);
  print_cr();
  special_code(0,tswitch);
  file.openfile = 0;
  if (!create_temp_file(&file,MAX_EDITOR_LEN,0))
	 { print_str_cr("Mail Aborted.");
	   return (0);
	 }

  print_string("--> Sending Mail...");
  mentry.dest_user_no = user_no;
  mentry.system_no = sys_info.system_number;
  mentry.user_no = user_lines[tswitch].user_info.number;
  mentry.dest_no = sys_info.system_number;
  strcpy_n(mentry.username,user_lines[tswitch].user_info.handle,USER_NAME_LEN);
  strcpy_n(mentry.systemname,sys_info.system_name,SYSTEM_NAME_LEN);
  mentry.ent_date=time(NULL);
  send_mail_to(&mentry,&file,user_no);
  print_str_cr("<Done>");
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

#ifdef DEBUG
    print_str_cr("read_single_message();");
#endif

  if ((message_no<data->entry.tail_entry_no) ||
				(message_no>=data->entry.head_entry_no))
	{print_str_cr(" No Such Message");
	 return 0;
	}

	file.openfile=0;
	if (!read_entry(data,message_no,&mentry,&file))
	  {
		print_str_cr("Read Failed");
		return -1;
	  }

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
#ifdef DEBUG
    print_str_cr("read_single_message: going to close");
#endif

    if (file.openfile)
    {
       g_fclose(file.fp);
       file.fp = 0;
    }

#ifdef DEBUG
    print_str_cr("read_single_message: closed");
#endif
	return 1;
}


long int read_new_message(void)
{
  struct mail_pass_data data;
  long int return_value;
  unsigned long int message_no;


  if (read_mail_base(user_lines[tswitch].user_info.number,&data,0))
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

  if (read_mail_base(user_lines[tswitch].user_info.number,&data,0))
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

int delete_mail(long int entry_no, char *command)
{
  long int first, last;
  char *point;
  struct mail_pass_data data;

  if (entry_no == -1)
  {
	if (!(get_multiple_range("Delete message: ",command,&first,&last)))
	   return (0);
  } else
	first = last = entry_no;

  open_mail_base(user_lines[tswitch].user_info.number,&data,0);

  for (;first<=last;first++)
	 delete_msg_from_base(&data,first,0);

  close_base(&data);
  return (1);
}

void undelete_a_message(struct mail_pass_data *data,unsigned long int entry_num)
{
  char filename[FILENAME_LEN];
  int error;

	sprintf(filename,"%s\\basedata",data->basename);

    if ((data->fp = g_fopen_excl(filename,"rb+","UNDEL",PRIVATE_WRITE_ACCESS,&error))!=0)
	  {
	   undelete_msg_from_base(data,entry_num,NO);
	   g_fclose(data->fp);
       data->fp = 0;
	  }
	else
	 {
	   print_str_cr("Message Un-deletion failed");
	 }
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

  if (!read_mail_base(user_lines[tswitch].user_info.number,&data,NO_CREATE))
   {
	 undelete_a_message(&data,convert_to_entry_num(&data,num));
	 close_base(&data);
   }
   else
   {
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
  int error;
  char temp_str[100];
  char *dummy;

  sprintf(wildcard,"%s\\%s",data->basename,file_pattern);

  mentry.dest_user_no = user_lines[tswitch].user_info.number;
  mentry.system_no = sys_info.system_number;
  mentry.user_no = 999;
  mentry.dest_no = sys_info.system_number;
  strcpy_n(mentry.username,"*OLD* Mail",USER_NAME_LEN);
  strcpy_n(mentry.subject,"Mail From Old Mail System",SUBJECT_LEN);
  strcpy_n(mentry.systemname,sys_info.system_name,SYSTEM_NAME_LEN);

  isfile = findfirst(wildcard,&file_ptr, FA_NORMAL);
  while((!isfile))
   {
	 print_string("               Adding: ");
	 print_string(file_ptr.ff_name);

	 sprintf(in_file.filename,"%s\\%s",data->basename,file_ptr.ff_name);
      if ((in_file.fp = g_fopen_excl(in_file.filename,"rb+","ROLDMAIL",PRIVATE_WRITE_ACCESS,&error))!=0)
	  { char *end;
		mail_line(temp_str,USER_NAME_LEN-5,USER_NAME_LEN-5,in_file.fp);
		strcpy(mentry.username,scan_str_for_chr(temp_str,')')+1);
		truncate_string(mentry.username);
		mentry.user_no = str_to_num(scan_str_for_chr(temp_str,'#')+1,&dummy);
		mail_line(mentry.subject,SUBJECT_LEN-5,SUBJECT_LEN-5,in_file.fp);
		truncate_string(mentry.subject);
		g_fclose(in_file.fp);
        in_file.fp=0;

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
	 dest_file.openfile=0;
	 if (!add_msg_to_base(data,sys_info.system_number, &dest_file, &mentry,0))
		{
		  print_str_cr("   Error Adding Mail");
		}
	 else
		{
		 dest_file.openfile=0;

		 copy_a_file(&dest_file,&in_file,0);
		 print_string("     As: ");
		 print_str_cr(mentry.filename);
		}

	 isfile = findnext(&file_ptr);
   }
}



void assimilate_old_mail(int user_number,struct mail_pass_data *data)
{

  if (open_mail_base(user_lines[tswitch].user_info.number,data,CREATE))
   {
	 print_str_cr("--> Error Creating New Mail Index");
	 return;
   }

  if (user_lines[tswitch].class_info.priority)
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

void list_mail_special(char *command)
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
  int show_deleted=0;
  int entry_read;
  int mode;

  if (read_mail_base(user_lines[tswitch].user_info.number,&data,NO_CREATE))
  {
	print_str_cr("--> No mail index.. assimilating old mail");
    assimilate_old_mail(user_lines[tswitch].user_info.number,&data);
	return;
  }

  switch (*command)
  {
	 default:
	 case 'N':
	 case 'n':			   cur = data.entry.new_msg_no;
						   stop_message_number = data.entry.head_entry_no;
						   mode = LIST_NEW_MAIL;
						   break;
	 case 'A':
	 case 'a':			   show_deleted = 1;
						   command++;
 //	 default:		       mode = LIST_ALL_MAIL;
	 case 'O':
	 case 'o':			   if ((*command == 'O') || (*command == 'o'))
						   {
							  mode = LIST_OLD_MAIL;
							  command++;
						   }
						   if (*command)
						   {
							  if (!(get_multiple_range("List messages:",command,&cur,&stop_message_number)))
								return;
							  stop_message_number++;
						   } else
						   {
							 cur = data.entry.tail_entry_no;
							 stop_message_number = data.entry.head_entry_no;
						   }
						   break;
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

	while ((key_hit=int_char(tswitch))!=-1)
	  if ((key_hit==27) || (key_hit==32))
		abort=1;

	next_task();
	cur++;
  }
  close_base(&data);

  if (!showed_message)
   {
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
   }
}

void send_mail(void)
{
}

/*
void user_feedback(char *string, char *name, int portnum)
{
	send_mail_subj(0);
}
*/

void is_new_mail(void)
{
  struct mail_pass_data data;
  int result;

  if (user_lines[tswitch].user_info.number < 0)
	  return;

  if (read_mail_base(user_lines[tswitch].user_info.number,&data,NO_CREATE))
	{
	  print_str_cr("NO MAIL INDEX!");
	  return;
	}

   if ((data.entry.new_msg_no+1)<=data.entry.head_entry_no)
		   result = YES;
   else
		   result = NO;
   close_base(&data);

   if (result)
	{
	 special_code(1,tswitch);
	 print_str_cr("|*f4|*h1*** You have |*f1NEW|*f4 mail ***|*r1");
	 special_code(0,tswitch);
	}
   else
	 print_str_cr("No New Mail");

}

void auto_reply_to_message(unsigned long int entry_num)
{
  char s[10];
  struct mail_pass_data data;
  struct mesg_entry mentry;
  struct file_entry file;
  int message_num;

  open_mail_base(user_lines[tswitch].user_info.number,&data,0);

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
	delete_mail(entry_num,NULL);

  return;
}

void auto_reply(void)
{
	print_string(" Enter Message # to Auto Reply to: ");
	print_str_cr("UNFINISHED");
	return;
}

int get_multiple_range(char *prompt, char *point,
		unsigned long int *first, unsigned long int *last)
{
   struct mail_pass_data data;
   int num;
   long int temp;
   char input_line[9];

   while (*point == ' ')
	  point++;
   if (!(*point))
   {
	  prompt_get_string(prompt,input_line,7);
	  point = input_line;
   }

   if (!read_mail_base(user_lines[tswitch].user_info.number,&data,0))
   {
	  num = str_to_num(point,&point);
	  temp = convert_to_entry_num(&data,num);
	  if ((temp < data.entry.tail_entry_no) || (temp >= data.entry.head_entry_no))
	  {
		print_str_cr("Message out of range");
		return(0);
	  }
	  *first = temp;
	  if (*point == '-')
	  {
		num = str_to_num((point+1),&point);
		temp = convert_to_entry_num(&data,num);
		if ((temp < data.entry.tail_entry_no) || (temp >= data.entry.head_entry_no))
		{
			print_str_cr("Message out of range");
			return(0);
		}
		if (!last)
		{
		  print_str_cr("Cannot enter range here");
		  return (0);
		}
		*last = temp;
		if (*last < *first)
		{
			unsigned long int temp = *first;
			*first = *last;
			*last = temp;
		}
	  } else
	  {
		if (last)
		 *last = *first;
	  }
	  return (1);
   }
   print_str_cr("Can not open base");
   return (0);
}

void mail_system(char *str,char *name, int portnum)
{
  int flag = 1;
  char command[14];
  int num;
  long int temp;
  unsigned long int last_message_read = 0;
  int  has_read_message = NO;
  int  need_to_compress = 0;
  int  read_temp;
  char last_command[9];
  char *point;
  char *temp_chr;
  struct mail_pass_data data;

  last_command[0]=0;
  list_mail_special("N");

  while (flag)
  {
	check_for_privates();
	print_cr();
	prompt_get_string("Mail Command (|*h1?|*h0 for Menu): ",command,12);
	temp_chr=command;
	while (*temp_chr)
	  {*temp_chr = toupper(*temp_chr);
	   temp_chr++;
	  }

	read_temp = 0;

	switch (*command)
	  {
		case 'A':
				  if (!test_bit(user_options[tswitch].privs,SEND_MAIL_PRV))
				   {
					print_str_cr("You cannot send mail");
					break;
				   }

				  if (has_read_message)
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
		case 'D': if (*(command+1)=='L')
				   {
					 if (has_read_message)
						{ // DELETE LAST MESSAGE
							delete_mail(last_message_read,NULL);
							print_str_cr("Last Message Read Deleted.");
							need_to_compress = YES;
							break;
						} else
					 print_str_cr("No last message to delete");
					 break;
				   }
				  if (delete_mail(-1,command+1))
					  need_to_compress = YES;
				  break;
		case 'U': undelete_mail();
				  break;
		case 'Q': flag = 0;
				  break;
		case 'S':
				  if (!test_bit(user_options[tswitch].privs,SEND_MAIL_PRV))
				   {
					print_str_cr("You cannot send mail");
					break;
				   }
				  temp = str_to_num(command+1,&point);
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
        case 'E': if (!user_lines[tswitch].class_info.priority)
					   examine_board();
				  break;
		case 'L': list_mail_special(command+1);
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
	   /* used to be a strncpy(last_command,command,6); */
	  strcpy(last_command,command);
	  last_command[6]=0;
  }

  if (need_to_compress)
  {
	  print_string("Purging Deleted Messages.");

      if (!open_mail_base(user_lines[tswitch].user_info.number,&data,NO_CREATE))
	   {compress_base(&data,'.');
		close_base(&data);
		print_str_cr("Done.");
	   }
  }
}


