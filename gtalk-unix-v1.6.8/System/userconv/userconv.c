/* user convert */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#include "userst.h"
#include "ouserst.h"

void remove_ansi(char *str)
{
  char *newstr = str;

   while (*str)
    {
        if (*str==':')
            *str = '.';
        if ((*str=='|') &&
           ((*(str+1)=='*') || (*(str+1)=='+'))&&
            (*(str+2)!=0) && (*(str+3)!=0))
           str+=4;
        else
		  if (*str=='^')  /* filter carrots also */
              str++;
        else
          *newstr++ = *str++;
    }
  *newstr = 0;

}





int load_old_user(int old_fd, int number, struct old_user_data *user_data_old)
{
  struct old_user_data_record buf;

  lseek(old_fd,USER_START_BLOCK_SIZE + 
	(number*(sizeof(struct old_user_data_record))),SEEK_SET);
 
  if (read(old_fd,&buf,sizeof(buf)) != sizeof(buf))
    return 1;

  user_data_old->user_info = buf.user.info;
  user_data_old->class_mod_info = buf.class_mod.info;
  user_data_old->real_info = buf.real.info;

  return 0;

}

int write_new_start_block(int new_fd, union user_data_start_block *temp)
{
  lseek(new_fd,0,SEEK_SET);
  if (write(new_fd,temp,sizeof(*temp))!=sizeof(*temp))
    return 1;

  return 0;
}

int save_new_user(int new_fd, int number, struct user_data *user_data_new)
{
 struct user_data_record buf;
 lseek(new_fd,USER_START_BLOCK_SIZE + 
	(number*(sizeof(struct user_data_record))),SEEK_SET);

 buf.user.info = user_data_new->user_info;
 buf.class_mod.info = user_data_new->class_mod_info;
 buf.real.info = user_data_new->real_info;

 if (write(new_fd,&buf,sizeof(buf)) != sizeof(buf))
    return 1;

  return 0;
  return 0;
}


void conv_to_new(struct user_data *user_data_new,
		 struct old_user_data *user_data_old)
{
  int valid_rec;
  char unix_login[100];

  bzero(user_data_new,sizeof(*user_data_new));

  user_data_new->user_info.login[0]=0;
  user_data_new->user_info.class_name[0]=0;

  if (user_data_old->user_info.number>=0)
   {
     strcpy(user_data_new->user_info.class_name,
	user_data_old->class_mod_info.class_name);
     printf("[%03d] %s :pw[%s]: %s ",user_data_old->user_info.number,
	user_data_old->user_info.handle,user_data_old->user_info.password,
        user_data_old->real_info.name);

     printf(": %s\n",user_data_new->user_info.class_name);

     sprintf(user_data_new->user_info.login,"u%03d",
	user_data_old->user_info.number);
   }
  user_data_new->user_info.user_no = user_data_old->user_info.number;

  if (user_data_old->user_info.number>=0)
     user_data_new->user_info.enable = 1;
  else
     user_data_new->user_info.enable = 0;
  
  strcpy(user_data_new->user_info.handle,user_data_old->user_info.handle);
  memcpy(user_data_new->user_info.toggles,user_data_old->user_info.toggles,10);
  user_data_new->user_info.reset_color = user_data_old->user_info.reset_color;
  
  user_data_new->user_info.expiration = user_data_old->user_info.expiration;
  user_data_new->user_info.conception = user_data_old->user_info.conception;
  user_data_new->user_info.last_call = user_data_old->user_info.last_call;
  user_data_new->user_info.account_balance = 0;
  user_data_new->user_info.starting_date = 
    user_data_old->user_info.starting_date;

  user_data_new->user_info.killstats = user_data_old->user_info.killstats;
  user_data_new->user_info.killedstats = user_data_old->user_info.killedstats;

  user_data_new->user_info.stats = user_data_old->user_info.stats;

  user_data_new->user_info.width = user_data_old->user_info.width;

  user_data_new->user_info.created_by = -1;

/* now do the real information copy */

  bzero(&user_data_new->class_mod_info,sizeof(user_data_new->class_mod_info));
  user_data_new->real_info = user_data_old->real_info;

}

int old_get_num_records(int old_fd)
{
  union user_data_start_block start;

  lseek(old_fd,0,SEEK_SET);
  read(old_fd,&start,sizeof(start));
  
  printf("[%d] Users, [%d] bytes per record\n",
	 start.data.num_users,start.data.record_length);
  return start.data.num_users;
}

void main(void)
{
    char filename[80];
    char new_filename[80];
    char pass_filename[80];
    int loop,num_records;
    int read_errors=0;
    int write_errors=0;
    FILE *passinfo_file;
    struct user_data user_data_new;
    struct old_user_data user_data_old;
    union user_data_start_block start_data;
    int old_users_fd, new_users_fd;

    printf("\n==================================\n");
    printf(  "Gtalk User file CONVERSION program\n");
    printf(  "==================================\n");

    printf("\n\nEnter *OLD* filename\n\n--> ");
    scanf("%s",filename);

    printf("\n\nEnter *NEW* filename\n\n--> ");
    scanf("%s",new_filename);

    printf("\n\nEnter *NEW PASSINFO* filename\n\n--> ");
    scanf("%s",pass_filename);

    printf("-> Creating > %s <\n\n\n",new_filename);
    
    printf("Opening Old...\n");
    if ((old_users_fd = open(filename,O_RDONLY))<0)
      { printf("Error opening OLD for reading\n");
        exit(1);
      }
    printf("Opening New...\n");
    if ((new_users_fd = open(new_filename,O_WRONLY | O_CREAT))<0)
      { printf("Error opening NEW for writing\n");
        exit(1);
      }
    printf("Opening Passinfo...\n");
    if ((passinfo_file = fopen(pass_filename,"wb+"))==0)
      { perror(pass_filename); 
        printf("Error opening PASSINFO for writing\n");
        exit(1);
      }

    printf("Transfering records...");

    num_records = old_get_num_records(old_users_fd);

    bzero(&start_data,sizeof(start_data));
    start_data.data.num_users = num_records;
    start_data.data.record_length = sizeof(struct user_data_record);
    write_new_start_block(new_users_fd,&start_data);

    printf("There are %d users in the old data file\n",num_records);

    printf("Current User #");

    for (loop=0;loop<num_records;loop++) 
    {
        char dummy_string[10];
        char crypted_passwd[30];

        sprintf(dummy_string,"%03d ",loop);
        printf(dummy_string);

        {

            if (!load_old_user(old_users_fd,loop,&user_data_old))
                {

                    memset(&user_data_new,0,sizeof(struct user_data));
                    conv_to_new(&user_data_new,&user_data_old);

                        /* now we need to generate the unix info/passinfo
                           file */

                    if (user_data_new.user_info.login[0])
                      { 
			char salt[3];
                        strncpy(salt,user_data_old.user_info.password,2);
                        salt[2]=0;

			if (salt[0]==' ')
				salt[0]++;
                        if (salt[1]==' ')
                                salt[1]++;
			strcpy(crypted_passwd,(char *)crypt(user_data_old.user_info.password,salt));

			remove_ansi(user_data_old.user_info.handle);

			fprintf(passinfo_file,"%s:%s:%d:220:%s:/home/gt/%s:/usr/local/bin/gtshell\n",
			   user_data_new.user_info.login,
		           crypted_passwd,
                           user_data_new.user_info.user_no+10000,
			   user_data_old.user_info.handle,
			   user_data_new.user_info.login);
                      }

                    if (save_new_user(new_users_fd,loop,&user_data_new))
                       write_errors++;

                }
                else
                    read_errors++;


        }

    }

   close(new_users_fd);
   close(old_users_fd);
   fclose(passinfo_file);
   printf("\nThere were %d read errors and %d write errors\n",read_errors,write_errors);
}

