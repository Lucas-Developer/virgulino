CC=gcc
MAIN=main.c
LIBS=output.c
OBJ=output.o
CFLAGS=-pedantic -Wall -O2 -fPIC -std=c11 -ldl
SHARED=$(CFLAGS) -shared 
LIBSWITCH=-c
OUTPUT=-o
DEST=../virgulino

all: default

default:
	$(CC) $(CFLAGS) $(LIBS) $(LIBSWITCH)
	$(CC) $(CFLAGS) $(OBJ) $(MAIN) $(OUTPUT) $(DEST)

clean:
	rm -rf *.*.gch *.o ../virgulino 

