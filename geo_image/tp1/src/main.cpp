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

void test_trous(const char * fic){
  image im(fic);
  image neg(im);
  neg.negatif();
  neg.EcrireImagePGM("negatifInit.pgm");

  neg.seuiller(neg.getValmax()-50);
  int** conn=neg.connexite8();

  int n=im.getHauteur();
  int m=im.getLargeur();
  image sortie(n,m,255);
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      sortie(i,j)=conn[i][j];
    }
  }
  sortie.recadre(0,200);

  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      if(sortie(i,j)!=0)sortie(i,j)+=50;
    }
  }

  //sortie.EcrireImagePGM("objetsBlancs.pgm");
  //sortie.seuiller(1);
  sortie.EcrireImagePGM("sortiePareil.pgm");
}

int main(int argc, char* argv[]){
using namespace std;

  //test_DupStructCarre("objets.pgm","imgDuplique10.pgm",10);
  image im("objets.pgm");
  image elem_structurant = creerImCarre(5);
  image* sortie=im.dilatation(elem_structurant);
  sortie->EcrireImagePGM("dilatation.pgm");

  
  
return 0;
}
