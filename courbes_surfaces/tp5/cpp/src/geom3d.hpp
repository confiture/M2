///////////////////////////////////////////////////////////////////////////////
// types et opérations géométriques en 3D
///////////////////////////////////////////////////////////////////////////////

#ifndef _GEOM_3D_H_
#define _GEOM_3D_H_

#include<vector>

using namespace std;

class Point3D
{ 
public:
	// coordonnées
	double x,y,z;

	// constructeur
    Point3D(double x0=0.0, double y0=0.0, double z0=0.0)
	{ x=x0; y=y0; z=z0; }
		
	// constructeur par copie
    Point3D(const Point3D &P)
	{ x=P.x; y=P.y; z=P.z; }
		
	// opérateurs de modification
	Point3D& operator+=(Point3D P2);	
	Point3D& operator-=(Point3D P2);	
	Point3D& operator*=(double r2);	
	Point3D& operator/=(double r2);	

	// autres méthodes simples
	double norme(); // le module
	void normalise(); // normalise le point
	void print(const char* format="%f"); // ecriture
};  

// surcharge d'opérateurs ou routines existants
Point3D operator+(Point3D P1, Point3D P2);
Point3D operator-(Point3D P1, Point3D P2);
Point3D operator*(double r1, Point3D P2);
Point3D operator*(Point3D P1, double r2);
Point3D operator/(Point3D P1, double r2);
Point3D operator+(Point3D P1);
Point3D operator-(Point3D P1);
bool operator==(Point3D P1, Point3D P2);
bool operator!=(Point3D P1, Point3D P2);

// autres routines
double norme(Point3D P1);
double distance_(Point3D P1, Point3D P2);
double produit_scalaire(Point3D P1, Point3D P2);
Point3D normalise(Point3D P1);
Point3D produit_vectoriel(Point3D P1, Point3D P2);
double volume(Point3D a, Point3D b, Point3D c);
double distance_point_segment(Point3D P, Point3D S1, Point3D S2);
double distance_point_triangle(Point3D P, Point3D S1, Point3D S2, Point3D S3);
ostream & operator<<(ostream & o, Point3D P);	
void print(Point3D P, const char* format="%f");

#endif
