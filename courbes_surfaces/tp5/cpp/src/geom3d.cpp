///////////////////////////////////////////////////////////////////////////////
// types et opérations géométriques en 3D
///////////////////////////////////////////////////////////////////////////////

#include<math.h>
#include<stdio.h>
#include<iostream>
#include"geom3d.hpp"

///////////////////////////////////////////////////////////////////////////////
// les définitions des différentes méthodes de la classe Point3D
///////////////////////////////////////////////////////////////////////////////

double Point3D::norme()
{
	return sqrt(x*x+y*y+z*z);
}

void Point3D::normalise()
{
	double n = sqrt(x*x+y*y+z*z);
	x /= n; y /= n; z /= n; 
}

void Point3D::print(const char* format)
{
	printf("(");	
	printf(format, x);    
	printf(",");
	printf(format, y);    
	printf(",");
	printf(format, z);    
	printf(")");	
}

Point3D& Point3D::operator+=(Point3D P2)
{
	x += P2.x;
	y += P2.y;
	z += P2.z;
	return *this;
}

Point3D& Point3D::operator-=(Point3D P2)
{
	x -= P2.x;
	y -= P2.y;
	z -= P2.z;
	return *this;
}

Point3D& Point3D::operator*=(double r2)
{
	x *= r2;
	y *= r2;
	z *= r2;
	return *this;
}

Point3D& Point3D::operator/=(double r2)
{
	x /= r2;
	y /= r2;
	z /= r2;
	return *this;
}


///////////////////////////////////////////////////////////////////////////////
// autres routines
///////////////////////////////////////////////////////////////////////////////
Point3D operator+(Point3D P1, Point3D P2)
{
	return Point3D  (
		P1.x+P2.x ,
		P1.y+P2.y ,
		P1.z+P2.z   );
}

Point3D operator-(Point3D P1, Point3D P2)
{
	return Point3D  (
		P1.x-P2.x ,
		P1.y-P2.y ,
		P1.z-P2.z   );
}

Point3D operator*(double r1, Point3D P2)
{
	return Point3D  (
		r1*P2.x ,
		r1*P2.y ,
		r1*P2.z   );
}

Point3D operator*(Point3D P1, double r2)
{
	return Point3D  (
		P1.x*r2 ,
		P1.y*r2 ,
		P1.z*r2   );
}

Point3D operator/(Point3D P1, double r2)
{
	return Point3D  (
		P1.x/r2 ,
		P1.y/r2 ,
		P1.z/r2   );
}

Point3D operator+(Point3D P1)
{
	return Point3D  (
		P1.x ,
		P1.y ,
		P1.z   );
}

Point3D operator-(Point3D P1)
{
	return Point3D  (
		-P1.x ,
		-P1.y ,
		-P1.z	);
}

bool operator==(Point3D P1, Point3D P2)
{
	return
	  (P1.x == P2.x) &&
	  (P1.y == P2.y) &&
	  (P1.z == P2.z) ;
}

bool operator!=(Point3D P1, Point3D P2)
{
	return !(P1==P2);
}


ostream & operator<<(ostream & o, Point3D P)
{
	o << "(" << P.x << "," << P.y << "," << P.z << ")";
	return o;
}

void print(Point3D P, const char* format)
{
	P.print(format);
}

double norme(Point3D P1)
{
	return P1.norme();
}
double distance_(Point3D P1, Point3D P2)
{
	return norme(P1-P2);
}
double produit_scalaire(Point3D P1, Point3D P2)
{
	return P1.x*P2.x + P1.y*P2.y + P1.z*P2.z ;	  
}

Point3D normalise(Point3D P1)
{
	return P1/norme(P1);
}

Point3D produit_vectoriel(Point3D P1, Point3D P2)
{
	return Point3D(
	  P1.y*P2.z-P2.y*P1.z,
	  P1.z*P2.x-P2.z*P1.x,
	  P1.x*P2.y-P2.x*P1.y );
}

///////////////////////////////////////////////////////////////////////////////
// produit mixte des 3 points a,b,c
double volume(Point3D a, Point3D b, Point3D c)
{
	return produit_scalaire(a,produit_vectoriel(b,c));
}

///////////////////////////////////////////////////////////////////////////////
// calcul de la distance entre un point P et un segment [S1,S2]              //
// Entrée : P = un point 3D                                                  //
//          S1,S2 = points-extrémité 3D du segment                           //
///////////////////////////////////////////////////////////////////////////////
double distance_point_segment(Point3D P, Point3D S1, Point3D S2)
{
	double t;
	Point3D v1,v2,v12;
	
	v1  = P-S1;
	v2  = P-S2;
	v12 = S2-S1;
	t = produit_scalaire(v1 ,v12)/
	    produit_scalaire(v12,v12);
	if (t<0)
		return norme(v1);
	if (t>1)
		return norme(v2);
	return distance_( v1, t*v12 );
}

///////////////////////////////////////////////////////////////////////////////
// calcul de la distance entre un point P et un triangle [S1,S2,S3]          //
// Entrée : P = un point 3D                                                  //
//          S1,S2,S3 = points-extrémité 3D du triangle                       //
///////////////////////////////////////////////////////////////////////////////
double distance_point_triangle(Point3D P, Point3D S1, Point3D S2, Point3D S3)
{
	double u,v,w,a11,a12,a22,b1,b2,delta;
	Point3D v1,v12,v13;
	
	v1  = P-S1;
	v12 = S2-S1;
	v13 = S3-S1;
	
	a11 = produit_scalaire(v12,v12);
	a12 = produit_scalaire(v12,v13);
	a22 = produit_scalaire(v13,v13);
	b1  = produit_scalaire(v1,v12);
	b2  = produit_scalaire(v1,v13);
	delta = a11*a22-a12*a12;
	
	u = (b1*a22-b2*a12)/delta;
	v = (b2*a11-b1*a12)/delta;
	w = 1.0-u-v;
//cout << "u = " << u << ", v = " << v << endl;
	
	if (u<0)
		return(distance_point_segment(P,S1,S3));
	if (v<0)
		return(distance_point_segment(P,S1,S2));
	if (w<0)
		return(distance_point_segment(P,S2,S3));
	
	return distance_(v1, u*v12+v*v13);  
}

