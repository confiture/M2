#include "image.hpp"



// Cette fonction permet de creer une image représentant l'élément structurant qui sera
// donc un carre de taille 'taille'
image creerImCarre(int taille){
  image sortie(taille,taille,255);
  for(int i=0;i<taille;i++){
		for(int j=0;j<taille;j++){
			sortie(i,j)=1;
		}
	}
	return sortie; 
}

void test_DupStructCarre(char* fichier_entree ,char* fichier_sortie,int taille_carre){
  image im(fichier_entree);
  image elem_structurant = creerImCarre(taille_carre);
  image* sortie=im.duplique_elemStruc_bord(elem_structurant);
  sortie->EcrireImagePGM(fichier_sortie);
}

int main(int argc, char* argv[]){
using namespace std;

  //test_DupStructCarre("objets.pgm","imgDuplique10.pgm",10);
  image im("objets.pgm");
  image elem_structurant = creerImCarre(5);
  image* sortie=im.dilatation(elem_structurant);
  sortie->EcrireImagePGM("dilatation.pgm");

  
  
  //im.seuiller(100);
  //im.negatif();
  //im.EcrireImagePGM("negatif.pgm");
  //im.dispCompConn("compConn.pgm");
  //im.writePgmItems("tests.pgm",50);
 // im.dispCompConn("compConn.pgm");

return 0;
}
