/******************includes section*******************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
/*****************macros section*********************/
#define COUNT 100

int main(int argc, char* argv[])
{
	/*checking for arguments 1.program 2.source file 3.destination*/
	if(argc != 3)
	{
		printf("Usage: %s source_file destination_file\n", argv[0]);
		exit(-1);
	}
	/*open() return file descriptor(fd) to source file (argv[1])*/
	int fds = open(argv[1], O_RDONLY);
	/*checking fds value if less than 0 its mean open() return error*/
	if(fds < 0)
	{
		printf("could not open source file\n");
		exit(-2);
	}
	/*open() flags mode :create or open existing file to reading and writeing, truncating to zero bytes*/
	int openFlags = O_CREAT | O_RDWR | O_TRUNC;
	/*open() retuen file descriptor (fd) to destination file (argv[2])
	 * 0644:destionation file  permissions mode*/
	int fdd = open(argv[2] , openFlags, 0644);
	/*checking fdd value if less than 0 its mean open() return error*/
	if(fdd < 0)
        {
                printf("could not open destination file\n");
                exit(-3);
        }
	/*numread: actual bytes read() syscall read*/
	int numread;
	/*buf: buffer of size COUNT (100) to store date read from source file*/
	char buf[COUNT];
	/*while read() dont reach End of File (EOF = 0) still read and write*/
	while((numread = read(fds, buf, COUNT)) > 0)
	{
		/*checking for write() return value*/
		if((write(fdd, buf, numread) < 0))
		{
			printf("could not apple to write in destination file\n");
			exit(-4);
		}
	}
	/*close source file and destination file*/
	close(fds);
	close(fdd);
	return 0;
}
