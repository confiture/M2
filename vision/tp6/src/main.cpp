#include "image.hpp"
#include <list>

using namespace std;


void testLecEcr(const char* fic1,const char* fic2){
  image im(fic1);
  im.EcrireImagePPM(fic2);
}

void testInitCentroids(const char* fic1){
  image im(fic1);
  pixPPM* l=im.initCentroids(6);
  for(int i=0;i<6;i++){
    cout<<"i "<<l[i].i<<" j "<<l[i].j<<endl;
  }

}

int main(int argc, char* argv[]){
  //testLecEcr(argv[1],argv[2]);
  //testPixelAddition();
	testInitCentroids(argv[1]);

  return 0;
}
