CC=gcc
CFLAGS=-c
GFLAGS=-g -c
BUILDDIR=build

all: fasm

.PHONY: tools

fasm:
	mkdir -p build
	$(CC) $(CFLAGS) fasm.c -o $(BUILDDIR)/fasm.o
	$(CC) $(CFLAGS) lexer.c -o $(BUILDDIR)/lexer.o
	$(CC) $(CFLAGS) asm.c -o $(BUILDDIR)/asm.o
	$(CC) $(CFLAGS) token.c -o $(BUILDDIR)/token.o
	$(CC) -o fasm $(BUILDDIR)/fasm.o $(BUILDDIR)/lexer.o $(BUILDDIR)/asm.o $(BUILDDIR)/token.o

clean:
	rm -rf build
	rm -rf fasm

debug:
	mkdir -p build
	$(CC) $(GFLAGS) fasm.c -o $(BUILDDIR)/fasm.o
	$(CC) $(GFLAGS) lexer.c -o $(BUILDDIR)/lexer.o
	$(CC) $(GFLAGS) asm.c -o $(BUILDDIR)/asm.o
	$(CC) $(GFLAGS) token.c -o $(BUILDDIR)/token.o
	$(CC) -o fasm $(BUILDDIR)/fasm.o $(BUILDDIR)/lexer.o $(BUILDDIR)/asm.o $(BUILDDIR)/token.o

compile:
	make clean && make
