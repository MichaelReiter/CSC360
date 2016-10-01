.phony all:
all: pman

pman: pman.c
	clear && gcc -Wall pman.c -lreadline -o PMan && ./PMan

.PHONY clean:
clean:
	-rm -rf *.o *.exe
