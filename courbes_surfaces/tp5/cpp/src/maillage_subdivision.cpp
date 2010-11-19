/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/* Surcharge de maillage pour effectuer de la subdivision                    */ 
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/

#include "maillage_subdivision.hpp"
#include<cmath>

///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//
// constructeurs
//
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

MaillageSubdivision::MaillageSubdivision
(double S[][3], ULONG nS, ULONG T[], ULONG TS[], ULONG nT):	
 Maillage(S, nS, T, TS, nT)
{
}
	
MaillageSubdivision::MaillageSubdivision
(Point3D *S, ULONG nS, ULONG T[], ULONG TS[], ULONG nT):	
 Maillage(S, nS, T, TS, nT)
{
}
	
MaillageSubdivision::MaillageSubdivision
(char *nom_fichier, int format):
 Maillage(nom_fichier, format)
{
}

///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//
// méthodes de subdivision sur des maillages quelconques
//
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// étape de subdivision - méthode de Doo-Sabin
///////////////////////////////////////////////////////////////////////////////
typedef ULONG int4[4];

void MaillageSubdivision::subdivision_doo_sabin()
{
	////-----------------------------------------------------------------------
	//// calcul du nombre de sommets finaux 
	//// et du nombre de faces finales
	ULONG nb_ns; // nombre final de sommets 
	ULONG nb_nf; // nombre final de faces
	nb_ns = 2*nb_arete + nb_s_externes();
	nb_nf = nb_sommet + nb_arete + nb_face;

	////-----------------------------------------------------------------------
	//// création des nouveaux sommets
	//// pour chaque face, un sommet est créé pour chaque sommet de la face
	//// pour chaque arete externe, deux sommets supplémentaires sont créés
	//// pour chaque face avec n sommets, un tableau SF des indices des 
	////  nouveaux sommets est créé : 
	////  SF[0] SF[1] ... SF[n-1] donnera une nouvelle face avec n sommets
	//// pour chaque arete, les indices des quatre nouveaux sommets sont
	////  conservés dans le tableau SA :
	////   SA[iA][0] et SA[iA][1] : nouveaux sommets de la face1
	////   SA[iA][2] et SA[iA][3] : nouveaux sommets de la face2
	////   SA[iA][0] SA[iA][1] SA[iA][2] SA[iA][3] donnera une face
	////   quadrangulaire
	ULONG k;
	Point3D *Scoord;
	int4 *SA;
	try {
		Scoord = new Point3D[nb_ns];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_doo_sabin" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}
	try {
		SA = new int4[nb_arete];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_doo_sabin" 
		     << " - memoire insuffissante " << endl;
	}
	numerotation_elements(); // (re)numeroter les elements à partir de 0
	
	k=0;
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		int n = f1->nb_sommets(); // sommets de la face f1
		Point3D Sf1_coord[n];     // coordonnées de ces sommets
		
		// créer le tableau SF
		ULONG *SF;
		try {
			SF = new ULONG[n];
		}
		catch(const exception& e)
		{
			cerr << "MaillageSubdivision::subdivision_doo_sabin" 
			     << " - memoire insuffissante " << endl;
		}
		f1->marque((ULONG)SF);   // lier le tableau SF à la face f1
		lSommets lsf1=f1->sommets();
		lSommets::iterator ilsf1=lsf1.begin();
		for (int i=0; i<n; i++) 
		{
			SF[i] = k; k++;
			Sf1_coord[i] = (*ilsf1)->coord(); ilsf1++;
		}
		
		// calcul des coordonnées des n nouveaux sommets
		double alpha[n]; // coefficients pour les combinaisons
		alpha[0] = 0.25 + 1.25/n;
		for (int i=1; i<n; i++) 
		{
			alpha[i] = (3.0+2.0*cos(2.0*i*M_PI/n))/(4.0*n);
		}
// cout << "alpha : ";
// for (int i=0; i<n; i++) cout << alpha[i] << " ";
// cout << endl;

		for (int i=0; i<n; i++) 
		{
			// calcul des coordonnees du nouveau sommet d'indice SF[i]
			Point3D C;
			for (int j=0; j<n; j++) 
				C += alpha[j]*Sf1_coord[(j+i)%n];
			Scoord[SF[i]] = C;
		}
		
				
		// lier chaque couple de nouveaux sommets à l'arete correspondante
		lAretes laf1 = f1->aretes();
		lAretes::iterator ilaf1 = laf1.begin();
		for (int i=0; i<n; i++) 
		{
			PtrArete a1 = *ilaf1;
			int ia1 = a1->num;
			if (a1->face1()==f1)
			{
				SA[ia1][0] = SF[(i+1)%n];
				SA[ia1][1] = SF[i];
			}
			else
			{
				SA[ia1][2] = SF[(i+1)%n];
				SA[ia1][3] = SF[i];
			}
			ilaf1++;
		}		
	}
	
	// créer pour chaque arete externe, deux sommets supplémentaires
	for (PtrArete a1 = a_first; a1 != ARETE_NONE; a1 = a1->suiv)
	if (a1->estExterne)
	{
		int ia1 = a1->num;
		// coordonnées des deux nouveaux sommets - schéma de Chaikin
		Point3D S1a1 = a1->sommet1()->coord();
		Point3D S2a1 = a1->sommet2()->coord();
		Point3D Sn1 = (3.0*S1a1+S2a1)/4.0;
		Point3D Sn2 = (S1a1+3.0*S2a1)/4.0;
		if (a1->face1()==FACE_NONE)
		{
			Scoord[k]=Sn1; SA[ia1][1] = k; k++;
			Scoord[k]=Sn2; SA[ia1][0] = k; k++;
		}
		else
		{
			Scoord[k]=Sn1; SA[ia1][2] = k; k++;
			Scoord[k]=Sn2; SA[ia1][3] = k; k++;
		}
		
	}	

// cout << "SA : " << endl;
// for (int i=0; i<nb_arete; i++)
// {
// 	cout << " arete " << i << " : [";
// 	cout << SA[i][0] << "," << SA[i][1] << "," << SA[i][2] << "," << SA[i][3];
// 	cout << "]" << endl;
// }
// cout << "SF : " << endl;
// for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
// {
// 	ULONG* SF = (ULONG*)(f1->marque());
// 	cout << " face " << f1->num << " :" ;
// 	for (int i=0; i<f1->nb_sommets(); i++)
// 		cout << " " << SF[i];
// 	cout << endl;
// }
// cout << "Scoord : " << endl;
// for (k=0; k<nb_ns; k++)
// {
//  cout << k << " " << Scoord[k] << endl;
// }

	////-----------------------------------------------------------------------
	//// création des tableaux des nouvelles faces 
	
	// taille du tableau T :
	// somme_face degre(face)                         + 
	// 4*nb_arete                                     +
	// somme_sommet_interne degre(sommet_interne)     +
	// somme_sommet_externe [degre(sommet_interne)+1] =
	// 2*nb_arete_interne + nb_arete_externe          +
	// 4*nb_arete                                     +
	// 2*nb_arete + nb_sommet_externe                 =
	// 8*nb_arete
	int tailleT = 8*nb_arete;
	ULONG *T;
	ULONG *TS;
	ULONG kT = 0;
	ULONG kTS = 0;

	try {
		T = new ULONG[tailleT];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_doo_sabin" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}
	try {
		TS = new ULONG[nb_nf+1];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_doo_sabin" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}

	TS[0] = 0;
	
	// création des nouvelles faces correspondant aux faces
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		ULONG *SF = (ULONG *)(f1->marque()); 
		int n =  f1->nb_sommets(); // nb de sommets de la face
		for (int i=0; i<n; i++)
			T[kT++] = SF[i];
		TS[kTS+1] = TS[kTS]+n; kTS++;
	}
	
	// création des nouvelles faces quadrangulaires correspondant aux aretes
	for (int i=0; i<nb_arete; i++)
	{
		for (int j=0; j<4; j++)
			T[kT++] = SA[i][j];
		TS[kTS+1] = TS[kTS]+4; kTS++;
	}
	
	// création des nouvelles faces correspondant aux sommets
	for (PtrSommet s1=s_first; s1!=SOMMET_NONE; s1=s1->suiv)
	{
		lAretes las1 = s1->aretes(); // aretes incidentes à s1
		lAretes::iterator ilas1 = las1.begin();
		PtrArete a1;
		int n=0;
		if (s1->estExterne)
		{
			// un sommet supplémentaire pour la face correspondant
			// à un sommet s1 externe
			a1=*ilas1;
			T[kT++] = SA[a1->num][a1->sommet1()==s1 ? 2 : 0];
			n++;
		}
		// parcourir la liste d'aretes 
		while (ilas1 != las1.end())
		{
			a1=*ilas1;
			T[kT++] = SA[a1->num][a1->sommet1()==s1 ? 1 : 3];
			n++;
			ilas1++;
		}
		TS[kTS+1] = TS[kTS]+n; kTS++;		
	}
	
// cout << "TS : ";
// for (int i=0; i<nb_nf+1; i++)
//  cout << " " << TS[i];    
// cout << endl;
// cout << "T : ";
// for (int i=0; i<tailleT; i++)
//  cout << " " << T[i];	  
// cout << endl;
// for (int i=0; i<nb_ns; i++)
// {
//  cout << "Sommet " << i << " : " << Scoord[i] << endl;
// }
//  
// for (int i=0; i<nb_nf; i++)
// {
// cout << "Face " << i << " : [ " ;
// for (int j=TS[i]; j<TS[i+1]; j++) cout << T[j] << " ";
// cout << "]" << endl;
// }

	//// suppression des tableaux annexes
	// suppression des tableaux annexes SF pour chaque face
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		ULONG *SF = (ULONG*)(f1->marque());
		delete []SF;
	}

	//// modification de la structure ////
	
	// destruction des éléments existants
	supprimer_elements();
	
	// création des nouveaux éléments 
	creer(Scoord, nb_ns, T, TS, nb_nf);
	
	delete[] Scoord;
	delete[] SA;
	delete[] T;
	delete[] TS;
}

///////////////////////////////////////////////////////////////////////////////
// étape de subdivision - méthode de Catmull-Clark
///////////////////////////////////////////////////////////////////////////////
void MaillageSubdivision::subdivision_catmull_clark()
{

	////-----------------------------------------------------------------------
	//// calcul du nombre de sommets finaux 
	//// et du nombre de faces finales
	ULONG nb_ns; // nombre final de sommets 
	ULONG nb_nf; // nombre final de faces
	nb_ns = nb_sommet + nb_arete + nb_face;
	nb_nf = 2*nb_arete - nb_s_externes();
	
	////-----------------------------------------------------------------------
	//// création des nouveaux sommets
	//// chaque nouveau sommet d'indice k est lié à l'élément existant
	//// au moyen de sa marque
	ULONG k;
	
	// création d'un nouveau sommet pour chaque sommet 
	// indice de 0 à nb_sommet-1
	k = 0;
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
		s1->marque(k++); 
		
	// création d'un nouveau sommet pour chaque arete
	// indice de nb_sommet à nb_sommet+nb_arete-1
	k = nb_sommet;
	for (PtrArete a1 = a_first; a1 != ARETE_NONE; a1 = a1->suiv)
		a1->marque(k++); 
		
	// création d'un nouveau sommet pour chaque face
	// indice de nb_sommet+nb_arete à nb_sommet+nb_arete+nb_face-1
	k = nb_sommet+nb_arete;
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
		f1->marque(k++); 
		
	////-----------------------------------------------------------------------
	//// création du tableau des coordonnées des sommets 
	Point3D *Scoord;
	try {
		Scoord = new Point3D[nb_ns];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_catmull_clark" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}
	
	// coordonnées des nouveaux sommets correspondants aux faces existantes 	
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		k = f1->marque();
		Scoord[k] = f1->barycentre();
	}
	
	// coordonnées des nouveaux sommets correspondants aux aretes existantes 	
	for (PtrArete a1 = a_first; a1 != ARETE_NONE; a1 = a1->suiv)
	{
		Point3D C;
		PtrSommet s1=a1->sommet1();
		PtrSommet s2=a1->sommet2();
		Point3D C1 = s1->coord();
		Point3D C2 = s2->coord();
		if (a1->estExterne)
		{
			// arete externe
			C = (C1+C2)/2.0;
		}
		else
		{
			// arete interne
			PtrFace f1 = a1->face1();
			PtrFace f2 = a1->face2();
			Point3D Q1 = Scoord[f1->marque()];
			Point3D Q2 = Scoord[f2->marque()];
			C = (C1+C2+Q1+Q2)/4.0;
		}
		
		k = a1->marque(); 
		Scoord[k] = C;
	}
	
	// coordonnées des nouveaux sommets correspondants aux sommets existants 	
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	{
		Point3D C;
		if (s1->estExterne)
		{
			// sommet externe
			PtrSommet s2=s1->sommet_adjacent_initial();
			PtrSommet s3=s1->sommet_adjacent_final();
			Point3D C1 = s1->coord();
			Point3D C2 = s2->coord();
			Point3D C3 = s3->coord();
			C = (C2+6.0*C1+C3)/8.0;
		}
		else
		{
			// sommet interne
			lAretes las1 = s1->aretes();
			lFaces  laf1 = s1->faces();
			int n = las1.size();
			
			Point3D S = s1->coord();
			Point3D R(0.0,0.0,0.0);
			Point3D Q(0.0,0.0,0.0);
			
			// calcul de R 
			for (lAretes::iterator i = las1.begin(); i != las1.end(); i++)
			{
				R += (*i)->barycentre();
			}
			R /= n;
			
			// calcul de Q 
			for (lFaces::iterator i = laf1.begin(); i != laf1.end(); i++)
			{
				Q += (*i)->barycentre();
			}
			Q /= n;
			
			// calcul du nouveau sommet
//			C = (Q+2*R+(n-3)*S)/n; // maillage quelconque		   
			C = (Q+2*R+S)/4;       // maillage quadrangle régulier		   
		}
		k = s1->marque(); 
		Scoord[k] = C;
	}
	
// cout << "Scoord : " << endl;
// for (k=0; k<nb_ns; k++)
// {
//  cout << k << " " << Scoord[k] << endl;
// }

	
	////-----------------------------------------------------------------------
	//// création des tableaux des nouvelles faces (quadrangulaires)
	int tailleT = 4*nb_nf;
	ULONG *T;
	ULONG *TS;
	ULONG kT = 0;
	ULONG kTS = 0;

	try {
		T = new ULONG[tailleT];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_catmull_clark" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}
	try {
		TS = new ULONG[nb_nf+1];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_catmull_clark" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}

	TS[0] = 0;
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		lAretes laf1 = f1->aretes(); // aretes de la face f1
		// indice du nouveau sommet correspondant à f1
		ULONG isf1 = f1->marque(); 
		lAretes::iterator i1 = laf1.begin(); 
		lAretes::iterator i2=i1; i2++;
		while (i1 != laf1.end())
		{
			if (i2==laf1.end()) i2=laf1.begin();
			PtrArete a1 = *i1;
			PtrArete a2 = *i2;
			// indice du nouveau sommet correspondant à a1
			ULONG isa1 = a1->marque();
			// indice du nouveau sommet correspondant à a2
			ULONG isa2 = a2->marque();
			
			// sommet commun à a1 et a2
			PtrSommet s12;
			s12 = (a1->face1() == f1 ? a1->sommet2() : a1->sommet1());
			
			// indice du nouveau sommet correspondant à s12
			ULONG iss12 = s12->marque();
			
			// création d'une nouvelle face quadrangulaire
			TS[kTS+1] = TS[kTS]+4; kTS++;
			T[kT++] = isf1;
			T[kT++] = isa1;
			T[kT++] = iss12;
			T[kT++] = isa2;
			
			i1++; i2++;
		}
	}
	
// cout << "catmull_clark - avant creer : nb_nf = " << nb_nf << endl;
// for (int i=0; i<nb_nf; i++)
// {
// cout << "Face " << i << " : [ " ;
// for (int j=TS[i]; j<TS[i+1]; j++) cout << T[j] << " ";
// cout << "]" << endl;
// }
	
	//// modification de la structure ////
	
	// destruction des éléments existants
	supprimer_elements();
	
	// création des nouveaux éléments 
	creer(Scoord, nb_ns, T, TS, nb_nf);

	delete[] Scoord;
	delete[] T;
	delete[] TS;
}

///////////////////////////////////////////////////////////////////////////////
// subdivision Mid-Edge :
// - chaque arete donne un nouveau sommet
// - chaque face de degré n donne une face de degré n 
// - chaque sommet de degré n>=3 donne une face de degré n 
///////////////////////////////////////////////////////////////////////////////
void MaillageSubdivision::subdivision_midedge()
{
	////-----------------------------------------------------------------------
	//// calcul du nombre de sommets finaux 
	//// et du nombre de faces finales
	ULONG nb_ns; // nombre final de sommets 
	ULONG nb_nf; // nombre final de faces
	nb_ns = nb_arete;
	nb_nf = nb_face;
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	if (s1->nb_sommets() >= 3) nb_nf++;
	
	////-----------------------------------------------------------------------
	//// création des nouveaux sommets
	//// chaque nouveau sommet d'indice k est lié à l'élément existant
	//// au moyen de sa marque
	ULONG k;
	
	// création d'un nouveau sommet pour chaque arete
	// indice de 0 à nb_arete-1
	k = 0;
	for (PtrArete a1 = a_first; a1 != ARETE_NONE; a1 = a1->suiv)
		a1->marque(k++); 
		
	////-----------------------------------------------------------------------
	//// création du tableau des coordonnées des sommets 
	Point3D *Scoord;
	try {
		Scoord = new Point3D[nb_ns];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_midedge" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}
	
	// coordonnées des nouveaux sommets correspondants aux aretes existantes 	
	for (PtrArete a1 = a_first; a1 != ARETE_NONE; a1 = a1->suiv)
	{
		k = a1->marque();
		Scoord[k] = a1->barycentre();
	}
	
	////-----------------------------------------------------------------------
	//// création des tableaux des nouvelles faces 

	// calculer la somme des degrés des nouvelles faces
	ULONG tailleT = 0;
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
		tailleT += f1->nb_sommets();
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	if (s1->nb_sommets() >= 3) 
		tailleT += s1->nb_sommets();
	
	ULONG *T;
	ULONG *TS;
	ULONG kT = 0;
	ULONG kTS = 0;

	try {
		T = new ULONG[tailleT];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_midedge" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}
	try {
		TS = new ULONG[nb_nf+1];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_midedge" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}

	TS[0] = 0;
	
	// pour chaque face existante, créer une nouvelle face
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		lAretes laf1 = f1->aretes();
		
		// créer la face dont les sommets sont les milieux des aretes
		for (lAretes::iterator i1 = laf1.begin(); i1!=laf1.end(); i1++)
		{
			PtrArete a1 = *i1;
			T[kT] = (*i1)->marque(); kT++;
		}
		TS[kTS+1] = TS[kTS]+f1->nb_aretes(); kTS++;
	}
	
	// pour chaque sommet de degré >= 3, créer une nouvelle face 
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	if (s1->nb_sommets() >= 3) 
	{
		lAretes las1 = s1->aretes();
		
		// créer la face dont les sommets sont les milieux des aretes
		for (lAretes::iterator i1 = las1.begin(); i1!=las1.end(); i1++)
		{
			PtrArete a1 = *i1;
			T[kT] = (*i1)->marque(); kT++;
		}
		TS[kTS+1] = TS[kTS]+s1->nb_aretes(); kTS++;
	}
	
// cout << "TS : ";
// for (int i=0; i<nb_nf+1; i++)
// 	cout << " " << TS[i];	 
// cout << endl;
// cout << "T : ";
// for (int i=0; i<4*(nb_nf-nb_face); i++)
// 	cout << " " << T[i];	 
// cout << endl;
// for (int i=0; i<nb_ns; i++)
// {
// 	cout << "Sommet " << i << " : " << Scoord[i] << endl;
// }
// 	
// for (int i=0; i<nb_nf; i++)
// {
// cout << "Face " << i << " : [ " ;
// for (int j=TS[i]; j<TS[i+1]; j++) cout << T[j] << " ";
// cout << "]" << endl;
// }

	////-----------------------------------------------------------------------
	//// modification de la structure 
	
	// destruction des éléments existants
	supprimer_elements();
	
	// création des nouveaux éléments 
	creer(Scoord, nb_ns, T, TS, nb_nf);
		
	delete[] Scoord;
	delete[] T;
	delete[] TS;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// routines spécifiques pour des maillages triangulaires
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// récuperer les 3 aretes de la face triangulaire f
///////////////////////////////////////////////////////////////////////////////
void aretes_triangle(PtrFace f, PtrArete &a1, PtrArete &a2, PtrArete &a3)
{
	lAretes laf = f->aretes();
	lAretes::iterator i = laf.begin();
	
	a1 = *i; i++;
	a2 = *i; i++;
	a3 = *i; 
}

///////////////////////////////////////////////////////////////////////////////
// récuperer les 3 sommets de la face triangulaire f
///////////////////////////////////////////////////////////////////////////////
void sommets_triangle(PtrFace f, PtrSommet &s1, PtrSommet &s2, PtrSommet &s3)
{
	lSommets lsf = f->sommets();
	lSommets::iterator i = lsf.begin();
	
	s1 = *i; i++;
	s2 = *i; i++;
	s3 = *i; 
}

///////////////////////////////////////////////////////////////////////////////
// pour l'arete a1, trouver le sommet oppose par rapport à la face nf (1 ou 2)
///////////////////////////////////////////////////////////////////////////////
PtrSommet sommet_oppose(PtrArete a1, int nf)
{
	PtrSommet s_opp = SOMMET_NONE;
	if (nf==1 || nf==2)
	{
		PtrFace f=(nf==1 ? a1->face1() : a1->face2());
		if (f != FACE_NONE)
		{
			PtrArete a2 = f->arete_suivante_trigo(a1);
			s_opp = (a2->face1()==f ? a2->sommet2() : a2->sommet1());
		}
	}
	return s_opp;
}

///////////////////////////////////////////////////////////////////////////////
// pour l'arete a1, trouver le sommet oppose par rapport à la face f1
///////////////////////////////////////////////////////////////////////////////
PtrSommet sommet_oppose(PtrArete a1, PtrFace f1)
{
	PtrSommet s1, s2, s3;
	sommets_triangle(f1, s1, s2, s3);
	if (s1 != a1->sommet1() && s1 != a1->sommet2())
		return s1;
	if (s2 != a1->sommet1() && s2 != a1->sommet2())
		return s2;
	return s3;
}

///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//
// méthodes de subdivision sur des maillages triangulaires
//
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// subdivision de type sqrt-3 
// (subdivision simple face-aretes externes + 
//  flipping des aretes internes initiales)
// le maillage initial doit etre triangulaire
///////////////////////////////////////////////////////////////////////////////
void MaillageSubdivision::subdivision_sqrt3()
{
	////-----------------------------------------------------------------------
	// tester la triangularité
	if (!n_angulaire(3))
	{
		cerr << "MaillageSubdivision::subdivision_sqrt3() :";
		cerr << " non triangulaire!!"
		     << endl;
		return;
	}
	
	////-----------------------------------------------------------------------
	//// calcul du nombre de sommets finaux 
	//// et du nombre de faces finales
	ULONG nb_ns; // nombre final de sommets 
	ULONG nb_nf; // nombre final de faces
	nb_ns = nb_sommet + nb_face + nb_s_externes();
	nb_nf = 2*nb_arete; 
	
	////-----------------------------------------------------------------------
	//// création des nouveaux sommets
	//// chaque nouveau sommet d'indice k est lié à l'élément existant
	//// au moyen de sa marque
	ULONG k;
	
	// création d'un nouveau sommet pour chaque sommet 
	// indice de 0 à nb_sommet-1
	k = 0;
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
		s1->marque(k++); 
		
	// création d'un nouveau sommet pour chaque face
	// indice de nb_sommet à nb_sommet+nb_face-1
	k = nb_sommet;
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
		f1->marque(k++); 
		
	// création d'un nouveau sommet pour chaque arete externe 
	// indice de nb_sommet+nb_face à nb_sommet+nb_face+nb_s_externes()-1
	k = nb_sommet+nb_face;
	for (PtrArete a1 = a_first; a1 != ARETE_NONE; a1 = a1->suiv)
	if (a1->estExterne)
		a1->marque(k++); 
		
	////-----------------------------------------------------------------------
	//// création du tableau des coordonnées des sommets 
	Point3D *Scoord;
	try {
		Scoord = new Point3D[nb_ns];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_sqrt3" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}
		
	// coordonnées des nouveaux sommets correspondants aux sommets existants 	
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	{
		k = s1->marque();
		if (s1->estExterne)
		{
			// schéma Catmull-Clark
			PtrSommet s11 = s1->sommet_adjacent_initial();
			PtrSommet s12 = s1->sommet_adjacent_final();
			Scoord[k] = (6*s1->coord()+s11->coord()+s12->coord())/8;
		}
		else
		{
			lSommets lss1 = s1->sommets();
			int n=lss1.size();
			double alpha = (4.0-2.0*cos(2.0*M_PI/n))/9.0;
			Point3D C = (1.0-alpha)*s1->coord();
			for (lSommets::iterator i=lss1.begin(); i!=lss1.end(); i++)
				C += alpha/n * ((*i)->coord());
			Scoord[k] = C;
		}
	}
	
	// coordonnées des nouveaux sommets correspondants aux milieux des faces 	
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		k = f1->marque();
		Scoord[k] = f1->barycentre();
	}
	
	for (PtrArete a1 = a_first; a1 != ARETE_NONE; a1 = a1->suiv)
	if (a1->estExterne)
	{
		k = a1->marque();
		Scoord[k] = a1->barycentre();
	}

	////-----------------------------------------------------------------------
	//// création des tableaux des nouvelles faces (toutes triangulaires)
	int tailleT = 3*nb_nf;
	ULONG *T;
	ULONG *TS;
	ULONG kT = 0;
	ULONG kTS = 0;

	try {
		T = new ULONG[tailleT];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_sqrt3" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}
	try {
		TS = new ULONG[nb_nf+1];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_sqrt3" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}

	TS[0] = 0;
	
	// pour chaque arete, créer deux nouvelles faces 
	for (PtrArete a1 = a_first; a1 != ARETE_NONE; a1 = a1->suiv)
	{
		PtrSommet s1 = a1->sommet1(); ULONG is1 = s1->marque();
		PtrSommet s2 = a1->sommet2(); ULONG is2 = s2->marque();
		PtrFace f1 = a1->face1();
		PtrFace f2 = a1->face2();
		if (a1->estExterne)
		{
			ULONG ia1 = a1->marque();
			if (f1 != FACE_NONE)
			{
				// face f1 existante : créer deux nouvelles faces
				// [is1,ia1,if1] et [ia1,is2,if1]			
				ULONG if1 = f1->marque();				

				// création d'une nouvelle face triangulaire
				T[kT++] = is1;
				T[kT++] = ia1;
				T[kT++] = if1;
				TS[kTS+1] = TS[kTS]+3; kTS++;

				// création d'une nouvelle face triangulaire
				T[kT++] = ia1;
				T[kT++] = is2;
				T[kT++] = if1;
				TS[kTS+1] = TS[kTS]+3; kTS++;
			}
			else
			{
				// face f2 existante : créer deux nouvelles faces
				// [ia1,is1,if2] et [is2,ia1,if2]			
				ULONG if2 = f2->marque();				

				// création d'une nouvelle face triangulaire
				T[kT++] = ia1;
				T[kT++] = is1;
				T[kT++] = if2;
				TS[kTS+1] = TS[kTS]+3; kTS++;

				// création d'une nouvelle face triangulaire
				T[kT++] = is2;
				T[kT++] = ia1;
				T[kT++] = if2;
				TS[kTS+1] = TS[kTS]+3; kTS++;
			}
		} // fin cas a1->estExterne
		else
		{
			// créer deux faces [is1,if2,if1] et [is2,if1,if2]
			ULONG if1 = f1->marque();				
			ULONG if2 = f2->marque();	
						
			// création d'une nouvelle face triangulaire
			T[kT++] = is1;
			T[kT++] = if2;
			T[kT++] = if1;
			TS[kTS+1] = TS[kTS]+3; kTS++;

			// création d'une nouvelle face triangulaire
			T[kT++] = is2;
			T[kT++] = if1;
			T[kT++] = if2;
			TS[kTS+1] = TS[kTS]+3; kTS++;
		}	
	}
	 
// cout << "TS : ";
// for (int i=0; i<nb_nf+1; i++)
//  cout << " " << TS[i];    
// cout << endl;
// cout << "T : ";
// for (int i=0; i<4*(nb_nf-nb_face); i++)
//  cout << " " << T[i];	  
// cout << endl;
// for (int i=0; i<nb_ns; i++)
// {
//  cout << "Sommet " << i << " : " << Scoord[i] << endl;
// }
//  
// for (int i=0; i<nb_nf; i++)
// {
// cout << "Face " << i << " : [ " ;
// for (int j=TS[i]; j<TS[i+1]; j++) cout << T[j] << " ";
// cout << "]" << endl;
// }

	////-----------------------------------------------------------------------
	//// modification de la structure 
	
	// destruction des éléments existants
	supprimer_elements();
	
	// création des nouveaux éléments 
	creer(Scoord, nb_ns, T, TS, nb_nf);
		
	delete[] Scoord;
	delete[] T;
	delete[] TS;
}

///////////////////////////////////////////////////////////////////////////////
// subdivision de Loop
///////////////////////////////////////////////////////////////////////////////
void MaillageSubdivision::subdivision_loop()
{
	////-----------------------------------------------------------------------
	// tester la triangularité
	if (!n_angulaire(3))
	{
		cerr << "MaillageSubdivision::subdivision_loop() :";
		cerr << " non triangulaire!!"
		     << endl;
		return;
	}
	
	ULONG nb_ns; // nombre final de sommets 
	ULONG nb_na; // nombre final d'aretes 
	ULONG nb_nt; // nombre final de faces

	nb_ns = nb_sommet + nb_arete;
	nb_na = 2*nb_sommet+3*nb_face;
	nb_nt = 4*nb_face;
	
	////-----------------------------------------------------------------------
	//// création des nouveaux sommets
	//// chaque nouveau sommet d'indice k est lié à l'élément existant
	//// au moyen de sa marque
	ULONG k;
	
	// création d'un nouveau sommet pour chaque sommet 
	// indice de 0 à nb_sommet-1
	k = 0;
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
		s1->marque(k++); 
		
	// création d'un nouveau sommet pour chaque arete
	// indice de nb_sommet à nb_sommet+nb_arete-1
	k = nb_sommet;
	for (PtrArete a1 = a_first; a1 != ARETE_NONE; a1 = a1->suiv)
		a1->marque(k++); 
		
	////-----------------------------------------------------------------------
	//// création du tableau des coordonnées des sommets 
	Point3D *Scoord;
	try {
		Scoord = new Point3D[nb_ns];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_loop" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}
	
	// coordonnées des nouveaux sommets correspondants aux sommets existants 	
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	{
		lAretes las1 = s1->aretes();
		Point3D C=s1->coord();
		if (s1->estExterne)
		{
			Point3D C1 = s1->sommet_adjacent_initial()->coord();
			Point3D C2 = s1->sommet_adjacent_final()->coord();
			C = (C1 + 6*C + C2)/8.0;
		}
		else
		{
			ULONG nsi = las1.size(); // nb sommets incidents
			double beta = 0.375+0.25*cos(2*M_PI/nsi);
			beta = (0.625-beta*beta)/nsi;
			double alpha = 1.0-nsi*beta;
			
			C *= alpha;
			
			for (lAretes::iterator i=las1.begin(); i!=las1.end(); i++)
			{
				PtrArete a1 = *i;
				Point3D C1 = s1->sommet_adjacent(a1)->coord();
				C += C1*beta;
			}
		}
		k = s1->marque();
		Scoord[k] = C;
	}
	
	// coordonnées des nouveaux sommets correspondants aux aretes existantes 	
	for (PtrArete a1 = a_first; a1 != ARETE_NONE; a1 = a1->suiv)
	{
		Point3D C;
		C = a1->barycentre();
		if (!a1->estExterne)
		{
			Point3D C1 = sommet_oppose(a1, 1)->coord();
			Point3D C2 = sommet_oppose(a1, 2)->coord();
			C = (6.0*C+C1+C2)*0.125;
		}
		k = a1->marque();
		Scoord[k] = C;
	}
	
	////-----------------------------------------------------------------------
	//// création des tableaux des nouvelles faces 
	ULONG tailleT = 3*nb_nt;
	ULONG *T;
	ULONG *TS;
	ULONG kT = 0;
	ULONG kTS = 0;

	try {
		T = new ULONG[tailleT];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_loop" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}
	try {
		TS = new ULONG[nb_nt+1];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_loop" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}

	TS[0] = 0;
	
	// pour chaque face existante, créer une nouvelle face
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		PtrArete a1,a2,a3;
		aretes_triangle(f1, a1, a2, a3);
		T[kT] = a1->marque(); kT++;
		T[kT] = a2->marque(); kT++;
		T[kT] = a3->marque(); kT++;
		TS[kTS+1] = TS[kTS]+3; kTS++;
	}
	
	// pour chaque sommet, créer une nouvelle face pour chaque face incidente
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	{
		ULONG is1 = s1->marque(); 
		lAretes las1 = s1->aretes();
		lAretes::iterator i1 = las1.begin(); 
		lAretes::iterator i2 = i1; i2++;
		while (i2!=las1.end())
		{
			T[kT] = (*i1)->marque(); kT++;
			T[kT] = (*i2)->marque(); kT++;
			T[kT] = is1;             kT++;
			TS[kTS+1] = TS[kTS]+3; kTS++;			
			
			i1++;
			i2++;
		} 
		if (!s1->estExterne)
		{
			i2 = las1.begin();
			T[kT] = (*i1)->marque(); kT++;
			T[kT] = (*i2)->marque(); kT++;
			T[kT] = is1;             kT++;
			TS[kTS+1] = TS[kTS]+3; kTS++;			
		}
	}
	 
// for (int i=0; i<nb_ns; i++)
// {
// cout << "Sommet " << i << " : " << Scoord[i] << endl;
// }
// 	
// for (int i=0; i<nb_nt; i++)
// {
// cout << "Face " << i << " : [ " ;
// for (int j=TS[i]; j<TS[i+1]; j++) cout << T[j] << " ";
// cout << "]" << endl;
// }

	////-----------------------------------------------------------------------
	//// modification de la structure 
	
	// destruction des éléments existants
	supprimer_elements();
	
	// création des nouveaux éléments 
	creer(Scoord, nb_ns, T, TS, nb_nt);
		
	delete[] Scoord;
	delete[] T;
	delete[] TS;
}

///////////////////////////////////////////////////////////////////////////////
// subdivision Butterfly
///////////////////////////////////////////////////////////////////////////////
void MaillageSubdivision::subdivision_butterfly()
{
	////-----------------------------------------------------------------------
	// tester la triangularité
	if (!n_angulaire(3))
	{
		cerr << "MaillageSubdivision::subdivision_butterfly() :";
		cerr << " non triangulaire!!"
		     << endl;
		return;
	}
	
	ULONG nb_ns; // nombre final de sommets 
	ULONG nb_na; // nombre final d'aretes 
	ULONG nb_nt; // nombre final de faces

	nb_ns = nb_sommet + nb_arete;
	nb_na = 2*nb_sommet+3*nb_face;
	nb_nt = 4*nb_face;
	
	////-----------------------------------------------------------------------
	//// création des nouveaux sommets
	//// chaque nouveau sommet d'indice k est lié à l'élément existant
	//// au moyen de sa marque
	ULONG k;
	
	// création d'un nouveau sommet pour chaque sommet 
	// indice de 0 à nb_sommet-1
	k = 0;
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
		s1->marque(k++); 
		
	// création d'un nouveau sommet pour chaque arete
	// indice de nb_sommet à nb_sommet+nb_arete-1
	k = nb_sommet;
	for (PtrArete a1 = a_first; a1 != ARETE_NONE; a1 = a1->suiv)
		a1->marque(k++); 
		
	////-----------------------------------------------------------------------
	//// création du tableau des coordonnées des sommets 
	Point3D *Scoord;
	try {
		Scoord = new Point3D[nb_ns];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_butterfly" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}
	
	// coordonnées des nouveaux sommets correspondants aux sommets existants 	
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	{
		k = s1->marque();
		Scoord[k] = s1->coord();
	}
	
	// coordonnées des nouveaux sommets correspondants aux aretes existantes 	
	for (PtrArete a1 = a_first; a1 != ARETE_NONE; a1 = a1->suiv)
	{
		Point3D C;
		if (!a1->estExterne)
		{
			PtrFace f1 = a1->face1();
			PtrFace f2 = a1->face2();
			PtrArete a11 = f1->arete_suivante_trigo_inverse(a1);
			PtrArete a12 = f1->arete_suivante_trigo(a1);
			PtrArete a21 = f2->arete_suivante_trigo_inverse(a1);
			PtrArete a22 = f2->arete_suivante_trigo(a1);
			PtrFace f11 = a11->face1()==f1 ? a11->face2() : a11->face1();
			PtrFace f12 = a12->face1()==f1 ? a12->face2() : a12->face1();
			PtrFace f21 = a21->face1()==f2 ? a21->face2() : a21->face1();
			PtrFace f22 = a22->face1()==f2 ? a22->face2() : a22->face1();
			PtrSommet s1 = sommet_oppose(a1,f1);
			PtrSommet s2 = sommet_oppose(a1,f2);
			PtrSommet s11;
			PtrSommet s12;
			PtrSommet s21;
			PtrSommet s22;
			s11 = f11 ? sommet_oppose(a11,f11) : SOMMET_NONE;
			s12 = f12 ? sommet_oppose(a12,f12) : SOMMET_NONE;
			s21 = f21 ? sommet_oppose(a21,f21) : SOMMET_NONE;
			s22 = f22 ? sommet_oppose(a22,f22) : SOMMET_NONE;
			Point3D C1 = s1->coord();
			Point3D C2 = s2->coord();
			Point3D C11 = s11 ? s11->coord() : a11->barycentre();
			Point3D C12 = s12 ? s12->coord() : a12->barycentre();
			Point3D C21 = s21 ? s21->coord() : a21->barycentre();
			Point3D C22 = s22 ? s22->coord() : a22->barycentre();
			C = a1->barycentre()+(2.0*(C1+C2)-(C11+C12+C21+C22))/16.0;
		}
		else
		{
			PtrSommet s1 = a1->sommet1();
			PtrSommet s2 = a1->sommet2();
			PtrArete a11 = s1->arete_initiale()==a1 ?
			               s1->arete_finale()       :
			               s1->arete_initiale()     ;
			PtrArete a12 = s2->arete_initiale()==a1 ?
			               s2->arete_finale()       :
			               s2->arete_initiale()     ;
			C = (10*a1->barycentre()-a11->barycentre()-a12->barycentre())/8;
		}
		k = a1->marque();
		Scoord[k] = C;
	}
	
	////-----------------------------------------------------------------------
	//// création des tableaux des nouvelles faces 
	ULONG tailleT = 3*nb_nt;
	ULONG *T;
	ULONG *TS;
	ULONG kT = 0;
	ULONG kTS = 0;

	try {
		T = new ULONG[tailleT];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_loop" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}
	try {
		TS = new ULONG[nb_nt+1];
	}
	catch(const exception& e)
	{
		cerr << "MaillageSubdivision::subdivision_loop" 
		     << " - memoire insuffissante " << endl;
		exit(1);
	}

	TS[0] = 0;
	
	// pour chaque face existante, créer une nouvelle face
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		PtrArete a1,a2,a3;
		aretes_triangle(f1, a1, a2, a3);
		T[kT] = a1->marque(); kT++;
		T[kT] = a2->marque(); kT++;
		T[kT] = a3->marque(); kT++;
		TS[kTS+1] = TS[kTS]+3; kTS++;
	}
	
	// pour chaque sommet, créer une nouvelle face pour chaque face incidente
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	{
		ULONG is1 = s1->marque(); 
		lAretes las1 = s1->aretes();
		lAretes::iterator i1 = las1.begin(); 
		lAretes::iterator i2 = i1; i2++;
		while (i2!=las1.end())
		{
			T[kT] = (*i1)->marque(); kT++;
			T[kT] = (*i2)->marque(); kT++;
			T[kT] = is1;             kT++;
			TS[kTS+1] = TS[kTS]+3; kTS++;			
			
			i1++;
			i2++;
		} 
		if (!s1->estExterne)
		{
			i2 = las1.begin();
			T[kT] = (*i1)->marque(); kT++;
			T[kT] = (*i2)->marque(); kT++;
			T[kT] = is1;             kT++;
			TS[kTS+1] = TS[kTS]+3; kTS++;			
		}
	}
	 
// for (int i=0; i<nb_ns; i++)
// {
// cout << "Sommet " << i << " : " << Scoord[i] << endl;
// }
// 	
// for (int i=0; i<nb_nt; i++)
// {
// cout << "Face " << i << " : [ " ;
// for (int j=TS[i]; j<TS[i+1]; j++) cout << T[j] << " ";
// cout << "]" << endl;
// }

	////-----------------------------------------------------------------------
	//// modification de la structure 
	
	// destruction des éléments existants
	supprimer_elements();
	
	// création des nouveaux éléments 
	creer(Scoord, nb_ns, T, TS, nb_nt);
		
	delete[] Scoord;
	delete[] T;
	delete[] TS;
}


