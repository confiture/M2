#include "imagePGM.hpp"
#include "imagePPM.hpp"
/*
void testLecEcr(const char* fic1,const char* fic2){
  image im(fic1);
  im.EcrireImagePPM(fic2);
}

 void testPixelAddition(){
 pixPPM pixA(1,1,100,100,100);
 pixPPM pixB;
 pixB = pixA + pixA;
 std::cout<<"i : "<<pixB.i<<"; ";
 std::cout<<"j : "<<pixB.j<<"; ";
 std::cout<<"R : "<<pixB.valR<<"; ";
 std::cout<<"G : "<<pixB.valG<<"; ";
 std::cout<<"B : "<<pixB.valB<<std::endl;
 }

 void testPixelDivision(){
 pixPPM pixA(3,3,200,200,200);
 pixPPM pixB;
 pixB = pixA/2;
 std::cout<<"i : "<<pixB.i<<"; ";
 std::cout<<"j : "<<pixB.j<<"; ";
 std::cout<<"R : "<<pixB.valR<<"; ";
 std::cout<<"G : "<<pixB.valG<<"; ";
 std::cout<<"B : "<<pixB.valB<<std::endl;
 }
*/

void testkMean(const char* nomFichier1,int k,int niter,const char* nomFichier2){
      imagePGM im(nomFichier1);

      std::list<pixPGM>* group;
      group = im.kMean(k,niter);
      imagePGM sortie(k,group,im.getHauteur(),im.getLargeur());
}

int main(int argc, char* argv[]){
  //testLecEcr(argv[1],argv[2]);
  //testPixelAddition();
  //testPixelDivision();
  testkMean(argv[1],atoi(argv[2]),atoi(argv[3]),argv[4]);
  return 0;
}
