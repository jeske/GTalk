


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */




/* headers */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include "newuser.h"
#include "ansi.h"
#include "userst.h"
#include "files.h"
#include "output.h"
#include "gtmain.h"
#include "input.h"

#include "log.h"



int find_free_user_number(int low,int high)
{
  int loop;

  for (loop=low;loop<high;loop++)
  {
	if (!exist(loop))
	   return (loop);
  }

  return (-1);
}

int verbose_get_yes_no(char *prompt)
{
  int count=3;
  int result=0;
  int old_state = ansi_on(1);
  char input[4];

  while (count--)
  {
	print_string(prompt);
	print_string("|*f4|*h1[|*r1|*h1YES|*f4|*h1/|*r1|*h1NO|*f4|*h1]|*r1");

	do {
	get_input(input,3);
	} while (!*input);

	fix_classname(input);

	if (!strcmp(input,"YES"))
	{ count=0;
	  result=1;
	}
	if (!strcmp(input,"NO"))
	{ count=0;
	  result=0;
	}

  }

  ansi_on(old_state);
  return (result);
}


void nu_string_prompt(char *prompt,char *string,int length)
{
  int old_state;
  
  print_cr();
  old_state = ansi_on(1);
  print_str_cr(prompt);
  print_string(": |*f6|*h1");
  
  do {
    get_input(string,length);
  } while (!*string);
  ansi_on(old_state);

}

void nu_fix_phone(char *string)
{
 char *to,*from;


 to=from=string;

 while (*from)
 {
	 if ( ((*from)>='0') && ((*from)<='9'))
	 {
	   *to=*from;
	   to++;
	   from++;
	 }
	 else
	  from++;
 }
 *to=0;

/*
 if (strlen(string)==7)
  {
	char temp[USER_PHONE_LEN];

	strcpy(temp,string);
	strncpy(string,sys_info.sys_phone,3);
	strcpy(string+3,temp);

  }
 */
}

void nu_print_phone(char *string)
{
	int old_code = ansi_on(1);

	print_string("|*r1|*h1(|*f6|*h1");
	print_string_len(string,'.',0,3);
	print_string("|*r1|*h1)|*f6|*h1");
	print_string_len(string,'.',3,3);
	print_string("|*r1|*h1-|*f6|*h1");
	print_string_len(string,'.',6,4);

	print_string("|*r1");
	ansi_on(old_code);

}


int nu_get_date_string(char *string,int limit,int start_pos)
{
  int pos = start_pos;
  int key;
  int flag = 1;
  int stage = 1;
  int retval = 1;
  
  while (flag) 			                        /* wait while editing */
    {
      key = wait_ch();
      if (((key == 8) || (key == 127)))
	if (pos>0)
	  {
	    pos--; 					/* if an edit key is pressed and there's more to */
	    print_string(backspace_string);              /* erase, erase the character */
	  }						/* and go back one */
	else
	  flag=0;
      
      if (key == 27)					/* if we abort, then clear all characters */
	{
	  flag = 0;
	  if (pos)					/* print a backslash to indicate abort */
	    {
	      print_chr('\\');
	      print_chr(13);
	      print_chr(10);
	      pos = 0;
	    };
	};
      
      if ((((key >= '0') && (key <= '9')) || ((key=='/') && pos)) && (pos < limit))
	{						/* insert the character if there's room */
	  *(string+pos) = key;
	  if (key == '+')        /* if +, don't let it be typed normally */
	    {
	      print_chr(key); 	/* print the character with a space */
	      print_chr(32);		/* and a backspace */
	      print_chr(8);
	    }
	  else
	    print_chr(key);		/* otherwise, print it normally */
	  pos++;
	};
      
      if (pos == limit)
	flag=0;
    };
  *(string+pos) = 0;			/* mark end of the string */
  if (pos==0)
    retval=0;
  
  return (retval);
}

void nu_get_phone(char *string,int abortable)
{
   int pos = 0;
   int key;
   int flag = 1;
   int stage = 1;
   int limit = 10;
   int old_special = ansi_on(1);

   print_string("|*r1|*h1(|*f6|*h1");

   while (flag) 			/* wait while editing */
	{
	  key = wait_ch();
	  if (((key == 8) || (key == 127)))
	  if (pos>0)
	   {
		 pos--; 						  /* if an edit key is pressed and there's more to */
		 print_string(backspace_string);   /* erase, erase the character */
	   }								  /* and go back one */

	  if (key == 13)	/* finish the line */
	   {
		 if (pos==limit)
		  {
			flag = 0;
			print_chr(13);
			print_chr(10);
		  }
		 else
		 {
		  if ((abortable) && (pos==0))
		  {
			  flag = 0;
			  print_string(backspace_string);   /* erase, erase the character */
			  print_string("None.");
			  print_cr();
		  }

		 }

	   };
	  if (((key >= '0') && (key <= '9')) && (pos < limit))
	   {					/* insert the character if there's room */
		 *(string+pos) = key;
		 print_chr(key);		/* otherwise, print it normally */
		 pos++;
	   };

	   switch (stage)
	   {
		 case 1:
				 if (pos==3)
				   {
					 print_string("|*r1|*h1)|*f6|*h1");
					 stage=2;
				   }
				 break;

		 case 2:
				 if (pos<3)
					{
					 print_string(backspace_string);   /* erase, erase the character */
					 stage=1;
					}
				 else
				 if (pos==6)
					{
					  print_string("|*r1|*h1-|*f6|*h1");
					  stage=3;
					}
				 break;
		 case 3:
				if (pos<6)
					{
					  print_string(backspace_string);   /* erase, erase the character */
					  stage=2;
					}
				 break;
		 default:break;
		}

	};

   print_string("|*r1");
   ansi_on(old_special);
   *(string+pos) = 0;			/* mark end of the string */
}

void nu_phone_prompt(char *prompt,char *string,int abortable)
{
	int old_code = ansi_on(1);
	print_cr();

	print_str_cr(prompt);
	print_string(": ");

	nu_get_phone(string,abortable);
	ansi_on(old_code);
}


void nu_print_date(struct exp_date *date,int limit)
{
  char s[30];
  int old_code = ansi_on(1);

  sprintf(s,"|*f6|*h1%02d",date->month);
  print_string(s);
  print_string("|*r1|*h1/");
  sprintf(s,"|*f6|*h1%02d",date->day);
  print_string(s);
  print_string("|*r1|*h1/");
  sprintf(s,"|*f6|*h1%02d",date->year);
  print_string(s);
  print_string("|*r1");
  ansi_on(old_code);

}


void nu_get_date(struct exp_date *date,int limit)
{
	char s[30];
	int flag=1;
	int stage=1;
	int direction=1;
	int pos=0;
	int key;
	int old_special = ansi_on(1);

	print_string("|*f6|*h1");

	while (flag)
	{
		switch (stage)
		{
			case 1:
					sprintf(s,"%02d",date->month);

					if (direction)
					  direction = nu_get_date_string(s,2,0);
					else
					  direction = nu_get_date_string(s,2,1);

					date->month = atoi(s);

					break;
			case 2:
					sprintf(s,"%02d",date->day);
					if (direction)
					  direction = nu_get_date_string(s,2,0);
					else
					  direction = nu_get_date_string(s,2,1);

					date->day = atoi(s);

					break;
			case 3:

					sprintf(s,"%02d",date->year);

					if (direction)
					  direction = nu_get_date_string(s,2,0);
					else
					  direction = nu_get_date_string(s,2,1);

					date->year = atoi(s);

					break;
			default:
					break;
		}


		if ((stage==3) && (direction))
		{
		  while ((direction) && (flag))
		  {
			  key = wait_ch();
			  if ((key==10) || (key==13))
				flag=0;
			  else
			  if ((key==8) || (key==127))
			  {
				direction=0;
				/* erase, erase the character */
				print_string(backspace_string);	 
				
			  }
		  }
		}
		else
		if ((stage==1) && (direction==0))
		{
			direction=1;
		}
		else
		{
			if (stage<1)
			  {
			  stage=1;
			  direction=1;
			  }
			else
			if (stage>3)
			  stage=3;
			else
			{
				if (direction)
				  {
					if (s[1]=='/')
					  {
					   int num;
					   s[1]=0;
					   num = atoi(s);
					   sprintf(s,"%02d",num);
					   /* erase, erase the character */
					   print_string(backspace_string);
					   print_string(backspace_string);
					   print_string(s);

					  }
					print_string("|*r1|*h1/|*f6|*h1");
					stage++;
				  }
				else
				  {
				  stage--;
				  /* erase, erase the character */
				  print_string(backspace_string);   
				  print_string(backspace_string); 
				  }
			}
		}
	      }

	reset_attributes();
  print_cr();

  ansi_on(old_special);

};

void nu_date_prompt(char *prompt,struct exp_date *ptr)
{
	char s[20];
	int old_special = ansi_on(1);

	print_cr();
	ansi_on(1);
	print_str_cr(prompt);
	print_string("|*f3|*h1MM|*r1|*h1/|*f3|*h1DD|*r1|*h1/|*f3|*h1YY|*r1|*h1: |*r1");

	nu_get_date(ptr,6);
	ansi_on(old_special);
}

int nu_get_number(char *prompt,int def,int low,int high)
{
  char s[20];
  int retval;
  int ansi_state = ansi_on(1);

  print_cr();
  print_string(prompt);
  
  sprintf(s," [%d]: ",def);
  print_string(s);
  
  get_input(s,15);
  if (!*s)
    { sprintf(s,"%d",def);
      print_str_cr(s);
      retval = def;
    }
  else
    {
      retval = atoi(s);
      if (retval<low)
	retval = low;
      else
	if (retval>high)
	  retval = high;
    }
  
  ansi_on(ansi_state);
  return (retval);
}


void new_user_application(struct user_data *ptr)
{

	nu_string_prompt("|*f4|*h1Enter your |*r1|*h1real name|*f4|*h1 (first and last).",
					 ptr->real_info.name,USER_REAL_NAME_LEN);

	nu_string_prompt("|*f4|*h1Enter your |*r1|*h1Street Address|*f4|*h1.",
					 ptr->real_info.street,USER_STREET_LEN);

	nu_string_prompt("|*f4|*h1Enter your |*r1|*h1city|*f4|*h1.",
					ptr->real_info.city,USER_CITY_LEN);

	nu_string_prompt("|*f4|*h1Enter your |*r1|*h1State or Province|*f4|*h1.",
					ptr->real_info.state_or_province,USER_STATE_LEN);

	nu_string_prompt("|*f4|*h1Enter your |*r1|*h1Postal Code|*f4|*h1.",
					ptr->real_info.postal_code,USER_POSTAL_CODE_LEN);

	nu_phone_prompt("|*f4|*h1Enter your |*r1|*h1home/voice phone|*f4|*h1 number. (Required)",
					ptr->real_info.phone,0);

	nu_phone_prompt("|*f4|*h1Enter your |*r1|*h1data/fax phone|*f4|*h1 number. (Optional)",
					ptr->real_info.phone2,1);

	nu_string_prompt("|*f4|*h1Enter your |*r1|*h1Country|*f4|*h1.",
					ptr->real_info.country,USER_COUNTRY_LEN);

	nu_date_prompt("|*f4|*h1Enter your |*r1|*h1Birthdate|*f4|*h1.",
					&ptr->real_info.birth_date);

};

void add_to_validation_queue(int number)
{
 char s[256];

 sprintf(s,"User #%03d Applied",number);
 log_system_event("LOG\\APPL.LOG",s);

}


void nu_get_password(char *password,int length)
{
	char s[PASSWORD_LEN+1];
	char s2[PASSWORD_LEN+1];
	int flag=1;
	int old_code = ansi_on(1);

   while (flag)
   {
	   print_string("|*f4|*h1Enter |*r1|*h1Password|*f4|*h1 : ");
	   *s=0;

	   empty_inbuffer();
	   do {
	   get_input_cntrl(s,length,GI_FLAG_MASK_ECHO);
	   } while (!*s);

	   print_string("|*f4|*h1Enter |*r1|*h1Password|*f4|*h1 Again : ");

	   empty_inbuffer();
	   do {
	   get_input_cntrl(s2,length,GI_FLAG_MASK_ECHO);
	   } while (!*s2);

	   if (!strcmp(s,s2))
		  flag=0;
	   else
	   {
		  print_cr();
		  print_str_cr("|*f4|*h1Sorry, those passwords did not match");
		  print_str_cr("please reenter your password.|*r1");
		  print_cr();

	   }

	}

	strcpy(password,s);
	print_cr();
	print_str_cr("Password Accepted");


	ansi_on(old_code);
}

void set_startup_class_information(struct user_data *ptr)
{
  strcpy(ptr->user_info.class_name,"NEW");
  ptr->user_info.width=80;
}



int new_user_app_common(void)
{
	struct user_data temp_info;
	int number;
	char s[80];


	if (!class_exists("NEW"))
	{
	  print_file_cntrl(NO_NEW_USERS_FILE,PFC_ANSI);
	  return 0;
	}


	memset(&temp_info,0,sizeof(temp_info));
	print_cr();

	print_file_cntrl(NEW_USER_FILE1,(PFC_ANSI | PFC_PAGING)); 

	if (!verbose_get_yes_no("|*f4|*h1Are You sure you want to Continue?|*r1 "))
	{
	  return 0;
	}

	print_file_cntrl(NEW_USER_HANDLE_INFO,(PFC_ANSI | PFC_PAGING));

	nu_string_prompt("|*f4|*h1Enter |*r1|*h1Handle|*f4|*h1: ",
			 temp_info.user_info.handle,HANDLE_LEN);

	new_user_application(&temp_info);

	temp_info.user_info.width = mynode->userdata.online_info.width =
		nu_get_number("|*f4|*h1Please enter the |*r1|*h1width|*f4|*h1 of your screen ",80,20,255);

	print_cr();
	/*
	 * get them a user number
	 */

	ansi_on(1);
	print_string("|*f2|*h1Please Wait:|*r1|*f2 finding free user number...");

	number = find_free_user_number(20,1000);
	if (number==-1)
	  {
		print_str_cr("ERROR!! could not get free user number.");
		return;
	  }
	else
	sprintf(s,"[%03d]",number);
	print_str_cr(s);
	ansi_on(0);

	print_file_cntrl(NEW_USER_FILE2,(PFC_ANSI | PFC_PAGING));

	nu_get_password(temp_info.user_info.password,PASSWORD_LEN);

	print_cr();

	print_file_cntrl(NEW_USER_FILE3,(PFC_ANSI | PFC_PAGING));

	ansi_on(1);
	sprintf(s,"|*f4|*h1     Number : [|*r1|*h1%03d|*f4|*h1]",number);
	print_str_cr(s);
	ansi_on(0);

	if (!verbose_get_yes_no("|*f4|*h1Are You sure you want to Continue?|*r1 "))
	{
	  return 0;
	}

	ansi_on(1);
	print_string("|*f2|*h1Please Wait:|*r1|*f2 preparing user directories...");
	prep_user_dirs(number);
	print_str_cr("Done.");


	print_string("             saving your user information...");
	set_startup_class_information(&temp_info);
	temp_info.user_info.user_no = number;
	temp_info.user_info.enable = 1;
    temp_info.user_info.conception =
        temp_info.user_info.last_call = time(NULL);
    save_user_record(number,&temp_info);
	print_str_cr("Done.");

	print_string("             adding to validation queue...");
	add_to_validation_queue(number);
	print_str_cr("Done.");
	ansi_on(0);

	print_cr();

	print_file_cntrl(NEW_USER_BEFORE_EDITFILE,(PFC_ANSI | PFC_PAGING));

	wait_for_return();

	sprintf(s,"USER\\USER%03d\\APPL.TXT",number);
	line_editor(s,4096);

	ansi_on(1);
	sprintf(s,"|*f2|*h1Remember your Account Number : [|*r1|*h1%03d|*f2|*h1]",number);
	print_str_cr(s);
	ansi_on(0);

	return 1;
}

void new_user_app_command(char *str,char *name,int portnum)
{

	if (new_user_app_common())
	 {
	   mynode->userdata.online_info.newuser_apps++;
	   print_sys_mesg("Application Completed");
	 }
	else
	 print_sys_mesg("Application Aborted");

}

int new_user_app(void)
{
	int result=1;

	if (mynode->userdata.online_info.newuser_apps)
	  { print_cr();
		print_str_cr("You already applied!");
		return (0);
	  }

	if (new_user_app_common())
	{
	  mynode->userdata.online_info.newuser_apps++;
	  print_file_cntrl(NEW_USER_FILE4,(PFC_ANSI|PFC_PAGING));
	  result=1;
	}
	else
	 {
	   result=0;
	   print_str_cr("[Application Aborted]");
	   print_cr();
	 }

  return (result);
}


int online_validation(int portnum)
{
  int phone_matched=0;
  int choice;
  int old_code;
  char s[256];
  struct user_data disk_data;
  
  
  if (mynode->userdata.user_info.user_no<0)
    { 
      printf_ansi("Guests cannot online validate\n"); 
      return;
    }

  if (read_user_record(mynode->userdata.user_info.user_no, &disk_data))
    {
      log_error("Error loading user data in online_validate()");
      return;
    }

  /* check to see if they can't be validated */
  
  if (!mynode->userdata.online_info.call_info)
    {
      print_file_cntrl(VALIDATION_INFO_UNAVAILABLE_FILE,PFC_ANSI);
      return (1);
    }
  if (strlen(mynode->userdata.online_info.call_info->number)<6)
    {
      print_file_cntrl(VALIDATION_OUT_OF_SERVICE_AREA_FILE,PFC_ANSI);
      return (1);
    }
  
  /* ok, their set, so let them in */
  
  print_file_cntrl(VALIDATION_WELCOME_FILE,PFC_ANSI);
  
  nu_fix_phone(mynode->userdata.online_info.call_info->number);
 
  nu_fix_phone(disk_data.real_info.phone);
  nu_fix_phone(disk_data.real_info.phone2);
  
  if (!strcmp(mynode->userdata.online_info.call_info->number,disk_data.real_info.phone))
    {
      phone_matched=1;
      old_code = ansi_on(1);
      print_str_cr("|*r1|*f4|*h1The number you are calling from has been identified");
      print_string("as your |*r1|*h1Voice|*f4|*h1 line.");
      ansi_on(old_code);
    }
  else
    if (!strcmp(mynode->userdata.online_info.call_info->number,disk_data.real_info.phone2))
      {
	phone_matched=1;
	old_code = ansi_on(1);
	print_str_cr("|*r1|*f4|*h1The number you are calling from has been identified");
	print_string("as your |*r1|*h1Data/Fax|*f4|*h1 line.");
	ansi_on(old_code);
      }
    else
      {
	print_file_cntrl(VALIDATION_NO_MATCH_FILE,PFC_ANSI);
	old_code = ansi_on(1);
	print_cr();
	if (!get_yes_no("|*r1|*f4|*h1Are you calling from home?|*r1|*h1 "))
	  {
	    print_file_cntrl(VALIDATION_NOT_AT_HOME_FILE,(PFC_ANSI));
	    return (1);
	  }
	
	print_cr();
	do
	  {
	    print_str_cr("|*r1|*f4|*h1Which of your lines are your calling from now?");
	    print_str_cr("|*f6|*h11|*f4|*h1) |*r1|*h1Voice");
	    print_str_cr("|*f6|*h12|*f4|*h1) |*r1|*h1Data/FAX");
	    print_str_cr("|*f6|*h13|*f4|*h1) |*r1|*h1Neither");
	    print_cr();
	    print_string("|*r1|*h1: |*f6|*h1");
	    choice = get_number();
	    if (choice<0 || choice>3)
	      { print_str_cr("|*r1|*f4|*h1Invalid");
		choice = 0;
	      }
	  }
	while (!choice);
	
	ansi_on(old_code);
	
	switch (choice)
	  {
	  case 3:	print_file_cntrl(VALIDATION_NOT_AT_HOME_FILE,PFC_ANSI);
	    return (1);
	    break;
	  case 2:	sprintf(s,"(A) Data Num Change [%s]->[%s]",
				disk_data.real_info.phone2,
				mynode->userdata.online_info.call_info->number);
	    
	    log_system_event_for_user("AUDIT.LOG",s,mynode->userdata.user_info.user_no);
	    phone_matched=1;
	    strcpy(disk_data.real_info.phone2,mynode->userdata.online_info.call_info->number);
	    break;
	  case 1:	sprintf(s,"(A) Voice Num Change [%s]->[%s]",
				disk_data.real_info.phone2,
				mynode->userdata.online_info.call_info->number);
	    
	    log_system_event_for_user("AUDIT.LOG",s,mynode->userdata.user_info.user_no);
	    phone_matched=1;
	    strcpy(disk_data.real_info.phone,mynode->userdata.online_info.call_info->number);
	    break;
	  }
      }
  
  if (!phone_matched)
    {
      print_file_cntrl(VALIDATION_NOT_AT_HOME_FILE,PFC_ANSI);
      return (1);
    }
  
  mynode->userdata.user_info.validate_info = USER_CALLER_ID_VALIDATED;
  
  old_code = ansi_on(1);
  
  print_string("|*r1|*f4|*h1Your calling from|*r1|*h1: ");
  nu_print_phone(mynode->userdata.online_info.call_info->number);
  print_cr();
  if (mynode->userdata.online_info.call_info->name[0])
    {
      print_string("|*r1|*f4|*h1Registered to|*r1|*h1: ");
      nu_print_phone(mynode->userdata.online_info.call_info->name);
      print_cr();
      sprintf(s,"(A) Validate Info [%s]-[%s]",
	      mynode->userdata.online_info.call_info->number,
	      mynode->userdata.online_info.call_info->name);
    }
  else
    {
      sprintf(s,"(A) Validate Info [%s]",
	      mynode->userdata.online_info.call_info->number);
    }
  
  print_str_cr("|*r1|*f4|*h1You have been Validated.");
  log_system_event_for_user("AUDIT.LOG",s,mynode->userdata.user_info.user_no);
  
  if (!strcmp("NEW",mynode->userdata.user_info.class_name))
    {
      strcpy(mynode->userdata.user_info.class_name,"REG_GUEST");
      printf_ansi("You are now a |*r1[|*h1RGA|*r1]|*f4|*h1!\n","REG_GUEST");
      sprintf(s,"User #%03d was auto-validated",mynode->userdata.user_info.user_no);
      log_system_event("LOG/APPL.LOG",s);
    }

  disk_data.user_info = mynode->userdata.user_info;
  save_user_record(disk_data.user_info.user_no,&disk_data);
  
  wait_for_return();
  
  ansi_on(old_code);
  print_file_cntrl(VALIDATION_EXIT_FILE,PFC_ANSI);
  return (0);
}



