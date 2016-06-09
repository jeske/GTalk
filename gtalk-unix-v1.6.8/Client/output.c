
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - output.c
 *
 * General Output routines
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>

#include <pwd.h>

#include "types.h"
#include "ansi.h"
#include "common.h"
#include "output.h"
#include "gtmain.h"

int more_file(char *progname)
{
  FILE *fp;
  int ansi_state;
  int next_char;

  if (fp=fopen(progname,"r"))
    {
      ansi_state = ansi_on(1);
      while (!feof(fp))
	{
	  next_char = fgetc(fp);
	  if (next_char != EOF)
	    print_chr((char)next_char);
	}
      fclose(fp);
      ansi_on(ansi_state);
      return (0);
    }
  return (-1);
}

int more_file2(char *progname, node_struct *a_node)
{
 struct passwd *unix_passinfo;
 uid_t uid;
 gid_t gid;
 pid_t child_pid;
 int result_val;
 int file_uid = getuid(); 
 int file_gid = getgid(); 
 char mailpath[80];

 return;

 unix_passinfo = getpwnam("gtgst");
 uid = unix_passinfo->pw_uid;
 gid = unix_passinfo->pw_gid;

 sprintf(mailpath,"/var/spool/mail/%s",
	a_node->userdata.online_info.unix_passinfo.pw_name);
     
 if (uid == 0) 
   {
     printf("Cannot Exec with UID = 0\r\n");
     return -1;
   }
 if ((child_pid = fork())==0)
   {
     /* we're the child */ 

     setenv("HOME",a_node->userdata.online_info.unix_passinfo.pw_dir,1);
     setenv("USER",a_node->userdata.online_info.unix_passinfo.pw_name,1);
     setenv("LOGNAME",a_node->userdata.online_info.unix_passinfo.pw_name,1);
     setenv("SHELL",a_node->userdata.online_info.unix_passinfo.pw_shell,1);
     setenv("MAIL",mailpath,1);
     setenv("TERM","vt100",1);
     set_special_canonical();

     if (fchown(0,uid,gid))
	{ perror("STDIN");}

     setuid(uid);
     setgid(gid);
     umask(077);

     /* execl(progname,progname,(char *)0);  */
     execl("/bin/more","more",progname,(char *)0);  
     
     exit(result_val);
   }
 else
   {
     fflush(stdout);
     waitpid(child_pid,NULL,NULL);
     fchown(0,file_uid,file_gid);
     tty_raw(STDIN_FILENO);
     fflush(stdout);
   }

}
void print_file(char *string)
{
  more_file(string);
}



int print_file_cntrl(const char *filename,unsigned long int options)
{
  FILE *fp;
  int ansi = (options & PFC_ANSI);
  int ansi_state;
  int next_char;

  if (fp=fopen(filename,"r"))
    {
      ansi_state = ansi_on(ansi);
      while (!feof(fp))
	{
	  next_char = fgetc(fp);
	  if (next_char != EOF)
	    print_chr((char)next_char);
	}
      fclose(fp);
      ansi_on(ansi_state);
      return (0);
    }
  return (-1);
}


void set_scrolling_region(int x1,int x2)
{
  printf("%c[%d;%dr",27,x1,x2);
}

void erase_region(int x1,int x2)
{ 

 if (x1==x2)
   return;

 if (x1>x2)
  { int temp = x1;
    x1 = x2;
    x2 = temp;
  }



 set_scrolling_region(x1,x2);
 position(x2,0);
 for (;x1<x2;x1++)
 {
   print_cr();
 }
 print_cr();
 set_scrolling_region(0,24);
 position(x1,0);

}


void print_sys_mesg(char *string)
{
 printf_ansi("--> %s\n",string);
}


void print_string_len(char *string,unsigned char filler,int start,int len)
{ 
   int empty=0;

    while ((start--) && (*string))
    {
     string++;
    }

    while (len--)
    {

      if (*string)
       {
         print_chr(*string++);
       }
      else
       {
          print_chr(filler);
       }

    }

}



int print_file_tail_grep(const char *filename,int num_lines, 
			 char *search_string, unsigned long int options)
{
  FILE *fp;
  int ansi = (options & PFC_ANSI);
  int ansi_state;
  char buf[1024];
  int file_len, cur_pos, section_len;
  int buf_len, cur_buf_pos, cur_buf_pos_end;
  
  if (fp=fopen(filename,"r"))
    {
      ansi_state = ansi_on(ansi);
      fseek(fp,0,SEEK_END);
      file_len = cur_pos = ftell(fp);
      
      do {
	/* calculate the size and offset of the next buffer */

	section_len = 1024;
	if ((cur_pos-section_len)<0)
	  section_len = cur_pos;
	cur_pos-=section_len;
	
	/* now go there and read it */

	fseek(fp,cur_pos,SEEK_SET);
	buf_len = fread(buf,1,section_len,fp);
	cur_buf_pos = buf_len - 1;
	cur_buf_pos_end = 0;
	while (cur_buf_pos>0) {
	  if ((buf[cur_buf_pos]==10))
	    {
	      if (cur_buf_pos_end) 
		{
		  int temp_cur_buf_pos = cur_buf_pos+1;
		  if (!(num_lines--))
		    break;
		  while (temp_cur_buf_pos<=cur_buf_pos_end) {
		    print_chr(buf[temp_cur_buf_pos++]);
		  }
		  cur_buf_pos_end = cur_buf_pos;
		  
		}
	      else
		cur_buf_pos_end = cur_buf_pos;
	    } 
	  if (!num_lines)
	    break;
	  cur_buf_pos--;
	}
	if (!num_lines)
	  break;
	if (cur_buf_pos_end==0)
	  {
	    /* there was not even ONE line in all 1024 bytes, so just 
               quit */
	    return (-1);
	  }
	
	/* 
         * now we want to go back, but remember we need to keep that
         * last line we were working on "in" the window
         */
	if (cur_pos)
	  cur_pos -= cur_buf_pos_end;    
      } while ((num_lines) && (cur_pos>0));
      
      /* there will not be a LF to set of the start of the first line */
      if ((cur_pos==0) && (num_lines)) { 
	if (cur_buf_pos!=cur_buf_pos_end) {
	  int temp_cur_buf_pos = cur_buf_pos;

	  while (temp_cur_buf_pos<=cur_buf_pos_end) {
	    print_chr(buf[temp_cur_buf_pos++]);
	  }
	  cur_buf_pos_end = cur_buf_pos;
	  
	}
      }

      fclose(fp);
      ansi_on(ansi_state);
      return (0);
    }
  else
    {
      log_error("tail_grep FNF: %s",filename);
    }
  return (-1);
}
