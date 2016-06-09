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
	?debug	S "poker.c"
	?debug	C E947786A1B07706F6B65722E63
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C737464696F2E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C5F646566732E68
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C5F6E66696C652E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C5F6E756C6C2E68
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C7374646172672E68
	?debug	C E94019CA181F433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C74696D652E68
	?debug	C E94019CA181E433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C6469722E68
	?debug	C E94019CA181E433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C646F732E68
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C737472696E672E68
	?debug	C E94019CA1821433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C7374646C69622E68
	?debug	C E94019CA1820433A5C50524F475C424F524C414E44435C494E434C+
	?debug	C 5544455C63747970652E68
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
_ourblock	label	word
	db	71
	db	115
	db	84
	db	97
	db	114
	db	84
	db	105
	db	68
	db	2 dup (0)
	db	0
	db	0
	db	27
	db	0
	db	77
	db	69
	db	66
	db	79
	db	84
	db	4 dup (0)
	db	2
	db	0
	db	0
	db	0
_run_levels	label	word
	dw	CODE:s@
	dw	CODE:s@+5
	dw	CODE:s@+10
	dw	CODE:s@+15
	dw	CODE:s@+19
	dw	CODE:s@+27
	dw	CODE:s@+31
_num_players	label	word
	db	0
	db	0
_game_run_level	label	word
	db	0
	db	0
_poker_channel	label	word
	db	2
	db	0
_first_better	label	word
	db	0
	db	0
_current_better	label	word
	db	0
	db	0
_poker_ante	label	word
	db	5
	db	0
	db	0
	db	0
_long_suit_names	label	word
	dw	CODE:s@+40
	dw	CODE:s@+46
	dw	CODE:s@+53
	dw	CODE:s@+62
_long_card_names	label	word
	dw	CODE:s@+69
	dw	CODE:s@+73
	dw	CODE:s@+79
	dw	CODE:s@+84
	dw	CODE:s@+89
	dw	CODE:s@+93
	dw	CODE:s@+99
	dw	CODE:s@+105
	dw	CODE:s@+110
	dw	CODE:s@+114
	dw	CODE:s@+119
	dw	CODE:s@+125
	dw	CODE:s@+130
_plural_long_card_names	label	word
	dw	CODE:s@+134
	dw	CODE:s@+139
	dw	CODE:s@+146
	dw	CODE:s@+152
	dw	CODE:s@+158
	dw	CODE:s@+164
	dw	CODE:s@+171
	dw	CODE:s@+178
	dw	CODE:s@+184
	dw	CODE:s@+189
	dw	CODE:s@+195
	dw	CODE:s@+202
	dw	CODE:s@+208
_short_suit_names	label	word
	dw	CODE:s@+213
	dw	CODE:s@+215
	dw	CODE:s@+229
	dw	CODE:s@+243
_short_card_names	label	word
	dw	CODE:s@+245
	dw	CODE:s@+247
	dw	CODE:s@+249
	dw	CODE:s@+251
	dw	CODE:s@+253
	dw	CODE:s@+255
	dw	CODE:s@+257
	dw	CODE:s@+259
	dw	CODE:s@+261
	dw	CODE:s@+264
	dw	CODE:s@+266
	dw	CODE:s@+268
	dw	CODE:s@+270
_tokens	label	word
	dw	CODE:s@+272
	dw	CODE:s@+276
	dw	CODE:s@+280
	dw	CODE:s@+284
	dw	CODE:s@+288
	dw	CODE:s@+292
	dw	CODE:s@+296
	dw	CODE:s@+300
	dw	CODE:s@+304
	dw	CODE:s@+308
	dw	CODE:s@+312
	dw	CODE:s@+316
	dw	CODE:s@+320
	dw	CODE:s@+324
	dw	CODE:s@+328
	dw	CODE:s@+332
	dw	CODE:s@+336
	dw	CODE:s@+340
   ;	
   ;	void reset_round_vars(void)
   ;	
	assume	cs:CODE
_reset_round_vars	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	 int count;
   ;	
   ;	 for (count=0;count<num_players;count++)
   ;	
	xor	cx,cx
	jmp	short @1@114
@1@58:
   ;	
   ;	  {
   ;	   players[count].warning_level=0;
   ;	
	mov	ax,cx
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+68],0
	inc	cx
@1@114:
	cmp	cx,word ptr CODE:_num_players
	jl	short @1@58
   ;	
   ;	
   ;	  }
   ;	  current_better = first_better;
   ;	
	mov	ax,word ptr CODE:_first_better
	mov	word ptr CODE:_current_better,ax
   ;	
   ;	  current_bet=0;
   ;	
	mov	word ptr CODE:_current_bet+2,0
	mov	word ptr CODE:_current_bet,0
   ;	
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_reset_round_vars	endp
   ;	
   ;	void initialize_game(void)
   ;	
	assume	cs:CODE
_initialize_game	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	   int count,count2;
   ;	   int selected_player;
   ;	   int bet_selection[MAX_PLAYERS];
   ;	   int already_selected;
   ;	
   ;	
   ;	   current_bet=0;
   ;	
	mov	word ptr CODE:_current_bet+2,0
	mov	word ptr CODE:_current_bet,0
   ;	
   ;	   broke_player=0;
   ;	
	mov	word ptr CODE:_broke_player,0
   ;	
   ;	
   ;	   for (count=0;count<num_players;count++)
   ;	
	xor	si,si
	jmp	@2@254
@2@58:
   ;	
   ;	    {
   ;	     players[count].playing=1;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+74],1
   ;	
   ;	     players[count].run_level=NOT_RUNNING;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+66],0
   ;	
   ;	     players[count].fold=0;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+72],0
   ;	
   ;	     players[count].have_cards=0;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	byte ptr cs:_players[bx+61],0
   ;	
   ;	
   ;	
   ;		 if (players[count].money>players[count].credit)
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+2]
	mov	dx,word ptr cs:_players[bx]
	push	ax
	mov	ax,si
	mov	bx,76
	push	dx
	imul	bx
	mov	bx,ax
	pop	ax
	pop	dx
	cmp	dx,word ptr cs:_players[bx+6]
	jb	short @2@170
	ja	short @2@142
	cmp	ax,word ptr cs:_players[bx+4]
	jbe	short @2@170
@2@142:
   ;	
   ;		  { players[count].money -= players[count].credit;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+6]
	mov	dx,word ptr cs:_players[bx+4]
	push	ax
	mov	ax,si
	mov	bx,76
	push	dx
	imul	bx
	mov	bx,ax
	pop	ax
	sub	word ptr cs:_players[bx],ax
	pop	ax
	sbb	word ptr cs:_players[bx+2],ax
	jmp	short @2@198
@2@170:
   ;	
   ;			players[count].credit = 0;
   ;		  }
   ;		 else
   ;		  {
   ;			players[count].money  = 0;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+2],0
	mov	word ptr cs:_players[bx],0
@2@198:
   ;	
   ;			players[count].credit = 0;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+6],0
	mov	word ptr cs:_players[bx+4],0
	inc	si
@2@254:
	cmp	si,word ptr CODE:_num_players
	jge	@@0
	jmp	@2@58
@@0:
   ;	
   ;		  }
   ;	    }
   ;								 
   ;	    /* pick the first better */
   ;	
   ;	   bet_limit=0;
   ;	
	mov	word ptr CODE:_bet_limit+2,0
	mov	word ptr CODE:_bet_limit,0
   ;	
   ;	
   ;	    first_better++;
   ;	
	inc	word ptr CODE:_first_better
   ;	
   ;	    if (first_better>num_players)
   ;	
	mov	ax,word ptr CODE:_first_better
	cmp	ax,word ptr CODE:_num_players
	jle	short @2@338
   ;	
   ;	     first_better=0;
   ;	
	mov	word ptr CODE:_first_better,0
@2@338:
   ;	
   ;	    current_better=first_better;
   ;	
	mov	ax,word ptr CODE:_first_better
	mov	word ptr CODE:_current_better,ax
   ;	
   ;	
   ;	   reset_round_vars();
   ;	
	call	near ptr CODE:_reset_round_vars
   ;	
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
_initialize_game	endp
   ;	
   ;	void put_players_in_game(void)
   ;	
	assume	cs:CODE
_put_players_in_game	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	 int count;
   ;	
   ;	 for (count=0;count<num_players;count++)
   ;	
	xor	cx,cx
	jmp	short @3@114
@3@58:
   ;	
   ;	   players[count].run_level=DEAL_ROUND;
   ;	
	mov	ax,cx
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+66],1
	inc	cx
@3@114:
	cmp	cx,word ptr CODE:_num_players
	jl	short @3@58
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_put_players_in_game	endp
   ;	
   ;	void start_game(int portnum)
   ;	
	assume	cs:CODE
_start_game	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	    int player_no = player_number(portnum);
   ;	
	push	si
	call	near ptr CODE:_player_number
	pop	cx
	mov	word ptr [bp-2],ax
   ;	
   ;	
   ;	
   ;	    if (player_no<0)
   ;	
	cmp	word ptr [bp-2],0
	jge	short @4@114
   ;	
   ;	     {
   ;	      private_poker_message("Your not in the game, type \"/b poker join\" to join the game",portnum);
   ;	
	push	si
	push	cs
	mov	ax,offset CODE:s@+344
@4@86:
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
   ;	
   ;	      return;
   ;	
	jmp	short @4@254
@4@114:
   ;	
   ;	     }
   ;	
   ;	    if (game_run_level!=NOT_RUNNING)
   ;	
	cmp	word ptr CODE:_game_run_level,0
	je	short @4@170
   ;	
   ;	      {
   ;	      private_poker_message("This game is not yet over!",portnum);
   ;	
	push	si
	push	cs
	mov	ax,offset CODE:s@+404
	jmp	short @4@86
@4@170:
   ;	
   ;	      return;
   ;	      }
   ;	
   ;	    if (num_players<2)
   ;	
	cmp	word ptr CODE:_num_players,2
	jge	short @4@226
   ;	
   ;	      {
   ;	       poker_message("Must have two players to play");
   ;	
	push	cs
	mov	ax,offset CODE:s@+431
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	       return;
   ;	
	jmp	short @4@254
@4@226:
   ;	
   ;	      }
   ;	
   ;	    initialize_game();
   ;	
	call	near ptr CODE:_initialize_game
   ;	
   ;	    shuffle_deck();
   ;	
	call	near ptr CODE:_shuffle_deck
   ;	
   ;	    put_players_in_game();
   ;	
	call	near ptr CODE:_put_players_in_game
   ;	
   ;	    game_run_level = DEAL_ROUND;
   ;	
	mov	word ptr CODE:_game_run_level,1
@4@254:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_start_game	endp
   ;	
   ;	void ante_player(int portnum)
   ;	
	assume	cs:CODE
_ante_player	proc	near
	push	bp
	mov	bp,sp
	sub	sp,200
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	 int player_no = player_number(portnum);
   ;	
	push	si
	call	near ptr CODE:_player_number
	pop	cx
	mov	di,ax
   ;	
   ;	 int s[100];
   ;	
   ;	 if (player_no<0)
   ;	
	or	di,di
	jge	short @5@114
   ;	
   ;	   {
   ;	    private_poker_message("You must be in the game to ante",portnum);
   ;	
	push	si
	push	cs
	mov	ax,offset CODE:s@+461
@5@86:
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
   ;	
   ;	    return;
   ;	
	jmp	@5@254
@5@114:
   ;	
   ;	   }
   ;	
   ;	 if (game_run_level!=ANTE_ROUND)
   ;	
	cmp	word ptr CODE:_game_run_level,2
	je	short @5@170
   ;	
   ;	   {
   ;	     private_poker_message("This is not time to Ante",portnum);
   ;	
	push	si
	push	cs
	mov	ax,offset CODE:s@+493
	jmp	short @5@86
@5@170:
   ;	
   ;	     return;
   ;	   }
   ;	 if (players[player_no].run_level!=ANTE_ROUND)
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],2
	je	short @5@226
   ;	
   ;	   {
   ;	     private_poker_message("This is not the time for you to Ante.",portnum);
   ;	
	push	si
	push	cs
	mov	ax,offset CODE:s@+518
	jmp	short @5@86
@5@226:
   ;	
   ;	     return;
   ;	   }
   ;	
   ;	
   ;	  players[player_no].money-=poker_ante;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr CODE:_poker_ante+2
	mov	bx,word ptr CODE:_poker_ante
	xchg	bx,ax
	sub	word ptr cs:_players[bx],ax
	sbb	word ptr cs:_players[bx+2],dx
   ;	
   ;	  poker_pot+=poker_ante;
   ;	
	mov	ax,word ptr CODE:_poker_ante+2
	mov	dx,word ptr CODE:_poker_ante
	add	word ptr CODE:_poker_pot,dx
	adc	word ptr CODE:_poker_pot+2,ax
   ;	
   ;	  sprintf(s,"%s|*r1 has anted",(char far *)players[player_no].handle);
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+556
	push	ax
	push	ss
	lea	ax,word ptr [bp-200]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	  poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-200]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	
   ;	  players[player_no].run_level=FIRST_BET_ROUND;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+66],3
   ;	
   ;	
   ;	  print_poker_cards(portnum);
   ;	
	push	si
	call	near ptr CODE:_print_poker_cards
	pop	cx
@5@254:
   ;	
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_ante_player	endp
   ;	
   ;	unsigned long int read_amount(char *string)
   ;	
	assume	cs:CODE
_read_amount	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
   ;	
   ;	{
   ;	 unsigned long int amount1 = 0l;
   ;	
	mov	word ptr [bp-2],0
	mov	word ptr [bp-4],0
   ;	
   ;	 unsigned long int amount2 = 0l;
   ;	
	mov	word ptr [bp-6],0
	mov	word ptr [bp-8],0
	jmp	short @6@86
@6@58:
   ;	
   ;	
   ;	 while ((*string==' ') && (*string))
   ;	  string++;
   ;	
	inc	word ptr [bp+4]
@6@86:
	les	bx,dword ptr [bp+4]
	cmp	byte ptr es:[bx],32
	jne	short @6@142
	les	bx,dword ptr [bp+4]
	cmp	byte ptr es:[bx],0
	jne	short @6@58
@6@142:
   ;	
   ;	
   ;	 for (;;)
   ;	 {
   ;	   if ((*string<'0') || (*string>'9'))
   ;	
	les	bx,dword ptr [bp+4]
	cmp	byte ptr es:[bx],48
	jl	short @6@198
	les	bx,dword ptr [bp+4]
	cmp	byte ptr es:[bx],57
	jle	short @6@226
@6@198:
   ;	
   ;	     return (amount1);
   ;	
	mov	dx,word ptr [bp-2]
	mov	ax,word ptr [bp-4]
	jmp	short @6@394
@6@226:
   ;	
   ;	
   ;	   amount2=((amount1*10) + (*string-'0'));
   ;	
	mov	cx,word ptr [bp-2]
	mov	bx,word ptr [bp-4]
	xor	dx,dx
	mov	ax,10
	call	far ptr F_LXMUL@
	les	bx,dword ptr [bp+4]
	push	ax
	mov	al,byte ptr es:[bx]
	cbw	
	add	ax,-48
	push	dx
	cwd	
	pop	bx
	pop	cx
	add	cx,ax
	adc	bx,dx
	mov	word ptr [bp-6],bx
	mov	word ptr [bp-8],cx
   ;	
   ;	
   ;	   if (amount2<amount1) /* if it flipped over return */
   ;	
	mov	ax,word ptr [bp-6]
	mov	dx,word ptr [bp-8]
	cmp	ax,word ptr [bp-2]
	ja	short @6@338
	jb	short @6@310
	cmp	dx,word ptr [bp-4]
	jae	short @6@338
@6@310:
	jmp	short @6@198
@6@338:
   ;	
   ;	     return amount1;
   ;	
   ;	   amount1=amount2;
   ;	
	mov	ax,word ptr [bp-6]
	mov	dx,word ptr [bp-8]
	mov	word ptr [bp-2],ax
	mov	word ptr [bp-4],dx
   ;	
   ;	
   ;	   string++;
   ;	
	inc	word ptr [bp+4]
	jmp	short @6@142
@6@394:
   ;	
   ;	
   ;	 }
   ;	
   ;	
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_read_amount	endp
   ;	
   ;	int check_bet_round(int portnum)
   ;	
	assume	cs:CODE
_check_bet_round	proc	near
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int player_no = player_number(portnum);
   ;	
	push	si
	call	near ptr CODE:_player_number
	pop	cx
	mov	di,ax
   ;	
   ;	
   ;	 if (player_no<0)
   ;	
	or	di,di
	jge	short @7@170
   ;	
   ;	  {
   ;	    private_poker_message("You must be in the game to bet",portnum);
   ;	
	push	si
	push	cs
	mov	ax,offset CODE:s@+573
@7@86:
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
   ;	
   ;		return 0;
   ;	
@7@114:
	xor	ax,ax
@7@142:
	jmp	short @7@506
@7@170:
   ;	
   ;	  }
   ;	
   ;	  if ((game_run_level==FIRST_BET_ROUND) || (game_run_level==FINAL_BET_ROUND))
   ;	
	cmp	word ptr CODE:_game_run_level,3
	je	short @7@226
	cmp	word ptr CODE:_game_run_level,5
	jne	short @7@450
@7@226:
   ;	
   ;	  {
   ;	
   ;		if (players[player_no].fold)
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+72],0
	je	short @7@282
   ;	
   ;		 {
   ;		   private_poker_message("You folded, you're no longer in this hand.",portnum);
   ;	
	push	si
	push	cs
	mov	ax,offset CODE:s@+604
	jmp	short @7@86
@7@282:
   ;	
   ;		   return 0;
   ;		}
   ;	
   ;		if (players[player_no].run_level==NOT_RUNNING)
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],0
	jne	short @7@338
   ;	
   ;		 {
   ;		   private_poker_message("You're not in this hand.",portnum);
   ;	
	push	si
	push	cs
	mov	ax,offset CODE:s@+647
	jmp	short @7@86
@7@338:
   ;	
   ;		   return 0;
   ;		 }
   ;	
   ;	    if (!players[player_no].warning_level)
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+68],0
	jne	short @7@394
   ;	
   ;	     {
   ;		   private_poker_message("It is not your turn to bet.",portnum);
   ;	
	push	si
	push	cs
	mov	ax,offset CODE:s@+672
	jmp	short @7@86
@7@394:
   ;	
   ;		   return 0;
   ;	     }
   ;	
   ;		 return 1;
   ;	
	mov	ax,1
	jmp	short @7@142
   ;	
   ;	
   ;	  }
   ;	
	jmp	short @7@478
@7@450:
   ;	
   ;	  else
   ;	    private_poker_message("This is not the time to bet.",portnum);
   ;	
	push	si
	push	cs
	mov	ax,offset CODE:s@+700
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
@7@478:
	jmp	short @7@114
@7@506:
   ;	
   ;	
   ;	  return 0;
   ;	}
   ;	
	pop	di
	pop	si
	pop	bp
	ret	
_check_bet_round	endp
   ;	
   ;	void make_bet(int portnum,char *string)
   ;	
	assume	cs:CODE
_make_bet	proc	near
	push	bp
	mov	bp,sp
	sub	sp,204
	push	si
	push	di
   ;	
   ;	{
   ;	 int player_no = player_number(portnum);
   ;	
	push	word ptr [bp+4]
	call	near ptr CODE:_player_number
	pop	cx
	mov	di,ax
   ;	
   ;	 char s[200];
   ;	 unsigned long int amount=read_amount(string);
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr CODE:_read_amount
	pop	cx
	pop	cx
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
   ;	
   ;	
   ;	 if (!check_bet_round(portnum))
   ;	
	push	word ptr [bp+4]
	call	near ptr CODE:_check_bet_round
	pop	cx
	or	ax,ax
	jne	short @8@86
   ;	
   ;	  return;
   ;	
	jmp	@8@1346
@8@86:
   ;	
   ;	
   ;	
   ;	
   ;	     if (amount<players[player_no].current_bet)
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+64]
	mov	dx,word ptr cs:_players[bx+62]
	cmp	ax,word ptr [bp-2]
	jb	short @8@226
	ja	short @8@170
	cmp	dx,word ptr [bp-4]
	jbe	short @8@226
@8@170:
   ;	
   ;	     {
   ;	      sprintf(s,"You have already bet %lu.",players[player_no].current_bet);
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+64]
	push	word ptr cs:_players[bx+62]
	push	cs
	mov	ax,offset CODE:s@+729
@8@198:
	push	ax
	push	ss
	lea	ax,word ptr [bp-204]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
	jmp	@8@898
@8@226:
   ;	
   ;	      private_poker_message(s,portnum);
   ;	      return;
   ;	     }
   ;	
   ;	    if (players[player_no].money<(amount - players[player_no].current_bet))
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	push	ax
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr [bp-2]
	mov	bx,word ptr [bp-4]
	xchg	bx,ax
	sub	ax,word ptr cs:_players[bx+62]
	sbb	dx,word ptr cs:_players[bx+64]
	pop	bx
	cmp	word ptr cs:_players[bx+2],dx
	ja	short @8@338
	jb	short @8@310
	cmp	word ptr cs:_players[bx],ax
	jae	short @8@338
@8@310:
   ;	
   ;	     {
   ;	      sprintf(s,"You only have %lu, you can't afford that bet",players[player_no].money);
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+2]
	push	word ptr cs:_players[bx]
	push	cs
	mov	ax,offset CODE:s@+755
	jmp	short @8@198
@8@338:
   ;	
   ;	      private_poker_message(s,portnum);
   ;	      return;
   ;	     }
   ;	
   ;	     if ((amount)<current_bet)
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	cmp	ax,word ptr CODE:_current_bet+2
	ja	short @8@450
	jb	short @8@422
	cmp	dx,word ptr CODE:_current_bet
	jae	short @8@450
@8@422:
   ;	
   ;	     {
   ;	      sprintf(s,"You must at least see the current bet of %lu.",current_bet);
   ;	
	push	word ptr CODE:_current_bet+2
	push	word ptr CODE:_current_bet
	push	cs
	mov	ax,offset CODE:s@+800
	jmp	@8@198
@8@450:
   ;	
   ;	      private_poker_message(s,portnum);
   ;	      return;
   ;	     }
   ;	
   ;	     if ((!amount) && (broke_player) && (game_run_level==FINAL_BET_ROUND))
   ;	
	mov	ax,word ptr [bp-4]
	or	ax,word ptr [bp-2]
	jne	short @8@562
	cmp	word ptr CODE:_broke_player,0
	je	short @8@562
	cmp	word ptr CODE:_game_run_level,5
	jne	short @8@562
   ;	
   ;	      {
   ;	        private_poker_message("Sorry, a player will go broke this hand, you must bet at least 1.",portnum);
   ;	
	push	word ptr [bp+4]
	push	cs
	mov	ax,offset CODE:s@+846
	jmp	short @8@926
@8@562:
   ;	
   ;	        return;
   ;	      }
   ;	
   ;	     if ((amount>bet_limit) && (amount!=current_bet) && (bet_limit))
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	cmp	ax,word ptr CODE:_bet_limit+2
	jb	short @8@954
	ja	short @8@646
	cmp	dx,word ptr CODE:_bet_limit
	jbe	short @8@954
@8@646:
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	cmp	ax,word ptr CODE:_current_bet+2
	jne	short @8@702
	cmp	dx,word ptr CODE:_current_bet
	je	short @8@954
@8@702:
	mov	ax,word ptr CODE:_bet_limit
	or	ax,word ptr CODE:_bet_limit+2
	je	short @8@954
   ;	
   ;		 {
   ;		   if (current_bet>bet_limit)
   ;	
	mov	ax,word ptr CODE:_current_bet+2
	mov	dx,word ptr CODE:_current_bet
	cmp	ax,word ptr CODE:_bet_limit+2
	jb	short @8@842
	ja	short @8@814
	cmp	dx,word ptr CODE:_bet_limit
	jbe	short @8@842
@8@814:
   ;	
   ;			 sprintf(s,"Sorry, you may only see the current bet of %lu.",current_bet);
   ;	
	push	word ptr CODE:_current_bet+2
	push	word ptr CODE:_current_bet
	push	cs
	mov	ax,offset CODE:s@+912
	jmp	short @8@870
@8@842:
   ;	
   ;		   else
   ;			 sprintf(s,"Sorry, a bet limit of %lu is imposed.",bet_limit);
   ;	
	push	word ptr CODE:_bet_limit+2
	push	word ptr CODE:_bet_limit
	push	cs
	mov	ax,offset CODE:s@+960
@8@870:
	push	ax
	push	ss
	lea	ax,word ptr [bp-204]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
@8@898:
   ;	
   ;	
   ;		   private_poker_message(s,portnum);
   ;	
	push	word ptr [bp+4]
	push	ss
	lea	ax,word ptr [bp-204]
@8@926:
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
   ;	
   ;		   return;
   ;	
	jmp	@8@1346
@8@954:
   ;	
   ;		 }
   ;	
   ;	
   ;	      players[player_no].warning_level=0;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+68],0
   ;	
   ;	      players[player_no].run_level = game_run_level + 1;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr CODE:_game_run_level
	inc	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+66],dx
   ;	
   ;	
   ;	     if ((!current_bet) && (!amount))
   ;	
	mov	ax,word ptr CODE:_current_bet
	or	ax,word ptr CODE:_current_bet+2
	jne	short @8@1038
	mov	ax,word ptr [bp-4]
	or	ax,word ptr [bp-2]
	jne	short @8@1038
   ;	
   ;	     {
   ;	       sprintf(s,"%s|*r1 checks.",(char far *)players[player_no].handle);
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+998
	push	ax
	push	ss
	lea	ax,word ptr [bp-204]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	       poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-204]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	
   ;	       return;
   ;	
	jmp	@8@1346
@8@1038:
   ;	
   ;	     }
   ;	
   ;	     if (amount==current_bet)
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	cmp	ax,word ptr CODE:_current_bet+2
	je	@@1
	jmp	@8@1178
@@1:
	cmp	dx,word ptr CODE:_current_bet
	je	@@2
	jmp	@8@1178
@@2:
   ;	
   ;	     {
   ;	      sprintf(s,"%s|*r1 sees the current bet of %lu.",(char far *)players[player_no].handle,current_bet);
   ;	
	push	word ptr CODE:_current_bet+2
	push	word ptr CODE:_current_bet
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+1013
	push	ax
	push	ss
	lea	ax,word ptr [bp-204]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,16
   ;	
   ;	      poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-204]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	      players[player_no].money-=(amount - players[player_no].current_bet);
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr [bp-2]
	mov	bx,word ptr [bp-4]
	xchg	bx,ax
	sub	ax,word ptr cs:_players[bx+62]
	sbb	dx,word ptr cs:_players[bx+64]
	push	ax
	mov	ax,di
	mov	bx,76
	push	dx
	imul	bx
	mov	bx,ax
	pop	ax
	pop	dx
	sub	word ptr cs:_players[bx],dx
	sbb	word ptr cs:_players[bx+2],ax
   ;	
   ;	      poker_pot+=(amount-players[player_no].current_bet);
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr [bp-2]
	mov	bx,word ptr [bp-4]
	xchg	bx,ax
	sub	ax,word ptr cs:_players[bx+62]
	sbb	dx,word ptr cs:_players[bx+64]
	add	word ptr CODE:_poker_pot,ax
	adc	word ptr CODE:_poker_pot+2,dx
   ;	
   ;	      players[player_no].current_bet = amount;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr [bp-2]
	mov	bx,word ptr [bp-4]
	mov	si,ax
	mov	word ptr cs:_players[si+64],dx
	mov	word ptr cs:_players[si+62],bx
   ;	
   ;	      if (!players[player_no].money)
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx]
	or	ax,word ptr cs:_players[bx+2]
	jne	short @8@1150
   ;	
   ;	       broke_player=1;
   ;	
	mov	word ptr CODE:_broke_player,1
@8@1150:
   ;	
   ;	      return;
   ;	
	jmp	@8@1346
@8@1178:
   ;	
   ;	     }
   ;	
   ;	
   ;	
   ;	     if (!current_bet)
   ;	
	mov	ax,word ptr CODE:_current_bet
	or	ax,word ptr CODE:_current_bet+2
	jne	short @8@1234
   ;	
   ;	       sprintf(s,"%s|*r1 has set the bet to %lu.",(char far *)players[player_no].handle,(unsigned long int)amount);
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+1049
	push	ax
	push	ss
	lea	ax,word ptr [bp-204]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,16
	jmp	short @8@1262
@8@1234:
   ;	
   ;	     else
   ;	       sprintf(s,"%s|*r1 raises %lu to make the bet %lu.",(char far *)players[player_no].handle,(unsigned long int)(amount-curre
   ;	
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	sub	dx,word ptr CODE:_current_bet
	sbb	ax,word ptr CODE:_current_bet+2
	push	ax
	push	dx
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+1080
	push	ax
	push	ss
	lea	ax,word ptr [bp-204]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,20
@8@1262:
   ;	
   ;	
   ;	      poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-204]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	
   ;	      current_bet = amount;
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	mov	word ptr CODE:_current_bet+2,ax
	mov	word ptr CODE:_current_bet,dx
   ;	
   ;	      players[player_no].money -= (amount - players[player_no].current_bet);
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr [bp-2]
	mov	bx,word ptr [bp-4]
	xchg	bx,ax
	sub	ax,word ptr cs:_players[bx+62]
	sbb	dx,word ptr cs:_players[bx+64]
	push	ax
	mov	ax,di
	mov	bx,76
	push	dx
	imul	bx
	mov	bx,ax
	pop	ax
	pop	dx
	sub	word ptr cs:_players[bx],dx
	sbb	word ptr cs:_players[bx+2],ax
   ;	
   ;	      poker_pot += (amount - players[player_no].current_bet);
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr [bp-2]
	mov	bx,word ptr [bp-4]
	xchg	bx,ax
	sub	ax,word ptr cs:_players[bx+62]
	sbb	dx,word ptr cs:_players[bx+64]
	add	word ptr CODE:_poker_pot,ax
	adc	word ptr CODE:_poker_pot+2,dx
   ;	
   ;	      players[player_no].current_bet = amount;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr [bp-2]
	mov	bx,word ptr [bp-4]
	mov	si,ax
	mov	word ptr cs:_players[si+64],dx
	mov	word ptr cs:_players[si+62],bx
   ;	
   ;	
   ;	      if (!players[player_no].money)
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx]
	or	ax,word ptr cs:_players[bx+2]
	jne	short @8@1318
   ;	
   ;	       broke_player=1;
   ;	
	mov	word ptr CODE:_broke_player,1
@8@1318:
   ;	
   ;	      return;
   ;	
	jmp	short @8@1346
@8@1346:
   ;	
   ;	
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_make_bet	endp
   ;	
   ;	void see_bet(int portnum)
   ;	
	assume	cs:CODE
_see_bet	proc	near
	push	bp
	mov	bp,sp
	sub	sp,104
	push	si
	push	di
   ;	
   ;	{
   ;	 int player_no = player_number(portnum);
   ;	
	push	word ptr [bp+4]
	call	near ptr CODE:_player_number
	pop	cx
	mov	di,ax
   ;	
   ;	 char s[100];
   ;	 unsigned long int amount_in_question;
   ;	
   ;	 if (!check_bet_round(portnum))
   ;	
	push	word ptr [bp+4]
	call	near ptr CODE:_check_bet_round
	pop	cx
	or	ax,ax
	jne	short @9@86
   ;	
   ;	  return;
   ;	
	jmp	@9@282
@9@86:
   ;	
   ;	
   ;	  if (!current_bet)
   ;	
	mov	ax,word ptr CODE:_current_bet
	or	ax,word ptr CODE:_current_bet+2
	jne	short @9@142
   ;	
   ;	   {
   ;	     private_poker_message("There is no current bet to see, use \"bet 0\" to check",portnum);
   ;	
	push	word ptr [bp+4]
	push	cs
	mov	ax,offset CODE:s@+1119
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
   ;	
   ;	     return;
   ;	
	jmp	@9@282
@9@142:
   ;	
   ;	   }
   ;	
   ;	  amount_in_question = (current_bet - players[player_no].current_bet);
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr CODE:_current_bet+2
	mov	bx,word ptr CODE:_current_bet
	xchg	bx,ax
	sub	ax,word ptr cs:_players[bx+62]
	sbb	dx,word ptr cs:_players[bx+64]
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],ax
   ;	
   ;	
   ;	  players[player_no].warning_level=0;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+68],0
   ;	
   ;	  players[player_no].run_level = game_run_level + 1;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr CODE:_game_run_level
	inc	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+66],dx
   ;	
   ;	
   ;	 if ((amount_in_question)>players[player_no].money)
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+2]
	mov	dx,word ptr cs:_players[bx]
	cmp	ax,word ptr [bp-2]
	jbe	@@3
	jmp	@9@254
@@3:
	jb	short @9@226
	cmp	dx,word ptr [bp-4]
	jb	@@4
	jmp	@9@254
@@4:
@9@226:
   ;	
   ;	  {
   ;	
   ;	    players[player_no].credit += (amount_in_question - players[player_no].money) + 1;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr [bp-2]
	mov	bx,word ptr [bp-4]
	xchg	bx,ax
	sub	ax,word ptr cs:_players[bx]
	sbb	dx,word ptr cs:_players[bx+2]
	add	ax,1
	adc	dx,0
	push	ax
	mov	ax,di
	mov	bx,76
	push	dx
	imul	bx
	mov	bx,ax
	pop	ax
	pop	dx
	add	word ptr cs:_players[bx+4],dx
	adc	word ptr cs:_players[bx+6],ax
   ;	
   ;	
   ;		sprintf(s,"%s|*r1 is lent %lu so he can see the bet.",players[player_no].handle,players[player_no].credit);
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+6]
	push	word ptr cs:_players[bx+4]
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+1172
	push	ax
	push	ss
	lea	ax,word ptr [bp-104]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,16
   ;	
   ;		poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-104]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;		sprintf(s,"Bet limit of 50 imposed for %s|*r1",players[player_no].handle);
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+1214
	push	ax
	push	ss
	lea	ax,word ptr [bp-104]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;		poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-104]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	
   ;		players[player_no].money = 1;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+2],0
	mov	word ptr cs:_players[bx],1
   ;	
   ;	    players[player_no].current_bet = current_bet;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr CODE:_current_bet+2
	mov	bx,word ptr CODE:_current_bet
	mov	si,ax
	mov	word ptr cs:_players[si+64],dx
	mov	word ptr cs:_players[si+62],bx
   ;	
   ;	
   ;		bet_limit = 50;
   ;	
	mov	word ptr CODE:_bet_limit+2,0
	mov	word ptr CODE:_bet_limit,50
   ;	
   ;		poker_pot +=amount_in_question;
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	add	word ptr CODE:_poker_pot,dx
	adc	word ptr CODE:_poker_pot+2,ax
   ;	
   ;	
   ;	  }
   ;	
	jmp	short @9@282
@9@254:
   ;	
   ;	 else
   ;	 {
   ;	
   ;		 sprintf(s,"%s|*r1 pays %lu to see the current bet of %lu.",players[player_no].handle,amount_in_question,current_bet);
   ;	
	push	word ptr CODE:_current_bet+2
	push	word ptr CODE:_current_bet
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+1249
	push	ax
	push	ss
	lea	ax,word ptr [bp-104]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,20
   ;	
   ;		 poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-104]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	
   ;		 players[player_no].money -= amount_in_question;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr [bp-2]
	mov	bx,word ptr [bp-4]
	xchg	bx,ax
	sub	word ptr cs:_players[bx],ax
	sbb	word ptr cs:_players[bx+2],dx
   ;	
   ;		 poker_pot				  += amount_in_question;
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,word ptr [bp-4]
	add	word ptr CODE:_poker_pot,dx
	adc	word ptr CODE:_poker_pot+2,ax
   ;	
   ;	
   ;		 players[player_no].current_bet = current_bet;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr CODE:_current_bet+2
	mov	bx,word ptr CODE:_current_bet
	mov	si,ax
	mov	word ptr cs:_players[si+64],dx
	mov	word ptr cs:_players[si+62],bx
@9@282:
   ;	
   ;	 }
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_see_bet	endp
   ;	
   ;	void fold_player(int portnum)
   ;	
	assume	cs:CODE
_fold_player	proc	near
	push	bp
	mov	bp,sp
	sub	sp,120
	push	si
	push	di
	mov	di,word ptr [bp+4]
   ;	
   ;	{
   ;	 int player_no = player_number(portnum);
   ;	
	push	di
	call	near ptr CODE:_player_number
	pop	cx
	mov	si,ax
   ;	
   ;	 char s[120];
   ;	
   ;	
   ;	 if (player_no<0)
   ;	
	or	si,si
	jge	short @10@114
   ;	
   ;	  {
   ;	    private_poker_message("You are not in the game.",portnum);
   ;	
	push	di
	push	cs
	mov	ax,offset CODE:s@+1296
@10@86:
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
   ;	
   ;	    return;
   ;	
	jmp	@10@282
@10@114:
   ;	
   ;	  }
   ;	
   ;	 if (game_run_level==NOT_RUNNING)
   ;	
	cmp	word ptr CODE:_game_run_level,0
	jne	short @10@170
   ;	
   ;	  {
   ;	    private_poker_message("The game has not yet begun.",portnum);
   ;	
	push	di
	push	cs
	mov	ax,offset CODE:s@+1321
	jmp	short @10@86
@10@170:
   ;	
   ;	    return;
   ;	  }
   ;	
   ;	 if ((players[player_no].fold) || (players[player_no].run_level==NOT_RUNNING))
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+72],0
	jne	short @10@226
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],0
	jne	short @10@254
@10@226:
   ;	
   ;	  {
   ;	    private_poker_message("You are not in this game.",portnum);
   ;	
	push	di
	push	cs
	mov	ax,offset CODE:s@+1349
	jmp	short @10@86
@10@254:
   ;	
   ;	    return;
   ;	  }
   ;	 players[player_no].fold=1;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+72],1
   ;	
   ;	 players[player_no].run_level = NOT_RUNNING;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+66],0
   ;	
   ;	
   ;	 sprintf(s,"%s|*r1 folds.",players[player_no].handle);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+1375
	push	ax
	push	ss
	lea	ax,word ptr [bp-120]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	
   ;	 poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-120]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
@10@282:
   ;	
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_fold_player	endp
   ;	
   ;	int find_card_in_hand(int player_no,int card)
   ;	
	assume	cs:CODE
_find_card_in_hand	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	 int count;
   ;	 for(count=0;count<5;count++)
   ;	
	xor	cx,cx
	jmp	short @11@170
@11@58:
   ;	
   ;	   if (players[player_no].cards_in_hand[count]==card)
   ;	
	mov	ax,word ptr [bp+4]
	mov	dx,76
	imul	dx
	add	ax,cx
	mov	bx,ax
	mov	al,byte ptr cs:_players[bx+56]
	cbw	
	cmp	ax,word ptr [bp+6]
	jne	short @11@142
   ;	
   ;	     return count;
   ;	
	mov	ax,cx
@11@114:
	jmp	short @11@226
@11@142:
	inc	cx
@11@170:
	cmp	cx,5
	jl	short @11@58
   ;	
   ;	
   ;	 return -1;
   ;	
	mov	ax,-1
	jmp	short @11@114
@11@226:
   ;	
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_find_card_in_hand	endp
   ;	
   ;	int check_for_blank_line(char *string)
   ;	
	assume	cs:CODE
_check_for_blank_line	proc	near
	push	bp
	mov	bp,sp
	jmp	short @12@198
@12@58:
   ;	
   ;	{
   ;	  while (*string)
   ;	   if (*string!=' ')
   ;	
	les	bx,dword ptr [bp+4]
	cmp	byte ptr es:[bx],32
	je	short @12@170
   ;	
   ;	     return 0;
   ;	
	xor	ax,ax
@12@114:
	jmp	short @12@254
	jmp	short @12@198
@12@170:
   ;	
   ;	   else
   ;	     string++;
   ;	
	inc	word ptr [bp+4]
@12@198:
	les	bx,dword ptr [bp+4]
	cmp	byte ptr es:[bx],0
	jne	short @12@58
   ;	
   ;	
   ;	   return 1;
   ;	
	mov	ax,1
	jmp	short @12@114
@12@254:
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_check_for_blank_line	endp
   ;	
   ;	void discard(int portnum,char *string)
   ;	
	assume	cs:CODE
_discard	proc	near
	push	bp
	mov	bp,sp
	sub	sp,174
	push	si
	push	di
   ;	
   ;	{
   ;	   int player_no = player_number(portnum);
   ;	
	push	word ptr [bp+4]
	call	near ptr CODE:_player_number
	pop	cx
	mov	word ptr [bp-2],ax
   ;	
   ;	   int count,count2,temp;
   ;	   char *command=string+1;
   ;	
	mov	ax,word ptr [bp+8]
	mov	dx,word ptr [bp+6]
	inc	dx
	mov	word ptr [bp-8],ax
	mov	word ptr [bp-10],dx
   ;	
   ;	   int bad_format=0;
   ;	
	mov	word ptr [bp-12],0
   ;	
   ;	   int type=0;
   ;	
	mov	word ptr [bp-14],0
   ;	
   ;	   char s[150];
   ;	   long int value;
   ;	   char discard_positions[5];
   ;	   int num_tokens=0;
   ;	
	xor	si,si
   ;	
   ;	
   ;	
   ;	   if (player_no<0)
   ;	
	cmp	word ptr [bp-2],0
	jge	short @13@142
   ;	
   ;	     {
   ;	        private_poker_message("You must be in the game to discard.",portnum);
   ;	
	push	word ptr [bp+4]
	push	cs
	mov	ax,offset CODE:s@+1389
@13@86:
	push	ax
@13@114:
	call	near ptr CODE:_private_poker_message
	add	sp,6
   ;	
   ;	        return;
   ;	
	jmp	@13@1486
@13@142:
   ;	
   ;	     }
   ;	   if (game_run_level!=DISCARD_ROUND)
   ;	
	cmp	word ptr CODE:_game_run_level,4
	je	short @13@198
   ;	
   ;	     {
   ;	        private_poker_message("This is not the time to discard.",portnum);
   ;	
	push	word ptr [bp+4]
	push	cs
	mov	ax,offset CODE:s@+1425
	jmp	short @13@86
@13@198:
   ;	
   ;	        return;
   ;	     }
   ;	   if (players[player_no].run_level!=DISCARD_ROUND)
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],4
	je	short @13@254
   ;	
   ;	     {
   ;	        private_poker_message("You have already discarded.",portnum);
   ;	
	push	word ptr [bp+4]
	push	cs
	mov	ax,offset CODE:s@+1458
	jmp	short @13@86
@13@254:
   ;	
   ;	        return;
   ;	     }
   ;	
   ;	   if (check_for_blank_line(string))
   ;	
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr CODE:_check_for_blank_line
	pop	cx
	pop	cx
	or	ax,ax
	je	short @13@310
   ;	
   ;	     {
   ;	        sprintf(s,"%s|*r1 is discarding no cards.",(char far *)players[player_no].handle);
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+1486
	push	ax
	push	ss
	lea	ax,word ptr [bp-174]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
	jmp	@13@1458
@13@310:
	jmp	short @13@870
@13@338:
   ;	
   ;	        poker_message(s);
   ;	        players[player_no].run_level = FINAL_BET_ROUND;
   ;	        players[player_no].warning_level=0;
   ;	        return;
   ;	     }
   ;	
   ;	   while ((get_token(&command, &type, &value)) && num_tokens<5)
   ;	   {
   ;	
   ;	      if (type==NUMBER_TOKEN)
   ;	
	cmp	word ptr [bp-14],2
	jne	short @13@618
   ;	
   ;	        {
   ;	         if ((value<1) || (value>5))
   ;	
	cmp	word ptr [bp-16],0
	jl	short @13@534
	jne	short @13@450
	cmp	word ptr [bp-18],1
	jb	short @13@534
@13@450:
	cmp	word ptr [bp-16],0
	jl	short @13@562
	jg	short @13@534
	cmp	word ptr [bp-18],5
	jbe	short @13@562
@13@534:
   ;	
   ;	           bad_format=1;
   ;	
	mov	word ptr [bp-12],1
	jmp	short @13@590
@13@562:
   ;	
   ;	         else
   ;	           discard_positions[num_tokens] = (value-1);
   ;	
	mov	al,byte ptr [bp-18]
	dec	al
	mov	byte ptr [bp+si-24],al
@13@590:
   ;	
   ;	        }
   ;	
	jmp	short @13@786
@13@618:
   ;	
   ;	      else
   ;	      if (type==CARD_TOKEN)
   ;	
	cmp	word ptr [bp-14],3
	jne	short @13@758
   ;	
   ;	        {
   ;	         temp = find_card_in_hand(player_no,value);
   ;	
	push	word ptr [bp-18]
	push	word ptr [bp-2]
	call	near ptr CODE:_find_card_in_hand
	pop	cx
	pop	cx
	mov	word ptr [bp-6],ax
   ;	
   ;	         if (temp==-1)
   ;	
	cmp	word ptr [bp-6],-1
	jne	short @13@702
   ;	
   ;	           bad_format=1;
   ;	
	mov	word ptr [bp-12],1
	jmp	short @13@730
@13@702:
   ;	
   ;	         else
   ;	            discard_positions[num_tokens] = temp;
   ;	
	mov	al,byte ptr [bp-6]
	mov	byte ptr [bp+si-24],al
@13@730:
   ;	
   ;	        }
   ;	
	jmp	short @13@786
@13@758:
   ;	
   ;	      else
   ;	        bad_format=1;
   ;	
	mov	word ptr [bp-12],1
@13@786:
   ;	
   ;	
   ;	
   ;	      if (bad_format)
   ;	
	cmp	word ptr [bp-12],0
	je	short @13@842
   ;	
   ;	       {
   ;	         private_poker_message("Bad format in discard request, please retry.",portnum);
   ;	
	push	word ptr [bp+4]
	push	cs
	mov	ax,offset CODE:s@+1517
	jmp	@13@86
@13@842:
   ;	
   ;	         return;
   ;	       }
   ;	
   ;	      num_tokens++;
   ;	
	inc	si
   ;	
   ;	      type=0;
   ;	
	mov	word ptr [bp-14],0
@13@870:
	push	ss
	lea	ax,word ptr [bp-18]
	push	ax
	push	ss
	lea	ax,word ptr [bp-14]
	push	ax
	push	ss
	lea	ax,word ptr [bp-10]
	push	ax
	call	near ptr CODE:_get_token
	add	sp,12
	or	ax,ax
	je	short @13@926
	cmp	si,5
	jge	@@5
	jmp	@13@338
@@5:
@13@926:
   ;	
   ;	     }
   ;	
   ;	
   ;	    if (!num_tokens)
   ;	
	or	si,si
	jne	short @13@982
   ;	
   ;	     {
   ;	      private_poker_message("Bad discard request:",portnum);
   ;	
	push	word ptr [bp+4]
	push	cs
	mov	ax,offset CODE:s@+1562
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
   ;	
   ;	      private_poker_message(string,portnum);
   ;	
	push	word ptr [bp+4]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	jmp	@13@114
@13@982:
   ;	
   ;	      return;
   ;	     }
   ;	
   ;	    if (num_tokens==5)
   ;	
	cmp	si,5
	jne	short @13@1038
   ;	
   ;	     {
   ;	      private_poker_message("You may not discard all your cards.",portnum);
   ;	
	push	word ptr [bp+4]
	push	cs
	mov	ax,offset CODE:s@+1583
	jmp	@13@86
@13@1038:
   ;	
   ;	      return;
   ;	     }
   ;	
   ;	    for (count=0;count<num_tokens;count++)
   ;	
	xor	di,di
	jmp	short @13@1234
@13@1066:
   ;	
   ;	      for (count2=count+1;count2<num_tokens;count2++)
   ;	
	mov	ax,di
	inc	ax
	mov	word ptr [bp-4],ax
	jmp	short @13@1178
@13@1094:
   ;	
   ;	        if (discard_positions[count]==discard_positions[count2])
   ;	
	mov	al,byte ptr [bp+di-24]
	lea	dx,word ptr [bp-24]
	mov	bx,word ptr [bp-4]
	add	bx,dx
	cmp	al,byte ptr ss:[bx]
	jne	short @13@1150
   ;	
   ;	          {
   ;	            private_poker_message("Confusing discard request, please retry.",portnum);
   ;	
	push	word ptr [bp+4]
	push	cs
	mov	ax,offset CODE:s@+1619
	jmp	@13@86
@13@1150:
	inc	word ptr [bp-4]
@13@1178:
	cmp	word ptr [bp-4],si
	jl	short @13@1094
	inc	di
@13@1234:
	cmp	di,si
	jl	short @13@1066
   ;	
   ;	            return;
   ;	          }
   ;	
   ;	
   ;	    /* put stuff in to check for no ace and 4 discards */
   ;	
   ;	
   ;	    for (count=0;count<num_tokens;count++)
   ;	
	xor	di,di
	jmp	short @13@1346
@13@1290:
   ;	
   ;	      players[player_no].cards_in_hand[discard_positions[count]] = NO_CARD;
   ;	
	mov	al,byte ptr [bp+di-24]
	cbw	
	push	ax
	mov	ax,word ptr [bp-2]
	mov	dx,76
	imul	dx
	pop	dx
	add	ax,dx
	mov	bx,ax
	mov	byte ptr cs:_players[bx+56],255
	inc	di
@13@1346:
	cmp	di,si
	jl	short @13@1290
   ;	
   ;	
   ;	    if (num_tokens==1)
   ;	
	cmp	si,1
	jne	short @13@1430
   ;	
   ;	      sprintf(s,"%s|*r1 will be discarding 1 card.",(char far *)players[player_no].handle);
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+1660
	push	ax
	push	ss
	lea	ax,word ptr [bp-174]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
	jmp	short @13@1458
@13@1430:
   ;	
   ;	    else
   ;	      sprintf(s,"%s|*r1 will be discarding %d cards.",(char far *)players[player_no].handle,num_tokens);
   ;	
	push	si
	mov	ax,word ptr [bp-2]
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+1694
	push	ax
	push	ss
	lea	ax,word ptr [bp-174]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,14
@13@1458:
   ;	
   ;	    poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-174]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	
   ;	
   ;	    players[player_no].run_level = FINAL_BET_ROUND;
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+66],5
   ;	
   ;	    players[player_no].warning_level=0;
   ;	
	mov	ax,word ptr [bp-2]
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+68],0
   ;	
   ;	    return;
   ;	
	jmp	short @13@1486
@13@1486:
   ;	
   ;	
   ;	};
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_discard	endp
   ;	
   ;	void end_game(void)
   ;	
	assume	cs:CODE
_end_game	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	   int count;
   ;	
   ;	
   ;	   for (count=0;count<num_players;count++)
   ;	
	xor	cx,cx
	jmp	short @14@114
@14@58:
   ;	
   ;	    {
   ;	     players[count].playing=0;
   ;	
	mov	ax,cx
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+74],0
   ;	
   ;	     players[count].run_level=NOT_RUNNING;
   ;	
	mov	ax,cx
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+66],0
   ;	
   ;	     players[count].fold=0;
   ;	
	mov	ax,cx
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+72],0
   ;	
   ;	     players[count].have_cards=0;
   ;	
	mov	ax,cx
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	byte ptr cs:_players[bx+61],0
	inc	cx
@14@114:
	cmp	cx,word ptr CODE:_num_players
	jl	short @14@58
   ;	
   ;	    }
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_end_game	endp
   ;	
   ;	void empty_deck(void)
   ;	
	assume	cs:CODE
_empty_deck	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	  int count;
   ;	  for (count=0;count<NUM_CARDS;count++)
   ;	
	xor	si,si
	jmp	short @15@114
@15@58:
   ;	
   ;	    card_deck[count] = NO_CARD;
   ;	
	mov	byte ptr cs:_card_deck[si],255
	inc	si
@15@114:
	cmp	si,52
	jl	short @15@58
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
_empty_deck	endp
   ;	
   ;	void private_poker_message(char *string,int portnum)
   ;	
	assume	cs:CODE
_private_poker_message	proc	near
	push	bp
	mov	bp,sp
	sub	sp,20
	push	si
	mov	si,word ptr [bp+8]
   ;	
   ;	{
   ;	    char s[20];
   ;	    sprintf(s,"Port: [%d] ",portnum);
   ;	
	push	si
	push	cs
	mov	ax,offset CODE:s@+1730
	push	ax
	push	ss
	lea	ax,word ptr [bp-20]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,10
   ;	
   ;	    print_string("(private_poker_message): <PokerBot>: ");
   ;	
	push	cs
	mov	ax,offset CODE:s@+1742
	push	ax
	call	near ptr CODE:_print_string
	pop	cx
	pop	cx
   ;	
   ;	    print_string(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-20]
	push	ax
	call	near ptr CODE:_print_string
	pop	cx
	pop	cx
   ;	
   ;	    print_str_cr(string);
   ;	
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	call	near ptr CODE:_print_str_cr
	pop	cx
	pop	cx
   ;	
   ;	
   ;	    aput_append_into_buffer(portnum, poker_channel, 1,
   ;	
   ;	
   ;	                 tswitch, portnum, 0, 2, private_poker_header,string);
   ;	
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	push	cs
	mov	ax,offset CODE:_private_poker_header
	push	ax
	mov	ax,2
	push	ax
	xor	ax,ax
	push	ax
	push	si
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+220]
	push	word ptr es:[bx]
	mov	ax,1
	push	ax
	push	word ptr CODE:_poker_channel
	push	si
	call	near ptr CODE:_aput_append_into_buffer
	add	sp,22
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_private_poker_message	endp
   ;	
   ;	void poker_message(char *string)
   ;	
	assume	cs:CODE
_poker_message	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	
   ;	    print_string("(poker_message): --> PokerBot: ");
   ;	
	push	cs
	mov	ax,offset CODE:s@+1780
	push	ax
	call	near ptr CODE:_print_string
	pop	cx
	pop	cx
   ;	
   ;	    print_str_cr(string);
   ;	
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	call	near ptr CODE:_print_str_cr
	pop	cx
	pop	cx
   ;	
   ;	
   ;	    aput_append_into_buffer(server, poker_channel, 0,
   ;	
   ;	
   ;	                 poker_channel, tswitch, 0, 2, public_poker_header,string);
   ;	
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	push	cs
	mov	ax,offset CODE:_public_poker_header
	push	ax
	mov	ax,2
	push	ax
	xor	ax,ax
	push	ax
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+220]
	push	word ptr es:[bx]
	push	word ptr CODE:_poker_channel
	xor	ax,ax
	push	ax
	push	word ptr CODE:_poker_channel
	push	word ptr CODE:_server
	call	near ptr CODE:_aput_append_into_buffer
	add	sp,22
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_poker_message	endp
   ;	
   ;	int process_ante_round(void)
   ;	
	assume	cs:CODE
_process_ante_round	proc	near
	push	bp
	mov	bp,sp
	sub	sp,152
	push	si
	push	di
   ;	
   ;	{
   ;	    int count;
   ;	    int waiting_for_players=0;
   ;	
	xor	di,di
   ;	
   ;	    unsigned int time_elapsed;
   ;	    char s[150];
   ;	
   ;	    for(count=0;count<num_players;count++)
   ;	
	xor	si,si
	jmp	@18@702
@18@58:
   ;	
   ;	     if ((players[count].have_cards) && (!players[count].fold) && (players[count].run_level!=NOT_RUNNING))
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	byte ptr cs:_players[bx+61],0
	jne	@@6
	jmp	@18@674
@@6:
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+72],0
	je	@@7
	jmp	@18@674
@@7:
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],0
	jne	@@8
	jmp	@18@674
@@8:
   ;	
   ;	     {
   ;	
   ;	        if ((players[count].run_level==DEAL_ROUND))
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],1
	je	@@9
	jmp	@18@338
@@9:
   ;	
   ;	         {
   ;	
   ;	           if (players[count].money<poker_ante)
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+2]
	mov	dx,word ptr cs:_players[bx]
	cmp	ax,word ptr CODE:_poker_ante+2
	ja	short @18@282
	jb	short @18@254
	cmp	dx,word ptr CODE:_poker_ante
	jae	short @18@282
@18@254:
   ;	
   ;	               {
   ;	                 sprintf(s,"%s|*r1 is being forced out for not being able to afford the ante",(char far *)players[count].handle)
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+1812
	push	ax
	push	ss
	lea	ax,word ptr [bp-152]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	                 poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-152]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	                 deregister_player(players[count].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	call	near ptr CODE:_deregister_player
	pop	cx
   ;	
   ;	               }
   ;	
	jmp	short @18@310
@18@282:
   ;	
   ;	           else
   ;	               {
   ;	                 waiting_for_players=1;
   ;	
	mov	di,1
   ;	
   ;	                 players[count].warning_level=0;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+68],0
   ;	
   ;	                 poker_timer=dans_counter;
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	mov	ax,word ptr es:[bx]
	mov	word ptr CODE:_poker_timer+2,0
	mov	word ptr CODE:_poker_timer,ax
   ;	
   ;	                 players[count].run_level=ANTE_ROUND;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+66],2
   ;	
   ;	                 sprintf(s,"Type \"/b %s ante\" to ante. Current Ante is %lu",(char far *)"poker",poker_ante);
   ;	
	push	word ptr CODE:_poker_ante+2
	push	word ptr CODE:_poker_ante
	push	cs
	mov	ax,offset CODE:s@+1924
	push	ax
	push	cs
	mov	ax,offset CODE:s@+1877
	push	ax
	push	ss
	lea	ax,word ptr [bp-152]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,16
   ;	
   ;	                 private_poker_message(s,players[count].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	push	ss
	lea	ax,word ptr [bp-152]
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
@18@310:
   ;	
   ;	               }
   ;	         }
   ;	
	jmp	@18@674
@18@338:
   ;	
   ;	        else
   ;	        if ((players[count].run_level==ANTE_ROUND))
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],2
	je	@@10
	jmp	@18@674
@@10:
   ;	
   ;	          {
   ;	            waiting_for_players=1;
   ;	
	mov	di,1
   ;	
   ;	
   ;	            if (dans_counter<poker_timer)
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	mov	ax,word ptr es:[bx]
	xor	dx,dx
	cmp	dx,word ptr CODE:_poker_timer+2
	ja	short @18@478
	jb	short @18@450
	cmp	ax,word ptr CODE:_poker_timer
	jae	short @18@478
@18@450:
   ;	
   ;	              time_elapsed =  (unsigned int)0xFFFF - (poker_timer-dans_counter);
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	mov	ax,word ptr CODE:_poker_timer
	sub	ax,word ptr es:[bx]
	mov	dx,-1
	sub	dx,ax
	mov	word ptr [bp-2],dx
	jmp	short @18@506
@18@478:
   ;	
   ;	            else
   ;	              time_elapsed = dans_counter - poker_timer;
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	mov	ax,word ptr es:[bx]
	sub	ax,word ptr CODE:_poker_timer
	mov	word ptr [bp-2],ax
@18@506:
   ;	
   ;	
   ;	            if (((time_elapsed/18) > 60) && !players[count].warning_level)
   ;	
	mov	ax,word ptr [bp-2]
	mov	bx,18
	xor	dx,dx
	div	bx
	cmp	ax,60
	jbe	short @18@590
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+68],0
	jne	short @18@590
   ;	
   ;	              {
   ;	               private_poker_message("Please Ante or Quit the game (you have 45 seconds)",players[count].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	push	cs
	mov	ax,offset CODE:s@+1930
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
   ;	
   ;	               players[count].warning_level=1;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+68],1
@18@590:
   ;	
   ;	              }
   ;	
   ;	            if (((time_elapsed/18) > 105) && (players[count].warning_level>0))
   ;	
	mov	ax,word ptr [bp-2]
	mov	bx,18
	xor	dx,dx
	div	bx
	cmp	ax,105
	jbe	short @18@674
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+68],0
	jle	short @18@674
   ;	
   ;	               {
   ;	                 sprintf(s,"%s|*r1 is being kicked out for not responding",(char far *)players[count].handle);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+1981
	push	ax
	push	ss
	lea	ax,word ptr [bp-152]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	                 poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-152]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	                 deregister_player(players[count].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	call	near ptr CODE:_deregister_player
	pop	cx
@18@674:
	inc	si
@18@702:
	cmp	si,word ptr CODE:_num_players
	jge	@@11
	jmp	@18@58
@@11:
   ;	
   ;	               }
   ;	          }
   ;	     }
   ;	
   ;	    if (waiting_for_players)
   ;	
	or	di,di
	je	short @18@814
   ;	
   ;	       return 0;
   ;	
	xor	ax,ax
@18@786:
	jmp	short @18@842
@18@814:
   ;	
   ;	
   ;	
   ;	    return 1;    /* continue to next round */
   ;	
	mov	ax,1
	jmp	short @18@786
@18@842:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_process_ante_round	endp
   ;	
   ;	int process_bets(int next_round)
   ;	
	assume	cs:CODE
_process_bets	proc	near
	push	bp
	mov	bp,sp
	sub	sp,154
	push	si
	push	di
   ;	
   ;	{
   ;	 int count;
   ;	 int round_complete=1;
   ;	
	mov	di,1
   ;	
   ;	 int first_bets_in=1;
   ;	
	mov	word ptr [bp-2],1
   ;	
   ;	 unsigned int elapsed_time;
   ;	 char s[150];
   ;	
   ;	
   ;	 for (count=0;count<num_players;count++)
   ;	
	xor	si,si
	jmp	short @19@282
@19@58:
   ;	
   ;	  if (!((players[count].fold) || (players[count].run_level==NOT_RUNNING)))
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+72],0
	jne	short @19@254
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],0
	je	short @19@254
   ;	
   ;	    {
   ;	    if ((players[count].run_level!=next_round))
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+66]
	cmp	ax,word ptr [bp+4]
	je	short @19@170
   ;	
   ;	     {
   ;	      first_bets_in=0;
   ;	
	mov	word ptr [bp-2],0
   ;	
   ;	      players[count].current_bet=0;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+64],0
	mov	word ptr cs:_players[bx+62],0
   ;	
   ;	      round_complete=0;
   ;	
	xor	di,di
@19@170:
   ;	
   ;	     }
   ;	
   ;	    if (players[count].current_bet!=current_bet)
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+64]
	mov	dx,word ptr cs:_players[bx+62]
	cmp	ax,word ptr CODE:_current_bet+2
	jne	short @19@226
	cmp	dx,word ptr CODE:_current_bet
	je	short @19@254
@19@226:
   ;	
   ;	       round_complete=0;
   ;	
	xor	di,di
@19@254:
	inc	si
@19@282:
	cmp	si,word ptr CODE:_num_players
	jl	short @19@58
   ;	
   ;	
   ;	    }
   ;	
   ;	
   ;	 if (round_complete)
   ;	
	or	di,di
	je	short @19@394
   ;	
   ;	  return 1;
   ;	
	mov	ax,1
@19@366:
	jmp	@19@1234
@19@394:
   ;	
   ;	
   ;	 if (current_better>=num_players)
   ;	
	mov	ax,word ptr CODE:_current_better
	cmp	ax,word ptr CODE:_num_players
	jl	short @19@450
   ;	
   ;	   current_better=0;
   ;	
	mov	word ptr CODE:_current_better,0
@19@450:
	jmp	short @19@534
@19@478:
   ;	
   ;	
   ;	 while ((players[current_better].fold) || players[current_better].run_level==NOT_RUNNING ||
   ;	          ((players[current_better].run_level==next_round)
   ;	              && (players[current_better].current_bet==current_bet)) )
   ;	 {
   ;	   current_better++;
   ;	
	inc	word ptr CODE:_current_better
   ;	
   ;	   if (current_better>=num_players)
   ;	
	mov	ax,word ptr CODE:_current_better
	cmp	ax,word ptr CODE:_num_players
	jl	short @19@534
   ;	
   ;	    current_better=0;
   ;	
	mov	word ptr CODE:_current_better,0
@19@534:
	mov	ax,word ptr CODE:_current_better
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+72],0
	jne	short @19@478
	mov	ax,word ptr CODE:_current_better
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],0
	je	short @19@478
	mov	ax,word ptr CODE:_current_better
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+66]
	cmp	ax,word ptr [bp+4]
	jne	short @19@674
	mov	ax,word ptr CODE:_current_better
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+64]
	mov	dx,word ptr cs:_players[bx+62]
	cmp	ax,word ptr CODE:_current_bet+2
	jne	short @19@674
	cmp	dx,word ptr CODE:_current_bet
	je	short @19@478
@19@674:
   ;	
   ;	 }
   ;	
   ;	   if (current_better>=num_players)
   ;	
	mov	ax,word ptr CODE:_current_better
	cmp	ax,word ptr CODE:_num_players
	jl	short @19@730
   ;	
   ;	    current_better=0;
   ;	
	mov	word ptr CODE:_current_better,0
@19@730:
   ;	
   ;	 count = current_better;
   ;	
	mov	si,word ptr CODE:_current_better
   ;	
   ;	
   ;	 if (!players[count].warning_level)
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+68],0
	jne	short @19@926
   ;	
   ;	  {
   ;	   poker_timer = dans_counter;
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	mov	ax,word ptr es:[bx]
	mov	word ptr CODE:_poker_timer+2,0
	mov	word ptr CODE:_poker_timer,ax
   ;	
   ;	
   ;	   if ((count==first_better) && (!first_bets_in))
   ;	
	cmp	si,word ptr CODE:_first_better
	jne	short @19@842
	cmp	word ptr [bp-2],0
	jne	short @19@842
   ;	
   ;	    sprintf(s,"%s|*r1 your first to place a bet.",(char far *)players[count].handle);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+2027
	jmp	short @19@870
@19@842:
   ;	
   ;	   else
   ;	    sprintf(s,"%s|*r1 your next to place a bet.",(char far *)players[count].handle);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+2061
@19@870:
	push	ax
	push	ss
	lea	ax,word ptr [bp-154]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	
   ;	   poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-154]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	   players[count].warning_level=1;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+68],1
   ;	
   ;	  }
   ;	
	jmp	@19@1206
@19@926:
   ;	
   ;	 else
   ;	  {
   ;	   if (dans_counter<poker_timer)
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	mov	ax,word ptr es:[bx]
	xor	dx,dx
	cmp	dx,word ptr CODE:_poker_timer+2
	ja	short @19@1038
	jb	short @19@1010
	cmp	ax,word ptr CODE:_poker_timer
	jae	short @19@1038
@19@1010:
   ;	
   ;	     elapsed_time = 0xFFFF - (poker_timer - dans_counter);
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	mov	ax,word ptr CODE:_poker_timer
	sub	ax,word ptr es:[bx]
	mov	dx,-1
	sub	dx,ax
	mov	word ptr [bp-4],dx
	jmp	short @19@1066
@19@1038:
   ;	
   ;	   else
   ;	     elapsed_time = dans_counter - poker_timer;
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	mov	ax,word ptr es:[bx]
	sub	ax,word ptr CODE:_poker_timer
	mov	word ptr [bp-4],ax
@19@1066:
   ;	
   ;	
   ;	   if ((elapsed_time/18)>120)
   ;	
	mov	ax,word ptr [bp-4]
	mov	bx,18
	xor	dx,dx
	div	bx
	cmp	ax,120
	jbe	short @19@1122
   ;	
   ;	    {
   ;	         sprintf(s,"%s|*r1 is being kicked out for not responding",(char far *)players[count].handle);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+2094
	push	ax
	push	ss
	lea	ax,word ptr [bp-154]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	         poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-154]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	         deregister_player(players[count].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	call	near ptr CODE:_deregister_player
	pop	cx
@19@1122:
   ;	
   ;	    }
   ;	
   ;	   if (((elapsed_time/18)>60) && (players[count].warning_level<2))
   ;	
	mov	ax,word ptr [bp-4]
	mov	bx,18
	xor	dx,dx
	div	bx
	cmp	ax,60
	jbe	short @19@1206
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+68],2
	jge	short @19@1206
   ;	
   ;	    {
   ;	     players[count].warning_level=2;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+68],2
   ;	
   ;	     private_poker_message("Please Bet or Quit the game (you have 60 seconds)",players[count].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	push	cs
	mov	ax,offset CODE:s@+2140
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
@19@1206:
   ;	
   ;	    }
   ;	
   ;	  }
   ;	
   ;	  return 0;
   ;	
	xor	ax,ax
	jmp	@19@366
@19@1234:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_process_bets	endp
   ;	
   ;	int process_first_bets(void)
   ;	
	assume	cs:CODE
_process_first_bets	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    return (process_bets(DISCARD_ROUND));
   ;	
	mov	ax,4
	push	ax
	call	near ptr CODE:_process_bets
	pop	cx
	jmp	short @20@58
@20@58:
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_process_first_bets	endp
   ;	
   ;	int process_discard_round(void)
   ;	
	assume	cs:CODE
_process_discard_round	proc	near
	push	bp
	mov	bp,sp
	sub	sp,152
	push	si
	push	di
   ;	
   ;	{
   ;	    int count;
   ;	    int waiting_for_players=0;
   ;	
	mov	word ptr [bp-2],0
   ;	
   ;	    unsigned int time_elapsed;
   ;	    char s[150];
   ;	
   ;	    for(count=0;count<num_players;count++)
   ;	
	xor	si,si
	jmp	@21@422
@21@58:
   ;	
   ;	     {
   ;	
   ;	        if ((players[count].run_level==DISCARD_ROUND))
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],4
	je	@@12
	jmp	@21@394
@@12:
   ;	
   ;	         {
   ;	             waiting_for_players=1;
   ;	
	mov	word ptr [bp-2],1
   ;	
   ;	             poker_timer=dans_counter;
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	mov	ax,word ptr es:[bx]
	mov	word ptr CODE:_poker_timer+2,0
	mov	word ptr CODE:_poker_timer,ax
   ;	
   ;	
   ;	             if (dans_counter<poker_timer)
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	mov	ax,word ptr es:[bx]
	xor	dx,dx
	cmp	dx,word ptr CODE:_poker_timer+2
	ja	short @21@198
	jb	short @21@170
	cmp	ax,word ptr CODE:_poker_timer
	jae	short @21@198
@21@170:
   ;	
   ;	               time_elapsed =  (unsigned int)0xFFFF - (poker_timer-dans_counter);
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	mov	ax,word ptr CODE:_poker_timer
	sub	ax,word ptr es:[bx]
	mov	dx,-1
	sub	dx,ax
	mov	di,dx
	jmp	short @21@226
@21@198:
   ;	
   ;	             else
   ;	               time_elapsed = dans_counter - poker_timer;
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	mov	ax,word ptr es:[bx]
	sub	ax,word ptr CODE:_poker_timer
	mov	di,ax
@21@226:
   ;	
   ;	
   ;	
   ;	             if (((time_elapsed/18) > 60) && !players[count].warning_level)
   ;	
	mov	ax,di
	mov	bx,18
	xor	dx,dx
	div	bx
	cmp	ax,60
	jbe	short @21@310
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+68],0
	jne	short @21@310
   ;	
   ;	               {
   ;	                 private_poker_message("Please Discard or Quit the game (you have 45 seconds)",players[count].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	push	cs
	mov	ax,offset CODE:s@+2190
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
   ;	
   ;	                 players[count].warning_level=1;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+68],1
@21@310:
   ;	
   ;	               }
   ;	
   ;	             if (((time_elapsed/18) > 105) && (players[count].warning_level>0))
   ;	
	mov	ax,di
	mov	bx,18
	xor	dx,dx
	div	bx
	cmp	ax,105
	jbe	short @21@394
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+68],0
	jle	short @21@394
   ;	
   ;	               {
   ;	                 sprintf(s,"%s|*r1 is being kicked out for not responding",(char far *)players[count].handle);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+2244
	push	ax
	push	ss
	lea	ax,word ptr [bp-152]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	                 poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-152]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	                 deregister_player(players[count].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	call	near ptr CODE:_deregister_player
	pop	cx
@21@394:
	inc	si
@21@422:
	cmp	si,word ptr CODE:_num_players
	jge	@@13
	jmp	@21@58
@@13:
   ;	
   ;	               }
   ;	          }
   ;	     }
   ;	
   ;	    if (waiting_for_players)
   ;	
	cmp	word ptr [bp-2],0
	je	short @21@534
   ;	
   ;	       return 0;
   ;	
	xor	ax,ax
@21@506:
	jmp	short @21@562
@21@534:
   ;	
   ;	
   ;	
   ;	    return 1;    /* continue to next round */
   ;	
	mov	ax,1
	jmp	short @21@506
@21@562:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_process_discard_round	endp
   ;	
   ;	void replace_discards(void)
   ;	
	assume	cs:CODE
_replace_discards	proc	near
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
   ;	
   ;	{
   ;	  int num_cards_in_hand=0;
   ;	
	mov	word ptr [bp-2],0
   ;	
   ;	  int count_player=0;
   ;	
	xor	di,di
   ;	
   ;	  int temp;
   ;	  int tries;
   ;	
   ;	  g_delay(10);
   ;	
	mov	ax,10
	push	ax
	call	near ptr CODE:_g_delay
	pop	cx
   ;	
   ;	
   ;	  for (num_cards_in_hand=0;num_cards_in_hand<5;num_cards_in_hand++)
   ;	
	mov	word ptr [bp-2],0
	jmp	@22@618
@22@58:
   ;	
   ;	   {
   ;	
   ;	    for (count_player=0;count_player<num_players;count_player++)
   ;	
	xor	di,di
	jmp	@22@562
@22@86:
   ;	
   ;	    if (players[count_player].cards_in_hand[num_cards_in_hand]==NO_CARD)
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,word ptr [bp-2]
	mov	bx,ax
	cmp	byte ptr cs:_players[bx+56],255
	je	@@14
	jmp	@22@534
@@14:
   ;	
   ;	        {
   ;	           tries=0;
   ;	
	mov	word ptr [bp-4],0
@22@142:
   ;	
   ;	
   ;	           do
   ;	           {
   ;	            temp = rand() % NUM_CARDS;
   ;	
	call	near ptr CODE:_rand
	mov	bx,52
	cwd	
	idiv	bx
	mov	si,dx
   ;	
   ;	
   ;	            if ((tries % 30) == 29)
   ;	
	mov	ax,word ptr [bp-4]
	mov	bx,30
	cwd	
	idiv	bx
	cmp	dx,29
	jne	short @22@198
   ;	
   ;	              srand((unsigned) dans_counter);
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	push	word ptr es:[bx]
	call	near ptr CODE:_srand
	pop	cx
@22@198:
   ;	
   ;	
   ;	           }  while ((card_deck[temp]==NO_CARD) && (tries++)<140);
   ;	
	cmp	byte ptr cs:_card_deck[si],255
	jne	short @22@254
	mov	ax,word ptr [bp-4]
	inc	word ptr [bp-4]
	cmp	ax,140
	jl	short @22@142
@22@254:
   ;	
   ;	
   ;	           if (card_deck[temp]==NO_CARD)
   ;	
	cmp	byte ptr cs:_card_deck[si],255
	jne	short @22@506
   ;	
   ;	             { poker_message("Having trouble dealing cards (random problems)");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2290
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	               tries=0;
   ;	
	mov	word ptr [bp-4],0
@22@310:
   ;	
   ;	               do
   ;	               { temp++;
   ;	
	inc	si
   ;	
   ;	                 if (temp>=52) temp=0;
   ;	
	cmp	si,52
	jl	short @22@366
	xor	si,si
@22@366:
   ;	
   ;	                 tries++;
   ;	
	inc	word ptr [bp-4]
   ;	
   ;	               } while ((card_deck[temp]==NO_CARD) && tries<60);
   ;	
	cmp	byte ptr cs:_card_deck[si],255
	jne	short @22@450
	cmp	word ptr [bp-4],60
	jl	short @22@310
@22@450:
   ;	
   ;	
   ;	               if (card_deck[temp]==NO_CARD)
   ;	
	cmp	byte ptr cs:_card_deck[si],255
	jne	short @22@506
   ;	
   ;	                {
   ;	                 poker_message("The Card Deck is corrupted, ending game.");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2337
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	                 game_run_level = GAME_COMPLETE;
   ;	
	mov	word ptr CODE:_game_run_level,6
@22@506:
   ;	
   ;	                }
   ;	
   ;	             }
   ;	
   ;	
   ;	           players[count_player].cards_in_hand[num_cards_in_hand] = card_deck[temp];
   ;	
	mov	al,byte ptr cs:_card_deck[si]
	push	ax
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,word ptr [bp-2]
	mov	bx,ax
	pop	ax
	mov	byte ptr cs:_players[bx+56],al
   ;	
   ;	
   ;	           card_deck[temp] = NO_CARD;
   ;	
	mov	byte ptr cs:_card_deck[si],255
   ;	
   ;	
   ;	           players[count_player].have_cards=1;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	byte ptr cs:_players[bx+61],1
@22@534:
	inc	di
@22@562:
	cmp	di,word ptr CODE:_num_players
	jge	@@15
	jmp	@22@86
@@15:
	inc	word ptr [bp-2]
@22@618:
	cmp	word ptr [bp-2],5
	jge	@@16
	jmp	@22@58
@@16:
   ;	
   ;	
   ;	        }
   ;	
   ;	   }
   ;	   g_delay(10);
   ;	
	mov	ax,10
	push	ax
	call	near ptr CODE:_g_delay
	pop	cx
   ;	
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_replace_discards	endp
   ;	
   ;	int process_final_bets(void)
   ;	
	assume	cs:CODE
_process_final_bets	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    return (process_bets(GAME_COMPLETE));
   ;	
	mov	ax,6
	push	ax
	call	near ptr CODE:_process_bets
	pop	cx
	jmp	short @23@58
@23@58:
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_process_final_bets	endp
   ;	
   ;	int has_card_with_value(int player_no,signed char value)
   ;	
	assume	cs:CODE
_has_card_with_value	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	  int count;
   ;	
   ;	  if ((value<0) || (value>12))
   ;	
	cmp	byte ptr [bp+6],0
	jl	short @24@86
	cmp	byte ptr [bp+6],12
	jle	short @24@142
@24@86:
   ;	
   ;	   return 0;
   ;	
	xor	ax,ax
@24@114:
	jmp	short @24@310
@24@142:
   ;	
   ;	
   ;	  for(count=0;count<5;count++)
   ;	
	xor	cx,cx
	jmp	short @24@254
@24@170:
   ;	
   ;	    if ((players[player_no].cards_in_hand[count] % 13) == value)
   ;	
	mov	ax,word ptr [bp+4]
	mov	dx,76
	imul	dx
	add	ax,cx
	mov	bx,ax
	mov	al,byte ptr cs:_players[bx+56]
	cbw	
	mov	bx,13
	cwd	
	idiv	bx
	mov	al,byte ptr [bp+6]
	cbw	
	cmp	dx,ax
	jne	short @24@226
   ;	
   ;	      return 1;
   ;	
	mov	ax,1
	jmp	short @24@114
@24@226:
	inc	cx
@24@254:
	cmp	cx,5
	jl	short @24@170
	jmp	short @24@86
@24@310:
   ;	
   ;	
   ;	  return 0;
   ;	}
   ;	
	pop	bp
	ret	
_has_card_with_value	endp
   ;	
   ;	int find_high_card(int player_no)
   ;	
	assume	cs:CODE
_find_high_card	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
	mov	di,word ptr [bp+4]
   ;	
   ;	{
   ;	   int count;
   ;	   int high_card=NO_CARD;
   ;	
	mov	word ptr [bp-2],-1
   ;	
   ;	
   ;	   if ((player_no<0) || (player_no>=num_players))
   ;	
	or	di,di
	jl	short @25@86
	cmp	di,word ptr CODE:_num_players
	jl	short @25@142
@25@86:
   ;	
   ;	     return NO_CARD;
   ;	
	mov	ax,-1
@25@114:
	jmp	short @25@310
@25@142:
   ;	
   ;	
   ;	   for (count=0;count<5;count++)
   ;	
	xor	si,si
	jmp	short @25@254
@25@170:
   ;	
   ;	      if (card_less_than(high_card,players[player_no].cards_in_hand[count]))
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,si
	mov	bx,ax
	mov	al,byte ptr cs:_players[bx+56]
	cbw	
	push	ax
	push	word ptr [bp-2]
	call	near ptr CODE:_card_less_than
	pop	cx
	pop	cx
	or	ax,ax
	je	short @25@226
   ;	
   ;	        high_card = players[player_no].cards_in_hand[count];
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,si
	mov	bx,ax
	mov	al,byte ptr cs:_players[bx+56]
	cbw	
	mov	word ptr [bp-2],ax
@25@226:
	inc	si
@25@254:
	cmp	si,5
	jl	short @25@170
   ;	
   ;	
   ;	   return (high_card);
   ;	
	mov	ax,word ptr [bp-2]
	jmp	short @25@114
@25@310:
   ;	
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_find_high_card	endp
   ;	
   ;	int is_a_flush(int player_no)
   ;	
	assume	cs:CODE
_is_a_flush	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  signed char suit;
   ;	  int count;
   ;	
   ;	  suit = (players[player_no].cards_in_hand[0] / 13);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	al,byte ptr cs:_players[bx+56]
	cbw	
	mov	bx,13
	cwd	
	idiv	bx
	mov	byte ptr [bp-1],al
   ;	
   ;	
   ;	  for (count=1;count<5;count++)
   ;	
	mov	cx,1
	jmp	short @26@170
@26@58:
   ;	
   ;	     if (suit != (players[player_no].cards_in_hand[count] / 13))
   ;	
	mov	al,byte ptr [bp-1]
	cbw	
	push	ax
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,cx
	mov	bx,ax
	mov	al,byte ptr cs:_players[bx+56]
	cbw	
	mov	bx,13
	cwd	
	idiv	bx
	pop	dx
	cmp	dx,ax
	je	short @26@142
   ;	
   ;	       return NO_CARD;
   ;	
	mov	ax,-1
@26@114:
	jmp	short @26@226
@26@142:
	inc	cx
@26@170:
	cmp	cx,5
	jl	short @26@58
   ;	
   ;	
   ;	  return suit;
   ;	
	mov	al,byte ptr [bp-1]
	cbw	
	jmp	short @26@114
@26@226:
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_is_a_flush	endp
   ;	
   ;	int is_a_straight(int player_no)
   ;	
	assume	cs:CODE
_is_a_straight	proc	near
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  signed char high_card_value = (find_high_card(player_no) % 13);
   ;	
	push	si
	call	near ptr CODE:_find_high_card
	pop	cx
	mov	bx,13
	cwd	
	idiv	bx
	mov	byte ptr [bp-1],dl
   ;	
   ;	  int count;
   ;	
   ;	  if (high_card_value<5)
   ;	
	cmp	byte ptr [bp-1],5
	jge	short @27@114
@27@58:
   ;	
   ;	    return NO_CARD;
   ;	
	mov	ax,-1
@27@86:
	jmp	short @27@282
@27@114:
   ;	
   ;	
   ;	
   ;	  for (count=1;count<5;count++)
   ;	
	mov	word ptr [bp-4],1
	jmp	short @27@226
@27@142:
   ;	
   ;	    if (!has_card_with_value(player_no,(high_card_value-count)))
   ;	
	mov	al,byte ptr [bp-1]
	sub	al,byte ptr [bp-4]
	push	ax
	push	si
	call	near ptr CODE:_has_card_with_value
	pop	cx
	pop	cx
	or	ax,ax
	jne	short @27@198
	jmp	short @27@58
@27@198:
	inc	word ptr [bp-4]
@27@226:
	cmp	word ptr [bp-4],5
	jl	short @27@142
   ;	
   ;	      return NO_CARD;
   ;	
   ;	  return high_card_value;
   ;	
	mov	al,byte ptr [bp-1]
	cbw	
	jmp	short @27@86
@27@282:
   ;	
   ;	
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_is_a_straight	endp
   ;	
   ;	int n_of_a_kind(int player_no,int n,int not_of_value)
   ;	
	assume	cs:CODE
_n_of_a_kind	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
   ;	
   ;	{
   ;	    int count,count2;
   ;	    int test_value;
   ;	    int num_of_this_kind;
   ;	
   ;	    for (count=0;count<(6-n);count++)
   ;	
	xor	cx,cx
	jmp	short @28@338
@28@58:
   ;	
   ;	     if ((test_value = (players[player_no].cards_in_hand[count] % 13)) != not_of_value)
   ;	
	mov	ax,word ptr [bp+4]
	mov	dx,76
	imul	dx
	add	ax,cx
	mov	bx,ax
	mov	al,byte ptr cs:_players[bx+56]
	cbw	
	mov	bx,13
	cwd	
	idiv	bx
	mov	word ptr [bp-2],dx
	mov	ax,dx
	cmp	ax,word ptr [bp+8]
	je	short @28@310
   ;	
   ;	        {
   ;	         num_of_this_kind=1;
   ;	
	mov	di,1
   ;	
   ;	
   ;	         for (count2=count+1;count2<5;count2++)
   ;	
	mov	ax,cx
	inc	ax
	mov	si,ax
	jmp	short @28@198
@28@114:
   ;	
   ;	            if (test_value == (players[player_no].cards_in_hand[count2] % 13))
   ;	
	mov	ax,word ptr [bp+4]
	mov	dx,76
	imul	dx
	add	ax,si
	mov	bx,ax
	mov	al,byte ptr cs:_players[bx+56]
	cbw	
	mov	bx,13
	cwd	
	idiv	bx
	cmp	dx,word ptr [bp-2]
	jne	short @28@170
   ;	
   ;	              num_of_this_kind++;
   ;	
	inc	di
@28@170:
	inc	si
@28@198:
	cmp	si,5
	jl	short @28@114
   ;	
   ;	
   ;	         if (num_of_this_kind==n)
   ;	
	cmp	di,word ptr [bp+6]
	jne	short @28@310
   ;	
   ;	           return  (test_value);
   ;	
	mov	ax,word ptr [bp-2]
@28@282:
	jmp	short @28@394
@28@310:
	inc	cx
@28@338:
	mov	ax,6
	sub	ax,word ptr [bp+6]
	cmp	ax,cx
	jg	short @28@58
   ;	
   ;	
   ;	        }
   ;	
   ;	    return NO_CARD;
   ;	
	mov	ax,-1
	jmp	short @28@282
@28@394:
   ;	
   ;	
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_n_of_a_kind	endp
   ;	
   ;	int card_less_than(int card,int card2)
   ;	
	assume	cs:CODE
_card_less_than	proc	near
	push	bp
	mov	bp,sp
	sub	sp,6
	push	si
	push	di
	mov	cx,word ptr [bp+4]
	mov	si,word ptr [bp+6]
   ;	
   ;	{
   ;	    int value  = card  % 13;
   ;	
	mov	ax,cx
	mov	bx,13
	cwd	
	idiv	bx
	mov	di,dx
   ;	
   ;	    int value2 = card2 % 13;
   ;	
	mov	ax,si
	mov	bx,13
	cwd	
	idiv	bx
	mov	word ptr [bp-2],dx
   ;	
   ;	    int suit   = card  / 13;
   ;	
	mov	ax,cx
	mov	bx,13
	cwd	
	idiv	bx
	mov	word ptr [bp-4],ax
   ;	
   ;	    int suit2  = card2 / 13;
   ;	
	mov	ax,si
	mov	bx,13
	cwd	
	idiv	bx
	mov	word ptr [bp-6],ax
   ;	
   ;	
   ;	    if (value<value2)
   ;	
	cmp	di,word ptr [bp-2]
	jge	short @29@114
@29@58:
   ;	
   ;	     return 1;
   ;	
	mov	ax,1
@29@86:
	jmp	short @29@282
@29@114:
   ;	
   ;	    if (value>value2)
   ;	
	cmp	di,word ptr [bp-2]
	jle	short @29@170
	jmp	short @29@254
@29@170:
   ;	
   ;	     return 0;
   ;	     
   ;	    if (suit<suit2)
   ;	
	mov	ax,word ptr [bp-4]
	cmp	ax,word ptr [bp-6]
	jge	short @29@254
	jmp	short @29@58
   ;	
   ;	     return 1;
   ;	
	jmp	short @29@282
@29@254:
   ;	
   ;	    else
   ;	     return 0;
   ;	
	xor	ax,ax
	jmp	short @29@86
@29@282:
   ;	
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_card_less_than	endp
_winning_hand	label	word
	db	1
	db	1
	db	255
	db	255
	db	255
	db	255
	db	1
	db	255
	db	255
	db	1
	db	255
	db	255
	db	255
	db	255
	db	255
	db	255
	db	255
	db	1
	db	1
	db	255
	db	255
	db	255
	db	1
	db	255
	db	255
	db	255
	db	255
	db	1
	db	1
	db	255
	db	255
	db	255
	db	255
	db	255
	db	1
	db	255
	db	255
	db	255
	db	1
	db	255
	db	255
	db	255
	db	255
	db	255
	db	255
	db	255
	db	1
	db	1
	db	255
	db	255
	db	255
	db	255
	db	255
	db	1
	db	255
	db	255
	db	255
	db	255
	db	255
	db	255
	db	255
	db	255
	db	1
   ;	
   ;	int player_with_hand(struct hand_classification_struct *everyone,struct hand_classification_struct *winning_hand)
   ;	
	assume	cs:CODE
_player_with_hand	proc	near
	push	bp
	mov	bp,sp
	sub	sp,12
	push	si
	push	di
   ;	
   ;	{
   ;	  signed char high_card = NO_CARD;
   ;	
	mov	byte ptr [bp-1],255
   ;	
   ;	  int high_player = NO_CARD;
   ;	
	mov	di,-1
   ;	
   ;	  struct hand_classification_struct *this_player;
   ;	  struct hand_classification_struct  *the_high_player;
   ;	  int has_hand;
   ;	  int count;
   ;	
   ;	
   ;	  for (count=0;count<num_players;count++)
   ;	
	mov	word ptr [bp-12],0
	jmp	@30@1542
@30@58:
   ;	
   ;	    if ((!players[count].fold) && (players[count].run_level==GAME_COMPLETE))
   ;	
	mov	ax,word ptr [bp-12]
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+72],0
	je	@@17
	jmp	@30@1514
@@17:
	mov	ax,word ptr [bp-12]
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],6
	je	@@18
	jmp	@30@1514
@@18:
   ;	
   ;	     {
   ;	        has_hand=1;
   ;	
	mov	si,1
   ;	
   ;	
   ;	        this_player = &everyone[count];
   ;	
	mov	ax,word ptr [bp-12]
	mov	dx,7
	imul	dx
	mov	dx,word ptr [bp+6]
	mov	bx,word ptr [bp+4]
	add	bx,ax
	mov	word ptr [bp-4],dx
	mov	word ptr [bp-6],bx
   ;	
   ;	        if (high_player!=NO_CARD)
   ;	
	cmp	di,-1
	je	short @30@170
   ;	
   ;	          the_high_player = &everyone[high_player];
   ;	
	mov	ax,di
	mov	dx,7
	imul	dx
	mov	dx,word ptr [bp+6]
	mov	bx,word ptr [bp+4]
	add	bx,ax
	mov	word ptr [bp-8],dx
	mov	word ptr [bp-10],bx
@30@170:
   ;	
   ;	
   ;	        if (winning_hand->straight!=NO_CARD)
   ;	
	les	bx,dword ptr [bp+8]
	cmp	byte ptr es:[bx],255
	je	short @30@338
   ;	
   ;	         {
   ;	           if (this_player->straight==NO_CARD)
   ;	
	les	bx,dword ptr [bp-6]
	cmp	byte ptr es:[bx],255
	jne	short @30@254
   ;	
   ;	            has_hand=0;
   ;	
	xor	si,si
	jmp	short @30@338
@30@254:
   ;	
   ;	           else
   ;	            {
   ;	               if (high_player!=NO_CARD)
   ;	
	cmp	di,-1
	je	short @30@338
   ;	
   ;	                    if (card_less_than(this_player->straight,the_high_player->straight))
   ;	
	les	bx,dword ptr [bp-10]
	mov	al,byte ptr es:[bx]
	cbw	
	push	ax
	les	bx,dword ptr [bp-6]
	mov	al,byte ptr es:[bx]
	cbw	
	push	ax
	call	near ptr CODE:_card_less_than
	pop	cx
	pop	cx
	or	ax,ax
	je	short @30@338
   ;	
   ;	                       has_hand=0;
   ;	
	xor	si,si
@30@338:
   ;	
   ;	            }
   ;	
   ;	         }
   ;	
   ;	        if ((winning_hand->flush!=NO_CARD) && has_hand)
   ;	
	les	bx,dword ptr [bp+8]
	cmp	byte ptr es:[bx+1],255
	je	short @30@534
	or	si,si
	je	short @30@534
   ;	
   ;	         {
   ;	           if (this_player->flush==NO_CARD)
   ;	
	les	bx,dword ptr [bp-6]
	cmp	byte ptr es:[bx+1],255
	jne	short @30@450
   ;	
   ;	            has_hand=0;
   ;	
	xor	si,si
	jmp	short @30@534
@30@450:
   ;	
   ;	           else
   ;	            {
   ;	               if (high_player!=NO_CARD)
   ;	
	cmp	di,-1
	je	short @30@534
   ;	
   ;	                    if (this_player->flush < the_high_player->flush)
   ;	
	les	bx,dword ptr [bp-6]
	mov	al,byte ptr es:[bx+1]
	les	bx,dword ptr [bp-10]
	cmp	al,byte ptr es:[bx+1]
	jge	short @30@534
   ;	
   ;	                       has_hand=0;
   ;	
	xor	si,si
@30@534:
   ;	
   ;	            }
   ;	         }
   ;	
   ;	        if ((winning_hand->four_of_a_kind!=NO_CARD)  && has_hand)
   ;	
	les	bx,dword ptr [bp+8]
	cmp	byte ptr es:[bx+2],255
	je	short @30@730
	or	si,si
	je	short @30@730
   ;	
   ;	         {
   ;	           if (this_player->four_of_a_kind==NO_CARD)
   ;	
	les	bx,dword ptr [bp-6]
	cmp	byte ptr es:[bx+2],255
	jne	short @30@646
   ;	
   ;	            has_hand=0;
   ;	
	xor	si,si
	jmp	short @30@730
@30@646:
   ;	
   ;	           else
   ;	            {
   ;	               if (high_player!=NO_CARD)
   ;	
	cmp	di,-1
	je	short @30@730
   ;	
   ;	                    if (this_player->four_of_a_kind < the_high_player->four_of_a_kind)
   ;	
	les	bx,dword ptr [bp-6]
	mov	al,byte ptr es:[bx+2]
	les	bx,dword ptr [bp-10]
	cmp	al,byte ptr es:[bx+2]
	jge	short @30@730
   ;	
   ;	                       has_hand=0;
   ;	
	xor	si,si
@30@730:
   ;	
   ;	            }
   ;	
   ;	
   ;	         }
   ;	
   ;	        if ((winning_hand->three_of_a_kind!=NO_CARD) && has_hand)
   ;	
	les	bx,dword ptr [bp+8]
	cmp	byte ptr es:[bx+3],255
	je	short @30@926
	or	si,si
	je	short @30@926
   ;	
   ;	         {
   ;	           if (this_player->three_of_a_kind==NO_CARD)
   ;	
	les	bx,dword ptr [bp-6]
	cmp	byte ptr es:[bx+3],255
	jne	short @30@842
   ;	
   ;	            has_hand=0;
   ;	
	xor	si,si
	jmp	short @30@926
@30@842:
   ;	
   ;	           else
   ;	            {
   ;	               if (high_player!=NO_CARD)
   ;	
	cmp	di,-1
	je	short @30@926
   ;	
   ;	                    if (this_player->three_of_a_kind < the_high_player->three_of_a_kind)
   ;	
	les	bx,dword ptr [bp-6]
	mov	al,byte ptr es:[bx+3]
	les	bx,dword ptr [bp-10]
	cmp	al,byte ptr es:[bx+3]
	jge	short @30@926
   ;	
   ;	                       has_hand=0;
   ;	
	xor	si,si
@30@926:
   ;	
   ;	            }
   ;	
   ;	         }
   ;	
   ;	        if ((winning_hand->two_of_a_kind!=NO_CARD) && has_hand)
   ;	
	les	bx,dword ptr [bp+8]
	cmp	byte ptr es:[bx+4],255
	je	short @30@1122
	or	si,si
	je	short @30@1122
   ;	
   ;	         {
   ;	           if (this_player->two_of_a_kind==NO_CARD)
   ;	
	les	bx,dword ptr [bp-6]
	cmp	byte ptr es:[bx+4],255
	jne	short @30@1038
   ;	
   ;	            has_hand=0;
   ;	
	xor	si,si
	jmp	short @30@1122
@30@1038:
   ;	
   ;	           else
   ;	            {
   ;	               if (high_player!=NO_CARD)
   ;	
	cmp	di,-1
	je	short @30@1122
   ;	
   ;	                    if (this_player->two_of_a_kind < the_high_player->two_of_a_kind)
   ;	
	les	bx,dword ptr [bp-6]
	mov	al,byte ptr es:[bx+4]
	les	bx,dword ptr [bp-10]
	cmp	al,byte ptr es:[bx+4]
	jge	short @30@1122
   ;	
   ;	                       has_hand=0;
   ;	
	xor	si,si
@30@1122:
   ;	
   ;	            }
   ;	
   ;	         }
   ;	
   ;	        if ((winning_hand->two_of_a_kind2!=NO_CARD) && has_hand)
   ;	
	les	bx,dword ptr [bp+8]
	cmp	byte ptr es:[bx+5],255
	je	short @30@1318
	or	si,si
	je	short @30@1318
   ;	
   ;	         {
   ;	           if (this_player->two_of_a_kind2==NO_CARD)
   ;	
	les	bx,dword ptr [bp-6]
	cmp	byte ptr es:[bx+5],255
	jne	short @30@1234
   ;	
   ;	            has_hand=0;
   ;	
	xor	si,si
	jmp	short @30@1318
@30@1234:
   ;	
   ;	           else
   ;	            {
   ;	               if (high_player!=NO_CARD)
   ;	
	cmp	di,-1
	je	short @30@1318
   ;	
   ;	                    if (this_player->two_of_a_kind2 < the_high_player->two_of_a_kind2)
   ;	
	les	bx,dword ptr [bp-6]
	mov	al,byte ptr es:[bx+5]
	les	bx,dword ptr [bp-10]
	cmp	al,byte ptr es:[bx+5]
	jge	short @30@1318
   ;	
   ;	                       has_hand=0;
   ;	
	xor	si,si
@30@1318:
   ;	
   ;	            }
   ;	
   ;	         }
   ;	
   ;	        if ((winning_hand->high_card!=NO_CARD) && has_hand)
   ;	
	les	bx,dword ptr [bp+8]
	cmp	byte ptr es:[bx+6],255
	je	short @30@1458
	or	si,si
	je	short @30@1458
   ;	
   ;	         {
   ;	           if (high_player!=NO_CARD)
   ;	
	cmp	di,-1
	je	short @30@1458
   ;	
   ;	                if (card_less_than(this_player->high_card,the_high_player->high_card))
   ;	
	les	bx,dword ptr [bp-10]
	mov	al,byte ptr es:[bx+6]
	cbw	
	push	ax
	les	bx,dword ptr [bp-6]
	mov	al,byte ptr es:[bx+6]
	cbw	
	push	ax
	call	near ptr CODE:_card_less_than
	pop	cx
	pop	cx
	or	ax,ax
	je	short @30@1458
   ;	
   ;	                   has_hand=0;
   ;	
	xor	si,si
@30@1458:
   ;	
   ;	
   ;	         }
   ;	
   ;	         if ((has_hand))
   ;	
	or	si,si
	je	short @30@1514
   ;	
   ;	          {
   ;	            high_player = count;
   ;	
	mov	di,word ptr [bp-12]
   ;	
   ;	            high_card   = this_player->high_card;
   ;	
	les	bx,dword ptr [bp-6]
	mov	al,byte ptr es:[bx+6]
	mov	byte ptr [bp-1],al
@30@1514:
	inc	word ptr [bp-12]
@30@1542:
	mov	ax,word ptr [bp-12]
	cmp	ax,word ptr CODE:_num_players
	jge	@@19
	jmp	@30@58
@@19:
   ;	
   ;	           }
   ;	
   ;	
   ;	     }
   ;	
   ;	     return (high_player);
   ;	
	mov	ax,di
	jmp	short @30@1598
@30@1598:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_player_with_hand	endp
   ;	
   ;	void check_for_winner(void)
   ;	
	assume	cs:CODE
_check_for_winner	proc	near
	push	bp
	mov	bp,sp
	sub	sp,416
	push	si
	push	di
   ;	
   ;	{
   ;	  int count,count2;
   ;	  char s[250];
   ;	  char s2[50];
   ;	  char s3[70];
   ;	  int high_card=1;
   ;	
	mov	word ptr [bp-4],1
   ;	
   ;	  struct hand_classification_struct player_classification[MAX_PLAYERS];
   ;	  int winner=-1;
   ;	
	mov	di,-1
   ;	
   ;	
   ;	
   ;	  poker_message("Revealing the players cards:");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2378
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	
   ;	  for (count=0;count<num_players;count++)
   ;	
	xor	si,si
	jmp	@31@282
@31@58:
   ;	
   ;	    if ((!players[count].fold) && (players[count].run_level!=NOT_RUNNING))
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+72],0
	je	@@20
	jmp	@31@254
@@20:
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],0
	jne	@@21
	jmp	@31@254
@@21:
   ;	
   ;	      {
   ;	        sprintf(s,"%s|*r1 ",players[count].handle);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+2407
	push	ax
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	        for (count2=0;count2<5;count2++)
   ;	
	mov	word ptr [bp-2],0
	jmp	short @31@198
@31@142:
   ;	
   ;	          {
   ;	            sprintf(s2,"%2s-%s  ",
   ;	
   ;	
   ;	                         (char far *)short_card_names[((unsigned char) players[count].cards_in_hand[count2]) % 13],
   ;	                         (char far *)short_suit_names[((unsigned char) players[count].cards_in_hand[count2]) / 13]);
   ;	
	push	ds
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,word ptr [bp-2]
	mov	bx,ax
	mov	al,byte ptr cs:_players[bx+56]
	mov	ah,0
	mov	bx,13
	cwd	
	idiv	bx
	shl	ax,1
	mov	bx,ax
	push	word ptr cs:_short_suit_names[bx]
	push	ds
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,word ptr [bp-2]
	mov	bx,ax
	mov	al,byte ptr cs:_players[bx+56]
	mov	ah,0
	mov	bx,13
	cwd	
	idiv	bx
	shl	dx,1
	mov	bx,dx
	push	word ptr cs:_short_card_names[bx]
	push	cs
	mov	ax,offset CODE:s@+2415
	push	ax
	push	ss
	lea	ax,word ptr [bp-304]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,16
   ;	
   ;	            strcat((char far *)s,(char far *)s2);
   ;	
	push	ss
	lea	ax,word ptr [bp-304]
	push	ax
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_strcat
	add	sp,8
	inc	word ptr [bp-2]
@31@198:
	cmp	word ptr [bp-2],5
	jl	short @31@142
   ;	
   ;	          }
   ;	        poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
@31@254:
	inc	si
@31@282:
	cmp	si,word ptr CODE:_num_players
	jge	@@22
	jmp	@31@58
@@22:
   ;	
   ;	      }
   ;	
   ;	
   ;	  for (count=0;count<num_players;count++)
   ;	
	xor	si,si
	jmp	@31@506
@31@338:
   ;	
   ;	    if ((!players[count].fold) && (players[count].run_level!=NOT_RUNNING))
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+72],0
	je	@@23
	jmp	@31@478
@@23:
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],0
	jne	@@24
	jmp	@31@478
@@24:
   ;	
   ;	        {
   ;	          player_classification[count].straight        = is_a_straight(count);
   ;	
	push	si
	call	near ptr CODE:_is_a_straight
	pop	cx
	push	ax
	mov	ax,si
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-416]
	add	ax,dx
	mov	bx,ax
	pop	ax
	mov	byte ptr ss:[bx],al
   ;	
   ;	          player_classification[count].flush           = is_a_flush(count);
   ;	
	push	si
	call	near ptr CODE:_is_a_flush
	pop	cx
	push	ax
	mov	ax,si
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-415]
	add	ax,dx
	mov	bx,ax
	pop	ax
	mov	byte ptr ss:[bx],al
   ;	
   ;	          player_classification[count].high_card       = find_high_card(count);
   ;	
	push	si
	call	near ptr CODE:_find_high_card
	pop	cx
	push	ax
	mov	ax,si
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-410]
	add	ax,dx
	mov	bx,ax
	pop	ax
	mov	byte ptr ss:[bx],al
   ;	
   ;	          player_classification[count].four_of_a_kind  = n_of_a_kind(count,4,NO_CARD);
   ;	
	mov	ax,-1
	push	ax
	mov	ax,4
	push	ax
	push	si
	call	near ptr CODE:_n_of_a_kind
	add	sp,6
	push	ax
	mov	ax,si
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-414]
	add	ax,dx
	mov	bx,ax
	pop	ax
	mov	byte ptr ss:[bx],al
   ;	
   ;	          player_classification[count].three_of_a_kind = n_of_a_kind(count,3,NO_CARD);
   ;	
	mov	ax,-1
	push	ax
	mov	ax,3
	push	ax
	push	si
	call	near ptr CODE:_n_of_a_kind
	add	sp,6
	push	ax
	mov	ax,si
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-413]
	add	ax,dx
	mov	bx,ax
	pop	ax
	mov	byte ptr ss:[bx],al
   ;	
   ;	          player_classification[count].two_of_a_kind   = n_of_a_kind(count,2,player_classification[count].three_of_a_kind % 13);
   ;	
	mov	ax,si
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-413]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	cbw	
	mov	bx,13
	cwd	
	idiv	bx
	push	dx
	mov	ax,2
	push	ax
	push	si
	call	near ptr CODE:_n_of_a_kind
	add	sp,6
	push	ax
	mov	ax,si
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-412]
	add	ax,dx
	mov	bx,ax
	pop	ax
	mov	byte ptr ss:[bx],al
   ;	
   ;	          if (player_classification[count].three_of_a_kind == NO_CARD)
   ;	
	mov	ax,si
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-413]
	add	ax,dx
	mov	bx,ax
	cmp	byte ptr ss:[bx],255
	jne	short @31@450
   ;	
   ;	              player_classification[count].two_of_a_kind2  = n_of_a_kind(count,2,player_classification[count].two_of_a_kind % 13
   ;	
	mov	ax,si
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-412]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	cbw	
	mov	bx,13
	cwd	
	idiv	bx
	push	dx
	mov	ax,2
	push	ax
	push	si
	call	near ptr CODE:_n_of_a_kind
	add	sp,6
	push	ax
	mov	ax,si
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-411]
	add	ax,dx
	mov	bx,ax
	pop	ax
	mov	byte ptr ss:[bx],al
	jmp	short @31@478
@31@450:
   ;	
   ;	          else
   ;	              player_classification[count].two_of_a_kind2 = NO_CARD;
   ;	
	mov	ax,si
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-411]
	add	ax,dx
	mov	bx,ax
	mov	byte ptr ss:[bx],255
@31@478:
	inc	si
@31@506:
	cmp	si,word ptr CODE:_num_players
	jge	@@25
	jmp	@31@338
@@25:
   ;	
   ;	
   ;	        }
   ;	
   ;	  /* check for winning hands in order now */
   ;	  count=0;
   ;	
	xor	si,si
	jmp	short @31@590
@31@562:
   ;	
   ;	
   ;	  while ((winner = player_with_hand(player_classification,&winning_hand[count])) == NO_CARD)
   ;	   count++;
   ;	
	inc	si
@31@590:
	mov	ax,si
	mov	dx,7
	imul	dx
	add	ax,offset CODE:_winning_hand
	push	cs
	push	ax
	push	ss
	lea	ax,word ptr [bp-416]
	push	ax
	call	near ptr CODE:_player_with_hand
	add	sp,8
	mov	di,ax
	cmp	ax,-1
	je	short @31@562
   ;	
   ;	
   ;	  sprintf(s2,"%s|*r1 wins with ",players[winner].handle);
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+2424
	push	ax
	push	ss
	lea	ax,word ptr [bp-304]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	
   ;	  switch (count)
   ;	
	mov	bx,si
	cmp	bx,8
	jbe	@@26
	jmp	@31@1094
@@26:
	shl	bx,1
	jmp	word ptr cs:@31@C1522[bx]
@31@730:
   ;	
   ;	   {
   ;	     case STRAIGHT_FLUSH:
   ;	             if ((player_classification[winner].high_card % 13) == 12)
   ;	
	mov	ax,di
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-410]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	cbw	
	mov	bx,13
	cwd	
	idiv	bx
	cmp	dx,12
	jne	short @31@786
   ;	
   ;	              {
   ;	               strcpy(s,"a Royal Flush");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2442
	push	ax
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_strcpy
	add	sp,8
   ;	
   ;	               high_card=0;
   ;	
	mov	word ptr [bp-4],0
   ;	
   ;	              }
   ;	
	jmp	short @31@814
@31@786:
   ;	
   ;	             else
   ;	              strcpy(s,"a Straight Flush");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2456
	push	ax
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_strcpy
	add	sp,8
@31@814:
   ;	
   ;	
   ;	             break;
   ;	
	jmp	@31@1094
@31@842:
   ;	
   ;	     case STRAIGHT:
   ;	             strcpy(s,"a Straight");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2473
	push	ax
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_strcpy
	add	sp,8
   ;	
   ;	             break;
   ;	
	jmp	@31@1094
@31@870:
   ;	
   ;	     case FOUR_OF_A_KIND:
   ;	             high_card=0;
   ;	
	mov	word ptr [bp-4],0
   ;	
   ;	             sprintf(s,"Four %s",
   ;	
   ;	
   ;	                         (char far *)plural_long_card_names[((unsigned char) player_classification[winner].four_of_a_kind) % 13]
   ;	
	push	ds
	mov	ax,di
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-414]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	mov	ah,0
	mov	bx,13
	cwd	
	idiv	bx
	shl	dx,1
	mov	bx,dx
	push	word ptr cs:_plural_long_card_names[bx]
	push	cs
	mov	ax,offset CODE:s@+2484
	push	ax
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	             break;
   ;	
	jmp	@31@1094
@31@898:
   ;	
   ;	     case FLUSH:
   ;	             sprintf(s,"a %s Flush",
   ;	
   ;	
   ;	                         (char far *)long_suit_names[((unsigned char) player_classification[winner].flush) / 13]);
   ;	
	push	ds
	mov	ax,di
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-415]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	mov	ah,0
	mov	bx,13
	cwd	
	idiv	bx
	shl	ax,1
	mov	bx,ax
	push	word ptr cs:_long_suit_names[bx]
	push	cs
	mov	ax,offset CODE:s@+2492
	push	ax
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	             break;
   ;	
	jmp	@31@1094
@31@926:
   ;	
   ;	     case FULL_HOUSE:
   ;	             high_card=0;
   ;	
	mov	word ptr [bp-4],0
   ;	
   ;	             strcpy(s,"a Full House");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2503
	push	ax
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_strcpy
	add	sp,8
   ;	
   ;	             break;
   ;	
	jmp	@31@1094
@31@954:
   ;	
   ;	     case THREE_OF_A_KIND:
   ;	             high_card=0;
   ;	
	mov	word ptr [bp-4],0
   ;	
   ;	             sprintf(s,"Three %s",
   ;	
   ;	
   ;	                         (char far *)plural_long_card_names[((unsigned char) player_classification[winner].three_of_a_kind) % 13
   ;	
	push	ds
	mov	ax,di
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-413]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	mov	ah,0
	mov	bx,13
	cwd	
	idiv	bx
	shl	dx,1
	mov	bx,dx
	push	word ptr cs:_plural_long_card_names[bx]
	push	cs
	mov	ax,offset CODE:s@+2516
	push	ax
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	             break;
   ;	
	jmp	@31@1094
@31@982:
   ;	
   ;	     case DBL_TWO_OF_A_KIND:
   ;	             high_card=0;
   ;	
	mov	word ptr [bp-4],0
   ;	
   ;	             sprintf(s,"Two Pair, %s and %s",
   ;	
   ;	
   ;	                         (char far *)plural_long_card_names[((unsigned char) player_classification[winner].two_of_a_kind) % 13],
   ;	                         (char far *)plural_long_card_names[((unsigned char) player_classification[winner].two_of_a_kind2) % 13]
   ;	
	push	ds
	mov	ax,di
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-411]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	mov	ah,0
	mov	bx,13
	cwd	
	idiv	bx
	shl	dx,1
	mov	bx,dx
	push	word ptr cs:_plural_long_card_names[bx]
	push	ds
	mov	ax,di
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-412]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	mov	ah,0
	mov	bx,13
	cwd	
	idiv	bx
	shl	dx,1
	mov	bx,dx
	push	word ptr cs:_plural_long_card_names[bx]
	push	cs
	mov	ax,offset CODE:s@+2525
	jmp	@31@1066
@31@1010:
   ;	
   ;	             break;
   ;	     case TWO_OF_A_KIND:
   ;	             high_card=0;
   ;	
	mov	word ptr [bp-4],0
   ;	
   ;	             sprintf(s,"a Pair of %s",
   ;	
   ;	
   ;	                         (char far *)plural_long_card_names[((unsigned char) player_classification[winner].two_of_a_kind) % 13])
   ;	
	push	ds
	mov	ax,di
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-412]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	mov	ah,0
	mov	bx,13
	cwd	
	idiv	bx
	shl	dx,1
	mov	bx,dx
	push	word ptr cs:_plural_long_card_names[bx]
	push	cs
	mov	ax,offset CODE:s@+2545
	push	ax
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	             break;
   ;	
	jmp	short @31@1094
@31@1038:
   ;	
   ;	     case HIGH_CARD:
   ;	             high_card=0;
   ;	
	mov	word ptr [bp-4],0
   ;	
   ;	             sprintf(s,"a %s of %s high",
   ;	
   ;	
   ;	                         (char far *)long_card_names[((unsigned char) player_classification[winner].high_card) % 13],
   ;	                         (char far *)long_suit_names[((unsigned char) player_classification[winner].high_card) / 13]);
   ;	
	push	ds
	mov	ax,di
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-410]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	mov	ah,0
	mov	bx,13
	cwd	
	idiv	bx
	shl	ax,1
	mov	bx,ax
	push	word ptr cs:_long_suit_names[bx]
	push	ds
	mov	ax,di
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-410]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	mov	ah,0
	mov	bx,13
	cwd	
	idiv	bx
	shl	dx,1
	mov	bx,dx
	push	word ptr cs:_long_card_names[bx]
	push	cs
	mov	ax,offset CODE:s@+2558
@31@1066:
	push	ax
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,16
   ;	
   ;	             break;
   ;	
	jmp	short @31@1094
@31@1094:
   ;	
   ;	
   ;	
   ;	
   ;	   }
   ;	
   ;	
   ;	  if (high_card)
   ;	
	cmp	word ptr [bp-4],0
	je	short @31@1150
   ;	
   ;	     sprintf(s3,", %s-%s high.",
   ;	
   ;	
   ;	             (char far *)long_card_names[((unsigned char) player_classification[winner].high_card) % 13],
   ;	             (char far *)long_suit_names[((unsigned char) player_classification[winner].high_card) / 13]);
   ;	
	push	ds
	mov	ax,di
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-410]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	mov	ah,0
	mov	bx,13
	cwd	
	idiv	bx
	shl	ax,1
	mov	bx,ax
	push	word ptr cs:_long_suit_names[bx]
	push	ds
	mov	ax,di
	mov	dx,7
	imul	dx
	lea	dx,word ptr [bp-410]
	add	ax,dx
	mov	bx,ax
	mov	al,byte ptr ss:[bx]
	mov	ah,0
	mov	bx,13
	cwd	
	idiv	bx
	shl	dx,1
	mov	bx,dx
	push	word ptr cs:_long_card_names[bx]
	push	cs
	mov	ax,offset CODE:s@+2574
	push	ax
	push	ss
	lea	ax,word ptr [bp-374]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,16
	jmp	short @31@1178
@31@1150:
   ;	
   ;	  else
   ;	     strcpy(s3,".");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2588
	push	ax
	push	ss
	lea	ax,word ptr [bp-374]
	push	ax
	call	near ptr CODE:_strcpy
	add	sp,8
@31@1178:
   ;	
   ;	
   ;	
   ;	
   ;	   aput_append_into_buffer(server, poker_channel, 0,
   ;	
   ;	
   ;	                 poker_channel, tswitch, 0, 4, public_poker_header,s2,s,s3);
   ;	
	push	ss
	lea	ax,word ptr [bp-374]
	push	ax
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	push	ss
	lea	ax,word ptr [bp-304]
	push	ax
	push	cs
	mov	ax,offset CODE:_public_poker_header
	push	ax
	mov	ax,4
	push	ax
	xor	ax,ax
	push	ax
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+220]
	push	word ptr es:[bx]
	push	word ptr CODE:_poker_channel
	xor	ax,ax
	push	ax
	push	word ptr CODE:_poker_channel
	push	word ptr CODE:_server
	call	near ptr CODE:_aput_append_into_buffer
	add	sp,30
   ;	
   ;	
   ;	
   ;	  if (poker_pot)
   ;	
	mov	ax,word ptr CODE:_poker_pot
	or	ax,word ptr CODE:_poker_pot+2
	je	short @31@1234
   ;	
   ;	   sprintf(s,"There was %lu in the pot.",poker_pot);
   ;	
	push	word ptr CODE:_poker_pot+2
	push	word ptr CODE:_poker_pot
	push	cs
	mov	ax,offset CODE:s@+2590
	push	ax
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
	jmp	short @31@1262
@31@1234:
   ;	
   ;	  else
   ;	   sprintf(s,"There was no money in the pot");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2616
	push	ax
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,8
@31@1262:
   ;	
   ;	
   ;	  if (players[winner].credit)
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+4]
	or	ax,word ptr cs:_players[bx+6]
	je	short @31@1458
   ;	
   ;	   {
   ;	    if (poker_pot<players[winner].credit)
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+6]
	mov	dx,word ptr cs:_players[bx+4]
	cmp	ax,word ptr CODE:_poker_pot+2
	jb	short @31@1402
	ja	short @31@1374
	cmp	dx,word ptr CODE:_poker_pot
	jbe	short @31@1402
@31@1374:
   ;	
   ;		 {
   ;			poker_pot=0;
   ;	
	mov	word ptr CODE:_poker_pot+2,0
	mov	word ptr CODE:_poker_pot,0
   ;	
   ;			players[winner].money = 0;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+2],0
	mov	word ptr cs:_players[bx],0
   ;	
   ;		 }
   ;	
	jmp	short @31@1430
@31@1402:
   ;	
   ;		else
   ;		  poker_pot -= players[winner].credit;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+6]
	mov	dx,word ptr cs:_players[bx+4]
	sub	word ptr CODE:_poker_pot,dx
	sbb	word ptr CODE:_poker_pot+2,ax
@31@1430:
   ;	
   ;	
   ;		players[winner].credit=0;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+6],0
	mov	word ptr cs:_players[bx+4],0
@31@1458:
   ;	
   ;	   }
   ;	
   ;		players[winner].money+=poker_pot;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	dx,word ptr CODE:_poker_pot+2
	mov	bx,word ptr CODE:_poker_pot
	xchg	bx,ax
	add	word ptr cs:_players[bx],ax
	adc	word ptr cs:_players[bx+2],dx
   ;	
   ;	
   ;	  poker_pot=0;
   ;	
	mov	word ptr CODE:_poker_pot+2,0
	mov	word ptr CODE:_poker_pot,0
   ;	
   ;	
   ;	  poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-254]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	  game_run_level=GAME_COMPLETE;
   ;	
	mov	word ptr CODE:_game_run_level,6
   ;	
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_check_for_winner	endp
@31@C1522	label	word
	dw	@31@730
	dw	@31@870
	dw	@31@926
	dw	@31@898
	dw	@31@842
	dw	@31@954
	dw	@31@982
	dw	@31@1010
	dw	@31@1038
   ;	
   ;	void poker_winner(int player)
   ;	
	assume	cs:CODE
_poker_winner	proc	near
	push	bp
	mov	bp,sp
	sub	sp,120
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  char s[120];
   ;	
   ;	  if (poker_pot)
   ;	
	mov	ax,word ptr CODE:_poker_pot
	or	ax,word ptr CODE:_poker_pot+2
	je	short @32@86
   ;	
   ;	   sprintf(s,"%s|*r1 wins with %lu in the pot.",players[player].handle,poker_pot);
   ;	
	push	word ptr CODE:_poker_pot+2
	push	word ptr CODE:_poker_pot
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	cs
	mov	ax,offset CODE:s@+2646
	push	ax
	push	ss
	lea	ax,word ptr [bp-120]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,16
	jmp	short @32@114
@32@86:
   ;	
   ;	  else
   ;	   sprintf(s,"No money in the pot");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2679
	push	ax
	push	ss
	lea	ax,word ptr [bp-120]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,8
@32@114:
   ;	
   ;	
   ;	  players[player].money+=poker_pot;
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	dx,word ptr CODE:_poker_pot+2
	mov	bx,word ptr CODE:_poker_pot
	xchg	bx,ax
	add	word ptr cs:_players[bx],ax
	adc	word ptr cs:_players[bx+2],dx
   ;	
   ;	  poker_pot=0;
   ;	
	mov	word ptr CODE:_poker_pot+2,0
	mov	word ptr CODE:_poker_pot,0
   ;	
   ;	
   ;	  poker_message("Only one player left. This game is over.");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2699
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	  poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-120]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	  game_run_level=GAME_COMPLETE;
   ;	
	mov	word ptr CODE:_game_run_level,6
   ;	
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_poker_winner	endp
   ;	
   ;	void print_roll_over_message(void)
   ;	
	assume	cs:CODE
_print_roll_over_message	proc	near
	push	bp
	mov	bp,sp
	sub	sp,150
   ;	
   ;	{
   ;	 char s[150];
   ;	
   ;	 sprintf(s,"No final bet, the pot (%lu) stays and we'll deal again.",poker_pot);
   ;	
	push	word ptr CODE:_poker_pot+2
	push	word ptr CODE:_poker_pot
	push	cs
	mov	ax,offset CODE:s@+2740
	push	ax
	push	ss
	lea	ax,word ptr [bp-150]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,12
   ;	
   ;	 poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-150]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_print_roll_over_message	endp
   ;	
   ;	int number_players_in(void)
   ;	
	assume	cs:CODE
_number_players_in	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	 int count;
   ;	 int number_playing=0;
   ;	
	xor	si,si
   ;	
   ;	
   ;	        for (count=0;count<num_players;count++)
   ;	
	xor	cx,cx
	jmp	short @34@142
@34@58:
   ;	
   ;	         if (!players[count].fold)
   ;	
	mov	ax,cx
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+72],0
	jne	short @34@114
   ;	
   ;	           { number_playing++;
   ;	
	inc	si
@34@114:
	inc	cx
@34@142:
	cmp	cx,word ptr CODE:_num_players
	jl	short @34@58
   ;	
   ;	           }
   ;	
   ;	 return (number_playing);
   ;	
	mov	ax,si
	jmp	short @34@198
@34@198:
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
_number_players_in	endp
   ;	
   ;	void process_game_level(void)
   ;	
	assume	cs:CODE
_process_game_level	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
   ;	
   ;	{
   ;	    int number_playing=0;
   ;	
	xor	di,di
   ;	
   ;	    int count;
   ;	    int a_player;
   ;	
   ;	
   ;	
   ;	    if (game_run_level!=NOT_RUNNING)
   ;	
	cmp	word ptr CODE:_game_run_level,0
	je	short @35@366
   ;	
   ;	      {
   ;	
   ;	        if (num_players)
   ;	
	cmp	word ptr CODE:_num_players,0
	je	short @35@254
   ;	
   ;	            for (count=0;count<num_players;count++)
   ;	
	xor	si,si
	jmp	short @35@226
@35@114:
   ;	
   ;	             if ((!players[count].fold) && (players[count].run_level!=NOT_RUNNING))
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+72],0
	jne	short @35@198
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],0
	je	short @35@198
   ;	
   ;	               { number_playing++;
   ;	
	inc	di
   ;	
   ;	                 a_player=count;
   ;	
	mov	word ptr [bp-2],si
@35@198:
	inc	si
@35@226:
	cmp	si,word ptr CODE:_num_players
	jl	short @35@114
@35@254:
   ;	
   ;	               }
   ;	
   ;	
   ;	        if (number_playing==1)
   ;	
	cmp	di,1
	jne	short @35@310
   ;	
   ;	         {
   ;	            poker_winner(a_player);
   ;	
	push	word ptr [bp-2]
	call	near ptr CODE:_poker_winner
	pop	cx
   ;	
   ;	            game_run_level=NOT_RUNNING;
   ;	
	mov	word ptr CODE:_game_run_level,0
@35@310:
   ;	
   ;	         }
   ;	
   ;	        if (!number_playing)
   ;	
	or	di,di
	jne	short @35@366
   ;	
   ;	        {
   ;	          poker_pot=0;
   ;	
	mov	word ptr CODE:_poker_pot+2,0
	mov	word ptr CODE:_poker_pot,0
   ;	
   ;	          game_run_level=NOT_RUNNING;
   ;	
	mov	word ptr CODE:_game_run_level,0
@35@366:
   ;	
   ;	        }
   ;	
   ;	      }
   ;	
   ;	    /* check for empty game here */
   ;	    /* check for forceouts after a 1 minute time limit here */
   ;	
   ;	    switch (game_run_level)
   ;	
	mov	bx,word ptr CODE:_game_run_level
	cmp	bx,6
	jbe	@@27
	jmp	@35@926
@@27:
	shl	bx,1
	jmp	word ptr cs:@35@C946[bx]
@35@450:
   ;	
   ;	     {
   ;	
   ;	            case NOT_RUNNING:       break;
   ;	
	jmp	@35@926
@35@478:
   ;	
   ;	
   ;	
   ;	            case DEAL_ROUND:        deal_cards();
   ;	
	call	near ptr CODE:_deal_cards
   ;	
   ;	                                    game_run_level = ANTE_ROUND;
   ;	
	mov	word ptr CODE:_game_run_level,2
   ;	
   ;	                                    reset_round_vars();
   ;	
	call	near ptr CODE:_reset_round_vars
   ;	
   ;	                                    break;
   ;	
	jmp	@35@926
@35@506:
   ;	
   ;	
   ;	            case ANTE_ROUND:        if (process_ante_round())
   ;	
	call	near ptr CODE:_process_ante_round
	or	ax,ax
	je	short @35@562
   ;	
   ;	                                      {
   ;	                                       game_run_level = FIRST_BET_ROUND;
   ;	
	mov	word ptr CODE:_game_run_level,3
   ;	
   ;	                                       poker_message("Everyone has anted, now for some betting.");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2796
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	                                       reset_round_vars();
   ;	
	call	near ptr CODE:_reset_round_vars
@35@562:
   ;	
   ;	                                      }
   ;	                                    break;
   ;	
	jmp	short @35@926
@35@590:
   ;	
   ;	
   ;	            case FIRST_BET_ROUND:   if (process_first_bets())
   ;	
	call	near ptr CODE:_process_first_bets
	or	ax,ax
	je	short @35@646
   ;	
   ;	                                      {
   ;	                                       game_run_level = DISCARD_ROUND;
   ;	
	mov	word ptr CODE:_game_run_level,4
   ;	
   ;	                                       poker_message("All bets are in, now discard.");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2838
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	                                       reset_round_vars();
   ;	
	call	near ptr CODE:_reset_round_vars
@35@646:
   ;	
   ;	                                      }
   ;	                                    break;
   ;	
	jmp	short @35@926
@35@674:
   ;	
   ;	
   ;	            case DISCARD_ROUND:     if (process_discard_round())
   ;	
	call	near ptr CODE:_process_discard_round
	or	ax,ax
	je	short @35@730
   ;	
   ;	                                     {
   ;	                                     game_run_level = FINAL_BET_ROUND;
   ;	
	mov	word ptr CODE:_game_run_level,5
   ;	
   ;	
   ;	                                     replace_discards();
   ;	
	call	near ptr CODE:_replace_discards
   ;	
   ;	                                     poker_message("Discarded cards replaced, now it's time for the final bets.");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2868
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	                                     g_delay(5);
   ;	
	mov	ax,5
	push	ax
	call	near ptr CODE:_g_delay
	pop	cx
   ;	
   ;	
   ;	                                     announce_all_cards();
   ;	
	call	near ptr CODE:_announce_all_cards
   ;	
   ;	                                     reset_round_vars();
   ;	
	call	near ptr CODE:_reset_round_vars
@35@730:
   ;	
   ;	                                     }
   ;	                                    break;
   ;	
	jmp	short @35@926
@35@758:
   ;	
   ;	
   ;	            case FINAL_BET_ROUND:   if (process_final_bets())
   ;	
	call	near ptr CODE:_process_final_bets
	or	ax,ax
	je	short @35@870
   ;	
   ;	                                    {
   ;	                                      game_run_level = GAME_COMPLETE;
   ;	
	mov	word ptr CODE:_game_run_level,6
   ;	
   ;	                                      if (!current_bet)
   ;	
	mov	ax,word ptr CODE:_current_bet
	or	ax,word ptr CODE:_current_bet+2
	jne	short @35@842
   ;	
   ;	                                        {
   ;	                                            print_roll_over_message();
   ;	
	call	near ptr CODE:_print_roll_over_message
   ;	
   ;	                                            game_run_level = NOT_RUNNING;
   ;	
	mov	word ptr CODE:_game_run_level,0
@35@842:
   ;	
   ;	                                        }
   ;	                                      reset_round_vars();
   ;	
	call	near ptr CODE:_reset_round_vars
@35@870:
   ;	
   ;	                                    }
   ;	                                    break;
   ;	
	jmp	short @35@926
@35@898:
   ;	
   ;	
   ;	            case GAME_COMPLETE:     check_for_winner();
   ;	
	call	near ptr CODE:_check_for_winner
   ;	
   ;	                                    end_game();
   ;	
	call	near ptr CODE:_end_game
   ;	
   ;	                                    game_run_level = NOT_RUNNING;
   ;	
	mov	word ptr CODE:_game_run_level,0
   ;	
   ;	                                    break;
   ;	
	jmp	short @35@926
@35@926:
   ;	
   ;	
   ;	     }
   ;	
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_process_game_level	endp
@35@C946	label	word
	dw	@35@450
	dw	@35@478
	dw	@35@506
	dw	@35@590
	dw	@35@674
	dw	@35@758
	dw	@35@898
   ;	
   ;	void shuffle_deck(void)
   ;	
	assume	cs:CODE
_shuffle_deck	proc	near
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
   ;	
   ;	{
   ;	   int count=0;
   ;	
	mov	word ptr [bp-2],0
   ;	
   ;	   int loc;
   ;	   int count2;
   ;	
   ;	   empty_deck();
   ;	
	call	near ptr CODE:_empty_deck
   ;	
   ;	   srand((unsigned) dans_counter);
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	push	word ptr es:[bx]
	call	near ptr CODE:_srand
	pop	cx
   ;	
   ;	
   ;	   poker_message("Shuffling...");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2928
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	
   ;	   for (count=0;count<NUM_CARDS;count++)
   ;	
	mov	word ptr [bp-2],0
	jmp	short @36@422
@36@58:
   ;	
   ;	   {
   ;	      count2=0;
   ;	
	xor	di,di
@36@86:
   ;	
   ;	
   ;	      do
   ;	      { loc = rand() % NUM_CARDS;
   ;	
	call	near ptr CODE:_rand
	mov	bx,52
	cwd	
	idiv	bx
	mov	si,dx
   ;	
   ;	        count2++; }
   ;	
	inc	di
   ;	
   ;	      while ((card_deck[loc]!=NO_CARD) && (count2<52));
   ;	
	cmp	byte ptr cs:_card_deck[si],255
	je	short @36@170
	cmp	di,52
	jl	short @36@86
@36@170:
   ;	
   ;	
   ;	      if (card_deck[loc] != NO_CARD)
   ;	
	cmp	byte ptr cs:_card_deck[si],255
	je	short @36@282
   ;	
   ;	       {
   ;	         loc=0;
   ;	
	xor	si,si
	jmp	short @36@254
@36@226:
   ;	
   ;	         while (card_deck[loc]!=NO_CARD)
   ;	          loc++;
   ;	
	inc	si
@36@254:
	cmp	byte ptr cs:_card_deck[si],255
	jne	short @36@226
@36@282:
   ;	
   ;	       }
   ;	      if ((loc>=0) && (loc<52))
   ;	
	or	si,si
	jl	short @36@366
	cmp	si,52
	jge	short @36@366
   ;	
   ;	        card_deck[loc] = count;
   ;	
	mov	al,byte ptr [bp-2]
	mov	byte ptr cs:_card_deck[si],al
	jmp	short @36@394
@36@366:
   ;	
   ;	      else
   ;	        print_str_cr("It's screwed dave");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2941
	push	ax
	call	near ptr CODE:_print_str_cr
	pop	cx
	pop	cx
@36@394:
	inc	word ptr [bp-2]
@36@422:
	cmp	word ptr [bp-2],52
	jl	short @36@58
   ;	
   ;	
   ;	   }
   ;	
   ;	   print_str_cr("Deck Shuffled");
   ;	
	push	cs
	mov	ax,offset CODE:s@+2959
	push	ax
	call	near ptr CODE:_print_str_cr
	pop	cx
	pop	cx
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_shuffle_deck	endp
   ;	
   ;	void deregister_player(int portnum)
   ;	
	assume	cs:CODE
_deregister_player	proc	near
	push	bp
	mov	bp,sp
	sub	sp,120
	push	si
	push	di
   ;	
   ;	{
   ;	
   ;	   int count=0;
   ;	
	xor	si,si
	jmp	short @37@226
@37@58:
   ;	
   ;	   int count2;
   ;	   char s[120];
   ;	
   ;	   while (count<num_players)
   ;	    {
   ;	     if (players[count].portnum == portnum)
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+52]
	cmp	ax,word ptr [bp+4]
	jne	short @37@198
   ;	
   ;	       {
   ;	         sprintf(s,"Player (#%02d):%s|*r1 has left the game",portnum,players[count].handle);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	word ptr [bp+4]
	push	cs
	mov	ax,offset CODE:s@+2973
	push	ax
	push	ss
	lea	ax,word ptr [bp-120]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,14
   ;	
   ;	         poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-120]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	
   ;	         count2=count+1;
   ;	
	mov	ax,si
	inc	ax
	mov	di,ax
	jmp	short @37@142
@37@114:
   ;	
   ;	         while (count2<num_players)
   ;	          { players[count2-1] = players[count2];
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players-76
	push	cs
	push	ax
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players
	push	cs
	push	ax
	mov	cx,76
	call	far ptr F_SCOPY@
   ;	
   ;	            count2++;
   ;	
	inc	di
@37@142:
	cmp	di,word ptr CODE:_num_players
	jl	short @37@114
   ;	
   ;	          }
   ;	         num_players--;
   ;	
	dec	word ptr CODE:_num_players
@37@198:
   ;	
   ;	
   ;	
   ;	       }
   ;	       count++;
   ;	
	inc	si
@37@226:
	cmp	si,word ptr CODE:_num_players
	jl	short @37@58
   ;	
   ;	    }
   ;	
   ;	    if (first_better>num_players)
   ;	
	mov	ax,word ptr CODE:_first_better
	cmp	ax,word ptr CODE:_num_players
	jle	short @37@310
   ;	
   ;	     first_better=0;
   ;	
	mov	word ptr CODE:_first_better,0
@37@310:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_deregister_player	endp
   ;	
   ;	void deregister_player_verbose(int portnum)
   ;	
	assume	cs:CODE
_deregister_player_verbose	proc	near
	push	bp
	mov	bp,sp
	sub	sp,122
	push	si
	push	di
   ;	
   ;	{
   ;	
   ;	   int count=0;
   ;	
	xor	si,si
   ;	
   ;	   int count2;
   ;	   int found_player=0;
   ;	
	mov	word ptr [bp-2],0
	jmp	short @38@226
@38@58:
   ;	
   ;	   char s[120];
   ;	
   ;	   while (count<num_players)
   ;	    {
   ;	     if (players[count].portnum == portnum)
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+52]
	cmp	ax,word ptr [bp+4]
	jne	short @38@198
   ;	
   ;	       {
   ;	         sprintf(s,"Player (#%02d):%s|*r1 has left the game",portnum,players[count].handle);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	push	word ptr [bp+4]
	push	cs
	mov	ax,offset CODE:s@+3013
	push	ax
	push	ss
	lea	ax,word ptr [bp-122]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,14
   ;	
   ;	         poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-122]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	         found_player=1;
   ;	
	mov	word ptr [bp-2],1
   ;	
   ;	
   ;	         count2=count+1;
   ;	
	mov	ax,si
	inc	ax
	mov	di,ax
	jmp	short @38@142
@38@114:
   ;	
   ;	         while (count2<num_players)
   ;	          { players[count2-1] = players[count2];
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players-76
	push	cs
	push	ax
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players
	push	cs
	push	ax
	mov	cx,76
	call	far ptr F_SCOPY@
   ;	
   ;	            count2++;
   ;	
	inc	di
@38@142:
	cmp	di,word ptr CODE:_num_players
	jl	short @38@114
   ;	
   ;	          }
   ;	         num_players--;
   ;	
	dec	word ptr CODE:_num_players
@38@198:
   ;	
   ;	
   ;	
   ;	       }
   ;	       count++;
   ;	
	inc	si
@38@226:
	cmp	si,word ptr CODE:_num_players
	jge	@@28
	jmp	@38@58
@@28:
   ;	
   ;	    }
   ;	
   ;	    if (found_player)
   ;	
	cmp	word ptr [bp-2],0
	je	short @38@366
   ;	
   ;	     {
   ;	        if (first_better>num_players)
   ;	
	mov	ax,word ptr CODE:_first_better
	cmp	ax,word ptr CODE:_num_players
	jle	short @38@338
   ;	
   ;	         first_better=0;
   ;	
	mov	word ptr CODE:_first_better,0
@38@338:
   ;	
   ;	     }
   ;	
	jmp	short @38@394
@38@366:
   ;	
   ;	    else
   ;	     private_poker_message("You're not in the game!",portnum);
   ;	
	push	word ptr [bp+4]
	push	cs
	mov	ax,offset CODE:s@+3053
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
@38@394:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_deregister_player_verbose	endp
   ;	
   ;	void register_player(int portnum)
   ;	
	assume	cs:CODE
_register_player	proc	near
	push	bp
	mov	bp,sp
	sub	sp,724
	push	si
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  struct user_data a_user;
   ;	  struct ln_type a_line;
   ;	  int player_no = player_number(portnum);
   ;	
	push	si
	call	near ptr CODE:_player_number
	pop	cx
	mov	word ptr [bp-2],ax
   ;	
   ;	
   ;	  char s[150];
   ;	
   ;	  if (player_no!=-1)
   ;	
	cmp	word ptr [bp-2],-1
	je	short @39@114
   ;	
   ;	   {
   ;	    private_poker_message("You are already in the game!",portnum);
   ;	
	push	si
	push	cs
	mov	ax,offset CODE:s@+3077
@39@86:
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
   ;	
   ;	    return;
   ;	
	jmp	@39@310
@39@114:
   ;	
   ;	   }
   ;	
   ;	  if (num_players>=MAX_PLAYERS)
   ;	
	cmp	word ptr CODE:_num_players,6
	jl	short @39@170
   ;	
   ;	     {
   ;	       private_poker_message("No room in the game\007",portnum);
   ;	
	push	si
	push	cs
	mov	ax,offset CODE:s@+3106
	jmp	short @39@86
@39@170:
   ;	
   ;	       return;
   ;	     }
   ;	
   ;	
   ;	
   ;	
   ;	  if (!st_copy((void far *)&a_line,(sizeof(struct user_data)),LINE_STATUS,portnum))
   ;	
	push	si
	mov	ax,2
	push	ax
	mov	ax,384
	push	ax
	push	ss
	lea	ax,word ptr [bp-574]
	push	ax
	call	near ptr CODE:_st_copy
	add	sp,10
	or	ax,ax
	jne	short @39@226
   ;	
   ;	    return;
   ;	
	jmp	@39@310
@39@226:
   ;	
   ;	
   ;	  if (a_line.mainchannel!=poker_channel)
   ;	
	mov	al,byte ptr [bp-557]
	mov	ah,0
	cmp	ax,word ptr CODE:_poker_channel
	je	short @39@282
   ;	
   ;	    {
   ;	       sprintf(s,"<PokerBot>: You must be on the poker channel%c%c              Current Channel: %d",13,10,poker_channel);
   ;	
	push	word ptr CODE:_poker_channel
	mov	ax,10
	push	ax
	mov	ax,13
	push	ax
	push	cs
	mov	ax,offset CODE:s@+3127
	push	ax
	push	ss
	lea	ax,word ptr [bp-724]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,14
   ;	
   ;	       aput_into_buffer(portnum,s,
   ;	
   ;	
   ;	         0, 1, tswitch, portnum, 0);
   ;	
	xor	ax,ax
	push	ax
	push	si
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+220]
	push	word ptr es:[bx]
	mov	ax,1
	push	ax
	xor	ax,ax
	push	ax
	push	ss
	lea	ax,word ptr [bp-724]
	push	ax
	push	si
	call	near ptr CODE:_aput_into_buffer
	add	sp,16
   ;	
   ;	       return;
   ;	
	jmp	@39@310
@39@282:
   ;	
   ;	    }
   ;	
   ;	  st_copy((void far *)&a_user,(sizeof(struct user_data)),USER_LINES,portnum);
   ;	
	push	si
	xor	ax,ax
	push	ax
	mov	ax,384
	push	ax
	push	ss
	lea	ax,word ptr [bp-386]
	push	ax
	call	near ptr CODE:_st_copy
	add	sp,10
   ;	
   ;	
   ;	
   ;	
   ;	  players[num_players].user_number = a_user.number;
   ;	
	mov	ax,word ptr CODE:_num_players
	mov	dx,76
	imul	dx
	mov	dx,word ptr [bp-386]
	mov	bx,ax
	mov	word ptr cs:_players[bx+50],dx
   ;	
   ;	  strncpy(players[num_players].handle,a_user.handle,HANDLE_SIZE+1);
   ;	
	mov	ax,42
	push	ax
	push	ss
	lea	ax,word ptr [bp-384]
	push	ax
	mov	ax,word ptr CODE:_num_players
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	call	near ptr CODE:_strncpy
	add	sp,10
   ;	
   ;	  players[num_players].handle[HANDLE_SIZE]=0;
   ;	
	mov	ax,word ptr CODE:_num_players
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	byte ptr cs:_players[bx+49],0
   ;	
   ;	  players[num_players].portnum = portnum;
   ;	
	mov	ax,word ptr CODE:_num_players
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+52],si
   ;	
   ;	  players[num_players].pid = 0;
   ;	
	mov	ax,word ptr CODE:_num_players
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+54],0
   ;	
   ;	  players[num_players].credit=0;
   ;	
	mov	ax,word ptr CODE:_num_players
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+6],0
	mov	word ptr cs:_players[bx+4],0
   ;	
   ;	  players[num_players].have_cards=0;
   ;	
	mov	ax,word ptr CODE:_num_players
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	byte ptr cs:_players[bx+61],0
   ;	
   ;	  players[num_players].run_level = NOT_RUNNING;
   ;	
	mov	ax,word ptr CODE:_num_players
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+66],0
   ;	
   ;	
   ;	
   ;	  players[num_players].money=1000;
   ;	
	mov	ax,word ptr CODE:_num_players
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	word ptr cs:_players[bx+2],0
	mov	word ptr cs:_players[bx],1000
   ;	
   ;	
   ;	  /* load his money here */
   ;	
   ;	
   ;	  sprintf(s,"%s|*r1 has joined the game with %lu money",a_user.handle,players[num_players].money);
   ;	
	mov	ax,word ptr CODE:_num_players
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+2]
	push	word ptr cs:_players[bx]
	push	ss
	lea	ax,word ptr [bp-384]
	push	ax
	push	cs
	mov	ax,offset CODE:s@+3209
	push	ax
	push	ss
	lea	ax,word ptr [bp-724]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,16
   ;	
   ;	  poker_message(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-724]
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	  num_players++;
   ;	
	inc	word ptr CODE:_num_players
@39@310:
   ;	
   ;	
   ;	}
   ;	
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_register_player	endp
   ;	
   ;	int compare_list(char near *list[], int length, char **cvalue)
   ;	
	assume	cs:CODE
_compare_list	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	  int count;
   ;	
   ;	  for (count=0;count<length;count++)
   ;	
	xor	si,si
	jmp	short @40@170
@40@58:
   ;	
   ;	  {
   ;	   if (!strncmp(*cvalue,list[count],strlen(list[count])))
   ;	
	push	ds
	mov	ax,si
	shl	ax,1
	les	bx,dword ptr [bp+4]
	add	bx,ax
	push	word ptr es:[bx]
	call	near ptr CODE:_strlen
	pop	cx
	pop	cx
	push	ax
	push	ds
	mov	ax,si
	shl	ax,1
	les	bx,dword ptr [bp+4]
	add	bx,ax
	push	word ptr es:[bx]
	les	bx,dword ptr [bp+10]
	push	word ptr es:[bx+2]
	push	word ptr es:[bx]
	call	near ptr CODE:_strncmp
	add	sp,10
	or	ax,ax
	jne	short @40@142
   ;	
   ;	   {
   ;	     (*cvalue) += strlen(list[count]);
   ;	
	push	ds
	mov	ax,si
	shl	ax,1
	les	bx,dword ptr [bp+4]
	add	bx,ax
	push	word ptr es:[bx]
	call	near ptr CODE:_strlen
	pop	cx
	pop	cx
	les	bx,dword ptr [bp+10]
	add	word ptr es:[bx],ax
   ;	
   ;	     return (count);
   ;	
	mov	ax,si
@40@114:
	jmp	short @40@226
@40@142:
	inc	si
@40@170:
	cmp	si,word ptr [bp+8]
	jl	short @40@58
   ;	
   ;	   }
   ;	  }
   ;	  return (-1);
   ;	
	mov	ax,-1
	jmp	short @40@114
@40@226:
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
_compare_list	endp
   ;	
   ;	int get_token(char **token, int *type, long int *ret_value)
   ;	
	assume	cs:CODE
_get_token	proc	near
	push	bp
	mov	bp,sp
	sub	sp,16
	push	si
	push	di
   ;	
   ;	{
   ;	  char token_buf[TOKEN_LENGTH+1];
   ;	  int count = 0;
   ;	
	xor	di,di
   ;	
   ;	  char *copy = token_buf;
   ;	
	lea	ax,word ptr [bp-6]
	mov	word ptr [bp-8],ss
	mov	word ptr [bp-10],ax
   ;	
   ;	  long int value = -1;
   ;	
	mov	word ptr [bp-12],-1
	mov	word ptr [bp-14],-1
	jmp	short @41@86
@41@58:
   ;	
   ;	
   ;	  while (**token == ' ') (*token)++;
   ;	
	les	bx,dword ptr [bp+4]
	inc	word ptr es:[bx]
@41@86:
	les	bx,dword ptr [bp+4]
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],32
	je	short @41@58
   ;	
   ;	  if (!(**token)) return (0);
   ;	
	les	bx,dword ptr [bp+4]
	les	bx,dword ptr es:[bx]
	mov	al,byte ptr es:[bx]
	cbw	
	or	ax,ax
	jne	short @41@198
@41@142:
	xor	ax,ax
@41@170:
	jmp	@41@1318
@41@198:
	jmp	short @41@366
@41@226:
   ;	
   ;	
   ;	  while ((count < TOKEN_LENGTH) && ((**token) != ' ') && (**token))
   ;	  {
   ;	    *copy++ = (((**token)>='a') && ((**token)<='z')) ?
   ;	
   ;	
   ;	       (*(*token)++ - ' ') : (*(*token)++);
   ;	
	les	bx,dword ptr [bp+4]
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],97
	jl	short @41@310
	les	bx,dword ptr [bp+4]
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],122
	jg	short @41@310
	les	bx,dword ptr [bp+4]
	mov	ax,word ptr es:[bx+2]
	mov	si,word ptr es:[bx]
	inc	word ptr es:[bx]
	mov	es,ax
	mov	al,byte ptr es:[si]
	add	al,224
	jmp	short @41@338
@41@310:
	les	bx,dword ptr [bp+4]
	mov	ax,word ptr es:[bx+2]
	mov	si,word ptr es:[bx]
	inc	word ptr es:[bx]
	mov	es,ax
	mov	al,byte ptr es:[si]
@41@338:
	les	bx,dword ptr [bp-10]
	mov	byte ptr es:[bx],al
	inc	word ptr [bp-10]
   ;	
   ;	    count++;
   ;	
	inc	di
@41@366:
	cmp	di,4
	jge	short @41@450
	les	bx,dword ptr [bp+4]
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],32
	je	short @41@450
	les	bx,dword ptr [bp+4]
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],0
	jne	short @41@226
@41@450:
	jmp	short @41@506
@41@478:
   ;	
   ;	  }
   ;	  while (((**token) != ' ') && (**token)) (*token)++;
   ;	
	les	bx,dword ptr [bp+4]
	inc	word ptr es:[bx]
@41@506:
	les	bx,dword ptr [bp+4]
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],32
	je	short @41@562
	les	bx,dword ptr [bp+4]
	les	bx,dword ptr es:[bx]
	cmp	byte ptr es:[bx],0
	jne	short @41@478
@41@562:
   ;	
   ;	  *copy = 0;
   ;	
	les	bx,dword ptr [bp-10]
	mov	byte ptr es:[bx],0
   ;	
   ;	  copy = token_buf;
   ;	
	lea	ax,word ptr [bp-6]
	mov	word ptr [bp-8],ss
	mov	word ptr [bp-10],ax
   ;	
   ;	  value = compare_list(tokens,NUM_TOKENS,&copy);
   ;	
	push	ss
	lea	ax,word ptr [bp-10]
	push	ax
	mov	ax,18
	push	ax
	push	cs
	mov	ax,offset CODE:_tokens
	push	ax
	call	near ptr CODE:_compare_list
	add	sp,10
	cwd	
	mov	word ptr [bp-12],dx
	mov	word ptr [bp-14],ax
   ;	
   ;	  if (value != -1)
   ;	
	cmp	word ptr [bp-12],-1
	jne	short @41@618
	cmp	word ptr [bp-14],-1
	je	short @41@730
@41@618:
   ;	
   ;	  {
   ;	    if ((*type) && (*type != WORD_TOKEN)) return (0);
   ;	
	les	bx,dword ptr [bp+8]
	cmp	word ptr es:[bx],0
	je	short @41@702
	les	bx,dword ptr [bp+8]
	cmp	word ptr es:[bx],1
	je	short @41@702
	jmp	@41@142
@41@702:
   ;	
   ;	    *type = WORD_TOKEN;
   ;	
	les	bx,dword ptr [bp+8]
	mov	word ptr es:[bx],1
	jmp	@41@1262
@41@730:
   ;	
   ;	    *ret_value = value;
   ;	    return (1);
   ;	  }
   ;	  value = compare_list(short_card_names,13,&copy);
   ;	
	push	ss
	lea	ax,word ptr [bp-10]
	push	ax
	mov	ax,13
	push	ax
	push	cs
	mov	ax,offset CODE:_short_card_names
	push	ax
	call	near ptr CODE:_compare_list
	add	sp,10
	cwd	
	mov	word ptr [bp-12],dx
	mov	word ptr [bp-14],ax
   ;	
   ;	  if (value != -1)
   ;	
	cmp	word ptr [bp-12],-1
	jne	short @41@786
	cmp	word ptr [bp-14],-1
	jne	@@29
	jmp	@41@1010
@@29:
@41@786:
   ;	
   ;	  {
   ;	    int card = value;
   ;	
	mov	ax,word ptr [bp-14]
	mov	word ptr [bp-16],ax
   ;	
   ;	
   ;	    if (*copy == '-') copy++;
   ;	
	les	bx,dword ptr [bp-10]
	cmp	byte ptr es:[bx],45
	jne	short @41@842
	inc	word ptr [bp-10]
@41@842:
   ;	
   ;	    value = compare_list(short_suit_names,4,&copy);
   ;	
	push	ss
	lea	ax,word ptr [bp-10]
	push	ax
	mov	ax,4
	push	ax
	push	cs
	mov	ax,offset CODE:_short_suit_names
	push	ax
	call	near ptr CODE:_compare_list
	add	sp,10
	cwd	
	mov	word ptr [bp-12],dx
	mov	word ptr [bp-14],ax
   ;	
   ;	    if (value != -1)
   ;	
	cmp	word ptr [bp-12],-1
	jne	short @41@898
	cmp	word ptr [bp-14],-1
	je	short @41@1010
@41@898:
   ;	
   ;	    {
   ;	      if ((*type) && (*type != CARD_TOKEN)) return (0);
   ;	
	les	bx,dword ptr [bp+8]
	cmp	word ptr es:[bx],0
	je	short @41@982
	les	bx,dword ptr [bp+8]
	cmp	word ptr es:[bx],3
	je	short @41@982
	jmp	@41@142
@41@982:
   ;	
   ;	      *type = CARD_TOKEN;
   ;	
	les	bx,dword ptr [bp+8]
	mov	word ptr es:[bx],3
   ;	
   ;	      *ret_value = card + (value * 13);
   ;	
	mov	ax,word ptr [bp-16]
	cwd	
	mov	cx,word ptr [bp-12]
	mov	bx,word ptr [bp-14]
	push	ax
	push	dx
	xor	dx,dx
	mov	ax,13
	call	far ptr F_LXMUL@
	pop	bx
	pop	cx
	add	cx,ax
	adc	bx,dx
	les	si,dword ptr [bp+12]
	mov	word ptr es:[si+2],bx
	mov	word ptr es:[si],cx
	jmp	@41@1290
@41@1010:
   ;	
   ;	      return (1);
   ;	    }
   ;	  }
   ;	  copy = token_buf;
   ;	
	lea	ax,word ptr [bp-6]
	mov	word ptr [bp-8],ss
	mov	word ptr [bp-10],ax
   ;	
   ;	  value = 0;
   ;	
	mov	word ptr [bp-12],0
	mov	word ptr [bp-14],0
	jmp	short @41@1066
@41@1038:
   ;	
   ;	  while ((*copy >= '0') && (*copy <= '9'))
   ;	    value = (value * 10) + (*copy++ - '0');
   ;	
	mov	cx,word ptr [bp-12]
	mov	bx,word ptr [bp-14]
	xor	dx,dx
	mov	ax,10
	call	far ptr F_LXMUL@
	les	bx,dword ptr [bp-10]
	push	ax
	mov	al,byte ptr es:[bx]
	cbw	
	add	ax,-48
	push	dx
	cwd	
	pop	bx
	pop	cx
	add	cx,ax
	adc	bx,dx
	mov	word ptr [bp-12],bx
	mov	word ptr [bp-14],cx
	inc	word ptr [bp-10]
@41@1066:
	les	bx,dword ptr [bp-10]
	cmp	byte ptr es:[bx],48
	jl	short @41@1122
	les	bx,dword ptr [bp-10]
	cmp	byte ptr es:[bx],57
	jle	short @41@1038
@41@1122:
   ;	
   ;	  if ((*copy) || ((*type) && (*type != NUMBER_TOKEN))) return (0);
   ;	
	les	bx,dword ptr [bp-10]
	cmp	byte ptr es:[bx],0
	jne	short @41@1206
	les	bx,dword ptr [bp+8]
	cmp	word ptr es:[bx],0
	je	short @41@1234
	les	bx,dword ptr [bp+8]
	cmp	word ptr es:[bx],2
	je	short @41@1234
@41@1206:
	jmp	@41@142
@41@1234:
   ;	
   ;	  *type = NUMBER_TOKEN;
   ;	
	les	bx,dword ptr [bp+8]
	mov	word ptr es:[bx],2
@41@1262:
   ;	
   ;	  *ret_value = value;
   ;	
	les	bx,dword ptr [bp+12]
	mov	ax,word ptr [bp-12]
	mov	dx,word ptr [bp-14]
	mov	word ptr es:[bx+2],ax
	mov	word ptr es:[bx],dx
@41@1290:
   ;	
   ;	  return (1);
   ;	
	mov	ax,1
	jmp	@41@170
@41@1318:
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_get_token	endp
   ;	
   ;	int player_number(portnum)
   ;	
	assume	cs:CODE
_player_number	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	 int count=0;
   ;	
	xor	cx,cx
	jmp	short @42@198
@42@58:
   ;	
   ;	
   ;	 while (count<num_players)
   ;	  {
   ;	    if (players[count].portnum==portnum)
   ;	
	mov	ax,cx
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+52]
	cmp	ax,word ptr [bp+4]
	jne	short @42@170
   ;	
   ;	     return (count);
   ;	
	mov	ax,cx
@42@114:
	jmp	short @42@254
	jmp	short @42@198
@42@170:
   ;	
   ;	    else
   ;	    count++;
   ;	
	inc	cx
@42@198:
	cmp	cx,word ptr CODE:_num_players
	jl	short @42@58
   ;	
   ;	
   ;	  }
   ;	  return -1;
   ;	
	mov	ax,-1
	jmp	short @42@114
@42@254:
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_player_number	endp
   ;	
   ;	void print_poker_cards_for_player(int player_no)
   ;	
	assume	cs:CODE
_print_poker_cards_for_player	proc	near
	push	bp
	mov	bp,sp
	sub	sp,240
	push	si
	push	di
	mov	si,word ptr [bp+4]
   ;	
   ;	{
   ;	  int count;
   ;	  char s[200];
   ;	  char s2[40];
   ;	
   ;	
   ;	  if ((player_no<0) || (player_no)>=num_players)
   ;	
	or	si,si
	jl	short @43@86
	cmp	si,word ptr CODE:_num_players
	jl	short @43@114
@43@86:
   ;	
   ;	   return;
   ;	
	jmp	@43@646
@43@114:
   ;	
   ;	
   ;	  if (game_run_level==NOT_RUNNING)
   ;	
	cmp	word ptr CODE:_game_run_level,0
	jne	short @43@170
   ;	
   ;	   {
   ;		private_poker_message("The game has not yet begun.",players[player_no].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	push	cs
	mov	ax,offset CODE:s@+3251
	jmp	@43@618
@43@170:
   ;	
   ;	    return;
   ;	   }
   ;	
   ;	  if (!players[player_no].have_cards)
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	al,byte ptr cs:_players[bx+61]
	cbw	
	or	ax,ax
	jne	short @43@226
   ;	
   ;	    { private_poker_message("You have no cards",players[player_no].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	push	cs
	mov	ax,offset CODE:s@+3279
	jmp	@43@618
@43@226:
   ;	
   ;	      return;
   ;	    }
   ;	
   ;	  if (players[player_no].fold)
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+72],0
	je	short @43@282
   ;	
   ;	   {
   ;		private_poker_message("You have already folded.",players[player_no].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	push	cs
	mov	ax,offset CODE:s@+3297
	jmp	@43@618
@43@282:
   ;	
   ;		return;
   ;	   }
   ;	
   ;	  if (players[player_no].run_level==NOT_RUNNING)
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],0
	jne	short @43@338
   ;	
   ;	   {
   ;		private_poker_message("You are not in this round of play.",players[player_no].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	push	cs
	mov	ax,offset CODE:s@+3322
	jmp	@43@618
@43@338:
   ;	
   ;		return;
   ;	   }
   ;	
   ;	  if (players[player_no].run_level<FIRST_BET_ROUND)
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],3
	jge	short @43@394
   ;	
   ;	    {
   ;	     private_poker_message("You must ante first.",players[player_no].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	push	cs
	mov	ax,offset CODE:s@+3357
	jmp	@43@618
@43@394:
   ;	
   ;	     return;
   ;	    }
   ;	  if ((players[player_no].run_level!=DISCARD_ROUND) && (game_run_level==DISCARD_ROUND))
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],4
	je	short @43@478
	cmp	word ptr CODE:_game_run_level,4
	jne	short @43@478
   ;	
   ;	    {
   ;	       private_poker_message("You must wait for all others to discard.",players[player_no].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	push	cs
	mov	ax,offset CODE:s@+3378
	jmp	@43@618
@43@478:
   ;	
   ;	       return;
   ;	    }
   ;	
   ;	  strcpy(s,"Your Cards: ");
   ;	
	push	cs
	mov	ax,offset CODE:s@+3419
	push	ax
	push	ss
	lea	ax,word ptr [bp-200]
	push	ax
	call	near ptr CODE:_strcpy
	add	sp,8
   ;	
   ;	  for (count=0;count<5;count++)
   ;	
	xor	di,di
	jmp	short @43@562
@43@506:
   ;	
   ;	   {
   ;	     sprintf(s2,"%d) %s-%s  ",count+1,
   ;	
   ;	
   ;	                         (char far *)short_card_names[((unsigned char) players[player_no].cards_in_hand[count]) % 13],
   ;	                         (char far *)short_suit_names[((unsigned char) players[player_no].cards_in_hand[count]) / 13]);
   ;	
	push	ds
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,di
	mov	bx,ax
	mov	al,byte ptr cs:_players[bx+56]
	mov	ah,0
	mov	bx,13
	cwd	
	idiv	bx
	shl	ax,1
	mov	bx,ax
	push	word ptr cs:_short_suit_names[bx]
	push	ds
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,di
	mov	bx,ax
	mov	al,byte ptr cs:_players[bx+56]
	mov	ah,0
	mov	bx,13
	cwd	
	idiv	bx
	shl	dx,1
	mov	bx,dx
	push	word ptr cs:_short_card_names[bx]
	mov	ax,di
	inc	ax
	push	ax
	push	cs
	mov	ax,offset CODE:s@+3432
	push	ax
	push	ss
	lea	ax,word ptr [bp-240]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,18
   ;	
   ;	     strcat(s,s2);
   ;	
	push	ss
	lea	ax,word ptr [bp-240]
	push	ax
	push	ss
	lea	ax,word ptr [bp-200]
	push	ax
	call	near ptr CODE:_strcat
	add	sp,8
	inc	di
@43@562:
	cmp	di,5
	jl	short @43@506
   ;	
   ;	   }
   ;	
   ;	   private_poker_message(s,players[player_no].portnum);
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	push	ss
	lea	ax,word ptr [bp-200]
@43@618:
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
@43@646:
   ;	
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_print_poker_cards_for_player	endp
   ;	
   ;	void print_poker_cards(int portnum)
   ;	
	assume	cs:CODE
_print_poker_cards	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	 int player_no = player_number(portnum);
   ;	
	push	word ptr [bp+4]
	call	near ptr CODE:_player_number
	pop	cx
	mov	si,ax
   ;	
   ;	
   ;	 if (player_no<0)
   ;	
	or	si,si
	jge	short @44@86
   ;	
   ;	  return;
   ;	
	jmp	short @44@114
@44@86:
   ;	
   ;	 print_poker_cards_for_player(player_no);
   ;	
	push	si
	call	near ptr CODE:_print_poker_cards_for_player
	pop	cx
@44@114:
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
_print_poker_cards	endp
   ;	
   ;	void announce_all_cards(void)
   ;	
	assume	cs:CODE
_announce_all_cards	proc	near
	push	bp
	mov	bp,sp
	push	si
   ;	
   ;	{
   ;	 int count;
   ;	
   ;	 print_str_cr("Announcing Cards");
   ;	
	push	cs
	mov	ax,offset CODE:s@+3444
	push	ax
	call	near ptr CODE:_print_str_cr
	pop	cx
	pop	cx
   ;	
   ;	
   ;	 for (count=0;count < num_players;count++)
   ;	
	xor	si,si
	jmp	short @45@170
@45@58:
   ;	
   ;	  if (!(players[count].fold) && (players[count].run_level!=NOT_RUNNING))
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+72],0
	jne	short @45@142
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],0
	je	short @45@142
   ;	
   ;	    print_poker_cards_for_player(count);
   ;	
	push	si
	call	near ptr CODE:_print_poker_cards_for_player
	pop	cx
@45@142:
	inc	si
@45@170:
	cmp	si,word ptr CODE:_num_players
	jl	short @45@58
   ;	
   ;	}
   ;	
	pop	si
	pop	bp
	ret	
_announce_all_cards	endp
   ;	
   ;	void print_poker_status(int portnum)
   ;	
	assume	cs:CODE
_print_poker_status	proc	near
	push	bp
	mov	bp,sp
	sub	sp,124
	push	si
	push	di
	mov	di,word ptr [bp+4]
   ;	
   ;	{
   ;	  int count;
   ;	  char s[120];
   ;	  char *header;
   ;	
   ;	  if (portnum==server)
   ;	
	cmp	di,word ptr CODE:_server
	jne	short @46@86
   ;	
   ;	    header = public_poker_header;
   ;	
	mov	word ptr [bp-2],cs
	mov	word ptr [bp-4],offset CODE:_public_poker_header
	jmp	short @46@114
@46@86:
   ;	
   ;	  else
   ;	    header = private_poker_header;
   ;	
	mov	word ptr [bp-2],cs
	mov	word ptr [bp-4],offset CODE:_private_poker_header
@46@114:
   ;	
   ;	
   ;	  if (!num_players)
   ;	
	cmp	word ptr CODE:_num_players,0
	jne	short @46@170
   ;	
   ;	   {
   ;	
   ;	        aput_append_into_buffer(portnum, poker_channel, 1,
   ;	
   ;	
   ;	                 tswitch, portnum, 0, 2, header,"No Current Players");
   ;	
	push	cs
	mov	ax,offset CODE:s@+3461
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	mov	ax,2
	push	ax
	xor	ax,ax
	push	ax
	push	di
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+220]
	push	word ptr es:[bx]
	mov	ax,1
	push	ax
	push	word ptr CODE:_poker_channel
	push	di
	call	near ptr CODE:_aput_append_into_buffer
	add	sp,22
   ;	
   ;	        aput_append_into_buffer(portnum, poker_channel, 1,
   ;	
   ;	
   ;	                 tswitch, portnum, 0, 2, header,info_line);
   ;	
	push	cs
	mov	ax,offset CODE:_info_line
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	mov	ax,2
	push	ax
	xor	ax,ax
	push	ax
	push	di
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+220]
	push	word ptr es:[bx]
	mov	ax,1
	push	ax
	push	word ptr CODE:_poker_channel
	push	di
	call	near ptr CODE:_aput_append_into_buffer
	add	sp,22
   ;	
   ;	
   ;	        return;
   ;	
	jmp	@46@450
@46@170:
   ;	
   ;	   }
   ;	
   ;	  for (count=0;count<num_players;count++)
   ;	
	xor	si,si
	jmp	@46@366
@46@198:
   ;	
   ;	    {
   ;	
   ;	        if (players[count].credit)
   ;	
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+4]
	or	ax,word ptr cs:_players[bx+6]
	je	short @46@254
   ;	
   ;	           sprintf(s,"#%02d:%s|*r1 with -% 9lu money  at run_level:%s",players[count].portnum,players[count].handle,players[coun
   ;	
   ;	
   ;	                  run_levels[players[count].run_level]);
   ;	
	push	ds
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	bx,word ptr cs:_players[bx+66]
	shl	bx,1
	push	word ptr cs:_run_levels[bx]
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	ax,word ptr cs:_players[bx+6]
	mov	dx,word ptr cs:_players[bx+4]
	push	ax
	mov	ax,si
	mov	bx,76
	push	dx
	imul	bx
	mov	bx,ax
	pop	ax
	sub	ax,word ptr cs:_players[bx]
	pop	dx
	sbb	dx,word ptr cs:_players[bx+2]
	push	dx
	push	ax
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	push	cs
	mov	ax,offset CODE:s@+3480
	jmp	short @46@282
@46@254:
   ;	
   ;	        else
   ;	           sprintf(s,"#%02d:%s|*r1 with % 9lu money  at run_level:%s",players[count].portnum,players[count].handle,players[count
   ;	
   ;	
   ;	                  run_levels[players[count].run_level]);
   ;	
	push	ds
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	bx,word ptr cs:_players[bx+66]
	shl	bx,1
	push	word ptr cs:_run_levels[bx]
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+2]
	push	word ptr cs:_players[bx]
	mov	ax,si
	mov	dx,76
	imul	dx
	add	ax,offset CODE:_players+8
	push	cs
	push	ax
	mov	ax,si
	mov	dx,76
	imul	dx
	mov	bx,ax
	push	word ptr cs:_players[bx+52]
	push	cs
	mov	ax,offset CODE:s@+3528
@46@282:
	push	ax
	push	ss
	lea	ax,word ptr [bp-124]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,22
   ;	
   ;	
   ;	        aput_append_into_buffer(portnum, poker_channel, 1,
   ;	
   ;	
   ;	                 tswitch, portnum, 0, 2, header,s);
   ;	
	push	ss
	lea	ax,word ptr [bp-124]
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	mov	ax,2
	push	ax
	xor	ax,ax
	push	ax
	push	di
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+220]
	push	word ptr es:[bx]
	mov	ax,1
	push	ax
	push	word ptr CODE:_poker_channel
	push	di
	call	near ptr CODE:_aput_append_into_buffer
	add	sp,22
	inc	si
@46@366:
	cmp	si,word ptr CODE:_num_players
	jge	@@30
	jmp	@46@198
@@30:
   ;	
   ;	    }
   ;	
   ;	  if (poker_pot)
   ;	
	mov	ax,word ptr CODE:_poker_pot
	or	ax,word ptr CODE:_poker_pot+2
	je	short @46@450
   ;	
   ;	   {
   ;	      sprintf(s,"Current Pot: %lu    Current Bet: %lu",poker_pot,current_bet);
   ;	
	push	word ptr CODE:_current_bet+2
	push	word ptr CODE:_current_bet
	push	word ptr CODE:_poker_pot+2
	push	word ptr CODE:_poker_pot
	push	cs
	mov	ax,offset CODE:s@+3575
	push	ax
	push	ss
	lea	ax,word ptr [bp-124]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,16
   ;	
   ;	      aput_append_into_buffer(portnum,poker_channel, 1,
   ;	
   ;	
   ;	                 tswitch, portnum, 0, 2, header, s);
   ;	
	push	ss
	lea	ax,word ptr [bp-124]
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	mov	ax,2
	push	ax
	xor	ax,ax
	push	ax
	push	di
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+220]
	push	word ptr es:[bx]
	mov	ax,1
	push	ax
	push	word ptr CODE:_poker_channel
	push	di
	call	near ptr CODE:_aput_append_into_buffer
	add	sp,22
@46@450:
   ;	
   ;	   }
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_print_poker_status	endp
   ;	
   ;	void interpret_command(char *string, int portnum,int in_public)
   ;	
	assume	cs:CODE
_interpret_command	proc	near
	push	bp
	mov	bp,sp
	sub	sp,94
	push	si
	push	di
	mov	si,word ptr [bp+8]
   ;	
   ;	{
   ;	  int type,result;
   ;	  long int value;
   ;	  char *command = string;
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,word ptr [bp+4]
	mov	word ptr [bp-8],ax
	mov	word ptr [bp-10],dx
   ;	
   ;	  char s[80];
   ;	
   ;	    if (portnum==tswitch)
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+220]
	cmp	word ptr es:[bx],si
	jne	short @47@86
   ;	
   ;	      return;
   ;	
	jmp	@47@1038
@47@86:
   ;	
   ;	
   ;	    type = 0;
   ;	
	mov	word ptr [bp-2],0
   ;	
   ;	
   ;	    if (result=get_token(&command,&type,&value))
   ;	
	push	ss
	lea	ax,word ptr [bp-6]
	push	ax
	push	ss
	lea	ax,word ptr [bp-2]
	push	ax
	push	ss
	lea	ax,word ptr [bp-10]
	push	ax
	call	near ptr CODE:_get_token
	add	sp,12
	mov	di,ax
	or	ax,ax
	je	short @47@142
   ;	
   ;	      sprintf(s,"Token %d/%d",type,value);
   ;	
	push	word ptr [bp-4]
	push	word ptr [bp-6]
	push	word ptr [bp-2]
	push	cs
	mov	ax,offset CODE:s@+3612
	jmp	short @47@170
@47@142:
   ;	
   ;	      else
   ;	      sprintf(s,"Bad Token %d/%d",type,value);
   ;	
	push	word ptr [bp-4]
	push	word ptr [bp-6]
	push	word ptr [bp-2]
	push	cs
	mov	ax,offset CODE:s@+3624
@47@170:
	push	ax
	push	ss
	lea	ax,word ptr [bp-94]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,14
   ;	
   ;	
   ;	    print_str_cr(s);
   ;	
	push	ss
	lea	ax,word ptr [bp-94]
	push	ax
	call	near ptr CODE:_print_str_cr
	pop	cx
	pop	cx
   ;	
   ;	
   ;	
   ;	    if ((result && (type!=WORD_TOKEN)) || (!result))
   ;	
	or	di,di
	je	short @47@254
	cmp	word ptr [bp-2],1
	jne	short @47@282
@47@254:
	or	di,di
	jne	short @47@310
@47@282:
   ;	
   ;	     {
   ;	      private_poker_message("Unrecognized command",portnum);
   ;	
	push	si
	push	cs
	mov	ax,offset CODE:s@+3640
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
   ;	
   ;	      return;
   ;	
	jmp	@47@1038
@47@310:
   ;	
   ;	     }
   ;	
   ;	
   ;	    if ((result) && (type==WORD_TOKEN))
   ;	
	or	di,di
	jne	@@31
	jmp	@47@1038
@@31:
	cmp	word ptr [bp-2],1
	je	@@32
	jmp	@47@1038
@@32:
   ;	
   ;	      switch(value)
   ;	
	mov	ax,word ptr [bp-4]
	mov	dx,word ptr [bp-6]
	mov	word ptr [bp-14],dx
	mov	word ptr [bp-12],ax
	mov	cx,11
	mov	bx,offset @47@C818
@47@422:
	mov	ax,word ptr cs:[bx]
	cmp	ax,word ptr [bp-14]
	jne	short @47@478
	mov	ax,word ptr cs:[bx+22]
	cmp	ax,word ptr [bp-12]
	je	short @47@534
@47@478:
	add	bx,2
	loop	short @47@422
	jmp	short @47@1010
@47@534:
	jmp	word ptr cs:[bx+44]
@47@562:
   ;	
   ;	       {
   ;	
   ;			 case 0:  /* SEE the current bet */
   ;	         case 5:  /* CALL the current bet */
   ;				  see_bet(portnum);
   ;	
	push	si
	call	near ptr CODE:_see_bet
@47@590:
	pop	cx
   ;	
   ;				  break;
   ;	
	jmp	short @47@1038
@47@618:
   ;	
   ;	
   ;			 case 2:	/* FOLD */
   ;	              fold_player(portnum);
   ;	
	push	si
	call	near ptr CODE:_fold_player
	jmp	short @47@590
@47@646:
   ;	
   ;	              break;
   ;	
   ;	         case 3:    /* BET money */
   ;	              make_bet(portnum,command);
   ;	
	push	word ptr [bp-8]
	push	word ptr [bp-10]
	push	si
	call	near ptr CODE:_make_bet
	jmp	short @47@982
@47@674:
   ;	
   ;	              break;
   ;	
   ;	         case 6:    /* show CARDS */
   ;	              print_poker_cards(portnum);
   ;	
	push	si
	call	near ptr CODE:_print_poker_cards
	jmp	short @47@590
@47@702:
   ;	
   ;	              break;
   ;	
   ;	         case 7:    /* STATUS requested */
   ;	              if (in_public)
   ;	
	cmp	word ptr [bp+10],0
	je	short @47@758
   ;	
   ;	                print_poker_status(server);
   ;	
	push	word ptr CODE:_server
	jmp	short @47@786
@47@758:
   ;	
   ;	              else
   ;	                print_poker_status(portnum);
   ;	
	push	si
@47@786:
	call	near ptr CODE:_print_poker_status
	pop	cx
   ;	
   ;	              break;
   ;	
	jmp	short @47@1038
@47@842:
   ;	
   ;	
   ;	         case 12:   /* JOIN game */
   ;	              register_player(portnum);
   ;	
	push	si
	call	near ptr CODE:_register_player
	jmp	short @47@590
@47@870:
   ;	
   ;	              break;
   ;	
   ;	         case 13:   /* QUIT the game */
   ;	              deregister_player_verbose(portnum);
   ;	
	push	si
	call	near ptr CODE:_deregister_player_verbose
	jmp	short @47@590
@47@898:
   ;	
   ;	              break;
   ;	
   ;	         case 14:   /* ANTE player */
   ;	              ante_player(portnum);
   ;	
	push	si
	call	near ptr CODE:_ante_player
	jmp	short @47@590
@47@926:
   ;	
   ;	              break;
   ;	
   ;	         case 15:   /* DEAL cards */
   ;	              start_game(portnum);
   ;	
	push	si
	call	near ptr CODE:_start_game
	jmp	short @47@590
@47@954:
   ;	
   ;	              break;
   ;	
   ;	         case 17:   /* DIScard */
   ;	              discard(portnum,command);
   ;	
	push	word ptr [bp-8]
	push	word ptr [bp-10]
	push	si
	call	near ptr CODE:_discard
@47@982:
	add	sp,6
   ;	
   ;	              break;
   ;	
	jmp	short @47@1038
@47@1010:
   ;	
   ;	
   ;	         default: break;
   ;	
	jmp	short @47@1038
@47@1038:
   ;	
   ;	
   ;	       }
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_interpret_command	endp
@47@C818	label	word
	db	0
	db	0
	db	2
	db	0
	db	3
	db	0
	db	5
	db	0
	db	6
	db	0
	db	7
	db	0
	db	12
	db	0
	db	13
	db	0
	db	14
	db	0
	db	15
	db	0
	db	17
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	dw	@47@562
	dw	@47@618
	dw	@47@646
	dw	@47@562
	dw	@47@674
	dw	@47@702
	dw	@47@842
	dw	@47@870
	dw	@47@898
	dw	@47@926
	dw	@47@954
   ;	
   ;	int interpret_abuf(void)
   ;	
	assume	cs:CODE
_interpret_abuf	proc	near
	push	bp
	mov	bp,sp
	sub	sp,438
   ;	
   ;	{
   ;	  char s[421];
   ;	  char *start;
   ;	  int sentby, channel, type, temp1, temp2, temp3;
   ;	
   ;	  process_game_level();
   ;	
	call	near ptr CODE:_process_game_level
   ;	
   ;	
   ;	  if (aget_abuffer(&sentby,&channel,s,&type,&temp1,&temp2,&temp3))
   ;	
	push	ss
	lea	ax,word ptr [bp-16]
	push	ax
	push	ss
	lea	ax,word ptr [bp-14]
	push	ax
	push	ss
	lea	ax,word ptr [bp-12]
	push	ax
	push	ss
	lea	ax,word ptr [bp-10]
	push	ax
	push	ss
	lea	ax,word ptr [bp-438]
	push	ax
	push	ss
	lea	ax,word ptr [bp-8]
	push	ax
	push	ss
	lea	ax,word ptr [bp-6]
	push	ax
	call	near ptr CODE:_aget_abuffer
	add	sp,28
	or	ax,ax
	jne	@@33
	jmp	@48@1038
@@33:
   ;	
   ;	  {
   ;	    s[420]=0;
   ;	
	mov	byte ptr [bp-18],0
   ;	
   ;	    switch (type)
   ;	
	mov	bx,word ptr [bp-10]
	cmp	bx,4
	jbe	@@34
	jmp	@48@1010
@@34:
	shl	bx,1
	jmp	word ptr cs:@48@C1026[bx]
@48@170:
   ;	
   ;	    {
   ;	      case 0:                                           /* Normal Message */
   ;	              if (channel!=poker_channel)
   ;	
	mov	ax,word ptr [bp-8]
	cmp	ax,word ptr CODE:_poker_channel
	je	short @48@226
   ;	
   ;	                break; /* throw it out if it's not on our channel */
   ;	
	jmp	@48@1010
@48@226:
   ;	
   ;	
   ;	              start=s;
   ;	
	lea	ax,word ptr [bp-438]
	mov	word ptr [bp-2],ss
	mov	word ptr [bp-4],ax
	jmp	short @48@282
@48@254:
   ;	
   ;	              while ( (*start) && !( (*start=='-') && ( (*(start+1)=='P') || (*(start+1) == 'p')) ) )
   ;	                start++;
   ;	
	inc	word ptr [bp-4]
@48@282:
	les	bx,dword ptr [bp-4]
	cmp	byte ptr es:[bx],0
	je	short @48@394
	les	bx,dword ptr [bp-4]
	cmp	byte ptr es:[bx],45
	jne	short @48@254
	les	bx,dword ptr [bp-4]
	cmp	byte ptr es:[bx+1],80
	je	short @48@394
	les	bx,dword ptr [bp-4]
	cmp	byte ptr es:[bx+1],112
	jne	short @48@254
@48@394:
   ;	
   ;	
   ;	              if (!*start)
   ;	
	les	bx,dword ptr [bp-4]
	mov	al,byte ptr es:[bx]
	cbw	
	or	ax,ax
	jne	short @48@450
   ;	
   ;	                break;
   ;	
	jmp	@48@1010
@48@450:
	jmp	short @48@506
@48@478:
   ;	
   ;	
   ;	              while ((*start) && (*start!=' '))
   ;	                  start++;
   ;	
	inc	word ptr [bp-4]
@48@506:
	les	bx,dword ptr [bp-4]
	cmp	byte ptr es:[bx],0
	je	short @48@562
	les	bx,dword ptr [bp-4]
	cmp	byte ptr es:[bx],32
	jne	short @48@478
@48@562:
   ;	
   ;	
   ;	              interpret_command(start,temp2,1);
   ;	
	mov	ax,1
	push	ax
	push	word ptr [bp-14]
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	jmp	short @48@702
@48@590:
   ;	
   ;	              break;
   ;	
   ;	      case 1: if (*s == '*') return (0);                /* PRIVATE MESSAGE */
   ;	
	cmp	byte ptr [bp-438],42
	jne	short @48@674
	xor	ax,ax
@48@646:
	jmp	@48@1094
@48@674:
   ;	
   ;	              interpret_command(s,temp1,0);
   ;	
	xor	ax,ax
	push	ax
	push	word ptr [bp-12]
	push	ss
	lea	ax,word ptr [bp-438]
	push	ax
@48@702:
	call	near ptr CODE:_interpret_command
	add	sp,8
   ;	
   ;	              break;
   ;	
	jmp	short @48@1010
@48@730:
   ;	
   ;	
   ;	      case 3: if (temp3 == 1)                           /* Login/Logout Message */
   ;	
	cmp	word ptr [bp-16],1
	jne	short @48@786
   ;	
   ;	               {
   ;	                 sprintf(s,"Poker on T%d",poker_channel);
   ;	
	push	word ptr CODE:_poker_channel
	push	cs
	mov	ax,offset CODE:s@+3661
	push	ax
	push	ss
	lea	ax,word ptr [bp-438]
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,10
   ;	
   ;	                 private_poker_message(s,temp2);
   ;	
	push	word ptr [bp-14]
	push	ss
	lea	ax,word ptr [bp-438]
	push	ax
	call	near ptr CODE:_private_poker_message
	add	sp,6
@48@786:
   ;	
   ;	               }
   ;	              if (temp3 == 2) deregister_player(temp2);
   ;	
	cmp	word ptr [bp-16],2
	jne	short @48@842
	push	word ptr [bp-14]
	call	near ptr CODE:_deregister_player
	pop	cx
@48@842:
   ;	
   ;	              break;
   ;	
	jmp	short @48@1010
@48@870:
   ;	
   ;	
   ;	      case 4:                                           /* Multiple Channel Message */
   ;	              if (((temp1==2) || (temp1==1)) && (channel==poker_channel))
   ;	
	cmp	word ptr [bp-12],2
	je	short @48@926
	cmp	word ptr [bp-12],1
	jne	short @48@982
@48@926:
	mov	ax,word ptr [bp-8]
	cmp	ax,word ptr CODE:_poker_channel
	jne	short @48@982
   ;	
   ;	                 deregister_player(temp3);
   ;	
	push	word ptr [bp-16]
	call	near ptr CODE:_deregister_player
	pop	cx
@48@982:
   ;	
   ;	
   ;	              break;
   ;	
	jmp	short @48@1010
@48@1010:
   ;	
   ;	
   ;	    } // end switch
   ;	
   ;	  }
   ;	
	jmp	short @48@1066
@48@1038:
   ;	
   ;	  else  /* (no messages waiting, then sleep a while ) */
   ;	    g_delay(9);
   ;	
	mov	ax,9
	push	ax
	call	near ptr CODE:_g_delay
	pop	cx
@48@1066:
   ;	
   ;	
   ;	  process_game_level();
   ;	
	call	near ptr CODE:_process_game_level
   ;	
   ;	
   ;	  return (1);
   ;	
	mov	ax,1
	jmp	@48@646
@48@1094:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_interpret_abuf	endp
@48@C1026	label	word
	dw	@48@170
	dw	@48@590
	dw	@48@1010
	dw	@48@730
	dw	@48@870
_running	label	word
	db	1
	db	0
   ;	
   ;	void exit_poker_bot(void)
   ;	
	assume	cs:CODE
_exit_poker_bot	proc	near
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    running=0;
   ;	
	mov	word ptr CODE:_running,0
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_exit_poker_bot	endp
   ;	
   ;	int far ginsu_main(void)
   ;	
	assume	cs:CODE
_ginsu_main	proc	far
	push	bp
	mov	bp,sp
	sub	sp,16
   ;	
   ;	{
   ;	  int type,temp1,temp2,temp3,channel,sentby;
   ;	  char s[451];
   ;	  char real_name[15];
   ;	
   ;	  server = get_server();
   ;	
	call	near ptr CODE:_get_server
	mov	word ptr CODE:_server,ax
   ;	
   ;	  initabuffer(2048);
   ;	
	mov	ax,2048
	push	ax
	call	near ptr CODE:_initabuffer
	pop	cx
   ;	
   ;	  register_bot("Poker");
   ;	
	push	cs
	mov	ax,offset CODE:s@+3674
	push	ax
	call	near ptr CODE:_register_bot
	pop	cx
	pop	cx
   ;	
   ;	  get_registered_bot_name_for_myself(real_name,14);
   ;	
	mov	ax,14
	push	ax
	push	ss
	lea	ax,word ptr [bp-16]
	push	ax
	call	near ptr CODE:_get_registered_bot_name_for_myse
	add	sp,6
   ;	
   ;	
   ;	  sprintf(info_line,"Come to Channel: %d",poker_channel);
   ;	
	push	word ptr CODE:_poker_channel
	push	cs
	mov	ax,offset CODE:s@+3680
	push	ax
	push	cs
	mov	ax,offset CODE:_info_line
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,10
   ;	
   ;	  change_my_info_line(info_line);
   ;	
	push	cs
	mov	ax,offset CODE:_info_line
	push	ax
	call	near ptr CODE:_change_my_info_line
	pop	cx
	pop	cx
   ;	
   ;	
   ;	  sprintf(private_poker_header,"|*h1P|*r1#%02d<T%d:%s> ",tswitch,poker_channel,real_name);
   ;	
	push	ss
	lea	ax,word ptr [bp-16]
	push	ax
	push	word ptr CODE:_poker_channel
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+220]
	push	word ptr es:[bx]
	push	cs
	mov	ax,offset CODE:s@+3700
	push	ax
	push	cs
	mov	ax,offset CODE:_private_poker_header
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,16
   ;	
   ;	  sprintf(public_poker_header,"#%02d<T%d:%s> ",tswitch,poker_channel,real_name);
   ;	
	push	ss
	lea	ax,word ptr [bp-16]
	push	ax
	push	word ptr CODE:_poker_channel
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+220]
	push	word ptr es:[bx]
	push	cs
	mov	ax,offset CODE:s@+3724
	push	ax
	push	cs
	mov	ax,offset CODE:_public_poker_header
	push	ax
	call	near ptr CODE:_g_sprintf
	add	sp,16
   ;	
   ;	
   ;	  broadcast_message("");
   ;	
	push	cs
	mov	ax,offset CODE:s@+3739
	push	ax
	call	near ptr CODE:_broadcast_message
	pop	cx
	pop	cx
   ;	
   ;	  broadcast_message("Poker BOT is running! \007\007");
   ;	
	push	cs
	mov	ax,offset CODE:s@+3740
	push	ax
	call	near ptr CODE:_broadcast_message
	pop	cx
	pop	cx
   ;	
   ;	  broadcast_message("Type /BOT to see the BOT name!");
   ;	
	push	cs
	mov	ax,offset CODE:s@+3765
	push	ax
	call	near ptr CODE:_broadcast_message
	pop	cx
	pop	cx
   ;	
   ;	  broadcast_message("");
   ;	
	push	cs
	mov	ax,offset CODE:s@+3796
	push	ax
	call	near ptr CODE:_broadcast_message
	pop	cx
	pop	cx
	jmp	short @50@86
@50@58:
   ;	
   ;	
   ;	
   ;	  while (running)
   ;	  {
   ;	    running = interpret_abuf();
   ;	
	call	near ptr CODE:_interpret_abuf
	mov	word ptr CODE:_running,ax
   ;	
   ;	    next_task();
   ;	
	les	bx,dword ptr CODE:_jmptl
	pushf	
	call	dword ptr es:[bx+204]
@50@86:
	cmp	word ptr CODE:_running,0
	jne	short @50@58
   ;	
   ;	  }
   ;	
   ;	  unregister_bot_myself();
   ;	
	call	near ptr CODE:_unregister_bot_myself
   ;	
   ;	  poker_message("Bot Shutting Down.");
   ;	
	push	cs
	mov	ax,offset CODE:s@+3797
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	  return (1);
   ;	
	mov	ax,1
	jmp	short @50@142
@50@142:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
_ginsu_main	endp
   ;	
   ;	void deal_cards(void)
   ;	
	assume	cs:CODE
_deal_cards	proc	near
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
   ;	
   ;	{
   ;	  int num_cards_in_hand=0;
   ;	
	mov	word ptr [bp-2],0
   ;	
   ;	  int count_player=0;
   ;	
	xor	di,di
   ;	
   ;	  int temp;
   ;	  int tries;
   ;	
   ;	  poker_message("Dealing...");
   ;	
	push	cs
	mov	ax,offset CODE:s@+3816
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	  g_delay(10);
   ;	
	mov	ax,10
	push	ax
	call	near ptr CODE:_g_delay
	pop	cx
   ;	
   ;	
   ;	  for (num_cards_in_hand=0;num_cards_in_hand<5;num_cards_in_hand++)
   ;	
	mov	word ptr [bp-2],0
	jmp	@51@618
@51@58:
   ;	
   ;	   {
   ;	        for (count_player=0;count_player<num_players;count_player++)
   ;	
	xor	di,di
	jmp	@51@562
@51@86:
   ;	
   ;	        if (players[count_player].run_level==DEAL_ROUND)
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	cmp	word ptr cs:_players[bx+66],1
	je	@@35
	jmp	@51@534
@@35:
   ;	
   ;	        {
   ;	           tries=0;
   ;	
	mov	word ptr [bp-4],0
@51@142:
   ;	
   ;	
   ;	           do
   ;	               {
   ;	                temp = rand() % NUM_CARDS;
   ;	
	call	near ptr CODE:_rand
	mov	bx,52
	cwd	
	idiv	bx
	mov	si,dx
   ;	
   ;	
   ;	                if (tries>30)
   ;	
	cmp	word ptr [bp-4],30
	jle	short @51@198
   ;	
   ;	                  srand((unsigned) dans_counter);
   ;	
	les	bx,dword ptr CODE:_jmptl
	les	bx,dword ptr es:[bx+272]
	push	word ptr es:[bx]
	call	near ptr CODE:_srand
	pop	cx
@51@198:
   ;	
   ;	
   ;	               }  while ((card_deck[temp]==NO_CARD) && (tries++)<100);
   ;	
	cmp	byte ptr cs:_card_deck[si],255
	jne	short @51@254
	mov	ax,word ptr [bp-4]
	inc	word ptr [bp-4]
	cmp	ax,100
	jl	short @51@142
@51@254:
   ;	
   ;	
   ;	           if (card_deck[temp]==NO_CARD)
   ;	
	cmp	byte ptr cs:_card_deck[si],255
	jne	short @51@506
   ;	
   ;	                 { poker_message("Having trouble dealing cards (random problems)");
   ;	
	push	cs
	mov	ax,offset CODE:s@+3827
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	                   tries=0;
   ;	
	mov	word ptr [bp-4],0
@51@310:
   ;	
   ;	                   do
   ;	                   { temp++;
   ;	
	inc	si
   ;	
   ;	                     if (temp>=52) temp=0;
   ;	
	cmp	si,52
	jl	short @51@366
	xor	si,si
@51@366:
   ;	
   ;	                     tries++;
   ;	
	inc	word ptr [bp-4]
   ;	
   ;	                   } while ((card_deck[temp]==NO_CARD) && tries<60);
   ;	
	cmp	byte ptr cs:_card_deck[si],255
	jne	short @51@450
	cmp	word ptr [bp-4],60
	jl	short @51@310
@51@450:
   ;	
   ;	
   ;	                   if (card_deck[temp]==NO_CARD)
   ;	
	cmp	byte ptr cs:_card_deck[si],255
	jne	short @51@506
   ;	
   ;	                    {
   ;	                     poker_message("The Card Deck is corrupted, exiting bot");
   ;	
	push	cs
	mov	ax,offset CODE:s@+3874
	push	ax
	call	near ptr CODE:_poker_message
	pop	cx
	pop	cx
   ;	
   ;	                     exit_poker_bot();
   ;	
	call	near ptr CODE:_exit_poker_bot
@51@506:
   ;	
   ;	                    }
   ;	
   ;	                 }
   ;	
   ;	           card_deck[temp] = NO_CARD;
   ;	
	mov	byte ptr cs:_card_deck[si],255
   ;	
   ;	
   ;	           players[count_player].cards_in_hand[num_cards_in_hand] = card_deck[temp];
   ;	
	mov	al,byte ptr cs:_card_deck[si]
	push	ax
	mov	ax,di
	mov	dx,76
	imul	dx
	add	ax,word ptr [bp-2]
	mov	bx,ax
	pop	ax
	mov	byte ptr cs:_players[bx+56],al
   ;	
   ;	           players[count_player].have_cards=1;
   ;	
	mov	ax,di
	mov	dx,76
	imul	dx
	mov	bx,ax
	mov	byte ptr cs:_players[bx+61],1
@51@534:
	inc	di
@51@562:
	cmp	di,word ptr CODE:_num_players
	jge	@@36
	jmp	@51@86
@@36:
	inc	word ptr [bp-2]
@51@618:
	cmp	word ptr [bp-2],5
	jge	@@37
	jmp	@51@58
@@37:
   ;	
   ;	        }
   ;	
   ;	   }
   ;	
   ;	   g_delay(10);
   ;	
	mov	ax,10
	push	ax
	call	near ptr CODE:_g_delay
	pop	cx
   ;	
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_deal_cards	endp
_current_bet	label	word
	db	4 dup (0)
_poker_timer	label	word
	db	4 dup (0)
_broke_player	label	word
	db	2 dup (0)
_bet_limit	label	word
	db	4 dup (0)
_poker_pot	label	word
	db	4 dup (0)
_server	label	word
	db	2 dup (0)
_bet_order	label	word
	db	12 dup (0)
_game_stats	label	word
	db	8 dup (0)
_players	label	word
	db	456 dup (0)
_card_deck	label	byte
	db	52 dup (0)
_public_poker_header	label	byte
	db	40 dup (0)
_private_poker_header	label	byte
	db	40 dup (0)
_info_line	label	byte
	db	21 dup (0)
	?debug	C E9
	?debug	C FA00000000
s@	label	byte
	db	'wait'
	db	0
	db	'deal'
	db	0
	db	'ante'
	db	0
	db	'bet'
	db	0
	db	'discard'
	db	0
	db	'bet'
	db	0
	db	'complete'
	db	0
	db	'Clubs'
	db	0
	db	'Hearts'
	db	0
	db	'Diamonds'
	db	0
	db	'Spades'
	db	0
	db	'Two'
	db	0
	db	'Three'
	db	0
	db	'Four'
	db	0
	db	'Five'
	db	0
	db	'Six'
	db	0
	db	'Seven'
	db	0
	db	'Eight'
	db	0
	db	'Nine'
	db	0
	db	'Ten'
	db	0
	db	'Jack'
	db	0
	db	'Queen'
	db	0
	db	'King'
	db	0
	db	'Ace'
	db	0
	db	'Twos'
	db	0
	db	'Threes'
	db	0
	db	'Fours'
	db	0
	db	'Fives'
	db	0
	db	'Sixes'
	db	0
	db	'Sevens'
	db	0
	db	'Eights'
	db	0
	db	'Nines'
	db	0
	db	'Tens'
	db	0
	db	'Jacks'
	db	0
	db	'Queens'
	db	0
	db	'Kings'
	db	0
	db	'Aces'
	db	0
	db	'C'
	db	0
	db	'|*f1|*h1H|*r1'
	db	0
	db	'|*f1|*h1D|*r1'
	db	0
	db	'S'
	db	0
	db	'2'
	db	0
	db	'3'
	db	0
	db	'4'
	db	0
	db	'5'
	db	0
	db	'6'
	db	0
	db	'7'
	db	0
	db	'8'
	db	0
	db	'9'
	db	0
	db	'10'
	db	0
	db	'J'
	db	0
	db	'Q'
	db	0
	db	'K'
	db	0
	db	'A'
	db	0
	db	'SEE'
	db	0
	db	'CHE'
	db	0
	db	'FOL'
	db	0
	db	'BET'
	db	0
	db	'RAI'
	db	0
	db	'CAL'
	db	0
	db	'CAR'
	db	0
	db	'STA'
	db	0
	db	'POT'
	db	0
	db	'MON'
	db	0
	db	'REM'
	db	0
	db	'FOR'
	db	0
	db	'JOI'
	db	0
	db	'QUI'
	db	0
	db	'ANT'
	db	0
	db	'DEA'
	db	0
	db	'AUT'
	db	0
	db	'DIS'
	db	0
	db	'Your not in the game, type "/b poker join" to join the game'
	db	0
	db	'This game is not yet over!'
	db	0
	db	'Must have two players to play'
	db	0
	db	'You must be in the game to ante'
	db	0
	db	'This is not time to Ante'
	db	0
	db	'This is not the time for you to Ante.'
	db	0
	db	'%s|*r1 has anted'
	db	0
	db	'You must be in the game to bet'
	db	0
	db	'You folded, you'
	db	39
	db	're no longer in this hand.'
	db	0
	db	'You'
	db	39
	db	're not in this hand.'
	db	0
	db	'It is not your turn to bet.'
	db	0
	db	'This is not the time to bet.'
	db	0
	db	'You have already bet %lu.'
	db	0
	db	'You only have %lu, you can'
	db	39
	db	't afford that bet'
	db	0
	db	'You must at least see the current bet of %lu.'
	db	0
	db	'Sorry, a player will go broke this hand, you must bet at leas'
	db	't 1.'
	db	0
	db	'Sorry, you may only see the current bet of %lu.'
	db	0
	db	'Sorry, a bet limit of %lu is imposed.'
	db	0
	db	'%s|*r1 checks.'
	db	0
	db	'%s|*r1 sees the current bet of %lu.'
	db	0
	db	'%s|*r1 has set the bet to %lu.'
	db	0
	db	'%s|*r1 raises %lu to make the bet %lu.'
	db	0
	db	'There is no current bet to see, use "bet 0" to check'
	db	0
	db	'%s|*r1 is lent %lu so he can see the bet.'
	db	0
	db	'Bet limit of 50 imposed for %s|*r1'
	db	0
	db	'%s|*r1 pays %lu to see the current bet of %lu.'
	db	0
	db	'You are not in the game.'
	db	0
	db	'The game has not yet begun.'
	db	0
	db	'You are not in this game.'
	db	0
	db	'%s|*r1 folds.'
	db	0
	db	'You must be in the game to discard.'
	db	0
	db	'This is not the time to discard.'
	db	0
	db	'You have already discarded.'
	db	0
	db	'%s|*r1 is discarding no cards.'
	db	0
	db	'Bad format in discard request, please retry.'
	db	0
	db	'Bad discard request:'
	db	0
	db	'You may not discard all your cards.'
	db	0
	db	'Confusing discard request, please retry.'
	db	0
	db	'%s|*r1 will be discarding 1 card.'
	db	0
	db	'%s|*r1 will be discarding %d cards.'
	db	0
	db	'Port: [%d] '
	db	0
	db	'(private_poker_message): <PokerBot>: '
	db	0
	db	'(poker_message): --> PokerBot: '
	db	0
	db	'%s|*r1 is being forced out for not being able to afford the a'
	db	'nte'
	db	0
	db	'Type "/b %s ante" to ante. Current Ante is %lu'
	db	0
	db	'poker'
	db	0
	db	'Please Ante or Quit the game (you have 45 seconds)'
	db	0
	db	'%s|*r1 is being kicked out for not responding'
	db	0
	db	'%s|*r1 your first to place a bet.'
	db	0
	db	'%s|*r1 your next to place a bet.'
	db	0
	db	'%s|*r1 is being kicked out for not responding'
	db	0
	db	'Please Bet or Quit the game (you have 60 seconds)'
	db	0
	db	'Please Discard or Quit the game (you have 45 seconds)'
	db	0
	db	'%s|*r1 is being kicked out for not responding'
	db	0
	db	'Having trouble dealing cards (random problems)'
	db	0
	db	'The Card Deck is corrupted, ending game.'
	db	0
	db	'Revealing the players cards:'
	db	0
	db	'%s|*r1 '
	db	0
	db	'%2s-%s  '
	db	0
	db	'%s|*r1 wins with '
	db	0
	db	'a Royal Flush'
	db	0
	db	'a Straight Flush'
	db	0
	db	'a Straight'
	db	0
	db	'Four %s'
	db	0
	db	'a %s Flush'
	db	0
	db	'a Full House'
	db	0
	db	'Three %s'
	db	0
	db	'Two Pair, %s and %s'
	db	0
	db	'a Pair of %s'
	db	0
	db	'a %s of %s high'
	db	0
	db	', %s-%s high.'
	db	0
	db	'.'
	db	0
	db	'There was %lu in the pot.'
	db	0
	db	'There was no money in the pot'
	db	0
	db	'%s|*r1 wins with %lu in the pot.'
	db	0
	db	'No money in the pot'
	db	0
	db	'Only one player left. This game is over.'
	db	0
	db	'No final bet, the pot (%lu) stays and we'
	db	39
	db	'll deal again.'
	db	0
	db	'Everyone has anted, now for some betting.'
	db	0
	db	'All bets are in, now discard.'
	db	0
	db	'Discarded cards replaced, now it'
	db	39
	db	's time for the final bets.'
	db	0
	db	'Shuffling...'
	db	0
	db	'It'
	db	39
	db	's screwed dave'
	db	0
	db	'Deck Shuffled'
	db	0
	db	'Player (#%02d):%s|*r1 has left the game'
	db	0
	db	'Player (#%02d):%s|*r1 has left the game'
	db	0
	db	'You'
	db	39
	db	're not in the game!'
	db	0
	db	'You are already in the game!'
	db	0
	db	'No room in the game'
	db	7
	db	0
	db	'<PokerBot>: You must be on the poker channel%c%c             '
	db	' Current Channel: %d'
	db	0
	db	'%s|*r1 has joined the game with %lu money'
	db	0
	db	'The game has not yet begun.'
	db	0
	db	'You have no cards'
	db	0
	db	'You have already folded.'
	db	0
	db	'You are not in this round of play.'
	db	0
	db	'You must ante first.'
	db	0
	db	'You must wait for all others to discard.'
	db	0
	db	'Your Cards: '
	db	0
	db	'%d) %s-%s  '
	db	0
	db	'Announcing Cards'
	db	0
	db	'No Current Players'
	db	0
	db	'#%02d:%s|*r1 with -% 9lu money  at run_level:%s'
	db	0
	db	'#%02d:%s|*r1 with % 9lu money  at run_level:%s'
	db	0
	db	'Current Pot: %lu    Current Bet: %lu'
	db	0
	db	'Token %d/%d'
	db	0
	db	'Bad Token %d/%d'
	db	0
	db	'Unrecognized command'
	db	0
	db	'Poker on T%d'
	db	0
	db	'Poker'
	db	0
	db	'Come to Channel: %d'
	db	0
	db	'|*h1P|*r1#%02d<T%d:%s> '
	db	0
	db	'#%02d<T%d:%s> '
	db	0
	db	0
	db	'Poker BOT is running! '
	db	7
	db	7
	db	0
	db	'Type /BOT to see the BOT name!'
	db	0
	db	0
	db	'Bot Shutting Down.'
	db	0
	db	'Dealing...'
	db	0
	db	'Having trouble dealing cards (random problems)'
	db	0
	db	'The Card Deck is corrupted, exiting bot'
	db	0
CODE	ends
	extrn	_broadcast_message:near
	public	_exit_poker_bot
	public	_running
	public	_interpret_abuf
	public	_interpret_command
	public	_print_poker_status
	public	_print_poker_cards_for_player
	public	_compare_list
	public	_register_player
	public	_deregister_player_verbose
	extrn	F_SCOPY@:far
	public	_process_game_level
	public	_print_roll_over_message
	public	_poker_winner
	public	_check_for_winner
	public	_player_with_hand
	public	_winning_hand
	public	_n_of_a_kind
	public	_is_a_straight
	public	_is_a_flush
	public	_card_less_than
	public	_find_high_card
	public	_has_card_with_value
	public	_process_final_bets
	public	_replace_discards
	public	_process_discard_round
	public	_process_first_bets
	public	_process_bets
	public	_process_ante_round
	public	_empty_deck
	public	_end_game
	public	_discard
	public	_check_for_blank_line
	public	_find_card_in_hand
	public	_fold_player
	public	_see_bet
	public	_make_bet
	public	_check_bet_round
	extrn	F_LXMUL@:far
	public	_read_amount
	public	_ante_player
	public	_start_game
	public	_put_players_in_game
	public	_initialize_game
	public	_reset_round_vars
	public	_announce_all_cards
	public	_get_token
	public	_print_poker_cards
	public	_deregister_player
	public	_poker_message
	public	_private_poker_message
	public	_shuffle_deck
	public	_deal_cards
	public	_player_number
	public	_number_players_in
	public	_tokens
	public	_short_card_names
	public	_short_suit_names
	public	_plural_long_card_names
	public	_long_card_names
	public	_long_suit_names
	public	_current_bet
	public	_poker_timer
	public	_poker_ante
	public	_broke_player
	public	_bet_limit
	public	_poker_pot
	public	_current_better
	public	_first_better
	public	_poker_channel
	public	_server
	public	_game_run_level
	public	_num_players
	public	_bet_order
	public	_game_stats
	public	_players
	public	_card_deck
	public	_public_poker_header
	public	_private_poker_header
	public	_info_line
	public	_run_levels
	public	_ourblock
	extrn	_get_registered_bot_name_for_myse:near
	extrn	_get_server:near
	extrn	_st_copy:near
	extrn	_change_my_info_line:near
	extrn	_register_bot:near
	extrn	_unregister_bot_myself:near
	extrn	_print_string:near
	extrn	_g_delay:near
	extrn	_aput_append_into_buffer:near
	extrn	_aput_into_buffer:near
	extrn	_aget_abuffer:near
	extrn	_initabuffer:near
	extrn	_print_str_cr:near
	extrn	_g_sprintf:near
	extrn	_jmptl:dword
	public	_ginsu_main
	extrn	_srand:near
	extrn	_rand:near
	extrn	_strncpy:near
	extrn	_strncmp:near
	extrn	_strlen:near
	extrn	_strcpy:near
	extrn	_strcat:near
_s@	equ	s@
	end
