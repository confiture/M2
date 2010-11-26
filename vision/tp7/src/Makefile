#######################################
# Makefile                            #  
#                                     #
# E.B.                                #
#######################################
 

PROG = face

all : $(PROG) 


# Variables pour la compilation des fichiers
CC        =  gcc
CFLAGS    =  -g -Wall
CPPFLAGS  =  -DDEBUG
LDFLAGS   =  -g 


utils.o : utils.c 
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@ 

NewpixmapIO.o : NewpixmapIO.c utils.h
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@


$(PROG): % : %.o NewpixmapIO.o utils.o
	$(CC) $(LDFLAGS) -o $@ $< NewpixmapIO.o utils.o -lm -ll


clean:
	rm -f *.o *.dat *~

realclean: clean
	rm -f $(PROG)
