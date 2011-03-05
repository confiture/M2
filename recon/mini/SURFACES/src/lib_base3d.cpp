///////////////////////////////////////////////////////////////////
// librairie de base pour le mini-projet
// Partie pour la reconstruction de courbes - points en 3D
// Implementation

#include"lib_base3d.hpp"

// écrit la liste d'entiers I
void ecrire(vint I)
{
	printf("vint : %d valeur(s)\n", I.size());
	for (unsigned int i=0; i < I.size(); i++)
		printf(" %d", I[i]);
	printf("\n");
}

// ecrit les n points du tableau P
void ecrire(Point *P, ULONG n)
{
	printf("vPoint : %li valeur(s)\n", n);
	for (ULONG i=0; i < n; i++)
		printf(" %f %f\n", P[i].x, P[i].y);
}

// calcule le point égal à A+B
Point operator+(Point A, Point B)
{
	return Point(A.x + B.x , A.y + B.y , A.z + B.z);
}

// calcule le point égal à A-B
Point operator-(Point A, Point B)
{
	return Point(A.x - B.x , A.y - B.y , A.z - B.z);
}

// calcule le point égal à -A
Point operator-(Point A)
{
	return Point(-A.x , -A.y , -A.z);
}

// calcule le point égal à r*A avec r scalaire réel
Point operator*(double r, Point A)
{
	return Point(r*A.x  , r*A.y  , r*A.z);
}

// calcule le point égal à A*r avec r scalaire réel
Point operator*(Point A, double r)
{
	return Point(r*A.x  , r*A.y  , r*A.z);
}

// calcule le point égal à A/r avec r scalaire réel
Point operator/(Point A, double r)
{
	return Point(A.x/r  , A.y/r  , A.z/r);
}

// calcule le produit scalaire A.B
double dot(Point A, Point B)
{
	return (A.x * B.x + A.y * B.y + A.z * B.z);
}

// calcule le produit vectoriel A x B
Point cross(Point A, Point B)
{
	return Point(
	  A.y * B.z - A.z * B.y,
	  A.z * B.x - A.x * B.z,
	  A.x * B.y - A.y * B.x);
}

// calcule la norme de A
double norm(Point A)
{
	return sqrt(A.x * A.x + A.y * A.y + A.z * A.z);
}

// calcule la distance entre A et B
double distance_point(Point A, Point B)
{
	return norm(A-B);
}

// calcule la boite englobante des n points du tableau P
// Entree : P = tableay de Point
//          n = nombre de points
// Sortie : Pxmin, Pxmax, Pymin, Pymax = bornes de la boite englobante
void boite_englobante(Point *P, ULONG n,
   double &Pxmin, double &Pxmax,
   double &Pymin, double &Pymax,
   double &Pzmin, double &Pzmax)
{
	Pxmin = Pxmax = P[0].x;
	Pymin = Pymax = P[0].y;
	Pzmin = Pzmax = P[0].z;
	for (ULONG i=1; i<n; i++)
	{
		if (Pxmin > P[i].x) Pxmin = P[i].x;
		if (Pymin > P[i].y) Pymin = P[i].y;
		if (Pzmin > P[i].z) Pzmin = P[i].z;
		if (Pxmax < P[i].x) Pxmax = P[i].x;
		if (Pymax < P[i].y) Pymax = P[i].y;
		if (Pzmax < P[i].z) Pzmax = P[i].z;
	}
}

/* racine cubique de x */
double sqrt3(double x)
{
	return (x>=0 ? pow(x,1.0/3.0) : -pow(-x,1.0/3.0));
}


// Construit la droite aux moindres carrres a partir de la liste
// de points pts. En sortie, B est le barycentre des points, v est le
// vecteur directeur de la droite, et n est la normale de la droite.
void plan_mc(const std::list<Point> & pts,Point & B,Point & n){
	std::list<Point>::const_iterator it=pts.begin();
	std::list<Point>::const_iterator end=pts.end();
	B.x=0;
	B.y=0;
	B.z=0;
	for(it;it!=end;it++)
		B=B+(*it);

	B=B/pts.size();

	double A11,A12,A13,A22,A23,A33;A11=A12=A13=A22=A23=A33=0;
	for(it=pts.begin();it!=end;it++){
		A11+=(it->x-B.x)*(it->x-B.x);
		A12+=(it->x-B.x)*(it->y-B.y);
		A13+=(it->x-B.x)*(it->z-B.z);
		A22+=(it->y-B.y)*(it->y-B.y);
		A23+=(it->y-B.y)*(it->z-B.z);
		A33+=(it->z-B.z)*(it->z-B.z);
	}

	double lmin;
	elements_propres_min_mat_sym(A11,A12,A13,A22,A23,A33,lmin,n);
}


/* racine cubique de z = a+i*b */
void sqrt3complexe(double a, double b, double *c, double *d)
{
	double rho, theta;
	rho = sqrt(a*a+b*b);
	theta = atan2(b,a);
	*c = sqrt3(rho)*cos(theta/3.0);
	*d = sqrt3(rho)*sin(theta/3.0);
}

/* calcul des racines d'un polynome de degré 3 à coeff. réels */
/* P(x) = x^3+a*x^2+b*x+c */
/* Entree : a,b,c = les coef réels de P */
/* Sortie : p1, p2, p3 = pointeurs vers 3 reels v1, v2, v3 */
/* La routine renvoie une valeur entière res 0 et 1 */
/*   res = 0 : il y a une racine réelle x1= v1 et deux racines complexes */
/*     conjuguées x2 = v2 + i*v3 et x3 = v2-i*v3 */
/*   res = 1 : les trois racines sont réelles vi=xi */
/* reference : http://fr.wikipedia.org/wiki/M%C3%A9thode_de_Cardan */
int racines_polynome_degre3(double a, double b, double c,
double *p1, double *p2, double *p3)
{
	double p, q;
	double delta;
	double x1, x2, x3;
	int res;
	double jRe = -0.5;
	double jIm = sqrt(3)/2.0;

	/* p, q coeff. de l'equation P(x) = X^3 + p*X + q = 0  avec x = X-a/3 */
	p = b-a*a/3.0;
	q = c+(2*a*a-9*b)*a/27.0;


	delta = q*q+p*p*p*4.0/27.0;

	if (delta > 0)
	{
		double u, v;
		u = sqrt3(0.5*(-q+sqrt(delta)));
		v = sqrt3(0.5*(-q-sqrt(delta)));
		x1 = u+v - a/3;
		x2 = jRe*(u+v) - a/3;
		x3 = jIm*(u-v);
		res=0;
	}
	else if (delta<0)
	{
		double uRe, uIm;
		sqrt3complexe(-q/2.0, sqrt(-delta)/2.0, &uRe, &uIm);
		x1 = 2*uRe - a/3;
		x2 = 2*(uRe*jRe-uIm*jIm) - a/3;
		x3 = 2*(uRe*jRe+uIm*jIm) - a/3;
		res=1;
	}
	else // cas delta=0
	{
		x1 = 2*sqrt3(-q/2.0) - a/3;
		x2 = x3 = -sqrt3(-q/2.0) - a/3;
		res=1;
	}
	*p1 = x1;
	*p2 = x2;
	*p3 = x3;
	return res;
}

// calcul de la plus petite valeur propre et du vecteur propre associé
// d'une matrice A symétrique de dimension 3
//     ( A11 A12 A13)
// A = ( A12 A22 A23)
//     ( A13 A23 A33)
// Entrée : A11,A12,A13,A22,A23,A33 = les six valeurs définissant
//             la matrice symetrique
// Sortie : lmin = la plus petite valeur propre de A
//          vmin = le vecteur propre unitaire associé à lmin
void elements_propres_min_mat_sym(double A11, double A12, double A13,
 double A22, double A23, double A33,
 double &lmin, Point &vmin)
{
	/* coefficients du polynome caractéristique */
	/* f(l) = l^3+a*l^2+b*l+c */
	/*      = (l-A11)*(l-A22)*(l-A33)-2*A12*A13*A23 */
	/*      + (A11-l)*A23^2 + (A22-l)*A13^2  + (A33-l)*A12^2 */
	double a,b,c;
	double l1,l2,l3,l;
	double v1,v2,v3,nv;
	double A21=A12;
	double A31=A13;
	double A32=A23;
	double delta1,delta2,delta3;
	double eps;
	double Amax;
	int res;

	/* normalisation pour que Max|Aij| = 1 */
	Amax = 0.0;
	if (Amax < ABS(A11)) Amax = ABS(A11);
	if (Amax < ABS(A12)) Amax = ABS(A12);
	if (Amax < ABS(A13)) Amax = ABS(A13);
	if (Amax < ABS(A22)) Amax = ABS(A22);
	if (Amax < ABS(A23)) Amax = ABS(A23);
	if (Amax < ABS(A33)) Amax = ABS(A33);
	if (Amax == 0.0)
	{
		/* A matrice nulle : lmin = 0 et v quelconque */
		lmin =0.0;
		vmin = Point(1.0,0.0,0.0);
		return;
	}

	/* diviser Aij par Amax */
	A11 /= Amax; A12 /= Amax; A13 /= Amax;
	A21 /= Amax; A22 /= Amax; A23 /= Amax;
	A31 /= Amax; A32 /= Amax; A33 /= Amax;

	eps=1.0e-10;

	a = -A11-A22-A33;
	b = A11*A22+A11*A33+A22*A33-A12*A12-A13*A13-A23*A23;
	c = A11*A23*A23+A22*A13*A13+A33*A12*A12-2*A12*A13*A23-A11*A22*A33;

	/* calcul des racines de f */
	res = racines_polynome_degre3(a, b, c, &l1, &l2, &l3);
	if (res==0)
		/* erreur numérique (l3 très petit) -> l3 nul */
		/* -> une racine double l3 = l2 */
		l3 = l2;

	/* l = min(li) */
	l = MIN(MIN(l1,l2),l3);

	/* matrice A = A-l*I de rang <= 2 */
	A11 -= l;
	A22 -= l;
	A33 -= l;

	/* calcul d'un vecteur propre associé à l : */
	/* résoudre le système Av = 0 avec v non nul */
	delta1 = A22*A33-A32*A23;
	delta2 = A33*A11-A13*A31;
	delta3 = A11*A22-A21*A12;
	if (ABS(delta1) >= ABS(delta2) &&
	    ABS(delta1) >= ABS(delta3) &&
	    ABS(delta1) >= eps)
	{
		/* calculer un vecteur propre non nul avec les lignes 2 et 3 de A */
		/*  en fixant la 1ère coordonnée égale à 1 */
		v2 = (A23*A31-A21*A33)/delta1;
		v3 = (A21*A23-A22*A31)/delta1;
		v1 = 1;
	}
	else if (ABS(delta2) >= ABS(delta3) &&
	         ABS(delta2) >= ABS(delta1) &&
	         ABS(delta2) >= eps)
	{
		/* calculer un vecteur propre non nul avec les lignes 1 et 3 de A */
		/*  en fixant la 2ème coordonnée égale à 1 */
		v3 = (A31*A12-A32*A11)/delta2;
		v1 = (A32*A31-A33*A12)/delta2;
		v2 = 1;
	}
	else if (ABS(delta3) >= eps)
	{
		/* calculer un vecteur propre non nul avec les lignes 1 et 2 de A */
		/*  en fixant la 3ème coordonnée égale à 1 */
		v1 = (A12*A23-A13*A22)/delta3;
		v2 = (A13*A12-A11*A23)/delta3;
		v3 = 1;
	}

	else
	/* les trois delta_i nuls : A est de rang <= 1 */
	{
		if (ABS(A11) >= ABS(A22) &&
		    ABS(A11) >= ABS(A33) &&
			ABS(A11) >= eps)
		{
			/* calculer un vecteur propre non nul avec la ligne 1 de A */
			/*  en fixant la 3ème coordonnée égale à 0 */
			v1 = -A12;
			v2 =  A11;
			v3 = 0;
		}
		else if (ABS(A22) >= ABS(A33) &&
		         ABS(A22) >= ABS(A11) &&
				 ABS(A22) >= eps)
		{
			/* calculer un vecteur propre non nul avec la ligne 2 de A */
			/*  en fixant la 1ère coordonnée égale à 0 */
			v2 = -A23;
			v3 =  A22;
			v1 = 0;
		}
		else if (ABS(A33) >= eps)
		{
			/* calculer un vecteur propre non nul avec la ligne 3 de A */
			/*  en fixant la 2ème coordonnée égale à 0 */
			v3 = -A31;
			v1 =  A33;
			v2 = 0;
		}
		else /* la matrice A est nulle */
		{
			/* A de rang 0 : v quelconque */
			v1 = 1; v2 = v3 = 0;
		}
	}

	/* normalisation du vecteur */
	nv = sqrt(v1*v1+v2*v2+v3*v3);
	vmin = Point(v1,v2,v3)/nv;
	lmin = l*Amax;
}

///////////////////////////////////////////////////////////////////
// calcule l'arbre couvrant minimal correspondant à
// d'un graphe à n sommets dont les aretes valuees sont triees
// par ordre croissant
Arbre calcule_ACM(Graphe G)
{
	ULONG n = G.nS;
	Noeud *noeuds = new Noeud[n];

	for (ULONG i=0; i<n; i++)
		noeuds[i].num = 0;

	ULONG nb_aretes_ajoutees=0;
	ULONG iA = 0;
	ULONG num_sous_arbre=0;
	ULONG nA = G.nA;
	Arete* A=G.a;
	while (nb_aretes_ajoutees<n-1 && iA<nA)
	{
  		// sommets de l'arete iA
  		ULONG s1 = A[iA].s1;
  		ULONG s2 = A[iA].s2;

  		// numeros associes aux sommets
  		// pour un sommet, le numero est :
  		//   >0 si le sommet appartient au sous-arbre de meme numero
  		//   =0 si le sommet n'appartient à aucun sous-arbre
		ULONG num1 = noeuds[s1].num;
		ULONG num2 = noeuds[s2].num;

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
				for (ULONG k=0; k<noeuds[s].voisins.size(); k++)
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
// 		printf("iA = %4d, nb_aretes_ajoutees = %d, n = %d\n",
// 		  iA, nb_aretes_ajoutees, n);
		iA++;
	}

// 	// ecrit l'ACM
// 	for (int i=0; i<n; i++)
// 	{
// 		printf("sommet %d (%d) : [", i, noeuds[i].num);
// 		for (int k=0; k<noeuds[i].voisins.size(); k++)
// 		{
// 			int j = noeuds[i].voisins[k];
// 			printf(" %d",j);
// 		}
// 		printf(" ]\n");
// 	}

	// retourner l'arbre CM
	Arbre ACM;
	ACM.n = n;
	ACM.noeuds = noeuds;
	return ACM;
}


///////////////////////////////////////////////////////////////////
// création d'un ensemble de points répartis uniformément sur une sphère
// Entrée : n = entier > 0 (nombre de parallèles)
// Sortie : S = le tableaux des points
//          nS = le nombre de points
// la routine renvoie 1 si l'ensemble de point a pu etre créé,
// 0 sinon (valeur de n trop grande)
int creer_points_sphere(ULONG n, Point* &S, ULONG &nS)
{
	if (n<1) n=1;

	// la différence de latitude entre 2 paralleles voisins
	double a = M_PI/(n+1.0);

	// la distance de base entre les points
	double d = sqrt(2-2*cos(a));

	// le nombre de points par latitude
	ULONG nPtslatitude[n+2];
	nS = 0;
	nS += nPtslatitude[0] = 1; // un point au pole Sud
	for (ULONG i=1; i<n+1; i++)
	{
		// latitude du parallele i
		double ai = i*a-M_PI*0.5;

		// circonférence du paralléle
		double l = cos(ai)*2*M_PI;

		// nombre de points sur le parallèle
		nS += nPtslatitude[i] = round(l/d);
	}
	nS += nPtslatitude[n+1] = 1; // un point au pole Nord

	// calcul des coordonnées des sommets
	S = (Point*)malloc(nS*sizeof(Point));
	if (S==(Point*)NULL)
	{
		return 0;
	}
	ULONG k=0;

	for (ULONG i=0; i<n+2; i++)
	{
		// latitude du parallele i
		double ai = i*a-M_PI*0.5;

		// rayon du paralléle
		double r = cos(ai);

		// hauteur du parallele
		double z = sin(ai);

		// les points sur le parallele i
		ULONG ni = nPtslatitude[i];

		// angle entre deux points consecutifs
		double bi = 2.0*M_PI/ni;
		for (ULONG j=0; j<ni; j++)
			S[k++] = Point(r*cos(j*bi),r*sin(j*bi),z);
	}
	return 1;
}
