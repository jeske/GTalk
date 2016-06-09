
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/vfs.h>

#define DOS_FILENAME_LEN 160
#define FILENAME_LEN 300

#define COMMAND_LEN 100
#define COPY_SPEED 4
#define RENAME_SPEED 6
#define COPY_SIZE 16384

#define MAX_FILENAMES 8

#define COMMAND_TOK_LEN 10

#define MAX_DRIVES 8

char *drive_array[] = 
{
  "/home",
  "/usr",
  "/bin",
  NULL
};

void pathcat_n(char *str1, char **str2, int length);

char *dos_to_unix_translate(char *tnum)
{
  char *c;
  int len = FILENAME_LEN;
  char drive;
  static char name[FILENAME_LEN+1];

  c = name;
  drive = *tnum;
  if (drive > 'Z')
    drive -= ' ';
  if (((drive >= 'C') && (drive <= 'Z')) && (tnum[1] == ':'))
    {
      char **drv = drive_array;

      tnum += 2;
      while ((*drv) && (drive > 'C'))
	{
	  drive--;
	  drv++;
	}
      if (!(*drv))
	return "";
      strcpy(name, *drv);
      strcat(name, "/");
      c += strlen(name);
      len -= strlen(name);
    }
  while ((len > 0) && (*tnum))
    {
      if (*tnum == '\\')
	*c++ = '/';
      else
	*c++ = *tnum;
      tnum++;
      len--;
    }
  *c = '\000';
  return (name);
}

int getfree(char *filename, unsigned long int *freebytes,
			unsigned long int *totalbytes)
{
  struct statfs st;

  if (statfs(dos_to_unix_translate(filename), &st) < 0)
    {
      *freebytes = 0;
      *totalbytes = 0;
      return (-1);
    }
  *freebytes = st.f_bfree * 1024;
  *totalbytes = st.f_blocks * 1024;
  return (0);
}

typedef struct tempfileptr
{
  char current_drive;
  char path[DOS_FILENAME_LEN+20];
  char command[COMMAND_LEN+1];
  char new_path[DOS_FILENAME_LEN+20];
  char filename[DOS_FILENAME_LEN+20];
  char filename2[DOS_FILENAME_LEN+20];
  char filename3[DOS_FILENAME_LEN+20];
  char filename4[DOS_FILENAME_LEN+20];
} tfile;

void change_directory(char *data, int id, tfile *files);
void list_files(char *data, int id, tfile *files);
void help_cmds(char *data, int id, tfile *files);
void erase_files(char *data, int id, tfile *files);
void show_file(char *data, int id, tfile *files);
void make_dir(char *data, int id, tfile *files);
void remove_dir(char *data, int id, tfile *files);
void dos_edit_file(char *data, int id, tfile *files);
void copy_files(char *data, int id, tfile *files);
void send_xmodem(char *data, int id, tfile *files);
void receive_xmodem(char *data, int id, tfile *files);
void send_ymodem(char *data, int id, tfile *files);
void receive_ymodem(char *data, int id, tfile *files);
void rename_move_files(char *data, int id, tfile *files);

struct dos_command_list
{
  char command[COMMAND_TOK_LEN+1];
  void (*command_func)(char *data, int id, tfile *files);
  int id;
} cmd_list[] =
{
  { "CD",      change_directory,    0 },
  { "LS",      list_files,          1 },
  { "DIR",     list_files,          2 },
  { "HELP",    help_cmds,           3 },
  { "?",       help_cmds,           4 },
  { "ERASE",   erase_files,         5 },
  { "DEL",     erase_files,         6 },
  { "EXIT",    NULL,                7 },
  { "QUIT",    NULL,                7 },
  { "ANSI",    show_file,           8 },
  { "TYPE",    show_file,           9 },
  { "MORE",    show_file,          10 },
  { "RD",      remove_dir,         11 },
  { "RMDIR",   remove_dir,         12 },
  { "MD",      make_dir,           13 },
  { "MKDIR",   make_dir,           14 },
  { "EDIT",    dos_edit_file,          15 },
  { "COPY",    copy_files,         16 },
  { "MOVE",    rename_move_files,  17 },
  { "SX",      send_xmodem,        18 },
  { "RX",      receive_xmodem,     19 },
  { "SB",      send_ymodem,        20 },
  { "RB",      receive_ymodem,     21 },
  { "*",       NULL,                0 }
};

int asctonum(char *t)
{
  int temp = 0;
  while (*t)
  {
    if ((*t>='0') && (*t<='9')) temp = (temp * 10) + (*t- '0');
     else break;
    t++;
  }
  return(temp);
}

void help_cmds(char *data, int id, tfile *files)
{
  struct dos_command_list *entry = cmd_list;
  int wrap = 0;

  print_str_cr("List of commands:");
  while (*entry->command != '*')
  {
    print_string(entry->command);
    entry++;
    if (*entry->command != '*')
    {
      print_string(", ");
      if ((++wrap) == 8)
      {
        wrap = 0;
        print_cr();
      }
    }
  }
  print_cr();
}

#define compose_pathname(buffer,filename,slashatend) \
        compose_pathname_with_path(buffer,filename,files->path,slashatend,0)

void compose_pathname_with_path(char *buffer, char *filename,
				char *pathname, int slashatend, int justadd)
{
  int n = 0;

  if (!justadd)
  {
    *buffer = 0;
    if ((*filename != '\\') &&
        ((!*filename) || (filename[1] != ':')))
    {
      pathcat_n(buffer,&pathname,DOS_FILENAME_LEN);
      n = strlen(buffer);
      buffer += n;
    } else
    {
      if ((*filename == '\\') &&
          (*pathname) && (pathname[1] == ':'))
      {
        sprintf(buffer,"%c:",*pathname);
        n = 2;
        buffer += 2;
      }
    }
  } else
  {
    if (buffer != pathname) 
      pathcat_n(buffer,&pathname,DOS_FILENAME_LEN);
    n = strlen(buffer);
    buffer += n;
  }
  while (*filename)
  {
    if (*filename == '.')
    {
      filename++;
      if (*filename == '.')
      {
        while (n>0)
        {
          if (*(buffer-1) == ':') break;
          buffer--;
          n--;
          if (n>0)
           if (*(buffer-1) == '\\') break;
        }
      }
    } else
    {
      while ((*filename) && (n<DOS_FILENAME_LEN))
      {
        *buffer++ = *filename;
        n++;
        if (*filename == '\\') break;
        filename++;
      }
    }
    while (*filename)
    {
      if (*filename == '\\')
      {
        filename++;
        break;
      }
      filename++;
    }
  }
  if ((slashatend) && (n>0) && (n<DOS_FILENAME_LEN))
    if (*(buffer-1) != '\\') *buffer++ = '\\';
  *buffer = 0;
/*  {
    char s[80];
    sprintf(s,"n: %d\n",n);
    print_str_cr(s);
  } */
}

int letter_to_drive(int letter)
{
  if ((letter >= 'A') && (letter <= 'Z')) return (letter-'@');
  if ((letter >= 'a') && (letter <= 'z')) return (letter-'`');
  return (-1);
}

void change_directory(char *data, int id, tfile *files)
{
  int done;
  struct stat statbuf;

  *files->filename = 0;
  pathcat_n(files->filename,&data,DOS_FILENAME_LEN);
  if ((files->filename[1] == ':') && (*files->filename))
  {
    sprintf(files->new_path,"%c:\\",*files->filename);
      compose_pathname_with_path(files->filename2,
         &files->filename[2], files->new_path,1,0);
  } else
    compose_pathname_with_path(files->filename2,
       files->filename,files->path,1,0);

  if (strlen(files->filename2) < 4)
  {
    strcpy(files->path,files->filename2);
    return;
  }

  strcpy(files->filename3,files->filename2);
  strcat(files->filename3,".");

  done = stat(dos_to_unix_translate(files->filename3),&statbuf);
  if (!done)
  {
    if (S_ISDIR(statbuf.st_mode))
    {
      strcpy(files->path,files->filename2);
      return;
    }
  }
  print_str_cr("Cannot change directory to path");
}

void pathcat_n(char *str1, char **str2, int length)
{
  int n = 0;
  while (*str1)
  {
    str1++;
    n++;
  }
  while (**str2 == ' ') (*str2)++;
  while ((**str2) && (n<length) && (**str2 != ' '))
  {
    if (**str2 == '/') *str1++ = '\\';
     else
    *str1++ = **str2;
    (*str2)++;
    n++;
  }
  *str1 = 0;
}



void dos_edit_file(char *data, int id, tfile *files)
{
  char s[6];
  unsigned int length;

  *files->filename = 0;
  pathcat_n(files->filename,&data,FILENAME_LEN);
  if (!(*files->filename))
  {
    print_str_cr("File name required");
    return;
  }
  compose_pathname(files->new_path,files->filename,0);

  print_string("Edit buffer length: ");
  get_string_cntrl(s,5,0,0,0,0,0,0,1);

  if (*s)
  {
    length = asctonum(s);
    {
      char t[80];
      sprintf(t,"%s %d",s,length);
      print_str_cr(t);
    }
    if (length<100)
    {
      print_str_cr("Bad size buffer");
    } else line_editor(files->new_path,length);
  }
}

void make_dir(char *data, int id, tfile *files)
{
  *files->filename = 0;
  pathcat_n(files->filename,&data,FILENAME_LEN);
  if (!(*files->filename))
  {
    print_str_cr("Directory name required");
    return;
  }
  compose_pathname(files->new_path,files->filename,0);

  if (mkdir(files->new_path)) print_str_cr("Make directory failed");
}

void remove_dir(char *data, int id, tfile *files)
{
  *files->filename = 0;
  pathcat_n(files->filename,&data,FILENAME_LEN);
  if (!(*files->filename))
  {
    print_str_cr("Filename required");
    return;
  }
  compose_pathname(files->new_path,files->filename,0);

  if (rmdir(files->new_path)) print_str_cr("Remove directory failed");
}

int dos_copy_file_function(char *file1, char *file2)
{
   int bufsize = 1;
   FILE *infile;
   FILE *outfile;
   char *buffer;
   int loop;
   int kode = 0;

   if (!(buffer = g_malloc(COPY_SIZE,"COPYBUF")))
          return 1;
   if ((infile=g_fopen(file1,"rb+","SYSOP#2"))==NULL)
    {
      g_free(buffer);
      log_error(file1);
      return 1;
    };
   if ((outfile=g_fopen(file2,"wb","SYSOP#1"))==NULL)
    {
      g_free(buffer);
      g_fclose(infile);
      log_error(file2);
      return 1;
    };
   while (bufsize)
    {
      if (get_nchar(tswitch) == 27)
      {
        kode = 2;
        break;
      }
      lock_dos(234);
	  bufsize = fread(buffer, 1, COPY_SIZE, infile);
	  fwrite(buffer, 1, bufsize, outfile);
      unlock_dos();
	  delay(COPY_SPEED);
	};
   g_fclose(infile);
   g_fclose(outfile);
   g_free(buffer);
   return (kode);
}

void copy_files(char *data, int id, tfile *files)
{
  int done;
  int direc = 0;
  int kode;
  struct ffblk ourblock;

  *files->filename = 0;
  pathcat_n(files->filename,&data,FILENAME_LEN);
  compose_pathname(files->new_path,files->filename,0);
  strcpy(files->filename,files->new_path);
  *files->filename2 = 0;
  pathcat_n(files->filename2,&data,FILENAME_LEN);
  compose_pathname(files->new_path,files->filename2,0);
  strcpy(files->filename2,files->new_path);

  done = findfirst(files->filename2,&ourblock,FA_DIREC);
  if (!done)
  {
    if (ourblock.ff_attrib & FA_DIREC)
    {
      direc = 1;
      strcat(files->filename2,"\\");
    }
  }

  done = findfirst(files->filename,&ourblock,0);
  while (!done)
  {
    if (get_nchar(tswitch) == 27)
    {
      done = 1;
      print_str_cr("Aborted");
    }
    else
    {
      *files->filename3 = 0;
      *files->filename4 = 0;
      compose_pathname_with_path(files->filename3,"\\..\\",files->filename,0,1);
      strcat(files->filename3,ourblock.ff_name);
      if (direc)
      {
        compose_pathname_with_path(files->filename4,"\\..\\",files->filename2,0,1);
        strcat(files->filename4,ourblock.ff_name);
      } else strcpy(files->filename4,files->filename2);
      print_string("Copying ");
      print_string(files->filename3);
      print_string(" to ");
      print_str_cr(files->filename4);
	  if (kode=dos_copy_file_function(files->filename3,files->filename4))
      {
        print_str_cr(kode == 2 ? "Copy failed" : "Copy aborted");
        done = 1;
      }
        else done = findnext(&ourblock);
    }
  }
}

void rename_move_files(char *data, int id, tfile *files)
{

  int done;
  int direc = 0;
  struct ffblk ourblock;

  *files->filename = 0;
  pathcat_n(files->filename,&data,FILENAME_LEN);
  compose_pathname(files->new_path,files->filename,0);
  strcpy(files->filename,files->new_path);
  *files->filename2 = 0;
  pathcat_n(files->filename2,&data,FILENAME_LEN);
  compose_pathname(files->new_path,files->filename2,0);
  strcpy(files->filename2,files->new_path);

  done = findfirst(files->filename2,&ourblock,FA_DIREC);
  if (!done)
  {
    if (ourblock.ff_attrib & FA_DIREC)
    {
      direc = 1;
      strcat(files->filename2,"\\");
    }
  }

  done = findfirst(files->filename,&ourblock,0);
  while (!done)
  {
    if (get_nchar(tswitch) == 27)
    {
      done = 1;
      print_str_cr("Aborted");
    }
    else
    {
      *files->filename3 = 0;
      *files->filename4 = 0;
      compose_pathname_with_path(files->filename3,"\\..\\",files->filename,0,1);
      strcat(files->filename3,ourblock.ff_name);
      if (direc)
      {
        compose_pathname_with_path(files->filename4,"\\..\\",files->filename2,0,1);
        strcat(files->filename4,ourblock.ff_name);
      } else strcpy(files->filename4,files->filename2);
      print_string("Moving ");
      print_string(files->filename3);
      print_string(" to ");
      print_str_cr(files->filename4);
	  delay(RENAME_SPEED);
      if (rename(files->filename3,files->filename4))
      {
        print_str_cr("Move failed");
        done = 1;
      }
        else done = findnext(&ourblock);
    }
  }

}

void send_xmodem(char *data, int id, tfile *files)
{
  char *temp = files->filename3;

  *files->filename = 0;
  pathcat_n(files->filename,&data,FILENAME_LEN);
  if (*files->filename)
  {
    compose_pathname(files->filename3,files->filename,0);
    print_string("Sending file XMODEM ");
    print_str_cr(temp);
    send_files(&temp,1,1);
  } else
  {
    print_str_cr("Must specify filename to send!");
  }
}

void send_ymodem(char *data, int id, tfile *files)
{
  char *filebuffer = g_malloc((FILENAME_LEN+20)*MAX_FILENAMES,"SNDYNMS");
  char *filepointers[MAX_FILENAMES];
  int num_names;

  if (!filebuffer)
  {
    print_str_cr("Could not allocate memory for filenames");
    return;
  }
  for (num_names=0;num_names<MAX_FILENAMES;num_names++)
    filepointers[num_names] = &filebuffer[num_names*(FILENAME_LEN+1)];
  num_names = 0;
  print_str_cr("Sending files YMODEM BATCH:");
  do
  {
    *files->filename = 0;
    pathcat_n(files->filename,&data,FILENAME_LEN);
    if (*files->filename)
    {
      compose_pathname(files->new_path,files->filename,0);
      print_str_cr(files->new_path);
      if (*files->new_path)
      {
        strcpy(filepointers[num_names],files->new_path);
       num_names++;
      }
    }
  } while ((num_names<MAX_FILENAMES) && (*files->filename));
  print_str_cr("Sending files NOW...");
  if (num_names)
    send_files(filepointers,num_names,3);
  g_free(filebuffer);
}

void receive_ymodem(char *data, int id, tfile *files)
{
  char *filebuffer = g_malloc((FILENAME_LEN+1)*MAX_FILENAMES,"RCVYNMS");
  char *filepointers[MAX_FILENAMES];
  int num_names;

  if (!filebuffer)
  {
    print_str_cr("Could not allocate memory for filenames");
    return;
  }
  for (num_names=0;num_names<MAX_FILENAMES;num_names++)
    filepointers[num_names] = &filebuffer[num_names*(FILENAME_LEN+1)];

   *files->filename = 0;
   pathcat_n(files->filename,&data,FILENAME_LEN);
   print_string("Receiving YMODEM BATCH to ");
   if (*files->filename)
   {
     compose_pathname(files->new_path,files->filename,0);
     print_str_cr(files->new_path);
	 receive_files(files->new_path,filepointers,&num_names,3);
   } else
   {
     print_str_cr(files->path);
	 receive_files(files->path,filepointers,&num_names,3);
   }
}

void receive_xmodem(char *data, int id, tfile *files)
{
  int num;
  char *temp = files->filename3;

  *files->filename = 0;
  pathcat_n(files->filename,&data,FILENAME_LEN);
  if (*files->filename)
  {
	compose_pathname(files->filename3,files->filename,0);
	print_string("Receiving file XMODEM ");
	print_str_cr(temp);
	receive_files(NULL,&temp,&num,1);
  } else
  {
    print_str_cr("Must specify filename to receive to!");
  }
}

int dos_entry(void)
{
  struct dos_command_list *entry;
  char *next;
  int found;
  int flag = 1;
  char cmd_copy[COMMAND_TOK_LEN+1];
  tfile *files = g_malloc(sizeof(tfile),"SYSOP");

  if (!files)
  {
    print_cr();
    print_str_cr("SYSOP GLM: Could not allocate memory for buffers");
    print_cr();
  }

  strcpy(files->path,"");
  strcpy(files->command,"*");

  files->current_drive = getdisk();

  getcwd(files->path,FILENAME_LEN-1);
  strcat(files->path,"\\");

  print_cr();
  print_cr();
  special_code(1,tswitch);
  print_str_cr("|*h1|*f2Entering G-DOS version 1.0|*r1");
  special_code(0,tswitch);
  print_cr();

  while (flag)
  {
    check_for_privates();
    print_cr();
    special_code(1,tswitch);
	foreground(1,tswitch);
    print_string(files->path);
    foreground(7,tswitch);
    special_code(0,tswitch);
    print_string(" > ");
    get_string_cntrl(files->command,COMMAND_LEN,0,0,0,0,0,0,0);
    *cmd_copy = 0;
    next = files->command;
    entry = cmd_list;
    pathcat_n(cmd_copy,&next,COMMAND_TOK_LEN);
    if (*cmd_copy)
    {
      if ((cmd_copy[1] == ':') && (!cmd_copy[2]))
        change_directory(cmd_copy, 0, files);
      else
      {
        found = 0;
        while ((*entry->command != '*') && (!found))
        {
          if (!strcmp(cmd_copy,entry->command))
          {
            found = 1;
            if (entry->id != 7)
              (entry->command_func)(next,entry->id,files);
              else
              flag = 0;
          }
          entry++;
        }
        if (!found)
        {
          print_string("Cannot find command: ");
          print_str_cr(cmd_copy);
        }
      }
    }
  }
  print_cr();
  print_str_cr("--> Exiting shell");
  g_free(files);
  return (1);
}

