#include <stdio.h>
#include <stdlib.h>
#include "a3helpers.h"

/*
  entry: int representing entry in FAT table
  p: a char* mapped to memory
  Returns an int representing the value of the FAT table ???
*/
int getSectorValue(int entry, char* p) {
  // base address of FAT table
  int base = 0x200;
  int* entryBuffer = malloc(2 * sizeof(int));

  if ((entry % 2) == 0) {
    // low 4 bits
    entryBuffer[0] = p[base + 1 + ((3 * entry) / 2)] & 0x0F;
    // all 8 bits
    entryBuffer[1] = p[base + ((3 * entry) / 2)] & 0xFF;
    return ((entryBuffer[0] << 8) + entryBuffer[1]);
  } else {
    // high 4 bits
    entryBuffer[0] = p[base + ((3 * entry) / 2)] & 0xF0;
    // all 8 bits
    entryBuffer[1] = p[base + ((3 * entry) / 2)] & 0xFF;
    return (entryBuffer[0] >> 4) + (entryBuffer[1] << 4);
  }

  free(entryBuffer);
}

/*
  diskSize: the total amount of bytes on disk
  p: a pointer to the memory
  Returns the amount of free space on disk
*/
int getFreeSize(int diskSize, char* p) {
  int count = 0;

  int i;
  for (i = 2; i <= (diskSize / SECTOR_SIZE); i++) {
    // if sector is unused
    if (getSectorValue(i, p) == 0x000) {
      count++;
    }
  }

  return SECTOR_SIZE * count;
}
