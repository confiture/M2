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
#include "pixelPPM.hpp"
#include <list>
#include <cmath>
#include <cassert>

class image{
public:
  enum color{R,G,B};

  image(int hauteur, int largeur,int valmax);

  image(const char* nomFichier);

  image(const image & im);

  void updateValmax();

  /**
   *Destructeur.
   */
  inline ~image(){
    delete [] bufferR;
    delete [] bufferG;
    delete [] bufferB;
  }

  int getHauteur();

  int getLargeur();

  int getValmax();

  int EcrireImagePPM(const char* nomFichier)const;

  double& operator()(int i,int j,color c);

  double operator()(int i,int j,color c)const;

  std::list<pixPPM> initCentroids(int k);

private:
  int largeur, hauteur, valmax;
  double* bufferR;
  double* bufferG;
  double* bufferB;
};
