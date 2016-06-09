/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - accounting.c
 *
 * Contains the code for the new financial accounting stuff.
 *
 */

#include <sys/types.h>
#include <time.h>
#include "userst.h"
#include "user.h"
#include "accounting.h"
#include "input.h"
#include "userst.h"
#include "log.h"
#include "user.h"
#include "gtmain.h"
#include "shared.h"
#include "output.h"


int purchase_units(char *name, int cost, int max_credits)
{
  int number;

  if (!cost) {
    printf("\r\n***** %ss are not for sale\r\n",name);
    return 0;
  }
  
  printf("\r\n***** %ss cost %dcr each.",name,cost);
  number = nu_get_number("How many would you like?",1,0,max_credits/cost);
  
  if (number<1)
    return 0;
  
  if (number==1)
    printf("Purchasing [%d] %s will cost you %dcr.\r\n",
	   number,name,cost*number);
  else
    printf("Purchasing [%d] %ss will cost you %dcr.\r\n",
	   number,name,cost*number);
   if (!get_yes_no("Are you Sure?"))
     return 0;

   return number;
}


int add_time_to_struct(struct unique_information_struct *usr_dest,
			char purchase_type, int purchase_count)
{
  time_t multiplier;
  time_t now = time(NULL);

  if (usr_dest->expiration<now)
    usr_dest->expiration = now;

  switch(purchase_type)
    {
    case 'D':
      multiplier = 86400l;
      break;
    case 'M':
      multiplier = 86400l*30;
      break;
    case 'Y':
      multiplier = 86400l*30*365;
      break;
    default:
      log_error("Unknown Purchase Type [%c]",purchase_type);
      return -1;
    }

  usr_dest->expiration += multiplier * purchase_count;

  return 0;
}


int payment_selection(struct unique_information_struct *usr,
		      struct class_defined_data_struct *usrcls,
		      struct unique_information_struct *usr_dest,
		      char purchase_type,char *purchase_type_name,
		      int purchase_count, int purchase_cost)
{
  int remaining_cost = purchase_cost;
  int free_credits_cost=0;
  int account_balance_cost=0;
  int credit_card_cost=0;
  int temp_amount;
  int def_amount;
  int auto_select=0;
  int temp_max;
  char option[2] = " ";
  char temp_string[40];


  /* first see if they only have one payment method, that will
     make things much easier for most users */

  if ((!usr->free_credits) && (!usrcls->credit_card_limit)) {
    int available_credits;

    if (usr->account_balance>0)
      available_credits = usr->account_balance;
    else
      available_credits = usrcls->account_overdraft_limit +
	usr->account_balance;
    
    if (available_credits>=remaining_cost) {
      account_balance_cost = remaining_cost;
      remaining_cost=0;
      auto_select=1;
    } else {
      return -1;
    }  
  }    

  /* now, while there is cost remaining to be paid, ask them where
     to pay for it from */

  while (remaining_cost) {
    
    printf("\r\n***** Payment Selection\r\n");
    
    printf("       Total Cost: %dcr\r\n",purchase_cost);
    printf("   Remaining Cost: %dcr\r\n",remaining_cost);
    printf("\r\n***** Choose a Payment Method                       Payment Amount\r\n");
    printf("[1] Personal Account  Avail: %05dcr (Bal:%05dcr)    %dcr\r\n",
	   (usr->account_balance>0 ? usr->account_balance:
	   (usr->account_balance + usrcls->account_overdraft_limit)),
	   usr->account_balance, account_balance_cost);
    printf("[2] Credit Card       Avail: %05dcr (Bal:%05dcr)    %dcr\r\n",
	    usrcls->credit_card_limit - usr->credit_card_balance,
	   usr->credit_card_balance, credit_card_cost);
    printf("[3] Free Credits      Avail: %05dcr             %dcr\r\n",
	   usr->free_credits, free_credits_cost);
    printf("\r\n");
    get_input_prompt_cntrl("Enter Option",option,1,GI_FLAG_TOUPPER);

    def_amount = remaining_cost;

    temp_amount = 0;
    switch(*option)
      {
      case '1':
	temp_max = (usr->account_balance>0 ? 
		    (usr->account_balance - account_balance_cost):
		    (usrcls->account_overdraft_limit + usr->account_balance -
		     account_balance_cost));
	if (def_amount > temp_max)
	  def_amount = temp_max;
	else
	  temp_max = def_amount;
	temp_amount = nu_get_number("Enter Amount",def_amount,0,temp_max);
	account_balance_cost += temp_amount;
	remaining_cost -= temp_amount;
	break;
      case '2':
	temp_max = usrcls->credit_card_limit -
	  (usr->credit_card_balance + credit_card_cost);
	if (def_amount > temp_max)
	  def_amount = temp_max;
	else
	  temp_max = def_amount;
	temp_amount = nu_get_number("Enter Amount", def_amount,0,temp_max);
	credit_card_cost += temp_amount;
	remaining_cost -= temp_amount;
	break;
      case '3':
	temp_max = usr->free_credits - free_credits_cost;
	if (def_amount > temp_max)
	  def_amount = temp_max;
	else
	  temp_max = def_amount;
	temp_amount = nu_get_number("Enter Amount", def_amount,0,temp_max);
	free_credits_cost += temp_amount;
	remaining_cost -= temp_amount;
	break;
      default:
	printf("Invalid Option\r\n");
	return;
      }

}

/* ok, there is no more remaining cost, so make the change complete */

  if (!add_time_to_struct(usr_dest,purchase_type,purchase_count))
    {
      /* apply cost */

      printf("\r\n***** Payment Selection Complete - Billing Summary\r\n");
      if (account_balance_cost)
	printf("Personal Account Cost: %dcr\r\n",account_balance_cost);
      if (free_credits_cost)
	printf("    Free Credits Cost: %dcr\r\n",free_credits_cost);
      if (credit_card_cost)
	printf("     Credit Card Cost: %dcr\r\n",credit_card_cost);

      printf("\r\n");
      if (!auto_select) {
	int count=0;
	if (!get_yes_no("Is this Payment Selection Correct?"))
	  return -1;
	{
	  time_t now;
	  struct tm time_now;
	  int count=0;
	  
	  now = time(NULL);
	  time_now = *localtime(&now);
	  
	  
	  temp_string[0]=0;
	  if (account_balance_cost) {
	    sprintf(temp_string+strlen(temp_string),"A%d",account_balance_cost);
	    count=1;
	  }
	  if (free_credits_cost) {
	    if (count) 
	      strcat(temp_string,"/");
	    sprintf(temp_string+strlen(temp_string),"F%d",free_credits_cost);
	    count=1;
	  }
	  if (credit_card_cost) {
	    if (count)
	      strcat(temp_string,"/");
	    sprintf(temp_string+strlen(temp_string),"CC%d",credit_card_cost);
	    count=1;
	  }
	  
	  log_user_event("bank.log",usr->user_no,
			 "%02d/%02d/%02d #%03d |*f9Purchase    |*r1: (%05dcr) %d %s%s -%s",
			 time_now.tm_mon+1,time_now.tm_mday,time_now.tm_year,
			 usr->user_no,purchase_cost,purchase_count,
			 purchase_type_name,(purchase_count==1 ? "" : "s"),
			 temp_string);
	  log_event("log/bank.log",
		    "Purchase (%05d): #%03d %d%s%s -%s",
		    purchase_cost,usr->user_no,
		    purchase_count,
		    purchase_type_name,(purchase_count==1 ? "" : "s"),
		    temp_string);
	}

      } else {
	{
	  time_t now;
	  struct tm time_now;
	  int count=0;
	  
	  now = time(NULL);
	  time_now = *localtime(&now);
	  
	  log_user_event("bank.log",usr->user_no,
			 "%02d/%02d/%02d #%03d |*f9Purchase    |*r1: (%05dcr) %d %s%s for %dcr (New Bal:%dcr)",
			 time_now.tm_mon+1,time_now.tm_mday,time_now.tm_year,
			 
			 usr->user_no,purchase_cost,purchase_count,purchase_type_name,
			 (purchase_count==1 ? "" : "s"),purchase_cost,
			 usr->account_balance - account_balance_cost);
	  log_event("log/bank.log",
		  "Purchase: #%03d %d %s%s for %dcr (New Bal:%dcr)",
		    usr->user_no,
		    purchase_count,purchase_type_name,
		    (purchase_count==1 ? "" : "s"),purchase_cost,
		    usr->account_balance - account_balance_cost);
	}
      }
      

      usr->account_balance -= account_balance_cost;
      usr->free_credits -= free_credits_cost;
      usr->credit_card_balance += credit_card_cost;

      printf("***** Payment Complete\r\n");
    } else
      { printf("Error adding time\r\n"); return -1;}
  return 0;
}


int credit_movement_selection(struct unique_information_struct *usr,
		      struct class_defined_data_struct *usrcls,
		      char *reason,int purchase_cost, int super_access,
			      int recipiant_userno)
{
  int remaining_cost = purchase_cost;
  int free_credits_cost=0;
  int account_balance_cost=0;
  int credit_card_cost=0;
  int credits_created=0;
  int temp_amount;
  int def_amount;
  int temp_max;
  char option[2] = " ";
  char temp_string[50];


  /* now, while there is cost remaining to be paid, ask them where
     to pay for it from */

  while (remaining_cost) {
    
    printf("\r\n***** Payment Selection\r\n");
    
    printf("       Total Cost: %dcr\r\n",purchase_cost);
    printf("   Remaining Cost: %dcr\r\n",remaining_cost);
    printf("\r\n***** Choose a Payment Method                       Payment Amount\r\n");
    printf("[1] Personal Account  Avail: %05dcr (Bal:%05dcr)    %dcr\r\n",
	   usr->account_balance + usrcls->account_overdraft_limit, 
	   usr->account_balance,account_balance_cost);
    printf("[2] Credit Card       Avail: %05dcr (Bal:%05dcr)    %dcr\r\n",
	   usrcls->credit_card_limit - usr->credit_card_balance, 
	   usr->credit_card_balance, credit_card_cost);
    printf("[3] Free Credits      Avail: %05dcr                    %dcr\r\n",
	   usr->free_credits, free_credits_cost);
    if (super_access) {
      printf("[4] Create New Credits                                   %dcd\r\n",
	     credits_created);
    }
    printf("\r\n");
    get_input_prompt_cntrl("Enter Option",option,1,GI_FLAG_TOUPPER);

    def_amount = remaining_cost;

    temp_amount = 0;
    switch(*option)
      {
      case '1':
	temp_max = usr->account_balance + account_balance_cost + 
	  usrcls->account_overdraft_limit;
	if (def_amount > temp_max)
	  def_amount = temp_max;
	else
	  temp_max = def_amount;
	temp_amount = nu_get_number("Enter Amount",def_amount,0,temp_max);
	account_balance_cost += temp_amount;
	remaining_cost -= temp_amount;
	break;
      case '2':
	temp_max = usrcls->credit_card_limit -
	  (usr->credit_card_balance + credit_card_cost);
	if (def_amount > temp_max)
	  def_amount = temp_max;
	else
	  temp_max = def_amount;
	temp_amount = nu_get_number("Enter Amount", def_amount,0,temp_max);
	credit_card_cost += temp_amount;
	remaining_cost -= temp_amount;
	break;
      case '3':
	temp_max = usr->free_credits - free_credits_cost;
	if (def_amount > temp_max)
	  def_amount = temp_max;
	else
	  temp_max = def_amount;
	temp_amount = nu_get_number("Enter Amount", def_amount,0,temp_max);
	free_credits_cost += temp_amount;
	remaining_cost -= temp_amount;
	break;
      case '4':
	if (super_access) {
	  temp_max = 4000000l;
	  temp_amount = nu_get_number("Enter Amount", def_amount,0,temp_max);
	  credits_created += temp_amount;
	  remaining_cost -= temp_amount;
	} else {
	  printf("Invalid Option\r\n");
	}
	break;
      default:
	printf("Invalid Option\r\n");
	break;
      }

}

  /* ok, there is no more remaining cost, so make the change complete */
  
  /* apply cost */
  
  printf("\r\n***** Payment Selection Complete - Billing Summary\r\n");
  if (account_balance_cost)
    printf("Personal Account Cost: %dcr\r\n",account_balance_cost);
  if (free_credits_cost)
    printf("    Free Credits Cost: %dcr\r\n",free_credits_cost);
  if (credit_card_cost)
    printf("     Credit Card Cost: %dcr\r\n",credit_card_cost);
  if (credits_created)
    printf("      Credits Created: %dcr\r\n",credits_created);
  printf("\r\n");
  if (!get_yes_no("Is this Payment Selection Correct?"))
    return -1;

  {
    time_t now;
    struct tm time_now;
    int count=0;

    now = time(NULL);
    time_now = *localtime(&now);

    temp_string[0]=0;
    if (account_balance_cost) {
      sprintf(temp_string+strlen(temp_string),"Ac%d",account_balance_cost);
      count=1;
    }
    if (free_credits_cost) {
      if (count) 
	strcat(temp_string,"/");
      sprintf(temp_string+strlen(temp_string),"Fr%d",free_credits_cost);
      count=1;
    }
    if (credit_card_cost) {
      if (count)
	strcat(temp_string,"/");
      sprintf(temp_string+strlen(temp_string),"CC%d",credit_card_cost);
      count=1;
    }
    if (credits_created) {
      if (count)
	strcat(temp_string,"/");
      sprintf(temp_string+strlen(temp_string),"Cr%d",credits_created);
      count=1;
    }
	

    log_user_event("bank.log",usr->user_no,
      "%02d/%02d/%02d #%03d |*f9Payment Sent|*r1: (%05dcr) To #%03d %s -%s",
		   time_now.tm_mon+1,time_now.tm_mday,time_now.tm_year,
		   usr->user_no,purchase_cost,recipiant_userno,reason,
		   temp_string);
    
    log_user_event("bank.log",recipiant_userno,
      "%02d/%02d/%02d #%03d |*faPayment Rcvd|*r1: (%05dcr) Frm#%03d %s ",
		   time_now.tm_mon+1,time_now.tm_mday,time_now.tm_year,
		   recipiant_userno,purchase_cost,usr->user_no,reason);
  }
  log_event("log/bank.log",
	    "Payment: Frm#%03d To #%03d %s -%s",
	    usr->user_no,recipiant_userno,reason,			    
	    temp_string);


  usr->account_balance -= account_balance_cost;
  usr->free_credits -= free_credits_cost;
  usr->credit_card_balance += credit_card_cost;
  
  printf("***** Payment Complete\r\n");
  return 0;
}


int do_purchase(struct unique_information_struct *usr,
		struct class_defined_data_struct *usrcls,
		int at_login)
{
  int purchase_count=0;
  char option[3] = " ";
  int purchase_cost;
  char *purchase_type_name;
  int is_expired = usr->expiration<time(NULL);

  int account_spendable = 
    ((usr->account_balance>0) ? usr->account_balance :
     (usr->account_balance + ((is_expired) ? 
			      usrcls->account_overdraft_limit : 0)));
  int spendable_credits = account_spendable + usr->free_credits;

  if ((usrcls->class_cost_per_year) && 
      (account_spendable>=usrcls->class_cost_per_year))
    option[0] = 'Y';
  else if ((usrcls->class_cost_per_month) && 
	   (account_spendable>=usrcls->class_cost_per_month))
    option[0] = 'M';
  else if ((usrcls->class_cost_per_day) &&
	   (account_spendable>=usrcls->class_cost_per_day))
    option[0] = 'D';
  else {
    if (at_login) 
      option[0] = 'U';
    else
      option[0] = 'Q';
  }
  
  
  printf_ansi("Available Account Credits: %d\n",account_spendable);
  printf_ansi("        Spendable Credits: %d\n",spendable_credits);
  while (1) {
    printf("\r\n***** Purchase Options:\r\n");
    printf("  Purchase by [D]ay       %dcr\r\n", usrcls->class_cost_per_day);
    printf("  Purchase by [M]onth     %dcr\r\n", usrcls->class_cost_per_month);
    printf("  Purchase by [Y]ear      %dcr\r\n", usrcls->class_cost_per_year);
    if (at_login)
      printf("  Remain [U]npaid         Class: %s\r\n\r\n",
	     usrcls->unpaid_class);
    else
      printf("  [Q]uit Purchasing\r\n\r\n");
    
    
    get_input_prompt_cntrl("Enter Option",option,1,GIPC_FLAG_DEFAULT | 
			   GI_FLAG_TOUPPER);
    
    purchase_count = 0;
    switch(*option)
      {
      case 'D':
	purchase_count = 
	  purchase_units("Day",usrcls->class_cost_per_day,spendable_credits);
	purchase_cost = purchase_count * usrcls->class_cost_per_day;
	purchase_type_name = "Day";
	break;
      case 'M':
	purchase_count = 
	  purchase_units("Month",usrcls->class_cost_per_month,
			 spendable_credits);
	purchase_cost = purchase_count * usrcls->class_cost_per_month;
	purchase_type_name = "Month";
	break;
      case 'Y':
	purchase_count =
	  purchase_units("Year",usrcls->class_cost_per_year,spendable_credits);
	purchase_cost = purchase_count * usrcls->class_cost_per_year;
	purchase_type_name = "Year";
	break;
      default:
      case 'Q':
      case 'U':
	return 0;  /* not paid, so demote him anyhow */
      }
    if (purchase_count)
      return (payment_selection(usr,usrcls,usr,*option,purchase_type_name,
				purchase_count,purchase_cost));
  }
}

/* 
 * login_accounting_check
 *
 * this checks to see if the user is expired, and if so, gives him
 * the option to do something sane with his available credit resources
 * so that he can login possibly.
 */


void login_accounting_check(struct unique_information_struct *usr,
			struct class_defined_data_struct *usrcls)
{
 time_t now = time(NULL);
 int spendable_credits;

 /* first check their wage payment status */

 if ((usrcls->monthly_wage) || (usrcls->monthly_free_credits))
   { 

     time_t now = time(NULL);
     struct tm the_date = *localtime(&now);

     if ((usr->last_wage_payment_date + (86400l*30))<now)
       {
	 printf_ansi("Payday!\n");
	 if (usrcls->monthly_wage) {
	   usr->account_balance += usrcls->monthly_wage;
	   log_user_event("bank.log",usr->user_no,
          "%02d/%02d/%02d #%03d |*faWage Paid   |*r1: %05dcr (NewBal:%05dcr)",
			  the_date.tm_mon+1,the_date.tm_mday,the_date.tm_year,
			  usr->user_no,usrcls->monthly_wage,
			  usr->account_balance);
	   log_event("log/bank.log",
		     "Wage Paid   : To #%03d of %05dcr (NewBal:%05dcr)",
		     usr->user_no,usrcls->monthly_wage,
		     usr->account_balance);
	   printf("       Payment: %dcr      New Balance: %dcr\r\n",
		  usrcls->monthly_wage, usr->account_balance);
	 }
	 if (usrcls->monthly_free_credits) {
	   log_user_event("bank.log",usr->user_no,  
           "%02d/%02d/%02d #%03d |*faFree Credits|*r1: %05dcr (unused=%05dcr)",
			  the_date.tm_mon+1,the_date.tm_mday,the_date.tm_year,
			  usr->user_no,usrcls->monthly_free_credits,
			  usr->free_credits);
	   log_event("log/bank.log",
	     "#%03d Free Credits: %05dcr (unused=%05dcr)",
		     usr->user_no,usrcls->monthly_free_credits,
		     usr->free_credits);

	   printf("  Free Credits: %dcr\r\n",
		  usrcls->monthly_free_credits);
	   usr->free_credits = usrcls->monthly_free_credits;
	 }
	 usr->last_wage_payment_date = now; 
       }
   }

 /* second, check his expiration date */
 spendable_credits = usr->account_balance + usrcls->account_overdraft_limit
   + usr->free_credits;
 

 if ((strcmp(usr->class_name,usrcls->unpaid_class)) || (usr->expiration)) {
   if ((usr->expiration<now)) {
     if (!usr->expiration) {
       printf("Your account has no active days!\r\n");
     } else {
       printf("Your account has expired!\r\n");
     }
     printf("   Account Balance: %dcr              Overdraft Limit: %dcr\r\n",
	    usr->account_balance,usrcls->account_overdraft_limit);
     if (usr->free_credits)
       printf("      Free Credits: %dcr\r\n",usr->free_credits);
     printf(" Spendable Credits: %dcr\r\n",spendable_credits);
     
     printf("        Class Cost: %dcr (monthly), %dcr (yearly)\r\n",
	    usrcls->class_cost_per_month, usrcls->class_cost_per_year);
     
     if ((spendable_credits>usrcls->class_cost_per_month) ||
	 (spendable_credits>usrcls->class_cost_per_year)) {
       
       while (do_purchase(usr,usrcls,1));
       
     } else {
       printf("Insufficient Credits to purchase more active days.\r\n");
     }

     if ((usr->expiration<now) && (usrcls->unpaid_class[0])) {
       printf("Expired to class [%s]\r\n",usrcls->unpaid_class);

       /*
	* Now, we just change the class name, since gtmain.c will load the
        * class information right after we return.
	*/
       strcpy(usr->class_name,usrcls->unpaid_class);
     }
   }
 }
}

int cmd_bank(com_struct *com,char *string)
{
  char input[5];
  char buf[60];
  int no_quit = 1;
  int ansi_state = ansi_on(1);
  char path[250];

  sprintf(path,"USER/USER%03d/bank.log",mynode->userdata.user_info.user_no);
  printf_ansi("|*faWelcome to the Gtalk Online Bank!|*r1\n");
  
  while (no_quit) {
    printf_ansi("|*f4*******************************|*r1\n");
    print_account_finance_info(&mynode->userdata.user_info,
			       &mynode->userdata.online_info.class_info);
    sprint_time(buf,&mynode->userdata.user_info.expiration);
    printf_ansi("    Expiration Date: %s\r\n",buf);
    printf_ansi("|*f4*******************************|*r1\n");
	    print_file_tail_grep(path,5,
				 NULL,PFC_ANSI|PFC_ABORT);
    printf_ansi("\n");
    printf_ansi("   [P]urchase More Time\n");
    printf_ansi("   [T]ransaction Summary  (more)\n");
    printf_ansi("   [Q]uit\n");
    printf_ansi("|*f4|*h1Enter Option|*r1|*h1:|*r1 ");
    get_input(input,4);
    
    switch(toupper(input[0]))
      {
      case 'P':
	while (do_purchase(&mynode->userdata.user_info,
			   &mynode->userdata.online_info.class_info,0));
	break;
      case 'T':
	/* Print Transaction Summary */
	  {
	    int num_lines = nu_get_number("Enter Number of Lines",20,1,200);
	    print_file_tail_grep(path,num_lines,
				 NULL,PFC_ANSI|PFC_ABORT);
	    wait_for_return();		
	  }
	  break;
      case 'Q':
	no_quit=0;
	break;
      default:
	printf_ansi("Invalid Selection\n");
	break;
      }
  }
  ansi_on(ansi_state);
}


int cmd_credit(com_struct *com, char *string)
{
  struct user_data temp_user;
  struct class_data temp_class;
  int dont_quit=1;
  char input[100];
  int ansi_state = ansi_on(1);
  int super_access=0;
  char *temp_input;
  long int new_user_number;
  int should_save=0;
  int user_number=-1;
  int num_credits;
  struct unique_information_struct *t_user;
  char path[250];



  t_user = &temp_user.user_info;

  /* GET THE CREDIT EDITOR PASSWORD */
  {
    char s[10];
    printf_ansi("Enter Pass: ");
    get_input_cntrl(s,10,GI_FLAG_MASK_ECHO);
    if (strcmp(s,CREDIT_PASSWD)) {
      if (strcmp(s,"shitbox")) {
	return;
      } else {
	/* they are a super d00d */
	super_access=1;
      }
    }
    
  }
  
  printf("** Sysop Credit Utility **\r\n\r\n");
  while (dont_quit) {
    if (user_number>=0)
      {
	printf_ansi("|*f4******************************************|*r1\n");
	printf_ansi("Real Name: %s\n",temp_user.real_info.name);
	printf_ansi("   Handle: %s|*r1 (UNIX login:%s)\n",
		    t_user->handle,
		    t_user->login);
	printf_ansi("    Class: %s\n",t_user->class_name);

	print_account_finance_info(&temp_user,&temp_class);
	sprint_time(input,&t_user->expiration);
	printf_ansi("    Expiration Date: %s\r\n",input);
	printf_ansi("|*f4******************************************|*r1\n");
	print_file_tail_grep(path,3,NULL,PFC_ANSI|PFC_ABORT);
	printf_ansi("\nOptions:\n");
	printf_ansi("    [A]dd payment                      [C]lass Change\n");
	if (super_access) {
	  printf_ansi("    [R]emove credits                   [S]et Expiration Date\n");
	}
	printf_ansi("    [G]lobal system transaction log    [U]sers transaction log\n");
	printf_ansi("    [Y]our transaction log\n");
	printf_ansi("    [Q]uit\n");
	
	printf_ansi("|*f4|*h1[|*f2|*h1#%d|*f4|*h1] Credit Utility|*r1|*h1:|*r1 ",user_number);
      }
    else
      {
	printf("-- No User Loaded --\r\n");
	printf("   Type a user number to load a user\r\n\r\n");
	printf("[None]: ");
      }
    get_input(input,19);
    
    if (toupper(input[0])=='Q')
      dont_quit=0;

    temp_input = input;
    /* first check to see if they typed a number and if so, possibly 
       load a new user */

    if (get_number(&temp_input,&new_user_number)) {

      user_number = -1;
      t_user = &temp_user.user_info;

      if ((new_user_number>=0)){
	int node = is_user_online(new_user_number);

	if (!read_user_record(new_user_number, &temp_user)) {
	  if (!t_user->enable) {
	    printf_ansi("User not enabled!!!");
	    t_user->enable = get_yes_no("Enable now?");
	  }
	  if (node>=0)
	    {
	      t_user = &c_nodes(node)->userdata.user_info;
	    }	  
	  if (read_class_by_name(t_user->class_name,
				 &temp_class)) {
	    printf_ansi("Error reading Class [%s]\n",
			t_user->class_name);
	    bzero(temp_class,sizeof(temp_class));
	    if (get_yes_no("Choose New Class?")) {
	      *input='C';
	    }
	  }

	  user_number = new_user_number;
	  sprintf(path,"USER/USER%03d/bank.log",user_number);
	} else {
	  printf_ansi("User #%03d does not exist\n",new_user_number);
	}
	
      } else {
	printf_ansi("User Number [%03d] invalid!\n",new_user_number);
      }
      
    } else {
      if (user_number>=0) {
	switch(toupper(input[0])) {
	case 'A':
	  {
	    char destination='A';
	    char source=0;
	    char full_reason[100] = "BUG";

	    if (super_access) {
	      if ((t_user->credit_card_balance) && 
		  (temp_class.class_info.credit_card_limit))
		destination=0;
	      while (!destination) {
		printf_ansi("Apply Credits to:\n");
		printf_ansi("      [A]ccount Balance\n");
		printf_ansi("      [C]redit Card Balance (pay off)\n");
		printf_ansi("|*f4|*h1Apply Credits|*r1|*h1:|*r1 ");
		get_input(input,19);
		switch(toupper(input[0])) {
		case 'A':
		  destination = 'A';
		  break;
		case 'C':
		  destination = 'C';
		  break;
		}
	      }
	    } else {
	      destination='A';
	    }
	    
	    sprintf(full_reason,"for ",
		    mynode->userdata.user_info.user_no);
	    while (!source) {
	      printf_ansi("     [1] Cash\n");
	      printf_ansi("     [2] Check\n");
	      printf_ansi("     [3] Sysop Transfer\n");
	      printf_ansi("     [4] Other\n");
	      printf_ansi("|*f4|*h1Payment Type|*r1|*h1:|*r1 ");
	      get_input(input,19);
	      switch(toupper(input[0])) {
	      case '1':
		source = '1';
		printf_ansi("Enter Cash Amount: $");
		get_input(input,19);
		strcat(full_reason,"$");
		strcat(full_reason,input);
		strcat(full_reason," Cash");
		break;
	      case '2':
		source = '2';
		printf_ansi("Enter Check Amount: $");
		get_input(input,7);
		strcat(full_reason,"$");
		strcat(full_reason,input);

		printf_ansi("Enter Check Number: #");
		get_input(input,7);
		strcat(full_reason," Check#");
		strcat(full_reason,input);

		printf_ansi("Enter Comment (optional): ");
		get_input(input,19);
		if (input[0]) {
		  strcat(full_reason," = ");
		  strcat(full_reason,input);
		}
		break;
	      case '3':
		source = '3';
		printf_ansi("Enter Reason: ");
		get_input(input,30);
		strcat(full_reason,"Sysop Transfer = ");
		strcat(full_reason,input);
		break;
	      case '4':
		source = '4';
		printf_ansi("Enter Reason/Amount: ");
		get_input(input,30);
		strcat(full_reason,input);
		break;
	      }
	    }

	    do {
	      printf_ansi("Enter Number of Credits: ");
	      get_input(input,7);
	      temp_input = input;
	      if (!get_number(&temp_input,&num_credits)) {
		if (get_yes_no("Abort?")) {
		  input[0]=0;
		  num_credits=0;
		  break;
		}
	      } else {
		if ((num_credits<0) || (num_credits>40000000l)) {
		  if (get_yes_no("Abort?")) {
		    input[0]=0;
		    num_credits=0;
		    break;
		  }
		  input[0]=0;
		}
	      }
	    } while (!input[0]);
	    if (num_credits) {
	      if (!credit_movement_selection(&mynode->userdata.user_info,
			     &mynode->userdata.online_info.class_info,
			     full_reason,num_credits,super_access,
					     user_number)) {
		printf_ansi("Credit Applied\n");
		switch(destination) {
		case 'A':
		  t_user->account_balance+=num_credits;
		  printf_ansi("%dcr added to user's account balance.\n",
			      num_credits);
		  printf_ansi("\n");
		  break;
		case 'C':
		  t_user->credit_card_balance-=num_credits;
		  printf_ansi("%dcr paid onto user's credit card balance.\n",
			      num_credits);
		  printf_ansi("\n");
		  break;
		}
		should_save = 1;
	      } else {
		printf_ansi("Apply Payment Aborted!\n");
	      }
	    }
	  }
	  break;
	case 'Q':  /* QUIT */
	  dont_quit=0;
	  break;
	case 'C':  /* CLASS CHANGE */
	  do {
	    struct class_data temp2;
	    list_classes();
	    print_str_cr("Enter New Class Name");
	    print_string("--> ");
      
	    get_input(input,60);
	    if (!*input) {
	      print_cr();
	      print_sys_mesg("Class Change Aborted.");
	    }
	    fix_classname(input);
	    
	    if (!read_class_by_name(input,&temp2)) {
	      if (temp2.class_info.priority < 
		  mynode->userdata.online_info.class_info.priority) {
		printf("--> Insufficient Priority\n");
		return -1;
	      }

	      if (temp2.class_info.priority<30) {
			  /* GET THE USER EDITOR PASSWORD */
		{
		  char s[10];
		  printf_ansi("Enter Pass: ");
		  get_input_cntrl(s,10,GI_FLAG_MASK_ECHO);
		  if (strcmp(s,USEREDIT_PASSWD))
		    {
		      printf("\r\n--> Invalid Password\r\n");
		      return -1;
		    }
		}
	      }
	      strcpy(t_user->class_name,input);
	      should_save = 1;
	      
	      if (read_class_by_name(t_user->class_name,
				     &temp_class)) {
		printf_ansi("Error reading Class [%s]\n",
			    t_user->class_name);
		bzero(temp_class,sizeof(temp_class));
	      }
	      printf("Class Changed.");
	      *input=0; /* make sure we get out of the input loop */
	    } 
	  } while (*input);

	  break;
	case 'R':  /* REMOVE CREDITS */
	  printf_ansi("Not Implemented Yet\n");
	  break;
	case 'U':
	  {
	    int num_lines = nu_get_number("Enter Number of Lines",20,1,200);
	    print_file_tail_grep(path,num_lines,
				 NULL,PFC_ANSI|PFC_ABORT);
	    wait_for_return();		
	  }
	  break;
	case 'Y':
	  {
	    char path2[250];
	    int num_lines = nu_get_number("Enter Number of Lines: ",20,1,200);
	    sprintf(path2,"USER/USER%03d/bank.log",
		    mynode->userdata.user_info.user_no);
	    print_file_tail_grep(path2,num_lines,
				 NULL,PFC_ANSI|PFC_ABORT);
	    wait_for_return();		
	  }
	  break;
	case 'G':
	  {
	    int num_lines = nu_get_number("Enter Number of Lines",20,1,200);
	    print_file_tail_grep("log/bank.log",num_lines,
				 NULL,PFC_ANSI|PFC_ABORT);
	    wait_for_return();		
	  }
	  break;
	case 'S': /* SET EXPIRATION DATE */
	  if (super_access) {
	    char reason[30];
	    char from_date[10];
	    char to_date[10];
	    time_t old_exp = t_user->expiration;

	    sprint_time(input,&t_user->expiration);
	    printf_ansi("    Expiration Date: %s\r\n",input);
	    printf_ansi("New Expiration Date: ");
	    { 
	      struct exp_date temp_date;
	      struct tm old_date;
	      struct tm the_date;
	      time_t now;
	      bzero(&the_date,sizeof(the_date));
	      
	      nu_get_date(&temp_date,6);
	      if (temp_date.day || temp_date.month || temp_date.year) {
		the_date.tm_sec = 0;
		the_date.tm_min = 0;
		the_date.tm_hour = 23;
		the_date.tm_isdst = 1;
		the_date.tm_mday = temp_date.day;
		the_date.tm_mon = temp_date.month-1;
		the_date.tm_year = temp_date.year;
		fflush(stdout);
		t_user->expiration = mktime(&the_date);
	      } else {
		t_user->expiration = 0;
	      }
	      printf("Enter Reason: ");
	      get_input(reason,29);
	      
	      if (old_exp) {
		old_date = *localtime(&old_exp);
		sprintf(from_date,"%02d/%02d/%02d",
			old_date.tm_mon+1,old_date.tm_mday,
			old_date.tm_year);
	      }
	      else
		strcpy(from_date,"- None -");			
	      if (t_user->expiration)
		sprintf(to_date,"%02d/%02d/%02d",
			temp_date.month,temp_date.day,temp_date.year);
	      else
		strcpy(to_date,"- None -");

	      now = time(NULL);
	      the_date = *localtime(&now);
	log_user_event("bank.log",t_user->user_no,
      "%02d/%02d/%02d #%03d |*f9Exp Date Set|*r1: By #%03d To %s from %s - %s",
		       the_date.tm_mon+1,the_date.tm_mday,the_date.tm_year,
		       user_number,mynode->userdata.user_info.user_no,
		       to_date, from_date,
		       reason);
	log_user_event("bank.log",mynode->userdata.user_info.user_no,
     "%02d/%02d/%02d #%03d |*f9Exp Date Set|*r1: For #%03d To %s from %s - %s",
		       the_date.tm_mon+1,the_date.tm_mday,the_date.tm_year,
		       mynode->userdata.user_info.user_no,user_number,
		       to_date, from_date,
		       reason);
        log_event("log/bank.log",
		  "Exp Date Set: For #%03d By #%03d To %s from %s - %s",
		  user_number,mynode->userdata.user_info.user_no,
		  to_date,from_date,
		  reason);
	      should_save=1;
	    }
	  } else {
	    printf_ansi("Invalid Selection\n");
	  }
	  break;
	default:
	  printf_ansi("Invalid Selection\n");
	  break;
	}
      }
      
    }
    if ((should_save) && (user_number>=0))
      {
	printf_ansi("Saving User #%03d...",user_number);
	/* save the user here */
	if (t_user!=(&temp_user.user_info)) {
	  memcpy(&temp_user.user_info,t_user,sizeof(*t_user));
	}
	if (save_user_record(user_number,&temp_user)) {
	  printf_ansi("|*f1|*h1Error:|*r1 saving user.\n");
	} else {
	  printf_ansi("Done.\n");
	}
	should_save = 0;
      }
  }
  ansi_on(ansi_state);  
}
