/********************************

              List.h
   List Class by Daniel Marks
   (C) Copyright 1995 by Daniel Marks,
   all Rights Reserved
 ********************************/

#ifndef _LIST_H
#define _LIST_H

#define MAX_INDEXES 10

typedef int list_int;
typedef unsigned int ptr_int;

typedef int (*compare_type)(void *,void *);
typedef void *(*malloc_func)(int);
typedef void *(*realloc_func)(void *, int);
typedef void (*free_func)(void *);

#define s_offset(str, el) (((int) &(((str *)NULL)->el)))
#define s_length(str, el) ((sizeof(((str *)NULL)->el)))
#define elements(x) ((x)->cur_no)
#define indexes(x) ((x)->num_indexes)
#define index_loc(list,el,ind) &((((list)->index[(ind)]).index_list)[(el)])
#define real_index_no(list,el,ind) ((((list)->index[(ind)]).index_list)[(el)])
#define element_of(type,list,el) ((type *) (((list)->data) + \
      (((list)->struct_length) * (el))))
#define element_of_index(type,list,el,ind) ((type *) (((list)->data) + \
     (((((list)->index[(ind)]).index_list)[(el)]) * ((list)->struct_length))))

typedef struct _list_index
{
  compare_type    c_func;
  list_int       *index_list;
} list_index;

typedef struct _list
{
  char                 *data;
  list_int              struct_length;
  list_int              alloc_no;
  list_int              cur_no;
  int                   num_indexes;
  struct _list_index    index[MAX_INDEXES];
  int                   fixed_len;
  int                   list_fd;

  malloc_func     malloc_f;
  realloc_func    realloc_f;
  free_func       free_f;

  malloc_func     malloc_f_ind;
  realloc_func    realloc_f_ind;
  free_func       free_f_ind;
} list;

int new_static_list(list *clist, list_int struct_length, void *data, int recs);
int new_list(list *clist, list_int struct_length);
int new_file_list(list *clist, list_int struct_length, char *filename,
		  int create);
void list_qsort(list *clist, compare_type cf, 
		list_int *begin, list_int *end);
void sort_index(list *clist, int indno);
void re_sort(list *clist);
int index_with_func(list *clist, compare_type func);
int search_list_for(list *clist, int ind_no, void *find, int *exact);
int search_list(list *clist, int ind_no, void *find);
int find_exact_in_index(list *clist, int ind_no, void *find);
int find_first(list *clist, int ind_no, void *find);
int find_first_after(list *clist, int ind_no, void *find);
int add_index(list *clist, compare_type func);
int delete_index(list *clist, int ind_no);
int add_list(list *clist, void *data);
void write_list(list *clist);
int free_list(list *clist);
int delete_list(list *clist, int indx);

#define DEFAULT_NAME_LEN 30
#define DEFAULT_DATA_LEN 80

typedef struct _default_entry
{
  char default_name[DEFAULT_NAME_LEN+1];
  char default_data[DEFAULT_DATA_LEN+1];
} default_entry;

int read_defaults_file(char *filename, list *defaults);
char *get_default(char *default_name, list *defaults);

#endif   /* _LIST_H */










