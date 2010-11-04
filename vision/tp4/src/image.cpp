#include "image.hpp"

using namespace std;

int& image::operator()(int i,int j){
	return buffer[i*largeur+j];
}

int image::operator()(int i,int j)const{
	return buffer[i*largeur+j];
}

image::image(int hauteur, int largeur,int valmax){
	this->buffer = new int[hauteur*largeur];
	this->hauteur=hauteur;
	this->largeur=largeur;
	this->valmax=valmax;
}

image::image(char* nomFichier){
	/* Ouverture */
	FILE* ifp = fopen(nomFichier,"r");

	/* Lecture du Magic number */
	int ich1 = getc( ifp );
	if ( ich1 == EOF )
		std::cerr<<"EOF / read error reading magic number"<<std::endl;
	int ich2 = getc( ifp );
	if ( ich2 == EOF )
		std::cerr<<"EOF / read error reading magic number"<<std::endl;
	/*if(ich2 != '2'){
	  std::cerr<<" wrong ifp format "<<std::endl;
	  exit(-1);
	  }*/


	/*Lecture des dimensions*/
	largeur = pm_getint( ifp );
	hauteur = pm_getint( ifp );
	valmax = pm_getint( ifp );

	buffer = new int[hauteur*largeur];

	/*Lecture*/
	for(int i=0; i < hauteur; i++){
		for(int j=0; j < largeur ; j++){
			(*this)(i,j)=pm_getrawbyte(ifp);
			//
			//cerr<<(*this)(i,j)<<" ";
			//cerr<<buffer[i*largeur+j];
			//exit(-1);
		}
	}
	/* fermeture */
	fclose(ifp);
}

int image::EcrireImagePGM(char* nomFichier)const{
	char str[50];
	/* Ouverture */
	filebuf fb;
	fb.open(nomFichier,ios::out);
	ostream os(&fb);

	/* Ecriture */
	os<<"P2"<<endl;
	os<<largeur<<" "<<hauteur<<endl;
	os<<valmax<<endl;

	for(int i=0; i < hauteur; i++){
		for(int j=0; j < largeur ; j++){
			//cerr<<(i,j)<<" ";
			// exit(-1);
			os<<(*this)(i,j);
			//cerr<<(*this)(i,j)<<" ";
			os<<std::endl;
			//sprintf(str,"%u ",(*this)(i,j));
			//os<<str;
			//cerr<<str<<" ";
		}
	}
	fb.close();
	return 1;

}

/**
 *On applique le filtre filter de dimensions p*p sur la positions (pix_i,pix_j)
 *à partir de l'image courante sur l'image de sortie passée en paramètre.
 */
void image::ApplyFilter(int p,float** filter,int pix_i,int pix_j,image & sortie){
	int pos = (p-1)/2;
	int k,l;
	float val=0;
	sortie(pix_i,pix_j)=0;
	for(k=pix_i-pos;k<=pix_i+pos;k++){
		for(l=pix_j-pos;l<=pix_j+pos;l++){
			val+=(float)(*this)(k,l)*filter[k+pos-pix_i][l+pos-pix_j];
		}
	}
	sortie(pix_i,pix_j)=val;
}



image* image::HarrisFilter(double alpha){
	image* gauss=GaussFilter();
	image* Ix2=gauss->contourX2();
	image* Iy2=gauss->contourY2();
	image* Ixy=gauss->contourXY();

	image* Ix2g=Ix2->GaussFilter();
	image* Iy2g=Iy2->GaussFilter();
	image* Ixyg=Ixy->GaussFilter();

	delete gauss;delete Ix2;delete Iy2;delete Ixy;

	image* sortie=new image(hauteur,largeur,0);
	for(int i=1;i<hauteur;i++){
		for(int j=1;j<largeur;j++){
			(*this)(i,j)=(*Ix2g)(i,j)*(*Iy2g)(i,j)-
				alpha*((*Ix2g)(i,j)+(*Ix2g)(i,j))*((*Ix2g)(i,j)+(*Ix2g)(i,j));
		}
	}

	return sortie;
}


image* image::GaussFilter(){
	image* sortie = new image(hauteur,largeur,valmax);
	float** filter=new float*[3];
	int i;
	int j;

	for(i=0;i<3;i++)filter[i]=new float[3];
	filter[0][0]=filter[0][2]=filter[2][0]=filter[2][2]=1.0/16;
	filter[0][1]=filter[1][0]=filter[1][2]=filter[2][1]=2.0/16;
	filter[1][1]=4.0/16;

	for(i=1;i<hauteur-1;i++){
		for(j=1;j<largeur-1;j++){
			ApplyFilter(3,filter,i,j,(*sortie));
		}
	}
	for(i=0;i<3;i++)delete [] filter[i];
	delete [] filter;

	return sortie;
}

image* image::contourX(){
	image* sortie = new image(hauteur,largeur,valmax);
	float** filter=new float*[3];
	int i;
	int j;

	for(i=0;i<3;i++)filter[i]=new float[3];
	filter[0][0]=filter[2][0]=-1.0;
	filter[0][2]=filter[2][2]=1.0;
	filter[1][0]=-2.0;
	filter[1][2]=2.0;
	filter[0][1]=filter[1][1]=filter[2][1]=0.0;
	for(i=1;i<hauteur-1;i++){
		for(j=1;j<largeur-1;j++){
			ApplyFilter(3,filter,i,j,(*sortie));
			//cerr<<sortie[i*cols+j]<<endl;
		}
	}
	for(i=0;i<3;i++)delete [] filter[i];
	delete [] filter;

	return sortie;
}

image* image::contourX2(){
	image* sortie=contourX();
	for(int i=1;i<hauteur-1;i++){
		for(int j=1;j<largeur-1;j++){
			(*sortie)(i,j)=(*sortie)(i,j)*(*sortie)(i,j);
		}
	}

	return sortie;
}

image* image::LissageGaussienX2(){
	image* Ix2=contourX2();
	image* sortie = Ix2->GaussFilter();

	delete Ix2;
	return sortie;
}

image* image::LissageGaussienY2(){
	image* Iy2=contourY2();
	image* sortie = Iy2->GaussFilter();
	return sortie;
}

image* image::LissageGaussienXY(){
	image* Ixy=contourXY();
	image* sortie = Ixy->GaussFilter();
	return sortie;
}
image* image::contourY2(){
	image* sortie=contourY();
	for(int i=1;i<hauteur-1;i++){
		for(int j=1;j<largeur-1;j++){
			(*sortie)(i,j)=(*sortie)(i,j)*(*sortie)(i,j);
		}
	}

	return sortie;
}

image* image::contourXY(){
	image* sortie = new image(hauteur,largeur,valmax);
	image* sortieX=contourX();
	image* sortieY=contourY();
	for(int i=1;i<hauteur-1;i++){
		for(int j=1;j<largeur-1;j++){
			(*sortie)(i,j)=(*sortieX)(i,j)*(*sortieY)(i,j);
		}
	}
	return sortie;
}

image* image::contourY(){
	image* sortie = new image(hauteur,largeur,valmax);
	float** filter=new float*[3];
	int i;
	int j;

	for(i=0;i<3;i++)filter[i]=new float[3];
	filter[0][0]=filter[0][2]=-1.0;
	filter[2][0]=filter[2][2]=1.0;
	filter[0][1]=-2.0;
	filter[2][1]=2.0;
	filter[1][0]=filter[1][1]=filter[1][2]=0.0;
	for(i=1;i<hauteur-1;i++){
		for(j=1;j<largeur-1;j++){
			ApplyFilter(3,filter,i,j,(*sortie));
			//cerr<<sortie[i*cols+j]<<endl;
		}
	}
	for(i=0;i<3;i++)delete [] filter[i];
	delete [] filter;

	return sortie;
}


