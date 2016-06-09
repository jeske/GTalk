
/***************************************

               Answer.c

 ***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <termios.h>
#include <arpa/telnet.h>

#include "types.h"
#include "answer.h"
#include "gtmain.h"

/*
   This subroutine achieves whatever answering and setting up to
   the terminal that is needed
*/

#define LAST_MAX_LEN 100

int set_special_canonical(void)
{
  struct termios buf;

  if (tcgetattr(STDIN_FILENO, &buf) < 0)
    {
      log_error("Could not get attributes of %s\n",mydev->name);
      return (-1);
    }

  buf.c_iflag &= ~(IGNBRK|BRKINT|IXON|IXOFF|IXANY|IUCLC|PARMRK|INLCR|IGNCR);
  buf.c_iflag |= ICRNL;
  buf.c_oflag &= ~(OLCUC|OCRNL|ONOCR|ONLRET);
  buf.c_oflag |= (OPOST|ONLCR);
  buf.c_lflag &= ~(ISIG|XCASE|ECHONL|NOFLSH|TOSTOP|ECHOPRT|FLUSHO|PENDIN);
  buf.c_lflag |= ICANON|ECHO|ECHOE|ECHOK|ECHOCTL|ECHOKE|IEXTEN;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &buf) < 0)
    {
      log_error("Could not set attributes of %s\n",mydev->name);
      return (-1);
    }
  return (0);
}

int set_clocal(int clocal)
{
  struct termios buf;

  if (tcgetattr(STDIN_FILENO, &buf) < 0)
    {
      log_error("Could not get attributes of %s\n",mydev->name);
      return (-1);
    }

  if (clocal)
    buf.c_cflag |= CLOCAL;
  else
    buf.c_cflag &= ~CLOCAL;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &buf) < 0)
    {
      log_error("Could not set attributes of %s\n",mydev->name);
      return (-1);
    }
  return (0);
}  

int drop_dtr(void)
{
  struct termios buf;
  struct termios temp_buf;

  if (tcgetattr(STDIN_FILENO, &buf) < 0)
    {
      log_error("Could not get attributes of %s\n",mydev->name);
      return (-1);
    }
  temp_buf = buf;

  if (cfsetispeed(&buf, B0) < 0)
    return (-1);
  if (cfsetospeed(&buf, B0) < 0)
    return (-1);

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &buf) < 0)
    {
      log_error("Could not set attributes of %s\n",mydev->name);
      return (-1);
    }
  sleep(1);
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &temp_buf) < 0)
    {
      log_error("Could not set attributes of %s\n",mydev->name);
      return (-1);
    }
  return (0);
}  

int wait_for(char *string, int time)
{
  fd_set fdset;
  char last[LAST_MAX_LEN];
  struct timeval tim;
  int len = strlen(string);
  int temp;
  char c;

  for (;;)
    {
      FD_ZERO(&fdset);
      FD_SET(STDIN_FILENO, &fdset);

      if (time)
	{
	  tim.tv_sec = time;
	  tim.tv_usec = 0;
	}
      if (time)
	temp = select(STDIN_FILENO+1,&fdset,NULL,NULL,&tim);
      else
	temp = select(STDIN_FILENO+1,&fdset,NULL,NULL,NULL);
      if (temp < 0)
	if (errno != EINTR)
	  return (-1);
      if (temp == 0)
	return (0);
      if (temp > 0)
	{
	  if (FD_ISSET(STDIN_FILENO, &fdset))
	    {
	      if (read(STDIN_FILENO, &c, sizeof(c) > 0))
		  {
		    memmove(last, last+1, LAST_MAX_LEN - 1);
		    last[LAST_MAX_LEN - 1] = c;
		    if (!(memcmp(last + LAST_MAX_LEN - len, string,
			len)))
		      return (1);
		  }
	    }
	}
    }
}

int set_parity_etc(int fd, char *c)
{
  struct termios buf;

  if (tcgetattr(fd, &buf) < 0)
    return (-1);
  while (*c)
    {
      switch (*c)
	{
	  case '8':
	     buf.c_cflag = (buf.c_cflag & ~CSIZE) | CS8;
	     break;
	  case '7':
	     buf.c_cflag = (buf.c_cflag & ~CSIZE) | CS7;
	     break;
  	  case 'O':
	  case 'o':
             buf.c_cflag |= (PARENB | PARODD);
	     break;
          case 'E':
	  case 'e':
             buf.c_cflag |= PARENB;
	     buf.c_cflag &= ~PARODD;
	     break;
          case 'N':
          case 'n':
             buf.c_cflag &= ~PARENB;
             break;
          case '1': 
	     buf.c_cflag &= ~CSTOPB;
	     break;
	  case '2':
	     buf.c_cflag |= CSTOPB;
	     break;
	}
      c++;
    }
  if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
    return (-1);
  return (0);
}
  
int set_baud_rate(int baud)
{
  int kons = -1;
  struct termios buf;

  if (tcgetattr(STDIN_FILENO, &buf) < 0)
    return (-1);

  switch (baud)
    {
      case 300:   kons = B300;
                  break;
      case 1200:  kons = B1200;
                  break;
      case 2400:  kons = B2400;
                  break;
      case 4800:  kons = B4800;
                  break;
      case 9600:  kons = B9600;
	          break;
      case 19200: kons = B19200;
                  break;
      case 38400: kons = B38400;
                  break;
   }
  if (cfsetispeed(&buf, kons) < 0)
    return (-1);
  if (cfsetospeed(&buf, kons) < 0)
    return (-1);
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &buf) < 0)
    return (-1);
  return (0);
}

int get_baud_rate(void)
{
  g_uint32 baud = 0;
  char c;

  do
    {
      if (read(STDIN_FILENO, &c, sizeof(c)) < 0)
	return (-1);
    } while (((c < '0') || (c > '9')) && (c != '\r') && (c != '\n'));
  do
    {
      if (((c == '\r') || (c == '\n')) && (baud == 0))
	{
	  baud = 300;
	  break;
	}
      baud = (baud * 10) + (c - '0');
      if (read(STDIN_FILENO, &c, sizeof(c)) < 0)
	return (-1);
    } while ((c >= '0') && (c <= '9'));
  while ((c != '\r') && (c != '\n'))
    {
      if (read(STDIN_FILENO, &c, sizeof(c)) < 0)
	return (-1);
    }      
  if (set_baud_rate(baud) < 0)
    return (-1);
  log_error("set %s to baud rate %d",mydev->name,baud);
  return (0);
}      

int send_slow(char *string)
{
  while (*string)
    {
      write(STDOUT_FILENO, string, sizeof(char));
      usleep(50000);
      string++;
    }
}

int answer_properly(void)
{
  int type = mydev->node_type;
  struct termios buf;
  int temp;

  if (tcgetattr(STDIN_FILENO, &buf) < 0)
    {
      log_error("Could not get attributes of %s\n",mydev->name);
      return (-1);
    }

  if ((type == SERIAL_NODE_TYPE) || (type == MODEM_NODE_TYPE)) {
      if (mydev->rts_cts)
	buf.c_cflag |= CRTSCTS;
      else
	buf.c_cflag &= ~CRTSCTS;

  if (type == MODEM_NODE_TYPE)
    buf.c_cflag |= HUPCL;
  else
    buf.c_cflag &= ~HUPCL;

  if (set_clocal(1) < 0)
    return (-1);
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &buf) < 0)
    return (-1);
  if (set_baud_rate(mydev->baud_rate) < 0)
    {
      log_error("Could not set attributes of %s\n",mydev->name);
      return (-1);
    }
  }

  if (tty_raw(STDIN_FILENO) < 0)
    return (-1);

  if (type == MODEM_NODE_TYPE)
    {
      if (drop_dtr() < 0)
	return (-1); 
      send_slow("\r\r");
      usleep(250000l);
      if (*mydev->init1)
	{
	  send_slow(mydev->init1);
	  send_slow("\r");
	  if (wait_for("OK",15) < 1)
	    {
	      log_error("Did not get OK from modem %s\n",mydev->name);
	      return (-1);
	    }
	  sleep(1);
	}
      if (*mydev->init2)
	{
	  send_slow(mydev->init2);
	  send_slow("\r");
	  if (wait_for("OK",15) < 1)
	    {
	      log_error("Did not get OK from modem %s\n",mydev->name);
	      return (-1);
	    }
	  sleep(1);
	}
      if (set_clocal(0) < 0)
	return (-1);
      mynode->status = NODE_CONN_WAITING;
      if (wait_for("CONNECT",0) < 1)
	{
	  log_error("Did not get CONNECT from modem %s\n",
		    mydev->name);
	  return (-1);
	}
      mynode->status = NODE_CONNECTING;
      if (!mydev->lock_dte)
	if (get_baud_rate() < 0)
	  return (-1); 
      else
	wait_for("\r",1);
      sleep(1);
      tcflush(STDIN_FILENO, TCIFLUSH);
    }

  if ((type == DIRECT_NODE_TYPE))
    { char key;
      int ansi_state = ansi_on(1);
      printf_ansi("|*r1\n\n");
      printf_ansi("|*f4*************************\n");
      printf_ansi("|*f4*|*r1|*h1   Gtalk Direct Login  |*r1|*f4*\n");
      printf_ansi("|*f4*|*r1|*h1 Press Return To Login |*r1|*f4*\n");
      printf_ansi("|*f4*************************|*r1\n\n");

      do { 
	key = getc(stdin);
      } while ((key!=10) && (key!=13));
      printf("Connected...\r\n");
      ansi_on(ansi_state);
    }
  mynode->status = NODE_CONNECTED;
  return (0);
}



