#include <stdio.h>
#include <stdlib.h>
#include "Util.hpp"

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

float absf(float x){
	if(x>=0)return x;
	return -x;
}


double absd(double x){
	if(x>=0)return x;
	return -x;
}


float max(float a,float b){
	if(a>b)return a;
	return b;
}

float mod2PI(float theta){
	const float PI = 3.141592;
	while(theta<=0){
		theta+=2*PI;
	}
	return theta;
}


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


bit pm_getbit(FILE* file)
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
        pm_erreur("EOF / read error ici" );
    return (unsigned char) iby;
    }

void pm_erreur(char *texte)
{
  fprintf(stderr, "\n%s \n\n", texte);
  exit(1);
}

double medianTab(double* tab_in,int dim){
	double tab[dim];
	int i;
	int j;
	int min_ind;
	int temp;
	for(i=0;i<dim;i++)tab[i]=tab_in[i];
	for(i=0;i<(dim-1)/2+1;i++){
		min_ind=i+1;
		for(j=i+1;j<dim;j++){
			if(tab[j]<tab[min_ind])min_ind=j;
		}
		temp=tab[min_ind];
		tab[min_ind]=tab[i];
		tab[i]=temp;
	}
	return tab[(dim-1)/2];
}

double** invMat22(double** mat){
	double det=mat[0][0]*mat[1][1]-mat[0][1]*mat[1][0];
	double** matInv=new double*[2];
	matInv[0]=new double[2];
	matInv[1]=new double[2];

	matInv[0][0]=mat[1][1]/det;
	matInv[0][1]=-mat[1][0]/det;
	matInv[1][0]=-mat[0][1]/det;
	matInv[1][1]=mat[0][0]/det;

	return matInv;
}
