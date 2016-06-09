/*
 * input.h
 */

#ifndef GT_INPUT_H
#define GT_INPUT_H

#define GI_FLAG_NO_ECHO   (0x01)
#define GI_FLAG_MASK_ECHO (0x02)

int get_string(char *dest,int len, unsigned long int flags);

#endif
