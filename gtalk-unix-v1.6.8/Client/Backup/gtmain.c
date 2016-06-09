/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - gtmain.c
 *
 * This is where things start to happen on the "client" side of 
 * the Gtalk/UNIX system.
 *
 *
 * Features to add:
 *
 *  Lineouts
 *  Staples
 *  line enter mode (MIL) - ESC, backspace, 
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
#include <setjmp.h>
#include <sys/wait.h>

#include "types.h"
#include "common.h"
#include "gtmain.h"
#include "abuf.h"
#include "answer.h"
#include "editor.h"
#include "comparse.h"
#include "command.h"
#include "states.h"
#include "channel.h"
#include "shared.h"
#include "client_run.h"
#include "squelch.h"

/* REAL includes */
#include <sys/types.h>
#include "user.h"
#include <pwd.h>
#ifdef GT_SHADOW
#include <shadow.h>
#endif
#include "accounting.h"

#include "input.h"
#include "ddial.h"
#include "ansi.h"

void save_online_user(void);
int mynum;
node_struct *mynode;
device_struct *mydev;
int mypipe;
int ml_logout;

#define LOGIN_PROMPT_TIMEOUT (3)  /* number of minutes to wait at the
				     login prompt before timing out */
#define BULLSHIT_TIMEOUT (10) /* timeout for users */
#define DEFAULT_TIMEOUT (10)  /* timeout for guests */



void init_mynode_struct(node_struct *a_node)
{
  a_node->userdata.online_info.login_time = time(NULL);
  a_node->userdata.online_info.warnings = 0;
  strcpy(a_node->userdata.online_info.class_info.class_name,"");
  strcpy(a_node->userdata.user_info.class_name,"");
  a_node->userdata.online_info.width = 80;
  a_node->userdata.online_info.class_info.time = 60 * DEFAULT_TIMEOUT;
  a_node->userdata.online_info.class_info.priority = 50;
  a_node->userdata.online_info.location[0]=0;

  a_node->userdata.online_info.logged_in_flag = 0;
  *a_node->cur_chan = '\000';
  *a_node->new_chan = '\000';
  a_node->sigusr1_action = SIG1_ACTION_NONE;
}


void set_online_info(node_struct *a_node,
		     struct unique_information_struct *user_data,
		     struct class_data *class_info)
{
  if (user_data)
    a_node->userdata.user_info = *user_data;
  a_node->userdata.online_info.class_info = class_info->class_info;
  
  if (a_node->userdata.online_info.class_info.time)
    {
      a_node->timeout_time = time(NULL)+(a_node->userdata.online_info.class_info.time * (60));
      a_node->timeout_status = TIMEOUT_WARNING;
    }
  else
    {
      a_node->timeout_status = TIMEOUT_NONE;
      a_node->timeout_time = 0;
    }
  ping_server(); /* to make the timeout take effect */
}

void get_online_info(struct user_data *user_info, struct class_data *class_info,node_struct *a_node)
{
   if (user_info)
     user_info->user_info = a_node->userdata.user_info;
   if (class_info)
     class_info->class_info = a_node->userdata.online_info.class_info;
}

static void sighup_handler(int signo)
{
  save_online_user();
  exit(0);
}

void make_manual_guest(struct class_data *cptr)
{
  cptr->class_info.staple[0] = '(';
  cptr->class_info.staple[1] = ')';
  cptr->class_info.staple[2] = '(';
  cptr->class_info.staple[3] = ')';
  strcpy(cptr->class_info.class_name,"*GUEST");
  strcpy(cptr->class_info.verbose_class_name,"Guest Failsafe (hardcoded)");
  cptr->class_info.time = 10;
  cptr->class_info.added_time = 5;
  cptr->class_info.line_out = 5;
  cptr->class_info.class_index = -1;
  cptr->class_info.priority = 50;
  bzero(cptr->class_info.privs,MAX_NUM_PRIV_CHARS);
  cptr->class_info.privs[0]=0xFF;
  cptr->class_info.privs[1]=0xFF;
}

void make_manual_user(struct class_data *cptr)
{
  cptr->class_info.staple[0] = '[';
  cptr->class_info.staple[1] = ')';
  cptr->class_info.staple[2] = '[';
  cptr->class_info.staple[3] = ']';
  strcpy(cptr->class_info.class_name,"*USER");
  strcpy(cptr->class_info.verbose_class_name,"User Failsafe (hardcoded)");
  cptr->class_info.time = 60;
  cptr->class_info.added_time = 10;
  cptr->class_info.line_out = 5;
  cptr->class_info.class_index = -1;
  cptr->class_info.priority = 30;
  bzero(cptr->class_info.privs,MAX_NUM_PRIV_CHARS);
  cptr->class_info.privs[0]=0xFF;
  cptr->class_info.privs[1]=0xFF;
}

/* this will autodetect ANSI */

int find_ansi(void)
{
   char s[10];
   time_t myt = time(NULL);
   int inchar;
   int isthere;
   int flag = 1;
   fd_set read_fd;
   int temp;
   char buf[30];
   struct timeval timeout;
   int char_count=0;

   printf("%c[6n",27);  /* Send ANSI check code */
   fflush(stdout);
   while (((time(NULL)-myt)<4) && flag)
     {
       FD_ZERO(&read_fd);
       FD_SET(0, &read_fd);
       
       timeout.tv_sec=1;
       timeout.tv_usec=0;
       temp = select(mypipe+1, &read_fd, NULL, NULL, &timeout);    
       if (temp>0)
	 {
	   char_count += read(0,buf,30);
	   if (buf[0]==27)
	     flag=0;
	 }
     };
   print_cr();         /* Print two carriage returns to clean up */
   print_cr();
   if (!flag) 
     reset_attributes();

   empty_inbuffer();
   fflush(stdout);
   return (!flag);
};


void get_ansi_mode(void)
{
  int detected_state;
  sleep(2);

  printf("Welcome to GTalk, Detecting ANSI...");
  fflush(stdout);
  detected_state = find_ansi();

  if (detected_state) {
    printf("[ ANSI Detected ]\r\n");
  } else {
    printf("[ ANSI NOT Detected]\r\n");
  }

  set_term_ansi(detected_state);
  set_term_ascii(detected_state);

}

int setup_guest_login(node_struct *a_node)
{
  struct passwd *pass_info;
  struct class_data tempclass;
  
  mynode->userdata.online_info.login_time = time(NULL);
  mynode->userdata.online_info.logged_in_flag=1;
  pass_info = getpwnam("gtgst"); /* load guest UNIX account */
  if (!pass_info)
    {
      log_error("UNIX account gtgst does not exist!!");
      return -1;
    }
  mynode->userdata.online_info.unix_passinfo = *pass_info;
  
  /* do guest login */
  if (read_class_by_name("GUEST",&tempclass))
    {
      log_error("Error Loading GUEST class\r\n");
      printf("Error Loading GUEST class\r\n");
      make_manual_guest(&tempclass);
    }
  bzero(&a_node->userdata.user_info,sizeof(a_node->userdata.user_info));
  strcpy(a_node->userdata.user_info.class_name,"GUEST");
  a_node->userdata.user_info.user_no = GUEST_USER_NUMBER;
  a_node->userdata.user_info.enable = 0;
  strcpy(a_node->userdata.user_info.login,"gtgst");
  strcpy(a_node->userdata.user_info.handle,"Guest");
  a_node->userdata.user_info.width=80;
  bzero(&a_node->userdata.online_info.class_info,
	sizeof(a_node->userdata.online_info.class_info));
  set_online_info(a_node,NULL,&tempclass);
 
  return 0;
}

int pass_prompt(void)
{
  char s[100];
  int user_number;
  char salt[10];
  struct user_data user_info;
  struct passwd *uxacct_info;
#ifdef GT_SHADOW
  struct spwd *pass_info;
#endif
  int valid_login=0;
  int num_tries_left = 3;
  int ansi_state;
  
  
  mynode->status = NODE_LOGGING_IN;
  mynode->timeout_time = time(NULL)+(60*(LOGIN_PROMPT_TIMEOUT));
  mynode->timeout_status = TIMEOUT_WARNING;
  ping_server();
  empty_inbuffer();

  while ((num_tries_left--) && (!valid_login))
    {
      printf("\r\n\nEnter <RETURN> for Guest access.\r\n");
      printf("\r\nUser ID: ");
      get_input_cntrl(s,10,GI_FLAG_NO_ESC | GI_FLAG_NO_ABORT);
      
      if (s[0]==0)
	{ 
	  if (setup_guest_login(mynode))
	    {
	      printf("Error Setting up GUEST login\r\n");
	      exit(1);
	    }

	    ansi_state = ansi_on(1);
 	    printf_ansi("|*r1\r\nEnter Handle: ");
            get_input_cntrl(mynode->userdata.user_info.handle,HANDLE_LEN,
			    (GI_FLAG_NO_EMPTY | GI_FLAG_NO_ESC));
            printf_ansi("\r\n|*r1\r\n");
	    remove_ansi(mynode->userdata.user_info.handle);
	    ansi_on(ansi_state);
	  return 0;
	} 
      
      if ((s[0]>='0') && (s[0]<='9'))
	user_number = atol(s);
      else 
	{ 
	  printf("LOGIN entered (%s) - NOT SUPPORTED YET\r\n",s);
	  return -1;
	}
      
      if (!read_user_record(user_number, &user_info))
	{
	  uxacct_info = getpwnam(user_info.user_info.login);
#ifdef GT_SHADOW
          pass_info = getspnam(user_info.user_info.login);
#endif
	  
	  if (!uxacct_info)
	    { printf("No such User [%s]\r\n",user_info.user_info.login);
	      printf("Password: ");
	      get_input_cntrl(s,10,(GI_FLAG_MASK_ECHO | GI_FLAG_NO_EMPTY));
	    }
	  else
	    {
#ifdef GT_SHADOW
	      strncpy(salt,pass_info->sp_pwdp,2);
#else
              strncpy(salt,uxacct_info->pw_passwd,2);
#endif
	      salt[2]=0;
	      printf("Password: ");
	      get_input_cntrl(s,10,(GI_FLAG_MASK_ECHO | GI_FLAG_NO_EMPTY));

#ifdef GT_SHADOW	      
	      if (!strcmp(pass_info->sp_pwdp,(char *)crypt(s,salt)))
		  valid_login=1;
#else
              if (!strcmp(uxacct_info->pw_passwd,(char *)crypt(s,salt)))
                  valid_login=1;
#endif
	      strcpy(s,"abcde"); /* blank out the Password in memory */

	    }
	}
      else
	{
	  printf("\r\nReading of user file failed for User %d\r\n",
		 user_number);
	  printf("Password: ");
	  get_input_cntrl(s,10,(GI_FLAG_MASK_ECHO | GI_FLAG_NO_EMPTY));
	}
    }
     
  if (valid_login)
    { 
      struct class_data class_info;

      bzero(&class_info, sizeof(class_info));

	/* we need to read the class information so login_accounting_check()
         * will have the information it needs available
         */

      if (read_class_by_name(user_info.user_info.class_name,&class_info))
        {
          /* error reading class */
          printf("Error reading class [%s]\n",user_info.user_info.class_name);
          log_error("Error reading class [%s]",user_info.user_info.class_name);
          if (read_class_by_name("UNKNOWN",&class_info))
              {
                log_error("Error reading class [UNKNOWN]");
                make_manual_user(&class_info);
              }
        }

      login_accounting_check(&user_info.user_info,&class_info.class_info);

	/*
	 * login_accounting_check might have changed the user's class, so
	 * lets re-read it now.
	 */

      printf("User Class: %s\r\n",user_info.user_info.class_name);
      if (read_class_by_name(user_info.user_info.class_name,&class_info))
	{
	  /* error reading class */
	  printf("Error reading class [%s]\n",user_info.user_info.class_name);
	  log_error("Error reading class [%s]",user_info.user_info.class_name);
	  if (read_class_by_name("UNKNOWN",&class_info))
	      {
		log_error("Error reading class [UNKNOWN]");
		make_manual_user(&class_info);
	      }
	}

      /* if the login was successful, setup their online info and check
         necessary stuff */

      mynode->userdata.online_info.login_time = time(NULL);

      set_online_info(mynode,&(user_info.user_info),&class_info);
      mynode->userdata.online_info.logged_in_flag=1;
      mynode->userdata.online_info.unix_passinfo = *uxacct_info;
      return 0;
    }
  else
    {
      return -1;
    }
      
}

void save_online_user(void)
{
 
 struct user_data user_info;
 int number = mynode->userdata.user_info.user_no;
 char s[20];

 if (!mynode->userdata.online_info.logged_in_flag)
   { 
     log_error("save_online_user: ERR (logged_in_flag == 0)");
     return;
   }

 if (number<0)
   strcpy(s,"%GST");
 else
   sprintf(s,"#%03d",mynode->userdata.user_info.user_no);

 switch(mynode->sigusr1_action)
   {
   case SIG1_ACTION_TIMEOUT:
     log_event("log/signout.log","Timeout| %s:%s|*r1",
	       s,mynode->userdata.user_info.handle);
     break;
   case SIG1_ACTION_RELOG:
     log_event("log/signout.log","Relog  | %s:%s|*r1",
	       s,mynode->userdata.user_info.handle);
     break;
   case SIG1_ACTION_LINEOUT:
     log_event("log/signout.log","Lineout| %s:%s|*r1",
	       s,mynode->userdata.user_info.handle);
     break;
   case SIG1_ACTION_KILL:
     log_event("log/signout.log","|*f1|*h1Kill|*r1   | %s:%s|*r1",
	       s,mynode->userdata.user_info.handle);
     break;
   default:
     log_event("log/signout.log","Logout | %s:%s|*r1",
	       s,mynode->userdata.user_info.handle);
     break;
   }

 if (number<0)
   {
     return;
   }

 if (read_user_record(number, &user_info))
   {
     log_error("save_online_user: ERR reading user record");
     return;
   }

 get_online_info(&user_info, NULL,mynode);

 mynode->userdata.online_info.logged_in_flag=0;
 /*   mynode->status=NODE_CLEANUP; */

 if (user_info.user_info.enable==0)
   return; 
 user_info.user_info.last_call = time(NULL);


  if (!save_user_record(number, &user_info))
    { 
      mynode->userdata.online_info.logged_in_flag=0;
    }
  else
    {
      return;
    }

}

void process_command(char *cstring)
{
  com_struct *com;
  char old_channel_name[CHANNEL_NAME_LEN+1];

  while (*(++cstring) == ' ');
  if ((*cstring == 'h') || (*cstring == 'H'))
    {
      cmd_change_handle(NULL, ++cstring);
      return;
    }
  if (!(com=find_command(&cstring)))
    {
      printf("--> Invalid Command. Type /? for a list of commands.\r\n");
      return;
    }
  while (*cstring == ' ')
    cstring++;

  if (com->flag_name)
    {
      if (!testFlag(mynode, com->flag_name))
	{
	  printf("--> Insufficient Privilege\r\n");
	  return;
	}
    }
  
  /* 
   * now we're going to run the command selected 
   * first, notify the system the user is leaving if necessary
   */

  strcpy(old_channel_name,mynode->cur_chan);


  if (com->run_command) {
    if (com->location) {
      if (*mynode->cur_chan)
	client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
			   "LEAVE %s %s",  mynode->cur_chan, com->location); 
        mynode->cur_chan[0]=0;
      
      strncpy(mynode->userdata.online_info.location,com->location,LOCATION_LEN);
      mynode->userdata.online_info.location[LOCATION_LEN]=0;

    } else {
      if (*mynode->cur_chan)
	client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
			   "LEAVE %s %s",  mynode->cur_chan, com->location); 
        mynode->cur_chan[0]=0;
    }
    exec_user_program(com->run_command, mynode, com->options);
    
    mynode->userdata.online_info.location[0]=0;
    if (old_channel_name[0]) {
      strcpy(mynode->new_chan, old_channel_name);
      if (*mynode->new_chan)
	client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
			   "JOIN %s %s", mynode->new_chan, com->location);
    }
  }  else {
    if (com->location) {
      if (*mynode->cur_chan)
	client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
			   "LEAVE %s %s",  mynode->cur_chan, com->location); 
        mynode->cur_chan[0] = 0;

      strncpy(mynode->userdata.online_info.location,com->location,LOCATION_LEN);
      mynode->userdata.online_info.location[LOCATION_LEN]=0;
    }
    
    if (com->cfunc)
      (com->cfunc)(com, cstring);

    if (com->location) {
      mynode->userdata.online_info.location[0]=0;

      if (old_channel_name[0]) {
	strcpy(mynode->new_chan, old_channel_name);
	if (*mynode->new_chan)
	  client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
			     "JOIN %s %s", mynode->new_chan, com->location);
      }
    }
  }
}


void main_loop(void)
{
  fd_set read_fd;
  char s[1024];
  int temp;
  abuffer abuf;

  ansi_on(1);
  printf_ansi("\n|*f2|*h1Welcome to Gtalk|*r1|*h1/|*f1|*h1UNIX|*r1!\r\n");

  ansi_on(0);
  ml_logout = 0;
  while (!ml_logout)
    {
      FD_ZERO(&read_fd);
      FD_SET(mypipe, &read_fd);
      FD_SET(0, &read_fd);
      
      temp = select(mypipe+1, &read_fd, NULL, NULL, NULL);
      if (temp > 0)
	{
	  if (FD_ISSET(0, &read_fd))
	    {
	      get_input(s,sizeof(s)-1);

/* TEMPORARILY REMOVED -Gregg
              if (!strcmp(s,"/link"))
		link_main_loop();
*/
	      if (*s == '/')
		process_command(s);
	      else if (*s == '~' && testFlag(mynode,"MIL_DOANSI"))
		{
		  if (*mynode->cur_chan)
		      write_to_channel(mynode->cur_chan,"Coming soon to an NG near you: ~ (Action Commands)");
		  else
		      printf_ansi("--> No current channel\n\r");
		}
	      else if (*s)
		{
		  if (*mynode->cur_chan)
		    {
		      if (!testFlag(mynode,"MIL_DOANSI")) {
			remove_ansi(s);
		      } else {
			if (!testFlag(mynode,"MIL_DOFLASHING"))
			  remove_flashing(s);
		      }
		      limit_carrots(s,6);
		      write_to_channel(mynode->cur_chan, s);
		    }
		  else
		    printf_ansi("--> No current channel\n\r");
		}
	    }
	  if (FD_ISSET(mypipe, &read_fd))
	    {
	      if (read_abuffer(mypipe, &abuf, s, sizeof(s)-1) > 0)
		{ ansi_on(1);
		  call_state_machine(&abuf, s, &child_state_list);
		  ansi_on(0);
		}
	    }
	}
    }
};      




static void sigsegv_handler(int signo)
{
  abuffer abuf;
  char error_trap_str[]="\r\n\r\nCaught SIGSEGV: Gtalk Error Trap\r\n\r\n";

  abuf_to_server(&abuf);
  abuf.type = 0;
  writef_abuf(mypipe, &abuf, "|*r1|*f1--> Node #%02d(T1) Logout at xx:xx:xx\r\n--> #%03d:[%s|*r1|*f1]|*r1",mynum,
			mynode->userdata.user_info.user_no,mynode->userdata.user_info.handle);
  write(0,error_trap_str,strlen(error_trap_str));
  exit(0);
}


static hup_happened=0;

void relog_sighup_handler(int sig)
{
   hup_happened=1;
}

void launch_new_client(int nodenum, int pipe_fd, int relog)
{
  char s_nodenum[MAX_ENV_LN_LEN+1];
  char s_pipe_fd[MAX_ENV_LN_LEN+1];
  char s_relog[MAX_ENV_LN_LEN+1];

  sprintf(s_nodenum,"%d", nodenum);
  sprintf(s_pipe_fd,"%d", pipe_fd);
  sprintf(s_relog,"%d", relog);
  execl(GTALK_CLIENT, GTALK_CLIENT, s_nodenum, s_pipe_fd, s_relog, NULL);
  /* shouldn't get down here */
  exit(1);
}


void respawn_self(int mynumber, int mypipenumber, int flag)
{
  void (*old_sighandler)(int);
  int info;

  /* first we need to kill all our children */

  client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL, "BYE Relog");
  
  old_sighandler = signal(SIGHUP, relog_sighup_handler);
  kill(0,SIGHUP);
  while(!hup_happened)
   sleep(1);
  sleep(1);
  while (waitpid(-1,&info,WNOHANG)>0);
  signal(SIGHUP,old_sighandler);

  /* now we should tell the server to act like we died, and clean
     up stuff for our node */


  /* now we exec another child for ourself */

  launch_new_client(mynumber, mypipenumber, flag);
}

int check_for_lock(void)
{
  if ((mynode->userdata.online_info.class_info.priority >
      c_sys_info->lock_priority) && 
      (mynode->userdata.online_info.class_info.priority) &&
      (c_sys_info->lock_priority)) {
    printf("\nLock Priority [%d]\r\n",c_sys_info->lock_priority);
    if (mynode->userdata.user_info.enable) {
      print_file("text/locked.txt");
    } else {
      print_file("text/glocked.txt");
    }
    exit(0);
  }
}

static void sigusr1_handler(int signo)
{
  abuffer abuf;
  static char kill_str[] = "\r\n\r\n You have been Killed!\r\n\r\n";
  static char relog_str[] = "\r\n\r\n--> Relog\r\n";
  static char timeout_str[] = "\r\n\r\n Your Online Time Has Expired\r\n\r\n";
  static char unhandled_str[] = "\r\n\r\n Unhandled SIGUSR1\r\n\r\n";
  static char gotsigusr1_str[] = "\r\n\r\n Got SIGUSR1\r\n";
  static char lineout_str[] = "\r\n\r\n You have lined out, please shut up next time \r\n\r\n";
  char *selection;

#if 0  
  write(0,gotsigusr1_str,strlen(gotsigusr1_str)); 
#endif
 
  switch(mynode->sigusr1_action)
  {
    case SIG1_ACTION_TIMEOUT:
	selection = timeout_str;
        break;
    case SIG1_ACTION_RELOG:
        selection = relog_str;
        break;
    case SIG1_ACTION_KILL:
        selection = kill_str;
        break;
    case SIG1_ACTION_LINEOUT:
	selection = lineout_str;
    case SIG1_ACTION_NONE:
        log_error("SIG1_ACTION_NONE");
	return;
    default: 
        selection = unhandled_str;
	log_error("unknown SIG1_ACTION");
        break;
  }
  
  write(0,selection,strlen(selection));
  save_online_user();

  switch(mynode->sigusr1_action)
  {
    case SIG1_ACTION_TIMEOUT:
         exit(0);
    case SIG1_ACTION_RELOG:
         respawn_self(mynum, mypipe, 1);
         break;
    case SIG1_ACTION_LINEOUT:
    case SIG1_ACTION_KILL:
         exit(0);
    default:
         break;
  }
  selection = SIG1_ACTION_NONE;

}

void print_login_info(void)
{
  char spoolfile[250];
  char loginfile[250];  /* For login display file  -Gregg */
  struct stat spool_stat;
  int ansi_state = ansi_on(1);
  struct unique_information_struct *usr = &(mynode->userdata.user_info);

  sprintf(loginfile,"text/login.%s",mynode->userdata.online_info.class_info.class_name);
  print_file(loginfile);

  sprintf(spoolfile,"%s/var/spool/mail/%s","/home/gtalk/ROOT",
	  usr->login);
  if (stat(spoolfile,&spool_stat)) {
    log_error("Cannot Access Spoolfile [%s]",spoolfile);
    printf("[ No Email Information Available ]\r\n");
  } else {
    if (spool_stat.st_mtime > spool_stat.st_atime)
      printf_ansi("|*f2|*h1New Email!|*r1\n");
    else
      printf_ansi("|*f1|*h1No New Email|*r1\n");
  }

  ansi_on(ansi_state);
}

void main(int argc, char *argv[])
{
  int relog;

  if (argc < 4)
    {
      log_error("Gtalk Client Invoked Improperly");
      exit(1);
    }
  if (connect_to_shm() < 0)
    {
      log_error("Could not connect to shared memory");
      exit(1);
    }

  signal(SIGHUP, sighup_handler);
  signal(SIGSEGV, sigsegv_handler);
  signal(SIGUSR1, sigusr1_handler);

  init_abuffers();
  init_commands();

  mynum=atoi(argv[1]);
  mypipe=atoi(argv[2]);
  relog=atoi(argv[3]);

  log_error("num %d pipe %d relog %d", mynum, mypipe, relog);

  mynode = c_nodes(mynum);
  init_mynode_struct(mynode);
  mydev = c_devices(mynode->dev_no);

  init_state_list(default_child_state_list, &child_state_list);
  if (create_squelched_node_list() < 0)
    exit(1);
  
  if (!relog)
    {
      if (answer_properly() < 0)
	exit(1);
    }

  get_ansi_mode();

  print_file("text/login.txt");

  mynode->status = NODE_CONNECTED;

  if (!pass_prompt())
    {
      check_for_lock();
      mynode->status = NODE_ACTIVE;
      mynode->cur_chan[0]=0;             /* wipe our current main channel */
      strcpy(mynode->new_chan, "MAIN");

      client_abuf_writef(my_ip, SERVER_PROCESS, STATE_CHANNEL,
			 "JOIN %s %s", mynode->new_chan,
                         relog ? "Relog" : "Login");

      print_login_info();
      main_loop();
      printf("--> Logout\r\n");
      print_file("text/logout.txt");
      save_online_user();
    }
  else
    {
      printf("Login Invalid <CLICK>\r\n");
    }
  exit(0);
}




