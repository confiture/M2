#include <stdlib.h>
#include <stdio.h>
#include "Util.h"
//#include "math.h"
#include <cmath>
#include <iostream>

using namespace std;
/*

  void test(){
  int tab[9];
  tab[0]=15;
  tab[1]=12;
  tab[2]=25;
  tab[3]=15;
  tab[4]=32;
  tab[5]=8;
  tab[6]=11;
  tab[7]=13;
  tab[8]=1;
  printf("mediane %i",Median(tab,9));
  }
*/
void test3(){
	gray a=3;
	gray b=2;
	std::cerr<<"max(a,b) "<<(int)max(a,b)<<endl;
}

int direction(float theta);
void test2(){
	float PI=3.141;
	cerr<<"mod2PI(-0.5*PI) "<<mod2PI(-0.5*PI)<<endl;
	cerr<<"dir 2 : "<<direction(mod2PI(-0.5*PI))<<endl;
	cerr<<"dir 4 : "<<direction(PI+1e-2)<<endl;
	cerr<<"dir 5 : "<<direction(5*PI/4-1e-1)<<endl;
	cerr<<"dir 5 : "<<direction(5*PI/4-1e-1)<<endl;
}


void ApplyFilter(int p,float** filter,int pix_i,int pix_j,int cols,gray* mat_in,gray* mat_out){
	int pos = (p-1)/2;
	int k,l;
	float val=0;
	mat_out[pix_i*cols+pix_j]=0;
	for(k=pix_i-pos;k<=pix_i+pos;k++){
		for(l=pix_j-pos;l<=pix_j+pos;l++){
			val+=(float)mat_in[k*cols+l]*filter[k+pos-pix_i][l+pos-pix_j];
		}
	}
	mat_out[pix_i*cols+pix_j]=val;
}

void directionGrad(int rows,int cols, int* I_X, int* I_Y, float* direc){
	const float PI = 3.141592;
	for(int i=0;i<rows*cols;i++){
		if(I_X[i]==0){
			if(I_Y[i]<0.0){
				direc[i]=6*PI/4.0;
			}
			else{
				direc[i]=PI/2.0;
			}
		}
		else{
			direc[i]=mod2PI(atan(I_Y[i]/I_X[i]));
		}
	}
}

void ApplyIntFilter(int p,float** filter,int pix_i,int pix_j,int cols,gray* mat_in,int* mat_out){
	int pos = (p-1)/2;
	int k,l;
	float val=0;
	mat_out[pix_i*cols+pix_j]=0;
	for(k=pix_i-pos;k<=pix_i+pos;k++){
		for(l=pix_j-pos;l<=pix_j+pos;l++){
			val+=(float)mat_in[k*cols+l]*filter[k+pos-pix_i][l+pos-pix_j];
		}
	}
	mat_out[pix_i*cols+pix_j]=val;
}


void GaussFilter(gray* image,int rows,int cols,gray* sortie){
	float** filter=(float **)malloc(sizeof(float*)*3);
	int i;
	int j;

	for(i=0;i<3;i++)filter[i]=(float*)malloc(sizeof(float)*3);
	filter[0][0]=filter[0][2]=filter[2][0]=filter[2][2]=1.0/16;
	filter[0][1]=filter[1][0]=filter[1][2]=filter[2][1]=2.0/16;
	filter[1][1]=4.0/16;

	for(i=1;i<rows-1;i++){
		for(j=1;j<cols-1;j++){
			ApplyFilter(3,filter,i,j,cols,image,sortie);
		}
	}
}

void contourX(gray* image,int rows,int cols,int* sortie){
	float** filter=(float **)malloc(sizeof(float*)*3);
	int i;
	int j;

	for(i=0;i<3;i++)filter[i]=(float*)malloc(sizeof(float)*3);
	filter[0][0]=filter[2][0]=-1.0;
	filter[0][2]=filter[2][2]=1.0;
	filter[1][0]=-2.0;
	filter[1][2]=2.0;
	filter[0][1]=filter[1][1]=filter[2][1]=0.0;
	for(i=1;i<rows-1;i++){
		for(j=1;j<cols-1;j++){
			ApplyIntFilter(3,filter,i,j,cols,image,sortie);
		}
	}
}

void contourY(gray* image,int rows,int cols,int* sortie){
	float** filter=(float **)malloc(sizeof(float*)*3);
	int i;
	int j;

	for(i=0;i<3;i++)filter[i]=(float*)malloc(sizeof(float)*3);
	filter[0][0]=filter[0][2]=-1.0;
	filter[2][0]=filter[2][2]=1.0;
	filter[0][1]=-2.0;
	filter[2][1]=2.0;
	filter[1][0]=filter[1][1]=filter[1][2]=0.0;
	for(i=1;i<rows-1;i++){
		for(j=1;j<cols-1;j++){
			ApplyIntFilter(3,filter,i,j,cols,image,sortie);
		}
	}
}

int direction(float theta){
	const float PI = 3.141592;
	int ind_min=0;
	//cerr<<"THETA "<<theta/(PI);
	for(int i=0;i<8;i++)
		if(abs(theta-(float)i*PI/4.0)<abs(theta-(float)ind_min*PI/4.0))ind_min=i;


	//cerr<<"ind_min "<<ind_min<<endl;
	return ind_min;
}

void maxLocDirGrad2(gray* mod_grad,float* theta ,int rows,int cols,gray* sortie){
	float merde;
	for(int i=1;i<rows-1;i++){
		for(int j=1;j<cols-1;j++){
			switch (direction(theta[i*cols+j])){
			case 0:
				if((int)mod_grad[i*cols+j]>=(int)max(mod_grad[i*cols+j-1],mod_grad[i*cols+j+1])){sortie[i*cols+rows]=255;cerr<<"case 0"<<endl;}
				else{cerr<<"!case 0"<<endl;}
				break;
			case 1:
				if((int)mod_grad[i*cols+j]>=(int)max(mod_grad[(i-1)*cols+j-1],mod_grad[(i+1)*cols+j+1])){sortie[i*cols+rows]=255;cerr<<"case 1"<<endl;}
				else{cerr<<"!case 1"<<endl;}
				break;
			case 2:
				if((int)mod_grad[i*cols+j]>=(int)max(mod_grad[(i-1)*cols+j],mod_grad[(i+1)*cols+j])){sortie[i*cols+rows]=255;cerr<<"case 2"
					                                 <<endl;}
				else{cerr<<"!case 2"<<endl;}
				break;
			case 3:
				if((int)mod_grad[i*cols+j]>=(int)max(mod_grad[(i-1)*cols+j-1],mod_grad[(i+1)*cols+j+1])){sortie[i*cols+rows]=255;cerr<<"case 3"<<endl;}
				else{cerr<<"!case 3"<<endl;}
				break;
			case 4:
				if((int)mod_grad[i*cols+j]>=(int)max(mod_grad[i*cols+j-1],mod_grad[i*cols+j+1])){sortie[i*cols+rows]=255;cerr<<"case 4"<<endl;}
				else{cerr<<"!case 4"<<endl;}
				break;
			case 5:
				if((int)mod_grad[i*cols+j]>=(int)max(mod_grad[(i+1)*cols+j-1],mod_grad[(i-1)*cols+j+1])){sortie[i*cols+rows]=255;cerr<<"case 5"<<endl;}
				else{cerr<<"!case 5"<<endl;}
				break;
			case 6:
				if((int)mod_grad[i*cols+j]>=(int)max(mod_grad[(i+1)*cols+j],mod_grad[(i-1)*cols+j])){sortie[i*cols+rows]=255;cerr<<"case 6"<<endl;}
				else{cerr<<"!case 6"<<endl;}
				break;
			case 7:
				if((int)mod_grad[i*cols+j]>=(int)max(mod_grad[(i+1)*cols+j+1],mod_grad[(i-1)*cols+j-1])){sortie[i*cols+rows]=255;cerr<<"case 7"<<endl;}
				else{cerr<<"!case 7"<<endl;}
				break;
			default:
				std::cerr<<"Unknown direction in maxLocDirGrad"<<std::endl;
				exit(-1);
			}
		}
	}
}

void maxLocDirGrad(int* mod_grad,float* theta ,int rows,int cols,gray* sortie){
	for(int i=1;i<rows-1;i++){
		for(int j=1;j<cols-1;j++){
			switch (direction(theta[i*cols+j])){
			case 0:
				if(mod_grad[i*cols+j]>mod_grad[i*cols+j-1] && mod_grad[i*cols+j]>mod_grad[i*cols+j+1]){sortie[i*cols+j]=255;}
				break;
			case 1:
				if(mod_grad[i*cols+j]>mod_grad[(i-1)*cols+j-1] && mod_grad[i*cols+j]>mod_grad[(i+1)*cols+j+1]){sortie[i*cols+j]=255;}
				break;
			case 2:
				if(mod_grad[i*cols+j]>mod_grad[(i-1)*cols+j] && mod_grad[i*cols+j]>mod_grad[(i+1)*cols+j]){sortie[i*cols+j]=255;}
				break;
			case 3:
				if(mod_grad[i*cols+j]>mod_grad[(i-1)*cols+j-1] && mod_grad[i*cols+j]>mod_grad[(i+1)*cols+j+1]){sortie[i*cols+j]=255;}
				break;
			case 4:
				if(mod_grad[i*cols+j]>mod_grad[i*cols+j-1] && mod_grad[i*cols+j]>mod_grad[i*cols+j+1]){sortie[i*cols+j]=255;}
				break;
			case 5:
				if(mod_grad[i*cols+j]>mod_grad[(i+1)*cols+j-1] && mod_grad[i*cols+j]>mod_grad[(i-1)*cols+j+1]){sortie[i*cols+j]=255;}
				break;
			case 6:
				if(mod_grad[i*cols+j]>mod_grad[(i+1)*cols+j] && mod_grad[i*cols+j]>mod_grad[(i-1)*cols+j]){sortie[i*cols+j]=255;}
				break;
			case 7:
				if(mod_grad[i*cols+j]>mod_grad[(i+1)*cols+j+1] && mod_grad[i*cols+j]>mod_grad[(i-1)*cols+j-1]){sortie[i*cols+j]=255;}
				break;
			default:
				std::cerr<<"Unknown direction in maxLocDirGrad"<<std::endl;
				exit(-1);
			}
		}
		}
}


void ModuleGrad(int* X,int* Y,int rows,int cols,int* mod){
	float tab[cols*rows];
	float max=0;
	for(int i=0; i < rows; i++){
		for(int j=0; j < cols ; j++){
			tab[i*cols+j]=sqrt((float)(X[i*cols+j]*X[i*cols+j]) + (float)(Y[i*cols+j]*Y[i*cols+j]));
			if(tab[i*cols+j]>max)max=tab[i*cols+j];
			// Recherche du max
		}
	}
	for(int i=0;i<rows*cols;i++)mod[i]=((float)tab[i])/max*255.0;
}

void extremFilter(gray* image,int rows,int cols,gray* sortie){
	int I_X[rows*cols];
	int I_Y[rows*cols];
	int mod_grad[rows*cols];
	float theta[rows*cols];

	for(int i=0;i<rows*cols;i++)sortie[i]=0;

	contourX(image,rows,cols,I_X);
	contourY(image,rows,cols,I_Y);
	ModuleGrad(I_X,I_Y,rows,cols,mod_grad);
	directionGrad(rows,cols,I_X,I_Y,theta);

	maxLocDirGrad(mod_grad,theta,rows,cols,sortie);
}


int Median(int* tab_in,int dim){
	int tab[dim];
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

int MedianFilter(int p,int pix_i,int pix_j,int cols,gray* mat_in){
	int pos = (p-1)/2;
	int k,l;
	int tab[p*p];
	for(k=pix_i-pos;k<=pix_i+pos;k++){
		for(l=pix_j-pos;l<=pix_j+pos;l++){
			tab[(k-pix_i+pos)*p + (l+pos-pix_j)]=mat_in[k*cols+l];
		}
	}
	return (Median(tab,p*p));
}

void ApplyMedianFilter(gray* image,int rows,int cols,int p,gray* sortie){
	int pos=(p-1)/2;
	int i;
	int j;
	for(i=pos;i<rows-pos;i++){
		for(j=pos;j<cols-pos;j++){
			sortie[i*cols+j]=MedianFilter(p,i,j,cols,image);
		}
	}
}

void AdaptatifFilter(gray* image,int rows,int cols,int pix_i,int pix_j,int p,float** filter){
	float sum=0;
	int pos=(p-1)/2;
	int k;
	int l;
	for(k=pix_i-pos;k<=pix_i+pos;k++){
		for(l=pix_j-pos;l<=pix_j+pos;l++){
			if(image[pix_i*cols+pix_j]!=image[k*cols+l]){
				filter[k-pix_i+pos][l+pos-pix_j]=absf(1.0/(image[pix_i*cols+pix_j]-image[k*cols+l]));
				//printf("filter %f ",filter[k-pix_i+pos][l+pos-pix_j]);
				sum+=filter[k-pix_i+pos][l+pos-pix_j];
			}
			else{
				filter[k-pix_i+pos][l+pos-pix_j]=0.2;
				sum+=0.2;
			}
		}
	}

	for(k=0;k<p;k++)
		for(l=0;l<p;l++)
			filter[k][l]/=sum;
}

void ApplyAdaptatifFilter(gray* image,int rows,int cols,int p,gray* sortie){
	int pos=(p-1)/2;
	int i;
	int j;
	float** filter=(float**)malloc(p*sizeof(float*));
	for(i=0;i<p;i++)filter[i]=(float*)malloc(p*sizeof(float));

	for(i=pos;i<rows-pos;i++){
		for(j=pos;j<cols-pos;j++){
			AdaptatifFilter(image,rows,cols,i,j,p,filter);
			ApplyFilter(p,filter,i,j,cols,image,sortie);
		}
	}
}

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

	else{ // cas ou l'image est en binaire
		/*Lecture des dimensions*/
		cols = pm_getint( ifp );
		rows = pm_getint( ifp );
		maxval = pm_getint( ifp );

		/* Allocation memoire  */
		graymap = (gray *) malloc(cols * rows * sizeof(gray));
		graymap_s = (gray *) malloc(cols * rows * sizeof(gray));//image de sortie
		int gray_s[rows*cols];
		I_X = (int *) malloc(cols * rows * sizeof(int));//image de sortie Ix
		I_Y = (int *) malloc(cols * rows * sizeof(int));//image de sortie Iy
		direc = (float *) malloc(cols * rows * sizeof(float));//image de sortie direction

		/*Lecture*/
		for(i=0; i < rows; i++){
			for(j=0; j < cols ; j++){
				graymap[i*cols+j]=pm_getrawbyte(ifp);
				graymap_s[i*cols+j]=graymap[i*cols+j];
				//gray_s[i*cols+j]=0;
			}
		}
		//ApplyMedianFilter(graymap,rows,cols,3,graymap_s);
		//ApplyAdaptatifFilter(graymap,rows,cols,5,graymap_s);
		GaussFilter(graymap,rows,cols,graymap_s);
		for(i=0;i<rows*cols;i++){
			graymap[i]=graymap_s[i];
			gray_s[i]=graymap_s[i];
		}

		/*
		  contourX(graymap,rows,cols,I_X);
		  contourY(graymap,rows,cols,I_Y);
		  ModuleGrad(I_X,I_Y,rows,cols,gray_s);
		*/

		//QUESTION AVANT-DERNIERE
		extremFilter(graymap,rows,cols,graymap_s);
		//ModuleGrad(I_X,I_Y,rows,cols,graymap_s);


		/* Ecriture */
		if(pgmraw)
			printf("P2\n");
		else
			printf("P5\n");

		printf("%d %d \n", cols, rows);
		printf("%d\n",maxval);

		for(i=0; i < rows; i++)
			for(j=0; j < cols ; j++)
				printf("%u ",graymap_s[i*cols+j]);


		/* fermeture */
		fclose(ifp);
	}

	test3();
	return 0;
}


