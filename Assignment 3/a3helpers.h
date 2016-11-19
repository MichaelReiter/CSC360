/* ---------- Typedefs ---------- */

typedef struct something {

} something;

/* ---------- Constants and global variables ---------- */

#define TRUE 1
#define FALSE 0
#define SECTOR_SIZE 512

/* ---------- Helper functions ---------- */

int getSectorValue(int entry, char* p);
int getFreeSize(int diskSize, char* p);
