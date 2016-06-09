/* User.c */
#include <stdio.h>
#include <dos.h>
#include <alloc.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define USERFILE "user.dat"


#include "..\..\userst.h"

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
      printf("Start Block DATA Size: %d\n",sizeof(struct user_data_start_block_struct));


    if (!(fileptr=fopen(filename,"wb")))
      {
        perror(filename);
        return 1;
      }
    fseek(fileptr,0,SEEK_SET);

   /* WRITE START BLOCK INFO */

    temp.data.num_users = 1;
    temp.data.record_length = sizeof(struct user_data_record);
    fwrite(&temp,sizeof(temp),1,fileptr);

    /* now write user info */

    memset(&user_ptr,0,sizeof(user_ptr));

    user_ptr.user.info.number=0;
    strcpy(user_ptr.user.info.handle,"The Sysop");
    strcpy(user_ptr.user.info.password,"Password");
    strcpy(user_ptr.class_mod.info.class_name,"SYSOP");
    for(loop=0;loop<10;loop++)
      user_ptr.class_mod.info.enable_privs[loop]=0xFF;
    user_ptr.class_mod.info.time=0;
    user_ptr.user.info.enable=1;
    user_ptr.class_mod.info.staple[0]='{';
    user_ptr.class_mod.info.staple[1]='}';
    user_ptr.class_mod.info.staple[2]='{';
    user_ptr.class_mod.info.staple[3]='}';

    fwrite(&user_ptr,sizeof(user_ptr),1,fileptr);

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
