/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/* Structure de données pour un maillage dans l'espace                       */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/

#ifndef _MAILLAGE_H_
#define _MAILLAGE_H_

#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<exception>
#include<list>
#include"geom3d.hpp"

using namespace std;

#ifndef ULONG
#define ULONG unsigned long
#endif

typedef ULONG ulong3[3];

/* constantes pour les pointeurs */
typedef ULONG ptr;
#define PTR_NONE (ptr)0x00000000
#define PTR_BAD  (ptr)0xFFFFFFFF

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

class Sommet;
class Arete;
class Face;
typedef Sommet *PtrSommet;
typedef Arete *PtrArete;
typedef Face *PtrFace;
typedef list<PtrSommet> lSommets;
typedef list<PtrArete>  lAretes;
typedef list<PtrFace>   lFaces;

#define SOMMET_NONE ((PtrSommet) 0x00000000)
#define SOMMET_BAD  ((PtrSommet) 0xFFFFFFFF)
#define ARETE_NONE  ((PtrArete)  0x00000000)
#define ARETE_BAD   ((PtrArete)  0xFFFFFFFF)
#define FACE_NONE   ((PtrFace)   0x00000000)
#define FACE_BAD    ((PtrFace)   0xFFFFFFFF)

/* sommet */
class Sommet
{
	ULONG m;    // marque pour les algos.
	Point3D c;  // les coordonnées
	lAretes la; // liste des aretes incidentes
	
public:
	Sommet(Point3D xy)              { c = xy; la.clear(); }
	Sommet()                        { la.clear(); }
	Sommet(Point3D xy, lAretes la0) { c = xy; la = la0; }
	Sommet(lAretes la0)             { la = la0; }
	void coord(Point3D C) { c = C; }
	void X(double x0) { c.x = x0; }
	void Y(double y0) { c.y = y0; }
	void Z(double z0) { c.z = z0; }
	Point3D coord() { return c; }
	double X() { return c.x; }
	double Y() { return c.y; }
	double Z() { return c.z; }
	void aretes(lAretes la0) {la = la0;}
	lAretes aretes() {return la;}
	lFaces faces();
	lSommets sommets();
	static Point3D barycentre(lSommets ls);
	void translater(Point3D P) { c += P; }
	void homothetie(double a) { c *= a; } ;
	int nb_sommets() { return la.size(); };
	int nb_aretes()  { return la.size(); };
	int nb_faces()  {  return la.size()-(estExterne ? 1 : 0); };
	void marque(ULONG m0) { m = m0; }
	ULONG marque() { return m; }
	PtrArete arete_initiale() { return la.front(); }
	PtrArete arete_finale()   { return la.back(); }
	PtrSommet sommet_adjacent(PtrArete a);
	PtrSommet sommet_adjacent_initial() 
		{return sommet_adjacent(arete_initiale());}
	PtrSommet sommet_adjacent_final()
		{return sommet_adjacent(arete_finale());}
	PtrArete arete_suivante_trigo(PtrArete a);
	PtrArete arete_suivante_trigo_inverse(PtrArete a);
	PtrArete arete_suivante_trigo(PtrFace t);
	PtrArete arete_suivante_trigo_inverse(PtrFace t);
	PtrFace face_suivant_trigo(PtrArete a);
	PtrFace face_suivant_trigo_inverse(PtrArete a);
	PtrFace face_suivant_trigo(PtrFace t);
	PtrFace face_suivant_trigo_inverse(PtrFace t);
	bool interne();
	bool incident(PtrArete a0);
	bool incident(PtrFace f0);
//	list<PtrArete> l_aretes_trigo();
//	list<PtrArete> l_aretes_trigo_inverse();
	list<PtrFace> l_faces_trigo();
	list<PtrFace> l_faces_trigo_inverse();
	void write(ostream & o);
	friend ostream & operator<<(ostream & o, Sommet &s);
	void printf(FILE *f = stdout);	
	ULONG num;
	PtrSommet prec, suiv; // les voisins dans la liste
	bool estExterne;
};

/* arete */
class Arete
{
	ULONG m;  // marque pour les algos.
	PtrSommet s1, s2;   // sommets incidents	
	PtrFace f1, f2;   // faces incidentes	
public:
	Arete() 
	{ s1 = s2 = SOMMET_NONE; f1 = f2 = FACE_NONE;}
	Arete(PtrSommet s01, PtrSommet s02) 
	{ s1 = s01; s2 = s02 ; f1 = f2 = FACE_NONE;}
	Arete(PtrFace f01, PtrFace f02) 
	{ s1 = s2 = SOMMET_NONE; f1 = f01; f2 = f02;}
	Arete(PtrSommet s01, PtrSommet s02, PtrFace f01, PtrFace f02) 
	{ s1 = s01; s2 = s02 ; f1 = f01; f2 = f02;}
	void sommet1(PtrSommet s0) { s1 = s0; }
	void sommet2(PtrSommet s0) { s2 = s0; }
	void sommets(PtrSommet s01, PtrSommet s02) { s1 = s01; s2 = s02; }
	PtrSommet sommet1() { return s1; }
	PtrSommet sommet2() { return s2; }
	void face1(PtrFace f0) { f1 = f0; }
	void face2(PtrFace f0) { f2 = f0; }
	void faces(PtrFace f01, PtrFace f02) { f1 = f01; f2 = f02; }
	PtrFace face1() { return f1; }
	PtrFace face2() { return f2; }
	bool interne() { return f1 != FACE_NONE && f2 != FACE_NONE; }
	bool incident(PtrSommet s0) { return s0 == s1 || s0 == s2; }
	bool incident(PtrFace f0) { return f0 == f1 || f0 == f2; }
	Point3D barycentre();
	void marque(ULONG m0) { m = m0; }
	ULONG marque() { return m; }
	void write(ostream & o);
	friend ostream & operator<<(ostream & o, Arete &a);
	void printf(FILE *f = stdout);	
	
	ULONG num;
	PtrArete prec, suiv; // les voisins dans la liste
	bool estExterne;
};

/* face */
class Face
{
	ULONG m;      // marque pour les algos.
	lAretes la;   // listes des aretes incidentes	
public:
	Face() { la.clear();}
	Face(lAretes la0) { la = la0;}
	void aretes(lAretes la0) { la = la0;}
	lSommets sommets();
	lAretes aretes() {return la;}
	int nb_sommets() { return la.size(); };
	int nb_aretes()  { return la.size(); };
	PtrArete arete_initiale() {return la.front();}
	PtrArete arete_finale()   {return la.back();}
	PtrSommet sommet_initial() 
	{
		return la.front()->sommet1();
	}
	PtrArete arete_suivante_trigo(PtrArete a);
	PtrArete arete_suivante_trigo_inverse(PtrArete a);
	bool incident(PtrSommet s0);
	bool incident(PtrArete a0);
	Point3D barycentre();
	void marque(ULONG m0) { m = m0; }
	ULONG marque() { return m; }
	void write(ostream & o);
	friend ostream & operator<<(ostream & o, Face &f)	;
	void printf(FILE *f = stdout);	

ULONG num;
	PtrFace prec, suiv; // les voisins dans la liste

};

/* triangulation */
class Maillage
{
protected :
	ULONG nb_sommet; // nb de sommets
	ULONG nb_arete ; // nb d'aretes
	ULONG nb_face  ; // nb de faces
	PtrSommet s_first, s_last; // liste des sommets : debut et fin
	PtrArete  a_first, a_last; // liste des aretes  : debut et fin
	PtrFace   f_first, f_last; // liste des faces   : debut et fin
//	void creer_NGB(const char *nom_fichier);
	void creer_OFF(const char *nom_fichier);
//	void creer(Point3D *S, ULONG nS, ulong3 *T, ULONG nT);
	void creer(Point3D *S, ULONG nS, ULONG *T, ULONG *TS, ULONG nT);
	void supprimer_elements();
	void lire_off (const char *nom_fichier, 
	  Point3D* &S, ULONG &nS, ULONG* &T, ULONG* &TS, ULONG &nT);

public :
//	static const int NGB_FILE=1;
	static const int OFF_FILE=2;
	Maillage();
 	Maillage(double S[][3], ULONG nS, ULONG T[], ULONG nT);
 	Maillage(Point3D *S, ULONG nS, ULONG *T, ULONG nT);
	Maillage(double S[][3], ULONG nS, ULONG T[], ULONG TS[], ULONG nT);	
	Maillage(Point3D *S, ULONG nS, ULONG T[], ULONG TS[], ULONG nT);	
	Maillage(const char *nom_fichier, int format=Maillage::OFF_FILE);
	PtrSommet &ls() {return s_first;}
	PtrArete  &la() {return a_first;}
	PtrFace   &lf() {return f_first;}
	void marqueSommets(ULONG m);
	void marqueAretes(ULONG m);
	void marqueFaces(ULONG m);
	void numerotation_sommets(ULONG num_initial=0);
	void numerotation_aretes(ULONG num_initial=0);
	void numerotation_faces(ULONG num_initial=0);
	void numerotation_elements(ULONG num_initial=0);
	PtrSommet sommet(ULONG n);
	PtrArete arete(ULONG n);
	PtrFace face(ULONG n);
	PtrSommet sommet(Point3D C);
	ULONG nb_s() {return nb_sommet;}
	ULONG nb_a() {return nb_arete ;}
	ULONG nb_f() {return nb_face  ;}
	void add(PtrSommet snew);  
	void add(PtrArete  anew);   
	void add(PtrFace   fnew);
	void remove(PtrSommet s0);  
	void remove(PtrArete  a0);   
	void remove(PtrFace   f0);
	void trianguler();
	void quadranguler();
	bool n_angulaire(int n);
	bool triangulaire()  { return n_angulaire(3); }
	bool quadrangulaire(){ return n_angulaire(4); }
	bool n_regulier(int n);
	ULONG nb_s_externes();
	Point3D barycentre();
	void centrer();
	void translater(Point3D P);
	void homothetie(double a);

	void structure(Point3D* &S, ULONG &nS, ULONG* &T, ULONG* &TS, ULONG &nT);	
	void elevation3d(double h=1.0);
	void write(ostream & o);
	void write_bounding_box(ostream & o);
	friend ostream & operator<<(ostream & o, Maillage &m);	
	void printf(FILE *f = stdout);	
	
	// creation de triangulations de types particulier
	Maillage
	(ULONG dim_u, ULONG dim_v, ULONG type_grille, 
	 double x_uv(double u, double v), 
	 double y_uv(double u, double v), 
	 double z_uv(double u, double v), 
	 double u_min=0.0, double u_max=1.0, double v_min=0.0, double v_max=1.0);
	 ~Maillage();
	
	// sorties en différents formats
	void write_geomview(const char *nom_fichier = "triangulation.off");
//	void write_vrml(char nom_fichier[] = "triangulation.wrl");

};

// routines sur les listes
void write(lAretes la);
void write(lFaces  lf);

#endif
