#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "a3helpers.h"

/*
	n: an entry in FAT table
	p: a pointer to the mapped memory
	Returns the value of the FAT table at n
*/
int getFatEntry(int n, char* p) {
	int result;
	int firstByte;
	int secondByte;

	if ((n % 2) == 0) {
		firstByte = p[SECTOR_SIZE + ((3*n) / 2) + 1] & 0x0F;
		secondByte = p[SECTOR_SIZE + ((3*n) / 2)] & 0xFF;
		result = (firstByte << 8) + secondByte;
	} else {
		firstByte = p[SECTOR_SIZE + (int)((3*n) / 2)] & 0xF0;
		secondByte = p[SECTOR_SIZE + (int)((3*n) / 2) + 1] & 0xFF;
		result = (firstByte >> 4) + (secondByte << 4);
	}

	return result;
}

/*
	diskSize: the total amount of bytes on disk
	p: a pointer to the mapped memory
	Returns the amount of free space on disk
*/
int getFreeDiskSize(int diskSize, char* p) {
	int count = 0;

	int i;
	for (i = 2; i <= (diskSize / SECTOR_SIZE); i++) {
		if (getFatEntry(i, p) == 0x000) {
			count++;
		}
	}

	return SECTOR_SIZE * count;
}

/*
	fileName: the file to check for
	p: a pointer to the mapped memory
	Returns file size in bytes if the file exists in the root directory of p, -1 otherwise
*/
int getFileSize(char* fileName, char* p) {
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
				return (p[28]) + (p[29] << 8) + (p[30] << 16) + (p[31] << 24);
			}

			free(currentFileName);
			free(currentFileExtension);
		}
		p += 32;
	}
	return -1;
}
