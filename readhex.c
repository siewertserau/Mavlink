#include <stdio.h>
#include <stdlib.h>

void main(void)
{
    FILE *fp;
    unsigned char buffer[80];

    fp=fopen("/dev/ttyUSB0", "r");

    if(fp == (FILE *)0)
    {
        perror("opening ttyUSB0");  
        exit(-1);
    }
   
    while(1)
    {
        fread(buffer, 1, 1, fp);
        printf("%X", buffer[0]);
    } 
}
