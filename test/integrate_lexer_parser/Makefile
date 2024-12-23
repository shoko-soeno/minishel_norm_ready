CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

OBJ = lexer.o parser.o main.o

all: shell_parser

shell_parser: $(OBJ)
	$(CC) $(CFLAGS) -o shell_parser $(OBJ)

lexer.o: lexer.c sample.h
	$(CC) $(CFLAGS) -c lexer.c

parser.o: parser.c sample.h
	$(CC) $(CFLAGS) -c parser.c

main.o: main.c sample.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o shell_parser
