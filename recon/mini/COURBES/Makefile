CC = g++
CFLAGS = -Wall
INCLUDES =
LIBS = 

%.o : %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

prog_test2d : lib_base2d.o xfig.o prog_test2d.o 
	$(CC) $(CFLAGS) $(LIBS) lib_base2d.o xfig.o prog_test2d.o  -o $@ 

