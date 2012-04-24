# Local configuration. Change according to your system.
CC=gcc

all: fat-emul

fat-emul: main.c fatlib.o actions.h
	$(CC) -Wall -g main.c fatlib.o -o fat-emul

fatlib.o: fatlib.c fatlib.h constants.h
	$(CC) -Wall -g -c fatlib.c 

clean: 
	rm -f *.o fat-emul


.PHONY: all clean
