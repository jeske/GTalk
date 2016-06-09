/* Jumptable for called functions */



#ifndef JUMPTBL_HEADER
#define JUMPTBL_HEADER

#define LD_STARTID "GsTarTiD"
#define START_BLOCK_LEN 10



/* this is also in module.h */

#ifndef GLM_NAME_LEN
#define GLM_NAME_LEN 8
#endif


#define MAX_MODULE_LEN 64000l

#define NOT_SHARED 0
#define IS_SHARED 1
#define CAN_BE_SHARED 2

int far ginsu_main(void);

// int far ginsu_main(struct jumptable far *,int,int,void far *);

typedef int (far *remote_func)(struct jumptable far *,int,int,void far *);
typedef int (near *remote_func_nr)(struct jumptable far *,int,int,void far *);

struct startblock
{
  char initid[START_BLOCK_LEN];
  int glm_ver_no;
  int glm_header_len;
  char glmname[GLM_NAME_LEN+1];
  int shared;
  unsigned int length;
};

struct jumptable
{
  /* functions */

  int (far *vsprintf)(char far *str, char far *format, void far *ap);
  int (far *print_str_cr)(char far *string);
  int (far *get_string_cntrl)(char far *string, int limit,
                      char echo, char back_to_end,
                      char escape, char noblankline, char cr_on_blankline,
                      char upcase, char onlynum);
  void (far *lock_dos)(int x);
  void (far *unlock_dos)(void);
  time_t (far *g_time)(time_t far *);
  int  (far *islocked)(int);

  FILE far *(far *g_fopen)(const char far *filename,
        const char far *mode, const char far *description);
  int (far *g_fclose)(FILE far *file_ptr);
  int (far *g_flush)(FILE far *file_ptr);
  int (far *g_free)(void far *memory_pointer);
  void *(far *g_malloc_main_only)(unsigned long int memory,
        const char far *description);
  void *(far *g_malloc)(unsigned long int memory,
        const char far *description);
  int (far *g_fread)(void far *buf, size_t size, size_t count, FILE far *stream);
  int (far *g_fwrite)(void far *buf, size_t size, size_t count, FILE far *stream);
  int (far *g_fseek)(FILE far *stream, long int offset, int origin);
  long int (far *g_ftell)(FILE far *stream);

  void (far *initabuffer)(int bufsize);
  int (far *aget_abuffer)(int far *sentby, int far *channel, char far *string,
                 int far *parm1, int far *parm2, int far *parm3,int far *parm4);
  void (far *aput_into_buffer)(int id, char far *string, int channel,
                 int parm1, int parm2, int parm3, int parm4);
  void (far *aput_vargs_into_buffer)(int id, int channel, int parm1,
                      int parm2, int parm3, int parm4, int no_str,
                      va_list far *ap);
  void (far *dealloc_abuf)(int portnum);


  int (far *unregister_bot_myself)(void);
  int (far *register_bot)(char far *orig_name);
  int (far *change_my_info_line)(char far *newline);
  int (far *st_copy)(void far *to,size_t size,int which_structure,int data);

  void (far *print_file_to_cntrl)(const char far *filename,int portnum,
                int ansi, int pause,int abort,int paging);
  int (far *wait_ch)(void);
  int (far *get_server)(void);

  int (far *line_editor)(char far *filename, int length);
  void (far *delay)(int ticks);
  int (far *g_rename)(char far *file1, char *far file2);
  int (far *findfirst)(char far *wildcard, struct ffblk far *ffblk, int flags);
  int (far *findnext)(struct ffblk far *ffblk);
  void (far *log_error)(char far *file);
  int (far *g_remove)(char far *file);
  void (far *send_files)(char far **filepointers, int number, int mode);
  void (far *recv_files)(char far **filepointers, int far *number, int mode);
  int (far *g_mkdir)(char far *filename);
  int (far *g_rmdir)(char far *filename);


  void (far *print_chr)(char temp);
  void (far *print_string)(char far *string);
  void (far *print_cr)(void);
  void (far *position)(int x,int y);
  void (far *foreground)(int color, int portnum);
  void (far *background)(int color, int portnum);
  void (far *special_code)(int state, int portnum);
  void (far *reset_attributes)(int portnum);
  void (far *blink_video)(int portnum);
  void (far *bold_video)(int portnum);
  void (far *wrap_line)(char *string);
  void interrupt (far *next_task)(void);
  void (far *check_for_privates)(void);
  char *(far *getcwd)(char far *buf, int buflen);
  void (far *broadcast_message)(char *string);

  /* variables */

  int far *tswitch;

  int (far **a_chars_in_buffer)(int portnum);
  int (far **a_dcd_detect)(int portnum);
  void (far **a_put_char_in_buffer)(char temp, int portnum);
  void (far **a_get_char)(int portnum,int far *charput, int far *isthere);
  void (far **a_send_char)(int portnum,char charput);
  void (far **a_empty_inbuffer)(int portnum);
  int (far **a_char_in_buf)(int portnum);
  int (far **a_get_first_char)(int portnum);
  int (far **a_get_nchar)(int portnum);
  void (far **a_wait_for_xmit)(int portnum,int ticks);
  void (far **a_empty_outbuffer)(int portnum);
  void (far **a_change_dtr_state)(int portnum, int state);
  unsigned int far *dans_counter;

  /* new functions */

  int (far *getdisk)(void);
  unsigned long int (far *getdfree)(unsigned char drive, struct dfree far *table);
  int (far *getcurdir)(int drive, char far *directory);
  void (far *get_registered_bot_name_for_myself)(char far *string,int string_len);
  int (far *read_my_channel)(void);
  int (far *change_my_channel)(int channel);

};


#endif
