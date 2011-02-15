#ifndef _GRID_HPP
#define _GRID_HPP

#include "lib_base2d.hpp"
#include <limits>
#include <map>
#include "coords.hpp"
#include <iostream>

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

	return h;
}

struct grille{
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

	double& operator()(int i,int j){
		return vals[i][j];
	}

	void calc_iso_courbe(double,vPoint & E1,vPoint & E2);

	int nl;
	int nc;
	double pas;
	double xmin,xmax,ymin,ymax;
	double** vals;
};

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

grille calc_grille_dist(int n,Point * pts,Point * normales);

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


void normales(int n,Point * pts,Point * normales,double r);

#endif
