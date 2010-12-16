#include "image.hpp"
#include "item.hpp"

using namespace std;

void item::writePgmItem(const char * fic){
	/* Ouverture */
	filebuf fb;
	fb.open(fic,ios::out);
	ostream os(&fb);

	/* Ecriture */
	os<<"P2"<<endl;
	os<<"# INFORMATIONS GENERALES"<<std::endl;
	os<<"# epaisseur moyenne "<<epaisseur<<std::endl;
	os<<"# nb trous "<<nbTrous<<std::endl;
	os<<"# aire "<<aire<<std::endl;
	os<<pIm->getLargeur()<<" "<<pIm->getHauteur()<<endl;
	os<<pIm->getValmax()<<endl;

	for(int i=0; i < pIm->getHauteur(); i++){
		for(int j=0; j < pIm->getLargeur(); j++){
			os<<(*pIm)(i,j);
			os<<endl;
		}
	}

	fb.close();
}


void item::loadEpaisseur(double** masque,int n,int seuil){
	image* AM=pIm->axeMedian(masque,n,seuil);
	epaisseur=0;
	int nbr=0;
	int ray;
	for(int i=0;i<pIm->getHauteur();i++){
		for(int j=0;j<pIm->getLargeur();j++){
			ray=(*AM)(i,j);
			if(ray>0){
				nbr++;
				epaisseur+=ray;
			}
		}
	}

	epaisseur=(epaisseur*2.0/((double)nbr))/masque[(n-1)/2][(n-1)/2+1];
}

void item::loadNbTrous(int seuil){
	image neg(*pIm);
	neg.negatif();
	int val=neg.getValmax();

	nbTrous=neg.nbConnCom(8,val-seuil)-1;
}

void item::loadAire(int seuil){
	aire=0;
	int hauteur=pIm->getHauteur();
	int largeur=pIm->getLargeur();
	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			if((*pIm)(i,j)>0)
				aire++;
		}
	}
}

void item::load(double** masque,int n,int seuil){
	loadEpaisseur(masque,n,seuil);
	loadNbTrous(seuil);
	loadAire(seuil);
}
