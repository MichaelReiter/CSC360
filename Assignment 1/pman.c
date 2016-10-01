/*
	University of Victoria
	Fall 2016
	CSC 360 Assignment 1
	Michael Reiter
	V00831568
*/

#include <unistd.h>            // fork(), execvp()
#include <string.h>            // strcmp()
#include <ctype.h>             // isdigit()
#include <stdio.h>             // printf(), scanf(), setbuf(), perror()
#include <stdlib.h>            // malloc()
#include <sys/types.h>         // pid_t
#include <sys/wait.h>          // waitpid()
#include <signal.h>            // kill(), SIGTERM, SIGKILL, SIGSTOP, SIGCONT
#include <readline/readline.h> // readline
// #include <readline/history.h>

/* ---------- Constants ---------- */

#define TRUE 1
#define FALSE 0
#define MAX_INPUT_SIZE 128
#define COMMANDS_LENGTH 6
char* VALID_COMMANDS[] = {
	"bg",
	"bgkill",
	"bgstop",
	"bgstart",
	"bglist",
	"pstat"
};

/* ---------- Typedefs ---------- */

typedef enum status_t {
  running,
  stopped
} status_t;

typedef struct node {
	pid_t pid;
	char* arg;
	status_t status;
	struct node* next;
} node;

/* ---------- General Helper functions ---------- */

/*
	s: a string
	returns TRUE if the string is a valid integer, FALSE otherwise
*/
int isNumber(char* s) {
	for (int i = 0; i < strlen(s); i++) {
		if (!isdigit(s[i])) {
			return FALSE;
		}
	}
	return TRUE;
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
	returns an array containing the parsed command and its arguments
*/
// char** parseCommand(char* rawCommand) {
// 	char* result[2];
// 	return result;
// }

/*
	command: a command (e.g. "bg", "bgkill", etc.)
	returns an integer mapping to that command or -1 if the command is invalid
*/
int commandToInt(char* command) {
	for (int i = 0; i < COMMANDS_LENGTH; i++) {
		if (strcmp(command, VALID_COMMANDS[i]) == 0) {
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
	// use fork() and execvp()
}

/*
	pid: a process id
	sends the TERM signal to a process pid to terminate it
*/
void bgkill(pid_t pid) {
	int error = kill(pid, SIGTERM);
	if (!error) {
		sleep(1);
	} else {
		printf("bgkill error\n");
	}
}

/*
	pid: a process id
	sends the STOP signal to a process pid to temporarily stop it
*/
void bgstop(pid_t pid) {
	int error = kill(pid, SIGSTOP);
	if (!error) {
		sleep(1);
	} else {
		printf("bgstop error\n");
	}
}

/*
	pid: a process id
	sends the CONT signal to a stopped process pid to restart it
*/
void bgstart(pid_t pid) {
	int error = kill(pid, SIGCONT);
	if (!error) {
		sleep(1);
	} else {
		printf("bgstart error\n");
	}
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
	if (isValidProcess(pid)) {
		char* comm = NULL;
		char* state = NULL;
		char* utime = NULL;
		char* stime = NULL;
		char* rss = NULL;
		char* voluntary_ctxt_switches = NULL;
		char* nonvoluntary_ctxt_switches = NULL;

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

/* ---------- Main helper functions ---------- */

/*
	input: a pointer to an array of strings to contain the tokenized user input
	tokenizes user input and stores it in input
	returns TRUE on success, FALSE on error
*/
int getUserInput(char** userInput) {
	char* rawInput = readline("PMan: > ");
	if (strcmp(rawInput, "") == 0) {
		return FALSE;
	}
	char* token = strtok(rawInput, " ");
	for (int i = 0; i < MAX_INPUT_SIZE; i++) {
		userInput[i] = token;
		token = strtok(NULL, " ");
	}
	return TRUE;
}

/*
	input: a pointer to an array of strings containing the tokenized user input
	executes commands from input
*/
void executeUserInput(char** userInput) {
	int commandInt = commandToInt(userInput[0]);

	switch (commandInt) {
		case 0: {
			// char* process = "cat";
			// char* arguments[] = {"foo.txt"};
			// bg(process, arguments);
			break;
		}
		case 1: {
			if (userInput[1] == NULL || !isNumber(userInput[1])) {
				printf("usage: bgkill <pid>\n");
				return;
			}
			pid_t pid = atoi(userInput[1]);
			if (pid != 0) {
				bgkill(pid);
			}
			break;
		}
		case 2: {
			if (userInput[1] == NULL || !isNumber(userInput[1])) {
				printf("usage: bgstop <pid>\n");
				return;
			}
			pid_t pid = atoi(userInput[1]);
			bgstop(pid);
			break;
		}
		case 3: {
			if (userInput[1] == NULL || !isNumber(userInput[1])) {
				printf("usage: bgstart <pid>\n");
				return;
			}
			pid_t pid = atoi(userInput[1]);
			bgstart(pid);
			break;
		}
		case 4:
			bglist();
			break;
		case 5:
			// pstat(pid);
			break;
		default:
			printf("PMan: > %s:\tcommand not found\n", userInput[0]);
			break;
	}
}

/* ---------- Main ---------- */

int main() {
	while (TRUE) {
		char* userInput[MAX_INPUT_SIZE];
		int success = getUserInput(userInput);
		if (success) {
			executeUserInput(userInput);
		}
	}

	return 0;
}
