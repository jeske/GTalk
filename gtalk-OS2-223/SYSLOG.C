
/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* SYSLOG.C */
#define INCL_DOS
#include <os2.h>

#include "include.h"
#include "gtalk.h"
#include "com.h"


#define KILL_LOG_FILE "LOG\\KILL.LOG"
#define ERROR_LOG_FILE "LOG\\ERROR.LOG"


#undef PRINT_DEBUG

void mark_user_log_on(void)
{
   FILE *fileptr;
   time_t tim;
   struct tm time_now;
   int flag = islocked(DOS_SEM);

   if (!flag) lock_dos(491);
   fileptr=g_fopen(USER_ENTER_LOG_FILE,"a","IO#1"); /* dos error */
   if (!fileptr)                    /* open the log */
    {
      if (!flag) unlock_dos();
      log_error(USER_ENTER_LOG_FILE);
      return;
    };

   time(&tim);                      /* insert the right time */
   time_now = *localtime(&tim);
   fprintf(fileptr,"%02d/%02d/%02d %02d:%02d:%02d | [%02d]#%03d:%s\n",time_now.tm_mon,time_now.tm_mday,
                                    time_now.tm_year,time_now.tm_hour,time_now.tm_min,
                                    time_now.tm_sec,tswitch,
                                    user_lines[tswitch].user_info.number,
                                    user_lines[tswitch].user_info.handle);
   g_fclose(fileptr);                 /* close the log */
   if (!flag) unlock_dos();
};

/* this routine logs an error */
/* the parameter filename is normally a string which is the filename */
/* on which to check an error. if the first character is an asterisk, */
/* however, the whole string filename will be put verbatim into the */
/* error log */

void log_error(const char *filename)
 {
   FILE *fileptr;
   time_t tim;
   struct tm time_now;
   char s[100];
#ifdef PRINT_DEBUG
   int portnum = tswitch;

   if (portnum >= num_ports)
       portnum = 0;
   else
   if (!port[portnum].active)
       portnum = 0;
#endif

   if (!strncmp((char *)filename,(char *)"TEMP",4))
	 return;  // Dont log errors with temp files

   if (*filename=='*')              /* if the filename is a asterisk */
      sprintf(s,"%s\n",filename+1);   /* use it directly */
   else
     sprintf(s,"Err: %s",_strerror(filename));  /* otherwise use the */

#ifdef PRINT_DEBUG
   print_string("log_error(): ");
   print_str_cr(s);
#endif

   fileptr=g_fopen(ERROR_LOG_FILE,"a","IO#2");              /* dos error */

   if (!fileptr)                    /* open the error log */
	  return;
   time(&tim);                      /* insert the right time */
                                    /* add the time and the line to the file */
   time_now = *localtime(&tim);
   fprintf(fileptr,"%02d/%02d/%02d %02d:%02d:%02d [%02d]| %s",time_now.tm_mon+1,time_now.tm_mday,
                                    time_now.tm_year,time_now.tm_hour,time_now.tm_min,
                                    time_now.tm_sec,tswitch,s);
   g_fclose(fileptr);                 /* close the error log */

 };


/* LOG TO FILE */
/* this file logs a line to a log file */


void log_event(const char *filename,char *event)
 {
   FILE *fileptr;
   time_t tim;
   int flag = islocked(DOS_SEM);
   char s[100];


   if (!flag) lock_dos(493);

   fileptr=g_fopen(filename,"a","IO#3");              /* dos error */
   if (!fileptr)                    /* open the error log */
    {
      if (!flag) unlock_dos();
      return;
    };


                                    /* insert User Number, and NODE info */

   time(&tim);                      /* insert the right time */
   strftime(s,33,STRFTIME_CONFIG_STRING,localtime(&tim));
   fprintf(fileptr,"%s | ",s);

   fprintf(fileptr,"#%03d (%02d)",user_lines[tswitch].user_info.number,tswitch);

   fprintf(fileptr,"%s\n",event);         /* add the line to the file */

   g_fclose(fileptr);                 /* close the error log */
   if (!flag) unlock_dos();
 };

/* LOG TO FILE */
/* this file logs a line to a log file */


int log_system_event(const char *filename,char *event)
 {
   FILE *fileptr;
   time_t tim;
   char s[100];


   fileptr=g_fopen(filename,"a","IO#3");              /* dos error */
   if (!fileptr)                    /* open the error log */
    {
      return (1);
    };

   time(&tim);                      /* insert the right time */
   strftime(s,33,STRFTIME_CONFIG_STRING,localtime(&tim));
   fprintf(fileptr,"%s | ",s);

   fprintf(fileptr,"%s\n",event);         /* add the line to the file */

   g_fclose(fileptr);                 /* close the error log */
   return (0);
 };

void log_kill(int node,int who_killed)
 {
   FILE *fileptr;
   time_t tim;
   int flag = islocked(DOS_SEM);
   char n1[100],n2[100],n3[100];
   char temp[20];
   char type[5];

   if (!flag) lock_dos(494);

   fileptr=g_fopen(KILL_LOG_FILE,"a","IO#3");              /* dos error */
   if (!fileptr)                    /* open the error log */
    {
      if (!flag) unlock_dos();
      return;
    };

   if (user_lines[who_killed].user_info.number<0)
    { if (line_status[who_killed].link)
        strcpy(type,"LINK");
      else
        strcpy(type,"%GST");
    }
   else
     sprintf(type,"#%03d",user_lines[who_killed].user_info.number);

   strncpy(temp,user_options[who_killed].noansi_handle,15);
   temp[16]=0;


                                    /* insert User Number, and NODE info */
   sprintf(n1,"(%02d) %s:%c%s%c",who_killed,type,user_options[who_killed].staple[2],
                 temp,user_options[who_killed].staple[3]);

   if (user_lines[node].user_info.number<0)
    { if (line_status[node].link)
        strcpy(type,"LINK");
      else
        strcpy(type,"%GST");
    }
   else
     sprintf(type,"#%03d",user_lines[node].user_info.number);

   strncpy(temp,user_options[node].noansi_handle,15);
   temp[16]=0;

                                     /* insert User Number, and NODE info */
   sprintf(n2,"(%02d) %s:%c%s%c",node,type,user_options[node].staple[2],
                 temp,user_options[node].staple[3]);


   time(&tim);                      /* insert the right time */
   strftime(n3,22,STRFTIME_CONFIG_STRING,localtime(&tim));

   fprintf(fileptr,"%-28s %-28s %s",n1,n2,n3);


   g_fclose(fileptr);                 /* close the error log */
   if (!flag) unlock_dos();
 };


void last_ten_kills(int num_last,int portnum)
 {
   FILE *fileptr;
   char s[80];
   int num;
   int not_abort = 1;
   int key;

   print_cr();
   sprintf(s,"Last %d kills",num_last);
   print_str_cr(s);
   print_file("TEXT\\KILL.HDR");
   num_last++;
   print_cr();

   lock_dos(495);
   if ((fileptr=g_fopen(KILL_LOG_FILE,"rb","FILES#4"))==0)       /* open the user log */
    {
      log_error(KILL_LOG_FILE);
      unlock_dos();
      return;
    };
   num = 1;
   s[78]=0;
   while (not_abort && (num<num_last))
    {
      fseek(fileptr,-78*(long int)(num),SEEK_END);
      not_abort = (ftell(fileptr) != 0);
      if (not_abort)
       {
       //  sprintf(s,"%2d: ",num);
         unlock_dos();
      //   print_string(s);
         lock_dos(496);
         if (!fread(s,1,78,fileptr))
            not_abort=0;
         unlock_dos();
         print_str_cr(s);
         key = get_first_char(tswitch);
         if ((key == 3) || (key == 27))
          {
            int_char(portnum);
            not_abort = 0;
          };
         lock_dos(497);
       };
      num++;
    };
   g_fclose(fileptr);
   clear_call_on_logoff();
   unlock_dos();
   print_cr();
 };




