///////////////////////////////////////////////////////////////////
// TP - Simplification de lignes polygonale par l'algorithme
//      de Douglas-Peuker
// Squelette de programme C++
// compilation : g++ douglas_peuker.cpp -o douglas_peuker
// exécution   : ./douglas_peuker
///////////////////////////////////////////////////////////////////

#include<cstdio>
#include<cmath>
#include<list>
#include<iostream>
#include<cstdlib>
#include<cstring>
#include <limits>
#include <fstream>

#ifndef ABS
#define ABS(x) ((x)<0 ? -(x) : (x))
#endif

#ifndef MIN
#define MIN(x,y) ((x)<(y) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) ((x)>(y) ? (x) : (y))
#endif


using namespace std;

///////////////////////////////////////////////////////////////////
// DEFINITION DES STRUCTURES DE DONNEES
///////////////////////////////////////////////////////////////////

// type Point2 : point du plan
struct Point2
{
	double x,y;
	// constructeurs
	Point2()
	{
		x=y=0.0;
	}
	Point2(double x0, double y0)
	{
		x=x0; y=y0;
	}
};


typedef list<Point2> lPoints;


double dist(Point2 p1,Point2 p2){
	return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
}


/**
 *Exécute l'algorithme de Douglas-Parker pour la distance d sur la liste de points lpts.
 *Lorsque la procédure est lancée, deb doit pointer sur le premier élément de la liste,
 *et fin doit pointer sur le dernier élément de la liste et non sur lpts.end().
 */
void douglas(double d,std::list<Point2> & lpts,std::list<Point2>::iterator deb,std::list<Point2>::iterator fin){
	if(deb!=fin){
		double a,b,h,l,s,dj;
		dj=0;
		std::list<Point2>::iterator it=deb;
		std::list<Point2>::iterator ptLoin=deb;

		it++;
		if(it!=fin){
			while(it!=fin){
				a=dist((*deb),(*it));
				b=dist((*fin),(*it));
				l=dist((*deb),(*fin));
				s=(a*a-b*b+l*l)/(2*l);

				if(0<=s && s<=l){
					h=sqrt(a*a-s*s);
				}
				else if(s<0){
					h=a;
				}
				else{
					h=b;
				}

				if(h>dj){
					dj=h;
					ptLoin=it;
				}

				it++;
			}

			if(dj<d){
				it=deb;
				it++;
				lpts.erase(it,fin);
			}
			else{
				douglas(d,lpts,deb,ptLoin);
				douglas(d,lpts,ptLoin,fin);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
// ROUTINES D'ENTREE-SORTIE SOUS FORME DE FICHIER
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// création d'un fichier image à partir d'une polyline P
// (polygone ouvert ou fermé)
// Entrée : nom_f = nom du fichier
//          P = liste des points de la polyline
//          echelle = réel > 0
//           si d est le diamètre de la liste de points P
//           (d = plus grande distance entre deux points de P)
//           alors la taille du fichier image sera de d*echelle/1200 pouces
//           ou bien environ d*echelle/480 cm
//          couleur = entier entre 0 et 7 (cf. couleurs prédéfinies ci-dessous)
//          format_image = "eps", "jpeg" ou "gif"
#define XFIG_COLOR_BLACK 0
#define XFIG_COLOR_BLUE 1
#define XFIG_COLOR_GREEN 2
#define XFIG_COLOR_CYAN 3
#define XFIG_COLOR_RED 4
#define XFIG_COLOR_MAGENTA 5
#define XFIG_COLOR_YELLOW 6
#define XFIG_COLOR_WHITE 7

// écriture d'un objet de type polyline dans un fichier au format XFIG
// Entrée : f = identificateur de fichier
//          P = liste de points de la polyline
//          echelle = facteur de zoom (réel > 0)
//          couleur = entier entre 0 et 7
void XFIG_polyline(FILE *f, lPoints P, double echelle, int couleur)
{
	// nombre de points de la polyline
	int n=P.size();

	// écriture de l'instruction XFIG correspondante
	fprintf(f, "2 1 0 1 %d 7 50 -1 -1 0.000 0 0 -1 0 0 %d\n", couleur, n);
	for (lPoints::iterator i=P.begin(); i!=P.end(); i++)
		fprintf(f, " %d %d\n", (int)(i->x*echelle), -(int)(i->y*echelle));
}

// écriture d'un fichier au format XFIG avec un objet de type polyline
// Entrée : f = identificateur de fichier
//          P = liste de points de la polyline
//          echelle = facteur de zoom (réel > 0)
//          couleur = entier entre 0 et 7
void ecrire_polyline_XFIG(const char *nom_f, lPoints P, double echelle,
  int couleur)
{
	// ouverture du fichier
	FILE *f = fopen(nom_f, "w");

	// entete du fichier XFIG
	fprintf(f, "#FIG 3.2\n");
	fprintf(f, "Landscape\n");
	fprintf(f, "Center\n");
	fprintf(f, "Metric\n");
	fprintf(f, "A4    \n");
	fprintf(f, "100.00\n");
	fprintf(f, "Single\n");
	fprintf(f, "-2\n");
	fprintf(f, "1200 2\n");

	// écriture de la polyline P dans le fichier f
	XFIG_polyline(f, P, echelle, couleur);

	// fermeture du fichier
	fclose(f);
}

// création d'un fichier image au format eps, jpeg ou gif
// représentant une polyline P
// Entrée : f = identificateur de fichier
//          P = liste de points de la polyline
//          echelle = facteur de zoom (réel > 0)
//          couleur = entier entre 0 et 7
//          format = chaine de caractère "eps", "jpeg" ou "gif"
void image_polyline(const char *nom_f, lPoints P,
  double echelle, int couleur, const char *format_image)
{
	char nom_f_XFIG[1000], commande[1000];
	sprintf(nom_f_XFIG, "%s.fig", nom_f);
	ecrire_polyline_XFIG(nom_f_XFIG, P, echelle, couleur);
	if (strcmp(format_image,"eps")==0)
		sprintf(commande, "fig2dev -L eps %s %s", nom_f_XFIG, nom_f);
	else if (strcmp(format_image,"jpeg")==0)
		sprintf(commande, "fig2dev -L jpeg -q 90 %s %s", nom_f_XFIG, nom_f);
	else if (strcmp(format_image,"gif")==0)
		sprintf(commande, "fig2dev -L gif %s %s", nom_f_XFIG, nom_f);
	else
		sprintf(commande, "echo \"Format inconnu\"");
	system(commande);
	sprintf(commande, "rm -f %s", nom_f_XFIG);
	system(commande);
}

///////////////////////////////////////////////////////////////////
// lecture d'une polyline dans un fichier texte
// contenant une liste de points 2D
// Entrée : nom_f = nom du fichier de points
//           un point par ligne : 2 valeurs réelles par ligne
// La routine renvoie la liste de points lue.
lPoints lire_polyline(const char *nom_f)
{
	FILE *f = fopen(nom_f, "r");
	lPoints L;
	if (f != (FILE *)NULL)
	{
		while (true)
		{
			double x,y;
			int res = fscanf(f, "%lf %lf", &x, &y);
			if (res != 2) break;
			Point2 P;
			P.x = x; P.y = y;
			L.push_back(P);
		}
		fclose(f);
	}
	return L;
}

///////////////////////////////////////////////////////////////////
// ROUTINE PRINCIPALE
///////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	// lecture du fichier France.data et création des fichiers image
	// en différents formats
	// choisir l'échelle en fonction des données
	// Pour le TP, les différents jeux de donnée sont des polylines
	// dont les dimensions sont d'environ 2000
	// -> choisir une échelle entre 2 et 4 pour les fichiers images
	int d=atoi(argv[1]);
	lPoints L = lire_polyline(argv[2]);

	std::list<Point2>::iterator deb=L.begin();
	std::list<Point2>::iterator fin=L.end();fin--;

	std::string compressS(argv[3]);
	compressS+=".txt";
	std::ofstream compres(compressS.c_str(),std::ios::out);

	double tailleInit=L.size();
	douglas(d,L,deb,fin);
	double tailleFin=L.size();
	compres<<"taille initiale "<<tailleInit<<std::endl;
	compres<<"taille finale "<<tailleFin<<std::endl;
	compres<<"compression "<<tailleInit/tailleFin<<std::endl;

	compres.close();
// 	while(deb!=L.end()){
// 		std::cout<<"x "<<deb->x<<std::endl;
// 	}

	ecrire_polyline_XFIG(argv[3],L, 2,7);


	// 	image_polyline(argv[3], L, 2.0, XFIG_COLOR_BLACK, "eps");
//  	image_polyline(argv[3], L, 4.0, XFIG_COLOR_BLACK, "jpeg");
 	image_polyline(argv[3], L, 4.0, XFIG_COLOR_BLACK, "gif");

	return 0;
}

