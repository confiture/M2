/**
 *\file item.hpp
 *\autohor Koraa Galel, Trlin Moreno
 *\brief Définition de la classe item.
 */
#ifndef _ITEM_HPP
#define _ITEM_HPP

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <fstream>
#include <streambuf>
class image;

/**
 *\class item
 *Un item est un objet dans une image, c'est à dire une composante
 *connexe possédant certaines caractéristiques (épaisseur moyenne, nombre de trous).
 */
class item{
public:
	/**
	 *Une image qui contient l'objet en question.
	 */
	image* pIm;

	/**
	 *Le nombre de trous de l'objet.
	 */
	int nbTrous;

	/**
	 *L'épaisseur moyenne de l'objet calculée comme le diamètre moyen
	 *de l'axe médian.
	 */
	double epaisseur;

	/**
	 *L'aire de l'objet.
	 */
	int aire;

	/**
	 *\brief Ecrit l'image contenant l'objet dans le fichier
	 *<CODE>fic</CODE>.
	 *
	 *Les caractéristiques de l'objet sont affichées en commentaire.
	 */
	void writePgmItem(const char * fic);

	/**
	 *\brief Met à jour l'épaisseur moyenne de l'objet.
	 *
	 *\param masque : le masque utilisé pour les distances
	 *\param n      : dimension du masque
	 *\param seuil : seuil utilisé pour distinguer l'objet du fond
	 */
	void loadEpaisseur(double** masque,int n,int seuil);

	/**
	 *\brief Met à jour le nombre de trous de l'objet.
	 *
	 *\param seuil : seuil utilisé pour distinguer l'objet du fond
	 */
	void loadNbTrous(int seuil);

	/**
	 *\brief Met à jour l'aire (en pixels) de l'objet.
	 *
	 *\param seuil : seuil utilisé pour distinguer l'objet du fond
	 */
	void loadAire(int seuil);

	/**
	 *\brief Met à jour tous les champs de l'objet (toutes ses caractéristiques).
	 *
	 *\param masque : le masque utilisé pour les distances
	 *\param n      : dimension du masque
	 *\param seuil : seuil utilisé pour distinguer l'objet du fond
	 */
	void load(double** masque,int n,int seuil);

};

#endif
