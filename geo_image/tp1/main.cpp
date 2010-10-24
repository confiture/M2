#include "image.hpp"

int main(int argc, char* argv[]){

  image im("objets.pgm");
  im.EcrireImagePGM("test.pgm");
return 0;
}