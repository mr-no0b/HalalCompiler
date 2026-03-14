CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -D_POSIX_C_SOURCE=200809L

all: halalc

test: halalc
	bash ./run_tests.sh

halalc: parser.tab.c lex.yy.c main.c
	$(CC) $(CFLAGS) -o halalc parser.tab.c lex.yy.c main.c -lfl

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

clean:
	rm -f halalc parser.tab.c parser.tab.h lex.yy.c out.c

.PHONY: all clean test
