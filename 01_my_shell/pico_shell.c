#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 100
char* read_buf();
char** parse_args(char* line , int* ret_argc);
int builtin_uts(char** args, int argc);
void  prorgam_execute(char** args);


int main()
{
	char* buf;
	char** args;
	int argc;
	while(1)
	{
		printf("Pico shell>> ");
		buf = read_buf();
		if(buf == NULL || strlen(buf) == 0)
		{
			continue;
		}

		args = parse_args(buf ,&argc);

		int result = builtin_uts(args, argc);

		if(result == 0)
		{
			continue;
		}
		else if(result == -1)
		{
			prorgam_execute(args);
		}
	}
	return 0;
}

/*@bref: this function to take user input
 *@retunr: char pointer to the buffer "array" we stored user line in*/
char* read_buf()
{
	/*malloc(): allocate BUF_SIZE size in memory and return char pointer to this location if success*/
	char* buf = malloc(BUF_SIZE);
	/*if malloc() can not allocate spesified space returns NULL*/
	if(buf == NULL)
	{
		printf("null buf in read-buf()\n");
	}
	/*fgets():read user line
	 * buf: pointer to allocated space
	 * BUF_SIZE: size of allocated space
	 * stdin: reading forom standard input*/
	if(fgets(buf, BUF_SIZE, stdin) == NULL)
	{
		printf("null fgets() ret in read-buf()\n");
	}
	/*removing newline addded by fgets()*/
	buf[strlen(buf) - 1] = 0;
	/*return: pointer "buf" to allocated space*/
	return buf;
}

/*@breif: this function to parese user line to tokens and generate argument array
 *@return: pointer to array of pointers, each pointer in this array points to one argument
 *@parm_1: user line
 *@poam_2: number of arguments */
char** parse_args(char* line , int* ret_argc)
{
	/*buf_size: initial size to the array we will store agruments in*/
	int buf_size = 8;
	/*argc: argument counter*/
	int argc = 0;
	/*args: pointer to array of pointers each pointer in this array points to one argument
	 * malloc(): returns a pointer to allocated sapce if success
	 * it takesone parameter: number of bytes specified to allocate in memory*/
	char** args = malloc(buf_size * sizeof(char*));
	/*strtok(line, delimiter): returns a pointer to token and replace delimiter by \0  */
	char* token = strtok(line, " ");
	/*looping while token != NULL = while not reach the end of line  "last location fgets() buts in buf"*/
	while(token != NULL)
	{
		/*if argc reach buf_size reallocate args by double of buf_size*/
		if(argc >= buf_size)
		{
			buf_size *=2;
			/*realloc(): take pointer to space we want to reallocate and the new size
			 * returns pointer to the new space*/
			args = realloc(args, buf_size * sizeof(char*));
		}
		/*strdup():adding the pointed string by its token pointer to args array in the argc location and
		 * increment argc*/
		args[argc++] = strdup(token);
		/*go to new token start form "NULL" in user line we replaced " " 
		 * and return: pointer to the new token*/
		token = strtok(NULL, " ");
	}
	/*after parsing finish specifiy the last location inargs by "NULL"*/
	args[argc] = NULL;
	/*return the number of arguments by argc_out pointer*/
	*ret_argc = argc;
	/*return the pointer to array of pointers ppints to arguments*/
	return args;
}

/*@breif: built_in command in our programm
 *@return: int value to exit status
 *@parm_1: pointer to array of pointers points to arguments
 *@parm_2: number of arguments*/
int builtin_uts(char** args, int argc)
{	
	/*no commands or arguments return 1*/
	if(argc == 0)
	{
		return 1;
	}
	/*compare command args[0] with "exit", if returned 0 it mean 2 strings matched*/
	if(strcmp(args[0], "exit") == 0)
	{
		printf("Good bye: )\n");
		exit(0);
	}
	/*compare command args[0] with "pwd", if returned 0 it mean 2 strings matched*/
	else if(strcmp(args[0], "pwd") == 0)
	{
		/*PATH_MAX: macro in system to define max_path size*/
		char pwd[PATH_MAX];
		/*get cuuernt working directory by getcwd()*/
		getcwd(pwd, sizeof(pwd));
		printf("%s\n", pwd);
	}
	/*compare command args[0] with "cd", if returned 0 it mean 2 strings matched*/
	else if(strcmp(args[0], "cd") == 0)
	{
		/*path must be one argument*/
		if(argc == 2)
		{
			/*chdir(): to change diroctory by passing new path*/
		       int ret = chdir(args[1]);
		       if(ret == 0)
		       {
			       /*feature: print new path in prompt*/
		       }
		       else
		       {
			       printf("%s: No such file or directory\n",args[1]);
		       }
		}
		else if(argc > 2)
		{
			printf("cd: too many arguments\n");
		}
	}
	/*compare command args[0] with "echo", if returned 0 it mean 2 strings matched*/
	else if(strcmp(args[0], "echo") == 0)
	{
		int i;
		for(i = 1;i < argc; i++)
		{
			printf("%s ",args[i]);
		}
		printf("\n");
	}
	else
	{
		return -1;
	}

	return 1;
}
/*@breif: this function to run any parogram passing py path "not built in program"
 *@parm: pointer to array of pointers points to arguments*/
void prorgam_execute(char** args)
{
	/*create child process, returns child id*/
	pid_t pid = fork();

	/*return zero to child process*/
	if(pid == 0)
	{
		/*execvp: searching in PATH by args[0]
		 * PATH: environment variable contains some directories seperated by ":"*/
		execvp(args[0], args);
	}
	/*in parent process*/
	else if(pid > 0)
	{
		/*whaiting chiled process to terminate*/
		wait(NULL);
	}
	else
	{
		/*nothing*/
	}
}


