LIBS=-lglut -lGLU -lGL
# LIBS=/usr/lib/libglut.so.3 /usr/lib/libGL.so.1 /usr/lib/libGLU.so.1
INCLUDES=-I.
CC=gcc
CFLAGS=

all:	tp3 Exemple1

tp3.o:   tp3.c
	$(CC) $(CFLAGS) -c tp3.c $(INCLUDES)
	
tp3:	tp3.o
	$(CC) $(CFLAGS) tp3.o -o $@ $(LIBS)

Exemple1:	tp3
	./tp3

Exemple2:	tp3
	./tp3 CThead.bin 256 256 113 2 2

