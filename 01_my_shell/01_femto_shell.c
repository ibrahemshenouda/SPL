#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 	100

int main()
{
    /*buffer to store user comand */
    char buf[BUF_SIZE];
    /*while to still performing user programs until exit() our shell */
    while (1) {
	/*shell prompt */
	printf("Femto_sheel: ");
	/*fgets(): to get sdtring from user from stdin and store in buf */
	fgets(buf, BUF_SIZE, stdin);
	/*fgets(): Reading stops after an EOF or a newline.  If a newline is read, it is stored into the buffer.
	 * this line to remove this new line*/
	buf[strlen(buf) - 1] = 0;
	/*if user dont enter any key */
	if (strlen(buf) == 0) {
	    continue;
	}
	/*if user entered any key will create new process to perform user program */
	pid_t pid = fork();
	/*in parent process */
	if (pid > 0) {
	    /*wait(): to stop parent process untill child process terminated "finished"
	     *and return status of child process
	     *then cheking for the status of child process performed successfully or faild */
	    int status;
	    wait(&status);
	    /*if child process failled prints user command not found */
	    if (status != 0) {
		printf("%s not found\n", buf);
	    }
	}
	/*child process */
	else if (pid == 0) {
	    /*create new process in child process by execve() system call */
	    char *newargv[] = { buf, NULL };
	    char *newenvp[] = { NULL };
	    /*execve() replace child process by new process
	     * and if this process successfull that mean child process successful and return 0 to parent process*/
	    execve(buf, newargv, newenvp);
	    /*exiting new process() */
	    exit(-1);
	} else {
	    printf("failed to creat new process\n");
	}

    }
    return 0;
}
