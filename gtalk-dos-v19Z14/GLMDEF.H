/* GLM Defines */
/* GLMDEF.H */

#ifndef NULL
#define NULL 0
#endif

#define DOS_SEM 0
#define sprintf g_sprintf
#define tswitch ((*jmptl->tswitch)+0)
#define dans_counter ((*jmptl->dans_counter)+0)
#define time g_time
#define next_task() (jmptl->next_task)()


#ifdef random
#undef random
#endif
#define random(num) (rand() % (num))

/* which_structure for st_copy */
#define USER_LINES 0
#define USER_OPTIONS 1
#define LINE_STATUS 2
#define SYS_INFO 3
#define SYS_TOGGLES 4
#define ABUF_STATUS 5

#include "define.h"
#include "gtst.h"
#include "userst.h"

int far ginsu_main(void);

extern struct jumptable far *jmptl;
extern int old_DS;
extern int old_ES;
extern void far *arg_addr;


/* prototypes */


int g_sprintf(char far *str, char far *format, ...);
void print_str_cr(char far *string);
int get_string_cntrl(char far *string, int limit, char echo, char back_to_end,
    char escape, char noblankline, char cr_on_blankline,
    char upcase, char onlynum);
void lock_dos(int x);
void unlock_dos(void);
time_t g_time(time_t far *data);
int islocked(int sem);

FILE *g_fopen(const char far *filename, const char far *mode,
                const char far *description);
int g_fclose(FILE far *file_ptr);
int g_flush(FILE far *file_ptr);
int g_free(void far *memory_pointer);
void far *g_malloc_main_only(unsigned long int memory,
       const char far *description);
void far *g_malloc(unsigned long int memory,
       const char far *description);
int g_fread(void far *buf, size_t size, size_t count, FILE far *stream);
int g_fwrite(void far *buf, size_t size, size_t count, FILE far *stream);
int g_fseek(FILE far *stream, long int offset, int origin);
long int g_ftell(FILE far *stream);

void initabuffer(int bufsize);
int aget_abuffer(int far *sentby, int far *channel, char far *string,
                 int far *parm1, int far *parm2, int far *parm3,
                 int far *parm4);
void aput_into_buffer(int id, char far *string, int channel,
                 int parm1, int parm2, int parm3, int parm4);
void aput_append_into_buffer(int id, int channel, int parm1,
                 int parm2, int parm3, int parm4, int no_str, ...);
void dealloc_abuf(int portnum);
void print_file_to_cntrl(const char far *filename,int portnum, int ansi,
                   int pause,int abort,int paging);
int wait_ch(void);


int line_editor(char far *filename, int length);
void g_delay(int ticks);
int g_rename(char far *file1, char *far file2);
int g_findfirst(char far *wildcard, struct ffblk far *ffblk, int flags);
int g_findnext(struct ffblk far *ffblk);
void log_error(char far *file);
int g_remove(char far *file);
void send_files(char far **filepointers, int number, int mode);
void recv_files(char far **filepointers, int far *number, int mode);
int g_mkdir(char far *filename);
int g_rmdir(char far *filename);


void print_chr(char temp);
void print_string(char far *string);
void print_cr(void);
void position(int x,int y);
void foreground(int color, int portnum);
void background(int color, int portnum);
void special_code(int state, int portnum);
void reset_attributes(int portnum);
void blink_video(int portnum);
void bold_video(int portnum);
void wrap_line(char far *string);

int chars_in_buffer(int portnum);
int dcd_detect(int portnum);
void put_char_in_buffer(char temp, int portnum);
void get_char(int portnum, int far *charput, int far *isthere);
void send_char(int portnum, char charput);
void empty_buffer(int portnum);
int char_in_buf(int portnum);
int get_first_char(int portnum);

int get_nchar(int portnum);
void wait_for_xmit(int portnum, int ticks);
void empty_outbuffer(int portnum);
void change_dtr_state(int portnum, int state);
char far *g_getcwd(char far *buffer, int buflen);

int unregister_bot_myself(void);
int register_bot(char far *orig_name);
int change_my_info_line(char far *newline);
int st_copy(void *to,size_t size,int which_structure,int data);
int get_server(void);

int g_getdisk(void);
int g_getfree(int drive, unsigned long int far *freebytes,
            unsigned long int far *totalbytes);
int g_getcurdir(int drive, char far *directory);

void get_registered_bot_name_for_myself(char far *string,int string_len);
int read_my_channel(void);
int change_my_channel(int channel);
void broadcast_message(char far *newline);
