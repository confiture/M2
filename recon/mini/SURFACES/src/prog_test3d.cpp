//////////////////////////////////////////////////////////////////////////////
//
// TEST DE LA LIBRAIRIE LIB_BASE3D
//
// TEST DE LA LIBRAIRIE GEOMVIEW
//
// compilation :
//   make prog_test3d
//
// execution :
//   prog_test3d
//   prog_test3d 1
//     (test 1 : calcul de la plus petite valeur propre)
//     (et du vecteur propre associé d'une matrice sym. 3x3)
//
//   prog_test3d 2
//     (test 2 : ensemble de points sur une sphère + normales orientées)
//     (provenant du fichier de données donnees1.pno.txt)
//     (creation du fichier test2.list)
//     (visualisation : geomview test2.list geomview.default &)
//
//   prog_test3d 3
//     (test 3 : calcul d'un graphe de proximite et d'un arbre couvrant minimal)
//     (provenant du fichier de données donnees3.pno.txt)
//     (creation des fichiers test3_P.vect, test3_ACM.vect, test3_graphe.vect)
//     (visualisation : geomview test3* geomview.default &)
//
//   prog_test3d 4
//     (test 4 : création d'un objet composite Geomview )
//     (creation du fichier test4.list)
//     (visualisation : geomview test4.list geomview.default &)
//
//   prog_test3d 5
//     (test 5 : visualisation d'un jeu de données)
//     (l'utilisation entre le nom d'un fichier de donnees)
//     (le programme crée un fichier de points test5_P.vect)
//     (et éventuellement un fichier de vecteurs test5_N.vect
//     (contenant les normales orientées)
//     (visualisation : )
//        (geomview test5_P.vect geomview.default & )
//        (geomview test5_P.vect test5_NP.vect geomview.default & )
//
//////////////////////////////////////////////////////////////////////////////

#include"lib_base3d.hpp"
#include"geomview.hpp"
#include "surface.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <iostream>
///////////////////////////////////////////////////////////////////////////////
// lecture d'un fichier de nS points
// Entrée : nom_f = le nom du fichier à créer
// Sortie : nS = nombre de sommets
//          r = grain d'échantillonnage (r>0)
//          S = tableau de nS sommets
// La mémoire nécessaire à S est allouée dans cette routine avec new
// La routine renvoie 0 si tout se passe bien, une autre valeur sinon
int lecture_fichier_points
  (const char* nom_f, ULONG &nS, double &r, Point* &S)
{
	FILE *f;

	// ouverture du fichier
	if ((f = fopen(nom_f,"r"))==(FILE *)NULL)
		return 1;

	// lecture du nombre de points et du rayon d'échantillonnage
	fscanf(f, "%li %lf\n", &nS, &r);

	// lecture des points
	S = new Point[nS];
	for (ULONG i=0; i<nS; i++)
		fscanf(f, " %lf %lf %lf\n", &S[i].x, &S[i].y, &S[i].z);

	// fermeture du fichier
	fclose(f);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// lecture d'un fichier de nS points
// Entrée : nom_f = le nom du fichier à créer
// Sortie : nS = nombre de sommets
//          r = grain d'échantillonnage (r>0)
//          S = tableau de nS sommets
//          NS = tableau de nS normales
// La mémoire nécessaire à S et NS est allouée dans cette routine avec new
// La routine renvoie 0 si tout se passe bien, une autre valeur sinon
int lecture_fichier_points_normales_orientees
  (const char* nom_f, ULONG &nS, double &r, Point* &S, Point* &NS)
{
	FILE *f;

	// ouverture du fichier
	if ((f = fopen(nom_f,"r"))==(FILE *)NULL)
		return 1;

	// lecture du nombre de points et du rayon d'échantillonnage
	fscanf(f, "%li %lf\n", &nS, &r);

	// lecture des points
	S = new Point[nS];
	for (ULONG i=0; i<nS; i++)
		fscanf(f, " %lf %lf %lf\n", &S[i].x, &S[i].y, &S[i].z);

	// lecture des normales
	NS= new Point[nS];
	for (ULONG i=0; i<nS; i++)
		fscanf(f, " %lf %lf %lf\n", &NS[i].x, &NS[i].y, &NS[i].z);

	// fermeture du fichier
	fclose(f);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// test 1 : calcul de la plus petite valeur propre
// et du vecteur propre associé d'une matrice sym. 3x3
void test_elements_propres_min_mat_sym()
{
	double A11,A12,A13,A22,A23,A33;
	double lmin;
	Point vmin;

	printf("Calcul de la plus petite valeur propre \n");
	printf("et du vecteur propre associé d'une matrice A sym. 3x3\n");
	printf("    | A11 A12 A13 |\n");
	printf("A = | A12 A22 A23 |\n");
	printf("    | A13 A23 A33 |\n");
	printf("Entrer la valeur de A11 : "); scanf("%lf", &A11);
	printf("Entrer la valeur de A12 : "); scanf("%lf", &A12);
	printf("Entrer la valeur de A13 : "); scanf("%lf", &A13);
	printf("Entrer la valeur de A22 : "); scanf("%lf", &A22);
	printf("Entrer la valeur de A23 : "); scanf("%lf", &A23);
	printf("Entrer la valeur de A33 : "); scanf("%lf", &A33);
	elements_propres_min_mat_sym(A11,A12,A13,A22,A23,A33, lmin,vmin);
	printf("    | %15.7e %15.7e %15.7e |\n",A11,A12,A13);
	printf("A = | %15.7e %15.7e %15.7e |\n",A12,A22,A23);
	printf("    | %15.7e %15.7e %15.7e |\n",A13,A23,A33);
	printf("\n Valeur propre minimale : lmin = %15.7e\n", lmin);
	printf(" Vecteur propre associe : vmin = (%11.8f,%11.8f,%11.8f)\n",
	 vmin.x, vmin.y, vmin.z);
}


///////////////////////////////////////////////////////////////////
// test 2 : ensemble de points sur une sphère + normales orientées
// provenant du fichier de données donnees1.pno.txt
// creation des fichiers test2_P.vect, test2_N.vect
void test_trace_points_normales_orientees()
{
	printf("Création d'un ensemble de points et de normales orientées\n");

	// lecture des points et normales
	ULONG nS;
	double r;
	Point *S;
	Point *NS;
	if (lecture_fichier_points_normales_orientees
	  ("donnees1.pno.txt", nS, r, S, NS)!=0)
	{
		printf(" problème lors de la lecture de donnees1.pno.txt\n");
		return;
	}

	// ouverture du fichier Geomview
	GEOMVIEW_open("test2.list");

	// fichiers des points : en rouge, d'épaisseur 10
	GEOMVIEW_points(S, nS, Couleur(1.0,0.0,0.0), 10);

	// fichiers des normales : en noir, épaisseur 2, longueur 0.3
	GEOMVIEW_points_vects(S, NS, nS, Couleur(0.0,0.0,0.0), 2, 0.3);

	// fermeture du fichier Geomview
	GEOMVIEW_close();

	// libération de la mémoire
	delete[] S;
	delete[] NS;
}


///////////////////////////////////////////////////////////////////
// test 3 : calcul d'un graphe de proximite et d'un arbre couvrant minimal
// provenant du fichier de données donnees1.pno.txt
// creation des fichiers test3_P.vect, test3_ACM.vect, test3_graphe.vect

///////////////////////////////////////////////////////////////////
// routine de comparaison d'arete valuee pour les trier
int compar_Arete(const void *p1, const void *p2)
{
	Arete* A1 = (Arete *)p1;
	Arete* A2 = (Arete *)p2;
	return(A1->v < A2->v ? -1 : 1);
}

///////////////////////////////////////////////////////////////////
// détermine le graphe de proximite des n points P[i] a distance r
// le graphe contient les aretes telles que
//   une arete (s1,s2) avec 0<=s1<s2<n vérifie v = d(P[s1],P[s2])<=r
// les aretes du graphe sont triees par valeur v croissante
Graphe calcule_graphe_proximite(Point *P, int n, double r)
{

	// calcul du nombre d'aretes
	int nA = 0;
	for (int i=0; i<n; i++)
	for (int j=i+1; j<n; j++)
		if (distance_point(P[i],P[j])<=r)
		nA ++;

	// calcul des aretes
	Arete *A = new Arete[nA];
	int k=0;
	for (int i=0; i<n; i++)
	for (int j=i+1; j<n; j++)
		if (distance_point(P[i],P[j])<=r)
		{
				A[k].s1 = i;
				A[k].s2 = j;
				A[k].v  = distance_point(P[i],P[j]);
				k++;
		}

// 	printf("calcule_graphe_proximite - avant tri :\n");
// 	for (int i=0; i<nA; i++)
// 	{
// 		printf("%2d : (%2d,%2d) - %9.7f\n",
// 		 i, A[i].s1, A[i].s2, A[i].v);
// 	}

	// tri des aretes
	qsort(A, nA, sizeof(Arete), compar_Arete);

// 	printf("calcule_graphe_proximite - apres tri :\n");
// 	for (int i=0; i<nA; i++)
// 	{
// 		printf("%2d : (%2d,%2d) - %9.7f\n",
// 		 i, A[i].s1, A[i].s2, A[i].v);
// 	}

	Graphe G = {A,nA,n};
	return G;
}

void test_graphe_et_arbre_couvrant_minimal()
{
	printf("Création d'un graphe de proximité et d'un arbre couvrant minimal\n");

	// lecture des points et normales
	ULONG nS;
	double r;
	Point *S;
	if (lecture_fichier_points("donnees3.pno.txt", nS, r, S)!=0)
	{
		printf(" problème lors de la lecture de donnees1.pno.txt\n");
		return;
	}


	// création du graphe de proximité correspondant à r
	Graphe G = calcule_graphe_proximite(S, nS, r);

	// fichier du graphe (en bleu)
	GEOMVIEW_Graphe("test3_graphe.vect", G, S, Couleur(0.0,0.0,1.0));

	// calcul de l'arbre couvrant minimal
	Arbre A=calcule_ACM(G);

	// fichier de l'arbre (en vert foncé et épaisseur 3)
	GEOMVIEW_Arbre("test3_ACM.vect", A, S,Couleur(0.0,0.5,0.0), 3);

	// fichiers des points : en rouge, d'épaisseur 10
	GEOMVIEW_points("test3_P.vect", S, nS, Couleur(1.0,0.0,0.0), 5);

	// libération de la mémoire
	delete[] A.noeuds;
	delete[] G.a;
	delete[] S;
}

///////////////////////////////////////////////////////////////////
// test 4 : création d'un objet composite Geomview
// creation du fichier test4.list
void test_objet_composite()
{
	printf("Création d'un objet composite Geomview\n");

	// ouverture du fichier
	GEOMVIEW_open("test4.list");

	// sommets d'un tétraèdre
	int nS = 4;
	Point S[] = {
	  Point(-1,-1,-1),
	  Point(-1,-1, 1),
	  Point(-1, 1,-1),
	  Point( 1,-1,-1)
	  };

	int nT = 4;
	Triangle T[] = {{0,1,2},{1,0,3},{2,1,3},{3,0,2}};

	// centre des triangles
	Point BT[nT];
	// normale aux triangles
	Point NT[nT];
	for (int i=0; i<nT; i++)
	{
		Point S1 = S[T[i].s1];
		Point S2 = S[T[i].s2];
		Point S3 = S[T[i].s3];
		BT[i] = (S1+S2+S3)/3.0;
		Point V = cross(S2-S1,S3-S1);
		NT[i] = V/norm(V);
	}

	// ajout des sommets de la triangulation en rouge, épaisseur 20
	GEOMVIEW_points(S, nS, Couleur(1.0,0.0,0.0), 20);

	// ajout de la triangulation, face rose claire, aretes bleu foncé
	GEOMVIEW_triangulation(S, nS, T, nT,
	  Couleur(1.0,0.7,0.9), Couleur(0.0,0.0,0.7));

	// ajout des normales, couleur noir, epaisseur 2, longueur 0.5
	GEOMVIEW_points_vects(BT, NT, nT, Couleur(0.0,0.0,0.0), 2, 0.5);

	// fermeture du fichier
	GEOMVIEW_close();
}

///////////////////////////////////////////////////////////////////////////////
// test 5 : visualisation d'un jeu de données
// l'utilisation entre le nom d'un fichier de donnees
// le programme crée un fichier de points test5_P.vect
// et éventuellement un fichier de vecteurs test5_N.vect
// contenant les normales orientées
// visualisation :
//   geomview test5_P.vect geomview.default &
//   geomview test5_P.vect test5_NP.vect geomview.default &
void test_visualisation_jeu_donnees()
{
	char nom_f[1000];

	// entrée du nom du fichier
	printf("Entrer le nom du fichier de données : ");
	scanf("%s", nom_f);

	// ouverture du fichier
	FILE *f = fopen(nom_f,"r");
	if (f==(FILE*)NULL)
	{
		printf("Le fichier %s n'existe pas\n", nom_f);
		return;
	}

	Point *S;
	Point *NS;
	ULONG nS;
	double r;
	int l = strlen(nom_f);
	if (strcmp("pno.txt", &nom_f[l-7])==0)
	{
		if (lecture_fichier_points_normales_orientees
		  (nom_f, nS, r, S, NS)!=0)
		{
			printf(" problème lors de la lecture de %s\n", nom_f);
			return;
		}

		// fichiers des points : en rouge, d'épaisseur 5
		GEOMVIEW_points("test5_P.vect", S, nS, Couleur(1.0,0.0,0.0), 5);
		printf("fichier test5_P.vect créé\n");

		// fichiers des normales : en noir, épaisseur 2, longueur 0.3
		GEOMVIEW_points_vects("test5_NP.vect", S, NS, nS, Couleur(0.0,0.0,0.0), 2, 0.3);
		printf("fichier test5_NP.vect créé\n");

		// libération de la mémoire
		delete[] S;
		delete[] NS;
	}
	else
	{
		if (lecture_fichier_points(nom_f, nS, r, S)!=0)
		{
			printf(" problème lors de la lecture de %s\n", nom_f);
			return;
		}

		// fichiers des points : en rouge, d'épaisseur 5
		GEOMVIEW_points("test5_P.vect", S, nS, Couleur(1.0,0.0,0.0), 5);
		printf("fichier test5_P.vect créé\n");

		// libération de la mémoire
		delete[] S;
	}
}


void saisir(const char* nom_f,Point * & S,Point * & NS,ULONG & nS,double & r)
{
	// ouverture du fichier
	FILE *f = fopen(nom_f,"r");
	int l = strlen(nom_f);
	if (f==(FILE*)NULL)
	{
		printf("Le fichier %s n'existe pas\n", nom_f);
		return;
	}

	if (strcmp("pno.txt", &nom_f[l-7])==0)
	{
		if (lecture_fichier_points_normales_orientees
		    (nom_f, nS, r, S, NS)!=0)
		{
			printf(" problème lors de la lecture de %s\n", nom_f);
			return;
		}
	}
	else
	{
		if (lecture_fichier_points(nom_f, nS, r, S)!=0)
		{
			printf(" problème lors de la lecture de %s\n", nom_f);
			return;
		}
	}

	std::cout<<"le pas dedans "<<r<<std::endl;
}

void makeSurf(Point * S,Point * & NS,ULONG nS,double pas,double r,
              Point * & pts,ULONG & npts,Triangle * & Trs,ULONG & nT){

	std::cout<<"calcul des normales"<<std::endl;
	if(NS==NULL){
		NS=new Point[nS];
		normales(nS,S,NS,r);
	}

	std::cout<<"calcul de la grille des distances signées"<<std::endl;

	grille gr=calc_grille_dist(nS,S,NS,pas);

	std::cout<<"calcul de la surface iso-valeur"<<std::endl;

	std::list<Triangle> T;
	std::list<Point> SS;
	gr.calc_iso_surf(0,T,SS);

	std::cout<<"fin"<<std::endl;

	nT=T.size();
	Trs=new Triangle[nT];
	std::list<Triangle>::iterator itT=T.begin();
	for(int i=0;i<nT;i++){
		Trs[i]=(*itT);
		itT++;
	}

	npts=SS.size();
	pts=new Point[npts];
	std::list<Point>::iterator itS=SS.begin();
	for(int i=0;i<npts;i++){
		pts[i]=(*itS);
		itS++;
	}
}

// bruitage des points
void bruiter(int n,Point * pts,double amplitude){
	for(int i=0;i<n;i++){
		pts[i].x+=(double)rand()/RAND_MAX*amplitude-amplitude/2.0;
		pts[i].y+=(double)rand()/RAND_MAX*amplitude-amplitude/2.0;
		pts[i].z+=(double)rand()/RAND_MAX*amplitude-amplitude/2.0;
	}
}

///////////////////////////////////////////////////////////////////////////////
// PROGRAMME PRINCIPAL
int main(int argc, char *argv[])
{
	Point * S=NULL;
	Point * NS=NULL;
	Point * pts=NULL;
	Triangle * Trs=NULL;
	ULONG nS,nSS,nT;
	double pas,r;
	double facteurBruit,facteurRay;
	bool traceNormales=false;

	std::string file;
	std::cout<<"Entrez le nom du jeu de donnees"<<std::endl;
	cin>>file;

	std::cout<<"========"<<file.c_str()<<std::endl;

	saisir(file.c_str(),S,NS,nS,pas);
	pas*=1.1;

	if(NS==NULL){
		traceNormales=true;

		printf("Entrez le facteur du bruit :\n");
		scanf("%lf",&facteurBruit);

		printf("Entrez le facteur multiplicatif rayon r :\n");
		scanf("%lf",&facteurRay);

		std::cout<<"facteurRay "<<facteurRay<<std::endl;

		r=facteurRay*pas;

		if(facteurBruit!=0)
			bruiter(nS,S,facteurBruit);
	}

	makeSurf(S,NS,nS,pas,r,pts,nSS,Trs,nT);

	GEOMVIEW_open("isosurf.list");

// 	Point S2[gr.nl*gr.nc*gr.nt];
// 	int iS2=0;
// 	for(int i=0;i<gr.nl;i++){
// 		for(int j=0;j<gr.nc;j++){
// 			for(int k=0;k<gr.nt;k++){
// 				S2[iS2]=Point(i*gr.pas+gr.xmin,j*gr.pas+gr.ymin,
// 				             k*gr.pas+gr.zmin);
// 				iS2++;
// 			}
// 		}
// 	}

	//GEOMVIEW_points(S2, gr.nl*gr.nc*gr.nt, Couleur(1.0,0.0,0.0), 10);

	GEOMVIEW_triangulation(pts, nSS, Trs, nT,
	  Couleur(1.0,0.7,0.9), Couleur(0.0,0.0,0.7));

	GEOMVIEW_close();

	if(traceNormales){
		GEOMVIEW_open("normales.list");

		// fichiers des points : en rouge, d'épaisseur 10
		GEOMVIEW_points(S, nS, Couleur(1.0,0.0,0.0), 10);

		// fichiers des normales : en noir, épaisseur 2, longueur 0.3
		GEOMVIEW_points_vects(S, NS, nS, Couleur(0.0,0.0,0.0), 2, 0.3);

		GEOMVIEW_close();
	}

	return 0;
}

