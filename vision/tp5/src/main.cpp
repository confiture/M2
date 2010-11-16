#include "image.hpp"


void test_drawMatchPoints(double alpha,int winn,int winp,char * fic1,char* fic2,char * fic_s){
  image im1(fic1);
  image im2(fic2);

  image* sortie=im1.drawMatchPoints(im2,50,winn,winp,&image::zncc,true);
  sortie->EcrireImagePGM(fic_s);
}


void exHarris(char* charfic,double alpha,char* sortiefic){
  image im(charfic);
  image* sortie=im.HarrisFilter(alpha);
  sortie->recadre(0,255);
  sortie->EcrireImagePGM(sortiefic);
  delete sortie;
}

void nMeilPts(char* char0,char * char1,double alpha,int nb_interest_pts){
  image im(char0);
  image* sortie=im.HarrisFilter(alpha);;

  std::list<pixel> Lpix=sortie->best_interest_points(nb_interest_pts,1,1);

  //sortie->drawPts(Lpix,255);

  //sortie->recadre(0,255);
  //sortie->EcrireImagePGM(char1);

  im.drawPts(Lpix,255);
  im.EcrireImagePGM(char1);
}

void test_matchPoints(double alpha,int winn,int winp,char * fic1,char* fic2){
  image im1(fic1);
  image comp(fic2);
  im1.matchPoints(comp,50,winn,winp,&image::zncc,true);
}

void testDepth(int winn,int winp,char * fic1,char * fic2,char * ficSortie){
  image im1(fic1);
  image im2(fic2);

  image* imSortie=im1.makeDepth(im2,winn,winp,&image::ssd,false);
  imSortie->recadre(0,255);
  imSortie->EcrireImagePGM(ficSortie);
}

int main(int argc, char* argv[]){

  testDepth(atoi(argv[1]),atoi(argv[2]),argv[3],argv[4],argv[5]);
  return 0;
}
