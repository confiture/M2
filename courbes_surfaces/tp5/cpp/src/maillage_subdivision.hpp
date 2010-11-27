/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/* Surcharge de maillage pour effectuer de la subdivision                    */ 
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/

#ifndef _MAILLAGE_SUBDIVISION_H_
#define _MAILLAGE_SUBDIVISION_H_

#include"maillage.hpp"

class MaillageSubdivision : public Maillage
{

public :
	// constructeurs
	MaillageSubdivision(double S[][3], ULONG nS, ULONG T[], ULONG TS[], ULONG nT);	
	MaillageSubdivision(Point3D *S, ULONG nS, ULONG T[], ULONG TS[], ULONG nT);	
	MaillageSubdivision(char *nom_fichier, int format=Maillage::OFF_FILE);
	// méthodes de subdivision sur des maillages quelconques
	void insertion();
	void moyennage();
	void moyennage2();
	void subdivision_doo_sabin();
	void subdivision_catmull_clark();
	void subdivision_simple_arete_face();
	void subdivision_simple_arete();
	void subdivision_simple_face();
	void subdivision_midedge();
	// méthodes de subdivision sur des maillages triangulaires
	void subdivision_sqrt3();
	void subdivision_butterfly();
	void subdivision_loop();
	// méthodes de subdivision sur des maillages quadrangulaires
	void subdivision_catmull_clark_quad();	

	// numéros des méthodes
	static const int doo_sabin=0;
	static const int catmull_clark=1;
	static const int midedge=2;
	static const int sqrt3=3;
	static const int butterfly=4;
	static const int loop=5;
};

#endif
