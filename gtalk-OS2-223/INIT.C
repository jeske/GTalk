
/* INIT.C */


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#define INCL_DOS
#define INCL_DOSMISC
#define INCL_DOSPROCESS
#include <os2.h>

/* INCLUDES */
#include "include.h"
#include "gtalk.h"
#include "math.h"
#include "console.h"
#include "boards.h"

#include "function.h"
#include "except.h"

void start_pipe_server(void);
#include "signal.h"


/* prototype */
void relog_node_event(void);


PCHAR pc = NULL;

int gtalk_exit_now = 0;
int pipe_server=0;
HPIPE pipe_handle;

/* DEFINES */

void de_init_stdio(void);

// #define COPY_PROTECTION_ON
#undef COPY_PROTECTION_ON

#define PORTS_WITHOUT_COPY_PROTECTION 20

/* VERSION NUMBER MUST BE CHANGED IN DIAGS.C */


/* GLOBAL VARIABLES */

char os_name[60];
char dv_loaded=0;
char multitasking_os=0;
char os_type=0;
char major_version;
char minor_version;
int num_ports_loaded=0;
int num_consoles_loaded=0;
extern char system_number[6];
char initial_serial_config_file[]="SERIAL.CFG";
char *serial_config_file=initial_serial_config_file;
const char compile_time[]=__TIME__;
const char compile_date[]=__DATE__;

struct serial_config_info
{
    char node;
    char active;
    char is_dial_in;
    char init_string[60];
    char de_init_string[60];
    unsigned int short baud;
    char board_type;
    unsigned int short io_address;
    unsigned int short digi_lookup_address;
    char int_num;
    char port_number;
    char fixed_dte_speed;
    char rts_cts;
    char os2_com_name[15];
    char verify_node;
    char restrict_level;
    char dummy[32];
} ;

void reboot(void)
{

     /* need to reboot here */

     g_exit(1);
}

struct startup_screen_data_window_struct {
    int x1;
    int y1;
    int x2;
    int y2;
    int foreground;
    int background;
};
#define END_FIELD 1000
#define FULL_SCREEN 1001


struct startup_screen_data_struct {
    int x;
    int y;
    char *text;
    int foreground;
    int background;
};

struct startup_screen_data_window_struct start_win_data[] = {
  {  FULL_SCREEN, 0 , 0 , 0 , 7 , 1},  /* whole screen blue */

 /* copyright notice box */
    {  2 , 4 , 40 , 9 , 7 , 4  },        /* red box */
    {  3 , 10 , 41 , 10 , 7 , 0},        /* bottom shadow */
    {  41 , 5 , 41 , 10 , 7 , 0},        /* right shadow */

 /* startup progress box */
    {  15 , 20 , 65 , 23 , 7 , 4 },      /* red box */
    {  16 , 24 , 66 , 24 , 7 , 0 },      /* bottom shadow */
    {  66 , 21 , 66 , 24 , 7 , 0 },      /* right shadow */

 /* Address box */
    {  52 , 4 , 78 , 10 , 7 , 4  },        /* red box */
    {  53 , 11 , 79 , 11 , 7 , 0},        /* bottom shadow */
    {  79 , 5 , 79 , 11 , 7 , 0},        /* right shadow */

    {  END_FIELD }};

struct startup_screen_data_struct start_scrn_data[] = {
  /* copyright box */
     { 4, 5, version_title, 15 , 4},
     { 4, 6, "(C) Copyright 1993 by DCFG Software", 15 , 4},
     { 4, 7, "All rights Reserved", 15 , 4},

  /* address box */
     { 54, 5, "DCFG Software", 15 , 4},
     { 54, 6, "P.O. Box #2721", 15 , 4},
     { 54, 7, "Glenview, IL 60025", 15 , 4},
     { 54, 8, "USA", 15 , 4},

  /* startup progress box */
     { 16,21, "Percentage Complete: ", 15 , 4 },
     { 29,22, "Status: ", 15 , 4 },
     { 0 , 0 , NULL , 0 , 0 }};

void paint_window(struct startup_screen_data_window_struct *wptr)
{
 struct text_info t_info;

   textbackground(wptr->background);
   textcolor(wptr->foreground);
   if (wptr->x1 == FULL_SCREEN)
   {
     gettextinfo(&t_info);
     window(t_info.winleft,t_info.wintop,t_info.winright,t_info.winbottom);
    }
   else
     window(wptr->x1,wptr->y1,wptr->x2,wptr->y2);
   clrscr();
}

void init_gtalk_startup_screen()
{
 struct startup_screen_data_struct *ptr = start_scrn_data;
 struct startup_screen_data_window_struct *wptr = start_win_data;
 struct text_info t_info;

 gettextinfo(&t_info);

 while (wptr->x1 != END_FIELD)
 {
   paint_window(wptr);
   wptr++;
 }

     window(t_info.winleft,t_info.wintop,t_info.winright,t_info.winbottom);

  highvideo();
 while (ptr->text)
 {
   textbackground(ptr->background);
   textcolor(ptr->foreground);
   gotoxy(ptr->x,ptr->y);
   cprintf(ptr->text);
   ptr++;
 }

   textbackground(0);
   textcolor(7);
}

void update_startup_status(char *status_msg)
{
   if (!sys_toggles.system_booting)
     return;

   textbackground(4);
   textcolor(14);

   gotoxy(36,22);
   cprintf("                            ");
   gotoxy(36,22);
   cprintf(status_msg);

   textbackground(0);
}

void update_startup_percentage(int percentage)
{
   if (!sys_toggles.system_booting)
     return;

   textbackground(4);

   gotoxy(36,21);
   highvideo();
   cprintf("%02d%%",percentage);
   lowvideo();

   if (percentage == 100)
     {
        gotoxy(36,22);
        cprintf("                        ");
        gotoxy(36,22);
        textcolor(2);
        highvideo();
        cprintf("Ready (Press Return to Login)");
        lowvideo();

     }

   textbackground(0);
}


void loadChannelVars(void)
{
 int loop,loop2;
 int cur_num=0,at_eof=0;
 FILE *fileptr;
 struct channel_information *ch_ptr;


    if ((fileptr=fopen(CHANNEL_CONFIG_FILE,"rb")))
      {
        while ((cur_num<MAX_CHANNELS) && (!at_eof))
         {
           fseek(fileptr,(sizeof(struct channel_information)*cur_num),SEEK_SET);

           if (!fread(&channels[cur_num].default_cfg,sizeof (struct channel_information),1,fileptr))
             at_eof=1;
           else
             cur_num++;
         }

         sys_info.max_channels=cur_num-1;

         if(fclose(fileptr))
          perror(CHANNEL_CONFIG_FILE);

      /* file read successfull */
      if (cur_num)
        {
         for (loop=0;loop<cur_num;loop++)
           channels[loop].current_cfg=channels[loop].default_cfg;


         return;
        }

      }


    perror(CHANNEL_CONFIG_FILE);
    printf("* Channel config file did not load - Creating New");

        for(loop=0;loop<MAX_CHANNELS;loop++)
      {

        ch_ptr=&channels[loop].default_cfg;

        ch_ptr->priority=255;
        ch_ptr->anonymous=0;
        ch_ptr->rot_messages=1;
        ch_ptr->invite=0;
        ch_ptr->allow_moderation=1;
        ch_ptr->allow_channel_messages=1;
        for (loop2=0;loop2<MAX_THREADS-1;loop2++)
         {
           ch_ptr->invited_users[loop2]=-1;
         }
        sprintf(ch_ptr->title,"Channel %d",loop);

       channels[loop].current_cfg=*ch_ptr;

      }

      strcpy(channels[1].current_cfg.title,"Main");
      strcpy(channels[1].default_cfg.title,"Main");


}


void load_serial_config_info(void)
{
  FILE *fileptr;
  struct serial_config_info portin;
  char at_eof=0;
  int num=0;
  sys_toggles.num_dial_ins=0;


  for(num=1;num<MAXPORTS;num++)
    port[num].active=0;

  num=0;

  printf("Loading Serial Configuration Info\n");
  if (!(fileptr=fopen(serial_config_file,"rb")))
    { log_error(serial_config_file);
      printf("ERROR *** NO '%s' run serialcf.exe (in the CONFIG directory) \n",serial_config_file);
      printf("      *** Type GTALK /? for help \n");
      g_exit(1);
    }

    fseek(fileptr,0,SEEK_SET);

   while ((!at_eof) && (num<MAXPORTS))
   {
     if (!(fread(&portin, sizeof(struct serial_config_info), 1, fileptr)))
       at_eof=1;
     else
       {num++;
#ifdef CONSOLE
        if ((portin.active) && (!portin.board_type))
#else
        if (portin.active)
#endif
          {struct port_info *port_inf=&port[num];
           if (!portin.board_type)
              {printf("Node: %02d <Console>",num);
                num_consoles_loaded++;
               }
           else
             printf("Node: %02d  Speed: % -8u  Board Type: % 3d",num,portin.baud,portin.board_type);
           if (portin.board_type==BOARD_TYPE_OS2_COM)
             printf("  OS/2 Com Name: %s",portin.os2_com_name);

           port_inf->active=1;
#ifndef CONSOLE
           strcpy(port_inf->init_string,portin.init_string);
           strcpy(port_inf->de_init_string,portin.de_init_string);
           strcpy(port_inf->os2_com_name,portin.os2_com_name);
           port_inf->baud_rate=portin.baud;
           port_inf->io_address=portin.io_address;
           port_inf->digi_lookup_address=portin.digi_lookup_address;
           port_inf->port_number=portin.port_number;
           port_inf->dial_in_line=portin.is_dial_in;
           port_inf->board_type=portin.board_type;
           port_inf->fixed_dte_speed=portin.fixed_dte_speed;
           port_inf->rts_cts=portin.rts_cts;
           port_inf->verify_node=portin.verify_node;
           port_inf->restrict_level=portin.restrict_level;


           switch(portin.board_type)
           {
              case BOARD_TYPE_DUMB_DIGI:
              case BOARD_TYPE_SMART_DIGI:
                   printf("Digiboard Board Type Not Supported\n");
                   g_exit(1);
              default:break;
           }

#endif

           port_inf->console=!(portin.board_type);

           if ((!port_inf->console) && (port_inf->dial_in_line))
              sys_toggles.num_dial_ins++;
           printf("\n");
          }
        else
         {
           port[num].active=0;
#ifdef CONSOLE
           printf("Loaded Port Not Active (C)\n");
#else
           printf("Loaded Port Not Active\n");
#endif
         }
       }
   }
   port[0].active = 1;       /* CONSOLE ACTIVE */
   port[0].board_type = 0;   /* CONSOLE TYPE */
   port[0].console =1;
   port[0].dial_in_line= 0 ; /* LOCAL CONSOLE */
   fclose(fileptr);
   num_ports_loaded=num;


};




void loadSystemVars(void)
{   int loop;
    FILE *fileptr;
    struct tm *temp;
    time_t now;

    /* need to read system defaults */

    if ((fileptr=fopen(SYSTEM_CONFIG_FILE,"rb")))
      {
         memset(&sys_info,0,sizeof(sys_info));
         fseek(fileptr,0,SEEK_SET);
         if (!fread(&sys_info,1,sizeof(struct system_information),fileptr))
           {
            perror(SYSTEM_CONFIG_FILE);
            printf("*opened but couldn't read system config file in task.c");
           }
         else
         {
          if(fclose(fileptr))
             perror(SYSTEM_CONFIG_FILE);
          sys_info.last_uptime=sys_info.uptime;
          sys_info.down_time=sys_info.current_time;
          sys_info.uptime=time(NULL);
          sys_info.current_time=sys_info.uptime;
          sys_info.last_num_exceptions_trapped = sys_info.num_exceptions_trapped;
          sys_info.num_exceptions_trapped = 0;
#ifdef COPY_PROTECTION_ON
		  sys_info.max_nodes=0;
#else
		  sys_info.max_nodes=PORTS_WITHOUT_COPY_PROTECTION;
#endif

          return;
         }
	  }
        perror(SYSTEM_CONFIG_FILE);
    printf("* System config file did not load Creating NEW");


#ifdef COPY_PROTECTION_ON
   sys_info.max_nodes=0;
#else
   sys_info.max_nodes=PORTS_WITHOUT_COPY_PROTECTION;
#endif
   sys_info.paging=0;
   sys_info.lock_priority =255;

   strcpy(sys_info.user_edit_password,"");
   strcpy(sys_info.shutdown_password,"");
   strcpy(sys_info.system_name,"GTalk");
   sys_info.system_number=101;
   *sys_info.master_password=0;
   *sys_info.command_toggle_password=0;
   *sys_info.page_console_password=0;
   time(&now);
   sys_info.current_time=now;
   temp=localtime(&now);
   sys_info.this_month_number=temp->tm_mon;
   sys_info.last_month_number=temp->tm_mon;

   sys_info.max_channels=8;
   sys_info.calls.total=0;
   sys_info.day_calls.total=0;
   sys_info.month_calls.total=0;
   sys_info.last_month_calls.total=0;
   sys_info.last_num_exceptions_trapped = 0;
   sys_info.num_exceptions_trapped = 0;
   sys_info.paging=1;

   for(loop=0;loop<10;loop++)
   {
	 sys_info.calls.baud[loop]=0;
	 sys_info.day_calls.baud[loop]=0;
   }
}

void init_input_buf(int portnum);

void pre_init_vars(void)
{
   int loop,loop2,count;

   for (count=0;count<MAXPORTS;count++)
	{ init_input_buf(count);
	  port[count].active=0;
	}

   sys_toggles.system_update=0;
   sys_toggles.logins_disabled=0;
   sys_toggles.calls_updated = 0;

	sys_toggles.is_validated=0;

  init_tswitch_lookup();

  for (loop=0;loop<MAX_THREADS;loop++)
	{
	  abuf_status[loop].active = 0;
	  abuf_status[loop].abuffer = 0;
	  line_status[loop].connect=0;
	  line_status[loop].online=0;
	  line_status[loop].transfer=0;

 //     for (loop2=0;loop2<sizeof(struct abuf_type);loop2++)
 //       *((char *)&abuf_status[loop]+loop2)=0;
	 }

  for (loop=0;loop<MAXPORTS;loop++)
	{
	 //  line_status[count].online = 0;

      user_lines[loop].user_info.number=-1;

//      for (loop2=0;loop2<sizeof(struct user_data);loop2++)
//         *((char *)&user_lines[loop]+loop2)=0;

//      for (loop2=0;loop2<sizeof(struct u_parameters);loop2++)
//          *((char *)&user_options[loop]+loop2)=0;

	}


	/* take checksum */

/*  sys_toggles.checksum=checksum_system(); */
  sys_toggles.checksum_failed=0;

  sys_toggles.shutdown_system=0;
}

void print_software_startup(void)
{
	printf("\n%-60s %s\n","",ginsutalk_line);
	printf("%-60s %s\n",ginsutalk_line,po_box_line);
	printf("%-60s %s\n",by_line,glenview_il);
	printf("%-60s\n",copyright_mesg);
	printf("\n%s\n",system_startup);
}
void print_system_identification(void)
{
	printf("\nSystem Number: %02u    Nodes Loaded: %d   Con: %d   Registered: %d\n",sys_info.system_number,
				num_ports_loaded,num_consoles_loaded,sys_info.max_nodes);

	if (*sys_info.system_name)
	   printf("System Name: %s\n",sys_info.system_name);
	else
	   { printf("System Name: None Set             ");
		 sprintf(sys_info.system_name,"GinsuTalk #%02u",sys_info.system_number);
		 printf("Defaulting To: %s\n",sys_info.system_name);
	   }
	if (dv_loaded)
	{
	  printf("Multitask Environment: ");
	  if (dv_loaded)
		printf("DesqView\n");
	}



}

void print_stupid_shit(void)
{
	  printf("Software Version Not Valid\n");
	  printf("Contact DCFG Enterprises at (708)998-0008 , 2400 baud\n");
	  printf("For More Information\n");

}

char have_ems=0;
char have_com=0;
char have_local_os2_video=0;
char have_key_int=0;
char have_error=0;
char have_break=0;
int  old_break_value;

void de_allocate_resources(void)
{

  // there is "have_error" but it does
  // not seem to NEED to be deallocated
  de_init_stdio();

/*   if (have_break) setcbrk(old_break_value); */

/*   if (have_ems) deallocate_ems(); */

  if (have_local_os2_video)
     end_local_os2_video();
  if (have_com)
     end_com();                /* kill all the com ports at the end */

}

char *find_arg(char **argv,char *arg)
{
	char **temp=argv;

	while (*temp)
	{
	 if (!strcmp(*temp,arg))
		  return *temp;
	 temp++;
	}

  return 0;
}

char *find_file_name_arg(char **argv)
{
	char **temp=argv+1;

	while (*temp)
	{
	 if (((**temp>=0x30) && (**temp<=0x5a))
			   || ((**temp>=0x61) && (**temp<=0x7a))
			   || (**temp=='\\'))
		  return *temp;
	 temp++;
	}

  return 0;
}

void conv_argv_to_upper(char **argv)
{
  char **temp=argv;
  char *temp2;

  while (*temp)
  { temp2=*temp;

	while (*temp2)
	 {
	 *temp2=toupper(*temp2);
	 temp2++;
	 }
	// printf("%s\n",*temp);

  temp++;
  }

}

void print_help(void)
{

  deencrypt();  /* deencrypt the strings */
				/* we need the for the help page */
  printf("******************************************************************************\n");
  printf("*                                   GTALK.EXE                                *\n");
  printf("******************************************************************************\n");
  printf("* %-74s *\n","Usage : GTALK <filename> <switches>");
  printf("* %-74s *\n","           <filename>  =  the filename of the serial config");
  printf("* %-74s *\n","                          file you would like to use. Defaults");
  printf("* %-74s *\n","                          to SERIAL.CFG");
  printf("* %-74s *\n","           <switches>     /EMS  = use EMS memory");
  printf("* %-74s *\n","");

  printf("* %-74s *\n",nuke_greenhouse);
  printf("* %-74s *\n",information_question);
  printf("* %-54s %-19s *\n","",ginsutalk_line);
  printf("* %-54s %-19s *\n",company_name,po_box_line);
  printf("* %-54s %-19s *\n",version_title,glenview_il);
  printf("* %-74s *\n",copyright_mesg);
  printf("******************************************************************************\n");

  g_exit(1);
}


void init_os_settings(void)
{

   DosError(FERR_DISABLEHARDERR | FERR_DISABLEEXCEPTION);             /* turn off the error popup */

   init_local_os2_video();  /* this gets the pointer to the LVB and sets up the */
							/* local OS2 video structure , see video.c */
   have_local_os2_video = 1;

   signal(SIGINT,SIG_IGN);   /* ignore the interrupt (control-C) interrupt */

  /* init_except();           /* this sets up the OS/2 exception handlder to
                               log crashes. (except.c) */ */



}

HFILE SaveStdOut=0;
HFILE SaveStdErr=0;

void de_init_stdio(void)
{
  HFILE StdOut = 1;
  HFILE StdErr = 0;

    if (SaveStdOut)
    {
      DosClose(StdOut);
      DosDupHandle(SaveStdOut,&StdOut);
      DosClose(SaveStdOut);
    }
    if (SaveStdErr)
    {
      DosClose(StdErr);
      DosDupHandle(SaveStdErr,&StdErr);
      DosClose(SaveStdErr);
    }
}

void init_stdio(void)
{
  HFILE handle_temp;
  HFILE new_handle;
  ULONG action;
  ULONG pos;
  HFILE StdOut = 1;
  HFILE StdErr = 0;

    if(DosOpen("LOG\\STDOUT.LOG",
             &new_handle,
             &action,
             0,
             FILE_NORMAL,
             OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
             OPEN_ACCESS_WRITEONLY |
             OPEN_SHARE_DENYWRITE,
             0))
    {
        printf("Could not open LOG\\STDOUT.LOG (Is gtalk still running?)\n");
        g_exit(1);
    }
    DosSetFilePtr(new_handle,0, FILE_END, &pos);

    SaveStdOut = 0xFFFF;
    DosDupHandle(StdOut,&SaveStdOut);
    DosDupHandle(new_handle,&StdOut);
    DosClose(new_handle);

    if(DosOpen("LOG\\STDERR.LOG",
             &new_handle,
             &action,
             0,
             FILE_NORMAL,
             OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
             OPEN_ACCESS_WRITEONLY |
             OPEN_SHARE_DENYWRITE,
             0))
    {
        printf("Could not open LOG\\STDERR.LOG (Is Gtalk still running?)\n");
        g_exit(1);
    }
    DosSetFilePtr(new_handle,0, FILE_END, &pos);

    SaveStdErr = 0xFFFF;
    DosDupHandle(StdErr,&SaveStdErr);
    DosDupHandle(new_handle,&StdErr);
    DosClose(new_handle);

/*
     if (DosConnectNPipe(StdOut))
     {
      printf("Pipe Connection Failed\n");
      DosClose(pipe_handle);
      pipe_handle=0;
      return;
     }
 */

/*
      if (DosOpen("\\PIPE\GTDEBUG",&handle_temp,&action,0,
                0,OPEN_ACTION_OPEN_IF_EXISTS,
                OPEN_SHARE_DENYNONE,0))
         {
            g_exit(1);
            return;
         }
     }
 */

}

void allocate_resources(char **argv)
{
  int count;
  int use_ems=0;
  char *temp;
  int system_checksum;

  sys_toggles.system_booting=1;

  init_stdio();
  printf("\n***************** Gtalk Startup *******************\n");
  init_gtalk_startup_screen();
  update_startup_percentage(0);
  mkdir("LOG");
  mkdir("TEMP");


  conv_argv_to_upper(argv);

  if (find_arg(argv,"/?"))
	print_help();

  if (temp=find_file_name_arg(argv))
	{
		printf("Using Alternate Serial Config File : %s",temp);
		serial_config_file=temp;

	}

  pre_init_vars();

  update_startup_status("Init Conversion");
  init_conversion();
  update_startup_percentage(10);

  check_operating_system();

  initMultitask();           /* set up multitasker */

  load_serial_config_info();



  update_startup_percentage(15);

  update_startup_status("Init Alloc");
  init_alloc();

  update_startup_status("Init OS Settings");
  init_os_settings();

  update_startup_percentage(20);

  update_startup_status("Decrypting Strings");
  printf("Decrypting Strings\n");

  deencrypt();  /* deencrypt the strings */


 /* OS/2 Debug */
 /*  printf("Size of struct user_data : %d\n",sizeof(struct user_data)); */

  print_software_startup();

  update_startup_status("Loading Channel Data");
  printf("Loading: Channel Info");
  loadChannelVars();
  printf(",");
  update_startup_percentage(25);

  update_startup_status("Loading System Variables");
  printf(",System Variables");
  loadSystemVars();
  printf(".\n");

  update_startup_percentage(30);


  update_startup_status("File Integrity Check");
  printf("File Integrity Check\n");
  system_checksum = check_system_checksum(*argv);

#ifdef COPY_PROTECTION_ON

  if (!system_checksum)
    g_exit(1);

#else
  sys_toggles.is_validated=1;
  sys_info.max_nodes = num_ports_loaded - num_consoles_loaded;
  if (!(get_serial_number()==0x0000))
    {
      print_stupid_shit();
      g_exit(1);
    }
#endif

  update_startup_percentage(50);

  for (count=sys_info.max_nodes+1;count<MAXPORTS;count++)
   if (port[count].board_type) /* ONLY if it's a serial port */
      port[count].active=0;    /* make it INACTIVE */

  sys_info.max_nodes=sys_info.max_nodes+num_consoles_loaded;

  if (sys_info.max_nodes>=MAXPORTS)
    sys_info.max_nodes=MAXPORTS-1;


  print_system_identification();

  update_startup_percentage(75);

  sprintf(system_number,"%02u",sys_info.system_number);


  update_startup_status("Initializing Display");

  init_display(1);    /* needs to be done FIRST
                         pass this the number of lines for the STATUS
                         bar on the FIRST console */
  update_startup_percentage(100);

  printf("******************************************************\n");
  printf("*******   Operating System: %-18s *******\n",os_name);
  printf("******************************************************\n");


  start_com(sys_info.max_nodes+1); /* init the appropriate ports */
  have_com=1;


/*  create_bar(0); */ /* CREATE A STATUS BAR FOR SCREEN 0 */

 /*
 for (count=0;count<=sys_info.max_nodes;count++)
   if (port[count].active)
	make_task((task_type) ginsu, TASK_STACK_SIZE, count, 1,"LOGIN");
	*/


  server=make_task((task_type) start_server, TASK_STACK_SIZE, MAX_THREADS-1,0,"MSG-SVR");
  timeout_server=make_task((task_type) start_timeout_server,TASK_STACK_SIZE,MAX_THREADS-2,0,"TIMEOUT-SVR");
  pipe_server=make_task((task_type) start_pipe_server,TASK_STACK_SIZE,MAX_THREADS-3,0,"PIPE-SVR");

  sys_toggles.system_booting=0;

  while(!gtalk_exit_now)
   DosSleep(100L);
}

void g_exit(int return_code)
{
 int count=10;

   PTIB ptib; /* thread info block */
   PPIB ppib; /* process info block */
   DosGetInfoBlocks(&ptib,&ppib);

 while (!DosExitCritSec());

 de_allocate_resources();
 gtalk_exit_now=1;

 printf("\nExiting...[  ]%c",8);
 while ((gtalk_exit_now) && (count--))
   {
     printf("%c%c%02d",8,8,count);
     DosSleep(20);
   }
 printf("%c%cDone]\n",8,8);


 /* DosExit(EXIT_PROCESS,return_code); */
 DosKillProcess(0,ppib->pib_ulpid);
}
void auto_reboot_task(void);

void shut_down(char *str,char *name, int portnum)
{
    int count,test;
    char s[15];

    time_t counter;
    time_t counter2;

    sys_toggles.should_reboot=1;

    if (!*str)
     {
        print_cr();
        print_str_cr(" SYNTAX:");
        repeat_chr(' ',9,0);
        print_str_cr("/SHUTDOWN+    Shutdown WITH Reboot");
        repeat_chr(' ',9,0);
        print_str_cr("/SHUTDOWN-    Shutdown WITHOUT Reboot");
        repeat_chr(' ',9,0);
        print_str_cr("/SHUTDOWN*    AUTO-Shutdown in 2 minutes");
        return;
     }

    if (*str=='*')
    {
        print_str_cr("      --> SYSTEM SHUTDOWN <--");
        print_cr();
        if (get_password("Master",sys_info.master_password,1))
           {
             print_sys_mesg("System Auto-Shutdown Initiated");
             make_task((task_type) auto_reboot_task, TASK_STACK_SIZE,
                -1,1,"AUTOSHTDOWN");

           }
        else
           print_sys_mesg("Shutdown ABORTED");
        return;
    }


    switch(*str)
    {
       case '-' : print_str_cr("           [ SHUTDOWN  *NO*  REBOOT ]");
                  break;
       case '+' : print_str_cr("           [ SHUTDOWN  AND  REBOOT ]");
                  print_cr();
                  print_str_cr("--> Not supported under OS/2 yet.");
                  return;
                  break;
    }

    if (!get_password("Master",sys_info.master_password,1))
       {
         print_sys_mesg("Shutdown ABORTED");
         return;
       }
    if (*str=='-')
     {
         print_str_cr("Are you sure you want to *NOT* REBOOT?");
          if (!is_console())
            { print_str_cr(" ALERT: you are *NOT* on a console and this could cause");
              print_str_cr("        the system to NOT start itself again.");
            }
         if (get_password("Master",sys_info.master_password,1))
              sys_toggles.should_reboot=0;
     }

    print_string("--> Saving System Info...");
    save_sys_info_function();
    print_str_cr("<Done>");

    print_cr();
    broadcast_message("### System is Shutting Down ###");
    print_str_cr("### System Shutdown ###");

    print_str_cr("Shutting Down:");
    shutdown_server();
    print_str_cr("               message/task server");
    shutdown_timeout_server();
    print_str_cr("               timeout server");

    delay(2);

    print_cr();
    print_string("               login tasks : ");

    sys_toggles.shutdown_system=1;
    test=0;
    for (count=0;count<MAXPORTS;count++)
     if (count != tswitch)
      if (port[count].active)
      {                     /* is this a working task that's not our own? */
        if (test)
           print_chr('-');
        else
          test++;
        wait_for_death(count);
        kill_task(count);   /* ok, log it off */
        unlog_user(count);
        make_task((task_type) shutdown_task, TASK_STACK_SIZE, count,4,"SHUTDOWN");
        sprintf(s,"%d",count);
        print_string(s);

      };
    print_cr();
    print_sys_mesg("Logging you Out.");
    unlog_user(tswitch);

    print_sys_mesg("Shutting down non-login tasks ");
    for (count=MAXPORTS;count<MAX_THREADS;count++)
     if (count != tswitch)
                   kill_task(count);

    print_sys_mesg("Done...Pausing for Sync.");

    counter2=counter=time(NULL);

    while ((counter2-counter)<5)
     {
      counter2=time(NULL);
     }

     if (sys_toggles.should_reboot)
        print_sys_mesg("Restarting.");
     else
        print_sys_mesg("Exiting.");

    shutdown_node(portnum);

    // if (sys_toggles.should_reboot)
    // for now ALWAYS reboot
    // sys_toggles.should_reboot=1;

    // reboot();


    g_exit(0);

};





void check_operating_system(void)
{

     os_type=OS2;
     multitasking_os=YES;
     major_version=2;
     minor_version=1;
     sprintf(os_name,"OS/2 v%d.x (native)",major_version);
}
