///////////////////////////////////////////////////////////////////
// librairie de base pour le mini-projet
// Partie pour la reconstruction de courbes - points en 2D
// Implementation

#include"lib_base2d.hpp"
#include <iostream>
// écrit la liste d'entiers I
void ecrire(vint I)
{
	printf("vint : %d valeur(s)\n", I.size());
	for (unsigned int i=0; i < I.size(); i++)
		printf(" %d", I[i]);
	printf("\n");
}

// ecrit les n points du tableau P
void ecrire(Point *P, unsigned int n)
{
	printf("vPoint : %d valeur(s)\n", n);
	for (unsigned int i=0; i < n; i++)
		printf(" %f %f\n", P[i].x, P[i].y);
}

// calcule le point égal à A+B
Point operator+(Point A, Point B)
{
	return Point(A.x + B.x , A.y + B.y);
}

// calcule le point égal à A-B
Point operator-(Point A, Point B)
{
	return Point(A.x - B.x , A.y - B.y);
}

// calcule le point égal à -A
Point operator-(Point A)
{
	return Point(-A.x , -A.y);
}

// calcule le point égal à r*A avec r scalaire réel
Point operator*(double r, Point A)
{
	return Point(r*A.x  , r*A.y);
}

// calcule le point égal à A*r avec r scalaire réel
Point operator*(Point A, double r)
{
	return Point(r*A.x  , r*A.y);
}

// calcule le point égal à A/r avec r scalaire réel
Point operator/(Point A, double r)
{
	return Point(A.x/r  , A.y/r);
}

// calcule le produit scalaire A.B
double dot(Point A, Point B)
{
	return (A.x * B.x + A.y * B.y);
}

// calcule la norme de A
double norm(Point A)
{
	return sqrt(A.x * A.x + A.y * A.y);
}

// calcule la distance entre A et B
double distance_point(Point A, Point B)
{
	return norm(A-B);
}

bool operator<(Arete a1,Arete a2){
	return a1.v<a2.v;
}

void droite_mc(const std::list<Point> & pts,Point & B,Point & v,Point & n){
	std::list<Point>::const_iterator it=pts.begin();
	std::list<Point>::const_iterator end=pts.end();
	B.x=0;
	B.y=0;
	for(it;it!=end;it++)
		B=B+(*it);

	B=B/pts.size();

	double A11,A12,A22;A11=A12=A22=0;
	for(it=pts.begin();it!=end;it++){
		A11+=(it->x-B.x)*(it->x-B.x);
		A12+=(it->x-B.x)*(it->y-B.y);
		A22+=(it->y-B.y)*(it->y-B.y);
	}

	double l1,l2;
	elements_propres_mat_sym(A11,A12,A22,l1,l2,n,v);
}

// calcule la boite englobante des n points du tableau P
// Entree : P = tableay de Point
//          n = nombre de points
// Sortie : Pxmin, Pxmax, Pymin, Pymax = bornes de la boite englobante
void boite_englobante(Point *P, int n,
   double &Pxmin, double &Pxmax, double &Pymin, double &Pymax)
{
	Pxmin = Pxmax = P[0].x;
	Pymin = Pymax = P[0].y;
	for (int i=1; i<n; i++)
	{
		if (Pxmin > P[i].x) Pxmin = P[i].x;
		if (Pymin > P[i].y) Pymin = P[i].y;
		if (Pxmax < P[i].x) Pxmax = P[i].x;
		if (Pymax < P[i].y) Pymax = P[i].y;
	}
}

// calcul des elements propres de la matrice carree symetrique A
//     ( A11 A12 )
// A = (         )
//     ( A12 A22 )
// Entrée : A11,A12,A22 = les trois valeurs définissant la matrice symetrique
// Sortie : l1,l2 = les deux valeurs propres avec l1 <= l2
//          v1,v2 =  les vecteurs propres unitaires associés
void elements_propres_mat_sym(double A11, double A12, double A22,
 double &l1, double &l2, Point &v1, Point &v2)
{
	if ( ABS(A12) < 1.0e-8*MAX(ABS(A11),ABS(A22)) )
	{
		// A12 considéré comme nul par rapport à A11 et A22
		// -> la matrice est diagonale :
		//  les valeurs propres sont les éléments de la diagonale
		//  les vecteurs propres sont les colonnes de la matrice identité
		if (A11 <= A22)
		{
			l1 = A11; v1.x = 1.0; v1.y = 0.0;
			l2 = A22; v2.x = 0.0; v2.y = 1.0;
		}
		else
		{
			l1 = A22; v1.x = 0.0; v1.y = 1.0;
			l2 = A11; v2.x = 1.0; v2.y = 0.0;
		}
	}
	else
	{
		// A12 != 0 -> les deux valeurs propres sont distinctes
		double delta = sqrt(A11*A11-2*A11*A22+A22*A22+4*A12*A12);
		double nV,x,y;
		double w1,w2,w3,w4;
		l1 = 0.5*(A11+A22-delta);
		l2 = 0.5*(A11+A22+delta);
		w1 = ABS(A11-l1);
		w2 = ABS(A22-l1);
		w3 = ABS(A11-l2);
		w4 = ABS(A22-l2);

		if (w1 >= w2 && w1 >= w3 && w1 >= w4)
		{
			// calcul de v1 à partir de A12 et A11-l1

			// calcul de v1 unitaire
			y = A11-l1; x = -A12;
			nV = sqrt(x*x+y*y);
			v1.x = x/nV;
			v1.y = y/nV;
			// calcul de v2 à partir de v1
			v2.x = -v1.y;
			v2.y =  v1.x;
		}
		else if (w2 >= w1 && w2 >= w3 && w2 >= w4)
		{
			// calcul de v1 à partir de A12 et A22-l1

			// calcul de v1 unitaire
			x = A22-l1; y = -A12;
			nV = sqrt(x*x+y*y);
			v1.x = x/nV;
			v1.y = y/nV;
			// calcul de v2 à partir de v1
			v2.x = -v1.y;
			v2.y =  v1.x;
		}
		else if (w3 >= w1 && w3 >= w2 && w3 >= w4)
		{
			// calcul de v2 à partir de A12 et A11-l2

			// calcul de v2 unitaire
			y = A11-l2; x = -A12;
			nV = sqrt(x*x+y*y);
			v2.x = x/nV;
			v2.y = y/nV;
			// calcul de v1 à partir de v2
			v1.x = -v2.y;
			v1.y =  v2.x;
		}
		else //if (w4 >= w1 && w4 >= w2 && w4 >= w3)
		{
			// calcul de v2 à partir de A12 et A22-l2

			// calcul de v2 unitaire
			x = A22-l2; y = -A12;
			nV = sqrt(x*x+y*y);
			v2.x = x/nV;
			v2.y = y/nV;
			// calcul de v1 à partir de v2
			v1.x = -v2.y;
			v1.y =  v2.x;
		}

	}
}

///////////////////////////////////////////////////////////////////
// calcule l'arbre couvrant minimal correspondant à
// d'un graphe à n sommets dont les aretes valuees sont triees
// par ordre croissant
Arbre calcule_ACM(Graphe G)
{
	int n = G.nS;
	Noeud *noeuds = new Noeud[n];

	std::cout<<"here 1"<<std::endl;
	for (int i=0; i<n; i++)
		noeuds[i].num = 0;
	std::cout<<"here 2"<<std::endl;

	int nb_aretes_ajoutees=0;
	int iA = 0;
	int num_sous_arbre=0;
	int nA = G.nA;
	Arete* A=G.a;
	while (nb_aretes_ajoutees<n-1 && iA<nA)
	{
  		// sommets de l'arete iA
  		int s1 = A[iA].s1;
		Arete ac;
		//A[iA]=ac;
  		int s2 = A[iA].s2;

  		// numeros associes aux sommets
  		// pour un sommet, le numero est :
  		//   >0 si le sommet appartient au sous-arbre de meme numero
  		//   =0 si le sommet n'appartient à aucun sous-arbre
		std::cout<<"s1 "<<s1<<std::endl;
		int num1 = noeuds[s1].num;
		int num2 = noeuds[s2].num;

		if (num1==0 && num2==0)
		{
		    // creer un nouveau sous-arbre réduit à l'arete [s1,s2]
		    num_sous_arbre++;
		    noeuds[s1].num = num_sous_arbre;
		    noeuds[s2].num = num_sous_arbre;

			// ajouter l'arete [s1,s2]
		    noeuds[s1].voisins.push_back(s2);
		    noeuds[s2].voisins.push_back(s1);
		    nb_aretes_ajoutees++;
		}
		else if (num1==0)
		{
			// ajouter le sommet s1 au sous-arbre de s2
			noeuds[s1].num = num2;

			// ajouter l'arete [s1,s2]
		    noeuds[s1].voisins.push_back(s2);
		    noeuds[s2].voisins.push_back(s1);
		    nb_aretes_ajoutees++;
		}
		else if (num2==0)
		{
			// ajouter le sommet s2 au sous-arbre de s1
			noeuds[s2].num = num1;

			// ajouter l'arete [s1,s2]
		    noeuds[s1].voisins.push_back(s2);
		    noeuds[s2].voisins.push_back(s1);
		    nb_aretes_ajoutees++;
		}
		else if (num1 != num2)
		{
			// fusionner les arbres de s1 et s2 :
			// changer le numero de l'arbre contenant s2
			// i.e. parcourir l'arbre a partir de s2 et
			// remplacer le numero des noeuds par num1
			lint L;
			L.push_back(s2);
			while (!L.empty())
			{
				int s = L.back();
				L.pop_back();
				noeuds[s].num = num1;
				for (unsigned int k=0; k<noeuds[s].voisins.size(); k++)
				{
					int j = noeuds[s].voisins[k];
					if (noeuds[j].num == num2)
						L.push_back(j);
				}
			}

			// ajouter l'arete [s1,s2]
		    noeuds[s1].voisins.push_back(s2);
		    noeuds[s2].voisins.push_back(s1);
		    nb_aretes_ajoutees++;
		}
		printf("iA = %4d, nb_aretes_ajoutees = %d, n = %d\n",
		  iA, nb_aretes_ajoutees, n);
		iA++;
	}

	// ecrit l'ACM
	for (int i=0; i<n; i++)
	{
		printf("sommet %d (%d) : [", i, noeuds[i].num);
		for (int k=0; k<noeuds[i].voisins.size(); k++)
		{
			int j = noeuds[i].voisins[k];
			printf(" %d",j);
		}
		printf(" ]\n");
	}

// 	retourner l'arbre CM
	Arbre ACM;
	ACM.n = n;
	ACM.noeuds = noeuds;
	return ACM;
}


