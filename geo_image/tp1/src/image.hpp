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

	image(const char* nomFichier);

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

	void recadre(double a,double b);

	int EcrireImagePGM(const char* nomFichier)const;

	int& operator()(int i,int j);

	int operator()(int i,int j)const;

	image LireImagePGM(char* nomFichier);

	int seuiller(int seuil);

	int negatif();

	int** binConnexite4()const;

	int** binConnexite8()const;

	/**
	 *Fabrique le tableau contenant les composantes connexes de l'image.
	 *Le tableau est donc de même dimension que l'image courante.
	 *Deux points appartenant à un même objet ont une même étiquette.
	 *Par exemple, si les pixels aux positions (i,j) et (i,j+1) appartiennent
	 *à un même objet, on a alors à la sortie tab[i][j]=tab[i][j+1].
	 *
	 *@param connexity la connexité utilisé qui peut être 8 ou 4
	 *@seuil seuil utilisé pour distinguer les objets du fond de l'image
	 *@return le tableau des composantes connexes
	 */
	int** makeConnComp(int connexity,int seuil)const;

	int nbConnCom(int nconn,int seuil);

	void dispCompConn(const char* fic)const;

	image* duplique_elemStruc_bord(image elem_struct)const;

	image* dilatation(image elem_struct)const;

	image* erosion(image elem_struct) const;

	image* ouverture(image elem_struct) const;

	void writePgmItems(char * itemsName,int seuil);

	/**
	 *Fabrique l'objet qui a pour étiquette tag dans le tableau des composantes
	 *connexes connexTab.
	 *
	 *@param tag le numéro de la composante connexe de l'objet
	 *@return la structure item
	 */
	item makeItem(int tag,int** connexTab,int seuil)const;

	/**
	 *Fabrique la liste des objets qui sont présents dans l'image.
	 *
	 *@param seuil le seuil utilisé pour isoler les objets du fond.
	 */
	std::list<item> makeItemList(int seuil)const;

	image& operator=(const image & im);
private:
	int largeur, hauteur, valmax;
	int* buffer;

};
