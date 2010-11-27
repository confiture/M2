/*
 * visage.c : calculs divers sur des images de visages
 *
 * E.B.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.hpp"
#include "NewpixmapIO.hpp"
#include <iostream>

/* calcul d'histogramme des couleurs r,v sur l'image normalisee */
float * histogram(int w, int h, int *image, char * filename)
{
	int i, j, k,lum;
	float *hist;
	float *imageFloat;

	printf("computation of histogram %s\n", filename);

	imageFloat = (float *) malloc(w*h*3*sizeof(float));
	hist = (float *) malloc( 256 * 256 * sizeof(float));
	/* Initialisation de hist */
	for(i = 0;i<256;i++){
		for(j=0;j<256;j++){
			hist[i*256+j] = 0.0 ;
		}
	}

	/* First normalize the image by the luminance */

	for(i = 0; i < h ; i++){
		for(j = 0; j < w; j++)
		{
			lum =image[indppm(i,j,0,w)] + image[indppm(i,j,1,w)] + image[indppm(i,j,2,w)];

			// On divise par R(i,j), G(i,j) et B(i,j) par la luminance lum
			if(lum>0){
				for(k=0;k<3;k++){
					imageFloat[indppm(i,j,k,w)] =  255.0*(float)image[indppm(i,j,k,w)] / lum;
				}
			}
		}
	}

	/* imageFloat to image */
	for(int i=0;i<h*w*3;i++){
		image[i]=imageFloat[i]+0.5;
	}

	/* then update the histogram in RG space */
	int indice;
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			indice = image[indppm(i,j,0,w)]*256 + image[indppm(i,j,1,w)];
			hist[indice]=hist[indice]+1;
		}
	}

	/* normalize the histogram	 */
	float sum = 0.0;
	for(i = 0;i<256;i++){
		for(j=0;j<256;j++){
			sum += hist[ind(i,j,256)];
		}
	}

	for(i = 0;i<256;i++){
		for(j=0;j<256;j++){
			hist[ind(i,j,256)]/=sum;
		}
	}

	/* save the histrogram as an image .pgm */
	/* Pour recadrer l'histogramme entre 0 et 255*/
	float* histTemp = new float[256*256];
	int * histInt = new int[256*256];
	for(int i=0;i<256*256;i++){
		histTemp[i]=hist[i];
	}
	normalisePGM(histTemp,256,256,0,255);
	floatP2intP(histTemp,histInt,256*256);

	writePixmap(histInt, 256, 256, 2, filename);

	delete[] histTemp;
	delete[] histInt;
	delete[] imageFloat;
	return hist;
}



int main(int argc, char** argv)
{
	int *image, *result;
	int *skin;

	int rows, cols, type;
	int rowsskin, colsskin, typeskin;

	float *htot, *hskin;

	int *prob;

	int i,j, r, v;
	float val;

	int intmi, intmj, mi= 0, mj = 0, tot = 0;

	/* Test of arguments */
	if ( argc != 4 ){
		printf("\nUsage : face image_face.ppm image_skin.ppm image_result.ppm  \n\n");
		exit(0);
	}

	/* reading image_face */

	image = readPixmap(argv[1], &type, &cols, &rows);
	result = readPixmap(argv[1], &type, &cols, &rows);
	printf("Image type: P%d\n", type);
	printf("Image dimensions: %dx%d\n\n", cols, rows);

	if (!image) {
		printf("Couldn't read %s\n", argv[1]);
		exit(0);
	}

	/* reading image_skin */

	skin = readPixmap(argv[2], &typeskin, &colsskin, &rowsskin);
	printf("Image type: P%d\n", typeskin);
	printf("Image dimensions: %dx%d\n", colsskin, rowsskin);

	if (!skin) {
		printf("Couldn't read %s\n", argv[2]);
		exit(0);
	}



	/* histogram calculation */


	htot = histogram(cols, rows, image, "histoImage.pgm");
	hskin = histogram(colsskin, rowsskin, skin, "histoSkin.pgm");

	/* image of probabilities */
	float* probFloat=new float[rows*cols];
	float* probFloatForint=new float[rows*cols];
	prob = (int*) malloc( rows * cols * sizeof(int));

	/*TO BE COMPLETED */
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			if(htot[image[indppm(i,j,0,cols)]*256+image[indppm(i,j,1,cols)]]==0){
				probFloat[ind(i,j,cols)]=1;
			}
			else if(hskin[image[indppm(i,j,0,cols)]*256+image[indppm(i,j,1,cols)]]==0){
				probFloat[ind(i,j,cols)]=0;
			}
			if(htot[image[indppm(i,j,0,cols)]*256+image[indppm(i,j,1,cols)]]==0){
				probFloat[ind(i,j,cols)]=1;
			}
			else{
				probFloat[ind(i,j,cols)]=hskin[image[indppm(i,j,0,cols)]*256+image[indppm(i,j,1,cols)]]
					/htot[image[indppm(i,j,0,cols)]*256+image[indppm(i,j,1,cols)]];
			}

			probFloatForint[ind(i,j,cols)]=probFloat[ind(i,j,cols)];
		}
	}

	////////////////
	//image de prob
	////////////////
	normalisePGM(probFloatForint,rows,cols,0,255);
	floatP2intP(probFloatForint,prob,rows*cols);

	char* probFile="probs.pgm";
	writePixmap(prob,
	            cols,rows,
	            2,
	            probFile);


	/* Calculation of face barycenter */
	double iBar=0;
	double jBar=0;
	double sum=0;
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			iBar+=i*probFloat[ind(i,j,cols)];
			jBar+=j*probFloat[ind(i,j,cols)];
			sum+=probFloat[ind(i,j,cols)];;
		}
	}

	iBar/=sum;
	jBar/=sum;

	std::cout<<iBar<<" "<<jBar<<std::endl;

	/* save the final image results : the image of probability
	   + the initial image with the barycenter */
	//drawing a cross
	i=iBar+0.5;
	j=jBar+0.5;
	int epais=1;
	int grand=4;
	for(int ii=i-grand;ii<=i+grand;ii++){
		for(int jj=j-epais;jj<=j+epais;jj++){
			if(ii>=0 && ii<rows && jj<cols && jj>=0){
				result[indppm(ii,jj,0,cols)]=255;
				result[indppm(ii,jj,1,cols)]=0;
				result[indppm(ii,jj,2,cols)]=0;
			}
		}
	}

	//l'horizontale
	for(int jj=j-grand;jj<=j+grand;jj++){
		for(int ii=i-epais;ii<=i+epais;ii++){
			if(ii>=0 && ii<rows && jj<cols && jj>=0){
				result[indppm(ii,jj,0,cols)]=255;
				result[indppm(ii,jj,1,cols)]=0;
				result[indppm(ii,jj,2,cols)]=0;
			}
		}
	}







writePixmap(result, cols, rows, 6, argv[3]);


	free(image);
	free(skin);
	free(prob);
	free(htot);
	free(hskin);
	free(result);
	return 0;
}




