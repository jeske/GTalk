
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - command.h
 *
 */

#ifndef _GTALK_COMMAND_H
#define _GTALK_COMMAND_H

#include "common.h"

int cmd_change_handle(com_struct *com, char *string);
int cmd_quit_loop(com_struct *com, char *string);
int cmd_do_login_subshell(com_struct *com, char *string);
int cmd_do_user_shell(com_struct *com,char *string);
int cmd_do_user_menu(com_struct *com, char *string);
int cmd_enter_bbs(com_struct *com, char *string);
int edit_file(com_struct *com, char *string);
int cmd_system_list(com_struct *com,char *string);
int cmd_long_system_list(com_struct *com,char *string);
int cmd_command_help(com_struct *com,char *string);
int cmd_toggle_ansi_color(com_struct *com, char *string);
int cmd_toggle_high_ascii(com_struct *com, char *string);
int cmd_print_user_info(com_struct *com, char *string);
int cmd_print_user_last_info(com_struct *com, char *string);
int cmd_time_command(com_struct *com,char *string);
int cmd_show_gtalk_info(com_struct *com,char *string);
int cmd_send_private_message(com_struct *com,char *string);
int cmd_page_node(com_struct *com, char *string);
int cmd_kill_node(com_struct *com,char *string);
int cmd_relog_node(com_struct *com,char *string);
int change_to_channel(char *name);
int cmd_change_channel(com_struct *com,char *string);
int cmd_give_time(com_struct *com, char *string);
void link_main_loop(void);
int cmd_do_user_config(void);

int cmd_channel_invite(com_struct *com, char *string);
int cmd_channel_ban(com_struct *com, char *string);
int cmd_give_moderator(com_struct *com, char *string);
int cmd_allow_write(com_struct *com, char *string);
int cmd_channel_lock(com_struct *com, char *string);
int cmd_lineout_counter(com_struct *com, char *string);
int cmd_device_list(com_struct *com, char *string);
int cmd_make_access(com_struct *com, char *string);
int cmd_reset_device(com_struct *com, char *string);

int exec_user_program(char *progname, node_struct *a_node,
		      unsigned long int options);
int cmd_change_passwd(com_struct *com, char *string);
int cmd_print_caller_log(com_struct *com, char *string);
int cmd_print_sysinfo(com_struct *com, char *string);
int cmd_print_message_box(com_struct *com,char *string);
int cmd_lock_system(com_struct *com,char *string);


void print_account_finance_info(struct unique_information_struct *usr,
				struct class_defined_data_struct *usrcls);
#endif  /* _GTALK_COMMAND_H */

