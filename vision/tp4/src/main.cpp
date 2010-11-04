#include "image.hpp"

void exHarris(char* charfic,double alpha,char* sortiefic){
  image im(charfic);
  image* sortie=im.HarrisFilter(alpha);
  sortie->recadre(0,255);
  sortie->EcrireImagePGM(sortiefic);
  delete sortie;
}

int main(int argc, char* argv[]){

  double alpha;
  sscanf(argv[2],"%f",&alpha);
  exHarris(argv[1],alpha,argv[3]);

  //image im(argv[1]);
  //im.EcrireImagePGM("test.pgm");
  return 0;
}
