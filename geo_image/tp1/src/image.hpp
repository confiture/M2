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



//-----------------------------------------------------------------------------//
//-------------------------Fonctions interm�diaires----------------------------//
//-----------------------------------------------------------------------------//
	/*
	 * Cette fonction permet de creer une image repr�sentant l'�l�ment structurant qui sera
	 * donc un carre de taille 'taille'
	 * @param : taille = taille du cot� du cercle
	 * @return : image = image contenant l'�l�ment structurant carr�
	 */
	static image elemCarre(int taille);

	/*
	 * Cette fonction permet de creer une image repr�sentant l'�l�ment structurant qui sera
	 * donc un cercle de taille 'taille'
	 * @param : taille = diam�tre du cercle
	 * @return : image = image contenant l'�l�ment structurant cercle
	 */
	static image elemCercle(int taille);

	static image elemStruct(geom geo,int taille);

	/* Cette fonction permet de dupliquer les bords de l'image m fois sur la hauteur et n fois sur la largeur
	 *(m �tant la hauteur de l'image elem_struct et m la largeur de  l'image elem_struct)
	 * @param : elem_struct = image contenant l'�l�ment structurant
	 * @return : image* = image avec les bords dupliqu�s
	 */
	image* duplique_elemStruc_bord(int taille/*image elem_struct*/)const;

//-----------------------------------------------------------------------------//
//-------------------------OPERATEURS MORPHOLOGIQUES --------------------------//
//-----------------------------------------------------------------------------//
	/* Cette fonction applique l'op�rateur morphologique 'dilatation' sur l'objet courant et renvoie
	 * l'image dilat�e par l'�l�ment structurant elem_struct
	 * @param : elem_struct = image contenant l'�l�ment structurant
	 * @return : image* = image dilat�e
	 */
	image* dilatation(geom g,int taille /*image elem_struct*/)const;

	/* Cette fonction applique l'op�rateur morphologique 'erosion' sur l'objet courant et renvoie
	 * l'image  apr�s �rosion par l'�l�ment structurant elem_struct
	 * @param : elem_struct = image contenant l'�l�ment structurant
	 * @return : image* = image apr�s �rosion
	 */
	image* erosion(geom g,int taille/*image elem_struct*/) const;

	/* Cette fonction applique l'op�rateur morphologique 'ouverture' sur l'objet courant et renvoie
	 * l'image  apr�s ouverture par l'�l�ment structurant elem_struct
	 * @param : elem_struct = image contenant l'�l�ment structurant
	 * @return : image* = image apr�s ouverture
	 */
	image* ouverture(geom g,int taille/*image elem_struct*/) const;


	/* Cette fonction applique l'op�rateur morphologique 'fermeture' sur l'objet courant et renvoie
	 * l'image  apr�s fermeture par l'�l�ment structurant elem_struct
	 * @param : elem_struct = image contenant l'�l�ment structurant
	 * @return : image* = image apr�s fermeture
	 */
	image* fermeture(geom g,int taille/*image elem_struct*/) const;

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
