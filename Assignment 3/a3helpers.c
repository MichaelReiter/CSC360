#include <stdio.h>
#include <stdlib.h>
#include "a3helpers.h"

/*
  n: an entry in FAT table
  p: a pointer to the mapped memory
  Returns the value of the FAT table at n
*/
int getSectorValue(int n, char* p) {
  int result;
  int base = 0x200;
  int* resultBuffer = malloc(2*sizeof(int));

  if ((n % 2) == 0) {
    resultBuffer[0] = p[1 + base + ((3*n) / 2)] & 0x0F;
    resultBuffer[1] = p[base + ((3*n) / 2)] & 0xFF;
    result = (resultBuffer[0] << 8) + resultBuffer[1];
  } else {
    resultBuffer[0] = p[base + (int)((3*n) / 2)] & 0xF0;
    resultBuffer[1] = p[1 + base + (int)((3*n) / 2)] & 0xFF;
    result = (resultBuffer[0] >> 4) + (resultBuffer[1] << 4);
  }

  free(resultBuffer);
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
    if (getSectorValue(i, p) == 0x000) {
      count++;
    }
  }

  return SECTOR_SIZE * count;
}
