/* ALLOC.H */

#ifndef GT_ALLOC_H
#define GT_ALLOC_H


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

/* File handle and memory allocation stuff */

#define MAX_MEM_HANDLES 100
#define MAX_FILE_HANDLES 40
#define DESCRIPTION_LENGTH 10
#define FILENAME_LENGTH 25

#define MAIN_MEMORY 0
#define EMS_OR_MAIN 1
#define EMS_MEMORY 2

#define PUBLIC_ACCESS 0
#define PRIVATE_READ_ACCESS 1
#define PRIVATE_WRITE_ACCESS 2
#define PRIVATE_ACCESS 3


#define GFERR_FILE_NOT_FOUND -100
#define GFERR_FILE_BUSY      -101
#define GFERR_PATH_NOT_FOUND -102
#define GFERR_UNKNOWN        -199

typedef struct mem_alloc
{
		void	*memory_pointer;
		int 	task_id;
		int 	empty;
		unsigned long int bytes;
		char	kept_open;
		char	allocby[DESCRIPTION_LENGTH];
} mem_entry;

typedef struct file_alloc
{
		FILE	*file_pointer;
		int 	task_id;
		unsigned int size;
		char	filename[FILENAME_LENGTH];
		char	kept_open;
		char	allocby[DESCRIPTION_LENGTH];
		char	total_access;
} file_entry;

void init_alloc(void);
void deallocate_ems(void);
void *g_malloc_main_only(unsigned long int memory, const char *description);
void *g_malloc(unsigned long int memory, const char *description);
int g_keep_memory(void *memory_pointer,int keep_it);
void *g_malloc_with_owner(unsigned long int memory, const char *description,
	   int for_who, int use_ems, int owner);
int g_free_from_who(void *memory_pointer, int who);
int g_free(void *memory_pointer);
int g_transfer(void *memory_pointer,int new_task_id);
#ifdef REALLOC_DEFINED
void *g_realloc(void *memory_pointer,size_t new_size);
void *g_calloc(size_t num, size_t size, const char *description);
#endif
int g_owns_memory(void *memory_pointer);
void g_free_all_handles(int task_id);

FILE *g_fopen(const char *filename, const char *mode, const char *description);
FILE *g_fopen_excl(const char *filename, const char *mode, const char *description, int excl,int *error);
int g_fclose(FILE *file_ptr);
int g_ftransfer(FILE *file_ptr, int new_task_id);
int g_flush(FILE *file_ptr);
int g_owns_file(void *file_ptr);

extern mem_entry mem_array[MAX_MEM_HANDLES];
extern file_entry file_array[MAX_FILE_HANDLES];
extern mem_handles;
extern file_handles;


#endif /* GT_ALLOC_H */

