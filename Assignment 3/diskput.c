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
	Writes a file to an input file system image
*/
void writeFile() {
	// if (insufficient space) {
	// 	printf("Not enough free space in the disk image.\n");
	// }
}

/* ---------- Main ---------- */

/*
	Writes an input file from the current directory to an input file system image
*/
int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("Error: use as follows diskinfo <file system image> <file>\n");
		exit(1);
	}

	writeFile();

	return 0;
}
