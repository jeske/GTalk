/* event.h */

#ifndef GT_EVENT_H
#define GT_EVENT_H

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


void clear_all_old_pids_event(void);

void console_alarm_event(void);
void checksum_system_event(void);
void link_node_event(void);
void g_link_node_event(void);
void kill_node_event(void);
void relog_node_event(void);

#endif /* GT_EVENT_H */
