/******************************

  Miscellaneous routines file

 ******************************/


#ifndef _GTALK_MISC_H
#define _GTALK_MISC_H

int tty_raw(int fd);
int restore_termios(void);
int save_termios(void);

#endif  /* _GTALK_MISC_H */
