/*
	University of Victoria
	Fall 2016
	CSC 360 Assignment 3 Part 2
	Michael Reiter
	V00831568
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "a3helpers.h"

/* ---------- Helper functions ---------- */

/*
	listing: an array of entities on disk to print
	listingLength: the length of listing array
	Prints the disk image directory listing
*/
void printDirectoryListing(disk_file_t* listing, int listingLength) {
	// int i;
	// for (i = 0; i < listingLength; i++) {
	// 	printf("%s %10d %20s %s\n", listing[i]->fileType, listing[i]->fileSize, listing[i]->fileName, listing[i]->fileCreationDateTime);
	// }
}

/* ---------- Main ---------- */

/*
	Displays information about files in an input file system image
*/
int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Error: use as follows disklist <file system image>\n");
		exit(1);
	}

	disk_file_t* listing = malloc(listingLength * sizeof(disk_file_t));
	printDirectoryListing(listing, listingLength);

	return 0;
}
