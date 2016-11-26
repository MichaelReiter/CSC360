/*
	University of Victoria
	Fall 2016
	CSC 360 Assignment 3 Part 4
	Michael Reiter
	V00831568
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "a3helpers.h"

/* ---------- Helper functions ---------- */

/*
	fileName: the name of the file in the updated entry
	fileSize: the size in bytes of the file to copy
	p: a pointer to the mapped memory
	Adds an entry for fileName in the disk image root directory
*/
void updateRootDirectory(char* fileName, int fileSize, char* p) {
	// Find free root dir address
	p += SECTOR_SIZE * 19;
	while (p[0] != 0x00) {
		p += 32;
	}

	// Set filename and extension
	int i;
	int done = -1;
	for (i = 0; i < 8; i++) {
		char character = fileName[i];
		if (character == '.') {
			done = i;
		}
		p[i] = (done == -1) ? character : ' ';
	}
	for (i = 0; i < 3; i++) {
		p[i+8] = fileName[i+done+1];
	}

	// Set attributes
	p[11] = 0x00;

	// Set create date/time
	time_t t = time(NULL);
	struct tm *now = localtime(&t);
	int year = now->tm_year + 1900;
	int month = (now->tm_mon + 1);
	int day = now->tm_mday;
	int hour = now->tm_hour;
	int minute = now->tm_min;
	p[14] = 0;
	p[15] = 0;
	p[16] = 0;
	p[17] = 0;
	p[17] |= (year - 1980) << 1;
	p[17] |= (month - ((p[16] & 0b11100000) >> 5)) >> 3;
	p[16] |= (month - (((p[17] & 0b00000001)) << 3)) << 5;
	p[16] |= (day & 0b00011111);
	p[15] |= (hour << 3) & 0b11111000;
	p[15] |= (minute - ((p[14] & 0b11100000) >> 5)) >> 3;
	p[14] |= (minute - ((p[15] & 0b00000111) << 3)) << 5;

	// Set fileSize
	p[28] = (fileSize & 0x000000FF);
	p[29] = (fileSize & 0x0000FF00) >> 8;
	p[30] = (fileSize & 0x00FF0000) >> 16;
	p[31] = (fileSize & 0xFF000000) >> 24;
}

/*
	p: a pointer to the mapped memory
	Returns the next index in the FAT containing 0x00 (free)
*/
int getNextFreeFatIndex(char* p) {
	p += SECTOR_SIZE;

	int n = 0;
	while (getFatEntry(n, p) != 0x000) {
		n = getFatEntry(n, p);
	}

	return n;
}

/*
	n: the FAT entry to update
	value: the value to set FAT at index n
	p: a pointer to the mapped memory
	Sets FAT at entry n to value
*/
void setFatEntry(int n, int value, char* p) {
	p += SECTOR_SIZE;

	if ((n % 2) == 0) {
		p[SECTOR_SIZE + ((3*n) / 2) + 1] = (value >> 8) & 0x0F;
		p[SECTOR_SIZE + ((3*n) / 2)] = value & 0xFF;
	} else {
		p[SECTOR_SIZE + (int)((3*n) / 2)] = (value << 4) & 0xF0;
		p[SECTOR_SIZE + (int)((3*n) / 2) + 1] = (value >> 4) & 0xFF;
	}
}

/*
	p: a pointer to the mapped memory
	p2: a pointer to the mapped file to write to
	fileName: the file to copy from the disk image
	fileSize: the size in bytes of the file to copy
	Writes a file from a disk image to the local directory
*/
void copyFile(char* p, char* p2, char* fileName, int fileSize) {
	updateRootDirectory(fileName, fileSize, p);

	int bytesRemaining = fileSize;
	int current = getNextFreeFatIndex(p);

	while (bytesRemaining != 0) {
		int physicalAddress = SECTOR_SIZE * (31 + current);
		
		int i;
		for (i = 0; i < SECTOR_SIZE; i++) {
			if (bytesRemaining == 0) {
				setFatEntry(current, 0xFFF, p);
				return;
			}
			p[i + physicalAddress] = p2[fileSize - bytesRemaining];
			bytesRemaining--;
		}
		int next = getNextFreeFatIndex(p);
		setFatEntry(current, next, p);
		current = next;
	}
}

/* ---------- Main ---------- */

/*
	Copies an input file from the current directory to an input file system image
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
	char* p = mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		printf("Error: failed to map disk image memory\n");
		close(fd);
		exit(1);
	}

	// Open file and map memory
	int fd2 = open(argv[2], O_RDWR);
	if (fd2 < 0) {
		printf("File not found.\n");
		close(fd);
		exit(1);
	}
	struct stat buf2;
	fstat(fd2, &buf2);
	int fileSize = buf2.st_size;
	char* p2 = mmap(NULL, fileSize, PROT_READ, MAP_SHARED, fd2, 0);
	if (p2 == MAP_FAILED) {
		printf("Error: failed to map file memory\n");
		exit(1);
	}

	// Copy file from local directory to disk image if there's space
	int totalDiskSize = getTotalDiskSize(p);
	int freeDiskSize = getFreeDiskSize(totalDiskSize, p);
	if (freeDiskSize >= fileSize) {
		copyFile(p, p2, argv[2], fileSize);
	} else {
		printf("Not enough free space in the disk image.\n");
	}

	// Clean up
	munmap(p, buf.st_size);
	munmap(p2, fileSize);
	close(fd);
	close(fd2);

	return 0;
}
