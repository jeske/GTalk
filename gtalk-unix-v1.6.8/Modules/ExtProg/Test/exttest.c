
/******************************

         exttest.c

 ******************************/


#include <stdio.h>
#include <stdlib.h>

#include "abuf.h"
#include "extcon.h"


void main(int argc, char **argv)
{
  int temp, i;
  abuffer abuf;
  char s[1000];

  if (extrn_connect_pipe(NULL) < 0)
    {
      fprintf(stderr,"Can not connect to Gtalk pipe\n");
      exit(1);
    }
  if (extrn_connect_shm() < 0)
    {
      fprintf(stderr,"Can not connect to Gtalk shared memory\n");
      exit(1);
    }
  for (;;)
    {
      for (i=0;i<c->devices_used;i++)
	{
	  if (c->devices[i].status != DEVICE_EMPTY)
	    printf("device %s\n",c->devices[i].name);
	}
      temp = read_abuffer(mypipe, &abuf, s, 1000);
      if (temp < 0)
	break;
      if (temp > 0)
	printf("%s\n",s);
    }
}
