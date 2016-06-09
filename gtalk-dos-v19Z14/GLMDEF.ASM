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
	?debug	S "glmdef.c"
	?debug	C E9C06E691B08676C6D6465662E63
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C737464696F2E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C5F646566732E68
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C5F6E66696C652E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C5F6E756C6C2E68
	?debug	C E94019CA181E433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C6469722E68
	?debug	C E94019CA181E433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C646F732E68
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C7374646172672E68
	?debug	C E94019CA181F433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C74696D652E68
	?debug	C E9436E691B096A756D7074626C2E68
	?debug	C E9F465691B08676C6D6465662E68
	?debug	C E9CB70691B08646566696E652E68
	?debug	C E97074641B06677473742E68
	?debug	C E9CA76641B087573657273742E68
CODE	segment byte public 'CODE'
CODE	ends
CODE	group	CODE
	assume	cs:CODE,ds:CODE
CODE	segment byte public 'CODE'
d@	label	byte
d@w	label	word
b@	label	byte
b@w	label	word
   ;	
   ;	int g_sprintf(char far *str, char far *format, ...)
   ;	
	assume	cs:CODE
_g_sprintf	proc	near
	push	bp
	mov	bp,sp
	sub	sp,12
   ;	
   ;	{
   ;	  va_list ap;
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-6],ds
   ;	
   ;	  int result;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-10],ax
	mov	word ptr [bp-12],dx
   ;	
   ;	
   ;	  va_start(ap,format);
   ;	
	lea	ax,word ptr [bp+12]
	mov	word ptr [bp-2],ss
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  result=(jump->vsprintf)(str,format,ap);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-12]
	call	dword ptr es:[bx]
	add	sp,12
	mov	word ptr [bp-8],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-6]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  va_end(format);
   ;	
   ;	
   ;	
   ;	  return (result);
   ;	
	mov	ax,word ptr [bp-8]
	jmp	short @1@58
@1@58:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_g_sprintf	endp
   ;	
   ;	int get_server(void)
   ;	
	assume	cs:CODE
_get_server	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
   ;	
   ;	{
   ;	
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int result;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  result = (jump->get_server)();
   ;	
	mov	ds,word ptr CODE:_old_DS
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+112]
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (result);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @2@58
@2@58:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_get_server	endp
   ;	
   ;	int st_copy(void far *to,size_t size,int which_structure,int data)
   ;	
	assume	cs:CODE
_st_copy	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int result;
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  result = (jump->st_copy)(to,size,which_structure,data);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+100]
	add	sp,10
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (result);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @3@58
@3@58:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_st_copy	endp
   ;	
   ;	void print_str_cr(char far *string)
   ;	
	assume	cs:CODE
_print_str_cr	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->print_str_cr)(string);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+4]
	pop	cx
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_print_str_cr	endp
   ;	
   ;	int get_string_cntrl(char *string, int limit, char echo, char back_to_end,
   ;	
	assume	cs:CODE
_get_string_cntrl	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
   ;	
   ;	    char escape, char noblankline, char cr_on_blankline,
   ;	    char upcase, char onlynum)
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int result;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  result=(jump->get_string_cntrl)(string,limit,echo,back_to_end,escape,
   ;	
   ;	
   ;	        noblankline,cr_on_blankline,upcase,onlynum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	mov	al,byte ptr [bp+22]
	push	ax
	mov	al,byte ptr [bp+20]
	push	ax
	mov	al,byte ptr [bp+18]
	push	ax
	mov	al,byte ptr [bp+16]
	push	ax
	mov	al,byte ptr [bp+14]
	push	ax
	mov	al,byte ptr [bp+12]
	push	ax
	mov	al,byte ptr [bp+10]
	push	ax
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+8]
	add	sp,20
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (result);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @5@58
@5@58:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_get_string_cntrl	endp
   ;	
   ;	void lock_dos(int x)
   ;	
	assume	cs:CODE
_lock_dos	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->lock_dos)(x);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+12]
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_lock_dos	endp
   ;	
   ;	void unlock_dos(void)
   ;	
	assume	cs:CODE
_unlock_dos	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->unlock_dos)();
   ;	
	mov	ds,word ptr CODE:_old_DS
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+16]
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_unlock_dos	endp
   ;	
   ;	time_t g_time(time_t far *data)
   ;	
	assume	cs:CODE
_g_time	proc	near
	push	bp
	mov	bp,sp
	sub	sp,10
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  time_t temp;
   ;	  int islocked;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-8],ax
	mov	word ptr [bp-10],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @8@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+12]
	pop	cx
@8@86:
   ;	
   ;	  temp=(jump->g_time)(data);
   ;	
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+20]
	pop	cx
	pop	cx
	mov	word ptr [bp-4],dx
	mov	word ptr [bp-6],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @8@142
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+16]
@8@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	dx,word ptr [bp-4]
	mov	ax,word ptr [bp-6]
	jmp	short @8@170
@8@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_time	endp
   ;	
   ;	int islocked(int sem)
   ;	
	assume	cs:CODE
_islocked	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  temp = (jump->islocked)(sem);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @9@58
@9@58:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_islocked	endp
   ;	
   ;	FILE far *g_fopen(const char far *filename, const char far *mode,
   ;	
	assume	cs:CODE
_g_fopen	proc	near
	push	bp
	mov	bp,sp
	sub	sp,10
	push	si
   ;	
   ;	     const char far *description)
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  FILE far *temp;
   ;	  int islocked;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-8],ax
	mov	word ptr [bp-10],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @10@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+12]
	pop	cx
@10@86:
   ;	
   ;	  temp=(jump->g_fopen)(filename,mode,description);
   ;	
	push	word ptr [bp+14]
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+28]
	add	sp,12
	mov	word ptr [bp-4],dx
	mov	word ptr [bp-6],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @10@142
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+16]
@10@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	dx,word ptr [bp-4]
	mov	ax,word ptr [bp-6]
	jmp	short @10@170
@10@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_fopen	endp
   ;	
   ;	int g_fclose(FILE far *file_ptr)
   ;	
	assume	cs:CODE
_g_fclose	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  int islocked;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked(DOS_SEM));
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @11@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+12]
	pop	cx
@11@86:
   ;	
   ;	  temp=(jump->g_fclose)(file_ptr);
   ;	
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+32]
	pop	cx
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @11@142
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+16]
@11@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @11@170
@11@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_fclose	endp
   ;	
   ;	int g_flush(FILE *file_ptr)
   ;	
	assume	cs:CODE
_g_flush	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  int islocked;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked(DOS_SEM));
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @12@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+12]
	pop	cx
@12@86:
   ;	
   ;	  temp=(jump->g_flush)(file_ptr);
   ;	
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+36]
	pop	cx
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @12@142
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+16]
@12@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @12@170
@12@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_flush	endp
   ;	
   ;	int g_free(void *memory_pointer)
   ;	
	assume	cs:CODE
_g_free	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  int islocked;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @13@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+12]
	pop	cx
@13@86:
   ;	
   ;	  temp=(jump->g_free)(memory_pointer);
   ;	
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+40]
	pop	cx
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @13@142
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+16]
@13@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @13@170
@13@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_free	endp
   ;	
   ;	void *g_malloc_main_only(unsigned long int memory, const char far *description)
   ;	
	assume	cs:CODE
_g_malloc_main_only	proc	near
	push	bp
	mov	bp,sp
	sub	sp,10
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  void far *temp;
   ;	  int islocked;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-8],ax
	mov	word ptr [bp-10],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @14@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+12]
	pop	cx
@14@86:
   ;	
   ;	  temp=(jump->g_malloc_main_only)(memory,description);
   ;	
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+44]
	add	sp,8
	mov	word ptr [bp-4],dx
	mov	word ptr [bp-6],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @14@142
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+16]
@14@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	dx,word ptr [bp-4]
	mov	ax,word ptr [bp-6]
	jmp	short @14@170
@14@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_malloc_main_only	endp
   ;	
   ;	void *g_malloc(unsigned long int memory, const char far *description)
   ;	
	assume	cs:CODE
_g_malloc	proc	near
	push	bp
	mov	bp,sp
	sub	sp,10
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  void far *temp;
   ;	  int islocked;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-8],ax
	mov	word ptr [bp-10],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @15@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+12]
	pop	cx
@15@86:
   ;	
   ;	  temp=(jump->g_malloc)(memory,description);
   ;	
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+48]
	add	sp,8
	mov	word ptr [bp-4],dx
	mov	word ptr [bp-6],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @15@142
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+16]
@15@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	dx,word ptr [bp-4]
	mov	ax,word ptr [bp-6]
	jmp	short @15@170
@15@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_malloc	endp
   ;	
   ;	int g_fread(void far *buf, size_t size, size_t count, FILE far *stream)
   ;	
	assume	cs:CODE
_g_fread	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  int islocked;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @16@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+12]
	pop	cx
@16@86:
   ;	
   ;	  temp=(jump->g_fread)(buf,size,count,stream);
   ;	
	push	word ptr [bp+14]
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+52]
	add	sp,12
	mov	word ptr [bp-4],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @16@142
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+16]
@16@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @16@170
@16@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_fread	endp
   ;	
   ;	int g_fwrite(void far *buf, size_t size, size_t count, FILE far *stream)
   ;	
	assume	cs:CODE
_g_fwrite	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  int islocked;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @17@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+12]
	pop	cx
@17@86:
   ;	
   ;	  temp=(jump->g_fwrite)(buf,size,count,stream);
   ;	
	push	word ptr [bp+14]
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+56]
	add	sp,12
	mov	word ptr [bp-4],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @17@142
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+16]
@17@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @17@170
@17@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_fwrite	endp
   ;	
   ;	int g_fseek(FILE far *stream, long int offset, int origin)
   ;	
	assume	cs:CODE
_g_fseek	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  int islocked;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @18@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+12]
	pop	cx
@18@86:
   ;	
   ;	  temp=(jump->g_fseek)(stream,offset,origin);
   ;	
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+60]
	add	sp,10
	mov	word ptr [bp-4],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @18@142
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+16]
@18@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @18@170
@18@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_fseek	endp
   ;	
   ;	long int g_ftell(FILE far *stream)
   ;	
	assume	cs:CODE
_g_ftell	proc	near
	push	bp
	mov	bp,sp
	sub	sp,10
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  long int temp;
   ;	  int islocked;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-8],ax
	mov	word ptr [bp-10],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @19@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+12]
	pop	cx
@19@86:
   ;	
   ;	  temp=(jump->g_ftell)(stream);
   ;	
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+64]
	pop	cx
	pop	cx
	mov	word ptr [bp-4],dx
	mov	word ptr [bp-6],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @19@142
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+16]
@19@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	dx,word ptr [bp-4]
	mov	ax,word ptr [bp-6]
	jmp	short @19@170
@19@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_ftell	endp
   ;	
   ;	void initabuffer(int bufsize)
   ;	
	assume	cs:CODE
_initabuffer	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->initabuffer)(bufsize);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+68]
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_initabuffer	endp
   ;	
   ;	int aget_abuffer(int far *sentby, int far *channel,
   ;	
	assume	cs:CODE
_aget_abuffer	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
   ;	
   ;	                 char far *string, int far *parm1,
   ;	                 int far *parm2, int far *parm3, int far *parm4)
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  temp=(jump->aget_abuffer)(sentby,channel,string,parm1,parm2,parm3,parm4);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+30]
	push	word ptr [bp+28]
	push	word ptr [bp+26]
	push	word ptr [bp+24]
	push	word ptr [bp+22]
	push	word ptr [bp+20]
	push	word ptr [bp+18]
	push	word ptr [bp+16]
	push	word ptr [bp+14]
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+72]
	add	sp,28
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @21@58
@21@58:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_aget_abuffer	endp
   ;	
   ;	void aput_into_buffer(int id, char far *string, int channel,
   ;	
	assume	cs:CODE
_aput_into_buffer	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	                 int parm1, int parm2, int parm3,int parm4)
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->aput_into_buffer)(id,string,channel,parm1,parm2,parm3,parm4);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+18]
	push	word ptr [bp+16]
	push	word ptr [bp+14]
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+76]
	add	sp,16
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_aput_into_buffer	endp
   ;	
   ;	void dealloc_abuf(int portnum)
   ;	
	assume	cs:CODE
_dealloc_abuf	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->dealloc_abuf)(portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+84]
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_dealloc_abuf	endp
   ;	
   ;	void print_file_to_cntrl(const char far *filename,int portnum,int ansi,
   ;	
	assume	cs:CODE
_print_file_to_cntrl	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	                   int pause,int abort,int paging)
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->print_file_to_cntrl)(filename,portnum,ansi,pause,abort,paging);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+16]
	push	word ptr [bp+14]
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+104]
	add	sp,14
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_print_file_to_cntrl	endp
   ;	
   ;	int wait_ch(void)
   ;	
	assume	cs:CODE
_wait_ch	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  temp=(jump->wait_ch)();
   ;	
	mov	ds,word ptr CODE:_old_DS
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+108]
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return(temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @25@58
@25@58:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_wait_ch	endp
   ;	
   ;	int line_editor(char far *filename, int length)
   ;	
	assume	cs:CODE
_line_editor	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  temp=(jump->line_editor)(filename,length);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+116]
	add	sp,6
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return(temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @26@58
@26@58:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_line_editor	endp
   ;	
   ;	void g_delay(int ticks)
   ;	
	assume	cs:CODE
_g_delay	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->delay)(ticks);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+120]
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_g_delay	endp
   ;	
   ;	int g_rename(char far *file1, char *far file2)
   ;	
	assume	cs:CODE
_g_rename	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  int temp;
   ;	  int islocked;
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @28@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+12]
	pop	cx
@28@86:
   ;	
   ;	  temp=(jump->g_rename)(file1,file2);
   ;	
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+124]
	add	sp,8
	mov	word ptr [bp-8],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @28@142
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+16]
@28@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-8]
	jmp	short @28@170
@28@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_rename	endp
   ;	
   ;	int g_findfirst(char far *wildcard, struct ffblk far *ffblk, int flags)
   ;	
	assume	cs:CODE
_g_findfirst	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  int islocked;
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @29@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+12]
	pop	cx
@29@86:
   ;	
   ;	  temp=(jump->findfirst)(wildcard,ffblk,flags);
   ;	
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+128]
	add	sp,10
	mov	word ptr [bp-4],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @29@142
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+16]
@29@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return(temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @29@170
@29@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_findfirst	endp
   ;	
   ;	int g_findnext(struct ffblk far *ffblk)
   ;	
	assume	cs:CODE
_g_findnext	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  int islocked;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @30@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+12]
	pop	cx
@30@86:
   ;	
   ;	  temp=(jump->findnext)(ffblk);
   ;	
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+132]
	pop	cx
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @30@142
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+16]
@30@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return(temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @30@170
@30@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_findnext	endp
   ;	
   ;	void log_error(char far *file)
   ;	
	assume	cs:CODE
_log_error	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->log_error)(file);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+136]
	pop	cx
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_log_error	endp
   ;	
   ;	int g_remove(char far *file)
   ;	
	assume	cs:CODE
_g_remove	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int islocked;
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @32@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+12]
	pop	cx
@32@86:
   ;	
   ;	  temp=(jump->g_remove)(file);
   ;	
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+140]
	pop	cx
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @32@142
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+16]
@32@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @32@170
@32@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_remove	endp
   ;	
   ;	void send_files(char far **filepointers, int number, int mode)
   ;	
	assume	cs:CODE
_send_files	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->send_files)(filepointers,number,mode);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+144]
	add	sp,8
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_send_files	endp
   ;	
   ;	void recv_files(char far **filepointers, int far *number, int mode)
   ;	
	assume	cs:CODE
_recv_files	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->recv_files)(filepointers,number,mode);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+148]
	add	sp,10
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_recv_files	endp
   ;	
   ;	int g_mkdir(char far *filename)
   ;	
	assume	cs:CODE
_g_mkdir	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int islocked;
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @35@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+12]
	pop	cx
@35@86:
   ;	
   ;	  temp=(jump->g_mkdir)(filename);
   ;	
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+152]
	pop	cx
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @35@142
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+16]
@35@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @35@170
@35@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_mkdir	endp
   ;	
   ;	int g_rmdir(char far *filename)
   ;	
	assume	cs:CODE
_g_rmdir	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int islocked;
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @36@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+12]
	pop	cx
@36@86:
   ;	
   ;	  temp=(jump->g_rmdir)(filename);
   ;	
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+156]
	pop	cx
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @36@142
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+16]
@36@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @36@170
@36@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_rmdir	endp
   ;	
   ;	void print_chr(char temp)
   ;	
	assume	cs:CODE
_print_chr	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->print_chr)(temp);
   ;	
	mov	ds,word ptr CODE:_old_DS
	mov	al,byte ptr [bp+4]
	push	ax
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+160]
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_print_chr	endp
   ;	
   ;	void print_string(char *string)
   ;	
	assume	cs:CODE
_print_string	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->print_string)(string);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+164]
	pop	cx
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_print_string	endp
   ;	
   ;	void print_cr(void)
   ;	
	assume	cs:CODE
_print_cr	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->print_cr)();
   ;	
	mov	ds,word ptr CODE:_old_DS
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+168]
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_print_cr	endp
   ;	
   ;	void position(int x,int y)
   ;	
	assume	cs:CODE
_position	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->position)(x,y);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+172]
	pop	cx
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_position	endp
   ;	
   ;	void foreground(int color, int portnum)
   ;	
	assume	cs:CODE
_foreground	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->foreground)(color,portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+176]
	pop	cx
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_foreground	endp
   ;	
   ;	void background(int color, int portnum)
   ;	
	assume	cs:CODE
_background	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->background)(color,portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+180]
	pop	cx
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_background	endp
   ;	
   ;	void special_code(int state, int portnum)
   ;	
	assume	cs:CODE
_special_code	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->special_code)(state,portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+184]
	pop	cx
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_special_code	endp
   ;	
   ;	void reset_attributes(int portnum)
   ;	
	assume	cs:CODE
_reset_attributes	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->reset_attributes)(portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+188]
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_reset_attributes	endp
   ;	
   ;	void blink_video(int portnum)
   ;	
	assume	cs:CODE
_blink_video	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->blink_video)(portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+192]
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_blink_video	endp
   ;	
   ;	void bold_video(int portnum)
   ;	
	assume	cs:CODE
_bold_video	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->bold_video)(portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+196]
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_bold_video	endp
   ;	
   ;	void wrap_line(char far *string)
   ;	
	assume	cs:CODE
_wrap_line	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->wrap_line)(string);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+200]
	pop	cx
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_wrap_line	endp
   ;	
   ;	int chars_in_buffer(int portnum)
   ;	
	assume	cs:CODE
_chars_in_buffer	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  temp=(*(jump->a_chars_in_buffer)[portnum])(portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	si
	les	bx,dword ptr [bp-8]
	les	bx,dword ptr es:[bx+224]
	mov	ax,si
	mov	cl,2
	shl	ax,cl
	add	bx,ax
	call	dword ptr es:[bx]
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @48@58
@48@58:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_chars_in_buffer	endp
   ;	
   ;	int dcd_detect(int portnum)
   ;	
	assume	cs:CODE
_dcd_detect	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  temp=(*(jump->a_dcd_detect)[portnum])(portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	si
	les	bx,dword ptr [bp-8]
	les	bx,dword ptr es:[bx+228]
	mov	ax,si
	mov	cl,2
	shl	ax,cl
	add	bx,ax
	call	dword ptr es:[bx]
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @49@58
@49@58:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_dcd_detect	endp
   ;	
   ;	void put_char_in_buffer(char temp, int portnum)
   ;	
	assume	cs:CODE
_put_char_in_buffer	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	mov	si,word ptr [bp+6]
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (*(jump->a_put_char_in_buffer)[portnum])(temp,portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	si
	mov	al,byte ptr [bp+4]
	push	ax
	les	bx,dword ptr [bp-6]
	les	bx,dword ptr es:[bx+232]
	mov	ax,si
	mov	cl,2
	shl	ax,cl
	add	bx,ax
	call	dword ptr es:[bx]
	pop	cx
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_put_char_in_buffer	endp
   ;	
   ;	void get_char(int portnum, int far *charput, int far *isthere)
   ;	
	assume	cs:CODE
_get_char	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (*(jump->a_get_char)[portnum])(portnum,charput,isthere);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	si
	les	bx,dword ptr [bp-6]
	les	bx,dword ptr es:[bx+236]
	mov	ax,si
	mov	cl,2
	shl	ax,cl
	add	bx,ax
	call	dword ptr es:[bx]
	add	sp,10
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_get_char	endp
   ;	
   ;	void send_char(int portnum, char charput)
   ;	
	assume	cs:CODE
_send_char	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (*(jump->a_send_char)[portnum])(portnum,charput);
   ;	
	mov	ds,word ptr CODE:_old_DS
	mov	al,byte ptr [bp+6]
	push	ax
	push	si
	les	bx,dword ptr [bp-6]
	les	bx,dword ptr es:[bx+240]
	mov	ax,si
	mov	cl,2
	shl	ax,cl
	add	bx,ax
	call	dword ptr es:[bx]
	pop	cx
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_send_char	endp
   ;	
   ;	void empty_buffer(int portnum)
   ;	
	assume	cs:CODE
_empty_buffer	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (*(jump->a_empty_inbuffer)[portnum])(portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	si
	les	bx,dword ptr [bp-6]
	les	bx,dword ptr es:[bx+244]
	mov	ax,si
	mov	cl,2
	shl	ax,cl
	add	bx,ax
	call	dword ptr es:[bx]
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_empty_buffer	endp
   ;	
   ;	int char_in_buf(int portnum)
   ;	
	assume	cs:CODE
_char_in_buf	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  temp=(*(jump->a_char_in_buf)[portnum])(portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	si
	les	bx,dword ptr [bp-8]
	les	bx,dword ptr es:[bx+248]
	mov	ax,si
	mov	cl,2
	shl	ax,cl
	add	bx,ax
	call	dword ptr es:[bx]
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @54@58
@54@58:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_char_in_buf	endp
   ;	
   ;	int get_first_char(int portnum)
   ;	
	assume	cs:CODE
_get_first_char	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  temp=(*(jump->a_get_first_char)[portnum])(portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	si
	les	bx,dword ptr [bp-8]
	les	bx,dword ptr es:[bx+252]
	mov	ax,si
	mov	cl,2
	shl	ax,cl
	add	bx,ax
	call	dword ptr es:[bx]
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @55@58
@55@58:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_get_first_char	endp
   ;	
   ;	int get_nchar(int portnum)
   ;	
	assume	cs:CODE
_get_nchar	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  temp=(*(jump->a_get_nchar)[portnum])(portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	si
	les	bx,dword ptr [bp-8]
	les	bx,dword ptr es:[bx+256]
	mov	ax,si
	mov	cl,2
	shl	ax,cl
	add	bx,ax
	call	dword ptr es:[bx]
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @56@58
@56@58:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_get_nchar	endp
   ;	
   ;	void wait_for_xmit(int portnum, int ticks)
   ;	
	assume	cs:CODE
_wait_for_xmit	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (*(jump->a_wait_for_xmit)[portnum])(portnum,ticks);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+6]
	push	si
	les	bx,dword ptr [bp-6]
	les	bx,dword ptr es:[bx+260]
	mov	ax,si
	mov	cl,2
	shl	ax,cl
	add	bx,ax
	call	dword ptr es:[bx]
	pop	cx
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_wait_for_xmit	endp
   ;	
   ;	void empty_outbuffer(int portnum)
   ;	
	assume	cs:CODE
_empty_outbuffer	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (*(jump->a_empty_outbuffer)[portnum])(portnum);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	si
	les	bx,dword ptr [bp-6]
	les	bx,dword ptr es:[bx+264]
	mov	ax,si
	mov	cl,2
	shl	ax,cl
	add	bx,ax
	call	dword ptr es:[bx]
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_empty_outbuffer	endp
   ;	
   ;	void change_dtr_state(int portnum, int state)
   ;	
	assume	cs:CODE
_change_dtr_state	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (*(jump->a_change_dtr_state)[portnum])(portnum,state);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+6]
	push	si
	les	bx,dword ptr [bp-6]
	les	bx,dword ptr es:[bx+268]
	mov	ax,si
	mov	cl,2
	shl	ax,cl
	add	bx,ax
	call	dword ptr es:[bx]
	pop	cx
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_change_dtr_state	endp
   ;	
   ;	void check_for_privates(void)
   ;	
	assume	cs:CODE
_check_for_privates	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->check_for_privates)();
   ;	
	mov	ds,word ptr CODE:_old_DS
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+208]
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_check_for_privates	endp
   ;	
   ;	char far *g_getcwd(char far *buffer, int buflen)
   ;	
	assume	cs:CODE
_g_getcwd	proc	near
	push	bp
	mov	bp,sp
	sub	sp,10
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  char far *temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-8],ax
	mov	word ptr [bp-10],dx
   ;	
   ;	  int islocked;
   ;	
   ;	  if (!buffer) return (NULL);
   ;	
	mov	ax,word ptr [bp+4]
	or	ax,word ptr [bp+6]
	jne	short @61@114
	xor	dx,dx
	xor	ax,ax
@61@86:
	jmp	short @61@254
@61@114:
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @61@170
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+12]
	pop	cx
@61@170:
   ;	
   ;	  temp=(jump->getcwd)(buffer,buflen);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+212]
	add	sp,6
	mov	word ptr [bp-4],dx
	mov	word ptr [bp-6],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @61@226
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+16]
@61@226:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	dx,word ptr [bp-4]
	mov	ax,word ptr [bp-6]
	jmp	short @61@86
@61@254:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_getcwd	endp
   ;	
   ;	int unregister_bot_myself(void)
   ;	
	assume	cs:CODE
_unregister_bot_myself	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  temp=(jump->unregister_bot_myself)();
   ;	
	mov	ds,word ptr CODE:_old_DS
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+88]
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @62@58
@62@58:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_unregister_bot_myself	endp
   ;	
   ;	int register_bot(char far *orig_name)
   ;	
	assume	cs:CODE
_register_bot	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  temp=(jump->register_bot)(orig_name);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+92]
	pop	cx
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @63@58
@63@58:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_register_bot	endp
   ;	
   ;	int change_my_info_line(char far *newline)
   ;	
	assume	cs:CODE
_change_my_info_line	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  temp=(jump->change_my_info_line)(newline);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+96]
	pop	cx
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @64@58
@64@58:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_change_my_info_line	endp
   ;	
   ;	void broadcast_message(char far *newline)
   ;	
	assume	cs:CODE
_broadcast_message	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->broadcast_message)(newline);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+216]
	pop	cx
	pop	cx
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_broadcast_message	endp
   ;	
   ;	void aput_append_into_buffer(int id, int channel, int parm1,
   ;	
	assume	cs:CODE
_aput_append_into_buffer	proc	near
	push	bp
	mov	bp,sp
	sub	sp,10
   ;	
   ;	                 int parm2, int parm3, int parm4, int no_str, ...)
   ;	{
   ;	  va_list ap;
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-6],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-8],ax
	mov	word ptr [bp-10],dx
   ;	
   ;	
   ;	  va_start(ap,no_str);
   ;	
	lea	ax,word ptr [bp+18]
	mov	word ptr [bp-2],ss
	mov	word ptr [bp-4],ax
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->aput_vargs_into_buffer)
   ;	
   ;	
   ;	       (id,channel,parm1,parm2,parm3,parm4,no_str,ap);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr [bp+16]
	push	word ptr [bp+14]
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-10]
	call	dword ptr es:[bx+80]
	add	sp,18
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-6]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  va_end(format);
   ;	
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_aput_append_into_buffer	endp
   ;	
   ;	int g_getdisk(void)
   ;	
	assume	cs:CODE
_g_getdisk	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  int islocked;
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @67@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+12]
	pop	cx
@67@86:
   ;	
   ;	  temp=(jump->getdisk)();
   ;	
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+276]
	mov	word ptr [bp-4],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @67@142
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+16]
@67@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @67@170
@67@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_getdisk	endp
   ;	
   ;	int g_getfree(int drive, unsigned long int far *freebytes,
   ;	
	assume	cs:CODE
_g_getfree	proc	near
	push	bp
	mov	bp,sp
	sub	sp,14
	push	si
   ;	
   ;	            unsigned long int far *totalbytes)
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  struct dfree dbuf;
   ;	  int islocked;
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @68@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+12]
	pop	cx
@68@86:
   ;	
   ;	  (jump->getdfree)(drive,&dbuf);
   ;	
	push	ss
	lea	ax,word ptr [bp-14]
	push	ax
	mov	al,byte ptr [bp+4]
	push	ax
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+280]
	add	sp,6
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @68@142
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+16]
@68@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  if (dbuf.df_sclus == 0xFFFF) return (-1);
   ;	
	cmp	word ptr [bp-8],-1
	jne	short @68@226
	mov	ax,-1
@68@198:
	jmp	short @68@254
@68@226:
   ;	
   ;	  *freebytes = ((unsigned long int)dbuf.df_avail) *
   ;	
   ;	
   ;	               ((unsigned long int)dbuf.df_bsec) *
   ;	               ((unsigned long int)dbuf.df_sclus);
   ;	
	mov	bx,word ptr [bp-14]
	xor	cx,cx
	mov	ax,word ptr [bp-10]
	xor	dx,dx
	call	far ptr F_LXMUL@
	push	ax
	mov	ax,word ptr [bp-8]
	push	dx
	xor	dx,dx
	pop	cx
	pop	bx
	call	far ptr F_LXMUL@
	les	bx,dword ptr [bp+6]
	mov	word ptr es:[bx+2],dx
	mov	word ptr es:[bx],ax
   ;	
   ;	  *totalbytes = ((unsigned long int)dbuf.df_total) *
   ;	
   ;	
   ;	               ((unsigned long int)dbuf.df_bsec) *
   ;	               ((unsigned long int)dbuf.df_sclus);
   ;	
	mov	bx,word ptr [bp-12]
	xor	cx,cx
	mov	ax,word ptr [bp-10]
	xor	dx,dx
	call	far ptr F_LXMUL@
	push	ax
	mov	ax,word ptr [bp-8]
	push	dx
	xor	dx,dx
	pop	cx
	pop	bx
	call	far ptr F_LXMUL@
	les	bx,dword ptr [bp+10]
	mov	word ptr es:[bx+2],dx
	mov	word ptr es:[bx],ax
   ;	
   ;	  return (0);
   ;	
	xor	ax,ax
	jmp	short @68@198
@68@254:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_getfree	endp
   ;	
   ;	int g_getcurdir(int drive, char far *directory)
   ;	
	assume	cs:CODE
_g_getcurdir	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  int temp;
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-8],dx
   ;	
   ;	  int islocked;
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  islocked = (jump->islocked)(DOS_SEM);
   ;	
	mov	ds,word ptr CODE:_old_DS
	xor	ax,ax
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+24]
	pop	cx
	mov	si,ax
   ;	
   ;	  if (!islocked) (jump->lock_dos)(1001);
   ;	
	or	si,si
	jne	short @69@86
	mov	ax,1001
	push	ax
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+12]
	pop	cx
@69@86:
   ;	
   ;	  temp=(jump->getcurdir)(drive,directory);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+284]
	add	sp,6
	mov	word ptr [bp-4],ax
   ;	
   ;	  if (!islocked) (jump->unlock_dos)();
   ;	
	or	si,si
	jne	short @69@142
	les	bx,dword ptr [bp-8]
	call	dword ptr es:[bx+16]
@69@142:
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (temp);
   ;	
	mov	ax,word ptr [bp-4]
	jmp	short @69@170
@69@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_g_getcurdir	endp
   ;	
   ;	void get_registered_bot_name_for_myself(char far *string,int string_len)
   ;	
	assume	cs:CODE
_get_registered_bot_name_for_myse	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  (jump->get_registered_bot_name_for_myself)(string,string_len);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+288]
	add	sp,6
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_get_registered_bot_name_for_myse	endp
   ;	
   ;	int read_my_channel(void)
   ;	
	assume	cs:CODE
_read_my_channel	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  int result;
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  result = (jump->read_my_channel)();
   ;	
	mov	ds,word ptr CODE:_old_DS
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+292]
	mov	word ptr [bp-8],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (result);
   ;	
	mov	ax,word ptr [bp-8]
	jmp	short @71@58
@71@58:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_read_my_channel	endp
   ;	
   ;	int change_my_channel(int channel)
   ;	
	assume	cs:CODE
_change_my_channel	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
   ;	
   ;	{
   ;	  int b = _DS;
   ;	
	mov	word ptr [bp-2],ds
   ;	
   ;	  struct jumptable far *jump = jmptl;
   ;	
	mov	ax,word ptr CODE:_jmptl+2
	mov	dx,word ptr CODE:_jmptl
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],dx
   ;	
   ;	  int result;
   ;	
   ;	  _ES = old_ES;
   ;	
   ;	
   ;	  _DS = old_DS;
   ;	
	mov	es,word ptr CODE:_old_ES
   ;	
   ;	  result = (jump->change_my_channel)(channel);
   ;	
	mov	ds,word ptr CODE:_old_DS
	push	word ptr [bp+4]
	les	bx,dword ptr [bp-6]
	call	dword ptr es:[bx+296]
	pop	cx
	mov	word ptr [bp-8],ax
   ;	
   ;	  _ES = _DS = b;
   ;	
	mov	ax,word ptr [bp-2]
	mov	ds,ax
	mov	es,ax
   ;	
   ;	  return (result);
   ;	
	mov	ax,word ptr [bp-8]
	jmp	short @72@58
@72@58:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_change_my_channel	endp
	?debug	C E9
	?debug	C FA00000000
s@	label	byte
CODE	ends
	extrn	F_LXMUL@:far
	public	_broadcast_message
	public	_check_for_privates
	public	_change_my_channel
	public	_read_my_channel
	public	_get_registered_bot_name_for_myse
	public	_g_getcurdir
	public	_g_getfree
	public	_g_getdisk
	public	_get_server
	public	_st_copy
	public	_change_my_info_line
	public	_register_bot
	public	_unregister_bot_myself
	public	_g_getcwd
	public	_change_dtr_state
	public	_empty_outbuffer
	public	_wait_for_xmit
	public	_get_nchar
	public	_get_first_char
	public	_char_in_buf
	public	_empty_buffer
	public	_send_char
	public	_get_char
	public	_put_char_in_buffer
	public	_dcd_detect
	public	_chars_in_buffer
	public	_wrap_line
	public	_bold_video
	public	_blink_video
	public	_reset_attributes
	public	_special_code
	public	_background
	public	_foreground
	public	_position
	public	_print_cr
	public	_print_string
	public	_print_chr
	public	_g_rmdir
	public	_g_mkdir
	public	_recv_files
	public	_send_files
	public	_g_remove
	public	_log_error
	public	_g_findnext
	public	_g_findfirst
	public	_g_rename
	public	_g_delay
	public	_line_editor
	public	_wait_ch
	public	_print_file_to_cntrl
	public	_dealloc_abuf
	public	_aput_append_into_buffer
	public	_aput_into_buffer
	public	_aget_abuffer
	public	_initabuffer
	public	_g_ftell
	public	_g_fseek
	public	_g_fwrite
	public	_g_fread
	public	_g_malloc
	public	_g_malloc_main_only
	public	_g_free
	public	_g_flush
	public	_g_fclose
	public	_g_fopen
	public	_islocked
	public	_g_time
	public	_unlock_dos
	public	_lock_dos
	public	_get_string_cntrl
	public	_print_str_cr
	public	_g_sprintf
	extrn	_old_ES:word
	extrn	_old_DS:word
	extrn	_jmptl:dword
_s@	equ	s@
	end
