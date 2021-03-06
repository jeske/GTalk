#include <stdio.h>

#define circleplus(x,y) ((x) ^ ((y) & 0x07))

unsigned int convstring(char *string, int encrypt)
{
    char prev = 0x05;
    char temp;
    unsigned int checksum = 0;

    while (*string)
    {
       if (encrypt)
       {
           checksum += (unsigned int) *string;
           prev = *string = circleplus(*string,prev);
       }
       else
       {
           temp = *string;
           *string = circleplus(*string,prev);
           checksum += (unsigned int) *string;
           prev = temp;
       };
       string++;
    };
    return(checksum);
};

void main(int argc, char *argv[])
{
    int checksums[100];
    int numlines = 0;
    char s[80];
    char *t;
    int count;

    printf("char *str[] = {\n");

    do
    {
        gets(s);
        if (*s)
        {
            printf("/* Arry Elmt %d: %s */\n",numlines,s);
            checksums[numlines++]=convstring(s,1);
            t = s;
            printf("\"");
            while (*t)
            {
                if (*t == '\"') printf("\\\""); else
                if (*t == '\\') printf("\\\\"); else
                 printf("%c",*t);
                *t++;
            };
            printf("\",\n\n");
        };

    } while ((*s) && (*s!=EOF));
    printf("};\n\n");

    printf("int checksums[] = {\n");
    for (count=0;count<numlines;count++)
    {
        printf("%d,\n",checksums[count]);
    };
    printf("};\n");
};
