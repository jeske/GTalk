/* User.c */
#include <stdio.h>
#include <dos.h>
#include <alloc.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CLASSFILE "class.dat"

#include "..\..\userst.h"

#define PROGRAMNAME Gtalk

void make_default_classes(char *filename)
{
    FILE *fileptr;
    char *point;
    struct class_data_record user_ptr;
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

    temp.data.num_users=3;
    temp.data.record_length = sizeof(struct class_data_record);
    fwrite(&temp,sizeof(temp),1,fileptr);

    /* clear out the structure */

    memset(&user_ptr,0,sizeof(user_ptr));

    /* now write the "EMPTY" record as the first one */

    user_ptr.class.info.staple[0]='(';
    user_ptr.class.info.staple[1]=')';
    user_ptr.class.info.staple[2]='(';
    user_ptr.class.info.staple[3]=')';
    strcpy(user_ptr.class.info.class_name,"UNKNOWN");

    fwrite(&user_ptr,sizeof(user_ptr),1,fileptr);

    strcpy(user_ptr.class.info.class_name,"GUEST");

    fwrite(&user_ptr,sizeof(user_ptr),1,fileptr);

    strcpy(user_ptr.class.info.class_name,"SYSOP");
    memset(user_ptr.class.info.privs,0xFF,MAX_NUM_PRIV_CHARS);

    fwrite(&user_ptr,sizeof(user_ptr),1,fileptr);

    fseek(fileptr,0,SEEK_SET);
    fclose(fileptr);

}

void main(void)
{
    char filename[80];


    printf("\n Legnth of User(class) structure now = %d bytes\n\n",sizeof(struct user_data_record));

    printf("PROGRAMNAME *CLASS* file generator\n\nEnter filename\n\n--> ");


    scanf("%s",filename);

    printf("-> Creating > %s <\n\n\n",filename);

    make_default_classes(filename);

}
