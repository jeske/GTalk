/* MAIL.H */

#ifndef GT_MAIL_H
#define GT_MAIL_H


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#define FILENAME_LEN 80
#define USER_NAME_LEN 80
#define SUBJECT_LEN 80
#define SYSTEM_NAME_LEN 40
#define MAIL_MAX 3
#define BOARD_NAME 20
#define STALL_TASKS 30
#define MAX_EDITOR_LEN 16384
#define MAIL_PAGING 22

/* open modes */
#define R_BASE 0
#define RW_BASE 1

/* list_mail_special(int mode);
   command modes */

#define LIST_NEW_MAIL 0
#define LIST_OLD_MAIL 1
#define LIST_ALL_MAIL 2
#define NO 0
#define YES 1
#define NO_CREATE 0
#define CREATE 1


/* ERRORS */

#define GT_BASE_DOES_NOT_EXIST -200

/* MAIL.H */

struct file_entry
{
  char				 filename[FILENAME_LEN];
  FILE				 *fp;
  int				 openfile;
};

struct first_entry
{
  unsigned short int	   max_mesg;				/*	2	2 */
  unsigned char 	 priority;				  /*  1   3 */
  unsigned long int  head_entry_no; 		  /*  4   7 */
  unsigned long int  tail_entry_no; 		  /*  4  11 */
  char				 name[BOARD_NAME+1];	  /* 21  32 */
  unsigned long int  new_msg_no;			  /*  4  36 */
  unsigned char 	 post_priority; 		  /*  1  37 */
  char				 bbs_base_code[7];		  /*  7  44 */
  unsigned short int enter_priv;              /*  2  46 */
  char               junk[210];               /*210 256 */
};

struct mail_pass_data
{
  char				  basename[FILENAME_LEN];
  struct first_entry  entry;
  FILE				  *fp;
  int				  mail;
  char				  is_open;
  char				  open_mode;
  int				  bbs_num;
  unsigned long int   handle;
};

struct mesg_entry
{
  unsigned long int  entry_no;						/*	4  4  */
  char				 read;							/*	1  5  */
  char				 deleted;						/*	1  6  */
  time_t			 ent_date;						/*	4  10 */
  char				 filename[14];					/* 14  24 */
  unsigned short int	   system_no;					  /*  2  26 */
  unsigned short int	   dest_no; 					  /*  2  28 */
  short int 			   user_no; 					  /*  2  30 */
  char				 username[USER_NAME_LEN+1]; 	/* 81 111 */
  char				 systemname[SYSTEM_NAME_LEN+1]; /* 41 152 */
  char				 subject[SUBJECT_LEN+1];		/* 81 233 */
  short int 			   dest_user_no;				  /*  2 235 */
  char				 total_erase;					/*	1 236 */
  char				 bbs_base_code[7];				/*	7 243 */
  char				 junk[13];						/* 13 256 */
};

typedef unsigned long int ent_no;

void is_new_mail(void);
void mail_system(char *str,char *name, int portnum);
void user_feedback(char *string, char *name, int portnum);


/* NEW PROTOTYPES */


void mail_line(char *string, int len, int abs_len, FILE *fileptr);
void strcpy_n(char *outstr, char *instr, int length);
void flush_base(struct mail_pass_data *data);
void close_base(struct mail_pass_data *data);
void generate_id(struct mail_pass_data *data, char *filename,
				 struct file_entry *file);

void compress_base(struct mail_pass_data *data,char echo);
void print_fifo_info(struct mail_pass_data *data);
void examine_board(void);
void send_mail_message(void);
void read_messages(struct mail_pass_data *data, unsigned long int start);
long int read_new_message(void);
long int read_mail_message(void);
int delete_mail(long int entry_no, char *command);
void undelete_mail(void);
void print_str_nchr(char *string, int length);
void do_assimilation(struct mail_pass_data *data,char *file_pattern);
void assimilate_old_mail(int user_number,struct mail_pass_data *data);
void print_string_of_len(char *string, int len);
void list_mail_special(char *command);
void send_mail(void);
void user_feedback(char *string, char *name, int portnum);
void is_new_mail(void);
void auto_reply_to_message(unsigned long int entry_num);
void auto_reply(void);
void mail_system(char *str,char *name, int portnum);
int copy_a_file(struct file_entry *outfile,
				struct file_entry *infile, int append);
int open_mail_base(int user_no, struct mail_pass_data *data, int create_mail);
int open_bbs_base(int bbs_no, struct mail_pass_data *data, int create_bbs);


int read_single_message(struct mail_pass_data *data,
								   unsigned long int message_no);

unsigned long int convert_to_entry_num(struct mail_pass_data *data, long int num);
int convert_from_entry_num(struct mail_pass_data *data,unsigned long int entry_num);
int get_entry(struct mail_pass_data *data, unsigned long int mesg_no,
	 struct mesg_entry *mentry, struct file_entry *file);
int read_bbs_base(int bbs_no, struct mail_pass_data *data, int create_bbs);
int add_msg_to_base(struct mail_pass_data *data, int dest_no,
					struct file_entry *file, struct mesg_entry *mentry,
					int overwrite);
int create_temp_file(struct file_entry *file, unsigned int editor, int gen);
void free_all_bbsmail_locks(int portnum);
int delete_msg_from_base(struct mail_pass_data *data,
	   unsigned long int mesg_no, int do_erase);
int read_entry(struct mail_pass_data *data, unsigned long int mesg_no,
	 struct mesg_entry *mentry, struct file_entry *file);
int send_mail_subj(int user_no);
int get_multiple_range(char *prompt, char *point,
		unsigned long int *first, unsigned long int *last);


#endif /* GT_MAIL_H */
