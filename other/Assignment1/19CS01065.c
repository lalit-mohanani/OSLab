#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include<signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>
#include <fcntl.h>

/*

  Used GNU Readline 
  Install Readline: sudo apt-get install libreadline-dev
  Compile using gcc: gcc -o shell 19CS01065.c -lreadline
  Execute: ./shell

*/

/*Definitions */
#define HISTORY_SIZE 10
#define MAX_COMMAND_SIZE 100
#define MAX_BACKGROUND_JOBS 100
#define NO_OF_BUILTIN_COMMANDS 6

/*Implemented a circular queue of commands for history */
int rear = -1, front = -1;
int current_top = 1;
char commands[HISTORY_SIZE][MAX_COMMAND_SIZE];

/*Implemented two arrays for background_jobs, one of pids and one of commands */
int current_background_process = 0;
int last_index = 0;
pid_t background[MAX_BACKGROUND_JOBS];
char background_jobs[MAX_BACKGROUND_JOBS][MAX_COMMAND_SIZE];

/*List of the BuiltIn Commands of the Shell */
char *list_of_commands[NO_OF_BUILTIN_COMMANDS] = { "jobs", "cd", "history", "!", "kill", "exit" };


/* Declare all the funcntions */
int isFull();
void deQueue();
void enQueue(char *command);
void start_shell();
void display_cwd();
void parseSpace(char *str, char **parsed);
char *previousCommand(int n);
int isBackgroundProcess(char **parsedCommand);
int isBackgroundProcessRunning();
void addBackgroundProcess(char *command, pid_t pid);
int removeBackgroundProcess(pid_t pid);
void addCommand(char *command);
int isEmptyInput(const char *s);
int takeInput(char *str);
void jobsCommand();
void cdCommand(char *path);
void historyCommand();
void processPreviousCommand(char **parsedCommand);
void killCommand(char *parsedCommand);
void exitCommand();
int isBuiltInCommand(char **parsedCommand);
int isInputOutput(char **parsed, char **str, int choice);
void executeCommand(char **parsed);
void processCommand(char *inputString);

int main(int argc, char **argv)
{

	char inputString[MAX_COMMAND_SIZE];

	start_shell();

	while (1)
	{

		display_cwd();

		if (takeInput(inputString))
		{
			continue;
		}
		processCommand(inputString);
	}

	return 0;
}


/*Utility Functions for the Circular Queue of Commands */

//Function to Check if the queue is full.
int isFull()
{

	return ((front == 0 && rear == HISTORY_SIZE - 1) || (rear == (front - 1) % (HISTORY_SIZE - 1)));

}

/* Function to deQueue a command */
void deQueue()
{

	strcpy(commands[front], "");
	if (front == rear)
	{
		front = -1;
		rear = -1;
	}
	else if (front == HISTORY_SIZE - 1)
	{
		front = 0;
	}
	else
	{
		front++;
	}
}

/* Function to enqueue command */
void enQueue(char *command)
{

	if (front == -1)
	{
		front = rear = 0;
	}
	else if (rear == HISTORY_SIZE - 1 && front != 0)
	{
		rear = 0;
	}
	else
	{
		rear++;
	}

	strcpy(commands[rear], command);

}

/*-----------------------------------------------------*/

/* Utility functions */

/*Function to Display Initialize Shell */
void start_shell()
{

	system("clear");
	printf("\n\t Starting Shell.......\n\n");
	sleep(1);
	system("clear");

}

/* Function to display Current Working Directory. */
void display_cwd()
{

	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("\nDirectory: %s >> ", cwd);
	}
	else
	{
		perror("getcwd() error");
	}
}

/*Function to Parse the command through line */
void parseSpace(char *str, char **parsed)
{
	int i;

	for (i = 0; i < MAX_COMMAND_SIZE; i++)
	{	
		parsed[i] = strsep(&str, " ");
		if (parsed[i] == NULL) {
			break;
		}
		if (strlen(parsed[i]) == 0)
			i--;
	}
}

/*Function to return the previous command from the input number */
char *previousCommand(int n)
{

	if (n == -1 && rear != -1)
	{
		return commands[rear];
	}

	if (n >= HISTORY_SIZE + current_top || n < current_top)
	{
		return "-1";
	}

	n = n - current_top;

	if (front + n >= HISTORY_SIZE)
	{

		int rem = HISTORY_SIZE - front;
		int index = n - rem - 1;

		return commands[index];
	}
	return commands[front + n];

}

/*Function to identify if the process is a background process */
int isBackgroundProcess(char **parsedCommand)
{
	// printf("Checking if the process is a background process\n");
	int i = 0;
	for (; parsedCommand[i + 1] != NULL; i++);

	char *lastArguement = parsedCommand[i];

	return (strcmp(lastArguement, "&") == 0);

}

/*Function to check if there are running background processes */
int isBackgroundProcessRunning()
{
	return current_background_process > 0;
}

/*Function to Add Background processes */
void addBackgroundProcess(char *command, pid_t pid)
{

	strcpy(background_jobs[last_index], command);
	background[last_index++] = pid;
	current_background_process++;

}

/*Function to Kill a Background process */
int removeBackgroundProcess(pid_t pid)
{

	int i, found = 0;
	for (i = 0; i < last_index; i++)
	{

		if (background[i] == pid)
		{

			found = 1;

			int result = kill(pid, SIGKILL);
			if (result == 0)
			{
				printf("Child Killed Successfully\n");
				background[i] = -1;
				strcpy(background_jobs[i], "");
				current_background_process--;
			}
			else
				printf("Couldn't kill the process\n");

			return found;
		}
	}

	return found;

}

/*Function to add command to the history. */
void addCommand(char *command)
{

	if (isFull())
	{

		deQueue();
		current_top++;
	}

	enQueue(command);

}

/* Function to check if the input is empty or not. */
int isEmptyInput(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}



/* Function to take input */
int takeInput(char *str)
{
	char *buf;

	buf = readline("$ ");
	if (strlen(buf) != 0 && !isEmptyInput(buf))
	{
		if (buf[0] != '!') addCommand(buf);
		strcpy(str, buf);
		return 0;
	}
	else
	{
		return 1;
	}
}

/*---------------------------------------------*/

/*Functions for Handling the builtIn Commands */

// jobs Command
void jobsCommand()
{

	int i;
	for (i = 0; i < last_index; i++)
	{

		if (background[i] != -1)
		{

			printf("%d: %s\n", background[i], background_jobs[i]);
		}
	}
	printf("\n");

}

// cd Command
void cdCommand(char *path)
{

	chdir(path);

}

//history command
void historyCommand()
{

	int c = current_top;
	if (front == -1)
	{
		return;
	}
	printf("\n");
	if (rear >= front)
	{
		for (int i = front; i <= rear; i++)
			printf("%d: %s\n", c++, commands[i]);
	}
	else
	{
		for (int i = front; i < HISTORY_SIZE; i++)
			printf("%d: %s\n", c++, commands[i]);

		for (int i = 0; i <= rear; i++)
			printf("%d: %s\n", c++, commands[i]);
	}
}

//! command
void processPreviousCommand(char **parsedCommand)
{
	if ((parsedCommand + 1) == NULL || parsedCommand[1]==NULL)
	{

		printf("Invalid command, no previous exists with the number\n");
		return;
	}
	int n = atoi(parsedCommand[1]);

	char *command = previousCommand(n);
	if (strcmp(command, "-1") == 0)
	{
		printf("Invalid command, no previous exists with the number\n");
		return;
	}
	processCommand(command);
}

//kill command
void killCommand(char *parsedCommand)
{

	if (parsedCommand == NULL || parsedCommand[0] != '%')
	{
		printf("Inalid syntax, syntax is kill %% PID\n");
		return;
	}

	int pid = atoi(parsedCommand + 1);

	if (!removeBackgroundProcess(pid))
	{

		printf("No process with the given id exists\n");
		return;
	}
}

//exit command
void exitCommand()
{

	if (isBackgroundProcessRunning())
	{

		printf("Background processes are still running, kill them before exiting\n");
		return;
	}
	printf("Exiting...\n");
	system("clear");
	exit(0);

}

/*--------------------------------------------*/

/*Function to process built-in commands */
int isBuiltInCommand(char **parsedCommand)
{

	int i = 0, commandNum = 0;

	for (i = 0; i < NO_OF_BUILTIN_COMMANDS; i++)
	{

		if (strcmp(parsedCommand[0], list_of_commands[i]) == 0)
		{
			// printf("Hello, world! I am built in command\n");
			commandNum = i + 1;
			break;
		}
	}

	switch (commandNum)
	{
		case 0:
			break;
		case 1:
			jobsCommand();
			return 1;
		case 2:
			cdCommand(parsedCommand[1]);
			return 1;
		case 3:
			historyCommand();
			return 1;
		case 4:
			processPreviousCommand(parsedCommand);
			return 1;
		case 5:
			killCommand(parsedCommand[1]);
			return 1;
		case 6:
			exitCommand();
			return 1;
		default:
			break;
	}

	return 0;

}

/*Function to check for redirection commands */
int isInputOutput(char **parsed, char **str, int choice)
{

	char *s;
	if (!choice) s = "<";
	else s = ">";

	int flag = 0, i = 0;

	for (i = 0; parsed[i] != NULL; i++)
	{
		if (strcmp(parsed[i], s) == 0 && !flag && parsed[i + 1] != NULL)
		{

			flag = 1;
			*str = parsed[i + 1];
			return 1;
		}
	}

	return 0;

}

/*Function to execute program with arguements*/
void executeCommand(char **parsed)
{

	pid_t pid = fork();

	if (pid == -1)
	{
		printf("Forking child failed..");
		return;
	}
	else if (pid == 0)
	{

		char *input_file = NULL, *output_file = NULL;

		int I = isInputOutput(parsed, &input_file, 0);
		int O = isInputOutput(parsed, &output_file, 1);

		if (I)
		{
			int fd0 = open(input_file, O_RDONLY);
			dup2(fd0, STDIN_FILENO);
			close(fd0);
		}
		if (O)
		{

			int fd1 = open(output_file, O_RDWR | O_CREAT);
			dup2(fd1, STDOUT_FILENO);
			close(fd1);
		}

		execvp(parsed[0], parsed);
		fprintf(stderr, "Failed to exec %s\n", parsed[0]);
		return;
	}
	else
	{

		if (isBackgroundProcess(parsed))
		{
			addBackgroundProcess(parsed[0], pid);
			return;
		}
		else
		{

			waitpid(pid, NULL, 0);
			return;
		}
	}
}

/*Function to process command */
void processCommand(char *inputString)
{
	char *parsedCommand[MAX_COMMAND_SIZE];
	int executeFlag = 1;

	parseSpace(inputString, parsedCommand);
	
	if (parsedCommand[0]==NULL) return;

	if (isBuiltInCommand(parsedCommand))
	{

		executeFlag = 0;
	}

	if (executeFlag)
	{

		executeCommand(parsedCommand);
	}
}

