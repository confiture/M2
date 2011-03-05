#ifndef _SURFACE_HPP
#define _SURFACE_HPP

#include "lib_base3d.hpp"
#include <limits>
#include <list>
#include <iostream>
#include <vector>
#include <algorithm>

/**
 *Retourne le pas de la grille max_i(min_(j!=i)(P_i,P_j)) * facteur.
 */
inline double calc_pas(int n,Point * pts){
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

	return 2*h;
}

/**
 *Structure grille en 3 dimensions. La grille est principalement constituée d'un
 *tableau à 3 dimensions de double. Elle représente une grille régulière. Sa principale
 *fonction est en fait de contenir beaucoup de champs tel que l'abscisse minimum xmin,
 *l'abscisse maximum xmax, le pas, le nombre de lignes etc... Cela évite de passer
 *trop de paramètres aux fonctions.
 */
struct grille{

	/**
	 *Constructeur de la grille.
	 * nl : le nombre de lignes de la grille
	 * nc : le nombre de colonnes de la grille
	 * nt : dimension en la troisieme coordonnee
	 * pas : le pas de la grille
	 * xmin,xmax,ymin,ymax,zmin,zmax : Respectivement : minimum des abscisses, maximum des abscisses, minimum des ordonnees, maximum des ordonnees, minimum des cotes, maximum des cotes
	 */
	grille(int nl,int nc,int nt,double pas,double xmin,double xmax,double ymin,double ymax,
	       double zmin,double zmax){
		vals=new double**[nl];
		for(int i=0;i<nl;i++){
			vals[i]=new double*[nc];
			for(int j=0;j<nc;j++)
				vals[i][j]=new double[nt];
		}

		this->nl=nl;
		this->nc=nc;
		this->nt=nt;
		this->pas=pas;
		this->xmin=xmin;
		this->xmax=xmax;
		this->ymin=ymin;
		this->ymax=ymax;
		this->zmin=zmin;
		this->zmax=zmax;
	}

	/**
	 *Destructeur
	 */
	~grille(){
		for(int i=0;i<nl;i++){
			for(int j=0;j<nc;j++){
				delete[] vals[i][j];
			}
			delete[] vals[i];
		}
		delete[] vals;
	}

	/**
	 *Retourne la reference de la grille en (i,j,k). Attention (i,j,k) ne sont pas les
	 *coordonnes réelles du point mais les coordonnes du tableau vals[i][j][k].
	 *Donc (i,j,k) correspond en realite a (i*pas+xmin,j*pas+ymin,k*pas+zmin).
	 */
	double& operator()(int i,int j,int k){
		return vals[i][j][k];
	}

	/**
	 *Calcule la surface isovaleur de la grille régulière. T est la liste des triangles
	 *qui composent la surface iso-valeur à l'issue de cette méthode. S sont les coordonnées
	 *des sommets des triangles. L'odre de S doit être préservé.
	 */
	void calc_iso_surf(double v,std::list<Triangle> & T,std::list<Point> & S);

	int nl; // le nombre de lignes de la grille
	int nc; // le nombre de colonnes de la grille
	int nt; // dimension en la troisieme coordonnee
	double pas;// le pas de la grille
	double xmin,xmax,ymin,ymax,zmin,zmax;// Respectivement : minimum des abscisses, maximum des abscisses, minimum des ordonnees, maximum des ordonnees, minimum des cotes, maximum des cotes
	double*** vals; // tableau 3 dimensions definissant la grille
};

/**
 *Points a 4 coordonnees.
 */
struct Point4
{
	double x,y,z,w; // les 4 coordonnées
	Point4()
	{
		x=0.0; y=0.0; z=0.0; w=0.0;
	}

	Point4(double x0, double y0, double z0, double w0)
	{
		x=x0; y=y0; z=z0; w=w0;
	}
};

// calcul de A+B avec A,B de type Point4
Point4 operator+(Point4 A, Point4 B);

// calcul de A-B avec A,B de type Point4
Point4 operator-(Point4 A, Point4 B);

// calcul de r*A avec r de type double et A de type Point4
Point4 operator*(double r, Point4 A);

// calcul de A*r avec r de type double et A de type Point4
Point4 operator*(Point4 A, double r);

// calcul de A/r avec r de type double et A de type Point4
Point4 operator/(Point4 A, double r);

grille calc_grille_dist(ULONG n,Point * pts,Point * normales);

/**
 *Retourne l'intersection du segment [p1,p2] avec le plan w=v.
 *
 */
inline Point4 intersection(const Point4& p1,const Point4& p2,double v){
	double a = (v-p2.w)/(p1.w-p2.w);
	return a*p1+(1-a)*p2;
}
/**
 *Un tetraedre en dimension 4 constitue donc d'un tableau de 4 points
 *en dimension 4.
 */
struct Tetraedre4{
	Point4 p[4];
};

/**
 *Un triangle en dimension 4 constitue donc d'un tableau de 3 points
 *en dimension 4.
 */
struct Triangle4{
	Point4 p[3];
};

/**
 *Calcule l'intersection de l'hyperplan w=v avec le tetetraedre te. Les points
 *d'intersection sont stockés dans le tableau de points tr. Le nombre de points
 *intersectés est enregistré dans taille.
 */
void intersectionTetra4(Tetraedre4 te,double v,Point4* tr,int & taille);


/**
 *Oriente les normales contenues dans normales. ACM est l'arbre couvrant minimal
 *issu du graphe ou une arete e=(i,j) appartient au graphe G ssi d(P_i,P_j)<=r et ou
 *cout(e)=1-|normales[i] . normales[j]|. Le tableau visite doit etre de la meme taille
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
