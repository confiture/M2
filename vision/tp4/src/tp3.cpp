#include <stdlib.h>
#include <stdio.h>
#include "Util.h"
//#include "math.h"
#include <cmath>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	FILE* ifp;
	gray* graymap;
	gray* graymap_s;
	int* I_X;
	int* I_Y;
	float* direc;
	int ich1, ich2, rows, cols, maxval, pgmraw, i, j;



	/* Test des arguments */
	if ( argc != 2 ){
		printf("\nUsage : pbmtopgm file \n\n");
		exit(0);
	}

	/* Ouverture */
	ifp = fopen(argv[1],"r");

	/* Lecture du Magic number */
	ich1 = getc( ifp );
	if ( ich1 == EOF )
		pm_erreur( "EOF / read error reading magic number" );
	ich2 = getc( ifp );
	if ( ich2 == EOF )
		pm_erreur( "EOF / read error reading magic number" );
	if(ich2 != '2' && ich2 != '5')
		pm_erreur(" wrong ifp format ");
	else
		if(ich2 == '2')
			pgmraw = 0;
		else pgmraw = 1;

	if(ich2=='2'){ // cas ou l'image est en ASCII
		/*Lecture des dimensions*/
		cols = pm_getint( ifp );
		rows = pm_getint( ifp );
		maxval = pm_getint( ifp );

		/* Allocation memoire  */
		graymap = (gray *) malloc(cols * rows * sizeof(gray));
		graymap_s = (gray *) malloc(cols * rows * sizeof(gray));//image de sortie
		I_X = (int *) malloc(cols * rows * sizeof(int));//image de sortie Ix
		I_Y = (int *) malloc(cols * rows * sizeof(int));//image de sortie Iy
		direc = (float *) malloc(cols * rows * sizeof(float));//image de sortie direction
		/*Lecture*/
		for(i=0; i < rows; i++){
			for(j=0; j < cols ; j++){
				graymap[i*cols+j]=pm_getint(ifp);
				graymap_s[i*cols+j]=graymap[i*cols+j];
			}
		}

		//ApplyMedianFilter(graymap,rows,cols,3,graymap_s);
		//ApplyAdaptatifFilter(graymap,rows,cols,5,graymap_s);
		GaussFilter(graymap,rows,cols,graymap_s);

		for(i=0;i<rows*cols;i++)
			graymap[i]=graymap_s[i];

		/*
		  contourX(graymap,rows,cols,I_X);
		  contourY(graymap,rows,cols,I_Y);
		  ModuleGrad(I_X,I_Y,rows,cols,graymap_s);
		  directionGrad(rows,cols,I_X,I_Y,direc);
		*/

		//QUESTION AVANT-DERNIERE
		extremFilter(graymap,rows,cols,graymap_s);

		/* Ecriture */
		if(pgmraw)
			printf("P2\n");
		else
			printf("P5\n");

		printf("%d %d \n", cols, rows);
		printf("%d\n",maxval);

		for(i=0; i < rows; i++)
			for(j=0; j < cols ; j++)
				printf("%c ",graymap_s[i*cols+j]);


		/* fermeture */
		fclose(ifp);
	}
	
	test3();
	return 0;
}


