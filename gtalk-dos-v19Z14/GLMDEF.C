/* GLM CODE to be distributed in OBJ or LIB form */
/* GLMDEF.C */

#include <stdio.h>
#include <dir.h>
#include <dos.h>
#include <stdarg.h>
#include <time.h>
#include "jumptbl.h"
#include "glmdef.h"

int g_sprintf(char far *str, char far *format, ...)
{
  va_list ap;
  int b = _DS;
  int result;
  struct jumptable far *jump = jmptl;

  va_start(ap,format);
  _ES = old_ES;
  _DS = old_DS;
  result=(jump->vsprintf)(str,format,ap);
  _ES = _DS = b;
  va_end(format);

  return (result);
}

int get_server(void)
{

  int b = _DS;
  int result;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  result = (jump->get_server)();
  _ES = _DS = b;
  return (result);
}

int st_copy(void far *to,size_t size,int which_structure,int data)
{
  int b = _DS;
  int result;

  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  result = (jump->st_copy)(to,size,which_structure,data);
  _ES = _DS = b;
  return (result);
}


void print_str_cr(char far *string)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  (jump->print_str_cr)(string);
  _ES = _DS = b;
}

int get_string_cntrl(char *string, int limit, char echo, char back_to_end,
    char escape, char noblankline, char cr_on_blankline,
    char upcase, char onlynum)
{
  int b = _DS;
  int result;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  result=(jump->get_string_cntrl)(string,limit,echo,back_to_end,escape,
        noblankline,cr_on_blankline,upcase,onlynum);
  _ES = _DS = b;
  return (result);
}


void lock_dos(int x)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  (jump->lock_dos)(x);
  _ES = _DS = b;
}

void unlock_dos(void)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  (jump->unlock_dos)();
  _ES = _DS = b;
}

time_t g_time(time_t far *data)
{
  int b = _DS;
  time_t temp;
  int islocked;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_time)(data);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

int islocked(int sem)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  temp = (jump->islocked)(sem);
  _ES = _DS = b;
  return (temp);
}

FILE far *g_fopen(const char far *filename, const char far *mode,
     const char far *description)
{
  int b = _DS;
  FILE far *temp;
  int islocked;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_fopen)(filename,mode,description);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

int g_fclose(FILE far *file_ptr)
{
  int b = _DS;
  int temp;
  int islocked;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked(DOS_SEM));
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_fclose)(file_ptr);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

int g_flush(FILE *file_ptr)
{
  int b = _DS;
  int temp;
  int islocked;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked(DOS_SEM));
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_flush)(file_ptr);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

int g_free(void *memory_pointer)
{
  int b = _DS;
  int temp;
  int islocked;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_free)(memory_pointer);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

void *g_malloc_main_only(unsigned long int memory, const char far *description)
{
  int b = _DS;
  void far *temp;
  int islocked;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_malloc_main_only)(memory,description);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

void *g_malloc(unsigned long int memory, const char far *description)
{
  int b = _DS;
  void far *temp;
  int islocked;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_malloc)(memory,description);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

int g_fread(void far *buf, size_t size, size_t count, FILE far *stream)
{
  int b = _DS;
  int temp;
  int islocked;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_fread)(buf,size,count,stream);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

int g_fwrite(void far *buf, size_t size, size_t count, FILE far *stream)
{
  int b = _DS;
  int temp;
  int islocked;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_fwrite)(buf,size,count,stream);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

int g_fseek(FILE far *stream, long int offset, int origin)
{
  int b = _DS;
  int temp;
  int islocked;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_fseek)(stream,offset,origin);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

long int g_ftell(FILE far *stream)
{
  int b = _DS;
  long int temp;
  int islocked;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_ftell)(stream);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}


void initabuffer(int bufsize)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  (jump->initabuffer)(bufsize);
  _ES = _DS = b;
}


int aget_abuffer(int far *sentby, int far *channel,
                 char far *string, int far *parm1,
                 int far *parm2, int far *parm3, int far *parm4)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  temp=(jump->aget_abuffer)(sentby,channel,string,parm1,parm2,parm3,parm4);
  _ES = _DS = b;
  return (temp);
}

void aput_into_buffer(int id, char far *string, int channel,
                 int parm1, int parm2, int parm3,int parm4)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->aput_into_buffer)(id,string,channel,parm1,parm2,parm3,parm4);
  _ES = _DS = b;
}

void dealloc_abuf(int portnum)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  (jump->dealloc_abuf)(portnum);
  _ES = _DS = b;
}

void print_file_to_cntrl(const char far *filename,int portnum,int ansi,
                   int pause,int abort,int paging)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->print_file_to_cntrl)(filename,portnum,ansi,pause,abort,paging);
  _ES = _DS = b;
}

int wait_ch(void)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  temp=(jump->wait_ch)();
  _ES = _DS = b;
  return(temp);
}

int line_editor(char far *filename, int length)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  temp=(jump->line_editor)(filename,length);
  _ES = _DS = b;
  return(temp);
}


void g_delay(int ticks)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->delay)(ticks);
  _ES = _DS = b;
}

int g_rename(char far *file1, char *far file2)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  int temp;
  int islocked;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_rename)(file1,file2);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

int g_findfirst(char far *wildcard, struct ffblk far *ffblk, int flags)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;
  int islocked;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->findfirst)(wildcard,ffblk,flags);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return(temp);
}


int g_findnext(struct ffblk far *ffblk)
{
  int b = _DS;
  int temp;
  int islocked;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->findnext)(ffblk);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return(temp);
}

void log_error(char far *file)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->log_error)(file);
  _ES = _DS = b;
}

int g_remove(char far *file)
{
  int b = _DS;
  int islocked;
  int temp;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_remove)(file);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

void send_files(char far **filepointers, int number, int mode)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->send_files)(filepointers,number,mode);
  _ES = _DS = b;
}

void recv_files(char far **filepointers, int far *number, int mode)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->recv_files)(filepointers,number,mode);
  _ES = _DS = b;
}

int g_mkdir(char far *filename)
{
  int b = _DS;
  int islocked;
  int temp;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_mkdir)(filename);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

int g_rmdir(char far *filename)
{
  int b = _DS;
  int islocked;
  int temp;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->g_rmdir)(filename);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

void print_chr(char temp)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->print_chr)(temp);
  _ES = _DS = b;
}

void print_string(char *string)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->print_string)(string);
  _ES = _DS = b;
}

void print_cr(void)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->print_cr)();
  _ES = _DS = b;
}

void position(int x,int y)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->position)(x,y);
  _ES = _DS = b;
}

void foreground(int color, int portnum)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->foreground)(color,portnum);
  _ES = _DS = b;
}

void background(int color, int portnum)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->background)(color,portnum);
  _ES = _DS = b;
}

void special_code(int state, int portnum)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->special_code)(state,portnum);
  _ES = _DS = b;
}

void reset_attributes(int portnum)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->reset_attributes)(portnum);
  _ES = _DS = b;
}
void blink_video(int portnum)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->blink_video)(portnum);
  _ES = _DS = b;
}

void bold_video(int portnum)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->bold_video)(portnum);
  _ES = _DS = b;
}

void wrap_line(char far *string)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->wrap_line)(string);
  _ES = _DS = b;
}

int chars_in_buffer(int portnum)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  temp=(*(jump->a_chars_in_buffer)[portnum])(portnum);
  _ES = _DS = b;
  return (temp);
}

int dcd_detect(int portnum)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  temp=(*(jump->a_dcd_detect)[portnum])(portnum);
  _ES = _DS = b;
  return (temp);
}

void put_char_in_buffer(char temp, int portnum)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (*(jump->a_put_char_in_buffer)[portnum])(temp,portnum);
  _ES = _DS = b;
}

void get_char(int portnum, int far *charput, int far *isthere)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (*(jump->a_get_char)[portnum])(portnum,charput,isthere);
  _ES = _DS = b;
}

void send_char(int portnum, char charput)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (*(jump->a_send_char)[portnum])(portnum,charput);
  _ES = _DS = b;
}

void empty_buffer(int portnum)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (*(jump->a_empty_inbuffer)[portnum])(portnum);
  _ES = _DS = b;
}

int char_in_buf(int portnum)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  temp=(*(jump->a_char_in_buf)[portnum])(portnum);
  _ES = _DS = b;
  return (temp);
}

int get_first_char(int portnum)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  temp=(*(jump->a_get_first_char)[portnum])(portnum);
  _ES = _DS = b;
  return (temp);
}


int get_nchar(int portnum)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  temp=(*(jump->a_get_nchar)[portnum])(portnum);
  _ES = _DS = b;
  return (temp);
}

void wait_for_xmit(int portnum, int ticks)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (*(jump->a_wait_for_xmit)[portnum])(portnum,ticks);
  _ES = _DS = b;
}

void empty_outbuffer(int portnum)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (*(jump->a_empty_outbuffer)[portnum])(portnum);
  _ES = _DS = b;
}

void change_dtr_state(int portnum, int state)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (*(jump->a_change_dtr_state)[portnum])(portnum,state);
  _ES = _DS = b;
}

void check_for_privates(void)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  (jump->check_for_privates)();
  _ES = _DS = b;
}

char far *g_getcwd(char far *buffer, int buflen)
{
  int b = _DS;
  char far *temp;
  struct jumptable far *jump = jmptl;
  int islocked;

  if (!buffer) return (NULL);
  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->getcwd)(buffer,buflen);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

int unregister_bot_myself(void)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  temp=(jump->unregister_bot_myself)();
  _ES = _DS = b;
  return (temp);
}

int register_bot(char far *orig_name)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  temp=(jump->register_bot)(orig_name);
  _ES = _DS = b;
  return (temp);
}

int change_my_info_line(char far *newline)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;
  _ES = old_ES;
  _DS = old_DS;
  temp=(jump->change_my_info_line)(newline);
  _ES = _DS = b;
  return (temp);
}

void broadcast_message(char far *newline)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  (jump->broadcast_message)(newline);
  _ES = _DS = b;
}

void aput_append_into_buffer(int id, int channel, int parm1,
                 int parm2, int parm3, int parm4, int no_str, ...)
{
  va_list ap;
  int b = _DS;
  struct jumptable far *jump = jmptl;

  va_start(ap,no_str);
  _ES = old_ES;
  _DS = old_DS;
  (jump->aput_vargs_into_buffer)
       (id,channel,parm1,parm2,parm3,parm4,no_str,ap);
  _ES = _DS = b;
  va_end(format);
}

int g_getdisk(void)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;
  int islocked;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->getdisk)();
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

int g_getfree(int drive, unsigned long int far *freebytes,
            unsigned long int far *totalbytes)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  struct dfree dbuf;
  int islocked;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  (jump->getdfree)(drive,&dbuf);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  if (dbuf.df_sclus == 0xFFFF) return (-1);
  *freebytes = ((unsigned long int)dbuf.df_avail) *
               ((unsigned long int)dbuf.df_bsec) *
               ((unsigned long int)dbuf.df_sclus);
  *totalbytes = ((unsigned long int)dbuf.df_total) *
               ((unsigned long int)dbuf.df_bsec) *
               ((unsigned long int)dbuf.df_sclus);
  return (0);
}

int g_getcurdir(int drive, char far *directory)
{
  int b = _DS;
  int temp;
  struct jumptable far *jump = jmptl;
  int islocked;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  temp=(jump->getcurdir)(drive,directory);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  return (temp);
}

void get_registered_bot_name_for_myself(char far *string,int string_len)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;

  _ES = old_ES;
  _DS = old_DS;
  (jump->get_registered_bot_name_for_myself)(string,string_len);
  _ES = _DS = b;
}

int read_my_channel(void)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  int result;

  _ES = old_ES;
  _DS = old_DS;
  result = (jump->read_my_channel)();
  _ES = _DS = b;
  return (result);
}

int change_my_channel(int channel)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  int result;

  _ES = old_ES;
  _DS = old_DS;
  result = (jump->change_my_channel)(channel);
  _ES = _DS = b;
  return (result);
}
