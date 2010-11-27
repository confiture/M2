/**********************************************************************
 *              Histogram and Central Limit Theorem                   *
 **********************************************************************/
#include<stdio.h>   // to read and write on standard output and in files
#include<string.h>  // to find extension of a file
#include<stdlib.h>
#include <limits.h> // to use INT_MIN and INT_MAX
#include <float.h>  // to use DBL_MIN and DBL_MAX
#include "utils.hpp"

/*****************************************************************
 *        To read char, bit, rawbyte and int from a file         *
 *****************************************************************/
char pm_getc(FILE* file)
{
  register int ich;
  register char ch;

  ich = getc( file );
  if ( ich == EOF )
    pm_erreur("EOF / read error" );
  ch = (char) ich;

  if ( ch == '#' )
    {
      do
	{
	  ich = getc( file );
	  if ( ich == EOF )
	    pm_erreur("EOF / read error" );
	  ch = (char) ich;
	}
      while ( ch != '\n' && ch != '\r' );
    }

  return ch;
}


unsigned char pm_getbit(FILE* file)
{
  register char ch;

  do
    {
      ch = pm_getc( file );
    }
  while ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' );

  if ( ch != '0' && ch != '1' )
        pm_erreur("junk in file where bits should be" );

  return ( ch == '1' ) ? 1 : 0;
}


unsigned char pm_getrawbyte(FILE* file)
{
  register int iby;

  iby = getc( file );
  if ( iby == EOF )
    pm_erreur("EOF / read error" );
  return (unsigned char) iby;
}


int pm_getint( FILE* file)
{
  register char ch;
  register int i;

  do
    {
      ch = pm_getc( file );
    }
  while ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' );

  if ( ch < '0' || ch > '9' )
    pm_erreur( "junk in file where an integer should be" );

  i = 0;
  do
    {
      i = i * 10 + ch - '0';
      ch = pm_getc( file );
    }
  while ( ch >= '0' && ch <= '9' );

  return i;
}

int pm_get_signed_int( FILE* file)
{
  register int sign;
  register char ch;
  register int i;

  sign = 1;
  do
    {
      ch = pm_getc( file );
    }
  while ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' );

  if ( (ch < '0' || ch > '9') && (ch != '-') )
    pm_erreur( "junk in file where an integer should be" );

  if (ch == '-') {
    sign = -1;
    ch = pm_getc( file );
    if ( ch < '0' || ch > '9' )
      pm_erreur( "junk in file where an integer should be" );
  }

  i = 0;
  do
    {
      i = i * 10 + ch - '0';
      ch = pm_getc( file );
    }
  while ( ch >= '0' && ch <= '9' );

  i *= sign;

  return i;
}

double pm_get_signed_double( FILE* file)
{
  register int sign;
  register char ch;
  register double i;
  register double v;
  register double nb;

  sign = 1;
  do
    {
      ch = pm_getc( file );
    }
  while ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' );

  if ( (ch < '0' || ch > '9') && (ch != '-') )
    pm_erreur( "junk in file where an integer should be" );

  if (ch == '-') {
    sign = -1;
    ch = pm_getc( file );
    if ( ch < '0' || ch > '9' )
      pm_erreur( "junk in file where an integer should be" );
  }

  i = 0.0;
  do
    {
      i = i * 10.0 + (ch - '0');
      ch = pm_getc( file );
    }
  while ( ch >= '0' && ch <= '9' );
  if (ch == '.') {
    ch = pm_getc( file );
    v = 0.0;
    nb = 1;
    do
      {
	v = v * 10.0 + (ch - '0');
	nb *= 0.1;
	ch = pm_getc( file );
      }
    while ( ch >= '0' && ch <= '9' );
    i += v*nb;
  }

  i *= sign;

  return i;
}

void pm_erreur(char *texte)
{
  fprintf(stderr, "\n%s \n\n", texte);
  exit(1);
}


int pm_getMax(int * pixmap, int nbPixels)
{
  int i;
  int result = INT_MIN;
  for (i = 0; i < nbPixels; i++)
    if (pixmap[i] > result)
      result = pixmap[i];

  return result;
}

int pm_getMin(int * pixmap, int nbPixels)
{
  int i;
  int result = INT_MAX;
  for (i = 0; i < nbPixels; i++)
    if (pixmap[i] < result)
      result = pixmap[i];

  return result;
}

double pm_getDBLMax(double * pixmap, int nbPixels)
{
  int i;
  double result = DBL_MIN;
  for (i = 0; i < nbPixels; i++)
    if (pixmap[i] > result)
      result = pixmap[i];

  return result;
}

double pm_getDBLMin(double * pixmap, int nbPixels)
{
  int i;
  double result = DBL_MAX;
  for (i = 0; i < nbPixels; i++)
    if (pixmap[i] < result)
      result = pixmap[i];

  return result;
}

void pm_setExtension(char * filename, char *ext)
{
  char *s, *t;
  if ((s = strrchr(filename, '.')) == NULL ||
      ((t = strrchr(filename, '\\')) != NULL && s < t)) {
    strcat(filename, "."); strcat(filename, ext);
  }
}

int ind(int i,int j,int ncol){
  return i*ncol+j;
}

int indppm(int i,int j, int k,int ncol){
  return i*ncol*3+j*3+k;
}

void normalisePPM(float* imageFloat, int h, int w,int a, int b){
	float min, max;
	int i, j, k;
	min=max=imageFloat[0];
	//printf("max : %f",max);
	//printf("min : %f",min);
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
		      for(k=0;k<3;k++){
			//printf("val hist : %f \n",hist[i*256+j]);
			    if(min>imageFloat[indppm(i,j,k,w)])min=imageFloat[indppm(i,j,k,w)];
			    if(max<imageFloat[indppm(i,j,k,w)])max=imageFloat[indppm(i,j,k,w)];
		      }
		}
	}
	//printf("max : %f",max);
	//printf("min : %f",min);
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
		    for(k=0;k<3;k++){
          imageFloat[indppm(i,j,k,w)]=((imageFloat[indppm(i,j,k,w)]-min)/
                                     (max-min))*(b-a)+a;
		    }
		}
	}
}

void normalisePGM(float* imageFloat, int h, int w,int a, int b){
	float min, max;
	int i, j;
	min=max=imageFloat[0];
	//printf("max : %f",max);
	//printf("min : %f",min);
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			//printf("val hist : %f \n",hist[i*256+j]);
			    if(min>imageFloat[ind(i,j,w)])min=imageFloat[ind(i,j,w)];
			    if(max<imageFloat[ind(i,j,w)])max=imageFloat[ind(i,j,w)];
		}
	}
	//printf("max : %f",max);
	//printf("min : %f",min);
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			 imageFloat[ind(i,j,w)]=((imageFloat[ind(i,j,w)]-min)/
			              (max-min))*(b-a)+a;
		}
	}
}

void normalisePGM(int* image, int h, int w,int a, int b){
	float min, max;
	int i, j, k;
	min=max=image[0];
	//printf("max : %f",max);
	//printf("min : %f",min);
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			//printf("val hist : %f \n",hist[i*256+j]);
			    if(min>image[ind(i,j,w)])min=image[ind(i,j,w)];
			    if(max<image[ind(i,j,w)])max=image[ind(i,j,w)];
		}
	}
	//printf("max : %f",max);
	//printf("min : %f",min);
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			 image[ind(i,j,w)]=(((double)image[ind(i,j,w)]-(double)min)/
			              ((double)max-(double)min))*(b-a)+(double)a;
		}
	}
}

void floatP2intP(float* tab_f,int* tab_i,int n){
	for(int i=0;i<n;i++)tab_i[i]=tab_f[i]+0.5;
}

void intP2floatP(int* tab_i,float* tab_f,int n){
	for(int i=0;i<n;i++)tab_f[i]=tab_i[i];
}
