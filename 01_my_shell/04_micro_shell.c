/************includes section**************/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

/***********macros defintions**************/
#define BUF_SIZE 100

/**********data types defintion************/

/*Var: drived data type consists of:
 * name: char pointer points to char data
 * value: char pointer points to char data"char array"
 * exported: integer data refer to data environment or local
 * next:pointer to the created new Var */
typedef struct Var{
	char* name;
	char* value;
	int exported; /*1: environment*/

	struct Var* next;	
}Var;
/*var_list: is apointer to Var type "pointer to linked list each element is a struct Var"*/
Var* var_list = NULL;
/********interfaces declerations**********/
char* read_buf();
char** parse_args(char* line , int* ret_argc);
int builtin_uts(char** args, int argc);
void  program_execute(char** args);
void set_variable(const char* name, const char* value, int exported);
void set_new_value(char* old_value, int new_value);
char* get_variable(const char* name);
void free_variables();
void var_sub(char** args, int argc);
char var_check(char* buf, char exp);
int  io_redirection(char **args);
/*********entery point:main()*************/
int main()
{
	char* buf;
	char** args;
	int argc;
	/*if user not enter exit command, our shell still running*/
	while(1)
	{
		/*shell prompt*/
		printf("Nano Shell >> ");
		/*reading line from the user*/
		buf = read_buf();
		/*if user dont enter any thing continue*/
		if(buf == NULL || strlen(buf) == 0)
		{
			continue;
		}
		/*saving copy of buffer*/
                char* buf_tmp = strdup(buf);
		/*passing buf_tmp becouse parse_argc() chane in the buf*/
		args = parse_args(buf_tmp, &argc);
		/*if the user enterd '='sign*/
		if(argc == 1  && strchr(buf, '='))
		{
			/*var_check(): cheking var is local or environment, 0 to local*/
			char check = var_check(buf, 0);
			if(check == 1)
			{
				printf("invalid command\n");
				continue;
			}
			continue;
		}
		/*variable substitution*/
		var_sub(args, argc);
		
		/*command built_in or executable file?*/
		int result = builtin_uts(args, argc);
		if(result == -1)
		{
			program_execute(args);
		}

		/*free all allocated spaeces*/
		for(int i = 0; i < argc; i++)
		{
			free(args[i]);
		}
		free(args);
		free(buf);
	}
}

/***********************************interfaces declerations******************************************/
/****************************************************************************************************/
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

		/*tokenization io_redirections '<' '>' '2>'*/
		if((token[0] == '>' ||token[0] == '<') && strlen(token) > 1)
		{
			args[argc++] = strndup(token, 1);       /*token io*/
			args[argc++] = strdup(token +1);	/*tokenize filename*/
		}
		else if(strncmp(token, "2>", 2 ) == 0 && strlen(token) >2)
		{
			args[argc++] = strndup(token, 2);
			args[argc++] = strdup(token + 2);
		}
		else
		{
		args[argc++] = strdup(token);
		}
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

		
	/*no commainds or arguments return 1*/
	if(argc == 0)
	{
		return 1;
	}
	/*compare command args[0] with "exit", if returned 0 it mean 2 strings matched*/
	if(strcmp(args[0], "exit") == 0)
	{
		printf("Good bye: )\n");
		free_variables();
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
	/*built_in export keyword*/
	else if(strcmp(args[0], "export") == 0)
	{
		/*checking for export form*/
		if(argc != 2)
		{
			printf("Usage: export varname=value\n");
		}
		else
		{
			/*var check: if exported in the right way stores it as environment var*/
   			var_check(args[1], 1);			
		}
        }
	else
	{
		return -1;
	}

	return 1;
}
/*@breif: this function to run any parogram passing py path "not built in program"
 *@parm: pointer to array of pointers points to arguments*/
void program_execute(char** args)
{
	/*create child process, returns child id*/
	pid_t pid = fork();
	
	/*return zero to child process*/
	if(pid == 0)
	{
		
		/*this lines to copy all environment vars to "envp" array and passing it to exvpe()*/

		/*count environment vars in the curent process*/
		int env_counter = 0;
		/*curr: pointer to var_list*/
		Var* curr = var_list;
		while(curr)
		{
			/*if returned 1: means environment var*/
			if(curr->exported)
			{
				env_counter++;
			}
			curr = curr->next;

		}
		/*creating a pointer to  array of pointers its size = env_counter+1 for NULL, to store env vars*/
		char** envp = malloc((env_counter+1)*sizeof(char*));
		int i = 0;
		curr = var_list;
		while(curr)
		{
			/*copy only env vars in envp array each element is a string "name=value"*/
			if(curr->exported)
			{
				/*string lenth = name_length+ value-lentgh+ 2 for ""*/
				int len = strlen(curr->name)+ strlen(curr->value)+2;
				envp[i] = malloc(len);
				/*write in each element created*/
				snprintf(envp[i], len, "%s=%s", curr->name, curr->value);
				i++;
			}
			curr = curr->next;
		}
		envp[i] = NULL;
		 /*execvp: searching in PATH by args[0]
                 * PATH: environment variable contains some directories seperated by ":"*/
		if(io_redirection(args) != 0)
                {
                        exit(-1);
                }
		int status = execvpe(args[0], args, envp);
		if(status == -1)
		{
			printf("invalid command\n");
			exit(-1);
		}
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

/*@breif: set new variable or overwrite by new value or adding to environment variables
 *@return: void
 *@parm_1: pointer to name of variable
 *@parm_2: pointer to vlaue 
  @parm_3: integer value to refer local oe env var*/
void set_variable(const char* name, const char* value, int exported)
{
	/*pointer to var_list*/
	Var* curr = var_list;
	/*searching this element existing or not*/
	while(curr != NULL)
	{
		/*if existing, overwrite by new value if != null
		 * and if adding to environment vars set exportted*/
		if(strcmp(curr->name, name) == 0)
		{
			if(value != NULL)
			{
				free(curr->value);
				curr->value = strdup(value);
			}
			if(exported == 1)
			{
				curr->exported = 1;
			}
			return;
		}
		curr = curr->next;
	}
	
	/*if not existing create new node and adding to our linked list*/
	Var *new_var = malloc(sizeof(Var));
	new_var->name = strdup(name);
	new_var->value = strdup(value);
	new_var->exported = exported;
	new_var->next = var_list;
	var_list = new_var;

}
/*@breif: to get varible value by its name 
 *@return: pointer to value
 *@parm_1: pointer to name */
char* get_variable(const char* name)
{
	Var* curr = var_list;
	while(curr != NULL)
	{
		if(strcmp(curr->name , name) == 0)
		{
			return curr->value;
		}
		curr= curr->next;
	}
	return NULL;
}
/*@breif:to free all vars if exit
 *@return: void  */
void free_variables()
{
	Var* curr = var_list;
	while(curr)
	{
		Var* next = curr->next;
		free(curr->name);
		free(curr->value);
		free(curr);
		curr = next;
	}
}

/*@breif:print variables if user enterd $"varname"
 *@return: void
 *@parm_1: pointer to pointer array of arguments entered
 *@parm_2: number of arguments*/
void var_sub(char** args, int argc)
{
	/*in this version: if args[i] start with $var replace the next var name by its value 
	 * in the next versions will be replace $var in any place in args[i] not only in the begin*/
	for(int i = 0; i < argc; i++)
	{
		if(args[i][0] == '$')
		{
			char* var_name = args[i]+1;
			char* val = get_variable(var_name);
			free(args[i]);
			if(val)
			{
				args[i] = strdup(val);
			}
			else
			{
				args[i] = strdup("");
			}
		}
	}
}

/*@breif: check var in the rigth way or not
 *@return: status of program
 *@parm_1: pointer to user line
 *@parm_2: local or environment
 */
char var_check(char* buf,  char exp)
{

	/*searching for '=' sign*/
                 char* eq = strchr(buf, '=');
		 char* name;
		 char* value;
		 /*if '=' is exiting , name is the string before '=', and value is the string after '='*/
		 if(eq != NULL)
		 {	
		         name = strndup(buf, eq - buf);
		         value = strdup(eq+1);
		 }
		 /*else: str name = buf, and value = null*/
		 else if(eq == NULL)
		 {
			 name = strdup(buf);
			 value = NULL;
		 }
		 /*if vare is local, and writen in the right way*/
                 if((eq != NULL) &&( strchr(buf, ' ') == NULL) && exp == 0)
                  {
                         if(eq == buf || *(eq+1) == '\0')
                          {
                                  return 1;
                          }
                          set_variable(name, value, exp);
                          free(name);
                          free(value);
		          return 0;
                  }
                  else if(eq && strchr(buf, ' ') != NULL && exp == 0)
                  {
                         return 1;
                  }
		 else if(exp == 1)
		 {
			set_variable(name, value, exp);
		 }
		 else
		 {
			return 1;
		 }
}

int io_redirection(char **args)
{
	for(int i = 0; args[i] != NULL; i++)
	{
		if(strcmp(args[i], ">") == 0)
		{
			int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(fd < 0)
			{
				printf("failed to open file\n");
				return -1;
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		
			while(args[i])
			{
				args[i] = args[i+2];
				i++;
			}
			i = -1;
		}

		else if(strcmp(args[i], "<") == 0)
		{
			int fd =  open(args[i+1], O_RDONLY);
			if(fd < 0)
                        {
                                printf("failed to open file\n");
                                return -1;
                        }
			dup2(fd, STDIN_FILENO);
			close(fd);
			
			while(args[i])
			{
				args[i] = args[i+2];
				i++;
			}
			i = -1;
		}
		else if(strcmp(args[i], "2>") == 0)
		{
			int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(fd < 0)
                        {
                                printf("failed to open file\n");
                                return -1;
                        }
			dup2(fd, STDERR_FILENO);
			close(fd);

			while(args[i])
			{
				args[i] = args[i+1];
				i++;
			}
			i = -1;
		}
	}
	return 0;
}
