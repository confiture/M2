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
int *histogram(int w, int h, int *image, char * filename)
{
  int i, j, k,lum;
  int *hist;
  float *imageFloat;

  printf("computation of histogram %s\n", filename);

  imageFloat = (float *) malloc(w*h*3*sizeof(float));
  hist = (int *) malloc( 256 * 256 * sizeof(float));
  /* Initialisation de hist */
  for(i = 0;i<256;i++){
	for(j=0;j<256;j++){
	hist[i*256+j] = 0 ;
	}
   }

  /* First normalize the image by the luminance */

  for(i = 0; i < h ; i++){
    for(j = 0; j < w; j++)
      {
	/* For each pixel, normalize its R, G, B values by its luminance */

	/* TO BE COMPLETED */
  		  //  R(i,j)   //		//  G(i,j)  //			//  B(i,j)  //
//	printf("R(i,j) : %i \n",image[i*w*3+j*3+0]);
//	printf("G(i,j) : %i \n",image[i*w*3+j*3+1]);
//	printf("B(i,j) : %i \n",image[i*w*3+j*3+2]);
	lum =image[indppm(i,j,0,w)] + image[indppm(i,j,1,w)] + image[indppm(i,j,2,w)];
//	printf("lum : %i \n",lum);

	// On divise par R(i,j), G(i,j) et B(i,j) par la luminance lum
	for(k=0;k<3;k++){
	  imageFloat[indppm(i,j,k,w)] =  (float)image[indppm(i,j,k,w)] / lum;
//	  printf("val lumi normal : %f \n",imageFloat[i*w*3+j*3+k]);

	}

	/* then update the histogram in RG space */

	/* TO BE COMPLETED */
			//  R(i,j)   //			// G(i,j)
       }
}
	int a = 0;
	int b = 255;
	float min, max;

	min=max=hist[0];
	//printf("max : %f",max);
	//printf("min : %f",min);
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
		  for(k=0;k<3;k++){
			//printf("val hist : %f \n",hist[i*256+j]);
			if(min>imageFloat[i*w*3+j*3+k])min=imageFloat[i*w*3+j*3+k];
			if(max<imageFloat[i*w*3+j*3+k])max=imageFloat[i*w*3+j*3+k];
		  }
		}
	}
	//printf("max : %f",max);
	//printf("min : %f",min);
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
		    for(k=0;k<3;k++){
			 image[i*w*3+j*3+k]=(((double)imageFloat[i*w*3+j*3+k]-(double)min)/
			              ((double)max-(double)min))*(b-a)+(double)a;
			if(image[i*w*3+j*3+k]<256){
			printf("val lumi normal [0,255] : %i \n",image[i*w*3+j*3+k]);
			}
			else{printf("PROBLEMEEEEEEEEEEEEEEEE \n");
			      exit(-1);}
		    }
		}
	}
	printf("h : %i \n",h);
	printf("w : %i \n",w);
	int ind;
	//printf("REGARDE ICI : %i \n",image[160*w*3+247*3+0]*256+image[160*w*3+247*3+1]);
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
	      //  printf("ICI : %i \n",image[i*w*3+j*3+0]*256);
	      //  printf("ICI 2 : %i \n",image[i*w*3+j*3+1]);
		//printf("i : %i \n",i);
		//printf("j : %i \n",j);
		ind = image[i*w*3+j*3+0]*256 + image[i*w*3+j*3+1];
		//printf("IND : %i \n",ind);
		hist[ind]=hist[ind]+1.0;
		//printf("val histo : %f\n",hist[i*256+j]);
		}
	}

	for(i=0;i<256;i++){
		for(j=0;j<256;j++){
	//	printf("val histo : %f\n",hist[i*256+j]);
		}
	}
	/* normalize the histogram	 */
/*	  int sum = 0;
	  for(i = 0;i<256;i++){
		for(j=0;j<256;j++){
		//	printf("val hist : %f \n",hist[i*256+j]);
			sum += hist[i*256+j];
		}
	  }

       	for(i = 0;i<256;i++){
		for(j=0;j<256;j++){
			hist[i*256+j]=hist[i*256+j]/sum;
		}
	}*/

  /* save the histrogram as an image .pgm */
  /* Pour recadrer l'histogramme entre 0 et 255*/

/*	int a = 0;
	int b = 255;
	float min, max;

	min=max=hist[0];
	//printf("max : %f",max);
	//printf("min : %f",min);
	for(i=0;i<256;i++){
		for(j=0;j<256;j++){
			//printf("val hist : %f \n",hist[i*256+j]);
			if(min>hist[i*256+j])min=hist[i*256+j];
			if(max<hist[i*256+j])max=hist[i*256+j];
		}
	}
	//printf("max : %f",max);
	//printf("min : %f",min);
	for(i=0;i<256;i++){
		for(j=0;j<256;j++){
			hist[i*256+j]=(((double)hist[i*256+j]-(double)min)/
			              ((double)max-(double)min))*(b-a)+(double)a;
			//printf("val hist :%d \n",hist[i*256+j]);
		}
	}*/


  writePixmap(hist, 256, 256, 5, filename);

  return hist;
}



int main(int argc, char** argv)
{
  int *image, *result;
  int *skin;

  int rows, cols, type;
  int rowsskin, colsskin, typeskin;

  int *htot, *hskin;

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




