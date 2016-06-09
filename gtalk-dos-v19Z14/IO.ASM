	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif
	?debug	V 300h
	?debug	S "io.c"
	?debug	C E9CD506A1B04696F2E63
	?debug	C E9317A641B09696E636C7564652E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C737464696F2E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C5F646566732E68
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C5F6E66696C652E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C5F6E756C6C2E68
	?debug	C E94019CA181E433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C646F732E68
	?debug	C E94019CA181F433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C62696F732E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C616C6C6F632E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C636F6E696F2E68
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C7374646C69622E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C63747970652E68
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C737472696E672E68
	?debug	C E94019CA181F433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C74696D652E68
	?debug	C E94019CA181E433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C6D656D2E68
	?debug	C E94019CA181E433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C6469722E68
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C7374646172672E68
	?debug	C E9866B691B076774616C6B2E68
	?debug	C E9CB70691B08646566696E652E68
	?debug	C E9732BE71A0A66756E6374696F6E2E68
	?debug	C E92B7A691B067461736B2E68
	?debug	C E9E60A931A06696E69742E68
	?debug	C E9AB78631B07616C6C6F632E68
	?debug	C E9259AE11A05636F6D2E68
	?debug	C E9AA45681B07766964656F2E68
	?debug	C E9A675641B096368616E6E656C2E68
	?debug	C E9B67B641B04696F2E68
	?debug	C E9EBAC651B0467742E68
	?debug	C E97074641B06677473742E68
	?debug	C E98B74641B06757365722E68
	?debug	C E9CA76641B087573657273742E68
	?debug	C E9DCAEF51A09636F6D6D616E642E68
	?debug	C E9C3793B1A066C696E6B2E68
	?debug	C E9FB9D921A07676C696E6B2E68
	?debug	C E9C2793B1A09737472756374732E68
	?debug	C E944A7951A08737472696E672E68
	?debug	C E91251FD1A0770726976732E68
	?debug	C E9EB64691B09746F67676C65732E68
	?debug	C E9C4793B1A07696E7075742E68
	?debug	C E91B82891A08656469746F722E68
	?debug	C E90574581B066D61696C2E68
	?debug	C E97AA69A1A09726F7461746F722E68
	?debug	C E9C5793B1A086766696C65732E68
	?debug	C E994913C1A0A7363686564756C652E68
	?debug	C E9C2793B1A077379736F702E68
	?debug	C E9C7B5331B056262732E68
	?debug	C E98D8B931A0764696167732E68
	?debug	C E9C1793B1A0A70726F746F636F6C2E68
	?debug	C E94019CA181F433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C74696D652E68
_TEXT	segment byte public 'CODE'
_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_TEXT	segment byte public 'CODE'
   ;	
   ;	unsigned int convstring(char *string, int encrypt)
   ;	
	assume	cs:_TEXT
_convstring	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	    char prev = 0x05;
   ;	
	mov	byte ptr [bp-1],5
   ;	
   ;	    char temp;
   ;	    unsigned int checksum = 0;
   ;	
	xor	cx,cx
	jmp	short @1@198
@1@58:
   ;	
   ;	
   ;	    while (*string)
   ;	    {
   ;	       if (encrypt)
   ;	
	cmp	word ptr [bp+6],0
	je	short @1@114
   ;	
   ;	       {
   ;	           checksum += (unsigned int) *string;
   ;	
	mov	al,byte ptr [si]
	cbw	
	add	cx,ax
   ;	
   ;	           prev = *string = circleplus(*string,prev);
   ;	
	mov	al,byte ptr [si]
	mov	dl,byte ptr [bp-1]
	and	dl,7
	xor	al,dl
	mov	byte ptr [si],al
	jmp	short @1@142
@1@114:
   ;	
   ;	       }
   ;	       else
   ;	       {
   ;	           temp = *string;
   ;	
	mov	al,byte ptr [si]
	mov	byte ptr [bp-2],al
   ;	
   ;	           *string = circleplus(*string,prev);
   ;	
	mov	al,byte ptr [si]
	mov	dl,byte ptr [bp-1]
	and	dl,7
	xor	al,dl
	mov	byte ptr [si],al
   ;	
   ;	           checksum += (unsigned int) *string;
   ;	
	mov	al,byte ptr [si]
	cbw	
	add	cx,ax
   ;	
   ;	           prev = temp;
   ;	
	mov	al,byte ptr [bp-2]
@1@142:
	mov	byte ptr [bp-1],al
   ;	
   ;	       };
   ;	       string++;
   ;	
	inc	si
@1@198:
	cmp	byte ptr [si],0
	jne	short @1@58
   ;	
   ;	    };
   ;	    return(checksum);
   ;	
	mov	ax,cx
	jmp	short @1@254
@1@254:
   ;	
   ;	};
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_convstring	endp
   ;	
   ;	int wait_ch(void)                   /* wait for a character */
   ;	
	assume	cs:_TEXT
_wait_ch	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
   ;	
   ;	 {
   ;	   int portnum = our_task_id();
   ;	
	mov	si,word ptr DGROUP:_tswitch
   ;	
   ;	   int ischar, notisthere = 1;
   ;	
	mov	word ptr [bp-2],1
	jmp	short @2@198
@2@58:
   ;	
   ;	
   ;	   while (notisthere)                 /* is we don't have a character yet */
   ;	    {
   ;	      if (!dcd_detect(portnum)) leave();    /* if we lost carrier, log off */
   ;	
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_dcd_detect[bx]
	pop	cx
	or	ax,ax
	jne	short @2@114
	call	near ptr _leave
@2@114:
   ;	
   ;	      ischar=int_char(portnum);              /* is there a character ready? */
   ;	
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_get_nchar[bx]
	pop	cx
	mov	di,ax
   ;	
   ;	      if (ischar == -1) next_task();            /* if no key, next_task */
   ;	
	cmp	di,-1
	jne	short @2@170
	pushf	
	call	far ptr _next_task
	jmp	short @2@198
@2@170:
   ;	
   ;	       else notisthere = 0;
   ;	
	mov	word ptr [bp-2],0
@2@198:
	cmp	word ptr [bp-2],0
	jne	short @2@58
   ;	
   ;	    };
   ;	   return(ischar);                  /* return the character */
   ;	
	mov	ax,di
	jmp	short @2@254
@2@254:
   ;	
   ;	 };
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_wait_ch	endp
   ;	
   ;	int do_page_break(void)
   ;	
	assume	cs:_TEXT
_do_page_break	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
   ;	
   ;	{
   ;	  int count;
   ;	  int abort=0;
   ;	
	xor	di,di
   ;	
   ;	
   ;	    print_string("[ Press Return ]");
   ;	
	mov	ax,offset DGROUP:s@
	push	ax
	call	near ptr _print_string
	pop	cx
@3@58:
   ;	
   ;	    do
   ;	     {
   ;	       count = wait_ch();
   ;	
	call	near ptr _wait_ch
	mov	si,ax
   ;	
   ;	       if ((count == 27) || (count == 3))
   ;	
	cmp	si,27
	je	short @3@114
	cmp	si,3
	jne	short @3@142
@3@114:
   ;	
   ;	        {
   ;	         count = 13;
   ;	
	mov	si,13
   ;	
   ;	         abort=1;
   ;	
	mov	di,1
@3@142:
   ;	
   ;	       };
   ;	     } while (count != 13);
   ;	
	cmp	si,13
	jne	short @3@58
   ;	
   ;	
   ;	    for (count=0;count<16;count++)
   ;	
	xor	si,si
	jmp	short @3@254
@3@198:
   ;	
   ;	      print_string(backspacestring);
   ;	
	mov	ax,offset DGROUP:_backspacestring
	push	ax
	call	near ptr _print_string
	pop	cx
	inc	si
@3@254:
	cmp	si,16
	jl	short @3@198
   ;	
   ;	
   ;	  return (abort);
   ;	
	mov	ax,di
	jmp	short @3@310
@3@310:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_do_page_break	endp
   ;	
   ;	void get_no_echo(char *string, int limit)
   ;	
	assume	cs:_TEXT
_get_no_echo	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
	mov	si,word ptr [bp+4]
	mov	di,word ptr [bp+6]
	jmp	short @4@226
@4@58:
   ;	
   ;	 {
   ;	   char ch;
   ;	   while (limit>0)
   ;	    {
   ;	      ch = wait_ch();
   ;	
	call	near ptr _wait_ch
	mov	byte ptr [bp-1],al
   ;	
   ;	      if (ch == 13) limit=0;
   ;	
	cmp	byte ptr [bp-1],13
	jne	short @4@114
	xor	di,di
	jmp	short @4@226
@4@114:
   ;	
   ;	       else if ((ch>=32) && (ch<=126) && (limit>1))
   ;	
	cmp	byte ptr [bp-1],32
	jl	short @4@226
	cmp	byte ptr [bp-1],126
	jg	short @4@226
	cmp	di,1
	jle	short @4@226
   ;	
   ;	        {
   ;	          *string++ = ch;
   ;	
	mov	al,byte ptr [bp-1]
	mov	byte ptr [si],al
	inc	si
   ;	
   ;	          limit--;
   ;	
	dec	di
@4@226:
	or	di,di
	jg	short @4@58
   ;	
   ;	        };
   ;	    };
   ;	   *string=0;
   ;	
	mov	byte ptr [si],0
   ;	
   ;	 };
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_get_no_echo	endp
   ;	
   ;	int get_hot_key_prompt(char *prompt,char *chars_allowed,char def,char commands)
   ;	
	assume	cs:_TEXT
_get_hot_key_prompt	proc	near
	push	bp
	mov	bp,sp
	sub	sp,456
	push	si
	push	di
	mov	di,word ptr [bp+4]
   ;	
   ;	{
   ;	   int key_pr=1;
   ;	
	mov	word ptr [bp-2],1
   ;	
   ;	   int key;
   ;	   int action;
   ;	   int bkspchr=8;
   ;	
	mov	word ptr [bp-6],8
   ;	
   ;	
   ;	   special_code(1,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	ax,1
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
   ;	
   ;	   print_string(prompt);
   ;	
	push	di
	call	near ptr _print_string
	pop	cx
   ;	
   ;	   special_code(0,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	xor	ax,ax
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
   ;	
   ;	
   ;	   print_chr(def);
   ;	
	mov	al,byte ptr [bp+8]
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	   print_chr(bkspchr);
   ;	
	mov	al,byte ptr [bp-6]
	push	ax
	call	near ptr _print_chr
	pop	cx
	jmp	@5@1318
@5@58:
   ;	
   ;	
   ;	   while (key_pr)
   ;	     {
   ;	       key = wait_ch();
   ;	
	call	near ptr _wait_ch
	mov	word ptr [bp-4],ax
   ;	
   ;	       if (key>'Z') key -= 32;
   ;	
	cmp	word ptr [bp-4],90
	jle	short @5@114
	sub	word ptr [bp-4],32
@5@114:
   ;	
   ;	
   ;	       if (key==13 || key==11)
   ;	
	cmp	word ptr [bp-4],13
	je	short @5@170
	cmp	word ptr [bp-4],11
	jne	short @5@198
@5@170:
   ;	
   ;	          key=toupper(def);
   ;	
	mov	al,byte ptr [bp+8]
	cbw	
	push	ax
	call	near ptr _toupper
	pop	cx
	mov	word ptr [bp-4],ax
@5@198:
   ;	
   ;	
   ;	      if (key=='/' && commands)  // if they entered / and they WANT commands
   ;	
	cmp	word ptr [bp-4],47
	je	@@0
	jmp	@5@1262
@@0:
	cmp	byte ptr [bp+10],0
	jne	@@1
	jmp	@5@1262
@@1:
   ;	
   ;	       {      /* IF they did /p we need to start getting all the others */
   ;	         char s2[STRING_SIZE];
   ;	         print_chr('/');
   ;	
	mov	al,47
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	
   ;	         switch(get_string_cntrl(s2,STRING_SIZE-300,0,1,1,0,0,0,0))
   ;	
	mov	al,0
	push	ax
	mov	al,0
	push	ax
	mov	al,0
	push	ax
	mov	al,0
	push	ax
	mov	al,1
	push	ax
	mov	al,1
	push	ax
	mov	al,0
	push	ax
	mov	ax,150
	push	ax
	lea	ax,word ptr [bp-456]
	push	ax
	call	near ptr _get_string_cntrl
	add	sp,18
	or	ax,ax
	je	short @5@422
	cmp	ax,1
	je	short @5@450
	cmp	ax,2
	je	short @5@478
	jmp	short @5@506
@5@422:
   ;	
   ;	         {
   ;	           case 0 : action=1;
   ;	
	mov	si,1
   ;	
   ;	                    break;
   ;	
	jmp	short @5@534
@5@450:
   ;	
   ;	           case 1 : action=2;
   ;	
	mov	si,2
   ;	
   ;	                    break;
   ;	
	jmp	short @5@534
@5@478:
	jmp	short @5@506
@5@506:
   ;	
   ;	           case 2 : action=3;
   ;	                    break;
   ;	           default: action=3;
   ;	
	mov	si,3
   ;	
   ;	                    break;
   ;	
	jmp	short @5@534
@5@534:
   ;	
   ;	         }
   ;	
   ;	         switch(action)
   ;	
	mov	ax,si
	cmp	ax,1
	je	short @5@674
	cmp	ax,2
	jne	@@2
	jmp	@5@1178
@@2:
	cmp	ax,3
	jne	@@3
	jmp	@5@1206
@@3:
	jmp	@5@1234
@5@674:
   ;	
   ;	         {
   ;	         case 1:
   ;	          {
   ;	
   ;	            print_cr();
   ;	
	call	near ptr _print_cr
   ;	
   ;	
   ;	               if (!test_bit(user_options[tswitch].privs,CAN_TYPE_ANSI_PRV))
   ;	
	mov	ax,39
	push	ax
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,178
	imul	dx
	add	ax,offset _user_options+2
	push	ax
	call	near ptr _test_bit
	pop	cx
	pop	cx
	or	ax,ax
	jne	short @5@730
   ;	
   ;	                  remove_ansi(s2);
   ;	
	lea	ax,word ptr [bp-456]
	push	ax
	call	near ptr _remove_ansi
	pop	cx
	jmp	short @5@786
@5@730:
   ;	
   ;	               else
   ;	               if (!test_bit(user_options[tswitch].privs,CAN_TYPE_FLASHING_PRV))
   ;	
	mov	ax,40
	push	ax
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,178
	imul	dx
	add	ax,offset _user_options+2
	push	ax
	call	near ptr _test_bit
	pop	cx
	pop	cx
	or	ax,ax
	jne	short @5@786
   ;	
   ;	                  remove_flashing(s2);
   ;	
	lea	ax,word ptr [bp-456]
	push	ax
	call	near ptr _remove_flashing
	pop	cx
@5@786:
   ;	
   ;	
   ;	               if (user_options[tswitch].priority)
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,178
	imul	dx
	mov	dx,seg _user_options
	mov	bx,ax
	mov	es,dx
	cmp	word ptr es:_user_options[bx+32],0
	je	short @5@842
   ;	
   ;	                      (void)limit_carrots(s2,6);
   ;	
	mov	ax,6
	push	ax
	lea	ax,word ptr [bp-456]
	push	ax
	call	near ptr _limit_carrots
	pop	cx
	pop	cx
@5@842:
   ;	
   ;	
   ;	            switch (exec(s2,line_status[tswitch].handleline,tswitch,1))
   ;	
	mov	al,1
	push	ax
	push	word ptr DGROUP:_tswitch
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,187
	imul	dx
	add	ax,offset DGROUP:_line_status+43
	push	ax
	lea	ax,word ptr [bp-456]
	push	ax
	call	near ptr _exec
	add	sp,8
	or	ax,ax
	je	short @5@982
	cmp	ax,1
	je	short @5@1010
	cmp	ax,2
	je	short @5@1038
	jmp	short @5@1066
@5@982:
   ;	
   ;	            {
   ;	                case 0 :  break;
   ;	
	jmp	short @5@1066
@5@1010:
   ;	
   ;	                case 1 :  print_invalid_command();
   ;	
	call	near ptr _print_invalid_command
   ;	
   ;	                          break;
   ;	
	jmp	short @5@1066
@5@1038:
   ;	
   ;	                case 2 :  print_str_cr("Command not available.");
   ;	
	mov	ax,offset DGROUP:s@+17
	push	ax
	call	near ptr _print_str_cr
	pop	cx
   ;	
   ;	                          break;
   ;	
	jmp	short @5@1066
@5@1066:
   ;	
   ;	            }
   ;	            if (test_bit(&line_status[tswitch].handlelinechanged,HANDLELINE_SPRINTF))
   ;	
	xor	ax,ax
	push	ax
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,187
	imul	dx
	add	ax,offset DGROUP:_line_status+42
	push	ax
	call	near ptr _test_bit
	pop	cx
	pop	cx
	or	ax,ax
	je	short @5@1122
   ;	
   ;	              remake_handleline();
   ;	
	call	near ptr _remake_handleline
@5@1122:
   ;	
   ;	
   ;	            check_for_privates();
   ;	
	call	near ptr _check_for_privates
   ;	
   ;	            print_cr();
   ;	
	call	near ptr _print_cr
   ;	
   ;	
   ;	            special_code(1,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	ax,1
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
   ;	
   ;	            print_string(prompt);
   ;	
	push	di
	call	near ptr _print_string
	pop	cx
   ;	
   ;	            special_code(0,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	xor	ax,ax
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
@5@1150:
   ;	
   ;	
   ;	            print_chr(def);
   ;	
	mov	al,byte ptr [bp+8]
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            print_chr(bkspchr);
   ;	
	mov	al,byte ptr [bp-6]
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	          } break;
   ;	
	jmp	short @5@1234
@5@1178:
   ;	
   ;	
   ;	            case 2:
   ;	          {   /* OTHERWISE.. just let them go back to typing */
   ;	            print_chr(bkspchr);
   ;	
	mov	al,byte ptr [bp-6]
	push	ax
	call	near ptr _print_chr
	pop	cx
	jmp	short @5@1150
@5@1206:
	jmp	short @5@1122
@5@1234:
   ;	
   ;	            print_chr(def);
   ;	            print_chr(bkspchr);
   ;	           } break;
   ;	
   ;	           case 3:
   ;	           {
   ;	             check_for_privates();
   ;	             print_cr();
   ;	
   ;	             special_code(1,tswitch);
   ;	             print_string(prompt);
   ;	             special_code(0,tswitch);
   ;	
   ;	             print_chr(def);
   ;	             print_chr(bkspchr);
   ;	           } break;
   ;	          }
   ;	       } // end of stuff for / commands
   ;	
	jmp	short @5@1318
@5@1262:
   ;	
   ;	       else
   ;	       if (strrchr(chars_allowed,key))
   ;	
	push	word ptr [bp-4]
	push	word ptr [bp+6]
	call	near ptr _strrchr
	pop	cx
	pop	cx
	or	ax,ax
	je	short @5@1318
   ;	
   ;	           key_pr=0;
   ;	
	mov	word ptr [bp-2],0
@5@1318:
	cmp	word ptr [bp-2],0
	je	@@4
	jmp	@5@58
@@4:
   ;	
   ;	     }
   ;	
   ;	   if (key!=def)
   ;	
	mov	al,byte ptr [bp+8]
	cbw	
	cmp	ax,word ptr [bp-4]
	je	short @5@1402
   ;	
   ;	      print_chr(key);
   ;	
	mov	al,byte ptr [bp-4]
	push	ax
	call	near ptr _print_chr
	pop	cx
@5@1402:
   ;	
   ;	
   ;	   print_cr();
   ;	
	call	near ptr _print_cr
   ;	
   ;	   return key;
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @5@1430
@5@1430:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_get_hot_key_prompt	endp
   ;	
   ;	void prompt_get_string(const char *prompt,char *string,int limit)
   ;	
	assume	cs:_TEXT
_prompt_get_string	proc	near
	push	bp
	mov	bp,sp
	sub	sp,456
	push	si
	push	di
	mov	di,word ptr [bp+6]
   ;	
   ;	{
   ;	   int action;
   ;	   int pos = 0;
   ;	
	xor	si,si
   ;	
   ;	   int key;
   ;	   int flag = 1;
   ;	
	mov	word ptr [bp-6],1
   ;	
   ;	
   ;	   special_code(1,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	ax,1
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
   ;	
   ;	   print_string(prompt);
   ;	
	push	word ptr [bp+4]
	call	near ptr _print_string
	pop	cx
   ;	
   ;	   special_code(0,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	xor	ax,ax
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
	jmp	@6@1542
@6@58:
   ;	
   ;	
   ;	   while (flag)             /* wait while editing */
   ;	    {
   ;	      key = wait_ch();
   ;	
	call	near ptr _wait_ch
	mov	word ptr [bp-4],ax
   ;	
   ;	      if (((key == 8) || (key == 127)))
   ;	
	cmp	word ptr [bp-4],8
	je	short @6@114
	cmp	word ptr [bp-4],127
	jne	short @6@170
@6@114:
   ;	
   ;	      if (pos>0)
   ;	
	or	si,si
	jle	short @6@170
   ;	
   ;	       {
   ;	         pos--;         /* if an edit key is pressed and there's more to */
   ;	
	dec	si
   ;	
   ;	         print_string(backspacestring);   /* erase, erase the character */
   ;	
	mov	ax,offset DGROUP:_backspacestring
	push	ax
	call	near ptr _print_string
	pop	cx
@6@170:
   ;	
   ;	       }               /* and go back one */
   ;	
   ;	      if (key == 27)    /* if we abort, then clear all characters */
   ;	
	cmp	word ptr [bp-4],27
	jne	short @6@254
   ;	
   ;	       {
   ;	         if (pos)       /* print a backslash to indicate abort */
   ;	
	or	si,si
	je	short @6@254
   ;	
   ;	          { flag=0;
   ;	
	mov	word ptr [bp-6],0
   ;	
   ;	            print_chr('\\');
   ;	
	mov	al,92
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            print_chr(13);
   ;	
	mov	al,13
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            print_chr(10);
   ;	
	mov	al,10
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            pos = 0;
   ;	
	xor	si,si
@6@254:
   ;	
   ;	          };
   ;	       };
   ;	      if (key == 13)    /* finish the line */
   ;	
	cmp	word ptr [bp-4],13
	jne	short @6@338
   ;	
   ;	       {
   ;	
   ;	         if (pos)
   ;	
	or	si,si
	je	short @6@338
   ;	
   ;	          { flag=0;
   ;	
	mov	word ptr [bp-6],0
   ;	
   ;	            print_chr(13);
   ;	
	mov	al,13
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            print_chr(10);
   ;	
	mov	al,10
	push	ax
	call	near ptr _print_chr
	pop	cx
@6@338:
   ;	
   ;	          };
   ;	       };
   ;	      if (((key >= 32) && (key <= 126)) && (pos < limit))
   ;	
	cmp	word ptr [bp-4],32
	jl	short @6@562
	cmp	word ptr [bp-4],126
	jg	short @6@562
	cmp	si,word ptr [bp+8]
	jge	short @6@562
   ;	
   ;	       {                        /* insert the character if there's room */
   ;	         *(string+pos) = key;
   ;	
	mov	bx,si
	mov	al,byte ptr [bp-4]
	mov	byte ptr [bx+di],al
   ;	
   ;	         if (key == '+')        /* if +, don't let it be typed normally */
   ;	
	cmp	word ptr [bp-4],43
	jne	short @6@478
   ;	
   ;	          {
   ;	            print_chr(key);     /* print the character with a space */
   ;	
	mov	al,byte ptr [bp-4]
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            print_chr(32);      /* and a backspace */
   ;	
	mov	al,32
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            print_chr(8);
   ;	
	mov	al,8
	jmp	short @6@506
@6@478:
   ;	
   ;	          }
   ;	          else
   ;	          print_chr(key);       /* otherwise, print it normally */
   ;	
	mov	al,byte ptr [bp-4]
@6@506:
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	         pos++;
   ;	
	inc	si
@6@562:
   ;	
   ;	       };
   ;	
   ;	      if ((pos>0 && *string=='/' ))
   ;	
	or	si,si
	jg	@@5
	jmp	@6@1542
@@5:
	cmp	byte ptr [di],47
	je	@@6
	jmp	@6@1542
@@6:
   ;	
   ;	       {      /* IF they did /p we need to start getting all the others */
   ;	         char s2[STRING_SIZE];
   ;	
   ;	         switch(get_string_cntrl(s2,STRING_SIZE-300,0,1,1,0,0,0,0))
   ;	
	mov	al,0
	push	ax
	mov	al,0
	push	ax
	mov	al,0
	push	ax
	mov	al,0
	push	ax
	mov	al,1
	push	ax
	mov	al,1
	push	ax
	mov	al,0
	push	ax
	mov	ax,150
	push	ax
	lea	ax,word ptr [bp-456]
	push	ax
	call	near ptr _get_string_cntrl
	add	sp,18
	or	ax,ax
	je	short @6@758
	cmp	ax,1
	je	short @6@786
	cmp	ax,2
	je	short @6@814
	jmp	short @6@842
@6@758:
   ;	
   ;	         {
   ;	           case 0 : action=1;
   ;	
	mov	word ptr [bp-2],1
   ;	
   ;	                    break;
   ;	
	jmp	short @6@870
@6@786:
   ;	
   ;	           case 1 : action=2;
   ;	
	mov	word ptr [bp-2],2
   ;	
   ;	                    break;
   ;	
	jmp	short @6@870
@6@814:
	jmp	short @6@842
@6@842:
   ;	
   ;	           case 2 : action=3;
   ;	                    break;
   ;	           default: action=3;
   ;	
	mov	word ptr [bp-2],3
   ;	
   ;	                    break;
   ;	
	jmp	short @6@870
@6@870:
   ;	
   ;	         }
   ;	
   ;	         switch(action)
   ;	
	mov	ax,word ptr [bp-2]
	cmp	ax,1
	je	short @6@1010
	cmp	ax,2
	jne	@@7
	jmp	@6@1486
@@7:
	cmp	ax,3
	jne	@@8
	jmp	@6@1514
@@8:
	jmp	@6@1542
@6@1010:
   ;	
   ;	         {
   ;	         case 1:
   ;	          {
   ;	
   ;	            print_cr();
   ;	
	call	near ptr _print_cr
   ;	
   ;	
   ;	
   ;	               if (!test_bit(user_options[tswitch].privs,CAN_TYPE_ANSI_PRV))
   ;	
	mov	ax,39
	push	ax
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,178
	imul	dx
	add	ax,offset _user_options+2
	push	ax
	call	near ptr _test_bit
	pop	cx
	pop	cx
	or	ax,ax
	jne	short @6@1066
   ;	
   ;	                  remove_ansi(s2);
   ;	
	lea	ax,word ptr [bp-456]
	push	ax
	call	near ptr _remove_ansi
	pop	cx
	jmp	short @6@1122
@6@1066:
   ;	
   ;	               else
   ;	               if (!test_bit(user_options[tswitch].privs,CAN_TYPE_FLASHING_PRV))
   ;	
	mov	ax,40
	push	ax
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,178
	imul	dx
	add	ax,offset _user_options+2
	push	ax
	call	near ptr _test_bit
	pop	cx
	pop	cx
	or	ax,ax
	jne	short @6@1122
   ;	
   ;	                  remove_flashing(s2);
   ;	
	lea	ax,word ptr [bp-456]
	push	ax
	call	near ptr _remove_flashing
	pop	cx
@6@1122:
   ;	
   ;	
   ;	               if (user_options[tswitch].priority)
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,178
	imul	dx
	mov	dx,seg _user_options
	mov	bx,ax
	mov	es,dx
	cmp	word ptr es:_user_options[bx+32],0
	je	short @6@1178
   ;	
   ;	                      (void)limit_carrots(s2,6);
   ;	
	mov	ax,6
	push	ax
	lea	ax,word ptr [bp-456]
	push	ax
	call	near ptr _limit_carrots
	pop	cx
	pop	cx
@6@1178:
   ;	
   ;	
   ;	            switch (exec(s2,line_status[tswitch].handleline,tswitch,1))
   ;	
	mov	al,1
	push	ax
	push	word ptr DGROUP:_tswitch
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,187
	imul	dx
	add	ax,offset DGROUP:_line_status+43
	push	ax
	lea	ax,word ptr [bp-456]
	push	ax
	call	near ptr _exec
	add	sp,8
	or	ax,ax
	je	short @6@1318
	cmp	ax,1
	je	short @6@1346
	cmp	ax,2
	je	short @6@1374
	jmp	short @6@1402
@6@1318:
   ;	
   ;	            {
   ;	                case 0 :  break;
   ;	
	jmp	short @6@1402
@6@1346:
   ;	
   ;	                case 1 :  print_invalid_command();
   ;	
	call	near ptr _print_invalid_command
   ;	
   ;	                          break;
   ;	
	jmp	short @6@1402
@6@1374:
   ;	
   ;	                case 2 :  print_str_cr("Command not available.");
   ;	
	mov	ax,offset DGROUP:s@+40
	push	ax
	call	near ptr _print_str_cr
	pop	cx
   ;	
   ;	                          break;
   ;	
	jmp	short @6@1402
@6@1402:
   ;	
   ;	            }
   ;	            if (test_bit(&line_status[tswitch].handlelinechanged,HANDLELINE_SPRINTF))
   ;	
	xor	ax,ax
	push	ax
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,187
	imul	dx
	add	ax,offset DGROUP:_line_status+42
	push	ax
	call	near ptr _test_bit
	pop	cx
	pop	cx
	or	ax,ax
	je	short @6@1458
   ;	
   ;	              remake_handleline();
   ;	
	call	near ptr _remake_handleline
@6@1458:
   ;	
   ;	
   ;	            check_for_privates();
   ;	
	call	near ptr _check_for_privates
   ;	
   ;	            print_cr();
   ;	
	call	near ptr _print_cr
   ;	
   ;	
   ;	            special_code(1,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	ax,1
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
   ;	
   ;	            print_string(prompt);
   ;	
	push	word ptr [bp+4]
	call	near ptr _print_string
	pop	cx
   ;	
   ;	            special_code(0,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	xor	ax,ax
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
   ;	
   ;	
   ;	            pos=0;
   ;	
	xor	si,si
   ;	
   ;	          } break;
   ;	
	jmp	short @6@1542
@6@1486:
   ;	
   ;	            case 2:
   ;	          {   /* OTHERWISE.. just let them go back to typing */
   ;	            pos--;
   ;	
	dec	si
   ;	
   ;	            print_string(backspacestring);
   ;	
	mov	ax,offset DGROUP:_backspacestring
	push	ax
	call	near ptr _print_string
	pop	cx
   ;	
   ;	           } break;
   ;	
	jmp	short @6@1542
@6@1514:
	jmp	short @6@1458
@6@1542:
	cmp	word ptr [bp-6],0
	je	@@9
	jmp	@6@58
@@9:
   ;	
   ;	           case 3:
   ;	           {
   ;	             check_for_privates();
   ;	             print_cr();
   ;	
   ;	             special_code(1,tswitch);
   ;	             print_string(prompt);
   ;	             special_code(0,tswitch);
   ;	
   ;	             pos=0;
   ;	           } break;
   ;	          }
   ;	       }; // end of stuff for / commands
   ;	
   ;	    };  // end WHILE loop
   ;	
   ;	   *(string+pos) = 0;           /* mark end of the string */
   ;	
	mov	bx,si
	mov	byte ptr [bx+di],0
   ;	
   ;	   if (!strncmp(string,"NO CARRIER",10)) *string = 0;
   ;	
	mov	ax,10
	push	ax
	mov	ax,offset DGROUP:s@+63
	push	ax
	push	di
	call	near ptr _strncmp
	add	sp,6
	or	ax,ax
	jne	short @6@1626
	mov	byte ptr [di],0
@6@1626:
   ;	
   ;	   if (!strncmp(string,"CONNECT",7)) *string = 0;
   ;	
	mov	ax,7
	push	ax
	mov	ax,offset DGROUP:s@+74
	push	ax
	push	di
	call	near ptr _strncmp
	add	sp,6
	or	ax,ax
	jne	short @6@1682
	mov	byte ptr [di],0
@6@1682:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_prompt_get_string	endp
   ;	
   ;	void get_string(char *string, int limit)    /* get a string with editing */
   ;	
	assume	cs:_TEXT
_get_string	proc	near
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
	mov	di,word ptr [bp+4]
   ;	
   ;	 {
   ;	   int pos = 0;
   ;	
	xor	si,si
   ;	
   ;	   int key;
   ;	   int flag = 1;
   ;	
	mov	word ptr [bp-4],1
	jmp	@7@590
@7@58:
   ;	
   ;	   while (flag)             /* wait while editing */
   ;	    {
   ;	      key = wait_ch();
   ;	
	call	near ptr _wait_ch
	mov	word ptr [bp-2],ax
   ;	
   ;	      if (((key == 8) || (key == 127)))
   ;	
	cmp	word ptr [bp-2],8
	je	short @7@114
	cmp	word ptr [bp-2],127
	jne	short @7@198
@7@114:
   ;	
   ;	      if (pos>0)
   ;	
	or	si,si
	jle	short @7@170
   ;	
   ;	       {
   ;	         pos--;         /* if an edit key is pressed and there's more to */
   ;	
	dec	si
   ;	
   ;	         print_string(backspacestring);   /* erase, erase the character */
   ;	
	mov	ax,offset DGROUP:_backspacestring
	push	ax
	call	near ptr _print_string
	pop	cx
   ;	
   ;	       }               /* and go back one */
   ;	
	jmp	short @7@198
@7@170:
   ;	
   ;	      else
   ;	         flag=0;
   ;	
	mov	word ptr [bp-4],0
@7@198:
   ;	
   ;	
   ;	      if (key == 27)    /* if we abort, then clear all characters */
   ;	
	cmp	word ptr [bp-2],27
	jne	short @7@282
   ;	
   ;	       {
   ;	         flag = 0;
   ;	
	mov	word ptr [bp-4],0
   ;	
   ;	         if (pos)       /* print a backslash to indicate abort */
   ;	
	or	si,si
	je	short @7@282
   ;	
   ;	          {
   ;	            print_chr('\\');
   ;	
	mov	al,92
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            print_chr(13);
   ;	
	mov	al,13
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            print_chr(10);
   ;	
	mov	al,10
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            pos = 0;
   ;	
	xor	si,si
@7@282:
   ;	
   ;	          };
   ;	       };
   ;	      if (key == 13)    /* finish the line */
   ;	
	cmp	word ptr [bp-2],13
	jne	short @7@366
   ;	
   ;	       {
   ;	         flag = 0;
   ;	
	mov	word ptr [bp-4],0
   ;	
   ;	         if (pos)
   ;	
	or	si,si
	je	short @7@366
   ;	
   ;	          {
   ;	            print_chr(13);
   ;	
	mov	al,13
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            print_chr(10);
   ;	
	mov	al,10
	push	ax
	call	near ptr _print_chr
	pop	cx
@7@366:
   ;	
   ;	          };
   ;	       };
   ;	      if (((key >= 32) && (key <= 126)) && (pos < limit))
   ;	
	cmp	word ptr [bp-2],32
	jl	short @7@590
	cmp	word ptr [bp-2],126
	jg	short @7@590
	cmp	si,word ptr [bp+6]
	jge	short @7@590
   ;	
   ;	       {                        /* insert the character if there's room */
   ;	         *(string+pos) = key;
   ;	
	mov	bx,si
	mov	al,byte ptr [bp-2]
	mov	byte ptr [bx+di],al
   ;	
   ;	         if (key == '+')        /* if +, don't let it be typed normally */
   ;	
	cmp	word ptr [bp-2],43
	jne	short @7@506
   ;	
   ;	          {
   ;	            print_chr(key);     /* print the character with a space */
   ;	
	mov	al,byte ptr [bp-2]
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            print_chr(32);      /* and a backspace */
   ;	
	mov	al,32
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            print_chr(8);
   ;	
	mov	al,8
	jmp	short @7@534
@7@506:
   ;	
   ;	          }
   ;	          else
   ;	          print_chr(key);       /* otherwise, print it normally */
   ;	
	mov	al,byte ptr [bp-2]
@7@534:
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	         pos++;
   ;	
	inc	si
@7@590:
	cmp	word ptr [bp-4],0
	je	@@10
	jmp	@7@58
@@10:
   ;	
   ;	       };
   ;	    };
   ;	   *(string+pos) = 0;           /* mark end of the string */
   ;	
	mov	bx,si
	mov	byte ptr [bx+di],0
   ;	
   ;	   if (!strncmp(string,"NO CARRIER",10)) *string = 0;
   ;	
	mov	ax,10
	push	ax
	mov	ax,offset DGROUP:s@+82
	push	ax
	push	di
	call	near ptr _strncmp
	add	sp,6
	or	ax,ax
	jne	short @7@674
	mov	byte ptr [di],0
@7@674:
   ;	
   ;	   if (!strncmp(string,"CONNECT",7)) *string = 0;
   ;	
	mov	ax,7
	push	ax
	mov	ax,offset DGROUP:s@+93
	push	ax
	push	di
	call	near ptr _strncmp
	add	sp,6
	or	ax,ax
	jne	short @7@730
	mov	byte ptr [di],0
@7@730:
   ;	
   ;	 };
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_get_string	endp
   ;	
   ;	void mark_user_log_on(void)
   ;	
	assume	cs:_TEXT
_mark_user_log_on	proc	near
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
   ;	
   ;	{
   ;	   FILE *fileptr;
   ;	   time_t tim;
   ;	   int flag = islocked(DOS_SEM);
   ;	
	xor	ax,ax
	push	ax
	call	near ptr _islocked
	pop	cx
	mov	di,ax
   ;	
   ;	
   ;	   if (!flag) lock_dos(491);
   ;	
	or	di,di
	jne	short @8@86
	mov	ax,491
	push	ax
	call	near ptr _lock_dos
	pop	cx
@8@86:
   ;	
   ;	   fileptr=g_fopen(USER_ENTER_LOG_FILE,"a","IO#1"); /* dos error */
   ;	
	mov	ax,offset DGROUP:s@+113
	push	ax
	mov	ax,offset DGROUP:s@+111
	push	ax
	mov	ax,offset DGROUP:s@+101
	push	ax
	call	near ptr _g_fopen
	add	sp,6
	mov	si,ax
   ;	
   ;	   if (!fileptr)                    /* open the log */
   ;	
	or	si,si
	jne	short @8@198
   ;	
   ;	    {
   ;	      if (!flag) unlock_dos();
   ;	
	or	di,di
	jne	short @8@170
	call	near ptr _unlock_dos
@8@170:
   ;	
   ;	      log_error(USER_ENTER_LOG_FILE);
   ;	
	mov	ax,offset DGROUP:s@+118
	push	ax
	call	near ptr _log_error
	pop	cx
   ;	
   ;	      return;
   ;	
	jmp	short @8@254
@8@198:
   ;	
   ;	    };
   ;	
   ;	   fprintf(fileptr,"(%02d,%03d) %s\n",tswitch,
   ;	
   ;	
   ;	                                    user_lines[tswitch].number,
   ;	                                    user_lines[tswitch].handle
   ;	            );         /* add the line to the file */
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,384
	imul	dx
	mov	dx,seg _user_lines
	add	ax,offset _user_lines+2
	push	dx
	push	ax
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,384
	imul	dx
	mov	dx,seg _user_lines
	mov	bx,ax
	mov	es,dx
	push	word ptr es:_user_lines[bx]
	push	word ptr DGROUP:_tswitch
	mov	ax,offset DGROUP:s@+128
	push	ax
	push	si
	call	near ptr _fprintf
	add	sp,12
   ;	
   ;	   time(&tim);                      /* insert the right time */
   ;	
	lea	ax,word ptr [bp-4]
	push	ax
	call	near ptr _time
	pop	cx
   ;	
   ;	   fprintf(fileptr,"Date: %s",ctime(&tim));
   ;	
	lea	ax,word ptr [bp-4]
	push	ax
	call	near ptr _ctime
	pop	cx
	push	ax
	mov	ax,offset DGROUP:s@+144
	push	ax
	push	si
	call	near ptr _fprintf
	add	sp,6
   ;	
   ;	   g_fclose(fileptr);                 /* close the log */
   ;	
	push	si
	call	near ptr _g_fclose
	pop	cx
   ;	
   ;	   if (!flag) unlock_dos();
   ;	
	or	di,di
	jne	short @8@254
	call	near ptr _unlock_dos
@8@254:
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_mark_user_log_on	endp
   ;	
   ;	void log_error(const char *filename)
   ;	
	assume	cs:_TEXT
_log_error	proc	near
	push	bp
	mov	bp,sp
	sub	sp,106
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	 {
   ;	   FILE *fileptr;
   ;	   time_t tim;
   ;	   int flag = islocked(DOS_SEM);
   ;	
	xor	ax,ax
	push	ax
	call	near ptr _islocked
	pop	cx
	mov	word ptr [bp-6],ax
   ;	
   ;	   char s[100];
   ;	
   ;	   if (!tasking)
   ;	
	mov	al,byte ptr DGROUP:_tasking
	cbw	
	or	ax,ax
	jne	short @9@170
   ;	
   ;	   {
   ;	     printf("\n");
   ;	
	mov	ax,offset DGROUP:s@+153
	push	ax
	call	near ptr _printf
	pop	cx
   ;	
   ;	     if (*filename == '*')
   ;	
	cmp	byte ptr [si],42
	jne	short @9@114
   ;	
   ;	      printf("%s\n",filename);
   ;	
	push	si
	mov	ax,offset DGROUP:s@+155
	push	ax
	call	near ptr _printf
	pop	cx
	pop	cx
	jmp	short @9@142
@9@114:
   ;	
   ;	     else
   ;	      perror(filename);
   ;	
	push	si
	call	near ptr _perror
	pop	cx
@9@142:
   ;	
   ;	     return;
   ;	
	jmp	@9@562
@9@170:
   ;	
   ;	   }
   ;	
   ;	#ifdef DEBUG
   ;	   print_string_to("FUCKING ERROR in ",0);
   ;	   print_str_cr_to(filename,0);
   ;	   return;
   ;	#endif
   ;	
   ;	   if (!strncmp(filename,"TEMP",4))
   ;	
	mov	ax,4
	push	ax
	mov	ax,offset DGROUP:s@+159
	push	ax
	push	si
	call	near ptr _strncmp
	add	sp,6
	or	ax,ax
	jne	short @9@226
   ;	
   ;	     return;  // Dont log errors with temp files
   ;	
	jmp	@9@562
@9@226:
   ;	
   ;	
   ;	   if (!flag) lock_dos(492);
   ;	
	cmp	word ptr [bp-6],0
	jne	short @9@282
	mov	ax,492
	push	ax
	call	near ptr _lock_dos
	pop	cx
@9@282:
   ;	
   ;	   if (*filename=='*')              /* if the filename is a asterisk */
   ;	
	cmp	byte ptr [si],42
	jne	short @9@338
   ;	
   ;	    sprintf(s,"%s\n",filename+1);   /* use it directly */
   ;	
	mov	ax,si
	inc	ax
	push	ax
	mov	ax,offset DGROUP:s@+164
	jmp	short @9@366
@9@338:
   ;	
   ;	    else sprintf(s,"Error: %s",_strerror(filename));  /* otherwise use the */
   ;	
	push	si
	call	near ptr __strerror
	pop	cx
	push	ax
	mov	ax,offset DGROUP:s@+168
@9@366:
	push	ax
	lea	ax,word ptr [bp-106]
	push	ax
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	   fileptr=g_fopen("error.log","a","IO#2");              /* dos error */
   ;	
	mov	ax,offset DGROUP:s@+190
	push	ax
	mov	ax,offset DGROUP:s@+188
	push	ax
	mov	ax,offset DGROUP:s@+178
	push	ax
	call	near ptr _g_fopen
	add	sp,6
	mov	di,ax
   ;	
   ;	   if (!fileptr)                    /* open the error log */
   ;	
	or	di,di
	jne	short @9@506
   ;	
   ;	    {
   ;	      if (!flag) unlock_dos();
   ;	
	cmp	word ptr [bp-6],0
	jne	short @9@478
	call	near ptr _unlock_dos
@9@478:
   ;	
   ;	      return;
   ;	
	jmp	short @9@562
@9@506:
   ;	
   ;	    };
   ;	   fprintf(fileptr,"%s",s);         /* add the line to the file */
   ;	
	lea	ax,word ptr [bp-106]
	push	ax
	mov	ax,offset DGROUP:s@+195
	push	ax
	push	di
	call	near ptr _fprintf
	add	sp,6
   ;	
   ;	   time(&tim);                      /* insert the right time */
   ;	
	lea	ax,word ptr [bp-4]
	push	ax
	call	near ptr _time
	pop	cx
   ;	
   ;	   fprintf(fileptr,"Date: %s\n",ctime(&tim));
   ;	
	lea	ax,word ptr [bp-4]
	push	ax
	call	near ptr _ctime
	pop	cx
	push	ax
	mov	ax,offset DGROUP:s@+198
	push	ax
	push	di
	call	near ptr _fprintf
	add	sp,6
   ;	
   ;	   g_fclose(fileptr);                 /* close the error log */
   ;	
	push	di
	call	near ptr _g_fclose
	pop	cx
   ;	
   ;	   if (!flag) unlock_dos();
   ;	
	cmp	word ptr [bp-6],0
	jne	short @9@562
	call	near ptr _unlock_dos
@9@562:
   ;	
   ;	 };
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_log_error	endp
   ;	
   ;	void log_event(const char *filename,char *event)
   ;	
	assume	cs:_TEXT
_log_event	proc	near
	push	bp
	mov	bp,sp
	sub	sp,104
	push	si
	push	di
   ;	
   ;	 {
   ;	   FILE *fileptr;
   ;	   time_t tim;
   ;	   int flag = islocked(DOS_SEM);
   ;	
	xor	ax,ax
	push	ax
	call	near ptr _islocked
	pop	cx
	mov	di,ax
   ;	
   ;	   char s[100];
   ;	
   ;	
   ;	   if (!flag) lock_dos(493);
   ;	
	or	di,di
	jne	short @10@86
	mov	ax,493
	push	ax
	call	near ptr _lock_dos
	pop	cx
@10@86:
   ;	
   ;	
   ;	   fileptr=g_fopen(filename,"a","IO#3");              /* dos error */
   ;	
	mov	ax,offset DGROUP:s@+210
	push	ax
	mov	ax,offset DGROUP:s@+208
	push	ax
	push	word ptr [bp+4]
	call	near ptr _g_fopen
	add	sp,6
	mov	si,ax
   ;	
   ;	   if (!fileptr)                    /* open the error log */
   ;	
	or	si,si
	jne	short @10@198
   ;	
   ;	    {
   ;	      if (!flag) unlock_dos();
   ;	
	or	di,di
	jne	short @10@170
	call	near ptr _unlock_dos
@10@170:
   ;	
   ;	      return;
   ;	
	jmp	short @10@254
@10@198:
   ;	
   ;	    };
   ;	
   ;	
   ;	                                    /* insert User Number, and NODE info */
   ;	   fprintf(fileptr,"#%03d (%02d) | ",user_lines[tswitch].number,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,384
	imul	dx
	mov	dx,seg _user_lines
	mov	bx,ax
	mov	es,dx
	push	word ptr es:_user_lines[bx]
	mov	ax,offset DGROUP:s@+215
	push	ax
	push	si
	call	near ptr _fprintf
	add	sp,8
   ;	
   ;	
   ;	   time(&tim);                      /* insert the right time */
   ;	
	lea	ax,word ptr [bp-4]
	push	ax
	call	near ptr _time
	pop	cx
   ;	
   ;	   strftime(s,33,"%a %b %d %Y %I:%M:%S %p",localtime(&tim));
   ;	
	lea	ax,word ptr [bp-4]
	push	ax
	call	near ptr _localtime
	pop	cx
	push	ax
	mov	ax,offset DGROUP:s@+231
	push	ax
	mov	ax,33
	push	ax
	lea	ax,word ptr [bp-104]
	push	ax
	call	near ptr _strftime
	add	sp,8
   ;	
   ;	   fprintf(fileptr,"%s | ",s);
   ;	
	lea	ax,word ptr [bp-104]
	push	ax
	mov	ax,offset DGROUP:s@+255
	push	ax
	push	si
	call	near ptr _fprintf
	add	sp,6
   ;	
   ;	
   ;	   fprintf(fileptr,"%s\n",event);         /* add the line to the file */
   ;	
	push	word ptr [bp+6]
	mov	ax,offset DGROUP:s@+261
	push	ax
	push	si
	call	near ptr _fprintf
	add	sp,6
   ;	
   ;	
   ;	   g_fclose(fileptr);                 /* close the error log */
   ;	
	push	si
	call	near ptr _g_fclose
	pop	cx
   ;	
   ;	   if (!flag) unlock_dos();
   ;	
	or	di,di
	jne	short @10@254
	call	near ptr _unlock_dos
@10@254:
   ;	
   ;	 };
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_log_event	endp
   ;	
   ;	void log_kill(int node,int who_killed)
   ;	
	assume	cs:_TEXT
_log_kill	proc	near
	push	bp
	mov	bp,sp
	sub	sp,334
	push	si
	push	di
	mov	si,word ptr [bp+4]
	mov	di,word ptr [bp+6]
   ;	
   ;	 {
   ;	   FILE *fileptr;
   ;	   time_t tim;
   ;	   int flag = islocked(DOS_SEM);
   ;	
	xor	ax,ax
	push	ax
	call	near ptr _islocked
	pop	cx
	mov	word ptr [bp-8],ax
   ;	
   ;	   char n1[100],n2[100],n3[100];
   ;	   char temp[20];
   ;	   char type[5];
   ;	
   ;	   if (!flag) lock_dos(494);
   ;	
	cmp	word ptr [bp-8],0
	jne	short @11@86
	mov	ax,494
	push	ax
	call	near ptr _lock_dos
	pop	cx
@11@86:
   ;	
   ;	
   ;	   fileptr=g_fopen(KILL_LOG_FILE,"a","IO#3");              /* dos error */
   ;	
	mov	ax,offset DGROUP:s@+280
	push	ax
	mov	ax,offset DGROUP:s@+278
	push	ax
	mov	ax,offset DGROUP:s@+265
	push	ax
	call	near ptr _g_fopen
	add	sp,6
	mov	word ptr [bp-2],ax
   ;	
   ;	   if (!fileptr)                    /* open the error log */
   ;	
	cmp	word ptr [bp-2],0
	jne	short @11@198
   ;	
   ;	    {
   ;	      if (!flag) unlock_dos();
   ;	
	cmp	word ptr [bp-8],0
	jne	short @11@170
	call	near ptr _unlock_dos
@11@170:
   ;	
   ;	      return;
   ;	
	jmp	@11@646
@11@198:
   ;	
   ;	    };
   ;	
   ;	   if (user_lines[who_killed].number<0)
   ;	
	mov	ax,di
	mov	dx,384
	imul	dx
	mov	dx,seg _user_lines
	mov	bx,ax
	mov	es,dx
	cmp	word ptr es:_user_lines[bx],0
	jge	short @11@366
   ;	
   ;	    { if (line_status[who_killed].link)
   ;	
	mov	ax,di
	mov	dx,187
	imul	dx
	mov	bx,ax
	cmp	byte ptr DGROUP:_line_status[bx+180],0
	je	short @11@282
   ;	
   ;	        strcpy(type,"LINK");
   ;	
	mov	ax,offset DGROUP:s@+285
	jmp	short @11@310
@11@282:
   ;	
   ;	      else
   ;	        strcpy(type,"%GST");
   ;	
	mov	ax,offset DGROUP:s@+290
@11@310:
	push	ax
	lea	ax,word ptr [bp-14]
	push	ax
	call	near ptr _strcpy
	pop	cx
	pop	cx
   ;	
   ;	    }
   ;	
	jmp	short @11@394
@11@366:
   ;	
   ;	   else
   ;	     sprintf(type,"#%03d",user_lines[who_killed].number);
   ;	
	mov	ax,di
	mov	dx,384
	imul	dx
	mov	dx,seg _user_lines
	mov	bx,ax
	mov	es,dx
	push	word ptr es:_user_lines[bx]
	mov	ax,offset DGROUP:s@+295
	push	ax
	lea	ax,word ptr [bp-14]
	push	ax
	call	near ptr _sprintf
	add	sp,6
@11@394:
   ;	
   ;	
   ;	   strncpy(temp,user_options[who_killed].noansi_handle,15);
   ;	
	mov	ax,15
	push	ax
	mov	ax,di
	mov	dx,178
	imul	dx
	add	ax,offset _user_options+38
	push	ax
	lea	ax,word ptr [bp-334]
	push	ax
	call	near ptr _strncpy
	add	sp,6
   ;	
   ;	   temp[16]=0;
   ;	
	mov	byte ptr [bp-318],0
   ;	
   ;	
   ;	
   ;	                                    /* insert User Number, and NODE info */
   ;	   sprintf(n1,"(%02d) %s:%c%s%c",who_killed,type,user_options[who_killed].staple[2],
   ;	
   ;	
   ;	                 temp,user_options[who_killed].staple[3]);
   ;	
	mov	ax,di
	mov	dx,178
	imul	dx
	mov	dx,seg _user_options
	mov	bx,ax
	mov	es,dx
	mov	al,byte ptr es:_user_options[bx+37]
	cbw	
	push	ax
	lea	ax,word ptr [bp-334]
	push	ax
	mov	ax,di
	mov	dx,178
	imul	dx
	mov	dx,seg _user_options
	mov	bx,ax
	mov	es,dx
	mov	al,byte ptr es:_user_options[bx+36]
	cbw	
	push	ax
	lea	ax,word ptr [bp-14]
	push	ax
	push	di
	mov	ax,offset DGROUP:s@+301
	push	ax
	lea	ax,word ptr [bp-114]
	push	ax
	call	near ptr _sprintf
	add	sp,14
   ;	
   ;	
   ;	   if (user_lines[node].number<0)
   ;	
	mov	ax,si
	mov	dx,384
	imul	dx
	mov	dx,seg _user_lines
	mov	bx,ax
	mov	es,dx
	cmp	word ptr es:_user_lines[bx],0
	jge	short @11@562
   ;	
   ;	    { if (line_status[node].link)
   ;	
	mov	ax,si
	mov	dx,187
	imul	dx
	mov	bx,ax
	cmp	byte ptr DGROUP:_line_status[bx+180],0
	je	short @11@478
   ;	
   ;	        strcpy(type,"LINK");
   ;	
	mov	ax,offset DGROUP:s@+318
	jmp	short @11@506
@11@478:
   ;	
   ;	      else
   ;	        strcpy(type,"%GST");
   ;	
	mov	ax,offset DGROUP:s@+323
@11@506:
	push	ax
	lea	ax,word ptr [bp-14]
	push	ax
	call	near ptr _strcpy
	pop	cx
	pop	cx
   ;	
   ;	    }
   ;	
	jmp	short @11@590
@11@562:
   ;	
   ;	   else
   ;	     sprintf(type,"#%03d",user_lines[node].number);
   ;	
	mov	ax,si
	mov	dx,384
	imul	dx
	mov	dx,seg _user_lines
	mov	bx,ax
	mov	es,dx
	push	word ptr es:_user_lines[bx]
	mov	ax,offset DGROUP:s@+328
	push	ax
	lea	ax,word ptr [bp-14]
	push	ax
	call	near ptr _sprintf
	add	sp,6
@11@590:
   ;	
   ;	
   ;	   strncpy(temp,user_options[node].noansi_handle,15);
   ;	
	mov	ax,15
	push	ax
	mov	ax,si
	mov	dx,178
	imul	dx
	add	ax,offset _user_options+38
	push	ax
	lea	ax,word ptr [bp-334]
	push	ax
	call	near ptr _strncpy
	add	sp,6
   ;	
   ;	   temp[16]=0;
   ;	
	mov	byte ptr [bp-318],0
   ;	
   ;	
   ;	                                     /* insert User Number, and NODE info */
   ;	   sprintf(n2,"(%02d) %s:%c%s%c",node,type,user_options[node].staple[2],
   ;	
   ;	
   ;	                 temp,user_options[node].staple[3]);
   ;	
	mov	ax,si
	mov	dx,178
	imul	dx
	mov	dx,seg _user_options
	mov	bx,ax
	mov	es,dx
	mov	al,byte ptr es:_user_options[bx+37]
	cbw	
	push	ax
	lea	ax,word ptr [bp-334]
	push	ax
	mov	ax,si
	mov	dx,178
	imul	dx
	mov	dx,seg _user_options
	mov	bx,ax
	mov	es,dx
	mov	al,byte ptr es:_user_options[bx+36]
	cbw	
	push	ax
	lea	ax,word ptr [bp-14]
	push	ax
	push	si
	mov	ax,offset DGROUP:s@+334
	push	ax
	lea	ax,word ptr [bp-214]
	push	ax
	call	near ptr _sprintf
	add	sp,14
   ;	
   ;	
   ;	
   ;	   time(&tim);                      /* insert the right time */
   ;	
	lea	ax,word ptr [bp-6]
	push	ax
	call	near ptr _time
	pop	cx
   ;	
   ;	   strftime(n3,22,"%m/%d/%y %I:%M:%S %p",localtime(&tim));
   ;	
	lea	ax,word ptr [bp-6]
	push	ax
	call	near ptr _localtime
	pop	cx
	push	ax
	mov	ax,offset DGROUP:s@+351
	push	ax
	mov	ax,22
	push	ax
	lea	ax,word ptr [bp-314]
	push	ax
	call	near ptr _strftime
	add	sp,8
   ;	
   ;	
   ;	   fprintf(fileptr,"%-28s %-28s %-18s",n1,n2,n3);
   ;	
	lea	ax,word ptr [bp-314]
	push	ax
	lea	ax,word ptr [bp-214]
	push	ax
	lea	ax,word ptr [bp-114]
	push	ax
	mov	ax,offset DGROUP:s@+372
	push	ax
	push	word ptr [bp-2]
	call	near ptr _fprintf
	add	sp,10
   ;	
   ;	
   ;	
   ;	   g_fclose(fileptr);                 /* close the error log */
   ;	
	push	word ptr [bp-2]
	call	near ptr _g_fclose
	pop	cx
   ;	
   ;	   if (!flag) unlock_dos();
   ;	
	cmp	word ptr [bp-8],0
	jne	short @11@646
	call	near ptr _unlock_dos
@11@646:
   ;	
   ;	 };
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_log_kill	endp
   ;	
   ;	void last_ten_kills(int num_last,int portnum)
   ;	
	assume	cs:_TEXT
_last_ten_kills	proc	near
	push	bp
	mov	bp,sp
	sub	sp,86
	push	si
	push	di
	mov	di,word ptr [bp+4]
   ;	
   ;	 {
   ;	   FILE *fileptr;
   ;	   char s[80];
   ;	   int num;
   ;	   int not_abort = 1;
   ;	
	mov	si,1
   ;	
   ;	   int key;
   ;	
   ;	   print_cr();
   ;	
	call	near ptr _print_cr
   ;	
   ;	   sprintf(s,"Last %d kills",num_last);
   ;	
	push	di
	mov	ax,offset DGROUP:s@+390
	push	ax
	lea	ax,word ptr [bp-86]
	push	ax
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	   print_str_cr(s);
   ;	
	lea	ax,word ptr [bp-86]
	push	ax
	call	near ptr _print_str_cr
	pop	cx
   ;	
   ;	   print_file("TEXT\\KILL.HDR");
   ;	
	mov	ax,offset DGROUP:s@+404
	push	ax
	call	near ptr _print_file
	pop	cx
   ;	
   ;	   num_last++;
   ;	
	inc	di
   ;	
   ;	   print_cr();
   ;	
	call	near ptr _print_cr
   ;	
   ;	
   ;	   lock_dos(495);
   ;	
	mov	ax,495
	push	ax
	call	near ptr _lock_dos
	pop	cx
   ;	
   ;	   if (!(fileptr=g_fopen(KILL_LOG_FILE,"rb","FILES#4")))       /* open the user log */
   ;	
	mov	ax,offset DGROUP:s@+434
	push	ax
	mov	ax,offset DGROUP:s@+431
	push	ax
	mov	ax,offset DGROUP:s@+418
	push	ax
	call	near ptr _g_fopen
	add	sp,6
	mov	word ptr [bp-2],ax
	or	ax,ax
	jne	short @12@86
   ;	
   ;	    {
   ;	      log_error(KILL_LOG_FILE);
   ;	
	mov	ax,offset DGROUP:s@+442
	push	ax
	call	near ptr _log_error
	pop	cx
   ;	
   ;	      unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	      return;
   ;	
	jmp	@12@506
@12@86:
   ;	
   ;	    };
   ;	   num = 1;
   ;	
	mov	word ptr [bp-4],1
   ;	
   ;	   s[78]=0;
   ;	
	mov	byte ptr [bp-8],0
	jmp	@12@422
@12@114:
   ;	
   ;	   while (not_abort && (num<num_last))
   ;	    {
   ;	      fseek(fileptr,-78*(long int)(num),SEEK_END);
   ;	
	mov	ax,2
	push	ax
	mov	ax,word ptr [bp-4]
	cwd	
	push	ax
	push	dx
	mov	dx,-1
	mov	ax,-78
	pop	cx
	pop	bx
	call	near ptr N_LXMUL@
	push	dx
	push	ax
	push	word ptr [bp-2]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	      not_abort = (ftell(fileptr) != 0);
   ;	
	push	word ptr [bp-2]
	call	near ptr _ftell
	pop	cx
	or	ax,dx
	je	short @12@170
	mov	ax,1
	jmp	short @12@198
@12@170:
	xor	ax,ax
@12@198:
	mov	si,ax
   ;	
   ;	      if (not_abort)
   ;	
	or	si,si
	je	short @12@394
   ;	
   ;	       {
   ;	       //  sprintf(s,"%2d: ",num);
   ;	         unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	      //   print_string(s);
   ;	         lock_dos(496);
   ;	
	mov	ax,496
	push	ax
	call	near ptr _lock_dos
	pop	cx
   ;	
   ;	         if (!fread(s,1,78,fileptr))
   ;	
	push	word ptr [bp-2]
	mov	ax,78
	push	ax
	mov	ax,1
	push	ax
	lea	ax,word ptr [bp-86]
	push	ax
	call	near ptr _fread
	add	sp,8
	or	ax,ax
	jne	short @12@282
   ;	
   ;	            not_abort=0;
   ;	
	xor	si,si
@12@282:
   ;	
   ;	         unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	         print_str_cr(s);
   ;	
	lea	ax,word ptr [bp-86]
	push	ax
	call	near ptr _print_str_cr
	pop	cx
   ;	
   ;	         key = get_first_char(tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	call	word ptr DGROUP:_a_get_first_char[bx]
	pop	cx
	mov	word ptr [bp-6],ax
   ;	
   ;	         if ((key == 3) || (key == 27))
   ;	
	cmp	word ptr [bp-6],3
	je	short @12@338
	cmp	word ptr [bp-6],27
	jne	short @12@366
@12@338:
   ;	
   ;	          {
   ;	            int_char(portnum);
   ;	
	push	word ptr [bp+6]
	mov	bx,word ptr [bp+6]
	shl	bx,1
	call	word ptr DGROUP:_a_get_nchar[bx]
	pop	cx
   ;	
   ;	            not_abort = 0;
   ;	
	xor	si,si
@12@366:
   ;	
   ;	          };
   ;	         lock_dos(497);
   ;	
	mov	ax,497
	push	ax
	call	near ptr _lock_dos
	pop	cx
@12@394:
   ;	
   ;	       };
   ;	      num++;
   ;	
	inc	word ptr [bp-4]
@12@422:
	or	si,si
	je	short @12@478
	cmp	word ptr [bp-4],di
	jge	@@11
	jmp	@12@114
@@11:
@12@478:
   ;	
   ;	    };
   ;	   g_fclose(fileptr);
   ;	
	push	word ptr [bp-2]
	call	near ptr _g_fclose
	pop	cx
   ;	
   ;	   clear_call_on_logoff();
   ;	
	call	near ptr _clear_call_on_logoff
   ;	
   ;	   unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	   print_cr();
   ;	
	call	near ptr _print_cr
@12@506:
   ;	
   ;	 };
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_last_ten_kills	endp
   ;	
   ;	void print_file(const char *filename)
   ;	
	assume	cs:_TEXT
_print_file	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	 {
   ;	  print_file_to(filename,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	push	word ptr [bp+4]
	call	near ptr _print_file_to
	pop	cx
	pop	cx
   ;	
   ;	
   ;	 };
   ;	
	pop	bp
	ret	
_print_file	endp
   ;	
   ;	void print_file_to(const char *filename,int portnum)
   ;	
	assume	cs:_TEXT
_print_file_to	proc	near
	push	bp
	mov	bp,sp
	sub	sp,610
	push	si
	push	di
	mov	si,word ptr [bp+6]
   ;	
   ;	 {
   ;	   FILE *fileptr;
   ;	   long int location = 0;
   ;	
	mov	word ptr [bp-4],0
	mov	word ptr [bp-6],0
   ;	
   ;	   int count=0;
   ;	
	mov	word ptr [bp-8],0
   ;	
   ;	   char buf[512];
   ;	   int loop, point = 1;
   ;	
	mov	word ptr [bp-12],1
   ;	
   ;	   int ischar;
   ;	   int is_ansi_file=0;
   ;	
	mov	word ptr [bp-16],0
   ;	
   ;	   char *temp_file=filename;
   ;	
	mov	ax,word ptr [bp+4]
	mov	word ptr [bp-18],ax
   ;	
   ;	   char temp_buf[80];
   ;	
   ;	   if ((strlen(filename)<75) && (line_status[portnum].ansi) &&
   ;	
   ;	
   ;	                (line_status[portnum].full_screen_ansi))
   ;	
	push	word ptr [bp+4]
	call	near ptr _strlen
	pop	cx
	cmp	ax,75
	jb	@@12
	jmp	@14@422
@@12:
	mov	ax,si
	mov	dx,187
	imul	dx
	mov	bx,ax
	cmp	byte ptr DGROUP:_line_status[bx+28],0
	jne	@@13
	jmp	@14@422
@@13:
	mov	ax,si
	mov	dx,187
	imul	dx
	mov	bx,ax
	cmp	byte ptr DGROUP:_line_status[bx+29],0
	jne	@@14
	jmp	@14@422
@@14:
   ;	
   ;	    {  char *end;
   ;	       strcpy(temp_buf,filename);
   ;	
	push	word ptr [bp+4]
	lea	ax,word ptr [bp-610]
	push	ax
	call	near ptr _strcpy
	pop	cx
	pop	cx
   ;	
   ;	       end=temp_buf+strlen(temp_buf);
   ;	
	lea	ax,word ptr [bp-610]
	push	ax
	call	near ptr _strlen
	pop	cx
	lea	dx,word ptr [bp-610]
	add	ax,dx
	mov	di,ax
	jmp	short @14@170
@14@142:
   ;	
   ;	
   ;	       while ((*end!='\\') && (*end!='.') && (end>=temp_buf))
   ;	         end--;
   ;	
	dec	di
@14@170:
	cmp	byte ptr [di],92
	je	short @14@254
	cmp	byte ptr [di],46
	je	short @14@254
	lea	ax,word ptr [bp-610]
	cmp	ax,di
	jbe	short @14@142
@14@254:
   ;	
   ;	
   ;	       if (*end=='.') *end=0;
   ;	
	cmp	byte ptr [di],46
	jne	short @14@310
	mov	byte ptr [di],0
@14@310:
   ;	
   ;	
   ;	       strcat(temp_buf,".ANS");
   ;	
	mov	ax,offset DGROUP:s@+455
	push	ax
	lea	ax,word ptr [bp-610]
	push	ax
	call	near ptr _strcat
	pop	cx
	pop	cx
   ;	
   ;	       temp_file=temp_buf;
   ;	
	lea	ax,word ptr [bp-610]
	mov	word ptr [bp-18],ax
   ;	
   ;	       is_ansi_file=1;
   ;	
	mov	word ptr [bp-16],1
   ;	
   ;	       lock_dos(498);
   ;	
	mov	ax,498
	push	ax
	call	near ptr _lock_dos
	pop	cx
   ;	
   ;	       if ((fileptr=g_fopen(temp_file,"rb","IO#3"))==NULL)   /* ANSI */
   ;	
	mov	ax,offset DGROUP:s@+463
	push	ax
	mov	ax,offset DGROUP:s@+460
	push	ax
	push	word ptr [bp-18]
	call	near ptr _g_fopen
	add	sp,6
	mov	word ptr [bp-2],ax
	or	ax,ax
	jne	short @14@366
   ;	
   ;	         { is_ansi_file=0;
   ;	
	mov	word ptr [bp-16],0
   ;	
   ;	           temp_file=filename;
   ;	
	mov	ax,word ptr [bp+4]
	mov	word ptr [bp-18],ax
   ;	
   ;	         }
   ;	
	jmp	short @14@394
@14@366:
   ;	
   ;	       else
   ;	         g_fclose(fileptr);
   ;	
	push	word ptr [bp-2]
	call	near ptr _g_fclose
	pop	cx
@14@394:
   ;	
   ;	       unlock_dos();
   ;	
	call	near ptr _unlock_dos
@14@422:
   ;	
   ;	    }
   ;	
   ;	   special_code(!is_ansi_file,portnum);
   ;	
	push	si
	mov	ax,word ptr [bp-16]
	neg	ax
	sbb	ax,ax
	inc	ax
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
	jmp	@14@954
@14@450:
   ;	
   ;	   while (point)
   ;	    {
   ;	      fileptr=0;
   ;	
	mov	word ptr [bp-2],0
	jmp	short @14@590
@14@478:
   ;	
   ;	      while (!fileptr)
   ;	       {
   ;	         lock_dos(499);
   ;	
	mov	ax,499
	push	ax
	call	near ptr _lock_dos
	pop	cx
   ;	
   ;	         if( (fileptr=g_fopen(temp_file,"rb","IO#3"))==NULL)   /* open the file */
   ;	
	mov	ax,offset DGROUP:s@+471
	push	ax
	mov	ax,offset DGROUP:s@+468
	push	ax
	push	word ptr [bp-18]
	call	near ptr _g_fopen
	add	sp,6
	mov	word ptr [bp-2],ax
	or	ax,ax
	jne	short @14@534
   ;	
   ;	          {
   ;	            log_error(filename);
   ;	
	push	word ptr [bp+4]
	call	near ptr _log_error
	pop	cx
   ;	
   ;	            unlock_dos();
   ;	
	call	near ptr _unlock_dos
	jmp	@14@982
@14@534:
   ;	
   ;	            special_code(0,portnum);
   ;	            return;
   ;	          };
   ;	         unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	         if (!fileptr) next_task();
   ;	
	cmp	word ptr [bp-2],0
	jne	short @14@590
	pushf	
	call	far ptr _next_task
@14@590:
	cmp	word ptr [bp-2],0
	je	short @14@478
   ;	
   ;	       };
   ;	      lock_dos(500);
   ;	
	mov	ax,500
	push	ax
	call	near ptr _lock_dos
	pop	cx
   ;	
   ;	      fseek(fileptr,location,SEEK_SET);         /* go to the portion of */
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp-4]
	push	word ptr [bp-6]
	push	word ptr [bp-2]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	                /* the file we want to read */
   ;	      point = fread(&buf, 1, 512, fileptr);     /* read 512 bytes */
   ;	
	push	word ptr [bp-2]
	mov	ax,512
	push	ax
	mov	ax,1
	push	ax
	lea	ax,word ptr [bp-530]
	push	ax
	call	near ptr _fread
	add	sp,8
	mov	word ptr [bp-12],ax
   ;	
   ;	      g_fclose(fileptr);          /* close the file */
   ;	
	push	word ptr [bp-2]
	call	near ptr _g_fclose
	pop	cx
   ;	
   ;	      unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	      location += 512;          /* go 512 bytes further */
   ;	
	add	word ptr [bp-6],512
	adc	word ptr [bp-4],0
   ;	
   ;	      for (loop=0;loop<point;loop++)     /* print what's in the buffer */
   ;	
	mov	word ptr [bp-10],0
	jmp	@14@926
@14@646:
   ;	
   ;	       {
   ;	         print_chr_to(buf[loop],portnum);
   ;	
	push	si
	lea	ax,word ptr [bp-530]
	mov	bx,word ptr [bp-10]
	add	bx,ax
	mov	al,byte ptr [bx]
	push	ax
	call	near ptr _print_chr_to
	pop	cx
	pop	cx
   ;	
   ;	         ischar = get_first_char(portnum);
   ;	
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_get_first_char[bx]
	pop	cx
	mov	word ptr [bp-14],ax
   ;	
   ;	         if (ischar==19)
   ;	
	cmp	word ptr [bp-14],19
	jne	short @14@702
   ;	
   ;	          {
   ;	            wait_ch();
   ;	
	call	near ptr _wait_ch
   ;	
   ;	            wait_ch();
   ;	
	call	near ptr _wait_ch
@14@702:
   ;	
   ;	          };
   ;	         if (!dcd_detect(portnum))  leave();
   ;	
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_dcd_detect[bx]
	pop	cx
	or	ax,ax
	jne	short @14@758
	call	near ptr _leave
@14@758:
   ;	
   ;	         if ((ischar==3) || (ischar==27))
   ;	
	cmp	word ptr [bp-14],3
	je	short @14@814
	cmp	word ptr [bp-14],27
	jne	short @14@842
@14@814:
   ;	
   ;	          {
   ;	            point=0;
   ;	
	mov	word ptr [bp-12],0
   ;	
   ;	            int_char(portnum);
   ;	
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_get_nchar[bx]
	pop	cx
   ;	
   ;	            empty_outbuffer(portnum);
   ;	
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_empty_outbuffer[bx]
	pop	cx
   ;	
   ;	            special_code(0,portnum);
   ;	
	push	si
	xor	ax,ax
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
   ;	
   ;	            print_cr();
   ;	
	call	near ptr _print_cr
   ;	
   ;	            print_str_cr("--> Aborted");
   ;	
	mov	ax,offset DGROUP:s@+476
	push	ax
	call	near ptr _print_str_cr
	pop	cx
@14@842:
   ;	
   ;	          };
   ;	       if ((count++)>120)
   ;	
	mov	ax,word ptr [bp-8]
	inc	word ptr [bp-8]
	cmp	ax,120
	jle	short @14@898
   ;	
   ;	         {wait_for_xmit(portnum,30);
   ;	
	mov	ax,30
	push	ax
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_wait_for_xmit[bx]
	pop	cx
	pop	cx
   ;	
   ;	          count=0;
   ;	
	mov	word ptr [bp-8],0
@14@898:
	inc	word ptr [bp-10]
@14@926:
	mov	ax,word ptr [bp-10]
	cmp	ax,word ptr [bp-12]
	jge	@@15
	jmp	@14@646
@@15:
@14@954:
	cmp	word ptr [bp-12],0
	je	@@16
	jmp	@14@450
@@16:
@14@982:
   ;	
   ;	         }
   ;	       };
   ;	
   ;	    };
   ;	   special_code(0,portnum);
   ;	
	push	si
	xor	ax,ax
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
   ;	
   ;	 };
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_print_file_to	endp
   ;	
   ;	void print_file_to_cntrl(const char *filename,int portnum,int ansi,
   ;	
	assume	cs:_TEXT
_print_file_to_cntrl	proc	near
	push	bp
	mov	bp,sp
	sub	sp,614
	push	si
	push	di
	mov	si,word ptr [bp+6]
   ;	
   ;	                   int pause,int abort,int paging)
   ;	 {
   ;	   FILE *fileptr;
   ;	   long int location = 0;
   ;	
	mov	word ptr [bp-4],0
	mov	word ptr [bp-6],0
   ;	
   ;	   char buf[512];
   ;	   int xmitwait=0;
   ;	
	mov	word ptr [bp-8],0
   ;	
   ;	   int loop, point = 1;
   ;	
	mov	word ptr [bp-12],1
   ;	
   ;	   int ischar;
   ;	   int page_break = 0;
   ;	
	mov	word ptr [bp-16],0
   ;	
   ;	   int count;
   ;	   char *temp_file=filename;
   ;	
	mov	ax,word ptr [bp+4]
	mov	word ptr [bp-20],ax
   ;	
   ;	   char temp_buf[80];
   ;	   int pagebreaks = is_console() ? 15 : 20;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	cmp	byte ptr DGROUP:_is_a_console[bx],0
	je	short @15@86
	mov	ax,15
	jmp	short @15@114
@15@86:
	mov	ax,20
@15@114:
	mov	word ptr [bp-22],ax
   ;	
   ;	
   ;	   /* code for .ans files */
   ;	   if ((strlen(filename)<75) && (line_status[portnum].ansi) &&
   ;	
   ;	
   ;	         (line_status[portnum].full_screen_ansi))
   ;	
	push	word ptr [bp+4]
	call	near ptr _strlen
	pop	cx
	cmp	ax,75
	jb	@@17
	jmp	@15@506
@@17:
	mov	ax,si
	mov	dx,187
	imul	dx
	mov	bx,ax
	cmp	byte ptr DGROUP:_line_status[bx+28],0
	jne	@@18
	jmp	@15@506
@@18:
	mov	ax,si
	mov	dx,187
	imul	dx
	mov	bx,ax
	cmp	byte ptr DGROUP:_line_status[bx+29],0
	jne	@@19
	jmp	@15@506
@@19:
   ;	
   ;	    {  char *end;
   ;	       strcpy(temp_buf,filename);
   ;	
	push	word ptr [bp+4]
	lea	ax,word ptr [bp-614]
	push	ax
	call	near ptr _strcpy
	pop	cx
	pop	cx
   ;	
   ;	       end=temp_buf+strlen(temp_buf);
   ;	
	lea	ax,word ptr [bp-614]
	push	ax
	call	near ptr _strlen
	pop	cx
	lea	dx,word ptr [bp-614]
	add	ax,dx
	mov	di,ax
	jmp	short @15@254
@15@226:
   ;	
   ;	
   ;	       while ((*end!='\\') && (*end!='.') && (end>=temp_buf))
   ;	         end--;
   ;	
	dec	di
@15@254:
	cmp	byte ptr [di],92
	je	short @15@338
	cmp	byte ptr [di],46
	je	short @15@338
	lea	ax,word ptr [bp-614]
	cmp	ax,di
	jbe	short @15@226
@15@338:
   ;	
   ;	
   ;	       if (*end=='.') *end=0;
   ;	
	cmp	byte ptr [di],46
	jne	short @15@394
	mov	byte ptr [di],0
@15@394:
   ;	
   ;	
   ;	       strcat(temp_buf,".ANS");
   ;	
	mov	ax,offset DGROUP:s@+488
	push	ax
	lea	ax,word ptr [bp-614]
	push	ax
	call	near ptr _strcat
	pop	cx
	pop	cx
   ;	
   ;	       temp_file=temp_buf;
   ;	
	lea	ax,word ptr [bp-614]
	mov	word ptr [bp-20],ax
   ;	
   ;	       lock_dos(501);
   ;	
	mov	ax,501
	push	ax
	call	near ptr _lock_dos
	pop	cx
   ;	
   ;	       if ((fileptr=g_fopen(temp_file,"rb","IO#3"))==NULL)   /* ANSI */
   ;	
	mov	ax,offset DGROUP:s@+496
	push	ax
	mov	ax,offset DGROUP:s@+493
	push	ax
	push	word ptr [bp-20]
	call	near ptr _g_fopen
	add	sp,6
	mov	word ptr [bp-2],ax
	or	ax,ax
	jne	short @15@450
   ;	
   ;	           temp_file=filename;
   ;	
	mov	ax,word ptr [bp+4]
	mov	word ptr [bp-20],ax
	jmp	short @15@478
@15@450:
   ;	
   ;	       else
   ;	         {g_fclose(fileptr);
   ;	
	push	word ptr [bp-2]
	call	near ptr _g_fclose
	pop	cx
   ;	
   ;	          ansi=0;
   ;	
	mov	word ptr [bp+8],0
   ;	
   ;	          paging=0;
   ;	
	mov	word ptr [bp+14],0
@15@478:
   ;	
   ;	          }
   ;	       unlock_dos();
   ;	
	call	near ptr _unlock_dos
@15@506:
   ;	
   ;	    }
   ;	
   ;	
   ;	   /* end of code for .ansi files */
   ;	
   ;	
   ;	
   ;	
   ;	
   ;	
   ;	
   ;	   if (ansi) special_code(1,portnum);
   ;	
	cmp	word ptr [bp+8],0
	je	short @15@562
	push	si
	mov	ax,1
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
@15@562:
	jmp	@15@1682
@15@590:
   ;	
   ;	   while (point)
   ;	    {
   ;	      fileptr=0;
   ;	
	mov	word ptr [bp-2],0
	jmp	short @15@786
@15@618:
   ;	
   ;	      while (!fileptr)
   ;	       {
   ;	         lock_dos(502);
   ;	
	mov	ax,502
	push	ax
	call	near ptr _lock_dos
	pop	cx
   ;	
   ;	         if( (fileptr=g_fopen(temp_file,"rb","IO#4"))==NULL)   /* open the file */
   ;	
	mov	ax,offset DGROUP:s@+504
	push	ax
	mov	ax,offset DGROUP:s@+501
	push	ax
	push	word ptr [bp-20]
	call	near ptr _g_fopen
	add	sp,6
	mov	word ptr [bp-2],ax
	or	ax,ax
	jne	short @15@730
   ;	
   ;	          {
   ;	            log_error(filename);
   ;	
	push	word ptr [bp+4]
	call	near ptr _log_error
	pop	cx
   ;	
   ;	            unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	            if (ansi) special_code(0,portnum);
   ;	
	cmp	word ptr [bp+8],0
	je	short @15@702
	push	si
	xor	ax,ax
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
@15@702:
   ;	
   ;	            return;
   ;	
	jmp	@15@1766
@15@730:
   ;	
   ;	          };
   ;	         unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	         if (!fileptr) next_task();
   ;	
	cmp	word ptr [bp-2],0
	jne	short @15@786
	pushf	
	call	far ptr _next_task
@15@786:
	cmp	word ptr [bp-2],0
	je	short @15@618
   ;	
   ;	       };
   ;	      lock_dos(503);
   ;	
	mov	ax,503
	push	ax
	call	near ptr _lock_dos
	pop	cx
   ;	
   ;	      fseek(fileptr,location,SEEK_SET);         /* go to the portion of */
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp-4]
	push	word ptr [bp-6]
	push	word ptr [bp-2]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	                /* the file we want to read */
   ;	      point = fread(&buf, 1, 512, fileptr);     /* read 512 bytes */
   ;	
	push	word ptr [bp-2]
	mov	ax,512
	push	ax
	mov	ax,1
	push	ax
	lea	ax,word ptr [bp-534]
	push	ax
	call	near ptr _fread
	add	sp,8
	mov	word ptr [bp-12],ax
   ;	
   ;	      g_fclose(fileptr);          /* close the file */
   ;	
	push	word ptr [bp-2]
	call	near ptr _g_fclose
	pop	cx
   ;	
   ;	      unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	      location += 512;          /* go 512 bytes further */
   ;	
	add	word ptr [bp-6],512
	adc	word ptr [bp-4],0
   ;	
   ;	      for (loop=0;loop<point;loop++)     /* print what's in the buffer */
   ;	
	mov	word ptr [bp-10],0
	jmp	@15@1654
@15@842:
   ;	
   ;	       {
   ;	         print_chr_to(buf[loop],portnum);
   ;	
	push	si
	lea	ax,word ptr [bp-534]
	mov	bx,word ptr [bp-10]
	add	bx,ax
	mov	al,byte ptr [bx]
	push	ax
	call	near ptr _print_chr_to
	pop	cx
	pop	cx
   ;	
   ;	         if (paging)
   ;	
	cmp	word ptr [bp+14],0
	je	short @15@1262
   ;	
   ;	          {
   ;	            if (buf[loop] == 10)
   ;	
	lea	ax,word ptr [bp-534]
	mov	bx,word ptr [bp-10]
	add	bx,ax
	cmp	byte ptr [bx],10
	jne	short @15@1262
   ;	
   ;	             {
   ;	               if (!cur_video_state(portnum))
   ;	
	push	si
	call	near ptr _cur_video_state
	pop	cx
	cbw	
	or	ax,ax
	jne	short @15@1262
   ;	
   ;	                {
   ;	                 page_break++;
   ;	
	inc	word ptr [bp-16]
   ;	
   ;	                 if (page_break == pagebreaks)
   ;	
	mov	ax,word ptr [bp-16]
	cmp	ax,word ptr [bp-22]
	jne	short @15@1262
   ;	
   ;	                  {
   ;	                    print_string("[ Press Return ]");
   ;	
	mov	ax,offset DGROUP:s@+509
	push	ax
	call	near ptr _print_string
	pop	cx
@15@982:
   ;	
   ;	                    do
   ;	                     {
   ;	                       count = wait_ch();
   ;	
	call	near ptr _wait_ch
	mov	word ptr [bp-18],ax
   ;	
   ;	                       if (((count == 27) || (count == 3)) && abort)
   ;	
	cmp	word ptr [bp-18],27
	je	short @15@1038
	cmp	word ptr [bp-18],3
	jne	short @15@1094
@15@1038:
	cmp	word ptr [bp+12],0
	je	short @15@1094
   ;	
   ;	                        {
   ;	                         count = 13;
   ;	
	mov	word ptr [bp-18],13
   ;	
   ;	                         point = 0;
   ;	
	mov	word ptr [bp-12],0
@15@1094:
   ;	
   ;	                       };
   ;	                     } while (count != 13);
   ;	
	cmp	word ptr [bp-18],13
	jne	short @15@982
   ;	
   ;	                    for (count=0;count<16;count++)
   ;	
	mov	word ptr [bp-18],0
	jmp	short @15@1206
@15@1150:
   ;	
   ;	                     print_string(backspacestring);
   ;	
	mov	ax,offset DGROUP:_backspacestring
	push	ax
	call	near ptr _print_string
	pop	cx
	inc	word ptr [bp-18]
@15@1206:
	cmp	word ptr [bp-18],16
	jl	short @15@1150
   ;	
   ;	                    page_break = 0;
   ;	
	mov	word ptr [bp-16],0
@15@1262:
   ;	
   ;	                  };
   ;	                };
   ;	             };
   ;	          };
   ;	         ischar = get_first_char(portnum);
   ;	
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_get_first_char[bx]
	pop	cx
	mov	word ptr [bp-14],ax
   ;	
   ;	         if ((ischar==19) && pause)
   ;	
	cmp	word ptr [bp-14],19
	jne	short @15@1346
	cmp	word ptr [bp+10],0
	je	short @15@1346
   ;	
   ;	          {
   ;	            wait_ch();
   ;	
	call	near ptr _wait_ch
   ;	
   ;	            wait_ch();
   ;	
	call	near ptr _wait_ch
@15@1346:
   ;	
   ;	          };
   ;	         if (!dcd_detect(portnum)) leave();
   ;	
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_dcd_detect[bx]
	pop	cx
	or	ax,ax
	jne	short @15@1402
	call	near ptr _leave
@15@1402:
   ;	
   ;	         if (((ischar==3) || (ischar==27)) && abort)
   ;	
	cmp	word ptr [bp-14],3
	je	short @15@1458
	cmp	word ptr [bp-14],27
	jne	short @15@1570
@15@1458:
	cmp	word ptr [bp+12],0
	je	short @15@1570
   ;	
   ;	          {
   ;	            point=0;
   ;	
	mov	word ptr [bp-12],0
   ;	
   ;	            int_char(portnum);
   ;	
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_get_nchar[bx]
	pop	cx
   ;	
   ;	            empty_outbuffer(portnum);
   ;	
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_empty_outbuffer[bx]
	pop	cx
   ;	
   ;	            if (ansi) special_code(0,portnum);
   ;	
	cmp	word ptr [bp+8],0
	je	short @15@1542
	push	si
	xor	ax,ax
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
@15@1542:
   ;	
   ;	            print_cr();
   ;	
	call	near ptr _print_cr
   ;	
   ;	            print_str_cr("--> Aborted");
   ;	
	mov	ax,offset DGROUP:s@+526
	push	ax
	call	near ptr _print_str_cr
	pop	cx
@15@1570:
   ;	
   ;	          };
   ;	        if ((xmitwait++)>120)
   ;	
	mov	ax,word ptr [bp-8]
	inc	word ptr [bp-8]
	cmp	ax,120
	jle	short @15@1626
   ;	
   ;	           {wait_for_xmit(portnum,30);
   ;	
	mov	ax,30
	push	ax
	push	si
	mov	bx,si
	shl	bx,1
	call	word ptr DGROUP:_a_wait_for_xmit[bx]
	pop	cx
	pop	cx
   ;	
   ;	            xmitwait=0;
   ;	
	mov	word ptr [bp-8],0
@15@1626:
	inc	word ptr [bp-10]
@15@1654:
	mov	ax,word ptr [bp-10]
	cmp	ax,word ptr [bp-12]
	jge	@@20
	jmp	@15@842
@@20:
@15@1682:
	cmp	word ptr [bp-12],0
	je	@@21
	jmp	@15@590
@@21:
   ;	
   ;	           }
   ;	       };
   ;	    };
   ;	   if (ansi) special_code(0,portnum);
   ;	
	cmp	word ptr [bp+8],0
	je	short @15@1766
	push	si
	xor	ax,ax
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
@15@1766:
   ;	
   ;	 };
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_print_file_to_cntrl	endp
   ;	
   ;	int is_abuf_empty(int id)
   ;	
	assume	cs:_TEXT
_is_abuf_empty	proc	near
	push	bp
	mov	bp,sp
	mov	cx,word ptr [bp+4]
   ;	
   ;	 {
   ;	   return (abuf_status[id].abufwrite == abuf_status[id].abufread);
   ;	
	mov	ax,cx
	mov	dx,24
	imul	dx
	mov	bx,ax
	mov	ax,word ptr DGROUP:_abuf_status[bx+18]
	mov	dx,word ptr DGROUP:_abuf_status[bx+16]
	push	ax
	mov	ax,cx
	mov	bx,24
	push	dx
	imul	bx
	mov	bx,ax
	pop	ax
	pop	dx
	cmp	dx,word ptr DGROUP:_abuf_status[bx+14]
	jne	short @16@114
	cmp	ax,word ptr DGROUP:_abuf_status[bx+12]
	jne	short @16@114
	mov	ax,1
	jmp	short @16@142
@16@114:
	xor	ax,ax
@16@142:
	jmp	short @16@170
@16@170:
   ;	
   ;	 };
   ;	
	pop	bp
	ret	
_is_abuf_empty	endp
   ;	
   ;	void aput_char(struct abuf_type *abuf, char c)
   ;	
	assume	cs:_TEXT
_aput_char	proc	near
	push	bp
	mov	bp,sp
	push	si
	mov	cx,word ptr [bp+4]
   ;	
   ;	 {
   ;	   *abuf->abufwrite++ = c;     /* write the character */
   ;	
	mov	bx,cx
	les	si,dword ptr [bx+16]
	inc	word ptr [bx+16]
	mov	al,byte ptr [bp+6]
	mov	byte ptr es:[si],al
   ;	
   ;	   if (abuf->abufwrite == abuf->abufend) /* if were at the end of the buffer */
   ;	
	mov	bx,cx
	mov	ax,word ptr [bx+18]
	mov	dx,word ptr [bx+16]
	mov	bx,cx
	cmp	ax,word ptr [bx+10]
	jne	short @17@114
	cmp	dx,word ptr [bx+8]
	jne	short @17@114
   ;	
   ;	     abuf->abufwrite = abuf->abuffer;   /* go back to the beginning */
   ;	
	mov	bx,cx
	mov	ax,word ptr [bx+6]
	mov	dx,word ptr [bx+4]
	mov	bx,cx
	mov	word ptr [bx+18],ax
	mov	word ptr [bx+16],dx
@17@114:
   ;	
   ;	   if (abuf->abufwrite == abuf->abufread)   /* if were at the the end of the */
   ;	
	mov	bx,cx
	mov	ax,word ptr [bx+18]
	mov	dx,word ptr [bx+16]
	mov	bx,cx
	cmp	ax,word ptr [bx+14]
	jne	short @17@254
	cmp	dx,word ptr [bx+12]
	jne	short @17@254
   ;	
   ;	    {                                       /* buffer, move the read pointer */
   ;	      abuf->abufread++;                     /* ahead a bit */
   ;	
	mov	bx,cx
	inc	word ptr [bx+12]
   ;	
   ;	      if (abuf->abufread == abuf->abufend)
   ;	
	mov	bx,cx
	mov	ax,word ptr [bx+14]
	mov	dx,word ptr [bx+12]
	mov	bx,cx
	cmp	ax,word ptr [bx+10]
	jne	short @17@254
	cmp	dx,word ptr [bx+8]
	jne	short @17@254
   ;	
   ;	        abuf->abufread = abuf->abuffer;
   ;	
	mov	bx,cx
	mov	ax,word ptr [bx+6]
	mov	dx,word ptr [bx+4]
	mov	bx,cx
	mov	word ptr [bx+14],ax
	mov	word ptr [bx+12],dx
@17@254:
   ;	
   ;	    };
   ;	 };
   ;	
	pop	si
	pop	bp
	ret	
_aput_char	endp
   ;	
   ;	void aput_into_buffer(int id, char *string, int channel, int parm1,
   ;	
	assume	cs:_TEXT
_aput_into_buffer	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
	mov	di,word ptr [bp+6]
   ;	
   ;	                      int parm2, int parm3, int parm4)
   ;	{
   ;	   struct abuf_type near *abuf = &abuf_status[id];
   ;	
	mov	ax,word ptr [bp+4]
	mov	dx,24
	imul	dx
	add	ax,offset DGROUP:_abuf_status
	mov	si,ax
   ;	
   ;	   int tempDS = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	
   ;	
   ;	//   _DS = FP_SEG(&abuf_status);
   ;	
   ;	   if (!abuf->active)
   ;	
	cmp	word ptr [si+2],0
	jne	short @18@86
   ;	
   ;	    {
   ;	      _DS = tempDS;
   ;	
   ;	
   ;	      return;
   ;	
	mov	ds,word ptr [bp-2]
	jmp	@18@338
@18@86:
	jmp	short @18@142
@18@114:
   ;	
   ;	    };                          /* dont post into buffer that isnt read */
   ;	
   ;	   disable();
   ;	   while (abuf->used)           /* wait for use of buffer */
   ;	    {
   ;	      enable();
   ;	
	db	251
   ;	
   ;	      next_task();
   ;	
	pushf	
	call	far ptr _next_task
   ;	
   ;	      disable();
   ;	
@18@142:
	db	250
	cmp	word ptr [si],0
	jne	short @18@114
   ;	
   ;	    };
   ;	   abuf->used = 1;
   ;	
	mov	word ptr [si],1
   ;	
   ;	   set_death_off();
   ;	
	call	near ptr _set_death_off
   ;	
   ;	   enable();
   ;	
	db	251
   ;	
   ;	
   ;	   aput_char(abuf,0xAA);        /* put our beginning message ID there */
   ;	
	mov	al,170
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,0x55);
   ;	
	mov	al,85
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,0xBC);
   ;	
	mov	al,188
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,tswitch);     /* put sender and channel # */
   ;	
	mov	al,byte ptr DGROUP:_tswitch
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,channel);
   ;	
	mov	al,byte ptr [bp+8]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,parm1);
   ;	
	mov	al,byte ptr [bp+10]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,parm2);
   ;	
	mov	al,byte ptr [bp+12]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,parm3);
   ;	
	mov	al,byte ptr [bp+14]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,parm4);
   ;	
	mov	al,byte ptr [bp+16]
	jmp	short @18@254
@18@226:
   ;	
   ;	
   ;	   while (*string) aput_char(abuf,*string++);   /* put the string into */
   ;	
	mov	bx,di
	inc	di
	mov	al,byte ptr [bx]
@18@254:
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
	cmp	byte ptr [di],0
	jne	short @18@226
   ;	
   ;	   aput_char(abuf,0);               /* the buffer with a zero */
   ;	
	mov	al,0
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   abuf->used = 0;              /* let someone else use it */
   ;	
	mov	word ptr [si],0
   ;	
   ;	   _DS = tempDS;
   ;	
   ;	
   ;	   set_death_on();
   ;	
	mov	ds,word ptr [bp-2]
	call	near ptr _set_death_on
@18@338:
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_aput_into_buffer	endp
   ;	
   ;	void aput_append_into_buffer(int id, int channel, int parm1,
   ;	
	assume	cs:_TEXT
_aput_append_into_buffer	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	push	di
   ;	
   ;	                      int parm2, int parm3, int parm4, int no_str, ...)
   ;	 {
   ;	   struct abuf_type near *abuf = &abuf_status[id];
   ;	
	mov	ax,word ptr [bp+4]
	mov	dx,24
	imul	dx
	add	ax,offset DGROUP:_abuf_status
	mov	si,ax
   ;	
   ;	   int tempDS = _DS;
   ;	
	mov	di,ds
   ;	
   ;	   va_list ap;
   ;	   char far *string;
   ;	
   ;	//   _DS = FP_SEG(&abuf_status);
   ;	
   ;	   if (!abuf->active)
   ;	
	cmp	word ptr [si+2],0
	jne	short @19@86
   ;	
   ;	    {
   ;	      _DS = tempDS;
   ;	
	mov	ds,di
   ;	
   ;	      return;
   ;	
	jmp	@19@394
@19@86:
	jmp	short @19@142
@19@114:
   ;	
   ;	    };                          /* dont post into buffer that isnt read */
   ;	
   ;	   disable();
   ;	   while (abuf->used)           /* wait for use of buffer */
   ;	    {
   ;	      enable();
   ;	
	db	251
   ;	
   ;	      next_task();
   ;	
	pushf	
	call	far ptr _next_task
   ;	
   ;	      disable();
   ;	
@19@142:
	db	250
	cmp	word ptr [si],0
	jne	short @19@114
   ;	
   ;	    };
   ;	   abuf->used = 1;
   ;	
	mov	word ptr [si],1
   ;	
   ;	   set_death_off();
   ;	
	call	near ptr _set_death_off
   ;	
   ;	   enable();
   ;	
	db	251
   ;	
   ;	
   ;	   aput_char(abuf,0xAA);        /* put our beginning message ID there */
   ;	
	mov	al,170
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,0x55);
   ;	
	mov	al,85
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,0xBC);
   ;	
	mov	al,188
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,tswitch);     /* put sender and channel # */
   ;	
	mov	al,byte ptr DGROUP:_tswitch
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,channel);
   ;	
	mov	al,byte ptr [bp+6]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,parm1);
   ;	
	mov	al,byte ptr [bp+8]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,parm2);
   ;	
	mov	al,byte ptr [bp+10]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,parm3);
   ;	
	mov	al,byte ptr [bp+12]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,parm4);
   ;	
	mov	al,byte ptr [bp+14]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	
   ;	   va_start(ap,no_str);
   ;	
	lea	ax,word ptr [bp+18]
	mov	word ptr [bp-2],ax
	jmp	short @19@338
@19@226:
   ;	
   ;	
   ;	   while (no_str>0)
   ;	   {
   ;	     string = (char far *)va_arg(ap,char far *);
   ;	
	add	word ptr [bp-2],4
	mov	bx,word ptr [bp-2]
	mov	ax,word ptr [bx-2]
	mov	dx,word ptr [bx-4]
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
	jmp	short @19@282
@19@254:
   ;	
   ;	     while (*string) aput_char(abuf,*string++);   /* put the string into */
   ;	
	les	bx,dword ptr [bp-6]
	inc	word ptr [bp-6]
	mov	al,byte ptr es:[bx]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
@19@282:
	les	bx,dword ptr [bp-6]
	cmp	byte ptr es:[bx],0
	jne	short @19@254
   ;	
   ;	     no_str--;
   ;	
	dec	word ptr [bp+16]
@19@338:
	cmp	word ptr [bp+16],0
	jg	short @19@226
   ;	
   ;	   }
   ;	
   ;	   va_end(ap);
   ;	
   ;	
   ;	
   ;	   aput_char(abuf,0);               /* the buffer with a zero */
   ;	
	mov	al,0
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   abuf->used = 0;              /* let someone else use it */
   ;	
	mov	word ptr [si],0
   ;	
   ;	   _DS = tempDS;
   ;	
	mov	ds,di
   ;	
   ;	   set_death_on();
   ;	
	call	near ptr _set_death_on
@19@394:
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_aput_append_into_buffer	endp
   ;	
   ;	void aput_vargs_into_buffer(int id, int channel, int parm1,
   ;	
	assume	cs:_TEXT
_aput_vargs_into_buffer	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	push	di
	mov	di,word ptr [bp+16]
   ;	
   ;	                      int parm2, int parm3, int parm4, int no_str,
   ;	                      va_list far *ap)
   ;	 {
   ;	   struct abuf_type near *abuf = &abuf_status[id];
   ;	
	mov	ax,word ptr [bp+4]
	mov	dx,24
	imul	dx
	add	ax,offset DGROUP:_abuf_status
	mov	si,ax
   ;	
   ;	   int tempDS = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	   char far *string;
   ;	
   ;	//   _DS = FP_SEG(&abuf_status);
   ;	
   ;	   if (!abuf->active)
   ;	
	cmp	word ptr [si+2],0
	jne	short @20@86
   ;	
   ;	    {
   ;	      _DS = tempDS;
   ;	
   ;	
   ;	      return;
   ;	
	mov	ds,word ptr [bp-2]
	jmp	@20@394
@20@86:
	jmp	short @20@142
@20@114:
   ;	
   ;	    };                          /* dont post into buffer that isnt read */
   ;	
   ;	   disable();
   ;	   while (abuf->used)           /* wait for use of buffer */
   ;	    {
   ;	      enable();
   ;	
	db	251
   ;	
   ;	      next_task();
   ;	
	pushf	
	call	far ptr _next_task
   ;	
   ;	      disable();
   ;	
@20@142:
	db	250
	cmp	word ptr [si],0
	jne	short @20@114
   ;	
   ;	    };
   ;	   abuf->used = 1;
   ;	
	mov	word ptr [si],1
   ;	
   ;	   set_death_off();
   ;	
	call	near ptr _set_death_off
   ;	
   ;	   enable();
   ;	
	db	251
   ;	
   ;	
   ;	   aput_char(abuf,0xAA);        /* put our beginning message ID there */
   ;	
	mov	al,170
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,0x55);
   ;	
	mov	al,85
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,0xBC);
   ;	
	mov	al,188
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,tswitch);     /* put sender and channel # */
   ;	
	mov	al,byte ptr DGROUP:_tswitch
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,channel);
   ;	
	mov	al,byte ptr [bp+6]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,parm1);
   ;	
	mov	al,byte ptr [bp+8]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,parm2);
   ;	
	mov	al,byte ptr [bp+10]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,parm3);
   ;	
	mov	al,byte ptr [bp+12]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   aput_char(abuf,parm4);
   ;	
	mov	al,byte ptr [bp+14]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
	jmp	short @20@338
@20@226:
   ;	
   ;	
   ;	   while (no_str>0)
   ;	   {
   ;	     string = (char far *)va_arg(ap,char far *);
   ;	
	add	word ptr [bp+18],4
	mov	bx,word ptr [bp+18]
	mov	ax,word ptr [bx-2]
	mov	dx,word ptr [bx-4]
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
	jmp	short @20@282
@20@254:
   ;	
   ;	     while (*string) aput_char(abuf,*string++);   /* put the string into */
   ;	
	les	bx,dword ptr [bp-6]
	inc	word ptr [bp-6]
	mov	al,byte ptr es:[bx]
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
@20@282:
	les	bx,dword ptr [bp-6]
	cmp	byte ptr es:[bx],0
	jne	short @20@254
   ;	
   ;	     no_str--;
   ;	
	dec	di
@20@338:
	or	di,di
	jg	short @20@226
   ;	
   ;	   }
   ;	
   ;	   aput_char(abuf,0);               /* the buffer with a zero */
   ;	
	mov	al,0
	push	ax
	push	si
	call	near ptr _aput_char
	pop	cx
	pop	cx
   ;	
   ;	   abuf->used = 0;              /* let someone else use it */
   ;	
	mov	word ptr [si],0
   ;	
   ;	   _DS = tempDS;
   ;	
   ;	
   ;	   set_death_on();
   ;	
	mov	ds,word ptr [bp-2]
	call	near ptr _set_death_on
@20@394:
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_aput_vargs_into_buffer	endp
   ;	
   ;	void aclear_buffer(int id)
   ;	
	assume	cs:_TEXT
_aclear_buffer	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	 {
   ;	   struct abuf_type *abuf = &abuf_status[id];
   ;	
	mov	ax,word ptr [bp+4]
	mov	dx,24
	imul	dx
	add	ax,offset DGROUP:_abuf_status
	mov	si,ax
   ;	
   ;	
   ;	   if (!abuf->active) return;
   ;	
	cmp	word ptr [si+2],0
	jne	short @21@86
	jmp	short @21@226
@21@86:
	jmp	short @21@142
@21@114:
   ;	
   ;	                                /* dont change buffer that isnt read */
   ;	
   ;	   disable();
   ;	   while (abuf->used)           /* wait for use of buffer */
   ;	    {
   ;	      enable();
   ;	
	db	251
   ;	
   ;	      next_task();
   ;	
	pushf	
	call	far ptr _next_task
   ;	
   ;	      disable();
   ;	
@21@142:
	db	250
	cmp	word ptr [si],0
	jne	short @21@114
   ;	
   ;	    };
   ;	   abuf->used = 1;
   ;	
	mov	word ptr [si],1
   ;	
   ;	   set_death_off();
   ;	
	call	near ptr _set_death_off
   ;	
   ;	   enable();
   ;	
	db	251
   ;	
   ;	   abuf->abufread = abuf->abufwrite;
   ;	
	mov	ax,word ptr [si+18]
	mov	dx,word ptr [si+16]
	mov	word ptr [si+14],ax
	mov	word ptr [si+12],dx
   ;	
   ;	   abuf->used = 0;              /* let someone else use it */
   ;	
	mov	word ptr [si],0
   ;	
   ;	
   ;	   set_death_on();
   ;	
	call	near ptr _set_death_on
@21@226:
   ;	
   ;	  };
   ;	
	pop	si
	pop	bp
	ret	
_aclear_buffer	endp
   ;	
   ;	unsigned char aget_char(struct abuf_type *abuf)
   ;	
	assume	cs:_TEXT
_aget_char	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	mov	cx,word ptr [bp+4]
   ;	
   ;	  {
   ;	    char temp;
   ;	
   ;	    if (abuf->abufread == abuf->abufwrite)
   ;	
	mov	bx,cx
	mov	ax,word ptr [bx+14]
	mov	dx,word ptr [bx+12]
	mov	bx,cx
	cmp	ax,word ptr [bx+18]
	jne	short @22@142
	cmp	dx,word ptr [bx+16]
	jne	short @22@142
   ;	
   ;	     return 0;
   ;	
	mov	al,0
@22@114:
	jmp	short @22@254
@22@142:
   ;	
   ;	    temp = *abuf->abufread++;
   ;	
	mov	bx,cx
	les	si,dword ptr [bx+12]
	inc	word ptr [bx+12]
	mov	al,byte ptr es:[si]
	mov	byte ptr [bp-1],al
   ;	
   ;	    if (abuf->abufread == abuf->abufend)
   ;	
	mov	bx,cx
	mov	ax,word ptr [bx+14]
	mov	dx,word ptr [bx+12]
	mov	bx,cx
	cmp	ax,word ptr [bx+10]
	jne	short @22@226
	cmp	dx,word ptr [bx+8]
	jne	short @22@226
   ;	
   ;	     abuf->abufread = abuf->abuffer;
   ;	
	mov	bx,cx
	mov	ax,word ptr [bx+6]
	mov	dx,word ptr [bx+4]
	mov	bx,cx
	mov	word ptr [bx+14],ax
	mov	word ptr [bx+12],dx
@22@226:
   ;	
   ;	    return(temp);
   ;	
	mov	al,byte ptr [bp-1]
	jmp	short @22@114
@22@254:
   ;	
   ;	  };
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_aget_char	endp
   ;	
   ;	int aget_abuffer(int *sentby, int *channel, char *string, int *parm1,
   ;	
	assume	cs:_TEXT
_aget_abuffer	proc	near
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
	mov	di,word ptr [bp+8]
   ;	
   ;	                 int *parm2, int *parm3,int *parm4)
   ;	  {
   ;	    struct abuf_type near *abuf = &abuf_status[tswitch];
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,24
	imul	dx
	add	ax,offset DGROUP:_abuf_status
	mov	si,ax
   ;	
   ;	    char temp;
   ;	    char abort = 0;
   ;	
	mov	byte ptr [bp-2],0
   ;	
   ;	    int tempDS = _DS;
   ;	
	mov	word ptr [bp-4],ds
   ;	
   ;	
   ;	    _DS = FP_SEG(&abuf_status);
   ;	
	mov	ax,ds
	mov	ds,ax
   ;	
   ;	
   ;	    if (!abuf->active)
   ;	
	cmp	word ptr [si+2],0
	jne	short @23@114
@23@58:
   ;	
   ;	     {
   ;	       _DS = tempDS;
   ;	
   ;	
   ;	       return 0;
   ;	
	mov	ds,word ptr [bp-4]
	xor	ax,ax
@23@86:
	jmp	@23@674
@23@114:
	jmp	short @23@170
@23@142:
   ;	
   ;	     };
   ;	    disable();
   ;	    while (abuf->used)
   ;	     {
   ;	       enable();
   ;	
	db	251
   ;	
   ;	       next_task();
   ;	
	pushf	
	call	far ptr _next_task
   ;	
   ;	       disable();
   ;	
@23@170:
	db	250
	cmp	word ptr [si],0
	jne	short @23@142
   ;	
   ;	     };
   ;	    abuf->used = 1;
   ;	
	mov	word ptr [si],1
   ;	
   ;	    set_death_off();
   ;	
	call	near ptr _set_death_off
   ;	
   ;	    enable();
   ;	
	db	251
   ;	
   ;	
   ;	    if (aget_char(abuf) != 0xAA) abort = 1;
   ;	
	push	si
	call	near ptr _aget_char
	pop	cx
	cmp	al,170
	je	short @23@282
	mov	byte ptr [bp-2],1
@23@282:
   ;	
   ;	    if (!abort) if (aget_char(abuf) != 0x55) abort = 1;
   ;	
	mov	al,byte ptr [bp-2]
	cbw	
	or	ax,ax
	jne	short @23@366
	push	si
	call	near ptr _aget_char
	pop	cx
	cmp	al,85
	je	short @23@366
	mov	byte ptr [bp-2],1
@23@366:
   ;	
   ;	    if (!abort) if (aget_char(abuf) != 0xBC) abort = 1;
   ;	
	mov	al,byte ptr [bp-2]
	cbw	
	or	ax,ax
	jne	short @23@450
	push	si
	call	near ptr _aget_char
	pop	cx
	cmp	al,188
	je	short @23@450
	mov	byte ptr [bp-2],1
@23@450:
   ;	
   ;	    if (abort)
   ;	
	cmp	byte ptr [bp-2],0
	je	short @23@562
	jmp	short @23@506
@23@506:
   ;	
   ;	     {
   ;	       while (aget_char(abuf));
   ;	
	push	si
	call	near ptr _aget_char
	pop	cx
	or	al,al
	jne	short @23@506
   ;	
   ;	       abuf->used = 0;
   ;	
	mov	word ptr [si],0
   ;	
   ;	       set_death_on();
   ;	
	call	near ptr _set_death_on
	jmp	short @23@58
@23@562:
   ;	
   ;	       _DS = tempDS;
   ;	       return 0;
   ;	     };
   ;	    *sentby = aget_char(abuf);
   ;	
	push	si
	call	near ptr _aget_char
	pop	cx
	mov	ah,0
	mov	bx,word ptr [bp+4]
	mov	word ptr [bx],ax
   ;	
   ;	    *channel = aget_char(abuf);
   ;	
	push	si
	call	near ptr _aget_char
	pop	cx
	mov	ah,0
	mov	bx,word ptr [bp+6]
	mov	word ptr [bx],ax
   ;	
   ;	    *parm1 = aget_char(abuf);
   ;	
	push	si
	call	near ptr _aget_char
	pop	cx
	mov	ah,0
	mov	bx,word ptr [bp+10]
	mov	word ptr [bx],ax
   ;	
   ;	    *parm2 = aget_char(abuf);
   ;	
	push	si
	call	near ptr _aget_char
	pop	cx
	mov	ah,0
	mov	bx,word ptr [bp+12]
	mov	word ptr [bx],ax
   ;	
   ;	    *parm3 = aget_char(abuf);
   ;	
	push	si
	call	near ptr _aget_char
	pop	cx
	mov	ah,0
	mov	bx,word ptr [bp+14]
	mov	word ptr [bx],ax
   ;	
   ;	    *parm4 = aget_char(abuf);
   ;	
	push	si
	call	near ptr _aget_char
	pop	cx
	mov	ah,0
	mov	bx,word ptr [bp+16]
	mov	word ptr [bx],ax
@23@590:
   ;	
   ;	    do
   ;	      {
   ;	        *string++ = (temp = aget_char(abuf));
   ;	
	push	si
	call	near ptr _aget_char
	pop	cx
	mov	byte ptr [bp-1],al
	mov	byte ptr [di],al
	inc	di
   ;	
   ;	      } while (temp);
   ;	
	cmp	byte ptr [bp-1],0
	jne	short @23@590
   ;	
   ;	    abuf->used = 0;
   ;	
	mov	word ptr [si],0
   ;	
   ;	
   ;	    _DS = tempDS;
   ;	
   ;	
   ;	    set_death_on();
   ;	
	mov	ds,word ptr [bp-4]
	call	near ptr _set_death_on
   ;	
   ;	    return 1;
   ;	
	mov	ax,1
	jmp	@23@86
@23@674:
   ;	
   ;	  };
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_aget_abuffer	endp
   ;	
   ;	int aback_abuffer(int id, int lines)
   ;	
	assume	cs:_TEXT
_aback_abuffer	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	push	di
	mov	di,word ptr [bp+6]
   ;	
   ;	  {
   ;	    struct abuf_type near *abuf = &abuf_status[id];
   ;	
	mov	ax,word ptr [bp+4]
	mov	dx,24
	imul	dx
	add	ax,offset DGROUP:_abuf_status
	mov	si,ax
   ;	
   ;	    char far *back_ptr;
   ;	    int tempDS = _DS;
   ;	
	mov	word ptr [bp-6],ds
   ;	
   ;	
   ;	    _DS = FP_SEG(&abuf_status);
   ;	
	mov	ax,ds
	mov	ds,ax
   ;	
   ;	
   ;	    if (!abuf->active)
   ;	
	cmp	word ptr [si+2],0
	jne	short @24@114
   ;	
   ;	     {
   ;	       _DS = tempDS;
   ;	
   ;	
   ;	       return 0;
   ;	
	mov	ds,word ptr [bp-6]
	xor	ax,ax
@24@86:
	jmp	short @24@478
@24@114:
	jmp	short @24@170
@24@142:
   ;	
   ;	     };
   ;	    disable();
   ;	    while (abuf->used)
   ;	     {
   ;	       enable();
   ;	
	db	251
   ;	
   ;	       next_task();
   ;	
	pushf	
	call	far ptr _next_task
   ;	
   ;	       disable();
   ;	
@24@170:
	db	250
	cmp	word ptr [si],0
	jne	short @24@142
   ;	
   ;	     };
   ;	    abuf->used = 1;
   ;	
	mov	word ptr [si],1
   ;	
   ;	    set_death_off();
   ;	
	call	near ptr _set_death_off
   ;	
   ;	    enable();
   ;	
	db	251
   ;	
   ;	
   ;	    lines += 2;
   ;	
	add	di,2
   ;	
   ;	
   ;	    back_ptr = abuf->abufwrite;
   ;	
	mov	ax,word ptr [si+18]
	mov	dx,word ptr [si+16]
	mov	word ptr [bp-2],ax
	mov	word ptr [bp-4],dx
	jmp	short @24@366
@24@254:
   ;	
   ;	
   ;	    while ((lines>0) && (back_ptr != abuf->abufread))
   ;	     {
   ;	       back_ptr--;
   ;	
	dec	word ptr [bp-4]
   ;	
   ;	       if (back_ptr < abuf->abuffer) back_ptr = abuf->abufend-1;
   ;	
	mov	ax,word ptr [si+4]
	cmp	ax,word ptr [bp-4]
	jbe	short @24@310
	mov	ax,word ptr [si+10]
	mov	dx,word ptr [si+8]
	dec	dx
	mov	word ptr [bp-2],ax
	mov	word ptr [bp-4],dx
@24@310:
   ;	
   ;	       if (!(*back_ptr)) lines--;
   ;	
	les	bx,dword ptr [bp-4]
	mov	al,byte ptr es:[bx]
	cbw	
	or	ax,ax
	jne	short @24@366
	dec	di
@24@366:
	or	di,di
	jle	short @24@450
	mov	ax,word ptr [si+14]
	mov	dx,word ptr [si+12]
	cmp	ax,word ptr [bp-2]
	jne	short @24@254
	cmp	dx,word ptr [bp-4]
	jne	short @24@254
@24@450:
   ;	
   ;	     };
   ;	
   ;	    abuf->abufread = back_ptr;
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	mov	word ptr [si+14],ax
	mov	word ptr [si+12],dx
   ;	
   ;	
   ;	    abuf->used = 0;
   ;	
	mov	word ptr [si],0
   ;	
   ;	
   ;	    _DS = tempDS;
   ;	
   ;	
   ;	    set_death_on();
   ;	
	mov	ds,word ptr [bp-6]
	call	near ptr _set_death_on
   ;	
   ;	    return 1;
   ;	
	mov	ax,1
	jmp	short @24@86
@24@478:
   ;	
   ;	  };
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_aback_abuffer	endp
   ;	
   ;	void initabuffer(int bufsize)
   ;	
	assume	cs:_TEXT
_initabuffer	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	di,word ptr [bp+4]
   ;	
   ;	 {
   ;	   struct abuf_type *abuf = &abuf_status[tswitch];
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,24
	imul	dx
	add	ax,offset DGROUP:_abuf_status
	mov	si,ax
   ;	
   ;	
   ;	   abuf->used = 0;
   ;	
	mov	word ptr [si],0
   ;	
   ;	   lock_dos(504);
   ;	
	mov	ax,504
	push	ax
	call	near ptr _lock_dos
	pop	cx
   ;	
   ;	   abuf->abuffer = g_malloc_main_only(bufsize,"ABUFFER");
   ;	
	mov	ax,offset DGROUP:s@+538
	push	ax
	mov	ax,di
	cwd	
	push	dx
	push	ax
	call	near ptr _g_malloc_main_only
	add	sp,6
	mov	word ptr [si+6],ds
	mov	word ptr [si+4],ax
   ;	
   ;	   unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	   if (!abuf->abuffer) return;
   ;	
	mov	ax,word ptr [si+4]
	or	ax,word ptr [si+6]
	jne	short @25@86
	jmp	short @25@114
@25@86:
   ;	
   ;	   abuf->abufend = abuf->abuffer + bufsize;
   ;	
	mov	ax,word ptr [si+6]
	mov	dx,word ptr [si+4]
	add	dx,di
	mov	word ptr [si+10],ax
	mov	word ptr [si+8],dx
   ;	
   ;	   abuf->abufread = abuf->abuffer;
   ;	
	mov	ax,word ptr [si+6]
	mov	dx,word ptr [si+4]
	mov	word ptr [si+14],ax
	mov	word ptr [si+12],dx
   ;	
   ;	   abuf->abufwrite = abuf->abuffer;
   ;	
	mov	ax,word ptr [si+6]
	mov	dx,word ptr [si+4]
	mov	word ptr [si+18],ax
	mov	word ptr [si+16],dx
   ;	
   ;	   abuf->max_buffer = bufsize - 1;
   ;	
	mov	ax,di
	dec	ax
	mov	word ptr [si+22],ax
   ;	
   ;	   abuf->num_buffer = 0;
   ;	
	mov	word ptr [si+20],0
   ;	
   ;	   abuf->active = 1;
   ;	
	mov	word ptr [si+2],1
@25@114:
   ;	
   ;	 };
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_initabuffer	endp
   ;	
   ;	void dealloc_abuf(int portnum)
   ;	
	assume	cs:_TEXT
_dealloc_abuf	proc	near
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	
   ;	   abuf_status[portnum].active=0;
   ;	
	mov	ax,si
	mov	dx,24
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_abuf_status[bx+2],0
   ;	
   ;	   if (abuf_status[portnum].abuffer)
   ;	
	mov	ax,si
	mov	dx,24
	imul	dx
	mov	bx,ax
	mov	ax,word ptr DGROUP:_abuf_status[bx+4]
	or	ax,word ptr DGROUP:_abuf_status[bx+6]
	je	short @26@86
   ;	
   ;	    {
   ;	      lock_dos(505);
   ;	
	mov	ax,505
	push	ax
	call	near ptr _lock_dos
	pop	cx
   ;	
   ;	      g_free(abuf_status[portnum].abuffer);
   ;	
	mov	ax,si
	mov	dx,24
	imul	dx
	mov	bx,ax
	push	word ptr DGROUP:_abuf_status[bx+4]
	call	near ptr _g_free
	pop	cx
   ;	
   ;	      abuf_status[portnum].abuffer = NULL;
   ;	
	mov	ax,si
	mov	dx,24
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_abuf_status[bx+6],0
	mov	word ptr DGROUP:_abuf_status[bx+4],0
   ;	
   ;	      unlock_dos();
   ;	
	call	near ptr _unlock_dos
@26@86:
   ;	
   ;	    }
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_dealloc_abuf	endp
   ;	
   ;	int get_string_cntrl(char *string, int limit, char echo, char back_to_end,
   ;	
	assume	cs:_TEXT
_get_string_cntrl	proc	near
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
   ;	
   ;	                      char escape, char noblankline, char cr_on_blankline,
   ;	                      char upcase, char onlynum)
   ;	 {
   ;	   int pos = 0;
   ;	
	xor	si,si
   ;	
   ;	   int key;
   ;	   int flag = 1;
   ;	
	mov	di,1
   ;	
   ;	   int reason=0;
   ;	
	mov	word ptr [bp-4],0
	jmp	@27@982
@27@58:
   ;	
   ;	
   ;	   while (flag)             /* wait while editing */
   ;	    {
   ;	      key = wait_ch();
   ;	
	call	near ptr _wait_ch
	mov	word ptr [bp-2],ax
   ;	
   ;	      if (((key == 8) || (key == 127)))
   ;	
	cmp	word ptr [bp-2],8
	je	short @27@114
	cmp	word ptr [bp-2],127
	jne	short @27@226
@27@114:
   ;	
   ;	      if (pos>0)
   ;	
	or	si,si
	jle	short @27@170
   ;	
   ;	       {
   ;	         pos--;         /* if an edit key is pressed and there's more to */
   ;	
	dec	si
   ;	
   ;	         print_string(backspacestring);   /* erase, erase the character */
   ;	
	mov	ax,offset DGROUP:_backspacestring
	push	ax
	call	near ptr _print_string
	pop	cx
   ;	
   ;	       }               /* and go back one */
   ;	
	jmp	short @27@226
@27@170:
   ;	
   ;	      else
   ;	       if (back_to_end) { flag = 0; reason=1; }
   ;	
	cmp	byte ptr [bp+10],0
	je	short @27@226
	xor	di,di
	mov	word ptr [bp-4],1
@27@226:
   ;	
   ;	
   ;	      if ((key == 27) && escape)  /* if we abort, then clear all characters */
   ;	
	cmp	word ptr [bp-2],27
	jne	short @27@338
	cmp	byte ptr [bp+12],0
	je	short @27@338
   ;	
   ;	       {
   ;	         flag = 0;
   ;	
	xor	di,di
   ;	
   ;	         reason=2;
   ;	
	mov	word ptr [bp-4],2
   ;	
   ;	         if (pos)       /* print a backslash to indicate abort */
   ;	
	or	si,si
	je	short @27@338
   ;	
   ;	          {
   ;	            print_chr('\\');
   ;	
	mov	al,92
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            print_chr(13);
   ;	
	mov	al,13
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            print_chr(10);
   ;	
	mov	al,10
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            pos = 0;
   ;	
	xor	si,si
@27@338:
   ;	
   ;	          };
   ;	       };
   ;	      if (key == 13)    /* finish the line */
   ;	
	cmp	word ptr [bp-2],13
	jne	short @27@506
   ;	
   ;	       {
   ;	         if ((!noblankline) || (pos))
   ;	
	mov	al,byte ptr [bp+14]
	cbw	
	or	ax,ax
	je	short @27@422
	or	si,si
	je	short @27@506
@27@422:
   ;	
   ;	          {
   ;	            flag = 0;
   ;	
	xor	di,di
   ;	
   ;	            if ((pos) || (cr_on_blankline))
   ;	
	or	si,si
	jne	short @27@478
	cmp	byte ptr [bp+16],0
	je	short @27@506
@27@478:
   ;	
   ;	             {
   ;	              print_chr(13);
   ;	
	mov	al,13
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	              print_chr(10);
   ;	
	mov	al,10
	push	ax
	call	near ptr _print_chr
	pop	cx
@27@506:
   ;	
   ;	             };
   ;	          };
   ;	       };
   ;	      if (((key >= 32) && (key <= 126)) && (pos < limit))
   ;	
	cmp	word ptr [bp-2],32
	jl	short @27@982
	cmp	word ptr [bp-2],126
	jg	short @27@982
	cmp	si,word ptr [bp+6]
	jge	short @27@982
   ;	
   ;	       {
   ;	         if ((upcase) && (key>='a') && (key<='z')) key -= 32;
   ;	
	cmp	byte ptr [bp+18],0
	je	short @27@702
	cmp	word ptr [bp-2],97
	jl	short @27@702
	cmp	word ptr [bp-2],122
	jg	short @27@702
	sub	word ptr [bp-2],32
@27@702:
   ;	
   ;	         if (!((onlynum) && ((key<'0') || (key>'9'))))
   ;	
	cmp	byte ptr [bp+20],0
	je	short @27@786
	cmp	word ptr [bp-2],48
	jl	short @27@982
	cmp	word ptr [bp-2],57
	jg	short @27@982
@27@786:
   ;	
   ;	          {
   ;	                                /* insert the character if there's room */
   ;	           *(string+pos) = key;
   ;	
	mov	bx,word ptr [bp+4]
	add	bx,si
	mov	al,byte ptr [bp-2]
	mov	byte ptr [bx],al
   ;	
   ;	           if (key == '+')        /* if +, don't let it be typed normally */
   ;	
	cmp	word ptr [bp-2],43
	jne	short @27@842
   ;	
   ;	            {
   ;	              print_chr(key);     /* print the character with a space */
   ;	
	mov	al,byte ptr [bp-2]
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	              print_chr(32);      /* and a backspace */
   ;	
	mov	al,32
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	              print_chr(8);
   ;	
	mov	al,8
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            }
   ;	
	jmp	short @27@954
@27@842:
   ;	
   ;	            else
   ;	            if (echo) print_chr(echo);
   ;	
	cmp	byte ptr [bp+8],0
	je	short @27@898
	mov	al,byte ptr [bp+8]
	jmp	short @27@926
@27@898:
   ;	
   ;	             else print_chr(key);       /* otherwise, print it normally */
   ;	
	mov	al,byte ptr [bp-2]
@27@926:
	push	ax
	call	near ptr _print_chr
	pop	cx
@27@954:
   ;	
   ;	           pos++;
   ;	
	inc	si
@27@982:
	or	di,di
	je	@@22
	jmp	@27@58
@@22:
   ;	
   ;	          };
   ;	       };
   ;	    };
   ;	   *(string+pos) = 0;           /* mark end of the string */
   ;	
	mov	bx,word ptr [bp+4]
	add	bx,si
	mov	byte ptr [bx],0
   ;	
   ;	   return reason;
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @27@1038
@27@1038:
   ;	
   ;	 };
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_get_string_cntrl	endp
   ;	
   ;	void check_for_privates(void)
   ;	
	assume	cs:_TEXT
_check_for_privates	proc	near
	push	bp
	mov	bp,sp
	sub	sp,462
	push	si
	push	di
   ;	
   ;	 {
   ;	   int not_print_priv = 1;
   ;	
	mov	di,1
	jmp	short @28@562
@28@58:
   ;	
   ;	   int channel, id, type, code1, code2,code3;
   ;	   int print_mesg;
   ;	   char s[STRING_SIZE];
   ;	   while (aget_abuffer(&id,&channel,s,&type,&code1,&code2,&code3))
   ;	    {
   ;	       print_mesg=0;
   ;	
	xor	si,si
   ;	
   ;	
   ;	       switch (type)
   ;	
	mov	bx,word ptr [bp-6]
	dec	bx
	cmp	bx,11
	ja	short @28@450
	shl	bx,1
	jmp	word ptr cs:@28@C626[bx]
@28@170:
   ;	
   ;	       {
   ;	       case 1 :  print_mesg++;  /* privates */
   ;	
	inc	si
   ;	
   ;	                 break;
   ;	
	jmp	short @28@450
@28@198:
   ;	
   ;	       case 5 :  if (code2==7) print_mesg++;   /* system messsage */
   ;	
	cmp	word ptr [bp-10],7
	jne	short @28@254
	inc	si
@28@254:
   ;	
   ;	                 break;
   ;	
	jmp	short @28@450
@28@282:
   ;	
   ;	       case 7 :  print_mesg++;  /* user alert mesg / timeout */
   ;	
	jmp	short @28@170
@28@310:
   ;	
   ;	                 break;
   ;	       case 8 :  if (code3!=8)
   ;	
	cmp	word ptr [bp-12],8
	je	short @28@366
   ;	
   ;	                    print_mesg++;  /* personal system message */
   ;	
	inc	si
@28@366:
   ;	
   ;	                 break;
   ;	
	jmp	short @28@450
@28@394:
   ;	
   ;	       case 10:  print_mesg++;  /* link privates */
   ;	
	jmp	short @28@170
@28@422:
	jmp	short @28@170
@28@450:
   ;	
   ;	                 break;
   ;	       case 12:  print_mesg++;
   ;	                 break;
   ;	      }
   ;	
   ;	      if (print_mesg)
   ;	
	or	si,si
	je	short @28@562
   ;	
   ;	       {
   ;	       if (not_print_priv)
   ;	
	or	di,di
	je	short @28@534
   ;	
   ;	          {
   ;	            not_print_priv = 0;
   ;	
	xor	di,di
   ;	
   ;	            print_chr(7);
   ;	
	mov	al,7
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	            print_cr();
   ;	
	call	near ptr _print_cr
@28@534:
   ;	
   ;	          };
   ;	
   ;	         wrap_line(s);
   ;	
	lea	ax,word ptr [bp-462]
	push	ax
	call	near ptr _wrap_line
	pop	cx
@28@562:
	lea	ax,word ptr [bp-12]
	push	ax
	lea	ax,word ptr [bp-10]
	push	ax
	lea	ax,word ptr [bp-8]
	push	ax
	lea	ax,word ptr [bp-6]
	push	ax
	lea	ax,word ptr [bp-462]
	push	ax
	lea	ax,word ptr [bp-2]
	push	ax
	lea	ax,word ptr [bp-4]
	push	ax
	call	near ptr _aget_abuffer
	add	sp,14
	or	ax,ax
	jne	short @28@58
   ;	
   ;	       }
   ;	    }
   ;	 };
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_check_for_privates	endp
@28@C626	label	word
	dw	@28@170
	dw	@28@450
	dw	@28@450
	dw	@28@450
	dw	@28@198
	dw	@28@450
	dw	@28@282
	dw	@28@310
	dw	@28@450
	dw	@28@394
	dw	@28@450
	dw	@28@422
	?debug	C E9
	?debug	C FA00000000
_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'[ Press Return ]'
	db	0
	db	'Command not available.'
	db	0
	db	'Command not available.'
	db	0
	db	'NO CARRIER'
	db	0
	db	'CONNECT'
	db	0
	db	'NO CARRIER'
	db	0
	db	'CONNECT'
	db	0
	db	'ENTER.LOG'
	db	0
	db	'a'
	db	0
	db	'IO#1'
	db	0
	db	'ENTER.LOG'
	db	0
	db	'(%02d,%03d) %s'
	db	10
	db	0
	db	'Date: %s'
	db	0
	db	10
	db	0
	db	'%s'
	db	10
	db	0
	db	'TEMP'
	db	0
	db	'%s'
	db	10
	db	0
	db	'Error: %s'
	db	0
	db	'error.log'
	db	0
	db	'a'
	db	0
	db	'IO#2'
	db	0
	db	'%s'
	db	0
	db	'Date: %s'
	db	10
	db	0
	db	'a'
	db	0
	db	'IO#3'
	db	0
	db	'#%03d (%02d) | '
	db	0
	db	'%a %b %d %Y %I:%M:%S %p'
	db	0
	db	'%s | '
	db	0
	db	'%s'
	db	10
	db	0
	db	'LOG\KILL.LOG'
	db	0
	db	'a'
	db	0
	db	'IO#3'
	db	0
	db	'LINK'
	db	0
	db	'%GST'
	db	0
	db	'#%03d'
	db	0
	db	'(%02d) %s:%c%s%c'
	db	0
	db	'LINK'
	db	0
	db	'%GST'
	db	0
	db	'#%03d'
	db	0
	db	'(%02d) %s:%c%s%c'
	db	0
	db	'%m/%d/%y %I:%M:%S %p'
	db	0
	db	'%-28s %-28s %-18s'
	db	0
	db	'Last %d kills'
	db	0
	db	'TEXT\KILL.HDR'
	db	0
	db	'LOG\KILL.LOG'
	db	0
	db	'rb'
	db	0
	db	'FILES#4'
	db	0
	db	'LOG\KILL.LOG'
	db	0
	db	'.ANS'
	db	0
	db	'rb'
	db	0
	db	'IO#3'
	db	0
	db	'rb'
	db	0
	db	'IO#3'
	db	0
	db	'--> Aborted'
	db	0
	db	'.ANS'
	db	0
	db	'rb'
	db	0
	db	'IO#3'
	db	0
	db	'rb'
	db	0
	db	'IO#4'
	db	0
	db	'[ Press Return ]'
	db	0
	db	'--> Aborted'
	db	0
	db	'ABUFFER'
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	public	_aget_char
	extrn	N_LXMUL@:far
	extrn	_limit_carrots:near
	extrn	_remove_flashing:near
	extrn	_remove_ansi:near
	extrn	_print_invalid_command:near
	extrn	_exec:near
	extrn	_user_options:word
	extrn	_user_lines:word
	extrn	_test_bit:near
	extrn	_remake_handleline:near
	extrn	_set_death_on:near
	extrn	_set_death_off:near
	extrn	_clear_call_on_logoff:near
	extrn	_leave:near
	extrn	_line_status:word
	extrn	_abuf_status:word
	extrn	_backspacestring:byte
	public	_do_page_break
	public	_last_ten_kills
	public	_log_kill
	public	_convstring
	public	_mark_user_log_on
	public	_check_for_privates
	public	_print_file_to_cntrl
	public	_get_string_cntrl
	public	_dealloc_abuf
	public	_initabuffer
	public	_aback_abuffer
	public	_aclear_buffer
	public	_get_no_echo
	public	_print_file_to
	public	_print_file
	public	_is_abuf_empty
	public	_aput_char
	public	_aput_vargs_into_buffer
	public	_aput_append_into_buffer
	public	_aput_into_buffer
	public	_aget_abuffer
	public	_get_hot_key_prompt
	public	_get_string
	public	_prompt_get_string
	public	_wait_ch
	public	_log_event
	public	_log_error
	extrn	_is_a_console:byte
	extrn	_cur_video_state:near
	extrn	_wrap_line:near
	extrn	_special_code:near
	extrn	_print_cr:near
	extrn	_print_str_cr:near
	extrn	_print_string:near
	extrn	_print_chr_to:near
	extrn	_print_chr:near
	extrn	_a_empty_outbuffer:word
	extrn	_a_wait_for_xmit:word
	extrn	_a_get_nchar:word
	extrn	_a_get_first_char:word
	extrn	_a_dcd_detect:word
	extrn	_g_fclose:near
	extrn	_g_fopen:near
	extrn	_g_free:near
	extrn	_g_malloc_main_only:near
	extrn	_unlock_dos:near
	extrn	_lock_dos:near
	extrn	_next_task:far
	extrn	_islocked:near
	extrn	_tasking:byte
	extrn	_tswitch:word
	extrn	_strftime:near
	extrn	_time:near
	extrn	_localtime:near
	extrn	_ctime:near
	extrn	_strrchr:near
	extrn	_strncpy:near
	extrn	_strncmp:near
	extrn	_strlen:near
	extrn	_strcpy:near
	extrn	_strcat:near
	extrn	_toupper:near
	extrn	__strerror:near
	extrn	_sprintf:near
	extrn	_printf:near
	extrn	_perror:near
	extrn	_ftell:near
	extrn	_fseek:near
	extrn	_fread:near
	extrn	_fprintf:near
_s@	equ	s@
	end
