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
#include <stdio.h>             // printf()
#include <stdlib.h>            // malloc()
#include <sys/types.h>         // pid_t
#include <sys/wait.h>          // waitpid()
#include <signal.h>            // kill(), SIGTERM, SIGSTOP, SIGCONT
#include <readline/readline.h> // readline

/* ---------- Typedefs ---------- */

typedef struct node_t {
	pid_t pid;
	char* status;
	char* process;
	struct node_t* next;
} node_t;

/* ---------- Constants and global variables ---------- */

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

node_t* processListHead = NULL;

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
int isExistingProcess(pid_t pid) {
	node_t* iterator = processListHead;
	while (iterator != NULL) {
		if (iterator->pid == pid) {
			return TRUE;
		}
		iterator = iterator->next;
	}
	return FALSE;
}

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

/* ---------- Linked List functions ---------- */

/*
	pid: a process id (e.g. 123)
	adds a node with a given pid to the list of processes
*/
void addProcessToList(pid_t pid, char* status, char* process) {
	node_t* n = (node_t*)malloc(sizeof(node_t));
	n->pid = pid;
	n->process = process;
	n->status = status;
	n->next = NULL;

	if (processListHead == NULL) {
		processListHead = n;
	} else {
		node_t* iterator = processListHead;
		while (iterator->next != NULL) {
			iterator = iterator->next;
		}
		iterator->next = n;
	}
}

/*
	pid: a process id (e.g. 123)
	removes a node with a given pid from the list of processes
*/
void removeProcessFromList(pid_t pid) {
	if (!isExistingProcess(pid)) {
		return;
	}
	node_t* iterator1 = processListHead;
	node_t* iterator2 = NULL;

	while (iterator1 != NULL) {
		if (iterator1->pid == pid) {
			if (iterator1 == processListHead) {
				processListHead = processListHead->next;
			} else {
				iterator2->next = iterator1->next;
			}
			free(iterator1);
			return;
		}
		iterator2 = iterator1;
		iterator1 = iterator1->next;
	}
}

/*
	pid: a process id (e.g. 123)
	returns a node with a given pid to the list of processes
*/
node_t* getNode(pid_t pid) {
	node_t* iterator = processListHead;
	while (iterator != NULL) {
		if (iterator->pid == pid) {
			return iterator;
		}
		iterator = iterator->next;
	}
	return NULL;
}

/* ---------- Commands ---------- */

/*
	userInput: a pointer to an array of tokenized user input strings
	creates a new child process and executes userInput[1] command
*/
void bg(char** userInput) {
	pid_t pid = fork();
	if (pid == 0) {    // child
		char* command = userInput[1];
		execvp(command, &userInput[1]);
		printf("Error: failed to execute command %s\n", command);
		exit(1);
	} else if (pid > 0) {		// parent
		printf("Started background process %d\n", pid);
		addProcessToList(pid, "running", userInput[1]);
		sleep(1);
	} else {
		printf("Error: failed to fork\n");
	}
}

/*
	pid: a process id
	sends the TERM signal to a process pid to terminate it
*/
void bgkill(pid_t pid) {
	removeProcessFromList(pid);
	int error = kill(pid, SIGTERM);
	if (!error) {
		sleep(1);
	} else {
		printf("Error: failed to execute bgkill\n");
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
		printf("Error: failed to execute bgstop\n");
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
		printf("Error: failed to execute bgstart\n");
	}
}

/*
	displays a list of all programs currently executing in the background
*/
void bglist() {
	int count = 0;
	node_t* iterator = processListHead;

	while (iterator != NULL) {
		count++;
		printf("%d:\t %s\n", iterator->pid, iterator->process);
		iterator = iterator->next;
	}
	printf("Total background jobs:\t%d\n", count);
}

/*
	pid: a process id
	lists information relevant to a process pid
*/
void pstat(pid_t pid) {
	if (isExistingProcess(pid)) {
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
		case 0:
			if (userInput[1] == NULL) {
				printf("Error: invalid command to background\n");
				return;
			}
			bg(userInput);
			break;
		case 1: {
			if (userInput[1] == NULL || !isNumber(userInput[1])) {
				printf("Error: invalid pid>\n");
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
				printf("Error: invalid pid>\n");
				return;
			}
			pid_t pid = atoi(userInput[1]);
			bgstop(pid);
			break;
		}
		case 3: {
			if (userInput[1] == NULL || !isNumber(userInput[1])) {
				printf("Error: invalid pid\n");
				return;
			}
			pid_t pid = atoi(userInput[1]);
			bgstart(pid);
			break;
		}
		case 4:
			bglist();
			break;
		case 5: {
			if (userInput[1] == NULL || !isNumber(userInput[1])) {
				printf("Error: invalid pid\n");
				return;
			}
			pid_t pid = atoi(userInput[1]);
			pstat(pid);
			break;
		}
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
