/*
	University of Victoria
	Fall 2016
	CSC 360 Assignment 3 Part 3
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
	Reads a file from a disk image
*/
void copyFile(char* p, char* file, int fileSize, int* firstLogicalSector, int physicalAddress) {
	// int count = 0;
	// int entry = *firstLogicalSector;

	// // copy the first 512 bytes from the first logical address
	// int i;
	// for (i = 0; i < SECTOR_SIZE; i++) {
	// 	if (count == *fileSize) {
	// 		return;
	// 	}
	// 	destination[count] = source[physicalAddress + i];
	// 	count++;
	// }

	// // while the FAT table does not have an end of file value
	// // read in another 512 bytes from the physical address
	// while (getFatEntry(entry, source) != 0xFFF) {
	// 	// get the value from the next FAT entry
	// 	entry = getFatEntry(entry, source);
	// 	// do the necesary math to get the physical address from the logical address
	// 	physicalAddress = SECTOR_SIZE * (31 + entry);

	// 	for (i = 0; i < SECTOR_SIZE; i++) {
	// 		// check to see if we have read all bytes from the input file
	// 		if (count == *fileSize) {
	// 			break;
	// 		}
	// 		destination[count] = source[physicalAddress + i];
	// 		count++;
	// 	}
	// }
}

/* ---------- Main ---------- */

/*
	Copies an input file from an input file system image to the current directory
*/
int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("Error: use as follows diskget <file system image> <file>\n");
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
		printf("Error: failed to map disk image memory\n");
		exit(1);
	}

	int fileSize = getFileSize(argv[2], p + SECTOR_SIZE * 19);
	if (fileSize > 0) {
		// Open file and map memory
		int fd2 = open(argv[2], O_RDWR | O_CREAT, 0666);
		if (fd2 < 0) {
			printf("Error: failed to open file\n");
			exit(1);
		}

		int result = lseek(fd2, fileSize-1, SEEK_SET);
		if (result == -1) {
			close(fd2);
			printf("Error: failed to seek to end of file");
			exit(1);
		}

		result = write(fd2, "", 1);
		if (result != 1) {
			close(fd2);
			printf("Error: failed to write last byte");
			exit(1);
		}

		char* p2 = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
		if (p2 == MAP_FAILED) {
			printf("Error: failed to map file memory\n");
			exit(1);
		}

		// int i;
		// for (i = 0; i < fileSize; i++) {
		// 	printf("%c\n", p2[i]);
		// }

		// char* source;
		// char* destination;
		// int fileSize = getFileSize;
		// int* firstLogicalSector;
		// int physicalAddress;
		// copyFile(source, destination, fileSize, firstLogicalSector, physicalAddress);

		munmap(p2, fileSize);
		close(fd2);
	} else {
		printf("File not found.\n");
	}

	// Clean up
	munmap(p, buf.st_size);
	close(fd);

	return 0;
}
