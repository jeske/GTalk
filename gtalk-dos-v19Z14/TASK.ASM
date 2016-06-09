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
	?debug	S "task.c"
	?debug	C E9B6B0EC1A067461736B2E63
	?debug	C E95A7F8C1A09696E636C7564652E68
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
	?debug	C E9260B931A076774616C6B2E68
	?debug	C E9ECA8951A067461736B2E68
	?debug	C E9E60A931A06696E69742E68
	?debug	C E9F0A58A1A07616C6C6F632E68
	?debug	C E9259AE11A05636F6D2E68
	?debug	C E92327E91A07766964656F2E68
	?debug	C E90EA6951A096368616E6E656C2E68
	?debug	C E943A0E51A04696F2E68
	?debug	C E93D1AE21A0467742E68
	?debug	C E979A6951A06757365722E68
	?debug	C E9438B9B1A09636F6D6D616E642E68
	?debug	C E9C3793B1A066C696E6B2E68
	?debug	C E9FB9D921A07676C696E6B2E68
	?debug	C E9C2793B1A09737472756374732E68
	?debug	C E944A7951A08737472696E672E68
	?debug	C E94A07E91A0770726976732E68
	?debug	C E90AB9931A09746F67676C65732E68
	?debug	C E9C4793B1A07696E7075742E68
	?debug	C E91B82891A08656469746F722E68
	?debug	C E95A3DE91A066D61696C2E68
	?debug	C E97AA69A1A09726F7461746F722E68
	?debug	C E9C5793B1A086766696C65732E68
	?debug	C E994913C1A0A7363686564756C652E68
	?debug	C E9C2793B1A077379736F702E68
	?debug	C E9169AE71A056262732E68
	?debug	C E98D8B931A0764696167732E68
	?debug	C E9C1793B1A0A70726F746F636F6C2E68
	?debug	C E9C388E11A08646566696E652E68
	?debug	C E9732BE71A0A66756E6374696F6E2E68
TASK_TEXT	segment byte public 'CODE'
TASK_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:TASK_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_DATA	segment word public 'DATA'
_emm_page_mapping	label	word
	db	0
	db	0
	db	0
	db	0
	db	1
	db	0
	db	1
	db	0
	db	2
	db	0
	db	2
	db	0
	db	3
	db	0
	db	3
	db	0
_tasking	label	byte
	db	0
_timer_section	label	byte
	db	0
_num_task_switches	label	word
	db	0
	db	0
	db	0
	db	0
_max_task_switches	label	word
	db	1
	db	0
	db	0
	db	0
_system_load	label	word
	db	0
	db	0
	db	0
	db	0
_DATA	ends
TASK_TEXT	segment byte public 'CODE'
   ;	
   ;	int our_task_id(void)           /* returns the id of our task */
   ;	
	assume	cs:TASK_TEXT
_our_task_id	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{                               /* (this is a technicality, it really */
   ;	  return(tswitch);              /* is "tswitch" */
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	jmp	short @1@58
@1@58:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_our_task_id	endp
   ;	
   ;	void end_task(void)             /* this ends the task that calls this */
   ;	
	assume	cs:TASK_TEXT
_end_task	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  kill_task(tswitch);
   ;	
	push	word ptr DGROUP:_tswitch
	call	far ptr _kill_task
	pop	cx
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_end_task	endp
   ;	
   ;	void kill_task(int id)          /* this kills a task */
   ;	
	assume	cs:TASK_TEXT
_kill_task	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+6]
   ;	
   ;	{
   ;	  int loop;
   ;	#ifdef DEBUG
   ;	  char s[80];
   ;	#endif
   ;	
   ;	
   ;	  free_semaphores(id);
   ;	
	push	si
	call	far ptr _free_semaphores
	pop	cx
   ;	
   ;	  g_free_all_handles(id);
   ;	
	push	si
	call	far ptr _g_free_all_handles
	pop	cx
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	
   ;	  if (id==tswitch)
   ;	
	cmp	si,word ptr DGROUP:_tswitch
	jne	short @3@86
   ;	
   ;	    unlock_dos();
   ;	
	call	far ptr _unlock_dos
@3@86:
   ;	
   ;	
   ;	  if (tasks[id].status)         /* kill only if we're alive */
   ;	
	mov	ax,si
	mov	dx,35
	imul	dx
	mov	bx,ax
	cmp	word ptr DGROUP:_tasks[bx],0
	je	short @3@170
   ;	
   ;	   {
   ;	    tasks[id].status = DEAD;    /* mark status as DEAD */
   ;	
	mov	ax,si
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx],0
   ;	
   ;	    tasks[id].paused = 0;       /* mark task as NOT paused */
   ;	
	mov	ax,si
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+2],0
   ;	
   ;	    tasks[id].who_paused_me=-1;
   ;	
	mov	ax,si
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+4],-1
   ;	
   ;	#ifdef DEBUG
   ;	    sprintf(s,"Free memory %p",tasks[id].stck);
   ;	    direct_screen(2,0,0x17,s);
   ;	    sprintf(s,"%p",*((char *)tasks[id].stck-4));
   ;	    direct_screen(2,40,0x17,s);
   ;	#endif
   ;	    g_free_from_who(tasks[id].stck,id);    /* free our stack memory */
   ;	
	push	si
	mov	ax,si
	mov	dx,35
	imul	dx
	mov	bx,ax
	push	word ptr DGROUP:_tasks[bx+13]
	push	word ptr DGROUP:_tasks[bx+11]
	call	far ptr _g_free_from_who
	add	sp,6
   ;	
   ;	    numTasksOpen--;             /* let tasker know there's one less task */
   ;	
	dec	word ptr DGROUP:_numTasksOpen
   ;	
   ;	    if (id == tswitch) switchTasks = 1;  /* if the current task is dead, */
   ;	
	cmp	si,word ptr DGROUP:_tswitch
	jne	short @3@170
	mov	word ptr DGROUP:_switchTasks,1
@3@170:
   ;	
   ;	   };                                    /* make sure we switch */
   ;	   /* ALSO: make sure any tasks we paused get restarted */
   ;	
   ;	   for (loop=0;loop<MAX_THREADS;loop++)
   ;	
	xor	di,di
	jmp	short @3@282
@3@198:
   ;	
   ;	     if (who_paused(loop)==id)
   ;	
	push	di
	call	far ptr _who_paused
	pop	cx
	cmp	ax,si
	jne	short @3@254
   ;	
   ;	             unpause(loop);
   ;	
	push	di
	call	far ptr _unpause
	pop	cx
@3@254:
	inc	di
@3@282:
	cmp	di,26
	jl	short @3@198
   ;	
   ;	
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	  next_task();                  /* just in case we're the dead task */
   ;	
	pushf	
	call	far ptr _next_task
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_kill_task	endp
   ;	
   ;	void switch_to_ems_context(int portnum)
   ;	
	assume	cs:TASK_TEXT
_switch_to_ems_context	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
   ;	
   ;	{
   ;	  struct task_struct near *cur_task_struct = &tasks[portnum];
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,35
	imul	dx
	add	ax,offset DGROUP:_tasks
	mov	di,ax
   ;	
   ;	
   ;	  if (cur_task_struct->is_ems)
   ;	
	cmp	byte ptr [di+15],0
	je	short @4@86
   ;	
   ;	  {
   ;	    _DX = cur_task_struct->ems_handle;
   ;	
	mov	dx,word ptr [di+16]
   ;	
   ;	    _CX = cur_task_struct->mapped_pages;
   ;	
	mov	al,byte ptr [di+18]
	mov	ah,0
	mov	cx,ax
   ;	
   ;	    _SI = (unsigned int) emm_page_mapping;
   ;	
	mov	si,offset DGROUP:_emm_page_mapping
   ;	
   ;	    _AX = 0x5000;
   ;	
	mov	ax,20480
   ;	
   ;	    geninterrupt(0x67);
   ;	
	int	103
@4@86:
   ;	
   ;	  }
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_switch_to_ems_context	endp
   ;	
   ;	void _saveregs flip_ems_page(void)
   ;	
	assume	cs:TASK_TEXT
_flip_ems_page	proc	far
	push	ax
	push	cx
	push	dx
	push	bx
	push	sp
	push	bp
	push	si
	push	di
	push	ds
	push	es
	mov	bp,sp
   ;	
   ;	{
   ;	    _DX = task_fast[tswitch]->ems_handle;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	dx,word ptr [bx+16]
   ;	
   ;	    _CX = task_fast[tswitch]->mapped_pages;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	al,byte ptr [bx+18]
	mov	ah,0
	mov	cx,ax
   ;	
   ;	    _SI = (unsigned int) (emm_page_mapping);
   ;	
	mov	si,offset DGROUP:_emm_page_mapping
   ;	
   ;	    _AX = 0x5000;
   ;	
	mov	ax,20480
   ;	
   ;	    geninterrupt(0x67);
   ;	
	int	103
   ;	
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	es
	pop	ds
	pop	di
	pop	si
	pop	bp
	pop	bx
	pop	bx
	pop	dx
	pop	cx
	pop	ax
	ret	
_flip_ems_page	endp
   ;	
   ;	void _saveregs flip_a_ems_page(struct task_struct *task_ptr)
   ;	
	assume	cs:TASK_TEXT
_flip_a_ems_page	proc	far
	push	ax
	push	cx
	push	dx
	push	bx
	push	sp
	push	bp
	push	si
	push	di
	push	ds
	push	es
	mov	bp,sp
   ;	
   ;	{
   ;	
   ;	    _DX = task_ptr->ems_handle;
   ;	
	les	bx,dword ptr [bp+24]
	mov	dx,word ptr es:[bx+16]
   ;	
   ;	    _CX = task_ptr->mapped_pages;
   ;	
	les	bx,dword ptr [bp+24]
	mov	al,byte ptr es:[bx+18]
	mov	ah,0
	mov	cx,ax
   ;	
   ;	    _SI = (unsigned int) (emm_page_mapping);
   ;	
	mov	si,offset DGROUP:_emm_page_mapping
   ;	
   ;	    _AX = 0x5000;
   ;	
	mov	ax,20480
   ;	
   ;	    geninterrupt(0x67);
   ;	
	int	103
   ;	
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	es
	pop	ds
	pop	di
	pop	si
	pop	bp
	pop	bx
	pop	bx
	pop	dx
	pop	cx
	pop	ax
	ret	
_flip_a_ems_page	endp
   ;	
   ;	void interrupt next_task(void)
   ;	
	assume	cs:TASK_TEXT
_next_task	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	  if ((tasking) && (!switchTasks))
   ;	
	cmp	byte ptr DGROUP:_tasking,0
	je	short @7@114
	cmp	word ptr DGROUP:_switchTasks,0
	jne	short @7@114
   ;	
   ;	    return;  /* if dos is locked, just return */
   ;	
	jmp	@7@562
@7@114:
   ;	
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	
   ;	  old_tswitch = tswitch;
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	word ptr DGROUP:_old_tswitch,ax
   ;	
   ;	
   ;	  task_fast[tswitch]->ss = _SS;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	word ptr [bx+8],ss
   ;	
   ;	  task_fast[tswitch]->sp = _SP;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	word ptr [bx+6],sp
   ;	
   ;	
   ;	  _SS = oldss;
   ;	
	mov	ss,word ptr DGROUP:_oldss
   ;	
   ;	  _SP = oldsp;
   ;	
	mov	sp,word ptr DGROUP:_oldsp
   ;	
   ;	
   ;	  {
   ;	    struct task_struct near *cur_task_struct;  /* ptr for quick access */
   ;	    int ttemp;                        /* local variable for quick access */
   ;	
   ;	    ttemp = tswitch;
   ;	
	mov	dx,word ptr DGROUP:_tswitch
   ;	
   ;	    cur_task_struct = (struct task_struct near *) task_fast[ttemp];
   ;	
	mov	bx,dx
	shl	bx,1
	mov	si,word ptr DGROUP:_task_fast[bx]
   ;	
   ;	    dontKeepNextTask = 0;
   ;	
	mov	word ptr DGROUP:_dontKeepNextTask,0
   ;	
   ;	    if (!numTasksOpen)            /* if no more tasks, leave multitasking */
   ;	
	cmp	word ptr DGROUP:_numTasksOpen,0
	jne	short @7@198
   ;	
   ;	     {
   ;	      switchTasks = 0;            /* don't switch task */
   ;	
	mov	word ptr DGROUP:_switchTasks,0
   ;	
   ;	      tasking = 0;                /* turn multitasking off */
   ;	
	mov	byte ptr DGROUP:_tasking,0
@7@198:
   ;	
   ;	     }
   ;	    if (switchTasks)              /* if we are going to switch tasks */
   ;	
	cmp	word ptr DGROUP:_switchTasks,0
	je	short @7@366
@7@226:
   ;	
   ;	     {
   ;	      do
   ;	        {
   ;	          ttemp++;                /* look for an alive task */
   ;	
	inc	dx
   ;	
   ;	          cur_task_struct++;
   ;	
	add	si,35
   ;	
   ;	          if (cur_task_struct == end_task_struct)
   ;	
	cmp	si,word ptr DGROUP:_end_task_struct
	jne	short @7@282
   ;	
   ;	          {
   ;	            ttemp = 0;
   ;	
	xor	dx,dx
   ;	
   ;	            cur_task_struct = begin_task_struct;
   ;	
	mov	si,word ptr DGROUP:_begin_task_struct
@7@282:
   ;	
   ;	          }
   ;	        } while (((!cur_task_struct->status) || (cur_task_struct->paused)));
   ;	
	cmp	word ptr [si],0
	je	short @7@226
	cmp	word ptr [si+2],0
	jne	short @7@226
   ;	
   ;	
   ;	      num_task_switches++;
   ;	
	add	word ptr DGROUP:_num_task_switches,1
	adc	word ptr DGROUP:_num_task_switches+2,0
   ;	
   ;	      tswitch = ttemp;
   ;	
	mov	word ptr DGROUP:_tswitch,dx
@7@366:
   ;	
   ;	   }
   ;	  }
   ;	
   ;	  if (!tasking)
   ;	
	mov	al,byte ptr DGROUP:_tasking
	cbw	
	or	ax,ax
	jne	short @7@422
   ;	
   ;	   {
   ;	     disable();                 /* set up stack that called our stack */
   ;	
	db	250
   ;	
   ;	/*   _SS = oldss;
   ;	     _SP = oldsp;  */
   ;	     setvect(8, old_int8);      /* reset timer tick */
   ;	
	push	word ptr DGROUP:_old_int8+2
	push	word ptr DGROUP:_old_int8
	mov	ax,8
	push	ax
	call	far ptr _setvect
	add	sp,6
	jmp	short @7@534
@7@422:
   ;	
   ;	     enable();
   ;	     return;
   ;	   }
   ;	
   ;	  if ((task_fast[tswitch]->is_ems) && (tswitch != old_tswitch))
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	cmp	byte ptr [bx+15],0
	je	short @7@506
	mov	ax,word ptr DGROUP:_tswitch
	cmp	ax,word ptr DGROUP:_old_tswitch
	je	short @7@506
   ;	
   ;	    flip_ems_page();
   ;	
	push	cs
	call	near ptr _flip_ems_page
@7@506:
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	
   ;	  _SS = task_fast[tswitch]->ss;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	ss,word ptr [bx+8]
   ;	
   ;	  _SP = task_fast[tswitch]->sp;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	sp,word ptr [bx+6]
@7@534:
   ;	
   ;	
   ;	  enable();   /* on leaving this routine, the IRET and POP instructions */
   ;	
	db	251
@7@562:
   ;	
   ;	};            /* will take all of the new registers off the stack, including */
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_next_task	endp
   ;	
   ;	void interrupt dv_int8_task_switch(void)
   ;	
	assume	cs:TASK_TEXT
_dv_int8_task_switch	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	
   ;	  (*old_int8)();    /* call old int8 function */
   ;	
	pushf	
	call	dword ptr DGROUP:_old_int8
   ;	
   ;	  dans_counter++;    /* increment counter used for 1/18 second timing */
   ;	
	inc	word ptr DGROUP:_dans_counter
   ;	
   ;	  timer_section++;   /* increment counter for system_load */
   ;	
	inc	byte ptr DGROUP:_timer_section
   ;	
   ;	
   ;	
   ;	  if ((!tasking) || (!numTasksOpen))
   ;	
	mov	al,byte ptr DGROUP:_tasking
	cbw	
	or	ax,ax
	je	short @8@86
	cmp	word ptr DGROUP:_numTasksOpen,0
	jne	short @8@114
@8@86:
   ;	
   ;	   {
   ;	     disable();
   ;	
	db	250
   ;	
   ;	     /* switch to stack of old task */
   ;	     _SS = oldss;
   ;	
	mov	ss,word ptr DGROUP:_oldss
   ;	
   ;	     _SP = oldsp;
   ;	
	mov	sp,word ptr DGROUP:_oldsp
   ;	
   ;	     setvect(8, old_int8);  /* reset timer tick */
   ;	
	push	word ptr DGROUP:_old_int8+2
	push	word ptr DGROUP:_old_int8
	mov	ax,8
	push	ax
	call	far ptr _setvect
	add	sp,6
   ;	
   ;	     enable();
   ;	
	db	251
   ;	
   ;	     return;
   ;	
	jmp	@8@394
@8@114:
   ;	
   ;	   }
   ;	
   ;	   if (timer_section==MAX_TIMER)
   ;	
	cmp	byte ptr DGROUP:_timer_section,18
	je	@@0
	jmp	@8@366
@@0:
   ;	
   ;	    {
   ;	      if (num_task_switches>max_task_switches)
   ;	
	mov	ax,word ptr DGROUP:_num_task_switches+2
	mov	dx,word ptr DGROUP:_num_task_switches
	cmp	ax,word ptr DGROUP:_max_task_switches+2
	jb	short @8@254
	ja	short @8@226
	cmp	dx,word ptr DGROUP:_max_task_switches
	jbe	short @8@254
@8@226:
   ;	
   ;	          max_task_switches=num_task_switches;
   ;	
	mov	ax,word ptr DGROUP:_num_task_switches+2
	mov	dx,word ptr DGROUP:_num_task_switches
	mov	word ptr DGROUP:_max_task_switches+2,ax
	mov	word ptr DGROUP:_max_task_switches,dx
@8@254:
   ;	
   ;	      if (system_load)
   ;	
	mov	ax,word ptr DGROUP:_system_load
	or	ax,word ptr DGROUP:_system_load+2
	je	short @8@310
   ;	
   ;	      system_load= (unsigned long int) ((((unsigned long int)system_load*7l) +
   ;	
   ;	
   ;	                         (unsigned long int)(num_task_switches))>>3);
   ;	
	mov	cx,word ptr DGROUP:_system_load+2
	mov	bx,word ptr DGROUP:_system_load
	xor	dx,dx
	mov	ax,7
	call	far ptr F_LXMUL@
	add	ax,word ptr DGROUP:_num_task_switches
	adc	dx,word ptr DGROUP:_num_task_switches+2
	mov	cl,3
	call	far ptr F_LXURSH@
	mov	word ptr DGROUP:_system_load+2,dx
	mov	word ptr DGROUP:_system_load,ax
	jmp	short @8@338
@8@310:
   ;	
   ;	      else
   ;	        system_load= (num_task_switches);
   ;	
	mov	ax,word ptr DGROUP:_num_task_switches+2
	mov	dx,word ptr DGROUP:_num_task_switches
	mov	word ptr DGROUP:_system_load+2,ax
	mov	word ptr DGROUP:_system_load,dx
@8@338:
   ;	
   ;	      timer_section=0;
   ;	
	mov	byte ptr DGROUP:_timer_section,0
   ;	
   ;	      num_task_switches=0;
   ;	
	mov	word ptr DGROUP:_num_task_switches+2,0
	mov	word ptr DGROUP:_num_task_switches,0
@8@366:
   ;	
   ;	
   ;	    }
   ;	
   ;	   return;
   ;	
	jmp	short @8@394
@8@394:
   ;	
   ;	}
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_dv_int8_task_switch	endp
   ;	
   ;	void interrupt int8_task_switch(void)
   ;	
	assume	cs:TASK_TEXT
_int8_task_switch	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	
   ;	  (*old_int8)();    /* call old int8 function */
   ;	
	pushf	
	call	dword ptr DGROUP:_old_int8
   ;	
   ;	  dans_counter++;    /* increment counter used for 1/18 second timing */
   ;	
	inc	word ptr DGROUP:_dans_counter
   ;	
   ;	  timer_section++;   /* increment counter for system_load */
   ;	
	inc	byte ptr DGROUP:_timer_section
   ;	
   ;	
   ;	
   ;	   if (timer_section==MAX_TIMER)
   ;	
	cmp	byte ptr DGROUP:_timer_section,18
	je	@@1
	jmp	@9@282
@@1:
   ;	
   ;	    {
   ;	      if (num_task_switches>max_task_switches)
   ;	
	mov	ax,word ptr DGROUP:_num_task_switches+2
	mov	dx,word ptr DGROUP:_num_task_switches
	cmp	ax,word ptr DGROUP:_max_task_switches+2
	jb	short @9@170
	ja	short @9@142
	cmp	dx,word ptr DGROUP:_max_task_switches
	jbe	short @9@170
@9@142:
   ;	
   ;	          max_task_switches=num_task_switches;
   ;	
	mov	ax,word ptr DGROUP:_num_task_switches+2
	mov	dx,word ptr DGROUP:_num_task_switches
	mov	word ptr DGROUP:_max_task_switches+2,ax
	mov	word ptr DGROUP:_max_task_switches,dx
@9@170:
   ;	
   ;	
   ;	      if (system_load)
   ;	
	mov	ax,word ptr DGROUP:_system_load
	or	ax,word ptr DGROUP:_system_load+2
	je	short @9@226
   ;	
   ;	      system_load= (unsigned long int) ((((unsigned long int)system_load*7l) +
   ;	
   ;	
   ;	                         (num_task_switches))>>3);
   ;	
	mov	cx,word ptr DGROUP:_system_load+2
	mov	bx,word ptr DGROUP:_system_load
	xor	dx,dx
	mov	ax,7
	call	far ptr F_LXMUL@
	add	ax,word ptr DGROUP:_num_task_switches
	adc	dx,word ptr DGROUP:_num_task_switches+2
	mov	cl,3
	call	far ptr F_LXURSH@
	mov	word ptr DGROUP:_system_load+2,dx
	mov	word ptr DGROUP:_system_load,ax
	jmp	short @9@254
@9@226:
   ;	
   ;	      else
   ;	        system_load= (num_task_switches);
   ;	
	mov	ax,word ptr DGROUP:_num_task_switches+2
	mov	dx,word ptr DGROUP:_num_task_switches
	mov	word ptr DGROUP:_system_load+2,ax
	mov	word ptr DGROUP:_system_load,dx
@9@254:
   ;	
   ;	
   ;	      timer_section=0;
   ;	
	mov	byte ptr DGROUP:_timer_section,0
   ;	
   ;	      num_task_switches=0;
   ;	
	mov	word ptr DGROUP:_num_task_switches+2,0
	mov	word ptr DGROUP:_num_task_switches,0
@9@282:
   ;	
   ;	    }
   ;	
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	  old_tswitch = tswitch;
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	word ptr DGROUP:_old_tswitch,ax
   ;	
   ;	
   ;	  if ((!tasking) || (!numTasksOpen))
   ;	
	mov	al,byte ptr DGROUP:_tasking
	cbw	
	or	ax,ax
	je	short @9@338
	cmp	word ptr DGROUP:_numTasksOpen,0
	jne	short @9@366
@9@338:
   ;	
   ;	   {
   ;	     disable();
   ;	
	db	250
   ;	
   ;	     /* switch to stack of old task */
   ;	     _SS = oldss;
   ;	
	mov	ss,word ptr DGROUP:_oldss
   ;	
   ;	     _SP = oldsp;
   ;	
	mov	sp,word ptr DGROUP:_oldsp
   ;	
   ;	     setvect(8, old_int8);  /* reset timer tick */
   ;	
	push	word ptr DGROUP:_old_int8+2
	push	word ptr DGROUP:_old_int8
	mov	ax,8
	push	ax
	call	far ptr _setvect
	add	sp,6
	jmp	@9@786
@9@366:
   ;	
   ;	     enable();
   ;	     return;
   ;	   }
   ;	
   ;	  if (!dontKeepNextTask)
   ;	
	cmp	word ptr DGROUP:_dontKeepNextTask,0
	jne	short @9@422
   ;	
   ;	  {
   ;	     dontKeepNextTask = 1;
   ;	
	mov	word ptr DGROUP:_dontKeepNextTask,1
   ;	
   ;	     enable();
   ;	
	jmp	@9@786
@9@422:
   ;	
   ;	     return;
   ;	  }
   ;	                /* if we kept the current task this tick, */
   ;	                /* make sure we change next tick */
   ;	
   ;	  if (!switchTasks)
   ;	
	cmp	word ptr DGROUP:_switchTasks,0
	jne	short @9@478
	jmp	@9@786
@9@478:
   ;	
   ;	  {
   ;	     enable();
   ;	     return;
   ;	  }
   ;	
   ;	  task_fast[tswitch]->ss = _SS;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	word ptr [bx+8],ss
   ;	
   ;	  task_fast[tswitch]->sp = _SP;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	word ptr [bx+6],sp
   ;	
   ;	
   ;	  _SS = oldss;
   ;	
	mov	ss,word ptr DGROUP:_oldss
   ;	
   ;	  _SP = oldsp;
   ;	
	mov	sp,word ptr DGROUP:_oldsp
   ;	
   ;	
   ;	
   ;	  {
   ;	    struct task_struct near *cur_task_struct;  /* local variables for fast */
   ;	    int ttemp;                                 /* access */
   ;	
   ;	    ttemp = tswitch;
   ;	
	mov	dx,word ptr DGROUP:_tswitch
   ;	
   ;	    cur_task_struct = (struct task_struct near *) task_fast[ttemp];
   ;	
	mov	bx,dx
	shl	bx,1
	mov	si,word ptr DGROUP:_task_fast[bx]
@9@534:
   ;	
   ;	
   ;	    do
   ;	      {
   ;	
   ;	        ttemp++;                /* look for a task that's alive */
   ;	
	inc	dx
   ;	
   ;	        cur_task_struct++;
   ;	
	add	si,35
   ;	
   ;	
   ;	       if (cur_task_struct == end_task_struct)
   ;	
	cmp	si,word ptr DGROUP:_end_task_struct
	jne	short @9@590
   ;	
   ;	        {
   ;	          ttemp = 0;
   ;	
	xor	dx,dx
   ;	
   ;	          cur_task_struct = begin_task_struct;
   ;	
	mov	si,word ptr DGROUP:_begin_task_struct
@9@590:
   ;	
   ;	
   ;	        }
   ;	      } while (((!cur_task_struct->status) || (cur_task_struct->paused)));
   ;	
	cmp	word ptr [si],0
	je	short @9@534
	cmp	word ptr [si+2],0
	jne	short @9@534
   ;	
   ;	
   ;	      tswitch = ttemp;
   ;	
	mov	word ptr DGROUP:_tswitch,dx
   ;	
   ;	  }
   ;	
   ;	  if ((task_fast[tswitch]->is_ems) && (tswitch != old_tswitch))
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	cmp	byte ptr [bx+15],0
	je	short @9@758
	mov	ax,word ptr DGROUP:_tswitch
	cmp	ax,word ptr DGROUP:_old_tswitch
	je	short @9@758
   ;	
   ;	     flip_ems_page();
   ;	
	push	cs
	call	near ptr _flip_ems_page
@9@758:
   ;	
   ;	  disable();
   ;	
	db	250
   ;	
   ;	
   ;	  _SS = task_fast[tswitch]->ss;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	ss,word ptr [bx+8]
   ;	
   ;	  _SP = task_fast[tswitch]->sp;
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	mov	sp,word ptr [bx+6]
@9@786:
   ;	
   ;	
   ;	  // SEE NOTES IN VERSION.TXT BEFORE CHANGING THIS
   ;	  //  outp(0x20,0x20);
   ;	  enable();
   ;	
	db	251
   ;	
   ;	
   ;	}
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_int8_task_switch	endp
TASK_TEXT	ends
_BSS	segment word public 'BSS'
	db	4 dup (?)
	db	4 dup (?)
	db	4 dup (?)
	db	4 dup (?)
	db	2 dup (?)
	db	2 dup (?)
	db	4 dup (?)
_BSS	ends
TASK_TEXT	segment byte public 'CODE'
   ;	
   ;	int make_task(task_type task, unsigned int stck_size, int reqid,
   ;	
	assume	cs:TASK_TEXT
_make_task	proc	far
	push	bp
	mov	bp,sp
	sub	sp,12
	push	si
	push	di
   ;	
   ;	                char taskchar,char *name)
   ;	{
   ;	  static struct int_regs *r;
   ;	  static struct task_struct *cur_task_struct;
   ;	  static struct task_struct *old_task_struct;
   ;	  static task_type new_task;
   ;	  static unsigned int save_ss;
   ;	  static unsigned int save_sp;
   ;	  static time_t now;
   ;	
   ;	  int testid;
   ;	  int id;
   ;	  char t[12];
   ;	
   ;	#ifdef DEBUG
   ;	  static char s[80];
   ;	#endif
   ;	
   ;	  disable();    /* disable so we don't accidenally jump to a task */
   ;	
	db	250
   ;	
   ;	                /* with no stack, and some other task doesn't */
   ;	                /* trash our static variables */
   ;	  id = -1;
   ;	
	mov	si,-1
   ;	
   ;	
   ;	  now=time(NULL);
   ;	
	xor	ax,ax
	xor	dx,dx
	push	ax
	push	dx
	call	far ptr _time
	pop	cx
	pop	cx
	mov	word ptr DGROUP:b@w+20+2,dx
	mov	word ptr DGROUP:b@w+20,ax
   ;	
   ;	
   ;	  if ((reqid != -1) && (!tasks[reqid].status)) id = reqid;
   ;	
	cmp	word ptr [bp+12],-1
	je	short @10@114
	mov	ax,word ptr [bp+12]
	mov	dx,35
	imul	dx
	mov	bx,ax
	cmp	word ptr DGROUP:_tasks[bx],0
	jne	short @10@114
	mov	si,word ptr [bp+12]
	jmp	short @10@282
@10@114:
   ;	
   ;	   else     /* force the task number if reqid != -1 */
   ;	    {
   ;	     for (testid=(MAX_THREADS-1);(testid>=0) && (id == -1);testid--)
   ;	
	mov	di,25
	jmp	short @10@226
@10@142:
   ;	
   ;	      if (!(tasks[testid].status))  /* otherwise look for an dead thread */
   ;	
	mov	ax,di
	mov	dx,35
	imul	dx
	mov	bx,ax
	cmp	word ptr DGROUP:_tasks[bx],0
	jne	short @10@198
   ;	
   ;	       id = testid;
   ;	
	mov	si,di
@10@198:
	dec	di
@10@226:
	or	di,di
	jl	short @10@282
	cmp	si,-1
	je	short @10@142
@10@282:
   ;	
   ;	    }
   ;	
   ;	  if (id==-1)
   ;	
	cmp	si,-1
	jne	short @10@366
@10@310:
   ;	
   ;	  {
   ;	    enable();
   ;	
	db	251
   ;	
   ;	    return (-1);
   ;	
	mov	ax,-1
@10@338:
	jmp	@10@562
@10@366:
   ;	
   ;	  }
   ;	
   ;	  sprintf(t,"STACK%02d",id);
   ;	
	push	si
	push	ds
	mov	ax,offset DGROUP:s@
	push	ax
	push	ss
	lea	ax,word ptr [bp-12]
	push	ax
	call	far ptr _sprintf
	add	sp,10
   ;	
   ;	
   ;	  cur_task_struct = &tasks[id];
   ;	
	mov	ax,si
	mov	dx,35
	imul	dx
	add	ax,offset DGROUP:_tasks
	mov	word ptr DGROUP:b@w+4+2,ds
	mov	word ptr DGROUP:b@w+4,ax
   ;	
   ;	  old_task_struct = &tasks[tswitch];
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,35
	imul	dx
	add	ax,offset DGROUP:_tasks
	mov	word ptr DGROUP:b@w+8+2,ds
	mov	word ptr DGROUP:b@w+8,ax
   ;	
   ;	  new_task = task;
   ;	
	mov	ax,word ptr [bp+8]
	mov	dx,word ptr [bp+6]
	mov	word ptr DGROUP:b@w+12+2,ax
	mov	word ptr DGROUP:b@w+12,dx
   ;	
   ;	/* ALLOCATE STACKS FROM MAIN IF (1), from EMS
   ;	   if (0), but right now, it's commented out */
   ;	
   ;	/* if (1)
   ;	 *{
   ;	 *  cur_task_struct->stck = g_malloc_main_only
   ;	 *  (stck_size + sizeof(struct int_regs),t);
   ;	 *} else
   ;	 *{
   ;	 */
   ;	                        /* ALLOCATE STACKS FROM EMS IF WE CAN */
   ;	    cur_task_struct->stck = g_malloc_no_owner
   ;	
   ;	
   ;	    (stck_size + sizeof(struct int_regs),t,id);
   ;	
	push	si
	push	ss
	lea	ax,word ptr [bp-12]
	push	ax
	mov	ax,word ptr [bp+10]
	add	ax,24
	xor	dx,dx
	push	dx
	push	ax
	call	far ptr _g_malloc_no_owner
	add	sp,10
	les	bx,dword ptr DGROUP:b@w+4
	mov	word ptr es:[bx+13],dx
	mov	word ptr es:[bx+11],ax
   ;	
   ;	
   ;	/* }*/
   ;	
   ;	#ifdef DEBUG
   ;	  sprintf(s,"Allocated memory %p",tasks[id].stck);
   ;	  direct_screen(3,0,0x17,s);
   ;	  sprintf(s,"%p",*((char *)tasks[id].stck-4));
   ;	  direct_screen(3,40,0x17,s);
   ;	#endif
   ;	
   ;	  if (!cur_task_struct->stck)
   ;	
	les	bx,dword ptr DGROUP:b@w+4
	mov	ax,word ptr es:[bx+11]
	or	ax,word ptr es:[bx+13]
	jne	short @10@422
	jmp	@10@310
@10@422:
   ;	
   ;	  {
   ;	    enable();
   ;	    return (-1);
   ;	  }   /* return -1 if we couldn't allocate a stack */
   ;	
   ;	  r = (struct int_regs far *) ((long int) (cur_task_struct->stck)
   ;	
   ;	
   ;	    | ( stck_size - sizeof(struct int_regs)));
   ;	
	mov	ax,word ptr [bp+10]
	sub	ax,24
	les	bx,dword ptr DGROUP:b@w+4
	mov	dx,word ptr es:[bx+13]
	mov	bx,word ptr es:[bx+11]
	or	bx,ax
	or	dx,0
	mov	word ptr DGROUP:b@w+0+2,dx
	mov	word ptr DGROUP:b@w+0,bx
   ;	
   ;	  /* Initialize task stack */
   ;	  cur_task_struct->taskchar = taskchar;
   ;	
	les	bx,dword ptr DGROUP:b@w+4
	mov	al,byte ptr [bp+14]
	mov	byte ptr es:[bx+10],al
   ;	
   ;	  cur_task_struct->sp = FP_OFF((struct int_regs *) r);
   ;	
	les	bx,dword ptr DGROUP:b@w+4
	mov	ax,word ptr DGROUP:b@w+0
	mov	word ptr es:[bx+6],ax
   ;	
   ;	         /* set new stack location */
   ;	  cur_task_struct->ss = FP_SEG((struct int_regs *) r);
   ;	
	les	bx,dword ptr DGROUP:b@w+4
	mov	ax,word ptr DGROUP:b@w+0+2
	mov	word ptr es:[bx+8],ax
   ;	
   ;	         /* to registers in array */
   ;	  save_ss = _SS;
   ;	
	mov	word ptr DGROUP:b@w+16,ss
   ;	
   ;	  save_sp = _SP;
   ;	
	mov	word ptr DGROUP:b@w+18,sp
   ;	
   ;	
   ;	  _SS = oldss;
   ;	
	mov	ss,word ptr DGROUP:_oldss
   ;	
   ;	  _SP = oldsp;
   ;	
	mov	sp,word ptr DGROUP:_oldsp
   ;	
   ;	
   ;	
   ;	  if (cur_task_struct->is_ems)
   ;	
	les	bx,dword ptr DGROUP:b@w+4
	cmp	byte ptr es:[bx+15],0
	je	short @10@478
   ;	
   ;	  {
   ;	    /*
   ;	    _DX = cur_task_struct->ems_handle;
   ;	    _CX = cur_task_struct->mapped_pages;
   ;	    _SI = (unsigned int) emm_page_mapping;
   ;	    _AX = 0x5000;
   ;	    geninterrupt(0x67);
   ;	    */
   ;	    flip_a_ems_page(cur_task_struct);
   ;	
	push	word ptr DGROUP:b@w+4+2
	push	word ptr DGROUP:b@w+4
	push	cs
	call	near ptr _flip_a_ems_page
	pop	cx
	pop	cx
   ;	
   ;	    disable();
   ;	
	db	250
@10@478:
   ;	
   ;	  }
   ;	
   ;	
   ;	  /* set up task code segment and IP */
   ;	  r->cs = FP_SEG(new_task); /* set up CS:IP of new task to function start */
   ;	
	les	bx,dword ptr DGROUP:b@w+0
	mov	ax,word ptr DGROUP:b@w+12+2
	mov	word ptr es:[bx+20],ax
   ;	
   ;	  r->ip = FP_OFF(new_task);
   ;	
	les	bx,dword ptr DGROUP:b@w+0
	mov	ax,word ptr DGROUP:b@w+12
	mov	word ptr es:[bx+18],ax
   ;	
   ;	
   ;	  /* set up DS and ES segments */
   ;	  r->ds = _DS;              /* set it for same DS and ES to use same data */
   ;	
	les	bx,dword ptr DGROUP:b@w+0
	mov	word ptr es:[bx+6],ds
   ;	
   ;	  r->es = _ES;
   ;	
	les	bx,dword ptr DGROUP:b@w+0
	mov	word ptr es:[bx+8],es
   ;	
   ;	
   ;	  /* enable interrupts - see text */
   ;	  r->flags = 0x200;         /* set up flags for interrupt enable */
   ;	
	les	bx,dword ptr DGROUP:b@w+0
	mov	word ptr es:[bx+22],512
   ;	
   ;	
   ;	  if (old_task_struct->is_ems)
   ;	
	les	bx,dword ptr DGROUP:b@w+8
	cmp	byte ptr es:[bx+15],0
	je	short @10@534
   ;	
   ;	  {
   ;	    /*_DX = old_task_struct->ems_handle;
   ;	    _CX = old_task_struct->mapped_pages;
   ;	    _SI = (unsigned int) emm_page_mapping;
   ;	    _AX = 0x5000;
   ;	    geninterrupt(0x67);
   ;	     */
   ;	     flip_a_ems_page(old_task_struct);
   ;	
	push	word ptr DGROUP:b@w+8+2
	push	word ptr DGROUP:b@w+8
	push	cs
	call	near ptr _flip_a_ems_page
	pop	cx
	pop	cx
   ;	
   ;	     disable();
   ;	
	db	250
@10@534:
   ;	
   ;	
   ;	/*  for (map=0;map<4;map++)
   ;	    {
   ;	      _DX = old_task_struct->ems_handle;
   ;	      _AL = map;
   ;	      _BX = map;
   ;	      _AH = 0x44;
   ;	      geninterrupt(0x67);
   ;	    } */
   ;	  }
   ;	
   ;	  _SS = save_ss;
   ;	
	mov	ss,word ptr DGROUP:b@w+16
   ;	
   ;	  _SP = save_sp;
   ;	
	mov	sp,word ptr DGROUP:b@w+18
   ;	
   ;	
   ;	  cur_task_struct->status = ALIVE;
   ;	
	les	bx,dword ptr DGROUP:b@w+4
	mov	word ptr es:[bx],1
   ;	
   ;	  cur_task_struct->paused = 0;
   ;	
	les	bx,dword ptr DGROUP:b@w+4
	mov	word ptr es:[bx+2],0
   ;	
   ;	  cur_task_struct->who_paused_me=-1;
   ;	
	les	bx,dword ptr DGROUP:b@w+4
	mov	word ptr es:[bx+4],-1
   ;	
   ;	  cur_task_struct->time_created=now;
   ;	
	les	bx,dword ptr DGROUP:b@w+4
	mov	ax,word ptr DGROUP:b@w+20+2
	mov	dx,word ptr DGROUP:b@w+20
	mov	word ptr es:[bx+23],ax
	mov	word ptr es:[bx+21],dx
   ;	
   ;	  strncpy(cur_task_struct->name,name,9);
   ;	
	mov	ax,9
	push	ax
	push	word ptr [bp+18]
	push	word ptr [bp+16]
	mov	ax,word ptr DGROUP:b@w+4
	add	ax,25
	push	word ptr DGROUP:b@w+4+2
	push	ax
	call	far ptr _strncpy
	add	sp,10
   ;	
   ;	  cur_task_struct->name[9]=0;
   ;	
	les	bx,dword ptr DGROUP:b@w+4
	mov	byte ptr es:[bx+34],0
   ;	
   ;	  numTasksOpen++;
   ;	
	inc	word ptr DGROUP:_numTasksOpen
   ;	
   ;	
   ;	  enable();
   ;	
	db	251
   ;	
   ;	  return(id);
   ;	
	mov	ax,si
	jmp	@10@338
@10@562:
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_make_task	endp
   ;	
   ;	int ctrl_brk_handler(void)
   ;	
	assume	cs:TASK_TEXT
_ctrl_brk_handler	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  return 1;
   ;	
	mov	ax,1
	jmp	short @11@58
@11@58:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_ctrl_brk_handler	endp
   ;	
   ;	void initMultitask(void)
   ;	
	assume	cs:TASK_TEXT
_initMultitask	proc	far
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	  int count;
   ;	  begin_task_struct = (struct task_struct near *) &tasks;  /* initialize */
   ;	
	mov	word ptr DGROUP:_begin_task_struct,offset DGROUP:_tasks
   ;	
   ;	  end_task_struct = begin_task_struct + MAX_THREADS;  /* quick boundary ptrs */
   ;	
	mov	ax,word ptr DGROUP:_begin_task_struct
	add	ax,910
	mov	word ptr DGROUP:_end_task_struct,ax
   ;	
   ;	  for (count=0;count<MAX_THREADS;count++)    /* mark all tasks as currently */
   ;	
	xor	si,si
	jmp	short @12@114
@12@58:
   ;	
   ;	   {
   ;	     tasks[count].status = 0;                  /* dead */
   ;	
	mov	ax,si
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx],0
   ;	
   ;	     tasks[count].paused = 0;                  /* and NOT paused */
   ;	
	mov	ax,si
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+2],0
   ;	
   ;	     tasks[count].who_paused_me=-1;
   ;	
	mov	ax,si
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+4],-1
   ;	
   ;	     tasks[count].is_ems = 0;
   ;	
	mov	ax,si
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	byte ptr DGROUP:_tasks[bx+15],0
   ;	
   ;	     task_fast[count] = (struct task_struct near *) &tasks[count];
   ;	
	mov	ax,si
	mov	dx,35
	imul	dx
	add	ax,offset DGROUP:_tasks
	mov	bx,si
	shl	bx,1
	mov	word ptr DGROUP:_task_fast[bx],ax
	inc	si
@12@114:
	cmp	si,26
	jl	short @12@58
   ;	
   ;	                               /* set up task struct loc table */
   ;	   };
   ;	  for (count=0;count<MAXSEMAPHORES;count++)  /* mark all semaphores as not */
   ;	
	xor	si,si
	jmp	short @12@226
@12@170:
   ;	
   ;	   semaphores[count]=-1;                     /* used */
   ;	
	mov	byte ptr DGROUP:_semaphores[si],255
	inc	si
@12@226:
	cmp	si,256
	jl	short @12@170
   ;	
   ;	  numTasksOpen=0;                            /* tell tasker no tasks are open */
   ;	
	mov	word ptr DGROUP:_numTasksOpen,0
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_initMultitask	endp
   ;	
   ;	void interrupt multitask(void)
   ;	
	assume	cs:TASK_TEXT
_multitask	proc	far
	push	ax
	push	bx
	push	cx
	push	dx
	push	es
	push	ds
	push	si
	push	di
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
   ;	
   ;	{
   ;	  int checksw = 0;
   ;	
	xor	si,si
   ;	
   ;	
   ;	  dans_counter = 0;
   ;	
	mov	word ptr DGROUP:_dans_counter,0
   ;	
   ;	  ctrlbrk(ctrl_brk_handler);        /* make sure ctrl-brk doesn't interrupt! */
   ;	
	mov	ax,seg _ctrl_brk_handler
	push	ax
	mov	ax,offset _ctrl_brk_handler
	push	ax
	call	far ptr _ctrlbrk
	pop	cx
	pop	cx
   ;	
   ;	  disable();                        /* don't accidentally switch */
   ;	
	db	250
   ;	
   ;	  tswitch = -1;                     /* start at task 0 (really) */
   ;	
	mov	word ptr DGROUP:_tswitch,-1
   ;	
   ;	  dontKeepNextTask = 1;             /* make sure we keep this task at least */
   ;	
	mov	word ptr DGROUP:_dontKeepNextTask,1
	jmp	short @13@170
@13@58:
   ;	
   ;	                                    /* a tick */
   ;	  while ((tswitch == -1) && (checksw < MAX_THREADS))
   ;	   {
   ;	    if (tasks[checksw].status && !tasks[checksw].paused)
   ;	
	mov	ax,si
	mov	dx,35
	imul	dx
	mov	bx,ax
	cmp	word ptr DGROUP:_tasks[bx],0
	je	short @13@142
	mov	ax,si
	mov	dx,35
	imul	dx
	mov	bx,ax
	cmp	word ptr DGROUP:_tasks[bx+2],0
	jne	short @13@142
   ;	
   ;	               tswitch=checksw;  /* search for an open thread */
   ;	
	mov	word ptr DGROUP:_tswitch,si
@13@142:
   ;	
   ;	    checksw++;
   ;	
	inc	si
@13@170:
	cmp	word ptr DGROUP:_tswitch,-1
	jne	short @13@226
	cmp	si,26
	jl	short @13@58
@13@226:
   ;	
   ;	   };
   ;	  if (tswitch == -1) return;        /* if no tasks yet, bomb */
   ;	
	cmp	word ptr DGROUP:_tswitch,-1
	jne	short @13@282
	jmp	@13@478
@13@282:
   ;	
   ;	  tasking = 1; /* we will start tasking */
   ;	
	mov	byte ptr DGROUP:_tasking,1
   ;	
   ;	  switchTasks = 1;
   ;	
	mov	word ptr DGROUP:_switchTasks,1
   ;	
   ;	
   ;	 /* switch_to_ems_context(tswitch);*/   /* switch into EMS context of task */
   ;	
   ;	  if (task_fast[tswitch]->is_ems) flip_ems_page();
   ;	
	mov	bx,word ptr DGROUP:_tswitch
	shl	bx,1
	mov	bx,word ptr DGROUP:_task_fast[bx]
	cmp	byte ptr [bx+15],0
	je	short @13@338
	push	cs
	call	near ptr _flip_ems_page
@13@338:
   ;	
   ;	
   ;	  /* Reset interrupt 8 */
   ;	  old_int8 = getvect(8);            /* set interrupt 8 for tasking */
   ;	
	mov	ax,8
	push	ax
	call	far ptr _getvect
	pop	cx
	mov	word ptr DGROUP:_old_int8+2,dx
	mov	word ptr DGROUP:_old_int8,ax
   ;	
   ;	  if (dv_loaded)
   ;	
	cmp	byte ptr DGROUP:_dv_loaded,0
	je	short @13@394
   ;	
   ;	    setvect(8, dv_int8_task_switch);
   ;	
	mov	ax,seg _dv_int8_task_switch
	push	ax
	mov	ax,offset _dv_int8_task_switch
	jmp	short @13@422
@13@394:
   ;	
   ;	  else
   ;	    setvect(8, int8_task_switch);
   ;	
	mov	ax,seg _int8_task_switch
	push	ax
	mov	ax,offset _int8_task_switch
@13@422:
	push	ax
	mov	ax,8
	push	ax
	call	far ptr _setvect
	add	sp,6
   ;	
   ;	
   ;	  /* save original stack and pointer */
   ;	  oldss = _SS;                      /* save our original stack */
   ;	
	mov	word ptr DGROUP:_oldss,ss
   ;	
   ;	  oldsp = _SP;
   ;	
	mov	word ptr DGROUP:_oldsp,sp
   ;	
   ;	
   ;	  /* reroute stack to first task */
   ;	  _SS = tasks[tswitch].ss;          /* go to first task's stack */
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	ss,word ptr DGROUP:_tasks[bx+8]
   ;	
   ;	  _SP = tasks[tswitch].sp;
   ;	
	mov	ax,word ptr DGROUP:_tswitch
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	sp,word ptr DGROUP:_tasks[bx+6]
   ;	
   ;	
   ;	  enable();
   ;	
	db	251
@13@478:
   ;	
   ;	
   ;	  /* see next_task for details of routine exit */
   ;	};
   ;	
	pop	bp
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	iret	
_multitask	endp
   ;	
   ;	void lock(int sem)
   ;	
	assume	cs:TASK_TEXT
_lock	proc	far
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+6]
	jmp	short @14@86
@14@58:
   ;	
   ;	{
   ;	 disable();                 /* make sure someone else isn't locking */
   ;	 while (semaphores[sem] != -1)    /* if we don't have semaphores yet */
   ;	  {
   ;	    enable();               /* go to the next task to wait for semaphore */
   ;	
	db	251
   ;	
   ;	    next_task();
   ;	
	pushf	
	push	cs
	call	near ptr _next_task
   ;	
   ;	    disable();
   ;	
@14@86:
	db	250
	cmp	byte ptr DGROUP:_semaphores[si],255
	jne	short @14@58
   ;	
   ;	  };
   ;	 semaphores[sem] = tswitch; /* flag our semaphore! */
   ;	
	mov	al,byte ptr DGROUP:_tswitch
	mov	byte ptr DGROUP:_semaphores[si],al
   ;	
   ;	 enable();
   ;	
	db	251
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_lock	endp
   ;	
   ;	void unlock(int sem)
   ;	
	assume	cs:TASK_TEXT
_unlock	proc	far
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+6]
   ;	
   ;	{
   ;	 disable();                     /* unflag the semaphore */
   ;	
	db	250
   ;	
   ;	 semaphores[sem] = -1;
   ;	
	mov	byte ptr DGROUP:_semaphores[si],255
   ;	
   ;	 enable();
   ;	
	db	251
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_unlock	endp
   ;	
   ;	void free_semaphores(int task_num)
   ;	
	assume	cs:TASK_TEXT
_free_semaphores	proc	far
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	 int count;
   ;	 for (count=0;count<MAXSEMAPHORES;count++)
   ;	
	xor	si,si
	jmp	short @16@142
@16@58:
   ;	
   ;	  if (semaphores[count] == task_num) semaphores[count] = -1;
   ;	
	mov	al,byte ptr DGROUP:_semaphores[si]
	cbw	
	cmp	ax,word ptr [bp+6]
	jne	short @16@114
	mov	byte ptr DGROUP:_semaphores[si],255
@16@114:
	inc	si
@16@142:
	cmp	si,256
	jl	short @16@58
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_free_semaphores	endp
   ;	
   ;	int islocked(int sem)
   ;	
	assume	cs:TASK_TEXT
_islocked	proc	far
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+6]
   ;	
   ;	{
   ;	 return (semaphores[sem] != - 1);  /* see if this semaphore is locked */
   ;	
	cmp	byte ptr DGROUP:_semaphores[si],255
	je	short @17@86
	mov	ax,1
	jmp	short @17@114
@17@86:
	xor	ax,ax
@17@114:
	jmp	short @17@142
@17@142:
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_islocked	endp
   ;	
   ;	void lock_dos(void)
   ;	
	assume	cs:TASK_TEXT
_lock_dos	proc	far
	push	bp
	mov	bp,sp
	sub	sp,104
   ;	
   ;	{
   ;	 void far **thing = (((char *)&thing) + 4);
   ;	
	lea	ax,word ptr [bp]
	mov	word ptr [bp-2],ss
	mov	word ptr [bp-4],ax
   ;	
   ;	 char s[100];
   ;	
   ;	 sprintf(s,"Last stack pointer: %p %p ofs: %08lX",*thing,
   ;	
   ;	
   ;	     wait_for_xmit_keyboard,
   ;	     abs_adr(*thing)-abs_adr(wait_for_xmit_keyboard));
   ;	
	les	bx,dword ptr [bp-4]
	mov	ax,word ptr es:[bx+2]
	xor	dx,dx
	mov	cl,4
	call	far ptr F_LXLSH@
	les	bx,dword ptr [bp-4]
	add	ax,word ptr es:[bx]
	adc	dx,0
	push	ax
	mov	ax,seg _wait_for_xmit_keyboard
	push	dx
	xor	dx,dx
	mov	cl,4
	call	far ptr F_LXLSH@
	add	ax,offset _wait_for_xmit_keyboard
	adc	dx,0
	pop	bx
	pop	cx
	sub	cx,ax
	sbb	bx,dx
	push	bx
	push	cx
	mov	ax,seg _wait_for_xmit_keyboard
	push	ax
	mov	ax,offset _wait_for_xmit_keyboard
	push	ax
	les	bx,dword ptr [bp-4]
	push	word ptr es:[bx+2]
	push	word ptr es:[bx]
	push	ds
	mov	ax,offset DGROUP:s@+10
	push	ax
	push	ss
	lea	ax,word ptr [bp-104]
	push	ax
	call	far ptr _sprintf
	add	sp,20
   ;	
   ;	 direct_screen_override(0,0,0x17,s);
   ;	
	push	ss
	lea	ax,word ptr [bp-104]
	push	ax
	mov	ax,23
	push	ax
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	call	far ptr _direct_screen_override
	add	sp,10
   ;	
   ;	
   ;	 disable();                     /* see lock for details */
   ;	
	db	250
   ;	
   ;	 semaphores[DOS_SEM] = tswitch;
   ;	
	mov	al,byte ptr DGROUP:_tswitch
	mov	byte ptr DGROUP:_semaphores,al
   ;	
   ;	 switchTasks = 0;
   ;	
	mov	word ptr DGROUP:_switchTasks,0
   ;	
   ;	 enable();
   ;	
	db	251
   ;	
   ;	};
   ;	
	mov	sp,bp
	pop	bp
	ret	
_lock_dos	endp
   ;	
   ;	void unlock_dos(void)
   ;	
	assume	cs:TASK_TEXT
_unlock_dos	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	 disable();
   ;	
	db	250
   ;	
   ;	 semaphores[DOS_SEM] = -1;
   ;	
	mov	byte ptr DGROUP:_semaphores,255
   ;	
   ;	 switchTasks = 1;
   ;	
	mov	word ptr DGROUP:_switchTasks,1
   ;	
   ;	 enable();
   ;	
	db	251
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_unlock_dos	endp
   ;	
   ;	int iskilled(int task_num)
   ;	
	assume	cs:TASK_TEXT
_iskilled	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  return(!tasks[task_num].status);
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	ax,word ptr DGROUP:_tasks[bx]
	neg	ax
	sbb	ax,ax
	inc	ax
	jmp	short @20@58
@20@58:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_iskilled	endp
   ;	
   ;	int ispaused(int task_num)
   ;	
	assume	cs:TASK_TEXT
_ispaused	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  return(tasks[task_num].paused);
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	ax,word ptr DGROUP:_tasks[bx+2]
	jmp	short @21@58
@21@58:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_ispaused	endp
   ;	
   ;	int who_paused(int task_num)
   ;	
	assume	cs:TASK_TEXT
_who_paused	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  return(tasks[task_num].who_paused_me);
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	ax,word ptr DGROUP:_tasks[bx+4]
	jmp	short @22@58
@22@58:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_who_paused	endp
   ;	
   ;	void pause(int task_num)
   ;	
	assume	cs:TASK_TEXT
_pause	proc	far
	push	bp
	mov	bp,sp
	push	si
	mov	si,word ptr [bp+6]
   ;	
   ;	{
   ;	  if (task_num!=tswitch)
   ;	
	cmp	si,word ptr DGROUP:_tswitch
	je	short @23@86
   ;	
   ;	     {
   ;	        wait_for_death(task_num);
   ;	
	push	si
	call	far ptr _wait_for_death
	pop	cx
   ;	
   ;	        tasks[task_num].paused=1;
   ;	
	mov	ax,si
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+2],1
   ;	
   ;	        tasks[task_num].who_paused_me=tswitch;
   ;	
	mov	ax,si
	mov	dx,35
	imul	dx
	mov	dx,word ptr DGROUP:_tswitch
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+4],dx
@23@86:
   ;	
   ;	     }
   ;	
   ;	  return;
   ;	
	jmp	short @23@114
@23@114:
   ;	
   ;	};
   ;	
	pop	si
	pop	bp
	ret	
_pause	endp
   ;	
   ;	void unpause(int task_num)
   ;	
	assume	cs:TASK_TEXT
_unpause	proc	far
	push	bp
	mov	bp,sp
	mov	cx,word ptr [bp+6]
   ;	
   ;	{
   ;	  tasks[task_num].paused=0;
   ;	
	mov	ax,cx
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+2],0
   ;	
   ;	  tasks[task_num].who_paused_me=-1;
   ;	
	mov	ax,cx
	mov	dx,35
	imul	dx
	mov	bx,ax
	mov	word ptr DGROUP:_tasks[bx+4],-1
   ;	
   ;	  return;
   ;	
	jmp	short @24@58
@24@58:
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_unpause	endp
   ;	
   ;	void main(int argc, char **argv)
   ;	
	assume	cs:TASK_TEXT
_main	proc	far
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  allocate_resources(argv);
   ;	
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	call	far ptr _allocate_resources
	pop	cx
	pop	cx
   ;	
   ;	  multitask();              /* start multitasking! */
   ;	
	pushf	
	push	cs
	call	near ptr _multitask
   ;	
   ;	  if (sys_toggles.should_reboot)
   ;	
	cmp	byte ptr DGROUP:_sys_toggles,0
	je	short @25@86
   ;	
   ;	                    reboot();
   ;	
	call	far ptr _reboot
@25@86:
   ;	
   ;	  de_allocate_resources();
   ;	
	call	far ptr _de_allocate_resources
   ;	
   ;	
   ;	};
   ;	
	pop	bp
	ret	
_main	endp
TASK_TEXT	ends
_BSS	segment word public 'BSS'
_dontKeepNextTask	label	word
	db	2 dup (?)
_semaphores	label	byte
	db	256 dup (?)
_curMaxTasks	label	word
	db	2 dup (?)
_old_tswitch	label	word
	db	2 dup (?)
_oldsp	label	word
	db	2 dup (?)
_oldss	label	word
	db	2 dup (?)
_old_int8	label	dword
	db	4 dup (?)
_lock_dos_offset	label	word
	db	2 dup (?)
_numTasksOpen	label	word
	db	2 dup (?)
_switchTasks	label	word
	db	2 dup (?)
_dans_counter	label	word
	db	2 dup (?)
_tswitch	label	word
	db	2 dup (?)
_end_task_struct	label	word
	db	2 dup (?)
_begin_task_struct	label	word
	db	2 dup (?)
_tasks	label	word
	db	910 dup (?)
_task_fast	label	word
	db	52 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'STACK%02d'
	db	0
	db	'Last stack pointer: %p %p ofs: %08lX'
	db	0
_DATA	ends
TASK_TEXT	segment byte public 'CODE'
TASK_TEXT	ends
	extrn	__setargv__:far
	public	_main
	extrn	_direct_screen_override:far
	extrn	F_LXLSH@:far
	extrn	F_LXURSH@:far
	extrn	F_LXMUL@:far
	public	_flip_a_ems_page
	public	_flip_ems_page
	public	_switch_to_ems_context
	public	_dontKeepNextTask
	public	_semaphores
	public	_curMaxTasks
	public	_old_tswitch
	public	_oldsp
	public	_oldss
	public	_old_int8
	public	_emm_page_mapping
	public	_dv_int8_task_switch
	public	_lock_dos_offset
	extrn	_wait_for_death:far
	extrn	_sys_toggles:word
	extrn	_wait_for_xmit_keyboard:far
	extrn	_g_free_all_handles:far
	extrn	_g_free_from_who:far
	extrn	_g_malloc_no_owner:far
	extrn	_de_allocate_resources:far
	extrn	_allocate_resources:far
	extrn	_reboot:far
	extrn	_dv_loaded:byte
	public	_unpause
	public	_pause
	public	_who_paused
	public	_ispaused
	public	_free_semaphores
	public	_iskilled
	public	_ctrl_brk_handler
	public	_unlock_dos
	public	_lock_dos
	public	_our_task_id
	public	_next_task
	public	_kill_task
	public	_end_task
	public	_islocked
	public	_unlock
	public	_lock
	public	_initMultitask
	public	_make_task
	public	_int8_task_switch
	public	_multitask
	public	_system_load
	public	_max_task_switches
	public	_num_task_switches
	public	_timer_section
	public	_numTasksOpen
	public	_switchTasks
	public	_dans_counter
	public	_tasking
	public	_tswitch
	public	_end_task_struct
	public	_begin_task_struct
	public	_tasks
	public	_task_fast
	extrn	_time:far
	extrn	_strncpy:far
	extrn	_setvect:far
	extrn	_getvect:far
	extrn	_ctrlbrk:far
	extrn	_sprintf:far
_s@	equ	s@
	end
