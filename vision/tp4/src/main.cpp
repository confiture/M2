#include "image.hpp"

void exHarris(char* charfic,double alpha,char* sortiefic){
  image im(charfic);
  image* sortie=im.HarrisFilter(alpha);
  sortie->recadre(0,255);
  sortie->EcrireImagePGM(sortiefic);
  delete sortie;
}

void nMeilPts(char* char0,char * char1,char * char2,double alpha){
  image im(char0);
  image* sortie=im.HarrisFilter(alpha);;

  std::list<pixel> Lpix=sortie->best_interest_points(50,1,1);

  sortie->drawPts(Lpix,255);

  sortie->recadre(0,255);
  sortie->EcrireImagePGM(char1);

  im.drawPts(Lpix,255);
  im.EcrireImagePGM(char2);
}

void test_matchPoints(double alpha,int winn,int winp,char * fic1,char* fic2){
  image im1(fic1);
  image comp(fic2);
  im1.matchPoints(comp,50,winn,winp,&image::ssd,false);
}

void test_drawMatchPoints(double alpha,int winn,int winp,char * fic1,char* fic2,char * fic_s){
  image im1(fic1);
  image im2(fic2);

  image* sortie=im1.drawDblMatchPoints(im2,100,winn,winp,&image::zncc,true);
  sortie->EcrireImagePGM(fic_s);
}


int main(int argc, char* argv[]){

  double alpha;
  sscanf(argv[1],"%f",&alpha);

  //exHarris(argv[1],alpha,argv[3]);

  //nMeilPts(argv[1],argv[3],argv[4],alpha);
  // image im1(argv[1]);
  // image im2(argv[2]);
  // image im(im1,im2);
   //image sortie(im.getHauteur(),im.getLargeur(),im.getValmax());
   //im.EcrireImagePGM(argv[3]);
  //test_matchPoints(alpha,atoi(argv[2]),atoi(argv[3]),argv[4],argv[5]);

  test_drawMatchPoints(alpha,atoi(argv[2]),atoi(argv[3]),argv[4],argv[5],argv[6]);

  //////////////////////////////////
  //////////////////////////////////
  //////////////////////////////////
  //debil(argv[1],argv[2]);

  return 0;
}
