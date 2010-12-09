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
#include <limits>
#include "pixel.hpp"
#include <list>
#include <cmath>
#include <cassert>

class image{
public:

  image(int hauteur, int largeur,int valmax);

  image(char* nomFichier);

  image(const image & im);

  image(const image & im,int i1,int j1,int n,int m);

  // Fusion de l'image im1 avec l'image im2
  image(const image & im1, const image & im2);

  image(){
    hauteur=0;largeur=0;valmax=0;
  }

  void updateValmax();

  /**
   *Destructeur.
   */
  inline ~image(){
    delete [] buffer;
  }

  int getHauteur();

  int getLargeur();

  int getValmax();

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

  std::list<pixel> best_interest_points(int n,int winn,int winm)const;

  image* GaussFilter();

  image* medianFilter(int n);

  void recadre(double a,double b);

  bool maxLoc(int i,int j);

  void drawCross(int i,int j,int color);

  void drawPts(const std::list<pixel> & Lpix,int col);

  void drawLine(int xi,int yi,int xf,int yf,int color);

  double ssd(int i1,int j1,const image & comp,int i2,int j2,int n, int p)const;

  double zncc(int i1,int j1,const image & comp,int i2,int j2,int n, int p)const;

  double median(int p,int pix_i,int pix_j);

  double moyenne(int i_pix,int j_pix, int n, int p)const;

  double sigma(int i_pix,int j_pix, int n, int p)const;

  pixel** matchPoints(const image & comp,int nbpoints,int winn,int winp,
                      double (image::*score)(int,int,const image &,int,int,int,int)const,bool sim=false)const;

  int** matchProfPoints(const image & comp,int winn,int winp,
                      double (image::*score)(int,int,const image &,int,int,int,int)const,bool sim=false)const;

  image* dblMatchProfPoints(const image & comp,int winn,int winp,
                      double (image::*score)(int,int,const image &,int,int,int,int)const,bool sim=false)const;

  pixel** dblMatchPoints(const image & comp,int nbpoints,int winn,int winp,
                      double (image::*score)(int,int,const image &,int,int,int,int)const,bool sim=false)const;

  image* drawMatchPoints(const image & comp,int nbpoints,int winn,int winp,
                      double (image::*score)(int,int,const image &,int,int,int,int)const,bool sim=false)const;

  image* drawDblMatchPoints(const image & comp,int nbpoints,int winn,int winp,
                            double (image::*score)(int,int,const image &,int,int,int,int)const,bool sim)const;

  image* makeDepth(const image & comp,int winn,int winp,
                   double (image::*score)(int,int,const image &,int,int,int,int)const,bool sim)const;

  image* transInterpol(double di,double dj);

  image& operator=(const image & im);

  friend image operator-(const image & im1,const image & im2);

  friend double residu(const image & im1,const image & im2);

private:
  int largeur, hauteur, valmax;
  double* buffer;

};
