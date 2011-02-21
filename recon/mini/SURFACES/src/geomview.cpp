///////////////////////////////////////////////////////////////////////////////
// LIBRAIRIE POUR LA CREATION D'OBJETS AU FORMAT GEOMVIEW 
// Implementation
///////////////////////////////////////////////////////////////////////////////

#include"geomview.hpp"

FILE *GEOMVIEW_file;

///////////////////////////////////////////////////////////////////////////////
// ouverture du fichier Geomview 
// renvoie 0 en cas de pb, 1 sinon
int GEOMVIEW_open(const char *nom)
{
	GEOMVIEW_file = fopen(nom, "w");
	if  (GEOMVIEW_file==(FILE *)NULL)
		return 0;
	fprintf(GEOMVIEW_file, "{\nLIST\n");
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// fermeture du fichier Geomview
void GEOMVIEW_close()
{
	fprintf(GEOMVIEW_file, "}\n");
	fclose(GEOMVIEW_file);
}

///////////////////////////////////////////////////////////////////////////////
// ajout de n points P[i] de couleur c et épaisseur e
// Entrée : P = tableau de n Point
//          n = nb de points 
//          c = couleur des points
//          e = épaisseur des points pour la visu (par défaut 1)
void GEOMVIEW_points(Point *P, ULONG n, Couleur c, unsigned int e)
{
	// début de l'objet composite
	fprintf(GEOMVIEW_file,"{\n");

	// attributs pour l'épaisseur et la couleur
	fprintf(GEOMVIEW_file,"  appearance {\n");
	fprintf(GEOMVIEW_file,"    linewidth %u\n",e);
	fprintf(GEOMVIEW_file,"    material {edgecolor %6.3f %6.3f %6.3f}\n",
	  c.r, c.g, c.b);
	fprintf(GEOMVIEW_file,"  }\n");

	// objet VECT : n polyline avec chacune 1 sommet et 0 couleur
	ULONG i;
	fprintf(GEOMVIEW_file, "{\n");
	fprintf(GEOMVIEW_file, "VECT\n%li %li 0\n", n, n);
	for (i=0; i<n; i++)
		fprintf(GEOMVIEW_file, "1 ");
	fprintf(GEOMVIEW_file, "\n");
	for (i=0; i<n; i++)
		fprintf(GEOMVIEW_file, "0 ");
	fprintf(GEOMVIEW_file, "\n");
	for (i=0; i<n; i++)
		fprintf(GEOMVIEW_file, "%15.7e %15.7e %15.7e\n", 
		  P[i].x, P[i].y, P[i].z);
		
	fprintf(GEOMVIEW_file, "}\n"); // fin objet VECT
	fprintf(GEOMVIEW_file, "}\n"); // fin objet composite
}

///////////////////////////////////////////////////////////////////////////////
// ecriture dans un fichier 
// de n points P[i] de couleur c et épaisseur e
// Entrée : nom_fichier = le nom du fichier
//          P = tableau de n Point
//          n = nb de points 
//          c = couleur des points
//          e = épaisseur des points pour la visu (par défaut 1)
void GEOMVIEW_points(const char *nom_fichier, 
  Point *P, ULONG n, Couleur c, unsigned int e)
{
	if (GEOMVIEW_open(nom_fichier)==0) return;
	GEOMVIEW_points(P, n, c, e);
	GEOMVIEW_close();
}

///////////////////////////////////////////////////////////////////////////////
// ajout de n segments P1[i],P2[i] de couleur c et épaisseur e
// Entrée : P1,P2 = tableaux de n Point
//          n = nb de segments 
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
void GEOMVIEW_lines(Point *P1, Point *P2, ULONG n, Couleur c, unsigned int e)
{
	// début de l'objet composite
	fprintf(GEOMVIEW_file,"{\n");

	// attributs pour l'épaisseur et la couleur
	fprintf(GEOMVIEW_file,"  appearance {\n");
	fprintf(GEOMVIEW_file,"    linewidth %u\n",e);
	fprintf(GEOMVIEW_file,"    material {edgecolor %6.3f %6.3f %6.3f}\n",
	  c.r, c.g, c.b);
	fprintf(GEOMVIEW_file,"  }\n");

	// objet VECT : n polyline avec chacune 2 sommet et 0 couleur
	ULONG i;
	fprintf(GEOMVIEW_file, "{\n");
	fprintf(GEOMVIEW_file, "VECT\n%li %li 0\n", n, 2*n);
	for (i=0; i<n; i++)
		fprintf(GEOMVIEW_file, "2 ");
	fprintf(GEOMVIEW_file, "\n");
	for (i=0; i<n; i++)
		fprintf(GEOMVIEW_file, "0 ");
	fprintf(GEOMVIEW_file, "\n");
	for (i=0; i<n; i++)
		fprintf(GEOMVIEW_file, "%15.7e %15.7e %15.7e  %15.7e %15.7e %15.7e\n", 
		  P1[i].x, P1[i].y, P1[i].z,  P2[i].x, P2[i].y, P2[i].z);
		
	fprintf(GEOMVIEW_file, "}\n"); // fin objet VECT
	fprintf(GEOMVIEW_file, "}\n"); // fin objet composite
}

///////////////////////////////////////////////////////////////////////////////
// ecriture dans un fichier 
// de n segments P1[i],P2[i] de couleur c et épaisseur e
// Entrée : nom_fichier = le nom du fichier
//          P1,P2 = tableaux de n Point
//          n = nb de segments 
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
void GEOMVIEW_lines(const char *nom_fichier, 
  Point *P1, Point *P2, ULONG n, Couleur c, unsigned int e)
{
	if (GEOMVIEW_open(nom_fichier)==0) return;
	GEOMVIEW_lines(P1, P2, n, c, e);
	GEOMVIEW_close();
}

///////////////////////////////////////////////////////////////////////////////
// ajout d'un segment P1,P2 de couleur c et épaisseur e
// Entrée : P1,P2 = Point
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
void GEOMVIEW_line(Point P1, Point P2, Couleur c, unsigned int e)
{
	GEOMVIEW_lines(&P1,&P2,1,c,e);
}

///////////////////////////////////////////////////////////////////////////////
// ecriture dans un fichier 
// d'un segment P1,P2 de couleur c et épaisseur e
// Entrée : nom_fichier = le nom du fichier
//          P1,P2 = Point
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
void GEOMVIEW_line(const char *nom_fichier, 
  Point P1, Point P2, Couleur c, unsigned int e)
{
	if (GEOMVIEW_open(nom_fichier)==0) return;
	GEOMVIEW_line(P1, P2, c, e);
	GEOMVIEW_close();
}

///////////////////////////////////////////////////////////////////
// ajout d'un arbre A dans un fichier Geomview
// Entrée : A = arbre
//          P = tableau des coordonnées des noeuds de l'arbre
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
void GEOMVIEW_Arbre(Arbre A, Point *P, Couleur c, unsigned int e)
{
	// détermination des segments
	ULONG nA = A.n - 1; // nombre d'aretes de l'arbre
	Point P1[nA], P2[nA];
	
	// parcours de l'arbre
	list<ULONG> L;
	vector<bool> marque(A.n,false);
	
	L.push_back(0);
	ULONG iA=0;
	while (!L.empty())
	{
		// retirer un sommet de la liste L et le marquer
		ULONG s = L.back();
		L.pop_back();
		marque[s] = true;
		
		// parcourir les voisins de s
		vULONG voisins = A.noeuds[s].voisins;
		for (unsigned int j=0; j<voisins.size(); j++)
		{
			ULONG s1 = voisins[j];
			if (!marque[s1])
			{
				// nouvelle arete parcourue
				// l'ajouter au tableaux P1 et P2
				P1[iA] = P[s ];
				P2[iA] = P[s1];
				iA++;
				
				// ajouter s1 à L
				L.push_back(s1);
			}
		}
	}
	
	// tracé des nA aretes
	GEOMVIEW_lines(P1, P2, nA, c, e);
}

///////////////////////////////////////////////////////////////////
// ecriture dans un fichier Geomview 
// d'un arbre A
// Entrée : nom_fichier = le nom du fichier
//          A = arbre
//          P = tableau des coordonnées des noeuds de l'arbre
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
void GEOMVIEW_Arbre(const char *nom_fichier, Arbre A, Point *P,
  Couleur c, unsigned int e)
{
	if (GEOMVIEW_open(nom_fichier)==0) return;
	GEOMVIEW_Arbre(A, P, c, e);
	GEOMVIEW_close();
}

///////////////////////////////////////////////////////////////////
// ajout d'un graphe G dans un fichier Geomview
// Entrée : G = graphe
//          P = tableau des coordonnées des noeuds de l'arbre
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
void GEOMVIEW_Graphe(Graphe G, Point *P, Couleur c, unsigned int e)
{
	// détermination des segments
	ULONG nA = G.nA; // nombre d'aretes de l'arbre
	Point P1[nA], P2[nA];
	
	// parcours du graphe
	for (ULONG i=0; i<nA; i++)
	{
		P1[i] = P[G.a[i].s1];
		P2[i] = P[G.a[i].s2];
	}
	
	// tracé des nA aretes
	GEOMVIEW_lines(P1, P2, nA, c, e);
}

///////////////////////////////////////////////////////////////////
// ajout d'un graphe G dans un fichier Geomview
// Entrée : nom_fichier = le nom du fichier
//          G = graphe
//          P = tableau des coordonnées des noeuds de l'arbre
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
void GEOMVIEW_Graphe(const char *nom_fichier, Graphe G, Point *P,
  Couleur c, unsigned int e)
{
	if (GEOMVIEW_open(nom_fichier)==0) return;
	GEOMVIEW_Graphe(G, P, c, e);
	GEOMVIEW_close();
}

///////////////////////////////////////////////////////////////////
// ajout à un fichier Geomview d'un ensemble de points+vecteurs
// Entrée : nom_fichier = le nom du fichier
//          P,N = tableaux de n points
//          n = nombre de points
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
//          h = facteur d'échelle pour les vecteurs (par défaut 1.0)
// La routine trace les n segments [P[i],P[i]+h*N[i]]
void GEOMVIEW_points_vects(Point *P, Point *N, ULONG n,
  Couleur c, unsigned int e, double h)  
{
	// détermination des segments
	Point P2[n];
	
	// parcours du graphe
	for (ULONG i=0; i<n; i++)
		P2[i] = P[i]+h*N[i];
	
	// tracé des nA aretes
	GEOMVIEW_lines(P, P2, n, c, e);
}

///////////////////////////////////////////////////////////////////
// écriture dans un fichier Geomview 
// d'un ensemble de points+vecteurs
// Entrée : nom_fichier = le nom du fichier
//          P,N = tableaux de n points
//          n = nombre de points
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
//          h = facteur d'échelle pour les vecteurs (par défaut 1.0)
// La routine trace les n segments [P[i],P[i]+h*N[i]]
void GEOMVIEW_points_vects(const char *nom_fichier, Point *P, Point *N, ULONG n,
  Couleur c, unsigned int e, double h)
{
	if (GEOMVIEW_open(nom_fichier)==0) return;
	GEOMVIEW_points_vects(P, N, n, c, e, h);
	GEOMVIEW_close();
}

///////////////////////////////////////////////////////////////////
// ecriture dans un fichier ou ajout à un fichier Geomview
// d'une triangulation
// Entrée : nom_fichier = le nom du fichier
//          S = tableaux de nS Point (les sommets)
//          nS = nombre de points
//          T = tableaux de nT Triangle (les triangles)
//          nT = nombre de points
//          col_face = couleur des faces
//          col_edge = couleur des aretes
//          e = épaisseur des aretes pour la visu (par défaut 1)
void GEOMVIEW_triangulation(Point *S, ULONG nS, Triangle *T, ULONG nT,
  Couleur col_face, Couleur col_edge, unsigned int e)  
{
	// début de l'objet composite
	fprintf(GEOMVIEW_file,"{\n");

	// attributs pour l'épaisseur et les couleurs
	fprintf(GEOMVIEW_file,"  appearance {\n");
	fprintf(GEOMVIEW_file,"    +edge\n");
	fprintf(GEOMVIEW_file,"    linewidth %u\n",e);
	fprintf(GEOMVIEW_file,"    material {\n");
	fprintf(GEOMVIEW_file,"      edgecolor %6.3f %6.3f %6.3f\n",
	  col_edge.r, col_edge.g, col_edge.b);
	fprintf(GEOMVIEW_file,"      diffuse %6.3f %6.3f %6.3f\n",
	  col_face.r, col_face.g, col_edge.b);
	fprintf(GEOMVIEW_file,"      specular %6.3f %6.3f %6.3f\n",
	  col_face.r, col_face.g, col_edge.b);
	fprintf(GEOMVIEW_file,"    }\n");
	fprintf(GEOMVIEW_file,"  }\n");

	// objet au format OFF
	fprintf(GEOMVIEW_file,"{\n");
	fprintf(GEOMVIEW_file,"OFF\n%li %li 0\n", nS, nT);
	
	// écriture des sommets
	for (ULONG i=0; i<nS; i++)
		fprintf(GEOMVIEW_file," %15.7e %15.7e %15.7e\n", 
		  S[i].x, S[i].y, S[i].z);
	// écriture des triangles
	for (ULONG i=0; i<nT; i++)
		fprintf(GEOMVIEW_file,"3 %li %li %li\n", 
		  T[i].s1, T[i].s2, T[i].s3);

	// fin de l'objet OFF	
	fprintf(GEOMVIEW_file, "}\n"); // fin objet composite
	// fin de l'objet composite	
	fprintf(GEOMVIEW_file, "}\n"); // fin objet composite

}

///////////////////////////////////////////////////////////////////
// ecriture dans un fichier ou ajout à un fichier Geomview
// d'une triangulation
// Entrée : nom_fichier = le nom du fichier
//          S = tableaux de nS Point (les sommets)
//          nS = nombre de points
//          T = tableaux de nT Triangle (les triangles)
//          nT = nombre de points
//          col_face = couleur des faces
//          col_edge = couleur des aretes
//          e = épaisseur des aretes pour la visu (par défaut 1)
void GEOMVIEW_triangulation(const char *nom_fichier, 
  Point *S, ULONG nS, Triangle *T, ULONG nT,
  Couleur col_face, Couleur col_edge, unsigned int e)  
{
	if (GEOMVIEW_open(nom_fichier)==0) return;
	GEOMVIEW_triangulation(S, nS, T, nT, col_face, col_edge, e);
	GEOMVIEW_close();
}  
