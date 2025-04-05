/*******includes Section***********/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/*******Mcros Section*************/
#define COUNT 1024

int main(int argc, char** argv)
{
    /*checking for arguments*/
    if(argc != 1)
    {
        printf("Usage: %s\n", argv[0]);
        exit(-1);
    }
    /*creat buffer to store path in this buffer*/
    char buf[COUNT];
    /*checking for getced() reurn value*/
    /*@buf: char array to store path, 
     @COUNT: number of bytes to store in buf*/
    if(getcwd(buf, COUNT) != NULL)   
    {
        /*checking for write() return value*/
        /*1 : to write in stdout 'terminal'
         buf: data write() will print in stdout
         strlen(buf): buffer actual size reurned from getcwd()
        */
        if(write(1, buf, strlen(buf)) < 0)
        {
            printf("error: can not write in stdout\n");
            exit(-2);
        }
	printf("\n");
    }
    else
    {
        printf("error: can not read path\n");
        exit(-3);
    }
    return 0;
}
