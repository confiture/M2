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

void test_kMean(int k,int niter,const char * fic1,const char* fic2){
  imagePPM im1(fic1);
  std::list<pixPPM>* groupes=im1.kMean(k,niter);
  imagePPM sortie(k,groupes,im1.getHauteur(),im1.getLargeur());
  sortie.EcrireImagePPM(fic2);
}


int main(int argc, char* argv[]){
  //testLecEcr(argv[1],argv[2]);
  //testPixelAddition();
  //testPixelDivision();

  test_kMean(atoi(argv[1]),atoi(argv[2]),argv[3],argv[4]);
  return 0;
}
