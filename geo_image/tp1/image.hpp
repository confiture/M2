#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include "Util.h"
#include <iostream>
#include <fstream>
#include <streambuf>

class image{
public:

image(int largeur, int hauteur,int valmax);

image(char* nomFichier);

int EcrireImagePGM(char* nomFichier)const;

int& operator()(int i,int j);

int operator()(int i,int j)const;

image LireImagePGM(char* nomFichier);

private:
int largeur, hauteur, valmax;
int* buffer;

};