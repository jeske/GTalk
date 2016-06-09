
/* sts.c */


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


#define INCL_DOS
#define INCL_DOSDEVIOCTL
#define INCL_DOSPROCESS
#include <os2.h>

/* headers */
#include "include.h"
#include "gtalk.h"
#include "os2ser.h"

#include "com.h"


void os2_set_dtr(int portnum,int state)
{
	port_info_ptr temp = &port[portnum];
	MODEMSTATUS ms;
	UINT data;

	ms.fbModemOn = state ? DTR_ON : 0;
	ms.fbModemOff = state ? 255 : DTR_OFF;
	DosDevIOCtl(temp->os2_PortHandle, IOCTL_ASYNC, ASYNC_SETMODEMCTRL, &ms,
		sizeof(ms), NULL, &data, sizeof(data), NULL);}

int flag=0;

void os2_com_thread(ULONG data)
{
  int port_num = data;
  port_info_ptr temp = (port_info_ptr) &port[port_num];
  char ch;
  SHORT rc;
  ULONG BytesRead;
  PTIB ptib; /* thread info block */
  PPIB ppib; /* process info block */


  DosGetInfoBlocks(&ptib,&ppib);
  DosSetPriority(PRTYS_THREAD,PRTYC_REGULAR,10,ptib->tib_ptib2->tib2_ultid);

  while (1)
  {
	/* read character */
	flag=1;
    rc = DosRead(temp->os2_PortHandle,(PVOID)&ch,1L,&BytesRead);

	if (rc)
	   DosSleep(100L);

	if (BytesRead)
	{
	  put_in_input_buf(port_num,ch);
	}
	else
	DosSleep(2);

	if (temp->should_die)
	  DosExit(EXIT_THREAD,0);

  }
}

void de_init_os2_port(int port_num)
{
	port_info_ptr temp = &port[port_num];

	temp->should_die=1;
	DosSleep(13);

	DosKillThread(temp->recv_thread_id);
	DosWaitThread((&temp->recv_thread_id),DCWW_WAIT);
	DosClose(temp->os2_PortHandle);

}

int init_os2_port(int port_num,unsigned int baud, int databits,
	   int stopbits, char parity)
{
   USHORT rc;
   ULONG action;
   LINECONTROL lctl;
   DCBINFO dcb;
   USHORT BaudRate;
   port_info_ptr temp = &port[port_num];

   temp->port_number = port_num;
   temp->should_die = 0;



   rc = DosOpen(port[port_num].os2_com_name,&(port[port_num].os2_PortHandle),
			&action,0L,0,FILE_OPEN,
			OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYREADWRITE |
			OPEN_FLAGS_FAIL_ON_ERROR,0L);

   if (rc)
        {
        printf("DosOpen denied for port %s\n",port[port_num].os2_com_name);
        return 0;
        }



   lctl.bParity = 0;
   lctl.bDataBits = 8;
   lctl.bStopBits = 0;
   lctl.fTransBreak = 0;

   if ((rc = DosDevIOCtl(port[port_num].os2_PortHandle,IOCTL_ASYNC,
			ASYNC_SETLINECTRL, &lctl, sizeof(LINECONTROL), NULL, NULL,
			0L, NULL))!=0)
   {
	 printf("Error Setting Line Parameters for OS/2 Port %s\n",
			port[port_num].os2_com_name);
	 return 1;
   }

   /* set device control block info */

   dcb.usWriteTimeout = 0;
   dcb.usReadTimeout = 0;
   dcb.fbCtlHndShake = MODE_DTR_CONTROL | MODE_CTS_HANDSHAKE;
   dcb.fbFlowReplace = MODE_RTS_CONTROL;
   dcb.fbTimeout = MODE_NO_WRITE_TIMEOUT | MODE_WAIT_READ_TIMEOUT;
   // dcb.fbTimeout = MODE_NO_WRITE_TIMEOUT;

   dcb.bErrorReplacementChar = 0x00;
   dcb.bBreakReplacementChar = 0x00;
   dcb.bXONChar = 0x11;
   dcb.bXOFFChar = 0x13;
   if ((rc = DosDevIOCtl(port[port_num].os2_PortHandle,IOCTL_ASYNC,
		 ASYNC_SETDCBINFO, &dcb, sizeof(DCBINFO), 0L, NULL, 0L, NULL))!=0)
	 {
		printf("Cannot set control block info: %s\n",
			port[port_num].os2_com_name);
		return 1;
	 }

	 BaudRate = temp->baud_rate;
	 DosDevIOCtl(temp->os2_PortHandle, IOCTL_ASYNC, ASYNC_SETBAUDRATE,
		 &BaudRate, sizeof(USHORT), NULL, NULL, 0L, NULL);

   os2_set_dtr(port_num,1);

   DosCreateThread(&(temp->recv_thread_id),(PFNTHREAD)os2_com_thread,(ULONG)port_num,0,4096 * 3);

   return 1;
}


void set_baud_rate_os2(int port_num, unsigned int baud,
     int databits, int stopbits, char parity)
{
    USHORT BaudRate = baud;
    port_info_ptr temp = &port[port_num];

    if ((BaudRate<= 38400) && (BaudRate>=50))
    {
     DosDevIOCtl(temp->os2_PortHandle, IOCTL_ASYNC, ASYNC_SETBAUDRATE,
         &BaudRate, sizeof(USHORT), NULL, NULL, 0L, NULL);
    }

}

int get_first_char_os2(int portnum)
{
  return (get_from_input_buf(portnum,0));
}

int get_nchar_os2(int portnum)
{
  return (get_from_input_buf(portnum,1));
}

void put_char_in_buffer_os2(char temp,int portnum)
{
 put_in_input_buf(portnum,temp);
}

void get_char_os2(int portnum, int *charput, int *isthere)
{
   *charput = get_from_input_buf(portnum,1);
   if (*charput==-1)
	 *isthere=0;
   else
	 *isthere=1;
}

void send_char_os2(int portnum, char charput)
{
   ULONG BytesWritten;
   port_info_ptr temp = port_fast[portnum];

   DosWrite(temp->os2_PortHandle, &charput, sizeof(charput), &BytesWritten);
}

void send_chars_os2(int portnum, char *charptr, int length)
{
   ULONG BytesWritten;
   port_info_ptr temp = port_fast[portnum];

   DosWrite(temp->os2_PortHandle, charptr, length, &BytesWritten);
}

int chars_in_buffer_os2(int portnum)
{
   return(char_in_input_buf(portnum));
}

int dcd_detect_os2(int port_num)
{
  BYTE instat;
  port_info_ptr temp = &port[port_num];


  if (DosDevIOCtl(temp->os2_PortHandle, IOCTL_ASYNC,ASYNC_GETMODEMINPUT,
       NULL, 0 , NULL, &instat, sizeof(instat), NULL))
     return 0;

  return (instat & DCD_ON);
}
int char_in_buf_os2(int portnum)
{
   return(char_in_input_buf(portnum));
}
void empty_inbuffer_os2(int portnum)
{
   empty_input_buf(portnum);
}

struct tx_queue_info_struct {
unsigned short int 	tx_queue_data;
unsigned short int 	tx_queue_size;
};

void wait_for_xmit_os2(int portnum, int seconds)
{
	port_info_ptr temp = &port[portnum];
	struct tx_queue_info_struct status;
	ULONG ulStatusLen;
	int flag=1;
	int begin_time = time(NULL);


	while (flag)
	{

	DosDevIOCtl(temp->os2_PortHandle, IOCTL_ASYNC, ASYNC_GETOUTQUECOUNT, NULL,
		0, NULL, &status, sizeof(status), &ulStatusLen);

	if (!(status.tx_queue_data))
	   flag=0;
	else
	if ((time(NULL) - begin_time)>seconds)
	   flag=0;
	DosSleep(50l);
	}

}

void empty_outbuffer_os2(int portnum)
{
}
void change_dtr_state_os2(int portnum, int state)
{
	os2_set_dtr(portnum,state);
}
