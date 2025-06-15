# Makefile

CC=gcc
CFLAGS=-Wall -g

all: analisador

analisador: main.o lexer.o sintatic.o
	$(CC) $(CFLAGS) -o analisador main.o lexer.o sintatic.o

main.o: src/main.c header/lexer.h
	$(CC) $(CFLAGS) -c src/main.c

lexer.o: src/lexer.c header/lexer.h
	$(CC) $(CFLAGS) -c src/lexer.c

sintatic.o: src/sintatic.c header/sintatic.h
	$(CC) $(CFLAGS) -c src/sintatic.c

clean:
	rm -rf *.o analisador *.txt
