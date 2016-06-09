
/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

/* USEREDIT.C */
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../userst.h"
#include "useredit.h"

#include "function.h"
#include "automenu.h"
#include "ansi.h"

int tswitch=0;

struct old_style_user_data
{
    struct unique_information_struct user_info;
    struct user_class_information_struct class_mod_info;
    struct rl_info real_info;
    struct class_defined_data_struct class_info;
};



FILE *g_fopen(char *path, char *mode,char *string)
{
   return (fopen(path,mode));
}


void list_classes(void);

int ce_menu_class_edit(struct structures_used_struct *a_struct,
                     struct dirty_rect_struct *dirty,
                     struct a_menu_struct *old_menu);
void edit_main_privs(unsigned char *privs,char *filename);
void ce_class_edit_start_new(struct class_data *classdata);
void ce_priv_time_edit(struct class_data *edituser);
char um_main_prompt[]="User Maintenance (? for Help): ";
char edit_user_prompt[]="Editing User (1,2,3,4,5,6,Q): ";
void ue_delete_users(int number);
void ue_main_prompt(void);
time_t enter_date(int time_too);
void set_new_expiration_date(struct user_data *edit_data);
void ce_class_edit_start(struct class_data *edituser);
void ue_user_edit_start(struct user_data *edituser);
void edit_staples(struct class_data *editclass);

#define NUMPRVS 30
#define MAX_PRIV_NUM 80
#define MAX_MAIN_PRIV_NUM 12

struct privledge_entry
{
    int priv;
    char priv_title[50];
};

struct priv_main_entry
{
    char menu_key;
    char priv_filename[50];
    char item_title[40];
};


/* Get a string from the file FILEPTR */

void file_get_string(char *string, unsigned int num_ch, FILE *fileptr)
{
    char ch;

    while ((!feof(fileptr)) && (num_ch))
     {
       ch = fgetc(fileptr);
       if ((ch==10) || (ch=='*')) num_ch = 0;
        else
       if (ch!=13)
        {
          *string++ = ch;
          num_ch--;
        };
     };
   *string = 0;
};

/*************************************************************
 *       NEW STUFF                                           *
 *************************************************************/


struct string_field_info_struct real_name_field_info =
 { USER_REAL_NAME_LEN, 20, 1 ,0};

struct string_field_info_struct handle_field_info =
 { HANDLE_LEN, 20 , 1 , 0 };

extern struct string_field_info_struct class_field_info;
extern struct string_field_info_struct class_field_info_no_edit;

struct string_field_info_struct password_field_info =
 { PASSWORD_LEN, 20, 1, '*'};

struct string_field_info_struct address_field_info =
 { USER_STREET_LEN, 20, 1, 0 };

struct string_field_info_struct banner_field_info =
 { 0, 0, 0, 0 };

int quit_button_target(struct structures_used_struct *a_struct,
                     struct dirty_rect_struct *dirty,
                     struct a_menu_struct *old_menu)
{
    return (MENU_QUIT);
}


int ceue_not_implemented(struct structures_used_struct *a_struct,
                     struct dirty_rect_struct *dirty,
                     struct a_menu_struct *old_menu)
{

    struct field_info_struct fields[] = {
        { 37, 15, "<OK>", 0, NULL, button_field, NONE, NONE, NONE, NONE, NONE, NONE, 0, quit_button_target ,NULL},
        { 15,13,"|*f1|*h1I'm sorry but this feature is not implemented yet.",
                        0, NULL, string_field, NONE, NONE, NONE, NONE, NONE, NONE,  0, &banner_field_info ,NULL},
        { 0,0, 0    , 0, 0, 0           ,    0,    0,    0,    0,   0,   0 , 0}
    };

    struct a_menu_struct the_menu;
    struct menu_help_info_struct temp_help_info= {19,8,"|*b1|*h1",10};

    the_menu.fields = fields;
    the_menu.sts = NULL;
    the_menu.menu_info = NULL;
    the_menu.help_info = &temp_help_info;
    the_menu.old_menu = NULL;

    erase_region(9,23,tswitch);
    do_menu_ansi(&the_menu,NULL);
    erase_region(9,23,tswitch);

    return 0;
}

int real_info_edit2(struct structures_used_struct *a_struct,
                     struct dirty_rect_struct *dirty,
                     struct a_menu_struct *old_menu)
{
    struct user_data *userdata = a_struct->structs[0];

    struct field_info_struct fields[] = {
        { 2,10, "[R]eal Name",      0,&(((struct user_data *)0)->real_info.name),               string_field, 1, 7, NONE, 1, NONE, 4, 'R', &real_name_field_info,"|*b1|*h1Real Name" },
        { 2,11, "[A]ddress  ",      0,&(((struct user_data *)0)->real_info.street),             string_field, 2, 0, 0, 2, NONE, 5, 'A', &address_field_info ,"|*b1|*h1Street Address"},
        { 2,12, "[C]ity     ",      0,&(((struct user_data *)0)->real_info.city),               string_field, 3, 1, 1, 3, NONE, 6, 'C', &address_field_info , "|*b1|*h1City"},
        { 2,13, "[S]tate    ",      0,&(((struct user_data *)0)->real_info.state_or_province),  string_field, 4, 2, 2, NONE, NONE, 7, 'S', &address_field_info ,"|*b1|*h1State or Province"},
        { 40,10,"[P]ostal Code    ",0,&(((struct user_data *)0)->real_info.postal_code),        string_field, 5, 3, NONE, 5, 0, NONE, 'P', &address_field_info ,"|*b1|*h1Postal Code"},
        { 40,11,"[B]irthdate      ",0,&(((struct user_data *)0)->real_info.birth_date),         raw_date_field, 6, 4, 4, 6, 1, NONE, 'B', NULL ,"|*b1|*h1Birthdate"},
        { 40,12,"[V]oice Phone    ",0,&(((struct user_data *)0)->real_info.phone),              phone_field,  7, 5, 5, 7, 2, NONE, 'V', &address_field_info ,"|*b1|*h1Voice Phone"},
        { 40,13,"[D]ata/Fax Phone ",0,&(((struct user_data *)0)->real_info.phone2),             phone_field,  0, 6, 6, NONE, 3, NONE, 'D', &address_field_info ,"|*b1|*h1Data/FAX Phone"},
        { 0,0, 0    , 0, 0, 0           ,    0,    0,    0,    0,   0,   0 ,0}
    };

    void *struct_list[2];
    struct user_data temp_user_info = *userdata;

    struct structures_used_struct structs_used;
    struct a_menu_struct the_menu;
    struct menu_help_info_struct temp_help_info= {19,8,"|*b1|*h1",10};


    struct_list[0] = &temp_user_info;
    struct_list[1] = NULL;
    structs_used.count=1;
    structs_used.structs = struct_list;

    the_menu.fields = fields;
    the_menu.sts = &structs_used;
    the_menu.menu_info = NULL;
    the_menu.help_info = &temp_help_info;
    the_menu.old_menu = old_menu;

    erase_region(9,23,tswitch);

    if (!do_menu_ansi(&the_menu,NULL))
     {
       *userdata = temp_user_info;
     }

   return 0;
}


int expiration_edit(struct structures_used_struct *a_struct,
                     struct dirty_rect_struct *dirty,
                     struct a_menu_struct *old_menu)
{
    struct user_data *userdata = a_struct->structs[0];

    struct field_info_struct fields[] = {
        { 2,10, "[E]xpiration   ",      0,&(((struct user_data *)0)->user_info.expiration),   date_field,   1, 3, NONE,    1, NONE,    2, 'E', NULL , "|*b1|*h1Expiration Date"},
        { 2,11, "<No Expiration>",      0,&(((struct user_data *)0)->user_info.expiration),   clear_date_field, 2, 0,    0, NONE, NONE,    3,  0,  NULL , "|*b1|*h1Clear Expiration Date"},
        { 40,10,"[C]redit    ",         0,&(((struct user_data *)0)->user_info.credit),       date_field,   3, 1, NONE,    3,    0, NONE, 'C', NULL , "|*b1|*h1Days Credit"},
        { 40,11,"<No Credit> ",         0,&(((struct user_data *)0)->user_info.credit),       clear_date_field, 0, 2,    2, NONE,    1, NONE,  0,  NULL , "|*b1|*h1Clear Credit"},
        { 0,0, 0    , 0, 0, 0           ,    0,    0,    0,    0,   0,   0 ,0}
    };

    void *struct_list[2];
    struct user_data temp_user_info = *userdata;

    struct structures_used_struct structs_used;
    struct a_menu_struct the_menu;
    struct menu_help_info_struct temp_help_info= {19,8,"|*b1|*h1",10};

    struct_list[0] = &temp_user_info;
    struct_list[1] = NULL;
    structs_used.count=1;
    structs_used.structs = struct_list;

    the_menu.fields = fields;
    the_menu.sts = &structs_used;
    the_menu.menu_info = NULL;
    the_menu.help_info = &temp_help_info;
    the_menu.old_menu = old_menu;

    erase_region(9,23,tswitch);

    if (!do_menu_ansi(&the_menu,NULL))
     {
       *userdata = temp_user_info;
     }

   return 0;
}


int ue_set_new_expiration_date_start(struct structures_used_struct *a_struct,
                     struct dirty_rect_struct *dirty,
                     struct a_menu_struct *old_menu)
{
    struct user_data *userdata = a_struct->structs[0];

    erase_region(9,24,tswitch);

    set_scrolling_region(9,24,tswitch);
    position(9,0);

    set_new_expiration_date(userdata);

    erase_region(9,24,tswitch);
    paint_info_row();
    return 0;
}

int ce_edit_staples_start(struct structures_used_struct *a_struct,
                     struct dirty_rect_struct *dirty,
                     struct a_menu_struct *old_menu)
{
    struct class_data *classdata = a_struct->structs[0];

    erase_region(9,24,tswitch);

    set_scrolling_region(9,24,tswitch);
    position(9,0);

    edit_staples(classdata);

    erase_region(9,24,tswitch);
    paint_info_row();
    return 0;
}


/*        { 2,4, "[C]redit     ",        0, &(((struct user_data *)0)->user_info.credit),           date_field,   4, 1,    1,    3, NONE, NONE, 'C', NULL                 ,"|*b1|*h1Credit"}, */

void ue_user_edit_start(struct user_data *userdata)
{
    struct field_info_struct fields[] = {
        { 2,2, "Han[d]le     ",        0, &(((struct user_data *)0)->user_info.handle),           string_field, 1, 4, NONE,    1, NONE,    4, 'D', &handle_field_info   ,"|*b1|*h1Handle"},
        { 2,3, "[E]xpiration ",        0, &(((struct user_data *)0)->user_info.expiration),       date_field,   2, 0,    0,    2, NONE,    5, 'E', NULL                 ,"|*b1|*h1Expiration Date"},
        { 2,4, "[C]redit     ",        0, &(((struct user_data *)0)->user_info.credit),           button_field, 4, 1,    1,    3, NONE, NONE, 'C', ue_set_new_expiration_date_start,"|*b1|*h1Credit"},
        { 2,6, "<[R]eal Info>",        0, NULL,                                                   button_field, 6, 5,    2,    9, NONE,    6, 'R', real_info_edit2      ,"|*b1|*h1--> Real Info Edit"},
/*        { 40,2,"[P]assword       ",    0, &(((struct user_data *)0)->user_info.password),         string_field, 5, 2, NONE,    5,    0, NONE, 'P', &password_field_info ,"|*b1|*h1Password"}, */
        { 40,3,"Cla[s]s          ",    0, &(((struct user_data *)0)->user_info.class_name),  string_field, 3, 4,    4,    7,    1, NONE, 'S', &class_field_info    ,"|*b1|*h1Change User's Class"},
        { 22,6,"<Expiration>",         0, NULL,                                                   button_field, 7, 3,    2, NONE,    3,    7,  0 , expiration_edit      ,"|*b1|*h1--> Expiration Edit"},
        { 42,6,"<User Logs>",          0, NULL,                                                   button_field, 8, 6,    5, NONE,    6,    8,  0 , ceue_not_implemented ,"|*b1|*h1--> Log Utility"},
        { 62,6,"<Edit Class>",         0, NULL,                                                   button_field, 9, 7,    5, NONE,    7, NONE,  0 , ce_menu_class_edit   ,"|*b1|*h1--> Class Edit"},
        { 2, 7,"<Class Modification>", 0, NULL,                                                   button_field, 0, 8,    3, NONE, NONE, NONE,  0 , ceue_not_implemented ,"|*b1|*h1--> Class Mod Edit"},
        { 1,8,"|*b1|*h1 Main           |                                           | GT-UE v1.00  ",
                                   0, NULL,                                              string_field, NONE, NONE, NONE, NONE, NONE, NONE, 0, &banner_field_info ,NULL},
        { 0,0, 0    , 0, 0, 0           ,    0,    0,    0,    0,   0,  0,0  }
    };

    void *struct_list[2];
    struct user_data temp_user_info = *userdata;

    struct structures_used_struct structs_used;
    struct a_menu_struct the_menu;
    struct menu_help_info_struct temp_help_info= {19,8,"|*b1|*h1",10};


    struct_list[0] = &temp_user_info;
    struct_list[1] = NULL;
    structs_used.count=1;
    structs_used.structs = struct_list;

    the_menu.fields = fields;
    the_menu.sts = &structs_used;
    the_menu.menu_info = NULL;
    the_menu.help_info = &temp_help_info;
    the_menu.old_menu = NULL;

    clear_screen();
    if (!do_menu_ansi(&the_menu,NULL))
     {
       *userdata = temp_user_info;
     }
}



/*********************************************************************
 *             NEW STUFF    -   CLASS EDIT                           *
 *********************************************************************/

struct string_field_info_struct class_field_info =
 { CLASS_NAME_LEN, 20, 1 ,0};
struct string_field_info_struct class_field_info_no_edit =
 { CLASS_NAME_LEN, 20, 0 ,0};


int ce_menu_class_edit(struct structures_used_struct *a_struct,
                     struct dirty_rect_struct *dirty,
                     struct a_menu_struct *old_menu)
{
    struct user_data *userdata = a_struct->structs[0];
    int load_result,test,flag;
    char s[CLASS_NAME_LEN+1];
    struct class_data temp_class;


    strcpy(s,userdata->user_info.class_name);

/*
	  if (!get_password("Master",&sys_info.master_password,1))
	   { print_str_cr("Sorry.");
		 return;
	   }
*/

      load_result = read_class_by_name(s,&temp_class);

	  if (load_result)
		{
		 print_str_cr("ERROR LOADING CLASS");
         return 1;
        }

   do {

            ce_class_edit_start_new(&temp_class);
            dirty->dirty_flag=1;
            dirty->all_dirty=1;

            erase_region(9,23,tswitch);

            set_scrolling_region(9,23,tswitch);
            position(9,0);


			test = 1;
			print_cr();
			print_str_cr("[S]ave and Quit");
			print_str_cr("[C]ancel");
			print_str_cr("[A]bort Save and Quit");
			while (test)
			  { print_cr();
				print_string("Option (S,C,A): ");

				do {
				   get_string(s,1);
				} while (!*s);

				switch (toupper(*s))
				{
				   case 'A':
							print_sys_mesg("Aborted");
							test=0; flag=0;
							break;
				   case 'C':
							test=0;
							break;
				   case 'S':

                            /* OKAY, NOW WE HAVE TO SAVE THE USER AND EXIT */
                            if (temp_class.class_info.class_index>=0)
							{

								print_str_cr("Saving Class by Number");
                              if (!save_class(temp_class.class_info.class_index,&temp_class))
								   print_sys_mesg("CLASS Saved to Disk" );
							  else
									log_error("* tried to save in class editor and failed");

							 }
							 else
							  {
								print_str_cr("Saving Class by Name");
                                 if (!save_class_by_name(temp_class.class_info.class_name,&temp_class))
									print_sys_mesg("CLASS saved to disk");
								 else
									{ print_sys_mesg("CLASS save error");
									  log_error("* could not save class by name in class editor");
								   }
							   }
							 test=0; flag=0;
							 break;
				 default: test=1;
						  flag=1;
						  break;
			 } /* end switch */
		} /* end while */

   } while (flag);

   set_scrolling_region(0,24,tswitch);
   print_cr();
   clear_screen();


}

int ce_class_priv_edit_start(struct structures_used_struct *a_struct,
                     struct dirty_rect_struct *dirty,
                     struct a_menu_struct *old_menu)
{
    struct class_data *classdata = a_struct->structs[0];

    erase_region(9,24,tswitch);

    set_scrolling_region(9,24,tswitch);
    position(9,0);

    edit_main_privs(classdata->class_info.privs,"sysop/main.prc");

    erase_region(9,24,tswitch);
    paint_info_row();

    return 0;
}


int ce_class_time_edit_start(struct structures_used_struct *a_struct,
                     struct dirty_rect_struct *dirty,
                     struct a_menu_struct *old_menu)
{
    struct class_data *classdata = a_struct->structs[0];
    char s[100];


    erase_region(9,24,tswitch);

    set_scrolling_region(9,24,tswitch);
    position(9,0);

    print_cr();
    print_string("Editing Priorities/Time for Class [");
    sprintf(s,"%s]",classdata->class_info.class_name);
    print_str_cr(s);
    print_cr();
    ce_priv_time_edit(classdata);

    erase_region(9,24,tswitch);
    paint_info_row();
    return 0;
}

int ce_class_misc_edit_start(struct structures_used_struct *a_struct,
                     struct dirty_rect_struct *dirty,
                     struct a_menu_struct *old_menu)
{
    struct class_data *classdata = a_struct->structs[0];

    erase_region(9,24,tswitch);

    set_scrolling_region(9,24,tswitch);
    position(9,0);

    edit_main_privs(classdata->class_info.privs,"sysop/main.prc");

    erase_region(9,24,tswitch);
    paint_info_row();
    return 0;
}


void ce_class_edit_start_new(struct class_data *classdata)
{
    struct field_info_struct fields[] = {
        { 2,2, "Class [N]ame   ",        0, &(((struct class_data *)0)->class_info.class_name),  string_field, 1, 5, NONE,    1, NONE,    4, 'N', &class_field_info_no_edit    ,"|*b4|*h1Class Name"},
        { 2,3, "[P]riority     ",        0, &(((struct class_data *)0)->class_info.priority),    button_field, 2, 0,    0,    2, NONE,    5, 'P', ceue_not_implemented         ,"|*b4|*h1Priority (0-255)"},
        { 2,6, "<Time/Priorities>",      0, NULL,                                                button_field, 3, 1,    1, NONE, NONE,    3,  0 , ce_class_time_edit_start     ,"|*b4|*h1--> Time/Priorities Edit"},
        { 22,6,"<Privs>",                0, NULL,                                                button_field, 4, 2,    1, NONE,    2,    4,  0 , ce_class_priv_edit_start     ,"|*b4|*h1--> Priv Edit"},
        { 42,6,"<Misc>",                 0, NULL,                                                button_field, 5, 3,    1, NONE,    3,    5,  0 , ce_edit_staples_start     ,"|*b4|*h1--> Misc Edit"},
        { 62,6,"<Logs>",                 0, NULL,                                                button_field, 0, 4,    1, NONE,    4, NONE,  0 , ceue_not_implemented         ,"|*b4|*h1--> Log Utility"},
        { 1,8,"|*b4|*h1 Main           |                                           | GT-CE v1.00  ",
                                         0, (char *)0,                                      string_field, NONE, NONE, NONE, NONE, NONE, NONE, 0, &banner_field_info ,NULL},
        { 0,0, 0    , 0, 0, 0           ,    0,    0,    0,    0,   0,   0 ,0}
    };

    void *struct_list[2];
    struct class_data temp_class_info = *classdata;

    struct structures_used_struct structs_used;
    struct a_menu_struct the_menu;
    struct menu_help_info_struct temp_help_info= {19,8,"|*b4|*h1",10};


    struct_list[0] = &temp_class_info;
    struct_list[1] = NULL;
    structs_used.count=1;
    structs_used.structs = struct_list;

    the_menu.fields = fields;
    the_menu.sts = &structs_used;
    the_menu.menu_info = NULL;
    the_menu.help_info = &temp_help_info;
    the_menu.old_menu = NULL;

    clear_screen();
    if (!do_menu_ansi(&the_menu,NULL))
     {
       *classdata = temp_class_info;
     }

}



/********************************************************************
 *             OLD STUFF                                            *
 ********************************************************************/


void edit_staple_menu(struct class_data *editclass)
{

    char s[80];
    print_str_cr("Editing Staples");
    print_cr();
    sprintf(s,"#00%cT1:%s %c This is a sample statement.",editclass->class_info.staple[0],"User Handle",editclass->class_info.staple[1]);
    print_str_cr(s);
    sprintf(s,"--> %c %s %c has arrived.",editclass->class_info.staple[2],"User Handle",editclass->class_info.staple[3]);
    print_str_cr(s);
    print_cr();
    print_file("menu/staple.mnu");
}

void get_new_staple(int staple,struct class_data *editclass)
{ char s[3];
  char out[40];
        print_cr();
        sprintf(out,"Current Staple : %c",editclass->class_info.staple[staple]);
        print_str_cr(out);
        print_string("Enter New Staple: ");
        do
        { get_string(s,1); }
        while (!*s);
        editclass->class_info.staple[staple]=s[0];
}

void edit_staples(struct class_data *editclass)
{   int flag=1;
    char s[3];
    edit_staple_menu(editclass);
    while(flag)
     {
      print_string("Enter Selection: ");

      do
      { get_string(s,1); }
      while (!*s);

      if (*s>'Z') *s-=32;
      if (*s=='?')
        edit_staple_menu(editclass);
      else
      if (*s=='Q')
       flag=0;
      else
      if (*s=='1')
       get_new_staple(0,editclass);
      else
      if (*s=='2')
       get_new_staple(1,editclass);
      else
      if (*s=='3')
       get_new_staple(2,editclass);
      else
      if (*s=='4')
       get_new_staple(3,editclass);

     }
}



void print_main_privs_menu(struct priv_main_entry priv_entry[],int number,char *header)
{
  char s[120];
  int loop;

      print_cr();
      print_str_cr(header);
      print_cr();

        for (loop=0;loop<number;loop++)
          {
            sprintf(s,"[%c] %s",priv_entry[loop].menu_key,
                           priv_entry[loop].item_title);
            print_str_cr(s);

          }
       print_cr();

}

/* EDIT_PRIVS_MAIN - privledge editor */

void edit_main_privs(unsigned char *privs,char *filename)
{
  char next_filename[80];
  int exit=0;
  while (!exit)
   {
      { int loop,flag;
        char s[120];
        char header[80];
        char strnumber[20];
        struct priv_main_entry priv_entry[MAX_MAIN_PRIV_NUM];


        int number;
        FILE *fileptr;

        if ((fileptr=g_fopen(filename,"r","USERED #1"))==NULL)
           {
            log_error(filename);
            print_sys_mesg("Could not open privs file");
            return;
           }
        file_get_string(header,79,fileptr);
        file_get_string(strnumber,20,fileptr);
        strnumber[3]=0;
        number=atoi(strnumber);
        if (number>MAX_MAIN_PRIV_NUM)
          {
            sprintf(s,"* Too many Privs in file %s",filename);
            log_error(s);
            print_str_cr(s);
            g_fclose(fileptr);
            return;
          }

        for (loop=0;loop<number;loop++)
          {
            fscanf(fileptr,"%c*",&priv_entry[loop].menu_key);
             priv_entry[loop].menu_key=toupper(priv_entry[loop].menu_key);
            file_get_string(priv_entry[loop].item_title,39,fileptr);
            file_get_string(priv_entry[loop].priv_filename,39,fileptr);
            if (feof(fileptr))
              { sprintf(strnumber,"* Incorrect format in file %s",filename);
                log_error(strnumber);
                print_str_cr(strnumber);
                g_fclose(fileptr);
                return;
              }
          }

	g_fclose(fileptr);
	
        /* file is read */
	
	print_main_privs_menu(priv_entry,number,header);
	
        flag=1;
        while(flag)
         {

           print_string("Main Priv Edit Sub Menu (?,Q=quit): ");
           *s=0;
           while (!*s)
              get_string(s,2);
           if (*s>'Z') *s-=32;
           if (*s=='?')
             print_main_privs_menu(priv_entry,number,header);
           else
           if (*s=='Q')
             { flag=0; exit=1; }
           else
           {
             loop=0;
             while (loop<number && *s!=priv_entry[loop].menu_key)
                loop++;
             if (loop!=number)
              { strcpy(next_filename,priv_entry[loop].priv_filename);
                flag=0;
              }

           }

         }
     }

 if (!exit) edit_privs(privs,next_filename);

 } /* total exit */
}





/* EDIT_PRIVS - privledge editor */

void edit_privs(unsigned char *privs,char *filename)
{
    int loop,flag,loop2;
    char s[120];
    char header[80];
    char strnumber[20];
    struct privledge_entry priv_entry[NUMPRVS];

    int number;
    FILE *fileptr;

    if ((fileptr=g_fopen(filename,"r","USERED #1"))==NULL)
       {
        log_error(filename);
        print_sys_mesg("Could not open privs file");
        return;
       }
    file_get_string(header,79,fileptr);
    print_str_cr(header);
    file_get_string(strnumber,20,fileptr);
    strnumber[3]=0;
    number=atoi(strnumber);
    if (number>NUMPRVS)
      {
        sprintf(s,"* Too many Privs in file %s",filename);
        log_error(s);
        print_str_cr(s);
        g_fclose(fileptr);
        return;
      }


    for (loop=0;loop<number;loop++)
      {
        fscanf(fileptr,"%d*",&priv_entry[loop].priv);
        file_get_string(priv_entry[loop].priv_title,39,fileptr);
        if (feof(fileptr))
          { sprintf(strnumber,"* Incorrect format in file %s",filename);
            log_error(strnumber);
            print_str_cr(strnumber);
            g_fclose(fileptr);
            return;
          }
      }
   g_fclose(fileptr);

    /* file is read */


        print_cr();
        print_str_cr(header);

        for (loop=0;loop<number;loop++)
          { int num1;
            sprintf(s,"[%02d] %s",loop,priv_entry[loop].priv_title);
            print_string(s);
            num1=30-strlen(s);
            for (loop2=0;loop2<num1;loop2++)
              print_chr(' ');

            if (!(0==test_bit(privs,priv_entry[loop].priv)))
               print_str_cr("Enabled");
            else print_str_cr("Disabled");
          }
       print_cr();

    flag=1;
    while(flag)
     {  int num;

       print_string("Priv Edit Sub Menu (?,Q=quit): ");
       *s=0;
       while (!*s)
          get_string(s,2);
       if (*s>'Z') *s-=32;
       if (*s=='?')
         {     /* PRINT MENU */
           print_cr();
           print_str_cr(header);

           for (loop=0;loop<number;loop++)
             { int num1;

               sprintf(s,"[%02d] %s",loop,priv_entry[loop].priv_title);
               print_string(s);
               num1=30-strlen(s);

               for (loop2=0;loop2<num1;loop2++)
                 print_chr(' ');
               if (!(0==test_bit(privs,priv_entry[loop].priv)))
                  print_str_cr("Enabled");
               else print_str_cr("Disabled");
             }
       print_cr();
        }
       else
       if (*s=='Q')
         flag=0;
       else
       { num=atoi(s);
         if (num<number)
           { int num1;
            set_bit(privs,priv_entry[num].priv,(0==test_bit(privs,priv_entry[num].priv)));
            sprintf(s,"--[%s]--",priv_entry[num].priv_title);
            print_string(s);
            num1=30-strlen(s);
            for (loop2=0;loop2<num1;loop2++)
              print_chr(' ');
            if (!(0==test_bit(privs,priv_entry[num].priv)))
                print_str_cr("Enabled");
            else print_str_cr("Disabled");
           }
       }

     }

}


/* EXIST, will tell us if a user number exists */


int exist(int number)
{
    struct user_data user;

    if (load_user(number,&user))
      return 0;

    if (number<0) return 0; /* DEBUG */

    if (user.user_info.user_no<0) return 0;
    if (user.user_info.enable==0) return 0;

   return 1;
}


void print_user_time_pri(struct class_data *editclass)
{   char s[80];

    print_cr();

    sprintf(s,"[P]riority            : %d",editclass->class_info.priority);
    print_str_cr(s);
    sprintf(s,"[T]ime                : %d",editclass->class_info.time);
    print_str_cr(s);
    sprintf(s,"[A]dded Time          : %d",editclass->class_info.added_time);
    print_str_cr(s);
/*
    sprintf(s,"[L]ogin Channel       : %d",editclass->class_info.login_channel);
    print_str_cr(s);
*/
	print_cr();



}
void ce_priv_time_edit(struct class_data *editclass)
 {
    char s[80];
    int flag=1,num;
    char *point;


     print_user_time_pri(editclass);

    *s=0;
    while(flag)
      {
        print_string("Priority/Time Edit (P,T,A,Q,?): ");
        *s=0;
        while (!*s)
          get_string(s,1);
        if (*s>'Z') *s-=32;
        if (*s=='Q')
            flag=0;
        else
        if (*s=='?')
          print_user_time_pri(editclass);
        if (*s=='P')
          {
            sprintf(s,"Old User Priority: %d",editclass->class_info.priority);
            print_str_cr(s);
            print_string("Enter New Priority: ");
            get_string(s,3);
            if (*s)
              {
                if ((num=str_to_num(s,&point))!=-1)
                    editclass->class_info.priority=num;
              }
            sprintf(s,"New Priority: %d",editclass->class_info.priority);
            print_str_cr(s);
         }
         else
         if (*s=='T')
          {
            sprintf(s,"Old User Time Online: %d Minutes",editclass->class_info.time);
            print_str_cr(s);
            print_string("Enter New Time (Min): ");

            get_string(s,3);
            if (*s)
               if ((num=str_to_num(s,&point))!=-1)
                   editclass->class_info.time=num;
           sprintf(s,"New Time Online: %d Minutes ", editclass->class_info.time);
           print_str_cr(s);
          }
        else
        if (*s=='A')
         {
            sprintf(s,"Old User Added Time: %d Minutes",editclass->class_info.added_time);
            print_str_cr(s);
            print_string("Enter New Added Time (Min): ");
            get_string(s,3);
            if (*s)
             {if ((num=str_to_num(s,&point))!=-1)
                   editclass->class_info.added_time=num;
               sprintf(s,"New Added Time: %d Minutes",editclass->class_info.added_time);
              }
            else
              sprintf(s,"Added Time unchanged");
            print_str_cr(s);
        }
      }
}

/* IS USER ONLINE - returns whether the user #<usernumber> is online */

int is_user_online(int usernumber)
{
  int loop;

  if (usernumber<0)
    return -1;

  /*
      for (loop=0;loop<MAXPORTS;loop++)
      {
         if (line_status[loop].online)
           if (user_lines[loop].user_info.number == usernumber)
            return loop;
       }
   */
  return -1;

}

void ue_delete_users(int number)
{
  char input[50];
  int user_number;
  char *dummy;
  int do_delete;
  int flag=1;
  struct user_data temp_user;

  print_cr();

  while(flag)
      {
        print_string("-->");
        do
        {
           get_string(input,6);
        }
        while (!*input);

        if (*input=='q' || *input=='Q')
            flag=0;                               /* he quit the menu */

        else
        {
           user_number=str_to_num(input,&dummy);
           if (user_number<1 || user_number>999)
              print_sys_mesg("Number Out of Range");
           else
            {

              if (load_user(user_number,&temp_user))
                 {print_sys_mesg("Error opening user file");
                  log_error("*error loading user to DELETE*");
                  return;
                 }
              do_delete=1;
              if (temp_user.user_info.user_no<=0)
                { do_delete=0;
                  print_str_cr("That User Does Not Exist");
                }
              else
              {   special_code(1,tswitch);
                  print_string("Handle: ");
                  print_str_cr(temp_user.user_info.handle);
                  print_string("Real Name: ");
                  print_str_cr(temp_user.real_info.name);
                  special_code(0,tswitch);
                  if (get_yes_no("Really Delete This User?"))
                  {
		    temp_user.user_info.user_no=-1;
		    temp_user.user_info.enable=0;
		    if (save_user(user_number,&temp_user))
		      { print_sys_mesg("Error Saving Deleted User");
			log_error("*ERROR saving user to DELETE*");
			return;
		      }
		    print_str_cr("          -> Deleted <-");
		    print_string("Killing his rotator messages, please wait");
		    kill_all_messages_for_user(user_number,'.');
		    print_str_cr("<done>");
		    
		  }
		}
	    }
	 }
      }
  print_sys_mesg("Done");
}


void ue_undelete_users(int number)
{
  char input[50];
  int user_number;
  char *dummy;
  int do_delete;
  int flag=1;
  struct user_data temp_user;

  print_cr();

  while(flag)
      {
        print_string("-->");
        do
        {
           get_string(input,6);
        }
        while (!*input);

        if (*input=='q' || *input=='Q')
            flag=0;                               /* he quit the menu */

        else
        {
           user_number=str_to_num(input,&dummy);
           if (user_number<1 || user_number>999)
              print_sys_mesg("Number Out of Range");
           else
            {

              if (load_user(user_number,&temp_user))
                 {print_sys_mesg("Error opening user file");
                  log_error("*error loading user to unDELETE*");
                  return;
                 }
              do_delete=1;

	     /*
              *  if (temp_user.user_info.number>=0)
              *    { do_delete=0;
              *      print_str_cr("That User Does Already Exists");
              *    }
              *  else 
	      */

              {   special_code(1,tswitch);
                  print_string("Handle: ");
                  print_str_cr(temp_user.user_info.handle);
                  print_string("Real Name: ");
                  print_str_cr(temp_user.real_info.name);
                  special_code(0,tswitch);
                  if (get_yes_no("Really UN Delete This User?"))
                  {
		    temp_user.user_info.user_no=user_number;
		    temp_user.user_info.enable=1;
		    if (save_user(user_number,&temp_user))
		      { print_sys_mesg("Error Saving UNDeleted User");
			log_error("*ERROR saving user to UNDELETE*");
			return;
		      }
		    print_str_cr("          -> UNDeleted <-");
		  
                  }
		}
	    }
	 }
      }
  print_sys_mesg("Done");
}


void ue_class_edit_prompt(int *number)
{
  char s[100];
  int num,flag=1;
  int test=1;
  int loop,line;
  int load_result;
  struct class_data temp_class;
  
  
  memset(&temp_class,0,sizeof(temp_class));
  
  list_classes();
  
  if (!number)
    {print_string("CLASS Name or Number to Edit : ");
     *s=0;
     while (!*s)
       get_string(s,60);
     
     if (*s=='#')
       num=atoi(s+1);
     else
       {
	 fix_classname(s);
	 num=-1;
       }
   }
  else num=*number;
  
  
  if (num!=-1)
    load_result = load_class(num,&temp_class);
  else
    load_result = read_class_by_name(s,&temp_class);
  
  
  if (load_result)
    {
      print_str_cr("ERROR LOADING CLASS");
      
      if (!get_yes_no("Would you like to create a new one? "))
	return;
      else
	{ int flag=1;
	  /* now ask him if he wants to base it on another class */
	  while (flag)
	    { char class_temp[80];
	      print_cr();
	      print_str_cr("What other Class would you like to base it on? (return for none)");
	      print_string("--> ");
	      get_string(class_temp,60);
	      if (!*class_temp)
		{
		  print_str_cr("Creating Empty Class");
		  flag=0;
		  /* make the class "empty" here */
                  memset(&temp_class,0,sizeof(temp_class));
                  temp_class.class_info.class_index=-1;
                  strcpy(temp_class.class_info.class_name,s);
		}
	      else
		{
		  fix_classname(class_temp);
		  if (!read_class_by_name(class_temp,&temp_class))
		    {
		      flag=0;
		      strcpy(temp_class.class_info.class_name,s);
		      temp_class.class_info.class_index=-1;
		    }
		  else
		    {
		      print_string("Error Loading: ");
		      print_str_cr(class_temp);
		    }
		  
		}
	      
	    }
	}
    }
  
  
    
  do {
    
    /* ce_class_edit_start(&edituser); */
    ce_class_edit_start_new(&temp_class);
    
    erase_region(9,23,tswitch);
    
    set_scrolling_region(9,23,tswitch);
    position(9,0);
    
    test = 1;
    print_cr();
    print_str_cr("[S]ave and Quit");
    print_str_cr("[C]ancel");
    print_str_cr("[A]bort Save and Quit");
    while (test)
      { print_cr();
	print_string("Option (S,C,A): ");
	
	do {
	  get_string(s,1);
	} while (!*s);
	
	switch (toupper(*s))
	  {
	  case 'A':
	    print_sys_mesg("Aborted");
	    test=0; flag=0;
	    break;
	  case 'C':
	    test=0;
	    break;
	  case 'S':
	    
	    /* OKAY, NOW WE HAVE TO SAVE THE USER AND EXIT */
	    if (temp_class.class_info.class_index>=0)
	      {
		
		print_str_cr("Saving Class by Number");
		if (!save_class(temp_class.class_info.class_index,&temp_class))
		  print_sys_mesg("CLASS Saved to Disk" );
		else
		  log_error("* tried to save in class editor and failed");
		
	      }
	    else
	      {
		print_str_cr("Saving Class by Name");
		if (!save_class_by_name(temp_class.class_info.class_name,&temp_class))
		  print_sys_mesg("CLASS saved to disk");
		else
		  { print_sys_mesg("CLASS save error");
		    log_error("* could not save class by name in class editor");
		  }
	      }
	    test=0; flag=0;
	    break;
	  default: test=1;
	    flag=1;
	    break;
	  } /* end switch */
      } /* end while */
    
  } while (flag);
  
  set_scrolling_region(0,24,tswitch);
  print_cr();
  clear_screen();
  
};



void ue_user_edit_prompt(int *number)
{
  char s[100];
  int num,flag=1;
  int test=1;
  int loop,line;
  struct user_data edituser;
  
  
  memset(&edituser,0,sizeof(edituser));
  
  if (!number)
    {print_string("User Number to Edit : ");
     *s=0;
     while (!*s)
       get_string(s,3);
     num=atoi(s);
   }
  
  else num=*number;
  
  if (!exist(num))
    {
      print_sys_mesg("User does not exist.");
      return;
    }
  
  if (num<0)
    {
      print_str_cr("Number out of range");
      return;
    }
  
  if ((line=is_user_online(num))<0)
    {
      if (load_user(num,&edituser))
	{
	  print_str_cr("ERROR LOADING USER");
	  return;
	}
    }
  else
    {
      /* UNIX UNIX
       * 
       * edituser.user_info = user_lines[line].user_info;
       * edituser.class_mod_info = user_lines[line].class_mod_info;
       * edituser.real_info = user_lines[line].real_info;
       */
    }
  
  do {
    
    ue_user_edit_start(&edituser);
    
    erase_region(9,23,tswitch);
    
    set_scrolling_region(9,23,tswitch);
    position(9,0);
    
    
    test = 1;
    print_cr();
    print_str_cr("[S]ave and Quit");
    print_str_cr("[C]ancel");
    print_str_cr("[A]bort Save and Quit");
    while (test)
      { print_cr();
	print_string("Option (S,C,A): ");
	
	do {
	  get_string(s,1);
	} while (!*s);
	
	switch (toupper(*s))
	  {
	  case 'A':
	    print_sys_mesg("Aborted");
	    test=0; flag=0;
	    break;
	  case 'C':
	    test=0;
	    break;
	  case 'S':
	    /* OKAY, NOW WE HAVE TO SAVE THE USER AND EXIT */
	    if (edituser.user_info.user_no>=0)
	      {
		int line;
		
		if (((line=is_user_online(edituser.user_info.user_no))<0) || 
		    (edituser.user_info.user_no<0))
		  {
		    if (!save_user(edituser.user_info.user_no,&edituser))
		      print_sys_mesg("User Saved to Disk" );
		    else
		      log_error("* tried to save in user editor and failed");
		  }
		else {
		  /* UNIX UNIX 
		   *
		   * user_lines[line].user_info=edituser.user_info;
		   * user_lines[line].class_mod_info = edituser.class_mod_info;
		   * user_lines[line].real_info = edituser.real_info;
		   * set_temp_user_info(line);
		   * print_sys_mesg("User Saved to Memory");
		   * line_status[line].handlelinechanged = ALL_BITS_SET;
		   */
		}
	      }
	    else
	      {
		print_sys_mesg("Negative User Numbers cannot be saved");
		log_error("* user editor tried to save a negative user number");
	      }
	    test=0; flag=0;
	    break;
	  default: test=1;
	    flag=1;
	    break;
	  } /* end switch */
      } /* end while */
    
  } while (flag);
  
  set_scrolling_region(0,24,tswitch);
  print_cr();
  clear_screen();
  
  
};


/* User edit prompt */




void ce_class_edit_start(struct class_data *editclass)
{
   char s[100];
   int num,flag;
   int loop,line;

   flag=1;

   while (flag)
      {

        print_cr();
        print_string("Editing Class [");
        sprintf(s,"%s]",editclass->class_info.class_name);
        print_str_cr(s);
        print_file("menu/EDITU.MNU");
        print_string(edit_user_prompt);
        *s=0;
        while (!*s)
          get_string(s,1);

        *s = toupper(*s);

        switch(*s)
        {
            case 'Q': return;
                      break;
            case '2':
                      edit_main_privs(editclass->class_info.privs,"sysop/main.prc");
                      break;
            case '3':
                      print_sys_mesg("Not Implemented");
                      break;
            case '4':
                     {
                        print_cr();
                        print_string("Editing Priorities/Time for Class [");
                        sprintf(s,"%s]",editclass->class_info.class_name);
                        print_str_cr(s);
                        print_cr();
                        ce_priv_time_edit(editclass);
                     }
                     break;
            case '5':
                     {
                       print_cr();

                       edit_staples(editclass);
                     }
                     break;
            case '1':
	      {
		ce_class_edit_start_new(editclass);
	      }
            default :
                    break;
        }
      }

   return;
}


void ue_new_user_prompt(int *newnumber)
{
  /* need a user data struct */
  struct user_data newuser;
  char selection[2]={0,0};
  int usertype;
  int flag=1;
  char line[100];
  int number;
  
  memset(&newuser,0,sizeof(newuser));
  
  if (!get_yes_no("Are you sure you want to create a NEW user?"))
    {
      /* NO, they don't want to create a user account  */
      return;
    }
  
  /* YES, they have decided that a new user is what they want*/
  /* FIRST, we will need to know what kind of user this will be */
  /* SYSOP/CO-SYSOP/BABY-CO-SYSOP/USER/REG-GUEST */
  
  print_file("menu/ueNEW.MNU");
  
  while(flag)
    {
      struct class_data temp_temp;
      
      list_classes();
      print_str_cr("Enter Class Name for new User.");
      print_string("--> ");
      
      get_string(line,60);
      if (!*line)
	{ print_cr();
	  print_sys_mesg("Aborted.");
	  return;
	}
      fix_classname(line);
      
      if (!read_class_by_name(line,&temp_temp))
	{
	  flag=0;
	  print_str_cr("Class loaded successfully");
	}
      else
	print_str_cr("Error, that class does not exist");
      
    }
  
  strcpy(newuser.user_info.class_name,line);
  
  /* Print out the user Stats */
  print_cr();
  flag=1;
  if (!newnumber)
    while (flag)
      { char *dummy;
	*line=0;
	print_string("Enter User Number to Create : ");
	get_string(line,3);
	
	if (!*line)
	  {
	    print_cr();
	    print_str_cr(" [Aborted]");
	    return;
	  }
	number=str_to_num(line,&dummy);
	
	if (exist(number))
	  {
	    if (get_yes_no("That user already exists, still create new?"))
	      {
		flag=0;
		
		if (number>0 && number<1000)
		  flag=0;
		
		if ((number==0) && (*dummy=='+'))
		  flag = 0;
	      }	 
	  }
	else
	  {
	    if (number>0 && number<1000)
	      flag=0;
	    
	    if ((number==0) && (*dummy=='+'))
	      flag = 0;	 
	  }
	
	
      }
  else
    if ((*newnumber<=0) || (*newnumber>999))
      { print_sys_mesg("Illegal User Number");
	return; 
      }
    else number=*newnumber;
  
  
  sprintf(line,"Creating New User [%03d]",number);
  print_cr();
  print_str_cr(line);
  
  
  print_cr();
  /* init the user structure*/
  *newuser.real_info.name=0;
  *newuser.real_info.street=0;
  *newuser.real_info.state_or_province=0;
  *newuser.real_info.city=0;
  *newuser.real_info.postal_code=0;
  *newuser.real_info.phone=0;
  *newuser.real_info.phone2=0;
  
  
  print_string("Creating New ");
  print_str_cr(newuser.user_info.class_name);
  print_cr();
  
  print_string("Enter Name       : ");
  while (!*(newuser.real_info.name))
    get_string(newuser.real_info.name,USER_REAL_NAME_LEN);
  print_string("Enter Street     : ");
  
  while (!*(newuser.real_info.street))
    get_string(newuser.real_info.street,USER_STREET_LEN);
  print_string("Enter City       : ");
  while (!*(newuser.real_info.city))
    get_string(newuser.real_info.city,USER_CITY_LEN);
  print_string("Enter State or Province  : ");
  while (!*(newuser.real_info.state_or_province))
    get_string(newuser.real_info.state_or_province,USER_STATE_LEN);
  print_string("Enter Postal Code    : ");
  while (!*(newuser.real_info.postal_code))
    get_string(newuser.real_info.postal_code,USER_POSTAL_CODE_LEN);
  
  print_string("Enter Phone 1    : ");
  nu_get_phone(newuser.real_info.phone,10);
  
  print_string("Enter Phone 2    : ");
  nu_get_phone(newuser.real_info.phone2,10);
  
  print_cr();
  print_string("Enter Password        : ");
  /* UNIX UNIX */
  { char passtemp[11];
    
    while (!*(passtemp))
      get_string(passtemp,10);
    
    printf("WE DIDNT DO ANYTHING WITH THE PASSWORD, DAVE FIX!!!\n");
  }
  
  newuser.user_info.expiration=0;
  newuser.user_info.starting_date=0;
  newuser.user_info.credit=0;
  
  /* get the birthdate */
  
  {
    char temp[5];
    print_str_cr("Enter Birthdate");
    do
      {
	print_string("Month: ");
	do { get_string(temp,2); } while (!*temp);
	newuser.real_info.birth_date.month = (char) atoi(temp);
      }
    while ((newuser.real_info.birth_date.month<1) && 
	   (newuser.real_info.birth_date.month>12));
    
    do
      {
	print_string("Day  : ");
	do { get_string(temp,2); } while (!*temp);
	
	newuser.real_info.birth_date.day = (char) atoi(temp);
      }
    while ((newuser.real_info.birth_date.day<1) && 
	   (newuser.real_info.birth_date.day>31));
    
    do
      {
	print_string("Year : ");
	do { get_string(temp,4); } while (!*temp);
	newuser.real_info.birth_date.year = (unsigned int) atoi(temp);
      }
    while ((newuser.real_info.birth_date.year<1800) && 
	   (newuser.real_info.birth_date.year>9999));
    
  } /* end get birthdate */
  
  
  
  newuser.user_info.conception=time(NULL);
  newuser.user_info.last_call=time(NULL);
  
  set_new_expiration_date(&newuser);
  
  print_str_cr("Setting User Enabled Flag");
  newuser.user_info.enable=1;
  print_str_cr("Setting User Number");
  newuser.user_info.user_no=number;
  print_cr();
  flag=1;
  while (flag)
    {
      print_string("--> Are you sure you want to save this user?");
      
      
      do {
	get_string(selection,1);
      } while (!*selection);
      
      if (*selection=='n' || *selection=='N')
	{
	  print_cr();
	  print_str_cr("********** ABORTING NEW USER **********");
	  return;
	}
      if (*selection=='y' || *selection=='Y')
	flag=0;
    }
  
  print_string("Cleaning User Directories...");
  prep_user_dirs(number);
  print_str_cr("Done.");
  
  if (save_user(number,&newuser))
    {
      log_error("* error trying to save new user in UserEditor");
      print_sys_mesg("Error saving new user");
      return;
    }
  print_sys_mesg("User account saved ");
}

void start_user_edit(char *str,char *name,int portnum)
{
  int number;
  char *point;
  
  number=str_to_num(str,&point);
  
  
  /* GET THE USER EDITOR PASSWORD */
  
  /* OKAY THE PASSWORD WAS CORRECT */
  
  if (number==-1)
    {
      
      print_file("menu/UEMAIN.MNU");
      
      ue_main_prompt();
      /*   run the user editor from the main menu */
    }
  else
    {
      /* they entered a number */
      
      if (!exist(number))
	{
	  /* user does not exist so run new user maker */
	  print_file("menu/UENEWU.MNU");
	  /* DEBUG */
	  ue_new_user_prompt(&number);
	  
	}
      else
        /* start the user editor for the entered user right now */
        {
	  print_file("menu/UEEDITU.MNU");
	  ue_user_edit_prompt(&number);
        }
    }
  
  print_sys_mesg("GinsuTalk: Returning to System");
}


void ue_main_prompt(void)
{
  char selection[2]={0,0};

  while(*selection!='Q')
   {
    check_for_privates();
    print_string(um_main_prompt);
    *selection=0;
    while (!*selection)
       get_string(selection,1);
   if ((*selection>=97) && (*selection<=122))
      *selection-=32;

    if (*selection=='?')
        print_file("menu/UEMAIN.MNU");
    else
    if (*selection=='N')
       {
        print_file("menu/UENEWU.MNU");
        ue_new_user_prompt(NULL);
       }
    else
    if (*selection=='E')
      {
       print_file("menu/UEEDITU.MNU");
	   ue_user_edit_prompt(NULL);
	  }
   else
   if (*selection=='C')
	  {
	   print_file("menu/UEEDITC.MNU");
	   ue_class_edit_prompt(NULL);
	  }
   else
   if (*selection=='L')
     {
        print_file("menu/UECLIST.MNU");
        list_classes();
     }
   else
   if (*selection=='D')
      {print_file("menu/UEDELETE.MNU");
       ue_delete_users(0);
      }
   else
   if (*selection=='U')
      { print_file("menu/ueundel.mnu");
        ue_undelete_users(0);
      }
   else
   if (*selection=='P')
      {print_file("menu/UEPRIV.MNU");
       ue_priv_change();
      }
   else
   if (*selection=='G')
     { print_file("menu/UEGLOBAL.MNU");
       ue_global_copy();
     }
   }

}
void edit_credit(time_t *credit)
{
 char input[6];
 unsigned char days;
 char *dummy;


 print_cr();
 print_string("Enter Days Credit: ");
 do
 { get_string(input,5);
 } while(!*input);
 days=str_to_num(input,&dummy);
 *credit=(86400l*days);
 print_str_cr("Credit Set.");

}

void set_new_expiration_date(struct user_data *edit_data)
{
    char s[80];
    char line[10];
    int notquit = 1;
    int option;


    while (notquit)
     {
       print_cr();
       if (edit_data->user_info.expiration)
        {
         sprintf(s,"Current expiration date: %s",
            asctime(localtime(&edit_data->user_info.expiration)));
        }
        else sprintf(s,"Current expiration date: None");

       print_str_cr(s);

       if (edit_data->user_info.starting_date)
        {
         sprintf(s,"  Current starting date: %s",
            asctime(localtime(&edit_data->user_info.starting_date)));
        }
        else sprintf(s,"  Current starting date: None");
       print_str_cr(s);


       if (edit_data->user_info.credit)
         sprintf(s,"                 CREDIT: %u days",(edit_data->user_info.credit)/86400l);
       else
         strcpy(s, "                 CREDIT: None");
       print_str_cr(s);

       print_str_cr("1. Edit Expiration Date");
       print_str_cr("2. No Expiration Date");
       print_str_cr("3. Set CREDIT");
       print_str_cr("4. Set Starting Date");
       print_str_cr("5. No Starting Date");
       print_str_cr("Q. Quit");
       print_string("Option (1,2,Q): ");
       get_string(line,5);
       option=atoi(line);
       if ((*line=='Q') || (*line=='q')) notquit = 0;
       switch (option)
        {
          case 1: edit_data->user_info.expiration = enter_date(0);
                  edit_data->user_info.credit=0;
                  break;
          case 2: edit_data->user_info.expiration = 0;
                  edit_data->user_info.credit=0;
                  break;
          case 3: edit_credit(&(edit_data->user_info.credit));
                  break;
          case 4: edit_data->user_info.starting_date = enter_date(0);
                  break;
          case 5: edit_data->user_info.starting_date=0;
                  break;
        };
     };
    return;
};

time_t enter_date(int time_too)
{
    struct tm temp_dt;
    time_t temp_t;
    char line[10];
    temp_dt.tm_sec = 0;
    temp_dt.tm_min = 0;
    temp_dt.tm_hour = 0;
    temp_dt.tm_isdst = 0;

    print_cr();
    temp_dt.tm_mon = -1;
    while ((temp_dt.tm_mon<0) || (temp_dt.tm_mon>11))
     {
       print_string("Month: (1-12) ");
       get_string(line,5);
       temp_dt.tm_mon=atoi(line)-1;
     };
    temp_dt.tm_mday = -1;
    while ((temp_dt.tm_mday<1) || (temp_dt.tm_mday>31))
     {
       print_string("Day: (1-31) ");
       get_string(line,5);
       temp_dt.tm_mday=atoi(line);
     };
    temp_dt.tm_year = -1;
    while ((temp_dt.tm_year<0) || (temp_dt.tm_year>99))
     {
       print_string("Year: 19(00-99): ");
       get_string(line,5);
       temp_dt.tm_year=atoi(line);
     };
    if (time_too)
     {
        temp_dt.tm_hour = -1;
        while ((temp_dt.tm_hour<0) || (temp_dt.tm_hour>11))
         {
           print_string("Hour: (0-23) ");
           get_string(line,5);
           temp_dt.tm_hour=atoi(line);
         };
        temp_dt.tm_min = -1;
        while ((temp_dt.tm_min<0) || (temp_dt.tm_min>59))
         {
           print_string("Minute: (0-59) ");
           get_string(line,5);
           temp_dt.tm_min=atoi(line);
         };
        temp_dt.tm_sec = -1;
        while ((temp_dt.tm_sec<0) || (temp_dt.tm_sec>59))
         {
           print_string("Second: (0-59) ");
           get_string(line,5);
           temp_dt.tm_sec=atoi(line);
         };
     };
    temp_t = mktime(&temp_dt);
    return (temp_t);
};



