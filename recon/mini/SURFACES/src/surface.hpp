#ifndef _SURFACE_HPP
#define _SURFACE_HPP

#include "lib_base3d.hpp"
#include <limits>
#include <list>
#include <iostream>
#include <vector>
#include <algorithm>


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

struct grille{
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

	~grille(){
		for(int i=0;i<nl;i++){
			for(int j=0;j<nc;j++){
				delete[] vals[i][j];
			}
			delete[] vals[i];
		}
		delete[] vals;
	}

	double& operator()(int i,int j,int k){
		return vals[i][j][k];
	}

	void calc_iso_courbe(double v,std::list<Triangle> & T,std::list<Point> & S);

	int nl;
	int nc;
	int nt;
	double pas;
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double*** vals;
};

struct Point4
{
	double x,y,z,w; // les 3 coordonnées
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
 *Retourne l'intersection du segment [p1,p2] avec le plan z=v.
 *
 */
inline Point4 intersection(const Point4& p1,const Point4& p2,double v){
	double a = (v-p2.w)/(p1.w-p2.w);
	return a*p1+(1-a)*p2;
}

struct Tetraedre4{
	Point4 p[4];
};

struct Triangle4{
	Point4 p[3];
};

void intersectionTetra4(Tetraedre4 te,double v,Point4* tr,int & taille);

void oriente_normales(Arbre ACM,int numNoeud,Point * normales,bool * visite);

void normales(int n,Point * pts,Point * normales,double r);

#endif
