.phony all:
all: pman

# rsi: rsi.c
# 	gcc rsi.c -lreadline -lhistory -o rsi

pman: pman.c
	clear && gcc pman.c -o PMan && ./PMan

.PHONY clean:
clean:
	-rm -rf *.o *.exe
