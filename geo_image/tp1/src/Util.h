/* Entete des utilitaires de PBM+ */
/* E.B.  12/98                   */
#include <cassert>
#include <iostream>
typedef unsigned char bit;
typedef unsigned char gray;

char pm_getc(FILE* file);
bit pm_getbit(FILE* file);
unsigned char pm_getrawbyte(FILE* file);
int pm_getint( FILE* file);

void pm_erreur(char *);

float absf(float);
float max(float,float);
int max(int,int);
//gray max(gray,gray);
float mod2PI(float);

template<class T>
void updateEquiv(T & vec,int taille,int a,int b){
  assert(a>-1);
  assert(b>-1);
  int min;
  int max;
  min=a;max=b;

  if(a>b){max=a;min=b;}

  for(int i=0;i<taille;i++){
    if(vec[i]==max)vec[i]=min;
  }
}

template<class T>
T max(T** tab,int n,int m){
  T max=tab[0][0];
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      if(max<tab[i][j])max=tab[i][j];
    }
  }

  return max;
}
