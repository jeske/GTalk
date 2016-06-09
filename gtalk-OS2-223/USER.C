/* User.c */
/* test */
/* headers */

#define INCL_DOSPROCESS
#include <os2.h>

#include "include.h"
#include "gtalk.h"

user_dat user_lines[MAXPORTS];
user_p user_options[MAXPORTS];

/*********************************/

char userfile[] = "USER.DAT";
char classfile[] = "CLASS.DAT";
char  user_member_list[] = "USERS\\MEMBER.LST";
char sysop_member_list[] = "USERS\\SYSMBR.LST";



void set_user_class_info(struct class_defined_data_struct *class,
       struct user_class_information_struct *mod)
{
    int loop;

    for(loop=0;loop<MAX_NUM_PRIV_CHARS;loop++)
    {
     class->privs[loop] |= mod->enable_privs[loop];
     class->privs[loop] &= ~(mod->disable_privs[loop]);
    }

}

/* PRIVLEDGES */


int load_user_info(int number, struct user_data *user_ptr)
{
    struct class_data temp_class;
    int retval;


    if ((retval = load_user(number,user_ptr))!=0)
     {
       return (retval);
     }
    else
     {
       if (load_class_by_name(user_ptr->class_mod_info.class_name,&temp_class))
         {
            char s[40];
            sprintf(s,"* UNKNOWN Class: '%s' User #%03d",user_ptr->class_mod_info.class_name,number);
            log_error(s);

            if (load_class_by_name("UNKNOWN",&temp_class))
             {
                 return 1;
             }
          }

     }

    user_ptr->class_info = temp_class.class_info;

    set_user_class_info(&user_ptr->class_info,&user_ptr->class_mod_info);

    return 0;
}

int load_info_by_class(char *name, struct user_data *user_ptr)
{
     struct class_data temp_class;

    if (load_class_by_name(name,&temp_class))
    { char s[40];
      sprintf(s,"* UNKNOWN Class: %s ",user_ptr->class_mod_info.class_name);
      log_error(s);
      return 1;
    }

    user_ptr->class_info = temp_class.class_info;
    return 0;
}

int load_info_by_class_number(int number, struct user_data *user_ptr)
{
     struct class_data temp_class;

    if (load_class(number,&temp_class))
    { char s[40];
      sprintf(s,"* UNKNOWN Class: %s User #%03d",user_ptr->class_mod_info.class_name,number);
      log_error(s);
      return 1;
    }

    user_ptr->class_info = temp_class.class_info;
    return 0;
}


int test_bit(unsigned char *privs, int bit)
{
  int byte = bit >> 3;
  int whichbit = (0x01) << (bit & 0x07);
  return((*(privs+byte) & whichbit));
};

void set_bit(unsigned char *privs,int bit,int state)
{
  int byte = bit >> 3;
  int whichbit = (0x01) << (bit & 0x07);
  if (state)
   *(privs+byte) = *(privs+byte) | whichbit;
   else
   *(privs+byte) = *(privs+byte) & (~whichbit);
};


int load_user(int number, struct user_data *user_ptr)
{
	int number_users;
	struct user_data_start_block_struct st_block;
    struct user_data_record this_record;
	FILE *fileptr;
	int bytes_read;

	if (number<0)
	 {
	  log_error("* load_user() tried to load negative user");
      return LUERR_NO_SUCH_USER;
	 }

	if (!(fileptr=g_fopen(userfile,"rb","USER#1")))
	   {
		log_error("*user file wouldn't open");
		log_error(userfile);
        return LUERR_FILE_OPEN_ERROR;
	   }

	fseek(fileptr,0,SEEK_SET);

	if (!fread(&st_block, sizeof(struct user_data_start_block_struct), 1, fileptr))
	   {
		 log_error("*Error reading user start block");
		 log_error(userfile);
         return LUERR_START_BLOCK_READ_ERROR;

		}

    if (st_block.record_length != sizeof(struct user_data_record))
	 {
	  log_error("*  !!!!  User record length incorrect !!!! ");
      print_str_cr("Corrupt User File (record length incorrect)");
      return LUERR_INVALID_USER_FILE_RECORD_LENGTH;
	 }

		number_users = st_block.num_users;

	if (number>number_users)
	   {
		log_error("*LOAD USER : system tried to read past end of user file");
		g_fclose(fileptr);
        return (LUERR_NO_SUCH_USER);
	   }
	else
		fseek(fileptr,
         (long int)((sizeof(struct user_data_record)*(number)) +
				   sizeof(union user_data_start_block)),
		 SEEK_SET);

        if ((bytes_read = fread(&this_record, 1, sizeof(struct user_data_record), fileptr)) != sizeof(struct user_data_record))
			 {
				char error_string[120];
                sprintf(error_string,"* fread() filed on file (load_user) - %d bytes read of %d",bytes_read,sizeof(struct user_data_record));
				log_error(error_string);
				log_error(userfile);
				g_fclose(fileptr);
                return LUERR_FREAD_FAILED;
			 }
		if (g_fclose(fileptr))
		   {
			 log_error("g_fclose failed");
			 log_error(userfile);
             return LUERR_CLOSE_FAILED;
		   }

  user_ptr->user_info = this_record.user.info;
  user_ptr->real_info = this_record.real.info;
  user_ptr->class_mod_info = this_record.class_mod.info;

 if (user_ptr->user_info.number != number)
   {
	 user_ptr->user_info.is_class_template=0;
	 user_ptr->user_info.enable=0;
	 user_ptr->user_info.number = -1;
   }

 return 0;
}


int save_user(int number, struct user_data *user_ptr)
{
	int number_users;
	int putit;
    struct user_data_record temp;
	struct user_data_start_block_struct st_block;
	FILE *fileptr;

	if (number<0)
	{
	 log_error("* save_user() tried to save negative user");
	 return 1;
	}


	if (!(fileptr=g_fopen(userfile,"rb+","USER#2")))
	   {
		log_error(userfile);
		return 1;
	   }

	fseek(fileptr,0,SEEK_SET);

	if (!fread(&st_block, sizeof(struct user_data_start_block_struct), 1, fileptr))
	   {
		 log_error("*Error reading user start block");
		 log_error(userfile);
		 return 1;

		}
	number_users = st_block.num_users;

	if (number>=number_users)
		{
		   log_error("* SYSTEM tried to add user that exhisted");

		/* blank user data here */

		/* DEBUG!!! this needs to blank the user data to guest standing */

        memset(&temp,0,sizeof(struct user_data_record));
        temp.user.info.number=-1;
        temp.user.info.enable=0;

		/* now add the record */

		fseek(fileptr,
         (long int)sizeof(struct user_data_record)*(number_users) +
				   sizeof(union user_data_start_block) ,
		 SEEK_SET);

		for (putit=number_users;putit<number;putit++)
         if (!fwrite(&temp, sizeof(struct user_data_record), 1, fileptr))
			  {  log_error(userfile);
				 g_fclose(fileptr);
				 return 1;
			  }

		fseek(fileptr,0,SEEK_SET);
		st_block.num_users = number+1;
		if (!fwrite(&st_block, sizeof(struct user_data_start_block_struct), 1, fileptr))
		   {
			 log_error("*Error writing user start block");
			 log_error(userfile);
             g_fclose(fileptr);
			 return 1;
			}

		}

		fseek(fileptr,
         (long int)sizeof(struct user_data_record)*(number) +
				   sizeof(union user_data_start_block),
		 SEEK_SET);

        temp.user.info = user_ptr->user_info;
        temp.class_mod.info = user_ptr->class_mod_info;
        temp.real.info = user_ptr->real_info;

        if (!fwrite(&temp, sizeof(struct user_data_record), 1, fileptr))
			 {  log_error(userfile);
				log_error("*tried to write user and failed");
				g_fclose(fileptr);
				return 1;
			 }
   fflush(fileptr);

   if (g_fclose(fileptr))
		{
		  log_error(userfile);
		  return 1;
		}
 return 0;

}


int save_class_by_name(char *name,struct class_data *class_ptr)
{
	int num=0;
	int flag=1;
    struct class_data_record temp;

    if (class_ptr->class_info.class_index)
	{
      if (!load_class(class_ptr->class_info.class_index,&temp))
	  {
        if (!strcmp(temp.class.info.class_name,class_ptr->class_info.class_name))
		 {

          temp.class.info = class_ptr->class_info;
          temp.move.info = class_ptr->move_info;

          save_class(class_ptr->class_info.class_index,&temp);
		  return 0;
		 }
	  }
	}

    if (!load_class_by_name(class_ptr->class_info.class_name,&temp))
	  {
        temp.class.info = class_ptr->class_info;
        temp.move.info  = class_ptr->move_info;

        save_class(temp.class.info.class_index,&temp);
		return 0;
	  }


    temp.class.info = class_ptr->class_info;
    temp.move.info = class_ptr->move_info;

    if (add_class(&temp))
	  {return 1;}

	return 0;
}

int class_exists(char *name)
{
 struct class_data temp_class;
 int retval;

 fix_classname(name);
 retval = load_class_by_name(name,&temp_class);

 if (retval)
  return (0);
 else
  return (1);

}


int load_class_by_name(char *name,struct class_data *class_ptr)
{

	int num=1;
    int max_num = read_num_classes();

    for (num=0;num<max_num;num++)
    {
        if (load_class(num,class_ptr))
			{
             memset(class_ptr,0,sizeof(*class_ptr));
			}

        if (!strcmp(class_ptr->class_info.class_name,name))
		{
		 return 0;
		}

    }
}


void list_classes(void)
{
    struct class_data user_ptr;
	int column=0;
	int justify;
    char s[80];
    int num;
    int max_num = read_num_classes();


    sprintf(s,"%d classes",max_num);
    print_str_cr(s);

    for (num=0;num<max_num;num++)
    {
      if (!load_class(num,&user_ptr))
          if (*(user_ptr.class_info.class_name))
          {
            justify = 19 - strlen(user_ptr.class_info.class_name);

            print_string(user_ptr.class_info.class_name);
            repeat_chr(' ',(justify),0);
            column++;
            if (((column+1)*19) > user_options[tswitch].width)
              {
                column=0;
                print_cr();
              }
          }
    }
    print_cr();
}


int read_num_classes(void)
{
	int number_users;
	struct user_data_start_block_struct st_block;
	FILE *fileptr;
	int bytes_read;

	if (!(fileptr=g_fopen(classfile,"rb","CLASS#1")))
	   {
		log_error("*class file wouldn't open");
		log_error(userfile);
        return 0;
	   }
	fseek(fileptr,0,SEEK_SET);

	if (!fread(&st_block, sizeof(struct user_data_start_block_struct), 1, fileptr))
	   {
		 log_error("*Error reading user start block(class)");
		 log_error(userfile);
         return 0;

		}

		if (g_fclose(fileptr))
		   {
			 log_error("g_fclose failed");
			 log_error(userfile);
             return 0;
		   }
    return (st_block.num_users);

}


int load_class(int number, struct class_data *class_ptr)
{
	int number_users;
	struct user_data_start_block_struct st_block;
    struct class_data_record temp;
	FILE *fileptr;
	int bytes_read;

	if (number<0)
	{
	 log_error("* load_class() tried to load negative class");
	 return 1;
	}

	if (!(fileptr=g_fopen(classfile,"rb","CLASS#1")))
	   {
		log_error("*class file wouldn't open");
		log_error(userfile);
		return 1;
	   }
	fseek(fileptr,0,SEEK_SET);

	if (!fread(&st_block, sizeof(struct user_data_start_block_struct), 1, fileptr))
	   {
		 log_error("*Error reading user start block(class)");
		 log_error(userfile);
		 return 1;

		}

    if (st_block.record_length != sizeof(struct class_data_record))
	 {
	  log_error("*  !!!!  Class record length incorrect !!!! ");
	  print_str_cr("Corrupt Class File.");
	  return 1;
	 }

	number_users = st_block.num_users;

	if (number>number_users)
	   {
		log_error("*LOAD USER : system tried to read past end of class file");
		g_fclose(fileptr);
		return 1;
	   }
	else
		fseek(fileptr,
         (long int)((sizeof(struct class_data_record)*(number)) +
				   sizeof(union user_data_start_block)),
		 SEEK_SET);

        if ((bytes_read = fread(&temp, 1, sizeof(struct class_data_record), fileptr)) != sizeof(struct class_data_record))
			 {
				char error_string[120];
                sprintf(error_string,"* fread() filed on file (load_class) - %d bytes read of %d",bytes_read,sizeof(struct class_data_record));
				log_error(error_string);
				log_error(userfile);
				g_fclose(fileptr);
				return 1;
			 }
		if (g_fclose(fileptr))
		   {
			 log_error("g_fclose failed");
			 log_error(userfile);
			 return 1;
		   }
 class_ptr->class_info = temp.class.info;
 class_ptr->move_info = temp.move.info;

 class_ptr->class_info.class_index = number;
 return 0;

}

int add_class(struct class_data *class_ptr)
{
	int number_users;
	int putit;
    int number=0;
    int flag=1;
    struct class_data_record temp;
	struct user_data_start_block_struct st_block;

    print_str_cr("Add class");

    while (flag)
    {
      if (load_class(number,&temp))
           flag=0;
      else
        number++;
    }

    temp.class.info = class_ptr->class_info;
    temp.move.info  = class_ptr->move_info;

    return (save_class(number,&temp));

}


int save_class(int number, struct class_data *class_ptr)
{
	int number_users;
	int putit;
    struct class_data_record temp;
	struct user_data_start_block_struct st_block;
	FILE *fileptr;


	if (number<0)
	{
	 log_error("* save_class() tried to save negative class");
	 return 1;
	}

	if (!(fileptr=g_fopen(classfile,"rb+","CLASS#2")))
	   {
		log_error(userfile);
		return 1;
	   }

	fseek(fileptr,0,SEEK_SET);

	if (!fread(&st_block, sizeof(struct user_data_start_block_struct), 1, fileptr))
	   {
		 log_error("*Error reading user start block");
		 log_error(userfile);
		 return 1;

		}

	number_users = st_block.num_users;

	if (number>=number_users)
		{
		   log_error("* SYSTEM tried to add user that exhisted");

		/* blank user data here */

		/* DEBUG!!! this needs to blank the user data to guest standing */

        memset(&temp,0,sizeof(temp));

		fseek(fileptr,
         (long int)sizeof(struct class_data_record)*(number_users) +
				   sizeof(union user_data_start_block) ,
		 SEEK_SET);

		for (putit=number_users;putit<number;putit++)
         if (!fwrite(&temp, sizeof(struct class_data_record), 1, fileptr))
			  {  log_error(userfile);
				 g_fclose(fileptr);
				 return 1;
			  }

		fseek(fileptr,0,SEEK_SET);
        st_block.num_users = number + 1;

		if (!fwrite(&st_block, sizeof(struct user_data_start_block_struct), 1, fileptr))
		   {
			 log_error("*Error writing user start block");
			 log_error(userfile);
			 return 1;
			}

		}

		fseek(fileptr,
         (long int)sizeof(struct class_data_record)*(number) +
				   sizeof(union user_data_start_block),
		 SEEK_SET);

        memset(&temp,0,sizeof(temp));
        temp.class.info = class_ptr->class_info;
        temp.move.info = class_ptr->move_info;

        if (!fwrite(&temp, sizeof(temp), 1, fileptr))
			 {  log_error(userfile);
				log_error("*tried to write user and failed");
				g_fclose(fileptr);
				return 1;
			 }
   fflush(fileptr);

   if (g_fclose(fileptr))
		{
		  log_error(userfile);
		  return 1;
		}
 return 0;

}



/* FUNCTIONS TO LOAD OTHER CLASSES ACCESS */

int load_access_of_class(char *class_name,struct u_parameters *user,int portnum)
{
   struct class_data tempdata;
   int loop;
   time_t now;

   lock_dos(73);
   time(&now);
   unlock_dos();

   fix_classname(class_name);

   if (load_class_by_name(class_name,&tempdata))
      {log_error("* tried to load class in access loader"); return 1;}

   strcpy(user_options[portnum].class_name,class_name);

   for(loop=0;loop<10;loop++)
    {
      user->privs[loop]=tempdata.class_info.privs[loop];
    }
   user->priority=tempdata.class_info.priority;
   if (tempdata.class_info.time==0)
     user->time=tempdata.class_info.time;
   else
     {
       if (tempdata.class_info.time<=((now-line_status[portnum].time_online)/60))
          user->time=(unsigned int)((long int)(now-line_status[portnum].time_online)/60)+1;
       else
          user->time=tempdata.class_info.time;
     }

   user->added_time=tempdata.class_info.added_time;
   for(loop=0;loop<4;loop++)
     user->staple[loop]=tempdata.class_info.staple[loop];


   /* NEED TO RECALC time_sec AND time_warning_sec */
   calc_time_for_node(portnum);
   /* NEED TO set handle line to be changed */
   line_status[portnum].handlelinechanged = ALL_BITS_SET;
   sync_status[portnum].handlelinechanged_at_tick = dans_counter;
  return 0; /* SUCCESSFUL */

}

void update_members_list(void)
{
    int data=(int)schedule_data[tswitch];
    char line[140];
    int number_users,number;
    char *outfile;
    int loop;
    int count=3;
    struct user_data user_ptr;
	struct user_data_start_block_struct st_block;
    FILE *fileptr,*fileptr2;
    char temptime[60];

    switch (data)
      {
         case  0  : outfile=user_member_list;
                    aput_into_buffer(server,"--> Updating USER's Member List",0,5,tswitch,9,0);
                    break;
         case  1  : outfile=sysop_member_list;
                    aput_into_buffer(server,"--> Updating SYSOP's Member List",0,5,tswitch,9,0);
                    break;
         default  : end_task();
                    break;
      }

    lock_dos(74);


    if (!(fileptr=g_fopen(userfile,"rb","USER#3")))
       {
        log_error("*user file wouldn't open in update member list");
        log_error(userfile);
        end_task();
       }
    if (!(fileptr2=g_fopen(outfile,"wb","USER#4")))
       {
        log_error("*could not open user member list");
        log_error(user_member_list);
        g_fclose(fileptr);
        end_task();
       }
    fseek(fileptr,0,SEEK_SET);
    fseek(fileptr2,0,SEEK_SET);

	if (!fread(&st_block,sizeof(struct user_data_start_block_struct),1,fileptr))
	{
				log_error("* fread() failed on file (during start block read) in member update");
					 log_error(userfile);
					 g_fclose(fileptr);
					 g_fclose(fileptr2);
					 unlock_dos();
					 end_task();	}

	number_users = st_block.num_users;

	for (number=0;number<number_users;number++)
	  {

        load_user_info(number,&user_ptr);

        if (user_ptr.user_info.number>=0)
          { int point;
           if (data)
            {
              sprintf(line,"#%03d : %c%s|*r1%c",user_ptr.user_info.number,user_ptr.class_info.staple[2],user_ptr.user_info.handle,user_ptr.class_info.staple[3]);

              point=ansi_strlen(line);

             // for(loop=0;loop<(40-point);loop++)


              for(loop=0;loop<(35-point);loop++)
                 strcat(line," ");

              // fprintf(fileptr2,"%s%-20s %s%c%c",line,user_ptr.real_info.name,user_ptr.real_info.phone,13,10);

              sprint_time(temptime,&(user_ptr.user_info.expiration));
              fprintf(fileptr2,"%s%-13s%s%c%c",line,
                 user_ptr.class_mod_info.class_name,temptime,13,10);
            }
            else
              fprintf(fileptr2,"#%03d : %c%s|*r1%c%c%c",user_ptr.user_info.number,user_ptr.class_info.staple[2],user_ptr.user_info.handle,user_ptr.class_info.staple[3],13,10);
          }
        if ((count--)<=0)
         {
           count=3;
           DosSleep(100l);
         }
         next_task();
      }

     lock_dos(76);


        if (g_fclose(fileptr))
           {
             log_error("g_fclose failed");
             log_error(userfile);
             g_fclose(fileptr2);
             unlock_dos();
             end_task();
           }
         if (g_fclose(fileptr2))
           { log_error("*fclose failed");
             log_error(user_member_list);
             unlock_dos();
             end_task();
           }


    unlock_dos();
    switch (data)
    { case 0  : aput_into_buffer(server,"--> Update of USER's Member List DONE",0,5,tswitch,9,0);
                break;
      case 1  : aput_into_buffer(server,"--> Update of SYSOP's Member List DONE",0,5,tswitch,9,0);
                break;
      default : aput_into_buffer(server,"--> (DEFAULT) Member List Update Done",0,5,tswitch,9,0);
                break;
    }
    end_task();


}


void log_event_for_user(char *filename,char *event,int number)
{
 char s[256];

    sprintf(s,"USER\\USER%03d\\%s",number,filename);

    log_event(s,event);
}

void make_user_dir(int number)
{
    char s[256];

    sprintf(s,"USER\\USER%03d",number);

    if (mkdir(s))
    {
      if (mkdir("USER"))
       {
         /* the directory is already there so clean it out */
         delete_files_function(s,"*");

       }
      else
      { /* it was not already there so now make the new part */

        if (mkdir(s))
          log_error("* Unable to create user home directory!!");
      }

    }

    /*
     *   Directories are made/cleaned in the "USER" tree
     */

}



void log_system_event_for_user(char *filename,char *event,int number)
{
 char s[256];

    sprintf(s,"USER\\USER%03d\\%s",number,filename);

    if (log_system_event(s,event))
    {
      make_user_dir(number);
      log_system_event(s,event);
    }
}

/*
 * this routine should clean out all directories and data files which
 * have to do with a user
 */

void prep_user_dirs(int number)
{
    char s[256];

    sprintf(s,"USER\\USER%03d",number);

    if (mkdir(s))
    {
      if (mkdir("USER"))
       {
         /* the directory is already there so clean it out */
         delete_files_function(s,"*");

       }
      else
      { /* it was not already there so now make the new part */

        if (mkdir(s))
          log_error("* Unable to create user home directory!!");
      }

    }

    /*
     *   Directories are made/cleaned in the "USER" tree
     */


    sprintf(s,"MAIL\\MAIL%03d",number);

    if (mkdir(s))
    {
      if (mkdir("MAIL"))
       {
         /* the directory is already there so clean it out */
         delete_files_function(s,"*");

       }
      else
      { /* it was not already there so now make the new part */

        if (mkdir(s))
          log_error("* Unable to create user home directory!!");
      }

    }

    /* ok, the mail dirs are clean */

    log_system_event_for_user("AUDIT.LOG","User directory Cleaned",number);
}

