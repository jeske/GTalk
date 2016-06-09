/* User.c */

#include <stdio.h>
#include <dos.h>
#include <bios.h>
#include <alloc.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define USERFILE "user.dat"

#define NUMDEFAULT 7
#define DEF_GUEST -1
#define DEF_REG_GUEST -2
#define DEF_USER -3
#define DEF_BABY_CO -4
#define DEF_CO -5
#define DEF_SYSOP -6
#define EMPTY -7


#define PROGRAMNAME Gtalk


#define HANDLE_SIZE 41
#define PASSWORD_SIZE 11


struct kl_stats
{
    unsigned int kills_day;
    unsigned int kills_month;
    unsigned int kills_total;
    unsigned int slow_kills_day;
    unsigned int slow_kills_month;
    unsigned int slow_kills_total;


    /* KILL STUFF */

};

struct other_stats
{
     unsigned long int calls_total;
     unsigned long int time_total;
     unsigned int calls_month;
     unsigned int time_month;
     unsigned int calls_day;
     unsigned int time_day;

     unsigned int num_validates;
     unsigned int give_times;

};



struct rl_info
{

    char name[51];
    char street[51];
    char city[13];
    char state[3];
    char zip[10];
    char phone[11];
    char phone2[11];

};

struct exp_date
{
   char day;
   char month;
   unsigned int year;

};

struct user_data
{
    int number;
    char handle[HANDLE_SIZE];
    char password[PASSWORD_SIZE];

    unsigned char privs[10];
    unsigned char toggles[10];
    time_t expiration;
    time_t conception;
    time_t last_call;
    unsigned int time;
    unsigned int added_time;
    char staple[4];
    int enable;
    int priority;
    long int num_calls;
    struct kl_stats killstats;
    struct rl_info real_info;
    struct other_stats stats;
    unsigned char width;
    unsigned char line_out;
    unsigned char num_eat_lines;
    struct exp_date birth_date;
    time_t credit;
    time_t starting_date;
    int priv_give_limit;
    char no_fuck_with;
    struct kl_stats killedstats;
    unsigned char reset_color;
    unsigned char user_type;
    char dummy[68];
} ;




void make_default_users(char *filename)
{
    FILE *fileptr;
    char *point;
    struct user_data user_ptr;
    int loop,numusers,loop2;


    if (!(fileptr=fopen(filename,"r")))
      {
        perror(filename);
        return 1;
      }
    fseek(fileptr,0,SEEK_SET);
    fscanf(fileptr,"%d",&numusers);
    fclose(fileptr);

    if (!(fileptr=fopen(filename,"r")))
      {
        perror(filename);
        return 1;
      }
    printf("There is/are : %d users.\n\n",numusers);


    for (loop2=0;loop2<numusers;loop2++)
     {
      fseek(fileptr,(unsigned int )( sizeof(struct user_data)* (NUMDEFAULT+ loop2)),SEEK_SET);

      if (fread(&user_ptr,sizeof(struct user_data),1,fileptr))
        {
           if ((user_ptr.number==(loop2)))
           {
             int loop;
             printf("%03d  %c%s%c    %s \n",user_ptr.number,user_ptr.staple[0],
                  user_ptr.handle,user_ptr.staple[1],user_ptr.password);
             printf("%s   %s   %s   %s\n",user_ptr.real_info.name,
                    user_ptr.real_info.street,user_ptr.real_info.phone,user_ptr.real_info.phone2);
             printf("Privs : ");
             for (loop=0;loop<10;loop++)
              printf("%x ",user_ptr.privs[loop]);
             printf("\n");
           }
       }
//       else
//         perror("User File");

//     if (feof(fileptr))
//       { printf("End of file\n");
//        return; }
    }

    printf("Done.\n");
    fclose(fileptr);

}

void main(void)
{
    char filename[80];


    printf("\n Legnth of User structure now = %d bytes\n\n",sizeof(struct user_data));

    printf("User file LISTER\n\nEnter filename\n\n--> ");


    scanf("%s",filename);

    printf("-> Reading > %s <\n\n\n",filename);

    make_default_users(filename);

}
