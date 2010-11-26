/*
 * visage.c : calculs divers sur des images de visages
 *
 * E.B. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "NewpixmapIO.h"


/* calcul d'histogramme des couleurs r,v sur l'image normalisee */
float *histogram(int w, int h, int *image, char * filename)
{
  int i, j, k,lum;
  float *hist;

  printf("computation of histogram %s\n", filename);


  hist = (float *) malloc( 256 * 256 * sizeof(float));
  /* Initialisation de hist */
  for(i = 0;i<256;i++){
	for(j=0;j<256;j++){
	hist[i*256+j] = 0 ;
	}	 
   }

  /* First normalize the image by the luminance */

  for(i = 0; i < h ; i++)
    for(j = 0; j < w; j++)
      {

	/* For each pixel, normalize its R, G, B values by its luminance */

	/* TO BE COMPLETED */
                          //  R(i,j)   //		//  G(i,j)  //			//  B(i,j)  //
	lum = image[i * w * 3 + j * 3 + 0] + image[i * w * 3 + j * 3 + 1] + image[i * w * 3 + j * 3 + 2];

	// On divise par R(i,j), G(i,j) et B(i,j) par la luminance lum
	for(k=0;k<w;k++){
	image[i * w * 3 +j * 3 +k] = image[i * w * 3 +j * 3 +k] / lum;	
	printf("k %d\n",k);
	printf("val %d\n", image[i*w*3+j*3+k]);
	 printf("lum: %d\n", lum);
	}
	/* then update the histogram in RG space */

	/* TO BE COMPLETED */
			//  R(i,j)   //			// G(i,j) 
	hist[image[i * w * 3 + j * 3 + 0 ]*256 + image[i * w * 3 + j * 3 + 1 ] ] = hist[image[i * w * 3 + j * 3 + 0 ]*256 + image[i * w * 3 + j * 3 + 1 ]] + 1;
	    
       }

  
	/* normalize the histogram	 */
	  int sum = 0;
	  for(i = 0;i<256;i++){
		for(j=0;j<256;j++){
			sum += hist[i*256+j];
		}
	  }
	
       	for(i = 0;i<256;i++){
		for(j=0;j<256;j++){
			hist[i*256+j]=hist[i*256+j]/sum;
			 printf("sum: %d\n", sum);
		}
	}
 
  /* save the histrogram as an image .pgm */
  
  	 
 int x = writePixmap(hist, 256, 256, 5, filename);
	   
  return hist;
}



void main(int argc, char** argv)
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

  writePixmap(result, cols, rows, 5, argv[3]);


  free(image);
  free(skin);
  free(prob);
  free(htot);
  free(hskin);
  free(result);
 }




