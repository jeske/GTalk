#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <dir.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include "jumptbl.h"
#include "glmdef.h"

struct startblock ourblock =
{
  LD_STARTID,                   /* startup string */
  0,                            /* glm version number */
  sizeof(struct startblock),    /* size of header */
  "MEBOT",                      /* name of glm */
  CAN_BE_SHARED,                /* shared or non-shared */
  0                             /* dummy location to */
                                /* fill in length */
};

int far ginsu_main(void)
{
  int running = 1;
  int type,temp1,temp2,temp3,channel,sentby;
  char s[451];
  print_cr();
  print_str_cr("Welcome to BOTME");
  print_str_cr("The FIRST ever interactive C bot for Gtalk");
  initabuffer(2048);
  register_bot("BOTME");
  change_my_info_line("The First Gtalk Bot ever");

  while (running)
  {
      if (aget_abuffer(&sentby, &channel, s, &type, &temp1, &temp2,&temp3))
      {
       s[420]=0;
       switch (type)
        {
          case 1: print_str_cr(s);
                  aput_into_buffer(temp1,"--> BotMe message: ",0,1,tswitch,temp1,0);
                  aput_into_buffer(temp1,s,0,1,tswitch,temp1,0);
                  if (*s=='*') running = 0;
                  break;
        } /* END FIRST */
    }
    next_task();
  }
  unregister_bot_myself();
  return (1);
}

