
/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - ansi.h
 *
 */


#ifndef _GTALK_ANSI_H
#define _GTALK_ANSI_H

int wait_ch(void);
int in_char(void);
int ansi_strlen(char *str);
int wait_for_return(void);
void str_cpy(char *to,char *from);
void print_centered(char *str);
void repeat_chr(char chr,int times,char print_a_cr);
int printf_ansi(char *format, ...);
void print_str_cr(char *string);
void print_string(register char *string);
void print_chr(char temp);
int ansi_on(int on);
void wrap_line(char *string);

static void clear_screen(void)
{

  printf("%c",12);  

/**** ANSI way **** 
   send_char(tswitch,27);      
   send_string(tswitch,"[2J");
 */
}
static void position(int y, int x)
 {
   printf("\033[%d;%dH",y,x);       /* Send position codes */
 };

static void foreground(int color)
{
  printf("\033[%dm",color+30);
}

static void background(int color)
{
  printf("\033[%dm",color+40);
}

static void blink_video()
{
  printf("\033[5m");
}

static void bold_video()
{
  printf("\033[1m");
}

static void reset_attributes()
{
  printf("\033[0m");
}

#endif _GTALK_ANSI_H







