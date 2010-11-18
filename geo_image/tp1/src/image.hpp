#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include "Util.h"
#include "item.hpp"
#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <set>
#include <list>
#include <string>
#include <sstream>

class image{
public:

	image(){}

	image(int hauteur, int largeur,int valmax=0);

	image(char* nomFichier);

	image(const image &);

	image(const image &,int i1,int i2,int j1,int j2);

	/**
	 *Destructeur.
	 */
	inline ~image(){
		delete [] buffer;
	}

	inline int getHauteur(){return hauteur;}

	inline int getLargeur(){return largeur;}

	inline int getValmax(){return valmax;}

	int EcrireImagePGM(const char* nomFichier)const;

	int& operator()(int i,int j);

	int operator()(int i,int j)const;

	image LireImagePGM(char* nomFichier);

	int seuiller(int seuil);

	int negatif();

	int** connexite4()const;

	int** connexite8()const;

	int nbConnCom(int nconn,int seuil);

	void dispCompConn(const char* fic)const;

	image duplique_elemStruc_bord(image elem_struct)const;

	void writePgmItems(char * itemsName,int seuil);

private:
	int largeur, hauteur, valmax;
	int* buffer;

};
