/* Utilitaires de lecture pour les fichiers PBM, PGM, PPM
**
**
** E.B. 12/98
*/

#include <stdio.h>
#include "Util.h"
#include <cstdlib>

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
void pm_erreur(char *texte)
{
  fprintf(stderr, "\n%s \n\n", texte);
  exit(1);
}

float absf(float x){
	if(x>=0)return x;
	return -x;
}

float max(float a,float b){
	if(a>b)return a;
	return b;
}

int max(int a,int b){
	if(a>b)return a;
	return b;
}

int min(int a,int b){
      if(a<b) return a;
      return b;
}

float mod2PI(float theta){
	const float PI = 3.141592;
	while(theta<=0){
		theta+=2*PI;
	}
	return theta;
}

gray max(gray a,gray b){
	if((int)a>(int)b)return a;
	return b;
}
