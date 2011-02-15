///////////////////////////////////////////////////////////////////
// librairie de base pour le mini-projet
// Partie pour la reconstruction de courbes - points en 2D
// Interface

#ifndef _LIB_BASE2D_HPP
#define _LIB_BASE2D_HPP

#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<vector>
#include<list>

#ifndef ABS
#define ABS(x) ((x)>0 ? (x) : (-x))
#endif

#ifndef MIN
#define MIN(x,y) ((x)>(y) ? (y) : (x))
#endif

#ifndef MAX
#define MAX(x,y) ((x)<(y) ? (y) : (x))
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// DECLARATION DES STRUCTURES DE DONNEES
//////////////////////////////////////////////////////////////////////////////

// point 2d
class Point
{
	public:
	double x,y; // coordonnees

	// constructeurs
	Point(): x(0.0),y(0.0){}
	Point(double x0, double y0): x(x0),y(y0){}
};

// liste d'entiers
typedef list<int> lint;

// vecteur d'entiers
typedef vector<int> vint;

// vecteur de points
typedef vector<Point> vPoint;

// arete d'un graphe value non oriente (s1<s2)
struct Arete
{
	int s1,s2; // indices des deux sommets
	double v;  // valeur de l'arete
};

// graphe value non oriente
struct Graphe
{
	Arete *a; // tableau des aretes valuees
	int nA;   // nombre d'aretes
	int nS;   // nombre de sommets
};

// noeud d'un arbre
struct Noeud
{
	int num;
	vint voisins;
};

// arbre
struct Arbre
{
	Noeud *noeuds;
	int n;
};

//////////////////////////////////////////////////////////////////////////////
// DECLARATION DES ROUTINES
//////////////////////////////////////////////////////////////////////////////

// écrit la liste d'entiers I
void ecrire(vint I);

// ecrit les n points du tableau P
void ecrire(Point *P, int n);

///////////////////////////////////////////////////////////////////
// routines de calcul sur des points 2D

// calcule le point égal à A+B
Point operator+(Point A, Point B);

// calcule le point égal à A-B;
Point operator-(Point A, Point B);

// calcule le point égal à -A
Point operator-(Point A);

// calcule le point égal à r*A avec r scalaire réel
Point operator*(double r, Point A);

// calcule le point égal à A*r avec r scalaire réel
Point operator*(Point A, double r);

// calcule le point égal à A/r avec r scalaire réel
Point operator/(Point A, double r);

// a1<a2 si a1.v < a2.v
bool operator<(Arete a1,Arete a2);

// calcule le produit scalaire A.B
double dot(Point A, Point B) ;

// calcule la norme de A
double norm(Point A) ;

// calcule la distance entre A et B
double distance_point(Point A, Point B) ;

// Construit la droite aux moindres carrres a partir de la liste
// de points pts. En sortie, B est le barycentre des points, v est le
// vecteur directeur de la droite, et n est la normale de la droite.
void droite_mc(const std::list<Point> & pts,Point & B,Point & v,Point & n);

// calcule la boite englobante des n points du tableau P
void boite_englobante(Point *P, int n,
   double &Pxmin, double &Pxmax, double &Pymin, double &Pymax);


///////////////////////////////////////////////////////////////////
// calcul des elements propres de la matrice carree symetrique A
//     ( A11 A12 )
// A = (         )
//     ( A12 A22 )
// Entrée : A11,A12,A22 = les trois valeurs définissant la matrice symetrique
// Sortie : l1,l2 = les deux valeurs propres avec l1 <= l2
//          v1,v2 =  les vecteurs propres unitaires associés
void elements_propres_mat_sym(double A11, double A12, double A22,
 double &l1, double &l2, Point &v1, Point &v2);


///////////////////////////////////////////////////////////////////
// calcule l'arbre couvrant minimal correspondant à
// d'un graphe G non oriente dont les aretes valuees sont triees
// par ordre croissant
Arbre calcule_ACM(Graphe G);

#endif
