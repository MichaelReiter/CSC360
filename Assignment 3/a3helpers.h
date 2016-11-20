/* ---------- Typedefs ---------- */

typedef struct disk_file_t {
  char fileType;
  int fileSize;
  char* fileName;
  char* fileCreationDateTime;
} disk_file_t;

/* ---------- Constants and global variables ---------- */

#define TRUE 1
#define FALSE 0
#define SECTOR_SIZE 512

/* ---------- Helper functions ---------- */

int getSectorValue(int entry, char* p);
int getFreeSize(int diskSize, char* p);
