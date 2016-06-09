

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* automenu.c */

int tswitch=0;

/* headers */

#include <time.h>
#include <sys/types.h>
#include "userst.h"

#include "automenu.h"
#include "ansi.h"


/*
 * int process_ansi_key(void);
 *
 * this routine returns a result code for the following keys
 *
 * PA_UP_ARROW
 * PA_DOWN_ARROW
 * PA_LEFT_ARROW
 * PA_RIGHT_ARROW
 * PA_PAGE_UP
 * PA_PAGE_DOWN
 *
 * for any other key it returns the ASCII value. These return codes are
 * all less than 0
 */

int process_ansi_key(void)
{
    return (wait_ch());
}

/*
 * int paint_dirty_ansi(struct a_menu_struct *menu, struct dirty_rect_struct *dirty);
 *
 * This asks the fields to repaint themselves if their dirty
 *
 */

int paint_dirty_ansi(struct a_menu_struct *menu, struct dirty_rect_struct *dirty)
{
    struct field_info_struct *cur;
    struct return_info_struct r_info;

    cur = menu->fields;

    while (cur->title)
    {
        cur->handler(cur,menu,FIELD_DISPLAY_IF_DIRTY,&r_info,dirty);
        cur++;
    }

    return (0);
}

/*
 * void paint_info_row(void);
 *
 * This paints the info row at the bottom. It should be similar to:
 * -> Ctrl-S to Save, Ctrl-A to abort, Ctrl-P for previous, Ctrl-N for next <-
 *
 */

char menu_info[] = "|*r1|*h1-> |*f2Ctrl-W |*f1to Write, |*f2Ctrl-A|*f1 to abort, |*f2Ctrl-P|*f1 for previous,|*f2 Ctrl-N|*f1 for next |*r1|*h1<-";

void paint_info_row(void)
{
    int old_code;
    int portnum = tswitch;
    int len = ansi_strlen(menu_info);
    int width = 80;
    int pos;

    if (len>width)
      pos = 0;
    else
      pos = (width - len)>>1;

    position(24,pos);

    old_code = ansi_on(1);
    print_string(menu_info);
    ansi_on(old_code);
}

int find_hotkey_match(struct a_menu_struct *menu,int in_key)
{
struct field_info_struct *cur;
int key = toupper(in_key);
int count = 0;

cur = menu->fields;

  while (cur->title)
  {
    if (key == toupper(cur->hot_key))
    {
      return (count);
    }

    cur++;
    count++;
  }
  return (NONE);

}

void show_help(struct a_menu_struct *menu,struct field_info_struct *cur,
               struct field_info_struct *old)
{
   int portnum = tswitch;
   int old_code = ansi_on(1);

   if (!cur->help_info)
     return;

   if (old)
    if (old!=cur)
     { int old_len, new_len;
       int x;

       old_len = ansi_strlen(old->help_info);
       new_len = ansi_strlen(cur->help_info);

        position(menu->help_info->y_pos,menu->help_info->x_pos);
        print_string(cur->help_info);

       if ((new_len) < (old_len))
       {
           if (menu->help_info->attribute_string)
             print_string(menu->help_info->attribute_string);
           for (x=0; x < (old_len-new_len); x++)
             print_chr(' ');
        }
        reset_attributes(portnum);

     }
   else
   {

       position(menu->help_info->y_pos,menu->help_info->x_pos);
       print_string(cur->help_info);
       reset_attributes(portnum);
   }
   ansi_on(old_code);

}

void erase_help(struct a_menu_struct *menu,struct field_info_struct *cur)
{
   int portnum = tswitch;
   int old_code = ansi_on(1);
   int len;
   int x;

   if (!cur->help_info)
     return;

   len = strlen(cur->help_info);

   position(menu->help_info->y_pos,menu->help_info->x_pos);
   if (menu->help_info->attribute_string)
     print_string(menu->help_info->attribute_string);
   for (x=0;x<len;x++)
     print_chr(' ');
   reset_attributes(portnum);

   ansi_on(old_code);

}

/*
 * int do_menu_ansi(struct a_menu_struct *menu);
 *
 * this clears the screen and creates the ansi menu. It handles all
 * input and only returns when the menu has been completed.
 * Return codes include
 *
 * MENU_SAVE
 * MENU_QUIT
 * MENU_NEXT
 * MENU_PREV
 * MENU_DATA_CHANGED
 */

void refresh_changed_data(struct a_menu_struct *menu,struct field_info_struct *field)
{
    void *offset = field->offset;
    int struct_num = field->structure_number;
    struct field_info_struct *field_loop;
    struct return_info_struct r_info;
    struct a_menu_struct *this_menu = menu;



    do
    {
        field_loop = this_menu->fields;
        while(field_loop->offset || field_loop->title)
        {
          if ((field_loop->offset == offset) && (field_loop->structure_number == struct_num))
          {
              if (field_loop!=field);
                 field_loop->handler(field_loop,menu,FIELD_DISPLAY,&r_info,NULL);
          }

          field_loop++;
        }

   }
   while ((this_menu = this_menu->old_menu)!=NULL);
}


int do_menu_ansi(struct a_menu_struct *menu,struct field_info_struct *start_field)
{
   struct field_info_struct *cur;
   struct field_info_struct *new;
   struct field_info_struct *old;
   struct return_info_struct r_info;
   struct dirty_rect_struct dirty;
   int key_hit;
   int return_code;

   if (!start_field)
      start_field = menu->fields;

    old = cur = new = start_field;



   /* Clear the screen and MOVE TO THE UPPER LEFT */
   /*   clear_screen(); */

   /* Paint the screen the first time */

   paint_dirty_ansi(menu,NULL);

   /* Put the control information at the bottom */

   paint_info_row();

   show_help(menu,cur,old);
   return_code = cur->handler(cur,menu,FIELD_HIGHLIGHT,&r_info,NULL);

   while (1)
   {
         dirty.dirty_flag=0;
         dirty.all_dirty=0;

          switch(return_code)
          {
             case 0:
             default:
                       key_hit = process_ansi_key();
                       return_code = 0;
                       break;
             case MENU_QUIT:
                       key_hit = ASCII_CTRL_A;
                       break;
             case MENU_SAVE:
                       key_hit = ASCII_CTRL_W;
                       break;
             case MENU_NEXT:
                       key_hit = ASCII_CTRL_N;
                       break;
             case MENU_PREV:
                       key_hit = ASCII_CTRL_P;
                       break;
             case MENU_DATA_CHANGED:
                       refresh_changed_data(menu,cur);
                       cur->handler(cur,menu,FIELD_HIGHLIGHT,&r_info,&dirty);
                       key_hit = process_ansi_key();
                       return_code = 0;
                       break;
          }



         switch(key_hit)
         {
            case 13:    /* CR */
                        erase_help(menu,cur);
                        return_code = cur->handler(cur,menu,FIELD_EDIT,&r_info,&dirty);
                        show_help(menu,cur,old);
                        cur->handler(cur,menu,FIELD_HIGHLIGHT,&r_info,&dirty);
                        break;

            case PA_RIGHT_ARROW:
            case 'l':
            case 'L': /* right */
                        if (cur->right_field != NONE)
                          new = &menu->fields[cur->right_field];
                        break;

            case PA_LEFT_ARROW:
            case 'h':
            case 'H': /* left */
                        if (cur->left_field != NONE)
                          new = &menu->fields[cur->left_field];
                        break;

            case PA_UP_ARROW:
            case 'k':
            case 'K': /* up */
                        if (cur->up_field != NONE)
                          new = &menu->fields[cur->up_field];
                        break;

            case PA_DOWN_ARROW:
            case 'j':
            case 'J': /* down */
                        if (cur->down_field != NONE)
                          new = &menu->fields[cur->down_field];
                        break;


            case ASCII_CTRL_N: /* next field */
                        if (cur->next_field != NONE)
                          new = &menu->fields[cur->next_field];
                        break;

            case ASCII_CTRL_P: /* previous field */
                        if (cur->prev_field != NONE)
                          new = &menu->fields[cur->prev_field];
                        break;
            case ASCII_CTRL_A: /* abort */
                      erase_help(menu,cur);
                      return_code = cur->handler(cur,menu,FIELD_UNHIGHLIGHT,&r_info,&dirty);

                      position(24,79);
                      return (1);

            case ASCII_CTRL_W: /* save/write */
                      erase_help(menu,cur);
                      return_code = cur->handler(cur,menu,FIELD_UNHIGHLIGHT,&r_info,&dirty);

                      position(24,79);
                      return (0);

            case NONE: break;
            default:
                      {
                        int tempnum = find_hotkey_match(menu,key_hit);
                        if (tempnum != NONE)
                          new = &menu->fields[tempnum];
                      }
                      break;
         }

      if (dirty.dirty_flag)
       {
         if (dirty.all_dirty)
         {
           paint_dirty_ansi(menu,NULL);
           show_help(menu,cur,old);
           return_code = cur->handler(cur,menu,FIELD_HIGHLIGHT,&r_info,NULL);
         }

       }

      if (new!=cur)
       {
          old = cur;
          return_code = cur->handler(cur,menu,FIELD_UNHIGHLIGHT,&r_info,&dirty);
          cur = new;
          show_help(menu,cur,old);
          return_code =  cur->handler(cur,menu,FIELD_HIGHLIGHT,&r_info,&dirty);
       }

      if (dirty.dirty_flag)
       {
         if (dirty.all_dirty)
         {
           paint_dirty_ansi(menu,NULL);
           show_help(menu,cur,old);
           paint_info_row();
           return_code = cur->handler(cur,menu,FIELD_HIGHLIGHT,&r_info,NULL);
         }

       }

   }

}


void print_masked_string(char *string,char mask)
{
      if (mask)
        {
         repeat_chr(mask,strlen(string),0);
        }
      else
        print_string(string);
}
/*
 *  int string_field(struct field_info_struct *field,int command,
 *             struct return_info_struct *rinfo,struct dirty_rect_struct *dirty);
 *
 * This field edits normal strings.
 *
 */


int string_field(struct field_info_struct *field,struct a_menu_struct *menu,int command,
              struct return_info_struct *rinfo,struct dirty_rect_struct *dirty)
{
    struct string_field_info_struct *temp_info = field->info;
    struct string_field_info_struct info;
    int portnum = tswitch;
    int old_code;
    int len;
    int ret_code=0;
    char *data;
    char null_string[] = "";
    char s[100];

    if (temp_info)
       info = *temp_info;
    else
    {
       struct string_field_info_struct temp_sinfo =
          { 10, 20, 1 ,0};  /* default info for a string field */
       info = temp_sinfo;
    }

    old_code = ansi_on(1);

    if (menu->sts)
    {
        if (field->structure_number > menu->sts->count)
          data = null_string;
        else
         {
          data = menu->sts->structs[field->structure_number];
          data = data + (int)field->offset;
         }
    }
    else
     data = null_string;


    switch(command)
    {
        case FIELD_HIGHLIGHT:
                position(field->y_pos,field->x_pos);
                bold_video();
                print_string(field->title);
                reset_attributes(portnum);
                position(field->y_pos,field->x_pos);
                break;

        case FIELD_EDIT:
                if (!info.editable)
                 break;

                len = ansi_strlen(field->title);

                position(field->y_pos,field->x_pos + len + 2);
                print_string("|*f0|*b6");
                repeat_chr(' ',info.screen_len,0);


                position(field->y_pos,field->x_pos + len + 2);


                get_input_cntrl_pos(data,info.screen_len,0,0,1,0,0,0,0,0);

                /*
                print_masked_string(data,info.mask_char);
                get_input_cntrl_pos(data,info.screen_len,info.mask_char,0,1,0,0,0,0,strlen(data));

                */

                reset_attributes(portnum);
                position(field->y_pos,field->x_pos + len + 2);
                repeat_chr(' ',info.screen_len,0);
                position(field->y_pos,field->x_pos + len + 2);
                print_masked_string(data,info.mask_char);

                position(field->y_pos,field->x_pos);
                ret_code = MENU_DATA_CHANGED;
                break;

        case FIELD_UNHIGHLIGHT:
                position(field->y_pos,field->x_pos);
                print_string(field->title);
                reset_attributes(portnum);
                break;

        default:
                position(field->y_pos,field->x_pos);
                print_string(field->title);
                reset_attributes(portnum);
                if (info.screen_len)
                {
                  print_string(": ");
                  print_masked_string(data,info.mask_char);
                }
                break;
    }

    ansi_on(old_code);
    return (ret_code);
}

int phone_field(struct field_info_struct *field,struct a_menu_struct *menu,int command,
              struct return_info_struct *rinfo,struct dirty_rect_struct *dirty)
{
    int portnum = tswitch;
    int old_code;
    int len;
    char *data;
    char null_string[] = "";
    int ret_code=0;
    char s[15];

    old_code = ansi_on(1);


    if (field->structure_number > menu->sts->count)
      data = null_string;
    else
     {
      data = menu->sts->structs[field->structure_number];
      data = data + (int)field->offset;
     }


    switch(command)
    {
        case FIELD_HIGHLIGHT:
                position(field->y_pos,field->x_pos);
                bold_video();
                print_string(field->title);
                reset_attributes(portnum);
                position(field->y_pos,field->x_pos);
                break;

        case FIELD_EDIT:
                len = ansi_strlen(field->title);

                position(field->y_pos,field->x_pos + len + 2);
                print_string("|*f0|*b6");
                repeat_chr(' ',13,0);

                position(field->y_pos,field->x_pos + len + 2);

                nu_get_phone(s,1);
                if (*s)
                 strcpy(data,s);

                reset_attributes(portnum);

                position(field->y_pos,field->x_pos + len + 2);
                nu_print_phone(data);

                position(field->y_pos,field->x_pos);
                ret_code = MENU_DATA_CHANGED;
                break;

        case FIELD_UNHIGHLIGHT:
                position(field->y_pos,field->x_pos);
                print_string(field->title);
                reset_attributes(portnum);
                break;

        default:
                position(field->y_pos,field->x_pos);
                print_string(field->title);
                reset_attributes(portnum);
                print_string(": ");

                nu_print_phone(data);
                /* print the string here */

                break;
    }

    ansi_on(old_code);
    return (ret_code);
}


int date_field(struct field_info_struct *field,struct a_menu_struct *menu,int command,
              struct return_info_struct *rinfo,struct dirty_rect_struct *dirty)
{
    int portnum = tswitch;
    int old_code;
    int len;
    char *data;
    time_t date;
    int ret_code=0;
    struct tm my_tm;
    struct exp_date the_date;

    old_code = ansi_on(1);

    if (field->structure_number > menu->sts->count)
      {
        position(field->y_pos,field->x_pos);
        bold_video();
        print_string("Field Data Error");
        reset_attributes(portnum);
        position(field->y_pos,field->x_pos);
        return (ret_code);
      }
    else
     {
      data = menu->sts->structs[field->structure_number];
      data = data + (int)field->offset;
     }

    date = *((time_t *)data);
    if (date)
    {
        my_tm = *localtime(&date);

        the_date.day   = my_tm.tm_mday;
        the_date.month = my_tm.tm_mon+1;
        the_date.year  = my_tm.tm_year;
    }

    switch(command)
    {
        case FIELD_HIGHLIGHT:
                position(field->y_pos,field->x_pos);
                bold_video();
                print_string(field->title);
                reset_attributes(portnum);
                position(field->y_pos,field->x_pos);
                break;

        case FIELD_EDIT:
                len = ansi_strlen(field->title);

                position(field->y_pos,field->x_pos + len + 2);
                print_string("|*f0|*b4");
                repeat_chr(' ',8,0);

                position(field->y_pos,field->x_pos + len + 2);

                nu_get_date(&the_date,6);

                if (the_date.day || the_date.month || the_date.year)
                {
                    my_tm.tm_mday = the_date.day;
                    my_tm.tm_mon  = the_date.month-1;
                    my_tm.tm_year = the_date.year;
                    *((time_t *)data) = mktime(&my_tm);
                    reset_attributes(portnum);

                    position(field->y_pos,field->x_pos + len + 2);
                    nu_print_date(&the_date,6);
                }
                else
                {
                    *data = 0;
                    reset_attributes(portnum);

                    position(field->y_pos,field->x_pos + len + 2);
                    print_string("(None)  ");
                }

                position(field->y_pos,field->x_pos);
                ret_code = MENU_DATA_CHANGED;
                break;

        case FIELD_UNHIGHLIGHT:
                position(field->y_pos,field->x_pos);
                print_string(field->title);
                reset_attributes(portnum);
                break;

        default:
                position(field->y_pos,field->x_pos);
                print_string(field->title);
                reset_attributes(portnum);

                print_string(": ");
                if (date)
                   nu_print_date(&the_date,6);
                else
                   print_string("(None)  ");

                break;
    }

    ansi_on(old_code);
    return (ret_code);
}

int raw_date_field(struct field_info_struct *field,struct a_menu_struct *menu,int command,
              struct return_info_struct *rinfo,struct dirty_rect_struct *dirty)
{
    int portnum = tswitch;
    int old_code;
    int len;
    char *data;
    time_t date;
    int ret_code=0;
    struct tm my_tm;
    struct exp_date the_date;

    old_code = ansi_on(1);

    if (field->structure_number > menu->sts->count)
      {
        position(field->y_pos,field->x_pos);
        bold_video();
        print_string("Field Data Error");
        reset_attributes(portnum);
        position(field->y_pos,field->x_pos);
        return (ret_code);
      }
    else
     {
      data = menu->sts->structs[field->structure_number];
      data = data + (int)field->offset;
     }

    the_date = *((struct exp_date *)data);

    switch(command)
    {
        case FIELD_HIGHLIGHT:
                position(field->y_pos,field->x_pos);
                bold_video();
                print_string(field->title);
                reset_attributes(portnum);
                position(field->y_pos,field->x_pos);
                break;

        case FIELD_EDIT:
                len = ansi_strlen(field->title);

                position(field->y_pos,field->x_pos + len + 2);
                print_string("|*f0|*b4");
                repeat_chr(' ',8,0);

                position(field->y_pos,field->x_pos + len + 2);

                nu_get_date(&the_date,6);

                if (the_date.day || the_date.month || the_date.year)
                {
                    *((struct exp_date *)data) = the_date;
                    reset_attributes(portnum);

                    position(field->y_pos,field->x_pos + len + 2);
                    nu_print_date(&the_date,6);
                }
                else
                {
                    *data = 0;
                    reset_attributes(portnum);

                    position(field->y_pos,field->x_pos + len + 2);
                    print_string("(None)  ");
                }

                position(field->y_pos,field->x_pos);
                ret_code = MENU_DATA_CHANGED;
                break;

        case FIELD_UNHIGHLIGHT:
                position(field->y_pos,field->x_pos);
                print_string(field->title);
                reset_attributes(portnum);
                break;

        default:
                position(field->y_pos,field->x_pos);
                print_string(field->title);
                reset_attributes(portnum);

                print_string(": ");
                if (date)
                   nu_print_date(&the_date,6);
                else
                   print_string("(None)  ");

                break;
    }

    ansi_on(old_code);
    return (ret_code);
}

int clear_date_field(struct field_info_struct *field,struct a_menu_struct *menu,int command,
              struct return_info_struct *rinfo,struct dirty_rect_struct *dirty)
{
    int portnum = tswitch;
    int old_code;
    int len;
    char *data;
    time_t date;
    int ret_code=0;
    struct tm my_tm;
    struct exp_date the_date;

    old_code = ansi_on(1);

    if (field->structure_number > menu->sts->count)
      {
        position(field->y_pos,field->x_pos);
        bold_video();
        print_string("Field Data Error");
        reset_attributes(portnum);
        position(field->y_pos,field->x_pos);
        return (ret_code);
      }
    else
     {
      data = menu->sts->structs[field->structure_number];
      data = data + (int)field->offset;
     }

    switch(command)
    {
        case FIELD_HIGHLIGHT:
                position(field->y_pos,field->x_pos);
                bold_video();
                print_string(field->title);
                reset_attributes(portnum);
                position(field->y_pos,field->x_pos);
                break;

        case FIELD_EDIT:
                *((time_t *)data)=0;
                ret_code = MENU_DATA_CHANGED;
                break;

        case FIELD_UNHIGHLIGHT:
                position(field->y_pos,field->x_pos);
                print_string(field->title);
                reset_attributes(portnum);
                break;

        default:
                position(field->y_pos,field->x_pos);
                print_string(field->title);
                reset_attributes(portnum);
                break;
    }

    ansi_on(old_code);
    return (ret_code);
}


/*
 *  int button_field(struct field_info_struct *field,int command,
 *             struct return_info_struct *rinfo,struct dirty_rect_struct *dirty);
 *
 *
 */


int button_field(struct field_info_struct *field,struct a_menu_struct *menu,int command,
              struct return_info_struct *rinfo,struct dirty_rect_struct *dirty)
{
    int portnum = tswitch;
    int old_code;
    int len;
    char *data;
    char null_string[] = "";
    char s[100];
    int (*next_menu)(struct structures_used_struct *a_struct,
                     struct dirty_rect_struct *dirty,
                     struct a_menu_struct *old_menu);
    int return_code=0;

    old_code = ansi_on(1);


    data = field->info;

    switch(command)
    {
        case FIELD_HIGHLIGHT:
                position(field->y_pos,field->x_pos);
                bold_video();
                print_string(field->title);
                reset_attributes(portnum);
                position(field->y_pos,field->x_pos);
                break;

        case FIELD_EDIT:
                next_menu = data;
                if (data)
                {
                  position(field->y_pos,field->x_pos);
                  blink_video(portnum);
                  print_string(field->title);
                  reset_attributes(portnum);
                  position(field->y_pos,field->x_pos);

                  return_code = next_menu(menu->sts,dirty,menu);

                  position(field->y_pos,field->x_pos);
                  bold_video();
                  print_string(field->title);
                  reset_attributes(portnum);
                  position(field->y_pos,field->x_pos);

                }
                position(field->y_pos,field->x_pos);
                break;

        case FIELD_UNHIGHLIGHT:
                position(field->y_pos,field->x_pos);
                print_string(field->title);
                reset_attributes(portnum);
                break;

        default:
                position(field->y_pos,field->x_pos);
                print_string(field->title);
                reset_attributes(portnum);
                break;
    }

    ansi_on(old_code);
    return (return_code);
}


