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
  
  
  float* imageFloat=new float[cols*rows]; 
  for(int i=0;i<rows*cols;i++){
   imageFloat[i]=image[i];
  }
  
  normalise(imageFloat, rows, cols,0,100);
   for(int i=0;i<rows*cols;i++){
   image[i]=imageFloat[i];
  }
  
  
  
   writePixmap(image,cols,rows,5,"test.pgm");
}