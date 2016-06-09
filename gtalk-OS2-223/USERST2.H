/* USERST2.H */

#ifndef GT_USERST2_H
#define GT_USERST2_H

#include "userst.h"


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


struct u_parameters
{
    int add_time_counter;
    unsigned char privs[MAX_NUM_PRIV_CHARS];
    unsigned char toggles[10];
    unsigned char system[10];
    int priority;
    char staple[4];
    char noansi_handle[50];
    unsigned int time;
    unsigned long int time_sec;           /* time online */
    unsigned long int time_warning_sec;   /* time until warning */
    char warning_prefix;
    unsigned int added_time;
    time_t login_time;
    int warnings;
    char location;
    char v_location[15];
    int squelched[MAXPORTS];
    int squelch_all;
    int neutralized[MAXPORTS];
    int neutralize_all;
    unsigned char width;
    a_class class_name;

} ;


#endif /* GT_USERST2_H */
