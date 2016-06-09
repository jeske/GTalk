

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* GLINK.C */


#define INCL_DOSPROCESS
#include <os2.h>

/* HEADERS */
#include "include.h"
#include "gtalk.h"
#include "glink.h"

//#include "link.h"
//#include "ctype.h"
//#include "useredit.h"
//#include "editor.h"

#include "function.h"

#undef GLINK_DEBUG

/* PROTOTYPES */
void request_system_list(int system_number);


void send_my_system_info(void);
void send_my_system_list(void);
void send_disconnect_for_system(int system_number);
void request_info_for_system(int system_number);
void record_user_for_system(struct sys_list_entry *new_entry,int system_no);

int channel_is_netlinked(int channel);

void abort_glink(void);

void glink_main_loop(char *buffer);
void decode_normal_message_packet(char *begin,int system_no);
void decode_handle_update_packet(char *begin,int system_no);
void decode_channel_move_message_packet(char *begin,int system_no);
void decode_system_info_packet(char *begin,int system_no);
void decode_system_list_packet(char *begin,int system_no);
void decode_system_disconnect_packet(char *begin,int system_no);
void decode_logout_message_packet(char *begin,int system_no);
void compose_logout_message_packet(char *out_packet,int *output_packet_len,int node,int channel);
void compose_private_message_packet(char *out_packet,int *output_packet_len,char *abuf_input,int sentby,int dest_node,int type);
void decode_private_message_packet(char *begin,int system_no);
void decode_login_message_packet(char *begin,int system_no);
void compose_login_message_packet(char *out_packet,int *output_packet_len,int node,int channel);
void construct_header(int pkt_type,int destination,unsigned int packet_len,char *out_buf,unsigned int multi_packet_mode);

struct system_memory_block *find_system_in_data(int system_no);

int decode_header(char *begin,int *type,int *system_no,int *dest_sys_no,int *message_no,unsigned int *len,unsigned int *divide_info);

/* global variables */

#define PACKET_BUFFER_SIZE  (2048)
#define ABUF_GET_SIZE       (STRING_SIZE+200)

char sys_glinks_ready=0;
struct system_glink_struct sys_glinks;
struct system_packet_struct *this_system;

#define HEADER_LEN          13
#define START_STRING_LEN    5
#define CRC_LEN             4

char packet_start[]= "!G!G!";

void list_glink_systems_function(void)
{
    int count=0;
    char s[140];
    char s2[80];
    struct system_packet_struct *temp;
    int is_a_sysop = test_bit(user_options[tswitch].privs,SHUTDOWN_PRV);

    if (!sys_glinks_ready)
      { print_sys_mesg("Glinks Not Active");
        return;
      }

    special_code(1,tswitch);


    print_str_cr("System                                    # Users    On Line");
    repeat_chr('-',65,1);

    temp = &((find_system_in_data(this_system->system_number))->sys);
    if (!temp)
      temp = this_system;

    sprintf(s,"%02d:%s|*r1",temp->system_number,temp->system_name);
    sprintf(s2,"  %02d/%02d     --",temp->num_nodes_in_use,temp->num_nodes);
    print_string(s);
    repeat_chr(' ',(43 - ansi_strlen(s)),0);
    print_str_cr(s2);


    while (count<sys_glinks.num_net_systems)
     {
        temp = &(sys_glinks.net_systems[count]->sys);
        if (((temp->num_nodes) || is_a_sysop) && ((temp->system_number)!=(this_system->system_number)))
            {
              sprintf(s,"%02d:%s|*r1",temp->system_number,temp->system_name);
              sprintf(s2,"  %02d/%02d     ",temp->num_nodes_in_use,temp->num_nodes);
              print_string(s);
              repeat_chr(' ',(43 - ansi_strlen(s)),0);
              print_string(s2);
              if (temp->connected)
                sprintf(s2,"%02d",temp->local_node);
              else
                strcpy(s2,"--");
              print_str_cr(s2);
            }
        count++;
     }
     special_code(0,tswitch);

}

void list_glink_users_on_system_function(int system_number)
{
    int count=0;
    char s[140];
    char s2[60];
    int white_space = 32;
    int tempcount;
    struct system_memory_block *temp;
    int is_a_sysop = test_bit(user_options[tswitch].privs,SHUTDOWN_PRV);

    if (!sys_glinks_ready)
      { print_sys_mesg("Glinks Not Active");
        return;
      }
    temp = find_system_in_data(system_number);
    if (!temp)
     {
        print_sys_mesg("No Such System");
        return;
     }

    special_code(1,tswitch);

    sprintf(s,"|*h1[|*f5%d|*r1|*h1:|*r1 %s|*r1|*h1]|*r1",system_number,temp->sys.system_name);
    print_sys_mesg(s);

    for (count=0;count<temp->known_num_users;count++)
     {
       sprintf(s,"#%02d[T%d:%s|*r1)",temp->users[count].node,temp->users[count].channel,temp->users[count].handle);
       if (temp->users[count].time)
         sprintf(s2,"#%03d/XXX/%03d",temp->users[count].user_number,temp->users[count].time);
       else
         sprintf(s2,"#%03d/XXX/UNL",temp->users[count].user_number);

       tempcount = white_space - ansi_strlen(s);
       print_string(s);
       repeat_chr(' ',tempcount,0);
       print_str_cr(s2);
     }
      special_code(0,tswitch);

//    send_my_system_list();
}


void update_a_user_in_list(int loop)
{
 struct sys_list_entry a_user;

   if ((line_status[loop].online) && (line_status[loop].link<2))
       {
         a_user.node = loop;
         a_user.user_number = user_lines[loop].user_info.number;
         strncpy(a_user.handle,user_lines[loop].user_info.handle,HANDLE_SIZE-1);
         a_user.handle[HANDLE_SIZE-1] = 0;
         a_user.channel = line_status[loop].mainchannel;
         a_user.time    = user_lines[loop].class_info.time;
         record_user_for_system(&a_user,this_system->system_number);
       }
}

void update_my_system_list(void)
{
 int loop;

 for (loop=0;loop<num_ports;loop++)
     update_a_user_in_list(loop);
}

void add_new_block_to_list(struct system_memory_block *new_block)
{
 int count=0;
 int system_number = new_block->sys.system_number;
 int not_found_place = 1;
 int not_at_location = 1;
 int old_table_size;
 int insert_location;
 int new_table_size;
 char *buffer;

 while (sys_glinks.used)
   next_task();
 sys_glinks.used=1;


 if (sys_glinks.num_net_systems == sys_glinks.net_systems_table_size)
  {
    old_table_size = (sys_glinks.net_systems_table_size);
    if (!(old_table_size>>1))
      new_table_size = old_table_size + 5;
    else
      new_table_size = (old_table_size) + (old_table_size >> 1);

    buffer = g_malloc_with_owner(sizeof(struct system_memory_block *) * new_table_size,"SYSTABLE",tswitch,0,0);
    if (!buffer)
     {
      sys_glinks.used=0;
      return;
     }
    memcpy(buffer,sys_glinks.net_systems,(sys_glinks.net_systems_table_size) * sizeof(struct system_memory_block *));
    g_free(sys_glinks.net_systems);
    sys_glinks.net_systems = buffer;
    sys_glinks.net_systems_table_size = new_table_size;

  }



 while ((count<sys_glinks.num_net_systems) && (not_found_place) && (not_at_location))
  {
    if (sys_glinks.net_systems[count]->sys.system_number == system_number)
       not_found_place = 0;
//    else
//    if (sys_glinks.net_systems[count]->sys.system_number > system_number)
//       not_at_location = 0;
    else
       count++;
  }

 /* now we want to insert it here in the list */

 insert_location = count;

 if (not_found_place)
 {
     while ((count<sys_glinks.num_net_systems))
      {  sys_glinks.net_systems[count+1] = sys_glinks.net_systems[count];
         count++;
      }

     sys_glinks.num_net_systems++;
     sys_glinks.net_systems[insert_location] = new_block;
 }

 /* For now, if there was an entry, we'll just stop, and die */

 sys_glinks.used=0;

}

struct system_memory_block *find_system_in_data(int system_no)
{
 int count=0;

 while (sys_glinks.used)
   next_task();
 sys_glinks.used=1;

 while ((count<sys_glinks.num_net_systems))
  {
     if (sys_glinks.net_systems[count]->sys.system_number == system_no)
      {
        sys_glinks.used=0;
        return (sys_glinks.net_systems[count]);
      }
     else
      count++;
  }


 sys_glinks.used=0;
 return (0);
}

void create_new_block_for_info(struct system_packet_struct *new_info)
{
    char *new_buffer;
    int  num_nodes = new_info->num_nodes;
    struct system_memory_block *new_block;
    char s[30];

    sprintf(s,"GNK%03d",new_info->system_number);
    new_buffer = g_malloc_with_owner(sizeof(struct system_memory_block) + (num_nodes * sizeof(struct sys_list_entry)) + PACKET_BUFFER_SIZE,s,tswitch,0,0);
    if (!new_buffer)
       return;
    
    new_info->local_node                        =   tswitch;
    new_info->connected                         =   1;
    new_block                                   =   (struct system_memory_block *)new_buffer;
    (char *)(new_block->packet.buffer_start)    =   (new_buffer + sizeof(struct system_memory_block));
    new_block->packet.cur_buffer                =   new_block->packet.buffer_start;
    new_block->packet.cur_buffer_len            =   0;
    new_block->packet.last_message_no           =   0;
    new_block->users                            =   (new_buffer + sizeof(struct system_memory_block) + PACKET_BUFFER_SIZE);
    new_block->known_num_users                  =   0;
    new_block->sys                              =   *new_info;
    new_block->used                             =   0;
    new_block->sys_list_table_size              =   num_nodes;

    add_new_block_to_list(new_block);

}


void delete_system_entry(int system_number)
{
 int count=0;
 int not_found=1;

 while (sys_glinks.used)
   next_task();
 sys_glinks.used=1;

 while ((count<sys_glinks.num_net_systems) && not_found)
  {
     if (sys_glinks.net_systems[count]->sys.system_number == system_number)
       not_found=0;
     else
      count++;
  }
 g_free(sys_glinks.net_systems[count]);

 while ((count<sys_glinks.num_net_systems))
  {
     sys_glinks.net_systems[count] = sys_glinks.net_systems[count+1];
     count++;
  }
  sys_glinks.num_net_systems--;

 sys_glinks.used=0;

 /* return (0); */

}

void disconnect_system(int system_number)
{
  struct system_memory_block *temp;

  temp = find_system_in_data(system_number);

  if (!temp)
    {
      create_new_block_for_info(temp);

      temp = find_system_in_data(system_number);

      if (!temp)
        return;
    }

  temp->sys.connected=0;

}

void record_new_system_information(struct system_packet_struct *new_info)
{
  struct system_memory_block *temp;

  temp = find_system_in_data(new_info->system_number);
  if (!temp)
   {
    create_new_block_for_info(new_info);

      /* since we didn't know about him, chances are, he dosn't know about
         us either */

    send_my_system_info();
   }
  else
   {
     /* update it's current info */
     if ((temp->sys.num_nodes!=new_info->num_nodes) || (!(temp->sys.connected)))
      {
         delete_system_entry(new_info->system_number);
         create_new_block_for_info(new_info);
         send_my_system_info();
         request_system_list(new_info->system_number);
      }

   }
}

void delete_user_for_system(int node_number,int system_number)
{
 struct system_memory_block *temp;
 int count=0;
 int found=0;

 temp = find_system_in_data(system_number);
 if (!temp)
   {
    return;
   }

 while (temp->used)
   next_task();
 temp->used=1;

 while ((count<(temp->known_num_users)) && (!found))
 {  if (temp->users[count].node == node_number)
      found=1;
    else
      count++;
 }

 if (found)
  {
    while (count<(temp->known_num_users))
     {
       temp->users[count] = temp->users[count+1];
       count++;
     }
    (temp->known_num_users)--;
  }

 temp->sys.num_nodes_in_use = temp->known_num_users;
 temp->used=0;

}

void record_user_for_system(struct sys_list_entry *new_entry,int system_no)
{
    struct system_memory_block *temp;
    int count=0;
    int not_found=1;
    int not_at_location=1;
    int insert_location;

    temp = find_system_in_data(system_no);

    if (!temp)
     return;

    while (temp->used)
      next_task();
    temp->used = 1;

    if (temp->known_num_users == temp->sys_list_table_size)
     {
      temp->used = 0;
      return;
     }

    while ((count<temp->known_num_users) && not_found  && not_at_location)
      {
        if (new_entry->node == temp->users[count].node)
          not_found=0;
        else
        if (new_entry->node < temp->users[count].node)
          not_at_location=0;
        else
          count++;
      }

    insert_location = count;


    if (not_found)
      { count = temp->known_num_users;

        while (count>insert_location)
         {
           temp->users[count] = temp->users[count-1];
           count--;
         }

        (temp->known_num_users)++;
      }


    temp->users[insert_location] = *new_entry;
    temp->sys.num_nodes_in_use = temp->known_num_users;

    temp->used=0;
}


struct system_memory_block *make_psudo_entry_for_system(int system_no)
{
 struct system_packet_struct temp_struct;
 char *temp;
 int count=0;

 temp = &temp_struct;
 while (count < sizeof(struct system_packet_struct))
  {
   count++;
   *(temp++) = 0;
  }

 request_info_for_system(system_no);

 temp_struct.system_number = system_no;
 sprintf(temp_struct.system_name,"System #%03d (unregistered)",system_no);
 temp_struct.num_nodes = 0;
 record_new_system_information(&temp_struct);

 return (find_system_in_data(system_no));
}

void line_disconnected(int line_num)
{
 int count=0;

 while (sys_glinks.used)
   next_task();
 sys_glinks.used=1;

 while ((count<sys_glinks.num_net_systems))
  {
     if (sys_glinks.net_systems[count]->sys.local_node == line_num)
        { sys_glinks.net_systems[count]->sys.connected = 0;
          send_disconnect_for_system(sys_glinks.net_systems[count]->sys.system_number);
        }

     count++;
  }

 sys_glinks.used=0;
 return;
}

/*************************************/
/* END OF TABLE MANAGEMENT FUNCTIONS */
/*************************************/


void leave_glink(void)
{
  line_disconnected(tswitch);
  leave();
}

/****************************/
/* PACKET HANDLING ROUTINES */
/****************************/


int merge_multi_packet(struct system_memory_block *temp_block,int type,int system_no,int dest_system_no,int message_no,unsigned int len,unsigned int divide_info,char *begin)
{
   char *cur_buffer;
   int buffer_len = 0;



   if (divide_info==MULTI_PACKET_START)
    {

        temp_block->packet.system_number             =   system_no;
        temp_block->packet.dest_system_number        =   dest_system_no;
        temp_block->packet.len                       =   len;
        temp_block->packet.last_message_no           =   message_no;
        temp_block->packet.type                      =   type;

        buffer_len                                   =   0;
        cur_buffer                                   =    temp_block->packet.buffer_start;

        while (*begin)
         {
           *cur_buffer = *begin;
           cur_buffer++; begin++;
           buffer_len++;
         }

        temp_block->packet.cur_buffer                =   cur_buffer;
        temp_block->packet.cur_buffer_len            =   buffer_len;

    return 1;
    }

    if (!(temp_block->packet.cur_buffer_len))
     {
      return 1;
     }
    if (temp_block->packet.type != type)
     {
      return 1;
     }
    if (temp_block->packet.dest_system_number != dest_system_no)
     {
      return 1;
     }

    //if ((temp_block->packet.last_message_no + 1) != message_no)
    //  return 1;



    temp_block->packet.last_message_no           =    message_no;
    cur_buffer                                   =    temp_block->packet.cur_buffer;
    buffer_len                                   =    temp_block->packet.cur_buffer_len;

    while (*begin)
     {
       *cur_buffer = *begin;
       cur_buffer++; begin++;
       buffer_len++;
     }

    temp_block->packet.cur_buffer                =   cur_buffer;
    temp_block->packet.cur_buffer_len            =   buffer_len;


    if (divide_info==MULTI_PACKET_CONTINUE)
     {
         return 1;
      }
    else
     if (divide_info==MULTI_PACKET_END)
       {

         *(temp_block->packet.cur_buffer)=0;
         (temp_block->packet.cur_buffer)++;
         (temp_block->packet.cur_buffer_len)++;
         return 0;
       }

    return 0;

}

int assemble_frame_from_data(char mode,char **packet,char *frame_buf,int *packet_len,int *frame_len,
                             int max_packet_size,int pkt_type,int destination)
{
  int retval=0;
  int cur_packet_len=0;

    if ((*packet_len<max_packet_size) && !mode)
       {
         construct_header(pkt_type,destination,*packet_len,frame_buf,FULL_PACKET);
         frame_buf += HEADER_LEN;
         (*frame_len) += HEADER_LEN;

         while (*packet_len)
          {
            *frame_buf = **packet;
            frame_buf++; (*packet)++;
            (*packet_len)--; (*frame_len)++;
          }

          return (retval);
        }
      else    /* Make Multi-Packet */
        {
            if (*packet_len<max_packet_size)
              {
                  /* do ENDING packet */
                 construct_header(pkt_type,destination,*packet_len,frame_buf,MULTI_PACKET_END);
                 (frame_buf)+= HEADER_LEN;
                 *frame_len += HEADER_LEN;

                  while (*packet_len)
                  {
                    *frame_buf = **packet;
                    frame_buf++; (*packet)++;
                    (*packet_len)--; (*frame_len)++;
                  }
              }
            else
             {
                retval++;
                if (mode)      /* CONTINUED packet */
                 {
                     construct_header(pkt_type,destination,max_packet_size,frame_buf,MULTI_PACKET_CONTINUE);
                     frame_buf += HEADER_LEN;
                     (*frame_len) += HEADER_LEN;
                  }
                else            /* Start of a NEW multipacket */
                 {
                     construct_header(pkt_type,destination,max_packet_size,frame_buf,MULTI_PACKET_START);
                     frame_buf    += HEADER_LEN;
                     (*frame_len) += HEADER_LEN;
                 };

                    while (cur_packet_len<max_packet_size)
                     {
                       *frame_buf = (**packet);
                       frame_buf++; (*packet)++;
                       (*packet_len)--; (*frame_len)++;
                       cur_packet_len++;
                     }

             }
        }

   return (retval);

}


unsigned int get_crc(char *begin)
{
 unsigned int crc_value;

 read_int(&begin,&crc_value,NULL);
 return (crc_value);;
}




void broadcast_glink_abuf(char *string,int channel,int type,int data1,int data2,int data3)
{
  int count;

  for (count=0;count<sys_info.max_nodes;count++)
  if ((line_status[count].link==2) && (count!=tswitch))
    {
     aput_into_buffer(count,string,channel,type,data1,data2,data3);
    }
}

void broadcast_glink_packet(char *begin)
{
  int count;

  for (count=0;count<sys_info.max_nodes;count++)
  if ((line_status[count].link==2) && (count!=tswitch))
    {
     aput_into_buffer(count,begin,0,REPEAT_PACKET_CMD_ABUF,0,0,0);
    }
}

void process_packet(char *packet,int packet_len)
{
    char *begin = packet + START_STRING_LEN;
    int type,system_no,dest_system_no,message_no,len,divide_info;
    struct system_memory_block *temp_block;
    unsigned int crc_value;



    crc_value = get_crc(begin);
    begin += (CRC_LEN+1); /* get past the CRC and the '!' */

    if (decode_header(begin,&type,&system_no,&dest_system_no,&message_no,&len,&divide_info))
      return;

#ifdef GLINK_DEBUG
     {
      char s[200];
      sprintf(s,"Pkt hdr - type:%02d from:%02d to:%02d no:%02d len:%02d div:%02d",
            type,system_no,dest_system_no,message_no,len,divide_info);
      print_str_cr_to(s,0);
      print_string_to("pkt: ",0);
      print_str_cr_to(begin,0);
    }
#endif


    begin += HEADER_LEN;

    /* if this message came from us, then toss it */

    if ((system_no==this_system->system_number))
      return;


    if (dest_system_no==NET_DESTINATION_ALL)
      broadcast_glink_packet(packet);


    if ((dest_system_no==this_system->system_number) || (dest_system_no==NET_DESTINATION_ALL))   /* it's bound for US */
     {
          temp_block = find_system_in_data(system_no);

          if (!temp_block)
           {
            temp_block = make_psudo_entry_for_system(system_no);
              if (!temp_block)
                {
                 /* for some reason we coulnd't make a entry for him, so just forget it */
                 return;
                }
           }
          else
          if ((!(temp_block->sys.num_nodes) || (!(temp_block->sys.connected))) && (type!=SYSTEM_INFO_NET_MESSAGE) && (type!=SYSTEM_INFO_REQUEST_MESSAGE))
           {
             /* request system info from system */
#ifdef GLINK_DEBUG
             print_str_cr_to("Got illegal packet",0);
#endif
             request_info_for_system(temp_block->sys.system_number);
           }

          if (divide_info)
            {
                   /* it is a divided packet, so lets put it togeather */
                   if (merge_multi_packet(temp_block,type,system_no,dest_system_no,message_no,len,divide_info,begin))
                      return;
                   begin = temp_block->packet.buffer_start;
            }


          /* if it's a full packet, just deal with it immediately */

          switch (type)
            {
             case  NORMAL_NET_MESSAGE:
                                        decode_normal_message_packet(begin,system_no);
                                        break;
             case  PRIVATE_NET_MESSAGE:
#ifdef GLINK_DEBUG
                                        print_str_cr_to("Got private message",0);
#endif
                                        decode_private_message_packet(begin,system_no);
                                        break;
             case  LOGIN_NET_MESSAGE:
#ifdef GLINK_DEBUG
                                        print_str_cr_to("Got login message",0);
#endif
                                        decode_login_message_packet(begin,system_no);
                                        break;
             case  LOGOUT_NET_MESSAGE:
#ifdef GLINK_DEBUG
                                        print_str_cr_to("Got logout message",0);
#endif
                                        decode_logout_message_packet(begin,system_no);
                                        break;

             case  CHANNEL_MOVE_NET_MESSAGE:
                                        decode_channel_move_message_packet(begin,system_no);
                                        break;

             case  HANDLE_UPDATE_NET_MESSAGE:
                                        decode_handle_update_packet(begin,system_no);
                                        break;

             case  SYSTEM_INFO_NET_MESSAGE:
#ifdef GLINK_DEBUG
                                        print_str_cr_to("Got System Info Message",0);
#endif
                                        decode_system_info_packet(begin,system_no);
                                        break;

             case  SYSTEM_DISCONNECT_NET_MESSAGE:
                                        print_str_cr_to("Got System Disconnect Message",0);
                                        decode_system_disconnect_packet(begin,system_no);
                                        break;

             case  SYSTEM_LIST_NET_MESSAGE:
#ifdef GLINK_DEBUG
                                        print_str_cr_to("Got System List Message",0);
#endif
                                        decode_system_list_packet(begin,system_no);
                                        break;

             case  SYSTEM_INFO_REQUEST_MESSAGE:
#ifdef GLINK_DEBUG
                                        print_str_cr_to("Got System Info Request",0);
#endif
                                        send_my_system_info();
                                        break;

             case  SYSTEM_LIST_REQUEST_MESSAGE:
#ifdef GLINK_DEBUG
                                        print_str_cr_to("Got System List Request",0);
#endif
                                        send_my_system_list();
                                        break;
             default:
                                        break;
            }


     }
    else
     {
#ifdef GLINK_DEBUG
      char s[100];
      broadcast_message("--> Packet for other system");
      sprintf(s,"System #%03d",dest_system_no);
      broadcast_message(s);
#endif
      temp_block = find_system_in_data(dest_system_no);
      if ((temp_block->sys.connected) || (temp_block->sys.local_node!=tswitch))
        {
          aput_into_buffer(temp_block->sys.local_node,packet,0,REPEAT_PACKET_CMD_ABUF,0,0,0);
        }
     }

}


int channel_is_netlinked(int channel)
{
 if (channel>sys_info.max_channels)
   return 0;

 if (channels[channel].current_cfg.glinked)
   return 1;
 return 0;
}

void decode_normal_message_packet(char *begin,int system_no)
{
 int sentby,channel;
 char s[20];

 read_short(&begin,&sentby,NULL);
 read_short(&begin,&channel,NULL);

 if (channel_is_netlinked(channel))
 {
       /* put the normal message into the buffer */
      sprintf(s,"%02d",system_no);
      aput_append_into_buffer(server,channel,0,channel,tswitch,10,2,s,begin);
 }

}

void compose_normal_message_packet(int sentby,int channel,int type,char *out_packet,int *output_packet_len,char *abuf_input)
{

    add_short(&out_packet,sentby,NULL);
    add_short(&out_packet,channel,NULL);
    (*output_packet_len)+=2;

    (*output_packet_len) += sprintf(out_packet,"%s",abuf_input);

};

void decode_private_message_packet(char *begin,int system_no)
{
 int sentby,dest_node;
 char s[20];
#ifdef GLINK_DEBUG
 char shit[100];
#endif

 read_short(&begin,&sentby,NULL);
 read_short(&begin,&dest_node,NULL);

 /* put the private message into the buffer */
 sprintf(s,"|*f1P|*r1%02d",system_no);

#ifdef GLINK_DEBUG
 sprintf(shit,"P bound for: %d",dest_node);
 print_str_cr_to(shit,0);
#endif

 aput_append_into_buffer(dest_node,0,1,0,tswitch,10,2,s,begin);

}


void compose_private_message_packet(char *out_packet,int *output_packet_len,char *abuf_input,int sentby,int dest_node,int type)
{
#ifdef GLINK_DEBUG
    char shit[100];
#endif
    add_short(&out_packet,sentby,NULL);
    add_short(&out_packet,dest_node,NULL);
    (*output_packet_len)+=2;

#ifdef GLINK_DEBUG
    sprintf(shit,"out P bound for %d",dest_node);
    print_str_cr_to(shit,0);
#endif

    (*output_packet_len) += sprintf(out_packet,"%s",abuf_input);

};


void compose_login_message_packet(char *out_packet,int *output_packet_len,int node,int channel)
{

  add_short(&out_packet,node,NULL);
  (*output_packet_len)++;
  add_short(&out_packet,channel,NULL);
  (*output_packet_len)++;
  add_medium(&out_packet,user_lines[node].user_info.number,NULL);
  (*output_packet_len)+=2;
  add_medium(&out_packet,user_options[node].time,NULL);
  (*output_packet_len)+=2;

  *output_packet_len += sprintf(out_packet,"%s",user_lines[node].user_info.handle);

}

void compose_logout_message_packet(char *out_packet,int *output_packet_len,int node,int channel)
{

  add_short(&out_packet,node,NULL);
  (*output_packet_len)++;
  add_short(&out_packet,channel,NULL);
  (*output_packet_len)++;

  *output_packet_len += sprintf(out_packet,"%s",user_lines[node].user_info.handle);

}

void compose_handle_update_packet(char *out_packet,int *output_packet_len,int node,int channel)
{

  add_short(&out_packet,node,NULL);
  (*output_packet_len)++;
  add_short(&out_packet,channel,NULL);
  (*output_packet_len)++;
  add_medium(&out_packet,user_lines[node].user_info.number,NULL);
  (*output_packet_len)+=2;
  add_medium(&out_packet,user_options[node].time,NULL);
  (*output_packet_len)+=2;

  *output_packet_len += sprintf(out_packet,"%s",user_lines[node].user_info.handle);

}


void decode_login_message_packet(char *begin,int system_no)
{
  int node,channel,user_number,time;
  char temp[150];
  struct sys_list_entry a_user;

  read_short(&begin,&node,NULL);
  read_short(&begin,&channel,NULL);
  read_medium(&begin,&user_number,NULL);
  read_medium(&begin,&time,NULL);

  sprintf(temp,"--> Login #%02d/Gtalk-%03d - [T%02d]:%s",node,system_no,channel,begin);
  aput_into_buffer(server,temp,channel,3,channel,node,3);

  a_user.node = node;
  a_user.channel = channel;
  a_user.user_number = user_number;
  a_user.time        = time;
  strncpy(a_user.handle,begin,HANDLE_SIZE-1);
  a_user.handle[HANDLE_SIZE-1] = 0;
  record_user_for_system(&a_user,system_no);
}

void decode_handle_update_packet(char *begin,int system_no)
{
  int node,channel,user_number,time;
//  char temp[150];
  struct sys_list_entry a_user;

  read_short(&begin,&node,NULL);
  read_short(&begin,&channel,NULL);
  read_medium(&begin,&user_number,NULL);
  read_medium(&begin,&time,NULL);

//  sprintf(temp,"--> Login #%02d/Gtalk-%03d - [T%02d]:%s",node,system_no,channel,begin);
//  aput_into_buffer(server,temp,channel,3,channel,node,3);

  a_user.node = node;
  a_user.channel = channel;
  a_user.user_number = user_number;
  a_user.time        = time;
  strncpy(a_user.handle,begin,HANDLE_SIZE-1);
  a_user.handle[HANDLE_SIZE-1] = 0;
  record_user_for_system(&a_user,system_no);
}

void decode_logout_message_packet(char *begin,int system_no)
{
  int node,channel;
  char temp[150];

  read_short(&begin,&node,NULL);
  read_short(&begin,&channel,NULL);

  sprintf(temp,"--> Logout #%02d/Gtalk-%03d - [T%02d]:%s",node,system_no,channel,begin);
  aput_into_buffer(server,temp,channel,3,channel,node,4);
  delete_user_for_system(node,system_no);
}										 


void compose_system_list_packet(char *out_packet,int *output_packet_len,int data1,int data2)
{
  struct system_memory_block *this_block;

  this_block = find_system_in_data(this_system->system_number);

  if (!this_block)
    return;

  add_short(&out_packet,this_block->known_num_users,NULL);
  (*output_packet_len)++;

  add_string(&out_packet,this_block->users,(sizeof(struct sys_list_entry) * this_block->known_num_users),NULL, output_packet_len);
}

void decode_system_list_packet(char *begin,int system_no)
{
  struct system_memory_block *this_block;
  int len=0;
  int num_users;

  this_block = find_system_in_data(system_no);
  if ((!this_block))
    {
#ifdef GLINK_DEBUG
     print_str_cr_to("Don't have system info",0);
#endif
     request_info_for_system(system_no);
     return;
    }
  read_short(&begin,&num_users,NULL);

  if (!(this_block->sys.num_nodes) || (num_users>(this_block->sys_list_table_size)))
    {
#ifdef GLINK_DEBUG
     print_str_cr_to("Table too small",0);
#endif
     request_info_for_system(system_no);
     return;
    }

  read_string(this_block->users,&begin,&len,NULL,(sizeof(struct sys_list_entry) * this_block->sys_list_table_size));
  this_block->known_num_users = num_users;

}

void compose_system_info_packet(char *out_packet,int *output_packet_len,int data1,int data2)
{
  add_string(&out_packet,this_system,(sizeof(struct system_packet_struct)), NULL, output_packet_len);
}

void decode_system_info_packet(char *begin,int system_no)
{
 struct system_packet_struct temp_data;
 int len;

 read_string(&temp_data,&begin,&len,NULL,sizeof(struct system_packet_struct));

 record_new_system_information(&temp_data);

}


void compose_system_disconnect_packet(char *out_packet,int *output_packet_len,int system_number,int data2)
{
  struct system_memory_block *temp;

  if (system_number>0)
    {  if (!(temp = find_system_in_data(system_number)))
          return;
       add_string(&out_packet,&(temp->sys),(sizeof(struct system_packet_struct)),NULL,output_packet_len);
    }
  else
  add_string(&out_packet,this_system,(sizeof(struct system_packet_struct)), NULL, output_packet_len);
}

void decode_system_disconnect_packet(char *begin,int system_no)
{
 struct system_packet_struct temp_data;
 int len;

 read_string(&temp_data,&begin,&len,NULL,sizeof(struct system_packet_struct));

 record_new_system_information(&temp_data);

 disconnect_system(temp_data.system_number);
}

void decode_channel_move_message_packet(char *begin,int system_no)
{
 char s[20];
 int type,data,channel;


 read_short(&begin,&type,NULL);
 read_short(&begin,&data,NULL);
 read_short(&begin,&channel,NULL);

 if (channel_is_netlinked(channel))
 {
     /* put the normal message into the buffer */
     sprintf(s,"%02d",system_no);

     aput_append_into_buffer(server,channel,54,type,data,tswitch,2,s,begin);
 }

}

void compose_channel_move_message_packet(int data1,int data2,int channel,char *out_packet,int *output_packet_len,char *abuf_input)
{

    add_short(&out_packet,data1,NULL);
    add_short(&out_packet,data2,NULL);
    add_short(&out_packet,channel,NULL);
    (*output_packet_len)+=3;

    (*output_packet_len) += sprintf(out_packet,"%s",abuf_input);

}



int decode_header(char *begin,int *type,int *system_no,int *dest_sys_no,int *message_no,unsigned int *len,unsigned int *divide_info)
{

    if (!read_medium(&begin,type,NULL))
    {
     print_str_cr_to("screwed1",0);
     return 1;
    }
    if (!read_medium(&begin,system_no,NULL))
    {
     print_str_cr_to("screwed2",0);
     return 1;
    }
    if (!read_medium(&begin,dest_sys_no,NULL))
    {
     print_str_cr_to("screwed3",0);
     return 1;
    }
    if (!read_int(&begin,message_no,NULL))
    {
     print_str_cr_to("screwed4",0);
     return 1;
    }
    if (!read_int(&begin,(int *)len,NULL))
    {
     print_str_cr_to("screwed5",0);
     return 1;
    }

    *divide_info = (*len >> 14);
    *len &= 0x3FFF;
    *divide_info &= 0x0003;
    return 0;

}


void construct_header(int pkt_type,int destination,unsigned int packet_len,char *out_buf,unsigned int multi_packet_mode)
{
#ifdef GLINK_DEBUG
  char s[200];

  sprintf(s,"Out Pkt hdr - type:%02d to:%02d len:%02d div:%02d",
         pkt_type,destination,packet_len,multi_packet_mode);
  print_str_cr_to(s,0);
#endif


  packet_len |= (multi_packet_mode << 14);



  add_medium(&out_buf,pkt_type,NULL);
  add_medium(&out_buf,this_system->system_number,NULL);
  add_medium(&out_buf,destination,NULL);
  add_int(&out_buf,this_system->last_packet_num++,NULL);
  add_int(&out_buf,packet_len,NULL);
  *out_buf = '!';
}

/* GLINK MAIN LOOP */

void glink_main_loop(char *buffer)
{
    char should_disconnect          =   0;
    const char      *input_packet   =   buffer;
    const char      *output_packet  =   (buffer + PACKET_BUFFER_SIZE);
    const char      *abuf_input     =   (buffer + (PACKET_BUFFER_SIZE*2));
    const char      *output_frame   =   (buffer + (PACKET_BUFFER_SIZE*3));
    int             output_packet_len=0;
    int             input_packet_len=0;
    int             output_frame_len=0;
    int             pkt_type,destination;
    int             multi_packet_mode=0;
    char            *out_packet     =   output_packet;
    char            *out_packet_st  =   output_packet;
    char            *in_packet      =   input_packet;
    char            *out_frame      =   output_frame;
    char            *out_frame_st   =   output_frame;
    char            *end_out_frame  =   (output_frame + PACKET_BUFFER_SIZE);
    char            *end_in_packet  =   (input_packet + PACKET_BUFFER_SIZE);
    char            *end_out_packet =   (output_packet + PACKET_BUFFER_SIZE);
    char            have_sending_packet=0;
    char            should_send=0;
    time_t          glink_time = time(NULL);
    int             result;
    int             new_key;
    int             channel,sentby,type,data1,data2,data3;   /* in abuffer message */
    int             max_packet_size = 170; // MAX IRC size of frame is about 260, this is conservative
    char            should_sleep    = 1;
    int             my_port = tswitch;


    /* request a SYSTEM_INFO_NET_PACKET to go out */
    send_my_system_info();


    while (1)
     {
        time_t now = time(NULL);

        if ((now < glink_time) || ((now - glink_time) >= 600))
           {
             send_my_system_info();
             send_my_system_list();
             glink_time = now;
           }

        if (should_send)
         {
           should_sleep=0;
            if ((!output_frame_len) || (out_frame>end_out_frame))
               {

                /* SEND PACKET END HERE */
                print_cr();
                print_cr();
                print_chr(17);
                print_cr();
                should_send=multi_packet_mode;

                if (should_send)
                 {
                 out_frame = out_frame_st;
                 output_frame_len=0;

                 multi_packet_mode = assemble_frame_from_data(1,&out_packet,out_frame,
                            &output_packet_len,&output_frame_len,max_packet_size,pkt_type,destination);

                    print_chr(17);          /* XON */
                    print_string(packet_start);  /* PACKET START */
                    print_string("CRCC");   /* CRC */

                    print_chr('!');         /* ! */
                 }

               }
            else
              {
                print_chr_to_noflush(*out_frame,my_port);
                out_frame++;
                output_frame_len--;
              }

         }
         else
         {

          if ((should_disconnect) || (!dcd_detect(tswitch)))
              leave_glink();

          if (aget_abuffer(&sentby, &channel, abuf_input, &type, &data1, &data2,&data3 /*,PACKET_BUFFER_SIZE-1 */))
            {
                should_sleep = 0;
                output_packet_len=0;
                out_packet = output_packet;
                out_frame = out_frame_st;
                output_frame_len=0;
                switch (type)
                {  case 0 : if (line_status[sentby].link==2)
                              break;
                            if (data2==tswitch) break;      /* normal message */
                            if (data3<10)
                            { compose_normal_message_packet(data2,channel,type,out_packet,&output_packet_len,abuf_input);
                              pkt_type=NORMAL_NET_MESSAGE;  /* normal message */
                              destination=NET_DESTINATION_ALL; /* all */
                            }
                            break;
				   case 3 :

                            if (data3<3)
                            {
                                if (line_status[data2].link>1)
                                 break;

                                switch (data3) {
                                    case 1:
                                            update_my_system_list();
#ifdef GLINK_DEBUG
                                            print_str_cr_to("Sending Login Message",0);
#endif
                                            compose_login_message_packet(out_packet,&output_packet_len,data2,data1);
                                            pkt_type = LOGIN_NET_MESSAGE;
                                            destination = NET_DESTINATION_ALL;
                                            break;
                                    case 2:
                                            update_my_system_list();
#ifdef GLINK_DEBUG
                                            print_str_cr_to("Sending Logout Message",0);
#endif
                                            compose_logout_message_packet(out_packet,&output_packet_len,data2,data1);
                                            pkt_type = LOGOUT_NET_MESSAGE;
                                            destination = NET_DESTINATION_ALL;
                                            break;
                                }
                            }

                            break;
                   case 4 :
                            compose_channel_move_message_packet(data1,data2,channel,out_packet,&output_packet_len,abuf_input);
                            pkt_type = CHANNEL_MOVE_NET_MESSAGE;
                            destination = NET_DESTINATION_ALL;
                            break;
                   case 9 : if (data2!=tswitch)         /* Link Channel Message*/
                               break;
                            break;
                   case 14:
                   case 20:
                   case 21:
                   case 22:
                            break;
                   case SEND_PRIVATE_MESSAGE_ABUF:
                            compose_private_message_packet(out_packet,&output_packet_len,abuf_input,sentby,data2,data3);
                            pkt_type    = PRIVATE_NET_MESSAGE;
                            destination = data1;
                            break;

                   case SEND_SYS_INFO_ABUF:        /* Send SYS Info */
                            compose_system_info_packet(out_packet,&output_packet_len,data1,data2);
                            pkt_type    = SYSTEM_INFO_NET_MESSAGE;
                            destination = NET_DESTINATION_ALL;
                            break;

                   case SEND_SYS_LIST_ABUF:
                            update_my_system_list();
                            compose_system_list_packet(out_packet,&output_packet_len,data1,data2);
                            pkt_type    = SYSTEM_LIST_NET_MESSAGE;
                            destination = NET_DESTINATION_ALL;
                            break;

                   case DISCONNECT_CMD_ABUF:
                            compose_system_disconnect_packet(out_packet,&output_packet_len,-1,data2);
                            pkt_type    = SYSTEM_DISCONNECT_NET_MESSAGE;
                            destination = NET_DESTINATION_ALL;
                            should_disconnect = 1;
                            break;

                   case SEND_DISCONNECT_MSG_ABUF:
                            compose_system_disconnect_packet(out_packet,&output_packet_len,data1,data2);
                            pkt_type    = SYSTEM_DISCONNECT_NET_MESSAGE;
                            destination = NET_DESTINATION_ALL;
                            break;

                   case SEND_SYS_INFO_REQ_ABUF:
#ifdef GLINK_DEBUG
                            print_str_cr_to("Send system info request",0);
#endif
                            pkt_type    = SYSTEM_INFO_REQUEST_MESSAGE;
                            destination = data1;
                            *(out_packet++) = '#';
                            (output_packet_len)++;
                            break;

                   case SEND_SYS_LIST_REQ_ABUF:
                            pkt_type    = SYSTEM_LIST_REQUEST_MESSAGE;
                            destination = data1;
                            *(out_packet++) = '#';
                            (output_packet_len)++;
                            break;
                   case SEND_HANDLE_UPDATE_ABUF:
                            update_a_user_in_list(data2);
                            compose_handle_update_packet(out_packet,&output_packet_len,data2,data1);
                            pkt_type    = HANDLE_UPDATE_NET_MESSAGE;
                            destination = NET_DESTINATION_ALL;
                            break;

                   default: break;
                 }
                out_packet     =   output_packet;

                if (output_packet_len)
                 {
                        multi_packet_mode = assemble_frame_from_data(0,&out_packet,out_frame,
                                &output_packet_len,&output_frame_len,max_packet_size,pkt_type,destination);

                        /* print packet START and CRC here */

                        print_chr(17);          /* XON */
                        print_string(packet_start);  /* PACKET START */
                        print_string("CRCC");   /* CRC */

                        print_chr('!');         /* ! */
                        should_send=1;
                 }

            } /* end of "if (aget_abuffer..." case */

          }


          new_key = get_first_char(tswitch);
          if (new_key != -1)
           {
             should_sleep = 0;

             if ((new_key < 32) && (new_key!=13) && (new_key!=10) )
                in_char(tswitch,&sentby,&sentby);
              else
              {

                new_key=int_char(tswitch);

                while (((new_key>=32) && (in_packet<=end_in_packet)))
                    {
                     *(in_packet++)=new_key;
                     input_packet_len++;
                     new_key=int_char(tswitch);
                    }

                if (input_packet_len<=START_STRING_LEN)
                  if (strncmp(input_packet,packet_start,input_packet_len))
                    {
                      in_packet = input_packet;
                      input_packet_len=0;
                    }

                if ((new_key==13) || (new_key==10) || (in_packet>=end_in_packet))
                     {
                         if (in_packet>=end_in_packet)
                              in_packet = end_in_packet;

                        *in_packet=0;
                         input_packet_len = (in_packet-input_packet);

                         if (input_packet_len)
                           process_packet(input_packet,input_packet_len);

                         /* reset packet pointers */
                         in_packet      =   input_packet;
                         input_packet_len=0;
                     }

               }
            }

       if (should_sleep)
         {
           DosSleep(100l);
         }
       should_sleep = 1;


     } // END MAIN WHILE LOOP

} // END GLINK_MAIN_LOOP

/*
void aput_into_buffer(int id, char *string, int channel, int parm1,
                      int parm2, int parm3, int parm4)
 */
void request_system_list(int system_number)
{
    aput_into_buffer(tswitch," ",0,SEND_SYS_LIST_REQ_ABUF,system_number,0,0);
}

void send_disconnect_for_system(int system_number)
{
    broadcast_glink_abuf(" ",0,SEND_DISCONNECT_MSG_ABUF,system_number,0,0);
}

void send_my_system_list(void)
{
    aput_into_buffer(server," ",0,SEND_SYS_LIST_ABUF,0,0,0);
}

void request_info_for_system(int system_number)
{
    aput_into_buffer(tswitch," ",0,SEND_SYS_INFO_REQ_ABUF,system_number,0,0);
}

void send_my_system_info(void)
{
    aput_into_buffer(tswitch," ",0,SEND_SYS_INFO_ABUF,0,0,0);
}

#define NUM_SYSTEMS     10
#define NUM_NET_BOTS    10
#define GTALK           0

void load_glink_data(void)
{
 char *buffer;
 char t[20];

 this_system = &sys_glinks.my_system.sys;

 strncpy(this_system->system_name,sys_info.system_name,39);
 this_system->system_name[39] = 0;
 this_system->system_number = sys_info.system_number;
 this_system->software_type = GTALK;
 this_system->time_adjust_seconds = 0;
 this_system->num_nodes             = num_ports;


 buffer = g_malloc_with_owner(sizeof(struct system_memory_block *) * NUM_SYSTEMS,"SYSTABLE",tswitch,0,0);
 if (!buffer)
   abort_glink();
 sys_glinks.net_systems                 = buffer;
 sys_glinks.net_systems_table_size      = NUM_SYSTEMS;
 sys_glinks.num_net_systems             = 0;
 sys_glinks.used                        = 0;

 buffer = g_malloc_with_owner(sizeof(struct bot_list_entry) * NUM_NET_BOTS,"NETBOTS",tswitch,0,0);

 if (!buffer)
   abort_glink();

 sys_glinks.net_bots                = buffer;
 sys_glinks.net_bots_table_size     = NUM_NET_BOTS;
 sys_glinks.num_net_bots            = 0;

 sys_glinks_ready                   = 1;
 create_new_block_for_info(this_system);

 update_my_system_list();
}

void abort_glink(void)
{
    // HANG UP
    end_task();
}

void glink_main(void)
{
  char *buffer;

  /* FIRST, is glink data loaded? if not LOAD IT */
  if (!sys_glinks_ready)
   load_glink_data();

  line_status[tswitch].link = 2;

  buffer = g_malloc((PACKET_BUFFER_SIZE * 3) + ABUF_GET_SIZE,"GLINK");

  if (!buffer)
     {
        broadcast_message("--> Not enough memory for Glink");
        abort_glink();
     }

   line_status[tswitch].online = 1;
   line_status[tswitch].handlelinechanged = ALL_BITS_SET;

  broadcast_message("--> Glinks Started");
  glink_main_loop(buffer);

  // CLEAN GLINK END
  end_task();

}


