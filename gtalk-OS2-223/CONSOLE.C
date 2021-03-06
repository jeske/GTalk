

/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

/* headers */

#define NO_ERROR 0

#define INCL_DOS
#define INCL_KBD
#define INCL_DOSPROCESS
#define INCL_VIO
#define INCL_DOSSEMAPHORES
#include <os2.h>

#include "include.h"
#include "gtalk.h"
#include "console.h"
#include "com.h"

#define SCROLLBACK_LEN (4096*40)
#define KBD_WAIT 0
#define V_TIMEOUT   0x100l               /* wait this long before */
										/* automatically flushing video */



#define MAX_SCREENS 8
/* Video Output Routines */

struct video_scrollback_struct {
  unsigned long int buflen;
  short int *bufstart;
  short int *bufend;
  short int *curbuf;
  short int *dispptr;
  short int *datastart;
  short int keyup;
  int max_lines;
};

struct video_buf_struct {
   unsigned short *bufstart;
   unsigned short buflen;
   unsigned short status_bar_len;
   unsigned short width;
   unsigned short height;
   unsigned short xpos;
   unsigned short ypos;
   struct video_scrollback_struct *backscroll;
   unsigned char scrollback_on;
};


struct video_screen
{
  struct video_buf_struct my_screen;
  short *screenPtr;
  USHORT screenLen;
  USHORT width;
  USHORT height;
  short *endScr;
  short *curLoc;
  short *normalScr;
  int x_pos;
  int y_pos;
  char top;
  char at_last_col;
  int top80;
  char bottom;
  int attrib;
  int movebyte;
  int cur_con_number;
  char used;
  int width_minus_one;

  short *scroll_start;
  unsigned int scroll_length;
  unsigned char top_scroll;
  unsigned char bottom_scroll;

  int old_x_pos;
  int old_y_pos;
  int old_attrib;

  unsigned char elements;
  unsigned char read_number;
  unsigned char element[MAX_ANSI_ELEMENTS];

  unsigned char cur_number;
  short int	portnum;

  void (*next_console_char)(int portnum, int temp);
  struct video_scrollback_struct backscroll;
};


struct local_video_struct {
	HVIO	VioHandle;
	HEV 	LVBDirty;
	struct video_buf_struct def;
	VIOMODEINFO viom;
	TID 	video_flush_tid;
	struct video_buf_struct *current_buf;
};

int cur_console = 0;
int is_mono=0;
unsigned int base_seg;
unsigned int cur_pos1;
unsigned int cur_pos2;
struct video_screen screens[MAX_SCREENS];
struct video_screen *port_screen[MAX_THREADS];
struct local_video_struct local_os2_video;


void refresh_video(void)
{
 if (cur_console == port_screen[tswitch]->cur_con_number)
   DosPostEventSem(local_os2_video.LVBDirty);
}

void refresh_video_port(int port)
{
  if (cur_console == port_screen[port]->cur_con_number)
   DosPostEventSem(local_os2_video.LVBDirty);
}


void hard_refresh_video(void)
{
  DosPostEventSem(local_os2_video.LVBDirty);
}

int direct_buf(char *dest_buf, char *from_buf,unsigned char attribute)
{
  int len = 0;

 while (*from_buf)
 {
  *dest_buf++ = *from_buf++;
  *dest_buf++ = attribute;
  len++;
 }
 return (len);

}

void update_status_bar(void)
{
	   int temp;
	   char temp_str[40];
	   char temp_buf[80];
	   int temp_len;
	   int cur_pos=1;
	   time_t now;
	   int temp_number;
	   struct tm *tblock;


	   sprintf(temp_str," %02d  ",cur_console);

	   temp_len = (direct_buf(temp_buf,temp_str,0x17));
	   puttext(cur_pos,1,cur_pos + temp_len - 1,1,temp_buf);
	   cur_pos +=temp_len;

	   if (sys_info.lock_priority!=255)
		 sprintf(temp_str,"Lck:%02d  ",sys_info.lock_priority);
	   else
		 sprintf(temp_str,"Unlckd  ");

	   temp_len = direct_buf(temp_buf,temp_str,0x17);
	   puttext(cur_pos,1,cur_pos + temp_len - 1,1,temp_buf);
	   cur_pos +=temp_len;


	   if ((temp = system_nodes_free()) == 0)
		 sprintf(temp_str,"FULL  ");
	   else
		 sprintf(temp_str,"%02dNF  ",temp);

	   temp_len = direct_buf(temp_buf,temp_str,0x17);
	   puttext(cur_pos,1,cur_pos + temp_len - 1,1,temp_buf);
	   cur_pos +=temp_len;


	   sprintf(temp_str,"--:--:--  ");

	   temp_len = direct_buf(temp_buf,temp_str,0x17);
	   puttext(cur_pos,1,cur_pos + temp_len - 1,1,temp_buf);
	   cur_pos +=temp_len;


	   sprintf(temp_str,"NoPGS  ");
	   temp_len = direct_buf(temp_buf,temp_str,0x17);
	   puttext(cur_pos,1,cur_pos + temp_len - 1,1,temp_buf);
	   cur_pos +=temp_len;

	   sprintf(temp_str,"%04dc  ",sys_info.day_calls.total);
	   temp_len = direct_buf(temp_buf,temp_str,0x17);
	   puttext(cur_pos,1,cur_pos + temp_len - 1,1,temp_buf);
	   cur_pos +=temp_len;


	   sprintf(temp_str,"% 12s ",version_title);
	   temp_len = direct_buf(temp_buf,temp_str,0x17);
	   puttext(cur_pos,1,cur_pos + temp_len - 1,1,temp_buf);
	   cur_pos +=temp_len;

	   sprintf(temp_str,"--%% ");
	   temp_len = direct_buf(temp_buf,temp_str,0x17);
	   puttext(cur_pos,1,cur_pos + temp_len - 1,1,temp_buf);
	   cur_pos +=temp_len;

/* now do the date and time */
		now = time(NULL);
		tblock = localtime(&now);

	   sprintf(temp_str,"%02d/%02d/%02d ",tblock->tm_mon + 1,
			tblock->tm_mday,tblock->tm_year);
	   temp_len = direct_buf(temp_buf,temp_str,0x17);
	   puttext(cur_pos,1,cur_pos + temp_len - 1,1,temp_buf);
	   cur_pos +=temp_len;


		temp_number = (tblock->tm_hour % 12);
		 if (!temp_number)
			temp_number = 12;
       sprintf(temp_str,"%02d:%02d:%02d%cm ",temp_number,
		  tblock->tm_min,tblock->tm_sec,(tblock->tm_hour<11) ? 'a' : 'p');

	   temp_len = direct_buf(temp_buf,temp_str,0x17);
	   puttext(cur_pos,1,cur_pos + temp_len - 1,1,temp_buf);
	   cur_pos +=temp_len;


}


void make_scrollback_info_bar(struct video_scrollback_struct *temp)
{
       gotoxy(1,local_os2_video.current_buf->height + local_os2_video.current_buf->status_bar_len);
       printf("--- ScrollBack ---  Line %d/%d --- Console #%d --- ",
		   temp->keyup,temp->max_lines,
		   screens[cur_console].portnum);
}

void local_video_flush_thread(void)
{
   ULONG post_count=0;
   ULONG temp;
   struct video_scrollback_struct backtemp;
   short *walk_through;
   short *region_top_ptr;
   int current_scanline;
   int width;
   int region_top;
   int flag,should_draw;

#ifdef DEBUG
   printf("Video Refresh daemon running.\n");
#endif

   while (1)
   {
	if (local_os2_video.current_buf)
	{
		  if (local_os2_video.current_buf->scrollback_on)
		  {
			 if (local_os2_video.current_buf->backscroll)
			 {
				 DosEnterCritSec();
				 backtemp = *(local_os2_video.current_buf->backscroll);
				 DosExitCritSec();

				 if (backtemp.dispptr>=backtemp.bufend)
					   backtemp.dispptr =
						 local_os2_video.current_buf->backscroll->dispptr
							=  backtemp.curbuf;


				   region_top_ptr = walk_through = backtemp.dispptr;
				   region_top = current_scanline = 1 + local_os2_video.current_buf->status_bar_len;
				   width = local_os2_video.current_buf->width;

				   if (walk_through >= backtemp.curbuf)
					 flag=1;
				   else
					 flag=0;

				   should_draw=0;
				   while (((walk_through+width) <= backtemp.bufend)
							 && (((walk_through+width) <= backtemp.curbuf) ||
								   flag )
							  && (current_scanline < local_os2_video.current_buf->height + local_os2_video.current_buf->status_bar_len))
				   {  walk_through += width;
					  current_scanline++;
					  should_draw=1;
				   }

				   if (should_draw)
					   puttext(1,region_top,local_os2_video.current_buf->width,
						 current_scanline - 1,
						 region_top_ptr);

					region_top = current_scanline;
					region_top_ptr = walk_through;

				   /*************************
					* ok, now we have displayed region 1
					*************************/

				   /* now, if we are stopped because we hit the end
					* of the buffer, take care of it, and then
					* do the above loop again, if not, then just
					* display the stuff on the screen
					*/

				   if ((walk_through+width >= backtemp.bufend) &&
						(current_scanline < local_os2_video.current_buf->height + local_os2_video.current_buf->status_bar_len))
				   {
					  puttext(1,region_top,backtemp.bufend - walk_through,
							  region_top,walk_through);
					  puttext(backtemp.bufend - walk_through + 1,region_top,
							  local_os2_video.current_buf->width, region_top,
							  backtemp.bufstart);

					  walk_through += width;
					  (char *)walk_through -= backtemp.buflen;
					  region_top = ++current_scanline;
					  region_top_ptr = walk_through;


					  /* ok, now we need to start displaying
					   * at the top of the buffer like we were
					   */
					   should_draw=0;
					   while ( ((walk_through+width) <= backtemp.curbuf)
								  && (current_scanline < local_os2_video.current_buf->height + local_os2_video.current_buf->status_bar_len))
					   {  walk_through += width;
						  current_scanline++;
						  should_draw=1;
					   }

					   if (should_draw)
						   puttext(1,region_top,
							 local_os2_video.current_buf->width,
							 current_scanline - 1,
							 region_top_ptr);

					   region_top = current_scanline;
					   region_top_ptr = walk_through;


				   }

				   /* ok, now just put the region which is still on
					  the physical screen, and be done */

				  if (current_scanline < local_os2_video.current_buf->height + local_os2_video.current_buf->status_bar_len)
				   {
					   puttext(1,region_top,local_os2_video.current_buf->width,
							 (local_os2_video.current_buf->height -1 + local_os2_video.current_buf->status_bar_len),
							 local_os2_video.current_buf->bufstart);
				   }

				   make_scrollback_info_bar(&backtemp);

			 }  /* if (local_os2_video.current_buf->backscroll) */
			 else
			 {
			   local_os2_video.current_buf->scrollback_on = 0;
			 }

		  }  /* if (local_os2_video.current_buf->scrollback_on) */
		  else
		  {
			  puttext(1,local_os2_video.current_buf->status_bar_len+1,local_os2_video.current_buf->width,
			   local_os2_video.current_buf->height + local_os2_video.current_buf->status_bar_len,
			   (void *)local_os2_video.current_buf->bufstart);
			  gotoxy(local_os2_video.current_buf->xpos+1,
					 local_os2_video.current_buf->ypos+1+local_os2_video.current_buf->status_bar_len);
		  }
	}


	do
	{   
		if (local_os2_video.current_buf)
            update_status_bar();
        DosWaitEventSem((local_os2_video.LVBDirty),V_TIMEOUT);
		DosResetEventSem((local_os2_video.LVBDirty),&post_count);

	} while (!post_count);


   }

}

void console_input_thread(void)
{
   KBDKEYINFO key_info;
   int loop=0;
   USHORT rc;
   unsigned char key;
   int con_number;
   struct video_scrollback_struct *temp;
   int space,flag;
   int should_go_into_scrollback;
   int count,should_refresh;
  PTIB ptib; /* thread info block */
  PPIB ppib; /* process info block */
   struct vt100_conv_type *key_c;
   KBDINFO my_info;

   KbdGetStatus(&my_info,0);
   my_info.fsMask = 0x0104;
   KbdSetStatus(&my_info,0);

   DosGetInfoBlocks(&ptib,&ppib);
   DosSetPriority(PRTYS_THREAD,PRTYC_REGULAR,10,ptib->tib_ptib2->tib2_ultid);

   while ((sys_toggles.system_booting) && (loop++<100))
        DosSleep(50l);

   while (1)
   {
	 rc = KbdCharIn(&key_info, KBD_WAIT ,0);
	 if (rc)
		DosSleep(100);
	 else
	  {
        if ((key_info.fsState & 0x0618) || (key_info.fbStatus & 0x0001) || local_os2_video.current_buf->scrollback_on) /* ALT key down */
        {

           switch(key_info.chScan)
           {

             case 1: /* ESC */
                   local_os2_video.current_buf->scrollback_on=0;
                   hard_refresh_video();
                   break;
             case 72: /* up arrow */
                  temp = local_os2_video.current_buf->backscroll;
                  should_go_into_scrollback = 0;

                  if (!(local_os2_video.current_buf->scrollback_on))
                  {
                     temp->dispptr = temp->curbuf;
                     temp->keyup = 0;
                     should_go_into_scrollback = 1;
                  }

                  if (temp->dispptr > temp->curbuf)
                     space = temp->dispptr - temp->curbuf;
                  else
                     space = (temp->buflen) - (temp->dispptr - temp->curbuf);

                  if (space > local_os2_video.current_buf->width)
                  {
                   if (temp->datastart)
                    {
                     if (temp->dispptr >= temp->datastart)
                        space = temp->dispptr - temp->datastart;
                     else
                        space = (temp->buflen) - (temp->dispptr - temp->datastart);

                     flag=0;
                    }
                    else
                     flag=1;

                    if ((space > local_os2_video.current_buf->width) || flag)
                        {
                          temp->dispptr -= local_os2_video.current_buf->width;
                          if (temp->dispptr<temp->bufstart)
                             (char *)temp->dispptr += temp->buflen;

                          if (should_go_into_scrollback)
                            local_os2_video.current_buf->scrollback_on=1;
                          temp->keyup++;

                          hard_refresh_video();
                        }
                  }

            break;
            case 80: /* down arrow */
                  temp = local_os2_video.current_buf->backscroll;

                  if (local_os2_video.current_buf->scrollback_on)
                  {
                      temp->dispptr += local_os2_video.current_buf->width;
                      if (temp->dispptr>=temp->bufend)
                        (char *)temp->dispptr -= temp->buflen;
                      temp->keyup--;
                      if (!temp->keyup)
                       local_os2_video.current_buf->scrollback_on=0;

                      hard_refresh_video();
                  }
                  break;
                case 73: /* page up */

                      temp = local_os2_video.current_buf->backscroll;
                      count = (local_os2_video.current_buf->height-1+ local_os2_video.current_buf->status_bar_len);
                      should_refresh = 0;
                      should_go_into_scrollback = 0;
                      flag=1;

                      if (!(local_os2_video.current_buf->scrollback_on))
                      {
                         temp->dispptr = temp->curbuf;
                         temp->keyup = 0;
                         should_go_into_scrollback = 1;
                      }

                      while (flag && count--)
                      {
                          flag=0;
                          if (temp->dispptr > temp->curbuf)
                             space = temp->dispptr - temp->curbuf;
                          else
                             space = (temp->buflen) - (temp->dispptr - temp->curbuf);

                          if (space > local_os2_video.current_buf->width)
                          {
                           if (temp->datastart)
                            {
                             if (temp->dispptr >= temp->datastart)
                                space = temp->dispptr - temp->datastart;
                             else
                                space = (temp->buflen) - (temp->dispptr - temp->datastart);

                             flag=0;
                            }
                            else
                             flag=1;

                            if ((space > local_os2_video.current_buf->width) || flag)
                                {
                                  temp->dispptr -= local_os2_video.current_buf->width;
                                  if (temp->dispptr<temp->bufstart)
                                     (char *)temp->dispptr += temp->buflen;
                                  flag=1;
                                  temp->keyup++;
                                  should_refresh=1;


                                }
                          }
                    }

                    if (should_refresh)
                    {
                      hard_refresh_video();
                      if (should_go_into_scrollback)
                       local_os2_video.current_buf->scrollback_on=1;
                    }
                break;
                case 81: /* page down */
                  temp = local_os2_video.current_buf->backscroll;

                  if (local_os2_video.current_buf->scrollback_on)
                  {
                      flag=1;
                      count=local_os2_video.current_buf->height-1+ local_os2_video.current_buf->status_bar_len;

                      while (flag && count--)
                      {
                          temp->dispptr += local_os2_video.current_buf->width;
                          if (temp->dispptr>=temp->bufend)
                            (char *)temp->dispptr -= temp->buflen;
                          temp->keyup--;

                          if (!temp->keyup)
                           { flag=0;
                             local_os2_video.current_buf->scrollback_on=0;
                           }
                      }

                      hard_refresh_video();
                 }
                 break;
              default: break;
            }; /* end switch */


        }
        else /* no ALT key down */
        {
            int in_vt_list = 0;

            if ((key_info.chScan>=59) && (key_info.chScan<=68))
            {
                con_number = key_info.chScan - 59;
                if (con_number<MAX_SCREENS)
                  {
                    switch_virtual_console(con_number);
                  }
            }

            for (key_c=vt100_key_list;key_c->vt100_key_string;key_c++)
            {
              if (key_info.chScan == key_c->scan_code)
               {
                 int len;
                 in_vt_list = 1;
                 for (len=0;len<key_c->key_code_len;len++)
                  put_in_input_buf(screens[cur_console].portnum,
                     key_c->vt100_key_string[len]);
               }
            }

            if (!in_vt_list)
            if ((key_info.chChar != 0x00) && (key_info.chChar !=0xE0))
              put_in_input_buf(screens[cur_console].portnum,key_info.chChar);
      } /* end no alt key down */
    } /* end "rc" error */

   } /* end while */
};


void init_local_os2_video(void)
{
	USHORT	rc;
	VIOMODEINFO viom;
	int count=0;
	int loop;
	int height, width;
	TID dummy;

	for (loop=0;loop<MAX_SCREENS;loop++)
	 screens[loop].backscroll.bufstart=0;

	local_os2_video.current_buf = NULL;

	local_os2_video.viom.cb = sizeof(VIOMODEINFO);
	rc = VioGetMode(&local_os2_video.viom,0);

	switch (rc)
		{
			case NO_ERROR: break;
			default:  break;
					printf("Error getting Video Mode\n");
					exit(1);
		}
	width = local_os2_video.viom.col;
	height = local_os2_video.viom.row;


	local_os2_video.def.status_bar_len = 1;

	local_os2_video.def.buflen = ((width * height) * 2);
	printf("Video Width: %d Height: %d\n",local_os2_video.viom.col,
		local_os2_video.viom.row);
	local_os2_video.def.width = width;
	local_os2_video.def.height = height - local_os2_video.def.status_bar_len;
	local_os2_video.def.ypos=0;
	local_os2_video.def.scrollback_on=0;

	DosCreateEventSem(NULL,&(local_os2_video.LVBDirty),0,0);
	DosCreateThread(&(local_os2_video.video_flush_tid),(PFNTHREAD)local_video_flush_thread,0,0,4096 * 2);
	DosCreateThread(&(dummy),(PFNTHREAD)console_input_thread,0,0,4096 * 2);



};

void end_local_os2_video(void)
{
  if (local_os2_video.video_flush_tid);
   {
     DosKillThread(local_os2_video.video_flush_tid);
     local_os2_video.video_flush_tid=0;
   }

}


int index_of_console(int num)
{
   return (port_screen[num]->cur_con_number);
}

void backscroll_line(struct video_screen *vptr,int *line_start)
{
   int count = vptr->width;
   short *from = vptr->screenPtr;
   short *curbuf_temp;
   short *curbuf_temp_orig;
   int flag;
   int flag2=0;

   if (vptr->backscroll.bufstart)
   {
       curbuf_temp_orig = curbuf_temp = vptr->backscroll.curbuf;


		while (count)
        {

           *(curbuf_temp++) = *(from++);
		   count--;

           if (curbuf_temp >= vptr->backscroll.bufend)
             { (char *)curbuf_temp -= vptr->backscroll.buflen;
               curbuf_temp_orig = curbuf_temp;
               flag2=1;
             }

           if ( (curbuf_temp_orig <= (vptr->backscroll.dispptr)) &&
               (curbuf_temp > vptr->backscroll.dispptr))
			 {
			   vptr->backscroll.dispptr+=vptr->width;

			   if (vptr->backscroll.dispptr >= vptr->backscroll.bufend)
				 (char *)vptr->backscroll.dispptr -= vptr->backscroll.buflen;
			 }
		}

        if (vptr->backscroll.keyup<vptr->backscroll.max_lines)
           vptr->backscroll.keyup++;

        vptr->backscroll.curbuf = curbuf_temp;
        if (flag2)
          vptr->backscroll.datastart=0;
   }

};

int console_is_mono(void)
{
 return (is_mono);
}


char is_a_console[MAX_THREADS];

char color_lookup[] = {0,4,2,6,1,5,3,7};
			/* Conversion array between screen and ANSI colors */

/* Color list:
   0 = Black, 1 = Red, 2 = Green, 3 = Brown, 4 = Blue,
   5 = Purple, 6 = Cyan, 7 = White */

void auto_reboot_task(void);

void init_screen_vars(struct video_screen *vptr)
{
  int screen_size = vptr->my_screen.width * (vptr->my_screen.height);
  int count;
  short *temp;

  vptr->screenPtr = vptr->my_screen.bufstart;
  vptr->top = 0;
  vptr->bottom = vptr->my_screen.height;
  vptr->attrib = 0x0700;
  vptr->at_last_col = 0;
  vptr->endScr = (short *) vptr->screenPtr + screen_size;
  vptr->top80 = (vptr->top * vptr->my_screen.width);
  vptr->normalScr = vptr->screenPtr + vptr->top80;
  vptr->curLoc = vptr->normalScr;
  vptr->movebyte = (vptr->my_screen.height - vptr->top)
					   * vptr->my_screen.width * 2;

  vptr->next_console_char = a_send_console_char;
  vptr->x_pos = 0;
  vptr->y_pos = 0;

  vptr->old_x_pos = 0;
  vptr->old_y_pos = 0;
  vptr->scroll_start = vptr->normalScr;
  vptr->scroll_length = (vptr->my_screen.height - vptr->top) *
						 vptr->my_screen.width;
  vptr->top_scroll = 0;
  vptr->bottom_scroll = vptr->bottom;

  vptr->width_minus_one = vptr->width-1;

  temp = vptr->screenPtr;
  while (temp < vptr->endScr)
   *temp++ = 0x0720;
}

void init_scrollback(struct video_buf_struct *sbptr1)
{
 struct video_scrollback_struct *sbptr = sbptr1->backscroll;

 sbptr->curbuf = sbptr->bufstart;
 sbptr->bufend = (char *)sbptr->bufstart + sbptr->buflen;
 sbptr->datastart = sbptr->dispptr = sbptr->bufstart;
 sbptr->max_lines = ((sbptr->buflen/(sbptr1->width * 2)));

}


int allocate_a_console(int portnum)
{
  int count;
  struct video_screen *vptr;

  if (!portnum)
	{
		 port_screen[0]=&screens[0];
		 screens[0].used=1;
		 is_a_console[0]=1;

		 DosAllocMem(&(screens[0].screenPtr),
			   local_os2_video.def.buflen,PAG_READ | PAG_WRITE | PAG_COMMIT);

		 vptr = &screens[0];
		 vptr->my_screen = local_os2_video.def;
		 vptr->my_screen.bufstart = screens[0].screenPtr;
		 vptr->width = local_os2_video.def.width;
		 vptr->height = local_os2_video.def.height;
		 vptr->screenLen = local_os2_video.def.buflen;
		 vptr->cur_con_number = 0;
		 vptr->portnum = portnum;

		 init_screen_vars(vptr);

		 hard_refresh_video();


		 DosAllocMem(&(vptr->backscroll.bufstart),SCROLLBACK_LEN,
			PAG_READ | PAG_WRITE | PAG_COMMIT);
		 vptr->backscroll.buflen = SCROLLBACK_LEN;
		 vptr->my_screen.backscroll = &(vptr->backscroll);
		 init_scrollback(&vptr->my_screen);

		 gettext(1,local_os2_video.def.status_bar_len + 1,local_os2_video.def.width,
		 local_os2_video.def.height + local_os2_video.def.status_bar_len,
				   vptr->my_screen.bufstart);

		 local_os2_video.current_buf = &(vptr->my_screen);
		 return 0;
	}

  for (count=1;count<MAX_SCREENS;count++)
	if (!screens[count].used)
	   {
		 vptr=&screens[count];
		 port_screen[portnum]=vptr;
		 vptr->used=1;
		 is_a_console[portnum]=1;

		 DosAllocMem(&(vptr->screenPtr),
			   local_os2_video.def.buflen,PAG_READ | PAG_WRITE | PAG_COMMIT);


		 vptr->my_screen = local_os2_video.def;
		 vptr->my_screen.bufstart = vptr->screenPtr;
		 vptr->width = local_os2_video.def.width;
		 vptr->height = local_os2_video.def.height - local_os2_video.def.status_bar_len;
		 vptr->screenLen = local_os2_video.def.buflen;
		 vptr->cur_con_number = count;
		 vptr->portnum = portnum;
		 init_screen_vars(vptr);


		 DosAllocMem(&(vptr->backscroll.bufstart),SCROLLBACK_LEN,
			PAG_READ | PAG_WRITE | PAG_COMMIT);
		 vptr->backscroll.buflen = SCROLLBACK_LEN;
		 vptr->my_screen.backscroll = &(vptr->backscroll);
         init_scrollback(&vptr->my_screen);


		 return count;
	   }

  return -1;  // NONE could be allocated
}




void switch_virtual_console(int virt_cons)
{
   int portnum;

   if (virt_cons<0 || virt_cons>MAX_SCREENS)
	 return;

   if (!screens[virt_cons].used)
    return;

   portnum = screens[virt_cons].portnum;

   if (portnum<0 || portnum>MAX_THREADS)
	 return;

   if (!is_a_console[portnum])
     return;

   if (port[portnum].active)
   {
	   DosEnterCritSec();

	   cur_console = virt_cons;
	   local_os2_video.current_buf = &(port_screen[portnum]->my_screen);

	   hard_refresh_video();
	   DosExitCritSec();
   }

}

void clear_console(int virt_cons, int mode)
{
  struct video_screen *cur_cons = port_screen[virt_cons];
  short *cur_ptr;
  short *end_ptr;

  switch (mode)
  {
	case 0: cur_ptr = cur_cons->curLoc;
			end_ptr = cur_cons->endScr;
			break;
	case 1: cur_ptr = cur_cons->normalScr;
			end_ptr = cur_cons->curLoc + 1;
			break;
	default: cur_ptr = cur_cons->normalScr;
			 end_ptr = cur_cons->endScr;
			 cur_cons->x_pos = 0;
			 cur_cons->y_pos = 0;
			 cur_cons->curLoc = cur_cons->normalScr;
			 break;
  }

  while (cur_ptr < end_ptr) *cur_ptr++ = 0x0720;
}

void clear_to_eol_console(int virt_cons, int mode)
{
  struct video_screen *cur_cons = port_screen[virt_cons];
  short *cur_ptr;
  short *end_ptr;

  switch (mode)
  {
	case 0: cur_ptr = cur_cons->curLoc;
			end_ptr = cur_ptr + (cur_cons->width - cur_cons->x_pos);
			break;
	case 1: cur_ptr = cur_cons->curLoc - cur_cons->x_pos;
			end_ptr = cur_cons->curLoc + 1;
			break;
	default: cur_ptr = cur_cons->curLoc - cur_cons->x_pos;
			 end_ptr = cur_ptr + cur_cons->width;
			 break;
  }

 while (cur_ptr < end_ptr) *cur_ptr++ = 0x0720;
}

void position_console(int virt_cons, int x_pos, int y_pos, int rel)
{
  struct video_screen *cur_cons = port_screen[virt_cons];
  int cursadr;

  if (rel)
  {
	cur_cons->x_pos += x_pos;
	cur_cons->y_pos += y_pos;
  } else
  {
	cur_cons->x_pos = x_pos;
	cur_cons->y_pos = y_pos;
  }
  if (cur_cons->x_pos < 0) cur_cons->x_pos = 0;
  if (cur_cons->x_pos > cur_cons->width_minus_one) cur_cons->x_pos = cur_cons->width_minus_one;
  if (cur_cons->y_pos < 0) cur_cons->y_pos = 0;
  if (cur_cons->y_pos >= cur_cons->bottom) cur_cons->y_pos =
	cur_cons->bottom - 1;
  cur_cons->curLoc = (cur_cons->screenPtr) +
	   (((cur_cons->top)+((cur_cons->y_pos)))*cur_cons->width+(cur_cons->x_pos));
  cur_cons->my_screen.xpos = x_pos;
  cur_cons->my_screen.ypos = y_pos;
}

void move_back_line(int virt_cons, unsigned int char_move)
{
   struct video_screen *cur_cons = port_screen[virt_cons];
   short *next_move;
   short *end_move;
   short *start_move;
   int clr_with;

   clr_with = cur_cons->attrib & 0xFF00;
   start_move = cur_cons->curLoc;
   end_move = start_move + (cur_cons->width - cur_cons->x_pos);
   if ((cur_cons->x_pos + char_move) < cur_cons->width)
   {
	 next_move = cur_cons->curLoc + char_move;
	 start_move = cur_cons->curLoc;
	 while (next_move<end_move) *start_move++ = *next_move++;
   }
   while (start_move<end_move) *start_move++ = clr_with;
}

void scroll_up_at_cursor(int virt_cons, int dir)
{
   struct video_screen *cur_cons = port_screen[virt_cons];
   int clr_with;
   short *beginScr;
   short *endofScr;
   short *lastLine;
   short *nextLine;
   unsigned int real_scroll_length;

   if ((cur_cons->y_pos>=cur_cons->top_scroll) ||
	   (cur_cons->y_pos<cur_cons->bottom_scroll))
   {
	 clr_with = cur_cons->attrib & 0xFF00;
	 beginScr = cur_cons->curLoc - cur_cons->x_pos;
	 endofScr = cur_cons->scroll_start + cur_cons->scroll_length;
	 lastLine = endofScr - cur_cons->width;
	 nextLine = beginScr + cur_cons->width;
	 real_scroll_length = ((unsigned int) lastLine) -
						  ((unsigned int) beginScr);

	 if (dir) memmove(beginScr,nextLine,real_scroll_length);
	 {
		while (lastLine > beginScr)
		{
		  endofScr--;
		  lastLine--;
		  *endofScr = *lastLine;
		}
	 }

	 if (dir)
	 {
	   while (lastLine < endofScr)
		*lastLine++ = clr_with;
	 } else
	 {
	   while (beginScr < nextLine)
		*beginScr++ = clr_with;
	 }
   }
}

/* Scroll the view up one line on CONSOLE */

void scroll_view(int virt_cons)
 {
   struct video_screen *cur_cons = port_screen[virt_cons];
   int clr_with = cur_cons->attrib & 0xFF00;
   short *beginScr = cur_cons->scroll_start;
   short *endofScr = cur_cons->scroll_start + cur_cons->scroll_length;
   short *lastLine = endofScr - cur_cons->width;
   short *nextLine = beginScr + cur_cons->width;
   unsigned int real_scroll_length = ((unsigned int) lastLine) -
						  ((unsigned int) beginScr);

   backscroll_line(cur_cons,(int *)NULL);

   memmove(beginScr,nextLine,real_scroll_length);
								 /* Move the screen up one line */
								 /* on console with move return */

   while (lastLine < endofScr)
	 *lastLine++ = clr_with;

 };

void set_scroll_region(int virt_cons, int low, int high)
{
   struct video_screen *cur_cons = port_screen[virt_cons];

   low--;
   if (high >= cur_cons->bottom) high=cur_cons->bottom;
   if (low<0) low=0;

   cur_cons->scroll_start = cur_cons->normalScr + (cur_cons->width * low);
   cur_cons->scroll_length = (high - low) * cur_cons->width;
   cur_cons->top_scroll = low;
   cur_cons->bottom_scroll = high;
}

void a_reading_ansi(int portnum, unsigned int temp)
{
  struct video_screen  *cur_cons = port_screen[portnum];

  if ((temp>='0') && (temp<='9'))
  {
	cur_cons->cur_number = (cur_cons->cur_number * 10) + (temp - '0');
	cur_cons->read_number = 1;
	return;
  }

  if ((cur_cons->read_number) || (temp==';'))
  {
	 if (cur_cons->elements<MAX_ANSI_ELEMENTS)
	  cur_cons->element[cur_cons->elements++] = cur_cons->cur_number;
	 cur_cons->read_number = 0;
  }
  cur_cons->cur_number = 0;
  switch (temp)
  {
	case '?':
	case ';':   return;
	case 'D':   if (cur_cons->elements)
					position_console(portnum,-cur_cons->element[0],0,1);
				else position_console(portnum,-1,0,1);
				break;
	case 'C':   if (cur_cons->elements)
					position_console(portnum,cur_cons->element[0],0,1);
				else position_console(portnum,1,0,1);
				break;
	case 'A':   if (cur_cons->elements)
					position_console(portnum,0,-cur_cons->element[0],1);
				else position_console(portnum,0,-1,1);
				break;
	case 'B':   if (cur_cons->elements)
					position_console(portnum,0,cur_cons->element[0],1);
				else position_console(portnum,0,1,1);
				break;
	case 'f':
	case 'H':
	  {
		int row = 1;
		int col = 1;
		if (cur_cons->elements>0) row = cur_cons->element[0];
		if (cur_cons->elements>1) col = cur_cons->element[1];
		if (row == 0) row = 1;
		if (col == 0) col = 1;
		position_console(portnum,col-1,row-1,0);
	  }
	  break;
	case 'J':   {
				  int mode = 0;
				  if (cur_cons->elements) mode = cur_cons->element[0];
				  clear_console(portnum,mode);
				}
				break;
	case 'L':   if (cur_cons->elements)
				{
				  int lines = cur_cons->element[0];
				  while (lines>0)
				  {
					scroll_up_at_cursor(portnum,0);
					lines--;
				  }
				}
				break;
	case 'M':   if (cur_cons->elements)
				{
				  int lines = cur_cons->element[0];
				  while (lines>0)
				  {
					scroll_up_at_cursor(portnum,1);
					lines--;
				  }
				}
				break;
	case 'P':   if (cur_cons->elements)
				  move_back_line(portnum,cur_cons->element[0]);
				break;
	case 'K':   {
				  int mode = 0;
				  if (cur_cons->elements) mode = cur_cons->element[0];
				  clear_to_eol_console(portnum,mode);
				}
				break;
	case 's':   cur_cons->old_x_pos = cur_cons->x_pos;
				cur_cons->old_y_pos = cur_cons->y_pos;
				break;
	case 'u':   position_console(portnum,cur_cons->old_x_pos,
				   cur_cons->old_y_pos,0);
				break;
	case 'r':   {
				  int low = 1;
				  int high = cur_cons->bottom;

				  if (cur_cons->elements>0) low=cur_cons->element[0];
				  if (cur_cons->elements>1) high=cur_cons->element[1];
				  if (low<=high)
					set_scroll_region(portnum,low,high);
				}
				break;
	case 'm':
	   {
		  int count = 0;
		  int cthing;
		  if (!cur_cons->elements) cur_cons->attrib = 0x0700;
		  while (count<cur_cons->elements)
		  {
			cthing = cur_cons->element[count];
			switch (cthing)
			{
			  case 0:
			  case 27: cur_cons->attrib = 0x0700;
					   break;
			  case 1:  cur_cons->attrib = (cur_cons->attrib | 0x0800);
					   break;
			  case 5:  cur_cons->attrib = (cur_cons->attrib | 0x8000);
					   break;
			  case 7:  cur_cons->attrib = 0x7000;
					   break;
			  case 21:
			  case 22: cur_cons->attrib = (cur_cons->attrib & 0xF7FF);
					   break;
			  case 25: cur_cons->attrib = (cur_cons->attrib & 0x7FFF);
					   break;
			  default:
				if ((cthing>=30) && (cthing<=37))
				{
				   port_screen[portnum]->attrib =
					 (port_screen[portnum]->attrib & 0xF800) |
					 (color_lookup[cthing - 30] << 8);
				}
				if ((cthing>=40) && (cthing<=47))
				{
				   port_screen[portnum]->attrib =
					 (port_screen[portnum]->attrib & 0x8700) |
					 (color_lookup[cthing - 40] << 12);
				}
				break;
			}
			count++;
		  }
	   }
  }
  cur_cons->next_console_char = a_send_console_char;
}

void move_cursor(int portnum)
{
  struct video_screen *cur_cons = port_screen[portnum];
  unsigned int cursadr;

  if (cur_cons->x_pos > cur_cons->width)
	cur_cons->x_pos = cur_cons->width;

  cur_cons->my_screen.xpos = cur_cons->x_pos;
  cur_cons->my_screen.ypos = cur_cons->y_pos;

  if (cur_console == port_screen[tswitch]->cur_con_number)
		 gotoxy(local_os2_video.current_buf->xpos+1,
				local_os2_video.current_buf->ypos+1+local_os2_video.current_buf->status_bar_len);}


void a_got_ansi(int portnum, int temp)
{
  struct video_screen  *cur_cons = port_screen[portnum];

  switch (temp)
  {
	case '[':  cur_cons->cur_number = 0;
			   cur_cons->elements = 0;
			   cur_cons->read_number = 0;
			   cur_cons->next_console_char = a_reading_ansi;
			   return;
	case '7':  cur_cons->old_attrib = cur_cons->attrib;
			   cur_cons->old_x_pos = cur_cons->x_pos;
			   cur_cons->old_y_pos = cur_cons->y_pos;
			   break;
	case '8':  cur_cons->attrib = cur_cons->old_attrib;
			   position_console(portnum,cur_cons->old_x_pos,
				   cur_cons->old_y_pos,0);
			   break;
	case 'E':  cur_cons->x_pos = 0;
	case 'D':  cur_cons->y_pos++;
			   if (cur_cons->y_pos >= cur_cons->bottom_scroll)
			   {
				 scroll_view(portnum);
				 cur_cons->y_pos--;
			   }
			   cur_cons->curLoc = (cur_cons->screenPtr +
				 ((cur_cons->top)+(cur_cons->y_pos))*cur_cons->width +
				 (cur_cons->x_pos));
			   move_cursor(portnum);
			   break;	 /* recalculate screen pos */
	case 'M':  cur_cons->y_pos--;
			   if ((cur_cons->y_pos < cur_cons->top_scroll) ||
				   (cur_cons->y_pos >= cur_cons->bottom_scroll))
			   {
				 scroll_up_at_cursor(portnum,0);
				 cur_cons->y_pos++;
			   }
			   cur_cons->curLoc = (cur_cons->screenPtr +
				 ((cur_cons->top)+(cur_cons->y_pos))*cur_cons->width +
				 (cur_cons->x_pos));
			   move_cursor(portnum);
			   break;	 /* recalculate screen pos */
  }
  if (temp != 27) cur_cons->next_console_char = a_send_console_char;

}



void send_char_keyboard(int portnum, char charput)
{
  (port_screen[portnum]->next_console_char)(portnum, (unsigned int) charput);

	refresh_video_port(portnum);

}

void send_chars_keyboard(int portnum, char *charptr, int length)
{
  while (length>0)
  {
	(port_screen[portnum]->next_console_char)(portnum, (unsigned int) (*charptr++));
	length--;
  }

	refresh_video_port(portnum);
}

void a_send_console_char(int portnum, int temp)
{
  struct video_screen  *cur_cons = port_screen[portnum];
  int cursadr;

  switch (temp)
  {
	case 27: cur_cons->next_console_char = a_got_ansi;
			 return;
	case 12: clear_console(portnum,2);
			 break;
	case 13: cur_cons->x_pos = 0;		 /* return = back to begin */
			 cur_cons->curLoc = ((cur_cons->screenPtr)
			  + ((cur_cons->top)+(cur_cons->y_pos))*cur_cons->width);
			 break;
	case 10: cur_cons->y_pos++;
			 if (cur_cons->y_pos >= cur_cons->bottom_scroll)
					 /* if we're at bottom */
			 {
			   if (cur_cons->y_pos == cur_cons->bottom_scroll)
				scroll_view(portnum);		 /* and scroll it! */
			   cur_cons->y_pos--;			 /* go back up a line */
			 }
			 cur_cons->curLoc = (cur_cons->screenPtr +
			   ((cur_cons->top)+(cur_cons->y_pos))*cur_cons->width +
			   (cur_cons->x_pos));
			 break;    /* recalculate screen pos */
	case 8:  cur_cons->x_pos--; 			  /* backspace on screen */
			 if (cur_cons->x_pos<0)
			 {
			   cur_cons->x_pos = cur_cons->width_minus_one;
									/* reset to end of last line */
			   cur_cons->y_pos--;			 /* if we're at left */
			   if (cur_cons->y_pos<0) cur_cons->y_pos=0;
			 }
			 cur_cons->curLoc = ((cur_cons->screenPtr) +
				((cur_cons->top)+(cur_cons->y_pos))*cur_cons->width +
				(cur_cons->x_pos));   /* recalc screen */
			 break;
	case 7:  beep_console(2000,120);
			 break;
	case 9:  /* TAB */
			 { int loop=0;
			   for (loop=0;loop<5;loop++)
				{
					a_send_console_char(portnum,' ');
				}
			 }
			 break;
	default: if (!temp) break;
			 *(cur_cons->curLoc)++ = ((unsigned char) temp)
					| (cur_cons->attrib);
					/* put it on the screen */
			 cur_cons->x_pos++; 			  /* go over a character */
             if (cur_cons->x_pos >= cur_cons->width)
											 /* if we're at right edge */
			 {
			   cur_cons->x_pos = 0; 		 /* go back to bottom */
			   cur_cons->y_pos++;			 /* go to next line */
			   if (cur_cons->y_pos==cur_cons->bottom_scroll)
			   {
				 cur_cons->y_pos--;
				 scroll_view(portnum);
			   }
			 cur_cons->curLoc = ((cur_cons->screenPtr) +
			   ((cur_cons->top)+(cur_cons->y_pos))*cur_cons->width +
			   (cur_cons->x_pos));
			 }
			 break;
  }
  cur_cons->my_screen.xpos = cur_cons->x_pos;
  cur_cons->my_screen.ypos = cur_cons->y_pos;

}



/* This routine initializes the display */
/* num_rows = is the number of lines in status bar */

void init_display(int num_rows)
 {

 };

/* This creates the status bar at the top of the screen */

void create_bar(int virt_cons)
 {
};

/* Do a direct screen write: DANGER: this can screw up a lot. */

void direct_screen(int y, int x, unsigned int attrib,unsigned char *string)
 {
 };

void direct_screen_override(int y, int x, unsigned int attrib,unsigned char *string)
 {
 };


void beep_console(unsigned int pitch, unsigned int length)
{
    DosBeep(pitch,length);
};

void console_alarm(void)
{
	int loop;
	int loop2;

	for (loop2=0;loop2<4;loop2++)
	  {
		 for(loop=0;loop<4;loop++)
		  { beep_console(2000,2);
			beep_console(1700,2);}
		 for (loop=0;loop<4;loop++)
		 { beep_console(1500,2);
		   beep_console(1200,2);
		 }
	  }

 }


 /**************************************************
  *    these routines get stuff from console	   *
  **************************************************/


/* get a key from the console */

void get_char_keyboard(int portnum, int *charput, int *isthere)
 {
   *charput = get_from_input_buf(portnum,1);
   if (*charput==-1)
	 *isthere=0;
   else
	 *isthere=1;

 };

int char_in_buf_keyboard(int portnum)
{
   return(char_in_input_buf(portnum));
}

int get_first_char_keyboard(int portnum)
{
  return (get_from_input_buf(portnum,0));
}


int get_nchar_keyboard(int portnum)
 {
  return (get_from_input_buf(portnum,1));
 };

void put_char_in_buffer_keyboard(char temp, int portnum)
{
}

void empty_inbuffer_keyboard(int portnum)
{
}

void empty_outbuffer_keyboard(int portnum)
{
}

void wait_for_xmit_keyboard(int portnum,int ticks)
{
    next_task();
}

