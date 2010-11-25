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

class imagePGM{
public:
  imagePGM(int hauteur, int largeur,int valmax);

  imagePGM(const char* nomFichier);

  imagePGM(const imagePGM & im);

  void updateValmax();

  /**
   *Destructeur.
   */
  inline ~imagePGM(){
    delete [] bufferR;
    delete [] bufferG;
    delete [] bufferB;
  }

  int getHauteur();

  int getLargeur();

  int getValmax();

  int EcrireImagePGM(const char* nomFichier)const;

  double& operator()(int i,int j);

  double operator()(int i,int j)const;

  std::list<pixPPM> initCentroids(int k);

private:
  int largeur, hauteur, valmax;
  double* buffer;
};
