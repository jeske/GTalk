
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - output.h
 *
 * general output routines
 *
 */

#ifndef GT_OUTPUT_H
#define GT_OUTPUT_H



static char backspace_string[]={8,32,8,0};

/*
 * print_file_cntrl 
 *
 * the associated options are listed below the function
 */
 
int print_file_cntrl(const char *filename,unsigned long int options);
#define PFC_PAUSE   (0x01)
#define PFC_ABORT   (0x02)
#define PFC_PAGING  (0x04)
#define PFC_ANSI    (0x08)


/*
 * print_file
 *
 */

void print_file(char *string);

void print_sys_mesg(char *string);


void erase_region(int x1,int x2);
void set_scrolling_region(int x1,int x2);
void print_string_len(char *string,unsigned char filler,int start,int len);
int print_file_tail_grep(const char *filename,int num_lines, 
			 char *search_string, unsigned long int options);

#endif /* GT_OUTPUT_H */





