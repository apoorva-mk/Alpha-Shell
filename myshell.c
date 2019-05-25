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




int main()
{
	init();
	getInput();
	getDir();
	Exit();
	printf("\nMain");
	return 0;
}



