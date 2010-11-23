#include "image.hpp"

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
 

int main(int argc, char* argv[]){
  //testLecEcr(argv[1],argv[2]);
  testPixelAddition();
  
  return 0;
}
