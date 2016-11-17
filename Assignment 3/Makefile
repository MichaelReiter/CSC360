.phony all:
all: sfs

sfs: sfs.c
	clear && gcc -Wall -pthread sfs.c -o diskinfo && ./diskinfo

.PHONY clean:
clean:
	-rm -rf *.o *.exe
