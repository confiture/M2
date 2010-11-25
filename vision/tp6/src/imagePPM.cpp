#include "imagePPM.hpp"

using namespace std;

double& imagePPM::operator()(int i,int j,color c){
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

double imagePPM::operator()(int i,int j,color c)const{
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

int imagePPM::getHauteur(){
	return hauteur;
}

int imagePPM::getLargeur(){
	return largeur;
}

int imagePPM::getValmax(){
	return valmax;
}

void imagePPM::updateValmax(){
	valmax=bufferR[0];
	for(int i=0;i<hauteur*largeur;i++){
		if(bufferR[i]>valmax)valmax=bufferR[i];
		if(bufferG[i]>valmax)valmax=bufferG[i];
		if(bufferB[i]>valmax)valmax=bufferB[i];
	}
}

imagePPM::imagePPM(int hauteur, int largeur,int valmax){
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

imagePPM::imagePPM(const imagePPM & im){
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

imagePPM::imagePPM(const char* nomFichier){
	/* Ouverture */
	FILE* ifp = fopen(nomFichier,"r");

	/* LelistPixcture du Magic number */
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

imagePPM::imagePPM(int k, std::list<pixPPM> * tab,int hauteur, int largeur){
	bufferR = new double[hauteur*largeur];
	bufferG = new double[hauteur*largeur];
	bufferB = new double[hauteur*largeur];
    for(int i=0;i<k;i++){
	pixPPM moy = pixPPM::moyenne(tab[i]);
	std::list<pixPPM>::const_iterator it=tab[i].begin();
	 for(it;it!=tab[i].end();it++){
			(*this)((*it).i,(*it).j,R)=moy.valR;
			(*this)((*it).i,(*it).j,R)=moy.valG;
			(*this)((*it).i,(*it).j,R)=moy.valB;	    	   
	  }	       
    }
    
}

int imagePPM::EcrireImagePPM(const char* nomFichier)const{
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

pixPPM* imagePPM::initCentroids(int k)const{
	pixPPM* centroids=new pixPPM[k];;
	int kk;
	if(k%2==1){kk=k+1;}
	else{kk=k;}

	int ind=0;
	int j1=largeur/3;
	int j2=2*largeur/3;
	for(int i=0;i<kk/2-1;i++){
		std::cout<<"ici 1"<<endl;
		int posi=hauteur/(kk/2+1)*(i+1);
		cout<<posi<<endl;
		pixPPM pix1(posi,j1,(*this)(posi,j1,R),(*this)(posi,j1,G),(*this)(posi,j1,B));
		pixPPM pix2(posi,j2,(*this)(posi,j2,R),(*this)(posi,j2,G),(*this)(posi,j2,B));
		centroids[ind]=pix1;
		ind++;
		centroids[ind]=pix2;
		ind++;
	}

	if(k%2==1){
		std::cout<<"ici 2"<<endl;
		int posi=hauteur/(kk/2+1)*kk/2;
		int j=largeur/2;
		pixPPM pix(posi,j,(*this)(posi,j,R),(*this)(posi,j,G),(*this)(posi,j,B));
		centroids[ind]=pix;
	}
	else{
		std::cout<<"ici 3"<<endl;
		int posi=hauteur/(kk/2+1)*kk/2;
		pixPPM pix1(posi,j1,(*this)(posi,j1,R),(*this)(posi,j1,G),(*this)(posi,j1,B));
		pixPPM pix2(posi,j2,(*this)(posi,j2,R),(*this)(posi,j2,G),(*this)(posi,j2,B));
		centroids[ind]=pix1;
		ind++;
		centroids[ind]=pix2;
		ind++;
	}

	return centroids;
}


std::list<pixPPM>* imagePPM::kMean(int k,int niter)const{
	std::list<pixPPM>* groups=new std::list<pixPPM>[k];
	pixPPM* repres=initCentroids(k);

	for(int iter=0;iter<niter;iter++){
		for(int kind=0;kind<k;k++){
			groups[kind].clear();
		}

		for(int i=0;i<hauteur;i++){
			for(int j=0;j<largeur;j++){
				pixPPM currentPix(i,j,(*this)(i,j,R),(*this)(i,j,G),(*this)(i,j,B));
				int belongInd;//le numéro du représentant auquel appartiendra currentPix
				int dist=numeric_limits<double>::infinity();
				for(int kind=0;kind<k;k++){//on cherche le représentant de currentPix
					int currentDist=pixPPM::distance2(currentPix,repres[kind]);
					if(currentDist<dist){
						belongInd=k;
						dist=currentDist;
					}
				}

				groups[belongInd].push_back(currentPix);
			}
		}

		//on met les représentants à jour
		for(int kind=0;kind<k;k++){
			repres[kind]=pixPPM::moyenne(groups[kind]);
		}
	}

	return groups;
}
