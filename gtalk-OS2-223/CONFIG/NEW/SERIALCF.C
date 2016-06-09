
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

int is_saved=1;
int num_ports_loaded=0;

#define NUM_BOARDS 7
#define NUM_FIELDS 14

void show_board_type(struct serial_config_info *port);
void show_dial_in(struct serial_config_info *port);
void show_fixed_dte_speed(struct serial_config_info *port);
void show_rts_cts(struct serial_config_info *port);
void show_init_string(struct serial_config_info *port);
void show_de_init_string(struct serial_config_info *port);
void show_baud_rate(struct serial_config_info *port);
void show_io_address(struct serial_config_info *port);
void show_digi_loopup_address(struct serial_config_info *port);
void show_int_num(struct serial_config_info *port);
void show_port_num(struct serial_config_info *port);
void show_os2_com_name(struct serial_config_info *port);
void show_verify_node(struct serial_config_info *port);
void show_restrict_level(struct serial_config_info *port);

void edit_board_type(struct serial_config_info *port);
void edit_dial_in(struct serial_config_info *port);
void edit_fixed_dte_speed(struct serial_config_info *port);
void edit_rts_cts(struct serial_config_info *port);
void edit_init_string(struct serial_config_info *port);
void edit_de_init_string(struct serial_config_info *port);
void edit_baud_rate(struct serial_config_info *port);
void edit_io_address(struct serial_config_info *port);
void edit_digi_loopup_address(struct serial_config_info *port);
void edit_int_num(struct serial_config_info *port);
void edit_port_num(struct serial_config_info *port);
void edit_os2_com_name(struct serial_config_info *port);
void edit_verify_node(struct serial_config_info *port);
void edit_restrict_level(struct serial_config_info *port);

char fields_avail[NUM_BOARDS][NUM_FIELDS] =
{
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1 }
};

struct disp_edit_item
{
  int first_row;
  int last_row;
  void (*display)(struct serial_config_info *port);
  void (*edit)(struct serial_config_info *port);
} items[] =
{
  {  6,  7, show_board_type,            edit_board_type },
  {  9,  9, show_dial_in,               edit_dial_in },
  { 10, 10, show_fixed_dte_speed,       edit_fixed_dte_speed },
  { 11, 11, show_rts_cts,               edit_rts_cts },
  { 12, 12, show_init_string,           edit_init_string },
  { 13, 13, show_de_init_string,        edit_de_init_string },
  { 14, 14, show_baud_rate,             edit_baud_rate },
  { 15, 15, show_io_address,            edit_io_address },
  { 16, 16, show_digi_loopup_address,   edit_digi_loopup_address },
  { 17, 17, show_int_num,               edit_int_num },
  { 18, 18, show_port_num,              edit_port_num },
  { 19, 19, show_os2_com_name,          edit_os2_com_name },
  { 20, 20, show_verify_node,           edit_verify_node },
  { 21, 21, show_restrict_level,        edit_restrict_level },
  {  0,  0, NULL,                       NULL }
};

struct choose_list
{
  char *item;
  unsigned int token;
};

struct choose_list yes_or_no[] =
{
  { "No", 0 },
  { "Yes", 1 },
};

struct choose_list no_or_yes[] =
{
  { "Yes", 0 },
  { "No", 1 },
};

struct choose_list boards[] =
{
  { "Console", 0 },
  { "COM1-4", 1 },
  { "Dumb Digibrd", 2 },
  { "Intl Digibrd", 3 },
  { "StarGate", 4 },
  { "Lightspeed", 5 },
  { "OS/2 COM", 6 }
};

#define BAUD_RATES 6

struct choose_list baud_rates[] =
{
  { "300", 300 },
  { "1200", 1200 },
  { "2400", 2400 },
  { "9600", 9600 },
  { "19200", 19200 },
  { "38400", 38400 }
};

#define INTERRUPTS 5

struct choose_list interrupts[] =
{
  { "2", 2 },
  { "3", 3 },
  { "4", 4 },
  { "5", 5 },
  { "7", 7 }
};

struct choose_list dial_in_line[] =
{
  { "Local Line",   0 },
  { "Dial In Line", 1 }
};

struct choose_list fixed_dte_speed[] =
{
  { "Floating DTE Speed", 0 },
  { "Fixed DTE Speed", 1 }
};

struct choose_list rts_cts_list[] =
{
  { "No flow control", 0 },
  { "RTS/CTS flow control", 1 }
};

struct choose_list auto_verify[] =
{
  { "No Automatic Verification", 0 },
  { "Caller ID Automatic Verification", 1 }
};

char gtalk_path[200];

#define MAX_PORTS 50

struct serial_config_info
{
	char node;                     /* OS/2 */
	char active;                   /* OS/2 */
	char is_dial_in;               /* OS/2 */
	char init_string[60];          /* OS/2 */
	char de_init_string[60];       /* OS/2 */
	unsigned short int baud;             /* OS/2 */
	char board_type;               /* OS/2 (6 for OS/2) */
	unsigned short int io_address;
	unsigned short int digi_lookup_address;
	char int_num;
	char port_number;
	char fixed_dte_speed;          /* OS/2 */
	char rts_cts;                  /* OS/2 */
	char os2_com_name[15];    /* filename of com port (no colon) */
							  /* com1-8 digi0-xxx */
	char verify_node;         /* 0=no auto verification, 1=caller id auto */
							  /* verif */
	unsigned char restrict_level;  /* like priority, 0=sysop,50=regular, */
								   /* 0-255 */
	char dummy[32];

}  port[MAX_PORTS];

void show_bar()
{
  int c;
  for (c=0;c<80;c++) putch('Ä');
}

void blank_line(int l)
{
  gotoxy(1,l);
  clreol();
}

void show_string(char *string)
{
  while (*string) putch(*string++);
}

void show_title(void)
{
  clrscr();
  highvideo();
  gotoxy(1,1);
  show_string("                  G-Talk Serial Configuration Program");
  normvideo();
  gotoxy(1,2);
  show_string("             (C) 1994  Daniel L. Marks and David W. Jeske");
}

void show_string_len(char *string, int len)
{
  while ((len > 0) && (*string))
  {
    putch(*string++);
    len--;
  }
}

void show_string_padded(char *string, int len)
{
  while ((len > 0) && (*string))
  {
    putch(*string++);
    len--;
  }
  while (len > 0)
  {
    putch(' ');
    len--;
  }
}

void show_string_padded_x_y(char *string, int len, int x, int y)
{
  gotoxy(x+1,y+1);
  while ((len > 0) && (*string))
  {
    while ((len > 0) && (*string) && (x < 80))
    {
      putch(*string);
      string++;
      len--;
      x++;
    }
    if (x >= 80)
    {
      x = 0;
      y++;
      gotoxy(x+1,y+1);
    }
  }
  while (len > 0)
  {
    while ((len > 0) && (x < 80))
    {
      putch(' ');
      len--;
      x++;
    }
    x = 0;
    y++;
    gotoxy(x+1,y+1);
  }
}

void mod_gotoxy(int x, int y)
{
  y += (x / 80);
  x = x % 80;
  gotoxy(x+1,y+1);
}

void get_string(char *string, int len, int y, int x)
{
  int cloc = len;
  char ch;

  highvideo();
  show_string_padded_x_y(string, len, x, y);
  cloc = strlen(string);
  while (kbhit()) getch();
  for (;;)
  {
    mod_gotoxy(x+cloc,y);
    if (!(ch = getch()))
      if (getch() == 75) ch = 8;

    if (ch == 13) break;
    switch (ch)
    {
      case   0:  break;
      case   8:
      case 127:  if (cloc > 0)
                 {
                   cloc--;
                   mod_gotoxy(x+cloc,y);
                   putch(' ');
                 }
                 break;
      default:   if ((cloc < len) && (ch >= ' ') && (ch < 127))
                 {
                   string[cloc] = ch;
                   cloc++;
                   putch(ch);
                 }
                 break;
    }
  }
  string[cloc] = '\0';
  normvideo();
  show_string_padded_x_y(string, len, x, y);
}

void show_choice(struct choose_list choices[],
                 int num, int which, int y, int x, int hi)
{
  int count;
  int choice = -1;

  for (count=0;((choice == -1) && (count<num));count++)
    if (choices[count].token == which) choice = count;
  if (choice == -1) choice = 0;
  gotoxy(x+1,y+1);
  for (count=0;count<num;count++)
  {
    if (count) putch(' ');
    if (count == choice)
    {
      if (!hi) highvideo();
       else
      {
        textcolor(BLACK);
        textbackground(LIGHTGRAY);
      }
    }
    show_string_len(choices[count].item,80);
    normvideo();
  }
}

int select_choice(struct choose_list choices[],
                  int num, int which, int y, int x)
{
  char ch;
  int choice = -1;
  int initl;
  int extended;
  int count;

  for (count=0;((choice == -1) && (count<num));count++)
    if (choices[count].token == which) choice = count;
  if (choice == -1) choice = 0;
  initl = choice;
  while (kbhit()) getch();
  for (;;)
  {
    show_choice(choices, num, choices[choice].token, y, x, 1);
    ch = getch();
    if (!ch) extended = getch();
      else extended = 0;

    if ((extended == 75) || (ch == 8))
         choice = (!choice) ? (num - 1) : (choice - 1);
    if ((extended == 77) || (ch == ' '))
         choice = (choice >= (num - 1)) ? 0 : (choice + 1);
    if (ch == 13) break;
    if (ch == 27)
    {
      choice = initl;
      break;
    }
  }
  show_choice(choices, num, choices[choice].token, y, x, 0);
  return (choices[choice].token);
}

void show_hex_number(int number, int digits)
{
  char format[10];
  char string[20];

  sprintf(format,"%%-%dX",digits);
  sprintf(string,format,number);
  show_string_padded(string,digits);
}

void show_dec_number(int number, int digits)
{
  char format[10];
  char string[20];

  sprintf(format,"%%-%du",digits);
  sprintf(string,format,number);
  show_string_padded(string,digits);
}

int conv_hex(char *string, int *hex_no)
{
  *hex_no = 0;
  while (*string)
  {
    if ((*string >= 'A') && (*string <= 'F'))
       *hex_no = (*hex_no << 4) | (*string - 'A' + 10);
    else if ((*string >= 'a') && (*string <= 'f'))
       *hex_no = (*hex_no << 4) | (*string - 'a' + 10);
    else if ((*string >= '0') && (*string <= '9'))
       *hex_no = (*hex_no << 4) | (*string - '0');
    else return 0;
    string++;
  }
  return 1;
}

int conv_dec(char *string, int *dec_no)
{
  *dec_no = 0;
  while (*string)
  {
    if ((*string >= '0') && (*string <= '9'))
       *dec_no = (*dec_no * 10) + (*string - '0');
    else return 0;
    string++;
  }
  return 1;
}

int get_hex_number(int old_number, int digits, int y, int x)
{
  char format[10];
  char string[20];
  int hex_number;

  sprintf(string,"%X",old_number);
  get_string(string, digits, y, x);
  if (!conv_hex(string, &hex_number)) hex_number = old_number;
  gotoxy(x+1,y+1);
  sprintf(string,"%X",hex_number);
  show_string_padded(string,digits);
  return (hex_number);
}

int get_dec_number(int old_number, int digits, int y, int x)
{
  char format[10];
  char string[20];
  int dec_number;

  sprintf(string,"%u",old_number);
  get_string(string, digits, y, x);
  if (!conv_dec(string, &dec_number)) dec_number = old_number;
  gotoxy(x+1,y+1);
  sprintf(string,"%u",dec_number);
  show_string_padded(string,digits);
  return (dec_number);
}

int do_the_loading(char *filename)
{
  FILE *fileptr;
  char at_eof=0;
  int num=0;

  if (!(fileptr=fopen(filename,"rb"))) return 0;
  fseek(fileptr,0,SEEK_SET);
  while (!at_eof)
  {
    if (!(fread(&port[num], sizeof(struct serial_config_info), 1, fileptr)))
       at_eof=1;
     else
       num++;
  }
  fclose(fileptr);
  num_ports_loaded=num;
  return 1;
}

int do_the_saving(char *filename)
{
  FILE *fileptr;
  int num=0;

  if (!(fileptr=fopen(filename,"wb+"))) return 0;
  fseek(fileptr,0,SEEK_SET);

  while (num<num_ports_loaded)
  {
     if (!(fwrite(&port[num], sizeof(struct serial_config_info), 1, fileptr)))
     {
       fclose(fileptr);
       return 0;
     } else
       num++;
  }
  fclose(fileptr);
  is_saved=1;
  return 1;
}

void make_fake_port(struct serial_config_info *port, int portnum)
{
 port->node=1;
 port->is_dial_in=1;
 strcpy(port->init_string,"ATS0=1");
 strcpy(port->de_init_string,"ATS0=0");
 port->baud=2400;
 port->port_number=0;
 port->board_type=1;
 port->verify_node = 0;
 port->active = 1;
 port->restrict_level = 255;
 *port->os2_com_name = '\0';
 switch (portnum)
 {
   case 1:  port->io_address = 0x3f8;
			port->int_num = 4;
			break;
   case 2:  port->io_address = 0x2f8;
			port->int_num = 3;
			break;
   case 3:  port->io_address = 0x3e8;
			port->int_num = 4;
			break;
   case 4:  port->io_address = 0x2e8;
			port->int_num = 3;
			break;
   default: port->board_type = 0;
			break;
 }
}

void load_config_file()
{
  if (!(*gtalk_path))
  {
	show_title();
	gotoxy(1,8);
	show_string("Pathname of G-talk configuration file:");
	get_string(gtalk_path, 160, 8, 0);
  }
  if (*gtalk_path)
	if (gtalk_path[strlen(gtalk_path)-1] != '\\')
	  strcat(gtalk_path,"\\");
  strcat(gtalk_path,"SERIAL.CFG");
  if (!do_the_loading(gtalk_path))
  {
	gotoxy(1,13);
	show_string("Could not load file: ");
	gotoxy(1,14);
	show_string(gtalk_path);
	gotoxy(1,16);
	show_string("Create a new file? ");
	if (select_choice(no_or_yes, 2, 0, 15, 20))
	{
	  gotoxy(1,22);
	  show_string("Exiting to DOS");
	  exit(1);
	}
	num_ports_loaded = 0;
  } else is_saved = 0;
}

void show_board_type(struct serial_config_info *port)
{
  gotoxy(1,6);
  show_string("Board Type: ");
  normvideo();
  show_choice(boards, NUM_BOARDS, port->board_type, 6, 3, 0);
}

void show_dial_in(struct serial_config_info *port)
{
  gotoxy(1,9);
  show_string("Dial in line: ");
  normvideo();
  show_choice(dial_in_line, 2, port->is_dial_in, 8, 19, 0);
}

void show_fixed_dte_speed(struct serial_config_info *port)
{
  gotoxy(1,10);
  show_string("Fixed DTE Speed: ");
  normvideo();
  show_choice(fixed_dte_speed, 2, port->fixed_dte_speed, 9, 19, 0);
}

void show_rts_cts(struct serial_config_info *port)
{
  gotoxy(1, 11);
  show_string("Flow control: ");
  normvideo();
  show_choice(rts_cts_list, 2, port->rts_cts, 10, 19, 0);
}

void show_init_string(struct serial_config_info *port)
{
  gotoxy(1, 12);
  show_string("Init string: ");
  normvideo();
  gotoxy(20, 12);
  show_string(port->init_string);
}

void show_de_init_string(struct serial_config_info *port)
{
  gotoxy(1, 13);
  show_string("De Init String: ");
  normvideo();
  gotoxy(20, 13);
  show_string(port->de_init_string);
}

void show_baud_rate(struct serial_config_info *port)
{
  gotoxy(1,14);
  show_string("Baud Rate:       ");
  normvideo();
  show_choice(baud_rates, BAUD_RATES, port->baud, 13, 19, 0);
}

void show_io_address(struct serial_config_info *port)
{
  gotoxy(1,15);
  show_string("I/O Address:     ");
  normvideo();
  gotoxy(20,15);
  show_hex_number(port->io_address,4);
}

void show_digi_loopup_address(struct serial_config_info *port)
{
  gotoxy(1,16);
  show_string("Digi Lookup Addr:");
  normvideo();
  gotoxy(20,16);
  show_hex_number(port->digi_lookup_address,4);
}

void show_int_num(struct serial_config_info *port)
{
  gotoxy(1,17);
  show_string("Interrupt number:");
  normvideo();
  show_choice(interrupts, INTERRUPTS, port->int_num, 16, 19, 0);
}

void show_port_num(struct serial_config_info *port)
{
  gotoxy(1,18);
  show_string("Port on board:");
  normvideo();
  gotoxy(20,18);
  show_dec_number(port->port_number,2);
}

void show_os2_com_name(struct serial_config_info *port)
{
  gotoxy(1, 19);
  show_string("OS/2 Com Name: ");
  normvideo();
  gotoxy(20, 19);
  show_string(port->os2_com_name);
}

void show_verify_node(struct serial_config_info *port)
{
  gotoxy(1,20);
  show_string("Verify Node: ");
  normvideo();
  show_choice(auto_verify, 2, port->verify_node, 19, 19, 0);
}

void show_restrict_level(struct serial_config_info *port)
{
  gotoxy(1,21);
  show_string("Restrict Level:");
  normvideo();
  gotoxy(20,21);
  show_dec_number(port->restrict_level, 3);
}

void edit_board_type(struct serial_config_info *port)
{
  port->board_type =
	select_choice(boards, NUM_BOARDS, port->board_type, 6, 3);
}

void edit_dial_in(struct serial_config_info *port)
{
  port->is_dial_in =
	select_choice(dial_in_line, 2, port->is_dial_in, 8, 19);
}

void edit_fixed_dte_speed(struct serial_config_info *port)
{
  port->fixed_dte_speed =
	select_choice(fixed_dte_speed, 2, port->fixed_dte_speed, 9, 19);
}

void edit_rts_cts(struct serial_config_info *port)
{
  port->rts_cts =
	select_choice(rts_cts_list, 2, port->rts_cts, 10, 19);
}

void edit_init_string(struct serial_config_info *port)
{
  get_string(port->init_string, 59, 11, 19);
}

void edit_de_init_string(struct serial_config_info *port)
{
  get_string(port->de_init_string, 59, 12, 19);
}

void edit_baud_rate(struct serial_config_info *port)
{
  port->baud =
	select_choice(baud_rates, BAUD_RATES, port->baud, 13, 19);
}

void edit_io_address(struct serial_config_info *port)
{
   port->io_address =
	 get_hex_number(port->io_address, 4, 14, 19);
}

void edit_digi_loopup_address(struct serial_config_info *port)
{
   port->digi_lookup_address =
	 get_hex_number(port->digi_lookup_address, 4, 15, 19);
}

void edit_int_num(struct serial_config_info *port)
{
  port->int_num =
	select_choice(interrupts, INTERRUPTS, port->int_num, 16, 19);
}

void edit_port_num(struct serial_config_info *port)
{
   port->port_number =
	 get_dec_number(port->port_number, 2, 17, 19);
}

void edit_os2_com_name(struct serial_config_info *port)
{
  get_string(port->os2_com_name, 14, 18, 19);
}

void edit_verify_node(struct serial_config_info *port)
{
  port->verify_node =
	select_choice(auto_verify, 2, port->verify_node, 19, 19);
}

void edit_restrict_level(struct serial_config_info *port)
{
  port->restrict_level =
	get_dec_number(port->restrict_level,3,20,19);
}

void display_record(int current, int total, int field)
{
  int row;
  int cl;
  struct serial_config_info *cport;
  struct disp_edit_item *item = items;

  gotoxy(1,4);

  if (!total)
  {
	printf("No records           ");
	for (row=6;row<20;row++) blank_line(row);
	gotoxy(1,6);
	show_string("No records");
	return;
  }

  printf("Port number %d/%d  ",current+1,total);
  cport = &port[current];

  row = 0;
  while (item->display)
  {
	if (!fields_avail[cport->board_type][row])
	{
	  for (cl=item->first_row;cl<=item->last_row;cl++)
		blank_line(cl);
	} else
	{
	  if (field == row) highvideo();
	  (item->display)(cport);
	  normvideo();
	}
	row++;
	item++;
  }
}

void edit_config_file(void)
{
  int current = 0;
  int field = 0;
  char ch, key, extended;
  int count;
  struct serial_config_info *cport;

  show_title();
  gotoxy(1,5);
  show_bar();
  gotoxy(30,4);
  highvideo();
  show_string("Q-Quit S-Save I-Insert D-Delete A-Add");
  normvideo();
  for (;;)
  {
    display_record(current, num_ports_loaded, field);
    gotoxy(1,4);
    ch = getch();
    if (!ch) extended = getch();
      else extended = 0;
    if ((ch >= 'a') && (ch <= 'z')) ch -= ' ';
    if (num_ports_loaded)
    {
      cport = &port[current];
      if ((extended == 75) || (extended == 77) || (ch == 32))
      {
         if (extended == 75)
           current = (!current) ? (num_ports_loaded - 1) : (current - 1);
           else
           current = (current >= (num_ports_loaded - 1)) ? 0 : (current + 1);
         cport = &port[current];
         while (!fields_avail[cport->board_type][field])
           field = (!field) ? (NUM_FIELDS-1) : (field - 1);
      }
      if (extended == 72)
      {
       do
        {
          field = (!field) ? (NUM_FIELDS-1) : (field - 1);
        } while (!fields_avail[cport->board_type][field]);
      }
      if (extended == 80)
      {
       do
        {
          field = (field >= (NUM_FIELDS-1)) ? 0 : (field + 1);
		} while (!fields_avail[cport->board_type][field]);
      }
      if (ch == 13)
      {
        is_saved = 0;
        (items[field].edit)(cport);
      }
      if (ch == 'D')
      {
        gotoxy(1,24);
        show_string("Are you sure you want to delete this record? (Y/N) ");
        do
        {
          key = getch();
        } while ((key != 'Y') && (key != 'N') && (key != 'y') && (key != 'n'));
        blank_line(24);
        if ((key == 'Y') || (key == 'y'))
        {
          for (count=current;count<num_ports_loaded;count++)
            port[count] = port[count+1];
          num_ports_loaded--;
          if (current >= num_ports_loaded)
            current = num_ports_loaded ? num_ports_loaded - 1 : 0;
        }
      }
    }
    if (ch == 'Q')
    {
      if (!is_saved)
      {
        gotoxy(1,24);
        show_string("Are you sure you want to quit without saving? (Y/N) ");
        do
        {
          key = getch();
        } while ((key != 'Y') && (key != 'N') && (key != 'y') && (key != 'n'));
        blank_line(24);
        if ((key == 'Y') || (key == 'y')) break;
      } else break;
    }
    if (ch == 'S')
    {
      if (!do_the_saving(gtalk_path))
      {
        gotoxy(1,24);
		show_string("Could not save the file! Press RETURN");
        //sound(2000);
		// delay(300);
		// nosound();
		while (getch() != 13);
		blank_line(24);
	  } else
	  {
		gotoxy(1,24);
		show_string("Saved serial configuration! Press RETURN");
		//sound(2000);
		//delay(300);
		//nosound();
        while (getch() != 13);
        blank_line(24);
        is_saved = 1;
      }
    }
    if ((ch == 'I') && (num_ports_loaded < MAX_PORTS))
    {
      if (num_ports_loaded)
        for (count=num_ports_loaded;count>current;count--)
          port[count] = port[count-1];
        make_fake_port(&port[current],current+1);
      num_ports_loaded++;
      is_saved = 0;
    }
    if ((ch == 'A') && (num_ports_loaded < MAX_PORTS))
    {
      is_saved = 0;
      make_fake_port(&port[num_ports_loaded], num_ports_loaded+1);
      current = num_ports_loaded;
      num_ports_loaded++;
      is_saved = 0;
    }
  }
}

void main(int argc, char *argv[])
{
  if (argc >= 2) strcpy(gtalk_path, argv[1]);
    else *gtalk_path = '\0';
  load_config_file();
  edit_config_file();
  clrscr();
  printf("End program");
}

