#include "include.h"
#include "gtalk.h"


#define INCL_DOS
#define INCL_DOSMEMMGR
#define INCL_DOSERRORS
#define INCL_DOSSEMAPHORES
#include <os2.h>

// leave this much for DOS
#define DAVE_FUDGE_FACTOR 32768l
// #define DAVE_FUDGE_FACTOR 0

#define ALLOC_SEM_WAIT (200L)

#undef FILE_DEBUG
#undef MEM_DEBUG

/* Memory and file allocation unit */

int delete_file_entry(int entry_num);

mem_entry  mem_array[MAX_MEM_HANDLES];
file_entry file_array[MAX_FILE_HANDLES];

int 		  mem_handles = 0;
int 		  file_handles = 0;

char  *fileblacklist[] =
{
  "AUX",  "NUL",  "PRN",  "LPT",
  "LPT1", "LPT2", "LPT3", "LPT4",
  "COM",  "COM1", "COM2", "COM3",
  "COM4", "CLOCK$", "CON", NULL
};

HMTX alloc_sem;
int alloc_sem_owner=-1;

void grab_all_available_memory(int use_ems);

void init_alloc(void)
{
  DosCreateMutexSem(NULL,&alloc_sem,0,0);
  grab_all_available_memory(0);

}

int find_memory_pointer(void *memory_pointer)
{
#ifdef DEBUG
	char s[80];
#endif
	mem_entry *cur_entry = mem_array;
	int count = 0;
	int flag = 0;

	while ((count<mem_handles) && (!flag))
	{
	 if (cur_entry->memory_pointer == memory_pointer) flag = 1;
	 if (!flag)
	 {
		count++;
		cur_entry++;
	 }
	}
#ifdef DEBUG
    sprintf(s,"Find Memory Pointer %p entry %d flag %d",memory_pointer,count,flag);
    print_str_cr_to(s,0);
#endif
	if (flag) return (count);
	 else return (-1);
};

int insert_memory_entry_at(int insert_at, void *memory_pointer, int task_id,
					  unsigned int size, char keep_open,
					  const char *description, int empty)
{
#ifdef DEBUG
	char s[80];
#endif
	int count;
	mem_entry *cur_entry;

	if (mem_handles==MAX_MEM_HANDLES) return (-1);

	for (count=mem_handles;count>insert_at;count--)
	 mem_array[count] = mem_array[count-1];

	cur_entry = &mem_array[insert_at];

	cur_entry->memory_pointer = memory_pointer;
	cur_entry->task_id = task_id;
	cur_entry->bytes = size;
	cur_entry->kept_open = keep_open;
	cur_entry->empty = empty;
	strncpy(cur_entry->allocby,description,DESCRIPTION_LENGTH-1);
	cur_entry->allocby[DESCRIPTION_LENGTH-1] = 0;
#ifdef DEBUG
    sprintf(s,"Add memory entry %p entry %c",memory_pointer,mem_handles);
    print_str_cr_to(s,0);
#endif
	mem_handles++;
	return (insert_at);
};

int delete_memory_entry(int entry_num)
{
#ifdef DEBUG
	char s[80];
#endif
	int count;

	if (entry_num >= mem_handles) return (0);

	for (count=entry_num;count<(mem_handles-1);count++)
	 mem_array[count] = mem_array[count+1];
#ifdef DEBUG
    sprintf(s,"Deleting entry %d",entry_num);
    print_str_cr_to(s,0);
#endif
	mem_handles--;
	return (1);
};

int find_smallest_available_memory(unsigned long int size, int who)
{
  int smallest = -1;
  unsigned long int smallest_size = 0x7FFFFFFF;
  int current = 0;
  char is_smallest;
  mem_entry *cur_entry = mem_array;

  while (current < mem_handles)
  {
	if (cur_entry->empty)
	{
	  if ((cur_entry->bytes>=size))
	  {
		smallest_size = cur_entry->bytes;
		smallest = current;
	  }
	}
	current++;
	cur_entry++;
  }
  return (smallest);
}

void merge_blocks(int entry)
{

#ifdef DEBUG
print_str_cr_to("Merge Blocks",0);
#endif

  if (entry>0)
  {
   if (mem_array[entry-1].empty)
   {
	 mem_array[entry].bytes += mem_array[entry-1].bytes;
	 mem_array[entry].memory_pointer = mem_array[entry-1].memory_pointer;
	 entry--;
	 delete_memory_entry(entry);
   }
  }

 if (entry<(mem_handles-1))
 {
   if (mem_array[entry+1].empty)
   {
	 mem_array[entry+1].bytes += mem_array[entry].bytes;
	 mem_array[entry+1].memory_pointer = mem_array[entry].memory_pointer;
	 delete_memory_entry(entry);
   }
 }
}


void grab_all_available_memory(int use_ems)
{
  unsigned long int precore = 400000;
  unsigned long int core_left = precore /* - DAVE_FUDGE_FACTOR */;
  unsigned long int bytes = (unsigned int) (core_left);
  void *total_memory_pointer;


  DosAllocMem(&total_memory_pointer,core_left,PAG_READ | PAG_WRITE | PAG_COMMIT);

  printf("Allocated memory starting at %p\n",total_memory_pointer);


  if (!total_memory_pointer)
  {
	printf("Could not allocate memory!\n");
	g_exit(1);
  }
  printf("CoreLeft Begin: %ld asking for %ld \n",precore,core_left);
  printf("We have [%ud] bytes\n",bytes);
  mem_handles = 0;
  insert_memory_entry_at(0,(char *)total_memory_pointer,
					  -1,bytes,1,"EMPTY",1);

  sys_toggles.total_starting_memory=core_left;


  use_ems=0;

#ifdef MEM_DEBUG
  g_exit(1);
#endif
}

void *allocate_some_memory(unsigned long int length,
 const char *description, int have_owner, int who)
{
  int available_block = find_smallest_available_memory(length,who);
  mem_entry *cur_entry;
  void *old_pointer;

  if (available_block==-1) return (NULL);
  if (mem_handles==MAX_MEM_HANDLES) return (NULL);
  cur_entry = &mem_array[available_block];
  old_pointer = cur_entry->memory_pointer;
  cur_entry->memory_pointer = (void *)
   (((char *) cur_entry->memory_pointer) + length);
  cur_entry->bytes -= length;
  if (!cur_entry->bytes) delete_memory_entry(available_block);
  insert_memory_entry_at(available_block,old_pointer,
   ((tasking && have_owner) ? tswitch : -1),
   length,!(tasking && have_owner),description,0);
  return(old_pointer);
}

int free_some_memory(void *pointer_to_free)
{
  int freed_memory_entry = find_memory_pointer(pointer_to_free);
  mem_entry *cur_entry;

  if (freed_memory_entry==-1) return (0);
  cur_entry = &mem_array[freed_memory_entry];

  if (cur_entry->empty) return (1);
  cur_entry->task_id = -1;
  cur_entry->empty = 1;
  cur_entry->kept_open = 0;
  strcpy(cur_entry->allocby,"EMPTY");

  merge_blocks(freed_memory_entry);
  return (1);
}


void *g_malloc_main_only(unsigned long int memory,
		const char *description)
{
#ifdef DEBUG
	char s[80];
#endif
	void *memory_pointer;

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);
    alloc_sem_owner = tswitch;

	memory_pointer = allocate_some_memory(memory,description,1,tswitch);
#ifdef DEBUG
	  sprintf(s,"Mallocing memory pointer %p",memory_pointer);
	  print_str_cr_to(s,0);
#endif
    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner = -1;
    return (memory_pointer);
};

void *g_malloc(unsigned long int memory, const char *description)
{
#ifdef DEBUG
	char s[80];
#endif
	void *memory_pointer;

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);
    alloc_sem_owner = tswitch;

	memory_pointer = allocate_some_memory(memory,description,1,tswitch);
#ifdef DEBUG
	  sprintf(s,"Mallocing memory pointer %p",memory_pointer);
	  print_str_cr_to(s,0);
#endif
    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner = -1;
	return (memory_pointer);
};

void *g_malloc_with_owner(unsigned long int memory, const char *description,
         int for_task, int use_ems, int have_owner)
{
#ifdef DEBUG
	char s[80];
#endif

	void *memory_pointer;

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);
    alloc_sem_owner = tswitch;

    memory_pointer = allocate_some_memory(memory,description,have_owner,for_task);
#ifdef DEBUG
	sprintf(s,"Mallocing memory pointer %p",memory_pointer);
#endif
    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner = -1;
	return (memory_pointer);
}

int g_free_from_who(void *memory_pointer, int who)
{
#ifdef DEBUG
	char s[80];
#endif

	int result;

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);
    alloc_sem_owner = tswitch;
	result=!free_some_memory(memory_pointer);

#ifdef DEBUG
	  sprintf(s,"Freeing pointer %p",memory_pointer);
#endif

    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner = -1;
	return (result);
};


int g_free(void *memory_pointer)
{
#ifdef DEBUG
	char s[80];
#endif

	int dos_not_locked;
	int result;

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);
    alloc_sem_owner = tswitch;
	result=!free_some_memory(memory_pointer);

#ifdef DEBUG
	  sprintf(s,"Freeing pointer %p",memory_pointer);
#endif

    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner = -1;
	return (result);
};

int g_transfer(void *memory_pointer,int new_task_id)
{
	int mem_point;
	int response = 0;

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);
    alloc_sem_owner = tswitch;
	mem_point = find_memory_pointer(memory_pointer);
	if (mem_point != -1)
	  {
		mem_array[mem_point].task_id = new_task_id;
		response = 1;
	  };

    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner = -1;
	return (response);
};

int g_keep_memory(void *memory_pointer,int keep_it)
{
	int mem_point;
	int response = 0;

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);
    alloc_sem_owner = tswitch;
	mem_point = find_memory_pointer(memory_pointer);
	if (mem_point != -1)
	 {
	   mem_array[mem_point].kept_open = keep_it;
	   response = 1;
	 };

    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner = -1;
	return (response);
};

#ifdef REALLOC_DEFINED
void *g_realloc(void *memory_pointer,size_t new_size)
{
	void *new_pointer;
	int mem_point;

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);
    alloc_sem_owner = tswitch;

	mem_point = find_memory_pointer(memory_pointer);
	if (mem_point == -1)
	 {
	   return NULL;
	 };
	new_pointer = realloc(memory_pointer,new_size);
	if (new_pointer)
	  {
		mem_array[mem_point].size = new_size;
		mem_array[mem_point].memory_pointer = new_pointer;
	  };

    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner = -1;

	return (new_pointer);
};

void *g_calloc(size_t num, size_t size, const char *description)
{
	void *memory_pointer;

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);
    alloc_sem_owner = tswitch;

	memory_pointer = calloc(num,size);
	if (memory_pointer)
	 add_memory_entry(memory_pointer,tswitch,(num*size),0,description);

    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner = -1;

    return (memory_pointer);
};
#endif

void g_free_all_handles(int task_id) /* Should only be called in tasker */
{
	mem_entry *cur_entry = &mem_array[mem_handles];
	file_entry *cur_file_entry = &file_array[file_handles];
	int current = mem_handles;

    if (alloc_sem_owner != task_id)
      DosRequestMutexSem(alloc_sem,100L);


	while (current > 0)
	 {
	   cur_entry--;
	   current--;
	   if ((cur_entry->task_id == task_id) && (!cur_entry->kept_open) &&
		   (!(cur_entry->empty)))
		{
		  free_some_memory(cur_entry->memory_pointer);
		};
	 };

	current=file_handles;
	while (current > 0)
	 {
	   current--;
	   cur_file_entry--;
	   if ((cur_file_entry->task_id == task_id) && (!cur_file_entry->kept_open))
		{
		  fclose(cur_file_entry->file_pointer);
		  delete_file_entry(current);
		};
	 };
    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner = -1;
};

int g_owns_memory(void *memory_pointer)
{
	int mem_point;

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);
    alloc_sem_owner = tswitch;
	mem_point = find_memory_pointer(memory_pointer);
    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner = -1;
	return (mem_point);
};

int find_file_pointer(FILE *file_pointer)
{
	file_entry *cur_entry = file_array;
	int count = 0;
	int flag = 0;

	while ((count<file_handles) && (!flag))
	 if (cur_entry->file_pointer == file_pointer)
	  flag = 1;
	  else
	  {
		count++;
		cur_entry++;
	  };
	if (flag) return (count);
	 else return (-1);
};

int add_file_entry(FILE *file_pointer,int task_id,
					  const char *filename, char keep_open,
					  const char *description)
{
	file_entry *cur_entry = &file_array[file_handles];

	if (file_handles==MAX_FILE_HANDLES) return (-1);

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);
    alloc_sem_owner=tswitch;

	cur_entry->file_pointer = file_pointer;
	cur_entry->task_id = task_id;
	strncpy(cur_entry->filename,filename,FILENAME_LENGTH-1);
	cur_entry->filename[FILENAME_LENGTH - 1] = 0;
	cur_entry->kept_open = keep_open;
	strncpy(cur_entry->allocby,description,DESCRIPTION_LENGTH-1);
	cur_entry->allocby[DESCRIPTION_LENGTH - 1] = 0;
	file_handles++;

    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner=-1;
	return (file_handles - 1);
};

int delete_file_entry(int entry_num)
{
	int count;

	if (entry_num >= file_handles) return (0);

	for (count=entry_num;count<(file_handles-1);count++)
	 file_array[count] = file_array[count+1];
	file_handles--;
	return (1);
};

int checkblacklist(const char *filename)
{
  int backlen, len = strlen(filename);
  char *endstr;
  char filebuf[10];
  char *t;
  char **cmp;

  backlen = len;
  endstr = (char *) &filename[len];
  while (backlen>0)
  {
	endstr--;
	if ((*endstr == ':') || (*endstr == '\\'))
	{
	  endstr++;
	  break;
	}
	backlen--;
  }
  if ((len-backlen) > 6) return 0;
  t = filebuf;
  do
  {
	*t++ = ((*endstr >= 'a') && (*endstr <= 'z')) ?
			 (*endstr - ' ') : *endstr;
  } while (*endstr++);
  cmp = fileblacklist;
  while (*cmp)
  {
	if (!strcmp(filebuf,*cmp)) return 1;
	cmp++;
  }
  return 0;
}

FILE *g_fopen(const char *filename, const char *mode, const char *description)
{
	void *file_pointer;

	if (checkblacklist(filename)) return NULL;

	file_pointer = fopen(filename,mode);
	if (file_pointer)
	 if (add_file_entry(file_pointer,tswitch,filename,0,description)==-1)
	  {
        fflush(file_pointer);
		fclose(file_pointer);
		file_pointer = NULL;
	  };
	return (file_pointer);
};

struct open_mode_table
{
  char *mode_string;
  int modes;
  int openflags;
} mode_table[] =
{
  { "rb+",  OPEN_ACCESS_READWRITE, OPEN_ACTION_OPEN_IF_EXISTS },
  { "wb+",  OPEN_ACCESS_READWRITE, OPEN_ACTION_REPLACE_IF_EXISTS | OPEN_ACTION_CREATE_IF_NEW },
  { "ab+",  OPEN_ACCESS_READWRITE, OPEN_ACTION_CREATE_IF_NEW },
  { "rb",   OPEN_ACCESS_READONLY,  OPEN_ACTION_OPEN_IF_EXISTS },
  { "wb",   OPEN_ACCESS_WRITEONLY, OPEN_ACTION_REPLACE_IF_EXISTS | OPEN_ACTION_CREATE_IF_NEW },
  { "ab",   OPEN_ACCESS_WRITEONLY, OPEN_ACTION_CREATE_IF_NEW },
  { "r+",   OPEN_ACCESS_READWRITE, OPEN_ACTION_OPEN_IF_EXISTS },
  { "w+",   OPEN_ACCESS_READWRITE, OPEN_ACTION_REPLACE_IF_EXISTS | OPEN_ACTION_CREATE_IF_NEW },
  { "a+",   OPEN_ACCESS_READWRITE, OPEN_ACTION_CREATE_IF_NEW },
  { "r",    OPEN_ACCESS_READONLY,  OPEN_ACTION_OPEN_IF_EXISTS },
  { "w",    OPEN_ACCESS_WRITEONLY, OPEN_ACTION_REPLACE_IF_EXISTS | OPEN_ACTION_CREATE_IF_NEW },
  { "a",    OPEN_ACCESS_WRITEONLY, OPEN_ACTION_CREATE_IF_NEW },
  { NULL, 0, 0 }
};

char spincursor[] = { '/', '|' , '\\' , '-' , (char)0 };

FILE *g_fopen_excl(const char *filename, const char *mode, const char *description, int excl,int *error)
{
	void *file_pointer;
	HFILE fd;
	APIRET ret;
    int found_mode=0;
    struct open_mode_table *entry = mode_table;
	int modes = OPEN_ACCESS_READWRITE;
    int openflags = OPEN_SHARE_DENYNONE;
    ULONG action;
    char *cur_spin = spincursor;
    HFILE handle=0;
    int dummy;
	USHORT rc;
	int waiting_temp=0;
	int time_init = time(NULL);

   if (!error)
     error = &dummy;

#ifdef FILE_DEBUG
    print_str_cr("g_fopen_excl()");
#endif

	if (checkblacklist(filename)) return NULL;

    while ((entry->mode_string) && (!found_mode))
	{
	  if (!strcmp(mode,entry->mode_string))
	  {
		modes = entry->modes;
		openflags = entry->openflags;
        found_mode=1;
		break;
	  }
	  entry++;
	}

     if (!found_mode)
     {
       log_error("* Did not find mode in g_fopen_excl()");
       *error = (NULL);
       return (NULL);
     }
	switch (excl)
	{
	  case PRIVATE_ACCESS: modes |= OPEN_SHARE_DENYREADWRITE;
						   break;
	  case PRIVATE_READ_ACCESS:  modes |= OPEN_SHARE_DENYREAD;
						   break;
	  case PRIVATE_WRITE_ACCESS: modes |= OPEN_SHARE_DENYWRITE;
						   break;
	  default: modes |= OPEN_SHARE_DENYNONE;
			   break;
	}

    while ((rc = DosOpen(filename,
                         &handle,
                         &action,
                         0,
                         FILE_NORMAL,
                         openflags,
                         modes,
                         NULL)) !=0)
	{
       if ((time(NULL)-time_init) > 4)
        {
        if (waiting_temp)
          print_cr();
#ifdef FILE_DEBUG
        print_str_cr("timeout");
#endif
         *error = (GFERR_FILE_BUSY);
         return (NULL);
        }

       if (rc == ERROR_FILE_NOT_FOUND)
       {
            if (waiting_temp)
              print_cr();
#ifdef FILE_DEBUG
        print_str_cr("File not Found");
#endif
           *error = (GFERR_FILE_NOT_FOUND);
           return (NULL);
       }

       if (rc == ERROR_PATH_NOT_FOUND)
       {
            if (waiting_temp)
              print_cr();
#ifdef FILE_DEBUG
        print_str_cr("File not Found");
#endif
           *error = (GFERR_PATH_NOT_FOUND);
           return (NULL);
       }

	   if (rc != ERROR_SHARING_VIOLATION)
       {
        char s[100];
        if (waiting_temp)
          print_cr();
        log_error("*g_fopen_excl(): Non-Sharing Violation Error");
#ifdef FILE_DEBUG
        print_str_cr("Unknown Error");
#endif
        sprintf(s,"*              + ERR#%04d - (%s)",rc,filename);
        log_error(s);

        *error =  (GFERR_UNKNOWN);
        return (NULL);
       }

	   DosSleep(500);
	   if (!waiting_temp)
         {
               print_string("Waiting...-");
               waiting_temp++;
         }
	   else
	   {
             print_chr(8);
             print_chr(*cur_spin);
             cur_spin++;
             if (!(*cur_spin))
               cur_spin = spincursor;
			 waiting_temp++;
	   }
	}
	if (waiting_temp)
	  print_cr();


	file_pointer = fdopen(handle,mode);
	if (file_pointer)
	{
	 if (add_file_entry(file_pointer,tswitch,filename,0,description)==-1)
	  {
        fflush(file_pointer);
        fclose(file_pointer);
		file_pointer = NULL;
        DosClose(handle);
        *error = 0;
        return (NULL);
	  }
	}
    else
    { *error = 0;
      DosClose(handle);
      return (NULL);
    }

    *error = 0;
	return (file_pointer);
};


int g_fclose(FILE *file_pointer)
{
	int file_point;
	int result=0;

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);
    alloc_sem_owner=tswitch;

	file_point = find_file_pointer(file_pointer);
	if (file_point != -1)
	 {
	  result=fclose(file_pointer);
	  delete_file_entry(file_point);
     }
     else
    {
     if (file_pointer)
       {
         char s[100];
         sprintf(s,"*g_fclose(): no entry for ptr:0x(%p) -closing",file_pointer);
         log_error(s);
         fclose(file_pointer);
       }
     else
      log_error("*g_fclose(): no entry for null ptr");
    }

    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner=-1;
    return (result);
};

int g_ftransfer(FILE *file_pointer, int new_task_id)
{

	int file_point;
	int response = 0;

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);
    alloc_sem_owner = tswitch;

	file_point = find_file_pointer(file_pointer);
	if (file_point != -1)
	 if (file_array[file_point].task_id == tswitch)
	  {
		file_array[file_point].task_id = new_task_id;
		response = 1;
	  };

    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner = -1;
	return (response);
};

int g_flush(FILE *file_pointer)
{
	int file_point;
	int result=0;

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);

	file_point = find_file_pointer(file_pointer);
	if (file_point != -1)
	 {
	  result=fflush(file_pointer);
	 };
    DosReleaseMutexSem(alloc_sem);

	return (result);
};

int g_owns_file(void *file_pointer)
{
	int file_point;

    DosRequestMutexSem(alloc_sem,ALLOC_SEM_WAIT);
    alloc_sem_owner = tswitch;

	file_point = find_file_pointer(file_pointer);
    DosReleaseMutexSem(alloc_sem);
    alloc_sem_owner = -1;

	return (file_point);
};

