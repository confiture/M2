#include "image.hpp"

int main(int argc, char* argv[]){
using namespace std;
  image im("objets.pgm");
  //im.seuiller(100);
  //im.negatif();
  //im.EcrireImagePGM("negatif.pgm");
  //im.dispCompConn("compConn.pgm");
  im.writePgmItems("objets",50);
return 0;
}
