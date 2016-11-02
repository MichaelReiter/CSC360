.phony all:
all: mfs

mfs: mfs.c
	clear && gcc -Wall mfs.c -o MFS && ./MFS flows.txt

.PHONY clean:
clean:
	-rm -rf *.o *.exe
