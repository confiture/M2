#include "image.hpp"
#include <cmath>


/*void test_DupStructCarre(char* fichier_entree ,char* fichier_sortie,int taille_carre){
  image im(fichier_entree);
  image elem_structurant = image::elemCarre(taille_carre);
  image* sortie=im.duplique_elemStruc_bord(elem_structurant);
  sortie->EcrireImagePGM(fichier_sortie);
  }*/

void test_trous(const char * fic){
  image im(fic);
  image neg(im);
  neg.negatif();
  neg.EcrireImagePGM("negatifInit.pgm");

  neg.seuiller(neg.getValmax()-50);
  int** conn=neg.binConnexite8();

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


void testDistT(const char* fic){
  double** masque=new double*[3];
  for(int i=0;i<3;i++){
    masque[i]=new double[3];
  }
  masque[0][0]=masque[0][2]=masque[2][0]=masque[2][2]=4;
  masque[0][1]=masque[1][0]=masque[2][1]=masque[1][2]=3;

  image im(fic);
  image* DT=im.distanceT(masque,3,50);
  DT->recadre(0,255);
  DT->EcrireImagePGM("testDistT.pgm");
}

void testBoule(){
  double** masque=new double*[3];
  for(int i=0;i<3;i++){
    masque[i]=new double[3];
  }
  masque[0][0]=masque[0][2]=masque[2][0]=masque[2][2]=4;
  masque[0][1]=masque[1][0]=masque[2][1]=masque[1][2]=3;

  image bl=image::boule(masque,3,2000);
  bl.updateValmax();
  bl.EcrireImagePGM("testBoule.pgm");
}

int main(int argc, char* argv[]){
  using namespace std;

  //testDistT(argv[1]);

  testBoule();

  return 0;

}
