#include "grid.hpp"


grille calc_grille_dist(int n,Point * pts,Point * normales){
	double pas=calc_pas(n,pts);
	double xmin,xmax,ymin,ymax;

	boite_englobante(pts,n,xmin,xmax,ymin,ymax);
	xmin-=2*pas;xmax+=2*pas;ymin-=2*pas;ymax+=2*pas;

	int dimx=(int)((xmax-xmin)/pas+0.5);
	int dimy=(int)((ymax-ymin)/pas+0.5);

	grille grDist(dimy,dimx,pas,xmin,xmax,ymin,ymax);

	for(int i=0;i<dimy;i++){
		for(int j=0;j<dimx;j++){
			grDist(i,j)=numeric_limits<double>::infinity();
		}
	}

	for(int i=0;i<n;i++){
		int indi;
		int indj;
		std::cout<<"=========="<<std::endl;
		if(pts[i].x-((int)((pts[i].x-xmin)/pas))*pas-xmin<=pas/2.0){
			indj=((int)((pts[i].x-xmin)/pas));
			std::cout<<"pts[i].x "<<pts[i].x<<std::endl;
			std::cout<<"((int)((pts[i].x-xmin)/pas))*pas+xmin "
			         <<((int)((pts[i].x-xmin)/pas))*pas+xmin
			         <<std::endl;
			std::cout<<"cas 1 x"<<std::endl;
		}
		else{
			indj=1+((int)((pts[i].x-xmin)/pas));
			std::cout<<"pts[i].x "<<pts[i].x<<std::endl;
			std::cout<<"((int)((pts[i].x-xmin)/pas))*pas+xmin "
			         <<((int)((pts[i].x-xmin)/pas))*pas+xmin
			         <<std::endl;
			std::cout<<"cas 2 x"<<std::endl;
		}

		if(pts[i].y-((int)((pts[i].y-ymin)/pas))*pas-ymin<=pas/2.0){
			indi=((int)((pts[i].y-ymin)/pas));
			std::cout<<"cas 1 y"<<std::endl;
		}
		else{
			indi=1+((int)((pts[i].y-ymin)/pas));
			std::cout<<"pts[i].y "<<pts[i].y<<std::endl;
			std::cout<<"(1+(int)((pts[i].y-ymin)/pas))*pas+ymin "
			         <<(1+(int)((pts[i].y-ymin)/pas))*pas+ymin
			         <<std::endl;
			std::cout<<"cas 2 y"<<std::endl;
		}

		std::cout<<"verifs===="
		         <<std::endl
		         <<"indi "<<indi<<std::endl
		         <<"indj "<<indj<<std::endl
		         <<"dimy "<<dimy<<std::endl
		         <<"dimx "<<dimx<<std::endl
		         <<"====verifs"<<std::endl;

		for(int ig=-1;ig<=1;ig++){
			for(int jg=-1;jg<=1;jg++){
				Point ptGrille(indj*pas+jg*pas+xmin,
				               indi*pas+ig*pas+ymin);

				double d=dot(ptGrille-pts[i],normales[i])/dot(normales[i],normales[i]);

				if(d<grDist(indi+ig,indj+jg))grDist(indi+ig,indj+jg)=d;
				std::cout<<"not here2"<<std::endl;
			}
		}
	}

	return grDist;
}


void grille::calc_iso_courbe(double v,vPoint & E1,vPoint & E2){
	// allocation mémoire des tableaux E1 et E2
	E1.resize((this->nl)*(this->nc)*2);
	E2.resize((this->nl)*(this->nc)*2);

	int N=0;
	bool e2; // booléen qui indique si on est sur le deuxième sommet du segment (E2[N])

	Point3 T[2][3];//les triangles
	Point3 pt1,pt2,pt3,pt4;
	Point3 inter;

	for(int i=1;i<this->nl-1;i++){
		for(int j=1;j<this->nc-1;j++){// boucle sur les mailles
			pt1=Point3(j*pas+xmin,i*pas+ymin,(*this)(i,j));
			pt2=Point3(j*pas+xmin,(i+1)*pas+ymin,(*this)(i+1,j));
			pt3=Point3((j+1)*pas+xmin,(i+1)*pas+ymin,(*this)(i+1,j+1));
			pt4=Point3((j+1)*pas+xmin,i*pas+ymin,(*this)(i,j+1));

			T[0][0]=pt1;T[0][1]=pt3;T[0][2]=pt4;
			T[1][0]=pt1;T[1][1]=pt2;T[1][2]=pt3;

			for(int iT=0;iT<2;iT++){
				e2=false;
				for(int ar=0;ar<3;ar++){ //boucle sur les arêtes
					if(T[iT][ar].z < numeric_limits<double>::infinity() && T[iT][(ar+1)%3].z < numeric_limits<double>::infinity()){
						if(T[iT][ar].z==v && T[iT][(ar+1)%3].z==v){ // cas où l'arête est sur le plan
							E1[N]=Point(T[iT][ar].x,T[iT][ar].y);;
							E2[N]=Point(T[iT][(ar+1)%3].x,T[iT][(ar+1)%3].y);
							N++;
							break;
						}
						else if((T[iT][ar].z-v)*(T[iT][(ar+1)%3].z-v)<=0){ // cas où le plan intersecte l'arête en un seul point
							inter=intersection(T[iT][ar],T[iT][(ar+1)%3],v);
							if(e2){
								E2[N]=Point(inter.x,inter.y);
								N++;
								break;
							}
							else{
								E1[N]=Point(inter.x,inter.y);
								e2=true;
							}
						}
					}
				}
			}
		}
	}

	E1.resize(N);
	E2.resize(N);
}

Point3 operator+(Point3 A, Point3 B)
{
	return Point3(A.x+B.x, A.y+B.y, A.z+B.z);
}

// calcul de A-B avec A,B de type Point3
Point3 operator-(Point3 A, Point3 B)
{
	return Point3(A.x-B.x, A.y-B.y, A.z-B.z);
}

// calcul de r*A avec r de type double et A de type Point3
Point3 operator*(double r, Point3 A)
{
	return Point3(r*A.x, r*A.y, r*A.z);
}

// calcul de A*r avec r de type double et A de type Point3
Point3 operator*(Point3 A, double r)
{
	return Point3(A.x*r, A.y*r, A.z*r);
}

// calcul de A/r avec r de type double et A de type Point3
Point3 operator/(Point3 A, double r)
{
	return Point3(A.x/r, A.y/r, A.z/r);
}
