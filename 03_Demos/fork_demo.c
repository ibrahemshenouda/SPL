#include <stdio.h>
#include <unistd.h>
int x = 5; 
int y;
int main(int argc, char** agrv)
{
	int z = 10;
	printf("before creating new process, to creat press any key\n");
	getchar();

	pid_t child_pid = fork();
	/*checking of new process created or failed 
	 * this statement will performed in the parent process*/
	if(child_pid > 0)
	{
		while(1)
		{	
			x++; y++; z++;
			printf("PARENT: x = %d y = %d z = %d\n",x,y,z);
			printf("PARENT: my pid = %d, my chiled pid = %d \n",getpid(), child_pid);
			sleep(1);
		}
	}
	/*this statement will performed in child process if fork() success*/
	else if(child_pid == 0)
	{
		while(1)
		{
			x+=2; y+=2; z+=2;
                        printf("CHILD: x = %d y = %d z = %d\n",x,y,z);
			printf("CHILD: my pid = %d, my parent ppid = %d \n", getpid(), getppid());
			sleep(1);
		}
	}
	/*this statement will perform in the parent process if fork() failled*/
	else
	{
		printf("failed to creat new process\n");
	}
	return 0;
}
