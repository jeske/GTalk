/* CHANNEL.H */

#ifndef GT_CHANNEL_H
#define GT_CHANNEL_H

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



 struct channel_information
 {
   short int priority;
   unsigned char rot_messages;
   unsigned char invite;
   unsigned char anonymous;
   unsigned char allow_channel_messages;
   unsigned char allow_moderation;
   unsigned char lineouts_active;
   char glinked;
   char title[20];
   short int invited_users[MAX_THREADS-1];
 };

 struct channel_stats
 {
    struct channel_information default_cfg;
    struct channel_information current_cfg;
 };

extern struct channel_stats channels[MAX_CHANNELS];



void monitor_channel(char *str, char *name, int portnum);
void clear_channel_semaphores(int portnum);

void add_channel(int portnum, int channel);
void del_channel(int portnum, int channel);
int is_monitoring(int portnum,int channel);

void to_channel(char *str,char *name,int portnum);

int channel_empty(int channel);

void reset_channel(int channel);

extern int max_channels;

int invited(int portnum,int channel);


#endif /* GT_CHANNEL_H */
