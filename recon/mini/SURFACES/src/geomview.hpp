///////////////////////////////////////////////////////////////////////////////
// LIBRAIRIE POUR LA CREATION D'OBJETS AU FORMAT GEOMVIEW 
// Interface
///////////////////////////////////////////////////////////////////////////////

#ifndef _GEOMVIEW_HPP
#define _GEOMVIEW_HPP

#include"lib_base3d.hpp"

// couleur RGB
class Couleur
{
	public:
	double r,g,b; // composantes rouge, vert, bleu entre 0.0 et 1.0
	
	// constructeurs
	Couleur(): r(0.0),g(0.0),b(0.0){}
	Couleur(double r0, double g0, double b0): r(r0),g(g0),b(b0){}
};


///////////////////////////////////////////////////////////////////////////////
// ouverture du fichier Geomview 
// renvoie 0 en cas de pb, 1 sinon
int GEOMVIEW_open(const char *nom);

///////////////////////////////////////////////////////////////////////////////
// fermeture du fichier Geomview
void GEOMVIEW_close();

///////////////////////////////////////////////////////////////////////////////
// ecriture dans un fichier ou ajout à un fichier Geomview
// de n points P[i] de couleur c et épaisseur e
// Entrée : nom_fichier = le nom du fichier
//          P = tableau de n Point
//          n = nb de points 
//          c = couleur des points
//          e = épaisseur des points pour la visu (par défaut 1)
void GEOMVIEW_points(Point *P, ULONG n, Couleur c, unsigned int e=1);
void GEOMVIEW_points(const char *nom_fichier, 
  Point *P, ULONG n, Couleur c, unsigned int e=1);

///////////////////////////////////////////////////////////////////////////////
// ecriture dans un fichier ou ajout à un fichier Geomview
// de n segments P1[i],P2[i] de couleur c et épaisseur e
// Entrée : nom_fichier = le nom du fichier
//          P1,P2 = tableaux de n Point
//          n = nb de segments 
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
void GEOMVIEW_lines(Point *P1, Point *P2, ULONG n, Couleur c, unsigned int e=1);
void GEOMVIEW_lines(const char *nom_fichier, 
  Point *P1, Point *P2, ULONG n, Couleur c, unsigned int e=1);

///////////////////////////////////////////////////////////////////////////////
// ecriture dans un fichier ou ajout à un fichier Geomview
// d'un segment P1,P2 de couleur c et épaisseur e
// Entrée : nom_fichier = le nom du fichier
//          P1,P2 = Point
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
void GEOMVIEW_line(Point P1, Point P2, Couleur c, unsigned int e=1);
void GEOMVIEW_line(const char *nom_fichier, 
  Point P1, Point P2, Couleur c, unsigned int e=1);

///////////////////////////////////////////////////////////////////
// ecriture dans un fichier ou ajout à un fichier Geomview
// d'un arbre A
// Entrée : nom_fichier = le nom du fichier
//          A = arbre
//          P = tableau des coordonnées des noeuds de l'arbre
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
void GEOMVIEW_Arbre(Arbre A, Point *P, Couleur c, unsigned int e=1);
void GEOMVIEW_Arbre(const char *nom_fichier, Arbre A, Point *P,
  Couleur c, unsigned int e=1);

///////////////////////////////////////////////////////////////////
// ecriture dans un fichier ou ajout à un fichier Geomview
// d'un graphe G
// Entrée : nom_fichier = le nom du fichier
//          G = graphe
//          P = tableau des coordonnées des noeuds de l'arbre
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
void GEOMVIEW_Graphe(Graphe G, Point *P, Couleur c, unsigned int e=1);
void GEOMVIEW_Graphe(const char *nom_fichier, Graphe G, Point *P,
  Couleur c, unsigned int e=1);

///////////////////////////////////////////////////////////////////
// ecriture dans un fichier ou ajout à un fichier Geomview
// d'un ensemble de points+vecteurs
// Entrée : nom_fichier = le nom du fichier
//          P,N = tableaux de n points
//          n = nombre de points
//          c = couleur des segments
//          e = épaisseur des segments pour la visu (par défaut 1)
//          h = facteur d'échelle pour les vecteurs (par défaut 1.0)
// La routine trace les n segments [P[i],P[i]+h*N[i]]
void GEOMVIEW_points_vects(Point *P, Point *N, ULONG n,
  Couleur c, unsigned int e=1, double h=1.0);  
void GEOMVIEW_points_vects(const char *nom_fichier, Point *P, Point *N, ULONG n,
  Couleur c, unsigned int e=1, double h=1.0);  
  
///////////////////////////////////////////////////////////////////
// ecriture dans un fichier ou ajout à un fichier Geomview
// d'une triangulation T de sommets S
// chaque triangle T[j] est un triplet d'entiers {s1,s2,s3}
// indices des 3 sommets 
// Entrée : nom_fichier = le nom du fichier
//          S = tableaux de nS Point (les sommets)
//          nS = nombre de points
//          T = tableaux de nT Triangle (les triangles)
//          nT = nombre de points
//          col_face = couleur des faces (par défaut blanc)
//          col_edge = couleur des aretes (par défaut noir)
//          e = épaisseur des aretes pour la visu (par défaut 1)
// La routine trace les n segments [P[i],P[i]+h*N[i]]
void GEOMVIEW_triangulation(Point *S, ULONG nS, Triangle *T, ULONG nT,
  Couleur col_face=Couleur(1.0,1.0,1.0), Couleur col_edge=Couleur(0.0,0.0,0.0), 
  unsigned int e=1);  
void GEOMVIEW_triangulation(const char *nom_fichier, 
  Point *S, ULONG nS, Triangle *T, ULONG nT,
  Couleur col_face=Couleur(1.0,1.0,1.0), Couleur col_edge=Couleur(0.0,0.0,0.0), 
  unsigned int e=1);  
  
#endif
