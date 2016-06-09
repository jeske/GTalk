/* AUTOMST.H */

#ifndef GT_AUTOMST_H
#define GT_AUTOMST_H

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */



#define NULL_OFFSET ((unsigned int)0)

struct a_menu_struct;

struct dirty_rect_struct {
    int x1,y1;
    int x2,y2;
    int all_dirty;
    int dirty_flag;
};

struct return_info_struct {
    int last_char;
    char field_edited;
    struct dirty_rect_struct dirty_rect;
    char is_rect_dirty;
    char field_movement;
};

struct structures_used_struct {
    int count;
    void **structs;
};

struct field_info_struct {
    int x_pos;
    int y_pos;
    char *title;
    int structure_number;
    void *offset;
    int (*handler)(struct field_info_struct *field,struct a_menu_struct *menu,int command,struct return_info_struct *rinfo,struct dirty_rect_struct *dirty);
    int next_field,prev_field,up_field,down_field,
                             left_field,right_field;
    char hot_key;
    void *info;
    char *help_info;
};

struct menu_info_struct {
    char *menu_name;
    int x_name_pos;
    int y_name_pos;
    int attribute_string;
    int name_max_len;
};

struct menu_help_info_struct {
    int x_pos;
    int y_pos;
    char *attribute_string;
    int max_len;
};

struct a_menu_struct {
    struct field_info_struct *fields;
    struct structures_used_struct *sts;
    struct menu_info_struct *menu_info;
    struct menu_help_info_struct *help_info;
    struct a_menu_struct *old_menu;
};

#endif
