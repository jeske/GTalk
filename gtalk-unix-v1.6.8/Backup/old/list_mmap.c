/********************************

              List.c

 ********************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "list.h"

typedef struct _myst
{
  char name[10];
  int number;
} myst;

void *dummy_malloc(int blah)
{
  return (NULL);
}

void *dummy_realloc(void *blah, int blah2)
{
  return (NULL);
}

void dummy_free(void *blah)
{
  return;
}

int byname(void *l1, void *l2)
{
  return (strcmp(((myst *)l1)->name, ((myst *)l2)->name));
}

int bynumber(void *l1, void *l2)
{
  return ((((myst *)l1)->number - ((myst *)l2)->number));
}

void *list_ind_realloc(list *clist, void *ptr, int size)
{
  if (ptr)
    return ((clist->realloc_f_ind)(ptr, size));
  else
    return ((clist->malloc_f_ind)(size));
}

void *list_ch_realloc(list *clist, void *ptr, int size)
{
  if (ptr)
    return ((clist->realloc_f)(ptr, size));
  else
    return ((clist->malloc_f)(size));
}

int realloc_list(list *clist, int temp_no)
{
  void *temp_buf;
  int index_no;

  if (!(temp_buf=list_ch_realloc(clist,clist->data,
				 temp_no * clist->struct_length)))
    return (0);
  clist->data = temp_buf;
  for (index_no=0;index_no<clist->num_indexes;index_no++)
    {
      if (!(temp_buf=list_ind_realloc(clist,clist->index[index_no].index_list,
	     temp_no * sizeof(list_int))))
	return (0);
      clist->index[index_no].index_list = temp_buf;
    }
  return (1);
}

int add_list_check(list *clist, int curno)
{
  int temp_no;
  
  if (curno > clist->alloc_no)
    {
      if (clist->fixed_len)
	return (0);
      temp_no = clist->alloc_no;
      if (!temp_no)
	temp_no++;
      do
	{
	  temp_no <<= 1;
	} while (temp_no < clist->cur_no);
      if (realloc_list(clist, temp_no))
	{
	  clist->alloc_no = temp_no;
	  return (1);
	}
      return (0);
    }
  return (1);
}

int del_list_check(list *clist, int curno)
{
  int temp_no = (clist->alloc_no << 2);

  if (curno < temp_no)
    {
      if (clist->fixed_len)
	return (1);
      clist->alloc_no = temp_no;
      realloc_list(clist, temp_no);
    }
  return (1);
}

int new_list(list *clist, list_int struct_length)
{
  clist->data = NULL;
  clist->num_indexes = clist->cur_no = clist->alloc_no = 0;
  clist->struct_length = struct_length;
  clist->malloc_f_ind = clist->malloc_f = malloc;
  clist->realloc_f_ind = clist->realloc_f = realloc;
  clist->free_f_ind = clist->free_f = free;
  clist->fixed_len = 0;
  clist->list_fd = -1;
  clist->list_type = NORMAL_TYPE;
  return (1);
}

int new_static_list(list *clist, list_int struct_length, void *data, int recs)
{
  clist->data = data;
  clist->num_indexes = 0;
  clist->cur_no = clist->alloc_no = recs;
  clist->struct_length = struct_length;
  clist->malloc_f = dummy_malloc;
  clist->malloc_f_ind = malloc;
  clist->realloc_f = dummy_realloc;
  clist->realloc_f_ind = realloc;
  clist->free_f = dummy_free;
  clist->free_f_ind = free;
  clist->fixed_len = 1;
  clist->list_fd = -1;
  clist->list_type = NORMAL_TYPE;
  return (1);
}

int new_static_file_list(list *clist, list_int struct_length,
			 char *filename, int recs)
{
  int fd = open(filename, O_RDWR | O_CREAT, 0666);
  long int offset;
  void *data;

  if (fd < 0)
    return (0);
  offset = lseek(fd, SEEK_END, 0);
  clist->alloc_no = clist->cur_no = offset / struct_length;
  if (recs > 0)
    {
      long int write_bytes;
      char buffer[512];

      memset(buffer, 0, 512);
      write_bytes = (recs * struct_length) - offset;
      while (write_bytes > 0)
	{
	  write(fd, buffer, (write_bytes > 512) ? 512 : write_bytes);
	  write_bytes -= ((write_bytes > 512) ? 512 : write_bytes);
	}
      clist->alloc_no = recs;
    }
  clist->data = mmap((caddr_t) NULL, clist->alloc_no * struct_length,
		     PROT_READ|PROT_WRITE, MAP_FILE|MAP_PRIVATE,
		     fd, 0);
  if (!clist->data)
    {
      clist->alloc_no = clist->cur_no = 0;
      close(fd);
      return (0);
    }
  clist->num_indexes = 0;
  clist->struct_length = struct_length;
  clist->malloc_f = dummy_malloc;
  clist->malloc_f_ind = malloc;
  clist->realloc_f = dummy_realloc;
  clist->realloc_f_ind = realloc;
  clist->free_f = dummy_free;
  clist->free_f_ind = free;
  clist->fixed_len = 1;
  clist->list_fd = fd;
  clist->list_type = MMAP_TYPE;
  return (1);
}
  
void list_qsort(list *clist, compare_type cf, 
		list_int *begin, list_int *end)
{
  register list_int *fi;
  register list_int *li;
  void *pivot;
  list_int temp;
 
  fi = begin;
  li = end;

  pivot = element_of(void,clist,
     begin[(((((ptr_int)end)-((ptr_int)begin))/sizeof(list_int)) >> 1)]);
  
  for (;;)
    {
      while (fi<end)
	{
	  if (((cf)(element_of(void,clist,*fi),pivot) < 0))
	    fi++;
	  else
	    break;
	}
      while (li>begin)
	{
	  if (((cf)(element_of(void,clist,*li),pivot) > 0))
	    li--;
	  else
	    break;
	}
      if (fi<=li)
	{
	  temp = *fi;
	  *fi = *li;
	  *li = temp;
	  fi++;
	  li--;
	} else
	  break;
    }
  if (li>begin) 
    list_qsort(clist, cf, begin, li);
  if (fi<end)
    list_qsort(clist, cf, fi, end);
}

void sort_index(list *clist, int indno)
{
  list_index *ind;

  if ((indno < clist->num_indexes) && (clist->cur_no))
    {
      ind = &clist->index[indno];
      list_qsort(clist, ind->c_func, ind->index_list,
		 &ind->index_list[clist->cur_no - 1]);
    }
}

void re_sort(list *clist)
{
  int i;

  for (i=0;i<clist->num_indexes;i++)
    sort_index(clist, i);
}

int index_with_func(list *clist, compare_type func)
{
  int id;

  for (id=0;id<clist->num_indexes;id++)
    {
      if (func == clist->index[id].c_func)
	return (id);
    }
  return (-1);
}

int search_list_for(list *clist, int ind_no, void *find, int *exact)
{
  register list_int first;
  register list_int last;
  register list_int mid;
  list_int last_mid;
  void *first_el;
  void *last_el;
  compare_type cf = clist->index[ind_no].c_func;
  int srch;

  if (!clist->cur_no)
    {
      *exact = 0;
      return (0);
    }
  first = 0;
  last = clist->cur_no;
  last_mid = -1;
  for (;;)
    {
      mid = (first + last) >> 1;
      if (last_mid == mid)
	{
	  *exact = 0;
	  return (mid);
	}
      last_mid = mid;
      srch=((cf)(find,element_of_index(void,clist,mid,ind_no)));
      if (!srch)
	{
	  *exact = 1;
	  return (mid);
	}
      else
	{
	  if (srch < 0)
	    last = mid;
	  else
	    first = mid;
	}
    }
}      

int find_exact_in_index(list *clist, int ind_no, void *find)
{
  int exact, srch;
  compare_type cf = clist->index[ind_no].c_func;
  int found = search_list_for(clist, ind_no, find, &exact);
  int found_low;

  if (!exact)
    return (-1);
  found_low = found;
  while (found_low > 0)
    {
      found_low--;
      if (((cf)(find,element_of_index(void,clist,found_low,ind_no))))
	break;
      if (!memcmp(find,element_of_index(void,clist,found_low,ind_no),
		  clist->struct_length))
	return (found_low);
    }
  while (found < clist->cur_no)
    {
      if (((cf)(find,element_of_index(void,clist,found,ind_no))))
	break;
      if (!memcmp(find,element_of_index(void,clist,found,ind_no),
		  clist->struct_length))
	return (found);
      found++;
    }
  return (-1);
}

int find_first(list *clist, int ind_no, void *find)
{
  int exact, srch;
  compare_type cf = clist->index[ind_no].c_func;
  int found = search_list_for(clist, ind_no, find, &exact);

  if (!exact)
    return (-1);
  while (found > 0)
    {
      if (((cf)(find,element_of_index(void,clist,found,ind_no))) <= 0)
	found--;
      else
	break;
    }
  while (found < clist->cur_no)
    {
      if (((cf)(find,element_of_index(void,clist,found,ind_no))) > 0)
	found++;
      else
	break;
    }
  return (found);
}

int find_first_after(list *clist, int ind_no, void *find)
{
  int exact;
  compare_type cf = clist->index[ind_no].c_func;
  int found = search_list_for(clist, ind_no, find, &exact);

  if ((found > 0) && (found < clist->cur_no))
    if (((cf)(find,element_of_index(void,clist,found,ind_no))) < 0)
      found--;
  while (found < clist->cur_no)
    {
      if (((cf)(find,element_of_index(void,clist,found,ind_no))) >= 0)
	found++;
      else
	break;
    }
  return (found);
}

int add_index(list *clist, compare_type func)
{
  int temp_no;
  list_int *temp;

  if (clist->num_indexes >= MAX_INDEXES)
    return (0);
  if (!(temp=(clist->malloc_f_ind)(clist->alloc_no * sizeof(list_int))))
    return (0);
  for (temp_no=0;temp_no<clist->cur_no;temp_no++)
    temp[temp_no] = temp_no;

  clist->index[clist->num_indexes].c_func = func;
  clist->index[clist->num_indexes++].index_list = temp;
  sort_index(clist, clist->num_indexes-1);
  return (1);
}  

int delete_index(list *clist, int ind_no)
{
  int i;

  if (ind_no >= clist->num_indexes)
    return (0);
  (clist->free_f_ind)(clist->index[ind_no].index_list);
  for (i=ind_no;i<clist->num_indexes;i++)
    clist->index[i] = clist->index[i+1];
  clist->num_indexes--;
}

int add_list(list *clist, void *data)
{
  int ind;
  int find;

  if (!add_list_check(clist, clist->cur_no+1))
    return (0);
  memcpy(element_of(void,clist,clist->cur_no), data, clist->struct_length);
  for (ind=0;ind<clist->num_indexes;ind++)
    {
      find = find_first_after(clist, ind, data);
      if (find < clist->cur_no)
	memmove(index_loc(clist,find+1,ind),
		index_loc(clist,find,ind),
		(clist->cur_no - find) * sizeof(list_int));
      *(index_loc(clist,find,ind)) = clist->cur_no;
    }
  clist->cur_no++;
  return (1);
}

int free_list(list *clist)
{
  int i;

  for (i=0;i<clist->num_indexes;i++)
    (clist->free_f_ind)(clist->index[i].index_list);
  clist->num_indexes = 0;
  if (clist->list_type == MMAP_TYPE)
    {
  /* munmap(clist->data, clist->alloc_no * clist->struct_length); */
      ftruncate(clist->list_fd, clist->cur_no * clist->struct_length);
    }
  (clist->free_f)(clist->data);
  clist->data = NULL;
  if (clist->list_fd >= 0)
    {
      close(clist->list_fd);
      clist->list_fd = -1;
    }
  clist->alloc_no = clist->cur_no = 0;
}

int delete_list(list *clist, int indx)
{
  int ind;
  void *erased_data;
  void *last_element;
  list_int repl = clist->cur_no - 1;

  if (indx >= clist->cur_no)
    return (0);
  last_element = element_of(void,clist,repl);
  erased_data = element_of(void,clist,indx);

  for (ind=0;ind<clist->num_indexes;ind++)
    {
      int del_index;
      int repl_index;

      if ((del_index = find_exact_in_index(clist, ind, erased_data)) >= 0)
	{
	  if ((repl_index =
	       find_exact_in_index(clist, ind, last_element)) >= 0)
	    {
	      *(index_loc(clist, repl_index, ind)) = indx;
	      if (del_index < clist->cur_no)
		memmove(index_loc(clist,del_index,ind),
			index_loc(clist,del_index+1,ind),
			(clist->cur_no - del_index - 1) * sizeof(list_int));
	    }
	}
    }
  memcpy(erased_data, last_element, clist->struct_length);
  del_list_check(clist, clist->cur_no - 1);
  clist->cur_no--;
  return (1);
}

void main(void)
{
  list mylist;
  int i, j, k;
  myst add;
  char s[100];
  FILE *fp;

/*  new_list(&mylist, sizeof(myst)); */
  if (!new_static_file_list(&mylist, sizeof(myst), "datab", 1000))
    {
      printf("Could not make static file list!\n");
      exit(1);
    }
  for (;;)
    {
      k = (elements(&mylist) > 20) ? 20 : elements(&mylist);
      for (i=0;i<k;i++)
	{
	  printf("%d:",i);
	  for (j=0;j<indexes(&mylist);j++)
	    printf("    %02d-%-10s-%-3d",
		   *(index_loc(&mylist, i, j)),
		   element_of_index(myst, &mylist, i, j)->name,
		   element_of_index(myst, &mylist, i, j)->number);
	  printf("\n");
	}
      printf("Choice: %d {A,D} ",elements(&mylist));
      gets(s);
      switch (*s)
	{
   	  case '1':
	    add_index(&mylist, byname);
	    break;
	  case '2':
	    add_index(&mylist, bynumber); 
	    break;
	  case '3':
	    delete_index(&mylist, 0);
	    break;
	  case 'f':
	    printf("filename: ");
	    gets(s);
	    if (fp=fopen(s,"r"))
	      {
		printf("opened %s\n",s);
		while (!feof(fp))
		  {
		    fgets(add.name, 9, fp);
		    for (i=0;i<10;i++)
		      if ((add.name[i] == '\n') || (add.name[i] == '\t') ||
			  (add.name[i] == ' '))
			add.name[i] = '\000';
		    if (*add.name)
		      {
			add.number = add.name[2]+add.name[3];
			add_list(&mylist,&add);
		      }
		  }
		fclose(fp);
	      }
	    break;
	  case '9':
	    free_list(&mylist);
	    break;
	  case '8':
	    re_sort(&mylist);
	    break;
	  case 'a':
   	  case 'A': 
	    printf("name: ");
	    gets(add.name);
	    printf("number: ");
	    gets(s);
	    add.number = atoi(s);
	    add_list(&mylist,&add);
	    break;
          case 'd':
	  case 'D':
	    printf("number: ");
	    gets(s);
	    delete_list(&mylist,atoi(s));
	    break;
	  }
    }
}













