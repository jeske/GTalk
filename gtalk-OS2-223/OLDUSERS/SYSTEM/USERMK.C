/* User.c */
#include <stdio.h>
#include <dos.h>
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
#include "..\userst.h"

#define PROGRAMNAME Gtalk

void make_default_users(char *filename)
{
    FILE *fileptr;
    char *point;
    struct user_data user_ptr;
    int loop;

    if (!(fileptr=fopen(filename,"w")))
      {
        perror(filename);
        return 1;
      }
    point=&user_ptr;
   /*WRITE BLANK FOR FIRST USER */

    for(loop=0;loop<sizeof(struct user_data);loop++)
       *point++=0;


    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);

    user_ptr.staple[0]='{';
    user_ptr.staple[1]='}';
    user_ptr.staple[2]='{';
    user_ptr.staple[3]='}';
    user_ptr.number=DEF_SYSOP;
    strcpy(user_ptr.handle,"New Sysop");
    strcpy(user_ptr.password,"Password");
    for (loop=0;loop<10;loop++)
        user_ptr.privs[loop]=0xFF;
    user_ptr.enable=0;
    user_ptr.time=0;

    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);

    user_ptr.number=DEF_CO;
    user_ptr.staple[0]='<';
    user_ptr.staple[1]='>';
    user_ptr.staple[2]='<';
    user_ptr.staple[3]='>';
    strcpy(user_ptr.handle,"New Co");
    strcpy(user_ptr.password,"Password");
    user_ptr.privs[5]=0;
    user_ptr.enable=0;
    user_ptr.time=0;

    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);

    user_ptr.number=DEF_BABY_CO;
    user_ptr.staple[0]='<';
    user_ptr.staple[1]=')';
    user_ptr.staple[2]='<';
    user_ptr.staple[3]='>';
    strcpy(user_ptr.handle,"New Baby Co");
    strcpy(user_ptr.password,"Password");
    user_ptr.privs[4]=0;
    user_ptr.time=45;

    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);

    user_ptr.number=DEF_USER;
    user_ptr.staple[0]='[';
    user_ptr.staple[1]=')';
    user_ptr.staple[2]='[';
    user_ptr.staple[3]=']';
    strcpy(user_ptr.handle,"New User");
    strcpy(user_ptr.password,"Password");
    user_ptr.privs[3]=0;
    user_ptr.enable=0;
    user_ptr.time=45;

    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);


    user_ptr.number=DEF_REG_GUEST;
    strcpy(user_ptr.handle,"Reg Guest User");
    strcpy(user_ptr.password,"Password");
    for (loop=0;loop<10;loop++)
        user_ptr.privs[loop]=0x0;
    user_ptr.privs[0]=0xFF;
    user_ptr.time=5;
    user_ptr.enable=0;
    user_ptr.staple[0]='(';
    user_ptr.staple[1]=')';
    user_ptr.staple[2]='(';
    user_ptr.staple[3]=')';


    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);



    user_ptr.number=DEF_GUEST;
    strcpy(user_ptr.handle,"Guest User");
    strcpy(user_ptr.password,"Password");
    for (loop=0;loop<10;loop++)
        user_ptr.privs[loop]=0x0;
    user_ptr.privs[0]=0xFF;
    user_ptr.time=5;
    user_ptr.enable=0;
    user_ptr.staple[0]='(';
    user_ptr.staple[1]=')';
    user_ptr.staple[2]='(';
    user_ptr.staple[3]=')';


    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);
    user_ptr.number=0;
    strcpy(user_ptr.handle,"The Sysop");
    strcpy(user_ptr.password,"Password");
    for(loop=0;loop<10;loop++)
      user_ptr.privs[loop]=0xFF;
    user_ptr.time=0;
    user_ptr.enable=1;
    user_ptr.staple[0]='{';
    user_ptr.staple[1]='}';
    user_ptr.staple[2]='{';
    user_ptr.staple[3]='}';

    fwrite(&user_ptr,sizeof(struct user_data),1,fileptr);

    fseek(fileptr,0,SEEK_SET);
    fprintf(fileptr,"%d",1);
    fclose(fileptr);

}

void main(void)
{
    char filename[80];


    printf("\n Legnth of User structure now = %d bytes\n\n",sizeof(struct user_data));

    printf("PROGRAMNAME User file generator\n\nEnter filename\n\n--> ");


    scanf("%s",filename);

    printf("-> Creating > %s <\n\n\n",filename);

    make_default_users(filename);

}
