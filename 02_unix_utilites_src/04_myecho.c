#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv)
{
	if(argc > 1)
	{
		int i = 1;
		while(argv[i] != NULL)
		{
			if(write(1, argv[i], strlen(argv[i])) < 0)
			{
				printf("error to write in stdout\n");
				exit(-1);
			}
			printf(" ");
			fflush(stdout);
			i++;
		}
		printf("\n");
	}
	else{ printf("\n");}
	return 0;
}
