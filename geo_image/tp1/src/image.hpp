/**
 *\file polyhedron.hpp
 *\autohor Koraa Galel, Trlin Moreno
 *\brief D�finition des champs et des m�thodes de la classe image.
 */
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
#include <limits>

/**
 *\class image
 *\brief Toutes les m�thodes associ�es � la classe image, en
 *particulier les m�thodes de morphologie math�matique.
 *
 *Les images lues et �crites sont des images au format <CODE>pgm</CODE>
 *en code <CODE>ascii</CODE> (<CODE>P2</CODE>).
 *
 */
class image{
public:
	/**
	 *\brief La g�om�trie utilis�e pour les �l�ments structurants
	 *lors des op�rations de morphologie math�matique/
	 *
	 *\see dilatation, erosion, ouverture, fermeture
	 */
	enum geom{
		/**
		 *G�om�trie d'un carr�.
		 */
		carre,
		/**
		 *G�om�trie d'un cercle.
		 */
		cercle,
		/**
		 *G�om�trie d'un triangle.
		 */
		triangle
	};

	/**
	 *\brief Constructeur vide.
	 */
	image(){}

	/**
	 *\brief Construit une image de hauteur <CODE>hauteur</CODE>,
	 *de largeur <CODE>largeur</CODE> dont tous les pixels valent
	 *<CODE>valmax</CODE>. La valeur maximale est �galement <CODE>valmax</CODE>,
	 *elle vaut 0 par d�faut.
	 *
	 *\param : hauteur la hauteur
	 *\param : largeur la largeur
	 *\param : valmax la valeur maximale
	 */
	image(int hauteur, int largeur,int valmax=0);

	/**
	 *\brief Construit l'image � partir du fichier ayant pour nom <CODE>nomFichier</CODE>
	 *au format <CODE>pgm</CODE> <CODE>P2</CODE>.
	 *
	 *\param nomFichier : nom du fichier
	 */
	image(const char* nomFichier);

	/**
	 *\brief Constructeur par copie.
	 *
	 *\param im : l'image qui va �tre copi�e
	 */
	image(const image & im);

	/**
	 *\brief Met � jour le champ <CODE>valmax</CODE>
	 *qui est la valeur maximale.
	 */
	void updateValmax();

	/**
	 *\brief Construit l'image � partir d'une portion d'image.
	 *
	 *Constructeur par copie de la portion l'image <CODE>im</CODE>
	 *�tant dans le rectangle <CODE>(i1,j1),(i2,j1),(i2,j2),(i1,j2)</CODE>.
	 *
	 *\param i1 : indice de ligne du coin sup�rieur gauche du rectangle
	 *\param i2 : indice de ligne du coin inf�rieur droit du rectange
	 *\param j1 : indice de colonne du coin sup�rieur gauche du rectangle
	 *\param j2 : indice de colonne du coin inf�rieur droit du rectange
	 */
	image(const image & im,int i1,int i2,int j1,int j2);


	/**
	 *Destructeur.
	 */
	inline ~image(){
		delete [] buffer;
	}

	/**
	 *\brief Retourne la hauteur de l'image, c'est � dire son nombre de lignes.
	 */
	inline int getHauteur(){return hauteur;}

	/**
	 *\brief Retourne la largeur de l'image, c'est � dire son nombre de colonnes.
	 */
	inline int getLargeur(){return largeur;}

	/**
	 *\brief Retourne la valeur maximale de l'image.
	 */
	inline int getValmax(){return valmax;}

	/**
	 *\brief Recadre l'image entre les valeurs de <CODE>a</CODE> et <CODE>b</CODE>.
	 *
	 *\param a : borne inf�rieure de la valeur des pixels
	 *\param b : borne sup�rieure de la valeur des pixels
	 */
	void recadre(double a,double b);

	/**
	 *\brief Ecrit l'image courante au format <CODE>pgm P2</CODE> dans
	 *le fichier <CODE>nomFichier</CODE>.
	 *
	 *\param nomFichier : le nom du fichier dans lequel on veut enregistre
	 *l'image
	 */
	int EcrireImagePGM(const char* nomFichier)const;

	/**
	 *\brief Retourne la r�f�rence du pixel � la ligne <CODE>i</CODE> et �
	 *la colonne <CODE>j<CODE>.
	 *
	 *\return la r�f�rence du pixel � la ligne <CODE>i</CODE> et �
	 *la colonne <CODE>j<CODE>
	 */
	int& operator()(int i,int j);

	/**
	 *\brief Retourne la valeur du pixel � la ligne <CODE>i</CODE> et �
	 *la colonne <CODE>j<CODE>.
	 *
	 *\return la valeur du pixel � la ligne <CODE>i</CODE> et �
	 *la colonne <CODE>j<CODE>
	 */
	int operator()(int i,int j)const;

	/**
	 *\brief Seuille l'image avec le seuil <CODE>seuil</CODE>.
	 *
	 *\param seuil : seuil utilis� pour le seuillage
	 */
	int seuiller(int seuil);

	/**
	 *\brief Fabrique l'image n�gative de l'image courante.
	 *
	 *Apr�s l'utilisation de cette m�thode, l'image courante est modifi�e,
	 *elle devient l'image n�gative.
	 */
	int negatif();

	/**
	 *\brief Retourne l'�tiquetage des composantes 4-connexes.
	 *
	 *Si <CODE>int** tab</CODE> est l'�tiquetage des composantes
	 *4-connexes, alros <CODE>tab[i][j]</CODE> a pour valeur l'�tiquette
	 *de la composante connexe du pixel situ� � la ligne <CODE>i</CODE> et �
	 *la colonne <CODE>j</CODE> de l'image.
	 *
	 *Cette m�thode s'ex�cute sur une image binaris�e, c'est � dire
	 *que les valeurs de pixels sont soit 0 ou 255.
	 */
	int** binConnexite4()const;

	/**
	 *\brief Retourne l'�tiquetage des composantes 8-connexes.
	 *
	 *Si <CODE>int** tab</CODE> est l'�tiquetage des composantes
	 *8-connexes, alros <CODE>tab[i][j]</CODE> a pour valeur l'�tiquette
	 *de la composante connexe du pixel situ� � la ligne <CODE>i</CODE> et �
	 *la colonne <CODE>j</CODE> de l'image.
	 *
	 *Cette m�thode s'ex�cute sur une image binaris�e, c'est � dire
	 *que les valeurs de pixels sont soit 0 ou 255.
	 */
	int** binConnexite8()const;

	/**
	 *Fabrique le tableau contenant les composantes connexes de l'image.
	 *Le tableau est donc de m�me dimension que l'image courante.
	 *Deux points appartenant a un m�me objet ont une m�me �tiquette.
	 *Par exemple, si les pixels aux positions <CODE>(i,j)</CODE> et <CODE>(i,j+1)</CODE> appartiennent
	 *� un m�me objet, on a alors � la sortie <CODE>tab[i][j]=tab[i][j+1]</CODE>.
	 *
	 *@param connexity : la connexit� utilis�e qui peut �tre 8 ou 4
	 *@param seuil : seuil utilis� pour distinguer les objets du fond de l'image
	 *@return le tableau des composantes connexes
	 */
	int** makeConnComp(int connexity,int seuil)const;

	/**
	 *\brief Retourne le nombre de composantes connexes.
	 *
	 *\param nconn : la connexit� utilis�e (8 ou 4)
	 *\param seuil : le seuil utilis� pour distinguer les objets du fond
	 */
	int nbConnCom(int nconn,int seuil);

	/**
	 *\brief Ecrit le fichier image au format <CODE>pgm P2</CODE> dont les composantes connexes
	 *sont affich�es en blanc et le fond est affich� en noir.
	 *
	 *\param fic : nom du fichier dans lequel l'image est enregistr�e
	 */
	void dispCompConn(const char* fic)const;

	image* duplique_elemStruc_bord(image elem_struct)const;

	image* dilatation(image elem_struct)const;

	image* erosion(image elem_struct)const;

	image* ouverture(image elem_struct)const;

	/**
	 *\brief Retourne la boule binaire de rayon <CODE>r</CODE> avec
	 *le masque <CODE>masque</CODE> de dimension <CODE>n*n</CODE>.
	 *
	 *\param masque : le masque utilis�
	 *\param n      : dimension du masque
	 *\param r      : le rayon de la boule fabriqu�e
	 */
	static image boule(double** masque,int n,int r);

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

	/*
	 * Cette fonction permet de creer une image repr�sentant l'�l�ment structurant qui sera
	 * donc un triangle de taille 'taille'
	 * @param : taille =  cot� du triangle
	 * @return : image = image contenant l'�l�ment structurant cercle
	 */
	static image elemTriangle(int taille);
	
	
	
	static image elemCroix(int taille,int epaisseur);
	
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
	 * @param elem_struct : image contenant l'�l�ment structurant
	 * @return image* : image apr�s ouverture
	 */
	image* ouverture(geom g,int taille/*image elem_struct*/) const;


	/* Cette fonction applique l'op�rateur morphologique 'fermeture' sur l'objet courant et renvoie
	 * l'image  apr�s fermeture par l'�l�ment structurant elem_struct
	 * @param elem_struct : image contenant l'�l�ment structurant
	 * @return image* : image apr�s fermeture
	 */
	image* fermeture(geom g,int taille/*image elem_struct*/) const;

	void writePgmItems(char * itemsName,int seuil);

	/**
	 *\brief Fabrique l'objet qui a pour étiquette tag dans le tableau des composantes
	 *connexes connexTab.
	 *
	 *\param tag le numéro de la composante connexe de l'objet
	 *\return la structure item
	 */
	item makeItem(int tag,int** connexTab,int seuil)const;

	/**
	 *\brief Fabrique la liste des objets qui sont pr�sents dans l'image.
	 *
	 *\param seuil : le seuil utilis� pour isoler les objets du fond.
	 */
	std::list<item> makeItemList(int seuil)const;

	/**
	 *\brief Retourne l'image qui est la transform�e en distance en utilisant
	 *le masque <CODE>masque</CODE>.
	 *
	 *\param n : la dimension du masque est de <CODE>n*n</CODE>
	 *\param seuil : le seuil utilis� pour binariser l'image
	 */
	image* distanceT(double** masque,int n,int seuil)const;

	image& operator=(const image & im);

private:
	int largeur, hauteur, valmax;
	int* buffer;
};
