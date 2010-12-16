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

void testAxeMed(const char* fic){
  double** masque=new double*[3];
  for(int i=0;i<3;i++){
    masque[i]=new double[3];
  }
  masque[0][0]=masque[0][2]=masque[2][0]=masque[2][2]=4;
  masque[0][1]=masque[1][0]=masque[2][1]=masque[1][2]=3;

  image im(fic);
  image* DT=im.axeMedian(masque,3,50);
  image* recons=DT->reconsAxeMed(masque,3);
  recons->EcrireImagePGM("reconstruction.pgm");
  DT->recadre(0,255);
  DT->EcrireImagePGM("testAxeMed.pgm");
}

void testBoulesMax(char * fic){
  double** masque=new double*[3];
  for(int i=0;i<3;i++){
    masque[i]=new double[3];
  }
  masque[0][0]=masque[0][2]=masque[2][0]=masque[2][2]=4;
  masque[0][1]=masque[1][0]=masque[2][1]=masque[1][2]=3;

  image im(fic);
  image* DT=im.boulesMax(masque,3,50);
  image* recons=DT->reconsAxeMed(masque,3);
  recons->EcrireImagePGM("reconstruction.pgm");
}

void testPetiteBoule(){
  double** masque=new double*[3];
  for(int i=0;i<3;i++){
    masque[i]=new double[3];
  }
  masque[0][0]=masque[0][2]=masque[2][0]=masque[2][2]=4;
  masque[0][1]=masque[1][0]=masque[2][1]=masque[1][2]=3;

  image b3=image::boule(masque,3,4);
  b3.EcrireImagePGM("petiteBoule.pgm");
}

void testBouleMax(char* fic){
  double** masque=new double*[3];
  for(int i=0;i<3;i++){
    masque[i]=new double[3];
  }
  masque[0][0]=masque[0][2]=masque[2][0]=masque[2][2]=4;
  masque[0][1]=masque[1][0]=masque[2][1]=masque[1][2]=3;

  image im(fic);
  image* dt=im.boulesMax(masque,3,50);
  dt->recadre(0,255);

  dt->EcrireImagePGM("dansKoi.pgm");
}

void testItems(char * fic){
  double** masque=new double*[3];
  for(int i=0;i<3;i++){
    masque[i]=new double[3];
  }

  masque[0][0]=masque[0][2]=masque[2][0]=masque[2][2]=4;
  masque[0][1]=masque[1][0]=masque[2][1]=masque[1][2]=3;
  image im(fic);
  im.writePgmItems("item",50,masque,3);
}

int main(int argc, char* argv[]){
using namespace std;

 //testDistT(argv[1]);
 //testAxeMed(argv[1]);
 //testPetiteBoule();
 //testBouleMax(argv[1]);
//testBoulesMax(argv[1]);
 testItems(argv[1]);

  // Test dilatation
//   image im("objets.pgm");
//   image elem_structurant = image::elemCarre(5);
//   image* sortie=im.dilatation(elem_structurant);
//   sortie->EcrireImagePGM("dilatation.pgm");

  //    image im("objets.pgm");
  //    image* sortie=im.dilatation(image::carre,11);
   //   sortie->EcrireImagePGM("test_cercle_ICI.pgm");



    //  image elemCercle(5);
    //  image* el->EcrireImagePGM("CercleTest.pgm");

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
//elem_cercle.EcrireImagePGM("cercle_ICI.pgm");

// image im("riz.pgm");
// // image* dilatation=im.dilatation(image::cercle,11,false);
// // dilatation->EcrireImagePGM("dilatation.pgm");

//  image* ouverture=im.ouverture(image::cercle,15);
//  ouverture->EcrireImagePGM("ouverture_cercle.pgm");

//  image* im_duplique=im.duplique_elemStruc_bord(15);
//  im_duplique->EcrireImagePGM("im_duplique.pgm");
// //
// //
//  image enleve_fond = (*im_duplique)-(*ouverture);
//  enleve_fond.EcrireImagePGM("Resultat.pgm");
//  enleve_fond.seuiller(5);
//  enleve_fond.EcrireImagePGM("ResultatSeuiller.pgm");
//   image* ouverturefin=enleve_fond.ouverture(image::cercle,15);
//  ouverturefin->EcrireImagePGM("ResultatFinal.pgm");

//image* sortie=im.erosion(image::triangle,11);
//sortie->EcrireImagePGM("erosion_triangle.pgm");
//image elem_structurant = image::elemCroix(11,1);


//image* sortie=im.dilatation(elem_structurant);
//sortie->EcrireImagePGM("dilatation_croix.pgm");


//image* sortie3=im.dilatation(image::cercle,5);
//sortie3->EcrireImagePGM("dilatation_cercle5.pgm");

/*image* sortie2=im.dilatation(image::carre,11);
sortie2->EcrireImagePGM("dilatation_carre.pgm");*/

//image* sortie4=im.erosion(image::carre,11);
//sortie4->EcrireImagePGM("erosion_carre.pgm");

 //image cercle = image::elemStruct(image::cercle,11);
 //cercle.EcrireImagePGM("CERCLE.pgm");




//image elem_carre = image::elemStruct(image::carre,11);
//elem_carre.EcrireImagePGM("carre_ICI.pgm");




// Test fonction elemCarre
// image elem_structurant = image::elemCarre(15);
// elem_structurant.EcrireImagePGM("carre.pgm");
//image elem_carre = image::elemStruct(image::carre,15);
//elem_carre.EcrireImagePGM("carre_ICI.pgm");

return 0;

}
