/* MODULE.H */


#include "define.h"
#include "jumptbl.h"

#define LOAD_UNSHARED 0
#define LOAD_SHARED 1
#define LOAD_UNSHARED_FIRST 2
#define LOAD_SHARED_FIRST 3

/* which_structure for st_copy */
#define USER_LINES 0
#define USER_OPTIONS 1
#define LINE_STATUS 2
#define SYS_INFO 3
#define SYS_TOGGLES 4
#define ABUF_STATUS 5


/* this is also in jumptbl.h */
#ifndef GLM_NAME_LEN
#define GLM_NAME_LEN 8
#endif


struct bot_struct
{
    unsigned int pid;
    char name[GLM_NAME_LEN+1];
    int portnum;
    char info_line[GLM_INFO_LINE_LEN+1];
    int channel;

    /* add security restriction of some kind */

};

struct shared_glm_entry
{
    char glmname[GLM_NAME_LEN+1];
    unsigned int length;
    int number_of_tasks;
    int pid_list[MAX_THREADS];
    remote_func *start_prog;
};


extern struct bot_struct system_bots[];
extern int num_shared;
extern struct shared_glm_entry *glm_entries[];

void run_module(char *str, char *name, int portnum);
void run_sysop_section(char *str, char *name, int portnum);
void show_shared_glms(char *str, char *name, int portnum);
void clear_all_old_pids(void);
void print_bot_list(char *str, char *name, int portnum);
void message_bot(char *str, char *name, int portnum);

int unregister_bot_myself(void);
int register_bot(char *orig_name);
int change_my_info_line(char *newline);
int st_copy(void *to,size_t size,int which_structure,int data);

int read_my_channel(void);
int change_my_channel(int channel);
int is_bot_on_channel(int channel);

