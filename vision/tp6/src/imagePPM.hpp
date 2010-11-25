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

class imagePPM{
public:
  enum color{R,G,B};

  imagePPM(int hauteur, int largeur,int valmax);

  imagePPM(const char* nomFichier);

  imagePPM(const imagePPM & im);

  void updateValmax();

  /**
   *Destructeur.
   */
  inline ~imagePPM(){
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

  pixPPM* initCentroids(int k);

  std::list<pixPPM>* kMean(int k);
private:
  int largeur, hauteur, valmax;
  double* bufferR;
  double* bufferG;
  double* bufferB;
};
