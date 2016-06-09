

/******************************************

            Pass through program

 *****************************************/

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
#include <sys/wait.h>

#include "bufio.h"

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

int main(int argc, char *argv[])
{
  fd_set read_fd;
  int fd1, fd2, temp;
  iobuf buf1, buf2;
  unsigned char ch;

  int last = 0;

  if (argc < 3)
    {
      fprintf(stderr, "Need two files to open\n");
      exit(1);
    }
  if ((fd1=open(argv[1],O_RDWR|O_NOCTTY)) < 0)
    {
      fprintf(stderr,"Could not open %s\n", argv[1]);
      exit(1);
    }
  if ((fd2=open(argv[2],O_RDWR|O_NOCTTY)) < 0)
    {
      fprintf(stderr,"Could not open %s\n", argv[2]);
      exit(1);
    }
  tty_raw(fd1);
  tty_raw(fd2);

  if (fd_buffer(fd1, &buf1, 8192, 8192, 0) < 0)
    {
      fprintf(stderr,"Could not open iobuf 1\n");
      exit(1);
    }
  if (fd_buffer(fd2, &buf2, 8192, 8192, 0) < 0)
    {
      fprintf(stderr,"Could not open iobuf 1\n");
      exit(1);
    }
  add_buffer_to_select(&buf1);
  add_buffer_to_select(&buf2);

  for (;;)
    {
      poll_buffers(-1, -1, -1);
      while (!buffer_input_empty(&buf1))
	{
	  ch = read_ch_buffer(&buf1);
	  if (last != 1)
	    {
	      printf("\nPort 1:");
	      last = 1;
	    }
	  printf(" %02X", ch);
	  write_buffer(&buf2, &ch, 1);
	}
      while (!buffer_input_empty(&buf2))
	{
	  ch = read_ch_buffer(&buf2);
	  if (last != 2)
	    {
	      printf("\nPort 2:");
	      last = 2;
	    }
	  printf(" %02X", ch);
	  write_buffer(&buf1, &ch, 1);
	}
    }
}
    
	


