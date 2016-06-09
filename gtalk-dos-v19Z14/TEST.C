#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <dir.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include "jumptbl.h"
#include "glmdef.h"

#define FILENAME_LEN 160
#define COMMAND_LEN 100
#define COPY_SPEED 4
#define RENAME_SPEED 6
#define COPY_SIZE 16384

#define MAX_FILENAMES 8

#define COMMAND_TOK_LEN 10

#define MAX_DRIVES 8

void pathcat_n(char *str1, char **str2, int length);

typedef struct tempfileptr
{
  char current_drive;
  char path[FILENAME_LEN+20];
  char command[COMMAND_LEN+1];
  char new_path[FILENAME_LEN+20];
  char filename[FILENAME_LEN+20];
  char filename2[FILENAME_LEN+20];
  char filename3[FILENAME_LEN+20];
  char filename4[FILENAME_LEN+20];
} tfile;

void change_directory(char *data, int id, tfile *files);
void list_files(char *data, int id, tfile *files);
void help_cmds(char *data, int id, tfile *files);
void erase_files(char *data, int id, tfile *files);
void show_file(char *data, int id, tfile *files);
void make_dir(char *data, int id, tfile *files);
void remove_dir(char *data, int id, tfile *files);
void edit_file(char *data, int id, tfile *files);
void copy_files(char *data, int id, tfile *files);
void send_xmodem(char *data, int id, tfile *files);
void receive_xmodem(char *data, int id, tfile *files);
void send_ymodem(char *data, int id, tfile *files);
void receive_ymodem(char *data, int id, tfile *files);
void rename_move_files(char *data, int id, tfile *files);

struct command_list
{
  char command[COMMAND_TOK_LEN+1];
  void (near *command_func)(char *data, int id, tfile *files);
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
  { "EDIT",    edit_file,          15 },
  { "COPY",    copy_files,         16 },
  { "MOVE",    rename_move_files,  17 },
  { "SX",      send_xmodem,        18 },
  { "RX",      receive_xmodem,     19 },
  { "SB",      send_ymodem,        20 },
  { "RB",      receive_ymodem,     21 },
  { "*",       NULL,                0 }
};

struct startblock ourblock =
{
  LD_STARTID,                   /* startup string */
  0,                            /* glm version number */
  sizeof(struct startblock),    /* size of header */
  "SYSOP",                      /* name of glm */
  CAN_BE_SHARED,                /* shared or non-shared */
  0                             /* dummy location to */
                                /* fill in length */
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
  struct command_list *entry = cmd_list;
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
      pathcat_n(buffer,&pathname,FILENAME_LEN);
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
    if (buffer != pathname) pathcat_n(buffer,&pathname,FILENAME_LEN);
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
      while ((*filename) && (n<FILENAME_LEN))
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
  if ((slashatend) && (n>0) && (n<FILENAME_LEN))
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
  struct ffblk ourblock;

  *files->filename = 0;
  pathcat_n(files->filename,&data,FILENAME_LEN);
  if ((files->filename[1] == ':') && (*files->filename))
  {
    if ((done=letter_to_drive(*files->filename)) == -1)
    {
      print_str_cr("Invalid Drive Letter");
      return;
    }
    if (g_getcurdir(done,files->filename3))
    {
      print_str_cr("Invalid Drive Letter");
      return;
    }
    if (*files->filename3)
      sprintf(files->new_path,"%c:\\%s\\",*files->filename,
         files->filename3);
      else
      sprintf(files->new_path,"%c:\\",*files->filename);
    compose_pathname_with_path(files->filename2,
         &files->filename[2], files->new_path,1,0);
  } else
    compose_pathname_with_path(files->filename2,
       files->filename,files->path,1,0);

  /*print_str_cr(files->filename2);*/

  if (strlen(files->filename2) < 4)
  {
    strcpy(files->path,files->filename2);
    return;
  }

  strcpy(files->filename3,files->filename2);
  strcat(files->filename3,".");

  done = g_findfirst(files->filename3,&ourblock,FA_DIREC);
  if (!done)
  {
    if (ourblock.ff_attrib & FA_DIREC)
    {
      strcpy(files->path,files->filename2);
      return;
    }
  }
  print_str_cr("Cannot change directory to path");
}

void list_files(char *data, int id, tfile *files)
{
  struct ffblk ourblock;
  int done;
  int nfiles = 2;
  int totalfiles = 0;
  unsigned long int totallength = 0;
  unsigned long int totalbytes;
  int pm, tm;
  char s[80];

  *files->filename = 0;
  pathcat_n(files->filename,&data,FILENAME_LEN);
  if (*files->filename)
    compose_pathname(files->new_path,files->filename,0);
    else
    compose_pathname(files->new_path,"*.*",0);

  print_cr();
  print_string("Directory of ");
  print_str_cr(files->new_path);
  print_cr();

  done = g_findfirst(files->new_path,&ourblock,~FA_LABEL);
  while (!done)
  {
    if (get_nchar(tswitch) == 27) break;
    tm = ((ourblock.ff_ftime >> 11) & 0x1F);
    pm = (tm > 11);
    tm -= (pm ? 12 : 0);
    sprintf(s,"%-13s%-6s%-10ld%02d-%02d-%02d %02d:%02d:%02d%c",ourblock.ff_name,
         ((ourblock.ff_attrib & FA_DIREC) ? "<DIR>" : ""),
         ourblock.ff_fsize,
         (ourblock.ff_fdate >> 5) & 0x0F,
         (ourblock.ff_fdate & 0x1F),
         (((ourblock.ff_fdate >> 9) & 0x3F) + 80) % 100,
         tm,
         (ourblock.ff_ftime >> 5) & 0x3F,
         (ourblock.ff_ftime & 0x1F) << 1,
         pm ? 'p' : 'a');
    print_str_cr(s);
    nfiles++;
    totalfiles++;
    totallength += ourblock.ff_fsize;
    done = g_findnext(&ourblock);
    if (nfiles == 22)
    {
      print_string("Press any key");
      if (wait_ch() == 27)
      {
        print_cr();
        break;
      }
      for (nfiles=0;nfiles<13;nfiles++) print_chr(8);
      nfiles=0;
    }
  }
  sprintf(s,"Total Files %d, Total Bytes %lu",totalfiles,totallength);
  print_str_cr(s);
  if (!g_getfree(letter_to_drive(*files->new_path),&totallength,&totalbytes))
  {
    sprintf(s,"Bytes free %lu, Total bytes on disk %lu",totallength,totalbytes);
    print_str_cr(s);
  }
  print_cr();
}

void erase_files(char *data, int id, tfile *files)
{
  struct ffblk ourblock;
  int done;
  int notquit = 1;
  int key;

  while (notquit)
  {
    *files->filename = 0;
    pathcat_n(files->filename,&data,FILENAME_LEN);
    if (!(*files->filename)) break;
    compose_pathname(files->new_path,files->filename,0);
    done = 0;
    if (strstr(files->new_path,"*.*"))
    {
      print_string("Erase ");
      print_string(files->new_path);
      print_string(" ? (y/n) ");
      do
      {
        key = wait_ch();
        if (key > 'Z') key -= ' ';
      } while ((key != 'Y') && (key != 'N'));
      print_chr(key);
      print_cr();
      if (key == 'N') done = 1;
    }
    if (!done)
       done = g_findfirst(files->new_path,&ourblock,(~FA_LABEL) & (~FA_DIREC));
    while ((!done) && (notquit))
    {
      if (get_nchar(tswitch) == 27)
      {
        notquit = 0;
        break;
      }
      compose_pathname(files->new_path,files->filename,1);
      compose_pathname_with_path(files->new_path,"..\\",files->new_path,1,1);
      compose_pathname_with_path(files->new_path,ourblock.ff_name,files->new_path,0,1);
      print_string("Erasing file ");
      print_str_cr(files->new_path);
      if (g_remove(files->new_path))
      {
        print_str_cr("Could not remove file");
        done = 1;
      } else done = g_findnext(&ourblock);
    }
  }
  if (!notquit) print_str_cr("Aborted");
}

void show_file(char *data, int id, tfile *files)
{
  *files->filename = 0;
  pathcat_n(files->filename,&data,FILENAME_LEN);
  if (!(*files->filename))
  {
    print_str_cr("Filename required");
    return;
  }
  compose_pathname(files->new_path,files->filename,0);

  print_file_to_cntrl(files->new_path,tswitch,(id == 8),
     1,1,(id == 10));
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
    if ((**str2 >= 'a') && (**str2 <= 'z')) *str1++ = (**str2 - ' ');
     else
    *str1++ = **str2;
    (*str2)++;
    n++;
  }
  *str1 = 0;
}



void edit_file(char *data, int id, tfile *files)
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

  if (g_mkdir(files->new_path)) print_str_cr("Make directory failed");
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

  if (g_rmdir(files->new_path)) print_str_cr("Remove directory failed");
}

int copy_file_function(char *file1, char *file2)
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
      bufsize = g_fread(buffer, 1, COPY_SIZE, infile);
      g_fwrite(buffer, 1, bufsize, outfile);
      unlock_dos();
      g_delay(COPY_SPEED);
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

  done = g_findfirst(files->filename2,&ourblock,FA_DIREC);
  if (!done)
  {
    if (ourblock.ff_attrib & FA_DIREC)
    {
      direc = 1;
      strcat(files->filename2,"\\");
    }
  }

  done = g_findfirst(files->filename,&ourblock,(~FA_LABEL) & (~FA_DIREC));
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
      if (kode=copy_file_function(files->filename3,files->filename4))
      {
        print_str_cr(kode == 2 ? "Copy failed" : "Copy aborted");
        done = 1;
      }
        else done = g_findnext(&ourblock);
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

  done = g_findfirst(files->filename2,&ourblock,FA_DIREC);
  if (!done)
  {
    if (ourblock.ff_attrib & FA_DIREC)
    {
      direc = 1;
      strcat(files->filename2,"\\");
    }
  }

  done = g_findfirst(files->filename,&ourblock,(~FA_LABEL) & (~FA_DIREC));
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
      g_delay(RENAME_SPEED);
      if (g_rename(files->filename3,files->filename4))
      {
        print_str_cr("Move failed");
        done = 1;
      }
        else done = g_findnext(&ourblock);
    }
  }

}

void send_xmodem(char *data, int id, tfile *files)
{
  char *temp = files->filename3;

  *files->filename = 0;
  pathcat_n(files->filename,&data,FILENAME_LEN);
  compose_pathname(files->filename3,files->filename,0);

  send_files(&temp,1,1);
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
  do
  {
    *files->filename = 0;
    pathcat_n(files->filename,&data,FILENAME_LEN);
    compose_pathname(files->new_path,files->filename,0);
    if (*files->new_path)
    {
      strcpy(filepointers[num_names],files->new_path);
      num_names++;
    }
  } while ((num_names<MAX_FILENAMES) && (*files->new_path));
  if (num_names)
    send_files(filepointers,num_names,3);
  g_free(filebuffer);
}

void receive_ymodem(char *data, int id, tfile *files)
{
  char *filebuffer = g_malloc((FILENAME_LEN+1)*MAX_FILENAMES,"SNDYNMS");
  char *filepointers[MAX_FILENAMES];
  int num_names;

  if (!filebuffer)
  {
    print_str_cr("Could not allocate memory for filenames");
    return;
  }
  for (num_names=0;num_names<MAX_FILENAMES;num_names++)
    filepointers[num_names] = &filebuffer[num_names*(FILENAME_LEN+1)];

  recv_files(filepointers,&num_names,3);
}

void receive_xmodem(char *data, int id, tfile *files)
{
  int num;
  char *temp = files->filename3;

  *files->filename = 0;
  pathcat_n(files->filename,&data,FILENAME_LEN);
  compose_pathname(files->filename3,files->filename,0);

  recv_files(&temp,&num,1);
}

int far ginsu_main(void)
{
  struct command_list *entry;
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

  files->current_drive = g_getdisk();

  g_getcwd(files->path,FILENAME_LEN-1);
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

