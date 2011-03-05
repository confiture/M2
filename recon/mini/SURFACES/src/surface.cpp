#include "surface.hpp"

grille calc_grille_dist(ULONG n,Point * pts,Point * normales,double pas){
	double xmin,xmax,ymin,ymax,zmin,zmax;

	boite_englobante(pts,n,xmin,xmax,ymin,ymax,zmin,zmax);
	xmin-=2*pas;xmax+=2*pas;ymin-=2*pas;ymax+=2*pas;zmin-=2.0*pas;zmax+=2.0*pas;

	// On calcule les dimensions de la grille
	int dimx=(int)((xmax-xmin)/pas+0.5);
	int dimy=(int)((ymax-ymin)/pas+0.5);
	int dimz=(int)((zmax-zmin)/pas+0.5);

	double*** grDist=new double**[dimx];// grille des distances des points de la grille aux points reels
	for(int i=0;i<dimx;i++){
		grDist[i]=new double*[dimy];
		for(int j=0;j<dimy;j++){
			grDist[i][j]=new double[dimz];
		}
	}

	// grille des distances signees que l'on va renvoyer
	grille grDistSig(dimx,dimy,dimz,pas,xmin,xmax,ymin,ymax,zmin,zmax);

	for(int i=0;i<dimx;i++){
		for(int j=0;j<dimy;j++){
			for(int k=0;k<dimz;k++){
				grDist[i][j][k]=numeric_limits<double>::infinity();
				grDistSig(i,j,k)=numeric_limits<double>::infinity();
			}
		}
	}

	// On calcule les distances minimum des points de la grilles aux points du tableau pts
	for(int i=0;i<n;i++){
		int indi;// indice en i du point le plus proche de pts[i] dans la grille reguliere grDist
		int indj;// indice en j du point le plus proche de pts[i] dans la grille reguliere grDist
		int indk;// indice en k du point le plus proche de pts[i] dans la grille reguliere grDist

		// On calcule le point de la grille reguliere grDist le plus proche de pts[i]
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
		// maintenant (indi,indj,indk) minimise norm(pts-(indi*pas+xmin,indi*pas+ymin,indk*pas+zmin))

		// On parcourt les 26 voisins de grDist[indi][indj][indk] et on met à jour
		// les distances minimum si nécessaire
		for(int ig=-1;ig<=1;ig++){
			for(int jg=-1;jg<=1;jg++){
				for(int kg=-1;kg<=1;kg++){
					Point ptGrille(indi*pas+ig*pas+xmin,
					               indj*pas+jg*pas+ymin,
					               indk*pas+kg*pas+zmin);

					double d=distance_point2(ptGrille,pts[i]);

					if(d<grDist[indi+ig][indj+jg][indk+kg])grDist[indi+ig][indj+jg][indk+kg]=d;
				}
			}
		}
	}

	for(int i=0;i<n;i++){
		int indi;// indice en i du point le plus proche de pts[i] dans la grille reguliere grDistSig
		int indj;// indice en j du point le plus proche de pts[i] dans la grille reguliere grDistSig
		int indk;// indice en k du point le plus proche de pts[i] dans la grille reguliere grDistSig

		// On calcule le point de la grille reguliere grDistSig le plus proche de pts[i]
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
		// maintenant (indi,indj,indk) minimise norm(pts-(indi*pas+xmin,indi*pas+ymin,indk*pas+zmin))

		// On parcourt les 26 voisins de grDistSig[indi][indj][indk] et on met à jour
		// les distances minimum si nécessaire
		for(int ig=-1;ig<=1;ig++){
			for(int jg=-1;jg<=1;jg++){
				for(int kg=-1;kg<=1;kg++){
					Point ptGrille(indi*pas+ig*pas+xmin,
					               indj*pas+jg*pas+ymin,
					               indk*pas+kg*pas+zmin);

					double d=distance_point2(ptGrille,pts[i]);

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



void grille::calc_iso_surf(double v,std::list<Triangle> & T,std::list<Point> & S){
	T.clear();
	S.clear();
	int numSommet=0;// les numéros des sommets de la liste S

	for(int i=1;i<this->nl-1;i++){
		for(int j=1;j<this->nc-1;j++){// boucle sur les mailles
			for(int k=1;k<this->nt-1;k++){
				Point4 p[8];/* coordonnées des sommets du cube élémentaire */

				/* calcul du cube elementaire : 8 sommets (m=0 à 7) */
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
				p[0]=Point4(i    *pas+xmin,(j+1)*pas+ymin,k*pas+zmin,(*this)(i  ,j+1,k));//la 4eme dimension des points est valeur de la grille au point courant
				p[1]=Point4((i+1)*pas+xmin,(j+1)*pas+ymin,k*pas+zmin,(*this)(i+1,j+1,k));
				p[2]=Point4((i+1)*pas+xmin,j    *pas+ymin,k*pas+zmin,(*this)(i+1,j  ,k));
				p[3]=Point4(i    *pas+xmin,j    *pas+ymin,k*pas+zmin,(*this)(i  ,j  ,k));

				p[4]=Point4(i    *pas+xmin,(j+1)*pas+ymin,(k+1)*pas+zmin,(*this)(i  ,j+1,k+1));
				p[5]=Point4((i+1)*pas+xmin,(j+1)*pas+ymin,(k+1)*pas+zmin,(*this)(i+1,j+1,k+1));
				p[6]=Point4((i+1)*pas+xmin,j    *pas+ymin,(k+1)*pas+zmin,(*this)(i+1,j  ,k+1));
				p[7]=Point4(i    *pas+xmin,j    *pas+ymin,(k+1)*pas+zmin,(*this)(i  ,j  ,k+1));

				// on fabrique un tableau de 6 tetraedres en dimension 4 qui décompose la maille courante
				Tetraedre4 trs[6];
				{
					int s;
					s=0;//on teste si l'hyperplan w=v intersecte la maille courante
					for (int is=0; is<8; is++)
						s+=p[is].w<v;

					if (s>0 && s<8)// si c'est le cas, on décompose la maille courante en 6 tétraèdres
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

						// on parcourt tous les tetraedres pour calculer leur intersection
						// avec l'hyperplan w=v
						for(int iTrs=0;iTrs<6;iTrs++){
						// on vérifie qu'aucun des points ne vaut l'infini, auquel cas cela signifie
							// qu'il n'a jamais été initialisé
							if(trs[iTrs].p[0].w<numeric_limits<double>::infinity() &&
							   trs[iTrs].p[1].w<numeric_limits<double>::infinity() &&
							   trs[iTrs].p[2].w<numeric_limits<double>::infinity() &&
							   trs[iTrs].p[3].w<numeric_limits<double>::infinity())
							{


								s=0;// on teste si le tétraèdre courant trs[iTrs] est intersecté par l'hyperplan w=v
								for(int is=0;is<4;is++)
									s+=trs[iTrs].p[is].w<v;

								if(s>0 && s<4){//il y a alors intersection avec l'hyperplan w=v
									Point4 pts[4];
									int length;

									//on calcule l'intersection du tétraèdre trs[iTrs] avec l'hyperplan w=v
									intersectionTetra4(trs[iTrs],v,pts,length);

									// on ajoute les points intersectés a la liste des sommets S
									for(int is=0;is<3;is++)
										S.push_back(Point(pts[is].x,pts[is].y,pts[is].z));

									// on fabrique le triangle qui est l'intersection de l'hyperplan avec le tétraèdre courant
									Triangle tri3d;
									tri3d.s1=numSommet;
									tri3d.s2=numSommet+1;
									tri3d.s3=numSommet+2;

									T.push_back(tri3d);

									//s'il y a 4 points intersectés dans le tétraèdre, on fait alors tous les triangles possibles
									// avec ces 4 points qu'on ajoute à la liste des triangles
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
}

/**
 *Retourne l'intersection du  avec l'hyperplan plan w=v.
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
	double r2=r*r;

	// B : barycentre et v vecteur normale au plan
	Point B,v;

	std::cout<<"calcul normales non orientees"<<std::endl;
	// on parcourt tous les points
	for(int i=0;i<n;i++){
		std::list<Point> voisinage; // voisinage de pts[i], c'est à dire tous les points pts[j] tels
		                            // que distance(pts[i],pts[j])<=r
		for(int j=0;j<n;j++){
			if(i!=j && distance_point2(pts[i],pts[j])<=r2){
				voisinage.push_back(pts[j]);
			}

		}

		// evaluation de la normale grace à la fonction qui calcule le plan
		// aux moindres carres des points dans voisinage, la normale du
		// du plan est la normale du point
		plan_mc(voisinage,B,normales[i]);
	}

	// on compte le nombre d'aretes
	int nA = 0;
	for (int i=0; i<n; i++)
		for (int j=i+1; j<n; j++)
			if (distance_point2(pts[i],pts[j])<=r2)
				nA ++;

	// On fabrique le graphe G de l'algorithme
	Arete* aretes=new Arete[nA];
	std::cout<<"calcul des aretes"<<std::endl;
	int k=0;
	for(int i=0;i<n-1;i++){
		for(int j=i+1;j<n;j++){
			if(distance_point2(pts[i],pts[j])<=r2){
				Arete ac;
				ac.v=1-ABS(dot(pts[i],pts[j]));
				ac.s1=i;
				ac.s2=j;

				aretes[k]=ac;
				k++;
			}
		}
	}

	std::cout<<"tri des aretes"<<std::endl;
	Arete* begin=&aretes[0];
	Arete* end=&aretes[nA-1];
	end++;

	// on ordonne les aretes selon leur cout
	sort(begin,end);

	std::cout<<"calcul ACM"<<std::endl;
	Graphe G = {aretes,nA,n};
	Arbre ACM = calcule_ACM(G);// on calcule l'arbre couvrant minimal

	bool visite[n];
	for(int i=0;i<n;i++)
		visite[i]=false;

	// on oriente les normales
	std::cout<<"orientation normales"<<std::endl;
	oriente_normales(ACM,0,normales,visite);

	delete[] aretes;
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
