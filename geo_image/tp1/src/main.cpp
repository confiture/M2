#include "image.hpp"
#include <cmath>
using namespace std;

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

void test_morpho(){
  cout<<"----------------------------------"<<endl;
  cout<<"TEST DES OPERATEURS MORPHOLOGIQUES"<<endl;
  cout<<"----------------------------------"<<endl<<endl;
  cout<<"Entrer le nom de l'image (image_entree.pgm) que vous souhaitez traiter : ";
  char* nomFichier=new char[40];
  cin>>nomFichier;


  cout<<endl<<"Choix de l'operateur :"<<endl;
  cout<<"1 : Dilatation"<<endl;
  cout<<"2 : Erosion"<<endl;
  cout<<"3 : Ouverture"<<endl;
  cout<<"4 : Fermeture"<<endl;
  cout<<"Entrer votre choix : ";
  int choix_oper;
  cin>>choix_oper;
  while(choix_oper!=1 && choix_oper!=2 && choix_oper!=3 && choix_oper!=4){
    cout<<"Choix incorrect..."<<endl<<endl;
    cout<<"Choix de l'operateur :"<<endl;
    cout<<"1 : Dilatation"<<endl;
    cout<<"2 : Erosion"<<endl;
    cout<<"3 : Ouverture"<<endl;
    cout<<"4 : Fermeture"<<endl;
    cout<<"Entrer votre choix : "<<endl;
    cin>>choix_oper;
  }

  cout<<endl<<"Choix de l'element structurant : "<<endl;
  cout<<"1 : carre"<<endl;
  cout<<"2 : cercle"<<endl;
  cout<<"3 : triangle"<<endl;
  cout<<"4 : croix"<<endl;
  cout<<"Entrer votre choix : ";
  int choix_elem;
  cin>>choix_elem;
  while(choix_elem!=1 && choix_elem!=2 && choix_elem!=3 && choix_elem!=4){
    cout<<"Choix incorrect..."<<endl<<endl;
    cout<<"Choix de l'element structurant :"<<endl;
    cout<<"1 : carre"<<endl;
    cout<<"2 : cercle"<<endl;
    cout<<"3 : triangle"<<endl;
    cout<<"4 : croix"<<endl;
    cout<<"Entrer votre choix : "<<endl;
    cin>>choix_elem;
  }

  image im(nomFichier);
  image elem_structurant;
  int taille;
  int epaisseur;
  image* sortie;

  switch (choix_oper) {

    // Dilatation
  case 1 :
    switch(choix_elem){
      // carre
    case 1 :
      cout<<"Entrer la taille en pixel du cote de votre carre : ";
      cin>>taille;
      sortie=im.dilatation(image::carre,taille,false);
      break;
      // cercle
    case 2 :
      cout<<"Entrer la taille (entier impair) en pixel du diametre de votre cercle : ";
      cin>>taille;
      while (taille%2!=1){
        cout<<"Taille incorrecte..."<<endl;
        cout<<"Entrer la taille (entier impair) en pixel du diametre de votre cercle : ";
        cin>>taille;
      }
      sortie=im.dilatation(image::cercle,taille,false);
      break;
      // triangle
    case 3 :
      cout<<"Entrer la taille en pixel du cote de votre triangle : ";
      cin>>taille;
      sortie=im.dilatation(image::triangle,taille,false);
      break;
      // croix
    case 4 :
      cout<<"Entrer la taille (entier impair) en pixel de votre croix : ";
      cin>>taille;
      while (taille%2!=1){
        cout<<"Taille incorrecte..."<<endl;
        cout<<"Entrer la taille (entier impair) en pixel de votre croix : ";
        cin>>taille;
      }
      cout<<"Entrer l'epaisseur (entier impair & epaisseur<taille) en pixel de votre croix : ";
      cin>>epaisseur;
      cout<<"taille : "<<taille<<endl;
      cout<<"epaisseur : "<<epaisseur<<endl;
      while (epaisseur%2!=1 || epaisseur>=taille){
        cout<<"Epaisseur incorrecte..."<<endl;
        cout<<"Entrer l'epaisseur (entier impair  & epaisseur<taille) en pixel de votre croix : ";
        cin>>epaisseur;
      }
      elem_structurant= image::elemCroix(taille,epaisseur);
      sortie=im.dilatation(elem_structurant,false);
      break;
    }
    break;

    // Erosion
  case 2 :
    switch(choix_elem){
      // carre
    case 1 :
      cout<<"Entrer la taille en pixel du cote de votre carre : ";
      cin>>taille;
      sortie=im.erosion(image::carre,taille,false);
      break;
      // cercle
    case 2 :
      cout<<"Entrer la taille (entier impair) en pixel du diametre de votre cercle : ";
      cin>>taille;
      while (taille%2!=1){
        cout<<"Taille incorrecte..."<<endl;
        cout<<"Entrer la taille (entier impair) en pixel du diametre de votre cercle : ";
        cin>>taille;
      }
      sortie=im.erosion(image::cercle,taille,false);
      break;
      // triangle
    case 3 :
      cout<<"Entrer la taille en pixel du cote de votre triangle : ";
      cin>>taille;
      sortie=im.erosion(image::triangle,taille,false);
      break;
      // croix
    case 4 :
      cout<<"Entrer la taille (entier impair) en pixel de votre croix : ";
      cin>>taille;
      while (taille%2!=1){
        cout<<"Taille incorrecte..."<<endl;
        cout<<"Entrer la taille (entier impair) en pixel de votre croix : ";
        cin>>taille;
      }
      cout<<"Entrer l'epaisseur (entier impair & epaisseur<taille) en pixel de votre croix : ";
      cin>>epaisseur;
      cout<<"taille : "<<taille<<endl;
      cout<<"epaisseur : "<<epaisseur<<endl;
      while (epaisseur%2!=1 || epaisseur>=taille){
        cout<<"Epaisseur incorrecte..."<<endl;
        cout<<"Entrer l'epaisseur (entier impair  & epaisseur<taille) en pixel de votre croix : ";
        cin>>epaisseur;
      }
      elem_structurant= image::elemCroix(taille,epaisseur);
      sortie=im.erosion(elem_structurant,false);
      break;
    }
    break;

    break;
    // Ouverture
  case 3 :
		switch(choix_elem){
      // carre
    case 1 :
      cout<<"Entrer la taille en pixel du cote de votre carre : ";
      cin>>taille;
      sortie=im.ouverture(image::carre,taille);
      break;
      // cercle
    case 2 :
      cout<<"Entrer la taille (entier impair) en pixel du diametre de votre cercle : ";
      cin>>taille;
      while (taille%2!=1){
        cout<<"Taille incorrecte..."<<endl;
        cout<<"Entrer la taille (entier impair) en pixel du diametre de votre cercle : ";
        cin>>taille;
      }
      sortie=im.ouverture(image::cercle,taille);
      break;
      // triangle
    case 3 :
      cout<<"Entrer la taille en pixel du cote de votre triangle : ";
      cin>>taille;
      sortie=im.ouverture(image::triangle,taille);
      break;
      // croix
    case 4 :
      cout<<"Entrer la taille (entier impair) en pixel de votre croix : ";
      cin>>taille;
      while (taille%2!=1){
        cout<<"Taille incorrecte..."<<endl;
        cout<<"Entrer la taille (entier impair) en pixel de votre croix : ";
        cin>>taille;
      }
      cout<<"Entrer l'epaisseur (entier impair & epaisseur<taille) en pixel de votre croix : ";
      cin>>epaisseur;
      cout<<"taille : "<<taille<<endl;
      cout<<"epaisseur : "<<epaisseur<<endl;
      while (epaisseur%2!=1 || epaisseur>=taille){
        cout<<"Epaisseur incorrecte..."<<endl;
        cout<<"Entrer l'epaisseur (entier impair  & epaisseur<taille) en pixel de votre croix : ";
        cin>>epaisseur;
      }
      elem_structurant= image::elemCroix(taille,epaisseur);
      sortie=im.ouverture(elem_structurant);
      break;
    }
    break;

    // Fermeture
  case 4 :
    switch(choix_elem){
      // carre
    case 1 :
      cout<<"Entrer la taille en pixel du cote de votre carre : ";
      cin>>taille;
      sortie=im.fermeture(image::carre,taille);
      break;
      // cercle
    case 2 :
      cout<<"Entrer la taille (entier impair) en pixel du diametre de votre cercle : ";
      cin>>taille;
      while (taille%2!=1){
        cout<<"Taille incorrecte..."<<endl;
        cout<<"Entrer la taille (entier impair) en pixel du diametre de votre cercle : ";
        cin>>taille;
      }
      sortie=im.fermeture(image::cercle,taille);
      break;
      // triangle
    case 3 :
      cout<<"Entrer la taille en pixel du cote de votre triangle : ";
      cin>>taille;
      sortie=im.fermeture(image::triangle,taille);
      break;
      // croix
    case 4 :
      cout<<"Entrer la taille (entier impair) en pixel de votre croix : ";
      cin>>taille;
      while (taille%2!=1){
        cout<<"Taille incorrecte..."<<endl;
        cout<<"Entrer la taille (entier impair) en pixel de votre croix : ";
        cin>>taille;
      }
      cout<<"Entrer l'epaisseur (entier impair & epaisseur<taille) en pixel de votre croix : ";
      cin>>epaisseur;
      cout<<"taille : "<<taille<<endl;
      cout<<"epaisseur : "<<epaisseur<<endl;
      while (epaisseur%2!=1 || epaisseur>=taille){
        cout<<"Epaisseur incorrecte..."<<endl;
        cout<<"Entrer l'epaisseur (entier impair  & epaisseur<taille) en pixel de votre croix : ";
        cin>>epaisseur;
      }
      elem_structurant= image::elemCroix(taille,epaisseur);
      sortie=im.fermeture(elem_structurant);
      break;
    }
    break;
  }


  cout<<"Traitement effectue !"<<endl;
  cout<<"Entrer le nom de l'image (image_sortie.pgm) ou vous souhaitez sauvegarder le traitement : ";
  char* nomFichierSortie=new char[40];
  cin>>nomFichierSortie;
  sortie->EcrireImagePGM(nomFichierSortie);
  cout<<"Image sauvegardee."<<endl;
}

void grains_de_riz(){
  // Lecture de l'image //
  image im("riz.pgm");

  // Différents éléments structurants nécessaires
  image cercle = image::elemStruct(image::cercle,19);
  image cercle2 = image::elemStruct(image::cercle,7);
  image cercle3 = image::elemStruct(image::cercle,5);


  cercle.EcrireImagePGM("cercle.pgm");

  // On effectue tout d'abord une ouverture //
  image* ouverture=im.ouverture(cercle);
  ouverture->EcrireImagePGM("etape1_ouverture_cercle19.pgm");


  // On effectue une correction de fond par chapeau haut de forme //
  image* im_duplique=im.duplique_elemStruc_bord(19);
  im_duplique->EcrireImagePGM("image_bord_duplique.pgm");
  image enleve_fond = (*im_duplique)-(*ouverture);
  enleve_fond.EcrireImagePGM("etape2_image-ouverture.pgm");


  // On seuille //
  enleve_fond.seuiller(2);
  enleve_fond.EcrireImagePGM("etape3_seuillage2.pgm");

  // On effectue une ouverture //
  image* ouverture2 =enleve_fond.ouverture(cercle2);
  ouverture2->EcrireImagePGM("etape4_ouverture_cercle7.pgm");

  // On effectue 2 erosions
  image* erosion = ouverture2->erosion(cercle3,true);
  erosion->EcrireImagePGM("etape5_erosion_cercle5.pgm");

  image* erosion2 = erosion->erosion(cercle3,true);
  erosion2->EcrireImagePGM("etape6_erosion_cercle5.pgm");

  // On effectue une ouverture //
  image* ouverture3 = erosion2->ouverture(cercle3);
  ouverture3->EcrireImagePGM("etape_final_ouverture_cercle5.pgm");

  // On compte le nombre de grains de riz//
  int nb_grains_riz = ouverture3->nbConnCom(4,1);
  cout<<"Il y a "<<nb_grains_riz<<" grains de riz sur l'image."<<endl;
}

void exercice2(){
  cout<<"----------------------------------"<<endl;
  cout<<"------------EXERCICE 2------------"<<endl;
  cout<<"----------------------------------"<<endl;
  cout<<"1 : Tests sur les operateurs morphologiques"<<endl;
  cout<<"2 : Compter le nombre de grains de riz sur l'image riz.pgm"<<endl;
  cout<<"Entrer votre choix : ";
  int choix;
  cin>>choix;
  while(choix!=1 && choix!=2){
    cout<<"Choix incorrect..."<<endl<<endl;
    cout<<"1 : Tests sur les operateurs morphologiques"<<endl;
    cout<<"2 : Compter le nombre de grains de riz sur l'image riz.pgm"<<endl;
    cout<<"Entrer votre choix : "<<endl;
    cin>>choix;
  }
  switch (choix) {
  case 1 :
    test_morpho();
	  break;
  case 2 :
    grains_de_riz();
    break;
  }
}

void exercice1(){
  //le masque de chanfrein (vous pouvez le modifier)
  double** masque=new double*[3];
  for(int i=0;i<3;i++){
    masque[i]=new double[3];
  }
  masque[0][0]=masque[0][2]=masque[2][0]=masque[2][2]=4;
  masque[0][1]=masque[1][0]=masque[2][1]=masque[1][2]=3;

  //la dimension du masque de Chanfrein : n*n
  int n=3;

  char ficE[100];
  char ficS[100];
  cout<<"----------------------------------"<<endl;
  cout<<"------------EXERCICE 1------------"<<endl;
  cout<<"----------------------------------"<<endl;
  cout<<"1 : Seuillage"<<endl;
  cout<<"2 : Negatif"<<endl;
  cout<<"3 : Compter le nombre de composantes connexes"<<endl;
  cout<<"4 : Calculer la transformée en distance de l'image"<<endl;
  cout<<"5 : Calculer l'axe médian"<<endl;
  cout<<"6 : Reconstruire l'image binaire à partir de son axe médian"<<endl;
  cout<<"7 : Extraire les caractéristiques des objets"<<endl;
  cout<<"Entrer votre choix : ";
  int choix;
  cin>>choix;

  while(choix<1 || choix>7){
    cout<<"Choix incorrect..."<<endl<<endl;
    cout<<"1 : Seuillage"<<endl;
    cout<<"2 : Negatif"<<endl;
    cout<<"3 : Compter le nombre de composantes connexes"<<endl;
    cout<<"4 : Calculer la transformée en distance de l'image"<<endl;
    cout<<"5 : Calculer l'axe médian"<<endl;
    cout<<"6 : Reconstruire l'image binaire à partir de son axe médian"<<endl;
    cout<<"7 : Extraire les caractéristiques des objets"<<endl;
    cout<<"Entrer votre choix : ";
    cout<<"Entrer votre choix : "<<endl;
    cin>>choix;
  }

  cout<<"Entrez le nom du fichier à traiter :"<<endl;
  cin>>ficE;
  image imE(ficE);

  if(choix!=3){
    cout<<"Entrez le nom du fichier de sortie :"<<endl;
    cin>>ficS;
  }

  int seuil;
  image* DT;
  image* AM;
  image* rec;
  switch (choix) {
  case 1 :
    cout<<"Entrez le seuil pour le seuillage"<<endl;
    do{
      cin>>seuil;
    }
    while(seuil<0);
    imE.seuiller(seuil);
    imE.EcrireImagePGM(ficS);
	  break;

  case 2 :
    imE.negatif();
    imE.EcrireImagePGM(ficS);
    break;

  case 3 :
    cout<<"Entrez le seuil pour le seuillage"<<endl;
    do{
      cin>>seuil;
    }
    while(seuil<0);
    cout<<"Il y a "<<imE.nbConnCom(4,seuil)<<" composantes connexes dans l'image."<<endl;
    break;

  case 4 :
    cout<<"Entrez le seuil pour le seuillage"<<endl;
    do{
      cin>>seuil;
    }
    while(seuil<0);
    DT=imE.distanceT(masque,n,seuil);
    DT->EcrireImagePGM(ficS);delete DT;
    break;

  case 5 :
    cout<<"Entrez le seuil pour le seuillage"<<endl;
    do{
      cin>>seuil;
    }
    while(seuil<0);
    AM=imE.axeMedian(masque,n,seuil);
    AM->EcrireImagePGM(ficS);delete AM;
    break;

  case 6 :
    rec=imE.reconsAxeMed(masque,n);
    rec->EcrireImagePGM(ficS);delete rec;
    break;

  case 7:
    cout<<"Entrez le seuil pour le seuillage"<<endl;
    do{
      cin>>seuil;
    }
    while(seuil<0);

    imE.writePgmItems(ficS,seuil,masque,n);
    break;
  }


}

int main(int argc, char* argv[]){
  using namespace std;
  exercice1();
  return 0;

}
