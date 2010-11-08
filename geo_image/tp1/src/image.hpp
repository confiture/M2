#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include "Util.h"
#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>

class image{
public:

	image(int hauteur, int largeur,int valmax=0);

	image(char* nomFichier);

	image(const image &);

	/**
	 *Destructeur.
	 */
	inline ~image(){
		delete [] buffer;
	}

	int EcrireImagePGM(char* nomFichier)const;

	int& operator()(int i,int j);

	int operator()(int i,int j)const;

	image LireImagePGM(char* nomFichier);

	int seuiller(int seuil);

	int negatif();

	int** composante_connnex(int conn)const;

	void dispCompConn(char* fic)const;

private:
	int largeur, hauteur, valmax;
	int* buffer;

};
