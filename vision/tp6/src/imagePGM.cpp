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

	if(ich2=='5'){
		/*Lecture*/
		for(int i=0; i < hauteur; i++){
			for(int j=0; j < largeur ; j++){
				(*this)(i,j)=pm_getrawbyte(ifp);
			}
		}
		/* fermeture */
	}
	else if(ich2=='2'){
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

imagePGM::imagePGM(int k, std::list<pixPGM> * tab,int hauteur, int largeur){
	buffer = new double[hauteur*largeur];
	(*this).hauteur = hauteur;
	(*this).largeur = largeur;
	valmax=0;
	for(int i=0;i<k;i++){
		if(!tab[i].empty()){
			pixPGM moy = pixPGM::moyenne(tab[i]);
			std::list<pixPGM>::const_iterator it=tab[i].begin();
			for(it;it!=tab[i].end();it++){
				(*this)((*it).i,(*it).j)=moy.val;
			}
			valmax=max(valmax,moy.val+0.5);
		}
	}

}

int imagePGM::EcrireImagePGM(const char* nomFichier)const{
	int pix;
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
			pix=(int)((*this)(i,j)+0.5);
			os<<pix;
			os<<std::endl;
		}
	}
	fb.close();
	return 1;

}


pixPGM* imagePGM::initCentroids(int k)const{
	pixPGM* centroids=new pixPGM[k];;
	int kk;
	if(k%2==1){kk=k+1;}
	else{kk=k;}

	int ind=0;
	int j1=largeur/3;
	int j2=2*largeur/3;
	for(int i=0;i<kk/2-1;i++){
		int posi=hauteur/(kk/2+1)*(i+1);
		pixPGM pix1(posi,j1,(*this)(posi,j1));
		pixPGM pix2(posi,j2,(*this)(posi,j2));
		centroids[ind]=pix1;
		ind++;
		centroids[ind]=pix2;
		ind++;
	}
	if(k%2==1){
		int posi=hauteur/(kk/2+1)*kk/2;
		int j=largeur/2;
		pixPGM pix(posi,j,(*this)(posi,j));
		centroids[ind]=pix;
	}
	else{
		int posi=hauteur/(kk/2+1)*kk/2;
		pixPGM pix1(posi,j1,(*this)(posi,j1));
		pixPGM pix2(posi,j2,(*this)(posi,j2));
		centroids[ind]=pix1;
		ind++;
		centroids[ind]=pix2;
		ind++;
	}

	return centroids;
}

pixPGM* imagePGM::randInitCentroids(int k,int seed)const{
	srand(seed);
	pixPGM* repres=new pixPGM[k];
	for(int j=0;j<k;j++){
		int pixi=rand()%hauteur;
		int pixj=rand()%largeur;
		repres[j].i=pixi;
		repres[j].j=pixj;
		repres[j].val=(*this)(pixi,pixj);
	}

	return repres;
}

std::list<pixPGM>* imagePGM::kMean(int k,pixPGM* repres,int niter)const{
	std::list<pixPGM>* groups=new std::list<pixPGM>[k];

	// Boucle principale
	for(int l=0;l<niter;l++){
		for(int i=0;i<k;i++){
			groups[i].clear();
		}
		for(int i=0;i<hauteur;i++){
			for(int j=0;j<largeur;j++){
				pixPGM pix_courant(i,j,(*this)(i,j));
				double dist_test=numeric_limits<double>::infinity();
				int ind_pix_repres = 0;
				for(int ind=0;ind<k;ind++){
					double dist = pixPGM::distance2(repres[ind],pix_courant);
					if(dist<dist_test){
						dist_test=dist;
						ind_pix_repres = ind;
					}
				}
				groups[ind_pix_repres].push_back(pix_courant);
			}
		}

		for(int i=0;i<k;i++){
			pixPGM pixmoy = pixPGM::moyenne(groups[i]);
			repres[i]=pixmoy;
		}

	}

	return groups;
}

