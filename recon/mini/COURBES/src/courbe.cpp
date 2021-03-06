#include "courbe.hpp"
#include"xfig.hpp"

grille calc_grille_dist(int n,Point * pts,Point * normales,double pas){
	double xmin,xmax,ymin,ymax;

	boite_englobante(pts,n,xmin,xmax,ymin,ymax);
	xmin-=2*pas;xmax+=2*pas;ymin-=2*pas;ymax+=2*pas;

	// On calcule les dimensions de la grille
	int dimx=(int)((xmax-xmin)/pas+0.5);
	int dimy=(int)((ymax-ymin)/pas+0.5);

	double grDist[dimy][dimx];// grille des distances des points de la grille aux points reels
	grille grDistSig(dimy,dimx,pas,xmin,xmax,ymin,ymax);// grille des distances signees que l'on va renvoyer

	// initialisation, on initialise toutes les distances a l'infini
	for(int i=0;i<dimy;i++){
		for(int j=0;j<dimx;j++){
			grDist[i][j]=numeric_limits<double>::infinity();
			grDistSig(i,j)=numeric_limits<double>::infinity();
		}
	}

	// On calcule les distances minimum des points de la grilles aux points du tableau pts,
	// c'est à dire le tableau grDist
	for(int i=0;i<n;i++){
		int indi;// indice en i du point le plus proche de pts[i] dans la grille reguliere grDist
		int indj;// indice en j du point le plus proche de pts[i] dans la grille reguliere grDist

		// On calcule le point de la grille reguliere grDist le plus proche de pts[i]
		if(pts[i].x-((int)((pts[i].x-xmin)/pas))*pas-xmin<=pas/2.0){
			indj=((int)((pts[i].x-xmin)/pas));
		}
		else{
			indj=1+((int)((pts[i].x-xmin)/pas));
		}

		if(pts[i].y-((int)((pts[i].y-ymin)/pas))*pas-ymin<=pas/2.0){
			indi=((int)((pts[i].y-ymin)/pas));
		}
		else{
			indi=1+((int)((pts[i].y-ymin)/pas));
		}
		// maintenant minimise norm(pts-(indj*pas+xmin,indi*pas+ymin))

		// On parcourt les 9 voisins de grDist[indi][indj] et on met à jour
		// les distances minimum de grDist si nécessaire
		for(int ig=-1;ig<=1;ig++){
			for(int jg=-1;jg<=1;jg++){
				Point ptGrille(indj*pas+jg*pas+xmin,
				               indi*pas+ig*pas+ymin);

				double d=norm(ptGrille-pts[i]);

				if(d<grDist[indi+ig][indj+jg])grDist[indi+ig][indj+jg]=d;
			}
		}
	}

	for(int i=0;i<n;i++){
		int indi;// indice en i du point le plus proche de pts[i] dans la grille reguliere grDistSig
		int indj;// indice en j du point le plus proche de pts[i] dans la grille reguliere grDistSig

		// On calcule le point de la grille reguliere grDistSig le plus proche de pts[i]
		if(pts[i].x-((int)((pts[i].x-xmin)/pas))*pas-xmin<=pas/2.0){
			indj=((int)((pts[i].x-xmin)/pas));
		}
		else{
			indj=1+((int)((pts[i].x-xmin)/pas));
		}

		if(pts[i].y-((int)((pts[i].y-ymin)/pas))*pas-ymin<=pas/2.0){
			indi=((int)((pts[i].y-ymin)/pas));
		}
		else{
			indi=1+((int)((pts[i].y-ymin)/pas));
		}
		// maintenant (indi,indj) minimise norm(pts-(indj*pas+xmin,indi*pas+ymin))

		// On parcourt les 9 voisins de grDistSig[indi][indj] et on met à jour
		// les distances minimum de grDistSig si nécessaire
		for(int ig=-1;ig<=1;ig++){
			for(int jg=-1;jg<=1;jg++){
				Point ptGrille(indj*pas+jg*pas+xmin,
				               indi*pas+ig*pas+ymin);

				double d=norm(ptGrille-pts[i]);

				if(d<=grDist[indi+ig][indj+jg])grDistSig(indi+ig,indj+jg)=dot(ptGrille-pts[i],normales[i])/dot(normales[i],normales[i]);
			}
		}
	}


	return grDistSig;
}



void grille::calc_iso_courbe(double v,vPoint & E1,vPoint & E2){
	// allocation mémoire des tableaux E1 et E2
	E1.resize((this->nl)*(this->nc)*2);
	E2.resize((this->nl)*(this->nc)*2);

	int N=0;// le nombre de segments
	bool e2; // booléen qui indique si on est sur le deuxième sommet du segment (E2[N])

	Point3 T[2][3];// les triangles maillant la maille rectangulaire courante
	Point3 pt1,pt2,pt3,pt4;// les 4 points de la maille rectangulaire courante
	Point3 inter;

	//On parcourt tous les points de la grille
	for(int i=1;i<this->nl-1;i++){
		for(int j=1;j<this->nc-1;j++){// boucle sur les mailles
			// les vraies coordonnees des quatre coins de la maille
			pt1=Point3(j*pas+xmin,i*pas+ymin,(*this)(i,j));
			pt2=Point3(j*pas+xmin,(i+1)*pas+ymin,(*this)(i+1,j));
			pt3=Point3((j+1)*pas+xmin,(i+1)*pas+ymin,(*this)(i+1,j+1));
			pt4=Point3((j+1)*pas+xmin,i*pas+ymin,(*this)(i,j+1));

			// a partir des quatre coins de la maille, on fabrique deux triangles T[0] et T[1] qui
			// decoupent la maille courante en deux triangles
			T[0][0]=pt1;T[0][1]=pt4;T[0][2]=pt3;// premier triangle
			T[1][0]=pt1;T[1][1]=pt2;T[1][2]=pt3;// deuxième triangle

			//On boucle sur les deux triangles
			for(int iT=0;iT<2;iT++){
				e2=false;// on initialise e2 a false ce qui veut dire que l'on a pas encore trouver le premier point d'intersection
				int ar=0;
				for(ar=0;ar<3;ar++){ //boucle sur les arêtes du triangle T[iT]
					if(T[iT][ar].z != numeric_limits<double>::infinity() && T[iT][(ar+1)%3].z != numeric_limits<double>::infinity()){
						if(T[iT][ar].z==v && T[iT][(ar+1)%3].z==v){ // cas où l'arête est sur le plan
							E1[N]=Point(T[iT][ar].x,T[iT][ar].y);
							E2[N]=Point(T[iT][(ar+1)%3].x,T[iT][(ar+1)%3].y);
							N++;
							break;
						}
						else if((T[iT][ar].z-v)*(T[iT][(ar+1)%3].z-v)<0 || T[iT][ar].z-v==0){ // cas où le plan intersecte l'arête en un seul point
							inter=intersection(T[iT][ar],T[iT][(ar+1)%3],v);
							if(e2){// si e2 alors le premier point d'intersection a ete trouve, il s'agit donc du deuxième point d'intersection
								E2[N]=Point(inter.x,inter.y);
								N++;
								break; // on a trouvé le deuxième point d'intersection, on arete donc la boucle sur les aretes du triangle
							}
							else{// on est sur le premier point d'intersection
								E1[N]=Point(inter.x,inter.y);
								e2=true; // e2=true car on a trouvé le premier point d'intersection qu'on stock dans E1
							}
						}
					}
				}
			}
		}
	}

	// On réduit la taille des vecteurs à N
	E1.resize(N);
	E2.resize(N);
}


void normales(int n,Point * pts,Point * normales,double r){
	double dist[n][n];
	Point B,v;

	// calcul des distances entre les points de pts
	for(int i=0;i<n;i++){
		dist[i][i]=0;
		for(int j=i+1;j<n;j++){
				dist[i][j]=distance_point(pts[i],pts[j]);
				dist[j][i]=distance_point(pts[i],pts[j]);
		}
	}

	// on parcourt tous les points pour calculer les normales
	for(int i=0;i<n;i++){
		std::list<Point> voisinage; // voisinage de pts[i], c'est à dire tous les points pts[j] tels
		                            // que distance(pts[i],pts[j])<=r
		for(int j=0;j<n;j++){
			if(i!=j && dist[i][j]<=r){
				voisinage.push_back(pts[j]);
			}
		}

		// evaluation de la normale grace à la fonction qui calcule la droite
		// aux moindres carres des points dans voisinage, la normale des
		// du vecteur directeur de la droite est la normale du point
		droite_mc(voisinage,B,v,normales[i]);
	}

	// on compte le nombre d'aretes
	int nA = 0;
	for (int i=0; i<n; i++)
		for (int j=i+1; j<n; j++)
			if (dist[i][j]<=r)
				nA ++;


	// On fabrique le graphe G de l'algorithme
	Arete aretes[nA];

	int k=0;
	for(int i=0;i<n-1;i++){
		for(int j=i+1;j<n;j++){
			if(dist[i][j]<=r){
				Arete ac;
				ac.v=1-ABS(dot(normales[i],normales[j]));
				ac.s1=i;
				ac.s2=j;

				aretes[k]=ac;
				k++;
			}
		}
	}

	Arete* begin=&aretes[0];
	Arete* end=&aretes[nA-1];
	end++;

	// on ordonne les aretes selon leur cout
	sort(begin,end);

	Graphe G = {aretes,nA,n};
	Arbre ACM = calcule_ACM(G);// on calcule l'arbre couvrant minimal

	bool visite[n];
	for(int i=0;i<n;i++)
		visite[i]=false;

	// on oriente les normales
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
