/* GLINK.h */


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


#include "GLNKST.H"


                                                /* LOCAL abuf commands */

                                             /*     DATA1           DATA2           DATA3           */
#define SEND_SYS_INFO_ABUF           100     /*                                                     */
#define SEND_SYS_LIST_ABUF           101     /*     Destination                                     */
#define SEND_SYS_LIST_ENTRY_ABUF     102     /*     Destination     Which Node                      */
#define DISCONNECT_CMD_ABUF          103     /*     Which Node                                      */
#define SEND_PRIVATE_MESSAGE_ABUF    104     /*     Destination     Which Node                      */
#define REPEAT_PACKET_CMD_ABUF       105     /*                                                     */

#define SEND_SYS_INFO_REQ_ABUF       150     /*     Destination                                     */
#define SEND_SYS_LIST_REQ_ABUF       151     /*     Destination                                     */
#define SEND_SYS_LIST_ENTRY_REQ_ABUF 152     /*     Destination     Which Node                      */
#define SEND_DISCONNECT_MSG_ABUF     153     /*     Which System                                    */
#define SEND_HANDLE_UPDATE_ABUF      154     /*     Channel         Node                            */




#define NET_DESTINATION_ALL             0

                                                /* Net Packet Types */

#define NORMAL_NET_MESSAGE              50
#define PRIVATE_NET_MESSAGE             51
#define LOGIN_NET_MESSAGE               53
#define CHANNEL_MOVE_NET_MESSAGE        54
#define LOGOUT_NET_MESSAGE              73
#define HANDLE_UPDATE_NET_MESSAGE       74


                                                /* System Net Packets */

#define SYSTEM_INFO_NET_MESSAGE         100
#define SYSTEM_LIST_NET_MESSAGE         101
#define SYSTEM_DISCONNECT_NET_MESSAGE   102

                                                /* System Command Packets */
#define SYSTEM_INFO_REQUEST_MESSAGE     150
#define SYSTEM_LIST_REQUEST_MESSAGE     151





#define FULL_PACKET             0
#define MULTI_PACKET_START      1
#define MULTI_PACKET_CONTINUE   2
#define MULTI_PACKET_END        3


/* prototypes */



struct system_memory_block *find_system_in_data(int system_no);


void list_glink_users_on_system_function(int system_number);

