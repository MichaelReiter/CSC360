.phony all:
all: sfs

sfs:
	gcc -Wall a3helpers.c diskinfo.c -o diskinfo
	gcc -Wall a3helpers.c disklist.c -o disklist
	gcc -Wall a3helpers.c diskget.c -o diskget
	gcc -Wall a3helpers.c diskput.c -o diskput

.PHONY clean:
clean:
	-rm -rf *.o *.exe
