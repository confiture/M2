#ifndef _GRID_HPP
#define _GRID_HPP

#include "lib_base2d.hpp"
#include <limits>
#include <map>
#include "coords.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

/**
 *Retourne le pas de la grille max_i(min_(j!=i)(P_i,P_j)) * facteur.
 */
inline double calc_pas(int n,Point * pts,double facteur){
	int indMinDist;
	double nrm;
	double dist[n];
	double h=-numeric_limits<double>::infinity();
	for(int i=0;i<n;i++)
		dist[i]=numeric_limits<double>::infinity();

	for(int i=0;i<n-1;i++){
		if(dist[i]==numeric_limits<double>::infinity()){
			for(int j=i+1;j<n;j++){
				nrm=norm(pts[i]-pts[j]);
				if(nrm<dist[i]){
					dist[i]=nrm;
					indMinDist=j;
				}
			}
			dist[indMinDist]=dist[i];

			if(h<dist[i])h=dist[i];
		}
	}

	return h*facteur;
}

/**
 *Structure grille en 2 dimensions. La grille est principalement constituée d'un
 *tableau à 2 dimensions de double. Elle représente une grille régulière. Sa principale
 *fonction est en fait de contenir beaucoup de champs tel que l'abscisse minimum xmin,
 *l'abscisse maximum xmax, le pas, le nombre de lignes etc... Cela évite de passer
 *trop de paramètres aux fonctions.
 */
struct grille{

	/**
	 *Constructeur de la grille.
	 * nl : le nombre de lignes de la grille
	 * nc : le nombre de colonnes de la grille
	 * pas : le pas de la grille
	 * xmin,xmax,ymin,ymax : Respectivement : minimum des abscisses, maximum des abscisses, minimum des ordonnees, maximum des ordonnees
	 */
	grille(int nl,int nc,double pas,double xmin,double xmax,double ymin,double ymax){
		vals=new double*[nl];
		for(int i=0;i<nl;i++)vals[i]=new double[nc];

		this->nl=nl;
		this->nc=nc;
		this->pas=pas;
		this->xmin=xmin;
		this->xmax=xmax;
		this->ymin=ymin;
		this->ymax=ymax;
	}

	/**
	 *Retourne la reference de la grille en (i,j). Attention (i,j) ne sont pas les
	 *coordonnes réelles du point mais les coordonnes du tableau vals[i][j].
	 *Donc (i,j) correspond en realite a (j*pas+xmin,i*pas+ymin).
	 */
	double& operator()(int i,int j){
		return vals[i][j];
	}

	/**
	 *Calcule la courbe isovaleur a partir des distances signees definies
	 *dans la grille reguliere. A la fin de cette methode, la courbe isovaleur
	 *est composee des segments [E1[i] , E2[i]].
	 */
	void calc_iso_courbe(double isovaleur,vPoint & E1,vPoint & E2);

	int nl;// le nombre de lignes de la grille
	int nc;// le nombre de colonnes de la grille
	double pas;// le pas
	double xmin,xmax,ymin,ymax;// Respectivement : minimum des abscisses, maximum des abscisses, minimum des ordonnees, maximum des ordonnees
	double** vals;// tableau definissant la grille reguliere
};

/**
 *Point a 3 coordonnées.
 */
struct Point3
{
	double x,y,z; // les 3 coordonnées
	Point3()
	{
		x=0.0; y=0.0; z=0.0;
	}
	Point3(double x0, double y0, double z0)
	{
		x=x0; y=y0; z=z0;
	}
};

// calcul de A+B avec A,B de type Point3
Point3 operator+(Point3 A, Point3 B);

// calcul de A-B avec A,B de type Point3
Point3 operator-(Point3 A, Point3 B);

// calcul de r*A avec r de type double et A de type Point3
Point3 operator*(double r, Point3 A);

// calcul de A*r avec r de type double et A de type Point3
Point3 operator*(Point3 A, double r);

// calcul de A/r avec r de type double et A de type Point3
Point3 operator/(Point3 A, double r);

/**
 *Retourne la grille régulière des distances signées.
 *n   : nombre de points
 *pts : les points
 *normales : les normales aux points
 *pas : pas de la grille régulière
 */
grille calc_grille_dist(int n,Point * pts,Point * normales,double pas);

/**
 *Retourne l'intersection du segment [p1,p2] avec le plan z=v.
 *
 */
inline Point3 intersection(const Point3& p1,const Point3& p2,int v){
	if(v==p1.z)return p1;
	if(v==p2.z)return p2;

	double a = (v-p2.z)/(p1.z-p2.z);
	return a*p1+(1-a)*p2;
}

/**
 *Oriente les normales contenues dans normales. ACM est l'arbre couvrant minimal
 *issu du graphe ou une arete e=(i,j) appartient au graphe G ssi d(P_i,P_j)<=r et ou
 *v(e)=1-|normales[i] . normales[j]|. Le tableau visite doit etre de la meme taille
 *que normales et initialise a false. Il signale qu'un noeud de l'arbre ACM a ete visite.
 */
void oriente_normales(Arbre ACM,int numNoeud,Point * normales,bool * visite);

/**
 *Calcule les normales à partir des points dans le tableau pts. Les normales
 *sont stockées dans le tableau normales. pts et normales doivent etre de taille
 *n, r est le rayon utilisé pour le calcul des normales.
 */
void normales(int n,Point * pts,Point * normales,double r);

#endif
