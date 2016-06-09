
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

void print_file(char *string);
void set_scrolling_region(int x1,int x2,int portnum);
void erase_region(int x1,int x2,int portnum);

#endif /* GT_OUTPUT_H */
