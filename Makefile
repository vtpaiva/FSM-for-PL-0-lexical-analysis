# Makefile

CC=gcc
CFLAGS=-Wall -g

all: analisador

analisador: main.o lexer.o
	$(CC) $(CFLAGS) -o analisador main.o lexer.o

main.o: src/main.c header/lexer.h
	$(CC) $(CFLAGS) -c src/main.c

lexer.o: src/lexer.c header/lexer.h
	$(CC) $(CFLAGS) -c src/lexer.c

clean:
	rm -rf *.o analisador saida.txt
