/*
	University of Victoria
	Fall 2016
	CSC 360 Assignment 1
	Michael Reiter
	V00831568
*/

#include <unistd.h>     // fork(), execvp()
#include <stdio.h>      // printf(), scanf(), setbuf(), perror()
#include <stdlib.h>     // malloc()
#include <sys/types.h>  // pid_t 
#include <sys/wait.h>   // waitpid()
#include <signal.h>     // kill(), SIGTERM, SIGKILL, SIGSTOP, SIGCONT
#include <errno.h>      // errno

/* ---------- Constants ---------- */

#define TRUE 1
#define FALSE 0
#define COMMANDS_LENGTH 6
char* VALID_COMMANDS[] = {"bg", "bgkill", "bgstop", "bgstart", "bglist", "pstat"};

/* ---------- Helper functions ---------- */

/*
	s: a string
	returns the length of s
*/
int stringLength(char* s) {
	return sizeof(s) / sizeof(s[0]);
}

/*
	command: a command (e.g. "bg", "bgkill", etc.)
	returns TRUE if the command is valid, FALSE otherwise
*/
int isValidCommand(char* command) {
	for (int i = 0; i < COMMANDS_LENGTH; i++) {
		if (command == VALID_COMMANDS[i]) {
			return TRUE;
		}
	}
	return FALSE;
}

/*
	pid: a process id (e.g. 123)
	returns TRUE if the pid is valid, FALSE otherwise
*/
int isValidProcess(pid_t pid) {
	return TRUE;
}

/*
	rawCommand: raw user input from the command line
	returns an array with parsed command and pid
*/
char* parseCommand(char* rawCommand) {
	char* result[2];
	return result;
}

/*
	command: a command (e.g. "bg", "bgkill", etc.)
	returns an integer mapping to that command or -1 if the command is invalid
*/
int commandToInt(char* command) {
	for (int i = 0; i < COMMANDS_LENGTH; i++) {
		if (command == VALID_COMMANDS[i]) {
			return i;
		}
	}
	return -1;
}

/* ---------- Commands ---------- */

/*
	command: a command to be executed in the background
	args: an array of arguments for command
	returns a pid for the executed command
*/
void bg(char* command, char* args[]) {

}

/*
	pid: a process id
	sends the TERM signal to a process pid to terminate it
*/
void bgkill(pid_t pid) {
	
}

/*
	pid: a process id
	sends the STOP signal to a process pid to temporarily stop it
*/
void bgstop(pid_t pid) {
	
}

/*
	pid: a process id
	sends the CONT signal to a stopped process pid to restart it
*/
void bgstart(pid_t pid) {
	
}

/*
	displays a list of all programs currently executing in the background
*/
void bglist() {
	int processCount = 0;
	// for each process:
	// 	printf("%d:\t %s\n", process->pid, process->path);
	printf("Total background jobs:\t%d\n", processCount);
}

/*
	pid: a process id
	lists information relevant to a process pid
*/
void pstat(pid_t pid) {
	if (isValidProcess(pid) == TRUE) {
		char* comm = (char *)malloc(sizeof(char));
		char* state = (char *)malloc(sizeof(char));
		char* utime = (char *)malloc(sizeof(char));
		char* stime = (char *)malloc(sizeof(char));
		char* rss = (char *)malloc(sizeof(char));
		char* voluntary_ctxt_switches = (char *)malloc(sizeof(char));
		char* nonvoluntary_ctxt_switches = (char *)malloc(sizeof(char));

		printf("comm: %s\n", comm);
		printf("state: %s\n", state);
		printf("utime: %s\n", utime);
		printf("stime: %s\n", stime);
		printf("rss: %s\n", rss);
		printf("voluntary_ctxt_switches: %s\n", voluntary_ctxt_switches);
		printf("nonvoluntary_ctxt_switches: %s\n", nonvoluntary_ctxt_switches);
	} else {
		printf("Error: Process %d does not exist.\n", pid);
	}
}

/* ---------- Main ---------- */

int main() {
	// struct for process shown in Tutorial
	// typedef struct proc_t {
	// 	pid_t pid;
	// 	char *cmd;
	// } proc_t;

	pid_t pid;          // The ID of child process
	int status;
	char cont = 'y';   // y - continue to create new child process; n - exit.

	while (cont == 'y') {
		char* rawCommand = (char *)malloc(sizeof(char));
		char* parsedCommand = (char *)malloc(sizeof(char));

		while (TRUE) {
			printf("PMan: > ");
			scanf("%s", rawCommand);
			int* parsedCommand = parseCommand(rawCommand);
			if (isValidCommand(rawCommand)) {
				break;
			} else {
				printf("PMan: > %s: command not found\n", parsedCommand);
			}
		}

		printf("Valid command. Continuing...\n");
		
		pid = fork();

		switch (parsedCommand[0]) {
			case 0:
				bg();
				break;
			case 1:
				bgkill(pid);
				break;
			case 2:
				bgstop(pid);
				break;
			case 3:
				bgstart(pid);
				break;
			case 4:
				bglist();
				break;
			case 5:
				pstat(pid);
				break;
			default:
				printf("Default switch. Should never get here.\n");
				break;
		}


		// if (pid == 0) {
		// 	//Child process
		// 	printf("...\n");
		// 	char *argv_execvp[] = {"inf", tag, interval, NULL};
		// 	if (execvp("./inf", argv_execvp) < 0) {
		// 		perror("Error on execvp");
		// 	}
		// 	printf("Child finished.\n");  // This line won't work since exec() successfully runs and replaces the code of child process
		// 	exit(EXIT_FAILURE); 
		// } else if (pid > 0) {
		// 	//parent process
		// 	printf("\nParent has created a new child with tag = %s and interval = %s.\n", tag, interval);
		// 	printf("\nThe pid of the child process is: %d\n", pid);
			
		// 	int opts = WNOHANG | WUNTRACED | WCONTINUED;
		// 	int retVal;
	
		// 	retVal = waitpid(pid, &status, opts);
		// 	if (retVal == -1) { 
		// 		perror("Fail at waitpid"); 
		// 		exit(EXIT_FAILURE); 
		// 	}

		// 	// Macros below can be found by "$ man 2 waitpid"
		// 	if (WIFEXITED(status)) {
		// 		printf("Normal, status code=%d\n", WEXITSTATUS(status));  // Display the status code of child process
		// 	} else if (WIFSIGNALED(status)) {
		// 		printf("killed by signal %d\n", WTERMSIG(status));
		// 	} else if (WIFSTOPPED(status)) {
		// 		printf("stopped by signal %d\n", WSTOPSIG(status));
		// 	} else if (WIFCONTINUED(status)) {    
		// 		printf("continued\n");   
		// 	}

		// 	sleep(10);
		// 	kill(pid, SIGKILL);
		// } else {  // Fail to create new process
		// 	perror("\nFailed to create a new process.\n");
		// }
		// printf("Do you want to continue? (y/n)\n ");
		// setbuf(stdin, NULL);  // clear the buffer. You could try to remove this line to see the result
		// scanf("%c", &cont);
	} // end of while loop

	printf("Parent process stops.\n");
	return 0;
}
