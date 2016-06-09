/*
 * Gtalk/UNIX 
 * Copyright (C) 1995, by David W Jeske, and Daniel L Marks 
 * Copying or distributing this source code without written  
 * permission of David W Jeske and Daniel L Marks is strictly forbidden 
 *
 * - accounting.h
 *
 * This handles the new financial accounting stuff.
 *
 */

#include "comparse.h"

#ifndef GT_ACCOUNTING_H
#define GT_ACCOUNTING_H

void login_accounting_check(struct unique_information_struct *usr,
			struct class_defined_data_struct *usrcls);

int cmd_bank(com_struct *com,char *string);
int cmd_credit(com_struct *com,char *string);

#endif /* GT_ACCOUNTING */
