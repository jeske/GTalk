/*
 * input.h
 */

#ifndef GT_INPUT_H
#define GT_INPUT_H

#define GI_FLAG_NO_ECHO   (0x01)
#define GI_FLAG_MASK_ECHO (0x02)
#define GI_FLAG_NO_EMPTY  (0x04)
#define GI_FLAG_NO_ESC    (0x08)
#define GIPC_FLAG_DEFAULT (0x10)
#define GI_FLAG_TOUPPER   (0x20)
#define GI_FLAG_NO_ABORT   (0x40)

int get_input_cntrl(char *dest,int len, unsigned long int flags);
int get_input(char *dest,int len);

int get_input_cntrl_pos(char *string, int limit, char echo, char back_to_end,
			char escape, char noblankline, char cr_on_blankline,
			char upcase, char onlynum, int start_pos);
void empty_inbuffer(void);
#endif
