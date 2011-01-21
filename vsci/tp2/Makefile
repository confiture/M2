LIBS=-lglut -lGLU -lGL
CC=g++
CFLAGS=

all:	VolRawData Example

VolRawData:	VolRawData.o
	$(CC) $(CFLAGS) VolRawData.o -o $@ $(LIBS)

Example:	VolRawData
	./VolRawData lobster.raw 301 324 56 2.0 1

