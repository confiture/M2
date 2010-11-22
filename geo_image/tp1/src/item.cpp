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
	os<<"#epaisseur "<<epaisseur<<std::endl;
	os<<"#nb trous "<<nbTrous<<std::endl;
	os<<pIm->getLargeur()<<" "<<pIm->getHauteur()<<endl;
	os<<pIm->getValmax()<<endl;

	for(int i=0; i < pIm->getHauteur(); i++){
		for(int j=0; j < pIm->getLargeur(); j++){
			os<<image(i,j);
			os<<endl;
		}
	}

	fb.close();
}
