.phony all:
all: mfs

mfs: mfs.c
	gcc -Wall -pthread mfs.c -o MFS

.PHONY clean:
clean:
	-rm -rf *.o *.exe
