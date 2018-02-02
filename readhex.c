#include <stdio.h>
#include <stdlib.h>

void main(int argc, char** argv)
{
    FILE *fp;
    unsigned char buffer[80];

    if(argc < 2)
    {
        printf("Will use ttyUSB0\n");
        fp=fopen("/dev/ttyUSB0", "r");
    }
    else
    {
        printf("Will use %s\n", argv[1]);
        fp=fopen(argv[1], "r");
    }

    if(fp == (FILE *)0)
    {
        perror("opening device or ttyUSB0");  
        exit(-1);
    }
   
    while(1)
    {
        fread(buffer, 1, 1, fp);
        printf("%X", buffer[0]);
    } 
}
