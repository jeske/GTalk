
/**********************************

           srv_login.c

***********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>

#include "list.h"
#include "str.h"
#include "shared.h"
#include "common.h"
#include "types.h"
#include "list.h"
#include "states.h"
#include "term_cli.h"

token_entry_type client_term_tokens[] =
{
  { "ENDTERM", endterm_message, T_TM_ENDTERM },
  { "TERM", term_message, T_TM_TERM }
};

token_list client_term_tok = { 2, client_term_tokens };

int term_to_fd(int fd)
{
  int state, temp, rfl;
  char c;
  fd_set read_fd;
  char buffer[512];

  state = TERM_STATE_INIT;
  while (state != TERM_STATE_QUIT)
    {
      FD_ZERO(&read_fd);
      FD_SET(0,&read_fd);
      FD_SET(fd,&read_fd);
      temp = select(fd+1, &read_fd, NULL, NULL, NULL);
      if (temp < 0)
	if (errno != EINTR)
	  return (-1);
      if (temp > 0)
	{
	  if (FD_ISSET(0, &read_fd))
	    {
	      if ((rfl = read(0, &c, sizeof(c))) > 0)
		{
		  switch (state)
		    {
		      case TERM_STATE_INIT:
		        if (c == '\005')
			  state = TERM_STATE_FIRST;
			else
			  write(fd, &c, sizeof(c));
			break;
		      case TERM_STATE_FIRST:
			switch (c)
			  {
			    case '\001':
			      state = TERM_STATE_QUIT;
			      break;
			    case '\005':
			      write(fd, &c, sizeof(c));
			    default:
			      state = TERM_STATE_INIT;
			      break;
			  }
		      }
		} else
		  if ((rfl < 0) && (errno != EINTR))
		    return (-1);
	    }
	  if (FD_ISSET(fd, &read_fd))
	    {
	      if ((rfl = read(fd, buffer, sizeof(buffer))) > 0)
		write(1, buffer, rfl); 
	      else
		if ((rfl < 0) && (errno != EINTR))
		  return (-1);
	    }
	}
    }
  return (0);
}

int term_message(abuffer *abuf, char *message)
{
  int device;
  device_struct *d;
  unsigned long int num;
  int fd;

  if (!get_number(&message, &num))
    return (-1);
  device = num;
  if ((device < 0) || (device >= c_devices_used))
    return (-1);
  d = c_devices(device);
  if (d->status != DEVICE_TERM)
    return (-1);
  printf_ansi("--> Entering /term with device #%02d\r\n", device);

  if ((fd=open(d->name,O_RDWR|O_NOCTTY)) >= 0)
    {
      if (fcntl(fd, F_SETFL, O_NONBLOCK) >= 0)
	{
	  printf_ansi("--> Press CTRL-E CTRL-A to end\r\n");
	  term_to_fd(fd);
	  fcntl(fd, F_SETFL, 0);
	}
      close(fd);
    } else
      printf_ansi("--> Could not open device \"%s\"\r\n", d->name);
  printf_ansi("\r\n--> Ending /term with device #%02d\r\n", device);
  client_abuf_writef(my_ip, SERVER_PROCESS, STATE_TERM, "ENDTERM %d", device);
  return (0);
}

int endterm_message(abuffer *abuf, char *message)
{
  /* nothing needs to be done right now */
} 

int client_term_process(abuffer *abuf, char *message)
{
  termcli_func ten;

  if (ten=get_token(&message, &client_term_tok, NULL)) 
    (ten)(abuf, message);
}

int cmd_terminal(com_struct *com, char *string)
{
  int device;
  unsigned long int num;

  if (!get_number(&string, &num))
    return (-1);
  device = num;
  if ((device < 0) || (device >= c_devices_used))
    return (-1);
  client_abuf_writef(my_ip, SERVER_PROCESS, STATE_TERM, "TERM %d", device);
  printf_ansi("--> Requested Terminal With Node #%02d\r\n", device);
  return (0);
}

int cmd_force_terminal(com_struct *com, char *string)
{
  int device;
  unsigned long int num;

  if (!get_number(&string, &num))
    return (-1);
  string = skip_blanks(string);
  if ((*string != '+') && (*string != '-'))
    {
      printf_ansi("--> Must have + or - to turn device on/off\r\n");
      return (-1);
    }
  device = num;
  if ((device < 0) || (device >= c_devices_used))
    return (-1);
  client_abuf_writef(my_ip, SERVER_PROCESS, STATE_TERM, "FORCE %d %c",
		     device, *string);
  printf_ansi("--> Forced device #%02d %s\r\n", device,
	      *string == '+' ? "On" : "Off");
  return (0);
}







