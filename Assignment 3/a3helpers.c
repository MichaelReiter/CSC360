#include <stdio.h>
#include <stdlib.h>
#include "a3helpers.h"

/*
	n: an entry in FAT table
	p: a pointer to the mapped memory
	Returns the value of the FAT table at n
*/
int readSector(int n, char* p) {
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
int getFreeSize(int diskSize, char* p) {
	int count = 0;

	int i;
	for (i = 2; i <= (diskSize / SECTOR_SIZE); i++) {
		if (readSector(i, p) == 0x000) {
			count++;
		}
	}

	return SECTOR_SIZE * count;
}
