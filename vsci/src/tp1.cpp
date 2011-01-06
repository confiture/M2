//////////////////////////////////////////////////////////////////
// Calcul de courbes iso-valeur sur une triangulation
//   compilation : g++ tp1.cpp -o tp1
//   exécution   : ./tp1
//////////////////////////////////////////////////////////////////

#include<cstdio>
#include<cstdlib>
#include<cstring>

// le type Point3
// les 3 coordonnées
struct Point3
{
	double x,y,z; // les 3 coordonnées
	Point3()
	{
		x=0.0; y=0.0; z=0.0;
	}
	Point3(double x0, double y0, double z0)
	{
		x=x0; y=y0; z=z0;
	}
};

// le type Couleur
struct Couleur
{
	double r,g,b; // composantes rouge, vert, bleu
	Couleur()
	{
		r=0.0; g=0.0; b=0.0;
	}
	Couleur(double r0, double g0, double b0)
	{
		r=r0; g=g0; b=b0;
	}
};

// le type Triangle = tableau de 3 double
// indices des 3 sommets du triangle
typedef int Triangle[3];

//////////////////////////////////////////////////////////////////
// routines de calcul sur les points

// calcul de A+B avec A,B de type Point3
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

//////////////////////////////////////////////////////////////////
// lecture d'un fichier au format OFF
// Entrée : nom_fichier = le nom du fichier
// Sortie : S = tableau des sommets
//          nS = le nombre de sommets
//          T = tableau des triangles
//              les indices étant compris entre 1 et nS
//          nT = le nombre de triangle
// Les tableaux S et T sont alloués dans cette routine
// La routine renvoie 0 si la lecture et la création des tableaux
// s'est déroulée correctement, une valeur différente de 0 sinon
int lire_fichier_OFF(const char *nom_fichier,
                     Point3* &S, int &nS, Triangle* &T, int &nT)
{
	FILE *f=fopen(nom_fichier,"r");
	if (f == (FILE*)NULL)
		return 1;

	char s[100];
	int res, nA;

	// lecture de l'entete
	res=fscanf(f,"%s\n",&s[0]);
	if (strncmp(s,"OFF",3)!=0 || res!=1)
	{
		fclose(f);
		fprintf(stderr, "lire_fichier_OFF : mauvais format de fichier\n");
		return 2;
	}

	// lecture des nombres de sommets et triangles
	res = fscanf(f,"%i %i %i", &nS, &nT, &nA);
	if (res != 3 || nS < 3 || nT < 1)
	{
		fclose(f);
		fprintf(stderr, "lire_fichier_OFF : mauvais format de fichier\n");
		return 2;
	}
	//fprintf(stdout, "nS = %i, nT = %i\n", nS, nT);

	// allocation des tableaux
	S = (Point3*)malloc(sizeof(Point3)*nS);
	if (S == (Point3*)NULL)
	{
		fclose(f);
		fprintf(stderr, "lire_fichier_OFF : mémoire insuffisante\n");
		return 3;
	}
	T = (Triangle*)malloc(sizeof(Triangle)*nT);
	if (T == (Triangle*)NULL)
	{
		fclose(f);
		free((void*)S);
		fprintf(stderr, "lire_fichier_OFF : mémoire insuffisante\n");
		return 3;
	}
	// lecture des sommets
	for (int i=0; i<nS; i++)
	{
		res = fscanf(f,"%lf %lf %lf", &S[i].x, &S[i].y, &S[i].z);
		if (res !=3)
		{
			fclose(f);
			free((void*)S);
			free((void*)T);
			fprintf(stderr, "lire_fichier_OFF : mauvais format de fichier\n");
			return 2;
		}
		//fprintf(stdout, "S[%d] : (%15.7e %15.7e %15.7e)\n", i, S[i].x, S[i].y, S[i].z);
	}
	// lecture des triangles
	for (int i=0; i<nT; i++)
	{
		int nst;
		res = fscanf(f,"%i %i %i %i", &nst, T[i], T[i]+1, T[i]+2);
		if (res !=4 || nst !=3)
		{
			fclose(f);
			free((void*)S);
			free((void*)T);
			fprintf(stderr, "lire_fichier_OFF : mauvais format de fichier\n");
			return 2;
		}
		//fprintf(stdout, "T[%d] : (%i %i %i)\n", i, T[i][0], T[i][1], T[i][2]);
	}

	fclose(f);
	return 0;

}


//////////////////////////////////////////////////////////////////
// ecriture dans un fichier VECT d'une liste de segments
// Entrée : E1,E2 : points extrémité des segments (tableaux de Point3)
//            le segment i est [E1[i],E2[i]]
//          N = nombre de segments
//          nom_fichier = le nom du fichier
//          coul = couleur (par défaut noir = {0,0,0})
// la routine renvoie 0 si le fichier a été créé correctement, 1 sinon
int ecrire_segments_VECT(Point3 E1[], Point3 E2[], int N,
                         const char* nom_fichier, Couleur coul=Couleur(0,0,0))
{
	// ouverture du fichier
	FILE* f=fopen(nom_fichier,"w");
	if (f==(FILE *)NULL)
	{
		fprintf(stderr, "ecrire_segments_VECT : fichier incorrect\n");
		return 1;
	}

	// écriture de l'en-tête
	fprintf(f, "VECT\n%i %i %i\n", N, N*2, N);
	fprintf(f,"\n");

	// nombre de points par segment : 2
	for (int i=0; i<N; i++)
		fprintf(f, " 2");
	fprintf(f, "\n\n");

	// nombre de couleurs par segment : 1
	for (int i=0; i<N; i++)
		fprintf(f, " 1");
	fprintf(f, "\n\n");

	// les différents points des segments
	for (int i=0; i<N; i++)
	{
		fprintf(f, " %15.7e %15.7e %15.7e\n", E1[i].x, E1[i].y, E1[i].z);
		fprintf(f, " %15.7e %15.7e %15.7e\n", E2[i].x, E2[i].y, E2[i].z);
		fprintf(f, "\n");
	}
	fprintf(f, "\n");

	// les couleurs
	for (int i=0; i<N; i++)
		fprintf(f, " %7.3f %7.3f %7.3f %7.3f\n",
		        coul.r, coul.g, coul.b, 1.0);

	// fermeture du fichier
	fclose(f);
	return 0;
}

/**
 *Retourne l'intersection du segment [p1,p2] avec le plan z=v.
 *
 */
Point3 intersection(const Point3& p1,const Point3& p2,int v){
	if(v==p1.z)return p1;
	if(v==p2.z)return p2;

	double a = (v-p2.z)/(p1.z-p2.z);
	return a*p1+(1-a)*p2;
}


//////////////////////////////////////////////////////////////////
// calcul d'isocourbe sur données triangulée
// Entrée : S = tableau des sommets, tableau de nS Point3
//          nS = nombre de sommets
//          T = tableau des triangles, tableau de nT triangles
//              les indices étant compris entre 0 et nS-1
//          nT = nombre de triangles
//          v = valeur de l'isocourbe
// Sortie : E1,E2 : tableaux de Point3 extrémité des segments
//            le segment i est [E1[i],E2[i]], 0<=i<N
//            les tableaux E1 et E2 sont alloués avec la taille maximale = nT
//          N : nombre de segments
// La routine renvoie 0 si tout se passe bien, 1 sinon
int calcul_isocourbe(Point3 *S, int nS, Triangle *T, int nT, double v,
                     Point3* &E1, Point3* &E2, int &N)
{
	// allocation mémoire des tableaux E1 et E2
	E1 = (Point3*)malloc(sizeof(Point3)*nT);
	if (E1==(Point3*)NULL)
	{
		fprintf(stderr, "calcul_isocourbe : memoire insuffisante\n");
		return 1;
	}
	E2 = (Point3*)malloc(sizeof(Point3)*nT);
	if (E2==(Point3*)NULL)
	{
		fprintf(stderr, "calcul_isocourbe : memoire insuffisante\n");
		free((void*)E1);
		return 1;
	}

	N=0;
	bool e2;
	for(int iT=0;iT<nT;iT++){
		e2=false;
		for(int ar=0;ar<3;ar++){
			if(S[T[iT][ar]].z==v && S[T[iT][(ar+1)%3]].z==v){
				E1[N]=S[T[iT][ar]];
				E2[N]=S[T[iT][(ar+1)%3]];
				N++;
				break;
			}
			else if((S[T[iT][ar]].z-v)*(S[T[iT][(ar+1)%3]].z-v)<=0){
				if(e2){
					E2[N]=intersection(S[T[iT][ar]],S[T[iT][(ar+1)%3]],v);
					N++;
					break;
				}
				else{
					E1[N]=intersection(S[T[iT][ar]],S[T[iT][(ar+1)%3]],v);
					e2=true;
				}
			}
		}
	}

	return 0;
}

	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	// routine principale
int main(int argc,char* argv[])
	{
		Point3 *S;
		int nS;
		Triangle *T;
		int nT;
		double v; // le plan des isovaleurs
		sscanf(argv[1],"%f",&v);
		char* ficE=argv[2];
		char* ficS=argv[3];

		// lecture du fichier Geomview au format OFF
		fprintf(stdout, "Lecture des données ...\n");
		if (lire_fichier_OFF(/*"ex_triangulation.off"*/ficE, S, nS, T, nT)!=0)
			return 1;

		// calcul de la courbe isovaleur sous la forme de nE segments
		// le segment d'indice i étant donné par ses 2 extrémités
		// E1[i] et E2[i]
		Point3 *E1;
		Point3 *E2;
		int nE;
		fprintf(stdout, "Calcul de la courbe isovaleur...\n");
		if (calcul_isocourbe(S,nS,T,nT,/*1.0*/v,E1,E2,nE)!=0)
		{
			free((void *)T);
			free((void *)S);
			return 2;
		}

		// écriture du fichier Geomview au format VECT
		fprintf(stdout, "Ecriture du fichier segments ...\n");
		if (ecrire_segments_VECT(E1,E2,nE,/*"ex_isoligne1.vect"*/ficS)!=0)
		{
			free((void *)T);
			free((void *)S);
			return 3;
		}

		// fin libération de la mémoire allouée précédemment
		fprintf(stdout, "TERMINE\n");
		free((void *)E2);
		free((void *)E1);
		free((void *)T);
		free((void *)S);
		return 0;
	}
