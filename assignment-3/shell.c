//Lalit Mohanani 21CS02006

//   Install Readline: sudo apt-get install libreadline-dev
//   Compile using gcc: gcc -o shell shell.c -lreadline
//   Execute: ./shell

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<fcntl.h>

//Definitions 
#define MAX_COMMAND_SZ 1000 
#define MAX_LIST_SZ 100 
#define HISTORY_SZ 10
#define MAX_BACKGROUND_JOBS 100

//Circlular QUEUE FOR STORING HISTORY 
int rear = -1, front = -1;
int current_top = 1;
char commands[HISTORY_SZ][MAX_COMMAND_SZ];

int isFull(){
	return ((front == 0 && rear == HISTORY_SZ - 1) || (rear == (front - 1) % (HISTORY_SZ - 1)));
}

void deQueue(){
	strcpy(commands[front], "");
	if (front == rear){
		front = -1;
		rear = -1;
	}
	else if (front == HISTORY_SZ - 1){
		front = 0;
	}
	else{
		front++;
	}
}
void enQueue(char *command){

	if (front == -1){
		front = rear = 0;
	}
	else if (rear == HISTORY_SZ - 1 && front != 0){
		rear = 0;
	}
	else{
		rear++;
	}

	strcpy(commands[rear], command);
}

void addCommand(char *command){

	if (isFull()){

		deQueue();
		current_top++;
	}

	enQueue(command);

}

void historyCmd(){
	int c = current_top;
	if (front == -1){
		return;
	}
	printf("\n");
	if (rear >= front){
		for (int i = front; i <= rear; i++)
			printf("%d: %s\n", c++, commands[i]);
	}
	else{
		for (int i = front; i < HISTORY_SZ; i++)
			printf("%d: %s\n", c++, commands[i]);

		for (int i = 0; i <= rear; i++)
			printf("%d: %s\n", c++, commands[i]);
	}
	// HISTORY_STATE *myhist = history_get_history_state ();

    // /* retrieve the history list */
    // HIST_ENTRY **mylist = history_list ();

    // // printf ("\nsession history for %s@%s\n\n", p, host);
    // for (int i = 0; i < myhist->length; i++) { /* output history list */
    //     printf (" %8s  %s\n", mylist[i]->line, mylist[i]->timestamp);
    //     free_history_entry (mylist[i]);     /* free allocated entries */
    // }
    // putchar ('\n');

    // free (myhist);  /* free HIST_ENTRY list */
    // free (mylist);
}

//for backgrouond jobs USE & at the end for running task in background 
//for knowing details use help command 
int current_background_process = 0;
int last_index = 0;
pid_t background[MAX_BACKGROUND_JOBS];
char background_jobs[MAX_BACKGROUND_JOBS][MAX_COMMAND_SZ];

int isBackgroundProcess(char **parsedCommand){
	int i = 0;
	for (; parsedCommand[i + 1] != NULL; i++);

	char *lastArguement = parsedCommand[i];

	return (strcmp(lastArguement, "&") == 0);

}
//adds process in background
void addBackgroundProcess(char *command, pid_t pid){

	strcpy(background_jobs[last_index], command);
	background[last_index++] = pid;
	current_background_process++;

}
//removing background process
int removeBackgroundProcess(pid_t pid){

	int i, found = 0;
	for (i = 0; i < last_index; i++){

		if (background[i] == pid){

			found = 1;

			int result = kill(pid, SIGKILL);
			if (result == 0){
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

// clear function 
#define clear() printf("\033[H\033[J")

int n = 0;

int takeInput(char* str){
	char* buf;
	// prompt();
	
	buf = readline(NULL);
	if (strlen(buf) != 0) {
		add_history(buf);
		addCommand(buf);
		strcpy(str, buf);
		return 0;
	} else {
		return 1;
	}
}

void prompt(){
	char cwd[1024];
	char* username = getenv("USER");
	printf("%s@", username);
	getcwd(cwd, sizeof(cwd));
	printf("%s$ ", cwd);
}

void execArgs(char** parsed,char* str){

	int input = STDIN_FILENO;
	int output = STDOUT_FILENO;
	int id = -1;
	
	for(int i =0; i<n;i++){
		if(strcmp(parsed[i],"<")==0){
			if(id==-1) id = i;
			input = open(parsed[i+1],O_RDONLY);
			if(input==-1) break;
		}
		if(strcmp(parsed[i],">")==0){
			if(id==-1) id = i;
			output = open(parsed[i+1],O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(output ==-1) break;
		}
	}

	if(input==-1 || output==-1){
		perror("input/output file error");
	}
	if(id!= -1) parsed[id] = NULL;

	// Forking a child
	pid_t pid = fork();

	if (pid == -1) {
		printf("\nFailed forking child..");
		return;
	} else if (pid == 0) {
		if(input != STDIN_FILENO){
			dup2(input,STDIN_FILENO);
			close(input);
		}
		if(output != STDOUT_FILENO){
			dup2(output,STDOUT_FILENO);
			close(output);
		}
        execvp(parsed[0], parsed);
		printf("\nCould not execute str..");
		exit(1);
	} else {

		if (isBackgroundProcess(parsed)){

			addBackgroundProcess(parsed[0], pid);

			return;
		}
		else{

			waitpid(pid, NULL, 0);
			return;
		}
	}
}

void Help(){
	puts("\n***WELCOME SHELL HELP***"
		"\nList of Commands supported:"
		"\n>cd - change directory"
		"\n>history - print history"
		"\n>exit - exits terminal"
		"\n>running - prints all running process"
		"\n>for running function in background u can use & at the end"
		"\n>for example :- './bg &'"
		"\n>kill - kill process with given pid syntax 'kill -pid'"
		"\n>all other general commands available in UNIX shell"
		"\n>improper space handling");

	return;
}

void runningCmd(){

	int i;
	for (i = 0; i < last_index; i++){

		if (background[i] != -1){
			printf("%d: %s\n", background[i], background_jobs[i]);
		}
	}
	printf("\n");

}

void killCmd(char *parsedCommand){

	if (parsedCommand == NULL || parsedCommand[0] != '-'){
		printf("Inalid syntax, syntax is kill -PID\n");
		return;
	}

	int pid = atoi(parsedCommand + 1);

	if (!removeBackgroundProcess(pid)){

		printf("No process with the given id exists\n");
		return;
	}
}

// Function to execute builtin commands
int isBuiltInCommand(char** parsed){
	int noOfOwnCmd = 7, i, sh = 0;
	char* OwnCommands[noOfOwnCmd];
	char* username;

	OwnCommands[0] = "exit";
	OwnCommands[1] = "cd";
	OwnCommands[2] = "help";
	OwnCommands[3] = "history";
	OwnCommands[4] = "hello";
	OwnCommands[5] = "jobs";
	OwnCommands[6] = "kill";
 
	for (i = 0; i < noOfOwnCmd; i++) {
		if (strcmp(parsed[0], OwnCommands[i]) == 0) {
			sh = i + 1;
			break;
		}
	}

	switch (sh) {
	case 1:
		printf("\nTerminal Exited\n");
		exit(0);
	case 2:
		chdir(parsed[1]);
		return 1;
	case 3:
		Help();
		return 1;
	case 4:
		historyCmd();
		return 1;
	case 5:
		username = getenv("USER");
		printf("\nHello %s" "\nUse help to know more..\n",username);
		return 1;
	case 6:
		runningCmd();
		return 1;
	case 7:
		killCmd(parsed[1]);
		return 1;
	default:
		break;
	}

	return 0;
}

void parseSpace(char* str, char** parsed){
	int i;

	for (i = 0; i < MAX_LIST_SZ; i++) {
		parsed[i] = strsep(&str, " ");
		// if(parsed[i])
		if (parsed[i] == NULL){
			break;
        }
		if (strlen(parsed[i]) == 0){
			i--;
        }
	}

	n = i;
	// input_output(parsed);
	// printf("%d",i);
}

int processCommand(char* str, char** parsed){
    
	parseSpace(str, parsed);

	if (isBuiltInCommand(parsed))
		return 0;
	else
		return 1;
}


//main
int main(){
	char inputString[MAX_COMMAND_SZ], *parsedArgs[MAX_LIST_SZ];
	int execFlag = 0;
	printf("Commands supported are {exit,cd,help,history,jobs,kill} and all general UNIX shell cmds \n");
	while (1) {
		// print shell line
		prompt();
		// take input
		if (takeInput(inputString))
			continue;
		// process
		execFlag = processCommand(inputString,parsedArgs);

		if (execFlag == 1)
			execArgs(parsedArgs,inputString);
	}
	return 0;
}
