/* BUFIO.H */

#ifndef _BUFIO_H
#define _BUFIO_H

#define BUFLIST_MAX 100
#define BUF_CHUNK 512
#define ROLLING_BUF_SIZE 100
#define STRING_LEN_LIMIT 1000

#define POLL_READ 0x01
#define POLL_WRITE 0x02
#define POLL_ACCEPT 0x04
#define POLL_ACCEPT2 0x08

typedef struct _iofifo
{
  char *buffer;
  char *end;
  char *head;
  char *tail;
  int used;
  int size;
} iofifo;

typedef struct _iobuf
{
  int fd;
  struct _iofifo input;
  struct _iofifo output;
  char is_socket;
  char closed;
  char read_error, write_error, except_error;
  int my_errno;
} iobuf;

#define is_closed(buf) ((buf)->closed)
#define buffer_is_flushed(buf) ((buf)->used == 0)
#define buffer_input_empty(buf) (((buf)->input).used == 0)
#define buffer_input_used(buf) (((buf)->input).used)
#define buffer_output_full(buf) (((buf)->output).used >= ((buf)->output).size)
#define buffer_output_empty(buf) (((buf)->output).used == 0)
#define write_buffer(buf,data,size) \
      (write_to_iofifo(&(buf)->output,(data),(size)))
#define read_buffer(buf,data,size) \
      (read_from_iofifo(&(buf)->input,(data),(size)))
#define read_ch_buffer(buf) \
      (read_ch_from_iofifo(&(buf)->input))

int poll_buffers(int wait_time, int accept_fd, int accept_fd2);
int read_from_iofifo(register iofifo *fifo, char *data, int size);
int write_to_iofifo(register iofifo *fifo, char *data, int size);
void requeue_iofifo(register iofifo *fifo, int size);
void clear_iofifo(register iofifo *fifo);
int write_to_iofifo_backwards(register iofifo *fifo, char *data, int size);
int delete_buffer_from_select(iobuf *buf);
int add_buffer_to_select(iobuf *buf);
int fd_buffer(int fd, iobuf *buf, int rsize, int wsize, int issocket);
int dealloc_fifo(iofifo *fifo);
int init_iofifo(iofifo *fifo, int size);
void kill_fd_buffer(iobuf *buf);
int read_ch_from_iofifo(register iofifo *fifo);

int send_printf(struct _iobuf *buf, char *format, ...);
int send_string(struct _iobuf *buf, char *string);

#endif  /* _BUFIO_H */
