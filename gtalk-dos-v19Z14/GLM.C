
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dir.h>
#include <dos.h>
#include <time.h>
#include "jumptbl.h"


void main(int argc, char **argv)
{
  FILE *fp;
  FILE *op;
  char filename[256];
  char outfilename[256];
  char rotstring[256];
  unsigned long int length;
  char *memory;
  char *cur;
  char *end;
  int found = 0;

  if (argc<2)
  {
    printf("Filename required\n");
    exit(1);
 }

strcpy(filename,argv[1]);
strcat(filename,".bin");

if (!(fp=fopen(filename,"rb")))
{
  printf("Could not open '%s'\n",filename);
  exit(1);
}

fseek(fp,0,SEEK_END);
length = ftell(fp);

if (length>MAX_MODULE_LEN)
{
  printf("'%s' too large, %lu bytes, needs to be %lu bytes\n",
            filename,length,MAX_MODULE_LEN);
  fclose(fp);
  exit(1);
}

memory = malloc(length);
if (!memory)
{
  printf("Could not get memory!\n");
  fclose(fp);
  exit(1);
}

fseek(fp,0,SEEK_SET);
fread(memory,sizeof(char),length,fp);

end = memory + length - sizeof(struct startblock);
cur = memory;

found = 0;
while ((cur<end) && (!found))
{
  if (!strcmp(cur,LD_STARTID)) found = 1;
    else cur++;
}

if (!found)
{
  printf("No GLM header found in file '%s'\n",filename);
  fclose(fp);
  free(memory);
  exit(1);
}

((struct startblock *)cur)->length = length;

strcpy(outfilename,argv[1]);
strcat(outfilename,".glm");

if (!(op=fopen(outfilename,"wb")))
{
  printf("Could not open output file '%s'\n",outfilename);
  fclose(fp);
  free(memory);
  exit(1);
}

fwrite(cur,sizeof(struct startblock),1,op);
fwrite(memory,sizeof(char),length,op);
fclose(op);
fclose(fp);
printf("GLM file '%s' created\n",outfilename);
printf("Length %04X\n",((struct startblock *)cur)->length);
free(memory);

exit(0);

}
