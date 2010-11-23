#include "image.hpp"

void testLecEcr(const char* fic1,const char* fic2){
  image im(fic1);
  im.EcrireImagePPM(fic2);
}

int main(int argc, char* argv[]){
  testLecEcr(argv[1],argv[2]);
  return 0;
}
