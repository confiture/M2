#include "imagePGM.hpp"
#include "imagePPM.hpp"
#include <string>

using namespace std;

int main(int argc, char* argv[]){
  //testLecEcr(argv[1],argv[2]);
  //testPixelAddition();
  //testPixelDivision();

  //kMean(atoi(argv[1]),atoi(argv[2]),argv[3],argv[4]);
  //kMeanTrace(atoi(argv[1]),atoi(argv[2]),argv[3]);
  //kMeanRand(atoi(argv[1]),atoi(argv[2]),3,argv[3],argv[4]);

  //test_initCentroids(argv[1]);
  string niterStr("-n");
  string ngroupStr("-g");
  string randStr("-r");
  string inputStr("-i");
  string outputStr("-o");
  char * fici=NULL;
  char * fico="output";
  int niter;
  int ngroup;
  bool randomize=false;
  int seed;

  int argi=1;
  while(argi<argc){
    string currentArg(argv[argi]);
    if(currentArg==niterStr){
      argi++;
      niter=atoi(argv[argi]);
    }
    else if(currentArg==ngroupStr){
      argi++;
      ngroup=atoi(argv[argi]);
    }
    else if(currentArg==randStr){
      randomize=true;
      argi++;
      seed=atoi(argv[argi]);
    }
    else if(currentArg==inputStr){
      argi++;
      fici=argv[argi];
    }
    else if(currentArg==outputStr){
      argi++;
      fico=argv[argi];
    }
    else{
      cout<<"error : unknown parameter "<<currentArg<<endl;
    }

    argi++;
  }


  if(fici==NULL){
    std::cout<<"error : enter an input file name"<<std::endl;
    exit(-1);
  }
  imagePGM im(fici);

  pixPGM* centroids;
  if(randomize){
    centroids=im.randInitCentroids(ngroup,seed);
  }
  else{
    centroids=im.initCentroids(ngroup);
  }

  std::list<pixPGM>* groupes=im.kMean(ngroup,centroids,niter);
  imagePGM sortie(ngroup,groupes,im.getHauteur(),im.getLargeur());
  sortie.EcrireImagePGM(fico);

  return 0;
}
