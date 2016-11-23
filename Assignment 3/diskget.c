/*
	University of Victoria
	Fall 2016
	CSC 360 Assignment 3 Part 3
	Michael Reiter
	V00831568
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "a3helpers.h"

/* ---------- Helper functions ---------- */

/*
	fileName: the file to check for
	p: a pointer to the mapped memory
	Returns TRUE if the file exists in the root directory p
*/
int fileExists(char* fileName, char* p) {
	while (p[0] != 0x00) {
		if ((p[11] & 0b00000010) == 0 && (p[11] & 0b00001000) == 0) {
			char* currentFileName = malloc(sizeof(char));
			char* currentFileExtension = malloc(sizeof(char));
			int i;
			for (i = 0; i < 8; i++) {
				if (p[i] == ' ') {
					break;
				}
				currentFileName[i] = p[i];
			}
			for (i = 0; i < 3; i++) {
				currentFileExtension[i] = p[i+8];
			}

			strcat(currentFileName, ".");
			strcat(currentFileName, currentFileExtension);

			if (strcmp(fileName, currentFileName) == 0) {
				return TRUE;
			}

			free(currentFileName);
			free(currentFileExtension);
		}
		p += 32;
	}
	return FALSE;
}

/*
	Reads a file from a disk image
*/
void copyFile(char* source, char* destination, int* fileSize, int* firstLogicalSector, int physicalAddress) {
	int count = 0;
	int entry = *firstLogicalSector;

	// copy the first 512 bytes from the first logical address
	int i;
	for (i = 0; i < SECTOR_SIZE; i++) {
		if (count == *fileSize) {
			return;
		}
		destination[count] = source[physicalAddress + i];
		count++;
	}

	// while the FAT table does not have an end of file value
	// read in another 512 bytes from the physical address
	while (getFatEntry(entry, source) != 0xFFF) {
		// get the value from the next FAT entry
		entry = getFatEntry(entry, source);
		// do the necesary math to get the physical address from the logical address
		physicalAddress = SECTOR_SIZE * (31 + entry);

		for (i = 0; i < SECTOR_SIZE; i++) {
			// check to see if we have read all bytes from the input file
			if (count == *fileSize) {
				break;
			}
			destination[count] = source[physicalAddress + i];
			count++;
		}
	}
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
		printf("Error: failed to map memory\n");
		exit(1);
	}

	if (!fileExists(argv[2], p + SECTOR_SIZE * 19)) {
		printf("File not found.\n");
	}

	// char* source;
	// char* destination;
	// int* fileSize;
	// int* firstLogicalSector;
	// int physicalAddress;
	// copyFile(source, destination, fileSize, firstLogicalSector, physicalAddress);

	// Clean up
	munmap(p, buf.st_size);
	close(fd);

	return 0;
}
