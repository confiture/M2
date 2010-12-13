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

int main(int argc, char* argv[]){
using namespace std;


 testDistT(argv[1]);


  // Test dilatation
//   image im("objets.pgm");
//   image elem_structurant = image::elemCarre(5);
//   image* sortie=im.dilatation(elem_structurant);
//   sortie->EcrireImagePGM("dilatation.pgm");
<<<<<<< HEAD
      image im("objets.pgm");
      image* sortie=im.dilatation(image::carre,11);
   //   sortie->EcrireImagePGM("test_cercle_ICI.pgm");
     
   
   
    //  image elemCercle(5);
    //  image* el->EcrireImagePGM("CercleTest.pgm");
=======
   //   image im("objets.pgm");
   //   image* sortie=im.erosion(image::cercle,11);
   //   sortie->EcrireImagePGM("test_cercle.pgm");

//image elemCercle(5);
//    image* el->EcrireImagePGM("CercleTest.pgm");
>>>>>>> 37c40e22bca074b0f0c23849b21e5fc4d6813ac7

     //image sortie= image::elemCercle(11);
     //sortie.EcrireImagePGM("CERCLE.PGM");
  // Test erosion
//   image im("objets.pgm");
//   image elem_structurant = image::elemCarre(5);
//   image* sortie=im.erosion(elem_structurant);
//   sortie->EcrireImagePGM("erosion.pgm");

  // Test ouverture
//     image im("objets.pgm");
//   image elem_structurant = image::elemCarre(5);
//   image* sortie=im.ouverture(elem_structurant);
//   sortie->EcrireImagePGM("ouvertureCarre5.pgm");

  // Test fermeture
//    image im("objets.pgm");
//    image elem_structurant = image::elemCarre(5);
//    image* sortie=im.fermeture(elem_structurant);
//    sortie->EcrireImagePGM("fermetureCarre5.pgm");

// Test fonction elemCercle
// image elem_structurant = image::elemCercle(15);
// elem_structurant.EcrireImagePGM("cercle.pgm");
image elem_cercle = image::elemStruct(image::cercle,15);
elem_cercle.EcrireImagePGM("cercle_ICI.pgm");

// Test fonction elemCarre
// image elem_structurant = image::elemCarre(15);
// elem_structurant.EcrireImagePGM("carre.pgm");
//image elem_carre = image::elemStruct(image::carre,15);
//elem_carre.EcrireImagePGM("carre_ICI.pgm");

return 0;

}
