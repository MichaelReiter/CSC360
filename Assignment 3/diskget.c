/*
	University of Victoria
	Fall 2016
	CSC 360 Assignment 3
	Michael Reiter
	V00831568
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Typedefs ---------- */

typedef struct something {

} something;

/* ---------- Constants and global variables ---------- */

#define TRUE 1
#define FALSE 0

/* ---------- Helper functions ---------- */

/*
	Reads a file from a file system image
*/
void copyFile() {
	// if (file not found) {
	// 	printf("File not found.\n");
	// }
}

/* ---------- Main ---------- */

/*
	Copies an input file from an input file system image to the current directory
*/
int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("Error: use as follows diskinfo <file system image> <file>\n");
		exit(1);
	}

	copyFile();

	return 0;
}
