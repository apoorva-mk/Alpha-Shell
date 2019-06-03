#include<stdio.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<curses.h>
#include<readline/readline.h> 
#include<readline/history.h> 

//Limit on the number of character per command and number of commands
#define MAXCHAR 1000
#define MAXCOMM 100

//Macro for clearing screen
#define clear() printf("\e[1;1H\e[2J")

//Defining text colours
#define KYEL  "\x1B[33m"
#define KCYN  "\x1B[36m"
#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"

void init()
{
	clear();
	//textcolor('RED');
	printf("\n%s                                      WELCOME TO \n\n",KYEL);
	printf("\n%s    *     *     ****   *   *      *                 ****  *   *  ***** *     *    ", KCYN );
	printf("\n%s   * *    *     *   *  *   *     * *               *      *   *  *     *     *    ", KCYN );
	printf("\n%s  *****   *     ****   *****    *****               ***   *****  ***** *     *    ", KCYN);
	printf("\n%s *     *  *     *      *   *   *     *                 *  *   *  *     *     *    ", KCYN);
	printf("\n%s*       * ***** *      *   *  *       *            ****   *   *  ***** ***** *****",KCYN );
	printf("\n\n");

	// char* username = getenv("USER"); 
 //    printf("\n\n\nUSER is: @%s", username); 
 //    printf("\n"); 
	printf("Enter 'help' to find list of alpha shell commands");

}

void getUser()
{
	char* user = getenv("USER");
	printf("\n@%s",user);
	printf("\n");
}

void menuHelp()
{
	clear();
	printf("\nCommands....\nuser -> to print current user\ndir -> to get directory name");

}

void getInput()
{
	printf("\n>>>");
	char buffer[MAXCHAR]; 
	scanf("%[^\n]%*c", buffer); 
	//printf("%s", buffer);  
}

void getDir()
{
	char dir[MAXCHAR]; 
    char *res = getcwd(dir, sizeof(dir)); 

    if(res!=NULL)
    printf("\n%s", dir); 

	else		
	printf("\n%sError fetching current directory!\n",KRED);
}

void Exit()
{
	printf("\nClosing shell...");
	exit(0);
}


//To execute fork the child and then make execute the command in the child process
void execComm(char ** commands)
{
	pid_t pid = fork();

	if(pid<0)
	printf("\n%sError in forking child",KRED);

	else if(pid==0)
	{
		if(execvp(commands[0],commands)<0)
			printf("\n%sError in forking child",KRED);

		exit(0);
	}

	else
	{
		wait(NULL);
		return;
	}
}

//To execute pipeline commands we need to change the file descriptor out the ends to that of stdout and stdin
void execPipeCommands(char **commands, char **pipecommands)
{
	int pipe_file_desc[2];

	if(pipe(pipe_file_desc)<0)
	{
		printf("\n%sError in initialising the pipe!",KRED );
		return ;
	}

	pid_t p1=fork();

	if(p1<0)
		printf("\n%sError in forking write child process",KRED );

	
	if(p1 == 0)
	{
		//Close the read end
		close(pipe_file_desc[0]);

		//Make stdout file descriptor point to the write pipe end
		dup2(pipe_file_desc[1],STDOUT_FILENO);

		//Unlink pipe write end
		close(pipe_file_desc[1]);

		if(execvp(commands[0],commands)<0)
		{
			printf("\n%sError in executing command 1!",KRED );
			exit(0);	
		}
	}
	else
	{
		pid_t p2 = fork();
		if(p2 < 0)
		{
			printf("\n%sError in forking read child!",KRED );
			return ;
		}

		if(p2 == 0)
		{
			//Close the write end
			close(pipe_file_desc[1]);
			dup2(pipe_file_desc[0],STDIN_FILENO);
			close(pipe_file_desc[0]);

			if(execvp(pipecommands[0],pipecommands)<0)
			{
				printf("\n%sError in executing command 2!",KRED );
				exit(0);	
			}

		}

		else
		{
			wait(NULL);
			wait(NULL);
		}
	}
	
}

//Split input into the two commands of the pipe
int parsePipedCommands(char *str, char ** strPipeCommands)
{
	int i=0;
	//strsep splits the string whereever there's a |
	for(;i<2;i++)
	{
		strPipeCommands[i] = strsep(str,"|");
	}

	//No piped command found if second argument is empty
	if(strPipeCommands[1]==NULL)
		return 0;

	else
		return 1;
}


//splits the input command into separate words 
void findCommands(char *str, char **parsedCommands)
{
	int i=0;
	for(;i<MAXCOMM;i++)
	{
		parsedCommands[i] = strsep(str," ");

		if(parsedCommands[i]==NULL)
			break;     //Since no more commands left

		if(strlen(parsedCommands[i])==0)
			i--;
	}
}

int myCommands(char **commands)
{
	return 0;
}

int processCommands(char * input, char **strCommands ,char **strPipeCommands)
{
	char* commands[2];
	//commands will hold the two parts incase of a pipe or just one otherwise

	//checking for if pipeline is there 
	int check_for_pipe = parsePipedCommands(input, commands);

	//If pipeline exists break down the individual parts of the pipe into commands
	if(check_for_pipe)
	{
		findCommands(commands[0], strCommands);
		findCommands(commands[1], strPipeCommands);
	}

	else
	{
		findCommands(commands[0], strCommands);
	}

	// strCommands holds commands of the first part while strPipeCommands holds commands
	// of the piped part

	if (myCommands(strCommands))
		return 0;

	else 
		return 1 + check_for_pipe; 


}





int main()
{
	init();
	getInput();
	getDir();
	Exit();
	printf("\nMain");
	return 0;
}



