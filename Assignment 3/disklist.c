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
	Prints the disk image directory listing
*/
void printDirectoryListing() {
	int i;
	for (i = 0; i < numFiles; i++) {
		printf("%s %10s %20s %s\n", fileType, fileSize, fileName, fileCreationDateTime);
	}
}

/* ---------- Main ---------- */

/*
	Displays information about an input file system
*/
int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Error: use as follows diskinfo <file system image>\n");
		exit(1);
	}

	printDirectoryListing();

	return 0;
}
