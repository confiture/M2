#include "imagePGM.hpp"
#include "imagePPM.hpp"
#include <string>

using namespace std;
/*
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

 void testPixelDivision(){
 pixPPM pixA(3,3,200,200,200);
 pixPPM pixB;
 pixB = pixA/2;
 std::cout<<"i : "<<pixB.i<<"; ";
 std::cout<<"j : "<<pixB.j<<"; ";
 std::cout<<"R : "<<pixB.valR<<"; ";
 std::cout<<"G : "<<pixB.valG<<"; ";
 std::cout<<"B : "<<pixB.valB<<std::endl;
 }
*/

void kMean(int k,int niter,const char * fic1,const char* fic2){
  imagePPM im1(fic1);
  pixPPM* repres=im1.initCentroids(k);
  std::list<pixPPM>* groupes=im1.kMean(k,repres,niter,pixPPM::distance2);
  imagePPM sortie(k,groupes,im1.getHauteur(),im1.getLargeur());
  sortie.EcrireImagePPM(fic2);
}

void kMeanRand(int k,int niter,int seed,const char * fic1,const char* fic2){
  imagePPM im1(fic1);
  pixPPM* repres=im1.randInitCentroids(k,seed);
  std::list<pixPPM>* groupes=im1.kMean(k,repres,niter,pixPPM::distance2);
  imagePPM sortie(k,groupes,im1.getHauteur(),im1.getLargeur());
  sortie.EcrireImagePPM(fic2);
}

void kMeanTrace(int k,int niter,const char * fic1){
  imagePPM im1(fic1);
  pixPPM* repres=im1.initCentroids(k);
  im1.kMeanTrace(k,repres,niter,pixPPM::distance2);
}


void test_initCentroids(const char* fic){
  imagePPM im1(fic);
  pixPPM* repres=im1.initCentroids(5);

  for(int i=0;i<5;i++){
    std::cout<<repres[i].i<<" "<<repres[i].j<<" "<<repres[i].valR
             <<" "<<repres[i].valG<<" "<<repres[i].valB<<std::endl;
  }

  std::cout<<std::endl;
}

int main(int argc, char* argv[]){
  typedef double (*distFunc)(const pixPPM&, const pixPPM&);
  distFunc df;
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
  string distStr("-d");
  string traceStr("-tr");
  string inputStr("-i");
  string outputStr("-o");
  string drawCrossesStr("-cr");
  char * fici=NULL;
  char * fico="output";
  int niter;
  int ngroup;
  int dist;
  bool randomize=false;
  int seed;
  bool trace=false;
  bool drawCrosses=false;

  int argi=1;
  while(argi<argc){
    std::cout<<argi<<endl;
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
    else if(currentArg==distStr){
      argi++;
      dist=atoi(argv[argi]);
      if(dist==3){
        df=&pixPPM::distance2;
      }
      else if(dist==5){
        df=&pixPPM::distComp;
      }
      else{
        std::cout<<"error : unknown distance parameter -d "<<dist<<endl;
        exit(-1);
      }
    }
    else if(currentArg==traceStr){
      trace=true;
    }
    else if(currentArg==inputStr){
      argi++;
      fici=argv[argi];
    }
    else if(currentArg==outputStr){
      argi++;
      fico=argv[argi];
    }
    else if(currentArg==drawCrossesStr){
      argi++;
      drawCrosses=true;
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
  imagePPM im(fici);

  pixPPM* centroids;
  if(randomize){
    std::cout<<"SEED : "<<seed<<std::endl;
    centroids=im.randInitCentroids(ngroup,seed);
  }
  else{
    centroids=im.initCentroids(ngroup);
  }

  if(!trace){
    std::list<pixPPM>* groupes=im.kMean(ngroup,centroids,niter,df);
    imagePPM sortie(ngroup,groupes,im.getHauteur(),im.getLargeur());
    if(drawCrosses){
      for(int i=0;i<ngroup;i++)sortie.drawCross(centroids[i].i,centroids[i].j,
                                            255,0,0);
    }
    sortie.EcrireImagePPM(fico);
  }
  else{
    im.kMeanTrace(ngroup,centroids,niter,df,fico);
  }

  return 0;
}
