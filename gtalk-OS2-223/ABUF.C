
/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


#include "include.h"
#include "gtalk.h"

#define INCL_DOSPROCESS
#include <os2.h>

/* ABUF.C */

#define CLIENT_BUFFER 2048          /* size of a client's buffer */
#define CLIENT_BUFFER_1 2047        /* CLIENT_BUFFER - 1 */
#define SERVER_BUFFER 8192          /* size of server's buffer */
#define SERVER_BUFFER_1 8191

#undef DEBUG

/* global varialbles */


struct abuf_type  abuf_status[MAX_THREADS];   /* this MUST be
                                                     if it is changed from
                                                     , then aput_into_??
                                                     and aget_??? must be
                                                     changed also
                                                   */


/* check to see if abuf is empty */
int is_abuf_empty(int id)
 {
   return (abuf_status[id].abufwrite == abuf_status[id].abufread);
 };

/* add a character to the port's stream */
/* abuf is the pointer of where to add the character */
/* c is the character to add */
void aput_char(struct abuf_type *abuf, char c)
 {
   *abuf->abufwrite++ = c;     /* write the character */
   if (((char *)abuf->abufwrite) >= ((char *)abuf->abufend)) /* if were at the end of the buffer */
     abuf->abufwrite = abuf->abuffer;   /* go back to the beginning */
   if (((char *)abuf->abufwrite) == ((char *)abuf->abufread))   /* if were at the the end of the */
    {                                       /* buffer, move the read pointer */
      abuf->abufread++;                     /* ahead a bit */
      if (((char  *)abuf->abufread) == ((char  *)abuf->abufend))
        abuf->abufread = abuf->abuffer;
    };
 };

/* add a message to the port id's stream */
/* id is the stream, *string is the string to use, */
/* channel is the channel number */
void aput_into_buffer(int id, char *string, int channel, int parm1,
                      int parm2, int parm3, int parm4)
{
   struct abuf_type  *abuf = &abuf_status[id];


   if ((id<0) || (id>MAX_THREADS))
     return;

   if (!abuf->active)
	{
	  return;
	};                          /* dont post into buffer that isnt read */

   DosEnterCritSec();
   while (abuf->used)           /* wait for use of buffer */
	{
	  DosExitCritSec();
	  next_task();
	  DosEnterCritSec();
	};

   abuf->used = 1;
   set_death_off();
   DosExitCritSec();

   aput_char(abuf,0xAA);        /* put our beginning message ID there */
   aput_char(abuf,0x55);
   aput_char(abuf,0xBC);
   aput_char(abuf,tswitch);     /* put sender and channel # */
   aput_char(abuf,channel);
   aput_char(abuf,parm1);
   aput_char(abuf,parm2);
   aput_char(abuf,parm3);
   aput_char(abuf,parm4);

   while (*string) aput_char(abuf,*string++);   /* put the string into */
   aput_char(abuf,0);               /* the buffer with a zero */
   abuf->used = 0;              /* let someone else use it */
   task_wake(id);
   set_death_on();
};

/* add a message to the port id's stream */
/* id is the stream, *string is the string to use, */
/* channel is the channel number */
void aput_into_buffer_owner(int id, char *string, int owner,int channel, int parm1,
                      int parm2, int parm3, int parm4)
{
   struct abuf_type  *abuf = &abuf_status[id];


   if ((id<0) || (id>MAX_THREADS))
     return;

   if (!abuf->active)
	{
	  return;
	};                          /* dont post into buffer that isnt read */

   DosEnterCritSec();
   while (abuf->used)           /* wait for use of buffer */
	{
	  DosExitCritSec();
	  next_task();
	  DosEnterCritSec();
	};

   abuf->used = 1;
   set_death_off();
   DosExitCritSec();

   aput_char(abuf,0xAA);        /* put our beginning message ID there */
   aput_char(abuf,0x55);
   aput_char(abuf,0xBC);
   aput_char(abuf,owner);     /* put sender and channel # */
   aput_char(abuf,channel);
   aput_char(abuf,parm1);
   aput_char(abuf,parm2);
   aput_char(abuf,parm3);
   aput_char(abuf,parm4);

   while (*string) aput_char(abuf,*string++);   /* put the string into */
   aput_char(abuf,0);               /* the buffer with a zero */
   abuf->used = 0;              /* let someone else use it */
   task_wake(id);
   set_death_on();
};

void aput_append_into_buffer(int id, int channel, int parm1,
					  int parm2, int parm3, int parm4, int no_str, ...)
 {
   struct abuf_type  *abuf = &abuf_status[id];
   va_list ap;
   char  *string;

   if ((id<0) || (id>MAX_THREADS))
	 return;


   if (!abuf->active)
	{
	  return;
	};                          /* dont post into buffer that isnt read */

   DosEnterCritSec();
   while (abuf->used)           /* wait for use of buffer */
	{
	  DosExitCritSec();
	  next_task();
	  DosEnterCritSec();
	};
   abuf->used = 1;
   set_death_off();
   DosExitCritSec();

   aput_char(abuf,0xAA);        /* put our beginning message ID there */
   aput_char(abuf,0x55);
   aput_char(abuf,0xBC);
   aput_char(abuf,tswitch);     /* put sender and channel # */
   aput_char(abuf,channel);
   aput_char(abuf,parm1);
   aput_char(abuf,parm2);
   aput_char(abuf,parm3);
   aput_char(abuf,parm4);

   va_start(ap,no_str);

   while (no_str>0)
   {
     string = (char  *)va_arg(ap,char *);
	 while (*string) aput_char(abuf,*string++);   /* put the string into */
	 no_str--;
   }

   va_end(ap);

   aput_char(abuf,0);               /* the buffer with a zero */
   abuf->used = 0;              /* let someone else use it */
   task_wake(id);
   set_death_on();
};

void aput_vargs_into_buffer(int id, int channel, int parm1,
					  int parm2, int parm3, int parm4, int no_str,
					  va_list  *ap)
 {
   struct abuf_type  *abuf = &abuf_status[id];
   char  *string;

   if (!abuf->active)
	  return;
							  /* dont post into buffer that isnt read */

   DosEnterCritSec();
   while (abuf->used)           /* wait for use of buffer */
	{
	  DosExitCritSec();
	  next_task();
	  DosEnterCritSec();
	};
   abuf->used = 1;
   set_death_off();
   DosExitCritSec();

   aput_char(abuf,0xAA);        /* put our beginning message ID there */
   aput_char(abuf,0x55);
   aput_char(abuf,0xBC);
   aput_char(abuf,tswitch);     /* put sender and channel # */
   aput_char(abuf,channel);
   aput_char(abuf,parm1);
   aput_char(abuf,parm2);
   aput_char(abuf,parm3);
   aput_char(abuf,parm4);

   while (no_str>0)
   {
     string = (char  *)va_arg(*ap,char  *);
	 while (*string) aput_char(abuf,*string++);   /* put the string into */
	 no_str--;
   }

   aput_char(abuf,0);               /* the buffer with a zero */
   abuf->used = 0;              /* let someone else use it */
   task_wake(id);
   set_death_on();
};

/* clear a port's buffer */
/* id is the stream, *string is the string to use, */
/* channel is the channel number */
void aclear_buffer(int id)
 {
   struct abuf_type *abuf = &abuf_status[id];

   if (!abuf->active) return;
								/* dont change buffer that isnt read */

   DosEnterCritSec();
   while (abuf->used)           /* wait for use of buffer */
	{
	  DosExitCritSec();
	  next_task();
	  DosEnterCritSec();
	};
   abuf->used = 1;
   set_death_off();
   DosExitCritSec();
   abuf->abufread = abuf->abufwrite;
   abuf->used = 0;              /* let someone else use it */

   task_wake(id);
   set_death_on();
  };

unsigned char aget_char(struct abuf_type *abuf)
  {
	char temp;

	if (((char  *)abuf->abufread) == ((char  *)abuf->abufwrite))
	 return 0;
	temp = *abuf->abufread++;
	if (((char  *)abuf->abufread) >= ((char  *)abuf->abufend))
	 abuf->abufread = abuf->abuffer;
	return(temp);
  };

int aget_abuffer(int *sentby, int *channel, char *string, int *parm1,
				 int *parm2, int *parm3,int *parm4)
  {
	struct abuf_type  *abuf = &abuf_status[tswitch];
	char temp;
	char abort = 0;


	if (!abuf->active)
	   return 0;
	DosEnterCritSec();
	while (abuf->used)
	 {
	   DosExitCritSec();
	   next_task();
	   DosEnterCritSec();
	 };
	abuf->used = 1;
	set_death_off();
	DosExitCritSec();

	if (aget_char(abuf) != 0xAA) abort = 1;
	if (!abort) if (aget_char(abuf) != 0x55) abort = 1;
	if (!abort) if (aget_char(abuf) != 0xBC) abort = 1;
	if (abort)
	 {
	   while (aget_char(abuf));
	   abuf->used = 0;
	   set_death_on();
	   return 0;
	 };
	*sentby = aget_char(abuf);
	*channel = aget_char(abuf);
	*parm1 = aget_char(abuf);
	*parm2 = aget_char(abuf);
	*parm3 = aget_char(abuf);
	*parm4 = aget_char(abuf);
	do
	  {
		*string++ = (temp = aget_char(abuf));
	  } while (temp);
	abuf->used = 0;

	set_death_on();
	return 1;
  };

int aback_abuffer(int id, int lines)
  {
	struct abuf_type  *abuf = &abuf_status[id];
	char  *back_ptr;

	if (!abuf->active)
	   return 0;
	DosEnterCritSec();
	while (abuf->used)
	 {
	   DosExitCritSec();
	   next_task();
	   DosEnterCritSec();
	 };
	abuf->used = 1;
	set_death_off();
	DosExitCritSec();

	lines += 2;

	back_ptr = abuf->abufwrite;

	while ((lines>0) && (back_ptr != abuf->abufread))
	 {
	   back_ptr--;
	   if (back_ptr < abuf->abuffer) back_ptr = abuf->abufend-1;
	   if (!(*back_ptr)) lines--;
	 };

	abuf->abufread = back_ptr;

	abuf->used = 0;

	set_death_on();
	return 1;
  };

void initabuffer(int bufsize)
 {
   struct abuf_type *abuf = &abuf_status[tswitch];

   abuf->used = 0;
   lock_dos(504);
   abuf->abuffer = g_malloc_main_only(bufsize,"ABUFFER");
   unlock_dos();
   if (!abuf->abuffer) return;
   abuf->abufend = abuf->abuffer + bufsize;
   abuf->abufread = abuf->abuffer;
   abuf->abufwrite = abuf->abuffer;
   abuf->max_buffer = bufsize - 1;
   abuf->num_buffer = 0;
   abuf->active = 1;
 };

void dealloc_abuf(int portnum)
{

   abuf_status[portnum].active=0;
   if (abuf_status[portnum].abuffer)
	{  DosEnterCritSec();
	  g_free(abuf_status[portnum].abuffer);
	  abuf_status[portnum].abuffer = NULL;
	  DosExitCritSec();

	}
};
