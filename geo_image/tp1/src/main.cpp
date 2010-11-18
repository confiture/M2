#include "image.hpp"

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
//image im("objets.pgm");
  //im.seuiller(100);
  //im.negatif();
  //im.EcrireImagePGM("negatif.pgm");
  //im.dispCompConn("compConn.pgm");
  //im.writePgmItems("objets",50);
 // im.dispCompConn("compConn.pgm");

  //image sortie=im.duplique_elemStruc_bord("elem_structurant.pgm");
  //sortie.EcrireImagePGM("imgDuplique.pgm");
 test_trous("objets.pgm");
return 0;
}
