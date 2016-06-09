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
  
  
  if (!(fileptr=fopen(filename,"rb+")))
    {
      perror(filename);
      return;
    }
  
  fseek(fileptr,0,SEEK_SET);
  
  if (!fread(&st_block,sizeof(struct user_data_start_block_struct),
	     1,fileptr))
    {
      printf("Error reading start block\n");
      perror(filename);
      exit(1);
    }
  
  printf("There are %d Records in this file\n",st_block.num_users);
  
  numusers = st_block.num_users;
  
  for (loop2=0;loop2<numusers;loop2++)
    {
      fseek(fileptr,(sizeof(struct user_data_record)*loop2) + 
	    sizeof(union user_data_start_block),SEEK_SET);
      
      if (fread(&user_ptr,sizeof(struct user_data_record),1,fileptr))
        {
	  if ((user_ptr.user.info.user_no==(loop2)))
	    {
	      int loop;
	      
	      printf("%03d  %s    %s \n",user_ptr.user.info.user_no,
		     user_ptr.user.info.handle,user_ptr.user.info.password);
	      
	      printf("Real: %s   %s   %s   %s\n",user_ptr.real.info.name,
		     user_ptr.real.info.street,user_ptr.real.info.phone,
		     user_ptr.real.info.phone2);
	      
	      printf("Old Balance: %d\r\n",user_ptr.user.info.account_balance);

	      printf("\n");
	    }
	  user_ptr.user.info.account_balance = 0;
	  user_ptr.user.info.last_wage_payment_date = 0;
	  user_ptr.user.info.free_credits = 0;
	  user_ptr.user.info.credit_card_balance = 0;
	  fseek(fileptr,(sizeof(struct user_data_record)*loop2) + 
		sizeof(union user_data_start_block),SEEK_SET);
	  
	  fwrite(&user_ptr,sizeof(struct user_data_record),1,fileptr);
	    
	  
	}
      
    }
  
  printf("Done.\n");
  fclose(fileptr);
  
}

void main(void)
{
    char filename[80];


    printf("\n Legnth of User structure now = %d bytes\n\n",sizeof(struct user_data_record));

    printf("User file LISTER\n\nEnter filename\n\n--> ");


    scanf("%s",filename);

    printf("-> Reading > %s <\n\n\n",filename);

    list_users(filename);

}
