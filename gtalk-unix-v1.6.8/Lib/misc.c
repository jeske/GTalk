/*****************************

  Miscellaneous routines

 *****************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/time.h>
#include <termios.h>

#include "types.h"

struct termios savebuf;

int save_termios(void)
{
  if (tcgetattr(STDIN_FILENO, &savebuf) < 0)
    return (-1);
  return (0);
}

int restore_termios(void)
{
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &savebuf) < 0)
    return (-1);
  return (0);
}

int tty_raw(int fd)
{
  struct termios buf;

  if (tcgetattr(fd, &buf) < 0)
    return (-1);
  buf.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ECHOCTL |
		   ICANON | IEXTEN | ISIG | ECHOKE);
  buf.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON | IXOFF | IXANY);
  buf.c_cflag &= ~(CSIZE | PARENB);
  buf.c_cflag |= CS8;
  buf.c_oflag &= ~(OPOST | OCRNL | ONOCR | ONLCR);
  buf.c_cc[VMIN] = 1;
  buf.c_cc[VTIME] = 0;
  if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
    return (-1);
  return (0);
}

