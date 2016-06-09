
/* GLNKST.h */
/* GTALK LINK Structures */

#ifndef GT_GLINKST_H
#define GT_GLINKST_H


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


struct system_packet_struct {
    unsigned int        system_number;
    unsigned int        software_type;
    unsigned char       channel_activity[8];
    time_t              last_contact;
    unsigned int        last_packet_num;
    unsigned char       local_node;
    char                connected;
    unsigned char       hops;
    char                phone_number[20];
    char                system_name[40];
    unsigned int        num_nodes;
    unsigned int        num_nodes_in_use;
    time_t              last_ping_time;
    unsigned char       last_ping_hops;
    unsigned char       bitfields[20];
    long int            time_adjust_seconds;
    char                private_message_command[40];
};

/* bitfields : */
#define CURRENTLY_AVAILABLE         0
#define ACCEPTS_EMAIL               1

struct file_based_system_struct {
    struct system_packet_struct netinfo;
};

struct sys_list_entry {
    char    node;
    char    handle[HANDLE_SIZE];
    /* add alias field */
    char    staples[4];
    int     time;
    time_t  time_online;
    int     user_number;
    int     channel;
};

struct bot_list_entry {
    char            botname[HANDLE_LEN];
    unsigned char   task_num;
    char            net_service_flags;
    unsigned int    system_number;
    unsigned int    software_type;
};

struct reconstruction_struct {
    int             type;
    int             system_number;
    int             dest_system_number;
    int             len;
    int             last_message_no;
    const char      *buffer_start;
    char            *cur_buffer;
    int             cur_buffer_len;
};

struct system_memory_block {
    struct system_packet_struct     sys;
    struct file_based_system_struct file;
    struct reconstruction_struct    packet;
    char used;
    int known_num_users;
    int sys_list_table_size;
    struct sys_list_entry           *users;
};

struct system_glink_struct {
    struct system_memory_block  **net_systems;
    unsigned int                net_systems_table_size;
    unsigned int                num_net_systems;
    struct system_memory_block  my_system;
    struct bot_list_entry       *net_bots;
    unsigned int                net_bots_table_size;
    unsigned int                num_net_bots;
    char used;
};

#endif
