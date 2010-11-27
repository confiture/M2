#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.hpp"
#include "NewpixmapIO.hpp"

int main(int argc, char** argv)
{
	int *image;
	int rows, cols, type;
	image = readPixmap(argv[1], &type, &cols, &rows);


	float* imageFloat=new float[cols*rows*3];
	for(int i=0;i<rows*cols*3;i++){
		imageFloat[i]=image[i];
	}

	writePixmap(image,cols,rows,type,"testinit.pgm");

	normalise(imageFloat, rows, cols,0,3);

	for(int i=0;i<rows*cols*3;i++){
		image[i]=imageFloat[i];
	}



	writePixmap(image,cols,rows,type,"test.ppm");

	return 0;
}
