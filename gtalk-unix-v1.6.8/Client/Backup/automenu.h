/* AUTOMENU.H */

#ifndef GT_AUTOMENU_H
#define GT_AUTOMENU_H

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

#include "automst.h"
#include "keys.h"


/* commands that the handlers should implement */

#define FIELD_DISPLAY               0
#define FIELD_DISPLAY_IF_DIRTY      1
#define FIELD_EDIT                  2
#define FIELD_HIGHLIGHT             3
#define FIELD_UNHIGHLIGHT           4

#define MENU_SAVE                   -1
#define MENU_QUIT                   -2
#define MENU_NEXT                   -3
#define MENU_PREV                   -4
#define MENU_DATA_CHANGED           -5


#define NONE (-1)

/* prototypes */


struct string_field_info_struct {
    int max_len;
    int screen_len;
    int editable;
    char mask_char;
};
int string_field(struct field_info_struct *field,struct a_menu_struct *menu,int command,
              struct return_info_struct *rinfo,struct dirty_rect_struct *dirty);

int phone_field(struct field_info_struct *field,struct a_menu_struct *menu,int command,
              struct return_info_struct *rinfo,struct dirty_rect_struct *dirty);

#define DATE_FIELD_REL_MODE_NONE       0
#define DATE_FIELD_REL_MODE_SUM        1
#define DATE_FIELD_REL_MODE_DIFFERENCE 2


struct date_field_info_struct {
    time_t min;
    time_t max;
    int relative;
    time_t rel_to;
};

int date_field(struct field_info_struct *field,struct a_menu_struct *menu,int command,
              struct return_info_struct *rinfo,struct dirty_rect_struct *dirty);
int raw_date_field(struct field_info_struct *field,struct a_menu_struct *menu,int command,
              struct return_info_struct *rinfo,struct dirty_rect_struct *dirty);
int clear_date_field(struct field_info_struct *field,struct a_menu_struct *menu,int command,
              struct return_info_struct *rinfo,struct dirty_rect_struct *dirty);

int button_field(struct field_info_struct *field,struct a_menu_struct *menu,int command,
              struct return_info_struct *rinfo,struct dirty_rect_struct *dirty);


#endif
