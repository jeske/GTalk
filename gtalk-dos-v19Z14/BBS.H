


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* BBS.H */

#define BBS_MAX 50
#define NUM_BBS_PRIVS 20

void bbs_system(const char *str,const char *name, int portnum);

struct bbs_user_account
{
   unsigned long int newscan_pointer;                       /* 4   4   */
   char is_moderator;                                       /* 1   5   */
   char moderator_level;                                    /* 1   6   */
   char newscan;                                            /* 1   7   */
   char privs[NUM_BBS_PRIVS];                               /* 20  27  */
   char dummy[229];                                         /* 229 256 */
}

