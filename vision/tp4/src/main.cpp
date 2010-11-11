#include "image.hpp"

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

void test_matchPoints(char * fic1,char* fic2,double alpha,int winn,int winp){
  image im1(fic1);
  image comp(fic2);
  im1.matchPoints(comp,50,winn,winp,&image::zncc);
}

void debil(char * fic,char * fic2){
  image im(fic);
  image* gaussien =im.GaussFilter();
  gaussien->EcrireImagePGM(fic2);
}

int main(int argc, char* argv[]){

   double alpha;
  sscanf(argv[2],"%f",&alpha);
//   int nb_interest_pts = atoi(argv[3]);
//   nMeilPts(argv[1],argv[4],alpha,nb_interest_pts);
 
  
  exHarris(argv[1],alpha,argv[3]);

  
  
   //image im1(argv[1]);
   //image im2(argv[2]);
   //image im(im1,im2);
   // im.EcrireImagePGM(argv[3]);
  //test_matchPoints(argv[1],argv[3],alpha,atoi(argv[4]),atoi(argv[5]));

  //////////////////////////////////
  //////////////////////////////////
  //////////////////////////////////
  //debil(argv[1],argv[2]);

  return 0;
}
