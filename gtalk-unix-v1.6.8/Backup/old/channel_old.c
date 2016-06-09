/***********************************

           Channel List

 ***********************************/

channel *base = NULL;

int make_channel_name(char *oldname, char *newname)
{
  int len = 0;

  while (((*oldname >= 'A') && (*oldname <= 'Z')) ||
	 ((*oldname >= 'a') && (*oldname <= 'z')) ||
	 ((*oldname >= '0') && (*oldname <= '9')))
    {
      if ((*oldname >= 'A') && (*oldname <= 'Z'))
	*newname++ = (*oldname++ + ' ');
      else
	*newname++ = *oldname++;
      len++;
    }
  *newname = '\000';
  return (len);
}
  
channel *find_channel(char *channel)
{
  char temp[CHANNEL_NAME_LEN+1];
  channel *search = base;

  make_channel_name(channel, temp);
  while (search)
    {
      if (!(strcmp(temp, search->name)))
	return (search);
    }
  return (NULL);
}

int add_node_to_channel(char *channel, int node, int monitor_only)
{
  channel *search = find_channel(channel);

  if (search)
    {
      if (test_bit(search->barred, node))
	return (-1);
      set_bit(search->monitors, node);
      search->num_monitor++;
      if (!monitor_only)
	{
	  set_bit(search->onchannel, node);
	  search->num_onchannel++;
	}
      return (0);
    }
  if (!(search = malloc(sizeof(channel))))
    {
      return (-1);
    }
  search->next = base;
  base = search;
  make_channel_name(channel, search->name);
  *search->title = '\000';
  clear_bits(search->monitors, MAX_NODES);
  clear_bits(search->onchannel, MAX_NODES);
  clear_bits(search->moderator, MAX_NODES);
  clear_bits(search->barred, MAX_NODES);
  search->num_monitor = search->num_onchannel =
    search->num_moderator = search->num_barred = 0;
}

int channel_process(g_uint32 type, abuf *abuf, char *message)
{
};

	  









