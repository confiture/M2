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
	 *Epaisseur moyenne de l'objet.
	 */
	int epaisseur;

	/**
	 *Le nombre de trous de l'objet.
	 */
	int nbTrous;

	/**
	 *\brief Ecrit l'image contenant l'objet dans le fichier
	 *<CODE>fic</CODE>.
	 *
	 *Les caractéristiques de l'objet sont affichées en commentaire.
	 */
	void writePgmItem(const char * fic);

};

#endif
