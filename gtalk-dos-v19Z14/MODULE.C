/* MODULE.C */

/* (C) Copyright 1993 by Daniel Marks and David Jeske */

#undef DEBUG
#define MAX_NUM_BOTS 12

#define CURRENT_GLM_VERSION 0

#include "include.h"
#include "gtalk.h"
#include "module.h"

/* prototypes */
int get_server(void);
void get_registered_bot_name_for_myself(char *string,int string_len);

int num_shared = 0;
struct shared_glm_entry *glm_entries[MAX_THREADS];


struct jumptable far ginsu_jumptbl =
  {
    /* function jump locations */

    vsprintf,
    print_str_cr,
    get_string_cntrl,
    lock_dos,
    unlock_dos,
    time,
    islocked,

    g_fopen,
    g_fclose,
    g_flush,
    g_free,
    g_malloc_main_only,
    g_malloc,

    fread,
    fwrite,
    fseek,
    ftell,

    initabuffer,
    aget_abuffer,
    aput_into_buffer,
    aput_vargs_into_buffer,
    dealloc_abuf,
    unregister_bot_myself,
    register_bot,
    change_my_info_line,
    st_copy,
    print_file_to_cntrl,
    wait_ch,
    get_server,

    line_editor,
    delay,
    rename,
    findfirst,
    findnext,
    log_error,
    remove,
    send_files,
    receive_files,
    mkdir,
    rmdir,

    print_chr,
    print_string,
    print_cr,
    position,
    foreground,
    background,
    special_code,
    reset_attributes,
    blink_video,
    bold_video,
    wrap_line,

    next_task,
    check_for_privates,
    getcwd,
    broadcast_message,

    /* global variables */

    &tswitch,
    &a_chars_in_buffer,
    &a_dcd_detect,
    &a_put_char_in_buffer,
    &a_get_char,
    &a_send_char,
    &a_empty_inbuffer,
    &a_char_in_buf,
    &a_get_first_char,
    &a_get_nchar,
    &a_wait_for_xmit,
    &a_empty_outbuffer,
    &a_change_dtr_state,
    &dans_counter,

    /* new functions */

    getdisk,
    getdfree,
    getcurdir,
    get_registered_bot_name_for_myself,
    read_my_channel,
    change_my_channel

};

void show_shared_glms(char *str, char *name, int portnum)
{
  char s[100];
  int show;
  int spid;
  struct shared_glm_entry *entry;

  print_str_cr("Name      Length Address   Pid List");
  if (!num_shared) print_str_cr("None");
  for (show=0;show<num_shared;show++)
  {
    entry = glm_entries[show];
    sprintf(s,"%-10s%-7lu%-10p",entry->glmname,entry->length,entry->start_prog);
    print_string(s);
    for (spid=0;spid<entry->number_of_tasks;spid++)
    {
      if (spid) print_string(", ");
      sprintf(s,"%d",entry->pid_list[spid]);
      print_string(s);
    }
    print_cr();
  }
}


void delete_pid_from_entry(struct shared_glm_entry *entry, int pid)
{
  int search = entry->number_of_tasks;
  int del;

  while (search>0)
  {
    search--;
    if (entry->pid_list[search] == pid)
    {
      entry->number_of_tasks--;
      for (del=search;del<entry->number_of_tasks;del++)
        entry->pid_list[del] = entry->pid_list[del+1];
      break;
    }
  }
  if (!entry->number_of_tasks)
  {
    g_free(entry);
    search = num_shared;
    while (search>0)
    {
      search--;
      if (glm_entries[search] == entry)
      {
        num_shared--;
        for (del=search;del<num_shared;del++)
          glm_entries[del] = glm_entries[del+1];
        break;
      }
    }
  }
}

struct shared_glm_entry *search_glms_for_program(char *glmname)
{
  int search;
  struct shared_glm_entry **glmlist = glm_entries;

  for (search=0;search<num_shared;search++)
  {
    if (!strcmp(glmname,(*glmlist)->glmname))
      return (*glmlist);
    glmlist++;
  }
  return(NULL);
}

struct shared_glm_entry *init_shared(struct startblock *ourblock, FILE *fp)
{
  struct shared_glm_entry *glm_entry;

  if (ourblock->shared == NOT_SHARED) return (NULL);
  if (num_shared >= MAX_THREADS) return (NULL);
  if (!(glm_entry=search_glms_for_program(ourblock->glmname)))
  {
    glm_entry = g_malloc_with_owner(ourblock->length+0xF+
        sizeof(struct shared_glm_entry),ourblock->glmname,tswitch,0,0);
    if (!glm_entry) return (NULL);
    glm_entries[num_shared] = glm_entry;
    num_shared++;
    glm_entry->length = ourblock->length;
    glm_entry->number_of_tasks = 0;
    strncpy(glm_entry->glmname,ourblock->glmname,GLM_NAME_LEN);
    glm_entry->glmname[GLM_NAME_LEN] = 0;
    glm_entry->start_prog = (remote_func) &glm_entry[1];
    glm_entry->start_prog = (remote_func)
         MK_FP((FP_SEG(glm_entry->start_prog)+
         (((FP_OFF(glm_entry->start_prog)+0xF)>>4))),0x0);
    fseek(fp,ourblock->glm_header_len,SEEK_SET);
    fread((void *)glm_entry->start_prog,sizeof(char),
          ourblock->length,fp);
  }
  if (glm_entry->number_of_tasks >= MAX_THREADS) return (NULL);
  glm_entry->pid_list[glm_entry->number_of_tasks] =
     pid_of(tswitch);
  (glm_entry->number_of_tasks)++;
  return (glm_entry);
}

remote_func init_unshared(struct startblock *ourblock, FILE *fp,
                          void **memblock, int ems)
{
  remote_func start_prog;

  if (ourblock->shared == IS_SHARED) return (NULL);
  if (!(*memblock = g_malloc_with_owner(ourblock->length+0xF,
       ourblock->glmname,tswitch,ems ? 2 : 0, 1)))
       return (NULL);
  start_prog = (remote_func)
    MK_FP((FP_SEG(*memblock)+
    (((FP_OFF(*memblock)+0xF)>>4))),0x0);
  fseek(fp,ourblock->glm_header_len,SEEK_SET);
  fread(start_prog,sizeof(char),ourblock->length,fp);
  return (start_prog);
}

int run_glm(char *filename, void *pass_ptr, int shared)
{
  FILE *fp;
  int flag = !islocked(DOS_SEM);
  struct shared_glm_entry *glm_entry;
  remote_func start_prog;
  void *memblock;
  int oldDS, oldES;
  int return_code;
  struct startblock ourblock;
  struct jumptable far *jtemp = &ginsu_jumptbl;
  int is_shared = 0;
  int bomb = 0;

  if (flag) lock_dos(1000);
#ifdef DEBUG
  print_str_cr("opening file");
#endif
  if (!(fp=g_fopen(filename,"rb","SHMODULE")))
  {
    if (flag) unlock_dos();
    return (0);
  }
  fread(&ourblock,sizeof(struct startblock),1,fp);
  if ((strcmp(LD_STARTID,ourblock.initid)) ||
     (ourblock.glm_ver_no > CURRENT_GLM_VERSION)) bomb = 1;
  else
  {
    if ((shared == LOAD_SHARED) || (shared == LOAD_SHARED_FIRST))
    {
      if (glm_entry = init_shared(&ourblock,fp)) is_shared = 1;
       else if (shared == LOAD_SHARED) bomb = 1;
        else if (!(start_prog=init_unshared(&ourblock,fp,&memblock,0)))
               bomb = 1;
          else is_shared = 0;
    } else
    {
      if (start_prog=init_unshared(&ourblock,fp,&memblock,
              shared != LOAD_UNSHARED)) is_shared = 0;
       else if (shared == LOAD_UNSHARED) bomb = 1;
         else if ((glm_entry = init_shared(&ourblock,fp))) is_shared = 1;
          else if (start_prog = init_unshared(&ourblock,fp,&memblock,0))
              is_shared = 0;
            else bomb = 1;
    }
  }
  g_fclose(fp);
  if (flag) unlock_dos();
  if (bomb) return (0);
  if (is_shared) start_prog = glm_entry->start_prog;
  oldDS = _DS;
  oldES = _ES;
  _ES = _DS = FP_SEG(start_prog);
  return_code=(*start_prog)(jtemp,oldDS,oldES,pass_ptr);
  _DS = oldDS;
  _ES = oldES;
  if (flag) lock_dos(1001);
  if (is_shared) delete_pid_from_entry(glm_entry,pid_of(tswitch));
   else g_free(memblock);
  if (flag) unlock_dos();
  return (return_code);
}

void run_module(char *str, char *name, int portnum)
{
  char filename[12];
  char *t=filename;

  int lettercount=8;
  while (*str==' ') str++;
  while ((lettercount>0) && (*str))
  {
    if ((*str>='A') && (*str<='Z')) *t++=*str;
     else
    if ((*str>='a') && (*str<='z')) *t++=(*str-' ');
     else lettercount = 0;
    str++;
  }
  strcpy(t,".GLM");
  print_string("Loading module ");
  print_str_cr(filename);
  run_glm(filename,NULL,LOAD_UNSHARED);
}


void run_sysop_section(char *str, char *name, int portnum)
{

  if (!get_password("Master",sys_info.master_password,1))
       return;

  if (run_glm("glm\\sysop.glm",NULL,LOAD_UNSHARED_FIRST) != 1)
    print_sys_mesg("Error in loading or execution of SYSOP GLM");
}



/* BOT registration */
struct bot_struct system_bots[MAX_NUM_BOTS];
int num_bots=0;

void print_bot_list(char *str, char *name, int portnum)
{
  int count=0;
  char s[200];
  struct bot_struct *curbot = system_bots;

  if (!num_bots)
  {
    print_str_cr("--> No Bots.");
    return;
  }
  print_str_cr("Channel Name       PID   TaskNum   InfoLine ");
  while (count<num_bots)
  {
    if (curbot->channel<0)
        print_string("  N/A  ");
    else
       { sprintf(s,"  %02d   ",curbot->channel);
         print_string(s);
        }
    sprintf(s," %-9s %05u    %-8d%s",curbot->name,
          curbot->pid,curbot->portnum,curbot->info_line);

    print_str_cr(s);
    count++;
    curbot++;
  }
  print_cr();
}


void get_registered_bot_name_for_myself(char *string,int string_len)
{
   int bot_num = find_bot_pid(pid_of(tswitch));

   if (bot_num<0)
     {
       if (string_len)
         *string=0;
       return;
     }

   strncpy(string,system_bots[bot_num].name,string_len);
   string[string_len-1] = 0;
}

int find_bot_pid(int pid)
{
  int count;
  struct bot_struct *curbot = system_bots;

  for (count=0;count<num_bots;count++)
  {
    if (pid == curbot->pid) return (count);
    curbot++;
  }
  return (-1);
}

int find_bot_by_name(char *name, char **last_end)
{
  int count = 0;
  struct bot_struct *curbot = system_bots;
  int maxmatch = 0, match;
  int bestmatch = -1;
  char *cp1, *cp2;
  char ch1, ch2;
  int exact_match = 0;

  while (count<num_bots)
  {
    cp1 = name;
    cp2 = curbot->name;
    match = 0;
    while ((*cp1) && (*cp2))
    {
      ch1 = ((*cp1>='a') && (*cp1<='z')) ? (*cp1 - ' ') : *cp1;
      ch2 = ((*cp2>='a') && (*cp2<='z')) ? (*cp2 - ' ') : *cp2;
      if (ch1 != ch2) break;
      match++;
      cp1++;
      cp2++;
    }
    if (match > maxmatch)
    {
      maxmatch = match;
      bestmatch = count;
      if (((!(*cp1)) || (*cp1 == ' ')) && (!(*cp2))) exact_match = 1;
      *last_end = cp1;
    } else if ((match == maxmatch) && (!exact_match)) bestmatch = -1;
    count++;
    curbot++;
  }
  return (bestmatch);
}



int unregister_bot(int pid)
{
  int entry;
  int flag = !islocked(DOS_SEM);

  if (flag) lock_dos(778);
  if ((entry=find_bot_pid(pid)) == -1)
  {
    if (flag) unlock_dos();
    return 0;
  }
  num_bots--;
  for (;entry<num_bots;entry++)
   system_bots[entry] = system_bots[entry+1];
  if (flag) unlock_dos();
  return 1;
}

int unregister_bot_myself(void)
{
  return (unregister_bot(pid_of(tswitch)));
}

int change_my_info_line(char *newline)
{
  int entry;
  int flag = !islocked(DOS_SEM);

  if (flag) lock_dos(778);
  if ((entry=find_bot_pid(pid_of(tswitch))) == -1)
  {
    if (flag) unlock_dos();
    return 0;
  }
  strncpy(system_bots[entry].info_line,newline,GLM_INFO_LINE_LEN);
  system_bots[entry].info_line[GLM_INFO_LINE_LEN] = 0;
  if (flag) unlock_dos();
  return 1;
}

int change_my_channel(int channel)
{

  int bot_num = find_bot_pid(pid_of(tswitch));

  if (bot_num<0)
   return 0;

  system_bots[bot_num].channel = channel;
  return 1;

}

int read_my_channel(void)
{
  int bot_num = find_bot_pid(pid_of(tswitch));

  if (bot_num<0)
   return -1;
  else
   return (system_bots[bot_num].channel);
}

void message_bot(char *str, char *name, int portnum)
{
   int entry;
   int flag = !islocked(DOS_SEM);
   char *message;
   struct bot_struct *curbot;
   char s[80];

   if (flag) lock_dos(790);
   while (*str==' ') str++;

   if ((entry=find_bot_by_name(str,&message)) == -1)
   {
     if (flag) unlock_dos();
     print_str_cr("--> Nonexistent or ambigious bot name");
     return;
   }
   while ((*message != ' ') && (!(*message))) message++;
   while (*message == ' ') message++;
   curbot = &system_bots[entry];
   aput_into_buffer(curbot->portnum,message,0,1,tswitch,
                        curbot->portnum,0);
   sprintf(s,"--> sent to \"%s\" bot",curbot->name);
   if (flag) unlock_dos();
   print_str_cr(s);
   return;
}

int register_bot(char *orig_name)
{
    int flag = !islocked(DOS_SEM);
    int name_taken=0;
    char name[GLM_NAME_LEN+1];
    char *last_char;
    int count=0;

    if (num_bots>=MAX_NUM_BOTS)
       return 0;
    strncpy(name,orig_name,GLM_NAME_LEN);
    name[GLM_NAME_LEN]=0;
    last_char = name;

    while (*last_char) last_char++;
    while ((last_char - name)>=(GLM_NAME_LEN)) last_char--;
    *(last_char+1) = 0;

    if (flag) lock_dos(777);

    if (find_bot_pid(pid_of(tswitch)) != -1)
    {
      if (flag) unlock_dos();
      return 0;
    }

    while (count<num_bots)
    {
      while ((count<num_bots) && (!name_taken))
      {
        if (!strcmp(system_bots[count].name,name))
        {
          name_taken=1;
          count=0;
        }
        else count++;
      }
      if (name_taken)
      {
        if ((*last_char<'9') && (*last_char>='1')) (*last_char)++;
        else
        if (*last_char=='9')
        {
          if (flag) unlock_dos();
          return 0;
        }
        else
        *last_char='1';
      }
      name_taken=0;
    }

    strncpy(system_bots[num_bots].name,name,GLM_NAME_LEN+1);
    system_bots[num_bots].pid = pid_of(tswitch);
    system_bots[num_bots].portnum = tswitch;
    system_bots[num_bots].channel = -1;
    num_bots++;

    if (flag) unlock_dos();
    return 1;
}

/* end BOT registration */



/* runbot code */

struct run_bot_struct
{
  int got_info;
  char filename[FILENAME_LEN+1];
};


void run_bot_event(void)
{
  struct run_bot_struct far *temp=(struct run_bot_struct far *)schedule_data[tswitch];
  struct run_bot_struct bot_info;

  bot_info = *temp;
  temp->got_info=1;

  run_glm(bot_info.filename,NULL,LOAD_UNSHARED);
  end_task();
}

void run_a_bot(char *str,char *name,int portnum)
{
  struct run_bot_struct transfer_struct;
  char filename[12];
  int count=0;
  int success=0;
  char *t=filename;

  int lettercount=8;
  while (*str==' ') str++;
  while ((lettercount>0) && (*str))
  {
    if ((*str>='A') && (*str<='Z')) *t++=*str;
     else
    if ((*str>='a') && (*str<='z')) *t++=(*str-' ');
     else lettercount = 0;
    str++;
  }
  strcpy(t,".GLB");
  print_string("Loading BOT :");
  print_str_cr(filename);

  strcpy(transfer_struct.filename,filename);
  transfer_struct.got_info=0;

  print_string("--> Launching Bot.");

    add_task_to_scheduler((task_type) run_bot_event,
      (void *)&transfer_struct, REL_SHOT_TASK, 0,1,1024, "BOTRUN");
    delay(5);

  while ((count<10) && (!success))
   {
    if (transfer_struct.got_info)
       success=1;
    else
       { delay(18);
         print_chr('.');
       }
   }
   print_cr();

   if (success)
     print_str_cr("--> Bot loaded correctly");
   else
     print_str_cr("--> Bot failed to load");
}

/* end runbot code */


/* Structure Copy function */

int st_copy(void *to,size_t size,int which_structure,int data)
{




    switch(which_structure)
     {
        case USER_LINES:
                    if (data>num_ports)
                      return 0;
                    if ((size) != (sizeof(struct user_data)))
                      return 0;
                    memcpy(to,&user_lines[data],size);
                    return 1;
        case USER_OPTIONS:
                    if (data>num_ports)
                      return 0;
                    memcpy(to,&user_options[data],size);
                    return 1;
        case LINE_STATUS:
                    if (data>num_ports)
                      return 0;
                    memcpy(to,&line_status[data],size);
                    return 1;
        case SYS_INFO:
                    memcpy(to,&sys_info,size);
                    return 1;
        case SYS_TOGGLES:
                    memcpy(to,&sys_toggles,size);
                    return 1;
        case ABUF_STATUS:
                    return 0;
        default:    return 0;
     }

}

/* end structure copy function */

int get_server(void)
{ return (server); }

#define DEF_SPLIT_SIZE 512






/*

int set_default(char *filename, char *default, char *value, int max_len)
{
  FILE *fp;
  int flag = !islocked(DOS_SEM);
  char *buffer = g_malloc(DEF_SPLIT_SIZE*2,"DEFWRITE");
  char *half2 = &buffer[DEF_SPLIT_SIZE];
  char *endbuf = &buffer[DEF_SPLIT_SIZE*2];
  char *compareptr;
  char *found;
  int block = 0;
  int flag;
  int deflen = strlen(default);

  if (!buffer) return 0;

  if (flag) lock_dos(790);
  if (!(fp=g_fopen(filename,"rb+","DEFWRITE")))
  {
    if (!(fp=g_fopen(filename,"wb+","DEFCREAT")))
    {
      if (flag) unlock_dos();
      g_free(buffer);
      return (0);
    }
  }
  memset(buffer,0,DEF_SPLIT_SIZE);
  flag = fread(buffer,sizeof(char),DEF_SPLIT_SIZE,fp);
  while (flag && (!found))
  {
    memset(half2,0,DEF_SPLIT_SIZE);
    flag = fread(half2,sizeof(char),DEF_SPLIT_SIZE,fp);
    while ((compareptr < half2) && (!found) && (*compareptr))
    {
      compareptr = buffer;
      found = NULL;
      if (!strncmp(compareptr,default,deflen))
          found = compareptr;
      while ((*compareptr) && (compareptr < half2) &&
            ((*(int *)compareptr) == 0x0A0D))
         compareptr++;
    }
    if (!found)
    {
      block += 1;
      memcpy(buffer,half2,DEF_SPLIT_SIZE);
      memset(half2,0,DEF_SPLIT_SIZE);
      flag = fread(half2,sizeof(char),
    }
  }


    while (compareptr<half2)
    {
      if ((*(int *)compareptr) == 0x0A0D)
      {
        compareptr += 2;
        if (strncmp(compareptr,
*/

#define DEF_BUF_SIZE 512

int get_default(char *filename, char *def_name, char *value, int max_len)
{
  FILE *fp;
  int lockflag = !islocked(DOS_SEM);
  char *buffer = g_malloc(DEF_BUF_SIZE,"DEFREAD");
  char *endbuf;
  char *curbufptr = buffer;
  unsigned long int absolute_position=0;
  unsigned long int absolute_pos_of_begin_line=0;
  char *curmatchptr = def_name;
  char *substringptr = def_name;
  char *substringptr2 = def_name;
  char *substringsave;
  int flag;
  char *def_nameend = def_name;

  /* find the end of the def_name first */
  while (*def_nameend)
    def_nameend++;

  if (!buffer) return 0;

  if (lockflag) lock_dos(790);
  if (!(fp=g_fopen(filename,"rb+","DEFREAD")))
  {
    print_str_cr_to("Failed to open file",0);
    return 0;
    /*
    if (!(fp=g_fopen(filename,"wb+","DEFCREAT")))
    {
      if (lockflag) unlock_dos();
      g_free(buffer);
      return (0);
    }
    */
  }
  memset(buffer,0,DEF_BUF_SIZE);

  flag = fread(buffer,sizeof(char),DEF_BUF_SIZE,fp);
  endbuf = &buffer[flag];

  if (lockflag) unlock_dos();

  while (flag && (curmatchptr < def_nameend))
  {
       curmatchptr = def_name;

       if ((*curbufptr==10) || (*curbufptr==13))
        {
         absolute_pos_of_begin_line = absolute_position;

           while (((*curbufptr==10) || (*curbufptr==13)) && (curbufptr<endbuf))
            {
            curbufptr++;
            absolute_position++;
           }

           if (curbufptr==endbuf) /* read the next block */
             {
                 if (lockflag) lock_dos(791);
                 flag = fread(buffer,sizeof(char),DEF_BUF_SIZE,fp);
                 if (lockflag) unlock_dos();
                 endbuf = &buffer[flag];
                 curbufptr = buffer;
             }

           while ((*curbufptr == *curmatchptr) && (curmatchptr<def_nameend))
               {
                    curmatchptr++;

                    /* advance the pointers */
                    curbufptr++;
                    absolute_position++;

                    if (curbufptr==endbuf) /* read the next block */
                      {
                          if (lockflag) lock_dos(791);
                          flag = fread(buffer,sizeof(char),DEF_BUF_SIZE,fp);
                          if (lockflag) unlock_dos();
                          endbuf = &buffer[flag];
                          curbufptr = buffer;
                      }

               }
       }
      else
      {
          curbufptr++;
          absolute_position++;

            if (curbufptr==endbuf) /* read the next block */
              {
                  if (lockflag) lock_dos(791);
                  flag = fread(buffer,sizeof(char),DEF_BUF_SIZE,fp);
                  if (lockflag) unlock_dos();
                  endbuf = &buffer[flag];
                  curbufptr = buffer;
              }

       }

     }

     if (curmatchptr!=def_nameend)
      return 0;

     /* ok, we found the string, now read starting at that position */

     if (lockflag) lock_dos(792);
     fseek(fp,absolute_pos_of_begin_line,SEEK_SET);
     flag = fread(buffer,sizeof(char),DEF_BUF_SIZE,fp);
     if (lockflag) unlock_dos();
     endbuf = &buffer[flag];

     print_str_cr("Found String:");

     while (((*buffer==10) || (*buffer==13)) && (buffer<endbuf))
      buffer++;

     while ((*buffer!=10) && (buffer<endbuf))
      print_chr(*(buffer++));
     print_cr();
     return 1;

  }


int is_bot_on_channel(int channel)
{
 int count;

   for(count=0;count<num_bots;count++)
    {
      if (system_bots[count].channel==channel)
        return 1;
    }

 return 0;
}

