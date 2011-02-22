#include "surface.hpp"

grille calc_grille_dist(ULONG n,Point * pts,Point * normales){
	double pas=calc_pas(n,pts);
	double xmin,xmax,ymin,ymax,zmin,zmax;

	boite_englobante(pts,n,xmin,xmax,ymin,ymax,zmin,zmax);
	xmin-=2*pas;xmax+=2*pas;ymin-=2*pas;ymax+=2*pas;zmin-=2*pas;zmax+=2*pas;

	int dimx=(int)((xmax-xmin)/pas+0.5);
	int dimy=(int)((ymax-ymin)/pas+0.5);
	int dimz=(int)((zmax-zmin)/pas+0.5);

	double*** grDist=new double**[dimx];
	for(int i=0;i<dimx;i++){
		grDist[i]=new double*[dimy];
		for(int j=0;j<dimy;j++){
			grDist[i][j]=new double[dimz];
		}
	}

	grille grDistSig(dimx,dimy,dimz,pas,xmin,xmax,ymin,ymax,zmin,zmax);

	for(int i=0;i<dimx;i++){
		for(int j=0;j<dimy;j++){
			for(int k=0;k<dimz;k++){
				grDist[i][j][k]=numeric_limits<double>::infinity();
				grDistSig(i,j,k)=numeric_limits<double>::infinity();
			}
		}
	}

	for(int i=0;i<n;i++){
		int indi;
		int indj;
		int indk;
		if(pts[i].x-((int)((pts[i].x-xmin)/pas))*pas-xmin<=pas/2.0){
			indi=((int)((pts[i].x-xmin)/pas));
		}
		else{
			indi=1+((int)((pts[i].x-xmin)/pas));
		}

		if(pts[i].y-((int)((pts[i].y-ymin)/pas))*pas-ymin<=pas/2.0){
			indj=((int)((pts[i].y-ymin)/pas));
		}
		else{
			indj=1+((int)((pts[i].y-ymin)/pas));
		}

		if(pts[i].z-((int)((pts[i].z-zmin)/pas))*pas-zmin<=pas/2.0){
			indk=((int)((pts[i].z-zmin)/pas));
		}
		else{
			indk=1+((int)((pts[i].z-zmin)/pas));
		}

		std::cout<<"indi*tralalal "<<indi*pas+xmin<<std::endl;
		std::cout<<"pts[i].x "<<pts[i].x<<std::endl;
		std::cout<<"indj*tralalal "<<indj*pas+ymin<<std::endl;
		std::cout<<"pts[i].y "<<pts[i].y<<std::endl;
		std::cout<<"indk*tralalal "<<indk*pas+zmin<<std::endl;
		std::cout<<"pts[i].z "<<pts[i].z<<std::endl;



		for(int ig=-1;ig<=1;ig++){
			for(int jg=-1;jg<=1;jg++){
				for(int kg=-1;kg<=1;kg++){
					Point ptGrille(indi*pas+ig*pas+xmin,
					               indj*pas+jg*pas+ymin,
					               indk*pas+kg*pas+zmin);

					double d=norm(ptGrille-pts[i]);

					if(d<grDist[indi+ig][indj+jg][indk+kg])grDist[indi+ig][indj+jg][indk+kg]=d;
				}
			}
		}
	}

	for(int i=0;i<n;i++){
		int indi;
		int indj;
		int indk;
		if(pts[i].x-((int)((pts[i].x-xmin)/pas))*pas-xmin<=pas/2.0){
			indi=((int)((pts[i].x-xmin)/pas));
		}
		else{
			indi=1+((int)((pts[i].x-xmin)/pas));
		}

		if(pts[i].y-((int)((pts[i].y-ymin)/pas))*pas-ymin<=pas/2.0){
			indj=((int)((pts[i].y-ymin)/pas));
		}
		else{
			indj=1+((int)((pts[i].y-ymin)/pas));
		}

		if(pts[i].z-((int)((pts[i].z-zmin)/pas))*pas-zmin<=pas/2.0){
			indk=((int)((pts[i].z-zmin)/pas));
		}
		else{
			indk=1+((int)((pts[i].z-zmin)/pas));
		}


		for(int ig=-1;ig<=1;ig++){
			for(int jg=-1;jg<=1;jg++){
				for(int kg=-1;kg<=1;kg++){
					Point ptGrille(indi*pas+ig*pas+xmin,
					               indj*pas+jg*pas+ymin,
					               indk*pas+kg*pas+zmin);

					double d=norm(ptGrille-pts[i]);

					if(d<=grDist[indi+ig][indj+jg][indk+kg])grDistSig(indi+ig,indj+jg,indk+kg)=dot(ptGrille-pts[i],normales[i])/dot(normales[i],normales[i]);
				}
			}
		}
	}

	for(int i=0;i<dimx;i++){
		for(int j=0;j<dimy;j++){
			delete[] grDist[i][j];
		}
		delete[] grDist[i];
	}
	delete[] grDist;


	return grDistSig;
}



void grille::calc_iso_courbe(double v,std::list<Triangle> & T,std::list<Point> & S){
	// allocation mémoire des tableaux E1 et E2
	T.clear();
	S.clear();
	int numSommet=0;// les numéros des sommets de la liste S

	std::cout<<"nl "<<nl<<std::endl;
	std::cout<<"nc "<<nc<<std::endl;
	std::cout<<"nt "<<nt<<std::endl;
	for(int i=1;i<this->nl-1;i++){
		for(int j=1;j<this->nc-1;j++){// boucle sur les mailles
			for(int k=1;k<this->nt-1;k++){
				Point4 p[8];/* coordonnées des sommets du cube élémentaire */

				/* calcul du cube elementaire : 8 sommets (m=0 à 7) */
				/*   p[m]   : coordonnées du sommet m               */
				/*   val[m] : valeur au sommet m                    */
				/* le sommet m du cube élémentaire correspond au    */
				/* point P(im,jm,km) de la grille de données        */
				/* les 6 faces du cube sont :                       */
				/*      7------------4          z                   */
				/*     /|           /|          |                   */
				/*    / |          / |          |                   */
				/*   6------------5  |          |                   */
				/*   |  |         |  |          |                   */
				/*   |  |         |  |          |                   */
				/*   |  |         |  |          |                   */
				/*   |  3---------|--0          .------------y      */
				/*   | /          | /          /                    */
				/*   |/           |/          /                     */
				/*   2------------1          x                      */
				p[0]=Point4(i    *pas+xmin,(j+1)*pas+ymin,k*pas+zmin,(*this)(i  ,j+1,k));
				p[1]=Point4((i+1)*pas+xmin,(j+1)*pas+ymin,k*pas+zmin,(*this)(i+1,j+1,k));
				p[2]=Point4((i+1)*pas+xmin,j    *pas+ymin,k*pas+zmin,(*this)(i+1,j  ,k));
				p[3]=Point4(i    *pas+xmin,j    *pas+ymin,k*pas+zmin,(*this)(i  ,j  ,k));

				p[4]=Point4(i    *pas+xmin,(j+1)*pas+ymin,(k+1)*pas+zmin,(*this)(i  ,j+1,k+1));
				p[5]=Point4((i+1)*pas+xmin,(j+1)*pas+ymin,(k+1)*pas+zmin,(*this)(i+1,j+1,k+1));
				p[6]=Point4((i+1)*pas+xmin,j    *pas+ymin,(k+1)*pas+zmin,(*this)(i+1,j  ,k+1));
				p[7]=Point4(i    *pas+xmin,j    *pas+ymin,(k+1)*pas+zmin,(*this)(i  ,j  ,k+1));

				/* découpage du cube élémentaire */
				/* calcul des triangles correspondants au cube élémentaire */
				/* DEBUT PARTIE A MODIFIER */

				/* PARTIE A ENLEVER/MODIFIER PAR LA SUITE            */
				/* exemple d'ajout de triangle :                     */
				/* ajout d'1 triangle par cube intersecté            */
				/* par la surface iso-valeur                         */
				/* i.e. cube avec changement de signe en ses sommets */

				Tetraedre4 trs[6];
				{
					int s;
					s=0;
					for (int is=0; is<8; is++)
						s+=p[is].w<v;

					if (s>0 && s<8)
					{
						trs[0].p[0]=p[2];
						trs[0].p[1]=p[1];
						trs[0].p[2]=p[3];
						trs[0].p[3]=p[6];

						trs[1].p[0]=p[1];
						trs[1].p[1]=p[3];
						trs[1].p[2]=p[6];
						trs[1].p[3]=p[0];

						trs[2].p[0]=p[7];
						trs[2].p[1]=p[0];
						trs[2].p[2]=p[3];
						trs[2].p[3]=p[6];

						trs[3].p[0]=p[7];
						trs[3].p[1]=p[4];
						trs[3].p[2]=p[5];
						trs[3].p[3]=p[0];

						trs[4].p[0]=p[7];
						trs[4].p[1]=p[5];
						trs[4].p[2]=p[6];
						trs[4].p[3]=p[0];

						trs[5].p[0]=p[5];
						trs[5].p[1]=p[6];
						trs[5].p[2]=p[1];
						trs[5].p[3]=p[0];

						for(int iTrs=0;iTrs<6;iTrs++){
							if(trs[iTrs].p[0].w<numeric_limits<double>::infinity() &&
							   trs[iTrs].p[1].w<numeric_limits<double>::infinity() &&
							   trs[iTrs].p[2].w<numeric_limits<double>::infinity() &&
							   trs[iTrs].p[3].w<numeric_limits<double>::infinity())
							{

								s=0;
								for(int is=0;is<4;is++)
									s+=trs[iTrs].p[is].w<v;

								if(s>0 && s<4){//il y a donc intersection avec l'isosurface
									Point4 pts[4];
									int length;

									intersectionTetra4(trs[iTrs],v,pts,length);

									/* le cube élémentaire intersecte l'iso-surface   */
									/* -> affichage d'une face du cube, cette face    */
									/* est décomposée en 2 triangles                  */

									/* affichage de la face [0 1 2 3]        */
									/* -> ajout du triangle (p[0],p[1],p[2]) */
									/* -> ajout du triangle (p[0],p[2],p[3]) */

									for(int is=0;is<3;is++)
										S.push_back(Point(pts[is].x,pts[is].y,pts[is].z));

									Triangle tri3d;
									tri3d.s1=numSommet;
									tri3d.s2=numSommet+1;
									tri3d.s3=numSommet+2;

									T.push_back(tri3d);

									if(length==4){
										S.push_back(Point(pts[3].x,pts[3].y,pts[3].z));

										tri3d.s1=numSommet;
										tri3d.s3=numSommet+2;
										tri3d.s3=numSommet+3;
										T.push_back(tri3d);

										tri3d.s1=numSommet;
										tri3d.s3=numSommet+1;
										tri3d.s3=numSommet+3;
										T.push_back(tri3d);

										tri3d.s3=numSommet+1;
										tri3d.s1=numSommet+2;
										tri3d.s3=numSommet+3;
										T.push_back(tri3d);

										numSommet++;
									}
									numSommet+=3;
								}
							}
						}
					}
				}
			}
		}
	}

	std::cout<<" numSommet "<<numSommet<<std::endl;
	std::cout<<"sizeOfSlist "<<S.size()<<std::endl;
}

/**
 *Retourne l'intersection du  avec le plan z=v.
 *
 */
void intersectionTetra4(Tetraedre4 te,double v,Point4* tr,int & taille){
	taille=0;

	for(int i=0;i<3;i++){
		for(int j=i+1;j<4;j++){
			if((te.p[i].w-v)*(te.p[j].w-v)<=0){
				tr[taille]=intersection(te.p[i],te.p[j],v);
				taille++;
			}
		}
	}
}


void normales(int n,Point * pts,Point * normales,double r){
	double** dist=new double*[n];
	for(int i=0;i<n;i++)
		dist[i]=new double[n];

	Point B,v;

	for(int i=0;i<n;i++){
		dist[i][i]=0;
		for(int j=i+1;j<n;j++){
				dist[i][j]=distance_point(pts[i],pts[j]);
				dist[j][i]=distance_point(pts[i],pts[j]);
		}
	}

	for(int i=0;i<n;i++){
		std::list<Point> voisinage;
		for(int j=0;j<n;j++){
			if(i!=j && dist[i][j]<=r){
				voisinage.push_back(pts[j]);
			}
		}

		plan_mc(voisinage,B,normales[i]);
	}

	int nA = 0;
	for (int i=0; i<n; i++)
		for (int j=i+1; j<n; j++)
			if (dist[i][j]<=r)
				nA ++;

	Arete aretes[nA];

	int k=0;
	for(int i=0;i<n-1;i++){
		for(int j=i+1;j<n;j++){
			if(dist[i][j]<=r){
				Arete ac;
				ac.v=1-ABS(dot(pts[i],pts[j]));
				ac.s1=i;
				ac.s2=j;

				aretes[k]=ac;
				k++;
			}
		}
	}

	for(int i=0;i<n;i++)
		delete[] dist[i];
	delete[] dist;

	Arete* begin=&aretes[0];
	Arete* end=&aretes[nA-1];
	end++;

	sort(begin,end);

	Graphe G = {aretes,nA,n};
	Arbre ACM = calcule_ACM(G);

	bool visite[n];
	for(int i=0;i<n;i++)
		visite[i]=false;

	oriente_normales(ACM,0,normales,visite);
}

void oriente_normales(Arbre ACM,int numNoeud,Point * normales,bool * visite){
	if(!visite[numNoeud]){
		visite[numNoeud]=true;

		int n=ACM.noeuds[numNoeud].voisins.size();
		for(int i=0;i<n;i++){
			if(dot(normales[numNoeud],normales[ACM.noeuds[numNoeud].voisins[i]]) < 0)
				normales[ACM.noeuds[numNoeud].voisins[i]]=-1*normales[ACM.noeuds[numNoeud].voisins[i]];

			oriente_normales(ACM,ACM.noeuds[numNoeud].voisins[i],normales,visite);
		}
	}
}

Point4 operator+(Point4 A, Point4 B)
{
	return Point4(A.x+B.x, A.y+B.y, A.z+B.z, A.w+B.w);
}

// calcul de A-B avec A,B de type Point4
Point4 operator-(Point4 A, Point4 B)
{
	return Point4(A.x-B.x, A.y-B.y, A.z-B.z, A.w-B.w);
}

// calcul de r*A avec r de type double et A de type Point4
Point4 operator*(double r, Point4 A)
{
	return Point4(r*A.x, r*A.y, r*A.z, r*A.w);
}

// calcul de A*r avec r de type double et A de type Point4
Point4 operator*(Point4 A, double r)
{
	return Point4(A.x*r, A.y*r, A.z*r, A.w*r);
}

// calcul de A/r avec r de type double et A de type Point4
Point4 operator/(Point4 A, double r)
{
	return Point4(A.x/r, A.y/r, A.z/r, A.w/r);
}
