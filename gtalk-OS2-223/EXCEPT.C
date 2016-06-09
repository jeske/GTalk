/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



#define INCL_DOS
#define INCL_DOSEXCEPTIONS
#define INCL_DOSFILEMGR
#define INCL_NOPMAPI
#include <os2.h>
#include "include.h"
#include "gtalk.h"
#include <errno.h>

#include "except.h"
#include "task.h"

#define EXCEPTION_LOG_FILE "LOG\\EXCEPT.LOG"

#include "taskst.h"
extern struct task_struct tasks[MAX_THREADS];
/****************************************************************************/
/* The exception handler gets control and checks for an addressing          */
/* exception.  This is important because the exception handler will also get*/
/* control for other exceptions, i.e. when a page needs to be swapped in    */
/* from the swap file.  If another exception has been found, the exception  */
/* handler returns with XCPT_CONTINUE_SEARCH, which tells OS/2 to pass the  */
/* exception on to the next handler in the list.                            */
/*                                                                          */
/* The exception handler opens a log file.  If this fails, the file pointer */
/* is redirected to stderr.  The registers are then written to the log file */
/* and the process is terminated with DosExit.                              */
/*                                                                          */
/****************************************************************************/

void first_procedure(void);

int init_except(void)
{
   EXCEPTIONREGISTRATIONRECORD er = { NULL, (_ERR *)&ExceptionHandler }; /* To register the handler */
   PCHAR pc = NULL;

   printf("Installing Exception Handler.\n");
   DosSetExceptionHandler (&er);                      /* Add our routine to the chain */

   return 0;
}

void dump_task_info(FILE *fp,time_t now, int portnum)
{
  int loop=0;
  char s[220];

  fprintf(fp, "-------------------------------------------------------\n");
  fprintf(fp, "PID    Num   OS/2 TID  TChar    PAUSED    TIME     NAME\n");

  for (loop=0;loop<MAX_THREADS;loop++)
   if (tasks[loop].status)
    {
      fprintf(fp, "%06u [%02d]   % 4u      %02d        %u  ",tasks[loop].pid,
			   loop,tasks[loop].tid,tasks[loop].taskchar,tasks[loop].paused);

      fprintf(fp, " % 4u:",((unsigned int)((now-tasks[loop].time_created)/3600)));

      fprintf(fp, "%02u:",(((unsigned int)((now-tasks[loop].time_created)/60))%60));

      fprintf(fp, "%02u   ",((unsigned int)(now-tasks[loop].time_created)%60));

      fprintf(fp, "%s\n",tasks[loop].name);
    }

   fprintf(fp, "-------------------------------------------------------\n");
}

void dump_system_list(FILE *fp,time_t now, int portnum)
{
    char n[80];
    char channel_mod = '#';
    int can_see_lurk=test_bit(user_lines[tswitch].class_info.privs,LURK_PRV);
    char n2[80];
    int white_space = 32;
    int loop,index,loop2;
	struct tm *temp;
	int nodes_now_free=nodes_free();

      for (loop=0;loop<num_ports;loop++)
          if (line_status[loop].online)
          {
            if (user_lines[loop].user_info.number<0)
               {
               if (line_status[loop].link)
                 sprintf(n,"%c%02d%c%c%d=%s|*r1%c",user_options[loop].warning_prefix,
                  loop,user_options[loop].staple[0],user_options[loop].location,
                  line_status[loop].mainchannel,user_lines[loop].user_info.handle,
                  user_options[loop].staple[1]);
               else
                 sprintf(n,"%c%02d%c%c%d:%s|*r1%c",user_options[loop].warning_prefix,
                  loop,user_options[loop].staple[0],user_options[loop].location,
                  line_status[loop].mainchannel,user_lines[loop].user_info.handle,
                  user_options[loop].staple[1]);

               if (line_status[loop].link)
               {

                 if (user_options[loop].time==0)
                   sprintf(n2,"LINK/%03u/UNL",
                     (int)(now-line_status[loop].time_online)/60);
                 else
                    sprintf(n2,"LINK/%03u/%03u",
                       (int)(now-line_status[loop].time_online)/60,
                       user_options[loop].time);

               }
               else
               {
                 if (user_options[loop].time==0)
                   sprintf(n2,"%%GST/%03u/UNL",
                     (int)(now-line_status[loop].time_online)/60);
                 else
                    sprintf(n2,"%%GST/%03u/%03u",
                       (int)(now-line_status[loop].time_online)/60,
                       user_options[loop].time);
               }
              }
            else
                if (user_options[loop].time==0)
              {

               sprintf(n,"%c%02d%c%c%d:%s|*r1%c",user_options[loop].warning_prefix,
                  loop,user_options[loop].staple[0],user_options[loop].location,
                  line_status[loop].mainchannel,user_lines[loop].user_info.handle,
                  user_options[loop].staple[1]);

               sprintf(n2,"%c%03u/%03u/%s",channel_mod,user_lines[loop].user_info.number,
               (int)(now-line_status[loop].time_online)/60,"UNL");
              }
            else
               {

               sprintf(n,"%c%02d%c%c%d:%s|*r1%c",user_options[loop].warning_prefix,
               loop,user_options[loop].staple[0],user_options[loop].location,
               line_status[loop].mainchannel,user_lines[loop].user_info.handle,
               user_options[loop].staple[1]);

               sprintf(n2,"%c%03u/%03u/%03u",channel_mod,user_lines[loop].user_info.number,
               (int)(now-line_status[loop].time_online)/60,
               user_options[loop].time);
               }

            filter_ansi(n,n);
            fprintf(fp, n);
            index=(white_space)-strlen(n);
            for(loop2=0;loop2<index;loop2++)
               fprintf(fp , " ");
            fprintf(fp, " ");
            filter_ansi(n2,n2);
            fprintf(fp, n2);


            if (line_status[loop].lurking)
              if (test_bit(user_lines[loop].class_info.privs,LURK_PRV))
                  fprintf(fp, "-LURK");
              else
                  fprintf(fp, "-Forced Lurk");


            if (line_status[loop].silenced)
                fprintf(fp, "-Silenced");
            if (user_options[loop].location!='T')
                fprintf(fp, n,"-%s",user_options[loop].v_location);

            fprintf(fp, "\n");
       }


    temp=localtime(&now);
    strftime(n,79,"--> %a %b %d %Y ",temp);

    fprintf(fp, n);
    str_time(n,79,temp);
    fprintf(fp, n);

    if (user_options[tswitch].width<60)
     { print_cr();
       print_string("-->");
     }
    if (nodes_now_free)
    {
       if (nodes_now_free==1)
           strcpy(n," [1 Node Free]");
        else
           sprintf(n," [%d Nodes Free]", nodes_now_free);

    }
    else
       strcpy(n," [ SYSTEM FULL ]");

    fprintf(fp, "%s\n",n);
}

void broadcast_node_crash(int portnum)
{
 char string[100];

 sprintf(string,"--> Gtalk Memory Protection Fault on Node [%02d]",portnum);
 aput_into_buffer(server,string,255,12,tswitch,0,0);

}

ULONG APIENTRY ExceptionHandler (PEXCEPTIONREPORTRECORD pERepRec, /* Main exception handler */
                      PEXCEPTIONREGISTRATIONRECORD pERegRec,
                      PCONTEXTRECORD pCtxRec, PVOID p)
{
   FILE * fp;
   int portnum=0xFFFF;
   int file_open_flag;
   time_t now;

   sys_info.num_exceptions_trapped;

   switch(pERepRec->ExceptionNum)
    {
      case GT_XCPT_INVALID_TID_LOOKUP:
              if ((fp = fopen (EXCEPTION_LOG_FILE, "a+")) == 0)           /* Open the log file */
              {
                 fprintf (stderr, "\nReturn code %ud from fopen for log file\n", errno);
                 fp = stderr;                                       /* On error, reset log to stderr */
                 file_open_flag=0;
              }
              file_open_flag=1;

             /* The following fprintf statements log the error message to the file */

              fprintf (fp, "****** Gtalk Tswitch Lookup Violation Occured ******\r\n");

              fprintf (fp, "(tswitch not available)\n");

              fprintf (fp, "Registers in hex:\n");
              fprintf (fp, "EAX = %8.8X, EBX = %8.8X, ECX = %8.8X, EDX = %8.8X\r\n",
                       pCtxRec->ctx_RegEax, pCtxRec->ctx_RegEbx,
                       pCtxRec->ctx_RegEcx, pCtxRec->ctx_RegEdx);
              fprintf (fp, "ESI = %8.8X, EDI = %8.8X\n\n",
                       pCtxRec->ctx_RegEsi, pCtxRec->ctx_RegEdi);
              if (pCtxRec->ContextFlags & CONTEXT_CONTROL)             /* If control registers are listed */
              {
                 fprintf (fp, "Current instruction pointer:\nCS = %8.8X, EIP = %8.8X Flags = %8.8X\r\n\n",
                          pCtxRec->ctx_SegCs, pCtxRec->ctx_RegEip, pCtxRec->ctx_EFlags);
                 fprintf (fp, "Current stack: SS = %8.8X, ESP = %8.8X, EBP = %8.8X\r\n",
                          pCtxRec->ctx_SegSs, pCtxRec->ctx_RegEsp, pCtxRec->ctx_RegEbp);
              }
              else
                 fprintf (fp, "Control registers not available\n\n");
              if (pCtxRec->ContextFlags & CONTEXT_SEGMENTS)            /* If segment registers are listed */
                 fprintf (fp, "DS = %8.8X, ES = %8.8X, FS = %8.8X, GS = %8.8X\r\n\n\n",
                          pCtxRec->ctx_SegDs, pCtxRec->ctx_SegEs,
                          pCtxRec->ctx_SegFs, pCtxRec->ctx_SegGs);
              else
                 fprintf (fp, "Segment registers not available\n\n");


              fprintf(fp, "Action: Exiting.\n\n");

              if (file_open_flag)                 /* If we opened a file before */
                 fclose (fp);                     /*   then close the log file  */

              /* block */
              {
                PTIB ptib; /* thread info block */
                PPIB ppib; /* process info block */
                DosGetInfoBlocks(&ptib,&ppib);
                DosKillProcess(0,ppib->pib_ulpid);
              }
        break;
      case XCPT_ACCESS_VIOLATION:
              if ((fp = fopen (EXCEPTION_LOG_FILE, "a+")) == 0)           /* Open the log file */
              {
                 fprintf (stderr, "Return code %ud from fopen for log file\n", errno);
                 fp = stderr;                                       /* On error, reset log to stderr */
                 file_open_flag=0;
              }
              file_open_flag=1;

             /* The following fprintf statements log the error message to the file */

              if (pERepRec -> ExceptionAddress == 0)
              {
                fprintf (fp, "****** Gtalk Access violation occurred with NULL location ******\r\n");
                /* block */
                {
                    PTIB ptib; /* thread info block */
                    PPIB ppib; /* process info block */
                    DosGetInfoBlocks(&ptib,&ppib);
                    DosKillProcess(0,ppib->pib_ulpid);
                }
              }


              fprintf (fp, "****** Gtalk Access violation occurred at location 0x%8.8X ******\r\n",
                       pERepRec -> ExceptionAddress);
              fprintf (fp, "****** First Procedure at 0x%8.8X - RelAdd 0x%8.8X ",
                       first_procedure,((unsigned long int)(pERepRec->ExceptionAddress) - (unsigned long int)first_procedure));

              fprintf (fp, "(tswitch: %d)\n",(portnum = tswitch));

              switch(sys_info.exception_debug_level)
              {
                    case 2:
                              now = time(NULL);
                              if (portnum < num_ports)
                                 dump_system_list(fp,now,portnum);
                              dump_task_info(fp,now,portnum);
                              break;
                    default:  break;
              }


              fprintf (fp, "Registers in hex:\n");
              fprintf (fp, "EAX = %8.8X, EBX = %8.8X, ECX = %8.8X, EDX = %8.8X\r\n",
                       pCtxRec->ctx_RegEax, pCtxRec->ctx_RegEbx,
                       pCtxRec->ctx_RegEcx, pCtxRec->ctx_RegEdx);
              fprintf (fp, "ESI = %8.8X, EDI = %8.8X\n\n",
                       pCtxRec->ctx_RegEsi, pCtxRec->ctx_RegEdi);
              if (pCtxRec->ContextFlags & CONTEXT_CONTROL)             /* If control registers are listed */
              {
                 fprintf (fp, "Current instruction pointer:\nCS = %8.8X, EIP = %8.8X Flags = %8.8X\r\n\n",
                          pCtxRec->ctx_SegCs, pCtxRec->ctx_RegEip, pCtxRec->ctx_EFlags);
                 fprintf (fp, "Current stack: SS = %8.8X, ESP = %8.8X, EBP = %8.8X\r\n",
                          pCtxRec->ctx_SegSs, pCtxRec->ctx_RegEsp, pCtxRec->ctx_RegEbp);
              }
              else
                 fprintf (fp, "Control registers not available\n\n");
              if (pCtxRec->ContextFlags & CONTEXT_SEGMENTS)            /* If segment registers are listed */
                 fprintf (fp, "DS = %8.8X, ES = %8.8X, FS = %8.8X, GS = %8.8X\r\n\n\n",
                          pCtxRec->ctx_SegDs, pCtxRec->ctx_SegEs,
                          pCtxRec->ctx_SegFs, pCtxRec->ctx_SegGs);
              else
                 fprintf (fp, "Segment registers not available\n\n");


              if ((portnum < num_ports) && !sys_info.reboot_on_all_exceptions)
                {
                     fprintf(fp, "Action: Killing Task\n\n");
                     switch (sys_info.exception_debug_level)
                     {
                        case 1: print_str_cr_to("ERR: Gtalk Protection Fault",portnum);
                                break;
                        case 2:

                                 /*
                                 print_cr();
                                 print_str_cr("********** Gtalk Protection Fault **********");
                                 print_str_cr("An software error has occured on your node.");
                                 print_str_cr("If possible, please inform the sysops of");
                                 print_str_cr("what you were doing prior to this error.");
                                 print_str_cr("This will aid in the speedy fixing of the");
                                 print_str_cr("bug.");
                                 print_str_cr("********************************************");
                                 print_cr();
                                 */
                        default: break;
                     }

                     broadcast_node_crash(portnum);
                }
              else
                {
                 fprintf(fp, "Action: Exiting.\n\n");
                }

              if (file_open_flag)                 /* If we opened a file before */
                 fclose (fp);                     /*   then close the log file  */


              if ((portnum < num_ports) && !sys_info.reboot_on_all_exceptions)
                  end_task();
              else
              {
                 PTIB ptib; /* thread info block */
                 PPIB ppib; /* process info block */

                 return (XCPT_CONTINUE_SEARCH);
                 DosGetInfoBlocks(&ptib,&ppib);
                 DosKillProcess(0,ppib->pib_ulpid);
              }
         break;

       default :
                 return (XCPT_CONTINUE_SEARCH);
   }

    return XCPT_CONTINUE_SEARCH;
}

