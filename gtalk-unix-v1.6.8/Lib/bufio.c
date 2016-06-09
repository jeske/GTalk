

/*  Buffered I/O system  */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "bufio.h"

iobuf *buflist[BUFLIST_MAX];
int numbufs = 0;

int init_iofifo(iofifo *fifo, int size)
{
  if (!(fifo->buffer=malloc(size)))
    return (-1);
  fifo->end = &fifo->buffer[size];
  fifo->head = fifo->tail = fifo->buffer;
  fifo->size = size;
  fifo->used = 0;
  return (0);
}

int dealloc_fifo(iofifo *fifo)
{
  if (fifo->buffer)
    free(fifo->buffer);
  fifo->buffer = fifo->end = fifo->head = fifo->tail = NULL;
  fifo->size = fifo->used = 0;
  return (0);
}

void kill_fd_buffer(iobuf *buf)
{
  dealloc_fifo(&buf->input);
  dealloc_fifo(&buf->output);
}

int fd_buffer(int fd, iobuf *buf, int rsize, int wsize, int issocket)
{
  int temp;

  if (init_iofifo(&buf->input,rsize) < 0)
    return (-1);
  if (init_iofifo(&buf->output,wsize) < 0)
    {
      dealloc_fifo(&buf->input);
      return (-1);
    }
  buf->fd = fd;
  fcntl(fd, F_SETFL, O_NONBLOCK);  
  buf->is_socket = issocket;
  buf->closed = buf->read_error = buf->write_error = buf->except_error = 0;
  return (0);
}

int add_buffer_to_select(iobuf *buf)
{
  int i;

  for (i=0;i<numbufs;i++)
    {
      if (buflist[i] == buf)
	return (0);
    }
  if (numbufs >= BUFLIST_MAX)
    return (-1);
  buflist[numbufs++] = buf;
  return (0);
}

int delete_buffer_from_select(iobuf *buf)
{
  int i;

  for (i=0;i<numbufs;i++)
    {
      if (buflist[i] == buf)
	{
	  buflist[i] = buflist[numbufs - 1];
	  numbufs--;
	  return (0);
	}
    }
  return (0);
}

int write_to_iofifo_backwards(register iofifo *fifo, char *data, int size)
{
  register int temp = 0;
  
  data += size;
  while ((fifo->used < fifo->size) && (temp < size))
    {
      if (fifo->tail == fifo->buffer)
	fifo->tail = fifo->end;
      *(--fifo->tail) = *(--data);
      fifo->used++;
      temp++;
    }
  return (temp);
}

void clear_iofifo(register iofifo *fifo)
{
  fifo->tail = fifo->head; 
  fifo->used = 0;
}

void requeue_iofifo(register iofifo *fifo, int size)
{
  fifo->tail -= size;
  if (fifo->tail < fifo->buffer)
    fifo->tail += fifo->size;
  fifo->used += size;
}

int write_to_iofifo(register iofifo *fifo, char *data, int size)
{
  register int temp = 0;

  while ((fifo->used < fifo->size) && (temp < size))
    {
      *(fifo->head)++ = *data++;
      if (fifo->head == fifo->end)
	fifo->head = fifo->buffer;
      fifo->used++;
      temp++;
    }
  return (temp);
}

int read_ch_from_iofifo(register iofifo *fifo)
{
  char ch;

  if (fifo->used > 0)
    {
      ch = *(fifo->tail)++;
      if (fifo->tail == fifo->end)
	fifo->tail = fifo->buffer;
      fifo->used--;
      return ((unsigned char)ch);
    }
  return (-1);
}

int read_from_iofifo(register iofifo *fifo, char *data, int size)
{
  register int temp = 0;

  while ((fifo->used > 0) && (temp < size))
    {
      *data++ = *(fifo->tail)++;
      if (fifo->tail == fifo->end)
	fifo->tail = fifo->buffer;
      fifo->used--;
      temp++;
    }
  return (temp);
}

int poll_buffers(int wait_time, int accept_fd, int accept_fd2)
{
  int highfd;
  int i, retcode = 0;
  fd_set read_fd;
  fd_set write_fd;
  iobuf *buf;
  struct timeval timev;
  int end_time;
  int temp;
  struct timeval *timev_ptr;
  char buffer[BUF_CHUNK];

  if (wait_time >= 0)
    {
      end_time = time(NULL) + wait_time;
      timev_ptr = &timev;
    } else
      timev_ptr = NULL;

  for (;;)
    {
      FD_ZERO(&read_fd);
      FD_ZERO(&write_fd);

      if (accept_fd >= 0)
	{
	  FD_SET(accept_fd,&read_fd);
	  highfd = accept_fd;
	} else
	  highfd = 0;
      if (accept_fd2 >= 0)
	{
	  FD_SET(accept_fd2,&read_fd);
	  if (accept_fd2 > highfd)
	    highfd = accept_fd2;
	}
      for (i=0;i<numbufs;i++)
	{
	  buf = buflist[i];
	  if (!buf->closed)
	    {
	      if (buf->input.used < buf->input.size)
		FD_SET(buf->fd,&read_fd);
	      if (buf->output.used)
		FD_SET(buf->fd,&write_fd);
	      if (buf->fd > highfd)
		highfd = buf->fd;
	    }
	}
      if (timev_ptr)
	{
	  time_t tm = time(NULL);
	  timev.tv_sec = (tm > end_time) ? 0 : (end_time - tm);
	  timev.tv_usec = 0;
	}
      temp=select(highfd+1,&read_fd,&write_fd,NULL,timev_ptr);
      if (temp > 0)
	{
	  if (accept_fd >= 0)
	    if (FD_ISSET(accept_fd,&read_fd))
	      retcode |= POLL_ACCEPT;
	  if (accept_fd2 >= 0)
	    if (FD_ISSET(accept_fd2,&read_fd))
	      retcode |= POLL_ACCEPT2;
	  for (i=0;i<numbufs;i++)
	    {
	      buf = buflist[i];
	      if (FD_ISSET(buf->fd,&read_fd))
		{
                  int maxread = buf->input.size - buf->input.used;
		  int bytes;

		  retcode |= POLL_READ;
		  if ((bytes = read(buf->fd, buffer, (maxread > BUF_CHUNK) ?
				    BUF_CHUNK : maxread)) <= 0)
		    {
		      if ((!bytes) && (buf->is_socket))
			buf->closed = 1;
		      buf->read_error = 1;
		      if (bytes < 0)
			buf->my_errno = errno;
		      else
			buf->my_errno = 0;
		    }
		  else
		    write_to_iofifo(&buf->input,buffer,bytes);
		}
	      if (FD_ISSET(buf->fd,&write_fd))
		{
		  int maxwrite = (buf->output.used > BUF_CHUNK) ?
		    BUF_CHUNK : buf->output.used;
		  int bytes;

		  retcode |= POLL_WRITE;
		  read_from_iofifo(&buf->output,buffer,maxwrite);
		  if ((bytes=write(buf->fd,buffer,maxwrite)) < 0)
		    {
		      if (bytes < 0)
			buf->my_errno = errno;
		      else
			buf->my_errno = 0;
		      buf->write_error = 1;
		    }
		  else
		    {
		      if (bytes != maxwrite)
			requeue_iofifo(&buf->output,maxwrite-bytes);
		    }
		}
	    }
	  return (retcode);
        } else
	  {
	    if (!temp)
	      return (0);
	    else
	      {
		if (errno != EINTR)
		  return (-1);
	      }
	  }
    }
}

int send_string(iobuf *buf, char *string)
{
  return (write_buffer(buf,string,strlen(string)));
}

int send_printf(iobuf *buf, char *format, ...)
{
  va_list list;
  int temp;
  char my_string[STRING_LEN_LIMIT];

  va_start(list, format);
  temp=vsprintf(my_string,format,list);
  send_string(buf,my_string);
  va_end(list);
  return (temp);
}







