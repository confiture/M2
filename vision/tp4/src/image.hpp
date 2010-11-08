#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include "Util.hpp"
#include <iostream>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include "pixel.hpp"
#include <list>

class image{
public:

  image(int hauteur, int largeur,int valmax);

  image(char* nomFichier);

  image(const image & im);

  void updateValmax();

  /**
   *Destructeur.
   */
  inline ~image(){
    delete [] buffer;
  }

  int EcrireImagePGM(char* nomFichier)const;

  double& operator()(int i,int j);

  double operator()(int i,int j)const;

  image LireImagePGM(char* nomFichier);

  void ApplyFilter(int p,double** filter,int pix_i,int pix_j,image & sortie);

  image* contourX();

  image* contourY();

  image* contourX2();
  image* contourY2();
  image* contourXY();

  image* LissageGaussienX2();
  image* LissageGaussienY2();
  image* LissageGaussienXY();

  image* HarrisFilter(double alpha);

  void elim_neg();

  std::list<pixel> best_interest_points(int n);

  image* GaussFilter();

  void recadre(int a,int b);

  bool maxLoc(int i,int j);

  void drawCross(int i,int j,int color);

  void drawPts(const std::list<pixel> & Lpix,int col);

private:
  int largeur, hauteur, valmax;
  double* buffer;

};
