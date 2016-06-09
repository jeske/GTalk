/* User.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define USERFILE "user.dat"

#include "userst.h"

#define PROGRAMNAME Gtalk

void make_default_users(char *filename)
{
    FILE *fileptr;
    char *point;
    struct user_data_record user_ptr;
    union user_data_start_block temp;
    int loop;

    if (USER_START_BLOCK_SIZE  < sizeof(struct user_data_start_block_struct))
      {
        printf("Start Block DATA too large!!!\n");
        exit(1);
      }
    else
      printf("Start Block DATA Size: %d(%d used)\n",USER_START_BLOCK_SIZE,
        sizeof(struct user_data_start_block_struct));

    printf("User record size: %d\n",sizeof(struct user_data_record));

    printf("Total File Size should be: %d\n",sizeof(struct user_data_record)+
		USER_START_BLOCK_SIZE);


    if (!(fileptr=fopen(filename,"wb")))
      {
        perror(filename);
        return;
      }

    fseek(fileptr,0,SEEK_SET);

   /* WRITE START BLOCK INFO */

    temp.data.num_users = 1;
    temp.data.record_length = sizeof(struct user_data_record);
    fwrite(&temp,USER_START_BLOCK_SIZE,1,fileptr);

    /* now write user info */

    memset(&user_ptr,0,sizeof(user_ptr));

    user_ptr.user.info.user_no=0;
    strcpy(user_ptr.user.info.handle,"The Sysop");
    strcpy(user_ptr.user.info.login,"gtsys");
    strcpy(user_ptr.user.info.class_name,"SYSOP");
    for(loop=0;loop<10;loop++)
      user_ptr.class_mod.info.enable_privs[loop]=0xFF;
    user_ptr.class_mod.info.time=0;
    user_ptr.user.info.enable=1;

    fwrite(&user_ptr,sizeof(struct user_data_record),1,fileptr);

    fseek(fileptr,0,SEEK_SET);
    fclose(fileptr);

}

void main(void)
{
    char filename[80];


    printf("\n Legnth of User structure now = %d bytes\n\n",sizeof(struct user_data_record));

    printf("PROGRAMNAME User file generator\n\nEnter filename\n\n--> ");


    scanf("%s",filename);

    printf("-> Creating > %s <\n\n\n",filename);

    make_default_users(filename);

}
