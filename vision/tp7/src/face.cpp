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


/* calcul d'histogramme des couleurs r,v sur l'image normalisee */
float *histogram(int w, int h, int *image, char * filename)
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
	/* For each pixel, normalize its R, G, B values by its luminance */

	/* TO BE COMPLETED */
  		  //  R(i,j)   //		//  G(i,j)  //			//  B(i,j)  //
	lum =image[indppm(i,j,0,w)] + image[indppm(i,j,1,w)] + image[indppm(i,j,2,w)];

	// On divise par R(i,j), G(i,j) et B(i,j) par la luminance lum
	for(k=0;k<3;k++){
	  imageFloat[indppm(i,j,k,w)] =  (float)image[indppm(i,j,k,w)] / lum;
	}
	
	/* TO BE COMPLETED */
			//  R(i,j)   //			// G(i,j)
       }
}
// Normalise imageFloat entre 0 et 255
	normalisePPM(imageFloat, h, w,0,255);
	
	/* imageFloat to image */
	for(int i=0;i<h*w*3;i++){
		image[i]=imageFloat[i]+0.5;
	}
	
/* then update the histogram in RG space */
	int indice;
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
		indice = image[indppm(i,j,0,w)]*256 + image[indppm(i,j,1,w)];
		hist[indice]++;
		}
	}

/* save the histrogram as an image .pgm */
  /* Pour recadrer l'histogramme entre 0 et 255*/
  normalisePGM(hist,256,256,0,255);
  int* histInt=new int[256*256]; 
  floatP2intP(hist,histInt,256*256);
  writePixmap(histInt, 256, 256, 5, filename);

	
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

  prob = (int*) malloc( rows * cols * sizeof(int));

  /*TO BE COMPLETED */

 /* ... ;*/

  /* Calculation of face barycenter */

  /* TO BE COMPLETED */

  /*... ;*/


  /* save the final image results : the image of probability
     + the initial image with the barycenter */

  writePixmap(result, cols, rows, 6, argv[3]);


  free(image);
  free(skin);
  free(prob);
  free(htot);
  free(hskin);
  free(result);
  return 0;
 }




