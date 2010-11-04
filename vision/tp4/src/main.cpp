#include "image.hpp"

int main(int argc, char* argv[]){
  
  
  image im(argv[1]);
  
  // im.EcrireImagePGM("test.pgm");
  // Exercice 2 //
 
  image* Ix=im.contourX2();
  image* Iy=im.contourY2();
  image* Ixy=im.contourXY();
  
  // Lissage
  
  image* imGauss= im.GaussFilter();
  imGauss->EcrireImagePGM(argv[2]);
  //image* Iy=im.contourY();
 //contourX(const image & im,int rows,int cols);
  return 0;
}