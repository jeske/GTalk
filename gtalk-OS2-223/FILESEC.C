#include "include.h"

#include "gtalk.h"


#define CONFIG_FILE "FILESEC.CFG"
#define XFER_FILE "TEXT\\FILESEC.TXT"
#define OPEN_FILE_LEN 30
#define FILE_SEC_NAME 34
#define FILESEC_FILENAME_LENGTH 15
#define DESC_LENGTH 80
#define MAX_FILENAMES 20
#define FILESEC_MAX_FILENAME_LENGTH 80
								/* Note, this is a hack so that we use the right */
                                /* length for protocol.c */
#define FILESEC_ROOT_LENGTH 20

char file_sec_config_file[] = CONFIG_FILE;

struct filesec_options
{
  int auto_approve;             /* Automatic approval of uploads */
  int max_sections;
  int filesec_pri;
  char open_file[OPEN_FILE_LEN+1];
  char filesec_root[FILESEC_ROOT_LENGTH+1];
} filesec_ops;

struct filesec_data
{
  int num_files;
  int auto_approve;
  int use_priority;
  int dl_priority;
  int ul_priority;
  int filesec_pri;
  int admin_user;
  char name[FILE_SEC_NAME+1];
};

struct filesec_entry
{
  int active;
  int approved;
  int uploaded_by;
  int downloads;
  time_t date_of;
  unsigned long int length;
  char name[FILESEC_FILENAME_LENGTH+1];
  char description[DESC_LENGTH+1];
};

struct current_data
{
  int current_section;
  struct filesec_data fildata;
  char *filename_buffer;
  char *filepointers[MAX_FILENAMES];
  int filesections[MAX_FILENAMES];
  long int filelengths[MAX_FILENAMES];
  int num_files;
  int priority;
  int baud_rate;
  int user_number;
  time_t last_call;
};

unsigned long int asctonum_long(char *t)
{
  unsigned long int temp = 0;
  while (*t)
  {
    if ((*t>='0') && (*t<='9')) temp = (temp * 10) + (*t- '0');
     else break;
    t++;
  }
  return(temp);
}

int super(struct current_data *data)
{
  if (((data->priority > filesec_ops.filesec_pri) || (data->priority > data->fildata.filesec_pri)) &&
     (data->user_number != data->fildata.admin_user))  return (0);
  return (1);
}

int get_yes_or_no(char *prompt)
{
  char s[5];

  for (;;)
  {
    special_code(1,tswitch);
    print_cr();
    print_string(prompt);
    print_string(" (Y/N/Q) ");
    special_code(0,tswitch);
    get_string_cntrl(s,1,0,0,0,1,1,1,0);
    if (*s == 'Q') return (-1);
    if (*s == 'N') return (0);
    if (*s == 'Y') return (1);
    print_cr();
    print_str_cr("Enter 'Y', 'N', or 'Q' to quit");
  }
}

long int filesec_get_number(char *prompt)
{
  char s[10];

  for (;;)
  {
    special_code(1,tswitch);
	print_cr();
    print_string(prompt);
    special_code(0,tswitch);
    get_string_cntrl(s,9,0,0,0,1,1,1,0);
    if (*s == 'Q') return (-1);
    if ((*s >= '0') && (*s <= '9')) return (asctonum_long(s));
    print_cr();
    print_str_cr("Enter a number, or 'Q' to quit");
  }
}

int get_pr_string(char *prompt, char *string, int len, int upcase, int blank)
{
  char s[81];

  if (len > 80) len = 80;
  special_code(1,tswitch);
  print_cr();
  print_string(prompt);
  special_code(0,tswitch);
  get_string_cntrl(s,len,0,0,0,0,1,upcase,0);
  if ((!blank) && (!(*s))) return (-1);
  strcpy(string, s);
  return (0);
}

void init_batch_files(struct current_data *data)
{
  int count;
  for (count=0;count<MAX_FILENAMES;count++)
    data->filepointers[count] = data->filename_buffer + (count * (FILESEC_MAX_FILENAME_LENGTH+1));
  data->num_files = 0;
}

unsigned long int asctonum_move(char **t)
{
  unsigned long int temp = -1;
  while (**t)
  {
    if ((**t>='0') && (**t<='9')) temp = (temp * 10) + (**t- '0');
     else break;
    (*t)++;
  }
  return(temp);
}

void print_justified(char *string, int length)
{
  while ((length > 0) && (*string))
  {
    print_chr(*string++);
    length--;
  }
  while (length > 0)
  {
    print_chr(' ');
    length--;
  }
}

void edit_filesection_options(void)
{
  int choice;
  char s[80];

  for (;;)
  {
    special_code(1,tswitch);
    print_cr();
    print_str_cr("|*h1|*f6Configuring Filesection Options|*r1");
    print_cr();
    special_code(0,tswitch);

    print_justified("1. Auto upload approval",45);
    print_str_cr(filesec_ops.auto_approve ? "Yes" : "No");
    print_justified("2. Maximum file sections",45);
    sprintf(s,"%d",filesec_ops.max_sections);
    print_str_cr(s);
    print_justified("3. File to show",45);
    print_str_cr(filesec_ops.open_file);
    print_justified("4. File section superuser priority",45);
	sprintf(s,"%d",filesec_ops.filesec_pri);
	print_str_cr(s);
	print_justified("5. File section root directory",45);
	print_str_cr(filesec_ops.filesec_root);

    print_str_cr("Q. Quit filesection options");

    print_cr();
    special_code(1,tswitch);
    print_string("|*h1|*f4Enter choice: (1-4) ");
    special_code(0,tswitch);
    get_string_cntrl(s,3,0,0,0,1,1,1,0);

    if (*s == 'Q') break;
    switch (asctonum_long(s))
    {
      case  1: if ((choice=get_yes_or_no("Auto upload approval?")) != -1)
                 filesec_ops.auto_approve = choice;
               break;
      case  2: if ((choice=filesec_get_number("Maximum file sections: ")) != -1)
                 filesec_ops.max_sections = choice;
               break;
	  case  3: get_pr_string("File to show when entering file section: ",filesec_ops.open_file,OPEN_FILE_LEN,1,0);
               break;
      case  4: if ((choice=filesec_get_number("File section superuser priority: ")) != -1)
                 filesec_ops.filesec_pri = choice;
               break;
	  case  5: get_pr_string("Root directory for file section: ",filesec_ops.filesec_root,FILESEC_ROOT_LENGTH,1,0);
			   break;
	  default: print_str_cr("You have entered an invalid option");
               break;
    }
  }
}

void load_filesection_options(void)
{
  FILE *fp;

  lock_dos(5000);
  if (!(fp=g_fopen(file_sec_config_file,"rb","LOADFLSEC")))
  {
    filesec_ops.auto_approve = 0;
    filesec_ops.max_sections = 1;
    filesec_ops.filesec_pri = 0;
	strcpy(filesec_ops.open_file, XFER_FILE);
	strcpy(filesec_ops.filesec_root, "FILESEC");
    unlock_dos();
    return;
  }
  fread(&filesec_ops,sizeof(struct filesec_options),1,fp);
  g_fclose(fp);
  unlock_dos();
}

void save_filesection_options(void)
{
  FILE *fp;

  lock_dos(5001);
  if (!(fp=g_fopen(file_sec_config_file,"wb","SAVEFLSEC")))
  {
    unlock_dos();
    return;
  }
  fwrite(&filesec_ops,sizeof(struct filesec_options),1,fp);
  g_fclose(fp);
  unlock_dos();
}

int load_filesection(int filesection, struct filesec_data *fildata)
{
  FILE *fp;
  char s[80];
  char t[80];

  sprintf(t,"FILESEC\\FILE%03d",filesection);
  sprintf(s,"%s\\FILEDATA",t);
  lock_dos(5002);
  if (!(fp=g_fopen(s,"rb","LOADFILS")))
  {
    mkdir(t);
    if (!(fp=g_fopen(s,"wb","LOADFILS2")))
    {
      unlock_dos();
      return (0);
    }
    fildata->num_files = 0;
    fildata->auto_approve = 0;
    fildata->use_priority = 255;
    fildata->dl_priority = 255;
    fildata->ul_priority = 255;
    fildata->filesec_pri = filesec_ops.filesec_pri;
    fildata->admin_user = 0;
    strcpy(fildata->name,"No Name");
    fwrite(fildata,sizeof(struct filesec_data),1,fp);
    g_fclose(fp);
    unlock_dos();
    return (1);
  }
  fread(fildata,sizeof(struct filesec_data),1,fp);
  g_fclose(fp);
  unlock_dos();
  return (1);
}

int save_filesection(int filesection, struct filesec_data *fildata)
{
  FILE *fp;
  char s[80];

  sprintf(s,"FILESEC\\FILE%03d\\FILEDATA",filesection);
  lock_dos(5003);
  if (!(fp=g_fopen(s,"rb+","SAVFIL1")))
  {
    if (!(fp=g_fopen(s,"wb","SAVEFILS")))
    {
      unlock_dos();
      return (0);
    }
  }
  fwrite(fildata,sizeof(struct filesec_data),1,fp);
  g_fclose(fp);
  unlock_dos();
  return (1);
}

void edit_filesection(struct filesec_data *fildata)
{
  char s[80];
  int choice, chs;

  for (;;)
  {
    special_code(1,tswitch);
    print_cr();
    print_str_cr("|*h1|*f6Editing Filesection|*r1");
    print_cr();
    special_code(0,tswitch);

    print_justified("1. Auto upload approval",45);
    print_str_cr(fildata->auto_approve ? "Yes" : "No");
    print_justified("2. Use priority",45);
    sprintf(s,"%d",fildata->use_priority);
    print_str_cr(s);
    print_justified("3. Download priority",45);
	sprintf(s,"%d",fildata->dl_priority);
    print_str_cr(s);
    print_justified("4. Upload priority",45);
	sprintf(s,"%d",fildata->ul_priority);
    print_str_cr(s);
	print_justified("5. Name of filesection",45);
	print_str_cr(fildata->name);
    print_justified("6. File section superuser priority",45);
    sprintf(s,"%d",fildata->filesec_pri);
    print_str_cr(s);
    print_justified("7. Admin user",45);
    sprintf(s,"%d",fildata->admin_user);
    print_str_cr(s);

    print_str_cr("Q. Quit filesection editor");

    print_cr();
    special_code(1,tswitch);
    print_string("|*h1|*f4Enter choice: (1-7) ");
    special_code(0,tswitch);
    get_string_cntrl(s,3,0,0,0,1,1,1,0);

    if (*s == 'Q') break;
	switch (choice=asctonum_long(s))
    {
      case  1: if ((chs=get_yes_or_no("Auto upload approval for filesection?")) != -1)
                 fildata->auto_approve = chs;
               break;
	  case	2:
	  case	3:
      case  4:
      case  6:
      case  7: switch(choice)
			   {
				 case 2: chs = filesec_get_number("Use priority: ");
						 break;
				 case 3: chs = filesec_get_number("Download priority: ");
						 break;
				 case 4: chs = filesec_get_number("Upload priority: ");
						 break;
                 case 6: chs = filesec_get_number("File section superuser priority: ");
						 break;
                 case 7: chs = filesec_get_number("Admin user: ");
                         break;
			   }
			   if (chs == -1) break;
			   switch (choice)
			   {
				  case 2: fildata->use_priority = chs;
						  break;
				  case 3: fildata->dl_priority = chs;
						  break;
				  case 4: fildata->ul_priority = chs;
						  break;
                  case 6: fildata->filesec_pri = chs;
                          break;
                  case 7: fildata->admin_user = chs;
                          break;
			   }
               break;
      case  5: get_pr_string("Name of file section: ",fildata->name,FILE_SEC_NAME,0,1);
               break;
      default: print_str_cr("You have entered an invalid option");
               break;
    }
  }
}

void change_filesection(struct current_data *data, int sec)
{
  struct filesec_data fildata;
  int count;
  char s[80];
  char ch;

  if (!sec)
  {
    special_code(1,tswitch);
    print_cr();
    print_str_cr("|*h1|*f6Available Filesections|*r1");
    print_cr();
    for (count=1;count<=filesec_ops.max_sections;count++)
    {
      ch = get_nchar(tswitch);
      if ((ch == 3) || (ch == 27))
      {
        empty_outbuffer(tswitch);
        break;
      }
      if (load_filesection(count,&fildata))
      {
        sprintf(s,"%d. %s|*r1",count,fildata.name);
        print_str_cr(s);
      }
    }
    sprintf(s,"|*h1|*f5Choose filesection: (1-%d) ",filesec_ops.max_sections);
    if ((sec = filesec_get_number(s)) == -1) return;
  }
  if ((sec < 1) || (sec > filesec_ops.max_sections))
  {
    print_str_cr("That filesection does not exist");
    return;
  }
  if (load_filesection(sec,&fildata))
  {
    if (data->priority > fildata.use_priority)
    {
      print_str_cr("Insufficient priority to access file section");
      return;
    }
    memcpy(&data->fildata,&fildata,sizeof(struct filesec_data));
    data->current_section = sec;
    return;
  }
  print_str_cr("Error loading filesection");
}

int load_file_entry(struct current_data *data, struct filesec_entry *entry, int entry_no)
{
  FILE *fp;
  char s[80];

  lock_dos(5004);
  sprintf(s,"FILESEC\\FILE%03d\\FILEDATA",data->current_section);
  if (!(fp=g_fopen(s,"rb","LDFLENTR")))
  {
    unlock_dos();
    return (0);
  }
  fseek(fp, sizeof(struct filesec_data) + (sizeof(struct filesec_entry) * entry_no), SEEK_SET);
  if (!fread(entry, sizeof(struct filesec_entry), 1, fp))
  {
    g_fclose(fp);
    unlock_dos();
    return (0);
  }
  g_fclose(fp);
  unlock_dos();
  return (1);
}

int save_file_entry(struct current_data *data, struct filesec_entry *entry, int entry_no)
{
  FILE *fp;
  char s[80];

  lock_dos(5005);
  sprintf(s,"FILESEC\\FILE%03d\\FILEDATA",data->current_section);
  if (!(fp=g_fopen(s,"rb+","SVFLENTR")))
  {
    unlock_dos();
    return (0);
  }
  fseek(fp, sizeof(struct filesec_data) + (sizeof(struct filesec_entry) * entry_no), SEEK_SET);
  fwrite(entry, sizeof(struct filesec_entry), 1, fp);
  g_fclose(fp);
  unlock_dos();
  return (1);
}

int add_save_file_entry(struct current_data *data, struct filesec_entry *entry)
{
  FILE *fp;
  char s[80];

  lock_dos(5006);
  sprintf(s,"FILESEC\\FILE%03d\\FILEDATA",data->current_section);
  if (!(fp=g_fopen(s,"rb+","SVFLENTR")))
  {
    unlock_dos();
    return (0);
  }
  entry->date_of = time(NULL);
  fseek(fp, 0, SEEK_END);
  fwrite(entry, sizeof(struct filesec_entry), 1, fp);

  fseek(fp, 0, SEEK_SET);
  fread(&data->fildata, sizeof(struct filesec_data), 1, fp);
  (data->fildata.num_files)++;
  fseek(fp, 0, SEEK_SET);
  fwrite(&data->fildata, sizeof(struct filesec_data), 1, fp);
  g_fclose(fp);
  unlock_dos();
  return (1);
}

void edit_file_entry(struct filesec_entry *entry, int function)
{
  int choice;
  long int chs;
  int x;
  char s[80];

  for (;;)
  {
    special_code(1,tswitch);
    print_cr();
	switch (function)
	{
	  case 1: print_str_cr("|*h1|*f6Editing File Entry|*r1");
			  break;
	  case 2: print_str_cr("|*h1|*f6Adding New File Entry|*r1");
			  break;
	}
    print_cr();
    special_code(0,tswitch);

    print_justified("1. Filename:",45);
    print_str_cr(entry->name);
    print_justified("2. Length:",45);
    sprintf(s,"%lu",entry->length);
    print_str_cr(s);
    print_justified("3. Active file:",45);
	print_str_cr(entry->active ? "Yes" : "No");
    print_justified("4. Approved file:",45);
	print_str_cr(entry->approved ? "Yes" : "No");
    print_justified("5. Uploaded by user #:",45);
	sprintf(s,"%d",entry->uploaded_by);
	print_str_cr(s);
    print_str_cr("6. Description:");
    print_chr('>');
	print_str_cr(entry->description);
    print_str_cr("Q. Quit file entry editor");

    print_cr();
    special_code(1,tswitch);
    print_string("|*h1|*f4Enter choice: (1-6) ");
    special_code(0,tswitch);
    get_string_cntrl(s,3,0,0,0,1,1,1,0);

    if (*s == 'Q') break;
	switch (choice=asctonum_long(s))
    {
      case  1: get_pr_string("Filename: ",entry->name,FILESEC_FILENAME_LENGTH,1,0);
               break;
      case  2: if ((chs = filesec_get_number("Length of file: ")) == -1) break;
               entry->length = chs;
               break;
      case  3:
      case  4: if (choice == 3) x = get_yes_or_no("Active? (Y/N) ");
                 else x = get_yes_or_no("Approved? (Y/N) ");
               if (x == -1) break;
               if (choice == 3) entry->active = x;
                  else entry->approved = x;
               break;
      case  5: if ((x = filesec_get_number("Uploaded by user #:")) == -1) break;
               entry->uploaded_by = x;
               break;
      case  6: print_str_cr("Description:");
               get_pr_string(">",entry->description,DESC_LENGTH,0,1);
               break;
      default: print_str_cr("You have entered an invalid option");
               break;
    }
  }
}


int load_file_entry_no_open(FILE *fp, struct filesec_entry *entry, int entry_no)
{
  fseek(fp, sizeof(struct filesec_data) + (sizeof(struct filesec_entry) * entry_no), SEEK_SET);
  if (!fread(entry, sizeof(struct filesec_entry), 1, fp)) return (0);
  return (1);
}

void list_file_entries(struct current_data *data, int first, int last, time_t after)
{
  int mid;
  struct filesec_entry entry;
  FILE *fp;
  int lines = 0;
  int ch;
  int shown = 0;
  char s[200];

  print_cr();
  lock_dos(5012);
  sprintf(s,"FILESEC\\FILE%03d\\FILEDATA",data->current_section);
  if (!(fp=g_fopen(s,"rb","LDFLENTY")))
  {
    unlock_dos();
    return;
  }

  if (!after) mid = first;
  else
  {
    int b_last = last;

    for (;;)
    {
      mid = (first+last) >> 1;
      if (!load_file_entry_no_open(fp,&entry,mid))
      {
        g_fclose(fp);
        unlock_dos();
        return;
      }
      if (after > entry.date_of)
      {
        if (first == mid) break;
        first = mid;
      } else
      if (after < entry.date_of)
      {
        if (last == mid) break;
        last = mid;
      } else break;
    }
    last = b_last;
  }

  while (mid < last)
  {
    if (!load_file_entry_no_open(fp,&entry,mid))
    {
      g_fclose(fp);
      unlock_dos();
      return;
    }
    if (entry.active)
    {
      sprintf(s,"|*h1|*f2%c%03d |*f4%-13s (#%03d) |*r1%9lu ",entry.approved ? '*' : ' ',mid+1,entry.name,
                entry.uploaded_by,entry.length);
      unlock_dos();
      ch = get_nchar(tswitch);
      if ((ch == 27) || (ch == 3))
      {
        empty_outbuffer(tswitch);
        print_cr();
        shown++;
        break;
      }
      special_code(1,tswitch);
      print_string(s);
      lines++;
      if (strlen(entry.description) > 35)
      {
        print_cr();
        print_chr(' ');
        print_str_cr(entry.description);
        lines++;
      } else print_str_cr(entry.description);
      special_code(0,tswitch);
      wait_for_xmit(tswitch,0);
      if (lines > 20)
      {
        print_string("[ Press Return ]");
        do
        {
          ch = wait_ch();
          if ((ch == 27) || (ch == 3))
          {
            mid = last;
            break;
          }
        } while (ch != 13);
        for (ch=0;ch<16;ch++)
          print_string(backspacestring);
        lines = 0;
      }
      shown++;
      lock_dos(5013);
    }
    mid++;
  }
  unlock_dos();
  g_fclose(fp);
  if (!shown) print_str_cr("No files matching search");
  return;
}

void add_a_file_entry(struct current_data *data)
{
  struct filesec_entry entry;

  *entry.description = 0;
  *entry.name = 0;
  entry.length = 0;
  entry.active = 0;
  entry.approved = 0;
  entry.uploaded_by = 0;
  edit_file_entry(&entry,2);
  if (!(*entry.name))
  {
    print_str_cr("No filename! Could not saved file entry");
    return;
  }
  print_string("File entry ");
  if (!add_save_file_entry(data,&entry)) print_string("not ");
  print_str_cr("saved");
}

void list_file_command(struct current_data *data, char *rest_of_line)
{
  int first, last;

  if (*rest_of_line == 'D')
  {
    list_file_entries(data,0,data->fildata.num_files,data->last_call);
    return;
  }
  first = asctonum_move(&rest_of_line);
  if ((first < 1) || (first >= data->fildata.num_files))
  {
    list_file_entries(data,0,data->fildata.num_files,0);
    return;
  }
  if (*rest_of_line != '-') list_file_entries(data,first-1,first,0);
   else
   {
     rest_of_line++;
     last = asctonum_move(&rest_of_line);
     if ((last < first) || (last >= data->fildata.num_files))
       list_file_entries(data,first-1,data->fildata.num_files,0);
       else list_file_entries(data,first-1,last,0);
   }
}

int enter_file_number(struct current_data *data)
{
  char s[80];
  int c;

  for (;;)
  {
    special_code(1,tswitch);
    print_cr();
    sprintf(s,"|*h1|*f4Enter file number (1-%d): ",data->fildata.num_files);
    print_string(s);
    special_code(0,tswitch);
    get_string_cntrl(s,10,0,0,0,1,1,1,0);
    if (*s == 'Q') return (-1);
    if (*s == 'L')
    {
      list_file_command(data,&s[1]);
      continue;
    }
    c = asctonum_long(s);
    if ((c >= 1) && (c <= data->fildata.num_files)) return (c);
    print_str_cr("Enter a valid file number, or 'Q' to quit, or 'L' to list files");
  }
}

void edit_a_file_entry(struct current_data *data, char *s)
{
  int eno;
  struct filesec_entry entry;

  eno = asctonum_long(s);
  if ((eno < 1) || (eno > data->fildata.num_files))
    if (!(eno = enter_file_number(data))) return;
  eno--;
  if (!load_file_entry(data,&entry,eno))
  {
    print_str_cr("Could not load file entry!");
    return;
  }
  edit_file_entry(&entry,1);
  save_file_entry(data,&entry,eno);
}

void compose_file_path(char *t, char *filename, struct current_data *data)
{
  sprintf(t,"FILESEC\\FILE%03d\\%s",data->current_section,filename);
}

void activate_file_entry(struct current_data *data, char *s)
{
  int eno;
  int x;
  FILE *fp;
  struct filesec_entry entry;
  char t[80];

  special_code(1,tswitch);
  print_cr();
  if (*s == 'V') print_str_cr("|*h1|*f4Approve file entry");
    else print_str_cr("|*h1|*f4Activate file entry");
  special_code(0,tswitch);
  eno = asctonum_long(&s[1]);
  if ((eno < 1) || (eno > data->fildata.num_files))
    if (!(eno = enter_file_number(data))) return;
  eno--;
  if (!load_file_entry(data,&entry,eno))
  {
    print_str_cr("Could not load file entry!");
    return;
  }
  special_code(1,tswitch);
  print_cr();
  print_string("|*f6File name is: |*h1");
  print_str_cr(entry.name);
  print_cr();
  if (*s == 'V')
  {
    print_string("|*h1|*f3File entry is currently |*f4");
    if (!entry.approved) print_string("not ");
    print_str_cr("approved|*r1");
    x = get_yes_or_no("|*h1|*f2Approve this file? ");
    if (x == -1) return;
    entry.approved = x;
  } else
  {
    if (!entry.active)
    {
      compose_file_path(t,entry.name,data);
      if (!(fp=g_fopen(t,"rb","FLACTIVE")))
      {
        print_str_cr("|*h1|*f2Entry file is deleted.  Can not reactivate");
        special_code(0,tswitch);
        return;
      }
      g_fclose(fp);
    }
    print_string("|*h1|*f3File entry is currently |*f4");
    if (!entry.active) print_string("not ");
    print_str_cr("active|*r1");
    x = get_yes_or_no("|*h1|*f2Activate this file?");
    if (x == -1) return;
    entry.active = x;
    if (!x)
    {
      x = get_yes_or_no("|*h1|*f2Delete this file?");
      if (x == -1) return;
      if (x)
      {
        compose_file_path(t,entry.name,data);
        remove(t);
        print_str_cr("File removed");
      }
    }
  }
  save_file_entry(data,&entry,eno);
}

void compress_filesection(struct current_data *data)
{
  int count = 0;
  struct filesec_entry entry;
  char t[80];
  char s[80];
  FILE *fp1;
  FILE *fp2;

  special_code(1,tswitch);
  print_cr();
  print_string("|*h1|*f2Compress filesection? (Y/N) ");
  special_code(0,tswitch);
  get_string_cntrl(t,1,0,0,0,1,1,1,0);
  if (*t != 'Y') return;

  sprintf(t,"FILESEC\\FILE%03d\\FILEDATA",data->current_section);
  sprintf(s,"FILESEC\\FILE%03d\\FILETEMP",data->current_section);
  lock_dos(5016);
  if (!(fp1=g_fopen(t,"rb","FLCOMRD")))
  {
    unlock_dos();
    return;
  }
  if (!(fp2=g_fopen(s,"wb","FLCOMWT")))
  {
    g_fclose(fp1);
    unlock_dos();
    return;
  }
  fwrite(&data->fildata,sizeof(struct filesec_data),1,fp2);
  fseek(fp1,sizeof(struct filesec_data),SEEK_SET);
  while (!feof(fp1))
  {
	if (fread(&entry,sizeof(struct filesec_entry),1,fp1))
	{
      if (entry.active)
      {
		fwrite(&entry,sizeof(struct filesec_entry),1,fp2);
        count++;
      }
    }
    unlock_dos();
	delay(2);
    lock_dos(5017);
  }
  data->fildata.num_files = count;
  fseek(fp2,0,SEEK_SET);
  fwrite(&data->fildata,sizeof(struct filesec_data),1,fp2);
  g_fclose(fp1);
  g_fclose(fp2);
  remove(t);
  rename(s,t);
  unlock_dos();
  print_str_cr("Compression completed");
}

int choose_protocol(void)
{
  int c;

  for (;;)
  {
	c = filesec_get_number("|*h1|*f1Protocol |*f60-|*f4Xmodem, |*f61-|*f4Xmodem CRC, |*f62-|*f4Xmodem 1K, |*f63-|*f4Y-modem Batch: ");
    if (c == -1) return (-1);
    if ((c >= 0) && (c <= 3)) return (c);
    print_cr();
    print_str_cr("Invalid protocol number");
  }
}

void list_batch(struct current_data *data)
{
  int x;
  char t[81];
  long int total = 0;

  special_code(1,tswitch);
  print_cr();
  if (!data->num_files)
  {
    print_str_cr("|*h1|*f6No files to send");
    special_code(0,tswitch);
    return;
  }
  print_str_cr("|*h1|*f6Files to send:|*r1");
  print_str_cr("    |*h1|*f7Section Length    Name");
  for (x=0;x<data->num_files;x++)
  {
    sprintf(t,"|*h1|*f7%03d     |*f5%-3d |*f4%-9ld |*f6%s",x+1,data->filesections[x],data->filelengths[x],
            data->filepointers[x]);
    total += data->filelengths[x];
    print_str_cr(t);
  }
  print_cr();
  sprintf(t,"|*h1|*f7Total bytes %ld|*r1",total);
  print_str_cr(t);
  if (data->baud_rate)
  {
    total /= (long int)(data->baud_rate / 11);
    sprintf(t,"|*h1|*f6Total transfer time |*f5%02ld:%02ld|*r1",total / 60,total % 60);
    print_str_cr(t);
  }
  special_code(0,tswitch);
}

void remove_junk(char *s, char *t)
{
  char *l = s;

  while (*t)
  {
    if (*t == '\\') l = s;
     else *l++ = *t;
    t++;
  }
  *l = 0;
}

void send_protocol(struct current_data *data, char *s, int batch)
{
  int eno;
  int proto;
  int x;
  struct filesec_entry entry;
  char t[81];

  if (!batch)
  {
    eno = asctonum_long(&s[1]);
    if ((eno < 1) || (eno > data->fildata.num_files))
      if (!(eno = enter_file_number(data))) return;
    eno--;
    if (!load_file_entry(data,&entry,eno))
    {
      print_str_cr("Could not load file entry!");
      return;
    }
    if (!entry.active)
    {
      print_str_cr("Entry is not active!");
      return;
    }
    if ((!entry.approved) && (!super(data)))
    {
      print_str_cr("Entry is not approved for download");
      return;
    }
    if ((proto=choose_protocol()) == -1) return;

    if (data->num_files >= MAX_FILENAMES)
    {
      print_str_cr("No more files can be sent");
      return;
    }
    if (proto != 3) data->num_files = 0;
    strcpy(data->filepointers[data->num_files],entry.name);
    data->filesections[data->num_files] = data->current_section;
    data->filelengths[data->num_files++] = entry.length;
    if (proto == 3)
      if ((x = get_yes_or_no("|*h1|*f4Send file(s) now?")) != 1) return;
  } else proto = 3;
  if (!data->num_files)
  {
    print_str_cr("No files to send");
    return;
  }
  list_batch(data);
  print_cr();
  print_string("Press RETURN to send file(s), or ESCAPE to abort...");
  special_code(0,tswitch);
  do
  {
    x = wait_ch();
    if ((x == 'Q') || (x == 'q') || (x == 27) || (x == 3))
    {
      print_cr();
      if (proto != 3) data->num_files = 0;
      return;
    }
  } while (x != 13);
  print_cr();
  print_str_cr("Sending file(s)...");
  print_str_cr("Hit CTRL-X many times to cancel");
  for (x=0;x<data->num_files;x++)
  {
    compose_file_path(t,data->filepointers[x],data);
    strcpy(data->filepointers[x],t);
  }
  switch (proto)
  {
    case 0:
    case 1:
    case 2: send_files(data->filepointers,1,proto);
            data->num_files = 0;
            break;
	case 3: if (send_files(data->filepointers,data->num_files,proto))
            {
              print_cr();
              print_str_cr("Error in transfer");
              for (x=0;x<data->num_files;x++)
              {
                remove_junk(t,data->filepointers[x]);
                strcpy(data->filepointers[x],t);
              }
            } else data->num_files = 0;
            break;
  }
  do
  {
    print_cr();
    print_string("Type 'GO' to end transfer: ");
    get_string_cntrl(t,2,0,0,0,0,1,1,0);
  } while (strcmp(t,"GO"));
}

void receive_protocol(struct current_data *data)
{
  int proto;
  int num_fl;
  int ct;
  FILE *fp;
  char t[80];
  struct filesec_entry entry;

  if ((proto=choose_protocol()) == -1) return;
  data->num_files = 0;
  if (proto != 3)
  {
    if (get_pr_string("Filename to upload: ",*data->filepointers,FILESEC_MAX_FILENAME_LENGTH,1,0) == -1) return;
    compose_file_path(t,*data->filepointers,data);
    if (fp=g_fopen(t,"rb","RPROTO"))
    {
      g_fclose(fp);
      print_str_cr("That file already exists!");
      return;
    }
    data->num_files++;
  }
  print_cr();
  print_str_cr("Receiving file(s) now...");
  print_str_cr("Hit CTRL-X many times to cancel");
  sprintf(t,"FILESEC\\FILE%03d",data->current_section);
  num_fl = MAX_FILENAMES;
  switch (proto)
  {
    case 0:
    case 1:
	case 2: receive_files(t,data->filepointers,&num_fl,proto);
			break;
	case 3: receive_files(t,data->filepointers,&num_fl,proto);
            break;
  }
  do
  {
    print_cr();
    print_string("Type 'GO' to end transfer: ");
    get_string_cntrl(t,2,0,0,0,0,1,1,0);
  } while (strcmp(t,"GO"));
  print_cr();
  sprintf(t,"Files received: %d",num_fl);
  print_str_cr(t);
  for (ct=0;ct<num_fl;ct++)
  {
    strncpy(entry.name,data->filepointers[ct],FILESEC_MAX_FILENAME_LENGTH);
    entry.name[FILESEC_MAX_FILENAME_LENGTH+1] = 0;
    lock_dos(5018);
    entry.date_of = time(NULL);
    unlock_dos();
    entry.approved = ((filesec_ops.auto_approve) && (data->fildata.auto_approve));
    entry.downloads = 0;
    entry.uploaded_by = data->user_number;
    compose_file_path(t,entry.name,data);
	if (fp=g_fopen(t,"rb","R2PROTO"))
    {
	  fseek(fp,0,SEEK_END);
      entry.length = ftell(fp);
      g_fclose(fp);
      print_cr();
      special_code(1,tswitch);
      print_string("|*h1|*f2File: |*f4");
      print_str_cr(entry.name);
      print_string("|*f3Enter description: ");
      special_code(0,tswitch);
      get_pr_string(">",entry.description,DESC_LENGTH,0,1);
      add_save_file_entry(data,&entry);
    }
  }
}

int get_information(struct current_data *data)
{
  data->baud_rate = line_status[tswitch].baud;
  data->priority = user_lines[tswitch].class_info.priority;
  data->user_number = user_lines[tswitch].user_info.number;
  data->last_call = user_lines[tswitch].user_info.last_call;
  return (1);
}

int filesec_entry(void)
{
  int count;
  struct current_data data;
  char s[200];

  if (!get_information(&data))
  {
    print_str_cr("Could not load user information");
    return (0);
  }
  data.current_section = 1;
  load_filesection_options();
  if (!load_filesection(1,&data.fildata))
  {
    print_str_cr("Could not load filesection #1!");
    return (1);
  }
  data.filename_buffer = g_malloc(MAX_FILENAMES*(FILESEC_MAX_FILENAME_LENGTH+1),"FSBUF");
  if (!data.filename_buffer)
  {
    print_str_cr("Could not allocate file buffer memory");
    return (1);
  }
  init_batch_files(&data);
  print_file_to_cntrl(filesec_ops.open_file, tswitch, 1, 1, 1, 1);
  for (;;)
  {
    special_code(1,tswitch);
    print_cr();
    sprintf(s,"|*h1|*f2File section |*f3(#%d) |*f4%s |*r1|*h1|*f2: (1-%d) ",data.current_section, data.fildata.name,
            data.fildata.num_files);
    print_string(s);
    special_code(0,tswitch);

    get_string_cntrl(s,4,0,0,0,1,1,1,0);
    if (*s == 'Q')
    {
      special_code(1,tswitch);
      print_str_cr("--> |*h1|*f2Leaving file section");
      special_code(0,tswitch);
      break;
    }
    if (*s)
    {
      switch (*s)
      {
        case 'E':  if (data.priority > filesec_ops.filesec_pri)
                   {
                     print_str_cr("Insufficient priority to edit filesection options");
                     break;
                   }
                   edit_filesection_options();
                   save_filesection_options();
                   break;
        case 'F':  if (!super(&data))
                   {
                     print_str_cr("Can not edit file section");
                     break;
                   }
                   edit_filesection(&data.fildata);
				   save_filesection(data.current_section,&data.fildata);
                   break;
		case 'J':  change_filesection(&data,asctonum_long(&s[1]));
                   break;
        case 'A':  if (!super(&data))
                   {
                     print_str_cr("Can not add records to file section");
                     break;
                   }
                   add_a_file_entry(&data);
                   break;
        case 'L':  if (s[1] == 'B') list_batch(&data);
                     else list_file_command(&data,&s[1]);
                   break;
        case 'N':  if (!super(&data))
                   {
                     print_str_cr("Can not edit records in file section");
                     break;
                   }
                   edit_a_file_entry(&data,&s[1]);
                   break;
        case 'K':  if (!super(&data))
                   {
                     print_str_cr("Can not compress records in file section");
                     break;
                   }
                   compress_filesection(&data);
                   break;
        case 'V':
        case 'X':  if (!super(&data))
                   {
                     if (*s == 'V') print_str_cr("Can not mark records approved in file section");
					   else print_str_cr("Can not mark records active in file section");
                     break;
                   }
                   activate_file_entry(&data, s);
                   break;
        case '?':  print_cr();
                   print_file_to_cntrl("HELP\\FILESEC.HLP", tswitch, 1, 1, 1, 1);
                   if (super(&data))
                   {
                     print_cr();
                     print_file_to_cntrl("HELP\\FILEADMN.HLP", tswitch, 1, 1, 1, 1);
                   }
                   break;
        case 'B':
		case 'D':  if ((data.priority > data.fildata.dl_priority) && (data.user_number != data.fildata.admin_user))
                   {
					 print_str_cr("Insufficient priority to download files");
                     break;
                   }
                   send_protocol(&data, s, (*s == 'B') ? 1 : 0);
                   break;
        case 'C':  data.num_files = 0;
                   print_str_cr("Batched files cleared");
                   break;
		case 'U':  if ((data.priority > data.fildata.ul_priority) && (data.user_number != data.fildata.admin_user))
                   {
					 print_str_cr("Insufficient priority to upload files");
                     break;
                   }
                   receive_protocol(&data);
                   break;
        default:   print_str_cr("Command not found");
                   break;
      }
    }
  }
  g_free(data.filename_buffer);
  return (1);
}

