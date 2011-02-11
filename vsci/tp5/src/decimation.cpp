///////////////////////////////////////////////////////////////////
// TP - Simplification de lignes polygonale par l'algorithme
//      de decimation
// compilation : g++ decimation.cpp -o decimation
// exécution   : ./decimation
///////////////////////////////////////////////////////////////////

#include<cstdio>
#include<cmath>
#include<list>
#include<iostream>
#include<cstdlib>
#include<cstring>

#ifndef ABS
#define ABS(x) ((x)<0 ? -(x) : (x))
#endif

#ifndef MIN
#define MIN(x,y) ((x)<(y) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) ((x)>(y) ? (x) : (y))
#endif

#ifndef MAXFLOAT
#define MAXFLOAT 1e100
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

struct DoubleListeElement
{
	Point2 p;
	double v;
	DoubleListeElement* polygone_prec;
	DoubleListeElement* polygone_suiv;
	DoubleListeElement* liste_triee_prec;
	DoubleListeElement* liste_triee_suiv;
};

struct Liste
{
	DoubleListeElement* front;
	DoubleListeElement* back;
	Liste() {front=back=NULL;}
};

Liste polygone;
Liste liste_triee;

///////////////////////////////////////////////////////////////////
// FONCTIONS CRITERE POUR LA DECIMATION
// pour trois sommets consecutifs S0 S1 S2, calcule un critere
// geometrique
///////////////////////////////////////////////////////////////////


// somme de la longueur des deux aretes |S0 S1|+|S0 S2|
double critere_longueur(Point2 S0, Point2 S1, Point2 S2)
{
	return sqrt((S0.x-S1.x)*(S0.x-S1.x)+(S0.y-S1.y)*(S0.y-S1.y))
		+ sqrt((S2.x-S1.x)*(S2.x-S1.x)+(S2.y-S1.y)*(S2.y-S1.y));
}

// deviation de l'angle plat |S0 S1|,|S0 S2|
double critere_angulaire(Point2 S0, Point2 S1, Point2 S2)
{
	double S0x = S0.x;
	double S0y = S0.y;
	double S1x = S1.x;
	double S1y = S1.y;
	double S2x = S2.x;
	double S2y = S2.y;
	double A01x = S1.x-S0.x;
	double A01y = S1.y-S0.y;
	double A21x = S1.x-S2.x;
	double A21y = S1.y-S2.y;
	double l01 = sqrt(A01x*A01x+A01y*A01y);
	double l21 = sqrt(A21x*A21x+A21y*A21y);
	if (l01==0.0 || l21==0.0) return 0.0;
	double v = -(A01x*A21x+A01y*A21y)/l01/l21;
	v = MIN(MAX(v,-1.0),1.0);
	return acos(v)*180.0/M_PI;
}

// distance du point S1 au segment [S0,S2]
double critere_deviation(Point2 S0, Point2 S1, Point2 S2)
{
	double S0x = S0.x;
	double S0y = S0.y;
	double S1x = S1.x;
	double S1y = S1.y;
	double S2x = S2.x;
	double S2y = S2.y;
	double A01x = S1.x-S0.x;
	double A01y = S1.y-S0.y;
	double A21x = S1.x-S2.x;
	double A21y = S1.y-S2.y;
	double A20x = S0.x-S2.x;
	double A20y = S0.y-S2.y;
	double a = sqrt(A01x*A01x+A01y*A01y);
	double b = sqrt(A21x*A21x+A21y*A21y);
	double l = sqrt(A20x*A20x+A20y*A20y);
	if (l==0.0) return a;
	double s = (a*a-b*b+l*l)/2/l;
	if (s<0) return a;
	if (s>l) return b;
	double v = MAX(a*a-s*s,0.0);
	return sqrt(v);
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
void lire_polyline(const char *nom_f, lPoints &L)
{
	FILE *f = fopen(nom_f, "r");
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
}

///////////////////////////////////////////////////////////////////
// ROUTINES DE GESTION DE LISTE
///////////////////////////////////////////////////////////////////

// ajoute l'element e en tete du polygone
void push_front_polygone(DoubleListeElement* e)
{
	e->polygone_prec = NULL;
	e->polygone_suiv = polygone.front;
	if (polygone.front)
		polygone.front->polygone_prec = e;
	polygone.front = e;
	if (!polygone.back)
		polygone.back = e;
}

// ajoute l'element e en tete de la liste triee
void push_front_liste_triee(DoubleListeElement* e)
{
	e->liste_triee_prec = NULL;
	e->liste_triee_suiv = liste_triee.front;
	if (liste_triee.front)
		liste_triee.front->liste_triee_prec = e;
	liste_triee.front = e;
	if (liste_triee.back)
		liste_triee.back = e;
}

// supprime l'element e des deux listes
void supprime_element(DoubleListeElement* e)
{
	DoubleListeElement* prec;
	DoubleListeElement* suiv;

	/* suppression du polygone */
	prec = e->polygone_prec;
	suiv = e->polygone_suiv;
	if (prec)
		prec->polygone_suiv = suiv;
	else
		polygone.front = suiv;
	if (suiv)
		suiv->polygone_prec = prec;
	else
		polygone.back = prec;

	/* suppression de la liste triee */
	prec = e->liste_triee_prec;
	suiv = e->liste_triee_suiv;
	if (prec)
		prec->liste_triee_suiv = suiv;
	else
		liste_triee.front = suiv;
	if (suiv)
		suiv->liste_triee_prec = prec;
	else
		liste_triee.back = prec;

	delete e;
}

// permute l'element e avec son suivant dans la liste triee
// renvoie vrai si la liste a ete modifiee
//  (e n'etait pas le dernier element)
// faux sinon (e etait le dernier element)
bool avance_element_liste_triee(DoubleListeElement* e)
{
	DoubleListeElement* prec;
	DoubleListeElement* suiv;
	DoubleListeElement* suiv_suiv;

	prec = e->liste_triee_prec;
	suiv = e->liste_triee_suiv;
	if (!suiv)
		return false;
	suiv_suiv = suiv->liste_triee_suiv;
	if (prec)
		prec->liste_triee_suiv = suiv;
	else
		liste_triee.front = suiv;
	suiv->liste_triee_prec = prec;
	suiv->liste_triee_suiv = e;
	e->liste_triee_prec = suiv;
	e->liste_triee_suiv = suiv_suiv;
	if (suiv_suiv)
		suiv_suiv->liste_triee_prec = e;
	else
		liste_triee.back = suiv;
	return true;
}

// permute l'element e avec son predecesseur dans la liste triee
// renvoie vrai si la liste a ete modifiee
//  (e n'etait pas le premier element)
// faux sinon (e etait le premier element)
bool recule_element_liste_triee(DoubleListeElement* e)
{
	DoubleListeElement* prec_prec;
	DoubleListeElement* prec;
	DoubleListeElement* suiv;

	prec = e->liste_triee_prec;
	suiv = e->liste_triee_suiv;
	if (!prec)
		return false;
	prec_prec = prec->liste_triee_prec;
	if (suiv)
		suiv->liste_triee_prec = prec;
	else
		liste_triee.back = prec;
	prec->liste_triee_suiv = suiv;
	prec->liste_triee_prec = e;
	e->liste_triee_prec = prec_prec;
	e->liste_triee_suiv = prec;
	if (prec_prec)
		prec_prec->liste_triee_suiv = e;
	else
		liste_triee.front = e;
	return true;
}

// permute les elements e1 et e2 dans la liste triee
void permute(DoubleListeElement* e1, DoubleListeElement* e2)
{
	if (e1==e2)
		return;
	if (e1->liste_triee_suiv==e2)
	{
		DoubleListeElement* e2_suiv = e2->liste_triee_suiv;
		DoubleListeElement* e1_prec = e1->liste_triee_prec;

		if (e1_prec)
			e1_prec->liste_triee_suiv = e2;
		else
			liste_triee.front = e2;
		e2->liste_triee_prec = e1_prec;
		e2->liste_triee_suiv = e1;
		e1->liste_triee_prec = e2;
		e1->liste_triee_suiv = e2_suiv;
		if (e2_suiv)
			e2_suiv->liste_triee_prec = e1;
		else
			liste_triee.back = e1;
		return;
	}
	if (e1->liste_triee_prec==e2)
	{
		DoubleListeElement* e1_suiv = e1->liste_triee_suiv;
		DoubleListeElement* e2_prec = e2->liste_triee_prec;

		if (e2_prec)
			e2_prec->liste_triee_suiv = e1;
		else
			liste_triee.front = e1;
		e1->liste_triee_prec = e2_prec;
		e1->liste_triee_suiv = e2;
		e2->liste_triee_prec = e1;
		e2->liste_triee_suiv = e1_suiv;
		if (e1_suiv)
			e1_suiv->liste_triee_prec = e2;
		else
			liste_triee.back = e2;
		return;
	}
	DoubleListeElement* e1_prec = e1->liste_triee_prec;
	DoubleListeElement* e1_suiv = e1->liste_triee_suiv;
	DoubleListeElement* e2_prec = e2->liste_triee_prec;
	DoubleListeElement* e2_suiv = e2->liste_triee_suiv;
	if (e1_prec)
		e1_prec->liste_triee_suiv = e2;
	else
		liste_triee.front = e2;
	e2->liste_triee_prec = e1_prec;
	e2->liste_triee_suiv = e1_suiv;
	if (e1_suiv)
		e1_suiv->liste_triee_prec = e2;
	else
		liste_triee.back = e2;
	if (e2_prec)
		e2_prec->liste_triee_suiv = e1;
	else
		liste_triee.front = e1;
	e1->liste_triee_prec = e2_prec;
	e1->liste_triee_suiv = e2_suiv;
	if (e2_suiv)
		e2_suiv->liste_triee_prec = e1;
	else
		liste_triee.back = e1;
}


// cree la liste de points a partir de la double liste polygone-liste_triee
// suivant l'ordre de la lsite polygone
void polygone_liste_triee2lPoints(lPoints &l)
{
	DoubleListeElement* e;
	l.clear();
	e = polygone.front;
	while (e)
	{
		l.push_back(e->p);
		e = e->polygone_suiv;
	}
}

// fonction de comparaison pour la creation de la liste triee
int compare_sommet(const void* p1, const void* p2)
{
	DoubleListeElement* e1 = *(DoubleListeElement**)p1;
	DoubleListeElement* e2 = *(DoubleListeElement**)p2;
	if (e1->v < e2->v) return 1;
	return -1;
}

// cree la double liste polygone-liste_triee a partir
// de la liste de points l
void lPoints2polygone_liste_triee(lPoints &l,
                                  double (*critere)(Point2,Point2,Point2))
{
	int k, n=l.size();
	DoubleListeElement* T[n];

	// initialiser la double liste polygone-liste_triee comme vide
	polygone.front = NULL;
	polygone.back = NULL;
	liste_triee.front = NULL;
	liste_triee.back = NULL;

	// inserer les elements de la liste l dans le polygone et le tableau T
	k = 0;
	for (lPoints::reverse_iterator i=l.rbegin(); i != l.rend(); i++)
	{
		DoubleListeElement* e = new DoubleListeElement;
		e->p = *i;
		push_front_polygone(e);
		T[k++] = e;
	}

	T[0]->v = T[n-1]->v = MAXFLOAT;
	// calculer les criteres pour chaque sommet interne
	for (k=1; k<n-1; k++)
	{
		T[k]->v = critere(T[k-1]->p, T[k]->p, T[k+1]->p);
	}

	qsort(T, n, sizeof(DoubleListeElement*), compare_sommet);

	// créer la liste triee suivant T
	for (k=0; k<n; k++)
		push_front_liste_triee(T[k]);


}


// algorithme de décimation sur la liste l
// Entrée : l = une liste polygonale
//          n = le nombre maximal de points à supprimer
//          critere = la fonction servant de critere pour la décimation
//            critere du type double(Point2,Point2,Point2)
// Sortie : lS = la liste polygonale simplifiee
void decimation(lPoints &l, int n, double seuil, lPoints &lS,
                double (*critere)(Point2,Point2,Point2))
{
	// creation de la structure double_liste
	lPoints2polygone_liste_triee(l, critere);

	// faire la décimation
	int i=1;
	while (i<=n && liste_triee.front->v < seuil)
	{

		// recuperer le premier element de la liste triee
		DoubleListeElement* e = liste_triee.front;
		// et ses deux voisins dans le polygone
		DoubleListeElement* e_prec = e->polygone_prec;
		DoubleListeElement* e_suiv = e->polygone_suiv;

		// supprimer le sommet e
		supprime_element(e);

		// calculer la nouvelle valeur pour e_prec
		e = e_prec;
		if (e != polygone.front)
			e->v = critere
				(e->polygone_prec->p, e->p, e->polygone_suiv->p);
		// repositionner e_prec dans la liste triee
		while (e->liste_triee_prec && e->liste_triee_prec->v > e->v)
			recule_element_liste_triee(e);
		while (e->liste_triee_suiv && e->liste_triee_suiv->v < e->v)
			avance_element_liste_triee(e);

		// calculer la nouvelle valeur pour e_prec
		e = e_suiv;
		if (e != polygone.back)
			e->v = critere
				(e->polygone_prec->p, e->p, e->polygone_suiv->p);
		// repositionner e_suiv dans la liste triee
		while (e->liste_triee_prec && e->liste_triee_prec->v > e->v)
			recule_element_liste_triee(e);
		while (e->liste_triee_suiv && e->liste_triee_suiv->v < e->v)
			avance_element_liste_triee(e);

		i++;
	}

	// creer la liste de points finale lS
	polygone_liste_triee2lPoints(lS);
}


///////////////////////////////////////////////////////////////////
// ROUTINE PRINCIPALE
///////////////////////////////////////////////////////////////////
int main()
{
	// le fichier de donnees
	char fichier_donnees[100];

	// la fonction critere utilisee pour la decimation
	double (*fonction_critere)(Point2,Point2,Point2);

	// le polygone initial
	lPoints l;
	// le polygone simplifie
	lPoints lS;

	int n;  // nombre de points dans le polygone initial
	int nS; // nombre de points dans le polygone simplifie
	int nMax; // nombre maximal de points a supprimer
	double taux_compression; // taux de compression

	double seuil;

	int choix;

	printf("\nChoix du fichier de donnees :\n");
	printf("  1 : France.data     \n");
	printf("  2 : arbre.data      \n");
	printf("  3 : demicercle.data \n");
	printf("  4 : fleur.data      \n");
	printf("  5 : hippocampe.data \n");
	printf("  entrer un nombre entre 1 et 5 : ");
	scanf("%i", &choix);
	switch (choix)
	{
	case 1 :
		strcpy(fichier_donnees, "France.data"); break;
	case 2 :
		strcpy(fichier_donnees, "arbre.data"); break;
	case 3 :
		strcpy(fichier_donnees, "demicercle.data"); break;
	case 4 :
		strcpy(fichier_donnees, "fleur.data"); break;
	default :
		strcpy(fichier_donnees, "hippocampe.data");
	}

	printf("\nChoix du critere de decimation :\n");
	printf("  1 : deviation\n");
	printf("  2 : longueur\n");
	printf("  3 : angle\n");
	printf("  entrer un nombre entre 1 et 3 : ");
	scanf("%i", &choix);
	switch (choix)
	{
	case 1 :
		fonction_critere = critere_deviation; break;
	case 2 :
		fonction_critere = critere_longueur; break;
	default :
		fonction_critere = critere_angulaire;
	}

	printf("\nChoix du critere d'arret :\n");
	printf("  1 : seuil\n");
	printf("  2 : taux de compression\n");
	printf("  entrer un nombre entre 1 et 2 : ");
	scanf("%i", &choix);
	switch (choix)
	{
	case 1 :
		printf("\nEntrer la valeur du seuil : ");
		scanf("%lf", &seuil);
		break;
	default :
		printf("\nEntrer la valeur du taux de compression : ");
		scanf("%lf", &taux_compression);
	}

	// creation du polygone initial
	printf("\nlecture des donnees ...\n");
	lire_polyline(fichier_donnees, l);
	image_polyline("poly_initial.eps", l, 2.0, XFIG_COLOR_BLACK, "eps");
	image_polyline("poly_initial.gif", l, 4.0, XFIG_COLOR_BLACK, "gif");
	n = l.size();
	switch (choix)
	{
	case 1 :
		nMax = n;
		break;
	default :
		seuil = MAXFLOAT*2.0;
		nMax = (int)(n*(taux_compression-1.0)/taux_compression);
	}

	// simplification par décimation
	printf("\nsimplification par decimation en cours ...\n");
	decimation(l, nMax, seuil, lS, fonction_critere);
	nS = lS.size();

	printf("\necriture du polygone simplifie ...\n");
	image_polyline("poly_simplifie.eps", lS, 2.0, XFIG_COLOR_BLACK, "eps");
	image_polyline("poly_simplifie.gif", lS, 4.0, XFIG_COLOR_BLACK, "gif");

	// calcul du taux de compresion
	taux_compression = (double)n/(double)nS;
	printf("taux de compression = (%5d/%5d) = %f\n", n, nS, taux_compression);
}
