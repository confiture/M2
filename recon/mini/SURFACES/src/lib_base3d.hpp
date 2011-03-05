///////////////////////////////////////////////////////////////////
// librairie de base pour le mini-projet
// Partie pour la reconstruction de courbes - points en 3D
// Interface

#ifndef _LIB_BASE3D_HPP
#define _LIB_BASE3D_HPP

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

typedef unsigned long ULONG;

// point 3d
class Point
{
	public:
	double x,y,z; // coordonnees

	// constructeurs
	Point(): x(0.0),y(0.0),z(0.0){}
	Point(double x0, double y0, double z0): x(x0),y(y0),z(z0){}
};

// liste d'entiers
typedef list<int> lint;

// vecteur d'entiers
typedef vector<int> vint;

// vecteur d'entiers longs positifs
typedef vector<ULONG> vULONG;

// vecteur de points
typedef vector<Point> vPoint;

// arete d'un graphe value non oriente (s1<s2)
struct Arete
{
	ULONG s1,s2; // indices des deux sommets
	double v;  // valeur de l'arete
};

// graphe value non oriente
struct Graphe
{
	Arete *a; // tableau des aretes valuees
	ULONG nA;   // nombre d'aretes
	ULONG nS;   // nombre de sommets
};

// noeud d'un arbre
struct Noeud
{
	ULONG num;      // numéro du noeud
	vULONG voisins; // indices des sommets voisins
};

// arbre
struct Arbre
{
	Noeud *noeuds; // tableau des sommets
	ULONG n;       // nombre de sommets
};

// triangle
struct Triangle
{
	ULONG s1, s2, s3; // indices des 3 sommets
};

//////////////////////////////////////////////////////////////////////////////
// DECLARATION DES ROUTINES
//////////////////////////////////////////////////////////////////////////////

// écrit la liste d'entiers I
void ecrire(vint I);

// ecrit les n points du tableau P
void ecrire(Point *P, ULONG n);

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

inline bool operator<(const Arete & a1,const Arete & a2){
	return a1.v < a2.v;
}

// calcule le produit scalaire A.B
double dot(Point A, Point B) ;

// calcule le produit vectoriel A x B
Point cross(Point A, Point B);

// calcule la norme de A
double norm(Point A) ;

// calcule la distance entre A et B
double distance_point(Point A, Point B) ;

// calcule la distance entre A et B au carre
inline double distance_point2(const Point & A, const Point & B)
{
	return (A.x -B.x)*(A.x -B.x) + (A.y-B.y)*(A.y-B.y)+ (A.z-B.z)*(A.z-B.z);
}


// calcule la boite englobante des n points du tableau P
void boite_englobante(Point *P, ULONG n,
   double &Pxmin, double &Pxmax,
   double &Pymin, double &Pymax,
   double &Pzmin, double &Pzmax);


// Construit la droite aux moindres carrres a partir de la liste
// de points pts. En sortie, B est le barycentre des points, v est le
// vecteur directeur de la droite, et n est la normale de la droite.
void plan_mc(const std::list<Point> & pts,Point & B,Point & n);


///////////////////////////////////////////////////////////////////
// calcul de la plus petite valeur propre et du vecteur propre associé
// d'une matrice A symétrique de dimension 3
//     ( A11 A12 A13)
// A = ( A12 A22 A23)
//     ( A13 A23 A33)
// Entrée : A11,A12,A13,A22,A23,A33 = les six valeurs définissant
//             la matrice symetrique
// Sortie : lmin = la plus petite valeur propre de A
//          vmin = le vecteur propre unitaire associé à lmin
void elements_propres_min_mat_sym(double A11, double A12, double A13,
 double A22, double A23, double A33,
 double &lmin, Point &vmin);


///////////////////////////////////////////////////////////////////
// calcule l'arbre couvrant minimal correspondant à
// d'un graphe G non oriente dont les aretes valuees sont triees
// par ordre croissant
Arbre calcule_ACM(Graphe G);

///////////////////////////////////////////////////////////////////
// création d'un ensemble de points répartis uniformément sur une sphère
// Entrée : n = entier > 0 (nombre de parallèles)
// Sortie : S = le tableaux des points
//          nS = le nombre de points
// la routine renvoie 1 si l'ensemble de point a pu etre créé,
// 0 sinon (valeur de n trop grande)
int creer_points_sphere(ULONG n, Point* &S, ULONG &nS);
#endif
