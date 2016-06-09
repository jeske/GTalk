
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - command.c
 *
 * This contains the code for most of the internal commands for general
 * use. "comprase.c" contains the smarts of the command list.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <grp.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <termios.h>
#include <arpa/telnet.h>

#include "types.h"
#include "str.h"
#include "list.h"
#include "abuf.h"
#include "comparse.h"
#include "command.h"
#include "gtmain.h"
#include "states.h"
#include "common.h"
#include "input.h"
#include "output.h"
#include "usercommon.h"

#include "pwd.h"
#include "shadow.h"
#include "shared.h"
#include "channel.h"

void conditional_print(unsigned long int scalar,const char *title)
{ char buf[30];
  char temp[20];
  strcpy(temp,title);

    if (!scalar)
      return;

    if (scalar==1)
      temp[strlen(title)-1]=0;

    sprintf(buf,"%lu %s ",scalar,temp);
    print_string(buf);
}

void conditional_sprint(unsigned long int scalar,const char *title,char *string){
  char buf[30];
  char temp[20];
  strcpy(temp,title);

  if (!scalar) return;

  if (scalar==1)
    temp[strlen(title)-1]=0;
  sprintf(buf,"%lu %s ",scalar,temp);
  strcat(string,buf);
}

void print_expanded_time(unsigned long int seconds)
{
    unsigned long int minutes,hours,days,months,years;

 if (!seconds)
  {
    printf("- None -"); 
    return;
  }

 minutes=seconds/60;
 seconds=seconds % 60;
 hours=minutes / 60;
 minutes=minutes % 60;
 days=hours/24;
 hours=hours % 24;
 months=days/30;
 days=days % 30;
 years=months/12;
 months=months % 12;

 conditional_print(years,"Years");
 conditional_print(months,"Months");
 conditional_print(days,"Days");
 conditional_print(hours,"Hours");
 conditional_print(minutes,"Minutes");
 conditional_print(seconds,"Seconds");
}

void print_expanded_time_cr(unsigned long int seconds)
{ print_expanded_time(seconds);
  printf("\r\n");
}

void sprint_expanded_time(unsigned long int seconds,char *string)
{unsigned long int minutes,hours,days,months,years;
 *string=0;
 minutes=seconds/60;
 seconds=seconds % 60;
 hours=minutes / 60;
 minutes=minutes % 60;
 days=hours/24;
 hours=hours % 24;
 months=days/30;
 days=days % 30;
 years=months/12;
 months=months % 12;

 conditional_sprint(years,"Years",string);
 conditional_sprint(months,"Months",string);
 conditional_sprint(days,"Days",string);
 conditional_sprint(hours,"Hours",string);
 conditional_sprint(minutes,"Minutes",string);
 conditional_sprint(seconds,"Seconds",string);
}

int edit_file(com_struct *com, char *string)
{
  char filename[100];

  get_string(filename, &string, sizeof(filename)-1, 1, 0, 1);
  if (*filename)
    line_editor(filename, 65536);
  return (1);
}

int cmd_change_passwd(com_struct *com, char *string)
{  
  char salt[3];
  int valid_passwd = 0;
  char s[11];
  char s2[11];
  struct passwd *uxacct_info;
#ifdef GT_SHADOW
  struct spwd *pass_info;
#endif
  FILE *pw_cng;
  int old_pw_pos;
  char *str_tmp;

  
  /* 
   * get their UNIX account info 
   */

  uxacct_info = getpwnam(mynode->userdata.user_info.login);
#ifdef GT_SHADOW
  pass_info = getspnam(mynode->userdata.user_info.login);
#endif
  
  /* 
   * first get their old password and find out if it's right
   */


  if (!uxacct_info)
    { printf("No such User [%s]\r\n",mynode->userdata.user_info.login);
      printf("ERROR changing password\r\n");
      log_error("UNIX acct [%s] does not exist (but logged in)",
		mynode->userdata.user_info.login);
    }
  else
    {
#ifdef GT_SHADOW
      strncpy(salt,pass_info->sp_pwdp,2);
#else
      strncpy(salt,uxacct_info->pw_passwd,2);
#endif
      salt[2]=0;
      printf("Enter OLD Password: ");
      get_input_cntrl(s,10,GI_FLAG_MASK_ECHO);
      
#ifdef GT_SHADOW	      
      if (!strcmp(pass_info->sp_pwdp,(char *)crypt(s,salt)))
	valid_passwd=1;
#else
      if (!strcmp(uxacct_info->pw_passwd,(char *)crypt(s,salt)))
	valid_passwd=1;
#endif
      strcpy(s,"abcde"); /* blank out the Password in memory */
      
    }

  /* 
   * now that we got their old password (correct or not)
   * ask them for the new one 
   */

  printf("Enter NEW Password: ");
  get_input_cntrl(s,10,GI_FLAG_MASK_ECHO);
  
  printf("Enter NEW Password Again: ");
  get_input_cntrl(s2,10,GI_FLAG_MASK_ECHO);
  
  if ((!strcmp(s,s2)) && (valid_passwd))  
    {
      /* 
       * if the passwords matched, and they entered it 
       * correctly before, then copy in the new one.
       *
       * We Still need to WRITE OUT the information here.
       */

#ifdef GT_SHADOW	      
      if ((pw_cng = fopen("/etc/shadow","rb+"))==NULL)
	{
	  printf("--> ERROR changing password\r\n");
	  log_error("Error opening /etc/shadow for /passwd");
	  return 0;
	}

      /* find the start of the entry we want to change */
      fseek(pw_cng,0,SEEK_SET);

      do {
	old_pw_pos = ftell(pw_cng);
      } while (strcmp(fgetspent(pw_cng)->sp_namp,
		       mynode->userdata.user_info.login));
      
      fseek(pw_cng,old_pw_pos,SEEK_SET);
      while (fgetc(pw_cng)!=':');
      str_tmp = (char *)crypt(s,salt);

      /* now find the width of the field */
      old_pw_pos = ftell(pw_cng);
      while (fgetc(pw_cng)!=':');

      if ((ftell(pw_cng)-old_pw_pos)<strlen(str_tmp))
	{
	  printf("ERROR Password UnChanged\r\n");
	  log_error("Password entry too short for user [%s]",
		    mynode->userdata.user_info.login);
	  return 0;
	}

      fseek(pw_cng,old_pw_pos,SEEK_SET);
      while (*str_tmp) {
	fputc(*str_tmp++,pw_cng);
      }
      fclose(pw_cng);
#else
      if ((pw_cng = fopen("/etc/passwd","rb+"))==NULL)
	{
	  printf("--> ERROR changing password\r\n");
	  log_error("Error opening /etc/passwd for /passwd");
	  return 0;
	}

      /* find the start of the entry we want to change */
      fseek(pw_cng,0,SEEK_SET);

      do {
	old_pw_pos = ftell(pw_cng);
      } while (strcmp(fgetpwent(pw_cng)->pw_name,
		       mynode->userdata.user_info.login));
      
      fseek(pw_cng,old_pw_pos,SEEK_SET);
      while (fgetc(pw_cng)!=':');
      str_tmp = (char *)crypt(s,salt);

      /* now find the width of the field */
      old_pw_pos = ftell(pw_cng);
      while (fgetc(pw_cng)!=':');

      if ((ftell(pw_cng)-old_pw_pos)<strlen(str_tmp))
	{
	  printf("ERROR Password UnChanged\r\n");
	  log_error("Password entry too short for user [%s]",
		    mynode->userdata.user_info.login);
	  return 0;
	}

      fseek(pw_cng,old_pw_pos,SEEK_SET);

      while (*str_tmp) {
	fputc(*str_tmp++,pw_cng);
      }
      fclose(pw_cng);
#endif
      

      strcpy(s,"abcde"); /* blank out the Password "s" in memory */
      strcpy(s2,"abcde"); /* blank out the Password "s2" in memory */
      printf("--> New Password Accepted\r\n");
    }
  else
    {
      printf("--> Password Change FAILED\r\n");
    }

  return 0;
}



int cmd_do_login_subshell(com_struct *com, char *string)
{
  struct termios buf;
  
  if (tcgetattr(0, &buf) < 0)
    {
      printf("Unable to get tty settings.\r\n");
    }
  else
    {
      char *envp[] = { "PATH=/tmp",NULL};

      set_special_canonical();
      
      execl("/bin/login","login","-p", (char *)0);

      if (tcsetattr(0, TCSAFLUSH, &buf) < 0)
	{ 
	  printf("Unable to restore tty settings.\r\n");
	}
    }
/*printf("--> Returned to %s\r\n","GTalk"); */
}



int exec_user_program(char *progname, node_struct *a_node,
		      unsigned long int options)
{
 uid_t uid = a_node->userdata.online_info.unix_passinfo.pw_uid;
 gid_t gid = a_node->userdata.online_info.unix_passinfo.pw_gid;
 pid_t child_pid;
 int result_val;
 int file_uid = getuid(); 
 int file_gid = getgid(); 
 char mailpath[80];

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
     printf("USER: [%s]\r\n",a_node->userdata.online_info.unix_passinfo.pw_name);

     setenv("HOME",a_node->userdata.online_info.unix_passinfo.pw_dir,1);
     setenv("USER",a_node->userdata.online_info.unix_passinfo.pw_name,1);
     setenv("LOGNAME",a_node->userdata.online_info.unix_passinfo.pw_name,1);
     setenv("SHELL",a_node->userdata.online_info.unix_passinfo.pw_shell,1);
     setenv("MAIL",mailpath,1);
     if (!mynode->userdata.user_info.termtype[0])
         setenv("TERM","nansisys",1);
     else
         setenv("TERM",mynode->userdata.user_info.termtype,1);
     
     set_special_canonical();

     if (fchown(0,uid,gid))
	{ perror("STDIN");}

     if (!(options & COM_NOCHROOT)) {
       if (chroot(GTALK_ROOT_DIR)) {
	 perror(GTALK_ROOT_DIR);
	 log_error("Error changing root");
	 return;
       }
     chdir(a_node->userdata.online_info.unix_passinfo.pw_dir);
     }

     setuid(uid);
     setgid(gid);
     umask(077);
     /* create a process group? */

     if (result_val = execl(progname,progname,(char *)0))
	{
	log_error("Error with exec of [%s]",progname);
	}
    /*  execl("/usr/bin/strace","strace",progname,(char *)0);  */
     exit(result_val);
   }
 else
   {
#if 0
     printf("--> Launching\r\n",progname);
     fflush(stdout);
#endif
     waitpid(child_pid,NULL,NULL);
     fchown(0,file_uid,file_gid);
     tty_raw(STDIN_FILENO);
#if 0
     printf("--> Returning to Gtalk\r\n",progname);
     fflush(stdout);
#endif
   }

}

int cmd_do_user_shell(com_struct *com,char *string)
{
   char s[11];

   printf("--> Entering shell\r\n");
   exec_user_program("/bin/tcsh", mynode, COM_NONE);
}

int cmd_do_user_menu(com_struct *com, char *string)
{
   char s[11];

    printf("--> Entering User Menu\r\n");
    exec_user_program("/usr/bin/menu", mynode, COM_NONE);
}


int cmd_quit_loop(com_struct *com, char *string)
{
/*
	if (*string=='=') {
		node_id node;
		char class_name[CLASS_NAME_LEN+1];
		struct class_data temp_class;
		string++;
		if (get_system_no_and_node(&string, &system, &node) < 0)
			ml_logout = 1;
		get_string(class_name,&string, CLASS_NAME_LEN, 1, 0 , 1);
		fix_classname(class_name);
		if (!read_class_by_name(class_name,&temp_class)) {
			char s[10];
			printf_ansi("Password: ");
			get_input_cntrl(s,10,GI_FLAG_MASK_ECHO);
			if (strcmp(s,USEREDIT_PASSWD)) {
			   	ml_logout = 1;
			} else {
				set_online_info(c_nodes(node),NULL,&temp_class);
				printf("### [%02d] [%s]\r\n",node,class_name);
			}
		} else
			ml_logout = 1;
	} else
*/
		ml_logout = 1;
}

int cmd_change_handle(com_struct *com, char *string)
{
  int real_len;

  while (*string == ' ')
    string++;

  if (!testFlag(mynode,"CMD_H_ANSI"))
    remove_ansi(string);
  if (!testFlag(mynode,"CMD_MAKE"))
    remove_flashing(string);

  real_len = strlen(string);

  while ((ansi_strlen(string))>40) {
    real_len--;
    string[real_len]=0;
  }

 if (mynode->userdata.user_info.enable==0) {
	strncpy(mynode->userdata.user_info.handle,string,GUEST_HANDLE_LEN);
	mynode->userdata.user_info.handle[GUEST_HANDLE_LEN]=0;
 } else {
  strncpy(mynode->userdata.user_info.handle,string,HANDLE_LEN);
  mynode->userdata.user_info.handle[HANDLE_LEN-1]=0;
 }

  printf("--> Handle Changed.\r\n");
}


int change_to_channel(char *name)
{
  if (strcmp(mynode->cur_chan,name))
     {
     if (*mynode->cur_chan)
       client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
		          "LEAVE %s",  mynode->cur_chan); 
     strcpy(mynode->new_chan, name);
     if (*mynode->new_chan)
       client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
		          "JOIN %s", mynode->new_chan);
     }
  else
     printf("--> Already in that channel\r\n");
}

int cmd_change_channel(com_struct *com, char *string)
{
  char channel[CHANNEL_NAME_LEN+1];

  if ((*string>='0') && (*string<='9')) {
    printf("--> Invalid Command Format, type '/t <channelname>'\r\n");
    return -1;
  }

  get_string(channel, &string, sizeof(channel)-1, 1, 1, 1);
  if (!(*channel))
    {
      printf_ansi("--> Specify a channel to change to\n\r");
      return (-1);
    }
  if (!testFlag(mynode,"CMD_LURK"))
	remove_ansi(channel);
  return (change_to_channel(channel));
}


int modems_free()
{
  int node_count;
  int free_count=0;

  for(node_count=0;node_count<MAX_NODES;node_count++)
    switch(c_nodes(node_count)->status) {
    case NODE_EMPTY:
      break;
    case NODE_IDLE:
    case NODE_CONN_WAITING:
      if (c_devices((c_nodes(node_count)->dev_no))->node_type != DIRECT_NODE_TYPE)
         free_count++;
      break;
    case NODE_CONNECTING:
    case NODE_CONNECTED:
    case NODE_LOGGING_IN:
      break;
    case NODE_ACTIVE:
      break;
    default:
      break;
    }

  return (free_count);

}

int cmd_system_list(com_struct *com, char *string)
{
  int node_count;
  int len;
  int ansi_state = ansi_on(1);
  char buf[80];
  time_t time_now = time(NULL);
  int print_dev_no=0;
  int free_count;

  /* first parse command line options */

  string = skip_blanks(string);
  while ((*string) && (*string != ' '))
    {
      switch (*string)
	{
	case 'd':    /* print device numbers */
	  print_dev_no =1;
	  break;
	default:
	  break;
	}
      string++;
    }

  /* now print the system list */


  for(node_count=0;node_count<MAX_NODES;node_count++)
	switch(c_nodes(node_count)->status) {
	case NODE_EMPTY:
	case NODE_IDLE:
	case NODE_CONN_WAITING:
			break;
        case NODE_CONNECTING:
                        printf_ansi("#%02d|*f8-|*ffConnecting...|*r1\r\n",
                           node_count);
                        break;
        case NODE_CONNECTED:
        case NODE_LOGGING_IN:
                        printf_ansi("#%02d|*f8-|*ffLogging in...|*r1\r\n",
                           node_count);
                        break;
	case NODE_ACTIVE:
     if (!c_nodes(node_count)->userdata.online_info.logged_in_flag)
       break;
	  
     len = printf_ansi("#%02d:%c%s|*r1%c",node_count,
	    c_nodes(node_count)->userdata.online_info.class_info.staple[0],
	    c_nodes(node_count)->userdata.user_info.handle,
	    c_nodes(node_count)->userdata.online_info.class_info.staple[1]);
			
     repeat_chr(' ',35-len,0);
     if (c_nodes(node_count)->userdata.user_info.enable==0)
       printf("%%GST");
     else
       printf("#%03lu",
	      c_nodes(node_count)->userdata.user_info.user_no);
     printf("/%03d",((time_now - 
            (c_nodes(node_count)->userdata.online_info.login_time))/60));
     if (c_nodes(node_count)->timeout_status!=TIMEOUT_NONE)
     printf("/%03d",(((c_nodes(node_count)->timeout_time - 
           c_nodes(node_count)->userdata.online_info.login_time))/60));
     else
       printf("/UNL");
			
     if (print_dev_no)
       printf("/D%02d",c_nodes(node_count)->dev_no);
			
     if (c_nodes(node_count)->cur_chan[0])
       printf_ansi(" (%s|*r1) ", c_nodes(node_count)->cur_chan);
     else 
       printf(" - ");

     if (c_nodes(node_count)->link == TRUE)
       printf_ansi("|*f4LINKED|*r1");

     if (c_nodes(node_count)->userdata.online_info.location[0])
       printf_ansi("%s",
		  c_nodes(node_count)->userdata.online_info.location);
     printf("\r\n");
     break;
     } /* end select */

  time_now = time(NULL);
  sprint_time(buf,&time_now);
  printf_ansi("--> %s ",buf);

  if ((free_count = modems_free())!=0) {
    if (free_count==1) {
      printf_ansi("  |*f2[1 Modem Free]|*r1\n");
    } else {
      printf_ansi("  |*f2[%d Modems Free]|*r1\n",free_count);
    }
  } else {
    printf_ansi("  |*f1|*h1[ No Modems Free ]|*r1\n"); 
  }
  if (c_sys_info->lock_priority) {
    printf_ansi("--> Modems Locked to Priority [%d]\n",
		c_sys_info->lock_priority);
  }
  if (c_sys_info->lock_priority_telnet) {
    printf_ansi("--> Telnet Locked to Priority [%d]\n",
		c_sys_info->lock_priority_telnet);
  }

  ansi_on(ansi_state);
  return (0);
}


int cmd_long_system_list(com_struct *com, char *string)
{
  int node_count;
  int len;
  int ansi_state = ansi_on(1);
  char buf[80];
  time_t time_now = time(NULL);
  int print_dev_no=0;
  int free_count;

  /* first parse command line options */

  string = skip_blanks(string);
  while ((*string) && (*string != ' '))
    {
      switch (*string)
	{
	case 'd':    /* print device numbers */
	  print_dev_no =1;
	  break;
	default:
	  break;
	}
      string++;
    }

  /* now print the system list */


  for(node_count=0;node_count<MAX_NODES;node_count++)
	switch(c_nodes(node_count)->status) {
	case NODE_EMPTY:
	case NODE_IDLE:
	case NODE_CONN_WAITING:
			break;
	case NODE_CONNECTING:
			printf_ansi("#%02d:|*f8-|*ffConnecting...|*r1\r\n",
			   node_count);
			break;
	case NODE_CONNECTED:
	case NODE_LOGGING_IN:
			printf_ansi("#%02d|*f8-|*ffLogging in...|*r1\r\n",
			   node_count);
			break;
	case NODE_ACTIVE:
     if (!c_nodes(node_count)->userdata.online_info.logged_in_flag)
       break;			
  
     len = printf_ansi("#%02d:%c%s|*r1%c",node_count,
	    c_nodes(node_count)->userdata.online_info.class_info.staple[0],
	    c_nodes(node_count)->userdata.user_info.handle,
	    c_nodes(node_count)->userdata.online_info.class_info.staple[1]);
			
     repeat_chr(' ',35-len,0);

     if (c_nodes(node_count)->userdata.user_info.enable==0)
       printf("%%GST");
     else
       printf("#%03lu",
	      c_nodes(node_count)->userdata.user_info.user_no);
     printf("/%03d",((time_now - 
            (c_nodes(node_count)->userdata.online_info.login_time))/60));
     if (c_nodes(node_count)->timeout_status!=TIMEOUT_NONE)
     printf("/%03d",(((c_nodes(node_count)->timeout_time - 
           c_nodes(node_count)->userdata.online_info.login_time))/60));
     else
       printf("/UNL");
			
     if (print_dev_no)
       printf("/D%02d",c_nodes(node_count)->dev_no);

     switch(c_devices((c_nodes(node_count)->dev_no))->node_type)
     {
     case SERIAL_NODE_TYPE:
       printf(" Serial");
	break;
     case TELNET_NODE_TYPE:
       printf(" Telnet");
       break;
     case DIRECT_NODE_TYPE:
       printf(" Direct");
       break;
     case MODEM_NODE_TYPE:
       printf(" Modem ");
       break;
     }
			
     if (c_nodes(node_count)->cur_chan[0])
       printf_ansi(" (%s|*r1) ", c_nodes(node_count)->cur_chan);
     else 
       printf(" - ");

     if (c_nodes(node_count)->link == TRUE)
       printf_ansi("|*f4LINKED|*r1");

     if (c_nodes(node_count)->userdata.online_info.location[0])
       printf_ansi("%s",
		  c_nodes(node_count)->userdata.online_info.location);
     printf("\r\n");
     break;
     } /* end select */

  time_now = time(NULL);
  sprint_time(buf,&time_now);
  printf_ansi("--> %s ",buf);

  if ((free_count = modems_free())!=0) {
    if (free_count==1) {
      printf_ansi("  |*f2[1 Modem Free]|*r1\n");
    } else {
      printf_ansi("  |*f2[%d Modems Free]|*r1\n",free_count);
    }
  } else {
    printf_ansi("  |*f1|*h1[ No Modems Free ]|*r1\n"); 
  }
  if (c_sys_info->lock_priority) {
    printf_ansi("--> Modems Locked to Priority [%d]\n",
		c_sys_info->lock_priority);
  } 
  if (c_sys_info->lock_priority_telnet) {
    printf_ansi("--> Telnet Locked to Priority [%d]\n",
                c_sys_info->lock_priority_telnet);
  }

  ansi_on(ansi_state);
  return (0);
}

int cmd_system_listip(com_struct *com, char *string)
{
  int node_count;
  int len;
  int ansi_state = ansi_on(1);
  char buf[80];
  time_t time_now = time(NULL);
  int print_dev_no=0;
  int free_count;

  for(node_count=0;node_count<MAX_NODES;node_count++)
	switch(c_nodes(node_count)->status) {
	case NODE_EMPTY:
	case NODE_IDLE:
	case NODE_CONN_WAITING:
			break;
        case NODE_CONNECTING:
                        printf_ansi("#%02d|*f8-|*ffConnecting...|*r1\r\n",
                           node_count);
                        break;
        case NODE_CONNECTED:
        case NODE_LOGGING_IN:
                        printf_ansi("#%02d|*f8-|*ffLogging in...|*r1\r\n",
                           node_count);
                        break;
	case NODE_ACTIVE:
     if (!c_nodes(node_count)->userdata.online_info.logged_in_flag)
       break;
	  
     len = printf_ansi("#%02d:%c%s|*r1%c",node_count,
	    c_nodes(node_count)->userdata.online_info.class_info.staple[0],
	    c_nodes(node_count)->userdata.user_info.handle,
	    c_nodes(node_count)->userdata.online_info.class_info.staple[1]);
			
     repeat_chr(' ',35-len,0);
     if (c_nodes(node_count)->userdata.user_info.enable==0)
       printf("%%GST");
     else
       printf("#%03lu",
	      c_nodes(node_count)->userdata.user_info.user_no);
			
     if (c_nodes(node_count)->cur_chan[0])
       printf_ansi(" (%s|*r1) ", c_nodes(node_count)->cur_chan);
     else 
       printf(" - ");
     if (*c_nodes(node_count)->ip) 
       printf("%s",c_nodes(node_count)->ip);     
     else 
       printf("No IP Addy");
     printf("\r\n");
     break;
     } /* end select */

  ansi_on(ansi_state);
  printf("--> End of IP List\r\n");
  return (0);
}

int cmd_command_help(com_struct *com, char *string)
{ 
  com_struct *walker;
  int index;
  int num_elements = elements(&commands);
  int len;
  int ansi_state = ansi_on(1);
  int screen_height = 24;
  int line_count=3;
  com_struct *help_com;
  char *old_str;

  string = skip_blanks(string);
  old_str = string;

  /* if there is something here, then they want extended help */

  if (*string)
    {
      char filename[100];
      if (!(help_com=find_command(&string)))
	{
	  printf("--> Invalid command [%s]. Type /? for a list.\r\n",old_str);
	  return -1;
	}
      
      printf_ansi("|*f4|*h1[ Extended Command Help ]|*r1\n");
      printf_ansi("|*h1Name: |*r1%s\n",help_com->command);
      printf_ansi("|*h1Description: |*r1%s\n",help_com->description);
      printf_ansi("|*f4|*h1[");
      repeat_chr('-',50,0);
      printf_ansi("]|*r1\n");
      sprintf(filename,"cmdhelp/%s",help_com->command);
      print_file_cntrl(filename,PFC_ANSI|PFC_PAGING);
      printf_ansi("|*f4|*h1[");
      repeat_chr('-',50,0);
      printf_ansi("]|*r1\n");
      return 0;
    }

  /* no, they didn't want extended help, so print the command list */

  printf_ansi("|*f4|*h1Gtalk|*r1 |*h1Command List|*r1\r\n");
  printf_ansi("|*h1[|*f4%d|*f7] Commands|*r1\r\n\r\n",num_elements);

  for(index=0;index<num_elements;index++)
    {
      walker = element_of_index(com_struct,&commands,index,0);

      if (testFlag(mynode,walker->flag_name))
	{
	  
	  len = printf("/%s",walker->command);
	  repeat_chr(' ',MAX_COMMAND_LEN-len,0);
	  printf_ansi(":%s\r\n",walker->description);
	  
	  line_count++;
	  if (line_count>=screen_height)
	    {
	      if (wait_for_return())
		{ 
		  printf_ansi("--> [ Aborted  ]\n");
                  ansi_on(ansi_state);
		  return 0;
  		}
	      line_count=0;
	    }
	}
    }

   ansi_on(ansi_state);
}

int cmd_toggle_ansi_color(com_struct *com, char *string)
{
  int new_setting = !get_term_ansi();

  set_term_ansi(new_setting);

  if (new_setting)
    printf("--> ANSI Color Enabled\r\n");
  else
    printf("--> ANSI Color Disabled\r\n");

}

int cmd_toggle_high_ascii(com_struct *com, char *string)
{
  int new_setting = !get_term_ascii();

  set_term_ascii(new_setting);

  if (new_setting)
    printf("--> Extended ASCII Enabled\r\n");
  else
    printf("--> Extended ASCII Disabled\r\n");
}


void print_class_info(struct class_defined_data_struct *usrcls)
{
  int privs_per_line = 40;
  int cur_priv  = 0;
  int priv_loop;


  if (usrcls->verbose_class_name[0])
    printf("\r\n    Class Description: %s",usrcls->verbose_class_name);
  printf("\r\n       Priority Level: %d",usrcls->priority);



}

void print_class_flag_info(struct class_defined_data_struct *usrcls)
{
  struct flag_map_struct *ptr = flags;
  int count = 24;

  printf("\r\n");
  wait_for_return();

  printf("Flags:\r\n");

  while (ptr->flagname) {
    if (testbit(usrcls->privs,ptr->flagnum)) {
      printf("(%d) %s\r\n",ptr->flagnum, ptr->flagname);
      count--;
      if (count == 0)
	{ wait_for_return();
	  count = 24;
	}
    }
    
    ptr++;
  }
}


void print_user_last_info(struct unique_information_struct *usr,char *string)
{
  int ansi_state = ansi_on(1);
  char buf[80];
  char spoolfile[100];
  struct stat spool_stat;

  /* first parse command line options */

  string = skip_blanks(string);
  while ((*string) && (*string != ' '))
    {
      switch (*string)
	{
	default:
	  break;
	}
      string++;
    }
  
  /* now print the user info */

  printf_ansi("Last Call Info for #%03d:(%s|*r1)\r\n\r\n",
	      usr->user_no,usr->handle);

  sprint_time(buf,&usr->conception);
  printf("    Conception Date: %s\r\n",buf);
  fflush(stdout);
  sprint_time(buf,&usr->last_call);
  printf("    Last Call  Date: %s\r\n",buf);

  sprintf(spoolfile,"%s/var/spool/mail/%s", GTALK_ROOT_DIR,
	  usr->login);
  if (stat(spoolfile,&spool_stat)) {
    log_error("Cannot Access Spoolfile [%s]",spoolfile);
    printf("[ No Email Information Available ]\r\n");
  } else {
    sprint_time(buf,&(spool_stat.st_mtime));
    printf("Last Email Received: %s ",buf);
    if (spool_stat.st_mtime > spool_stat.st_atime)
      printf_ansi("|*f2|*h1(New Email)|*r1\n");
    else
      printf_ansi("\n");
    sprint_time(buf,&(spool_stat.st_atime));
    printf("    Last Email Read: %s\r\n",buf);
  }

}

int cmd_print_user_last_info(com_struct *com, char *string)
{
  unsigned long int num;
  struct user_data temp_info;
  int old_state = ansi_on(1);

  if (get_number(&(string),&num))
    {
      if (read_user_record(num,&temp_info)) {
	log_error("Error reading #%03d in cmd_print_user_last_info",num);
	printf("--> Info for User #%03d cannot be read\r\n");
	ansi_on(old_state);
	return 0;
      }
      print_user_last_info(&(temp_info.user_info),string);
    }
  else {  
    printf("--> Invalid Format\r\n"); 
    ansi_on(old_state);
    return -1; 
  }
  return 0;
}



void print_account_finance_info(struct unique_information_struct *usr,
				struct class_defined_data_struct *usrcls)
{
  char buf[100];
  
  /* fancy new account stuff */
  
  printf("    Account Balance: %dcr        Overdraft Limit: %dcr\r\n",
	 usr->account_balance,usrcls->account_overdraft_limit);
  if (usrcls->credit_card_limit) {
    printf("Credit Card Balance: %dcr",usr->credit_card_balance);
    printf("   Limit: %dcr   Available: %dcr\r\n",
	   usrcls->credit_card_limit, usrcls->credit_card_limit - 
	   usr->credit_card_balance);
  } else {
    if (usr->credit_card_balance)
      printf("Credit Card Balance: %dcr    (Credit Line Canceled)\r\n",
	     usr->credit_card_balance);
  }
  
  if ((usrcls->monthly_wage) || (usrcls->monthly_free_credits)
      || (usr->free_credits)) {
    time_t temp;
    
    if (usrcls->monthly_wage)
      printf(" Monthly Class Wage: %dcr\r\n",usrcls->monthly_wage);
    if ((usrcls->monthly_free_credits) || (usr->free_credits))
      printf("       Free Credits: %dcr available    %dcr monthly\r\n",
	     usr->free_credits,usrcls->monthly_free_credits);
    sprint_time(buf,&usr->last_wage_payment_date);
    printf("      Payment Dates: (last) %s\r\n",buf);
    temp = usr->last_wage_payment_date + (86400l*30);
    sprint_time(buf,&temp);
    printf("                   : (next) %s\r\n",buf);
  }
}


void print_user_info(struct unique_information_struct *usr,
		     struct class_defined_data_struct *usrcls,char *string,
		     int info_flags)
{
  int ansi_state = ansi_on(1);
  char buf[80];
  int print_flag_info = 0;

  /* first parse command line options */

  string = skip_blanks(string);
  while ((*string) && (*string != ' '))
    {
      switch (*string)
	{
	case 'F':
	case 'f':    /* print flag information */
	  print_flag_info =1;
	  break;
	default:
	  break;
	}
      string++;
    }
  
  /* now print the user info */

  printf_ansi("Info for #%03d:%c%s|*r1%c\r\n\r\n",usr->user_no,
	      usrcls->staple[0],usr->handle,usrcls->staple[1]);
  printf_ansi("   Email/Unix Login: %s\n",usr->login);
  sprint_time(buf,&usr->conception);
  printf("    Conception Date: %s\r\n",buf);
  fflush(stdout);

  if (info_flags & 0x0001) /* only sysops see this for everyone */
    {
      sprint_time(buf,&usr->expiration);
      printf("    Expiration Date: %s\r\n",buf);
      fflush(stdout);
      
      print_account_finance_info(usr,usrcls);
    }
      sprint_time(buf,&usr->last_call);
      printf("    Last Call  Date: %s\r\n",buf);
      if (usrcls->time)
  	printf("    Time (per call): %d\r\n",usrcls->time);
      else
 	printf("    Time (per call): UNLIMITED\r\n");
      printf("    Number of Calls: %d\r\n",usr->stats.calls_total);
      printf("\r\nRecorded:\r\n");
      printf("    Total Online Time: ");
      print_expanded_time(usr->stats.time_total);
      printf("\r\n    Average Time/Call: "); 
      if (usr->stats.calls_total)
	print_expanded_time((usr->stats.time_total)/(usr->stats.calls_total));
      else
	printf("- None -");
      printf("\r\n      Number of Kills: %d",usr->killstats.kills_total);
      printf("\r\n         Times Killed: %d",usr->killedstats.kills_total);

  if (info_flags & 0x0001)
    {
      printf("\r\n           Class Name: %s",usrcls->class_name);
      if (strcmp(usr->class_name,usrcls->class_name)) {
	printf(" (Real Class = %s)",usr->class_name);
      }
      print_class_info(usrcls);
      if (print_flag_info)
	print_class_flag_info(usrcls);
    }
  printf_ansi("|*r1\r\n");
  ansi_on(ansi_state);
}

int cmd_print_user_info(com_struct *com, char *string)
{
  unsigned long int num;
  int info_level=0;
  int old_state;
 

  if (testFlag(mynode,"SYS_VIEW_USER_DATA"))
    info_level = 0xffff;

  if (*string=='#')
   {
     struct user_data temp_info;
     struct class_data class_info;

     string++;
     if (get_number(&(string),&num))
       {
	 if (read_user_record(num,&temp_info)) {
	   log_error("Error reading #%03d in cmd_print_user_last_info",num);
	   printf("--> Info for User #%03d cannot be read\r\n",num);
	   ansi_on(old_state);
	   return 0;
	 }
	 
	 if (read_class_by_name(temp_info.user_info.class_name,&class_info)) {
	   log_error("Error reading class [%s] in cmd_print_user_last_info",
		     temp_info.user_info.class_name);
	   printf("--> Info for User #%03d cannot be read\r\n",num);
	   ansi_on(old_state);
	   return 0;
	 }
	 
	 print_user_info(&(temp_info.user_info),&class_info.class_info,
			 string,info_level);
       }
     else {  
       printf("--> Invalid Format\r\n"); 
       ansi_on(old_state);
       return 1; 
     }
     ansi_on(old_state); 
     return 0;
   }
  if (get_number(&string,&num))
   {
     printf("--> /INFO%d\r\n",num);
     if ((num<0) || (num>c_nodes_used) || ((!is_node_online(c_nodes(num)))))
	 {
	   printf("--> Invalid Node\r\n");
	   return -1;
	 }
     print_user_info(&(c_nodes(num)->userdata.user_info),
		     &(c_nodes(num)->userdata.online_info.class_info),
		       string,info_level);
     return 0;
   }

  info_level = 0xffff;
  
  print_user_info(&(mynode->userdata.user_info),
		  &(mynode->userdata.online_info.class_info),
		    string,info_level);

}



int cmd_time_command(com_struct *com, char *string)
{
   time_t now;
   char s[80];
   unsigned long int today,total_time;
   int ansi_state = ansi_on(1);

   now=time(NULL);
   sprint_time(s,&now);
   print_cr();
   print_string("      |*h1|*f4Current Time: |*r1");
   print_str_cr(s);

   print_string("      |*h1|*f4Logged in at: |*r1");
   sprint_time(s,&mynode->userdata.online_info.login_time);
   print_str_cr(s);

   print_string("            |*h1|*f4Online: |*r1");
   today=now-mynode->userdata.online_info.login_time;
   print_expanded_time_cr(today);

/*
   if (user_lines[portnum].user_info.number>=0)
    {
      print_string("|*h1|*f4      Online Total: |*r1");
      total_time=user_lines[portnum].user_info.stats.time_total+today;
      print_expanded_time_cr(total_time);

      print_string("|*h1|*f4 Average Time/Call: |*r1");
      print_expanded_time_cr(total_time/(user_lines[portnum].user_info.stats.cal
    }
*/
   ansi_on(ansi_state);
   print_cr();
}


int cmd_show_gtalk_info(com_struct *com, char *string)
{
 int ansi_state = ansi_on(1);
 printf_ansi("GTalk/UNIX\r\n");
 printf_ansi("Copyright (C) 1995 by David W. Jeske and Daniel L. Marks\r\n");
 printf_ansi("\r\nYou may obtain information about this product by writing to:\r\n\r\n");
 printf_ansi("GTalk\r\n");
 printf_ansi("P.O. Box 2721\r\n");
 printf_ansi("Glenview, IL 60025\r\n\r\n");
 printf_ansi("Or call Nuclear Greenhouse at (708) 998-0008 for information\r\n");
 printf_ansi("3/12/2400/14.4/28.8 baud 8N1\r\n\r\n");

 ansi_on(ansi_state);
}

int cmd_kill_node(com_struct *com,char *string)
{
  long int num;

  if (get_number(&(string),&num))
    {
      if (c_nodes(num)->userdata.online_info.class_info.priority <
         mynode->userdata.online_info.class_info.priority) {
         printf("--> Insufficient Priority\r\n");
         return 0;
      }
      printf("--> Killing Node [%02d]\r\n",num);
      if (((num)<0) || (num>=c_nodes_used))
	{ printf("--> Node Invalid\r\n"); return 1; }
      if (!is_node_online(c_nodes(num)))
	{ printf("--> Node Inactive\r\n"); return 1; }
      mynode->userdata.user_info.killstats.kills_total++;
      c_nodes(num)->sigusr1_action = SIG1_ACTION_KILL;
      kill(c_devices(c_nodes(num)->dev_no)->owner_pid,SIGUSR1);
      return 0;
    }
  printf("--> Invalid Command Format\r\n");
  return 1;
}

int cmd_relog_node(com_struct *com,char *string)
{
  long int num;

  if (get_number(&(string),&num))
    {
      if (c_nodes(num)->userdata.online_info.class_info.priority <
         mynode->userdata.online_info.class_info.priority) {
         printf("--> Insufficient Priority\r\n");
         return 0;
      }
      printf("--> Relogging Node [%02d]\r\n",num);
      if (((num)<0) || (num>=c_nodes_used))
	{ printf("--> Node Invalid\r\n"); return 1; }
      if (c_nodes(num)->status != NODE_ACTIVE)
	{ printf("--> Node Inactive\r\n"); return 1; }

      c_nodes(num)->sigusr1_action = SIG1_ACTION_RELOG;
      kill(c_devices(c_nodes(num)->dev_no)->owner_pid,SIGUSR1);
      return 0;
    }
  printf("--> Invalid Command Format\r\n");
  return 1;
}

int cmd_give_time(com_struct *com, char *string)
{
  long int num;
  long int timev;
  time_t newt;
  time_t tim = time(NULL);
  node_struct *n;
  char function;


  if (get_number(&(string),&num))
    {
      if ((num >= c_nodes_used) || (!is_node_online(c_nodes(num))))
	{
	  printf_ansi("--> Node not Online\r\n");
	  return (0);
	}
      if (c_nodes(num)->userdata.online_info.class_info.priority <
             mynode->userdata.online_info.class_info.priority)
        {
          printf("--> Insufficient Priority\r\n");
          return 0;
        }

      n = c_nodes(num);
      string = skip_blanks(string);
      function = *string++;
      if (get_number(&string, &timev))
	{
	  switch (function)
	    {
	      case '+':
                n->timeout_time += (timev * 60l);
		n->timeout_status = TIMEOUT_WARNING;
		ping_server();
		printf_ansi("--> Node [%02d] added %d minutes\r\n",
			    num, timev);
		return (0);
	      case '-':
		newt = n->timeout_time - (timev * 60l);
		if (newt < tim)
		  newt = tim;
		n->timeout_time = newt;
		n->timeout_status = TIMEOUT_WARNING;
		ping_server();
		printf_ansi("--> Node [%02d] removed %d minutes\r\n",
			    num, timev);
		return (0);
	    }
	}
    }
  printf_ansi("--> Error in command format\r\n");
  return (-1);
}

int get_system_no_and_node(char **cn, g_system_t *system, node_id *node)
{
  unsigned long int num1;

  *system = 0;
  *cn = skip_blanks(*cn);
  if (!get_number(cn, &num1))
    {
      printf_ansi("--> Node Number Required\r\n");
      return (-1);
    }
  if (**cn == '/')
    {
      (*cn)++;
      *system =  num1;
      if (!get_number(cn, &num1))
	{
	  printf_ansi("--> Node Number Required\r\n");
	  return (-1);
	}
    }
  *node = num1;
  if (!(*system))
    {
      if ((*node >= 0) && (*node < c_nodes_used))
	{
	  if (!is_node_online(c_nodes(*node)))
	    {
	      printf_ansi("--> Node is not present\r\n");
	      return (-1);
	    }
	  return (0);
	} else
	  {
	    printf_ansi("--> Node is out of range\r\n");
	    return (-1);
	  }
    }
  return (0);
}

int cmd_send_private_message(com_struct *com,char *string)
{
  g_system_t system;
  node_id node;
  int identified=1;
  int old_state;
  struct _user_perm *temp_data;

  old_state = ansi_on(1);

  if (get_system_no_and_node(&string, &system, &node) < 0)
    return (-1);
  string = skip_blanks(string);

  limit_carrots(string,6);

  client_abuf_writef(my_ip, node, STATE_PRIVATE,
		     "%lu/%ld %s", my_ip,mynum, string);

  if (identified)
    {
      temp_data = &(c_nodes(node)->userdata);
      if (mynode->link == FALSE) 
        printf_ansi("--> /P to #%02d:%c%s|*r1%c Sent\n",node,
 		  temp_data->online_info.class_info.staple[0],
		  temp_data->user_info.handle,
		  temp_data->online_info.class_info.staple[1]);
    }
  else 
    if (mynode->link == FALSE) 
    printf_ansi("--> /P%02d Sent\n",node);

ansi_on(old_state);
  return 0;
}

int cmd_page_node(com_struct *com, char *string)
{
  g_system_t system;
  node_id node;
  int identified=1;
  int old_state;
  struct _user_perm *temp_data;

  old_state = ansi_on(1);

  if (get_system_no_and_node(&string, &system, &node) < 0)
    return (-1);
  string = skip_blanks(string);

  limit_carrots(string,6);

  if (strlen(string) != 0)
    {
    client_abuf_writef(my_ip, node, STATE_PRIVATE,
       "%lu/%ld \007\007\007\007\007[|*f9|*p1PAGE|*p0] Reason: %s", my_ip,mynum, string);

    temp_data = &(c_nodes(node)->userdata);
    printf_ansi("--> Paging #%02d:%c%s|*r1%c\n--> Reason: %s\n",node,
		temp_data->online_info.class_info.staple[0],
		temp_data->user_info.handle,
		temp_data->online_info.class_info.staple[1], string);
    }
  else
    printf_ansi("--> A paging reason is required\n");

ansi_on(old_state);
  return 0;
}


int cmd_channel_invite(com_struct *com, char *string)
{
  g_system_t system;
  node_id node;
  char ch;

  if (!(*mynode->cur_chan))
    {
      printf_ansi("--> Not on a channel\r\n");
      return (-1);
    }
  if (get_system_no_and_node(&string, &system, &node) < 0)
    return (-1);
  if ((*string != '+') && (*string != '-') && (*string != ' ') &&
      (*string))
    {
      printf("--> Must use + or - to invite/uninvite user\r\n");
      return (0);
    }
  ch = (*string == '-') ? '-' : '+';
  if (ch == '+')
    printf_ansi("--> Inviting user\r\n");
  else
    printf_ansi("--> Uninviting user\r\n");
  client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
		     "SETPERM %s %lu/%ld I%c", 
		     mynode->cur_chan, my_ip, node, ch);
}

int cmd_channel_ban(com_struct *com, char *string)
{
  g_system_t system;
  node_id node;
  char ch;

  if (!(*mynode->cur_chan))
    {
      printf_ansi("--> Not on a channel\r\n");
      return (-1);
    }
  if (get_system_no_and_node(&string, &system, &node) < 0)
    return (-1);
  if ((*string != '+') && (*string != '-') && (*string != ' ') &&
      (*string))
    {
      printf("--> Must use + or - to ban/unban user\r\n");
      return (0);
    }
  ch = (*string == '-') ? '-' : '+';
  if (ch == '+')
    printf_ansi("--> Banning user\r\n");
  else
    printf_ansi("--> Unbanning user\r\n");
  client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
		     "SETPERM %s %lu/%ld B%c", 
		     mynode->cur_chan, my_ip, node, ch);
}

int cmd_give_moderator(com_struct *com, char *string)
{
  g_system_t system;
  node_id node;
  char ch;

  if (!(*mynode->cur_chan))
    {
      printf_ansi("--> Not on a channel\r\n");
      return (-1);
    }
  if (get_system_no_and_node(&string, &system, &node) < 0)
    return (-1);
  if ((*string != '+') && (*string != '-') && (*string != ' ') &&
      (*string))
    {
      printf("--> Must use + or - to moderate/demoderate user\r\n");
      return (0);
    }
  ch = (*string == '-') ? '-' : '+';
  if (ch == '+')
    printf_ansi("--> Giving user moderator\r\n");
  else
    printf_ansi("--> Removing user moderator\r\n");
  client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
		     "SETPERM %s %lu/%ld M%c", 
		     mynode->cur_chan, my_ip, node, ch);
}

int cmd_allow_write(com_struct *com, char *string)
{
  g_system_t system;
  node_id node;
  char ch;

  if (!(*mynode->cur_chan))
    {
      printf_ansi("--> Not on a channel\r\n");
      return (-1);
    }
  if (get_system_no_and_node(&string, &system, &node) < 0)
    return (-1);
  if ((*string != '+') && (*string != '-') && (*string != ' ') &&
      (*string))
    {
      printf("--> Must use + or - to allow/disallow writes\r\n");
      return (0);
    }
  ch = (*string == '-') ? '-' : '+';
  if (ch == '+')
    printf_ansi("--> Allowing writes\r\n");
  else
    printf_ansi("--> Disallowing writes\r\n");
  client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
		     "SETPERM %s %lu/%ld W%c", 
		     mynode->cur_chan, my_ip, node, ch);
}

int cmd_channel_lock(com_struct *com, char *string)
{
  char ch;

  if (!(*mynode->cur_chan))
    {
      printf_ansi("--> Not on a channel\r\n");
      return (-1);
    }
  if ((*string != '+') && (*string != '-') && (*string != ' ') &&
      (*string))
    {
      printf("--> Must use + or - to lock/unlock channel");
      return (0);
    }
  ch = (*string == '+') ? 'A' : 'a';
  if (ch == 'a')
    printf_ansi("--> Channel Unlocked\r\n");
  else
    printf_ansi("--> Channel Locked\r\n");
  client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
		     "SETCHAN %s %c", 
		     mynode->cur_chan, ch);
}

int cmd_lineout_counter(com_struct *com, char *string)
{
  unsigned long int num;

  if (!(*mynode->cur_chan))
    {
      printf_ansi("--> Not on a channel\r\n");
      return (-1);
    }
  if (!get_number(&string, &num))
    {
      printf_ansi("--> Must include lineout count\r\n");
      return (-1);
    }
  client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
		     "SETCHAN %s L%lu", 
		     mynode->cur_chan, num);
}


void print_device_info(int devnum,int title)
{
 struct _device_struct *ptr = c_devices(devnum);
 
 if (title) {
   printf("[DEVICE LIST]      Lock DTE Baud----+\r\n");
   printf("                          RTS/CTS--+|\r\n");
   printf("                                   ||\r\n");
   printf("Num  STAT PID  Type  ass/ret Baud  || Name\r\n");
 }
 
 printf("%02d  ",devnum);
 switch(ptr->status) {
 case DEVICE_UNUSED:
   printf("UNUSED     ");
   break;
 case DEVICE_USED:
   printf(" USED %04d ",ptr->owner_pid);
   break;
 case DEVICE_TERM:
   printf(" TERM %04d ",ptr->term_pid);
   break;
 case DEVICE_OFF:
   printf("OFF/ERROR  ");
   break;
 default:
   printf("??????     ");
   break;
 }      
 
 switch(ptr->node_type) {
 case SERIAL_NODE_TYPE:
   printf("Serial #r%02d   %05d %d%d",ptr->retries,ptr->baud_rate,
	  ptr->rts_cts,ptr->lock_dte);
 case TELNET_NODE_TYPE:
   printf("Telnet %04d           ",ptr->assist_pid);
   break;
 case DIRECT_NODE_TYPE:
   printf("Direct #r%02d           ",ptr->retries);
   break;
 case MODEM_NODE_TYPE:
   printf("Modem  #r%02d   %05d %d%d",ptr->retries,ptr->baud_rate,
	  ptr->rts_cts,ptr->lock_dte);
   break;
 }
 
 if (ptr->name[0])
   printf(" %s",ptr->name);
 else
   printf(" (no name)");
 printf_ansi("\n");
 
}

int cmd_reset_device(com_struct *com, char *string)
{
  unsigned long int num;
  char s[2];
  pid_t pid_to_kill;

  string = skip_blanks(string);
  if (get_number(&(string),&num))
    {
      if (num>c_devices_used)
	{
	  printf("--> Invalid Device\r\n");
	  return -1;
	}
      if (c_devices(num)->status==DEVICE_EMPTY)
	{
	  printf("--> Inactive Device\r\n");
	  return -1;
	}
      printf("--> Reset Device #%02d Requested\r\n\r\n");

      print_device_info(num,1);

      if (c_devices(num)->status==DEVICE_OFF)
	{
	  printf("NOTE: Device is currently OFF because of an error\r\n");
	  printf("      Resetting this device will make it active again\r\n");
	  printf("\r\n");
	  if (get_yes_no("Are you sure you want to reactive this device?")) {
	    /* turn it back on! */
	    client_abuf_writef(my_ip, SERVER_PROCESS, STATE_TERM, 
			       "FORCE %d +", num);
	    ping_server();
	  }
	  return 0;
	}

      printf("\r\n");
      pid_to_kill = c_devices(num)->owner_pid;
      if ((pid_to_kill == 0) || (pid_to_kill==-1))
	{
	  printf("--> Invalid PID (%d)\r\n",pid_to_kill);
	  return -1;
	}
      if (get_yes_no("Are you sure you want to reset this device?")) {
	/* reset the device */
	kill(pid_to_kill, SIGHUP);
	printf("--> Sending SIGHUP to PID(%d)\r\n",pid_to_kill);
	return 0;
      } else {
	printf("--> Reset Aborted\r\n");
	return -1;
      }  
    }

  printf("--> Invalid Command Format\r\n");
  return -1;
}

int cmd_device_list(com_struct *com, char *string)
{
  int device_count;
  int len;
  int ansi_state = ansi_on(1);
  char buf[80];
  time_t time_now = time(NULL);
  struct _device_struct *ptr;


  printf("[DEVICE LIST]      Lock DTE Baud----+\r\n");
  printf("                          RTS/CTS--+|\r\n");
  printf("                                   ||\r\n");
  printf("Num  STAT PID  Type  ass/ret Baud  || Name\r\n");
  
  for(device_count=0;device_count<c_devices_used;device_count++) 
    if ((ptr=c_devices(device_count)) && (ptr->status)) {
      printf("%02d  ",device_count);
      switch(ptr->status) {
      case DEVICE_UNUSED:
	printf("UNUSED     ");
	break;
      case DEVICE_USED:
	printf(" USED %04d ",ptr->owner_pid);
	break;
      case DEVICE_TERM:
	printf(" TERM %04d ",ptr->term_pid);
	break;
      case DEVICE_OFF:
	printf(" OFF/ERROR ");
	break;
      default:
	printf("??????     ");
	break;
      }      

      switch(ptr->node_type) {
      case SERIAL_NODE_TYPE:
	printf("Serial #r%02d   %05d %d%d",ptr->retries,ptr->baud_rate,
	       ptr->rts_cts,ptr->lock_dte);
      case TELNET_NODE_TYPE:
	printf("Telnet %04d           ",ptr->assist_pid);
	break;
      case DIRECT_NODE_TYPE:
	printf("Direct #r%02d           ",ptr->retries);
	break;
      case MODEM_NODE_TYPE:
	printf("Modem  #r%02d   %05d %d%d",ptr->retries,ptr->baud_rate,
	       ptr->rts_cts,ptr->lock_dte);
	break;
      }
      
      if (ptr->name[0])
	printf(" %s",ptr->name);
      else
	printf(" (no name)");
        printf_ansi("\n");
    }
      
  ansi_on(ansi_state);
}


int cmd_make_access(com_struct *com, char *string)
{
  char *old_str;
  char *temp_str;
  int len;
  int flag_num;
  g_system_t system;
  node_id node;
  char operation;
  int is_cmd=0;
  char *flag_str;
  com_struct *make_com;
 
  /* first get the node number */

  if (get_system_no_and_node(&string, &system, &node) < 0)
    return (-1);

  if (c_nodes(node)->userdata.online_info.class_info.priority < 
      mynode->userdata.online_info.class_info.priority) {
    printf("--> Insufficient Priority\r\n");
    return 0;
  }

  string = skip_blanks(string);
  if ((*string=='+') || (*string=='-')) {
    operation = *string;
    string++;
  } else {
    if (*string=='=') {
      char class_name[CLASS_NAME_LEN+1];
      struct class_data temp_class;
      string++;
      get_string(class_name,&string, CLASS_NAME_LEN, 1, 0 , 1);
      fix_classname(class_name);
      if (!read_class_by_name(class_name,&temp_class)) {
	if (temp_class.class_info.priority < 
	    mynode->userdata.online_info.class_info.priority) {
	  printf("--> Insufficient Priority to load class [%s]\r\n",
		 class_name);
	  return -1;
	}
	if (temp_class.class_info.priority < 10) {
	  /* GET THE USER EDITOR PASSWORD */
	  {
	    char s[10];
	    printf_ansi("Enter Pass: ");
	    get_input_cntrl(s,10,GI_FLAG_MASK_ECHO);
	    if (strcmp(s,USEREDIT_PASSWD))
	      {
		printf("\r\n--> Invalid Password\r\n");
		return -1;
	     }
	  }
	}
	set_online_info(c_nodes(node),NULL,&temp_class);
	printf("--> Node #%02d set to Class [%s]\r\n",node,class_name);
      } else {
	printf("--> Class [%s] does not exist\r\n",class_name);
	return -1;
      }
      return 0;
    } else if (*string=='#') {
      char class_name[CLASS_NAME_LEN+1];
      struct class_data temp_class;
      string++;
      get_string(class_name,&string, CLASS_NAME_LEN, 1, 0 , 1);
      fix_classname(class_name);
      if (!read_class_by_name(class_name,&temp_class)) {
	if (temp_class.class_info.priority < 
	    mynode->userdata.online_info.class_info.priority) {
	  printf("--> Insufficient Priority to load class [%s]\r\n",
		 class_name);
	  return -1;
	}
	if (temp_class.class_info.priority < 10) {
	  /* GET THE USER EDITOR PASSWORD */
	  {
	    char s[10];
	    printf_ansi("Enter Pass: ");
	    get_input_cntrl(s,10,GI_FLAG_MASK_ECHO);
	    if (strcmp(s,USEREDIT_PASSWD))
	      {
		printf("\r\n--> Invalid Password\r\n");
		return -1;
	     }
	  }
	}
	set_online_info(c_nodes(node),NULL,&temp_class);
	printf("--> Node #%02d set to Class [%s]\r\n",node,class_name);
      } else {
	printf("--> Class [%s] does not exist\r\n",class_name);
	return -1;
      }
      return 0;
    }

    printf("--> Invalid Command Format\r\n");
    return -1;
  }

  if (*string=='/')
    {
      is_cmd=1;
      string++;
    }

  string = skip_blanks(string);
  old_str = string;
  temp_str = string;
  flag_str = string;
  len = strlen(string);

  /* convert it to uppercase */

  while ((is_alpha(*temp_str)) && (len > 0))
    {
      *temp_str = upcase(*temp_str);
      temp_str++;
      len--;
    }

  if (is_cmd) {
    /* if it's a slash command find the command name */
      if (!(make_com=find_command(&string)))
	{
	  printf("--> Invalid command [/%s]. Type /? for a list.\r\n",old_str);
	  return -1;
	}    
      flag_str = make_com->flag_name;
  } else {
    /* see if the flag exists */

    flag_num = findFlagNumber(string);
    
    if (flag_num==-1) {
      printf_ansi("--> Flag [%s] not found\n",old_str);
      return -1;
    }
  }

  /* make sure they asked for a legal operation */

  if (!testFlag(mynode,flag_str))
    {
      printf("--> Insufficient Priority\r\n");
      return -1;
    }

  if (operation=='+') {
    setFlag(c_nodes(node),flag_str,1);
    printf("--> Enabled [%s] for Node #%02d\r\n",flag_str,node);
  } else {
    if ((operation=='-')) {
     setFlag(c_nodes(node),flag_str,0); 
    printf("--> Disabled [%s] for Node #%02d\r\n",flag_str,node);
    } 
  }
  
  /* mark that the class has been altered */
  if (c_nodes(node)->userdata.online_info.class_info.class_name[0]!='*')
    {
      char temp_var[CLASS_NAME_LEN+1];
      char *clname = c_nodes(node)->userdata.online_info.class_info.class_name;
      temp_var[0]='*';
      strncpy(temp_var+1,clname,CLASS_NAME_LEN-2);
      temp_var[CLASS_NAME_LEN-1]=0;
      strcpy(clname,temp_var);
    }
}



int cmd_print_caller_log(com_struct *com, char *string)
{
  int num_lines=10;
  int ansi_state = ansi_on(1);
  long int num;

  if (get_number(&string,&num))
    {
      if ((num>0) && (num<400))
	num_lines = num;
      else
	{
	  printf("--> Invalid Format\r\n");
	  return -1;
	}
    }

  printf_ansi("|*ffLast [|*r1%d|*ff] callers:\n",num_lines);
  printf_ansi("|*r1|*f4------------------|*r1\n");
  print_file_tail_grep("log/signout.log",num_lines,NULL,PFC_ANSI|PFC_ABORT);
  printf_ansi("|*r1|*f4-[|*r1end|*r1|*f4]------------|*r1\n");

  ansi_on(ansi_state);

}

int cmd_print_sysinfo(com_struct *com, char *string)
{
  int ansi_state = ansi_on(1);

  printf_ansi("\r\n|*ffSystem Name|*r1:   %s\r\n",c_sys_info->system_name);
  printf_ansi("|*ffLock Priority|*r1: %d\r\n\r\n",c_sys_info->lock_priority);

  ansi_on(ansi_state);
}

int cmd_lock_system(com_struct *com,char *string)
{
 long int num;

 if (get_number(&string,&num))
   {
     if ((num<1) || (num>255)) {
       printf("--> Lock Priority must be 1-255\r\n");
       return -1;
     }
     printf("--> Modems Locked to Priority [%d]\r\n",num);
     c_sys_info->lock_priority = num;
   }
 else
   {
     printf("--> System Unlocked\r\n");
     c_sys_info->lock_priority=0;
   }
}

int cmd_lock_telnet(com_struct *com,char *string)
{
 long int num;

 if (get_number(&string,&num))
   {
     if ((num<1) || (num>255)) {
       printf("--> Lock Priority must be 1-255\r\n");
       return -1;
     }
     printf("--> Telnet Locked to Priority [%d]\r\n",num);
     c_sys_info->lock_priority_telnet = num;
   }
 else
   {
     printf("--> Telnet Unlocked\r\n");
     c_sys_info->lock_priority_telnet=0;
   }
}


int cmd_print_message_box(com_struct *com, char *string)
{
  unsigned long int num;
  char path[250];
  char s[250];
  int ansi_state = ansi_on(1);

  if (!get_number(&string, &num))
    {
      /* no number was there so show them the index */

      sprintf(s, "%s/home/gtalk/rotator/index.hdr", GTALK_ROOT_DIR);
      print_file_cntrl(s, PFC_ANSI);
      sprintf(s, "%s/home/gtalk/rotator/index.txt", GTALK_ROOT_DIR);
      print_file_cntrl(s, (PFC_ANSI|PFC_ABORT|PFC_PAGING));
      return 0;
    }
  
  printf_ansi("|*ff--> Message Box [|*r1%03d|*ff]|*r1\n",num);
  sprintf(path,"%s/home/gtalk/rotator/rot%03d", GTALK_ROOT_DIR, num);
  print_file_cntrl(path,PFC_ANSI|PFC_ABORT|PFC_PAGING);
  printf_ansi("|*p0|*ff--> End [|*r1%03d|*ff]|*r1\n",num); 
  ansi_on(ansi_state);
}

void cmd_display_user_list(void)
{
  int ansi_state = ansi_on(1);
  print_file_cntrl("text/userlist.hdr", PFC_ANSI);
  print_file_cntrl("text/userlist.txt", (PFC_ANSI|PFC_ABORT|PFC_PAGING));
  print_file_cntrl("text/userlist.footer", PFC_ANSI);
  ansi_on(ansi_state);
}

int cmd_enter_bbs(com_struct *com)
  {
  char s[250];

  printf("--> Entering BBS subsystem\r\n");
  sprintf(s, "%s/bin/bbs", GTALK_HOME_DIR);
  exec_user_program(s, mynode, COM_NOCHROOT);
}

int cmd_do_user_config(void)
   {
   int ansi_state = 1;
   char tmp[100];
   char email_name[25+1];
   char email_login[3+1];
   char newstr[25];
   char menu_main_file[25];

   ansi_on(ansi_state);
   printf("--> Entering User Configuration Menu\r\n");
   sprintf(menu_main_file,"text/menu/config.menu");
   print_file(menu_main_file);

/* sprintf(tmp,"/usr/bin/chfn %s", mynode->userdata.user_info.login); */
   printf_ansi("|*r1--> Enter the user number to change (|*ff###|*r1): ");
   get_input_cntrl(email_login,3,GI_FLAG_NO_ESC | GI_FLAG_NO_ABORT);

   printf_ansi("|*r1--> Enter the |*f9NEW|*r1 Email name (GECOS) field: ");
   get_input_cntrl(email_name,25,0);

/*
   get_input(email_login,4);
   printf("Enter the NEW Email name (GECOS) field: ");
   get_input(email_name,25);
*/

   sprintf(tmp,"/usr/bin/chfn -f \"%s\" u%03s", email_name, email_login);
/* sprintf(tmp,"/usr/bin/chfn %s",mynode->userdata.user_info.login); */
   printf("--> Command: %s\r\n--> Feature temporarily disabled\r\n",tmp);

/* exec_user_program(tmp, mynode, COM_NOCHROOT); */
/* system(tmp); */
   tty_raw(STDIN_FILENO);
/* exec_user_program("/usr/bin/chfn", mynode, COM_NOCHROOT); */
}

int cmd_wall_message(com_struct *com, char *string)
{
  if (!(*string)) {
    printf("--> Usage: /WALL <message>\r\n");
    return -1;
  } else {
	client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
		"WALL %s %s", mynode->cur_chan, string);
  }
}

int cmd_walla_message(com_struct *com, char *string)
{
  if (*string == '`') ++string;
  if (!(*string)) {
    printf("--> Usage: /WALLA <message>\r\n");
    return -1;
  } else {
        client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
                "WALLA %s %s", mynode->cur_chan, string);
  }
}

int cmd_lurk_message(com_struct *com, char *string)
{
  char s_tmp[1024];
  if (*string == '!') ++string;

  if (!(*string)) {
    printf("--> Usage: !<message>\r\n");
    return -1;
  } else {
        client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
                "MSGLURK %s %s", mynode->cur_chan, "TESTING!"); /* string);
/*
	sprintf(s_tmp, my_ip, SERVER_PROCESS, STATE_CHANNEL,
		"MSGLURK %s %s", mynode->cur_chan, string);
	printf("Writing to Server: |%s|",s_tmp);
*/
	printf("--> Sent Lurked msg = |%s|\r\n", string);
  }
}

int cmd_raw_write (com_struct *com, char *string)
{
  if (!(*string)) {
    printf("--> Usage: /RAW <msg type> <channel> <string>\r\n");
    return -1;
  } else {
        client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
                "%s", string);
/*	log_error("Writing RAW Server message: |%s|", string); */
  }
}

