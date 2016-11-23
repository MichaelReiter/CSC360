/*
	University of Victoria
	Fall 2016
	CSC 360 Assignment 3 Part 4
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
	Writes a file to an input file system image
*/
void writeFile() {
	// if (file not found) {
	// 	printf("File not found.\n");
	// }

	// if (insufficient space) {
	// 	printf("Not enough free space in the disk image.\n");
	// }

	// Copy each sector and update FAT table
}

/* ---------- Main ---------- */

/*
	Writes an input file from the current directory to an input file system image
*/
int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("Error: use as follows diskput <file system image> <file>\n");
		exit(1);
	}

	// Open disk image and map memory
	int fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		printf("Error: failed to read disk image\n");
		exit(1);
	}
	struct stat buf;
	fstat(fd, &buf);
	char* p = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		printf("Error: failed to map memory\n");
		exit(1);
	}

	// Write to disk image
	writeFile();

	// Clean up
	munmap(p, buf.st_size);
	close(fd);

	return 0;
}
