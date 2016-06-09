

/*
 *
 * include/channel.h
 *
 * common channel stuff 
 */

#ifndef GT_INCL_CHANNEL_H
#define GT_INCL_CHANNEL_H

#define CHANNEL_NAME_LEN 20
#define CHANNEL_TITLE 40

#define T_CH_MESSAGE 1
#define T_CH_JOIN 2
#define T_CH_LEAVE 3
#define T_CH_BANNED 4
#define T_CH_CHANNELERROR 5
#define T_CH_INVITED 6
#define T_CH_JOINCHANNEL 7
#define T_CH_LEAVECHANNEL 8
#define T_CH_NOBELONG 10
#define T_CH_NOCHANNEL 11
#define T_CH_NOMODERATE 12
#define T_CH_NOPERMCREATE 13
#define T_CH_NOTINVITED 14
#define T_CH_NOWRITE 15
#define T_CH_PRIVATE 16
#define T_CH_UNBANNED 17
#define T_CH_UNINVITED 18
#define T_CH_BINARY 19
#define T_CH_SETPERM 20
#define T_CH_SETCHAN 21
#define T_CH_BYE 22
#define T_WALL 23
#define T_WALLA 24
#define T_MSG_LURK 25
#define T_LOGIN_LURK 26

typedef int (* channel_func)(abuffer *buf, char *message);

#endif












