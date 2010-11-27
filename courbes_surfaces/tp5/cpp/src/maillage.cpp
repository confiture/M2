/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/* Maillage construit à partir d'un ensemble de points 3D                    */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/

#include"maillage.hpp"
#include<cstdlib>
#include<string>
#include<fstream>
#include<iostream>

///////////////////////////////////////////////////////////////////////////////
// détermine la liste des faces "incidentes" au sommet dans le sens trigo
///////////////////////////////////////////////////////////////////////////////
lFaces Sommet::faces()
{
	lFaces lf;
	for (lAretes::iterator i=la.begin(); i != la.end(); i++)
	if (i!=la.begin() || !estExterne)
	{
		PtrArete a = *i;
		lf.push_back(a->sommet1()==this ? a->face2() : a->face1());
	}
	return lf;
}

///////////////////////////////////////////////////////////////////////////////
// détermine la liste des sommets adjacents au sommet dans le sens trigo
///////////////////////////////////////////////////////////////////////////////
lSommets Sommet::sommets()
{
	lSommets ls;
	for (lAretes::iterator i=la.begin(); i != la.end(); i++)
	{
		PtrArete a = *i;
		ls.push_back(a->sommet1()==this ? a->sommet2() : a->sommet1());
	}
	return ls;
}


///////////////////////////////////////////////////////////////////////////////
// détermine le sommet adjacent correspondant à l'arete a
// renvoie SOMMET_NONE si l'arete n'est pas incidente
///////////////////////////////////////////////////////////////////////////////
PtrSommet Sommet::sommet_adjacent(PtrArete a )
{
	if (!incident(a)) return SOMMET_NONE;
	return a->sommet1()==this ? a->sommet2() : a->sommet1();
}

///////////////////////////////////////////////////////////////////////////////
// détermine l'arete suivant l'arete a dans le sens trigo autour du sommet
///////////////////////////////////////////////////////////////////////////////
PtrArete Sommet::arete_suivante_trigo(PtrArete a)
{
	if (a == ARETE_NONE) return ARETE_NONE;
	if (a == ARETE_BAD)  return ARETE_BAD;
	if (a == arete_finale() && !estExterne) return arete_initiale();
	
	// recherche de a dans la liste d'aretes
	lAretes::iterator i1 = la.begin();
	lAretes::iterator i2 = i1; i2++;
	
	while (i2 != la.end())
	{
		if (*i1 == a) return *i2;
		i1++; i2++;
	}
	return ARETE_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// détermine l'arete suivant l'arete a dans le sens trigo inv. autour du sommet
///////////////////////////////////////////////////////////////////////////////
PtrArete Sommet::arete_suivante_trigo_inverse(PtrArete a)
{
	if (a == ARETE_NONE) return ARETE_NONE;
	if (a == ARETE_BAD)  return ARETE_BAD;
	if (a == arete_initiale() && !estExterne) return arete_finale();
	
	// recherche de a dans la liste d'aretes
	lAretes::reverse_iterator i1 = la.rbegin();
	lAretes::reverse_iterator i2 = i1; i2++;
	
	while (i2 != la.rend())
	{
		if (*i1 == a) return *i2;
		i1++; i2++;
	}
	return ARETE_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// détermine l'arete suivant le face t dans le sens trigo autour du sommet
///////////////////////////////////////////////////////////////////////////////
PtrArete Sommet::arete_suivante_trigo(PtrFace t)
{
	if (t == FACE_NONE) return ARETE_NONE;
	if (t == FACE_BAD)  return ARETE_BAD;
	
	// recherche de l'arete dont 
	// - soit le sommet 1 est le sommet et le face 1 est t
	// - soit le sommet 2 est le sommet et le face 2 est t
	lAretes::iterator i1 = la.begin();
	while (i1 != la.end())
	{
		PtrArete a = *i1;
		if (a->sommet1()==this && a->face1()==t) return a;
		if (a->sommet2()==this && a->face2()==t) return a;
		i1++;
	}
	return ARETE_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// détermine l'arete suivant le face t dans le sens t. i. autour du sommet
///////////////////////////////////////////////////////////////////////////////
PtrArete Sommet::arete_suivante_trigo_inverse(PtrFace t)
{
	if (t == FACE_NONE) return ARETE_NONE;
	if (t == FACE_BAD)  return ARETE_BAD;
	
	// recherche de l'arete dont 
	// - soit le sommet 1 est le sommet et le face 2 est t
	// - soit le sommet 2 est le sommet et le face 1 est t
	lAretes::iterator i1 = la.begin();
	while (i1 != la.end())
	{
		PtrArete a = *i1;
		if (a->sommet1()==this && a->face2()==t) return a;
		if (a->sommet2()==this && a->face1()==t) return a;
		i1++;
	}
	return ARETE_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// détermine le face suivant l'arete a dans le sens trigo autour du sommet
///////////////////////////////////////////////////////////////////////////////
PtrFace Sommet::face_suivant_trigo(PtrArete a)
{
	if (a == ARETE_NONE) return FACE_NONE;
	if (a == ARETE_BAD)  return FACE_BAD;

	return (a->sommet1()==this) ? a->face1() : a->face2();
}

///////////////////////////////////////////////////////////////////////////////
// détermine le face suivant l'arete a dans le sens t. i. autour du sommet
///////////////////////////////////////////////////////////////////////////////
PtrFace Sommet::face_suivant_trigo_inverse(PtrArete a)
{
	if (a == ARETE_NONE) return FACE_NONE;
	if (a == ARETE_BAD)  return FACE_BAD;

	return (a->sommet1()==this) ? a->face2() : a->face1();
}

///////////////////////////////////////////////////////////////////////////////
// détermine le face suivant le face t dans le sens t. autour du sommet
///////////////////////////////////////////////////////////////////////////////
PtrFace Sommet::face_suivant_trigo(PtrFace t)
{
	if (t == FACE_NONE) return FACE_NONE;
	if (t == FACE_BAD)  return FACE_BAD;
	
	// recherche de l'arete dont 
	// - soit le sommet 1 est le sommet et le face 1 est t
	//     on renvoie alors le face 2
	// - soit le sommet 2 est le sommet et le face 2 est t
	//     on renvoie alors le face 1
	lAretes::iterator i1 = la.begin();
	while (i1 != la.end())
	{
		PtrArete a = *i1;
		if (a->sommet1()==this && a->face1()==t) 
			return a->face2();
		if (a->sommet2()==this && a->face2()==t)  
			return a->face1();
		i1++;
	}
	return FACE_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// détermine le face suivant le face t dans le sens t. autour du sommet
///////////////////////////////////////////////////////////////////////////////
PtrFace Sommet::face_suivant_trigo_inverse(PtrFace t)
{
	if (t == FACE_NONE) return FACE_NONE;
	if (t == FACE_BAD)  return FACE_BAD;
	
	// recherche de l'arete dont 
	// - soit le sommet 1 est le sommet et le face 2 est t
	//     on renvoie alors le face 1
	// - soit le sommet 2 est le sommet et le face 1 est t
	//     on renvoie alors le face 2
	lAretes::iterator i1 = la.begin();
	while (i1 != la.end())
	{
		PtrArete a = *i1;
		if (a->sommet1()==this && a->face2()==t) 
			return a->face1();
		if (a->sommet2()==this && a->face1()==t)  
			return a->face2();
		i1++;
	}
	return FACE_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// détermine si le sommet est interne
///////////////////////////////////////////////////////////////////////////////
bool Sommet::interne()
{
	PtrArete a0=la.front();
	return a0->interne();	
}

///////////////////////////////////////////////////////////////////////////////
// détermine si l'arete a0 est incidente au sommet
///////////////////////////////////////////////////////////////////////////////
bool Sommet::incident(PtrArete a0)
{
	if (a0 == ARETE_BAD || a0 == ARETE_NONE) return false;
	return a0->sommet1()==this || a0->sommet2()==this;
}

///////////////////////////////////////////////////////////////////////////////
// détermine si l'arete t est incident au sommet
///////////////////////////////////////////////////////////////////////////////
bool Sommet::incident(PtrFace t)
{
	lAretes::iterator i1 = la.begin();
	while (i1 != la.end())
	{
		PtrArete a = *i1;
		if (a->face1()==t || a->face2()==t) return true;
		i1++;
	}
	return false;
}

// ///////////////////////////////////////////////////////////////////////////////
// // détermine la liste des aretes incidentes au sommet dans le sens trigo
// ///////////////////////////////////////////////////////////////////////////////
// list<PtrArete> Sommet::l_aretes_trigo()
// {
// 	PtrArete a1 = a;
// 	list<PtrArete> l;
// 	
// 	if (a1 == ARETE_NONE || a1 == ARETE_BAD) return l; // sommet isolé
// 	
// 	// parcours des aretes dans le sens trigo
// 	do
// 	{
// 		l.push_back(a1);
// 		a1 = arete_suivante_trigo(a1);
// 		if (a1 == ARETE_BAD) { l.clear(); return l; }
// 	} while (a1 != a && a1 != ARETE_NONE);
// 	if (a1 == a) return l; // cas sommet interne
// 	
// 	// parcours des aretes dans le sens inverse trigo (cas sommet externe)
// 	a1 = arete_suivante_trigo_inverse(a);
// 	while (a1 != ARETE_NONE)
// 	{
// 		l.push_front(a1);
// 		a1 = arete_suivante_trigo_inverse(a1);
// 		if (a1 == ARETE_BAD) { l.clear(); return l; }
// 	} 
// 	return l; 
// }
// 
// list<PtrArete> Sommet::l_aretes_trigo_inverse()
// {
// 	list<PtrArete> l = l_aretes_trigo();
// 	l.reverse();
// 	return l;
// }
// 
list<PtrFace> Sommet::l_faces_trigo()
{
	list<PtrFace> l;
	lAretes::iterator i1 = la.begin(), i2=i1++;
	while (i2 != la.end())
	{
		PtrArete a = *i1;
		if (a->sommet1()==this) 
			l.push_back(a->face1());
		else
			l.push_back(a->face2());
		i1++;
		i2++;
	}
	if (!estExterne)
	{
		PtrArete a = *i1;
		if (a->sommet1()==this) 
			l.push_back(a->face1());
		else
			l.push_back(a->face2());
	}
	return l;
}

list<PtrFace> Sommet::l_faces_trigo_inverse()
{
	list<PtrFace> l = l_faces_trigo();
	l.reverse();
	return l;
}
	
///////////////////////////////////////////////////////////////////////////////
//
// methodes amies pour l'écriture
//
///////////////////////////////////////////////////////////////////////////////

ostream & operator<<(ostream & o, Sommet &s)
{
o << dec;
o << "S[" << s.num << "]";
o << "-" << s.coord();
o << "-a[";
lAretes la = s.aretes();
for (lAretes::iterator i=la.begin(); i != la.end(); i++)
	o << (*i)->num << " ";
o << "]";
	
return o;
}

//////////////////////////////////////////////////////////////////////////////
// ecriture de l'objet dans le flux o
//////////////////////////////////////////////////////////////////////////////
void Sommet::write(ostream & o)
{
o << dec;
o << "S[" << num << "]";
o << (estExterne ? "E" : "I");
o << "-" << c;
o << "-a[";
for (lAretes::iterator i=la.begin(); i != la.end(); i++)
	o << (*i)->num << " ";
o << "]";
}

//////////////////////////////////////////////////////////////////////////////
// ecriture de l'objet dans le fichier f
//////////////////////////////////////////////////////////////////////////////
void Sommet::printf(FILE *f)
{
fprintf(f, "S[%2d]", num);
fprintf(f, "-(%4.1f,%4.1f,%4.1f)", c.x, c.y, c.z);
fprintf(f, "-a[");
for (lAretes::iterator i=la.begin(); i != la.end(); i++)
	fprintf(f, "-a(%2d) ", (*i)->num);
fprintf(f, "]");
}

//////////////////////////////////////////////////////////////////////////////
// renvoie les coordonnees du barycentre de l'arete  
//////////////////////////////////////////////////////////////////////////////
Point3D Arete::barycentre() 
{
	return (s1->coord()+s2->coord())/2.0;
}

//////////////////////////////////////////////////////////////////////////////
// ecriture de l'objet dans le flux o
//////////////////////////////////////////////////////////////////////////////
ostream & operator<<(ostream & o, Arete &a)
{
o << dec;
o << "A[" << a.num << "]";
o << "-s1(" << a.sommet1()->num << ") ";
o << "-s2(" << a.sommet2()->num << ") ";
if (a.face1() == FACE_NONE)
	o << "-f1(----) ";
else
	o << "-f1(" << a.face1()->num << ") "  ;
if (a.face2() == FACE_NONE)
	o << "-f2(----) ";
else
	o << "-f2(" << a.face2()->num << ") "  ;
return o;
}

//////////////////////////////////////////////////////////////////////////////
// ecriture de l'objet dans le flux o
//////////////////////////////////////////////////////////////////////////////
void Arete::write(ostream & o)
{
o << dec;
o << "A[" << num << "]";
o << (estExterne ? "E" : "I");
o << "-s1(" << s1->num << ") ";
o << "-s2(" << s2->num << ") ";
if (f1 == FACE_NONE)
	o << "-f1(----) ";
else
	o << "-f1(" << f1->num << ") "  ;
if (f2 == FACE_NONE)
	o << "-f2(----) ";
else
	o << "-f2(" << f2->num << ") "  ;
}

ostream & operator<<(ostream & o, Face &t)
{
o << dec;
o << "T[" << t.num << "] - a[ ";
lAretes la = t.aretes();
for (lAretes::iterator i=la.begin(); i != la.end(); i++)
	o << (*i)->num << " " ;
o << "]" ;
return o;
}

//////////////////////////////////////////////////////////////////////////////
// ecriture de l'objet dans le fichier f
//////////////////////////////////////////////////////////////////////////////
void Arete::printf(FILE *f)
{
fprintf(f, "A[%2d]", num);
fprintf(f, "-s1(%2d) ", s1->num);
fprintf(f, "-s2(%2d) ", s2->num);
if (f1 == FACE_NONE)
	fprintf(f, "-f1(--) ");
else
	fprintf(f, "-f1(%2d) ", f1->num);
if (f2 == FACE_NONE)
	fprintf(f, "-f2(--) ");
else
	fprintf(f, "-f2(%2d) ", f2->num);
}

///////////////////////////////////////////////////////////////////////////////
// détermine la liste des sommets de la face
///////////////////////////////////////////////////////////////////////////////
lSommets Face::sommets()
{
	lSommets ls;
	for (lAretes::iterator i=la.begin(); i != la.end(); i++)
	{
		PtrArete a = *i;
		ls.push_back(a->face1()==this ? a->sommet1() : a->sommet2());
	}
	return ls;
}

// détermine l'arete suivant l'arete a dans le sens trigo autour de la face
///////////////////////////////////////////////////////////////////////////////
PtrArete Face::arete_suivante_trigo(PtrArete a)
{
	if (a == ARETE_NONE) return ARETE_NONE;
	if (a == ARETE_BAD)  return ARETE_BAD;
	if (a == arete_finale()) return arete_initiale();
	
	// recherche de a dans la liste d'aretes
	lAretes::iterator i1 = la.begin();
	lAretes::iterator i2 = i1; i2++;
	
	while (i2 != la.end())
	{
		if (*i1 == a) return *i2;
		i1++; i2++;
	}
	return ARETE_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// détermine l'arete suivant l'arete a dans le sens trigo inv. autour de la face
///////////////////////////////////////////////////////////////////////////////
PtrArete Face::arete_suivante_trigo_inverse(PtrArete a)
{
	if (a == ARETE_NONE) return ARETE_NONE;
	if (a == ARETE_BAD)  return ARETE_BAD;
	if (a == arete_initiale()) return arete_finale();
	
	// recherche de a dans la liste d'aretes
	lAretes::reverse_iterator i1 = la.rbegin();
	lAretes::reverse_iterator i2 = i1; i2++;
	
	while (i2 != la.rend())
	{
		if (*i1 == a) return *i2;
		i1++; i2++;
	}
	return ARETE_NONE;
}

//////////////////////////////////////////////////////////////////////////////
// renvoie les coordonnees du barycentre de la liste de sommets ls 
//////////////////////////////////////////////////////////////////////////////
Point3D Sommet::barycentre(lSommets ls)
{
	Point3D B(0.0,0.0,0.0);
	for (lSommets::iterator i=ls.begin(); i!=ls.end(); i++)
		B += (*i)->coord();
	return B/ls.size();
}

//////////////////////////////////////////////////////////////////////////////
// renvoie les coordonnees du barycentre de la face 
//////////////////////////////////////////////////////////////////////////////
Point3D Face::barycentre() 
{
	return Sommet::barycentre(sommets());
}

//////////////////////////////////////////////////////////////////////////////
// ecriture de l'objet dans le flux o
//////////////////////////////////////////////////////////////////////////////
void Face::write(ostream & o)
{
o << dec;
o << "T[" << num << "]";
o << "-a[";
for (lAretes::iterator i=la.begin(); i != la.end(); i++)
	o << (*i)->num << " ";
o << "]";
}

//////////////////////////////////////////////////////////////////////////////
// ecriture de l'objet dans le fichier f
//////////////////////////////////////////////////////////////////////////////
void Face::printf(FILE *f)
{
fprintf(f, "T[%2d]", num);
fprintf(f, "-a[");
for (lAretes::iterator i=la.begin(); i != la.end(); i++)
	fprintf(f, "-a(%2d) ", (*i)->num);
fprintf(f, "]");
}

ostream & operator<<(ostream & o, Maillage &m)
{
	o << hex;
	o << "Maillage[" << (ULONG)(&m) << "]" << endl;
	o << dec;
	o << " nb sommets : " << m.nb_s() << endl;
	PtrSommet sc = m.ls();
	while (sc)
	{
		o << "  "; sc->write(o); o << endl; sc = sc->suiv;
	}
	o << " nb aretes : " << m.nb_a() << endl;
	PtrArete ac = m.la();
	while (ac)
	{
		o << "  "; ac->write(o); o << endl; ac = ac->suiv;
	}
	o << " nb faces : " << m.nb_f() << endl;
	PtrFace fc = m.lf();
	while (fc)
	{
		o << "  "; fc->write(o); o << endl; fc = fc->suiv;
	}
	o << endl;

	return o;
}

//////////////////////////////////////////////////////////////////////////////
// ecriture de l'objet dans le flux o
//////////////////////////////////////////////////////////////////////////////
void Maillage::write(ostream & o)
{
	o << hex;
	o << "Maillage[" << (ULONG)(this) << "]" << endl;
	o << dec;
	o << " nb sommets : " << nb_sommet << endl;
	PtrSommet sc = s_first;
	while (sc)
	{
		o << "  "; sc->write(o); o << endl; sc = sc->suiv;
	}
	o << " nb aretes : " << nb_arete << endl;
	PtrArete ac = a_first;
	while (ac)
	{
		o << "  "; ac->write(o); o << endl; ac = ac->suiv;
	}
	o << " nb faces : " << nb_face << endl;
	PtrFace fc = f_first;
	while (fc)
	{
		o << "  "; fc->write(o); o << endl; fc = fc->suiv;
	}
	o << endl;
}

//////////////////////////////////////////////////////////////////////////////
// ecriture de l'objet dans le fichier f
//////////////////////////////////////////////////////////////////////////////
void Maillage::printf(FILE *f)
{
	fprintf(f, "Maillage[%8X]\n", (ULONG)(this) );
	fprintf(f, " nb sommets : %d\n", nb_sommet);
	PtrSommet sc = s_first;
	while (sc)
	{
		fprintf(f, "  "); sc->printf(f); fprintf(f, "\n"); 
		sc = sc->suiv;
	}
	fprintf(f, " nb aretes : %d\n", nb_arete);
	PtrArete ac = a_first;
	while (ac)
	{
		fprintf(f, "  "); ac->printf(f); fprintf(f, "\n"); 
		ac = ac->suiv;
	}
	fprintf(f, " nb faces : %d\n", nb_face);
	PtrFace fc = f_first;
	while (fc)
	{
		fprintf(f, "  "); fc->printf(f); fprintf(f, "\n"); 
		fc = fc->suiv;
	}
	fprintf(f, "\n");
}

///////////////////////////////////////////////////////////////////////////////
// marquage des éléments d'un même type
///////////////////////////////////////////////////////////////////////////////
void Maillage::marqueSommets(ULONG m)
{
	PtrSommet sc = s_first;
	while (sc)
	{
		sc->marque(m); sc = sc->suiv;
	}
	
}

void Maillage::marqueAretes(ULONG m)
{
	PtrArete ac = a_first;
	while (ac)
	{
		ac->marque(m); ac = ac->suiv;
	}
	
}

void Maillage::marqueFaces(ULONG m)
{
	PtrFace fc = f_first;
	while (fc)
	{
		fc->marque(m); fc = fc->suiv;
	}
	
}

///////////////////////////////////////////////////////////////////////////////
// marquage des éléments d'un même type
///////////////////////////////////////////////////////////////////////////////
void Maillage::numerotation_sommets(ULONG num_initial)
{
	ULONG n=num_initial;
	PtrSommet sc = s_first;
	while (sc)
	{
		sc->num = n++; sc = sc->suiv;
	}
}

void Maillage::numerotation_aretes(ULONG num_initial)
{
	ULONG n=num_initial;
	PtrArete ac = a_first;
	while (ac)
	{
		ac->num = n++; ac = ac->suiv;
	}	
}

void Maillage::numerotation_faces(ULONG num_initial)
{
	ULONG n=num_initial;
	PtrFace fc = f_first;
	while (fc)
	{
		fc->num = n++; fc = fc->suiv;
	}
}

void Maillage::numerotation_elements(ULONG num_initial)
{
	numerotation_sommets(num_initial);
	numerotation_aretes(num_initial);
	numerotation_faces(num_initial);
}

///////////////////////////////////////////////////////////////////////////////
// renvoie le pointeur de l'élément (sommet,arete,face) 
// dont le numéro est n
// renvoie element_BAD si le numéro est invalide
///////////////////////////////////////////////////////////////////////////////
PtrSommet Maillage::sommet(ULONG n)
{
	PtrSommet ps = SOMMET_BAD;
	PtrSommet sc = s_first;
	while (sc)
	{
		if (sc->num == n)
		{
			ps = sc;
			break;
		}
		sc = sc->suiv;
	}
	return ps;
}

PtrArete Maillage::arete(ULONG n)
{
	PtrArete pa = ARETE_BAD;
	PtrArete ac = a_first;
	while (ac)
	{
		if (ac->num == n)
		{
			pa = ac;
			break;
		}
		ac = ac->suiv;
	}
	return pa;
}

PtrFace Maillage::face(ULONG n)
{
	PtrFace pf = FACE_BAD;
	PtrFace fc = f_first;
	while (fc)
	{
		if (fc->num == n)
		{
			pf = fc;
			break;
		}
		fc = fc->suiv;
	}
	return pf;
}


///////////////////////////////////////////////////////////////////////////////
// renvoie le sommet le plus proche du point C
///////////////////////////////////////////////////////////////////////////////
PtrSommet Maillage::sommet(Point3D C)
{
	PtrSommet s = s_first;
	if (s == SOMMET_NONE)
		return SOMMET_NONE;
	
	double dmax = distance_(C, s->coord()), d;
	PtrSommet s1 = s;
	while (s != SOMMET_NONE)
	{
		d = distance_(C, s->coord());
		if (dmax > d)
		{
			dmax = d; s1 = s;
		}
		s = s->suiv;
	}
	
	return s1;
}

///////////////////////////////////////////////////////////////////////////////
// opérations d'ajout / suppression d'élément
///////////////////////////////////////////////////////////////////////////////

void Maillage::add(PtrSommet snew)
{
	nb_sommet++;
//	// ajout en début de liste
// 	snew->suiv = s;
// 	snew->prec = SOMMET_NONE;
// 	if (s)
// 		s->prec = snew;
// 	s = snew;
	
	// ajout en fin de liste
	snew->suiv = SOMMET_NONE;
	snew->prec = s_last;
	if (s_last)
		s_last->suiv = snew;
	else
		s_first = snew;
	s_last = snew;
}  

void Maillage::add(PtrArete anew)
{
	nb_arete++;
//	// ajout en début de liste
// 	anew->suiv = a;
// 	anew->prec = ARETE_NONE;
// 	if (a)
// 		a->prec = anew;
// 	a = anew;
	
	// ajout en fin de liste
	anew->suiv = ARETE_NONE;
	anew->prec = a_last;
	if (a_last)
		a_last->suiv = anew;
	else
		a_first = anew;
	a_last = anew;
}  
  
void Maillage::add(PtrFace fnew)
{
	nb_face++;
//	// ajout en début de liste
// 	tnew->suiv = t;
// 	tnew->prec = FACE_NONE;
// 	if (t)
// 		t->prec = tnew;
// 	t = tnew;
	
	// ajout en fin de liste
	fnew->suiv = FACE_NONE;
	fnew->prec = f_last;
	if (f_last)
		f_last->suiv = fnew;
	else
		f_first = fnew;
	f_last = fnew;
}  

void Maillage::remove(PtrSommet s0)
{
	nb_sommet--;
	PtrSommet sprec = s0->prec;
	PtrSommet ssuiv = s0->suiv;
	if (sprec)
		sprec->suiv = ssuiv;
	if (ssuiv)
		ssuiv->prec = sprec;
	if (s0 == s_first)
		s_first = ssuiv;	
	if (s0 == s_last)
		s_last = sprec;
}  
 
void Maillage::remove(PtrArete a0)
{
	nb_arete--;
	PtrArete aprec = a0->prec;
	PtrArete asuiv = a0->suiv;
	if (aprec)
		aprec->suiv = asuiv;
	if (asuiv)
		asuiv->prec = aprec;
	if (a0 == a_first)
		a_first = asuiv;	
	if (a0 == a_last)
		a_last = aprec;
}  
  
void Maillage::remove(PtrFace f0)
{
	nb_face--;
	PtrFace fprec = f0->prec;
	PtrFace fsuiv = f0->suiv;
	if (fprec)
		fprec->suiv = fsuiv;
	if (fsuiv)
		fsuiv->prec = fprec;
	if (f0 == f_first)
		f_first = fsuiv;	
	if (f0 == f_last)
		f_last = fprec;
}  


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//            CREATION DE TRIANGULATIONS DE TYPES PARTICULIERS               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// structure demi_arete
typedef long long4[4];

#define FACE_VIDE 0xFFFFFFFF

///////////////////////////////////////////////////////////////////////////////
// routine pour le tri des demi-aretes par qsort()
///////////////////////////////////////////////////////////////////////////////
int comp_demi_arete(const void *p1, const void *p2)
{
	long *pda1 = ((long *)p1);
	long *pda2 = ((long *)p2);
	
	if (  pda1[0] <  pda2[0]  ) return(-1);
	
	if ( (pda1[0] == pda2[0]) && 
	     (pda1[1] <  pda2[1]) ) return(-1);
		 
	if ( (pda1[0] == pda2[0]) && 
	     (pda1[1] == pda2[1]) && 
	     (pda1[2] <  pda2[2]) ) return(-1);
		 
	return(1);
}

///////////////////////////////////////////////////////////////////////////////
// test si le maillage définit par le tableau de demi-aretes da est conforme
// Entrée : da = tableau de type long définissant les demi aretes
//          nF = nombre de faces (donc 3*nF demi-aretes)
//          FS = tableau d'indexation des sommets par face
//           FS[i+1]-FS[i] donne le nb d'aretes par face
//           FS[nF] donne le nombre de demi-aretes
// Sortie : da = tableau modifié (dans le cas conforme)
// la routine renvoie vrai si le maillage est conforme, faux sinon
// le tableau da est modifié par la routine, et contient un maillage orienté
// convenablement si le maillage est conforme         
///////////////////////////////////////////////////////////////////////////////
struct InfoFace
{
	vector<ULONG> ida; // indices dans le tableau da des trois demi-aretes
	int marque       ; // marque
};

bool est_conforme(long4 *da, ULONG nF, ULONG *FS)
{
	ULONG nDA = FS[nF]; // nb de demi-aretes;

// cout << "da : " << endl;
// for (int i=0; i<nDA; i++)
// {
// cout << i << ":(" << da[i][0] << "," << da[i][1] << "," << da[i][2] << "," << da[i][3] << ")" << endl;
// }

	//// passer en revue le tableau de demi-aretes afin de detecter 
	// les demi-aretes avec même sommet
	// les demi-aretes en triple et 
	// les demi-aretes en double mal orientées
	bool existe_demi_arete_avec_meme_sommet = false;
	bool existe_demi_arete_triple = false;
	bool existe_demi_arete_double_mal_orientee = false;
	ULONG k=0;
	while (k<nDA-1)
	{
		ULONG s1, s2;
		long tg, td;
		s1 = da[k][0]; s2 = da[k][1]; // sommets de la demi-arete 
		
		// tester si meme sommet
		if (s1==s2)
		{
			existe_demi_arete_avec_meme_sommet = true;
			break;
		}
		
		// tester si la demi-arete est triple
		if (k<nDA-2)
		{
			if ((s1 == da[k+2][0]) && (s2 == da[k+2][1]))
			{
// cout << "trouvée demi-arete triple " << k << endl;
				existe_demi_arete_triple = true;
				break; // sortir de la boucle while
			}
		}
		// tester si la demi-arete est double
		if ((s1 ==  da[k+1][0]) && 
			(s2 ==  da[k+1][1])     )
		{	
			// tester si la demi-arete est mal orientee :
			// faces de même signe			 
			tg =  da[k+1][2]; td = da[k][2];
			if ( ((tg<0) && (td<0)) || ((tg>0) && (td>0)) )
			{
// cout << "trouvée demi-arete mal orientée " << k << " " << tg << " " << td << endl;
				existe_demi_arete_double_mal_orientee = true;
				break; // sortir de la boucle while
			}
			k++;
		}
		k++;			
	} 	 
	
	if (existe_demi_arete_avec_meme_sommet) 
	{
cout << "Maillage::estConforme : existe_demi_arete_avec_meme_sommet" << endl;
		return false;
	}
	if (existe_demi_arete_triple) 
	{
cout << "Maillage::estConforme : existe_demi_arete_triple" << endl;
		return false;
	}
	if (!existe_demi_arete_double_mal_orientee) 
	{
cout << "Maillage::estConforme : aucune_demi_arete_double_mal_orientee" << endl;
	return true;
	}
		
// cout << "essayer de réorienter les différentes faces ..." << endl;
	//// essayer de réorienter les différentes faces 
	InfoFace infoFace[nF];
	
	// marquer les faces à 0
	for (int i=0; i<nF; i++) 
	{
		infoFace[i].marque=0;
		infoFace[i].ida.resize(FS[i+1]-FS[i]); 
	}
	
	// retrouver les indices des demi-aretes des faces
	for (int i=0; i<nDA; i++)
	{
		long f = da[i][2]; f = f<0 ? -f-1 : f-1; // numéro de la face
		int k = da[i][3]; // position de la demi-arete dans la face	
//cout << i << " " << f << " " << k << endl;	
		infoFace[f].ida[k] = i;
	}
	
	bool existe_face_non_marquee=true;
	while (existe_face_non_marquee && existe_demi_arete_double_mal_orientee)
	{

// cout << "Boucle while (existe_face_non_marquee)" << endl;
// 
// cout << "demi arete mal orientee : " << k << endl;
// 
// cout << "da : " << endl;
// for (int i=0; i<nDA; i++)
// {
// cout << i << ":(" << da[i][0] << "," << da[i][1] << "," << da[i][2] << "," << da[i][3] << ")" << endl;
// }
// 
// cout << "infoFace : " << endl;
// for (int i=0; i<nF; i++)
// {
// 	cout << infoFace[i].marque << " (";
// 	for (int j=0; j<infoFace[i].ida.size(); j++)
// 		cout << " " << infoFace[i].ida[j];
// 	cout << ")" << endl;	
// }
	
		// essayer de réorienter le maillage à partir de la demi-arete k
		long f1 = da[k][2]  ; f1 = f1<0 ? -f1-1 : f1-1;				 
		long f2 = da[k+1][2]; f2 = f2<0 ? -f2-1 : f2-1;	
		list<long> L;
		
		if (infoFace[f1].marque != 0 && infoFace[f2].marque	!= 0)
		{
cout << "Maillage::estConforme : maillage non orientable" << endl;
			return false;
		}
		
		if (infoFace[f1].marque == 0 && infoFace[f2].marque == 0)
		{
			infoFace[f1].marque = 1;
			L.push_front(f2);
		}
		else if (infoFace[f2].marque == 0)	
			L.push_front(f2);
		else
			L.push_front(f1);
			
		// réorienter les faces de L
		while (!L.empty())
		{

// cout << "Boucle while (!L.empty())" << endl;
// 
// cout << "L =" ;
// for (list<long>::iterator i=L.begin(); i!=L.end(); i++) cout << " " << (*i); 
// cout << endl;
// 
// cout << "infoFace : " << endl;
// for (int i=0; i<nF; i++)
// {
// 	cout << infoFace[i].marque << " (";
// 	for (int j=0; j<infoFace[i].ida.size(); j++)
// 		cout << " " << infoFace[i].ida[j];
// 	cout << ")" << endl;	
// }

			long f = L.front(); L.pop_front();
			
			// marquer f
			infoFace[f].marque = 1;
			
			// demi-aretes de f
			vector<ULONG> ida = infoFace[f].ida;
			
			//  leur nombre
			int nda = ida.size();
			
			for (int i=0; i<nda; i++)
			{
				ULONG a = ida[i];
				
				// inverser a
				da[a][2] = -da[a][2];
				
				// sommets de a
				ULONG s1 = da[a][0]; 
				ULONG s2 = da[a][1]; 
				
				// tester si a est une demi-arete interne
				bool estInterne=false;
				ULONG a1; // indice de l'autre demi-arete égale à a
				if (a>0)
				{
					a1 = a-1;
					if (s1==da[a1][0] && s2==da[a1][1])
						estInterne=true;
				}
				else if (a<nDA-1)				
				{
					a1 = a+1;
					if (s1==da[a1][0] && s2==da[a1][1])
						estInterne=true;
				}
				if (!estInterne) continue; // passer à la demi-arete suivante
				
				// tester si a est mal orientée
				bool a_est_mal_orientee = (
					(da[a][2]>0 && da[a1][2]>0) ||
					(da[a][2]<0 && da[a1][2]<0) );
					
				// l'autre face de a
				long f2 = da[a1][2]; f2 = f2<0 ? -f2-1 : f2-1;	
				
				if (infoFace[f2].marque==0)
				{
					infoFace[f2].marque = 1;
					if (a_est_mal_orientee)
						L.push_front(f2); // ajouter f2 à la liste L
				}
				else
				{					
					if (a_est_mal_orientee)
					{
cout << "Maillage::estConforme : maillage non orientable" << endl;
						return false;
					}
				}
			}
			// renverser l'ordre des demi-aretes de la face f
			for (int i=0; i<nda/2; i++)
			{
				int j=nda-1-i;
				ULONG ai = infoFace[f].ida[i];
				ULONG aj = infoFace[f].ida[j];
				
				// permuter les demi-aretes i et j				
				infoFace[f].ida[i] = aj;
				infoFace[f].ida[j] = ai;
				da[ai][3] = j;
				da[aj][3] = i;				
			}
			
		}
		
		// tester s'il existe une face non marquée
		existe_face_non_marquee = false;		 
		for (int i=0; i<nF; i++)
			if (infoFace[i].marque==0)
			{
				existe_face_non_marquee = true;
				break;
			}
			
		// chercher une arete interne mal orientée
		existe_demi_arete_double_mal_orientee = false;
		k=0;
		while (k<nDA-1)
		{
			ULONG s1, s2;
			long fg, fd;
			s1 = da[k][0]; s2 = da[k][1]; // sommets de la demi-arete 
		
			// tester si la demi-arete est double
			if ((s1 ==  da[k+1][0]) && 
			    (s2 ==  da[k+1][1])     )
			{	
				// tester si la demi-arete est mal orientee :
				// faces de même signe			 
				fg =  da[k+1][2]; fd = da[k][2];
				if ( ((fg<0) && (fd<0)) || ((fg>0) && (fd>0)) )
				{
// cout << "arete mal orientee : " << k << endl;				
					existe_demi_arete_double_mal_orientee = true;
					break; // sortir de la boucle while
				}
				k++;
			}
			k++;			
		} 

	}

	if (existe_demi_arete_double_mal_orientee) 
	{
cout << "Maillage::estConforme : maillage non orientable" << endl;
		return false;
	}

// cout << "da : " << endl;
// for (int i=0; i<nDA; i++)
// {
// cout << i << ":(" << da[i][0] << "," << da[i][1] << "," << da[i][2] << "," << da[i][3] << ")" << endl;
// }

qsort(da, nDA, sizeof(long4), comp_demi_arete);

cout << "da (apres tri) : " << endl;
// for (int i=0; i<nDA; i++)
// {
// cout << i << ":(" << da[i][0] << "," << da[i][1] << "," << da[i][2] << "," << da[i][3] << ")" << endl;
// }

	return true;
}


///////////////////////////////////////////////////////////////////////////////
// création d'une triangulation quelconque à partir d'une structure simple
// nS sommets S
// nT faces   T
// Entrée : S = tableau nS Point3D, S[i] = coordonnée du sommet i
//          nS = nb de sommets
//          T = tableau ULONG, 
//          TS = tableau nT+1 ULONG, 
//     TS[j] à TS[j+1]-1 = indices dans le tableau T des sommets de la face j
//          nT = nb de faces
///////////////////////////////////////////////////////////////////////////////
void Maillage::creer(Point3D *S, ULONG nS, ULONG *T, ULONG *TS, ULONG nT)
{
	// initialisation du maillage
	nb_sommet = nb_arete = nb_face = 0;
	s_first = s_last = SOMMET_NONE;
	a_first = a_last = ARETE_NONE;
	f_first = f_last = FACE_NONE;

	ULONG nb_da = TS[nT]; // nb de demi-aretes
	long4 *da;
	ULONG *TabT; // tableau liens face->arete
	ULONG *TabS;
	ULONG f, i, j, k, nA;
	
	// allocation tableau liens face->arete
	try {
		TabT = new ULONG[nb_da];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::creer" << " - memoire insuffissante " << endl;
		exit(1);
	}
	
	// allocation tableau de sommets
	try {
		TabS = new ULONG[nS];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::creer" << " - memoire insuffissante " << endl;
		exit(1);
	}

	// allocation tableau de demi-aretes
	try {
		da = new long4[nb_da];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::creer" << " - memoire insuffissante " << endl;
		exit(1);
	}

	j=0;
	for (f=0; f<nT; f++)
	{
		ULONG i1, i2, s1, s2;
		k=0; // numero de la demi-arete dans la face f
		i1 = TS[f];     // indice du premier sommet de f
		i2 = TS[f+1]-1; // indice du dernier sommet de f
		for (i=i1; i<=i2; i++)
		{
			s1 = T[i]; 
			s2 = i<i2 ? T[i+1] : T[i1];
			
			// demi-arete de la face i
			if (s1 < s2)
			{ da[j][0] = s1; da[j][1] = s2; da[j][2] = +f+1; }
			else
			{ da[j][0] = s2; da[j][1] = s1; da[j][2] = -f-1; }
			da[j][3] = k;
			j++;
			k++;
		}
	}
// cout << "Avant tri de da" << endl;
// for (i=0; i<3*nT; i++)
// 	cout << da[i][0] << " " << 
// 	        da[i][1] << " " << 
// 	        da[i][2] << " " << 
// 	        da[i][3] << endl;
			
	// tri des demi-aretes
	qsort(da, nb_da, sizeof(long4), comp_demi_arete);
// cout << "Apres tri de da" << endl;	
// for (i=0; i<3*nT; i++)
// 	cout << da[i][0] << " " << 
// 	        da[i][1] << " " << 
// 	        da[i][2] << " " << 
// 	        da[i][3] << endl;

// 	// tester la conformité
// 	if (!est_conforme(da, nT, TS))
// 	{
// cout << "Maillage::creer : impossible de créer le maillage" << endl;
// 		return;	
// 	}	
	
	// parcours des demi-arêtes pour la création des aretes
	k = 0;
	nA = 0;
	while (k<nb_da)
	{
		ULONG s1, s2, tg, td, atg, atd;
		
		// tester si les deux demi-arêtes ont les mêmes sommets
		s1 = da[k][0]; s2 = da[k][1];
		if ((k<nb_da-1)          && 
		    (s1 ==  da[k+1][0]) && 
			(s2 ==  da[k+1][1])     )
		{	
			// arête interne
			tg =  da[k+1][2]-1; atg = da[k+1][3];
			td = -da[k  ][2]-1; atd = da[k  ][3];
			k++;
		}	
		else if (da[k][2]>0)
		{
			// arete externe avec une face gauche
			tg = da[k][2]-1; atg = da[k][3];
			td = FACE_VIDE; 
		}		
		else
		{
			// arete externe avec une face droite
			td = -da[k][2]-1; atd = da[k][3];
			tg = FACE_VIDE; 
		}
					
// 		// lien des sommets s1 et s2 vers nA
// 		TabS[s1] = nA;
// 		TabS[s2] = nA;
		
		// lien des faces tg et td vers nA
		if (td != FACE_VIDE) TabT[TS[td]+atd] = nA;
		if (tg != FACE_VIDE) TabT[TS[tg]+atg] = nA;

		// lien de l'arete nA vers s1, s2, tg et td			
		da[nA][0] = s1; 
		da[nA][1] = s2; 
		da[nA][2] = tg; 
		da[nA][3] = td; 
		
		nA++;
		k++;
	}

// cout << " Après traitement des demi-aretes : " << endl;
// cout << " Sommets :" << endl;
// // for (i=0; i<nS; i++)
// // 	cout << " Sommet " << i << ":" << 
// // 	        TabS[i] << endl;
// cout << " Aretes :" << endl;
// for (i=0; i<nA; i++)
// 	cout << " Arete " << i << ":" << 
// 	        da[i][0] << "," <<
// 			da[i][1] << "," <<
// 			da[i][2] << "," <<
// 			da[i][3] << endl;
// cout << " Faces :" << endl;
// for (i=0; i<nT; i++)
// {
// 	cout << " Face " << i << ": ( ";
// 	for (j=TS[i]; j<TS[i+1]; j++) 
// 		cout <<TabT[j] << " ";
// 	cout << ")" << endl;
// }		

	// création des éléments de la triangulation
	PtrSommet* TPS;
	PtrArete*  TPA;
	PtrFace*   TPT;

	// allocation tableau de sommets
	try {
		TPS = new PtrSommet[nS];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::creer" << " - memoire insuffissante " << endl;
		exit(1);
	}
	// allocation tableau d'aretes
	try {
		TPA = new PtrArete[nA];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::creer" << " - memoire insuffissante " << endl;
		exit(1);
	}
	// allocation tableau de faces
	try {
		TPT = new PtrFace[nT];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::creer" << " - memoire insuffissante " << endl;
		exit(1);
	}

	nb_sommet = nb_arete = nb_face = 0;
	s_first = s_last = SOMMET_NONE;
	a_first = a_last = ARETE_NONE;
	f_first = f_last = FACE_NONE;
	
	for (i=0; i<nS; i++)
	{
		// allocation sommet
		try {
			TPS[i] = new Sommet;
		}
		catch(const exception& e)
		{
			cerr << "Maillage::creer" << " - memoire insuffissante " << endl;
			exit(1);
		}
		add(TPS[i]);
 		TPS[i]->num = i+1;
// cout << "add sommet : s =" << s << endl;
	}
	for (i=0; i<nA; i++)
	{
		// allocation arete
		try {
			TPA[i] = new Arete;
		}
		catch(const exception& e)
		{
			cerr << "Maillage::creer" << " - memoire insuffissante " << endl;
			exit(1);
		}
		add(TPA[i]);
 		TPA[i]->num = i+1;
// cout << "add arete : a =" << a << endl;
	}
	for (i=0; i<nT; i++)
	{
		// allocation face
		try {
			TPT[i] = new Face;
		}
		catch(const exception& e)
		{
			cerr << "Maillage::creer" << " - memoire insuffissante " << endl;
			exit(1);
		}
		add(TPT[i]);
 		TPT[i]->num = i+1;
// cout << "add face : t =" << t << endl;
	}
	
	// mise à jour relation arete->sommet,face
	for (i=0; i<nA; i++)
	{
		TPA[i]->sommet1(TPS[da[i][0]]);
		TPA[i]->sommet2(TPS[da[i][1]]);
		if (da[i][2]!=FACE_VIDE)
			TPA[i]->face1(TPT[da[i][2]]);
		if (da[i][3]!=FACE_VIDE)
			TPA[i]->face2(TPT[da[i][3]]);
		TPA[i]->estExterne = (da[i][2]==FACE_VIDE || da[i][3]==FACE_VIDE);
	}
	nb_arete = nA;
	
// cout << "Relation arete->sommet,face" << endl;
// for (i=0; i<nA; i++)
// {
// 	PtrArete a = TPA[i];
// 	cout << "Arete " << a->num << " : ";
// 	cout << a->sommet1()->num << "-";
// 	cout << a->sommet2()->num << " / ";
// 	if (a->face1() == FACE_NONE)
// 		cout << "..-";
// 	else
// 		cout << a->face1()->num << "-";
// 	if (a->face2() == FACE_NONE)
// 		cout << "..";
// 	else
// 		cout << a->face2()->num;
// 	cout << endl;
// }

	// mise à jour relation face->arete
	for (f=0; f<nT; f++)
	{
		ULONG i1, i2;
		
		i1 = TS[f];     // indice du premier sommet de f
		i2 = TS[f+1]-1; // indice du dernier sommet de f
		lAretes l;
		for (i=i1; i<=i2; i++)
			l.push_back(TPA[TabT[i]]);
		TPT[f]->aretes(l);
	}
	nb_face = nT;

// cout << "Relation face->arete" << endl;
// for (f=0; f<nT; f++)
// {
// 	lAretes l = TPT[f]->aretes();
// 	cout << "Face " << TPT[f]->num << " : [ ";
// 	for (lAretes::iterator i = l.begin(); i != l.end(); i++)
// 		cout << (*i)->num << " ";
// 	cout << "]" << endl;
// }
	
	// coordonnées des sommets
	for (i=0; i<nS; i++)
	{
		TPS[i]->X(S[i].x);
		TPS[i]->Y(S[i].y);
		TPS[i]->Z(S[i].z);
	}
	nb_sommet = nS;
	
	//// mise à jour relation sommet->arete ////
	marqueSommets(0);
	
	// relation sommet->arete pour les sommets externes
	for (i=0; i<nA; i++)
	if (TPA[i]->estExterne)
	{
		PtrArete a = TPA[i];
		PtrSommet s;
		PtrFace f;
		if (a->face1()==FACE_NONE)
		{
			s = a->sommet2();
			f = a->face2();
		}
		else
		{
			s = a->sommet1();
			f = a->face1();
		}
		s->estExterne = true;
		s->marque(1);
		
		// tourner à partir de a dans le sens trigo 
		// jusqu'à l'autre arete externe
		lAretes l;
		l.push_back(a);
		while (f != FACE_NONE)
		{
			a = f->arete_suivante_trigo_inverse(a);
			l.push_back(a);
			f = a->sommet1()==s ? a->face1() : a->face2();
		} 
		s->aretes(l);
	}
	
	// relation sommet->arete pour les sommets internes
	for (i=0; i<nA; i++)
	if (!TPA[i]->estExterne)
	{
		PtrArete a = TPA[i];
		PtrSommet s = a->sommet1();
		PtrFace f = a->face1();
		if (s->marque()==1) 
		{
			s = a->sommet2(); f = a->face2();
			if (s->marque()==1) continue; // les deux sommets de a déjà traités
		}
		
		s->estExterne = false;
		s->marque(1);
		
		// tourner à partir de a dans le sens trigo 
		// jusqu'à revenir à l'arete initiale
		lAretes l;
		PtrFace f1=f;
		do
		{
			l.push_back(a);
			a = f->arete_suivante_trigo_inverse(a);
			f = a->sommet1()==s ? a->face1() : a->face2();
		} while (f != f1);
		s->aretes(l);
	}
		
	// suppression des tableaux temporaires
	delete[] da; 
	delete[] TabS; 
	delete[] TabT; 
	delete[] TPS; 
	delete[] TPA; 
	delete[] TPT; 

	// numérotation des éléments
	numerotation_elements();
}


///////////////////////////////////////////////////////////////////////////////
// modifier le maillage pour le rendre triangulaire
// chaque face non triangulaire est triangulée en ajoutant un sommet en son
// barycentre
///////////////////////////////////////////////////////////////////////////////
void Maillage::trianguler()
{
	//// étape 1 : passer en revue le maillage pour //// 
	//// déterminer le nombre d'éléments à ajouter  //// 
	ULONG nS = nb_sommet; // nb de sommets 
	ULONG nF = nb_face  ; // nb de faces 
	ULONG nSS = 0; // nb de sommets supplémentaires
	ULONG nFT = 0; // nb final de triangles 
	
	// parcourir les faces pour déterminer celles à trianguler
// cout << "nSS = " << nSS << endl;
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		ULONG nsf = f1->nb_aretes(); // nombre de sommets de la face 
// cout << "nsf = " << nsf << endl;
		
		nSS += nsf>3 ? 1 : 0;
// cout << "nSS = " << nSS << endl;
		nFT += nsf>3 ? nsf : 1;
		f1->marque(nsf>3 ? 0 : 1);
	}
	
	// si le maillage déjà triangulaire ne rien faire
// cout << "nSS = " << nSS << endl;
	if (nSS==0) return;
	
	//// étape 2 : calculer la nouvelle triangulation ////
	
	// numéroter les sommets existants à partir de 0
	numerotation_sommets();
	
	// tableaux pour la nouvelle structure
	Point3D S[nS+nSS];
	ULONG T[nFT*3];
	ULONG TS[nFT+1];
	
	// copier les coord. des sommets existants
	ULONG ind_s=0;
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	{
		S[ind_s] = s1->coord(); ind_s++;
	}
	
	// calculer les nouveaux sommets
	// et les faces
	ULONG ind_f=0;
	TS[0] = 0;
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		lSommets lsf1 = f1->sommets(); // sommets de la face
		ULONG nsf = lsf1.size(); // nombre de sommets de la face 
		
		// numéros des sommets
		ULONG numS[nsf];
		lSommets::iterator i = lsf1.begin();
		for (int j=0; j<nsf; j++)
		{
			numS[j] = (*i)->num; i++;
		}
		
		if (nsf == 3)
		{
			// recopier la face telle quelle
			T[3*ind_f  ] = numS[0];
			T[3*ind_f+1] = numS[1];
			T[3*ind_f+2] = numS[2];
			ind_f++;	
			TS[ind_f] = 3*ind_f;
		}
		else
		{
			// calcul des coordonnées du nouveau sommet
			lSommets::iterator i = lsf1.begin();
			Point3D C(0,0,0);
			for (int j=0; j<nsf; j++)
			{
// 				C.x += (*i)->X();
// 				C.y += (*i)->Y();
// 				C.z += (*i)->Z();
//				C += (*i)->coord();
				Point3D X = (*i)->coord(); C += X;
				i++;
			}
// 			S[ind_s].x = C.x/nsf;
// 			S[ind_s].y = C.y/nsf;
// 			S[ind_s].z = C.z/nsf;
			S[ind_s] = C/nsf;
			
			// calcul des nouvelles faces
			for (int j=0; j<nsf; j++)
			{
				T[3*ind_f  ] = ind_s;
				T[3*ind_f+1] = numS[j];
				T[3*ind_f+2] = numS[(j+1)%nsf];
				ind_f++;	
				TS[ind_f] = 3*ind_f;
			}
			
			ind_s++;
		}
	}
	ULONG nb_ns = nS+nSS;
	
// for (int i=0; i<nS+nSS; i++)
// {
// cout << "Sommet " << i << " : [ " ;
// cout << S[i].x << " " << S[i].y << " " << S[i].z << "]" << endl;
// }
// 	
// for (int i=0; i<nFT; i++)
// {
// cout << "Face " << i << " : [ " ;
// for (int j=TS[i]; j<TS[i+1]; j++) cout << T[j] << " ";
// cout << "]" << endl;
// }

	//// modification de la structure ////
	
	// destruction des éléments existants
	supprimer_elements();
	
	// création des nouveaux éléments 
	creer(S, nS+nSS, T, TS, nFT);

}

///////////////////////////////////////////////////////////////////////////////
// modifier le maillage pour le rendre quadrangulaire
// chaque face est quadrangulée en ajoutant un sommet au barycentre et
// un sommet au baycentre de chaque arete
///////////////////////////////////////////////////////////////////////////////
void Maillage::quadranguler()
{
	////-----------------------------------------------------------------------
	//// calcul du nombre de sommets finaux 
	//// et du nombre de faces finales
	ULONG nb_ns; // nombre final de sommets 
	ULONG nb_nf; // nombre final de faces
	nb_ns = nb_sommet + nb_arete + nb_face;
	nb_nf = 2*nb_arete - nb_s_externes();
// 	nb_nf = 0;
// 	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
// 		nb_nf += f1->nb_sommets(); 
	
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
	Point3D Scoord[nb_ns];
	
	// coordonnées des nouveaux sommets correspondants aux sommets existants 	
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	{
		k = s1->marque();
		Scoord[k] = s1->coord();
	}
	
	// coordonnées des nouveaux sommets correspondants aux aretes existantes 	
	for (PtrArete a1 = a_first; a1 != ARETE_NONE; a1 = a1->suiv)
	{
		k = a1->marque();
		Scoord[k] = a1->barycentre();
	}
	
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		k = f1->marque();
		Scoord[k] = f1->barycentre();
	}
	
	////-----------------------------------------------------------------------
	//// création des tableaux des nouvelles faces (toutes quadrangulaires)
	int tailleT = 4*nb_nf;
	ULONG T[tailleT];
	ULONG TS[nb_nf+1];
	ULONG kT = 0;
	ULONG kTS = 0;
	TS[0] = 0;
	
	// pour chaque face, créer une nouvelle face pour chaque sommet 
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
		
}

///////////////////////////////////////////////////////////////////////////////
// création d'une triangulation quelconque à partir d'une structure simple
// nS sommets S
// nT faces   T
// Entrée : S = tableau nS Point3D, S[i] = coordonnée du sommet i
//          nS = nb de sommets
//          T = tableau ULONG, 
//          TS = tableau nT+1 ULONG, 
//     TS[j] à TS[j+1]-1 = indices dans le tableau T des sommets de la face j
//          nT = nb de faces
///////////////////////////////////////////////////////////////////////////////
Maillage::Maillage(double S[][3], ULONG nS, ULONG T[], ULONG TS[], ULONG nT)
{
	creer((Point3D *)S, nS, T, TS, nT);
}

Maillage::Maillage(Point3D *S, ULONG nS, ULONG T[], ULONG TS[], ULONG nT)
{
	creer(S, nS, T, TS, nT);
}

///////////////////////////////////////////////////////////////////////////////
// création d'une triangulation quelconque à partir d'une structure simple
// nS sommets   S
// nT faces T
// Entrée : S = tableau nS Point3D, S[i] = coordonnée du sommet i
//          nS = nb de sommets
//          T = tableau nT ulong3, T[j][k] = sommet k du face j
//          nT = nb de faces
///////////////////////////////////////////////////////////////////////////////
Maillage::Maillage(double S[][3], ULONG nS, ULONG T[], ULONG nT)
{
	ULONG* TS;
	
	// creer le tableau d'indices des triangles
	try {
		TS = new ULONG[nT+1];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::Maillage" << " - memoire insuffissante " << endl;
		exit(1);
	}

	for (ULONG i=0; i<=nT; i++)
		TS[i] = 3*i;
	
	creer((Point3D *)S, nS, T, TS, nT);
	delete[] TS;
}

Maillage::Maillage(Point3D *S, ULONG nS, ULONG *T, ULONG nT)
{
	ULONG* TS;
	
	// creer le tableau d'indices des triangles
	try {
		TS = new ULONG[nT+1];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::Maillage" << " - memoire insuffissante " << endl;
		exit(1);
	}

	for (ULONG i=0; i<=nT; i++)
		TS[i] = 3*i;
	
	creer(S, nS, T, TS, nT);
	delete[] TS;
}


///////////////////////////////////////////////////////////////////////////////
// CREATION A PARTIR DE FICHIERS
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// lecture d'un maillage dans un fichier OFF
// Entrée : nom_fichier = nom du fichier
// Sortie : S, nS, T, TS, nT : le maillage
//            S tableau des sommets
//            nS nb de sommets
//            T tableau des faces
//            TS tableau d'indicage des faces
//            nT nb de faces
//            la face i est décrite par les indices de ses sommets
//            dans le sous-tableau T[k1:k2] avec k1=TS[i] et k2=TS[i+1]-1
//          les tableaux sons créés dynamiquement à l'aide de new[]
//          et doivent etre détruits par l'utilisateur avec delete[]
///////////////////////////////////////////////////////////////////////////////
void Maillage::lire_off
  (const char *nom_fichier, Point3D* &S, ULONG &nS, ULONG* &T, ULONG* &TS, ULONG &nT)
{
	ifstream f(nom_fichier);
	
	if (! f.is_open())
	{
		cerr << "!!Maillage::creer_OFF : " << endl;
		cerr << "ouverture de " << nom_fichier << " impossible. " << endl;			
		return;
	}
// cout << "Maillage::creer_OFF("	<< nom_fichier << ")" << endl;

	int dummy;	
	double dummyD;	
	string entete;
	ULONG nST, i, j, k; 
	
// cout << "Passe 1\n";
	//// PASSE 1 : lecture des faces et 
	//// détermination du nombre d'éléments de la triangulation résultante
	
	// lecture de l'entete
	f >> entete;
// cout << entete << endl;
	
	// lecture du nombre de sommets et faces
	f >> nS >> nT >> dummy;
// cout << nS << " " << nT << endl;
	
	// lecture des sommets
	for (i=0; i<nS; i++)
	{
		f >> dummyD >> dummyD >> dummyD;
	}
	// lecture des faces
	nST=0;
	for (i=0; i<nT; i++)
	{
		f >> j;
		nST += j;
		while (j-- > 0) f >> dummy;
	}
	f.close();
	
// cout << "Passe 2\n";
	//// PASSE 2 : lecture des sommets, lecture des faces et création des 
	//// éléments de la triangulation résultante
	
	// allocation mémoire pour S
	try {
		S = new Point3D[nS];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::lire_off" << " - memoire insuffissante " << endl;
		exit(1);
	}
	// allocation mémoire pour T
	try {
		T = new ULONG[nST]; 
	}
	catch(const exception& e)
	{
		cerr << "Maillage::lire_off" << " - memoire insuffissante " << endl;
		exit(1);
	}
	// allocation mémoire pour TS
	try {
		TS = new ULONG[nT+1]; 
	}
	catch(const exception& e)
	{
		cerr << "Maillage::lire_off" << " - memoire insuffissante " << endl;
		exit(1);
	}
	
	f.open(nom_fichier);
	
	// lecture de l'entete
	f >> entete;
//  cout << entete << endl;
	
	// lecture du nombre de sommets et faces
	f >> dummy >> dummy >> dummy;
//  cout << nS << " " << nT << endl;
	
// cout << "Sommets\n";
	// lecture des sommets
	for (i=0; i<nS; i++)
	{
		f >> S[i].x >> S[i].y >> S[i].z;
//  cout << S[i].x << " " << S[i].y << " " << S[i].z << endl;
	}
	
// cout << "Faces\n";
	// lecture des faces
	TS[0] = 0;
	k = 0;
	for (i=0; i<nT; i++)
	{
		f >> j;
		TS[i+1] = TS[i]+j;
		
		// lecture des indices des sommets de la face i
		while (j-- > 0) f >> T[k++];
	}
	f.close();
	
// cout << "Maillage::creer_OFF - avant Maillage" << endl;
// for (i=0; i<nT; i++)
// {
// cout << "Face " << i << " : [ " ;
// for (j=TS[i]; j<TS[i+1]; j++) cout << T[j] << " ";
// cout << "]" << endl;
// }
 
}

///////////////////////////////////////////////////////////////////////////////
// création à partir d'un fichier OFF
///////////////////////////////////////////////////////////////////////////////
void Maillage::creer_OFF(const char *nom_fichier)
{
	Point3D* S;
	ULONG nS, nT, *T, *TS;
	
	lire_off(nom_fichier, S, nS, T, TS, nT);
	creer(S, nS, T, TS, nT);

	delete []S;
	delete []T;
	delete []TS;
}

///////////////////////////////////////////////////////////////////////////////
// constructeur par défaut
///////////////////////////////////////////////////////////////////////////////
Maillage::Maillage()
{
cout << "Maillage::Maillage()" << endl;
	nb_sommet = nb_arete = nb_face = 0;
	s_first = s_last = SOMMET_NONE;
	a_first = a_last = ARETE_NONE;
	f_first = f_last = FACE_NONE;
}

///////////////////////////////////////////////////////////////////////////////
// constructeur à partir des données d'un fichier
///////////////////////////////////////////////////////////////////////////////
Maillage::Maillage(const char *nom_fichier, int format)
{
	nb_sommet = nb_arete = nb_face = 0;
	switch (format)
	{
		case Maillage::OFF_FILE :
			creer_OFF(nom_fichier); break;
		
// 		case Maillage::NGB_FILE :
// 			creer_NGB(nom_fichier); break;
// 		
		default :
			cerr << "!!Maillage::Maillage : format inconnu" << endl;
	}
}

///////////////////////////////////////////////////////////////////////////////
// test si le maillage est n-angulaire (i.e. si toutes les faces ont n cotés)
// n entier>=3
///////////////////////////////////////////////////////////////////////////////
bool Maillage::n_angulaire(int n)
{
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		if (f1->nb_aretes() != n)
			return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// test si le maillage est n-régulier 
// (i.e. si tous les sommets internes sont de degré n)
// n entier>=3
///////////////////////////////////////////////////////////////////////////////
bool Maillage::n_regulier(int n)
{	
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	{
		if (!s1->estExterne && s1->nb_aretes() != n)
			return false;
	}
	return true;
	
}

///////////////////////////////////////////////////////////////////////////////
// renvoie le nb de sommets externes
///////////////////////////////////////////////////////////////////////////////
ULONG Maillage::nb_s_externes()
{
	ULONG k=0;
	
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	{
		if (s1->estExterne) k++;
	}
	return k;
}

///////////////////////////////////////////////////////////////////////////////
// calcule le barycentre du maillage (barycentre de la boite englobante)
///////////////////////////////////////////////////////////////////////////////
Point3D Maillage::barycentre()
{
	Point3D Pmin(0.0,0.0,0.0), Pmax;
	
	if (s_first == SOMMET_NONE)
		return Pmin;
	Pmin = Pmax = s_first->coord();
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	{
		Point3D P = s1->coord();
		if (Pmin.x > P.x) Pmin.x = P.x;
		if (Pmin.y > P.y) Pmin.y = P.y;
		if (Pmin.z > P.z) Pmin.z = P.z;
		if (Pmax.x < P.x) Pmax.x = P.x;
		if (Pmax.y < P.y) Pmax.y = P.y;
		if (Pmax.z < P.z) Pmax.z = P.z;
	}
	return (Pmin+Pmax)/2.0;
}

///////////////////////////////////////////////////////////////////////////////
// centrer le maillage afin que son barycentre soit à l'origine
///////////////////////////////////////////////////////////////////////////////
void Maillage::centrer()
{
	translater(-barycentre());
}

///////////////////////////////////////////////////////////////////////////////
// translater chaque sommet du point P
///////////////////////////////////////////////////////////////////////////////
void Maillage::translater(Point3D P)
{
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	{
		s1->coord(s1->coord()+P);
	}
}

///////////////////////////////////////////////////////////////////////////////
// effectuer une homothetie de rapport a
///////////////////////////////////////////////////////////////////////////////
void Maillage::homothetie(double a)
{
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	{
		s1->coord(s1->coord()*a);
	}
}

///////////////////////////////////////////////////////////////////////////////
// récupére la structure du maillage sous forme de tableaux
///////////////////////////////////////////////////////////////////////////////
void Maillage::structure
  (Point3D* &S, ULONG &nS, ULONG* &T, ULONG* &TS, ULONG &nT)	
{
	numerotation_elements();
	
	// allocation memoire pour S
	try {
		S = new Point3D[nb_sommet];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::structure" << " - memoire insuffissante " << endl;
		exit(1);
	}
	
	// allocation memoire pour TS
	try {
		TS = new ULONG[nb_face+1];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::structure" << " - memoire insuffissante " << endl;
		exit(1);
	}
	
	// calcule le nombre de sommets pour l'ensemble des faces
	ULONG nts=0;
	
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		nts += f1->nb_aretes();
	}
	
	// allocation memoire pour T
	try {
		T = new ULONG[nts];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::structure" << " - memoire insuffissante " << endl;
		exit(1);
	}
	
	// remplissage des tableaux
	ULONG kS = 0;
	for (PtrSommet s1 = s_first; s1 != SOMMET_NONE; s1 = s1->suiv)
	{
		S[kS] = s1->coord(); kS++;
	}
	
	ULONG kT = 0;
	ULONG kTS = 0;
	TS[0] = 0;	
	for (PtrFace f1 = f_first; f1 != FACE_NONE; f1 = f1->suiv)
	{
		lSommets lsf1 = f1->sommets();
		for (lSommets::iterator i = lsf1.begin(); i != lsf1.end(); i++)
		{
			T[kT] = (*i)->num; kT++;
		}
		kTS++; TS[kTS] = kT;
	}
	nS = nb_sommet;
	nT = nb_face  ;	
}

///////////////////////////////////////////////////////////////////////////////
// transforme un maillage plan en maillage 3D en effectuant une elévation
// en z de hauteur h
///////////////////////////////////////////////////////////////////////////////
void Maillage::elevation3d(double h)
{
	numerotation_elements();
	
	// récuperer la structure plane
	Point3D *S1;
	ULONG *F1;
	ULONG *FS1;
	ULONG nS1;
	ULONG nF1;
	ULONG ne = nb_s_externes();
	structure(S1,nS1,F1,FS1,nF1);
	
	// créer la structure finale
	ULONG nS2 = 2*nS1;
	ULONG nF2 = 2*nF1+ne;
	ULONG nFS1 = FS1[nF1];
	ULONG nFS2 = 2*nFS1+4*ne;
	Point3D *S2;
	ULONG *F2;
	ULONG *FS2;
	
	
	// allocation memoire pour S2
	try {
		S2 = new Point3D[nS2];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::elevation3d" << " - memoire insuffissante " << endl;
		exit(1);
	}
	// allocation memoire pour F2
	try {
		F2 = new ULONG[nFS2];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::elevation3d" << " - memoire insuffissante " << endl;
		exit(1);
	}
	// allocation memoire pour FS2
	try {
		FS2 = new ULONG[nF2+1];
	}
	catch(const exception& e)
	{
		cerr << "Maillage::elevation3d" << " - memoire insuffissante " << endl;
		exit(1);
	}
	
	// sommets
	for (ULONG i=0; i<nS1; i++)
	{
		Point3D C = S1[i];
		
		// sommets existants
		S2[i] = C;
		
		// nouveaux sommets existants
		C.z += h;
		S2[i+nS1] = C;		
	}
	
	// faces
	ULONG kF2 = 0;
	ULONG kFS2 = 0;
	FS2[0] = 0;

	// faces existantes
	for (ULONG j=0; j<nF1; j++)
	{
		ULONG nb_sommets_kF1 = FS1[j+1]-FS1[j];
		ULONG base = FS1[j];
		for (ULONG i=0; i<nb_sommets_kF1; i++)
		{
			F2[kF2] = F1[base+i]; kF2++;
		}
		kFS2++; FS2[kFS2] = kF2;
	}
	
	// duplication des faces existantes
	for (ULONG j=0; j<nF1; j++)
	{
		ULONG nb_sommets_kF1 = FS1[j+1]-FS1[j];
		ULONG base = FS1[j+1]-1;
		for (ULONG i=0; i<nb_sommets_kF1; i++)
		{
			F2[kF2] = F1[base-i]+nS1; kF2++;
		}
		kFS2++; FS2[kFS2] = kF2;
	}
	
	// créations des faces verticales sur les aretes externes
	for (PtrArete a1=a_first; a1 != ARETE_NONE; a1=a1->suiv)
	if (a1->estExterne)
	{
		ULONG s1, s2;
		if (a1->face1()==FACE_NONE) 
		{
			s1 = a1->sommet1()->num;
			s2 = a1->sommet2()->num;
		}
		else
		{
			s2 = a1->sommet1()->num;
			s1 = a1->sommet2()->num;
		}
		
		// la face quadrangulaire
		F2[kF2] = s1; kF2++;
		F2[kF2] = s2; kF2++;
		F2[kF2] = s2+nS1; kF2++;
		F2[kF2] = s1+nS1; kF2++;
		kFS2++; FS2[kFS2] = kF2;
	}
// cout << "nS1 = " << nS1 << endl;
// cout << "nF1 = " << nF1 << endl;
// cout << "nFS1 = " << nFS1 << endl;
// cout << "ne = " << ne << endl;
// cout << "nS2 = " << nS2 << endl;
// cout << "nF2 = " << nF2 << endl;
// cout << "nFS2 = " << nFS2 << endl;
// cout << "kF2 = " << kF2 << endl;
// cout << "kFS2 = " << kFS2 << endl;
// 	
// cout << "S2:" << endl;
// for (ULONG i=0; i<nS2; i++)
// {
// 	cout << S2[i].x << " " << S2[i].y << " " << S2[i].z << endl;
// }	
// 
// cout << "F2:" << endl;
// for (ULONG i=0; i<nFS2; i++)
// {
// 	cout << F2[i] << " ";
// }	
// cout << endl;
// 
// cout << "FS2:" << endl;
// for (ULONG i=0; i<=nF2; i++)
// {
// 	cout << FS2[i] << " ";
// }	
// cout << endl;
// 
// cout << "Faces:" << endl;
// for (ULONG i=0; i<nF2; i++)
// {
// 	for (ULONG j=FS2[i]; j<FS2[i+1]; j++)
// 		cout << F2[j] << " ";
// 	cout << endl;
// }	

	//// modification de la structure ////
	
	// destruction des éléments existants
	supprimer_elements();
	
	// création des nouveaux éléments 
	creer(S2, nS2, F2, FS2, nF2);

	delete []S1;
	delete []F1;
	delete []FS1;
	delete []S2;
	delete []F2;
	delete []FS2;
}

///////////////////////////////////////////////////////////////////////////////
// écriture de la triangulation dans le fichier GeomView dont le nom est
// nom_fichier - format OFF
///////////////////////////////////////////////////////////////////////////////
void Maillage::write_geomview(const char *nom_fichier)
{
	ofstream f(nom_fichier);
	
	if (! f.is_open())
	{
		cerr << "!!write_geomview : ouverture de " << nom_fichier
		     << " impossible. " << endl;			
		return;
	}
	
	numerotation_elements();
	// entete
	f << "OFF" << endl 
	  << nb_sommet << " " << nb_face << " " << nb_arete << endl;
	f << endl;
	// liste des sommets
	PtrSommet sc = s_first;
	while (sc)
	{
		Point3D c = sc->coord();
		f << c.x << " " << c.y << " " << c.z << endl;
		sc = sc->suiv;
	}
	f << endl;
	// liste des faces
	PtrFace fc = f_first;
	while (fc)
	{
		lSommets lsfc = fc->sommets();
		f << lsfc.size();
		for (lSommets::iterator i=lsfc.begin(); i != lsfc.end(); i++)
			f << " " << ((*i)->num) ;
		f << endl;
		fc = fc->suiv;
	}
	f.close();
}

///////////////////////////////////////////////////////////////////////////////
// écriture des bornes en x,y,z de la triangulation
///////////////////////////////////////////////////////////////////////////////
void Maillage::write_bounding_box(ostream & o)
{
	double xmin, xmax; PtrSommet sxmin,sxmax;
	double ymin, ymax; PtrSommet symin,symax;
	double zmin, zmax; PtrSommet szmin,szmax;
	PtrSommet sc = s_first;
	sxmin = sxmax = symin = symax = szmin = szmax = sc;
	xmin = xmax = sc->X(); 
	ymin = ymax = sc->Y(); 
	zmin = zmax = sc->Z(); 
	
	while (sc)
	{
		PtrSommet Sptr=sc;
		double xS = Sptr->X();
		double yS = Sptr->Y();
		double zS = Sptr->Z();
		
		if (xmin>xS) { xmin = xS; sxmin = Sptr; }
		if (ymin>yS) { ymin = yS; symin = Sptr; }
		if (zmin>zS) { zmin = zS; szmin = Sptr; }
		if (xmax<xS) { xmax = xS; sxmax = Sptr; }
		if (ymax<yS) { ymax = yS; symax = Sptr; }
		if (zmax<zS) { zmax = zS; szmax = Sptr; }
		sc = sc->suiv;
	}
	
	o << "sxmin : (" << sxmin->X() << "," << sxmin->Y() << "," << sxmin->Z() << ")\n"; 
	o << "sxmax : (" << sxmax->X() << "," << sxmax->Y() << "," << sxmax->Z() << ")\n"; 
	o << "symin : (" << symin->X() << "," << symin->Y() << "," << symin->Z() << ")\n"; 
	o << "symax : (" << symax->X() << "," << symax->Y() << "," << symax->Z() << ")\n"; 
	o << "szmin : (" << szmin->X() << "," << szmin->Y() << "," << szmin->Z() << ")\n"; 
	o << "szmax : (" << szmax->X() << "," << szmax->Y() << "," << szmax->Z() << ")\n"; 
}

// ///////////////////////////////////////////////////////////////////////////////
// // écriture de la triangulation dans le fichier VRML dont le nom est
// // nom_fichier - format VRML
// ///////////////////////////////////////////////////////////////////////////////
// void Maillage::write_vrml(char nom_fichier[])
// {
// 	ofstream f(nom_fichier);
// 	
// 	if (! f.is_open())
// 	{
// 		cerr << "!!write_vrml : ouverture de " << nom_fichier
// 		     << " impossible. " << endl;			
// 		return;
// 	}
// 	
// 	numerotation_elements();
// 	// entete
// 	f << "#VRML V2.0 utf8" << endl;
// 
// 	// forme
// 	f << "Shape {" << endl;
// 
// 	// couleur
// 	f << "  appearance Appearance {";
// 	f << "    material Material {";
// 	f << "      diffuseColor 0.8 0.8 0.8";
// 	f << "    }";
// 	f << "  }";
// 
// 	// géométrie
// 	f << "  geometry IndexedFaceSet {" << endl;
// 	f << "    colorPerVertex FALSE" << endl;
// 	f << "    solid FALSE" << endl;
// 	f << endl;
// 
// 	// liste des sommets
// 	f << "    coord Coordinate {" << endl;
// 	f << "      point [" << endl;
// 	PtrSommet sc = s;
// 	while (sc)
// 	{
// 		Point3D c = sc->coord();
// 		f << "        " << c.x << " " << c.y << " " << c.z << "," << endl;
// 		sc = sc->suiv;
// 	}
// 	f << "      ]" << endl;
// 	f << "    }" << endl;
// 	f << endl;
// 
// 	// liste des faces
// 	f << "    coordIndex [" << endl;
// 	PtrFace tc = t;
// 	while (tc)
// 	{
// 		f << "      " 
// 	      << tc->sommet1()->num-1 << " "          
// 		  << tc->sommet2()->num-1 << " "
// 		  << tc->sommet3()->num-1 << " -1," << endl;
// 		tc = tc->suiv;
// 	}
// 	f << "    ]" << endl;
// 
// 	// fin du fichier
// 	f << "  }" << endl; // geometry IndexedFaceSet
// 	f << "}" << endl;   // Shape
// 
// 	f << endl;
// 	f.close();
// }

///////////////////////////////////////////////////////////////////////////////
// destruction des éléments du maillage
///////////////////////////////////////////////////////////////////////////////
void Maillage::supprimer_elements()
{
	// suppression des sommets
	while (s_first)
	{
		PtrSommet ssuiv = s_first->suiv;
		delete s_first;
		s_first = ssuiv;
	}
	
	// suppression des aretes
	while (a_first)
	{
		PtrArete asuiv = a_first->suiv;
		delete a_first;
		a_first = asuiv;
	}
	
	// suppression des faces
	while (f_first)
	{
		PtrFace fsuiv = f_first->suiv;
		delete f_first;
		f_first = fsuiv;
	}
	
}

///////////////////////////////////////////////////////////////////////////////
// le destructeur
// suppression des 3 listes et des éléments correspondants
///////////////////////////////////////////////////////////////////////////////
Maillage::~Maillage()
{
	supprimer_elements();
}

// routines sur les listes
void write(lAretes la)
{
	cout << "(";
	for (lAretes::iterator i = la.begin(); i!=la.end(); i++)
	{
		if (i!=la.begin()) cout << ",";
		cout << (*i)->num ;
	}
	cout << ")";
}

void write(lFaces lf)
{
	cout << "(";
	for (lFaces::iterator i = lf.begin(); i!=lf.end(); i++)
	{
		if (i!=lf.begin()) cout << ",";
		cout << (*i)->num ;
	}
	cout << ")";
}
