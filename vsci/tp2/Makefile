LIBS=-lglut -lGLU -lGL
CC=g++
CFLAGS=

all:	VolRawData Example

VolRawData:	VolRawData.o
	$(CC) $(CFLAGS) VolRawData.o -o $@ $(LIBS)

Example:	VolRawData
	./VolRawData CThead.bin 256 256 113 2.0 2
