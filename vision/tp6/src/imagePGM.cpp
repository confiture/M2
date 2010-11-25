#include "imagePGM.hpp"

using namespace std;

double& imagePGM::operator()(int i,int j){
	assert(i>=0);
	assert(i<hauteur);
	assert(j>=0);
	assert(j<largeur);

	return buffer[i*largeur+j];
}

double imagePGM::operator()(int i,int j)const{
	assert(i>=0);
	assert(i<hauteur);
	assert(j>=0);
	assert(j<largeur);

	return buffer[i*largeur+j];
}

int imagePGM::getHauteur(){
	return hauteur;
}

int imagePGM::getLargeur(){
	return largeur;
}

int imagePGM::getValmax(){
	return valmax;
}

void imagePGM::updateValmax(){
	valmax=buffer[0];
	for(int i=0;i<hauteur*largeur;i++){
		if(buffer[i]>valmax)valmax=buffer[i];
	}
}

imagePGM::imagePGM(int hauteur, int largeur,int valmax){
	this->buffer = new double[hauteur*largeur];
	this->hauteur=hauteur;
	this->largeur=largeur;
	this->valmax=valmax;
	for(int i=0;i<hauteur*largeur;i++)
	{
		buffer[i]=valmax;
	}
}

imagePGM::imagePGM(const imagePGM & im){
	this->buffer = new double[hauteur*largeur];
	this->hauteur=im.hauteur;
	this->largeur=im.largeur;
	this->valmax=im.valmax;

	for(int i=0;i<hauteur;i++){
		for(int j=0;j<largeur;j++){
			(*this)(i,j)=im(i,j);
		}
	}
}

imagePGM::imagePGM(const char* nomFichier){
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

	buffer = new double[hauteur*largeur];

	if(ich2=='6'){
		/*Lecture*/
		for(int i=0; i < hauteur; i++){
			for(int j=0; j < largeur ; j++){
				(*this)(i,j)=pm_getrawbyte(ifp);
			}
		}
		/* fermeture */
	}
	else if(ich2=='3'){
		for(int i=0; i < hauteur; i++){
			for(int j=0; j < largeur ; j++){
				(*this)(i,j)=pm_getint(ifp);
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

int imagePGM::EcrireImagePGM(const char* nomFichier)const{
	int pix;
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
			pix=(int)((*this)(i,j)+0.5);
			os<<pix;
			os<<std::endl;
		}
	}
	fb.close();
	return 1;

}

std::list<pixPGM> imagePGM::initCentroids(int k){
	std::list<pixPGM> centroids;
	int kk;
	if(k%2==1){kk=k+1;}
	else{kk=k;}

	int j1=largeur/3;
	int j2=2*largeur/3;
	for(int i=0;i<kk/2-1;i++){
		std::cout<<"ici 1"<<endl;
		int posi=hauteur/(kk/2+1)*(i+1);
		cout<<posi<<endl;
		pixPGM pix1(posi,j1,(*this)(posi,j1));
		pixPGM pix2(posi,j2,(*this)(posi,j2));
		centroids.push_back(pix1);
		centroids.push_back(pix2);
	}

	if(k%2==1){
		std::cout<<"ici 2"<<endl;
		int posi=hauteur/(kk/2+1)*kk/2;
		int j=largeur/2;
		pixPGM pix(posi,j,(*this)(posi,j));
		centroids.push_back(pix);
	}
	else{
		std::cout<<"ici 3"<<endl;
		int posi=hauteur/(kk/2+1)*kk/2;
		pixPGM pix1(posi,j1,(*this)(posi,j1));
		pixPGM pix2(posi,j2,(*this)(posi,j2));
		centroids.push_back(pix1);
		centroids.push_back(pix2);
	}

	return centroids;
}

