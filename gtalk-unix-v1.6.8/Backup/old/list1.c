
int search_list_for(list *clist, int ind_no, void *find, int *exact)
{
  register list_int first;
  register list_int last;
  register list_int mid;
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
  last = clist->cur_no - 1;
  for (;;)
    {
      mid = (first + last + 1) >> 1;
      srch=((cf)(find,element_of_index(void,clist,mid,ind_no)));
      printf("wow %d/%d/%d/%d/%d\n",srch,mid,first,last,ind_no);
      if (!srch)
	{
	  *exact = 1;
	  printf("exact!\n");
	  return (mid);
	}
      else
	if (srch < 0)
	  {
	    if (mid == first)
	      {
		*exact = 0;
		return (mid);
	      } 
	    last = mid;
	  }
	else
	  {
	    if (mid == last)
	      {
		*exact = 0;
		return (mid);
	      }
	    first = mid;
	  }
    }
}      
