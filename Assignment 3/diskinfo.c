/*
	University of Victoria
	Fall 2016
	CSC 360 Assignment 3 Part 1
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
	osname: a char array to read the os name into
	p: a pointer to the memory
	Reads the OS name read from a disk image
*/
void getOsName(char* osname, char* p) {
	int i;
	for (i = 0; i < 8; i++) {
		osname[i] = p[3+i];
	}
}

/*
	p: a pointer to the memory
	Returns the number of sectors per FAT read from p
*/
int getSectorsPerFat(char* p) {
	// read 2 bytes starting at byte 22
	return 0;
}

/*
	diskLabel: a character pointer to store the volume label
	p: a pointer to the memory
	Sets diskLabel to the volume label
*/
void getDiskLabel(char* diskLabel, char* p) {
	// *diskLabel = "test";
}

/*
	p: a pointer to the memory
	Returns total disk size
*/
int getTotalSize(char* p) {
	int* temp1 = malloc(sizeof(int));
	int* temp2 = malloc(sizeof(int));
	int result;

	*temp1 = p[19];
	*temp2 = p[20];

	// Use a for loop here if necesary
	result = *temp1 + ((*temp2) << 8);
	// result = (p[19] << 0) | (p[20] << 8);
	free(temp1);
	free(temp2);
	return result;
}

/*
	p: a pointer to the memory
	Returns the number of files in the root directory
*/
int getNumberOfRootFiles(char* p) {

	return 0;
}

/*
	p: a pointer to the memory
	Returns the number of copies of the FAT table
*/
int getNumberOfFatCopies(char* p) {

	return 0;
}

/*
	Prints file system image information
*/
void printInfo(char* osName, char* diskLabel, int diskSize, int freeSize, int numberOfRootFiles, int numberOfFatCopies, int sectorsPerFat) {
	printf("OS Name:\t\t%s\n", osName);
	printf("Label of the disk:\t%s\n", diskLabel);
	printf("Total size of the disk:\t%d\n", diskSize);
	printf("Free size of the disk:\t%d\n\n", freeSize); // number of free sectors times size of sector
	printf("==============\n");
	printf("The number of files in the root directory (not including subdirectories):\n");
	printf("%d\n", numberOfRootFiles);
	printf("==============\n");
	printf("Number of FAT copies:\t%d\n", numberOfFatCopies);
	printf("Sectors per FAT:\t%d\n", sectorsPerFat);
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

	char* osName = malloc(sizeof(char));
	getOsName(osName, p);
	char* diskLabel = malloc(sizeof(char));
	getDiskLabel(diskLabel, p);
	int diskSize = getTotalSize(p);
	int freeSize = getFreeSize(diskSize, p);
	int numberOfRootFiles = getNumberOfRootFiles(p);
	int numberOfFatCopies = getNumberOfFatCopies(p);
	int sectorsPerFat = getSectorsPerFat(p);
	printInfo(osName, diskLabel, diskSize, freeSize, numberOfRootFiles, numberOfFatCopies, sectorsPerFat);
	
	// Clean up
	close(fd);
	free(osName);
	free(diskLabel);

	return 0;
}
