# Makefile

CC=gcc
CFLAGS=-Wall -g

all: analisador

analisador: main.o lexer.o
	$(CC) $(CFLAGS) -o analisador main.o lexer.o

main.o: main.c lexer.h
	$(CC) $(CFLAGS) -c main.c

lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

clean:
	rm -f *.o analisador saida.txt
