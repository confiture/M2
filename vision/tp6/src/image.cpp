#include "image.hpp"

using namespace std;

double& image::operator()(int i,int j,color c){
	assert(i>=0);
	assert(i<hauteur);
	assert(j>=0);
	assert(j<largeur);

	switch(c){
	case R:
		return bufferR[i*largeur+j];
		break;
	case G:
		return bufferG[i*largeur+j];
		break;
	case B:
		return bufferB[i*largeur+j];
		break;
	}
}

double image::operator()(int i,int j,color c)const{
	assert(i>=0);
	assert(i<hauteur);
	assert(j>=0);
	assert(j<largeur);

	switch(c){
	case R:
		return bufferR[i*largeur+j];
		break;
	case G:
		return bufferG[i*largeur+j];
		break;
	case B:
		return bufferB[i*largeur+j];
		break;
	}
}

int image::getHauteur(){
	return hauteur;
}

int image::getLargeur(){
	return largeur;
}

int image::getValmax(){
	return valmax;
}

void image::updateValmax(){
	valmax=bufferR[0];
	for(int i=0;i<hauteur*largeur;i++){
		if(bufferR[i]>valmax)valmax=bufferR[i];
		if(bufferG[i]>valmax)valmax=bufferG[i];
		if(bufferB[i]>valmax)valmax=bufferB[i];
	}
}

image::image(int hauteur, int largeur,int valmax){
	this->bufferR = new double[hauteur*largeur];
	this->bufferG = new double[hauteur*largeur];
	this->bufferB = new double[hauteur*largeur];
	this->hauteur=hauteur;
	this->largeur=largeur;
	this->valmax=valmax;
	for(int i=0;i<hauteur*largeur;i++)
	{
		bufferR[i]=valmax;
		bufferG[i]=valmax;
		bufferB[i]=valmax;
	}
}

image::image(const image & im){
	this->bufferR = new double[hauteur*largeur];
	this->bufferG = new double[hauteur*largeur];
	this->bufferB = new double[hauteur*largeur];
	this->hauteur=im.hauteur;
	this->largeur=im.largeur;
	this->valmax=im.valmax;

	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			(*this)(i,j,R)=im(i,j,R);
			(*this)(i,j,G)=im(i,j,G);
			(*this)(i,j,B)=im(i,j,B);
		}
	}
}

image::image(const char* nomFichier){
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

	bufferR = new double[hauteur*largeur];
	bufferG = new double[hauteur*largeur];
	bufferB = new double[hauteur*largeur];

	if(ich2=='6'){
		/*Lecture*/
		for(int i=0; i < hauteur; i++){
			for(int j=0; j < largeur ; j++){
				(*this)(i,j,R)=pm_getrawbyte(ifp);
				(*this)(i,j,G)=pm_getrawbyte(ifp);
				(*this)(i,j,B)=pm_getrawbyte(ifp);
			}
		}
		/* fermeture */
	}
	else if(ich2=='3'){
		for(int i=0; i < hauteur; i++){
			for(int j=0; j < largeur ; j++){
				(*this)(i,j,R)=pm_getint(ifp);
				(*this)(i,j,G)=pm_getint(ifp);
				(*this)(i,j,B)=pm_getint(ifp);
			}
		}
	}
	else{
		std::cout<<"wrong format : P"<<ich2<<std::endl;
		fclose(ifp);
		exit(-1);
	}

	fclose(ifp);
}

int image::EcrireImagePPM(const char* nomFichier)const{
	int pixR;
	int pixG;
	int pixB;
	char str[50];
	/* Ouverture */
	filebuf fb;
	fb.open(nomFichier,ios::out);
	ostream os(&fb);

	/* Ecriture */
	os<<"P3"<<endl;
	os<<largeur<<" "<<hauteur<<endl;
	os<<valmax<<endl;

	for(int i=0; i < hauteur; i++){
		for(int j=0; j < largeur ; j++){
			pixR=(int)((*this)(i,j,R)+0.5);
			pixG=(int)((*this)(i,j,G)+0.5);
			pixB=(int)((*this)(i,j,B)+0.5);
			os<<pixR<<" "<<pixG<<" "<<pixB;;
			os<<std::endl;
		}
	}
	fb.close();
	return 1;

}

