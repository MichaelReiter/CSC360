/*
	University of Victoria
	Fall 2016
	CSC 360 Assignment 3 Part 2
	Michael Reiter
	V00831568
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "a3helpers.h"

/* ---------- Helper functions ---------- */

/*
	n: a 32 bit unsigned integer to convert
	Returns an integer converted to either big or little endian
*/
int endianSwap(int n) {
	return ((n >> 24) & 0xff)    |
         ((n << 8) & 0xff0000) |
         ((n >> 8) & 0xff00)   |
         ((n << 24) & 0xff000000);
}

/*
	p: a pointer to the mapped memory
	Prints the disk image directory listing
*/
void printDirectoryListing(char* p) {
	while (p[0] != 0x00) {

		char fileType;
		if (p[11] & 0b00010000) {
			fileType = 'D';
		} else {
			fileType = 'F';
		}

		int fileSize = (p[28]) + (p[29] << 8) + (p[30] << 16) + (p[31] << 24);

		char* fileName = malloc(sizeof(char));
		int i;
		for (i = 0; i < 8; i++) {
			if (p[i] == ' ') {
				break;
			}
			fileName[i] = p[i];
		}

		char* fileExtension = malloc(sizeof(char));
		for (i = 0; i < 3; i++) {
			fileExtension[i] = p[i+8];
		}

		int year = (p[16] & 0b00011111) + 1980;
		int month = ((p[16] & 0b11100000) >> 5) + ((p[17] & 0b00000011));
		int day = (p[16] & 0x0);
		int hour = (p[14] & 0x0);
		int minute = (p[14] & 0x0);
		if ((p[11] & 0b00000010) == 0 && (p[11] & 0b00001000) == 0) {
			printf("%c %10d %20s.%s %d-%d-%d %02d:%02d\n", fileType, fileSize, fileName, fileExtension, year, month, day, hour, minute);
		}

		free(fileName);
		free(fileExtension);

		p += 32;
	}
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

	// List all entries in disk image root directory
	printDirectoryListing(p + SECTOR_SIZE * 19);

	// Clean up
	munmap(p, buf.st_size);
	close(fd);

	return 0;
}
