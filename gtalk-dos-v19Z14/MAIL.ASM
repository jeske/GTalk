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
	?debug	S "mail.c"
	?debug	C E94D47DB1A066D61696C2E63
	?debug	C E95A7F8C1A09696E636C7564652E68
	?debug	C E94019CA181B433A5C424F524C414E44435C494E434C5544455C73+
	?debug	C 7464696F2E68
	?debug	C E94019CA181B433A5C424F524C414E44435C494E434C5544455C5F+
	?debug	C 646566732E68
	?debug	C E94019CA181C433A5C424F524C414E44435C494E434C5544455C5F+
	?debug	C 6E66696C652E68
	?debug	C E94019CA181B433A5C424F524C414E44435C494E434C5544455C5F+
	?debug	C 6E756C6C2E68
	?debug	C E94019CA1819433A5C424F524C414E44435C494E434C5544455C64+
	?debug	C 6F732E68
	?debug	C E94019CA181A433A5C424F524C414E44435C494E434C5544455C62+
	?debug	C 696F732E68
	?debug	C E94019CA181B433A5C424F524C414E44435C494E434C5544455C61+
	?debug	C 6C6C6F632E68
	?debug	C E94019CA181B433A5C424F524C414E44435C494E434C5544455C63+
	?debug	C 6F6E696F2E68
	?debug	C E94019CA181C433A5C424F524C414E44435C494E434C5544455C73+
	?debug	C 74646C69622E68
	?debug	C E94019CA181B433A5C424F524C414E44435C494E434C5544455C63+
	?debug	C 747970652E68
	?debug	C E94019CA181C433A5C424F524C414E44435C494E434C5544455C73+
	?debug	C 7472696E672E68
	?debug	C E94019CA181A433A5C424F524C414E44435C494E434C5544455C74+
	?debug	C 696D652E68
	?debug	C E94019CA1819433A5C424F524C414E44435C494E434C5544455C6D+
	?debug	C 656D2E68
	?debug	C E94019CA1819433A5C424F524C414E44435C494E434C5544455C64+
	?debug	C 69722E68
	?debug	C E9260B931A076774616C6B2E68
	?debug	C E9ECA8951A067461736B2E68
	?debug	C E9E60A931A06696E69742E68
	?debug	C E9F0A58A1A07616C6C6F632E68
	?debug	C E9AB8CC51A05636F6D2E68
	?debug	C E99DBB951A07766964656F2E68
	?debug	C E90EA6951A096368616E6E656C2E68
	?debug	C E915838C1A04696F2E68
	?debug	C E9207ABD1A0467742E68
	?debug	C E9056CD41A06757365722E68
	?debug	C E9438B9B1A09636F6D6D616E642E68
	?debug	C E9C3793B1A066C696E6B2E68
	?debug	C E9FB9D921A07676C696E6B2E68
	?debug	C E9C2793B1A09737472756374732E68
	?debug	C E944A7951A08737472696E672E68
	?debug	C E9127DB01A0770726976732E68
	?debug	C E90AB9931A09746F67676C65732E68
	?debug	C E9C4793B1A07696E7075742E68
	?debug	C E91B82891A08656469746F722E68
	?debug	C E91202DB1A066D61696C2E68
	?debug	C E97AA69A1A09726F7461746F722E68
	?debug	C E9C5793B1A086766696C65732E68
	?debug	C E994913C1A0A7363686564756C652E68
	?debug	C E9C2793B1A077379736F702E68
	?debug	C E9DC69D41A056262732E68
	?debug	C E98D8B931A0764696167732E68
	?debug	C E9C1793B1A0A70726F746F636F6C2E68
	?debug	C E93B9AC21A08646566696E652E68
	?debug	C E988B2931A0A66756E6374696F6E2E68
	?debug	C E9C2793B1A09737472756374732E68
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
   ;	void strcpy_n(char *outstr, char *instr, int length)
   ;	
	assume	cs:_TEXT
_strcpy_n	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+4]
	mov	di,word ptr [bp+6]
	mov	dx,word ptr [bp+8]
	jmp	short @1@86
@1@58:
   ;	
   ;	{
   ;	  while ((*instr) && (length>0))
   ;	  {
   ;	    *outstr++ = *instr++;
   ;	
	mov	al,byte ptr [di]
	mov	byte ptr [si],al
	inc	di
	inc	si
   ;	
   ;	    length--;
   ;	
	dec	dx
@1@86:
	cmp	byte ptr [di],0
	je	short @1@142
	or	dx,dx
	jg	short @1@58
@1@142:
   ;	
   ;	  }
   ;	  *outstr = 0;
   ;	
	mov	byte ptr [si],0
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_strcpy_n	endp
   ;	
   ;	int load_new_base(struct mail_pass_data *data)
   ;	
	assume	cs:_TEXT
_load_new_base	proc	near
	push	bp
	mov	bp,sp
	sub	sp,80
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  char filename[FILENAME_LEN];
   ;	
   ;	  sprintf(filename,"%s\\basedata",data->basename);
   ;	
	push	si
	mov	ax,offset DGROUP:s@
	push	ax
	lea	ax,word ptr [bp-80]
	push	ax
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	  if (!(data->fp=g_fopen(filename,"rb+","LOADBASE")))
   ;	
	mov	ax,offset DGROUP:s@+16
	push	ax
	mov	ax,offset DGROUP:s@+12
	push	ax
	lea	ax,word ptr [bp-80]
	push	ax
	call	near ptr _g_fopen
	add	sp,6
	mov	word ptr [si+336],ax
	or	ax,ax
	je	@@0
	jmp	@2@282
@@0:
   ;	
   ;	  {
   ;	    mkdir(data->basename);
   ;	
	push	si
	call	near ptr _mkdir
	pop	cx
   ;	
   ;	    if (!(data->fp=g_fopen(filename,"wb+","CREBASE"))) return (0);
   ;	
	mov	ax,offset DGROUP:s@+29
	push	ax
	mov	ax,offset DGROUP:s@+25
	push	ax
	lea	ax,word ptr [bp-80]
	push	ax
	call	near ptr _g_fopen
	add	sp,6
	mov	word ptr [si+336],ax
	or	ax,ax
	jne	short @2@142
	xor	ax,ax
@2@114:
	jmp	@2@310
@2@142:
   ;	
   ;	    if (data->mail != -1)
   ;	
	cmp	word ptr [si+338],-1
	je	short @2@198
   ;	
   ;	    {
   ;	      sprintf(filename,"Mailbox #%03d",data->mail);
   ;	
	push	word ptr [si+338]
	mov	ax,offset DGROUP:s@+37
	push	ax
	lea	ax,word ptr [bp-80]
	push	ax
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	      strcpy(data->entry.name,filename);
   ;	
	lea	ax,word ptr [bp-80]
	jmp	short @2@226
@2@198:
   ;	
   ;	    } else strcpy(data->entry.name,"blank");
   ;	
	mov	ax,offset DGROUP:s@+51
@2@226:
	push	ax
	mov	ax,si
	add	ax,91
	push	ax
	call	near ptr _strcpy
	pop	cx
	pop	cx
   ;	
   ;	    data->entry.head_entry_no = 0;
   ;	
	mov	word ptr [si+85],0
	mov	word ptr [si+83],0
   ;	
   ;	    data->entry.tail_entry_no = 0;
   ;	
	mov	word ptr [si+89],0
	mov	word ptr [si+87],0
   ;	
   ;	    data->entry.new_msg_no = 0;
   ;	
	mov	word ptr [si+114],0
	mov	word ptr [si+112],0
   ;	
   ;	    data->entry.priority = 255;
   ;	
	mov	byte ptr [si+82],255
   ;	
   ;	    fwrite(&data->entry,sizeof(struct first_entry),1,data->fp);
   ;	
	push	word ptr [si+336]
	mov	ax,1
	push	ax
	mov	ax,256
	push	ax
	mov	ax,si
	add	ax,80
	push	ax
	call	near ptr _fwrite
	add	sp,8
   ;	
   ;	    fseek(data->fp,0,SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	xor	ax,ax
	xor	dx,dx
	push	ax
	push	dx
	push	word ptr [si+336]
	call	near ptr _fseek
	add	sp,8
@2@282:
   ;	
   ;	  }
   ;	  fread(&data->entry,sizeof(struct first_entry),1,data->fp);
   ;	
	push	word ptr [si+336]
	mov	ax,1
	push	ax
	mov	ax,256
	push	ax
	mov	ax,si
	add	ax,80
	push	ax
	call	near ptr _fread
	add	sp,8
   ;	
   ;	  return (1);
   ;	
	mov	ax,1
	jmp	@2@114
@2@310:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_load_new_base	endp
   ;	
   ;	void close_new_base(struct mail_pass_data *data)
   ;	
	assume	cs:_TEXT
_close_new_base	proc	near
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  fseek(data->fp,0,SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	xor	ax,ax
	xor	dx,dx
	push	ax
	push	dx
	push	word ptr [si+336]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	  fwrite(&data->entry,sizeof(struct first_entry),1,data->fp);
   ;	
	push	word ptr [si+336]
	mov	ax,1
	push	ax
	mov	ax,256
	push	ax
	mov	ax,si
	add	ax,80
	push	ax
	call	near ptr _fwrite
	add	sp,8
   ;	
   ;	  g_fclose(data->fp);
   ;	
	push	word ptr [si+336]
	call	near ptr _g_fclose
	pop	cx
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
_close_new_base	endp
   ;	
   ;	int open_mail_base(int user_no, struct mail_pass_data *data, int create_mail)
   ;	
	assume	cs:_TEXT
_open_mail_base	proc	near
	push	bp
	mov	bp,sp
	sub	sp,44
	push	si
	push	di
	mov	di,word ptr [bp+4]
	mov	si,word ptr [bp+6]
   ;	
   ;	{                 /* Current user #=user_lines[tswitch].number */
   ;	  struct ffblk look_up;
   ;	
   ;	  sprintf(data->basename,"MAIL\\MAIL%03d",user_no);
   ;	
	push	di
	mov	ax,offset DGROUP:s@+57
	push	ax
	push	si
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	  if (!create_mail)
   ;	
	cmp	word ptr [bp+8],0
	jne	short @4@142
   ;	
   ;	    if (!findfirst(data->basename,&look_up,0)) return (0);
   ;	
	xor	ax,ax
	push	ax
	lea	ax,word ptr [bp-44]
	push	ax
	push	si
	call	near ptr _findfirst
	add	sp,6
	or	ax,ax
	jne	short @4@142
	xor	ax,ax
@4@114:
	jmp	short @4@170
@4@142:
   ;	
   ;	  data->mail = user_no;
   ;	
	mov	word ptr [si+338],di
   ;	
   ;	  return (load_new_base(data));
   ;	
	push	si
	call	near ptr _load_new_base
	pop	cx
	jmp	short @4@114
@4@170:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_open_mail_base	endp
   ;	
   ;	int open_bbs_base(int bbs_no, struct mail_pass_data *data, int create_bbs)
   ;	
	assume	cs:_TEXT
_open_bbs_base	proc	near
	push	bp
	mov	bp,sp
	sub	sp,44
	push	si
	mov	si,word ptr [bp+6]
   ;	
   ;	{                 /* Current user #=user_lines[tswitch].number */
   ;	  struct ffblk look_up;
   ;	
   ;	  sprintf(data->basename,"BBS\\BBS%02d",bbs_no);
   ;	
	push	word ptr [bp+4]
	mov	ax,offset DGROUP:s@+71
	push	ax
	push	si
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	  if (!create_bbs)
   ;	
	cmp	word ptr [bp+8],0
	jne	short @5@142
   ;	
   ;	    if (!findfirst(data->basename,&look_up,0)) return (0);
   ;	
	xor	ax,ax
	push	ax
	lea	ax,word ptr [bp-44]
	push	ax
	push	si
	call	near ptr _findfirst
	add	sp,6
	or	ax,ax
	jne	short @5@142
	xor	ax,ax
@5@114:
	jmp	short @5@170
@5@142:
   ;	
   ;	  data->mail = -1;
   ;	
	mov	word ptr [si+338],-1
   ;	
   ;	  return (load_new_base(data));
   ;	
	push	si
	call	near ptr _load_new_base
	pop	cx
	jmp	short @5@114
@5@170:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_open_bbs_base	endp
   ;	
   ;	int create_temp_file(struct file_entry *file, unsigned int editor, int gen)
   ;	
	assume	cs:_TEXT
_create_temp_file	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+4]
	mov	di,word ptr [bp+6]
   ;	
   ;	{
   ;	  sprintf(file->filename,"TEMP%02d%01d",tswitch,gen);
   ;	
	push	word ptr [bp+8]
	push	word ptr DGROUP:_tswitch
	mov	ax,offset DGROUP:s@+83
	push	ax
	push	si
	call	near ptr _sprintf
	add	sp,8
   ;	
   ;	  if (editor)
   ;	
	or	di,di
	je	short @6@254
   ;	
   ;	  {
   ;	    lock_dos();
   ;	
	call	near ptr _lock_dos
   ;	
   ;	    remove(file->filename);
   ;	
	push	si
	call	near ptr _remove
	pop	cx
   ;	
   ;	    unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	    if (!line_editor(file->filename,editor)) return (0);
   ;	
	push	di
	push	si
	call	near ptr _line_editor
	pop	cx
	pop	cx
	or	ax,ax
	jne	short @6@142
@6@86:
	xor	ax,ax
@6@114:
	jmp	short @6@338
@6@142:
   ;	
   ;	    if (file->openfile)
   ;	
	cmp	word ptr [si+82],0
	je	short @6@226
   ;	
   ;	     if (!(file->fp=g_fopen(file->filename,"rb+","EDITORFL"))) return (0);
   ;	
	mov	ax,offset DGROUP:s@+100
	push	ax
	mov	ax,offset DGROUP:s@+96
	push	ax
	push	si
	call	near ptr _g_fopen
	add	sp,6
	mov	word ptr [si+80],ax
	or	ax,ax
	jne	short @6@226
	jmp	short @6@86
@6@226:
	jmp	short @6@310
@6@254:
   ;	
   ;	    return (1);
   ;	  }
   ;	  if (!(file->fp=g_fopen(file->filename,"wb+","EDITORFL"))) return (0);
   ;	
	mov	ax,offset DGROUP:s@+113
	push	ax
	mov	ax,offset DGROUP:s@+109
	push	ax
	push	si
	call	near ptr _g_fopen
	add	sp,6
	mov	word ptr [si+80],ax
	or	ax,ax
	jne	short @6@310
	jmp	short @6@86
@6@310:
   ;	
   ;	  return (1);
   ;	
	mov	ax,1
	jmp	short @6@114
@6@338:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_create_temp_file	endp
   ;	
   ;	void generate_id(struct mail_pass_data *data, char *filename,
   ;	
	assume	cs:_TEXT
_generate_id	proc	near
	push	bp
	mov	bp,sp
	sub	sp,44
	push	si
	push	di
	mov	si,word ptr [bp+6]
	mov	di,word ptr [bp+8]
@7@30:
   ;	
   ;	                 struct file_entry *file)
   ;	{
   ;	  struct ffblk look_up;
   ;	
   ;	  do
   ;	  {
   ;	    sprintf(filename,"%02X%04X",sys_info.system_number,(unsigned)
   ;	
   ;	
   ;	            dans_counter);
   ;	
	push	word ptr DGROUP:_dans_counter
	mov	al,byte ptr DGROUP:_sys_info+305
	cbw	
	push	ax
	mov	ax,offset DGROUP:s@+122
	push	ax
	push	si
	call	near ptr _sprintf
	add	sp,8
   ;	
   ;	    sprintf(file->filename,"%s\\%s",data->basename,filename);
   ;	
	push	si
	push	word ptr [bp+4]
	mov	ax,offset DGROUP:s@+131
	push	ax
	push	di
	call	near ptr _sprintf
	add	sp,8
   ;	
   ;	    unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	    delay(1);
   ;	
	mov	ax,1
	push	ax
	call	near ptr _delay
	pop	cx
   ;	
   ;	    lock_dos();
   ;	
	call	near ptr _lock_dos
   ;	
   ;	  }
   ;	  while (!findfirst(file->filename,&look_up,FA_NORMAL));
   ;	
	xor	ax,ax
	push	ax
	lea	ax,word ptr [bp-44]
	push	ax
	push	di
	call	near ptr _findfirst
	add	sp,6
	or	ax,ax
	je	short @7@30
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_generate_id	endp
   ;	
   ;	int copy_a_file(struct file_entry *infile,
   ;	
	assume	cs:_TEXT
_copy_a_file	proc	near
	push	bp
	mov	bp,sp
	sub	sp,516
	push	si
	push	di
	mov	si,word ptr [bp+4]
	mov	di,word ptr [bp+6]
   ;	
   ;	                struct file_entry *outfile, int append)
   ;	{
   ;	  char buffer[512];
   ;	  int length;
   ;	  int delay;
   ;	
   ;	  if (!infile->openfile)
   ;	
	cmp	word ptr [si+82],0
	jne	short @8@142
   ;	
   ;	   if (!(infile->fp=g_fopen(infile->filename,"rb","INCOPY"))) return (0);
   ;	
	mov	ax,offset DGROUP:s@+140
	push	ax
	mov	ax,offset DGROUP:s@+137
	push	ax
	push	si
	call	near ptr _g_fopen
	add	sp,6
	mov	word ptr [si+80],ax
	or	ax,ax
	jne	short @8@142
@8@86:
	xor	ax,ax
@8@114:
	jmp	@8@674
@8@142:
   ;	
   ;	  if (!outfile->openfile)
   ;	
	cmp	word ptr [di+82],0
	jne	short @8@366
   ;	
   ;	   if (!(outfile->fp=g_fopen((char *)outfile,append ? "a" : "wb","OUTCOPY")))
   ;	
	mov	ax,offset DGROUP:s@+152
	push	ax
	cmp	word ptr [bp+8],0
	je	short @8@226
	mov	ax,offset DGROUP:s@+147
	jmp	short @8@254
@8@226:
	mov	ax,offset DGROUP:s@+149
@8@254:
	push	ax
	push	di
	call	near ptr _g_fopen
	add	sp,6
	mov	word ptr [di+80],ax
	or	ax,ax
	jne	short @8@366
   ;	
   ;	    {
   ;	      if (!infile->openfile) g_fclose(infile->fp);
   ;	
	cmp	word ptr [si+82],0
	jne	short @8@338
	push	word ptr [si+80]
	call	near ptr _g_fclose
	pop	cx
@8@338:
	jmp	short @8@86
@8@366:
   ;	
   ;	      return (0);
   ;	    }
   ;	  do
   ;	  {
   ;	    length = fread(buffer,sizeof(char),512,infile->fp);
   ;	
	push	word ptr [si+80]
	mov	ax,512
	push	ax
	mov	ax,1
	push	ax
	lea	ax,word ptr [bp-516]
	push	ax
	call	near ptr _fread
	add	sp,8
	mov	word ptr [bp-2],ax
   ;	
   ;	    fwrite(buffer,sizeof(char),length,outfile->fp);
   ;	
	push	word ptr [di+80]
	push	word ptr [bp-2]
	mov	ax,1
	push	ax
	lea	ax,word ptr [bp-516]
	push	ax
	call	near ptr _fwrite
	add	sp,8
   ;	
   ;	    unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	    for (delay=0;delay<STALL_TASKS;delay++) next_task();
   ;	
	mov	word ptr [bp-4],0
	jmp	short @8@450
@8@394:
	pushf	
	call	far ptr _next_task
	inc	word ptr [bp-4]
@8@450:
	cmp	word ptr [bp-4],30
	jl	short @8@394
   ;	
   ;	    lock_dos();
   ;	
	call	near ptr _lock_dos
   ;	
   ;	  } while (length);
   ;	
	cmp	word ptr [bp-2],0
	jne	short @8@366
   ;	
   ;	  if (!infile->openfile) g_fclose(infile->fp);
   ;	
	cmp	word ptr [si+82],0
	jne	short @8@590
	push	word ptr [si+80]
	call	near ptr _g_fclose
	pop	cx
@8@590:
   ;	
   ;	  if (!outfile->openfile) g_fclose(outfile->fp);
   ;	
	cmp	word ptr [di+82],0
	jne	short @8@646
	push	word ptr [di+80]
	call	near ptr _g_fclose
	pop	cx
@8@646:
   ;	
   ;	  return (1);
   ;	
	mov	ax,1
	jmp	@8@114
@8@674:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_copy_a_file	endp
   ;	
   ;	int scan_for_next_message(struct mail_pass_data *data,
   ;	
	assume	cs:_TEXT
_scan_for_next_message	proc	near
	push	bp
	mov	bp,sp
	sub	sp,274
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	                          struct mesg_entry *mentry)
   ;	{
   ;	  unsigned long int cur = mentry->entry_no;
   ;	
	mov	bx,word ptr [bp+6]
	mov	ax,word ptr [bx+2]
	mov	dx,word ptr [bx]
	mov	word ptr [bp-2],ax
	mov	word ptr [bp-4],dx
   ;	
   ;	  unsigned long int pos;
   ;	  struct mesg_entry temp_mentry;
   ;	  int find_tail;
   ;	  int find_new;
   ;	  unsigned long int real_tail,real_new;
   ;	
   ;	  find_tail = (cur == data->entry.tail_entry_no);
   ;	
	mov	ax,word ptr [si+89]
	mov	dx,word ptr [si+87]
	cmp	ax,word ptr [bp-2]
	jne	short @9@114
	cmp	dx,word ptr [bp-4]
	jne	short @9@114
	mov	ax,1
	jmp	short @9@142
@9@114:
	xor	ax,ax
@9@142:
	mov	di,ax
   ;	
   ;	  find_new = ((data->mail != 1) && (cur == data->entry.new_msg_no));
   ;	
	cmp	word ptr [si+338],1
	je	short @9@254
	mov	ax,word ptr [si+114]
	mov	dx,word ptr [si+112]
	cmp	ax,word ptr [bp-2]
	jne	short @9@254
	cmp	dx,word ptr [bp-4]
	jne	short @9@254
	mov	ax,1
	jmp	short @9@282
@9@254:
	xor	ax,ax
@9@282:
	mov	word ptr [bp-10],ax
   ;	
   ;	  if ((!find_tail) && (!find_new)) return (1);
   ;	
	or	di,di
	jne	short @9@366
	cmp	word ptr [bp-10],0
	jne	short @9@366
@9@338:
	mov	ax,1
	jmp	@9@1346
@9@366:
   ;	
   ;	  real_tail = -1;
   ;	
	mov	word ptr [bp-12],-1
	mov	word ptr [bp-14],-1
   ;	
   ;	  real_new = -1;
   ;	
	mov	word ptr [bp-16],-1
	mov	word ptr [bp-18],-1
	jmp	@9@730
@9@394:
   ;	
   ;	
   ;	  while ((cur<data->entry.head_entry_no) &&
   ;	         (((find_tail) && (real_tail == -1))
   ;	      || ((find_new) && (real_new == -1))))
   ;	  {
   ;	    pos = sizeof(struct first_entry)+
   ;	
   ;	
   ;	           (sizeof(struct mesg_entry)*(cur % data->entry.max_mesg));
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [si+80]
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	near ptr N_LUMOD@
	mov	cl,8
	call	near ptr N_LXLSH@
	add	ax,256
	adc	dx,0
	mov	word ptr [bp-6],dx
	mov	word ptr [bp-8],ax
   ;	
   ;	    fseek(data->fp,pos,SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	push	word ptr [si+336]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	    if (fread(&temp_mentry,sizeof(struct mesg_entry),1,data->fp) == 1)
   ;	
	push	word ptr [si+336]
	mov	ax,1
	push	ax
	mov	ax,256
	push	ax
	lea	ax,word ptr [bp-274]
	push	ax
	call	near ptr _fread
	add	sp,8
	cmp	ax,1
	jne	short @9@702
   ;	
   ;	    {
   ;	      if (!temp_mentry.deleted)
   ;	
	mov	al,byte ptr [bp-269]
	cbw	
	or	ax,ax
	jne	short @9@702
   ;	
   ;	      {
   ;	        if ((find_tail) && (real_tail == -1)) real_tail = cur;
   ;	
	or	di,di
	je	short @9@562
	cmp	word ptr [bp-12],-1
	jne	short @9@562
	cmp	word ptr [bp-14],-1
	jne	short @9@562
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	mov	word ptr [bp-12],ax
	mov	word ptr [bp-14],dx
@9@562:
   ;	
   ;	        if ((!temp_mentry.read) && (find_new) &&
   ;	
   ;	
   ;	              (real_new == -1)) real_new = cur;
   ;	
	mov	al,byte ptr [bp-270]
	cbw	
	or	ax,ax
	jne	short @9@702
	cmp	word ptr [bp-10],0
	je	short @9@702
	cmp	word ptr [bp-16],-1
	jne	short @9@702
	cmp	word ptr [bp-18],-1
	jne	short @9@702
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	mov	word ptr [bp-16],ax
	mov	word ptr [bp-18],dx
@9@702:
   ;	
   ;	      }
   ;	    }
   ;	    cur++;
   ;	
	add	word ptr [bp-4],1
	adc	word ptr [bp-2],0
@9@730:
	mov	ax,word ptr [si+85]
	mov	dx,word ptr [si+83]
	cmp	ax,word ptr [bp-2]
	jb	short @9@982
	ja	short @9@814
	cmp	dx,word ptr [bp-4]
	jbe	short @9@982
@9@814:
	or	di,di
	je	short @9@898
	cmp	word ptr [bp-12],-1
	jne	short @9@898
	cmp	word ptr [bp-14],-1
	jne	@@1
	jmp	@9@394
@@1:
@9@898:
	cmp	word ptr [bp-10],0
	je	short @9@982
	cmp	word ptr [bp-16],-1
	jne	short @9@982
	cmp	word ptr [bp-18],-1
	jne	@@2
	jmp	@9@394
@@2:
@9@982:
   ;	
   ;	  }
   ;	  if (find_tail)
   ;	
	or	di,di
	je	short @9@1150
   ;	
   ;	  {
   ;	    if (real_tail != -1) data->entry.tail_entry_no = real_tail;
   ;	
	cmp	word ptr [bp-12],-1
	jne	short @9@1066
	cmp	word ptr [bp-14],-1
	je	short @9@1094
@9@1066:
	mov	ax,word ptr [bp-12]
	mov	dx,word ptr [bp-14]
	jmp	short @9@1122
@9@1094:
   ;	
   ;	     else data->entry.tail_entry_no = data->entry.head_entry_no;
   ;	
	mov	ax,word ptr [si+85]
	mov	dx,word ptr [si+83]
@9@1122:
	mov	word ptr [si+89],ax
	mov	word ptr [si+87],dx
@9@1150:
   ;	
   ;	  }
   ;	  if (find_new)
   ;	
	cmp	word ptr [bp-10],0
	je	short @9@1318
   ;	
   ;	  {
   ;	    if (real_new != -1) data->entry.new_msg_no = real_new;
   ;	
	cmp	word ptr [bp-16],-1
	jne	short @9@1234
	cmp	word ptr [bp-18],-1
	je	short @9@1262
@9@1234:
	mov	ax,word ptr [bp-16]
	mov	dx,word ptr [bp-18]
	jmp	short @9@1290
@9@1262:
   ;	
   ;	     else data->entry.new_msg_no = data->entry.head_entry_no;
   ;	
	mov	ax,word ptr [si+85]
	mov	dx,word ptr [si+83]
@9@1290:
	mov	word ptr [si+114],ax
	mov	word ptr [si+112],dx
@9@1318:
   ;	
   ;	  }
   ;	  fseek(data->fp,0,SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	xor	ax,ax
	xor	dx,dx
	push	ax
	push	dx
	push	word ptr [si+336]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	  fwrite(&data->entry,sizeof(struct first_entry),1,data->fp);
   ;	
	push	word ptr [si+336]
	mov	ax,1
	push	ax
	mov	ax,256
	push	ax
	mov	ax,si
	add	ax,80
	push	ax
	call	near ptr _fwrite
	add	sp,8
	jmp	@9@338
@9@1346:
   ;	
   ;	  return (1);
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_scan_for_next_message	endp
   ;	
   ;	int add_msg_to_base(struct mail_pass_data *data, int dest_no,
   ;	
	assume	cs:_TEXT
_add_msg_to_base	proc	near
	push	bp
	mov	bp,sp
	sub	sp,340
	push	si
	push	di
	mov	si,word ptr [bp+4]
	mov	di,word ptr [bp+10]
   ;	
   ;	                    struct file_entry *file, struct mesg_entry *mentry)
   ;	{
   ;	  unsigned long int pos = sizeof(struct first_entry)+
   ;	
   ;	
   ;	       (sizeof(struct mesg_entry)*
   ;	       (data->entry.head_entry_no % data->entry.max_mesg));
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [si+80]
	push	word ptr [si+85]
	push	word ptr [si+83]
	call	near ptr N_LUMOD@
	mov	cl,8
	call	near ptr N_LXLSH@
	add	ax,256
	adc	dx,0
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
   ;	
   ;	  struct mesg_entry mentryold;
   ;	  char filename[FILENAME_LEN];
   ;	
   ;	  fseek(data->fp,pos,SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr [si+336]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	  if (fread(&mentryold,sizeof(struct mesg_entry),1,data->fp) == 1)
   ;	
	push	word ptr [si+336]
	mov	ax,1
	push	ax
	mov	ax,256
	push	ax
	lea	ax,word ptr [bp-260]
	push	ax
	call	near ptr _fread
	add	sp,8
	cmp	ax,1
	jne	short @10@226
   ;	
   ;	  {
   ;	    if (!mentryold.deleted)
   ;	
	mov	al,byte ptr [bp-255]
	cbw	
	or	ax,ax
	jne	short @10@226
   ;	
   ;	    {
   ;	      if (1 ||(data->mail == -1))
   ;	
	jmp	short @10@142
	cmp	word ptr [si+338],-1
	jne	short @10@170
@10@142:
   ;	
   ;	      {
   ;	        sprintf(filename,"%s\\%s",data->basename,mentryold.filename);
   ;	
	lea	ax,word ptr [bp-250]
	push	ax
	push	si
	mov	ax,offset DGROUP:s@+160
	push	ax
	lea	ax,word ptr [bp-340]
	push	ax
	call	near ptr _sprintf
	add	sp,8
   ;	
   ;	        remove(filename);
   ;	
	lea	ax,word ptr [bp-340]
	push	ax
	call	near ptr _remove
	pop	cx
   ;	
   ;	        mentryold.deleted = 1;
   ;	
	mov	byte ptr [bp-255],1
   ;	
   ;	        fseek(data->fp,pos,SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr [si+336]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	        fwrite(&mentryold,sizeof(struct mesg_entry),1,data->fp);
   ;	
	push	word ptr [si+336]
	mov	ax,1
	push	ax
	mov	ax,256
	push	ax
	lea	ax,word ptr [bp-260]
	push	ax
	call	near ptr _fwrite
	add	sp,8
   ;	
   ;	        scan_for_next_message(data,&mentryold);
   ;	
	lea	ax,word ptr [bp-260]
	push	ax
	push	si
	call	near ptr _scan_for_next_message
	pop	cx
	pop	cx
   ;	
   ;	      } else return(0);
   ;	
	jmp	short @10@226
@10@170:
	xor	ax,ax
@10@198:
	jmp	@10@338
@10@226:
   ;	
   ;	    }
   ;	  }
   ;	  fseek(data->fp,pos,SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr [si+336]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	  mentry->entry_no = data->entry.head_entry_no;
   ;	
	mov	ax,word ptr [si+85]
	mov	dx,word ptr [si+83]
	mov	word ptr [di+2],ax
	mov	word ptr [di],dx
   ;	
   ;	  mentry->read = 0;
   ;	
	mov	byte ptr [di+4],0
   ;	
   ;	  mentry->deleted = 0;
   ;	
	mov	byte ptr [di+5],0
   ;	
   ;	  mentry->ent_date = time(NULL);
   ;	
	xor	ax,ax
	push	ax
	call	near ptr _time
	pop	cx
	mov	word ptr [di+8],dx
	mov	word ptr [di+6],ax
   ;	
   ;	  mentry->dest_no = dest_no;
   ;	
	mov	ax,word ptr [bp+6]
	mov	word ptr [di+26],ax
   ;	
   ;	  generate_id(data,mentry->filename,file);
   ;	
	push	word ptr [bp+8]
	mov	ax,di
	add	ax,10
	push	ax
	push	si
	call	near ptr _generate_id
	add	sp,6
   ;	
   ;	  fwrite(mentry,sizeof(struct mesg_entry),1,data->fp);
   ;	
	push	word ptr [si+336]
	mov	ax,1
	push	ax
	mov	ax,256
	push	ax
	push	di
	call	near ptr _fwrite
	add	sp,8
   ;	
   ;	  data->entry.head_entry_no++;
   ;	
	add	word ptr [si+83],1
	adc	word ptr [si+85],0
   ;	
   ;	  if (file->openfile)
   ;	
	mov	bx,word ptr [bp+8]
	cmp	word ptr [bx+82],0
	je	short @10@310
   ;	
   ;	   if (!(file->fp = g_fopen(file->filename,"wb+","ADDMSG"))) return (0);
   ;	
	mov	ax,offset DGROUP:s@+170
	push	ax
	mov	ax,offset DGROUP:s@+166
	push	ax
	push	word ptr [bp+8]
	call	near ptr _g_fopen
	add	sp,6
	mov	bx,word ptr [bp+8]
	mov	word ptr [bx+80],ax
	or	ax,ax
	jne	short @10@310
	jmp	@10@170
@10@310:
   ;	
   ;	  return (1);
   ;	
	mov	ax,1
	jmp	@10@198
@10@338:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_add_msg_to_base	endp
   ;	
   ;	int delete_msg_from_base(struct mail_pass_data *data,
   ;	
	assume	cs:_TEXT
_delete_msg_from_base	proc	near
	push	bp
	mov	bp,sp
	sub	sp,340
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	       unsigned long int mesg_no, int do_erase)
   ;	{
   ;	  unsigned long int pos = sizeof(struct first_entry)+
   ;	
   ;	
   ;	       (sizeof(struct mesg_entry)*
   ;	       (mesg_no % data->entry.max_mesg));
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [si+80]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr N_LUMOD@
	mov	cl,8
	call	near ptr N_LXLSH@
	add	ax,256
	adc	dx,0
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
   ;	
   ;	  struct mesg_entry mentry;
   ;	  char filename[FILENAME_LEN];
   ;	
   ;	  fseek(data->fp,pos,SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr [si+336]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	  if (fread(&mentry,sizeof(struct mesg_entry),1,data->fp) != 1) return (0);
   ;	
	push	word ptr [si+336]
	mov	ax,1
	push	ax
	mov	ax,256
	push	ax
	lea	ax,word ptr [bp-260]
	push	ax
	call	near ptr _fread
	add	sp,8
	cmp	ax,1
	je	short @11@114
	xor	ax,ax
@11@86:
	jmp	short @11@226
@11@114:
   ;	
   ;	  if ((!mentry.deleted) && (do_erase))
   ;	
	mov	al,byte ptr [bp-255]
	cbw	
	or	ax,ax
	jne	short @11@198
	cmp	word ptr [bp+10],0
	je	short @11@198
   ;	
   ;	  {
   ;	    sprintf(filename,"%s\\%s",data->basename,mentry.filename);
   ;	
	lea	ax,word ptr [bp-250]
	push	ax
	push	si
	mov	ax,offset DGROUP:s@+177
	push	ax
	lea	ax,word ptr [bp-340]
	push	ax
	call	near ptr _sprintf
	add	sp,8
   ;	
   ;	    remove(filename);
   ;	
	lea	ax,word ptr [bp-340]
	push	ax
	call	near ptr _remove
	pop	cx
@11@198:
   ;	
   ;	  }
   ;	  mentry.deleted = 1;
   ;	
	mov	byte ptr [bp-255],1
   ;	
   ;	  mentry.read = 1;
   ;	
	mov	byte ptr [bp-256],1
   ;	
   ;	  fseek(data->fp,pos,SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr [si+336]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	  fwrite(&mentry,sizeof(struct mesg_entry),1,data->fp);
   ;	
	push	word ptr [si+336]
	mov	ax,1
	push	ax
	mov	ax,256
	push	ax
	lea	ax,word ptr [bp-260]
	push	ax
	call	near ptr _fwrite
	add	sp,8
   ;	
   ;	/*  scan_for_next_message(data,&mentry); */
   ;	  return (1);
   ;	
	mov	ax,1
	jmp	short @11@86
@11@226:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_delete_msg_from_base	endp
   ;	
   ;	int get_entry(struct mail_pass_data *data, unsigned long int mesg_no,
   ;	
	assume	cs:_TEXT
_get_entry	proc	near
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
	mov	si,word ptr [bp+4]
	mov	di,word ptr [bp+12]
   ;	
   ;	     struct mesg_entry *mentry, struct file_entry *file)
   ;	{
   ;	  unsigned long int pos = sizeof(struct first_entry)+
   ;	
   ;	
   ;	       (sizeof(struct mesg_entry)*
   ;	       (mesg_no % data->entry.max_mesg));
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [si+80]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr N_LUMOD@
	mov	cl,8
	call	near ptr N_LXLSH@
	add	ax,256
	adc	dx,0
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
   ;	
   ;	
   ;	  fseek(data->fp,pos,SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr [si+336]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	  if (fread(mentry,sizeof(struct mesg_entry),1,data->fp) != 1)
   ;	
	push	word ptr [si+336]
	mov	ax,1
	push	ax
	mov	ax,256
	push	ax
	push	word ptr [bp+10]
	call	near ptr _fread
	add	sp,8
	cmp	ax,1
	je	short @12@114
@12@58:
   ;	
   ;	     return (0);
   ;	
	xor	ax,ax
@12@86:
	jmp	short @12@226
@12@114:
   ;	
   ;	  sprintf(file->filename,"%s\\%s",data->basename,mentry->filename);
   ;	
	mov	ax,word ptr [bp+10]
	add	ax,10
	push	ax
	push	si
	mov	ax,offset DGROUP:s@+183
	push	ax
	push	di
	call	near ptr _sprintf
	add	sp,8
   ;	
   ;	  if (file->openfile)
   ;	
	cmp	word ptr [di+82],0
	je	short @12@198
   ;	
   ;	    if (!(file->fp=g_fopen(file->filename,"rb","GETBBS"))) return (0);
   ;	
	mov	ax,offset DGROUP:s@+192
	push	ax
	mov	ax,offset DGROUP:s@+189
	push	ax
	push	di
	call	near ptr _g_fopen
	add	sp,6
	mov	word ptr [di+80],ax
	or	ax,ax
	jne	short @12@198
	jmp	short @12@58
@12@198:
   ;	
   ;	  return (1);
   ;	
	mov	ax,1
	jmp	short @12@86
@12@226:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_get_entry	endp
   ;	
   ;	int read_entry(struct mail_pass_data *data, unsigned long int mesg_no,
   ;	
	assume	cs:_TEXT
_read_entry	proc	near
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
	mov	si,word ptr [bp+4]
	mov	di,word ptr [bp+10]
   ;	
   ;	     struct mesg_entry *mentry, struct file_entry *file)
   ;	{
   ;	
   ;	  unsigned long int pos = sizeof(struct first_entry)+
   ;	
   ;	
   ;	       (sizeof(struct mesg_entry)*
   ;	       (mesg_no % data->entry.max_mesg));
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [si+80]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr N_LUMOD@
	mov	cl,8
	call	near ptr N_LXLSH@
	add	ax,256
	adc	dx,0
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
   ;	
   ;	
   ;	  fseek(data->fp,pos,SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr [si+336]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	  if (fread(mentry,sizeof(struct mesg_entry),1,data->fp) != 1)
   ;	
	push	word ptr [si+336]
	mov	ax,1
	push	ax
	mov	ax,256
	push	ax
	push	di
	call	near ptr _fread
	add	sp,8
	cmp	ax,1
	je	short @13@114
@13@58:
   ;	
   ;	      return (0);
   ;	
	xor	ax,ax
@13@86:
	jmp	@13@338
@13@114:
   ;	
   ;	  sprintf(file->filename,"%s\\%s",data->basename,mentry->filename);
   ;	
	mov	ax,di
	add	ax,10
	push	ax
	push	si
	mov	ax,offset DGROUP:s@+199
	push	ax
	push	word ptr [bp+12]
	call	near ptr _sprintf
	add	sp,8
   ;	
   ;	  if (!mentry->deleted)
   ;	
	mov	al,byte ptr [di+5]
	cbw	
	or	ax,ax
	jne	short @13@226
   ;	
   ;	  {
   ;	    if (data->mail != -1)
   ;	
	cmp	word ptr [si+338],-1
	je	short @13@198
   ;	
   ;	    {
   ;	      mentry->read = 1;
   ;	
	mov	byte ptr [di+4],1
   ;	
   ;	      fseek(data->fp,pos,SEEK_SET);
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr [si+336]
	call	near ptr _fseek
	add	sp,8
   ;	
   ;	      fwrite(mentry,sizeof(struct mesg_entry),1,data->fp);
   ;	
	push	word ptr [si+336]
	mov	ax,1
	push	ax
	mov	ax,256
	push	ax
	push	di
	call	near ptr _fwrite
	add	sp,8
@13@198:
   ;	
   ;	    }
   ;	    scan_for_next_message(data,mentry);
   ;	
	push	di
	push	si
	call	near ptr _scan_for_next_message
	pop	cx
	pop	cx
@13@226:
   ;	
   ;	  }
   ;	  if (file->openfile)
   ;	
	mov	bx,word ptr [bp+12]
	cmp	word ptr [bx+82],0
	je	short @13@310
   ;	
   ;	    if (!(file->fp=g_fopen(file->filename,"rb","READBBS"))) return (0);
   ;	
	mov	ax,offset DGROUP:s@+208
	push	ax
	mov	ax,offset DGROUP:s@+205
	push	ax
	push	word ptr [bp+12]
	call	near ptr _g_fopen
	add	sp,6
	mov	bx,word ptr [bp+12]
	mov	word ptr [bx+80],ax
	or	ax,ax
	jne	short @13@310
	jmp	@13@58
@13@310:
   ;	
   ;	  return (1);
   ;	
	mov	ax,1
	jmp	@13@86
@13@338:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_read_entry	endp
   ;	
   ;	void examine_board(void)
   ;	
	assume	cs:_TEXT
_examine_board	proc	near
	push	bp
	mov	bp,sp
	sub	sp,504
   ;	
   ;	{
   ;	  struct mail_pass_data data;
   ;	  char s[160];
   ;	  char *dat;
   ;	  int user_num;
   ;	
   ;	  print_cr();
   ;	
	call	near ptr _print_cr
   ;	
   ;	  print_string("Which user number to read: ");
   ;	
	mov	ax,offset DGROUP:s@+216
	push	ax
	call	near ptr _print_string
	pop	cx
   ;	
   ;	  get_editor_string(s,5);
   ;	
	mov	ax,5
	push	ax
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _get_editor_string
	pop	cx
	pop	cx
   ;	
   ;	  user_num=str_to_num(s,&dat);
   ;	
	lea	ax,word ptr [bp-2]
	push	ax
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _str_to_num
	pop	cx
	pop	cx
	mov	word ptr [bp-4],ax
   ;	
   ;	
   ;	  open_mail_base(user_num,&data,0);
   ;	
	xor	ax,ax
	push	ax
	lea	ax,word ptr [bp-344]
	push	ax
	push	word ptr [bp-4]
	call	near ptr _open_mail_base
	add	sp,6
   ;	
   ;	  sprintf(s,"Basename: %s",data.basename);
   ;	
	lea	ax,word ptr [bp-344]
	push	ax
	mov	ax,offset DGROUP:s@+244
	push	ax
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	  print_str_cr(s);
   ;	
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _print_str_cr
	pop	cx
   ;	
   ;	  sprintf(s,"Mail:     %d",data.mail);
   ;	
	push	word ptr [bp-6]
	mov	ax,offset DGROUP:s@+257
	push	ax
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	  print_str_cr(s);
   ;	
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _print_str_cr
	pop	cx
   ;	
   ;	  sprintf(s,"Max mesg: %d",data.entry.max_mesg);
   ;	
	push	word ptr [bp-264]
	mov	ax,offset DGROUP:s@+270
	push	ax
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	  print_str_cr(s);
   ;	
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _print_str_cr
	pop	cx
   ;	
   ;	  sprintf(s,"Priority: %d",data.entry.priority);
   ;	
	mov	al,byte ptr [bp-262]
	mov	ah,0
	push	ax
	mov	ax,offset DGROUP:s@+283
	push	ax
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	  print_str_cr(s);
   ;	
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _print_str_cr
	pop	cx
   ;	
   ;	  sprintf(s,"Head/Tail: %ld %ld %ld",data.entry.head_entry_no,
   ;	
   ;	
   ;	        data.entry.tail_entry_no,data.entry.new_msg_no);
   ;	
	push	word ptr [bp-230]
	push	word ptr [bp-232]
	push	word ptr [bp-255]
	push	word ptr [bp-257]
	push	word ptr [bp-259]
	push	word ptr [bp-261]
	mov	ax,offset DGROUP:s@+296
	push	ax
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _sprintf
	add	sp,16
   ;	
   ;	  print_str_cr(s);
   ;	
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _print_str_cr
	pop	cx
   ;	
   ;	  sprintf(s,"Name:     %d",data.entry.name);
   ;	
	lea	ax,word ptr [bp-253]
	push	ax
	mov	ax,offset DGROUP:s@+319
	push	ax
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _sprintf
	add	sp,6
   ;	
   ;	  print_str_cr(s);
   ;	
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _print_str_cr
	pop	cx
   ;	
   ;	  g_fclose(data.fp);
   ;	
	push	word ptr [bp-8]
	call	near ptr _g_fclose
	pop	cx
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_examine_board	endp
   ;	
   ;	int send_mail_to(struct mesg_entry *mentry, struct file_entry *file,
   ;	
	assume	cs:_TEXT
_send_mail_to	proc	near
	push	bp
	mov	bp,sp
	sub	sp,504
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	     int user_no)
   ;	{
   ;	  struct file_entry file2;
   ;	  struct mail_pass_data data;
   ;	  int error = 1;
   ;	
	mov	di,1
   ;	
   ;	  char s[80];
   ;	
   ;	  file2.openfile = 1;
   ;	
	mov	word ptr [bp-2],1
   ;	
   ;	  lock_dos();
   ;	
	call	near ptr _lock_dos
   ;	
   ;	  data.entry.max_mesg = MAIL_MAX;
   ;	
	mov	word ptr [bp-344],3
   ;	
   ;	  if (!open_mail_base(user_no,&data,1)) error = 0;
   ;	
	mov	ax,1
	push	ax
	lea	ax,word ptr [bp-424]
	push	ax
	push	word ptr [bp+8]
	call	near ptr _open_mail_base
	add	sp,6
	or	ax,ax
	jne	short @15@86
	xor	di,di
	jmp	@15@254
@15@86:
   ;	
   ;	   else if (!add_msg_to_base(&data,0,&file2,mentry)) error = 0;
   ;	
	push	si
	lea	ax,word ptr [bp-84]
	push	ax
	xor	ax,ax
	push	ax
	lea	ax,word ptr [bp-424]
	push	ax
	call	near ptr _add_msg_to_base
	add	sp,8
	or	ax,ax
	jne	short @15@142
	xor	di,di
	jmp	@15@254
@15@142:
   ;	
   ;	    else
   ;	    {
   ;	      fprintf(file2.fp,"|*h1|*f4From:     |*f7");
   ;	
	mov	ax,offset DGROUP:s@+332
	push	ax
	push	word ptr [bp-4]
	call	near ptr _fprintf
	pop	cx
	pop	cx
   ;	
   ;	      if (user_no < 0) fprintf(file2.fp,"(%%GST)");
   ;	
	cmp	word ptr [bp+8],0
	jge	short @15@198
	mov	ax,offset DGROUP:s@+355
	push	ax
	push	word ptr [bp-4]
	call	near ptr _fprintf
	pop	cx
	pop	cx
	jmp	short @15@226
@15@198:
   ;	
   ;	       else fprintf(file2.fp,"(#%03d)",mentry->user_no);
   ;	
	push	word ptr [si+28]
	mov	ax,offset DGROUP:s@+363
	push	ax
	push	word ptr [bp-4]
	call	near ptr _fprintf
	add	sp,6
@15@226:
   ;	
   ;	      fprintf(file2.fp," %s|*r1\r\n",mentry->username);
   ;	
	mov	ax,si
	add	ax,30
	push	ax
	mov	ax,offset DGROUP:s@+371
	push	ax
	push	word ptr [bp-4]
	call	near ptr _fprintf
	add	sp,6
   ;	
   ;	      fprintf(file2.fp,"|*h1|*f4System:   |*f7( %03d) %s|*r1\r\n",
   ;	
   ;	
   ;	        mentry->user_no,mentry->systemname);
   ;	
	mov	ax,si
	add	ax,111
	push	ax
	push	word ptr [si+28]
	mov	ax,offset DGROUP:s@+381
	push	ax
	push	word ptr [bp-4]
	call	near ptr _fprintf
	add	sp,8
   ;	
   ;	      fprintf(file2.fp,"|*h1|*f4Subject:  |*f7%s|*r1\r\n",mentry->subject);
   ;	
	mov	ax,si
	add	ax,152
	push	ax
	mov	ax,offset DGROUP:s@+420
	push	ax
	push	word ptr [bp-4]
	call	near ptr _fprintf
	add	sp,6
   ;	
   ;	      strftime(s,39,"%m/%d/%y %I:%M:%S %p",localtime(&mentry->ent_date));
   ;	
	mov	ax,si
	add	ax,6
	push	ax
	call	near ptr _localtime
	pop	cx
	push	ax
	mov	ax,offset DGROUP:s@+451
	push	ax
	mov	ax,39
	push	ax
	lea	ax,word ptr [bp-504]
	push	ax
	call	near ptr _strftime
	add	sp,8
   ;	
   ;	      fprintf(file2.fp,"|*h1|*f4Date:     |*f7%s [%lu]|*r1\r\n",
   ;	
   ;	
   ;	            s,mentry->ent_date);
   ;	
	push	word ptr [si+8]
	push	word ptr [si+6]
	lea	ax,word ptr [bp-504]
	push	ax
	mov	ax,offset DGROUP:s@+472
	push	ax
	push	word ptr [bp-4]
	call	near ptr _fprintf
	add	sp,10
   ;	
   ;	      fprintf(file2.fp,"|*h1|*f4Contents:|*r1\r\n\r\n");
   ;	
	mov	ax,offset DGROUP:s@+509
	push	ax
	push	word ptr [bp-4]
	call	near ptr _fprintf
	pop	cx
	pop	cx
   ;	
   ;	      copy_a_file(file,&file2,0);
   ;	
	xor	ax,ax
	push	ax
	lea	ax,word ptr [bp-84]
	push	ax
	push	word ptr [bp+6]
	call	near ptr _copy_a_file
	add	sp,6
   ;	
   ;	      g_fclose(file2.fp);
   ;	
	push	word ptr [bp-4]
	call	near ptr _g_fclose
	pop	cx
@15@254:
   ;	
   ;	    }
   ;	  close_new_base(&data);
   ;	
	lea	ax,word ptr [bp-424]
	push	ax
	call	near ptr _close_new_base
	pop	cx
   ;	
   ;	  unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	  return (error);
   ;	
	mov	ax,di
	jmp	short @15@282
@15@282:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_send_mail_to	endp
   ;	
   ;	int send_mail_subj(int user_no)
   ;	
	assume	cs:_TEXT
_send_mail_subj	proc	near
	push	bp
	mov	bp,sp
	sub	sp,924
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int error = 1;
   ;	
	mov	di,1
   ;	
   ;	  struct mesg_entry mentry;
   ;	  struct file_entry file;
   ;	  struct user_data temp_data;
   ;	  char s[200];
   ;	
   ;	  if ((user_no>=0) && (user_no<=999))
   ;	
	or	si,si
	jl	short @16@142
	cmp	si,999
	jg	short @16@142
   ;	
   ;	   if (!load_user(user_no,&temp_data)) error = 0;
   ;	
	lea	ax,word ptr [bp-724]
	push	ax
	push	si
	call	near ptr _load_user
	pop	cx
	pop	cx
	or	ax,ax
	jne	short @16@142
	xor	di,di
@16@142:
   ;	
   ;	
   ;	  if (error)
   ;	
	or	di,di
	je	short @16@254
   ;	
   ;	  {
   ;	    print_str_cr("That user does NOT EXIST");
   ;	
	mov	ax,offset DGROUP:s@+535
	push	ax
	call	near ptr _print_str_cr
	pop	cx
   ;	
   ;	    return (0);
   ;	
@16@198:
	xor	ax,ax
@16@226:
	jmp	@16@394
@16@254:
   ;	
   ;	  }
   ;	  print_cr();
   ;	
	call	near ptr _print_cr
   ;	
   ;	  special_code(1,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	ax,1
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
   ;	
   ;	  sprintf(s,"|*f4|*h1     To: |*f7(#%03d) %c|*r1%s|*r1|*f7|*h1%c ",user_no,temp_data.staple[2],temp_data.handle,temp_data.staple
   ;	
	mov	al,byte ptr [bp-631]
	cbw	
	push	ax
	lea	ax,word ptr [bp-722]
	push	ax
	mov	al,byte ptr [bp-632]
	cbw	
	push	ax
	push	si
	mov	ax,offset DGROUP:s@+560
	push	ax
	lea	ax,word ptr [bp-924]
	push	ax
	call	near ptr _sprintf
	add	sp,12
   ;	
   ;	  print_str_cr(s);
   ;	
	lea	ax,word ptr [bp-924]
	push	ax
	call	near ptr _print_str_cr
	pop	cx
   ;	
   ;	  print_string("Subject: ");
   ;	
	mov	ax,offset DGROUP:s@+613
	push	ax
	call	near ptr _print_string
	pop	cx
   ;	
   ;	  special_code(0,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	xor	ax,ax
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
   ;	
   ;	  get_string_cntrl(mentry.subject,SUBJECT_LEN,0,0,0,0,1,0,0);
   ;	
	mov	al,0
	push	ax
	mov	al,0
	push	ax
	mov	al,1
	push	ax
	mov	al,0
	push	ax
	mov	al,0
	push	ax
	mov	al,0
	push	ax
	mov	al,0
	push	ax
	mov	ax,80
	push	ax
	lea	ax,word ptr [bp-104]
	push	ax
	call	near ptr _get_string_cntrl
	add	sp,18
   ;	
   ;	  print_cr();
   ;	
	call	near ptr _print_cr
   ;	
   ;	  if (!(*mentry.subject)) return (0);
   ;	
	mov	al,byte ptr [bp-104]
	cbw	
	or	ax,ax
	jne	short @16@310
	jmp	@16@198
@16@310:
   ;	
   ;	  file.openfile = 0;
   ;	
	mov	word ptr [bp-258],0
   ;	
   ;	  if (!create_temp_file(&file,MAX_EDITOR_LEN,0)) return (0);
   ;	
	xor	ax,ax
	push	ax
	mov	ax,16384
	push	ax
	lea	ax,word ptr [bp-340]
	push	ax
	call	near ptr _create_temp_file
	add	sp,6
	or	ax,ax
	jne	short @16@366
	jmp	@16@198
@16@366:
   ;	
   ;	  mentry.dest_user_no = user_no;
   ;	
	mov	word ptr [bp-23],si
   ;	
   ;	  mentry.system_no = sys_info.system_number;
   ;	
	mov	al,byte ptr DGROUP:_sys_info+305
	cbw	
	mov	word ptr [bp-232],ax
   ;	
   ;	  mentry.user_no = user_lines[tswitch].number;
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,384
	imul	dx
	mov	dx,seg _user_lines
	mov	bx,ax
	mov	es,dx
	mov	ax,word ptr es:_user_lines[bx]
	mov	word ptr [bp-228],ax
   ;	
   ;	  mentry.dest_no = sys_info.system_number;
   ;	
	mov	al,byte ptr DGROUP:_sys_info+305
	cbw	
	mov	word ptr [bp-230],ax
   ;	
   ;	  strcpy_n(mentry.username,user_lines[tswitch].handle,USER_NAME_LEN);
   ;	
	mov	ax,80
	push	ax
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,384
	imul	dx
	add	ax,offset _user_lines+2
	push	ax
	lea	ax,word ptr [bp-226]
	push	ax
	call	near ptr _strcpy_n
	add	sp,6
   ;	
   ;	  strcpy_n(mentry.systemname,sys_info.system_name,SYSTEM_NAME_LEN);
   ;	
	mov	ax,40
	push	ax
	mov	ax,offset DGROUP:_sys_info+176
	push	ax
	lea	ax,word ptr [bp-145]
	push	ax
	call	near ptr _strcpy_n
	add	sp,6
   ;	
   ;	  lock_dos();
   ;	
	call	near ptr _lock_dos
   ;	
   ;	  mentry.ent_date=time(NULL);
   ;	
	xor	ax,ax
	push	ax
	call	near ptr _time
	pop	cx
	mov	word ptr [bp-248],dx
	mov	word ptr [bp-250],ax
   ;	
   ;	  unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	  send_mail_to(&mentry,&file,user_no);
   ;	
	push	si
	lea	ax,word ptr [bp-340]
	push	ax
	lea	ax,word ptr [bp-256]
	push	ax
	call	near ptr _send_mail_to
	add	sp,6
   ;	
   ;	  return (1);
   ;	
	mov	ax,1
	jmp	@16@226
@16@394:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_send_mail_subj	endp
   ;	
   ;	void send_mail_message(void)
   ;	
	assume	cs:_TEXT
_send_mail_message	proc	near
	push	bp
	mov	bp,sp
	sub	sp,14
   ;	
   ;	{
   ;	  char s[10];
   ;	  char *data;
   ;	  int user_num;
   ;	
   ;	  print_cr();
   ;	
	call	near ptr _print_cr
   ;	
   ;	  print_string("Which user number to send to: ");
   ;	
	mov	ax,offset DGROUP:s@+623
	push	ax
	call	near ptr _print_string
	pop	cx
   ;	
   ;	  get_editor_string(s,5);
   ;	
	mov	ax,5
	push	ax
	lea	ax,word ptr [bp-10]
	push	ax
	call	near ptr _get_editor_string
	pop	cx
	pop	cx
   ;	
   ;	  user_num=str_to_num(s,&data);
   ;	
	lea	ax,word ptr [bp-12]
	push	ax
	lea	ax,word ptr [bp-10]
	push	ax
	call	near ptr _str_to_num
	pop	cx
	pop	cx
	mov	word ptr [bp-14],ax
   ;	
   ;	  send_mail_subj(user_num);
   ;	
	push	word ptr [bp-14]
	call	near ptr _send_mail_subj
	pop	cx
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_send_mail_message	endp
   ;	
   ;	void read_messages(struct mail_pass_data *data, unsigned long int start)
   ;	
	assume	cs:_TEXT
_read_messages	proc	near
	push	bp
	mov	bp,sp
	sub	sp,422
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int flag = 1;
   ;	
	mov	word ptr [bp-2],1
   ;	
   ;	  struct mesg_entry mentry;
   ;	  struct file_entry file;
   ;	  char s[80];
   ;	
   ;	  if (start<data->entry.tail_entry_no) start = data->entry.tail_entry_no;
   ;	
	mov	ax,word ptr [si+89]
	mov	dx,word ptr [si+87]
	cmp	ax,word ptr [bp+8]
	jb	short @18@142
	ja	short @18@114
	cmp	dx,word ptr [bp+6]
	jbe	short @18@142
@18@114:
	mov	ax,word ptr [si+89]
	mov	dx,word ptr [si+87]
	mov	word ptr [bp+8],ax
	mov	word ptr [bp+6],dx
@18@142:
	jmp	@18@478
@18@170:
   ;	
   ;	  while ((flag) && (start<data->entry.head_entry_no))
   ;	  {
   ;	    file.openfile = 0;
   ;	
	mov	word ptr [bp-260],0
   ;	
   ;	    mentry.deleted = 1;
   ;	
	mov	byte ptr [bp-253],1
	jmp	short @18@254
@18@198:
   ;	
   ;	    while ((mentry.deleted) && (start<data->entry.head_entry_no))
   ;	    {
   ;	      lock_dos();
   ;	
	call	near ptr _lock_dos
   ;	
   ;	      read_entry(data,start,&mentry,&file);
   ;	
	lea	ax,word ptr [bp-342]
	push	ax
	lea	ax,word ptr [bp-258]
	push	ax
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	si
	call	near ptr _read_entry
	add	sp,10
   ;	
   ;	      unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	      if (mentry.deleted) start++;
   ;	
	cmp	byte ptr [bp-253],0
	je	short @18@254
	add	word ptr [bp+6],1
	adc	word ptr [bp+8],0
@18@254:
	cmp	byte ptr [bp-253],0
	je	short @18@366
	mov	ax,word ptr [si+85]
	mov	dx,word ptr [si+83]
	cmp	ax,word ptr [bp+8]
	ja	short @18@198
	jne	short @18@366
	cmp	dx,word ptr [bp+6]
	ja	short @18@198
@18@366:
   ;	
   ;	    }
   ;	    if (start<data->entry.head_entry_no)
   ;	
	mov	ax,word ptr [si+85]
	mov	dx,word ptr [si+83]
	cmp	ax,word ptr [bp+8]
	jb	short @18@478
	ja	short @18@450
	cmp	dx,word ptr [bp+6]
	jbe	short @18@478
@18@450:
   ;	
   ;	    {
   ;	      sprintf(s,"|*h1|*f4Message   |*f7%02ld |*f4of |*f7%02d|*r1",
   ;	
   ;	
   ;	         1 + data->entry.max_mesg -
   ;	         (data->entry.head_entry_no - start),data->entry.max_mesg);
   ;	
	push	word ptr [si+80]
	mov	ax,word ptr [si+80]
	inc	ax
	xor	dx,dx
	mov	bx,word ptr [si+85]
	mov	cx,word ptr [si+83]
	sub	cx,word ptr [bp+6]
	sbb	bx,word ptr [bp+8]
	sub	ax,cx
	sbb	dx,bx
	push	dx
	push	ax
	mov	ax,offset DGROUP:s@+654
	push	ax
	lea	ax,word ptr [bp-422]
	push	ax
	call	near ptr _sprintf
	add	sp,10
   ;	
   ;	      special_code(1,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	ax,1
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
   ;	
   ;	      print_str_cr(s);
   ;	
	lea	ax,word ptr [bp-422]
	push	ax
	call	near ptr _print_str_cr
	pop	cx
   ;	
   ;	      special_code(0,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	xor	ax,ax
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
   ;	
   ;	      print_file_to_cntrl(file.filename,tswitch,1,1,1,MAIL_PAGING);
   ;	
	mov	ax,22
	push	ax
	mov	ax,1
	push	ax
	mov	ax,1
	push	ax
	mov	ax,1
	push	ax
	push	word ptr DGROUP:_tswitch
	lea	ax,word ptr [bp-342]
	push	ax
	call	near ptr _print_file_to_cntrl
	add	sp,12
   ;	
   ;	      start++;
   ;	
	add	word ptr [bp+6],1
	adc	word ptr [bp+8],0
@18@478:
	cmp	word ptr [bp-2],0
	je	short @18@590
	mov	ax,word ptr [si+85]
	mov	dx,word ptr [si+83]
	cmp	ax,word ptr [bp+8]
	jbe	@@3
	jmp	@18@170
@@3:
	jne	short @18@590
	cmp	dx,word ptr [bp+6]
	jbe	@@4
	jmp	@18@170
@@4:
@18@590:
   ;	
   ;	    }
   ;	  }
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_read_messages	endp
   ;	
   ;	void read_new_messages(void)
   ;	
	assume	cs:_TEXT
_read_new_messages	proc	near
	push	bp
	mov	bp,sp
	sub	sp,340
   ;	
   ;	{
   ;	  struct mail_pass_data data;
   ;	
   ;	  lock_dos();
   ;	
	call	near ptr _lock_dos
   ;	
   ;	  open_mail_base(user_lines[tswitch].number,&data,0);
   ;	
	xor	ax,ax
	push	ax
	lea	ax,word ptr [bp-340]
	push	ax
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,384
	imul	dx
	mov	dx,seg _user_lines
	mov	bx,ax
	mov	es,dx
	push	word ptr es:_user_lines[bx]
	call	near ptr _open_mail_base
	add	sp,6
   ;	
   ;	  unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	  read_messages(&data,data.entry.new_msg_no);
   ;	
	push	word ptr [bp-226]
	push	word ptr [bp-228]
	lea	ax,word ptr [bp-340]
	push	ax
	call	near ptr _read_messages
	add	sp,6
   ;	
   ;	  lock_dos();
   ;	
	call	near ptr _lock_dos
   ;	
   ;	  close_new_base(&data);
   ;	
	lea	ax,word ptr [bp-340]
	push	ax
	call	near ptr _close_new_base
	pop	cx
   ;	
   ;	  unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_read_new_messages	endp
   ;	
   ;	void read_a_mail_message(int num)
   ;	
	assume	cs:_TEXT
_read_a_mail_message	proc	near
	push	bp
	mov	bp,sp
	sub	sp,340
   ;	
   ;	{
   ;	  struct mail_pass_data data;
   ;	
   ;	  lock_dos();
   ;	
	call	near ptr _lock_dos
   ;	
   ;	  open_mail_base(user_lines[tswitch].number,&data,0);
   ;	
	xor	ax,ax
	push	ax
	lea	ax,word ptr [bp-340]
	push	ax
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,384
	imul	dx
	mov	dx,seg _user_lines
	mov	bx,ax
	mov	es,dx
	push	word ptr es:_user_lines[bx]
	call	near ptr _open_mail_base
	add	sp,6
   ;	
   ;	  unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	  read_messages(&data,data.entry.head_entry_no -
   ;	
   ;	
   ;	        data.entry.max_mesg + num - 1);
   ;	
	mov	ax,word ptr [bp-255]
	mov	dx,word ptr [bp-257]
	sub	dx,word ptr [bp-260]
	sbb	ax,0
	push	dx
	push	ax
	mov	ax,word ptr [bp+4]
	cwd	
	pop	bx
	pop	cx
	add	cx,ax
	adc	bx,dx
	sub	cx,1
	sbb	bx,0
	push	bx
	push	cx
	lea	ax,word ptr [bp-340]
	push	ax
	call	near ptr _read_messages
	add	sp,6
   ;	
   ;	  lock_dos();
   ;	
	call	near ptr _lock_dos
   ;	
   ;	  close_new_base(&data);
   ;	
	lea	ax,word ptr [bp-340]
	push	ax
	call	near ptr _close_new_base
	pop	cx
   ;	
   ;	  unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_read_a_mail_message	endp
   ;	
   ;	void delete_mail(void)
   ;	
	assume	cs:_TEXT
_delete_mail	proc	near
	push	bp
	mov	bp,sp
	sub	sp,350
	push	si
   ;	
   ;	{
   ;	  char command[7];
   ;	  int num;
   ;	  char *point;
   ;	  struct mail_pass_data data;
   ;	
   ;	  prompt_get_string("Delete message: ",command,4);
   ;	
	mov	ax,4
	push	ax
	lea	ax,word ptr [bp-8]
	push	ax
	mov	ax,offset DGROUP:s@+702
	push	ax
	call	near ptr _prompt_get_string
	add	sp,6
   ;	
   ;	  if ((num=str_to_num(command,&point))>0)
   ;	
	lea	ax,word ptr [bp-10]
	push	ax
	lea	ax,word ptr [bp-8]
	push	ax
	call	near ptr _str_to_num
	pop	cx
	pop	cx
	mov	si,ax
	or	ax,ax
	jle	short @21@142
   ;	
   ;	  {
   ;	    lock_dos();
   ;	
	call	near ptr _lock_dos
   ;	
   ;	    open_mail_base(user_lines[tswitch].number,&data,0);
   ;	
	xor	ax,ax
	push	ax
	lea	ax,word ptr [bp-350]
	push	ax
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,384
	imul	dx
	mov	dx,seg _user_lines
	mov	bx,ax
	mov	es,dx
	push	word ptr es:_user_lines[bx]
	call	near ptr _open_mail_base
	add	sp,6
   ;	
   ;	    if (num<=data.entry.max_mesg)
   ;	
	cmp	si,word ptr [bp-270]
	ja	short @21@114
   ;	
   ;	     delete_msg_from_base(&data,data.entry.head_entry_no -
   ;	
   ;	
   ;	        data.entry.max_mesg + num - 1,0);
   ;	
	xor	ax,ax
	push	ax
	mov	ax,word ptr [bp-265]
	mov	dx,word ptr [bp-267]
	sub	dx,word ptr [bp-270]
	sbb	ax,0
	push	dx
	push	ax
	mov	ax,si
	cwd	
	pop	bx
	pop	cx
	add	cx,ax
	adc	bx,dx
	sub	cx,1
	sbb	bx,0
	push	bx
	push	cx
	lea	ax,word ptr [bp-350]
	push	ax
	call	near ptr _delete_msg_from_base
	add	sp,8
@21@114:
   ;	
   ;	    close_new_base(&data);
   ;	
	lea	ax,word ptr [bp-350]
	push	ax
	call	near ptr _close_new_base
	pop	cx
   ;	
   ;	    unlock_dos();
   ;	
	call	near ptr _unlock_dos
@21@142:
   ;	
   ;	  }
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_delete_mail	endp
   ;	
   ;	void print_str_nchr(char *string, int length)
   ;	
	assume	cs:_TEXT
_print_str_nchr	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	di,word ptr [bp+4]
	mov	si,word ptr [bp+6]
	jmp	short @22@86
@22@58:
   ;	
   ;	{
   ;	  while ((*string) && (length>0))
   ;	  {
   ;	    print_chr(*string++);
   ;	
	mov	bx,di
	inc	di
	mov	al,byte ptr [bx]
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	    length--;
   ;	
	dec	si
@22@86:
	cmp	byte ptr [di],0
	je	short @22@142
	or	si,si
	jg	short @22@58
@22@142:
	jmp	short @22@198
@22@170:
   ;	
   ;	  }
   ;	  while (length>0)
   ;	  {
   ;	    print_chr(' ');
   ;	
	mov	al,32
	push	ax
	call	near ptr _print_chr
	pop	cx
   ;	
   ;	    length--;
   ;	
	dec	si
@22@198:
	or	si,si
	jg	short @22@170
   ;	
   ;	  }
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_print_str_nchr	endp
   ;	
   ;	void list_mail(int last)
   ;	
	assume	cs:_TEXT
_list_mail	proc	near
	push	bp
	mov	bp,sp
	sub	sp,724
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  struct mail_pass_data data;
   ;	  struct mesg_entry mentry;
   ;	  struct file_entry file;
   ;	  unsigned long int cur;
   ;	  char s[40];
   ;	
   ;	  lock_dos();
   ;	
	call	near ptr _lock_dos
   ;	
   ;	  open_mail_base(user_lines[tswitch].number,&data,0);
   ;	
	xor	ax,ax
	push	ax
	lea	ax,word ptr [bp-344]
	push	ax
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,384
	imul	dx
	mov	dx,seg _user_lines
	mov	bx,ax
	mov	es,dx
	push	word ptr es:_user_lines[bx]
	call	near ptr _open_mail_base
	add	sp,6
   ;	
   ;	  if (!last) last = data.entry.max_mesg;
   ;	
	or	si,si
	jne	short @23@86
	mov	si,word ptr [bp-264]
@23@86:
   ;	
   ;	  cur = data.entry.head_entry_no;
   ;	
	mov	ax,word ptr [bp-259]
	mov	dx,word ptr [bp-261]
	mov	word ptr [bp-2],ax
	mov	word ptr [bp-4],dx
	jmp	@23@142
@23@114:
   ;	
   ;	  while ((cur>data.entry.tail_entry_no) && (last>0))
   ;	  {
   ;	    cur--;
   ;	
	sub	word ptr [bp-4],1
	sbb	word ptr [bp-2],0
   ;	
   ;	    last--;
   ;	
	dec	si
   ;	
   ;	    get_entry(&data,cur,&mentry,&file);
   ;	
	lea	ax,word ptr [bp-684]
	push	ax
	lea	ax,word ptr [bp-600]
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	lea	ax,word ptr [bp-344]
	push	ax
	call	near ptr _get_entry
	add	sp,10
   ;	
   ;	    unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	    special_code(1,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	mov	ax,1
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
   ;	
   ;	    sprintf(s,"|*h1|*f4%02ld  |*f7",1 + data.entry.max_mesg -
   ;	
   ;	
   ;	         (data.entry.head_entry_no - cur));
   ;	
	mov	ax,word ptr [bp-264]
	inc	ax
	xor	dx,dx
	mov	bx,word ptr [bp-259]
	mov	cx,word ptr [bp-261]
	sub	cx,word ptr [bp-4]
	sbb	bx,word ptr [bp-2]
	sub	ax,cx
	sbb	dx,bx
	push	dx
	push	ax
	mov	ax,offset DGROUP:s@+719
	push	ax
	lea	ax,word ptr [bp-724]
	push	ax
	call	near ptr _sprintf
	add	sp,8
   ;	
   ;	    print_string(s);
   ;	
	lea	ax,word ptr [bp-724]
	push	ax
	call	near ptr _print_string
	pop	cx
   ;	
   ;	    print_str_nchr(mentry.username,30);
   ;	
	mov	ax,30
	push	ax
	lea	ax,word ptr [bp-570]
	push	ax
	call	near ptr _print_str_nchr
	pop	cx
	pop	cx
   ;	
   ;	    print_string("|*r1  |*h1|*f4");
   ;	
	mov	ax,offset DGROUP:s@+739
	push	ax
	call	near ptr _print_string
	pop	cx
   ;	
   ;	    print_str_nchr(mentry.subject,40);
   ;	
	mov	ax,40
	push	ax
	lea	ax,word ptr [bp-448]
	push	ax
	call	near ptr _print_str_nchr
	pop	cx
	pop	cx
   ;	
   ;	    print_cr();
   ;	
	call	near ptr _print_cr
   ;	
   ;	    special_code(0,tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	xor	ax,ax
	push	ax
	call	near ptr _special_code
	pop	cx
	pop	cx
   ;	
   ;	    lock_dos();
   ;	
	call	near ptr _lock_dos
@23@142:
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	cmp	ax,word ptr [bp-255]
	jb	short @23@254
	ja	short @23@226
	cmp	dx,word ptr [bp-257]
	jbe	short @23@254
@23@226:
	or	si,si
	jle	@@5
	jmp	@23@114
@@5:
@23@254:
   ;	
   ;	  }
   ;	  close_new_base(&data);
   ;	
	lea	ax,word ptr [bp-344]
	push	ax
	call	near ptr _close_new_base
	pop	cx
   ;	
   ;	  unlock_dos();
   ;	
	call	near ptr _unlock_dos
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_list_mail	endp
   ;	
   ;	void send_mail(void)
   ;	
	assume	cs:_TEXT
_send_mail	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	}
   ;	
	pop	bp
	ret	
_send_mail	endp
   ;	
   ;	void user_feedback(char *string, char *name, int portnum)
   ;	
	assume	cs:_TEXT
_user_feedback	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	}
   ;	
	pop	bp
	ret	
_user_feedback	endp
   ;	
   ;	void is_new_mail(void)
   ;	
	assume	cs:_TEXT
_is_new_mail	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	}
   ;	
	pop	bp
	ret	
_is_new_mail	endp
   ;	
   ;	void mail_system(const char *str,const char *name, int portnum)
   ;	
	assume	cs:_TEXT
_mail_system	proc	near
	push	bp
	mov	bp,sp
	sub	sp,14
	push	si
   ;	
   ;	{
   ;	  int flag = 1;
   ;	
	mov	si,1
   ;	
   ;	  char command[7];
   ;	  int num;
   ;	  char *point;
   ;	
   ;	  list_mail(2);
   ;	
	mov	ax,2
	push	ax
	call	near ptr _list_mail
	pop	cx
	jmp	@27@590
@27@58:
   ;	
   ;	
   ;	  while (flag)
   ;	  {
   ;	    prompt_get_string("Mail Command (|*h1?|*h0 for Menu): ",command,4);
   ;	
	mov	ax,4
	push	ax
	lea	ax,word ptr [bp-8]
	push	ax
	mov	ax,offset DGROUP:s@+754
	push	ax
	call	near ptr _prompt_get_string
	add	sp,6
   ;	
   ;	    if ((num=str_to_num(command,&point))>0) read_a_mail_message(num);
   ;	
	lea	ax,word ptr [bp-12]
	push	ax
	lea	ax,word ptr [bp-8]
	push	ax
	call	near ptr _str_to_num
	pop	cx
	pop	cx
	mov	word ptr [bp-10],ax
	or	ax,ax
	jle	short @27@114
	push	word ptr [bp-10]
	call	near ptr _read_a_mail_message
	pop	cx
	jmp	short @27@590
@27@114:
   ;	
   ;	    else
   ;	    {
   ;	      if (*command>'Z') *command -= ' ';
   ;	
	cmp	byte ptr [bp-8],90
	jle	short @27@170
	mov	al,byte ptr [bp-8]
	add	al,224
	mov	byte ptr [bp-8],al
@27@170:
   ;	
   ;	      switch (*command)
   ;	
	mov	al,byte ptr [bp-8]
	cbw	
	mov	word ptr [bp-14],ax
	mov	cx,8
	mov	bx,offset @27@C546
@27@226:
	mov	ax,word ptr cs:[bx]
	cmp	ax,word ptr [bp-14]
	je	short @27@310
	add	bx,2
	loop	short @27@226
	jmp	short @27@590
@27@310:
	jmp	word ptr cs:[bx+16]
@27@338:
   ;	
   ;	      {
   ;	        case 'R': /* read_mail_message(); */
   ;	                  break;
   ;	
	jmp	short @27@590
@27@366:
   ;	
   ;	        case 'X': delete_mail();
   ;	
	call	near ptr _delete_mail
   ;	
   ;	                  break;
   ;	
	jmp	short @27@590
@27@394:
   ;	
   ;	        case 'Q': flag = 0;
   ;	
	xor	si,si
   ;	
   ;	                  break;
   ;	
	jmp	short @27@590
@27@422:
   ;	
   ;	        case 'S': send_mail_message();
   ;	
	call	near ptr _send_mail_message
   ;	
   ;	                  break;
   ;	
	jmp	short @27@590
@27@450:
   ;	
   ;	        case 'N': read_new_messages();
   ;	
	call	near ptr _read_new_messages
   ;	
   ;	                  break;
   ;	
	jmp	short @27@590
@27@478:
   ;	
   ;	        case 'D': examine_board();
   ;	
	call	near ptr _examine_board
   ;	
   ;	                  break;
   ;	
	jmp	short @27@590
@27@506:
   ;	
   ;	        case 'L': list_mail(0);
   ;	
	xor	ax,ax
	push	ax
	call	near ptr _list_mail
	jmp	short @27@562
@27@534:
   ;	
   ;	                  break;
   ;	        case '?': print_file("help\\mail.hlp");
   ;	
	mov	ax,offset DGROUP:s@+790
	push	ax
	call	near ptr _print_file
@27@562:
	pop	cx
   ;	
   ;	                  break;
   ;	
	jmp	short @27@590
@27@590:
	or	si,si
	je	@@6
	jmp	@27@58
@@6:
   ;	
   ;	      }
   ;	    }
   ;	  }
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_mail_system	endp
@27@C546	label	word
	db	63
	db	0
	db	68
	db	0
	db	76
	db	0
	db	78
	db	0
	db	81
	db	0
	db	82
	db	0
	db	83
	db	0
	db	88
	db	0
	dw	@27@534
	dw	@27@478
	dw	@27@506
	dw	@27@450
	dw	@27@394
	dw	@27@338
	dw	@27@422
	dw	@27@366
	?debug	C E9
	?debug	C FA00000000
_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'%s\basedata'
	db	0
	db	'rb+'
	db	0
	db	'LOADBASE'
	db	0
	db	'wb+'
	db	0
	db	'CREBASE'
	db	0
	db	'Mailbox #%03d'
	db	0
	db	'blank'
	db	0
	db	'MAIL\MAIL%03d'
	db	0
	db	'BBS\BBS%02d'
	db	0
	db	'TEMP%02d%01d'
	db	0
	db	'rb+'
	db	0
	db	'EDITORFL'
	db	0
	db	'wb+'
	db	0
	db	'EDITORFL'
	db	0
	db	'%02X%04X'
	db	0
	db	'%s\%s'
	db	0
	db	'rb'
	db	0
	db	'INCOPY'
	db	0
	db	'a'
	db	0
	db	'wb'
	db	0
	db	'OUTCOPY'
	db	0
	db	'%s\%s'
	db	0
	db	'wb+'
	db	0
	db	'ADDMSG'
	db	0
	db	'%s\%s'
	db	0
	db	'%s\%s'
	db	0
	db	'rb'
	db	0
	db	'GETBBS'
	db	0
	db	'%s\%s'
	db	0
	db	'rb'
	db	0
	db	'READBBS'
	db	0
	db	'Which user number to read: '
	db	0
	db	'Basename: %s'
	db	0
	db	'Mail:     %d'
	db	0
	db	'Max mesg: %d'
	db	0
	db	'Priority: %d'
	db	0
	db	'Head/Tail: %ld %ld %ld'
	db	0
	db	'Name:     %d'
	db	0
	db	'|*h1|*f4From:     |*f7'
	db	0
	db	'(%%GST)'
	db	0
	db	'(#%03d)'
	db	0
	db	' %s|*r1'
	db	13
	db	10
	db	0
	db	'|*h1|*f4System:   |*f7( %03d) %s|*r1'
	db	13
	db	10
	db	0
	db	'|*h1|*f4Subject:  |*f7%s|*r1'
	db	13
	db	10
	db	0
	db	'%m/%d/%y %I:%M:%S %p'
	db	0
	db	'|*h1|*f4Date:     |*f7%s [%lu]|*r1'
	db	13
	db	10
	db	0
	db	'|*h1|*f4Contents:|*r1'
	db	13
	db	10
	db	13
	db	10
	db	0
	db	'That user does NOT EXIST'
	db	0
	db	'|*f4|*h1     To: |*f7(#%03d) %c|*r1%s|*r1|*f7|*h1%c '
	db	0
	db	'Subject: '
	db	0
	db	'Which user number to send to: '
	db	0
	db	'|*h1|*f4Message   |*f7%02ld |*f4of |*f7%02d|*r1'
	db	0
	db	'Delete message: '
	db	0
	db	'|*h1|*f4%02ld  |*f7'
	db	0
	db	'|*r1  |*h1|*f4'
	db	0
	db	'Mail Command (|*h1?|*h0 for Menu): '
	db	0
	db	'help\mail.hlp'
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	public	_send_mail
	public	_list_mail
	public	_print_str_nchr
	public	_delete_mail
	public	_read_a_mail_message
	public	_read_new_messages
	public	_read_messages
	public	_send_mail_message
	public	_send_mail_subj
	public	_send_mail_to
	public	_examine_board
	public	_read_entry
	public	_get_entry
	public	_delete_msg_from_base
	public	_add_msg_to_base
	extrn	N_LXLSH@:far
	extrn	N_LUMOD@:far
	public	_scan_for_next_message
	public	_copy_a_file
	public	_generate_id
	public	_create_temp_file
	public	_open_bbs_base
	public	_open_mail_base
	public	_close_new_base
	public	_load_new_base
	public	_strcpy_n
	public	_user_feedback
	public	_mail_system
	public	_is_new_mail
	extrn	_line_editor:near
	extrn	_get_editor_string:near
	extrn	_str_to_num:near
	extrn	_user_lines:word
	extrn	_load_user:near
	extrn	_sys_info:word
	extrn	_print_file_to_cntrl:near
	extrn	_get_string_cntrl:near
	extrn	_print_file:near
	extrn	_prompt_get_string:near
	extrn	_special_code:near
	extrn	_print_cr:near
	extrn	_print_str_cr:near
	extrn	_print_string:near
	extrn	_print_chr:near
	extrn	_g_fclose:near
	extrn	_g_fopen:near
	extrn	_unlock_dos:near
	extrn	_lock_dos:near
	extrn	_next_task:far
	extrn	_dans_counter:word
	extrn	_tswitch:word
	extrn	_mkdir:near
	extrn	_findfirst:near
	extrn	_strftime:near
	extrn	_time:near
	extrn	_localtime:near
	extrn	_strcpy:near
	extrn	_delay:near
	extrn	_sprintf:near
	extrn	_remove:near
	extrn	_fwrite:near
	extrn	_fseek:near
	extrn	_fread:near
	extrn	_fprintf:near
_s@	equ	s@
	end
