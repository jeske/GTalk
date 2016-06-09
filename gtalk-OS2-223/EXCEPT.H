
/* EXCEPT.H */

#ifndef GT_EXCEPT_H
#define GT_EXCEPT_H

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


int init_except(void);

ULONG APIENTRY ExceptionHandler (PEXCEPTIONREPORTRECORD pERepRec,
                      PEXCEPTIONREGISTRATIONRECORD pERegRec,
                      PCONTEXTRECORD pCtxRec,
                      PVOID p);


#define GT_XCPT_INVALID_TID_LOOKUP   0xC0000100L

#endif
