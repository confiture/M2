//////////////////////////////////////////////////////////////////////////////
//
// TEST DE LA LIBRAIRIE LIB_BASE2D
//
// TEST DE LA LIBRAIRIE XFIG
//
// compilation :
//   make programme_tests
//
// execution :
//   programme_tests
//   programme_tests 1
//     (test 1 : un exemple de figure XFIG avec différents objets)
//     (et différentes profondeurs)
//     (creation des fichiers test1.fig, test1.eps, test1.jpg, test1.gif)
//
//   programme_tests 2
//     (test 2 : ensemble de points sur un cercle + normales orientées)
//     (creation des fichiers test2.fig, test2.eps, test2.jpg, test2.gif)
//
//   programme_tests 3
//     (test 3 : calcul d'un graphe de proximite et d'un arbre couvrant minimal)
//     (creation des fichiers ACM.fig, ACM.gif, graphe.fig, graphe.gif)
//
//   programme_tests 4
//     (test 4 : calcul des éléments propres d'une matrice sym. 2x2)
//
//   programme_tests 5
//     (test 5 : création de différents jeux de données )
//     (         ensemble de points + normales orientées)
//     (choisir ensuite le jeux de données à créer )
//     (  1 : points sur un oval )
//     (  2 : points sur une courbe de subdivision ouverte )
//     (  3 : points sur une courbe de subdivision fermée )
//     (creation des fichiers test5.fig, test5.eps, test5.jpg, test5.gif)
//
// visualisation des fichiers FIG avec xfig
//
// visualisation des fichiers EPS avec gv
//
// visualisation des fichiers GIF ou JPG avec gqview, gimp, ...
//
//  (inclure de préférence des fichiers EPS dans un document Open Office)
//
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include"lib_base2d.hpp"
#include"xfig.hpp"
#include "courbe.hpp"
#include <algorithm>
#include <string>
#include <stdlib.h>

// bruitage des points
void bruiter(vPoint & pts,double amplitude){
	int n=pts.size();
	for(int i=0;i<n;i++){
		pts[i].x+=(double)rand()/RAND_MAX*amplitude-amplitude/2.0;
		pts[i].y+=(double)rand()/RAND_MAX*amplitude-amplitude/2.0;
	}
}

//////////////////////////////////////////////////////////////////////////////
// test 1 : un exemple de figure XFIG avec différents objets
// et différentes profondeurs
// creation des fichiers test1.fig, test1.eps, test1.jpg, test1.gif
void test_xfig_et_exports()
{
	printf("Création d'une figure avec différents objets\n");
	printf("et export aux formats EPS, JPG, GIF\n");

	// création du fichier FIG
	printf("Création du fichier test1.fig\n");
	XFIG_open("test1.fig");

	// tracé d'un polygone avec 5 sommets, contour rouge d'épaisseur 4,
	//   et interieur bleu
	{
		// Point P[5];
		vPoint P(5);
		P[0] = Point(0.0,0.0);
		P[1] = Point(2.0,0.0);
		P[2] = Point(2.0,2.0);
		P[3] = Point(1.0,3.0);
		P[4] = Point(0.0,2.0);
		// double x[] = {0,2,2,1,0};
		// double y[] = {0,0,2,3,2};
		XFIG_thickness=4;
		XFIG_pen_color = XFIG_COLOR_RED;
		XFIG_fill_color = XFIG_COLOR_BLUE;
		XFIG_area_fill=XFIG_AREA_FILL_FULL;
		// XFIG_polygon(x,y,5); // version tableaux double (x,y)
		// XFIG_polygon(P,5); // version tableau Point P
		XFIG_polygon(P); // version vector<Point> P
	}

	// tracé d'un rectangle arrondi par dessus le polygone
	//   de couleur uni vert
	XFIG_depth--;
	XFIG_thickness=1;
	XFIG_pen_color = XFIG_fill_color = XFIG_COLOR_GREEN;
	XFIG_arc_box(1,1,3,2);

	// tracé d'un texte en noir par-dessus
	XFIG_depth--;
	XFIG_pen_color = XFIG_COLOR_BLACK;
	XFIG_text_justify = XFIG_JUSTIFY_CENTER;
	XFIG_text(2,1.5,"Déjà");

	// fermeture du fichier
	XFIG_close();

	// export du fichier au format EPS avec une échelle de 0.5
	printf("Création du fichier test1.eps\n");
	XFIG_export_EPS("test1.fig", "test1.eps", 0.5);

	// export du fichier au format JPEG avec une échelle de 2
	//   et qualité 95
	printf("Création du fichier test1.jpg\n");
	XFIG_export_JPG("test1.fig", "test1.jpg", 2.0, 95);

	// export du fichier au format GIF avec une échelle de 1
	printf("Création du fichier test1.gif\n");
	XFIG_export_GIF("test1.fig", "test1.gif", 1.0);
}

///////////////////////////////////////////////////////////////////
// test 2 : ensemble de points sur un cercle + normales orientées
// creation des fichiers test2.fig, test2.eps, test2.jpg, test2.gif
void test_trace_points_normales_orientees()
{
	int i,n=30;
	double r=2.0;

	printf("Création d'une figure avec un ensemble de points\n");
	printf("sur un cercle et de normales orientées\n");
	printf("et export aux formats EPS, JPG, GIF\n");

	// création du fichier FIG
	printf("Création du fichier test2.fig\n");
	XFIG_open("test2.fig");

	// cercle en vert de centre (0,0) et rayon r, d'épaisseur 3
	//  à la profondeur 50
	XFIG_thickness=3;
	XFIG_depth = 50;
	XFIG_pen_color = XFIG_COLOR_GREEN;
	XFIG_circle(0,0,r);


	// tracé de n points sur le cercle, chaque point sur la forme
	// d'un cercle	rouge de rayon 0.04, d'épaisseur 2,
	// les points sont tracés  à la profondeur 49
	XFIG_thickness=2;
	XFIG_depth = 49;
	XFIG_pen_color = XFIG_COLOR_RED;
	for (i=0; i<n; i++)
	{
		double a;
		a = (2.0*M_PI*i)/n;
		Point C(r*cos(a),r*sin(a));
		XFIG_circle(C,0.04);
	}

	// tracé de n vecteurs-normales au cercle, chaque vecteur
	// de longueur 1, en bleu et à la profondeur 48
	XFIG_thickness=1;
	XFIG_depth = 48;
	XFIG_pen_color = XFIG_COLOR_BLUE;
	for (i=0; i<n; i++)
	{
		double a;
		a = (2.0*M_PI*i)/n;
		Point P1(cos(a),sin(a)); // point sur le cercle unité
		Point P = r*P1;          // point sur le cercle de rayon r
		Point N = P1;            // normale au cercle au point P
		XFIG_arrow(P,P+N);
	}
	// fermeture du fichier
	XFIG_close();

	// export du fichier au format EPS avec une échelle de 1
	printf("Création du fichier test2.eps\n");
	XFIG_export_EPS("test2.fig", "test2.eps", 1.0);

	// export du fichier au format JPEG avec une échelle de 0.5
	//   et qualité 85
	printf("Création du fichier test2.jpg\n");
	XFIG_export_JPG("test2.fig", "test2.jpg", 0.5, 85);

	// export du fichier au format GIF avec une échelle de 1
	printf("Création du fichier test2.gif\n");
	XFIG_export_GIF("test2.fig", "test2.gif", 1.0);
}


///////////////////////////////////////////////////////////////////
// test 3 : calcul d'un graphe de proximite et d'un arbre couvrant minimal
// creation des fichiers test3.fig, test3.eps, test3.jpg, test3.gif

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

	// génération aléatoire de n points dans un carré de côté égal à 4
	int n=20;
	Point P[n];
	long int randSeed = 1;
	srand48(randSeed); // initialisation du générateur aléatoire

	for (int i=0; i<n; i++)
		P[i] = Point(drand48(),drand48())*4.0;

	// création du graphe de proximité correspondant à r
	double r=2.0;
	Graphe G = calcule_graphe_proximite(P, n, r);

	// tracé du graphe
	printf("Création du fichier graphe.fig\n");
	XFIG_open("graphe.fig");

	// tracé des sommets du graphe, chaque sommet sous forme d'un petit cercle
	XFIG_depth = 50;
	XFIG_pen_color = XFIG_COLOR_RED;
	for (int i=0; i<n; i++)
		XFIG_circle(P[i],0.05);

	// tracé des aretes du graphe en vert foncé
	XFIG_depth = 49;
	XFIG_pen_color = XFIG_COLOR_GREEN3;
	for (int i=0; i<G.nA; i++)
		XFIG_line(P[G.a[i].s1],P[G.a[i].s2]);

	XFIG_close();

	// export du fichier au format GIF avec une échelle de 2
	printf("Création du fichier graphe.gif\n");
	XFIG_export_GIF("graphe.fig", "graphe.gif", 2.0);

	// calcul de l'arbre couvrant minimal
	Arbre A=calcule_ACM(G);

	// tracé de l'a.c.m.
	printf("Création du fichier ACM.fig\n");
	XFIG_open("ACM.fig");

	// tracé des noeuds de l'arbre, chaque sommet sous forme d'un petit cercle
	XFIG_depth = 50;
	XFIG_pen_color = XFIG_COLOR_RED;
	for (int i=0; i<n; i++)
		XFIG_circle(P[i],0.01);

	// tracé des aretes de l'arbre en bleu
	XFIG_depth = 48;
	XFIG_pen_color = XFIG_COLOR_BLUE;
	for (int i=0; i<A.n; i++)
	{
		vint voisins = A.noeuds[i].voisins;
		for (unsigned int j = 0; j<voisins.size(); j++)
		if (voisins[j]<i)
			XFIG_line(P[voisins[j]],P[i]);
	}

	XFIG_close();

	// export du fichier au format GIF avec une échelle de 2
	printf("Création du fichier ACM.gif\n");
	XFIG_export_GIF("ACM.fig", "ACM.gif", 2.0);

	// libération de la mémoire
	delete[] A.noeuds;
	delete[] G.a;
}


//////////////////////////////////////////////////////////////////////////////
// test 4 : calcul des éléments propres d'une matrice sym. 2x2
void test_elements_propres_mat_sym()
{
	double A11=4.0;
	double A12=0.01;
	double A22=2.0;
	double l1, l2;
	Point v1, v2;

	printf("Calcul des elements propres d'une matrice A sym. 2x2\n");
	printf("    | A11 A12 |\n");
	printf("A = |         |\n");
	printf("    | A12 A22 |\n");
	printf("Entrer la valeur de A11 : "); scanf("%lf", &A11);
	printf("Entrer la valeur de A12 : "); scanf("%lf", &A12);
	printf("Entrer la valeur de A22 : "); scanf("%lf", &A22);
	elements_propres_mat_sym(A11,A12,A22,l1,l2,v1,v2);
	printf("    | %15.7e %15.7e |\n",A11,A12);
	printf("A = |                                 |\n");
	printf("    | %15.7e %15.7e |\n",A12,A22);
	printf("\n Elements propres :\n");
	printf("l1 = %15.7e, v1 = (%10.8f,%10.8f)\n", l1, v1.x, v1.y);
	printf("l2 = %15.7e, v2 = (%10.8f,%10.8f)\n", l2, v2.x, v2.y);
	printf("\nVerification (v1,v2) : base orthonormee\n");
	printf("||v1|| = %15.7e\n", norm(v1));
	printf("||v2|| = %15.7e\n", norm(v2));
	printf("v1.v2 = %15.7e\n", dot(v1,v2));
}

///////////////////////////////////////////////////////////////////////////////
// test 5 : création de différents jeux de données
//  		ensemble de points + normales orientées
// choisir ensuite le jeux de données à créer
//   1 : points sur un oval
//   2 : points sur une courbe de subdivision ouverte
//   3 : points sur une courbe de subdivision fermée
// creation des fichiers test5.fig, test5.eps, test5.jpg, test5.gif

///////////////////////////////////////////////////////////////////////////////
// subdivision du polygone P par la méthode de Chaikin
// Entree : P = vecteur de n points
//          ferme = booleen qui indique si le polygone se referme
//            par défaut ferme=false
// Sortie : Q  = vecteur de m points issus de la subdivision
//            m = 2*n si P est fermé, 2*n-2 sinon
vPoint chaikin(vPoint P, bool ferme=false)
{
	int n = P.size();
	if (n<3)
	{
		fprintf(stderr, "chaikin : il faut au moins 3 points\n");
		return P;
	}

	vPoint Q;
	if (ferme)
	{
		Q.resize(2*n);
		Point* Qt = &Q[0];
		Qt[0] = 0.75*P[n-1]+0.25*P[0];
		Qt[1] = 0.25*P[n-1]+0.75*P[0];
		for (int i=2; i<=n; i++)
		{
			Qt[2*i-2] = 0.75*P[i-2]+0.25*P[i-1];
			Qt[2*i-1] = 0.25*P[i-2]+0.75*P[i-1];
		}
	}
	else
	{
		Q.resize(2*n-2);
		Point* Qt = &Q[0];
		Qt[0] = P[0];
		Qt[1] = 0.5*(P[0]+P[1]);
		for (int i=2; i<n; i++)
		{
			Qt[2*i-2] = 0.75*P[i-1]+0.25*P[i];
			Qt[2*i-1] = 0.25*P[i-1]+0.75*P[i];
		}
		Qt[2*n-4] = 0.5*(P[n-2]+P[n-1]);
		Qt[2*n-3] = P[n-1];
	}

	return Q;
}

///////////////////////////////////////////////////////////////////////////////
// évaluation de normales orientées à partir d'une courbe discrétisée
// Entree : P = vecteur de n points
//          ferme = booleen qui indique si le polygone se referme
//            par défaut ferme=false
// Sortie : NP  = vecteur de n points (unitaires)
//           NP[i] : normale orientée associée à P[i]
vPoint normale_orientee(vPoint P, bool ferme=false)
{
	int n = P.size();
	vPoint NP(n);
	if (n<3)
	{
		fprintf(stderr, "normale_orientee : il faut au moins 3 points\n");
		return NP;
	}

	for (int i=0; i<n; i++)
	{
		Point T;

		// calcul de la tangente orientée T
		if (i==0)
		{
			if (ferme)
				T = 0.5*P[1]-0.5*P[n-1];
			else
				T = -1.5*P[0]+2.0*P[1]-0.5*P[2];
		}
		else if (i==n-1)
		{
			if (ferme)
				T = 0.5*P[0]-0.5*P[n-2];
			else
				T = 0.5*P[n-3]-2.0*P[n-2]+1.5*P[n-1];
		}
		else
			T = 0.5*P[i+1]-0.5*P[i-1];

		// calcul de la normale orientée
		T = T/norm(T);
		NP[i] = Point(T.y,-T.x);
	}
	return NP;
}

///////////////////////////////////////////////////////////////////////////////
// calcul les écarts dmin et dmax pour un ensemble de points P
// dmin = min( min(|P(i)-P(j)|)
//         i  j!=i
// dmax = max( min(|P(i)-P(j)|)
//         i  j!=i
void ecart_min_max(vPoint P,double &dmin, double &dmax)
{
	dmin = norm(P[0]-P[1]);
	dmax = dmin;
	int n = P.size();
	for (int i=0; i<n; i++)
	{
		double dmin_i = norm(P[i]-P[(i+1)%n]);
		for (int j=0; j<n; j++)
		{
			if (j!=i)
			{
				double d = norm(P[i]-P[j]);
				if (dmin_i>d) dmin_i = d;
			}
		}
		if (dmin > dmin_i) dmin = dmin_i;
		if (dmax < dmin_i) dmax = dmin_i;
	}
}

void test_jeux_donnees_normales_orientees()
{
	printf("Création d'un ensemble de points et normales orientées\n");
	printf("et export aux formats EPS, JPG, GIF\n");

	int num_test=0;
	while (num_test<1 || num_test>3)
	{
		printf("Choisir le jeux de données à créer :\n");
		printf("1 : points sur un oval   						 \n");
		printf("2 : points sur une courbe de subdivision ouverte \n");
		printf("3 : points sur une courbe de subdivision fermée  \n");
		scanf("%i", &num_test);

	}

	// création du jeu de données
	vPoint P ,NP;
	switch (num_test)
	{
		case 1 :
		{
		// point sur un oval
		int n=50;
		double rx=1.5, ry=1.0;
		P.resize(n);
		NP.resize(n);
		for (int i=0; i<n; i++)
		{
			double a = 2.0*i*M_PI/n;
			P[i] = Point(rx*cos(a),ry*sin(a));
			Point Ni = Point(ry*cos(a),rx*sin(a));
			Ni = Ni/norm(Ni);
			NP[i] = Ni;
		}

		break;
		}

		case 2 :
		{
		// points issus d'une courbe de subdivision ouverte
		P.resize(6);
		int k=0;
		bool ferme=false;
		P[k++] = Point(0,0);
		P[k++] = Point(0,1);
		P[k++] = Point(1,1);
		P[k++] = Point(1,0);
		P[k++] = Point(3,0);
		P[k++] = Point(3,1);
		// création de points par subdivision
		for (int i=1; i<=4; i++)
			P=chaikin(P,ferme);
		// calcul des normales orientées par différences finies
		NP = normale_orientee(P,ferme);
		break;
		}
		case 3 :
		{
		// points issus d'une courbe de subdivision fermée
		P.resize(4);
		int k=0;
		bool ferme=true;
		P[k++] = Point(0,0);
		P[k++] = Point(1,0);
		P[k++] = Point(1,1);
		P[k++] = Point(0,1);
		// création de points par subdivision
		for (int i=1; i<=4; i++)
			P=chaikin(P,ferme);
		// calcul des normales orientées par différences finies
		NP = normale_orientee(P,ferme);
		break;
		}
	}

	// calcul des écarts
	double dmin, dmax;
	ecart_min_max(P, dmin, dmax);
	printf("dmin = %f\n", dmin);
	printf("dmax = %f\n", dmax);

	// nombre de points
	int n = P.size();

	// création du fichier FIG
	printf("Création du fichier test5.fig\n");
	XFIG_open("test5.fig");

	// tracé des n points, chaque point sur la forme
	// d'un cercle	rouge de rayon dmin/3, d'épaisseur 2,
	// les points sont tracés  à la profondeur 49
	XFIG_thickness=2;
	XFIG_depth = 49;
	XFIG_pen_color = XFIG_COLOR_RED;
	for (int i=0; i<n; i++)
		XFIG_circle(P[i],dmin/3.0);

	// tracé des n vecteurs-normales, chaque vecteur
	// de longueur 0.3, en bleu et à la profondeur 48
	XFIG_thickness=1;
	XFIG_depth = 48;
	XFIG_pen_color = XFIG_COLOR_BLUE;
	for (int i=0; i<n; i++)
		XFIG_arrow(P[i],P[i]+0.3*NP[i]);

	// fermeture du fichier
	XFIG_close();

	// export du fichier au format EPS avec une échelle de 1
	printf("Création du fichier test5.eps\n");
	XFIG_export_EPS("test5.fig", "test5.eps", 1.0);

	// export du fichier au format JPEG avec une échelle de 2
	//   et qualité 85
	printf("Création du fichier test5.jpg\n");
	XFIG_export_JPG("test5.fig", "test5.jpg", 2.0, 85);

	// export du fichier au format GIF avec une échelle de 2
	printf("Création du fichier test5.gif\n");
	XFIG_export_GIF("test5.fig", "test5.gif", 2.0);
}

void partie1_shell(int num_test,float facteurPas,const char * fic_sortie)
{

	// création du jeu de données
	vPoint P ,NP;
	switch (num_test)
	{
	case 1 :
		{
			// point sur un oval
			int n=50;
			double rx=1.5, ry=1.0;
			P.resize(n);
			NP.resize(n);
			for (int i=0; i<n; i++)
			{
				double a = 2.0*i*M_PI/n;
				P[i] = Point(rx*cos(a),ry*sin(a));
				Point Ni = Point(ry*cos(a),rx*sin(a));
				Ni = Ni/norm(Ni);
				NP[i] = Ni;
			}

			break;
		}

	case 2 :
		{
			// points issus d'une courbe de subdivision ouverte
			P.resize(6);
			int k=0;
			bool ferme=false;
			P[k++] = Point(0,0);
			P[k++] = Point(0,1);
			P[k++] = Point(1,1);
			P[k++] = Point(1,0);
			P[k++] = Point(3,0);
			P[k++] = Point(3,1);
			// création de points par subdivision
			for (int i=1; i<=4; i++)
				P=chaikin(P,ferme);
			// calcul des normales orientées par différences finies
			NP = normale_orientee(P,ferme);
			break;
		}
	case 3 :
		{
			// points issus d'une courbe de subdivision fermée
			P.resize(4);
			int k=0;
			bool ferme=true;
			P[k++] = Point(0,0);
			P[k++] = Point(1,0);
			P[k++] = Point(1,1);
			P[k++] = Point(0,1);
			// création de points par subdivision
			for (int i=1; i<=4; i++)
				P=chaikin(P,ferme);
			// calcul des normales orientées par différences finies
			NP = normale_orientee(P,ferme);
			break;
		}
	}

	int n=P.size();
	Point pTab[n];
	Point nTab[n];
	for(int i=0;i<n;i++){
		pTab[i]=P[i];
		nTab[i]=NP[i];
	}

	double pas=calc_pas(n,pTab,facteurPas);
	grille gr=calc_grille_dist(n,pTab,nTab,pas);

	vPoint E1,E2;
	gr.calc_iso_courbe(0,E1,E2);
	int N=E1.size();

	// calcul des écarts
	double dmin, dmax;
	ecart_min_max(P, dmin, dmax);

	// création du fichier FIG
	std::string filename(fic_sortie);
	filename+=".fig";
	XFIG_open(filename.c_str());

	// tracé des n points, chaque point sur la forme
	// d'un cercle	rouge de rayon dmin/3, d'épaisseur 2,
	// les points sont tracés  à la profondeur 49
	XFIG_thickness=2;
	XFIG_depth = 49;
	XFIG_pen_color = XFIG_COLOR_RED;
	for (int i=0; i<n; i++)
		XFIG_circle(P[i],dmin/3.0);

	vPoint segm(2);
	for(int i=0;i<N;i++){
		segm[0]=E1[i];
		segm[1]=E2[i];
		XFIG_polygon(segm);
	}

	XFIG_close();

	std::string filenameJPG(fic_sortie);
	filenameJPG+=".jpg";
	XFIG_export_JPG(filename.c_str(), filenameJPG.c_str(), 2.0, 100);
}


void partie1()
{
	float facteurPas;
	printf("Création d'un ensemble de points et normales orientées\n");
	printf("et export aux formats EPS, JPG, GIF\n");

	int num_test=0;
	while (num_test<1 || num_test>3)
	{
		printf("Choisir le jeux de données à créer :\n");
		printf("1 : points sur un oval   						 \n");
		printf("2 : points sur une courbe de subdivision ouverte \n");
		printf("3 : points sur une courbe de subdivision fermée  \n");
		scanf("%i", &num_test);

		printf("Entrez le facteur multiplicatif pour le pas de la grille :\n");
		scanf("%f",&facteurPas);
	}

	// création du jeu de données
	vPoint P ,NP;
	switch (num_test)
	{
	case 1 :
		{
			// point sur un oval
			int n=50;
			double rx=1.5, ry=1.0;
			P.resize(n);
			NP.resize(n);
			for (int i=0; i<n; i++)
			{
				double a = 2.0*i*M_PI/n;
				P[i] = Point(rx*cos(a),ry*sin(a));
				Point Ni = Point(ry*cos(a),rx*sin(a));
				Ni = Ni/norm(Ni);
				NP[i] = Ni;
			}

			break;
		}

	case 2 :
		{
			// points issus d'une courbe de subdivision ouverte
			P.resize(6);
			int k=0;
			bool ferme=false;
			P[k++] = Point(0,0);
			P[k++] = Point(0,1);
			P[k++] = Point(1,1);
			P[k++] = Point(1,0);
			P[k++] = Point(3,0);
			P[k++] = Point(3,1);
			// création de points par subdivision
			for (int i=1; i<=4; i++)
				P=chaikin(P,ferme);
			// calcul des normales orientées par différences finies
			NP = normale_orientee(P,ferme);
			break;
		}
	case 3 :
		{
			// points issus d'une courbe de subdivision fermée
			P.resize(4);
			int k=0;
			bool ferme=true;
			P[k++] = Point(0,0);
			P[k++] = Point(1,0);
			P[k++] = Point(1,1);
			P[k++] = Point(0,1);
			// création de points par subdivision
			for (int i=1; i<=4; i++)
				P=chaikin(P,ferme);
			// calcul des normales orientées par différences finies
			NP = normale_orientee(P,ferme);
			break;
		}
	}

	int n=P.size();
	Point pTab[n];
	Point nTab[n];
	for(int i=0;i<n;i++){
		pTab[i]=P[i];
		nTab[i]=NP[i];
	}

	double pas=calc_pas(n,pTab,facteurPas);
	grille gr=calc_grille_dist(n,pTab,nTab,pas);

	vPoint E1,E2;
	gr.calc_iso_courbe(0,E1,E2);
	int N=E1.size();

	// calcul des écarts
	double dmin, dmax;
	ecart_min_max(P, dmin, dmax);
	printf("dmin = %f\n", dmin);
	printf("dmax = %f\n", dmax);

	// création du fichier FIG
	printf("Création du fichier partie1.fig\n");
	XFIG_open("partie1.fig");

	// tracé des n points, chaque point sur la forme
	// d'un cercle	rouge de rayon dmin/3, d'épaisseur 2,
	// les points sont tracés  à la profondeur 49
	XFIG_thickness=2;
	XFIG_depth = 49;
	XFIG_pen_color = XFIG_COLOR_RED;
	for (int i=0; i<n; i++)
		XFIG_circle(P[i],dmin/3.0);

	vPoint segm(2);
	for(int i=0;i<N;i++){
		segm[0]=E1[i];
		segm[1]=E2[i];
		XFIG_polygon(segm);
	}

	XFIG_close();

	// export du fichier au format JPEG avec une échelle de 2
	//   et qualité 95
	printf("Création du fichier partie1.jpg\n");
	XFIG_export_JPG("partie1.fig", "partie1.jpg", 2.0, 100);
}



void partie2_shell(int num_test,float facteurBruit,float facteurPas,float facteurRay,const char * fic_sortie)
{
	// création du jeu de données
	vPoint P ,NP;
	switch (num_test)
	{
		case 1 :
		{
		// point sur un oval
		int n=50;
		double rx=1.5, ry=1.0;
		P.resize(n);
		NP.resize(n);
		for (int i=0; i<n; i++)
		{
			double a = 2.0*i*M_PI/n;
			P[i] = Point(rx*cos(a),ry*sin(a));
		}

		break;
		}

		case 2 :
		{
		// points issus d'une courbe de subdivision ouverte
		P.resize(6);
		int k=0;
		bool ferme=false;
		P[k++] = Point(0,0);
		P[k++] = Point(0,1);
		P[k++] = Point(1,1);
		P[k++] = Point(1,0);
		P[k++] = Point(3,0);
		P[k++] = Point(3,1);
		// création de points par subdivision
		for (int i=1; i<=4; i++)
			P=chaikin(P,ferme);

		break;
		}
		case 3 :
		{
		// points issus d'une courbe de subdivision fermée
		P.resize(4);
		int k=0;
		bool ferme=true;
		P[k++] = Point(0,0);
		P[k++] = Point(1,0);
		P[k++] = Point(1,1);
		P[k++] = Point(0,1);
		// création de points par subdivision
		for (int i=1; i<=4; i++)
			P=chaikin(P,ferme);
		break;
		}
	}

	double dmin, dmax;
	ecart_min_max(P, dmin, dmax);
	bruiter(P,2*facteurBruit*dmin);

	int n=P.size();
	Point pTab[n];
	Point nTab[n];
	for(int i=0;i<n;i++){
		pTab[i]=P[i];
	}


	double pas=calc_pas(n,pTab,facteurPas);
	normales(n,pTab,nTab,pas*facteurRay);
	grille gr=calc_grille_dist(n,pTab,nTab,pas);

	vPoint E1,E2;
	gr.calc_iso_courbe(0,E1,E2);
	int N=E1.size();

	// création du fichier FIG
	std::string filename(fic_sortie);
	filename+=".fig";
	XFIG_open(filename.c_str());

	XFIG_thickness=2;
	XFIG_depth = 49;
	XFIG_pen_color = XFIG_COLOR_RED;
	for (int i=0; i<n; i++)
		XFIG_circle(P[i],dmin/3.0);

	for (int i=0; i<n; i++)
		XFIG_arrow(P[i],P[i]+0.3*nTab[i]);

	vPoint segm(2);
	for(int i=0;i<N;i++){
		segm[0]=E1[i];
		segm[1]=E2[i];
		XFIG_polygon(segm); // version vector<Point> P
	}

	XFIG_close();

	std::string filenameJPG(fic_sortie);
	filenameJPG+=".jpg";
	XFIG_export_JPG(filename.c_str(), filenameJPG.c_str(), 2.0, 100);
}


void partie2()
{
	float facteurPas=1;
	float facteurRay;
	float facteurBruit;

	printf("Création d'un ensemble de points et normales orientées\n");
	printf("et export aux formats EPS, JPG, GIF\n");

	int num_test=0;
	while (num_test<1 || num_test>3)
	{
		printf("Choisir le jeux de données à créer :\n");
		printf("1 : points sur un oval   						 \n");
		printf("2 : points sur une courbe de subdivision ouverte \n");
		printf("3 : points sur une courbe de subdivision fermée  \n");
		scanf("%i", &num_test);

		printf("Entrez le facteur du bruit :\n");
		scanf("%f",&facteurBruit);

// 		printf("Entrez le facteur multiplicatif du pas :\n");
// 		scanf("%f",&facteurPas);

		printf("Entrez le facteur multiplicatif rayon r :\n");
		scanf("%f",&facteurRay);
	}

	// création du jeu de données
	vPoint P ,NP;
	switch (num_test)
	{
		case 1 :
		{
		// point sur un oval
		int n=50;
		double rx=1.5, ry=1.0;
		P.resize(n);
		NP.resize(n);
		for (int i=0; i<n; i++)
		{
			double a = 2.0*i*M_PI/n;
			P[i] = Point(rx*cos(a),ry*sin(a));
		}

		break;
		}

		case 2 :
		{
		// points issus d'une courbe de subdivision ouverte
		P.resize(6);
		int k=0;
		bool ferme=false;
		P[k++] = Point(0,0);
		P[k++] = Point(0,1);
		P[k++] = Point(1,1);
		P[k++] = Point(1,0);
		P[k++] = Point(3,0);
		P[k++] = Point(3,1);
		// création de points par subdivision
		for (int i=1; i<=4; i++)
			P=chaikin(P,ferme);

		break;
		}
		case 3 :
		{
		// points issus d'une courbe de subdivision fermée
		P.resize(4);
		int k=0;
		bool ferme=true;
		P[k++] = Point(0,0);
		P[k++] = Point(1,0);
		P[k++] = Point(1,1);
		P[k++] = Point(0,1);
		// création de points par subdivision
		for (int i=1; i<=4; i++)
			P=chaikin(P,ferme);
		break;
		}
	}

	double dmin, dmax;
	ecart_min_max(P, dmin, dmax);
	bruiter(P,2*facteurBruit*dmin);

	int n=P.size();
	Point pTab[n];
	Point nTab[n];
	for(int i=0;i<n;i++){
		pTab[i]=P[i];
	}

	double pas=calc_pas(n,pTab,facteurPas);
	normales(n,pTab,nTab,pas*facteurRay);
	grille gr=calc_grille_dist(n,pTab,nTab,pas);

	vPoint E1,E2;
	gr.calc_iso_courbe(0,E1,E2);
	int N=E1.size();

	// création du fichier FIG
	printf("Création du fichier partie2.fig\n");
	XFIG_open("partie2.fig");

	XFIG_thickness=2;
	XFIG_depth = 49;
	XFIG_pen_color = XFIG_COLOR_RED;
	for (int i=0; i<n; i++)
		XFIG_circle(P[i],dmin/3.0);

	for (int i=0; i<n; i++)
		XFIG_arrow(P[i],P[i]+0.3*nTab[i]);

	vPoint segm(2);
	for(int i=0;i<N;i++){
		segm[0]=E1[i];
		segm[1]=E2[i];
		XFIG_polygon(segm); // version vector<Point> P
	}

	XFIG_close();

	// export du fichier au format JPEG avec une échelle de 2
	//   et qualité 95
	printf("Création du fichier partie2.jpg\n");
	XFIG_export_JPG("partie2.fig", "partie2.jpg", 2.0, 100);
}

///////////////////////////////////////////////////////////////////////////////
// PROGRAMME PRINCIPAL
int main(int argc, char *argv[])
{
// 	if(argc==6){
// 		float facteurPas;
// 		float facteurRay;
// 		float facteurBruit;
// 		sscanf(argv[2],"%f",&facteurBruit);
// 		sscanf(argv[3],"%f",&facteurPas);
// 		sscanf(argv[4],"%f",&facteurRay);

// 		partie2_shell(atoi(argv[1]),facteurBruit,facteurPas,facteurRay,argv[5]);
// 	}
// 	else if(argc==4){
// 		float facteurPas;
// 		sscanf(argv[2],"%f",&facteurPas);

// 		partie1_shell(atoi(argv[1]),facteurPas,argv[3]);
// 	}


// POUR L'UTILISATEUR
	int choix;
	do{
		std::cout<<"Entrez 1 pour la partie 1 ou 2 pour la partie 2"<<std::endl;
		cin>>choix;
	}
	while(choix!=1 && choix!=2);

	if(choix==1)
		partie1();
	else
		partie2();
	return 0;
}

