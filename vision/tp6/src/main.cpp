#include "image.hpp"
#include <list>

using namespace std;


void testLecEcr(const char* fic1,const char* fic2){
  image im(fic1);
  im.EcrireImagePPM(fic2);
}

void testInitCentroids(const char* fic1){
  image im(fic1);
  std::list<pixPPM> l=im.initCentroids(6);
  list<pixPPM>::iterator it=l.begin();
  for(it;it!=l.end();it++){
    cout<<"i "<<it->i<<" j "<<it->j<<endl;
  }

}

int main(int argc, char* argv[]){
  //testLecEcr(argv[1],argv[2]);
  testInitCentroids(argv[1]);
  return 0;
}
