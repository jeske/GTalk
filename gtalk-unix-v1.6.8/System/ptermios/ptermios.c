
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>

struct bit_value
{
  char *name;
  int value;
};

struct bit_value ciflag[] = 
{
  { "IGNBRK", IGNBRK },
  { "BRKINT", BRKINT },
  { "IGNPAR", IGNPAR },
  { "PARMRK", PARMRK },
  { "INPCK",  INPCK },
  { "ISTRIP", ISTRIP },
  { "INLCR",  INLCR },
  { "IGNCR",  IGNCR },
  { "ICRNL",  ICRNL },
  { "IUCLC",  IUCLC },
  { "IXON",   IXON },
  { "IXOFF",  IXOFF },
  { "IXANY",  IXANY },
  { "IMAXBEL", IMAXBEL },
  { NULL, 0 }
};

struct bit_value coflag[] = 
{
  { "OPOST",  OPOST },
  { "OLCUC",  OLCUC },
  { "ONLCR",  ONLCR },
  { "OCRNL",  OCRNL },
  { "ONOCR",  ONOCR },
  { "ONLRET", ONLRET },
  { "OFILL",  OFILL },
  { "OFDEL",  OFDEL },
  { "NLDLY",  NLDLY },
  { "CRDLY",  CRDLY },
  { "TABDLY", TABDLY },
  { "BSDLY",  BSDLY },
  { "VTDLY",  VTDLY },
  { "FFDLY",  FFDLY },
  { NULL, 0 }
};

struct bit_value ccflag[] =
{
  { "CBAUD",  CBAUD },
  { "CSIZE",  CSIZE },
  { "CSTOPB", CSTOPB },
  { "CREAD",  CREAD },
  { "PARENB", PARENB },
  { "PARODD", PARODD },
  { "HUPCL",  HUPCL },
  { "CLOCAL", CLOCAL },
  { "CIBAUD", CIBAUD },
  { "CRTSCTS", CRTSCTS },
  { NULL, 0 }
};

struct bit_value clflag[] =
{
  { "ISIG",   ISIG },
  { "ICANON", ICANON },
  { "XCASE",  XCASE },
  { "ECHO",   ECHO },
  { "ECHOE",  ECHOE },
  { "ECHOK",  ECHOK },
  { "ECHONL", ECHONL },
  { "NOFLSH", NOFLSH },
  { "TOSTOP", TOSTOP },
  { "ECHOCTL", ECHOCTL },
  { "ECHOPRT", ECHOPRT },
  { "ECHOKE", ECHOKE },
  { "FLUSHO", FLUSHO },
  { "PENDIN", PENDIN },
  { "IEXTEN", IEXTEN },
  { NULL, 0 }
};

struct bit_value mdmstat[] = 
{
  { "TIOCM_LE", TIOCM_LE },
  { "TIOCM_DTR", TIOCM_DTR },
  { "TIOCM_RTS", TIOCM_RTS },
  { "TIOCM_ST", TIOCM_ST },
  { "TIOCM_SR", TIOCM_SR },
  { "TIOCM_CTS", TIOCM_CTS },
  { "TIOCM_CAR", TIOCM_CAR },
  { "TIOCM_RNG", TIOCM_RNG },
  { "TIOCM_DSR", TIOCM_DSR },
  { NULL, 0 }
};

struct bit_value ccc_val[] = 
{ 
  { "VINTR", VINTR },
  { "VQUIT", VQUIT },
  { "VERASE", VERASE },
  { "VKILL", VKILL },
  { "VEOF", VEOF },
  { "VTIME", VTIME },
  { "VMIN", VMIN },
  { "VSWTC", VSWTC },
  { "VSTART", VSTART },
  { "VSTOP", VSTOP },
  { "VSUSP", VSUSP },
  { "VEOL", VEOL },
  { "VREPRINT", VREPRINT },
  { "VDISCARD", VDISCARD },
  { "VWERASE", VWERASE },
  { "VLNEXT", VLNEXT },
  { "VEOL2", VEOL2 },
  { NULL, 0 }
};

void print_cc(struct bit_value *bits, struct termios *buf)
{
  while (bits->name)
    {
      printf(" %s=%X", bits->name, buf->c_cc[bits->value]);
      bits++;
    }
  printf("\n");
}

void print_bits(struct bit_value *bits, int number)
{
  while (bits->name)
    {
      unsigned int n = bits->value;
      unsigned int j = number & bits->value;
      
      while (!(n & 0x1))
	{
	  n >>= 1;
	  j >>= 1;
	}
      
      printf(" %s=%X", bits->name, j);
      bits++;
    }
  printf("\n");
}

void main(int argc, char **argv)
{
  struct termios buf;
  struct winsize win;
  int speed;
  int fd;

  if (argc < 2)
    fd = 0;
  else
    if ((fd=open(argv[1],O_RDWR|O_NOCTTY)) < 0)
      {
	fprintf(stderr,"Unable to open device %s\n",argv[1]);
	exit(1);
      }

  if (tcgetattr(fd, &buf) < 0)
    {
      fprintf(stderr,"Could not get termio attributes\n");
      exit(1);
    }
  printf("Input Baud: ");
  speed = cfgetispeed(&buf);
  switch (speed)
    {
      case B0:     printf("DTR low     ");
	           break;
      case B300:   printf("300 baud    ");
	           break;
      case B1200:  printf("1200 baud   ");
	           break;
      case B2400:  printf("2400 baud   ");
	           break;
      case B4800:  printf("4800 baud   ");
	           break;
      case B9600: printf("9600 baud  ");
	           break;
      case B19200: printf("19200 baud ");
	           break;
      case B38400: printf("38400 baud ");
	           break;
      default:     printf("other      ");
	           break;
    }
  printf("Output Baud: ");
  speed = cfgetospeed(&buf);
  switch (speed)
    {
      case B0:     printf("DTR low     ");
	           break;
      case B300:   printf("300 baud    ");
	           break;
      case B1200:  printf("1200 baud   ");
	           break;
      case B2400:  printf("2400 baud   ");
	           break;
      case B4800:  printf("4800 baud   ");
	           break;
      case B9600: printf("9600 baud  ");
	           break;
      case B19200: printf("19200 baud ");
	           break;
      case B38400: printf("38400 baud ");
	           break;
      default:     printf("other      ");
	           break;
    }
  printf("process group: %d\n",tcgetpgrp(fd));
  
  printf("c_iflag bits:");
  print_bits(ciflag, buf.c_iflag);
  printf("c_oflag bits:");
  print_bits(coflag, buf.c_oflag);
  printf("c_cflag bits:");
  print_bits(ccflag, buf.c_cflag);
  printf("c_lflag bits:");
  print_bits(clflag, buf.c_lflag);

  if (ioctl(fd, TIOCMGET, &speed) < 0)
    printf("Unable to read modem status lines\n");
  else
    {
      printf("Modem status lines:");
      print_bits(mdmstat, speed);
    }

  printf("Special characters:");
  print_cc(ccc_val,&buf);

  if (ioctl(fd, TIOCGWINSZ, &win) < 0)
    printf("Unable to read window size\n");
  else
    {
      printf("win WS_ROW=%d WS_COL=%d WS_XPIXEL=%d WS_YPIXEL=%d\n",
	     win.ws_row, win.ws_col, win.ws_xpixel, win.ws_ypixel);
    }
  close(fd);
}
