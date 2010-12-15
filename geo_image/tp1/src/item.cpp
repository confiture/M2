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
	os<<"#INFORMATIONS GENERALES"<<std::endl;
	os<<"#epaisseur moyenne"<<epaisseur<<std::endl;
	os<<"#nb trous "<<nbTrous<<std::endl;
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

	epaisseur=epaisseur*2.0/((double)nbr);
}
