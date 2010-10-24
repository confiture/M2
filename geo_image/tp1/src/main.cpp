#include "image.hpp"

int main(int argc, char* argv[]){

  image im("objets.pgm");
  //im.seuiller(100);
  im.negatif();
  im.EcrireImagePGM("negatif.pgm");
return 0;
}
