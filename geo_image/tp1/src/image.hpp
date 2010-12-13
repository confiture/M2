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
	enum geom{carre,cercle};

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
	 *Le tableau est donc de mÃªme dimension que l'image courante.
	 *Deux points appartenant Ã  un mÃªme objet ont une mÃªme Ã©tiquette.
	 *Par exemple, si les pixels aux positions (i,j) et (i,j+1) appartiennent
	 *Ã  un mÃªme objet, on a alors Ã  la sortie tab[i][j]=tab[i][j+1].
	 *
	 *@param connexity la connexitÃ© utilisÃ© qui peut Ãªtre 8 ou 4
	 *@seuil seuil utilisÃ© pour distinguer les objets du fond de l'image
	 *@return le tableau des composantes connexes
	 */
	int** makeConnComp(int connexity,int seuil)const;

	int nbConnCom(int nconn,int seuil);

	void dispCompConn(const char* fic)const;

	image* duplique_elemStruc_bord(image elem_struct)const;

	image* dilatation(image elem_struct)const;

	image* erosion(image elem_struct) const;

	image* ouverture(image elem_struct) const;



//-----------------------------------------------------------------------------//
//-------------------------Fonctions intermédiaires----------------------------//
//-----------------------------------------------------------------------------//
	/*
	 * Cette fonction permet de creer une image représentant l'élément structurant qui sera
	 * donc un carre de taille 'taille'
	 * @param : taille = taille du coté du cercle
	 * @return : image = image contenant l'élément structurant carré
	 */
	static image elemCarre(int taille);

	/*
	 * Cette fonction permet de creer une image représentant l'élément structurant qui sera
	 * donc un cercle de taille 'taille'
	 * @param : taille = diamètre du cercle
	 * @return : image = image contenant l'élément structurant cercle
	 */
	static image elemCercle(int taille);

	static image elemStruct(geom geo,int taille);

	/* Cette fonction permet de dupliquer les bords de l'image m fois sur la hauteur et n fois sur la largeur
	 *(m étant la hauteur de l'image elem_struct et m la largeur de  l'image elem_struct)
	 * @param : elem_struct = image contenant l'élément structurant
	 * @return : image* = image avec les bords dupliqués
	 */
	image* duplique_elemStruc_bord(int taille/*image elem_struct*/)const;

//-----------------------------------------------------------------------------//
//-------------------------OPERATEURS MORPHOLOGIQUES --------------------------//
//-----------------------------------------------------------------------------//
	/* Cette fonction applique l'opérateur morphologique 'dilatation' sur l'objet courant et renvoie
	 * l'image dilatée par l'élément structurant elem_struct
	 * @param : elem_struct = image contenant l'élément structurant
	 * @return : image* = image dilatée
	 */
	image* dilatation(geom g,int taille /*image elem_struct*/)const;

	/* Cette fonction applique l'opérateur morphologique 'erosion' sur l'objet courant et renvoie
	 * l'image  après érosion par l'élément structurant elem_struct
	 * @param : elem_struct = image contenant l'élément structurant
	 * @return : image* = image après érosion
	 */
	image* erosion(geom g,int taille/*image elem_struct*/) const;

	/* Cette fonction applique l'opérateur morphologique 'ouverture' sur l'objet courant et renvoie
	 * l'image  après ouverture par l'élément structurant elem_struct
	 * @param : elem_struct = image contenant l'élément structurant
	 * @return : image* = image après ouverture
	 */
	image* ouverture(geom g,int taille/*image elem_struct*/) const;


	/* Cette fonction applique l'opérateur morphologique 'fermeture' sur l'objet courant et renvoie
	 * l'image  après fermeture par l'élément structurant elem_struct
	 * @param : elem_struct = image contenant l'élément structurant
	 * @return : image* = image après fermeture
	 */
	image* fermeture(geom g,int taille/*image elem_struct*/) const;

	void writePgmItems(char * itemsName,int seuil);

	/**
	 *Fabrique l'objet qui a pour Ã©tiquette tag dans le tableau des composantes
	 *connexes connexTab.
	 *
	 *@param tag le numÃ©ro de la composante connexe de l'objet
	 *@return la structure item
	 */
	item makeItem(int tag,int** connexTab,int seuil)const;

	/**
	 *Fabrique la liste des objets qui sont prÃ©sents dans l'image.
	 *
	 *@param seuil le seuil utilisÃ© pour isoler les objets du fond.
	 */
	std::list<item> makeItemList(int seuil)const;

	image& operator=(const image & im);
private:
	int largeur, hauteur, valmax;
	int* buffer;
};
