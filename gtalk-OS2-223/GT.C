
/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


const char version_title[] = "GTalk v2.2.3";    /* version message */

#define INCL_DOSEXCEPTIONS
#define INCL_DOSFILEMGR
#define INCL_NOPMAPI
#define INCL_DOS
#define INCL_DOSPROCESS
#include <os2.h>

/* headers */
#include "include.h"
#include "gtalk.h"
#include <time.h>
#include "console.h"

#include "event.h"
#include "function.h"
#include "files.h"
#include "valid.h"
#include "newuser.h"

#include "except.h"


#define LOGIN_VALID             0
#define LOGIN_INVALID_COUNT     101
#define LOGIN_INVALID_NOCOUNT   102
#define LOGIN_BLANK             103
#define LOGIN_GUEST             103
#define LOGIN_ERROR             104
#define NUM_LOGIN_ATTEMPTS      3


#define LOGIN_TIMEOUT_SEC  20
#define APPLICATION_TIMEOUT_SEC (60*(6))


#define SCHEDULER_ON


#undef MACRO_LIMIT



void bbs_base_auto_newscan(void);
/* Acutal Gtalk System */

void main_loop(void);
void link_loop(void);
void print_login_messages(int portnum);

void print_system_login(void);
void print_login_line(void);
void g_linked(void);


// user_dat far user_lines[MAXPORTS];


#define TIMEOUT_NODES 1             /* if this many nodes are free then
                                       the system will timeout people */
#define TIMEOUT_PRIORITY 30         /* priority that will NOT timeout */
#define CLIENT_BUFFER 2048          /* size of a client's buffer */
#define CLIENT_BUFFER_1 2047        /* CLIENT_BUFFER - 1 */
#define SERVER_BUFFER 8192          /* size of server's buffer */
#define SERVER_BUFFER_1 8191
#define NUM_TRIES 3
#define LOGIN_MESSAGE_FILE       "TEXT\\LOGIN.TXT"
#define GUEST_LOGIN_MESSAGE_FILE "TEXT\\GSTLIN.TXT"
#define USER_LOGIN_MESSAGE_FILE  "TEXT\\USRLIN.TXT"
#define SYSOP_LOGIN_MESSAGE_FILE "TEXT\\SYSLIN.TXT"
#define USER_LINEOUT_WARNING     "TEXT\\LINOUT.WRN"
#define LINEOUT_MESSAGE_FILE     "TEXT\\LINOUT.TXT"
#define SHUTDOWN_MESSAGE_FILE    "TEXT\\SHUTDN.TXT"
#define SUSPENDED_FILE           "TEXT\\SUSPND.TXT"

char backspacestring[] = {8, 32, 8, 0};     /* sent with a backspace */
char welcome[] = "Welcome to GTalk!";  /* our message */
char cr_lf[] = { 13,10,0 };
char system_number[6];
int server;                     /* task_id of server */
int timeout_server;
int number_till_core;           /* number of ticks till server does */
                                /* routine maintenance */


struct system_toggles sys_toggles;
struct system_information sys_info;


/* Remember, if you change the size of line_status, you need to change the
   initialization of them to "MAX_THREADS" in pre_init_vars() in init.c
 */
struct ln_type line_status[MAX_THREADS];



struct sync_number_storage sync_status[MAX_THREADS];

struct a_line
{
  unsigned char y_loc;
  unsigned char x_loc;
  unsigned char len;
  unsigned char character;
  unsigned char attrib;
};


void increment_call_statistics(void)
{
   /* NOTE: for the system only, the user call stats are done on logout */

   sys_info.calls.total++;
   sys_info.day_calls.total++;
   sys_info.month_calls.total++;
   sys_toggles.calls_updated++;
}

int count_num_char_in_string(char test_for,char *string)
{
   int num = 0;

   while((*string) && (*string==test_for))
     {
       num++;
       string++;
     }

   return num;

}

int count_num_char_in_string_and_remove(char test_for,char *string)
{
    int num = count_num_char_in_string(test_for,string);

    while (*(string+num))
    {
     *string = *(string+num);
     string++;
    }

    *string = 0;
    return num;

}

void update_display_one(int initial_draw)
{
  char s[200];
  int loop,loop2;
  int dos_locked = islocked(DOS_SEM);
  time_t now;
  char n[120];
  char n2[120];
  if (dans_counter<sync_status[tswitch].last_screen_redraw)
    initial_draw=1;

  //direct_screen(4,30,0x07,(unsigned char *)sys_info.system_name);
  //direct_screen(4,17,0x07,(unsigned char *)"System Name: ");

  if (initial_draw)
  {
      position(3,30);
      special_code(1,tswitch);
      print_string(sys_info.system_name);
      special_code(0,tswitch);

      position(3,17);
      print_string("System Name: ");
  }

  if ((sync_status[tswitch].calls_updated!=sys_toggles.calls_updated)
      || initial_draw)
  {
          strcpy(s,"Calls");
          position(5,2);
          print_string(s);
          //direct_screen(6,2,0x07,(unsigned char *)s);

          sprintf(s,"Total: % -6lu",sys_info.calls.total);
		  position(6,8);
          print_string(s);
          //direct_screen(7,8,0x07,(unsigned char *)s);

          sprintf(s,"Today: % -4lu",sys_info.day_calls.total);
          position(7,8);
          print_string(s);
          //direct_screen(8,8,0x07,(unsigned char *)s);

          sprintf(s,"Yesterday: % -4lu",sys_info.yesterday_calls.total);
		  position(8,4);
          print_string(s);

          //direct_screen(9,4,0x07,(unsigned char *)s);

          sprintf(s,"Record: % -4lu",sys_info.record_calls.total);
          position(9,7);
          print_string(s);
          //direct_screen(10,7,0x07,(unsigned char *)s);
    }

    if (!dos_locked) lock_dos(121);
    now = time(NULL);
    if (!dos_locked) unlock_dos();

//  direct_screen(23,0,0x07,(unsigned char *)"                                                                             ");
//  direct_screen(23,0,0x07,(unsigned char *)"Uptime:");
//  sprint_expanded_time(((unsigned long int)now-(unsigned long int)sys_info.uptime),s);
//  direct_screen(23,8,0x07,(unsigned char *)s);

  /* do /s */


      for (loop=0;loop<num_ports;loop++)
       if  ((sync_status[tswitch].last_screen_redraw<sync_status[loop].handlelinechanged_at_tick) || initial_draw)
       {

        if (loop==tswitch)
        {
                position(5+loop,30);
		   /* first we need to blank the line */
                for (loop2=0;loop2<50;loop2++)
                    print_chr(' ');
            position(5+loop,50);
            special_code(1,tswitch);
            print_string("|*f1<STATUS SCREEN>");
            special_code(0,tswitch);


        }
		else
        {

            if (line_status[loop].online)
              {
                set_bit(&line_status[loop].handlelinechanged,CONSOLE_REWRITE,0);

                position(5+loop,30);

           /* first we need to blank the line */
				for (loop2=0;loop2<50;loop2++)
                    print_chr(' ');
                  // direct_screen(5+loop,40+loop2,0x07,(unsigned char *)" ");

                if (user_lines[loop].user_info.number<0)
                   {
                   sprintf(n,"% 5s %%GST %c%02d%c%c%d:%s|*r1%c",line_status[loop].baud,
                      user_options[loop].warning_prefix,
                      loop,user_options[loop].staple[0],user_options[loop].location,
                      line_status[loop].mainchannel,user_lines[loop].user_info.handle,
					  user_options[loop].staple[1]);

                   if (user_options[loop].time==0)
                   sprintf(n2,"%%GST/%03u/UNL",
                      (int)(now-line_status[loop].time_online)/60);
                   else
                   sprintf(n2,"%%GST/%03u/%03u",
                      (int)(now-line_status[loop].time_online)/60,
                      user_options[loop].time);


                  }
                else
                    if (user_options[loop].time==0)
                  {

                   sprintf(n,"% 5s #%03d %c%02d%c%c%d:%s|*r1%c",line_status[loop].baud,
                      user_lines[loop].user_info.number,user_options[loop].warning_prefix,
                      loop,user_options[loop].staple[0],user_options[loop].location,
                      line_status[loop].mainchannel,user_lines[loop].user_info.handle,
					  user_options[loop].staple[1]);

                   sprintf(n2,"#%03d/%03u/%s",user_lines[loop].user_info.number,
                   (int)(now-line_status[loop].time_online)/60,"UNL");
                  }
                else
                   {

                   sprintf(n,"% 5s #%03d %c%02d%c%c%d:%s|*r1%c",line_status[loop].baud,
                   user_lines[loop].user_info.number,user_options[loop].warning_prefix,
				   loop,user_options[loop].staple[0],user_options[loop].location,
                   line_status[loop].mainchannel,user_lines[loop].user_info.handle,
                   user_options[loop].staple[1]);

                   sprintf(n2,"#%03d/%03d/%03d",user_lines[loop].user_info.number,
                   (int)(now-line_status[loop].time_online)/60,
                   user_options[loop].time);
                   }
               n[69] = 0;
               position(5+loop,30);

               special_code(1,tswitch);
               print_string(n);
               special_code(0,tswitch);

               //direct_screen(5+loop,40,0x07,(unsigned char *)n);

           }
          else
               if (line_status[loop].connect)
				  {
                    position(5+loop,30);
                   /* first we need to blank the line */
                        for (loop2=0;loop2<50;loop2++)
                            print_chr(' ');
                    position(5+loop,50);
                    special_code(1,tswitch);
                    print_string("|*f1<Connected>");
                    special_code(0,tswitch);
                  }
				  //direct_screen(5+loop,50,0x04,(unsigned char *)"<Connected>");
               else
                  {
                    position(5+loop,30);
                   /* first we need to blank the line */
                        for (loop2=0;loop2<50;loop2++)
                            print_chr(' ');
                    position(5+loop,50);
                    special_code(1,tswitch);
                    print_string("|*f0|*h1<Idle>");
					special_code(0,tswitch);
                  }
                  //direct_screen(5+loop,50,0x04,(unsigned char *)"<Idle>");
         }  // if (loop==tswitch)
    }
  /* DONE WITH /s */
  sync_status[tswitch].last_screen_redraw = dans_counter;
  wait_for_xmit(tswitch,20);
  position(25,1);
}


void console_status(void)
{
  char temp;
  char flag = 1;
  char s[120];
  time_t now;

  line_status[tswitch].ansi = find_ansi();

  clear_screen();


  if (tswitch)
		{
		 sprintf(s,"GTalk Virtual Console [%02d] (node %02d)",
				   index_of_console(tswitch),tswitch);
		 print_str_cr(s);
		 print_str_cr("Press [RETURN] to Login");
		}

   if (!tswitch) update_display_one(1);

  while (flag)
   {
	 if (!tswitch) update_display_one(0);

	 temp = -1;
	 now = time(NULL);
	 while( ((temp=int_char(tswitch))==-1) && ((time(NULL)-now)<CONSOLE_STATUS_UPDATE) )
	 {
		next_task();
		DosSleep(500l);
	 }

     if ((temp==13) || (temp==10))
      flag = 0;
   }


}



void remove_flashing(char *str)
{
   char *newstr = str;

   while (*str)
    {
        if ((*str=='|')&&(*(str+1)=='*') && ( (*(str+2)=='p') || (*(str+2)=='P') ) && (*(str+3)!=0))
           str += 4;
		else
           *newstr++ = *str++;
    }
  *newstr = 0;

}

void filter_flashing(char *str,char *newstr)
{
   while (*str)
	{
        if ((*str=='|')&&(*(str+1)=='*') && ( (*(str+2)=='p') || (*(str+2)=='P') ) && (*(str+3)!=0))
           str += 4;
        else
           *newstr++ = *str++;
    }
  *newstr = 0;
}

void remove_ansi(char *str)
{
  char *newstr = str;

   while (*str)
    {
        if ((*str=='|') &&
           ((*(str+1)=='*') || (*(str+1)=='+'))&&
            (*(str+2)!=0) && (*(str+3)!=0))
           str+=4;
        else
		  if (*str=='^')  // filter carrots also
              str++;
        else
          *newstr++ = *str++;
    }
  *newstr = 0;

}


void filter_ansi(char *str,char *newstr)
{
   if (!newstr)
     newstr = str;

   while (*str)
    {
        if ((*str=='|') &&
             ((*(str+1)=='*') || (*(str+1)=='+')) &&
               (*(str+2)!=0) && (*(str+3)!=0))
           str += 4;
        else
          if (*str=='^')  // filter carrots also
			  str++;
        else
          *newstr++ = *str++;
    }
  *newstr=0;
}
void ansi_end_fix(char *str)
{
  char *end = str;
  int back = 0;

  while (*end) end++;
  if (end == str) return;

  while ((end > str) && (back < 3))
  {
    end--;
    back++;
    if ((*end == '|') && ((end[1] == '*') || (end[1] == '+')))
    {
	  *end = 0;
      back = 0;
    }
  }
  return;
}

void init_timeout_vars(int testid)
{
   line_status[testid].connect = 0;
   line_status[testid].online = 0;
   line_status[testid].handlelinechanged = ALL_BITS_SET;
   sync_status[testid].handlelinechanged_at_tick = dans_counter;
   line_status[testid].timeout = 1;
   user_options[testid].time_sec = 0;
   user_options[testid].warning_prefix= '#';
}

void restart_task(int testid)
{

    init_timeout_vars(testid);
    make_task((task_type) ginsu, TASK_STACK_SIZE, testid,1,"LOGIN");
}



void print_to_all_with_priv(char *str,int test_prv)
{
    int loop;

    for (loop=0;loop<=sys_info.max_nodes;loop++)
      if (test_bit(user_lines[loop].class_info.privs,test_prv))
         aput_into_buffer(loop,str,0,8,tswitch,loop,9);

}


void make_manual_user(int portnum)
{   int loop;

    user_lines[portnum].user_info.number=-1;
    user_lines[portnum].class_info.priority=40;
    user_lines[portnum].class_info.time=60;

    user_lines[portnum].class_info.staple[0]='[';
    user_lines[portnum].class_info.staple[1]=')';
    user_lines[portnum].class_info.staple[2]='[';
    user_lines[portnum].class_info.staple[3]=']';
    user_lines[portnum].user_info.width=80;
    user_lines[portnum].class_info.line_out=8;
    user_lines[portnum].user_info.num_eat_lines=10;
    user_options[portnum].width=80;
    user_options[portnum].staple[0]='[';
    user_options[portnum].staple[1]=')';
    user_options[portnum].staple[2]='[';
    user_options[portnum].staple[3]=']';
    user_options[portnum].time=60;
    user_options[portnum].priority=40;

    line_status[portnum].lurking=0;

    for (loop=0;loop<16;loop++)
      set_bit(user_lines[portnum].class_info.privs,loop,1);
    for (loop=16;loop<80;loop++)
      set_bit(user_lines[portnum].class_info.privs,loop,0);


}

void make_manual_sysop(void)
{
    int loop;

    user_lines[tswitch].user_info.number=-1;
    user_lines[tswitch].class_info.priority=0;
    user_lines[tswitch].class_info.time=0;
    user_lines[tswitch].class_info.staple[0]='{';
    user_lines[tswitch].class_info.staple[1]=')';
    user_lines[tswitch].class_info.staple[2]='{';
    user_lines[tswitch].class_info.staple[3]='}';
    user_lines[tswitch].user_info.width=80;
    user_options[tswitch].width=80;
    user_lines[tswitch].class_info.line_out=0;
    user_lines[tswitch].user_info.num_eat_lines=10;
    for (loop=0;loop<80;loop++)
      set_bit(user_lines[tswitch].class_info.privs,loop,1);

}





void broadcast_message(char *string)
 {
   aput_into_buffer(server,string,255,12,tswitch,0,0);
 };

char *limit_carrots(char *str,int max_carrots)
{
   char *temp=str;

   while (max_carrots && *temp)
    if (*(temp++)=='^') max_carrots--;

   while (*temp)
	if (*temp++=='^') *(temp-1)=' ';

  return str;
}


int has_channel(int portnum, int channel)
 {
   int check_ch;
   int max_ch;
   struct ln_type *ln_stat = &line_status[portnum];

   if ((portnum) > num_ports)
      return 1;

   if ((channel == ln_stat->mainchannel) || (channel > 249))
     return 1;

   if (ln_stat->mainchannel>249)
     return 1;

   max_ch = ln_stat->numchannels;
   for (check_ch=0;check_ch<max_ch;check_ch++)
    {
	  if (channel == ln_stat->channels[check_ch]) return 1;
    };
   return 0;
 };

void update_m_index(void);

void schedule_events(void)
{

  add_task_to_scheduler((task_type) midnight_task, NULL,
	DAILY_TASK, 0, 1, 4096*2, "MIDNIGHT");

  add_task_to_scheduler((task_type) update_members_list, (void *)0,
	DAILY_TASK, 21600, 1, 4096*2, "UPDATEMEM");

  add_task_to_scheduler((task_type) update_members_list, (void *)1,
	DAILY_TASK, 23000, 1, 4096*2, "SYSMEMBUD");

  add_task_to_scheduler((task_type) update_m_index, NULL,
	DAILY_TASK, 22400, 1, 4096*2, "MINDEXUD");

  add_task_to_scheduler((task_type) save_sys_info, NULL,
	PERIODIC_TASK, 900, 1, 4062*2, "SAVEINFO");

  add_task_to_scheduler((task_type) clear_all_old_pids_event, NULL,
	PERIODIC_TASK, 30, 1, 4096*2, "CLEARPID");

/*
 if (sys_info.checksum_task)
	   sys_toggles.perodic_checksum_task_id=
		 add_task_to_scheduler((task_type) perodic_checksum_system_event, NULL,
		  PERIODIC_TASK, 60,1,4096*2, "CHECKSUM");
		  */


}


void restart_monitor(void)
 {
   char s[10];
   int count;
   for (count=0;count<MAXPORTS;count++)
    {
      sprintf(s,"%d:%d",count,line_status[count].restart);
      direct_screen(2,count*5,0x17,(unsigned char *)s);
    };
 };

int nodes_online(void)
{
  int number=0;
  int loop;

  for (loop=1;loop<num_ports;loop++)
    if (line_status[loop].online)
        number++;

  return number;

}

void do_time_warning(int testid)
{
  char s[100];
  time_t curtime;


  lock_dos(122);
  curtime=time(NULL);
  unlock_dos();

  user_options[testid].warning_prefix='*';
  line_status[testid].handlelinechanged = ALL_BITS_SET;
  sync_status[testid].handlelinechanged_at_tick = dans_counter;

  if (line_status[testid].link)
       sprintf(s,"|*h1|*f1--> Link on Node #%02d: Timeout in %d seconds",
                 testid,user_options[testid].time_sec -
				 user_options[testid].time_warning_sec);
  else
       sprintf(s,"|*h1|*f1--> #%02d:%c|*r1%s|*r1|*h1|*f1%c Timeout in %d seconds",
                 testid,user_options[testid].staple[2],
                 user_lines[testid].user_info.handle,user_options[testid].staple[3],
                 user_options[testid].time_sec -
                 user_options[testid].time_warning_sec);

  user_options[testid].time_sec=(((long int)curtime-(long int)line_status[testid].time_online)+(long int)60);

  /* if they are not online then don't print it to them because they wont see
     it anyhow */

  if (line_status[testid].online)
    aput_into_buffer(server,s,line_status[testid].mainchannel,8,tswitch,testid,10);

  user_options[testid].warnings = 1;

}

void do_time_out(int testid)
{
  if (line_status[testid].online)
    {
       pause(testid);
       print_cr_to(testid);
       print_str_cr_to("--> Your online time has expired.",testid);
       print_cr_to(testid);
       delay(2);
     }
   log_off(testid,1);

}

void start_timeout_server(void)
{
  int testid;
  time_t dif;
  time_t core_counter;
  int operation;
  int full_system;
  time_t curtime;
  sys_toggles.timeout_flag=0;

  while (1)
	{
	  core_counter=time(NULL);

	  while ((time(NULL)-core_counter)<2)
		{ next_task();                 /* sleep */
		  DosSleep(100l);
		}


	  /* DONE SLEEPING */


		  curtime=time(NULL);

	  for (testid=0;testid<MAXPORTS;testid++)
	   {


		 if (system_nodes_free()<=(TIMEOUT_NODES))
			full_system=1;
		 else
			full_system=0;

		  dif=(time_t)((unsigned long int)((unsigned long int)curtime -
						 (unsigned long int)line_status[testid].time_online));

		  operation=0;

		  if ((dif>(user_options[testid].time_warning_sec)) &&
			  (!user_options[testid].warnings))
				 operation=1;
		  else
		  if (dif>((user_options[testid].time_sec)))
				 operation=2;

		  if (!line_status[testid].connect)
			  operation=0;
		  if (!user_options[testid].time_sec)
			  operation=0;
		  if (!line_status[testid].should_timeout)
			  operation=0;


		  switch (get_taskchar(testid)) /* type of task? */
			{                           /* eval correct operation */

			  case 0  : operation=0;
						break;   /* the server, it should NOT
									be found here... but hey */
			  case 1  :
              case 2  :
                        if (operation==1) /* ONLY if they have not got a warning */
                         if (((!full_system) && test_bit(user_options[testid].privs,TIMEOUT_ONLY_WHEN_FULL_PRV) &&
                                 line_status[testid].online ))/* Gtalk, the main task */
							operation=0;
						break;

			  case 3  : break;         /* LINKED , a link task */

			  case 4  : operation=0;
						break;         /* shutdown task */

			  case 5  : operation=0;
						break;         /* termainal dummy task */

			  case 6  : operation=0;
						break;         /* scheduled task */
			  default : operation=0;
						break;

			}


		  switch (operation)     /* DO the OPERATION */
			{
			  case 0  : break;                       /* do nothing */
			  case 1  : do_time_warning(testid);     /* print timeout */
						break;
			  case 2  : do_time_out(testid);         /* timeout */
						break;
			}

		  next_task(); /* don't lag the system */
		}

	}
  end_task();
}

void time_monitor(void)
{
	int flag=!islocked(DOS_SEM);
    char n[180];
    struct tm *temp;
    time_t now;

    if (flag) lock_dos(124);
	now=time(NULL);
    temp=localtime(&now);
    str_time(n,79,temp);
    if (flag) unlock_dos();

    direct_screen(0,67,0x17,(unsigned char *)n);

    // sprint_expanded_time(now-sys_info.uptime,n);
    // direct_screen(3,0,0x17,(unsigned char *)n);

	//for (loop=strlen(n);loop<80;loop++)
    //   direct_screen(3,loop,0x17,(unsigned char *)" ");

}

/* this function will CHECK to see what the current day
   is relative to the day stored in sys_info.current_time
   IF they are the same, everything is fine.
   IF NOT, then we need to update current_time, and
   calls_yesterday, and record_calls appropriately */

void date_sync(void)
{

}

void start_pipe_server(void)
{


   end_task();
}

void start_server(void)
 {
   char s[STRING_SIZE+400];
   int sentby, channel;
   int testid;
   int type;
   int temp1;
   int temp2,temp3;
   /*
   PTIB ptib; /* thread info block */
   PPIB ppib; /* process info block */
   DosGetInfoBlocks(&ptib,&ppib);
   DosSetPriority(PRTYS_THREAD,PRTYC_FOREGROUNDSERVER,PRTYD_MAXIMUM,ptib->tib_ptib2->tib2_ultid);
     */

   sys_toggles.system_update=0;

   /* since the program is just starting, we should see if
      the current time in sys_info.current_time is yesterday or today,
      if it was yesterday (or even later, then we better update
	  the number of calls and stuff BEFORE we let the scheduler run
      (which would reset the sys_info.current_time */

   date_sync();

   /* INIT SERVER FUNCTIONS */

	  sys_toggles.should_reboot=1;

	  number_till_core = time(NULL);

	  initabuffer(SERVER_BUFFER);
	  init_rotator_bit_array();
#ifdef SCHEDULER_ON
	  schedule_events();
#endif

   /* MAKE SURE SYS INFO IS SAVED RIGHT AWAY */

	add_task_to_scheduler((task_type) save_sys_info, NULL,
	   REL_SHOT_TASK, 0, 1, 1024, "SAVEINFO");

   /* START SERVER LOOP */

    sys_toggles.system_booting = 0;
    log_error("*GTALK: Server Boot.....");

   for (;;)
	{

	  /* FIRST, circulate messages */

	  if (aget_abuffer(&sentby, &channel, s, &type, &temp1, &temp2,&temp3))
	  {
	   s[420]=0;
	   switch (type)
		{
		  case 3 :  for (testid=0;testid<MAX_THREADS;testid++)
					 if (testid!=tswitch)
                        aput_into_buffer_owner(testid,s,sentby,channel,type,temp1,temp2,temp3);
					break;

		  case 12:  for (testid=0;testid<MAX_THREADS;testid++)
					 if (testid!=tswitch)
                        aput_into_buffer_owner(testid,s,sentby,channel,type,temp1,temp2,temp3);
					break;

		  default:  for (testid=0;testid<MAX_THREADS;testid++)
					 if (testid!=tswitch)
					  if (has_channel(testid,channel))
                         aput_into_buffer_owner(testid,s,sentby,channel,type,temp1,temp2,temp3);
					break;

		} /* END FIRST */
	   }
	   else
		task_sleep_timeout(500l);

	   next_task();
					 /* SECOND, if it's time, update the display
						and remake tasks */

	   if ((time(NULL)-number_till_core)>=1)
		{

		  number_till_core = time(NULL);

          /* put status bar stuff here */

           time_monitor();
		  if (!max_task_switches)
            direct_screen(0,63,0x17,(unsigned char *)"100%");
          else
          {
             sprintf(s,"%02lu%%",
                   (((100l)*(unsigned long int)(max_task_switches-system_load))/
                   (unsigned long int)max_task_switches));
             direct_screen(0,64,0x17,(unsigned char *)s);
          }

		  //lock_dos(125);
          //sprintf(s,"Core Left: %ld  ",farcoreleft());
          //unlock_dos();
          //direct_screen(0,0,0x17,(unsigned char *)s);


          /* ----- */
#ifdef SCHEDULER_ON
          //curtime =
          (void)see_if_scheduled_event_occurs();
#endif

          for (testid=0;testid<=sys_info.max_nodes;testid++)
                if (iskilled(testid) && port[testid].active)
                        restart_task(testid);


        } /* end SECOND */

    } /* THIS LOOP NEVER EXITS */

    end_task();  /* THIS SHOULD NEVER EXECUTE */

 };

void shutdown_timeout_server(void)
{
 kill_task(timeout_server);
}

void shutdown_server(void)
 {
   kill_task(server);
   if (abuf_status[server].abuffer)
      g_free(abuf_status[server].abuffer);
 };


void clear_call_on_logoff()
 {
 };

void unlog_user(int portnum)
 {
   int was_online = line_status[portnum].online;


   if (was_online) log_out_user(portnum);
   if (was_online) print_log_off(portnum);

   line_status[portnum].online=0;

   line_status[portnum].handlelinechanged = ALL_BITS_SET;
   sync_status[portnum].handlelinechanged_at_tick = dans_counter;

   clear_channel_semaphores(portnum);
   wait_for_death(portnum);
   dealloc_abuf(portnum);
   delay(2);

}

void log_out_user(portnum)
{
  time_t now;
  unsigned int online_time;
  int flag=!islocked(DOS_SEM);


   if (flag) lock_dos(126);
   if ((user_lines[portnum].user_info.number)>=0)
       {
         now=time(NULL);

         user_lines[portnum].user_info.last_call = now;
         user_lines[portnum].user_info.stats.calls_total++;

         online_time=(unsigned int)(now-line_status[portnum].time_online);

         user_lines[portnum].user_info.stats.time_total+=online_time;

         save_user(user_lines[portnum].user_info.number,&user_lines[portnum]);

       }
    if (flag) unlock_dos();

    if (!line_status[portnum].lurking)
       log_user_is_leaving(portnum,USER_LOG_FILE);
}

void hangup_user(int portnum)
{

	hang_up(portnum);

	line_status[portnum].connect = 0;
	sync_status[portnum].handlelinechanged_at_tick = dans_counter;

	if (!wait_for_modem_result_portnum("CARRIER",30,portnum))
	   {
		 sendslow(portnum,"AT");
		 sendslow(portnum,cr_lf);
		 if (!wait_for_modem_result_portnum("K",25,portnum))
		 {
		   sendslow(portnum,"+++");
		   wait_for_modem_result_portnum("K",40,portnum);
		   sendslow(portnum,"ATH");
		   sendslow(portnum,cr_lf);
		   wait_for_modem_result_portnum("K",40,portnum);
		 }
	   }
#ifdef NEW_LOGOFF_STUFF
	   else
		   change_dtr_state(portnum,0);
#endif
}

void set_death_off(void)
 {
   line_status[tswitch].timeout = 0;
 };

void set_death_on(void)
 {
   line_status[tswitch].timeout = 1;
 };



void log_off(int portnum,int should_unlog)
 {
   int count=0;

   if (portnum!=tswitch)
            pause(portnum);

   if (should_unlog)
			unlog_user(portnum);


   if (!is_console_node(portnum))
	{

	   set_death_off();

		 /* FIRST, we need to hang up the user */

	   if (wait_for_dcd_state(portnum,1))
		  hangup_user(portnum);

	   if (!port_fast[portnum]->no_dcd_detect)
		 {

		   delay(2);

		   count=0;
		   do
			 {
			   sendslow(portnum,"AT");
			   sendslow(portnum,cr_lf);
			 }
		   while ((!wait_for_modem_result_portnum("K",50,portnum)) && (count++<4));

			count=0;
			do
			   {
				sendslow(portnum,port_fast[portnum]->de_init_string);
				sendslow(portnum,cr_lf);
			   }
			while ((!wait_for_modem_result_portnum("K",50,portnum)) && (count++<4));

		  }

	 }
	set_death_on();
	 kill_task(portnum);

}


void re_log(int portnum)
 {
   int flag=!islocked(DOS_SEM);

   clear_channel_semaphores(portnum);

   if (line_status[portnum].online)
     print_log_off(portnum);

   user_options[portnum].warning_prefix='+';
   log_out_user(portnum);
   dealloc_abuf(portnum);

   wait_for_death(portnum);
   if (flag) lock_dos(127);
     kill_task(portnum);
     make_task((task_type) relogged, TASK_STACK_SIZE, portnum,2,"R-LOGIN");
   if (flag) unlock_dos();

 }

void remote(int portnum)
 {

   clear_channel_semaphores(portnum);

   if (line_status[portnum].online)
     print_log_off(portnum);

   user_options[portnum].warning_prefix='+';
   log_out_user(portnum);
   dealloc_abuf(portnum);

   wait_for_death(portnum);
   lock_dos(128);
   kill_task(portnum);
   make_task((task_type) linked, TASK_STACK_SIZE, portnum,3,"LINKED");
   unlock_dos();

 }

void g_remote(int portnum)
 {
   clear_channel_semaphores(portnum);

   if (line_status[portnum].online)
     print_log_off(portnum);

   user_options[portnum].warning_prefix='+';
   log_out_user(portnum);
   dealloc_abuf(portnum);

   wait_for_death(portnum);
   lock_dos(128);
   kill_task(portnum);
   make_task((task_type) g_linked, TASK_STACK_SIZE, portnum,3,"LINKED");
   unlock_dos();

 }



void leave(void)
 {
   empty_outbuffer(tswitch);
   log_off(tswitch,1);
 };

void print_call_stats(void)
{
//  unsigned long int online_time;
//  unsigned long int temp=864*100;
  char s[100];

   increment_call_statistics();
   sprintf(s,"You are caller #%06lu (%04lu today)",sys_info.calls.total,sys_info.day_calls.total);
   print_str_cr(s);
   print_cr();


   if (!user_lines[tswitch].user_info.stats.calls_total || user_lines[tswitch].user_info.number<0)
      return;
   if (user_lines[tswitch].user_info.stats.calls_total==1)
      print_str_cr("You have called once. ");
   else
    {
     sprintf(s,"You have called %d times. ",user_lines[tswitch].user_info.stats.calls_total);
     print_str_cr(s);
    }

}



void construct_log_in_message(int portnum,char *s)
{
    int flag=!islocked(DOS_SEM);
    struct tm *now;
    char light='a';
    int hour;

    if (flag) lock_dos(143);

    now=localtime(&user_options[portnum].login_time);

    hour=now->tm_hour;

    if (hour>=12)
      {
        light='p';
        if (hour>12)
          hour=hour-12;
      }

    if (!hour) hour=12;

    if (line_status[portnum].link)
    {

        sprintf(s,"%c|*f7|*h1--> Node #%02d:(T%d) Connect/%s at %d:%02d:%02d %cm%c%c--> (Link):%s",
            7,portnum,line_status[portnum].mainchannel, line_status[portnum].baud,
            hour,now->tm_min,now->tm_sec,light,13,10,
            user_lines[portnum].user_info.handle);


    }
    else

    if (user_lines[portnum].user_info.number<0)
    {

        sprintf(s,"%c|*f7|*h1--> Node #%02d:(T%d) Connect/%s at %d:%02d:%02d %cm%c%c--> (Guest):|*r1%s",
            7,portnum,line_status[portnum].mainchannel, line_status[portnum].baud,
            hour,now->tm_min,now->tm_sec,light,13,10,
            user_lines[portnum].user_info.handle);
    }
    else
    {

        sprintf(s,"%c|*f7|*h1--> Node #%02d:(T%d) Connect/%s  at %d:%02d:%02d %cm%c%c--> #%03d:%c|*r1%s|*r1|*f7|*h1%c",
            7,portnum,line_status[portnum].mainchannel,line_status[portnum].baud,
            hour,now->tm_min,now->tm_sec,light,13,10,
            user_lines[portnum].user_info.number,
            user_lines[portnum].class_info.staple[2],user_lines[portnum].user_info.handle,user_lines[portnum].class_info.staple[3]);
    }                                                                                                               
    if (flag) unlock_dos();
}

void construct_log_out_message(int portnum,char *s)
{

    int flag=!islocked(DOS_SEM);
    struct tm *tempnow;
    struct tm now;
    char light='a';
    int hour;
    char reason_flag=user_options[portnum].warning_prefix;
    time_t temp;
    char reason[15];

    switch (reason_flag)
    {

    case '%': strcpy(reason,"Lineout");
              break;
    case '*': strcpy(reason,"Timeout");
              break;
    case '-': strcpy(reason,"Killed");
              break;
    case '+': strcpy(reason,"Relog");
              break;
    case '|': strcpy(reason,"Linked");
              break;
    default : strcpy(reason,"Logout");
              break;
    }

    if (flag) lock_dos(145);
    temp=time(NULL);
    tempnow=localtime(&temp);
    now=*tempnow;
    if (flag) unlock_dos();
    hour=now.tm_hour;

    if (hour>=12)
      {
        light='p';
        if (hour>12)
          hour=hour-12;

      }
    if (!hour) hour=12;

    if (line_status[portnum].link)
    {
        sprintf(s,"|*f1--> Node #%02d: %s at %d:%02d:%02d %cm%c%c--> (Link):|*r1%s",
            portnum,reason,
            hour,now.tm_min,now.tm_sec,light,13,10,
            user_lines[portnum].user_info.handle);
    }
    else
    if (user_lines[portnum].user_info.number<0)
    {
        sprintf(s,"|*f1--> Node #%02d: %s at %d:%02d:%02d %cm%c%c--> (Guest):|*r1%s",
            portnum,reason,
            hour,now.tm_min,now.tm_sec,light,13,10,
            user_lines[portnum].user_info.handle);
    }
	else
    {
        sprintf(s,"|*f1--> Node #%02d: %s at %d:%02d:%02d %cm%c%c--> #%03d:%c|*r1%s|*r1|*f1%c",
            portnum,reason,
            hour,now.tm_min,now.tm_sec,light,13,10,user_lines[portnum].user_info.number,
            user_lines[portnum].class_info.staple[2],
            user_lines[portnum].user_info.handle,user_lines[portnum].class_info.staple[3]);
    }


}

void construct_ddial_log_in_message(int portnum,char *str)
{
    char *begin=str;
	char who[15];
    strcpy(str,"}}");

    if ((user_lines[portnum].user_info.number<0) && (!line_status[portnum].link))
      begin=str+=1;
    else
      begin=str+=2;


    if ((user_lines[portnum].user_info.number<0))
      {if (line_status[portnum].link)
        strcpy(who,"Link");
      else
        strcpy(who,"Guest");
      }
	else
      sprintf(who,"#%03d",user_lines[portnum].user_info.number);



    sprintf(begin,"|*f7|*h1%c--> Login GT#%02d (%s):#%02d%c%c%d:|*r1%s|*r1|*f7|*h1%c at (%s)",
            7,sys_info.system_number,who,portnum,user_options[portnum].staple[0],
            user_options[portnum].location,line_status[portnum].mainchannel,
            user_lines[portnum].user_info.handle,user_options[portnum].staple[1], line_status[portnum].baud);



}

void construct_ddial_log_out_message(int portnum,char *str)
{
    char reason[15];
    char *begin=str;
    char reason_flag=user_options[portnum].warning_prefix;
    char who[15];

    switch (reason_flag)
    {

    case '%': strcpy(reason,"Lineout");
              break;
    case '*': strcpy(reason,"Timeout");
              break;
    case '-': strcpy(reason,"Killed");
              break;
	case '+': strcpy(reason,"Relog");
              break;
    case '|': strcpy(reason,"Linked");
              break;
    default : strcpy(reason,"Logout");
              break;
    }

    if (user_lines[portnum].user_info.number<0)
      {if (line_status[portnum].link)
        strcpy(who,"Link");
      else
        strcpy(who,"Guest");
      }
    else
	  sprintf(who,"#%03d",user_lines[portnum].user_info.number);


    strcpy(str,"}}");

    if ((user_lines[portnum].user_info.number<0) && (!line_status[portnum].link))
      begin=str+1;
    else
      begin=str+2;


    sprintf(begin,"|*f1%c--> %s GT#%02d (%s):#%02d%c%c%d:|*r1%s|*r1|*f1%c",
            7,reason,sys_info.system_number,who,portnum,user_options[portnum].staple[0],
            user_options[portnum].location,line_status[portnum].mainchannel,
            user_lines[portnum].user_info.handle,user_options[portnum].staple[1]);

}

void print_log_in(int portnum)
{
    char s[256];


    if (line_status[portnum].lurking)
      {
       sprintf(s,"|*f4|*h1--> Node [%02d] #%03d:%c%s|*r1|*f4|*h1%c Login/LURK",portnum,user_lines[portnum].user_info.number,
               user_options[portnum].staple[2],user_lines[portnum].user_info.handle,
               user_options[portnum].staple[3]);
      print_to_all_with_priv(s,LURK_PRV);
      return;
	  }
    construct_log_in_message(portnum,&s);

    aput_into_buffer(server,s,line_status[portnum].mainchannel,3,line_status[portnum].mainchannel,tswitch,1);

    construct_ddial_log_in_message(portnum,&s);
    aput_into_buffer(server,s,line_status[portnum].mainchannel,3,line_status[portnum].mainchannel,tswitch,10);

}

void show_log_in(int portnum)
{  char s[256];
   if (line_status[portnum].lurking) return;

   construct_log_in_message(portnum,&s);
	special_code(1,tswitch);
    print_str_cr(s);
    special_code(0,tswitch);
}

void show_log_off(int portnum)
{  char s[256];
   if (line_status[portnum].lurking) return;

    construct_log_out_message(portnum,&s);
    special_code(1,tswitch);
    print_str_cr(s);
    special_code(0,tswitch);
}


void print_log_off(int portnum)
{

    char s[256];

    if (line_status[portnum].lurking)
      {
       sprintf(s,"|*f4|*h1--> Node [%02d] #%03d:%c%s|*r1|*f4|*h1%c Logout/LURK",portnum,user_lines[portnum].user_info.number,
               user_options[portnum].staple[2],user_lines[portnum].user_info.handle,
               user_options[portnum].staple[3]);
      print_to_all_with_priv(s,LURK_PRV);
      return;
      }

	construct_log_out_message(portnum,s);
    aput_into_buffer(server,s,line_status[portnum].mainchannel,3,line_status[portnum].mainchannel,portnum,2);

    construct_ddial_log_out_message(portnum,&s);
    aput_into_buffer(server,s,line_status[portnum].mainchannel,3,line_status[portnum].mainchannel,portnum,11);

}





void init_vars(void)
{
   int loop;
   struct ln_type *fast_ls=&line_status[tswitch];

   fast_ls->restart=1;

   fast_ls->online = 0;
   fast_ls->handlelinechanged = ALL_BITS_SET;
   sync_status[tswitch].handlelinechanged_at_tick = dans_counter;
   fast_ls->timeout = 1;
   fast_ls->should_timeout=0;
   fast_ls->call_info=0;
                                    /* set to NO USER */
   user_lines[tswitch].user_info.number = -1;
   user_options[tswitch].time_sec = 0;
   user_options[tswitch].warning_prefix= '#';

   abuf_status[tswitch].active = 0;
   abuf_status[tswitch].abuffer = 0;
   fast_ls->numchannels = 0;
   fast_ls->mainchannel = 1;
   fast_ls->width = 80;
   fast_ls->watcher=-1;
   fast_ls->link=0;
   fast_ls->silenced = 0;
   fast_ls->lurking=0;
   fast_ls->chat_with=-1;
   fast_ls->wants_to_chat=0;
   fast_ls->ready_to_chat=0;
   fast_ls->ansi=0;
   fast_ls->full_screen_ansi=0;
   fast_ls->safe_lurking=1;
   fast_ls->slowdown_value=0;
   fast_ls->vtkey.num_keycodes = 0;
   for(loop=0;loop<MAX_CHANNEL_ITEMS;loop++)
    fast_ls->line_lock[loop] = -1;

   for(loop=0;loop<MAX_NUM_PRIV_CHARS;loop++)
      user_options[tswitch].system[loop]=0;

   fast_ls->ansi = 0;
   user_lines[tswitch].user_info.reset_color=0;
   clear_call_on_logoff();
}

void get_result_code(char *string, int length)
 {
   int flag = 1;
   char ch;
   while (length && flag)
    {
      length--;
      ch = wait_ch();
      if (ch == 13) flag = 0;
        else *string++ = ch;
    };
   *string=0;
 };

void delay(unsigned int ticks)
 {
   unsigned int cur = (ticks*100)/18;

   DosSleep(cur);

   next_task();
 };

void sendslow(int port_num, char *string)
 {
   while (*string)
    {
	  DosSleep(0);
	  send_char(port_num,*string++);
	};
	wait_for_xmit(port_num,10);
 };



void set_temp_user_info(int portnum)
{
     int loop;

     for (loop=0;loop<MAX_NUM_PRIV_CHARS;loop++)
      {
        user_options[portnum].privs[loop]=user_lines[portnum].class_info.privs[loop];
      }

     for (loop=0;loop<10;loop++)
        user_options[portnum].toggles[loop]=user_lines[portnum].user_info.toggles[loop];

     user_options[portnum].priority= user_lines[portnum].class_info.priority;
     user_options[portnum].width   = user_lines[portnum].user_info.width;

     for (loop=0;loop<4;loop++)
	   user_options[portnum].staple[loop]=user_lines[portnum].class_info.staple[loop];

    // if (test_bit(user_lines[portnum].user_info.toggles,ANSIOFF_TOG))
    //      line_status[portnum].ansi=0;

}


void calc_time_at_login(int portnum)
{
   user_options[portnum].time_sec=
       (unsigned long int)(user_options[portnum].time)*60;
   user_options[portnum].time_warning_sec =
        user_options[portnum].time_sec-60;
   user_options[portnum].warnings=0;
   user_options[portnum].warning_prefix='#';
}

void calc_time_for_node(int portnum)
{  time_t curtime;
   long int dif;
   int testid=portnum;
   int full_system=0;
   char s[80];


   user_options[portnum].time_sec=
       (unsigned long int)(user_options[portnum].time)*60;
   user_options[portnum].time_warning_sec = user_options[portnum].time_sec-60;


   if (nodes_online()>=(TIMEOUT_NODES)) full_system=1;
       else full_system=0;


   if (user_options[portnum].time && full_system)
   {
    lock_dos(146);
    curtime=time(NULL);
    unlock_dos();

              dif=(int)((long int)((long int)curtime-(long int)line_status[testid].time_online));
              if ((dif>(user_options[testid].time_warning_sec)) && (!user_options[testid].warnings))
                {
                  user_options[testid].warning_prefix='*';
                  line_status[testid].handlelinechanged = ALL_BITS_SET;
				  sync_status[testid].handlelinechanged_at_tick = dans_counter;
                  sprintf(s,"|*h1|*f1--> #%02d:%c|*r1%s|*r1|*h1|*f1%c Timeout in %d seconds",testid,user_options[testid].staple[2],user_lines[testid].user_info.handle,user_options[testid].staple[3],user_options[testid].time_sec-user_options[testid].time_warning_sec);
                  aput_into_buffer(server,s,line_status[testid].mainchannel,8,tswitch,testid,10);
               /* for (putid=0;putid<num_ports;putid++)
                    aput_into_buffer(putid,s,0,0,0,0); <<-- fix numbers */
                  user_options[testid].warnings = 1;
                }
              else
               {
                 if (user_options[portnum].warnings)
                  { user_options[portnum].warnings=0;
                    user_options[portnum].warning_prefix='#';
                    line_status[portnum].handlelinechanged = ALL_BITS_SET;
                    sync_status[portnum].handlelinechanged_at_tick = dans_counter;
                    sprintf(s,"|*h1--> Node #%02d: Time added",testid);
					aput_into_buffer(server,s,line_status[testid].mainchannel,8,tswitch,testid,11);
               /*   for (putid=0;putid<num_ports;putid++)
                      aput_into_buffer(putid,s,0,0,0,0); <<-- fix numbers*/
                   }
               }
   }
   else {
          if (user_options[portnum].warnings)
             {
               sprintf(s,"|*h1--> Node #%02d: Time added",testid);
               aput_into_buffer(server,s,line_status[testid].mainchannel,8,tswitch,testid,11);
             }

          user_options[portnum].warnings=0;
          user_options[portnum].warning_prefix='#';
		  line_status[portnum].handlelinechanged = ALL_BITS_SET;
          sync_status[portnum].handlelinechanged_at_tick = dans_counter;
        }
}

void init_login_vars(int portnum)
{
   int loop;
   struct ln_type *line_stat = &line_status[portnum];

   lock_dos(147);
   user_options[portnum].login_time=time(NULL);
   unlock_dos();

   user_options[portnum].squelch_all=0;
   user_options[portnum].neutralize_all=0;
   for(loop=0;loop<MAXPORTS;loop++)
   {
    user_options[portnum].squelched[loop]=-1;
    user_options[portnum].neutralized[loop]=-1;
   }

   if ((user_lines[portnum].user_info.width<20) || (user_lines[portnum].user_info.width>200))
     user_lines[portnum].user_info.width=80;

   /* NEED TO CHECK FOR LAST CALL, if last call was
      not TODAY, then reset "day" stats , if
      last call was not this month then reset MONTH stats */


   user_options[portnum].warnings=0;



   user_options[portnum].class_name[0]=0;

   if (user_lines[portnum].class_info.login_channel)
     line_stat->mainchannel=user_lines[portnum].class_info.login_channel;
   else
     line_stat->mainchannel=1;

   line_stat->numchannels=0;
   line_stat->handlelinechanged = ALL_BITS_SET;
   sync_status[portnum].handlelinechanged_at_tick = dans_counter;
   line_stat->silenced=0;
   line_stat->watcher=-1;
   line_stat->chat_with=-1;
   line_stat->ready_to_chat=0;
   line_stat->wants_to_chat=0;
   line_stat->safe_lurking=1;
   line_stat->paging=1;
   line_stat->feedbacks=0;
   line_stat->apps=0;
   line_stat->num_feedbacks_allowed=2;
   user_options[portnum].time=user_lines[portnum].class_info.time;
   user_options[portnum].location='T';
   if (test_bit(user_lines[portnum].class_info.privs,SYSMON_PRV) &&
       test_bit(user_lines[portnum].user_info.toggles,SYSMON_TOG))
     {
     add_channel(portnum,0);
    }

   line_status[portnum].timeout=1;
   line_status[portnum].should_timeout=0;

   lock_dos(148);
   time(&line_status[portnum].time_online);
   calc_time_at_login(portnum);
   line_status[portnum].should_timeout=1;
   unlock_dos();
   next_task();

   /* finally set them to be ONLINE */

  // line_status[portnum].online=1;
}


void print_login_messages(int portnum)
{
   char handletemp[45];

         /* print the right LOGIN file */

   if (user_lines[tswitch].user_info.number<0)
	  print_file_to_cntrl(GUEST_LOGIN_MESSAGE_FILE,tswitch,1,0,0,0);
   else
   if (user_lines[tswitch].class_info.priority)
      print_file_to_cntrl(USER_LOGIN_MESSAGE_FILE,tswitch,1,0,0,0);
   else
      print_file_to_cntrl(SYSOP_LOGIN_MESSAGE_FILE,tswitch,1,0,0,0);



   print_call_stats();

   if (test_bit(user_lines[portnum].class_info.privs,SYSMON_PRV) &&
       test_bit(user_lines[portnum].user_info.toggles,SYSMON_TOG))
     {
     print_str_cr("Monitoring System Channel.");
    }
   else if (test_bit(user_lines[portnum].class_info.privs,SYSMON_PRV))
       { print_str_cr("NOT Monitoring System Channel.");}

   if (test_bit(user_lines[portnum].user_info.toggles,LINECOLORS_TOG))
     print_str_cr("Line Colors ON (not implemented).");

   is_new_mail();

   /* NOW FIX HANDLE IF IT'S FULL OF ANSI, and it shouldn't be */

   if (!test_bit(user_options[tswitch].privs,ANSI_HANDLE_PRV))
     {
        filter_ansi(user_lines[tswitch].user_info.handle,handletemp);
        strcpy(user_lines[tswitch].user_info.handle,handletemp);
     }

  filter_ansi(user_lines[tswitch].user_info.handle,user_options[tswitch].noansi_handle);

}

void warn_user_lineout(void)
{
   print_file_to_cntrl(USER_LINEOUT_WARNING,tswitch,1,0,0,0);
}

void line_out_user(void)
{
   user_options[tswitch].warning_prefix='%';
   print_file_to_cntrl(LINEOUT_MESSAGE_FILE,tswitch,1,0,0,0);
   leave_quietly("","",tswitch);
}

int remake_handleline(void)
{
       if (test_bit(&line_status[tswitch].handlelinechanged,HANDLELINE_SPRINTF))
        {
          set_bit(&line_status[tswitch].handlelinechanged,HANDLELINE_SPRINTF,0);

         switch (sys_info.message_mode)
         {
           default:
           case 0:
              sprintf(line_status[tswitch].handleline,
                  "%c%02d%c%c%d:%s|*r1%c ",user_options[tswitch].warning_prefix,
                  tswitch,user_options[tswitch].staple[0], user_options[tswitch].location,
                  line_status[tswitch].mainchannel,
                  user_lines[tswitch].user_info.handle,user_options[tswitch].staple[1]);
              break;
           case 1:
              sprintf(line_status[tswitch].handleline,
                  "%c%02d%c%s|*r1%c ",user_options[tswitch].warning_prefix,
                  tswitch,user_options[tswitch].staple[0],
                  user_lines[tswitch].user_info.handle,user_options[tswitch].staple[1]);
              break;
           case 2:
              sprintf(line_status[tswitch].handleline,
                  "%c%02d:%c%s|*r1%c ",user_options[tswitch].warning_prefix,
                  tswitch,user_options[tswitch].staple[0],
                  user_lines[tswitch].user_info.handle,user_options[tswitch].staple[1]);
              break;
         }

         return 1;
      // BOTH of these done outside now
      //   strcpy(s,line_status[tswitch].handleline);
     //    datas=s+strlen(s);
        }
      /* else
       if (line_status[tswitch].super_handle)
       {
          strcpy(s,line_status[tswitch].handleline);
          datas=s+strlen(s);
       }*/
       return 0;
}

void remove_priority_messages(char *datas);

void remove_priority_messages(char *datas)
{
    char *temp=(datas-1);

    while (*(++temp))
      if (*temp=='|')
       if (*(temp+1)=='\\')
         if  (*(temp+2)!=0)
           if   (*(temp+3)!=0)
        {
         *(temp+2)='0';
         *(temp+3)='0';
        }

}

void check_for_chat_confirmation(void)
{

      if (line_status[tswitch].chat_with!=-1)
       if (line_status[tswitch].wants_to_chat)
        { int chat_with=line_status[tswitch].chat_with;
          int new_char;
          int is_a_char=0;
          char i[STRING_SIZE];

          unsigned long int counter;

            /* ask a last confirmation message */
         sprintf(i,"%c--> #%02d:%c%s|*r1%c is ready to chat.%c",7,chat_with,
                 user_options[chat_with].staple[2],user_lines[chat_with].user_info.handle,
                 user_options[chat_with].staple[3],7);
         special_code(1,tswitch);
         print_str_cr(i);
         special_code(0,tswitch);
         print_string("--> Enter chat mode now (Y/N/A): ");
		 counter=time(NULL);

		 while(((time(NULL)-counter)<10) && !is_a_char && line_status[tswitch].wants_to_chat)
		  {
			task_sleep_timeout(500l);
            in_char(tswitch,&new_char,&is_a_char);
          }
         if (is_a_char)
         {
		  print_chr(new_char);
          print_cr();

		  if ((new_char=='Y') || (new_char=='y'))
			{
			  counter=time(NULL);
			  line_status[tswitch].ready_to_chat=1;
			  while ((time(NULL)-counter)<5 && line_status[tswitch].wants_to_chat)
			  {
				task_sleep_timeout(500l); /* let him chat with us */
			  }
			}

		  else /* ANOTHER ANSWER.. BUMMER */
		  if ((new_char=='A') || (new_char=='a'))
		   { /* abort, so remove page invite */
			line_status[tswitch].chat_with=-1;
			}
		  else
		  if((new_char=='N') || (new_char=='n'))
		   {
			line_status[tswitch].wants_to_chat=0;
		   }

		} /* timeout, guess their not at the keyboard */
		else
		print_cr();

		line_status[tswitch].wants_to_chat=0;
	   } // done with this node to node crap

}


void main_loop(void)
{
   char r[STRING_SIZE+100];
   char i[STRING_SIZE];
   char t[STRING_SIZE];
   char temp[40];
   char *s=r+100;
   int lenhl;
   char *begins;

   char *datas;
   char *endbufs=(r+STRING_SIZE+100);
   int channel,sentby;
//   int new_key;
   int type,adjlen;
   int eventHappened=0;
#ifdef MACRO_LIMIT
   unsigned int before,after;
   unsigned int duration;
   unsigned int speed;
#endif
   int data1,data2,data3;
   struct line_outs *lo=&line_status[tswitch].lo;

   line_status[tswitch].online=1;
   line_status[tswitch].handlelinechanged = ALL_BITS_SET;
   sync_status[tswitch].handlelinechanged_at_tick = dans_counter;

   lo->warnings_left=2;
   lo->lines_typed=0;
   if (user_lines[tswitch].class_info.priority>41)
	   lo->lines_allowed=5;
   else
	   lo->lines_allowed=9;


   if (user_lines[tswitch].class_info.priority)
	  {

		adjlen=200;
	  }
   else
	 {lo->lines_allowed=0;
	  adjlen=100;
	 }

      if (remake_handleline())
		  {
			lenhl=strlen(line_status[tswitch].handleline);

		   strncpy(s-lenhl,line_status[tswitch].handleline,lenhl);
		   datas=s;
		   begins=s-lenhl;
		  }

   bbs_base_auto_newscan();


   for (;;)
	{

	  if (remake_handleline())
		  {
			lenhl=strlen(line_status[tswitch].handleline);

		   strncpy(s-lenhl,line_status[tswitch].handleline,lenhl);
		   datas=s;
		   begins=s-lenhl;
		  }

if (char_in_buf(tswitch))
		{
			   eventHappened=1;
		  {
            if (test_bit(user_lines[tswitch].user_info.toggles,STREAM_TOG))
			  get_no_echo(datas, (int)((endbufs-datas)-adjlen));
			  else
			 {
				get_string(datas, (int)((endbufs-datas)-adjlen));
				// this makes it so that people don't have
				// "lingering" identifiers in their handle lines
				// after being forced or kicked

				if (remake_handleline())
				   { lenhl=strlen(line_status[tswitch].handleline);
					 strncpy(s-lenhl,line_status[tswitch].handleline,lenhl);
					 datas=s;
					 begins=s-lenhl;
				   }
			  }

			remove_priority_messages(datas);

			if (*datas)
			 {
			   if (!test_bit(user_options[tswitch].privs,CAN_TYPE_ANSI_PRV))
				  remove_ansi(datas);
			   else
			   if (!test_bit(user_options[tswitch].privs,CAN_TYPE_FLASHING_PRV))
				  remove_flashing(datas);

			   if (user_options[tswitch].priority)
					  (void)limit_carrots(datas,6);

			   if (*datas=='/')
				 {
				   /*
					 print_string("Command: ");
					 print_str_cr(datas);
					*/

					command(datas,begins,tswitch);   /* It's a command so Parse it */
					if (islocked(DOS_SEM))
					  {
						 unlock_dos();
						 log_error("* GT.DEBUG - doslock error");
						 sprintf(i,"* The command %s returned with dos locked!",datas);
						 log_error(i);
					  }
				  }               /* Otherwise just print it to the buffer */
			   else
			   {


				  if (((*datas=='~')|| (*datas=='&')) && test_bit(user_options[tswitch].privs,ACTION_PRV))
				   {
					 if (!check_if_silenced())
					  {
                       sprintf(t,"==> |\\59(%02d):|\\ %s|*r1 %s",tswitch,user_lines[tswitch].user_info.handle,eat_one_space(datas+1));
					   lo->lines_typed++;
					   aput_into_buffer(server,t,line_status[tswitch].mainchannel,0,line_status[tswitch].mainchannel,tswitch,1);
					  };
				   }
				  else
				  if ((*datas=='`') && test_bit(user_options[tswitch].privs,SHUTDOWN_PRV) && (!check_if_silenced()))
				  {
				   aput_into_buffer(server,datas+1,line_status[tswitch].mainchannel,0,line_status[tswitch].mainchannel,tswitch,4);
				   lo->lines_typed++;
				  }
				  else
				  if (!check_if_silenced())
				   {
					/* here is the stuff for anonymous channels */
					if (channels[line_status[tswitch].mainchannel].current_cfg.anonymous)
					   /*ANONYMOUS*/
					   { aput_into_buffer(server,datas,line_status[tswitch].mainchannel,0,line_status[tswitch].mainchannel,tswitch,5);
						 lo->lines_typed++;
					   }
					else
					 if ((line_status[tswitch].lurking && line_status[tswitch].safe_lurking)
						  ||  ((*datas=='!') && test_bit(user_options[tswitch].privs,LURK_PRV)) )
						{
						 if (*datas=='!')
						   str_cpy(datas,datas+1);

						 print_lurk_message_from(begins,tswitch);
						}
					 else
						/*IDENTIFIED*/
					   { aput_into_buffer(server,begins,line_status[tswitch].mainchannel,0,line_status[tswitch].mainchannel,tswitch,0);
						 lo->lines_typed++;
					   }

				   };

				};


				if (line_status[tswitch].mainchannel==1)
				  if ((lo->lines_typed>lo->lines_allowed) && (lo->lines_allowed))
					 if (lo->warnings_left--)
					 {  lo->lines_typed--;
							 warn_user_lineout();
					  }
					 else
							line_out_user();


			 }

					if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
					  print_cr();

			 }; // End of Else from new_key checking


		 } // No Key Was Entered


	 if (aget_abuffer(&sentby, &channel, i, &type, &data1, &data2,&data3))
	 {

       if (neutralized(sentby,tswitch))
       {
        switch (type)
        {
          default:break;
        }
       }
       else
       switch (type)
		{
		  case 0  : if (data2!=tswitch)  /* Normal Message */
						lo->lines_typed=0;
					else // If it's a message from *US* and NO_RETURN_ECHO_TOG
					if (test_bit(user_options[tswitch].toggles,NO_RETURN_ECHO_TOG))
					  break; // DONT PRINT

                    switch (sys_info.message_mode)
                    {
                      case 1:
                      case 2:
                        if (line_status[tswitch].mainchannel!=channel)
                         { sprintf(temp,"C%d",channel);
                           print_string(temp);
                         }
                      default:
                                break;
                    }
                    wrap_line(i);
					if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
					  print_cr();
					break;
		  case 1  : if (data1!=tswitch) /*private message */
						lo->lines_typed=0;
					wrap_line(i);
					if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
					  print_cr();
					break;
		  case 2  : if (data1!=tswitch)      /* channel message */
						lo->lines_typed=0;
					wrap_line(i);
					if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
					  print_cr();
					break;
		  case 3  : if (data2==tswitch)   /* If this is YOUR login message*/
					   break;         /*DONT print it */
					if (data3>9)  /* it's a LINK message */
					   break;
					wrap_line(i);
					if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
					  print_cr();
					break;
		  case 4  : if (data3==tswitch) /* channel moving message*/
					   break;        /* If it's YOU don't print it */
					wrap_line(i);
					if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
					  print_cr();
					break;
		  case 5  : if ((data2==6) && (data1==tswitch)) /* FORCE message*/
							break;
					if ((data2==3) && (data1==tswitch)) /* channel mod message*/
							break;
					wrap_line(i);
					if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
					  print_cr();
					break;
		  case 11 : if (data3==10)  /* if it's a LEFT message */
					   if (data2==tswitch) /* AND it's YOU */
							break; /* THEN , DONT print it */
					wrap_line(i);
					if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
					  print_cr();
					break;
		  case 13 : if (!data1)
					   {print_file(i);        /* if this is a file message  */
						print_cr();           /*   print the file */
					   }
					else
					   {
						special_code(1,tswitch);
						print_str_cr(i);
						special_code(0,tswitch);
					   }
					break;

		  default : wrap_line(i);
					lo->lines_typed=0;
					if (test_bit(user_options[tswitch].toggles,DOUBLESPACE_TOG))
					  print_cr();
					break;
		}
	  eventHappened=1;
	 }


	  check_for_chat_confirmation();

	  if (!dcd_detect(tswitch)) leave();

	  if (!eventHappened)
	  {
	   task_sleep();
	  }
	  else
		eventHappened=0;

	  if (!dcd_detect(tswitch)) leave();

	  next_task();
	  wait_for_xmit(tswitch,30);

};
}




void ansi_choice_menu(void)
{
 char lines[][20]={"[F]ull Screen ANSI","[C]olor ANSI Only","[N]o ANSI"};
 char choices[]={'F','C','N',13,10,0};
 int loop;
 int got_key;
 int mode=2;
 int portnum = tswitch;
 int in_key;

/* if (!portnum) *//* CONSOLE */
/*  {  */
/*    line_status[portnum].full_screen_ansi=0;*/ /* console dosn't do REAL ansi */
/*    return;    */
/*  }  */


   line_status[portnum].should_timeout=0;
   line_status[portnum].time_online=time(NULL);
   user_options[portnum].time_warning_sec = LOGIN_TIMEOUT_SEC;
   user_options[portnum].warnings = 0;
   user_options[portnum].time_sec = LOGIN_TIMEOUT_SEC;
   line_status[portnum].should_timeout=1;


 print_cr();
 print_str_cr("Would you like:");
 print_cr();

 if (line_status[portnum].ansi)
   mode=0;

 for (loop=0;loop<3;loop++)
 {
   if (loop==mode)
    print_chr('*');
   else
    print_chr(' ');
   print_str_cr(lines[loop]);

 }

 print_cr();
 print_str_cr("Press <RETURN> for * default.");
 print_cr();
 print_string("Select: ");

 got_key=0;
 while (!got_key)
  {
   in_key=toupper(wait_ch());

   loop=0;
   while (choices[loop])
   {
    if (choices[loop]==in_key)
      got_key=1;
    loop++;
   }

  }

  print_chr(in_key);
  print_cr();

  switch(in_key)
  {
    case 'F'  :  line_status[portnum].full_screen_ansi=1;
    case 'C'  :  line_status[portnum].ansi=1;
                 break;
    case 'N'  :  line_status[portnum].ansi=0;
                 line_status[portnum].full_screen_ansi=0;
                 break;
    default   :  if (line_status[portnum].ansi)
                    line_status[portnum].full_screen_ansi=1;
                 break;
  }

  clear_screen();
  if (line_status[portnum].ansi)
    {
     if (test_bit(user_options[portnum].toggles,HIGH_ASCII_TOG))
       line_status[portnum].ansi |= 0x02;
    }

   user_options[portnum].warnings = 0;
   user_options[portnum].time_sec = 0;
   line_status[portnum].should_timeout=0;

}

void print_system_login(void)
{
   line_status[tswitch].ansi = find_ansi();
   clear_screen();
   print_cr();
   position(0,0);

   clear_screen();

	print_str_cr("Version Title:  ");
	print_str_cr(version_title);
	print_str_cr("System Number: ");
	print_string("System #");
	print_str_cr(system_number);
	print_cr();

   if (line_status[tswitch].ansi)
	print_str_cr("ANSI Detected.");
	else
	print_str_cr("ANSI Disabled.");

	ansi_choice_menu();

	print_cr();

   print_file_to_cntrl(LOGIN_MESSAGE_FILE,tswitch,1,0,1,0);
   empty_inbuffer(tswitch);

}

void clear_call_info(struct caller_id_struct *call_info)
{
  memset(call_info,0,sizeof(*call_info));

}



void process_connect(int portnum,struct caller_id_struct *call_info,int newline);
void process_date(int portnum,struct caller_id_struct *call_info,int newline);
void process_time(int portnum,struct caller_id_struct *call_info,int newline);
void process_number(int portnum,struct caller_id_struct *call_info,int newline);
void process_message(int portnum,struct caller_id_struct *call_info,int newline);
void process_name(int portnum,struct caller_id_struct *call_info,int newline);
void process_ring(int portnum,struct caller_id_struct *call_info,int newline);

void process_connect(int portnum,struct caller_id_struct *call_info,int newline)
{
   if (newline)
   {
    strcpy(call_info->modem_connect_string,"300");
   }
   else
   {
   get_result_code(call_info->modem_connect_string,MODEM_STRING_LEN);
   }
}

void process_date(int portnum,struct caller_id_struct *call_info,int newline)
{
   get_result_code(call_info->date,CID_DATE_LEN);

}

void process_time(int portnum,struct caller_id_struct *call_info,int newline)
{
   get_result_code(call_info->time,CID_TIME_LEN);

}

void process_number(int portnum,struct caller_id_struct *call_info,int newline)
{
   get_result_code(call_info->number,CID_NUM_LEN);
}

void process_message(int portnum,struct caller_id_struct *call_info,int newline)
{
   get_result_code(call_info->message,CID_MESSAGE_LEN);

}

void process_name(int portnum,struct caller_id_struct *call_info,int newline)
{
   get_result_code(call_info->name,CID_NAME_LEN);
}

void process_ring(int portnum,struct caller_id_struct *call_info,int newline)
{
   call_info->num_rings++;
}

#define MAX_MODEM_RESULT_LEN 10

struct modem_result_struct {
  char result[MAX_MODEM_RESULT_LEN];
  void (*process_result)(int portnum, struct caller_id_struct *call_info,int newline);
};

struct modem_result_struct modem_results[] = {
      { "CONN", process_connect }, /* this must be the first entry */
      { "DATE", process_date } ,
      { "TIME", process_time } ,
      { "NMBR", process_number } ,
      { "MESG", process_message } ,
      { "NAME", process_name } ,
      { "RING", process_ring } ,
      { "" , 0 }
};


#define INPUT_STRING_LEN  20
int wait_for_connect(int portnum,struct caller_id_struct *call_info)
{
   char input_char;
   int match,loop,pos;
   int newline;
   int waiting_for_connect=1;
   char input_string[INPUT_STRING_LEN+1];
   int flag;
   time_t tempnum;
   int baud;


   clear_call_info(call_info);

   do
	{
	  flag = 0;
	  while (!dcd_detect(tswitch))
	   {
		 next_task();
		 DosSleep(100l);
	   };
	  tempnum = time(NULL);
	  while (((time(NULL) - tempnum) < 1) && (!flag))
	   {
		 if (!dcd_detect(tswitch))
			{ flag = 1;
			  end_task();

			}
		 next_task();
	   };
	} while (flag);


   if (is_console_node(tswitch))
     return 0;

     do {

             newline=0;
             pos=0;
             flag=1;
             do {

                  input_char = toupper(wait_ch());

                 if (pos == INPUT_STRING_LEN)
                    flag=0;
                 else
                  switch (input_char)
                  {
                    case  10:
                    case  13:
                               newline=1;
                    case  ' ':
                    case  '/':
                    case  '\\':
                               input_string[pos] = 0;
                               flag=0;
                               break;

                    default:   input_string[pos] = input_char;
                               pos++;
                               break;

                  }

             } while (flag);


        loop=0;
        match=-1;
        while (modem_results[loop].process_result)
        {
          if (!g_strncmp(modem_results[loop].result,input_string,strlen(modem_results[loop].result)))
          {
            match=loop;
            if (match==0)
             waiting_for_connect=0;
            break;
          }
          else
          loop++;

        }

        if (match!=-1)
         {
           (modem_results[match].process_result)(portnum,call_info,newline);
         }
        else
          {
            /* no match */
          }
     }
     while ((waiting_for_connect));


     if (tempnum == 0)
	   {
		baud = 300;
        strcpy(call_info->modem_connect_string,"300");
        strcpy(line_status[tswitch].baud,"300");
	   }
      else
       {char *temp=line_status[tswitch].baud;
//        get_result_code(call_info->modem,30);

        strncpy(line_status[tswitch].baud,call_info->modem_connect_string,9);
        line_status[tswitch].baud[9]=0;
        while ((*temp<='9') && (*temp>='0'))
          temp++;
        *temp=0;
		baud = (unsigned int) atol(line_status[tswitch].baud);
       };

    if (!port[tswitch].fixed_dte_speed)
	{
        /* ONLY set their baud rate if it's not a
           fixed DTE speed modem */
     if ((baud==300) || (baud==1200) || (baud==2400) || (baud=9600) || (baud==14400))
       {

		 set_baud_rate(tswitch,baud,8,1,'N');
       }
       else
       {
		 set_baud_rate(tswitch,300,8,1,'N');
       }
     }

     sprintf(line_status[tswitch].baud,"%d",baud);
     line_status[tswitch].connect = 1;
	 sync_status[tswitch].handlelinechanged_at_tick = dans_counter;
	 empty_inbuffer(tswitch);

     return 0;
}

void wait_for_silence(int portnum)
{

  if (!is_console())
  {
      DosSleep(100l);

      print_str_cr("...GTalk...");

      DosSleep(300l);
  }

}

void connect_user(void)
{

   char s[320];
  // char *data;
   time_t tempnum;
   char chr1=0, chr2=0, chr3=0;
   unsigned int baud;
   int flag;
   int count;
   char modem_hung=0;
   int result;
   struct caller_id_struct *save_call_info;
   struct caller_id_struct call_info;

   dump_output_buffer(tswitch);
   if (wait_for_dcd_state(tswitch,1))
     {
      hang_up(tswitch);
      delay(10);
	 }
   else
	 {
		change_dtr_state(tswitch,1);
	 }

   line_status[tswitch].connect = 0;
   sync_status[tswitch].handlelinechanged_at_tick = dans_counter;


   line_status[tswitch].ansi = is_console();
     // clear_screen();

   if (!is_console_node(tswitch))
	{
      set_death_off();

	  /* SET THE BAUD RATE INITIALLY */
	  set_baud_rate(tswitch,port[tswitch].baud_rate,8,1,'N');
	  delay(2);
	  count=0;
	  do
		{
		  if (count)
			{
			  set_death_on();
			  delay(3);
			  set_death_off();
			}

		  sendslow(tswitch,"AT");
		  sendslow(tswitch,cr_lf);
		}
		while (!(result=wait_for_modem_result("K",20)) && (count++<6));

	  if (!result)
	  {
		 modem_hung=1;

         set_death_on();
         delay(10);
         set_death_off();
	  }

	  count=0;
	  do
		{
		  sendslow(tswitch,port[tswitch].init_string);
		  sendslow(tswitch,cr_lf);
		}
	  while (!(result=wait_for_modem_result("K",20)) && (count++<6));

	  set_death_on();

	  // IF THE MODEM IS FRIED, then tell SOMEONE about it
	  if ((!result) && modem_hung )
		{
			char str[200];
			port_fast[tswitch]->modem_responding = 0;
			if (!port_fast[tswitch]->ignore_response)
				{
					sprintf(str,"|*f1|*p1|*h1%c--> Modem on Line [|*p0%02d|*f1|*h1|*p1] not responding!! (|*p0%d|*f1|*h1|*p1 retries)%c",
									   7,tswitch,port_fast[tswitch]->num_retries++,7);
					aput_into_buffer(server,str,0,5,tswitch,9,0);
				}

			hang_up(tswitch);

			if (port_fast[tswitch]->num_retries<5)
			  { delay(18*5);
				end_task();
			  }

			hang_up(tswitch);

			if (port_fast[tswitch]->num_retries<20)
			   delay(port_fast[tswitch]->seconds_between_retries * 18);
			else
			 {
			  sendslow(tswitch,"+++");
			  wait_for_modem_result("K",30);

			  if (port_fast[tswitch]->ignore_response)
			   delay(18 * 60);
			  else
			   delay(port_fast[tswitch]->seconds_between_retries * 18 * 3);
			  }
			end_task();

		}
	  else
		if (!port_fast[tswitch]->modem_responding)
		 {
			char str[200];

			sprintf(str,"|*f1|*p1|*h1%c--> Modem on Line [|*p0%02d|*f1|*h1|*p1] *IS* responding!! (after |*p0%d|*f1|*h1|*p1 retries)%c",
						7,tswitch,port_fast[tswitch]->num_retries,7);

			port_fast[tswitch]->num_retries=0;
			port_fast[tswitch]->modem_responding=1;
			aput_into_buffer(server,str,0,5,tswitch,9,0);
		 }


	}
 //  else
 //   clear_screen();

	empty_inbuffer(tswitch);


   if (wait_for_connect(tswitch,&call_info))
    {
      end_task();
    }

   wait_for_silence(tswitch);



     if (!is_console_node(tswitch))
     {
         if (!port_fast[tswitch]->no_dcd_detect) /* delays for MNP not LOCAL */
           {
            /* even more delay */
            delay(40);
            DosSleep(1000l);
           }

         print_cr();
         sprintf(s,"--> Login Node [%02d] at %s ",tswitch,call_info.modem_connect_string);
		 broadcast_message(s);
         if (*call_info.number)
         {
           save_call_info = g_malloc(sizeof(*save_call_info),"CALLID");
           line_status[tswitch].call_info = save_call_info;

           sprintf(s,"--> Node [%02d] Number: %s%s                 ame: %s",tswitch,call_info.number,cr_lf,call_info.name);
           aput_into_buffer(server,s,0,5,tswitch,8,0);
           *save_call_info = call_info;
         }

         delay(5);
         DosSleep(200l);
     }
	 else  /* if console then do status display */
	  { time_t now;
		int flag1=1;
		int charin;

		now = time(NULL);
		if (!tswitch)
        while(((time(NULL)-now) < CONSOLE_PRESTATUS_DELAY) && flag1)
		 {if ( (charin=int_char(tswitch))!=-1 )
             switch(charin)
              {

                case 13: flag1=0;
                         break;
                case 27:
                case 32: flag1=0;
                         break;
              }


           next_task();
          }

       if (/* (flag1) &&*/ (charin!=13)) console_status();

	   /* NOW THEY ARE WANTING TO CONNECT, so let them login */


	   strcpy(line_status[tswitch].baud,"Con");
	   line_status[tswitch].connect = 1;
	   sync_status[tswitch].handlelinechanged_at_tick = dans_counter;
	  };
}

void print_login_line(void)
{ int flag=!islocked(DOS_SEM);
  char s[100];
  char stime[40];
  time_t now;
  struct tm *nowstruct;


  if (flag)  lock_dos(149);
  now=time(NULL);
  nowstruct=localtime(&now);
  strftime(stime,39,"%m/%d/%y %I:%M:%S %p",nowstruct);
  if (flag)  unlock_dos();

  sprintf(s,"GTalk: Login Node [%02d] at (%s) %s",tswitch,
       line_status[tswitch].baud,stime);
  print_str_cr(s);
}




int login_multiple_login_check(int portnum)
{
    int is_online;
    int check_user;
    char s[100];

    if (!test_bit(user_lines[portnum].class_info.privs,MULTIPLE_LOGIN_PRV))
    {
     is_online = 0;
     check_user = 0;
     while ((check_user<MAX_THREADS) && (!is_online))
      {
        if (line_status[check_user].online)
         if (check_user != portnum)
          if (user_lines[portnum].user_info.number == user_lines[check_user].user_info.number)
           is_online = 1;
        check_user++;
      };
     if (is_online)
      {
        print_cr();
        print_str_cr("    *** MULTIPLE LOGIN ATTEMPT ***");
        print_cr();
        print_str_cr("Only one user permitted per account!  Sysops have been notified!");
        print_cr();
        print_chr(7);
        print_chr(7);
        sprintf(s,"--> Multiple login attempt on Node [%02d] User [%03d]",portnum,
                  user_lines[portnum].user_info.number);
        aput_into_buffer(server,s,0,5,portnum,7,0);
        leave();
      };
    };

   return (0);
}

int login_sys_lock_check(int portnum)
{   char s[100];
           /* if system is locked we don't have to ask
              guests for their handles */

     if (user_lines[portnum].class_info.priority!=0);
       {
        if (sys_toggles.logins_disabled)
           {
            print_cr();
            print_str_cr("**** LOGINS CURRENTLY DISABLED FOR SHUTDOWN ****");
            print_file(LOGINS_DISABLED_TXT);
            log_off(portnum,1);
           }

        if (sys_info.lock_priority<user_lines[portnum].class_info.priority)
           {
            print_str_cr("");
            sprintf(s,"Lock Priority [%d]",sys_info.lock_priority);
            print_str_cr(s);
            print_cr();
            if (user_lines[portnum].user_info.number<0)
              print_file(GUEST_LOCKED_TXT);
            else
              print_file(LOCKED_TXT);
            log_off(portnum,1);
           }
        }
    return (0);
}

int login_get_guest_handle(int portnum)
{
    char handletemp[45];

    print_cr();
    print_string(" Handle: ");
    *handletemp=0;
    while (!(*handletemp))
        get_string(handletemp,20);
    filter_ansi(handletemp,user_lines[tswitch].user_info.handle);
    print_cr();
   return (0);
}

void load_as_guest(int portnum)
{
     /* GUEST USER SO LOAD GUEST USER FILE */
     if (load_info_by_class("GUEST",&user_lines[portnum]))
	   {
		  log_error("* GT.DEBUG - user file guest failed to load");
			/* the user file couldn't load a guest so make one */
		  if (is_console())
		  {
			make_manual_sysop();
			repeat_chr('*',9,0);
			print_string("  USER FILE IS CORRUPTED  ");
            repeat_chr('*',9,1);
          }
        else
           make_manual_user(portnum);

        log_error("* Guest User login ERROR");
       }
     user_lines[portnum].user_info.width=80;
     user_lines[portnum].user_info.number=NO_SAVE_USER;

}


int login_cbd_check(int portnum)
{
   time_t cur_system_time;

   if (!test_bit(user_lines[portnum].class_info.privs,IMMUNE_TO_CBD_PRV) && sys_info.call_back_delay)
    { unsigned long int time_remaining;

      cur_system_time=time(NULL);

      if ( ((unsigned long int)cur_system_time -
            (unsigned long int)user_lines[tswitch].user_info.last_call ) <
                                         sys_info.call_back_delay)

       {
          repeat_chr('*',5,0);
          print_string(" Call Back Delay Lockout ");
          repeat_chr('*',5,1);
          time_remaining=sys_info.call_back_delay- (cur_system_time-
                               user_lines[tswitch].user_info.last_call);

          repeat_chr('*',5,0);
          print_string(" Call Back in: ");
          print_expanded_time_cr(time_remaining);

           print_file_to_cntrl(CBD_FILE,tswitch,1,0,0,0);
           leave();
       }

      /* check so see if they SHOULD be calling back */
    }

   return (0);
}

int login_lurk_check(int portnum)
{
   /* Check to see if someone is trying to log in Lurked but does
      not have the lurk priv */
    line_status[portnum].lurking = ((line_status[tswitch].lurking) && (test_bit(user_lines[tswitch].class_info.privs,LURK_PRV)));
    return (LOGIN_VALID);
}

int login_expiration_check(int portnum)
{  char s[100];
   time_t cur_system_time;
   int retval = LOGIN_VALID;
   int daysleft;

   /* This code detects whether a user is attempt to use an account */
   /* past the expiration date */

   if ((user_lines[portnum].class_info.priority) && (user_lines[portnum].user_info.expiration || user_lines[portnum].user_info.starting_date))
     {                  /* If they have a nonzero priority and nonzero date */
      cur_system_time=time(NULL);

       if ((user_lines[portnum].user_info.starting_date>cur_system_time))
       {

         // ITs suspended...

        print_cr();
        print_str_cr("   *** YOUR ACCOUNT IS SUSPENDED ***");
        print_cr();
        print_string("   Until: ");
        strcpy(s,asctime(localtime(&user_lines[portnum].user_info.starting_date)));
        print_str_cr(s);
        print_chr(7);
        print_chr(7);

        print_file_to_cntrl(SUSPENDED_FILE,portnum,1,0,0,0);

        retval = LOGIN_GUEST;
       }
       else
      if (((user_lines[portnum].user_info.expiration)<cur_system_time) && user_lines[portnum].user_info.expiration)
       { // START BLOCK A

        if ((user_lines[portnum].user_info.expiration+user_lines[portnum].user_info.credit)<cur_system_time)
             {  print_cr();

                print_str_cr("   *** YOUR ACCOUNT HAS EXPIRED ***");
                print_cr();
                print_str_cr("      You have guest access only.");
                print_chr(7);
                print_chr(7);
                print_cr();
                retval = LOGIN_GUEST;
            }
         else
         {
            print_cr();
            print_str_cr("   *** YOUR ACCOUNT HAS EXPIRED ***");
            print_cr();
            sprintf(s,"You have only %lu days credit remaining.",((user_lines[portnum].user_info.expiration+user_lines[portnum].user_info.credit)-cur_system_time)/86400l);
            print_str_cr(s);
            print_chr(7);
            print_chr(7);
            print_cr();
        }

       }   // DONE BLOCK A
       else
       {
        daysleft = ((unsigned long int)(user_lines[portnum].user_info.expiration -
                     cur_system_time)/86400l);
        if (daysleft <= 10)
        {
         print_cr();
         print_string("Warning: Your account will expire in ");
         sprintf(s,"%d day(s).",daysleft);
         print_str_cr(s);
         print_cr();
         print_chr(7);
         print_chr(7);
        };
       };
     };

    return (retval);
}


int login_restricted_line_check(int portnum)
{
  if (user_lines[portnum].class_info.priority)
  {
    if (port_fast[portnum]->restrict_level)
     {
        if (port_fast[portnum]->restrict_level<30)
           {
              print_file_to_cntrl(RESTRICTED_NODE_FILE,tswitch,1,0,0,0);
              leave();
           }
      }
  }

  return (LOGIN_VALID);
}

int login_validation_check(int portnum)
{
   if (user_lines[portnum].user_info.validate_info != USER_NOT_VALIDATED)
     {
       return (LOGIN_VALID);
     }

    /* Check to see if this is a verification node */


   if (!port_fast[portnum]->verify_node)
   {
     print_file_to_cntrl(VALIDATION_REMINDER_FILE,portnum,1,0,0,0);
     return (LOGIN_VALID);
   }


    /* ok, it's a validation node... and he needs to be validated */
    online_validation(portnum);
    return (0);
}

struct login_checks_struct guest_login_checks[]= {
        { login_restricted_line_check},
        { login_sys_lock_check},
        { login_get_guest_handle},
        { login_lurk_check },
        { NULL }};

struct login_checks_struct user_login_checks[]= {
        { login_validation_check },
        { login_restricted_line_check },
        { login_sys_lock_check },
        { login_cbd_check },
        { login_expiration_check },
        { login_multiple_login_check },
        { login_lurk_check },
        { NULL }};

int check_login(struct login_checks_struct *check,int portnum)
{
    int retval;

    while (check->check_login)
    {
      if ((retval = (check->check_login)(portnum))!=LOGIN_VALID)
         return (retval);
      check++;
    }

    return (LOGIN_VALID);
}

int get_pass_prompt(void)
{

    char pass[PASSWORD_LEN+1];
    char pass_temp[PASSWORD_LEN+1];
    int retval;
    int ret_val2;
    int tempnum;
    char *point;


      user_lines[tswitch].user_info.enable=0;
      print_file_to_cntrl(PASS_PROMPT_FILE,tswitch,1,0,0,0);
      delay(2);

      print_string(" User ID: ");
      print_chr(7);

      empty_inbuffer(tswitch);
      get_string_cntrl(pass,10,0,0,0,0,1,0,0);

        /* is the user number blank? if so then just let them login
           as a guest
         */

      if (!*pass)
       return (LOGIN_BLANK);

        /* fix the password, this happens to be the same as fix_classname */

      fix_classname(pass);

       /* check to see if they want to get validated */

      if (!strcmp(pass,"NEW"))
        {
           /* give them extra time in the new user application */

           line_status[tswitch].should_timeout=0;
           line_status[tswitch].time_online=time(NULL);
           user_options[tswitch].time_warning_sec = APPLICATION_TIMEOUT_SEC;
           user_options[tswitch].warnings = 0;
           user_options[tswitch].time_sec = APPLICATION_TIMEOUT_SEC;
           line_status[tswitch].should_timeout=1;

           retval = new_user_app();

            /* but now they should timeout again */

           line_status[tswitch].should_timeout=0;
           line_status[tswitch].time_online=time(NULL);
           user_options[tswitch].time_warning_sec = LOGIN_TIMEOUT_SEC;
           user_options[tswitch].warnings = 0;
           user_options[tswitch].time_sec = LOGIN_TIMEOUT_SEC;
           line_status[tswitch].should_timeout=1;
           return ((retval) ? LOGIN_INVALID_COUNT : LOGIN_INVALID_NOCOUNT);
        };


            /* Convert the entered number into an integer */

          tempnum=str_to_num(pass,&point);

            /* if they didn't enter a number then let them try again*/

          if (tempnum<0)
            {
             print_cr();
             return (LOGIN_INVALID_COUNT);
            }

                /* get a password from them */


             print_string("Password: ");
             get_string_cntrl(pass,11,'.',0,1,0,0,0,0);


                /* if the pass was aborted then let them go around again */

             if (!*pass)
               {
                 print_str_cr("<abort>");
                 return (LOGIN_INVALID_NOCOUNT);
               }

                /* check for lurk login */

             if (*pass=='!')
               {
                line_status[tswitch].lurking=1;
                strcpy(pass_temp,pass+1);
                strcpy(pass,pass_temp);
               }

                /* now we need to check and see if the password matched
                   for the given user number */

             print_cr();

             switch(ret_val2 = load_user_info(tempnum,&user_lines[tswitch]))
               {
                    case LUERR_NO_SUCH_USER:
                    case LUERR_FREAD_FAILED:
                                 return (ret_val2);
                    case 0:
                                 break;   /* read it fine */
                    default:
                                 log_error("*reading of user failed");
                                 print_str_cr("Unable to Read User Record.");

                                 return (LOGIN_ERROR);
               }

             if (strcmp(user_lines[tswitch].user_info.password,pass))
               {
                  line_status[tswitch].lurking=0;
                  // sprintf(s,"BAD LOGIN PW : ->%s<-",pass);
                  // log_event(PASSWORD_LOG_FILE,s);

                  print_str_cr("Login Failed.");
                  print_cr();

                  return (LOGIN_INVALID_COUNT);

               }

               if (!user_lines[tswitch].user_info.enable)
                {  print_str_cr("Your account was corrupt, please notify a sysop");
                   print_str_cr("Correcting...");
                   user_lines[tswitch].user_info.enable=1;
                }

               if ((user_lines[tswitch].user_info.number<0) || (!user_lines[tswitch].user_info.enable))
                {  print_str_cr("Account Not Active.");
                   return (LOGIN_INVALID_COUNT);
                }
        /* apparently they made it through all the checks */

        return (LOGIN_VALID);

}



int pass_prompt(void)
{
  int count=NUM_LOGIN_ATTEMPTS;
  int retval=LOGIN_GUEST;
  int cont_flag=1;

  while ((count) && cont_flag)
  {
      switch(get_pass_prompt())
      {
        case LOGIN_VALID:           retval = LOGIN_VALID;
                                    cont_flag=0;
                                    break;

        case LOGIN_INVALID_COUNT:   count--;
                                    retval = LOGIN_GUEST;
        case LOGIN_INVALID_NOCOUNT: break;

        case LOGIN_BLANK:           cont_flag=0;
                                    retval = LOGIN_GUEST;
                                    break;

        case LOGIN_ERROR:           retval = LOGIN_ERROR;
                                    cont_flag=0;
                                    break;
        case LUERR_NO_SUCH_USER:
        case LUERR_FREAD_FAILED:
                                    retval = LOGIN_GUEST;
                                    count--;
                                    break;
        default:                    count--;
                                    break;
      }
  }

    return (retval);

}


int default_login(int portnum)
{
  load_as_guest(portnum);


  return (LOGIN_VALID);
}


void user_login_process(int should_check_cbd)
{
   int retval;
   int portnum = tswitch;

   line_status[tswitch].apps=0;

   line_status[tswitch].should_timeout=0;
   line_status[tswitch].time_online=time(NULL);
   user_options[tswitch].time_warning_sec = LOGIN_TIMEOUT_SEC;
   user_options[tswitch].warnings = 0;
   user_options[tswitch].time_sec = LOGIN_TIMEOUT_SEC;
   line_status[tswitch].should_timeout=1;

    set_temp_user_info(tswitch);

    switch(retval = pass_prompt())
    {
        case LOGIN_VALID:       print_str_cr("User Login.");
                                retval = check_login(user_login_checks,portnum);
                                break;
                                print_str_cr("Login Error...(1)");
        case LOGIN_ERROR:       retval = default_login(portnum);
                                break;
        default:                break;
    }

    switch(retval)
    {
        case LOGIN_VALID:       break;

        case LOGIN_GUEST:       load_as_guest(portnum);
                                set_temp_user_info(tswitch);
                                print_str_cr("Guest Login.");
                                retval = check_login(guest_login_checks,portnum);
                                break;
        default:
                                print_str_cr("Login Error...(2)");
        case LOGIN_ERROR:       retval = default_login(portnum);
                                break;
    }

   /* OK, the user is finally allowed in so let him in.*/

   set_temp_user_info(tswitch);
   init_login_vars(tswitch);
   print_login_messages(tswitch);

   line_status[tswitch].handlelinechanged = ALL_BITS_SET;
   sync_status[tswitch].handlelinechanged_at_tick = dans_counter;
   mark_user_log_on();
   print_cr();

}

void ginsu(void)
 {
   EXCEPTIONREGISTRATIONRECORD er = { NULL, (_ERR *)&ExceptionHandler }; /* To register the handler */

   DosSetExceptionHandler (&er);                      /* Add our routine to the chain */

   init_vars();
   connect_user();
   empty_inbuffer(tswitch);
   flushall();
   print_system_login();
   print_login_line();

   user_login_process(1);
      /* this will init the temp vars and run
                     init login vars too */

               /* OKAY , THEY ARE LOGGED IN */
   initabuffer(CLIENT_BUFFER);
   wait_for_xmit(tswitch,30);

   print_log_in(tswitch);
   show_log_in(tswitch);


   empty_inbuffer(tswitch);

   main_loop();                /* this should never exit */
   end_task();
 };

void relogged(void)
 {
   EXCEPTIONREGISTRATIONRECORD er = { NULL, (_ERR *)&ExceptionHandler }; /* To register the handler */
   char temp[10];

   DosSetExceptionHandler (&er);                      /* Add our routine to the chain */

   line_status[tswitch].restart=1; /* safeguard to make sure this
                                      task will restart */
   strcpy(temp,line_status[tswitch].baud);
   init_vars();
   strcpy(line_status[tswitch].baud,temp);
   empty_inbuffer(tswitch);
   print_system_login();
   print_login_line();

   user_login_process(0);
      /* this will init the temp vars and run
                     init login vars too */

               /* OKAY , THEY ARE LOGGED IN */

   initabuffer(CLIENT_BUFFER);

   wait_for_xmit(tswitch,30);

   print_log_in(tswitch);
   show_log_in(tswitch);


   empty_inbuffer(tswitch);

   main_loop();                /* this should never exit */
   end_task();

}

void midnight_task(void)
{ int loop;
  int flag=!islocked(DOS_SEM);
  char s[80];
  struct tm *time_tmp;
  struct tm exp_time_now;
  time_t now;
  lock_dos(150);
  now=time(NULL);
  unlock_dos();

  if (sys_info.day_calls.total>sys_info.record_calls.total)
   {
    sys_info.record_calls.total=sys_info.day_calls.total;
    for (loop=0;loop<10;loop++)
       sys_info.record_calls.total=sys_info.day_calls.total;

   }

  sys_info.yesterday_calls.total=sys_info.day_calls.total;
  sys_info.day_calls.total=0;
  for (loop=0;loop<10;loop++)
   { sys_info.yesterday_calls.baud[loop]=sys_info.day_calls.baud[loop];
     sys_info.day_calls.baud[loop]=0;
   }

  if (flag)  lock_dos(151);

  time_tmp=localtime(&now);
  exp_time_now=*time_tmp;
  strftime(s,79,"--> Midnight on %A %B %d, %Y",time_tmp);
  if (flag)  unlock_dos();

  if (sys_info.this_month_number != exp_time_now.tm_mon)
    {
       sys_info.last_month_number=sys_info.this_month_number;
       sys_info.this_month_number=exp_time_now.tm_mon;
       sys_info.last_month_calls.total=sys_info.month_calls.total;
       sys_info.month_calls.total=0;

    }
  sys_toggles.calls_updated++;

  broadcast_message(s);
  end_task();
};


void save_channel_info_function(void)
{
   FILE *fileptr;
   int should_lock=!islocked(DOS_SEM);
   int loop;

   if (should_lock) lock_dos(152);
   sys_info.current_time=time(NULL);

   if (!(fileptr=g_fopen(CHANNEL_CONFIG_FILE,"wb","GT.C #1")))
     {
        log_error(CHANNEL_CONFIG_FILE);
        log_error("* tried to save system information");
        if (should_lock) unlock_dos();
        return;
     }
   fseek(fileptr,0,SEEK_SET);

   for (loop=0;loop<=sys_info.max_channels;loop++)
   if (!fwrite(&channels[loop].default_cfg,sizeof (struct channel_information),1,fileptr))
     { log_error(CHANNEL_CONFIG_FILE);
       g_fclose(fileptr);
       if (should_lock) unlock_dos();
       return;
     }

   if(g_fclose(fileptr))
     log_error(CHANNEL_CONFIG_FILE);
   if (should_lock) unlock_dos();
   return;
}


void save_sys_info_function(void)
{
   FILE *fileptr;

   sys_info.current_time=time(NULL);

   if (!(fileptr=g_fopen(SYSTEM_CONFIG_FILE,"wb","GT.C #1")))
     {
        log_error(SYSTEM_CONFIG_FILE);
        log_error("* tried to save system information");
        return;
     }

   fseek(fileptr,0,SEEK_SET);
   if (!fwrite(&sys_info,sizeof (struct system_information),1,fileptr))
     { log_error(SYSTEM_CONFIG_FILE);
       g_fclose(fileptr);
       return;
     }

   if(g_fclose(fileptr))
     log_error(SYSTEM_CONFIG_FILE);
   return;
}

void save_sys_info(void)
{
   aput_into_buffer(server,"--> Saving System Info",0,5,tswitch,8,0);
   save_sys_info_function();
   end_task();
}


/* OLD

void linked(void)
 {
   EXCEPTIONREGISTRATIONRECORD er = { NULL, (_ERR *)&ExceptionHandler }; /* To register the handler */
   char temp[10];

   DosSetExceptionHandler (&er);                      /* Add our routine to the chain */
   line_status[tswitch].restart=1; /* safeguard to make sure this
									  task will restart */
   strcpy(temp,line_status[tswitch].baud);
   init_vars();
   init_login_vars(tswitch);
   strcpy(line_status[tswitch].baud,temp);
   line_status[tswitch].connect=1;
   sync_status[tswitch].handlelinechanged_at_tick = dans_counter;
   empty_inbuffer(tswitch);
   print_cr();
   make_manual_user();
   line_status[tswitch].link=1;
   line_status[tswitch].link_info.auto_sp_minutes=10;
   line_status[tswitch].link_info.repeat_sp_lists=1;
   line_status[tswitch].link_info.can_see_guests=1;
   line_status[tswitch].link_info.send_sp_now=0;
   sprintf(user_lines[tswitch].user_info.handle,"Remote #%02d",tswitch);
   initabuffer(CLIENT_BUFFER);
   line_status[tswitch].mainchannel=2;
   print_log_in(tswitch);
   line_status[tswitch].handlelinechanged = ALL_BITS_SET;
   sync_status[tswitch].handlelinechanged_at_tick = dans_counter;
   empty_inbuffer(tswitch);
   print_cr();
   /* just in case turn off ansi again */
   line_status[tswitch].ansi = 0;
   print_sys_mesg("Linked");
   link_loop();     /* this function should never exit */
   end_task();
}
*/

void do_link_login_functions(void)
{
   char temp[10];
   int portnum = tswitch;


   line_status[portnum].restart=1; /* safeguard to make sure this
                                      task will restart */
   strcpy(temp,line_status[portnum].baud);
   init_vars();
   init_login_vars(portnum);
   strcpy(line_status[portnum].baud,temp);
   line_status[portnum].connect=1;
   sync_status[portnum].handlelinechanged_at_tick = dans_counter;
   empty_inbuffer(portnum);
   print_cr();
   make_manual_user(portnum);
   line_status[portnum].link=1;
   line_status[portnum].link_info.auto_sp_minutes=10;
   line_status[portnum].link_info.repeat_sp_lists=1;
   line_status[portnum].link_info.can_see_guests=1;
   line_status[portnum].link_info.send_sp_now=0;
   sprintf(user_lines[portnum].user_info.handle,"Remote #%02d",portnum);
   initabuffer(CLIENT_BUFFER);

   print_log_in(portnum);
   line_status[portnum].handlelinechanged = ALL_BITS_SET;
   sync_status[portnum].handlelinechanged_at_tick = dans_counter;
   empty_inbuffer(portnum);
   print_cr();
   /* just in case turn off ansi again */
   line_status[portnum].ansi = 0;
   print_sys_mesg("Linked");


}


void linked(void)
 {
   EXCEPTIONREGISTRATIONRECORD er = { NULL, (_ERR *)&ExceptionHandler }; /* To register the handler */

   DosSetExceptionHandler (&er);                      /* Add our routine to the chain */

   do_link_login_functions();
   line_status[tswitch].mainchannel=2;

   link_loop();     /* this function should never exit */
   end_task();
}

void g_linked(void)
 {
   EXCEPTIONREGISTRATIONRECORD er = { NULL, (_ERR *)&ExceptionHandler }; /* To register the handler */

   DosSetExceptionHandler (&er);                      /* Add our routine to the chain */

   do_link_login_functions();
   line_status[tswitch].mainchannel=250;

   glink_main();    /* this functions should never exit */
   end_task();
}

void shutdown_node(int portnum)
{

 if (!is_console_node(portnum))
     {

		if (wait_for_dcd_state(portnum,1))
          {

            print_file_to_cntrl(SHUTDOWN_MESSAGE_FILE,portnum,1,0,0,0);
            wait_for_xmit(portnum,30);
            hang_up(portnum);
            delay(5);
          }

       delay(10);
       sendslow(portnum,cr_lf);
       sendslow(portnum,"AT");
       sendslow(portnum,cr_lf);
       delay(6);
       sendslow(portnum,port[portnum].de_init_string);
       sendslow(portnum,cr_lf);

     }
    else
       {
         print_file_to_cntrl(SHUTDOWN_MESSAGE_FILE,portnum,1,0,0,0);
         wait_for_xmit(portnum,30);
       }
}


void shutdown_task(void)
{
  shutdown_node(tswitch);
  de_initport(tswitch);
  end_task();
}

