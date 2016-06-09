

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */


/* INPUT.C */

#define INCL_DOSPROCESS
#include <os2.h>

#include "include.h"
#include "gtalk.h"
#include "keys.h"

void get_string_echo(char *string, int limit, char echo)
 {
   get_string_cntrl(string,limit,echo,0,0,0,1,0,0);
 };

 /* GET HOT KEY PROMPT

	it will take a list of hotkeys, the DEFAULT hot key, and a 0 or 1
	to indicate whether they should be able to use / commands at it
	*/

int get_hot_key_prompt(char *prompt,char *chars_allowed,char def,char commands)
{
   int key_pr=1;
   int key;
   int action;
   int bkspchr=8;

   special_code(1,tswitch);
   print_string(prompt);
   special_code(0,tswitch);

   print_chr(def);
   print_chr(bkspchr);

   while (key_pr)
	 {
	   key = wait_ch();
	   if (key>'Z') key -= 32;

	   if (key==13 || key==11)
		  key=toupper(def);

	  if (key=='/' && commands)  // if they entered / and they WANT commands
	   {	  /* IF they did /p we need to start getting all the others */
		 char s2[STRING_SIZE];
		 print_chr('/');

		 switch(get_string_cntrl(s2,STRING_SIZE-300,0,1,1,0,0,0,0))
		 {
		   case 0 : action=1;
					break;
		   case 1 : action=2;
					break;
		   case 2 : action=3;
					break;
		   default: action=3;
					break;
		 }

		 switch(action)
		 {
		 case 1:
		  {

			print_cr();

			   if (!test_bit(user_options[tswitch].privs,CAN_TYPE_ANSI_PRV))
				  remove_ansi(s2);
			   else
			   if (!test_bit(user_options[tswitch].privs,CAN_TYPE_FLASHING_PRV))
				  remove_flashing(s2);

			   if (user_options[tswitch].priority)
					  (void)limit_carrots(s2,6);

			switch (exec(s2,line_status[tswitch].handleline,tswitch,1))
			{
				case 0 :  break;
				case 1 :  print_invalid_command();
						  break;
				case 2 :  print_str_cr("Command not available.");
						  break;
			}
			if (test_bit(&line_status[tswitch].handlelinechanged,HANDLELINE_SPRINTF))
			  remake_handleline();

			check_for_privates();
			print_cr();

			special_code(1,tswitch);
			print_string(prompt);
			special_code(0,tswitch);

			print_chr(def);
			print_chr(bkspchr);
		  } break;

			case 2:
		  {   /* OTHERWISE.. just let them go back to typing */
			print_chr(bkspchr);
			print_chr(def);
			print_chr(bkspchr);
		   } break;

		   case 3:
		   {
			 check_for_privates();
			 print_cr();

			 special_code(1,tswitch);
			 print_string(prompt);
			 special_code(0,tswitch);

			 print_chr(def);
			 print_chr(bkspchr);
		   } break;
		  }
	   } // end of stuff for / commands
	   else
	   if (strrchr(chars_allowed,key))
		   key_pr=0;
	 }

   if (key!=def)
	  print_chr(key);

   print_cr();
   return key;
}

/* prompt get string, like get string below, but it takes the
   prompt as an argument so that if someone is going to
   do a /p, it will allocate a big buffer, and let him send it
   right from the prompt, reprinting the prompt when it's done */

void prompt_get_string(const char *prompt,char *string,int limit)
{
   int action;
   int pos = 0;
   int key;
   int flag = 1;

   special_code(1,tswitch);
   print_string(prompt);
   special_code(0,tswitch);

   while (flag) 			/* wait while editing */
	{
	  key = wait_ch();
	  if (((key == 8) || (key == 127)))
	  if (pos>0)
	   {
		 pos--; 		/* if an edit key is pressed and there's more to */
		 print_string(backspacestring);   /* erase, erase the character */
	   }			   /* and go back one */

	  if (key == 27)	/* if we abort, then clear all characters */
	   {
		 if (pos)		/* print a backslash to indicate abort */
		  { flag=0;
			print_chr('\\');
			print_chr(13);
			print_chr(10);
			pos = 0;
		  };
	   };
	  if (key == 13)	/* finish the line */
	   {

		 if (pos)
		  { flag=0;
			print_chr(13);
			print_chr(10);
		  };
	   };
	  if (((key >= 32) && (key <= 126)) && (pos < limit))
	   {						/* insert the character if there's room */
		 *(string+pos) = key;
		 if (key == '+')        /* if +, don't let it be typed normally */
		  {
			print_chr(key); 	/* print the character with a space */
			print_chr(32);		/* and a backspace */
			print_chr(8);
		  }
		  else
		  print_chr(key);		/* otherwise, print it normally */
		 pos++;
	   };

	  if ((pos>0 && *string=='/' ))
	   {	  /* IF they did /p we need to start getting all the others */
		 char s2[STRING_SIZE];

		 switch(get_string_cntrl(s2,STRING_SIZE-300,0,1,1,0,0,0,0))
		 {
		   case 0 : action=1;
					break;
		   case 1 : action=2;
					break;
		   case 2 : action=3;
					break;
		   default: action=3;
					break;
		 }

		 switch(action)
		 {
		 case 1:
		  {

			print_cr();


			   if (!test_bit(user_options[tswitch].privs,CAN_TYPE_ANSI_PRV))
				  remove_ansi(s2);
			   else
			   if (!test_bit(user_options[tswitch].privs,CAN_TYPE_FLASHING_PRV))
				  remove_flashing(s2);

			   if (user_options[tswitch].priority)
					  (void)limit_carrots(s2,6);

			switch (exec(s2,line_status[tswitch].handleline,tswitch,1))
			{
				case 0 :  break;
				case 1 :  print_invalid_command();
						  break;
				case 2 :  print_str_cr("Command not available.");
						  break;
			}
			if (test_bit(&line_status[tswitch].handlelinechanged,HANDLELINE_SPRINTF))
			  remake_handleline();

			check_for_privates();
			print_cr();

			special_code(1,tswitch);
			print_string(prompt);
			special_code(0,tswitch);

			pos=0;
		  } break;
			case 2:
		  {   /* OTHERWISE.. just let them go back to typing */
			pos--;
			print_string(backspacestring);
		   } break;
		   case 3:
		   {
			 check_for_privates();
			 print_cr();

			 special_code(1,tswitch);
			 print_string(prompt);
			 special_code(0,tswitch);

			 pos=0;
		   } break;
		  }
	   }; // end of stuff for / commands

	};	// end WHILE loop

   *(string+pos) = 0;			/* mark end of the string */
   if (!strncmp(string,"NO CARRIER",10)) *string = 0;
   if (!strncmp(string,"CONNECT",7)) *string = 0;
}


/* get a string with editing */
/* string = pointer to where to edit */
/* limit = max number of characters to enter */
void get_string(char *string, int limit)	/* get a string with editing */
 {
   int pos = 0;
   int key;
   int flag = 1;
   while (flag) 			/* wait while editing */
	{
	  key = wait_ch();
	  if (((key == 8) || (key == 127)))
	  if (pos>0)
	   {
		 pos--; 		/* if an edit key is pressed and there's more to */
		 print_string(backspacestring);   /* erase, erase the character */
	   }			   /* and go back one */
	  else
		 flag=0;

	  if (key == 27)	/* if we abort, then clear all characters */
	   {
		 flag = 0;
		 if (pos)		/* print a backslash to indicate abort */
		  {
			print_chr('\\');
			print_chr(13);
			print_chr(10);
			pos = 0;
		  };
	   };
	  if (key == 13)	/* finish the line */
	   {
		 flag = 0;
		 if (pos)
		  {
			print_chr(13);
			print_chr(10);
		  };
	   };
	  if (((key >= 32) && (key <= 126)) && (pos < limit))
	   {						/* insert the character if there's room */
		 *(string+pos) = key;
		 if (key == '+')        /* if +, don't let it be typed normally */
		  {
			print_chr(key); 	/* print the character with a space */
			print_chr(32);		/* and a backspace */
			print_chr(8);
		  }
		  else
		  print_chr(key);		/* otherwise, print it normally */
		 pos++;
	   };
	};
   *(string+pos) = 0;			/* mark end of the string */
   if (!strncmp(string,"NO CARRIER",10)) *string = 0;
   if (!strncmp(string,"CONNECT",7)) *string = 0;
 };

struct vt100_conv_type vt100_key_list[] =
 {
   {  "\033[A", 3,
	  "\033OA", 3,
      PA_UP_ARROW, 72, 0 },
   {  "\033[B", 3,
	  "\033OB", 3,
      PA_DOWN_ARROW, 80, 0 },
   {  "\033[C", 3,
	  "\033OC", 3,
      PA_RIGHT_ARROW, 77, 0 },
   {  "\033[D", 3,
	  "\033OD", 3,
      PA_LEFT_ARROW, 75, 0 },
   {  NULL, 0, NULL, 0, 0, 0, 0 }
 };

int wait_ch(void)					/* wait for a character */
 {
   int portnum = our_task_id();
   int ischar, notisthere = 1;
   int wait_loop;
   struct ln_type *my_line = &line_status[portnum];
   struct vt100_conv_type *vtkeylist;

   flush_output_buffer(tswitch);
   while (notisthere)				  /* is we don't have a character yet */
	{
	  if (!dcd_detect(portnum)) leave();	/* if we lost carrier, log off */
	  ischar = int_char(portnum);			   /* is there a character ready? */
	  if ((ischar == 27) || (my_line->vtkey.num_keycodes))
		{
		  if (my_line->vtkey.num_keycodes == KEYCODE_SIZE_MAX)
		   {
			 my_line->vtkey.num_keycodes = 0;
             continue;
		   } else
			 {
               if ((ischar == 27) && (!my_line->vtkey.num_keycodes))
                 {
                    wait_loop=0;
                    while ((wait_loop++<6) && !char_in_buf(portnum))
                    {
                      DosSleep(10l);
                    }

                    if (!char_in_buf(portnum))
                       return (ischar);
                 }
			   my_line->vtkey.keycodes[my_line->vtkey.num_keycodes++]
							= ischar;
			   my_line->vtkey.keycodes[my_line->vtkey.num_keycodes]
							= '\000';

			   for (vtkeylist=vt100_key_list;vtkeylist->vt100_key_string;
					vtkeylist++)
				{
				  if ((!memcmp(vtkeylist->vt100_key_string,
					   my_line->vtkey.keycodes,
					   vtkeylist->key_code_len) ||
					   (!memcmp(vtkeylist->vt100_alternate_string,
					   my_line->vtkey.keycodes,
					   vtkeylist->alt_code_len))))
                   {
					 my_line->vtkey.num_keycodes = 0;
					 return (vtkeylist->keycode);
				   }
				}
               continue;
			}
		}
	  if (ischar == -1)
         {
             task_sleep();
		 }
	   else
		 notisthere = 0;
	};
   return(ischar);					/* return the character */
 };


/* get a string with editing and command control */
/* string = pointer to where to edit */
/* limit = max number of characters to enter */


int get_string_cntrl_pos(char *string, int limit, char echo, char back_to_end,
					  char escape, char noblankline, char cr_on_blankline,
					  char upcase, char onlynum, int start_pos)
 {
   int pos = start_pos;
   int key;
   int flag = 1;
   int reason=0;

   while (flag) 			/* wait while editing */
	{
	  key = wait_ch();
	  if (((key == 8) || (key == 127)))
	  if (pos>0)
	   {
		 pos--; 		/* if an edit key is pressed and there's more to */
		 print_string(backspacestring);   /* erase, erase the character */
	   }			   /* and go back one */
	  else
	   if (back_to_end) { flag = 0; reason=1; }

	  if ((key == 27) && escape)  /* if we abort, then clear all characters */
	   {
		 flag = 0;
		 reason=2;
		 if (pos)		/* print a backslash to indicate abort */
		  {
			print_chr('\\');
			print_chr(13);
			print_chr(10);
			pos = 0;
		  };
	   };
	  if (key == 13)	/* finish the line */
	   {
		 if ((!noblankline) || (pos))
		  {
			flag = 0;
			if ((pos) || (cr_on_blankline))
			 {
			  print_chr(13);
			  print_chr(10);
			 };
		  };
	   };
	  if (((key >= 32) && (key <= 126)) && (pos < limit))
	   {
		 if ((upcase) && (key>='a') && (key<='z')) key -= 32;
		 if (!((onlynum) && ((key<'0') || (key>'9'))))
		  {
								/* insert the character if there's room */
		   *(string+pos) = key;
		   if (key == '+')        /* if +, don't let it be typed normally */
			{
			  print_chr(key);	  /* print the character with a space */
			  print_chr(32);	  /* and a backspace */
			  print_chr(8);
			}
			else
			if (echo) print_chr(echo);
			 else print_chr(key);		/* otherwise, print it normally */
		   pos++;
		  };
	   };
	};
   *(string+pos) = 0;			/* mark end of the string */
   return reason;
 };

int get_string_cntrl(char *string, int limit, char echo, char back_to_end,
					  char escape, char noblankline, char cr_on_blankline,
					  char upcase, char onlynum)
{
return (get_string_cntrl_pos(string,limit, echo, back_to_end,
					  escape, noblankline, cr_on_blankline,
					  upcase, onlynum, 0));
}

void check_for_privates(void)
 {
   int not_print_priv = 1;
   int channel, id, type, code1, code2,code3;
   int print_mesg;
   char s[STRING_SIZE];
   while (aget_abuffer(&id,&channel,s,&type,&code1,&code2,&code3))
	{
	   print_mesg=0;

	   switch (type)
	   {
	   case 1 :  print_mesg++;	/* privates */
				 break;
	   case 5 :  if (code2==7) print_mesg++;   /* system messsage */
				 break;
	   case 7 :  print_mesg++;	/* user alert mesg / timeout */
				 break;
	   case 8 :  if (code3!=8)
					print_mesg++;  /* personal system message */
				 break;
	   case 10:  print_mesg++;	/* link privates */
				 break;
	   case 12:  print_mesg++;
				 break;
	  }

	  if (print_mesg)
	   {
	   if (not_print_priv)
		  {
			not_print_priv = 0;
			print_chr(7);
			print_cr();
		  };

		 wrap_line(s);
	   }
	}
 };

/* get a string with no echo or editing */
void get_no_echo(char *string, int limit)
 {
   char ch;
   while (limit>0)
	{
	  ch = wait_ch();
	  if (ch == 13) limit=0;
	   else if ((ch>=32) && (ch<=126) && (limit>1))
		{
		  *string++ = ch;
		  limit--;
		};
	};
   *string=0;
 };

