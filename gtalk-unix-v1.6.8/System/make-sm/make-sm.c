/* User.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define USERFILE "user.dat"

#define PROGRAMNAME Gtalk

#include <userst.h>

void list_users(char *filename)
{
    FILE *fileptr;
    char *point;
    struct user_data_record user_ptr;
    struct user_data_start_block_struct st_block;
    int loop,numusers,loop2;
    int numrecords = 0;


    if (!(fileptr=fopen(filename,"r")))
      {
        perror(filename);
        return;
      }

    fseek(fileptr,0,SEEK_SET);

    if (!fread(&st_block,sizeof(struct user_data_start_block_struct),1,fileptr))
        {
          printf("Error reading start block\n");
          perror(filename);
          exit(1);
        }

    numusers = st_block.num_users;

    for (loop2=0;loop2<numusers;loop2++)
     {
      fseek(fileptr,(sizeof(struct user_data_record)*loop2) + sizeof(union user_data_start_block),SEEK_SET);

      if (fread(&user_ptr,sizeof(struct user_data_record),1,fileptr))
        {
           if ((user_ptr.user.info.user_no==(loop2)))
           {
             int loop;

	     numrecords++;
             printf("|*r1#|*ff%03d |*r1: %s|*r1\n",user_ptr.user.info.user_no,
                  user_ptr.user.info.handle);

/*
             printf("Real: %s   %s   %s   %s\n",user_ptr.real.info.name,
                    user_ptr.real.info.street,user_ptr.real.info.phone,
                    user_ptr.real.info.phone2);

             printf("Privs : ");
             for (loop=0;loop<10;loop++)
              printf("%x ",user_ptr.class_mod.info.enable_privs[loop]);
             printf("\n");
*/
           }
       }

    }

    printf("\n|*ffTotal Users|*r1: %d\n",numrecords);
    fclose(fileptr);
}

void main(void)
{
    char filename[80];

/*
    printf("\n Length of User structure now = %d bytes\n\n",sizeof(struct user_data_record));

    printf("User file LISTER\n\nEnter filename\n\n--> ");

    scanf("%s",filename);

    printf("-> Reading > %s <\n\n\n",filename);
*/

    list_users(USERFILE);
}
