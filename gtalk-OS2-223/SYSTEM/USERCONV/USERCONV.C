
/* user convert */
#include <stdio.h>
#include <dos.h>
#include <alloc.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "..\..\userst.h"
#include "..\ouserst.h"


void repeat_character(int character, int num)
{
 while (num--)
 {
   printf("%c",character);
 }
};


void copy_real_info(struct rl_info *new,struct old_rl_info *old)
{
  strcpy(new->name,old->name);
  strcpy(new->street,old->street);
  strcpy(new->city,old->city);
  strcpy(new->state_or_province,old->state);
  strcpy(new->country,"Unknown");
  strcpy(new->postal_code,old->zip);
  strcpy(new->phone,old->phone);
  strcpy(new->phone2,old->phone2);
}


void conv_class_info(struct class_defined_data_struct *new,struct old_user_data *old)
{
    switch (old->user_type)
    {
        case 0: strcpy(new->class_name,"SYSOP");
                break;
        case 10: strcpy(new->class_name,"MAJOR_COSYSOP");
                break;
        case 20: strcpy(new->class_name,"MINOR_COSYSOP");
                break;
        case 30: strcpy(new->class_name,"USER");
                break;
        case 40: strcpy(new->class_name,"REG_GUEST");
                break;
        case 50: strcpy(new->class_name,"GUEST");
                break;
    }

    memcpy(new->privs,old->privs,10);
    memcpy(new->staple,old->staple,4);
    new->time = old->time;
    new->added_time = old->added_time;
    new->line_out = old->line_out;
    new->priority = old->priority;

}

void conv_to_new(struct user_data *user_data_new,struct old_user_data *user_data_old)
{
    user_data_new->user_info.number = user_data_old->number;
    user_data_new->user_info.enable = 1;

//    user_data_new->user_info.password_enc=0;

    strcpy(user_data_new->user_info.handle,user_data_old->handle);
    strcpy(user_data_new->user_info.password,user_data_old->password);

    memcpy(user_data_new->user_info.toggles,user_data_old->toggles,10);

    user_data_new->user_info.reset_color = user_data_old->reset_color;

    user_data_new->user_info.expiration = user_data_old->expiration;
    user_data_new->user_info.conception = user_data_old->conception;
    user_data_new->user_info.last_call = user_data_old->last_call;
    user_data_new->user_info.credit = user_data_old->credit;
    user_data_new->user_info.starting_date = user_data_old->starting_date;

    user_data_new->user_info.killstats = user_data_old->killstats;
    user_data_new->user_info.killedstats = user_data_old->killedstats;

    user_data_new->user_info.stats = user_data_old->stats;

    user_data_new->user_info.width = user_data_old->width;

    user_data_new->user_info.created_by = -1;


/* now do the real information copy */

    copy_real_info(&(user_data_new->real_info),&(user_data_old->real_info));

/* now birthdate */

    user_data_new->real_info.birth_date = user_data_old->birth_date;

/***************************************************/
/* now we need to do the class_info side of things */
/***************************************************/

    conv_class_info(&(user_data_new->class_info),user_data_old);
    strcpy(user_data_new->class_mod_info.class_name,user_data_new->class_info.class_name);


}

void main(void)
{
    char filename[80];
    char new_filename[80];
    int loop,num_records;
    int read_errors=0;
    int write_errors=0;
    struct user_data user_data_new;
    struct old_user_data user_data_old;

    printf("\n==================================\n");
    printf(  "Gtalk User file CONVERSION program\n");
    printf(  "==================================\n");

    printf("\n\nEnter *OLD* filename\n\n--> ");
    scanf("%s",filename);


    printf("\n\nEnter *NEW* filename\n\n--> ");
    scanf("%s",new_filename);


    printf("-> Creating > %s <\n\n\n",new_filename);

    printf("Opening Old...\n");
    open_old_users(filename);
    printf("Opening New...\n");
    open_new_users(new_filename);

    printf("Transfering records...");

    num_records = old_get_num_records();

    printf("There are %d users in the old data file\n",num_records);

    printf("Current User #");


    for (loop=0;loop<num_records;loop++)
    {
        char dummy_string[10];

        sprintf(dummy_string,"%03d ",loop);
        printf(dummy_string);

        {
            printf("[Reading]");

            if (!load_old_user(loop,&user_data_old))
                {
                    repeat_character(8,strlen("[Reading]"));

                    memset(&user_data_new,0,sizeof(struct user_data));
                    conv_to_new(&user_data_new,&user_data_old);

                    printf("[Writing]");

                    if (save_new_user(loop,&user_data_new))
                       write_errors++;

                }
                else
                    read_errors++;

            repeat_character(8,strlen("[Reading]"));

        }

        repeat_character(8,strlen(dummy_string)); /* erase the number */
    }

   printf("\nThere were %d read errors and %d write errors\n",read_errors,write_errors);
}

