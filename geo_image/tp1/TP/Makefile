 
# options de compilation
# au choix selon que l'on utilise le mode DEBUG ou non
#CFLAGS=-Wall -W -std=c99
CFLAGS=-Wall -W -std=c99 -DDEBUG
LFLAGS=

#ex�cutable � cr�er
default : traite_image 

# nom des fichiers objets � cr�er
OBJ= main.o

# nom du compilateur
GCC=`which gcc`

# �dition de liens
traite_image : $(OBJ)
	$(GCC) $(LFLAGS) $^ -o $@ 

# cr�ation des fichiers objets
%.o : %.c 
	$(GCC) $(CFLAGS) -c $<

# nettoyage
clean : 
	rm -f *~ $(OBJ) a.out core 
