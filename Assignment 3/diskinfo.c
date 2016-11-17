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
	Prints file system image information
*/
void printInfo() {
	// printf("OS Name:\t%s\n", osName);
	// printf("Label of the disk:\t%s\n", diskLabel);
	// printf("Total size of the disk:\t%d\n", diskSize);
	// printf("Free size of the disk:\t%d\n", freeSize);
	// printf("==============\n");
	// printf("The number of files in the root directory (not including subdirectories):\n");
	// printf("%d\n", numberOfRootFiles);
	// printf("==============\n");
	// printf("Number of FAT copies:\t%d\n", numberOfFatCopies);
	// printf("Sectors per FAT:\t%d\n", sectorsPerFat);
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

	printInfo();

	return 0;
}
