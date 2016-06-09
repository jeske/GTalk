/********************************

              List.h

 ********************************/

#ifndef _GTALK_LIST_H
#define _GTALK_LIST_H

#define MAX_INDEXES 4

typedef int list_int;
typedef unsigned int ptr_int;

typedef int (*compare_type)(void *,void *);
typedef void *(*malloc_func)(int);
typedef void *(*realloc_func)(void *, int);
typedef void (*free_func)(void *);

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

#endif   /* _GTALK_LIST_H */
