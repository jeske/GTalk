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
	?debug	C E94E526A1B04696F2E63
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
IO_TEXT	segment byte public 'CODE'
IO_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:IO_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
IO_TEXT	segment byte public 'CODE'
   ;	
   ;	unsigned int convstring(char *string, int encrypt)
   ;	
	assume	cs:IO_TEXT
_convstring	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	mov	si,word ptr [bp+10]
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
	jmp	short @1@170
@1@58:
   ;	
   ;	
   ;	    while (*string)
   ;	    {
   ;	       if (encrypt)
   ;	
	or	si,si
	je	short @1@114
   ;	
   ;	       {
   ;	           checksum += (unsigned int) *string;
   ;	
	les	bx,dword ptr [bp+6]
	mov	al,byte ptr es:[bx]
	mov	bl,al
	cbw	
	add	cx,ax
   ;	
   ;	           prev = *string = circleplus(*string,prev);
   ;	
	mov	al,bl
	mov	dl,byte ptr [bp-1]
	and	dl,7
	xor	al,dl
	mov	bx,word ptr [bp+6]
	mov	byte ptr es:[bx],al
	mov	byte ptr [bp-1],al
   ;	
   ;	       }
   ;	
	jmp	short @1@142
@1@114:
   ;	
   ;	       else
   ;	       {
   ;	           temp = *string;
   ;	
	les	bx,dword ptr [bp+6]
	mov	al,byte ptr es:[bx]
	mov	bl,al
	mov	byte ptr [bp-2],al
   ;	
   ;	           *string = circleplus(*string,prev);
   ;	
	mov	dl,byte ptr [bp-1]
	and	dl,7
	xor	al,dl
	mov	bx,word ptr [bp+6]
	mov	byte ptr es:[bx],al
   ;	
   ;	           checksum += (unsigned int) *string;
   ;	
	mov	al,byte ptr es:[bx]
	cbw	
	add	cx,ax
   ;	
   ;	           prev = temp;
   ;	
	mov	al,byte ptr [bp-2]
	mov	byte ptr [bp-1],al
@1@142:
   ;	
   ;	       };
   ;	       string++;
   ;	
	inc	word ptr [bp+6]
@1@170:
	les	bx,dword ptr [bp+6]
	cmp	byte ptr es:[bx],0
	jne	short @1@58
   ;	
   ;	    };
   ;	    return(checksum);
   ;	
	mov	ax,cx
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
	assume	cs:IO_TEXT
_wait_ch	proc	far
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
	mov	cl,2
	shl	bx,cl
	call	dword ptr DGROUP:_a_dcd_detect[bx]
	add	sp,2
	or	ax,ax
	jne	short @2@114
	call	far ptr _leave
@2@114:
   ;	
   ;	      ischar=int_char(portnum);              /* is there a character ready? */
   ;	
	push	si
	mov	bx,si
	mov	cl,2
	shl	bx,cl
	call	dword ptr DGROUP:_a_get_nchar[bx]
	add	sp,2
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
	assume	cs:IO_TEXT
_do_page_break	proc	far
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
	push	ds
	mov	ax,offset DGROUP:s@
	push	ax
	call	far ptr _print_string
	add	sp,4
@3@58:
   ;	
   ;	    do
   ;	     {
   ;	       count = wait_ch();
   ;	
	push	cs
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
@3@198:
   ;	
   ;	      print_string(backspacestring);
   ;	
	push	ds
	mov	ax,offset DGROUP:_backspacestring
	push	ax
	call	far ptr _print_string
	add	sp,4
	inc	si
	cmp	si,16
	jl	short @3@198
   ;	
   ;	
   ;	  return (abort);
   ;	
	mov	ax,di
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	ret	
_do_page_break	endp
   ;	
   ;	void get_no_echo(char *string, int limit)
   ;	
	assume	cs:IO_TEXT
_get_no_echo	proc	far
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+10]
	jmp	short @4@226
@4@58:
   ;	
   ;	 {
   ;	   char ch;
   ;	   while (limit>0)
   ;	    {
   ;	      ch = wait_ch();
   ;	
	push	cs
	call	near ptr _wait_ch
	mov	dl,al
   ;	
   ;	      if (ch == 13) limit=0;
   ;	
	cmp	dl,13
	jne	short @4@114
	xor	si,si
	jmp	short @4@226
@4@114:
   ;	
   ;	       else if ((ch>=32) && (ch<=126) && (limit>1))
   ;	
	cmp	dl,32
	jl	short @4@226
	cmp	dl,126
	jg	short @4@226
	cmp	si,1
	jle	short @4@226
   ;	
   ;	        {
   ;	          *string++ = ch;
   ;	
	les	bx,dword ptr [bp+6]
	mov	byte ptr es:[bx],dl
	inc	word ptr [bp+6]
   ;	
   ;	          limit--;
   ;	
	dec	si
@4@226:
	or	si,si
	jg	short @4@58
   ;	
   ;	        };
   ;	    };
   ;	   *string=0;
   ;	
	les	bx,dword ptr [bp+6]
	mov	byte ptr es:[bx],0
   ;	
   ;	 };
   ;	
	pop	si
	pop	bp
	ret	
_get_no_echo	endp
   ;	
   ;	int get_hot_key_prompt(char *prompt,char *chars_allowed,char def,char commands)
   ;	
	assume	cs:IO_TEXT
_get_hot_key_prompt	proc	far
	push	bp
	mov	bp,sp
	sub	sp,454
	push	si
	push	di
   ;	
   ;	{
   ;	   int key_pr=1;
   ;	
	mov	si,1
   ;	
   ;	   int key;
   ;	   int action;
   ;	   int bkspchr=8;
   ;	
	mov	word ptr [bp-4],8
   ;	
   ;	
   ;	   special_code(1,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	ax,1
	push	ax
	call	far ptr _special_code
	add	sp,4
   ;	
   ;	   print_string(prompt);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	far ptr _print_string
	add	sp,4
   ;	
   ;	   special_code(0,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	xor	ax,ax
	push	ax
	call	far ptr _special_code
	add	sp,4
   ;	
   ;	
   ;	   print_chr(def);
   ;	
	mov	al,byte ptr [bp+14]
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	   print_chr(bkspchr);
   ;	
	mov	al,byte ptr [bp-4]
	push	ax
	call	far ptr _print_chr
	add	sp,2
	jmp	@5@1262
@5@58:
   ;	
   ;	
   ;	   while (key_pr)
   ;	     {
   ;	       key = wait_ch();
   ;	
	push	cs
	call	near ptr _wait_ch
	mov	word ptr [bp-2],ax
   ;	
   ;	       if (key>'Z') key -= 32;
   ;	
	cmp	word ptr [bp-2],90
	jle	short @5@114
	sub	word ptr [bp-2],32
@5@114:
   ;	
   ;	
   ;	       if (key==13 || key==11)
   ;	
	cmp	word ptr [bp-2],13
	je	short @5@170
	cmp	word ptr [bp-2],11
	jne	short @5@198
@5@170:
   ;	
   ;	          key=toupper(def);
   ;	
	mov	al,byte ptr [bp+14]
	cbw	
	push	ax
	call	far ptr _toupper
	add	sp,2
	mov	word ptr [bp-2],ax
@5@198:
   ;	
   ;	
   ;	      if (key=='/' && commands)  // if they entered / and they WANT commands
   ;	
	cmp	word ptr [bp-2],47
	je	@@0
	jmp	@5@1150
@@0:
	cmp	byte ptr [bp+16],0
	jne	@@1
	jmp	@5@1150
@@1:
   ;	
   ;	       {      /* IF they did /p we need to start getting all the others */
   ;	         char s2[STRING_SIZE];
   ;	         print_chr('/');
   ;	
	mov	al,47
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	
   ;	         switch(get_string_cntrl(s2,STRING_SIZE-300,0,1,1,0,0,0,0))
   ;	
	mov	al,0
	push	ax
	push	ax
	push	ax
	push	ax
	mov	al,1
	push	ax
	push	ax
	mov	al,0
	push	ax
	mov	ax,150
	push	ax
	push	ss
	lea	ax,word ptr [bp-454]
	push	ax
	call	far ptr _get_string_cntrl
	add	sp,20
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
	mov	dx,1
   ;	
   ;	                    break;
   ;	
	jmp	short @5@534
@5@450:
   ;	
   ;	           case 1 : action=2;
   ;	
	mov	dx,2
   ;	
   ;	                    break;
   ;	
	jmp	short @5@534
@5@478:
   ;	
   ;	           case 2 : action=3;
   ;	
	mov	dx,3
   ;	
   ;	                    break;
   ;	
	jmp	short @5@534
@5@506:
   ;	
   ;	           default: action=3;
   ;	
	mov	dx,3
   ;	
   ;	                    break;
   ;	
@5@534:
   ;	
   ;	         }
   ;	
   ;	         switch(action)
   ;	
	mov	ax,dx
	cmp	ax,1
	je	short @5@674
	cmp	ax,2
	jne	@@2
	jmp	@5@1094
@@2:
	cmp	ax,3
	jne	@@3
	jmp	@5@1122
@@3:
	jmp	@5@1262
@5@674:
   ;	
   ;	         {
   ;	         case 1:
   ;	          {
   ;	
   ;	            print_cr();
   ;	
	call	far ptr _print_cr
   ;	
   ;	
   ;	               if (!test_bit(user_options[tswitch].privs,CAN_TYPE_ANSI_PRV))
   ;	
	mov	ax,39
	push	ax
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,178
	imul	dx
	mov	dx,seg _user_options
	add	ax,offset _user_options+2
	push	dx
	push	ax
	call	far ptr _test_bit
	add	sp,6
	or	ax,ax
	jne	short @5@730
   ;	
   ;	                  remove_ansi(s2);
   ;	
	push	ss
	lea	ax,word ptr [bp-454]
	push	ax
	call	far ptr _remove_ansi
	add	sp,4
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
	mov	dx,seg _user_options
	add	ax,offset _user_options+2
	push	dx
	push	ax
	call	far ptr _test_bit
	add	sp,6
	or	ax,ax
	jne	short @5@786
   ;	
   ;	                  remove_flashing(s2);
   ;	
	push	ss
	lea	ax,word ptr [bp-454]
	push	ax
	call	far ptr _remove_flashing
	add	sp,4
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
	push	ss
	lea	ax,word ptr [bp-454]
	push	ax
	call	far ptr _limit_carrots
	add	sp,6
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
	push	ds
	push	ax
	push	ss
	lea	ax,word ptr [bp-454]
	push	ax
	call	far ptr _exec
	add	sp,12
	cmp	ax,1
	je	short @5@954
	cmp	ax,2
	je	short @5@982
	jmp	short @5@1010
@5@954:
   ;	
   ;	            {
   ;	                case 0 :  break;
   ;	                case 1 :  print_invalid_command();
   ;	
	call	far ptr _print_invalid_command
   ;	
   ;	                          break;
   ;	
	jmp	short @5@1010
@5@982:
   ;	
   ;	                case 2 :  print_str_cr("Command not available.");
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+17
	push	ax
	call	far ptr _print_str_cr
	add	sp,4
   ;	
   ;	                          break;
   ;	
@5@1010:
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
	push	ds
	push	ax
	call	far ptr _test_bit
	add	sp,6
	or	ax,ax
	je	short @5@1066
   ;	
   ;	              remake_handleline();
   ;	
	call	far ptr _remake_handleline
@5@1066:
   ;	
   ;	
   ;	            check_for_privates();
   ;	
	call	far ptr _check_for_privates
   ;	
   ;	            print_cr();
   ;	
	call	far ptr _print_cr
   ;	
   ;	
   ;	            special_code(1,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	ax,1
	push	ax
	call	far ptr _special_code
	add	sp,4
   ;	
   ;	            print_string(prompt);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	far ptr _print_string
	add	sp,4
   ;	
   ;	            special_code(0,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	xor	ax,ax
	push	ax
	call	far ptr _special_code
	add	sp,4
   ;	
   ;	
   ;	            print_chr(def);
   ;	
	mov	al,byte ptr [bp+14]
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(bkspchr);
   ;	
	mov	al,byte ptr [bp-4]
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	          } break;
   ;	
	jmp	@5@1262
@5@1094:
   ;	
   ;	
   ;	            case 2:
   ;	          {   /* OTHERWISE.. just let them go back to typing */
   ;	            print_chr(bkspchr);
   ;	
	mov	al,byte ptr [bp-4]
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(def);
   ;	
	mov	al,byte ptr [bp+14]
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(bkspchr);
   ;	
	mov	al,byte ptr [bp-4]
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	           } break;
   ;	
	jmp	short @5@1262
@5@1122:
   ;	
   ;	
   ;	           case 3:
   ;	           {
   ;	             check_for_privates();
   ;	
	call	far ptr _check_for_privates
   ;	
   ;	             print_cr();
   ;	
	call	far ptr _print_cr
   ;	
   ;	
   ;	             special_code(1,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	ax,1
	push	ax
	call	far ptr _special_code
	add	sp,4
   ;	
   ;	             print_string(prompt);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	far ptr _print_string
	add	sp,4
   ;	
   ;	             special_code(0,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	xor	ax,ax
	push	ax
	call	far ptr _special_code
	add	sp,4
   ;	
   ;	
   ;	             print_chr(def);
   ;	
	mov	al,byte ptr [bp+14]
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	             print_chr(bkspchr);
   ;	
	mov	al,byte ptr [bp-4]
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	           } break;
   ;	
	jmp	short @5@1262
@5@1150:
   ;	
   ;	          }
   ;	       } // end of stuff for / commands
   ;	       else
   ;	       if (strrchr(chars_allowed,key))
   ;	
	les	di,dword ptr [bp+10]
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	mov	ax,word ptr [bp-2]
	dec	di
	std	
	repnz 	scasb	
	je	short @5@1206
	mov	di,-1
	xor	ax,ax
	mov	es,ax
@5@1206:
	inc	di
	cld	
	mov	ax,es
	or	di,ax
	je	short @5@1262
   ;	
   ;	           key_pr=0;
   ;	
	xor	si,si
@5@1262:
	or	si,si
	je	@@4
	jmp	@5@58
@@4:
   ;	
   ;	     }
   ;	
   ;	   if (key!=def)
   ;	
	mov	al,byte ptr [bp+14]
	cbw	
	cmp	ax,word ptr [bp-2]
	je	short @5@1346
   ;	
   ;	      print_chr(key);
   ;	
	mov	al,byte ptr [bp-2]
	push	ax
	call	far ptr _print_chr
	add	sp,2
@5@1346:
   ;	
   ;	
   ;	   print_cr();
   ;	
	call	far ptr _print_cr
   ;	
   ;	   return key;
   ;	
	mov	ax,word ptr [bp-2]
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
	assume	cs:IO_TEXT
_prompt_get_string	proc	far
	push	bp
	mov	bp,sp
	sub	sp,452
	push	si
	push	di
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
	mov	di,1
   ;	
   ;	
   ;	   special_code(1,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	ax,1
	push	ax
	call	far ptr _special_code
	add	sp,4
   ;	
   ;	   print_string(prompt);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	far ptr _print_string
	add	sp,4
   ;	
   ;	   special_code(0,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	xor	ax,ax
	push	ax
	call	far ptr _special_code
	add	sp,4
	jmp	@6@1458
@6@58:
   ;	
   ;	
   ;	   while (flag)             /* wait while editing */
   ;	    {
   ;	      key = wait_ch();
   ;	
	push	cs
	call	near ptr _wait_ch
	mov	word ptr [bp-2],ax
   ;	
   ;	      if (((key == 8) || (key == 127)))
   ;	
	cmp	word ptr [bp-2],8
	je	short @6@114
	cmp	word ptr [bp-2],127
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
	push	ds
	mov	ax,offset DGROUP:_backspacestring
	push	ax
	call	far ptr _print_string
	add	sp,4
@6@170:
   ;	
   ;	       }               /* and go back one */
   ;	
   ;	      if (key == 27)    /* if we abort, then clear all characters */
   ;	
	cmp	word ptr [bp-2],27
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
	xor	di,di
   ;	
   ;	            print_chr('\\');
   ;	
	mov	al,92
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(13);
   ;	
	mov	al,13
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(10);
   ;	
	mov	al,10
	push	ax
	call	far ptr _print_chr
	add	sp,2
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
	cmp	word ptr [bp-2],13
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
	xor	di,di
   ;	
   ;	            print_chr(13);
   ;	
	mov	al,13
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(10);
   ;	
	mov	al,10
	push	ax
	call	far ptr _print_chr
	add	sp,2
@6@338:
   ;	
   ;	          };
   ;	       };
   ;	      if (((key >= 32) && (key <= 126)) && (pos < limit))
   ;	
	cmp	word ptr [bp-2],32
	jl	short @6@534
	cmp	word ptr [bp-2],126
	jg	short @6@534
	cmp	si,word ptr [bp+14]
	jge	short @6@534
   ;	
   ;	       {                        /* insert the character if there's room */
   ;	         *(string+pos) = key;
   ;	
	les	bx,dword ptr [bp+10]
	add	bx,si
	mov	al,byte ptr [bp-2]
	mov	byte ptr es:[bx],al
   ;	
   ;	         if (key == '+')        /* if +, don't let it be typed normally */
   ;	
	cmp	word ptr [bp-2],43
	jne	short @6@478
   ;	
   ;	          {
   ;	            print_chr(key);     /* print the character with a space */
   ;	
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(32);      /* and a backspace */
   ;	
	mov	al,32
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(8);
   ;	
	mov	al,8
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	          }
   ;	
	jmp	short @6@506
@6@478:
   ;	
   ;	          else
   ;	          print_chr(key);       /* otherwise, print it normally */
   ;	
	mov	al,byte ptr [bp-2]
	push	ax
	call	far ptr _print_chr
	add	sp,2
@6@506:
   ;	
   ;	         pos++;
   ;	
	inc	si
@6@534:
   ;	
   ;	       };
   ;	
   ;	      if ((pos>0 && *string=='/' ))
   ;	
	or	si,si
	jg	@@5
	jmp	@6@1458
@@5:
	les	bx,dword ptr [bp+10]
	cmp	byte ptr es:[bx],47
	je	@@6
	jmp	@6@1458
@@6:
   ;	
   ;	       {      /* IF they did /p we need to start getting all the others */
   ;	         char s2[STRING_SIZE];
   ;	
   ;	         switch(get_string_cntrl(s2,STRING_SIZE-300,0,1,1,0,0,0,0))
   ;	
	mov	al,0
	push	ax
	push	ax
	push	ax
	push	ax
	mov	al,1
	push	ax
	push	ax
	mov	al,0
	push	ax
	mov	ax,150
	push	ax
	push	ss
	lea	ax,word ptr [bp-452]
	push	ax
	call	far ptr _get_string_cntrl
	add	sp,20
	or	ax,ax
	je	short @6@730
	cmp	ax,1
	je	short @6@758
	cmp	ax,2
	je	short @6@786
	jmp	short @6@814
@6@730:
   ;	
   ;	         {
   ;	           case 0 : action=1;
   ;	
	mov	dx,1
   ;	
   ;	                    break;
   ;	
	jmp	short @6@842
@6@758:
   ;	
   ;	           case 1 : action=2;
   ;	
	mov	dx,2
   ;	
   ;	                    break;
   ;	
	jmp	short @6@842
@6@786:
   ;	
   ;	           case 2 : action=3;
   ;	
	mov	dx,3
   ;	
   ;	                    break;
   ;	
	jmp	short @6@842
@6@814:
   ;	
   ;	           default: action=3;
   ;	
	mov	dx,3
   ;	
   ;	                    break;
   ;	
@6@842:
   ;	
   ;	         }
   ;	
   ;	         switch(action)
   ;	
	mov	ax,dx
	cmp	ax,1
	je	short @6@982
	cmp	ax,2
	jne	@@7
	jmp	@6@1402
@@7:
	cmp	ax,3
	jne	@@8
	jmp	@6@1430
@@8:
	jmp	@6@1458
@6@982:
   ;	
   ;	         {
   ;	         case 1:
   ;	          {
   ;	
   ;	            print_cr();
   ;	
	call	far ptr _print_cr
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
	mov	dx,seg _user_options
	add	ax,offset _user_options+2
	push	dx
	push	ax
	call	far ptr _test_bit
	add	sp,6
	or	ax,ax
	jne	short @6@1038
   ;	
   ;	                  remove_ansi(s2);
   ;	
	push	ss
	lea	ax,word ptr [bp-452]
	push	ax
	call	far ptr _remove_ansi
	add	sp,4
	jmp	short @6@1094
@6@1038:
   ;	
   ;	               else
   ;	               if (!test_bit(user_options[tswitch].privs,CAN_TYPE_FLASHING_PRV))
   ;	
	mov	ax,40
	push	ax
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,178
	imul	dx
	mov	dx,seg _user_options
	add	ax,offset _user_options+2
	push	dx
	push	ax
	call	far ptr _test_bit
	add	sp,6
	or	ax,ax
	jne	short @6@1094
   ;	
   ;	                  remove_flashing(s2);
   ;	
	push	ss
	lea	ax,word ptr [bp-452]
	push	ax
	call	far ptr _remove_flashing
	add	sp,4
@6@1094:
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
	je	short @6@1150
   ;	
   ;	                      (void)limit_carrots(s2,6);
   ;	
	mov	ax,6
	push	ax
	push	ss
	lea	ax,word ptr [bp-452]
	push	ax
	call	far ptr _limit_carrots
	add	sp,6
@6@1150:
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
	push	ds
	push	ax
	push	ss
	lea	ax,word ptr [bp-452]
	push	ax
	call	far ptr _exec
	add	sp,12
	cmp	ax,1
	je	short @6@1262
	cmp	ax,2
	je	short @6@1290
	jmp	short @6@1318
@6@1262:
   ;	
   ;	            {
   ;	                case 0 :  break;
   ;	                case 1 :  print_invalid_command();
   ;	
	call	far ptr _print_invalid_command
   ;	
   ;	                          break;
   ;	
	jmp	short @6@1318
@6@1290:
   ;	
   ;	                case 2 :  print_str_cr("Command not available.");
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+17
	push	ax
	call	far ptr _print_str_cr
	add	sp,4
   ;	
   ;	                          break;
   ;	
@6@1318:
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
	push	ds
	push	ax
	call	far ptr _test_bit
	add	sp,6
	or	ax,ax
	je	short @6@1374
   ;	
   ;	              remake_handleline();
   ;	
	call	far ptr _remake_handleline
@6@1374:
   ;	
   ;	
   ;	            check_for_privates();
   ;	
	call	far ptr _check_for_privates
   ;	
   ;	            print_cr();
   ;	
	call	far ptr _print_cr
   ;	
   ;	
   ;	            special_code(1,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	ax,1
	push	ax
	call	far ptr _special_code
	add	sp,4
   ;	
   ;	            print_string(prompt);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	far ptr _print_string
	add	sp,4
   ;	
   ;	            special_code(0,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	xor	ax,ax
	push	ax
	call	far ptr _special_code
	add	sp,4
   ;	
   ;	
   ;	            pos=0;
   ;	
	xor	si,si
   ;	
   ;	          } break;
   ;	
	jmp	short @6@1458
@6@1402:
   ;	
   ;	            case 2:
   ;	          {   /* OTHERWISE.. just let them go back to typing */
   ;	            pos--;
   ;	
	dec	si
   ;	
   ;	            print_string(backspacestring);
   ;	
	push	ds
	mov	ax,offset DGROUP:_backspacestring
	push	ax
	call	far ptr _print_string
	add	sp,4
   ;	
   ;	           } break;
   ;	
	jmp	short @6@1458
@6@1430:
   ;	
   ;	           case 3:
   ;	           {
   ;	             check_for_privates();
   ;	
	call	far ptr _check_for_privates
   ;	
   ;	             print_cr();
   ;	
	call	far ptr _print_cr
   ;	
   ;	
   ;	             special_code(1,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	ax,1
	push	ax
	call	far ptr _special_code
	add	sp,4
   ;	
   ;	             print_string(prompt);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	far ptr _print_string
	add	sp,4
   ;	
   ;	             special_code(0,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	xor	ax,ax
	push	ax
	call	far ptr _special_code
	add	sp,4
   ;	
   ;	
   ;	             pos=0;
   ;	
	xor	si,si
   ;	
   ;	           } break;
   ;	
@6@1458:
	or	di,di
	je	@@9
	jmp	@6@58
@@9:
   ;	
   ;	          }
   ;	       }; // end of stuff for / commands
   ;	
   ;	    };  // end WHILE loop
   ;	
   ;	   *(string+pos) = 0;           /* mark end of the string */
   ;	
	les	bx,dword ptr [bp+10]
	add	bx,si
	mov	byte ptr es:[bx],0
   ;	
   ;	   if (!strncmp(string,"NO CARRIER",10)) *string = 0;
   ;	
	mov	ax,word ptr [bp+12]
	mov	si,word ptr [bp+10]
	push	ds
	pop	es
	mov	di,offset DGROUP:s@+40
	push	ax
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	sub	di,cx
	mov	ax,10
	cmp	ax,cx
	jae	short @6@1542
	mov	cx,10
@6@1542:
	mov	ax,ds
	pop	ds
	push	ax
	xor	ax,ax
	repz 	cmpsb	
	pop	ds
	je	short @6@1598
	sbb	ax,ax
	sbb	ax,-1
@6@1598:
	or	ax,ax
	jne	short @6@1654
	les	bx,dword ptr [bp+10]
	mov	byte ptr es:[bx],0
@6@1654:
   ;	
   ;	   if (!strncmp(string,"CONNECT",7)) *string = 0;
   ;	
	mov	ax,word ptr [bp+12]
	mov	si,word ptr [bp+10]
	push	ds
	pop	es
	mov	di,offset DGROUP:s@+51
	push	ax
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	sub	di,cx
	mov	ax,7
	cmp	ax,cx
	jae	short @6@1710
	mov	cx,7
@6@1710:
	mov	ax,ds
	pop	ds
	push	ax
	xor	ax,ax
	repz 	cmpsb	
	pop	ds
	je	short @6@1766
	sbb	ax,ax
	sbb	ax,-1
@6@1766:
	or	ax,ax
	jne	short @6@1822
	les	bx,dword ptr [bp+10]
	mov	byte ptr es:[bx],0
@6@1822:
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
	assume	cs:IO_TEXT
_get_string	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
   ;	
   ;	 {
   ;	   int pos = 0;
   ;	
	xor	si,si
   ;	
   ;	   int key;
   ;	   int flag = 1;
   ;	
	mov	di,1
	jmp	@7@562
@7@58:
   ;	
   ;	   while (flag)             /* wait while editing */
   ;	    {
   ;	      key = wait_ch();
   ;	
	push	cs
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
	push	ds
	mov	ax,offset DGROUP:_backspacestring
	push	ax
	call	far ptr _print_string
	add	sp,4
   ;	
   ;	       }               /* and go back one */
   ;	
	jmp	short @7@198
@7@170:
   ;	
   ;	      else
   ;	         flag=0;
   ;	
	xor	di,di
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
	xor	di,di
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
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(13);
   ;	
	mov	al,13
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(10);
   ;	
	mov	al,10
	push	ax
	call	far ptr _print_chr
	add	sp,2
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
	xor	di,di
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
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(10);
   ;	
	mov	al,10
	push	ax
	call	far ptr _print_chr
	add	sp,2
@7@366:
   ;	
   ;	          };
   ;	       };
   ;	      if (((key >= 32) && (key <= 126)) && (pos < limit))
   ;	
	cmp	word ptr [bp-2],32
	jl	short @7@562
	cmp	word ptr [bp-2],126
	jg	short @7@562
	cmp	si,word ptr [bp+10]
	jge	short @7@562
   ;	
   ;	       {                        /* insert the character if there's room */
   ;	         *(string+pos) = key;
   ;	
	les	bx,dword ptr [bp+6]
	add	bx,si
	mov	al,byte ptr [bp-2]
	mov	byte ptr es:[bx],al
   ;	
   ;	         if (key == '+')        /* if +, don't let it be typed normally */
   ;	
	cmp	word ptr [bp-2],43
	jne	short @7@506
   ;	
   ;	          {
   ;	            print_chr(key);     /* print the character with a space */
   ;	
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(32);      /* and a backspace */
   ;	
	mov	al,32
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(8);
   ;	
	mov	al,8
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	          }
   ;	
	jmp	short @7@534
@7@506:
   ;	
   ;	          else
   ;	          print_chr(key);       /* otherwise, print it normally */
   ;	
	mov	al,byte ptr [bp-2]
	push	ax
	call	far ptr _print_chr
	add	sp,2
@7@534:
   ;	
   ;	         pos++;
   ;	
	inc	si
@7@562:
	or	di,di
	je	@@10
	jmp	@7@58
@@10:
   ;	
   ;	       };
   ;	    };
   ;	   *(string+pos) = 0;           /* mark end of the string */
   ;	
	les	bx,dword ptr [bp+6]
	add	bx,si
	mov	byte ptr es:[bx],0
   ;	
   ;	   if (!strncmp(string,"NO CARRIER",10)) *string = 0;
   ;	
	mov	ax,word ptr [bp+8]
	mov	si,word ptr [bp+6]
	push	ds
	pop	es
	mov	di,offset DGROUP:s@+40
	push	ax
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	sub	di,cx
	mov	ax,10
	cmp	ax,cx
	jae	short @7@646
	mov	cx,10
@7@646:
	mov	ax,ds
	pop	ds
	push	ax
	xor	ax,ax
	repz 	cmpsb	
	pop	ds
	je	short @7@702
	sbb	ax,ax
	sbb	ax,-1
@7@702:
	or	ax,ax
	jne	short @7@758
	les	bx,dword ptr [bp+6]
	mov	byte ptr es:[bx],0
@7@758:
   ;	
   ;	   if (!strncmp(string,"CONNECT",7)) *string = 0;
   ;	
	mov	ax,word ptr [bp+8]
	mov	si,word ptr [bp+6]
	push	ds
	pop	es
	mov	di,offset DGROUP:s@+51
	push	ax
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	sub	di,cx
	mov	ax,7
	cmp	ax,cx
	jae	short @7@814
	mov	cx,7
@7@814:
	mov	ax,ds
	pop	ds
	push	ax
	xor	ax,ax
	repz 	cmpsb	
	pop	ds
	je	short @7@870
	sbb	ax,ax
	sbb	ax,-1
@7@870:
	or	ax,ax
	jne	short @7@926
	les	bx,dword ptr [bp+6]
	mov	byte ptr es:[bx],0
@7@926:
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
	assume	cs:IO_TEXT
_mark_user_log_on	proc	far
	push	bp
	mov	bp,sp
	sub	sp,8
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
	call	far ptr _islocked
	add	sp,2
	mov	si,ax
   ;	
   ;	
   ;	   if (!flag) lock_dos(491);
   ;	
	or	si,si
	jne	short @8@86
	mov	ax,491
	push	ax
	call	far ptr _lock_dos
	add	sp,2
@8@86:
   ;	
   ;	   fileptr=g_fopen(USER_ENTER_LOG_FILE,"a","IO#1"); /* dos error */
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+71
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+69
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+59
	push	ax
	call	far ptr _g_fopen
	add	sp,12
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
   ;	
   ;	   if (!fileptr)                    /* open the log */
   ;	
	or	ax,word ptr [bp-2]
	jne	short @8@198
   ;	
   ;	    {
   ;	      if (!flag) unlock_dos();
   ;	
	or	si,si
	jne	short @8@170
	call	far ptr _unlock_dos
@8@170:
   ;	
   ;	      log_error(USER_ENTER_LOG_FILE);
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+59
	push	ax
	call	far ptr _log_error
	add	sp,4
   ;	
   ;	      return;
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
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
	mov	di,ax
	mov	dx,seg _user_lines
	add	ax,offset _user_lines+2
	push	dx
	push	ax
	mov	ax,seg _user_lines
	mov	es,ax
	push	word ptr es:_user_lines[di]
	push	word ptr DGROUP:_tswitch
	push	ds
	mov	ax,offset DGROUP:s@+76
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _fprintf
	add	sp,16
   ;	
   ;	   time(&tim);                      /* insert the right time */
   ;	
	push	ss
	lea	ax,word ptr [bp-8]
	push	ax
	call	far ptr _time
	add	sp,4
   ;	
   ;	   fprintf(fileptr,"Date: %s",ctime(&tim));
   ;	
	push	ss
	lea	ax,word ptr [bp-8]
	push	ax
	call	far ptr _ctime
	add	sp,4
	push	dx
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+92
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _fprintf
	add	sp,12
   ;	
   ;	   g_fclose(fileptr);                 /* close the log */
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _g_fclose
	add	sp,4
   ;	
   ;	   if (!flag) unlock_dos();
   ;	
	or	si,si
	jne	short @8@254
	call	far ptr _unlock_dos
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
	assume	cs:IO_TEXT
_log_error	proc	far
	push	bp
	mov	bp,sp
	sub	sp,110
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
	call	far ptr _islocked
	add	sp,2
	mov	word ptr [bp-10],ax
   ;	
   ;	   char s[100];
   ;	
   ;	   if (!tasking)
   ;	
	mov	al,byte ptr DGROUP:_tasking
	cbw	
	or	ax,ax
	jne	short @9@142
   ;	
   ;	   {
   ;	     printf("\n");
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+90
	push	ax
	call	far ptr _printf
	add	sp,4
   ;	
   ;	     if (*filename == '*')
   ;	
	les	bx,dword ptr [bp+6]
	cmp	byte ptr es:[bx],42
	jne	short @9@114
   ;	
   ;	      printf("%s\n",filename);
   ;	
	push	word ptr [bp+8]
	push	bx
	push	ds
	mov	ax,offset DGROUP:s@+88
	push	ax
	call	far ptr _printf
	add	sp,8
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@9@114:
   ;	
   ;	     else
   ;	      perror(filename);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	far ptr _perror
	add	sp,4
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@9@142:
   ;	
   ;	     return;
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
	mov	ax,word ptr [bp+8]
	mov	si,word ptr [bp+6]
	push	ds
	pop	es
	mov	di,offset DGROUP:s@+101
	push	ax
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	sub	di,cx
	mov	ax,4
	cmp	ax,cx
	jae	short @9@198
	mov	cx,4
@9@198:
	mov	ax,ds
	pop	ds
	push	ax
	xor	ax,ax
	repz 	cmpsb	
	pop	ds
	je	short @9@254
	sbb	ax,ax
	sbb	ax,-1
@9@254:
	or	ax,ax
	jne	@@11
	jmp	@9@562
@@11:
   ;	
   ;	     return;  // Dont log errors with temp files
   ;	
   ;	   if (!flag) lock_dos(492);
   ;	
	cmp	word ptr [bp-10],0
	jne	short @9@338
	mov	ax,492
	push	ax
	call	far ptr _lock_dos
	add	sp,2
@9@338:
   ;	
   ;	   if (*filename=='*')              /* if the filename is a asterisk */
   ;	
	les	bx,dword ptr [bp+6]
	cmp	byte ptr es:[bx],42
	jne	short @9@394
   ;	
   ;	    sprintf(s,"%s\n",filename+1);   /* use it directly */
   ;	
	mov	ax,word ptr [bp+6]
	inc	ax
	push	word ptr [bp+8]
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+88
	push	ax
	push	ss
	lea	ax,word ptr [bp-110]
	push	ax
	call	far ptr _sprintf
	add	sp,12
	jmp	short @9@422
@9@394:
   ;	
   ;	    else sprintf(s,"Error: %s",_strerror(filename));  /* otherwise use the */
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	far ptr __strerror
	add	sp,4
	push	dx
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+106
	push	ax
	push	ss
	lea	ax,word ptr [bp-110]
	push	ax
	call	far ptr _sprintf
	add	sp,12
@9@422:
   ;	
   ;	   fileptr=g_fopen("error.log","a","IO#2");              /* dos error */
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+126
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+69
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+116
	push	ax
	call	far ptr _g_fopen
	add	sp,12
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
   ;	
   ;	   if (!fileptr)                    /* open the error log */
   ;	
	or	ax,word ptr [bp-2]
	jne	short @9@506
   ;	
   ;	    {
   ;	      if (!flag) unlock_dos();
   ;	
	cmp	word ptr [bp-10],0
	jne	short @9@562
	call	far ptr _unlock_dos
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@9@506:
   ;	
   ;	      return;
   ;	    };
   ;	   fprintf(fileptr,"%s",s);         /* add the line to the file */
   ;	
	push	ss
	lea	ax,word ptr [bp-110]
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+98
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _fprintf
	add	sp,12
   ;	
   ;	   time(&tim);                      /* insert the right time */
   ;	
	push	ss
	lea	ax,word ptr [bp-8]
	push	ax
	call	far ptr _time
	add	sp,4
   ;	
   ;	   fprintf(fileptr,"Date: %s\n",ctime(&tim));
   ;	
	push	ss
	lea	ax,word ptr [bp-8]
	push	ax
	call	far ptr _ctime
	add	sp,4
	push	dx
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+131
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _fprintf
	add	sp,12
   ;	
   ;	   g_fclose(fileptr);                 /* close the error log */
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _g_fclose
	add	sp,4
   ;	
   ;	   if (!flag) unlock_dos();
   ;	
	cmp	word ptr [bp-10],0
	jne	short @9@562
	call	far ptr _unlock_dos
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
	assume	cs:IO_TEXT
_log_event	proc	far
	push	bp
	mov	bp,sp
	sub	sp,108
	push	si
   ;	
   ;	 {
   ;	   FILE *fileptr;
   ;	   time_t tim;
   ;	   int flag = islocked(DOS_SEM);
   ;	
	xor	ax,ax
	push	ax
	call	far ptr _islocked
	add	sp,2
	mov	si,ax
   ;	
   ;	   char s[100];
   ;	
   ;	
   ;	   if (!flag) lock_dos(493);
   ;	
	or	si,si
	jne	short @10@86
	mov	ax,493
	push	ax
	call	far ptr _lock_dos
	add	sp,2
@10@86:
   ;	
   ;	
   ;	   fileptr=g_fopen(filename,"a","IO#3");              /* dos error */
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+141
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+69
	push	ax
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	far ptr _g_fopen
	add	sp,12
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
   ;	
   ;	   if (!fileptr)                    /* open the error log */
   ;	
	or	ax,word ptr [bp-2]
	jne	short @10@170
   ;	
   ;	    {
   ;	      if (!flag) unlock_dos();
   ;	
	or	si,si
	je	@@12
	jmp	@10@226
@@12:
	call	far ptr _unlock_dos
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@10@170:
   ;	
   ;	      return;
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
	push	ds
	mov	ax,offset DGROUP:s@+146
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _fprintf
	add	sp,12
   ;	
   ;	
   ;	   time(&tim);                      /* insert the right time */
   ;	
	push	ss
	lea	ax,word ptr [bp-8]
	push	ax
	call	far ptr _time
	add	sp,4
   ;	
   ;	   strftime(s,33,"%a %b %d %Y %I:%M:%S %p",localtime(&tim));
   ;	
	push	ss
	lea	ax,word ptr [bp-8]
	push	ax
	call	far ptr _localtime
	add	sp,4
	push	dx
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+162
	push	ax
	mov	ax,33
	push	ax
	push	ss
	lea	ax,word ptr [bp-108]
	push	ax
	call	far ptr _strftime
	add	sp,14
   ;	
   ;	   fprintf(fileptr,"%s | ",s);
   ;	
	push	ss
	lea	ax,word ptr [bp-108]
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+186
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _fprintf
	add	sp,12
   ;	
   ;	
   ;	   fprintf(fileptr,"%s\n",event);         /* add the line to the file */
   ;	
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	ds
	mov	ax,offset DGROUP:s@+88
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _fprintf
	add	sp,12
   ;	
   ;	
   ;	   g_fclose(fileptr);                 /* close the error log */
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _g_fclose
	add	sp,4
   ;	
   ;	   if (!flag) unlock_dos();
   ;	
	or	si,si
	jne	short @10@226
	call	far ptr _unlock_dos
@10@226:
   ;	
   ;	 };
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_log_event	endp
   ;	
   ;	void log_kill(int node,int who_killed)
   ;	
	assume	cs:IO_TEXT
_log_kill	proc	far
	push	bp
	mov	bp,sp
	sub	sp,336
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
	call	far ptr _islocked
	add	sp,2
	mov	word ptr [bp-10],ax
   ;	
   ;	   char n1[100],n2[100],n3[100];
   ;	   char temp[20];
   ;	   char type[5];
   ;	
   ;	   if (!flag) lock_dos(494);
   ;	
	cmp	word ptr [bp-10],0
	jne	short @11@86
	mov	ax,494
	push	ax
	call	far ptr _lock_dos
	add	sp,2
@11@86:
   ;	
   ;	
   ;	   fileptr=g_fopen(KILL_LOG_FILE,"a","IO#3");              /* dos error */
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+141
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+69
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+192
	push	ax
	call	far ptr _g_fopen
	add	sp,12
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
   ;	
   ;	   if (!fileptr)                    /* open the error log */
   ;	
	or	ax,word ptr [bp-2]
	jne	short @11@170
   ;	
   ;	    {
   ;	      if (!flag) unlock_dos();
   ;	
	cmp	word ptr [bp-10],0
	je	@@13
	jmp	@11@618
@@13:
	call	far ptr _unlock_dos
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@11@170:
   ;	
   ;	      return;
   ;	    };
   ;	
   ;	   if (user_lines[who_killed].number<0)
   ;	
	mov	ax,word ptr [bp+8]
	mov	dx,384
	imul	dx
	mov	si,ax
	mov	dx,seg _user_lines
	mov	bx,ax
	mov	es,dx
	cmp	word ptr es:_user_lines[bx],0
	jge	short @11@282
   ;	
   ;	    { if (line_status[who_killed].link)
   ;	
	mov	ax,word ptr [bp+8]
	mov	dx,187
	imul	dx
	mov	bx,ax
	cmp	byte ptr DGROUP:_line_status[bx+180],0
	je	short @11@254
   ;	
   ;	        strcpy(type,"LINK");
   ;	
	push	ss
	pop	es
	lea	di,word ptr [bp-16]
	mov	si,offset DGROUP:s@+205
	mov	cx,2
	rep 	movsw	
	movsb	
	jmp	short @11@310
@11@254:
   ;	
   ;	      else
   ;	        strcpy(type,"%GST");
   ;	
	push	ss
	pop	es
	lea	di,word ptr [bp-16]
	mov	si,offset DGROUP:s@+210
	mov	cx,2
	rep 	movsw	
	movsb	
	jmp	short @11@310
@11@282:
   ;	
   ;	    }
   ;	   else
   ;	     sprintf(type,"#%03d",user_lines[who_killed].number);
   ;	
	mov	ax,seg _user_lines
	mov	es,ax
	push	word ptr es:_user_lines[si]
	push	ds
	mov	ax,offset DGROUP:s@+215
	push	ax
	push	ss
	lea	ax,word ptr [bp-16]
	push	ax
	call	far ptr _sprintf
	add	sp,10
@11@310:
   ;	
   ;	
   ;	   strncpy(temp,user_options[who_killed].noansi_handle,15);
   ;	
	mov	ax,seg _user_options
	push	ax
	mov	ax,word ptr [bp+8]
	mov	dx,178
	imul	dx
	add	ax,offset _user_options+38
	mov	dx,ss
	lea	si,word ptr [bp-336]
	push	ax
	push	dx
	mov	dx,15
	pop	ax
	pop	di
	pop	es
	push	ax
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	sub	di,cx
	mov	ax,ds
	pop	ds
	push	ax
	xchg	si,di
	mov	bx,ds
	mov	ax,es
	mov	ds,ax
	mov	es,bx
	sub	dx,cx
	jae	short @11@366
	add	cx,dx
	xor	dx,dx
@11@366:
	shr	cx,1
	rep 	movsw	
	adc	cx,cx
	rep 	movsb	
	mov	cx,dx
	xor	ax,ax
	rep 	stosb	
	pop	ds
   ;	
   ;	   temp[16]=0;
   ;	
	mov	byte ptr [bp-320],0
   ;	
   ;	
   ;	
   ;	                                    /* insert User Number, and NODE info */
   ;	   sprintf(n1,"(%02d) %s:%c%s%c",who_killed,type,user_options[who_killed].staple[2],
   ;	
   ;	
   ;	                 temp,user_options[who_killed].staple[3]);
   ;	
	mov	ax,word ptr [bp+8]
	mov	dx,178
	imul	dx
	mov	si,ax
	mov	dx,seg _user_options
	mov	bx,ax
	mov	es,dx
	mov	al,byte ptr es:_user_options[bx+37]
	cbw	
	push	ax
	push	ss
	lea	ax,word ptr [bp-336]
	push	ax
	mov	ax,seg _user_options
	mov	es,ax
	mov	al,byte ptr es:_user_options[si+36]
	cbw	
	push	ax
	push	ss
	lea	ax,word ptr [bp-16]
	push	ax
	push	word ptr [bp+8]
	push	ds
	mov	ax,offset DGROUP:s@+221
	push	ax
	push	ss
	lea	ax,word ptr [bp-116]
	push	ax
	call	far ptr _sprintf
	add	sp,22
   ;	
   ;	
   ;	   if (user_lines[node].number<0)
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,384
	imul	dx
	mov	si,ax
	mov	dx,seg _user_lines
	mov	bx,ax
	mov	es,dx
	cmp	word ptr es:_user_lines[bx],0
	jge	short @11@478
   ;	
   ;	    { if (line_status[node].link)
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,187
	imul	dx
	mov	bx,ax
	cmp	byte ptr DGROUP:_line_status[bx+180],0
	je	short @11@450
   ;	
   ;	        strcpy(type,"LINK");
   ;	
	push	ss
	pop	es
	lea	di,word ptr [bp-16]
	mov	si,offset DGROUP:s@+205
	mov	cx,2
	rep 	movsw	
	movsb	
	jmp	short @11@506
@11@450:
   ;	
   ;	      else
   ;	        strcpy(type,"%GST");
   ;	
	push	ss
	pop	es
	lea	di,word ptr [bp-16]
	mov	si,offset DGROUP:s@+210
	mov	cx,2
	rep 	movsw	
	movsb	
	jmp	short @11@506
@11@478:
   ;	
   ;	    }
   ;	   else
   ;	     sprintf(type,"#%03d",user_lines[node].number);
   ;	
	mov	ax,seg _user_lines
	mov	es,ax
	push	word ptr es:_user_lines[si]
	push	ds
	mov	ax,offset DGROUP:s@+215
	push	ax
	push	ss
	lea	ax,word ptr [bp-16]
	push	ax
	call	far ptr _sprintf
	add	sp,10
@11@506:
   ;	
   ;	
   ;	   strncpy(temp,user_options[node].noansi_handle,15);
   ;	
	mov	ax,seg _user_options
	push	ax
	mov	ax,word ptr [bp+6]
	mov	dx,178
	imul	dx
	add	ax,offset _user_options+38
	mov	dx,ss
	lea	si,word ptr [bp-336]
	push	ax
	push	dx
	mov	dx,15
	pop	ax
	pop	di
	pop	es
	push	ax
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	sub	di,cx
	mov	ax,ds
	pop	ds
	push	ax
	xchg	si,di
	mov	bx,ds
	mov	ax,es
	mov	ds,ax
	mov	es,bx
	sub	dx,cx
	jae	short @11@562
	add	cx,dx
	xor	dx,dx
@11@562:
	shr	cx,1
	rep 	movsw	
	adc	cx,cx
	rep 	movsb	
	mov	cx,dx
	xor	ax,ax
	rep 	stosb	
	pop	ds
   ;	
   ;	   temp[16]=0;
   ;	
	mov	byte ptr [bp-320],0
   ;	
   ;	
   ;	                                     /* insert User Number, and NODE info */
   ;	   sprintf(n2,"(%02d) %s:%c%s%c",node,type,user_options[node].staple[2],
   ;	
   ;	
   ;	                 temp,user_options[node].staple[3]);
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,178
	imul	dx
	mov	si,ax
	mov	dx,seg _user_options
	mov	bx,ax
	mov	es,dx
	mov	al,byte ptr es:_user_options[bx+37]
	cbw	
	push	ax
	push	ss
	lea	ax,word ptr [bp-336]
	push	ax
	mov	ax,seg _user_options
	mov	es,ax
	mov	al,byte ptr es:_user_options[si+36]
	cbw	
	push	ax
	push	ss
	lea	ax,word ptr [bp-16]
	push	ax
	push	word ptr [bp+6]
	push	ds
	mov	ax,offset DGROUP:s@+221
	push	ax
	push	ss
	lea	ax,word ptr [bp-216]
	push	ax
	call	far ptr _sprintf
	add	sp,22
   ;	
   ;	
   ;	
   ;	   time(&tim);                      /* insert the right time */
   ;	
	push	ss
	lea	ax,word ptr [bp-8]
	push	ax
	call	far ptr _time
	add	sp,4
   ;	
   ;	   strftime(n3,22,"%m/%d/%y %I:%M:%S %p",localtime(&tim));
   ;	
	push	ss
	lea	ax,word ptr [bp-8]
	push	ax
	call	far ptr _localtime
	add	sp,4
	push	dx
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+238
	push	ax
	mov	ax,22
	push	ax
	push	ss
	lea	ax,word ptr [bp-316]
	push	ax
	call	far ptr _strftime
	add	sp,14
   ;	
   ;	
   ;	   fprintf(fileptr,"%-28s %-28s %-18s",n1,n2,n3);
   ;	
	push	ss
	lea	ax,word ptr [bp-316]
	push	ax
	push	ss
	lea	ax,word ptr [bp-216]
	push	ax
	push	ss
	lea	ax,word ptr [bp-116]
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+259
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _fprintf
	add	sp,20
   ;	
   ;	
   ;	
   ;	   g_fclose(fileptr);                 /* close the error log */
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _g_fclose
	add	sp,4
   ;	
   ;	   if (!flag) unlock_dos();
   ;	
	cmp	word ptr [bp-10],0
	jne	short @11@618
	call	far ptr _unlock_dos
@11@618:
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
	assume	cs:IO_TEXT
_last_ten_kills	proc	far
	push	bp
	mov	bp,sp
	sub	sp,84
	push	si
	push	di
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
	call	far ptr _print_cr
   ;	
   ;	   sprintf(s,"Last %d kills",num_last);
   ;	
	push	word ptr [bp+6]
	push	ds
	mov	ax,offset DGROUP:s@+277
	push	ax
	push	ss
	lea	ax,word ptr [bp-84]
	push	ax
	call	far ptr _sprintf
	add	sp,10
   ;	
   ;	   print_str_cr(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-84]
	push	ax
	call	far ptr _print_str_cr
	add	sp,4
   ;	
   ;	   print_file("TEXT\\KILL.HDR");
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+291
	push	ax
	call	far ptr _print_file
	add	sp,4
   ;	
   ;	   num_last++;
   ;	
	inc	word ptr [bp+6]
   ;	
   ;	   print_cr();
   ;	
	call	far ptr _print_cr
   ;	
   ;	
   ;	   lock_dos(495);
   ;	
	mov	ax,495
	push	ax
	call	far ptr _lock_dos
	add	sp,2
   ;	
   ;	   if (!(fileptr=g_fopen(KILL_LOG_FILE,"rb","FILES#4")))       /* open the user log */
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+308
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+305
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+192
	push	ax
	call	far ptr _g_fopen
	add	sp,12
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
	or	ax,dx
	jne	short @12@86
   ;	
   ;	    {
   ;	      log_error(KILL_LOG_FILE);
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+192
	push	ax
	push	cs
	call	near ptr _log_error
	add	sp,4
   ;	
   ;	      unlock_dos();
   ;	
	call	far ptr _unlock_dos
   ;	
   ;	      return;
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@12@86:
   ;	
   ;	    };
   ;	   num = 1;
   ;	
	mov	di,1
   ;	
   ;	   s[78]=0;
   ;	
	mov	byte ptr [bp-6],0
	jmp	@12@422
@12@114:
   ;	
   ;	   while (not_abort && (num<num_last))
   ;	    {
   ;	      fseek(fileptr,-78*(long int)(num),SEEK_END);
   ;	
	mov	ax,2
	push	ax
	mov	ax,di
	cwd	
	push	ax
	push	dx
	mov	dx,-1
	mov	ax,-78
	pop	cx
	pop	bx
	call	far ptr F_LXMUL@
	push	dx
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _fseek
	add	sp,10
   ;	
   ;	      not_abort = (ftell(fileptr) != 0);
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _ftell
	add	sp,4
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
	jne	@@14
	jmp	@12@394
@@14:
   ;	
   ;	       {
   ;	       //  sprintf(s,"%2d: ",num);
   ;	         unlock_dos();
   ;	
	call	far ptr _unlock_dos
   ;	
   ;	      //   print_string(s);
   ;	         lock_dos(496);
   ;	
	mov	ax,496
	push	ax
	call	far ptr _lock_dos
	add	sp,2
   ;	
   ;	         if (!fread(s,1,78,fileptr))
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	mov	ax,78
	push	ax
	mov	ax,1
	push	ax
	push	ss
	lea	ax,word ptr [bp-84]
	push	ax
	call	far ptr _fread
	add	sp,12
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
	call	far ptr _unlock_dos
   ;	
   ;	         print_str_cr(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-84]
	push	ax
	call	far ptr _print_str_cr
	add	sp,4
   ;	
   ;	         key = get_first_char(tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	bx,word ptr DGROUP:_tswitch
	mov	cl,2
	shl	bx,cl
	call	dword ptr DGROUP:_a_get_first_char[bx]
	add	sp,2
	mov	dx,ax
   ;	
   ;	         if ((key == 3) || (key == 27))
   ;	
	cmp	dx,3
	je	short @12@338
	cmp	dx,27
	jne	short @12@366
@12@338:
   ;	
   ;	          {
   ;	            int_char(portnum);
   ;	
	push	word ptr [bp+8]
	mov	bx,word ptr [bp+8]
	mov	cl,2
	shl	bx,cl
	call	dword ptr DGROUP:_a_get_nchar[bx]
	add	sp,2
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
	call	far ptr _lock_dos
	add	sp,2
@12@394:
   ;	
   ;	       };
   ;	      num++;
   ;	
	inc	di
@12@422:
	or	si,si
	je	short @12@478
	cmp	di,word ptr [bp+6]
	jge	@@15
	jmp	@12@114
@@15:
@12@478:
   ;	
   ;	    };
   ;	   g_fclose(fileptr);
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _g_fclose
	add	sp,4
   ;	
   ;	   clear_call_on_logoff();
   ;	
	call	far ptr _clear_call_on_logoff
   ;	
   ;	   unlock_dos();
   ;	
	call	far ptr _unlock_dos
   ;	
   ;	   print_cr();
   ;	
	call	far ptr _print_cr
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
	assume	cs:IO_TEXT
_print_file	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	 {
   ;	  print_file_to(filename,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	far ptr _print_file_to
	add	sp,6
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
	assume	cs:IO_TEXT
_print_file_to	proc	far
	push	bp
	mov	bp,sp
	sub	sp,612
	push	si
	push	di
   ;	
   ;	 {
   ;	   FILE *fileptr;
   ;	   long int location = 0;
   ;	
	mov	word ptr [bp-6],0
	mov	word ptr [bp-8],0
   ;	
   ;	   int count=0;
   ;	
	mov	word ptr [bp-10],0
   ;	
   ;	   char buf[512];
   ;	   int loop, point = 1;
   ;	
	mov	word ptr [bp-12],1
   ;	
   ;	   int ischar;
   ;	   int is_ansi_file=0;
   ;	
	xor	si,si
   ;	
   ;	   char *temp_file=filename;
   ;	
	mov	ax,word ptr [bp+8]
	mov	dx,word ptr [bp+6]
	mov	word ptr [bp-14],ax
	mov	word ptr [bp-16],dx
   ;	
   ;	   char temp_buf[80];
   ;	
   ;	   if ((strlen(filename)<75) && (line_status[portnum].ansi) &&
   ;	
   ;	
   ;	                (line_status[portnum].full_screen_ansi))
   ;	
	les	di,dword ptr [bp+6]
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	dec	cx
	cmp	cx,75
	jb	@@16
	jmp	@14@422
@@16:
	mov	ax,word ptr [bp+10]
	mov	dx,187
	imul	dx
	mov	di,ax
	mov	bx,ax
	cmp	byte ptr DGROUP:_line_status[bx+28],0
	jne	@@17
	jmp	@14@422
@@17:
	cmp	byte ptr DGROUP:_line_status[di+29],0
	jne	@@18
	jmp	@14@422
@@18:
   ;	
   ;	    {  char *end;
   ;	       strcpy(temp_buf,filename);
   ;	
	push	ss
	pop	es
	lea	di,word ptr [bp-612]
	push	es
	mov	es,word ptr [bp+8]
	push	di
	mov	di,word ptr [bp+6]
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	sub	di,cx
	shr	cx,1
	mov	ax,word ptr [bp+8]
	mov	si,word ptr [bp+6]
	pop	di
	pop	es
	push	ds
	mov	ds,ax
	rep 	movsw	
	adc	cx,cx
	rep 	movsb	
	pop	ds
   ;	
   ;	       end=temp_buf+strlen(temp_buf);
   ;	
	push	ss
	pop	es
	lea	di,word ptr [bp-612]
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	dec	cx
	lea	ax,word ptr [bp-612]
	add	cx,ax
	mov	word ptr [bp-18],ss
	mov	word ptr [bp-20],cx
	jmp	short @14@170
@14@142:
   ;	
   ;	
   ;	       while ((*end!='\\') && (*end!='.') && (end>=temp_buf))
   ;	         end--;
   ;	
	dec	word ptr [bp-20]
@14@170:
	les	bx,dword ptr [bp-20]
	cmp	byte ptr es:[bx],92
	je	short @14@254
	cmp	byte ptr es:[bx],46
	je	short @14@254
	lea	ax,word ptr [bp-612]
	cmp	ax,word ptr [bp-20]
	jbe	short @14@142
@14@254:
   ;	
   ;	
   ;	       if (*end=='.') *end=0;
   ;	
	les	bx,dword ptr [bp-20]
	cmp	byte ptr es:[bx],46
	jne	short @14@310
	mov	byte ptr es:[bx],0
@14@310:
   ;	
   ;	
   ;	       strcat(temp_buf,".ANS");
   ;	
	push	ss
	pop	es
	lea	di,word ptr [bp-612]
	mov	si,offset DGROUP:s@+316
	mov	cx,-1
	xor	ax,ax
	repnz 	scasb	
	dec	di
	mov	cx,5
	rep 	movsb	
   ;	
   ;	       temp_file=temp_buf;
   ;	
	lea	ax,word ptr [bp-612]
	mov	word ptr [bp-14],ss
	mov	word ptr [bp-16],ax
   ;	
   ;	       is_ansi_file=1;
   ;	
	mov	si,1
   ;	
   ;	       lock_dos(498);
   ;	
	mov	ax,498
	push	ax
	call	far ptr _lock_dos
	add	sp,2
   ;	
   ;	       if ((fileptr=g_fopen(temp_file,"rb","IO#3"))==NULL)   /* ANSI */
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+141
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+305
	push	ax
	push	word ptr [bp-14]
	push	word ptr [bp-16]
	call	far ptr _g_fopen
	add	sp,12
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
	or	ax,dx
	jne	short @14@366
   ;	
   ;	         { is_ansi_file=0;
   ;	
	xor	si,si
   ;	
   ;	           temp_file=filename;
   ;	
	mov	ax,word ptr [bp+8]
	mov	dx,word ptr [bp+6]
	mov	word ptr [bp-14],ax
	mov	word ptr [bp-16],dx
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
	push	word ptr [bp-4]
	call	far ptr _g_fclose
	add	sp,4
@14@394:
   ;	
   ;	       unlock_dos();
   ;	
	call	far ptr _unlock_dos
@14@422:
   ;	
   ;	    }
   ;	
   ;	   special_code(!is_ansi_file,portnum);
   ;	
	push	word ptr [bp+10]
	mov	ax,si
	neg	ax
	sbb	ax,ax
	inc	ax
	push	ax
	call	far ptr _special_code
	add	sp,4
	jmp	@14@982
@14@450:
   ;	
   ;	   while (point)
   ;	    {
   ;	      fileptr=0;
   ;	
	mov	word ptr [bp-2],0
	mov	word ptr [bp-4],0
	jmp	short @14@590
@14@478:
   ;	
   ;	      while (!fileptr)
   ;	       {
   ;	         lock_dos(499);
   ;	
	mov	ax,499
	push	ax
	call	far ptr _lock_dos
	add	sp,2
   ;	
   ;	         if( (fileptr=g_fopen(temp_file,"rb","IO#3"))==NULL)   /* open the file */
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+141
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+305
	push	ax
	push	word ptr [bp-14]
	push	word ptr [bp-16]
	call	far ptr _g_fopen
	add	sp,12
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
	or	ax,dx
	jne	short @14@534
   ;	
   ;	          {
   ;	            log_error(filename);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	cs
	call	near ptr _log_error
	add	sp,4
   ;	
   ;	            unlock_dos();
   ;	
	call	far ptr _unlock_dos
   ;	
   ;	            special_code(0,portnum);
   ;	
	push	word ptr [bp+10]
	xor	ax,ax
	push	ax
	call	far ptr _special_code
	add	sp,4
   ;	
   ;	            return;
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@14@534:
   ;	
   ;	          };
   ;	         unlock_dos();
   ;	
	call	far ptr _unlock_dos
   ;	
   ;	         if (!fileptr) next_task();
   ;	
	mov	ax,word ptr [bp-4]
	or	ax,word ptr [bp-2]
	jne	short @14@590
	pushf	
	call	far ptr _next_task
@14@590:
	mov	ax,word ptr [bp-4]
	or	ax,word ptr [bp-2]
	je	short @14@478
   ;	
   ;	       };
   ;	      lock_dos(500);
   ;	
	mov	ax,500
	push	ax
	call	far ptr _lock_dos
	add	sp,2
   ;	
   ;	      fseek(fileptr,location,SEEK_SET);         /* go to the portion of */
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _fseek
	add	sp,10
   ;	
   ;	                /* the file we want to read */
   ;	      point = fread(&buf, 1, 512, fileptr);     /* read 512 bytes */
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	mov	ax,512
	push	ax
	mov	ax,1
	push	ax
	push	ss
	lea	ax,word ptr [bp-532]
	push	ax
	call	far ptr _fread
	add	sp,12
	mov	word ptr [bp-12],ax
   ;	
   ;	      g_fclose(fileptr);          /* close the file */
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _g_fclose
	add	sp,4
   ;	
   ;	      unlock_dos();
   ;	
	call	far ptr _unlock_dos
   ;	
   ;	      location += 512;          /* go 512 bytes further */
   ;	
	add	word ptr [bp-8],512
	adc	word ptr [bp-6],0
   ;	
   ;	      for (loop=0;loop<point;loop++)     /* print what's in the buffer */
   ;	
	xor	si,si
	cmp	si,word ptr [bp-12]
	jl	@@19
	jmp	@14@982
@@19:
@14@674:
   ;	
   ;	       {
   ;	         print_chr_to(buf[loop],portnum);
   ;	
	push	word ptr [bp+10]
	mov	al,byte ptr [bp+si-532]
	push	ax
	call	far ptr _print_chr_to
	add	sp,4
   ;	
   ;	         ischar = get_first_char(portnum);
   ;	
	push	word ptr [bp+10]
	mov	bx,word ptr [bp+10]
	mov	cl,2
	shl	bx,cl
	call	dword ptr DGROUP:_a_get_first_char[bx]
	add	sp,2
	mov	di,ax
   ;	
   ;	         if (ischar==19)
   ;	
	cmp	di,19
	jne	short @14@730
   ;	
   ;	          {
   ;	            wait_ch();
   ;	
	push	cs
	call	near ptr _wait_ch
   ;	
   ;	            wait_ch();
   ;	
	push	cs
	call	near ptr _wait_ch
@14@730:
   ;	
   ;	          };
   ;	         if (!dcd_detect(portnum))  leave();
   ;	
	push	word ptr [bp+10]
	mov	bx,word ptr [bp+10]
	mov	cl,2
	shl	bx,cl
	call	dword ptr DGROUP:_a_dcd_detect[bx]
	add	sp,2
	or	ax,ax
	jne	short @14@786
	call	far ptr _leave
@14@786:
   ;	
   ;	         if ((ischar==3) || (ischar==27))
   ;	
	cmp	di,3
	je	short @14@842
	cmp	di,27
	jne	short @14@870
@14@842:
   ;	
   ;	          {
   ;	            point=0;
   ;	
	mov	word ptr [bp-12],0
   ;	
   ;	            int_char(portnum);
   ;	
	push	word ptr [bp+10]
	mov	bx,word ptr [bp+10]
	mov	cl,2
	shl	bx,cl
	call	dword ptr DGROUP:_a_get_nchar[bx]
	add	sp,2
   ;	
   ;	            empty_outbuffer(portnum);
   ;	
	push	word ptr [bp+10]
	mov	bx,word ptr [bp+10]
	mov	cl,2
	shl	bx,cl
	call	dword ptr DGROUP:_a_empty_outbuffer[bx]
	add	sp,2
   ;	
   ;	            special_code(0,portnum);
   ;	
	push	word ptr [bp+10]
	xor	ax,ax
	push	ax
	call	far ptr _special_code
	add	sp,4
   ;	
   ;	            print_cr();
   ;	
	call	far ptr _print_cr
   ;	
   ;	            print_str_cr("--> Aborted");
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+321
	push	ax
	call	far ptr _print_str_cr
	add	sp,4
@14@870:
   ;	
   ;	          };
   ;	       if ((count++)>120)
   ;	
	mov	ax,word ptr [bp-10]
	inc	word ptr [bp-10]
	cmp	ax,120
	jle	short @14@926
   ;	
   ;	         {wait_for_xmit(portnum,30);
   ;	
	mov	ax,30
	push	ax
	push	word ptr [bp+10]
	mov	bx,word ptr [bp+10]
	mov	cl,2
	shl	bx,cl
	call	dword ptr DGROUP:_a_wait_for_xmit[bx]
	add	sp,4
   ;	
   ;	          count=0;
   ;	
	mov	word ptr [bp-10],0
@14@926:
	inc	si
	cmp	si,word ptr [bp-12]
	jge	@@20
	jmp	@14@674
@@20:
@14@982:
	cmp	word ptr [bp-12],0
	je	@@21
	jmp	@14@450
@@21:
   ;	
   ;	         }
   ;	       };
   ;	
   ;	    };
   ;	   special_code(0,portnum);
   ;	
	push	word ptr [bp+10]
	xor	ax,ax
	push	ax
	call	far ptr _special_code
	add	sp,4
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
	assume	cs:IO_TEXT
_print_file_to_cntrl	proc	far
	push	bp
	mov	bp,sp
	sub	sp,616
	push	si
	push	di
   ;	
   ;	                   int pause,int abort,int paging)
   ;	 {
   ;	   FILE *fileptr;
   ;	   long int location = 0;
   ;	
	mov	word ptr [bp-6],0
	mov	word ptr [bp-8],0
   ;	
   ;	   char buf[512];
   ;	   int xmitwait=0;
   ;	
	mov	word ptr [bp-10],0
   ;	
   ;	   int loop, point = 1;
   ;	
	mov	word ptr [bp-12],1
   ;	
   ;	   int ischar;
   ;	   int page_break = 0;
   ;	
	mov	word ptr [bp-14],0
   ;	
   ;	   int count;
   ;	   char *temp_file=filename;
   ;	
	mov	ax,word ptr [bp+8]
	mov	dx,word ptr [bp+6]
	mov	word ptr [bp-16],ax
	mov	word ptr [bp-18],dx
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
	mov	word ptr [bp-20],ax
   ;	
   ;	
   ;	   /* code for .ans files */
   ;	   if ((strlen(filename)<75) && (line_status[portnum].ansi) &&
   ;	
   ;	
   ;	         (line_status[portnum].full_screen_ansi))
   ;	
	les	di,dword ptr [bp+6]
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	dec	cx
	cmp	cx,75
	jb	@@22
	jmp	@15@506
@@22:
	mov	ax,word ptr [bp+10]
	mov	dx,187
	imul	dx
	mov	si,ax
	mov	bx,ax
	cmp	byte ptr DGROUP:_line_status[bx+28],0
	jne	@@23
	jmp	@15@506
@@23:
	cmp	byte ptr DGROUP:_line_status[si+29],0
	jne	@@24
	jmp	@15@506
@@24:
   ;	
   ;	    {  char *end;
   ;	       strcpy(temp_buf,filename);
   ;	
	push	ss
	pop	es
	lea	di,word ptr [bp-616]
	push	es
	mov	es,word ptr [bp+8]
	push	di
	mov	di,word ptr [bp+6]
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	sub	di,cx
	shr	cx,1
	mov	ax,word ptr [bp+8]
	mov	si,word ptr [bp+6]
	pop	di
	pop	es
	push	ds
	mov	ds,ax
	rep 	movsw	
	adc	cx,cx
	rep 	movsb	
	pop	ds
   ;	
   ;	       end=temp_buf+strlen(temp_buf);
   ;	
	push	ss
	pop	es
	lea	di,word ptr [bp-616]
	xor	ax,ax
	mov	cx,-1
	repnz 	scasb	
	not	cx
	dec	cx
	lea	ax,word ptr [bp-616]
	add	cx,ax
	mov	word ptr [bp-22],ss
	mov	word ptr [bp-24],cx
	jmp	short @15@254
@15@226:
   ;	
   ;	
   ;	       while ((*end!='\\') && (*end!='.') && (end>=temp_buf))
   ;	         end--;
   ;	
	dec	word ptr [bp-24]
@15@254:
	les	bx,dword ptr [bp-24]
	cmp	byte ptr es:[bx],92
	je	short @15@338
	cmp	byte ptr es:[bx],46
	je	short @15@338
	lea	ax,word ptr [bp-616]
	cmp	ax,word ptr [bp-24]
	jbe	short @15@226
@15@338:
   ;	
   ;	
   ;	       if (*end=='.') *end=0;
   ;	
	les	bx,dword ptr [bp-24]
	cmp	byte ptr es:[bx],46
	jne	short @15@394
	mov	byte ptr es:[bx],0
@15@394:
   ;	
   ;	
   ;	       strcat(temp_buf,".ANS");
   ;	
	push	ss
	pop	es
	lea	di,word ptr [bp-616]
	mov	si,offset DGROUP:s@+316
	mov	cx,-1
	xor	ax,ax
	repnz 	scasb	
	dec	di
	mov	cx,5
	rep 	movsb	
   ;	
   ;	       temp_file=temp_buf;
   ;	
	lea	ax,word ptr [bp-616]
	mov	word ptr [bp-16],ss
	mov	word ptr [bp-18],ax
   ;	
   ;	       lock_dos(501);
   ;	
	mov	ax,501
	push	ax
	call	far ptr _lock_dos
	add	sp,2
   ;	
   ;	       if ((fileptr=g_fopen(temp_file,"rb","IO#3"))==NULL)   /* ANSI */
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+141
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+305
	push	ax
	push	word ptr [bp-16]
	push	word ptr [bp-18]
	call	far ptr _g_fopen
	add	sp,12
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
	or	ax,dx
	jne	short @15@450
   ;	
   ;	           temp_file=filename;
   ;	
	mov	ax,word ptr [bp+8]
	mov	dx,word ptr [bp+6]
	mov	word ptr [bp-16],ax
	mov	word ptr [bp-18],dx
	jmp	short @15@478
@15@450:
   ;	
   ;	       else
   ;	         {g_fclose(fileptr);
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _g_fclose
	add	sp,4
   ;	
   ;	          ansi=0;
   ;	
	mov	word ptr [bp+12],0
   ;	
   ;	          paging=0;
   ;	
	mov	word ptr [bp+18],0
@15@478:
   ;	
   ;	          }
   ;	       unlock_dos();
   ;	
	call	far ptr _unlock_dos
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
	cmp	word ptr [bp+12],0
	jne	@@25
	jmp	@15@1654
@@25:
	push	word ptr [bp+10]
	mov	ax,1
	push	ax
	call	far ptr _special_code
	add	sp,4
	jmp	@15@1654
@15@562:
   ;	
   ;	   while (point)
   ;	    {
   ;	      fileptr=0;
   ;	
	mov	word ptr [bp-2],0
	mov	word ptr [bp-4],0
	jmp	short @15@730
@15@590:
   ;	
   ;	      while (!fileptr)
   ;	       {
   ;	         lock_dos(502);
   ;	
	mov	ax,502
	push	ax
	call	far ptr _lock_dos
	add	sp,2
   ;	
   ;	         if( (fileptr=g_fopen(temp_file,"rb","IO#4"))==NULL)   /* open the file */
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+333
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+305
	push	ax
	push	word ptr [bp-16]
	push	word ptr [bp-18]
	call	far ptr _g_fopen
	add	sp,12
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
	or	ax,dx
	jne	short @15@674
   ;	
   ;	          {
   ;	            log_error(filename);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	cs
	call	near ptr _log_error
	add	sp,4
   ;	
   ;	            unlock_dos();
   ;	
	call	far ptr _unlock_dos
   ;	
   ;	            if (ansi) special_code(0,portnum);
   ;	
	cmp	word ptr [bp+12],0
	jne	@@26
	jmp	@15@1738
@@26:
	push	word ptr [bp+10]
	xor	ax,ax
	push	ax
	call	far ptr _special_code
	add	sp,4
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@15@674:
   ;	
   ;	            return;
   ;	          };
   ;	         unlock_dos();
   ;	
	call	far ptr _unlock_dos
   ;	
   ;	         if (!fileptr) next_task();
   ;	
	mov	ax,word ptr [bp-4]
	or	ax,word ptr [bp-2]
	jne	short @15@730
	pushf	
	call	far ptr _next_task
@15@730:
	mov	ax,word ptr [bp-4]
	or	ax,word ptr [bp-2]
	je	short @15@590
   ;	
   ;	       };
   ;	      lock_dos(503);
   ;	
	mov	ax,503
	push	ax
	call	far ptr _lock_dos
	add	sp,2
   ;	
   ;	      fseek(fileptr,location,SEEK_SET);         /* go to the portion of */
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _fseek
	add	sp,10
   ;	
   ;	                /* the file we want to read */
   ;	      point = fread(&buf, 1, 512, fileptr);     /* read 512 bytes */
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	mov	ax,512
	push	ax
	mov	ax,1
	push	ax
	push	ss
	lea	ax,word ptr [bp-536]
	push	ax
	call	far ptr _fread
	add	sp,12
	mov	word ptr [bp-12],ax
   ;	
   ;	      g_fclose(fileptr);          /* close the file */
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	far ptr _g_fclose
	add	sp,4
   ;	
   ;	      unlock_dos();
   ;	
	call	far ptr _unlock_dos
   ;	
   ;	      location += 512;          /* go 512 bytes further */
   ;	
	add	word ptr [bp-8],512
	adc	word ptr [bp-6],0
   ;	
   ;	      for (loop=0;loop<point;loop++)     /* print what's in the buffer */
   ;	
	xor	di,di
	cmp	di,word ptr [bp-12]
	jl	@@27
	jmp	@15@1654
@@27:
@15@814:
   ;	
   ;	       {
   ;	         print_chr_to(buf[loop],portnum);
   ;	
	push	word ptr [bp+10]
	mov	al,byte ptr [bp+di-536]
	push	ax
	call	far ptr _print_chr_to
	add	sp,4
   ;	
   ;	         if (paging)
   ;	
	cmp	word ptr [bp+18],0
	je	short @15@1234
   ;	
   ;	          {
   ;	            if (buf[loop] == 10)
   ;	
	cmp	byte ptr [bp+di-536],10
	jne	short @15@1234
   ;	
   ;	             {
   ;	               if (!cur_video_state(portnum))
   ;	
	push	word ptr [bp+10]
	call	far ptr _cur_video_state
	add	sp,2
	cbw	
	or	ax,ax
	jne	short @15@1234
   ;	
   ;	                {
   ;	                 page_break++;
   ;	
	inc	word ptr [bp-14]
   ;	
   ;	                 if (page_break == pagebreaks)
   ;	
	mov	ax,word ptr [bp-14]
	cmp	ax,word ptr [bp-20]
	jne	short @15@1234
   ;	
   ;	                  {
   ;	                    print_string("[ Press Return ]");
   ;	
	push	ds
	mov	ax,offset DGROUP:s@
	push	ax
	call	far ptr _print_string
	add	sp,4
@15@954:
   ;	
   ;	                    do
   ;	                     {
   ;	                       count = wait_ch();
   ;	
	push	cs
	call	near ptr _wait_ch
	mov	si,ax
   ;	
   ;	                       if (((count == 27) || (count == 3)) && abort)
   ;	
	cmp	si,27
	je	short @15@1010
	cmp	si,3
	jne	short @15@1066
@15@1010:
	cmp	word ptr [bp+16],0
	je	short @15@1066
   ;	
   ;	                        {
   ;	                         count = 13;
   ;	
	mov	si,13
   ;	
   ;	                         point = 0;
   ;	
	mov	word ptr [bp-12],0
@15@1066:
   ;	
   ;	                       };
   ;	                     } while (count != 13);
   ;	
	cmp	si,13
	jne	short @15@954
   ;	
   ;	                    for (count=0;count<16;count++)
   ;	
	xor	si,si
@15@1122:
   ;	
   ;	                     print_string(backspacestring);
   ;	
	push	ds
	mov	ax,offset DGROUP:_backspacestring
	push	ax
	call	far ptr _print_string
	add	sp,4
	inc	si
	cmp	si,16
	jl	short @15@1122
   ;	
   ;	                    page_break = 0;
   ;	
	mov	word ptr [bp-14],0
@15@1234:
   ;	
   ;	                  };
   ;	                };
   ;	             };
   ;	          };
   ;	         ischar = get_first_char(portnum);
   ;	
	push	word ptr [bp+10]
	mov	bx,word ptr [bp+10]
	mov	cl,2
	shl	bx,cl
	call	dword ptr DGROUP:_a_get_first_char[bx]
	add	sp,2
	mov	si,ax
   ;	
   ;	         if ((ischar==19) && pause)
   ;	
	cmp	si,19
	jne	short @15@1318
	cmp	word ptr [bp+14],0
	je	short @15@1318
   ;	
   ;	          {
   ;	            wait_ch();
   ;	
	push	cs
	call	near ptr _wait_ch
   ;	
   ;	            wait_ch();
   ;	
	push	cs
	call	near ptr _wait_ch
@15@1318:
   ;	
   ;	          };
   ;	         if (!dcd_detect(portnum)) leave();
   ;	
	push	word ptr [bp+10]
	mov	bx,word ptr [bp+10]
	mov	cl,2
	shl	bx,cl
	call	dword ptr DGROUP:_a_dcd_detect[bx]
	add	sp,2
	or	ax,ax
	jne	short @15@1374
	call	far ptr _leave
@15@1374:
   ;	
   ;	         if (((ischar==3) || (ischar==27)) && abort)
   ;	
	cmp	si,3
	je	short @15@1430
	cmp	si,27
	jne	short @15@1542
@15@1430:
	cmp	word ptr [bp+16],0
	je	short @15@1542
   ;	
   ;	          {
   ;	            point=0;
   ;	
	mov	word ptr [bp-12],0
   ;	
   ;	            int_char(portnum);
   ;	
	push	word ptr [bp+10]
	mov	bx,word ptr [bp+10]
	mov	cl,2
	shl	bx,cl
	call	dword ptr DGROUP:_a_get_nchar[bx]
	add	sp,2
   ;	
   ;	            empty_outbuffer(portnum);
   ;	
	push	word ptr [bp+10]
	mov	bx,word ptr [bp+10]
	mov	cl,2
	shl	bx,cl
	call	dword ptr DGROUP:_a_empty_outbuffer[bx]
	add	sp,2
   ;	
   ;	            if (ansi) special_code(0,portnum);
   ;	
	cmp	word ptr [bp+12],0
	je	short @15@1514
	push	word ptr [bp+10]
	xor	ax,ax
	push	ax
	call	far ptr _special_code
	add	sp,4
@15@1514:
   ;	
   ;	            print_cr();
   ;	
	call	far ptr _print_cr
   ;	
   ;	            print_str_cr("--> Aborted");
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+321
	push	ax
	call	far ptr _print_str_cr
	add	sp,4
@15@1542:
   ;	
   ;	          };
   ;	        if ((xmitwait++)>120)
   ;	
	mov	ax,word ptr [bp-10]
	inc	word ptr [bp-10]
	cmp	ax,120
	jle	short @15@1598
   ;	
   ;	           {wait_for_xmit(portnum,30);
   ;	
	mov	ax,30
	push	ax
	push	word ptr [bp+10]
	mov	bx,word ptr [bp+10]
	mov	cl,2
	shl	bx,cl
	call	dword ptr DGROUP:_a_wait_for_xmit[bx]
	add	sp,4
   ;	
   ;	            xmitwait=0;
   ;	
	mov	word ptr [bp-10],0
@15@1598:
	inc	di
	cmp	di,word ptr [bp-12]
	jge	@@28
	jmp	@15@814
@@28:
@15@1654:
	cmp	word ptr [bp-12],0
	je	@@29
	jmp	@15@562
@@29:
   ;	
   ;	           }
   ;	       };
   ;	    };
   ;	   if (ansi) special_code(0,portnum);
   ;	
	cmp	word ptr [bp+12],0
	je	short @15@1738
	push	word ptr [bp+10]
	xor	ax,ax
	push	ax
	call	far ptr _special_code
	add	sp,4
@15@1738:
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
	assume	cs:IO_TEXT
_is_abuf_empty	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
   ;	
   ;	 {
   ;	   return (abuf_status[id].abufwrite == abuf_status[id].abufread);
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,24
	imul	dx
	mov	word ptr [bp-2],ax
	mov	bx,ax
	mov	ax,word ptr DGROUP:_abuf_status[bx+18]
	mov	dx,word ptr DGROUP:_abuf_status[bx+16]
	mov	bx,word ptr [bp-2]
	cmp	ax,word ptr DGROUP:_abuf_status[bx+14]
	jne	short @16@114
	cmp	dx,word ptr DGROUP:_abuf_status[bx+12]
	jne	short @16@114
	mov	ax,1
	jmp	short @16@142
@16@114:
	xor	ax,ax
@16@142:
   ;	
   ;	 };
   ;	
	mov	sp,bp
	pop	bp
	ret	
_is_abuf_empty	endp
   ;	
   ;	void aput_char(struct abuf_type *abuf, char c)
   ;	
	assume	cs:IO_TEXT
_aput_char	proc	far
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	 {
   ;	   *abuf->abufwrite++ = c;     /* write the character */
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr es:[bx+18]
	mov	si,word ptr es:[bx+16]
	inc	word ptr es:[bx+16]
	mov	dl,byte ptr [bp+10]
	mov	es,ax
	mov	byte ptr es:[si],dl
   ;	
   ;	   if (((char near *)abuf->abufwrite) >= ((char near *)abuf->abufend)) /* if were at the end of the buffer */
   ;	
	mov	es,word ptr [bp+8]
	mov	ax,word ptr es:[bx+16]
	cmp	ax,word ptr es:[bx+8]
	jb	short @17@86
   ;	
   ;	     abuf->abufwrite = abuf->abuffer;   /* go back to the beginning */
   ;	
	mov	ax,word ptr es:[bx+6]
	mov	dx,word ptr es:[bx+4]
	mov	word ptr es:[bx+18],ax
	mov	word ptr es:[bx+16],dx
@17@86:
   ;	
   ;	   if (((char near *)abuf->abufwrite) == ((char near*)abuf->abufread))   /* if were at the the end of the */
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr es:[bx+16]
	cmp	ax,word ptr es:[bx+12]
	jne	short @17@170
   ;	
   ;	    {                                       /* buffer, move the read pointer */
   ;	      abuf->abufread++;                     /* ahead a bit */
   ;	
	inc	word ptr es:[bx+12]
   ;	
   ;	      if (((char near *)abuf->abufread) == ((char near *)abuf->abufend))
   ;	
	mov	ax,word ptr es:[bx+12]
	cmp	ax,word ptr es:[bx+8]
	jne	short @17@170
   ;	
   ;	        abuf->abufread = abuf->abuffer;
   ;	
	mov	ax,word ptr es:[bx+6]
	mov	dx,word ptr es:[bx+4]
	mov	word ptr es:[bx+14],ax
	mov	word ptr es:[bx+12],dx
@17@170:
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
	assume	cs:IO_TEXT
_aput_into_buffer	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
   ;	
   ;	                      int parm2, int parm3, int parm4)
   ;	{
   ;	   struct abuf_type near *abuf = &abuf_status[id];
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,24
	imul	dx
	add	ax,offset DGROUP:_abuf_status
	mov	si,ax
   ;	
   ;	   int tempDS = _DS;
   ;	
	mov	di,ds
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
	mov	ds,di
   ;	
   ;	      return;
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
@18@86:
   ;	
   ;	    };                          /* dont post into buffer that isnt read */
   ;	
   ;	   disable();
   ;	
	db	250
	jmp	short @18@142
@18@114:
   ;	
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
	db	250
@18@142:
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
	call	far ptr _set_death_off
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
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,0x55);
   ;	
	mov	al,85
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,0xBC);
   ;	
	mov	al,188
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,tswitch);     /* put sender and channel # */
   ;	
	mov	al,byte ptr DGROUP:_tswitch
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,channel);
   ;	
	mov	al,byte ptr [bp+12]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,parm1);
   ;	
	mov	al,byte ptr [bp+14]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,parm2);
   ;	
	mov	al,byte ptr [bp+16]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,parm3);
   ;	
	mov	al,byte ptr [bp+18]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,parm4);
   ;	
	mov	al,byte ptr [bp+20]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
	jmp	short @18@226
@18@198:
   ;	
   ;	
   ;	   while (*string) aput_char(abuf,*string++);   /* put the string into */
   ;	
	les	bx,dword ptr [bp+8]
	inc	word ptr [bp+8]
	mov	al,byte ptr es:[bx]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
@18@226:
	les	bx,dword ptr [bp+8]
	cmp	byte ptr es:[bx],0
	jne	short @18@198
   ;	
   ;	   aput_char(abuf,0);               /* the buffer with a zero */
   ;	
	mov	al,0
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
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
	call	far ptr _set_death_on
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_aput_into_buffer	endp
   ;	
   ;	void aput_append_into_buffer(int id, int channel, int parm1,
   ;	
	assume	cs:IO_TEXT
_aput_append_into_buffer	proc	far
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
	push	di
   ;	
   ;	                      int parm2, int parm3, int parm4, int no_str, ...)
   ;	 {
   ;	   struct abuf_type near *abuf = &abuf_status[id];
   ;	
	mov	ax,word ptr [bp+6]
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
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@19@86:
   ;	
   ;	    };                          /* dont post into buffer that isnt read */
   ;	
   ;	   disable();
   ;	
	db	250
	jmp	short @19@142
@19@114:
   ;	
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
	db	250
@19@142:
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
	call	far ptr _set_death_off
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
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,0x55);
   ;	
	mov	al,85
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,0xBC);
   ;	
	mov	al,188
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,tswitch);     /* put sender and channel # */
   ;	
	mov	al,byte ptr DGROUP:_tswitch
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,channel);
   ;	
	mov	al,byte ptr [bp+8]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,parm1);
   ;	
	mov	al,byte ptr [bp+10]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,parm2);
   ;	
	mov	al,byte ptr [bp+12]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,parm3);
   ;	
	mov	al,byte ptr [bp+14]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,parm4);
   ;	
	mov	al,byte ptr [bp+16]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	
   ;	   va_start(ap,no_str);
   ;	
	lea	ax,word ptr [bp+20]
	mov	word ptr [bp-2],ss
	mov	word ptr [bp-4],ax
	jmp	short @19@310
@19@198:
   ;	
   ;	
   ;	   while (no_str>0)
   ;	   {
   ;	     string = (char far *)va_arg(ap,char far *);
   ;	
	add	word ptr [bp-4],4
	les	bx,dword ptr [bp-4]
	mov	ax,word ptr es:[bx-2]
	mov	dx,word ptr es:[bx-4]
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
	jmp	short @19@254
@19@226:
   ;	
   ;	     while (*string) aput_char(abuf,*string++);   /* put the string into */
   ;	
	les	bx,dword ptr [bp-8]
	inc	word ptr [bp-8]
	mov	al,byte ptr es:[bx]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
@19@254:
	les	bx,dword ptr [bp-8]
	cmp	byte ptr es:[bx],0
	jne	short @19@226
   ;	
   ;	     no_str--;
   ;	
	dec	word ptr [bp+18]
@19@310:
	cmp	word ptr [bp+18],0
	jg	short @19@198
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
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
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
	call	far ptr _set_death_on
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
	assume	cs:IO_TEXT
_aput_vargs_into_buffer	proc	far
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	push	di
	mov	di,word ptr [bp+18]
   ;	
   ;	                      int parm2, int parm3, int parm4, int no_str,
   ;	                      va_list far *ap)
   ;	 {
   ;	   struct abuf_type near *abuf = &abuf_status[id];
   ;	
	mov	ax,word ptr [bp+6]
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
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@20@86:
   ;	
   ;	    };                          /* dont post into buffer that isnt read */
   ;	
   ;	   disable();
   ;	
	db	250
	jmp	short @20@142
@20@114:
   ;	
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
	db	250
@20@142:
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
	call	far ptr _set_death_off
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
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,0x55);
   ;	
	mov	al,85
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,0xBC);
   ;	
	mov	al,188
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,tswitch);     /* put sender and channel # */
   ;	
	mov	al,byte ptr DGROUP:_tswitch
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,channel);
   ;	
	mov	al,byte ptr [bp+8]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,parm1);
   ;	
	mov	al,byte ptr [bp+10]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,parm2);
   ;	
	mov	al,byte ptr [bp+12]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,parm3);
   ;	
	mov	al,byte ptr [bp+14]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
   ;	
   ;	   aput_char(abuf,parm4);
   ;	
	mov	al,byte ptr [bp+16]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
	jmp	short @20@310
@20@198:
   ;	
   ;	
   ;	   while (no_str>0)
   ;	   {
   ;	     string = (char far *)va_arg(ap,char far *);
   ;	
	add	word ptr [bp+20],4
	les	bx,dword ptr [bp+20]
	mov	ax,word ptr es:[bx-2]
	mov	dx,word ptr es:[bx-4]
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
	jmp	short @20@254
@20@226:
   ;	
   ;	     while (*string) aput_char(abuf,*string++);   /* put the string into */
   ;	
	les	bx,dword ptr [bp-6]
	inc	word ptr [bp-6]
	mov	al,byte ptr es:[bx]
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
@20@254:
	les	bx,dword ptr [bp-6]
	cmp	byte ptr es:[bx],0
	jne	short @20@226
   ;	
   ;	     no_str--;
   ;	
	dec	di
@20@310:
	or	di,di
	jg	short @20@198
   ;	
   ;	   }
   ;	
   ;	   aput_char(abuf,0);               /* the buffer with a zero */
   ;	
	mov	al,0
	push	ax
	push	ds
	push	si
	push	cs
	call	near ptr _aput_char
	add	sp,6
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
	call	far ptr _set_death_on
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
	assume	cs:IO_TEXT
_aclear_buffer	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
   ;	
   ;	 {
   ;	   struct abuf_type *abuf = &abuf_status[id];
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,24
	imul	dx
	add	ax,offset DGROUP:_abuf_status
	mov	word ptr [bp-2],ds
	mov	word ptr [bp-4],ax
   ;	
   ;	
   ;	   if (!abuf->active) return;
   ;	
	les	bx,dword ptr [bp-4]
	cmp	word ptr es:[bx+2],0
	je	short @21@170
   ;	
   ;	                                /* dont change buffer that isnt read */
   ;	
   ;	   disable();
   ;	
	db	250
	jmp	short @21@114
@21@86:
   ;	
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
	db	250
@21@114:
	les	bx,dword ptr [bp-4]
	cmp	word ptr es:[bx],0
	jne	short @21@86
   ;	
   ;	    };
   ;	   abuf->used = 1;
   ;	
	mov	word ptr es:[bx],1
   ;	
   ;	   set_death_off();
   ;	
	call	far ptr _set_death_off
   ;	
   ;	   enable();
   ;	
	db	251
   ;	
   ;	   abuf->abufread = abuf->abufwrite;
   ;	
	les	bx,dword ptr [bp-4]
	mov	ax,word ptr es:[bx+18]
	mov	dx,word ptr es:[bx+16]
	mov	word ptr es:[bx+14],ax
	mov	word ptr es:[bx+12],dx
   ;	
   ;	   abuf->used = 0;              /* let someone else use it */
   ;	
	mov	word ptr es:[bx],0
   ;	
   ;	
   ;	   set_death_on();
   ;	
	call	far ptr _set_death_on
@21@170:
   ;	
   ;	  };
   ;	
	mov	sp,bp
	pop	bp
	ret	
_aclear_buffer	endp
   ;	
   ;	unsigned char aget_char(struct abuf_type *abuf)
   ;	
	assume	cs:IO_TEXT
_aget_char	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
   ;	
   ;	  {
   ;	    char temp;
   ;	
   ;	    if (((char near *)abuf->abufread) == ((char near *)abuf->abufwrite))
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr es:[bx+12]
	cmp	ax,word ptr es:[bx+16]
	jne	short @22@86
   ;	
   ;	     return 0;
   ;	
	mov	al,0
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@22@86:
   ;	
   ;	    temp = *abuf->abufread++;
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr es:[bx+14]
	mov	si,word ptr es:[bx+12]
	inc	word ptr es:[bx+12]
	mov	es,ax
	mov	al,byte ptr es:[si]
	mov	byte ptr [bp-1],al
   ;	
   ;	    if (((char near *)abuf->abufread) >= ((char near *)abuf->abufend))
   ;	
	mov	es,word ptr [bp+8]
	mov	ax,word ptr es:[bx+12]
	cmp	ax,word ptr es:[bx+8]
	jb	short @22@142
   ;	
   ;	     abuf->abufread = abuf->abuffer;
   ;	
	mov	ax,word ptr es:[bx+6]
	mov	dx,word ptr es:[bx+4]
	mov	word ptr es:[bx+14],ax
	mov	word ptr es:[bx+12],dx
@22@142:
   ;	
   ;	    return(temp);
   ;	
	mov	al,byte ptr [bp-1]
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
	assume	cs:IO_TEXT
_aget_abuffer	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
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
	mov	byte ptr [bp-1],0
   ;	
   ;	    int tempDS = _DS;
   ;	
	mov	di,ds
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
	jne	short @23@86
   ;	
   ;	     {
   ;	       _DS = tempDS;
   ;	
	mov	ds,di
   ;	
   ;	       return 0;
   ;	
	xor	ax,ax
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@23@86:
   ;	
   ;	     };
   ;	    disable();
   ;	
	db	250
	jmp	short @23@142
@23@114:
   ;	
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
	db	250
@23@142:
	cmp	word ptr [si],0
	jne	short @23@114
   ;	
   ;	     };
   ;	    abuf->used = 1;
   ;	
	mov	word ptr [si],1
   ;	
   ;	    set_death_off();
   ;	
	call	far ptr _set_death_off
   ;	
   ;	    enable();
   ;	
	db	251
   ;	
   ;	
   ;	    if (aget_char(abuf) != 0xAA) abort = 1;
   ;	
	push	ds
	push	si
	push	cs
	call	near ptr _aget_char
	add	sp,4
	cmp	al,170
	je	short @23@226
	mov	byte ptr [bp-1],1
@23@226:
   ;	
   ;	    if (!abort) if (aget_char(abuf) != 0x55) abort = 1;
   ;	
	mov	al,byte ptr [bp-1]
	cbw	
	or	ax,ax
	jne	short @23@310
	push	ds
	push	si
	push	cs
	call	near ptr _aget_char
	add	sp,4
	cmp	al,85
	je	short @23@310
	mov	byte ptr [bp-1],1
@23@310:
   ;	
   ;	    if (!abort) if (aget_char(abuf) != 0xBC) abort = 1;
   ;	
	mov	al,byte ptr [bp-1]
	cbw	
	or	ax,ax
	jne	short @23@394
	push	ds
	push	si
	push	cs
	call	near ptr _aget_char
	add	sp,4
	cmp	al,188
	je	short @23@394
	mov	byte ptr [bp-1],1
@23@394:
   ;	
   ;	    if (abort)
   ;	
	cmp	byte ptr [bp-1],0
	je	short @23@478
@23@422:
   ;	
   ;	     {
   ;	       while (aget_char(abuf));
   ;	
	push	ds
	push	si
	push	cs
	call	near ptr _aget_char
	add	sp,4
	or	al,al
	jne	short @23@422
   ;	
   ;	       abuf->used = 0;
   ;	
	mov	word ptr [si],0
   ;	
   ;	       set_death_on();
   ;	
	call	far ptr _set_death_on
   ;	
   ;	       _DS = tempDS;
   ;	
	mov	ds,di
   ;	
   ;	       return 0;
   ;	
	xor	ax,ax
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@23@478:
   ;	
   ;	     };
   ;	    *sentby = aget_char(abuf);
   ;	
	push	ds
	push	si
	push	cs
	call	near ptr _aget_char
	add	sp,4
	mov	ah,0
	les	bx,dword ptr [bp+6]
	mov	word ptr es:[bx],ax
   ;	
   ;	    *channel = aget_char(abuf);
   ;	
	push	ds
	push	si
	push	cs
	call	near ptr _aget_char
	add	sp,4
	mov	ah,0
	les	bx,dword ptr [bp+10]
	mov	word ptr es:[bx],ax
   ;	
   ;	    *parm1 = aget_char(abuf);
   ;	
	push	ds
	push	si
	push	cs
	call	near ptr _aget_char
	add	sp,4
	mov	ah,0
	les	bx,dword ptr [bp+18]
	mov	word ptr es:[bx],ax
   ;	
   ;	    *parm2 = aget_char(abuf);
   ;	
	push	ds
	push	si
	push	cs
	call	near ptr _aget_char
	add	sp,4
	mov	ah,0
	les	bx,dword ptr [bp+22]
	mov	word ptr es:[bx],ax
   ;	
   ;	    *parm3 = aget_char(abuf);
   ;	
	push	ds
	push	si
	push	cs
	call	near ptr _aget_char
	add	sp,4
	mov	ah,0
	les	bx,dword ptr [bp+26]
	mov	word ptr es:[bx],ax
   ;	
   ;	    *parm4 = aget_char(abuf);
   ;	
	push	ds
	push	si
	push	cs
	call	near ptr _aget_char
	add	sp,4
	mov	ah,0
	les	bx,dword ptr [bp+30]
	mov	word ptr es:[bx],ax
@23@506:
   ;	
   ;	    do
   ;	      {
   ;	        *string++ = (temp = aget_char(abuf));
   ;	
	push	ds
	push	si
	push	cs
	call	near ptr _aget_char
	add	sp,4
	mov	dl,al
	les	bx,dword ptr [bp+14]
	mov	byte ptr es:[bx],al
	inc	word ptr [bp+14]
   ;	
   ;	      } while (temp);
   ;	
	or	dl,dl
	jne	short @23@506
   ;	
   ;	    abuf->used = 0;
   ;	
	mov	word ptr [si],0
   ;	
   ;	
   ;	    _DS = tempDS;
   ;	
	mov	ds,di
   ;	
   ;	    set_death_on();
   ;	
	call	far ptr _set_death_on
   ;	
   ;	    return 1;
   ;	
	mov	ax,1
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
	assume	cs:IO_TEXT
_aback_abuffer	proc	far
	push	bp
	mov	bp,sp
	sub	sp,14
	push	si
	push	di
	mov	di,word ptr [bp+8]
   ;	
   ;	  {
   ;	    struct abuf_type near *abuf = &abuf_status[id];
   ;	
	mov	ax,word ptr [bp+6]
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
	jne	short @24@86
   ;	
   ;	     {
   ;	       _DS = tempDS;
   ;	
   ;	
   ;	       return 0;
   ;	
	mov	ds,word ptr [bp-6]
	xor	ax,ax
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
@24@86:
   ;	
   ;	     };
   ;	    disable();
   ;	
	db	250
	jmp	short @24@142
@24@114:
   ;	
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
	db	250
@24@142:
	cmp	word ptr [si],0
	jne	short @24@114
   ;	
   ;	     };
   ;	    abuf->used = 1;
   ;	
	mov	word ptr [si],1
   ;	
   ;	    set_death_off();
   ;	
	call	far ptr _set_death_off
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
	dec	ax
	mov	cx,ax
	mov	ax,word ptr [si+14]
	mov	word ptr [bp-8],ax
	mov	ax,word ptr [si+12]
	mov	word ptr [bp-10],ax
	mov	ax,word ptr [si+4]
	mov	word ptr [bp-12],ax
	mov	ax,word ptr [si+10]
	mov	word ptr [bp-14],ax
	jmp	short @24@338
@24@226:
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
	mov	ax,word ptr [bp-12]
	cmp	ax,word ptr [bp-4]
	jbe	short @24@282
	mov	ax,word ptr [bp-14]
	mov	word ptr [bp-2],ax
	mov	word ptr [bp-4],cx
@24@282:
   ;	
   ;	       if (!(*back_ptr)) lines--;
   ;	
	les	bx,dword ptr [bp-4]
	mov	al,byte ptr es:[bx]
	cbw	
	or	ax,ax
	jne	short @24@338
	dec	di
@24@338:
	or	di,di
	jle	short @24@422
	mov	ax,word ptr [bp-8]
	mov	dx,word ptr [bp-10]
	cmp	ax,word ptr [bp-2]
	jne	short @24@226
	cmp	dx,word ptr [bp-4]
	jne	short @24@226
@24@422:
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
	call	far ptr _set_death_on
   ;	
   ;	    return 1;
   ;	
	mov	ax,1
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
	assume	cs:IO_TEXT
_initabuffer	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	mov	si,word ptr [bp+6]
   ;	
   ;	 {
   ;	   struct abuf_type *abuf = &abuf_status[tswitch];
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,24
	imul	dx
	add	ax,offset DGROUP:_abuf_status
	mov	word ptr [bp-2],ds
	mov	word ptr [bp-4],ax
   ;	
   ;	
   ;	   abuf->used = 0;
   ;	
	les	bx,dword ptr [bp-4]
	mov	word ptr es:[bx],0
   ;	
   ;	   lock_dos(504);
   ;	
	mov	ax,504
	push	ax
	call	far ptr _lock_dos
	add	sp,2
   ;	
   ;	   abuf->abuffer = g_malloc_main_only(bufsize,"ABUFFER");
   ;	
	push	ds
	mov	ax,offset DGROUP:s@+338
	push	ax
	mov	ax,si
	cwd	
	push	dx
	push	ax
	call	far ptr _g_malloc_main_only
	add	sp,8
	les	bx,dword ptr [bp-4]
	mov	word ptr es:[bx+6],dx
	mov	word ptr es:[bx+4],ax
   ;	
   ;	   unlock_dos();
   ;	
	call	far ptr _unlock_dos
   ;	
   ;	   if (!abuf->abuffer) return;
   ;	
	les	bx,dword ptr [bp-4]
	mov	ax,word ptr es:[bx+4]
	or	ax,word ptr es:[bx+6]
	je	short @25@86
   ;	
   ;	   abuf->abufend = abuf->abuffer + bufsize;
   ;	
	les	bx,dword ptr [bp-4]
	mov	ax,word ptr es:[bx+6]
	mov	dx,word ptr es:[bx+4]
	add	dx,si
	mov	word ptr es:[bx+10],ax
	mov	word ptr es:[bx+8],dx
   ;	
   ;	   abuf->abufread = abuf->abuffer;
   ;	
	mov	dx,word ptr es:[bx+4]
	mov	word ptr es:[bx+14],ax
	mov	word ptr es:[bx+12],dx
   ;	
   ;	   abuf->abufwrite = abuf->abuffer;
   ;	
	mov	word ptr es:[bx+18],ax
	mov	word ptr es:[bx+16],dx
   ;	
   ;	   abuf->max_buffer = bufsize - 1;
   ;	
	mov	ax,si
	dec	ax
	mov	word ptr es:[bx+22],ax
   ;	
   ;	   abuf->num_buffer = 0;
   ;	
	mov	word ptr es:[bx+20],0
   ;	
   ;	   abuf->active = 1;
   ;	
	mov	word ptr es:[bx+2],1
@25@86:
   ;	
   ;	 };
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_initabuffer	endp
   ;	
   ;	void dealloc_abuf(int portnum)
   ;	
	assume	cs:IO_TEXT
_dealloc_abuf	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	mov	si,word ptr [bp+6]
   ;	
   ;	{
   ;	
   ;	   abuf_status[portnum].active=0;
   ;	
	mov	ax,si
	mov	dx,24
	imul	dx
	mov	word ptr [bp-2],ax
	mov	bx,ax
	mov	word ptr DGROUP:_abuf_status[bx+2],0
   ;	
   ;	   if (abuf_status[portnum].abuffer)
   ;	
	mov	bx,word ptr [bp-2]
	mov	ax,word ptr DGROUP:_abuf_status[bx+4]
	or	ax,word ptr DGROUP:_abuf_status[bx+6]
	je	short @26@86
   ;	
   ;	    {
   ;	      lock_dos(505);
   ;	
	mov	ax,505
	push	ax
	call	far ptr _lock_dos
	add	sp,2
   ;	
   ;	      g_free(abuf_status[portnum].abuffer);
   ;	
	mov	ax,si
	mov	dx,24
	imul	dx
	mov	bx,ax
	push	word ptr DGROUP:_abuf_status[bx+6]
	push	word ptr DGROUP:_abuf_status[bx+4]
	call	far ptr _g_free
	add	sp,4
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
	call	far ptr _unlock_dos
@26@86:
   ;	
   ;	    }
   ;	};
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_dealloc_abuf	endp
   ;	
   ;	int get_string_cntrl(char *string, int limit, char echo, char back_to_end,
   ;	
	assume	cs:IO_TEXT
_get_string_cntrl	proc	far
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
	jmp	@27@954
@27@58:
   ;	
   ;	
   ;	   while (flag)             /* wait while editing */
   ;	    {
   ;	      key = wait_ch();
   ;	
	push	cs
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
	push	ds
	mov	ax,offset DGROUP:_backspacestring
	push	ax
	call	far ptr _print_string
	add	sp,4
   ;	
   ;	       }               /* and go back one */
   ;	
	jmp	short @27@226
@27@170:
   ;	
   ;	      else
   ;	       if (back_to_end) { flag = 0; reason=1; }
   ;	
	cmp	byte ptr [bp+14],0
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
	cmp	byte ptr [bp+16],0
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
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(13);
   ;	
	mov	al,13
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_chr(10);
   ;	
	mov	al,10
	push	ax
	call	far ptr _print_chr
	add	sp,2
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
	mov	al,byte ptr [bp+18]
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
	cmp	byte ptr [bp+20],0
	je	short @27@506
@27@478:
   ;	
   ;	             {
   ;	              print_chr(13);
   ;	
	mov	al,13
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	              print_chr(10);
   ;	
	mov	al,10
	push	ax
	call	far ptr _print_chr
	add	sp,2
@27@506:
   ;	
   ;	             };
   ;	          };
   ;	       };
   ;	      if (((key >= 32) && (key <= 126)) && (pos < limit))
   ;	
	cmp	word ptr [bp-2],32
	jge	@@30
	jmp	@27@954
@@30:
	cmp	word ptr [bp-2],126
	jle	@@31
	jmp	@27@954
@@31:
	cmp	si,word ptr [bp+10]
	jge	short @27@954
   ;	
   ;	       {
   ;	         if ((upcase) && (key>='a') && (key<='z')) key -= 32;
   ;	
	cmp	byte ptr [bp+22],0
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
	cmp	byte ptr [bp+24],0
	je	short @27@786
	cmp	word ptr [bp-2],48
	jl	short @27@954
	cmp	word ptr [bp-2],57
	jg	short @27@954
@27@786:
   ;	
   ;	          {
   ;	                                /* insert the character if there's room */
   ;	           *(string+pos) = key;
   ;	
	les	bx,dword ptr [bp+6]
	add	bx,si
	mov	al,byte ptr [bp-2]
	mov	byte ptr es:[bx],al
   ;	
   ;	           if (key == '+')        /* if +, don't let it be typed normally */
   ;	
	cmp	word ptr [bp-2],43
	jne	short @27@842
   ;	
   ;	            {
   ;	              print_chr(key);     /* print the character with a space */
   ;	
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	              print_chr(32);      /* and a backspace */
   ;	
	mov	al,32
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	              print_chr(8);
   ;	
	mov	al,8
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            }
   ;	
	jmp	short @27@926
@27@842:
   ;	
   ;	            else
   ;	            if (echo) print_chr(echo);
   ;	
	cmp	byte ptr [bp+12],0
	je	short @27@898
	mov	al,byte ptr [bp+12]
	push	ax
	call	far ptr _print_chr
	add	sp,2
	jmp	short @27@926
@27@898:
   ;	
   ;	             else print_chr(key);       /* otherwise, print it normally */
   ;	
	mov	al,byte ptr [bp-2]
	push	ax
	call	far ptr _print_chr
	add	sp,2
@27@926:
   ;	
   ;	           pos++;
   ;	
	inc	si
@27@954:
	or	di,di
	je	@@32
	jmp	@27@58
@@32:
   ;	
   ;	          };
   ;	       };
   ;	    };
   ;	   *(string+pos) = 0;           /* mark end of the string */
   ;	
	les	bx,dword ptr [bp+6]
	add	bx,si
	mov	byte ptr es:[bx],0
   ;	
   ;	   return reason;
   ;	
	mov	ax,word ptr [bp-4]
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
	assume	cs:IO_TEXT
_check_for_privates	proc	far
	push	bp
	mov	bp,sp
	sub	sp,462
	push	si
   ;	
   ;	 {
   ;	   int not_print_priv = 1;
   ;	
	mov	si,1
	jmp	short @28@506
@28@58:
   ;	
   ;	   int channel, id, type, code1, code2,code3;
   ;	   int print_mesg;
   ;	   char s[STRING_SIZE];
   ;	   while (aget_abuffer(&id,&channel,s,&type,&code1,&code2,&code3))
   ;	    {
   ;	       print_mesg=0;
   ;	
	xor	ax,ax
   ;	
   ;	
   ;	       switch (type)
   ;	
	mov	bx,word ptr [bp-6]
	dec	bx
	cmp	bx,11
	ja	short @28@394
	shl	bx,1
	jmp	word ptr cs:@28@C498[bx]
@28@170:
   ;	
   ;	       {
   ;	       case 1 :  print_mesg++;  /* privates */
   ;	
	inc	ax
   ;	
   ;	                 break;
   ;	
	jmp	short @28@394
@28@198:
   ;	
   ;	       case 5 :  if (code2==7) print_mesg++;   /* system messsage */
   ;	
	cmp	word ptr [bp-10],7
	jne	short @28@394
	inc	ax
	jmp	short @28@394
@28@254:
   ;	
   ;	                 break;
   ;	       case 7 :  print_mesg++;  /* user alert mesg / timeout */
   ;	
	inc	ax
   ;	
   ;	                 break;
   ;	
	jmp	short @28@394
@28@282:
   ;	
   ;	       case 8 :  if (code3!=8)
   ;	
	cmp	word ptr [bp-12],8
	je	short @28@394
   ;	
   ;	                    print_mesg++;  /* personal system message */
   ;	
	inc	ax
	jmp	short @28@394
@28@338:
   ;	
   ;	                 break;
   ;	       case 10:  print_mesg++;  /* link privates */
   ;	
	inc	ax
   ;	
   ;	                 break;
   ;	
	jmp	short @28@394
@28@366:
   ;	
   ;	       case 12:  print_mesg++;
   ;	
	inc	ax
   ;	
   ;	                 break;
   ;	
@28@394:
   ;	
   ;	      }
   ;	
   ;	      if (print_mesg)
   ;	
	or	ax,ax
	je	short @28@506
   ;	
   ;	       {
   ;	       if (not_print_priv)
   ;	
	or	si,si
	je	short @28@478
   ;	
   ;	          {
   ;	            not_print_priv = 0;
   ;	
	xor	si,si
   ;	
   ;	            print_chr(7);
   ;	
	mov	al,7
	push	ax
	call	far ptr _print_chr
	add	sp,2
   ;	
   ;	            print_cr();
   ;	
	call	far ptr _print_cr
@28@478:
   ;	
   ;	          };
   ;	
   ;	         wrap_line(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-462]
	push	ax
	call	far ptr _wrap_line
	add	sp,4
@28@506:
	push	ss
	lea	ax,word ptr [bp-12]
	push	ax
	push	ss
	lea	ax,word ptr [bp-10]
	push	ax
	push	ss
	lea	ax,word ptr [bp-8]
	push	ax
	push	ss
	lea	ax,word ptr [bp-6]
	push	ax
	push	ss
	lea	ax,word ptr [bp-462]
	push	ax
	push	ss
	lea	ax,word ptr [bp-2]
	push	ax
	push	ss
	lea	ax,word ptr [bp-4]
	push	ax
	push	cs
	call	near ptr _aget_abuffer
	add	sp,28
	or	ax,ax
	je	@@33
	jmp	@28@58
@@33:
   ;	
   ;	       }
   ;	    }
   ;	 };
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_check_for_privates	endp
@28@C498	label	word
	dw	@28@170
	dw	@28@394
	dw	@28@394
	dw	@28@394
	dw	@28@198
	dw	@28@394
	dw	@28@254
	dw	@28@282
	dw	@28@394
	dw	@28@338
	dw	@28@394
	dw	@28@366
	?debug	C E9
	?debug	C FA99000000
IO_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'[ Press Return ]'
	db	0
	db	'Command not available.'
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
	db	'(%02d,%03d) %s'
	db	10
	db	0
	db	'Date: %s'
	db	0
	db	'TEMP'
	db	0
	db	'Error: %s'
	db	0
	db	'error.log'
	db	0
	db	'IO#2'
	db	0
	db	'Date: %s'
	db	10
	db	0
	db	'IO#3'
	db	0
	db	'#%03d (%02d) | '
	db	0
	db	'%a %b %d %Y %I:%M:%S %p'
	db	0
	db	'%s | '
	db	0
	db	'LOG\KILL.LOG'
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
	db	'rb'
	db	0
	db	'FILES#4'
	db	0
	db	'.ANS'
	db	0
	db	'--> Aborted'
	db	0
	db	'IO#4'
	db	0
	db	'ABUFFER'
	db	0
_DATA	ends
IO_TEXT	segment byte public 'CODE'
IO_TEXT	ends
	public	_aget_char
	extrn	F_LXMUL@:far
	extrn	_limit_carrots:far
	extrn	_remove_flashing:far
	extrn	_remove_ansi:far
	extrn	_print_invalid_command:far
	extrn	_exec:far
	extrn	_user_options:word
	extrn	_user_lines:word
	extrn	_test_bit:far
	extrn	_remake_handleline:far
	extrn	_set_death_on:far
	extrn	_set_death_off:far
	extrn	_clear_call_on_logoff:far
	extrn	_leave:far
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
	extrn	_cur_video_state:far
	extrn	_wrap_line:far
	extrn	_special_code:far
	extrn	_print_cr:far
	extrn	_print_str_cr:far
	extrn	_print_string:far
	extrn	_print_chr_to:far
	extrn	_print_chr:far
	extrn	_a_empty_outbuffer:dword
	extrn	_a_wait_for_xmit:dword
	extrn	_a_get_nchar:dword
	extrn	_a_get_first_char:dword
	extrn	_a_dcd_detect:dword
	extrn	_g_fclose:far
	extrn	_g_fopen:far
	extrn	_g_free:far
	extrn	_g_malloc_main_only:far
	extrn	_unlock_dos:far
	extrn	_lock_dos:far
	extrn	_next_task:far
	extrn	_islocked:far
	extrn	_tasking:byte
	extrn	_tswitch:word
	extrn	_strftime:far
	extrn	_time:far
	extrn	_localtime:far
	extrn	_ctime:far
	extrn	_toupper:far
	extrn	__strerror:far
	extrn	_sprintf:far
	extrn	_printf:far
	extrn	_perror:far
	extrn	_ftell:far
	extrn	_fseek:far
	extrn	_fread:far
	extrn	_fprintf:far
_s@	equ	s@
	end
