/*
 *
 *  ddial.h
 */

#ifndef _GT_DDIAL_H
#define _GT_DDIAL_H

extern list ddial_state_list;
extern state_machine ddial_link_state_list[];

int client_ddial_link_process(abuffer *abuf, char *message);
int client_ddial_channel_process(abuffer *abuf, char *message);
int ddial_receive_message(abuffer *abuf, char *message);
int client_ddial_process(abuffer *abuf,char *message);

#endif   /* _GT_DDIAL_H */
