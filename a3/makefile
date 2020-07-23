
CC=gcc

# The line with -DDEBUG can be used for development. When
# building your code for evaluation, however, the line *without*
# the -DDEBUG will be used.
#

CFLAGS=-c -Wall -g -DDEBUG -std=c99 -O0
# CFLAGS=-c -Wall -g -std=c99 -O0


all: senjify3

senjify3: senjify3.o list.o emalloc.o
	$(CC) senjify3.o list.o emalloc.o -o senjify3

senjify3.o: senjify3.c list.h emalloc.h
	$(CC) $(CFLAGS) senjify3.c

list.o: list.c list.h emalloc.h
	$(CC) $(CFLAGS) list.c

emalloc.o: emalloc.c emalloc.h
	$(CC) $(CFLAGS) emalloc.c

clean:
	rm -rf *.o senjify3 
